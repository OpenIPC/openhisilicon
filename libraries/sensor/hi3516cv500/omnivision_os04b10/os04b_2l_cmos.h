/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of os04b_2l_cmos.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __OS04B_2L_CMOS_H_
#define __OS04B_2L_CMOS_H_

#include "hi_comm_isp.h"
#include "hi_sns_ctrl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define OS04B_2L_I2C_ADDR    0x78
#define OS04B_2L_ADDR_BYTE   1
#define OS04B_2L_DATA_BYTE   1

#define OS04B_2L_SENSOR_GET_CTX(dev, pstCtx)   ((pstCtx) = os04b_2l_get_ctx(dev))

ISP_SNS_STATE_S *os04b_2l_get_ctx(VI_PIPE vi_pipe);
ISP_SNS_COMMBUS_U *os04b_2l_get_bus_Info(VI_PIPE vi_pipe);

void os04b_2l_init(VI_PIPE vi_pipe);
void os04b_2l_exit(VI_PIPE vi_pipe);
void os04b_2l_standby(VI_PIPE vi_pipe);
void os04b_2l_restart(VI_PIPE vi_pipe);
int  os04b_2l_write_register(VI_PIPE vi_pipe, HI_U32 addr, HI_U32 data);
int  os04b_2l_read_register(VI_PIPE vi_pipe, HI_U32 addr);
void os04b_2l_mirror_flip(VI_PIPE vi_pipe, ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __OS04B_2L_CMOS_H_ */
