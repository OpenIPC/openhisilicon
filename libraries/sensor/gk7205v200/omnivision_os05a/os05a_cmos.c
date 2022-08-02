/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#if !defined(__OS05A_CMOS_H_)
#define __OS05A_CMOS_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "comm_sns.h"
#include "comm_video.h"
#include "sns_ctrl.h"
#include "gk_api_isp.h"
#include "gk_api_ae.h"
#include "gk_api_awb.h"

#include "os05a_cmos_ex.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */


#define OS05A_ID                          05
#define HIGH_8BITS(x) (((x) & 0xff00) >> 8)
#define LOW_8BITS(x)  ((x) & 0x00ff)

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ?  (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ?  (b) : (a))
#endif

ISP_SNS_STATE_S *g_pastOs05a[ISP_MAX_PIPE_NUM] = {GK_NULL};

#define OS05A_SENSOR_GET_CTX(dev, pstCtx)   (pstCtx = g_pastOs05a[dev])
#define OS05A_SENSOR_SET_CTX(dev, pstCtx)   (g_pastOs05a[dev] = pstCtx)
#define OS05A_SENSOR_RESET_CTX(dev)         (g_pastOs05a[dev] = GK_NULL)

ISP_SNS_COMMBUS_U g_aunOs05aBusInfo[ISP_MAX_PIPE_NUM] = {
    [0] = { .s8I2cDev = 0},
    [1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1}
};

static ISP_FSWDR_MODE_E genFSWDRMode[ISP_MAX_PIPE_NUM] = {
    [0 ... ISP_MAX_PIPE_NUM - 1] = ISP_FSWDR_NORMAL_MODE
};

static GK_U32 gu32MaxTimeGetCnt[ISP_MAX_PIPE_NUM] = {0};

static GK_U32 g_au32InitExposure[ISP_MAX_PIPE_NUM]  = {0};
static GK_U32 g_au32LinesPer500ms[ISP_MAX_PIPE_NUM] = {0};
static GK_U16 g_au16InitWBGain[ISP_MAX_PIPE_NUM][3] = {{0}};
static GK_U16 g_au16SampleRgain[ISP_MAX_PIPE_NUM] = {0};
static GK_U16 g_au16SampleBgain[ISP_MAX_PIPE_NUM] = {0};

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
extern const unsigned int os05a_i2c_addr;
extern unsigned int os05a_addr_byte;
extern unsigned int os05a_data_byte;

extern void os05a_init(VI_PIPE ViPipe);
extern void os05a_exit(VI_PIPE ViPipe);
extern void os05a_standby(VI_PIPE ViPipe);
extern void os05a_restart(VI_PIPE ViPipe);
extern int  os05a_write_register(VI_PIPE ViPipe, int addr, int data);
extern int  os05a_read_register(VI_PIPE ViPipe, int addr);

#define OS05A_FULL_LINES_MAX              (0x9240)

#define VMAX_ADDR_H                       (0x380E)
#define VMAX_ADDR_L                       (0x380F)
#define AGAIN_H                           (0x3508)
#define AGAIN_L                           (0x3509)
#define DGAIN_H                           (0x350A)
#define DGAIN_L                           (0x350B)
#define SHORT_AGAIN_H                     (0x350C)
#define SHORT_AGAIN_L                     (0x350D)
#define SHORT_DGAIN_H                     (0x350E)
#define SHORT_DGAIN_L                     (0x350F)
#define EXPOSURE_TIME_LONG_H              (0x3501)
#define EXPOSURE_TIME_LONG_L              (0x3502)
#define EXPOSURE_TIME_SHORT_H             (0x3511)
#define EXPOSURE_TIME_SHORT_L             (0x3512)
#define LINE_LEN_PCK                      (0x300C)

#define INCREASE_LINES                    (5) /* make real fps less than stand fps because NVR require */
#define OS05A_VMAX_5M30_12bit_LINEAR      (0x921+INCREASE_LINES)
#define OS05A_VMAX_5M30_10bit_WDR         (0x9c0+INCREASE_LINES)

#define Os05a_5M_2688x1944_12bit_linear30 (0)
#define Os05a_5M_2688x1944_10bit_wdr30    (1)

