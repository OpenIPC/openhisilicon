/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "ot_wtdg.h"
#ifdef __LITEOS__
#include "pthread.h"
#include <sys/prctl.h>
#endif
#include "ot_osal.h"
#include "ot_wtdg_hal.h"

#ifndef NULL
#define NULL  ((void *)0)
#endif

#define ot_wdt_reg(x)    (OT_WDT_BASE + (x))

#define OT_WDT_LOAD      0x000
#define OT_WDT_VALUE     0x004
#define OT_WDT_CTRL      0x008
#define OT_WDT_INTCLR    0x00C
#define OT_WDT_RIS       0x010
#define OT_WDT_MIS       0x014
#define OT_WDT_LOCK      0xC00

#define OT_WDT_UNLOCK_VAL    0x1ACCE551

volatile void *g_wtdg_reg_base = NULL;

#define ot_wdt_io_address(x) ((uintptr_t)(g_wtdg_reg_base) + ((x) - (OT_WDT_BASE)))

#define ot_wdt_readl(x)       (*(const volatile unsigned int *)ot_wdt_io_address(ot_wdt_reg(x)))
#define ot_wdt_writel(v, x)   (*(volatile unsigned int *)ot_wdt_io_address(ot_wdt_reg(x)) = (v))

/* debug */
#define OT_DOG_PFX "Dog: "
#define ot_dog_dbg(params...) osal_printk(OT_DOG_PFX params)

/* module param */
static int g_default_margin = OT_DOG_TIMER_MARGIN;    /* in seconds */

#define OT_DOG_TIMER_DEMULTIPLY  9

static int g_nodeamon = 0;


/* watchdog info */
static const struct watchdog_info g_ident = {
    .options =  WDIOF_SETTIMEOUT |
                WDIOF_KEEPALIVEPING |
                WDIOF_MAGICCLOSE,
    .firmware_version = 0,
    .identity = "Vendor Watchdog",
};

/* local var */
static osal_spinlock g_ot_dog_lock;
static int g_cur_margin = OT_DOG_TIMER_MARGIN;
static int g_need_iounmap = 0;

#ifdef __LITEOS__
static pthread_t g_task_ot_dog_deamon = 0;
#else
static osal_task *g_task_ot_dog_deamon = NULL;
#endif

#define OT_DOG_EXIT      0
#define OT_DOG_SELFCLR   1
#define OT_DOG_EXTCLR    2

static volatile unsigned int g_ot_dog_state = 0;
static osal_atomic g_driver_open;
static unsigned int g_options = WDIOS_ENABLECARD;
/* Set by a "V" through ot_dog_write(): the caller means the next close. */
static int g_expect_close = 0;

#ifndef MHZ
#define MHZ (1000 * 1000)
#endif

static const unsigned long g_rate = 3 * MHZ; /* 3MHZ */

static int g_fiq_support = 0;

volatile void **get_wtdg_reg_base(void)
{
    return &g_wtdg_reg_base;
}

void wtdg_set_module_param(int margin, int daemon, int fiq_support)
{
    g_default_margin = margin;
    g_nodeamon = daemon;
    g_fiq_support = fiq_support;
}

/*
 * The counter is loaded with half a margin's worth of ticks, because the SP805
 * runs the load value twice before it resets anything: reaching zero raises the
 * interrupt and reloads, and only reaching zero a second time with that
 * interrupt still pending drives the reset. Loading a whole margin made every
 * timeout mean twice itself. Same arithmetic as drivers/watchdog/sp805_wdt.c.
 *
 * Half of the counter's range: the load has to be added back to report the
 * time left, and keeping that sum inside 32 bits is what lets the arithmetic
 * stay 32-bit.
 */
#define OT_WDT_LOAD_MAX 0x7fffffffU

static unsigned int ot_dog_max_margin(void)
{
    return (unsigned int)(OT_WDT_LOAD_MAX / (g_rate / 2));
}

static unsigned int ot_dog_load_val(unsigned int nr)
{
    if (nr == 0 || nr > ot_dog_max_margin()) {
        return OT_WDT_LOAD_MAX;
    }

    return (unsigned int)(nr * (g_rate / 2)) - 1;
}

static unsigned int g_load_val = OT_WDT_LOAD_MAX;

static void ot_dog_set_timeout(unsigned int nr)
{
    unsigned int cnt = ot_dog_load_val(nr);
    unsigned long flags;

    osal_spin_lock_irqsave(&g_ot_dog_lock, &flags);

    g_load_val = cnt;
    /* unlock watchdog registers */
    ot_wdt_writel(OT_WDT_UNLOCK_VAL, OT_WDT_LOCK);
    ot_wdt_writel(cnt, OT_WDT_LOAD);
    ot_wdt_writel(cnt, OT_WDT_VALUE);
    /* lock watchdog registers */
    ot_wdt_writel(0, OT_WDT_LOCK);
    osal_spin_unlock_irqrestore(&g_ot_dog_lock, &flags);
}

/*
 * A ping restarts the timer, which is what WDIOC_KEEPALIVE means and all a
 * caller can be expected to assume.
 *
 * This used to read OT_WDT_RIS first and return without touching anything when
 * no interrupt was pending -- so a ping did nothing at all until the counter
 * had already run out once, and even then only cleared the interrupt without
 * reloading. The deadline therefore stayed where the previous accepted ping had
 * put it, which made the honest cadence of one ping per half-margin worth
 * exactly one accepted ping per cycle: miss it and a process that had been
 * feeding the device all along got the board reset under it. Writing the load
 * and clearing the interrupt is unconditional in sp805_wdt.c for the same
 * reason.
 */
void ot_dog_feed(void)
{
    unsigned long flags;

    osal_spin_lock_irqsave(&g_ot_dog_lock, &flags);
    /* unlock watchdog registers */
    ot_wdt_writel(OT_WDT_UNLOCK_VAL, OT_WDT_LOCK);
    /* reload the counter, and clear an interrupt if one is already up */
    ot_wdt_writel(g_load_val, OT_WDT_LOAD);
    ot_wdt_writel(0x00, OT_WDT_INTCLR);
    /* lock watchdog registers */
    ot_wdt_writel(0, OT_WDT_LOCK);
    osal_spin_unlock_irqrestore(&g_ot_dog_lock, &flags);
}

/*
 * Out of range is -EINVAL and the margin in force is left alone, as every other
 * watchdog driver does it. Half-applying a value the caller was told was
 * rejected -- which is what clamping here amounted to -- left the device
 * running at a margin nobody had asked for.
 */
static int ot_dog_set_heartbeat(int t)
{
    if (t <= 0 || (unsigned int)t > ot_dog_max_margin()) {
        return -EINVAL;
    }

    g_cur_margin = t;

    ot_dog_set_timeout(t);
    ot_dog_feed();

    return 0;
}

static int ot_dog_keepalive(void)
{
    ot_dog_feed();
    return 0;
}

static void ot_dog_start(void)
{
    unsigned long flags;

    osal_spin_lock_irqsave(&g_ot_dog_lock, &flags);
    /* unlock watchdog registers */
    ot_wdt_writel(OT_WDT_UNLOCK_VAL, OT_WDT_LOCK);
    ot_wdt_writel(0x00, OT_WDT_CTRL);     /* 0x00: disable watch dog reset signal and interrupt */
    ot_wdt_writel(0x00, OT_WDT_INTCLR);   /* 0x00: clear interrupt */
    ot_wdt_writel(0x03, OT_WDT_CTRL);     /* 0x03: enable watch dog reset signal and interrupt */
    /* lock watchdog registers */
    ot_wdt_writel(0, OT_WDT_LOCK);
    osal_spin_unlock_irqrestore(&g_ot_dog_lock, &flags);

    g_options = WDIOS_ENABLECARD;
}

