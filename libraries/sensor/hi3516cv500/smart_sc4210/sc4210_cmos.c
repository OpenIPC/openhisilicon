/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of sc4210_cmos.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "sc4210_cmos.h"
#include "sc4210_cmos_ex.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "mpi_isp.h"
#include "mpi_ae.h"
#include "mpi_awb.h"

#define SC4210_ID 4210
#define SENSOR_SC4210_WIDTH 2560
#define SENSOR_SC4210_HEIGHT 1440
#define SC4210_FRAME_TYPE_SHORT 0
#define SC4210_FRAME_TYPE_LONG 1
/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/
#define HIGH_8BITS(x) (((x)&0xFF00) >> 8)
#define LOW_8BITS(x) ((x)&0x00FF)
#define LOWER_4BITS(x) ((x)&0x000F)
#define HIGHER_4BITS(x) (((x)&0xF000) >> 12)
#define HIGHER_8BITS(x) (((x)&0x0FF0) >> 4)
#define SC4210_EXP_LOW_4BITS(x) (((x)&0x000F) << 4)
#define SC4210_EXP_MID_8BITS(x) (((x)&0x0FF0) >> 4)
#define SC4210_EXP_HIG_4BITS(x) (((x)&0xF000) >> 12)

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ? (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ? (b) : (a))
#endif
ISP_SNS_STATE_S *g_pastSc4210[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define SC4210_SENSOR_SET_CTX(dev, pstCtx)   ((g_pastSc4210[dev]) = (pstCtx))
#define SC4210_SENSOR_RESET_CTX(dev)         (g_pastSc4210[dev] = HI_NULL)
ISP_SNS_COMMBUS_U g_aunSc4210BusInfo[ISP_MAX_PIPE_NUM] = {
    [0] = { .s8I2cDev = 0},
    [1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1}
};

ISP_SNS_COMMBUS_U *sc4210_get_bus_Info(VI_PIPE vi_pipe)
{
    return &g_aunSc4210BusInfo[vi_pipe];
}

ISP_SNS_STATE_S *sc4210_get_ctx(VI_PIPE vi_pipe)
{
    return g_pastSc4210[vi_pipe];
}

static ISP_FSWDR_MODE_E g_genFSWDRMode[ISP_MAX_PIPE_NUM] = {
    [0 ... ISP_MAX_PIPE_NUM - 1] = ISP_FSWDR_NORMAL_MODE
};

static HI_U32 g_au32MaxTimeGetCnt[ISP_MAX_PIPE_NUM] = {0};
static HI_U32 g_au32InitExposure[ISP_MAX_PIPE_NUM]  = {0};
static HI_U32 g_au32LinesPer500ms[ISP_MAX_PIPE_NUM] = {0};

static HI_U16 g_au16InitWBGain[ISP_MAX_PIPE_NUM][ISP_RGB_CHN_NUM] = {{0}};
static HI_U16 g_au16SampleRgain[ISP_MAX_PIPE_NUM] = {0};
static HI_U16 g_au16SampleBgain[ISP_MAX_PIPE_NUM] = {0};
static HI_BOOL g_quick_start_en[ISP_MAX_PIPE_NUM] = {HI_FALSE};

static HI_U16 g_au16Sc4210VMAX[ISP_MAX_PIPE_NUM] = {0};
static HI_U32 g_au32Sc4210Ratio[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = 64}; /* expo ratio 64 */
static HI_U16 g_au16Sc4210ShortFrameExpMin[ISP_MAX_PIPE_NUM] = {0};

static HI_BOOL g_abAERouteExValid[ISP_MAX_PIPE_NUM] = {0};
static ISP_AE_ROUTE_S g_astInitAERoute[ISP_MAX_PIPE_NUM] = {{0}};
static ISP_AE_ROUTE_EX_S g_astInitAERouteEx[ISP_MAX_PIPE_NUM] = {{0}};
static ISP_AE_ROUTE_S g_astInitAERouteSF[ISP_MAX_PIPE_NUM] = {{0}};
static ISP_AE_ROUTE_EX_S g_astInitAERouteSFEx[ISP_MAX_PIPE_NUM] = {{0}};

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
/* ****Sc4210 Register Address**** */
#define SC4210_EXP_H_ADDR                     0x3e00
#define SC4210_EXP_M_ADDR                     0x3e01
#define SC4210_EXP_L_ADDR                     0x3e02
#define SC4210_AGAIN_H_ADDR                   0x3e08
#define SC4210_AGAIN_L_ADDR                   0x3e09
#define SC4210_DGAIN_H_ADDR                   0x3e06
#define SC4210_DGAIN_L_ADDR                   0x3e07
#define SC4210_VMAX_H_ADDR                    0x320e
#define SC4210_VMAX_L_ADDR                    0x320f
#define SC4210_HMAX_H_ADDR                    0x320c
#define SC4210_HMAX_L_ADDR                    0x320d

#define SC4210_EXP_SHORT_H_ADDR       0x3e04
#define SC4210_EXP_SHORT_L_ADDR       0x3e05
#define SC4210_AGAIN_SHORT_H_ADDR     0x3e12
#define SC4210_AGAIN_SHORT_L_ADDR     0x3e13
#define SC4210_DGAIN_SHORT_H_ADDR     0x3e10
#define SC4210_DGAIN_SHORT_L_ADDR     0x3e11

#define SC4210_MAX_SHORT_EXP_L_ADDR     0x3e24
#define SC4210_MAX_SHORT_EXP_H_ADDR     0x3e23

#define SC4210_VMAX_MAX 0x7FFF
#define SC4210_FULL_LINES_MAX ((SC4210_VMAX_MAX) * 2)
#define SC4210_INCREASE_LINES 0 /* make real fps less than stand fps because NVR require */
#define SC4210_VMAX_1440P30_LINEAR (1500 + SC4210_INCREASE_LINES)
#define SC4210_VMAX_1440P30_WDR (3000 + SC4210_INCREASE_LINES)
#define SC4210_SHORT_FRAME_MAX  0x7FE   /* Register only 12bit max 0xFFF */
#define SC4210_SHORT_FRAME_LIMIT_MAX    0xb86

#define SC4210_REG_VALUE_0X3206_0X3207      0x5af
#define SC4210_REG_VALUE_0X3202_0X3203      0x0

/* sensor fps mode */
#define SC4210_SENSOR_1440P_30FPS_LINEAR_MODE   1
#define SC4210_SENSOR_1440P_30FPS_WDR_MODE   2

#define SC4210_RES_IS_1440P(w, h) ((w) <= 2560 && (h) <= 1440)
static HI_VOID cmos_get_ae_comm_default(VI_PIPE vi_pipe,  AE_SENSOR_DEFAULT_S *pstAeSnsDft,
    ISP_SNS_STATE_S *pstSnsState)
{
    HI_U32 u32Fll = 0;
    HI_U32 U32MaxFps = 0;
    if (pstSnsState->u8ImgMode == SC4210_SENSOR_1440P_30FPS_LINEAR_MODE) {
        u32Fll = SC4210_VMAX_1440P30_LINEAR;
        U32MaxFps = 30;  /* 30fps */
    } else if (pstSnsState->u8ImgMode == SC4210_SENSOR_1440P_30FPS_WDR_MODE) {
        u32Fll = SC4210_VMAX_1440P30_WDR;
        U32MaxFps = 30; /* 30fps */
    }
    pstSnsState->u32FLStd = u32Fll;
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FlickerFreq = 50 * 256; /* light flicker freq: 50Hz, accuracy: 256 */
    pstAeSnsDft->u32FullLinesMax = SC4210_FULL_LINES_MAX;

    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 2; /* accuracy 2 */
    pstAeSnsDft->stIntTimeAccu.f32Offset = 0;

    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 1;

    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 1;

    pstAeSnsDft->u32ISPDgainShift = 8; /* accuracy 8 */
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 2 << pstAeSnsDft->u32ISPDgainShift; /* max 2 */

    pstAeSnsDft->u32HmaxTimes = (1000000) / div_0_to_1(pstSnsState->u32FLStd * 30); /* 1000000 us  30 fps */

    if (g_au32InitExposure[vi_pipe] == 0) {
        pstAeSnsDft->u32InitExposure = 76151; /* init 76151 */
    } else {
        pstAeSnsDft->u32InitExposure = g_au32InitExposure[vi_pipe];
    }

    if (g_au32LinesPer500ms[vi_pipe] == 0) {
        pstAeSnsDft->u32LinesPer500ms = pstSnsState->u32FLStd * U32MaxFps / 2; /* div 2 */
    } else {
        pstAeSnsDft->u32LinesPer500ms = g_au32LinesPer500ms[vi_pipe];
    }

    pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_0;
    pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_32_0;

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

    pstAeSnsDft->u32MaxAgain = 44704; /* max 44704 */
    pstAeSnsDft->u32MinAgain = 1024; /* min 1024 */
    pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
    pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

    pstAeSnsDft->u32MaxDgain = 32256; /* max 32256 */
    pstAeSnsDft->u32MinDgain = 1024; /* min 1024 */
    pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
    pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

    pstAeSnsDft->u8AeCompensation = 50; /* AeCompensation 50 */
    pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;

    pstAeSnsDft->u32InitExposure = g_au32InitExposure[vi_pipe] ? g_au32InitExposure[vi_pipe] : 49216; /* init 49216 */

    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8; /* sub 8 */
    pstAeSnsDft->u32MinIntTime = 1;
    pstAeSnsDft->u32MaxIntTimeTarget = 65535; /* max 65535 */
    pstAeSnsDft->u32MinIntTimeTarget = 1;

    pstAeSnsDft->bAERouteExValid = g_abAERouteExValid[vi_pipe];
    memcpy_s(&pstAeSnsDft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), &g_astInitAERoute[vi_pipe], sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&pstAeSnsDft->stAERouteAttrEx, sizeof(ISP_AE_ROUTE_EX_S),
        &g_astInitAERouteEx[vi_pipe],  sizeof(ISP_AE_ROUTE_EX_S));
    return;
}
static HI_VOID cmos_get_ae_2to1_line_wdr_default(VI_PIPE vi_pipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft,
                                                 ISP_SNS_STATE_S *pstSnsState)
{
    pstAeSnsDft->au8HistThresh[0] = 0xc;
    pstAeSnsDft->au8HistThresh[1] = 0x18;
    pstAeSnsDft->au8HistThresh[2] = 0x60; /* index 2 */
    pstAeSnsDft->au8HistThresh[3] = 0x80; /* index 3 */

    pstAeSnsDft->u32MaxAgain = 44700; /* 44700 */
    pstAeSnsDft->u32MinAgain = 1024; /* 1024 */
    pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
    pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

    pstAeSnsDft->u32MaxDgain = 32256; /* 32256 */
    pstAeSnsDft->u32MinDgain = 1024; /* 1024 */
    pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
    pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

    pstAeSnsDft->u32MaxISPDgainTarget = 16 << pstAeSnsDft->u32ISPDgainShift; /* 16 */

    pstAeSnsDft->u8AeCompensation = 32; /* AeCompensation 32 */

    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 2; /* accuracy 2 */
    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8; /* sub 8 */
    pstAeSnsDft->u32MinIntTime = 2; /* min 2 */
    g_au16Sc4210ShortFrameExpMin[vi_pipe] = pstAeSnsDft->u32MinIntTime;
    pstAeSnsDft->u32MaxIntTimeTarget = 65535; /* max 65535 */
    pstAeSnsDft->u32MinIntTimeTarget = pstAeSnsDft->u32MinIntTime;

    pstAeSnsDft->enAeExpMode = AE_EXP_LOWLIGHT_PRIOR;

    pstAeSnsDft->u32ExpRatioMin = 0x80;  /* The exposure ration from 2x to convergence */

    pstAeSnsDft->u32InitExposure =
        g_au32InitExposure[vi_pipe] ? g_au32InitExposure[vi_pipe] : 112768; /* 112768 */

    if (g_genFSWDRMode[vi_pipe] != ISP_FSWDR_LONG_FRAME_MODE) {
        pstAeSnsDft->u16ManRatioEnable = HI_TRUE;
        pstAeSnsDft->au32Ratio[0] = 0x400;
        pstAeSnsDft->au32Ratio[1] = 0x40;
        pstAeSnsDft->au32Ratio[2] = 0x40; /* array index 2 */
    }
    return;
}

