#ifndef __OS04J10_CTRL_H__
#define __OS04J10_CTRL_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OS04J10_I2C_ADDR  0x78
#define OS04J10_ADDR_BYTE 1
#define OS04J10_DATA_BYTE 1

#define OS04J10_4M_LINEAR_MODE  1
#define OS04J10_4M_WDR_MODE     2

xmedia_s32 os04j10_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr);
xmedia_s32 os04j10_i2c_exit(xmedia_u32 dev);
xmedia_s32 os04j10_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data);
xmedia_s32 os04j10_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data);
xmedia_s32 os04j10_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en);
xmedia_s32 os04j10_init_image(xmedia_u32 dev, xmedia_u8 img_mode);
xmedia_s32 os04j10_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr);
void delay_ms(int ms);

#ifdef __cplusplus
}
#endif

#endif //__OS04J10_CTRL_H__
