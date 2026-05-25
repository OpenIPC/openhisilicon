/*
 * Simple synchronous userspace interface to SPI devices
 *
 * Copyright (C) 2006 SWAPP
 *	Andrea Paterniani <a.paterniani@swapp-eng.it>
 * Copyright (C) 2007 David Brownell (simplification, cleanup)
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/compat.h>
#include <linux/delay.h>
#ifdef __HuaweiLite__
#include <spi.h>
#include "fcntl.h"
#else
#include <linux/spi/spi.h>
#endif
#include <asm/uaccess.h>

#include "isp_ext.h"
#include "sensor_spi.h"

#define SPI_BUS_NUM_MAX		3
static unsigned spi_csn[SPI_BUS_NUM_MAX] = {1,2,1};
#ifdef __HuaweiLite__
static char sensor[64];
#else
static char *sensor = "";
#endif
#if 1
static unsigned bus_num = 0;
static unsigned csn = 0;

module_param(bus_num, uint, S_IRUGO);
MODULE_PARM_DESC(bus_num, "spi bus number");

module_param(csn, uint, S_IRUGO);
MODULE_PARM_DESC(csn, "chip select number");
#endif

/* some sensor has special dev addr */
module_param(sensor, charp, S_IRUGO);
MODULE_PARM_DESC(sensor, "sensor name");

#ifndef __HuaweiLite__
struct spi_master *hi_master[SPI_BUS_NUM_MAX];
struct spi_device *hi_spi[SPI_BUS_NUM_MAX][2];
/* spi_bus_type became `const struct bus_type` in 6.x.
 * compat_spi_busnum_to_controller (in kernel_compat.h) replaces
 * spi_busnum_to_master after 6.4. */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0)
#include <linux/spi/spi.h>
#define spi_busnum_to_master(num) compat_spi_busnum_to_controller(num)
#else
extern struct bus_type   spi_bus_type;
#endif


#define SPI_MSG_NUM		30
typedef struct hi_spi_message_s
{
	struct spi_transfer	t;
	struct spi_message	m;
	unsigned char	buf[8];
}spi_message_s;

typedef struct hi_spi_message_info_s
{
	int msg_idx;
	spi_message_s spi_msg_array[SPI_MSG_NUM];
}spi_message_info_s;

static spi_message_info_s g_spi_msg = {0};
#endif



#ifdef __HuaweiLite__
extern int hi_spidev_set(int host_no, int cs_no, struct spi_ioc_transfer * transfer);

int ssp_write_alt(unsigned int spi_bus, unsigned int spi_csn,
					unsigned int u32DevAddr, unsigned int u32DevAddrByteNum,
                  unsigned int u32RegAddr, unsigned int u32RegAddrByteNum,
                  unsigned int u32Data , unsigned int u32DataByteNum)
{
    char file_name[0x20];
    unsigned char  buf[0x10];
    struct spi_ioc_transfer transfer[1];
    int fd = -1;
    int retval = 0;
    int index = 0;
    unsigned int dev_addr, reg_addr, data;
    unsigned int dev_width = 1, reg_width = 1, data_width = 1;

    dev_addr = u32DevAddr;
    reg_addr = u32RegAddr;
    data = u32Data;
    dev_width = u32DevAddrByteNum;
    reg_width = u32RegAddrByteNum;
    data_width = u32DataByteNum;

    memset(transfer, 0, sizeof(transfer));
    transfer[0].tx_buf = (char *)buf;
    transfer[0].rx_buf = (char *)buf;
    transfer[0].len = dev_width + reg_width + data_width;

    memset(buf, 0, sizeof(buf));
    if (strcmp(sensor, "imx117"))
    {
        buf[index++] = dev_addr & (~0x80);
    }
    else
    {
        if(dev_width == 2) 
		{
            buf[index++] = dev_addr >> 8;
		}
        buf[index++] = dev_addr & 0xFF;
    }

    if(reg_width == 2) 
	{
        buf[index++] = reg_addr >> 8;
	} 
    buf[index++] = reg_addr & 0xFF;

    if(data_width == 2) 
	{
        buf[index++] = data >>8;
	}
    buf[index++] = data & 0xFF;

	bus_num = spi_bus;
	csn = spi_csn;
    retval = hi_spidev_set(bus_num, csn, &transfer[0]);
    if (retval != transfer[0].len)
	{
		printf("SPI_IOC_MESSAGE error \n");
		retval = -1;
	}
    


	return retval;
}

