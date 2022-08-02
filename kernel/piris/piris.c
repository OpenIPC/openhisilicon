/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
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
#ifdef __LITEOS__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "type.h"

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
#include <asm/io.h>
#include <linux/slab.h>
#include "piris_hal.h"
#include "piris.h"
#include "isp_ext.h"
#include "mm_ext.h"

#include <linux/completion.h>

/* Piris use GPIO1_5, GPIO1_4, GPIO1_3, GPIO1_2 */
/* GPIO1's base address is 0x1214_1000 */
#ifdef PIRIS_DRV_DBG
#define PIRIS_DRV_PRINTF(fmt, args...)  osal_printk(fmt, ##args)
#else
#define PIRIS_DRV_PRINTF
#endif

#ifndef __iomem
#define __iomem
#endif
void __iomem *reg_pirisI_base_va[PIRIS_MAX_DEV_NUM];

#define IO_PIRISI_ADDRESS(dev, x)  ((GK_UINTPTR_T)reg_pirisI_base_va[dev] + ((x)-(PIRISI_ADRESS_BASE)))

#define PIRIS_CFG_REG0(dev)        IO_PIRISI_ADDRESS(dev, PIRISI_ADRESS_BASE + 0x03C0)
#define PIRIS_CTRL_REG0(dev)       IO_PIRISI_ADDRESS(dev, PIRISI_ADRESS_BASE + 0x0400)

#define  PIRIS_WRITE_REG(Addr, Value) ((*(volatile unsigned int *)(Addr)) = (Value))
#define  PIRIS_READ_REG(Addr)         (*(volatile unsigned int *)(Addr))

#ifndef MAX
#define MAX(a, b) (a > b? a : b)
#endif
#ifndef MIN
#define MIN(a, b) (a < b? a : b)
#endif
#define MAX_MOTOR_PAHSE 4
#define MAX_STEPS       92
#define PIRIS_PPS       100

static const unsigned char motor_phase_tbl[MAX_MOTOR_PAHSE] = { 0x0, 0x1, 0x2, 0x3};

typedef struct PIRIS_DEV {
    int src_pos;
    int dest_pos;
    unsigned int pps;
    int phase;
    const unsigned char *phase_tbl;
    spinlock_t lock;
    struct semaphore sem;

    struct timer_list timer;
    struct completion piris_comp;

} PIRIS_DEV;

static PIRIS_DEV *p_piris_dev[PIRIS_MAX_DEV_NUM];

GK_BOOL g_bPirisIoRemap = GK_FALSE;
int piris_gpio_update(int dev, const int *pPirisPos)
{
    unsigned long u32Flags;
    PIRIS_DEV *pstPiris;

    pstPiris = p_piris_dev[dev];

    spin_lock_irqsave(&pstPiris->lock, u32Flags);
    pstPiris->dest_pos = MAX(MIN(*pPirisPos, 200), -200);

#if 1
    pstPiris->pps = PIRIS_PPS;
    pstPiris->pps = MAX(MIN(pstPiris->pps, HZ), 1);
    pstPiris->timer.expires = jiffies + HZ / pstPiris->pps;

    if (!timer_pending(&pstPiris->timer)) {
        add_timer(&pstPiris->timer);
    }
#endif
    spin_unlock_irqrestore(&pstPiris->lock, u32Flags);
    return 0;
}


#define HYS_STEPS       100

