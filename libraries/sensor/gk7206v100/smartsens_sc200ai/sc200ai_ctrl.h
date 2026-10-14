#ifndef __SC200AI_CTRL_H__
#define __SC200AI_CTRL_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SC200AI_I2C_ADDR  0x60
#define SC200AI_ADDR_BYTE 2
#define SC200AI_DATA_BYTE 1

// sc200ai specs list
#define SC200AI_1M_20FPS_10BIT_LINEAR_MODE 1 // 1280x720x21.548fps
#define SC200AI_1M_6FPS_10BIT_WDR_MODE     2 // 1280x720x6.459fps
#define SC200AI_2M_12FPS_10BIT_LINEAR_MODE 3 // 1920x1080x12.929fps
#define SC200AI_2M_6FPS_10BIT_WDR_MODE     4 // 1920x1080x6.459fps

xmedia_s32 sc200ai_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr);
xmedia_s32 sc200ai_i2c_exit(xmedia_u32 dev);
xmedia_s32 sc200ai_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data);
xmedia_s32 sc200ai_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data);
xmedia_s32 sc200ai_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en);
xmedia_s32 sc200ai_init_image(xmedia_u32 dev, xmedia_u8 img_mode);
xmedia_s32 sc200ai_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr);

#ifdef __cplusplus
}
#endif

#endif //__SC200AI_CTRL_H__
