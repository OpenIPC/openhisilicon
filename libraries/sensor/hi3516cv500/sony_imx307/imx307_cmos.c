/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of imx307_cmos.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "imx307_cmos.h"
#include "imx307_cmos_ex.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "mpi_isp.h"
#include "mpi_ae.h"
#include "mpi_awb.h"

#define IMX307_ID    307

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/
#define HIGHER_4BITS(x) (((x) & 0xf0000) >> 16)
#define HIGH_8BITS(x) (((x) & 0xff00) >> 8)
#define LOW_8BITS(x)  ((x) & 0x00ff)

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ? (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ? (b) : (a))
#endif

ISP_SNS_STATE_S *g_pastImx307[ISP_MAX_PIPE_NUM] = {HI_NULL};
#define IMX307_SENSOR_SET_CTX(dev, pstCtx) ((g_pastImx307[dev]) = (pstCtx))
#define IMX307_SENSOR_RESET_CTX(dev)       (g_pastImx307[dev] = HI_NULL)

ISP_SNS_COMMBUS_U g_aunImx307BusInfo[ISP_MAX_PIPE_NUM] = {
    [0] = { .s8I2cDev = 0},
    [1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1}
};

ISP_SNS_COMMBUS_U *imx307_get_bus_Info(VI_PIPE vi_pipe)
{
    return &g_aunImx307BusInfo[vi_pipe];
}

ISP_SNS_STATE_S *imx307_get_ctx(VI_PIPE vi_pipe)
{
    return g_pastImx307[vi_pipe];
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

static HI_BOOL g_bFirstFps[ISP_MAX_PIPE_NUM] = {[0 ... (ISP_MAX_PIPE_NUM - 1)] = 1};
typedef struct hiIMX307_STATE_S {
    HI_U8 u8Hcg;
    HI_U32 u32BRL;
    HI_U32 u32RHS1_MAX;
    HI_U32 u32RHS2_MAX;
} IMX307_STATE_S;

IMX307_STATE_S g_astimx307State[ISP_MAX_PIPE_NUM] = {{0}};

/****************************************************************************
 * local variables                                                            *
 ***************************************************************************/
#define IMX307_FULL_LINES_MAX 0x3FFFF
#define IMX307_FULL_LINES_MAX_2TO1_WDR 0x966  /* considering the YOUT_SIZE and bad frame */

/* ****Imx307 Register Address**** */
#define IMX307_SHS1_ADDR 0x3020
#define IMX307_SHS2_ADDR 0x3024
#define IMX307_GAIN_ADDR 0x3014
#define IMX307_HCG_ADDR 0x3009
#define IMX307_VMAX_ADDR 0x3018
#define IMX307_HMAX_ADDR 0x301c
#define IMX307_RHS1_ADDR 0x3030
#define IMX307_RHS2_ADDR 0x3034
#define IMX307_Y_OUT_SIZE_ADDR 0x3418

#define IMX307_INCREASE_LINES 0 /* make real fps less than stand fps because NVR require */

#define IMX307_VMAX_1080P30_LINEAR (1125 + IMX307_INCREASE_LINES)

#define IMX307_VMAX_1080P60TO30_WDR (1220 + IMX307_INCREASE_LINES)  /* 10bit */

/* sensor fps mode */
#define IMX307_SENSOR_1080P_30FPS_LINEAR_MODE 1
#define IMX307_SENSOR_1080P_30FPS_2t1_WDR_MODE 2

#define IMX307_RES_IS_1080P(w, h) ((w) <= 1920 && (h) <= 1080)

#define IMX307_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState)\
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
    if (g_au32LinesPer500ms[vi_pipe] == 0) {
        pstAeSnsDft->u32LinesPer500ms = pstSnsState->u32FLStd * 30 / 2; /* 30fps / 2 */
    } else {
        pstAeSnsDft->u32LinesPer500ms = g_au32LinesPer500ms[vi_pipe];
    }
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FlickerFreq = 50 * 256; /* light flicker freq: 50Hz, accuracy: 256 */
    pstAeSnsDft->u32FullLinesMax = IMX307_FULL_LINES_MAX;

    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
    pstAeSnsDft->stIntTimeAccu.f32Offset = 0;

    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 1;

    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 1;

    pstAeSnsDft->u32ISPDgainShift = 8; /* accuracy: 8 */
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 2 << pstAeSnsDft->u32ISPDgainShift; /* max 2 */

    pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_0;
    pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_32_0;

    pstAeSnsDft->bAERouteExValid = HI_FALSE;
    pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
    pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;
    return;
}

