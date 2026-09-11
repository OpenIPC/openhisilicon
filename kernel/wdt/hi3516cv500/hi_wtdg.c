/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2006-2019. All rights reserved.
 * Description: watchdog module's init, exit, hidog_dev register.
 * Author: Hisilicon multimedia software group
 * Create: 2006-12-16
 */

#include "watchdog.h"
#ifdef __HuaweiLite__
#include "pthread.h"
#include <sys/prctl.h>
#endif
#include "hi_osal.h"
#include "hi_wtdg_hal.h"

#ifndef NULL
#define NULL  ((void *)0)
#endif

#define hiwdt_reg(x)    (HIWDT_BASE + (x))

#define HIWDT_LOAD      0x000
#define HIWDT_VALUE     0x004
#define HIWDT_CTRL      0x008
#define HIWDT_INTCLR    0x00C
#define HIWDT_RIS       0x010
#define HIWDT_MIS       0x014
#define HIWDT_LOCK      0xC00

#define HIWDT_UNLOCK_VAL    0x1ACCE551

volatile void *g_wtdg_reg_base = NULL;

#define hiwdt_io_address(x) ((uintptr_t)(g_wtdg_reg_base) + ((x) - (HIWDT_BASE)))

#define hiwdt_readl(x)       osal_readl(hiwdt_io_address(hiwdt_reg(x)))
#define hiwdt_writel(v, x)   osal_writel(v, hiwdt_io_address(hiwdt_reg(x)))

/* debug */
#define HIDOG_PFX "HiDog: "
#define hidog_dbg(params...) osal_printk(HIDOG_PFX params)

/* module param */
#define HIDOG_TIMER_MARGIN    60
int default_margin = HIDOG_TIMER_MARGIN;    /* in seconds */
#define HIDOG_TIMER_DEMULTIPLY  9

int nodeamon = 0;

/* watchdog info */
static struct watchdog_info g_ident = {
    .options =  WDIOF_SETTIMEOUT |
                WDIOF_KEEPALIVEPING |
                WDIOF_MAGICCLOSE,
    .firmware_version = 0,
    .identity = "Hisilicon Watchdog",
};

/* local var */
static struct osal_spinlock g_hidog_lock;
static int g_cur_margin = HIDOG_TIMER_MARGIN;
static int g_need_iounmap = 0;

#ifdef __HuaweiLite__
pthread_t g_task_hidog_deamon = 0;
#else
struct osal_task *g_task_hidog_deamon = NULL;
#endif

#define HIDOG_EXIT      0
#define HIDOG_SELFCLR   1
#define HIDOG_EXTCLR    2

volatile static unsigned int g_hidog_state = 0;
static osal_atomic_t g_driver_open;
static unsigned int g_options = WDIOS_ENABLECARD;
/* Set by a "V" through hidog_write(): the caller means the next close. */
static int g_expect_close = 0;

#ifndef MHZ
#define MHZ (1000 * 1000)
#endif

static const unsigned long g_rate = 3 * MHZ; /* 3MHZ */

/*
 * The counter is loaded with half a margin's worth of ticks, because the
 * SP805 runs the load value twice before it resets anything: reaching zero
 * raises the interrupt and reloads, and only reaching zero a second time with
 * that interrupt still pending drives the reset. Loading a whole margin made
 * every timeout mean twice itself, so nothing a caller set through
 * WDIOC_SETTIMEOUT was the time it got. Same arithmetic as
 * drivers/watchdog/sp805_wdt.c.
 *
 * Half of the counter's range: the load has to be added back to report the
 * time left, and keeping that sum inside 32 bits is what lets the arithmetic
 * stay 32-bit.
 */
#define HIWDT_LOAD_MAX 0x7fffffffU

static unsigned int hidog_max_margin(void)
{
    return (unsigned int)(HIWDT_LOAD_MAX / (g_rate / 2));
}

static unsigned int hidog_load_val(unsigned int nr)
{
    if (nr == 0 || nr > hidog_max_margin()) {
        return HIWDT_LOAD_MAX;
    }

    return (unsigned int)(nr * (g_rate / 2)) - 1;
}

