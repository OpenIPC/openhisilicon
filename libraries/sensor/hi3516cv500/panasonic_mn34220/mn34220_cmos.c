/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of mn34220_cmos.c
 * Author: ISP SW
 * Create: 2012/06/28
 */
#include "mn34220_cmos.h"
#include "mn34220_cmos_ex.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "mpi_isp.h"
#include "mpi_ae.h"
#include "mpi_awb.h"

#define MN34220_ID    34220

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/
ISP_SNS_STATE_S    *g_astMn34220[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define MN34220_SENSOR_SET_CTX(dev, pstCtx)   ((g_astMn34220[dev]) = (pstCtx))
#define MN34220_SENSOR_RESET_CTX(dev)         (g_astMn34220[dev] = HI_NULL)

static HI_BOOL g_abAERouteExValid[ISP_MAX_PIPE_NUM] = {0};
static ISP_AE_ROUTE_S g_astInitAERoute[ISP_MAX_PIPE_NUM] = {{0}};
static ISP_AE_ROUTE_EX_S g_astInitAERouteEx[ISP_MAX_PIPE_NUM] = {{0}};
static ISP_AE_ROUTE_S g_astInitAERouteSF[ISP_MAX_PIPE_NUM] = {{0}};
static ISP_AE_ROUTE_EX_S g_astInitAERouteSFEx[ISP_MAX_PIPE_NUM] = {{0}};
static HI_BOOL g_quick_start_en[ISP_MAX_PIPE_NUM] = {HI_FALSE};

ISP_SNS_COMMBUS_U g_aunMn34220BusInfo[ISP_MAX_PIPE_NUM] = {
    [0] = { .s8I2cDev = 0},
    [1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1}
};

ISP_SNS_COMMBUS_U *mn34220_get_bus_Info(VI_PIPE vi_pipe)
{
    return &g_aunMn34220BusInfo[vi_pipe];
}

ISP_SNS_STATE_S *mn34220_get_ctx(VI_PIPE vi_pipe)
{
    return g_astMn34220[vi_pipe];
}

static ISP_FSWDR_MODE_E g_genFSWDRMode[ISP_MAX_PIPE_NUM] = {
    [0 ... ISP_MAX_PIPE_NUM - 1] = ISP_FSWDR_NORMAL_MODE
};

static HI_U32   g_au32MaxTimeGetCnt[ISP_MAX_PIPE_NUM] = {0};

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
#define MN34220_FULL_LINES_MAX              0xFFFF

#define MN34220_EXPOSURE_ADDR_H             0x0202
#define MN34220_EXPOSURE_ADDR_L             0x0203
#define MN34220_LONG_EXPOSURE_ADDR_H        0x312A
#define MN34220_LONG_EXPOSURE_ADDR_L        0x312B
#define MN34220_SHORT_EXPOSURE_ADDR_H       0x312C
#define MN34220_SHORT_EXPOSURE_ADDR_L       0x312D
#define MN34220_SHORT_SHORT_EXPOSURE_ADDR_H 0x312E
#define MN34220_SHORT_SHORT_EXPOSURE_ADDR_L 0x312F
#define MN34220_AGC_ADDR_H                  0x0204
#define MN34220_AGC_ADDR_L                  0x0205
#define MN34220_DGC_ADDR_H                  0x3108
#define MN34220_DGC_ADDR_L                  0x3109
#define MN34220_VMAX_ADDR_H                 0x0340
#define MN34220_VMAX_ADDR_L                 0x0341

#define MN34220_INCREASE_LINES              1 /* make real fps less than stand fps because NVR require */
#define MN34220_VMAX_1080P30_LINEAR         (1125 + MN34220_INCREASE_LINES)
#define MN34220_VMAX_1080P60_LINEAR         (1125 + MN34220_INCREASE_LINES)
#define MN34220_VMAX_1080P60TO30_WDR        (1250 + MN34220_INCREASE_LINES)
#define MN34220_VMAX_1080P50TO25_WDR        (1500 + MN34220_INCREASE_LINES)
#define MN34220_VMAX_1080P120TO30_WDR       (1125 + MN34220_INCREASE_LINES)
#define MN34220_VMAX_1080P90TO30_WDR        (1500 + MN34220_INCREASE_LINES)
#define MN34220_VMAX_720P_WDR               (750  + MN34220_INCREASE_LINES)
#define MN34220_VMAX_720P120_LINEAR         (750  + MN34220_INCREASE_LINES)
#define MN34220_VMAX_VGA240_LINEAR          (562  + MN34220_INCREASE_LINES)

#define MN34220_1080P_60FPS_MODE            1
#define MN34220_1080P_30FPS_MODE            2
#define MN34220_720P_120FPS_MODE            3
#define MN34220_VGA_240FPS_MODE             4
#define MN34220_720P_30FPS_MODE             5 /* for FPGA */

#define MN34220_RES_IS_VGA(w, h)        ((w) <= 640  && (h) <= 480)
#define MN34220_RES_IS_720P(w, h)       ((w) <= 1280 && (h) <= 720)
#define MN34220_RES_IS_1080P(w, h)      ((w) <= 1920 && (h) <= 1080)

static HI_VOID cmos_get_ae_comm_default(VI_PIPE vi_pipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft,
    ISP_SNS_STATE_S *pstSnsState)
{
    pstAeSnsDft->u32LinesPer500ms = pstSnsState->u32FLStd * 25 / 2; /* 25fps, div 2 */
    pstAeSnsDft->u32FullLinesStd  = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FlickerFreq   = 0;
    pstAeSnsDft->u32FullLinesMax  = MN34220_FULL_LINES_MAX;

    pstAeSnsDft->stIntTimeAccu.enAccuType  = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
    pstAeSnsDft->stIntTimeAccu.f32Offset   = 0.8045; /* offset 0.8045 */

    pstAeSnsDft->stAgainAccu.enAccuType  = AE_ACCURACY_TABLE;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 0.38; /* accuracy 0.38 */

    pstAeSnsDft->stDgainAccu.enAccuType  = AE_ACCURACY_TABLE;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 0.38; /* accuracy 0.38 */

    pstAeSnsDft->u32ISPDgainShift     = 8; /* accuracy 8 */
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 8 << pstAeSnsDft->u32ISPDgainShift; /* max 8 */

    pstAeSnsDft->u32InitExposure = 921600; /* init 921600 */

    if ((pstAeSnsDft->f32Fps == 50) || (pstAeSnsDft->f32Fps == 60)) { /* 50fps, 60fps */
        pstAeSnsDft->stIntTimeAccu.f32Offset = 0.6082; /* offset 0.6082 */
    } else if ((pstAeSnsDft->f32Fps == 25) || (pstAeSnsDft->f32Fps == 30)) { /* 25fps, 30fps */
        pstAeSnsDft->stIntTimeAccu.f32Offset = 0.8045; /* offset 0.8045 */
    } else {
    }

    memcpy_s(&pstAeSnsDft->stPirisAttr, sizeof(ISP_PIRIS_ATTR_S), &g_gstPirisAttr, sizeof(ISP_PIRIS_ATTR_S));
    pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_4;
    pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_5_6;

    pstAeSnsDft->bAERouteExValid = HI_FALSE;
    pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
    pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;
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

    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 2; /* sub 2 */
    pstAeSnsDft->u32MinIntTime = 2; /* min 2 */
    pstAeSnsDft->u32MaxIntTimeTarget = 65535; /* max 65535 */
    pstAeSnsDft->u32MinIntTimeTarget = 2; /* min 2 */

    pstAeSnsDft->u32MaxAgain = 32382; /* max 32382 */
    pstAeSnsDft->u32MinAgain = 1024; /* min 1024 */
    pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
    pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

    pstAeSnsDft->u32MaxDgain = 32382; /* max 32382 */
    pstAeSnsDft->u32MinDgain = 1024; /* min 1024 */
    pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
    pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;
    pstAeSnsDft->bAERouteExValid = g_abAERouteExValid[vi_pipe];
    memcpy_s(&pstAeSnsDft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), &g_astInitAERoute[vi_pipe], sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&pstAeSnsDft->stAERouteAttrEx, sizeof(ISP_AE_ROUTE_EX_S),
        &g_astInitAERouteEx[vi_pipe],  sizeof(ISP_AE_ROUTE_EX_S));
    return;
}
static HI_VOID cmos_get_ae_2to1_frame_wdr_default(VI_PIPE vi_pipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft,
    ISP_SNS_STATE_S *pstSnsState)
{
    pstAeSnsDft->au8HistThresh[0] = 0xC;
    pstAeSnsDft->au8HistThresh[1] = 0x18;
    pstAeSnsDft->au8HistThresh[2] = 0x60; /* index 2 */
    pstAeSnsDft->au8HistThresh[3] = 0x80; /* index 3 */

    pstAeSnsDft->u8AeCompensation = 0x18;
    pstAeSnsDft->enAeExpMode = AE_EXP_LOWLIGHT_PRIOR;

    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 2; /* sub 2 */
    pstAeSnsDft->u32MinIntTime = 2; /* max 2 */
    pstAeSnsDft->u32MaxIntTimeTarget = 65535; /* max 65535 */
    pstAeSnsDft->u32MinIntTimeTarget = 2; /* min 2 */

    pstAeSnsDft->u32MaxAgain = 32382; /* max 32382 */
    pstAeSnsDft->u32MinAgain = 1024; /* min 1024 */
    pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
    pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

    pstAeSnsDft->u32MaxDgain = 32382; /* max 32382 */
    pstAeSnsDft->u32MinDgain = 1024; /* min 1024 */
    pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
    pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

    pstAeSnsDft->u16ManRatioEnable = HI_TRUE;
    pstAeSnsDft->au32Ratio[0]      = 0x200;
    pstAeSnsDft->bAERouteExValid = g_abAERouteExValid[vi_pipe];
    memcpy_s(&pstAeSnsDft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), &g_astInitAERoute[vi_pipe],  sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&pstAeSnsDft->stAERouteAttrEx, sizeof(ISP_AE_ROUTE_EX_S),
        &g_astInitAERouteEx[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S));
    memcpy_s(&pstAeSnsDft->stAERouteSFAttr, sizeof(ISP_AE_ROUTE_S),
        &g_astInitAERouteSF[vi_pipe], sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&pstAeSnsDft->stAERouteSFAttrEx, sizeof(ISP_AE_ROUTE_EX_S),
        &g_astInitAERouteSFEx[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S));
    return;
}
static HI_VOID cmos_get_ae_2to1_line_wdr_default(VI_PIPE vi_pipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft,
    ISP_SNS_STATE_S *pstSnsState)
{
    pstAeSnsDft->u16ManRatioEnable = HI_FALSE;
    pstAeSnsDft->au32Ratio[0] = 0xc9;
    pstAeSnsDft->au8HistThresh[0] = 0xC;
    pstAeSnsDft->au8HistThresh[1] = 0x18;
    pstAeSnsDft->au8HistThresh[2] = 0x60; /* index 2 */
    pstAeSnsDft->au8HistThresh[3] = 0x80; /* index 3 */

    pstAeSnsDft->u8AeCompensation = 0x1C;
    pstAeSnsDft->enAeExpMode = AE_EXP_LOWLIGHT_PRIOR;

    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 2; /* sub 2 */
    pstAeSnsDft->u32MinIntTime = 2; /* min 2 */
    pstAeSnsDft->u32MaxIntTimeTarget = 65535; /* max 65535 */
    pstAeSnsDft->u32MinIntTimeTarget = 2; /* min 2 */

    pstAeSnsDft->u32MaxAgain = 32382; /* max 32382 */
    pstAeSnsDft->u32MinAgain = 1024; /* min 1024 */
    pstAeSnsDft->u32MaxAgainTarget = 8134; /* max 8134 */
    pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

    pstAeSnsDft->u32MaxDgain = 32382; /* max 32382 */
    pstAeSnsDft->u32MinDgain = 1024; /* min 1024 */
    pstAeSnsDft->u32MaxDgainTarget = 1024; /* min 1024 */
    pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

    if ((pstAeSnsDft->f32Fps == 50) || (pstAeSnsDft->f32Fps == 60)) { /* 50fps, 60fps */
        pstAeSnsDft->stIntTimeAccu.f32Offset = 0.2155; /* offset 0.2155 */
    } else if ((pstAeSnsDft->f32Fps == 25) || (pstAeSnsDft->f32Fps == 30)) { /* 25fps, 30fps */
        pstAeSnsDft->stIntTimeAccu.f32Offset = 0.2818; /* offset 0.2818 */
    } else {
    }

    if (g_genFSWDRMode[vi_pipe] == ISP_FSWDR_LONG_FRAME_MODE) {
        pstAeSnsDft->u32MaxAgainTarget = 32382; /* max 32382 */
        pstAeSnsDft->u32MaxDgainTarget = 32382; /* max 32382 */
        pstAeSnsDft->u32MaxISPDgainTarget = 8 << pstAeSnsDft->u32ISPDgainShift; /* max 8 */
        pstAeSnsDft->u8AeCompensation = 0x38;
        pstAeSnsDft->enAeExpMode = AE_EXP_LOWLIGHT_PRIOR;
    }

    pstAeSnsDft->bAERouteExValid = g_abAERouteExValid[vi_pipe];
    memcpy_s(&pstAeSnsDft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), &g_astInitAERoute[vi_pipe], sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&pstAeSnsDft->stAERouteAttrEx, sizeof(ISP_AE_ROUTE_EX_S),
        &g_astInitAERouteEx[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S));
    memcpy_s(&pstAeSnsDft->stAERouteSFAttr, sizeof(ISP_AE_ROUTE_S),
        &g_astInitAERouteSF[vi_pipe], sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&pstAeSnsDft->stAERouteSFAttrEx, sizeof(ISP_AE_ROUTE_EX_S),
        &g_astInitAERouteSFEx[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S));
    return;
}
static HI_VOID cmos_get_ae_3to1_line_wdr_default(VI_PIPE vi_pipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft,
    ISP_SNS_STATE_S *pstSnsState)
{
    pstAeSnsDft->au8HistThresh[0] = 0xC;
    pstAeSnsDft->au8HistThresh[1] = 0x18;
    pstAeSnsDft->au8HistThresh[2] = 0x60; /* index 2 */
    pstAeSnsDft->au8HistThresh[3] = 0x80; /* index 3 */

    pstAeSnsDft->u8AeCompensation = 0x1C;
    /* Manual ratio the AE compensation must be smaller than linear */
    pstAeSnsDft->enAeExpMode = AE_EXP_LOWLIGHT_PRIOR;

    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 2; /* sub 2 */
    pstAeSnsDft->u32MinIntTime = 2; /* min 2 */
    pstAeSnsDft->u32MaxIntTimeTarget = 65535; /* max 65535 */
    pstAeSnsDft->u32MinIntTimeTarget = 2; /* min 2 */

    pstAeSnsDft->u32MaxAgain = 32382; /* max 32382 */
    pstAeSnsDft->u32MinAgain = 1024; /* min 1024 */
    pstAeSnsDft->u32MaxAgainTarget = 32382; /* max 32382 */
    pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

    pstAeSnsDft->u32MaxDgain = 32382; /* max 32382 */
    pstAeSnsDft->u32MinDgain = 1024; /* min 1024 */
    pstAeSnsDft->u32MaxDgainTarget = 32382; /* max 32382 */
    pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

    pstAeSnsDft->bAERouteExValid = HI_FALSE;
    pstAeSnsDft->stIntTimeAccu.f32Offset = 0.1077; /* offset 0.1077 */
    pstAeSnsDft->u16ManRatioEnable = HI_FALSE;
    pstAeSnsDft->au32Ratio[0] = 0x200;
    pstAeSnsDft->au32Ratio[1] = 0x200;
    pstAeSnsDft->au32Ratio[2] = 0x40; /* index 2 */

    if (g_genFSWDRMode[vi_pipe] == ISP_FSWDR_LONG_FRAME_MODE) {
        pstAeSnsDft->u8AeCompensation = 0x38;
        pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;
        pstAeSnsDft->u32MaxAgainTarget = 32382; /* max 32382 */
        pstAeSnsDft->u32MaxDgainTarget = 32382; /* max 32382 */
        pstAeSnsDft->u32MaxISPDgainTarget = 8 << pstAeSnsDft->u32ISPDgainShift; /* max 8 */
    }

    pstAeSnsDft->bAERouteExValid = g_abAERouteExValid[vi_pipe];
    memcpy_s(&pstAeSnsDft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), &g_astInitAERoute[vi_pipe], sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&pstAeSnsDft->stAERouteAttrEx, sizeof(ISP_AE_ROUTE_EX_S),
        &g_astInitAERouteEx[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S));
    memcpy_s(&pstAeSnsDft->stAERouteSFAttr, sizeof(ISP_AE_ROUTE_S),
        &g_astInitAERouteSF[vi_pipe], sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&pstAeSnsDft->stAERouteSFAttrEx, sizeof(ISP_AE_ROUTE_EX_S),
        &g_astInitAERouteSFEx[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S));
    return;
}
static HI_VOID cmos_get_ae_4to1_line_wdr_default(VI_PIPE vi_pipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft,
    ISP_SNS_STATE_S *pstSnsState)
{
    pstAeSnsDft->au8HistThresh[0] = 0xC;
    pstAeSnsDft->au8HistThresh[1] = 0x18;
    pstAeSnsDft->au8HistThresh[2] = 0x60; /* index 2 */
    pstAeSnsDft->au8HistThresh[3] = 0x80; /* index 3 */

    pstAeSnsDft->u8AeCompensation = 0x40;

    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 2; /* sub 2 */
    pstAeSnsDft->u32MinIntTime = 2; /* sub 2 */
    pstAeSnsDft->u32MaxIntTimeTarget = 65535; /* max 65535 */
    pstAeSnsDft->u32MinIntTimeTarget = 2; /* min 2 */

    pstAeSnsDft->u32MaxAgain = 32382; /* max 32382 */
    pstAeSnsDft->u32MinAgain = 1024; /* min 1024 */
    pstAeSnsDft->u32MaxAgainTarget = 32382; /* max 32382 */
    pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

    pstAeSnsDft->u32MaxDgain = 32382; /* max 32382 */
    pstAeSnsDft->u32MinDgain = 1024; /* min 1024 */
    pstAeSnsDft->u32MaxDgainTarget = 32382; /* max 32382 */
    pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

    pstAeSnsDft->stIntTimeAccu.f32Offset = 0.1077; /* offset 0.1077 */

    if (g_genFSWDRMode[vi_pipe] == ISP_FSWDR_LONG_FRAME_MODE) {
        pstAeSnsDft->u8AeCompensation = 0x38;
        pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;
        pstAeSnsDft->u32MaxAgainTarget = 32382; /* max 32382 */
        pstAeSnsDft->u32MaxDgainTarget = 32382; /* max 32382 */
        pstAeSnsDft->u32MaxISPDgainTarget = 8 << pstAeSnsDft->u32ISPDgainShift; /* max 8 */
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
    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset_s(&pstAeSnsDft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), 0, sizeof(ISP_AE_ROUTE_S));
    cmos_get_ae_comm_default(vi_pipe, pstAeSnsDft, pstSnsState);

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:   /* linear mode */
            cmos_get_ae_linear_default(vi_pipe, pstAeSnsDft, pstSnsState);
            break;

        case WDR_MODE_2To1_FRAME:
        case WDR_MODE_2To1_FRAME_FULL_RATE: /* linear mode for ISP frame switching WDR */

            cmos_get_ae_2to1_frame_wdr_default(vi_pipe, pstAeSnsDft, pstSnsState);

            break;

        case WDR_MODE_2To1_LINE:
            cmos_get_ae_2to1_line_wdr_default(vi_pipe, pstAeSnsDft, pstSnsState);

            break;

        case WDR_MODE_3To1_LINE:
            cmos_get_ae_3to1_line_wdr_default(vi_pipe, pstAeSnsDft, pstSnsState);

            break;

        case WDR_MODE_4To1_LINE:
            cmos_get_ae_4to1_line_wdr_default(vi_pipe, pstAeSnsDft, pstSnsState);
            break;
    }

    return HI_SUCCESS;
}
static HI_S32 cmos_fps_set_2To1_LINE(VI_PIPE vi_pipe, HI_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstAeSnsDft);
    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    if (pstSnsState->u8ImgMode == MN34220_1080P_30FPS_MODE) {
        /* In WDR_MODE_2To1_LINE mode, 30fps means 60 frames(30 short and 30 long) combine to 30 frames per sencond */
        if ((f32Fps <= 30) && (f32Fps >= 1)) { /* fps range [1, 30] */
            pstSnsState->u32FLStd     = MN34220_VMAX_1080P60TO30_WDR * 30 / f32Fps; /* 30fps */
            pstAeSnsDft->stIntTimeAccu.f32Offset = 0.2818; /* offset 0.2818 */
        } else {
            SNS_ERR_TRACE("Not support Fps: %f\n", f32Fps);
            return HI_FAILURE;
        }
    } else {
#if 1
        /* for FPGA */
        if ((f32Fps <= 30) && (f32Fps >= 2)) { /* fps range [2, 30] */
            pstSnsState->u32FLStd     = MN34220_VMAX_720P_WDR * 30 / f32Fps; /* 30fps */
            pstAeSnsDft->stIntTimeAccu.f32Offset = 0.1077; /* offset 0.1077 */
        } else {
            SNS_ERR_TRACE("Not support Fps: %f\n", f32Fps);
            return HI_FAILURE;
        }
#endif
    }

    return HI_SUCCESS;
}
static HI_S32 cmos_fps_set_3To1_LINE(VI_PIPE vi_pipe, HI_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstAeSnsDft);
    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    if (pstSnsState->u8ImgMode == MN34220_1080P_30FPS_MODE) {
        if ((f32Fps <= 30) && (f32Fps >= 1)) { /* fps range [1, 30] */
            pstSnsState->u32FLStd     = MN34220_VMAX_1080P90TO30_WDR * 30 / f32Fps; /* 30fps */
            pstAeSnsDft->stIntTimeAccu.f32Offset = 0.1077; /* offset 0.1077 */
        } else {
            SNS_ERR_TRACE("Not support Fps: %f\n", f32Fps);
            return HI_FAILURE;
        }
    } else {
#if 1
        /* for FPGA */
        if ((f32Fps <= 20) && (f32Fps >= 2)) { /* fps range [2, 20] */
            pstSnsState->u32FLStd     = MN34220_VMAX_720P_WDR * 20 / f32Fps; /* 20fps */
            pstAeSnsDft->stIntTimeAccu.f32Offset = 0.1077; /* offset 0.1077 */
        } else {
            SNS_ERR_TRACE("Not support Fps: %f\n", f32Fps);
            return HI_FAILURE;
        }
#endif
    }

    return HI_SUCCESS;
}
static HI_S32 cmos_fps_set_4To1_LINE(VI_PIPE vi_pipe, HI_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstAeSnsDft);
    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    if (pstSnsState->u8ImgMode == MN34220_1080P_30FPS_MODE) {
        if ((f32Fps <= 30) && (f32Fps >= 1)) { /* fps range [1, 30] */
            pstSnsState->u32FLStd     = MN34220_VMAX_1080P120TO30_WDR * 30 / f32Fps; /* 30fps */
            pstAeSnsDft->stIntTimeAccu.f32Offset = 0.1077; /* offset 0.1077 */
        } else {
            SNS_ERR_TRACE("Not support Fps: %f\n", f32Fps);
            return HI_FAILURE;
        }
    } else {
#if 1
        /* for FPGA */
        if ((f32Fps <= 15) && (f32Fps >= 2)) { /* fps range [2, 15] */
            pstSnsState->u32FLStd     = MN34220_VMAX_720P_WDR * 15 / f32Fps; /* 15fps */
            pstAeSnsDft->stIntTimeAccu.f32Offset = 0.1077; /* offset 0.1077 */
        } else {
            SNS_ERR_TRACE("Not support Fps: %f\n", f32Fps);
            return HI_FAILURE;
        }

#endif
    }

    return HI_SUCCESS;
}

