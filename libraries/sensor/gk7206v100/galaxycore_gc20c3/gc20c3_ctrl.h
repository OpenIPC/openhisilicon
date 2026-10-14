#ifndef __GC20C3_CTRL_H__
#define __GC20C3_CTRL_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GC20C3_I2C_ADDR  0x62
#define GC20C3_ADDR_BYTE 2
#define GC20C3_DATA_BYTE 1

#define GC20C3_2LANE_LINEAR_1920X1080_30FPS 1 // 0: 使用40fps的序列； 1: 使用30fps的序列

// gc20c3 specs list
#define GC20C3_2M_1920X1080_LINEAR_MODE 1

xmedia_s32 gc20c3_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr);
xmedia_s32 gc20c3_i2c_exit(xmedia_u32 dev);
xmedia_s32 gc20c3_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data);
xmedia_s32 gc20c3_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data);
xmedia_s32 gc20c3_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en);
xmedia_s32 gc20c3_init_image(xmedia_u32 dev, xmedia_u8 img_mode, xmedia_sensor_work_mode work_mode);
xmedia_s32 gc20c3_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr);

#ifdef __cplusplus
}
#endif

#endif //__GC20C3_CTRL_H__
