/*  isp/piris.c
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
 * along with this program;
 *
 * History:
 *      23-march-2011 create this file
 */
 
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/fcntl.h>

#include <linux/init.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/workqueue.h>
#include <linux/version.h>
#ifdef __HuaweiLite__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "hi_type.h"

#include <sys/ioctl.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#if 1
#include <asm/uaccess.h>
#include <linux/slab.h>
#endif

#else
#include <linux/hrtimer.h>
#endif
#include <asm/uaccess.h>
//#include <asm/system.h>
#include <asm/io.h>
#include <linux/slab.h>
#include "piris_hal.h"
#include "piris.h"
#include "isp_ext.h"

#include <linux/completion.h>

/* Piris use GPIO1_5, GPIO1_4, GPIO1_3, GPIO1_2 */
/* GPIO1's base address is 0x1214_1000*/
//#define PIRIS_DRV_DBG
#ifdef PIRIS_DRV_DBG
    #define PIRIS_DRV_PRINTF(fmt, args...)  printk(fmt, ##args)
#else
    #define PIRIS_DRV_PRINTF
#endif

#ifndef __iomem
#define __iomem
#endif
void __iomem* reg_pirisI_base_va[PIRIS_MAX_DEV_NUM];

#define HI_IO_PIRISI_ADDRESS(dev, x)  (reg_pirisI_base_va[dev] + ((x)-(PIRISI_ADRESS_BASE)))

#define PIRIS_CFG_REG0(dev)        HI_IO_PIRISI_ADDRESS(dev, PIRISI_ADRESS_BASE + 0x00F0)
#define PIRIS_CTRL_REG0(dev)       HI_IO_PIRISI_ADDRESS(dev, PIRISI_ADRESS_BASE + 0x0400)

#define  PIRIS_WRITE_REG(Addr, Value) ((*(volatile unsigned int *)(Addr)) = (Value))
#define  PIRIS_READ_REG(Addr)         (*(volatile unsigned int *)(Addr))

#ifndef MAX
#define MAX(a, b) (a > b? a : b)
#endif
#ifndef MIN
#define MIN(a, b) (a < b? a : b)
#endif
#define MAX_MOTOR_PAHSE 4
#define MAX_STEPS 92
#define PIRIS_PPS 100

static const unsigned char motor_phase_tbl[MAX_MOTOR_PAHSE] = { 0x0, 0x1, 0x2, 0x3};

typedef struct hiPIRIS_DEV
{
    int src_pos;
    int dest_pos;
    unsigned int pps;
    int phase;
    const unsigned char* phase_tbl;
	spinlock_t lock;
    struct semaphore sem;
#ifdef __HuaweiLite__	
    UINT16 u16TimerID;
#else
	struct timer_list timer;
#endif
    struct completion piris_comp;
	
} PIRIS_DEV;

static PIRIS_DEV* p_piris_dev[PIRIS_MAX_DEV_NUM];


int piris_gpio_update(int dev, int* pPirisPos)
{
    unsigned long u32Flags;
    PIRIS_DEV *pstPiris;
    
    pstPiris = p_piris_dev[dev];

    spin_lock_irqsave(&pstPiris->lock, u32Flags);
    pstPiris->dest_pos = *pPirisPos;
    /* according to Piris' introduction, it need to be stopped at phase "0" and "2". */
    //p_piris_dev->dest_pos = (p_piris_dev->dest_pos >> 1) << 1;        

#ifndef __HuaweiLite__
    pstPiris->pps = PIRIS_PPS;
    pstPiris->pps = MAX(MIN(pstPiris->pps, HZ), 1);
    pstPiris->timer.expires = jiffies + HZ / pstPiris->pps;

    /* whether piris timer already at the kerbel timer pending list */
    //if (p_piris_dev->timer.entry.next != NULL)
    //{
    //    spin_unlock_irqrestore(&p_piris_dev->lock, u32Flags);
    //    return -1;
    //}

    if (!timer_pending(&pstPiris->timer))
    {
        add_timer(&pstPiris->timer);
    }
#endif
    spin_unlock_irqrestore(&pstPiris->lock, u32Flags);
    return 0;
}


#define HYS_STEPS 100