/* first go to the full open iris step, set the full open as origin */
#ifdef __LITEOS__
static
#endif
int piris_origin_set(int dev, PIRIS_DATA_S *pstPirisData)
{
    int piris_pos;
    unsigned long u32Flags;
    PIRIS_DEV *pstPiris;
    int sign, hys_offset, hys_cnt;

    hys_cnt = 3;    // should be even
    pstPiris = p_piris_dev[dev];
    piris_pos = pstPirisData->CurPos;

    if ((piris_pos > 200) || (piris_pos < -200) || (piris_pos == p_piris_dev[dev]->src_pos)) {
        osal_printk("Invalid CurPos!\n");
        return -1;
    }

    if ((pstPirisData->TotalStep) > 400 || (pstPirisData->TotalStep == 0)) {
        osal_printk("Invalid TotalStep!\n");
        return -1;
    }

    piris_gpio_update(dev, &piris_pos);
    init_completion(&pstPiris->piris_comp);
    // wait for piris origin done
    wait_for_completion(&pstPiris->piris_comp);

    /* consider hysteresis effection */
    sign = (pstPirisData->ZeroIsMax) ? 1 : -1;
    do {
        hys_offset = sign * HYS_STEPS;
        piris_pos += hys_offset;
        piris_gpio_update(dev, &piris_pos);
        wait_for_completion(&pstPiris->piris_comp);
        sign = -sign;
        hys_cnt--;
    } while (hys_cnt);

#ifdef __LITEOS__
    LOS_TaskDelay(1 * HZ);
#else
    msleep(1000);
#endif

    piris_pos += pstPirisData->TotalStep * sign;
    piris_gpio_update(dev, &piris_pos);
    wait_for_completion(&pstPiris->piris_comp);

    /* then offset to full open positon */
    spin_lock_irqsave(&pstPiris->lock, u32Flags);
    if (pstPirisData->ZeroIsMax == 1) {
        pstPiris->src_pos  = 0;
        pstPiris->dest_pos = 0;
    } else {
        pstPiris->src_pos  = pstPirisData->TotalStep - 1;
        pstPiris->dest_pos = pstPirisData->TotalStep - 1;
    }
    spin_unlock_irqrestore(&pstPiris->lock, u32Flags);
    return 0;

}


/* go to the full close iris step */
#ifdef __LITEOS__
static
#endif
int piris_close_set(int dev, PIRIS_DATA_S *pstPirisData)
{
    int piris_pos;
    unsigned long u32Flags;
    PIRIS_DEV *pstPiris;

    pstPiris = p_piris_dev[dev];
    piris_pos = pstPirisData->CurPos;

    init_completion(&pstPiris->piris_comp);
    piris_gpio_update(dev, &piris_pos);
    // wait for piris origin done
    wait_for_completion(&pstPiris->piris_comp);

    spin_lock_irqsave(&pstPiris->lock, u32Flags);
    if (pstPirisData->ZeroIsMax == 1) {
        pstPiris->src_pos  = pstPirisData->TotalStep - 1;
        pstPiris->dest_pos = pstPirisData->TotalStep - 1;
    } else {
        pstPiris->src_pos  = 0;
        pstPiris->dest_pos = 0;
    }

    spin_unlock_irqrestore(&pstPiris->lock, u32Flags);
    return 0;
}


/* file operation */
#ifdef __LITEOS__
int piris_open(struct file *filep)
{
    return 0;
}

int  piris_close(struct file *filep)
{
    return 0;
}
#else
int piris_open(struct inode *inode, struct file *file)
{
    if (!capable(CAP_SYS_RAWIO)) {
        return -EPERM;
    }
    return 0;
}
int  piris_close(struct inode *inode, struct file *file)
{
    return 0;
}
#endif

