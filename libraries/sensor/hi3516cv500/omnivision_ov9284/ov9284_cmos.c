/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of ov12870_cmos.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "ov9284_cmos.h"
#include "ov9284_cmos_ex.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "mpi_isp.h"
#include "mpi_ae.h"
#include "mpi_awb.h"

#define OV9284_ID 9284

#define HIGH_8_BITS(x) (((x) & 0xff00) >> 8)
#define LOW_8_BITS(x)  ((x) & 0x00ff)

ISP_SNS_STATE_S *g_pastOv9284[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define OV9284_SENSOR_SET_CTX(dev, pstCtx)   ((g_pastOv9284[dev]) = (pstCtx))
#define OV9284_SENSOR_RESET_CTX(dev)         (g_pastOv9284[dev] = HI_NULL)

static HI_U32 g_au32InitExposure[ISP_MAX_PIPE_NUM]  = {0};
static HI_U32 g_au32LinesPer500ms[ISP_MAX_PIPE_NUM] = {0};
static HI_U16 g_au16InitWBGain[ISP_MAX_PIPE_NUM][ISP_RGB_CHN_NUM] = {{0}};
static HI_U16 g_au16SampleRgain[ISP_MAX_PIPE_NUM] = {0};
static HI_U16 g_au16SampleBgain[ISP_MAX_PIPE_NUM] = {0};
static HI_BOOL g_quick_start_en[ISP_MAX_PIPE_NUM] = {HI_FALSE};

static HI_U32 g_au32Again[ISP_MAX_PIPE_NUM] = {1024};

static HI_BOOL g_abAERouteExValid[ISP_MAX_PIPE_NUM] = {0};
static ISP_AE_ROUTE_S g_astInitAERoute[ISP_MAX_PIPE_NUM] = {{0}};
static ISP_AE_ROUTE_EX_S g_astInitAERouteEx[ISP_MAX_PIPE_NUM] = {{0}};

ISP_SNS_COMMBUS_U g_aunOv9284BusInfo[ISP_MAX_PIPE_NUM] = {
    [0] = { .s8I2cDev = 0},
    [1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1}
};

ISP_SNS_COMMBUS_U *ov9284_get_bus_Info(VI_PIPE vi_pipe)
{
    return &g_aunOv9284BusInfo[vi_pipe];
}

ISP_SNS_STATE_S *ov9284_get_ctx(VI_PIPE vi_pipe)
{
    return g_pastOv9284[vi_pipe];
}

const OV9284_VIDEO_MODE_TBL_S g_astOv9284ModeTbl[OV9284_MODE_BUTT] = {
    {0x0727, 0, 6, 12, 60, 0, "800P_10BIT_60FPS"  },
};

const OV9284_VIDEO_MODE_TBL_S *ov9284_get_mode_tb1(HI_U8 u8ImgMode)
{
    return &g_astOv9284ModeTbl[u8ImgMode];
}

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
#define OV9284_FULL_LINES_MAX  0xFFFF

#define OV9284_ERR_MODE_PRINT(pstSensorImageMode)\
    do{\
        SNS_ERR_TRACE("Not support! Width:%d, Height:%d, Fps:%f, SnsMode:%d\n",\
                      (pstSensorImageMode)->u16Width,  \
                      (pstSensorImageMode)->u16Height, \
                      (pstSensorImageMode)->f32Fps,    \
                      (pstSensorImageMode)->u8SnsMode);\
    }while (0)

static HI_VOID cmos_get_ae_comm_default(VI_PIPE vi_pipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft,
    ISP_SNS_STATE_S *pstSnsState)
{
    HI_U32 u32Fll;
    HI_FLOAT f32MaxFps;
    u32Fll = g_astOv9284ModeTbl[pstSnsState->u8ImgMode].u32VMax;
    f32MaxFps = g_astOv9284ModeTbl[pstSnsState->u8ImgMode].f32MaxFps;
    pstAeSnsDft->stIntTimeAccu.f32Offset = g_astOv9284ModeTbl[pstSnsState->u8ImgMode].u32Offset;
    pstSnsState->u32FLStd = u32Fll;
    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - g_astOv9284ModeTbl[pstSnsState->u8ImgMode].u32ExpLineLimit;

    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FlickerFreq = 0;
    pstAeSnsDft->u32HmaxTimes = (1000000000) / SNS_DIV_0_TO_1_FLOAT(u32Fll * f32MaxFps); /* 1000000000ns */

    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;

    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 0.0625; /* Accuracy 0.0625 */

    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 0.0625; /* Accuracy 0.0625 */

    pstAeSnsDft->u32ISPDgainShift = 8; /* accuracy: 8 */
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 4 << pstAeSnsDft->u32ISPDgainShift; /* max 4 */

    memcpy_s(&pstAeSnsDft->stPirisAttr, sizeof(ISP_PIRIS_ATTR_S), &g_gstPirisAttr, sizeof(ISP_PIRIS_ATTR_S));
    pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_4;
    pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_5_6;

    pstAeSnsDft->bAERouteExValid = HI_FALSE;
    pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
    pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;
    if (g_au32InitExposure[vi_pipe] == 0) {
        pstAeSnsDft->u32InitExposure = 1000000; /* init 1000000 */
    } else {
        pstAeSnsDft->u32InitExposure = g_au32InitExposure[vi_pipe];
    }

    if (g_au32LinesPer500ms[vi_pipe] == 0) {
        pstAeSnsDft->u32LinesPer500ms = ((HI_U64)(u32Fll * f32MaxFps)) >> 1;
    } else {
        pstAeSnsDft->u32LinesPer500ms = g_au32LinesPer500ms[vi_pipe];
    }
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

    pstAeSnsDft->u8AeCompensation = 0x2D;
    pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;

    pstAeSnsDft->u32MinIntTime = g_astOv9284ModeTbl[pstSnsState->u8ImgMode].u32ExpLineMin;
    pstAeSnsDft->u32MaxIntTimeTarget = 65535; /* max 65535 */
    pstAeSnsDft->u32MinIntTimeTarget = pstAeSnsDft->u32MinIntTime;

    pstAeSnsDft->u32MaxAgain = 255; /* max 255 is 16 x */
    pstAeSnsDft->u32MinAgain = 16; /* min 16 is 1x */
    pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
    pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

    pstAeSnsDft->u32MaxDgain = 16; /* max 16 */
    pstAeSnsDft->u32MinDgain = 16; /* min 16 */
    pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
    pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

    pstAeSnsDft->bAERouteExValid = g_abAERouteExValid[vi_pipe];
    memcpy_s(&pstAeSnsDft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), &g_astInitAERoute[vi_pipe], sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&pstAeSnsDft->stAERouteAttrEx, sizeof(ISP_AE_ROUTE_EX_S),
        &g_astInitAERouteEx[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S));
    return;
}

