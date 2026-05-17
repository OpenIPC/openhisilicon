/*  pwm.c
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
 *      23-march-2017 move this file to interdrv
 */

#include "hi_osal.h"
#include "pwm.h"

#ifndef NULL
#define NULL  ((void *)0)
#endif

#define PWM_NAME "pwm"

#define PWMI_ADRESS_BASE         0x12130000
#define CRG_PWM_ADRESS_BASE      0x12010038
#define PWM_PIN_ADRESS           0x1204000C
#define PWM_NUM_MAX              0x4

void* reg_pwmI_base_va = 0;
void* pwm_crg_base_va = 0;
void* pwm_pin_va = 0;

#define HI_IO_PWMI_ADDRESS(x)  ((unsigned long)reg_pwmI_base_va + ((x)-(PWMI_ADRESS_BASE)))

//PWMI
#define PWM0_CFG_REG0      HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0000)
#define PWM0_CFG_REG1      HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0004)
#define PWM0_CFG_REG2      HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0008)
#define PWM0_CTRL_REG       HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x000C)
#define PWM0_STATE_REG0       HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0010)
#define PWM0_STATE_REG1       HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0014)
#define PWM0_STATE_REG2       HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0018)

#define PWM1_CFG_REG0      HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0020)
#define PWM1_CFG_REG1      HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0024)
#define PWM1_CFG_REG2      HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0028)
#define PWM1_CTRL_REG       HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x002C)
#define PWM1_STATE_REG0       HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0030)
#define PWM1_STATE_REG1       HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0034)
#define PWM1_STATE_REG2       HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0038)

#define PWM2_CFG_REG0      HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0040)
#define PWM2_CFG_REG1      HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0044)
#define PWM2_CFG_REG2      HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0048)
#define PWM2_CTRL_REG       HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x004C)
#define PWM2_STATE_REG0       HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0050)
#define PWM2_STATE_REG1       HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0054)
#define PWM2_STATE_REG2       HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0058)

#define PWM3_CFG_REG0      HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0060)
#define PWM3_CFG_REG1      HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0064)
#define PWM3_CFG_REG2      HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0068)
#define PWM3_CTRL_REG       HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x006C)
#define PWM3_STATE_REG0       HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0070)
#define PWM3_STATE_REG1       HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0074)
#define PWM3_STATE_REG2       HI_IO_PWMI_ADDRESS(PWMI_ADRESS_BASE + 0x0078)

#define  PWM_WRITE_REG(Addr, Value) ((*(volatile unsigned int *)(Addr)) = (Value))
#define  PWM_READ_REG(Addr)         (*(volatile unsigned int *)(Addr))

//PWM
#define PWM_ENABLE          0x01
#define PWM_DISABLE         0x00

static int PWM_DRV_Disable(unsigned char pwm_num)
{
    if (pwm_num >= PWM_NUM_MAX)
    {
        osal_printk("The pwm number is big than the max value!\n");
        return -1;
    }
    switch (pwm_num)
    {
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
        default:
            break;
    }

    return 0;
}


int PWM_DRV_Write(unsigned char pwm_num, unsigned int duty, unsigned int period, unsigned char enable)
{
    if (pwm_num >= PWM_NUM_MAX)
    {
        osal_printk("The pwm number is big than the max value!\n");
        return -1;
    }
    if (enable)
    {

        switch (pwm_num)
        {
            case 0:
                PWM_WRITE_REG(PWM0_CTRL_REG, PWM_DISABLE);

                PWM_WRITE_REG(PWM0_CFG_REG0, period);
                PWM_WRITE_REG(PWM0_CFG_REG1, duty);
                //PWM_WRITE_REG(PWM0_CFG_REG2, 10); //pwm output number

                PWM_WRITE_REG(PWM0_CTRL_REG, (1 << 2 | PWM_ENABLE)); // keep the pwm always output;
                //osal_printk("The PWMI0 state %x\n",PWM_READ_REG(PWM0_STATE_REG));
                break;

            case 1:
                PWM_WRITE_REG(PWM1_CTRL_REG, PWM_DISABLE);

                PWM_WRITE_REG(PWM1_CFG_REG0, period);
                PWM_WRITE_REG(PWM1_CFG_REG1, duty);
                //PWM_WRITE_REG(PWM1_CFG_REG2, 10); //pwm output number

                PWM_WRITE_REG(PWM1_CTRL_REG, (1 << 2 | PWM_ENABLE)); // keep the pwm always output;
                //osal_printk("The PWMI1 state %x\n",PWM_READ_REG(PWM1_STATE_REG));
                break;

            case 2:
                PWM_WRITE_REG(PWM2_CTRL_REG, PWM_DISABLE);

                PWM_WRITE_REG(PWM2_CFG_REG0, period);
                PWM_WRITE_REG(PWM2_CFG_REG1, duty);
                //PWM_WRITE_REG(PWM2_CFG_REG2, 10); //pwm output number

                PWM_WRITE_REG(PWM2_CTRL_REG, (1 << 2 | PWM_ENABLE)); // keep the pwm always output;
                //osal_printk("The PWMI2 state %x\n",PWM_READ_REG(PWM2_STATE_REG));
                break;

            case 3:
                PWM_WRITE_REG(PWM3_CTRL_REG, PWM_DISABLE);

                PWM_WRITE_REG(PWM3_CFG_REG0, period);
                PWM_WRITE_REG(PWM3_CFG_REG1, duty);
                //PWM_WRITE_REG(PWM3_CFG_REG2, 10); //pwm output number

                PWM_WRITE_REG(PWM3_CTRL_REG, (1 << 2 | PWM_ENABLE)); // keep the pwm always output;
                //osal_printk("The PWMI3 state %x\n",PWM_READ_REG(PWM3_STATE_REG));
                break;

            default:
                PWM_WRITE_REG(PWM0_CTRL_REG, PWM_DISABLE);

                PWM_WRITE_REG(PWM0_CFG_REG0, period);
                PWM_WRITE_REG(PWM0_CFG_REG1, duty);
                //PWM_WRITE_REG(PWM0_CFG_REG2, 10); //pwm output number

                PWM_WRITE_REG(PWM0_CTRL_REG, (1 << 2 | PWM_ENABLE)); // keep the pwm always output;
                //osal_printk("The PWMII0 state %x\n",PWM_READ_REG(PWM0_STATE_REG));
                break;
        }
    }
    else
    {
        PWM_DRV_Disable(pwm_num);
    }


    return 0;
}

