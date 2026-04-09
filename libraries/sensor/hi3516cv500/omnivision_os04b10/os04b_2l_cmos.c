/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of os04b_2l_cmos.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "os04b_2l_cmos.h"
#include "os04b_2l_cmos_ex.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "mpi_isp.h"
#include "mpi_ae.h"
#include "mpi_awb.h"

#define OS04B_2L_ID 04
#define HIGH_8BITS(x) (((x)&0xff00) >> 8)
#define LOW_8BITS(x)  ((x)&0x00ff)

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ? (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ? (b) : (a))
#endif

ISP_SNS_STATE_S *g_pastOs04b_2l[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define OS04B_2L_SENSOR_SET_CTX(dev, pstCtx)   ((g_pastOs04b_2l[dev]) = (pstCtx))
#define OS04B_2L_SENSOR_RESET_CTX(dev)         (g_pastOs04b_2l[dev] = HI_NULL)

ISP_SNS_COMMBUS_U g_aunOs04b_2lBusInfo[ISP_MAX_PIPE_NUM] = {
    [0] = {
        .s8I2cDev = 0
    },
    [1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1 }
};

ISP_SNS_COMMBUS_U *os04b_2l_get_bus_Info(VI_PIPE vi_pipe)
{
    return &g_aunOs04b_2lBusInfo[vi_pipe];
}

ISP_SNS_STATE_S *os04b_2l_get_ctx(VI_PIPE vi_pipe)
{
    return g_pastOs04b_2l[vi_pipe];
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

static HI_U16 g_init_ccm[ISP_MAX_PIPE_NUM][CCM_MATRIX_SIZE] = {{0}};
static HI_U32 g_u32LGain = 0;

static HI_BOOL g_abAERouteExValid[ISP_MAX_PIPE_NUM] = {0};
static ISP_AE_ROUTE_S g_astInitAERoute[ISP_MAX_PIPE_NUM] = {{0}};
static ISP_AE_ROUTE_EX_S g_astInitAERouteEx[ISP_MAX_PIPE_NUM] = {{0}};

/* define */
#define OS04B_2L_FULL_LINES_MAX 0xFFF2

/* Register define */
#define PAGE_SELECT_ADDR 0xfd
#define EXP_ADDR_H 0x03
#define EXP_ADDR_L 0x04
#define AGAIN_ADDR 0x24
#define DGAIN_ADDR 0x39
#define VBLANK_H 0x05
#define VBLANK_L 0x06
#define TRIGGER_ADDR 0x01

#define INCREASE_LINES 0 /* make real fps less than stand fps because NVR require */
#define OS04B_2L_VMAX_4M30_LINEAR (1489 + INCREASE_LINES)

#define Os04b_2l_4M_2560x1440_10bit_linear30 0 /* for FPGA */

static HI_VOID cmos_get_ae_comm_default(VI_PIPE vi_pipe, HI_U32 u32Fll, HI_FLOAT f32maxFps,
    AE_SENSOR_DEFAULT_S *pstAeSnsDft, ISP_SNS_STATE_S *pstSnsState)
{
    if (pstSnsState->u8ImgMode == Os04b_2l_4M_2560x1440_10bit_linear30) {
        u32Fll = OS04B_2L_VMAX_4M30_LINEAR;
        f32maxFps = 30; /* 30fps */
    }
    pstSnsState->u32FLStd = u32Fll;
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FlickerFreq = 0;

    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
    pstAeSnsDft->u32MaxIntTime = (pstSnsState->u32FLStd >= 21) ? (pstSnsState->u32FLStd - 21) : (0); /* sub 21 */
    pstAeSnsDft->stIntTimeAccu.f32Offset = 0;
    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 0.0625; /* Accuracy 0.0625 */
    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 0.125; /* Accuracy 0.125 */
    pstAeSnsDft->u32MaxAgain = 15872; /* max 15872 */
    pstAeSnsDft->u32MinAgain = 1024; /* min 1024 */
    pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
    pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;
    pstAeSnsDft->u32MaxDgain = 255; /* max 255 */
    pstAeSnsDft->u32MinDgain = 8; /* min 8 */
    pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
    pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;
    pstAeSnsDft->u32ISPDgainShift = 8; /* Accuracy 8 */
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 6 << pstAeSnsDft->u32ISPDgainShift; /* max 6 */

    if (g_au32InitExposure[vi_pipe] == 0) {
        pstAeSnsDft->u32InitExposure = 130000; /* init 130000 */
    } else {
        pstAeSnsDft->u32InitExposure = g_au32InitExposure[vi_pipe];
    }
    if (g_au32LinesPer500ms[vi_pipe] == 0) {
        pstAeSnsDft->u32LinesPer500ms = ((HI_U64)(u32Fll * f32maxFps)) >> 1;
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
static HI_VOID cmos_get_ae_linear_default(VI_PIPE vi_pipe, ISP_SNS_STATE_S *pstSnsState,
    AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    pstAeSnsDft->au8HistThresh[0] = 0xd;
    pstAeSnsDft->au8HistThresh[1] = 0x28;
    pstAeSnsDft->au8HistThresh[2] = 0x60; /* index 2 */
    pstAeSnsDft->au8HistThresh[3] = 0x80; /* index 3 */
    pstAeSnsDft->u8AeCompensation = 0x38;
    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 21; /* sub 21 */
    pstAeSnsDft->u32MinIntTime = 2; /* min 2 */
    pstAeSnsDft->u32MaxIntTimeTarget = 65535; /* max 65535 */
    pstAeSnsDft->u32MinIntTimeTarget = 2; /* min 2 */

    pstAeSnsDft->bAERouteExValid = g_abAERouteExValid[vi_pipe];
    memcpy_s(&pstAeSnsDft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), &g_astInitAERoute[vi_pipe],
        sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&pstAeSnsDft->stAERouteAttrEx, sizeof(ISP_AE_ROUTE_EX_S), &g_astInitAERouteEx[vi_pipe],
        sizeof(ISP_AE_ROUTE_EX_S));
    return;
}

static HI_S32 cmos_get_ae_default(VI_PIPE vi_pipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    HI_U32 u32Fll = 0;

    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    HI_FLOAT f32maxFps = 0;

    CMOS_CHECK_POINTER(pstAeSnsDft);
    OS04B_2L_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset_s(&pstAeSnsDft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), 0, sizeof(ISP_AE_ROUTE_S));

    cmos_get_ae_comm_default(vi_pipe, u32Fll, f32maxFps, pstAeSnsDft, pstSnsState);

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE: /* linear mode */
            cmos_get_ae_linear_default(vi_pipe, pstSnsState, pstAeSnsDft);
            break;
    }
    return HI_SUCCESS;
}

/* the function of sensor set fps */
static HI_VOID cmos_fps_set(VI_PIPE vi_pipe, HI_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    HI_FLOAT f32maxFps;
    HI_U32 u32Lines;
    HI_U32 u32VBlank;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    OS04B_2L_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    switch (pstSnsState->u8ImgMode) {
        case Os04b_2l_4M_2560x1440_10bit_linear30:
            if ((f32Fps <= 30) && (f32Fps >= 0.68)) { /* linear mode fps range:[0.68, 30] */
                f32maxFps = 30.53;  /* fix to 30.53fps and dont support fps change */
                u32Lines = (OS04B_2L_VMAX_4M30_LINEAR) * f32maxFps / f32Fps;
                u32VBlank = u32Lines - OS04B_2L_VMAX_4M30_LINEAR;
            } else {
                printf("Not support Fps: %f\n", f32Fps);
                return;
            }
            pstAeSnsDft->u32LinesPer500ms = OS04B_2L_VMAX_4M30_LINEAR * 15; /* 15: 30fps div 2 */
            break;

        default:
            printf("Not support this Mode\n");
            return;
            break;
    }

    pstSnsState->u32FLStd = u32Lines;
    pstSnsState->u32FLStd = (pstSnsState->u32FLStd > OS04B_2L_FULL_LINES_MAX) ?
        OS04B_2L_FULL_LINES_MAX : pstSnsState->u32FLStd;
    pstAeSnsDft->f32Fps = f32Fps;
    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 21; /* sub 21 */
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];

    if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
        pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = LOW_8BITS(u32VBlank); /* index 6 */
        pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = HIGH_8BITS(u32VBlank); /* index 5 */
    }
    return;
}