static HI_S32 cmos_get_ae_default(VI_PIPE vi_pipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstAeSnsDft);
    OV9284_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    cmos_get_ae_comm_default(vi_pipe, pstAeSnsDft, pstSnsState);

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE: {
            cmos_get_ae_linear_default(vi_pipe, pstAeSnsDft, pstSnsState);
            break;
        }
    }
    return HI_SUCCESS;
}

/* the function of sensor set fps */
static HI_VOID cmos_fps_set(VI_PIPE vi_pipe, HI_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    HI_FLOAT f32MaxFps;
    HI_U32 u32Lines;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    OV9284_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    f32MaxFps = g_astOv9284ModeTbl[pstSnsState->u8ImgMode].f32MaxFps;
    u32Lines = g_astOv9284ModeTbl[pstSnsState->u8ImgMode].u32VMax * (f32MaxFps / SNS_DIV_0_TO_1_FLOAT(f32Fps));
    pstSnsState->u32FLStd = u32Lines;
    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - g_astOv9284ModeTbl[pstSnsState->u8ImgMode].u32ExpLineLimit;

    /* SHR 16bit, So limit full_lines as 0xFFFF */
    if (f32Fps > f32MaxFps || u32Lines > OV9284_FULL_LINES_MAX) {
        SNS_ERR_TRACE("Not support Fps: %f\n", f32Fps);
        return;
    }

    pstAeSnsDft->f32Fps = f32Fps;
    pstAeSnsDft->u32LinesPer500ms = g_astOv9284ModeTbl[pstSnsState->u8ImgMode].u32VMax * f32MaxFps / 2; /* div 2 */
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;

    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
    pstAeSnsDft->u32HmaxTimes = (1000000000) / SNS_DIV_0_TO_1_FLOAT(pstSnsState->u32FLStd * f32Fps); /* 1000000000ns */

    pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = 0x0;
    pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = 0x10; /* index 6 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = 0xa0; /* index 7 */

    pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = LOW_8_BITS(pstSnsState->au32FL[0]); /* index 8 */
    pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = HIGH_8_BITS(pstSnsState->au32FL[0]); /* index 9 */

    return;
}

