#ifndef __SC533HAI_H__
#define __SC533HAI_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SC533HAI_60FPS 0
// sensor fps mode
#if SC533HAI_60FPS
#define SC533HAI_VMAX_5M_1620_LINEAR   1750
#else
#define SC533HAI_VMAX_5M_1620_LINEAR   1875
#endif

#define SC533HAI_FULL_LINES_MAX      0x1FFF0

#define SC533HAI_EXP_OFFSET_LINEAR 8

#define SC533HAI_REG_ADDR_EXP_H       0x3e00
#define SC533HAI_REG_ADDR_EXP_M       0x3e01
#define SC533HAI_REG_ADDR_EXP_L       0x3e02
#define SC533HAI_REG_ADDR_AGAIN_H     0x3e08
#define SC533HAI_REG_ADDR_AGAIN_L     0x3e09
#define SC533HAI_REG_ADDR_DGAIN_H     0x3e06
#define SC533HAI_REG_ADDR_DGAIN_L     0x3e07
#define SC533HAI_REG_ADDR_VMAX_H      0x326d
#define SC533HAI_REG_ADDR_VMAX_M      0x320e
#define SC533HAI_REG_ADDR_VMAX_L      0x320f


typedef enum {
    SC533HAI_REG_EXP_H = 0,
    SC533HAI_REG_EXP_M,
    SC533HAI_REG_EXP_L,
    SC533HAI_REG_AGAIN_H,
    SC533HAI_REG_AGAIN_L,
    SC533HAI_REG_DGAIN_H,
    SC533HAI_REG_DGAIN_L,
    SC533HAI_REG_VMAX_H,
    SC533HAI_REG_VMAX_M,
    SC533HAI_REG_VMAX_L,
    SC533HAI_REG_L_MAX_NUM,
} sc533hai_reg_info;

#ifdef __cplusplus
}
#endif

#endif // __SC533HAI_H__