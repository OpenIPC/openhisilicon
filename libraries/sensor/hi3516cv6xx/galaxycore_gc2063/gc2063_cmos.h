/*
 * GC2063 (GalaxyCore, 2M) — CV610 driver constants.
 *
 * Referenced by the K662C6S firmware's M_SENSOR_TYPE_Init selector. Power-on
 * registers EXTRACTED byte-exact from xipc (8-bit paged scheme). Register map,
 * exposure and multi-register gain model come from the GalaxyCore GC2053
 * reference driver (Hi3516CV500 galaxycore_gc2053 — GC2063 is register-
 * compatible) and the CV610-native GC4023 skeleton. Linear 2M 1920x1080 30fps 10-bit.
 */
#ifndef GC2063_CMOS_H
#define GC2063_CMOS_H

#include "ot_common_isp.h"
#include "ot_sns_ctrl.h"
#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define GC2063_ID             2063
#define GC2063_WIDTH          1920
#define GC2063_HEIGHT         1080

/* register map (GalaxyCore GC2053-family, 8-bit addr / 8-bit data, page 0) */
#define GC2063_EXPTIME_ADDR_H  0x03   /* Shutter-time[13:8] */
#define GC2063_EXPTIME_ADDR_L  0x04   /* Shutter-time[7:0] */
#define GC2063_AGAIN_ADDR_H    0xb4   /* Analog-gain[9:8] */
#define GC2063_AGAIN_ADDR_L    0xb3   /* Analog-gain[7:0] */
#define GC2063_DGAIN_ADDR_H    0xb9   /* digital-gain[11:6] */
#define GC2063_DGAIN_ADDR_L    0xb8   /* digital-gain[5:0] */
#define GC2063_PREGAIN_ADDR_H  0xb1   /* auto-pregain[9:6] */
#define GC2063_PREGAIN_ADDR_L  0xb2   /* auto-pregain[5:0] */
#define GC2063_STATUS_ADDR     0x90   /* slow-shutter mode */
#define GC2063_VMAX_ADDR_H     0x41   /* Vmax[13:8] */
#define GC2063_VMAX_ADDR_L     0x42   /* Vmax[7:0] */
#define GC2063_PAGE_ADDR       0xfe   /* page select; AE and orientation live on page 0 */
#define GC2063_MIRROR_FLIP_ADDR 0x17  /* page 0: bit0 mirror, bit1 flip */
#define GC2063_MIRROR_FLIP_BASE 0x80
#define GC2063_MIRROR_BIT       0x01
#define GC2063_FLIP_BIT         0x02
/* Standby: the GC2053 family's global PLL/pad controls, which the init leaves
 * at 0x00/0x8e. */
#define GC2063_STANDBY_ADDR_0  0xf2
#define GC2063_STANDBY_ADDR_1  0xfc

#define GC2063_I2C_ADDR       0x6e
#define GC2063_ADDR_BYTE      1
#define GC2063_DATA_BYTE      1

#define I2C_DEV_FILE_NUM      16
#define I2C_BUF_NUM           8

/* timing — VMAX 1125 @ 30fps (0x41/0x42 = 0x0465 from xipc init); FULL_LINES 14-bit */
#define GC2063_INCREASE_LINES 0
#define GC2063_FULL_LINES_MAX_LINEAR  0x3FFF
#define GC2063_VMAX_VAL_LINEAR 1125
#define GC2063_VMAX_LINEAR     (GC2063_VMAX_VAL_LINEAR + GC2063_INCREASE_LINES)
#define GC2063_FRAME_RATE_MIN  0x3FFF
#define GC2063_FPS_MAX_LINEAR 30
#define GC2063_FPS_MIN_LINEAR 6.0
#define GC2063_WIDTH_LINEAR   1920
#define GC2063_HEIGHT_LINEAR  1080
#define GC2063_MODE_LINEAR    0

/* AE: analog gain is a 29-entry table (value->index->regs); isp digital gain
 * is 1/64 fixed point (64=1x, 512=8x) packed into the auto-pregain regs. */
#define GC2063_AGAIN_MIN      1024
#define GC2063_AGAIN_MAX      113168
#define GC2063_DGAIN_MIN      64
#define GC2063_DGAIN_MAX      512
#define GC2063_REG_VAL_TAB_ROW 29
#define GC2063_REG_VAL_TAB_COL 4

#define ISP_DGAIN_SHIFT          8
#define ISP_DGAIN_TARGET_MIN     1
#define ISP_DGAIN_TARGET_MAX     2
#define INT_TIME_ACCURACY        1
#define AGAIN_ACCURACY           1
#define DGAIN_ACCURACY           0.015625
#define FL_OFFSET_LINEAR         2
/* The table's own exposure (0x03/0x04 = 0x0460 lines). */
#define GC2063_EXP_DEFAULT_LINEAR 0x0460

/* AWB — GC2063's values from the GalaxyCore GC2053 reference driver */
#define CALIBRATE_STATIC_TEMP       4908
#define CALIBRATE_STATIC_WB_R_GAIN  388
#define CALIBRATE_STATIC_WB_GR_GAIN 256
#define CALIBRATE_STATIC_WB_GB_GAIN 256
#define CALIBRATE_STATIC_WB_B_GAIN  519
#define CALIBRATE_AWB_P1 39
#define CALIBRATE_AWB_P2 152
#define CALIBRATE_AWB_Q1 (-65)
#define CALIBRATE_AWB_A1 201612
#define CALIBRATE_AWB_B1 128
#define CALIBRATE_AWB_C1 (-147924)
#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0

#define STANDARD_FPS           30
#define FLICKER_FREQ           (50 * 256)
#define INIT_EXP_DEFAULT_LINEAR 148859
#define MAX_INT_TIME_TARGET    65535
#define AE_COMENSATION_DEFAULT 0x40

/* black level / DNG — GC2063 is 10-bit. CV610 ISP black level is in the fixed
 * ~12-bit-normalized domain (GalaxyCore GC4023 uses 0x400); the CV500 GC2053
 * driver's 0x100 is a different-generation ISP value. DNG white is 10-bit. */
#define BLACK_LEVEL_DEFAULT            0x400
#define DNG_RAW_FORMAT_BIT_LINEAR         10
#define DNG_RAW_FORMAT_WHITE_LEVEL_LINEAR 1023

#define high_8bits(x) (((x) & 0xff00) >> 8)
#define low_8bits(x)  ((x) & 0x00ff)
#ifndef clip3
#define clip3(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#endif

/* Sync register slots, in the order the reference driver writes them. */
typedef enum {
    GC2063_REG_EXP_L = 0,
    GC2063_REG_EXP_H,
    GC2063_REG_AGAIN_H,
    GC2063_REG_AGAIN_L,
    GC2063_REG_DGAIN_L,
    GC2063_REG_DGAIN_H,
    GC2063_REG_PREGAIN_H,
    GC2063_REG_PREGAIN_L,
    GC2063_REG_STATUS,
    GC2063_REG_VMAX_L,
    GC2063_REG_VMAX_H,
    GC2063_SNS_REGS
} gc2063_reg_index;

typedef enum {
    GC2063_2M_30FPS_10BIT_LINEAR_MODE = 0,
    GC2063_MODE_MAX
} gc2063_res_mode;

ot_isp_sns_obj *gc2063_get_obj(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* GC2063_CMOS_H */
