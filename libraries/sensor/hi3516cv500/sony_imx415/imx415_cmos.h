/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of imx415_cmos.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __IMX415_CMOS_H_
#define __IMX415_CMOS_H_

#include "hi_comm_isp.h"
#include "hi_sns_ctrl.h"
#include "imx415_cmos_priv.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define IMX415_I2C_ADDR    0x34
#define IMX415_ADDR_BYTE   2
#define IMX415_DATA_BYTE   1
#define IMX458_SENSOR_GET_CTX(dev, pstCtx)   ((pstCtx) = imx415_get_ctx(dev))

ISP_SNS_STATE_S *imx415_get_ctx(VI_PIPE vi_pipe);
ISP_SNS_COMMBUS_U *imx415_get_bus_Info(VI_PIPE vi_pipe);
const IMX415_VIDEO_MODE_TBL_S *imx415_get_mode_tb1(HI_U8 u8ImgMode);

void imx415_init(VI_PIPE vi_pipe);
void imx415_exit(VI_PIPE vi_pipe);
void imx415_standby(VI_PIPE vi_pipe);
void imx415_restart(VI_PIPE vi_pipe);
void imx415_mirror_flip(VI_PIPE vi_pipe, ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip);
int  imx415_write_register(VI_PIPE vi_pipe, HI_U32 addr, HI_U32 data);
int  imx415_read_register(VI_PIPE vi_pipe, HI_U32 addr);
/* Crop W×H stored by cmos_set_image_mode for Mode 4 / Mode 5. NULL-tolerant. */
HI_VOID imx415_get_crop(VI_PIPE vi_pipe, HI_U32 *pu32W, HI_U32 *pu32H);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* __IMX415_CMOS_H_ */
