/*  extdrv/interface/ssp/hi_ssp.c
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
 *      21-April-2006 create this file
 */

#include <linux/module.h>
//#include <linux/config.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/fcntl.h>

#include <linux/init.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
//#include <linux/workqueue.h>

#include <asm/uaccess.h>
#include <asm/system.h>
#include <asm/io.h>

#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/slab.h>
//#include <linux/smp_lock.h>
//#include <mach/hardware.h>
//#include <linux/kcom.h>
//#include <kcom-hidmac.h>
#include "hi_ssp.h"
//#include "hi_common.h"


#define  ssp_readw(addr,ret)			(ret =(*(volatile unsigned int *)(addr)))
#define  ssp_writew(addr,value)			((*(volatile unsigned int *)(addr)) = (value))

#define  HI_REG_READ(addr,ret)			(ret =(*(volatile unsigned int *)(addr)))
#define  HI_REG_WRITE(addr,value)		((*(volatile unsigned int *)(addr)) = (value))	

//#define SSP_BASE	0x200C0000
//#define SSP_SIZE	0x10000			// 64KB
//#define SSP_INT		65					// Interrupt No.

#define SSP_BASE	0x200C0000
#define SSP_SIZE	0x10000				// 64KB
#define SSP_INT		65					 // Interrupt No.

//V3	SPI0_CSN0	IPU/O	4	3.3	
//功能0：GPIO5_1  通用输入输出
//功能1：SPI0_CSN0   SPI的片选0输出

//V2	   SPI0_CSN1	 IPU/O	4	3.3	
//功能0：GPIO5_2    通用输入输出
//功能1：SPI0_CSN1  SPI的片选1输出
//功能2：VOU0_DV    BT.1120数据有效信号

//SPI0_SCLK	IPD/O	8	3.3	
//功能0：  GPIO4_6  通用输入输出
//功能1：SPI0_SCLK  SPI时钟信号

//SPI0_SDI	IPD/O	4	3.3	
//功能0：GPIO5_0    通用输入输出
//功能1：SPI0_SDI   SPI数据输入

//W1	SPI0_SDO	IPD/O	4	3.3	
//功能0：GPIO4_7   通用输入输出
//功能1：SPI0_SDO  SPI数据输出






void __iomem *reg_ssp_base_va;
#define IO_ADDRESS_VERIFY(x) (reg_ssp_base_va + ((x)-(SSP_BASE)))

#ifdef SSP_USE_GPIO_DO_CS
#define GPIO_CS_BASE		0x200C0000
#define GPIO_CS_SIZE		0x10000

void __iomem *reg_gpio_cs_va;
#define IO_ADDRESS_2(x)	(reg_gpio_cs_va + x)

/* gpio cs reg */
#define GPIO_CS		IO_ADDRESS_2(0x40)
#define GPIO_0_DIR  IO_ADDRESS_2(0x400)

#define SSP_CS		(1 << 4)    /* GPIO 0_4 */

#define HW_REG(reg)         *((volatile unsigned int *)(reg))

#endif


/* SSP register definition .*/
#define SSP_CR0              IO_ADDRESS_VERIFY(SSP_BASE + 0x00)
#define SSP_CR1              IO_ADDRESS_VERIFY(SSP_BASE + 0x04)
#define SSP_DR               IO_ADDRESS_VERIFY(SSP_BASE + 0x08)
#define SSP_SR               IO_ADDRESS_VERIFY(SSP_BASE + 0x0C)
#define SSP_CPSR             IO_ADDRESS_VERIFY(SSP_BASE + 0x10)
#define SSP_IMSC             IO_ADDRESS_VERIFY(SSP_BASE + 0x14)
#define SSP_RIS              IO_ADDRESS_VERIFY(SSP_BASE + 0x18)
#define SSP_MIS              IO_ADDRESS_VERIFY(SSP_BASE + 0x1C)
#define SSP_ICR              IO_ADDRESS_VERIFY(SSP_BASE + 0x20)
#define SSP_DMACR            IO_ADDRESS_VERIFY(SSP_BASE + 0x24)

unsigned int ssp_dmac_rx_ch,ssp_dmac_tx_ch;


#ifdef SSP_USE_GPIO_DO_CS
static inline int gpio_cs_high(void)
{
	unsigned char regvalue;

	regvalue = HW_REG(GPIO_0_DIR);
	regvalue |= SSP_CS; // output
	HW_REG(GPIO_0_DIR) = regvalue;
	
	regvalue = HW_REG(GPIO_CS);
	regvalue |= SSP_CS;
	HW_REG(GPIO_CS) = regvalue;

//	ssp_writew(GPIO_CS, 1);
	return 0;
}