static HI_S32 cmos_get_ae_default(VI_PIPE vi_pipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstAeSnsDft);
    SC4210_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);
    memset_s(&pstAeSnsDft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), 0, sizeof(ISP_AE_ROUTE_S));

    cmos_get_ae_comm_default(vi_pipe, pstAeSnsDft, pstSnsState);

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:   /* linear mode */

            cmos_get_ae_linear_default(vi_pipe, pstAeSnsDft, pstSnsState);
            break;

        case WDR_MODE_2To1_LINE:
            /*  FS WDR mode  */
            cmos_get_ae_2to1_line_wdr_default(vi_pipe, pstAeSnsDft, pstSnsState);

            break;
    }

    return HI_SUCCESS;
}

/* the function of sensor set fps */
static HI_VOID cmos_fps_set(VI_PIPE vi_pipe, HI_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    HI_U32 u32VMAX = SC4210_VMAX_1440P30_LINEAR;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    SC4210_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    switch (pstSnsState->u8ImgMode) {
        case SC4210_SENSOR_1440P_30FPS_LINEAR_MODE:
            if ((f32Fps <= 30) && (f32Fps >= 1.373)) { /* fps range:[1.373, 30) */
                u32VMAX = SC4210_VMAX_1440P30_LINEAR * 30 / SNS_DIV_0_TO_1_FLOAT(f32Fps); /* 30fps */
            } else {
                SNS_ERR_TRACE("Not support Fps: %f\n", f32Fps);
                return;
            }
            pstAeSnsDft->u32LinesPer500ms = SC4210_VMAX_1440P30_LINEAR * 15; /* 15: 30fps / 2 */

            break;

        case SC4210_SENSOR_1440P_30FPS_WDR_MODE:
            if ((f32Fps <= 30) && (f32Fps >= 15.0)) { /* fps range:[15.0, 30) */
                u32VMAX = SC4210_VMAX_1440P30_WDR * 30 / SNS_DIV_0_TO_1_FLOAT(f32Fps); /* 30fps */
            } else {
                SNS_ERR_TRACE("Not support Fps: %f\n", f32Fps);
                return;
            }
            pstAeSnsDft->u32LinesPer500ms = SC4210_VMAX_1440P30_WDR * 15; /* 15: 30fps / 2 */
            break;

        default:
            break;
    }

    u32VMAX = (u32VMAX > SC4210_VMAX_MAX) ? SC4210_VMAX_MAX : u32VMAX;

    g_au16Sc4210VMAX[vi_pipe] = u32VMAX;

    pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = LOW_8BITS(u32VMAX); /* index 7 */
    pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = HIGH_8BITS(u32VMAX); /* index 8 */

    pstSnsState->u32FLStd = ((u32VMAX << 1) > SC4210_FULL_LINES_MAX) ? SC4210_FULL_LINES_MAX : (u32VMAX << 1);

    pstSnsState->u32FLStd = u32VMAX;

    pstAeSnsDft->f32Fps = f32Fps;
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8; /* sub 8 */
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];

    return;
}
static HI_VOID cmos_slow_framerate_set(VI_PIPE vi_pipe, HI_U32 u32FullLines, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    HI_U32 u32VMAX = 0;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    SC4210_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        u32VMAX = u32FullLines;
        u32VMAX = (u32VMAX > (SC4210_VMAX_1440P30_WDR * 2)) ? (SC4210_VMAX_1440P30_WDR * 2) : u32VMAX; /* x2 */
        /* is 3 */
        pstSnsState->au32FL[0] = u32VMAX;
    } else {
        u32VMAX = u32FullLines;
        u32VMAX = (u32VMAX > SC4210_VMAX_MAX) ? SC4210_VMAX_MAX : u32VMAX;
        pstSnsState->au32FL[0] = u32VMAX;
    }

    pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = LOW_8BITS(u32VMAX); /* index 7 */
    pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = HIGH_8BITS(u32VMAX); /* index 8 */

    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
    pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 8; /* sub 8 */

    return;
}

