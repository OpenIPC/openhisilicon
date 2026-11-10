/*
 * IMX662 (Sony Starvis 2, 2M) — CV610 driver constants.
 * All sensor/AE/AWB values EXTRACTED from the firmware's own IMX662 driver
 * (xipc, type 272): register map, exposure/gain model, limits, tuning, AWB.
 * Linear 2M30 12-bit (xipc also has 2M60 12-bit and 2M90 10-bit modes).
 */
#ifndef IMX662_CMOS_H
#define IMX662_CMOS_H

#include "ot_common_isp.h"
#include "ot_sns_ctrl.h"
#include "securec.h"
#include "ot_math.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define IMX662_ID              662
#define IMX662_WIDTH           1920
#define IMX662_HEIGHT          1080

/* i2c: dev_addr 0x34, 16-bit reg, 8-bit data (from cmos_get_sns_reg_info) */
#define IMX662_I2C_ADDR        0x34
#define IMX662_ADDR_BYTE       2
#define IMX662_DATA_BYTE       1

/* register map (Starvis-2) — from cmos_get_sns_reg_info (sub_AF0FC) */
#define IMX662_SHR0_ADDR       0x3050  /* L=0x3050 M=0x3051 H=0x3052 (exposure) */
#define IMX662_GAIN_ADDR       0x3070  /* L=0x3070 H=0x3071 */
#define IMX662_VMAX_ADDR       0x3028  /* L=0x3028 M=0x3029 H=0x302A */
#define IMX662_VMAX_ADDR_L     0x3028
#define IMX662_VMAX_ADDR_M     0x3029
#define IMX662_VMAX_ADDR_H     0x302A
#define IMX662_FDG_ADDR        0x3030  /* conversion-gain select (HCG/LCG) */
#define IMX662_HREVERSE_ADDR   0x3020  /* bit 0: horizontal mirror */
#define IMX662_VREVERSE_ADDR   0x3021  /* bit 0: vertical flip */

/* frame length / timing — VMAX from cmos_sns_global_init (sub_AF594) */
#define IMX662_FULL_LINES_MAX  0xFFFFF
#define IMX662_VMAX_2M_30FPS   1251   /* VMAX is 1251 for all modes; fps set by HMAX */
#define IMX662_FPS_30          30
#define IMX662_FPS_60          60
#define IMX662_FPS_90          90
#define IMX662_FPS_MIN_LINEAR  0.1

/* AE: shared 0.3dB Sony gain ladder (ad_gain_table). HCG split at again idx>99. */
#define IMX662_AGAIN_MIN       1024
#define IMX662_AGAIN_MAX       188495   /* ad_gain_table[150], ~45.3dB */
#define IMX662_DGAIN_MIN       1024
#define IMX662_DGAIN_MAX       128914   /* ad_gain_table[140] */
#define IMX662_AGAIN_TBL_RANGE 150
#define IMX662_DGAIN_TBL_RANGE 140
#define IMX662_AD_GAIN_TBL_RANGE 241
#define IMX662_HCG_THRESHOLD   99       /* again idx > 99 -> HCG, subtract 51 */
#define IMX662_HCG_OFFSET      51

#define ISP_DGAIN_SHIFT        8
#define ISP_DGAIN_TARGET_MIN   1
#define ISP_DGAIN_TARGET_MAX   2

#define INIT_EXP_DEFAULT_LINEAR 76151
#define MAX_INT_TIME_TARGET    0xFFFFF
#define AE_COMPENSATION_DEFAULT 0x38
#define FL_OFFSET_LINEAR       2        /* max_int_time = VMAX - 2 */
#define MIN_INT_TIME           9
#define SHR0_MIN               3        /* SHR0 clamp [3, VMAX-2] */

#define STANDARD_FPS           30

/* black level / DNG — IMX662 12-bit linear; black 0x320 (from get_isp_black_level) */
#define BLACK_LEVEL_DEFAULT             0x320
#define DNG_RAW_FORMAT_BIT_LINEAR       12   /* 30/60fps modes */
#define DNG_RAW_FORMAT_WHITE_LEVEL_LINEAR 4095
#define DNG_RAW_FORMAT_BIT_10BIT        10   /* 90fps mode */
#define DNG_RAW_FORMAT_WHITE_LEVEL_10BIT 1023

/* AWB — IMX662's real values from cmos_get_awb_default (sub_AF988) */
#define CALIBRATE_STATIC_TEMP       4950
#define CALIBRATE_STATIC_WB_R_GAIN  450
#define CALIBRATE_STATIC_WB_GR_GAIN 256
#define CALIBRATE_STATIC_WB_GB_GAIN 256
#define CALIBRATE_STATIC_WB_B_GAIN  447
#define CALIBRATE_AWB_P1 (-53)
#define CALIBRATE_AWB_P2 309
#define CALIBRATE_AWB_Q1 0
#define CALIBRATE_AWB_A1 160279
#define CALIBRATE_AWB_B1 128
#define CALIBRATE_AWB_C1 (-110119)
#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0

#define high_8bits(x)   (((x) & 0xff00) >> 8)
#define low_8bits(x)    ((x) & 0x00ff)
#ifndef clip3
#define clip3(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#endif

typedef enum {
    IMX662_2M_30FPS_12BIT_LINEAR_MODE = 0,
    IMX662_2M_60FPS_12BIT_LINEAR_MODE,
    IMX662_2M_90FPS_10BIT_LINEAR_MODE,
    IMX662_MODE_MAX
} imx662_res_mode;

/* linear reg-sync slots (9): SHR0(L/M/H) GAIN(L/H) VMAX(L/M/H) FDG */
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
} imx662_reg_idx;

ot_isp_sns_obj *imx662_get_obj(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* IMX662_CMOS_H */
