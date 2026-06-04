/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef GC4023_CMOS_H
#define GC4023_CMOS_H

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
#define GC4023_ID             4023
#define GC4023_WIDTH          2560
#define GC4023_HEIGHT         1440

/* sensor Register Address */
#define GC4023_EXPO_H_ADDR                 0x0202
#define GC4023_EXPO_L_ADDR                 0x0203
#define GC4023_AGAIN_L_ADDR                0x0614
#define GC4023_AGAIN_M_ADDR                0x0615
#define GC4023_AGAIN_H_ADDR                0x0218
#define GC4023_COL_GAIN_H_ADDR             0x00b8
#define GC4023_COL_GAIN_L_ADDR             0x00b9
#define GC2053_AUTO_PREGAIN_ADDR_H         0x0064
#define GC2053_AUTO_PREGAIN_ADDR_L         0x0065
#define GC4023_VMAX_H_ADDR                 0x0340
#define GC4023_VMAX_L_ADDR                 0x0341

/****************************************************************************
 * i2c bus config                                                           *
 ****************************************************************************/
/* sensor I2C bus config */
#define GC4023_I2C_ADDR    0x52
#define GC4023_ADDR_BYTE   2
#define GC4023_DATA_BYTE   1

/* common I2C bus config */
#define I2C_DEV_FILE_NUM    16
#define I2C_BUF_NUM         8

/****************************************************************************
 * sensor lines configs                                                     *
 ****************************************************************************/
/* increase line */
#define GC4023_INCREASE_LINES 0  /* make real fps less than stand fps because NVR require */
/* linear mode */
#define GC4023_FULL_LINES_MAX_LINEAR  0x1D4C
#define GC4023_VMAX_VAL_LINEAR 1500         /* linear init sequence */
#define GC4023_VMAX_LINEAR     (GC4023_VMAX_VAL_LINEAR + GC4023_INCREASE_LINES)
#define GC4023_FPS_MAX_LINEAR 30
#define GC4023_FPS_MIN_LINEAR 6.0
#define GC4023_WIDTH_LINEAR  2560
#define GC4023_HEIGHT_LINEAR 1440
#define GC4023_MODE_LINEAR 0

#define GC4023_FRAME_RATE_MIN        0x1D4C /* Min:6.0fps */

/****************************************************************************
 * sensor ae configs                                                        *
 ****************************************************************************/
#define GC4023_AGAIN_MIN    1024
#define GC4023_AGAIN_MAX    65536

#define GC4023_DGAIN_MIN    64
#define GC4023_DGAIN_MAX    512

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
/* awb static param for Fuji Lens New IR_Cut */
#define CALIBRATE_STATIC_TEMP       5000
#define CALIBRATE_STATIC_WB_R_GAIN  399
#define CALIBRATE_STATIC_WB_GR_GAIN 256
#define CALIBRATE_STATIC_WB_GB_GAIN 256
#define CALIBRATE_STATIC_WB_B_GAIN  371

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1 (90)
#define CALIBRATE_AWB_P2 166
#define CALIBRATE_AWB_Q1 0
#define CALIBRATE_AWB_A1 201522
#define CALIBRATE_AWB_B1 128
#define CALIBRATE_AWB_C1 (-151902)

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
/* Black level */
#define BLACK_LEVEL_DEFAULT            0x400
/* DNG */
#define DNG_RAW_FORMAT_BIT_LINEAR         12 /* raw 12 bit */
#define DNG_RAW_FORMAT_WHITE_LEVEL_LINEAR 4095 /* 2^12 - 1 */

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
    GC4023_4M_30FPS_10BIT_LINEAR_MODE = 0,
    GC4023_MODE_MAX
} gc4023_res_mode;

ot_isp_sns_obj *gc4023_get_obj(td_void);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* GC4023_CMOS_H */
