/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __SC500AI_CMOS_H_
#define __SC500AI_CMOS_H_

#include "common.h"
#include "sns_ctrl.h"
#include "type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define SC500AI_I2C_ADDR    0x60
#define SC500AI_ADDR_BYTE   2
#define SC500AI_DATA_BYTE   1
#define sc500ai_sensor_get_ctx(dev, pstCtx)   (pstCtx = sc500ai_get_ctx(dev))

typedef enum {
    E_EXP_H_ADDR = 0,
    E_EXP_M_ADDR,
    E_EXP_L_ADDR,
    E_DGC_H_ADDR,
    E_DGC_L_ADDR,
    E_AGC_H_ADDR,
    E_AGC_L_ADDR,
    E_VMAX_H_ADDR,
    E_VMAX_L_ADDR,
    E_REG_MAX_IDX
}SC500AI_REG_ADDR_E;

ISP_SNS_STATE_S *sc500ai_get_ctx(VI_PIPE ViPipe);
ISP_SNS_COMMBUS_U *sc500ai_get_bus_Info(VI_PIPE ViPipe);

void sc500ai_init(VI_PIPE ViPipe);
void sc500ai_exit(VI_PIPE ViPipe);
void sc500ai_standby(VI_PIPE ViPipe);
void sc500ai_restart(VI_PIPE ViPipe);
int sc500ai_write_register(VI_PIPE ViPipe, GK_S32 addr, GK_S32 data);
int  sc500ai_read_register(VI_PIPE ViPipe, GK_S32 addr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* __SC500AI_CMOS_H_ */
