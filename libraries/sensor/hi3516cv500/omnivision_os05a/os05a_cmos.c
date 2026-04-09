/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of os05a_cmos.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "os05a_cmos.h"
#include "os05a_cmos_ex.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "mpi_isp.h"
#include "mpi_ae.h"
#include "mpi_awb.h"

#define OS05A_ID 05
#define HIGH_8BITS(x) (((x)&0xff00) >> 8)
#define LOW_8BITS(x) ((x)&0x00ff)

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ? (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ? (b) : (a))
#endif

ISP_SNS_STATE_S *g_pastOs05a[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define OS05A_SENSOR_SET_CTX(dev, pstCtx)   ((g_pastOs05a[dev]) = (pstCtx))
#define OS05A_SENSOR_RESET_CTX(dev)         (g_pastOs05a[dev] = HI_NULL)

ISP_SNS_COMMBUS_U g_aunOs05aBusInfo[ISP_MAX_PIPE_NUM] = {
    [0] = {
        .s8I2cDev = 0
    },
    [1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1 }
};

ISP_SNS_COMMBUS_U *os05a_get_bus_Info(VI_PIPE vi_pipe)
{
    return &g_aunOs05aBusInfo[vi_pipe];
}

ISP_SNS_STATE_S *os05a_get_ctx(VI_PIPE vi_pipe)
{
    return g_pastOs05a[vi_pipe];
}

static ISP_FSWDR_MODE_E g_genFSWDRMode[ISP_MAX_PIPE_NUM] = {
    [0 ... ISP_MAX_PIPE_NUM - 1] = ISP_FSWDR_NORMAL_MODE
};

static HI_U32 g_au32MaxTimeGetCnt[ISP_MAX_PIPE_NUM] = {0};

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

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
#define OS05A_FULL_LINES_MAX 0x9240

#define VMAX_ADDR_H 0x380E
#define VMAX_ADDR_L 0x380F
#define AGAIN_H 0x3508
#define AGAIN_L 0x3509
#define DGAIN_H 0x350A
#define DGAIN_L 0x350B
#define SHORT_AGAIN_H 0x350C
#define SHORT_AGAIN_L 0x350D
#define SHORT_DGAIN_H 0x350E
#define SHORT_DGAIN_L 0x350F
#define EXPOSURE_TIME_LONG_H 0x3501
#define EXPOSURE_TIME_LONG_L 0x3502
#define EXPOSURE_TIME_SHORT_H 0x3511
#define EXPOSURE_TIME_SHORT_L 0x3512
#define LINE_LEN_PCK 0x300C

#define INCREASE_LINES_LINEAR             0 /* make real fps less than stand fps because NVR require */
#define INCREASE_LINES_WDR                4 /* make real fps less than stand fps because NVR require */
#define OS05A_VMAX_5M30_12bit_LINEAR      (0x921 + INCREASE_LINES_LINEAR)
#define OS05A_VMAX_5M30_10bit_WDR         (0x9c0 + INCREASE_LINES_WDR)

#define Os05a_5M_2688x1944_12bit_linear30 0
#define Os05a_5M_2688x1944_10bit_wdr30    1

static HI_U32   g_u32STimeFps = 30;
static HI_FLOAT g_f32AGain = 0;
static HI_FLOAT g_f32DGain = 0;
#define OS05A_FRAME_TYPE_SHORT            1
#define OS05A_FRAME_TYPE_LONG             0
#define OS05A_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState)\
    do{\
        SNS_ERR_TRACE("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n", \
                      (pstSensorImageMode)->u16Width,  \
                      (pstSensorImageMode)->u16Height, \
                      (pstSensorImageMode)->f32Fps,    \
                      (pstSnsState)->enWDRMode);\
    }while (0)

static HI_VOID cmos_get_ae_comm_default(VI_PIPE vi_pipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft,
    ISP_SNS_STATE_S *pstSnsState)
{
    HI_U32 u32Fll = 0;
    HI_FLOAT f32maxFps = 0;
    if (pstSnsState->u8ImgMode == Os05a_5M_2688x1944_12bit_linear30) {
        u32Fll = OS05A_VMAX_5M30_12bit_LINEAR;
        f32maxFps = 30; /* 30fps */
    } else if (pstSnsState->u8ImgMode == Os05a_5M_2688x1944_10bit_wdr30) {
        u32Fll = OS05A_VMAX_5M30_10bit_WDR;
        f32maxFps = 30; /* 30fps */
    }
    pstSnsState->u32FLStd = u32Fll;
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FlickerFreq = 0;
    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
    pstAeSnsDft->u32MaxIntTime = (pstSnsState->u32FLStd >= 12) ? (pstSnsState->u32FLStd - 12) : (0); /* sub 12 */
    pstAeSnsDft->stIntTimeAccu.f32Offset = 0;
    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 0.0078125; /* Accuracy 0.0078125 */
    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 0.0009765625; /* Accuracy 0.0009765625 */
    pstAeSnsDft->u32MaxAgain = 2047; /* max 2047 */
    pstAeSnsDft->u32MinAgain = 128; /* min 128 */
    pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
    pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;
    pstAeSnsDft->u32MaxDgain = 16383; /* max 16383 */
    pstAeSnsDft->u32MinDgain = 1024; /* min 1024 */
    pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
    pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;
    pstAeSnsDft->u32ISPDgainShift = 8; /* Accuracy 8 */
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 9 << pstAeSnsDft->u32ISPDgainShift; /* max 9 */
    pstAeSnsDft->u32InitExposure = g_au32InitExposure[vi_pipe] ? g_au32InitExposure[vi_pipe] : 130000; /* 130000 */
    pstAeSnsDft->u32LinesPer500ms = g_au32LinesPer500ms[vi_pipe] ? g_au32LinesPer500ms[vi_pipe] :
                                   ((HI_U64)(u32Fll * f32maxFps)) >> 1;
    pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_0;
    pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_32_0;
    pstAeSnsDft->bAERouteExValid = HI_FALSE;
    pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
    pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;
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
    pstAeSnsDft->u8AeCompensation = 0x38;
    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8; /* sub 8 */
    pstAeSnsDft->u32MinIntTime = 2; /* min 2 */
    pstAeSnsDft->u32MaxIntTimeTarget = 65535; /* max 65535 */
    pstAeSnsDft->u32MinIntTimeTarget = 2; /* min 2 */

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
    pstAeSnsDft->au8HistThresh[0] = 0xC;
    pstAeSnsDft->au8HistThresh[1] = 0x18;
    pstAeSnsDft->au8HistThresh[2] = 0x60; /* index 2 */
    pstAeSnsDft->au8HistThresh[3] = 0x80; /* index 3 */
    pstAeSnsDft->u8AeCompensation = 0x38;
    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8; /* sub 8 */
    pstAeSnsDft->u32MinIntTime = 2; /* min 2 */
    pstAeSnsDft->u32MaxIntTimeTarget = 65535; /* max 65535 */
    pstAeSnsDft->u32MinIntTimeTarget = 2; /* min 2 */
    pstAeSnsDft->u32LFMinExposure = 4500000; /* init 4500000 */
    pstAeSnsDft->bDiffGainSupport = HI_TRUE;

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
        pstAeSnsDft->au32Ratio[2] = 0x40; /* index 2 */
    }
    return;
}

static HI_S32 cmos_get_ae_default(VI_PIPE vi_pipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstAeSnsDft);
    OS05A_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset_s(&pstAeSnsDft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), 0, sizeof(ISP_AE_ROUTE_S));

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

static HI_VOID cmos_config_vmax(ISP_SNS_STATE_S *pstSnsState)
{
    if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
        pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = LOW_8BITS(pstSnsState->au32FL[0]);
        pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = HIGH_8BITS(pstSnsState->au32FL[0]);
    } else {
        pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = LOW_8BITS(pstSnsState->au32FL[0]);
        pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = HIGH_8BITS(pstSnsState->au32FL[0]);
    }

    return;
}

/* the function of sensor set fps */
static HI_VOID cmos_fps_set(VI_PIPE vi_pipe, HI_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    HI_FLOAT f32maxFps;
    HI_U32 u32Lines;
    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    OS05A_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);
    switch (pstSnsState->u8ImgMode) {
        case Os05a_5M_2688x1944_12bit_linear30:
            if ((f32Fps <= 30) && (f32Fps >= 1.87)) { /* fps:[1.87, 30] */
                f32maxFps = 30; /* 30fps */
                u32Lines = OS05A_VMAX_5M30_12bit_LINEAR * f32maxFps / f32Fps;
            } else {
                printf("Not support Fps: %f\n", f32Fps);
                return;
            }
            pstAeSnsDft->u32LinesPer500ms = OS05A_VMAX_5M30_12bit_LINEAR * 15; /* 15: 30fps / 2 */
            break;

        case Os05a_5M_2688x1944_10bit_wdr30:
            if ((f32Fps <= 30) && (f32Fps >= 2.0)) { /* fps:[2.0, 30] */
                f32maxFps = 30; /* 30fps */
                u32Lines = OS05A_VMAX_5M30_10bit_WDR * f32maxFps / f32Fps;
            } else {
                printf("Not support Fps: %f\n", f32Fps);
                return;
            }
            pstAeSnsDft->u32LinesPer500ms = OS05A_VMAX_5M30_10bit_WDR * 15; /* 15: 30fps / 2 */
            break;

        default:
            printf("Not support this Mode\n");
            return;
    }
    pstSnsState->u32FLStd = u32Lines;
    pstSnsState->u32FLStd = (pstSnsState->u32FLStd > OS05A_FULL_LINES_MAX) ?
        OS05A_FULL_LINES_MAX : pstSnsState->u32FLStd;
    pstAeSnsDft->f32Fps = f32Fps;
    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8; /* sub 8 */
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];

    cmos_config_vmax(pstSnsState);

    return;
}

static HI_VOID cmos_slow_framerate_set(VI_PIPE vi_pipe, HI_U32 u32FullLines, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    OS05A_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->au32FL[0] = (u32FullLines > OS05A_FULL_LINES_MAX) ? OS05A_FULL_LINES_MAX : u32FullLines;
    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
    pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 8; /* sub 8 */

    if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
        pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = LOW_8BITS(pstSnsState->au32FL[0]);
        pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = HIGH_8BITS(pstSnsState->au32FL[0]);
    } else {
        pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = LOW_8BITS(pstSnsState->au32FL[0]);
        pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = HIGH_8BITS(pstSnsState->au32FL[0]);
    }

    return;
}

