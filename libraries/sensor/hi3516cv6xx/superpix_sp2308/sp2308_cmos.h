/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef SP2308_CMOS_H
#define SP2308_CMOS_H

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
#define SP2308_ID             2308
#define SP2308_WIDTH          1920
#define SP2308_HEIGHT         1080

/*
 * sensor Register Address, page 1 (0xfd selects the page). 0x05/0x06 hold
 * the vertical blanking, not the frame length; 0x01 = 0x01 latches the
 * exposure, gain and blanking just written.
 */
#define SP2308_PAGE_ADDR      0xfd
#define SP2308_PAGE_AE        0x01
#define SP2308_EXPO_H_ADDR    0x03
#define SP2308_EXPO_L_ADDR    0x04
#define SP2308_AGAIN_ADDR     0x24
#define SP2308_VBLANK_H_ADDR  0x05
#define SP2308_VBLANK_L_ADDR  0x06
#define SP2308_TRIGGER_ADDR   0x01
#define SP2308_TRIGGER        0x01

/****************************************************************************
 * i2c bus config                                                           *
 ****************************************************************************/
/* sensor I2C bus config */
#define SP2308_I2C_ADDR    0x78
#define SP2308_ADDR_BYTE   1
#define SP2308_DATA_BYTE   1

/****************************************************************************
 * sensor lines configs                                                     *
 ****************************************************************************/
/* increase line */
#define SP2308_INCREASE_LINES 1  /* make real fps less than stand fps because NVR require */

/*
 * linear mode: 1109 + 1 lines without blanking, 49 lines of blanking at
 * 30 fps (the hi3516ev200 driver's values).
 */
#define SP2308_FULL_LINES_MAX_LINEAR  0xFFFF
#define SP2308_VBLANK_LINEAR   49
#define SP2308_FRAME_LINES     (1109 + SP2308_INCREASE_LINES)
#define SP2308_VMAX_LINEAR     (SP2308_FRAME_LINES + SP2308_VBLANK_LINEAR)
#define SP2308_FPS_MAX_LINEAR 30
#define SP2308_FPS_MIN_LINEAR 5.0
#define SP2308_WIDTH_LINEAR  1920
#define SP2308_HEIGHT_LINEAR 1080
#define SP2308_MODE_LINEAR 0

/****************************************************************************
 * sensor ae configs                                                        *
 ****************************************************************************/
#define SP2308_AGAIN_MIN    1024
#define SP2308_AGAIN_MAX    15872
#define SP2308_DGAIN_MIN    64
#define SP2308_DGAIN_MAX    64

#define DGAIN_ACCURACY         0.015625
#define FL_OFFSET_LINEAR       9

/****************************************************************************
 * sensor awb calibrate configs                                             *
 ****************************************************************************/
/* The hi3516ev200 SP2308 driver's calibration. */
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
/* Black level: 14-bit ISP scale, the sensor's 64 DN 10-bit pedestal << 4 */
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
    SP2308_1080P30_10BIT_LINEAR_MODE = 0,
    SP2308_MODE_MAX
} sp2308_res_mode;

ot_isp_sns_obj *sp2308_get_obj(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* SP2308_CMOS_H */
