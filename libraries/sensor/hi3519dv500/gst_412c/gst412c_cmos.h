/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef GST412C_CMOS_H
#define GST412C_CMOS_H

#include "ot_common_isp.h"
#include "ot_sns_ctrl.h"
#include "securec.h"
#include "ot_math.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define GST412C_I2C_ADDR    0x84
#define GST412C_ADDR_BYTE   1
#define GST412C_DATA_BYTE   1
#define gst412c_sensor_get_ctx(dev, ctx)   ((ctx) = gst412c_get_ctx(dev))

#define GST412C_FULL_LINES_MAX 0x2610
#define GST412C_FULL_LINES_MAX_2TO1_WDR 0x1380
#define GST412C_INCREASE_LINES 0  /* make real fps less than stand fps because NVR require */
#define GST412C_VMAX_4M_30FPS_12BIT_LINEAR     (1624 + GST412C_INCREASE_LINES)
#define GST412C_VMAX_4M_30FPS_12BIT_2TO1_WDR   (1624 + GST412C_INCREASE_LINES)   /* 12bit */

typedef enum {
    GST412C_4M_30FPS_12BIT_LINEAR_MODE = 0,
    GST412C_MODE_BUTT
} gst412c_res_mode;

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
} gst412c_video_mode_tbl;


ot_isp_sns_state *gst412c_get_ctx(ot_vi_pipe vi_pipe);
ot_isp_sns_commbus *gst412c_get_bus_info(ot_vi_pipe vi_pipe);

td_void gst412c_init(ot_vi_pipe vi_pipe);
td_void gst412c_exit(ot_vi_pipe vi_pipe);
td_void gst412c_standby(ot_vi_pipe vi_pipe);
td_void gst412c_restart(ot_vi_pipe vi_pipe);
td_s32  gst412c_write_register(ot_vi_pipe vi_pipe, td_u32 addr, td_u32 data);
td_s32  gst412c_read_register(ot_vi_pipe vi_pipe, td_u32 addr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* GST412C_CMOS_H */
