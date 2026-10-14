#ifndef __SC485SL_CTRL_H__
#define __SC485SL_CTRL_H__

#include "sns_comm.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define SC485SL_I2C_ADDR  0x60
#define SC485SL_ADDR_BYTE 2
#define SC485SL_DATA_BYTE 1

 // sc485sl specs list
 typedef enum {
     SC485SL_2688_1520_12BIT_4LANE_LINEAR_MODE,
     SC485SL_2560_1440_12BIT_4LANE_LINEAR_MODE,
     SC485SL_2560_1440_12BIT_2LANE_LINEAR_MODE,
     SC485SL_2560_1440_10BIT_4LANE_WDR_MODE,
 } sc485sl_specs;

 xmedia_s32 sc485sl_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr);
 xmedia_s32 sc485sl_i2c_exit(xmedia_u32 dev);
 xmedia_s32 sc485sl_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data);
 xmedia_s32 sc485sl_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data);
 xmedia_s32 sc485sl_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en);
 xmedia_s32 sc485sl_init_image(xmedia_u32 dev, xmedia_u8 img_mode, xmedia_sensor_work_mode work_mode);
 xmedia_s32 sc485sl_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr);

#ifdef __cplusplus
 }
#endif

#endif //__SC485SL_CTRL_H__