int PIRIS_DRV_Write(int dev, unsigned char bits)
{
    switch (bits) {
        case 0:
            PIRIS_WRITE_REG(PIRIS_CTRL_REG0(dev), 0xF0);
            PIRIS_WRITE_REG(PIRIS_CFG_REG0(dev), PIRIS_CASE0_REG0);
            break;

        case 1:
            PIRIS_WRITE_REG(PIRIS_CTRL_REG0(dev), 0xF0);
            PIRIS_WRITE_REG(PIRIS_CFG_REG0(dev), PIRIS_CASE1_REG0);
            break;

        case 2:
            PIRIS_WRITE_REG(PIRIS_CTRL_REG0(dev), 0xF0);
            PIRIS_WRITE_REG(PIRIS_CFG_REG0(dev), PIRIS_CASE2_REG0);
            break;

        case 3:
            PIRIS_WRITE_REG(PIRIS_CTRL_REG0(dev), 0xF0);
            PIRIS_WRITE_REG(PIRIS_CFG_REG0(dev), PIRIS_CASE3_REG0);
            break;

        default:
            break;
    }


    return 0;
}
#ifdef __LITEOS__
static int piris_ioctl(struct file *file, int cmd, unsigned long arg)
#else
static long piris_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
#endif
{
    int ret;
    int PirisPos;
    PIRIS_DATA_S piris_data;
    PIRIS_STATUS_E  PirisStatus = PIRIS_IDLE;

#ifdef __LITEOS__
    int dev = (int)(file->f_priv);
#else
    long int dev = (long int)(GK_UINTPTR_T)(file->private_data);
#endif
    PIRIS_DEV *pstPiris;

    dev = MAX(MIN(dev, 1), 0);
    pstPiris = p_piris_dev[dev];

    if (_IOC_TYPE(cmd) != PIRIS_IOC_MAGIC) {
        return -ENOTTY;
    }
    if (_IOC_NR(cmd) > PIRIS_IOC_MAXNR) {
        return -ENOTTY;
    }
    if (0L == arg) {
        return -1;
    }
    // lock pstPiris
    if (down_interruptible(&pstPiris->sem)) {
        return -ERESTARTSYS;
    }

    switch (cmd) {
        case PIRIS_SET_FD:
#ifdef __LITEOS__
            if (read_user_linear_space_valid((GK_U8 *)arg, sizeof(int)) == GK_FALSE) {
                up(&pstPiris->sem);
                return -EFAULT;
            }
            ret = copy_from_user(&dev, (int __user *)arg, sizeof(int));
            if (ret) {
                up(&pstPiris->sem);
                return -EFAULT;
            }
            file->f_priv = (void *)(dev);
#else
            if (read_user_linear_space_valid((GK_U8 *)(GK_UINTPTR_T)arg, sizeof(long int)) == GK_FALSE) {
                up(&pstPiris->sem);
                return -EFAULT;
            }
            ret = copy_from_user(&dev, (long int __user *)(GK_UINTPTR_T)arg, sizeof(long int));
            if (ret) {
                up(&pstPiris->sem);
                return -EFAULT;
            }
            file->private_data = (void *)(GK_UINTPTR_T)(dev);
#endif
            break;

        case PIRIS_SET_ACT_ARGS:

            if (read_user_linear_space_valid((GK_U8 *)(GK_UINTPTR_T)arg, sizeof(int)) == GK_FALSE) {
                up(&pstPiris->sem);
                return -EFAULT;
            }

            ret = copy_from_user(&PirisPos, (int __user *)(GK_UINTPTR_T)arg, sizeof(int));
            if (ret) {
                up(&pstPiris->sem);
                return -EFAULT;
            }
            piris_gpio_update(dev, &PirisPos);
            break;

        case PIRIS_SET_ORGIN:
            if (read_user_linear_space_valid((GK_U8 *)(GK_UINTPTR_T)arg, sizeof(PIRIS_DATA_S)) == GK_FALSE) {
                up(&pstPiris->sem);
                return -EFAULT;
            }

            ret = copy_from_user(&piris_data, (PIRIS_DATA_S __user *)(GK_UINTPTR_T)arg, sizeof(PIRIS_DATA_S));
            if (ret) {
                up(&pstPiris->sem);
                return -EFAULT;
            }
            piris_origin_set(dev, &piris_data);
            break;

        case PIRIS_SET_CLOSE:
            if (read_user_linear_space_valid((GK_U8 *)(GK_UINTPTR_T)arg, sizeof(PIRIS_DATA_S)) == GK_FALSE) {
                up(&pstPiris->sem);
                return -EFAULT;
            }
            ret = copy_from_user(&piris_data, (PIRIS_DATA_S __user *)(GK_UINTPTR_T)arg, sizeof(PIRIS_DATA_S));
            if (ret) {
                up(&pstPiris->sem);
                return -EFAULT;
            }
            piris_close_set(dev, &piris_data);
            break;

        case PIRIS_GET_STATUS:
            if (pstPiris->dest_pos != pstPiris->src_pos) {
                PirisStatus = PIRIS_BUSY;
            } else {
                PirisStatus = PIRIS_IDLE;
            }

            if (write_user_linear_space_valid((GK_U8 *)(GK_UINTPTR_T)arg, sizeof(PIRIS_STATUS_E)) == GK_FALSE) {
                up(&pstPiris->sem);
                return -EFAULT;
            }

            ret = copy_to_user((PIRIS_STATUS_E __user *)(GK_UINTPTR_T)arg, &PirisStatus, sizeof(PIRIS_STATUS_E));
            if (ret) {
                up(&pstPiris->sem);
                return -EFAULT;
            }
            break;

        default:  /* redundant, as cmd was checked against MAXNR */
            break;
    }

    // unlock pstPiris
    up(&pstPiris->sem);

    return 0;
}

