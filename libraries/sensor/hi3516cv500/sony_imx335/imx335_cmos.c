/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of imx335_cmos.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "imx335_cmos.h"
#include "imx335_cmos_ex.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "mpi_isp.h"
#include "mpi_ae.h"
#include "mpi_awb.h"

#define IMX335_ID    335

#define IMX335_HIG_4BITS(x) (((x) & 0xF0000) >> 16)
#define IMX335_MID_8BITS(x) (((x) & 0x0FF00) >> 8)
#define IMX335_LOW_8BITS(x)  ((x) & 0x000ff)

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ?  (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ?  (b) : (a))
#endif

ISP_SNS_STATE_S *g_pastImx335[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define IMX335_SENSOR_SET_CTX(dev, pstCtx)   ((g_pastImx335[dev]) = (pstCtx))
#define IMX335_SENSOR_RESET_CTX(dev)         (g_pastImx335[dev] = HI_NULL)
static HI_U32 g_au32InitExposure[ISP_MAX_PIPE_NUM]  = {0};
static HI_U32 g_au32LinesPer500ms[ISP_MAX_PIPE_NUM] = {0};
static HI_U16 g_au16InitWBGain[ISP_MAX_PIPE_NUM][ISP_RGB_CHN_NUM] = {{0}};
static HI_U16 g_au16SampleRgain[ISP_MAX_PIPE_NUM] = {0};
static HI_U16 g_au16SampleBgain[ISP_MAX_PIPE_NUM] = {0};
static HI_BOOL g_quick_start_en[ISP_MAX_PIPE_NUM] = {HI_FALSE};

static HI_U32   g_u32Imx335AGain[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = 1024}; /* again 1024 */
static HI_U32   g_u32Imx335DGain[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = 1024}; /* dgain 1024 */

static HI_BOOL g_abAERouteExValid[ISP_MAX_PIPE_NUM] = {0};
static ISP_AE_ROUTE_S g_astInitAERoute[ISP_MAX_PIPE_NUM] = {{0}};
static ISP_AE_ROUTE_EX_S g_astInitAERouteEx[ISP_MAX_PIPE_NUM] = {{0}};
static ISP_AE_ROUTE_S g_astInitAERouteSF[ISP_MAX_PIPE_NUM] = {{0}};
static ISP_AE_ROUTE_EX_S g_astInitAERouteSFEx[ISP_MAX_PIPE_NUM] = {{0}};

ISP_SNS_COMMBUS_U g_aunImx335BusInfo[ISP_MAX_PIPE_NUM] = {
    [0] = { .s8I2cDev = 0},
    [1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1}
};

static ISP_FSWDR_MODE_E g_genFSWDRMode[ISP_MAX_PIPE_NUM] = {
    [0 ... ISP_MAX_PIPE_NUM - 1] = ISP_FSWDR_NORMAL_MODE
};

typedef struct hiIMX335_STATE_S {
    HI_U32      u32BRL;
    HI_U32      u32RHS1_MAX;
    HI_U32      u32deltaRHS1;
} IMX335_STATE_S;

IMX335_STATE_S g_astimx335State[ISP_MAX_PIPE_NUM] = {{0}};

static HI_U32  g_au32MaxTimeGetCnt[ISP_MAX_PIPE_NUM] = {0};

static HI_U32 g_au32STimeFps[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = 30}; /* time fps 30 */
static HI_U32 g_au32LGain[ISP_MAX_PIPE_NUM] = {0};

ISP_SNS_COMMBUS_U *imx335_get_bus_Info(VI_PIPE vi_pipe)
{
    return &g_aunImx335BusInfo[vi_pipe];
}

ISP_SNS_STATE_S *imx335_get_ctx(VI_PIPE vi_pipe)
{
    return g_pastImx335[vi_pipe];
}

const IMX335_VIDEO_MODE_TBL_S g_astImx335ModeTbl[IMX335_MODE_BUTT] = {
    {IMX335_VMAX_5M_30FPS_12BIT_LINEAR,   IMX335_FULL_LINES_MAX,          30, 2.0,
     2592, 1944, 0, WDR_MODE_NONE,       "IMX335_5M_30FPS_12BIT_LINEAR_MODE"},
    {IMX335_VMAX_5M_30FPS_10BIT_WDR,      IMX335_FULL_LINES_MAX,          30, 15.0,
     2592, 1944, 0, WDR_MODE_2To1_LINE,  "IMX335_5M_30FPS_10BIT_WDR_MODE"},
};

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
#define  IMX335_VMAX_ADDR_L             0x3030
#define  IMX335_VMAX_ADDR_M             0x3031
#define  IMX335_VMAX_ADDR_H             0x3032
#define  IMX335_Y_OUT_SIZE_LOW          0x3056
#define  IMX335_Y_OUT_SIZE_HIGH         0x3057
#define  IMX335_SHR0_LOW                 0x3058
#define  IMX335_SHR0_MIDDLE              0x3059
#define  IMX335_SHR0_HIGH               0x305A
#define  IMX335_SHR1_LOW                0x305C
#define  IMX335_SHR1_MIDDLE              0x305D
#define  IMX335_SHR1_HIGH                0x305E
#define  IMX335_RHS1_LOW                 0x3068
#define  IMX335_RHS1_MIDDLE              0x3069
#define  IMX335_RHS1_HIGH                0x306A
#define  IMX335_GAIN_LONG_LOW           0x30E8
#define  IMX335_GAIN_LONG_HIGH          0x30E9
#define  IMX335_GAIN_SHORT_LOW          0x30EA
#define  IMX335_GAIN_SHORT_HIGH         0x30EB

#define IMX335_RES_IS_5M_12BIT_LINEAR(w, h)    (((w) == 2592) && ((h) == 1944))
#define IMX335_RES_IS_5M_10BIT_WDR(w, h)       (((w) == 2592) && ((h) == 1944))
#define IMX335_RES_IS_4M_12BIT_LINEAR(w, h)    (((w) == 2592) && ((h) == 1520))
#define IMX335_RES_IS_4M_10BIT_WDR(w, h)       (((w)== 2592) && ((h)== 1520))
#define IMX335_RES_IS_4M_10BIT_WDR_EX(w, h)    (((w) == 2560) && ((h) == 1440))

/* sensor gain */
#define IMX335_AGAIN_MIN                  1024
#define IMX335_AGAIN_MAX                  32381    /* the max again is 32381 */

#define IMX335_DGAIN_MIN                  1024
#define IMX335_DGAIN_MAX                  128914

#define IMX335_AD_GAIN_TBL_RANGE          241
#define IMX335_AGAIN_TBL_RANGE            100
#define IMX335_DGAIN_TBL_RANGE            140

#define IMX335_WDR_BRL                          3968 /* 1984 x2 */

#define IMX335_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState)\
    do{\
        SNS_ERR_TRACE("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",\
                      (pstSensorImageMode)->u16Width,  \
                      (pstSensorImageMode)->u16Height, \
                      (pstSensorImageMode)->f32Fps,    \
                      (pstSnsState)->enWDRMode);\
    }while (0)

static HI_VOID cmos_get_ae_comm_default(VI_PIPE vi_pipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft,
    ISP_SNS_STATE_S *pstSnsState)
{
    HI_U32 u32Lines;
    HI_FLOAT f32MaxFps;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    IMX335_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32Lines = g_astImx335ModeTbl[pstSnsState->u8ImgMode].u32VertiLines;
    f32MaxFps = g_astImx335ModeTbl[pstSnsState->u8ImgMode].f32MaxFps;

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        pstSnsState->u32FLStd = u32Lines * 2; /* x2 */
        pstAeSnsDft->u32LinesPer500ms = u32Lines * f32MaxFps; /* Because one FSC is equal to 2 VMAX */
        g_astimx335State[vi_pipe].u32RHS1_MAX = g_astimx335State[vi_pipe].u32BRL * 2; /* x2 */
        g_astimx335State[vi_pipe].u32deltaRHS1 =
            (u32Lines * 2) - (g_astimx335State[vi_pipe].u32BRL * 2) - 2; /* Lines * 2 - BRL * 2 - 2 */
    } else {
        pstSnsState->u32FLStd = u32Lines;
        pstAeSnsDft->u32LinesPer500ms = u32Lines * f32MaxFps / 2; /* div 2 */
    }

    pstAeSnsDft->stIntTimeAccu.f32Offset = 0;
    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8; /* sub 8 */
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FlickerFreq = 0;
    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 1;
    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 1;
    pstAeSnsDft->u32ISPDgainShift = 8; /* accuracy: 8 */
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 2 << pstAeSnsDft->u32ISPDgainShift; /* max 2 */
    memcpy_s(&pstAeSnsDft->stPirisAttr, sizeof(ISP_PIRIS_ATTR_S), &g_gstPirisAttr, sizeof(ISP_PIRIS_ATTR_S));
    pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_4;
    pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_5_6;
    pstAeSnsDft->bAERouteExValid = HI_FALSE;
    pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
    pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;
    pstAeSnsDft->u32InitExposure = g_au32InitExposure[vi_pipe] ? g_au32InitExposure[vi_pipe] : 130000; /* 130000 */
    pstAeSnsDft->u32LinesPer500ms = g_au32LinesPer500ms[vi_pipe] ? g_au32LinesPer500ms[vi_pipe] :
                                    pstAeSnsDft->u32LinesPer500ms;
    pstAeSnsDft->stQuickStart.bQuickStartEn = g_quick_start_en[vi_pipe];
    pstAeSnsDft->stQuickStart.u8BlackFrameNum = 0;
    return;
}

