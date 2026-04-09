/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of imx390_cmos.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __IMX390_CMOS_H_
#define __IMX390_CMOS_H_

#include "hi_comm_isp.h"
#include "hi_sns_ctrl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define IMX390_I2C_ADDR    0x34
#define IMX390_ADDR_BYTE   2
#define IMX390_DATA_BYTE   1
#define IMX390_SENSOR_GET_CTX(dev, pstCtx)   ((pstCtx) = imx390_get_ctx(dev))

#define IMX390_FULL_LINES_MAX 0xFFFFF

#define IMX390_INCREASE_LINES 1 /* make real fps less than stand fps because NVR require */
#define IMX390_VMAX_1080P30_LINEAR                    (1125 + IMX390_INCREASE_LINES)

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ?  (b) : (a))
#endif

typedef enum {
    IMX390_SENSOR_1080P_30FPS_LINEAR_MODE = 0,
    IMX390_MODE_BUTT
} IMX390_RES_MODE_E;

typedef struct hiIMX390_VIDEO_MODE_TBL_S {
    HI_U32      u32VertiLines;
    HI_U32      u32MaxVertiLines;
    HI_FLOAT    f32MaxFps;
    HI_FLOAT    f32MinFps;
    HI_U32      u32Width;
    HI_U32      u32Height;
    HI_U8       u8SnsMode;
    WDR_MODE_E  enWDRMode;
    const char *pszModeName;
} IMX390_VIDEO_MODE_TBL_S;

ISP_SNS_STATE_S *imx390_get_ctx(VI_PIPE vi_pipe);
ISP_SNS_COMMBUS_U *imx390_get_bus_Info(VI_PIPE vi_pipe);
unsigned char imx390_get_serdes_i2c_addr(VI_PIPE vi_pipe);

void imx390_init(VI_PIPE vi_pipe);
void imx390_exit(VI_PIPE vi_pipe);
void imx390_standby(VI_PIPE vi_pipe);
void imx390_restart(VI_PIPE vi_pipe);
int  imx390_write_register(VI_PIPE vi_pipe, HI_U32 addr, HI_U32 data);
int  imx390_read_register(VI_PIPE vi_pipe, HI_U32 addr);
int  imx390_set_bus_ex_info(VI_PIPE vi_pipe, ISP_SNS_BUS_EX_S *serdes_info);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* __IMX390_CMOS_H_ */
