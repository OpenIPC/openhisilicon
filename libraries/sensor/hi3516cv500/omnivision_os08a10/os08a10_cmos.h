/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of os08a10_cmos.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __OS08A10_CMOS_H_
#define __OS08A10_CMOS_H_

#include "hi_comm_isp.h"
#include "hi_sns_ctrl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define OS08A10_I2C_ADDR    0x6C
#define OS08A10_ADDR_BYTE   2
#define OS08A10_DATA_BYTE   1
#define OS08A10_SENSOR_GET_CTX(dev, pstCtx) ((pstCtx) = os08a10_get_ctx(dev))

ISP_SNS_STATE_S *os08a10_get_ctx(VI_PIPE vi_pipe);
ISP_SNS_COMMBUS_U *os08a10_get_bus_Info(VI_PIPE vi_pipe);

void OS08A10_init(VI_PIPE vi_pipe);
void OS08A10_exit(VI_PIPE vi_pipe);
void OS08A10_standby(VI_PIPE vi_pipe);
void OS08A10_restart(VI_PIPE vi_pipe);
int  OS08A10_write_register(VI_PIPE vi_pipe, HI_U32 addr, HI_U32 data);
int  OS08A10_read_register(VI_PIPE vi_pipe, HI_U32 addr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __OS08A10_CMOS_H_ */
