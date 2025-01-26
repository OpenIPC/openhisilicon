/*
 * Copyright (c) GK.  All rights reserved.
 */

#include "sp2305_cmos.h"
#include "sp2305_cmos_ex.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "gk_api_isp.h"
#include "gk_api_ae.h"
#include "gk_api_awb.h"
#include "hicompat.h"

#define SP2305_ID 2305
#define HIGH_8BITS(x) (((x) & 0xff00) >> 8)
#define LOW_8BITS(x)  ((x) & 0x00ff)

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ?  (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ?  (b) : (a))
#endif

ISP_SNS_STATE_S *g_pastSp2305[ISP_MAX_PIPE_NUM] = {GK_NULL};

#define SP2305_SENSOR_SET_CTX(dev, pstCtx)   ((g_pastSp2305[dev]) = (pstCtx))
#define SP2305_SENSOR_RESET_CTX(dev)         ((g_pastSp2305[dev]) = GK_NULL)

ISP_SNS_COMMBUS_U g_aunSp2305_BusInfo[ISP_MAX_PIPE_NUM] = {
    [0] = { .s8I2cDev = 0 },
    [1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1 }
};

static ISP_FSWDR_MODE_E g_genFSWDRMode[ISP_MAX_PIPE_NUM] = {
    [0 ... ISP_MAX_PIPE_NUM - 1] = ISP_FSWDR_NORMAL_MODE
};

ISP_SNS_COMMBUS_U *sp2305_get_bus_Info(VI_PIPE ViPipe)
{
    return &g_aunSp2305_BusInfo[ViPipe];
}

ISP_SNS_STATE_S *sp2305_get_ctx(VI_PIPE ViPipe)
{
    return g_pastSp2305[ViPipe];
}


static GK_U32 g_au32MaxTimeGetCnt[ISP_MAX_PIPE_NUM] = {0};
static GK_U32 g_au32InitExposure[ISP_MAX_PIPE_NUM]  = {0};
static GK_U32 g_au32LinesPer500ms[ISP_MAX_PIPE_NUM] = {0};
static GK_U16 g_au16InitWBGain[ISP_MAX_PIPE_NUM][3] = {{0}};
static GK_U16 g_au16SampleRgain[ISP_MAX_PIPE_NUM] = {0};
static GK_U16 g_au16SampleBgain[ISP_MAX_PIPE_NUM] = {0};

#define SP2305_FULL_LINES_MAX   0xFFFF

/* Register define */
#define PAGE_SELECT_ADDR        0xfd
#define EXP_ADDR_H              0x03
#define EXP_ADDR_L              0x04
#define AGAIN_ADDR              0x24
#define DGAIN_ADDR              0x39
#define VBLANK_H                0x05
#define VBLANK_L                0x06
#define TRIGGER_ADDR            0x01

#define PAGE_ADDR_1_INDEX       0
#define EXP_ADDR_HIGH_INDEX     1
#define EXP_ADDR_LOW_INDEX      2
#define GAIN_ADDR_INDEX         3

#define VTS_ENABLE_INDEX        4
#define VTS_ADDR_HIGH_INDEX     5
#define VTS_ADDR_LOW_INDEX      6
#define TRIGGER_INDEX           7

/* Register define */
#define PAGE_SELECT_ADDR        0xfd
#define EXP_ADDR_H              0x03
#define EXP_ADDR_L              0x04
#define AGAIN_ADDR              0x24
#define VTS_ENABLE_ADDR         0x0d
#define VTS_ADDR_H              0x0e
#define VTS_ADDR_L              0x0f
#define TRIGGER_ADDR            0x01

#define INCREASE_LINES  0  /* make real fps less than stand fps because NVR require */
#define SP2305_VMAX_1080P30_LINEAR  (0x4c1 + INCREASE_LINES)  /*  1488 */
#define SP2305_2M_1920X1080_10BIT_LINEAR30 0

