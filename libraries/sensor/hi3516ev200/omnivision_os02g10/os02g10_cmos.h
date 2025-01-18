/*
 * Copyright (c) GK.  All rights reserved.
 */

#ifndef __OS02G10_CMOS_H_
#define __OS02G10_CMOS_H_

#include "comm_sns.h"
#include "sns_ctrl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define OS02G10_I2C_ADDR   0x78 /* I2C Address of Sp2308 */
#define OS02G10_ADDR_BYTE  1
#define OS02G10_DATA_BYTE  1
#define OS02G10_SENSOR_GET_CTX(dev, pstCtx)   (pstCtx) = os02g10_get_ctx(dev)

ISP_SNS_STATE_S    *os02g10_get_ctx(VI_PIPE ViPipe);
ISP_SNS_COMMBUS_U  *os02g10_get_bus_Info(VI_PIPE ViPipe);

void os02g10_init(VI_PIPE ViPipe);
void os02g10_exit(VI_PIPE ViPipe);
void os02g10_mirror_flip(VI_PIPE ViPipe, ISP_SNS_MIRRORFLIP_TYPE_E sns_mirror_flip);
void os02g10_standby(VI_PIPE ViPipe);
void os02g10_restart(VI_PIPE ViPipe);
int  os02g10_write_register(VI_PIPE ViPipe, GK_S32 addr, GK_S32 data);
int  os02g10_read_register(VI_PIPE ViPipe, GK_S32 addr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* __OS02G10_CMOS_H_ */

