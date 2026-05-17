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
#include "hi_osal.h"
#include "watchdog.h"
#ifdef __HuaweiLite__
#include "pthread.h"
#include <sys/prctl.h>
#endif

#ifndef NULL
#define NULL  ((void *)0)
#endif

#define OSDRV_MODULE_VERSION_STRING "HISI_WDT-MDC030001 @Hi3516CV300"

/* define watchdog IO */
#define HIWDT_BASE      0x12080000
#define HIWDT_REG(x)    (HIWDT_BASE + (x))

#define HIWDT_LOAD      0x000
#define HIWDT_VALUE     0x004
#define HIWDT_CTRL      0x008
#define HIWDT_INTCLR    0x00C
#define HIWDT_RIS       0x010
#define HIWDT_MIS       0x014
#define HIWDT_LOCK      0xC00

#define HIWDT_UNLOCK_VAL    0x1ACCE551

#define WATCHDOG_MINOR    101

volatile void *gpWtdgAllReg = NULL;
#define IO_WDT_ADDRESS(x) ((unsigned long)(gpWtdgAllReg) + ((x)-(HIWDT_BASE)))

#define hiwdt_readl(x)      osal_readl(IO_WDT_ADDRESS(HIWDT_REG(x)))
#define hiwdt_writel(v,x)   osal_writel(v, IO_WDT_ADDRESS(HIWDT_REG(x)))

/* debug */
#define HIDOG_PFX "HiDog: "
#define hidog_dbg(params...) osal_printk(HIDOG_PFX params)

/* module param */
#define HIDOG_TIMER_MARGIN    60
int default_margin = HIDOG_TIMER_MARGIN;    /* in seconds */
#define HIDOG_TIMER_DEMULTIPLY  9

//int nowayout = 0;

int nodeamon = 0;

/* watchdog info */
static struct watchdog_info ident = {
    .options =  WDIOF_SETTIMEOUT |
                WDIOF_KEEPALIVEPING |
                WDIOF_MAGICCLOSE,
    .firmware_version = 0,
    .identity = "Hisilicon Watchdog",
};

/* local var */
static struct osal_spinlock hidog_lock;
static int cur_margin = HIDOG_TIMER_MARGIN;
//static pid_t pid_hidog_deamon = -1;
#ifdef __HuaweiLite__
pthread_t task_hidog_deamon = 0;
#else
struct osal_task *task_hidog_deamon = NULL;
#endif


#define HIDOG_EXIT      0
#define HIDOG_SELFCLR   1
#define HIDOG_EXTCLR    2
volatile static unsigned int hidog_state = 0;

static osal_atomic_t driver_open;
//static int orphan_timer = 0;
static int options = WDIOS_ENABLECARD;

#ifndef MHZ
#define MHZ (1000*1000)
#endif

static unsigned long rate = 3*MHZ;

static inline void hidog_set_timeout(unsigned int nr)
{
    unsigned long cnt = (~0x0)/rate;        /* max cnt */
    unsigned long flags; 

    osal_spin_lock_irqsave(&hidog_lock, &flags); 

    if( nr==0 || nr>cnt)
        cnt=~0x0; 
    else
        cnt = nr*rate;
    /* unlock watchdog registers */ 
    hiwdt_writel(HIWDT_UNLOCK_VAL, HIWDT_LOCK); 
    hiwdt_writel(cnt, HIWDT_LOAD); 
    hiwdt_writel(cnt, HIWDT_VALUE); 
    /* lock watchdog registers */ 
    hiwdt_writel(0, HIWDT_LOCK); 
    osal_spin_unlock_irqrestore(&hidog_lock, &flags); 
};

static inline void hidog_feed(void)
{
    unsigned long flags; 

    osal_spin_lock_irqsave(&hidog_lock, &flags); 
    /* unlock watchdog registers */ 
    hiwdt_writel(HIWDT_UNLOCK_VAL, HIWDT_LOCK); 
    /* clear watchdog */ 
    hiwdt_writel(0x00, HIWDT_INTCLR); 
    /* lock watchdog registers */ 
    hiwdt_writel(0, HIWDT_LOCK); 
    osal_spin_unlock_irqrestore(&hidog_lock, &flags); 
};