static long pwm_ioctl(unsigned int cmd, unsigned long arg, void *private_data)
{
    PWM_DATA_S * argp = (PWM_DATA_S *)arg;

    unsigned char  PwmNum;
    unsigned int Duty;
    unsigned int Period;
    unsigned char  enable;

    switch (cmd)
    {
        case PWM_CMD_WRITE:
        {
            PWM_DATA_S stPwmData;

            osal_memcpy(&stPwmData, argp, sizeof(PWM_DATA_S));

            PwmNum  = stPwmData.pwm_num;
            Duty    = stPwmData.duty;
            Period  = stPwmData.period;
            enable  = stPwmData.enable;

            PWM_DRV_Write(PwmNum, Duty, Period, enable);
            break;
        }

        case PWM_CMD_READ:
        {
            break;
        }

        default:
        {
            osal_printk("invalid ioctl command!\n");
            return -1;
        }
    }

    return 0 ;
}

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static int PWM_freeze(osal_dev_t *pdev)
{
    osal_printk("%s  %d\n", __FUNCTION__, __LINE__);
    return 0;
}

static int PWM_restore(osal_dev_t *pdev)
{
    osal_printk("%s  %d\n", __FUNCTION__, __LINE__);
	/* enable pwm clk and reset*/
	PWM_WRITE_REG(pwm_crg_base_va, 0x3);
	PWM_WRITE_REG(pwm_crg_base_va, 0x2);
    return 0;
}
#else
static int PWM_freeze(osal_dev_t *pdev)
{
    return 0;
}

static int PWM_restore(osal_dev_t *pdev)
{
    return 0;
}
#endif

static osal_pmops_t stPwmDrvops =
{
	.pm_freeze = PWM_freeze,
	.pm_restore  = PWM_restore,
};

static int pwm_open(void *private_data)
{
	return 0;
}

static int pwm_release(void *private_data)
{
	return 0;
}

static struct osal_fileops pwm_fops =
{
    .unlocked_ioctl = pwm_ioctl,
    .open = pwm_open,
    .release = pwm_release,
};

static osal_dev_t *pwm_dev = 0;

int  pwm_init(void)
{
	int ret = 0;

	pwm_crg_base_va = (void *)osal_ioremap_nocache((unsigned long)CRG_PWM_ADRESS_BASE, (unsigned long)0x4);
	if (!pwm_crg_base_va)
    {
        osal_printk("Kernel: ioremap pwm crg failed!\n");
        return -1;
    }

	/* enable pwm clk and reset*/
	PWM_WRITE_REG(pwm_crg_base_va, 0x7);
	osal_msleep(20);
	PWM_WRITE_REG(pwm_crg_base_va, 0x6);

    pwm_pin_va = (void *)osal_ioremap_nocache((unsigned long)PWM_PIN_ADRESS, (unsigned long)0x4);
    PWM_WRITE_REG(pwm_pin_va, 0x1);

    if (!reg_pwmI_base_va)
    {
        reg_pwmI_base_va = (void *)osal_ioremap_nocache(PWMI_ADRESS_BASE, 0x10000);
        if (!reg_pwmI_base_va)
        {
            osal_printk("osal_ioremap_nocache err. \n");
            return -1;
        }
    }

    pwm_dev = osal_createdev(PWM_NAME);
    if (NULL == pwm_dev)
	{
	    osal_printk("cretate pwm device failed!\n");
		return -1;
	}

	pwm_dev->fops = &pwm_fops;
    pwm_dev->minor = 255;
	pwm_dev->osal_pmops = &stPwmDrvops;

	ret = osal_registerdevice(pwm_dev);
	if (0 != ret)
	{
		osal_printk("pwm device register failed!\n");
		osal_destroydev(pwm_dev);
		pwm_dev = NULL;
		return -1;
    }

    osal_printk("load pwm.ko OK!\n");
	return 0;
}

void  pwm_exit(void)
{
    int i;

    for (i = 0; i < PWM_NUM_MAX; i++)
    {
        PWM_DRV_Disable(i);
    }

	/* disable pwm clk */
	PWM_WRITE_REG(pwm_crg_base_va, 0x1);

	osal_iounmap((void*)pwm_crg_base_va);
    pwm_crg_base_va = NULL;

	osal_iounmap((void*)pwm_pin_va);
    pwm_pin_va = NULL;

    osal_iounmap((void*)reg_pwmI_base_va);
    reg_pwmI_base_va = NULL;

	osal_deregisterdevice(pwm_dev);
	osal_destroydev(pwm_dev);

    osal_printk("unload pwm.ko OK!\n");
}

MODULE_LICENSE("GPL");
