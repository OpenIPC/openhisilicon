#ifndef __SC500AI_H__
#define __SC500AI_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

// sensor fps mode
#define SC500AI_FULL_LINES_MAX          0xFFFF
#define SC500AI_VMAX_5M_LINEAR          1650
#define SC500AI_VMAX_5M_WDR             3300
#define SC500AI_VMAX_640_480_LINEAR     1650
#define SC500AI_EXP_OFFSET_LINEAR       10
#define SC500AI_EXP_OFFSET_WDR          18
#define SC500AI_S_EXP_MAX_DEFAULT_1620P 0xcc

// the value of the sc500ai exposure register is in half line units, (<< 1) for AE calculation, (>> 1) for EXP reg
#define SC500AI_EXP_SHIFT 1

#define SC500AI_REG_ADDR_EXP_H       0x3e00
#define SC500AI_REG_ADDR_EXP_M       0x3e01
#define SC500AI_REG_ADDR_EXP_L       0x3e02
#define SC500AI_REG_ADDR_AGAIN_H     0x3e08
#define SC500AI_REG_ADDR_AGAIN_L     0x3e09
#define SC500AI_REG_ADDR_DGAIN_H     0x3e06
#define SC500AI_REG_ADDR_DGAIN_L     0x3e07
#define SC500AI_REG_ADDR_VMAX_H      0x320e
#define SC500AI_REG_ADDR_VMAX_L      0x320f

#define SC500AI_REG_ADDR_S_EXP_H     0x3e04
#define SC500AI_REG_ADDR_S_EXP_L     0x3e05
#define SC500AI_REG_ADDR_S_AGAIN_H   0x3e12
#define SC500AI_REG_ADDR_S_AGAIN_L   0x3e13
#define SC500AI_REG_ADDR_S_DGAIN_H   0x3e10
#define SC500AI_REG_ADDR_S_DGAIN_L   0x3e11
#define SC500AI_REG_ADDR_S_EXP_MAX_H 0x3e23
#define SC500AI_REG_ADDR_S_EXP_MAX_L 0x3e24

typedef enum {
    SC500AI_REG_EXP_H = 0,
    SC500AI_REG_EXP_M,
    SC500AI_REG_EXP_L,
    SC500AI_REG_AGAIN_H,
    SC500AI_REG_AGAIN_L,
    SC500AI_REG_DGAIN_H,
    SC500AI_REG_DGAIN_L,
    SC500AI_REG_VMAX_H,
    SC500AI_REG_VMAX_L,
    SC500AI_REG_L_MAX_NUM,

    // short frame reg info from here
    SC500AI_REG_S_EXP_H = SC500AI_REG_L_MAX_NUM,
    SC500AI_REG_S_EXP_L,
    SC500AI_REG_S_AGAIN_H,
    SC500AI_REG_S_AGAIN_L,
    SC500AI_REG_S_DGAIN_H,
    SC500AI_REG_S_DGAIN_L,
    SC500AI_REG_S_EXP_MAX_H,
    SC500AI_REG_S_EXP_MAX_L,
    SC500AI_REG_MAX_NUM
} sc500ai_reg_info;


#ifdef __cplusplus
}
#endif

#endif // __SC500AI_H__