static HI_S32 cmos_fps_set_None_LINE(VI_PIPE vi_pipe, HI_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    CMOS_CHECK_POINTER(pstAeSnsDft);
    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);
    switch (pstSnsState->u8ImgMode) {
        case MN34220_1080P_30FPS_MODE:
            if ((f32Fps > 30) || (f32Fps < 1)) { /* fps range [1, 30] */
                break;
            }
            pstSnsState->u32FLStd = (MN34220_VMAX_1080P30_LINEAR * 30) / f32Fps; /* 30fps */
            pstAeSnsDft->stIntTimeAccu.f32Offset = 0.8045; /* offset 0.8045 */
            return HI_SUCCESS;
        case MN34220_1080P_60FPS_MODE:
            if ((f32Fps > 60) || (f32Fps < 1.5)) { /* fps range [1.5, 60] */
                break;
            }
            pstSnsState->u32FLStd = (MN34220_VMAX_1080P60_LINEAR * 60) / f32Fps; /* 60fps */
            pstAeSnsDft->stIntTimeAccu.f32Offset = 0.6082; /* offset 0.6082 */
            return HI_SUCCESS;
        case MN34220_720P_30FPS_MODE:
            if ((f32Fps > 30) || (f32Fps < 0.5)) { /* fps range [0.5, 30] */
                break;
            }
            pstSnsState->u32FLStd = (MN34220_VMAX_720P120_LINEAR * 30) / f32Fps; /* 30fps */
            pstAeSnsDft->stIntTimeAccu.f32Offset = 0;
            return HI_SUCCESS;
        case MN34220_720P_120FPS_MODE:
            if ((f32Fps > 120) || (f32Fps < 1.5)) { /* fps range [1.5, 120] */
                break;
            }
            pstSnsState->u32FLStd = (MN34220_VMAX_720P120_LINEAR * 120) / f32Fps; /* 120fps */
            pstAeSnsDft->stIntTimeAccu.f32Offset = 0.5982; /* offset 0.5982 */
            return HI_SUCCESS;
        case MN34220_VGA_240FPS_MODE:
            if ((f32Fps > 240) || (f32Fps < 2.5)) { /* fps range [2.5, 240] */
                break;
            }
            pstSnsState->u32FLStd = (MN34220_VMAX_VGA240_LINEAR * 240) / f32Fps; /* 240fps */
            pstAeSnsDft->stIntTimeAccu.f32Offset = 0.4309; /* offset 0.4309 */
            return HI_SUCCESS;
        default:
            SNS_ERR_TRACE("Not support! u8ImgMode:%d, f32Fps:%f\n", pstSnsState->u8ImgMode, f32Fps);
            return HI_FAILURE;
    }
    SNS_ERR_TRACE("Not support Fps: %f\n", f32Fps);
    return HI_FAILURE;
}
static HI_VOID cmos_config_vmax(ISP_SNS_STATE_S *pstSnsState, HI_U32 u32VMAX)
{
    switch (pstSnsState->enWDRMode) {
        case WDR_MODE_NONE:
            pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = ((u32VMAX & 0xFF00) >> 8); /* index 7, shift 8 */
            pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = (u32VMAX & 0xFF); /* index 8 */
            break;
        case WDR_MODE_2To1_LINE:     /* Replace  is_2to1_wdr_mode */
        case WDR_MODE_2To1_FRAME_FULL_RATE:
        case WDR_MODE_2To1_FRAME:
        case WDR_MODE_QUDRA:
            pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = ((u32VMAX & 0xFF00) >> 8); /* index 9, shift 8 */
            pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = (u32VMAX & 0xFF); /* index 10 */
            break;
        case WDR_MODE_3To1_LINE:
            pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = ((u32VMAX & 0xFF00) >> 8); /* index 11, shift 8 */
            pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = (u32VMAX & 0xFF); /* index 12 */
            break;
        case WDR_MODE_4To1_LINE:
            pstSnsState->astRegsInfo[0].astI2cData[13].u32Data = ((u32VMAX & 0xFF00) >> 8); /* index 13, shift 8 */
            pstSnsState->astRegsInfo[0].astI2cData[14].u32Data = (u32VMAX & 0xFF); /* index 14 */
            break;
        default:
            break;
    }

    return;
}

