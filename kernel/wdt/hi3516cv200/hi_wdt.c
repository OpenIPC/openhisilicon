/*
*
* Copyright (c) 2006 Hisilicon Co., Ltd.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*
* This file is based on linux-2.6.14/drivers/char/watchdog/softdog.c
*
* 20060830 Liu Jiandong <liujiandong@hisilicon.com>
*     Support Hisilicon's chips, as Hi3510.
*/


#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/timer.h>
#include <linux/miscdevice.h>
#include <linux/watchdog.h>
#include <linux/fs.h>
#include <linux/mutex.h>
#include <linux/notifier.h>
#include <linux/reboot.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <asm/io.h>
//#include <asm/arch/hardware.h>
#include <linux/kthread.h>
//#include <asm/arch/clock.h>
#include <linux/string.h>
#include <linux/sched.h>

#define OSDRV_MODULE_VERSION_STRING "HISI_WDT-MDC030001 @Hi3518v100"

#define HISILICON_SCTL_BASE 0x20050000
/* define watchdog IO */
#define HIWDT_BASE      0x20040000
#define HIWDT_REG(x)    (HIWDT_BASE + (x))

#define HIWDT_LOAD      0x000
#define HIWDT_VALUE     0x004
#define HIWDT_CTRL      0x008
#define HIWDT_INTCLR    0x00C
#define HIWDT_RIS       0x010
#define HIWDT_MIS       0x014
#define HIWDT_LOCK      0xC00

#define HIWDT_UNLOCK_VAL    0x1ACCE551

void __iomem *reg_ctl_base_va;
void __iomem *reg_wdt_base_va;
#define IO_WDT_ADDRESS(x) (reg_wdt_base_va + ((x)-(HIWDT_BASE)))

#define hiwdt_readl(x)      readl(IO_WDT_ADDRESS(HIWDT_REG(x)))
#define hiwdt_writel(v,x)   writel(v, IO_WDT_ADDRESS(HIWDT_REG(x)))

/* debug */
#define HIDOG_PFX "HiDog: "
#define hidog_dbg(params...) printk(KERN_INFO HIDOG_PFX params)

/* module param */
#define HIDOG_TIMER_MARGIN    60
static int default_margin = HIDOG_TIMER_MARGIN;    /* in seconds */
#define HIDOG_TIMER_DEMULTIPLY  9
module_param(default_margin, int, 0);
MODULE_PARM_DESC(default_margin, "Watchdog default_margin in seconds. (0<default_margin<80, default=" __MODULE_STRING(HIDOG_TIMER_MARGIN) ")");

static int nowayout = WATCHDOG_NOWAYOUT;
module_param(nowayout, int, 0);
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started (default=CONFIG_WATCHDOG_NOWAYOUT)");

static int nodeamon = 0;
module_param(nodeamon, int, 0);
MODULE_PARM_DESC(nodeamon, "By default, a kernel deamon feed watchdog when idle, set 'nodeamon=1' to disable this. (default=0)");

/* watchdog info */
static struct watchdog_info ident = {
    .options =  WDIOF_SETTIMEOUT |
                WDIOF_KEEPALIVEPING |
                WDIOF_MAGICCLOSE,
    .firmware_version = 0,
    .identity = "Hisilicon Watchdog",
};

/* local var */
static DEFINE_SPINLOCK(hidog_lock);
static int cur_margin = HIDOG_TIMER_MARGIN;
//static pid_t pid_hidog_deamon = -1;
struct task_struct *task_hidog_deamon = NULL;


#define HIDOG_EXIT    0
#define HIDOG_SELFCLR    1
#define HIDOG_EXTCLR    2
volatile static unsigned int hidog_state = 0;

static unsigned long driver_open = 0;
/* Set by the reboot notifier before it stops the timer. open() now starts the
 * hardware, so without this a process opening the device while the system is
 * going down could re-arm the watchdog behind the shutdown's back and reset
 * the board instead of letting it halt. */
static volatile int shutting_down = 0;
/*
 * Serialises a complete open against the reboot notifier. The register writes
 * have dog_lock, but arming is start-then-heartbeat and the notifier's stop can
 * land between them; both sides can sleep, so a mutex is what orders the whole
 * transition. Without it an open either re-arms the watchdog into a shutdown or
 * returns a device whose timer the notifier has just turned off.
 */
