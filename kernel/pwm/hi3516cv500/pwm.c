/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of pwm.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "pwm.h"
#include <linux/module.h>
#include <linux/errno.h>
#ifndef CONFIG_HISI_SNAPSHOT_BOOT
#include <linux/miscdevice.h>
#endif
#include <linux/fcntl.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/workqueue.h>
#include <asm/uaccess.h>
#include <linux/version.h>
#include <asm/io.h>

#include "hi_osal.h"
#include "mm_ext.h"


#ifndef __HuaweiLite__
#ifdef CONFIG_HISI_SNAPSHOT_BOOT
#include "himedia.h"
#endif

#else
#ifndef __iomem
#define __iomem
#endif
#ifndef ENOIOCTLCMD
#define ENOIOCTLCMD     515 /* No ioctl command */
#endif
#endif

#include "pwm_arch.h"

void __iomem *g_reg_pwm_ibase = 0;

#define hi_io_pwmi_address(x)  ((void __iomem *)((unsigned char __iomem *)g_reg_pwm_ibase + ((x)-(PWMI_ADRESS_BASE))))

/* PWMI */
#define PWM0_CFG_REG0   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0000)
#define PWM0_CFG_REG1   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0004)
#define PWM0_CFG_REG2   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0008)
#define PWM0_CTRL_REG   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x000C)
#define PWM0_STATE_REG0 hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0010)
#define PWM0_STATE_REG1 hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0014)
#define PWM0_STATE_REG2 hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0018)

#define PWM1_CFG_REG0   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0020)
#define PWM1_CFG_REG1   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0024)
#define PWM1_CFG_REG2   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0028)
#define PWM1_CTRL_REG   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x002C)
#define PWM1_STATE_REG0 hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0030)
#define PWM1_STATE_REG1 hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0034)
#define PWM1_STATE_REG2 hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0038)

#define PWM2_CFG_REG0   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0040)
#define PWM2_CFG_REG1   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0044)
#define PWM2_CFG_REG2   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0048)
#define PWM2_CTRL_REG   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x004C)
#define PWM2_STATE_REG0 hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0050)
#define PWM2_STATE_REG1 hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0054)
#define PWM2_STATE_REG2 hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0058)

#define PWM3_CFG_REG0   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0020)
#define PWM3_CFG_REG1   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0024)
#define PWM3_CFG_REG2   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0028)
#define PWM3_CTRL_REG   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x002C)
#define PWM3_STATE_REG0 hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0030)
#define PWM3_STATE_REG1 hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0034)
#define PWM3_STATE_REG2 hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0038)

#define PWM4_CFG_REG0   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0080)
#define PWM4_CFG_REG1   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0084)
#define PWM4_CFG_REG2   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0088)
#define PWM4_CTRL_REG   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x008C)
#define PWM4_STATE_REG0 hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0090)
#define PWM4_STATE_REG1 hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0094)
#define PWM4_STATE_REG2 hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x0098)

#define PWM5_CFG_REG0   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x00A0)
#define PWM5_CFG_REG1   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x00A4)
#define PWM5_CFG_REG2   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x00A8)
#define PWM5_CTRL_REG   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x00AC)
#define PWM5_STATE_REG0 hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x00B0)
#define PWM5_STATE_REG1 hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x00B4)
#define PWM5_STATE_REG2 hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x00B8)

#define PWM6_CFG_REG0   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x00C0)
#define PWM6_CFG_REG1   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x00C4)
#define PWM6_CFG_REG2   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x00C8)
#define PWM6_CTRL_REG   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x00CC)
#define PWM6_STATE_REG0 hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x00D0)
#define PWM6_STATE_REG1 hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x00D4)
#define PWM6_STATE_REG2 hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x00D8)

#define PWM7_CFG_REG0   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x00E0)
#define PWM7_CFG_REG1   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x00E4)
#define PWM7_CFG_REG2   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x00E8)
#define PWM7_CTRL_REG   hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x00EC)
#define PWM7_STATE_REG0 hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x00F0)
#define PWM7_STATE_REG1 hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x00F4)
#define PWM7_STATE_REG2 hi_io_pwmi_address(PWMI_ADRESS_BASE + 0x00F8)

#define  pwm_write_reg(addr, value) ((*(volatile unsigned int *)(addr)) = (value))
#define  pwm_read_reg(addr)         (*(volatile unsigned int *)(addr))

