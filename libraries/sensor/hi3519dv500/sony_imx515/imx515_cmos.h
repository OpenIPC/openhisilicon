/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef IMX515_CMOS_H
#define IMX515_CMOS_H

#include "ot_common.h"
#include "ot_common_isp.h"
#include "ot_common_video.h"
#include "ot_sns_ctrl.h"
#include "ot_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define IMX515_I2C_ADDR    0x34
#define IMX515_ADDR_BYTE   2
#define IMX515_DATA_BYTE   1
#define imx515_sensor_get_ctx(pipe, ctx)   ((ctx) = imx515_get_ctx(pipe))

#define IMX515_FULL_LINES_MAX 0xFFFFF

#define IMX515_INCREASE_LINES  1  /* make real fps less than stand fps because NVR require */
#define IMX515_VMAX_4K2K_LINEAR    (3300 + IMX515_INCREASE_LINES)

typedef enum {
    IMX515_SENSOR_8M_30FPS_LINEAR_MODE = 0,
    IMX515_MODE_BUTT
} imx515_res_mode;

typedef struct {
    td_u32      ver_lines;
    td_u32      max_ver_lines;
    td_float    max_fps;
    td_float    min_fps;
    td_u32      width;
    td_u32      height;
    td_u8       sns_mode;
    ot_wdr_mode wdr_mode;
    const char *mode_name;
} imx515_video_mode_tbl;

ot_isp_sns_state *imx515_get_ctx(ot_vi_pipe vi_pipe);
ot_isp_sns_commbus *imx515_get_bus_info(ot_vi_pipe vi_pipe);

td_void imx515_init(ot_vi_pipe vi_pipe);
td_void imx515_exit(ot_vi_pipe vi_pipe);
td_void imx515_standby(ot_vi_pipe vi_pipe);
td_void imx515_restart(ot_vi_pipe vi_pipe);
td_s32  imx515_write_register(ot_vi_pipe vi_pipe, td_u32 addr, td_u32 data);
td_s32  imx515_read_register(ot_vi_pipe vi_pipe, td_u32 addr);
td_void imx515_blc_clamp(ot_vi_pipe vi_pipe, ot_isp_sns_blc_clamp blc_clamp);
td_void imx515_set_blc_clamp_value(ot_vi_pipe vi_pipe, td_bool clamp_en);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* IMX515_CMOS_H */

