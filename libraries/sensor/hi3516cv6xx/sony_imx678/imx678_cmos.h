/*
 * IMX678 (Sony Starvis 2, 8M) — CV610 driver constants.
 * Register map and limits of the Hi3519DV500 IMX678 driver, on the Starvis 2
 * exposure/gain model of this tree's IMX662/IMX664 drivers. Linear 4K30 12-bit.
 */
#ifndef IMX678_CMOS_H
#define IMX678_CMOS_H

#include "ot_common_isp.h"
#include "ot_sns_ctrl.h"
#include "securec.h"
#include "ot_math.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define IMX678_ID              678
#define IMX678_WIDTH           3840
#define IMX678_HEIGHT          2160

/* i2c: dev_addr 0x34, 16-bit reg, 8-bit data */
#define IMX678_I2C_ADDR        0x34
#define IMX678_ADDR_BYTE       2
#define IMX678_DATA_BYTE       1

/* register map (Starvis 2) */
#define IMX678_SHR0_ADDR       0x3050  /* L=0x3050 M=0x3051 H=0x3052 (exposure) */
#define IMX678_GAIN_ADDR       0x3070  /* L=0x3070 H=0x3071 */
#define IMX678_VMAX_ADDR_L     0x3028
#define IMX678_VMAX_ADDR_M     0x3029
#define IMX678_VMAX_ADDR_H     0x302A
#define IMX678_FDG_ADDR        0x3030  /* FDG_SEL0: 0 LCG, 1 HCG */
#define IMX678_HREVERSE_ADDR   0x3020  /* bit 0: horizontal mirror */
#define IMX678_VREVERSE_ADDR   0x3021  /* bit 0: vertical flip */

/* frame length / timing: VMAX 2250 + 1, as the Hi3519DV500 driver runs it
 * ("make real fps less than stand fps because NVR require") */
#define IMX678_FULL_LINES_MAX  0xFFFFF
#define IMX678_VMAX_4K_30FPS   2251
#define IMX678_FPS_30          30
#define IMX678_FPS_MIN_LINEAR  0.07

/*
 * AE: one 0.3 dB table for analog and sensor digital gain, summed into GAIN.
 * Analog gain is 30 dB in conversion gain low (codes 0..100). Above analog
 * index 99 the driver switches to conversion gain high (FDG_SEL0 = 1) and
 * takes 27 steps (8.1 dB, the Hi3519DV500 driver's HCG step; the datasheet's
 * HCG/LCG ratio is 2.4..2.9, typ. 2.7) off the code. Switching at 99 rather
 * than the Hi3519DV500 driver's 27 keeps GAIN at 73 or more in HCG, inside
 * the 34..240 the IMX678 software reference allows with FDG_SEL0 = 1.
 * Analog therefore tops out at index 127 (38.1 dB) and sensor digital at
 * index 140, so the sum stays inside GAIN's 0..240.
 */
#define IMX678_AGAIN_MIN       1024
#define IMX678_AGAIN_MAX       82281    /* ad_gain_table[127], 38.1 dB */
#define IMX678_DGAIN_MIN       1024
#define IMX678_DGAIN_MAX       128914   /* ad_gain_table[140], 42 dB */
#define IMX678_AGAIN_TBL_RANGE 127
#define IMX678_DGAIN_TBL_RANGE 140
#define IMX678_AD_GAIN_TBL_RANGE 241
#define IMX678_HCG_THRESHOLD   99       /* again idx > 99 -> HCG, subtract 27 */
#define IMX678_HCG_OFFSET      27

#define ISP_DGAIN_SHIFT        8
#define ISP_DGAIN_TARGET_MIN   1
#define ISP_DGAIN_TARGET_MAX   2

#define INIT_EXP_DEFAULT_LINEAR 76151
#define MAX_INT_TIME_TARGET    65535
#define AE_COMPENSATION_DEFAULT 0x38
#define FL_OFFSET_LINEAR       8        /* max_int_time = VMAX - 8 (IMX678_EXP_TIME_LMT) */
#define MIN_INT_TIME           9
#define SHR0_MIN               6        /* SHR0 clamp [6, VMAX - 1]; Sony allows [4, VMAX - 1] */
#define SHR0_MAX_OFFSET        1

/* black level / DNG: 12-bit linear; black 0x320 (the CV610 ISP's ~12-bit domain) */
#define BLACK_LEVEL_DEFAULT             0x320
#define DNG_RAW_FORMAT_BIT_LINEAR       12
#define DNG_RAW_FORMAT_WHITE_LEVEL_LINEAR 4095

/* AWB: the Hi3519DV500 IMX678 driver's calibration */
#define CALIBRATE_STATIC_TEMP       4950
#define CALIBRATE_STATIC_WB_R_GAIN  450
#define CALIBRATE_STATIC_WB_GR_GAIN 256
#define CALIBRATE_STATIC_WB_GB_GAIN 256
#define CALIBRATE_STATIC_WB_B_GAIN  447
#define CALIBRATE_AWB_P1 18
#define CALIBRATE_AWB_P2 238
#define CALIBRATE_AWB_Q1 0
#define CALIBRATE_AWB_A1 212714
#define CALIBRATE_AWB_B1 128
#define CALIBRATE_AWB_C1 (-163748)
#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0

#define high_8bits(x)   (((x) & 0xff00) >> 8)
#define low_8bits(x)    ((x) & 0x00ff)
#ifndef clip3
#define clip3(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#endif

typedef enum {
    IMX678_8M_30FPS_12BIT_LINEAR_MODE = 0,
    IMX678_MODE_MAX
} imx678_res_mode;

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
} imx678_reg_idx;

ot_isp_sns_obj *imx678_get_obj(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* IMX678_CMOS_H */
