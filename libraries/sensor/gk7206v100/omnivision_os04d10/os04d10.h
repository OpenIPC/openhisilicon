#ifndef __OS04D10_H__
#define __OS04D10_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OS04D10_2L_VMAX_4M30_LINEAR  1769  //(1654,27Mhz)

#define OS04D10_REG_ADDR_EXP_H       0x03
#define OS04D10_REG_ADDR_EXP_L       0x04
#define OS04D10_REG_ADDR_AGAIN       0x24
#define OS04D10_REG_ADDR_DGAIN       0x39
#define OS04D10_REG_ADDR_VBLANK_H    0x05
#define OS04D10_REG_ADDR_VBLANK_L    0x06
#define OS04D10_REG_ADDR_TRIGGER     0x01
#define OS04D10_REG_ADDR_PAGE_SELECT 0xfd

#define OS04D10_EXP_OFFSET 9

typedef enum {
    OS04D10_REG_PAGE_SELECT = 0,
    OS04D10_REG_EXP_H ,
    OS04D10_REG_EXP_L,
    OS04D10_REG_AGAIN,
    OS04D10_REG_DGAIN,
    OS04D10_REG_VBLANK_H,
    OS04D10_REG_VBLANK_L,
    OS04D10_REG_TRIGGER,
    OS04D10_REG_L_MAX_NUM
} os04d10_reg_info;

#define OS04D10_FULL_LINES_MAX 0xFFF2

#ifdef __cplusplus
}
#endif

#endif //__OS04D10_H__