/*
 * IMX675 (Sony Starvis 2, 5M) — CV610 driver constants.
 *
 * Sensor registers, geometry, timing and bit depth are EXTRACTED from the
 * K662C6S firmware's own IMX675 bring-up (xipc, M_SENSOR_TYPE IMX675 / scene
 * layer): two linear modes — 5M 2592x1944 all-pixel and 4M 2592x1520 centred
 * vertical crop — both 30fps, 10-bit, VMAX 2200, HMAX 1125, INCK 37.125 MHz.
 * Register map confirmed against the Sony IMX675 Standard Register Setting.
 *
 * The firmware ships NO MPP cmos driver for IMX675 (only a vendor scene-tuning
 * layer), so the AE/gain model, AWB and ISP tuning are REUSED from the sibling
 * Sony Starvis-2 IMX664 driver (fully extracted from xipc) — identical register
 * map (SHR0 0x3050, GAIN 0x3070, VMAX 0x3028, FDG 0x3030) and 0.3dB HCG gain.
 */
#ifndef IMX675_CMOS_H
#define IMX675_CMOS_H

#include "ot_common_isp.h"
#include "ot_sns_ctrl.h"
#include "securec.h"
#include "ot_math.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define IMX675_ID              675

/* default (primary) geometry = 5M all-pixel */
#define IMX675_WIDTH           2592
#define IMX675_HEIGHT          1944
/* 4M centred vertical crop (PIX_VST 212, PIX_VWIDTH 1520) */
#define IMX675_WIDTH_4M        2592
#define IMX675_HEIGHT_4M       1520

#define IMX675_I2C_ADDR        0x34
#define IMX675_ADDR_BYTE       2
#define IMX675_DATA_BYTE       1

/* register map (Starvis-2) — confirmed from xipc init + Sony IMX675 register map */
#define IMX675_SHR0_ADDR       0x3050   /* SHR0 [19:0] L/M/H */
#define IMX675_GAIN_ADDR       0x3070   /* GAIN_0 [10:0] L/H */
#define IMX675_VMAX_ADDR       0x3028
#define IMX675_VMAX_ADDR_L     0x3028
#define IMX675_VMAX_ADDR_M     0x3029
#define IMX675_VMAX_ADDR_H     0x302A
#define IMX675_FDG_ADDR        0x3030   /* FDG_SEL0 [1:0] conversion-gain (HCG/LCG) */
#define IMX675_HREVERSE_ADDR   0x3020  /* bit 0: horizontal mirror */
#define IMX675_VREVERSE_ADDR   0x3021  /* bit 0: vertical flip */

/* timing — VMAX/HMAX from xipc IMX675 init (0x3028=0x0898, 0x302C=0x0465) */
#define IMX675_FULL_LINES_MAX  0xFFFFF
#define IMX675_VMAX_30FPS      2200     /* both 5M and 4M run VMAX 2200 @ 30fps */
#define IMX675_FPS_30          30
#define IMX675_FPS_MIN_LINEAR  0.1

/* AE: shared 0.3dB Sony gain ladder; HCG split at again idx>99 (Starvis-2 / IMX664) */
#define IMX675_AGAIN_MIN       1024
#define IMX675_AGAIN_MAX       188495
#define IMX675_DGAIN_MIN       1024
#define IMX675_DGAIN_MAX       128914
#define IMX675_AGAIN_TBL_RANGE 150
#define IMX675_DGAIN_TBL_RANGE 140
#define IMX675_AD_GAIN_TBL_RANGE 241
#define IMX675_HCG_THRESHOLD   99
#define IMX675_HCG_OFFSET      51

#define ISP_DGAIN_SHIFT        8
#define ISP_DGAIN_TARGET_MIN   1
#define ISP_DGAIN_TARGET_MAX   2

#define INIT_EXP_DEFAULT_LINEAR 76151
#define MAX_INT_TIME_TARGET    0xFFFFF
#define AE_COMPENSATION_DEFAULT 0x38
#define FL_OFFSET_LINEAR       2        /* max_int_time = VMAX - 2 */
#define SHR0_MAX_OFFSET        1        /* SHR0 clamp upper = VMAX - 1 */
#define MIN_INT_TIME           9
#define SHR0_MIN               6        /* SHR0 clamp [6, VMAX-1] (Sony min 6) */

#define STANDARD_FPS           30

/* black level / DNG — IMX675 is 10-bit (ADBIT 0x3022=0) in both modes.
 * The CV610 ISP black level is NOT scaled to the sensor's output bit depth:
 * all CV610-native Sony drivers (IMX335/662/664) use 0x320, and the firmware's
 * IMX662 10-bit 90fps *linear* mode also returns 0x320 (sub_AF2FC is gated on
 * wdr_mode, not bit depth). So a 10-bit Sony sensor uses the same 0x320 as the
 * 12-bit ones; the earlier 0xC8 (a CV500-era sensor-domain value) was wrong. */
#define BLACK_LEVEL_DEFAULT             0x320
#define DNG_RAW_FORMAT_BIT_LINEAR       10
#define DNG_RAW_FORMAT_WHITE_LEVEL_LINEAR 1023

/* AWB — reused from sibling Starvis-2 IMX664 (no IMX675 MPP AWB in firmware) */
#define CALIBRATE_STATIC_TEMP       4950
#define CALIBRATE_STATIC_WB_R_GAIN  472
#define CALIBRATE_STATIC_WB_GR_GAIN 256
#define CALIBRATE_STATIC_WB_GB_GAIN 256
#define CALIBRATE_STATIC_WB_B_GAIN  561
#define CALIBRATE_AWB_P1 (-50)
#define CALIBRATE_AWB_P2 306
#define CALIBRATE_AWB_Q1 0
#define CALIBRATE_AWB_A1 192002
#define CALIBRATE_AWB_B1 128
#define CALIBRATE_AWB_C1 (-138549)
#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0

#define high_8bits(x)   (((x) & 0xff00) >> 8)
#define low_8bits(x)    ((x) & 0x00ff)
#ifndef clip3
#define clip3(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#endif

/* smaller mode first so cmos_set_image_mode's height<= match is unambiguous */
typedef enum {
    IMX675_4M_30FPS_10BIT_LINEAR_MODE = 0,
    IMX675_5M_30FPS_10BIT_LINEAR_MODE,
    IMX675_MODE_MAX
} imx675_res_mode;

/* linear reg-sync slots: SHR0(L/M/H), GAIN(L/H), VMAX(L/M/H), FDG = 9 */
typedef enum {
    SHR0_L_IDX = 0,
    SHR0_M_IDX,
    SHR0_H_IDX,
    GAIN_L_IDX,
    GAIN_H_IDX,
    VMAX_L_IDX,
    VMAX_M_IDX,
    VMAX_H_IDX,
    FDG_IDX
} imx675_reg_idx;

ot_isp_sns_obj *imx675_get_obj(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* IMX675_CMOS_H */
