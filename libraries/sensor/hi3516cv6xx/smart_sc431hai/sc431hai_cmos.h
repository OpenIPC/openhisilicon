/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef SC431HAI_CMOS_H
#define SC431HAI_CMOS_H

#include "ot_common_isp.h"
#include "ot_sns_ctrl.h"
#include "securec.h"
#include "ot_math.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/****************************************************************************
 * basic info config                                                        *
 ****************************************************************************/
/* sensor ID & Resolution */
#define SC431HAI_ID             431
#define SC431HAI_WIDTH          2560
#define SC431HAI_HEIGHT         1440

/* sensor Register Address */
#define SC431HAI_EXPO_H_ADDR           0x3E00
#define SC431HAI_EXPO_M_ADDR           0x3E01
#define SC431HAI_EXPO_L_ADDR           0x3E02
#define SC431HAI_AGAIN_H_ADDR          0x3E08
#define SC431HAI_AGAIN_L_ADDR          0x3E09
#define SC431HAI_DGAIN_H_ADDR          0x3E06
#define SC431HAI_DGAIN_L_ADDR          0x3E07
#define SC431HAI_SHORT_EXPO_H_ADDR     0x3E22
#define SC431HAI_SHORT_EXPO_M_ADDR     0x3E04
#define SC431HAI_SHORT_EXPO_L_ADDR     0x3E05
#define SC431HAI_SHORT_AGAIN_H_ADDR    0x3E12
#define SC431HAI_SHORT_AGAIN_L_ADDR    0x3E13
#define SC431HAI_SHORT_DGAIN_H_ADDR    0x3E10
#define SC431HAI_SHORT_DGAIN_L_ADDR    0x3E11
#define SC431HAI_VMAX_H_ADDR           0x320E
#define SC431HAI_VMAX_L_ADDR           0x320F
#define SC431HAI_SHORT_EXP_MAX_H_ADDR  0x3E23
#define SC431HAI_SHORT_EXP_MAX_L_ADDR  0x3E24

/****************************************************************************
 * i2c bus config                                                           *
 ****************************************************************************/
/* sensor I2C bus config */
#define SC431HAI_I2C_ADDR    0x60
#define SC431HAI_ADDR_BYTE   2
#define SC431HAI_DATA_BYTE   1

/* common I2C bus config */
#define I2C_DEV_FILE_NUM     16
#define I2C_BUF_NUM          8

/****************************************************************************
 * sensor lines configs                                                     *
 ****************************************************************************/
 /* increase line */
 #define SC431HAI_INCREASE_LINES 0  /* make real fps less than stand fps because NVR require */
/* linear mode */
#define SC431HAI_FULL_LINES_MAX_LINEAR 0x4650 /* 0x4650 = 18000 = Min:2.5fps */
#define SC431HAI_VMAX_VAL_LINEAR 1500         /* linear init sequence e.g. 0x380e, 0x06, 0x380f, 0x58 */
#define SC431HAI_VMAX_LINEAR     (SC431HAI_VMAX_VAL_LINEAR + SC431HAI_INCREASE_LINES)
#define SC431HAI_FPS_MAX_LINEAR 30
#define SC431HAI_FPS_MIN_LINEAR 2.5
#define SC431HAI_WIDTH_LINEAR 2560
#define SC431HAI_HEIGHT_LINEAR 1440
#define SC431HAI_MODE_LINEAR 0

/* WDR mode */
#define SC431HAI_FULL_LINES_MAX_2TO1_LINE_WDR 0x2328 /* 0x2328 = 9000 = Min:10fps */
#define SC431HAI_VMAX_VAL_2TO1_LINE_WDR 3000         /* WDR init sequence 0x380e, 0x09, 0x380f, 0x84 */
#define SC431HAI_VMAX_2TO1_LINE_WDR   (SC431HAI_VMAX_VAL_2TO1_LINE_WDR + SC431HAI_INCREASE_LINES)
#define SC431HAI_FPS_MAX_2TO1_LINE_WDR 30
#define SC431HAI_FPS_MIN_2TO1_LINE_WDR 10.0
#define SC431HAI_WIDTH_2TO1_LINE_WDR 2560
#define SC431HAI_HEIGHT_2TO1_LINE_WDR 1440
#define SC431HAI_MODE_2TO1_LINE_WDR 0
#define SC431HAI_EXP_Y 0x5A7

/****************************************************************************
 * sensor ae configs                                                        *
 ****************************************************************************/
 /* linear mode */
#define SC431HAI_AGAIN_MIN    1024
#define SC431HAI_AGAIN_MAX    50069

#define SC431HAI_DGAIN_MIN    1024
#define SC431HAI_DGAIN_MAX   16256

#define INT_TIME_ACCURACY_LINEAR  1
#define INT_TIME_ACCURACY_WDR     4

