/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of gc2053_cmos.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __GC2053_CMOS_H_
#define __GC2053_CMOS_H_

#include "hi_comm_isp.h"
#include "hi_sns_ctrl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define GC2053_I2C_ADDR    0x6e
#define GC2053_ADDR_BYTE   1
#define GC2053_DATA_BYTE   1

#define GC2053_SENSOR_GET_CTX(dev, pstCtx)   ((pstCtx) = gc2053_get_ctx(dev))

ISP_SNS_STATE_S *gc2053_get_ctx(VI_PIPE vi_pipe);
ISP_SNS_COMMBUS_U *gc2053_get_bus_Info(VI_PIPE vi_pipe);

void gc2053_init(VI_PIPE vi_pipe);
void gc2053_exit(VI_PIPE vi_pipe);
void gc2053_standby(VI_PIPE vi_pipe);
void gc2053_restart(VI_PIPE vi_pipe);
void gc2053_mirror_flip(VI_PIPE vi_pipe, ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip);
int  gc2053_write_register(VI_PIPE vi_pipe, HI_U32 addr, HI_U32 data);
int  gc2053_read_register(VI_PIPE vi_pipe, HI_U32 addr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __GC2053_CMOS_H_ */
