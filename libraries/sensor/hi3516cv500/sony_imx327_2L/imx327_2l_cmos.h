/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of imx327_2l_cmos.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __IMX327_2L_CMOS_H_
#define __IMX327_2L_CMOS_H_

#include "hi_comm_isp.h"
#include "hi_sns_ctrl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define IMX327_2L_I2C_ADDR    0x34
#define IMX327_2L_ADDR_BYTE   2
#define IMX327_2L_DATA_BYTE   1
#define IMX327_2L_SENSOR_GET_CTX(dev, pstCtx)   ((pstCtx) = imx327_2l_get_ctx(dev))

#define IMX327_FULL_LINES_MAX 0x3FFFF
#define IMX327_FULL_LINES_MAX_2TO1_WDR 0x8AA  /* considering the YOUT_SIZE and bad frame */

#define IMX327_INCREASE_LINES 1 /* make real fps less than stand fps because NVR require */
#define IMX327_VMAX_1080P30_LINEAR                    (1125 + IMX327_INCREASE_LINES)
#define IMX327_VMAX_1080P60TO30_WDR                   (1220 + IMX327_INCREASE_LINES)   /* 10bit */

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ?  (b) : (a))
#endif

typedef enum {
    IMX327_SENSOR_1080P_30FPS_LINEAR_MODE = 0,
    IMX327_SENSOR_1080P_30FPS_WDR_2to1_MODE,
    IMX327_SENSOR_1080P_30FPS_FRAME_WDR_2to1_MODE,
    IMX327_MODE_BUTT
} IMX327_2L_RES_MODE_E;

typedef struct hiIMX327_2L_VIDEO_MODE_TBL_S {
    HI_U32      u32VertiLines;
    HI_U32      u32MaxVertiLines;
    HI_FLOAT    f32MaxFps;
    HI_FLOAT    f32MinFps;
    HI_U32      u32Width;
    HI_U32      u32Height;
    HI_U8       u8SnsMode;
    WDR_MODE_E  enWDRMode;
    const char *pszModeName;
} IMX327_2L_VIDEO_MODE_TBL_S;

ISP_SNS_STATE_S *imx327_2l_get_ctx(VI_PIPE vi_pipe);
ISP_SNS_COMMBUS_U *imx327_2l_get_bus_Info(VI_PIPE vi_pipe);

void imx327_2l_init(VI_PIPE vi_pipe);
void imx327_2l_exit(VI_PIPE vi_pipe);
void imx327_2l_standby(VI_PIPE vi_pipe);
void imx327_2l_restart(VI_PIPE vi_pipe);
int  imx327_2l_write_register(VI_PIPE vi_pipe, HI_U32 addr, HI_U32 data);
int  imx327_2l_read_register(VI_PIPE vi_pipe, HI_U32 addr);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* __IMX327_2l_CMOS_H_ */