/* first go to the full open iris step, set the full open as origin */
#ifdef __HuaweiLite__
static
#endif
int piris_origin_set(int dev, PIRIS_DATA_S* pstPirisData)
{
    int piris_pos;
    unsigned long u32Flags;
    PIRIS_DEV *pstPiris;
	int sign, hys_offset, hys_cnt;
    
	hys_cnt = 3;	// should be even
    pstPiris = p_piris_dev[dev];
    piris_pos = pstPirisData->CurPos;

    piris_gpio_update(dev, &piris_pos);

    // wait for piris origin done
    init_completion(&pstPiris->piris_comp);
    wait_for_completion(&pstPiris->piris_comp);

	/* consider hysteresis effection */
	sign = (pstPirisData->ZeroIsMax)? 1 : -1;
	do 
	{
		hys_offset = sign * HYS_STEPS;
		piris_pos += hys_offset;
		piris_gpio_update(dev, &piris_pos);
		wait_for_completion(&pstPiris->piris_comp);
		sign = -sign;
		hys_cnt--;
	} while (hys_cnt);

#ifdef __HuaweiLite__
	LOS_TaskDelay(1*HZ);
#else
	msleep(1000);
#endif
	piris_pos += pstPirisData->TotalStep * sign;
	piris_gpio_update(dev, &piris_pos);
	wait_for_completion(&pstPiris->piris_comp);

	/* then offset to full open positon */
    spin_lock_irqsave(&pstPiris->lock, u32Flags);
    if (pstPirisData->ZeroIsMax == 1)
    {
        pstPiris->src_pos  = 0;
        pstPiris->dest_pos = 0;
    }
    else
    {
        pstPiris->src_pos  = pstPirisData->TotalStep - 1;
        pstPiris->dest_pos = pstPirisData->TotalStep - 1;
    }

    spin_unlock_irqrestore(&pstPiris->lock, u32Flags);
    return 0;

}


/* go to the full close iris step */
#ifdef __HuaweiLite__
static
#endif
int piris_close_set(int dev, PIRIS_DATA_S* pstPirisData)
{
    int piris_pos;
    unsigned long u32Flags;
    PIRIS_DEV *pstPiris;
    
    pstPiris = p_piris_dev[dev];
    piris_pos = pstPirisData->CurPos;

    piris_gpio_update(dev, &piris_pos);

    // wait for piris origin done
    init_completion(&pstPiris->piris_comp);
    wait_for_completion(&pstPiris->piris_comp);

    spin_lock_irqsave(&pstPiris->lock, u32Flags);
    if (pstPirisData->ZeroIsMax == 1)
    {
        pstPiris->src_pos  = pstPirisData->TotalStep - 1;
        pstPiris->dest_pos = pstPirisData->TotalStep - 1;
    }
    else
    {
        pstPiris->src_pos  = 0;
        pstPiris->dest_pos = 0;
    }

    spin_unlock_irqrestore(&pstPiris->lock, u32Flags);
    return 0;

}


/* file operation */
#ifdef __HuaweiLite__
int piris_open(struct file* filep)
{
    return 0 ;
}

int  piris_close(struct file* filep)
{
    return 0;
}
#else
int piris_open(struct inode* inode, struct file* file)
{
    if (!capable(CAP_SYS_RAWIO))
            return -EPERM;
    return 0 ;
}
int  piris_close(struct inode* inode, struct file* file)
{
    return 0;
}
#endif

#if 0
static int PIRIS_DRV_Disable(void)
{
    PIRIS_WRITE_REG(PIRIS_CTRL_REG0, 0x80);
    PIRIS_WRITE_REG(PIRIS_CFG_REG0 , 0x0);
    PIRIS_WRITE_REG(PIRIS_CTRL_REG1, 0x7);
    PIRIS_WRITE_REG(PIRIS_CFG_REG1 , 0x0);
	return 0;
}
#endif