static inline int gpio_cs_low(void)
{
	unsigned char regvalue;

	regvalue = HW_REG(GPIO_0_DIR);
	regvalue |= SSP_CS; // output
	HW_REG(GPIO_0_DIR) = regvalue;
	
	regvalue = HW_REG(GPIO_CS);
	regvalue &= (~SSP_CS);
	HW_REG(GPIO_CS) = regvalue;

//	ssp_writew(GPIO_CS, 0);
	return 0;
}
#endif

void hi_ssp_writeOnly(int bWriteOnly)
{
    int ret = 0;

    bWriteOnly = 0;

    ssp_readw(SSP_CR1,ret);

    if (bWriteOnly)
    {
        ret = ret | (0x1 << 5);
    }
    else
    {
        ret = ret & (~(0x1 << 5));
    }

    ssp_writew(SSP_CR1,ret);
}

/*
 * enable SSP routine.
 *
 */
void hi_ssp_enable(void)
{
    int ret = 0;
    ssp_readw(SSP_CR1,ret);
    ret = (ret & 0xFFFD) | 0x2;

    ret = ret | (0x1 << 4); /* big/little end, 1: little, 0: big */

    ret = ret | (0x1 << 15); /* wait en */

    ssp_writew(SSP_CR1,ret);

    hi_ssp_writeOnly(0);
}





/*
 * disable SSP routine.
 *
 */

void hi_ssp_disable(void)
{
    int ret = 0;
    ssp_readw(SSP_CR1,ret);
    ret = ret & (~(0x1 << 1));
    ssp_writew(SSP_CR1,ret);
}

/*
 * set SSP frame form routine.
 *
 * @param framemode: frame form
 * 00: Motorola SPI frame form.
 * when set the mode,need set SSPCLKOUT phase and SSPCLKOUT voltage level.
 * 01: TI synchronous serial frame form
 * 10: National Microwire frame form
 * 11: reserved
 * @param sphvalue: SSPCLKOUT phase (0/1)
 * @param sp0: SSPCLKOUT voltage level (0/1)
 * @param datavalue: data bit
 * 0000: reserved    0001: reserved    0010: reserved    0011: 4bit data
 * 0100: 5bit data   0101: 6bit data   0110:7bit data    0111: 8bit data
 * 1000: 9bit data   1001: 10bit data  1010:11bit data   1011: 12bit data
 * 1100: 13bit data  1101: 14bit data  1110:15bit data   1111: 16bit data
 *
 * @return value: 0--success; -1--error.
 *
 */

int hi_ssp_set_frameform(unsigned char framemode,unsigned char spo,unsigned char sph,unsigned char datawidth)
{
    int ret = 0;
    ssp_readw(SSP_CR0,ret);
    if(framemode > 3)
    {
        printk("set frame parameter err.\n");
        return -1;
    }
    ret = (ret & 0xFFCF) | (framemode << 4);
    if((ret & 0x30) == 0)
    {
        if(spo > 1)
        {
            printk("set spo parameter err.\n");
            return -1;
        }
        if(sph > 1)
        {
            printk("set sph parameter err.\n");
            return -1;
        }
        ret = (ret & 0xFF3F) | (sph << 7) | (spo << 6);
    }
    if((datawidth > 16) || (datawidth < 4))
    {
        printk("set datawidth parameter err.\n");
        return -1;
    }
    ret = (ret & 0xFFF0) | (datawidth -1);
    ssp_writew(SSP_CR0,ret);
    return 0;
}

/*
 * set SSP serial clock rate routine.
 *
 * @param scr: scr value.(0-255,usually it is 0)
 * @param cpsdvsr: Clock prescale divisor.(2-254 even)
 *
 * @return value: 0--success; -1--error.
 *
 */

int hi_ssp_set_serialclock(unsigned char scr,unsigned char cpsdvsr)
{
    int ret = 0;
    ssp_readw(SSP_CR0,ret);
    ret = (ret & 0xFF) | (scr << 8);
    ssp_writew(SSP_CR0,ret);
    if((cpsdvsr & 0x1))
    {
        printk("set cpsdvsr parameter err.\n");
        return -1;
    }
    ssp_writew(SSP_CPSR,cpsdvsr);
    return 0;
}

int hi_ssp_alt_mode_set(int enable)
{
	int ret = 0;
	
	ssp_readw(SSP_CR1,ret);
	if (enable)
	{
		ret = ret & (~0x40);
	}
	else
	{
	    ret = (ret & 0xFF) | 0x40;
	}
	ssp_writew(SSP_CR1,ret);

    return 0;
}

