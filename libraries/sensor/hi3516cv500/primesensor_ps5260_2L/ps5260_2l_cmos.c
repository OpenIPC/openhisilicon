/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of ps5260_2l_cmos.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "ps5260_2l_cmos.h"
#include "ps5260_2l_cmos_ex.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include "mpi_isp.h"
#include "mpi_ae.h"
#include "mpi_awb.h"

#define PS5260_ID   5260

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/
#define HIGH_8BITS(x)   (((x) & 0xFF00) >> 8)
#define LOW_8BITS(x)    ((x) & 0x00FF)
#define LOWER_4BITS(x)  ((x) & 0x000F)
#define HIGHER_4BITS(x) (((x) & 0xF000) >> 12)
#define HIGHER_8BITS(x) (((x) & 0x0FF0) >> 4)

ISP_SNS_STATE_S *g_ps5260_2l[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define ps5260_2l_sensor_set_ctx(dev, pstCtx)   ((g_ps5260_2l[dev]) = (pstCtx))
#define ps5260_2l_sensor_reset_ctx(dev)         (g_ps5260_2l[dev] = HI_NULL)

ISP_SNS_COMMBUS_U g_aunps5260_2l_BusInfo[ISP_MAX_PIPE_NUM] = {
    [0] = { .s8I2cDev = 0},
    [1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1}
};

ISP_SNS_COMMBUS_U *ps5260_2l_get_bus_Info(VI_PIPE Vi_Pipe)
{
    return &g_aunps5260_2l_BusInfo[Vi_Pipe];
}

ISP_SNS_STATE_S *ps5260_2l_get_ctx(VI_PIPE vi_pipe)
{
    return g_ps5260_2l[vi_pipe];
}

static ISP_FSWDR_MODE_E g_genFSWDRMode[ISP_MAX_PIPE_NUM] = {
    [0 ... ISP_MAX_PIPE_NUM - 1] = ISP_FSWDR_NORMAL_MODE
};

static HI_U32 g_init_exposure[ISP_MAX_PIPE_NUM]  = {0};
static HI_U32 g_lines_per500ms[ISP_MAX_PIPE_NUM] = {0};

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

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
/* ps5260 Register Address */
#define PS5260_EXPOSURE_ADDR_H                  0xC
#define PS5260_EXPOSURE_ADDR_L                  0xD
#define PS5260_EXPOSURE_NE_ADDR_H               0xE
#define PS5260_EXPOSURE_NE_ADDR_L               0xF
#define PS5260_EXPOSURE_NEP_ADDR_H              0x10
#define PS5260_EXPOSURE_NEP_ADDR_L              0x12
#define PS5260_AGC_SGHD_ADDR                    0x18
#define PS5260_AGC_ADDR                         0x83
#define PS5260_DGC_ADDR                         0x80
#define PS5260_VMAX_ADDR_H                      0xA
#define PS5260_VMAX_ADDR_L                      0xB
#define PS5260_BANK_REG                         0xEF
#define PS5260_BANK1_UDPDATE                    0x9

enum REG_NUM {
    e_BANK_REG = 0,
    e_EXPOSURE_ADDR_H,
    e_EXPOSURE_ADDR_L,
    e_VMAX_ADDR_H,
    e_VMAX_ADDR_L,
    e_AGC_ADDR,
    e_DGC_ADDR,
    e_AGC_SGHD_ADDR,
    e_EXPOSURE_NE_ADDR_H,
    e_EXPOSURE_NE_ADDR_L,
    e_EXPOSURE_NEP_ADDR_H,
    e_EXPOSURE_NEP_ADDR_L,
    e_BANK1_UDPDATE,
    e_REG_NUM_MAX,
};

#define PS5260_MIN_AGAIN_IDX       2
#define PS5260_MIN_TIME            2

#define NEPLS_LB                    25
#define NEPLS_UB                    200
#define NEPLS_SCALE                 38
#define ABC_LRUB_MIN                3

HI_U32 g_sensor_const[ISP_MAX_PIPE_NUM] = {0};
HI_U16 g_temp_manual[ISP_MAX_PIPE_NUM] = {0};
HI_U8  g_abclr[ISP_MAX_PIPE_NUM] = {ABC_LRUB_MIN};
HI_BOOL g_first_time_patch[ISP_MAX_PIPE_NUM] = {HI_TRUE};
HI_U32 g_temp_manual_c[ISP_MAX_PIPE_NUM] = {0};
HI_U16 g_gain_min[ISP_MAX_PIPE_NUM] = {PS5260_MIN_AGAIN_IDX};
HI_U32 g_exp_time_patch[ISP_MAX_PIPE_NUM] = {0};
HI_FLOAT g_fps[ISP_MAX_PIPE_NUM] = {0};
HI_U32 g_again_patch[ISP_MAX_PIPE_NUM] = {0};
HI_U32 g_vmax_ts[ISP_MAX_PIPE_NUM] = {PS5260_VMAX_1080P30_LINEAR - 1};
HI_U32 g_vmax_target[ISP_MAX_PIPE_NUM] = {PS5260_VMAX_1080P30_LINEAR - 1};
HI_U8 g_fps_patch[ISP_MAX_PIPE_NUM] = {0};
HI_U8 g_fps_patch_cnt[ISP_MAX_PIPE_NUM] = {0};
HI_U32 g_pre_int_total_time[ISP_MAX_PIPE_NUM] = {PS5260_VMAX_1080P30_LINEAR - 1};

HI_U32 *g_sensor_const1_ctx(VI_PIPE vi_pipe)
{
    return &(g_sensor_const[vi_pipe]);
}

#define PS5260_RES_IS_1080P(w, h) ((w) <= 1920 && (h) <= 1080)

#define PS5260_2L_ERR_MODE_PRINT(sensor_image_mode, sns_state)                                             \
    do {                                                                                                      \
        SNS_ERR_TRACE("Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n", (sensor_image_mode)->u16Width, \
                      (sensor_image_mode)->u16Height, (sensor_image_mode)->f32Fps, (sns_state)->enWDRMode);     \
    } while (0)

#define QUEUENUM 18
typedef struct hiQUEUE_PSENSOR {
    HI_U8  size;
    HI_U8  ptr;
    HI_U32 sum;
    HI_U16 queue[QUEUENUM];
} QUEUE_PSENSOR;

enum QUEUE_NUM {
    e_QUEUE_FDC = 0,
    e_QUEUE_TEMP,
    e_QUEUE_MAX
};

#define QUEUE_PSENSOR_DEFAULT {18, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}

static QUEUE_PSENSOR g_queue_psensor[e_QUEUE_MAX] = {
    QUEUE_PSENSOR_DEFAULT,
    QUEUE_PSENSOR_DEFAULT
};

static void psensor_queue_reset(HI_U8 idx)
{
    if (idx < (sizeof(g_queue_psensor) / sizeof(QUEUE_PSENSOR))) {
        QUEUE_PSENSOR sQueueReset = QUEUE_PSENSOR_DEFAULT;
        memcpy_s(&g_queue_psensor[idx], sizeof(g_queue_psensor[idx]), &sQueueReset, sizeof(QUEUE_PSENSOR));
    }

    return;
}

static HI_U32 psensor_queue_add(HI_U8 idx, HI_U16 data)
{
    if (idx < (sizeof(g_queue_psensor) / sizeof(QUEUE_PSENSOR))) {
        HI_U8   cnt;
        HI_U32  sum, max, min;

        sum = 0;
        max = 0;
        min = 0xFFFFFFFF;
        g_queue_psensor[idx].queue[g_queue_psensor[idx].ptr] = data;
        g_queue_psensor[idx].ptr = (g_queue_psensor[idx].ptr + 1) % g_queue_psensor[idx].size;

        for (cnt = 0; cnt < g_queue_psensor[idx].size; cnt++) {
            sum += g_queue_psensor[idx].queue[cnt];
            if (min > g_queue_psensor[idx].queue[cnt]) {
                min = g_queue_psensor[idx].queue[cnt];
            } else if(max < g_queue_psensor[idx].queue[cnt]) {
                max = g_queue_psensor[idx].queue[cnt];
            }
        }
        g_queue_psensor[idx].sum = sum - min - max;
        return g_queue_psensor[idx].sum;
    }

    return 0;
}

static void psensor_queue_avg(HI_U8 idx, HI_U32 data)
{
    if (idx < (sizeof(g_queue_psensor) / sizeof(QUEUE_PSENSOR))) {
        HI_U8 cnt;

        g_queue_psensor[idx].sum = data;
        g_queue_psensor[idx].ptr = 0;
        data = data / g_queue_psensor[idx].size;
        for (cnt = 0; cnt < g_queue_psensor[idx].size; cnt++) {
            g_queue_psensor[idx].queue[cnt] = data;
        }
    }

    return;
}

const ps5260_2l_video_mode_tbl g_ps5260_2l_mode_tbl[2] = { /* max mode num 2 */
    {PS5260_VMAX_1080P30_LINEAR,        PS5260_FULL_LINES_MAX,          30, 0.51,
     1920, 1080, 0, WDR_MODE_NONE,      "ps5260_SENSOR_1080P_30FPS_LINEAR_MODE"},
    {PS5260_VMAX_1080P30_BuiltinWDR,    PS5260_FULL_LINES_MAX,          30, 0.51,
     1920, 1080, 0, WDR_MODE_BUILT_IN,  "ps5260_SENSOR_1080P_BuiltinWDR_MODE"},
};

static HI_VOID cmos_get_ae_comm_default(VI_PIPE Vi_Pipe, AE_SENSOR_DEFAULT_S *ae_sns_dft,
    ISP_SNS_STATE_S *sns_state)
{
    ae_sns_dft->u32FullLinesStd = sns_state->u32FLStd;
    ae_sns_dft->u32FlickerFreq = 50 * 256; /* light flicker freq: 50Hz, accuracy: 256 */
    ae_sns_dft->u32FullLinesMax = PS5260_FULL_LINES_MAX;

    ae_sns_dft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    ae_sns_dft->stIntTimeAccu.f32Accuracy = 1;
    ae_sns_dft->stIntTimeAccu.f32Offset = 0.5912926; /* offset 0.5912926 */

    ae_sns_dft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
    ae_sns_dft->stAgainAccu.f32Accuracy = 1;

    ae_sns_dft->stDgainAccu.enAccuType = AE_ACCURACY_TABLE;
    ae_sns_dft->stDgainAccu.f32Accuracy = 1;

    ae_sns_dft->u32ISPDgainShift = 8; /* accuracy: 8 */
    ae_sns_dft->u32MinISPDgainTarget = 1 << ae_sns_dft->u32ISPDgainShift;
    ae_sns_dft->u32MaxISPDgainTarget = 20 << ae_sns_dft->u32ISPDgainShift; /* max 20 */

    ae_sns_dft->enMaxIrisFNO = ISP_IRIS_F_NO_1_0;
    ae_sns_dft->enMinIrisFNO = ISP_IRIS_F_NO_32_0;

    ae_sns_dft->bAERouteExValid = HI_FALSE;
    ae_sns_dft->stAERouteAttr.u32TotalNum = 0;
    ae_sns_dft->stAERouteAttrEx.u32TotalNum = 0;
    if (g_lines_per500ms[Vi_Pipe] == 0) {
        ae_sns_dft->u32LinesPer500ms = sns_state->u32FLStd * 30 / 2; /* 30fps, div 2 */
    } else {
        ae_sns_dft->u32LinesPer500ms = g_lines_per500ms[Vi_Pipe];
    }
    ae_sns_dft->stQuickStart.bQuickStartEn = g_quick_start_en[Vi_Pipe];
    ae_sns_dft->stQuickStart.u8BlackFrameNum = 0;
    return;
}
static HI_VOID cmos_get_ae_linear_default(VI_PIPE vi_pipe, AE_SENSOR_DEFAULT_S *ae_sns_dft,
    ISP_SNS_STATE_S *sns_state)
{
    ae_sns_dft->au8HistThresh[0] = 0xd;
    ae_sns_dft->au8HistThresh[1] = 0x28;
    ae_sns_dft->au8HistThresh[2] = 0x60; /* index 2 */
    ae_sns_dft->au8HistThresh[3] = 0x80; /* index 3 */

    ae_sns_dft->u32MaxAgain = 65536; /* max 65536 */
    ae_sns_dft->u32MinAgain = 1152; /* min 1152 */
    ae_sns_dft->u32MaxAgainTarget = ae_sns_dft->u32MaxAgain;
    ae_sns_dft->u32MinAgainTarget = ae_sns_dft->u32MinAgain;

    ae_sns_dft->u32MaxDgain = 32768; /* max 32768 */
    ae_sns_dft->u32MinDgain = 1024; /* min 1024 */
    ae_sns_dft->u32MaxDgainTarget = ae_sns_dft->u32MaxDgain;
    ae_sns_dft->u32MinDgainTarget = ae_sns_dft->u32MinDgain;

    ae_sns_dft->u8AeCompensation = 0x38;
    ae_sns_dft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;

    ae_sns_dft->u32InitExposure = g_init_exposure[vi_pipe] ? g_init_exposure[vi_pipe] : 148859; /* init 148859 */

    ae_sns_dft->u32MaxIntTime = sns_state->u32FLStd - 4; /* sub 4 */
    ae_sns_dft->u32MinIntTime = 3; /* min 3 */
    ae_sns_dft->u32MaxIntTimeTarget = 65535; /* max 65535 */
    ae_sns_dft->u32MinIntTimeTarget = 3; /* min 3 */
    ae_sns_dft->bAERouteExValid = g_ae_routeex_valid[vi_pipe];
    memcpy_s(&ae_sns_dft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), &g_init_ae_route[vi_pipe],
             sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&ae_sns_dft->stAERouteAttrEx, sizeof(ISP_AE_ROUTE_EX_S), &g_init_ae_routeex[vi_pipe],
             sizeof(ISP_AE_ROUTE_EX_S));
    return;
}
static HI_VOID cmos_get_ae_builtinwdr_default(VI_PIPE vi_pipe, AE_SENSOR_DEFAULT_S *ae_sns_dft,
    ISP_SNS_STATE_S *sns_state)
{
    ae_sns_dft->au8HistThresh[0] = 0xC;
    ae_sns_dft->au8HistThresh[1] = 0x18;
    ae_sns_dft->au8HistThresh[2] = 0x60; /* index 2 */
    ae_sns_dft->au8HistThresh[3] = 0x80; /* index 3 */

    ae_sns_dft->u32MaxIntTime = sns_state->u32FLStd - 2; /* sub 2 */
    ae_sns_dft->u32MinIntTime = 4; /* min 4 */

    ae_sns_dft->u32MaxIntTimeTarget = 65535; /* max 65535 */
    ae_sns_dft->u32MinIntTimeTarget = ae_sns_dft->u32MinIntTime;

    ae_sns_dft->u32MaxAgain = 65536; /* max 65536 */
    ae_sns_dft->u32MinAgain = 1024; /* min 1024 */
    ae_sns_dft->u32MaxAgainTarget = ae_sns_dft->u32MaxAgain;
    ae_sns_dft->u32MinAgainTarget = ae_sns_dft->u32MinAgain;

    ae_sns_dft->u32MaxDgain = 32768; /* max 32768 */
    ae_sns_dft->u32MinDgain = 1024; /* min 1024 */
    ae_sns_dft->u32MaxDgainTarget = ae_sns_dft->u32MaxDgain;
    ae_sns_dft->u32MinDgainTarget = ae_sns_dft->u32MinDgain;
    ae_sns_dft->u8AeCompensation = 24; /* AeCompensation 24 */

    ae_sns_dft->u32InitExposure = g_init_exposure[vi_pipe] ? g_init_exposure[vi_pipe] : 16462; /* init 16462 */

    if (g_genFSWDRMode[vi_pipe] == ISP_FSWDR_LONG_FRAME_MODE) {
        ae_sns_dft->u8AeCompensation = 56; /* AeCompensation 56 */
        ae_sns_dft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;
    } else {
        ae_sns_dft->u32MaxISPDgainTarget = 1024; /* max 1024 */
        ae_sns_dft->enAeExpMode = AE_EXP_LOWLIGHT_PRIOR;
        ae_sns_dft->u16ManRatioEnable = HI_TRUE;
        ae_sns_dft->au32Ratio[0] = 0x100;
        ae_sns_dft->au32Ratio[1] = 0x40;
        ae_sns_dft->au32Ratio[2] = 0x40; /* index 2 */
    }
    ae_sns_dft->bAERouteExValid = g_ae_routeex_valid[vi_pipe];
    memcpy_s(&ae_sns_dft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), &g_init_ae_route[vi_pipe],
             sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&ae_sns_dft->stAERouteAttrEx, sizeof(ISP_AE_ROUTE_EX_S), &g_init_ae_routeex[vi_pipe],
             sizeof(ISP_AE_ROUTE_EX_S));
    memcpy_s(&ae_sns_dft->stAERouteSFAttrEx, sizeof(ISP_AE_ROUTE_EX_S), &g_init_ae_routesfex[vi_pipe],
             sizeof(ISP_AE_ROUTE_EX_S));
    return;
}

