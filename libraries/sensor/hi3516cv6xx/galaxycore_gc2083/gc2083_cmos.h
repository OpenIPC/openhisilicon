/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef GC2083_CMOS_H
#define GC2083_CMOS_H

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
/* sensor ID & Resolution */
#define GC2083_ID             2083
#define GC2083_WIDTH          1920
#define GC2083_HEIGHT         1080

/* sensor Register Address */
#define GC2083_EXPO_H_ADDR                 0x0d03
#define GC2083_EXPO_L_ADDR                 0x0d04
#define GC2083_VMAX_H_ADDR                 0x0d41
#define GC2083_VMAX_L_ADDR                 0x0d42

/****************************************************************************
 * i2c bus config                                                           *
 ****************************************************************************/
/* sensor I2C bus config */
#define GC2083_I2C_ADDR    0x6e
#define GC2083_ADDR_BYTE   2
#define GC2083_DATA_BYTE   1

/* common I2C bus config */
#define I2C_DEV_FILE_NUM    16
#define I2C_BUF_NUM         8

/****************************************************************************
 * sensor lines configs                                                     *
 ****************************************************************************/
/* increase line */
#define GC2083_INCREASE_LINES 0  /* make real fps less than stand fps because NVR require */
/* linear mode */
#define GC2083_FULL_LINES_MAX_LINEAR  0xFFFF
#define GC2083_VMAX_VAL_LINEAR  1125
#define GC2083_VMAX_LINEAR     (GC2083_VMAX_VAL_LINEAR + GC2083_INCREASE_LINES)
#define GC2083_FPS_MAX_LINEAR 30
#define GC2083_FPS_MIN_LINEAR 6.0
#define GC2083_WIDTH_LINEAR  1920
#define GC2083_HEIGHT_LINEAR 1080
#define GC2083_MODE_LINEAR 0

#define GC2083_FRAME_RATE_MIN        0x1D4C /* Min:6.0fps */

/****************************************************************************
 * sensor ae configs                                                        *
 ****************************************************************************/
#define GC2083_AGAIN_MIN    1024
#define GC2083_AGAIN_MAX    151584

#define GC2083_DGAIN_MIN    64
#define GC2083_DGAIN_MAX    64

#define ISP_DGAIN_SHIFT           8
#define ISP_DGAIN_TARGET_MIN      1
#define ISP_DGAIN_TARGET_MAX      16
#define ISP_DGAIN_TARGET_WDR_MIN  1
#define ISP_DGAIN_TARGET_WDR_MAX  4
#define INT_TIME_ACCURACY      1
#define AGAIN_ACCURACY         1
#define DGAIN_ACCURACY         0.015625

#define FL_OFFSET_LINEAR       8
/****************************************************************************
 * sensor awb calibrate configs                                             *
 ****************************************************************************/
/*
 * AWB calibration from the vendor scene INI [static_awb] (auto_static_wb,
 * auto_curve_para), which the vendor app applies over its driver's own
 * 492/443 and 16,240,0,188580,128,-139106. Those are a generic curve; on the
 * #260 camera they left a magenta cast that these values remove.
 */
#define CALIBRATE_STATIC_TEMP       5000
#define CALIBRATE_STATIC_WB_R_GAIN  411
#define CALIBRATE_STATIC_WB_GR_GAIN 256
#define CALIBRATE_STATIC_WB_GB_GAIN 256
#define CALIBRATE_STATIC_WB_B_GAIN  458

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1 87
#define CALIBRATE_AWB_P2 49
#define CALIBRATE_AWB_Q1 (-120)
#define CALIBRATE_AWB_A1 222213
#define CALIBRATE_AWB_B1 128
#define CALIBRATE_AWB_C1 (-169828)

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN                                  0
#define GOLDEN_BGAIN                                  0

/****************************************************************************
 * sensor other configs                                                     *
 ****************************************************************************/
#define STANDARD_FPS              30
#define FLICKER_FREQ            (50 * 256)  /* light flicker freq: 50Hz, accuracy: 256 */
#define INIT_EXP_DEFAULT_LINEAR   76151
#define MAX_INT_TIME_TARGET       65535
#define AE_COMENSATION_DEFAULT    0x40
/* Black level: 14-bit ISP scale, i.e. the sensor's 64 DN 10-bit pedestal << 4 (vendor value) */
#define BLACK_LEVEL_DEFAULT            0x400
/* DNG */
#define DNG_RAW_FORMAT_BIT_LINEAR         10 /* raw 10 bit */
#define DNG_RAW_FORMAT_WHITE_LEVEL_LINEAR 1023 /* 2^10 - 1 */

/****************************************************************************
 * assist function macros                                                   *
 ****************************************************************************/
#define higher_4bits(x) (((x) & 0xf0000) >> 16)
#define high_8bits(x) (((x) & 0xff00) >> 8)
#define low_8bits(x)  ((x) & 0x00ff)

#ifndef max
#define max(a, b) (((a) < (b)) ? (b) : (a))
#endif

/****************************************************************************
 * sensor data type defines                                                 *
 ****************************************************************************/
/* define your sensor modes */
typedef enum {
    GC2083_1080P30_10BIT_LINEAR_MODE = 0,
    GC2083_MODE_MAX
} gc2083_res_mode;

ot_isp_sns_obj *gc2083_get_obj(td_void);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* GC2083_CMOS_H */
