/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of ov2775_cmos.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "ov2775_cmos.h"
#include "ov2775_cmos_ex.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "mpi_isp.h"
#include "mpi_ae.h"
#include "mpi_awb.h"

#define OV2775_ID  2775

ISP_SNS_STATE_S *g_ov2775[ISP_MAX_PIPE_NUM] = {HI_NULL};
#define ov2775_sensor_set_ctx(dev, ctx) ((g_ov2775[dev]) = (ctx))
#define ov2775_sensor_reset_ctx(dev)    (g_ov2775[dev] = HI_NULL)

ISP_SNS_COMMBUS_U g_ov2775_bus_info[ISP_MAX_PIPE_NUM] = {
    [0] = { .s8I2cDev = 0},
    [1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1}
};

ISP_SNS_COMMBUS_U *ov2775_get_bus_info(VI_PIPE vi_pipe)
{
    return &g_ov2775_bus_info[vi_pipe];
}

ISP_SNS_STATE_S *ov2775_get_ctx(VI_PIPE vi_pipe)
{
    return g_ov2775[vi_pipe];
}

const ov2775_video_mode_tbl g_ov2775_mode_tbl[OV2775_MODE_BUTT] = {
    {VMAX_1080P30_LINEAR,       FULL_LINES_MAX,          30, 0.51,
     1920, 1080, 0, WDR_MODE_NONE,       "OV2775_SENSOR_1080P_30FPS_LINEAR_MODE"},
    {VMAX_1080P30_LINEAR,       FULL_LINES_MAX,          30, 0.51,
     1920, 1080, 0, WDR_MODE_BUILT_IN,   "OV2775_SENSOR_1080P_30FPS_BUILT_IN_MODE"},
    {VMAX_1080P30_2TO1_WDR,      FULL_LINES_MAX,          30, 0.51,
     1920, 1080, 0, WDR_MODE_2To1_LINE,  "OV2775_SENSOR_1080P_30FPS_WDR_2TO1_MODE"},
    {VMAX_1080P30_LINEAR,       FULL_LINES_MAX,          30, 0.51,
     1920, 1080, 1, WDR_MODE_NONE,       "OV2775_SENSOR_1080P_30FPS_LINEAR_2L_MODE"},
    {VMAX_1080P30_2TO1_WDR,      FULL_LINES_MAX,          30, 0.51,
     1920, 1080, 1, WDR_MODE_2To1_LINE,  "OV2775_SENSOR_1080P_30FPS_WDR_2TO1_2L_MODE"},
    {VMAX_1080P30_2TO1_WDR,      FULL_LINES_MAX,          30, 0.51,
     1920, 1080, 2, WDR_MODE_2To1_LINE,  "OV2775_SENSOR_1080P_30FPS_HLCG_WDR_2TO1_2L_MODE"},
};

const ov2775_video_mode_tbl *ov2775_get_mode_tb(HI_U8 img_mode)
{
    return &g_ov2775_mode_tbl[img_mode];
}

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
static HI_U32 g_lines_per500ms[ISP_MAX_PIPE_NUM] = {0};
static HI_U32 g_init_exposure[ISP_MAX_PIPE_NUM]  = {0};
static HI_U16 g_init_wb_gain[ISP_MAX_PIPE_NUM][ISP_RGB_CHN_NUM] = {{0}};
static HI_U16 g_sample_rgain[ISP_MAX_PIPE_NUM] = {0};
static HI_U16 g_sample_bgain[ISP_MAX_PIPE_NUM] = {0};
static HI_U16 g_init_ccm[ISP_MAX_PIPE_NUM][CCM_MATRIX_SIZE] = {{0}};
static HI_BOOL g_quick_start_en[ISP_MAX_PIPE_NUM] = {HI_FALSE};

static HI_BOOL g_ae_routeex_valid[ISP_MAX_PIPE_NUM] = {0};
static ISP_AE_ROUTE_S g_init_ae_route[ISP_MAX_PIPE_NUM] = {{0}};
static ISP_AE_ROUTE_EX_S g_init_ae_routeex[ISP_MAX_PIPE_NUM] = {{0}};
static ISP_AE_ROUTE_S g_init_ae_routesf[ISP_MAX_PIPE_NUM] = {{0}};
static ISP_AE_ROUTE_EX_S g_init_ae_routesfex[ISP_MAX_PIPE_NUM] = {{0}};


static ISP_FSWDR_MODE_E g_fswdr_mode[ISP_MAX_PIPE_NUM] = {[0 ... ISP_MAX_PIPE_NUM - 1] = ISP_FSWDR_NORMAL_MODE};

static HI_U32 g_max_time_get_cnt[ISP_MAX_PIPE_NUM] = {0};

#define GROUP_HOLD_SEL        0x3464
#define GROUP_HOLD_LAUNCH     0x3467
#define EXPOSURE_ADDR_H       0x30B6
#define EXPOSURE_ADDR_L       0x30B7
#define SHORT_EXPOSURE_ADDR_H 0x30B8
#define SHORT_EXPOSURE_ADDR_L 0x30B9 /* exp_1 VS */
#define AGC_ADDR              0x30BB
#define DGC_ADDR_H            0x315A
#define DGC_ADDR_L            0x315B
#define LCG_ADDR_H            0x315C
#define LCG_ADDR_L            0x315D
#define VS_ADDR_H             0x315E
#define VS_ADDR_L             0x315F /* VS dgain */
#define VMAX_ADDR_H           0x30B2
#define VMAX_ADDR_L           0x30B3
#define HCG_LCG_SEL           0xB0C2

#define AGAIN_NODE_NUM     8
#define WDR_AGAIN_NODE_NUM 4
static HI_U32 g_again_table[AGAIN_NODE_NUM] = { 1024, 2048, 4096, 8192, 11264, 22528, 45056, 90112 };

static HI_VOID cmos_get_ae_comm_default(VI_PIPE vi_pipe,
    AE_SENSOR_DEFAULT_S *ae_sns_dft, ISP_SNS_STATE_S *sns_tate)
{
    ae_sns_dft->u32FullLinesStd = sns_tate->u32FLStd;
    ae_sns_dft->u32FlickerFreq = 0;
    ae_sns_dft->u32FullLinesMax = FULL_LINES_MAX;

    ae_sns_dft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    ae_sns_dft->stIntTimeAccu.f32Accuracy = 1;
    ae_sns_dft->stIntTimeAccu.f32Offset = 0;

    ae_sns_dft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
    ae_sns_dft->stAgainAccu.f32Accuracy = 6; /* f32Accuracy 6 */

    ae_sns_dft->stDgainAccu.enAccuType = AE_ACCURACY_TABLE;
    ae_sns_dft->stDgainAccu.f32Accuracy = 0.00390625; /* f32Accuracy 0.00390625 */

    ae_sns_dft->u32ISPDgainShift = 8; /* f32Accuracy 8 */
    ae_sns_dft->u32MinISPDgainTarget = 1 << ae_sns_dft->u32ISPDgainShift;
    ae_sns_dft->u32MaxISPDgainTarget = 6 << ae_sns_dft->u32ISPDgainShift; /* max 6 */

    if (g_lines_per500ms[vi_pipe] == 0) {
        ae_sns_dft->u32LinesPer500ms = sns_tate->u32FLStd * 30 / 2; /* 30fps / 2 */
    } else {
        ae_sns_dft->u32LinesPer500ms = g_lines_per500ms[vi_pipe];
    }

    if (g_init_exposure[vi_pipe] == 0) {
        ae_sns_dft->u32InitExposure = 360000; /* init 360000 */
    } else {
        ae_sns_dft->u32InitExposure = g_init_exposure[vi_pipe];
    }

    ae_sns_dft->enMaxIrisFNO = ISP_IRIS_F_NO_1_0;
    ae_sns_dft->enMinIrisFNO = ISP_IRIS_F_NO_32_0;

    ae_sns_dft->bAERouteExValid = HI_FALSE;
    ae_sns_dft->stAERouteAttr.u32TotalNum = 0;
    ae_sns_dft->stAERouteAttrEx.u32TotalNum = 0;
    ae_sns_dft->stQuickStart.bQuickStartEn = g_quick_start_en[vi_pipe];
    ae_sns_dft->stQuickStart.u8BlackFrameNum = 0;

    return;
}
static void cmos_get_ae_linear_default(VI_PIPE vi_pipe,
    AE_SENSOR_DEFAULT_S *ae_sns_dft, ISP_SNS_STATE_S *sns_state)
{
    ae_sns_dft->au8HistThresh[0] = 0xd;
    ae_sns_dft->au8HistThresh[1] = 0x28;
    ae_sns_dft->au8HistThresh[2] = 0x60; /* index 2 */
    ae_sns_dft->au8HistThresh[3] = 0x80; /* index 3 */
    ae_sns_dft->u8AeCompensation = 0x30;
    ae_sns_dft->u32MaxIntTime = sns_state->u32FLStd - 4; /* sub 4 */
    ae_sns_dft->u32MinIntTime = 2; /* min 2 */
    ae_sns_dft->u32MaxIntTimeTarget = 65535; /* max 65535 */
    ae_sns_dft->u32MinIntTimeTarget = 2; /* min 2 */
    ae_sns_dft->u32MaxAgain = 90112; /* max 90112 */
    ae_sns_dft->u32MinAgain = 2048; /* min 2048 */
    ae_sns_dft->u32MaxAgainTarget = ae_sns_dft->u32MaxAgain;
    ae_sns_dft->u32MinAgainTarget = ae_sns_dft->u32MinAgain;
    ae_sns_dft->u32MaxDgain = 65535; /* max 65535 */
    ae_sns_dft->u32MinDgain = 1536; /* min 1536 */
    ae_sns_dft->u32MaxDgainTarget = ae_sns_dft->u32MaxDgain;
    ae_sns_dft->u32MinDgainTarget = ae_sns_dft->u32MinDgain;
    ae_sns_dft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;

    ae_sns_dft->bAERouteExValid = g_ae_routeex_valid[vi_pipe];
    memcpy_s(&ae_sns_dft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), &g_init_ae_route[vi_pipe],
             sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&ae_sns_dft->stAERouteAttrEx, sizeof(ISP_AE_ROUTE_EX_S), &g_init_ae_routeex[vi_pipe],
             sizeof(ISP_AE_ROUTE_EX_S));
    return;
}
static void cmos_get_ae_built_in_wdr_default(VI_PIPE vi_pipe,
    AE_SENSOR_DEFAULT_S *ae_sns_dft, ISP_SNS_STATE_S *sns_state)
{
    ae_sns_dft->au8HistThresh[0] = 0xd;
    ae_sns_dft->au8HistThresh[1] = 0x28;
    ae_sns_dft->au8HistThresh[2] = 0x60; /* index 2 */
    ae_sns_dft->au8HistThresh[3] = 0x80; /* index 3 */
    ae_sns_dft->u8AeCompensation = 0x38;
    ae_sns_dft->u32MaxIntTime = sns_state->u32FLStd - 4; /* sub 4 */
    ae_sns_dft->u32MinIntTime = 2; /* min 2 */
    ae_sns_dft->u32MaxIntTimeTarget = 65535; /* max 65535 */
    ae_sns_dft->u32MinIntTimeTarget = 2; /* min 2 */
    ae_sns_dft->u32MaxAgain = 8192; /* max 8192 */
    ae_sns_dft->u32MinAgain = 2048; /* min 2048 */
    ae_sns_dft->u32MaxAgainTarget = ae_sns_dft->u32MaxAgain;
    ae_sns_dft->u32MinAgainTarget = ae_sns_dft->u32MinAgain;
    ae_sns_dft->u32MaxDgain = 65535; /* max 65535 */
    ae_sns_dft->u32MinDgain = 1536; /* min 1536 */
    ae_sns_dft->u32MaxDgainTarget = ae_sns_dft->u32MaxDgain;
    ae_sns_dft->u32MinDgainTarget = ae_sns_dft->u32MinDgain;

    ae_sns_dft->u16ManRatioEnable = HI_TRUE;
    ae_sns_dft->au32Ratio[0] = 16 * 64; /* ratio 16x, accuracy 64 */
    ae_sns_dft->enAeExpMode = AE_EXP_LOWLIGHT_PRIOR;

    ae_sns_dft->bAERouteExValid = g_ae_routeex_valid[vi_pipe];
    memcpy_s(&ae_sns_dft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), &g_init_ae_route[vi_pipe],
             sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&ae_sns_dft->stAERouteAttrEx, sizeof(ISP_AE_ROUTE_EX_S), &g_init_ae_routeex[vi_pipe],
             sizeof(ISP_AE_ROUTE_EX_S));

    return;
}
static void cmos_get_ae_2to1_line_wdr_default(VI_PIPE vi_pipe,
    AE_SENSOR_DEFAULT_S *ae_sns_dft, ISP_SNS_STATE_S *sns_state)
{
    ae_sns_dft->au8HistThresh[0] = 0xC;
    ae_sns_dft->au8HistThresh[1] = 0x18;
    ae_sns_dft->au8HistThresh[2] = 0x60; /* index 2 */
    ae_sns_dft->au8HistThresh[3] = 0x80; /* index 3 */
    ae_sns_dft->u8AeCompensation = 32; /* u8AeCompensation 32 */
    ae_sns_dft->u32MaxIntTime = sns_state->u32FLStd - 2; /* sub 2 */
    ae_sns_dft->u32MinIntTime = 3; /* min 3 */
    ae_sns_dft->u32MaxIntTimeTarget = 65535; /* max 65535 */
    ae_sns_dft->u32MinIntTimeTarget = 3; /* min 3 */
    ae_sns_dft->u32MaxAgain = 90112; /* max 90112 */
    ae_sns_dft->u32MinAgain = 2048; /* min 2048 */
    ae_sns_dft->u32MaxAgainTarget = 90112; /* max 90112 */
    ae_sns_dft->u32MinAgainTarget = ae_sns_dft->u32MinAgain;
    ae_sns_dft->u32MaxDgain = 65535; /* max 65535 */
    ae_sns_dft->u32MinDgain = 1536; /* min 1536 */
    ae_sns_dft->u32MaxDgainTarget = 65535; /* min 65535 */
    ae_sns_dft->u32MinDgainTarget = ae_sns_dft->u32MinDgain;

    ae_sns_dft->u32LFMaxShortTime = 300; /* LFMaxShortTime 300 */
    ae_sns_dft->u32MaxIntTimeStep = 30; /* MaxIntTimeStep 30 */
    ae_sns_dft->u32LFMinExposure = 880000; /* LFMinExposure 880000 */

    ae_sns_dft->u32InitExposure = 237214; /* init exposure 237214 */
    ae_sns_dft->u32ISPDgainShift = 8; /* ispdgain shift 8 bit */
    ae_sns_dft->u32MinISPDgainTarget = 1 << ae_sns_dft->u32ISPDgainShift; /* min ispdgain 1x */
    ae_sns_dft->u32MaxISPDgainTarget = 2 << ae_sns_dft->u32ISPDgainShift; /* max ispdgain 2x */
    ae_sns_dft->stIntTimeAccu.f32Offset = -0.4375; /* offset -0.4375 */
    ae_sns_dft->u32ExpRatioMax = 680; /* ratio max 680 */
    ae_sns_dft->u32ExpRatioMin = 256; /* ratio min 256 */
    ae_sns_dft->enAeExpMode = AE_EXP_LOWLIGHT_PRIOR;

    ae_sns_dft->bAERouteExValid = g_ae_routeex_valid[vi_pipe];
    memcpy_s(&ae_sns_dft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), &g_init_ae_route[vi_pipe],
             sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&ae_sns_dft->stAERouteAttrEx, sizeof(ISP_AE_ROUTE_EX_S), &g_init_ae_routeex[vi_pipe],
             sizeof(ISP_AE_ROUTE_EX_S));
    memcpy_s(&ae_sns_dft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), &g_init_ae_routesf[vi_pipe],
             sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&ae_sns_dft->stAERouteAttrEx, sizeof(ISP_AE_ROUTE_EX_S), &g_init_ae_routesfex[vi_pipe],
             sizeof(ISP_AE_ROUTE_EX_S));
#ifdef GAIN_SEP /* if gain seperator set need define GAIN_SEP */
    ae_sns_dft->bDiffGainSupport = HI_TRUE;
#endif

    return;
}

static void cmos_get_ae_2to1_hlcg_line_wdr_default(VI_PIPE vi_pipe,
    AE_SENSOR_DEFAULT_S *ae_sns_dft, ISP_SNS_STATE_S *sns_state)
{
    ae_sns_dft->au8HistThresh[0] = 0xC;
    ae_sns_dft->au8HistThresh[1] = 0x18;
    ae_sns_dft->au8HistThresh[2] = 0x60; /* index 2 */
    ae_sns_dft->au8HistThresh[3] = 0x80; /* index 3 */
    ae_sns_dft->u8AeCompensation = 32; /* u8AeCompensation 32 */
    ae_sns_dft->u32MaxIntTime = sns_state->u32FLStd - 2; /* sub 2 */
    ae_sns_dft->u32MinIntTime = 2; /* min 2 */
    ae_sns_dft->u32MaxIntTimeTarget = 65535; /* max 65535 */
    ae_sns_dft->u32MinIntTimeTarget = 2; /* min 2 */
    ae_sns_dft->u32MaxAgain = 8192; /* max 8192 */
    ae_sns_dft->u32MinAgain = 1024; /* min 1024 */
    ae_sns_dft->u32MaxAgainTarget = 8192; /* max 8192 */
    ae_sns_dft->u32MinAgainTarget = ae_sns_dft->u32MinAgain;
    ae_sns_dft->u32MaxDgain = 65535; /* max 65535 */
    ae_sns_dft->u32MinDgain = 1536; /* min 1536 */
    ae_sns_dft->u32MaxDgainTarget = 65535; /* min 65535 */
    ae_sns_dft->u32MinDgainTarget = ae_sns_dft->u32MinDgain;

    ae_sns_dft->u32LFMaxShortTime = 300; /* LFMaxShortTime 300 */
    ae_sns_dft->u32MaxIntTimeStep = 30; /* MaxIntTimeStep 30 */
    ae_sns_dft->u32LFMinExposure = 880000; /* LFMinExposure 880000 */

    ae_sns_dft->u32InitExposure = 237214; /* init exposure 237214 */
    ae_sns_dft->u32ISPDgainShift = 8; /* ispdgain shift 8 bit */
    ae_sns_dft->u32MinISPDgainTarget = 1 << ae_sns_dft->u32ISPDgainShift; /* min ispdgain 1x */
    ae_sns_dft->u32MaxISPDgainTarget = 16 << ae_sns_dft->u32ISPDgainShift; /* max ispdgain 16x */
    ae_sns_dft->stIntTimeAccu.f32Offset = 0; /* offset 0 */
    ae_sns_dft->u32ExpRatioMax = 1024; /* ratio max 1024 */
    ae_sns_dft->u32ExpRatioMin = 704; /* ratio min 704 */
    ae_sns_dft->enAeExpMode = AE_EXP_LOWLIGHT_PRIOR;

    ae_sns_dft->bAERouteExValid = g_ae_routeex_valid[vi_pipe];
    memcpy_s(&ae_sns_dft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), &g_init_ae_route[vi_pipe],
             sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&ae_sns_dft->stAERouteAttrEx, sizeof(ISP_AE_ROUTE_EX_S), &g_init_ae_routeex[vi_pipe],
             sizeof(ISP_AE_ROUTE_EX_S));
    memcpy_s(&ae_sns_dft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), &g_init_ae_routesf[vi_pipe],
             sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&ae_sns_dft->stAERouteAttrEx, sizeof(ISP_AE_ROUTE_EX_S), &g_init_ae_routesfex[vi_pipe],
             sizeof(ISP_AE_ROUTE_EX_S));
    ae_sns_dft->enPriorFrame = LONG_FRAME;
    ae_sns_dft->bDiffGainSupport = HI_TRUE;
    ae_sns_dft->bAEGainSepCfg = HI_TRUE;
    ae_sns_dft->bLhcgSupport = HI_TRUE;
    ae_sns_dft->u32SnsLhcgExpRatio = 704; /* sensor reference ratio 704 */

    return;
}

static HI_S32 cmos_get_ae_default(VI_PIPE vi_pipe, AE_SENSOR_DEFAULT_S *ae_sns_dft)
{
    ISP_SNS_STATE_S *sns_state = HI_NULL;

    CMOS_CHECK_POINTER(ae_sns_dft);
    ov2775_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER(sns_state);

    memset_s(&ae_sns_dft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), 0, sizeof(ISP_AE_ROUTE_S));

    cmos_get_ae_comm_default(vi_pipe, ae_sns_dft, sns_state);

    switch (sns_state->enWDRMode) {
        default:
        case WDR_MODE_NONE: /* linear mode */

            cmos_get_ae_linear_default(vi_pipe, ae_sns_dft, sns_state);
            break;

        case WDR_MODE_BUILT_IN:

            cmos_get_ae_built_in_wdr_default(vi_pipe, ae_sns_dft, sns_state);
            break;

        case WDR_MODE_2To1_LINE:
            if (sns_state->u8ImgMode == OV2775_SENSOR_1080P_30FPS_HLCG_WDR_2TO1_MODE) {
                cmos_get_ae_2to1_hlcg_line_wdr_default(vi_pipe, ae_sns_dft, sns_state);
            } else {
                cmos_get_ae_2to1_line_wdr_default(vi_pipe, ae_sns_dft, sns_state);
            }
            break;
    }
    return HI_SUCCESS;
}

/* AWB default parameter and function */
#define CALIBRATE_STATIC_TEMP       5040
#define CALIBRATE_STATIC_WB_R_GAIN  557
#define CALIBRATE_STATIC_WB_GR_GAIN 256
#define CALIBRATE_STATIC_WB_GB_GAIN 256
#define CALIBRATE_STATIC_WB_B_GAIN  419

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1            (-44)
#define CALIBRATE_AWB_P2            300
#define CALIBRATE_AWB_Q1            0
#define CALIBRATE_AWB_A1            164244
#define CALIBRATE_AWB_B1            128
#define CALIBRATE_AWB_C1            (-114766)

static HI_S32 cmos_get_awb_default(VI_PIPE vi_pipe, AWB_SENSOR_DEFAULT_S *awb_sns_dft)
{
    ISP_SNS_STATE_S *sns_state = HI_NULL;
    HI_S32 i;

    CMOS_CHECK_POINTER(awb_sns_dft);
    ov2775_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER(sns_state);

    memset_s(awb_sns_dft, sizeof(AWB_SENSOR_DEFAULT_S), 0, sizeof(AWB_SENSOR_DEFAULT_S));

    awb_sns_dft->u16WbRefTemp = CALIBRATE_STATIC_TEMP;

    awb_sns_dft->au16GainOffset[0] = CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->au16GainOffset[1] = CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->au16GainOffset[2] = CALIBRATE_STATIC_WB_GB_GAIN; /* index 2 */
    awb_sns_dft->au16GainOffset[3] = CALIBRATE_STATIC_WB_B_GAIN; /* index 3 */

    awb_sns_dft->as32WbPara[0] = CALIBRATE_AWB_P1;
    awb_sns_dft->as32WbPara[1] = CALIBRATE_AWB_P2;
    awb_sns_dft->as32WbPara[2] = CALIBRATE_AWB_Q1; /* index 2 */
    awb_sns_dft->as32WbPara[3] = CALIBRATE_AWB_A1; /* index 3 */
    awb_sns_dft->as32WbPara[4] = CALIBRATE_AWB_B1; /* index 4 */
    awb_sns_dft->as32WbPara[5] = CALIBRATE_AWB_C1; /* index 5 */

    switch (sns_state->enWDRMode) {
        default:
        case WDR_MODE_NONE:
            memcpy_s(&awb_sns_dft->stCcm, sizeof(AWB_CCM_S), &g_stAwbCcm, sizeof(AWB_CCM_S));
            memcpy_s(&awb_sns_dft->stAgcTbl, sizeof(AWB_AGC_TABLE_S), &g_stAwbAgcTable, sizeof(AWB_AGC_TABLE_S));
            break;
        case WDR_MODE_2To1_LINE:
            memcpy_s(&awb_sns_dft->stCcm, sizeof(AWB_CCM_S), &g_stAwbCcmFsWdr, sizeof(AWB_CCM_S));
            memcpy_s(&awb_sns_dft->stAgcTbl, sizeof(AWB_AGC_TABLE_S), &g_stAwbAgcTableFSWDR, sizeof(AWB_AGC_TABLE_S));
            break;
        case WDR_MODE_BUILT_IN:
            memcpy_s(&awb_sns_dft->stCcm, sizeof(AWB_CCM_S), &g_stAwbCcmFsWdr, sizeof(AWB_CCM_S));
            memcpy_s(&awb_sns_dft->stAgcTbl, sizeof(AWB_AGC_TABLE_S), &g_stAwbAgcTableFSWDR, sizeof(AWB_AGC_TABLE_S));
            break;
    }

    awb_sns_dft->u16InitRgain = g_init_wb_gain[vi_pipe][0]; /* 0: Rgain */
    awb_sns_dft->u16InitGgain = g_init_wb_gain[vi_pipe][1]; /* 1: Ggain */
    awb_sns_dft->u16InitBgain = g_init_wb_gain[vi_pipe][2]; /* 2: Bgain */
    awb_sns_dft->u16SampleRgain = g_sample_rgain[vi_pipe];
    awb_sns_dft->u16SampleBgain = g_sample_bgain[vi_pipe];
    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        awb_sns_dft->au16InitCCM[i] = g_init_ccm[vi_pipe][i];
    }

    return HI_SUCCESS;
}

static HI_S32 cmos_init_awb_exp_function(AWB_SENSOR_EXP_FUNC_S *exp_funcs)
{
    CMOS_CHECK_POINTER(exp_funcs);

    memset_s(exp_funcs, sizeof(AWB_SENSOR_EXP_FUNC_S), 0, sizeof(AWB_SENSOR_EXP_FUNC_S));

    exp_funcs->pfn_cmos_get_awb_default = cmos_get_awb_default;

    return HI_SUCCESS;
}

static ISP_CMOS_DNG_COLORPARAM_S g_stDngColorParam = {{ 378, 256, 430 }, { 439, 256, 439 }};

static HI_VOID cmos_get_isp_dng_default(ISP_SNS_STATE_S *sns_state, ISP_CMOS_DEFAULT_S *def)
{
    memcpy_s(&def->stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S), &g_stDngColorParam,
             sizeof(ISP_CMOS_DNG_COLORPARAM_S));

    def->stSensorMode.stDngRawFormat.u8BitsPerSample = 12; /* 12bit */
    def->stSensorMode.stDngRawFormat.u32WhiteLevel = 4095; /* max 4095 */

    def->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleH.u32Denominator = 1;
    def->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleH.u32Numerator = 1;
    def->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleV.u32Denominator = 1;
    def->stSensorMode.stDngRawFormat.stDefaultScale.stDefaultScaleV.u32Numerator = 1;
    def->stSensorMode.stDngRawFormat.stCfaRepeatPatternDim.u16RepeatPatternDimRows = 2; /* pattern 2 */
    def->stSensorMode.stDngRawFormat.stCfaRepeatPatternDim.u16RepeatPatternDimCols = 2; /* pattern 2 */
    def->stSensorMode.stDngRawFormat.stBlcRepeatDim.u16BlcRepeatRows = 2; /* pattern 2 */
    def->stSensorMode.stDngRawFormat.stBlcRepeatDim.u16BlcRepeatCols = 2; /* pattern 2 */
    def->stSensorMode.stDngRawFormat.enCfaLayout = CFALAYOUT_TYPE_RECTANGULAR;
    def->stSensorMode.stDngRawFormat.au8CfaPlaneColor[0] = 0;
    def->stSensorMode.stDngRawFormat.au8CfaPlaneColor[1] = 1;
    def->stSensorMode.stDngRawFormat.au8CfaPlaneColor[2] = 2; /* index 2, CfaPlaneColor 2 */
    def->stSensorMode.stDngRawFormat.au8CfaPattern[0] = 0;
    def->stSensorMode.stDngRawFormat.au8CfaPattern[1] = 1;
    def->stSensorMode.stDngRawFormat.au8CfaPattern[2] = 1; /* index 2, CfaPattern 1 */
    def->stSensorMode.stDngRawFormat.au8CfaPattern[3] = 2; /* index 3, CfaPattern 2 */
    def->stSensorMode.bValidDngRawFormat = HI_TRUE;
    return;
}

static void cmos_get_isp_linear_default(ISP_CMOS_DEFAULT_S *def)
{
    def->unKey.bit1Demosaic       = 1;
    def->pstDemosaic              = &g_stIspDemosaic;
    def->unKey.bit1Sharpen        = 1;
    def->pstSharpen               = &g_stIspYuvSharpen;
    def->unKey.bit1Drc            = 1;
    def->pstDrc                   = &g_stIspDRC;
    def->unKey.bit1BayerNr        = 1;
    def->pstBayerNr               = &g_stIspBayerNr;
    def->unKey.bit1Gamma          = 1;
    def->pstGamma                 = &g_stIspGamma;
    def->unKey.bit1Detail         = 1;
    def->pstDetail                = &g_stIspDetail;
#ifdef CONFIG_HI_ISP_CR_SUPPORT
    def->unKey.bit1Ge             = 1;
    def->pstGe                    = &g_stIspGe;
#endif
    def->unKey.bit1AntiFalseColor = 1;
    def->pstAntiFalseColor = &g_stIspAntiFalseColor;
    def->unKey.bit1Ldci = 1;
    def->pstLdci = &g_stIspLdci;
    def->unKey.bit1Dehaze = 1;
    def->pstDehaze = &g_stIspDehaze;
    def->unKey.bit1Lcac = 1;
    def->pstLcac = &g_stIspLCac;
    memcpy_s(&def->stNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S), &g_stIspNoiseCalibration,
             sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
    return;
}

static void cmos_get_isp_built_in_wdr_default(ISP_CMOS_DEFAULT_S *def)
{
    def->unKey.bit1Expander       = 1;
    def->pstExpander              = &g_stCmosExpander;
    def->unKey.bit1Demosaic       = 1;
    def->pstDemosaic              = &g_stIspDemosaicWdr;
    def->unKey.bit1Sharpen        = 1;
    def->pstSharpen               = &g_stIspYuvSharpenWdr;
    def->unKey.bit1Drc            = 1;
    def->pstDrc                   = &g_stIspDRCWDR;
    def->unKey.bit1Gamma          = 1;
    def->pstGamma                 = &g_stIspGammaFSWDR;
#ifdef CONFIG_HI_ISP_PREGAMMA_SUPPORT
    def->unKey.bit1PreGamma       = 0;
    def->pstPreGamma              = &g_stPreGamma;
#endif
    def->unKey.bit1BayerNr        = 1;
    def->pstBayerNr               = &g_stIspBayerNrWdr2To1;
    def->unKey.bit1Detail         = 1;
    def->pstDetail                = &g_stIspDetailWdr2To1;
#ifdef CONFIG_HI_ISP_CR_SUPPORT
    def->unKey.bit1Ge             = 1;
    def->pstGe                    = &g_stIspWdrGe;
#endif
    def->unKey.bit1AntiFalseColor = 1;
    def->pstAntiFalseColor        = &g_stIspWdrAntiFalseColor;
    def->unKey.bit1Ldci           = 1;
    def->pstLdci                  = &g_stIspWdrLdci;
    def->unKey.bit1Dehaze         = 1;
    def->pstDehaze                = &g_stIspDehazeWDR;
    def->unKey.bit1Lcac           = 1;
    def->pstLcac                  = &g_stIspLCacWdr;
    def->stWdrSwitchAttr.au32ExpRatio[0] = 0x400;
    memcpy_s(&def->stNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S),
        &g_stIspNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
    return;
}

static void cmos_get_isp_2to1_line_wdr_default(ISP_CMOS_DEFAULT_S *def)
{
    def->unKey.bit1Demosaic       = 1;
    def->pstDemosaic              = &g_stIspDemosaicWdr;
    def->unKey.bit1Sharpen        = 1;
    def->pstSharpen               = &g_stIspYuvSharpenWdr;
    def->unKey.bit1Drc            = 1;
    def->pstDrc                   = &g_stIspDRCWDR;
    def->unKey.bit1Gamma          = 1;
    def->pstGamma                 = &g_stIspGammaFSWDR;
#ifdef CONFIG_HI_ISP_PREGAMMA_SUPPORT
    def->unKey.bit1PreGamma       = 0;
    def->pstPreGamma              = &g_stPreGamma;
#endif
    def->unKey.bit1BayerNr        = 1;
    def->pstBayerNr               = &g_stIspBayerNrWdr2To1;
    def->unKey.bit1Detail         = 1;
    def->pstDetail                = &g_stIspDetailWdr2To1;
#ifdef CONFIG_HI_ISP_CR_SUPPORT
    def->unKey.bit1Ge             = 1;
    def->pstGe                    = &g_stIspWdrGe;
#endif
    def->unKey.bit1AntiFalseColor = 1;
    def->pstAntiFalseColor        = &g_stIspWdrAntiFalseColor;
    def->unKey.bit1Ldci           = 1;
    def->pstLdci                  = &g_stIspWdrLdci;
    def->unKey.bit1Dehaze         = 1;
    def->pstDehaze                = &g_stIspDehazeWDR;
    def->unKey.bit1Lcac           = 1;
    def->pstLcac                  = &g_stIspLCacWdr;
    def->stWdrSwitchAttr.au32ExpRatio[0] = 0x400;
    memcpy_s(&def->stNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S),
        &g_stIspNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
    return;
}

static void cmos_get_isp_2to1_line_wdr_hlcg_default(ISP_CMOS_DEFAULT_S *def)
{
    def->unKey.bit1Demosaic       = 1;
    def->pstDemosaic              = &g_stIspDemosaicWdr;
    def->unKey.bit1Sharpen        = 1;
    def->pstSharpen               = &g_stIspYuvSharpenWdr;
    def->unKey.bit1Drc            = 1;
    def->pstDrc                   = &g_isp_drc_wdr_hlcg;
    def->unKey.bit1Gamma          = 1;
    def->pstGamma                 = &g_stIspGammaFSWDR;
#ifdef CONFIG_HI_ISP_PREGAMMA_SUPPORT
    def->unKey.bit1PreGamma       = 0;
    def->pstPreGamma              = &g_stPreGamma;
#endif
    def->unKey.bit1BayerNr        = 1;
    def->pstBayerNr               = &g_isp_bayer_nr_wdr2to1_hlcg;
    def->unKey.bit1Detail         = 1;
    def->pstDetail                = &g_stIspDetailWdr2To1;
#ifdef CONFIG_HI_ISP_CR_SUPPORT
    def->unKey.bit1Ge             = 1;
    def->pstGe                    = &g_stIspWdrGe;
#endif
    def->unKey.bit1AntiFalseColor = 1;
    def->pstAntiFalseColor        = &g_stIspWdrAntiFalseColor;
    def->unKey.bit1Ldci           = 1;
    def->pstLdci                  = &g_stIspWdrLdci;
    def->unKey.bit1Dehaze         = 1;
    def->pstDehaze                = &g_stIspDehazeWDR;
    def->unKey.bit1Lcac           = 1;
    def->pstLcac                  = &g_stIspLCacWdr;
    def->stWdrSwitchAttr.au32ExpRatio[0] = 0x400;
    memcpy_s(&def->stNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S),
        &g_isp_noise_calibration_hlcg, sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
    return;
}

static HI_S32 cmos_get_isp_default(VI_PIPE vi_pipe, ISP_CMOS_DEFAULT_S *def)
{
    ISP_SNS_STATE_S *sns_state = HI_NULL;

    CMOS_CHECK_POINTER(def);
    ov2775_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER(sns_state);

    memset_s(def, sizeof(ISP_CMOS_DEFAULT_S), 0, sizeof(ISP_CMOS_DEFAULT_S));
#ifdef CONFIG_HI_ISP_CA_SUPPORT
    def->unKey.bit1Ca       = 1;
    def->pstCa              = &g_stIspCA;
#endif
    def->unKey.bit1Clut     = 1;
    def->pstClut            = &g_stIspCLUT;

    def->unKey.bit1Dpc      = 1;
    def->pstDpc             = &g_stCmosDpc;

    def->unKey.bit1Wdr      = 1;
    def->pstWdr             = &g_stIspWDR;
#ifdef CONFIG_HI_ISP_HLC_SUPPORT
    def->unKey.bit1Hlc      = 0;
    def->pstHlc             = &g_stIspHlc;
#endif
    def->unKey.bit1Lsc      = 0;
    def->pstLsc             = &g_stCmosLsc;
#ifdef CONFIG_HI_ISP_EDGEMARK_SUPPORT
    def->unKey.bit1EdgeMark = 0;
    def->pstEdgeMark        = &g_stIspEdgeMark;
#endif
    switch (sns_state->enWDRMode) {
        default:
        case WDR_MODE_NONE:

            cmos_get_isp_linear_default(def);

            break;

        case WDR_MODE_BUILT_IN:

            cmos_get_isp_built_in_wdr_default(def);

            break;

        case WDR_MODE_2To1_LINE:
            if (sns_state->u8ImgMode == OV2775_SENSOR_1080P_30FPS_HLCG_WDR_2TO1_MODE) {
                cmos_get_isp_2to1_line_wdr_hlcg_default(def);
            } else {
                cmos_get_isp_2to1_line_wdr_default(def);
            }

            break;
    }

    def->stSensorMode.u32SensorID = OV2775_ID;
    def->stSensorMode.u8SensorMode = sns_state->u8ImgMode;

    cmos_get_isp_dng_default(sns_state, def);

    return HI_SUCCESS;
}

/* the function of sensor set fps */
static HI_VOID cmos_fps_set(VI_PIPE vi_pipe, HI_FLOAT fps, AE_SENSOR_DEFAULT_S *ae_sns_dft)
{
    HI_U32 lines, lines_max, vmax;
    HI_FLOAT max_fps, min_fps;
    ISP_SNS_STATE_S *sns_state = HI_NULL;

    CMOS_CHECK_POINTER_VOID(ae_sns_dft);
    ov2775_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER_VOID(sns_state);

    lines = g_ov2775_mode_tbl[sns_state->u8ImgMode].verti_lines;
    lines_max = g_ov2775_mode_tbl[sns_state->u8ImgMode].max_verti_lines;
    max_fps = g_ov2775_mode_tbl[sns_state->u8ImgMode].max_fps;
    min_fps = g_ov2775_mode_tbl[sns_state->u8ImgMode].min_fps;

    if ((fps > max_fps) || (fps < min_fps)) {
        SNS_ERR_TRACE("Not support Fps: %f\n", fps);
        return;
    }

    vmax = lines * max_fps / SNS_DIV_0_TO_1_FLOAT(fps);
    vmax = (vmax > lines_max) ? lines_max : vmax;

    sns_state->u32FLStd = vmax;
    sns_state->astRegsInfo[0].astI2cData[5].u32Data = ((sns_state->u32FLStd & 0xFF00) >> 8); /* index5,shift8 */
    sns_state->astRegsInfo[0].astI2cData[6].u32Data = (sns_state->u32FLStd & 0xFF); /* index 6 */
    ae_sns_dft->u32LinesPer500ms = lines * max_fps / 2; /* div 2 */
    ae_sns_dft->f32Fps = fps;
    ae_sns_dft->u32MaxIntTime = sns_state->u32FLStd - 4; /* sub 4 */
    ae_sns_dft->u32FullLinesStd = sns_state->u32FLStd;
    sns_state->au32FL[0]  = sns_state->u32FLStd;
    ae_sns_dft->u32FullLines = sns_state->au32FL[0];
    return;
}
static HI_VOID cmos_slow_framerate_set(VI_PIPE vi_pipe, HI_U32 full_lines,
                                       AE_SENSOR_DEFAULT_S *ae_sns_dft)
{
    ISP_SNS_STATE_S *sns_state = HI_NULL;

    CMOS_CHECK_POINTER_VOID(ae_sns_dft);
    ov2775_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER_VOID(sns_state);

    full_lines = (full_lines > FULL_LINES_MAX) ? FULL_LINES_MAX : full_lines;
    sns_state->au32FL[0]  = full_lines;
    ae_sns_dft->u32FullLines = sns_state->au32FL[0];
    sns_state->astRegsInfo[0].astI2cData[5].u32Data = ((full_lines & 0xFF00) >> 8); /* index5,shift8 */
    sns_state->astRegsInfo[0].astI2cData[6].u32Data = (full_lines & 0xFF); /* index 6 */
    ae_sns_dft->u32MaxIntTime = sns_state->au32FL[0]  - 4; /* sub 4 */
    return;
}
/* while isp notify ae to update sensor regs, ae call these funcs. */
static HI_VOID cmos_inttime_update(VI_PIPE vi_pipe, HI_U32 int_time)
{
    static HI_BOOL first[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = HI_TRUE};
    ISP_SNS_STATE_S *sns_state = HI_NULL;

    ov2775_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER_VOID(sns_state);

    if ((sns_state->enWDRMode == WDR_MODE_2To1_FRAME_FULL_RATE) || (sns_state->enWDRMode == WDR_MODE_2To1_FRAME)) {
        if (first[vi_pipe]) { /* short exposure */
            sns_state->astRegsInfo[0].astI2cData[0].u32Data = ((int_time & 0xFF00) >> 8); /* index0 shift8 */
            sns_state->astRegsInfo[0].astI2cData[1].u32Data = (int_time & 0xFF); /* index1 */
            first[vi_pipe] = HI_FALSE;
        } else { /* long exposure */
            first[vi_pipe] = HI_TRUE;
        }
    } else if (sns_state->enWDRMode == WDR_MODE_2To1_LINE) {
        if ((first[vi_pipe])
            && (sns_state->u8ImgMode != OV2775_SENSOR_1080P_30FPS_HLCG_WDR_2TO1_MODE)) { /* short exposure */
            sns_state->au32WDRIntTime[0] = int_time;
            sns_state->astRegsInfo[0].astI2cData[11].u32Data = ((int_time & 0xFF00) >> 8); /* index 11 shift8 */
            sns_state->astRegsInfo[0].astI2cData[12].u32Data = (int_time & 0xFF); /* index12 */
            first[vi_pipe] = HI_FALSE;
        } else { /* long exposure */
            sns_state->au32WDRIntTime[1] = int_time;
            sns_state->astRegsInfo[0].astI2cData[0].u32Data = ((int_time & 0xFF00) >> 8); /* index0 shift8 */
            sns_state->astRegsInfo[0].astI2cData[1].u32Data = (int_time & 0xFF); /* index1 */
            first[vi_pipe] = HI_TRUE;
        }
    } else {
        sns_state->astRegsInfo[0].astI2cData[0].u32Data = ((int_time & 0xFF00) >> 8); /* index0 shift8 */
        sns_state->astRegsInfo[0].astI2cData[1].u32Data = (int_time & 0xFF); /* index1 */
        first[vi_pipe] = HI_TRUE;
    }
    return;
}
static HI_VOID cmos_again_calc_table(VI_PIPE vi_pipe, HI_U32 *again_lin, HI_U32 *again_db)
{
    int i;
    HI_U8 index_num_max;
    ISP_SNS_STATE_S *sns_state = HI_NULL;

    ov2775_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER_VOID(sns_state);

    CMOS_CHECK_POINTER_VOID(again_lin);
    CMOS_CHECK_POINTER_VOID(again_db);

    if (sns_state->enWDRMode == WDR_MODE_NONE) {
        index_num_max = AGAIN_NODE_NUM;
    } else {
        if ((sns_state->enWDRMode == WDR_MODE_2To1_LINE) &&
            (sns_state->u8ImgMode == OV2775_SENSOR_1080P_30FPS_WDR_2TO1_2L_MODE)) {
            index_num_max = WDR_AGAIN_NODE_NUM + 4; /* use hcg will increase 4 index */
        } else {
            index_num_max = WDR_AGAIN_NODE_NUM;
        }
    }

    if (*again_lin >= g_again_table[index_num_max - 1]) {
        *again_lin = g_again_table[index_num_max - 1];
        *again_db = index_num_max - 1;
        return;
    }

    for (i = 1; i < index_num_max; i++) {
        if (*again_lin < g_again_table[i]) {
            *again_lin = g_again_table[i - 1];
            *again_db = i - 1;
            break;
        }
    }

    return;
}

static HI_VOID cmos_dgain_calc_table(VI_PIPE vi_pipe, HI_U32 *dgain_lin, HI_U32 *dgain_db)
{
    CMOS_CHECK_POINTER_VOID(dgain_lin);
    CMOS_CHECK_POINTER_VOID(dgain_db);

    *dgain_db = *dgain_lin >> 2;  /* sensor dgain is 8bit precision, need right shift 2 bit */
    *dgain_lin = *dgain_db << 2;  /* sensor dgain is 8bit precision, need right shift 2 bit */

    return;
}

static HI_U32 g_again_reg[AGAIN_NODE_NUM] = { 0x0, 0x1, 0x2, 0x3, 0x40, 0x41, 0x42, 0x43 };

static HI_VOID cmos_linear_gains_update(HI_U32 again, HI_U32 dgain, ISP_SNS_STATE_S *sns_state)
{
    again = g_again_reg[again];
    sns_state->astRegsInfo[0].astI2cData[2].u32Data = again & 0xFF; /* index 2 */
    sns_state->astRegsInfo[0].astI2cData[3].u32Data = ((dgain & 0xFF00) >> 8); /* index 3, shift 8 */
    sns_state->astRegsInfo[0].astI2cData[4].u32Data = (dgain & 0xFF); /* index 4 */
    return;
}
static HI_VOID cmos_builtin_wdr_gains_update(HI_U32 again, HI_U32 dgain, ISP_SNS_STATE_S *sns_state)
{
    again = (again > 3) ? 3 : again; /* max 3 */
    sns_state->astRegsInfo[0].astI2cData[2].u32Data = (again << 2) | again; /* index 2, shift 2 */
    sns_state->astRegsInfo[0].astI2cData[3].u32Data = ((dgain & 0xFF00) >> 8); /* index 3, shift 8 */
    sns_state->astRegsInfo[0].astI2cData[4].u32Data = (dgain & 0xFF); /* index 4 */
    sns_state->astRegsInfo[0].astI2cData[7].u32Data = ((dgain & 0xFF00) >> 8); /* index 7, shift 8 */
    sns_state->astRegsInfo[0].astI2cData[8].u32Data = (dgain & 0xFF); /* index 8 */
    return;
}

static HI_U32 g_again_reg_wdr[AGAIN_NODE_NUM] = { 0x0, 0x1, 0x2, 0x3, 0x0, 0x1, 0x2, 0x3 };

static HI_VOID cmos_wdr_gains_update(VI_PIPE vi_pipe, HI_U32 again,
    HI_U32 dgain, ISP_SNS_STATE_S *sns_state)
{
#ifdef GAIN_SEP
    static HI_BOOL first[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = HI_TRUE};
    static HI_U8 first_frame_again[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = 0};

    if (first[vi_pipe]) { // short frame
        first_frame_again[vi_pipe] = (again > 3) ? 3 : again; /* max 3 */
        sns_state->astRegsInfo[0].astI2cData[9].u32Data = ((dgain & 0xFF00) >> 8); /* index 9, shift 8 */
        sns_state->astRegsInfo[0].astI2cData[10].u32Data = (dgain & 0xFF); /* index 10 */
        first[vi_pipe] = HI_FALSE;
    } else { // long frame
        again = (again > 3) ? 3 : again; /* max 3 */
        sns_state->astRegsInfo[0].astI2cData[2].u32Data = /* index 2 */
        (first_frame_again[vi_pipe] << 4) | (again << 2) | again; /* shift 4, shift 2 */
        sns_state->astRegsInfo[0].astI2cData[3].u32Data = ((dgain & 0xFF00) >> 8); /* index 3, shift 8 */
        sns_state->astRegsInfo[0].astI2cData[4].u32Data = (dgain & 0xFF); /* index 4 */
        sns_state->astRegsInfo[0].astI2cData[7].u32Data = ((dgain & 0xFF00) >> 8); /* index 7, shift 8 */
        sns_state->astRegsInfo[0].astI2cData[8].u32Data = (dgain & 0xFF); /* index 8 */
        first[vi_pipe] = HI_TRUE;
    }
#else
    HI_U32 vs_dgain;
    if (again > 3) { /* max 3 */
        sns_state->astRegsInfo[0].astI2cData[15].u32Data = 0x21; /* index 15 */
        vs_dgain = dgain * 11; /* hcg is 11 times bigger than lcg  */
        vs_dgain = (vs_dgain > 0x3fff) ? 0x3fff : vs_dgain;
    } else {
        sns_state->astRegsInfo[0].astI2cData[15].u32Data = 0x20; /* index 15 */
        vs_dgain = dgain;
    }
    again = g_again_reg_wdr[again];
    sns_state->astRegsInfo[0].astI2cData[2].u32Data = /* index 2 */
    (again << 4) | (again << 2) | again; /* shift 4, shift 2 */
    sns_state->astRegsInfo[0].astI2cData[3].u32Data = ((dgain & 0xFF00) >> 8); /* index 3, shift 8 */
    sns_state->astRegsInfo[0].astI2cData[4].u32Data = (dgain & 0xFF); /* index 4 */
    sns_state->astRegsInfo[0].astI2cData[7].u32Data = ((dgain & 0xFF00) >> 8); /* index 7, shift 8 */
    sns_state->astRegsInfo[0].astI2cData[8].u32Data = (dgain & 0xFF); /* index 8 */
    sns_state->astRegsInfo[0].astI2cData[9].u32Data = ((vs_dgain & 0xFF00) >> 8); /* index 9, shift 8 */
    sns_state->astRegsInfo[0].astI2cData[10].u32Data = (vs_dgain & 0xFF); /* index 10 */
#endif

    return;
}

static HI_VOID cmos_hlcg_wdr_gains_update(VI_PIPE vi_pipe, HI_U32 again,
    HI_U32 dgain, ISP_SNS_STATE_S *sns_state)
{
    static HI_BOOL first[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = HI_TRUE};
    static HI_U8 first_frame_again[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = 0};
    static HI_U32 first_frame_dgain[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = 0};

    again = (again > 3) ? 3 : again; /* max 3 */

    if (first[vi_pipe]) { // long frame
        first_frame_again[vi_pipe] = again;
        first_frame_dgain[vi_pipe] = dgain;
        first[vi_pipe] = HI_FALSE;
    } else { // short frame
        sns_state->astRegsInfo[0].astI2cData[2].u32Data = /* index 2 */
        (again << 2) | first_frame_again[vi_pipe]; /*  shift 2 */
        sns_state->astRegsInfo[0].astI2cData[3].u32Data = /* index 3 */
        ((first_frame_dgain[vi_pipe] & 0xFF00) >> 8); /* shift 8 */
        sns_state->astRegsInfo[0].astI2cData[4].u32Data = (first_frame_dgain[vi_pipe] & 0xFF); /* index 4 */
        sns_state->astRegsInfo[0].astI2cData[7].u32Data = ((dgain & 0xFF00) >> 8); /* index 7, shift 8 */
        sns_state->astRegsInfo[0].astI2cData[8].u32Data = (dgain & 0xFF); /* index 8 */
        first[vi_pipe] = HI_TRUE;
    }

    return;
}

static HI_VOID cmos_gains_update(VI_PIPE vi_pipe, HI_U32 again, HI_U32 dgain)
{
    ISP_SNS_STATE_S *sns_state = HI_NULL;
    ov2775_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER_VOID(sns_state);
    switch (sns_state->enWDRMode) {
        case WDR_MODE_NONE:
            cmos_linear_gains_update(again, dgain, sns_state);
            break;
        case WDR_MODE_BUILT_IN:
            cmos_builtin_wdr_gains_update(again, dgain, sns_state);
            break;
        case WDR_MODE_2To1_LINE:
            if (sns_state->u8ImgMode == OV2775_SENSOR_1080P_30FPS_HLCG_WDR_2TO1_MODE) {
                cmos_hlcg_wdr_gains_update(vi_pipe, again, dgain, sns_state);
                break;
            }
            cmos_wdr_gains_update(vi_pipe, again, dgain, sns_state);
            break;
        default:
            break;
    }
    return;
}
#define ONETIMERATIO 0x40
#define FRAMEINTERMAX 30
#define EXPRATIOSHIFT 6
#define EXPTIMEMIN 2
/* Only used in WDR_MODE_2To1_LINE and WDR_MODE_2To1_FRAME mode */
static HI_VOID cmos_get_inttime_max(VI_PIPE vi_pipe, HI_U16 man_ratio_enable, HI_U32 *ratio,
    HI_U32 *int_time_max, HI_U32 *int_time_min, HI_U32 *lf_max_int_time)
{
    HI_U32 tmp = 0;
    ISP_SNS_STATE_S *sns_state = HI_NULL;

    CMOS_CHECK_POINTER_VOID(ratio);
    CMOS_CHECK_POINTER_VOID(int_time_max);
    CMOS_CHECK_POINTER_VOID(int_time_min);
    CMOS_CHECK_POINTER_VOID(lf_max_int_time);
    ov2775_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER_VOID(sns_state);

    if (int_time_max[0] == HI_NULL) {
        printf("null pointer when get ae sensor IntTimeMax value!\n");
        return;
    }

    if (sns_state->enWDRMode != WDR_MODE_2To1_LINE) {
        return;
    }

    if (g_fswdr_mode[vi_pipe] == ISP_FSWDR_LONG_FRAME_MODE) { /* long frame mode enable */
        printf("ISP_FSWDR_LONG_FRAME_MODE not support!\n");
    } else if (g_fswdr_mode[vi_pipe] == ISP_FSWDR_AUTO_LONG_FRAME_MODE) {
        printf("ISP_FSWDR_AUTO_LONG_FRAME_MODE not support!\n");
    } else {
        /* To avoid Long frame and Short frame exposure time out of sync, so use (gu32FullLinesStd - 30) */
        tmp = ((sns_state->au32FL[1] - FRAMEINTERMAX - sns_state->au32WDRIntTime[0])
            << EXPRATIOSHIFT) / div_0_to_1(ratio[0]);
        int_time_max[0] = ((sns_state->au32FL[0] - FRAMEINTERMAX) << EXPRATIOSHIFT)
            / (ratio[0] + ONETIMERATIO);
        int_time_max[0] = (tmp < int_time_max[0]) ? tmp : int_time_max[0];
        int_time_min[0] = EXPTIMEMIN;
        int_time_max[1] = sns_state->au32FL[0] - FRAMEINTERMAX - sns_state->au32WDRIntTime[0];
        int_time_min[1] = EXPTIMEMIN;
    }

    return;
}

