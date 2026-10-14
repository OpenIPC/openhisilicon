#ifndef __CV2005_H__
#define __CV2005_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

// sensor fps mode
#define CV2005_FULL_LINES_MAX        0xFFFFF
#define CV2005_VMAX_1920X1080_LINEAR 0x49C
#define CV2005_EXP_OFFSET_LINEAR     6

#define CV2005_REG_ADDR_EXP_H   0x304A
#define CV2005_REG_ADDR_EXP_M   0x3049
#define CV2005_REG_ADDR_EXP_L   0x3048
#define CV2005_REG_ADDR_AGAIN   0x3118
#define CV2005_REG_ADDR_DGAIN_H 0x311D
#define CV2005_REG_ADDR_DGAIN_L 0x311C
#define CV2005_REG_ADDR_VMAX_H  0x301E
#define CV2005_REG_ADDR_VMAX_M  0x301D
#define CV2005_REG_ADDR_VMAX_L  0x301C

typedef enum {
    CV2005_REG_EXP_H = 0,
    CV2005_REG_EXP_M,
    CV2005_REG_EXP_L,
    CV2005_REG_AGAIN,
    CV2005_REG_DGAIN_H,
    CV2005_REG_DGAIN_L,
    CV2005_REG_VMAX_H,
    CV2005_REG_VMAX_M,
    CV2005_REG_VMAX_L,
    CV2005_REG_MAX_NUM
} cv2005_reg_info;

#ifdef __cplusplus
}
#endif

#endif // __CV2005_H__