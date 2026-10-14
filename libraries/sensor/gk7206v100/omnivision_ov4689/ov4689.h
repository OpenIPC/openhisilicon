#ifndef __OV4689_H__
#define __OV4689_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OV4689_4L_VMAX_4M30_LINEAR  0x640

#define OV4689_REG_ADDR_EXP_H   0x3500
#define OV4689_REG_ADDR_EXP_M   0x3501
#define OV4689_REG_ADDR_EXP_L   0x3502
#define OV4689_REG_ADDR_AGC_H   0x3508
#define OV4689_REG_ADDR_AGC_L   0x3509
#define OV4689_REG_ADDR_VMAX_H  0x380e
#define OV4689_REG_ADDR_VMAX_L  0x380f

#define OV4689_EXP_OFFSET 9

typedef enum {
    OV4689_REG_EXP_H = 0,
    OV4689_REG_EXP_M,
    OV4689_REG_EXP_L,
    OV4689_REG_AGAIN_H,
    OV4689_REG_AGAIN_L,
    OV4689_REG_VMAX_H,
    OV4689_REG_VMAX_L,
    OV4689_REG_MAX_NUM
} ov4689_reg_info;

#define OV4689_FULL_LINES_MAX 0xFFF2

#ifdef __cplusplus
}
#endif

#endif //__OV4689_H__