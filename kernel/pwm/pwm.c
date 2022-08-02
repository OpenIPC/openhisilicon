/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <linux/module.h>
#include <linux/errno.h>
#ifndef CONFIG_SNAPSHOT_BOOT
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

#include "osal.h"
#include "mm_ext.h"
#include "pwm.h"


#ifndef __LITEOS__
#ifdef CONFIG_SNAPSHOT_BOOT
#include "media.h"
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


void __iomem *reg_pwmI_base_va = 0;


#define IO_PWMI_ADDRESS(x)  ((void __iomem *)((unsigned char __iomem *)reg_pwmI_base_va + ((x)-(PWMI_ADRESS_BASE))))


// PWMI
#define PWM0_CFG_REG0   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0000)
#define PWM0_CFG_REG1   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0004)
#define PWM0_CFG_REG2   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0008)
#define PWM0_CTRL_REG   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x000C)
#define PWM0_STATE_REG0 IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0010)
#define PWM0_STATE_REG1 IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0014)
#define PWM0_STATE_REG2 IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0018)

#define PWM1_CFG_REG0   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0020)
#define PWM1_CFG_REG1   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0024)
#define PWM1_CFG_REG2   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0028)
#define PWM1_CTRL_REG   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x002C)
#define PWM1_STATE_REG0 IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0030)
#define PWM1_STATE_REG1 IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0034)
#define PWM1_STATE_REG2 IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0038)

#define PWM2_CFG_REG0   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0040)
#define PWM2_CFG_REG1   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0044)
#define PWM2_CFG_REG2   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0048)
#define PWM2_CTRL_REG   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x004C)
#define PWM2_STATE_REG0 IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0050)
#define PWM2_STATE_REG1 IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0054)
#define PWM2_STATE_REG2 IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0058)

#define PWM3_CFG_REG0   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0060)
#define PWM3_CFG_REG1   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0064)
#define PWM3_CFG_REG2   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0068)
#define PWM3_CTRL_REG   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x006C)
#define PWM3_STATE_REG0 IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0070)
#define PWM3_STATE_REG1 IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0074)
#define PWM3_STATE_REG2 IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0078)

#define PWM4_CFG_REG0   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0080)
#define PWM4_CFG_REG1   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0084)
#define PWM4_CFG_REG2   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0088)
#define PWM4_CTRL_REG   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x008C)
#define PWM4_STATE_REG0 IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0090)
#define PWM4_STATE_REG1 IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0094)
#define PWM4_STATE_REG2 IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0098)

#define PWM5_CFG_REG0   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x00A0)
#define PWM5_CFG_REG1   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x00A4)
#define PWM5_CFG_REG2   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x00A8)
#define PWM5_CTRL_REG   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x00AC)
#define PWM5_STATE_REG0 IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x00B0)
#define PWM5_STATE_REG1 IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x00B4)
#define PWM5_STATE_REG2 IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x00B8)

#define PWM6_CFG_REG0   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x00C0)
#define PWM6_CFG_REG1   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x00C4)
#define PWM6_CFG_REG2   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x00C8)
#define PWM6_CTRL_REG   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x00CC)
#define PWM6_STATE_REG0 IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x00D0)
#define PWM6_STATE_REG1 IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x00D4)
#define PWM6_STATE_REG2 IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x00D8)

#define PWM7_CFG_REG0   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x00E0)
#define PWM7_CFG_REG1   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x00E4)
#define PWM7_CFG_REG2   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x00E8)
#define PWM7_CTRL_REG   IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x00EC)
#define PWM7_STATE_REG0 IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x00F0)
#define PWM7_STATE_REG1 IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x00F4)
#define PWM7_STATE_REG2 IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x00F8)

#define  PWM_WRITE_REG(Addr, Value) ((*(volatile unsigned int *)(Addr)) = (Value))
#define  PWM_READ_REG(Addr)         (*(volatile unsigned int *)(Addr))

// PWM
#define PWM_NUM_MAX     0x08
#define PWM_ENABLE      0x01
#define PWM_DISABLE     0x00

#define PWM_DEVNAME     "pwm"

#ifdef CONFIG_SNAPSHOT_BOOT
static struct media_device s_stPwmDevice;
#endif

static int PWM_DRV_Disable(unsigned char pwm_num)
{
    if (pwm_num >= PWM_NUM_MAX) {
        osal_printk("The pwm number is big than the max value!\n");
        return -1;
    }
    switch (pwm_num) {
        case 0:
            PWM_WRITE_REG(PWM0_CTRL_REG, PWM_DISABLE);
            break;
        case 1:
            PWM_WRITE_REG(PWM1_CTRL_REG, PWM_DISABLE);
            break;
        case 2:
            PWM_WRITE_REG(PWM2_CTRL_REG, PWM_DISABLE);
            break;
        case 3:
            PWM_WRITE_REG(PWM3_CTRL_REG, PWM_DISABLE);
            break;
        case 4:
            PWM_WRITE_REG(PWM4_CTRL_REG, PWM_DISABLE);
            break;
        case 5:
            PWM_WRITE_REG(PWM5_CTRL_REG, PWM_DISABLE);
            break;
        case 6:
            PWM_WRITE_REG(PWM6_CTRL_REG, PWM_DISABLE);
            break;
        case 7:
            PWM_WRITE_REG(PWM7_CTRL_REG, PWM_DISABLE);
            break;
        default:
            break;
    }

    return 0;
}


int PWM_DRV_Write(unsigned char pwm_num, unsigned int duty, unsigned int period, unsigned char enable)
{
    if (pwm_num >= PWM_NUM_MAX) {
        osal_printk("The pwm number is big than the max value!\n");
        return -1;
    }

    if (enable) {
        switch (pwm_num) {
            case 0:
                PWM_WRITE_REG(PWM0_CTRL_REG, PWM_DISABLE);

                PWM_WRITE_REG(PWM0_CFG_REG0, period);
                PWM_WRITE_REG(PWM0_CFG_REG1, duty);
                PWM_WRITE_REG(PWM0_CFG_REG2, 10); // pwm output number

                PWM_WRITE_REG(PWM0_CTRL_REG, (1 << 2 | PWM_ENABLE)); // keep the pwm always output;
                break;

            case 1:
                PWM_WRITE_REG(PWM1_CTRL_REG, PWM_DISABLE);

                PWM_WRITE_REG(PWM1_CFG_REG0, period);
                PWM_WRITE_REG(PWM1_CFG_REG1, duty);
                PWM_WRITE_REG(PWM1_CFG_REG2, 10); // pwm output number

                PWM_WRITE_REG(PWM1_CTRL_REG, (1 << 2 | PWM_ENABLE)); // keep the pwm always output;
                break;

            case 2:
                PWM_WRITE_REG(PWM2_CTRL_REG, PWM_DISABLE);

                PWM_WRITE_REG(PWM2_CFG_REG0, period);
                PWM_WRITE_REG(PWM2_CFG_REG1, duty);
                PWM_WRITE_REG(PWM2_CFG_REG2, 10); // pwm output number

                PWM_WRITE_REG(PWM2_CTRL_REG, (1 << 2 | PWM_ENABLE)); // keep the pwm always output;
                break;

            case 3:
                PWM_WRITE_REG(PWM3_CTRL_REG, PWM_DISABLE);

                PWM_WRITE_REG(PWM3_CFG_REG0, period);
                PWM_WRITE_REG(PWM3_CFG_REG1, duty);
                PWM_WRITE_REG(PWM3_CFG_REG2, 10); // pwm output number

                PWM_WRITE_REG(PWM3_CTRL_REG, (1 << 2 | PWM_ENABLE)); // keep the pwm always output;
                break;

            case 4:
                PWM_WRITE_REG(PWM4_CTRL_REG, PWM_DISABLE);
                PWM_WRITE_REG(PWM4_CFG_REG0, period);
                PWM_WRITE_REG(PWM4_CFG_REG1, duty);
                PWM_WRITE_REG(PWM4_CFG_REG2, 10); // pwm output number
                PWM_WRITE_REG(PWM4_CTRL_REG, (1 << 2 | PWM_ENABLE)); // keep the pwm always output;
                break;
            case 5:
                PWM_WRITE_REG(PWM5_CTRL_REG, PWM_DISABLE);
                PWM_WRITE_REG(PWM5_CFG_REG0, period);
                PWM_WRITE_REG(PWM5_CFG_REG1, duty);
                PWM_WRITE_REG(PWM5_CFG_REG2, 10); // pwm output number
                PWM_WRITE_REG(PWM5_CTRL_REG, (1 << 2 | PWM_ENABLE)); // keep the pwm always output;
                break;
            case 6:
                PWM_WRITE_REG(PWM6_CTRL_REG, PWM_DISABLE);
                PWM_WRITE_REG(PWM6_CFG_REG0, period);
                PWM_WRITE_REG(PWM6_CFG_REG1, duty);
                PWM_WRITE_REG(PWM6_CFG_REG2, 10); // pwm output number
                PWM_WRITE_REG(PWM6_CTRL_REG, (1 << 2 | PWM_ENABLE)); // keep the pwm always output;
                break;
            case 7:
                PWM_WRITE_REG(PWM7_CTRL_REG, PWM_DISABLE);
                PWM_WRITE_REG(PWM7_CFG_REG0, period);
                PWM_WRITE_REG(PWM7_CFG_REG1, duty);
                PWM_WRITE_REG(PWM7_CFG_REG2, 10); // pwm output number
                PWM_WRITE_REG(PWM7_CTRL_REG, (1 << 2 | PWM_ENABLE)); // keep the pwm always output;
                break;
            default:
                PWM_WRITE_REG(PWM3_CTRL_REG, PWM_DISABLE);
                PWM_WRITE_REG(PWM3_CFG_REG0, period);
                PWM_WRITE_REG(PWM3_CFG_REG1, duty);
                PWM_WRITE_REG(PWM3_CFG_REG2, 10); // pwm output number
                PWM_WRITE_REG(PWM3_CTRL_REG, (1 << 2 | PWM_ENABLE)); // keep the pwm always output;
                break;
        }
    } else {
        PWM_DRV_Disable(pwm_num);
    }


    return 0;
}

/* file operation                                                           */
#ifdef __LITEOS__
int PWM_Open(struct file *file)
{
    return 0;
}

int  PWM_Close(struct file *file)
{
    return 0;
}
#else
int PWM_Open(struct inode *inode, struct file *file)
{
    return 0;

}

int  PWM_Close(struct inode *inode, struct file *file)
{
    return 0;
}
#endif

#ifdef __LITEOS__
static int PWM_Ioctl(struct file *file, int cmd, unsigned long arg)
#else
static long PWM_Ioctl(struct file *file, unsigned int cmd, unsigned long arg)
#endif
{
    int ret;
    unsigned char  PwmNum;
    unsigned int Duty;
    unsigned int Period;
    unsigned char  enable;

    switch (cmd) {
        case PWM_CMD_WRITE: {
            PWM_DATA_S stPwmData;

            if (arg == GK_NULL) {
                return -1;
            }
            if (read_user_linear_space_valid((unsigned char *)(GK_UINTPTR_T)arg, sizeof(PWM_DATA_S)) == 0) {
                osal_printk("pwm address is invalid!\n");
                return -EFAULT;
            }

            ret = copy_from_user(&stPwmData, (PWM_DATA_S __user *)(GK_UINTPTR_T)arg, sizeof(PWM_DATA_S));
            if (ret) {
                return -EFAULT;
            }

            PwmNum  = stPwmData.pwm_num;
            Duty    = stPwmData.duty;
            Period  = stPwmData.period;
            enable  = stPwmData.enable;

            PWM_DRV_Write(PwmNum, Duty, Period, enable);
            break;
        }

        case PWM_CMD_READ: {
            break;
        }

        default: {
            osal_printk("invalid ioctl command!\n");
            return -ENOIOCTLCMD;
        }
    }

    return 0;
}

#ifdef CONFIG_SNAPSHOT_BOOT
static int PWM_freeze(struct media_device *pdev)
{
    osal_printk(KERN_DEBUG "%s  %d\n", __FUNCTION__, __LINE__);
    return 0;
}