static unsigned int g_load_val = HIWDT_LOAD_MAX;

static void hidog_set_timeout(unsigned int nr)
{
    unsigned int cnt = hidog_load_val(nr);
    unsigned long flags;

    osal_spin_lock_irqsave(&g_hidog_lock, &flags);

    g_load_val = cnt;
    /* unlock watchdog registers */
    hiwdt_writel(HIWDT_UNLOCK_VAL, HIWDT_LOCK);
    hiwdt_writel(cnt, HIWDT_LOAD);
    hiwdt_writel(cnt, HIWDT_VALUE);
    /* lock watchdog registers */
    hiwdt_writel(0, HIWDT_LOCK);
    osal_spin_unlock_irqrestore(&g_hidog_lock, &flags);
}

/*
 * A ping restarts the timer, which is what WDIOC_KEEPALIVE means and all a
 * caller can be expected to assume.
 *
 * This used to read HIWDT_RIS first and return without touching anything when
 * no interrupt was pending -- so a ping did nothing at all until the counter
 * had already run out once, and even then only cleared the interrupt without
 * reloading. The deadline therefore stayed where the previous accepted ping
 * had put it, which made the honest cadence of one ping per half-margin worth
 * exactly one accepted ping per cycle: miss it and a process that had been
 * feeding the device all along got the board reset under it. Writing the load
 * and clearing the interrupt is unconditional in sp805_wdt.c for the same
 * reason.
 */
static void hidog_feed(void)
{
    unsigned long flags;

    osal_spin_lock_irqsave(&g_hidog_lock, &flags);
    /* unlock watchdog registers */
    hiwdt_writel(HIWDT_UNLOCK_VAL, HIWDT_LOCK);
    /* reload the counter, and clear an interrupt if one is already up */
    hiwdt_writel(g_load_val, HIWDT_LOAD);
    hiwdt_writel(0x00, HIWDT_INTCLR);
    /* lock watchdog registers */
    hiwdt_writel(0, HIWDT_LOCK);
    osal_spin_unlock_irqrestore(&g_hidog_lock, &flags);
}

/*
 * Out of range is -EINVAL and the margin in force is left alone, as every
 * other watchdog driver does it. Half-applying a value the caller was told
 * was rejected -- which is what clamping here amounted to -- left the device
 * running at a margin nobody had asked for.
 */
static int hidog_set_heartbeat(int t)
{
    if (t <= 0 || (unsigned int)t > hidog_max_margin()) {
        return -EINVAL;
    }

    g_cur_margin = t;

    hidog_set_timeout(t);
    hidog_feed();

    return 0;
}

static int hidog_keepalive(void)
{
    hidog_feed();
    return 0;
}

static void hidog_start(void)
{
    unsigned long flags;

    osal_spin_lock_irqsave(&g_hidog_lock, &flags);
    /* unlock watchdog registers */
    hiwdt_writel(HIWDT_UNLOCK_VAL, HIWDT_LOCK);
    hiwdt_writel(0x00, HIWDT_CTRL);     /* 0x00: disable watch dog reset signal and interrupt */
    hiwdt_writel(0x00, HIWDT_INTCLR);   /* 0x00: clear interrupt */
    hiwdt_writel(0x03, HIWDT_CTRL);     /* 0x03: enable watch dog reset signal and interrupt */
    /* lock watchdog registers */
    hiwdt_writel(0, HIWDT_LOCK);
    osal_spin_unlock_irqrestore(&g_hidog_lock, &flags);

    g_options = WDIOS_ENABLECARD;
}

static void hidog_stop(void)
{
    unsigned long flags;

    osal_spin_lock_irqsave(&g_hidog_lock, &flags);
    /* unlock watchdog registers */
    hiwdt_writel(HIWDT_UNLOCK_VAL, HIWDT_LOCK);
    /* stop watchdog timer */
    hiwdt_writel(0x00, HIWDT_CTRL);     /* 0x00: disable watch dog reset signal and interrupt */
    hiwdt_writel(0x00, HIWDT_INTCLR);   /* 0x00: clear interrupt */
    /* lock watchdog registers */
    hiwdt_writel(0, HIWDT_LOCK);
    osal_spin_unlock_irqrestore(&g_hidog_lock, &flags);

    hidog_set_timeout(0);
    g_options = WDIOS_DISABLECARD;
}

