#ifndef _IMX678_COMM_H_
#define _IMX678_COMM_H_

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

// Imx678 Register Address
#define IMX678_REG_ADDR_SHR0_H   0x3050
#define IMX678_REG_ADDR_SHR0_M   0x3051
#define IMX678_REG_ADDR_SHR0_L   0x3052
#define IMX678_REG_ADDR_GAIN     0x3070 //增益调节
#define IMX678_REG_ADDR_HCG      0x3030 //LCG与HCG的设置
#define IMX678_REG_ADDR_VMAX_H   0x3028
#define IMX678_REG_ADDR_VMAX_M   0x3029
#define IMX678_REG_ADDR_VMAX_L   0x302A
#define IMX678_REG_ADDR_SHR1_H   0x3054
#define IMX678_REG_ADDR_SHR1_M   0x3055
#define IMX678_REG_ADDR_SHR1_L   0x3056
#define IMX678_REG_ADDR_RHS1_H   0x3060
#define IMX678_REG_ADDR_RHS1_M   0x3061
#define IMX678_REG_ADDR_RHS1_L   0x3062
#define IMX678_REG_ADDR_HCG_SEL1 0x3031
#define IMX678_REG_ADDR_HCG_SEL2 0x3032

#define IMX678_HMAX_ADDR         0x302c
#define IMX678_WDR_BRL           2210

typedef enum {
    IMX678_REG_SHR0_H = 0,
    IMX678_REG_SHR0_M,
    IMX678_REG_SHR0_L,
    IMX678_REG_GAIN,
    IMX678_REG_HCG,
    IMX678_REG_VMAX_H,
    IMX678_REG_VMAX_M,
    IMX678_REG_VMAX_L,
    IMX678_REG_L_MAX_NUM,

    // short frame reg info from here
    IMX678_REG_SHR1_H = IMX678_REG_L_MAX_NUM,
    IMX678_REG_SHR1_M,
    IMX678_REG_SHR1_L,
    IMX678_REG_RHS1_H,
    IMX678_REG_RHS1_M,
    IMX678_REG_RHS1_L,
    IMX678_REG__HCG_SEL1,
    IMX678_REG__HCG_SEL2,
    IMX678_REG_MAX_NUM
}imx678_reg_info;

typedef struct {
    xmedia_u8  hcg;
    xmedia_u32 brl;
    xmedia_u32 rhs1_max;
    xmedia_u32 rhs2_max;
} imx678_state_s;


#define IMX678_LINEAR_REG_INFO_MAX_NUM   IMX678_REG_L_MAX_NUM
#define IMX678_2TO1_WDR_REG_INFO_MAX_NUM IMX678_REG_MAX_NUM


// local variables
#define IMX678_FULL_LINES_MAX          0xFFFFF
#define IMX678_FULL_LINES_MAX_2TO1_WDR 9000

#define IMX678_VMAX_8M_LINEAR 3750
#define IMX678_VMAX_8M_WDR    2250  // 10bit

extern sensor_context *g_imx678_ctx[XMEDIA_SENSOR_DEV_MAX_NUM];
#define IMX678_GET_CTX(dev, ctx) ctx = g_imx678_ctx[dev]


#ifdef __cplusplus
}
#endif

#endif
