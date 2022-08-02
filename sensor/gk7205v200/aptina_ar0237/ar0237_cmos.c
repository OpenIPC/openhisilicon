/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#if !defined(__AR0237_CMOS_H_)
#define __AR0237_CMOS_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "comm_sns.h"
#include "comm_video.h"
#include "sns_ctrl.h"
#include "gk_api_isp.h"
#include "gk_api_ae.h"
#include "gk_api_awb.h"
#include "ar0237_cmos_ex.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define AR0237_ID                              237

static GK_U32 au32WDRIntTime[4] = {0};

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
ISP_SNS_STATE_S *g_pastAr0237[ISP_MAX_PIPE_NUM] = {GK_NULL};

#define AR0237_SENSOR_GET_CTX(dev, pstCtx)   (pstCtx = g_pastAr0237[dev])
#define AR0237_SENSOR_SET_CTX(dev, pstCtx)   (g_pastAr0237[dev] = pstCtx)
#define AR0237_SENSOR_RESET_CTX(dev)         (g_pastAr0237[dev] = GK_NULL)

ISP_SNS_COMMBUS_U g_aunAr0237BusInfo[ISP_MAX_PIPE_NUM] = {
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

extern const unsigned int ar0237_i2c_addr;
extern unsigned int ar0237_addr_byte;
extern unsigned int ar0237_data_byte;


extern void ar0237_init(VI_PIPE ViPipe);
extern void ar0237_exit(VI_PIPE ViPipe);
extern void ar0237_standby(VI_PIPE ViPipe);
extern void ar0237_restart(VI_PIPE ViPipe);
extern int  ar0237_write_register(VI_PIPE ViPipe, int addr, int data);
extern int  ar0237_read_register(VI_PIPE ViPipe, int addr);
#define FULL_LINES_MAX                         (0xFFFF)

#define ANALOG_GAIN                            (0x3060)
#define DIGITAL_GAIN                           (0x305E)
#define HCG_ADDR                               (0x3100)
#define VMAX_ADDR                              (0x300A)
#define EXPOSURE_TIME_L                        (0x3012)
#define EXPOSURE_TIME_S                        (0x3212)
#define Y_OUTPUT        (1080)

#define SENSOR_2M_1080p30_MODE                 (1)

#define INCREASE_LINES                         (1)
#define FRAME_LINES_2M_LIN                     (0x454+INCREASE_LINES)
#define FRAME_LINES_2M_WDR                     (0x4C4+INCREASE_LINES)


// sensor fps mode
#define AR0237_SENSOR_1080P_30FPS_LINEAR_MODE  (1)
#define AR0237_SENSOR_1080P_30FPS_3t1_WDR_MODE (2)
#define AR0237_SENSOR_1080P_30FPS_2t1_WDR_MODE (3)
#define AR0237_SENSOR_720P_30FPS_2t1_WDR_MODE  (4)

// Max integration time for HDR mode: T1 max = min ( 70* 16, FLL*16/17), when ratio=16
#define LONG_EXP_SHT_CLIP                      (FRAME_LINES_2M_LIN*16/17)

static GK_FLOAT gf32Fps = 30;

ISP_SNS_REGS_INFO_S g_stSnsRegsInfo = {0};
ISP_SNS_REGS_INFO_S g_stPreSnsRegsInfo = {0};

/* AE default parameter and function */
static GK_S32 cmos_get_ae_default(VI_PIPE ViPipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstAeSnsDft);
    AR0237_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset(&pstAeSnsDft->stAERouteAttr, 0, sizeof(ISP_AE_ROUTE_S));

    pstAeSnsDft->u32LinesPer500ms = pstSnsState->u32FLStd * 30 / 2;
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FlickerFreq = 0;
    pstAeSnsDft->u32FullLinesMax = FULL_LINES_MAX;

    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
    pstAeSnsDft->stIntTimeAccu.f32Offset = 0;

    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 0.0078125;

    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 0.0078125;

    pstAeSnsDft->u32ISPDgainShift = 8;
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 4 << pstAeSnsDft->u32ISPDgainShift;

    pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_0;
    pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_32_0;

    pstAeSnsDft->bAERouteExValid = GK_FALSE;
    pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
    pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;

    switch (pstSnsState->enWDRMode) {
        case WDR_MODE_NONE: { /* linear mode */
            pstAeSnsDft->au8HistThresh[0] = 0xd;
            pstAeSnsDft->au8HistThresh[1] = 0x28;
            pstAeSnsDft->au8HistThresh[2] = 0x60;
            pstAeSnsDft->au8HistThresh[3] = 0x80;

            pstAeSnsDft->u8AeCompensation = 0x38;
            pstAeSnsDft->u32InitExposure = 96181;

            pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 2;
            pstAeSnsDft->u32MinIntTime = 2;
            pstAeSnsDft->u32MaxIntTimeTarget = 65535;
            pstAeSnsDft->u32MinIntTimeTarget = 2;

            pstAeSnsDft->u32MaxAgain = 29030;
            pstAeSnsDft->u32MinAgain = 1024;
            pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
            pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

            pstAeSnsDft->u32MaxDgain = 1024;
            pstAeSnsDft->u32MinDgain = 128;
            pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
            pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;
            pstAeSnsDft->u32MaxISPDgainTarget = 2 << pstAeSnsDft->u32ISPDgainShift;
            break;
        }
        case WDR_MODE_2To1_LINE: {
            pstAeSnsDft->au8HistThresh[0] = 0xC;
            pstAeSnsDft->au8HistThresh[1] = 0x18;
            pstAeSnsDft->au8HistThresh[2] = 0x60;
            pstAeSnsDft->au8HistThresh[3] = 0x80;
            pstAeSnsDft->u8AeCompensation = 40;
            pstAeSnsDft->u32InitExposure = 27228;

            pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 2;
            pstAeSnsDft->u32MinIntTime = 8;
            pstAeSnsDft->u32MaxIntTimeTarget = 65535;
            pstAeSnsDft->u32MinIntTimeTarget = pstAeSnsDft->u32MinIntTime;

            pstAeSnsDft->u32MaxAgain = 16589;
            pstAeSnsDft->u32MinAgain = 1024;
            pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
            pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

            pstAeSnsDft->u32MaxDgain = 1024;
            pstAeSnsDft->u32MinDgain = 128;
            pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
            pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;
            pstAeSnsDft->u32MaxISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;

            pstAeSnsDft->u32LFMaxShortTime = 350;
            pstAeSnsDft->u32MaxIntTimeStep = 3000;
            pstAeSnsDft->u32LFMinExposure = 580000;

            pstAeSnsDft->u16ManRatioEnable = GK_FALSE;
            pstAeSnsDft->au32Ratio[0] = 0x400;
            pstAeSnsDft->au32Ratio[1] = 0x40;
            pstAeSnsDft->au32Ratio[2] = 0x40;
            pstAeSnsDft->u32InitExposure = 17099;
            break;
        }
        default:
            break;
    }
    return 0;
}


/* the function of sensor set fps */
static GK_VOID cmos_fps_set(VI_PIPE ViPipe, GK_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    AR0237_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);
    if (f32Fps <= 30) {
        gf32Fps = f32Fps;
    }
    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        if (f32Fps == 30) {
            pstSnsState->u32FLStd = FRAME_LINES_2M_WDR;
        } else if ((f32Fps < 30) && (f32Fps >= 0.5)) {
            pstSnsState->u32FLStd = (FRAME_LINES_2M_WDR * 30) / f32Fps;
        } else {
            printf("Not support Fps: %f\n", f32Fps);
            return;
        }
    } else {
        if ((f32Fps <= 30) && (f32Fps >= 0.5)) {
            pstSnsState->u32FLStd = (FRAME_LINES_2M_LIN * 30) / f32Fps;
        } else {
            printf("Not support Fps: %f\n", f32Fps);
            return;
        }
    }

    pstSnsState->u32FLStd = pstSnsState->u32FLStd > FULL_LINES_MAX ? FULL_LINES_MAX : pstSnsState->u32FLStd;
    g_stSnsRegsInfo.astI2cData[4].u32Data = pstSnsState->u32FLStd;

    pstAeSnsDft->f32Fps = f32Fps;
    pstAeSnsDft->u32LinesPer500ms = pstSnsState->u32FLStd * f32Fps / 2;
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 4;
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];

    return;
}
static GK_VOID cmos_slow_framerate_set(VI_PIPE ViPipe, GK_U32 u32FullLines,
                                       AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    AR0237_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);
    u32FullLines = (u32FullLines > FULL_LINES_MAX) ? FULL_LINES_MAX : u32FullLines;
    pstSnsState->au32FL[0] = u32FullLines;
    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
    g_stSnsRegsInfo.astI2cData[4].u32Data = pstSnsState->au32FL[0];
    pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 2;

    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static GK_VOID cmos_inttime_update(VI_PIPE ViPipe, GK_U32 u32IntTime)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;
    static GK_BOOL bFirst[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = 1};

    AR0237_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);
    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        if (bFirst[ViPipe]) { /* short exposure */
            pstSnsState->au32WDRIntTime[0] = u32IntTime;
            g_stSnsRegsInfo.astI2cData[0].u32Data = u32IntTime;
            bFirst[ViPipe] = GK_FALSE;
        } else { /* long exposure */
            au32WDRIntTime[1] = u32IntTime;
            g_stSnsRegsInfo.astI2cData[5].u32Data = u32IntTime;
            bFirst[ViPipe] = GK_TRUE;
        }
    } else {
        g_stSnsRegsInfo.astI2cData[5].u32Data = u32IntTime;
        bFirst[ViPipe] = GK_TRUE;
    }
    return;
}