/*
 * set SSP interrupt routine.
 *
 * @param regvalue: SSP_IMSC register value.(0-255,usually it is 0)
 *
 */
void hi_ssp_set_inturrupt(unsigned char regvalue)
{

    ssp_writew(SSP_IMSC,(regvalue&0x0f));
}

/*
 * clear SSP interrupt routine.
 *
 */

void hi_ssp_interrupt_clear(void)
{
    ssp_writew(SSP_ICR,0x3);
}

/*
 * enable SSP dma mode routine.
 *
 */

void hi_ssp_dmac_enable(void)
{
    ssp_writew(SSP_DMACR,0x3);
}

/*
 * disable SSP dma mode routine.
 *
 */

void hi_ssp_dmac_disable(void)
{
    ssp_writew(SSP_DMACR,0);
}



/*
 * check SSP busy state routine.
 *
 * @return value: 0--free; 1--busy.
 *
 */

unsigned int hi_ssp_busystate_check(void)
{
    int ret = 0;
    ssp_readw(SSP_SR,ret);
    if((ret & 0x10) != 0x10)
        return 0;
    else
        return 1;
}

unsigned int hi_ssp_is_fifo_empty(int bSend)
{
    int ret = 0;
    ssp_readw(SSP_SR,ret);

    if (bSend)
    {
        if((ret & 0x1) == 0x1) /* send fifo */
            return 1;
        else
            return 0;
    }
    else
    {
        if((ret & 0x4) == 0x4) /* receive fifo */
            return 0;
        else
            return 1;
    }
}

/*
 *  write SSP_DR register rountine.
 *
 *  @param  sdata: data of SSP_DR register
 *
 */


void hi_ssp_writedata(unsigned short sdata)
{
    ssp_writew(SSP_DR,sdata);
    udelay(2);
}

/*
 *  read SSP_DR register rountine.
 *
 *  @return value: data from SSP_DR register readed
 *
 */

int hi_ssp_readdata(void)
{
    int ret = 0;
    ssp_readw(SSP_DR,ret);
    return ret;
}

#if 0

/*
 * check SSP busy state routine.
 * @param prx_dmac_hook : dmac rx interrupt function pointer
 * @param ptx_dmac_hook : dmac tx interrupt function pointer
 *
 * @return value: 0--success; -1--error.
 *
 */

int hi_ssp_dmac_init(void * prx_dmac_hook,void * ptx_dmac_hook)
{
	ssp_dmac_rx_ch = dmac_channel_allocate(prx_dmac_hook);
	if(ssp_dmac_rx_ch < 0)
	{
	    printk("SSP no available rx channel can allocate.\n");
	    return -1;
	}
	ssp_dmac_tx_ch = dmac_channel_allocate(ptx_dmac_hook);
	if(ssp_dmac_tx_ch < 0)
	{
	    printk("SSP no available tx channel can allocate.\n");
	    dmac_channel_free(ssp_dmac_rx_ch);
	    return -1;
	}
	return 0;
}


/*
 * SSP dma mode data transfer routine.
 * @param phy_rxbufaddr : rxbuf physical address
 * @param phy_txbufaddr : txbuf physical address
 * @param transfersize : transfer data size
 *
 * @return value: 0--success; -1--error.
 *
 */
int hi_ssp_dmac_transfer(unsigned int phy_rxbufaddr,unsigned int phy_txbufaddr,unsigned int transfersize)
{
	int ret=0;

	ret=dmac_start_m2p(ssp_dmac_rx_ch,phy_rxbufaddr, DMAC_SSP_RX_REQ, transfersize,0);
	if(ret != 0)
	{
		return(ret);
    }
	ret=dmac_start_m2p(ssp_dmac_tx_ch,phy_txbufaddr, DMAC_SSP_TX_REQ, transfersize,0);
	if(ret != 0)
	{
		return(ret);
    }
	dmac_channelstart(ssp_dmac_rx_ch);
	dmac_channelstart(ssp_dmac_tx_ch);

	return 0;
}


/*
 * SSP dma mode exit
 *
 * @return value: 0 is ok
 *
 */
void hi_ssp_dmac_exit(void)
{
	dmac_channel_free(ssp_dmac_rx_ch);
	dmac_channel_free(ssp_dmac_tx_ch);
}

DECLARE_KCOM_HI_DMAC();

#endif

