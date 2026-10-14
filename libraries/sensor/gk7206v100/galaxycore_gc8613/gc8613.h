#ifndef __GC8613_H__
#define __GC8613_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef FPGA
#define GC8613_VMAX_4L_8M_LINEAR 0x23e0
#define GC8613_VMAX_2L_8M_LINEAR 0 // not support
#else
#define GC8613_VMAX_4L_8M_LINEAR 0x8f8
#define GC8613_VMAX_2L_8M_LINEAR 0x8ca
#endif

// GC8613 Register Address
#define GC8613_REG_ADDR_EXP_H       0x0202 //BIT[13:8]
#define GC8613_REG_ADDR_EXP_L       0x0203 //BIT[7:0]

#define GC8613_REG_ADDR_AGC_H       0x0225  //BIT[0]
#define GC8613_REG_ADDR_AGC_M       0x0615  //BIT[3:0]
#define GC8613_REG_ADDR_AGC_L       0x0614  //BIT[7:0]

#define GC8613_REG_ADDR_AGC1        0x1467
#define GC8613_REG_ADDR_AGC2        0x1468

#define GC8613_REG_ADDR_AGC3        0x026e
#define GC8613_REG_ADDR_AGC4        0x0270

#define GC8613_REG_ADDR_COL_GC_H    0x00b8
#define GC8613_REG_ADDR_COL_GC_L    0x00b9

#define GC8613_REG_ADDR_AGC5        0x1447

#define GC8613_REG_ADDR_DGC_H       0x0064  //BIT[3:0]
#define GC8613_REG_ADDR_DGC_L       0x0065  //BIT[7:2]

#define GC8613_REG_ADDR_VMAX_H      0x0340 //BIT[13:8]
#define GC8613_REG_ADDR_VMAX_L      0x0341 //BIT[7:0]

#define GC8613_REG_ADDR_AGC_CTL    0x031d

#define GC8613_EXP_OFFSET 8

typedef enum {
    GC8613_REG_EXP_H = 0,
    GC8613_REG_EXP_L,
    GC8613_REG_AGC_CTL1,
    GC8613_REG_AGC_L,
    GC8613_REG_AGC_M,
    GC8613_REG_AGC_CTL2,
    GC8613_REG_AGC_H,
    GC8613_REG_AGC1,
    GC8613_REG_AGC2,
    GC8613_REG_AGC_CTL3,
    GC8613_REG_AGC3,
    GC8613_REG_AGC4,
    GC8613_REG_AGC_CTL4,
    GC8613_REG_COL_GC_H,
    GC8613_REG_COL_GC_L,
    GC8613_REG_AGC5,
    GC8613_REG_DGC_H,
    GC8613_REG_DGC_L,
    GC8613_REG_VMAX_H,
    GC8613_REG_VMAX_L,
    GC8613_REG_L_MAX_NUM,
} gc8613_reg_index;

#ifdef __cplusplus
}
#endif

#endif // __GC8613_H__