static HI_VOID cmos_inttime_update_linear(VI_PIPE vi_pipe, HI_U32 u32IntTime)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    SC4210_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32IntTime = u32IntTime << 1;
    pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = SC4210_EXP_LOW_4BITS(u32IntTime);
    pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = SC4210_EXP_MID_8BITS(u32IntTime);
    pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = SC4210_EXP_HIG_4BITS(u32IntTime); /* index 2 */

    return;
}

static HI_VOID coms_2to1_line_update_reg(ISP_SNS_STATE_S *pstSnsState, HI_U16 u16LongExp,
                                         HI_U16 u16ShortExp, HI_U16 u16ShortExpMax)
{
    pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = SC4210_EXP_LOW_4BITS(u16LongExp);
    pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = SC4210_EXP_MID_8BITS(u16LongExp);
    pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = SC4210_EXP_HIG_4BITS(u16LongExp); /* index 2 */
    pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = SC4210_EXP_LOW_4BITS(u16ShortExp); /* index 9 */
    pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = SC4210_EXP_MID_8BITS(u16ShortExp); /* index 10 */
    pstSnsState->astRegsInfo[0].astI2cData[16].u32Data = LOW_8BITS(u16ShortExpMax); /* index 16 */
    pstSnsState->astRegsInfo[0].astI2cData[17].u32Data = HIGH_8BITS(u16ShortExpMax); /* index 17 */
}

static HI_VOID cmos_inttime_update_2to1_line_long(VI_PIPE vi_pipe, HI_U32 u32IntTime)
{
    HI_U16 u16LongExpMax, u16LongExpMax_Tmp, u16ShortExpMax, u16LongExp, u16ShortExp;

    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    SC4210_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->au32WDRIntTime[1] = u32IntTime;
    u16ShortExpMax = (pstSnsState->au32FL[0] - 8) * 64 / (g_au32Sc4210Ratio[vi_pipe] + 64); /* sub 8, x64 */
    u16LongExpMax = pstSnsState->au32FL[0] - 8 - u16ShortExpMax; /* sub 8 */
    u16LongExpMax_Tmp = pstSnsState->au32FL[0] - u16ShortExpMax - 5; /* 5 limit from sensor */
    u16LongExpMax = (u16LongExpMax > u16LongExpMax_Tmp) ? u16LongExpMax_Tmp : u16LongExpMax;
    u16LongExpMax = u16LongExpMax - (u16LongExpMax % 2); /* % 2 */

    /* down use even number */
    u16ShortExpMax = u16ShortExpMax - (u16ShortExpMax % 2); /* % 2 */

    if (pstSnsState->au32WDRIntTime[0] > (u16ShortExpMax - 4)) { /* 4 limit from sensor */
        pstSnsState->au32WDRIntTime[0] = u16ShortExpMax - 4; /* 4 limit from sensor */
    }

    pstSnsState->au32WDRIntTime[1] = (pstSnsState->au32WDRIntTime[1] > u16LongExpMax) ?
                                      u16LongExpMax : pstSnsState->au32WDRIntTime[1];

    if (pstSnsState->au32WDRIntTime[0] > 2) { /* min 2 */
        pstSnsState->au32WDRIntTime[1] = pstSnsState->au32WDRIntTime[1] - (pstSnsState->au32WDRIntTime[1] % 2); /* %2 */
        pstSnsState->au32WDRIntTime[0] = pstSnsState->au32WDRIntTime[0] - (pstSnsState->au32WDRIntTime[0] % 2); /* %2 */
    }

    if (pstSnsState->au32WDRIntTime[0] > pstSnsState->au32WDRIntTime[1]) {
        pstSnsState->au32WDRIntTime[0] = pstSnsState->au32WDRIntTime[1];
    }

    u16LongExp =  pstSnsState->au32WDRIntTime[1] << 1;
    u16ShortExp = pstSnsState->au32WDRIntTime[0] << 1;
    u16ShortExp = (u16ShortExp > 0xFFF) ? 0xFFF : u16ShortExp;
    u16ShortExpMax = (u16ShortExpMax > SC4210_SHORT_FRAME_LIMIT_MAX) ? SC4210_SHORT_FRAME_LIMIT_MAX : u16ShortExpMax;

    coms_2to1_line_update_reg(pstSnsState, u16LongExp, u16ShortExp, u16ShortExpMax);

    return;
}