/* PWM */
#define PWM_NUM_MAX     0x08
#define PWM_ENABLE      0x01
#define PWM_DISABLE     0x00

#define PWM_DEVNAME     "pwm"

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static struct himedia_device g_pwm_device;
#endif

static int pwm_drv_disable(unsigned char pwm_num)
{
    if (pwm_num >= PWM_NUM_MAX) {
        osal_printk("The pwm number is big than the max value!\n");
        return -1;
    }
    switch (pwm_num) {
        case 0: /* case 0 */
            pwm_write_reg(PWM0_CTRL_REG, PWM_DISABLE);
            break;
        case 1: /* case 1 */
            pwm_write_reg(PWM1_CTRL_REG, PWM_DISABLE);
            break;
        case 2: /* case 2 */
            pwm_write_reg(PWM2_CTRL_REG, PWM_DISABLE);
            break;
        case 3: /* case 3 */
            pwm_write_reg(PWM3_CTRL_REG, PWM_DISABLE);
            break;
        case 4: /* case 4 */
            pwm_write_reg(PWM4_CTRL_REG, PWM_DISABLE);
            break;
        case 5: /* case 5 */
            pwm_write_reg(PWM5_CTRL_REG, PWM_DISABLE);
            break;
        case 6: /* case 6 */
            pwm_write_reg(PWM6_CTRL_REG, PWM_DISABLE);
            break;
        case 7: /* case 7 */
            pwm_write_reg(PWM7_CTRL_REG, PWM_DISABLE);
            break;
        default:
            break;
    }

    return 0;
}

int pwm_drv_write(unsigned char pwm_num, unsigned int duty, unsigned int period, unsigned char enable)
{
    if (pwm_num >= PWM_NUM_MAX) {
        osal_printk("The pwm number is big than the max value!\n");
        return -1;
    }

    if (enable) {
        switch (pwm_num) {
            case 0:  /* case 0 */
                pwm_write_reg(PWM0_CTRL_REG, PWM_DISABLE);

                pwm_write_reg(PWM0_CFG_REG0, period);
                pwm_write_reg(PWM0_CFG_REG1, duty);
                pwm_write_reg(PWM0_CFG_REG2, 10); /* pwm output number */

                pwm_write_reg(PWM0_CTRL_REG, ((1 << 2) | PWM_ENABLE)); /* keep the pwm always output; */
                break;

            case 1:  /* case 1 */
                pwm_write_reg(PWM1_CTRL_REG, PWM_DISABLE);

                pwm_write_reg(PWM1_CFG_REG0, period);
                pwm_write_reg(PWM1_CFG_REG1, duty);
                pwm_write_reg(PWM1_CFG_REG2, 10); /* pwm output number */

                pwm_write_reg(PWM1_CTRL_REG, ((1 << 2) | PWM_ENABLE)); /* keep the pwm always output; */
                break;

            case 2: /* case 2 */
                pwm_write_reg(PWM2_CTRL_REG, PWM_DISABLE);

                pwm_write_reg(PWM2_CFG_REG0, period);
                pwm_write_reg(PWM2_CFG_REG1, duty);
                pwm_write_reg(PWM2_CFG_REG2, 10); /* pwm output number */

                pwm_write_reg(PWM2_CTRL_REG, ((1 << 2) | PWM_ENABLE)); /* keep the pwm always output; */
                break;

            case 3: /* case 3 */
                pwm_write_reg(PWM3_CTRL_REG, PWM_DISABLE);

                pwm_write_reg(PWM3_CFG_REG0, period);
                pwm_write_reg(PWM3_CFG_REG1, duty);
                pwm_write_reg(PWM3_CFG_REG2, 10); /* pwm output number */

                pwm_write_reg(PWM3_CTRL_REG, ((1 << 2) | PWM_ENABLE)); /* keep the pwm always output; */
                break;

            case 4: /* case 4 */
                pwm_write_reg(PWM4_CTRL_REG, PWM_DISABLE);
                pwm_write_reg(PWM4_CFG_REG0, period);
                pwm_write_reg(PWM4_CFG_REG1, duty);
                pwm_write_reg(PWM4_CFG_REG2, 10); /* pwm output number */
                pwm_write_reg(PWM4_CTRL_REG, ((1 << 2) | PWM_ENABLE)); /* keep the pwm always output; */
                break;

            case 5: /* case 5 */
                pwm_write_reg(PWM5_CTRL_REG, PWM_DISABLE);
                pwm_write_reg(PWM5_CFG_REG0, period);
                pwm_write_reg(PWM5_CFG_REG1, duty);
                pwm_write_reg(PWM5_CFG_REG2, 10); /* pwm output number */
                pwm_write_reg(PWM5_CTRL_REG, ((1 << 2) | PWM_ENABLE)); /* keep the pwm always output; */
                break;

            case 6: /* case 6 */
                pwm_write_reg(PWM6_CTRL_REG, PWM_DISABLE);
                pwm_write_reg(PWM6_CFG_REG0, period);
                pwm_write_reg(PWM6_CFG_REG1, duty);
                pwm_write_reg(PWM6_CFG_REG2, 10); /* pwm output number */
                pwm_write_reg(PWM6_CTRL_REG, ((1 << 2) | PWM_ENABLE)); /* keep the pwm always output; */
                break;

            case 7: /* case 7 */
                pwm_write_reg(PWM7_CTRL_REG, PWM_DISABLE);
                pwm_write_reg(PWM7_CFG_REG0, period);
                pwm_write_reg(PWM7_CFG_REG1, duty);
                pwm_write_reg(PWM7_CFG_REG2, 10); /* pwm output number */
                pwm_write_reg(PWM7_CTRL_REG, ((1 << 2) | PWM_ENABLE)); /* keep the pwm always output; */
                break;

            default:
                pwm_write_reg(PWM0_CTRL_REG, PWM_DISABLE);
                pwm_write_reg(PWM0_CFG_REG0, period);
                pwm_write_reg(PWM0_CFG_REG1, duty);
                pwm_write_reg(PWM0_CFG_REG2, 10); /* pwm output number */
                pwm_write_reg(PWM0_CTRL_REG, ((1 << 2) | PWM_ENABLE)); /* keep the pwm always output; */
                break;
        }
    } else {
        pwm_drv_disable(pwm_num);
    }

    return 0;
}