static DEFINE_MUTEX(hidog_lifecycle);
/* Set by a "V" through hidog_write(): the caller means the next close. This
 * used to be the nowayout module parameter itself, which inverted the whole
 * thing -- see hidog_release(). */
static int expect_close = 0;
static int options = WDIOS_ENABLECARD;

#ifndef MHZ
#define MHZ (1000*1000)
#endif

static unsigned long rate = 3*MHZ;

/*
 * The counter is loaded with half a margin's worth of ticks, because the SP805
 * runs the load value twice before it resets anything: reaching zero raises the
 * interrupt and reloads, and only reaching zero a second time with that
 * interrupt still pending drives the reset. Loading a whole margin made every
 * timeout mean twice itself. Same arithmetic as drivers/watchdog/sp805_wdt.c.
 *
 * Half of the counter's range: the load has to be added back to report the time
 * left, and keeping that sum inside 32 bits is what lets the arithmetic stay
 * 32-bit.
 */
#define HIWDT_LOAD_MAX 0x7fffffffU

static unsigned int hidog_max_margin(void)
{
    return (unsigned int)(HIWDT_LOAD_MAX / (rate / 2));
}

static unsigned int hidog_load_val(unsigned int nr)
{
    if (nr == 0 || nr > hidog_max_margin())
        return HIWDT_LOAD_MAX;

    return (unsigned int)(nr * (rate / 2)) - 1;
}

static unsigned int load_val = HIWDT_LOAD_MAX;

static inline void hidog_set_timeout(unsigned int nr)
{
    unsigned int cnt = hidog_load_val(nr);
    unsigned long flags;

    spin_lock_irqsave(&hidog_lock, flags);

    load_val = cnt;
    /* unlock watchdog registers */
    hiwdt_writel(HIWDT_UNLOCK_VAL, HIWDT_LOCK);
    hiwdt_writel(cnt, HIWDT_LOAD);
    hiwdt_writel(cnt, HIWDT_VALUE);
    /* lock watchdog registers */
    hiwdt_writel(0, HIWDT_LOCK);
    spin_unlock_irqrestore(&hidog_lock, flags);
};

/*
 * A ping restarts the timer, which is what WDIOC_KEEPALIVE means and all a
 * caller can be expected to assume. This used to clear the interrupt without
 * ever reloading the counter, so the deadline stayed where the last reload had
 * put it and a process feeding at the honest cadence of one ping per half
 * margin still got the board reset under it. Writing the load and clearing the
 * interrupt is unconditional in sp805_wdt.c for the same reason.
 */
static inline void hidog_feed(void)
{
    unsigned long flags;

    spin_lock_irqsave(&hidog_lock, flags);
    /* unlock watchdog registers */
    hiwdt_writel(HIWDT_UNLOCK_VAL, HIWDT_LOCK);
    /* reload the counter, and clear an interrupt if one is already up */
    hiwdt_writel(load_val, HIWDT_LOAD);
    hiwdt_writel(0x00, HIWDT_INTCLR);
    /* lock watchdog registers */
    hiwdt_writel(0, HIWDT_LOCK);
    spin_unlock_irqrestore(&hidog_lock, flags);
};

/*
 * Out of range is -EINVAL and the margin in force is left alone, as every other
 * watchdog driver does it. Half-applying a value the caller was told was
 * rejected -- which is what clamping here amounted to -- left the device
 * running at a margin nobody had asked for.
 */
static int hidog_set_heartbeat(int t)
{
    if (t <= 0 || (unsigned int)t > hidog_max_margin())
        return -EINVAL;

    cur_margin = t;

    hidog_set_timeout(t);
    hidog_feed();

    if (NULL != task_hidog_deamon)
        wake_up_process(task_hidog_deamon);

    return 0;
}

static int hidog_keepalive(void)
{
    hidog_feed();
    return 0;
}

