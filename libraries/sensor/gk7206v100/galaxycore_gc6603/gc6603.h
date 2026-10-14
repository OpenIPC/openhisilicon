#ifndef __GC6603_H__
#define __GC6603_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GC6603_VMAX_5M_LINEAR 2150

#define GC6603_FULL_LINES_MAX 0x3FFF

#define GC6603_REG_ADDR_STANDBY 0x0100

// GC6603 Register Address
#define GC6603_REG_ADDR_EXP_H       0x0202 //BIT[13:8]
#define GC6603_REG_ADDR_EXP_L       0x0203 //BIT[7:0]

#define GC6603_REG_ADDR_AGC_L       0x0914  //BIT[7:0]
#define GC6603_REG_ADDR_AGC_M       0x0915  //BIT[3:0]
#define GC6603_REG_ADDR_AGC_H       0x0225  //BIT[0]
#define GC6603_REG_ADDR_AGC2        0x0e67
#define GC6603_REG_ADDR_AGC3        0x0e68
#define GC6603_REG_ADDR_AGC4        0x0242
#define GC6603_REG_ADDR_AGC5        0x031d
#define GC6603_REG_ADDR_COL_GC_H    0x0e28
#define GC6603_REG_ADDR_COL_GC_L    0x031d
#define GC6603_REG_ADDR_DGC_H       0x0064  //BIT[3:0]
#define GC6603_REG_ADDR_DGC_L       0x0065  //BIT[7:2]

#define GC6603_REG_ADDR_VMAX_H      0x0340 //BIT[13:8]
#define GC6603_REG_ADDR_VMAX_L      0x0341 //BIT[7:0]

#define GC6603_EXP_OFFSET 8

typedef enum {
    GC6603_REG_EXP_H = 0,
    GC6603_REG_EXP_L,
    GC6603_REG_AGC_L,
    GC6603_REG_AGC_M,
    GC6603_REG_AGC_H,
    GC6603_REG_AGC2,
    GC6603_REG_AGC3,
    GC6603_REG_AGC4,
    GC6603_REG_AGC5,
    GC6603_REG_COL_GC_H,
    GC6603_REG_COL_GC_L,
    GC6603_REG_DGC_H,
    GC6603_REG_DGC_L,
    GC6603_REG_VMAX_H,
    GC6603_REG_VMAX_L,
    GC6603_REG_MAX_NUM,
} gc6603_reg_index;

#ifdef __cplusplus
}
#endif

#endif // __GC6603_H__