static int hidog_set_heartbeat(int t)
{
    int ret = 0;
    unsigned int cnt = (~0x0)/rate; 

    if( t==0 ) {
        osal_printk(HIDOG_PFX "set heartbeat to 0, heartbeat will not be changed.\n");
        t = cur_margin;
        ret = 1;
    } else if( t>cnt ) {
        osal_printk(HIDOG_PFX "set heartbeat range error, t = %d\n", t);
        osal_printk(HIDOG_PFX "force heartbeat to %d\n", cnt);
        t = cnt;
        ret = -1;
    }

    cur_margin = t;

    hidog_set_timeout(t);
    hidog_feed();

    //if(NULL != task_hidog_deamon)
    //    osal_wake_up_process(task_hidog_deamon);    

    return ret;
}

static int hidog_keepalive(void)
{
    hidog_feed();
    return 0;
}

static inline void hidog_start(void)
{
    unsigned long flags;
    //unsigned long t;
    
    osal_spin_lock_irqsave(&hidog_lock, &flags);
    /* unlock watchdog registers */
    hiwdt_writel(HIWDT_UNLOCK_VAL, HIWDT_LOCK);
    hiwdt_writel(0x00, HIWDT_CTRL);
    hiwdt_writel(0x00, HIWDT_INTCLR);
    hiwdt_writel(0x03, HIWDT_CTRL);
    /* lock watchdog registers */
    hiwdt_writel(0, HIWDT_LOCK);
    /* enable watchdog clock --- set the frequency to 3MHz */
    //t = osal_readl(reg_ctl_base_va);
    //osal_writel(t & ~0x00800000, reg_ctl_base_va);
    osal_spin_unlock_irqrestore(&hidog_lock, &flags);
    
    options = WDIOS_ENABLECARD;
}

static inline void hidog_stop(void)
{
    unsigned long flags;

    osal_spin_lock_irqsave(&hidog_lock, &flags);

    /* unlock watchdog registers */
    hiwdt_writel(HIWDT_UNLOCK_VAL, HIWDT_LOCK);

    /* stop watchdog timer */
    hiwdt_writel(0x00, HIWDT_CTRL);
    hiwdt_writel(0x00, HIWDT_INTCLR);

    /* lock watchdog registers */
    hiwdt_writel(0, HIWDT_LOCK);

    osal_spin_unlock_irqrestore(&hidog_lock, &flags);

    hidog_set_timeout(0);

    options = WDIOS_DISABLECARD;
}

//extern struct module __this_module;

static int hidog_open(void *private_data)
{
    int ret = 0;

    if(0 != osal_atomic_dec_return(&driver_open) )
    {    
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
     *    Activate timer
     */
    hidog_keepalive();


    return ret;
}


static int hidog_release(void *private_data)
{
    if(1 != osal_atomic_inc_return(&driver_open) )
    {
       osal_atomic_dec_return(&driver_open);
       return 0;
    }

    /*
     *    Shut off the timer.
     *     Lock it in if it's a module and we set nowayout
     */
//    if (nowayout == 0) {
        //cur_margin = default_margin;
        hidog_state = HIDOG_SELFCLR;
        hidog_set_heartbeat(cur_margin);
        //osal_module_put(&__this_module);
//    } else {
//        osal_printk(HIDOG_PFX "Unexpected close, not stopping watchdog!\n");
        //set_bit(0, &orphan_timer);
//        hidog_keepalive();
//    }
    //clear_bit(0, &driver_open);

    if(options == WDIOS_DISABLECARD)
        osal_printk(HIDOG_PFX "Watchdog is disabled!\n");

    return 0;
}

#if 0
static int hidog_write(const char *data, int len, long *ppos, void *private_data)
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
        hidog_keepalive();
    }

    return len;
}
#endif