static HI_VOID cmos_slow_framerate_set(VI_PIPE vi_pipe, HI_U32 u32FullLines,
    AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    OV9284_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32FullLines = (u32FullLines > OV9284_FULL_LINES_MAX) ? OV9284_FULL_LINES_MAX : u32FullLines;
    pstSnsState->au32FL[0] = u32FullLines;

    pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = LOW_8_BITS(pstSnsState->au32FL[0]); /* index 8 */
    pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = HIGH_8_BITS(pstSnsState->au32FL[0]); /* index 9 */

    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
    pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 12; /* 12 sensor limit */

    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static HI_VOID cmos_inttime_update(VI_PIPE vi_pipe, HI_U32 u32IntTime)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    OV9284_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = 0x0;
    pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = 0x10; /* 6 index */
    pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = 0xa0; /* 7 index */

    pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = (u32IntTime << 4) & 0xF0; /* 4 bit shift 3 index */
    pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = (u32IntTime >> 4) & 0xFF; /* 4 bit shift 2 index */
    pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = (u32IntTime >> 12) & 0xF; /* 12 bit shift */

    return;
}

static HI_VOID cmos_again_calc_table(VI_PIPE vi_pipe, HI_U32 *pu32AgainLin, HI_U32 *pu32AgainDb)
{
    return;
}

static HI_VOID cmos_dgain_calc_table(VI_PIPE vi_pipe, HI_U32 *pu32DgainLin, HI_U32 *pu32DgainDb)
{
    return;
}

static HI_VOID cmos_gains_update(VI_PIPE vi_pipe, HI_U32 u32Again, HI_U32 u32Dgain)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    OV9284_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = 0x0;
    pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = 0x10; /* 6 index */
    pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = 0xa0; /* 7 index */

    pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = (u32Again & 0xFF); /* 5 index */

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

    return HI_SUCCESS;
}

/* AWB default parameter and function */
#define CALIBRATE_STATIC_TEMP      4983
#define CALIBRATE_STATIC_WB_R_GAIN 256
#define CALIBRATE_STATIC_WB_GR_GAIN 256
#define CALIBRATE_STATIC_WB_GB_GAIN 256
#define CALIBRATE_STATIC_WB_B_GAIN 256

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1 30
#define CALIBRATE_AWB_P2 188
#define CALIBRATE_AWB_Q1 (-38)
#define CALIBRATE_AWB_A1 153679
#define CALIBRATE_AWB_B1 128
#define CALIBRATE_AWB_C1 (-101709)

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0

static HI_S32 cmos_get_awb_default(VI_PIPE vi_pipe, AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstAwbSnsDft);
    OV9284_SENSOR_GET_CTX(vi_pipe, pstSnsState);
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
    }
    pstAwbSnsDft->u16InitRgain = g_au16InitWBGain[vi_pipe][0]; /* 0: Rgain */
    pstAwbSnsDft->u16InitGgain = g_au16InitWBGain[vi_pipe][1]; /* 1: Ggain */
    pstAwbSnsDft->u16InitBgain = g_au16InitWBGain[vi_pipe][2]; /* 2: Bgain */
    pstAwbSnsDft->u16SampleRgain = g_au16SampleRgain[vi_pipe];
    pstAwbSnsDft->u16SampleBgain = g_au16SampleBgain[vi_pipe];

    return HI_SUCCESS;
}