static HI_VOID cmos_get_ae_linear_default(VI_PIPE vi_pipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft,
    ISP_SNS_STATE_S *pstSnsState)
{
    pstAeSnsDft->au8HistThresh[0] = 0xd;
    pstAeSnsDft->au8HistThresh[1] = 0x28;
    pstAeSnsDft->au8HistThresh[2] = 0x60; /* index 2 */
    pstAeSnsDft->au8HistThresh[3] = 0x80; /* index 3 */

    pstAeSnsDft->u8AeCompensation = 0x38;
    pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;

    pstAeSnsDft->u32MinIntTime = 2; /* min 2 */
    pstAeSnsDft->u32MaxIntTimeTarget = 65535; /* max 65535 */
    pstAeSnsDft->u32MinIntTimeTarget = pstAeSnsDft->u32MinIntTime;
    pstAeSnsDft->stIntTimeAccu.f32Offset = -0.198; /* offset: -0.198 */

    pstAeSnsDft->u32MaxAgain = IMX335_AGAIN_MAX;
    pstAeSnsDft->u32MinAgain = IMX335_AGAIN_MIN;
    pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
    pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

    pstAeSnsDft->u32MaxDgain = IMX335_DGAIN_MAX; /* if Dgain enable,please set ispdgain bigger than 1 */
    pstAeSnsDft->u32MinDgain = IMX335_DGAIN_MIN;
    pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
    pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;
    pstAeSnsDft->bAERouteExValid = g_abAERouteExValid[vi_pipe];
    memcpy_s(&pstAeSnsDft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S),
        &g_astInitAERoute[vi_pipe], sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&pstAeSnsDft->stAERouteAttrEx, sizeof(ISP_AE_ROUTE_EX_S),
        &g_astInitAERouteEx[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S));
    return;
}

static HI_VOID cmos_get_ae_2to1_line_wdr_default(VI_PIPE vi_pipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft,
    ISP_SNS_STATE_S *pstSnsState)
{
    HI_U32 u32Fll = IMX335_VMAX_5M_30FPS_10BIT_WDR;
    HI_U32 u32MaxFps = 30;
    pstSnsState->u32FLStd = u32Fll * u32MaxFps / SNS_DIV_0_TO_1(g_au32STimeFps[vi_pipe]);

    if ((pstSnsState->u32FLStd % 4) != 0) { /* 4 */
        pstSnsState->u32FLStd = pstSnsState->u32FLStd - (pstSnsState->u32FLStd % 4) + 4; /* 4 */
    }
    pstSnsState->u32FLStd = pstSnsState->u32FLStd * 2; /* 2 */
    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8; /* sub 8 */
    pstAeSnsDft->au8HistThresh[0] = 0xc;
    pstAeSnsDft->au8HistThresh[1] = 0x18;
    pstAeSnsDft->au8HistThresh[2] = 0x60; /* index 2 */
    pstAeSnsDft->au8HistThresh[3] = 0x80; /* index 3 */
    pstAeSnsDft->u8AeCompensation = 0x38;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 4; /* Accuracy 4 */
    pstAeSnsDft->u32MinIntTime = 4; /* min 4 */
    pstAeSnsDft->u32MaxIntTimeTarget = 65535; /* max 65535 */
    pstAeSnsDft->u32MinIntTimeTarget = pstAeSnsDft->u32MinIntTime;
    pstAeSnsDft->stIntTimeAccu.f32Offset = -0.396; /* offset: -0.396 */
    pstAeSnsDft->u32MaxIntTimeStep = 1000; /* MaxIntTimeStep 1000 */
    pstAeSnsDft->bMaxTimeStepEnable = HI_TRUE;
    pstAeSnsDft->u32MaxDecTimeStep[0] = 1000; /* 1000 lines */
    pstAeSnsDft->u32MaxIncTimeStep[0] = 1000; /* 1000 lines */
    pstAeSnsDft->u32LFMinExposure = 15000000; /* LFMinExposure 15000000 */
    pstAeSnsDft->enAeExpMode = AE_EXP_LOWLIGHT_PRIOR;
    pstAeSnsDft->u32MaxAgain = IMX335_AGAIN_MAX;
    pstAeSnsDft->u32MinAgain = IMX335_AGAIN_MIN;
    pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
    pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;
    pstAeSnsDft->u32MaxDgain = IMX335_DGAIN_MAX;  /* if Dgain enable,please set ispdgain bigger than 1 */
    pstAeSnsDft->u32MinDgain = IMX335_DGAIN_MIN;
    pstAeSnsDft->u32MaxDgainTarget = 1024; /* min 1024 */
    pstAeSnsDft->u32MinDgainTarget = 1024; /* min 1024 */
    pstAeSnsDft->u32MaxISPDgainTarget = 16 << pstAeSnsDft->u32ISPDgainShift; /* max 16 << shift */
    pstAeSnsDft->bDiffGainSupport = HI_TRUE;
    pstAeSnsDft->u32ExpRatioMin = 0x80;  /* The exposure ration from 2x to convergence */

    if (g_genFSWDRMode[vi_pipe] != ISP_FSWDR_LONG_FRAME_MODE) {
        pstAeSnsDft->u16ManRatioEnable = HI_TRUE;
        pstAeSnsDft->au32Ratio[0] = 0x400;
        pstAeSnsDft->au32Ratio[1] = 0x40;
        pstAeSnsDft->au32Ratio[2] = 0x40; /* array index 2 */
    }
    pstAeSnsDft->bAERouteExValid = g_abAERouteExValid[vi_pipe];
    memcpy_s(&pstAeSnsDft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), &g_astInitAERoute[vi_pipe],  sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&pstAeSnsDft->stAERouteAttrEx, sizeof(ISP_AE_ROUTE_EX_S),
        &g_astInitAERouteEx[vi_pipe],  sizeof(ISP_AE_ROUTE_EX_S));
    memcpy_s(&pstAeSnsDft->stAERouteSFAttr, sizeof(ISP_AE_ROUTE_S),
        &g_astInitAERouteSF[vi_pipe], sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&pstAeSnsDft->stAERouteSFAttrEx, sizeof(ISP_AE_ROUTE_EX_S),
        &g_astInitAERouteSFEx[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S));
    return;
}

static HI_S32 cmos_get_ae_default(VI_PIPE vi_pipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    CMOS_CHECK_POINTER(pstAeSnsDft);
    IMX335_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    cmos_get_ae_comm_default(vi_pipe, pstAeSnsDft, pstSnsState);

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:
            cmos_get_ae_linear_default(vi_pipe, pstAeSnsDft, pstSnsState);
            break;

        case WDR_MODE_2To1_LINE:
            /* FS WDR mode */
            cmos_get_ae_2to1_line_wdr_default(vi_pipe, pstAeSnsDft, pstSnsState);
            break;
    }
    return HI_SUCCESS;
}
static HI_VOID cmos_config_vmax(ISP_SNS_STATE_S *pstSnsState, HI_U32 u32VMAX)
{
    if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
        pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = IMX335_LOW_8BITS(u32VMAX); /* index 5 */
        pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = IMX335_MID_8BITS(u32VMAX); /* index 6 */
        pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = IMX335_HIG_4BITS(u32VMAX); /* index 7 */
    } else {
        pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = IMX335_LOW_8BITS(u32VMAX); /* index 8 */
        pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = IMX335_MID_8BITS(u32VMAX); /* index 9 */
        pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = IMX335_HIG_4BITS(u32VMAX); /* index 10 */
    }

    return;
}

