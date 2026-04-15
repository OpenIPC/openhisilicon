/*  extdrv/interface/i2c/hi_i2c.c
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
 *      19-April-2006 create this file
 */
#include <linux/module.h>
//#include <asm/hardware.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/fcntl.h>
#include <linux/spinlock.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/workqueue.h>

#include <asm/uaccess.h>
#include <asm/io.h>

#include "hi_i2c.h"
#if HI_ARCH==3518
#include "isp_ext.h"
#endif
#define I2C_WAIT_TIME_OUT       0x1000 
#define I2C_ADRESS_BASE         0x200D0000

/* Hi3518A/18C/18E Asic apb clk is 100M,  Hi3516C Asic apb clk is 110M
if DDR clk change, Asic apb clk should be change.  I2C_DFT_CLK is Asic apb clk*/
#define I2C_DFT_CLK            (100000000)    //(25000000)
#define I2C_DFT_RATE           (100000)

void __iomem * reg_i2c_base_va = 0;
#define HI_IO_ADDRESS(x) (reg_i2c_base_va + ((x)-(I2C_ADRESS_BASE)))

#define I2C_CTRL_REG      HI_IO_ADDRESS(I2C_ADRESS_BASE + 0x000)
#define I2C_COM_REB       HI_IO_ADDRESS(I2C_ADRESS_BASE + 0x004)
#define I2C_ICR_REG       HI_IO_ADDRESS(I2C_ADRESS_BASE + 0x008)
#define I2C_SR_REG        HI_IO_ADDRESS(I2C_ADRESS_BASE + 0x00C)
#define I2C_SCL_H_REG     HI_IO_ADDRESS(I2C_ADRESS_BASE + 0x010)
#define I2C_SCL_L_REG     HI_IO_ADDRESS(I2C_ADRESS_BASE + 0x014)
#define I2C_TXR_REG       HI_IO_ADDRESS(I2C_ADRESS_BASE + 0x018)
#define I2C_RXR_REG       HI_IO_ADDRESS(I2C_ADRESS_BASE + 0x01C)


#define  I2C_WRITE_REG(Addr, Value) ((*(volatile unsigned int *)(Addr)) = (Value))
#define  I2C_READ_REG(Addr)         (*(volatile unsigned int *)(Addr))

spinlock_t g_stI2CLock;
#define I2C_SPIN_LOCK_INIT      spin_lock_init(&g_stI2CLock)
#define I2C_SPIN_LOCK(flags)    spin_lock_irqsave(&g_stI2CLock, flags)
#define I2C_SPIN_UNLOCK(flags)  spin_unlock_irqrestore(&g_stI2CLock, flags)

static void I2C_DRV_SetRate(unsigned int I2cRate)
{
    unsigned int  Value = 0;
    unsigned int  SclH = 0;
    unsigned int  SclL = 0;

    /* Read CTRL */
    Value = I2C_READ_REG(I2C_CTRL_REG);

    /* Disable I2C interrupt */
    I2C_WRITE_REG(I2C_CTRL_REG, (Value & (~I2C_UNMASK_TOTAL)));

    SclH = (I2C_DFT_CLK / (I2cRate * 2)) / 2 - 1;
    I2C_WRITE_REG(I2C_SCL_H_REG, SclH);

    SclL = (I2C_DFT_CLK / (I2cRate * 2)) / 2 - 1;
    I2C_WRITE_REG(I2C_SCL_L_REG, SclL);

    /* Enable I2C interrupt */
    I2C_WRITE_REG(I2C_CTRL_REG, Value);

    return;
}

int I2C_DRV_WaitWriteEnd(unsigned int I2cNum)
{
    unsigned int  I2cSrReg;
    unsigned int  i = 0;

	do
	{
        I2cSrReg = I2C_READ_REG(I2C_SR_REG);

        if (i > I2C_WAIT_TIME_OUT)
        {
            //printk("wait write data timeout!\n");
            return -1;
        }
        i++;
	}while((I2cSrReg & I2C_OVER_INTR) != I2C_OVER_INTR);

  I2C_WRITE_REG(I2C_ICR_REG, I2C_CLEAR_ALL);

	return 0;
}

int I2C_DRV_WaitRead(unsigned int I2cNum)
{
    unsigned int  I2cSrReg;
    unsigned int  i = 0;

	do
	{
		I2cSrReg = I2C_READ_REG(I2C_SR_REG);

		if (i > I2C_WAIT_TIME_OUT)
        {
            //printk("wait write data timeout!\n");
            return -1;
        }
        i++;
	}while((I2cSrReg & I2C_RECEIVE_INTR) != I2C_RECEIVE_INTR);

	return 0;
}


