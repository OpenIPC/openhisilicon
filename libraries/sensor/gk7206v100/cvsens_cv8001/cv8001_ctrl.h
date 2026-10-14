#ifndef __CV8001_CTRL_H__
#define __CV8001_CTRL_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CV8001_I2C_ADDR  0x6a
#define CV8001_ADDR_BYTE 2
#define CV8001_DATA_BYTE 1

// cv8001 specs list
typedef enum {
    CV8001_8M_LINEAR_MODE = 1,   // 3840x2160
} cv8001_specs;

xmedia_s32 cv8001_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr);
xmedia_s32 cv8001_i2c_exit(xmedia_u32 dev);
xmedia_s32 cv8001_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data);
xmedia_s32 cv8001_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data);
xmedia_s32 cv8001_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en);
xmedia_s32 cv8001_init_image(xmedia_u32 dev, xmedia_u8 img_mode);
xmedia_s32 cv8001_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr);

#ifdef __cplusplus
}
#endif

#endif //__CV8001_CTRL_H__
