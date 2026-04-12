/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of piris.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "piris.h"
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
#include <asm/io.h>
#include <linux/slab.h>
#include "piris_hal.h"
#include "isp_ext.h"
#include "mm_ext.h"

#include <linux/completion.h>

/* Piris use GPIO1_5, GPIO1_4, GPIO1_3, GPIO1_2 */
/* GPIO1's base address is 0x1214_1000 */
#ifdef PIRIS_DRV_DBG
#define piris_drv_printf(fmt, args...)  osal_printk(fmt, ##args)
#else
#define PIRIS_DRV_PRINTF
#endif

#ifndef __iomem
#define __iomem
#endif
void __iomem *g_reg_piris_i_base[PIRIS_MAX_DEV_NUM];

#define hi_io_pirisi_address(dev, x)  ((hi_uintptr_t)g_reg_piris_i_base[dev] + ((x)-(PIRISI_ADRESS_BASE)))

#define piris_cfg_reg0(dev)        hi_io_pirisi_address(dev, PIRISI_ADRESS_BASE + 0x01E0)
#define piris_ctrl_reg0(dev)       hi_io_pirisi_address(dev, PIRISI_ADRESS_BASE + 0x0400)

#define  piris_write_reg(addr, value) ((*(volatile unsigned int *)(addr)) = (value))
#define  piris_read_reg(addr)         (*(volatile unsigned int *)(addr))

#define MAX_MOTOR_PAHSE 4
#define MAX_STEPS       92
#define PIRIS_PPS       100

static const unsigned char g_motor_phase_tbl[MAX_MOTOR_PAHSE] = { 0x0, 0x1, 0x2, 0x3};

typedef struct {
    int src_pos;
    int dest_pos;
    unsigned int pps;
    int phase;
    const unsigned char *phase_tbl;
    spinlock_t lock;
    struct semaphore sem;
    struct timer_list timer;
    struct completion piris_comp;
} piris_dev_s;

static piris_dev_s *g_piris_dev[PIRIS_MAX_DEV_NUM];

hi_bool g_piris_io_remap = HI_FALSE;
int piris_gpio_update(int dev, const int *piris_pos)
{
    unsigned long flags;
    piris_dev_s *piris;

    piris = g_piris_dev[dev];

    spin_lock_irqsave(&piris->lock, flags);
    piris->dest_pos = MAX2(MIN2(*piris_pos, 200), -200); /* range:[-200,200] */

#if 1
    piris->pps = PIRIS_PPS;
    piris->pps = MAX2(MIN2(piris->pps, HZ), 1);
    piris->timer.expires = jiffies;

    /* whether piris timer already at the kerbel timer pending list */
    if (!timer_pending(&piris->timer)) {
        add_timer(&piris->timer);
    }
#endif
    spin_unlock_irqrestore(&piris->lock, flags);
    return 0;
}

#define HYS_STEPS       100

/* first go to the full open iris step, set the full open as origin */
#ifdef __HuaweiLite__
static int piris_origin_set(int dev, piris_data *piris_data_info)
#else
int piris_origin_set(int dev, piris_data *piris_data_info)
#endif
{
    int piris_pos;
    unsigned long flags;
    piris_dev_s *piris;
    int sign, hys_offset, hys_cnt;

    hys_cnt = 3;    /* should be even, default as 3 */
    piris = g_piris_dev[dev];
    piris_pos = piris_data_info->cur_pos;

    if ((piris_pos > 200) || (piris_pos < -200) || (piris_pos == g_piris_dev[dev]->src_pos)) { /* pos: [-200, 200] */
        osal_printk("Invalid cur_pos!\n");
        return -1;
    }

    if ((piris_data_info->total_step) > 400 || (piris_data_info->total_step == 0)) { /* total_step range (0, 400] */
        osal_printk("Invalid total_step!\n");
        return -1;
    }

    piris_gpio_update(dev, &piris_pos);
    init_completion(&piris->piris_comp);
    /* wait for piris origin done */
    wait_for_completion(&piris->piris_comp);

    /* consider hysteresis effection */
    sign = (piris_data_info->zero_is_max) ? 1 : -1;
    do {
        hys_offset = sign * HYS_STEPS;
        piris_pos += hys_offset;
        piris_gpio_update(dev, &piris_pos);
        wait_for_completion(&piris->piris_comp);
        sign = -sign;
        hys_cnt--;
    } while (hys_cnt);

#ifdef __HuaweiLite__
    LOS_TaskDelay(1 * HZ);
#else
    msleep(1000); /* system sleep for 1000 ms (1s) */
#endif

    piris_pos += piris_data_info->total_step * sign;
    piris_gpio_update(dev, &piris_pos);
    wait_for_completion(&piris->piris_comp);

    /* then offset to full open positon */
    spin_lock_irqsave(&piris->lock, flags);
    if (piris_data_info->zero_is_max == 1) {
        piris->src_pos  = 0;
        piris->dest_pos = 0;
    } else {
        piris->src_pos  = piris_data_info->total_step - 1;
        piris->dest_pos = piris_data_info->total_step - 1;
    }
    spin_unlock_irqrestore(&piris->lock, flags);
    return 0;
}

