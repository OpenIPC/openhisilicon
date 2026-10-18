/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef IMX335_CMOS_H
#define IMX335_CMOS_H

#include "ot_common_isp.h"
#include "ot_sns_ctrl.h"
#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/****************************************************************************
 * basic info config                                                        *
 ****************************************************************************/
#define IMX335_ID              335
#define IMX335_WIDTH           2592
#define IMX335_HEIGHT          1944

/* sensor Register Address */
#define IMX335_VMAX_ADDR_L     0x3030
#define IMX335_VMAX_ADDR_M     0x3031
#define IMX335_VMAX_ADDR_H     0x3032
/* HMAX (13-bit: low 8 + high 5) carries the frame rate in crop modes. */
#define IMX335_HMAX_ADDR_L     0x3034
#define IMX335_HMAX_ADDR_H     0x3035
#define IMX335_SHR0_LOW        0x3058
#define IMX335_SHR0_MIDDLE     0x3059
#define IMX335_SHR0_HIGH       0x305A
#define IMX335_GAIN_LONG_LOW   0x30E8
#define IMX335_GAIN_LONG_HIGH  0x30E9

/* The widest frame length VMAX (20-bit) can hold, so the longest exposure. */
#define IMX335_FULL_LINES_MAX  0xFFFFC

/* VMAX for 2592x1944 @ 30 fps (sensor default). */
#define IMX335_VMAX_5M_30FPS_12BIT_LINEAR  (0x1194)

/* VMAX for 1920x1080 cropped @ 90 fps (ev200 IMX335_cropped_60fps_1080p_init).
 * cmos_fps_set scales it: VMAX = 0x08F8 * 90 / fps. */
#define IMX335_VMAX_1080P_CROP_10BIT       (0x08F8)

/****************************************************************************
 * i2c bus config                                                           *
 ****************************************************************************/
#define IMX335_I2C_ADDR    0x34
#define IMX335_ADDR_BYTE   2
#define IMX335_DATA_BYTE   1

/* common I2C bus config */
#define I2C_DEV_FILE_NUM   16
#define I2C_BUF_NUM        8

/****************************************************************************
 * sensor lines / mode configs                                              *
 ****************************************************************************/
#define IMX335_FPS_MAX_LINEAR   30
#define IMX335_FPS_MIN_LINEAR   0.02
#define IMX335_WIDTH_LINEAR     2592
#define IMX335_HEIGHT_LINEAR    1944

/* 1920x1080 crop, 10-bit, 4-lane, up to 90 fps.
 * The crop's VMAX is fixed at 0x08F8 and the rate is carried by HMAX, whose
 * 13-bit field caps the slowest expressible rate at 0x16E * 90 / 8191 = 4.02
 * fps; the floor is rounded up so a request always maps to a valid HMAX. */
#define IMX335_FPS_MAX_CROP     90
#define IMX335_FPS_MIN_CROP     4.03
#define IMX335_WIDTH_CROP       1920
#define IMX335_HEIGHT_CROP      1080
/* Table HMAX for the 1080p crop at IMX335_FPS_MAX_CROP. */
#define IMX335_HMAX_1080P_CROP  (0x016E)

/* 1296x972 2x2 binning 10-bit (ev200 binning_60pfs_init). */
#define IMX335_FPS_MAX_BINNING  76
#define IMX335_FPS_MIN_BINNING  2.0
#define IMX335_WIDTH_BINNING    1296
#define IMX335_HEIGHT_BINNING   972
#define IMX335_VMAX_BINNING      (0x0DE8)

/* 2592x1944 10-bit full-size (ev200 linear_5M30_12bit_40fps_init). */
#define IMX335_FPS_MAX_FULL     40
#define IMX335_FPS_MIN_FULL     2.0
#define IMX335_WIDTH_FULL       2592
#define IMX335_HEIGHT_FULL      1944
#define IMX335_VMAX_FULL_10BIT  (0x1194)