static HI_VOID cmos_inttime_update_2to1_line(VI_PIPE vi_pipe, HI_U32 u32IntTime)
{
    static HI_U8 au8ExpFrameType[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = SC4210_FRAME_TYPE_SHORT};

    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    SC4210_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (au8ExpFrameType[vi_pipe] == SC4210_FRAME_TYPE_SHORT) { /* short exposure */
        pstSnsState->au32WDRIntTime[0] = u32IntTime;

        au8ExpFrameType[vi_pipe] = SC4210_FRAME_TYPE_LONG;
    } else { /* long exposure */
        cmos_inttime_update_2to1_line_long(vi_pipe, u32IntTime);

        au8ExpFrameType[vi_pipe] = SC4210_FRAME_TYPE_SHORT;
    }

    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static HI_VOID cmos_inttime_update(VI_PIPE vi_pipe, HI_U32 u32IntTime)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    SC4210_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        cmos_inttime_update_2to1_line(vi_pipe, u32IntTime);
    } else {
        cmos_inttime_update_linear(vi_pipe, u32IntTime);
    }

    return;
}
#define SC4210_AGAIN_NODE_NUM               344
#define SC4210_AGAIN_ADDR_INDEX_NODE_NUM    344
#define SC4210_DGAIN_NODE_NUM               160
#define SC4210_DGAIN_ADDR_INDEX_NODE_NUM    160
static HI_U32 g_again_table[SC4210_AGAIN_NODE_NUM] = {
    1024,  1040,  1056,  1072,  1088,  1104,  1120,  1136,  1152,  1168,  1184,  1200,  1216,  1232,  1248,  1264,
    1280,  1296,  1312,  1328,  1344,  1360,  1376,  1392,  1408,  1424,  1440,  1456,  1472,  1488,  1504,  1520,
    1536,  1552,  1568,  1584,  1600,  1616,  1632,  1648,  1664,  1680,  1696,  1712,  1728,  1744,  1760,  1776,
    1792,  1808,  1824,  1840,  1856,  1872,  1888,  1904,  1920,  1936,  1952,  1968,  1984,  2000,  2016,  2032,
    2048,  2080,  2112,  2144,  2176,  2208,  2240,  2272,  2304,  2336,  2368,  2400,  2432,  2464,  2496,  2528,
    2560,  2592,  2624,  2656,  2688,  2720,  2752,  2784,  2816,  2860,  2904,  2948,  2992,  3036,  3080,  3124,
    3168,  3212,  3256,  3300,  3344,  3388,  3432,  3476,  3520,  3563,  3608,  3652,  3696,  3740,  3784,  3828,
    3872,  3916,  3960,  4004,  4048,  4092,  4136,  4180,  4224,  4268,  4312,  4356,  4400,  4444,  4488,  4532,
    4576,  4620,  4664,  4708,  4752,  4796,  4840,  4884,  4928,  4972,  5016,  5060,  5104,  5148,  5192,  5236,
    5280,  5324,  5368,  5412,  5456,  5500,  5544,  5588,  5632,  5720,  5808,  5896,  5984,  6072,  6160,  6248,
    6336,  6424,  6512,  6600,  6688,  6776,  6864,  6952,  7040,  7128,  7216,  7304,  7392,  7480,  7568,  7656,
    7744,  7832,  7920,  8008,  8096,  8184,  8272,  8360,  8448,  8567,  8624,  8712,  8800,  8888,  8976,  9064,
    9152,  9240,  9328,  9416,  9504,  9592,  9680,  9768,  9856,  9944,  10032, 10120, 10208, 10296, 10384, 10472,
    10560, 10648, 10736, 10824, 10912, 11000, 11088, 11176, 11264, 11440, 11616, 11792, 11968, 12144, 12320, 12496,
    12672, 12848, 13024, 13200, 13376, 13552, 13728, 13904, 14080, 14256, 14432, 14608, 14784, 14969, 15136, 15312,
    15488, 15664, 15840, 16016, 16192, 16368, 16544, 16720, 16896, 17072, 17248, 17424, 17600, 17776, 17952, 18128,
    18304, 18480, 18656, 18832, 19008, 19184, 19360, 19536, 19712, 19888, 20064, 20240, 20416, 20592, 20768, 20944,
    21120, 21296, 21472, 21648, 21824, 22000, 22176, 22352, 22528, 22880, 23232, 23584, 23936, 24362, 24640, 24992,
    25344, 25696, 26048, 26400, 26752, 27104, 27456, 27808, 28160, 28512, 28864, 29216, 29568, 29920, 30272, 30624,
    30976, 31328, 31680, 32032, 32384, 32736, 33088, 33550, 33792, 34144, 34496, 34848, 35200, 35552, 35904, 36256,
    36608, 36960, 37312, 37664, 38016, 38368, 38720, 39072, 39424, 39776, 40128, 40480, 40832, 41184, 41536, 41888,
    42240, 42592, 42944, 43296, 43648, 44000, 44352, 44704
};

static HI_U32 g_again_addrindex_table[SC4210_AGAIN_ADDR_INDEX_NODE_NUM] = {
    0x0340, 0x0341, 0x0342, 0x0343, 0x0344, 0x0345, 0x0346, 0x0347, 0x0348, 0x0349, 0x034a, 0x034b, 0x034c, 0x034d,
    0x034e, 0x034f, 0x0350, 0x0351, 0x0352, 0x0353, 0x0354, 0x0355, 0x0356, 0x0357, 0x0358, 0x0359, 0x035a, 0x035b,
    0x035c, 0x035d, 0x035e, 0x035f, 0x0360, 0x0361, 0x0362, 0x0363, 0x0364, 0x0365, 0x0366, 0x0367, 0x0368, 0x0369,
    0x036a, 0x036b, 0x036c, 0x036d, 0x036e, 0x036f, 0x0370, 0x0371, 0x0372, 0x0373, 0x0374, 0x0375, 0x0376, 0x0377,
    0x0378, 0x0379, 0x037a, 0x037b, 0x037c, 0x037d, 0x037e, 0x037f, 0x0740, 0x0741, 0x0742, 0x0743, 0x0744, 0x0745,
    0x0746, 0x0747, 0x0748, 0x0749, 0x074a, 0x074b, 0x074c, 0x074d, 0x074e, 0x074f, 0x0750, 0x0751, 0x0752, 0x0753,
    0x0754, 0x0755, 0x0756, 0x0757, 0x2340, 0x2341, 0x2342, 0x2343, 0x2344, 0x2345, 0x2346, 0x2347, 0x2348, 0x2349,
    0x234a, 0x234b, 0x234c, 0x234d, 0x234e, 0x234f, 0x2350, 0x2351, 0x2352, 0x2353, 0x2354, 0x2355, 0x2356, 0x2357,
    0x2358, 0x2359, 0x235a, 0x235b, 0x235c, 0x235d, 0x235e, 0x235f, 0x2360, 0x2361, 0x2362, 0x2363, 0x2364, 0x2365,
    0x2366, 0x2367, 0x2368, 0x2369, 0x236a, 0x236b, 0x236c, 0x236d, 0x236e, 0x236f, 0x2370, 0x2371, 0x2372, 0x2373,
    0x2374, 0x2375, 0x2376, 0x2377, 0x2378, 0x2379, 0x237a, 0x237b, 0x237c, 0x237d, 0x237e, 0x237f, 0x2740, 0x2741,
    0x2742, 0x2743, 0x2744, 0x2745, 0x2746, 0x2747, 0x2748, 0x2749, 0x274a, 0x274b, 0x274c, 0x274d, 0x274e, 0x274f,
    0x2750, 0x2751, 0x2752, 0x2753, 0x2754, 0x2755, 0x2756, 0x2757, 0x2758, 0x2759, 0x275a, 0x275b, 0x275c, 0x275d,
    0x275e, 0x275f, 0x2760, 0x2761, 0x2762, 0x2763, 0x2764, 0x2765, 0x2766, 0x2767, 0x2768, 0x2769, 0x276a, 0x276b,
    0x276c, 0x276d, 0x276e, 0x276f, 0x2770, 0x2771, 0x2772, 0x2773, 0x2774, 0x2775, 0x2776, 0x2777, 0x2778, 0x2779,
    0x277a, 0x277b, 0x277c, 0x277d, 0x277e, 0x277f, 0x2f40, 0x2f41, 0x2f42, 0x2f43, 0x2f44, 0x2f45, 0x2f46, 0x2f47,
    0x2f48, 0x2f49, 0x2f4a, 0x2f4b, 0x2f4c, 0x2f4d, 0x2f4e, 0x2f4f, 0x2f50, 0x2f51, 0x2f52, 0x2f53, 0x2f54, 0x2f55,
    0x2f56, 0x2f57, 0x2f58, 0x2f59, 0x2f5a, 0x2f5b, 0x2f5c, 0x2f5d, 0x2f5e, 0x2f5f, 0x2f60, 0x2f61, 0x2f62, 0x2f63,
    0x2f64, 0x2f65, 0x2f66, 0x2f67, 0x2f68, 0x2f69, 0x2f6a, 0x2f6b, 0x2f6c, 0x2f6d, 0x2f6e, 0x2f6f, 0x2f70, 0x2f71,
    0x2f72, 0x2f73, 0x2f74, 0x2f75, 0x2f76, 0x2f77, 0x2f78, 0x2f79, 0x2f7a, 0x2f7b, 0x2f7c, 0x2f7d, 0x2f7e, 0x2f7f,
    0x3f40, 0x3f41, 0x3f42, 0x3f43, 0x3f44, 0x3f45, 0x3f46, 0x3f47, 0x3f48, 0x3f49, 0x3f4a, 0x3f4b, 0x3f4c, 0x3f4d,
    0x3f4e, 0x3f4f, 0x3f50, 0x3f51, 0x3f52, 0x3f53, 0x3f54, 0x3f55, 0x3f56, 0x3f57, 0x3f58, 0x3f59, 0x3f5a, 0x3f5b,
    0x3f5c, 0x3f5d, 0x3f5e, 0x3f5f, 0x3f60, 0x3f61, 0x3f62, 0x3f63, 0x3f64, 0x3f65, 0x3f66, 0x3f67, 0x3f68, 0x3f69,
    0x3f6a, 0x3f6b, 0x3f6c, 0x3f6d, 0x3f6e, 0x3f6f, 0x3f70, 0x3f71, 0x3f72, 0x3f73, 0x3f74, 0x3f75, 0x3f76, 0x3f77,
    0x3f78, 0x3f79, 0x3f7a, 0x3f7b, 0x3f7c, 0x3f7d, 0x3f7e, 0x3f7f
};

