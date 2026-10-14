#ifndef _IMX307_COMM_H_
#define _IMX307_COMM_H_

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Imx307 Register Address */
#define IMX307_REG_ADDR_SHS1_L         0x3020//***积分时间设置寄存器
#define IMX307_REG_ADDR_SHS1_M         0x3021
#define IMX307_REG_ADDR_SHS1_H         0x3022
#define IMX307_REG_ADDR_GAIN           0x3014//增益调节
#define IMX307_REG_ADDR_HCG            0x3009//LCG与HCG的设置
#define IMX307_REG_ADDR_VMAX_L         0x3018
#define IMX307_REG_ADDR_VMAX_M         0x3019
#define IMX307_REG_ADDR_VMAX_H         0x301A
#define IMX307_REG_ADDR_SHS2_L         0x3024//长曝光的积分时间设置
#define IMX307_REG_ADDR_SHS2_M         0x3025
#define IMX307_REG_ADDR_SHS2_H         0x3026
#define IMX307_REG_ADDR_RHS1_L         0x3030//读出时间
#define IMX307_REG_ADDR_RHS1_M         0x3031
#define IMX307_REG_ADDR_RHS1_H         0x3032
#define IMX307_REG_ADDR_Y_OUT_SIZE     0x3418
#define IMX307_REG_ADDR_Y_OUT_SIZE_H   0x3419

#define IMX307_HMAX_ADDR               0x301c

typedef enum {
    IMX307_REG_SHS1_L = 0,
    IMX307_REG_SHS1_M,
    IMX307_REG_SHS1_H,
    IMX307_REG_GAIN,
    IMX307_REG_HCG,
    IMX307_REG_VMAX_L,
    IMX307_REG_VMAX_M,
    IMX307_REG_VMAX_H,
    IMX307_REG_L_MAX_NUM,

    // short frame reg info from here
    IMX307_REG_SHS2_L = IMX307_REG_L_MAX_NUM,
    IMX307_REG_SHS2_M,
    IMX307_REG_SHS2_H,
    IMX307_REG_RHS1_L,
    IMX307_REG_RHS1_M,
    IMX307_REG_RHS1_H,
    IMX307_REG__Y_OUT_SIZE,
    IMX307_REG__Y_OUT_SIZE_H,
    IMX307_REG_MAX_NUM
}imx307_reg_info;

typedef struct {
    xmedia_u8       u8Hcg;
    xmedia_u32      u32BRL;
    xmedia_u32      u32RHS1_MAX;
    xmedia_u32      u32RHS2_MAX;
} IMX307_STATE_S;


#define IMX307_LINEAR_REG_INFO_MAX_NUM   IMX307_REG_L_MAX_NUM
#define IMX307_2TO1_WDR_REG_INFO_MAX_NUM IMX307_REG_MAX_NUM

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
#define IMX307_FULL_LINES_MAX          (0x3FFFF)
#define IMX307_FULL_LINES_MAX_2TO1_WDR (0x8AA) // considering the YOUT_SIZE and bad frame

//imx307_init被使用
#define IMX307_VMAX_1080P30_LINEAR     1125

#define IMX307_VMAX_2L_1080P60TO30_WDR 1125
#define IMX307_VMAX_4L_1080P60TO30_WDR 1465

extern sensor_context *g_imx307_ctx[XMEDIA_SENSOR_DEV_MAX_NUM];
#define IMX307_GET_CTX(dev, ctx) ctx = g_imx307_ctx[dev]

#ifdef __cplusplus
}
#endif

#endif
