#ifndef __SC200AI_H__
#define __SC200AI_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SC200AI_VMAX_1M_LINEAR 0x2ee
#define SC200AI_VMAX_1M_WDR    0x8cc
#define SC200AI_VMAX_2M_LINEAR 0x465 // default value0x465 0x4b0
#define SC200AI_VMAX_2M_WDR    0x8ca

#define SC200AI_S_EXP_MAX_DEFAULT_720P   0x0088
#define SC200AI_S_EXP_MAX_DEFAULT_1080P  0x0088

// the value of the sc200ai exposure register is in half line units, (<< 1) for AE calculation, (>> 1) for EXP reg
#define SC200AI_EXP_SHIFT 1

#define SC200AI_EXP_OFFSET_LINEAR 8
#define SC200AI_EXP_OFFSET_WDR    10

#define SC200AI_REG_ADDR_EXP_H       0x3e00
#define SC200AI_REG_ADDR_EXP_M       0x3e01
#define SC200AI_REG_ADDR_EXP_L       0x3e02
#define SC200AI_REG_ADDR_S_EXP_H     0x3e22
#define SC200AI_REG_ADDR_S_EXP_M     0x3e04
#define SC200AI_REG_ADDR_S_EXP_L     0x3e05

#define SC200AI_REG_ADDR_S_EXP_MAX_H 0x3e23
#define SC200AI_REG_ADDR_S_EXP_MAX_L 0x3e24

#define SC200AI_REG_ADDR_AGAIN_H     0x3e08
#define SC200AI_REG_ADDR_AGAIN_L     0x3e09
#define SC200AI_REG_ADDR_DGAIN_H     0x3e06
#define SC200AI_REG_ADDR_DGAIN_L     0x3e07
#define SC200AI_REG_ADDR_S_AGAIN_H   0x3e12
#define SC200AI_REG_ADDR_S_AGAIN_L   0x3e13
#define SC200AI_REG_ADDR_S_DGAIN_H   0x3e10
#define SC200AI_REG_ADDR_S_DGAIN_L   0x3e11

#define SC200AI_REG_ADDR_VMAX_H      0x320e
#define SC200AI_REG_ADDR_VMAX_L      0x320f

typedef enum {
    SC200AI_REG_EXP_H = 0,
    SC200AI_REG_EXP_M,
    SC200AI_REG_EXP_L,
    SC200AI_REG_DGC_H,
    SC200AI_REG_DGC_L,
    SC200AI_REG_AGC_H,
    SC200AI_REG_AGC_L,
    SC200AI_REG_VMAX_H,
    SC200AI_REG_VMAX_L,
    SC200AI_REG_L_MAX_NUM,

    // short frame reg info from here
    SC200AI_REG_S_EXP_H = SC200AI_REG_L_MAX_NUM,
    SC200AI_REG_S_EXP_M,
    SC200AI_REG_S_EXP_L,
    SC200AI_REG_S_DGC_H,
    SC200AI_REG_S_DGC_L,
    SC200AI_REG_S_AGC_H,
    SC200AI_REG_S_AGC_L,
    SC200AI_REG_S_EXP_MAX_H,
    SC200AI_REG_S_EXP_MAX_L,
    SC200AI_REG_MAX_NUM
} sc200ai_reg_info;

#define SC200AI_FULL_LINES_MAX 0xFFFF

#ifdef __cplusplus
}
#endif

#endif // __SC200AI_H__