static HI_VOID cmos_inttime_update_linear(VI_PIPE vi_pipe, HI_U32 u32IntTime)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    OS05A_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = LOW_8BITS(u32IntTime); /* index 2 */
    pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = HIGH_8BITS(u32IntTime); /* index 3 */

    return;
}
static HI_VOID cmos_inttime_update_2to1_line_long(VI_PIPE vi_pipe, HI_U32 u32IntTime)
{
    HI_FLOAT fCGain = 0;
    HI_U32 u32Again = 0;
    HI_U32 u32Dgain = 0;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    OS05A_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->au32WDRIntTime[1] = u32IntTime;
    if (g_u32STimeFps != 30) { /* 30fps */
        pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = LOW_8BITS(u32IntTime); /* index 2 */
        pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = HIGH_8BITS(u32IntTime); /* index 3 */
        pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = LOW_8BITS(pstSnsState->au32WDRIntTime[0]); /* index 8 */
        pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = HIGH_8BITS(pstSnsState->au32WDRIntTime[0]); /* index 9 */

        return;
    }
    if ((u32IntTime >= 750) && (pstSnsState->au32WDRIntTime[0] < 750) && /* max 750 */
        ((u32IntTime + 750) < 2450) && (g_f32AGain >= 11)) { /* add 750, max 2450, again max 11 */
        fCGain = g_f32AGain * g_f32DGain * pstSnsState->au32WDRIntTime[0] / 750; /* max 750 */
        if (fCGain < 16) { /* fcgain thresh 16 */
            u32Again = fCGain * 128; /* fCGain * 128 */
            u32Dgain = 1024; /* dgain 1024 */
        } else if (fCGain >= 16) { /* fcgain thresh 16 */
            u32Again = 2047; /* again 2047 */
            u32Dgain = 1024 * fCGain * 128  / 2047; /* dgain 1024 * 128, div 2047 */
        }
        pstSnsState->au32WDRIntTime[0] = 750; /* max 750 */
        pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = LOW_8BITS(u32Again); /* index 10 */
        pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = ((u32Again & 0x3F00) >> 8); /* index 11, shift 8 */
        pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = LOW_8BITS(u32Dgain); /* index 12 */
        pstSnsState->astRegsInfo[0].astI2cData[13].u32Data = ((u32Dgain & 0x3F00) >> 8); /* index 13, shift 8 */
    } else {
    }

    pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = LOW_8BITS(u32IntTime); /* index 2 */
    pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = HIGH_8BITS(u32IntTime); /* index 3 */
    pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = LOW_8BITS(pstSnsState->au32WDRIntTime[0]); /* index 8 */
    pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = HIGH_8BITS(pstSnsState->au32WDRIntTime[0]); /* index 9 */

    return;
}

