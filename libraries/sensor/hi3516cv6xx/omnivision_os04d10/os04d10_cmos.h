/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef OS04D10_CMOS_H
#define OS04D10_CMOS_H

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
#define OS04D10_ID             4
#define OS04D10_WIDTH          2560
#define OS04D10_HEIGHT         1440

#define EXPEXPTAR (1784)
#define OS04D10_FULL_LINES_MAX  (0xFFFF)

/*****Os04d10 Register Address*****/
#define OS04D10_PAGE_ADDR (0xfd)
#define OS04D10_EXP_HIGH_ADDR (0x03)
#define OS04D10_EXP_LOW_ADDR (0x04)
#define OS04D10_AGAIN_ADDR (0x24)
#define OS04D10_DGAIN_ADDR_H (0x37)
#define OS04D10_DGAIN_ADDR_L (0x39)
#define OS04D10_VMAX_ADDR_H (0x05)
#define OS04D10_VMAX_ADDR_L (0x06)

/****************************************************************************
 * i2c bus config                                                           *
 ****************************************************************************/
/* sensor I2C bus config */
#define OS04D10_I2C_ADDR    0x78
#define OS04D10_ADDR_BYTE   1
#define OS04D10_DATA_BYTE   1

/* common I2C bus config */
#define I2C_DEV_FILE_NUM    16
#define I2C_BUF_NUM         8

/****************************************************************************
 * sensor lines configs                                                     *
 ****************************************************************************/
/* increase line */
#define OS04D10_INCREASE_LINES 1  /* make real fps less than stand fps because NVR require */
/* linear mode */
#define OS04D10_FULL_LINES_MAX_LINEAR  0xAC80
#define OS04D10_VMAX_VAL_LINEAR 1472         /* linear init sequence */
#define OS04D10_VMAX_LINEAR     (OS04D10_VMAX_VAL_LINEAR + OS04D10_INCREASE_LINES)
#define OS04D10_FPS_MAX_LINEAR 30
#define OS04D10_FPS_MIN_LINEAR (1.0)
#define OS04D10_WIDTH_LINEAR  2560
#define OS04D10_HEIGHT_LINEAR 1440
#define OS04D10_MODE_LINEAR 0

#define OS04D10_FRAME_RATE_MIN        0xAC80 /* Min:1.0fps */

/****************************************************************************
 * sensor ae configs                                                        *
 ****************************************************************************/
#define OS04D10_AGAIN_MIN    1024
#define OS04D10_AGAIN_MAX    15872

#define OS04D10_DGAIN_MIN    64
#define OS04D10_DGAIN_MAX    64

#define ISP_DGAIN_SHIFT           8
#define ISP_DGAIN_TARGET_MIN      1
#define ISP_DGAIN_TARGET_MAX      128
#define ISP_DGAIN_TARGET_WDR_MIN  1
#define ISP_DGAIN_TARGET_WDR_MAX  4
#define INT_TIME_ACCURACY      1
#define AGAIN_ACCURACY         1
#define DGAIN_ACCURACY         0.015625

#define FL_OFFSET_LINEAR       2
/****************************************************************************
 * sensor awb calibrate configs                                             *
 ****************************************************************************/
/* awb static param for Fuji Lens New IR_Cut */
#define CALIBRATE_STATIC_TEMP       5000
#define CALIBRATE_STATIC_WB_R_GAIN  488
#define CALIBRATE_STATIC_WB_GR_GAIN 256
#define CALIBRATE_STATIC_WB_GB_GAIN 256
#define CALIBRATE_STATIC_WB_B_GAIN  444

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1 (20)
#define CALIBRATE_AWB_P2 236
#define CALIBRATE_AWB_Q1 (0)
#define CALIBRATE_AWB_A1 155214
#define CALIBRATE_AWB_B1 128
#define CALIBRATE_AWB_C1 (-107022)

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
    OS04D10_4M_30FPS_10BIT_LINEAR_MODE = 0,
    OS04D10_MODE_MAX
} os04d10_res_mode;

typedef enum {
    PAGE_IDX = 0,
    EXPO_H_IDX,
    EXPO_L_IDX,
    AGAIN_IDX,
    VMAX_H_IDX,
    VMAX_L_IDX,
    VALUE_IDX,
    REG_MAX_IDX
} os04d10_linear_reg_index;

ot_isp_sns_obj *os04d10_get_obj(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* OS04D10_CMOS_H */