static HI_U32 g_dgain_table[SC4210_DGAIN_NODE_NUM] = {
    1024,  1056,  1088,  1120,  1152,  1184,  1216,  1248,  1280,  1312,  1344,  1376,  1408,  1440,  1472,  1504,
    1536,  1568,  1600,  1632,  1664,  1696,  1728,  1760,  1792,  1824,  1856,  1888,  1920,  1952,  1984,  2016,
    2048,  2112,  2176,  2240,  2304,  2368,  2432,  2496,  2560,  2624,  2688,  2752,  2816,  2880,  2944,  3008,
    3072,  3136,  3200,  3264,  3328,  3392,  3456,  3520,  3584,  3648,  3712,  3776,  3840,  3904,  3968,  4032,
    4096,  4224,  4352,  4480,  4608,  4736,  4864,  4992,  5120,  5248,  5376,  5504,  5632,  5760,  5888,  6016,
    6144,  6272,  6400,  6528,  6656,  6784,  6912,  7040,  7168,  7296,  7424,  7552,  7680,  7808,  7936,  8064,
    8192,  8448,  8704,  8960,  9216,  9472,  9728,  9984,  10240, 10496, 10752, 11008, 11264, 11520, 11776, 12032,
    12288, 12544, 12800, 13056, 13312, 13568, 13824, 14080, 14336, 14592, 14848, 15104, 15360, 15616, 15872, 16128,
    16384, 16896, 17408, 17920, 18432, 18944, 19456, 19968, 20480, 20992, 21504, 22016, 22528, 23040, 23552, 24064,
    24576, 25088, 25600, 26112, 26624, 27136, 27648, 28160, 28672, 29184, 29696, 30208, 30720, 31232, 31744, 32256
};

static HI_U32 g_dgain_addrindex_table[SC4210_DGAIN_ADDR_INDEX_NODE_NUM] = {
    0x080, 0x084, 0x088, 0x08c, 0x090, 0x094, 0x098, 0x09c, 0x0a0, 0x0a4, 0x0a8, 0x0ac, 0x0b0, 0x0b4, 0x0b8, 0x0bc,
    0x0c0, 0x0c4, 0x0c8, 0x0cc, 0x0d0, 0x0d4, 0x0d8, 0x0dc, 0x0e0, 0x0e4, 0x0e8, 0x0ec, 0x0f0, 0x0f4, 0x0f8, 0x0fc,
    0x180, 0x184, 0x188, 0x18c, 0x190, 0x194, 0x198, 0x19c, 0x1a0, 0x1a4, 0x1a8, 0x1ac, 0x1b0, 0x1b4, 0x1b8, 0x1bc,
    0x1c0, 0x1c4, 0x1c8, 0x1cc, 0x1d0, 0x1d4, 0x1d8, 0x1dc, 0x1e0, 0x1e4, 0x1e8, 0x1ec, 0x1f0, 0x1f4, 0x1f8, 0x1fc,
    0x380, 0x384, 0x388, 0x38c, 0x390, 0x394, 0x398, 0x39c, 0x3a0, 0x3a4, 0x3a8, 0x3ac, 0x3b0, 0x3b4, 0x3b8, 0x3bc,
    0x3c0, 0x3c4, 0x3c8, 0x3cc, 0x3d0, 0x3d4, 0x3d8, 0x3dc, 0x3e0, 0x3e4, 0x3e8, 0x3ec, 0x3f0, 0x3f4, 0x3f8, 0x3fc,
    0x780, 0x784, 0x788, 0x78c, 0x790, 0x794, 0x798, 0x79c, 0x7a0, 0x7a4, 0x7a8, 0x7ac, 0x7b0, 0x7b4, 0x7b8, 0x7bc,
    0x7c0, 0x7c4, 0x7c8, 0x7cc, 0x7d0, 0x7d4, 0x7d8, 0x7dc, 0x7e0, 0x7e4, 0x7e8, 0x7ec, 0x7f0, 0x7f4, 0x7f8, 0x7fc,
    0xf80, 0xf84, 0xf88, 0xf8c, 0xf90, 0xf94, 0xf98, 0xf9c, 0xfa0, 0xfa4, 0xfa8, 0xfac, 0xfb0, 0xfb4, 0xfb8, 0xfbc,
    0xfc0, 0xfc4, 0xfc8, 0xfcc, 0xfd0, 0xfd4, 0xfd8, 0xfdc, 0xfe0, 0xfe4, 0xfe8, 0xfec, 0xff0, 0xff4, 0xff8, 0xffc
};