/* 2592x1520 10-bit cropped, 16:9 (ev200 IMX335_cropped_41fps_2592_1520_init).
 * Like the other crop modes, VMAX stays at the window's frame length (0x0C58)
 * and the frame rate is carried by HMAX = HMAX_30 * 30 / fps, so the table HMAX
 * 0x030F (783) is scaled rather than VMAX. The reported minimum is the slowest
 * rate the 13-bit HMAX can express at this VMAX: 74.25MHz/(8191*0x0C58) = 2.87
 * fps, rounded up. */
#define IMX335_FPS_MAX_1520P    30
#define IMX335_FPS_MIN_1520P    2.87
#define IMX335_WIDTH_1520P      2592
#define IMX335_HEIGHT_1520P     1520
#define IMX335_VMAX_CROPPED_1520P  (0x0C58)

/* Flexible window-crop linear mode (enum 5): arbitrary W×H, linear 10-bit.
 * The crop registers are computed in imx335_flex_reg_init() from the per-pipe
 * crop set by cmos_isp_set_image_mode(); the datasheet constraints and the
 * register derivation are documented there.
 *
 * The T6 VI requires the crop-mode VMAX to stay at the window's own frame
 * length (scaling it from the requested fps stalls the pipeline), so VMAX is
 * fixed at max(VMAX_BASE, AREA3_WIDTH_1 + 96) and the frame rate is set with
 * HMAX instead: HMAX = round(FLEX_CLK / (fps * VMAX)). VMAX_BASE reuses the
 * 1080p crop table's frame length, which the flex init replays as its base. */
#define IMX335_FLEX_CLK            74250000
#define IMX335_FLEX_VMAX_BASE      (0x08F8)
#define IMX335_FLEX_HMAX_MIN       64
#define IMX335_FLEX_HMAX_MAX       8191
#define IMX335_CROP_W_DEFAULT      1920
#define IMX335_CROP_H_DEFAULT      1080
#define IMX335_FPS_MAX_FLEX        240
#define IMX335_FPS_MIN_FLEX        2.0
/* Flex VMAX for a crop height: the window's own frame length, which the T6 VI
 * requires to stay put. */
#define IMX335_FLEX_VMAX(crop_h) \
    (((((crop_h) + 20) * 2 + 96) > IMX335_FLEX_VMAX_BASE) ? \
     (((crop_h) + 20) * 2 + 96) : IMX335_FLEX_VMAX_BASE)
/* Slowest rate the 13-bit HMAX can express at a given fixed VMAX. */
#define IMX335_FLEX_MIN_FPS(vmax) \
    ((td_float)IMX335_FLEX_CLK / ((td_float)IMX335_FLEX_HMAX_MAX * (td_float)(vmax)))
/* Table HMAX for the 1520p crop at IMX335_FPS_MAX_1520P (min == max == 30). */
#define IMX335_HMAX_1520P           (0x030F)

/****************************************************************************
 * sensor ae configs                                                        *
 ****************************************************************************/
#define IMX335_AGAIN_MIN    1024
#define IMX335_AGAIN_MAX    32381
#define IMX335_DGAIN_MIN    1024
#define IMX335_DGAIN_MAX    128914

#define IMX335_AD_GAIN_TBL_RANGE   241
#define IMX335_AGAIN_TBL_RANGE     100
#define IMX335_DGAIN_TBL_RANGE     140

#define ISP_DGAIN_SHIFT            8
#define ISP_DGAIN_TARGET_MIN       1
#define ISP_DGAIN_TARGET_MAX       2

#define INT_TIME_ACCURACY      1
#define AGAIN_ACCURACY         1
#define DGAIN_ACCURACY         1

#define FL_OFFSET_LINEAR       8
#define STANDARD_FPS           30
#define FLICKER_FREQ           0
#define INIT_EXP_DEFAULT_LINEAR   130000
#define MAX_INT_TIME_TARGET        IMX335_FULL_LINES_MAX
#define AE_COMENSATION_DEFAULT    0x38
/* T6 ISP black level is 14-bit (ot_isp_black_level_manual_attr, Format:14.0).
 * The sensor BLKLEVEL 0x32 is 50 in 10-bit output and 200 in 12-bit output,
 * i.e. the same 14-bit pedestal 50<<4 = 200<<2 = 800. The ev200 driver's 200
 * is the raw 12-bit pedestal, not the 14-bit value T6 expects. */
