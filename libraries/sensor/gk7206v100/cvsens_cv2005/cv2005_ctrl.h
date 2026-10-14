#ifndef __CV2005_CTRL_H__
#define __CV2005_CTRL_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CV2005_I2C_ADDR  0x6a
#define CV2005_ADDR_BYTE 2
#define CV2005_DATA_BYTE 1

// cv2005 specs list
#define CV2005_2LANE_1920X1080_LINEAR_MODE 1

xmedia_s32 cv2005_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr);
xmedia_s32 cv2005_i2c_exit(xmedia_u32 dev);
xmedia_s32 cv2005_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data);
xmedia_s32 cv2005_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data);
xmedia_s32 cv2005_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en);
xmedia_s32 cv2005_init_image(xmedia_u32 dev, xmedia_u8 img_mode, xmedia_sensor_work_mode work_mode);
xmedia_s32 cv2005_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr);

#ifdef __cplusplus
}
#endif

#endif //__CV2005_CTRL_H__