#else


void reverse8(unsigned char *buf, unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++) 
    {
        buf[i] = (buf[i] & 0x55) << 1 | (buf[i] & 0xAA) >> 1;
        buf[i] = (buf[i] & 0x33) << 2 | (buf[i] & 0xCC) >> 2;
        buf[i] = (buf[i] & 0x0F) << 4 | (buf[i] & 0xF0) >> 4;
    }
}

/*****************************************************************
This function will be called in interrupt route.
So use spi_async, can't call spi_sync here.
*****************************************************************/
int ssp_write_alt(unsigned int spi_bus, unsigned int spi_csn,
					unsigned int addr1, unsigned int addr1bytenum,
   			      unsigned int addr2, unsigned int addr2bytenum,
   			      unsigned int data , unsigned int databytenum)
{
	struct spi_master	*master = hi_master[spi_bus];
	struct spi_device	*spi = hi_spi[spi_bus][spi_csn];
	struct spi_transfer *t;
	struct spi_message	*m;
	unsigned char		*buf;
	int 				status = 0;
	unsigned long		flags;
    int                 buf_idx = 0;
	int idx = g_spi_msg.msg_idx;

	g_spi_msg.msg_idx++;
	if (g_spi_msg.msg_idx > SPI_MSG_NUM - 1)
	{
		g_spi_msg.msg_idx = 0;
	}
	
	buf = g_spi_msg.spi_msg_array[idx].buf;
	t	= &g_spi_msg.spi_msg_array[idx].t;
	m	= &g_spi_msg.spi_msg_array[idx].m;
	
	/* check spi_message is or no finish */
	spin_lock_irqsave(&master->queue_lock, flags);
	if (m->state != NULL)
	{
		spin_unlock_irqrestore(&master->queue_lock, flags);
		dev_err(&spi->dev, "%s, %s, %d line: spi_message no finish!\n", __FILE__, __func__, __LINE__);
		return -EFAULT;
	}
	spin_unlock_irqrestore(&master->queue_lock, flags);

	spi->mode = SPI_MODE_3 | SPI_LSB_FIRST;
	
	memset(buf, 0, sizeof(g_spi_msg.spi_msg_array[idx].buf));

    if (strcmp(sensor, "imx117"))
    {
    	buf[buf_idx++] = addr1 & (~0x80);
    }
    else
    {
        /* imx117 has different dev addr format */
        buf[buf_idx++] = addr1;
    }

    if (2 == addr2bytenum)
    {
    	buf[buf_idx++] = addr2 >> 8;
    }
    buf[buf_idx++] = addr2;

    if (2 == databytenum)
    {
        buf[buf_idx++] = data >> 8;
    }
	buf[buf_idx++] = data;

	t->tx_buf	 = buf;
	t->rx_buf	 = buf;
	t->len		 = buf_idx;
	t->cs_change = 1;
	t->speed_hz  = 2000000;
	t->bits_per_word = 8;

	spi_message_init(m);
	spi_message_add_tail(t, m);
	m->state = m;
	status = spi_async(spi, m);
	if (status)
	{
		dev_err(&spi->dev, "%s: spi_async() error!\n", __func__);
		status = -EFAULT;
	}

	return status;
}

#endif 

