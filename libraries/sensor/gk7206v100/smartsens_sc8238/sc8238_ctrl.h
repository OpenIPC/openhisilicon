#ifndef __SC8238_CTRL_H__
#define __SC8238_CTRL_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SC8238_I2C_ADDR  0x60
#define SC8238_ADDR_BYTE 2
#define SC8238_DATA_BYTE 1

// sc8238 specs list
typedef enum {
    SC8238_8M_10BIT_LINEAR_MODE = 1,
    SC8238_5M_10BIT_LINEAR_MODE,
    SC8238_4M_10BIT_LINEAR_MODE,
    SC8238_3M_10BIT_LINEAR_MODE,
    SC8238_2M_10BIT_LINEAR_MODE,
    SC8238_1M_10BIT_LINEAR_MODE,
    SC8238_5M_10BIT_1728P_LINEAR_MODE,

    SC8238_8M_10BIT_WDR_MODE,
    SC8238_5M_10BIT_WDR_MODE,
    SC8238_4M_10BIT_WDR_MODE,
    SC8238_3M_10BIT_WDR_MODE,
    SC8238_2M_10BIT_WDR_MODE,
    SC8238_1M_10BIT_WDR_MODE,
} sc8238_specs;

xmedia_s32 sc8238_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr);
xmedia_s32 sc8238_i2c_exit(xmedia_u32 dev);
xmedia_s32 sc8238_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data);
xmedia_s32 sc8238_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data);
xmedia_s32 sc8238_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en);
xmedia_s32 sc8238_init_image(xmedia_u32 dev, xmedia_u8 img_mode);
xmedia_s32 sc8238_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr);

#ifdef __cplusplus
}
#endif

#endif //__SC8238_CTRL_H__
