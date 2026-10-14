#ifndef __GC4663_H__
#define __GC4663_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GC4663_FULL_LINES_MAX      (0x3FFF)

#define GC4663_SHUTTER_MAX  (0x3fdb)
#define GC4663_AGAIN_MIN    (1024)
#define GC4663_AGAIN_MAX    (77660)     //the max again is 77660 mean 75.84 recommend
#define GC4663_DGAIN_MIN    (1024)
#define GC4663_DGAIN_MAX    (16368)      //the max dgain is 16368 mean 15.98 recommend

#define GC4663_WDR_AGAIN_MIN    (1024)
#define GC4663_WDR_AGAIN_MAX    (113168)

#define GC4663_INCREASE_LINES (0) /* make real fps less than stand fps because NVR require*/
//#define GC4663_VMAX_4M_LINEAR (1500+GC4663_INCREASE_LINES)
#define GC4663_VMAX_4M_LINEAR (1500+GC4663_INCREASE_LINES)
#define GC4663_VMAX_4M_WDR    (1600+GC4663_INCREASE_LINES)

#define GC4663_EXP_OFFSET_LINEAR 2
#define GC4663_EXP_OFFSET_WDR    2

#define GC4663_REG_DELAY_CONFIG_NUM 0

#define GC4663_REG_ADDR_EXP_H         0x0203
#define GC4663_REG_ADDR_EXP_L         0x0202
#define GC4663_REG_ADDR_AGAIN_0_H     0x02b3
#define GC4663_REG_ADDR_AGAIN_0_L     0x02b4
#define GC4663_REG_ADDR_AGAIN_1_H     0x02b8
#define GC4663_REG_ADDR_AGAIN_1_L     0x02b9
#define GC4663_REG_ADDR_AGAIN_2       0x0515
#define GC4663_REG_ADDR_AGAIN_3       0x0519
#define GC4663_REG_ADDR_AGAIN_4       0x02d9
#define GC4663_REG_ADDR_DGAIN_H       0x020e
#define GC4663_REG_ADDR_DGAIN_L       0x020f
#define GC4663_REG_ADDR_VMAX_H        0x0340
#define GC4663_REG_ADDR_VMAX_L        0x0341
#define GC4663_REG_ADDR_S_EXP_H       0x0201
#define GC4663_REG_ADDR_S_EXP_L       0x0200

typedef enum {
    GC4663_REG_EXP_H = 0,
    GC4663_REG_EXP_L,
    GC4663_REG_AGAIN_0_H,
    GC4663_REG_AGAIN_0_L,
    GC4663_REG_AGAIN_1_H,
    GC4663_REG_AGAIN_1_L,
    GC4663_REG_AGAIN_2,
    GC4663_REG_AGAIN_3,
    GC4663_REG_AGAIN_4,
    GC4663_REG_DGAIN_H,
    GC4663_REG_DGAIN_L,
    GC4663_REG_VMAX_H,
    GC4663_REG_VMAX_L,
    GC4663_REG_L_MAX_NUM,

    // short frame reg info from here
    GC4663_REG_S_EXP_MAX_H = GC4663_REG_L_MAX_NUM,
    GC4663_REG_S_EXP_MAX_L,
    GC4663_REG_MAX_NUM
} gc4663_reg_info;

#ifdef __cplusplus
}
#endif

#endif // __GC4663_H__