static HI_VOID cmos_get_ae_linear_default(VI_PIPE vi_pipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft,
    ISP_SNS_STATE_S *pstSnsState)
{
    pstAeSnsDft->au8HistThresh[0] = 0xd;
    pstAeSnsDft->au8HistThresh[1] = 0x28;
    pstAeSnsDft->au8HistThresh[2] = 0x60; /* index 2 */
    pstAeSnsDft->au8HistThresh[3] = 0x80; /* index 3 */

    pstAeSnsDft->u32MaxAgain = 22924; /* max 22924 */
    pstAeSnsDft->u32MinAgain = 1024; /* min 1024 */
    pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
    pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

    pstAeSnsDft->u32MaxDgain = 128914; /* max 128914 */
    pstAeSnsDft->u32MinDgain = 1024; /* min 1024 */
    pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
    pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

    pstAeSnsDft->u8AeCompensation = 0x38;
    pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;

    pstAeSnsDft->u32InitExposure = g_au32InitExposure[vi_pipe] ? g_au32InitExposure[vi_pipe] : 148859; /* init 148859 */

    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 2; /* sub 2 */
    pstAeSnsDft->u32MinIntTime = 3; /* min 3 */
    pstAeSnsDft->u32MaxIntTimeTarget = 65535; /* max 65535 */
    pstAeSnsDft->u32MinIntTimeTarget = 3; /* min 3 */
    pstAeSnsDft->bAERouteExValid = g_abAERouteExValid[vi_pipe];
    memcpy_s(&pstAeSnsDft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), &g_astInitAERoute[vi_pipe], sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&pstAeSnsDft->stAERouteAttrEx, sizeof(ISP_AE_ROUTE_EX_S),
        &g_astInitAERouteEx[vi_pipe],  sizeof(ISP_AE_ROUTE_EX_S));
    return;
}
static HI_VOID cmos_get_ae_2to1_line_wdr_default(VI_PIPE vi_pipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft,
    ISP_SNS_STATE_S *pstSnsState)
{
    pstAeSnsDft->au8HistThresh[0] = 0xC;
    pstAeSnsDft->au8HistThresh[1] = 0x18;
    pstAeSnsDft->au8HistThresh[2] = 0x60; /* index 2 */
    pstAeSnsDft->au8HistThresh[3] = 0x80; /* index 3 */

    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 2; /* sub 2 */
    pstAeSnsDft->u32MinIntTime = 4; /* min 4 */

    pstAeSnsDft->u32MaxIntTimeTarget = 65535; /* max 65535 */
    pstAeSnsDft->u32MinIntTimeTarget = pstAeSnsDft->u32MinIntTime;

    pstAeSnsDft->u32MaxAgain = 22924; /* max 22924 */
    pstAeSnsDft->u32MinAgain = 1024; /* min 1024 */
    pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
    pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

    pstAeSnsDft->u32MaxDgain = 128914; /* max 128914 */
    pstAeSnsDft->u32MinDgain = 1024; /* min 1024 */
    pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
    pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;
    pstAeSnsDft->u8AeCompensation = 40; /* AeCompensation 40 */

    pstAeSnsDft->u32InitExposure = g_au32InitExposure[vi_pipe] ? g_au32InitExposure[vi_pipe] : 16462; /* init 16462 */

    if (g_genFSWDRMode[vi_pipe] == ISP_FSWDR_LONG_FRAME_MODE) {
        pstAeSnsDft->u8AeCompensation = 56; /* AeCompensation 56 */
        pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;
    } else {
        pstAeSnsDft->u32MaxISPDgainTarget = 1024; /* max 1024 */
        pstAeSnsDft->enAeExpMode = AE_EXP_LOWLIGHT_PRIOR;
        pstAeSnsDft->u16ManRatioEnable = HI_TRUE;
        pstAeSnsDft->au32Ratio[0] = 0x100;
        pstAeSnsDft->au32Ratio[1] = 0x40;
        pstAeSnsDft->au32Ratio[2] = 0x40; /* index 2 */
    }
    pstAeSnsDft->bAERouteExValid = g_abAERouteExValid[vi_pipe];
    memcpy_s(&pstAeSnsDft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), &g_astInitAERoute[vi_pipe], sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&pstAeSnsDft->stAERouteAttrEx, sizeof(ISP_AE_ROUTE_EX_S),
        &g_astInitAERouteEx[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S));
    memcpy_s(&pstAeSnsDft->stAERouteSFAttr, sizeof(ISP_AE_ROUTE_S),
        &g_astInitAERouteSF[vi_pipe], sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&pstAeSnsDft->stAERouteSFAttrEx, sizeof(ISP_AE_ROUTE_EX_S), &g_astInitAERouteSFEx[vi_pipe],
        sizeof(ISP_AE_ROUTE_EX_S));
    return;
}

static HI_S32 cmos_get_ae_default(VI_PIPE vi_pipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstAeSnsDft);
    IMX307_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset_s(&pstAeSnsDft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), 0, sizeof(ISP_AE_ROUTE_S));

    cmos_get_ae_comm_default(vi_pipe, pstAeSnsDft, pstSnsState);
    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE: /* linear mode */

            cmos_get_ae_linear_default(vi_pipe, pstAeSnsDft, pstSnsState);
            break;

        case WDR_MODE_2To1_LINE:

            cmos_get_ae_2to1_line_wdr_default(vi_pipe, pstAeSnsDft, pstSnsState);

            break;
    }
    return HI_SUCCESS;
}
static HI_VOID cmos_config_vmax(ISP_SNS_STATE_S *pstSnsState, HI_U32 u32VMAX)
{
    if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
        pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = LOW_8BITS(u32VMAX); /* index 5 */
        pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = HIGH_8BITS(u32VMAX); /* index 6 */
        pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = HIGHER_4BITS(u32VMAX); /* index 7 */
    } else {
        pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = LOW_8BITS(u32VMAX); /* index 8 */
        pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = HIGH_8BITS(u32VMAX); /* index 9 */
        pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = HIGHER_4BITS(u32VMAX); /* index 10 */
    }

    return;
}

