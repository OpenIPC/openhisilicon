/*
 * Copyright (c); XMEDIA. All rights reserved.
 */

#ifndef __I2C_DEV_H__
#define __I2C_DEV_H__

#include <linux/i2c.h>
#include "xmedia_type.h"

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

xmedia_u8 msensor_i2c_write(struct i2c_client *xmedia_i2c_client,
    xmedia_u8 reg_addr, const xmedia_u8 *reg_data,xmedia_u32 cnt);
xmedia_u8 msensor_i2c_read(struct i2c_client *xmedia_i2c_client,
    xmedia_u8 reg_addr, xmedia_u8 *reg_data, xmedia_u32 cnt);
xmedia_u32 msensor_i2c_get_frequency(struct i2c_client *xmedia_i2c_client, xmedia_s32 *frequency);
xmedia_u32 msensor_i2c_init(struct i2c_client **xmedia_i2c_client,
    struct i2c_board_info xmedia_i2c_board_info, xmedia_s32 adapt_num);

xmedia_void msensor_i2c_exit(struct i2c_client **xmedia_i2c_client);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif

