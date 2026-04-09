/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of imx390_cmos.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "imx390_cmos.h"
#include "imx390_cmos_ex.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "mpi_isp.h"
#include "mpi_ae.h"
#include "mpi_awb.h"

#define IMX390_ID                    390
#define SENSOR_IMX390_WIDTH          1920
#define SENSOR_IMX390_HEIGHT         1080

#define HIGHER_4BITS(x) (((x) & 0xf0000) >> 16)
#define HIGH_8BITS(x) (((x) & 0xff00) >> 8)
#define LOW_8BITS(x)  ((x) & 0x00ff)

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/
#define IMX390_SENSOR_SET_CTX(dev, pstCtx)   ((g_pastImx390[dev]) = (pstCtx))
#define IMX390_SENSOR_RESET_CTX(dev)         (g_pastImx390[dev] = HI_NULL)

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

static HI_BOOL g_abAERouteExValid[ISP_MAX_PIPE_NUM] = {0};
static ISP_AE_ROUTE_S g_astInitAERoute[ISP_MAX_PIPE_NUM] = {{0}};
static ISP_AE_ROUTE_EX_S g_astInitAERouteEx[ISP_MAX_PIPE_NUM] = {{0}};
static ISP_AE_ROUTE_S g_astInitAERouteSF[ISP_MAX_PIPE_NUM] = {{0}};
static ISP_AE_ROUTE_EX_S g_astInitAERouteSFEx[ISP_MAX_PIPE_NUM] = {{0}};

ISP_SNS_COMMBUS_U g_aunImx390BusInfo[ISP_MAX_PIPE_NUM] = {
    [0] = { .s8I2cDev = 0},
    [1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1}
};

ISP_SNS_STATE_S *g_pastImx390[ISP_MAX_PIPE_NUM] = {HI_NULL};

ISP_SNS_COMMBUS_U *imx390_get_bus_Info(VI_PIPE vi_pipe)
{
    return &g_aunImx390BusInfo[vi_pipe];
}

ISP_SNS_STATE_S *imx390_get_ctx(VI_PIPE vi_pipe)
{
    return g_pastImx390[vi_pipe];
}

const IMX390_VIDEO_MODE_TBL_S g_astImx390ModeTbl[IMX390_MODE_BUTT] = {
    {IMX390_VMAX_1080P30_LINEAR,   IMX390_FULL_LINES_MAX,          30, 30,
     1920, 1080, 0, WDR_MODE_NONE,       "IMX390_SENSOR_1080P_30FPS_LINEAR_MODE"},
};

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
#define IMX390_SHS1_ADDR_L                            0x000C
#define IMX390_SHS1_ADDR_M                            0x000D
#define IMX390_SHS1_ADDR_H                            0x000E

#define IMX390_SHS2_ADDR_L                            0x0010
#define IMX390_SHS2_ADDR_M                            0x0011
#define IMX390_SHS2_ADDR_H                            0x0012

#define IMX390_AGAIN_SP1H_ADDR_L                      0x0018
#define IMX390_AGAIN_SP1H_ADDR_H                      0x0019

#define IMX390_AGAIN_SP1L_ADDR_L                      0x001A
#define IMX390_AGAIN_SP1L_ADDR_H                      0x001B

#define IMX390_PGA_GAIN_SP1H_ADDR_L                   0x0024
#define IMX390_PGA_GAIN_SP1H_ADDR_H                   0x0025

#define IMX390_PGA_GAIN_SP1L_ADDR_L                   0x0026
#define IMX390_PGA_GAIN_SP1L_ADDR_H                   0x0027

#define IMX390_VMAX_ADDR_L                            0x2008
#define IMX390_VMAX_ADDR_M                            0x2009
#define IMX390_VMAX_ADDR_H                            0x200A