/* the function of sensor set fps */
static HI_VOID cmos_fps_set(VI_PIPE vi_pipe, HI_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    HI_S32 s32Ret;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);
    switch (pstSnsState->enWDRMode) {
        case WDR_MODE_2To1_LINE:
            s32Ret = cmos_fps_set_2To1_LINE(vi_pipe, f32Fps, pstAeSnsDft);
            break;
        case WDR_MODE_3To1_LINE:
            s32Ret = cmos_fps_set_3To1_LINE(vi_pipe, f32Fps, pstAeSnsDft);
            break;
        case WDR_MODE_4To1_LINE:
            s32Ret = cmos_fps_set_4To1_LINE(vi_pipe, f32Fps, pstAeSnsDft);
            break;
        default:
            s32Ret = cmos_fps_set_None_LINE(vi_pipe, f32Fps, pstAeSnsDft);
            break;
    }
    if (s32Ret != HI_SUCCESS) {
        return;
    }
    pstSnsState->u32FLStd = (pstSnsState->u32FLStd > MN34220_FULL_LINES_MAX) ?
        MN34220_FULL_LINES_MAX : pstSnsState->u32FLStd;

    cmos_config_vmax(pstSnsState, pstSnsState->u32FLStd);

    pstAeSnsDft->f32Fps               = f32Fps;
    pstAeSnsDft->u32LinesPer500ms     = pstSnsState->u32FLStd * f32Fps / 2; /* div 2 */
    pstAeSnsDft->u32MaxIntTime        = pstSnsState->u32FLStd - 2; /* sub 2 */
    pstAeSnsDft->u32FullLinesStd      = pstSnsState->u32FLStd;
    pstSnsState->au32FL[0]            = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FullLines         = pstSnsState->au32FL[0];
    return;
}
static HI_VOID cmos_slow_framerate_set(VI_PIPE vi_pipe, HI_U32 u32FullLines,
    AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32FullLines = (u32FullLines > MN34220_FULL_LINES_MAX) ? MN34220_FULL_LINES_MAX : u32FullLines;
    pstSnsState->au32FL[0] = u32FullLines;
    switch (pstSnsState->enWDRMode) {
        case WDR_MODE_NONE:
            pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = /* index 7 */
                ((pstSnsState->au32FL[0] & 0xFF00) >> 8); /* shift 8 */
            pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = (pstSnsState->au32FL[0] & 0xFF); /* index 8 */
            break;
        case WDR_MODE_2To1_LINE:
        case WDR_MODE_2To1_FRAME_FULL_RATE:
        case WDR_MODE_2To1_FRAME:
            pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = /* index 9 */
                ((pstSnsState->au32FL[0] & 0xFF00) >> 8); /* shift 8 */
            pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = (pstSnsState->au32FL[0] & 0xFF); /* index 10 */
            break;
        case WDR_MODE_3To1_LINE:
            pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = /* index 11 */
                ((pstSnsState->au32FL[0] & 0xFF00) >> 8); /* shift 8 */
            pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = (pstSnsState->au32FL[0] & 0xFF); /* index 12 */
            break;
        case WDR_MODE_4To1_LINE:
            pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = /* index 11 */
                ((pstSnsState->au32FL[0] & 0xFF00) >> 8); /* shift 8 */
            pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = (pstSnsState->au32FL[0] & 0xFF); /* index 12 */
            break;
        default:
            break;
    }

    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
    pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 2; /* sub 2 */

    return;
}

static HI_VOID cmos_inttime_update_linear(VI_PIPE vi_pipe, HI_U32 u32IntTime)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = ((u32IntTime & 0xFF00) >> 8); /* shift 8 */
    pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = (u32IntTime & 0xFF);

    return;
}
static HI_VOID cmos_inttime_update_2to1_line(VI_PIPE vi_pipe, HI_U32 u32IntTime)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    static HI_BOOL bFirst[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = HI_TRUE};

    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (bFirst[vi_pipe]) { /* short exposure */
        pstSnsState->au32WDRIntTime[0] = u32IntTime;
        pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = ((u32IntTime & 0xFF00) >> 8); /* index 7, shift 8 */
        pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = (u32IntTime & 0xFF); /* index 8 */
        bFirst[vi_pipe] = HI_FALSE;
    } else { /* long exposure */
        pstSnsState->au32WDRIntTime[1] = u32IntTime;
        pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = ((u32IntTime & 0xFF00) >> 8); /* shift 8 */
        pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = (u32IntTime & 0xFF);
        bFirst[vi_pipe] = HI_TRUE;
    }

    return;
}

static HI_VOID cmos_inttime_update_3to1_line(VI_PIPE vi_pipe, HI_U32 u32IntTime)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    static HI_U8 u8Count[ISP_MAX_PIPE_NUM] = {0};

    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (u8Count[vi_pipe] == 0) { /* short exposure */
        pstSnsState->au32WDRIntTime[0] = u32IntTime;
        pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = ((u32IntTime & 0xFF00) >> 8); /* index 9, shift 8 */
        pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = (u32IntTime & 0xFF); /* index 10 */
        u8Count[vi_pipe]++;
    } else if (u8Count[vi_pipe] == 1) { /* long exposure */
        pstSnsState->au32WDRIntTime[1] = u32IntTime;
        pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = ((u32IntTime & 0xFF00) >> 8); /* index 7, shift 8 */
        pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = (u32IntTime & 0xFF); /* index 8 */
        u8Count[vi_pipe]++;
    } else { /* long long exposure */
        pstSnsState->au32WDRIntTime[2] = u32IntTime; /* index 2 */
        pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = ((u32IntTime & 0xFF00) >> 8); /* shift 8 */
        pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = (u32IntTime & 0xFF);
        u8Count[vi_pipe] = 0;
    }

    return;
}
static HI_VOID cmos_inttime_update_4to1_line(VI_PIPE vi_pipe, HI_U32 u32IntTime)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    static HI_U8 u8Count[ISP_MAX_PIPE_NUM] = {0};

    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (u8Count[vi_pipe] == 0) { /* short short exposure */
        pstSnsState->au32WDRIntTime[0] = u32IntTime;
        pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = ((u32IntTime & 0xFF00) >> 8); /* index 11, shift 8 */
        pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = (u32IntTime & 0xFF); /* index 12 */
        u8Count[vi_pipe]++;
    } else if (u8Count[vi_pipe] == 1) { /* short exposure */
        pstSnsState->au32WDRIntTime[1] = u32IntTime;
        pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = ((u32IntTime & 0xFF00) >> 8); /* index 9, shift 8 */
        pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = (u32IntTime & 0xFF); /* index 10 */
        u8Count[vi_pipe]++;
    } else if (u8Count[vi_pipe] == 2) { /* 2: long exposure */
        pstSnsState->au32WDRIntTime[2] = u32IntTime; /* index 2 */
        pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = ((u32IntTime & 0xFF00) >> 8); /* index 7, shift 8 */
        pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = (u32IntTime & 0xFF); /* index 8 */
        u8Count[vi_pipe]++;
    } else { /* long long exposure */
        pstSnsState->au32WDRIntTime[3] = u32IntTime; /* index 3 */
        pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = ((u32IntTime & 0xFF00) >> 8); /* shift 8 */
        pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = (u32IntTime & 0xFF);
        u8Count[vi_pipe] = 0;
    }

    return;
}