HI_S32 hi_ssp_write(	unsigned int spi_bus, unsigned int spi_csn,
				unsigned int addr1, unsigned int addr1bytenum,
 			     unsigned int addr2, unsigned int addr2bytenum,
 			     unsigned int data ,unsigned int databytenum)
{
	if ((addr1bytenum > 1) || (addr2bytenum > 2) || (databytenum > 2))
	{
		printk("addr1_num: %d, addr2_num: %d, data_num: %d, bit_width not support now.\n", 
			   addr1bytenum, addr2bytenum, databytenum);
		return -1;
	}

	if(spi_bus>=SPI_BUS_NUM_MAX)
	{
		printk("spi_bus: %d,spi bus num can't be larger than %d\n", 
			   spi_bus, SPI_BUS_NUM_MAX);
		return -1;
	}
	if(((0 == spi_bus ) ||(2 == spi_bus ))&&(0 != spi_csn))
	{
		printk("spi_bus: %d,spi_csn must be 0\n", spi_bus);
		return -1;
	}
	if((1 == spi_bus ) && (spi_csn > 1))
	{
		printk("spi_bus: %d,spi_csn be smaller than 1\n", spi_bus);
		return -1;
	}
	#if 0
	printk("addr1: 0x%x, addr1_num: %d, addr2: 0x%x, addr2_num: %d, data: 0x%x, data_num: %d.\n", 
			   addr1, addr1bytenum, addr2, addr2bytenum, data, databytenum);
	#endif
    return ssp_write_alt(spi_bus, spi_csn, addr1, addr1bytenum, addr2, addr2bytenum, data, databytenum);
}


#ifndef __HuaweiLite__
/*****************************************************************
This function can't be called in interrupt route because spi_sync will
schedule out.
*****************************************************************/
int ssp_read_alt(unsigned int spi_bus, unsigned int spi_csn,
					unsigned char devaddr, unsigned char addr, unsigned char *data)
{
	struct spi_master 	        *master = hi_master[spi_bus];
	struct spi_device           *spi = hi_spi[spi_bus][spi_csn];
	int			                status = 0;
	unsigned long		        flags;
	static struct spi_transfer 	t;
	static struct spi_message	m;
	static unsigned char        buf[8];
    int                         buf_idx = 0;
	
	/* check spi_message is or no finish */
	spin_lock_irqsave(&master->queue_lock, flags);
	if (m.state != NULL)
	{
		spin_unlock_irqrestore(&master->queue_lock, flags);
		dev_err(&spi->dev, "\n**********%s, %s, %d line: spi_message no finish!*********\n", __FILE__, __func__, __LINE__);
		return -EFAULT;
	}
	spin_unlock_irqrestore(&master->queue_lock, flags);
	
	spi->mode = SPI_MODE_3 | SPI_LSB_FIRST;
	
	memset(buf, 0, sizeof(buf));
	buf[buf_idx++] = devaddr | 0x80;

	buf[buf_idx++] = addr;
	buf[buf_idx++] = 0;

	t.tx_buf    = buf;
	t.rx_buf    = buf;
	t.len       = buf_idx;
	t.cs_change = 1;
	t.speed_hz  = 2000000;
	t.bits_per_word = 8;

	spi_message_init(&m);
	spi_message_add_tail(&t, &m);
	m.state = &m;
	status  = spi_sync(spi, &m);
	if (status)
	{
		dev_err(&spi->dev, "%s: spi_async() error!\n", __func__);
		status = -EFAULT;
	}
	
	*data = buf[2];
	
	printk("func:%s rx_buf = %#x, %#x, %#x\n", __func__, buf[0], buf[1], buf[2]);
	return status;
}

#if 0
static void ssp_test(void)
{
	unsigned char data;
	ssp_write_alt(0x2, 1, 0x14, 1, 0x34, 1);
	// wait spi write finish
	msleep(1);
	ssp_read_alt(0x2, 0x14, &data);
}
#endif
#endif