#define IMX390_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState)\
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
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FlickerFreq = 50 * 256; /* light flicker freq: 50Hz, accuracy: 256 */
    pstAeSnsDft->u32FullLinesMax = IMX390_FULL_LINES_MAX;
    pstAeSnsDft->u32HmaxTimes = (1000000000) / (pstSnsState->u32FLStd * 30); /* 1000000000ns, 30fps */

    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
    pstAeSnsDft->stIntTimeAccu.f32Offset = 0;

    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 1;

    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 1;

    pstAeSnsDft->u32ISPDgainShift = 8; /* accuracy: 8 */
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 6 << pstAeSnsDft->u32ISPDgainShift; /* max 6 */
    if (g_au32LinesPer500ms[vi_pipe] == 0) {
        pstAeSnsDft->u32LinesPer500ms = pstSnsState->u32FLStd * 30 / 2; /* 30fps, div 2 */
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

    pstAeSnsDft->u32MaxAgain = 32382; /* max 32382 */
    pstAeSnsDft->u32MinAgain = 1550; /* min 1550 */
    pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
    pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

    pstAeSnsDft->u32MaxDgain = 128914; /* max 128914 */
    pstAeSnsDft->u32MinDgain = 1024; /* min 1024 */
    pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
    pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

    pstAeSnsDft->u8AeCompensation = 0x38;
    pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;
    pstAeSnsDft->u32InitExposure = g_au32InitExposure[vi_pipe] ? g_au32InitExposure[vi_pipe] : 76151; /* init 76151 */

    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 2; /* sub 2 */
    pstAeSnsDft->u32MinIntTime = 2; /* min 2 */
    pstAeSnsDft->u32MaxIntTimeTarget = 65535; /* max 65535 */
    pstAeSnsDft->u32MinIntTimeTarget = 2; /* min 2 */
    pstAeSnsDft->bAERouteExValid = g_abAERouteExValid[vi_pipe];
    memcpy_s(&pstAeSnsDft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S),
        &g_astInitAERoute[vi_pipe],  sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&pstAeSnsDft->stAERouteAttrEx, sizeof(ISP_AE_ROUTE_EX_S),
        &g_astInitAERouteEx[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S));
    return;
}

static HI_S32 cmos_get_ae_default(VI_PIPE vi_pipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstAeSnsDft);
    IMX390_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset_s(&pstAeSnsDft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), 0, sizeof(ISP_AE_ROUTE_S));

    cmos_get_ae_comm_default(vi_pipe, pstAeSnsDft, pstSnsState);

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:   /* linear mode */

            cmos_get_ae_linear_default(vi_pipe, pstAeSnsDft, pstSnsState);
            break;
    }

    return HI_SUCCESS;
}

static HI_VOID cmos_config_vmax(ISP_SNS_STATE_S *pstSnsState, HI_U32 u32VMAX)
{
    if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
        pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = LOW_8BITS(u32VMAX);    /* array index  12 */
        pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = HIGH_8BITS(u32VMAX);   /* array index  11 */
        pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = HIGHER_4BITS(u32VMAX);  /* array index 10 */
    } else {
    }

    return;
}

/* the function of sensor set fps */
static HI_VOID cmos_fps_set(VI_PIPE vi_pipe, HI_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    HI_U32 u32Lines, u32LinesMax, u32VMAX;
    HI_FLOAT f32MaxFps;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    IMX390_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32Lines = g_astImx390ModeTbl[pstSnsState->u8ImgMode].u32VertiLines;
    u32LinesMax = g_astImx390ModeTbl[pstSnsState->u8ImgMode].u32MaxVertiLines;
    f32MaxFps = g_astImx390ModeTbl[pstSnsState->u8ImgMode].f32MaxFps;

    if ((f32Fps < f32MaxFps)) {
        SNS_ERR_TRACE("Not support Fps: %f\n", f32Fps);
        return;
    }

    u32VMAX = u32Lines * f32MaxFps / SNS_DIV_0_TO_1_FLOAT(f32Fps);
    u32VMAX = (u32VMAX > u32LinesMax) ? u32LinesMax : u32VMAX;

    cmos_config_vmax(pstSnsState, u32VMAX);

    pstAeSnsDft->f32Fps = f32Fps;
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 2; /* sub 2 */
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
    pstAeSnsDft->u32HmaxTimes =
        (1000000000) / (pstSnsState->u32FLStd * SNS_DIV_0_TO_1_FLOAT(f32Fps)); /* 1000000000ns */

    return;
}

