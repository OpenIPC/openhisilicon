/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef CV2003_CMOS_H
#define CV2003_CMOS_H

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
/* sensor ID & Resolution; chip ID 0x2004 at 0x3003/0x3002 */
#define CV2003_ID             2003
#define CV2003_WIDTH          1920
#define CV2003_HEIGHT         1080

/*
 * sensor Register Address. Every value is little-endian across consecutive
 * registers, the low byte at the lowest address. These are the registers,
 * and the order, the vendor's CV608 firmware hands the ISP to sync.
 */
#define CV2003_EXPO_L_ADDR    0x3048 /* integration time in lines, 24 bits */
#define CV2003_EXPO_M_ADDR    0x3049
#define CV2003_EXPO_H_ADDR    0x304a
#define CV2003_AGAIN_ADDR     0x3154 /* code c: gain = 256 / (256 - c) */
#define CV2003_DGAIN_L_ADDR   0x314c /* 1/64 steps: 0x0040 is 1x */
#define CV2003_DGAIN_H_ADDR   0x314d
#define CV2003_VMAX_L_ADDR    0x3020 /* frame length in lines */
#define CV2003_VMAX_H_ADDR    0x3021

/****************************************************************************
 * i2c bus config                                                           *
 ****************************************************************************/
/* sensor I2C bus config */
#define CV2003_I2C_ADDR    0x6a
#define CV2003_ADDR_BYTE   2
#define CV2003_DATA_BYTE   1

/****************************************************************************
 * sensor lines configs                                                     *
 ****************************************************************************/
/*
 * linear mode: 2400 lines at 30 fps; the vendor lets the frame grow to
 * 28800 lines (2.5 fps) and keeps the exposure 10 lines short of it.
 */
#define CV2003_VMAX_LINEAR            2400
#define CV2003_FULL_LINES_MAX_LINEAR  28800
#define CV2003_FPS_MAX_LINEAR 30
#define CV2003_FPS_MIN_LINEAR 2.5
#define CV2003_WIDTH_LINEAR  1920
#define CV2003_HEIGHT_LINEAR 1080
#define CV2003_MODE_LINEAR 0

/****************************************************************************
 * sensor ae configs                                                        *
 ****************************************************************************/
/* 1x .. 32x analog, codes 0 .. 0xf8; sensor digital gain held at 1x */
#define CV2003_AGAIN_CODE_MAX 0xf8
#define CV2003_AGAIN_MIN    1024
#define CV2003_AGAIN_MAX    32768
#define CV2003_DGAIN_1X     0x40
#define CV2003_DGAIN_MIN    1024
#define CV2003_DGAIN_MAX    1024

#define DGAIN_ACCURACY         0.015625
#define FL_OFFSET_LINEAR       10
#define CV2003_INT_TIME_MIN    4

/****************************************************************************
 * sensor awb calibrate configs                                             *
 ****************************************************************************/
/* superpix_sp2308's calibration: no CV2003 has been calibrated. */
#define CALIBRATE_STATIC_TEMP       5082
#define CALIBRATE_STATIC_WB_R_GAIN  379
#define CALIBRATE_STATIC_WB_GR_GAIN 256
#define CALIBRATE_STATIC_WB_GB_GAIN 256
#define CALIBRATE_STATIC_WB_B_GAIN  651

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1 130
#define CALIBRATE_AWB_P2 (-12)
#define CALIBRATE_AWB_Q1 (-138)
#define CALIBRATE_AWB_A1 211250
#define CALIBRATE_AWB_B1 128
#define CALIBRATE_AWB_C1 (-143554)

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN                                  0
#define GOLDEN_BGAIN                                  0

/****************************************************************************
 * sensor other configs                                                     *
 ****************************************************************************/
#define STANDARD_FPS              30
#define FLICKER_FREQ            (50 * 256)  /* light flicker freq: 50Hz, accuracy: 256 */
#define INIT_EXP_DEFAULT_LINEAR   130000
#define AE_COMENSATION_DEFAULT    0x40
/* Black level: 14-bit ISP scale, 64 DN at 10 bits, what the vendor's ISP runs */
#define BLACK_LEVEL_DEFAULT            0x400

/* DNG */
#define DNG_RAW_FORMAT_BIT_LINEAR         10 /* raw 10 bit */
#define DNG_RAW_FORMAT_WHITE_LEVEL_LINEAR 1023 /* 2^10 - 1 */

/****************************************************************************
 * assist function macros                                                   *
 ****************************************************************************/
#define high_8bits(x) (((x) & 0xff00) >> 8)
#define low_8bits(x)  ((x) & 0x00ff)

/****************************************************************************
 * sensor data type defines                                                 *
 ****************************************************************************/
/* define your sensor modes */
typedef enum {
    CV2003_1080P30_10BIT_LINEAR_MODE = 0,
    CV2003_MODE_MAX
} cv2003_res_mode;

ot_isp_sns_obj *cv2003_get_obj(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* CV2003_CMOS_H */