static GK_U32 gu32STimeFps = 30;
static GK_FLOAT gf32AGain = 0;
static GK_FLOAT gf32DGain = 0;
#define OS05A_FRAME_TYPE_SHORT            1
#define OS05A_FRAME_TYPE_LONG             0
#define OS05A_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState)\
    do{\
        ISP_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n", \
                  pstSensorImageMode->u16Width,  \
                  pstSensorImageMode->u16Height, \
                  pstSensorImageMode->f32Fps,    \
                  pstSnsState->enWDRMode);\
    }while(0)

static GK_S32 cmos_get_ae_default(VI_PIPE ViPipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;
    GK_U32 u32Fll = 0;
    GK_FLOAT f32maxFps = 0;

    CMOS_CHECK_POINTER(pstAeSnsDft);
    OS05A_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset(&pstAeSnsDft->stAERouteAttr, 0, sizeof(ISP_AE_ROUTE_S));

    if (pstSnsState->u8ImgMode == Os05a_5M_2688x1944_12bit_linear30) {
        u32Fll = OS05A_VMAX_5M30_12bit_LINEAR;
        f32maxFps = 30;
    } else if (pstSnsState->u8ImgMode == Os05a_5M_2688x1944_10bit_wdr30) {
        u32Fll = OS05A_VMAX_5M30_10bit_WDR;
        f32maxFps = 30;
    }

    pstSnsState->u32FLStd = u32Fll;
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FlickerFreq = 0;

    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
    pstAeSnsDft->u32MaxIntTime = (pstSnsState->u32FLStd >= 12) ? (pstSnsState->u32FLStd - 12) : (0);
    pstAeSnsDft->stIntTimeAccu.f32Offset = 0;

    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 0.0078125;

    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 0.0009765625;

    pstAeSnsDft->u32MaxAgain = 2047;
    pstAeSnsDft->u32MinAgain = 128;
    pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
    pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

    pstAeSnsDft->u32MaxDgain = 16383;
    pstAeSnsDft->u32MinDgain = 1024;
    pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
    pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

    pstAeSnsDft->u32ISPDgainShift = 8;
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 9 << pstAeSnsDft->u32ISPDgainShift;

    if (g_au32InitExposure[ViPipe] == 0) {
        pstAeSnsDft->u32InitExposure = 130000;
    } else {
        pstAeSnsDft->u32InitExposure = g_au32InitExposure[ViPipe];
    }

    if (g_au32LinesPer500ms[ViPipe] == 0) {
        pstAeSnsDft->u32LinesPer500ms = ((GK_U64)(u32Fll * f32maxFps)) >> 1;
    } else {
        pstAeSnsDft->u32LinesPer500ms = g_au32LinesPer500ms[ViPipe];
    }


    pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_0;
    pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_32_0;

    pstAeSnsDft->bAERouteExValid = GK_FALSE;
    pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
    pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:   /* linear mode */
            pstAeSnsDft->au8HistThresh[0] = 0xd;
            pstAeSnsDft->au8HistThresh[1] = 0x28;
            pstAeSnsDft->au8HistThresh[2] = 0x60;
            pstAeSnsDft->au8HistThresh[3] = 0x80;
            pstAeSnsDft->u8AeCompensation = 0x38;
            pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8;
            pstAeSnsDft->u32MinIntTime = 2;
            pstAeSnsDft->u32MaxIntTimeTarget = 65535;
            pstAeSnsDft->u32MinIntTimeTarget = 2;

            break;

        case WDR_MODE_2To1_LINE:
            pstAeSnsDft->au8HistThresh[0] = 0xC;
            pstAeSnsDft->au8HistThresh[1] = 0x18;
            pstAeSnsDft->au8HistThresh[2] = 0x60;
            pstAeSnsDft->au8HistThresh[3] = 0x80;
            pstAeSnsDft->u8AeCompensation = 0x38;
            pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8;
            pstAeSnsDft->u32MinIntTime = 2;
            pstAeSnsDft->u32MaxIntTimeTarget = 65535;
            pstAeSnsDft->u32MinIntTimeTarget = 2;
            pstAeSnsDft->u32LFMinExposure = 4500000;
            pstAeSnsDft->bDiffGainSupport = GK_TRUE;
            if (genFSWDRMode[ViPipe] != ISP_FSWDR_LONG_FRAME_MODE) {
                pstAeSnsDft->enAeExpMode = AE_EXP_LOWLIGHT_PRIOR;
                pstAeSnsDft->u16ManRatioEnable = GK_TRUE;
                pstAeSnsDft->au32Ratio[0] = 0x100;
                pstAeSnsDft->au32Ratio[1] = 0x40;
                pstAeSnsDft->au32Ratio[2] = 0x40;
            }
            break;
    }

    return GK_SUCCESS;
}

/* the function of sensor set fps */
static GK_VOID cmos_fps_set(VI_PIPE ViPipe, GK_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;
    GK_FLOAT f32maxFps;
    GK_U32 u32Lines;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    OS05A_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    switch (pstSnsState->u8ImgMode) {
        case Os05a_5M_2688x1944_12bit_linear30:
            if ((f32Fps <= 30) && (f32Fps >= 1.87)) {
                f32maxFps = 30;
                u32Lines = OS05A_VMAX_5M30_12bit_LINEAR * f32maxFps / f32Fps;
                pstAeSnsDft->u32LinesPer500ms = OS05A_VMAX_5M30_12bit_LINEAR * 15;
            } else {
                printf("Not support Fps: %f\n", f32Fps);
                return;
            }
            break;

        case Os05a_5M_2688x1944_10bit_wdr30:
            if ((f32Fps <= 30) && (f32Fps >= 2.0)) {
                f32maxFps = 30;
                u32Lines = OS05A_VMAX_5M30_10bit_WDR * f32maxFps / f32Fps;
                pstAeSnsDft->u32LinesPer500ms = OS05A_VMAX_5M30_10bit_WDR * 15;
            } else {
                printf("Not support Fps: %f\n", f32Fps);
                return;
            }
            break;

        default:
            printf("Not support this Mode\n");
            return;
            break;
    }

    pstSnsState->u32FLStd = u32Lines;
    pstSnsState->u32FLStd = (pstSnsState->u32FLStd > OS05A_FULL_LINES_MAX) ? OS05A_FULL_LINES_MAX : pstSnsState->u32FLStd;

    pstAeSnsDft->f32Fps = f32Fps;

    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8;
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];

    if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
        pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = LOW_8BITS(pstSnsState->au32FL[0]);
        pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = HIGH_8BITS(pstSnsState->au32FL[0]);
    } else {
        pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = LOW_8BITS(pstSnsState->au32FL[0]);
        pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = HIGH_8BITS(pstSnsState->au32FL[0]);
    }

    return;
}

static GK_VOID cmos_slow_framerate_set(VI_PIPE ViPipe, GK_U32 u32FullLines, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    OS05A_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->au32FL[0] = (u32FullLines > OS05A_FULL_LINES_MAX) ? OS05A_FULL_LINES_MAX : u32FullLines;
    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
    pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 8;

    if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
        pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = LOW_8BITS(pstSnsState->au32FL[0]);
        pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = HIGH_8BITS(pstSnsState->au32FL[0]);
    } else {
        pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = LOW_8BITS(pstSnsState->au32FL[0]);
        pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = HIGH_8BITS(pstSnsState->au32FL[0]);
    }

    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static GK_VOID cmos_inttime_update(VI_PIPE ViPipe, GK_U32 u32IntTime)
{
    static GK_BOOL bFirst = GK_TRUE;
    static GK_U32 u32ShortIntTime = 0;
    static GK_U32 u32LongIntTime = 0;

    GK_FLOAT fCGain = 0;
    GK_U32 u32Again = 0;
    GK_U32 u32Dgain = 0;

    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    OS05A_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);


    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        if (bFirst) { /* short exposure */
            pstSnsState->au32WDRIntTime[0] = u32IntTime;
            u32ShortIntTime = u32IntTime;
            bFirst = GK_FALSE;
        } else { /* long exposure */
            pstSnsState->au32WDRIntTime[1] = u32IntTime;
            u32LongIntTime = u32IntTime;
            if (gu32STimeFps == 30) {
                if ((u32LongIntTime >= 750) && (u32ShortIntTime < 750) && ((u32LongIntTime + 750) < 2450) && (gf32AGain >= 11)) {
                    fCGain = gf32AGain * gf32DGain * u32ShortIntTime / 750;
                    if (fCGain < 16) {
                        u32Again = fCGain * 128;
                        u32Dgain = 1024;
                    } else if (fCGain >= 16) {
                        u32Again = 2047;
                        u32Dgain = (1024 * (fCGain * 128)) / 2047;
                    }
                    u32ShortIntTime = 750;
                    pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = LOW_8BITS(u32Again);
                    pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = ((u32Again & 0x3F00) >> 8);
                    pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = LOW_8BITS(u32Dgain);
                    pstSnsState->astRegsInfo[0].astI2cData[13].u32Data = ((u32Dgain & 0x3F00) >> 8);
                } else {
                }
            }
            pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = LOW_8BITS(u32IntTime);
            pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = HIGH_8BITS(u32IntTime);
            pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = LOW_8BITS(u32ShortIntTime);
            pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = HIGH_8BITS(u32ShortIntTime);

            bFirst = GK_TRUE;
        }
    } else {
        pstSnsState->astRegsInfo[0].astI2cData[2].u32Data =  LOW_8BITS(u32IntTime);
        pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = HIGH_8BITS(u32IntTime);
    }

    return;
}

static GK_VOID cmos_gains_update(VI_PIPE ViPipe, GK_U32 u32Again, GK_U32 u32Dgain)
{
    static GK_U8 u8ExpFrame = OS05A_FRAME_TYPE_LONG;
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;
    OS05A_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        if (u8ExpFrame == OS05A_FRAME_TYPE_LONG) {
            pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = LOW_8BITS(u32Again);
            pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = ((u32Again & 0x3F00) >> 8);
            pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = LOW_8BITS(u32Dgain);
            pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = ((u32Dgain & 0x3F00) >> 8);
            u8ExpFrame = OS05A_FRAME_TYPE_SHORT;
        } else if (u8ExpFrame == OS05A_FRAME_TYPE_SHORT) {
            pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = LOW_8BITS(u32Again);
            pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = ((u32Again & 0x3F00) >> 8);
            pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = LOW_8BITS(u32Dgain);
            pstSnsState->astRegsInfo[0].astI2cData[13].u32Data = ((u32Dgain & 0x3F00) >> 8);
            u8ExpFrame = OS05A_FRAME_TYPE_LONG;
        } else {
        }
    }

    else {
        pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = LOW_8BITS(u32Again);
        pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = ((u32Again & 0x3F00) >> 8);
        pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = LOW_8BITS(u32Dgain);
        pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = ((u32Dgain & 0x3F00) >> 8);
    }

    return;
}

/* Only used in WDR_MODE_2To1_LINE and WDR_MODE_2To1_FRAME mode */
static GK_VOID cmos_get_inttime_max(VI_PIPE ViPipe, GK_U16 u16ManRatioEnable, GK_U32 *au32Ratio, GK_U32 *au32IntTimeMax, GK_U32 *au32IntTimeMin, GK_U32 *pu32LFMaxIntTime)
{
    GK_U32 i = 0;
    GK_U32 u32IntTimeMaxTmp = 0;
    GK_U32 u32IntTimeMaxTmp1 = 0;
    GK_U32 u32RatioTmp = 0x40;
    GK_U32 u32ShortTimeMinLimit = 7;

    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER_VOID(au32Ratio);
    CMOS_CHECK_POINTER_VOID(au32IntTimeMax);
    CMOS_CHECK_POINTER_VOID(au32IntTimeMin);
    CMOS_CHECK_POINTER_VOID(pu32LFMaxIntTime);
    OS05A_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32ShortTimeMinLimit =  7;


    if ((pstSnsState->enWDRMode == WDR_MODE_2To1_LINE)) {
        if (genFSWDRMode[ViPipe] == ISP_FSWDR_LONG_FRAME_MODE) { /* long frame mode enable */
            if (gu32MaxTimeGetCnt[ViPipe] > 15) {
                u32IntTimeMaxTmp = pstSnsState->au32FL[0] - 50 - pstSnsState->au32WDRIntTime[0];
                u32IntTimeMaxTmp1 = pstSnsState->au32FL[0] - 50;
            } else {
                u32IntTimeMaxTmp = ((pstSnsState->au32FL[1] - 50 - pstSnsState->au32WDRIntTime[0]) << 6) / DIV_0_TO_1(au32Ratio[0]);
                u32IntTimeMaxTmp1 = (pstSnsState->au32FL[0] - 50) * 0x40 / (au32Ratio[0] + 0x40);
            }

            u32IntTimeMaxTmp = (u32IntTimeMaxTmp < u32IntTimeMaxTmp1) ? u32IntTimeMaxTmp : u32IntTimeMaxTmp1;
            gu32MaxTimeGetCnt[ViPipe]++;
        } else if (genFSWDRMode[ViPipe] == ISP_FSWDR_AUTO_LONG_FRAME_MODE) {
            if (pstSnsState->au32WDRIntTime[0] == u32ShortTimeMinLimit && au32Ratio[0] == 0x40) {
                if (gu32MaxTimeGetCnt[ViPipe] > 0) {
                    u32IntTimeMaxTmp = pstSnsState->au32FL[0] - 50 - pstSnsState->au32WDRIntTime[0];
                    u32IntTimeMaxTmp1 = pstSnsState->au32FL[0] - 50;
                } else {
                    u32IntTimeMaxTmp = ((pstSnsState->au32FL[1] - 50 - pstSnsState->au32WDRIntTime[0]) << 6) / DIV_0_TO_1(au32Ratio[0]);
                    u32IntTimeMaxTmp1 = (pstSnsState->au32FL[0] - 50) * 0x40 / (au32Ratio[0] + 0x40);
                }

                u32IntTimeMaxTmp = (u32IntTimeMaxTmp < u32IntTimeMaxTmp1) ? u32IntTimeMaxTmp : u32IntTimeMaxTmp1;
                *pu32LFMaxIntTime = u32IntTimeMaxTmp;
                gu32MaxTimeGetCnt[ViPipe]++;
            } else {
                gu32MaxTimeGetCnt[ViPipe] = 0;
                u32IntTimeMaxTmp = ((pstSnsState->au32FL[1] - 50 - pstSnsState->au32WDRIntTime[0]) << 6) / DIV_0_TO_1(au32Ratio[0]);
                u32IntTimeMaxTmp1 = (pstSnsState->au32FL[0] - 50) * 0x40 / (au32Ratio[0] + 0x40);
                u32IntTimeMaxTmp = (u32IntTimeMaxTmp <  u32IntTimeMaxTmp1) ? u32IntTimeMaxTmp :  u32IntTimeMaxTmp1;
                *pu32LFMaxIntTime = u32IntTimeMaxTmp1;
            }
        } else {
            u32IntTimeMaxTmp = ((pstSnsState->au32FL[1] - 50 - pstSnsState->au32WDRIntTime[0]) << 6) / DIV_0_TO_1(au32Ratio[0]);
            u32IntTimeMaxTmp1 = (pstSnsState->au32FL[0] - 50) * 0x40 / (au32Ratio[0] + 0x40);
            u32IntTimeMaxTmp = (u32IntTimeMaxTmp <  u32IntTimeMaxTmp1) ? u32IntTimeMaxTmp :  u32IntTimeMaxTmp1;
        }
    }

    if (u32IntTimeMaxTmp >= u32ShortTimeMinLimit) {
        if (IS_LINE_WDR_MODE(pstSnsState->enWDRMode)) {
            au32IntTimeMax[0] = u32IntTimeMaxTmp;
            au32IntTimeMax[1] = au32IntTimeMax[0] * au32Ratio[0] >> 6;
            au32IntTimeMax[2] = au32IntTimeMax[1] * au32Ratio[1] >> 6;
            au32IntTimeMax[3] = au32IntTimeMax[2] * au32Ratio[2] >> 6;
            au32IntTimeMin[0] = u32ShortTimeMinLimit;
            au32IntTimeMin[1] = au32IntTimeMin[0] * au32Ratio[0] >> 6;
            au32IntTimeMin[2] = au32IntTimeMin[1] * au32Ratio[1] >> 6;
            au32IntTimeMin[3] = au32IntTimeMin[2] * au32Ratio[2] >> 6;
        } else {
        }
    } else {
        if (u16ManRatioEnable == 1) {
            printf("Manaul ExpRatio is too large!\n");
            return;
        } else {
            u32IntTimeMaxTmp = u32ShortTimeMinLimit;

            if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
                for (i = 0x40; i <= 0xFFF; i++) {
                    if ((u32IntTimeMaxTmp + (u32IntTimeMaxTmp * i >> 6)) > (pstSnsState->au32FL[0] - 50)) {
                        u32RatioTmp = i - 1;
                        break;
                    }
                }
                au32IntTimeMax[0] = u32IntTimeMaxTmp;
                au32IntTimeMax[1] = au32IntTimeMax[0] * u32RatioTmp >> 6;
            } else {
            }
            au32IntTimeMin[0] = au32IntTimeMax[0];
            au32IntTimeMin[1] = au32IntTimeMax[1];
            au32IntTimeMin[2] = au32IntTimeMax[2];
            au32IntTimeMin[3] = au32IntTimeMax[3];
        }
    }
    return;
}

/* Only used in FSWDR mode */
static GK_VOID cmos_ae_fswdr_attr_set(VI_PIPE ViPipe, AE_FSWDR_ATTR_S *pstAeFSWDRAttr)
{
    genFSWDRMode[ViPipe] = pstAeFSWDRAttr->enFSWDRMode;
    gu32MaxTimeGetCnt[ViPipe] = 0;
}

static GK_S32 cmos_init_ae_exp_function(AE_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    CMOS_CHECK_POINTER(pstExpFuncs);

    memset(pstExpFuncs, 0, sizeof(AE_SENSOR_EXP_FUNC_S));

    pstExpFuncs->pfn_cmos_get_ae_default    = cmos_get_ae_default;
    pstExpFuncs->pfn_cmos_fps_set           = cmos_fps_set;
    pstExpFuncs->pfn_cmos_slow_framerate_set = cmos_slow_framerate_set;
    pstExpFuncs->pfn_cmos_inttime_update    = cmos_inttime_update;
    pstExpFuncs->pfn_cmos_gains_update      = cmos_gains_update;
    pstExpFuncs->pfn_cmos_again_calc_table  = NULL;
    pstExpFuncs->pfn_cmos_dgain_calc_table  = NULL;
    pstExpFuncs->pfn_cmos_get_inttime_max   = cmos_get_inttime_max;
    pstExpFuncs->pfn_cmos_ae_fswdr_attr_set = cmos_ae_fswdr_attr_set;

    return GK_SUCCESS;
}

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN                      0
#define GOLDEN_BGAIN                      0
static GK_S32 cmos_get_awb_default(VI_PIPE ViPipe, AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstAwbSnsDft);
    OS05A_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));

    pstAwbSnsDft->u16WbRefTemp = 4850;

    pstAwbSnsDft->au16GainOffset[0] = 0x21E;
    pstAwbSnsDft->au16GainOffset[1] = 0x100;
    pstAwbSnsDft->au16GainOffset[2] = 0x100;
    pstAwbSnsDft->au16GainOffset[3] = 0x1D8;

    pstAwbSnsDft->as32WbPara[0] = -13;
    pstAwbSnsDft->as32WbPara[1] = 269;
    pstAwbSnsDft->as32WbPara[2] = 0;
    pstAwbSnsDft->as32WbPara[3] = 154243;
    pstAwbSnsDft->as32WbPara[4] = 128;
    pstAwbSnsDft->as32WbPara[5] = -105138;

    pstAwbSnsDft->u16GoldenRgain = GOLDEN_RGAIN;
    pstAwbSnsDft->u16GoldenBgain = GOLDEN_BGAIN;

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:
            memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcm, sizeof(AWB_CCM_S));
            memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTable, sizeof(AWB_AGC_TABLE_S));
            break;
        case WDR_MODE_2To1_LINE:
            memcpy(&pstAwbSnsDft->stCcm,    &g_stAwbCcmFsWdr, sizeof(AWB_CCM_S));
            memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTableFSWDR, sizeof(AWB_AGC_TABLE_S));
            break;
    }
    pstAwbSnsDft->u16InitRgain = g_au16InitWBGain[ViPipe][0];
    pstAwbSnsDft->u16InitGgain = g_au16InitWBGain[ViPipe][1];
    pstAwbSnsDft->u16InitBgain = g_au16InitWBGain[ViPipe][2];
    pstAwbSnsDft->u16SampleRgain = g_au16SampleRgain[ViPipe];
    pstAwbSnsDft->u16SampleBgain = g_au16SampleBgain[ViPipe];

    return GK_SUCCESS;
}

static GK_S32 cmos_init_awb_exp_function(AWB_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    CMOS_CHECK_POINTER(pstExpFuncs);

    memset(pstExpFuncs, 0, sizeof(AWB_SENSOR_EXP_FUNC_S));

    pstExpFuncs->pfn_cmos_get_awb_default = cmos_get_awb_default;

    return GK_SUCCESS;
}


static ISP_CMOS_DNG_COLORPARAM_S g_stDngColorParam = {
    {378, 256, 430},
    {439, 256, 439}
};


static GK_S32 cmos_get_isp_default(VI_PIPE ViPipe, ISP_CMOS_DEFAULT_S *pstDef)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstDef);
    OS05A_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset(pstDef, 0, sizeof(ISP_CMOS_DEFAULT_S));
#ifdef CONFIG_ISP_CA_SUPPORT
    pstDef->unKey.bit1Ca       = 1;
    pstDef->pstCa              = &g_stIspCA;
#endif

    pstDef->unKey.bit1Dpc      = 1;
    pstDef->pstDpc             = &g_stCmosDpc;

    pstDef->unKey.bit1Wdr      = 1;
    pstDef->pstWdr             = &g_stIspWDR;
    pstDef->unKey.bit1Lsc      = 0;
    pstDef->pstLsc             = &g_stCmosLsc;
    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:
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
#ifdef CONFIG_ISP_CR_SUPPORT
            pstDef->unKey.bit1Ge             = 1;
            pstDef->pstGe                    = &g_stIspGe;
#endif
            pstDef->unKey.bit1AntiFalseColor = 1;
            pstDef->pstAntiFalseColor        = &g_stIspAntiFalseColor;
            pstDef->unKey.bit1Ldci           = 1;
            pstDef->pstLdci                  = &g_stIspLdci;
            pstDef->unKey.bit1Dehaze         = 1;
            pstDef->pstDehaze                = &g_stIspDehaze;
            pstDef->unKey.bit1Lcac           = 0;
            pstDef->pstLcac                  = &g_stIspLCac;
            memcpy(&pstDef->stNoiseCalibration,    &g_stIspNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
            break;
        case WDR_MODE_2To1_LINE:
            pstDef->unKey.bit1Demosaic       = 1;
            pstDef->pstDemosaic              = &g_stIspDemosaicWdr;
            pstDef->unKey.bit1Sharpen        = 1;
            pstDef->pstSharpen               = &g_stIspYuvSharpenWdr;
            pstDef->unKey.bit1Drc            = 1;
            pstDef->pstDrc                   = &g_stIspDRCWDR;
            pstDef->unKey.bit1Gamma          = 1;
            pstDef->pstGamma                 = &g_stIspGammaFSWDR;
#ifdef CONFIG_ISP_PREGAMMA_SUPPORT
            pstDef->unKey.bit1PreGamma       = 0;
            pstDef->pstPreGamma              = &g_stPreGamma;
#endif
            pstDef->unKey.bit1BayerNr        = 1;
            pstDef->pstBayerNr               = &g_stIspBayerNrWdr2To1;
#ifdef CONFIG_ISP_CR_SUPPORT
            pstDef->unKey.bit1Ge             = 1;
            pstDef->pstGe                    = &g_stIspWdrGe;
#endif
            pstDef->unKey.bit1AntiFalseColor = 1;
            pstDef->pstAntiFalseColor        = &g_stIspWdrAntiFalseColor;
            pstDef->unKey.bit1Ldci           = 1;
            pstDef->pstLdci                  = &g_stIspWdrLdci;
            pstDef->unKey.bit1Dehaze         = 1;
            pstDef->pstDehaze                = &g_stIspDehazeWDR;
            pstDef->unKey.bit1Lcac           = 0;
            pstDef->pstLcac                  = &g_stIspLCacWdr;
            pstDef->stWdrSwitchAttr.au32ExpRatio[0] = 0x100;
            memcpy(&pstDef->stNoiseCalibration, &g_stIspNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
            break;
    }

    pstDef->stSensorMode.u32SensorID = OS05A_ID;
    pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

    memcpy(&pstDef->stDngColorParam, &g_stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S));

    pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleH.u32Denominator = 1;
    pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleH.u32Numerator = 1;
    pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleV.u32Denominator = 1;
    pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleV.u32Numerator = 1;
    pstDef->stSensorMode.stDngRawFormat.stCfaRepeatPatternDim.u16RepeatPatternDimRows = 2;
    pstDef->stSensorMode.stDngRawFormat.stCfaRepeatPatternDim.u16RepeatPatternDimCols = 2;
    pstDef->stSensorMode.stDngRawFormat.stBlcRepeatDim.u16BlcRepeatRows = 2;
    pstDef->stSensorMode.stDngRawFormat.stBlcRepeatDim.u16BlcRepeatCols = 2;
    pstDef->stSensorMode.stDngRawFormat.enCfaLayout = CFALAYOUT_TYPE_RECTANGULAR;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPlaneColor[0] = 0;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPlaneColor[1] = 1;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPlaneColor[2] = 2;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[0] = 0;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[1] = 1;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[2] = 1;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[3] = 2;
    pstDef->stSensorMode.bValidDngRawFormat = GK_TRUE;

    return GK_SUCCESS;
}

static GK_S32 cmos_get_isp_black_level(VI_PIPE ViPipe, ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
    GK_S32  i;
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstBlackLevel);
    OS05A_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    /* Don't need to update black level when iso change */
    pstBlackLevel->bUpdate = GK_FALSE;
    if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
        for (i = 0; i < 4; i++) {
            pstBlackLevel->au16BlackLevel[i] = 64; /* 12bit,0x40 */
        }
    } else if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        for (i = 0; i < 4; i++) {
            pstBlackLevel->au16BlackLevel[i] = 256; /* 10bit,0x10s0 */
        }
    }

    return GK_SUCCESS;
}

static GK_VOID cmos_set_pixel_detect(VI_PIPE ViPipe, GK_BOOL bEnable)
{
    GK_U32 u32FullLines_5Fps, u32MaxIntTime_5Fps;
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    OS05A_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        return;
    } else {
        if (pstSnsState->u8ImgMode == Os05a_5M_2688x1944_12bit_linear30) {
            u32FullLines_5Fps = (OS05A_VMAX_5M30_12bit_LINEAR * 30) / 5;
        } else {
            return;
        }
    }

    u32MaxIntTime_5Fps = u32FullLines_5Fps - 4;

    if (bEnable) { /* setup for ISP pixel calibration mode */
        os05a_write_register(ViPipe, VMAX_ADDR_L, LOW_8BITS(u32FullLines_5Fps));
        os05a_write_register(ViPipe, VMAX_ADDR_H, HIGH_8BITS(u32FullLines_5Fps));

        os05a_write_register(ViPipe, EXPOSURE_TIME_LONG_H, HIGH_8BITS(u32MaxIntTime_5Fps));    /* shutter */
        os05a_write_register(ViPipe, EXPOSURE_TIME_LONG_L, LOW_8BITS(u32MaxIntTime_5Fps));

        os05a_write_register(ViPipe, AGAIN_H, 0x00); // gain
        os05a_write_register(ViPipe, AGAIN_L, 0x90);
    } else { /* setup for ISP 'normal mode' */
        os05a_write_register(ViPipe, VMAX_ADDR_L, LOW_8BITS(pstSnsState->u32FLStd));
        os05a_write_register(ViPipe, VMAX_ADDR_H, HIGH_8BITS(pstSnsState->u32FLStd));
        pstSnsState->bSyncInit = GK_FALSE;
    }
    return;
}

static GK_S32 cmos_set_wdr_mode(VI_PIPE ViPipe, GK_U8 u8Mode)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    OS05A_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    pstSnsState->bSyncInit = GK_FALSE;

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
            ISP_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return GK_FAILURE;
    }

    memset(pstSnsState->au32WDRIntTime, 0, sizeof(pstSnsState->au32WDRIntTime));

    return GK_SUCCESS;
}