static HI_VOID cmos_inttime_update_linear(VI_PIPE vi_pipe, HI_U32 u32IntTime)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    HI_U32 u32Value;

    IMX390_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32Value = pstSnsState->au32FL[0] - u32IntTime;

    pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = ((u32Value >> 16) & 0x0F); /* index 0, shift 16 */
    pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = ((u32Value >> 8) & 0xFF); /* index 1, shift 8 */
    pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = (u32Value & 0xFF); /* index 2 */

    pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = /* index 3 */
        pstSnsState->astRegsInfo[0].astI2cData[0].u32Data;
    pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = /* index 4 */
        pstSnsState->astRegsInfo[0].astI2cData[1].u32Data; /* index 1 */
    pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = /* index 5 */
        pstSnsState->astRegsInfo[0].astI2cData[2].u32Data; /* index 2 */

    return;
}


/* while isp notify ae to update sensor regs, ae call these funcs. */
static HI_VOID cmos_inttime_update(VI_PIPE vi_pipe, HI_U32 u32IntTime)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    IMX390_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
        cmos_inttime_update_linear(vi_pipe, u32IntTime);
    } else {
    }

    return;
}
#define AGAIN_NODE_NUM   101
#define DGAIN_NODE_NUM   141

static HI_U32 g_gain_table[] = {
    1024, 1060, 1097, 1136, 1176, 1217, 1260, 1304, 1350, 1397, 1446, 1497, 1550, 1604,
    1661, 1719, 1780, 1842, 1907, 1974, 2043, 2115, 2189, 2266, 2346, 2428, 2514, 2602,
    2693, 2788, 2886, 2987, 3092, 3201, 3314, 3430, 3551, 3675, 3805, 3938, 4077, 4220,
    4368, 4522, 4681, 4845, 5015, 5192, 5374, 5563, 5758, 5961, 6170, 6387, 6611, 6844,
    7084, 7333, 7591, 7858, 8134, 8420, 8716, 9022, 9339, 9667, 10007, 10359, 10723, 11099,
    11489, 11893, 12311, 12744, 13192, 13655, 14135, 14632, 15146, 15678, 16229, 16800, 17390, 18001,
    18634, 19289, 19966, 20668, 21394, 22146, 22925, 23730, 24564, 25427, 26321, 27246, 28203, 29194,
    30220, 31282, 32382, 33520, 34698, 35917, 37179, 38486, 39838, 41238, 42687, 44188, 45740, 47348,
    49012, 50734, 52517, 54363, 56273, 58251, 60298, 62417, 64610, 66881, 69231, 71664, 74182, 76789,
    79488, 82281, 85173, 88166, 91264, 94471, 97791, 101228, 104785, 108468, 112279, 116225, 120310, 124537, 128914
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

    IMX390_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = ((u32Again >> 8) & 7); /* index 6,shift 8 and 7 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = (u32Again & 0xFF); /* index 7 */

    pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = 0x0; /* index 8 */
    pstSnsState->astRegsInfo[0].astI2cData[9].u32Data = 0x0; /* index 9 */

    pstSnsState->astRegsInfo[0].astI2cData[13].u32Data = ((u32Dgain >> 8) & 3); /* index 13,shift 8 and 3 */
    pstSnsState->astRegsInfo[0].astI2cData[14].u32Data = (u32Dgain & 0xFF); /* index 14 */

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
    IMX390_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

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
    pstExpFuncs->pfn_cmos_slow_framerate_set = HI_NULL;
    pstExpFuncs->pfn_cmos_inttime_update    = cmos_inttime_update;
    pstExpFuncs->pfn_cmos_gains_update      = cmos_gains_update;
    pstExpFuncs->pfn_cmos_again_calc_table  = cmos_again_calc_table;
    pstExpFuncs->pfn_cmos_dgain_calc_table  = cmos_dgain_calc_table;
    pstExpFuncs->pfn_cmos_get_inttime_max   = cmos_get_inttime_max;
    pstExpFuncs->pfn_cmos_ae_fswdr_attr_set = cmos_ae_fswdr_attr_set;

    return HI_SUCCESS;
}

