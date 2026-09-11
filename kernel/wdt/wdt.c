/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

/*
 * The OSAL API header is not called the same thing in every vendor tree: the
 * V3 and hi3516cv500 SDKs put it in hi_osal.h and keep an unrelated osal.h
 * beside it, while in the V4/Goke tree osal/include/osal.h is the API. The
 * per-chip kbuild says which by defining WDT_OSAL_HI.
 */
#ifdef WDT_OSAL_HI
#include "hi_osal.h"
#else
#include "osal.h"
#endif
#include <linux/types.h>
#include "watchdog.h"
#ifdef __LITEOS__
#include "pthread.h"
#include <sys/prctl.h>
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

/*
 * define watchdog IO
 *
 * One driver, every HiSilicon/Goke generation that carries this SP805. The
 * per-chip kbuild supplies WDT_BASE; it anchors the register offsets (where it
 * cancels out, the mapping coming from the device tree) and is the address the
 * ioremap fallback uses on the parts that have no DT node for the watchdog.
 * The V4 map is the default because that is where this file started.
 */
#ifndef WDT_BASE
#define WDT_BASE 0x12030000
#endif
#define WDT_REG(x) (WDT_BASE + (x))

#define WDT_LOAD 0x000
#define WDT_VALUE 0x004
#define WDT_CTRL 0x008
#define WDT_INTCLR 0x00C
#define WDT_RIS 0x010
#define WDT_MIS 0x014
#define WDT_LOCK 0xC00

#define WDT_UNLOCK_VAL 0x1ACCE551

#define WATCHDOG_MINOR 101

volatile void *gpWtdgAllReg = NULL;
#define IO_WDT_ADDRESS(x) ((uintptr_t)(gpWtdgAllReg) + ((x) - (WDT_BASE)))

#ifdef WDT_SCTL_BASE
/*
 * The V2-era parts (0x20040000 map: hi3516cv100/cv200, hi3516av100,
 * hi3520dv200) gate the watchdog's 3 MHz clock in the system controller, and
 * the counter does not run until dog_start() ungates it. Later generations
 * have it on already, which is why this arrived here commented out.
 */
static volatile void *reg_ctl_base_va = NULL;
#endif

/* Set when the ioremap above was ours, so watchdog_exit() knows to undo it. */
static int need_iounmap = 0;

#define wdt_readl(x) osal_readl(IO_WDT_ADDRESS(WDT_REG(x)))
#define wdt_writel(v, x) osal_writel(v, IO_WDT_ADDRESS(WDT_REG(x)))

/* debug */
#define DOG_PFX "WatchDog: "
#define dog_dbg(params...) osal_printk(DOG_PFX params)

/* module param */
int default_margin = DOG_TIMER_MARGIN; /* in seconds */
#define DOG_TIMER_DEMULTIPLY 9

//int nowayout = 0;

int nodeamon = 0;

/* watchdog info */
static struct watchdog_info ident = {
	.options = WDIOF_SETTIMEOUT | WDIOF_KEEPALIVEPING | WDIOF_MAGICCLOSE,
	.firmware_version = 0,
	.identity = "Watchdog",
};

/* local var */
static struct osal_spinlock dog_lock;
static int cur_margin = DOG_TIMER_MARGIN;
//static pid_t pid_dog_deamon = -1;
#ifdef __LITEOS__
pthread_t task_dog_deamon = 0;
#else
struct osal_task *task_dog_deamon = NULL;
#endif

#define DOG_EXIT 0
#define DOG_SELFCLR 1
#define DOG_EXTCLR 2
volatile static unsigned int dog_state = 0;

static osal_atomic_t driver_open;
//static int orphan_timer = 0;
static unsigned int options = WDIOS_ENABLECARD;
/* Set by a "V" through dog_write(): the caller means the next close. */
static int expect_close = 0;

/*
 * The V3 OSAL predates the stop flag -- osal_kthread_destory() takes the task
 * alone there and always stops it, which is the only thing this driver ever
 * asks for. Later trees added the flag.
 */
#ifdef WDT_OSAL_NO_KTHREAD_STOP_FLAG
#define wdt_kthread_destroy(t) osal_kthread_destory(t)
#else
#define wdt_kthread_destroy(t) osal_kthread_destory((t), 1)
#endif

#ifndef MHZ
#define MHZ (1000 * 1000)
#endif

static unsigned long rate = 3 * MHZ;

