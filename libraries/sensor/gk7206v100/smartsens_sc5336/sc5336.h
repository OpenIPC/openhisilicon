#ifndef __SC5336_H__
#define __SC5336_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

// sensor fps mode
#define SC5336_FULL_LINES_MAX    0x7FFF
#define SC5336_VMAX_5M_LINEAR    1800
#define SC5336_EXP_OFFSET_LINEAR 10

#define SC5336_REG_ADDR_EXP_H   0x3e00
#define SC5336_REG_ADDR_EXP_M   0x3e01
#define SC5336_REG_ADDR_EXP_L   0x3e02
#define SC5336_REG_ADDR_AGAIN_H 0x3e08
#define SC5336_REG_ADDR_AGAIN_L 0x3e09
#define SC5336_REG_ADDR_DGAIN_H 0x3e06
#define SC5336_REG_ADDR_DGAIN_L 0x3e07
#define SC5336_REG_ADDR_VMAX_H  0x320e
#define SC5336_REG_ADDR_VMAX_L  0x320f

typedef enum {
    SC5336_REG_EXP_H = 0,
    SC5336_REG_EXP_M,
    SC5336_REG_EXP_L,
    SC5336_REG_AGAIN_H,
    SC5336_REG_AGAIN_L,
    SC5336_REG_DGAIN_H,
    SC5336_REG_DGAIN_L,
    SC5336_REG_VMAX_H,
    SC5336_REG_VMAX_L,
    SC5336_REG_L_MAX_NUM
} sc5336_reg_info;

#ifdef __cplusplus
}
#endif

#endif // __SC5336_H__