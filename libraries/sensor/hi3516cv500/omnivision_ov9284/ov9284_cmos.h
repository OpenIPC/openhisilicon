/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of ov12870_cmos.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __OV9284_CMOS_H_
#define __OV9284_CMOS_H_

#include "hi_comm_sns.h"
#include "hi_sns_ctrl.h"
#include "ov9284_cmos_priv.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define OV9284_I2C_ADDR    0xc0
#define OV9284_ADDR_BYTE   2
#define OV9284_DATA_BYTE   1
#define OV9284_SENSOR_GET_CTX(dev, pstCtx)   ((pstCtx) = ov9284_get_ctx(dev))

ISP_SNS_STATE_S *ov9284_get_ctx(VI_PIPE vi_pipe);
ISP_SNS_COMMBUS_U *ov9284_get_bus_Info(VI_PIPE vi_pipe);
const OV9284_VIDEO_MODE_TBL_S *ov9284_get_mode_tb1(HI_U8 u8ImgMode);

void ov9284_init(VI_PIPE vi_pipe);
void ov9284_exit(VI_PIPE vi_pipe);
void ov9284_standby(VI_PIPE vi_pipe);
void ov9284_restart(VI_PIPE vi_pipe);
void ov9284_mirror_flip(VI_PIPE vi_pipe, ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip);
int  ov9284_write_register(VI_PIPE vi_pipe, HI_U32 addr, HI_U32 data);
int  ov9284_read_register(VI_PIPE vi_pipe, HI_U32 addr);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* __OV9284_CMOS_H_ */