/* awb static param for Fuji Lens New IR_Cut */
#define CALIBRATE_STATIC_TEMP                         4950
#define CALIBRATE_STATIC_WB_R_GAIN                    450
#define CALIBRATE_STATIC_WB_GR_GAIN                   256
#define CALIBRATE_STATIC_WB_GB_GAIN                   256
#define CALIBRATE_STATIC_WB_B_GAIN                    447

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1                              (-53)
#define CALIBRATE_AWB_P2                              309
#define CALIBRATE_AWB_Q1                              0
#define CALIBRATE_AWB_A1                              160279
#define CALIBRATE_AWB_B1                              128
#define CALIBRATE_AWB_C1                              (-110119)

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN                                  0
#define GOLDEN_BGAIN                                  0
static HI_S32 cmos_get_awb_default(VI_PIPE vi_pipe, AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstAwbSnsDft);
    IMX390_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset_s(pstAwbSnsDft, sizeof(AWB_SENSOR_DEFAULT_S), 0, sizeof(AWB_SENSOR_DEFAULT_S));
    pstAwbSnsDft->u16WbRefTemp = CALIBRATE_STATIC_TEMP; /* u16WbRefTemp 4950 */

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
        case IMX390_SENSOR_1080P_30FPS_LINEAR_MODE:
            pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 12; /* 12bit */
            pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 4095; /* max 4095 */
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

static HI_S32 cmos_get_isp_default(VI_PIPE vi_pipe, ISP_CMOS_DEFAULT_S *pstDef)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstDef);
    IMX390_SENSOR_GET_CTX(vi_pipe, pstSnsState);
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
    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:

            cmos_get_isp_linear_default(pstDef);

            break;
    }

    pstDef->stWdrSwitchAttr.au32ExpRatio[0] = 0x40;

    pstDef->stSensorMode.u32SensorID = IMX390_ID;
    pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;
    cmos_get_isp_dng_default(pstSnsState, pstDef);

    return HI_SUCCESS;
}
static HI_S32 cmos_get_isp_black_level(VI_PIPE vi_pipe, ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
    HI_S32  i;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    CMOS_CHECK_POINTER(pstBlackLevel);
    IMX390_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    /* Don't need to update black level when iso change */
    pstBlackLevel->bUpdate = HI_FALSE;

    /* black level of linear mode */
    if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            pstBlackLevel->au16BlackLevel[i] = 0x100;
        }
    } else {
    }

    return HI_SUCCESS;
}

static HI_VOID cmos_set_pixel_detect(VI_PIPE vi_pipe, HI_BOOL bEnable)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    IMX390_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    return;
}


