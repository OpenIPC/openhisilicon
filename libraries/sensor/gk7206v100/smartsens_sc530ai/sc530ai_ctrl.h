#ifndef __SC530AI_CTRL_H__
#define __SC530AI_CTRL_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SC530AI_I2C_ADDR  0x60
#define SC530AI_ADDR_BYTE 2
#define SC530AI_DATA_BYTE 1

// sc530ai specs list
#define SC530AI_5M_30FPS_10BIT_LINEAR_MODE    1 // 2880x1620x30fps
#define SC530AI_5M_30FPS_10BIT_WDR_MODE       2 // 2880x1620x30fps
#define SC530AI_2L_5M_25FPS_10BIT_LINEAR_MODE 3 // 2880x1620x25fps

xmedia_s32 sc530ai_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr);
xmedia_s32 sc530ai_i2c_exit(xmedia_u32 dev);
xmedia_s32 sc530ai_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data);
xmedia_s32 sc530ai_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data);
xmedia_s32 sc530ai_init_image(xmedia_u32 dev, xmedia_u8 img_mode, xmedia_sensor_work_mode work_mode);
xmedia_s32 sc530ai_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en);
xmedia_s32 sc530ai_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr);

#ifdef __cplusplus
}
#endif

#endif // __SC530AI_CTRL_H__