/* the function of sensor set fps */
static HI_VOID cmos_fps_set(VI_PIPE vi_pipe, HI_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    HI_U32 u32Lines, u32LinesMax, u32VMAX;
    HI_FLOAT f32MaxFps, f32MinFps;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    IMX335_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32Lines = g_astImx335ModeTbl[pstSnsState->u8ImgMode].u32VertiLines;
    u32LinesMax = g_astImx335ModeTbl[pstSnsState->u8ImgMode].u32MaxVertiLines;
    f32MaxFps = g_astImx335ModeTbl[pstSnsState->u8ImgMode].f32MaxFps;
    f32MinFps = g_astImx335ModeTbl[pstSnsState->u8ImgMode].f32MinFps;

    if ((f32Fps > f32MaxFps) || (f32Fps < f32MinFps)) {
        SNS_ERR_TRACE("Not support Fps: %f\n", f32Fps);
        return;
    }

    u32VMAX = u32Lines * f32MaxFps / SNS_DIV_0_TO_1_FLOAT(f32Fps);
    u32VMAX = (u32VMAX > u32LinesMax) ? u32LinesMax : u32VMAX;

    if (pstSnsState->u8ImgMode == IMX335_5M_30FPS_10BIT_WDR_MODE) {
        u32VMAX = ((u32VMAX % 4) != 0) ? (u32VMAX - (u32VMAX % 4) + 4) : u32VMAX; /* u32VMAX % 4 */
    }

    cmos_config_vmax(pstSnsState, u32VMAX);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        pstSnsState->u32FLStd = u32VMAX * 2; /* x2 */
        pstAeSnsDft->u32LinesPer500ms = u32Lines * f32MaxFps; /* Because one FSC is equal to 2 VMAX */
        g_astimx335State[vi_pipe].u32RHS1_MAX = g_astimx335State[vi_pipe].u32BRL * 2; /* x2 */
        g_astimx335State[vi_pipe].u32deltaRHS1 =
            (u32VMAX * 2) - (g_astimx335State[vi_pipe].u32BRL * 2) - 2; /* VMAX * 2 - BRL * 2 - 2 */
        pstAeSnsDft->u32MaxDecTimeStep[0] =
			pstSnsState->u32FLStd - (IMX335_WDR_BRL * 2) - 2 - 10; /* 2 10 limit sensor step */
        pstAeSnsDft->u32MaxIncTimeStep[0] =
			pstSnsState->u32FLStd - (IMX335_WDR_BRL * 2) - 2 - 10; /* 2 10 limit sensor step */
    } else {
        pstSnsState->u32FLStd = u32VMAX;
        pstAeSnsDft->u32LinesPer500ms = u32Lines * f32MaxFps / 2; /* div 2 */
    }

    pstAeSnsDft->f32Fps = f32Fps;
    g_au32STimeFps[vi_pipe] = (HI_U32)f32Fps;
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;

    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8; /* sub 8 */
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
    pstAeSnsDft->u32HmaxTimes =
        (1000000000) / (pstSnsState->u32FLStd * SNS_DIV_0_TO_1_FLOAT(f32Fps)); /* 1000000000ns */

    return;
}

static HI_VOID cmos_slow_framerate_set(VI_PIPE vi_pipe, HI_U32 u32FullLines,
                                       AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    IMX335_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        u32FullLines = (u32FullLines > (2 * 2 * IMX335_VMAX_5M_30FPS_10BIT_WDR)) ? /* x2 x2 */
            (2 * 2 * IMX335_VMAX_5M_30FPS_10BIT_WDR) : u32FullLines; /* x2 x2 */
        if ((u32FullLines % 4) != 0) { /* %4 */
            u32FullLines = u32FullLines + (4 - (u32FullLines % 4));  /* FL + 4 - FL % 4 */
        }
        pstSnsState->au32FL[0] = u32FullLines;

        g_astimx335State[vi_pipe].u32RHS1_MAX = g_astimx335State[vi_pipe].u32BRL * 2; /* x2 */
        g_astimx335State[vi_pipe].u32deltaRHS1 =
            u32FullLines - (g_astimx335State[vi_pipe].u32BRL * 2) - 2; /* FL - BRL * 2 - 2 */
    } else {
        u32FullLines = (u32FullLines > IMX335_FULL_LINES_MAX) ? IMX335_FULL_LINES_MAX : u32FullLines;
        pstSnsState->au32FL[0] = u32FullLines;
    }

    if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
        pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = IMX335_LOW_8BITS(pstSnsState->au32FL[0]); /* index 5 */
        pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = IMX335_MID_8BITS(pstSnsState->au32FL[0]); /* index 6 */
        pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = IMX335_HIG_4BITS(pstSnsState->au32FL[0]); /* index 7 */
    } else {
        pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = IMX335_LOW_8BITS(pstSnsState->au32FL[0] >> 1); /* index 8 */
        pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = IMX335_MID_8BITS(pstSnsState->au32FL[0] >> 1); /* index 9 */
        pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = /* index 10 */
            IMX335_HIG_4BITS(pstSnsState->au32FL[0] >> 1);
    }

    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
    pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 8; /* MaxIntTime: Flstd - 8 */

    return;
}

/* Again and Dgain use the same table, Step is 0.3db */
static HI_U32 g_ad_gain_table[IMX335_AD_GAIN_TBL_RANGE] = {
    1024,    1059,    1097,    1135,    1175,    1217,    1259,
    1304,    1349,    1397,    1446,    1497,    1549,    1604,  /* 3.9dB */
    1660,    1719,    1779,    1842,    1906,    1973,    2043,
    2114,    2189,    2266,    2345,    2428,    2513,    2601,  /* 8.1dB */
    2693,    2788,    2886,    2987,    3092,    3201,    3313,
    3430,    3550,    3675,    3804,    3938,    4076,    4219,  /* 12.3dB */
    4368,    4521,    4680,    4845,    5015,    5191,    5374,
    5562,    5758,    5960,    6170,    6387,    6611,    6843,  /* 16.5dB */
    7084,    7333,    7591,    7857,    8133,    8419,    8715,
    9021,    9338,    9667,    10006,   10358,   10722,   11099,  /* 20.7dB */
    11489,   11893,   12311,   12743,   13191,   13655,   14135,
    14631,   15146,   15678,   16229,   16799,   17390,   18001,  /* 24.9dB */
    18633,   19288,   19966,   20668,   21394,   22146,   22924,
    23730,   24564,   25427,   26320,   27245,   28203,   29194,  /* 29.1dB */
    30220,   31282,   32381,   33519,   34697,   35917,   37179,
    38485,   39838,   41238,   42687,   44187,   45740,   47347,  /* 33.3dB */
    49011,   50734,   52517,   54362,   56273,   58250,   60297,
    62416,   64610,   66880,   69230,   71663,   74182,   76789,  /* 37.5dB */
    79487,   82281,   85172,   88165,   91264,   94471,   97791,
    101228,  104785,  108468,  112279,  116225,  120310,  124537,  /* 41.7dB */
    128914,  133444,  138134,  142988,  148013,  153215,  158599,
    164172,  169942,  175914,  182096,  188495,  195119,  201976,  /* 45.9dB */
    209074,  216421,  224027,  231900,  240049,  248485,  257217,
    266256,  275613,  285299,  295325,  305703,  316446,  327567,  /* 50.1dB */
    339078,  350994,  363329,  376097,  389314,  402995,  417157,
    431817,  446992,  462700,  478961,  495793,  513216,  531251,  /* 54.3dB */
    549921,  569246,  589250,  609958,  631393,  653582,  676550,
    700326,  724936,  750412,  776783,  804081,  832338,  861589,  /* 58.5dB */
    891867,  923209,  955652,  989236,  1024000, 1059985, 1097236,
    1135795, 1175709, 1217026, 1259795, 1304067, 1349895, 1397333,  /* 62.7dB */
    1446438, 1497269, 1549887, 1604353, 1660734, 1719095, 1779508,
    1842044, 1906777, 1973786, 2043149, 2114949, 2189273, 2266209,  /* 66.9dB */
    2345848, 2428287, 2513622, 2601956, 2693394, 2788046, 2886024,
    2987445, 3092431, 3201105, 3313599, 3430046, 3550585, 3675361,  /* 71.1dB */
    3804521, 3938220, 4076617                                       /* 72.0dB */
};

static HI_VOID cmos_inttime_update_linear(VI_PIPE vi_pipe, HI_U32 u32IntTime)
{
    HI_U32 u32Value;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    IMX335_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32Value = pstSnsState->au32FL[0] - u32IntTime;
    u32Value = MIN(u32Value, IMX335_FULL_LINES_MAX);
    u32Value = MIN(MAX(u32Value, 9), (pstSnsState->au32FL[0] - 1)); /* max 9 */

    pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = IMX335_LOW_8BITS(u32Value);   /* SHR0 */
    pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = IMX335_MID_8BITS(u32Value);
    pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = IMX335_HIG_4BITS(u32Value); /* index 2 */

    return;
}

static HI_VOID cmos_inttime_update_reg_2to1_line(HI_U32 u32SHR1, HI_U32 u32RHS1, HI_U32 u32SHR0,
    ISP_SNS_STATE_S *pstSnsState)
{
    pstSnsState->au32WDRIntTime[0] = u32RHS1 - u32SHR1;
    pstSnsState->au32WDRIntTime[1] = pstSnsState->au32FL[0] - u32SHR0;

    pstSnsState->astRegsInfo[0].astI2cData[0].u32Data =  IMX335_LOW_8BITS(u32SHR0);
    pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = IMX335_MID_8BITS(u32SHR0);
    pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = IMX335_HIG_4BITS(u32SHR0); /* index 2 */

    pstSnsState->astRegsInfo[0].astI2cData[5].u32Data =  IMX335_LOW_8BITS(u32SHR1); /* index 5 */
    pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = IMX335_MID_8BITS(u32SHR1); /* index 6 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = IMX335_HIG_4BITS(u32SHR1); /* index 7 */

    pstSnsState->astRegsInfo[0].astI2cData[11].u32Data =  IMX335_LOW_8BITS(u32RHS1); /* index 11 */
    pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = IMX335_MID_8BITS(u32RHS1); /* index 12 */
    pstSnsState->astRegsInfo[0].astI2cData[13].u32Data = IMX335_HIG_4BITS(u32RHS1); /* index 13 */
    return;
}

static HI_VOID cmos_inttime_update_2to1_line_long(VI_PIPE vi_pipe, HI_U32 u32IntTime)
{
    HI_U32 u32RHS1, u32SHR0, u32SHR1, u32RHS1Limit, u32ShortExpMod4, u32ShortExpMod8, u32LimitTmp;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    static HI_U32 u32PreRHS1 = 0;

    IMX335_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->au32WDRIntTime[1] = u32IntTime;
    u32SHR0 = pstSnsState->au32FL[1] - u32IntTime + (u32IntTime % 4); /* IntTime % 4 */
    u32ShortExpMod4 = pstSnsState->au32WDRIntTime[0] % 4; /* au32WDRIntTime % 4 */
    u32ShortExpMod8 = pstSnsState->au32WDRIntTime[0] % 8; /* au32WDRIntTime % 8 */

    if (u32ShortExpMod8 == 0) {
        u32SHR1 = 18; /* min 18 */
        u32RHS1 = 18 + pstSnsState->au32WDRIntTime[0]; /* min 18 */
    } else {
        if (u32ShortExpMod4 == u32ShortExpMod8) {
            u32SHR1 = 18; /* min 18 */
            u32RHS1 = 18 + pstSnsState->au32WDRIntTime[0] - u32ShortExpMod8; /* min 18 */
        } else {
            u32SHR1 = 22; /* min 22 */
            u32RHS1 = 22 + pstSnsState->au32WDRIntTime[0] - u32ShortExpMod4; /* min 22 */
        }
    }

    u32LimitTmp = (IMX335_WDR_BRL * 2) + 2; /* 2 from sensor limit */
    if (pstSnsState->au32FL[0] > u32LimitTmp) {
        u32RHS1Limit = pstSnsState->au32FL[0] - u32LimitTmp;
    } else {
        u32RHS1Limit = u32LimitTmp - pstSnsState->au32FL[0];
    }

    if (u32PreRHS1 > u32RHS1) {
        if ((u32PreRHS1 - u32RHS1) > u32RHS1Limit) {
            u32RHS1 = u32PreRHS1 - (u32RHS1Limit - (u32RHS1Limit % 8)); /* 8x */
        }
    } else {
        if ((u32RHS1 - u32PreRHS1) > u32RHS1Limit) {
            u32RHS1 = u32PreRHS1 + (u32RHS1Limit - (u32RHS1Limit % 8)); /* 8x */
        }
    }

    if ((HI_S32)(u32SHR0 - u32PreRHS1) < 18) { /* difference must > 18 */
        u32SHR0 = u32PreRHS1 + 18; /* difference must > 18 */
    }
    cmos_inttime_update_reg_2to1_line(u32SHR1, u32RHS1, u32SHR0, pstSnsState);
    u32PreRHS1 = u32RHS1;
    return;
}

static HI_VOID cmos_inttime_update_2to1_line(VI_PIPE vi_pipe, HI_U32 u32IntTime)
{
    static HI_BOOL bFirst[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = 1 };

    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    IMX335_SENSOR_GET_CTX(vi_pipe, pstSnsState);
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
    IMX335_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        cmos_inttime_update_2to1_line(vi_pipe, u32IntTime);
    } else {
        cmos_inttime_update_linear(vi_pipe, u32IntTime);
    }

    return;
}

static HI_VOID cmos_again_calc_table(VI_PIPE vi_pipe, HI_U32 *pu32AgainLin, HI_U32 *pu32AgainDb)
{
    int i;

    CMOS_CHECK_POINTER_VOID(pu32AgainLin);
    CMOS_CHECK_POINTER_VOID(pu32AgainDb);

    if (*pu32AgainLin >= g_ad_gain_table[IMX335_AGAIN_TBL_RANGE]) {
        *pu32AgainLin = g_ad_gain_table[IMX335_AGAIN_TBL_RANGE];
        *pu32AgainDb = IMX335_AGAIN_TBL_RANGE;
    } else {
        for (i = 1; i < (IMX335_AGAIN_TBL_RANGE + 1); i++) {
            if (*pu32AgainLin < g_ad_gain_table[i]) {
                *pu32AgainLin = g_ad_gain_table[i - 1];
                *pu32AgainDb = i - 1;
                break;
            }
        }
    }

    g_u32Imx335AGain[vi_pipe] = *pu32AgainLin;

    return;
}

static HI_VOID cmos_dgain_calc_table(VI_PIPE vi_pipe, HI_U32 *pu32DgainLin, HI_U32 *pu32DgainDb)
{
    int i;

    CMOS_CHECK_POINTER_VOID(pu32DgainLin);
    CMOS_CHECK_POINTER_VOID(pu32DgainDb);

    if (*pu32DgainLin >= g_ad_gain_table[IMX335_DGAIN_TBL_RANGE]) {
        *pu32DgainLin = g_ad_gain_table[IMX335_DGAIN_TBL_RANGE];
        *pu32DgainDb = IMX335_DGAIN_TBL_RANGE;
    } else {
        for (i = 1; i < (IMX335_DGAIN_TBL_RANGE + 1); i++) {
            if (*pu32DgainLin < g_ad_gain_table[i]) {
                *pu32DgainLin = g_ad_gain_table[i - 1];
                *pu32DgainDb = i - 1;
                break;
            }
        }
    }

    g_u32Imx335DGain[vi_pipe] = *pu32DgainLin;

    return;
}

static HI_VOID cmos_gains_update(VI_PIPE vi_pipe, HI_U32 u32Again, HI_U32 u32Dgain)
{
    static HI_BOOL bFirstGain[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = 1 };
    HI_U32 u32Tmp;

    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    IMX335_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        if (bFirstGain[vi_pipe]) {
            u32Tmp = u32Again + u32Dgain;
            g_au32LGain[vi_pipe] = u32Tmp;

            pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = IMX335_LOW_8BITS(u32Tmp); /* index 3 */
            pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = IMX335_MID_8BITS(u32Tmp); /* index 4 */
            bFirstGain[vi_pipe] = HI_FALSE;
        } else {
            u32Tmp = u32Again + u32Dgain;

            pstSnsState->astRegsInfo[0].astI2cData[14].u32Data = IMX335_LOW_8BITS(u32Tmp); /* index 14 */
            pstSnsState->astRegsInfo[0].astI2cData[15].u32Data = IMX335_MID_8BITS(u32Tmp); /* index 15 */
            bFirstGain[vi_pipe] = HI_TRUE;
        }
    } else {
        u32Tmp = u32Again + u32Dgain;
        g_au32LGain[vi_pipe] = u32Tmp;

        pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = IMX335_LOW_8BITS(u32Tmp); /* index 3 */
        pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = IMX335_MID_8BITS(u32Tmp); /* index 4 */

        pstSnsState->astRegsInfo[0].astI2cData[14].u32Data = IMX335_LOW_8BITS(u32Tmp); /* index 14 */
        pstSnsState->astRegsInfo[0].astI2cData[15].u32Data = IMX335_MID_8BITS(u32Tmp); /* index 15 */
    }

    return;
}

static HI_U32 g_au32LastIntTimeMaxTmp[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = 4 }; /* int timemax 4 */
static HI_VOID cmos_get_inttime_max_2to1_line_long_frame(VI_PIPE vi_pipe,
    HI_U32 *au32Ratio, HI_U32 *au32IntTimeMax,
    HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    HI_U32 u32ShortMax0, u32ShortMax, u32ShortTimeMinLimit;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    IMX335_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32ShortTimeMinLimit = 4; /* u32ShortTimeMinLimit 4 */

    u32ShortMax0 = pstSnsState->au32FL[1] - 280 - pstSnsState->au32WDRIntTime[0]; /* sub 280 */
    u32ShortMax = pstSnsState->au32FL[0] - 280; /* sub 280 */
    u32ShortMax = (u32ShortMax0 < u32ShortMax) ? u32ShortMax0 : u32ShortMax;
    au32IntTimeMax[1] = u32ShortMax;
    au32IntTimeMin[1] = u32ShortTimeMinLimit;
    g_au32LastIntTimeMaxTmp[vi_pipe] = u32ShortTimeMinLimit;

    return;
}
static HI_VOID cmos_get_inttime_max_2to1_line_auto_long_frame(VI_PIPE vi_pipe, HI_U32 *au32Ratio,
    HI_U32 *au32IntTimeMax, HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    HI_U32 u32ShortMax0, u32ShortMax, u32ShortTimeMinLimit, u32delta, u32deltaRHS1;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    IMX335_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32ShortTimeMinLimit = 4; /* u32ShortTimeMinLimit 4 */
    u32deltaRHS1 = g_astimx335State[vi_pipe].u32deltaRHS1 - (g_astimx335State[vi_pipe].u32deltaRHS1 % 8); /* RHS % 8 */

    if (pstSnsState->au32WDRIntTime[0] == u32ShortTimeMinLimit && au32Ratio[0] == 0x40) {
        u32ShortMax0 = pstSnsState->au32FL[1] - 280 - pstSnsState->au32WDRIntTime[0]; /* sub 280 */
        u32ShortMax = pstSnsState->au32FL[0] - 280; /* sub 280 */
        u32ShortMax = (u32ShortMax0 < u32ShortMax) ? u32ShortMax0 : u32ShortMax;
        au32IntTimeMax[0] = u32ShortMax;
        au32IntTimeMin[0] = u32ShortTimeMinLimit;
        g_au32LastIntTimeMaxTmp[vi_pipe] = u32ShortTimeMinLimit;
        return;
    }
    u32ShortMax0 = ((pstSnsState->au32FL[1] - 280 - pstSnsState->au32WDRIntTime[0]) * 0x40) / /* sub 280 */
        SNS_DIV_0_TO_1(au32Ratio[0]);
    u32ShortMax = ((pstSnsState->au32FL[0] - 280) * 0x40) / SNS_DIV_0_TO_1(au32Ratio[0] + 0x40); /* sub 280 */
    u32ShortMax = (u32ShortMax0 < u32ShortMax) ? u32ShortMax0 : u32ShortMax;
    u32ShortMax = (u32ShortMax > (g_astimx335State[vi_pipe].u32RHS1_MAX - 18)) ? /* sub 18 */
        (g_astimx335State[vi_pipe].u32RHS1_MAX - 18) : u32ShortMax; /* sub 18 */
    if (u32ShortMax > g_au32LastIntTimeMaxTmp[vi_pipe]) {
        u32delta = u32ShortMax - g_au32LastIntTimeMaxTmp[vi_pipe];
        if (u32delta > g_astimx335State[vi_pipe].u32deltaRHS1) {
            u32ShortMax = g_au32LastIntTimeMaxTmp[vi_pipe] + u32deltaRHS1;
        }
    } else if (g_au32LastIntTimeMaxTmp[vi_pipe] > u32ShortMax) {
        u32delta = g_au32LastIntTimeMaxTmp[vi_pipe] - u32ShortMax;
        if (u32delta > g_astimx335State[vi_pipe].u32deltaRHS1) {
            u32ShortMax = g_au32LastIntTimeMaxTmp[vi_pipe] - u32deltaRHS1;
        }
    }
    g_au32LastIntTimeMaxTmp[vi_pipe] = u32ShortMax ? u32ShortMax : 1;

    if (u32ShortMax >= u32ShortTimeMinLimit) {
        au32IntTimeMax[0] = u32ShortMax;
        au32IntTimeMax[1] = (au32IntTimeMax[0] * au32Ratio[0]) >> 6; /* shift 6 */
        au32IntTimeMin[0] = u32ShortTimeMinLimit;
        au32IntTimeMin[1] = (au32IntTimeMin[0] * au32Ratio[0]) >> 6; /* shift 6 */
        return;
    }

    u32ShortMax = u32ShortTimeMinLimit;
    au32IntTimeMax[0] = u32ShortMax;
    au32IntTimeMax[1] = (au32IntTimeMax[0] * 0xFFF) >> 6; /* shift 6 */
    au32IntTimeMin[0] = au32IntTimeMax[0];
    au32IntTimeMin[1] = au32IntTimeMax[1];

    return;
}
static HI_VOID cmos_get_inttime_max_2to1_line_normal(VI_PIPE vi_pipe,
    HI_U32 *au32Ratio, HI_U32 *au32IntTimeMax,
    HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    HI_U32 u32ShortMax0, u32ShortMax, u32ShortTimeMinLimit;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    IMX335_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32ShortTimeMinLimit = 4; /* u32ShortTimeMinLimit 4 */

    u32ShortMax0 = ((pstSnsState->au32FL[1] - 280 - pstSnsState->au32WDRIntTime[0]) * 0x40) / /* sub 280 */
        SNS_DIV_0_TO_1(au32Ratio[0]);
    u32ShortMax = ((pstSnsState->au32FL[0] - 280) * 0x40) / SNS_DIV_0_TO_1(au32Ratio[0] + 0x40); /* sub 280 */
    u32ShortMax = (u32ShortMax0 < u32ShortMax) ? u32ShortMax0 : u32ShortMax;
    u32ShortMax = (u32ShortMax > (g_astimx335State[vi_pipe].u32RHS1_MAX - 18)) ? /* sub 18 */
        (g_astimx335State[vi_pipe].u32RHS1_MAX - 18) : u32ShortMax; /* sub 18 */
    u32ShortMax = (u32ShortMax == 0) ? 1 : u32ShortMax;
    g_au32LastIntTimeMaxTmp[vi_pipe] = u32ShortMax;
    *pu32LFMaxIntTime = g_astimx335State[vi_pipe].u32RHS1_MAX - 18; /* sub 18 */

    if (u32ShortMax >= u32ShortTimeMinLimit) {
        au32IntTimeMax[0] = u32ShortMax;
        au32IntTimeMax[1] = (au32IntTimeMax[0] * au32Ratio[0]) >> 6; /* shift 6 */
        au32IntTimeMin[0] = u32ShortTimeMinLimit;
        au32IntTimeMin[1] = (au32IntTimeMin[0] * au32Ratio[0]) >> 6; /* shift 6 */
        return;
    }

    u32ShortMax = u32ShortTimeMinLimit;
    au32IntTimeMax[0] = u32ShortMax;
    au32IntTimeMax[1] = (au32IntTimeMax[0] * 0xFFF) >> 6; /* shift 6 */
    au32IntTimeMin[0] = au32IntTimeMax[0];
    au32IntTimeMin[1] = au32IntTimeMax[1];

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

static HI_VOID cmos_get_inttime_max(VI_PIPE vi_pipe, HI_U16 u16ManRatioEnable,
    HI_U32 *au32Ratio, HI_U32 *au32IntTimeMax,
    HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    CMOS_CHECK_POINTER_VOID(au32Ratio);
    CMOS_CHECK_POINTER_VOID(au32IntTimeMax);
    CMOS_CHECK_POINTER_VOID(au32IntTimeMin);
    CMOS_CHECK_POINTER_VOID(pu32LFMaxIntTime);
    IMX335_SENSOR_GET_CTX(vi_pipe, pstSnsState);
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
    CMOS_CHECK_POINTER_VOID(pstAeFSWDRAttr);

    g_genFSWDRMode[vi_pipe]      = pstAeFSWDRAttr->enFSWDRMode;
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
    pstExpFuncs->pfn_cmos_again_calc_table  = cmos_again_calc_table;
    pstExpFuncs->pfn_cmos_dgain_calc_table  = cmos_dgain_calc_table;
    pstExpFuncs->pfn_cmos_get_inttime_max   = cmos_get_inttime_max;
    pstExpFuncs->pfn_cmos_ae_fswdr_attr_set = cmos_ae_fswdr_attr_set;

    return HI_SUCCESS;
}

/* AWB default parameter and function */
#define CALIBRATE_STATIC_TEMP             4900
#define CALIBRATE_STATIC_WB_R_GAIN        0x1E3
#define CALIBRATE_STATIC_WB_GR_GAIN       0x100
#define CALIBRATE_STATIC_WB_GB_GAIN       0x100
#define CALIBRATE_STATIC_WB_B_GAIN        0x1d1

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1                  (-0x0012)
#define CALIBRATE_AWB_P2                  0x010b
#define CALIBRATE_AWB_Q1                  (-0x0007)
#define CALIBRATE_AWB_A1                  0x2711F
#define CALIBRATE_AWB_B1                  0x0080
#define CALIBRATE_AWB_C1                  (-0x1A5C1)

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN                      0
#define GOLDEN_BGAIN                      0
static HI_S32 cmos_get_awb_default(VI_PIPE vi_pipe, AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstAwbSnsDft);
    IMX335_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset_s(pstAwbSnsDft, sizeof(AWB_SENSOR_DEFAULT_S), 0, sizeof(AWB_SENSOR_DEFAULT_S));

    pstAwbSnsDft->u16WbRefTemp = CALIBRATE_STATIC_TEMP;

    pstAwbSnsDft->au16GainOffset[0] = CALIBRATE_STATIC_WB_R_GAIN;
    pstAwbSnsDft->au16GainOffset[1] = CALIBRATE_STATIC_WB_GR_GAIN;
    pstAwbSnsDft->au16GainOffset[2] = CALIBRATE_STATIC_WB_GB_GAIN; /* index 2 */
    pstAwbSnsDft->au16GainOffset[3] = CALIBRATE_STATIC_WB_B_GAIN; /* index 3 */

    pstAwbSnsDft->as32WbPara[0] = CALIBRATE_AWB_P1;
    pstAwbSnsDft->as32WbPara[1] = CALIBRATE_AWB_P2;
    pstAwbSnsDft->as32WbPara[2] = CALIBRATE_AWB_Q1; /* index 2 */
    pstAwbSnsDft->as32WbPara[3] = CALIBRATE_AWB_A1; /* index 3 */
    pstAwbSnsDft->as32WbPara[4] = CALIBRATE_AWB_B1; /* index 4 */
    pstAwbSnsDft->as32WbPara[5] = CALIBRATE_AWB_C1; /* index 5 */

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

    pstAwbSnsDft->u16InitRgain = g_au16InitWBGain[vi_pipe][0];
    pstAwbSnsDft->u16InitGgain = g_au16InitWBGain[vi_pipe][1];
    pstAwbSnsDft->u16InitBgain = g_au16InitWBGain[vi_pipe][2]; /* index 2 */
    pstAwbSnsDft->u16SampleRgain = g_au16SampleRgain[vi_pipe];
    pstAwbSnsDft->u16SampleBgain = g_au16SampleBgain[vi_pipe];

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

    switch (pstSnsState->u8ImgMode) {
        default:
        case IMX335_5M_30FPS_12BIT_LINEAR_MODE:
            pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 12; /* 12bit */
            pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 4095; /* max 4095 */
            break;

        case IMX335_5M_30FPS_10BIT_WDR_MODE:
            pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 10; /* 10bit */
            pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 1023; /* max 1023 */
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
    pstDef->unKey.bit1Lsc = 1;
    pstDef->pstLsc = &g_stCmosLsc_6mm;
    pstDef->unKey.bit1Lcac = 1;
    pstDef->pstLcac = &g_stIspLCac;
    memcpy_s(&pstDef->stNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S),
             &g_stIspNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
    return;
}

static void cmos_get_isp_2to1_line_wdr_default(ISP_CMOS_DEFAULT_S *pstDef)
{
    pstDef->unKey.bit1Clut           = 1;
    pstDef->pstClut                  = &g_stIspCLUTWDR;
    pstDef->unKey.bit1Dpc            = 1;
    pstDef->pstDpc                   = &g_stCmosDpcWDR;
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
    pstDef->unKey.bit1Ldci           = 0;
    pstDef->pstLdci                  = &g_stIspWdrLdci;
    pstDef->unKey.bit1Dehaze         = 0;
    pstDef->pstDehaze                = &g_stIspDehazeWDR;
    pstDef->unKey.bit1Lsc            = 1;
    pstDef->pstLsc                   = &g_stCmosLsc_4mm;
    pstDef->unKey.bit1Lcac           = 1;
    pstDef->pstLcac                  = &g_stIspLCacWdr;

    pstDef->stWdrSwitchAttr.au32ExpRatio[0] = 0x400;

    memcpy_s(&pstDef->stNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S), &g_stIspNoiseCalibration,
             sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
    return;
}

static HI_S32 cmos_get_isp_default(VI_PIPE vi_pipe, ISP_CMOS_DEFAULT_S *pstDef)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstDef);
    IMX335_SENSOR_GET_CTX(vi_pipe, pstSnsState);
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
    pstDef->stSensorMode.u32SensorID = IMX335_ID;
    pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;
    cmos_get_isp_dng_default(pstSnsState, pstDef);

    return HI_SUCCESS;
}
static HI_VOID cmos_get_isp_blacklevel_cal(VI_PIPE vi_pipe, ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
    if ((g_u32Imx335DGain[vi_pipe] >= 12288) && (g_u32Imx335DGain[vi_pipe] < 16384)) { /* range;[12288, 16384) */
        pstBlackLevel->au16BlackLevel[0] = 217; /* index 0, value 217 */
        pstBlackLevel->au16BlackLevel[1] = 211; /* index 1, value 211 */
        pstBlackLevel->au16BlackLevel[2] = 208; /* index 2, value 208 */
        pstBlackLevel->au16BlackLevel[3] = 216; /* index 3, value 216 */
        return;
    }
    if ((g_u32Imx335DGain[vi_pipe] >= 16384) && (g_u32Imx335DGain[vi_pipe] < 20480)) { /* range;[16384, 20480) */
        pstBlackLevel->au16BlackLevel[0] = 223; /* index 0, value 223 */
        pstBlackLevel->au16BlackLevel[1] = 228; /* index 1, value 228 */
        pstBlackLevel->au16BlackLevel[2] = 229; /* index 2, value 229 */
        pstBlackLevel->au16BlackLevel[3] = 219; /* index 3, value 219 */
        return;
    }
    if ((g_u32Imx335DGain[vi_pipe] >= 20480) && (g_u32Imx335DGain[vi_pipe] < 24576)) { /* range;[20480, 24576) */
        pstBlackLevel->au16BlackLevel[0] = 248; /* index 0, value 248 */
        pstBlackLevel->au16BlackLevel[1] = 255; /* index 1, value 255 */
        pstBlackLevel->au16BlackLevel[2] = 256; /* index 2, value 256 */
        pstBlackLevel->au16BlackLevel[3] = 244; /* index 3, value 244 */
        return;
    }
    if ((g_u32Imx335DGain[vi_pipe] >= 24576) && (g_u32Imx335DGain[vi_pipe] < 28672)) { /* range;[24576, 28672) */
        pstBlackLevel->au16BlackLevel[0] = 277; /* index 0, value 277 */
        pstBlackLevel->au16BlackLevel[1] = 283; /* index 1, value 283 */
        pstBlackLevel->au16BlackLevel[2] = 285; /* index 2, value 285 */
        pstBlackLevel->au16BlackLevel[3] = 272; /* index 3, value 272 */
        return;
    }
    if ((g_u32Imx335DGain[vi_pipe] >= 28672) && (g_u32Imx335DGain[vi_pipe] < 31768)) { /* range;[28672, 31768) */
        pstBlackLevel->au16BlackLevel[0] = 303; /* index 0, value 303 */
        pstBlackLevel->au16BlackLevel[1] = 308; /* index 1, value 308 */
        pstBlackLevel->au16BlackLevel[2] = 310; /* index 2, value 310 */
        pstBlackLevel->au16BlackLevel[3] = 295; /* index 3, value 295 */
        return;
    }
    pstBlackLevel->au16BlackLevel[0] = 330; /* index 0, value 330 */
    pstBlackLevel->au16BlackLevel[1] = 338; /* index 1, value 338 */
    pstBlackLevel->au16BlackLevel[2] = 339; /* index 2, value 339 */
    pstBlackLevel->au16BlackLevel[3] = 323; /* index 3, value 323 */
    return;
}
static HI_S32 cmos_get_isp_black_level(VI_PIPE vi_pipe, ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
    HI_S32  i;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    IMX335_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    if (pstBlackLevel == HI_NULL) {
        printf("null pointer when get isp black level value!\n");
        return -1;
    }
    /* Don't need to update black level when iso change */
    pstBlackLevel->bUpdate = HI_TRUE;

    if (pstBlackLevel->bUpdate == HI_TRUE) {
        if (g_u32Imx335AGain[vi_pipe] < 32381) { /* range;[0, 32381) */
            for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
                pstBlackLevel->au16BlackLevel[i] = 0xC8; /* 12bit,0xC8 */
            }
            return 0;
        }
        if (g_u32Imx335DGain[vi_pipe] < 12288) { /* range;[0, 12288) */
            for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
                pstBlackLevel->au16BlackLevel[i] = 0xC8; /* 12bit,0xC8 */
            }
            return 0;
        }
        cmos_get_isp_blacklevel_cal(vi_pipe, pstBlackLevel);
        return 0;
    }
    if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            pstBlackLevel->au16BlackLevel[i] = 0xC8; /* 12bit,0xC8 */
        }
        return 0;
    }
    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            pstBlackLevel->au16BlackLevel[i] = 0xC8; /* 10bit 50, 12bit 200 */
        }
        return 0;
    }
    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        pstBlackLevel->au16BlackLevel[i] = 0xC8; /* 10bit 50, 12bit 200 */
    }
    return 0;
}
static HI_VOID cmos_set_pixel_detect(VI_PIPE vi_pipe, HI_BOOL bEnable)
{
    HI_U32 u32FullLines_5Fps = 0;
    HI_U32 u32MaxIntTime_5Fps;

    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    IMX335_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        return;
    } else {
        if (pstSnsState->u8ImgMode == IMX335_5M_30FPS_12BIT_LINEAR_MODE) {
            u32FullLines_5Fps = (IMX335_VMAX_5M_30FPS_12BIT_LINEAR * 30) / 5; /* 30fps, 5fps */
        } else {
            return;
        }
    }

    u32MaxIntTime_5Fps = u32FullLines_5Fps - 8; /* sub 8 */

    if (bEnable) { /* setup for ISP pixel calibration mode */
        IMX335_write_register(vi_pipe, IMX335_GAIN_LONG_LOW, 0x00); /* gain */
        IMX335_write_register(vi_pipe, IMX335_GAIN_LONG_HIGH, 0x00);

        IMX335_write_register(vi_pipe, IMX335_VMAX_ADDR_L, IMX335_LOW_8BITS(u32FullLines_5Fps));    /* VMAX */
        IMX335_write_register(vi_pipe, IMX335_VMAX_ADDR_M, IMX335_MID_8BITS(u32FullLines_5Fps));
        IMX335_write_register(vi_pipe, IMX335_VMAX_ADDR_H, IMX335_HIG_4BITS(u32FullLines_5Fps));

        IMX335_write_register(vi_pipe, IMX335_SHR0_LOW, IMX335_LOW_8BITS(u32MaxIntTime_5Fps));
        IMX335_write_register(vi_pipe, IMX335_SHR0_MIDDLE, IMX335_MID_8BITS(u32MaxIntTime_5Fps));
        IMX335_write_register(vi_pipe, IMX335_SHR0_MIDDLE, IMX335_HIG_4BITS(u32MaxIntTime_5Fps));
    } else { /* setup for ISP 'normal mode' */
        pstSnsState->u32FLStd = (pstSnsState->u32FLStd > IMX335_FULL_LINES_MAX) ? IMX335_FULL_LINES_MAX
                                : pstSnsState->u32FLStd;
        IMX335_write_register(vi_pipe, IMX335_VMAX_ADDR_L, IMX335_LOW_8BITS(pstSnsState->u32FLStd));
        IMX335_write_register(vi_pipe, IMX335_VMAX_ADDR_M, IMX335_MID_8BITS(pstSnsState->u32FLStd));
        IMX335_write_register(vi_pipe, IMX335_VMAX_ADDR_H, IMX335_HIG_4BITS(pstSnsState->u32FLStd));
        pstSnsState->bSyncInit = HI_FALSE;
    }

    return;
}

static HI_S32 cmos_set_wdr_mode(VI_PIPE vi_pipe, HI_U8 u8Mode)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    IMX335_SENSOR_GET_CTX(vi_pipe, pstSnsState);
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
    pstSnsState->astRegsInfo[0].unComBus.s8I2cDev = g_aunImx335BusInfo[vi_pipe].s8I2cDev;
    pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2; /* DelayMax 2 */
    pstSnsState->astRegsInfo[0].u32RegNum = 8; /* RegNum 8 */

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        pstSnsState->astRegsInfo[0].u32RegNum += 8; /* RegNum add 8 */
    }

    for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
        pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
        pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr = IMX335_I2C_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[i].u32AddrByteNum = IMX335_ADDR_BYTE;
        pstSnsState->astRegsInfo[0].astI2cData[i].u32DataByteNum = IMX335_DATA_BYTE;
    }

    /* shutter related */
    pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
    pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr = IMX335_SHR0_LOW;        /* SHR0 */
    pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
    pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr = IMX335_SHR0_MIDDLE;
    pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0; /* index 2 */
    pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr = IMX335_SHR0_HIGH; /* index 2 */

    pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 0; /* index 3 */
    pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr = IMX335_GAIN_LONG_LOW; /* index 3 */
    pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 0; /* index 4 */
    pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr = IMX335_GAIN_LONG_HIGH; /* index 4 */

    /* Vmax */
    pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0; /* index 5 */
    pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr = IMX335_VMAX_ADDR_L; /* index 5 */
    pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0; /* index 6 */
    pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr = IMX335_VMAX_ADDR_M; /* index 6 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 0; /* index 7 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr = IMX335_VMAX_ADDR_H; /* index 7 */
    return;
}
static HI_VOID cmos_2to1_line_wdr_sns_reg_info_init(VI_PIPE vi_pipe, ISP_SNS_STATE_S *pstSnsState)
{
    pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 1; /* index 3 */
    pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 1; /* index 4 */

    pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0; /* index 5 */
    pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr = IMX335_SHR1_LOW; /* index 5 */
    pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0; /* index 6 */
    pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr = IMX335_SHR1_MIDDLE; /* index 6 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 0; /* index 7 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr = IMX335_SHR1_HIGH; /* index 7 */

    pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 1; /* index 8 */
    pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr = IMX335_VMAX_ADDR_L; /* index 8 */
    pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum = 1; /* index 9 */
    pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr = IMX335_VMAX_ADDR_M; /* index 9 */
    pstSnsState->astRegsInfo[0].astI2cData[10].u8DelayFrmNum = 1; /* index 10 */
    pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr = IMX335_VMAX_ADDR_H; /* index 10 */

    pstSnsState->astRegsInfo[0].astI2cData[11].u8DelayFrmNum = 0; /* index 11 */
    pstSnsState->astRegsInfo[0].astI2cData[11].u32RegAddr = IMX335_RHS1_LOW; /* index 11 */
    pstSnsState->astRegsInfo[0].astI2cData[12].u8DelayFrmNum = 0; /* index 12 */
    pstSnsState->astRegsInfo[0].astI2cData[12].u32RegAddr = IMX335_RHS1_MIDDLE; /* index 12 */
    pstSnsState->astRegsInfo[0].astI2cData[13].u8DelayFrmNum = 0; /* index 13 */
    pstSnsState->astRegsInfo[0].astI2cData[13].u32RegAddr = IMX335_RHS1_HIGH; /* index 13 */

    pstSnsState->astRegsInfo[0].astI2cData[14].u8DelayFrmNum = 1; /* index 14 */
    pstSnsState->astRegsInfo[0].astI2cData[14].u32RegAddr = IMX335_GAIN_SHORT_LOW; /* index 14 */
    pstSnsState->astRegsInfo[0].astI2cData[15].u8DelayFrmNum = 1; /* index 15 */
    pstSnsState->astRegsInfo[0].astI2cData[15].u32RegAddr = IMX335_GAIN_SHORT_HIGH; /* index 15 */
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
    IMX335_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    if ((pstSnsState->bSyncInit == HI_FALSE) || (pstSnsRegsInfo->bConfig == HI_FALSE)) {
        cmos_comm_sns_reg_info_init(vi_pipe, pstSnsState);

        if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
            cmos_2to1_line_wdr_sns_reg_info_init(vi_pipe, pstSnsState);
        }

        pstSnsState->bSyncInit = HI_TRUE;
    } else {
        cmos_sns_reg_info_update(vi_pipe, pstSnsState);
    }
    pstSnsRegsInfo->bConfig = HI_FALSE;
    memcpy_s(pstSnsRegsInfo, sizeof(ISP_SNS_REGS_INFO_S), &pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S));
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
    IMX335_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    u8SensorImageMode = pstSnsState->u8ImgMode;
    pstSnsState->bSyncInit = HI_FALSE;
    switch (pstSnsState->enWDRMode) {
        case WDR_MODE_2To1_LINE:
            if (IMX335_RES_IS_5M_10BIT_WDR(pstSensorImageMode->u16Width, pstSensorImageMode->u16Height) ||
                IMX335_RES_IS_4M_10BIT_WDR(pstSensorImageMode->u16Width, pstSensorImageMode->u16Height)) {
                u8SensorImageMode               = IMX335_5M_30FPS_10BIT_WDR_MODE;
                g_astimx335State[vi_pipe].u32BRL = 1984 * 2; /* BRL 1984 * 2 */
                pstSnsState->u32FLStd           = IMX335_VMAX_5M_30FPS_10BIT_WDR;
                break;
            }
            IMX335_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState);
            return HI_FAILURE;
        case WDR_MODE_NONE:
            if (IMX335_RES_IS_5M_12BIT_LINEAR(pstSensorImageMode->u16Width, pstSensorImageMode->u16Height) ||
                IMX335_RES_IS_4M_12BIT_LINEAR(pstSensorImageMode->u16Width, pstSensorImageMode->u16Height)) {
                u8SensorImageMode               = IMX335_5M_30FPS_12BIT_LINEAR_MODE;
                g_astimx335State[vi_pipe].u32BRL = 1984 * 2; /* BRL 1984 * 2 */
                pstSnsState->u32FLStd           = IMX335_VMAX_5M_30FPS_12BIT_LINEAR;
                break;
            }
            IMX335_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState);
            return HI_FAILURE;
        default:
            IMX335_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState);
            return HI_FAILURE;
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

    IMX335_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->bInit = HI_FALSE;
    pstSnsState->bSyncInit = HI_FALSE;
    pstSnsState->u8ImgMode = IMX335_5M_30FPS_12BIT_LINEAR_MODE;
    pstSnsState->enWDRMode = WDR_MODE_NONE;
    pstSnsState->u32FLStd = IMX335_VMAX_5M_30FPS_12BIT_LINEAR;
    pstSnsState->au32FL[0] = IMX335_VMAX_5M_30FPS_12BIT_LINEAR;
    pstSnsState->au32FL[1] = IMX335_VMAX_5M_30FPS_12BIT_LINEAR;

    memset_s(&pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset_s(&pstSnsState->astRegsInfo[1], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
    return;
}

static HI_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    CMOS_CHECK_POINTER(pstSensorExpFunc);

    memset_s(pstSensorExpFunc, sizeof(ISP_SENSOR_EXP_FUNC_S), 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init         = IMX335_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit         = IMX335_exit;
    pstSensorExpFunc->pfn_cmos_sensor_global_init  = sensor_global_init;
    pstSensorExpFunc->pfn_cmos_set_image_mode      = cmos_set_image_mode;
    pstSensorExpFunc->pfn_cmos_set_wdr_mode        = cmos_set_wdr_mode;
    pstSensorExpFunc->pfn_cmos_get_isp_default     = cmos_get_isp_default;
    pstSensorExpFunc->pfn_cmos_get_isp_black_level = cmos_get_isp_black_level;
    pstSensorExpFunc->pfn_cmos_set_pixel_detect    = cmos_set_pixel_detect;
    pstSensorExpFunc->pfn_cmos_get_sns_reg_info    = cmos_get_sns_regs_info;

    return HI_SUCCESS;
}

/****************************************************************************
 * callback structure                                                       *
 ****************************************************************************/
static HI_S32 IMX335_set_bus_info(VI_PIPE vi_pipe, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunImx335BusInfo[vi_pipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

    return HI_SUCCESS;
}

static HI_S32 sensor_ctx_init(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    IMX335_SENSOR_GET_CTX(vi_pipe, pastSnsStateCtx);

    if (pastSnsStateCtx == HI_NULL) {
        pastSnsStateCtx = (ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
        if (pastSnsStateCtx == HI_NULL) {
            SNS_ERR_TRACE("Isp[%d] SnsCtx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset_s(pastSnsStateCtx, sizeof(ISP_SNS_STATE_S), 0, sizeof(ISP_SNS_STATE_S));

    IMX335_SENSOR_SET_CTX(vi_pipe, pastSnsStateCtx);

    return HI_SUCCESS;
}

static HI_VOID sensor_ctx_exit(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    IMX335_SENSOR_GET_CTX(vi_pipe, pastSnsStateCtx);
    SENSOR_FREE(pastSnsStateCtx);
    IMX335_SENSOR_RESET_CTX(vi_pipe);
    return;
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

    stSnsAttrInfo.eSensorId = IMX335_ID;
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

    s32Ret = HI_MPI_ISP_SensorUnRegCallBack(vi_pipe, IMX335_ID);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AE_SensorUnRegCallBack(vi_pipe, pstAeLib, IMX335_ID);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AWB_SensorUnRegCallBack(vi_pipe, pstAwbLib, IMX335_ID);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function to awb lib failed!\n");
        return s32Ret;
    }

    sensor_ctx_exit(vi_pipe);

    return HI_SUCCESS;
}

static HI_S32 sensor_set_init(VI_PIPE vi_pipe, ISP_INIT_ATTR_S *pstInitAttr)
{
    CMOS_CHECK_POINTER(pstInitAttr);

    g_au32InitExposure[vi_pipe]  = pstInitAttr->u32Exposure;
    g_au32LinesPer500ms[vi_pipe] = pstInitAttr->u32LinesPer500ms;
    g_au16InitWBGain[vi_pipe][0] = pstInitAttr->u16WBRgain;
    g_au16InitWBGain[vi_pipe][1] = pstInitAttr->u16WBGgain;
    g_au16InitWBGain[vi_pipe][2] = pstInitAttr->u16WBBgain; /* index 2 */
    g_au16SampleRgain[vi_pipe]   = pstInitAttr->u16SampleRgain;
    g_au16SampleBgain[vi_pipe]   = pstInitAttr->u16SampleBgain;
    g_quick_start_en[vi_pipe] = pstInitAttr->stQuickStart.bQuickStartEn;

    g_abAERouteExValid[vi_pipe] = pstInitAttr->bAERouteExValid;
    memcpy_s(&g_astInitAERoute[vi_pipe], sizeof(ISP_AE_ROUTE_S), &pstInitAttr->stAERoute, sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&g_astInitAERouteEx[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S),
        &pstInitAttr->stAERouteEx, sizeof(ISP_AE_ROUTE_EX_S));
    memcpy_s(&g_astInitAERouteSF[vi_pipe], sizeof(ISP_AE_ROUTE_S), &pstInitAttr->stAERouteSF, sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&g_astInitAERouteSFEx[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S),
        &pstInitAttr->stAERouteSFEx, sizeof(ISP_AE_ROUTE_EX_S));

    return HI_SUCCESS;
}

ISP_SNS_OBJ_S stSnsImx335Obj = {
    .pfnRegisterCallback    = sensor_register_callback,
    .pfnUnRegisterCallback  = sensor_unregister_callback,
    .pfnStandby             = IMX335_standby,
    .pfnRestart             = IMX335_restart,
    .pfnMirrorFlip          = HI_NULL,
    .pfnWriteReg            = IMX335_write_register,
    .pfnReadReg             = IMX335_read_register,
    .pfnSetBusInfo          = IMX335_set_bus_info,
    .pfnSetInit             = sensor_set_init
};

