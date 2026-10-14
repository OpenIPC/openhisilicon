#ifndef __CV4003_CTRL_H__
#define __CV4003_CTRL_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CV4003_I2C_ADDR  0x6a
#define CV4003_ADDR_BYTE 2
#define CV4003_DATA_BYTE 1

// cv4003 specs list
typedef enum {
    CV4003_4M_LINEAR_MODE = 1,   // 2560x1440
    CV4003_1M_LINEAR_MODE = 2,   // 1280x720
} cv4003_specs;

//#define USE_PREROLL_INFO

xmedia_s32 cv4003_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr);
xmedia_s32 cv4003_i2c_exit(xmedia_u32 dev);
xmedia_s32 cv4003_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data);
xmedia_s32 cv4003_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data);
xmedia_s32 cv4003_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en);
xmedia_s32 cv4003_init_image(xmedia_u32 dev, xmedia_u8 img_mode);
xmedia_s32 cv4003_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr);

#ifdef USE_PREROLL_INFO
xmedia_s32 cv4003_preroll_720p_write_cis2psram_init(xmedia_u32 dev);
xmedia_s32 cv4003_get_preroll_psram_final_addr(xmedia_u32 dev);
#endif

#ifdef __cplusplus
}
#endif

#endif //__CV4003_CTRL_H__
