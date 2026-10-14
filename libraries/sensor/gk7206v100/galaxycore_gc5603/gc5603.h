#ifndef __GC5603_H__
#define __GC5603_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GC5603_VMAX_5M_LINEAR 1750

#define GC5603_FULL_LINES_MAX 0x3FFF

// GC5603 Register Address
#define GC5603_REG_ADDR_EXP_H       0x0202 //BIT[13:8]
#define GC5603_REG_ADDR_EXP_L       0x0203 //BIT[7:0]
#define GC5603_REG_ADDR_AGC_CTL     0x031d
#define GC5603_REG_ADDR_AGC_L       0x0614  //BIT[7:0]
#define GC5603_REG_ADDR_AGC_M       0x0615  //BIT[3:0]
#define GC5603_REG_ADDR_AGC_H       0x0225  //BIT[0]
#define GC5603_REG_ADDR_AGC3        0x1467
#define GC5603_REG_ADDR_AGC4        0x1468
#define GC5603_REG_ADDR_COL_GC_H    0x00b8
#define GC5603_REG_ADDR_COL_GC_L    0x00b9
#define GC5603_REG_ADDR_DGC_H       0x0064  //BIT[3:0]
#define GC5603_REG_ADDR_DGC_L       0x0065  //BIT[7:2]

#define GC5603_REG_ADDR_VMAX_H      0x0340 //BIT[13:8]
#define GC5603_REG_ADDR_VMAX_L      0x0341 //BIT[7:0]

#define GC5603_EXP_OFFSET 8

typedef enum {
    GC5603_REG_EXP_H = 0,
    GC5603_REG_EXP_L,
    GC5603_REG_AGC_CTL,
    GC5603_REG_AGC_L,
    GC5603_REG_AGC_M,
    GC5603_REG_AGC_H,
    GC5603_REG_AGC_CTL1,
    GC5603_REG_AGC3,
    GC5603_REG_AGC4,
    GC5603_REG_COL_GC_H,
    GC5603_REG_COL_GC_L,
    GC5603_REG_DGC_H,
    GC5603_REG_DGC_L,
    GC5603_REG_VMAX_H,
    GC5603_REG_VMAX_L,
    GC5603_REG_L_MAX_NUM,
} gc5603_reg_index;

#ifdef __cplusplus
}
#endif

#endif // __GC5603_H__
