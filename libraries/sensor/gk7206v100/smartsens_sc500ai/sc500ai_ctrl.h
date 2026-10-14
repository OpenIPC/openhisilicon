#ifndef __SC500AI_CTRL_H__
#define __SC500AI_CTRL_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SC500AI_I2C_ADDR  0x60
#define SC500AI_ADDR_BYTE 2
#define SC500AI_DATA_BYTE 1

// sc500ai specs list
#define SC500AI_5M_10BIT_LINEAR_MODE 1 // 2880x1620x30fps
#define SC500AI_5M_10BIT_WDR_MODE    2 // 2880x1620x30fps
#define SC500AI_640_480_10BIT_LINEAR_MODE 3 // 640x480x7fps

xmedia_s32 sc500ai_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr);
xmedia_s32 sc500ai_i2c_exit(xmedia_u32 dev);
xmedia_s32 sc500ai_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data);
xmedia_s32 sc500ai_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data);
xmedia_s32 sc500ai_init_image(xmedia_u32 dev, xmedia_u8 image_mode, xmedia_sensor_work_mode work_mode);
xmedia_s32 sc500ai_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en);
xmedia_s32 sc500ai_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr);

#ifdef __cplusplus
}
#endif

#endif //__SC500AI_CTRL_H__
