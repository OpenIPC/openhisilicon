#ifndef __SC3338_H__
#define __SC3338_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SC3338_VMAX_3M_LINEAR       1350

#define SC3338_EXP_SHIFT 0

#define SC3338_EXP_OFFSET_LINEAR 4

#define SC3336_EXP_H_ADDR            0x3E00
#define SC3336_EXP_M_ADDR            0x3E01
#define SC3336_EXP_L_ADDR            0x3E02
#define SC3336_AGAIN_H_ADDR          0x3E08
#define SC3336_AGAIN_L_ADDR          0x3E09
#define SC3336_DGAIN_H_ADDR          0x3E06
#define SC3336_DGAIN_L_ADDR          0x3E07
#define SC3336_VMAX_H_ADDR           0x320E
#define SC3336_VMAX_L_ADDR           0x320F

typedef enum {
    SC3338_REG_EXP_H = 0,
    SC3338_REG_EXP_M,
    SC3338_REG_EXP_L,
    SC3338_REG_DGC_H,
    SC3338_REG_DGC_L,
    SC3338_REG_AGC_H,
    SC3338_REG_AGC_L,
    SC3338_REG_VMAX_H,
    SC3338_REG_VMAX_L,
    SC3338_REG_MAX_NUM
} sc3338_reg_info;

#define SC3338_FULL_LINES_MAX 0x7FFF

xmedia_void sc3338_delay_ms(xmedia_s32 ms);

#ifdef __cplusplus
}
#endif

#endif // __SC3338_H__
