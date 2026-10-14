#ifndef __SC4336P_CTRL_H__
#define __SC4336P_CTRL_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SC4336P_I2C_ADDR  0x60
#define SC4336P_ADDR_BYTE 2
#define SC4336P_DATA_BYTE 1

// sc4336p specs list
#define SC4336P_4M_30FPS_10BIT_LINEAR_MODE  1 // 2560x1440x30fps

xmedia_s32 sc4336p_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr);
xmedia_s32 sc4336p_i2c_exit(xmedia_u32 dev);
xmedia_s32 sc4336p_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data);
xmedia_s32 sc4336p_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data);
xmedia_s32 sc4336p_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en);
xmedia_s32 sc4336p_init_image(xmedia_u32 dev, xmedia_u8 image_mode, xmedia_sensor_work_mode work_mode);
xmedia_s32 sc4336p_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr);

#ifdef __cplusplus
}
#endif

#endif //__SC4336P_CTRL_H__