/* file operation */
#ifdef __HuaweiLite__
int pwm_open(struct file *file)
{
    return 0;
}

int  pwm_close(struct file *file)
{
    return 0;
}
#else
int pwm_open(struct inode *inode, struct file *file)
{
    return 0;
}

int  pwm_close(struct inode *inode, struct file *file)
{
    return 0;
}
#endif

#ifdef __HuaweiLite__
static int pwm_ioctl(struct file *file, int cmd, unsigned long arg)
#else
static long pwm_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
#endif
{
    int ret;
    unsigned char  pwm_num;
    unsigned int duty;
    unsigned int period;
    unsigned char  enable;
    pwm_data pwm_data_info;
    switch (cmd) {
        case PWM_CMD_WRITE:
            if (arg == 0L) {
                return -1;
            }

            if (read_user_linear_space_valid((unsigned char *)(hi_uintptr_t)arg, sizeof(pwm_data)) == 0) {
                osal_printk("pwm address is invalid!\n");
                return -EFAULT;
            }

            ret = copy_from_user(&pwm_data_info, (pwm_data __user *)(hi_uintptr_t)arg, sizeof(pwm_data));
            if (ret) {
                return -EFAULT;
            }

            pwm_num = pwm_data_info.pwm_num;
            duty    = pwm_data_info.duty;
            period  = pwm_data_info.period;
            enable  = pwm_data_info.enable;

            pwm_drv_write(pwm_num, duty, period, enable);
            break;

        case PWM_CMD_READ:
            break;

        default:
            osal_printk("invalid ioctl command!\n");
            return -ENOIOCTLCMD;
    }

    return 0;
}

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static int pwm_freeze(struct himedia_device *pdev)
{
    osal_printk(KERN_DEBUG "%s  %d\n", __FUNCTION__, __LINE__);
    return 0;
}

static int pwm_restore(struct himedia_device *pdev)
{
    osal_printk(KERN_DEBUG "%s  %d\n", __FUNCTION__, __LINE__);
    return 0;
}
#endif
#ifdef __HuaweiLite__
const static struct file_operations_vfs g_pwm_fops = {
    .open = pwm_open,
    .close = pwm_close,
    .ioctl = pwm_ioctl,
};
#else
static struct file_operations g_pwm_fops = {
    .owner      = THIS_MODULE,
    .unlocked_ioctl = pwm_ioctl,
    .open       = pwm_open,
    .release    = pwm_close,
};
#endif