static HI_S32 cmos_get_awb_spec_default(VI_PIPE vi_pipe, AWB_SPEC_SENSOR_DEFAULT_S *pstAwbSpecSnsDft)
{
    CMOS_CHECK_POINTER(pstAwbSpecSnsDft);

    memset_s(pstAwbSpecSnsDft, sizeof(AWB_SPEC_SENSOR_DEFAULT_S), 0, sizeof(AWB_SPEC_SENSOR_DEFAULT_S));

    memcpy_s(&pstAwbSpecSnsDft->stSpecAwbAttrs, sizeof(ISP_SPECAWB_ATTR_S), &g_SpecAWBFactTbl,
             sizeof(ISP_SPECAWB_ATTR_S));
    memcpy_s(&pstAwbSpecSnsDft->stCaaControl, sizeof(ISP_SPECAWB_CAA_CONTROl_S), &g_SpecKCAWBCaaTblControl,
             sizeof(ISP_SPECAWB_CAA_CONTROl_S));

    return HI_SUCCESS;
}

static HI_S32 cmos_init_awb_exp_function(AWB_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    CMOS_CHECK_POINTER(pstExpFuncs);

    memset_s(pstExpFuncs, sizeof(AWB_SENSOR_EXP_FUNC_S), 0, sizeof(AWB_SENSOR_EXP_FUNC_S));

    pstExpFuncs->pfn_cmos_get_awb_default = cmos_get_awb_default;
    pstExpFuncs->pfn_cmos_get_awb_spec_default = cmos_get_awb_spec_default;

    return HI_SUCCESS;
}

static ISP_CMOS_DNG_COLORPARAM_S g_stDngColorParam = {{ 378, 256, 430 }, { 439, 256, 439 }};
static HI_VOID cmos_get_isp_dng_default(ISP_SNS_STATE_S *pstSnsState, ISP_CMOS_DEFAULT_S *pstDef)
{
    memcpy_s(&pstDef->stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S), &g_stDngColorParam,
             sizeof(ISP_CMOS_DNG_COLORPARAM_S));

    switch (pstSnsState->u8ImgMode) {
        default:
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
    pstDef->unKey.bit1Lcac = 1;
    pstDef->pstLcac = &g_stIspLCac;
    memcpy_s(&pstDef->stNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S),
             &g_stIspNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
    return;
}

static HI_S32 cmos_get_isp_default(VI_PIPE vi_pipe, ISP_CMOS_DEFAULT_S *pstDef)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstDef);
    OV9284_SENSOR_GET_CTX(vi_pipe, pstSnsState);
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

    cmos_get_isp_linear_default(pstDef);

    pstDef->stSensorMode.u32SensorID = OV9284_ID;
    pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

    cmos_get_isp_dng_default(pstSnsState, pstDef);

    return HI_SUCCESS;
}
static HI_S32 cmos_get_isp_black_level(VI_PIPE vi_pipe, ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
    CMOS_CHECK_POINTER(pstBlackLevel);

    /* It need to update black level when iso change */
    pstBlackLevel->bUpdate = HI_TRUE;

    if (g_au32Again[vi_pipe] < 4096) { /* adgain thresh 4096 */
        pstBlackLevel->au16BlackLevel[0] = 258; /* 0: R channel  258 */
        pstBlackLevel->au16BlackLevel[1] = 258; /* 1: Gr channel 258 */
        pstBlackLevel->au16BlackLevel[2] = 258; /* 2: Gb channel 258 */
        pstBlackLevel->au16BlackLevel[3] = 258; /* 3: B channel  258 */
    } else if (g_au32Again[vi_pipe] < 8192) { /* adgain thresh 8192 */
        pstBlackLevel->au16BlackLevel[0] = 258; /* 0: R channel  258 */
        pstBlackLevel->au16BlackLevel[1] = 258; /* 1: Gr channel 258 */
        pstBlackLevel->au16BlackLevel[2] = 258; /* 2: Gb channel 258 */
        pstBlackLevel->au16BlackLevel[3] = 258; /* 3: B channel  258 */
    } else if (g_au32Again[vi_pipe] < 15872) { /* adgain thresh 15872 */
        pstBlackLevel->au16BlackLevel[0] = 258; /* 0: R channel  258 */
        pstBlackLevel->au16BlackLevel[1] = 258; /* 1: Gr channel 258 */
        pstBlackLevel->au16BlackLevel[2] = 258; /* 2: Gb channel 258 */
        pstBlackLevel->au16BlackLevel[3] = 258; /* 3: B channel  258 */
    } else {
        pstBlackLevel->au16BlackLevel[0] = 258; /* 0: R channel  258 */
        pstBlackLevel->au16BlackLevel[1] = 258; /* 1: Gr channel 258 */
        pstBlackLevel->au16BlackLevel[2] = 258; /* 2: Gb channel 258 */
        pstBlackLevel->au16BlackLevel[3] = 258; /* 3: B channel  258 */
    }

    return HI_SUCCESS;
}

