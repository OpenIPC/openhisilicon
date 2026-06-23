/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "pwm.h"
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include "ot_osal.h"
#include "mm_ext.h"
#include "ot_debug.h"

#ifdef __LITEOS__
#ifndef IOMEM
#define IOMEM
#endif
#ifndef ENOIOCTLCMD
#define ENOIOCTLCMD     515 /* No ioctl command */
#endif
#else   /* linux */
#ifndef IOMEM
#define IOMEM __iomem
#endif
#endif

#include "pwm_hal.h"

td_void IOMEM *g_reg_pwm_ibase = 0;

#define  pwm_write_reg(addr, value) ((*(volatile td_u32 *)(addr)) = (value))
#define  pwm_read_reg(addr)         (*(volatile td_u32 *)(addr))
#define pwm_err_trace(fmt, ...) \
    OT_ERR_TRACE(OT_ID_ISP, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)


/* PWM */
#define PWM_ENABLE             0x01
#define PWM_DISABLE            0x00
#define PWM_DEVNAME            "pwm"
#define PWM_MAP_SIZE           0x4FF

static td_s32 pwm_drv_disable(td_u8 pwm_num)
{
    if (pwm_num >= PWM_NUM_MAX) {
        pwm_err_trace("The pwm number is big than the max value!\n");
        return -1;
    }
    if (pwm_num == 0) { /* pwm_idx == 0 the addr is PWM2_CHN0_CTRL_REG */
        pwm_write_reg(PWM2_CHN0_CTRL_REG, PWM_DISABLE);
    } else {
        pwm_write_reg(pwm_ctrl_reg(pwm_num), PWM_DISABLE);
    }
    return 0;
}

td_void pwm_drv_write_reg(td_s32 idx, td_u32 duty, td_u32 period)
{
    pwm_write_reg(pwm_ctrl_reg(idx), PWM_DISABLE);
    pwm_write_reg(pwm_cfg_reg0(idx), period);
    pwm_write_reg(pwm_cfg_reg1(idx), duty);
    pwm_write_reg(pwm_cfg_reg2(idx), 10);
    pwm_write_reg(pwm_ctrl_reg(idx), ((1 << 2) | PWM_ENABLE));
    return;
}
td_s32 pwm_drv_write(td_u8 pwm_num, td_u32 duty, td_u32 period, td_u8 enable)
{
    if (pwm_num >= PWM_NUM_MAX) {
        pwm_err_trace("The pwm number is big than the max value!\n");
        return -1;
    }
    if (enable) {
        switch (pwm_num) {
            case 0: /* case 0 */
                pwm_write_reg(PWM2_CHN0_CTRL_REG, PWM2_CHN0_WORK_MODE | PWM_DISABLE); /* work mode cfg */
                pwm_write_reg(PWM2_CHN0_CFG_REG0, period);
                pwm_write_reg(PWM2_CHN0_CFG_REG1, duty);       /* duty0 */
                pwm_write_reg(PWM2_CHN0_CFG_REG1 + 0x4, duty); /* duty1 */
                pwm_write_reg(PWM2_CHN0_CFG_REG1 + 0x8, duty); /* duty2 */
                pwm_write_reg(PWM2_CHN0_CTRL_REG, PWM2_CHN0_WORK_MODE | PWM_ENABLE);  /* work mode cfg */
                break;
            case 1: /* case 1 */
            case 2: /* case 2 */
            case 3: /* case 3 */
            case 4: /* case 4 */
                pwm_drv_write_reg(pwm_num, duty, period);
                break;
            default:
                pwm_drv_write_reg(0, duty, period);
                break;
        }
    } else {
        pwm_drv_disable(pwm_num);
    }

    return 0;
}

/* file operation */
#ifdef __LITEOS__
td_s32 pwm_open(struct file *file)
{
    return 0;
}

