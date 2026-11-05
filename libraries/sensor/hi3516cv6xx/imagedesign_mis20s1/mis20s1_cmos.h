/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef MIS20S1_CMOS_H
#define MIS20S1_CMOS_H

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
/*
 * sensor ID & Resolution; chip ID 0x20e1 at 0x3000/0x3001. 201 is the
 * sns_id the vendor driver reports to the ISP.
 */
#define MIS20S1_ID             201
#define MIS20S1_WIDTH          1920
#define MIS20S1_HEIGHT         1080

/*
 * sensor Register Address: big-endian pairs, high byte first. These are the
 * registers, and the order, the vendor's Hi3516CV610 firmware hands the ISP
 * to sync in linear mode. 0x3008 is written 1 after the others, every time.
 */
#define MIS20S1_EXPO_ADDR_H    0x3100 /* integration time in lines */
#define MIS20S1_EXPO_ADDR_L    0x3101
#define MIS20S1_AGAIN_ADDR_H   0x3106 /* analog gain code, bits 9:8 */
#define MIS20S1_AGAIN_ADDR_L   0x3107
#define MIS20S1_DGAIN_ADDR_H   0x420b /* digital gain in 1/256 */
#define MIS20S1_DGAIN_ADDR_L   0x420c
#define MIS20S1_VMAX_ADDR_H    0x310e /* frame length in lines */
#define MIS20S1_VMAX_ADDR_L    0x310f
#define MIS20S1_HCG_ADDR       0x310c /* 1: high conversion gain */
#define MIS20S1_UPDATE_ADDR    0x3008

/* The sensor's own conversion gain ratio, in tenths: OTP 0x2003/0x2004. */
#define MIS20S1_DCG_OTP_INT    0x2003
#define MIS20S1_DCG_OTP_FRAC   0x2004
#define MIS20S1_DCG_DEFAULT    90 /* 9.0x, when the OTP reads 0 or 0xff */

/****************************************************************************
 * i2c bus config                                                           *
 ****************************************************************************/
/* 0x60, where the vendor firmware addresses it */
#define MIS20S1_I2C_ADDR    0x60
#define MIS20S1_ADDR_BYTE   2
#define MIS20S1_DATA_BYTE   1

/****************************************************************************
 * sensor lines configs                                                     *
 ****************************************************************************/
/*
 * linear mode: 1125 lines at 30 fps, exposure 4 lines short of the frame.
 * The frame is allowed to 0x3fff lines, not the 0xffff the vendor's mode
 * table gives: the register is 14 bits wide (Vmax[13:8] at 0x310e, as the
 * GK7206V100 driver for this sensor has it), so a longer frame would wrap.
 */
#define MIS20S1_VMAX_LINEAR            1125
#define MIS20S1_FULL_LINES_MAX_LINEAR  0x3fff
#define MIS20S1_FPS_MAX_LINEAR 30
#define MIS20S1_FPS_MIN_LINEAR \
    ((td_float)(MIS20S1_VMAX_LINEAR * MIS20S1_FPS_MAX_LINEAR) / MIS20S1_FULL_LINES_MAX_LINEAR)
#define MIS20S1_WIDTH_LINEAR  1920
#define MIS20S1_HEIGHT_LINEAR 1080
#define MIS20S1_MODE_LINEAR 0

/****************************************************************************
 * sensor ae configs                                                        *
 ****************************************************************************/
/*
 * As the vendor driver: analog gain 1x (0x400) .. 288x (0x48000) in 1/1024,
 * the top of it with the sensor in high conversion gain; sensor digital gain
 * 1x .. 2x in 1/1024; exposure in single lines from 1.
 */
#define MIS20S1_AGAIN_MIN    0x400
#define MIS20S1_AGAIN_MAX    0x48000
#define MIS20S1_DGAIN_MIN    0x400
#define MIS20S1_DGAIN_MAX    0x800
#define AGAIN_ACCURACY       0.0009765625
#define DGAIN_ACCURACY       0.0009765625
#define INT_TIME_ACCURACY    1
#define INT_TIME_OFFSET      0.182
#define FL_OFFSET_LINEAR     4
#define MIS20S1_INT_TIME_MIN 1
#define MIS20S1_INT_TIME_MAX (MIS20S1_FULL_LINES_MAX_LINEAR - FL_OFFSET_LINEAR)
/* the analog gain code: gain = 1024 / (1024 - code), up to 32x (992) */
#define MIS20S1_AGAIN_CODE_1X  0x400
#define MIS20S1_AGAIN_CODE_MAX 992

/****************************************************************************
 * sensor awb calibrate configs                                             *
 ****************************************************************************/
/* The vendor firmware's own, for this sensor in linear mode. */
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
#define CALIBRATE_AWB_C1 (-136962)

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
/* Black level, 14-bit ISP scale, every channel, as the vendor driver */
#define BLACK_LEVEL_DEFAULT       1040

/* DNG */
#define DNG_RAW_FORMAT_BIT_LINEAR         12 /* raw 12 bit */
#define DNG_RAW_FORMAT_WHITE_LEVEL_LINEAR 4095 /* 2^12 - 1 */

/****************************************************************************
 * assist function macros                                                   *
 ****************************************************************************/
#define high_8bits(x) (((x) & 0xff00) >> 8)
#define low_8bits(x)  ((x) & 0x00ff)

/****************************************************************************
 * sensor data type defines                                                 *
 ****************************************************************************/
typedef enum {
    MIS20S1_1080P30_12BIT_LINEAR_MODE = 0,
    MIS20S1_MODE_MAX
} mis20s1_res_mode;

ot_isp_sns_obj *mis20s1_get_obj(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* MIS20S1_CMOS_H */
