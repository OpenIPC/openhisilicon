#ifndef __OS02K10_H__
#define __OS02K10_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif
#define OS02K10_FULL_LINES_MAX   0xffff

#define INCREASE_LINES                          (0) /* make real fps less than stand fps because NVR require */
#define INCREASE_WDR                            (0) /* make real fps less than stand fps because NVR require */

#define OS02K10_VMAX_2M_LINEAR                  (1406+INCREASE_LINES)
#define OS02K10_VMAX_2M_BUILT_IN                (1460+INCREASE_WDR)

#define OS02K10_EXP_OFFSET  4

// OS02K10 Register Address
#define OS02K10_REG_ADDR_EXP_H      0x3501
#define OS02K10_REG_ADDR_EXP_L      0x3502
#define OS02K10_REG_ADDR_AGC_H      0x3508
#define OS02K10_REG_ADDR_AGC_L      0x3509
#define OS02K10_REG_ADDR_DGC_H      0x350a
#define OS02K10_REG_ADDR_DGC_M      0x350b
#define OS02K10_REG_ADDR_DGC_L      0x350c
#define OS02K10_VMAX_H_ADDR         0x380E
#define OS02K10_VMAX_L_ADDR         0x380F

#define OS02K10_REG_ADDR_BUILT_IN_AGC_H    0x3548
#define OS02K10_REG_ADDR_BUILT_IN_AGC_L    0x3549
#define OS02K10_REG_ADDR_BUILT_IN_DGC_H    0x354a
#define OS02K10_REG_ADDR_BUILT_IN_DGC_M    0x354b
#define OS02K10_REG_ADDR_BUILT_IN_DGC_L    0x354c

typedef enum {
    OS02K10_REG_EXP_H = 0,
    OS02K10_REG_EXP_L,
    OS02K10_REG_AGC_L,
    OS02K10_REG_AGC_H,
    OS02K10_REG_DGC_L,
    OS02K10_REG_DGC_M,
    OS02K10_REG_DGC_H,
    OS02K10_REG_VMAX_H,
    OS02K10_REG_VMAX_L,
    OS02K10_REG_L_MAX_NUM,

    OS02K10_REG_BUILT_IN_AGC_L = OS02K10_REG_L_MAX_NUM,
    OS02K10_REG_BUILT_IN_AGC_H,
    OS02K10_REG_BUILT_IN_DGC_L,
    OS02K10_REG_BUILT_IN_DGC_M,
    OS02K10_REG_BUILT_IN_DGC_H,
    OS02K10_REG_MAX_NUM
} os02k10_reg_index;

#ifdef __cplusplus
}
#endif

#endif // __OS02K10_H__