static HI_S32 cmos_get_ae_default(VI_PIPE vi_pipe, AE_SENSOR_DEFAULT_S *ae_sns_dft)
{
    ISP_SNS_STATE_S *sns_state = HI_NULL;

    CMOS_CHECK_POINTER(ae_sns_dft);
    ps5260_2l_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER(sns_state);

    memset_s(&ae_sns_dft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), 0, sizeof(ISP_AE_ROUTE_S));

    cmos_get_ae_comm_default(vi_pipe, ae_sns_dft, sns_state);

    switch (sns_state->enWDRMode) {
        default:
        case WDR_MODE_NONE:   /* linear mode */

            cmos_get_ae_linear_default(vi_pipe, ae_sns_dft, sns_state);

            break;

        case WDR_MODE_BUILT_IN:

            cmos_get_ae_builtinwdr_default(vi_pipe, ae_sns_dft, sns_state);

            break;
    }

    return HI_SUCCESS;
}

/* the function of sensor set fps */
static HI_VOID cmos_fps_set(VI_PIPE vi_pipe, HI_FLOAT fps, AE_SENSOR_DEFAULT_S *ae_sns_dft)
{
    ISP_SNS_STATE_S *sns_state = HI_NULL;
    HI_U32 lines, lines_max, vmax;
    HI_FLOAT max_fps, min_fps;

    CMOS_CHECK_POINTER_VOID(ae_sns_dft);
    ps5260_2l_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER_VOID(sns_state);

    lines = g_ps5260_2l_mode_tbl[sns_state->u8ImgMode].verti_lines;
    lines_max = g_ps5260_2l_mode_tbl[sns_state->u8ImgMode].max_verti_lines;
    max_fps = g_ps5260_2l_mode_tbl[sns_state->u8ImgMode].max_fps;
    min_fps = g_ps5260_2l_mode_tbl[sns_state->u8ImgMode].min_fps;

    if ((fps > max_fps) || (fps < min_fps)) {
        SNS_ERR_TRACE("Not support Fps: %f\n", fps);
        return;
    }

    vmax = lines * max_fps / SNS_DIV_0_TO_1_FLOAT(fps);
    vmax = (vmax > lines_max) ? lines_max : vmax;

    sns_state->u32FLStd = vmax;

    if (ae_sns_dft->u32FullLines != sns_state->u32FLStd) {
        psensor_queue_reset(e_QUEUE_FDC);
        g_temp_manual_c[vi_pipe] = (g_temp_manual_c[vi_pipe] * sns_state->u32FLStd) / ae_sns_dft->u32FullLines;
        psensor_queue_avg(e_QUEUE_TEMP, g_temp_manual_c[vi_pipe]);
    }

    ae_sns_dft->f32Fps = fps;
    ae_sns_dft->u32LinesPer500ms = lines * max_fps / 2; /* div 2 */
    ae_sns_dft->u32FullLinesStd = sns_state->u32FLStd;
    ae_sns_dft->u32MaxIntTime = sns_state->u32FLStd - 2; /* sub 2 */
    sns_state->au32FL[0] = sns_state->u32FLStd;
    ae_sns_dft->u32FullLines = sns_state->au32FL[0];
    sns_state->astRegsInfo[0].astI2cData[e_VMAX_ADDR_H].u32Data = HIGH_8BITS(sns_state->au32FL[0] - 1);
    sns_state->astRegsInfo[0].astI2cData[e_VMAX_ADDR_L].u32Data = LOW_8BITS(sns_state->au32FL[0] - 1);

    g_first_time_patch[vi_pipe] = HI_TRUE;
    g_fps[vi_pipe] = fps;
    g_fps_patch[vi_pipe] = 1;
    g_fps_patch_cnt[vi_pipe] = 1;
    g_vmax_target[vi_pipe] = vmax;
    g_vmax_ts[vi_pipe] = g_pre_int_total_time[vi_pipe];
    g_pre_int_total_time[vi_pipe] = vmax;
    if ((g_vmax_ts[vi_pipe] >> 1) > vmax) {
        g_fps_patch[vi_pipe] = 1;
    } else {
        g_fps_patch[vi_pipe] = 0;
    }
    return;
}

static HI_VOID cmos_slow_framerate_set(VI_PIPE vi_pipe, HI_U32 full_lines, AE_SENSOR_DEFAULT_S *ae_sns_dft)
{
    ISP_SNS_STATE_S *sns_state = HI_NULL;
    HI_FLOAT fps;

    CMOS_CHECK_POINTER_VOID(ae_sns_dft);
    ps5260_2l_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER_VOID(sns_state);

    if (sns_state->enWDRMode == WDR_MODE_BUILT_IN) {
        full_lines = (full_lines > PS5260_FULL_LINES_MAX_BUILDINWDR) ?
            PS5260_FULL_LINES_MAX_BUILDINWDR : full_lines;
        sns_state->au32FL[0] = (full_lines >> 1) << 1;
    } else {
        full_lines = (full_lines > PS5260_FULL_LINES_MAX) ? PS5260_FULL_LINES_MAX : full_lines;
        sns_state->au32FL[0] = full_lines;
    }

    ae_sns_dft->u32FullLines = sns_state->au32FL[0];
    ae_sns_dft->u32MaxIntTime = sns_state->au32FL[0] - 4; /* MaxIntTime: Flstd - 4 */
    sns_state->astRegsInfo[0].astI2cData[e_VMAX_ADDR_H].u32Data = HIGH_8BITS(sns_state->au32FL[0] - 1);
    sns_state->astRegsInfo[0].astI2cData[e_VMAX_ADDR_L].u32Data = LOW_8BITS(sns_state->au32FL[0] - 1);

    fps = 30.0 * PS5260_VMAX_1080P30_LINEAR / full_lines; /* max fps 30.0 */
    g_first_time_patch[vi_pipe] = HI_TRUE;
    g_fps[vi_pipe] = fps;

    return;
}

static HI_VOID cmos_fps_patch(VI_PIPE vi_pipe, HI_U32 int_time)
{
    HI_U32  int_time_tmp;
    HI_U32 int_nep, int_ne;
    ISP_SNS_STATE_S *sns_state = HI_NULL;

    ps5260_2l_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER_VOID(sns_state);

    int_ne  = 0;
    int_nep = 0;

    g_vmax_ts[vi_pipe] = (g_vmax_ts[vi_pipe] >> 1) + 4; /* add 4 */
    if (g_vmax_ts[vi_pipe] <= g_vmax_target[vi_pipe]) {
        g_fps_patch[vi_pipe] = 0;
        g_vmax_ts[vi_pipe] = g_vmax_target[vi_pipe];
    }
    g_fps_patch_cnt[vi_pipe] = 0;
    int_time_tmp = g_vmax_ts[vi_pipe] - int_time ;
    sns_state->astRegsInfo[0].astI2cData[e_VMAX_ADDR_H].u32Data = HIGH_8BITS(g_vmax_ts[vi_pipe]);
    sns_state->astRegsInfo[0].astI2cData[e_VMAX_ADDR_L].u32Data = LOW_8BITS(g_vmax_ts[vi_pipe]);
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_ADDR_H].u32Data = HIGH_8BITS(int_time_tmp);
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_ADDR_L].u32Data = LOW_8BITS(int_time_tmp);
    int_nep = NEPLS_LB + ((int_time_tmp * NEPLS_SCALE) >> 8); /* shift 8 */
    int_nep = (int_nep > NEPLS_LB) ? ((int_nep < NEPLS_UB) ? int_nep : NEPLS_UB) : NEPLS_LB;
    if (g_sensor_const[vi_pipe] > int_nep) {
        int_ne = g_sensor_const[vi_pipe] - int_nep;
    }
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_NE_ADDR_H].u32Data = ((int_ne & 0x1F00) >> 8); /* shift 8 */
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_NE_ADDR_L].u32Data = LOW_8BITS(int_ne);
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_NEP_ADDR_H].u32Data = ((int_nep & 0x0100) >> 6); /* shift 6 */
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_NEP_ADDR_L].u32Data = LOW_8BITS(int_nep);
    g_exp_time_patch[vi_pipe] = int_time_tmp;

    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static HI_VOID cmos_inttime_update(VI_PIPE vi_pipe, HI_U32 int_time)
{
    HI_U32  int_time_tmp, int_total_time;
    HI_U32 int_nep, int_ne;
    ISP_SNS_STATE_S *sns_state = HI_NULL;

    int_ne  = 0;
    int_nep = 0;

    ps5260_2l_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER_VOID(sns_state);

    if ((g_fps_patch[vi_pipe] == 1) && (g_fps_patch_cnt[vi_pipe] > 1)) {
        cmos_fps_patch(vi_pipe, int_time);
    } else {
        int_time_tmp = (int_time > PS5260_FULL_LINES_MAX) ? PS5260_FULL_LINES_MAX : int_time;
        int_total_time = sns_state->astRegsInfo[0].astI2cData[e_VMAX_ADDR_H].u32Data << 8; /* shift 8 */
        int_total_time = int_total_time + sns_state->astRegsInfo[0].astI2cData[e_VMAX_ADDR_L].u32Data;
        g_vmax_ts[vi_pipe] = (g_vmax_ts[vi_pipe] < (PS5260_VMAX_1080P30_LINEAR - 1)) ?
            (PS5260_VMAX_1080P30_LINEAR - 1) : g_vmax_ts[vi_pipe];
        int_total_time = (g_fps_patch[vi_pipe] == 1) ? g_vmax_ts[vi_pipe] : int_total_time;
        int_time_tmp = int_total_time - int_time_tmp ;
        if (int_time_tmp < PS5260_MIN_TIME) {
            int_time_tmp = PS5260_MIN_TIME;
        }

        sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_ADDR_H].u32Data = HIGH_8BITS(int_time_tmp);
        sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_ADDR_L].u32Data = LOW_8BITS(int_time_tmp);
        sns_state->astRegsInfo[0].astI2cData[e_VMAX_ADDR_H].u32Data = ((int_total_time & 0xFF00) >> 8); /* shift 8 */
        sns_state->astRegsInfo[0].astI2cData[e_VMAX_ADDR_L].u32Data = (int_total_time & 0xFF);
        int_nep = NEPLS_LB + ((int_time_tmp * NEPLS_SCALE) >> 8); /* shift 8 */
        int_nep = (int_nep > NEPLS_LB) ? ((int_nep < NEPLS_UB) ? int_nep : NEPLS_UB) : NEPLS_LB;
        if (g_sensor_const[vi_pipe] > int_nep) {
            int_ne = g_sensor_const[vi_pipe] - int_nep;
        }
        sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_NE_ADDR_H].u32Data = ((int_ne & 0x1F00) >> 8); /* shift 8 */
        sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_NE_ADDR_L].u32Data = LOW_8BITS(int_ne);
        sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_NEP_ADDR_H].u32Data = ((int_nep & 0x0100) >> 6); /* shift 6 */
        sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_NEP_ADDR_L].u32Data = LOW_8BITS(int_nep);
        g_exp_time_patch[vi_pipe] = int_time_tmp;
    }

    g_fps_patch_cnt[vi_pipe] = g_fps_patch_cnt[vi_pipe] > 1 ? 0 : g_fps_patch_cnt[vi_pipe];
    g_fps_patch_cnt[vi_pipe]++;

    return;
}

