/*
 * IMX415 (Sony Starvis, 8M / 4K) — CV610 driver constants.
 *
 * IMX415 is the K662C6S board's DEFAULT sensor (sns0=imx415,sns1=imx415).
 * Sensor registers, geometry, timing and bit depth are EXTRACTED from the
 * firmware's own IMX415 bring-up (xipc): two linear modes — 8M 3840x2160
 * all-pixel and 2M 1920x1080 2x2 binning — both 30fps, 10-bit, VMAX 2250.
 *
 * The firmware ships NO MPP cmos driver for IMX415 (only a vendor scene
 * layer). The register map, exposure/gain model and AWB come from the
 * authoritative Hi3516CV500 IMX415 reference driver (validated against the
 * IMX415 datasheet). IMX415 is original Starvis (gen-1): single analog-gain
 * path, NO HCG/conversion-gain — the gain/AE model matches IMX335, not the
 * Starvis-2 sensors. ISP tuning is reused from the gen-1 Sony IMX335 baseline.
 */
#ifndef IMX415_CMOS_H
#define IMX415_CMOS_H

#include "ot_common_isp.h"
#include "ot_sns_ctrl.h"
#include "securec.h"
#include "ot_math.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define IMX415_ID              415

/* default (primary) geometry = 8M all-pixel 4K */
#define IMX415_WIDTH           3840
#define IMX415_HEIGHT          2160
/* 2M 2x2 binning */
#define IMX415_WIDTH_2M        1920
#define IMX415_HEIGHT_2M       1080

#define IMX415_I2C_ADDR        0x34
#define IMX415_ADDR_BYTE       2
#define IMX415_DATA_BYTE       1

/* register map (gen-1 Starvis) — from Hi3516CV500 IMX415 driver + xipc init */
#define IMX415_SHR0_ADDR       0x3050   /* SHR0 [19:0] L/M/H */
#define IMX415_GAIN_ADDR       0x3090   /* GAIN_LONG [10:0] L/H (no conversion gain) */
#define IMX415_VMAX_ADDR       0x3024
#define IMX415_VMAX_ADDR_L     0x3024
#define IMX415_VMAX_ADDR_M     0x3025
#define IMX415_VMAX_ADDR_H     0x3026
#define IMX415_REVERSE_ADDR    0x3030   /* readout inversion */
#define IMX415_HREVERSE_BIT    0x01     /* [0] HREVERSE: mirror */
#define IMX415_VREVERSE_BIT    0x02     /* [1] VREVERSE: flip */

/* timing — VMAX/HMAX from xipc IMX415 init (0x3024=0x08CA, 0x3028=0x044C) */
#define IMX415_FULL_LINES_MAX  0xFFFFF
#define IMX415_VMAX_30FPS      2250     /* both 8M and 2M-binning run VMAX 2250 @ 30fps */
#define IMX415_FPS_30          30
#define IMX415_FPS_MIN_LINEAR  0.1

/* AE: shared 0.3dB Sony gain ladder (gen-1: again idx 0..100, no HCG/FDG) */
#define IMX415_AGAIN_MIN       1024
#define IMX415_AGAIN_MAX       32381
#define IMX415_DGAIN_MIN       1024
#define IMX415_DGAIN_MAX       128914
#define IMX415_AGAIN_TBL_RANGE 100
#define IMX415_DGAIN_TBL_RANGE 140
#define IMX415_AD_GAIN_TBL_RANGE 241

#define ISP_DGAIN_SHIFT        8
#define ISP_DGAIN_TARGET_MIN   1
#define ISP_DGAIN_TARGET_MAX   2

#define INIT_EXP_DEFAULT_LINEAR 130000
#define MAX_INT_TIME_TARGET    65535
#define AE_COMPENSATION_DEFAULT 0x2D
#define FL_OFFSET_LINEAR       9        /* max_int_time = VMAX - SHR0_MIN */
#define SHR0_MAX_OFFSET        1        /* SHR0 clamp upper = VMAX - 1 */
#define MIN_INT_TIME           2        /* ExpLineMin */
#define SHR0_MIN               9        /* SHR0 clamp [9, VMAX-1] */

#define STANDARD_FPS           30

/* black level / DNG — IMX415 is 10-bit (ADBIT 0x3031=0) in both modes.
 * The CV610 ISP black level is NOT scaled to the sensor's output bit depth:
 * all CV610-native Sony drivers (IMX335/662/664) use 0x320, and the firmware's
 * IMX662 10-bit linear mode also returns 0x320 (sub_AF2FC, gated on wdr_mode).
 * The Hi3516CV500 IMX415 driver's 0xC8 is a different-generation ISP value and
 * does not apply to CV610; use the CV610 Sony-family 0x320. */
#define BLACK_LEVEL_DEFAULT             0x320
#define DNG_RAW_FORMAT_BIT_LINEAR       10
#define DNG_RAW_FORMAT_WHITE_LEVEL_LINEAR 1023

/* AWB — IMX415's real values from the Hi3516CV500 IMX415 reference driver */
#define CALIBRATE_STATIC_TEMP       4983
#define CALIBRATE_STATIC_WB_R_GAIN  440
#define CALIBRATE_STATIC_WB_GR_GAIN 256
#define CALIBRATE_STATIC_WB_GB_GAIN 256
#define CALIBRATE_STATIC_WB_B_GAIN  559
#define CALIBRATE_AWB_P1 8
#define CALIBRATE_AWB_P2 217
#define CALIBRATE_AWB_Q1 (-31)
#define CALIBRATE_AWB_A1 196503
#define CALIBRATE_AWB_B1 128
#define CALIBRATE_AWB_C1 (-144504)
#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0

#define high_8bits(x)   (((x) & 0xff00) >> 8)
#define low_8bits(x)    ((x) & 0x00ff)
#ifndef clip3
#define clip3(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#endif

/* smaller mode first so cmos_set_image_mode's height<= match is unambiguous */
typedef enum {
    IMX415_2M_BINNING_30FPS_10BIT_LINEAR_MODE = 0,
    IMX415_8M_30FPS_10BIT_LINEAR_MODE,
    IMX415_MODE_MAX
} imx415_res_mode;

/* linear reg-sync slots: SHR0(L/M/H), GAIN(L/H), VMAX(L/M/H) = 8 (no FDG) */
typedef enum {
    SHR0_L_IDX = 0,
    SHR0_M_IDX,
    SHR0_H_IDX,
    GAIN_L_IDX,
    GAIN_H_IDX,
    VMAX_L_IDX,
    VMAX_M_IDX,
    VMAX_H_IDX
} imx415_reg_idx;

ot_isp_sns_obj *imx415_get_obj(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* IMX415_CMOS_H */