static HI_VOID cmos_inttime_update_2to1_frame(VI_PIPE vi_pipe, HI_U32 u32IntTime)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    static HI_BOOL bFirst[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = HI_TRUE};

    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (bFirst[vi_pipe]) { /* short exposure */
        pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = ((u32IntTime & 0xFF00) >> 8); /* shift 8 */
        pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = (u32IntTime & 0xFF);
        bFirst[vi_pipe] = HI_FALSE;
    } else { /* long exposure */
        pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = ((u32IntTime & 0xFF00) >> 8); /* index 7, shift 8 */
        pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = (u32IntTime & 0xFF);
        bFirst[vi_pipe] = HI_TRUE;
    }

    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static HI_VOID cmos_inttime_update(VI_PIPE vi_pipe, HI_U32 u32IntTime)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if ((pstSnsState->enWDRMode == WDR_MODE_2To1_FRAME_FULL_RATE) || (pstSnsState->enWDRMode == WDR_MODE_2To1_FRAME)) {
        cmos_inttime_update_2to1_frame(vi_pipe, u32IntTime);
        return;
    }
    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        cmos_inttime_update_2to1_line(vi_pipe, u32IntTime);
        return;
    }
    if (pstSnsState->enWDRMode == WDR_MODE_3To1_LINE) {
        cmos_inttime_update_3to1_line(vi_pipe, u32IntTime);
        return;
    }
    if (pstSnsState->enWDRMode == WDR_MODE_4To1_LINE) {
        cmos_inttime_update_4to1_line(vi_pipe, u32IntTime);
        return;
    }

    cmos_inttime_update_linear(vi_pipe, u32IntTime);

    return;
}