#ifdef __LITEOS__
static struct file_operations_vfs piris_fops = {
    .ioctl = piris_ioctl,
    .open  = piris_open,
    .close = piris_close,
};

int piris_device_register(void)
{
    return register_driver("/dev/piris", &piris_fops, 0666, 0);
}

#else
static struct file_operations piris_fops = {
    .owner          = THIS_MODULE,
    .unlocked_ioctl = piris_ioctl,
    .open           = piris_open,
    .release        = piris_close,
};
#endif

#ifndef __LITEOS__
static struct miscdevice gstPirisDev = {
    .minor   = MISC_DYNAMIC_MINOR,
    .name    = "piris",
    .fops    = &piris_fops,
};
#endif

#ifdef __LITEOS__
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

    PIRIS_DEV *pstPiris = (PIRIS_DEV *)(GK_UINTPTR_T)arg;

    spin_lock_irqsave(&pstPiris->lock, u32Flags);
    if (pstPiris->src_pos == pstPiris->dest_pos) {
        spin_unlock_irqrestore(&pstPiris->lock, u32Flags);
        return;
    }

    sign = (pstPiris->dest_pos - pstPiris->src_pos < 0) ? -1 : 1;
    pstPiris->src_pos += sign;

    // close iris: 0->1->2->3->0;    open iris: 3->2->1->0->3
    pstPiris->phase = (pstPiris->phase + MAX_MOTOR_PAHSE + sign) % MAX_MOTOR_PAHSE;
    bits = pstPiris->phase_tbl[pstPiris->phase];

    for (i = 0; i < PIRIS_MAX_DEV_NUM; i++) {
        if (pstPiris == p_piris_dev[i]) {
            dev = i;
            break;
        }
    }
    if (i >= PIRIS_MAX_DEV_NUM) {
        spin_unlock_irqrestore(&pstPiris->lock, u32Flags);
        return;
    }

    PIRIS_DRV_Write(dev, bits);

    if (pstPiris->dest_pos == pstPiris->src_pos) {
        complete(&pstPiris->piris_comp);
    }

    pstPiris->timer.expires = jiffies + HZ / pstPiris->pps;
    if (!timer_pending(&pstPiris->timer)) {
        add_timer(&pstPiris->timer);
    }

    spin_unlock_irqrestore(&pstPiris->lock, u32Flags);

    return;
}

static int piris_isp_register(int dev)
{
    ISP_PIRIS_CALLBACK_S stPirisCb = {0};

    stPirisCb.pfn_piris_gpio_update = piris_gpio_update;
    if ((FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP) != NULL) && CKFN_ISP_RegisterPirisCallBack()) {
        CALL_ISP_RegisterPirisCallBack(dev, &stPirisCb);
    } else {
        osal_printk("register piris_gpio_write_callback to isp failed, piris init is failed!\n");
        return -1;
    }

    return 0;
}

static int piris_isp_unregister(int dev)
{
    ISP_PIRIS_CALLBACK_S stPirisCb = {0};

    if ((FUNC_ENTRY(ISP_EXPORT_FUNC_S, MOD_ID_ISP) != NULL) && CKFN_ISP_RegisterPirisCallBack()) {
        CALL_ISP_RegisterPirisCallBack(dev, &stPirisCb);
    } else {
        osal_printk("unregister piris_gpio_write_callback to isp failed!\n");
        return -1;
    }

    return 0;
}

