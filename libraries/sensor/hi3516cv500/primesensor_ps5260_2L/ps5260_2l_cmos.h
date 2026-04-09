/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of ps5260_2l_cmos.h
 * Author: ISP SW
 * Create: 2020/02/05
 */

#ifndef __PS5260_2L_CMOS_H_
#define __PS5260_2L_CMOS_H_

#include "hi_comm_isp.h"
#include "hi_sns_ctrl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define PS5260_2L_I2C_ADDR    0x90
#define PS5260_2L_ADDR_BYTE   1
#define PS5260_2L_DATA_BYTE   1
#define ps5260_2l_sensor_get_ctx(dev, pstCtx)   ((pstCtx) = ps5260_2l_get_ctx(dev))

#define PS5260_FULL_LINES_MAX 0xFFFF
#define PS5260_FULL_LINES_MAX_BUILDINWDR 0xFFFF  /* considering the YOUT_SIZE and bad frame */

#define PS5260_INCREASE_LINES                  1 /* make real fps less than stand fps because NVR require */
#define PS5260_VMAX_1080P30_LINEAR             (1124 + PS5260_INCREASE_LINES)
#define PS5260_VMAX_1080P30_BuiltinWDR         (1124 + PS5260_INCREASE_LINES)

#ifndef clip3
#define clip3(x, a, b) ((x > a) ? ((x < b) ? x : b) : a)
#endif

ISP_SNS_STATE_S *ps5260_2l_get_ctx(VI_PIPE vi_pipe);
ISP_SNS_COMMBUS_U *ps5260_2l_get_bus_Info(VI_PIPE vi_pipe);
HI_U32 *g_sensor_const1_ctx(VI_PIPE vi_pipe);

typedef enum {
    PS5260_SENSOR_1080P_30FPS_LINEAR_MODE = 0,
    PS5260_SENSOR_1080P_30FPS_BuiltinWDR_MODE,
    ps5260_MODE_BUTT
} ps5260_res_mode;

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
} ps5260_2l_video_mode_tbl;

void ps5260_2l_init(VI_PIPE vi_pipe);
void ps5260_2l_exit(VI_PIPE vi_pipe);
void ps5260_2l_standby(VI_PIPE vi_pipe);
void ps5260_2l_restart(VI_PIPE vi_pipe);
int  ps5260_2l_write_register(VI_PIPE vi_pipe, HI_U32 addr, HI_U32 data);
int  ps5260_2l_read_register(VI_PIPE vi_pipe, HI_U32 addr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* __PS5260_2l_CMOS_H_ */
