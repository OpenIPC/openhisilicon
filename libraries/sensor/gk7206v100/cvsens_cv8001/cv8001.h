#ifndef __CV8001_H__
#define __CV8001_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CV8001_VMAX_8M_LINEAR    4380 // 10bit

#define CV8001_EXP_OFFSET_LINEAR 4

// CV8001 Register Address
#define CV8001_REG_ADDR_EXP_H  0x3060
#define CV8001_REG_ADDR_EXP_M  0x3061
#define CV8001_REG_ADDR_EXP_L  0x3062
#define CV8001_REG_ADDR_GAIN_1 0x3164
#define CV8001_REG_ADDR_VMAX_H 0x3028
#define CV8001_REG_ADDR_VMAX_M 0x3029
#define CV8001_REG_ADDR_VMAX_L 0x302a

typedef enum {
    CV8001_REG_EXP_H_IDX = 0,
    CV8001_REG_EXP_M_IDX,
    CV8001_REG_EXP_L_IDX,
    CV8001_REG_AGAIN_1_IDX,
    CV8001_REG_VMAX_H_IDX,
    CV8001_REG_VMAX_M_IDX,
    CV8001_REG_VMAX_L_IDX,
    CV8001_REG_MAX_NUM,
} cv8001_reg_index;

extern sensor_context *g_cv8001_ctx[XMEDIA_SENSOR_DEV_MAX_NUM];
#define CV8001_GET_CONTEXT(dev, sns_ctx) sns_ctx = g_cv8001_ctx[dev]

xmedia_s32  cv8001_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps);

#ifdef __cplusplus
}
#endif

#endif // __CV8001_H__
