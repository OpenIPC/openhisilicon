#ifndef __OS04C10_H__
#define __OS04C10_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef FPGA
#define OS04C10_VMAX_4L_4M_LINEAR 0xA26
#define OS04C10_VMAX_2L_4M_LINEAR 0 // not support
#define OS04C10_VMAX_2L_4M_WDR    0 // not support
#else
#define OS04C10_VMAX_4L_4M_LINEAR 0x626
#define OS04C10_VMAX_2L_4M_LINEAR 0x626
#define OS04C10_VMAX_2L_4M_WDR    0x660
#endif

#define OS04C10_EXP_OFFSET  8

// OS04C10 Register Address
#define OS04C10_REG_ADDR_EXP_H      0x3501
#define OS04C10_REG_ADDR_EXP_L      0x3502
#define OS04C10_REG_ADDR_AGC_H      0x3508
#define OS04C10_REG_ADDR_AGC_L      0x3509
#define OS04C10_REG_ADDR_DGC_H      0x350a
#define OS04C10_REG_ADDR_DGC_L      0x350b

#define OS04C10_REG_ADDR_S_EXP_H    0x3511
#define OS04C10_REG_ADDR_S_EXP_L    0x3512
#define OS04C10_REG_ADDR_S_AGC_H    0x350c
#define OS04C10_REG_ADDR_S_AGC_L    0x350d
#define OS04C10_REG_ADDR_S_DGC_H    0x350e
#define OS04C10_REG_ADDR_S_DGC_L    0x350f

#define OS04C10_REG_ADDR_VMAX_H     0x380e
#define OS04C10_REG_ADDR_VMAX_L     0x380f

typedef enum {
    OS04C10_REG_EXP_H = 0,
    OS04C10_REG_EXP_L,
    OS04C10_REG_AGC_H,
    OS04C10_REG_AGC_L,
    OS04C10_REG_DGC_H,
    OS04C10_REG_DGC_L,
    OS04C10_REG_VMAX_H,
    OS04C10_REG_VMAX_L,
    OS04C10_REG_L_MAX_NUM,

    OS04C10_REG_S_EXP_H = OS04C10_REG_L_MAX_NUM,
    OS04C10_REG_S_EXP_L,
    OS04C10_REG_S_AGC_H,
    OS04C10_REG_S_AGC_L,
    OS04C10_REG_S_DGC_H,
    OS04C10_REG_S_DGC_L,
    OS04C10_REG_MAX_NUM
} os04c10_reg_index;

#ifdef __cplusplus
}
#endif

#endif // __OS04C10_H__