static inline void hidog_start(void)
{
    unsigned long flags;
    unsigned long t;

    spin_lock_irqsave(&hidog_lock, flags);
    /* unlock watchdog registers */
    hiwdt_writel(HIWDT_UNLOCK_VAL, HIWDT_LOCK);
    hiwdt_writel(0x00, HIWDT_CTRL);
    hiwdt_writel(0x00, HIWDT_INTCLR);
    hiwdt_writel(0x03, HIWDT_CTRL);
    /* lock watchdog registers */
    hiwdt_writel(0, HIWDT_LOCK);
    /* enable watchdog clock --- set the frequency to 3MHz */
    t = readl(reg_ctl_base_va);
    writel(t & ~0x00800000, reg_ctl_base_va);
    spin_unlock_irqrestore(&hidog_lock, flags);

    options = WDIOS_ENABLECARD;
}

static inline void hidog_stop(void)
{
    unsigned long flags;

    spin_lock_irqsave(&hidog_lock, flags);

    /* unlock watchdog registers */
    hiwdt_writel(HIWDT_UNLOCK_VAL, HIWDT_LOCK);

    /* stop watchdog timer */
    hiwdt_writel(0x00, HIWDT_CTRL);
    hiwdt_writel(0x00, HIWDT_INTCLR);

    /* lock watchdog registers */
    hiwdt_writel(0, HIWDT_LOCK);

    spin_unlock_irqrestore(&hidog_lock, flags);

    hidog_set_timeout(0);

    options = WDIOS_DISABLECARD;
}

static int hidog_open(struct inode *inode, struct file *file)
{
    int ret = 0;

    if (test_and_set_bit(0, &driver_open))
        return -EBUSY;
    /*
     * Nothing should be handed a watchdog while the system is going down. The
     * reboot notifier has already stopped the timer, and arming it again from
     * here is how an open racing a shutdown resets the board instead of
     * letting it halt -- so refuse rather than return a device that is either
     * about to bite or already dead.
     */
    mutex_lock(&hidog_lifecycle);
    if (shutting_down) {
        mutex_unlock(&hidog_lifecycle);
        clear_bit(0, &driver_open);
        return -ENODEV;
    }
    __module_get(THIS_MODULE);
    /*
     *    Activate timer
     *
     * "When the device is opened, the watchdog is started" -- the API says so
     * unconditionally, and that held here only for as long as nothing had
     * stopped the counter. A magic close, or WDIOS_DISABLECARD, clears
     * HIWDT_CTRL, and neither a ping nor WDIOC_SETTIMEOUT puts it back.
     *
     * The margin rather than a bare ping, too: hidog_stop() disarms through
     * hidog_set_timeout(0), which parks the load at the counter maximum, and a
     * feed would write that straight back.
     */
    hidog_start();
    hidog_set_heartbeat(cur_margin);
    mutex_unlock(&hidog_lifecycle);
    ret = nonseekable_open(inode, file);
    if(ret == 0) {
        /*
         * The kernel-side feeder covers the window between module load, where
         * hidog_init() arms the hardware, and the first open. Once userspace
         * has taken the device that window is over for good, so this is
         * one-way: hidog_release() does not put it back. It used to, and that
         * is why the watchdog never reset anything when the guarded process
         * died. See OpenIPC/firmware#1803.
         */
        hidog_state = HIDOG_EXTCLR;
        expect_close = 0;
    }

    return ret;
}

static int hidog_release(struct inode *inode, struct file *file)
{
    /*
     * Shut off the timer, if the caller said it meant to -- and this used to be
     * exactly backwards. hidog_write() assigned the nowayout module parameter,
     * so writing "V", the documented way to say "I am stopping it deliberately",
     * took the branch that left the dog running, while NOT writing it took the
     * branch that handed the device to the kernel feeder. The one event the
     * watchdog exists for -- the guarded process dying, and the kernel closing
     * its descriptor -- therefore ended with the driver feeding the dog for the
     * rest of the board's life, and nothing ever reset.
     *
     * WDIOF_MAGICCLOSE, which ident has advertised throughout, means a close
     * without a preceding "V" leaves the timer running and unfed. nowayout goes
     * back to meaning what it says: the device cannot be stopped at all.
     */
    if (expect_close && !nowayout) {
        hidog_stop();
    } else {
        if (expect_close)
            printk(KERN_CRIT HIDOG_PFX "nowayout set, refusing to stop the watchdog!\n");
        else if (options & WDIOS_ENABLECARD)
            printk(KERN_CRIT HIDOG_PFX "Unexpected close, watchdog left running!\n");
    }
    /*
     * Symmetric with the reference open() took. This used to be skipped on the
     * close that leaves the dog running, pinning the module for good: rmmod
     * then returned EAGAIN for the rest of the boot. Nothing needs the pin --
     * watchdog_exit() stops the timer on the way out, so unloading is safe.
     */
    expect_close = 0;
    module_put(THIS_MODULE);
    clear_bit(0, &driver_open);

    if(options == WDIOS_DISABLECARD)
        printk(KERN_INFO HIDOG_PFX "Watchdog is disabled!\n");

    return 0;
}