static HI_VOID cmos_inttime_update_2to1_line(VI_PIPE vi_pipe, HI_U32 u32IntTime)
{
    static HI_BOOL bFirst[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = HI_TRUE};

    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    OS05A_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (bFirst[vi_pipe]) { /* short exposure */
        pstSnsState->au32WDRIntTime[0] = u32IntTime;

        bFirst[vi_pipe] = HI_FALSE;
    } else { /* long exposure */
        cmos_inttime_update_2to1_line_long(vi_pipe, u32IntTime);

        bFirst[vi_pipe] = HI_TRUE;
    }

    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static HI_VOID cmos_inttime_update(VI_PIPE vi_pipe, HI_U32 u32IntTime)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    OS05A_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        cmos_inttime_update_2to1_line(vi_pipe, u32IntTime);
    } else {
        cmos_inttime_update_linear(vi_pipe, u32IntTime);
    }

    return;
}
static HI_VOID cmos_gains_update(VI_PIPE vi_pipe, HI_U32 u32Again, HI_U32 u32Dgain)
{
    static HI_U8 u8ExpFrame[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = OS05A_FRAME_TYPE_LONG};
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    OS05A_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        if (u8ExpFrame[vi_pipe] == OS05A_FRAME_TYPE_LONG) {
            pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = LOW_8BITS(u32Again); /* index 4 */
            pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = ((u32Again & 0x3F00) >> 8); /* index 5, shift 8 */
            pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = LOW_8BITS(u32Dgain); /* index 6 */
            pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = ((u32Dgain & 0x3F00) >> 8); /* index 7, shift 8 */
            u8ExpFrame[vi_pipe] = OS05A_FRAME_TYPE_SHORT;
        } else if (u8ExpFrame[vi_pipe] == OS05A_FRAME_TYPE_SHORT) {
            pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = LOW_8BITS(u32Again); /* index 10 */
            pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = ((u32Again & 0x3F00) >> 8); /* index 11, shift 8 */
            pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = LOW_8BITS(u32Dgain); /* index 12 */
            pstSnsState->astRegsInfo[0].astI2cData[13].u32Data = ((u32Dgain & 0x3F00) >> 8); /* index 13, shift 8 */
            u8ExpFrame[vi_pipe] = OS05A_FRAME_TYPE_LONG;
        } else {
        }
    } else {
        pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = LOW_8BITS(u32Again); /* index 4 */
        pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = ((u32Again & 0x3F00) >> 8); /* index 5, shift 8 */
        pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = LOW_8BITS(u32Dgain); /* index 6 */
        pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = ((u32Dgain & 0x3F00) >> 8); /* index 7, shift 8 */
    }

    return;
}
static HI_VOID cmos_get_inttime_max_2to1_line_long_frame(VI_PIPE vi_pipe,
    HI_U32 *au32Ratio, HI_U32 *au32IntTimeMax,
    HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    HI_U32 i, u32ShortMax, u32ShortMax1, u32ShortTimeMinLimit;
    HI_U32 u32RatioTmp = 0x40;

    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    OS05A_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32ShortTimeMinLimit = 7; /* u32ShortTimeMinLimit 7 */

    if (g_au32MaxTimeGetCnt[vi_pipe] > 15) { /* cnt 15fps */
        u32ShortMax = pstSnsState->au32FL[0] - 50 - pstSnsState->au32WDRIntTime[0]; /* sub 50 */
        u32ShortMax1 = pstSnsState->au32FL[0] - 50; /* sub 50 */
    } else {
        u32ShortMax = ((pstSnsState->au32FL[1] - 50 - pstSnsState->au32WDRIntTime[0]) << 6) / /* sub 50, shift 6 */
                           SNS_DIV_0_TO_1(au32Ratio[0]);
        u32ShortMax1 = (pstSnsState->au32FL[0] - 50) * 0x40 / (au32Ratio[0] + 0x40); /* sub 50 */
    }

    u32ShortMax = (u32ShortMax < u32ShortMax1) ? u32ShortMax : u32ShortMax1;
    g_au32MaxTimeGetCnt[vi_pipe]++;

    if (u32ShortMax >= u32ShortTimeMinLimit) {
        au32IntTimeMax[0] = u32ShortMax;
        au32IntTimeMax[1] = (au32IntTimeMax[0] * au32Ratio[0]) >> 6; /* shift 6 */
        au32IntTimeMin[0] = u32ShortTimeMinLimit;
        au32IntTimeMin[1] = (au32IntTimeMin[0] * au32Ratio[0]) >> 6; /* shift 6 */
    } else {
        u32ShortMax = u32ShortTimeMinLimit;

        for (i = 0x40; i <= 0xFFF; i++) {
            if ((u32ShortMax + ((u32ShortMax * i) >> 6)) > (pstSnsState->au32FL[0] - 50)) { /* sub 50, shift 6 */
                u32RatioTmp = i - 1;
                break;
            }
        }
        au32IntTimeMax[0] = u32ShortMax;
        au32IntTimeMax[1] = (au32IntTimeMax[0] * u32RatioTmp) >> 6; /* shift 6 */
        au32IntTimeMin[0] = au32IntTimeMax[0];
        au32IntTimeMin[1] = au32IntTimeMax[1];
    }

    return;
}
static HI_VOID cmos_get_inttime_max_2to1_line_auto_long_frame(VI_PIPE vi_pipe, HI_U32 *au32Ratio,
    HI_U32 *au32IntTimeMax, HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    HI_U32 i, u32ShortMax, u32ShortMax1, u32ShortTimeMinLimit;
    HI_U32 u32RatioTmp = 0x40;

    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    OS05A_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32ShortTimeMinLimit =  7; /* u32ShortTimeMinLimit 7 */

    if (pstSnsState->au32WDRIntTime[0] == u32ShortTimeMinLimit && au32Ratio[0] == 0x40) {
        if (g_au32MaxTimeGetCnt[vi_pipe] > 0) {
            u32ShortMax = pstSnsState->au32FL[0] - 50 - pstSnsState->au32WDRIntTime[0]; /* sub 50 */
            u32ShortMax1 = pstSnsState->au32FL[0] - 50; /* sub 50 */
        } else {
            u32ShortMax = ((pstSnsState->au32FL[1] - 50 - pstSnsState->au32WDRIntTime[0]) << 6) / /* sub 50, shift 6 */
                               SNS_DIV_0_TO_1(au32Ratio[0]);
            u32ShortMax1 = (pstSnsState->au32FL[0] - 50) * 0x40 / (au32Ratio[0] + 0x40); /* sub 50 */
        }

        u32ShortMax = (u32ShortMax < u32ShortMax1) ? u32ShortMax : u32ShortMax1;
        *pu32LFMaxIntTime = u32ShortMax;
        g_au32MaxTimeGetCnt[vi_pipe]++;
    } else {
        g_au32MaxTimeGetCnt[vi_pipe] = 0;
        u32ShortMax = ((pstSnsState->au32FL[1] - 50 - pstSnsState->au32WDRIntTime[0]) << 6) / /* sub 50, shift 6 */
                           SNS_DIV_0_TO_1(au32Ratio[0]);
        u32ShortMax1 = (pstSnsState->au32FL[0] - 50) * 0x40 / (au32Ratio[0] + 0x40); /* sub 50 */
        u32ShortMax = (u32ShortMax <  u32ShortMax1) ? u32ShortMax :  u32ShortMax1;
        *pu32LFMaxIntTime = u32ShortMax1;
    }

    if (u32ShortMax >= u32ShortTimeMinLimit) {
        au32IntTimeMax[0] = u32ShortMax;
        au32IntTimeMax[1] = (au32IntTimeMax[0] * au32Ratio[0]) >> 6; /* shift 6 */
        au32IntTimeMin[0] = u32ShortTimeMinLimit;
        au32IntTimeMin[1] = (au32IntTimeMin[0] * au32Ratio[0]) >> 6; /* shift 6 */
    } else {
        u32ShortMax = u32ShortTimeMinLimit;

        for (i = 0x40; i <= 0xFFF; i++) {
            if ((u32ShortMax + ((u32ShortMax * i) >> 6)) > (pstSnsState->au32FL[0] - 50)) { /* sub 50, shift 6 */
                u32RatioTmp = i - 1;
                break;
            }
        }
        au32IntTimeMax[0] = u32ShortMax;
        au32IntTimeMax[1] = (au32IntTimeMax[0] * u32RatioTmp) >> 6; /* shift 6 */
        au32IntTimeMin[0] = au32IntTimeMax[0];
        au32IntTimeMin[1] = au32IntTimeMax[1];
    }
    return;
}
static HI_VOID cmos_get_inttime_max_2to1_line_normal(VI_PIPE vi_pipe,
    HI_U32 *au32Ratio, HI_U32 *au32IntTimeMax,
    HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    HI_U32 i, u32ShortMax, u32ShortMax1, u32ShortTimeMinLimit;
    HI_U32 u32RatioTmp = 0x40;

    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    OS05A_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32ShortTimeMinLimit =  7; /* u32ShortTimeMinLimit 7 */

    u32ShortMax = ((pstSnsState->au32FL[1] - 50 - pstSnsState->au32WDRIntTime[0]) << 6) / /* sub 50, shift 6 */
        SNS_DIV_0_TO_1(au32Ratio[0]);
    u32ShortMax1 = (pstSnsState->au32FL[0] - 50) * 0x40 / (au32Ratio[0] + 0x40); /* sub 50 */
    u32ShortMax = (u32ShortMax <  u32ShortMax1) ? u32ShortMax :  u32ShortMax1;

    if (u32ShortMax >= u32ShortTimeMinLimit) {
        au32IntTimeMax[0] = u32ShortMax;
        au32IntTimeMax[1] = (au32IntTimeMax[0] * au32Ratio[0]) >> 6; /* shift 6 */
        au32IntTimeMin[0] = u32ShortTimeMinLimit;
        au32IntTimeMin[1] = (au32IntTimeMin[0] * au32Ratio[0]) >> 6; /* shift 6 */
    } else {
        u32ShortMax = u32ShortTimeMinLimit;

        for (i = 0x40; i <= 0xFFF; i++) {
            if ((u32ShortMax + ((u32ShortMax * i) >> 6)) > (pstSnsState->au32FL[0] - 50)) { /* sub 50, shift 6 */
                u32RatioTmp = i - 1;
                break;
            }
        }
        au32IntTimeMax[0] = u32ShortMax;
        au32IntTimeMax[1] = (au32IntTimeMax[0] * u32RatioTmp) >> 6; /* shift 6 */
        au32IntTimeMin[0] = au32IntTimeMax[0];
        au32IntTimeMin[1] = au32IntTimeMax[1];
    }

    return;
}

static HI_VOID cmos_get_inttime_max_2to1_line(VI_PIPE vi_pipe,
    HI_U32 *au32Ratio, HI_U32 *au32IntTimeMax,
    HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    switch (g_genFSWDRMode[vi_pipe]) {
        case ISP_FSWDR_LONG_FRAME_MODE:
            cmos_get_inttime_max_2to1_line_long_frame(vi_pipe, au32Ratio, au32IntTimeMax, au32IntTimeMin,
                pu32LFMaxIntTime);
            break;
        case ISP_FSWDR_AUTO_LONG_FRAME_MODE:
            cmos_get_inttime_max_2to1_line_auto_long_frame(vi_pipe, au32Ratio, au32IntTimeMax, au32IntTimeMin,
                pu32LFMaxIntTime);
            break;
        case ISP_FSWDR_NORMAL_MODE:
            cmos_get_inttime_max_2to1_line_normal(vi_pipe, au32Ratio, au32IntTimeMax, au32IntTimeMin,
                pu32LFMaxIntTime);
            break;
        default:
            break;
    }

    return;
}

/* Only used in WDR_MODE_2To1_LINE and WDR_MODE_2To1_FRAME mode */
static HI_VOID cmos_get_inttime_max(VI_PIPE vi_pipe, HI_U16 u16ManRatioEnable, HI_U32 *au32Ratio,
    HI_U32 *au32IntTimeMax, HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER_VOID(au32Ratio);
    CMOS_CHECK_POINTER_VOID(au32IntTimeMax);
    CMOS_CHECK_POINTER_VOID(au32IntTimeMin);
    CMOS_CHECK_POINTER_VOID(pu32LFMaxIntTime);
    OS05A_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    switch (pstSnsState->enWDRMode) {
        case WDR_MODE_2To1_LINE:
            cmos_get_inttime_max_2to1_line(vi_pipe, au32Ratio, au32IntTimeMax, au32IntTimeMin, pu32LFMaxIntTime);
            break;
        default:
            break;
    }

    return;
}

/* Only used in FSWDR mode */
static HI_VOID cmos_ae_fswdr_attr_set(VI_PIPE vi_pipe, AE_FSWDR_ATTR_S *pstAeFSWDRAttr)
{
    g_genFSWDRMode[vi_pipe] = pstAeFSWDRAttr->enFSWDRMode;
    g_au32MaxTimeGetCnt[vi_pipe] = 0;
    return;
}

static HI_S32 cmos_init_ae_exp_function(AE_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    CMOS_CHECK_POINTER(pstExpFuncs);

    memset_s(pstExpFuncs, sizeof(AE_SENSOR_EXP_FUNC_S), 0, sizeof(AE_SENSOR_EXP_FUNC_S));

    pstExpFuncs->pfn_cmos_get_ae_default    = cmos_get_ae_default;
    pstExpFuncs->pfn_cmos_fps_set           = cmos_fps_set;
    pstExpFuncs->pfn_cmos_slow_framerate_set = cmos_slow_framerate_set;
    pstExpFuncs->pfn_cmos_inttime_update    = cmos_inttime_update;
    pstExpFuncs->pfn_cmos_gains_update      = cmos_gains_update;
    pstExpFuncs->pfn_cmos_again_calc_table  = HI_NULL;
    pstExpFuncs->pfn_cmos_dgain_calc_table  = HI_NULL;
    pstExpFuncs->pfn_cmos_get_inttime_max   = cmos_get_inttime_max;
    pstExpFuncs->pfn_cmos_ae_fswdr_attr_set = cmos_ae_fswdr_attr_set;

    return HI_SUCCESS;
}

/* AWB default parameter and function */
#define CALIBRATE_STATIC_TEMP             4850
#define CALIBRATE_STATIC_WB_R_GAIN        0x21E
#define CALIBRATE_STATIC_WB_GR_GAIN       0x100
#define CALIBRATE_STATIC_WB_GB_GAIN       0x100
#define CALIBRATE_STATIC_WB_B_GAIN        0x1D8

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1                  (-13)
#define CALIBRATE_AWB_P2                  269
#define CALIBRATE_AWB_Q1                  0
#define CALIBRATE_AWB_A1                  154243
#define CALIBRATE_AWB_B1                  128
#define CALIBRATE_AWB_C1                  (-105138)

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN                      0
#define GOLDEN_BGAIN                      0
static HI_S32 cmos_get_awb_default(VI_PIPE vi_pipe, AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    HI_S32 i;

    CMOS_CHECK_POINTER(pstAwbSnsDft);
    OS05A_SENSOR_GET_CTX(vi_pipe, pstSnsState);
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
    memcpy_s(&pstDef->stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S), &g_stDngColorParam,
             sizeof(ISP_CMOS_DNG_COLORPARAM_S));
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
    pstDef->unKey.bit1BayerNr        = 1;
    pstDef->pstBayerNr               = &g_stIspBayerNr;
    pstDef->unKey.bit1Gamma          = 1;
    pstDef->pstGamma                 = &g_stIspGamma;
    pstDef->unKey.bit1Detail         = 1;
    pstDef->pstDetail                = &g_stIspDetail;
#ifdef CONFIG_HI_ISP_CR_SUPPORT
    pstDef->unKey.bit1Ge             = 1;
    pstDef->pstGe                    = &g_stIspGe;
#endif
    pstDef->unKey.bit1AntiFalseColor = 1;
    pstDef->pstAntiFalseColor = &g_stIspAntiFalseColor;
    pstDef->unKey.bit1Ldci = 1;
    pstDef->pstLdci = &g_stIspLdci;
    pstDef->unKey.bit1Dehaze = 1;
    pstDef->pstDehaze = &g_stIspDehaze;
    pstDef->unKey.bit1Lcac = 1;
    pstDef->pstLcac = &g_stIspLCac;
    memcpy_s(&pstDef->stNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S), &g_stIspNoiseCalibration,
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
    pstDef->stWdrSwitchAttr.au32ExpRatio[0] = 0x100;
    memcpy_s(&pstDef->stNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S), &g_stIspNoiseCalibration,
             sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
    return;
}

static HI_S32 cmos_get_isp_default(VI_PIPE vi_pipe, ISP_CMOS_DEFAULT_S *pstDef)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstDef);
    OS05A_SENSOR_GET_CTX(vi_pipe, pstSnsState);
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
    pstDef->unKey.bit1Lsc      = 0;
    pstDef->pstLsc             = &g_stCmosLsc;
#ifdef CONFIG_HI_ISP_EDGEMARK_SUPPORT
    pstDef->unKey.bit1EdgeMark = 0;
    pstDef->pstEdgeMark        = &g_stIspEdgeMark;
#endif
    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:

            cmos_get_isp_linear_default(pstDef);

            break;
        case WDR_MODE_2To1_LINE:

            cmos_get_isp_2to1_line_wdr_default(pstDef);

            break;
    }
    pstDef->stSensorMode.u32SensorID = OS05A_ID;
    pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

    cmos_get_isp_dng_default(pstSnsState, pstDef);
    return HI_SUCCESS;
}

static HI_S32 cmos_get_isp_black_level(VI_PIPE vi_pipe, ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
    HI_S32  i;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstBlackLevel);
    OS05A_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    /* Don't need to update black level when iso change */
    pstBlackLevel->bUpdate = HI_FALSE;
    if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            pstBlackLevel->au16BlackLevel[i] = 64; /* 12bit, 64 */
        }
    } else if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            pstBlackLevel->au16BlackLevel[i] = 256; /* 12bit, 256 */
        }
    }

    return HI_SUCCESS;
}

static HI_VOID cmos_set_pixel_detect(VI_PIPE vi_pipe, HI_BOOL bEnable)
{
    HI_U32 u32FullLines_5Fps, u32MaxIntTime_5Fps;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    OS05A_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        return;
    } else {
        if (pstSnsState->u8ImgMode == Os05a_5M_2688x1944_12bit_linear30) {
            u32FullLines_5Fps = (OS05A_VMAX_5M30_12bit_LINEAR * 30) / 5; /* 30fps, 5fps */
        } else {
            return;
        }
    }

    u32MaxIntTime_5Fps = u32FullLines_5Fps - 4; /* sub 4 */

    if (bEnable) { /* setup for ISP pixel calibration mode */
        os05a_write_register(vi_pipe, VMAX_ADDR_L, LOW_8BITS(u32FullLines_5Fps));
        os05a_write_register(vi_pipe, VMAX_ADDR_H, HIGH_8BITS(u32FullLines_5Fps));

        os05a_write_register(vi_pipe, EXPOSURE_TIME_LONG_H, HIGH_8BITS(u32MaxIntTime_5Fps)); /* shutter */
        os05a_write_register(vi_pipe, EXPOSURE_TIME_LONG_L, LOW_8BITS(u32MaxIntTime_5Fps));

        os05a_write_register(vi_pipe, AGAIN_H, 0x00);  /* gain */
        os05a_write_register(vi_pipe, AGAIN_L, 0x90);
    } else { /* setup for ISP 'normal mode' */
        os05a_write_register(vi_pipe, VMAX_ADDR_L, LOW_8BITS(pstSnsState->u32FLStd));
        os05a_write_register(vi_pipe, VMAX_ADDR_H, HIGH_8BITS(pstSnsState->u32FLStd));
        pstSnsState->bSyncInit = HI_FALSE;
    }
    return;
}