static HI_VOID cmos_set_pixel_detect(VI_PIPE vi_pipe, HI_BOOL bEnable)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    HI_U32 u32Vmax_5fps = 0;
    HI_U32 u32FullLines_5fps = 0;

    OV9284_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (bEnable) { /* setup for ISP pixel calibration mode */
        u32Vmax_5fps = g_astOv9284ModeTbl[pstSnsState->u8ImgMode].u32VMax *
                        g_astOv9284ModeTbl[pstSnsState->u8ImgMode].f32MaxFps / 5; /* 5 fps */
        u32Vmax_5fps = MIN(u32Vmax_5fps, OV9284_FULL_LINES_MAX);

        u32FullLines_5fps = u32Vmax_5fps - g_astOv9284ModeTbl[pstSnsState->u8ImgMode].u32ExpLineLimit;

        ov9284_write_register(vi_pipe, OV9284_VMAX_L, LOW_8_BITS(u32Vmax_5fps));
        ov9284_write_register(vi_pipe, OV9284_VMAX_H, HIGH_8_BITS(u32Vmax_5fps));

        ov9284_write_register(vi_pipe, OV9284_ANA_GAIN_GLOBAL, 0xF);

        ov9284_write_register(vi_pipe, OV9284_COARSE_INTEG_TIME_L, (u32FullLines_5fps << 4) & 0xF0); /* shift 4 */
        ov9284_write_register(vi_pipe, OV9284_COARSE_INTEG_TIME_M, (u32FullLines_5fps >> 4) & 0xFF); /* shift 4 */
        ov9284_write_register(vi_pipe, OV9284_COARSE_INTEG_TIME_H, (u32FullLines_5fps >> 12) & 0xF); /* shift 12 */
    } else { /* setup for ISP 'normal mode' */
        ov9284_write_register(vi_pipe, OV9284_VMAX_L,
                              LOW_8_BITS(g_astOv9284ModeTbl[pstSnsState->u8ImgMode].u32VMax & 0xFF));
        ov9284_write_register(vi_pipe, OV9284_VMAX_H,
                              HIGH_8_BITS(g_astOv9284ModeTbl[pstSnsState->u8ImgMode].u32VMax & 0xFF));
        pstSnsState->bSyncInit  = HI_FALSE;
    }

    return;
}

static HI_S32 cmos_set_wdr_mode(VI_PIPE vi_pipe, HI_U8 u8Mode)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    OV9284_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    pstSnsState->bSyncInit = HI_FALSE;

    switch (u8Mode & 0x3F) {
        case WDR_MODE_NONE:
            pstSnsState->enWDRMode = WDR_MODE_NONE;
            printf("linear mode\n");
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
    pstSnsState->astRegsInfo[0].unComBus.s8I2cDev = g_aunOv9284BusInfo[vi_pipe].s8I2cDev;
    pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2; /* DelayMax 2 */
    pstSnsState->astRegsInfo[0].u32RegNum = 10; /* RegNum 10 */

    for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
        pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
        pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr = OV9284_I2C_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[i].u32AddrByteNum = OV9284_ADDR_BYTE;
        pstSnsState->astRegsInfo[0].astI2cData[i].u32DataByteNum = OV9284_DATA_BYTE;
    }

    /* group hold start */
    pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
    pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr = OV9284_GROUP_HOLD_START;

    /* shutter related */
    pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0; /* index 1 */
    pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr = OV9284_COARSE_INTEG_TIME_H; /* index 1 */
    pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0; /* index 2 */
    pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr = OV9284_COARSE_INTEG_TIME_M; /* index 2 */
    pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 0; /* index 3 */
    pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr = OV9284_COARSE_INTEG_TIME_L; /* index 3 */

    pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 0; /* index 4 */
    pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr = OV9284_DEBUG_MODE; /* index 4 */

    /* gain related */
    pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0; /* index 5 */
    pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr = OV9284_ANA_GAIN_GLOBAL; /* index 5 */

    pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0; /* index 6 */
    pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr = OV9284_GROUP_HOLD_END1; /* index 6 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 0; /* index 7 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr = OV9284_GROUP_HOLD_END2; /* index 7 */

    /* Vmax */
    pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 0; /* index 8 */
    pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr = OV9284_VMAX_L; /* index 8 */
    pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum = 0; /* index 9 */
    pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr = OV9284_VMAX_H; /* index 9 */

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

    for (i = 0; i < 8; i++) { /* 8 loop */
        if (pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate == HI_TRUE) {
            pstSnsState->astRegsInfo[0].astI2cData[0].bUpdate = HI_TRUE;
            pstSnsState->astRegsInfo[0].astI2cData[1].bUpdate = HI_TRUE;
            pstSnsState->astRegsInfo[0].astI2cData[2].bUpdate = HI_TRUE; /* index 2 */
            pstSnsState->astRegsInfo[0].astI2cData[3].bUpdate = HI_TRUE; /* index 3 */
            pstSnsState->astRegsInfo[0].astI2cData[4].bUpdate = HI_TRUE; /* index 4 */
            pstSnsState->astRegsInfo[0].astI2cData[5].bUpdate = HI_TRUE; /* index 5 */
            pstSnsState->astRegsInfo[0].astI2cData[6].bUpdate = HI_TRUE; /* index 6 */
            pstSnsState->astRegsInfo[0].astI2cData[7].bUpdate = HI_TRUE; /* index 7 */
            break;
        }
    }

    return;
}