static HI_VOID cmos_again_calc_table(VI_PIPE vi_pipe, HI_U32 *pu32AgainLin, HI_U32 *pu32AgainDb)
{
    HI_S32 i;

    CMOS_CHECK_POINTER_VOID(pu32AgainLin);
    CMOS_CHECK_POINTER_VOID(pu32AgainDb);

    if (*pu32AgainLin >= g_again_table[SC4210_AGAIN_NODE_NUM - 1]) {
        *pu32AgainLin = g_again_table[SC4210_AGAIN_NODE_NUM - 1];
        *pu32AgainDb = g_again_addrindex_table[SC4210_AGAIN_NODE_NUM - 1];
        return;
    }

    for (i = 1; i < SC4210_AGAIN_NODE_NUM; i++) {
        if (*pu32AgainLin < g_again_table[i]) {
            *pu32AgainLin = g_again_table[i - 1];
            *pu32AgainDb = g_again_addrindex_table[i - 1];
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

    if (*pu32DgainLin >= g_dgain_table[SC4210_DGAIN_NODE_NUM - 1]) {
        *pu32DgainLin = g_dgain_table[SC4210_DGAIN_NODE_NUM - 1];
        *pu32DgainDb = g_dgain_addrindex_table[SC4210_DGAIN_NODE_NUM - 1];
        return;
    }

    for (i = 1; i < SC4210_DGAIN_NODE_NUM; i++) {
        if (*pu32DgainLin < g_dgain_table[i]) {
            *pu32DgainLin = g_dgain_table[i - 1];
            *pu32DgainDb = g_dgain_addrindex_table[i - 1];
            break;
        }
    }
    return;
}
static HI_VOID cmos_gains_update(VI_PIPE vi_pipe, HI_U32 u32Again, HI_U32 u32Dgain)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    SC4210_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = LOW_8BITS(u32Again); /* index 3 */
    pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = HIGH_8BITS(u32Again); /* index 4 */
    pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = LOW_8BITS(u32Dgain); /* index 5 */
    pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = HIGH_8BITS(u32Dgain); /* index 6 */

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = LOW_8BITS(u32Again); /* index 12 */
        pstSnsState->astRegsInfo[0].astI2cData[13].u32Data = HIGH_8BITS(u32Again); /* index 13 */

        pstSnsState->astRegsInfo[0].astI2cData[14].u32Data = LOW_8BITS(u32Dgain); /* index 14 */
        pstSnsState->astRegsInfo[0].astI2cData[15].u32Data = HIGH_8BITS(u32Dgain); /* index 15 */
    }

    return;
}
static HI_VOID cmos_get_inttime_max_2to1_line(VI_PIPE vi_pipe,
    HI_U32 *au32Ratio, HI_U32 *au32IntTimeMax,
    HI_U32 *au32IntTimeMin, HI_U32 *pu32LFMaxIntTime)
{
    HI_U32 u32IntTimeLongPreUnit, u32IntTimeShortCurUnit, u32IntTimeMaxTmp, u32ShortTimeMinLimit;;

    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    SC4210_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32ShortTimeMinLimit = g_au16Sc4210ShortFrameExpMin[vi_pipe];

    g_au32Sc4210Ratio[vi_pipe] = *au32Ratio;
    if (g_genFSWDRMode[vi_pipe] == ISP_FSWDR_LONG_FRAME_MODE) {
        u32IntTimeLongPreUnit = pstSnsState->au32FL[1] - 6 - pstSnsState->au32WDRIntTime[0]; /* sub 6 */
        u32IntTimeShortCurUnit = pstSnsState->au32FL[0] - 10; /* sub 10 */
        u32IntTimeMaxTmp = (u32IntTimeLongPreUnit < u32IntTimeShortCurUnit) ?
                           u32IntTimeLongPreUnit : u32IntTimeShortCurUnit;
        au32IntTimeMax[1] = u32IntTimeMaxTmp;
        au32IntTimeMin[1] = u32ShortTimeMinLimit;
        return;
    }

    u32IntTimeLongPreUnit = ((pstSnsState->au32FL[1] - 8 - pstSnsState->au32WDRIntTime[0]) * 0x40) / /* sub 8 */
        SNS_DIV_0_TO_1(au32Ratio[0]);

    u32IntTimeShortCurUnit = ((pstSnsState->au32FL[0] - 8) * 0x40) / SNS_DIV_0_TO_1(au32Ratio[0] + 0x40); /* sub 8 */
    u32IntTimeMaxTmp = (u32IntTimeLongPreUnit < u32IntTimeShortCurUnit) ?
        u32IntTimeLongPreUnit : u32IntTimeShortCurUnit;  /* Use the min */
    u32IntTimeMaxTmp = (u32IntTimeMaxTmp == 0) ? 1 : u32IntTimeMaxTmp;

    if (u32IntTimeMaxTmp >= u32ShortTimeMinLimit) {
        if (u32IntTimeMaxTmp > SC4210_SHORT_FRAME_MAX) {
            au32IntTimeMin[0] = u32ShortTimeMinLimit;
            au32IntTimeMax[0] = SC4210_SHORT_FRAME_MAX;
            au32IntTimeMin[1] = (au32IntTimeMin[0] * au32Ratio[0]) >> 6; /* shift 6 */
            au32IntTimeMax[1] = (au32IntTimeMax[0] * au32Ratio[0]) >> 6; /* shift 6 */
            return;
        }
        au32IntTimeMax[0] = u32IntTimeMaxTmp;
        au32IntTimeMax[1] = (au32IntTimeMax[0] * au32Ratio[0]) >> 6; /* shift 6 */
        au32IntTimeMin[0] = u32ShortTimeMinLimit;
        au32IntTimeMin[1] = (au32IntTimeMin[0] * au32Ratio[0]) >> 6; /* shift 6 */
        return;
    }

    au32IntTimeMax[0] = u32IntTimeMaxTmp;
    au32IntTimeMax[1] = (au32IntTimeMax[0] * 0xFFF) >> 6; /* shift 6 */
    au32IntTimeMin[0] = au32IntTimeMax[0];
    au32IntTimeMin[1] = au32IntTimeMax[1];
    return;
}

static HI_VOID cmos_get_inttime_max(VI_PIPE vi_pipe,
                                    HI_U16 u16ManRatioEnable,
                                    HI_U32 *au32Ratio,
                                    HI_U32 *au32IntTimeMax,
                                    HI_U32 *au32IntTimeMin,
                                    HI_U32 *pu32LFMaxIntTime)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER_VOID(au32Ratio);
    CMOS_CHECK_POINTER_VOID(au32IntTimeMax);
    CMOS_CHECK_POINTER_VOID(au32IntTimeMin);
    CMOS_CHECK_POINTER_VOID(pu32LFMaxIntTime);
    SC4210_SENSOR_GET_CTX(vi_pipe, pstSnsState);
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

    return;
}

static HI_S32 cmos_init_ae_exp_function(AE_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    CMOS_CHECK_POINTER(pstExpFuncs);

    memset_s(pstExpFuncs, sizeof(AE_SENSOR_EXP_FUNC_S), 0, sizeof(AE_SENSOR_EXP_FUNC_S));

    pstExpFuncs->pfn_cmos_get_ae_default     = cmos_get_ae_default;
    pstExpFuncs->pfn_cmos_fps_set            = cmos_fps_set;
    pstExpFuncs->pfn_cmos_slow_framerate_set = cmos_slow_framerate_set;
    pstExpFuncs->pfn_cmos_inttime_update     = cmos_inttime_update;
    pstExpFuncs->pfn_cmos_gains_update       = cmos_gains_update;
    pstExpFuncs->pfn_cmos_again_calc_table   = cmos_again_calc_table;
    pstExpFuncs->pfn_cmos_dgain_calc_table   = cmos_dgain_calc_table;
    pstExpFuncs->pfn_cmos_get_inttime_max    = cmos_get_inttime_max;
    pstExpFuncs->pfn_cmos_ae_fswdr_attr_set  = cmos_ae_fswdr_attr_set;

    return HI_SUCCESS;
}

/* awb static param for Fuji Lens New IR_Cut */
#define CALIBRATE_STATIC_TEMP       4950
#define CALIBRATE_STATIC_WB_R_GAIN  404
#define CALIBRATE_STATIC_WB_GR_GAIN 256
#define CALIBRATE_STATIC_WB_GB_GAIN 256
#define CALIBRATE_STATIC_WB_B_GAIN  405

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1 (-10)
#define CALIBRATE_AWB_P2 253
#define CALIBRATE_AWB_Q1 (-13)
#define CALIBRATE_AWB_A1 217851
#define CALIBRATE_AWB_B1 128
#define CALIBRATE_AWB_C1 (-162714)

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN                          0
#define GOLDEN_BGAIN                          0
static HI_S32 cmos_get_awb_default(VI_PIPE vi_pipe, AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstAwbSnsDft);
    SC4210_SENSOR_GET_CTX(vi_pipe, pstSnsState);
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
        case SC4210_SENSOR_1440P_30FPS_LINEAR_MODE:
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
    pstDef->pstAntiFalseColor        = &g_stIspAntiFalseColor;
    pstDef->unKey.bit1Ldci           = 0;
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
    memcpy_s(&pstDef->stNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S), &g_stIspNoiseCalibration,
             sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
    return;
}