static HI_S32 cmos_set_wdr_mode(VI_PIPE vi_pipe, HI_U8 u8Mode)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    OS05A_SENSOR_GET_CTX(vi_pipe, pstSnsState);
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
    hi_s32 i;

    pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
    pstSnsState->astRegsInfo[0].unComBus.s8I2cDev = g_aunOs05aBusInfo[vi_pipe].s8I2cDev;
    pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2; /* DelayMax 2 */
    pstSnsState->astRegsInfo[0].u32RegNum = 8; /* RegNum 8 */

    if ((pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) || (pstSnsState->enWDRMode == WDR_MODE_2To1_FRAME) ||
        (pstSnsState->enWDRMode == WDR_MODE_2To1_FRAME_FULL_RATE)) {
        pstSnsState->astRegsInfo[0].u32RegNum += 6; /* RegNum add 6 */
    }

    for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
        pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
        pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr = OS05A_I2C_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[i].u32AddrByteNum = OS05A_ADDR_BYTE;
        pstSnsState->astRegsInfo[0].astI2cData[i].u32DataByteNum = OS05A_DATA_BYTE;
    }
    pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
    pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr = VMAX_ADDR_L;        /* Vmax */
    pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
    pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr = VMAX_ADDR_H;        /* Vmax */
    pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0; /* index 2 */
    pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr = EXPOSURE_TIME_LONG_L; /* index 2 */
    pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 0; /* index 3 */
    pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr = EXPOSURE_TIME_LONG_H; /* index 3 */
    pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 0; /* index 4 */
    pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr = AGAIN_L; /* index 4 */
    pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0; /* index 5 */
    pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr = AGAIN_H; /* index 5 */
    pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0; /* index 6 */
    pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr = DGAIN_L; /* index 6 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 0; /* index 7 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr = DGAIN_H; /* index 7 */

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 0; /* index 8 */
        pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr = EXPOSURE_TIME_SHORT_L; /* index 8 */
        pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum = 0; /* index 9 */
        pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr = EXPOSURE_TIME_SHORT_H; /* index 9 */
        pstSnsState->astRegsInfo[0].astI2cData[10].u8DelayFrmNum = 0; /* index 10 */
        pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr = SHORT_AGAIN_L; /* index 10 */
        pstSnsState->astRegsInfo[0].astI2cData[11].u8DelayFrmNum = 0; /* index 11 */
        pstSnsState->astRegsInfo[0].astI2cData[11].u32RegAddr = SHORT_AGAIN_H; /* index 11 */
        pstSnsState->astRegsInfo[0].astI2cData[12].u8DelayFrmNum = 0; /* index 12 */
        pstSnsState->astRegsInfo[0].astI2cData[12].u32RegAddr = SHORT_DGAIN_L; /* index 12 */
        pstSnsState->astRegsInfo[0].astI2cData[13].u8DelayFrmNum = 0; /* index 13 */
        pstSnsState->astRegsInfo[0].astI2cData[13].u32RegAddr = SHORT_DGAIN_H; /* index 13 */
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
    OS05A_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    if ((pstSnsState->bSyncInit == HI_FALSE) || (pstSnsRegsInfo->bConfig == HI_FALSE)) {
        cmos_comm_sns_reg_info_init(vi_pipe, pstSnsState);
    } else {
        cmos_sns_reg_info_update(vi_pipe, pstSnsState);
    }

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        if (pstSnsState->au32FL[1] > pstSnsState->au32FL[0]) {
            pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 2; /* delay 2 */
            pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 2; /* delay 2 */
        } else if (pstSnsState->au32FL[1] < pstSnsState->au32FL[0]) {
            pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
            pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
        } else {
            /* do nothing */
        }
    }

    pstSnsRegsInfo->bConfig = HI_FALSE;
    memcpy_s(pstSnsRegsInfo, sizeof(ISP_SNS_REGS_INFO_S), &pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S));
    memcpy_s(&pstSnsState->astRegsInfo[1], sizeof(ISP_SNS_REGS_INFO_S), &pstSnsState->astRegsInfo[0],
             sizeof(ISP_SNS_REGS_INFO_S));
    pstSnsState->au32FL[1] = pstSnsState->au32FL[0];

    return HI_SUCCESS;
}
static HI_S32 cmos_set_image_mode(VI_PIPE vi_pipe, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode)
{
    HI_U8 u8SensorImageMode;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    CMOS_CHECK_POINTER(pstSensorImageMode);
    OS05A_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);
    u8SensorImageMode = pstSnsState->u8ImgMode;
    pstSnsState->bSyncInit = HI_FALSE;
    if ((pstSensorImageMode->u16Width > 2688) || (pstSensorImageMode->u16Height > 1944)) { /* w2688, h1944 */
        OS05A_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState);
        return HI_FAILURE;
    }
    switch (pstSnsState->enWDRMode) {
        case WDR_MODE_2To1_LINE:
            if (pstSensorImageMode->f32Fps > 30) { /* 30fps */
                OS05A_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState);
                return HI_FAILURE;
            }
            u8SensorImageMode = Os05a_5M_2688x1944_10bit_wdr30;
            pstSnsState->u32FLStd = OS05A_VMAX_5M30_10bit_WDR;
            break;
        case WDR_MODE_NONE:
            if (pstSensorImageMode->f32Fps > 30) { /* 30fps */
                OS05A_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState);
                return HI_FAILURE;
            }
            u8SensorImageMode = Os05a_5M_2688x1944_12bit_linear30;
            pstSnsState->u32FLStd = OS05A_VMAX_5M30_12bit_LINEAR;
            break;
        default:
            break;
    }

    /* Switch SensorImageMode */
    if ((pstSnsState->bInit == HI_TRUE) && (u8SensorImageMode == pstSnsState->u8ImgMode)) {
        /* Don't need to switch SensorImageMode */
        return ISP_DO_NOT_NEED_SWITCH_IMAGEMODE;
    }
    pstSnsState->u8ImgMode = u8SensorImageMode;
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstSnsState->au32FL[1] = pstSnsState->au32FL[0];
    memset_s(pstSnsState->au32WDRIntTime, sizeof(pstSnsState->au32WDRIntTime), 0, sizeof(pstSnsState->au32WDRIntTime));
    return HI_SUCCESS;
}

