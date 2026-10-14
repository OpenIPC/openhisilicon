#ifndef __GC20B3_CTRL_H__
#define __GC20B3_CTRL_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

// I2C_ID1: 0x62; I2C_ID2: 0x20
#define GC20B3_I2C_ADDR  0x6e
#define GC20B3_ADDR_BYTE 2
#define GC20B3_DATA_BYTE 1

#define GC20B3_2M_10BIT_LINEAR_MODE  1

xmedia_s32 gc20b3_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr);
xmedia_s32 gc20b3_i2c_exit(xmedia_u32 dev);
xmedia_s32 gc20b3_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data);
xmedia_s32 gc20b3_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data);
xmedia_s32 gc20b3_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en);
xmedia_s32 gc20b3_init_image(xmedia_u32 dev, xmedia_u8 img_mode);
xmedia_s32 gc20b3_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr);

#ifdef __cplusplus
}
#endif

#endif //__GC20B3_CTRL_H__
