/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "osal.h"
#include <linux/types.h>
#include "watchdog.h"
#ifdef __LITEOS__
#include "pthread.h"
#include <sys/prctl.h>
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

/* define watchdog IO */
#define WDT_BASE 0x12030000
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

#define wdt_readl(x) osal_readl(IO_WDT_ADDRESS(WDT_REG(x)))
#define wdt_writel(v, x) osal_writel(v, IO_WDT_ADDRESS(WDT_REG(x)))

/* debug */
#define DOG_PFX "WatchDog: "
#define dog_dbg(params...) osal_printk(DOG_PFX params)

/* module param */
#define DOG_TIMER_MARGIN 60
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

#ifndef MHZ
#define MHZ (1000 * 1000)
#endif

static unsigned long rate = 3 * MHZ;

static void dog_set_timeout(unsigned int nr)
{
	unsigned long cnt = (~0x0UL) / rate; /* max cnt */
	unsigned long flags;

	osal_spin_lock_irqsave(&dog_lock, &flags);

	if (nr == 0 || nr > cnt)
		cnt = ~0x0;
	else
		cnt = nr * rate / 2;
	/* unlock watchdog registers */
	wdt_writel(WDT_UNLOCK_VAL, WDT_LOCK);
	wdt_writel(cnt, WDT_LOAD);
	wdt_writel(cnt, WDT_VALUE);
	/* lock watchdog registers */
	wdt_writel(0, WDT_LOCK);
	osal_spin_unlock_irqrestore(&dog_lock, &flags);
};

static void dog_feed(void)
{
	unsigned long flags;

	/* read the RIS state of current wdg */
	unsigned int v = 0;
	v = (unsigned int)wdt_readl(WDT_RIS);
	v &= 0x1;
	if (0 == v) /*no INT on current wdg */
		return;

	osal_spin_lock_irqsave(&dog_lock, &flags);
	/* unlock watchdog registers */
	wdt_writel(WDT_UNLOCK_VAL, WDT_LOCK);
	/* clear watchdog */
	wdt_writel(0x00, WDT_INTCLR);
	/* lock watchdog registers */
	wdt_writel(0, WDT_LOCK);
	osal_spin_unlock_irqrestore(&dog_lock, &flags);
};

static int dog_set_heartbeat(int t)
{
	int ret = 0;
	unsigned int cnt = (~0x0UL) / rate;

	if (t == 0) {
		osal_printk(
			"set heartbeat to 0, heartbeat will not be changed.\n");
		t = cur_margin;
		ret = 1;
	} else if (t > cnt) {
		osal_printk("set heartbeat range error, t = %d\n", t);
		osal_printk("force heartbeat to %d\n", cnt);
		t = cnt;
		ret = -1;
	}

	cur_margin = t;

	dog_set_timeout(t);
	dog_feed();

	//if(NULL != task_dog_deamon)
	//    osal_wake_up_process(task_dog_deamon);

	return ret;
}

static int dog_keepalive(void)
{
	dog_feed();
	return 0;
}

static void dog_start(void)
{
	unsigned long flags;
	//unsigned long t;

	osal_spin_lock_irqsave(&dog_lock, &flags);
	/* unlock watchdog registers */
	wdt_writel(WDT_UNLOCK_VAL, WDT_LOCK);
	wdt_writel(0x00, WDT_CTRL);
	wdt_writel(0x00, WDT_INTCLR);
	wdt_writel(0x03, WDT_CTRL);
	/* lock watchdog registers */
	wdt_writel(0, WDT_LOCK);
	/* enable watchdog clock --- set the frequency to 3MHz */
	//t = osal_readl(reg_ctl_base_va);
	//osal_writel(t & ~0x00800000, reg_ctl_base_va);
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

	dog_state = DOG_EXTCLR;

	/*
     *    Activate timer
     */
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
     *    Shut off the timer.
     *     Lock it in if it's a module and we set nowayout
     */
	//    if (nowayout == 0) {
	//cur_margin = default_margin;
	dog_state = DOG_EXIT;
	dog_set_heartbeat(cur_margin);
	//osal_module_put(&__this_module);
	//    } else {
	//        osal_printk("Unexpected close, not stopping watchdog!\n");
	//orphan_timer = 1;
	//        dog_keepalive();
	//    }

	if (options == WDIOS_DISABLECARD)
		osal_printk("Watchdog is disabled!\n");

	return 0;
}

#if 0
static int dog_write(const char *data, int len, long *ppos, void *private_data)
{
    /*
     *Refresh the timer.
     */
    if(len) {
        int i;

        nowayout = 0;

        for (i = 0; i != len; i++) {
            char c;
            if (osal_copy_from_user(&c, data + i, sizeof(char)))
                return -1;
            if (c == 'V')
                nowayout = 1;
        }
        dog_keepalive();
    }

    return len;
}
#endif

static long dog_ioctl(unsigned int cmd, unsigned long arg, void *private_data)
{
	void *argp = (void *)(uintptr_t)arg;
	int *p = argp;
	int new_margin;
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
		if (dog_set_heartbeat(new_margin))
			return -1;
		dog_keepalive();
		return 0;

	case WDIOC_GETTIMEOUT:
		*p = cur_margin;
		return 0;

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
			return -WDIOS_UNKNOWN;

	default:
		return -1;
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
	//   .write        = dog_write,
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

static int dog_deamon(void *data)
{
#ifdef __LITEOS__
	prctl(PR_SET_NAME, "dog_deamon", 0, 0, 0);
#endif
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
		/* when self feed dog, only use the default margin */
		osal_msleep(default_margin * 1000 / 2 + 10);
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

	if (gpWtdgAllReg == NULL) {
		gpWtdgAllReg = (volatile void *)osal_ioremap(WDT_BASE, 0x1000);

		if (!gpWtdgAllReg) {
			osal_printk("osal_ioremap err. \n");
			goto watchdog_init_err1;
		}
	}
	// reg_ctl_base_va = (void *)OSAL_IO_ADDRESS(SCTL_BASE);
	// if (NULL == reg_ctl_base_va)
	// {
	//     osal_printk("function %s line %u failed\n",
	//         __FUNCTION__, __LINE__);
	//     goto watchdog_init_err2;
	// }

	cur_margin = default_margin;

	/* Check that the default_margin value is within it's range ; if not reset to the default */
	if (dog_set_heartbeat(default_margin)) {
		default_margin = DOG_TIMER_MARGIN;
		dog_set_heartbeat(DOG_TIMER_MARGIN);
		osal_printk(
			"default_margin value must be 0<default_margin<%lu, using %d\n",
			(~0x0UL) / rate, DOG_TIMER_MARGIN);
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
#if 0
watchdog_init_err3:
    //osal_iounmap(reg_ctl_base_va);
watchdog_init_err2:
    //osal_iounmap(gpWtdgAllReg);
#endif
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
		osal_kthread_destory(p_dog, 1);
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
	dog_exit();

	osal_deregisterdevice(dog_miscdev);
	osal_destroydev(dog_miscdev);
	//osal_unregister_reboot_notifier(&dog_notifier);
	//osal_iounmap(reg_ctl_base_va);
	//reg_ctl_base_va = NULL;
	//osal_iounmap(gpWtdgAllReg);
	//gpWtdgAllReg = NULL;
	osal_spin_lock_destory(&dog_lock);
	osal_atomic_destory(&driver_open);
	osal_printk("wtdg exit ok.\n");
}
