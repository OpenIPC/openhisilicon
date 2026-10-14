/*
 * Copyright (c); XMEDIA. All rights reserved.
 */

#ifndef __SPI_DEV_H__
#define __SPI_DEV_H__

#include "xmedia_type.h"
#include "osal.h"
#include "xmedia_debug.h"
#include <linux/spi/spi.h>

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

xmedia_s32 msensor_spi_write(struct spi_device *xmedia_spi, xmedia_u8 addr, const xmedia_u8 *data, xmedia_u32 cnt);
xmedia_s32 msensor_spi_read(struct spi_device *xmedia_spi, xmedia_u8 addr, xmedia_u8 *data, xmedia_u32 cnt);
xmedia_s32 msensor_spi_init(struct spi_device **xmedia_spi);
xmedia_s32 msensor_spi_deinit(const struct spi_device *spi_device);

#ifndef MSENSOR_ERR_TRACE
#define MSENSOR_ERR_TRACE(fmt, ...)                                                                              \
    printk(KERN_ERR "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, \
                 ##__VA_ARGS__)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif

