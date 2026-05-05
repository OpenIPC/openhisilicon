/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of sensor_spi.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __SENSOR_SPI_H__
#define __SENSOR_SPI_H__

#define C_SENOSOR_NUM 64
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

#include "../../compat/kernel_compat.h"

typedef struct {
    hi_u32 bus_num;
    hi_u32 u32csn;
    hi_char c_sensor[C_SENOSOR_NUM];
} spi_module_params;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(7, 0, 0)
extern const struct bus_type spi_bus_type;
#else
extern struct bus_type spi_bus_type;
#endif

#ifdef __HuaweiLite__
extern int spi_dev_set(int host_no, int cs_no, struct spi_ioc_transfer *transfer);   /* hi_spidev_set */
#endif

int ssp_write_alt(unsigned int spi_bus, unsigned int spi_csn,
                  unsigned int addr1, unsigned int addr1bytenum,
                  unsigned int addr2, unsigned int addr2bytenum,
                  unsigned int data, unsigned int databytenum);

int ssp_read_alt(unsigned int spi_bus, unsigned int spi_csn,
                 unsigned char devaddr, unsigned char addr, unsigned char *data);
#endif