static HI_S32 cmos_get_sns_regs_info(VI_PIPE vi_pipe, ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstSnsRegsInfo);
    OV9284_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    if ((pstSnsState->bSyncInit == HI_FALSE) || (pstSnsRegsInfo->bConfig == HI_FALSE)) {
        cmos_comm_sns_reg_info_init(vi_pipe, pstSnsState);
    } else {
        cmos_sns_reg_info_update(vi_pipe, pstSnsState);
    }
    memcpy_s(pstSnsRegsInfo, sizeof(ISP_SNS_REGS_INFO_S), &pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S));
    memcpy_s(&pstSnsState->astRegsInfo[1], sizeof(ISP_SNS_REGS_INFO_S),
        &pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S));

    pstSnsState->au32FL[1] = pstSnsState->au32FL[0];
    return HI_SUCCESS;
}

static HI_S32 cmos_comm_set_image_mode(HI_U32 u32W, HI_U32 u32H, HI_U8 *u8SensorImageMode,
    HI_U8 u8SnsMode, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode)
{
    if (OV9284_RES_IS_1M(u32W, u32H)) {
        if (u8SnsMode == 0) {
            *u8SensorImageMode = OV9284_1M_60FPS_10BIT_LINEAR_MODE;
            return HI_SUCCESS;
        } else {
            OV9284_ERR_MODE_PRINT(pstSensorImageMode);
            return HI_FAILURE;
        }
    }
    return HI_SUCCESS;
}

static HI_S32 cmos_set_image_mode(VI_PIPE vi_pipe, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode)
{
    HI_U8 u8SensorImageMode;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    HI_U8 u8SnsMode;
    HI_U32 u32W, u32H;
    HI_S32 ret;
    CMOS_CHECK_POINTER(pstSensorImageMode);
    OV9284_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    u8SensorImageMode = pstSnsState->u8ImgMode;
    pstSnsState->bSyncInit = HI_FALSE;
    u32H = pstSensorImageMode->u16Height;
    u32W = pstSensorImageMode->u16Width;
    u8SnsMode = pstSensorImageMode->u8SnsMode;

    ret = cmos_comm_set_image_mode(u32W, u32H, &u8SensorImageMode, u8SnsMode, pstSensorImageMode);
    if (ret == HI_FAILURE) {
        return HI_FAILURE;
    }

    if ((pstSnsState->bInit == HI_TRUE) && (u8SensorImageMode == pstSnsState->u8ImgMode)) {
        return ISP_DO_NOT_NEED_SWITCH_IMAGEMODE;
    }

    pstSnsState->u8ImgMode = u8SensorImageMode;
    pstSnsState->u32FLStd  = g_astOv9284ModeTbl[pstSnsState->u8ImgMode].u32VMax;
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstSnsState->au32FL[1] = pstSnsState->au32FL[0];

    return HI_SUCCESS;
}