#ifdef __HuaweiLite__
int sensor_spi_dev_init(void *pArgs)
{
    ISP_BUS_CALLBACK_S stBusCb = {0};

    SPI_MODULE_PARAMS_S* pstSpiModuleParam = (SPI_MODULE_PARAMS_S*)pArgs;

    if (pstSpiModuleParam != NULL)
    {
        bus_num = pstSpiModuleParam->u32BusNum;
        csn = pstSpiModuleParam->u32csn;
        memcpy(sensor,pstSpiModuleParam->cSensor, sizeof(pstSpiModuleParam->cSensor));
    }
    stBusCb.pfnISPWriteSSPData = hi_ssp_write;
    if (CKFN_ISP_RegisterBusCallBack())
    {
        CALL_ISP_RegisterBusCallBack(0, ISP_BUS_TYPE_SSP, &stBusCb);
		CALL_ISP_RegisterBusCallBack(1, ISP_BUS_TYPE_SSP, &stBusCb);
    }
    else
    {
        printk("register ssp_write_callback to isp failed, ssp init is failed!\n");
        return -1;
    }

    return 0;
}

#else
static int __init sensor_spi_dev_init(void)
{
	int 			    status = 0;
	struct spi_master 	*master;
	struct device		*dev = HI_NULL;
	char 			    spi_name[128] = {0};
	int bus_num = 0;	
	int csn = 0;

    ISP_BUS_CALLBACK_S stBusCb = {0};
    stBusCb.pfnISPWriteSSPData = hi_ssp_write;
    if ((NULL != FUNC_ENTRY(ISP_EXPORT_FUNC_S, HI_ID_ISP)) && (CKFN_ISP_RegisterBusCallBack()))
    {
        CALL_ISP_RegisterBusCallBack(0, ISP_BUS_TYPE_SSP, &stBusCb);
        CALL_ISP_RegisterBusCallBack(1, ISP_BUS_TYPE_SSP, &stBusCb);
    }
    else
    {
        printk("register ssp_write_callback to isp failed, ssp init is failed!\n");
        return -1;
    }
	for(bus_num = 0;bus_num<SPI_BUS_NUM_MAX; bus_num++)
	{		
	    master = spi_busnum_to_master(bus_num);
		for(csn = 0; csn<spi_csn[bus_num]; csn++)
		{			
        	if(master)
        	{
        		hi_master[bus_num] = master;
                snprintf(spi_name, sizeof(spi_name), "%s.%u", dev_name(&master->dev), csn);
        		dev = bus_find_device_by_name(&spi_bus_type, NULL, spi_name);
        		if (dev == NULL)
        		{
            		dev_err(NULL, "chipselect %d has not been used\n", csn);
            		status = -ENXIO;
            		goto end1;
        		}
        		
        		hi_spi[bus_num][csn] = to_spi_device(dev);
        		if(hi_spi[bus_num][csn] == NULL)
        		{
        			dev_err(dev, "to_spi_device() error!\n");
        			status = -ENXIO;
        			goto end1;
        		}
        	}
        	else
        	{
        		dev_err(NULL, "spi_busnum_to_master() error!\n");
        		status = -ENXIO;
        		goto end0;
			}
		}
	}

    printk(KERN_INFO "load hi_sensor_spi.ko ...OK!\n");
	//ssp_test();
end1:
	put_device(dev);
end0:
	return status;
}
#endif

#ifndef __HuaweiLite__
static void __exit sensor_spi_dev_exit(void)
{
    printk(KERN_INFO "unload hi_sensor_spi.ko ...OK!\n");
}
#endif

module_init(sensor_spi_dev_init);
module_exit(sensor_spi_dev_exit);

MODULE_AUTHOR("BVT OSDRV");
MODULE_LICENSE("GPL");
#ifndef __HuaweiLite__
MODULE_ALIAS("sensor spidev");
#endif

