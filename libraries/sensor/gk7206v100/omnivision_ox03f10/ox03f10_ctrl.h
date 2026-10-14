#ifndef __OX03F10_CTRL_H__
#define __OX03F10_CTRL_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OX03F10_I2C_ADDR  0x6c
#define OX03F10_ADDR_BYTE 2
#define OX03F10_DATA_BYTE 1

#define OX03F10_SENSOR_1M_BUILT_IN_MODE 1
#define OX03F10_SENSOR_2M_BUILT_IN_MODE 2
#define OX03F10_SENSOR_3M_BUILT_IN_MODE 3
#define OX03F10_SENSOR_3M_LINEAR_MODE   4

xmedia_s32 ox03f10_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr);
xmedia_s32 ox03f10_i2c_exit(xmedia_u32 dev);
xmedia_s32 ox03f10_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data);
xmedia_s32 ox03f10_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data);
xmedia_s32 ox03f10_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en);
xmedia_s32 ox03f10_init_image(xmedia_u32 dev, xmedia_u8 img_mode, xmedia_sensor_work_mode work_mode,
                            xmedia_bool mirror_en, xmedia_bool flip_en);
xmedia_s32 ox03f10_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr);

#ifdef __cplusplus
}
#endif

#endif //__OX03F10_CTRL_H__