static int hidog_open(void *private_data)
{
    int ret = 0;

    if (osal_atomic_dec_return(&g_driver_open) != 0) {
        ret = osal_atomic_inc_return(&g_driver_open);
        osal_printk("Error: device already open:%d.\n", ret);
        return -1;
    }

    /*
     * The kernel-side feeder exists to cover the window between module load,
     * where hidog_init() arms the hardware, and the first open. Once
     * userspace has taken the device that window is over for good, so this
     * transition is one-way: hidog_release() does not put it back. It used
     * to, and that is why the watchdog never reset anything -- the process
     * being guarded died, its descriptor was closed for it, and the driver
     * itself went on feeding the dog for the rest of the board's life. See
     * OpenIPC/firmware#1803.
     */
    g_hidog_state = HIDOG_EXTCLR;
    g_expect_close = 0;

    /*
     * "When the device is opened, the watchdog is started" -- the API says so
     * unconditionally, and that held here only for as long as nothing had
     * stopped the counter. A magic close, or WDIOS_DISABLECARD, clears
     * HIWDT_CTRL, and neither the ping below nor WDIOC_SETTIMEOUT puts it
     * back.
     */
    hidog_start();
    hidog_keepalive();

    return ret;
}

static int hidog_release(void *private_data)
{
    if (osal_atomic_inc_return(&g_driver_open) != 1) {
        osal_atomic_dec_return(&g_driver_open);
        return 0;
    }

    /*
     * WDIOF_MAGICCLOSE is advertised, and it means a close without a
     * preceding "V" leaves the timer running and unfed -- the reset the
     * caller is owed when the process being guarded dies.
     */
    if (g_expect_close) {
        hidog_stop();
        g_expect_close = 0;
    } else if (g_options & WDIOS_ENABLECARD) {
        osal_printk("Unexpected close, watchdog left running!\n");
    }

    if (g_options == WDIOS_DISABLECARD) {
        osal_printk("Watchdog is disabled!\n");
    }
    return 0;
}

/*
 * g_ident.options has advertised WDIOF_MAGICCLOSE all along while there was
 * no write handler at all, so a caller doing the documented thing -- write
 * "V", then close, to say the shutdown is deliberate -- got EINVAL from the
 * write and no way at all to stop the device it had started. A write is also
 * the other documented way to ping.
 */
static int hidog_write(const char *data, int len, long *ppos, void *private_data)
{
    if (len) {
        int i;

        g_expect_close = 0;

        for (i = 0; i != len; i++) {
            char c;
            if (osal_copy_from_user(&c, data + i, sizeof(char))) {
                return -EFAULT;
            }
            if (c == 'V') {
                g_expect_close = 1;
            }
        }
        hidog_keepalive();
    }

    return len;
}

