/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of ov12870_cmos.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __OV12870_CMOS_H_
#define __OV12870_CMOS_H_

#include "hi_comm_isp.h"
#include "hi_sns_ctrl.h"
#include "ov12870_cmos_priv.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define OV12870_I2C_ADDR    0x6C
#define OV12870_ADDR_BYTE   2
#define OV12870_DATA_BYTE   1
#define OV12870_SENSOR_GET_CTX(dev, pstCtx)   ((pstCtx) = ov12870_get_ctx(dev))

ISP_SNS_STATE_S *ov12870_get_ctx(VI_PIPE vi_pipe);
ISP_SNS_COMMBUS_U *ov12870_get_bus_Info(VI_PIPE vi_pipe);
const OV12870_VIDEO_MODE_TBL_S *ov12870_get_mode_tb1(HI_U8 u8ImgMode);

void ov12870_init(VI_PIPE vi_pipe);
void ov12870_exit(VI_PIPE vi_pipe);
void ov12870_standby(VI_PIPE vi_pipe);
void ov12870_restart(VI_PIPE vi_pipe);
void ov12870_mirror_flip(VI_PIPE vi_pipe, ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip);
int  ov12870_write_register(VI_PIPE vi_pipe, HI_U32 addr, HI_U32 data);
int  ov12870_read_register(VI_PIPE vi_pipe, HI_U32 addr);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* __OV12870_CMOS_H_ */