static HI_S32 cmos_set_wdr_mode(VI_PIPE vi_pipe, HI_U8 u8Mode)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    IMX390_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    pstSnsState->bSyncInit    = HI_FALSE;

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
    pstSnsState->astRegsInfo[0].unComBus.s8I2cDev = g_aunImx390BusInfo[vi_pipe].s8I2cDev;
    pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2; /* DelayMax 2 */
    pstSnsState->astRegsInfo[0].u32RegNum = 15; /* RegNum 15 */

    for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
        pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
        pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr = imx390_get_serdes_i2c_addr(vi_pipe);
        pstSnsState->astRegsInfo[0].astI2cData[i].u32AddrByteNum = IMX390_ADDR_BYTE;
        pstSnsState->astRegsInfo[0].astI2cData[i].u32DataByteNum = IMX390_DATA_BYTE;
    }

    /* Linear Mode Regs */
    pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0; /* index 0 */
    pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr = IMX390_SHS1_ADDR_H; /* index 0 */
    pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0; /* index 1 */
    pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr = IMX390_SHS1_ADDR_M; /* index 1 */
    pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0; /* index 2 */
    pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr = IMX390_SHS1_ADDR_L; /* index 2 */

    pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 0; /* index 3 */
    pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr = IMX390_SHS2_ADDR_H; /* index 3 */
    pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 0;  /* index 4 */
    pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr = IMX390_SHS2_ADDR_M; /* index 4 */
    pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0; /* index 5 */
    pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr = IMX390_SHS2_ADDR_L; /* index 5 */

    pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0; /* index 6 */
    pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr = IMX390_AGAIN_SP1H_ADDR_H; /* index 6 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 0; /* index 7 */
    pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr = IMX390_AGAIN_SP1H_ADDR_L; /* index 7 */

    pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 0; /* index 8 */
    pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr = IMX390_AGAIN_SP1L_ADDR_H; /* index 8 */
    pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum = 0; /* index 9 */
    pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr = IMX390_AGAIN_SP1L_ADDR_L; /* index 9 */

    pstSnsState->astRegsInfo[0].astI2cData[10].u8DelayFrmNum = 1; /* index 10 */
    pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr = IMX390_VMAX_ADDR_H; /* index 10 */
    pstSnsState->astRegsInfo[0].astI2cData[11].u8DelayFrmNum = 1; /* index 11 */
    pstSnsState->astRegsInfo[0].astI2cData[11].u32RegAddr = IMX390_VMAX_ADDR_M; /* index 11 */
    pstSnsState->astRegsInfo[0].astI2cData[12].u8DelayFrmNum = 1; /* index 12 */
    pstSnsState->astRegsInfo[0].astI2cData[12].u32RegAddr = IMX390_VMAX_ADDR_L; /* index 12 */

    pstSnsState->astRegsInfo[0].astI2cData[13].u8DelayFrmNum = 0; /* index 13 */
    pstSnsState->astRegsInfo[0].astI2cData[13].u32RegAddr = IMX390_PGA_GAIN_SP1H_ADDR_H; /* index 13 */
    pstSnsState->astRegsInfo[0].astI2cData[14].u8DelayFrmNum = 0; /* index 14 */
    pstSnsState->astRegsInfo[0].astI2cData[14].u32RegAddr = IMX390_PGA_GAIN_SP1H_ADDR_L; /* index 14 */

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
    IMX390_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    if ((pstSnsState->bSyncInit == HI_FALSE) || (pstSnsRegsInfo->bConfig == HI_FALSE)) {
        cmos_comm_sns_reg_info_init(vi_pipe, pstSnsState);

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

static HI_VOID cmos_config_image_mode_param(VI_PIPE vi_pipe, HI_U8 u8SensorImageMode,
    ISP_SNS_STATE_S *pstSnsState)
{
    switch (u8SensorImageMode) {
        default:
        case IMX390_SENSOR_1080P_30FPS_LINEAR_MODE:
            pstSnsState->u32FLStd          = IMX390_VMAX_1080P30_LINEAR;
            break;
    }
}

static HI_S32 cmos_set_image_mode(VI_PIPE vi_pipe, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode)
{
    HI_U32 i;
    HI_U8 u8SensorImageMode;
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;
    CMOS_CHECK_POINTER(pstSensorImageMode);
    IMX390_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    u8SensorImageMode = pstSnsState->u8ImgMode;
    pstSnsState->bSyncInit = HI_FALSE;

    for (i = 0; i < IMX390_MODE_BUTT; i++) {
        if (pstSensorImageMode->f32Fps <= g_astImx390ModeTbl[i].f32MaxFps &&
            pstSensorImageMode->u16Width <= g_astImx390ModeTbl[i].u32Width &&
            pstSensorImageMode->u16Height <= g_astImx390ModeTbl[i].u32Height &&
            pstSnsState->enWDRMode == g_astImx390ModeTbl[i].enWDRMode) {
            u8SensorImageMode = (IMX390_RES_MODE_E)i;
            break;
        }
    }

    if (i >= IMX390_MODE_BUTT) {
        IMX390_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState);
        return HI_FAILURE;
    }

    cmos_config_image_mode_param(vi_pipe, u8SensorImageMode, pstSnsState);

    if ((pstSnsState->bInit == HI_TRUE) && (u8SensorImageMode == pstSnsState->u8ImgMode)) {
        return ISP_DO_NOT_NEED_SWITCH_IMAGEMODE; /* Don't need to switch SensorImageMode */
    }
    pstSnsState->u8ImgMode = u8SensorImageMode;
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstSnsState->au32FL[1] = pstSnsState->au32FL[0];
    return HI_SUCCESS;
}