static long hidog_ioctl (unsigned int cmd, unsigned long arg, void *private_data)
{
    void *argp = (void *)arg;
    int *p = argp;
    int new_margin;
    int new_options;

    switch (cmd) {
        case WDIOC_GETSUPPORT:
            osal_memcpy(argp, &ident, sizeof(ident));
            return 0;
            
        case WDIOC_GETSTATUS:
        case WDIOC_GETBOOTSTATUS:
            *p = options;
            return 0;
            
        case WDIOC_KEEPALIVE:
            hidog_keepalive();
            return 0;
            
        case WDIOC_SETTIMEOUT:
            new_margin = *p;
            if (hidog_set_heartbeat(new_margin))
                return -1;
            hidog_keepalive();
            return 0;

        case WDIOC_GETTIMEOUT:
            *p = cur_margin;
            return 0;

        case WDIOC_SETOPTIONS:
            new_options = *p;
            if(new_options & WDIOS_ENABLECARD) {
                hidog_start();
                hidog_set_heartbeat(cur_margin);
                return 0;
            } else if (new_options & WDIOS_DISABLECARD) {
                hidog_stop();
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
static int hidog_notifier_sys(struct osal_notifier_block *this, unsigned long code,
    void *unused)
{
    //if(code==SYS_DOWN || code==SYS_HALT) {
        /* Turn the WDT off */
        hidog_stop();
    //}
    return 0;
}
#endif
/*
 *    Kernel Interfaces
 */

static struct osal_fileops hidog_fops = { 
//    .llseek         = no_llseek,        
//    .write          = hidog_write,        
    .unlocked_ioctl = hidog_ioctl,        
    .open           = hidog_open,         
    .release        = hidog_release,      
};                                        

static struct osal_dev *hidog_miscdev =  NULL;

#if 0
static struct osal_notifier_block hidog_notifier = {
    .notifier_call    = hidog_notifier_sys,
};
#endif

static char banner[] = "Hisilicon Watchdog Timer: 0.01 initialized. default_margin=%d sec (nodeamon= %d)\n";

static int hidog_deamon(void *data)
{
    #ifdef __HuaweiLite__
    prctl(PR_SET_NAME, "hidog_deamon", 0, 0, 0); 
    #endif
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
        /* sleep */
        /* when self feed dog, only use the default margin */
        osal_msleep(default_margin*1000/2+10);
    }

    return 0;
}

static int hidog_init(void)
{
    hidog_start();

    hidog_state = HIDOG_SELFCLR;
    if(!nodeamon) {

#ifdef __HuaweiLite__
        if(pthread_create(&task_hidog_deamon, NULL, (void *)hidog_deamon, 0) <0) 
        {
            osal_printk("---create hidog_deamon failed!\n");
            return -1;
        }
#else
        struct osal_task *p_dog =  NULL;
		p_dog = osal_kthread_create(hidog_deamon, NULL, "hidog");
		if(NULL == p_dog) {
			osal_printk(HIDOG_PFX "~~~create hidog_deamon failed!\n");
			return -1;
		}
		task_hidog_deamon = p_dog;
#endif
    }
    //osal_printk(OSDRV_MODULE_VERSION_STRING);
    return 0;
}

int watchdog_init(void)
{
    int ret = 0;

    ret = osal_atomic_init(&driver_open);
    if (ret)
    {
        osal_printk("Error: init atomic\n");
        return -1;
    }
    osal_atomic_set(&driver_open, 1);

    ret = osal_spin_lock_init(&hidog_lock);
    if (0 != ret)
    {
        osal_printk("function %s line %u failed\n", __FUNCTION__, __LINE__);
        goto watchdog_init_err0;
    }

    if( gpWtdgAllReg == NULL)
    {
        gpWtdgAllReg = (volatile void *)osal_ioremap_nocache(HIWDT_BASE, 0x1000);

        if (!gpWtdgAllReg)
        {
            osal_printk("osal_ioremap_nocache err. \n");
            goto watchdog_init_err1;
        }
    }
    
   // reg_ctl_base_va = (void *)OSAL_IO_ADDRESS(HISILICON_SCTL_BASE);
   // if (NULL == reg_ctl_base_va)
   // {
   //     osal_printk("function %s line %u failed\n", 
   //         __FUNCTION__, __LINE__);   
   //     goto watchdog_init_err2;
   // }
    
    cur_margin = default_margin;

    /* Check that the default_margin value is within it's range ; if not reset to the default */
    if(hidog_set_heartbeat(default_margin)) {
        default_margin = HIDOG_TIMER_MARGIN;
        hidog_set_heartbeat(HIDOG_TIMER_MARGIN);
        osal_printk(HIDOG_PFX "default_margin value must be 0<default_margin<%lu, using %d\n",
            ~0x0/rate, HIDOG_TIMER_MARGIN);
    }
#if 0
    ret = osal_register_reboot_notifier(&hidog_notifier);
    if(ret) {
        osal_printk(HIDOG_PFX "cannot register reboot notifier (err=%d)\n", ret);
        goto watchdog_init_err3;
    }
#endif
    hidog_miscdev = osal_createdev("watchdog");
    if(NULL == hidog_miscdev) {
        osal_printk("fail to create dev\n");
        goto watchdog_init_err4;
    }
	hidog_miscdev->minor = WATCHDOG_MINOR;
    hidog_miscdev->fops  = &hidog_fops;                                       

    ret = osal_registerdevice(hidog_miscdev);
    if(ret) 
	{                                                                     
        osal_printk (HIDOG_PFX "cannot register miscdev on minor=%d (err=%d)\n", WATCHDOG_MINOR, ret);                                                 
        goto watchdog_init_err5;                                                  
    }                                                                             

    ret = hidog_init();
    if(ret) {
        goto watchdog_init_err6;
    }
	
	osal_printk(banner, default_margin, nodeamon);
	
    //osal_printk(OSDRV_MODULE_VERSION_STRING "\n");
	osal_printk("hiwtdg init ok. ver=%s, %s.\n", __DATE__, __TIME__);
	
    return 0;
    
watchdog_init_err6:
    osal_deregisterdevice(hidog_miscdev);
watchdog_init_err5:
    osal_destroydev(hidog_miscdev);
watchdog_init_err4:
    //osal_unregister_reboot_notifier(&hidog_notifier);
#if 0
watchdog_init_err3:
    //osal_iounmap(reg_ctl_base_va);
watchdog_init_err2:
    //osal_iounmap(gpWtdgAllReg);
#endif
watchdog_init_err1:
    osal_spin_lock_destory(&hidog_lock);
watchdog_init_err0:
    osal_atomic_destory(&driver_open);
    return -1;
}

static void hidog_exit(void)
{
    hidog_set_timeout(0);
    hidog_stop();
    hidog_state=HIDOG_EXIT;

    if(!nodeamon) {
#ifdef __HuaweiLite__
        pthread_join(task_hidog_deamon, NULL);
#else        
        struct osal_task *p_dog = task_hidog_deamon;
        if(p_dog == NULL)
            return;
        //osal_wake_up_process(p_dog);
        osal_kthread_destory(p_dog);
#endif
        osal_yield(); 
    }

#ifdef __HuaweiLite__
    task_hidog_deamon = 0;
#else
    task_hidog_deamon = NULL;
#endif
}

void watchdog_exit(void)
{    
    hidog_exit();

    osal_deregisterdevice(hidog_miscdev);
    osal_destroydev(hidog_miscdev);
    //osal_unregister_reboot_notifier(&hidog_notifier);
    //osal_iounmap(reg_ctl_base_va);
    //reg_ctl_base_va = NULL;
	//osal_iounmap(gpWtdgAllReg);
	//gpWtdgAllReg = NULL;
    osal_spin_lock_destory(&hidog_lock);
    osal_atomic_destory(&driver_open);
    osal_printk("hiwtdg exit ok.\n");
}

MODULE_LICENSE("GPL");
