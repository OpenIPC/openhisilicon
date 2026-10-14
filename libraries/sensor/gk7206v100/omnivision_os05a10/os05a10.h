#ifndef __OS05A10_H__
#define __OS05A10_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef FPGA
#define OS05A10_VMAX_5M_LINEAR 0x9c0
#define OS05A10_VMAX_4M_LINEAR 0x9c0
#define OS05A10_VMAX_5M_WDR    0x0   // not spupport
#define OS05A10_VMAX_4M_WDR    0x0   // not spupport
#else
#define OS05A10_VMAX_5M_LINEAR 0x921
#define OS05A10_VMAX_4M_LINEAR 0x921
#define OS05A10_VMAX_5M_WDR    0x836
#define OS05A10_VMAX_4M_WDR    0x836
#endif

#define OS05A10_REG_ADDR_EXP_H  0x3501
#define OS05A10_REG_ADDR_EXP_L  0x3502
#define OS05A10_REG_ADDR_AGC_H  0x3508 // Bit[5:0]
#define OS05A10_REG_ADDR_AGC_L  0x3509 // Bit[7:0]
#define OS05A10_REG_ADDR_DGC_H  0x350A // Bit[5:0]
#define OS05A10_REG_ADDR_DGC_L  0x350B // Bit[7:0]
#define OS05A10_REG_ADDR_VMAX_H 0X380E
#define OS05A10_REG_ADDR_VMAX_L 0X380F

#define OS05A10_REG_ADDR_S_EXP_H 0X3511
#define OS05A10_REG_ADDR_S_EXP_L 0X3512
#define OS05A10_REG_ADDR_S_AGC_H 0X350C // Bit[5:0]
#define OS05A10_REG_ADDR_S_AGC_L 0X350D // Bit[7:0]
#define OS05A10_REG_ADDR_S_DGC_H 0X350E // Bit[5:0]
#define OS05A10_REG_ADDR_S_DGC_L 0X350F // Bit[7:0]

#define OS05A10_EXP_OFFSET 8

typedef enum {
    OS05A10_REG_EXP_H = 0,
    OS05A10_REG_EXP_L,
    OS05A10_REG_AGC_H,
    OS05A10_REG_AGC_L,
    OS05A10_REG_DGC_H,
    OS05A10_REG_DGC_L,
    OS05A10_REG_VMAX_H,
    OS05A10_REG_VMAX_L,
    OS05A10_REG_L_MAX_NUM,

    // short frame reg info from here
    OS05A10_REG_S_EXP_H = OS05A10_REG_L_MAX_NUM,
    OS05A10_REG_S_EXP_L,
    OS05A10_REG_S_DGC_H,
    OS05A10_REG_S_DGC_L,
    OS05A10_REG_S_AGC_H,
    OS05A10_REG_S_AGC_L,
    OS05A10_REG_MAX_NUM
} os05a10_reg_info;

#define OS05A10_FULL_LINES_MAX 0x9240

#ifdef __cplusplus
}
#endif

#endif //__OS05A10_H__
