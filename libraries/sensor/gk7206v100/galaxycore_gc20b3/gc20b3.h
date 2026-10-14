#ifndef __GC20B3_H__
#define __GC20B3_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GC20B3_VMAX_2M_LINEAR 1125

#define GC20B3_FULL_LINES_MAX 0x3FFF

// GC20B3 Register Address
#define GC20B3_REG_ADDR_EXP_H       0x0d03 //BIT[13:8]
#define GC20B3_REG_ADDR_EXP_L       0x0d04 //BIT[7:0]

#define GC20B3_REG_ADDR_AGC_L       0x00d0  //BIT[7:0]
#define GC20B3_REG_ADDR_AGC_H       0x0dc1  //BIT[0]
#define GC20B3_REG_ADDR_AGC3        0x00b8
#define GC20B3_REG_ADDR_AGC4        0x00b9
#define GC20B3_REG_ADDR_AGC5        0x0058
#define GC20B3_REG_ADDR_COL_GC_H    0x0080
#define GC20B3_REG_ADDR_COL_GC_L    0x0155
#define GC20B3_REG_ADDR_DGC_H       0x00b1  //BIT[3:0]
#define GC20B3_REG_ADDR_DGC_L       0x00b2  //BIT[7:2]

#define GC20B3_REG_ADDR_VMAX_H      0x0d41 //BIT[13:8]
#define GC20B3_REG_ADDR_VMAX_L      0x0d42 //BIT[7:0]

#define GC20B3_EXP_OFFSET 8

typedef enum {
    GC20B3_REG_EXP_H = 0,
    GC20B3_REG_EXP_L,
    GC20B3_REG_AGC_L,
    GC20B3_REG_AGC_H,
    GC20B3_REG_AGC3,
    GC20B3_REG_AGC4,
    GC20B3_REG_AGC5,
    GC20B3_REG_COL_GC_H,
    GC20B3_REG_COL_GC_L,
    GC20B3_REG_DGC_H,
    GC20B3_REG_DGC_L,
    GC20B3_REG_VMAX_H,
    GC20B3_REG_VMAX_L,
    GC20B3_REG_L_MAX_NUM,
} gc20b3_reg_index;

#ifdef __cplusplus
}
#endif

#endif // __GC20B3_H__