#define AGAIN_ACCURACY            1
#define DGAIN_ACCURACY            1
#define ISP_DGAIN_SHIFT           8
#define ISP_DGAIN_TARGET_MIN      1
#define ISP_DGAIN_TARGET_MAX     32

#define ISP_DGAIN_TARGET_WDR_MAX  4

#define FL_OFFSET_LINEAR                  11
#define FL_OFFSET_WDR_LONG                21
#define FL_OFFSET_WDR_SHORT               19
#define SC431HAI_SEXP_MAX_DEFAULT         0xB8
 /****************************************************************************
 * sensor awb calibrate configs                                             *
 ****************************************************************************/
/* awb static param for Fuji Lens New IR_Cut */
#define CALIBRATE_STATIC_TEMP       4950
#define CALIBRATE_STATIC_WB_R_GAIN  478
#define CALIBRATE_STATIC_WB_GR_GAIN 256
#define CALIBRATE_STATIC_WB_GB_GAIN 256
#define CALIBRATE_STATIC_WB_B_GAIN  497

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1 (-102)
#define CALIBRATE_AWB_P2 358
#define CALIBRATE_AWB_Q1 0
#define CALIBRATE_AWB_A1 183957
#define CALIBRATE_AWB_B1 128
#define CALIBRATE_AWB_C1 (-135938)

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN                                  0
#define GOLDEN_BGAIN                                  0

/****************************************************************************
 * sensor other configs                                                     *
 ****************************************************************************/
#define STANDARD_FPS              30
#define FLICKER_FREQ            (50 * 256)  /* light flicker freq: 50Hz, accuracy: 256 */

#define INIT_EXP_DEFAULT_LINEAR   148859
#define INIT_EXP_DEFAULT_WDR       66462

#define MAX_INT_TIME_TARGET        65535

#define AE_COMENSATION_DEFAULT       0x40
#define AE_COMENSATION_WDR_LONG_FRM    56
#define AE_COMENSATION_WDR_NORM_FRM    32

#define AE_ARR_RATIO_0_WDR             0x400
#define AE_ARR_RATIO_1_WDR             0x40
#define AE_ARR_RATIO_2_WDR             0x40

/* Black level */
#define BLACK_LEVEL_DEFAULT          0x410
/* DNG */
#define DNG_RAW_FORMAT_BIT_LINEAR         10 /* raw 10 bit */
#define DNG_RAW_FORMAT_WHITE_LEVEL_LINEAR 1023 /* 2^10 - 1 */
#define DNG_RAW_FORMAT_BIT_WDR            10 /* raw 10 bit */
#define DNG_RAW_FORMAT_WHITE_LEVEL_WDR    1023 /* 2^10 - 1 */

/****************************************************************************
 * assist function macros                                                   *
 ****************************************************************************/
#define high_8bits(x)   (((x) & 0xff00) >> 8)
#define low_8bits(x)    ((x) & 0x00ff)
#define lower_4bits(x)  (((x) & 0x000F) << 4)
#define higher_4bits(x) (((x) & 0xF000) >> 12)
#define higher_8bits(x) (((x) & 0x0FF0) >> 4)
#ifndef clip3
#define clip3(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#endif

/****************************************************************************
 * sensor data type defines                                                 *
 ****************************************************************************/
typedef enum {
    SC431HAI_4M_30FPS_10BIT_LINEAR_MODE = 0,
    SC431HAI_4M_30FPS_10BIT_2TO1_VC_MODE,
    SC431HAI_MODE_MAX
} sc431hai_res_mode;

typedef enum {
    EXPO_L_IDX = 0,
    EXPO_M_IDX,
    EXPO_H_IDX,
    AGAIN_L_IDX,
    AGAIN_H_IDX,
    DGAIN_L_IDX,
    DGAIN_H_IDX,
    VMAX_L_IDX,
    VMAX_H_IDX,
    REG_MAX_IDX
} sc431hai_linear_reg_index;
typedef enum {
    WDR_EXPO_L_IDX = 0,
    WDR_EXPO_M_IDX,
    WDR_EXPO_H_IDX,
    WDR_AGAIN_L_IDX,
    WDR_AGAIN_H_IDX,
    WDR_DGAIN_L_IDX,
    WDR_DGAIN_H_IDX,
    WDR_VMAX_L_IDX,
    WDR_VMAX_H_IDX,
    WDR_SHORT_EXPO_L_IDX,
    WDR_SHORT_EXPO_M_IDX,
    WDR_SHORT_AGAIN_L_IDX,
    WDR_SHORT_AGAIN_H_IDX,
    WDR_SHORT_DGAIN_L_IDX,
    WDR_SHORT_DGAIN_H_IDX,
    SHORT_EXP_MAX_L_ADDR_IDX,
    SHORT_EXP_MAX_H_ADDR_IDX,
	WDR_REG_MAX_IDX
}sc431hai_wdr_reg_index;

ot_isp_sns_obj *sc431hai_get_obj(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* SC431AI_CMOS_H */