/*
add by Jiang Lei 2010-08-24
I2C write-confirm function
Mainly used for EEPROM: poll the device to confirm write completion.
    After the stop bit, the EEPROM begins its internal write cycle.
    The I2C master uses acknowledge polling to confirm the write
*/
int I2C_DRV_WriteConfig(unsigned char I2cDevAddr)
{
    unsigned int          i = 0;
    unsigned int          j = 0;
    unsigned int          I2cSrReg;
    unsigned long         flags;
    
    I2C_SPIN_LOCK(flags);

    do
    {
        I2C_WRITE_REG(I2C_TXR_REG,(I2cDevAddr & WRITE_OPERATION));
        I2C_WRITE_REG(I2C_COM_REB, (I2C_WRITE | I2C_START));

        j = 0;
    	do
    	{
            I2cSrReg = I2C_READ_REG(I2C_SR_REG);

            if (j > I2C_WAIT_TIME_OUT)
            {
                //printk("wait write data timeout!\n");
                I2C_SPIN_UNLOCK(flags);
                return -1;
            }
            j++;
    	}while((I2cSrReg & I2C_OVER_INTR) != I2C_OVER_INTR);

        I2cSrReg = I2C_READ_REG( I2C_SR_REG);
        I2C_WRITE_REG(I2C_ICR_REG, I2C_CLEAR_ALL);

        i++;

        if (i > 0x200000) //I2C_WAIT_TIME_OUT)
        {
            printk("wait write ack ok timeout!\n");
            I2C_SPIN_UNLOCK(flags);
            return -1;
        }
    }while ((I2cSrReg & I2C_ACK_INTR));
    I2C_SPIN_UNLOCK(flags);

    return 0;
}

int I2C_DRV_Write(unsigned int I2cNum, unsigned char I2cDevAddr, unsigned int I2cRegAddr, unsigned int I2cRegAddrByteNum, unsigned int Data, unsigned int DataLen)
{
    unsigned int   i       ;
    unsigned int   RegAddr ;
    unsigned int   TXR_data;
    unsigned long  flags;

    I2C_SPIN_LOCK(flags);
    
    /* Clear interrupt flags */
    //I2C_WRITE_REG(I2C_ICR_REG, I2C_CLEAR_ALL);//08,03
    I2C_WRITE_REG(I2C_ICR_REG , 0x03 );//08,03
    I2C_WRITE_REG(I2C_CTRL_REG, 0x187);//0x00
    /* Send device address (write) */
    I2C_WRITE_REG(I2C_TXR_REG, (I2cDevAddr & WRITE_OPERATION));
    I2C_WRITE_REG(I2C_COM_REB, (I2C_WRITE | I2C_START));//04,0a

    if (I2C_DRV_WaitWriteEnd(I2cNum))
    {
        //printk("write devicAdd in write process!\n");
    }

    /* Send register address */
    for(i=0; i<I2cRegAddrByteNum; i++)
    {
        RegAddr = I2cRegAddr >> ((I2cRegAddrByteNum -i -1) * 8);
        I2C_WRITE_REG(I2C_TXR_REG, RegAddr  );//018,register
        I2C_WRITE_REG(I2C_COM_REB, I2C_WRITE);//04,02

        if (I2C_DRV_WaitWriteEnd(I2cNum))
        {
            //local_irq_restore(IntFlag);
            //printk("write write register ok in write process!\n");
        }
        
        //I2C_WRITE_REG(I2C_ICR_REG, I2C_CLEAR_ALL);//08,3f
    }

	/* Send data */
	for (i=0; i<DataLen; i++)
	{
      TXR_data = Data >> ((DataLen -i -1) * 8);
      I2C_WRITE_REG(I2C_TXR_REG, TXR_data );
    	I2C_WRITE_REG(I2C_COM_REB, I2C_WRITE);

        if (I2C_DRV_WaitWriteEnd(I2cNum))
        {
            //printk("write register data!\n");
        }
      I2C_WRITE_REG(I2C_ICR_REG, I2C_CLEAR_ALL);//08,3f
	}

	/* Send stop flag */
	I2C_WRITE_REG(I2C_COM_REB, I2C_STOP);
    if (I2C_DRV_WaitWriteEnd(I2cNum))
    {
        //printk("wait write process end!\n");
    }
    I2C_WRITE_REG(I2C_ICR_REG, 0x01);

    I2C_SPIN_UNLOCK(flags);
    
    return 0;
}


