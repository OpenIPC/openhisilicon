#ifndef __GC4663_CTRL_H__
#define __GC4663_CTRL_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GC4663_I2C_ADDR  0x52
#define GC4663_ADDR_BYTE 2
#define GC4663_DATA_BYTE 1

// gc4663 specs list
#define GC4663_4M_10BIT_LINEAR_MODE  1 // 2560x1440x30fps
#define GC4663_4M_10BIT_WDR_MODE     2 // 2560x1440x25fps

xmedia_s32 gc4663_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr);
xmedia_s32 gc4663_i2c_exit(xmedia_u32 dev);
xmedia_s32 gc4663_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data);
xmedia_s32 gc4663_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data);
xmedia_s32 gc4663_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en);
xmedia_s32 gc4663_init_image(xmedia_u32 dev, xmedia_u8 img_mode);
xmedia_s32 gc4663_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr);

#ifdef __cplusplus
}
#endif

#endif //__GC4663_CTRL_H__
