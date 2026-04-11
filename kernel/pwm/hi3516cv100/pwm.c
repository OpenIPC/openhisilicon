/*  extdrv/interface/pwm.c
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

#include <asm/uaccess.h>
#include <asm/io.h>

#include "pwm.h"


#define PWMI_ADRESS_BASE         0x20130000




void __iomem *reg_pwmI_base_va = 0;


#define HI_IO_PWMI_ADDRESS(x)  (reg_pwmI_base_va + ((x)-(PWMI_ADRESS_BASE)))



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




#define  PWM_WRITE_REG(Addr, Value) ((*(volatile unsigned int *)(Addr)) = (Value))
#define  PWM_READ_REG(Addr)         (*(volatile unsigned int *)(Addr))

//PWM 
#define PWM_NUM_MAX         0x03
#define PWM_ENABLE         0x01
#define PWM_DISABLE         0x00

static int PWM_DRV_Disable(unsigned char pwm_num)
{
	if(pwm_num >= PWM_NUM_MAX)
	{
		printk("The pwm number is big than the max value!\n");
        return -1;
	}
	switch(pwm_num)
	{
		case 0:
			PWM_WRITE_REG(PWM0_CTRL_REG,PWM_DISABLE);
			break;
		case 1:
			PWM_WRITE_REG(PWM1_CTRL_REG,PWM_DISABLE);
			break;
		case 2:
			PWM_WRITE_REG(PWM2_CTRL_REG,PWM_DISABLE);
			break;
	}
    
	return 0;
}


int PWM_DRV_Write(unsigned char pwm_num,unsigned short duty, unsigned short period,unsigned char enable)
{	
	if(pwm_num >= PWM_NUM_MAX)
	{
		printk("The pwm number is big than the max value!\n");
        return -1;
	}
	if(enable)
	{
		
		switch(pwm_num)
		{
			case 0:
				PWM_WRITE_REG(PWM0_CTRL_REG,PWM_DISABLE);
				
				PWM_WRITE_REG(PWM0_CFG_REG0,period);
				PWM_WRITE_REG(PWM0_CFG_REG1,duty);
				PWM_WRITE_REG(PWM0_CFG_REG2,10);//pwm output number
				
				PWM_WRITE_REG(PWM0_CTRL_REG,(1<<2|PWM_ENABLE));// keep the pwm always output;
				//printk("The PWMI0 state %x\n",PWM_READ_REG(PWM0_STATE_REG));
				break;
				
			case 1:
				PWM_WRITE_REG(PWM1_CTRL_REG,PWM_DISABLE);
				
				PWM_WRITE_REG(PWM1_CFG_REG0,period);
				PWM_WRITE_REG(PWM1_CFG_REG1,duty);
				PWM_WRITE_REG(PWM1_CFG_REG2,10);//pwm output number
				
				PWM_WRITE_REG(PWM1_CTRL_REG,(1<<2|PWM_ENABLE));// keep the pwm always output;
				//printk("The PWMI1 state %x\n",PWM_READ_REG(PWM1_STATE_REG));
				break;
				
			case 2:
				PWM_WRITE_REG(PWM2_CTRL_REG,PWM_DISABLE);
				
				PWM_WRITE_REG(PWM2_CFG_REG0,period);
				PWM_WRITE_REG(PWM2_CFG_REG1,duty);
				PWM_WRITE_REG(PWM2_CFG_REG2,10);//pwm output number
				
				PWM_WRITE_REG(PWM2_CTRL_REG,(1<<2|PWM_ENABLE));// keep the pwm always output;
				//printk("The PWMI2 state %x\n",PWM_READ_REG(PWM2_STATE_REG));
				break;
				
			default:
				PWM_WRITE_REG(PWM0_CTRL_REG,PWM_DISABLE);
				
				PWM_WRITE_REG(PWM0_CFG_REG0,period);
				PWM_WRITE_REG(PWM0_CFG_REG1,duty);
				PWM_WRITE_REG(PWM0_CFG_REG2,10);//pwm output number
				
				PWM_WRITE_REG(PWM0_CTRL_REG,(1<<2|PWM_ENABLE));// keep the pwm always output;
				//printk("The PWMII0 state %x\n",PWM_READ_REG(PWM0_STATE_REG));
				break;
		}
	}
	else
	{
		PWM_DRV_Disable(pwm_num);
	}
	

	return 0;
}

/* file operation                                                           */

int PWM_Open(struct inode * inode, struct file * file)
{
   return 0 ;

}

int  PWM_Close(struct inode * inode, struct file * file)
{
    return 0;
}

static long PWM_Ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    PWM_DATA_S __user *argp = (PWM_DATA_S __user*)arg;
    
	unsigned char  PwmNum; 
    unsigned int Duty; 
    unsigned int Period;
	unsigned char  enable;
    
    switch (cmd)
    {
        case PWM_CMD_WRITE:
        {
            PwmNum  = argp->pwm_num;
            Duty    = argp->duty;
            Period  = argp->period;
           	enable  = argp->enable;
			
            PWM_DRV_Write(PwmNum,Duty,Period,enable);
            break;
        }

        case PWM_CMD_READ:
        {
			/*
            devAdd = argp->dev_addr;
            RegAddr= argp->reg_addr;
            Reg_Len= argp->addr_byte  ;
            DataLen= argp->data_byte   ;

            argp->data = I2C_DRV_Read(1, devAdd, RegAddr, Reg_Len, DataLen);
            */
		    break;
        }

        default:
        {
            printk("invalid ioctl command!\n");
            return -ENOIOCTLCMD;
        }
    }

    return 0 ;
}

static struct file_operations pwm_fops = {
    .owner      = THIS_MODULE,
    .unlocked_ioctl = PWM_Ioctl  ,
    .open       = PWM_Open   ,
    .release    = PWM_Close  ,
};

static struct miscdevice pwm_dev = {
   .minor   = MISC_DYNAMIC_MINOR,
   .name    = "pwm"    ,
   .fops    = &pwm_fops,
};


/* module init and exit                                                     */

static int __init pwm_init(void)
{
    int     ret;
    reg_pwmI_base_va = ioremap_nocache(PWMI_ADRESS_BASE, 0x10000);
    ret = misc_register(&pwm_dev);
    if(ret != 0)
    {
    	printk("register i2c device failed with %#x!\n", ret);
    	return -1;
    }
    
    return 0;    
}

static void __exit pwm_exit(void)
{
	int i;
	for(i=0;i<PWM_NUM_MAX;i++)
	{
		PWM_DRV_Disable(i);
	}
    iounmap((void*)reg_pwmI_base_va);
	
    misc_deregister(&pwm_dev);
}


module_init(pwm_init);
module_exit(pwm_exit);

MODULE_DESCRIPTION("PWM Driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("hisilicon");

