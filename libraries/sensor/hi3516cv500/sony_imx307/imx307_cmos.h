/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of imx307_cmos.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __IMX307_CMOS_H_
#define __IMX307_CMOS_H_

#include "hi_comm_isp.h"
#include "hi_sns_ctrl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define IMX307_I2C_ADDR    0x34
#define IMX307_ADDR_BYTE   2
#define IMX307_DATA_BYTE   1
#define IMX307_SENSOR_GET_CTX(dev, pstCtx) ((pstCtx) = imx307_get_ctx(dev))

ISP_SNS_STATE_S *imx307_get_ctx(VI_PIPE vi_pipe);
ISP_SNS_COMMBUS_U *imx307_get_bus_Info(VI_PIPE vi_pipe);

void imx307_init(VI_PIPE vi_pipe);
void imx307_exit(VI_PIPE vi_pipe);
void imx307_standby(VI_PIPE vi_pipe);
void imx307_restart(VI_PIPE vi_pipe);
int  imx307_write_register(VI_PIPE vi_pipe, HI_U32 addr, HI_U32 data);
int  imx307_read_register(VI_PIPE vi_pipe, HI_U32 addr);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* __IMX307_CMOS_H_ */