int PIRIS_DRV_Write(int dev, unsigned char bits)
{
    
    switch (bits)
    {
        case 0:
            PIRIS_WRITE_REG(PIRIS_CTRL_REG0(dev), 0x3C);
            PIRIS_WRITE_REG(PIRIS_CFG_REG0(dev) , PIRIS_CASE0_REG0);
            break;

        case 1:
            PIRIS_WRITE_REG(PIRIS_CTRL_REG0(dev), 0x3C);
            PIRIS_WRITE_REG(PIRIS_CFG_REG0(dev) , PIRIS_CASE1_REG0);
            break;

        case 2:
            PIRIS_WRITE_REG(PIRIS_CTRL_REG0(dev), 0x3C);
            PIRIS_WRITE_REG(PIRIS_CFG_REG0(dev) , PIRIS_CASE2_REG0);
            break;

        case 3:
            PIRIS_WRITE_REG(PIRIS_CTRL_REG0(dev), 0x3C);
            PIRIS_WRITE_REG(PIRIS_CFG_REG0(dev) , PIRIS_CASE3_REG0);
            break;

        default:
            break;
    }


    return 0;
}


static long piris_ioctl(struct file* file, unsigned int cmd, unsigned long arg)
{
    int __user* pPirisPos;
    PIRIS_DATA_S __user* argp;
    PIRIS_STATUS_E __user* pPirisStatus;
#ifdef __HuaweiLite__
    int dev = (int)(file->f_priv);
#else
    int dev = (int)(file->private_data);
#endif
    PIRIS_DEV* pstPiris; 
    int err;

    dev = MAX(MIN(dev, 1), 0);
    pstPiris = p_piris_dev[dev];
    err = 0;

    if (_IOC_TYPE(cmd) != PIRIS_IOC_MAGIC)
    {
        return -ENOTTY;
    }
    if (_IOC_NR(cmd) > PIRIS_IOC_MAXNR)
    {
        return -ENOTTY;
    }
#ifndef __HuaweiLite__
    if (_IOC_DIR(cmd) & _IOC_READ)
    {
        err = !access_ok(VERIFY_WRITE, (void __user*)arg, _IOC_SIZE(cmd));
    }
    else if (_IOC_DIR(cmd) & _IOC_WRITE)
    {
        err =  !access_ok(VERIFY_READ, (void __user*)arg, _IOC_SIZE(cmd));
    }

    if (err)
    {
        return -EFAULT;
    }
#endif
    // lock pstPiris
    if (down_interruptible(&pstPiris->sem))
    {
        return -ERESTARTSYS;
    }

    //printk("[%s][%d] Dev:%#x\n", __FUNCTION__, __LINE__, dev);
    switch (cmd)
    {
        case PIRIS_SET_FD:
#ifdef __HuaweiLite__       
            file->f_priv = (void *)(*(int __user*)arg);  
#else
            file->private_data = (void *)(*(int __user*)arg);   
#endif
            break;
            
        case PIRIS_SET_ACT_ARGS:
            pPirisPos = (int __user*)arg;
            piris_gpio_update(dev, pPirisPos);
            break;

        case PIRIS_SET_ORGIN:
            argp = (PIRIS_DATA_S __user*)arg;
            piris_origin_set(dev, argp);
            break;

        case PIRIS_SET_CLOSE:
            argp = (PIRIS_DATA_S __user*)arg;
            piris_close_set(dev, argp);
            break;

        case PIRIS_GET_STATUS:
            pPirisStatus = (PIRIS_STATUS_E __user*)arg;

            if (pstPiris->dest_pos != pstPiris->src_pos)
            {
                *pPirisStatus = PIRIS_BUSY;
            }
            else
            {
                *pPirisStatus = PIRIS_IDLE;
                //PIRIS_DRV_Disable();
            }
            break;

        default:  /* redundant, as cmd was checked against MAXNR */
            break;
    }

    // unlock pstPiris
    up(&pstPiris->sem);

    return 0 ;
}


#ifdef __HuaweiLite__
static struct file_operations_vfs piris_fops =
{
   
    .ioctl = piris_ioctl  ,
    .open       = piris_open   ,
    .close    = piris_close  ,
};

int hi_piris_device_register(void)
{
	return register_driver("/dev/piris", &piris_fops, 0666, 0);
}

#else
static struct file_operations piris_fops =
{
    .owner      = THIS_MODULE,
    .unlocked_ioctl = piris_ioctl  ,
    .open       = piris_open   ,
    .release    = piris_close  ,
};
#endif
static struct miscdevice gstPirisDev =
{
    .minor   = MISC_DYNAMIC_MINOR,
    .name    = "piris"    ,
    .fops    = &piris_fops,
};