/* the function of sensor set fps */
static HI_VOID cmos_fps_set(VI_PIPE vi_pipe, HI_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    HI_U32 u32VMAX = IMX307_VMAX_1080P30_LINEAR;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    IMX307_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    switch (pstSnsState->u8ImgMode) {
        case IMX307_SENSOR_1080P_30FPS_2t1_WDR_MODE:
            if ((f32Fps <= 30) && (f32Fps >= 15.21)) { /* fps range [15.21, 30] */
                u32VMAX = IMX307_VMAX_1080P60TO30_WDR * 30 / SNS_DIV_0_TO_1_FLOAT(f32Fps); /* 30fps */
            } else {
                SNS_ERR_TRACE("Not support Fps: %f\n", f32Fps);
                return;
            }
            u32VMAX = (u32VMAX > IMX307_FULL_LINES_MAX_2TO1_WDR) ? IMX307_FULL_LINES_MAX_2TO1_WDR : u32VMAX;
            pstAeSnsDft->u32LinesPer500ms = IMX307_VMAX_1080P60TO30_WDR * 30; /* 30fps */
            break;

        case IMX307_SENSOR_1080P_30FPS_LINEAR_MODE:
            if ((f32Fps <= 30) && (f32Fps >= 0.119)) { /* fps range [0.119, 30] */
                u32VMAX = IMX307_VMAX_1080P30_LINEAR * 30 / SNS_DIV_0_TO_1_FLOAT(f32Fps); /* 30fps */
            } else {
                SNS_ERR_TRACE("Not support Fps: %f\n", f32Fps);
                return;
            }
            u32VMAX = (u32VMAX > IMX307_FULL_LINES_MAX) ? IMX307_FULL_LINES_MAX : u32VMAX;
            pstAeSnsDft->u32LinesPer500ms = IMX307_VMAX_1080P30_LINEAR * 15;  /* 15: 30fps / 2 */
            break;

        default:
            return;
    }

    cmos_config_vmax(pstSnsState, u32VMAX);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        pstSnsState->u32FLStd = u32VMAX * 2; /* x2 */

        g_astimx307State[vi_pipe].u32RHS1_MAX =
            (u32VMAX - g_astimx307State[vi_pipe].u32BRL) * 2 - 21; /* (VMAX - BRL) * 2 - 21 */
    } else {
        pstSnsState->u32FLStd = u32VMAX;
    }

    pstAeSnsDft->f32Fps = f32Fps;
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 2; /* sub 2 */
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];

    if (g_bFirstFps[vi_pipe]) {
        g_bFirstFps[vi_pipe] = HI_FALSE;
        pstSnsState->au32FL[1] = pstSnsState->au32FL[0];
    }
    return;
}
static HI_VOID cmos_slow_framerate_set(VI_PIPE vi_pipe, HI_U32 u32FullLines, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    IMX307_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        u32FullLines = (u32FullLines > 2 * IMX307_FULL_LINES_MAX_2TO1_WDR) ? /* 2 * Fl */
            2 * IMX307_FULL_LINES_MAX_2TO1_WDR : u32FullLines; /* 2 * Fl */
        pstSnsState->au32FL[0] = (u32FullLines >> 1) << 1;
        g_astimx307State[vi_pipe].u32RHS1_MAX = pstSnsState->au32FL[0] -
            g_astimx307State[vi_pipe].u32BRL * 2 - 21; /* FL - BRL * 2 - 21 */
    } else {
        u32FullLines = (u32FullLines > IMX307_FULL_LINES_MAX) ? IMX307_FULL_LINES_MAX : u32FullLines;
        pstSnsState->au32FL[0] = u32FullLines;
    }

    if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
        pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = LOW_8BITS(pstSnsState->au32FL[0]); /* index 5 */
        pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = HIGH_8BITS(pstSnsState->au32FL[0]); /* index 6 */
        pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = HIGHER_4BITS(pstSnsState->au32FL[0]); /* index 7 */
    } else if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = LOW_8BITS((pstSnsState->au32FL[0] >> 1)); /* index 8 */
        pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = HIGH_8BITS((pstSnsState->au32FL[0] >> 1)); /* index 9 */
        pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = HIGHER_4BITS((pstSnsState->au32FL[0] >> 1)); /* index 10 */
    } else {
    }

    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
    pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 2; /* MaxIntTime: Flstd - 2 */

    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static HI_VOID cmos_inttime_update(VI_PIPE vi_pipe, HI_U32 u32IntTime)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    HI_U32 u32Value = 0;
    static HI_BOOL bFirst[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = 1};
    static HI_U32 u32ShortIntTime[ISP_MAX_PIPE_NUM] = {0};
    static HI_U32 u32LongIntTime[ISP_MAX_PIPE_NUM] = {0};
    static HI_U32 u32RHS1[ISP_MAX_PIPE_NUM] = {0};
    static HI_U32 u32SHS1[ISP_MAX_PIPE_NUM] = {0};
    static HI_U32 u32SHS2[ISP_MAX_PIPE_NUM] = {0};

    HI_U32 u32YOUTSIZE;

    IMX307_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        if (bFirst[vi_pipe]) { /* short exposure */
            pstSnsState->au32WDRIntTime[0] = u32IntTime;
            u32ShortIntTime[vi_pipe] = u32IntTime;
            bFirst[vi_pipe] = HI_FALSE;
        } else { /* long exposure */
            pstSnsState->au32WDRIntTime[1] = u32IntTime;
            u32LongIntTime[vi_pipe] = u32IntTime;

            u32SHS2[vi_pipe] = pstSnsState->au32FL[1] - u32LongIntTime[vi_pipe] - 1;

            /* allocate the RHS1 */
            u32SHS1[vi_pipe] = (u32ShortIntTime[vi_pipe] % 2) + 2; /* (IntTime % 2) + 2  */
            u32RHS1[vi_pipe] = u32ShortIntTime[vi_pipe] + u32SHS1[vi_pipe] + 1;

            u32YOUTSIZE = (1097 + (u32RHS1[vi_pipe] - 1) / 2 + 7) * 2; /* (1097 + (RHS1 - 1) / 2 + 7) * 2 */
            u32YOUTSIZE = (u32YOUTSIZE >= 0x1FFF) ? 0x1FFF : u32YOUTSIZE;

            pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = LOW_8BITS(u32SHS1[vi_pipe]);
            pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = HIGH_8BITS(u32SHS1[vi_pipe]);
            pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = HIGHER_4BITS(u32SHS1[vi_pipe]); /* index 2 */

            pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = LOW_8BITS(u32SHS2[vi_pipe]); /* index 5 */
            pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = HIGH_8BITS(u32SHS2[vi_pipe]); /* index 6 */
            pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = HIGHER_4BITS(u32SHS2[vi_pipe]); /* index 7 */

            pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = LOW_8BITS(u32RHS1[vi_pipe]); /* index 11 */
            pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = HIGH_8BITS(u32RHS1[vi_pipe]); /* index 12 */
            pstSnsState->astRegsInfo[0].astI2cData[13].u32Data = HIGHER_4BITS(u32RHS1[vi_pipe]); /* index 13 */

            pstSnsState->astRegsInfo[0].astI2cData[14].u32Data = (u32YOUTSIZE & 0xFF); /* index 14 */
            pstSnsState->astRegsInfo[0].astI2cData[15].u32Data = ((u32YOUTSIZE & 0x1F00) >> 8); /* index 15,  shift 8 */

            bFirst[vi_pipe] = HI_TRUE;
        }
    } else {
        u32Value = pstSnsState->au32FL[0] - u32IntTime - 1;

        pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = LOW_8BITS(u32Value);
        pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = HIGH_8BITS(u32Value);
        pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = HIGHER_4BITS(u32Value); /* index 2 */
        bFirst[vi_pipe] = HI_TRUE;
    }

    return;
}
#define GAIN_NODE_NUM    241
#define AGAIN_NODE_NUM   91
#define DGAIN_NODE_NUM   141
static HI_U32 g_gain_table[GAIN_NODE_NUM] = {
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

static HI_VOID cmos_again_calc_table(VI_PIPE vi_pipe, HI_U32 *pu32AgainLin, HI_U32 *pu32AgainDb)
{
    int i;

    CMOS_CHECK_POINTER_VOID(pu32AgainLin);
    CMOS_CHECK_POINTER_VOID(pu32AgainDb);

    if (*pu32AgainLin >= g_gain_table[AGAIN_NODE_NUM - 1]) {
        *pu32AgainLin = g_gain_table[AGAIN_NODE_NUM - 1];
        *pu32AgainDb = AGAIN_NODE_NUM - 1;
        return;
    }

    for (i = 1; i < AGAIN_NODE_NUM; i++) {
        if (*pu32AgainLin < g_gain_table[i]) {
            *pu32AgainLin = g_gain_table[i - 1];
            *pu32AgainDb = i - 1;
            break;
        }
    }

    return;
}
static HI_VOID cmos_dgain_calc_table(VI_PIPE vi_pipe, HI_U32 *pu32DgainLin, HI_U32 *pu32DgainDb)
{
    int i;

    CMOS_CHECK_POINTER_VOID(pu32DgainLin);
    CMOS_CHECK_POINTER_VOID(pu32DgainDb);

    if (*pu32DgainLin >= g_gain_table[DGAIN_NODE_NUM - 1]) {
        *pu32DgainLin = g_gain_table[DGAIN_NODE_NUM - 1];
        *pu32DgainDb = DGAIN_NODE_NUM - 1;
        return;
    }

    for (i = 1; i < DGAIN_NODE_NUM; i++) {
        if (*pu32DgainLin < g_gain_table[i]) {
            *pu32DgainLin = g_gain_table[i - 1];
            *pu32DgainDb = i - 1;
            break;
        }
    }

    return;
}
static HI_VOID cmos_gains_update(VI_PIPE vi_pipe, HI_U32 u32Again, HI_U32 u32Dgain)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    HI_U32 u32HCG = g_astimx307State[vi_pipe].u8Hcg;
    HI_U32 u32Tmp;

    IMX307_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (u32Again >= 21) { /* HCG 21 */
        u32HCG = u32HCG | 0x10;  /* bit[4] HCG  .Reg0x3009[7:0] */
        u32Again = u32Again - 21; /* u32Again sub 21 */
    }

    u32Tmp = u32Again + u32Dgain;
    pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = LOW_8BITS(u32Tmp); /* index 3 */
    pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = LOW_8BITS(u32HCG); /* index 4 */

    return;
}
static HI_VOID cmos_get_inttime_max_2to1_line(VI_PIPE vi_pipe,
    HI_U32 *au32Ratio, HI_U32 *au32IntTimeMax,
    HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    HI_U32 u32ShortMax0 = 0;
    HI_U32 u32ShortMax  = 0;
    HI_U32 u32ShortTimeMinLimit;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    IMX307_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32ShortTimeMinLimit = 4; /* u32ShortTimeMinLimit 4 */

    if (g_genFSWDRMode[vi_pipe] == ISP_FSWDR_LONG_FRAME_MODE) {
        u32ShortMax0 = pstSnsState->au32FL[1] - 6 - pstSnsState->au32WDRIntTime[0]; /* sensor limit: sub 6 */
        u32ShortMax = pstSnsState->au32FL[0] - 10; /* sensor limit: sub 10 */
        u32ShortMax = (u32ShortMax0 < u32ShortMax) ? u32ShortMax0 : u32ShortMax;
        au32IntTimeMax[1] = u32ShortMax;
        au32IntTimeMin[1] = u32ShortTimeMinLimit;
        return;
    } else {
        u32ShortMax0 = ((pstSnsState->au32FL[1] - 6 - pstSnsState->au32WDRIntTime[0]) * 0x40) / /* sub 6 */
            SNS_DIV_0_TO_1(au32Ratio[0]);
        u32ShortMax = ((pstSnsState->au32FL[0] - 6) * 0x40) / SNS_DIV_0_TO_1(au32Ratio[0] + 0x40); /* sub 6 */
        u32ShortMax = (u32ShortMax0 < u32ShortMax) ? u32ShortMax0 : u32ShortMax;
        u32ShortMax = (u32ShortMax > (g_astimx307State[vi_pipe].u32RHS1_MAX - 3)) /* sub 3 */ ?
            (g_astimx307State[vi_pipe].u32RHS1_MAX - 3) : u32ShortMax; /* sub 3 */
        u32ShortMax = (u32ShortMax == 0) ? 1 : u32ShortMax;
    }

    if (u32ShortMax >= u32ShortTimeMinLimit) {
        au32IntTimeMax[0] = u32ShortMax;
        au32IntTimeMax[1] = (au32IntTimeMax[0] * au32Ratio[0]) >> 6; /* shift 6 */
        au32IntTimeMin[0] = u32ShortTimeMinLimit;
        au32IntTimeMin[1] = (au32IntTimeMin[0] * au32Ratio[0]) >> 6; /* shift 6 */
    } else {
        u32ShortMax = u32ShortTimeMinLimit;

        au32IntTimeMax[0] = u32ShortMax;
        au32IntTimeMax[1] = (au32IntTimeMax[0] * 0xFFF) >> 6; /* shift 6 */
        au32IntTimeMin[0] = au32IntTimeMax[0];
        au32IntTimeMin[1] = au32IntTimeMax[1];
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
    IMX307_SENSOR_GET_CTX(vi_pipe, pstSnsState);
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

/* Only used in LINE_WDR mode */
static HI_VOID cmos_ae_fswdr_attr_set(VI_PIPE vi_pipe, AE_FSWDR_ATTR_S *pstAeFSWDRAttr)
{
    CMOS_CHECK_POINTER_VOID(pstAeFSWDRAttr);

    g_genFSWDRMode[vi_pipe] = pstAeFSWDRAttr->enFSWDRMode;
    g_au32MaxTimeGetCnt[vi_pipe] = 0;
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

/* awb static param */
#define CALIBRATE_STATIC_TEMP                         4884
#define CALIBRATE_STATIC_WB_R_GAIN                    0x1DF
#define CALIBRATE_STATIC_WB_GR_GAIN                   0x100
#define CALIBRATE_STATIC_WB_GB_GAIN                   0x100
#define CALIBRATE_STATIC_WB_B_GAIN                    0x1DC

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1                              (-39)
#define CALIBRATE_AWB_P2                              295
#define CALIBRATE_AWB_Q1                              0
#define CALIBRATE_AWB_A1                              157585
#define CALIBRATE_AWB_B1                              128
#define CALIBRATE_AWB_C1                              (-105655)

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0
static HI_S32 cmos_get_awb_default(VI_PIPE vi_pipe, AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    HI_S32 i;

    CMOS_CHECK_POINTER(pstAwbSnsDft);
    IMX307_SENSOR_GET_CTX(vi_pipe, pstSnsState);
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
    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        pstDef->stWdrSwitchAttr.au32ExpRatio[0] = 0x100;
    }

    memcpy_s(&pstDef->stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S), &g_stDngColorParam,
             sizeof(ISP_CMOS_DNG_COLORPARAM_S));

    switch (pstSnsState->u8ImgMode) {
        default:
        case IMX307_SENSOR_1080P_30FPS_LINEAR_MODE:
            pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 12; /* 12bit */
            pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 4095; /* max 4095 */
            break;

        case IMX307_SENSOR_1080P_30FPS_2t1_WDR_MODE:
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
    pstDef->unKey.bit1BayerNr        = 1;
    pstDef->pstBayerNr               = &g_stIspBayerNr;
    pstDef->unKey.bit1AntiFalseColor = 1;
    pstDef->pstAntiFalseColor = &g_stIspAntiFalseColor;
    pstDef->unKey.bit1Ldci = 1;
    pstDef->pstLdci = &g_stIspLdci;
    pstDef->unKey.bit1Gamma = 1;
    pstDef->pstGamma = &g_stIspGamma;
    pstDef->unKey.bit1Detail = 1;
    pstDef->pstDetail = &g_stIspDetail;
    pstDef->unKey.bit1Lcac = 1;
    pstDef->pstLcac = &g_stIspLCac;
#ifdef CONFIG_HI_ISP_CR_SUPPORT
    pstDef->unKey.bit1Ge = 1;
    pstDef->pstGe = &g_stIspGe;
#endif
    pstDef->unKey.bit1Dehaze = 1;
    pstDef->pstDehaze = &g_stIspDehaze;
    memcpy_s(&pstDef->stNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S),
        &g_stIspNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
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
    pstDef->pstAntiFalseColor = &g_stIspWdrAntiFalseColor;
    pstDef->unKey.bit1Ldci = 1;
    pstDef->pstLdci = &g_stIspWdrLdci;
    pstDef->unKey.bit1Dehaze = 1;
    pstDef->pstDehaze = &g_stIspDehazeWDR;
    pstDef->unKey.bit1Lcac = 1;
    pstDef->pstLcac = &g_stIspLCacWdr;
    memcpy_s(&pstDef->stNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S), &g_stIspNoiseCalibration,
        sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
    return;
}

static HI_S32 cmos_get_isp_default(VI_PIPE vi_pipe, ISP_CMOS_DEFAULT_S *pstDef)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstDef);
    IMX307_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset_s(pstDef, sizeof(ISP_CMOS_DEFAULT_S), 0, sizeof(ISP_CMOS_DEFAULT_S));

#ifdef CONFIG_HI_ISP_CA_SUPPORT
    pstDef->unKey.bit1Ca      = 1;
    pstDef->pstCa             = &g_stIspCA;
#endif
    pstDef->unKey.bit1Clut    = 1;
    pstDef->pstClut           = &g_stIspCLUT;

    pstDef->unKey.bit1Dpc     = 1;
    pstDef->pstDpc            = &g_stCmosDpc;

    pstDef->unKey.bit1Wdr     = 1;
    pstDef->pstWdr            = &g_stIspWDR;

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
#ifdef CONFIG_HI_ISP_PREGAMMA_SUPPORT
    pstDef->unKey.bit1PreGamma = 0;
    pstDef->pstPreGamma        = &g_stPreGamma;
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

    pstDef->stSensorMode.u32SensorID = IMX307_ID;
    pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;
    cmos_get_isp_dng_default(pstSnsState, pstDef);

    return HI_SUCCESS;
}
static HI_S32 cmos_get_isp_black_level(VI_PIPE vi_pipe, ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
    HI_S32 i;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstBlackLevel);
    IMX307_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    /* Don't need to update black level when iso change */
    pstBlackLevel->bUpdate = HI_FALSE;

    /* black level of linear mode */
    if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            pstBlackLevel->au16BlackLevel[i] = 0xF0;
        }
    } else {
        /* black level of DOL mode */
        pstBlackLevel->au16BlackLevel[0] = 0xF0;
        pstBlackLevel->au16BlackLevel[1] = 0xF0;
        pstBlackLevel->au16BlackLevel[2] = 0xF0; /* index 2 */
        pstBlackLevel->au16BlackLevel[3] = 0xF0; /* index 3 */
    }

    return HI_SUCCESS;
}
static HI_VOID cmos_set_pixel_detect(VI_PIPE vi_pipe, HI_BOOL bEnable)
{
    HI_U32 u32FullLines_5Fps, u32MaxIntTime_5Fps;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    IMX307_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        return;
    } else {
        if (pstSnsState->u8ImgMode == IMX307_SENSOR_1080P_30FPS_LINEAR_MODE) {
            u32FullLines_5Fps = (IMX307_VMAX_1080P30_LINEAR * 30) / 5; /* 30fps, 5fps */
        } else {
            return;
        }
    }

    u32MaxIntTime_5Fps = 4; /* u32MaxIntTime_5Fps 4 */

    if (bEnable) { /* setup for ISP pixel calibration mode */
        imx307_write_register(vi_pipe, IMX307_GAIN_ADDR, 0x00);

        imx307_write_register(vi_pipe, IMX307_VMAX_ADDR, LOW_8BITS(u32FullLines_5Fps));
        imx307_write_register(vi_pipe, IMX307_VMAX_ADDR + 1, HIGH_8BITS(u32FullLines_5Fps));
        imx307_write_register(vi_pipe, IMX307_VMAX_ADDR + 2, HIGHER_4BITS(u32FullLines_5Fps)); /* index 2 */

        imx307_write_register(vi_pipe, IMX307_SHS1_ADDR, LOW_8BITS(u32MaxIntTime_5Fps));
        imx307_write_register(vi_pipe, IMX307_SHS1_ADDR + 1, HIGH_8BITS(u32MaxIntTime_5Fps));
        imx307_write_register(vi_pipe, IMX307_SHS1_ADDR + 2, HIGHER_4BITS(u32MaxIntTime_5Fps)); /* index 2 */
    } else { /* setup for ISP 'normal mode' */
        pstSnsState->u32FLStd = (pstSnsState->u32FLStd > 0x1FFFF) ? 0x1FFFF : pstSnsState->u32FLStd;
        imx307_write_register(vi_pipe, IMX307_VMAX_ADDR, LOW_8BITS(pstSnsState->u32FLStd));
        imx307_write_register(vi_pipe, IMX307_VMAX_ADDR + 1, HIGH_8BITS(pstSnsState->u32FLStd));
        imx307_write_register(vi_pipe, IMX307_VMAX_ADDR + 2, HIGHER_4BITS(pstSnsState->u32FLStd)); /* index 2 */
        pstSnsState->bSyncInit = HI_FALSE;
    }

    return;
}

static HI_S32 cmos_set_wdr_mode(VI_PIPE vi_pipe, HI_U8 u8Mode)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    IMX307_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    pstSnsState->bSyncInit = HI_FALSE;

    switch (u8Mode & 0x3F) {
        case WDR_MODE_NONE:
            pstSnsState->enWDRMode = WDR_MODE_NONE;
            printf("linear mode\n");
            break;

        case WDR_MODE_2To1_LINE:
            pstSnsState->enWDRMode = WDR_MODE_2To1_LINE;
            printf("2to1 line WDR 1080p mode(60fps->30fps)\n");
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
    pstSnsState->astRegsInfo[0].unComBus.s8I2cDev = g_aunImx307BusInfo[vi_pipe].s8I2cDev;
    pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2; /* DelayMax 2 */
    pstSnsState->astRegsInfo[0].u32RegNum = 8; /* RegNum 8 */

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        pstSnsState->astRegsInfo[0].u32RegNum += 8; /* RegNum add 8 */
        pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2; /* DelayMax 2 */
    }

    for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
        pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
        pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr = IMX307_I2C_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[i].u32AddrByteNum = IMX307_ADDR_BYTE;
        pstSnsState->astRegsInfo[0].astI2cData[i].u32DataByteNum = IMX307_DATA_BYTE;
    }

    /* Linear Mode Regs */
    pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
    pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr = IMX307_SHS1_ADDR;
    pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
    pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr = IMX307_SHS1_ADDR + 1;
    pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0; /* index 2 */
    pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr = IMX307_SHS1_ADDR + 2; /* index 2, index 2 */

    pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 0; /* index 3 */
    /* time */
    pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr = IMX307_GAIN_ADDR; /* index 3 */
    pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 1; /* index 4 */
    pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr = IMX307_HCG_ADDR; /* index 4 */

    pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0; /* index 5 */
    pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr = IMX307_VMAX_ADDR; /* index 5 */
    pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0; /* index 6 */
    pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr = IMX307_VMAX_ADDR + 1; /* index 6 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 0; /* index 7 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr = IMX307_VMAX_ADDR + 2; /* index 7, index 2 */
    return;
}
static HI_VOID cmos_2to1_line_wdr_sns_reg_info_init(VI_PIPE vi_pipe, ISP_SNS_STATE_S *pstSnsState)
{
    pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
    pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
    pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0; /* index 2 */

    pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0; /* index 5 */
    pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr = IMX307_SHS2_ADDR; /* index 5 */
    pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0; /* index 6 */
    pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr = IMX307_SHS2_ADDR + 1; /* index 6 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 0; /* index 7 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr = IMX307_SHS2_ADDR + 2; /* index 7, index 2 */

    pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 1; /* index 8 */
    pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr = IMX307_VMAX_ADDR; /* index 8 */
    pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum = 1; /* index 9 */
    pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr = IMX307_VMAX_ADDR + 1; /* index 9 */
    pstSnsState->astRegsInfo[0].astI2cData[10].u8DelayFrmNum = 1; /* index 10 */
    pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr = IMX307_VMAX_ADDR + 2; /* index 10, index 2 */

    pstSnsState->astRegsInfo[0].astI2cData[11].u8DelayFrmNum = 1; /* index 11 */
    pstSnsState->astRegsInfo[0].astI2cData[11].u32RegAddr = IMX307_RHS1_ADDR; /* index 11 */
    pstSnsState->astRegsInfo[0].astI2cData[12].u8DelayFrmNum = 1; /* index 12 */
    pstSnsState->astRegsInfo[0].astI2cData[12].u32RegAddr = IMX307_RHS1_ADDR + 1; /* index 12 */
    pstSnsState->astRegsInfo[0].astI2cData[13].u8DelayFrmNum = 1; /* index 13 */
    pstSnsState->astRegsInfo[0].astI2cData[13].u32RegAddr = IMX307_RHS1_ADDR + 2; /* index 13, index 2 */

    pstSnsState->astRegsInfo[0].astI2cData[14].u8DelayFrmNum = 1; /* index 14 */
    pstSnsState->astRegsInfo[0].astI2cData[14].u32RegAddr = IMX307_Y_OUT_SIZE_ADDR; /* index 14 */
    pstSnsState->astRegsInfo[0].astI2cData[15].u8DelayFrmNum = 1; /* index 15 */
    pstSnsState->astRegsInfo[0].astI2cData[15].u32RegAddr = IMX307_Y_OUT_SIZE_ADDR + 1; /* index 15 */
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
    IMX307_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    if ((pstSnsState->bSyncInit == HI_FALSE) || (pstSnsRegsInfo->bConfig == HI_FALSE)) {
        cmos_comm_sns_reg_info_init(vi_pipe, pstSnsState);

        /* DOL 2t1 Mode Regs */
        if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
            cmos_2to1_line_wdr_sns_reg_info_init(vi_pipe, pstSnsState);
        }

        pstSnsState->bSyncInit = HI_TRUE;
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
    HI_U8 u8SensorImageMode;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstSensorImageMode);
    IMX307_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    u8SensorImageMode = pstSnsState->u8ImgMode;
    pstSnsState->bSyncInit = HI_FALSE;

    if (pstSensorImageMode->f32Fps <= 30) { /* 30fps */
        switch (pstSnsState->enWDRMode) {
            case WDR_MODE_NONE:
                if (IMX307_RES_IS_1080P(pstSensorImageMode->u16Width, pstSensorImageMode->u16Height)) {
                    u8SensorImageMode = IMX307_SENSOR_1080P_30FPS_LINEAR_MODE;
                    pstSnsState->u32FLStd = IMX307_VMAX_1080P30_LINEAR;
                    g_astimx307State[vi_pipe].u8Hcg = 0x2;
                    break;
                }
                IMX307_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState);
                return HI_FAILURE;
            case WDR_MODE_2To1_LINE:
                if (IMX307_RES_IS_1080P(pstSensorImageMode->u16Width, pstSensorImageMode->u16Height)) {
                    u8SensorImageMode = IMX307_SENSOR_1080P_30FPS_2t1_WDR_MODE;
                    g_astimx307State[vi_pipe].u32BRL = 1109; /* u32BRL 1109 */
                    g_astimx307State[vi_pipe].u8Hcg = 0x1;
                    pstSnsState->u32FLStd = IMX307_VMAX_1080P60TO30_WDR * 2; /* x2 */
                    break;
                }
                IMX307_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState);
                return HI_FAILURE;
            default:
                IMX307_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState);
                return HI_FAILURE;
        }
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

    IMX307_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->bInit = HI_FALSE;
    pstSnsState->bSyncInit = HI_FALSE;
    pstSnsState->u8ImgMode = IMX307_SENSOR_1080P_30FPS_LINEAR_MODE;
    pstSnsState->enWDRMode = WDR_MODE_NONE;
    pstSnsState->u32FLStd = IMX307_VMAX_1080P30_LINEAR;
    pstSnsState->au32FL[0] = IMX307_VMAX_1080P60TO30_WDR;
    pstSnsState->au32FL[1] = IMX307_VMAX_1080P60TO30_WDR;

    memset_s(&pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset_s(&pstSnsState->astRegsInfo[1], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
    return;
}

static HI_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    CMOS_CHECK_POINTER(pstSensorExpFunc);

    memset_s(pstSensorExpFunc, sizeof(ISP_SENSOR_EXP_FUNC_S), 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init = imx307_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit = imx307_exit;
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
static HI_S32 imx307_set_bus_info(VI_PIPE vi_pipe, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunImx307BusInfo[vi_pipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

    return HI_SUCCESS;
}

static HI_S32 sensor_ctx_init(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    IMX307_SENSOR_GET_CTX(vi_pipe, pastSnsStateCtx);

    if (pastSnsStateCtx == HI_NULL) {
        pastSnsStateCtx = (ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
        if (pastSnsStateCtx == HI_NULL) {
            SNS_ERR_TRACE("Isp[%d] SnsCtx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset_s(pastSnsStateCtx, sizeof(ISP_SNS_STATE_S), 0, sizeof(ISP_SNS_STATE_S));

    IMX307_SENSOR_SET_CTX(vi_pipe, pastSnsStateCtx);

    return HI_SUCCESS;
}

static HI_VOID sensor_ctx_exit(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;
    g_bFirstFps[vi_pipe] = HI_TRUE;
    IMX307_SENSOR_GET_CTX(vi_pipe, pastSnsStateCtx);
    SENSOR_FREE(pastSnsStateCtx);
    IMX307_SENSOR_RESET_CTX(vi_pipe);
    return;
}

static HI_S32 sensor_register_callback(VI_PIPE vi_pipe, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib)
{
    HI_S32 s32Ret;
    ISP_SENSOR_REGISTER_S stIspRegister;
    AE_SENSOR_REGISTER_S stAeRegister;
    AWB_SENSOR_REGISTER_S stAwbRegister;
    ISP_SNS_ATTR_INFO_S   stSnsAttrInfo;

    CMOS_CHECK_POINTER(pstAeLib);
    CMOS_CHECK_POINTER(pstAwbLib);

    s32Ret = sensor_ctx_init(vi_pipe);
    if (s32Ret != HI_SUCCESS) {
        return HI_FAILURE;
    }

    stSnsAttrInfo.eSensorId = IMX307_ID;
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

    s32Ret = HI_MPI_ISP_SensorUnRegCallBack(vi_pipe, IMX307_ID);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AE_SensorUnRegCallBack(vi_pipe, pstAeLib, IMX307_ID);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AWB_SensorUnRegCallBack(vi_pipe, pstAwbLib, IMX307_ID);
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
    g_abAERouteExValid[vi_pipe] = pstInitAttr->bAERouteExValid;
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

ISP_SNS_OBJ_S stSnsImx307Obj = {
    .pfnRegisterCallback    = sensor_register_callback,
    .pfnUnRegisterCallback  = sensor_unregister_callback,
    .pfnStandby             = imx307_standby,
    .pfnRestart             = imx307_restart,
    .pfnMirrorFlip          = HI_NULL,
    .pfnWriteReg            = imx307_write_register,
    .pfnReadReg             = imx307_read_register,
    .pfnSetBusInfo          = imx307_set_bus_info,
    .pfnSetInit             = sensor_set_init
};

