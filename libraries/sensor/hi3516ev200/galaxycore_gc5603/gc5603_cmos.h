/*
 * Copyright (c) GK. All rights reserved.
 */

#ifndef __GC5603_CMOS_H_
#define __GC5603_CMOS_H_

#include "comm_isp.h"
#include "sns_ctrl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define GC5603_I2C_ADDR    0x62
#define GC5603_ADDR_BYTE   2
#define GC5603_DATA_BYTE   1

#define GC5603_SENSOR_GET_CTX(dev, pstCtx)   ((pstCtx) = gc5603_get_ctx(dev))

ISP_SNS_STATE_S *gc5603_get_ctx(VI_PIPE ViPipe);
ISP_SNS_COMMBUS_U *gc5603_get_bus_Info(VI_PIPE ViPipe);

void gc5603_init(VI_PIPE ViPipe);
void gc5603_exit(VI_PIPE ViPipe);
void gc5603_standby(VI_PIPE ViPipe);
void gc5603_restart(VI_PIPE ViPipe);
void gc5603_mirror_flip(VI_PIPE ViPipe, ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip);
int  gc5603_write_register(VI_PIPE ViPipe, GK_S32 addr, GK_S32 data);
int  gc5603_read_register(VI_PIPE ViPipe, GK_S32 addr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* __gc5603_CMOS_H_ */