#define AD_GAIN_TBL_NUM 97
#define D_GAIN_TBL_NUM 81
static HI_U32 g_analog_gain_table[AD_GAIN_TBL_NUM] = {
    1024, 1088, 1152, 1216, 1280, 1344, 1408, 1472, 1536, 1600, 1664, 1728,
    1792, 1856, 1920, 1984, 2048, 2176, 2304, 2432, 2560, 2688, 2816, 2944,
    3072, 3200, 3328, 3456, 3584, 3712, 3840, 3968, 4096, 4352, 4608, 4864,
    5120, 5376, 5632, 5888, 6144, 6400, 6656, 6912, 7168, 7424, 7680, 7936,
    8192, 8704, 9216, 9728, 10240, 10752, 11264, 11776, 12288, 12800, 13312,
    13824, 14336, 14848, 15360, 15872, 16384, 17408, 18432, 19456, 20480, 21504,
    22528, 23552, 24576, 25600, 26624, 27648, 28672, 29696, 30720, 31744, 32768,
    34816, 36864, 38912, 40960, 43008, 45056, 47104, 49152, 51200, 53248, 55296,
    57344, 59392, 61440, 63488, 65536,
};

static HI_VOID cmos_again_calc_table(VI_PIPE vi_pipe, HI_U32 *again_lin, HI_U32 *again_db)
{
    int i;

    CMOS_CHECK_POINTER_VOID(again_lin);
    CMOS_CHECK_POINTER_VOID(again_db);

    if (*again_lin >= g_analog_gain_table[AD_GAIN_TBL_NUM - 1]) {
        *again_lin = g_analog_gain_table[AD_GAIN_TBL_NUM - 1];
        *again_db = AD_GAIN_TBL_NUM - 1;
        g_again_patch[vi_pipe] = *again_lin;
        return;
    }

    for (i = (1 + g_gain_min[vi_pipe]); i < AD_GAIN_TBL_NUM; i++) {
        if (*again_lin < g_analog_gain_table[i]) {
            *again_lin = g_analog_gain_table[i - 1];
            *again_db = i - 1;
            break;
        }
    }
    g_again_patch[vi_pipe] = *again_lin;

    return;
}

