/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of gc2053_cmos.c
 * Author: ISP SW
 * Create: 2012/06/28
 */
#include "gc2053_cmos.h"
#include "gc2053_cmos_ex.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "mpi_isp.h"
#include "mpi_ae.h"
#include "mpi_awb.h"

#define GC2053_ID     2053

/****************************************************************************
 * global variables   *
 ****************************************************************************/
#define HIGH_8BITS(x)   (((x) & 0xFF00) >> 8)
#define LOW_8BITS(x)    ((x) & 0x00FF)
#define LOWER_4BITS(x)  ((x) & 0x000F)
#define HIGHER_4BITS(x) (((x) & 0xF000) >> 12)
#define HIGHER_8BITS(x) (((x) & 0x0FF0) >> 4)

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ?  (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ?  (b) : (a))
#endif

ISP_SNS_STATE_S *g_pastGc2053[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define GC2053_SENSOR_SET_CTX(dev, pstCtx)   ((g_pastGc2053[dev]) = (pstCtx))
#define GC2053_SENSOR_RESET_CTX(dev)         (g_pastGc2053[dev] = HI_NULL)

ISP_SNS_COMMBUS_U g_aunGc2053BusInfo[ISP_MAX_PIPE_NUM] = {
    [0] = { .s8I2cDev = 0},
    [1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1}
};

ISP_SNS_COMMBUS_U *gc2053_get_bus_Info(VI_PIPE vi_pipe)
{
    return &g_aunGc2053BusInfo[vi_pipe];
}

ISP_SNS_STATE_S *gc2053_get_ctx(VI_PIPE vi_pipe)
{
    return g_pastGc2053[vi_pipe];
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
static HI_U16 g_au16InitCCM[ISP_MAX_PIPE_NUM][CCM_MATRIX_SIZE] = {{0}};
static HI_BOOL g_quick_start_en[ISP_MAX_PIPE_NUM] = {HI_FALSE};

static HI_BOOL g_abAERouteExValid[ISP_MAX_PIPE_NUM] = {0};
static ISP_AE_ROUTE_S g_astInitAERoute[ISP_MAX_PIPE_NUM] = {{0}};
static ISP_AE_ROUTE_EX_S g_astInitAERouteEx[ISP_MAX_PIPE_NUM] = {{0}};
static ISP_AE_ROUTE_S g_astInitAERouteSF[ISP_MAX_PIPE_NUM] = {{0}};
static ISP_AE_ROUTE_EX_S g_astInitAERouteSFEx[ISP_MAX_PIPE_NUM] = {{0}};

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
#define GC2053_FULL_LINES_MAX                 0x3FFF

/* SC2235 Register Address */
#define GC2053_EXPTIME_ADDR_H                 0x03 /* Shutter-time[13:8] */
#define GC2053_EXPTIME_ADDR_L                 0x04 /* Shutter-time[7:0] */

#define GC2053_AGAIN_ADDR_H                   0xb4   /* Analog-gain[9:8] */
#define GC2053_AGAIN_ADDR_L                   0xb3   /* Analog-gain[7:0] */

#define GC2053_DGAIN_ADDR_H                   0xb9   /* digital-gain[11:6] */
#define GC2053_DGAIN_ADDR_L                   0xb8   /* digital-gain[5:0] */

#define GC2053_AUTO_PREGAIN_ADDR_H            0xb1   /* auto-pregain-gain[9:6] */
#define GC2053_AUTO_PREGAIN_ADDR_L            0xb2   /* auto-pregain-gain[5:0] */

#define GC2053_VMAX_ADDR_H                    0x41   /* Vmax[13:8] */
#define GC2053_VMAX_ADDR_L                    0x42    /* Vmax[7:0] */
#define GC2053_STATUS_ADDR                    0x90    /* slow shutter via framerate or exptime */

#define GC2053_INCREASE_LINES                 0 /* make real fps less than stand fps because NVR require */
#define GC2053_VMAX_1080P30_LINEAR            (1125 + GC2053_INCREASE_LINES)

/* sensor fps mode */
#define GC2053_SENSOR_1080P_30FPS_LINEAR_MODE  0
static HI_VOID cmos_get_ae_comm_default(VI_PIPE vi_pipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft,
    ISP_SNS_STATE_S *pstSnsState)
{
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FlickerFreq = 50 * 256; /* light flicker freq: 50Hz, accuracy: 256 */
    pstAeSnsDft->u32FullLinesMax = GC2053_FULL_LINES_MAX;
    pstAeSnsDft->u8AERunInterval  = 1;
    pstAeSnsDft->u32InitExposure  = 10; /* init exp 10 */

    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
    pstAeSnsDft->stIntTimeAccu.f32Offset = 0;

    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 1;

    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 0.015625; /* accuracy 0.015625 */

    pstAeSnsDft->u32ISPDgainShift = 8; /* accuracy: 8 */
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 2 << pstAeSnsDft->u32ISPDgainShift; /* max 2 */

    pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_0;
    pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_32_0;

    pstAeSnsDft->bAERouteExValid = HI_FALSE;
    pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
    pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;

    if (g_au32LinesPer500ms[vi_pipe] == 0) {
        pstAeSnsDft->u32LinesPer500ms = pstSnsState->u32FLStd * 30 / 2; /* 30fps, div 2 */
    } else {
        pstAeSnsDft->u32LinesPer500ms = g_au32LinesPer500ms[vi_pipe];
    }
    pstAeSnsDft->stQuickStart.bQuickStartEn = g_quick_start_en[vi_pipe];
    pstAeSnsDft->stQuickStart.u8BlackFrameNum = 0;
    return;
}

static HI_VOID cmos_get_ae_linear_default(VI_PIPE vi_pipe, ISP_SNS_STATE_S *pstSnsState,
    AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    pstAeSnsDft->au8HistThresh[0] = 0x0D;
    pstAeSnsDft->au8HistThresh[1] = 0x28;
    pstAeSnsDft->au8HistThresh[2] = 0x60; /* index 2 */
    pstAeSnsDft->au8HistThresh[3] = 0x80; /* index 3 */

    pstAeSnsDft->u32MaxAgain = 113168; /* max 113168 */
    pstAeSnsDft->u32MinAgain = 1024; /* min 1024 */
    pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
    pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

    pstAeSnsDft->u32MaxDgain = 512; /* max 512 */
    pstAeSnsDft->u32MinDgain = 64; /* min 64 */
    pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
    pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

    pstAeSnsDft->u8AeCompensation = 0x38;
    pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;

    pstAeSnsDft->u32InitExposure = g_au32InitExposure[vi_pipe] ? g_au32InitExposure[vi_pipe] : 148859; /* init 148859 */
    pstAeSnsDft->bAERouteExValid = g_abAERouteExValid[vi_pipe];
    memcpy_s(&pstAeSnsDft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), &g_astInitAERoute[vi_pipe],
             sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&pstAeSnsDft->stAERouteAttrEx, sizeof(ISP_AE_ROUTE_EX_S), &g_astInitAERouteEx[vi_pipe],
             sizeof(ISP_AE_ROUTE_EX_S));

    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 2; /* sub 2 */
    pstAeSnsDft->u32MinIntTime = 3; /* min 3 */
    pstAeSnsDft->u32MaxIntTimeTarget = 65535; /* max 65535 */
    pstAeSnsDft->u32MinIntTimeTarget = 3; /* min 3 */
    return;
}
static HI_S32 cmos_get_ae_default(VI_PIPE vi_pipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstAeSnsDft);
    GC2053_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset_s(&pstAeSnsDft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), 0, sizeof(ISP_AE_ROUTE_S));

    cmos_get_ae_comm_default(vi_pipe, pstAeSnsDft, pstSnsState);

    switch (pstSnsState->enWDRMode) {
        case WDR_MODE_NONE:
            cmos_get_ae_linear_default(vi_pipe, pstSnsState, pstAeSnsDft);
            break;
        default:
            printf("cmos_get_ae_default_Sensor Mode is error!\n");
            break;
    }
    return 0;
}

/* the function of sensor set fps */
static HI_VOID cmos_fps_set(VI_PIPE vi_pipe, HI_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    HI_U32 u32VMAX = GC2053_VMAX_1080P30_LINEAR;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    GC2053_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    switch (pstSnsState->u8ImgMode) {
        case GC2053_SENSOR_1080P_30FPS_LINEAR_MODE:
            if ((f32Fps > 30) || (f32Fps <= 2.06)) { /* linear mode fps range:[2.06, 30) */
                SNS_ERR_TRACE("Not support Fps: %f\n", f32Fps);
                return;
            }
            u32VMAX = GC2053_VMAX_1080P30_LINEAR * 30 / SNS_DIV_0_TO_1_FLOAT(f32Fps); /* 30fps */
            pstAeSnsDft->u32LinesPer500ms = GC2053_VMAX_1080P30_LINEAR * 15; /* 15: 30fps / 2 */
            break;
        default:
            break;
    }
    u32VMAX = (u32VMAX > GC2053_FULL_LINES_MAX) ? GC2053_FULL_LINES_MAX : u32VMAX;
    pstSnsState->u32FLStd = u32VMAX;
    pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = 0x0; /* index 8 */
    pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = (u32VMAX & 0xFF); /* index 9 */
    pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = ((u32VMAX & 0xFF00) >> 8); /* index 10, shift 8 */

    pstAeSnsDft->f32Fps = f32Fps;
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 2; /* sub 2 */
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];

    return;
}

static HI_VOID cmos_slow_framerate_set(VI_PIPE vi_pipe, HI_U32 u32FullLines,
                                       AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    GC2053_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);
    u32FullLines = (u32FullLines > GC2053_FULL_LINES_MAX) ? GC2053_FULL_LINES_MAX : u32FullLines;
    pstSnsState->au32FL[0] = u32FullLines;

    pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = 0x0; /* index 8 */
    pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = (u32FullLines & 0xFF); /* index 9 */
    pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = ((u32FullLines & 0xFF00) >> 8); /* index 10, shift 8 */

    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
    pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 2; /* sub 2 */

    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static HI_VOID cmos_inttime_update(VI_PIPE vi_pipe, HI_U32 u32IntTime)
{
    HI_U32 u32Val;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    GC2053_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32Val = (u32IntTime > GC2053_FULL_LINES_MAX) ? GC2053_FULL_LINES_MAX : u32IntTime;

    pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = (u32Val & 0xFF);
    pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = ((u32Val & 0xFF00) >> 8); /* shift 8 */
    return;
}
#define GC2053_REG_VAL_TAB_ROW    29
#define GC2053_REG_VAL_TAB_COL    4
static HI_U32 g_regValTable[GC2053_REG_VAL_TAB_ROW][GC2053_REG_VAL_TAB_COL] = {
    {0x00, 0x00, 0x01, 0x00}, /* 0xb4 0xb3 0xb8 0xb9 */
    {0x00, 0x10, 0x01, 0x0c},
    {0x00, 0x20, 0x01, 0x1b},
    {0x00, 0x30, 0x01, 0x2c},
    {0x00, 0x40, 0x01, 0x3f},
    {0x00, 0x50, 0x02, 0x16},
    {0x00, 0x60, 0x02, 0x35},
    {0x00, 0x70, 0x03, 0x16},
    {0x00, 0x80, 0x04, 0x02},
    {0x00, 0x90, 0x04, 0x31},
    {0x00, 0xa0, 0x05, 0x32},
    {0x00, 0xb0, 0x06, 0x35},
    {0x00, 0xc0, 0x08, 0x04},
    {0x00, 0x5a, 0x09, 0x19},
    {0x00, 0x83, 0x0b, 0x0f},
    {0x00, 0x93, 0x0d, 0x12},
    {0x00, 0x84, 0x10, 0x00},
    {0x00, 0x94, 0x12, 0x3a},
    {0x01, 0x2c, 0x1a, 0x02},
    {0x01, 0x3c, 0x1b, 0x20},
    {0x00, 0x8c, 0x20, 0x0f},
    {0x00, 0x9c, 0x26, 0x07},
    {0x02, 0x64, 0x36, 0x21},
    {0x02, 0x74, 0x37, 0x3a},
    {0x00, 0xc6, 0x3d, 0x02},
    {0x00, 0xdc, 0x3f, 0x3f},
    {0x02, 0x85, 0x3f, 0x3f},
    {0x02, 0x95, 0x3f, 0x3f},
    {0x00, 0xce, 0x3f, 0x3f},
};

#define GC2053_ANALOG_GAIN_LUT_SIZE    29
static HI_U32 g_analog_gain_table[GC2053_ANALOG_GAIN_LUT_SIZE] = {
    1024, 1230, 1440, 1730, 2032, 2380, 2880, 3460, 4080, 4800, 5776,
    6760, 8064, 9500, 11552, 13600, 16132, 18912, 22528, 27036, 32340,
    38256, 45600, 53912, 63768, 76880, 92300, 108904, 123568,
};

static HI_VOID cmos_again_calc_table(VI_PIPE vi_pipe, HI_U32 *pu32AgainLin, HI_U32 *pu32AgainDb)
{
    int again;
    int i;
    static HI_U8 againmax = 28; /* adgain max 28 */

    CMOS_CHECK_POINTER_VOID(pu32AgainLin);
    CMOS_CHECK_POINTER_VOID(pu32AgainDb);

    again = *pu32AgainLin;

    if (again >= g_analog_gain_table[againmax]) {
        *pu32AgainLin = g_analog_gain_table[againmax];
        *pu32AgainDb = againmax;
    } else {
        for (i = 1; i < GC2053_ANALOG_GAIN_LUT_SIZE; i++) {
            if (again < g_analog_gain_table[i]) {
                *pu32AgainLin = g_analog_gain_table[i - 1];
                *pu32AgainDb = i - 1;
                break;
            }
        }
    }

    return;
}

static HI_VOID cmos_gains_update(VI_PIPE vi_pipe, HI_U32 u32Again, HI_U32 u32Dgain)
{
    HI_U8 u8DgainHigh, u8DgainLow;

    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    GC2053_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u8DgainHigh = (u32Dgain >> 6) & 0x0f; /* shift 6 */
    u8DgainLow = (u32Dgain & 0x3f) << 2; /* shift 2 */

    pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = g_regValTable[u32Again][0]; /* index 2 */
    pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = g_regValTable[u32Again][1]; /* index 3 */
    pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = g_regValTable[u32Again][2]; /* index 4, index 2 */
    pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = g_regValTable[u32Again][3]; /* index 5, index 3 */

    pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = u8DgainLow; /* index 7 */
    pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = u8DgainHigh; /* index 6 */

    return;
}

static HI_VOID cmos_get_inttime_max(VI_PIPE vi_pipe,
                                    HI_U16 u16ManRatioEnable,
                                    HI_U32 *au32Ratio,
                                    HI_U32 *au32IntTimeMax,
                                    HI_U32 *au32IntTimeMin,
                                    HI_U32 *pu32LFMaxIntTime)
{
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
    pstExpFuncs->pfn_cmos_get_ae_default    = cmos_get_ae_default;
    pstExpFuncs->pfn_cmos_fps_set           = cmos_fps_set;
    pstExpFuncs->pfn_cmos_slow_framerate_set = cmos_slow_framerate_set;
    pstExpFuncs->pfn_cmos_inttime_update    = cmos_inttime_update;
    pstExpFuncs->pfn_cmos_gains_update      = cmos_gains_update;
    pstExpFuncs->pfn_cmos_again_calc_table  = cmos_again_calc_table;
    pstExpFuncs->pfn_cmos_dgain_calc_table  = HI_NULL;
    pstExpFuncs->pfn_cmos_get_inttime_max   = cmos_get_inttime_max;
    pstExpFuncs->pfn_cmos_ae_fswdr_attr_set = cmos_ae_fswdr_attr_set;

    return HI_SUCCESS;
}