static  GK_U32  again_table[51] = {
    // here 1024 means x1.52
    1024, 1075, 1132, 1195, 1265, 1344, 1434, 1536, 1654, 1792, 1814, 1873, 1935, 2002, 2074,
    2150, 2233, 2322, 2419, 2524, 2639, 2765, 2903, 3056, 3226, 3415, 3629, 3871, 4147, 4466,
    4838, 5278, 5806, 6451, 7258, 7741, 8294, 8932, 9677, 10557, 11612, 12902, 14515, 15483, 16589,
    17865, 19354, 21113, 23224, 25805, 29030
};

static GK_VOID cmos_again_calc_table(VI_PIPE ViPipe, GK_U32 *pu32AgainLin, GK_U32 *pu32AgainDb)
{
    int i;
    GK_U32 u32InTimes;
    CMOS_CHECK_POINTER_VOID(pu32AgainLin);
    CMOS_CHECK_POINTER_VOID(pu32AgainDb);

    u32InTimes = *pu32AgainLin;

    if (u32InTimes >= again_table[50]) {
        *pu32AgainLin = again_table[50];
        *pu32AgainDb = 50;
        return;
    }

    for (i = 1; i < 51; i++) {
        if (u32InTimes < again_table[i]) {
            *pu32AgainLin = again_table[i - 1];
            *pu32AgainDb = i - 1;
            break;
        }
    }
    return;
}

static GK_VOID cmos_gains_update(VI_PIPE ViPipe, GK_U32 u32Again, GK_U32 u32Dgain)
{
    static GK_BOOL bHCG = GK_FALSE;

    if (u32Again < 10) {
        if (bHCG == GK_TRUE) {
            g_stSnsRegsInfo.astI2cData[3].u32Data = 0x0000;
            bHCG = GK_FALSE;
        }

        if (u32Again < 5) {
            g_stSnsRegsInfo.astI2cData[1].u32Data = u32Again + 0xb;
        } else {
            // 0x10 + (again - 5) * 2
            g_stSnsRegsInfo.astI2cData[1].u32Data = u32Again * 2 + 0x6;
        }
    } else {
        if (bHCG == GK_FALSE) {
            g_stSnsRegsInfo.astI2cData[3].u32Data = 0x0004;
            bHCG = GK_TRUE;
        }
        if (u32Again < 26) {
            g_stSnsRegsInfo.astI2cData[1].u32Data = u32Again - 10;
        } else {
            // 0x10 + (again - 26) * 2
            g_stSnsRegsInfo.astI2cData[1].u32Data = u32Again * 2 - 36;
        }
    }
    g_stSnsRegsInfo.astI2cData[2].u32Data = u32Dgain;
    return;
}

/* Only used in WDR_MODE_2To1_LINE mode */
/* [T2 max = FLL - y_output 16] The y_output is the output rows calculated by
    (Y_ADDR_END - Y_ADDR_START +1).*/
static GK_VOID cmos_get_inttime_max(VI_PIPE ViPipe, GK_U16 u16ManRatioEnable, GK_U32 *au32Ratio, GK_U32 *au32IntTimeMax, GK_U32 *au32IntTimeMin, GK_U32 *pu32LFMaxIntTime)
{
    GK_U32 u32IntTimeMaxTmp0 = 0;
    GK_U32 u32IntTimeMaxTmp  = 0;
    GK_U32 u32T2Max = 0;
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;
    AR0237_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (au32IntTimeMax == GK_NULL) {
        printf("null pointer when get ae sensor IntTimeMax value!\n");
        return;
    }

    if ((pstSnsState->enWDRMode == WDR_MODE_2To1_LINE)) {
        /* Short + Long < 1V - 2;
            Ratio = Long * 0x40 / Short */
        if (genFSWDRMode[ViPipe] == ISP_FSWDR_LONG_FRAME_MODE) {
            u32T2Max = pstSnsState->au32FL[1] - 16 - au32WDRIntTime[0];
            *au32IntTimeMax = pstSnsState->au32FL[0] - 16;
            *au32IntTimeMax = (u32T2Max < *au32IntTimeMax) ? u32T2Max : *au32IntTimeMax;
        } else if (genFSWDRMode[ViPipe] == ISP_FSWDR_AUTO_LONG_FRAME_MODE) {
            if (au32WDRIntTime[0] == au32IntTimeMin[0] && au32Ratio[0] == 0x40) {
                u32T2Max = pstSnsState->au32FL[1] - 16 - au32WDRIntTime[0];
                *au32IntTimeMax = pstSnsState->au32FL[0] - 16;
                *au32IntTimeMax = (u32T2Max < *au32IntTimeMax) ? u32T2Max : *au32IntTimeMax;
            } else {
                u32IntTimeMaxTmp0 = ((pstSnsState->au32FL[1] - 30 - au32WDRIntTime[0]) * 0x40)  / DIV_0_TO_1(au32Ratio[0]);
                u32IntTimeMaxTmp = ((pstSnsState->au32FL[0] - 30) * 0x40)  / DIV_0_TO_1(au32Ratio[0] + 0x40);
                u32IntTimeMaxTmp = (u32IntTimeMaxTmp0 < u32IntTimeMaxTmp) ? u32IntTimeMaxTmp0 : u32IntTimeMaxTmp;
                u32IntTimeMaxTmp0 = pstSnsState->au32FL[0] - 30 - (pstSnsState->au32FL[0] - 16 - Y_OUTPUT);
                u32IntTimeMaxTmp0 = (u32IntTimeMaxTmp0 * 0x40) / DIV_0_TO_1(au32Ratio[0]);
                *au32IntTimeMax = (u32IntTimeMaxTmp0 > u32IntTimeMaxTmp) ? u32IntTimeMaxTmp0 : u32IntTimeMaxTmp;
                *pu32LFMaxIntTime = pstSnsState->au32FL[0] - 16 - Y_OUTPUT;
            }
        } else {
            u32T2Max = pstSnsState->au32FL[0] - 16 - Y_OUTPUT;
            *au32IntTimeMax = (pstSnsState->au32FL[0] - 30) * 0x40 / DIV_0_TO_1(au32Ratio[0] + 0x40);
            if (*au32IntTimeMax > u32T2Max) {
                *au32IntTimeMax = u32T2Max;
            }
        }
    } else {
    }
    return;
}

/* Only used in LINE_WDR mode */
static GK_VOID cmos_ae_fswdr_attr_set(VI_PIPE ViPipe, AE_FSWDR_ATTR_S *pstAeFSWDRAttr)
{
    CMOS_CHECK_POINTER_VOID(pstAeFSWDRAttr);
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
    pstExpFuncs->pfn_cmos_again_calc_table  = cmos_again_calc_table;
    pstExpFuncs->pfn_cmos_dgain_calc_table  = NULL;
    pstExpFuncs->pfn_cmos_get_inttime_max   = cmos_get_inttime_max;
    pstExpFuncs->pfn_cmos_ae_fswdr_attr_set = cmos_ae_fswdr_attr_set;

    return GK_SUCCESS;
}

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN                           0
#define GOLDEN_BGAIN                           0

static GK_S32 cmos_get_awb_default(VI_PIPE ViPipe, AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstAwbSnsDft);
    AR0237_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));

    pstAwbSnsDft->u16WbRefTemp = 4962;

    pstAwbSnsDft->au16GainOffset[0] = 392;
    pstAwbSnsDft->au16GainOffset[1] = 256;
    pstAwbSnsDft->au16GainOffset[2] = 256;
    pstAwbSnsDft->au16GainOffset[3] = 453;

    pstAwbSnsDft->as32WbPara[0] = 40;
    pstAwbSnsDft->as32WbPara[1] = 108;
    pstAwbSnsDft->as32WbPara[2] = -108;
    pstAwbSnsDft->as32WbPara[3] = 172722;
    pstAwbSnsDft->as32WbPara[4] = 128;
    pstAwbSnsDft->as32WbPara[5] = -122283;

    pstAwbSnsDft->u16GoldenRgain = GOLDEN_RGAIN;
    pstAwbSnsDft->u16GoldenBgain = GOLDEN_BGAIN;

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:
            memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcm, sizeof(AWB_CCM_S));
            memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTable, sizeof(AWB_AGC_TABLE_S));
            break;

        case WDR_MODE_2To1_LINE:
            memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcmFsWdr, sizeof(AWB_CCM_S));
            memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTableFSWDR, sizeof(AWB_AGC_TABLE_S));
            break;
    }


    pstAwbSnsDft->u16InitRgain = g_au16InitWBGain[ViPipe][0];
    pstAwbSnsDft->u16InitGgain = g_au16InitWBGain[ViPipe][1];
    pstAwbSnsDft->u16InitBgain = g_au16InitWBGain[ViPipe][2];
    pstAwbSnsDft->u16SampleRgain = g_au16SampleRgain[ViPipe];
    pstAwbSnsDft->u16SampleBgain = g_au16SampleBgain[ViPipe];

    pstAwbSnsDft->u8AWBRunInterval = 4;

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
    AR0237_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset(pstDef, 0, sizeof(ISP_CMOS_DEFAULT_S));

    pstDef->unKey.bit1Ca      = 1;
    pstDef->pstCa             = &g_stIspCA;

    pstDef->unKey.bit1Dpc     = 1;
    pstDef->pstDpc            = &g_stCmosDpc;

    pstDef->unKey.bit1Wdr     = 1;
    pstDef->pstWdr            = &g_stIspWDR;

    pstDef->unKey.bit1Lsc      = 0;
    pstDef->pstLsc             = &g_stCmosLsc;

    pstDef->unKey.bit1PreGamma = 0;
    pstDef->pstPreGamma        = &g_stPreGamma;
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
            pstDef->unKey.bit1AntiFalseColor = 1;
            pstDef->pstAntiFalseColor        = &g_stIspAntiFalseColor;
            pstDef->unKey.bit1Ldci           = 1;
            pstDef->pstLdci                  = &g_stIspLdci;
            pstDef->unKey.bit1Gamma          = 1;
            pstDef->pstGamma                 = &g_stIspGamma;
            pstDef->unKey.bit1Ge             = 1;
            pstDef->pstGe                    = &g_stIspGe;
            pstDef->unKey.bit1Dehaze         = 1;
            pstDef->pstDehaze                = &g_stIspDehaze;
            pstDef->unKey.bit1Lcac           = 0;
            pstDef->pstLcac                  = &g_stIspLCac;			
            pstDef->unKey.bit1Rgbir          = 1;
            pstDef->pstRgbir                 = &g_stIspRgbir;
            memcpy(&pstDef->stNoiseCalibration,   &g_stIspNoiseCalibratio, sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
            break;
        case WDR_MODE_2To1_FRAME:
        case WDR_MODE_2To1_LINE:
            pstDef->unKey.bit1Demosaic       = 1;
            pstDef->pstDemosaic              = &g_stIspDemosaicWdr;
            pstDef->unKey.bit1Sharpen        = 1;
            pstDef->pstSharpen               = &g_stIspYuvSharpenWdr;
            pstDef->unKey.bit1Drc            = 1;
            pstDef->pstDrc                   = &g_stIspDRCWDR;
            pstDef->unKey.bit1Gamma          = 1;
            pstDef->pstGamma                 = &g_stIspGammaFSWDR;
            pstDef->unKey.bit1PreGamma       = 0;
            pstDef->pstPreGamma              = &g_stPreGamma;
            pstDef->unKey.bit1BayerNr        = 1;
            pstDef->pstBayerNr               = &g_stIspBayerNrWdr2To1;
            pstDef->pstGe                    = &g_stIspWdrGe;
            pstDef->unKey.bit1AntiFalseColor = 1;
            pstDef->pstAntiFalseColor        = &g_stIspWdrAntiFalseColor;
            pstDef->unKey.bit1Ldci           = 1;
            pstDef->pstLdci                  = &g_stIspWdrLdci;
            pstDef->unKey.bit1Dehaze         = 1;
            pstDef->pstDehaze                = &g_stIspDehazeWDR;
            pstDef->unKey.bit1Rgbir          = 0;
            pstDef->pstRgbir                 = &g_stIspRgbir;
            pstDef->unKey.bit1Lcac           = 0;
            pstDef->pstLcac                  = &g_stIspLCacWdr;						
            memcpy(&pstDef->stNoiseCalibration, &g_stIspNoiseCalibratio, sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
            break;
    }

    pstDef->stSensorMode.u32SensorID = AR0237_ID;
    pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;


    memcpy(&pstDef->stDngColorParam, &g_stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S));

    switch (pstSnsState->u8ImgMode) {
        default:
        case AR0237_SENSOR_1080P_30FPS_LINEAR_MODE:
            pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 12;
            pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 4095;
            break;

        case AR0237_SENSOR_720P_30FPS_2t1_WDR_MODE:
            pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 10;
            pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 1023;
            break;
    }

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
    GK_U16 u16Bl;

    CMOS_CHECK_POINTER(pstBlackLevel);
    AR0237_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    /* Don't need to update black level when iso change */
    pstBlackLevel->bUpdate = GK_FALSE;

    u16Bl = (pstSnsState->enWDRMode) ? 0xA8 : 0xA6;

    for (i = 0; i < 4; i++) {
        pstBlackLevel->au16BlackLevel[i] = u16Bl;
    }


    return GK_SUCCESS;

}

static GK_VOID cmos_set_pixel_detect(VI_PIPE ViPipe, GK_BOOL bEnable)
{
    GK_U32 u32FullLines_5Fps = (FRAME_LINES_2M_LIN * 30) / 5;
    GK_U32 u32MaxExpTime_5Fps = u32FullLines_5Fps - 2;

    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    AR0237_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);
    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        return;
    }

    u32MaxExpTime_5Fps = 4;

    if (bEnable) { /* setup for ISP pixel calibration mode */
        ar0237_write_register(ViPipe, VMAX_ADDR, u32FullLines_5Fps); /* 5fps */
        ar0237_write_register(ViPipe, EXPOSURE_TIME_L, u32MaxExpTime_5Fps); /* max exposure time */
        ar0237_write_register(ViPipe, ANALOG_GAIN, 0x0);      /* AG, Context A */
        ar0237_write_register(ViPipe, DIGITAL_GAIN, 0x0080);  /* DG, Context A */
    } else { /* setup for ISP 'normal mode' */
        pstSnsState->u32FLStd = (pstSnsState->u32FLStd > 0xFFFF) ? 0xFFFF : pstSnsState->u32FLStd;
        pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
        ar0237_write_register(ViPipe, VMAX_ADDR, pstSnsState->u32FLStd);    /* 30fps */
        pstSnsState->bSyncInit = GK_FALSE;
    }

    return;
}

static GK_S32 cmos_set_wdr_mode(VI_PIPE ViPipe, GK_U8 u8Mode)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    AR0237_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);
    pstSnsState->bSyncInit = GK_FALSE;

    switch (u8Mode) {
        case WDR_MODE_NONE: {
            pstSnsState->enWDRMode = WDR_MODE_NONE;
            pstSnsState->u32FLStd = FRAME_LINES_2M_LIN;
            g_stSnsRegsInfo.astI2cData[4].u32Data = (pstSnsState->u32FLStd * 30) / DIV_0_TO_1(gf32Fps);
            ISP_TRACE(MODULE_DBG_ERR, "Line mode\n");
            break;
        }

        case WDR_MODE_2To1_LINE: {
            pstSnsState->enWDRMode = WDR_MODE_2To1_LINE;
            pstSnsState->u32FLStd = FRAME_LINES_2M_WDR;
            pstSnsState->au32FL[0] =  FRAME_LINES_2M_WDR;
            memset(au32WDRIntTime, 0, sizeof(au32WDRIntTime));
            g_stSnsRegsInfo.astI2cData[4].u32Data = (pstSnsState->u32FLStd * 30) / DIV_0_TO_1(gf32Fps);
            printf("2to1 line WDR mode\n");
            break;
        }

        default: {
            ISP_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return GK_SUCCESS;
            break;
        }
    }
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;

    return GK_SUCCESS;
}