static HI_VOID cmos_dgain_calc_table(VI_PIPE vi_pipe, HI_U32 *dgain_lin, HI_U32 *dgain_db)
{
    int i;

    CMOS_CHECK_POINTER_VOID(dgain_lin);
    CMOS_CHECK_POINTER_VOID(dgain_db);

    if (*dgain_lin >= g_analog_gain_table[D_GAIN_TBL_NUM - 1]) {
        *dgain_lin = g_analog_gain_table[D_GAIN_TBL_NUM - 1];
        *dgain_db = D_GAIN_TBL_NUM - 1;
        return;
    }

    for (i = 1; i < D_GAIN_TBL_NUM; i++) {
        if (*dgain_lin < g_analog_gain_table[i]) {
            *dgain_lin = g_analog_gain_table[i - 1];
            *dgain_db = i - 1;
            break;
        }
    }
    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
#define AG_HS_NODE  (4*1024)
#define AG_LS_NODE  (3*1024)
static HI_VOID cmos_gains_update(VI_PIPE vi_pipe, HI_U32 again, HI_U32 dgain)
{
    ISP_SNS_STATE_S *sns_state = HI_NULL;

    ps5260_2l_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER_VOID(sns_state);

    sns_state->astRegsInfo[0].astI2cData[e_AGC_ADDR].u32Data = (again & 0xFF);
    sns_state->astRegsInfo[0].astI2cData[e_DGC_ADDR].u32Data = (dgain & 0xFF);
    sns_state->astRegsInfo[0].astI2cData[e_AGC_SGHD_ADDR].u32Data = 0;

    return;
}

static HI_VOID cmos_get_inttime_max(VI_PIPE vi_pipe, HI_U16 man_ratio_enable, HI_U32 *ratio,
    HI_U32 *int_time_max, HI_U32 *int_time_min, HI_U32 *lf_max_int_time)
{
    ISP_SNS_STATE_S *sns_state = HI_NULL;
    CMOS_CHECK_POINTER_VOID(ratio);
    CMOS_CHECK_POINTER_VOID(int_time_max);
    CMOS_CHECK_POINTER_VOID(int_time_min);
    CMOS_CHECK_POINTER_VOID(lf_max_int_time);
    ps5260_2l_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER_VOID(sns_state);

    switch (sns_state->enWDRMode) {
        case WDR_MODE_2To1_LINE:
            printf("WDR_MODE_2To1_LINE not support!\n");
            break;
        default:
            break;
    }

    return;
}

/* Only used in LINE_WDR mode */
static HI_VOID cmos_ae_fswdr_attr_set(VI_PIPE vi_pipe, AE_FSWDR_ATTR_S *ae_fswdr_attr)
{
    CMOS_CHECK_POINTER_VOID(ae_fswdr_attr);

    g_genFSWDRMode[vi_pipe] = ae_fswdr_attr->enFSWDRMode;
    return;
}

static HI_S32 cmos_init_ae_exp_function(AE_SENSOR_EXP_FUNC_S *exp_funcs)
{
    CMOS_CHECK_POINTER(exp_funcs);

    memset_s(exp_funcs, sizeof(AE_SENSOR_EXP_FUNC_S), 0, sizeof(AE_SENSOR_EXP_FUNC_S));

    exp_funcs->pfn_cmos_get_ae_default = cmos_get_ae_default;
    exp_funcs->pfn_cmos_fps_set = cmos_fps_set;
    exp_funcs->pfn_cmos_slow_framerate_set = cmos_slow_framerate_set;
    exp_funcs->pfn_cmos_inttime_update = cmos_inttime_update;
    exp_funcs->pfn_cmos_gains_update = cmos_gains_update;
    exp_funcs->pfn_cmos_again_calc_table = cmos_again_calc_table;
    exp_funcs->pfn_cmos_dgain_calc_table = cmos_dgain_calc_table;
    exp_funcs->pfn_cmos_get_inttime_max = cmos_get_inttime_max;
    exp_funcs->pfn_cmos_ae_fswdr_attr_set = cmos_ae_fswdr_attr_set;

    return HI_SUCCESS;
}

/* awb static param */
#define CALIBRATE_STATIC_TEMP                         4884
#define CALIBRATE_STATIC_WB_R_GAIN                    0x19C
#define CALIBRATE_STATIC_WB_GR_GAIN                   0x100
#define CALIBRATE_STATIC_WB_GB_GAIN                   0x100
#define CALIBRATE_STATIC_WB_B_GAIN                    0x1C6

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1                              103
#define CALIBRATE_AWB_P2                              82
#define CALIBRATE_AWB_Q1                              (-71)
#define CALIBRATE_AWB_A1                              166546
#define CALIBRATE_AWB_B1                              128
#define CALIBRATE_AWB_C1                              (-116032)

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0
static HI_S32 cmos_get_awb_default(VI_PIPE vi_pipe, AWB_SENSOR_DEFAULT_S *awb_sns_dft)
{
    ISP_SNS_STATE_S *sns_state = HI_NULL;
    HI_S32 i;

    CMOS_CHECK_POINTER(awb_sns_dft);
    ps5260_2l_sensor_get_ctx(vi_pipe, sns_state);
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
    awb_sns_dft->u16GoldenRgain = GOLDEN_RGAIN;
    awb_sns_dft->u16GoldenBgain = GOLDEN_BGAIN;

    switch (sns_state->enWDRMode) {
        default:
        case WDR_MODE_NONE:
            memcpy_s(&awb_sns_dft->stCcm, sizeof(AWB_CCM_S), &g_stAwbCcm, sizeof(AWB_CCM_S));
            memcpy_s(&awb_sns_dft->stAgcTbl, sizeof(AWB_AGC_TABLE_S), &g_stAwbAgcTable, sizeof(AWB_AGC_TABLE_S));
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

    switch (sns_state->u8ImgMode) {
        default:
        case PS5260_SENSOR_1080P_30FPS_LINEAR_MODE:
            def->stSensorMode.stDngRawFormat.u8BitsPerSample = 12; /* 12bit */
            def->stSensorMode.stDngRawFormat.u32WhiteLevel = 4095; /* max 4095 */
            break;

        case PS5260_SENSOR_1080P_30FPS_BuiltinWDR_MODE:
            def->stSensorMode.stDngRawFormat.u8BitsPerSample = 10; /* 10bit */
            def->stSensorMode.stDngRawFormat.u32WhiteLevel = 1023; /* max 1023 */
            break;
    }

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
    def->pstAntiFalseColor        = &g_stIspAntiFalseColor;
    def->unKey.bit1Ldci           = 1;
    def->pstLdci                  = &g_stIspLdci;
    def->unKey.bit1Dehaze = 1;
    def->pstDehaze = &g_stIspDehaze;
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
    def->stWdrSwitchAttr.au32ExpRatio[0] = 0x400;
    memcpy_s(&def->stNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S), &g_stIspNoiseCalibration,
             sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
    return;
}

static HI_S32 cmos_get_isp_default(VI_PIPE vi_pipe, ISP_CMOS_DEFAULT_S *def)
{
    ISP_SNS_STATE_S *sns_state = HI_NULL;

    CMOS_CHECK_POINTER(def);
    ps5260_2l_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER(sns_state);

    memset_s(def, sizeof(ISP_CMOS_DEFAULT_S), 0, sizeof(ISP_CMOS_DEFAULT_S));

#ifdef CONFIG_HI_ISP_CA_SUPPORT
    def->unKey.bit1Ca      = 1;
    def->pstCa             = &g_stIspCA;
#endif
    def->unKey.bit1Clut    = 1;
    def->pstClut           = &g_stIspCLUT;

    def->unKey.bit1Dpc     = 1;
    def->pstDpc            = &g_stCmosDpc;

    def->unKey.bit1Wdr     = 1;
    def->pstWdr            = &g_stIspWDR;

#ifdef CONFIG_HI_ISP_HLC_SUPPORT
    def->unKey.bit1Hlc      = 0;
    def->pstHlc             = &g_stIspHlc;
#endif
    def->unKey.bit1Lsc      = 1;
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
    }

    def->stSensorMode.u32SensorID = PS5260_ID;
    def->stSensorMode.u8SensorMode = sns_state->u8ImgMode;

    cmos_get_isp_dng_default(sns_state, def);

    return HI_SUCCESS;
}
static HI_S32 cmos_get_isp_black_level(VI_PIPE vi_pipe, ISP_CMOS_BLACK_LEVEL_S *black_level)
{
    HI_S32 i;
    ISP_SNS_STATE_S *sns_state = HI_NULL;

    CMOS_CHECK_POINTER(black_level);
    ps5260_2l_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER(sns_state);

    black_level->bUpdate = HI_FALSE;

    if (sns_state->enWDRMode == WDR_MODE_NONE) {
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            black_level->au16BlackLevel[i] = 0;
        }
    } else {
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            black_level->au16BlackLevel[i] = 0;
        }
    }

    return HI_SUCCESS;
}
static HI_VOID cmos_set_pixel_detect(VI_PIPE vi_pipe, HI_BOOL enable)
{
    HI_U32 full_lines_5fps, max_int_time_5fps;
    ISP_SNS_STATE_S *sns_state = HI_NULL;

    ps5260_2l_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER_VOID(sns_state);

    if (sns_state->enWDRMode == WDR_MODE_2To1_LINE) {
        return;
    } else {
        if (sns_state->u8ImgMode == PS5260_SENSOR_1080P_30FPS_LINEAR_MODE) {
            full_lines_5fps = (PS5260_VMAX_1080P30_LINEAR * 30) / 5; /* 30fps, 5fps */
        } else {
            return;
        }
    }

    max_int_time_5fps = full_lines_5fps - 4; /* max_int_time_5fps\ 4 */

    if (enable) { /* setup for ISP pixel calibration mode */
        ps5260_2l_write_register(vi_pipe, PS5260_BANK_REG, 0x01);
        ps5260_2l_write_register(vi_pipe, PS5260_AGC_ADDR, 0x00);
        ps5260_2l_write_register(vi_pipe, PS5260_DGC_ADDR, 0x00);

        ps5260_2l_write_register(vi_pipe, PS5260_VMAX_ADDR_H, HIGH_8BITS(full_lines_5fps));
        ps5260_2l_write_register(vi_pipe, PS5260_VMAX_ADDR_L, LOW_8BITS(full_lines_5fps));

        ps5260_2l_write_register(vi_pipe, PS5260_EXPOSURE_ADDR_H, HIGH_8BITS(max_int_time_5fps));
        ps5260_2l_write_register(vi_pipe, PS5260_EXPOSURE_ADDR_L, LOW_8BITS(max_int_time_5fps));
        ps5260_2l_write_register(vi_pipe, PS5260_BANK1_UDPDATE, 0x01);
    } else { /* setup for ISP 'normal mode' */
        sns_state->u32FLStd = (sns_state->u32FLStd > 0xFFFF) ? 0xFFFF : sns_state->u32FLStd;
        ps5260_2l_write_register(vi_pipe, PS5260_BANK_REG, 0x01);

        ps5260_2l_write_register(vi_pipe, PS5260_VMAX_ADDR_H, HIGH_8BITS(sns_state->u32FLStd));
        ps5260_2l_write_register(vi_pipe, PS5260_VMAX_ADDR_L, LOW_8BITS(sns_state->u32FLStd));

        ps5260_2l_write_register(vi_pipe, PS5260_BANK1_UDPDATE, 0x01);
        sns_state->bSyncInit = HI_FALSE;
    }

    return;
}

static HI_S32 cmos_set_wdr_mode(VI_PIPE vi_pipe, HI_U8 mode)
{
    ISP_SNS_STATE_S *sns_state = HI_NULL;

    ps5260_2l_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER(sns_state);

    sns_state->bSyncInit = HI_FALSE;
    switch (mode & 0x3F) {
        case WDR_MODE_NONE:
            sns_state->enWDRMode = WDR_MODE_NONE;
            printf("linear mode\n");
            break;

        case WDR_MODE_BUILT_IN:
            sns_state->enWDRMode = WDR_MODE_BUILT_IN;
            printf("BUILT IN WDR 1080p mode\n");
            break;

        default:
            SNS_ERR_TRACE("NOT support this mode!\n");
            return HI_FAILURE;
    }

    memset_s(sns_state->au32WDRIntTime, sizeof(sns_state->au32WDRIntTime), 0, sizeof(sns_state->au32WDRIntTime));

    return HI_SUCCESS;
}

#define G_TBL_ABC_LR_W 2
#define G_TBL_ABC_LR_H 32
HI_U16 g_tbl_abc_lr[G_TBL_ABC_LR_H][G_TBL_ABC_LR_W] = {
    {0, 0},
    {0, 0},
    {0, 1},
    {1, 3},
    {2, 6},
    {4, 9},
    {6, 14},
    {8, 19},
    {11, 25},
    {14, 32},
    {18, 39},
    {21, 48},
    {26, 57},
    {30, 67},
    {35, 78},
    {40, 89},
    {46, 102},
    {52, 115},
    {58, 129},
    {65, 144},
    {72, 159},
    {79, 175},
    {87, 193},
    {95, 211},
    {104, 229},
    {113, 249},
    {122, 269},
    {131, 290},
    {141, 312},
    {152, 335},
    {162, 359},
    {173, 383},
};

