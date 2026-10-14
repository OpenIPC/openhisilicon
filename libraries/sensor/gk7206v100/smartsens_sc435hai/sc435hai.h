#ifndef __SC435HAI_H__
#define __SC435HAI_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

// sensor fps mode
#define SC435HAI_FULL_LINES_MAX      0x7FFF
#define SC435HAI_VMAX_4M_1440_LINEAR 2000

#define SC435HAI_EXP_OFFSET_LINEAR 11
#define SC435HAI_EXP_SHIFT         1

#define SC435HAI_REG_ADDR_STANDBY 0x0100

#define SC435HAI_REG_ADDR_EXP_H       0x3e00
#define SC435HAI_REG_ADDR_EXP_M       0x3e01
#define SC435HAI_REG_ADDR_EXP_L       0x3e02
#define SC435HAI_REG_ADDR_AGAIN_H     0x3e08
#define SC435HAI_REG_ADDR_AGAIN_L     0x3e09
#define SC435HAI_REG_ADDR_DGAIN_H     0x3e06
#define SC435HAI_REG_ADDR_DGAIN_L     0x3e07
#define SC435HAI_REG_ADDR_VMAX_H      0x320e
#define SC435HAI_REG_ADDR_VMAX_L      0x320f

typedef enum {
    SC435HAI_REG_EXP_H = 0,
    SC435HAI_REG_EXP_M,
    SC435HAI_REG_EXP_L,
    SC435HAI_REG_AGAIN_H,
    SC435HAI_REG_AGAIN_L,
    SC435HAI_REG_DGAIN_H,
    SC435HAI_REG_DGAIN_L,
    SC435HAI_REG_VMAX_H,
    SC435HAI_REG_VMAX_L,
    SC435HAI_REG_L_MAX_NUM,
    SC435HAI_REG_MAX_NUM
} sc435hai_reg_info;

#ifdef __cplusplus
}
#endif

#endif // __SC435HAI_H__