static GK_S32 cmos_get_sns_regs_info(VI_PIPE ViPipe, ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
    GK_S32 i;
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstSnsRegsInfo);
    OS05A_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    if ((pstSnsState->bSyncInit == GK_FALSE) || (pstSnsRegsInfo->bConfig == GK_FALSE)) {
        pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
        pstSnsState->astRegsInfo[0].unComBus.s8I2cDev = g_aunOs05aBusInfo[ViPipe].s8I2cDev;
        pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;
        pstSnsState->astRegsInfo[0].u32RegNum = 8;

        if ((pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) || (pstSnsState->enWDRMode == WDR_MODE_2To1_FRAME) || (pstSnsState->enWDRMode == WDR_MODE_2To1_FRAME_FULL_RATE)) {
            pstSnsState->astRegsInfo[0].u32RegNum += 6;
        }

        for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
            pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = GK_TRUE;
            pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr = os05a_i2c_addr;
            pstSnsState->astRegsInfo[0].astI2cData[i].u32AddrByteNum = os05a_addr_byte;
            pstSnsState->astRegsInfo[0].astI2cData[i].u32DataByteNum = os05a_data_byte;
        }
        pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr = VMAX_ADDR_L;        // Vmax
        pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr = VMAX_ADDR_H;        // Vmax
        pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr = EXPOSURE_TIME_LONG_L;        // Long shutter
        pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr = EXPOSURE_TIME_LONG_H;        // Long shutter
        pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr = AGAIN_L;        // Again
        pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr = AGAIN_H;        // Again
        pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr = DGAIN_L;     // Dgain
        pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr = DGAIN_H;     // Dgain

        if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
            pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 0;
            pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr = EXPOSURE_TIME_SHORT_L;     // Short shutter
            pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum = 0;
            pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr = EXPOSURE_TIME_SHORT_H;     // Short shutter
            pstSnsState->astRegsInfo[0].astI2cData[10].u8DelayFrmNum = 0;
            pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr = SHORT_AGAIN_L;     // Short Again
            pstSnsState->astRegsInfo[0].astI2cData[11].u8DelayFrmNum = 0;
            pstSnsState->astRegsInfo[0].astI2cData[11].u32RegAddr = SHORT_AGAIN_H;     // Short Again
            pstSnsState->astRegsInfo[0].astI2cData[12].u8DelayFrmNum = 0;
            pstSnsState->astRegsInfo[0].astI2cData[12].u32RegAddr = SHORT_DGAIN_L;     // Short Dgain
            pstSnsState->astRegsInfo[0].astI2cData[13].u8DelayFrmNum = 0;
            pstSnsState->astRegsInfo[0].astI2cData[13].u32RegAddr = SHORT_DGAIN_H;     // Short Dgain
        }
        pstSnsState->bSyncInit = GK_TRUE;
    } else {
        for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
            if (pstSnsState->astRegsInfo[0].astI2cData[i].u32Data == pstSnsState->astRegsInfo[1].astI2cData[i].u32Data) {
                pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = GK_FALSE;
            }

            else {
                pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = GK_TRUE;
            }
        }
    }

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        if (pstSnsState->au32FL[1] > pstSnsState->au32FL[0]) {
            pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 2;
            pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 2;
        } else if (pstSnsState->au32FL[1] < pstSnsState->au32FL[0]) {
            pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
            pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
        } else {
            // do nothing
        }
    }

    pstSnsRegsInfo->bConfig = GK_FALSE;
    memcpy(pstSnsRegsInfo, &pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S));
    memcpy(&pstSnsState->astRegsInfo[1], &pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S));
    pstSnsState->au32FL[1] = pstSnsState->au32FL[0];

    return GK_SUCCESS;
}

static GK_S32 cmos_set_image_mode(VI_PIPE ViPipe, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode)
{
    GK_U8 u8SensorImageMode;
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstSensorImageMode);
    OS05A_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    u8SensorImageMode = pstSnsState->u8ImgMode;
    pstSnsState->bSyncInit = GK_FALSE;

    if ((pstSensorImageMode->u16Width <= 2688) && (pstSensorImageMode->u16Height <= 1944)) {
        if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
            if (pstSensorImageMode->f32Fps <= 30) {
                u8SensorImageMode     = Os05a_5M_2688x1944_10bit_wdr30;
                pstSnsState->u32FLStd = OS05A_VMAX_5M30_10bit_WDR;
            } else {
                OS05A_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState);
                return GK_FAILURE;
            }
        } else if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
            if (pstSensorImageMode->f32Fps <= 30) {
                u8SensorImageMode      = Os05a_5M_2688x1944_12bit_linear30;
                pstSnsState->u32FLStd  = OS05A_VMAX_5M30_12bit_LINEAR;
            } else {
                OS05A_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState);;
                return GK_FAILURE;
            }
        }
    } else {
        OS05A_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState);;
        return GK_FAILURE;
    }

    /* Switch SensorImageMode */
    if ((pstSnsState->bInit == GK_TRUE) && (u8SensorImageMode == pstSnsState->u8ImgMode)) {
        /* Don't need to switch SensorImageMode */
        return ISP_DO_NOT_NEED_SWITCH_IMAGEMODE;
    }

    pstSnsState->u8ImgMode = u8SensorImageMode;
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstSnsState->au32FL[1] = pstSnsState->au32FL[0];
    memset(pstSnsState->au32WDRIntTime, 0, sizeof(pstSnsState->au32WDRIntTime));

    return GK_SUCCESS;
}

