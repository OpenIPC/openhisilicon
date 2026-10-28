/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef GC4653_CMOS_H
#define GC4653_CMOS_H

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
/* sensor ID & Resolution; chip ID 0x4653 at 0x03f0/0x03f1 */
#define GC4653_ID             4653
#define GC4653_WIDTH          2560
#define GC4653_HEIGHT         1440

/*
 * sensor Register Address: big-endian pairs, high byte first. These are the
 * registers, and the order, the vendor's Hi3516CV610 firmware hands the ISP
 * to sync: the seven analog-gain registers, digital gain, exposure, VMAX.
 */
#define GC4653_AGAIN_ADDR_0   0x02b3
#define GC4653_AGAIN_ADDR_1   0x02b4
#define GC4653_AGAIN_ADDR_2   0x02b8
#define GC4653_AGAIN_ADDR_3   0x02b9
#define GC4653_AGAIN_ADDR_4   0x0515
#define GC4653_AGAIN_ADDR_5   0x0519
#define GC4653_AGAIN_ADDR_6   0x02d9
#define GC4653_DGAIN_ADDR_H   0x020e /* digital gain [11:6] */
#define GC4653_DGAIN_ADDR_L   0x020f /* digital gain [5:0], << 2 */
#define GC4653_EXPO_ADDR_H    0x0202 /* shutter [13:8] */
#define GC4653_EXPO_ADDR_L    0x0203 /* shutter [7:0] */
#define GC4653_VMAX_ADDR_H    0x0340 /* frame length [13:8] */
#define GC4653_VMAX_ADDR_L    0x0341 /* frame length [7:0] */

/****************************************************************************
 * i2c bus config                                                           *
 ****************************************************************************/
#define GC4653_I2C_ADDR    0x52
#define GC4653_ADDR_BYTE   2
#define GC4653_DATA_BYTE   1

/****************************************************************************
 * sensor lines configs                                                     *
 ****************************************************************************/
/*
 * linear mode: 1500 lines at 30 fps, one more so real fps stays under the
 * standard, as the hi3516ev200 driver has it; the frame grows to 0x3fff
 * lines (about 2.75 fps) and the exposure ends 8 lines short of it.
 */
#define GC4653_INCREASE_LINES         1
#define GC4653_VMAX_LINEAR            (1500 + GC4653_INCREASE_LINES)
#define GC4653_FULL_LINES_MAX_LINEAR  0x3fff
#define GC4653_FPS_MAX_LINEAR 30
#define GC4653_FPS_MIN_LINEAR 2.75
#define GC4653_WIDTH_LINEAR  2560
#define GC4653_HEIGHT_LINEAR 1440
#define GC4653_MODE_LINEAR 0

/****************************************************************************
 * sensor ae configs                                                        *
 ****************************************************************************/
/* 1x .. 75.84x analog in 26 steps; digital in 1/64 steps, 1x .. 8x */
#define GC4653_AGAIN_ROWS   26
#define GC4653_AGAIN_MIN    1024
#define GC4653_AGAIN_MAX    77660
#define GC4653_DGAIN_MIN    64
#define GC4653_DGAIN_MAX    512

#define DGAIN_ACCURACY         0.015625
#define FL_OFFSET_LINEAR       8
#define GC4653_INT_TIME_MIN    1
#define GC4653_INT_TIME_MAX    0x3fdb

/****************************************************************************
 * sensor awb calibrate configs                                             *
 ****************************************************************************/
/* The GC4653's own calibration, from the hi3516ev200 driver. */
#define CALIBRATE_STATIC_TEMP       5082
#define CALIBRATE_STATIC_WB_R_GAIN  404
#define CALIBRATE_STATIC_WB_GR_GAIN 256
#define CALIBRATE_STATIC_WB_GB_GAIN 256
#define CALIBRATE_STATIC_WB_B_GAIN  509

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1 12
#define CALIBRATE_AWB_P2 213
#define CALIBRATE_AWB_Q1 (-30)
#define CALIBRATE_AWB_A1 186383
#define CALIBRATE_AWB_B1 128
#define CALIBRATE_AWB_C1 (-131654)

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN                                  0
#define GOLDEN_BGAIN                                  0

/****************************************************************************
 * sensor other configs                                                     *
 ****************************************************************************/
#define STANDARD_FPS              30
#define FLICKER_FREQ            (50 * 256)  /* light flicker freq: 50Hz, accuracy: 256 */
#define INIT_EXP_DEFAULT_LINEAR   148859
#define AE_COMENSATION_DEFAULT    0x40
/* Black level: 14-bit ISP scale; the hi3516ev200 driver's 256 at 12 bits */
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
typedef enum {
    GC4653_4M30_10BIT_LINEAR_MODE = 0,
    GC4653_MODE_MAX
} gc4653_res_mode;

ot_isp_sns_obj *gc4653_get_obj(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* GC4653_CMOS_H */