#define BLACK_LEVEL_DEFAULT       0x320
/* DNG */
#define DNG_RAW_FORMAT_BIT_LINEAR         12
#define DNG_RAW_FORMAT_WHITE_LEVEL_LINEAR 4095
#define DNG_RAW_FORMAT_BIT_CROP           10
#define DNG_RAW_FORMAT_WHITE_LEVEL_CROP   1023

/****************************************************************************
 * sensor awb calibrate configs (from OpenIPC ev200 imx335 driver)          *
 ****************************************************************************/
#define CALIBRATE_STATIC_TEMP       4900
#define CALIBRATE_STATIC_WB_R_GAIN  0x1E3
#define CALIBRATE_STATIC_WB_GR_GAIN 0x100
#define CALIBRATE_STATIC_WB_GB_GAIN 0x100
#define CALIBRATE_STATIC_WB_B_GAIN  0x1D1

#define CALIBRATE_AWB_P1 (-0x0012)
#define CALIBRATE_AWB_P2 0x010B
#define CALIBRATE_AWB_Q1 (-0x0007)
#define CALIBRATE_AWB_A1 0x2711F
#define CALIBRATE_AWB_B1 0x0080
#define CALIBRATE_AWB_C1 (-0x1A5C1)

#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0

/****************************************************************************
 * assist function macros                                                   *
 ****************************************************************************/
#define imx335_low_8bits(x)    ((x) & 0x00FF)
#define imx335_mid_8bits(x)    (((x) & 0x0FF00) >> 8)
#define imx335_high_4bits(x)   (((x) & 0xF0000) >> 16)
#ifndef clip3
#define clip3(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#endif

/****************************************************************************
 * sensor data type defines                                                 *
 ****************************************************************************/
typedef enum {
    IMX335_5M_30FPS_12BIT_LINEAR_MODE = 0,   /* 2592x1944 @ 30, 12-bit linear */
    IMX335_60FPS_BINNING_MODE = 1,           /* 1296x972 @ ~60, 10-bit 2x2 binning */
    IMX335_60FPS_CROPPED_1080P_MODE = 2,     /* 1920x1080, 10-bit crop, up to 90 fps */
    IMX335_60FPS_FULL_1944P_MODE = 3,        /* 2592x1944 @ ~40, 10-bit full */
    IMX335_1520P_10BIT_MODE = 4,             /* 2592x1520, 10-bit linear crop */
    IMX335_CROP_FLEX_LINEAR_MODE = 5,        /* arbitrary W×H, 10-bit linear crop */
    IMX335_MODE_MAX
} imx335_res_mode;

typedef enum {
    IMX335_SHR0_L_IDX = 0,
    IMX335_SHR0_M_IDX,
    IMX335_SHR0_H_IDX,
    IMX335_GAIN_L_IDX,
    IMX335_GAIN_H_IDX,
    IMX335_VMAX_L_IDX,
    IMX335_VMAX_M_IDX,
    IMX335_VMAX_H_IDX,
    IMX335_REG_MAX_IDX
} imx335_linear_reg_index;

td_s32 imx335_init(cis_info *cis);
td_s32 imx335_get_standby_cfg(ot_isp_sns_regs_info *standby_cfg);

/* Per-pipe crop window for IMX335_CROP_FLEX_LINEAR_MODE, in pixels. Set from
 * the requested image mode (Isp_W/Isp_H) and read back when programming the
 * crop registers. */
td_void imx335_set_crop(ot_vi_pipe vi_pipe, td_u32 width, td_u32 height);
td_void imx335_get_crop(ot_vi_pipe vi_pipe, td_u32 *width, td_u32 *height);
/* Requested frame rate for the flex crop, used to derive HMAX at init. */
td_void imx335_set_flex_fps(ot_vi_pipe vi_pipe, td_float fps);
td_float imx335_get_flex_fps(ot_vi_pipe vi_pipe);
/* Re-apply the HMAX cached by cmos_fps_set() for a fixed crop mode after the
 * mode table has been written (in case the write happened before I2C was up). */
td_void imx335_apply_crop_hmax(cis_info *cis);

ot_isp_sns_obj *imx335_get_obj(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* IMX335_CMOS_H */