/* awb static param */
#define CALIBRATE_STATIC_TEMP                         4908
#define CALIBRATE_STATIC_WB_R_GAIN                    388
#define CALIBRATE_STATIC_WB_GR_GAIN                   256
#define CALIBRATE_STATIC_WB_GB_GAIN                   256
#define CALIBRATE_STATIC_WB_B_GAIN                    519

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1                              39
#define CALIBRATE_AWB_P2                              152
#define CALIBRATE_AWB_Q1                              (-65)
#define CALIBRATE_AWB_A1                              201612
#define CALIBRATE_AWB_B1                              128
#define CALIBRATE_AWB_C1                              (-147924)

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN                          0
#define GOLDEN_BGAIN                          0
static HI_S32 cmos_get_awb_default(VI_PIPE vi_pipe, AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    HI_S32 i;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstAwbSnsDft);
    GC2053_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset_s(pstAwbSnsDft, sizeof(AWB_SENSOR_DEFAULT_S), 0, sizeof(AWB_SENSOR_DEFAULT_S));
    pstAwbSnsDft->u16WbRefTemp = CALIBRATE_STATIC_TEMP;

    pstAwbSnsDft->au16GainOffset[0] = CALIBRATE_STATIC_WB_R_GAIN;  /* 0: R channel 388 */
    pstAwbSnsDft->au16GainOffset[1] = CALIBRATE_STATIC_WB_GR_GAIN; /* 1: Gr channel 256 */
    pstAwbSnsDft->au16GainOffset[2] = CALIBRATE_STATIC_WB_GB_GAIN; /* 2: Gb channel 256 */
    pstAwbSnsDft->au16GainOffset[3] = CALIBRATE_STATIC_WB_B_GAIN;  /* 3: B channel 519 */

    pstAwbSnsDft->as32WbPara[0] = CALIBRATE_AWB_P1; /* index0: p1 39 */
    pstAwbSnsDft->as32WbPara[1] = CALIBRATE_AWB_P2; /* index1: p2 152 */
    pstAwbSnsDft->as32WbPara[2] = CALIBRATE_AWB_Q1; /* index2: q1 -65 */
    pstAwbSnsDft->as32WbPara[3] = CALIBRATE_AWB_A1; /* index3: a1 201612 */
    pstAwbSnsDft->as32WbPara[4] = CALIBRATE_AWB_B1; /* index4: b1 128 */
    pstAwbSnsDft->as32WbPara[5] = CALIBRATE_AWB_C1; /* index5: c1 -147924 */
    pstAwbSnsDft->u16GoldenRgain = GOLDEN_RGAIN;
    pstAwbSnsDft->u16GoldenBgain = GOLDEN_BGAIN;
    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:
            memcpy_s(&pstAwbSnsDft->stCcm, sizeof(AWB_CCM_S), &g_stAwbCcm, sizeof(AWB_CCM_S));
            memcpy_s(&pstAwbSnsDft->stAgcTbl, sizeof(AWB_AGC_TABLE_S), &g_stAwbAgcTable, sizeof(AWB_AGC_TABLE_S));
            break;
    }
    pstAwbSnsDft->u16SampleRgain = g_au16SampleRgain[vi_pipe];
    pstAwbSnsDft->u16SampleBgain = g_au16SampleBgain[vi_pipe];
    pstAwbSnsDft->u16InitRgain = g_au16InitWBGain[vi_pipe][0]; /* 0: Rgain */
    pstAwbSnsDft->u16InitGgain = g_au16InitWBGain[vi_pipe][1]; /* 1: Ggain */
    pstAwbSnsDft->u16InitBgain = g_au16InitWBGain[vi_pipe][2]; /* 2: Bgain */
    pstAwbSnsDft->u8AWBRunInterval = 4; /* RunInterval 4 */

    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        pstAwbSnsDft->au16InitCCM[i] = g_au16InitCCM[vi_pipe][i];
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

static ISP_CMOS_DNG_COLORPARAM_S g_stDngColorParam = {
    {378, 256, 430},
    {439, 256, 439}
};

static HI_VOID cmos_get_isp_dng_default(ISP_SNS_STATE_S *pstSnsState, ISP_CMOS_DEFAULT_S *pstDef)
{
    memcpy_s(&pstDef->stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S),
             &g_stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S));

    switch (pstSnsState->u8ImgMode) {
        default:
        case GC2053_SENSOR_1080P_30FPS_LINEAR_MODE:
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
    pstDef->unKey.bit1AntiFalseColor = 1;
    pstDef->pstAntiFalseColor        = &g_stIspAntiFalseColor;
    pstDef->unKey.bit1Ldci           = 1;
    pstDef->pstLdci                  = &g_stIspLdci;
    pstDef->unKey.bit1Gamma          = 1;
    pstDef->pstGamma                 = &g_stIspGamma;
    pstDef->unKey.bit1Detail         = 1;
    pstDef->pstDetail                = &g_stIspDetail;
    pstDef->unKey.bit1Lcac           = 1;
    pstDef->pstLcac                  = &g_stIspLCac;
#ifdef CONFIG_HI_ISP_CR_SUPPORT
    pstDef->unKey.bit1Ge             = 1;
    pstDef->pstGe                    = &g_stIspGe;
#endif
    pstDef->unKey.bit1Dehaze         = 1;
    pstDef->pstDehaze                = &g_stIspDehaze;
    memcpy_s(&pstDef->stNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S),
             &g_stIspNoiseCalibratio, sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
    return;
}

