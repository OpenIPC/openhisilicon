#ifndef __GC2053_CTRL_H__
#define __GC2053_CTRL_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GC2053_I2C_ADDR    0x6e
#define GC2053_ADDR_BYTE   1
#define GC2053_DATA_BYTE   1

//#define GC2053_DVP //DVP模式下需定义此宏，MIPI模式则不需要定义此宏

// gc2053 specs list
#define GC2053_2M_10BIT_LINEAR_MODE     1

xmedia_s32 gc2053_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr);
xmedia_s32 gc2053_i2c_exit(xmedia_u32 dev);
xmedia_s32 gc2053_read_reg(xmedia_u32 dev, xmedia_u32 addr,xmedia_u32 *data);
xmedia_s32 gc2053_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data);
xmedia_s32 gc2053_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en);
xmedia_s32 gc2053_init_image(xmedia_u32 dev, xmedia_u8 img_mode, xmedia_sensor_work_mode work_mode);
xmedia_s32 gc2053_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr);

#ifdef __cplusplus
}
#endif

#endif
