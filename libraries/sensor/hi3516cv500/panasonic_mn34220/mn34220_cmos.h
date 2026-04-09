/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of mn34220_cmos.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __MN34220_CMOS_H_
#define __MN34220_CMOS_H_

#include "hi_comm_isp.h"
#include "hi_sns_ctrl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define MN34220_I2C_ADDR    0x6C
#define MN34220_ADDR_BYTE   2
#define MN34220_DATA_BYTE   1
#define MN34220_SENSOR_GET_CTX(dev, pstCtx)   ((pstCtx) = mn34220_get_ctx(dev))

ISP_SNS_STATE_S *mn34220_get_ctx(VI_PIPE vi_pipe);
ISP_SNS_COMMBUS_U *mn34220_get_bus_Info(VI_PIPE vi_pipe);

void mn34220_init(VI_PIPE vi_pipe);
void mn34220_exit(VI_PIPE vi_pipe);
void mn34220_standby(VI_PIPE vi_pipe);
void mn34220_restart(VI_PIPE vi_pipe);
int  mn34220_write_register(VI_PIPE vi_pipe, HI_U32 addr, HI_U32 data);
int  mn34220_read_register(VI_PIPE vi_pipe, HI_U32 addr);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* __MN34220_CMOS_H_ */