/* Only used in LINE_WDR mode */
static HI_VOID cmos_ae_fswdr_attr_set(VI_PIPE vi_pipe, AE_FSWDR_ATTR_S *ae_fswdr_attr)
{
    CMOS_CHECK_POINTER_VOID(ae_fswdr_attr);

    g_fswdr_mode[vi_pipe] = ae_fswdr_attr->enFSWDRMode;
    g_max_time_get_cnt[vi_pipe] = 0;
}

HI_S32 cmos_init_ae_exp_function(AE_SENSOR_EXP_FUNC_S *exp_funcs)
{
    CMOS_CHECK_POINTER(exp_funcs);

    memset_s(exp_funcs, sizeof(AE_SENSOR_EXP_FUNC_S), 0, sizeof(AE_SENSOR_EXP_FUNC_S));

    exp_funcs->pfn_cmos_get_ae_default = cmos_get_ae_default;
    exp_funcs->pfn_cmos_fps_set = cmos_fps_set;
    exp_funcs->pfn_cmos_slow_framerate_set = cmos_slow_framerate_set;
    exp_funcs->pfn_cmos_inttime_update = cmos_inttime_update;
    exp_funcs->pfn_cmos_gains_update = cmos_gains_update;
    exp_funcs->pfn_cmos_again_calc_table = cmos_again_calc_table;
    exp_funcs->pfn_cmos_dgain_calc_table  = cmos_dgain_calc_table;
    exp_funcs->pfn_cmos_get_inttime_max   = cmos_get_inttime_max;
    exp_funcs->pfn_cmos_ae_fswdr_attr_set = cmos_ae_fswdr_attr_set;
    return HI_SUCCESS;
}
static HI_S32 cmos_get_isp_black_level(VI_PIPE vi_pipe, ISP_CMOS_BLACK_LEVEL_S *black_level)
{
    HI_S32 i;
    ISP_SNS_STATE_S *sns_state = HI_NULL;

    CMOS_CHECK_POINTER(black_level);
    ov2775_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER(sns_state);

    /* Don't need to update black level when iso change */
    black_level->bUpdate = HI_FALSE;
    if ((sns_state->enWDRMode == WDR_MODE_2To1_LINE)) {
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            black_level->au16BlackLevel[i] = 0x40;
        }
    } else if (sns_state->enWDRMode == WDR_MODE_BUILT_IN) {
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            black_level->au16BlackLevel[i] = 0x40;
        }
    } else {
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            black_level->au16BlackLevel[i] = 0x40;
        }
    }
    return HI_SUCCESS;
}
static HI_VOID cmos_set_pixel_detect(VI_PIPE vi_pipe, HI_BOOL enable)
{
    HI_U32 full_lines_5fps, max_int_time;
    ISP_SNS_STATE_S *sns_state = HI_NULL;

    ov2775_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER_VOID(sns_state);

    if (sns_state->enWDRMode == WDR_MODE_2To1_LINE) {
        return;
    } else {
        full_lines_5fps = (VMAX_1080P30_LINEAR * 30) / 5; /* 30fps, 5fps */
    }

    max_int_time = full_lines_5fps - 2; /* sub 2 */

    if (enable) { /* setup for ISP pixel calibration mode */
        ov2775_write_register(vi_pipe, VMAX_ADDR_H, (full_lines_5fps & 0xFF00) >> 8); /* shift 8 */
        ov2775_write_register(vi_pipe, VMAX_ADDR_L, full_lines_5fps & 0xFF);          /* 5fps */
        ov2775_write_register(vi_pipe, EXPOSURE_ADDR_H, (max_int_time & 0xFF00) >> 8); /* shift 8 */
        ov2775_write_register(vi_pipe, EXPOSURE_ADDR_L, max_int_time & 0xFF);          /* max exposure lines */
        ov2775_write_register(vi_pipe, AGC_ADDR, 0x00);                                   /* min AG */
        ov2775_write_register(vi_pipe, DGC_ADDR_H, 0x01);                                   /* min AG */
        ov2775_write_register(vi_pipe, DGC_ADDR_L, 0x00);                                   /* min DG */
        if (sns_state->enWDRMode == WDR_MODE_BUILT_IN) {
            ov2775_write_register (vi_pipe, LCG_ADDR_H, 0x01);                              /* min DG */
            ov2775_write_register (vi_pipe, LCG_ADDR_L, 0x00);                              /* min DG */
        }
    } else {                                                                           /* setup for ISP 'normal mode' */
        ov2775_write_register(vi_pipe, VMAX_ADDR_H, (sns_state->u32FLStd & 0xFF00) >> 8); /* shift 8 */
        ov2775_write_register(vi_pipe, VMAX_ADDR_L, sns_state->u32FLStd & 0xFF);
        sns_state->bInit = HI_FALSE;
    }

    return;
}

static HI_S32 cmos_set_wdr_mode(VI_PIPE vi_pipe, HI_U8 mode)
{
    ISP_SNS_STATE_S *sns_state = HI_NULL;

    ov2775_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER(sns_state);

    sns_state->bSyncInit = HI_FALSE;

    switch (mode & 0x3F) {
        case WDR_MODE_NONE:
            sns_state->enWDRMode = WDR_MODE_NONE;
            printf("linear mode\n");
            break;

        case WDR_MODE_2To1_LINE:
            sns_state->enWDRMode = WDR_MODE_2To1_LINE;
            printf("2to1 line WDR mode\n");
            break;

        case WDR_MODE_BUILT_IN:
            sns_state->enWDRMode = WDR_MODE_BUILT_IN;
            printf("sensor built in WDR mode\n");
            break;
        default:
            SNS_ERR_TRACE("NOT support this mode!\n");
            return HI_FAILURE;
    }

    memset_s(sns_state->au32WDRIntTime, sizeof(sns_state->au32WDRIntTime), 0, sizeof(sns_state->au32WDRIntTime));

    return HI_SUCCESS;
}

static HI_VOID cmos_linear_sns_reg_info_init(VI_PIPE vi_pipe, ISP_SNS_STATE_S *sns_state)
{
    HI_S32 i;

    sns_state->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
    sns_state->astRegsInfo[0].unComBus.s8I2cDev = g_ov2775_bus_info[vi_pipe].s8I2cDev;
    sns_state->astRegsInfo[0].u8Cfg2ValidDelayMax = 3; /* DelayMax 3 */
    sns_state->astRegsInfo[0].u32RegNum = 7; /* RegNum 7 */

    for (i = 0; i < sns_state->astRegsInfo[0].u32RegNum; i++) {
        sns_state->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
        sns_state->astRegsInfo[0].astI2cData[i].u8DevAddr = OV2775_I2C_ADDR;
        sns_state->astRegsInfo[0].astI2cData[i].u32AddrByteNum = OV2775_ADDR_BYTE;
        sns_state->astRegsInfo[0].astI2cData[i].u32DataByteNum = OV2775_DATA_BYTE;
        sns_state->astRegsInfo[0].astI2cData[i].u8DelayFrmNum = 1;
    }

    sns_state->astRegsInfo[0].astI2cData[0].u32RegAddr = EXPOSURE_ADDR_H; /* index 0 */
    sns_state->astRegsInfo[0].astI2cData[1].u32RegAddr = EXPOSURE_ADDR_L; /* index 1 */

    /* AG */
    sns_state->astRegsInfo[0].astI2cData[2].u32RegAddr = AGC_ADDR; /* index 2 */

    /* DG */
    sns_state->astRegsInfo[0].astI2cData[3].u32RegAddr = DGC_ADDR_H; /* index 3 */
    sns_state->astRegsInfo[0].astI2cData[4].u32RegAddr = DGC_ADDR_L; /* index 4 */

    /* VMAX */
    sns_state->astRegsInfo[0].astI2cData[5].u32RegAddr = VMAX_ADDR_H; /* index 5 */
    sns_state->astRegsInfo[0].astI2cData[6].u32RegAddr = VMAX_ADDR_L; /* index 6 */

    sns_state->bSyncInit = HI_TRUE;
    return;
}

static HI_VOID cmos_builtin_sns_reg_info_init(VI_PIPE vi_pipe, ISP_SNS_STATE_S *sns_state)
{
    HI_S32 i;

    sns_state->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
    sns_state->astRegsInfo[0].unComBus.s8I2cDev = g_ov2775_bus_info[vi_pipe].s8I2cDev;
    sns_state->astRegsInfo[0].u8Cfg2ValidDelayMax = 3; /* DelayMax 3 */
    sns_state->astRegsInfo[0].u32RegNum = 9; /* RegNum 9 */

    for (i = 0; i < sns_state->astRegsInfo[0].u32RegNum; i++) {
        sns_state->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
        sns_state->astRegsInfo[0].astI2cData[i].u8DevAddr = OV2775_I2C_ADDR;
        sns_state->astRegsInfo[0].astI2cData[i].u32AddrByteNum = OV2775_ADDR_BYTE;
        sns_state->astRegsInfo[0].astI2cData[i].u32DataByteNum = OV2775_DATA_BYTE;
        sns_state->astRegsInfo[0].astI2cData[i].u8DelayFrmNum = 1;
    }

    sns_state->astRegsInfo[0].astI2cData[0].u32RegAddr = EXPOSURE_ADDR_H; /* index 0 */
    sns_state->astRegsInfo[0].astI2cData[1].u32RegAddr = EXPOSURE_ADDR_L; /* index 1 */

    /* AG */
    sns_state->astRegsInfo[0].astI2cData[2].u32RegAddr = AGC_ADDR; /* index 2 */

    /* DG */
    sns_state->astRegsInfo[0].astI2cData[3].u32RegAddr = DGC_ADDR_H; /* index 3 */
    sns_state->astRegsInfo[0].astI2cData[4].u32RegAddr = DGC_ADDR_L; /* index 4 */

    /* VMAX */
    sns_state->astRegsInfo[0].astI2cData[5].u32RegAddr = VMAX_ADDR_H; /* index 5 */
    sns_state->astRegsInfo[0].astI2cData[6].u32RegAddr = VMAX_ADDR_L; /* index 6 */

    sns_state->astRegsInfo[0].astI2cData[7].u32RegAddr = LCG_ADDR_H; /* index 7 */
    sns_state->astRegsInfo[0].astI2cData[8].u32RegAddr = LCG_ADDR_L; /* index 8 */

    sns_state->bSyncInit = HI_TRUE;
    return;
}