static ssize_t hidog_write(struct file *file, const char __user *data, size_t len, loff_t *ppos)
{
    /*
     *      *    Refresh the timer.
     *           */
    if(len) {
        size_t i;

        expect_close = 0;

        for (i = 0; i != len; i++) {
            char c;

            if (get_user(c, data + i)) {
                /* The write failed, so nothing it carried stands -- including
                 * a "V" already seen. The watchdog core leaves the flag set
                 * here, but the cost of getting it wrong is a board that
                 * quietly stops being guarded. */
                expect_close = 0;
                return -EFAULT;
            }
            if (c == 'V')
                expect_close = 1;
        }
        hidog_keepalive();
    }

    return len;
}

//static int hidog_ioctl(struct inode *inode, struct file *file,
//    unsigned int cmd, unsigned long arg)
static long hidog_ioctl(struct file *file,
    unsigned int cmd, unsigned long arg)
{
    void __user *argp = (void __user *)arg;
    int __user *p = argp;
    int new_margin;
    int new_options;

    /*
     * Two of these commands do not carry the numbers <linux/watchdog.h> gives
     * them. From the V3 SDK onwards the vendor spelt WDIOC_SETOPTIONS _IOWR
     * and WDIOC_KEEPALIVE _IO, and an ioctl number encodes direction and
     * argument size, so those are different values -- 0xC0045704 and
     * 0x00005705 against mainline's 0x80045704 and 0x80045705. A client built
     * against the standard header therefore never matched, which is why
     * majestic carries a block of #undefs to speak the vendor's numbers.
     *
     * Only the encoding ever differed -- the command number is 4 and 5 on both
     * sides. Normalise on that, so either spelling reaches the same case: a
     * stock watchdog client works, and nothing built against the vendor header
     * breaks.
     */
    if (_IOC_TYPE(cmd) == WATCHDOG_IOCTL_BASE) {
        switch (_IOC_NR(cmd)) {
        case 4:
            cmd = WDIOC_SETOPTIONS;
            break;
        case 5:
            cmd = WDIOC_KEEPALIVE;
            break;
        default:
            break;
        }
    }

    switch (cmd) {
        case WDIOC_GETSUPPORT:
            return copy_to_user(argp, &ident,
                sizeof(ident)) ? -EFAULT : 0;
        case WDIOC_GETSTATUS:
        case WDIOC_GETBOOTSTATUS:
            return put_user(options, p);

        case WDIOC_KEEPALIVE:
            hidog_keepalive();
            return 0;

        case WDIOC_SETTIMEOUT: {
            int err;

            if (get_user(new_margin, p))
                return -EFAULT;
            err = hidog_set_heartbeat(new_margin);
            if (err)
                return err;
            hidog_keepalive();
            /* Falls through to GETTIMEOUT, as the watchdog core does: the
             * caller reads back the margin the device is running at. */
            return put_user(cur_margin, p);
        }

        case WDIOC_GETTIMEOUT:
            return put_user(cur_margin, p);

        case WDIOC_GETTIMELEFT: {
            unsigned long flags;
            unsigned long left;

            spin_lock_irqsave(&hidog_lock, flags);
            left = hiwdt_readl(HIWDT_VALUE);
            /* The counter is loaded with half a margin and run twice, so with
             * no interrupt pending there is still a whole load to go after
             * this one. */
            if (!(hiwdt_readl(HIWDT_RIS) & 0x1))
                left += (unsigned long)load_val + 1;
            spin_unlock_irqrestore(&hidog_lock, flags);

            return put_user((int)(left / rate), p);
        }

        case WDIOC_SETOPTIONS:
            if (get_user(new_options, p))
                return -EFAULT;
            if(new_options & WDIOS_ENABLECARD) {
                hidog_start();
                hidog_set_heartbeat(cur_margin);
                return 0;
            } else if (new_options & WDIOS_DISABLECARD) {
                /* nowayout means the device cannot be stopped, which is what
                 * the watchdog core answers with -EBUSY. It used to reject
                 * every SETOPTIONS, enabling included, and with the sign of
                 * WDIOS_UNKNOWN inverted it returned 1 rather than an error. */
                if (nowayout)
                    return -EBUSY;
                hidog_stop();
                return 0;
            } else
                return -EINVAL;

        default:
            return -ENOIOCTLCMD;
    }

}

