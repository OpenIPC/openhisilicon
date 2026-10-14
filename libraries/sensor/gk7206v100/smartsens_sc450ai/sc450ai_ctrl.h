#ifndef __SC450AI_CTRL_H__
#define __SC450AI_CTRL_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SC450AI_I2C_ADDR  0x60
#define SC450AI_ADDR_BYTE 2
#define SC450AI_DATA_BYTE 1

// sc450ai specs list
#define SC450AI_4M_1440_10BIT_LINEAR_MODE  1
#define SC450AI_4M_1440_10BIT_WDR_MODE     2
#define SC450AI_4M_1520_10BIT_LINEAR_MODE  3
#define SC450AI_1M_760_10BIT_LINEAR_MODE   4

xmedia_s32 sc450ai_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr);
xmedia_s32 sc450ai_i2c_exit(xmedia_u32 dev);
xmedia_s32 sc450ai_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data);
xmedia_s32 sc450ai_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data);
xmedia_s32 sc450ai_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en);
xmedia_s32 sc450ai_init_image(xmedia_u32 dev, xmedia_u8 img_mode);
xmedia_s32 sc450ai_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr);

#ifdef __cplusplus
}
#endif

#endif //__SC450AI_CTRL_H__