static long hidog_ioctl(unsigned int cmd, unsigned long arg, void *private_data)
{
    void *argp = (void *)(uintptr_t)arg;
    int new_margin;
    unsigned int new_options;

    switch (cmd) {
        case WDIOC_GETSUPPORT:
            osal_memcpy(argp, &g_ident, sizeof(g_ident));
            return 0;

        case WDIOC_GETSTATUS:
        case WDIOC_GETBOOTSTATUS:
            osal_memcpy((int *)argp, &g_options, sizeof(int));
            return 0;

        case WDIOC_KEEPALIVE:
            hidog_keepalive();
            return 0;

        case WDIOC_SETTIMEOUT: {
            int ret;

            osal_memcpy(&new_margin, (int *)argp, sizeof(int));

            ret = hidog_set_heartbeat(new_margin);
            if (ret) {
                return ret;
            }
            hidog_keepalive();
            /* Falls through to GETTIMEOUT, as the watchdog core does: the
             * caller reads back the margin the device is running at. */
            osal_memcpy((int *)argp, &g_cur_margin, sizeof(int));
            return 0;
        }

        case WDIOC_GETTIMEOUT:
            osal_memcpy((int *)argp, &g_cur_margin, sizeof(int));
            return 0;

        case WDIOC_GETTIMELEFT: {
            unsigned long flags;
            unsigned long left;
            int secs;

            osal_spin_lock_irqsave(&g_hidog_lock, &flags);
            left = hiwdt_readl(HIWDT_VALUE);
            /* The counter is loaded with half a margin and run twice, so with
             * no interrupt pending there is still a whole load to go after
             * this one. */
            if (!(hiwdt_readl(HIWDT_RIS) & 0x1)) {
                left += (unsigned long)g_load_val + 1;
            }
            osal_spin_unlock_irqrestore(&g_hidog_lock, &flags);

            secs = (int)(left / g_rate);
            osal_memcpy((int *)argp, &secs, sizeof(int));
            return 0;
        }

        case WDIOC_SETOPTIONS:
            osal_memcpy(&new_options, (int *)argp, sizeof(int));

            if (new_options & WDIOS_ENABLECARD) {
                hidog_start();
                hidog_set_heartbeat(g_cur_margin);
                return 0;
            } else if (new_options & WDIOS_DISABLECARD) {
                hidog_stop();
                return 0;
            } else {
                return -EINVAL;
            }

        default:
            return -ENOTTY;
    }
}

/* Kernel Interfaces */
static struct osal_fileops g_hidog_fops = {
    .unlocked_ioctl = hidog_ioctl,
    .open           = hidog_open,
    .release        = hidog_release,
    .write          = hidog_write,
};

static struct osal_dev *g_hidog_miscdev = NULL;

static char g_banner[] = "Hisilicon Watchdog Timer: 0.01 initialized. default_margin=%d sec (nodeamon= %d)\n";

#define HIDOG_DEAMON_MAX_SLEEP_MS 30000

/*
 * Feeds the dog from module load until the first open, and never again --
 * hidog_open() latches HIDOG_EXTCLR. Anything else would mean the driver
 * guarding itself.
 */
static int hidog_deamon(void *data)
{
    unsigned int period;

#ifdef __HuaweiLite__
    prctl(PR_SET_NAME, "hidog_deamon", 0, 0, 0);
#endif
    while (g_hidog_state != HIDOG_EXIT) {
        switch (g_hidog_state) {
            case HIDOG_SELFCLR:
                if (g_options & WDIOS_ENABLECARD) {
                    hidog_feed();
                }
                break;
            case HIDOG_EXTCLR:
                break;
            default:
                break;
        }
        /*
         * Half of the margin in force, not half of the module's default.
         * Userspace can change the margin and hand the device back, and a
         * feeder slower than the margin it is feeding resets the board on a
         * cycle nobody configured. Capped so that a long margin does not make
         * module unload wait out a whole sleep.
         */
        period = (unsigned int)g_cur_margin * 1000 / 2;
        if (period > HIDOG_DEAMON_MAX_SLEEP_MS) {
            period = HIDOG_DEAMON_MAX_SLEEP_MS;
        }
        osal_msleep(period + 10);
    }

    return 0;
}

static int hidog_init(void)
{
    hidog_start();

    g_hidog_state = HIDOG_SELFCLR;
    if (!nodeamon) {
#ifdef __HuaweiLite__
        if (pthread_create(&g_task_hidog_deamon, NULL, (void *)hidog_deamon, 0) < 0) {
            osal_printk("create hidog_deamon failed!\n");
            return -1;
        }
#else
        struct osal_task *dog = NULL;

        dog = osal_kthread_create(hidog_deamon, NULL, "hidog");
        if (dog == NULL) {
            osal_printk("create hidog_deamon failed!\n");
            return -1;
        }
        g_task_hidog_deamon = dog;
#endif
    }
    return 0;
}

static int get_margin(void)
{
    /* Check that the default_margin value is within it's range ; if not reset to the default */
    if (hidog_set_heartbeat(default_margin)) {
        default_margin = HIDOG_TIMER_MARGIN;
        hidog_set_heartbeat(HIDOG_TIMER_MARGIN);
        osal_printk("default_margin value must be 0<default_margin<=%u, using %d\n",
            hidog_max_margin(), HIDOG_TIMER_MARGIN);
    }

    return default_margin;
}