static void spi_enable(void)
{
//	int ret;
	
#if 1
//  HI_REG_WRITE(SSP_CR1, 0x02);
  HI_REG_WRITE(SSP_CR1, 0x42);
#else
    ssp_readw(SSP_CR1,ret);
    ret = (ret & 0xFFFD) | 0x2;
//    printk("enable cr1 = %#x\n", ret);
    ssp_writew(SSP_CR1,ret);
#endif
}

static void spi_disable(void)
{
//	int ret;
#if 1
    HI_REG_WRITE(SSP_CR1, 0x40);
//    HI_REG_WRITE(SSP_CR1, 0x0);
#else
    ssp_readw(SSP_CR1,ret);
    ret = ret & 0xFFFD;
    ssp_writew(SSP_CR1,ret);
#endif
}
int spiget(void)
{
    int regval;
    HI_REG_READ(SSP_DR,regval);
    return regval;
}

void spiset(int data)
{
 HI_REG_WRITE(SSP_DR, data);
}

 void spi_wait_setfinish(void)
{
    int regval;
    do
    {
        HI_REG_READ(SSP_SR,regval);
    }
    while((regval & 0x11) != 0x01);  
}


 void spiset_burst(int data)
{
//    int regval;
    spi_enable();
    spiset(data);
    spi_wait_setfinish();
    spi_disable();
}
//static unsigned int  sspinitialized =0;



int hi_ssp_init_cfg(void)
{
	unsigned char framemode = 0;
	unsigned char spo = 0;
	unsigned char sph = 0;
	unsigned char datawidth = 16;

	unsigned char scr = 4;
	unsigned char cpsdvsr = 10;
	
	spi_disable();

	hi_ssp_set_frameform(framemode, spo, sph, datawidth);

	hi_ssp_set_serialclock(scr, cpsdvsr);

	// altasens mode
	hi_ssp_alt_mode_set(1);

	hi_ssp_enable();
//	spi_enable();

    return 0;
}


unsigned short hi_ssp_read_alt(unsigned short addr)
{
	unsigned int ret;
	unsigned short value = 0, dontcare = 0x0000; 

	spi_enable();

#ifdef SSP_USE_GPIO_DO_CS
	gpio_cs_low();
#endif	

	//hi_ssp_enable();
	ssp_writew(SSP_DR, addr);
	//spi_wait_setfinish();
	ssp_writew(SSP_DR, dontcare);
	//spi_wait_setfinish();
	//ssp_writew(SSP_DR, dontcare);
	//spi_wait_setfinish();

	// judge send fifo empty?
//	while(!hi_ssp_is_fifo_empty(1)){};

#ifdef SSP_USE_GPIO_DO_CS
	//udelay(50);
	//gpio_cs_high();
#endif	
	while(hi_ssp_is_fifo_empty(0)){};
	ssp_readw(SSP_DR, ret);
	//printk("Debug:1 ret(%#x) == addr(%#x)?\n", ret, addr);
    
	while(hi_ssp_is_fifo_empty(0)){};
	ssp_readw(SSP_DR, ret);
	//printk("Debug:2 ret(%#x) == addr(%#x)?\n", ret, addr);

	//while(hi_ssp_is_fifo_empty(0)){};
	//ssp_readw(SSP_DR, ret);
	//printk("Debug:3 ret(%#x) == addr(%#x)?\n", ret, addr);

#ifdef SSP_USE_GPIO_DO_CS
	gpio_cs_high();
#endif	

	spi_disable();

	value = (unsigned short)(ret & 0xffff);
	
    return value;
}

int hi_ssp_write_alt(unsigned short addr, unsigned short data)
{
	unsigned int ret;

	spi_enable();
	//hi_ssp_enable();

	//udelay(10);

#ifdef SSP_USE_GPIO_DO_CS
	gpio_cs_low();
#endif	

#if 1
	ssp_writew(SSP_DR, addr);
	//spi_wait_setfinish();
	ssp_writew(SSP_DR, data);
	//spi_wait_setfinish();
#if 0
	ssp_writew(SSP_DR, data);
	ssp_readw(SSP_DR, ret);
#endif	

	// wait receive fifo has data
	while(hi_ssp_is_fifo_empty(0)){};
	ssp_readw(SSP_DR, ret);
	
	while(hi_ssp_is_fifo_empty(0)){};
	ssp_readw(SSP_DR, ret);

#else

	spiset_burst(addr);
	spiset_burst(data);

	ssp_readw(SSP_DR, ret);
	ssp_readw(SSP_DR, ret);

#endif

#ifdef SSP_USE_GPIO_DO_CS

	//udelay(30);
	gpio_cs_high();
#endif	
	
	spi_disable();
    
	return 0;
}