#ifndef __HuaweiLite__
#ifdef CONFIG_HISI_SNAPSHOT_BOOT
struct himedia_ops g_pwm_drv_ops = {
    .pm_freeze = pwm_freeze,
    .pm_restore  = pwm_restore
};
#else
static struct miscdevice g_pwm_dev = {
    .minor   = MISC_DYNAMIC_MINOR,
    .name    = "pwm",
    .fops    = &g_pwm_fops,
};
#endif
#endif

/* module init and exit */
#ifdef __HuaweiLite__
int pwm_init(void)
{
    int     ret;
    g_reg_pwm_ibase = (void __iomem *)IO_ADDRESS(PWMI_ADRESS_BASE);

    ret = register_driver("/dev/pwm", &g_pwm_fops, 0666, 0); /* register address 0666 */
    if (ret) {
        osal_printk("register pwd device failed with %#x!\n", ret);
        return -1;
    }

    return 0;
}

void pwm_exit(void)
{
    int i;

    for (i = 0; i < PWM_NUM_MAX; i++) {
        pwm_drv_disable(i);
    }

    g_reg_pwm_ibase = HI_NULL;
    unregister_driver("/dev/pwm");
}
#else

#if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)
static int __init pwm_init(void)
#else
static int pwm_init(void)
#endif
{
    int     ret;

    if (!g_reg_pwm_ibase) {
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)
        g_reg_pwm_ibase = (void __iomem *)osal_ioremap(PWMI_ADRESS_BASE, 0x100);
#else
        g_reg_pwm_ibase = HI_NULL;
#endif
    }

#ifdef CONFIG_HISI_SNAPSHOT_BOOT

    osal_snprintf(g_pwm_device.devfs_name, sizeof(g_pwm_device.devfs_name), PWM_DEVNAME);
    g_pwm_device.minor  = HIMEDIA_DYNAMIC_MINOR;
    g_pwm_device.fops   = &g_pwm_fops;
    g_pwm_device.drvops = &g_pwm_drv_ops;
    g_pwm_device.owner  = THIS_MODULE;
    ret = himedia_register(&g_pwm_device);
    if (ret) {
        osal_printk("register i2c device failed with %#x!\n", ret);
        return -1;
    }

#else
    ret = misc_register(&g_pwm_dev);
    if (ret != 0) {
        osal_printk("register i2c device failed with %#x!\n", ret);
        return -1;
    }
#endif

    osal_printk("load pwm.ko for %s...OK!\n", CHIP_NAME);

    return 0;
}
static void __exit pwm_exit(void)
{
    int i;
    for (i = 0; i < PWM_NUM_MAX; i++) {
        pwm_drv_disable(i);
    }

#if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)
    if (g_reg_pwm_ibase) {
        osal_iounmap((hi_void *)g_reg_pwm_ibase);
        g_reg_pwm_ibase = HI_NULL;
    }
#else
    g_reg_pwm_ibase = HI_NULL;
#endif

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
    himedia_unregister(&g_pwm_device);
#else
    misc_deregister(&g_pwm_dev);
#endif
    osal_printk("unload pwm.ko for %s...OK!\n", CHIP_NAME);
}
#endif

#ifndef __HuaweiLite__
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)
module_init(pwm_init);
module_exit(pwm_exit);

MODULE_DESCRIPTION("PWM Driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hisilicon");
#else
#include <linux/of_platform.h>
static int pwm_probe(struct platform_device *pdev)
{
    struct resource *mem;

    mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    g_reg_pwm_ibase = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(g_reg_pwm_ibase)) {
        return ptr_err(g_reg_pwm_ibase);
    }

    osal_printk("++++++++++ g_reg_pwm_ibase = %p\n", g_reg_pwm_ibase);

    pwm_init();
    return 0;
}
static int pwm_remove(struct platform_device *pdev)
{
    osal_printk("<%s> is called\n", __FUNCTION__);
    pwm_exit();
    return 0;
}
static const struct of_device_id g_pwm_match[] = {
    { .compatible = "hisilicon,pwm" },
    {},
};
MODULE_DEVICE_TABLE(of, g_pwm_match);
static struct platform_driver g_pwm_driver = {
    .probe          = pwm_probe,
    .remove         = pwm_remove,
    .driver         = {
        .name   = "pwm",
        .of_match_table = g_pwm_match,
    }
};
module_platform_driver(g_pwm_driver);
MODULE_LICENSE("GPL");
#endif
#endif
