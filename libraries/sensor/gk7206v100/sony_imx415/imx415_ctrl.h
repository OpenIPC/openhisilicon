#ifndef __IMX415_CTRL_H__
#define __IMX415_CTRL_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IMX415_I2C_ADDR   0x34
#define IMX415_ADDR_BYTE  2
#define IMX415_DATA_BYTE  1

// imx415 specs list
#define IMX415_8M_10BIT_LINEAR_MODE   1
#define IMX415_8M_10BIT_WDR_MODE      2

#define SENSOR_HIGHER_16BITS(x) (((x)&0x0F0000) >> 16)

xmedia_s32 imx415_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr);
xmedia_s32 imx415_i2c_exit(xmedia_u32 dev);
xmedia_s32 imx415_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data);
xmedia_s32 imx415_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data);
xmedia_s32 imx415_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en);
xmedia_s32 imx415_init_image(xmedia_u32 dev, xmedia_u8 img_mode);
xmedia_s32 imx415_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr);
xmedia_void imx415_delay_ms(xmedia_s32 ms);

#ifdef __cplusplus
}
#endif

#endif