/* go to the full close iris step */
#ifdef __HuaweiLite__
static int piris_close_set(int dev, piris_data *piris_data_info)
#else
int piris_close_set(int dev, piris_data *piris_data_info)
#endif
{
    int piris_pos;
    unsigned long flags;
    piris_dev_s *piris;

    piris = g_piris_dev[dev];
    piris_pos = piris_data_info->cur_pos;

    init_completion(&piris->piris_comp);
    piris_gpio_update(dev, &piris_pos);
    /* wait for piris origin done */
    wait_for_completion(&piris->piris_comp);

    spin_lock_irqsave(&piris->lock, flags);
    if (piris_data_info->zero_is_max == 1) {
        piris->src_pos  = piris_data_info->total_step - 1;
        piris->dest_pos = piris_data_info->total_step - 1;
    } else {
        piris->src_pos  = 0;
        piris->dest_pos = 0;
    }

    spin_unlock_irqrestore(&piris->lock, flags);
    return 0;
}

/* file operation */
#ifdef __HuaweiLite__
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

int piris_drv_write(int dev, unsigned char bits)
{
    switch (bits) {
        case 0: /* case 0 */
            piris_write_reg(piris_ctrl_reg0(dev), 0x78);
            piris_write_reg(piris_cfg_reg0(dev), PIRIS_CASE0_REG0);
            break;

        case 1: /* case 1 */
            piris_write_reg(piris_ctrl_reg0(dev), 0x78);
            piris_write_reg(piris_cfg_reg0(dev), PIRIS_CASE1_REG0);
            break;

        case 2: /* case 2 */
            piris_write_reg(piris_ctrl_reg0(dev), 0x78);
            piris_write_reg(piris_cfg_reg0(dev), PIRIS_CASE2_REG0);
            break;

        case 3: /* case 3 */
            piris_write_reg(piris_ctrl_reg0(dev), 0x78);
            piris_write_reg(piris_cfg_reg0(dev), PIRIS_CASE3_REG0);
            break;

        default:
            break;
    }

    return 0;
}

#ifdef __HuaweiLite__
static int piris_ioctl(struct file *file, int cmd, unsigned long arg)
#else
static long piris_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
#endif
{
    int ret;
    int piris_pos;
    piris_data    piris_data_info;
    piris_status  piris_status_info;
    piris_dev_s   *piris;

#ifdef __HuaweiLite__
    int dev;
#else
    long int dev;
#endif

#ifdef __HuaweiLite__
    dev = (int)(file->f_priv);
#else
    dev = (long int)(hi_uintptr_t)(file->private_data);
#endif

    dev = MAX2(MIN2(dev, 1), 0);
    piris = g_piris_dev[dev];

    if (_IOC_TYPE(cmd) != PIRIS_IOC_MAGIC) {
        return -ENOTTY;
    }
    if (_IOC_NR(cmd) > PIRIS_IOC_MAXNR) {
        return -ENOTTY;
    }
    if (arg == 0L) {
        return -1;
    }
    /* lock piris */
    if (down_interruptible(&piris->sem)) {
        return -ERESTARTSYS;
    }

    switch (cmd) {
        case PIRIS_SET_FD:
#ifdef __HuaweiLite__
            if (read_user_linear_space_valid((hi_u8 *)(hi_uintptr_t)arg, sizeof(int)) == HI_FALSE) {
                up(&piris->sem);
                return -EFAULT;
            }

            ret = copy_from_user(&dev, (int __user *)(hi_uintptr_t)arg, sizeof(int));
            if (ret) {
                up(&piris->sem);
                return -EFAULT;
            }
            file->f_priv = (void *)(dev);
#else
            if (read_user_linear_space_valid((hi_u8 *)(hi_uintptr_t)arg, sizeof(long int)) == HI_FALSE) {
                up(&piris->sem);
                return -EFAULT;
            }
            ret = copy_from_user(&dev, (long int __user *)(hi_uintptr_t)arg, sizeof(long int));
            if (ret) {
                up(&piris->sem);
                return -EFAULT;
            }
            file->private_data = (void *)(hi_uintptr_t)(dev);
#endif
            break;

        case PIRIS_SET_ACT_ARGS:

            if (read_user_linear_space_valid((hi_u8 *)(hi_uintptr_t)arg, sizeof(int)) == HI_FALSE) {
                up(&piris->sem);
                return -EFAULT;
            }

            ret = copy_from_user(&piris_pos, (int __user *)(hi_uintptr_t)arg, sizeof(int));
            if (ret) {
                up(&piris->sem);
                return -EFAULT;
            }
            piris_gpio_update(dev, &piris_pos);
            break;

        case PIRIS_SET_ORGIN:
            if (read_user_linear_space_valid((hi_u8 *)(hi_uintptr_t)arg, sizeof(piris_data)) == HI_FALSE) {
                up(&piris->sem);
                return -EFAULT;
            }

            ret = copy_from_user(&piris_data_info, (piris_data __user *)(hi_uintptr_t)arg, sizeof(piris_data));
            if (ret) {
                up(&piris->sem);
                return -EFAULT;
            }
            piris_origin_set(dev, &piris_data_info);
            break;

        case PIRIS_SET_CLOSE:
            if (read_user_linear_space_valid((hi_u8 *)(hi_uintptr_t)arg, sizeof(piris_data)) == HI_FALSE) {
                up(&piris->sem);
                return -EFAULT;
            }
            ret = copy_from_user(&piris_data_info, (piris_data __user *)(hi_uintptr_t)arg, sizeof(piris_data));
            if (ret) {
                up(&piris->sem);
                return -EFAULT;
            }
            piris_close_set(dev, &piris_data_info);
            break;

        case PIRIS_GET_STATUS:
            if (piris->dest_pos != piris->src_pos) {
                piris_status_info = PIRIS_BUSY;
            } else {
                piris_status_info = PIRIS_IDLE;
            }

            if (write_user_linear_space_valid((hi_u8 *)(hi_uintptr_t)arg, sizeof(piris_status)) == HI_FALSE) {
                up(&piris->sem);
                return -EFAULT;
            }

            ret = copy_to_user((piris_status __user *)(hi_uintptr_t)arg, &piris_status_info, sizeof(piris_status));
            if (ret) {
                up(&piris->sem);
                return -EFAULT;
            }
            break;

        default:  /* redundant, as cmd was checked against MAXNR */
            break;
    }

    /* unlock piris */
    up(&piris->sem);

    return 0;
}

#ifdef __HuaweiLite__
static struct file_operations_vfs g_piris_fops = {
    .ioctl = piris_ioctl,
    .open  = piris_open,
    .close = piris_close,
};

int hi_piris_device_register(void)
{
    return register_driver("/dev/piris", &g_piris_fops, 0666, 0); /* register addr, 0666 */
}

#else
static struct file_operations g_piris_fops = {
    .owner          = THIS_MODULE,
    .unlocked_ioctl = piris_ioctl,
    .open           = piris_open,
    .release        = piris_close,
};
#endif

#ifndef __HuaweiLite__
static struct miscdevice g_st_piris_dev = {
    .minor   = MISC_DYNAMIC_MINOR,
    .name    = "piris",
    .fops    = &g_piris_fops,
};
#endif

#ifdef __HuaweiLite__
void piris_timer_cb(UINT32 arg)
#else
void piris_timer_cb(unsigned long arg)
#endif
{
    int sign;
    int i;
    int dev = 0;
    unsigned char bits;
    unsigned long flags;
    piris_dev_s *piris = HI_NULL;

    piris = (piris_dev_s *)(hi_uintptr_t)arg;
    if (piris == HI_NULL) {
        return;
    }
    spin_lock_irqsave(&piris->lock, flags);
    if (piris->src_pos == piris->dest_pos) {
        spin_unlock_irqrestore(&piris->lock, flags);
        return;
    }

    sign = (piris->dest_pos - piris->src_pos < 0) ? -1 : 1;
    piris->src_pos += sign;

    /* close iris: 0->1->2->3->0;    open iris: 3->2->1->0->3 */
    piris->phase = (piris->phase + MAX_MOTOR_PAHSE + sign) % MAX_MOTOR_PAHSE;
    bits = piris->phase_tbl[piris->phase];

    for (i = 0; i < PIRIS_MAX_DEV_NUM; i++) {
        if (piris == g_piris_dev[i]) {
            dev = i;
            break;
        }
    }
    if (i >= PIRIS_MAX_DEV_NUM) {
        spin_unlock_irqrestore(&piris->lock, flags);
        return;
    }

    piris_drv_write(dev, bits);

    if (piris->dest_pos == piris->src_pos) {
        complete(&piris->piris_comp);
    }

    piris->timer.expires = jiffies;
    if (!timer_pending(&piris->timer)) {
        add_timer(&piris->timer);
    }

    spin_unlock_irqrestore(&piris->lock, flags);

    return;
}

static int hi_piris_isp_register(int dev)
{
    isp_piris_callback piris_cb = {0};

    piris_cb.pfn_piris_gpio_update = piris_gpio_update;
    if (ckfn_isp() && ckfn_isp_register_piris_callback()) {
        call_isp_register_piris_callback(dev, &piris_cb);
    } else {
        osal_printk("register piris_gpio_write_callback to isp failed, hi_piris init is failed!\n");
        return -1;
    }

    return 0;
}

static int hi_piris_isp_unregister(int dev)
{
    isp_piris_callback piris_cb = {0};

    if (ckfn_isp() && ckfn_isp_register_piris_callback()) {
        call_isp_register_piris_callback(dev, &piris_cb);
    } else {
        osal_printk("unregister piris_gpio_write_callback to isp failed!\n");
        return -1;
    }

    return 0;
}
#define PIRISI_SIZE_ALIGN 0x10000
int piris_init_remap(int i)
{
    int j;
    if (!g_reg_piris_i_base[i]) {
        g_reg_piris_i_base[i] = osal_ioremap(PIRISI_ADRESS_BASE, PIRISI_SIZE_ALIGN);
        if (!g_reg_piris_i_base[i]) {
            for (j = 0; j < i; j++) {
                osal_iounmap(g_reg_piris_i_base[j]);
            }

#ifdef __HuaweiLite__
            unregister_driver("/dev/piris");
#else
            misc_deregister(&g_st_piris_dev);
#endif
            g_piris_io_remap = HI_FALSE;
            return -1;
        }
    }

    return 0;
}

int piris_open_dev(int i, piris_dev_s *pdev)
{
    int j;

    if (!pdev) {
        for (j = 0; j < i; j++) {
            kfree(g_piris_dev[j]);
            g_piris_dev[j] = HI_NULL;
        }

#ifdef __HuaweiLite__
        unregister_driver("/dev/piris");
#else
        misc_deregister(&g_st_piris_dev);
#endif

        for (j = 0; j < PIRIS_MAX_DEV_NUM; j++) {
            if (g_piris_io_remap == HI_TRUE) {
                iounmap(g_reg_piris_i_base[j]);
            }
        }

        g_piris_io_remap = HI_FALSE;

        return -1;
    }
    osal_memset(pdev, 0x0, sizeof(piris_dev_s));
    g_piris_dev[i] = pdev;

    return 0;
}