/*
 * The counter is loaded with half a margin's worth of ticks, because the
 * SP805 runs the load value twice before it resets anything: reaching zero
 * raises the interrupt and reloads, and only reaching zero a second time with
 * that interrupt still pending drives the reset. Loading a whole margin made
 * every timeout mean twice itself — a margin of 30 s reset the board at 60 —
 * so nothing a caller set through WDIOC_SETTIMEOUT was the time it got. Same
 * arithmetic as drivers/watchdog/sp805_wdt.c.
 */
/*
 * Half of the counter's range. The load has to be added back to report the
 * time left, so keeping the sum inside 32 bits is what lets that arithmetic
 * stay 32-bit on every part this builds for — and 0x7fffffff / (rate / 2) is
 * 1431 s at 3 MHz, exactly where this driver has always put its ceiling.
 */
#define WDT_LOAD_MAX 0x7fffffffU

static unsigned int dog_max_margin(void)
{
	return (unsigned int)(WDT_LOAD_MAX / (rate / 2));
}

static unsigned int dog_load_val(unsigned int nr)
{
	if (nr == 0 || nr > dog_max_margin())
		return WDT_LOAD_MAX;

	return (unsigned int)(nr * (rate / 2)) - 1;
}

static unsigned int load_val = WDT_LOAD_MAX;

static void dog_set_timeout(unsigned int nr)
{
	unsigned long flags;
	unsigned int cnt = dog_load_val(nr);

	osal_spin_lock_irqsave(&dog_lock, &flags);

	load_val = cnt;
	/* unlock watchdog registers */
	wdt_writel(WDT_UNLOCK_VAL, WDT_LOCK);
	wdt_writel(cnt, WDT_LOAD);
	wdt_writel(cnt, WDT_VALUE);
	/* lock watchdog registers */
	wdt_writel(0, WDT_LOCK);
	osal_spin_unlock_irqrestore(&dog_lock, &flags);
};

/*
 * A ping restarts the timer, which is what WDIOC_KEEPALIVE means and all a
 * caller can be expected to assume.
 *
 * This used to read WDT_RIS first and return without touching anything when
 * no interrupt was pending — so a ping did nothing at all until the counter
 * had already run out once. The deadline therefore stayed where the previous
 * accepted ping had put it, which made the honest cadence of one ping per
 * half-margin worth exactly one accepted ping per cycle: miss it and a
 * process that had been feeding the device all along got the board reset
 * under it. Writing the load and clearing the interrupt is unconditional in
 * sp805_wdt.c for the same reason.
 */
static void dog_feed(void)
{
	unsigned long flags;

	osal_spin_lock_irqsave(&dog_lock, &flags);
	/* unlock watchdog registers */
	wdt_writel(WDT_UNLOCK_VAL, WDT_LOCK);
	/* reload the counter, and clear an interrupt if one is already up */
	wdt_writel(load_val, WDT_LOAD);
	wdt_writel(0x00, WDT_INTCLR);
	/* lock watchdog registers */
	wdt_writel(0, WDT_LOCK);
	osal_spin_unlock_irqrestore(&dog_lock, &flags);
};

static int dog_set_heartbeat(int t)
{
	if (t <= 0 || (unsigned int)t > dog_max_margin())
		return -EINVAL;

	cur_margin = t;

	dog_set_timeout(t);
	dog_feed();

	//if(NULL != task_dog_deamon)
	//    osal_wake_up_process(task_dog_deamon);

	return 0;
}

static int dog_keepalive(void)
{
	dog_feed();
	return 0;
}

static void dog_start(void)
{
	unsigned long flags;
#ifdef WDT_SCTL_BASE
	unsigned long t;
#endif

	osal_spin_lock_irqsave(&dog_lock, &flags);
	/* unlock watchdog registers */
	wdt_writel(WDT_UNLOCK_VAL, WDT_LOCK);
	wdt_writel(0x00, WDT_CTRL);
	wdt_writel(0x00, WDT_INTCLR);
	wdt_writel(0x03, WDT_CTRL);
	/* lock watchdog registers */
	wdt_writel(0, WDT_LOCK);
#ifdef WDT_SCTL_BASE
	/* enable watchdog clock --- set the frequency to 3MHz */
	t = osal_readl(reg_ctl_base_va);
	osal_writel(t & ~0x00800000, reg_ctl_base_va);
#endif
	osal_spin_unlock_irqrestore(&dog_lock, &flags);

	options = WDIOS_ENABLECARD;
}

static void dog_stop(void)
{
	unsigned long flags;

	osal_spin_lock_irqsave(&dog_lock, &flags);

	/* unlock watchdog registers */
	wdt_writel(WDT_UNLOCK_VAL, WDT_LOCK);

	/* stop watchdog timer */
	wdt_writel(0x00, WDT_CTRL);
	wdt_writel(0x00, WDT_INTCLR);

	/* lock watchdog registers */
	wdt_writel(0, WDT_LOCK);

	osal_spin_unlock_irqrestore(&dog_lock, &flags);

	dog_set_timeout(0);

	options = WDIOS_DISABLECARD;
}

//extern struct module __this_module;

static int dog_open(void *private_data)
{
	int ret = 0;

	if (0 != osal_atomic_dec_return(&driver_open)) {
		ret = osal_atomic_inc_return(&driver_open);
		osal_printk("Error: device already open:%d.\n", ret);
		return -1;
	}
	//if (!orphan_timer)
	//{
	//    osal_module_get(&__this_module);
	//}
	//orphan_timer = 0;

	/*
	 * The kernel-side feeder exists to cover the window between module
	 * load, where dog_init() arms the hardware, and the first open. Once
	 * userspace has taken the device that window is over for good, so
	 * this transition is one-way: dog_release() does not put it back.
	 * It used to, and that is why the watchdog never reset anything —
	 * the process being guarded died, its descriptor was closed for it,
	 * and the driver itself went on feeding the dog for the rest of the
	 * board's life. See OpenIPC/firmware#1803, where the only way to get
	 * a reset out of this driver was nodeamon=1, i.e. having no feeder
	 * at all and therefore no boot-time cover either.
	 */
	dog_state = DOG_EXTCLR;
	expect_close = 0;

	/*
     *    Activate timer
     *
     * "When the device is opened, the watchdog is started" -- the API says
     * so unconditionally, and it used to hold here only because nothing had
     * turned the counter off yet. A magic close, or WDIOC_SETOPTIONS with
     * WDIOS_DISABLECARD, clears WDT_CTRL; neither the ping below nor
     * WDIOC_SETTIMEOUT touches it, so before this call the device stayed
     * dead from the first deliberate stop until something thought to send
     * WDIOS_ENABLECARD -- an open would succeed, the margin would be
     * accepted, the pings would be accepted, and nothing would ever reset.
     */
	dog_start();
	dog_keepalive();

	return ret;
}

static int dog_release(void *private_data)
{
	if (1 != osal_atomic_inc_return(&driver_open)) {
		osal_atomic_dec_return(&driver_open);
		return 0;
	}

	/*
     *    Shut off the timer, if the caller said it meant to.
     */
	if (expect_close) {
		dog_stop();
		expect_close = 0;
	} else if (options & WDIOS_ENABLECARD) {
		osal_printk("Unexpected close, watchdog left running!\n");
	}
	//osal_module_put(&__this_module);

	if (options == WDIOS_DISABLECARD)
		osal_printk("Watchdog is disabled!\n");

	return 0;
}

/*
 * ident.options has advertised WDIOF_MAGICCLOSE all along while this was
 * compiled out, so a caller doing the documented thing — write "V", then
 * close, to say the shutdown is deliberate — got EINVAL from the write and no
 * way at all to stop the device it had started. A write is also the other
 * documented way to ping.
 */
static int dog_write(const char *data, int len, long *ppos, void *private_data)
{
	/*
     *Refresh the timer.
     */
	if (len) {
		int i;

		expect_close = 0;

		for (i = 0; i != len; i++) {
			char c;
			if (osal_copy_from_user(&c, data + i, sizeof(char)))
				return -EFAULT;
			if (c == 'V')
				expect_close = 1;
		}
		dog_keepalive();
	}

	return len;
}

static long dog_ioctl(unsigned int cmd, unsigned long arg, void *private_data)
{
	void *argp = (void *)(uintptr_t)arg;
	int *p = argp;
	int new_margin;
	int ret;
	unsigned int new_options;

	switch (cmd) {
	case WDIOC_GETSUPPORT:
		osal_memcpy(argp, &ident, sizeof(ident));
		return 0;

	case WDIOC_GETSTATUS:
	case WDIOC_GETBOOTSTATUS:
		*p = options;
		return 0;

	case WDIOC_KEEPALIVE:
		dog_keepalive();
		return 0;

	case WDIOC_SETTIMEOUT:
		new_margin = *p;
		ret = dog_set_heartbeat(new_margin);
		if (ret)
			return ret;
		dog_keepalive();
		/* Falls through to GETTIMEOUT, as the watchdog core does: the
		 * caller reads back the margin the device is running at. */
		*p = cur_margin;
		return 0;

	case WDIOC_GETTIMEOUT:
		*p = cur_margin;
		return 0;

	case WDIOC_GETTIMELEFT: {
		unsigned long flags;
		unsigned long left;

		osal_spin_lock_irqsave(&dog_lock, &flags);
		left = wdt_readl(WDT_VALUE);
		/* The counter is loaded with half a margin and run twice, so
		 * with no interrupt pending there is still a whole load to go
		 * after this one. */
		if (!(wdt_readl(WDT_RIS) & 0x1))
			left += (unsigned long)load_val + 1;
		osal_spin_unlock_irqrestore(&dog_lock, &flags);

		*p = (int)(left / rate);
		return 0;
	}

	case WDIOC_SETOPTIONS:
		new_options = *p;
		if (new_options & WDIOS_ENABLECARD) {
			dog_start();
			dog_set_heartbeat(cur_margin);
			return 0;
		} else if (new_options & WDIOS_DISABLECARD) {
			dog_stop();
			return 0;
		} else
			return -EINVAL;

	default:
		return -ENOTTY;
	}
}

/*
 *    Notifier for system down
 */
#if 0
static int dog_notifier_sys(struct osal_notifier_block *this, unsigned long code,
    void *unused)
{

        /* Turn the WDT off */
        dog_stop();

    return 0;
}
#endif
/*
 *    Kernel Interfaces
 */

static struct osal_fileops dog_fops = {
	.write = dog_write,
	//    .ioctl        = dog_ioctl,
	.unlocked_ioctl = dog_ioctl,
	.open = dog_open,
	.release = dog_release,
};

static struct osal_dev *dog_miscdev = NULL;

#if 0
static struct osal_notifier_block dog_notifier = {
    .notifier_call    = dog_notifier_sys,
};
#endif

static char banner[] =
	"Watchdog Timer: 0.01 initialized. default_margin=%d sec (nodeamon= %d)\n";

#define DOG_DEAMON_MAX_SLEEP_MS 30000

static int dog_deamon(void *data)
{
	unsigned int period;

#ifdef __LITEOS__
	prctl(PR_SET_NAME, "dog_deamon", 0, 0, 0);
#endif
	/*
	 * Feeds the dog from module load until the first open, and never
	 * again — dog_open() latches DOG_EXTCLR. Anything else would mean
	 * the driver guarding itself.
	 */
	while (dog_state != DOG_EXIT) {
		switch (dog_state) {
		case DOG_SELFCLR:
			if (options & WDIOS_ENABLECARD)
				dog_feed();
			break;
		case DOG_EXTCLR:
			break;
		default:
			break;
		}
		/* sleep */
		/*
		 * Half of the margin in force, not half of the module's
		 * default. Userspace can change the margin and hand the
		 * device back, and a feeder slower than the margin it is
		 * feeding resets the board on a cycle nobody configured.
		 * Capped so that a long margin does not make module unload
		 * wait out a whole sleep.
		 */
		period = (unsigned int)cur_margin * 1000 / 2;
		if (period > DOG_DEAMON_MAX_SLEEP_MS)
			period = DOG_DEAMON_MAX_SLEEP_MS;
		osal_msleep(period + 10);
	}

	return 0;
}

static int dog_init(void)
{
	dog_start();

	dog_state = DOG_SELFCLR;
	if (!nodeamon) {
#ifdef __LITEOS__
		if (pthread_create(&task_dog_deamon, NULL, (void *)dog_deamon,
				   0) < 0) {
			osal_printk("create dog_deamon failed!\n");
			return -1;
		}
#else
		struct osal_task *p_dog = NULL;
		p_dog = osal_kthread_create(dog_deamon, NULL, "dog");
		if (NULL == p_dog) {
			osal_printk("create dog_deamon failed!\n");
			return -1;
		}
		task_dog_deamon = p_dog;
#endif
	}
	return 0;
}