#ifdef __HuaweiLite__
void piris_timer_cb(UINT32 arg)
#else 
void piris_timer_cb(unsigned long arg)
#endif
{
    int sign = 1;
    int i;
    int dev = 0;
    unsigned char bits;
    unsigned long u32Flags;

    PIRIS_DEV* pstPiris = (PIRIS_DEV*)arg;

    spin_lock_irqsave(&pstPiris->lock, u32Flags);
    if (pstPiris->src_pos == pstPiris->dest_pos)
    {
        spin_unlock_irqrestore(&pstPiris->lock, u32Flags);
        return ;
    }

    sign = (pstPiris->dest_pos - pstPiris->src_pos < 0) ? -1 : 1;
    pstPiris->src_pos += sign;

    // close iris: 0->1->2->3->0;    open iris: 3->2->1->0->3
    pstPiris->phase = (pstPiris->phase + MAX_MOTOR_PAHSE + sign) % MAX_MOTOR_PAHSE;
    bits = pstPiris->phase_tbl[pstPiris->phase];

    for (i = 0; i < PIRIS_MAX_DEV_NUM; i++)
    {
        if (pstPiris == p_piris_dev[i])
        {
            dev = i;
            break;
        }
    }
    if (i >= PIRIS_MAX_DEV_NUM)
    {
        return ;
    }
    
    PIRIS_DRV_Write(dev, bits);

    if (pstPiris->dest_pos == pstPiris->src_pos)
    {
        complete(&pstPiris->piris_comp);
    }

#ifndef __HuaweiLite__
    pstPiris->timer.expires = jiffies + HZ / pstPiris->pps;
    if (!timer_pending(&pstPiris->timer))
    {
        add_timer(&pstPiris->timer);
    }
#endif
	
    //PIRIS_DRV_PRINTF("%s, pps: %d pos :%d\n",  __FUNCTION__, pstPiris->pps, pstPiris->src_pos);
	spin_unlock_irqrestore(&pstPiris->lock, u32Flags);

    return ;
}


static int hi_piris_isp_register(int dev)
{
    ISP_PIRIS_CALLBACK_S stPirisCb = {0};

    stPirisCb.pfn_piris_gpio_update = piris_gpio_update;
    if (CKFN_ISP_RegisterPirisCallBack())
    {
        CALL_ISP_RegisterPirisCallBack(dev, &stPirisCb);
    }
    else
    {
        printk("register piris_gpio_write_callback to isp failed, hi_piris init is failed!\n");
        return -1;
    }

    return 0;
}


//#ifdef __HuaweiLite__
static int hi_piris_isp_unregister(int dev)
{
    ISP_PIRIS_CALLBACK_S stPirisCb = {0};

    if (CKFN_ISP_RegisterPirisCallBack())
    {
        CALL_ISP_RegisterPirisCallBack(dev, &stPirisCb);
    }
    else
    {
        printk("unregister piris_gpio_write_callback to isp failed!\n");
        return -1;
    }

    return 0;
}
//#endif