/*
 *    Notifier for system down
 */

static int hidog_notifier_sys(struct notifier_block *this, unsigned long code,
    void *unused)
{
    if(code==SYS_DOWN || code==SYS_HALT) {
        /*
         * Under the same mutex as a complete open, so the two cannot
         * interleave: either the open finishes and this stops the timer
         * afterwards, or this runs first and the open refuses.
         */
        mutex_lock(&hidog_lifecycle);
        shutting_down = 1;
        /* Turn the WDT off */
        hidog_stop();
        mutex_unlock(&hidog_lifecycle);
    }
    return NOTIFY_DONE;
}

/*
 *    Kernel Interfaces
 */

static struct file_operations hidog_fops = {
    .owner        = THIS_MODULE,
    .llseek        = no_llseek,
    .write        = hidog_write,
//    .ioctl        = hidog_ioctl,
    .unlocked_ioctl = hidog_ioctl,
    .open        = hidog_open,
    .release    = hidog_release,
};

static struct miscdevice hidog_miscdev = {
    .minor        = WATCHDOG_MINOR,
    .name        = "watchdog",
    .fops        = &hidog_fops,
};

static struct notifier_block hidog_notifier = {
    .notifier_call    = hidog_notifier_sys,
};

static char banner[] __initdata = KERN_INFO "Hisilicon Watchdog Timer: 0.01 initialized. default_margin=%d sec (nowayout= %d, nodeamon= %d)\n";

#define HIDOG_DEAMON_MAX_SLEEP_MS 30000

/*
 * Feeds the dog from module load until the first open, and never again --
 * hidog_open() latches HIDOG_EXTCLR. Anything else would mean the driver
 * guarding itself.
 */
