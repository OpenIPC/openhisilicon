/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef OS08A20_CMOS_H
#define OS08A20_CMOS_H

#include "ot_common_isp.h"
#include "ot_sns_ctrl.h"
#include "securec.h"
#include "ot_common_video.h"
#include "ot_math.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifndef clip3
#define clip3(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#endif


#define OS08A20_I2C_ADDR    0x6c
#define OS08A20_ADDR_BYTE   2
#define OS08A20_DATA_BYTE   1
#define os08a20_sensor_get_ctx(pipe, ctx)   ((ctx) = os08a20_get_ctx(pipe))

#define OS08A20_FULL_LINES_MAX 0xFFFF
#define OS08A20_FULL_LINES_MAX_2TO1_WDR 0x363C
#define OS08A20_INCREASE_LINES 0  /* make real fps less than stand fps because NVR require */
#define OS08A20_VMAX_8M_30FPS_12BIT_LINEAR     (2314 + OS08A20_INCREASE_LINES)
#define OS08A20_VMAX_8M_30FPS_10BIT_2TO1_WDR   (2314 + OS08A20_INCREASE_LINES)   /* 10bit */

typedef enum {
    OS08A20_8M_30FPS_12BIT_LINEAR_MODE = 0,
    OS08A20_8M_30FPS_10BIT_2TO1_VC_MODE,
    OS08A20_MODE_BUTT
} os08a20_res_mode;

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
} os08a20_video_mode_tbl;

typedef enum {
    EXPO_L_IDX = 0,
    EXPO_H_IDX,
    AGAIN_L_IDX,
    AGAIN_H_IDX,
    DGAIN_L_IDX,
    DGAIN_H_IDX,
    VMAX_L_IDX,
    VMAX_H_IDX,
    REG_MAX_IDX
}os08a20_linear_reg_index;

typedef enum {
    WDR_EXPO_L_IDX = 0,
    WDR_EXPO_H_IDX,
    WDR_AGAIN_L_IDX,
    WDR_AGAIN_H_IDX,
    WDR_DGAIN_L_IDX,
    WDR_DGAIN_H_IDX,
    WDR_SHORT_EXPO_L_IDX,
    WDR_SHORT_EXPO_H_IDX,
    WDR_SHORT_AGAIN_L_IDX,
    WDR_SHORT_AGAIN_H_IDX,
    WDR_SHORT_DGAIN_L_IDX,
    WDR_SHORT_DGAIN_H_IDX,
    WDR_VMAX_L_IDX,
    WDR_VMAX_H_IDX,
    WDR_REG_MAX_IDX
}os08a20_wdr_reg_index;

typedef struct {
    td_u32 dec[OT_ISP_WDR_MAX_FRAME_NUM];
    td_u32 inc[OT_ISP_WDR_MAX_FRAME_NUM];
} time_step;
ot_isp_sns_state *os08a20_get_ctx(ot_vi_pipe vi_pipe);
ot_isp_sns_commbus *os08a20_get_bus_info(ot_vi_pipe vi_pipe);

td_void os08a20_init(ot_vi_pipe vi_pipe);
td_void os08a20_exit(ot_vi_pipe vi_pipe);
td_void os08a20_standby(ot_vi_pipe vi_pipe);
td_void os08a20_restart(ot_vi_pipe vi_pipe);
td_void os08a20_blc_clamp(ot_vi_pipe vi_pipe, ot_isp_sns_blc_clamp blc_clamp);
td_void os08a20_set_blc_clamp_value(ot_vi_pipe vi_pipe, td_bool clamp_en);

td_s32  os08a20_write_register(ot_vi_pipe vi_pipe, td_u32 addr, td_u32 data);
td_s32  os08a20_read_register(ot_vi_pipe vi_pipe, td_u32 addr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* OS08A20_CMOS_H */
