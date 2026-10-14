#ifndef __SC835HAI_H__
#define __SC835HAI_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

// sensor fps mode
#define SC835HAI_FULL_LINES_MAX      0x7FFF
#define SC835HAI_VMAX_8M_2160_LINEAR 2250
#define SC835HAI_VMAX_4M_1520_LINEAR 2250
#define SC835HAI_EXP_OFFSET_LINEAR 13

// the value of the sc835hai exposure register is in half line units, (<< 1) for AE calculation, (>> 1) for EXP reg
#define SC835HAI_EXP_SHIFT 1

#define SC835HAI_REG_ADDR_EXP_H       0x3e00
#define SC835HAI_REG_ADDR_EXP_M       0x3e01
#define SC835HAI_REG_ADDR_EXP_L       0x3e02
#define SC835HAI_REG_ADDR_AGAIN_H     0x3e08
#define SC835HAI_REG_ADDR_AGAIN_L     0x3e09
#define SC835HAI_REG_ADDR_DGAIN_H     0x3e06
#define SC835HAI_REG_ADDR_DGAIN_L     0x3e07
#define SC835HAI_REG_ADDR_VMAX_H      0x320e
#define SC835HAI_REG_ADDR_VMAX_L      0x320f

#define SC835HAI_REG_ADDR_S_EXP_H     0x3e04
#define SC835HAI_REG_ADDR_S_EXP_L     0x3e05
#define SC835HAI_REG_ADDR_S_AGAIN_H   0x3e12
#define SC835HAI_REG_ADDR_S_AGAIN_L   0x3e13
#define SC835HAI_REG_ADDR_S_DGAIN_H   0x3e10
#define SC835HAI_REG_ADDR_S_DGAIN_L   0x3e11
#define SC835HAI_REG_ADDR_S_EXP_MAX_H 0x3e23
#define SC835HAI_REG_ADDR_S_EXP_MAX_L 0x3e24

typedef enum {
    SC835HAI_REG_EXP_H = 0,
    SC835HAI_REG_EXP_M,
    SC835HAI_REG_EXP_L,
    SC835HAI_REG_AGAIN_H,
    SC835HAI_REG_AGAIN_L,
    SC835HAI_REG_DGAIN_H,
    SC835HAI_REG_DGAIN_L,
    SC835HAI_REG_VMAX_H,
    SC835HAI_REG_VMAX_L,
    SC835HAI_REG_L_MAX_NUM,

  // short frame reg info from here
    SC835HAI_REG_S_EXP_H = SC835HAI_REG_L_MAX_NUM,
    SC835HAI_REG_S_EXP_L,
    SC835HAI_REG_S_AGAIN_H,
    SC835HAI_REG_S_AGAIN_L,
    SC835HAI_REG_S_DGAIN_H,
    SC835HAI_REG_S_DGAIN_L,
    SC835HAI_REG_S_EXP_MAX_H,
    SC835HAI_REG_S_EXP_MAX_L,
    SC835HAI_REG_MAX_NUM
} sc835hai_reg_info;

#ifdef __cplusplus
}
#endif

#endif // __SC835HAI_H__