static HI_S32 patch_first(VI_PIPE vi_pipe, ISP_SNS_STATE_S *sns_state)
{
    HI_BOOL sg;
    HI_U16 fdc, fdc_abs, temp_new, temp_new_1, temp_new_2, temp_new_3, temp_new_4;
    HI_U16 sghd_gain = 0x23;
    HI_U32 lpf;
    HI_U32 int_time, sign;

    lpf = sns_state->au32FL[0];
    int_time = lpf - g_exp_time_patch[vi_pipe];
    ps5260_2l_write_register(vi_pipe, 0xEF, 0x00);
    sign = ps5260_2l_read_register(vi_pipe, 0xB7);
    fdc = ((sign & 0x0F) << 8); /* shift 8 */
    fdc += ps5260_2l_read_register(vi_pipe, 0xB8);
    sign = ps5260_2l_read_register(vi_pipe, 0xBB);
    fdc_abs = ((sign & 0x30) << 4); /* shift 4 */
    fdc_abs += ps5260_2l_read_register(vi_pipe, 0xBC);
    sign = ps5260_2l_read_register(vi_pipe, 0xBB);
    sg = (sign & 0x40) >> 6; /* shift 6 */

    temp_new = sg ? (fdc + fdc_abs) : ((fdc > fdc_abs) ? (fdc - fdc_abs) : 0);
    temp_new_1 = psensor_queue_add(e_QUEUE_FDC, temp_new);
    temp_new_2 = (lpf > (int_time << 4)) ? (int_time << 4) : lpf; /* shift 4 shift 4 */
    temp_new_3 = (sns_state->astRegsInfo[0].astI2cData[e_AGC_SGHD_ADDR].u32Data) ? sghd_gain : 0x10;
    temp_new_4 = int_time << 8; /* shift 8 */
    temp_new = temp_new_1 * temp_new_2 * temp_new_3 / temp_new_4;
    g_temp_manual_c[vi_pipe] = psensor_queue_add(e_QUEUE_TEMP, temp_new);
    return 0;
}

static HI_S32 patch_second(VI_PIPE vi_pipe, ISP_SNS_STATE_S *sns_state)
{
    HI_U8 cnt_s;
    HI_U32 sg;
    HI_U32 gain_report = g_again_patch[vi_pipe];

    sg = (g_temp_manual[vi_pipe] >> 3) + PS5260_MIN_AGAIN_IDX; /* shift 3 */
    g_gain_min[vi_pipe] = clip3(sg, PS5260_MIN_AGAIN_IDX, 16); /* clip 16 */
    if (sns_state->astRegsInfo[0].astI2cData[e_AGC_ADDR].u32Data < g_gain_min[vi_pipe]) {
        sns_state->astRegsInfo[0].astI2cData[e_AGC_ADDR].u32Data = g_gain_min[vi_pipe];
    }

    for (cnt_s = ABC_LRUB_MIN; cnt_s < (sizeof(g_tbl_abc_lr) / (2 * sizeof(HI_U16))); cnt_s++) { /* 2 * size */
        sg = sns_state->astRegsInfo[0].astI2cData[e_AGC_SGHD_ADDR].u32Data;
        if (g_temp_manual[vi_pipe] < g_tbl_abc_lr[cnt_s][sg]) {
            break;
        }
    }
    g_abclr[vi_pipe] = (cnt_s * ((gain_report >> (9)) + 63) + 32) >> 6; /* shift 9 \ add 63 \ add 32 \ shift 6 */
    return 0;
}

static HI_S32 cmos_get_sns_patch(VI_PIPE vi_pipe, ISP_SNS_STATE_S *sns_state)
{
    static HI_U16 cnt[ISP_MAX_PIPE_NUM] = {0};
    static HI_U16 pre_temp_manual[ISP_MAX_PIPE_NUM] = {0xFFFF};
    static HI_U8  lrub[ISP_MAX_PIPE_NUM] = {ABC_LRUB_MIN};
    HI_U32 fps;
    fps = g_fps[vi_pipe] * 100; /* 100 * fps */
    HI_U32 f = (fps >> 6) ? (fps >> 6) : 4; /* shift 6 \ shift 6 \ 4 */

    if (((cnt[vi_pipe]++) > f) || g_first_time_patch[vi_pipe]) {
        cnt[vi_pipe] = 0;
        if (g_first_time_patch[vi_pipe]) {
            pre_temp_manual[vi_pipe] = 0xFFFF;
            lrub[vi_pipe] = ABC_LRUB_MIN;
        } else {
            patch_first(vi_pipe, sns_state);
        }

        g_temp_manual[vi_pipe] = (g_temp_manual_c[vi_pipe] + 8) >> 4; /* add 8 \ shift 4 */
        ps5260_2l_write_register(vi_pipe, 0xEF, 0x06);
        ps5260_2l_write_register(vi_pipe, 0x45, 0xC0 | ((g_temp_manual[vi_pipe]>>8) & 0x07)); /* shift 8 */
        ps5260_2l_write_register(vi_pipe, 0x46, g_temp_manual[vi_pipe] & 0xFF);
        g_first_time_patch[vi_pipe] = HI_FALSE;
    } else if(cnt[vi_pipe] == 1) {
        if (pre_temp_manual[vi_pipe] == g_temp_manual[vi_pipe]) {
            patch_second(vi_pipe, sns_state);
        } else {
            g_abclr[vi_pipe] = g_abclr[vi_pipe] >> 1;
        }

        g_abclr[vi_pipe] = clip3(g_abclr[vi_pipe], ABC_LRUB_MIN, 31); /* clip 31 */
        pre_temp_manual[vi_pipe] = g_temp_manual[vi_pipe];
    } else if((cnt[vi_pipe] & 0x7) == 0) {
        if (lrub[vi_pipe] > g_abclr[vi_pipe]) {
            lrub[vi_pipe] -= (lrub[vi_pipe] - g_abclr[vi_pipe] + 1) >> 1;
        } else if(lrub[vi_pipe] < g_abclr[vi_pipe]) {
            lrub[vi_pipe]++;
        }

        ps5260_2l_write_register(vi_pipe, 0xEF, 0x02);
        ps5260_2l_write_register(vi_pipe, 0x2E, lrub[vi_pipe]);
        ps5260_2l_write_register(vi_pipe, 0x33, 0x80 | lrub[vi_pipe]);
        ps5260_2l_write_register(vi_pipe, 0xED, 0x01);
    } else {
    }

    return HI_SUCCESS;
}

static HI_VOID cmos_comm_sns_reg_info_init(VI_PIPE vi_pipe, ISP_SNS_STATE_S *sns_state)
{
    HI_S32 i;
    sns_state->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
    sns_state->astRegsInfo[0].unComBus.s8I2cDev = g_aunps5260_2l_BusInfo[vi_pipe].s8I2cDev;
    sns_state->astRegsInfo[0].u8Cfg2ValidDelayMax = 2; /* DelayMax 2 */
    sns_state->astRegsInfo[0].u32RegNum = e_REG_NUM_MAX;

    if (sns_state->enWDRMode == WDR_MODE_BUILT_IN) {
        sns_state->astRegsInfo[0].u32RegNum += 0;
        sns_state->astRegsInfo[0].u8Cfg2ValidDelayMax = 2; /* DelayMax 2 */
    }

    for (i = 0; i < sns_state->astRegsInfo[0].u32RegNum; i++) {
        sns_state->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
        sns_state->astRegsInfo[0].astI2cData[i].u8DevAddr = PS5260_2L_I2C_ADDR;
        sns_state->astRegsInfo[0].astI2cData[i].u32AddrByteNum = PS5260_2L_ADDR_BYTE;
        sns_state->astRegsInfo[0].astI2cData[i].u32DataByteNum = PS5260_2L_DATA_BYTE;
    }

    /* Bank 1 */
    sns_state->astRegsInfo[0].astI2cData[e_BANK_REG].u8DelayFrmNum = 0;
    sns_state->astRegsInfo[0].astI2cData[e_BANK_REG].u32RegAddr = PS5260_BANK_REG;
    sns_state->astRegsInfo[0].astI2cData[e_BANK_REG].u32Data = 1;

    /* Shutter (Shutter Long) */
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_ADDR_H].u8DelayFrmNum = 0;
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_ADDR_H].u32RegAddr = PS5260_EXPOSURE_ADDR_H;
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_ADDR_L].u8DelayFrmNum = 0;
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_ADDR_L].u32RegAddr = PS5260_EXPOSURE_ADDR_L;

    /* VMAX */
    sns_state->astRegsInfo[0].astI2cData[e_VMAX_ADDR_H].u8DelayFrmNum = 0;
    sns_state->astRegsInfo[0].astI2cData[e_VMAX_ADDR_H].u32RegAddr = PS5260_VMAX_ADDR_H;
    sns_state->astRegsInfo[0].astI2cData[e_VMAX_ADDR_L].u8DelayFrmNum = 0;
    sns_state->astRegsInfo[0].astI2cData[e_VMAX_ADDR_L].u32RegAddr = PS5260_VMAX_ADDR_L;

    /* AG */
    sns_state->astRegsInfo[0].astI2cData[e_AGC_ADDR].u8DelayFrmNum = 0;
    sns_state->astRegsInfo[0].astI2cData[e_AGC_ADDR].u32RegAddr = PS5260_AGC_ADDR;
    sns_state->astRegsInfo[0].astI2cData[e_AGC_SGHD_ADDR].u8DelayFrmNum = 0;
    sns_state->astRegsInfo[0].astI2cData[e_AGC_SGHD_ADDR].u32RegAddr = PS5260_AGC_SGHD_ADDR;

    /* DG */
    sns_state->astRegsInfo[0].astI2cData[e_DGC_ADDR].u8DelayFrmNum = 0;
    sns_state->astRegsInfo[0].astI2cData[e_DGC_ADDR].u32RegAddr = PS5260_DGC_ADDR;

    /* Bank1 Update */
    sns_state->astRegsInfo[0].astI2cData[e_BANK1_UDPDATE].u8DelayFrmNum = 0;
    sns_state->astRegsInfo[0].astI2cData[e_BANK1_UDPDATE].u32RegAddr = PS5260_BANK1_UDPDATE;
    sns_state->astRegsInfo[0].astI2cData[e_BANK1_UDPDATE].u32Data = 1;

    /* Patch */
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_NE_ADDR_H].u8DelayFrmNum = 0;
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_NE_ADDR_H].u32RegAddr = PS5260_EXPOSURE_NE_ADDR_H;
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_NE_ADDR_L].u8DelayFrmNum = 0;
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_NE_ADDR_L].u32RegAddr = PS5260_EXPOSURE_NE_ADDR_L;

    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_NEP_ADDR_H].u8DelayFrmNum = 0;
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_NEP_ADDR_H].u32RegAddr = PS5260_EXPOSURE_NEP_ADDR_H;
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_NEP_ADDR_L].u8DelayFrmNum = 0;
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_NEP_ADDR_L].u32RegAddr = PS5260_EXPOSURE_NEP_ADDR_L;

    return;
}
static HI_VOID cmos_builtin_sns_reg_info_init(VI_PIPE vi_pipe, ISP_SNS_STATE_S *sns_state)
{
    /* Bank 1 */
    sns_state->astRegsInfo[0].astI2cData[e_BANK_REG].u8DelayFrmNum = 0;
    sns_state->astRegsInfo[0].astI2cData[e_BANK_REG].u32RegAddr = PS5260_BANK_REG;
    sns_state->astRegsInfo[0].astI2cData[e_BANK_REG].u32Data = 1;

    /* Shutter (Shutter Long) */
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_ADDR_H].u8DelayFrmNum = 0;
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_ADDR_H].u32RegAddr = PS5260_EXPOSURE_ADDR_H;
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_ADDR_L].u8DelayFrmNum = 0;
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_ADDR_L].u32RegAddr = PS5260_EXPOSURE_ADDR_L;

    /* VMAX */
    sns_state->astRegsInfo[0].astI2cData[e_VMAX_ADDR_H].u8DelayFrmNum = 0;
    sns_state->astRegsInfo[0].astI2cData[e_VMAX_ADDR_H].u32RegAddr = PS5260_VMAX_ADDR_H;
    sns_state->astRegsInfo[0].astI2cData[e_VMAX_ADDR_L].u8DelayFrmNum = 0;
    sns_state->astRegsInfo[0].astI2cData[e_VMAX_ADDR_L].u32RegAddr = PS5260_VMAX_ADDR_L;

    /* AG */
    sns_state->astRegsInfo[0].astI2cData[e_AGC_ADDR].u8DelayFrmNum = 0;
    sns_state->astRegsInfo[0].astI2cData[e_AGC_ADDR].u32RegAddr = PS5260_AGC_ADDR;
    sns_state->astRegsInfo[0].astI2cData[e_AGC_SGHD_ADDR].u8DelayFrmNum = 0;
    sns_state->astRegsInfo[0].astI2cData[e_AGC_SGHD_ADDR].u32RegAddr = PS5260_AGC_SGHD_ADDR;

    /* DG */
    sns_state->astRegsInfo[0].astI2cData[e_DGC_ADDR].u8DelayFrmNum = 0;
    sns_state->astRegsInfo[0].astI2cData[e_DGC_ADDR].u32RegAddr = PS5260_DGC_ADDR;

    /* Bank1 Update */
    sns_state->astRegsInfo[0].astI2cData[e_BANK1_UDPDATE].u8DelayFrmNum = 0;
    sns_state->astRegsInfo[0].astI2cData[e_BANK1_UDPDATE].u32RegAddr = PS5260_BANK1_UDPDATE;
    sns_state->astRegsInfo[0].astI2cData[e_BANK1_UDPDATE].u32Data = 1;

    /* Patch */
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_NE_ADDR_H].u8DelayFrmNum = 0;
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_NE_ADDR_H].u32RegAddr = PS5260_EXPOSURE_NE_ADDR_H;
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_NE_ADDR_L].u8DelayFrmNum = 0;
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_NE_ADDR_L].u32RegAddr = PS5260_EXPOSURE_NE_ADDR_L;

    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_NEP_ADDR_H].u8DelayFrmNum = 0;
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_NEP_ADDR_H].u32RegAddr = PS5260_EXPOSURE_NEP_ADDR_H;
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_NEP_ADDR_L].u8DelayFrmNum = 0;
    sns_state->astRegsInfo[0].astI2cData[e_EXPOSURE_NEP_ADDR_L].u32RegAddr = PS5260_EXPOSURE_NEP_ADDR_L;

    return;
}
static HI_VOID cmos_sns_reg_info_update(VI_PIPE vi_pipe, ISP_SNS_STATE_S *sns_state)
{
    HI_S32 i;

    for (i = 1; i < sns_state->astRegsInfo[0].u32RegNum - 1; i++) {
        if (sns_state->astRegsInfo[0].astI2cData[i].u32Data == sns_state->astRegsInfo[1].astI2cData[i].u32Data) {
            sns_state->astRegsInfo[0].astI2cData[i].bUpdate = HI_FALSE;
        } else {
            sns_state->astRegsInfo[0].astI2cData[i].bUpdate = HI_TRUE;
        }
    }

    return;
}

static HI_S32 cmos_get_sns_regs_info(VI_PIPE vi_pipe, ISP_SNS_REGS_INFO_S *sns_regs_info)
{
    ISP_SNS_STATE_S *sns_state = HI_NULL;
    HI_S32 ret;

    CMOS_CHECK_POINTER(sns_regs_info);
    ps5260_2l_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER(sns_state);

    ret = cmos_get_sns_patch(vi_pipe, sns_state);
    if (ret != HI_SUCCESS) {
        printf("HI_MPI_ISP_QueryExposureInfo failed\n");
        return HI_FAILURE;
    }

    if ((sns_state->bSyncInit == HI_FALSE) || (sns_regs_info->bConfig == HI_FALSE)) {
        cmos_comm_sns_reg_info_init(vi_pipe, sns_state);
        /* WDR_MODE_BUILT_IN Mode Regs */
        if (sns_state->enWDRMode == WDR_MODE_BUILT_IN) {
            cmos_builtin_sns_reg_info_init(vi_pipe, sns_state);
        }
        sns_state->bSyncInit = HI_TRUE;
    } else {
        cmos_sns_reg_info_update(vi_pipe, sns_state);
    }

    sns_regs_info->bConfig = HI_FALSE;
    memcpy_s(sns_regs_info, sizeof(ISP_SNS_REGS_INFO_S), &sns_state->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S));
    memcpy_s(&sns_state->astRegsInfo[1], sizeof(ISP_SNS_REGS_INFO_S),
             &sns_state->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S));

    sns_state->au32FL[1] = sns_state->au32FL[0];

    return HI_SUCCESS;
}