static GK_VOID cmos_get_ae_comm_default(VI_PIPE ViPipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft,
    ISP_SNS_STATE_S *pstSnsState)
{
    GK_U32 u32Fll = 0;
    GK_FLOAT f32maxFps = 0;

    if (pstSnsState->u8ImgMode == SP2305_2M_1920X1080_10BIT_LINEAR30) {
        u32Fll = SP2305_VMAX_1080P30_LINEAR;
        f32maxFps = 30;  /* frame rate 30 */
    }

    pstSnsState->u32FLStd = u32Fll;
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FlickerFreq = 0;
    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
    pstAeSnsDft->stIntTimeAccu.f32Offset = 0;
    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 6; /* accuracy: 6 */
    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 0.00390625; /* accuracy: 0.00390625 */
    pstAeSnsDft->u32MaxAgain = 15872; /* max again 15872 */
    pstAeSnsDft->u32MinAgain = 1024;  /* min again 1024 */
    pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
    pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;
    pstAeSnsDft->u32MaxDgain = 256; /* 256 */
    pstAeSnsDft->u32MinDgain = 256; /* 256 */
    pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
    pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;
    pstAeSnsDft->u32ISPDgainShift = 8; /* 8 */
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 16 << pstAeSnsDft->u32ISPDgainShift; /* 16 */
    pstAeSnsDft->u32InitExposure = g_au32InitExposure[ViPipe] ? g_au32InitExposure[ViPipe] : 80000; /* 80000 */
    pstAeSnsDft->u32LinesPer500ms = g_au32LinesPer500ms[ViPipe] ? g_au32LinesPer500ms[ViPipe] :
                                    (((GK_U64)(u32Fll * f32maxFps)) >> 1);
    pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_0;
    pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_32_0;
    pstAeSnsDft->bAERouteExValid = GK_FALSE;
    pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
    pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;

    if (((gk_u32)(pstAeSnsDft->f32Fps * 100) == 2500) || /* 100, 2500 */
        ((gk_u32)(pstAeSnsDft->f32Fps * 100) == 3000)) { /* 100, 3000 */
        pstAeSnsDft->stIntTimeAccu.f32Offset = 0.8045; /* 0.8045 */
    }
    return;
}

static GK_VOID cmos_get_ae_linear_default(VI_PIPE ViPipe, ISP_SNS_STATE_S *pstSnsState,
    AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    pstAeSnsDft->au8HistThresh[0] = 0xd;
    pstAeSnsDft->au8HistThresh[1] = 0x28;
    pstAeSnsDft->au8HistThresh[2] = 0x60; /* index 2 */
    pstAeSnsDft->au8HistThresh[3] = 0x80; /* index 3 */
    pstAeSnsDft->u8AeCompensation = 0x38;
    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 4; /* minus 4 */
    pstAeSnsDft->u32MinIntTime = 2; /* mininttime 2 */
    pstAeSnsDft->u32MaxIntTimeTarget = 65535; /* max 65535 */
    pstAeSnsDft->u32MinIntTimeTarget = 2; /* min 2 */
    return;
}

static GK_S32 cmos_get_ae_default(VI_PIPE ViPipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstAeSnsDft);
    SP2305_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    (gk_void)memset_s(&pstAeSnsDft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), 0, sizeof(ISP_AE_ROUTE_S));

    cmos_get_ae_comm_default(ViPipe, pstAeSnsDft, pstSnsState);

    switch (pstSnsState->enWDRMode) {
        case WDR_MODE_NONE:   /* linear mode */
            cmos_get_ae_linear_default(ViPipe, pstSnsState, pstAeSnsDft);
            break;
        default:
            printf("cmos_get_ae_default Sensor Mode is error!\n");
            break;
    }

    return GK_SUCCESS;
}

/* the function of sensor set fps */
static GK_VOID cmos_fps_set(VI_PIPE ViPipe, GK_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;
    GK_FLOAT f32maxFps = 30;
    GK_U32 u32Lines = SP2305_VMAX_1080P30_LINEAR;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    SP2305_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    switch (pstSnsState->u8ImgMode) {
        case SP2305_2M_1920X1080_10BIT_LINEAR30:
            if ((f32Fps <= 30) && (f32Fps >= 0.56)) { /* 0.56 , 30 */
                u32Lines = (SP2305_VMAX_1080P30_LINEAR) * f32maxFps / f32Fps;
                pstAeSnsDft->stIntTimeAccu.f32Offset = 0.8045; /* 0.8045 */
            } else {
                printf("Not support Fps: %f\n", f32Fps);
                return;
            }
            pstAeSnsDft->u32LinesPer500ms = SP2305_VMAX_1080P30_LINEAR * 30 / 2; /* 30/2 */
            break;

        default:
            printf("Not support this Mode\n");
            break;
    }

    pstSnsState->u32FLStd = u32Lines;
    pstSnsState->u32FLStd = (pstSnsState->u32FLStd > SP2305_FULL_LINES_MAX) ?
        SP2305_FULL_LINES_MAX : pstSnsState->u32FLStd;

    pstAeSnsDft->f32Fps = f32Fps;

    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 4; /* 4 */
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];

    if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
        pstSnsState->astRegsInfo[0].astI2cData[VTS_ADDR_HIGH_INDEX].u32Data = HIGH_8BITS(pstSnsState->u32FLStd);
        pstSnsState->astRegsInfo[0].astI2cData[VTS_ADDR_LOW_INDEX].u32Data = LOW_8BITS(pstSnsState->u32FLStd);
    }

    return;
}