unsigned int I2C_DRV_Read(unsigned int I2cNum, unsigned char I2cDevAddr, unsigned int I2cRegAddr, unsigned int I2cRegAddrByteNum, unsigned int DataLen)
{
    unsigned int    dataTmp = 0xff;
    unsigned int    i             ;
    unsigned int    RegAddr       ;
    unsigned int    Data=0        ;
    unsigned long   flags;

    I2C_SPIN_LOCK(flags);

    /* Clear interrupt flags */
    //I2C_WRITE_REG(I2C_ICR_REG, I2C_CLEAR_ALL);
    I2C_WRITE_REG(I2C_ICR_REG , 0x03 );//08,03
    I2C_WRITE_REG(I2C_CTRL_REG, 0x187);//0x00

    /* Send device address (write) */
    I2C_WRITE_REG(I2C_TXR_REG, (I2cDevAddr & WRITE_OPERATION));
    I2C_WRITE_REG(I2C_COM_REB,(I2C_WRITE | I2C_START));

    if (I2C_DRV_WaitWriteEnd(I2cNum))
    {
        //printk("write first device ok in reading process!\n");
    }
    
    //I2C_WRITE_REG(I2C_ICR_REG, I2C_CLEAR_ALL);//08,3f
    
    /* Send register address */
    for(i=0; i<I2cRegAddrByteNum; i++)
    {
        RegAddr = I2cRegAddr >> ((I2cRegAddrByteNum -i -1) * 8);
        I2C_WRITE_REG(I2C_TXR_REG, RegAddr  );
        I2C_WRITE_REG(I2C_COM_REB, I2C_WRITE);

        if (I2C_DRV_WaitWriteEnd(I2cNum))
        {
            //printk("write register ok in reading process!\n");
            //return -1;
        }
        //I2C_WRITE_REG(I2C_ICR_REG, I2C_CLEAR_ALL);//08,3f
    }

    /* Send device address (read) */
    I2C_WRITE_REG(I2C_TXR_REG, (I2cDevAddr | READ_OPERATION));
    I2C_WRITE_REG(I2C_COM_REB, I2C_WRITE | I2C_START);

    if (I2C_DRV_WaitWriteEnd(I2cNum))
    {
        //printk("line :  %d    \n",__LINE__);
        //printk("write second devaddr ok!\n");
    }
    //I2C_WRITE_REG(I2C_ICR_REG, I2C_CLEAR_ALL);//08,3f
    
    
    /* Read data */
    for(i=0; i<DataLen; i++)
    {
        /* Last byte: do not send ACK */
        if (i == (DataLen - 1))
        {
            I2C_WRITE_REG(I2C_COM_REB, (I2C_READ | (~I2C_SEND_ACK)));
        }
        /* Read data and send ACK */
        else
        {
            I2C_WRITE_REG(I2C_COM_REB, I2C_READ);
        }

        if (I2C_DRV_WaitRead(I2cNum))
        {
            //local_irq_restore(IntFlag);
            //printk("read data!\n");
        }

        dataTmp = I2C_READ_REG(I2C_RXR_REG);
        Data= Data | (dataTmp<<((DataLen-i-1)*8)) ;

        if (I2C_DRV_WaitWriteEnd(I2cNum))
        {
            //local_irq_restore(IntFlag);
            //printk("wait write data timeout!\n");
        }
       //I2C_WRITE_REG(I2C_ICR_REG, I2C_CLEAR_ALL);//08,3f
    }

    /* Send stop flag */
    I2C_WRITE_REG(I2C_COM_REB, I2C_STOP);
    if (I2C_DRV_WaitWriteEnd(I2cNum))
    {
        //local_irq_restore(IntFlag);
        //printk("line:  %d  \n",__LINE__);
        //printk("wait write data timeout!\n");
    }
    I2C_SPIN_UNLOCK(flags);

    //local_irq_restore(IntFlag);
    return  Data ;

}
//------set----
int HI_I2C_WriteConfig(unsigned char I2cDevAddr)
{
	  int Ret ;
    Ret = I2C_DRV_WriteConfig(I2cDevAddr);
    return Ret;
}

int HI_I2C_Write(unsigned char I2cDevAddr, unsigned int I2cRegAddr, unsigned int I2cRegAddrByteNum, unsigned int Data, unsigned int DataLen)
{
    int  Ret;

    Ret = I2C_DRV_Write(1, I2cDevAddr, I2cRegAddr, I2cRegAddrByteNum, Data, DataLen);

    return Ret;
}

int HI_I2C_Read(unsigned char I2cDevAddr, unsigned int I2cRegAddr, unsigned int I2cRegAddrByteNum, unsigned int DataLen)
{
    int  Ret;

    Ret = I2C_DRV_Read(1, I2cDevAddr, I2cRegAddr, I2cRegAddrByteNum, DataLen);

    return Ret;
}


int I2C_Open(struct inode * inode, struct file * file)
{
   return 0 ;

};

int  I2C_Close(struct inode * inode, struct file * file)
{
    return 0;
}


/*****************************************************************************/
//typedef struct I2C_DATA_s{
//	unsigned char  I2cDevAddr; 
//	unsigned int I2cRegAddr; 
//	unsigned int RegCount  ; 
//	unsigned int *pData    ; 
//  unsigned int DataLen   ;
//  }I2C_DATA_S ;
	

