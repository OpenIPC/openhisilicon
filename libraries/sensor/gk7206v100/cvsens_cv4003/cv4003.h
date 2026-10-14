#ifndef __CV4003_H__
#define __CV4003_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CV4003_VMAX_4M_LINEAR    0x15e0 // 10bit
#define CV4003_VMAX_1M_LINEAR    0x708  // 10bit

#define CV4003_EXP_OFFSET_LINEAR 8

// CV4003 Register Address
#define CV4003_REG_ADDR_EXP_H   0x304a
#define CV4003_REG_ADDR_EXP_M   0x3049
#define CV4003_REG_ADDR_EXP_L   0x3048
#define CV4003_REG_ADDR_AGAIN   0x3118
#define CV4003_REG_ADDR_DGAIN_H 0x311D
#define CV4003_REG_ADDR_DGAIN_L 0x311C
#define CV4003_REG_ADDR_VMAX_H  0x301E
#define CV4003_REG_ADDR_VMAX_M  0x301D
#define CV4003_REG_ADDR_VMAX_L  0x301C

typedef enum {
    CV4003_REG_EXP_L = 0,
    CV4003_REG_EXP_M,
    CV4003_REG_EXP_H,
    CV4003_REG_AGAIN,
    CV4003_REG_DGAIN_L,
    CV4003_REG_DGAIN_H,
    CV4003_REG_VMAX_L,
    CV4003_REG_VMAX_M,
    CV4003_REG_VMAX_H,
    CV4003_REG_MAX_NUM,
} cv4003_reg_index;

#ifdef __cplusplus
}
#endif

#endif // __CV4003_H__
