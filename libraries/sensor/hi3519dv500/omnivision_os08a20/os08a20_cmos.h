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
/* 2x2 binning: 1157 lines is 120 fps (measured clean); 60 fps doubles them */
#define OS08A20_VMAX_2M_60FPS_10BIT_LINEAR     (1157 + OS08A20_INCREASE_LINES)

typedef enum {
    OS08A20_8M_30FPS_12BIT_LINEAR_MODE = 0,
    OS08A20_8M_30FPS_10BIT_2TO1_VC_MODE,
    OS08A20_2M_60FPS_10BIT_LINEAR_MODE,
    OS08A20_FLEX_12BIT_LINEAR_MODE,      /* sns_mode 4: a window of native pixels */
    OS08A20_FLEX_BIN_10BIT_LINEAR_MODE,  /* sns_mode 5: a window of the 2x2-binned array */
    OS08A20_MODE_BUTT
} os08a20_res_mode;

/*
 * Flexible crop: the sensor reads only a centred W x H window, and the frame
 * is as short as its rows allow, so the rate rises as the window shrinks.
 * Selected by sns_mode (the profile's Isp_SnsMode) 4 or 5 with W x H in the
 * image mode's size. Rate ceiling: PCLK / (HTS x (H + VBLANK)).
 *
 * Measured on a Hi3519DV500 (a static scene in a dim room, so at high gain),
 * each a whole picture checked against the same window at a long frame:
 *   - Line, native 12-bit: the 4K30 mode's 2072 shortens to 1400 clean, at
 *     full width too; 1300 streams a white picture; 1200 gives no frame.
 *     1440 is kept, 10.0 us.
 *   - Line, binned 10-bit: 1000 is clean, 960 turns blue, 900 purple, 860
 *     gives no frame. The vendor's 1036 (7.2 us) is kept: 3.5% is no margin.
 *   - Frame: it must run some 56 rows past the window. Binned (window 8 rows
 *     taller than the output) broke at a blank of 60 and was clean at 64;
 *     native (window 32 taller) broke at 80 and was clean at 90. Both keep 64
 *     past the window.
 *   - A native window too tall to start 24 rows above its picture (the
 *     picture's first row is 12 into the array) is clamped to the array's
 *     edge, and needs more: 3840x2160 broke at a blank of 96 and was clean at
 *     110. Such windows get OS08A20_FLEX_EDGE_VBLANK more; 3840x2048, which
 *     is not clamped, ran clean at 96 at the same pixel rate.
 */
#define OS08A20_SNS_MODE_FLEX      4
#define OS08A20_SNS_MODE_FLEX_BIN  5
#define OS08A20_PCLK               143838240 /* HTS x VTS x fps: 2072 x 2314 x 30 */
#define OS08A20_FLEX_HTS           0x5a0     /* 1440, native 12-bit */
#define OS08A20_FLEX_BIN_HTS       0x40c     /* 1036, binned 10-bit */
#define OS08A20_FLEX_VBLANK        (32 + 64) /* native: window extra rows + 64 */
#define OS08A20_FLEX_BIN_VBLANK    (8 + 64)  /* binned: window extra rows + 64 */
#define OS08A20_FLEX_EDGE_VBLANK   16
#define OS08A20_FLEX_EDGE_HEIGHT   2136      /* native: taller windows reach row 0 */
#define OS08A20_FLEX_MIN_W         320
#define OS08A20_FLEX_MIN_H         240

typedef struct {
    td_u32 width;
    td_u32 height;
    td_u32 hts;
    td_u32 vts;      /* the shortest frame: the window's rows plus the blank */
} os08a20_flex;

const os08a20_flex *os08a20_get_flex(ot_vi_pipe vi_pipe);

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
