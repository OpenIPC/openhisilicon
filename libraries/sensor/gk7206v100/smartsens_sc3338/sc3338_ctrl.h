#ifndef __SC3338_CTRL_H__
#define __SC3338_CTRL_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SC3338_I2C_ADDR  0x60
#define SC3338_ADDR_BYTE 2
#define SC3338_DATA_BYTE 1

#define SC3338_3M_10BIT_LINEAR_MODE  1

xmedia_s32 sc3338_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr);
xmedia_s32 sc3338_i2c_exit(xmedia_u32 dev);
xmedia_s32 sc3338_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data);
xmedia_s32 sc3338_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data);
xmedia_s32 sc3338_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en);
xmedia_s32 sc3338_init_image(xmedia_u32 dev, xmedia_u8 img_mode);
xmedia_s32 sc3338_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr);

#ifdef __cplusplus
}
#endif

#endif //__SC3338_CTRL_H__