static HI_VOID sensor_global_init(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    OS05A_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->bInit = HI_FALSE;
    pstSnsState->bSyncInit = HI_FALSE;
    pstSnsState->u8ImgMode = Os05a_5M_2688x1944_12bit_linear30;
    pstSnsState->enWDRMode = WDR_MODE_NONE;
    pstSnsState->u32FLStd = OS05A_VMAX_5M30_12bit_LINEAR;
    pstSnsState->au32FL[0] = OS05A_VMAX_5M30_12bit_LINEAR;
    pstSnsState->au32FL[1] = OS05A_VMAX_5M30_12bit_LINEAR;

    memset_s(&pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset_s(&pstSnsState->astRegsInfo[1], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
    return;
}

static HI_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    CMOS_CHECK_POINTER(pstSensorExpFunc);

    memset_s(pstSensorExpFunc, sizeof(ISP_SENSOR_EXP_FUNC_S), 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init = os05a_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit = os05a_exit;
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
static HI_S32 os05a_set_bus_info(VI_PIPE vi_pipe, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunOs05aBusInfo[vi_pipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

    return HI_SUCCESS;
}

static HI_S32 sensor_ctx_init(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    OS05A_SENSOR_GET_CTX(vi_pipe, pastSnsStateCtx);

    if (pastSnsStateCtx == HI_NULL) {
        pastSnsStateCtx = (ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
        if (pastSnsStateCtx == HI_NULL) {
            SNS_ERR_TRACE("Isp[%d] SnsCtx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset_s(pastSnsStateCtx, sizeof(ISP_SNS_STATE_S), 0, sizeof(ISP_SNS_STATE_S));

    OS05A_SENSOR_SET_CTX(vi_pipe, pastSnsStateCtx);

    return HI_SUCCESS;
}

static HI_VOID sensor_ctx_exit(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    OS05A_SENSOR_GET_CTX(vi_pipe, pastSnsStateCtx);
    SENSOR_FREE(pastSnsStateCtx);
    OS05A_SENSOR_RESET_CTX(vi_pipe);
}

static HI_S32 sensor_register_callback(VI_PIPE vi_pipe, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib)
{
    HI_S32 s32Ret;
    ISP_SENSOR_REGISTER_S stIspRegister;
    AE_SENSOR_REGISTER_S  stAeRegister;
    AWB_SENSOR_REGISTER_S stAwbRegister;
    ISP_SNS_ATTR_INFO_S   stSnsAttrInfo;

    CMOS_CHECK_POINTER(pstAeLib);
    CMOS_CHECK_POINTER(pstAwbLib);

    s32Ret = sensor_ctx_init(vi_pipe);
    if (s32Ret != HI_SUCCESS) {
        return HI_FAILURE;
    }

    stSnsAttrInfo.eSensorId = OS05A_ID;
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

    s32Ret = HI_MPI_ISP_SensorUnRegCallBack(vi_pipe, OS05A_ID);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AE_SensorUnRegCallBack(vi_pipe, pstAeLib, OS05A_ID);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AWB_SensorUnRegCallBack(vi_pipe, pstAwbLib, OS05A_ID);
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

    g_au32InitExposure[vi_pipe] = pstInitAttr->u32Exposure;
    g_au32LinesPer500ms[vi_pipe] = pstInitAttr->u32LinesPer500ms;
    g_au16InitWBGain[vi_pipe][0] = pstInitAttr->u16WBRgain; /* 0: Rgain */
    g_au16InitWBGain[vi_pipe][1] = pstInitAttr->u16WBGgain; /* 1: Ggain */
    g_au16InitWBGain[vi_pipe][2] = pstInitAttr->u16WBBgain; /* 2: Bgain */
    g_au16SampleRgain[vi_pipe] = pstInitAttr->u16SampleRgain;
    g_au16SampleBgain[vi_pipe] = pstInitAttr->u16SampleBgain;
    g_quick_start_en[vi_pipe] = pstInitAttr->stQuickStart.bQuickStartEn;

    g_abAERouteExValid[vi_pipe] = pstInitAttr->bAERouteExValid;
    memcpy_s(&g_astInitAERoute[vi_pipe], sizeof(ISP_AE_ROUTE_S), &pstInitAttr->stAERoute,
             sizeof(ISP_AE_ROUTE_S));
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

ISP_SNS_OBJ_S stSnsOs05aObj = {
    .pfnRegisterCallback = sensor_register_callback,
    .pfnUnRegisterCallback = sensor_unregister_callback,
    .pfnStandby = os05a_standby,
    .pfnRestart = os05a_restart,
    .pfnMirrorFlip = HI_NULL,
    .pfnWriteReg = os05a_write_register,
    .pfnReadReg = os05a_read_register,
    .pfnSetBusInfo = os05a_set_bus_info,
    .pfnSetInit = sensor_set_init
};

