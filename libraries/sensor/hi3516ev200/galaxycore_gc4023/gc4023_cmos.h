/*
 * Copyright (c) GK.  All rights reserved.
 */

#ifndef __gc4023_CMOS_H_
#define __gc4023_CMOS_H_

#include "comm_isp.h"
#include "sns_ctrl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define GC4023_I2C_ADDR    0x52
#define GC4023_ADDR_BYTE   2
#define GC4023_DATA_BYTE   1


#define GC4023_SENSOR_GET_CTX(dev, pstCtx)   (pstCtx) = GC4023_get_ctx(dev)


ISP_SNS_STATE_S *GC4023_get_ctx(VI_PIPE vi_pipe_value);
ISP_SNS_COMMBUS_U *GC4023_get_bus_Info(VI_PIPE vi_pipe_value);

void GC4023_init(VI_PIPE vi_pipe_value);
void GC4023_exit(VI_PIPE vi_pipe_value);
void GC4023_standby(VI_PIPE vi_pipe_value);
void GC4023_restart(VI_PIPE vi_pipe_value);
int  GC4023_write_register(VI_PIPE vi_pipe_value, GK_S32 addr, GK_S32 data);
int  GC4023_read_register(VI_PIPE vi_pipe_value, GK_S32 addr);
void GC4023_mirror_flip(VI_PIPE vi_pipe_value, ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* __GC2083_CMOS_H_ */