static HI_VOID cmos_slow_framerate_set(VI_PIPE vi_pipe, HI_U32 u32FullLines, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    HI_U32 u32VBlank;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    OS04B_2L_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->au32FL[0] = (u32FullLines > OS04B_2L_FULL_LINES_MAX) ? OS04B_2L_FULL_LINES_MAX : u32FullLines;
    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
    pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 21; /* sub 21 */
    u32VBlank = pstSnsState->au32FL[0] - OS04B_2L_VMAX_4M30_LINEAR;

    if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
        pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = LOW_8BITS(u32VBlank); /* index 6 */
        pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = HIGH_8BITS(u32VBlank); /* index 5 */
    }

    return;
}

#define AGAIN_NODE_NUM   64
static HI_U32 g_au32Again_table[AGAIN_NODE_NUM] = {
    1024, 1088, 1152, 1216, 1280,  1344,  1408,  1472,  1536,  1600,  1664,  1728,  1792,  1856,  1920,  1984,
    2048, 2176, 2304, 2432, 2560,  2688,  2816,  2944,  3072,  3200,  3328,  3456,  3584,  3712,  3840,  3968,
    4096, 4352, 4608, 4864, 5120,  5376,  5632,  5888,  6144,  6400,  6656,  6912,  7168,  7424,  7680,  7936,
    8192, 8704, 9216, 9728, 10240, 10752, 11264, 11776, 12288, 12800, 13312, 13824, 14336, 14848, 15360, 15872
};

/* while isp notify ae to update sensor regs, ae call these funcs. */
static HI_VOID cmos_inttime_update(VI_PIPE ViPipe, HI_U32 u32IntTime)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    OS04B_2L_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);
    if (u32IntTime <= 2) { /* min 2 */
        u32IntTime = 2; /* min 2 */
    }

    pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = HIGH_8BITS(u32IntTime);
    pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = LOW_8BITS(u32IntTime); /* index 2 */

    return;
}

static HI_VOID cmos_again_calc_table(VI_PIPE ViPipe, HI_U32 *pu32AgainLin, HI_U32 *pu32AgainDb)
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

static HI_VOID cmos_gains_update(VI_PIPE vi_pipe, HI_U32 u32Again, HI_U32 u32Dgain)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    OS04B_2L_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);
    HI_U32 temp_gain;

    g_u32LGain = u32Again;
    temp_gain = (HI_U32)(g_au32Again_table[u32Again] / 1024.0 * 16.0); /* div 1024.0 x16.0 */

    pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = temp_gain; /* index 3 */
    pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = u32Dgain; /* index 4 */

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

    pstExpFuncs->pfn_cmos_get_ae_default = cmos_get_ae_default;
    pstExpFuncs->pfn_cmos_fps_set = cmos_fps_set;
    pstExpFuncs->pfn_cmos_slow_framerate_set = cmos_slow_framerate_set;
    pstExpFuncs->pfn_cmos_inttime_update = cmos_inttime_update;
    pstExpFuncs->pfn_cmos_gains_update = cmos_gains_update;
    pstExpFuncs->pfn_cmos_again_calc_table = cmos_again_calc_table;
    pstExpFuncs->pfn_cmos_dgain_calc_table = HI_NULL;
    pstExpFuncs->pfn_cmos_get_inttime_max = HI_NULL;
    pstExpFuncs->pfn_cmos_ae_fswdr_attr_set = cmos_ae_fswdr_attr_set;
    return HI_SUCCESS;
}

