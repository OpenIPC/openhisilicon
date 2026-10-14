#ifndef __GC6603_CTRL_H__
#define __GC6603_CTRL_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

// I2C_ID1: 0x62; I2C_ID2: 0x20
#define GC6603_I2C_ADDR  0x62
#define GC6603_ADDR_BYTE 2
#define GC6603_DATA_BYTE 1

#define GC6603_5M_10BIT_LINEAR_MODE  1

xmedia_s32 gc6603_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr);
xmedia_s32 gc6603_i2c_exit(xmedia_u32 dev);
xmedia_s32 gc6603_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data);
xmedia_s32 gc6603_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data);
xmedia_s32 gc6603_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en);
xmedia_s32 gc6603_init_image(xmedia_u32 dev, xmedia_u8 img_mode);
xmedia_s32 gc6603_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr);
xmedia_s32 gc6603_set_stream_mode(xmedia_u32 dev, xmedia_bool stream_en);
xmedia_void gc6603_delay_ms(xmedia_s32 ms);

#ifdef __cplusplus
}
#endif

#endif //__GC6603_CTRL_H__