int watchdog_init(void)
{
	int ret = 0;

	ret = osal_atomic_init(&driver_open);
	if (ret) {
		osal_printk("Error: init atomic\n");
		return -1;
	}
	osal_atomic_set(&driver_open, 1);

	ret = osal_spin_lock_init(&dog_lock);
	if (0 != ret) {
		osal_printk("function %s line %u failed\n", __FUNCTION__,
			    __LINE__);
		goto watchdog_init_err0;
	}

	/*
	 * Non-NULL already means a platform probe handed us a devm mapping,
	 * which the device frees. Only what we map here is ours to unmap.
	 */
	if (gpWtdgAllReg == NULL) {
		gpWtdgAllReg = (volatile void *)osal_ioremap(WDT_BASE, 0x1000);

		if (!gpWtdgAllReg) {
			osal_printk("osal_ioremap err. \n");
			goto watchdog_init_err1;
		}
		need_iounmap = 1;
	}
#ifdef WDT_SCTL_BASE
	reg_ctl_base_va = (volatile void *)osal_ioremap(WDT_SCTL_BASE, 0x4);
	if (reg_ctl_base_va == NULL) {
		osal_printk("function %s line %u failed\n", __FUNCTION__,
			    __LINE__);
		goto watchdog_init_err2;
	}
#endif

	cur_margin = default_margin;

	/* Check that the default_margin value is within it's range ; if not reset to the default */
	if (dog_set_heartbeat(default_margin)) {
		default_margin = DOG_TIMER_MARGIN;
		dog_set_heartbeat(DOG_TIMER_MARGIN);
		osal_printk(
			"default_margin value must be 0<default_margin<=%u, using %d\n",
			dog_max_margin(), DOG_TIMER_MARGIN);
	}
#if 0
    ret = osal_register_reboot_notifier(&dog_notifier);
    if(ret) {
        osal_printk("cannot register reboot notifier (err=%d)\n", ret);
        goto watchdog_init_err3;
    }
#endif
	dog_miscdev = osal_createdev("watchdog");
	if (NULL == dog_miscdev) {
		osal_printk("fail to create dev\n");
		goto watchdog_init_err4;
	}
	osal_printk("register dev\n");
	dog_miscdev->minor = WATCHDOG_MINOR;
	dog_miscdev->fops = &dog_fops;

	ret = osal_registerdevice(dog_miscdev);
	if (ret) {
		osal_printk("fail to register dev\n");
		goto watchdog_init_err5;
	}

	//printk(banner, default_margin, nowayout, nodeamon);

	ret = dog_init();
	if (ret) {
		goto watchdog_init_err6;
	}

	osal_printk(banner, default_margin, nodeamon);
	osal_printk("wtdg init ok. ver=%s, %s.\n", __DATE__, __TIME__);

	return 0;

watchdog_init_err6:
	osal_deregisterdevice(dog_miscdev);
watchdog_init_err5:
	osal_destroydev(dog_miscdev);
watchdog_init_err4:
	//  osal_unregister_reboot_notifier(&dog_notifier);
#ifdef WDT_SCTL_BASE
	osal_iounmap((void *)reg_ctl_base_va);
	reg_ctl_base_va = NULL;
watchdog_init_err2:
#endif
	if (need_iounmap) {
		osal_iounmap((void *)gpWtdgAllReg);
		gpWtdgAllReg = NULL;
		need_iounmap = 0;
	}
watchdog_init_err1:
	osal_spin_lock_destory(&dog_lock);
watchdog_init_err0:
	osal_atomic_destory(&driver_open);
	return -1;
}

static void dog_exit(void)
{
	dog_set_timeout(0);
	dog_stop();
	dog_state = DOG_EXIT;
	if (!nodeamon) {
#ifdef __LITEOS__
		pthread_join(task_dog_deamon, NULL);
#else
		struct osal_task *p_dog = task_dog_deamon;
		if (p_dog == NULL)
			return;
		//osal_wake_up_process(p_dog);
		wdt_kthread_destroy(p_dog);
#endif
		osal_yield();
	}

#ifdef __LITEOS__
	task_dog_deamon = 0;
#else
	task_dog_deamon = NULL;
#endif
}

void watchdog_exit(void)
{
	/*
	 * Unregister before tearing anything down. dog_exit() sets DOG_EXIT
	 * and then blocks in kthread_stop() waiting for the feeder to notice,
	 * and the feeder's loop condition is dog_state -- which dog_open()
	 * sets back to DOG_EXTCLR. With the device still registered at that
	 * point, one open() racing the unload left the thread looping for
	 * ever and rmmod stuck in D state holding module_mutex, unkillable
	 * and not survivable by a clean reboot. Observed on a gk7205v200.
	 */
	osal_deregisterdevice(dog_miscdev);
	osal_destroydev(dog_miscdev);

	dog_exit();

#ifdef WDT_SCTL_BASE
	osal_iounmap((void *)reg_ctl_base_va);
	reg_ctl_base_va = NULL;
#endif
	if (need_iounmap) {
		osal_iounmap((void *)gpWtdgAllReg);
		gpWtdgAllReg = NULL;
		need_iounmap = 0;
	}
	//osal_unregister_reboot_notifier(&dog_notifier);
	osal_spin_lock_destory(&dog_lock);
	osal_atomic_destory(&driver_open);
	osal_printk("wtdg exit ok.\n");
}