static void ot_dog_stop(void)
{
    unsigned long flags;

    osal_spin_lock_irqsave(&g_ot_dog_lock, &flags);
    /* unlock watchdog registers */
    ot_wdt_writel(OT_WDT_UNLOCK_VAL, OT_WDT_LOCK);
    /* stop watchdog timer */
    ot_wdt_writel(0x00, OT_WDT_CTRL);     /* 0x00: disable watch dog reset signal and interrupt */
    ot_wdt_writel(0x00, OT_WDT_INTCLR);   /* 0x00: clear interrupt */
    /* lock watchdog registers */
    ot_wdt_writel(0, OT_WDT_LOCK);
    osal_spin_unlock_irqrestore(&g_ot_dog_lock, &flags);

    ot_dog_set_timeout(0);
    g_options = WDIOS_DISABLECARD;
}

static int ot_dog_open(void *private_data)
{
    int ret = 0;

    wtdg_unused(private_data);
    if (osal_atomic_dec_return(&g_driver_open) != 0) {
        ret = osal_atomic_inc_return(&g_driver_open);
        osal_printk("Error: device already open:%d.\n", ret);
        return -1;
    }

    /*
     * The kernel-side feeder exists to cover the window between module load,
     * where ot_dog_init() arms the hardware, and the first open. Once userspace
     * has taken the device that window is over for good, so this transition is
     * one-way: ot_dog_release() does not put it back. It used to, and that is
     * why the watchdog never reset anything -- the process being guarded died,
     * its descriptor was closed for it, and the driver itself went on feeding
     * the dog for the rest of the board's life. See OpenIPC/firmware#1803.
     */
    g_ot_dog_state = OT_DOG_EXTCLR;
    g_expect_close = 0;

    /*
     * "When the device is opened, the watchdog is started" -- the API says so
     * unconditionally, and that held here only for as long as nothing had
     * stopped the counter. A magic close, or WDIOS_DISABLECARD, clears
     * OT_WDT_CTRL, and neither the ping below nor WDIOC_SETTIMEOUT puts it
     * back.
     */
    ot_dog_start();
    ot_dog_keepalive();

    return ret;
}