//static int I2C_Ioctl(struct inode *inode, struct file *file,
static long I2C_Ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    I2C_DATA_S __user *argp = (I2C_DATA_S __user*)arg;
    int       Ret = 0;
    
    unsigned char  devAdd ;
    unsigned int RegAddr; 
    unsigned int Reg_Len;
    unsigned int DataLen;
    unsigned int Wdata  ;
    
    switch (cmd)
    {
        case CMD_I2C_WRITE:
        {
            devAdd = argp->dev_addr;
            RegAddr= argp->reg_addr;
            Reg_Len= argp->addr_byte_num  ;
            Wdata  = argp->data    ;
            DataLen= argp->data_byte_num   ;
            //Ret = I2C_DRV_Write(I2cData.I2cNum, I2cData.I2cDevAddr, I2cData.I2cRegAddr, I2cData.I2cRegCount, pData, I2cData.DataLen);
            Ret = I2C_DRV_Write(1, devAdd, RegAddr, Reg_Len, Wdata, DataLen);
            if(Ret == 0)
            	{
            		//printk("line :%d      write ok!\n",__LINE__);
            	}
            break;
        }

        case CMD_I2C_READ:
        {
            devAdd = argp->dev_addr;
            RegAddr= argp->reg_addr;
            Reg_Len= argp->addr_byte_num  ;
            //Wdata  = argp->RWData    ;
            DataLen= argp->data_byte_num   ;
            
            Ret = I2C_DRV_Read(1, devAdd, RegAddr, Reg_Len, DataLen);
            argp->data = Ret ;
            
    		    break;
        }

        default:
        {
            printk("command error!\n");
        }
    }

    return 0 ;
}


//---------linux kernel need----------------
static struct file_operations hi_i2c_fops = {
    .owner      = THIS_MODULE,
   // .ioctl      = I2C_Ioctl  ,
    .unlocked_ioctl = I2C_Ioctl  ,
    .open       = I2C_Open   ,
    .release    = I2C_Close  ,
};

static struct miscdevice hi_i2c_dev = {
   .minor   = MISC_DYNAMIC_MINOR,
   .name    = "hi_i2c"    ,
   .fops    = &hi_i2c_fops,
};

static int __init hi_i2c_init(void)
{
    int     ret;

    #if HI_ARCH==3518
    ISP_BUS_CALLBACK_S stBusCb = {0};
    stBusCb.pfnISPWriteI2CData = HI_I2C_Write;
    if (CKFN_ISP_RegisterBusCallBack())
    {
        CALL_ISP_RegisterBusCallBack(0, ISP_BUS_TYPE_I2C, &stBusCb);
    }
    else
    {
        printk("register i2c_write_callback to isp failed, hi_i2c init is failed!\n");
        return -1;
    }
    #endif
    
    reg_i2c_base_va = ioremap_nocache(I2C_ADRESS_BASE, 0x10000);
    ret = misc_register(&hi_i2c_dev);
    if(0 == ret)
    {
    	printk("hi_i2c init is ok!\n");
    }
    	
    
   I2C_DRV_SetRate(I2C_DFT_RATE);
//   I2C_WRITE_REG(I2C_CTRL_REG, 0x187); 
 //  I2C_WRITE_REG(I2C_ICR_REG, I2C_CLEAR_ALL);
 //  I2C_WRITE_REG(I2C_CTRL_REG, (I2C_ENABLE | I2C_UNMASK_TOTAL | I2C_UNMASK_ALL));

    I2C_SPIN_LOCK_INIT;

    /*
     * Sony IMX sensors (IMX122, IMX138, IMX222, IMX322, IMX323) power up
     * with TESTEN=0 in the STANDBY register (I2C addr 0x3000), which
     * prevents all subsequent register writes from taking effect.  Write
     * 0x30 (STANDBY=0, TESTEN=3) to prime the sensor before userspace runs.
     *
     * The I2C device address 0x34 is the standard Sony IMX write address.
     * On sensors using a different bus (SPI), this write is harmless (NAK).
     */
    HI_I2C_Write(0x34, 0x3000, 2, 0x30, 1);

    return 0;
}

static void __exit hi_i2c_exit(void)
{
    I2C_WRITE_REG(I2C_CTRL_REG, (~I2C_ENABLE));
    iounmap((void*)reg_i2c_base_va);
    misc_deregister(&hi_i2c_dev);
    
}


EXPORT_SYMBOL(I2C_Open );
EXPORT_SYMBOL(I2C_Close);
EXPORT_SYMBOL(HI_I2C_WriteConfig);
EXPORT_SYMBOL(HI_I2C_Write);
EXPORT_SYMBOL(HI_I2C_Read );

module_init(hi_i2c_init);
module_exit(hi_i2c_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("hisilicon");