static HI_VOID sensor_global_init(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *pstSnsState = HI_NULL;

    IMX390_SENSOR_GET_CTX(vi_pipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->bInit = HI_FALSE;
    pstSnsState->bSyncInit = HI_FALSE;
    pstSnsState->u8ImgMode = IMX390_SENSOR_1080P_30FPS_LINEAR_MODE;
    pstSnsState->enWDRMode = WDR_MODE_NONE;
    pstSnsState->u32FLStd  = IMX390_VMAX_1080P30_LINEAR;
    pstSnsState->au32FL[0] = IMX390_VMAX_1080P30_LINEAR;
    pstSnsState->au32FL[1] = IMX390_VMAX_1080P30_LINEAR;

    memset_s(&pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset_s(&pstSnsState->astRegsInfo[1], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
    return;
}

static HI_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    CMOS_CHECK_POINTER(pstSensorExpFunc);

    memset_s(pstSensorExpFunc, sizeof(ISP_SENSOR_EXP_FUNC_S), 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init         = imx390_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit         = imx390_exit;
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
static HI_S32 imx390_set_bus_info(VI_PIPE vi_pipe, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunImx390BusInfo[vi_pipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

    return HI_SUCCESS;
}

static HI_S32 sensor_ctx_init(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    IMX390_SENSOR_GET_CTX(vi_pipe, pastSnsStateCtx);

    if (pastSnsStateCtx == HI_NULL) {
        pastSnsStateCtx = (ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
        if (pastSnsStateCtx == HI_NULL) {
            SNS_ERR_TRACE("Isp[%d] SnsCtx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset_s(pastSnsStateCtx, sizeof(ISP_SNS_STATE_S), 0, sizeof(ISP_SNS_STATE_S));

    IMX390_SENSOR_SET_CTX(vi_pipe, pastSnsStateCtx);

    return HI_SUCCESS;
}

static HI_VOID sensor_ctx_exit(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = HI_NULL;

    IMX390_SENSOR_GET_CTX(vi_pipe, pastSnsStateCtx);
    SENSOR_FREE(pastSnsStateCtx);
    IMX390_SENSOR_RESET_CTX(vi_pipe);
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

    stSnsAttrInfo.eSensorId = IMX390_ID;
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

    s32Ret = HI_MPI_ISP_SensorUnRegCallBack(vi_pipe, IMX390_ID);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AE_SensorUnRegCallBack(vi_pipe, pstAeLib, IMX390_ID);
    if (s32Ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_AWB_SensorUnRegCallBack(vi_pipe, pstAwbLib, IMX390_ID);
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

ISP_SNS_OBJ_S stSnsImx390Obj = {
    .pfnRegisterCallback    = sensor_register_callback,
    .pfnUnRegisterCallback  = sensor_unregister_callback,
    .pfnStandby             = imx390_standby,
    .pfnRestart             = imx390_restart,
    .pfnMirrorFlip          = HI_NULL,
    .pfnWriteReg            = imx390_write_register,
    .pfnReadReg             = imx390_read_register,
    .pfnSetBusInfo          = imx390_set_bus_info,
    .pfnSetBusExInfo        = imx390_set_bus_ex_info,
    .pfnSetInit             = sensor_set_init
};