static HI_VOID cmos_wdr_sns_reg_info_init(VI_PIPE vi_pipe, ISP_SNS_STATE_S *sns_state)
{
    HI_S32 i;

    sns_state->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
    sns_state->astRegsInfo[0].unComBus.s8I2cDev = g_ov2775_bus_info[vi_pipe].s8I2cDev;
    sns_state->astRegsInfo[0].u8Cfg2ValidDelayMax = 3; /* DelayMax 3 */
    sns_state->astRegsInfo[0].u32RegNum = 18; /* RegNum 18 */

    for (i = 0; i < sns_state->astRegsInfo[0].u32RegNum; i++) {
        sns_state->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
        sns_state->astRegsInfo[0].astI2cData[i].u8DevAddr = OV2775_I2C_ADDR;
        sns_state->astRegsInfo[0].astI2cData[i].u32AddrByteNum = OV2775_ADDR_BYTE;
        sns_state->astRegsInfo[0].astI2cData[i].u32DataByteNum = OV2775_DATA_BYTE;
        sns_state->astRegsInfo[0].astI2cData[i].u8DelayFrmNum = 1;
    }

    sns_state->astRegsInfo[0].astI2cData[0].u32RegAddr = EXPOSURE_ADDR_H; /* index 0 */
    sns_state->astRegsInfo[0].astI2cData[1].u32RegAddr = EXPOSURE_ADDR_L; /* index 1 */

    /* AG */
    sns_state->astRegsInfo[0].astI2cData[2].u32RegAddr = AGC_ADDR; /* index 2 */

    /* DG */
    sns_state->astRegsInfo[0].astI2cData[3].u32RegAddr = DGC_ADDR_H; /* index 3 */
    sns_state->astRegsInfo[0].astI2cData[4].u32RegAddr = DGC_ADDR_L; /* index 4 */

    /* VMAX */
    sns_state->astRegsInfo[0].astI2cData[5].u32RegAddr = VMAX_ADDR_H; /* index 5 */
    sns_state->astRegsInfo[0].astI2cData[6].u32RegAddr = VMAX_ADDR_L; /* index 6 */

    sns_state->astRegsInfo[0].astI2cData[7].u32RegAddr = LCG_ADDR_H; /* index 7 */
    sns_state->astRegsInfo[0].astI2cData[8].u32RegAddr = LCG_ADDR_L; /* index 8 */

    sns_state->astRegsInfo[0].astI2cData[9].u32RegAddr = VS_ADDR_H; /* index 9 */
    sns_state->astRegsInfo[0].astI2cData[10].u32RegAddr = VS_ADDR_L; /* index 10 */

    sns_state->astRegsInfo[0].astI2cData[11].u32RegAddr = SHORT_EXPOSURE_ADDR_H; /* index 11 */
    sns_state->astRegsInfo[0].astI2cData[12].u32RegAddr = SHORT_EXPOSURE_ADDR_L; /* index 12 */

    sns_state->astRegsInfo[0].astI2cData[13].u32RegAddr = GROUP_HOLD_LAUNCH; /* index 13 */
    sns_state->astRegsInfo[0].astI2cData[13].u32Data = 0; /* index 13 */
    sns_state->astRegsInfo[0].astI2cData[13].u8DelayFrmNum = 2; /* index 13 delay 2 */
    sns_state->astRegsInfo[0].astI2cData[14].u32RegAddr = GROUP_HOLD_SEL; /* index 14 */
    sns_state->astRegsInfo[0].astI2cData[14].u32Data = 0; /* index 14 */
    sns_state->astRegsInfo[0].astI2cData[14].u8DelayFrmNum = 2; /* index 14 delay 2 */

    sns_state->astRegsInfo[0].astI2cData[15].u32RegAddr = HCG_LCG_SEL; /* index 15 */
    sns_state->astRegsInfo[0].astI2cData[15].u8DelayFrmNum = 2; /* index 15 delay 2 */

    sns_state->astRegsInfo[0].astI2cData[16].u32RegAddr = GROUP_HOLD_SEL; /* index 16 */
    sns_state->astRegsInfo[0].astI2cData[16].u32Data = 0x10; /* index 16 */
    sns_state->astRegsInfo[0].astI2cData[16].u8DelayFrmNum = 2; /* index 16 delay 2 */
    sns_state->astRegsInfo[0].astI2cData[17].u32RegAddr = GROUP_HOLD_LAUNCH; /* index 17 */
    sns_state->astRegsInfo[0].astI2cData[17].u32Data = 0x01; /* index 17 */
    sns_state->astRegsInfo[0].astI2cData[17].u8DelayFrmNum = 2; /* index 17 delay 2 */

    sns_state->bSyncInit = HI_TRUE;
    return;
}

static HI_VOID cmos_hlcg_wdr_sns_reg_info_init(VI_PIPE vi_pipe, ISP_SNS_STATE_S *sns_state)
{
    HI_S32 i;

    sns_state->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
    sns_state->astRegsInfo[0].unComBus.s8I2cDev = g_ov2775_bus_info[vi_pipe].s8I2cDev;
    sns_state->astRegsInfo[0].u8Cfg2ValidDelayMax = 3; /* DelayMax 3 */
    sns_state->astRegsInfo[0].u32RegNum = 9; /* RegNum 9 */

    for (i = 0; i < sns_state->astRegsInfo[0].u32RegNum; i++) {
        sns_state->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
        sns_state->astRegsInfo[0].astI2cData[i].u8DevAddr = OV2775_I2C_ADDR;
        sns_state->astRegsInfo[0].astI2cData[i].u32AddrByteNum = OV2775_ADDR_BYTE;
        sns_state->astRegsInfo[0].astI2cData[i].u32DataByteNum = OV2775_DATA_BYTE;
        sns_state->astRegsInfo[0].astI2cData[i].u8DelayFrmNum = 1;
    }

    sns_state->astRegsInfo[0].astI2cData[0].u32RegAddr = EXPOSURE_ADDR_H; /* index 0 */
    sns_state->astRegsInfo[0].astI2cData[1].u32RegAddr = EXPOSURE_ADDR_L; /* index 1 */

    /* AG */
    sns_state->astRegsInfo[0].astI2cData[2].u32RegAddr = AGC_ADDR; /* index 2 */

    /* DG */
    sns_state->astRegsInfo[0].astI2cData[3].u32RegAddr = DGC_ADDR_H; /* index 3 */
    sns_state->astRegsInfo[0].astI2cData[4].u32RegAddr = DGC_ADDR_L; /* index 4 */

    /* VMAX */
    sns_state->astRegsInfo[0].astI2cData[5].u32RegAddr = VMAX_ADDR_H; /* index 5 */
    sns_state->astRegsInfo[0].astI2cData[6].u32RegAddr = VMAX_ADDR_L; /* index 6 */

    sns_state->astRegsInfo[0].astI2cData[7].u32RegAddr = LCG_ADDR_H; /* index 7 */
    sns_state->astRegsInfo[0].astI2cData[8].u32RegAddr = LCG_ADDR_L; /* index 8 */

    sns_state->bSyncInit = HI_TRUE;
    return;
}

static HI_VOID cmos_sns_reg_info_update(VI_PIPE vi_pipe, ISP_SNS_STATE_S *sns_state)
{
    HI_S32 i;

    for (i = 0; i < sns_state->astRegsInfo[0].u32RegNum; i++) {
        if (sns_state->astRegsInfo[0].astI2cData[i].u32Data !=
            sns_state->astRegsInfo[1].astI2cData[i].u32Data) {
            sns_state->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
        } else {
            sns_state->astRegsInfo[0].astI2cData[i].bUpdate = HI_FALSE;
        }
    }

    if (sns_state->astRegsInfo[0].astI2cData[15].u32Data !=  /* index 15 */
        sns_state->astRegsInfo[1].astI2cData[15].u32Data) {  /* index 15 */
        for (i = 13; i < 18; i++) {                          /* index 13 num_max 18 */
            sns_state->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
        }
    } else {
        for (i = 13; i < 18; i++) {                          /* index 13 num_max 18 */
            sns_state->astRegsInfo[0].astI2cData[i].bUpdate = HI_FALSE;
        }
    }

    return;
}
static HI_S32 cmos_get_sns_regs_info(VI_PIPE vi_pipe, ISP_SNS_REGS_INFO_S *sns_regs_info)
{
    ISP_SNS_STATE_S *sns_state = HI_NULL;

    CMOS_CHECK_POINTER(sns_regs_info);
    ov2775_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER(sns_state);

    if ((sns_state->bSyncInit == HI_FALSE) || (sns_regs_info->bConfig == HI_FALSE)) {
        if (sns_state->enWDRMode == WDR_MODE_NONE) {
            cmos_linear_sns_reg_info_init(vi_pipe, sns_state);
        } else if (sns_state->enWDRMode == WDR_MODE_BUILT_IN) {
            cmos_builtin_sns_reg_info_init(vi_pipe, sns_state);
        } else if (sns_state->enWDRMode == WDR_MODE_2To1_LINE) {
            if (sns_state->u8ImgMode == OV2775_SENSOR_1080P_30FPS_HLCG_WDR_2TO1_MODE) {
                cmos_hlcg_wdr_sns_reg_info_init(vi_pipe, sns_state);
            } else {
                cmos_wdr_sns_reg_info_init(vi_pipe, sns_state);
            }
        }
    } else {
        cmos_sns_reg_info_update(vi_pipe, sns_state);
    }

    if (sns_state->au32FL[1] < sns_state->au32FL[0]) {
        sns_state->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0; /* index 5 */
        sns_state->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0; /* index 6 */
    } else {
        sns_state->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 1; /* index 5 */
        sns_state->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 1; /* index 6 */
    }

    sns_regs_info->bConfig = HI_FALSE;
    memcpy_s(sns_regs_info, sizeof(ISP_SNS_REGS_INFO_S), &sns_state->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S));
    memcpy_s(&sns_state->astRegsInfo[1], sizeof(ISP_SNS_REGS_INFO_S), &sns_state->astRegsInfo[0],
             sizeof(ISP_SNS_REGS_INFO_S));

    sns_state->au32FL[1] = sns_state->au32FL[0];

    return HI_SUCCESS;
}

