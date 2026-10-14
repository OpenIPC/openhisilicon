#ifndef __SC231HAI_H__
#define __SC231HAI_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

// sensor fps mode
#define SC231HAI_FULL_LINES_MAX      0x7FFF
#define SC231HAI_VMAX_2M_1080_LINEAR 1125

#define SC231HAI_EXP_OFFSET_LINEAR 11
#define SC231HAI_EXP_SHIFT         1

#define SC231HAI_REG_ADDR_EXP_H       0x3e00
#define SC231HAI_REG_ADDR_EXP_M       0x3e01
#define SC231HAI_REG_ADDR_EXP_L       0x3e02
#define SC231HAI_REG_ADDR_AGAIN_H     0x3e08
#define SC231HAI_REG_ADDR_AGAIN_L     0x3e09
#define SC231HAI_REG_ADDR_DGAIN_H     0x3e06
#define SC231HAI_REG_ADDR_DGAIN_L     0x3e07
#define SC231HAI_REG_ADDR_VMAX_H      0x320e
#define SC231HAI_REG_ADDR_VMAX_L      0x320f

typedef enum {
    SC231HAI_REG_EXP_H = 0,
    SC231HAI_REG_EXP_M,
    SC231HAI_REG_EXP_L,
    SC231HAI_REG_AGAIN_H,
    SC231HAI_REG_AGAIN_L,
    SC231HAI_REG_DGAIN_H,
    SC231HAI_REG_DGAIN_L,
    SC231HAI_REG_VMAX_H,
    SC231HAI_REG_VMAX_L,
    SC231HAI_REG_L_MAX_NUM,
    SC231HAI_REG_MAX_NUM
} sc231hai_reg_info;

#ifdef __cplusplus
}
#endif

#endif // __SC231HAI_H__