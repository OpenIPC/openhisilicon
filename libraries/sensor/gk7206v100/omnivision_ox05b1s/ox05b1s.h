#ifndef __OX05B1S_H__
#define __OX05B1S_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif
#define OX05B1S_FULL_LINES_MAX 0xFFFF

#define OX05B1S_VMAX_1M_LINEAR 0x424
#define OX05B1S_VMAX_5M_LINEAR 0x850

#define OX05B1S_EXP_OFFSET 30

// OX05B1S Register Address
#define OX05B1S_REG_ADDR_EXP_H 0x3501
#define OX05B1S_REG_ADDR_EXP_L 0x3502
#define OX05B1S_REG_ADDR_AGC_H 0x3508 //BIT[3:0]
#define OX05B1S_REG_ADDR_AGC_L 0x3509 //BIT[7:4]
#define OX05B1S_REG_ADDR_DGC_H 0x350a //BIT[3:0]
#define OX05B1S_REG_ADDR_DGC_M 0x350b //BIT[7:0]
#define OX05B1S_REG_ADDR_DGC_L 0x350c //BIT[7:6]
#define OX05B1S_VMAX_H_ADDR    0x380E
#define OX05B1S_VMAX_L_ADDR    0x380F

typedef enum {
    OX05B1S_REG_EXP_H = 0,
    OX05B1S_REG_EXP_L,
    OX05B1S_REG_AGC_L,
    OX05B1S_REG_AGC_H,
    OX05B1S_REG_DGC_L,
    OX05B1S_REG_DGC_M,
    OX05B1S_REG_DGC_H,
    OX05B1S_REG_VMAX_H,
    OX05B1S_REG_VMAX_L,
    OX05B1S_REG_MAX_NUM
} ox05b1s_reg_index;

#ifdef __cplusplus
}
#endif

#endif // __OX05B1S_H__
