#ifndef __OS04J10_H__
#define __OS04J10_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OS04J10_VMAX_4M_LINEAR 1489
#define OS04J10_VMAX_4M_WDR    1489

#define OS04J10_REG_ADDR_PAGE      0xFD
#define OS04J10_REG_ADDR_TRIG      0xFE
#define OS04J10_REG_ADDR_EXP_H     0x0E
#define OS04J10_REG_ADDR_EXP_L     0x0F
#define OS04J10_REG_ADDR_AGAIN     0x24
#define OS04J10_REG_ADDR_SCG_EN    0xED

#define OS04J10_REG_ADDR_DGAIN_H   0x1F
#define OS04J10_REG_ADDR_DGAIN_L   0x20
#define OS04J10_REG_ADDR_HMAX_H    0x2A
#define OS04J10_REG_ADDR_HMAX_L    0x2B
#define OS04J10_REG_ADDR_VBLANK_H  0x14
#define OS04J10_REG_ADDR_VBLANK_L  0x15
#define OS04J10_REG_ADDR_S_EXP_H   0x1A
#define OS04J10_REG_ADDR_S_EXP_L   0x1B
#define OS04J10_REG_ADDR_S_AGAIN   0xE7
#define OS04J10_REG_ADDR_S_DGAIN_H 0xE4
#define OS04J10_REG_ADDR_S_DGAIN_L 0xE5
#define OS04J10_REG_ADDR_STANDBY   0x09

#define OS04J10_EXP_OFFSET         8

typedef enum {
    OS04J10_REG_PAGE  = 0,
    OS04J10_REG_EXP_H,
    OS04J10_REG_EXP_L,
    OS04J10_REG_DGAIN_H,
    OS04J10_REG_DGAIN_L,
    OS04J10_REG_AGAIN,
    OS04J10_REG_SCG_EN,
    OS04J10_REG_VBLANK_H,
    OS04J10_REG_VBLANK_L,
    OS04J10_REG_TRIG,
    OS04J10_REG_L_MAX_NUM,

    // short frame reg info from here
    OS04J10_REG_S_EXP_H = OS04J10_REG_L_MAX_NUM,
    OS04J10_REG_S_EXP_L,
    OS04J10_REG_S_DGAIN_H,
    OS04J10_REG_S_DGAIN_L,
    OS04J10_REG_S_AGAIN,

    OS04J10_REG_MAX_NUM
} os04a10_reg_info;

#define OS04J10_FULL_LINES_MAX 0xFFFF

xmedia_void os04j10_set_dcg_ratio(xmedia_u32 dev, xmedia_u32 dcg_ratio);

#ifdef __cplusplus
}
#endif

#endif // __OS04J10_H__