static HI_S32 cmos_get_isp_default(VI_PIPE vi_pipe, ISP_CMOS_DEFAULT_S *pstDef)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstDef);
    GC2053_SENSOR_GET_CTX(vi_pipe, pstSnsState);
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
#ifdef CONFIG_HI_ISP_PREGAMMA_SUPPORT
    pstDef->unKey.bit1PreGamma = 0;
    pstDef->pstPreGamma        = &g_stPreGamma;
#endif

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:
            cmos_get_isp_linear_default(pstDef);
            break;
    }

    pstDef->stSensorMode.u32SensorID = GC2053_ID;
    pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

    cmos_get_isp_dng_default(pstSnsState, pstDef);
    return HI_SUCCESS;
}
static HI_S32 cmos_get_isp_black_level(VI_PIPE vi_pipe, ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstBlackLevel);
    GC2053_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    /* Don't need to update black level when iso change */
    pstBlackLevel->bUpdate = HI_FALSE;
    pstBlackLevel->au16BlackLevel[0] = 0x100; /* 0: R channel */
    pstBlackLevel->au16BlackLevel[1] = 0x100; /* 1: Gr channel */
    pstBlackLevel->au16BlackLevel[2] = 0x100; /* 2: Gb channel */
    pstBlackLevel->au16BlackLevel[3] = 0x100; /* 3: B channel */

    return HI_SUCCESS;
}

static HI_VOID cmos_set_pixel_detect(VI_PIPE vi_pipe, HI_BOOL bEnable)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    GC2053_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    return;
}

static HI_S32 cmos_set_wdr_mode(VI_PIPE vi_pipe, HI_U8 u8Mode)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    GC2053_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);
    pstSnsState->bSyncInit = HI_FALSE;

    switch (u8Mode) {
        case WDR_MODE_NONE:
            pstSnsState->enWDRMode = WDR_MODE_NONE;
            printf("GC2053_SENSOR_1080P_30FPS_LINEAR_MODE\n");
            break;
        default:
            SNS_ERR_TRACE("cmos_set_wdr_mode_NOT support this mode!\n");
            return HI_FAILURE;
    }

    memset_s(pstSnsState->au32WDRIntTime, sizeof(pstSnsState->au32WDRIntTime), 0, sizeof(pstSnsState->au32WDRIntTime));

    return HI_SUCCESS;
}
static HI_VOID cmos_comm_sns_reg_info_init(VI_PIPE vi_pipe, ISP_SNS_STATE_S *pstSnsState)
{
    hi_s32 i;
    pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
    pstSnsState->astRegsInfo[0].unComBus.s8I2cDev = g_aunGc2053BusInfo[vi_pipe].s8I2cDev;
    pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 3; /* DelayMax 3 */
    pstSnsState->astRegsInfo[0].u32RegNum = 11; /* RegNum 11 */
    for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
        pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
        pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr = GC2053_I2C_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[i].u32AddrByteNum = GC2053_ADDR_BYTE;
        pstSnsState->astRegsInfo[0].astI2cData[i].u32DataByteNum = GC2053_DATA_BYTE;
    }
    pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum  = 1;
    pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr  = GC2053_EXPTIME_ADDR_L;
    pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum  = 1;
    pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr  = GC2053_EXPTIME_ADDR_H;

    pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum  = 1; /* index 2 */
    pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr  = GC2053_AGAIN_ADDR_H; /* index 2 */
    pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum  = 1; /* index 3 */
    pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr  = GC2053_AGAIN_ADDR_L; /* index 3 */

    pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum  = 1; /* index 4 */
    pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr  = GC2053_DGAIN_ADDR_L; /* index 4 */
    pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum  = 1; /* index 5 */
    pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr  = GC2053_DGAIN_ADDR_H; /* index 5 */

    pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum  = 1; /* index 7 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr  = GC2053_AUTO_PREGAIN_ADDR_L; /* index 7 */
    pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum  = 1; /* index 6 */
    pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr  = GC2053_AUTO_PREGAIN_ADDR_H; /* index 6 */

    pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 0; /* index 8 */
    pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr = GC2053_STATUS_ADDR; /* index 8 */

    pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum  = 0; /* index 9 */
    pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr  = GC2053_VMAX_ADDR_L; /* index 9 */
    pstSnsState->astRegsInfo[0].astI2cData[10].u8DelayFrmNum  = 0; /* index 10 */
    pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr  = GC2053_VMAX_ADDR_H; /* index 10 */

    pstSnsState->bSyncInit = HI_TRUE;

    return;
}
static HI_VOID cmos_sns_reg_info_update(VI_PIPE vi_pipe, ISP_SNS_STATE_S *pstSnsState)
{
    HI_S32 i;
    for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum - 2; i++) { /* RegNum sub 2 */
        if (pstSnsState->astRegsInfo[0].astI2cData[i].u32Data ==
            pstSnsState->astRegsInfo[1].astI2cData[i].u32Data) {
            pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = HI_FALSE;
        } else {
            pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
        }
    }

    if ((pstSnsState->astRegsInfo[0].astI2cData[6].bUpdate == HI_TRUE) || /* index 6 */
        (pstSnsState->astRegsInfo[0].astI2cData[7].bUpdate == HI_TRUE)) { /* index 7 */
        pstSnsState->astRegsInfo[0].astI2cData[6].bUpdate = HI_TRUE; /* index 6 */
        pstSnsState->astRegsInfo[0].astI2cData[7].bUpdate = HI_TRUE; /* index 7 */
    }
    return;
}

