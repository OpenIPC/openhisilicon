#ifndef __SC530AI_H__
#define __SC530AI_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

// sensor fps mode
#define SC530AI_FULL_LINES_MAX            0xFFFF
#define SC530AI_FULL_LINES_MAX_2TO1_WDR   0x2000
#define SC530AI_INCREASE_LINES    0 // make real fps less than stand fps because NVR require
#define SC530AI_VMAX_5M_LINEAR    (1980 + SC530AI_INCREASE_LINES)
#define SC530AI_VMAX_5M_WDR       (3300 + SC530AI_INCREASE_LINES)
#define SC530AI_2L_VMAX_5M_LINEAR (1650 + SC530AI_INCREASE_LINES)
#define SC530AI_FRAME_RATE_MIN    0x4D58
#define SC530AI_SENSOR_FPS_MAX    30

#define SC530AI_EXP_OFFSET_LINEAR 10
#define SC530AI_EXP_OFFSET_WDR    18

#define SC530AI_S_EXP_MAX_DEFAULT_1620P 0xc8

// the value of the sc530ai exposure register is in half line units, (<< 1) for AE calculation, (>> 1) for EXP reg
#define SC530AI_EXP_SHIFT 1

#define SC530AI_REG_ADDR_EXP_H       0x3e00
#define SC530AI_REG_ADDR_EXP_M       0x3e01
#define SC530AI_REG_ADDR_EXP_L       0x3e02
#define SC530AI_REG_ADDR_AGAIN_H     0x3e08
#define SC530AI_REG_ADDR_AGAIN_L     0x3e09
#define SC530AI_REG_ADDR_DGAIN_H     0x3e06
#define SC530AI_REG_ADDR_DGAIN_L     0x3e07
#define SC530AI_REG_ADDR_VMAX_H      0x320e
#define SC530AI_REG_ADDR_VMAX_L      0x320f

#define SC530AI_REG_ADDR_S_EXP_H     0x3e04
#define SC530AI_REG_ADDR_S_EXP_L     0x3e05
#define SC530AI_REG_ADDR_S_AGAIN_H   0x3e12
#define SC530AI_REG_ADDR_S_AGAIN_L   0x3e13
#define SC530AI_REG_ADDR_S_DGAIN_H   0x3e10
#define SC530AI_REG_ADDR_S_DGAIN_L   0x3e11
#define SC530AI_REG_ADDR_S_EXP_MAX_H 0x3e23
#define SC530AI_REG_ADDR_S_EXP_MAX_L 0x3e24

typedef enum {
    SC530AI_REG_EXP_H = 0,
    SC530AI_REG_EXP_M,
    SC530AI_REG_EXP_L,
    SC530AI_REG_AGAIN_H,
    SC530AI_REG_AGAIN_L,
    SC530AI_REG_DGAIN_H,
    SC530AI_REG_DGAIN_L,
    SC530AI_REG_VMAX_H,
    SC530AI_REG_VMAX_L,
    SC530AI_REG_L_MAX_NUM,

    // short frame reg info from here
    SC530AI_REG_S_EXP_H = SC530AI_REG_L_MAX_NUM,
    SC530AI_REG_S_EXP_L,
    SC530AI_REG_S_AGAIN_H,
    SC530AI_REG_S_AGAIN_L,
    SC530AI_REG_S_DGAIN_H,
    SC530AI_REG_S_DGAIN_L,
    SC530AI_REG_S_EXP_MAX_H,
    SC530AI_REG_S_EXP_MAX_L,
    SC530AI_REG_MAX_NUM
} sc530ai_reg_info;

sensor_context* sc530ai_get_context(xmedia_u32 dev);
xmedia_s32  sc530ai_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps);

#ifdef __cplusplus
}
#endif

#endif // __SC530AI_H__