//int ssp_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
long ssp_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    unsigned int val;
    unsigned short addr, data;
	
	switch(cmd)
	{
		case SSP_READ_ALT:
			val = *(unsigned int *)arg;
			addr = ((unsigned short)(val&0xffff) | 0x8000) ;	// [15]: 1 means read
			data = hi_ssp_read_alt(addr);
			*(unsigned int *)arg = (unsigned int)(data&0x0000ffff);
			//printk("Debug: ----ssp read now!\n");
			break;
		
		case SSP_WRITE_ALT:
			val = *(unsigned int *)arg;
			addr = (unsigned short)((val&0xffff0000)>>16) & 0x7fff;		// [15]: 0 means write, [13]: should always 0
			data = (unsigned short)((val&0x0000ffff)>> 0);
			hi_ssp_write_alt(addr, data);
			//printk("Debug: ----ssp write now!\n");
			break;		
	
		default:
		{
			printk("Kernel: No such ssp command %#x!\n", cmd);
			return -1;
		}
	}

    return 0;
}

int ssp_open(struct inode * inode, struct file * file)
{
    return 0;
}
int ssp_close(struct inode * inode, struct file * file)
{
    return 0;
}


static struct file_operations ssp_fops = {
    .owner      = THIS_MODULE,
    //.ioctl      = ssp_ioctl,
    .unlocked_ioctl = ssp_ioctl,
    .open       = ssp_open,
    .release    = ssp_close
};


static struct miscdevice ssp_dev = {
   .minor	= MISC_DYNAMIC_MINOR,
   .name    = "ssp",
   .fops    = &ssp_fops,
};


/*
 * initializes SSP interface routine.
 *
 * @return value:0--success.
 *
 */
static int __init hi_ssp_init(void)
{
//    unsigned int reg;
    int ret;
    #if 0
    KCOM_HI_DMAC_INIT();

    if(sspinitialized == 0)
    {
        reg = readl(IO_ADDRESS_VERIFY(0x101E0040));
        reg &= 0xfffcf3ff;
        reg |= 0x00010800;
        writel(reg,IO_ADDRESS_VERIFY(0x101E0040));
        sspinitialized = 1;

        printk("Load hi_ssp.ko success.  \t(%s)\n", VERSION_STRING);

        return 0;
    }
    else
    {
        printk("SSP has been initialized.\n");
        return 0;
    }
    #else
    //spiconfig_multi();


	reg_ssp_base_va = ioremap_nocache((unsigned long)SSP_BASE, (unsigned long)SSP_SIZE);
	if (!reg_ssp_base_va)
	{
		printk("Kernel: ioremap ssp base failed!\n");
	    return -ENOMEM;
	}
#ifdef SSP_USE_GPIO_DO_CS
	reg_gpio_cs_va = ioremap_nocache((unsigned long)GPIO_CS_BASE, (unsigned long)GPIO_CS_SIZE);
	if (!reg_gpio_cs_va)
	{
		printk("Kernel: ioremap gpio cs failed!\n");
	    return -ENOMEM;
	}

#ifdef SSP_USE_GPIO_DO_CS
	gpio_cs_high();
#endif	

#endif

    ret = misc_register(&ssp_dev);
    if(0 != ret)
    {
    	printk("Kernel: register ssp_0 device failed!\n");
    	return -1;
	}

	ret = hi_ssp_init_cfg();
	if (ret)
	{
	    printk("Debug: ssp initial failed!\n");
	    return -1;
	}
	
	printk("Kernel: ssp initial ok!\n");
    
    return 0;
    #endif
}

static void __exit hi_ssp_exit(void)
{
    #if 0
    sspinitialized =0;
    hi_ssp_dmac_exit();
    KCOM_HI_DMAC_EXIT();
    #endif

    hi_ssp_disable();

    iounmap((void*)reg_ssp_base_va);

#ifdef SSP_USE_GPIO_DO_CS
    iounmap((void*)reg_gpio_cs_va);
#endif 	
	
    misc_deregister(&ssp_dev);
}

module_init(hi_ssp_init);
module_exit(hi_ssp_exit);

EXPORT_SYMBOL(hi_ssp_readdata);
EXPORT_SYMBOL(hi_ssp_writedata);
EXPORT_SYMBOL(hi_ssp_set_frameform);
EXPORT_SYMBOL(hi_ssp_set_serialclock);
EXPORT_SYMBOL(spiget);
EXPORT_SYMBOL(spiset);
EXPORT_SYMBOL(spi_wait_setfinish);
EXPORT_SYMBOL(spiset_burst);

MODULE_LICENSE("GPL");