/* module init and exit */
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)
int __init piris_init(void)
#else
int piris_init(void)
#endif
{
    int ret, i;
    piris_dev_s *pdev = HI_NULL;

#ifdef __HuaweiLite__
    ret = hi_piris_device_register();
#else
    ret = misc_register(&g_st_piris_dev);
#endif

    if (ret != 0) {
        osal_printk("register piris device failed with %#x!\n", ret);
        return -1;
    }

    /* initiate piris map */
    for (i = 0; i < PIRIS_MAX_DEV_NUM; i++) {
        ret = piris_init_remap(i);
        if (ret != 0) {
            osal_printk("initiate piris map failed at piris dev[%d]!\n", i);
            return -1;
        }
    }

    g_piris_io_remap = HI_TRUE;

    /* open dev */
    for (i = 0; i < PIRIS_MAX_DEV_NUM; i++) {
        pdev = kmalloc(sizeof(piris_dev_s), GFP_KERNEL);
        PIRIS_CHECK_POINTER(pdev);
        ret = piris_open_dev(i, pdev);
        if (ret != 0) {
            osal_printk("open piris dev failed at piris dev[%d]!\n", i);
            return -1;
        }
    }

    for (i = 0; i < PIRIS_MAX_DEV_NUM; i++) {
        spin_lock_init(&g_piris_dev[i]->lock);
        sema_init(&g_piris_dev[i]->sem, 1);
        init_completion(&g_piris_dev[i]->piris_comp);

        init_timer(&g_piris_dev[i]->timer);
#ifndef __HuaweiLite__
        g_piris_dev[i]->timer.function = piris_timer_cb;
#else
        g_piris_dev[i]->timer.function = (void *)piris_timer_cb;
#endif
        g_piris_dev[i]->timer.data = (unsigned long)(hi_uintptr_t)g_piris_dev[i];
        g_piris_dev[i]->timer.expires = jiffies + HZ; /* one second */
        g_piris_dev[i]->phase_tbl = g_motor_phase_tbl;

        hi_piris_isp_register(i);
    }

#if 1
    osal_printk("load hi_piris.ko for %s...OK!\n", CHIP_NAME);
#endif
    return 0;
}

void __exit piris_exit(void)
{
    int i;
    piris_dev_s *pdev = HI_NULL;

    for (i = 0; i < PIRIS_MAX_DEV_NUM; i++) {
        pdev = g_piris_dev[i];
        del_timer(&pdev->timer);
        if (g_piris_io_remap == HI_TRUE) {
            iounmap(g_reg_piris_i_base[i]);
        }
        kfree(pdev);
        pdev = HI_NULL;
        hi_piris_isp_unregister(i);
    }

    g_piris_io_remap = HI_FALSE;

#ifdef __HuaweiLite__
    unregister_driver("/dev/piris");
#else
    misc_deregister(&g_st_piris_dev);
    osal_printk("unload hi_piris.ko for %s...OK!\n", CHIP_NAME);
#endif
}

#ifndef __HuaweiLite__
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)
module_init(piris_init);
module_exit(piris_exit);

MODULE_DESCRIPTION("piris driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("hisilicon");

#else
#include <linux/of_platform.h>
static int piris_probe(struct platform_device *pdev)
{
    struct resource *mem = HI_NULL;
    int i;

    for (i = 0; i < PIRIS_MAX_DEV_NUM; i++) {
        mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
        g_reg_piris_i_base[i] = devm_ioremap_resource(&pdev->dev, mem);
        if (IS_ERR(g_reg_piris_i_base[i])) {
            return ptr_err(g_reg_piris_i_base[i]);
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

static const struct of_device_id g_piris_match[] = {
    { .compatible = "hisilicon,piris" },
    {},
};

MODULE_DEVICE_TABLE(of, g_piris_match);
static struct platform_driver g_piris_driver = {
    .probe          = piris_probe,
    .remove         = piris_remove,
    .driver         = {
        .name   = "piris",
        .of_match_table = g_piris_match,
    }
};

module_platform_driver(g_piris_driver);
MODULE_LICENSE("GPL");
#endif
#endif