static GK_VOID sensor_global_init(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    OS05A_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->bInit = GK_FALSE;
    pstSnsState->bSyncInit = GK_FALSE;
    pstSnsState->u8ImgMode = Os05a_5M_2688x1944_12bit_linear30;
    pstSnsState->enWDRMode = WDR_MODE_NONE;
    pstSnsState->u32FLStd = OS05A_VMAX_5M30_12bit_LINEAR;
    pstSnsState->au32FL[0] = OS05A_VMAX_5M30_12bit_LINEAR;
    pstSnsState->au32FL[1] = OS05A_VMAX_5M30_12bit_LINEAR;

    memset(&pstSnsState->astRegsInfo[0], 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset(&pstSnsState->astRegsInfo[1], 0, sizeof(ISP_SNS_REGS_INFO_S));

}

static GK_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    CMOS_CHECK_POINTER(pstSensorExpFunc);

    memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init = os05a_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit = os05a_exit;
    pstSensorExpFunc->pfn_cmos_sensor_global_init = sensor_global_init;
    pstSensorExpFunc->pfn_cmos_set_image_mode = cmos_set_image_mode;
    pstSensorExpFunc->pfn_cmos_set_wdr_mode = cmos_set_wdr_mode;

    pstSensorExpFunc->pfn_cmos_get_isp_default = cmos_get_isp_default;
    pstSensorExpFunc->pfn_cmos_get_isp_black_level = cmos_get_isp_black_level;
    pstSensorExpFunc->pfn_cmos_set_pixel_detect = cmos_set_pixel_detect;
    pstSensorExpFunc->pfn_cmos_get_sns_reg_info = cmos_get_sns_regs_info;

    return GK_SUCCESS;
}

/****************************************************************************
 * callback structure                                                       *
 ****************************************************************************/

static GK_S32 os05a_set_bus_info(VI_PIPE ViPipe, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunOs05aBusInfo[ViPipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

    return GK_SUCCESS;
}

static GK_S32 sensor_ctx_init(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

    OS05A_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);

    if (pastSnsStateCtx == GK_NULL) {
        pastSnsStateCtx = (ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
        if (pastSnsStateCtx == GK_NULL) {
            ISP_TRACE(MODULE_DBG_ERR, "Isp[%d] SnsCtx malloc memory failed!\n", ViPipe);
            return ERR_CODE_ISP_NOMEM;
        }
    }

    memset(pastSnsStateCtx, 0, sizeof(ISP_SNS_STATE_S));

    OS05A_SENSOR_SET_CTX(ViPipe, pastSnsStateCtx);

    return GK_SUCCESS;
}

static GK_VOID sensor_ctx_exit(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

    OS05A_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);
    SENSOR_FREE(pastSnsStateCtx);
    OS05A_SENSOR_RESET_CTX(ViPipe);
}

static GK_S32 sensor_register_callback(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib)
{
    GK_S32 s32Ret;
    ISP_SENSOR_REGISTER_S stIspRegister;
    AE_SENSOR_REGISTER_S  stAeRegister;
    AWB_SENSOR_REGISTER_S stAwbRegister;
    ISP_SNS_ATTR_INFO_S   stSnsAttrInfo;

    CMOS_CHECK_POINTER(pstAeLib);
    CMOS_CHECK_POINTER(pstAwbLib);

    s32Ret = sensor_ctx_init(ViPipe);

    if (s32Ret != GK_SUCCESS) {
        return GK_FAILURE;
    }

    stSnsAttrInfo.eSensorId = OS05A_ID;

    s32Ret  = cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
    s32Ret |= GK_API_ISP_SensorRegCallBack(ViPipe, &stSnsAttrInfo, &stIspRegister);

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor register callback function failed!\n");
        return s32Ret;
    }

    s32Ret  = cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
    s32Ret |= GK_API_AE_SensorRegCallBack(ViPipe, pstAeLib, &stSnsAttrInfo, &stAeRegister);

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor register callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret  = cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
    s32Ret |= GK_API_AWB_SensorRegCallBack(ViPipe, pstAwbLib, &stSnsAttrInfo, &stAwbRegister);

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor register callback function to awb lib failed!\n");
        return s32Ret;
    }

    return GK_SUCCESS;
}

static GK_S32 sensor_unregister_callback(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib)
{
    GK_S32 s32Ret;

    CMOS_CHECK_POINTER(pstAeLib);
    CMOS_CHECK_POINTER(pstAwbLib);

    s32Ret = GK_API_ISP_SensorUnRegCallBack(ViPipe, OS05A_ID);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor unregister callback function failed!\n");
        return s32Ret;
    }

    s32Ret = GK_API_AE_SensorUnRegCallBack(ViPipe, pstAeLib, OS05A_ID);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = GK_API_AWB_SensorUnRegCallBack(ViPipe, pstAwbLib, OS05A_ID);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor unregister callback function to awb lib failed!\n");
        return s32Ret;
    }

    sensor_ctx_exit(ViPipe);

    return GK_SUCCESS;
}

static GK_S32 sensor_set_init(VI_PIPE ViPipe, ISP_INIT_ATTR_S *pstInitAttr)
{
    CMOS_CHECK_POINTER(pstInitAttr);

    g_au32InitExposure[ViPipe] = pstInitAttr->u32Exposure;
    g_au32LinesPer500ms[ViPipe] = pstInitAttr->u32LinesPer500ms;
    g_au16InitWBGain[ViPipe][0] = pstInitAttr->u16WBRgain;
    g_au16InitWBGain[ViPipe][1] = pstInitAttr->u16WBGgain;
    g_au16InitWBGain[ViPipe][2] = pstInitAttr->u16WBBgain;
    g_au16SampleRgain[ViPipe] = pstInitAttr->u16SampleRgain;
    g_au16SampleBgain[ViPipe] = pstInitAttr->u16SampleBgain;

    return GK_SUCCESS;
}

ISP_SNS_OBJ_S stSnsOs05aObj = {
    .pfnRegisterCallback    = sensor_register_callback,
    .pfnUnRegisterCallback  = sensor_unregister_callback,
    .pfnStandby             = os05a_standby,
    .pfnRestart             = os05a_restart,
    .pfnMirrorFlip          = GK_NULL,
    .pfnWriteReg            = os05a_write_register,
    .pfnReadReg             = os05a_read_register,
    .pfnSetBusInfo          = os05a_set_bus_info,
    .pfnSetInit             = sensor_set_init
};


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