/* module init and exit */
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)
int __init piris_init(void)
#else
int piris_init(void)
#endif
{
    int ret, i, j;
    PIRIS_DEV *pdev = GK_NULL;

#ifdef __LITEOS__
    ret = piris_device_register();
#else
    ret = misc_register(&gstPirisDev);
#endif

    if (ret != 0) {
        osal_printk("register piris device failed with %#x!\n", ret);
        return -1;
    }


    for (i = 0; i < PIRIS_MAX_DEV_NUM; i++) {
        if (!reg_pirisI_base_va[i]) {
            reg_pirisI_base_va[i] = osal_ioremap(PIRISI_ADRESS_BASE, 0x10000);
            if (!reg_pirisI_base_va[i]) {
                for (j = 0; j < i; j++) {
                    osal_iounmap(reg_pirisI_base_va[j]);
                }

#ifdef __LITEOS__
                unregister_driver("/dev/piris");
#else
                misc_deregister(&gstPirisDev);
#endif
                g_bPirisIoRemap = GK_FALSE;
                return -1;

            }
        }
    }

    g_bPirisIoRemap = GK_TRUE;


    for (i = 0; i < PIRIS_MAX_DEV_NUM; i++) {
        pdev = kmalloc(sizeof(PIRIS_DEV), GFP_KERNEL);
        if (!pdev) {
            for (j = 0; j < i; j++) {
                kfree(p_piris_dev[j]);
                p_piris_dev[j] = GK_NULL;
            }

#ifdef __LITEOS__
            unregister_driver("/dev/piris");
#else
            misc_deregister(&gstPirisDev);
#endif

            for (j = 0; j < PIRIS_MAX_DEV_NUM; j++) {
                if (g_bPirisIoRemap == GK_TRUE) {
                    iounmap(reg_pirisI_base_va[j]);
                }
            }

            g_bPirisIoRemap = GK_FALSE;

            return -1;
        }
        osal_memset(pdev, 0x0, sizeof(PIRIS_DEV));
        p_piris_dev[i] = pdev;
    }


    for (i = 0; i < PIRIS_MAX_DEV_NUM; i++) {
        spin_lock_init(&p_piris_dev[i]->lock);
        sema_init(&p_piris_dev[i]->sem, 1);
        init_completion(&p_piris_dev[i]->piris_comp);

        init_timer(&p_piris_dev[i]->timer);
#ifndef __LITEOS__
        p_piris_dev[i]->timer.function = piris_timer_cb;
#else
        p_piris_dev[i]->timer.function = (void *)piris_timer_cb;
#endif

        p_piris_dev[i]->timer.data = (unsigned long)(GK_UINTPTR_T)p_piris_dev[i];
        p_piris_dev[i]->timer.expires = jiffies + HZ; /* one second */

        p_piris_dev[i]->phase_tbl = motor_phase_tbl;

        piris_isp_register(i);

    }

#if 1
    osal_printk("load piris.ko OK!!\n");
#endif
    return 0;
}

void __exit piris_exit(void)
{
    int i;
    PIRIS_DEV *pdev = GK_NULL;

    for (i = 0; i < PIRIS_MAX_DEV_NUM; i++) {
        pdev = p_piris_dev[i];
        del_timer(&pdev->timer);
        if (g_bPirisIoRemap == GK_TRUE) {
            iounmap(reg_pirisI_base_va[i]);
        }
        kfree(pdev);
        pdev = GK_NULL;
        piris_isp_unregister(i);
    }

    g_bPirisIoRemap = GK_FALSE;

#ifdef __LITEOS__
    unregister_driver("/dev/piris");
#else
    misc_deregister(&gstPirisDev);
    osal_printk("remove piris.ko OK!!\n");
#endif
}

#ifndef __LITEOS__
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)
module_init(piris_init);
module_exit(piris_exit);

MODULE_LICENSE("GPL");

#else
#include <linux/of_platform.h>
static int piris_probe(struct platform_device *pdev)
{
    struct resource *mem;
    int i;
    PIRIS_DEV *pdev;

    for (i = 0; i < PIRIS_MAX_DEV_NUM; i++) {
        mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
        reg_pirisI_base_va[i] = devm_ioremap_resource(&pdev->dev, mem);
        if (IS_ERR(reg_pirisI_base_va[i])) {
            return PTR_ERR(reg_pirisI_base_va[i]);
        }
    }

    piris_init();
    return 0;
}

static int piris_remove(struct platform_device *pdev)
{
    osal_printk("<%s> is called\n", __FUNCTION__);
    piris_exit();
    return 0;
}

static const struct of_device_id piris_match[] = {
    { .compatible = "goke,piris" },
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