/* AWB default parameter and function */
#define CALIBRATE_STATIC_TEMP             4850
#define CALIBRATE_STATIC_WB_R_GAIN        0x1A0
#define CALIBRATE_STATIC_WB_GR_GAIN       0x100
#define CALIBRATE_STATIC_WB_GB_GAIN       0x100
#define CALIBRATE_STATIC_WB_B_GAIN        0x1E6

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1                  33
#define CALIBRATE_AWB_P2                  223
#define CALIBRATE_AWB_Q1                  0
#define CALIBRATE_AWB_A1                  130289
#define CALIBRATE_AWB_B1                  128
#define CALIBRATE_AWB_C1                  (-77176)

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0
static HI_S32 cmos_get_awb_default(VI_PIPE vi_pipe, AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    HI_S32 i;

    CMOS_CHECK_POINTER(pstAwbSnsDft);
    OS04B_2L_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset_s(pstAwbSnsDft, sizeof(AWB_SENSOR_DEFAULT_S), 0, sizeof(AWB_SENSOR_DEFAULT_S));

    pstAwbSnsDft->u16WbRefTemp = CALIBRATE_STATIC_TEMP;  /* 4850:default reference color temperature */
    pstAwbSnsDft->au16GainOffset[0] = CALIBRATE_STATIC_WB_R_GAIN;  /* 0: R channel */
    pstAwbSnsDft->au16GainOffset[1] = CALIBRATE_STATIC_WB_GR_GAIN;  /* 1: Gr channel */
    pstAwbSnsDft->au16GainOffset[2] = CALIBRATE_STATIC_WB_GB_GAIN;  /* 2: Gb channel */
    pstAwbSnsDft->au16GainOffset[3] = CALIBRATE_STATIC_WB_B_GAIN;  /* 3: B channel */
    pstAwbSnsDft->as32WbPara[0] = CALIBRATE_AWB_P1;        /* index0: p1 */
    pstAwbSnsDft->as32WbPara[1] = CALIBRATE_AWB_P2;       /* index1: p2 */
    pstAwbSnsDft->as32WbPara[2] = CALIBRATE_AWB_Q1;         /* index2: q1 */
    pstAwbSnsDft->as32WbPara[3] = CALIBRATE_AWB_A1;    /* index3: a1 */
    pstAwbSnsDft->as32WbPara[4] = CALIBRATE_AWB_B1;       /* index4: b1 */
    pstAwbSnsDft->as32WbPara[5] = CALIBRATE_AWB_C1;    /* index5: c1 */

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

static HI_VOID cmos_get_isp_linear_default(ISP_CMOS_DEFAULT_S *pstDef)
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

static HI_S32 cmos_get_isp_default(VI_PIPE vi_pipe, ISP_CMOS_DEFAULT_S *pstDef)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstDef);
    OS04B_2L_SENSOR_GET_CTX(vi_pipe, pstSnsState);
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
    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:
            cmos_get_isp_linear_default(pstDef);
            break;
    }
    pstDef->stSensorMode.u32SensorID = OS04B_2L_ID;
    pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

    cmos_get_isp_dng_default(pstSnsState, pstDef);
    return HI_SUCCESS;
}
static HI_S32 cmos_get_isp_black_level(VI_PIPE vi_pipe, ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
    HI_S32 i;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstBlackLevel);
    OS04B_2L_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    /* Don't need to update black level when iso change */
    pstBlackLevel->bUpdate = HI_FALSE;
    if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            pstBlackLevel->au16BlackLevel[i] = 256; /* 12bit, 256 */
        }
    }
    return HI_SUCCESS;
}
static HI_VOID cmos_set_pixel_detect(VI_PIPE vi_pipe, HI_BOOL bEnable)
{
    HI_U32 u32VBlank;
    HI_U32 u32FullLines_5Fps, u32MaxIntTime_5Fps;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    OS04B_2L_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_2To1_LINE) {
        return;
    } else {
        if (pstSnsState->u8ImgMode == Os04b_2l_4M_2560x1440_10bit_linear30) {
            u32FullLines_5Fps = (OS04B_2L_VMAX_4M30_LINEAR * 30.53) / 5; /* 30.53fps, 5fps */
            u32VBlank = u32FullLines_5Fps - OS04B_2L_VMAX_4M30_LINEAR;
        } else {
            return;
        }
    }

    u32MaxIntTime_5Fps = u32FullLines_5Fps - 21; /* sub 21 */

    if (bEnable) {                                    /* setup for ISP pixel calibration mode */
        os04b_2l_write_register(vi_pipe, 0xfd, 0x01);  /* the first page 5fps */
        os04b_2l_write_register(vi_pipe, VBLANK_H, HIGH_8BITS(u32VBlank));
        os04b_2l_write_register(vi_pipe, VBLANK_L, LOW_8BITS(u32VBlank));
        os04b_2l_write_register(vi_pipe, AGAIN_ADDR, 0x10); /* again */
        os04b_2l_write_register(vi_pipe, DGAIN_ADDR, 0x08); /* dgain */
        os04b_2l_write_register(vi_pipe, EXP_ADDR_H, HIGH_8BITS(u32MaxIntTime_5Fps));
        os04b_2l_write_register(vi_pipe, EXP_ADDR_L, LOW_8BITS(u32MaxIntTime_5Fps));
        os04b_2l_write_register(vi_pipe, 0x01, 0x01);  /* trigger */
    } else {                                          /* setup for ISP 'normal mode' */
        os04b_2l_write_register(vi_pipe, 0xfd, 0x01);  /* the first page */
        os04b_2l_write_register(vi_pipe, 0x03, 0x04);  /* exportion */
        os04b_2l_write_register(vi_pipe, 0x04, 0x48);
        os04b_2l_write_register(vi_pipe, 0x01, 0x01);
        pstSnsState->bSyncInit = HI_FALSE;
    }
    return;
}

static HI_S32 cmos_set_wdr_mode(VI_PIPE vi_pipe, HI_U8 u8Mode)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    OS04B_2L_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    pstSnsState->bSyncInit = HI_FALSE;

    switch (u8Mode) {
        case WDR_MODE_NONE:
            pstSnsState->enWDRMode = WDR_MODE_NONE;
            pstSnsState->u8ImgMode = Os04b_2l_4M_2560x1440_10bit_linear30;
            pstSnsState->u32FLStd = OS04B_2L_VMAX_4M30_LINEAR;
            printf("linear mode\n");
            break;

        default:
            SNS_ERR_TRACE("NOT support this mode!\n");
            return HI_FAILURE;
            break;
    }
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstSnsState->au32FL[1] = pstSnsState->au32FL[0];
    memset_s(pstSnsState->au32WDRIntTime, sizeof(pstSnsState->au32WDRIntTime), 0, sizeof(pstSnsState->au32WDRIntTime));

    return HI_SUCCESS;
}

static HI_VOID cmos_comm_sns_reg_info_init(VI_PIPE vi_pipe, ISP_SNS_STATE_S *pstSnsState)
{
    hi_s32 i;
    pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
    pstSnsState->astRegsInfo[0].unComBus.s8I2cDev = g_aunOs04b_2lBusInfo[vi_pipe].s8I2cDev;
    pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2; /* DelayMax 2 */
    pstSnsState->astRegsInfo[0].u32RegNum = 8; /* RegNum 8 */

    if ((WDR_MODE_2To1_LINE == pstSnsState->enWDRMode) || (WDR_MODE_2To1_FRAME == pstSnsState->enWDRMode) ||
        (WDR_MODE_2To1_FRAME_FULL_RATE == pstSnsState->enWDRMode)) {
        pstSnsState->astRegsInfo[0].u32RegNum += 8; /* RegNum add 8 */
    }

    for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
        pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
        pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr = OS04B_2L_I2C_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[i].u32AddrByteNum = OS04B_2L_ADDR_BYTE;
        pstSnsState->astRegsInfo[0].astI2cData[i].u32DataByteNum = OS04B_2L_DATA_BYTE;
    }
    pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;  /* page */
    pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr = PAGE_SELECT_ADDR;
    pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;  /* exporsion */
    pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr = EXP_ADDR_H;
    pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0; /* index 2 */
    pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr = EXP_ADDR_L; /* index 2 */
    pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 0; /* index 3 */
    pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr = AGAIN_ADDR; /* index 3 */
    pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 0; /* index 4 */
    pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr = DGAIN_ADDR; /* index 4 */
    pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0; /* index 5 */
    pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr = VBLANK_H; /* index 5 */
    pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0; /* index 6 */
    pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr = VBLANK_L; /* index 6 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 0; /* index 7 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr = TRIGGER_ADDR; /* index 7 */

    pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = 0x01;  /* init the page ,VTS enable, the trigger data */
    pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = 0x01; /* index 7 */
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
    pstSnsState->astRegsInfo[0].astI2cData[0].bUpdate = HI_TRUE;  /* always refresh the data */
    pstSnsState->astRegsInfo[0].astI2cData[7].bUpdate = HI_TRUE; /* index 7 */

    return;
}