static void cmos_get_isp_2to1_line_wdr_default(ISP_CMOS_DEFAULT_S *pstDef)
{
    pstDef->unKey.bit1Clut           = 1;
    pstDef->pstClut                  = &g_stIspCLUT;
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
    pstDef->pstLsc                   = &g_stCmosLsc;
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
    SC4210_SENSOR_GET_CTX(vi_pipe, pstSnsState);
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
    pstDef->stSensorMode.u32SensorID = SC4210_ID;
    pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

    cmos_get_isp_dng_default(pstSnsState, pstDef);

    return HI_SUCCESS;
}

static HI_S32 cmos_get_isp_black_level(VI_PIPE vi_pipe, ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
    HI_S32  i;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstBlackLevel);
    SC4210_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    /* Don't need to update black level when iso change */
    pstBlackLevel->bUpdate = HI_FALSE;

    /* black level of linear mode */
    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        pstBlackLevel->au16BlackLevel[i] = 0x110;
    }

    return HI_SUCCESS;
}
static HI_VOID cmos_set_pixel_detect(VI_PIPE vi_pipe, HI_BOOL bEnable)
{
    HI_U32 u32FullLines_5Fps, u32MaxIntTime_5Fps;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    SC4210_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        SNS_ERR_TRACE("Please use linear mode to do defetc pixel detect!\n");
        return;
    }

    u32FullLines_5Fps = (SC4210_VMAX_1440P30_LINEAR * 30) / 5; /* 30fps, 5fps */
    u32MaxIntTime_5Fps = pstSnsState->u32FLStd - 8; /* sub 8 */
    if (bEnable) { /* setup for ISP pixel calibration mode */
        sc4210_write_register(vi_pipe, SC4210_AGAIN_L_ADDR, 0x10);
        sc4210_write_register(vi_pipe, SC4210_AGAIN_H_ADDR, 0x00);
        sc4210_write_register(vi_pipe, SC4210_DGAIN_L_ADDR, 0x80);
        sc4210_write_register(vi_pipe, SC4210_DGAIN_H_ADDR, 0x00);
        sc4210_write_register(vi_pipe, SC4210_EXP_L_ADDR,  LOWER_4BITS(u32MaxIntTime_5Fps) << 4); /* shift 4 */
        sc4210_write_register(vi_pipe, SC4210_EXP_M_ADDR,  HIGHER_8BITS(u32MaxIntTime_5Fps));
        sc4210_write_register(vi_pipe, SC4210_EXP_H_ADDR,  HIGHER_4BITS(u32MaxIntTime_5Fps));
        sc4210_write_register(vi_pipe, SC4210_VMAX_L_ADDR, LOW_8BITS(u32FullLines_5Fps));
        sc4210_write_register(vi_pipe, SC4210_VMAX_H_ADDR, HIGH_8BITS(u32FullLines_5Fps));
    } else { /* setup for ISP 'normal mode' */
        pstSnsState->u32FLStd = (pstSnsState->u32FLStd >
            ((SC4210_FULL_LINES_MAX) * 2)) ? ((SC4210_FULL_LINES_MAX) * 2) : pstSnsState->u32FLStd; /* x2 */
        sc4210_write_register(vi_pipe, SC4210_VMAX_L_ADDR, (pstSnsState->u32FLStd & 0x00F) << 4); /* shift 4 */
        sc4210_write_register(vi_pipe, SC4210_VMAX_H_ADDR, (pstSnsState->u32FLStd & 0xFF0) >> 4); /* shift 4 */
        pstSnsState->bSyncInit = HI_FALSE;
    }

    return;
}