static HI_S32 cmos_get_sns_regs_info(VI_PIPE vi_pipe, ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstSnsRegsInfo);
    GC2053_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    if ((pstSnsState->bSyncInit == HI_FALSE) || (pstSnsRegsInfo->bConfig == HI_FALSE)) {
        cmos_comm_sns_reg_info_init(vi_pipe, pstSnsState);
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
    HI_U8 u8SensorImageMode;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstSensorImageMode);
    GC2053_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    u8SensorImageMode = pstSnsState->u8ImgMode;
    pstSnsState->bSyncInit = HI_FALSE;

    if ((pstSensorImageMode->u16Width <= 1920) && (pstSensorImageMode->u16Height <= 1080)) { /* w 1920, h 1080 */
        if (pstSensorImageMode->f32Fps <= 30) { /* 30fps */
            u8SensorImageMode     = GC2053_SENSOR_1080P_30FPS_LINEAR_MODE;
            pstSnsState->u32FLStd = GC2053_VMAX_1080P30_LINEAR;
        } else {
            SNS_ERR_TRACE("Not support! Width:%d, Height:%d, Fps:%f\n",
                          pstSensorImageMode->u16Width,
                          pstSensorImageMode->u16Height,
                          pstSensorImageMode->f32Fps);
            return HI_FAILURE;
        }
    } else {
        SNS_ERR_TRACE("Not support! Width:%d, Height:%d, Fps:%f\n",
                      pstSensorImageMode->u16Width,
                      pstSensorImageMode->u16Height,
                      pstSensorImageMode->f32Fps);
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

    GC2053_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->bInit     = HI_FALSE;
    pstSnsState->bSyncInit = HI_FALSE;
    pstSnsState->u8ImgMode = GC2053_SENSOR_1080P_30FPS_LINEAR_MODE;
    pstSnsState->enWDRMode = WDR_MODE_NONE;
    pstSnsState->u32FLStd  = GC2053_VMAX_1080P30_LINEAR;
    pstSnsState->au32FL[0] = GC2053_VMAX_1080P30_LINEAR;
    pstSnsState->au32FL[1] = GC2053_VMAX_1080P30_LINEAR;

    memset_s(&pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset_s(&pstSnsState->astRegsInfo[1], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
    return;
}

static HI_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    CMOS_CHECK_POINTER(pstSensorExpFunc);
    memset_s(pstSensorExpFunc, sizeof(ISP_SENSOR_EXP_FUNC_S), 0, sizeof(ISP_SENSOR_EXP_FUNC_S));
    pstSensorExpFunc->pfn_cmos_sensor_init = gc2053_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit = gc2053_exit;
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
static HI_S32 gc2053_set_bus_info(VI_PIPE vi_pipe, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunGc2053BusInfo[vi_pipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

    return HI_SUCCESS;
}

static HI_S32 sensor_ctx_init(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    GC2053_SENSOR_GET_CTX(vi_pipe, pastSnsStateCtx);

    if (pastSnsStateCtx == HI_NULL) {
        pastSnsStateCtx = (ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
        if (pastSnsStateCtx == HI_NULL) {
            SNS_ERR_TRACE("Isp[%d] SnsCtx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset_s(pastSnsStateCtx, sizeof(ISP_SNS_STATE_S), 0, sizeof(ISP_SNS_STATE_S));

    GC2053_SENSOR_SET_CTX(vi_pipe, pastSnsStateCtx);

    return HI_SUCCESS;
}

static HI_VOID sensor_ctx_exit(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    GC2053_SENSOR_GET_CTX(vi_pipe, pastSnsStateCtx);
    SENSOR_FREE(pastSnsStateCtx);
    GC2053_SENSOR_RESET_CTX(vi_pipe);
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

    stSnsAttrInfo.eSensorId = GC2053_ID;
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

    s32Ret = HI_MPI_ISP_SensorUnRegCallBack(vi_pipe, GC2053_ID);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AE_SensorUnRegCallBack(vi_pipe, pstAeLib, GC2053_ID);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AWB_SensorUnRegCallBack(vi_pipe, pstAwbLib, GC2053_ID);
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

    g_au32InitExposure[vi_pipe]  = pstInitAttr->u32Exposure;
    g_au32LinesPer500ms[vi_pipe] = pstInitAttr->u32LinesPer500ms;
    g_au16InitWBGain[vi_pipe][0] = pstInitAttr->u16WBRgain; /* 0: Rgain */
    g_au16InitWBGain[vi_pipe][1] = pstInitAttr->u16WBGgain; /* 1: Ggain */
    g_au16InitWBGain[vi_pipe][2] = pstInitAttr->u16WBBgain; /* 2: Bgain */
    g_au16SampleRgain[vi_pipe]   = pstInitAttr->u16SampleRgain;
    g_au16SampleBgain[vi_pipe]   = pstInitAttr->u16SampleBgain;
    g_quick_start_en[vi_pipe] = pstInitAttr->stQuickStart.bQuickStartEn;

    g_abAERouteExValid[vi_pipe] = pstInitAttr->bAERouteExValid;
    memcpy_s(&g_astInitAERoute[vi_pipe], sizeof(ISP_AE_ROUTE_S), &pstInitAttr->stAERoute, sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&g_astInitAERouteEx[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S), &pstInitAttr->stAERouteEx,
             sizeof(ISP_AE_ROUTE_EX_S));
    memcpy_s(&g_astInitAERouteSF[vi_pipe], sizeof(ISP_AE_ROUTE_S), &pstInitAttr->stAERouteSF, sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&g_astInitAERouteSFEx[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S), &pstInitAttr->stAERouteSFEx,
             sizeof(ISP_AE_ROUTE_EX_S));

    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        g_au16InitCCM[vi_pipe][i] = pstInitAttr->au16CCM[i];
    }
    return HI_SUCCESS;
}

ISP_SNS_OBJ_S stSnsGc2053Obj = {
    .pfnRegisterCallback    = sensor_register_callback,
    .pfnUnRegisterCallback  = sensor_unregister_callback,
    .pfnStandby             = gc2053_standby,
    .pfnRestart             = gc2053_restart,
    .pfnMirrorFlip          = gc2053_mirror_flip,
    .pfnWriteReg            = gc2053_write_register,
    .pfnReadReg             = gc2053_read_register,
    .pfnSetBusInfo          = gc2053_set_bus_info,
    .pfnSetInit             = sensor_set_init
};