static GK_S32 cmos_get_sns_regs_info(VI_PIPE ViPipe, ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
    GK_S32 i;
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstSnsRegsInfo);
    AR0237_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);
    if (pstSnsState->bSyncInit == GK_FALSE) {
        g_stSnsRegsInfo.enSnsType = ISP_SNS_I2C_TYPE;
        g_stSnsRegsInfo.u8Cfg2ValidDelayMax = 3;
        g_stSnsRegsInfo.u32RegNum = 6;
        for (i = 0; i < g_stSnsRegsInfo.u32RegNum; i++) {
            g_stSnsRegsInfo.astI2cData[i].bUpdate = GK_TRUE;
            g_stSnsRegsInfo.astI2cData[i].u8DevAddr = ar0237_i2c_addr;
            g_stSnsRegsInfo.astI2cData[i].u32AddrByteNum =  ar0237_addr_byte;
            g_stSnsRegsInfo.astI2cData[i].u32DataByteNum =  ar0237_data_byte;
        }
        g_stSnsRegsInfo.astI2cData[0].u8DelayFrmNum = 1;
        g_stSnsRegsInfo.astI2cData[0].u32RegAddr = EXPOSURE_TIME_S;
        g_stSnsRegsInfo.astI2cData[5].u8DelayFrmNum = 1;
        g_stSnsRegsInfo.astI2cData[5].u32RegAddr = EXPOSURE_TIME_L;

        g_stSnsRegsInfo.astI2cData[1].u8DelayFrmNum = 1;
        g_stSnsRegsInfo.astI2cData[1].u32RegAddr = ANALOG_GAIN;
        g_stSnsRegsInfo.astI2cData[2].u8DelayFrmNum = 1;
        g_stSnsRegsInfo.astI2cData[2].u32RegAddr = DIGITAL_GAIN;

        // related registers in DCG mode
        g_stSnsRegsInfo.astI2cData[3].u8DelayFrmNum = 1;
        g_stSnsRegsInfo.astI2cData[3].u32RegAddr = HCG_ADDR;

        g_stSnsRegsInfo.astI2cData[4].u8DelayFrmNum = 0;
        g_stSnsRegsInfo.astI2cData[4].u32RegAddr = VMAX_ADDR;

        pstSnsState->bSyncInit = GK_TRUE;
    } else {
        for (i = 0; i < g_stSnsRegsInfo.u32RegNum; i++) {
            if (g_stSnsRegsInfo.astI2cData[i].u32Data == g_stPreSnsRegsInfo.astI2cData[i].u32Data) {
                g_stSnsRegsInfo.astI2cData[i].bUpdate = GK_FALSE;
            } else {
                g_stSnsRegsInfo.astI2cData[i].bUpdate = GK_TRUE;
            }
        }
    }

    if (pstSnsRegsInfo == GK_NULL) {
        printf("null pointer when get sns reg info!\n");
        return -1;
    }

    memcpy(pstSnsRegsInfo, &g_stSnsRegsInfo, sizeof(ISP_SNS_REGS_INFO_S));
    memcpy(&g_stPreSnsRegsInfo, &g_stSnsRegsInfo, sizeof(ISP_SNS_REGS_INFO_S));
    pstSnsState->au32FL[1] = pstSnsState->au32FL[0];
    return 0;
}
static GK_S32 cmos_set_image_mode(VI_PIPE ViPipe, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstSensorImageMode);
    AR0237_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);
    GK_U8 u8SensorImageMode = pstSnsState->u8ImgMode;

    pstSnsState->bSyncInit = GK_FALSE;

    if ((pstSensorImageMode->u16Width <= 1920) && (pstSensorImageMode->u16Height <= 1080)) {
        if (pstSensorImageMode->f32Fps <= 30) {
            u8SensorImageMode = SENSOR_2M_1080p30_MODE;
        }
    } else {
        ISP_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, Fps:%f\n",
                  pstSensorImageMode->u16Width,
                  pstSensorImageMode->u16Height,
                  pstSensorImageMode->f32Fps);
    }

    /* Sensor first init */
    if (pstSnsState->bInit == GK_FALSE) {
        pstSnsState->u8ImgMode = u8SensorImageMode;
        return 0;
    }

    /* Switch SensorImageMode */
    if (u8SensorImageMode == pstSnsState->u8ImgMode) {
        /* Don't need to switch SensorImageMode */
        return ISP_DO_NOT_NEED_SWITCH_IMAGEMODE;
    }

    pstSnsState->u8ImgMode = u8SensorImageMode;
    return 0;
}