static HI_VOID sensor_global_init(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    OV9284_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->bInit = HI_FALSE;
    pstSnsState->bSyncInit = HI_FALSE;
    pstSnsState->u8ImgMode = OV9284_1M_60FPS_10BIT_LINEAR_MODE;
    pstSnsState->enWDRMode = WDR_MODE_NONE;
    pstSnsState->u32FLStd = g_astOv9284ModeTbl[pstSnsState->u8ImgMode].u32VMax;
    pstSnsState->au32FL[0] = g_astOv9284ModeTbl[pstSnsState->u8ImgMode].u32VMax;
    pstSnsState->au32FL[1] = g_astOv9284ModeTbl[pstSnsState->u8ImgMode].u32VMax;

    memset_s(&pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset_s(&pstSnsState->astRegsInfo[1], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
    return;
}

static HI_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    CMOS_CHECK_POINTER(pstSensorExpFunc);

    memset_s(pstSensorExpFunc, sizeof(ISP_SENSOR_EXP_FUNC_S), 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init = ov9284_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit = ov9284_exit;
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
static HI_S32 ov9284_set_bus_info(VI_PIPE vi_pipe, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunOv9284BusInfo[vi_pipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

    return HI_SUCCESS;
}

static HI_S32 sensor_ctx_init(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    OV9284_SENSOR_GET_CTX(vi_pipe, pastSnsStateCtx);

    if (pastSnsStateCtx == HI_NULL) {
        pastSnsStateCtx = (ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
        if (pastSnsStateCtx == HI_NULL) {
            SNS_ERR_TRACE("Isp[%d] SnsCtx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset_s(pastSnsStateCtx, sizeof(ISP_SNS_STATE_S), 0, sizeof(ISP_SNS_STATE_S));

    OV9284_SENSOR_SET_CTX(vi_pipe, pastSnsStateCtx);

    return HI_SUCCESS;
}

static HI_VOID sensor_ctx_exit(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    OV9284_SENSOR_GET_CTX(vi_pipe, pastSnsStateCtx);
    SENSOR_FREE(pastSnsStateCtx);
    OV9284_SENSOR_RESET_CTX(vi_pipe);
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

    stSnsAttrInfo.eSensorId = OV9284_ID;
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

    s32Ret = HI_MPI_ISP_SensorUnRegCallBack(vi_pipe, OV9284_ID);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AE_SensorUnRegCallBack(vi_pipe, pstAeLib, OV9284_ID);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AWB_SensorUnRegCallBack(vi_pipe, pstAwbLib, OV9284_ID);
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
    g_au16InitWBGain[vi_pipe][0] = pstInitAttr->u16WBRgain;
    g_au16InitWBGain[vi_pipe][1] = pstInitAttr->u16WBGgain;
    g_au16InitWBGain[vi_pipe][2] = pstInitAttr->u16WBBgain; /* index 2 */
    g_au16SampleRgain[vi_pipe] = pstInitAttr->u16SampleRgain;
    g_au16SampleBgain[vi_pipe] = pstInitAttr->u16SampleBgain;
    g_quick_start_en[vi_pipe] = pstInitAttr->stQuickStart.bQuickStartEn;

    g_abAERouteExValid[vi_pipe] = pstInitAttr->bAERouteExValid;
    memcpy_s(&g_astInitAERoute[vi_pipe], sizeof(ISP_AE_ROUTE_S), &pstInitAttr->stAERoute, sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&g_astInitAERouteEx[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S),
        &pstInitAttr->stAERouteEx, sizeof(ISP_AE_ROUTE_EX_S));
    return HI_SUCCESS;
}

ISP_SNS_OBJ_S stSnsOv9284Obj = {
    .pfnRegisterCallback    = sensor_register_callback,
    .pfnUnRegisterCallback  = sensor_unregister_callback,
    .pfnStandby             = ov9284_standby,
    .pfnRestart             = ov9284_restart,
    .pfnMirrorFlip          = ov9284_mirror_flip,
    .pfnWriteReg            = ov9284_write_register,
    .pfnReadReg             = ov9284_read_register,
    .pfnSetBusInfo          = ov9284_set_bus_info,
    .pfnSetInit             = sensor_set_init
};

