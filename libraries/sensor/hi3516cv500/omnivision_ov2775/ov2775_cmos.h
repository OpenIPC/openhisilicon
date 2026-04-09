/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of ov2775_cmos.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __OV2775_CMOS_H_
#define __OV2775_CMOS_H_

#include "hi_comm_isp.h"
#include "hi_sns_ctrl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define OV2775_I2C_ADDR    0x6C
#define OV2775_ADDR_BYTE   2
#define OV2775_DATA_BYTE   1
#define ov2775_sensor_get_ctx(dev, pstCtx) ((pstCtx) = ov2775_get_ctx(dev))

#define FULL_LINES_MAX                          0xFFFF

#define INCREASE_LINES 0 /* make real fps less than stand fps because NVR require */
#define VMAX_1080P30_LINEAR  (1127 + INCREASE_LINES)
#define VMAX_1080P30_2TO1_WDR (1127 + INCREASE_LINES)

#ifndef min
#define min(a, b) (((a) > (b)) ?  (b) : (a))
#endif

#ifndef NA
#define NA                   0xFFFF
#endif

typedef enum {
    OV2775_SENSOR_1080P_30FPS_LINEAR_MODE = 0,
    OV2775_SENSOR_1080P_30FPS_BUILT_IN_MODE,
    OV2775_SENSOR_1080P_30FPS_WDR_2TO1_MODE,
    OV2775_SENSOR_1080P_30FPS_LINEAR_2L_MODE,
    OV2775_SENSOR_1080P_30FPS_WDR_2TO1_2L_MODE,
    OV2775_SENSOR_1080P_30FPS_HLCG_WDR_2TO1_MODE,
    OV2775_MODE_BUTT
} ov2775_res_mode;

typedef struct {
    HI_U32      verti_lines;
    HI_U32      max_verti_lines;
    HI_FLOAT    max_fps;
    HI_FLOAT    min_fps;
    HI_U32      width;
    HI_U32      height;
    HI_U8       sns_mode;
    WDR_MODE_E  wdr_mode;
    const char *mode_name;
} ov2775_video_mode_tbl;

ISP_SNS_STATE_S *ov2775_get_ctx(VI_PIPE vi_pipe);
ISP_SNS_COMMBUS_U *ov2775_get_bus_info(VI_PIPE vi_pipe);
const ov2775_video_mode_tbl *ov2775_get_mode_tb(HI_U8 img_mode);

void ov2775_init(VI_PIPE vi_pipe);
void ov2775_exit(VI_PIPE vi_pipe);
void ov2775_standby(VI_PIPE vi_pipe);
void ov2775_restart(VI_PIPE vi_pipe);
void ov2775_mirror_flip(VI_PIPE vi_pipe, ISP_SNS_MIRRORFLIP_TYPE_E sns_mirror_flip);
int  ov2775_write_register(VI_PIPE vi_pipe, HI_U32 addr, HI_U32 data);
int  ov2775_read_register(VI_PIPE vi_pipe, HI_U32 addr);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* __OV2775_CMOS_H_ */