static int hidog_deamon(void *data)
{
    struct sched_param param = { .sched_priority = 99 };
    unsigned int period;

    sched_setscheduler(current, SCHED_FIFO, &param);
    current->flags |= PF_NOFREEZE;

    set_current_state(TASK_INTERRUPTIBLE);

    while(hidog_state != HIDOG_EXIT)
    {
        switch(hidog_state) {
        case HIDOG_SELFCLR:
            if(options & WDIOS_ENABLECARD)
                hidog_feed();
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
        period = (unsigned int)cur_margin * 1000 / 2;
        if (period > HIDOG_DEAMON_MAX_SLEEP_MS)
            period = HIDOG_DEAMON_MAX_SLEEP_MS;
        schedule_timeout_interruptible(msecs_to_jiffies(period)+1);
    }

    set_current_state(TASK_RUNNING);

    return 0;
}

static int __init hidog_init(void)
{
    /*
     * hidog_start() arms the hardware before the feeder exists, so a failure
     * after it has to disarm again: watchdog_init() goes on to unmap the
     * registers and fail the load, and a module that failed to load has nothing
     * left that could feed the dog.
     */
    hidog_start();

    hidog_state = HIDOG_SELFCLR;
    if(!nodeamon) {
        #if 0
        struct task_struct *p_dog;
        p_dog = kthread_create(hidog_deamon, NULL, "hidog");
        if(IS_ERR(p_dog) <0) {
            printk(KERN_ERR HIDOG_PFX "create hidog_deamon failed!\n");
            return -1;
        pid_hidog_deamon = p_dog->pid;
        #endif

        struct task_struct *p_dog;
		p_dog = kthread_create(hidog_deamon, NULL, "hidog");
		if(IS_ERR(p_dog)) {
			printk(KERN_ERR HIDOG_PFX "create hidog_deamon failed!\n");
			hidog_stop();
			return -1;
		}
		wake_up_process(p_dog);
		task_hidog_deamon = p_dog;
    }
    //printk(KERN_INFO OSDRV_MODULE_VERSION_STRING);
    return 0;
}

static int __init watchdog_init(void)
{
    int ret = 0;

    reg_wdt_base_va = (void __iomem *)ioremap_nocache(HIWDT_BASE, 0x10000);
    if (NULL == reg_wdt_base_va)
    {
        printk(KERN_ERR HIDOG_PFX "function %s line %u failed\n",
            __FUNCTION__, __LINE__);
        return -1;
    }
    reg_ctl_base_va = (void __iomem *)ioremap_nocache(HISILICON_SCTL_BASE, 0x4);
    if (NULL == reg_ctl_base_va)
    {
        printk(KERN_ERR HIDOG_PFX "function %s line %u failed\n",
            __FUNCTION__, __LINE__);
        iounmap(reg_wdt_base_va);
        return -1;
    }

    cur_margin = default_margin;

    /* Check that the default_margin value is within it's range ; if not reset to the default */
    if(hidog_set_heartbeat(default_margin)) {
        default_margin = HIDOG_TIMER_MARGIN;
        hidog_set_heartbeat(HIDOG_TIMER_MARGIN);
        printk(KERN_WARNING HIDOG_PFX "default_margin value must be 0<default_margin<%lu, using %d\n",
            ~0x0/rate, HIDOG_TIMER_MARGIN);
    }

    ret = register_reboot_notifier(&hidog_notifier);
    if(ret) {
        printk(KERN_ERR HIDOG_PFX "cannot register reboot notifier (err=%d)\n", ret);
        goto watchdog_init_errA;
    }

    ret = misc_register(&hidog_miscdev);
    if(ret) {
        printk (KERN_ERR HIDOG_PFX "cannot register miscdev on minor=%d (err=%d)\n",
            WATCHDOG_MINOR, ret);
        goto watchdog_init_errB;
    }

    printk(banner, default_margin, nowayout, nodeamon);

    ret = hidog_init();
    if(ret) {
        goto watchdog_init_errC;
    }
    //printk(KERN_INFO OSDRV_MODULE_VERSION_STRING "\n");
    return ret;

watchdog_init_errC:
    misc_deregister(&hidog_miscdev);
watchdog_init_errB:
    unregister_reboot_notifier(&hidog_notifier);
watchdog_init_errA:
    return ret;
}

static void __exit hidog_exit(void)
{
    /*
     * nowayout means the watchdog cannot be stopped once started, and that has
     * to include module unload -- otherwise rmmod is a way to silence a dog
     * already counting down on a process that has died. Leave the hardware
     * alone and let it bite. The feeder below is stopped either way, so
     * nothing here goes on keeping it alive.
     */
    if (!nowayout) {
        hidog_set_timeout(0);
        hidog_stop();
    }
#if 0
    for(hidog_state=HIDOG_EXIT; !nodeamon; ) {
        #if 0
		struct task_struct *p_dog = find_task_by_vpid(pid_hidog_deamon);
        if(p_dog == NULL)
            break;
        #endif

        struct task_struct *p_dog = task_hidog_deamon;
		if(p_dog == NULL)
			break;
		wake_up_process(p_dog);
		yield();
    }
#endif
    hidog_state=HIDOG_EXIT;
    if(!nodeamon) {
        struct task_struct *p_dog = task_hidog_deamon;
        if(p_dog == NULL)
            return;
        wake_up_process(p_dog);
        kthread_stop(p_dog);
        yield();
    }

    task_hidog_deamon = NULL;
}

static void __exit watchdog_exit(void)
{
    misc_deregister(&hidog_miscdev);
    unregister_reboot_notifier(&hidog_notifier);

    hidog_exit();
    iounmap(reg_wdt_base_va);
    iounmap(reg_ctl_base_va);
    reg_ctl_base_va = NULL;
    reg_wdt_base_va = NULL;
}

module_init(watchdog_init);
module_exit(watchdog_exit);

MODULE_AUTHOR("Hisilicon");
MODULE_DESCRIPTION("Hisilicon Watchdog Device Driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);
MODULE_VERSION("HI_VERSION=" OSDRV_MODULE_VERSION_STRING);