td_s32 pwm_close(struct file *file)
{
    return 0;
}
#else
td_s32 pwm_open(struct inode *inode, struct file *file)
{
    ot_unused(inode);
    ot_unused(file);
    return 0;
}

td_s32 pwm_close(struct inode *inode, struct file *file)
{
    ot_unused(inode);
    ot_unused(file);
    return 0;
}
#endif

#ifdef __LITEOS__
static td_s32 pwm_ioctl(struct file *file, td_s32 cmd, td_ulong arg)
#else
static td_slong pwm_ioctl(struct file *file, td_u32 cmd, td_ulong arg)
#endif
{
    td_s32 ret;
    td_u8  pwm_num;
    td_u32 duty;
    td_u32 period;
    td_u8  enable;
    pwm_data pwm_data_info;
    ot_unused(file);

    switch (cmd) {
        case PWM_CMD_WRITE:
            if (arg == TD_NULL) {
                return -1;
            }

            if (read_user_linear_space_valid((td_u8 *)(td_uintptr_t)arg, sizeof(pwm_data)) == 0) {
                pwm_err_trace("pwm address is invalid!\n");
                return -EFAULT;
            }

            ret = osal_copy_from_user(&pwm_data_info, (pwm_data __user *)(td_uintptr_t)arg, sizeof(pwm_data));
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
            pwm_err_trace("invalid ioctl command!\n");
            return -ENOIOCTLCMD;
    }

    return 0;
}

#ifdef __LITEOS__
static const struct file_operations_vfs g_pwm_fops = {
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

#ifndef __LITEOS__
static struct miscdevice g_pwm_dev = {
    .minor   = MISC_DYNAMIC_MINOR,
    .name    = "pwm",
    .fops    = &g_pwm_fops,
};
#endif

/* module init and exit */
#ifdef __LITEOS__
td_s32 pwm_mod_init(td_void)
{
    td_s32     ret;
    g_reg_pwm_ibase = (td_void IOMEM *)IO_ADDRESS(PWMI_ADRESS_BASE);

    ret = register_driver("/dev/pwm", &g_pwm_fops, 0666, 0); /* register address 0666 */
    if (ret) {
        pwm_err_trace("register pwd device failed with %#x!\n", ret);
        goto OUT1;
    }

    return 0;

OUT1:
    g_reg_pwm_ibase = TD_NULL;

    return -1;
}

td_void pwm_mod_exit(td_void)
{
    td_s32 i;

    for (i = 0; i < PWM_NUM_MAX; i++) {
        pwm_drv_disable(i);
    }

    g_reg_pwm_ibase = TD_NULL;
    unregister_driver("/dev/pwm");
}
#else
td_s32 pwm_mod_init(td_void)
{
    td_s32 ret;

    if (!g_reg_pwm_ibase) {
        g_reg_pwm_ibase = (td_void IOMEM *)osal_ioremap(PWMI_ADRESS_BASE, PWM_MAP_SIZE);
    }

    ret = misc_register(&g_pwm_dev);
    if (ret != 0) {
        osal_printk("register i2c device failed with %#x!\n", ret);
        goto OUT1;
    }

    osal_printk("load pwm.ko ....OK!\n");

    return 0;

OUT1:
    if (g_reg_pwm_ibase) {
        osal_iounmap((td_void *)g_reg_pwm_ibase, PWM_MAP_SIZE);
    }
    g_reg_pwm_ibase = TD_NULL;

    return -1;
}
td_void pwm_mod_exit(td_void)
{
    td_s32 i;
    for (i = 0; i < PWM_NUM_MAX; i++) {
        pwm_drv_disable(i);
    }

    if (g_reg_pwm_ibase) {
        osal_iounmap((td_void *)g_reg_pwm_ibase, PWM_MAP_SIZE);
        g_reg_pwm_ibase = TD_NULL;
    }

    misc_deregister(&g_pwm_dev);

    osal_printk("unload pwm.ko ....OK!\n");
}
#endif