static HI_S32 cmos_get_sns_regs_info(VI_PIPE vi_pipe, ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstSnsRegsInfo);
    OS04B_2L_SENSOR_GET_CTX(vi_pipe, pstSnsState);
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
    HI_U8 u8SensorImageMode;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstSensorImageMode);
    OS04B_2L_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    u8SensorImageMode = pstSnsState->u8ImgMode;
    pstSnsState->bSyncInit = HI_FALSE;

    if (pstSensorImageMode == HI_NULL) {
        SNS_ERR_TRACE("null pointer when set image mode\n");
        return HI_FAILURE;
    }

    if ((pstSensorImageMode->u16Width <= 2560) && (pstSensorImageMode->u16Height <= 1440)) { /* w2560, h 1440 */
        if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
            if (pstSensorImageMode->f32Fps <= 30) { /* 30fps */
                u8SensorImageMode = Os04b_2l_4M_2560x1440_10bit_linear30;
            } else {
                SNS_ERR_TRACE("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
                              pstSensorImageMode->u16Width,
                              pstSensorImageMode->u16Height,
                              pstSensorImageMode->f32Fps,
                              pstSnsState->enWDRMode);
                return HI_FAILURE;
            }
        }
    } else {
        SNS_ERR_TRACE("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
                      pstSensorImageMode->u16Width,
                      pstSensorImageMode->u16Height,
                      pstSensorImageMode->f32Fps,
                      pstSnsState->enWDRMode);

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
        return HI_FAILURE;
    }
    pstSnsState->u8ImgMode = u8SensorImageMode;
    memset_s(pstSnsState->au32WDRIntTime, sizeof(pstSnsState->au32WDRIntTime), 0, sizeof(pstSnsState->au32WDRIntTime));

    return HI_SUCCESS;
}