static HI_S32 cmos_set_image_mode(VI_PIPE vi_pipe, ISP_CMOS_SENSOR_IMAGE_MODE_S *sensor_image_mode)
{
    HI_U8 image_mode;
    ISP_SNS_STATE_S *sns_state = HI_NULL;

    CMOS_CHECK_POINTER(sensor_image_mode);
    ps5260_2l_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER(sns_state);

    image_mode           = sns_state->u8ImgMode;
    sns_state->bSyncInit = HI_FALSE;

    if (sensor_image_mode->f32Fps <= 30) { /* 30fps */
        if (sns_state->enWDRMode == WDR_MODE_NONE) {
            if (PS5260_RES_IS_1080P(sensor_image_mode->u16Width, sensor_image_mode->u16Height)) {
                image_mode                  = PS5260_SENSOR_1080P_30FPS_LINEAR_MODE;
                sns_state->u32FLStd         = PS5260_VMAX_1080P30_LINEAR;
            } else {
                PS5260_2L_ERR_MODE_PRINT(sensor_image_mode, sns_state);
                return HI_FAILURE;
            }
        } else if (sns_state->enWDRMode == WDR_MODE_BUILT_IN) {
            if (PS5260_RES_IS_1080P(sensor_image_mode->u16Width, sensor_image_mode->u16Height)) {
                image_mode                   = PS5260_SENSOR_1080P_30FPS_BuiltinWDR_MODE;
                sns_state->u32FLStd          = PS5260_VMAX_1080P30_BuiltinWDR;
            } else {
                PS5260_2L_ERR_MODE_PRINT(sensor_image_mode, sns_state);
                return HI_FAILURE;
            }
        } else {
            PS5260_2L_ERR_MODE_PRINT(sensor_image_mode, sns_state);
            return HI_FAILURE;
        }
    } else {
    }

    if ((sns_state->bInit == HI_TRUE) && (image_mode == sns_state->u8ImgMode)) {
        /* Don't need to switch SensorImageMode */
        return ISP_DO_NOT_NEED_SWITCH_IMAGEMODE;
    }
    sns_state->u8ImgMode = image_mode;
    sns_state->au32FL[0] = sns_state->u32FLStd;
    sns_state->au32FL[1] = sns_state->au32FL[0];

    return HI_SUCCESS;
}

static HI_VOID sensor_global_init(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *sns_state = HI_NULL;

    ps5260_2l_sensor_get_ctx(vi_pipe, sns_state);
    CMOS_CHECK_POINTER_VOID(sns_state);

    sns_state->bInit = HI_FALSE;
    sns_state->bSyncInit = HI_FALSE;
    sns_state->u8ImgMode = PS5260_SENSOR_1080P_30FPS_LINEAR_MODE;
    sns_state->enWDRMode = WDR_MODE_NONE;
    sns_state->u32FLStd = PS5260_VMAX_1080P30_LINEAR;
    sns_state->au32FL[0] = PS5260_VMAX_1080P30_LINEAR;
    sns_state->au32FL[1] = PS5260_VMAX_1080P30_LINEAR;

    memset_s(&sns_state->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset_s(&sns_state->astRegsInfo[1], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
}

static HI_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *sensor_exp_func)
{
    CMOS_CHECK_POINTER(sensor_exp_func);

    memset_s(sensor_exp_func, sizeof(ISP_SENSOR_EXP_FUNC_S), 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    sensor_exp_func->pfn_cmos_sensor_init = ps5260_2l_init;
    sensor_exp_func->pfn_cmos_sensor_exit = ps5260_2l_exit;
    sensor_exp_func->pfn_cmos_sensor_global_init = sensor_global_init;
    sensor_exp_func->pfn_cmos_set_image_mode = cmos_set_image_mode;
    sensor_exp_func->pfn_cmos_set_wdr_mode = cmos_set_wdr_mode;

    sensor_exp_func->pfn_cmos_get_isp_default = cmos_get_isp_default;
    sensor_exp_func->pfn_cmos_get_isp_black_level = cmos_get_isp_black_level;
    sensor_exp_func->pfn_cmos_set_pixel_detect = cmos_set_pixel_detect;
    sensor_exp_func->pfn_cmos_get_sns_reg_info = cmos_get_sns_regs_info;

    return HI_SUCCESS;
}

/****************************************************************************
 * callback structure                                                       *
 ****************************************************************************/
static HI_S32 ps5260_2l_set_bus_info(VI_PIPE vi_pipe, ISP_SNS_COMMBUS_U sns_bus_info)
{
    g_aunps5260_2l_BusInfo[vi_pipe].s8I2cDev = sns_bus_info.s8I2cDev;

    return HI_SUCCESS;
}

static HI_S32 sensor_ctx_init(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *sns_state_ctx = HI_NULL;

    ps5260_2l_sensor_get_ctx(vi_pipe, sns_state_ctx);

    if (sns_state_ctx == HI_NULL) {
        sns_state_ctx = (ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
        if (sns_state_ctx == HI_NULL) {
            SNS_ERR_TRACE("Isp[%d] SnsCtx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset_s(sns_state_ctx, sizeof(ISP_SNS_STATE_S), 0, sizeof(ISP_SNS_STATE_S));

    ps5260_2l_sensor_set_ctx(vi_pipe, sns_state_ctx);

    return HI_SUCCESS;
}

static HI_VOID sensor_ctx_exit(VI_PIPE vi_pipe)
{
    ISP_SNS_STATE_S *sns_state_ctx = HI_NULL;

    ps5260_2l_sensor_get_ctx(vi_pipe, sns_state_ctx);
    SENSOR_FREE(sns_state_ctx);
    ps5260_2l_sensor_reset_ctx(vi_pipe);
    return;
}

static HI_S32 sensor_register_callback(VI_PIPE vi_pipe, ALG_LIB_S *ae_lib, ALG_LIB_S *awb_lib)
{
    HI_S32 ret;
    ISP_SENSOR_REGISTER_S isp_register;
    AE_SENSOR_REGISTER_S  ae_register;
    AWB_SENSOR_REGISTER_S awb_register;
    ISP_SNS_ATTR_INFO_S   sns_attr_info;

    CMOS_CHECK_POINTER(ae_lib);
    CMOS_CHECK_POINTER(awb_lib);

    ret = sensor_ctx_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        return HI_FAILURE;
    }

    sns_attr_info.eSensorId = PS5260_ID;
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

    ret = HI_MPI_ISP_SensorUnRegCallBack(vi_pipe, PS5260_ID);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function failed!\n");
        return ret;
    }

    ret = HI_MPI_AE_SensorUnRegCallBack(vi_pipe, ae_lib, PS5260_ID);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("sensor unregister callback function to ae lib failed!\n");
        return ret;
    }

    ret = HI_MPI_AWB_SensorUnRegCallBack(vi_pipe, awb_lib, PS5260_ID);
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

    g_init_exposure[vi_pipe]   = init_attr->u32Exposure;
    g_lines_per500ms[vi_pipe]  = init_attr->u32LinesPer500ms;
    g_init_wb_gain[vi_pipe][0] = init_attr->u16WBRgain; /* 0: Rgain */
    g_init_wb_gain[vi_pipe][1] = init_attr->u16WBGgain; /* 1: Ggain */
    g_init_wb_gain[vi_pipe][2] = init_attr->u16WBBgain; /* 2: Bgain */
    g_sample_rgain[vi_pipe]    = init_attr->u16SampleRgain;
    g_sample_bgain[vi_pipe]    = init_attr->u16SampleBgain;
    g_ae_routeex_valid[vi_pipe] = init_attr->bAERouteExValid;
    g_quick_start_en[vi_pipe] = init_attr->stQuickStart.bQuickStartEn;

    memcpy_s(&g_init_ae_route[vi_pipe], sizeof(ISP_AE_ROUTE_S), &init_attr->stAERoute, sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&g_init_ae_routeex[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S), &init_attr->stAERouteEx,
             sizeof(ISP_AE_ROUTE_EX_S));
    memcpy_s(&g_init_ae_routesf[vi_pipe], sizeof(ISP_AE_ROUTE_S), &init_attr->stAERouteSF, sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&g_init_ae_routesf[vi_pipe], sizeof(ISP_AE_ROUTE_S), &init_attr->stAERouteSF, sizeof(ISP_AE_ROUTE_S));
    memcpy_s(&g_init_ae_routesfex[vi_pipe], sizeof(ISP_AE_ROUTE_EX_S), &init_attr->stAERouteSFEx,
             sizeof(ISP_AE_ROUTE_EX_S));
    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        g_init_ccm[vi_pipe][i] = init_attr->au16CCM[i];
    }

    return HI_SUCCESS;
}

ISP_SNS_OBJ_S g_stSnsPs5260_2l_Obj = {
    .pfnRegisterCallback    = sensor_register_callback,
    .pfnUnRegisterCallback  = sensor_unregister_callback,
    .pfnStandby             = ps5260_2l_standby,
    .pfnRestart             = ps5260_2l_restart,
    .pfnMirrorFlip          = HI_NULL,
    .pfnWriteReg            = ps5260_2l_write_register,
    .pfnReadReg             = ps5260_2l_read_register,
    .pfnSetBusInfo          = ps5260_2l_set_bus_info,
    .pfnSetInit             = sensor_set_init
};