static GK_VOID sensor_global_init(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    AR0237_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);
    pstSnsState->bInit = GK_FALSE;
    pstSnsState->bSyncInit = GK_FALSE;
    pstSnsState->u8ImgMode = SENSOR_2M_1080p30_MODE;
    pstSnsState->enWDRMode = WDR_MODE_NONE;
    pstSnsState->u32FLStd = FRAME_LINES_2M_LIN;;
    pstSnsState->au32FL[0] = FRAME_LINES_2M_LIN;;
    pstSnsState->au32FL[1] = FRAME_LINES_2M_LIN;;

    memset(&pstSnsState->astRegsInfo[0], 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset(&pstSnsState->astRegsInfo[1], 0, sizeof(ISP_SNS_REGS_INFO_S));
}

static GK_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    CMOS_CHECK_POINTER(pstSensorExpFunc);
    memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init = ar0237_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit = ar0237_exit;
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
static GK_S32 ar0237_set_bus_info(VI_PIPE ViPipe, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunAr0237BusInfo[ViPipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

    return GK_SUCCESS;
}

static GK_S32 sensor_ctx_init(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

    AR0237_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);

    if (pastSnsStateCtx == GK_NULL) {
        pastSnsStateCtx = (ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
        if (pastSnsStateCtx == GK_NULL) {
            ISP_TRACE(MODULE_DBG_ERR, "Isp[%d] SnsCtx malloc memory failed!\n", ViPipe);
            return ERR_CODE_ISP_NOMEM;
        }
    }

    memset(pastSnsStateCtx, 0, sizeof(ISP_SNS_STATE_S));

    AR0237_SENSOR_SET_CTX(ViPipe, pastSnsStateCtx);

    return GK_SUCCESS;
}
static GK_VOID sensor_ctx_exit(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

    AR0237_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);
    SENSOR_FREE(pastSnsStateCtx);
    AR0237_SENSOR_RESET_CTX(ViPipe);
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

    stSnsAttrInfo.eSensorId = AR0237_ID;

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

    s32Ret = GK_API_ISP_SensorUnRegCallBack(ViPipe, AR0237_ID);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor unregister callback function failed!\n");
        return s32Ret;
    }


    s32Ret = GK_API_AE_SensorUnRegCallBack(ViPipe, pstAeLib, AR0237_ID);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = GK_API_AWB_SensorUnRegCallBack(ViPipe, pstAwbLib, AR0237_ID);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor unregister callback function to awb lib failed!\n");
        return s32Ret;
    }

    sensor_ctx_exit(ViPipe);

    return GK_SUCCESS;
}

static GK_S32  sensor_set_init(VI_PIPE ViPipe, ISP_INIT_ATTR_S *pstInitAttr)
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

ISP_SNS_OBJ_S stSnsAr0237Obj = {
    .pfnRegisterCallback    = sensor_register_callback,
    .pfnUnRegisterCallback  = sensor_unregister_callback,
    .pfnStandby             = ar0237_standby,
    .pfnRestart             = ar0237_restart,
    .pfnWriteReg            = ar0237_write_register,
    .pfnReadReg             = ar0237_read_register,
    .pfnSetBusInfo          = ar0237_set_bus_info,
    .pfnSetInit             = sensor_set_init
};
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
