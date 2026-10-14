/*
 * Copyright (c); XMEDIA. All rights reserved.
 */

#ifndef __MSENSOR_SPI_H__
#define __MSENSOR_SPI_H__

#include "xmedia_type.h"

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#define SSP_READ_ALT  0x1
#define SSP_WRITE_ALT 0X3

typedef struct {
    xmedia_u32 spi_no;
    xmedia_u8 dev_addr;
    xmedia_u32 dev_byte_num;
    xmedia_u32 reg_addr;
    xmedia_u32 addr_byte_num;
    xmedia_u32 data;
    xmedia_u32 data_byte_num;
} spi_data;

xmedia_s32 xmedia_msensor_ssp_write_alt(xmedia_s32 dev, xmedia_u8 reg_addr, const xmedia_u8 *data);
xmedia_u16 xmedia_msensor_ssp_read_alt(xmedia_s32 dev, xmedia_u8 reg_addr,
    xmedia_u8 *reg_data, xmedia_u32 cnt, xmedia_bool fifo_mode);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif

