#ifndef __OS08A20_CTRL_H__
#define __OS08A20_CTRL_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OS08A20_I2C_ADDR  0x6c
#define OS08A20_ADDR_BYTE 2
#define OS08A20_DATA_BYTE 1

// os08a20 specs list
typedef enum {
    OS08A20_8M_LINEAR_MODE = 1,   // 3840x2160
    OS08A20_5M_LINEAR_MODE,       // 2560x1920
    OS08A20_4M_LINEAR_MODE,       // 2688x1520
    OS08A20_4M_1440P_LINEAR_MODE, // 2560x1440
    OS08A20_3M_LINEAR_MODE,       // 2048x1536
    OS08A20_2M_LINEAR_MODE,       // 1920x1080

    OS08A20_8M_WDR_MODE,          // 3840x2160
    OS08A20_5M_WDR_MODE,          // 2560x1920
    OS08A20_4M_WDR_MODE,          // 2560x1920
    OS08A20_4M_1440P_WDR_MODE,    // 2560x1440
    OS08A20_3M_WDR_MODE,          // 2560x1920
    OS08A20_2M_WDR_MODE,          // 1920x1080
} os08a20_specs;

xmedia_s32 os08a20_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr);
xmedia_s32 os08a20_i2c_exit(xmedia_u32 dev);
xmedia_s32 os08a20_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data);
xmedia_s32 os08a20_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data);
xmedia_s32 os08a20_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en);
xmedia_s32 os08a20_init_image(xmedia_u32 dev, xmedia_u8 img_mode);
xmedia_s32 os08a20_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr);

#ifdef __cplusplus
}
#endif

#endif //__OS08A20_CTRL_H__