static int ot_dog_release(void *private_data)
{
    wtdg_unused(private_data);
    if (osal_atomic_inc_return(&g_driver_open) != 1) {
        osal_atomic_dec_return(&g_driver_open);
        return 0;
    }

    /*
     * WDIOF_MAGICCLOSE is advertised, and it means a close without a preceding
     * "V" leaves the timer running and unfed -- the reset the caller is owed
     * when the process being guarded dies.
     */
    if (g_expect_close) {
        ot_dog_stop();
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
 * g_ident.options has advertised WDIOF_MAGICCLOSE all along while there was no
 * write handler at all, so a caller doing the documented thing -- write "V",
 * then close, to say the shutdown is deliberate -- got EINVAL from the write
 * and no way at all to stop the device it had started. A write is also the
 * other documented way to ping.
 */
static int ot_dog_write(const char *buf, int size, long *offset, void *private_data)
{
    wtdg_unused(offset);
    wtdg_unused(private_data);

    if (size) {
        int i;

        g_expect_close = 0;

        for (i = 0; i != size; i++) {
            char c;
            if (osal_copy_from_user(&c, buf + i, sizeof(char))) {
                return -EFAULT;
            }
            if (c == 'V') {
                g_expect_close = 1;
            }
        }
        ot_dog_keepalive();
    }

    return size;
}

static int ot_dog_set_options(unsigned int new_options)
{
    if (new_options & WDIOS_ENABLECARD) {
        ot_dog_start();
        ot_dog_set_heartbeat(g_cur_margin);
        return 0;
    } else if (new_options & WDIOS_DISABLECARD) {
        ot_dog_stop();
        return 0;
    } else {
        return -WDIOS_UNKNOWN;
    }
}

static int ot_dog_ioctl_get_support(unsigned int cmd, void *arg, void *private_data)
{
    wtdg_unused(cmd);
    wtdg_unused(private_data);
    if (memcpy_s(arg, sizeof(struct watchdog_info), &g_ident, sizeof(g_ident)) != EOK) {
        return -1;
    }
    return 0;
}

static int ot_dog_ioctl_get_status(unsigned int cmd, void *arg, void *private_data)
{
    wtdg_unused(cmd);
    wtdg_unused(private_data);
    *(unsigned int *)arg = g_options;
    return 0;
}

static int ot_dog_ioctl_get_boot_status(unsigned int cmd, void *arg, void *private_data)
{
    return ot_dog_ioctl_get_status(cmd, arg, private_data);
}

static int ot_dog_ioctl_keep_alive(unsigned int cmd, void *arg, void *private_data)
{
    wtdg_unused(cmd);
    wtdg_unused(arg);
    wtdg_unused(private_data);
    ot_dog_keepalive();
    return 0;
}

static int ot_dog_ioctl_set_timeout(unsigned int cmd, void *arg, void *private_data)
{
    int ret;

    wtdg_unused(cmd);
    wtdg_unused(private_data);
    ret = ot_dog_set_heartbeat(*(int *)arg);
    if (ret) {
        return ret;
    }
    ot_dog_keepalive();
    /* Falls through to GETTIMEOUT, as the watchdog core does: the caller reads
     * back the margin the device is running at. */
    *(int *)arg = g_cur_margin;
    return 0;
}

static int ot_dog_ioctl_get_timeout(unsigned int cmd, void *arg, void *private_data)
{
    wtdg_unused(cmd);
    wtdg_unused(private_data);
    *(int *)arg = g_cur_margin;
    return 0;
}

static int ot_dog_ioctl_get_time_left(unsigned int cmd, void *arg, void *private_data)
{
    unsigned long flags;
    unsigned long left;

    wtdg_unused(cmd);
    wtdg_unused(private_data);

    osal_spin_lock_irqsave(&g_ot_dog_lock, &flags);
    left = ot_wdt_readl(OT_WDT_VALUE);
    /* The counter is loaded with half a margin and run twice, so with no
     * interrupt pending there is still a whole load to go after this one. */
    if (!(ot_wdt_readl(OT_WDT_RIS) & 0x1)) {
        left += (unsigned long)g_load_val + 1;
    }
    osal_spin_unlock_irqrestore(&g_ot_dog_lock, &flags);

    *(int *)arg = (int)(left / g_rate);
    return 0;
}

static int ot_dog_ioctl_set_options(unsigned int cmd, void *arg, void *private_data)
{
    wtdg_unused(cmd);
    wtdg_unused(private_data);
    return ot_dog_set_options(*(unsigned int *)arg);
}

static osal_ioctl_cmd g_ot_dog_ioctl_cmd_list[] = {
    { WDIOC_GETSUPPORT, ot_dog_ioctl_get_support },
    { WDIOC_GETSTATUS, ot_dog_ioctl_get_status },
    { WDIOC_GETBOOTSTATUS, ot_dog_ioctl_get_boot_status },
    { WDIOC_KEEPALIVE, ot_dog_ioctl_keep_alive },
    { WDIOC_SETTIMEOUT, ot_dog_ioctl_set_timeout },
    { WDIOC_GETTIMEOUT, ot_dog_ioctl_get_timeout },
    { WDIOC_GETTIMELEFT, ot_dog_ioctl_get_time_left },
    { WDIOC_SETOPTIONS, ot_dog_ioctl_set_options },
};

/* Kernel Interfaces */
static osal_fileops g_ot_dog_fops = {
    .open     = ot_dog_open,
    .release  = ot_dog_release,
    .write    = ot_dog_write,
    .cmd_list = g_ot_dog_ioctl_cmd_list,
    .cmd_cnt  = sizeof(g_ot_dog_ioctl_cmd_list) / sizeof(g_ot_dog_ioctl_cmd_list[0]),
};

static osal_dev *g_ot_dog_miscdev = NULL;

#define OT_BANNER_STRING "Watchdog Timer: 0.01 initialized. default_margin = %d sec (nodeamon = %d)\n"

static inline void dog_feed(void)
{
    if (g_options & WDIOS_ENABLECARD) {
        if (g_fiq_support) {
            fiq_dog_feed();
        } else {
            ot_dog_feed();
        }
    }
}

#define OT_DOG_DEAMON_MAX_SLEEP_MS 30000

/*
 * Half of the margin in force, not half of the module's default. Userspace can
 * change the margin and hand the device back, and a feeder slower than the
 * margin it is feeding resets the board on a cycle nobody configured. Capped so
 * that a long margin does not make module unload wait out a whole sleep.
 */
static unsigned int ot_dog_deamon_period(void)
{
    unsigned int period = (unsigned int)g_cur_margin * 1000 / 2;

    if (period > OT_DOG_DEAMON_MAX_SLEEP_MS) {
        period = OT_DOG_DEAMON_MAX_SLEEP_MS;
    }
    return period + 10;
}

/*
 * Feeds the dog from module load until the first open, and never again --
 * ot_dog_open() latches OT_DOG_EXTCLR. Anything else would mean the driver
 * guarding itself.
 */
int ot_dog_deamon(void *data)
{
    wtdg_unused(data);
#ifdef __LITEOS__
    prctl(PR_SET_NAME, "ot_dog_deamon", 0, 0, 0);
#endif
    while (g_ot_dog_state != OT_DOG_EXIT) {
        switch (g_ot_dog_state) {
            case OT_DOG_SELFCLR:
                dog_feed();
                break;
            case OT_DOG_EXTCLR:
                break;
            default:
                break;
        }
        /* sleep; when self feed dog, only use the default margin */
        osal_msleep(ot_dog_deamon_period());
    }

    return 0;
}

static int ot_dog_init(void)
{
    ot_dog_start();

    g_ot_dog_state = OT_DOG_SELFCLR;
    if (!g_nodeamon) {
#ifdef __LITEOS__
        if (pthread_create(&g_task_ot_dog_deamon, NULL, (void *)ot_dog_deamon, 0) < 0) {
            osal_printk("create ot_dog_deamon failed!\n");
            return -1;
        }
#else
        if (g_fiq_support) {
            int i = 0;
            for (i = 0; i < nr_cpu_ids; i++) {
                creat_dog_thread(i);
            }
        } else {
            osal_task *dog = NULL;

            dog = osal_kthread_create(ot_dog_deamon, NULL, "ot_dog", 0);
            if (dog == NULL) {
                osal_printk("create ot_dog_deamon failed!\n");
                return -1;
            }
            g_task_ot_dog_deamon = dog;
        }
#endif
    }
    return 0;
}

static int get_margin(void)
{
    int ret = g_default_margin;

    /* Check that the g_default_margin value is within it's range ; if not reset to the default */
    if (ot_dog_set_heartbeat(g_default_margin)) {
        g_default_margin = OT_DOG_TIMER_MARGIN;
        ot_dog_set_heartbeat(OT_DOG_TIMER_MARGIN);
        osal_printk("default_margin value must be 0<default_margin<=%u, using %d\n",
            ot_dog_max_margin(), OT_DOG_TIMER_MARGIN);
    }

    return ret;
}

static int ptr_ioremap(void)
{
    if (g_wtdg_reg_base == NULL) {
        g_wtdg_reg_base = (volatile void *)osal_ioremap(OT_WDT_BASE, WDT_REGS_LENGTH);
        if (g_wtdg_reg_base == NULL) {
            osal_printk("osal_ioremap err. \n");
            osal_spin_lock_destroy(&g_ot_dog_lock);
            return -1;
        }
        g_need_iounmap = 1;
    }
    return 1;
}

int watchdog_init(void)
{
    if (osal_spin_lock_init(&g_ot_dog_lock) != 0) {
        osal_printk("function %s line %u failed\n", __FUNCTION__, __LINE__);
        return -1;
    }

    if (ptr_ioremap() != 1) {
        return -1;
    }

    osal_atomic_set(&g_driver_open, 1);

    g_cur_margin = get_margin();

    g_ot_dog_miscdev = osal_dev_create("watchdog");
    if (g_ot_dog_miscdev == NULL) {
        osal_printk("fail to create dev\n");
        goto create_dev_fail;
    }

    g_ot_dog_miscdev->minor = WATCHDOG_MINOR;
    g_ot_dog_miscdev->fops = &g_ot_dog_fops;
    if (osal_dev_register(g_ot_dog_miscdev) != 0) {
        osal_printk("fail to register dev\n");
        goto register_dev_fail;
    }

    if (ot_dog_init() != 0) {
        goto dog_init_fail;
    }

    osal_printk(OT_BANNER_STRING, g_default_margin, g_nodeamon);
    osal_printk("ot_wtdg init ok. ver=%s, %s.\n", __DATE__, __TIME__);
    return 0;

dog_init_fail:
    osal_dev_unregister(g_ot_dog_miscdev);
register_dev_fail:
    osal_dev_destroy(g_ot_dog_miscdev);
create_dev_fail:
    osal_atomic_set(&g_driver_open, 0);
    if (g_need_iounmap) {
        osal_iounmap((void *)g_wtdg_reg_base, WDT_REGS_LENGTH);
        g_need_iounmap = 0;
        g_wtdg_reg_base = NULL;
    }
    osal_spin_lock_destroy(&g_ot_dog_lock);
    return -1;
}

static void ot_dog_exit(void)
{
    ot_dog_set_timeout(0);
    ot_dog_stop();
    g_ot_dog_state = OT_DOG_EXIT;
    if (!g_nodeamon) {
#ifdef __LITEOS__
        pthread_join(g_task_ot_dog_deamon, NULL);
#else
        if (g_fiq_support) {
            stop_hrtimer();
            stop_dog_thread();
        } else {
            osal_task *p_dog = g_task_ot_dog_deamon;
            if (p_dog == NULL) {
                return;
            }
            osal_kthread_destroy(p_dog, 1);
        }
#endif
        osal_yield();
    }

#ifdef __LITEOS__
    g_task_ot_dog_deamon = 0;
#else
    g_task_ot_dog_deamon = NULL;
#endif
}

void watchdog_exit(void)
{
    /*
     * Unregister before tearing anything down. ot_dog_exit() sets OT_DOG_EXIT
     * and then waits for the feeder to notice, and the feeder's loop condition
     * is g_ot_dog_state -- which ot_dog_open() sets back to OT_DOG_EXTCLR. With
     * the device still registered at that point, one open() racing the unload
     * left the thread looping for ever and rmmod stuck in D state holding
     * module_mutex. Observed on a gk7205v200 running the same logic.
     */
    osal_dev_unregister(g_ot_dog_miscdev);
    osal_dev_destroy(g_ot_dog_miscdev);

    ot_dog_exit();
    osal_atomic_set(&g_driver_open, 0);
    if (g_need_iounmap) {
        osal_iounmap((void *)g_wtdg_reg_base, WDT_REGS_LENGTH);
        g_need_iounmap = 0;
        g_wtdg_reg_base = NULL;
    }
    osal_spin_lock_destroy(&g_ot_dog_lock);
    osal_printk("ot_wtdg exit ok.\n");
}
