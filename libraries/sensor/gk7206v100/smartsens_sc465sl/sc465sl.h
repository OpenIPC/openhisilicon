#ifndef __SC465SL_H__
#define __SC465SL_H__

#include "sc465sl_ctrl.h"
#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SC465SL_REG_ADDR_EXP_HIGHER     0x3E20
#define SC465SL_REG_ADDR_EXP_H          0x3E00
#define SC465SL_REG_ADDR_EXP_M          0x3E01
#define SC465SL_REG_ADDR_EXP_L          0x3E02
#define SC465SL_REG_ADDR_DGAIN_H        0x3E06
#define SC465SL_REG_ADDR_DGAIN_L        0x3E07
#define SC465SL_REG_ADDR_AGAIN_H        0x3E08
#define SC465SL_REG_ADDR_AGAIN_L        0x3E09
#define SC465SL_REG_ADDR_VMAX_H         0x326D
#define SC465SL_REG_ADDR_VMAX_M         0x320E
#define SC465SL_REG_ADDR_VMAX_L         0x320F

#define SC465SL_REG_ADDR_S_EXP_HIGHER   0x3E21
#define SC465SL_REG_ADDR_S_EXP_H        0x3E22
#define SC465SL_REG_ADDR_S_EXP_M        0x3E04
#define SC465SL_REG_ADDR_S_EXP_L        0x3E05
#define SC465SL_REG_ADDR_S_AGAIN_H      0x3E12
#define SC465SL_REG_ADDR_S_AGAIN_L      0x3E13
#define SC465SL_REG_ADDR_S_DGAIN_H      0x3E10
#define SC465SL_REG_ADDR_S_DGAIN_L      0x3E11
#define SC465SL_SEXP_MAX_ADDR_H         0x3E33
#define SC465SL_SEXP_MAX_ADDR_M         0x3E23
#define SC465SL_SEXP_MAX_ADDR_L         0x3E24

#define SC465SL_FRAME_RATE_MIN   0x3DB8 //Min:3fps
#define EXP_OFFSET_LINEAR        8
#define EXP_OFFSET_WDR           13
#define SC465SL_EXP_MAX_DEFAULT    0x180

// the value of the sc465sl exposure register is in half line units, (<< 1) for AE calculation, (>> 1) for EXP reg
#define SC465SL_EXP_SHIFT 1

typedef enum {
    SC465SL_REG_EXP_HIGHER = 0,
    SC465SL_REG_EXP_H,
    SC465SL_REG_EXP_M,
    SC465SL_REG_EXP_L,
    SC465SL_REG_DGC_H,
    SC465SL_REG_DGC_L,
    SC465SL_REG_AGC_H,
    SC465SL_REG_AGC_L,
    SC465SL_REG_VMAX_H,
    SC465SL_REG_VMAX_M,
    SC465SL_REG_VMAX_L,
    SC465SL_REG_L_MAX_NUM,

    // short frame reg info from here
    SC465SL_REG_S_EXP_HIGHER = SC465SL_REG_L_MAX_NUM,
    SC465SL_REG_S_EXP_H,
    SC465SL_REG_S_EXP_M,
    SC465SL_REG_S_EXP_L,
    SC465SL_REG_S_DGAIN_H,
    SC465SL_REG_S_DGAIN_L,
    SC465SL_REG_S_AGAIN_H,
    SC465SL_REG_S_AGAIN_L,
    SC465SL_REG_S_EXP_MAX_H,
    SC465SL_REG_S_EXP_MAX_M,
    SC465SL_REG_S_EXP_MAX_L,
    SC465SL_REG_MAX_NUM
} sc465sl_reg_info;

#define SC465SL_VMAX_4M_2LANE_LINEAR  0xa6a
#if SC465SL_USE_CMS4
#define SC465SL_VMAX_4M_4LANE_LINEAR  0x62c
#else
#if SC465SL_60FPS
#define SC465SL_VMAX_4M_4LANE_LINEAR  0x640
#else
#define SC465SL_VMAX_4M_4LANE_LINEAR  0xa6a
#endif
#endif

#define SC465SL_VMAX_4M_4LANE_WDR     0xc80
#define SC465SL_FULL_LINES_MAX        0xFFFFF



#ifdef __cplusplus
}
#endif

#endif // __SC465SL_H__