static GK_VOID cmos_slow_framerate_set(VI_PIPE ViPipe, GK_U32 u32FullLines, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    SP2305_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->au32FL[0] = (u32FullLines > SP2305_FULL_LINES_MAX) ? SP2305_FULL_LINES_MAX : u32FullLines;
    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
    pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 4; /* 4 */

    if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
        pstSnsState->astRegsInfo[0].astI2cData[VTS_ADDR_HIGH_INDEX].u32Data = HIGH_8BITS(pstAeSnsDft->u32FullLines);
        pstSnsState->astRegsInfo[0].astI2cData[VTS_ADDR_LOW_INDEX].u32Data = LOW_8BITS(pstAeSnsDft->u32FullLines);
    }

    return;
}

#define AGAIN_NODE_NUM 64
static GK_U32 g_au32Again_table[AGAIN_NODE_NUM] = {
    1024, 1088, 1152, 1216, 1280, 1344, 1408, 1472, 1536, 1600, 1664, 1728, 1792, 1856, 1920, 1984,
    2048, 2176, 2304, 2432, 2560, 2688, 2816, 2944, 3072, 3200, 3328, 3456, 3584, 3712, 3840, 3968,
    4096, 4352, 4608, 4864, 5120, 5376, 5632, 5888, 6144, 6400, 6656, 6912, 7168, 7424, 7680, 7936,
    8192, 8704, 9216, 9728, 10240, 10752, 11264, 11776, 12288, 12800, 13312, 13824, 14336, 14848, 15360, 15872
};

/* while isp notify ae to update sensor regs, ae call these funcs. */
static GK_VOID cmos_inttime_update(VI_PIPE ViPipe, GK_U32 u32IntTime)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    SP2305_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (u32IntTime <= 2) { /* 2 */
        u32IntTime = 2; /* 2 */
    }

    pstSnsState->astRegsInfo[0].astI2cData[EXP_ADDR_HIGH_INDEX].u32Data = HIGH_8BITS(u32IntTime);
    pstSnsState->astRegsInfo[0].astI2cData[EXP_ADDR_LOW_INDEX].u32Data =  LOW_8BITS(u32IntTime);

    return;
}

static GK_VOID cmos_again_calc_table(VI_PIPE ViPipe, GK_U32 *pu32AgainLin, GK_U32 *pu32AgainDb)
{
    int i;

    CMOS_CHECK_POINTER_VOID(pu32AgainLin);
    CMOS_CHECK_POINTER_VOID(pu32AgainDb);

    if (*pu32AgainLin >= g_au32Again_table[AGAIN_NODE_NUM - 1]) {
        *pu32AgainLin = g_au32Again_table[AGAIN_NODE_NUM - 1];
        *pu32AgainDb = AGAIN_NODE_NUM - 1;
        return;
    }

    for (i = 1; i < AGAIN_NODE_NUM; i++) {
        if (*pu32AgainLin < g_au32Again_table[i]) {
            *pu32AgainLin = g_au32Again_table[i - 1];
            *pu32AgainDb = i - 1;
            break;
        }
    }
    return;
}

static GK_VOID cmos_gains_update(VI_PIPE ViPipe, GK_U32 u32Again, GK_U32 u32Dgain)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;
    SP2305_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);
    GK_U32 temp_gain;

    temp_gain = (GK_U32)(g_au32Again_table[u32Again] / 1024.0 * 16.0); /* 1024.0, 16.0 */

    pstSnsState->astRegsInfo[0].astI2cData[GAIN_ADDR_INDEX].u32Data = temp_gain;

    return;
}

/* Only used in FSWDR mode */
static GK_VOID cmos_ae_fswdr_attr_set(VI_PIPE ViPipe, AE_FSWDR_ATTR_S *pstAeFSWDRAttr)
{
    g_genFSWDRMode[ViPipe] = pstAeFSWDRAttr->enFSWDRMode;
    g_au32MaxTimeGetCnt[ViPipe] = 0;
    return;
}