static HI_S32 cmos_set_wdr_mode(VI_PIPE vi_pipe, HI_U8 u8Mode)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    SC4210_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    pstSnsState->bSyncInit = HI_FALSE;

    switch (u8Mode & 0x3F) {
        case WDR_MODE_NONE:
            pstSnsState->enWDRMode = WDR_MODE_NONE;
            SNS_ERR_TRACE("linear mode\n");
            break;

        case WDR_MODE_2To1_LINE:
            pstSnsState->enWDRMode = WDR_MODE_2To1_LINE;
            SNS_ERR_TRACE("wdr mode\n");
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
    pstSnsState->astRegsInfo[0].unComBus.s8I2cDev = g_aunSc4210BusInfo[vi_pipe].s8I2cDev;
    pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2; /* DelayMax 2 */
    pstSnsState->astRegsInfo[0].u32RegNum = 9; /* RegNum 9 */

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        pstSnsState->astRegsInfo[0].u32RegNum = 18; /* RegNum 18 */
    }

    for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
        pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
        pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr = SC4210_I2C_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[i].u32AddrByteNum = SC4210_ADDR_BYTE;
        pstSnsState->astRegsInfo[0].astI2cData[i].u32DataByteNum = SC4210_DATA_BYTE;
    }

    /* Linear Mode Regs */
    pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
    pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr = SC4210_EXP_L_ADDR;
    pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
    pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr = SC4210_EXP_M_ADDR;
    pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0; /* index 2 */
    pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr = SC4210_EXP_H_ADDR; /* index 2 */

    pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 0; /* index 3 */
    pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr = SC4210_AGAIN_L_ADDR; /* index 3 */
    pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 0; /* index 4 */
    pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr = SC4210_AGAIN_H_ADDR; /* index 4 */

    pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0; /* index 5 */
    pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr = SC4210_DGAIN_L_ADDR; /* index 5 */
    pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0; /* index 6 */
    pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr = SC4210_DGAIN_H_ADDR; /* index 6 */

    pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 1; /* index 7 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr = SC4210_VMAX_L_ADDR; /* index 7 */
    pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 1; /* index 8 */
    pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr = SC4210_VMAX_H_ADDR; /* index 8 */
    return;
}
static HI_VOID cmos_2to1_line_wdr_sns_reg_info_init(VI_PIPE vi_pipe, ISP_SNS_STATE_S *pstSnsState)
{
    pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 0; /* index 3 */
    pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 0; /* index 4 */

    pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum = 0; /* index 9 */
    pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr = SC4210_EXP_SHORT_L_ADDR; /* index 9 */
    pstSnsState->astRegsInfo[0].astI2cData[10].u8DelayFrmNum = 0; /* index 10 */
    pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr = SC4210_EXP_SHORT_H_ADDR; /* index 10 */

    pstSnsState->astRegsInfo[0].astI2cData[12].u8DelayFrmNum = 1; /* index 12 */
    pstSnsState->astRegsInfo[0].astI2cData[12].u32RegAddr = SC4210_AGAIN_SHORT_L_ADDR; /* index 12 */
    pstSnsState->astRegsInfo[0].astI2cData[13].u8DelayFrmNum = 1; /* index 13 */
    pstSnsState->astRegsInfo[0].astI2cData[13].u32RegAddr = SC4210_AGAIN_SHORT_H_ADDR; /* index 13 */

    pstSnsState->astRegsInfo[0].astI2cData[14].u8DelayFrmNum = 1; /* index 14 */
    pstSnsState->astRegsInfo[0].astI2cData[14].u32RegAddr = SC4210_DGAIN_SHORT_L_ADDR; /* index 14 */
    pstSnsState->astRegsInfo[0].astI2cData[15].u8DelayFrmNum = 1; /* index 15 */
    pstSnsState->astRegsInfo[0].astI2cData[15].u32RegAddr = SC4210_DGAIN_SHORT_H_ADDR; /* index 15 */

    pstSnsState->astRegsInfo[0].astI2cData[16].u8DelayFrmNum = 1; /* index 16 */
    pstSnsState->astRegsInfo[0].astI2cData[16].u32RegAddr = SC4210_MAX_SHORT_EXP_L_ADDR; /* index 16 */
    pstSnsState->astRegsInfo[0].astI2cData[17].u8DelayFrmNum = 1; /* index 17 */
    pstSnsState->astRegsInfo[0].astI2cData[17].u32RegAddr = SC4210_MAX_SHORT_EXP_H_ADDR; /* index 17 */
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
    SC4210_SENSOR_GET_CTX(vi_pipe, pstSnsState);
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
    SC4210_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    u8SensorImageMode = pstSnsState->u8ImgMode;
    pstSnsState->bSyncInit = HI_FALSE;

    if (pstSensorImageMode->f32Fps <= 30) { /* 30fps */
        switch (pstSnsState->enWDRMode) {
            case WDR_MODE_NONE:
                if (SC4210_RES_IS_1440P(pstSensorImageMode->u16Width, pstSensorImageMode->u16Height)) {
                    u8SensorImageMode = SC4210_SENSOR_1440P_30FPS_LINEAR_MODE;
                    pstSnsState->u32FLStd = SC4210_VMAX_1440P30_LINEAR;
                } else {
                    SNS_ERR_TRACE("Linear Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
                                  pstSensorImageMode->u16Width, pstSensorImageMode->u16Height,
                                  pstSensorImageMode->f32Fps, pstSnsState->enWDRMode);
                    return HI_FAILURE;
                }
                break;
            case WDR_MODE_2To1_LINE:
                if (SC4210_RES_IS_1440P(pstSensorImageMode->u16Width, pstSensorImageMode->u16Height)) {
                    u8SensorImageMode = SC4210_SENSOR_1440P_30FPS_WDR_MODE;
                    pstSnsState->u32FLStd = SC4210_VMAX_1440P30_WDR;
                } else {
                    SNS_ERR_TRACE("WDR Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
                                  pstSensorImageMode->u16Width, pstSensorImageMode->u16Height,
                                  pstSensorImageMode->f32Fps, pstSnsState->enWDRMode);
                    return HI_FAILURE;
                }
                break;
            default:
                SNS_ERR_TRACE("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n", pstSensorImageMode->u16Width,
                              pstSensorImageMode->u16Height, pstSensorImageMode->f32Fps, pstSnsState->enWDRMode);
                return HI_FAILURE;
                break;
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

    SC4210_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->bInit = HI_FALSE;
    pstSnsState->bSyncInit = HI_FALSE;
    pstSnsState->u8ImgMode = SC4210_SENSOR_1440P_30FPS_LINEAR_MODE;
    pstSnsState->enWDRMode = WDR_MODE_NONE;
    pstSnsState->u32FLStd  = SC4210_VMAX_1440P30_LINEAR;
    pstSnsState->au32FL[0] = SC4210_VMAX_1440P30_LINEAR;
    pstSnsState->au32FL[1] = SC4210_VMAX_1440P30_LINEAR;

    memset_s(&pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset_s(&pstSnsState->astRegsInfo[1], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
    return;
}

static HI_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    CMOS_CHECK_POINTER(pstSensorExpFunc);

    memset_s(pstSensorExpFunc, sizeof(ISP_SENSOR_EXP_FUNC_S), 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init = sc4210_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit = sc4210_exit;
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
static HI_S32 sc4210_set_bus_info(VI_PIPE vi_pipe, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunSc4210BusInfo[vi_pipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

    return HI_SUCCESS;
}

static HI_S32 sensor_ctx_init(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    SC4210_SENSOR_GET_CTX(vi_pipe, pastSnsStateCtx);

    if (pastSnsStateCtx == HI_NULL) {
        pastSnsStateCtx = (ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
        if (pastSnsStateCtx == HI_NULL) {
            SNS_ERR_TRACE("Isp[%d] SnsCtx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset_s(pastSnsStateCtx, sizeof(ISP_SNS_STATE_S), 0, sizeof(ISP_SNS_STATE_S));

    SC4210_SENSOR_SET_CTX(vi_pipe, pastSnsStateCtx);
    return HI_SUCCESS;
}

static HI_VOID sensor_ctx_exit(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    SC4210_SENSOR_GET_CTX(vi_pipe, pastSnsStateCtx);
    SENSOR_FREE(pastSnsStateCtx);
    SC4210_SENSOR_RESET_CTX(vi_pipe);
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

    stSnsAttrInfo.eSensorId = SC4210_ID;
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

    s32Ret = HI_MPI_ISP_SensorUnRegCallBack(vi_pipe, SC4210_ID);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AE_SensorUnRegCallBack(vi_pipe, pstAeLib, SC4210_ID);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AWB_SensorUnRegCallBack(vi_pipe, pstAwbLib, SC4210_ID);
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

    g_au32InitExposure[vi_pipe] = pstInitAttr->u32Exposure;
    g_au32LinesPer500ms[vi_pipe] = pstInitAttr->u32LinesPer500ms;
    g_au16InitWBGain[vi_pipe][0] = pstInitAttr->u16WBRgain; /* 0: Rgain */
    g_au16InitWBGain[vi_pipe][1] = pstInitAttr->u16WBGgain; /* 1: Ggain */
    g_au16InitWBGain[vi_pipe][2] = pstInitAttr->u16WBBgain; /* 2: Bgain */
    g_au16SampleRgain[vi_pipe] = pstInitAttr->u16SampleRgain;
    g_au16SampleBgain[vi_pipe] = pstInitAttr->u16SampleBgain;
    g_quick_start_en[vi_pipe] = pstInitAttr->stQuickStart.bQuickStartEn;

    g_abAERouteExValid[vi_pipe] = pstInitAttr->bAERouteExValid;
    memcpy_s(&g_astInitAERoute[vi_pipe], sizeof(ISP_AE_ROUTE_S), &pstInitAttr->stAERoute, sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&g_astInitAERouteEx[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S), &pstInitAttr->stAERouteEx,
             sizeof(ISP_AE_ROUTE_EX_S));
    memcpy_s(&g_astInitAERouteSF[vi_pipe], sizeof(ISP_AE_ROUTE_S), &pstInitAttr->stAERouteSF, sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&g_astInitAERouteSFEx[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S), &pstInitAttr->stAERouteSFEx,
             sizeof(ISP_AE_ROUTE_EX_S));

    return HI_SUCCESS;
}

ISP_SNS_OBJ_S stSnsSc4210Obj = {
    .pfnRegisterCallback    = sensor_register_callback,
    .pfnUnRegisterCallback  = sensor_unregister_callback,
    .pfnStandby             = sc4210_standby,
    .pfnRestart             = sc4210_restart,
    .pfnMirrorFlip          = HI_NULL,
    .pfnWriteReg            = sc4210_write_register,
    .pfnReadReg             = sc4210_read_register,
    .pfnSetBusInfo          = sc4210_set_bus_info,
    .pfnSetInit             = sensor_set_init
};