static int ptr_ioremap(void)
{
    if (g_wtdg_reg_base == NULL) {
        g_wtdg_reg_base = (volatile void *)osal_ioremap(HIWDT_BASE, 0x1000); /* 0x1000: watch dog reg length */
        if (g_wtdg_reg_base == NULL) {
            osal_printk("osal_ioremap err. \n");
            osal_spin_lock_destory(&g_hidog_lock);
            osal_atomic_destory(&g_driver_open);
            return -1;
        }
        g_need_iounmap = 1;
    }
    return 1;
}

int watchdog_init(void)
{
    if (osal_atomic_init(&g_driver_open) != 0) {
        osal_printk("Error: init atomic\n");
        return -1;
    }
    osal_atomic_set(&g_driver_open, 1);

    if (osal_spin_lock_init(&g_hidog_lock) != 0) {
        osal_printk("function %s line %u failed\n", __FUNCTION__, __LINE__);
        goto spin_lock_init_fail;
    }

    if (ptr_ioremap() != 1) {
        return -1;
    }

    g_cur_margin = get_margin();

    g_hidog_miscdev = osal_createdev("watchdog");
    if (g_hidog_miscdev == NULL) {
        osal_printk("fail to create dev\n");
        goto create_dev_fail;
    }

    g_hidog_miscdev->minor = WATCHDOG_MINOR;
    g_hidog_miscdev->fops = &g_hidog_fops;
    if (osal_registerdevice(g_hidog_miscdev) != 0) {
        osal_printk("fail to register dev\n");
        goto register_dev_fail;
    }

    if (hidog_init() != 0) {
        goto dog_init_fail;
    }

    osal_printk(g_banner, default_margin, nodeamon);
    osal_printk("hiwtdg init ok. ver=%s, %s.\n", __DATE__, __TIME__);
    return 0;

dog_init_fail:
    osal_deregisterdevice(g_hidog_miscdev);
register_dev_fail:
    osal_destroydev(g_hidog_miscdev);
create_dev_fail:
    if (g_need_iounmap) {
        osal_iounmap((void *)g_wtdg_reg_base);
        g_need_iounmap = 0;
        g_wtdg_reg_base = NULL;
    }
    osal_spin_lock_destory(&g_hidog_lock);
spin_lock_init_fail:
    osal_atomic_destory(&g_driver_open);
    return -1;
}

static void hidog_exit(void)
{
    hidog_set_timeout(0);
    hidog_stop();
    g_hidog_state = HIDOG_EXIT;
    if (!nodeamon) {
#ifdef __HuaweiLite__
        pthread_join(g_task_hidog_deamon, NULL);
#else
        struct osal_task *p_dog = g_task_hidog_deamon;
        if (p_dog == NULL) {
            return;
        }

        osal_kthread_destory(p_dog, 1);
#endif
        osal_yield();
    }

#ifdef __HuaweiLite__
    g_task_hidog_deamon = 0;
#else
    g_task_hidog_deamon = NULL;
#endif
}

void watchdog_exit(void)
{
    /*
     * Unregister before tearing anything down. hidog_exit() sets HIDOG_EXIT
     * and then blocks in kthread_stop() waiting for the feeder to notice, and
     * the feeder's loop condition is g_hidog_state -- which hidog_open() sets
     * back to HIDOG_EXTCLR. With the device still registered at that point,
     * one open() racing the unload left the thread looping for ever and rmmod
     * stuck in D state holding module_mutex.
     */
    osal_deregisterdevice(g_hidog_miscdev);
    osal_destroydev(g_hidog_miscdev);

    hidog_exit();
    if (g_need_iounmap) {
        osal_iounmap((void *)g_wtdg_reg_base);
        g_need_iounmap = 0;
        g_wtdg_reg_base = NULL;
    }
    osal_spin_lock_destory(&g_hidog_lock);
    osal_atomic_destory(&g_driver_open);
    osal_printk("hiwtdg exit ok.\n");
}