/* module init and exit */
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,0,0)
int __init piris_init(void)
#else
int piris_init(void)
#endif
{
    int ret, i,j;
    PIRIS_DEV *pdev;

#ifdef __HuaweiLite__
    ret = hi_piris_device_register();
#else
    ret = misc_register(&gstPirisDev);
#endif

    if (ret != 0)
    {
        printk("register piris device failed with %#x!\n", ret);
        return -1;
    }

    
    for (i = 0; i < PIRIS_MAX_DEV_NUM; i++)
    {
    	if(!reg_pirisI_base_va[i])
    	{
        	reg_pirisI_base_va[i] = ioremap_nocache(PIRISI_ADRESS_BASE, 0x10000);
            if(!reg_pirisI_base_va[i])
            {
                for(j = 0;j<i;j++)
                {
                    iounmap(reg_pirisI_base_va[j]);
                }

            #ifdef __HuaweiLite__
                unregister_driver("/dev/piris");
            #else
        	    misc_deregister(&gstPirisDev);
            #endif	
                return -1;
            
            }

            
        
    	}
    }
    


    for (i = 0; i < PIRIS_MAX_DEV_NUM; i++)
    {
        pdev = kmalloc(sizeof(PIRIS_DEV), GFP_KERNEL);
        if (!pdev)
        {
            for(j = 0;j<i;j++)
            {
                kfree(p_piris_dev[j]);
            }

        #ifdef __HuaweiLite__
            unregister_driver("/dev/piris");
        #else
	        misc_deregister(&gstPirisDev);
        #endif
        
            for(j = 0;j<PIRIS_MAX_DEV_NUM;j++)
            {
            #ifdef __HuaweiLite__
                iounmap(reg_pirisI_base_va[j]);
            #else
                #if LINUX_VERSION_CODE < KERNEL_VERSION(4,0,0)
                iounmap(reg_pirisI_base_va[j]);
                #endif
            #endif	
            }
        
            return -1;
        }
        memset(pdev, 0x0, sizeof(PIRIS_DEV));
        p_piris_dev[i] = pdev;
    }

    
    for (i = 0; i < PIRIS_MAX_DEV_NUM; i++)
    {
        spin_lock_init(&p_piris_dev[i]->lock);
        sema_init(&p_piris_dev[i]->sem, 1);
        init_completion(&p_piris_dev[i]->piris_comp);

        // init time
#ifdef __HuaweiLite__
    	UINT32 u32Interval;	
        u32Interval = HZ / PIRIS_PPS;
        LOS_SwtmrCreate(u32Interval, LOS_SWTMR_MODE_PERIOD, piris_timer_cb, &p_piris_dev[i]->u16TimerID, (UINT32)p_piris_dev[i]);
        LOS_SwtmrStart(p_piris_dev[i]->u16TimerID);
#else
    	init_timer(&p_piris_dev[i]->timer);
        p_piris_dev[i]->timer.function = piris_timer_cb;
        p_piris_dev[i]->timer.data = (unsigned long)p_piris_dev[i];
        p_piris_dev[i]->timer.expires = jiffies + HZ; /* one second */
#endif
    	
        p_piris_dev[i]->phase_tbl = motor_phase_tbl;

        hi_piris_isp_register(i);

    }
    
#ifndef __HuaweiLite__    
    printk("load hi_piris.ko OK!!\n");
#endif    
    return 0;
}

static void __exit piris_exit(void)
{
    int i;
    PIRIS_DEV *pdev;
    
    for (i = 0; i < PIRIS_MAX_DEV_NUM; i++)
    {
        pdev = p_piris_dev[i];
#ifdef __HuaweiLite__
        LOS_SwTmrDelete(pdev->u16TimerID);
        sema_destory(&pdev->sem);
        iounmap(reg_pirisI_base_va[i]);
#else
	    del_timer(&pdev->timer);
    #if LINUX_VERSION_CODE < KERNEL_VERSION(4,0,0)
        iounmap(reg_pirisI_base_va[i]);
    #endif
#endif	
        kfree(pdev);

        hi_piris_isp_unregister(i);
    }
    
#ifdef __HuaweiLite__
    unregister_driver("/dev/piris");
#else
	misc_deregister(&gstPirisDev);
    printk("remove hi_piris.ko OK!!\n");
#endif

}


#ifndef __HuaweiLite__
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,0,0)
module_init(piris_init);
module_exit(piris_exit);

MODULE_DESCRIPTION("piris driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("hisilicon");

#else
#include <linux/of_platform.h>
static int piris_probe(struct platform_device *pdev)
{
	struct resource *mem;
    int i;

    for (i = 0; i < PIRIS_MAX_DEV_NUM; i++)
    {
        mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
        reg_pirisI_base_va[i] = devm_ioremap_resource(&pdev->dev, mem);
        if (IS_ERR(reg_pirisI_base_va[i]))
                return PTR_ERR(reg_pirisI_base_va[i]);

        //printk("++++++++++ reg_pirisI_base_va = %p\n",reg_pirisI_base_va[i]);
    }    

    piris_init();
    return 0;
}
static int piris_remove(struct platform_device *pdev)
{
    printk("<%s> is called\n",__FUNCTION__);
    piris_exit();
    return 0;
}
static const struct of_device_id piris_match[] = {
        { .compatible = "hisilicon,piris" },
        {},
};
MODULE_DEVICE_TABLE(of, piris_match);
static struct platform_driver piris_driver = {
        .probe          = piris_probe,
        .remove         = piris_remove,
        .driver         = {
                .name   = "piris",
                .of_match_table = piris_match,
        }
};
module_platform_driver(piris_driver);
MODULE_LICENSE("GPL");
#endif
#endif