static GK_S32 cmos_init_ae_exp_function(AE_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    CMOS_CHECK_POINTER(pstExpFuncs);

    (gk_void)memset_s(pstExpFuncs, sizeof(AE_SENSOR_EXP_FUNC_S), 0, sizeof(AE_SENSOR_EXP_FUNC_S));

    pstExpFuncs->pfn_cmos_get_ae_default     = cmos_get_ae_default;
    pstExpFuncs->pfn_cmos_fps_set            = cmos_fps_set;
    pstExpFuncs->pfn_cmos_slow_framerate_set = cmos_slow_framerate_set;
    pstExpFuncs->pfn_cmos_inttime_update     = cmos_inttime_update;
    pstExpFuncs->pfn_cmos_gains_update       = cmos_gains_update;
    pstExpFuncs->pfn_cmos_again_calc_table   = cmos_again_calc_table;
    pstExpFuncs->pfn_cmos_dgain_calc_table   = GK_NULL;
    pstExpFuncs->pfn_cmos_get_inttime_max    = GK_NULL;
    pstExpFuncs->pfn_cmos_ae_fswdr_attr_set  = cmos_ae_fswdr_attr_set;

    return GK_SUCCESS;
}

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN                      0
#define GOLDEN_BGAIN                      0
static GK_S32 cmos_get_awb_default(VI_PIPE ViPipe, AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstAwbSnsDft);
    SP2305_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    (gk_void)memset_s(pstAwbSnsDft, sizeof(AWB_SENSOR_DEFAULT_S), 0, sizeof(AWB_SENSOR_DEFAULT_S));

    pstAwbSnsDft->u16WbRefTemp = 4850; /* Color Temp 4850 */

    pstAwbSnsDft->au16GainOffset[0] = 0x1A6;
    pstAwbSnsDft->au16GainOffset[1] = 0x100;
    pstAwbSnsDft->au16GainOffset[2] = 0x100; /* 2 */
    pstAwbSnsDft->au16GainOffset[3] = 0x1A5; /* 3 */

    pstAwbSnsDft->as32WbPara[0] = -6;     /* -6 */
    pstAwbSnsDft->as32WbPara[1] = 262;    /* 262 */
    pstAwbSnsDft->as32WbPara[2] = 0;      /* 2 */
    pstAwbSnsDft->as32WbPara[3] = 215604; /* 3, 215604 */
    pstAwbSnsDft->as32WbPara[4] = 128;    /* 4, 128 */
    pstAwbSnsDft->as32WbPara[5] = -165255; /* 5, -165255 */

    pstAwbSnsDft->u16GoldenRgain = GOLDEN_RGAIN;
    pstAwbSnsDft->u16GoldenBgain = GOLDEN_BGAIN;

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:
            (gk_void)memcpy_s(&pstAwbSnsDft->stCcm, sizeof(AWB_CCM_S), &g_stAwbCcm, sizeof(AWB_CCM_S));
            (gk_void)memcpy_s(&pstAwbSnsDft->stAgcTbl, sizeof(AWB_AGC_TABLE_S),
                              &g_stAwbAgcTable, sizeof(AWB_AGC_TABLE_S));
            break;
    }
    pstAwbSnsDft->u16InitRgain   = g_au16InitWBGain[ViPipe][0];
    pstAwbSnsDft->u16InitGgain   = g_au16InitWBGain[ViPipe][1];
    pstAwbSnsDft->u16InitBgain   = g_au16InitWBGain[ViPipe][2]; /* 2 */
    pstAwbSnsDft->u16SampleRgain = g_au16SampleRgain[ViPipe];
    pstAwbSnsDft->u16SampleBgain = g_au16SampleBgain[ViPipe];

    return GK_SUCCESS;
}

static GK_S32 cmos_init_awb_exp_function(AWB_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    CMOS_CHECK_POINTER(pstExpFuncs);

    (gk_void)memset_s(pstExpFuncs, sizeof(AWB_SENSOR_EXP_FUNC_S), 0, sizeof(AWB_SENSOR_EXP_FUNC_S));

    pstExpFuncs->pfn_cmos_get_awb_default = cmos_get_awb_default;

    return GK_SUCCESS;
}


static ISP_CMOS_DNG_COLORPARAM_S g_stDngColorParam = {
    {378, 256, 430},
    {439, 256, 439}
};