static int PWM_restore(struct media_device *pdev)
{
    osal_printk(KERN_DEBUG "%s  %d\n", __FUNCTION__, __LINE__);
    return 0;
}
#endif
#ifdef __LITEOS__
const static struct file_operations_vfs pwm_fops = {
    .open = PWM_Open,
    .close = PWM_Close,
    .ioctl = PWM_Ioctl,
};
#else
static struct file_operations pwm_fops = {
    .owner      = THIS_MODULE,
    .unlocked_ioctl = PWM_Ioctl,
    .open       = PWM_Open,
    .release    = PWM_Close,
};
#endif

#ifndef __LITEOS__
#ifdef CONFIG_SNAPSHOT_BOOT
struct media_ops stPwmDrvOps = {
    .pm_freeze = PWM_freeze,
    .pm_restore  = PWM_restore
};
#else
static struct miscdevice pwm_dev = {
    .minor   = MISC_DYNAMIC_MINOR,
    .name    = "pwm",
    .fops    = &pwm_fops,
};
#endif
#endif



/* module init and exit                                                     */
#ifdef __LITEOS__
int pwm_init(void)
{
    int     ret;
    reg_pwmI_base_va = (void __iomem *)IO_ADDRESS(PWMI_ADRESS_BASE);

    ret = register_driver("/dev/pwm", &pwm_fops, 0666, 0);
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
        PWM_DRV_Disable(i);
    }

    reg_pwmI_base_va = NULL;
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

    if (!reg_pwmI_base_va) {
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)
        reg_pwmI_base_va = (void __iomem *)osal_ioremap(PWMI_ADRESS_BASE, 0x100);
#else
        reg_pwmI_base_va = NULL;
#endif
    }

#ifdef CONFIG_SNAPSHOT_BOOT

    osal_snprintf(s_stPwmDevice.devfs_name, sizeof(s_stPwmDevice.devfs_name), PWM_DEVNAME);
    s_stPwmDevice.minor  = MEDIA_DYNAMIC_MINOR;
    s_stPwmDevice.fops   = &pwm_fops;
    s_stPwmDevice.drvops = &stPwmDrvOps;
    s_stPwmDevice.owner  = THIS_MODULE;
    ret = media_register(&s_stPwmDevice);
    if (ret) {
        osal_printk("register i2c device failed with %#x!\n", ret);
        return -1;
    }

#else
    ret = misc_register(&pwm_dev);
    if (ret != 0) {
        osal_printk("register i2c device failed with %#x!\n", ret);
        return -1;
    }
#endif
    return 0;
}
static void __exit pwm_exit(void)
{

#if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)
    if (reg_pwmI_base_va) {
        osal_iounmap((GK_VOID *)reg_pwmI_base_va);
        reg_pwmI_base_va = GK_NULL;
    }
#else
    reg_pwmI_base_va = NULL;
#endif

#ifdef CONFIG_SNAPSHOT_BOOT
    media_unregister(&s_stPwmDevice);
#else
    misc_deregister(&pwm_dev);
#endif
}
#endif

#ifndef __LITEOS__
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)
module_init(pwm_init);
module_exit(pwm_exit);

MODULE_LICENSE("GPL");
#else
#include <linux/of_platform.h>
static int pwm_probe(struct platform_device *pdev)
{
    struct resource *mem;

    mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    reg_pwmI_base_va = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(reg_pwmI_base_va)) {
        return PTR_ERR(reg_pwmI_base_va);
    }

    osal_printk("++++++++++ reg_pwmI_base_va = %p\n", reg_pwmI_base_va);


    pwm_init();
    return 0;
}
static int pwm_remove(struct platform_device *pdev)
{
    osal_printk("<%s> is called\n", __FUNCTION__);
    pwm_exit();
    return 0;
}
static const struct of_device_id pwm_match[] = {
    { .compatible = "goke,pwm" },
    {},
};
MODULE_DEVICE_TABLE(of, pwm_match);
static struct platform_driver pwm_driver = {
    .probe          = pwm_probe,
    .remove         = pwm_remove,
    .driver         = {
        .name           = "pwm",
        .of_match_table = pwm_match,
    }
};
module_platform_driver(pwm_driver);
MODULE_LICENSE("GPL");
#endif
#endif
