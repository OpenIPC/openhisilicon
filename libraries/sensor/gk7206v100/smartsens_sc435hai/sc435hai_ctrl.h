#ifndef __SC435HAI_CTRL_H__
#define __SC435HAI_CTRL_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SC435HAI_I2C_ADDR  0x60
#define SC435HAI_ADDR_BYTE 2
#define SC435HAI_DATA_BYTE 1

// sc435hai specs list
#define SC435HAI_4M_1440_10BIT_LINEAR_MODE  1

xmedia_s32 sc435hai_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr);
xmedia_s32 sc435hai_i2c_exit(xmedia_u32 dev);
xmedia_s32 sc435hai_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data);
xmedia_s32 sc435hai_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data);
xmedia_s32 sc435hai_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en);
xmedia_s32 sc435hai_init_image(xmedia_u32 dev, xmedia_u8 image_mode, xmedia_sensor_work_mode work_mode);
xmedia_s32 sc435hai_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr);
xmedia_s32 sc435hai_set_stream_mode(xmedia_u32 dev, xmedia_bool stream_en);

#ifdef __cplusplus
}
#endif

#endif //__SC435HAI_CTRL_H__
