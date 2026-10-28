/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef VS4388_CMOS_H
#define VS4388_CMOS_H

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
/* sensor ID & Resolution; chip ID 0x4388 at 0x3000/0x3001 */
#define VS4388_ID             4388
#define VS4388_WIDTH          2560
#define VS4388_HEIGHT         1440

/*
 * sensor Register Address: big-endian pairs, high byte first. These are the
 * registers, and the order, the vendor's Hi3516CV610 firmware hands the ISP
 * to sync in linear mode.
 */
#define VS4388_EXPO_ADDR_H    0x3500 /* integration time in lines */
#define VS4388_EXPO_ADDR_L    0x3501
#define VS4388_GAIN_ADDR_H    0x3508 /* gain in 1/16 steps: 0x0010 is 1x */
#define VS4388_GAIN_ADDR_L    0x3509
#define VS4388_VMAX_ADDR_H    0x3812 /* frame length in lines */
#define VS4388_VMAX_ADDR_L    0x3813

/****************************************************************************
 * i2c bus config                                                           *
 ****************************************************************************/
/* 0x6c, where the vendor firmware addresses it (I2C_SLAVE_FORCE 0x36) */
#define VS4388_I2C_ADDR    0x6c
#define VS4388_ADDR_BYTE   2
#define VS4388_DATA_BYTE   1

/****************************************************************************
 * sensor lines configs                                                     *
 ****************************************************************************/
/*
 * linear mode: 1482 lines at 30 fps; the vendor lets the frame grow to
 * 0xffff lines, down to 1 fps, and keeps the exposure 4 lines short of it.
 */
#define VS4388_VMAX_LINEAR            1482
#define VS4388_FULL_LINES_MAX_LINEAR  0xffff
#define VS4388_FPS_MAX_LINEAR 30
#define VS4388_FPS_MIN_LINEAR 1.0
#define VS4388_WIDTH_LINEAR  2560
#define VS4388_HEIGHT_LINEAR 1440
#define VS4388_MODE_LINEAR 0

/****************************************************************************
 * sensor ae configs                                                        *
 ****************************************************************************/
/*
 * As the vendor driver: analog gain is linear in 1/16 steps and goes to the
 * registers as it is, 1x (0x10) .. 64x (0x400); no digital gain on the
 * sensor; the exposure moves in steps of 2 lines.
 */
#define VS4388_AGAIN_MIN    0x10
#define VS4388_AGAIN_MAX    0x400
#define AGAIN_ACCURACY      0.0625
#define VS4388_DGAIN_1X     256
#define DGAIN_ACCURACY      0.00390625
#define INT_TIME_ACCURACY   2
#define FL_OFFSET_LINEAR    4
#define VS4388_INT_TIME_MIN 2
#define VS4388_INT_TIME_MAX 0xffff

/****************************************************************************
 * sensor awb calibrate configs                                             *
 ****************************************************************************/
/* superpix_sp2308's calibration: no VS4388 has been calibrated. */
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
#define INIT_EXP_DEFAULT_LINEAR   148859
#define AE_COMENSATION_DEFAULT    0x40
/* Black level, 14-bit ISP scale, per channel as the vendor's ISP runs it */
#define BLACK_LEVEL_R             1014
#define BLACK_LEVEL_GR            1024
#define BLACK_LEVEL_GB            1024
#define BLACK_LEVEL_B             1014

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
    VS4388_1440P30_10BIT_LINEAR_MODE = 0,
    VS4388_MODE_MAX
} vs4388_res_mode;

ot_isp_sns_obj *vs4388_get_obj(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* VS4388_CMOS_H */
