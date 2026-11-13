/*
 * IMX664 (Sony Starvis 2, 4M) — CV610 driver constants.
 * All sensor/AE/AWB values EXTRACTED from the firmware's own IMX664 driver
 * (xipc, type 284). Linear 4M30 12-bit. Same Starvis-2 register map and HCG
 * gain as IMX662; SHR0 clamp [6, VMAX-1].
 */
#ifndef IMX664_CMOS_H
#define IMX664_CMOS_H

#include "ot_common_isp.h"
#include "ot_sns_ctrl.h"
#include "securec.h"
#include "ot_math.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define IMX664_ID              664
#define IMX664_WIDTH           2688
#define IMX664_HEIGHT          1520

#define IMX664_I2C_ADDR        0x34
#define IMX664_ADDR_BYTE       2
#define IMX664_DATA_BYTE       1

/* register map (Starvis-2) — from cmos_get_sns_reg_info (sub_B1094) */
#define IMX664_SHR0_ADDR       0x3050
#define IMX664_GAIN_ADDR       0x3070
#define IMX664_VMAX_ADDR       0x3028
#define IMX664_VMAX_ADDR_L     0x3028
#define IMX664_VMAX_ADDR_M     0x3029
#define IMX664_VMAX_ADDR_H     0x302A
#define IMX664_FDG_ADDR        0x3030   /* conversion-gain (HCG/LCG) */
#define IMX664_HREVERSE_ADDR   0x3020  /* bit 0: horizontal mirror */
#define IMX664_VREVERSE_ADDR   0x3021  /* bit 0: vertical flip */

/* timing — VMAX from cmos_sns_global_init (sub_B14E0) */
#define IMX664_FULL_LINES_MAX  0xFFFFF
#define IMX664_VMAX_4M_30FPS   1651
#define IMX664_FPS_30          30
#define IMX664_FPS_MIN_LINEAR  0.1

/* AE: shared 0.3dB Sony gain ladder; HCG split at again idx>99 */
#define IMX664_AGAIN_MIN       1024
#define IMX664_AGAIN_MAX       188495
#define IMX664_DGAIN_MIN       1024
#define IMX664_DGAIN_MAX       128914
#define IMX664_AGAIN_TBL_RANGE 150
#define IMX664_DGAIN_TBL_RANGE 140
#define IMX664_AD_GAIN_TBL_RANGE 241
#define IMX664_HCG_THRESHOLD   99
#define IMX664_HCG_OFFSET      51

#define ISP_DGAIN_SHIFT        8
#define ISP_DGAIN_TARGET_MIN   1
#define ISP_DGAIN_TARGET_MAX   2

#define INIT_EXP_DEFAULT_LINEAR 76151
#define MAX_INT_TIME_TARGET    0xFFFFF
#define AE_COMPENSATION_DEFAULT 0x38
#define FL_OFFSET_LINEAR       2        /* max_int_time = VMAX - 2 */
#define SHR0_MAX_OFFSET        1        /* SHR0 clamp upper = VMAX - 1 */
#define MIN_INT_TIME           9
#define SHR0_MIN               6        /* SHR0 clamp [6, VMAX-1] */

#define STANDARD_FPS           30

/* black level / DNG — IMX664 12-bit linear; black 0x320 */
#define BLACK_LEVEL_DEFAULT             0x320
#define DNG_RAW_FORMAT_BIT_LINEAR       12
#define DNG_RAW_FORMAT_WHITE_LEVEL_LINEAR 4095

/* AWB — IMX664's real values from cmos_get_awb_default (sub_B18D0) */
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

typedef enum {
    IMX664_4M_30FPS_12BIT_LINEAR_MODE = 0,
    IMX664_MODE_MAX
} imx664_res_mode;

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
} imx664_reg_idx;

ot_isp_sns_obj *imx664_get_obj(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
