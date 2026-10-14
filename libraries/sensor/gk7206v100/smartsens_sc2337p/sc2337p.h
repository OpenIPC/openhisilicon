#ifndef __SC2337P_H__
#define __SC2337P_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

// sensor fps mode
#define SC2337P_FULL_LINES_MAX      0x7FFF
#define SC2337P_VMAX_2M_1080_LINEAR 0X465
#define SC2337P_EXP_OFFSET_LINEAR   6

#define SC2337P_REG_ADDR_EXP_H    0x3e00
#define SC2337P_REG_ADDR_EXP_M    0x3e01
#define SC2337P_REG_ADDR_EXP_L    0x3e02
#define SC2337P_REG_ADDR_AGAIN    0x3e09
#define SC2337P_REG_ADDR_DGAIN_H  0x3e06
#define SC2337P_REG_ADDR_DGAIN_L  0x3e07
#define SC2337P_REG_ADDR_VMAX_H   0x320e
#define SC2337P_REG_ADDR_VMAX_L   0x320f

typedef enum {
    SC2337P_REG_EXP_H = 0,
    SC2337P_REG_EXP_M,
    SC2337P_REG_EXP_L,
    SC2337P_REG_AGAIN,
    SC2337P_REG_DGAIN_H,
    SC2337P_REG_DGAIN_L,
    SC2337P_REG_VMAX_H,
    SC2337P_REG_VMAX_L,
    SC2337P_REG_MAX_NUM
} sc2337p_reg_info;

#ifdef __cplusplus
}
#endif

#endif // __SC2337P_H__