#define ov2775_err_mode_print(pstSensorImageMode,pstSnsState)\
    do{\
        SNS_ERR_TRACE("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",\
                      (pstSensorImageMode)->u16Width,  \
                      (pstSensorImageMode)->u16Height, \
                      (pstSensorImageMode)->f32Fps,    \
                      (pstSnsState)->enWDRMode);\
    }while (0)

static HI_S32 cmos_set_image_mode(VI_PIPE vi_pipe, ISP_CMOS_SENSOR_IMAGE_MODE_S *sns_image_mode)
{
    HI_U32 i;
    HI_U8 image_mode;
    ISP_SNS_STATE_S *sns_state = HI_NULL;
    CMOS_CHECK_POINTER(sns_image_mode);
    ov2775_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER(sns_state);

    image_mode = sns_state->u8ImgMode;
    sns_state->bSyncInit = HI_FALSE;

    for (i = 0; i < OV2775_MODE_BUTT; i++) {
        if (sns_image_mode->f32Fps <= g_ov2775_mode_tbl[i].max_fps &&
            sns_image_mode->u16Width <= g_ov2775_mode_tbl[i].width &&
            sns_image_mode->u16Height <= g_ov2775_mode_tbl[i].height &&
            sns_state->enWDRMode == g_ov2775_mode_tbl[i].wdr_mode &&
            sns_image_mode->u8SnsMode == g_ov2775_mode_tbl[i].sns_mode) {
            image_mode = (ov2775_res_mode)i;
            break;
        }
    }

    if (i >= OV2775_MODE_BUTT) {
        ov2775_err_mode_print(sns_image_mode, sns_state);
        return HI_FAILURE;
    }

    if ((sns_state->bInit == HI_TRUE) && (image_mode == sns_state->u8ImgMode)) {
        return ISP_DO_NOT_NEED_SWITCH_IMAGEMODE; /* Don't need to switch SensorImageMode */
    }

    sns_state->u32FLStd  = g_ov2775_mode_tbl[i].verti_lines;
    sns_state->u8ImgMode = image_mode;
    sns_state->au32FL[0] = sns_state->u32FLStd;
    sns_state->au32FL[1] = sns_state->au32FL[0];
    memset_s(sns_state->au32WDRIntTime, sizeof(sns_state->au32WDRIntTime), 0,
             sizeof(sns_state->au32WDRIntTime));

    return HI_SUCCESS;
}

HI_VOID sensor_global_init(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *sns_state = HI_NULL;

    ov2775_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER_VOID(sns_state);

    sns_state->bInit = HI_FALSE;
    sns_state->bSyncInit = HI_FALSE;
    sns_state->u8ImgMode = OV2775_SENSOR_1080P_30FPS_LINEAR_MODE;
    sns_state->enWDRMode = WDR_MODE_NONE;
    sns_state->u32FLStd  = VMAX_1080P30_LINEAR;
    sns_state->au32FL[0] = VMAX_1080P30_LINEAR;
    sns_state->au32FL[1] = VMAX_1080P30_LINEAR;

    memset_s(&sns_state->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset_s(&sns_state->astRegsInfo[1], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
}

HI_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *sns_exp_func)
{
    CMOS_CHECK_POINTER(sns_exp_func);

    memset_s(sns_exp_func, sizeof(ISP_SENSOR_EXP_FUNC_S), 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    sns_exp_func->pfn_cmos_sensor_init = ov2775_init;
    sns_exp_func->pfn_cmos_sensor_exit = ov2775_exit;
    sns_exp_func->pfn_cmos_sensor_global_init = sensor_global_init;
    sns_exp_func->pfn_cmos_set_image_mode = cmos_set_image_mode;
    sns_exp_func->pfn_cmos_set_wdr_mode = cmos_set_wdr_mode;

    sns_exp_func->pfn_cmos_get_isp_default = cmos_get_isp_default;
    sns_exp_func->pfn_cmos_get_isp_black_level = cmos_get_isp_black_level;
    sns_exp_func->pfn_cmos_set_pixel_detect = cmos_set_pixel_detect;
    sns_exp_func->pfn_cmos_get_sns_reg_info = cmos_get_sns_regs_info;

    return HI_SUCCESS;
}

/****************************************************************************
 * callback structure                                                       *
 ****************************************************************************/
static HI_S32 ov2775_set_bus_info(VI_PIPE vi_pipe, ISP_SNS_COMMBUS_U sns_bus_info)
{
    g_ov2775_bus_info[vi_pipe].s8I2cDev = sns_bus_info.s8I2cDev;

    return HI_SUCCESS;
}

static HI_S32 sensor_ctx_init(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *sns_state_ctx = HI_NULL;

    ov2775_sensor_get_ctx(vi_pipe, sns_state_ctx);

    if (sns_state_ctx == HI_NULL) {
        sns_state_ctx = (ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
        if (sns_state_ctx == HI_NULL) {
            SNS_ERR_TRACE("Isp[%d] SnsCtx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset_s(sns_state_ctx, sizeof(ISP_SNS_STATE_S), 0, sizeof(ISP_SNS_STATE_S));

    ov2775_sensor_set_ctx(vi_pipe, sns_state_ctx);

    return HI_SUCCESS;
}

static HI_VOID sensor_ctx_exit(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *sns_state_ctx = HI_NULL;

    ov2775_sensor_get_ctx(vi_pipe, sns_state_ctx);
    SENSOR_FREE(sns_state_ctx);
    ov2775_sensor_reset_ctx(vi_pipe);
}

static HI_S32 sensor_register_callback(VI_PIPE vi_pipe, ALG_LIB_S *ae_lib, ALG_LIB_S *awb_lib)
{
    HI_S32 ret;
    ISP_SENSOR_REGISTER_S isp_register;
    AE_SENSOR_REGISTER_S ae_register;
    AWB_SENSOR_REGISTER_S awb_register;
    ISP_SNS_ATTR_INFO_S sns_attr_info;

    CMOS_CHECK_POINTER(ae_lib);
    CMOS_CHECK_POINTER(awb_lib);

    ret = sensor_ctx_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        return HI_FAILURE;
    }

    sns_attr_info.eSensorId = OV2775_ID;

    ret = cmos_init_sensor_exp_function(&isp_register.stSnsExp);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("cmos init exp function failed!\n");
        return HI_FAILURE;
    }

    ret = HI_MPI_ISP_SensorRegCallBack(vi_pipe, &sns_attr_info, &isp_register);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor register callback function failed!\n");
        return ret;
    }

    ret = cmos_init_ae_exp_function(&ae_register.stSnsExp);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("cmos init ae exp function failed!\n");
        return HI_FAILURE;
    }

    ret = HI_MPI_AE_SensorRegCallBack(vi_pipe, ae_lib, &sns_attr_info, &ae_register);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor register callback function to ae lib failed!\n");
        return ret;
    }

    ret = cmos_init_awb_exp_function(&awb_register.stSnsExp);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("cmos init awb exp function failed!\n");
        return HI_FAILURE;
    }

    ret = HI_MPI_AWB_SensorRegCallBack(vi_pipe, awb_lib, &sns_attr_info, &awb_register);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor register callback function to awb lib failed!\n");
        return ret;
    }

    return HI_SUCCESS;
}

static HI_S32 sensor_unregister_callback(VI_PIPE vi_pipe, ALG_LIB_S *ae_lib, ALG_LIB_S *awb_lib)
{
    HI_S32 ret;

    CMOS_CHECK_POINTER(ae_lib);
    CMOS_CHECK_POINTER(awb_lib);

    ret = HI_MPI_ISP_SensorUnRegCallBack(vi_pipe, OV2775_ID);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function failed!\n");
        return ret;
    }

    ret = HI_MPI_AE_SensorUnRegCallBack(vi_pipe, ae_lib, OV2775_ID);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function to ae lib failed!\n");
        return ret;
    }

    ret = HI_MPI_AWB_SensorUnRegCallBack(vi_pipe, awb_lib, OV2775_ID);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function to awb lib failed!\n");
        return ret;
    }

    sensor_ctx_exit(vi_pipe);

    return HI_SUCCESS;
}

static HI_S32 sensor_set_init(VI_PIPE vi_pipe, ISP_INIT_ATTR_S *init_attr)
{
    HI_S32 i;
    CMOS_CHECK_POINTER(init_attr);

    g_init_exposure[vi_pipe]  = init_attr->u32Exposure;
    g_lines_per500ms[vi_pipe] = init_attr->u32LinesPer500ms;
    g_init_wb_gain[vi_pipe][0] = init_attr->u16WBRgain;
    g_init_wb_gain[vi_pipe][1] = init_attr->u16WBGgain;
    g_init_wb_gain[vi_pipe][2] = init_attr->u16WBBgain; /* 2: Bgain  */
    g_sample_rgain[vi_pipe]   = init_attr->u16SampleRgain;
    g_sample_bgain[vi_pipe]   = init_attr->u16SampleBgain;
    g_quick_start_en[vi_pipe] = init_attr->stQuickStart.bQuickStartEn;

    g_ae_routeex_valid[vi_pipe]  = init_attr->bAERouteExValid;
    memcpy_s(&g_init_ae_route[vi_pipe], sizeof(ISP_AE_ROUTE_S), &init_attr->stAERoute, sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&g_init_ae_routeex[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S), &init_attr->stAERouteEx,
             sizeof(ISP_AE_ROUTE_EX_S));
    memcpy_s(&g_init_ae_routesf[vi_pipe], sizeof(ISP_AE_ROUTE_S), &init_attr->stAERouteSF, sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&g_init_ae_routesfex[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S), &init_attr->stAERouteSFEx,
             sizeof(ISP_AE_ROUTE_EX_S));

    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        g_init_ccm[vi_pipe][i] = init_attr->au16CCM[i];
    }

    return HI_SUCCESS;
}

ISP_SNS_OBJ_S stSnsOV2775Obj = {
    .pfnRegisterCallback    = sensor_register_callback,
    .pfnUnRegisterCallback  = sensor_unregister_callback,
    .pfnStandby             = ov2775_standby,
    .pfnRestart             = ov2775_restart,
    .pfnMirrorFlip          = ov2775_mirror_flip,
    .pfnWriteReg            = ov2775_write_register,
    .pfnReadReg             = ov2775_read_register,
    .pfnSetBusInfo          = ov2775_set_bus_info,
    .pfnSetInit             = sensor_set_init
};

