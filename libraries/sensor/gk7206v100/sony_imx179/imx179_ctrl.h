#ifndef __IMX179_CTRL_H__
#define __IMX179_CTRL_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IMX179_I2C_ADDR   0x20
#define IMX179_ADDR_BYTE  2
#define IMX179_DATA_BYTE  1

// imx179 specs list
typedef enum {
    IMX179_1M_10BIT_LINEAR_MODE = 1,
    IMX179_8M_10BIT_LINEAR_MODE,
} imx179_specs;

xmedia_s32 imx179_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr);
xmedia_s32 imx179_i2c_exit(xmedia_u32 dev);
xmedia_s32 imx179_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data);
xmedia_s32 imx179_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data);
xmedia_s32 imx179_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en);
xmedia_s32 imx179_init_image(xmedia_u32 dev, xmedia_u8 img_mode);
xmedia_s32 imx179_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr);
xmedia_void imx179_delay_ms(xmedia_s32 ms);

#ifdef __cplusplus
}
#endif

#endif
