#ifndef __OX05B1S_CTRL_H__
#define __OX05B1S_CTRL_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OX05B1S_I2C_ADDR  0x6c
#define OX05B1S_ADDR_BYTE 2
#define OX05B1S_DATA_BYTE 1

#define OX05B1S_1M_LINEAR_MODE 1
#define OX05B1S_5M_LINEAR_MODE 2

xmedia_s32 ox05b1s_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr);
xmedia_s32 ox05b1s_i2c_exit(xmedia_u32 dev);
xmedia_s32 ox05b1s_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data);
xmedia_s32 ox05b1s_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data);
xmedia_s32 ox05b1s_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en);
xmedia_s32 ox05b1s_init_image(xmedia_u32 dev, xmedia_u8 img_mode, xmedia_sensor_work_mode work_mode);
xmedia_s32 ox05b1s_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr);

#ifdef __cplusplus
}
#endif

#endif //__OX05B1S_CTRL_H__