static HI_VOID sensor_global_init(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    OS04B_2L_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->bInit = HI_FALSE;
    pstSnsState->bSyncInit = HI_FALSE;
    pstSnsState->u8ImgMode = Os04b_2l_4M_2560x1440_10bit_linear30;
    pstSnsState->enWDRMode = WDR_MODE_NONE;
    pstSnsState->u32FLStd = OS04B_2L_VMAX_4M30_LINEAR;
    pstSnsState->au32FL[0] = OS04B_2L_VMAX_4M30_LINEAR;
    pstSnsState->au32FL[1] = OS04B_2L_VMAX_4M30_LINEAR;

    memset_s(&pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset_s(&pstSnsState->astRegsInfo[1], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
    return;
}

static HI_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    CMOS_CHECK_POINTER(pstSensorExpFunc);

    memset_s(pstSensorExpFunc, sizeof(ISP_SENSOR_EXP_FUNC_S), 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init = os04b_2l_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit = os04b_2l_exit;
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
static HI_S32 os04b_2l_set_bus_info(VI_PIPE vi_pipe, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunOs04b_2lBusInfo[vi_pipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

    return HI_SUCCESS;
}

static HI_S32 sensor_ctx_init(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    OS04B_2L_SENSOR_GET_CTX(vi_pipe, pastSnsStateCtx);

    if (pastSnsStateCtx == HI_NULL) {
        pastSnsStateCtx = (ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
        if (pastSnsStateCtx == HI_NULL) {
            SNS_ERR_TRACE("Isp[%d] SnsCtx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset_s(pastSnsStateCtx, sizeof(ISP_SNS_STATE_S), 0, sizeof(ISP_SNS_STATE_S));

    OS04B_2L_SENSOR_SET_CTX(vi_pipe, pastSnsStateCtx);

    return HI_SUCCESS;
}

static HI_VOID sensor_ctx_exit(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    OS04B_2L_SENSOR_GET_CTX(vi_pipe, pastSnsStateCtx);
    SENSOR_FREE(pastSnsStateCtx);
    OS04B_2L_SENSOR_RESET_CTX(vi_pipe);
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

    stSnsAttrInfo.eSensorId = OS04B_2L_ID;

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

    s32Ret = HI_MPI_ISP_SensorUnRegCallBack(vi_pipe, OS04B_2L_ID);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AE_SensorUnRegCallBack(vi_pipe, pstAeLib, OS04B_2L_ID);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AWB_SensorUnRegCallBack(vi_pipe, pstAwbLib, OS04B_2L_ID);
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
    g_au16InitWBGain[vi_pipe][0] = pstInitAttr->u16WBRgain;
    g_au16InitWBGain[vi_pipe][1] = pstInitAttr->u16WBGgain;
    g_au16InitWBGain[vi_pipe][2] = pstInitAttr->u16WBBgain;  /* 2: Bgain */
    g_au16SampleRgain[vi_pipe] = pstInitAttr->u16SampleRgain;
    g_au16SampleBgain[vi_pipe] = pstInitAttr->u16SampleBgain;
    g_quick_start_en[vi_pipe] = pstInitAttr->stQuickStart.bQuickStartEn;

    g_abAERouteExValid[vi_pipe] = pstInitAttr->bAERouteExValid;
    memcpy_s(&g_astInitAERoute[vi_pipe], sizeof(ISP_AE_ROUTE_S), &pstInitAttr->stAERoute, sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&g_astInitAERouteEx[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S), &pstInitAttr->stAERouteEx,
             sizeof(ISP_AE_ROUTE_EX_S));

    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        g_init_ccm[vi_pipe][i] = pstInitAttr->au16CCM[i];
    }

    return HI_SUCCESS;
}

ISP_SNS_OBJ_S stSnsOs04b10_2lObj = {
    .pfnRegisterCallback    = sensor_register_callback,
    .pfnUnRegisterCallback  = sensor_unregister_callback,
    .pfnStandby             = os04b_2l_standby,
    .pfnRestart             = os04b_2l_restart,
    .pfnMirrorFlip          = os04b_2l_mirror_flip,
    .pfnWriteReg            = os04b_2l_write_register,
    .pfnReadReg             = os04b_2l_read_register,
    .pfnSetBusInfo          = os04b_2l_set_bus_info,
    .pfnSetInit             = sensor_set_init
};

