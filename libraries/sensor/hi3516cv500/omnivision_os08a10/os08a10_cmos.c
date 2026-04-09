/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of os08a10_cmos.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "os08a10_cmos.h"
#include "os08a10_cmos_ex.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "mpi_isp.h"
#include "mpi_ae.h"
#include "mpi_awb.h"

#define OS08A10_ID 0x08a10

#define HIGH_8BITS(x) (((x)&0xff00) >> 8)
#define LOW_8BITS(x) ((x)&0x00ff)

#ifndef CLIP3
#define CLIP3(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#endif

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/
ISP_SNS_STATE_S *g_pastOS08A10[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define OS08A10_SENSOR_SET_CTX(dev, pstCtx) ((g_pastOS08A10[dev]) = (pstCtx))
#define OS08A10_SENSOR_RESET_CTX(dev) (g_pastOS08A10[dev] = HI_NULL)

static HI_U32 g_au32InitExposure[ISP_MAX_PIPE_NUM] = {0};
static HI_U32 g_au32LinesPer500ms[ISP_MAX_PIPE_NUM] = {0};
static HI_U16 g_au16InitWBGain[ISP_MAX_PIPE_NUM][ISP_RGB_CHN_NUM] = {{0}};
static HI_U16 g_au16SampleRgain[ISP_MAX_PIPE_NUM] = {0};
static HI_U16 g_au16SampleBgain[ISP_MAX_PIPE_NUM] = {0};
static HI_U16 g_init_ccm[ISP_MAX_PIPE_NUM][CCM_MATRIX_SIZE] = {{0}};
static HI_BOOL g_quick_start_en[ISP_MAX_PIPE_NUM] = {HI_FALSE};

static HI_BOOL g_abAERouteExValid[ISP_MAX_PIPE_NUM] = {0};
static ISP_AE_ROUTE_S g_astInitAERoute[ISP_MAX_PIPE_NUM] = {{0}};
static ISP_AE_ROUTE_EX_S g_astInitAERouteEx[ISP_MAX_PIPE_NUM] = {{0}};
static ISP_AE_ROUTE_S g_astInitAERouteSF[ISP_MAX_PIPE_NUM] = {{0}};
static ISP_AE_ROUTE_EX_S g_astInitAERouteSFEx[ISP_MAX_PIPE_NUM] = {{0}};

ISP_SNS_COMMBUS_U g_aunOS08A10BusInfo[ISP_MAX_PIPE_NUM] = {
    [0] = { .s8I2cDev = 0},
    [1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1}
};

ISP_SNS_COMMBUS_U *os08a10_get_bus_Info(VI_PIPE vi_pipe)
{
    return &g_aunOS08A10BusInfo[vi_pipe];
}

ISP_SNS_STATE_S *os08a10_get_ctx(VI_PIPE vi_pipe)
{
    return g_pastOS08A10[vi_pipe];
}

static ISP_FSWDR_MODE_E g_genFSWDRMode[ISP_MAX_PIPE_NUM] = {
    [0 ... ISP_MAX_PIPE_NUM - 1] = ISP_FSWDR_NORMAL_MODE
};

typedef struct hiOS08A10_STATE_S {
    HI_U32 u32BRL;
    HI_U32 u32RHS1_MAX;
    HI_U32 u32deltaRHS1;
} OS08A10_STATE_S;

OS08A10_STATE_S g_astOS08A10State[ISP_MAX_PIPE_NUM] = {{0}};

static HI_U32  g_au32MaxTimeGetCnt[ISP_MAX_PIPE_NUM] = {0};

static HI_U32 g_au32PreAGain[ISP_MAX_PIPE_NUM] = { 0x80, 0x80 };
static HI_U32 g_au32CurAGain[ISP_MAX_PIPE_NUM] = { 0x80, 0x80 };

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
#define OS08A10_FULL_LINES_MAX 0xE245
#define OS08A10_FULL_LINES_MAX_2TO1_WDR 0xE245  /* considering the YOUT_SIZE and bad frame */

/* OS08A10 Register Address */
#define OS08A10_EXPO_H_ADDR                       0x3501
#define OS08A10_EXPO_L_ADDR                       0x3502
#define OS08A10_AGAIN_H_ADDR                      0x3508
#define OS08A10_AGAIN_L_ADDR                      0x3509
#define OS08A10_DGAIN_H_ADDR                      0x350a
#define OS08A10_DGAIN_L_ADDR                      0x350b
#define OS08A10_VMAX_H_ADDR                       0x380e
#define OS08A10_VMAX_L_ADDR                       0x380f
#define OS08A10_R3740_ADDR                        0x3740
#define OS08A10_R3741_ADDR                        0x3741
/* OS08A10 2to1 WDR Register Address */
#define OS08A10_SHORT_EXPO_H_ADDR                0x3511
#define OS08A10_SHORT_EXPO_L_ADDR                0x3512
#define OS08A10_SHORT_AGAIN_H_ADDR               0x350c
#define OS08A10_SHORT_AGAIN_L_ADDR                0x350d
#define OS08A10_SHORT_DGAIN_H_ADDR                0x350e
#define OS08A10_SHORT_DGAIN_L_ADDR                0x350f

#define OS08A10_INCREASE_LINES 1 /* make real fps less than stand fps because NVR require */

#define OS08A10_VMAX_4k2k30_LINEAR                (2316 + OS08A10_INCREASE_LINES)

#define OS08A10_VMAX_4k2k30_WDR                   (2316 + OS08A10_INCREASE_LINES)

#define OS08A10_VMAX_1080P30_LINEAR               (2316 + OS08A10_INCREASE_LINES)

#define OS08A10_VMAX_1080P120_LINEAR              (1158 + OS08A10_INCREASE_LINES)

/* sensor fps mode */
#define OS08A10_SENSOR_8M_30FPS_10BIT_LINEAR_MODE         1
#define OS08A10_SENSOR_1080P30FPS_10BIT_LINEAR_MODE       2
#define OS08A10_SENSOR_1080P120FPS_10BI_LINEAR_MODE       3

#define OS08A10_RES_IS_8M_10BIT_LINEAR(w, h)    (((w) == 3840) && ((h) == 2160))

#define OS08A10_RES_IS_5M_12BIT_LINEAR(w, h)    (((w) == 2592) && ((h) == 1944))
#define OS08A10_RES_IS_4M_12BIT_LINEAR(w, h)    (((w) == 2592) && ((h) == 1520))
#define OS08A10_RES_IS_2M_12BIT_LINEAR(w, h)    (((w) == 1920) && ((h) == 1080))

static HI_VOID cmos_get_ae_comm_default(VI_PIPE vi_pipe,
                                        AE_SENSOR_DEFAULT_S *pstAeSnsDft,
                                        ISP_SNS_STATE_S *pstSnsState)
{
    HI_U32 u32Fll = 0;
    HI_U32 U32MaxFps = 0;
    if (pstSnsState->u8ImgMode == OS08A10_SENSOR_8M_30FPS_10BIT_LINEAR_MODE) {
        u32Fll = OS08A10_VMAX_4k2k30_LINEAR;
        U32MaxFps = 30; /* 30fps */
    }
    pstSnsState->u32FLStd = u32Fll;
    pstAeSnsDft->stIntTimeAccu.f32Offset = 0;
    pstAeSnsDft->u32MaxIntTime = (pstSnsState->u32FLStd > 8) ? (pstSnsState->u32FLStd - 8) : (0); /* sub 8 */
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FlickerFreq = 50 * 256; /* light flicker freq: 50Hz, accuracy: 256 */
    pstAeSnsDft->u32HmaxTimes = (1000000000) / SNS_DIV_0_TO_1(u32Fll * U32MaxFps); /* 1000000000ns */
    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
    pstAeSnsDft->stIntTimeAccu.f32Offset = 0;
    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 0.0078125; /* Accuracy 0.0078125 */
    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 0.0078125; /* Accuracy 0.0078125 */
    pstAeSnsDft->u32ISPDgainShift = 8; /* accuracy: 8 */
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 2 << pstAeSnsDft->u32ISPDgainShift; /* max 2 */

    memcpy_s(&pstAeSnsDft->stPirisAttr, sizeof(ISP_PIRIS_ATTR_S), &g_stPirisAttr, sizeof(ISP_PIRIS_ATTR_S));
    pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_4;
    pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_5_6;
    pstAeSnsDft->bAERouteExValid = HI_FALSE;
    pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
    pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;

    if (g_au32InitExposure[vi_pipe] == 0) {
        pstAeSnsDft->u32InitExposure = 130000; /* init 130000 */
    } else {
        pstAeSnsDft->u32InitExposure = g_au32InitExposure[vi_pipe];
    }

    if (g_au32LinesPer500ms[vi_pipe] == 0) {
        pstAeSnsDft->u32LinesPer500ms = (u32Fll * U32MaxFps) >> 1;
    } else {
        pstAeSnsDft->u32LinesPer500ms = g_au32LinesPer500ms[vi_pipe];
    }
    pstAeSnsDft->stQuickStart.bQuickStartEn = g_quick_start_en[vi_pipe];
    pstAeSnsDft->stQuickStart.u8BlackFrameNum = 0;
    return;
}
static HI_VOID cmos_get_ae_linear_default(VI_PIPE vi_pipe, ISP_SNS_STATE_S *pstSnsState,
    AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    pstAeSnsDft->au8HistThresh[0] = 0xd;
    pstAeSnsDft->au8HistThresh[1] = 0x28;
    pstAeSnsDft->au8HistThresh[2] = 0x60; /* index 2 */
    pstAeSnsDft->au8HistThresh[3] = 0x80; /* index 3 */

    pstAeSnsDft->u32MaxAgain = 1984; /* max 1984 */
    pstAeSnsDft->u32MinAgain = 128; /* min 128 */
    pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
    pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;
    pstAeSnsDft->u32MaxDgain = 2047; /* max 2047 */
    pstAeSnsDft->u32MinDgain = 128; /* min 128 */
    pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
    pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;
    pstAeSnsDft->u8AeCompensation = 0x38;
    pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;

    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8; /* sub 8 */
    pstAeSnsDft->u32MinIntTime = 2; /* min 2 */
    pstAeSnsDft->u32MaxIntTimeTarget = 65535; /* max 65535 */
    pstAeSnsDft->u32MinIntTimeTarget = pstAeSnsDft->u32MinIntTime;
    pstAeSnsDft->bAERouteExValid = g_abAERouteExValid[vi_pipe];
    memcpy_s(&pstAeSnsDft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S),
        &g_astInitAERoute[vi_pipe], sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&pstAeSnsDft->stAERouteAttrEx, sizeof(ISP_AE_ROUTE_EX_S),
        &g_astInitAERouteEx[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S));
    return;
}

static HI_VOID cmos_get_ae_2to1_line_wdr_default(VI_PIPE vi_pipe, ISP_SNS_STATE_S *pstSnsState,
    AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    pstAeSnsDft->au8HistThresh[0] = 0xc;
    pstAeSnsDft->au8HistThresh[1] = 0x18;
    pstAeSnsDft->au8HistThresh[2] = 0x60; /* index 2 */
    pstAeSnsDft->au8HistThresh[3] = 0x80; /* index 3 */
    pstAeSnsDft->u8AeCompensation = 0x38;
    pstAeSnsDft->u32MinIntTime = 27; /* min 27 */
    pstAeSnsDft->u32MaxIntTimeTarget = 65535; /* max 65535 */
    pstAeSnsDft->u32MinIntTimeTarget = pstAeSnsDft->u32MinIntTime;
    pstAeSnsDft->u32MaxIntTimeStep = 1000; /* MaxIntTimeStep 1000 */
    pstAeSnsDft->u32LFMinExposure = 15000000; /* LFMinExposure 15000000 */
    pstAeSnsDft->enAeExpMode = AE_EXP_LOWLIGHT_PRIOR;
    pstAeSnsDft->bAERouteExValid = g_abAERouteExValid[vi_pipe];
    memcpy_s(&pstAeSnsDft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S),
        &g_astInitAERoute[vi_pipe], sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&pstAeSnsDft->stAERouteAttrEx, sizeof(ISP_AE_ROUTE_EX_S),
        &g_astInitAERouteEx[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S));
    memcpy_s(&pstAeSnsDft->stAERouteSFAttr, sizeof(ISP_AE_ROUTE_S),
        &g_astInitAERouteSF[vi_pipe], sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&pstAeSnsDft->stAERouteSFAttrEx, sizeof(ISP_AE_ROUTE_EX_S),
        &g_astInitAERouteSFEx[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S));

    if (g_genFSWDRMode[vi_pipe] != ISP_FSWDR_LONG_FRAME_MODE) {
        pstAeSnsDft->enAeExpMode = AE_EXP_LOWLIGHT_PRIOR;
        pstAeSnsDft->u16ManRatioEnable = HI_TRUE;
        pstAeSnsDft->au32Ratio[0] = 0x100;
        pstAeSnsDft->au32Ratio[1] = 0x40;
        pstAeSnsDft->au32Ratio[2] = 0x40; /* array index 2 */
    }
    return;
}

static HI_S32 cmos_get_ae_default(VI_PIPE vi_pipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstAeSnsDft);
    OS08A10_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    cmos_get_ae_comm_default(vi_pipe, pstAeSnsDft, pstSnsState);

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE: /* linear mode */
            cmos_get_ae_linear_default(vi_pipe, pstSnsState, pstAeSnsDft);
            break;

        case WDR_MODE_2To1_LINE:
            cmos_get_ae_2to1_line_wdr_default(vi_pipe, pstSnsState, pstAeSnsDft);
            break;
    }
    return HI_SUCCESS;
}

/* the function of sensor set fps */
static HI_VOID cmos_fps_set(VI_PIPE vi_pipe, HI_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    HI_U32 u32MaxFps;
    HI_U32 u32Lines;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    OS08A10_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    switch (pstSnsState->u8ImgMode) {
        case OS08A10_SENSOR_8M_30FPS_10BIT_LINEAR_MODE:
            if ((f32Fps <= 30.0) && (f32Fps >= 1.2)) { /* range [1.2, 30.0] */
                u32MaxFps = 30; /* 30fps */
                u32Lines = OS08A10_VMAX_4k2k30_LINEAR * u32MaxFps / SNS_DIV_0_TO_1_FLOAT(f32Fps);
                pstSnsState->u32FLStd = u32Lines;
            } else {
                SNS_ERR_TRACE("Not support Fps: %f\n", f32Fps);
                return;
            }
            pstAeSnsDft->u32LinesPer500ms = OS08A10_VMAX_4k2k30_LINEAR * 15; /* 15: 30fps / 2 */
            break;
        case OS08A10_SENSOR_1080P120FPS_10BI_LINEAR_MODE:
            if ((f32Fps <= 120) && (f32Fps >= 30)) { /* range [30, 120] */
                u32Lines = OS08A10_VMAX_1080P120_LINEAR * 120 / f32Fps; /* 120fps */
            } else {
                printf("Not support Fps: %f\n", f32Fps);
                return;
            }
            u32Lines = (u32Lines > OS08A10_FULL_LINES_MAX) ? OS08A10_FULL_LINES_MAX : u32Lines;
            pstAeSnsDft->u32LinesPer500ms = OS08A10_VMAX_1080P120_LINEAR * 60; /* 60fps */
            break;

        default:
            SNS_ERR_TRACE("Not support this Mode!!!\n");
            return;
            break;
    }

    pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = HIGH_8BITS(u32Lines); /* index 4 */
    pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = LOW_8BITS(u32Lines); /* index 5 */

    pstSnsState->u32FLStd = u32Lines;

    pstAeSnsDft->f32Fps = f32Fps;

    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32HmaxTimes = (1000000000) / SNS_DIV_0_TO_1_FLOAT(pstSnsState->u32FLStd * f32Fps); /* 1000000000ns */
    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 0x48;
    } else {
        pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8; /* sub 8 */
    }
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];

    return;
}

static HI_VOID cmos_slow_framerate_set(VI_PIPE vi_pipe, HI_U32 u32FullLines,
    AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    OS08A10_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        u32FullLines = (u32FullLines > OS08A10_FULL_LINES_MAX_2TO1_WDR) ? OS08A10_FULL_LINES_MAX_2TO1_WDR
                       : u32FullLines;
    } else {
        u32FullLines = (u32FullLines > OS08A10_FULL_LINES_MAX) ? OS08A10_FULL_LINES_MAX : u32FullLines;
    }

    pstSnsState->au32FL[0] = u32FullLines;

    pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = HIGH_8BITS(pstSnsState->au32FL[0]); /* index 4 */
    pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = LOW_8BITS(pstSnsState->au32FL[0]); /* index 5 */

    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 0x48;
    } else {
        pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 8; /* sub 8 */
    }

    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static HI_VOID cmos_inttime_update(VI_PIPE vi_pipe, HI_U32 u32IntTime)
{
    HI_U32 u32Value = 0;

    static HI_BOOL bFirst[ISP_MAX_PIPE_NUM] = {1, 1};

    static HI_U32 u32ShortIntTime[ISP_MAX_PIPE_NUM] = {0};
    static HI_U32 u32LongIntTime[ISP_MAX_PIPE_NUM] = {0};
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    OS08A10_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        if (bFirst[vi_pipe]) { /* short exposure */
            pstSnsState->au32WDRIntTime[0] = u32IntTime;
            u32ShortIntTime[vi_pipe] = u32IntTime;

            pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = HIGH_8BITS(u32ShortIntTime[vi_pipe]); /* index 10 */
            pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = LOW_8BITS(u32ShortIntTime[vi_pipe]); /* index 11 */

            bFirst[vi_pipe] = HI_FALSE;
        } else { /* long exposure */
            pstSnsState->au32WDRIntTime[1] = u32IntTime;
            u32LongIntTime[vi_pipe] = (u32IntTime + u32ShortIntTime[vi_pipe] <= pstSnsState->au32FL[1] - 0x48) ?
                u32IntTime : pstSnsState->au32FL[1] - 0x48 - u32ShortIntTime[vi_pipe];

            pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = HIGH_8BITS(u32LongIntTime[vi_pipe]);
            pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = LOW_8BITS(u32LongIntTime[vi_pipe]);

            bFirst[vi_pipe] = HI_TRUE;
        }
    } else {
        u32Value = u32IntTime;

        pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = HIGH_8BITS(u32Value);
        pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = LOW_8BITS(u32Value);
    }

    return;
}

static HI_VOID cmos_gains_update(VI_PIPE vi_pipe, HI_U32 u32Again, HI_U32 u32Dgain)
{
    HI_U32 u32AGainReg;
    HI_U32 u32DGainReg = 0;
    HI_U32 u32AnalogTiming_1 = 0;
    HI_U32 u32AnalogTiming_2 = 0;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    OS08A10_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    g_au32PreAGain[vi_pipe] = g_au32CurAGain[vi_pipe];

    u32Again = CLIP3(u32Again, 0x80, 0x7FF);

    u32AGainReg = u32Again;

    g_au32CurAGain[vi_pipe] = u32Again;
    if (u32Again < 0x100) {
        u32AnalogTiming_1 = 0x07;
        u32AnalogTiming_2 = 0x03;
    } else if (u32Again < 0x200) {
        u32AnalogTiming_1 = 0x09;
        u32AnalogTiming_2 = 0x03;
    } else if (u32Again < 0x400) {
        u32AnalogTiming_1 = 0x10;
        u32AnalogTiming_2 = 0x03;
    } else {
        u32AnalogTiming_1 = 0x1b;
        u32AnalogTiming_2 = 0x03;
    }

    /* Digital Gain */
    if (u32Dgain < 0x80) {
        u32DGainReg = 0x400;
    } else if (u32Dgain < 0x800) {
        u32DGainReg = u32Dgain * 0x8;
    } else {
        u32DGainReg = 0x3fff;
    }

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        /*
        pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = ((u32AGainReg & 0xff00) >> 8);
        pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = (u32AGainReg & 0xff);
        pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = ((u32DGainReg & 0xff00) >> 8);
        pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = (u32DGainReg & 0xff);

        pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = ((u32AGainReg & 0xff00) >> 8);
        pstSnsState->astRegsInfo[0].astI2cData[13].u32Data = (u32AGainReg & 0xff);
        pstSnsState->astRegsInfo[0].astI2cData[14].u32Data = ((u32DGainReg & 0xff00) >> 8);
        pstSnsState->astRegsInfo[0].astI2cData[15].u32Data = (u32DGainReg & 0xff);

        pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = (u32AnalogTiming_1 & 0xff);
        pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = (u32AnalogTiming_2 & 0xff);
        */
    } else {
        pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = ((u32AGainReg & 0x3f00) >> 8); /* index 2, shift 8 */
        pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = (u32AGainReg & 0xff); /* index 3 */
        pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = ((u32DGainReg & 0x3f00) >> 8); /* index 8, shift 8 */
        pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = (u32DGainReg & 0xff); /* index 9 */

        pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = (u32AnalogTiming_1 & 0xff); /* index 6 */
        pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = (u32AnalogTiming_2 & 0xff); /* index 7 */
    }

    return;
}

static HI_VOID cmos_get_inttime_max(VI_PIPE vi_pipe, HI_U16 u16ManRatioEnable,
    HI_U32 *au32Ratio, HI_U32 *au32IntTimeMax,
    HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    HI_U32 u32IntTimeMaxTmp = 0;

    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER_VOID(au32Ratio);
    CMOS_CHECK_POINTER_VOID(au32IntTimeMax);
    CMOS_CHECK_POINTER_VOID(au32IntTimeMin);
    CMOS_CHECK_POINTER_VOID(pu32LFMaxIntTime);
    OS08A10_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if ((pstSnsState->enWDRMode == WDR_MODE_2To1_LINE)) {
        printf("null pointer when get ae sensor ExpRatio/IntTimeMax/IntTimeMin value!\n");
        return;
    }

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        /* Short + Long < VMax - 0x48; */
        u32IntTimeMaxTmp = ((pstSnsState->au32FL[1] - 0x48 - pstSnsState->au32WDRIntTime[0]) << 6) / /* shift 6 */
            SNS_DIV_0_TO_1(au32Ratio[0]);
        au32IntTimeMax[0] = (pstSnsState->au32FL[0] - 0x48) * 0x40 / (au32Ratio[0] + 0x40);

        au32IntTimeMax[0] = (u32IntTimeMaxTmp < au32IntTimeMax[0]) ? u32IntTimeMaxTmp : au32IntTimeMax[0];
        au32IntTimeMax[1] = (au32IntTimeMax[0] * au32Ratio[0]) >> 6; /* shift 6 */
        au32IntTimeMin[0] = 2; /* min 2 */
        au32IntTimeMin[1] = (au32IntTimeMin[0] * au32Ratio[0]) >> 6; /* shift 6 */
    }

    return;
}

/* Only used in FSWDR mode */
static HI_VOID cmos_ae_fswdr_attr_set(VI_PIPE vi_pipe, AE_FSWDR_ATTR_S *pstAeFSWDRAttr)
{
    CMOS_CHECK_POINTER_VOID(pstAeFSWDRAttr);

    g_genFSWDRMode[vi_pipe] = pstAeFSWDRAttr->enFSWDRMode;
    g_au32MaxTimeGetCnt[vi_pipe] = 0;
    return;
}

static HI_S32 cmos_init_ae_exp_function(AE_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    CMOS_CHECK_POINTER(pstExpFuncs);

    memset_s(pstExpFuncs, sizeof(AE_SENSOR_EXP_FUNC_S), 0, sizeof(AE_SENSOR_EXP_FUNC_S));

    pstExpFuncs->pfn_cmos_get_ae_default = cmos_get_ae_default;
    pstExpFuncs->pfn_cmos_fps_set = cmos_fps_set;
    pstExpFuncs->pfn_cmos_slow_framerate_set = cmos_slow_framerate_set;
    pstExpFuncs->pfn_cmos_inttime_update = cmos_inttime_update;
    pstExpFuncs->pfn_cmos_gains_update = cmos_gains_update;
    pstExpFuncs->pfn_cmos_get_inttime_max = cmos_get_inttime_max;
    pstExpFuncs->pfn_cmos_ae_fswdr_attr_set = cmos_ae_fswdr_attr_set;

    return HI_SUCCESS;
}

/* AWB default parameter and function */
#define CALIBRATE_STATIC_TEMP             4950
#define CALIBRATE_STATIC_WB_R_GAIN        531
#define CALIBRATE_STATIC_WB_GR_GAIN       256
#define CALIBRATE_STATIC_WB_GB_GAIN       256
#define CALIBRATE_STATIC_WB_B_GAIN        450

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1                  (-42)
#define CALIBRATE_AWB_P2                  298
#define CALIBRATE_AWB_Q1                  0
#define CALIBRATE_AWB_A1                  175532
#define CALIBRATE_AWB_B1                  128
#define CALIBRATE_AWB_C1                  (-125725)

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0
static HI_S32 cmos_get_awb_default(VI_PIPE vi_pipe, AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    HI_S32 i;

    CMOS_CHECK_POINTER(pstAwbSnsDft);
    OS08A10_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset_s(pstAwbSnsDft, sizeof(AWB_SENSOR_DEFAULT_S), 0, sizeof(AWB_SENSOR_DEFAULT_S));

    pstAwbSnsDft->u16WbRefTemp = CALIBRATE_STATIC_TEMP;

    pstAwbSnsDft->au16GainOffset[0] = CALIBRATE_STATIC_WB_R_GAIN;   /* 0: R channel */
    pstAwbSnsDft->au16GainOffset[1] = CALIBRATE_STATIC_WB_GR_GAIN;  /* 1: Gr channel */
    pstAwbSnsDft->au16GainOffset[2] = CALIBRATE_STATIC_WB_GB_GAIN;  /* 2: Gb channel */
    pstAwbSnsDft->au16GainOffset[3] = CALIBRATE_STATIC_WB_B_GAIN;   /* 3: B channel */

    pstAwbSnsDft->as32WbPara[0] = CALIBRATE_AWB_P1;       /* index0: p1 */
    pstAwbSnsDft->as32WbPara[1] = CALIBRATE_AWB_P2;       /* index1: p2 */
    pstAwbSnsDft->as32WbPara[2] = CALIBRATE_AWB_Q1;       /* index2: q1 */
    pstAwbSnsDft->as32WbPara[3] = CALIBRATE_AWB_A1;       /* index3: a1 */
    pstAwbSnsDft->as32WbPara[4] = CALIBRATE_AWB_B1;       /* index4: b1 */
    pstAwbSnsDft->as32WbPara[5] = CALIBRATE_AWB_C1;       /* index5: c1 */

    pstAwbSnsDft->u16GoldenRgain = GOLDEN_RGAIN;
    pstAwbSnsDft->u16GoldenBgain = GOLDEN_BGAIN;

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:
            memcpy_s(&pstAwbSnsDft->stCcm, sizeof(AWB_CCM_S), &g_stAwbCcm, sizeof(AWB_CCM_S));
            memcpy_s(&pstAwbSnsDft->stAgcTbl, sizeof(AWB_AGC_TABLE_S), &g_stAwbAgcTable, sizeof(AWB_AGC_TABLE_S));
            break;

        case WDR_MODE_2To1_LINE:
            memcpy_s(&pstAwbSnsDft->stCcm, sizeof(AWB_CCM_S), &g_stAwbCcmFsWdr, sizeof(AWB_CCM_S));
            memcpy_s(&pstAwbSnsDft->stAgcTbl, sizeof(AWB_AGC_TABLE_S), &g_stAwbAgcTableFSWDR, sizeof(AWB_AGC_TABLE_S));
            break;
    }

    pstAwbSnsDft->u16InitRgain = g_au16InitWBGain[vi_pipe][0]; /* 0: Rgain */
    pstAwbSnsDft->u16InitGgain = g_au16InitWBGain[vi_pipe][1]; /* 1: Ggain */
    pstAwbSnsDft->u16InitBgain = g_au16InitWBGain[vi_pipe][2]; /* 2: Bgain */
    pstAwbSnsDft->u16SampleRgain = g_au16SampleRgain[vi_pipe];
    pstAwbSnsDft->u16SampleBgain = g_au16SampleBgain[vi_pipe];
    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        pstAwbSnsDft->au16InitCCM[i] = g_init_ccm[vi_pipe][i];
    }

    return HI_SUCCESS;
}

static HI_S32 cmos_init_awb_exp_function(AWB_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    CMOS_CHECK_POINTER(pstExpFuncs);

    memset_s(pstExpFuncs, sizeof(AWB_SENSOR_EXP_FUNC_S), 0, sizeof(AWB_SENSOR_EXP_FUNC_S));

    pstExpFuncs->pfn_cmos_get_awb_default = cmos_get_awb_default;

    return HI_SUCCESS;
}

static ISP_CMOS_DNG_COLORPARAM_S g_stDngColorParam = {{ 378, 256, 430 }, { 439, 256, 439 }};
static HI_VOID cmos_get_isp_dng_default(ISP_SNS_STATE_S *pstSnsState, ISP_CMOS_DEFAULT_S *pstDef)
{
    memcpy_s(&pstDef->stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S),
        &g_stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S));

    switch (pstSnsState->u8ImgMode) {
        default:
        case OS08A10_SENSOR_8M_30FPS_10BIT_LINEAR_MODE:

            pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 12; /* 12bit */
            pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 4095; /* max 4095 */
            break;
    }

    pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleH.u32Denominator = 1;
    pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleH.u32Numerator = 1;
    pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleV.u32Denominator = 1;
    pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleV.u32Numerator = 1;
    pstDef->stSensorMode.stDngRawFormat.stCfaRepeatPatternDim.u16RepeatPatternDimRows = 2; /* pattern 2 */
    pstDef->stSensorMode.stDngRawFormat.stCfaRepeatPatternDim.u16RepeatPatternDimCols = 2; /* pattern 2 */
    pstDef->stSensorMode.stDngRawFormat.stBlcRepeatDim.u16BlcRepeatRows = 2; /* pattern 2 */
    pstDef->stSensorMode.stDngRawFormat.stBlcRepeatDim.u16BlcRepeatCols = 2; /* pattern 2 */
    pstDef->stSensorMode.stDngRawFormat.enCfaLayout = CFALAYOUT_TYPE_RECTANGULAR;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPlaneColor[0] = 0;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPlaneColor[1] = 1;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPlaneColor[2] = 2; /* index 2, CfaPlaneColor 2 */
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[0] = 0;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[1] = 1;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[2] = 1; /* index 2, CfaPattern 1 */
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[3] = 2; /* index 3, CfaPattern 2 */
    pstDef->stSensorMode.bValidDngRawFormat = HI_TRUE;
    return;
}

static void cmos_get_isp_linear_default(ISP_CMOS_DEFAULT_S *pstDef)
{
    pstDef->unKey.bit1Demosaic       = 1;
    pstDef->pstDemosaic              = &g_stIspDemosaic;
    pstDef->unKey.bit1Sharpen        = 1;
    pstDef->pstSharpen               = &g_stIspYuvSharpen;
    pstDef->unKey.bit1Drc            = 1;
    pstDef->pstDrc                   = &g_stIspDRC;
    pstDef->unKey.bit1Gamma          = 1;
    pstDef->pstGamma                 = &g_stIspGamma;
    pstDef->unKey.bit1BayerNr        = 1;
    pstDef->pstBayerNr               = &g_stIspBayerNr;
    pstDef->unKey.bit1AntiFalseColor = 1;
    pstDef->pstAntiFalseColor = &g_stIspAntiFalseColor;
    pstDef->unKey.bit1Ldci = 1;
    pstDef->pstLdci = &g_stIspLdci;
    pstDef->unKey.bit1Dehaze = 1;
    pstDef->pstDehaze = &g_stIspDehaze;
    pstDef->unKey.bit1Lcac = 1;
    pstDef->pstLcac = &g_stIspLCac;
    memcpy_s(&pstDef->stNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S), &g_stIspNoiseCalibratio,
             sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
    return;
}

static void cmos_get_isp_2to1_line_wdr_default(ISP_CMOS_DEFAULT_S *pstDef)
{
    pstDef->unKey.bit1Demosaic       = 1;
    pstDef->pstDemosaic              = &g_stIspDemosaicWdr;
    pstDef->unKey.bit1Sharpen        = 1;
    pstDef->pstSharpen               = &g_stIspYuvSharpenWdr;
    pstDef->unKey.bit1Drc            = 1;
    pstDef->pstDrc                   = &g_stIspDRCWDR;
    pstDef->unKey.bit1Gamma          = 1;
    pstDef->pstGamma                 = &g_stIspGammaFSWDR;
#ifdef CONFIG_HI_ISP_PREGAMMA_SUPPORT
    pstDef->unKey.bit1PreGamma       = 0;
    pstDef->pstPreGamma              = &g_stPreGamma;
#endif
    pstDef->unKey.bit1BayerNr        = 1;
    pstDef->pstBayerNr               = &g_stIspBayerNrWdr2To1;
    pstDef->unKey.bit1Detail         = 1;
    pstDef->pstDetail                = &g_stIspDetailWdr2To1;
#ifdef CONFIG_HI_ISP_CR_SUPPORT
    pstDef->unKey.bit1Ge             = 1;
    pstDef->pstGe                    = &g_stIspWdrGe;
#endif
    pstDef->unKey.bit1AntiFalseColor = 1;
    pstDef->pstAntiFalseColor        = &g_stIspWdrAntiFalseColor;
    pstDef->unKey.bit1Ldci           = 1;
    pstDef->pstLdci                  = &g_stIspWdrLdci;
    pstDef->unKey.bit1Dehaze         = 1;
    pstDef->pstDehaze                = &g_stIspDehazeWDR;
    pstDef->unKey.bit1Lcac           = 1;
    pstDef->pstLcac                  = &g_stIspLCacWdr;

    pstDef->stWdrSwitchAttr.au32ExpRatio[0] = 0x40;

    memcpy_s(&pstDef->stNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S), &g_stIspNoiseCalibratio,
             sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
    return;
}

static HI_S32 cmos_get_isp_default(VI_PIPE vi_pipe, ISP_CMOS_DEFAULT_S *pstDef)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstDef);
    OS08A10_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset_s(pstDef, sizeof(ISP_CMOS_DEFAULT_S), 0, sizeof(ISP_CMOS_DEFAULT_S));
#ifdef CONFIG_HI_ISP_CA_SUPPORT
    pstDef->unKey.bit1Ca       = 1;
    pstDef->pstCa              = &g_stIspCA;
#endif
    pstDef->unKey.bit1Clut     = 1;
    pstDef->pstClut            = &g_stIspCLUT;

    pstDef->unKey.bit1Dpc      = 1;
    pstDef->pstDpc             = &g_stCmosDpc;

    pstDef->unKey.bit1Wdr      = 1;
    pstDef->pstWdr             = &g_stIspWDR;
#ifdef CONFIG_HI_ISP_HLC_SUPPORT
    pstDef->unKey.bit1Hlc      = 0;
    pstDef->pstHlc             = &g_stIspHlc;
#endif
    pstDef->unKey.bit1Lsc      = 1;
    pstDef->pstLsc             = &g_stCmosLsc;
#ifdef CONFIG_HI_ISP_EDGEMARK_SUPPORT
    pstDef->unKey.bit1EdgeMark = 0;
    pstDef->pstEdgeMark        = &g_stIspEdgeMark;
#endif
#ifdef CONFIG_HI_ISP_CR_SUPPORT
    pstDef->unKey.bit1Ge       = 1;
    pstDef->pstGe              = &g_stIspGe;
#endif
    pstDef->unKey.bit1Detail   = 1;
    pstDef->pstDetail          = &g_stIspDetail;

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:

            cmos_get_isp_linear_default(pstDef);
            break;

        case WDR_MODE_2To1_LINE:

            cmos_get_isp_2to1_line_wdr_default(pstDef);

            break;
    }
    pstDef->stSensorMode.u32SensorID = OS08A10_ID;
    pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

    cmos_get_isp_dng_default(pstSnsState, pstDef);
    return HI_SUCCESS;
}
static HI_S32 cmos_get_isp_black_level(VI_PIPE vi_pipe, ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    OS08A10_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    if (pstBlackLevel == HI_NULL) {
        printf("null pointer when get isp black level value!\n");
        return -1;
    }

    /* Don't need to update black level when iso change */
    pstBlackLevel->bUpdate = HI_FALSE;

    if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
        pstBlackLevel->au16BlackLevel[0] = 254; /* 0: R channel 254 */
        pstBlackLevel->au16BlackLevel[1] = 256; /* 1: Gr channel 256 */
        pstBlackLevel->au16BlackLevel[2] = 256; /* 2: Gb channel 256 */
        pstBlackLevel->au16BlackLevel[3] = 252; /* 3: B channel 252 */
    } else if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        pstBlackLevel->au16BlackLevel[0] = 254; /* 0: R channel 254 */
        pstBlackLevel->au16BlackLevel[1] = 256; /* 1: Gr channel 256 */
        pstBlackLevel->au16BlackLevel[2] = 256; /* 2: Gb channel 256 */
        pstBlackLevel->au16BlackLevel[3] = 252; /* 3: B channel 252 */
    } else {
        pstBlackLevel->au16BlackLevel[0] = 254; /* 0: R channel 254 */
        pstBlackLevel->au16BlackLevel[1] = 256; /* 1: Gr channel 256 */
        pstBlackLevel->au16BlackLevel[2] = 256; /* 2: Gb channel 256 */
        pstBlackLevel->au16BlackLevel[3] = 252; /* 3: B channel 252 */
    }

    return 0;
}
static HI_VOID cmos_set_pixel_detect(VI_PIPE vi_pipe, HI_BOOL bEnable)
{
    HI_U32 u32FullLines_5Fps = 0;
    HI_U32 u32MaxIntTime_5Fps;

    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    OS08A10_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        return;
    } else {
        if (pstSnsState->u8ImgMode == OS08A10_SENSOR_8M_30FPS_10BIT_LINEAR_MODE) {
            u32FullLines_5Fps = (OS08A10_VMAX_4k2k30_LINEAR * 30) / 5; /* 30fps, 5fps */
        } else if (pstSnsState->u8ImgMode == OS08A10_SENSOR_1080P30FPS_10BIT_LINEAR_MODE) {
            u32FullLines_5Fps = (OS08A10_VMAX_1080P30_LINEAR * 30) / 5; /* 30fps, 5fps */
        } else {
            return;
        }
    }

    u32MaxIntTime_5Fps = u32FullLines_5Fps - 8; /* sub 8 */

    if (bEnable) { /* setup for ISP pixel calibration mode */
        OS08A10_write_register(vi_pipe, OS08A10_AGAIN_H_ADDR, 0x00);
        OS08A10_write_register(vi_pipe, OS08A10_AGAIN_L_ADDR, 0x80);

        OS08A10_write_register(vi_pipe, OS08A10_VMAX_L_ADDR, LOW_8BITS(u32FullLines_5Fps));
        OS08A10_write_register(vi_pipe, OS08A10_VMAX_H_ADDR, HIGH_8BITS(u32FullLines_5Fps));

        OS08A10_write_register(vi_pipe, OS08A10_EXPO_L_ADDR, LOW_8BITS(u32MaxIntTime_5Fps));
        OS08A10_write_register(vi_pipe, OS08A10_EXPO_H_ADDR, HIGH_8BITS(u32MaxIntTime_5Fps));
    } else { /* setup for ISP 'normal mode' */
        pstSnsState->u32FLStd = (pstSnsState->u32FLStd > OS08A10_FULL_LINES_MAX) ? OS08A10_FULL_LINES_MAX
                                : pstSnsState->u32FLStd;
        OS08A10_write_register(vi_pipe, OS08A10_VMAX_L_ADDR, LOW_8BITS(pstSnsState->u32FLStd));
        OS08A10_write_register(vi_pipe, OS08A10_VMAX_H_ADDR, HIGH_8BITS(pstSnsState->u32FLStd));
        pstSnsState->bSyncInit = HI_FALSE;
    }

    return;
}

static HI_S32 cmos_set_wdr_mode(VI_PIPE vi_pipe, HI_U8 u8Mode)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    OS08A10_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    pstSnsState->bSyncInit = HI_FALSE;

    switch (u8Mode & 0x3F) {
        case WDR_MODE_NONE:
            pstSnsState->enWDRMode = WDR_MODE_NONE;
            printf("linear mode\n");
            break;

        case WDR_MODE_2To1_LINE:
            pstSnsState->enWDRMode = WDR_MODE_2To1_LINE;
            printf("2to1 line WDR mode\n");
            break;

        default:
            SNS_ERR_TRACE("NOT support this mode!\n");
            return HI_FAILURE;
    }

    memset_s(pstSnsState->au32WDRIntTime, sizeof(pstSnsState->au32WDRIntTime), 0, sizeof(pstSnsState->au32WDRIntTime));

    return HI_SUCCESS;
}

static HI_VOID cmos_comm_sns_reg_info_init(VI_PIPE vi_pipe, ISP_SNS_STATE_S *pstSnsState)
{
    HI_S32 i;
    pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
    pstSnsState->astRegsInfo[0].unComBus.s8I2cDev = g_aunOS08A10BusInfo[vi_pipe].s8I2cDev;
    pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2; /* DelayMax 2 */
    pstSnsState->astRegsInfo[0].u32RegNum = 10; /* RegNum 10 */
    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        pstSnsState->astRegsInfo[0].u32RegNum += 6; /* RegNum add 6 */
    }
    for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
        pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
        pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr = OS08A10_I2C_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[i].u32AddrByteNum = OS08A10_ADDR_BYTE;
        pstSnsState->astRegsInfo[0].astI2cData[i].u32DataByteNum = OS08A10_DATA_BYTE;
    }
    pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
    pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr = OS08A10_EXPO_H_ADDR;
    pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
    pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr = OS08A10_EXPO_L_ADDR;
    pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0; /* index 2 */
    pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr = OS08A10_AGAIN_H_ADDR; /* index 2 */
    pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 0; /* index 3 */
    pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr = OS08A10_AGAIN_L_ADDR; /* index 3 */
    pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 0; /* index 8 */
    pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr = OS08A10_DGAIN_H_ADDR; /* index 8 */
    pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum = 0; /* index 9 */
    pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr = OS08A10_DGAIN_L_ADDR; /* index 9 */
    pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 1; /* index 4 */
    pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr = OS08A10_VMAX_H_ADDR; /* index 4 */
    pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 1; /* index 5 */
    pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr = OS08A10_VMAX_L_ADDR; /* index 5 */
    pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 1; /* index 6 */
    pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr = OS08A10_R3740_ADDR; /* index 6 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 1; /* index 7 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr = OS08A10_R3741_ADDR; /* index 7 */
    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        pstSnsState->astRegsInfo[0].astI2cData[10].u8DelayFrmNum = 0; /* index 10 */
        pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr = OS08A10_SHORT_EXPO_H_ADDR; /* index 10 */
        pstSnsState->astRegsInfo[0].astI2cData[11].u8DelayFrmNum = 0; /* index 11 */
        pstSnsState->astRegsInfo[0].astI2cData[11].u32RegAddr = OS08A10_SHORT_EXPO_L_ADDR; /* index 11 */
        pstSnsState->astRegsInfo[0].astI2cData[12].u8DelayFrmNum = 0; /* index 12 */
        pstSnsState->astRegsInfo[0].astI2cData[12].u32RegAddr = OS08A10_SHORT_AGAIN_H_ADDR; /* index 12 */
        pstSnsState->astRegsInfo[0].astI2cData[13].u8DelayFrmNum = 0; /* index 13 */
        pstSnsState->astRegsInfo[0].astI2cData[13].u32RegAddr = OS08A10_SHORT_AGAIN_L_ADDR; /* index 13 */
        pstSnsState->astRegsInfo[0].astI2cData[14].u8DelayFrmNum = 0; /* index 14 */
        pstSnsState->astRegsInfo[0].astI2cData[14].u32RegAddr = OS08A10_SHORT_DGAIN_H_ADDR; /* index 14 */
        pstSnsState->astRegsInfo[0].astI2cData[15].u8DelayFrmNum = 0; /* index 15 */
        pstSnsState->astRegsInfo[0].astI2cData[15].u32RegAddr = OS08A10_SHORT_DGAIN_L_ADDR; /* index 15 */
    }
    pstSnsState->bSyncInit = HI_TRUE;
    return;
}

static HI_VOID cmos_sns_reg_info_update(VI_PIPE vi_pipe, ISP_SNS_STATE_S *pstSnsState)
{
    HI_S32 i;

    for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
        if (pstSnsState->astRegsInfo[0].astI2cData[i].u32Data ==
            pstSnsState->astRegsInfo[1].astI2cData[i].u32Data) {
            pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = HI_FALSE;
        } else {
            pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
        }
    }
    return;
}

static HI_S32 cmos_get_sns_regs_info(VI_PIPE vi_pipe, ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstSnsRegsInfo);
    OS08A10_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    if ((pstSnsState->bSyncInit == HI_FALSE) || (pstSnsRegsInfo->bConfig == HI_FALSE)) {
        cmos_comm_sns_reg_info_init(vi_pipe, pstSnsState);
    } else {
        cmos_sns_reg_info_update(vi_pipe, pstSnsState);
    }

    pstSnsRegsInfo->bConfig = HI_FALSE;
    memcpy_s(pstSnsRegsInfo, sizeof(ISP_SNS_REGS_INFO_S),
        &pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S));
    memcpy_s(&pstSnsState->astRegsInfo[1], sizeof(ISP_SNS_REGS_INFO_S),
        &pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S));

    pstSnsState->au32FL[1] = pstSnsState->au32FL[0];
    return HI_SUCCESS;
}

static HI_S32 cmos_set_image_mode(VI_PIPE vi_pipe, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    HI_U8 u8SensorImageMode;

    CMOS_CHECK_POINTER(pstSensorImageMode);
    OS08A10_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    u8SensorImageMode = pstSnsState->u8ImgMode;
    pstSnsState->bSyncInit = HI_FALSE;

    printf("cmos_set_image_mode width: %d, height: %d, snsmode: %d, wdrmode:%d\n",
           pstSensorImageMode->u16Width, pstSensorImageMode->u16Height,
           pstSensorImageMode->u8SnsMode, pstSnsState->enWDRMode);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        {
            SNS_ERR_TRACE("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
                          pstSensorImageMode->u16Width,
                          pstSensorImageMode->u16Height,
                          pstSensorImageMode->f32Fps,
                          pstSnsState->enWDRMode);
            return HI_FAILURE;
        }
    } else if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
        if (OS08A10_RES_IS_8M_10BIT_LINEAR(pstSensorImageMode->u16Width, pstSensorImageMode->u16Height) ||
            OS08A10_RES_IS_4M_12BIT_LINEAR(pstSensorImageMode->u16Width, pstSensorImageMode->u16Height)) {
            u8SensorImageMode      = OS08A10_SENSOR_8M_30FPS_10BIT_LINEAR_MODE;
            pstSnsState->u32FLStd  = OS08A10_VMAX_4k2k30_LINEAR;
        } else {
            SNS_ERR_TRACE("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
                          pstSensorImageMode->u16Width, pstSensorImageMode->u16Height,
                          pstSensorImageMode->f32Fps, pstSnsState->enWDRMode);
            return HI_FAILURE;
        }
    } else {
        SNS_ERR_TRACE("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
                      pstSensorImageMode->u16Width, pstSensorImageMode->u16Height,
                      pstSensorImageMode->f32Fps, pstSnsState->enWDRMode);
        return HI_FAILURE;
    }

    /* Sensor first init */
    if (pstSnsState->bInit == HI_FALSE) {
        pstSnsState->u8ImgMode = u8SensorImageMode;
        return HI_SUCCESS;
    }

    if ((pstSnsState->bInit == HI_TRUE) && (u8SensorImageMode == pstSnsState->u8ImgMode)) {
        /* Don't need to switch SensorImageMode */
        return ISP_DO_NOT_NEED_SWITCH_IMAGEMODE;
    }
    pstSnsState->u8ImgMode = u8SensorImageMode;
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstSnsState->au32FL[1] = pstSnsState->au32FL[0];

    return HI_SUCCESS;
}

static HI_VOID sensor_global_init(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    OS08A10_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->bInit = HI_FALSE;
    pstSnsState->bSyncInit = HI_FALSE;
    pstSnsState->u8ImgMode = OS08A10_SENSOR_8M_30FPS_10BIT_LINEAR_MODE;
    pstSnsState->enWDRMode = WDR_MODE_NONE;
    pstSnsState->u32FLStd = OS08A10_VMAX_4k2k30_LINEAR;
    pstSnsState->au32FL[0] = OS08A10_VMAX_4k2k30_LINEAR;
    pstSnsState->au32FL[1] = OS08A10_VMAX_4k2k30_LINEAR;

    memset_s(&pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset_s(&pstSnsState->astRegsInfo[1], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
}

static HI_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    CMOS_CHECK_POINTER(pstSensorExpFunc);

    memset_s(pstSensorExpFunc, sizeof(ISP_SENSOR_EXP_FUNC_S), 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init = OS08A10_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit = OS08A10_exit;
    pstSensorExpFunc->pfn_cmos_sensor_global_init = sensor_global_init;
    pstSensorExpFunc->pfn_cmos_set_image_mode = cmos_set_image_mode;
    pstSensorExpFunc->pfn_cmos_set_wdr_mode = cmos_set_wdr_mode;

    pstSensorExpFunc->pfn_cmos_get_isp_default = cmos_get_isp_default;
    pstSensorExpFunc->pfn_cmos_get_isp_black_level = cmos_get_isp_black_level;
    pstSensorExpFunc->pfn_cmos_set_pixel_detect = cmos_set_pixel_detect;
    pstSensorExpFunc->pfn_cmos_get_sns_reg_info = cmos_get_sns_regs_info;

    return HI_SUCCESS;
}

/****************************************************************************
 * callback structure                                                       *
 ****************************************************************************/
static HI_S32 OS08A10_set_bus_info(VI_PIPE vi_pipe, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunOS08A10BusInfo[vi_pipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

    return HI_SUCCESS;
}

static HI_S32 sensor_ctx_init(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    OS08A10_SENSOR_GET_CTX(vi_pipe, pastSnsStateCtx);

    if (pastSnsStateCtx == HI_NULL) {
        pastSnsStateCtx = (ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
        if (pastSnsStateCtx == HI_NULL) {
            SNS_ERR_TRACE("Isp[%d] SnsCtx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset_s(pastSnsStateCtx, sizeof(ISP_SNS_STATE_S), 0, sizeof(ISP_SNS_STATE_S));

    OS08A10_SENSOR_SET_CTX(vi_pipe, pastSnsStateCtx);

    return HI_SUCCESS;
}

static HI_VOID sensor_ctx_exit(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    OS08A10_SENSOR_GET_CTX(vi_pipe, pastSnsStateCtx);
    SENSOR_FREE(pastSnsStateCtx);
    OS08A10_SENSOR_RESET_CTX(vi_pipe);
    return;
}

static HI_S32 sensor_register_callback(VI_PIPE vi_pipe, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib)
{
    HI_S32 s32Ret;
    ISP_SENSOR_REGISTER_S stIspRegister;
    AE_SENSOR_REGISTER_S stAeRegister;
    AWB_SENSOR_REGISTER_S stAwbRegister;
    ISP_SNS_ATTR_INFO_S stSnsAttrInfo;

    CMOS_CHECK_POINTER(pstAeLib);
    CMOS_CHECK_POINTER(pstAwbLib);

    s32Ret = sensor_ctx_init(vi_pipe);
    if (s32Ret != HI_SUCCESS) {
        return HI_FAILURE;
    }

    stSnsAttrInfo.eSensorId = OS08A10_ID;

    s32Ret = cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("cmos init exp function failed!\n");
        return HI_FAILURE;
    }
    s32Ret = HI_MPI_ISP_SensorRegCallBack(vi_pipe, &stSnsAttrInfo, &stIspRegister);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor register callback function failed!\n");
        return s32Ret;
    }

    s32Ret = cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("cmos init ae exp function failed!\n");
        return HI_FAILURE;
    }
    s32Ret = HI_MPI_AE_SensorRegCallBack(vi_pipe, pstAeLib, &stSnsAttrInfo, &stAeRegister);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor register callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("cmos init awb exp function failed!\n");
        return HI_FAILURE;
    }
    s32Ret = HI_MPI_AWB_SensorRegCallBack(vi_pipe, pstAwbLib, &stSnsAttrInfo, &stAwbRegister);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor register callback function to awb lib failed!\n");
        return s32Ret;
    }

    return HI_SUCCESS;
}

static HI_S32 sensor_unregister_callback(VI_PIPE vi_pipe, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib)
{
    HI_S32 s32Ret;

    CMOS_CHECK_POINTER(pstAeLib);
    CMOS_CHECK_POINTER(pstAwbLib);

    s32Ret = HI_MPI_ISP_SensorUnRegCallBack(vi_pipe, OS08A10_ID);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AE_SensorUnRegCallBack(vi_pipe, pstAeLib, OS08A10_ID);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AWB_SensorUnRegCallBack(vi_pipe, pstAwbLib, OS08A10_ID);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function to awb lib failed!\n");
        return s32Ret;
    }

    sensor_ctx_exit(vi_pipe);

    return HI_SUCCESS;
}

static HI_S32 sensor_set_init(VI_PIPE vi_pipe, ISP_INIT_ATTR_S *pstInitAttr)
{
    HI_S32 i;
    CMOS_CHECK_POINTER(pstInitAttr);

    g_au32InitExposure[vi_pipe]  = pstInitAttr->u32Exposure;
    g_au32LinesPer500ms[vi_pipe] = pstInitAttr->u32LinesPer500ms;
    g_au16InitWBGain[vi_pipe][0] = pstInitAttr->u16WBRgain; /* 0: Rgain */
    g_au16InitWBGain[vi_pipe][1] = pstInitAttr->u16WBGgain; /* 1: Ggain */
    g_au16InitWBGain[vi_pipe][2] = pstInitAttr->u16WBBgain; /* 2: Bgain */
    g_au16SampleRgain[vi_pipe]   = pstInitAttr->u16SampleRgain;
    g_au16SampleBgain[vi_pipe]   = pstInitAttr->u16SampleBgain;
    g_quick_start_en[vi_pipe] = pstInitAttr->stQuickStart.bQuickStartEn;

    g_abAERouteExValid[vi_pipe] = pstInitAttr->bAERouteExValid;
    memcpy_s(&g_astInitAERoute[vi_pipe], sizeof(ISP_AE_ROUTE_S), &pstInitAttr->stAERoute, sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&g_astInitAERouteEx[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S), &pstInitAttr->stAERouteEx,
             sizeof(ISP_AE_ROUTE_EX_S));
    memcpy_s(&g_astInitAERouteSF[vi_pipe], sizeof(ISP_AE_ROUTE_S), &pstInitAttr->stAERouteSF, sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&g_astInitAERouteSFEx[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S), &pstInitAttr->stAERouteSFEx,
             sizeof(ISP_AE_ROUTE_EX_S));

    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        g_init_ccm[vi_pipe][i] = pstInitAttr->au16CCM[i];
    }

    return HI_SUCCESS;
}

ISP_SNS_OBJ_S stSnsOS08A10Obj = {
    .pfnRegisterCallback    = sensor_register_callback,
    .pfnUnRegisterCallback  = sensor_unregister_callback,
    .pfnStandby             = OS08A10_standby,
    .pfnRestart             = OS08A10_restart,
    .pfnMirrorFlip          = HI_NULL,
    .pfnWriteReg            = OS08A10_write_register,
    .pfnReadReg             = OS08A10_read_register,
    .pfnSetBusInfo          = OS08A10_set_bus_info,
    .pfnSetInit             = sensor_set_init
};