static GK_VOID cmos_get_isp_dng_default(ISP_SNS_STATE_S *pstSnsState, ISP_CMOS_DEFAULT_S *pstDef)
{

    (gk_void)memcpy_s(&pstDef->stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S),
                      &g_stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S));

    pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleH.u32Denominator = 1;
    pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleH.u32Numerator = 1;
    pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleV.u32Denominator = 1;
    pstDef->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleV.u32Numerator = 1;
    pstDef->stSensorMode.stDngRawFormat.stCfaRepeatPatternDim.u16RepeatPatternDimRows = 2; /* pattern 2  */
    pstDef->stSensorMode.stDngRawFormat.stCfaRepeatPatternDim.u16RepeatPatternDimCols = 2; /* pattern 2  */
    pstDef->stSensorMode.stDngRawFormat.stBlcRepeatDim.u16BlcRepeatRows = 2; /* pattern 2  */
    pstDef->stSensorMode.stDngRawFormat.stBlcRepeatDim.u16BlcRepeatCols = 2; /* pattern 2  */
    pstDef->stSensorMode.stDngRawFormat.enCfaLayout = CFALAYOUT_TYPE_RECTANGULAR;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPlaneColor[0] = 0;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPlaneColor[1] = 1;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPlaneColor[2] = 2; /* pattern 2  */
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[0] = 0;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[1] = 1;
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[2] = 1; /* index 2 */
    pstDef->stSensorMode.stDngRawFormat.au8CfaPattern[3] = 2; /* index 3 pattern 2  */
    pstDef->stSensorMode.bValidDngRawFormat = GK_TRUE;
    return;
}

static GK_VOID cmos_get_isp_linear_default(ISP_CMOS_DEFAULT_S *pstDef)
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
    pstDef->unKey.bit1Ge             = 1;
    pstDef->pstGe                    = &g_stIspGe;
    pstDef->unKey.bit1Dehaze         = 1;
    pstDef->pstDehaze                = &g_stIspDehaze;
    pstDef->unKey.bit1Lcac           = 0;
    pstDef->pstLcac                  = &g_stIspLCac;
    pstDef->unKey.bit1Rgbir          = 0;
    (gk_void)memcpy_s(&pstDef->stNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S),
                      &g_stIspNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
    return;
}

static GK_S32 cmos_get_isp_default(VI_PIPE ViPipe, ISP_CMOS_DEFAULT_S *pstDef)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstDef);
    SP2305_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    (gk_void)memset_s(pstDef, sizeof(ISP_CMOS_DEFAULT_S), 0, sizeof(ISP_CMOS_DEFAULT_S));

    pstDef->unKey.bit1Ca       = 1;
    pstDef->pstCa              = &g_stIspCA;
    pstDef->unKey.bit1Dpc      = 1;
    pstDef->pstDpc             = &g_stCmosDpc;
    pstDef->unKey.bit1Wdr      = 1;
    pstDef->pstWdr             = &g_stIspWDR;
    pstDef->unKey.bit1Lsc      = 1;
    pstDef->pstLsc             = &g_stCmosLsc;

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:
            cmos_get_isp_linear_default(pstDef);
            break;
    }
    pstDef->stSensorMode.u32SensorID = SP2305_ID;
    pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;
    cmos_get_isp_dng_default(pstSnsState, pstDef);
    return GK_SUCCESS;
}

static GK_S32 cmos_get_isp_black_level(VI_PIPE ViPipe, ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
    GK_S32  i;
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstBlackLevel);
    SP2305_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    /* Don't need to update black level when iso change */
    pstBlackLevel->bUpdate = GK_FALSE;
    if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
        for (i = 0; i < 4; i++) { /* 4 */
            pstBlackLevel->au16BlackLevel[i] = 0x100;
        }
    }

    return GK_SUCCESS;
}

static GK_VOID cmos_set_pixel_detect(VI_PIPE ViPipe, GK_BOOL bEnable)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    SP2305_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (bEnable) { /* setup for ISP pixel calibration mode */
        sp2305_write_register(ViPipe, 0xfd, 0x01); /*  the first page 5fps */
        sp2305_write_register(ViPipe, 0x0d, 0x10); /*  manually modify VTS */
        sp2305_write_register(ViPipe, 0x0e, 0x0E); /*  VTS*3 */
        sp2305_write_register(ViPipe, 0x0f, 0x43);
        sp2305_write_register(ViPipe, 0x03, 0x0E); /*  exportion VTS*3 -4 */
        sp2305_write_register(ViPipe, 0x04, 0x3F);
        sp2305_write_register(ViPipe, 0x01, 0x01); /*  trigger */
    } else { /* setup for ISP 'normal mode' */
        sp2305_write_register(ViPipe, 0xfd, 0x01); /*  the first page */
        sp2305_write_register(ViPipe, 0x0d, 0x10); /*  manually modify VTS */
        sp2305_write_register(ViPipe, 0x0e, 0x04); /*  VTS - 4 */
        sp2305_write_register(ViPipe, 0x0f, 0xc1);
        sp2305_write_register(ViPipe, 0x03, 0x04); /*  exportion */
        sp2305_write_register(ViPipe, 0x04, 0x48);
        sp2305_write_register(ViPipe, 0x01, 0x01);
        pstSnsState->bSyncInit = GK_FALSE;
    }
    return;
}

