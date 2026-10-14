#ifndef __IMX307_CTRL_H__
#define __IMX307_CTRL_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IMX307_I2C_ADDR  0x34 // I2C Address of IMX307
#define IMX307_ADDR_BYTE 2
#define IMX307_DATA_BYTE 1

// imx307 specs list
#define IMX307_2M_2L_10BIT_LINEAR_MODE   1
#define IMX307_2M_4L_10BIT_LINEAR_MODE   2
#define IMX307_2M_2L_10BIT_2TO1_WDR_MODE 3
#define IMX307_2M_4L_10BIT_2TO1_WDR_MODE 4

#define SENSOR_HIGHER_16BITS(x) (((x)&0x30000) >> 16)

xmedia_s32 imx307_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr);
xmedia_s32 imx307_i2c_exit(xmedia_u32 dev);
xmedia_s32 imx307_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data);
xmedia_s32 imx307_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data);
xmedia_s32 imx307_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en);
xmedia_s32 imx307_init_image(xmedia_u32 dev, xmedia_u8 img_mode);
xmedia_s32 imx307_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr);
xmedia_void imx307_delay_ms(xmedia_s32 ms);

#ifdef __cplusplus
}
#endif

#endif //__SC8238_CTRL_H__