#define  MN34220_AD_GAIN_TAB_RANGE    81
static HI_U32 g_ad_gain_table[MN34220_AD_GAIN_TAB_RANGE] = {
    1024,  1070,  1116,  1166,  1217,  1271,  1327,  1386,  1446,  1511,  1577,  1647,  1719,  1796,
    1874,  1958,  2043,  2135,  2227,  2327,  2428,  2537,  2647,  2766,  2886,  3015,  3146,  3287,
    3430,  3583,  3739,  3907,  4077,  4259,  4444,  4643,  4845,  5062,  5282,  5518,  5758,  6016,
    6278,  6558,  6844,  7150,  7461,  7795,  8134,  8498,  8867,  9264,  9667,  10100, 10539, 11010,
    11489, 12003, 12526, 13086, 13655, 14266, 14887, 15552, 16229, 16955, 17693, 18484, 19289, 20151,
    21028, 21968, 22925, 23950, 24992, 26110, 27246, 28464, 29703, 31031, 32382
};
static HI_VOID cmos_again_calc_table(VI_PIPE vi_pipe, HI_U32 *pu32AgainLin, HI_U32 *pu32AgainDb)
{
    int i;

    CMOS_CHECK_POINTER_VOID(pu32AgainLin);
    CMOS_CHECK_POINTER_VOID(pu32AgainDb);

    if (*pu32AgainLin >= g_ad_gain_table[MN34220_AD_GAIN_TAB_RANGE - 1]) {
        *pu32AgainLin = g_ad_gain_table[MN34220_AD_GAIN_TAB_RANGE - 1];
        *pu32AgainDb = MN34220_AD_GAIN_TAB_RANGE - 1;
        return;
    }

    for (i = 1; i < MN34220_AD_GAIN_TAB_RANGE; i++) {
        if (*pu32AgainLin < g_ad_gain_table[i]) {
            *pu32AgainLin = g_ad_gain_table[i - 1];
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

    if (*pu32DgainLin >= g_ad_gain_table[MN34220_AD_GAIN_TAB_RANGE - 1]) {
        *pu32DgainLin = g_ad_gain_table[MN34220_AD_GAIN_TAB_RANGE - 1];
        *pu32DgainDb = MN34220_AD_GAIN_TAB_RANGE - 1;
        return;
    }

    for (i = 1; i < MN34220_AD_GAIN_TAB_RANGE; i++) {
        if (*pu32DgainLin < g_ad_gain_table[i]) {
            *pu32DgainLin = g_ad_gain_table[i - 1];
            *pu32DgainDb = i - 1;
            break;
        }
    }

    return;
}
static HI_VOID cmos_gains_update(VI_PIPE vi_pipe, HI_U32 u32Again, HI_U32 u32Dgain)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    HI_U32 u32Tmp_Again = 0x100 + u32Again * 4; /* x4 */
    HI_U32 u32Tmp_Dgain = 0x100 + u32Dgain * 4; /* x4 */

    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if ((pstSnsState->astRegsInfo[0].astI2cData[3].u32Data == ((u32Tmp_Again & 0xFF00) >> 8)) && /* index 3, shift 8 */
        (pstSnsState->astRegsInfo[0].astI2cData[4].u32Data == (u32Tmp_Again & 0xFF))) { /* index 4 */
        pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = 0x30; /* index 2 */
    } else {
        pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = 0x0; /* index 2 */
    }

    pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = ((u32Tmp_Again & 0xFF00) >> 8); /* index 3, shift 8 */
    pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = (u32Tmp_Again & 0xFF); /* index 4 */

    pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = ((u32Tmp_Dgain & 0xFF00) >> 8); /* index 5, shift 8 */
    pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = (u32Tmp_Dgain & 0xFF); /* index 6 */

    return;
}

static HI_VOID cmos_get_inttime_max_2to1_frame(VI_PIPE vi_pipe,
    HI_U32 *au32Ratio, HI_U32 *au32IntTimeMax,
    HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    HI_U32 u32ShortMax;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32ShortMax = ((pstSnsState->au32FL[0] - 2) << 6) / SNS_DIV_0_TO_1(au32Ratio[0]); /* sub 2, shift 6 */
    if (u32ShortMax >= 2) { /* min 2 */
        au32IntTimeMax[0] = u32ShortMax;
        au32IntTimeMax[1] = (pstSnsState->au32FL[0] - 2); /* sub 2 */
        au32IntTimeMin[0] = 2; /* min 2 */
        au32IntTimeMin[1] = (au32IntTimeMin[0] * au32Ratio[0]) >> 6; /* shift 6 */
    } else {
        u32ShortMax = 2; /* min 2 */

        au32IntTimeMax[0] = u32ShortMax;
        au32IntTimeMax[1] = (pstSnsState->au32FL[0] - 2); /* sub 2 */
        au32IntTimeMin[0] = au32IntTimeMax[0];
        au32IntTimeMin[1] = au32IntTimeMax[1];
    }

    return;
}
static HI_VOID cmos_get_inttime_max_3to1_frame(VI_PIPE vi_pipe,
    HI_U32 *au32Ratio, HI_U32 *au32IntTimeMax,
    HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    HI_U32 i = 0;
    HI_U32 u32ShortMax;
    HI_U32 u32RatioTmp    = 0x40;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32ShortMax = ((pstSnsState->au32FL[0] - 2) << 12) / /* sub 2, shift 12 */
        (SNS_DIV_0_TO_1(au32Ratio[0]) * SNS_DIV_0_TO_1(au32Ratio[1]));
    if (u32ShortMax >= 2) { /* min 2 */
        au32IntTimeMax[0] = u32ShortMax;
        au32IntTimeMax[1] = (au32IntTimeMax[0] * au32Ratio[0]) >> 6; /* shift 6 */
        au32IntTimeMax[2] = (pstSnsState->au32FL[0] - 2); /* index 2, sub 2 */
        au32IntTimeMin[0] = 2; /* min 2 */
        au32IntTimeMin[1] = (au32IntTimeMin[0] * au32Ratio[0]) >> 6; /* shift 6 */
        au32IntTimeMin[2] = (au32IntTimeMin[1] * au32Ratio[1]) >> 6; /* index 2, shift 6 */
    } else {
        u32ShortMax = 2; /* min 2 */

        for (i = 0x40; i <= 0xFFF; i++) {
            if (((u32ShortMax * i * i) >> 12) > (pstSnsState->au32FL[0] - 2)) { /* sub 2, shift 12 */
                u32RatioTmp = i - 1;
                break;
            }
        }

        au32IntTimeMax[0] = u32ShortMax;
        au32IntTimeMax[1] = (au32IntTimeMax[0] * u32RatioTmp) >> 6; /* shift 6 */
        au32IntTimeMax[2] = (pstSnsState->au32FL[0] - 2); /* index 2, sub 2 */

        au32IntTimeMin[0] = au32IntTimeMax[0];
        au32IntTimeMin[1] = au32IntTimeMax[1];
        au32IntTimeMin[2] = au32IntTimeMax[2]; /* index 2 */
    }

    return;
}

static HI_VOID cmos_get_inttime_max_4to1_frame(VI_PIPE vi_pipe,
    HI_U32 *au32Ratio, HI_U32 *au32IntTimeMax,
    HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    HI_U32 i = 0;
    HI_U32 u32ShortMax;
    HI_U32 u32RatioTmp    = 0x40;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32ShortMax = ((pstSnsState->au32FL[0] - 2) << 18) / /* sub 2, shift 18 */
        (SNS_DIV_0_TO_1(au32Ratio[0]) * SNS_DIV_0_TO_1(au32Ratio[1]) * SNS_DIV_0_TO_1(au32Ratio[2])); /* index 2 */
    if (u32ShortMax >= 2) { /* min 2 */
        au32IntTimeMax[0] = u32ShortMax;
        au32IntTimeMax[1] = (au32IntTimeMax[0] * au32Ratio[0]) >> 6; /* shift 6 */
        au32IntTimeMax[2] = (au32IntTimeMax[1] * au32Ratio[1]) >> 6; /* index 2, shift 6 */
        au32IntTimeMax[3] = (pstSnsState->au32FL[0] - 2); /* index 3, sub 2 */
        au32IntTimeMin[0] = 2; /* min 2 */
        au32IntTimeMin[1] = (au32IntTimeMin[0] * au32Ratio[0]) >> 6; /* shift 6 */
        au32IntTimeMin[2] = (au32IntTimeMin[1] * au32Ratio[1]) >> 6; /* index 2, shift 6 */
        au32IntTimeMin[3] = (au32IntTimeMin[2] * au32Ratio[2]) >> 6; /* index 2, index 3, shift 6 */
    } else {
        u32ShortMax = 2; /* min 2 */

        for (i = 0x40; i <= 0xFFF; i++) {
            if (((HI_U64)(u32ShortMax * i * i * i) >> 18) > (pstSnsState->au32FL[0] - 2)) { /* sub 2, shift 18 */
                u32RatioTmp = i - 1;
                break;
            }
        }

        au32IntTimeMax[0] = u32ShortMax;
        au32IntTimeMax[1] = (au32IntTimeMax[0] * u32RatioTmp) >> 6; /* shift 6 */
        au32IntTimeMax[2] = (au32IntTimeMax[1] * u32RatioTmp) >> 6; /* index 2, shift 6 */
        au32IntTimeMax[3] = (pstSnsState->au32FL[0] - 2); /* index 3, sub 2 */

        au32IntTimeMin[0] = au32IntTimeMax[0];
        au32IntTimeMin[1] = au32IntTimeMax[1];
        au32IntTimeMin[2] = au32IntTimeMax[2]; /* index 2 */
        au32IntTimeMin[3] = au32IntTimeMax[3]; /* index 3 */
    }

    return;
}

static HI_VOID cmos_get_inttime_max_2to1_line(VI_PIPE vi_pipe,
    HI_U32 *au32Ratio, HI_U32 *au32IntTimeMax,
    HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    HI_U32 i = 0;
    HI_U32 u32ShortMax, u32ShortMax1;
    HI_U32 u32RatioTmp    = 0x40;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (g_genFSWDRMode[vi_pipe] == ISP_FSWDR_LONG_FRAME_MODE) { /* long frame mode enable */
        /* when change LongFrameMode, the first 2 frames must limit the MaxIntTime to avoid flicker */
        u32ShortMax = pstSnsState->au32FL[1] - 10 - pstSnsState->au32WDRIntTime[0]; /* sub 10 */
        if (g_au32MaxTimeGetCnt[vi_pipe] < 2) { /* min 2 */
            u32ShortMax = pstSnsState->au32FL[1] - 10 - 750; /* sub 10, sub 750 */
        }
        u32ShortMax1 = pstSnsState->au32FL[0] - 10; /* sub 10 */
        u32ShortMax  = (u32ShortMax < u32ShortMax1) ? u32ShortMax : u32ShortMax1;
        au32IntTimeMax[0] = u32ShortMax;
        au32IntTimeMin[0] = 2; /* min 2 */
        g_au32MaxTimeGetCnt[vi_pipe]++;
        return;
    } else {
        /* To avoid Long frame and PreShort frame exposure time out of sync, so use (gu32FullLinesStd - 30) */
        u32ShortMax = ((pstSnsState->au32FL[1] - 30 - pstSnsState->au32WDRIntTime[0]) << 6) / /* sub 30, shift 6 */
                             SNS_DIV_0_TO_1(au32Ratio[0]);
        u32ShortMax1 = ((pstSnsState->au32FL[0] - 30) << 6) / (au32Ratio[0] + 0x40); /* sub 30, shift 6 */
        u32ShortMax = (u32ShortMax < u32ShortMax1) ? u32ShortMax : u32ShortMax1;
    }

    if (u32ShortMax >= 2) { /* min 2 */
            au32IntTimeMax[0] = u32ShortMax;
            au32IntTimeMax[1] = (au32IntTimeMax[0] * au32Ratio[0]) >> 6; /* shift 6 */
            au32IntTimeMin[0] = 2; /* min 2 */
            au32IntTimeMin[1] = (au32IntTimeMin[0] * au32Ratio[0]) >> 6; /* shift 6 */
    } else {
        u32ShortMax = 2; /* min 2 */

        for (i = 0x40; i <= 0xFFF; i++) {
            if ((u32ShortMax + ((u32ShortMax * i) >> 6)) > (pstSnsState->au32FL[0] - 30)) { /* sub 30, shift 6 */
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
static HI_VOID cmos_get_inttime_max_3to1_line(VI_PIPE vi_pipe, HI_U32 *au32Ratio, HI_U32 *au32IntTimeMax,
    HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    HI_U32 i, u32ShortMax, u32ShortMax1;
    HI_U32 u32RatioTmp    = 0x40;
    ISP_SNS_STATE_S *pstSns = HI_NULL;

    MN34220_SENSOR_GET_CTX(vi_pipe, pstSns);
    CMOS_CHECK_POINTER_VOID(pstSns);

    if (g_genFSWDRMode[vi_pipe] == ISP_FSWDR_LONG_FRAME_MODE) { /* long frame mode enable */
        /* when change LongFrameMode, the first 2 frames must limit the MaxIntTime to avoid flicker */
        u32ShortMax = pstSns->au32FL[1] - 30 - pstSns->au32WDRIntTime[0] - pstSns->au32WDRIntTime[1]; /* sub 30 */
        if (g_au32MaxTimeGetCnt[vi_pipe] < 2) { /* min 2 */
            u32ShortMax = pstSns->au32FL[1] - 30 - 750; /* sub 30, sub 750 */
        }
        u32ShortMax1 = pstSns->au32FL[0] - 30; /* sub 30 */
        u32ShortMax = (u32ShortMax < u32ShortMax1) ? u32ShortMax : u32ShortMax1;
        au32IntTimeMax[0] = u32ShortMax;
        au32IntTimeMin[0] = 2; /* min 2 */
        g_au32MaxTimeGetCnt[vi_pipe]++;
        return;
    }

    u32ShortMax = ((pstSns->au32FL[1] - 30 - pstSns->au32WDRIntTime[0] - pstSns->au32WDRIntTime[1]) * /* sub 30 */
        0x40 * 0x40) / (SNS_DIV_0_TO_1(au32Ratio[0]) * SNS_DIV_0_TO_1(au32Ratio[1]));
    u32ShortMax1 = ((pstSns->au32FL[0] - 30) << 12) / /* sub 30, shift 12 */
        (au32Ratio[0] * au32Ratio[1] + au32Ratio[0] * 0x40 + 0x40 * 0x40);
    u32ShortMax = (u32ShortMax < u32ShortMax1) ? u32ShortMax : u32ShortMax1;

    if (u32ShortMax >= 2) { /* min 2 */
        au32IntTimeMax[0] = u32ShortMax;
        au32IntTimeMax[1] = (au32IntTimeMax[0] * au32Ratio[0]) >> 6; /* shift 6 */
        au32IntTimeMax[2] = (au32IntTimeMax[1] * au32Ratio[1]) >> 6; /* index 2, shift 6 */
        au32IntTimeMin[0] = 2; /* min 2 */
        au32IntTimeMin[1] = (au32IntTimeMin[0] * au32Ratio[0]) >> 6; /* shift 6 */
        au32IntTimeMin[2] = (au32IntTimeMin[1] * au32Ratio[1]) >> 6; /* index 2, shift 6 */
    } else {
        u32ShortMax = 2; /* min 2 */

        for (i = 0x40; i <= 0xFFF; i++) {
            if ((u32ShortMax + ((u32ShortMax * i) >> 6) + ((u32ShortMax * i * i) >> 12)) > /* shift 6, shift 12 */
                (pstSns->au32FL[0] - 30)) { /* sub 30 */
                u32RatioTmp = i - 1;
                break;
            }
        }

        au32IntTimeMax[0] = u32ShortMax;
        au32IntTimeMax[1] = (au32IntTimeMax[0] * u32RatioTmp) >> 6; /* shift 6 */
        au32IntTimeMax[2] = (au32IntTimeMax[1] * u32RatioTmp) >> 6; /* index 2, shift 6 */

        au32IntTimeMin[0] = au32IntTimeMax[0];
        au32IntTimeMin[1] = au32IntTimeMax[1];
        au32IntTimeMin[2] = au32IntTimeMax[2]; /* index 2 */
    }

    return;
}
static HI_VOID cmos_get_inttime_max_4to1_line_long_frame(VI_PIPE vi_pipe, HI_U32 *au32Ratio,
    HI_U32 *au32IntTimeMax, HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    HI_U32 u32ShortMax, u32ShortMax1;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    /* when change LongFrameMode, the first 2 frames must limit the MaxIntTime to avoid flicker */
    u32ShortMax = pstSnsState->au32FL[1] - 30 - pstSnsState->au32WDRIntTime[0] - /* sub 30 */
                         pstSnsState->au32WDRIntTime[1] - pstSnsState->au32WDRIntTime[2]; /* index 2 */
    if (g_au32MaxTimeGetCnt[vi_pipe] < 2) { /* min 2 */
        u32ShortMax = pstSnsState->au32FL[1] - 30 - 560; /* sub 30, sub 560 */
    }
    u32ShortMax1 = pstSnsState->au32FL[0] - 30; /* sub 30 */
    u32ShortMax = (u32ShortMax < u32ShortMax1) ? u32ShortMax : u32ShortMax1;
    au32IntTimeMax[0] = u32ShortMax;
    au32IntTimeMin[0] = 2; /* min 2 */
    g_au32MaxTimeGetCnt[vi_pipe]++;

    return;
}
static HI_VOID cmos_get_inttime_max_4to1_line_normal(VI_PIPE vi_pipe, HI_U32 *au32Ratio,
    HI_U32 *au32IntTimeMax, HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    HI_U32 i, u32ShortMax, u32ShortMax1;
    HI_U32 u32RatioTmp    = 0x40;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32ShortMax = ((pstSnsState->au32FL[1] - 30 - pstSnsState->au32WDRIntTime[0] - /* sub 30 */
        pstSnsState->au32WDRIntTime[1] - pstSnsState->au32WDRIntTime[2])  << 18) / /* index 2, shift 18 */
        (SNS_DIV_0_TO_1(au32Ratio[0]) * SNS_DIV_0_TO_1(au32Ratio[1]) * SNS_DIV_0_TO_1(au32Ratio[2])); /* index 2 */
    u32ShortMax1 = ((pstSnsState->au32FL[0] - 30) << 18) / /* sub 30, shift 18 */
        (au32Ratio[0] * au32Ratio[1] * au32Ratio[2] + au32Ratio[0] * au32Ratio[1] * 0x40 + /* index 2 */
        au32Ratio[0] * 0x40 * 0x40 + 0x40 * 0x40 * 0x40);
    u32ShortMax = (u32ShortMax < u32ShortMax1) ? u32ShortMax : u32ShortMax1;

    if (u32ShortMax >= 2) { /* min 2 */
        au32IntTimeMax[0] = u32ShortMax;
        au32IntTimeMax[1] = (au32IntTimeMax[0] * au32Ratio[0]) >> 6; /* shift 6 */
        au32IntTimeMax[2] = (au32IntTimeMax[1] * au32Ratio[1]) >> 6; /* index 2, shift 6 */
        au32IntTimeMax[3] = (au32IntTimeMax[2] * au32Ratio[2]) >> 6; /* index 2, index 3, shift 6 */
        au32IntTimeMin[0] = 2; /* min 2 */
        au32IntTimeMin[1] = (au32IntTimeMin[0] * au32Ratio[0]) >> 6; /* shift 6 */
        au32IntTimeMin[2] = (au32IntTimeMin[1] * au32Ratio[1]) >> 6; /* index 2, shift 6 */
        au32IntTimeMin[3] = (au32IntTimeMin[2] * au32Ratio[2]) >> 6; /* index 2, index 3, shift 6 */
    } else {
        u32ShortMax = 2; /* min 2 */

        for (i = 0x40; i <= 0xFFF; i++) {
            if ((u32ShortMax + ((u32ShortMax * i) >> 6) + ((u32ShortMax * i * i) >> 12) + /* shift 6, shift 12 */
                 ((HI_U64)(u32ShortMax * i * i * i) >> 18)) > (pstSnsState->au32FL[0] - 30)) { /* sub 30, shift 18 */
                u32RatioTmp = i - 1;
                break;
            }
        }

        au32IntTimeMax[0] = u32ShortMax;
        au32IntTimeMax[1] = (au32IntTimeMax[0] * u32RatioTmp) >> 6; /* shift 6 */
        au32IntTimeMax[2] = (au32IntTimeMax[1] * u32RatioTmp) >> 6; /* index 2, shift 6 */
        au32IntTimeMax[3] = (au32IntTimeMax[2] * u32RatioTmp) >> 6; /* index 2, index 3, shift 6 */

        au32IntTimeMin[0] = au32IntTimeMax[0];
        au32IntTimeMin[1] = au32IntTimeMax[1];
        au32IntTimeMin[2] = au32IntTimeMax[2]; /* index 2 */
        au32IntTimeMin[3] = au32IntTimeMax[3]; /* index 3 */
    }

    return;
}

static HI_VOID cmos_get_inttime_max_4to1_line(VI_PIPE vi_pipe, HI_U32 *au32Ratio, HI_U32 *au32IntTimeMax,
    HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    switch (g_genFSWDRMode[vi_pipe]) {
        case ISP_FSWDR_LONG_FRAME_MODE:
            cmos_get_inttime_max_4to1_line_long_frame(vi_pipe, au32Ratio, au32IntTimeMax, au32IntTimeMin,
                pu32LFMaxIntTime);
            break;
        case ISP_FSWDR_NORMAL_MODE:
            cmos_get_inttime_max_4to1_line_normal(vi_pipe, au32Ratio, au32IntTimeMax, au32IntTimeMin,
                pu32LFMaxIntTime);
            break;
        default:
            break;
    }

    return;
}

static HI_VOID cmos_get_inttime_max(VI_PIPE vi_pipe, HI_U16 u16ManRatioEnable, HI_U32 *au32Ratio,
    HI_U32 *au32IntTimeMax, HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER_VOID(au32Ratio);
    CMOS_CHECK_POINTER_VOID(au32IntTimeMax);
    CMOS_CHECK_POINTER_VOID(au32IntTimeMin);
    CMOS_CHECK_POINTER_VOID(pu32LFMaxIntTime);

    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    switch (pstSnsState->enWDRMode) {
        case WDR_MODE_2To1_FRAME:
        case WDR_MODE_2To1_FRAME_FULL_RATE:
            cmos_get_inttime_max_2to1_frame(vi_pipe, au32Ratio, au32IntTimeMax, au32IntTimeMin, pu32LFMaxIntTime);
            break;
        case WDR_MODE_3To1_FRAME:
        case WDR_MODE_3To1_FRAME_FULL_RATE:
            cmos_get_inttime_max_3to1_frame(vi_pipe, au32Ratio, au32IntTimeMax, au32IntTimeMin, pu32LFMaxIntTime);
            break;
        case WDR_MODE_4To1_FRAME:
        case WDR_MODE_4To1_FRAME_FULL_RATE:
            cmos_get_inttime_max_4to1_frame(vi_pipe, au32Ratio, au32IntTimeMax, au32IntTimeMin, pu32LFMaxIntTime);
            break;
        case WDR_MODE_2To1_LINE:
            cmos_get_inttime_max_2to1_line(vi_pipe, au32Ratio, au32IntTimeMax, au32IntTimeMin, pu32LFMaxIntTime);
            break;
        case WDR_MODE_3To1_LINE:
            cmos_get_inttime_max_3to1_line(vi_pipe, au32Ratio, au32IntTimeMax, au32IntTimeMin, pu32LFMaxIntTime);
            break;
        case WDR_MODE_4To1_LINE:
            cmos_get_inttime_max_4to1_line(vi_pipe, au32Ratio, au32IntTimeMax, au32IntTimeMin, pu32LFMaxIntTime);
            break;

        default:
            break;
    }

    return;
}

/* Only used in Line_WDR mode */
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
#define CALIBRATE_STATIC_TEMP       5000
#define CALIBRATE_STATIC_WB_R_GAIN  0x1F0
#define CALIBRATE_STATIC_WB_GR_GAIN 0x100
#define CALIBRATE_STATIC_WB_GB_GAIN 0x100
#define CALIBRATE_STATIC_WB_B_GAIN  0x1A0

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1 41
#define CALIBRATE_AWB_P2 127
#define CALIBRATE_AWB_Q1 (-87)
#define CALIBRATE_AWB_A1 174535
#define CALIBRATE_AWB_B1 128
#define CALIBRATE_AWB_C1 (-123490)

static HI_S32 cmos_get_awb_default(VI_PIPE vi_pipe, AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstAwbSnsDft);
    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
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

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:
            memcpy_s(&pstAwbSnsDft->stCcm, sizeof(AWB_CCM_S), &g_stAwbCcm, sizeof(AWB_CCM_S));
            memcpy_s(&pstAwbSnsDft->stAgcTbl, sizeof(AWB_AGC_TABLE_S), &g_stAwbAgcTable, sizeof(AWB_AGC_TABLE_S));
            break;

        case WDR_MODE_2To1_LINE:
        case WDR_MODE_3To1_LINE:
        case WDR_MODE_4To1_LINE:
        case WDR_MODE_2To1_FRAME:
            memcpy_s(&pstAwbSnsDft->stCcm, sizeof(AWB_CCM_S), &g_stAwbCcmFsWdr, sizeof(AWB_CCM_S));
            memcpy_s(&pstAwbSnsDft->stAgcTbl, sizeof(AWB_AGC_TABLE_S), &g_stAwbAgcTableFSWDR, sizeof(AWB_AGC_TABLE_S));
            break;
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

    pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 12; /* 12bit */
    pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 4095; /* max 4095 */

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

static void cmos_get_isp_comm_default(ISP_CMOS_DEFAULT_S *pstDef)
{
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
    pstDef->unKey.bit1Lsc      = 0;
    pstDef->pstLsc             = &g_stCmosLsc;

#ifdef CONFIG_HI_ISP_EDGEMARK_SUPPORT
    pstDef->unKey.bit1EdgeMark = 0;
    pstDef->pstEdgeMark        = &g_stIspEdgeMark;
#endif
#ifdef CONFIG_HI_ISP_PREGAMMA_SUPPORT
    pstDef->unKey.bit1PreGamma = 0;
    pstDef->pstPreGamma        = &g_stPreGamma;
#endif

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
    pstDef->pstAntiFalseColor        = &g_stIspAntiFalseColor;
    pstDef->unKey.bit1Ldci           = 1;
    pstDef->pstLdci                  = &g_stIspLdci;
    pstDef->unKey.bit1Gamma          = 1;
    pstDef->pstGamma                 = &g_stIspGamma;
    pstDef->unKey.bit1Detail         = 1;
    pstDef->pstDetail                = &g_stIspDetail;
#ifdef CONFIG_HI_ISP_CR_SUPPORT
    pstDef->unKey.bit1Ge             = 1;
    pstDef->pstGe                    = &g_stIspGe;
#endif
    pstDef->unKey.bit1Dehaze = 1;
    pstDef->pstDehaze = &g_stIspDehaze;
    pstDef->unKey.bit1Lcac = 1;
    pstDef->pstLcac = &g_stIspLCac;
    memcpy_s(&pstDef->stNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S),
        &g_stIspNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
    return;
}

static void cmos_get_isp_wdr_default(ISP_CMOS_DEFAULT_S *pstDef)
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
    memcpy_s(&pstDef->stNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S),
        &g_stIspNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
    return;
}

static  HI_S32 cmos_get_isp_default(VI_PIPE vi_pipe, ISP_CMOS_DEFAULT_S *pstDef)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstDef);
    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset_s(pstDef, sizeof(ISP_CMOS_DEFAULT_S), 0, sizeof(ISP_CMOS_DEFAULT_S));

    cmos_get_isp_comm_default(pstDef);

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:
            cmos_get_isp_linear_default(pstDef);
            break;

        case WDR_MODE_BUILT_IN:
        case WDR_MODE_2To1_LINE:
        case WDR_MODE_2To1_FRAME:
        case WDR_MODE_2To1_FRAME_FULL_RATE:
        case WDR_MODE_3To1_LINE:
        case WDR_MODE_3To1_FRAME:
        case WDR_MODE_3To1_FRAME_FULL_RATE:
        case WDR_MODE_4To1_LINE:
        case WDR_MODE_4To1_FRAME:
        case WDR_MODE_4To1_FRAME_FULL_RATE:
            cmos_get_isp_wdr_default(pstDef);
            break;
    }

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        pstDef->stWdrSwitchAttr.au32ExpRatio[0] = 0x40;
        pstDef->stWdrSwitchAttr.au32ExpRatio[1] = 0x40;
        pstDef->stWdrSwitchAttr.au32ExpRatio[2] = 0x40; /* index 2 */
    } else if (pstSnsState->enWDRMode == WDR_MODE_2To1_FRAME) {
        pstDef->stWdrSwitchAttr.au32ExpRatio[0] = 0x200;
        pstDef->stWdrSwitchAttr.au32ExpRatio[1] = 0x40;
        pstDef->stWdrSwitchAttr.au32ExpRatio[2] = 0x40; /* index 2 */
    }

    pstDef->stSensorMode.u32SensorID = MN34220_ID;
    pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

    cmos_get_isp_dng_default(pstSnsState, pstDef);

    return HI_SUCCESS;
}

static HI_S32 cmos_get_isp_black_level(VI_PIPE vi_pipe, ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
    HI_S32  i;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstBlackLevel);
    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    /* Don't need to update black level when iso change */
    pstBlackLevel->bUpdate = HI_FALSE;

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        pstBlackLevel->au16BlackLevel[i] = 0x100;
    }

    return HI_SUCCESS;
}
static HI_VOID cmos_set_pixel_detect(VI_PIPE vi_pipe, HI_BOOL bEnable)
{
    HI_U32 u32FullLines_5Fps, u32MaxIntTime_5Fps;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        return;
    }
    switch (pstSnsState->u8ImgMode) {
        case MN34220_1080P_30FPS_MODE:
            u32FullLines_5Fps = (MN34220_VMAX_1080P30_LINEAR * 30) / 5; /* 30fps, 5fps */
            break;
        case MN34220_1080P_60FPS_MODE:
            u32FullLines_5Fps = (MN34220_VMAX_1080P60_LINEAR * 60) / 5; /* 60fps, 5fps */
            break;
        case MN34220_720P_120FPS_MODE:
            u32FullLines_5Fps = (MN34220_VMAX_720P120_LINEAR * 120) / 5; /* 120fps, 5fps */
            break;
        case MN34220_VGA_240FPS_MODE:
            u32FullLines_5Fps = (MN34220_VMAX_VGA240_LINEAR * 240) / 5; /* 240fps, 5fps */
            break;
        default:
            return;
    }
    u32MaxIntTime_5Fps = u32FullLines_5Fps - 2; /* sub 2 */

    if (bEnable) { /* setup for ISP pixel calibration mode */
        mn34220_write_register(vi_pipe, MN34220_VMAX_ADDR_H, (u32FullLines_5Fps & 0xFF00) >> 8); /* shift 8 */
        mn34220_write_register(vi_pipe, MN34220_VMAX_ADDR_L, u32FullLines_5Fps & 0xFF);          /* 5fps */
        mn34220_write_register(vi_pipe, 0x0202, (u32MaxIntTime_5Fps & 0xFF00) >> 8); /* shift 8 */
        mn34220_write_register(vi_pipe, 0x0203, u32MaxIntTime_5Fps & 0xFF);                 /* max exposure lines */
        mn34220_write_register(vi_pipe, 0x0204, 0x01);                                           /* min AG */
        mn34220_write_register(vi_pipe, 0x0205, 0x00);                                           /* min AG */
        mn34220_write_register(vi_pipe, 0x3108, 0x01);                                           /* min DG */
        mn34220_write_register(vi_pipe, 0x3109, 0x00);                                           /* min DG */
    } else { /* setup for ISP 'normal mode' */
        mn34220_write_register(vi_pipe, MN34220_VMAX_ADDR_H, (pstSnsState->u32FLStd & 0xFF00) >> 8); /* shift 8 */
        mn34220_write_register(vi_pipe, MN34220_VMAX_ADDR_L, pstSnsState->u32FLStd & 0xFF);
        pstSnsState->bSyncInit = HI_FALSE;
    }
    return;
}

static HI_VOID cmos_set_linear(ISP_SNS_STATE_S *pstSnsState)
{
    pstSnsState->enWDRMode = WDR_MODE_NONE;
    printf("linear mode\n");

    if (pstSnsState->u8ImgMode == MN34220_1080P_30FPS_MODE) {
        pstSnsState->u32FLStd = MN34220_VMAX_1080P30_LINEAR;
        return;
    }
    if (pstSnsState->u8ImgMode == MN34220_1080P_60FPS_MODE) {
        pstSnsState->u32FLStd = MN34220_VMAX_1080P60_LINEAR;
        return;
    }
    if (pstSnsState->u8ImgMode == MN34220_720P_120FPS_MODE) {
        pstSnsState->u32FLStd = MN34220_VMAX_720P120_LINEAR;
        return;
    }
    if (pstSnsState->u8ImgMode == MN34220_VGA_240FPS_MODE) {
        pstSnsState->u32FLStd = MN34220_VMAX_VGA240_LINEAR;
        return;
    }
    return;
}

static HI_VOID cmos_set_2to1_line(ISP_SNS_STATE_S *pstSnsState)
{
    pstSnsState->u32FLStd = MN34220_VMAX_1080P60TO30_WDR;
    pstSnsState->enWDRMode = WDR_MODE_2To1_LINE;
    printf("2to1 line WDR mode\n");

    return;
}

static HI_VOID cmos_set_2to1_frame(ISP_SNS_STATE_S *pstSnsState)
{
    if (pstSnsState->u8ImgMode == MN34220_1080P_30FPS_MODE) {
        pstSnsState->u32FLStd = MN34220_VMAX_1080P30_LINEAR;
    } else if (pstSnsState->u8ImgMode == MN34220_1080P_60FPS_MODE) {
        pstSnsState->u32FLStd = MN34220_VMAX_1080P60_LINEAR;
    }
    pstSnsState->enWDRMode = WDR_MODE_2To1_FRAME;
    printf("2to1 half-rate frame WDR mode\n");

    return;
}

static HI_VOID cmos_set_2to1_full_frame(ISP_SNS_STATE_S *pstSnsState)
{
    if (pstSnsState->u8ImgMode == MN34220_1080P_30FPS_MODE) {
        pstSnsState->u32FLStd = MN34220_VMAX_1080P30_LINEAR;
    } else if (pstSnsState->u8ImgMode == MN34220_1080P_60FPS_MODE) {
        pstSnsState->u32FLStd = MN34220_VMAX_1080P60_LINEAR;
    }
    pstSnsState->enWDRMode = WDR_MODE_2To1_FRAME_FULL_RATE;
    printf("2to1 full-rate frame WDR mode\n");

    return;
}

static HI_VOID cmos_set_3to1_line(ISP_SNS_STATE_S *pstSnsState)
{
    if (pstSnsState->u8ImgMode == MN34220_1080P_30FPS_MODE) {
        pstSnsState->u32FLStd = MN34220_VMAX_1080P90TO30_WDR;
    } else {
        /* for FPGA */
        pstSnsState->u32FLStd = MN34220_VMAX_720P_WDR;
    }
    pstSnsState->enWDRMode = WDR_MODE_3To1_LINE;
    printf("3to1 line WDR mode\n");

    return;
}

static HI_VOID cmos_set_4to1_line(ISP_SNS_STATE_S *pstSnsState)
{
    if (pstSnsState->u8ImgMode == MN34220_1080P_30FPS_MODE) {
        pstSnsState->u32FLStd = MN34220_VMAX_1080P120TO30_WDR;
    } else {
        /* for FPGA */
        pstSnsState->u32FLStd = MN34220_VMAX_720P_WDR;
    }
    pstSnsState->enWDRMode = WDR_MODE_4To1_LINE;
    printf("4to1 line WDR mode\n");

    return;
}

static HI_S32 cmos_set_wdr_mode(VI_PIPE vi_pipe, HI_U8 u8Mode)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);
    pstSnsState->bSyncInit = HI_FALSE;
    switch (u8Mode & 0x3F) {
        case WDR_MODE_NONE:
            cmos_set_linear(pstSnsState);
            break;
        case WDR_MODE_2To1_LINE:
            cmos_set_2to1_line(pstSnsState);
            break;
        case WDR_MODE_2To1_FRAME:
            cmos_set_2to1_frame(pstSnsState);
            break;
        case WDR_MODE_2To1_FRAME_FULL_RATE:
            cmos_set_2to1_full_frame(pstSnsState);
            break;
        case WDR_MODE_3To1_LINE:
            cmos_set_3to1_line(pstSnsState);
            break;
        case WDR_MODE_4To1_LINE:
            cmos_set_4to1_line(pstSnsState);
            break;
        default:
            SNS_ERR_TRACE("NOT support this mode!\n");
            return HI_FAILURE;
    }
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstSnsState->au32FL[1] = pstSnsState->au32FL[0];
    memset_s(pstSnsState->au32WDRIntTime, sizeof(pstSnsState->au32WDRIntTime), 0, sizeof(pstSnsState->au32WDRIntTime));
    return HI_SUCCESS;
}
static HI_VOID cmos_comm_sns_reg_info_init(VI_PIPE vi_pipe, ISP_SNS_STATE_S *pstSnsState)
{
    HI_S32 i;
    pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
    pstSnsState->astRegsInfo[0].unComBus.s8I2cDev = g_aunMn34220BusInfo[vi_pipe].s8I2cDev;
    pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2; /* DelayMax 2 */
    switch (pstSnsState->enWDRMode) {
        case WDR_MODE_2To1_LINE:
        case WDR_MODE_2To1_FRAME_FULL_RATE:
        case WDR_MODE_2To1_FRAME:
            pstSnsState->astRegsInfo[0].u32RegNum += 2; /* DelayMax add 2 */
            break;
        case WDR_MODE_3To1_LINE:
        case WDR_MODE_3To1_FRAME_FULL_RATE:
        case WDR_MODE_3To1_FRAME:
            pstSnsState->astRegsInfo[0].u32RegNum += 4; /* DelayMax add 4 */
            break;
        case WDR_MODE_4To1_LINE:
        case WDR_MODE_4To1_FRAME_FULL_RATE:
        case WDR_MODE_4To1_FRAME:
            pstSnsState->astRegsInfo[0].u32RegNum += 6; /* DelayMax add 6 */
            break;
        default:
            break;
    }
    for (i = 0; i < 9; i++) { /* RegNum 9 */
        pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
        pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr = MN34220_I2C_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[i].u32AddrByteNum = MN34220_ADDR_BYTE;
        pstSnsState->astRegsInfo[0].astI2cData[i].u32DataByteNum = MN34220_DATA_BYTE;
    }
    pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0 /* Shutter (Shutter Long) */;
    pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr = MN34220_EXPOSURE_ADDR_H;
    pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
    pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr = MN34220_EXPOSURE_ADDR_L;
    pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 1; /* index 2 */
    pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr = 0x3280; /* index 2 */
    pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 1; /* index 3 */
    pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr = MN34220_AGC_ADDR_H; /* index 3 */
    pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 1; /* index 4 */
    pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr = MN34220_AGC_ADDR_L; /* index 4 */
    pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 1; /* index 5 */
    pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr = MN34220_DGC_ADDR_H; /* index 5 */
    pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 1; /* index 6 */
    pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr = MN34220_DGC_ADDR_L; /* index 6 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 0; /* index 7 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr = MN34220_VMAX_ADDR_H; /* index 7 */
    pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 0; /* index 8 */
    pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr = MN34220_VMAX_ADDR_L; /* index 8 */
    return;
}

static HI_VOID cmos_2to1_line_wdr_sns_reg_info_init(VI_PIPE vi_pipe, ISP_SNS_STATE_S *pstSnsState)
{
    pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 1; /* index 7 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr = MN34220_LONG_EXPOSURE_ADDR_H; /* index 7 */
    pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 1; /* index 8 */
    pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr = MN34220_LONG_EXPOSURE_ADDR_L; /* index 8 */

    pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum = 1; /* index 9 */
    pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr = MN34220_VMAX_ADDR_H; /* index 9 */
    pstSnsState->astRegsInfo[0].astI2cData[10].u8DelayFrmNum = 1; /* index 10 */
    pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr = MN34220_VMAX_ADDR_L; /* index 10 */
    return;
}

static HI_VOID cmos_2to1_frame_wdr_sns_reg_info_init(VI_PIPE vi_pipe, ISP_SNS_STATE_S *pstSnsState)
{
    pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 1; /* index 7 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr = MN34220_EXPOSURE_ADDR_H; /* index 7 */
    pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 1; /* index 8 */
    pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr = MN34220_EXPOSURE_ADDR_L; /* index 8 */

    pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum = 0; /* index 9 */
    pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr = MN34220_VMAX_ADDR_H; /* index 9 */
    pstSnsState->astRegsInfo[0].astI2cData[10].u8DelayFrmNum = 0; /* index 10 */
    pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr = MN34220_VMAX_ADDR_L; /* index 10 */
    return;
}

static HI_VOID cmos_3to1_line_wdr_sns_reg_info_init(VI_PIPE vi_pipe, ISP_SNS_STATE_S *pstSnsState)
{
    pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 1; /* index 7 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr = MN34220_LONG_EXPOSURE_ADDR_H; /* index 7 */
    pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 1; /* index 8 */
    pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr = MN34220_LONG_EXPOSURE_ADDR_L; /* index 8 */
    pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum = 1; /* index 9 */
    pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr = MN34220_SHORT_EXPOSURE_ADDR_H; /* index 9 */
    pstSnsState->astRegsInfo[0].astI2cData[10].u8DelayFrmNum = 1; /* index 10 */
    pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr = MN34220_SHORT_EXPOSURE_ADDR_L; /* index 10 */

    pstSnsState->astRegsInfo[0].astI2cData[11].u8DelayFrmNum = 1; /* index 11 */
    pstSnsState->astRegsInfo[0].astI2cData[11].u32RegAddr = MN34220_VMAX_ADDR_H; /* index 11 */
    pstSnsState->astRegsInfo[0].astI2cData[12].u8DelayFrmNum = 1; /* index 12 */
    pstSnsState->astRegsInfo[0].astI2cData[12].u32RegAddr = MN34220_VMAX_ADDR_L; /* index 12 */
    return;
}

static HI_VOID cmos_4to1_line_wdr_sns_reg_info_init(VI_PIPE vi_pipe, ISP_SNS_STATE_S *pstSnsState)
{
    pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 1; /* index 7 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr = MN34220_LONG_EXPOSURE_ADDR_H; /* index 7 */
    pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 1; /* index 8 */
    pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr = MN34220_LONG_EXPOSURE_ADDR_L; /* index 8 */
    pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum = 1; /* index 9 */
    pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr = MN34220_SHORT_EXPOSURE_ADDR_H; /* index 9 */
    pstSnsState->astRegsInfo[0].astI2cData[10].u8DelayFrmNum = 1; /* index 10 */
    pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr = MN34220_SHORT_EXPOSURE_ADDR_L; /* index 10 */
    pstSnsState->astRegsInfo[0].astI2cData[11].u8DelayFrmNum = 1; /* index 11 */
    pstSnsState->astRegsInfo[0].astI2cData[11].u32RegAddr = MN34220_SHORT_SHORT_EXPOSURE_ADDR_H; /* index 11 */
    pstSnsState->astRegsInfo[0].astI2cData[12].u8DelayFrmNum = 1; /* index 12 */
    pstSnsState->astRegsInfo[0].astI2cData[12].u32RegAddr = MN34220_SHORT_SHORT_EXPOSURE_ADDR_L; /* index 12 */

    pstSnsState->astRegsInfo[0].astI2cData[13].u8DelayFrmNum = 1; /* index 13 */
    pstSnsState->astRegsInfo[0].astI2cData[13].u32RegAddr = MN34220_VMAX_ADDR_H; /* index 13 */
    pstSnsState->astRegsInfo[0].astI2cData[14].u8DelayFrmNum = 1; /* index 14 */
    pstSnsState->astRegsInfo[0].astI2cData[14].u32RegAddr = MN34220_VMAX_ADDR_L; /* index 14 */
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
    if ((pstSnsState->enWDRMode == WDR_MODE_2To1_FRAME_FULL_RATE) ||
        (pstSnsState->enWDRMode == WDR_MODE_2To1_FRAME)) {
        pstSnsState->astRegsInfo[0].astI2cData[0].bUpdate = HI_TRUE;
        pstSnsState->astRegsInfo[0].astI2cData[1].bUpdate = HI_TRUE;
        pstSnsState->astRegsInfo[0].astI2cData[7].bUpdate = HI_TRUE; /* index 7 */
        pstSnsState->astRegsInfo[0].astI2cData[8].bUpdate = HI_TRUE; /* index 8 */
    }

    return;
}