static GK_S32 cmos_set_wdr_mode(VI_PIPE ViPipe, GK_U8 u8Mode)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    SP2305_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    pstSnsState->bSyncInit = GK_FALSE;

    switch (u8Mode) {
        case WDR_MODE_NONE:
            pstSnsState->enWDRMode = WDR_MODE_NONE;
            pstSnsState->u8ImgMode = SP2305_2M_1920X1080_10BIT_LINEAR30;
            pstSnsState->u32FLStd  = SP2305_VMAX_1080P30_LINEAR;
            printf("linear mode\n");
            break;
        default:
            ISP_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            break;
    }
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstSnsState->au32FL[1] = pstSnsState->au32FL[0];
    (gk_void)memset_s(pstSnsState->au32WDRIntTime,
                      sizeof(pstSnsState->au32WDRIntTime), 0, sizeof(pstSnsState->au32WDRIntTime));

    return GK_SUCCESS;
}

static GK_VOID cmos_comm_sns_reg_info_init(VI_PIPE ViPipe, ISP_SNS_STATE_S *pstSnsState)
{
    gk_u32 i;

    pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
    pstSnsState->astRegsInfo[0].unComBus.s8I2cDev = g_aunSp2305_BusInfo[ViPipe].s8I2cDev;
    pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2; /* 2 */
    pstSnsState->astRegsInfo[0].u32RegNum = 8; /* 8 */

    for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
        pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = GK_TRUE;
        pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr = SP2305_I2C_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[i].u32AddrByteNum = SP2305_ADDR_BYTE;
        pstSnsState->astRegsInfo[0].astI2cData[i].u32DataByteNum = SP2305_DATA_BYTE;
    }
    pstSnsState->astRegsInfo[0].astI2cData[PAGE_ADDR_1_INDEX].u8DelayFrmNum     = 0;         /* page */
    pstSnsState->astRegsInfo[0].astI2cData[PAGE_ADDR_1_INDEX].u32RegAddr        = 0xFD;
    pstSnsState->astRegsInfo[0].astI2cData[EXP_ADDR_HIGH_INDEX].u8DelayFrmNum   = 0;         /* exporsion */
    pstSnsState->astRegsInfo[0].astI2cData[EXP_ADDR_HIGH_INDEX].u32RegAddr      = 0x03;
    pstSnsState->astRegsInfo[0].astI2cData[EXP_ADDR_LOW_INDEX].u8DelayFrmNum    = 0;          /* exporsion */
    pstSnsState->astRegsInfo[0].astI2cData[EXP_ADDR_LOW_INDEX].u32RegAddr       = 0x04;
    pstSnsState->astRegsInfo[0].astI2cData[GAIN_ADDR_INDEX].u8DelayFrmNum       = 0;          /* analog gain */
    pstSnsState->astRegsInfo[0].astI2cData[GAIN_ADDR_INDEX].u32RegAddr          = 0x24;

    pstSnsState->astRegsInfo[0].astI2cData[VTS_ENABLE_INDEX].u8DelayFrmNum      = 0;          /* Digital gain */
    pstSnsState->astRegsInfo[0].astI2cData[VTS_ENABLE_INDEX].u32RegAddr         = 0x0D;
    pstSnsState->astRegsInfo[0].astI2cData[VTS_ADDR_HIGH_INDEX].u8DelayFrmNum   = 0;          /* trigger */
    pstSnsState->astRegsInfo[0].astI2cData[VTS_ADDR_HIGH_INDEX].u32RegAddr      = 0x0E;
    pstSnsState->astRegsInfo[0].astI2cData[VTS_ADDR_LOW_INDEX].u8DelayFrmNum    = 0;          /* trigger */
    pstSnsState->astRegsInfo[0].astI2cData[VTS_ADDR_LOW_INDEX].u32RegAddr       = 0x0F;
    pstSnsState->astRegsInfo[0].astI2cData[TRIGGER_INDEX].u8DelayFrmNum         = 0;          /* trigger */
    pstSnsState->astRegsInfo[0].astI2cData[TRIGGER_INDEX].u32RegAddr            = 0x01;

    pstSnsState->astRegsInfo[0].astI2cData[PAGE_ADDR_1_INDEX].u32Data           = 0x01; /* init the page, VTS enable */
    pstSnsState->astRegsInfo[0].astI2cData[VTS_ENABLE_INDEX].u32Data            = 0x10;
    pstSnsState->astRegsInfo[0].astI2cData[TRIGGER_INDEX].u32Data               = 0x01;

    pstSnsState->bSyncInit = GK_TRUE;
    return;
}

static GK_VOID cmos_sns_reg_info_update(VI_PIPE ViPipe, ISP_SNS_STATE_S *pstSnsState)
{
    gk_u32 i;

    for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
        if (pstSnsState->astRegsInfo[0].astI2cData[i].u32Data == pstSnsState->astRegsInfo[1].astI2cData[i].u32Data) {
            pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = GK_FALSE;
        } else {
            pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = GK_TRUE;
        }
    }

    pstSnsState->astRegsInfo[0].astI2cData[PAGE_ADDR_1_INDEX].bUpdate = GK_TRUE;      /* always refresh the data  */
    pstSnsState->astRegsInfo[0].astI2cData[VTS_ENABLE_INDEX].bUpdate  = GK_TRUE;
    pstSnsState->astRegsInfo[0].astI2cData[TRIGGER_INDEX].bUpdate     = GK_TRUE;
    return;
}

static GK_S32 cmos_get_sns_regs_info(VI_PIPE ViPipe, ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstSnsRegsInfo);
    SP2305_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    if ((pstSnsState->bSyncInit == GK_FALSE) || (pstSnsRegsInfo->bConfig == GK_FALSE)) {
        cmos_comm_sns_reg_info_init(ViPipe, pstSnsState);
    } else {
        cmos_sns_reg_info_update(ViPipe, pstSnsState);
    }

    pstSnsRegsInfo->bConfig = GK_FALSE;
    (gk_void)memcpy_s(pstSnsRegsInfo, sizeof(ISP_SNS_REGS_INFO_S),
                      &pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S));
    (gk_void)memcpy_s(&pstSnsState->astRegsInfo[1], sizeof(ISP_SNS_REGS_INFO_S),
                      &pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S));

    pstSnsState->au32FL[1] = pstSnsState->au32FL[0];

    return GK_SUCCESS;
}

static GK_S32 cmos_set_image_mode(VI_PIPE ViPipe, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode)
{
    GK_U8 u8SensorImageMode;
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstSensorImageMode);
    SP2305_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    u8SensorImageMode = pstSnsState->u8ImgMode;
    pstSnsState->bSyncInit = GK_FALSE;

    if (pstSensorImageMode == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "null pointer when set image mode\n");
        return GK_FAILURE;
    }

    if ((pstSensorImageMode->u16Width <= 1920) && (pstSensorImageMode->u16Height <= 1080)) { /* 1920, 1080 */
        if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
            if (pstSensorImageMode->f32Fps <= 30) { /* 30 */
                u8SensorImageMode = SP2305_2M_1920X1080_10BIT_LINEAR30;
            } else {
                ISP_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
                    pstSensorImageMode->u16Width,
                    pstSensorImageMode->u16Height,
                    pstSensorImageMode->f32Fps,
                    pstSnsState->enWDRMode);
                return GK_FAILURE;
            }
        }
    } else {
        ISP_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
            pstSensorImageMode->u16Width,
            pstSensorImageMode->u16Height,
            pstSensorImageMode->f32Fps,
            pstSnsState->enWDRMode);

        return GK_FAILURE;
    }

    /* Sensor first init */
    if (pstSnsState->bInit == GK_FALSE) {
        pstSnsState->u8ImgMode = u8SensorImageMode;
        return GK_SUCCESS;
    }

    /* Switch SensorImageMode */
    if (u8SensorImageMode == pstSnsState->u8ImgMode) {
        /* Don't need to switch SensorImageMode */
        return GK_FAILURE;
    }

    pstSnsState->u8ImgMode = u8SensorImageMode;
    (gk_void)memset_s(pstSnsState->au32WDRIntTime,
                      sizeof(pstSnsState->au32WDRIntTime), 0, sizeof(pstSnsState->au32WDRIntTime));

    return GK_SUCCESS;
}

static GK_VOID sensor_global_init(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    SP2305_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->bInit     = GK_FALSE;
    pstSnsState->bSyncInit = GK_FALSE;
    pstSnsState->u8ImgMode = SP2305_2M_1920X1080_10BIT_LINEAR30;
    pstSnsState->enWDRMode = WDR_MODE_NONE;
    pstSnsState->u32FLStd  = SP2305_VMAX_1080P30_LINEAR;
    pstSnsState->au32FL[0] = SP2305_VMAX_1080P30_LINEAR;
    pstSnsState->au32FL[1] = SP2305_VMAX_1080P30_LINEAR;

    (gk_void)memset_s(&pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
    (gk_void)memset_s(&pstSnsState->astRegsInfo[1], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
    return;
}

static GK_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    CMOS_CHECK_POINTER(pstSensorExpFunc);

    (gk_void)memset_s(pstSensorExpFunc, sizeof(ISP_SENSOR_EXP_FUNC_S), 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init         = sp2305_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit         = sp2305_exit;
    pstSensorExpFunc->pfn_cmos_sensor_global_init  = sensor_global_init;
    pstSensorExpFunc->pfn_cmos_set_image_mode      = cmos_set_image_mode;
    pstSensorExpFunc->pfn_cmos_set_wdr_mode        = cmos_set_wdr_mode;
    pstSensorExpFunc->pfn_cmos_get_isp_default     = cmos_get_isp_default;
    pstSensorExpFunc->pfn_cmos_get_isp_black_level = cmos_get_isp_black_level;
    pstSensorExpFunc->pfn_cmos_set_pixel_detect    = cmos_set_pixel_detect;
    pstSensorExpFunc->pfn_cmos_get_sns_reg_info    = cmos_get_sns_regs_info;

    return GK_SUCCESS;
}

static GK_S32 sp2305_set_bus_info(VI_PIPE ViPipe, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunSp2305_BusInfo[ViPipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

    return GK_SUCCESS;
}

static GK_S32 sensor_ctx_init(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

    SP2305_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);

    if (pastSnsStateCtx == GK_NULL) {
        pastSnsStateCtx = (ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
        if (pastSnsStateCtx == GK_NULL) {
            ISP_TRACE(MODULE_DBG_ERR, "Isp[%d] SnsCtx malloc memory failed!\n", ViPipe);
            return ERR_CODE_ISP_NOMEM;
        }
    }

    (gk_void)memset_s(pastSnsStateCtx, sizeof(ISP_SNS_STATE_S), 0, sizeof(ISP_SNS_STATE_S));

    SP2305_SENSOR_SET_CTX(ViPipe, pastSnsStateCtx);

    return GK_SUCCESS;
}

static GK_VOID sensor_ctx_exit(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

    SP2305_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);
    SENSOR_FREE(pastSnsStateCtx);
    SP2305_SENSOR_RESET_CTX(ViPipe);
    return;
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

    stSnsAttrInfo.eSensorId = SP2305_ID;
    s32Ret  = cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
    s32Ret += GK_API_ISP_SensorRegCallBack(ViPipe, &stSnsAttrInfo, &stIspRegister);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor register callback function failed!\n");
        return s32Ret;
    }

    s32Ret  = cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
    s32Ret += GK_API_AE_SensorRegCallBack(ViPipe, pstAeLib, &stSnsAttrInfo, &stAeRegister);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor register callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret  = cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
    s32Ret += GK_API_AWB_SensorRegCallBack(ViPipe, pstAwbLib, &stSnsAttrInfo, &stAwbRegister);
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

    s32Ret = GK_API_ISP_SensorUnRegCallBack(ViPipe, SP2305_ID);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor unregister callback function failed!\n");
        return s32Ret;
    }

    s32Ret = GK_API_AE_SensorUnRegCallBack(ViPipe, pstAeLib, SP2305_ID);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = GK_API_AWB_SensorUnRegCallBack(ViPipe, pstAwbLib, SP2305_ID);
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

    g_au32InitExposure[ViPipe]  = pstInitAttr->u32Exposure;
    g_au32LinesPer500ms[ViPipe] = pstInitAttr->u32LinesPer500ms;
    g_au16InitWBGain[ViPipe][0] = pstInitAttr->u16WBRgain;
    g_au16InitWBGain[ViPipe][1] = pstInitAttr->u16WBGgain;
    g_au16InitWBGain[ViPipe][2] = pstInitAttr->u16WBBgain; /* 2 */
    g_au16SampleRgain[ViPipe]   = pstInitAttr->u16SampleRgain;
    g_au16SampleBgain[ViPipe]   = pstInitAttr->u16SampleBgain;

    return GK_SUCCESS;
}

ISP_SNS_OBJ_S stSnsSp2305Obj = {
    .pfnRegisterCallback    = sensor_register_callback,
    .pfnUnRegisterCallback  = sensor_unregister_callback,
    .pfnStandby             = sp2305_standby,
    .pfnRestart             = sp2305_restart,
    .pfnMirrorFlip          = sp2305_mirror_flip,
    .pfnWriteReg            = sp2305_write_register,
    .pfnReadReg             = sp2305_read_register,
    .pfnSetBusInfo          = sp2305_set_bus_info,
    .pfnSetInit             = sensor_set_init
};

