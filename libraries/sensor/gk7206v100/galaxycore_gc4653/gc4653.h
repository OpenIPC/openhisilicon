#ifndef __GC4653_H__
#define __GC4653_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GC4653_VMAX_4M_LINEAR 1500

#define GC4653_FULL_LINES_MAX 0x3FFF

// GC4653 Register Address
#define GC4653_REG_ADDR_EXP_H       0x0202
#define GC4653_REG_ADDR_EXP_L       0x0203
#define GC4653_REG_ADDR_AGC_EXT0    0x02b3
#define GC4653_REG_ADDR_AGC_EXT1    0x02b4
#define GC4653_REG_ADDR_AGC_EXT2    0x02b8
#define GC4653_REG_ADDR_AGC_EXT3    0x02b9
#define GC4653_REG_ADDR_AGC_EXT4    0x0515
#define GC4653_REG_ADDR_AGC_EXT5    0x0519
#define GC4653_REG_ADDR_AGC_EXT6    0x02d9
#define GC4653_REG_ADDR_DGC_H       0x020e
#define GC4653_REG_ADDR_DGC_L       0x020f
#define GC4653_REG_ADDR_VMAX_H      0x0340
#define GC4653_REG_ADDR_VMAX_L      0x0341

#define GC4653_EXP_OFFSET 8

typedef enum {
    GC4653_REG_EXP_H = 0,
    GC4653_REG_EXP_L,
    GC4653_REG_AGC_EXT0,
    GC4653_REG_AGC_EXT1,
    GC4653_REG_AGC_EXT2,
    GC4653_REG_AGC_EXT3,
    GC4653_REG_AGC_EXT4,
    GC4653_REG_AGC_EXT5,
    GC4653_REG_AGC_EXT6,
    GC4653_REG_DGC_H,
    GC4653_REG_DGC_L,
    GC4653_REG_VMAX_H,
    GC4653_REG_VMAX_L,
    GC4653_REG_L_MAX_NUM,
} gc4653_reg_index;

#ifdef __cplusplus
}
#endif

#endif // __GC4653_H__