static HI_S32 cmos_get_sns_regs_info(VI_PIPE vi_pipe, ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    CMOS_CHECK_POINTER(pstSnsRegsInfo);
    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);
    if ((pstSnsState->bSyncInit == HI_FALSE) || (pstSnsRegsInfo->bConfig == HI_FALSE)) {
        cmos_comm_sns_reg_info_init(vi_pipe, pstSnsState);
        switch (pstSnsState->enWDRMode) {
            case WDR_MODE_2To1_LINE:
                cmos_2to1_line_wdr_sns_reg_info_init(vi_pipe, pstSnsState);
                break;

            case WDR_MODE_2To1_FRAME_FULL_RATE:
            case WDR_MODE_2To1_FRAME:
                cmos_2to1_frame_wdr_sns_reg_info_init(vi_pipe, pstSnsState);
                break;

            case WDR_MODE_3To1_LINE:
                cmos_3to1_line_wdr_sns_reg_info_init(vi_pipe, pstSnsState);
                break;

            case WDR_MODE_4To1_LINE:
                cmos_4to1_line_wdr_sns_reg_info_init(vi_pipe, pstSnsState);
                break;

            default:
                break;
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

static HI_S32 cmos_set_image_mode_VGA(VI_PIPE vi_pipe, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode,
    HI_U8 *pu8SensorImageMode)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstSensorImageMode);
    CMOS_CHECK_POINTER(pu8SensorImageMode);
    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
        if (pstSensorImageMode->f32Fps <= 240) { /* 240fps */
            *pu8SensorImageMode = MN34220_VGA_240FPS_MODE;
        } else {
            SNS_ERR_TRACE("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n", pstSensorImageMode->u16Width,
                          pstSensorImageMode->u16Height, pstSensorImageMode->f32Fps, pstSnsState->enWDRMode);
            return HI_FAILURE;
        }
    } else {
        SNS_ERR_TRACE("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n", pstSensorImageMode->u16Width,
                      pstSensorImageMode->u16Height, pstSensorImageMode->f32Fps, pstSnsState->enWDRMode);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_VOID cmos_set_image_mode_720P_error(ISP_SNS_STATE_S *pstSnsState,
    ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode)
{
    SNS_ERR_TRACE("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
                  pstSensorImageMode->u16Width,
                  pstSensorImageMode->u16Height,
                  pstSensorImageMode->f32Fps,
                  pstSnsState->enWDRMode);
}
static HI_S32 cmos_set_image_mode_720P(VI_PIPE vi_pipe, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode,
    HI_U8 *pu8SensorImageMode)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    CMOS_CHECK_POINTER(pstSensorImageMode);
    CMOS_CHECK_POINTER(pu8SensorImageMode);
    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);
    switch (pstSnsState->enWDRMode) {
        case WDR_MODE_NONE:
            if (pstSensorImageMode->f32Fps <= 120) { /* 120fps */
                *pu8SensorImageMode = MN34220_720P_120FPS_MODE;
            } else {
                cmos_set_image_mode_720P_error(pstSnsState, pstSensorImageMode);
                return HI_FAILURE;
            }
            if (pstSensorImageMode->f32Fps <= 30) { /* 30fps */
                *pu8SensorImageMode = MN34220_720P_30FPS_MODE;
            }
            break;
        case WDR_MODE_2To1_LINE:
            if (pstSensorImageMode->f32Fps <= 30) { /* 30fps */
                *pu8SensorImageMode = MN34220_720P_30FPS_MODE;
            } else {
                SNS_ERR_TRACE("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
                              pstSensorImageMode->u16Width, pstSensorImageMode->u16Height,
                              pstSensorImageMode->f32Fps, pstSnsState->u8ImgMode);
                return HI_FAILURE;
            }
            break;
        case WDR_MODE_3To1_LINE:
            if (pstSensorImageMode->f32Fps > 20) { /* 20fps */
                cmos_set_image_mode_720P_error(pstSnsState, pstSensorImageMode);
                return HI_FAILURE;
            }
            *pu8SensorImageMode = MN34220_720P_30FPS_MODE;
            break;
        case WDR_MODE_4To1_LINE:
            if (pstSensorImageMode->f32Fps > 15) { /* 15fps */
                cmos_set_image_mode_720P_error(pstSnsState, pstSensorImageMode);
                return HI_FAILURE;
            }
            *pu8SensorImageMode = MN34220_720P_30FPS_MODE;
            break;
        default:
            cmos_set_image_mode_720P_error(pstSnsState, pstSensorImageMode);
            return HI_FAILURE;
            break;
    }
    return HI_SUCCESS;
}

static HI_S32 cmos_set_image_mode_1080P(VI_PIPE vi_pipe,
    ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode,
    HI_U8 *pu8SensorImageMode)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstSensorImageMode);
    CMOS_CHECK_POINTER(pu8SensorImageMode);
    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    if ((pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) ||
        (pstSnsState->enWDRMode == WDR_MODE_3To1_LINE) ||
        (pstSnsState->enWDRMode == WDR_MODE_4To1_LINE)) {
        if (pstSensorImageMode->f32Fps <= 30) { /* 30fps */
            *pu8SensorImageMode = MN34220_1080P_30FPS_MODE;
        } else {
            SNS_ERR_TRACE("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
                          pstSensorImageMode->u16Width,
                          pstSensorImageMode->u16Height,
                          pstSensorImageMode->f32Fps,
                          pstSnsState->enWDRMode);
            return HI_FAILURE;
        }
    } else {
        if (pstSensorImageMode->f32Fps <= 30) { /* 30fps */
            *pu8SensorImageMode = MN34220_1080P_30FPS_MODE;
        } else if (pstSensorImageMode->f32Fps <= 60) { /* 60fps */
            *pu8SensorImageMode = MN34220_1080P_60FPS_MODE;
        } else {
            SNS_ERR_TRACE("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
                          pstSensorImageMode->u16Width,
                          pstSensorImageMode->u16Height,
                          pstSensorImageMode->f32Fps,
                          pstSnsState->enWDRMode);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 cmos_sensorimage_cal(VI_PIPE vi_pipe, HI_U8 *u8SensorImageMode, ISP_SNS_STATE_S *pstSnsState,
    ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode)
{
    HI_S32 s32Ret;

    if (MN34220_RES_IS_VGA(pstSensorImageMode->u16Width, pstSensorImageMode->u16Height)) {
        s32Ret = cmos_set_image_mode_VGA(vi_pipe, pstSensorImageMode, u8SensorImageMode);
        if (s32Ret == HI_FAILURE) {
            return HI_FAILURE;
        }
        return HI_SUCCESS;
    }
    if (MN34220_RES_IS_720P(pstSensorImageMode->u16Width, pstSensorImageMode->u16Height)) {
        s32Ret = cmos_set_image_mode_720P(vi_pipe, pstSensorImageMode, u8SensorImageMode);
        if (s32Ret == HI_FAILURE) {
            return HI_FAILURE;
        }
        return HI_SUCCESS;
    }
    if (MN34220_RES_IS_1080P(pstSensorImageMode->u16Width, pstSensorImageMode->u16Height)) {
        s32Ret = cmos_set_image_mode_1080P(vi_pipe, pstSensorImageMode, u8SensorImageMode);
        if (s32Ret == HI_FAILURE) {
            return HI_FAILURE;
        }
        return HI_SUCCESS;
    }
    SNS_ERR_TRACE("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
                  pstSensorImageMode->u16Width,
                  pstSensorImageMode->u16Height,
                  pstSensorImageMode->f32Fps,
                  pstSnsState->enWDRMode);
    return HI_FAILURE;
}

static HI_S32 cmos_set_image_mode(VI_PIPE vi_pipe, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode)
{
    HI_U8  u8SensorImageMode;
    HI_S32 ret;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstSensorImageMode);
    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    u8SensorImageMode = pstSnsState->u8ImgMode;
    pstSnsState->bSyncInit = HI_FALSE;

    ret = cmos_sensorimage_cal(vi_pipe, &u8SensorImageMode, pstSnsState, pstSensorImageMode);
    if (ret == HI_FAILURE) {
        return HI_FAILURE;
    }
    /* Sensor first init */
    if (pstSnsState->bInit == HI_FALSE) {
        pstSnsState->u8ImgMode = u8SensorImageMode;
        return HI_SUCCESS;
    }

    /* Switch SensorImageMode */
    if (u8SensorImageMode == pstSnsState->u8ImgMode) {
        /* Don't need to switch SensorImageMode */
        return ISP_DO_NOT_NEED_SWITCH_IMAGEMODE;
    }

    pstSnsState->u8ImgMode = u8SensorImageMode;
    memset_s(pstSnsState->au32WDRIntTime, sizeof(pstSnsState->au32WDRIntTime), 0, sizeof(pstSnsState->au32WDRIntTime));

    return HI_SUCCESS;
}

static HI_VOID sensor_global_init(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    MN34220_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->bInit = HI_FALSE;
    pstSnsState->bSyncInit = HI_FALSE;
    pstSnsState->u8ImgMode = MN34220_1080P_30FPS_MODE;
    pstSnsState->enWDRMode = WDR_MODE_NONE;
    pstSnsState->u32FLStd = MN34220_VMAX_1080P30_LINEAR;
    pstSnsState->au32FL[0] = MN34220_VMAX_1080P30_LINEAR;
    pstSnsState->au32FL[1] = MN34220_VMAX_1080P30_LINEAR;

    memset_s(&pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset_s(&pstSnsState->astRegsInfo[1], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
    return;
}

static HI_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    CMOS_CHECK_POINTER(pstSensorExpFunc);
    memset_s(pstSensorExpFunc, sizeof(ISP_SENSOR_EXP_FUNC_S), 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init = mn34220_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit = mn34220_exit;
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
static HI_S32 mn34220_set_bus_info(VI_PIPE vi_pipe, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunMn34220BusInfo[vi_pipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

    return HI_SUCCESS;
}

static HI_S32 sensor_ctx_init(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    MN34220_SENSOR_GET_CTX(vi_pipe, pastSnsStateCtx);

    if (pastSnsStateCtx == HI_NULL) {
        pastSnsStateCtx = (ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
        if (pastSnsStateCtx == HI_NULL) {
            SNS_ERR_TRACE("Isp[%d] SnsCtx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset_s(pastSnsStateCtx, sizeof(ISP_SNS_STATE_S), 0, sizeof(ISP_SNS_STATE_S));

    MN34220_SENSOR_SET_CTX(vi_pipe, pastSnsStateCtx);

    return HI_SUCCESS;
}

static HI_VOID sensor_ctx_exit(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    MN34220_SENSOR_GET_CTX(vi_pipe, pastSnsStateCtx);
    SENSOR_FREE(pastSnsStateCtx);
    MN34220_SENSOR_RESET_CTX(vi_pipe);
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

    stSnsAttrInfo.eSensorId = MN34220_ID;
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

    s32Ret = HI_MPI_ISP_SensorUnRegCallBack(vi_pipe, MN34220_ID);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AE_SensorUnRegCallBack(vi_pipe, pstAeLib, MN34220_ID);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AWB_SensorUnRegCallBack(vi_pipe, pstAwbLib, MN34220_ID);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function to awb lib failed!\n");
        return s32Ret;
    }

    sensor_ctx_exit(vi_pipe);
    return HI_SUCCESS;
}

ISP_SNS_OBJ_S stSnsMn34220Obj = {
    .pfnRegisterCallback    = sensor_register_callback,
    .pfnUnRegisterCallback  = sensor_unregister_callback,
    .pfnStandby             = mn34220_standby,
    .pfnRestart             = mn34220_restart,
    .pfnMirrorFlip          = HI_NULL,
    .pfnWriteReg            = mn34220_write_register,
    .pfnReadReg             = mn34220_read_register,
    .pfnSetBusInfo          = mn34220_set_bus_info,
    .pfnSetInit             = HI_NULL
};

