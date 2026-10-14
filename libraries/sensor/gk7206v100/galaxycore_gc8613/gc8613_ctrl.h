#ifndef __GC8613_CTRL_H__
#define __GC8613_CTRL_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

// I2C_ID1: 0x62; I2C_ID2: 0x20
#define GC8613_I2C_ADDR  0x62
#define GC8613_ADDR_BYTE 2
#define GC8613_DATA_BYTE 1

// gc8613 specs list
typedef enum {
    GC8613_4L_8M_LINEAR_MODE = 1,   // 3840x2160
    GC8613_2L_8M_LINEAR_MODE,
} gc8613_specs;


xmedia_s32 gc8613_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr);
xmedia_s32 gc8613_i2c_exit(xmedia_u32 dev);
xmedia_s32 gc8613_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data);
xmedia_s32 gc8613_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data);
xmedia_s32 gc8613_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en);
xmedia_s32 gc8613_init_image(xmedia_u32 dev, xmedia_u8 img_mode);
xmedia_s32 gc8613_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr);

#ifdef __cplusplus
}
#endif

#endif //__GC8613_CTRL_H__
