/*
 * Copyright (c) GK. All rights reserved.
 */

#include "gc5603_cmos.h"
#include "gc5603_cmos_ex.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "gk_api_isp.h"
#include "gk_api_ae.h"
#include "gk_api_awb.h"

#define GC5603_ID   5603

#define HIGH_8BITS(x)   (((x) & 0xFF00) >> 8)
#define LOW_8BITS(x)    ((x) & 0x00FF)
#define LOWER_4BITS(x)  ((x) & 0x000F)
#define HIGHER_4BITS(x) (((x) & 0xF000) >> 12)
#define HIGHER_8BITS(x) (((x) & 0x0FF0) >> 4)
#define ISO_LUT_NUM     8

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ?  (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ?  (b) : (a))
#endif

ISP_SNS_STATE_S *g_pastGc5603[ISP_MAX_PIPE_NUM] = {GK_NULL};

#define GC5603_SENSOR_SET_CTX(dev, pstCtx)   ((g_pastGc5603[dev]) = (pstCtx))
#define GC5603_SENSOR_RESET_CTX(dev)         (g_pastGc5603[dev] = GK_NULL)

ISP_SNS_COMMBUS_U g_aunGc5603BusInfo[ISP_MAX_PIPE_NUM] = {
    [0] = { .s8I2cDev = 0 },
    [1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1 }
};

ISP_SNS_COMMBUS_U *gc5603_get_bus_Info(VI_PIPE ViPipe)
{
    return &g_aunGc5603BusInfo[ViPipe];
}

ISP_SNS_STATE_S *gc5603_get_ctx(VI_PIPE ViPipe)
{
    return g_pastGc5603[ViPipe];
}

static ISP_FSWDR_MODE_E g_genFSWDRMode[ISP_MAX_PIPE_NUM] = {
    [0 ... ISP_MAX_PIPE_NUM - 1] = ISP_FSWDR_NORMAL_MODE
};

static GK_U32 g_au32MaxTimeGetCnt[ISP_MAX_PIPE_NUM] = {0};
static GK_U32 g_au32InitExposure[ISP_MAX_PIPE_NUM]  = {0};
static GK_U32 g_au32LinesPer500ms[ISP_MAX_PIPE_NUM] = {0};
static GK_U32 g_au32InitIso[ISP_MAX_PIPE_NUM] = {[0 ... (ISP_MAX_PIPE_NUM - 1)] = 100}; /* init iso 100 */

static GK_U16 g_au16InitWBGain[ISP_MAX_PIPE_NUM][3] = {{0}};
static GK_U16 g_au16SampleRgain[ISP_MAX_PIPE_NUM] = {0};
static GK_U16 g_au16SampleBgain[ISP_MAX_PIPE_NUM] = {0};
static GK_U16 g_au16InitCCM[ISP_MAX_PIPE_NUM][CCM_MATRIX_SIZE] = {{0}};
static GK_BOOL g_abIsIrMode[ISP_MAX_PIPE_NUM] = {GK_FALSE};
static const GK_U32 g_au32IsoLut[ISO_LUT_NUM] = {100, 200, 400, 800, 1600, 3200, 6400, 12800};

static GK_BOOL g_abAERouteExValid[ISP_MAX_PIPE_NUM] = {0};
static ISP_AE_ROUTE_S g_astInitAERoute[ISP_MAX_PIPE_NUM] = {{0}};
static ISP_AE_ROUTE_EX_S g_astInitAERouteEx[ISP_MAX_PIPE_NUM] = {{0}};

#define GC5603_FULL_LINES_MAX                 0x3FFF
#define GC5603_INCREASE_LINES                 0 /* make real fps less than stand fps because NVR require */
#define GC5603_VMAX_2880x1620_LINEAR          (1750 + GC5603_INCREASE_LINES)
#define GC5603_SENSOR_FPS_MAX                 30
#define GC5603_SENSOR_FPS_MIN                 8
/* sensor fps mode */
#define GC5603_SENSOR_2880x1620_30FPS_LINEAR_MODE  0

static GK_VOID cmos_get_ae_comm_default(VI_PIPE ViPipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft,
    ISP_SNS_STATE_S *pstSnsState)
{
    (gk_void)memset_s(&pstAeSnsDft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), 0, sizeof(ISP_AE_ROUTE_S));

    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FlickerFreq = 50 * 256;  /* light flicker freq: 50Hz, accuracy: 256  */
    pstAeSnsDft->u32FullLinesMax = GC5603_FULL_LINES_MAX;

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

    pstAeSnsDft->bAERouteExValid = GK_FALSE;
    pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
    pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;

    if (g_au32LinesPer500ms[ViPipe] == 0) {
        pstAeSnsDft->u32LinesPer500ms = pstSnsState->u32FLStd * 30 / 2; /* 30fps, div 2 */
    } else {
        pstAeSnsDft->u32LinesPer500ms = g_au32LinesPer500ms[ViPipe];
    }

    return;
}

static GK_VOID cmos_get_ae_linear_default(VI_PIPE ViPipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft,
    ISP_SNS_STATE_S *pstSnsState)
{
    pstAeSnsDft->au8HistThresh[0] = 0x0D;
    pstAeSnsDft->au8HistThresh[1] = 0x28;
    pstAeSnsDft->au8HistThresh[2] = 0x60; /* index 2 */
    pstAeSnsDft->au8HistThresh[3] = 0x80; /* index 3 */

    pstAeSnsDft->u32MaxAgain = 65536; /* max 113168 */
    pstAeSnsDft->u32MinAgain = 1024; /* min 1024 */
    pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
    pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

    pstAeSnsDft->u32MaxDgain = 512; /* max 512 */
    pstAeSnsDft->u32MinDgain = 64; /* min 64 */
    pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
    pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

    pstAeSnsDft->u8AeCompensation = 0x38;
    pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;

    pstAeSnsDft->u32InitExposure = g_au32InitExposure[ViPipe] ? g_au32InitExposure[ViPipe] : 148859; /* init 148859 */
    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 2; /* sub 2 */
    pstAeSnsDft->u32MinIntTime = 1;
    pstAeSnsDft->u32MaxIntTimeTarget = 65535; /* max 65535 */
    pstAeSnsDft->u32MinIntTimeTarget = 1;
    pstAeSnsDft->bAERouteExValid = g_abAERouteExValid[ViPipe];
    (gk_void)memcpy_s(&pstAeSnsDft->stAERouteAttr, sizeof(ISP_AE_ROUTE_S), &g_astInitAERoute[ViPipe],
                      sizeof(ISP_AE_ROUTE_S));
    (gk_void)memcpy_s(&pstAeSnsDft->stAERouteAttrEx, sizeof(ISP_AE_ROUTE_EX_S), &g_astInitAERouteEx[ViPipe],
                      sizeof(ISP_AE_ROUTE_EX_S));
    return;
}

static GK_S32 cmos_get_ae_default(VI_PIPE ViPipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstAeSnsDft);
    GC5603_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    cmos_get_ae_comm_default(ViPipe, pstAeSnsDft, pstSnsState);

    switch (pstSnsState->enWDRMode) {
        case WDR_MODE_NONE:
            cmos_get_ae_linear_default(ViPipe, pstAeSnsDft, pstSnsState);
            break;
        default:
            printf("cmos_get_ae_default_Sensor Mode is error!\n");
            break;
    }

    return 0;
}

/* the function of sensor set fps */
static GK_VOID cmos_fps_set(VI_PIPE ViPipe, GK_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    GK_U32 u32FullLines;
    GK_U32 u32VMAX = GC5603_VMAX_2880x1620_LINEAR;
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    GC5603_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    switch (pstSnsState->u8ImgMode) {
        case GC5603_SENSOR_2880x1620_30FPS_LINEAR_MODE:

            if ((f32Fps <= 30) && (f32Fps > GC5603_SENSOR_FPS_MIN)) { /* fps range [2.06, 30] */
                u32VMAX = GC5603_VMAX_2880x1620_LINEAR * 30 / DIV_0_TO_1_FLOAT(f32Fps); /* 30 fps */
            } else {
                ISP_TRACE(MODULE_DBG_ERR, "Not support Fps: %f\n", f32Fps);
                return;
            }
            pstAeSnsDft->u32LinesPer500ms = GC5603_VMAX_2880x1620_LINEAR * 15 * 2; /* 15: 30fps / 2 */
            break;
        default:
            break;
    }
    u32VMAX = (u32VMAX > GC5603_FULL_LINES_MAX) ? GC5603_FULL_LINES_MAX : u32VMAX;
    //pstSnsState->u32FLStd = u32VMAX;
    pstSnsState->astRegsInfo[0].astI2cData[16].u32Data = ((u32VMAX & 0xFF00) >> 8); //340
    pstSnsState->astRegsInfo[0].astI2cData[17].u32Data = (u32VMAX & 0xFF);     //341

    u32FullLines = u32VMAX << 1;
    pstSnsState->u32FLStd = (u32FullLines > GC5603_FULL_LINES_MAX) ? GC5603_FULL_LINES_MAX : u32FullLines;
    pstAeSnsDft->f32Fps = f32Fps;

    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 2; /* sub 2 */
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];

    return;
}

static GK_VOID cmos_slow_framerate_set(VI_PIPE ViPipe, GK_U32 u32FullLines,
    AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    GC5603_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);
    u32FullLines = (u32FullLines > GC5603_FULL_LINES_MAX) ? GC5603_FULL_LINES_MAX : u32FullLines;
    pstSnsState->au32FL[0] = u32FullLines;


    pstSnsState->astRegsInfo[0].astI2cData[16].u32Data = ((u32FullLines & 0xFF00) >> 8); //340
    pstSnsState->astRegsInfo[0].astI2cData[17].u32Data = (u32FullLines & 0xFF);     //341

    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
    pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 2; /* sub 2 */

    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static GK_VOID cmos_inttime_update(VI_PIPE ViPipe, GK_U32 u32IntTime)
{
    GK_U32 u32Val;
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;
    GC5603_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32Val = (u32IntTime > GC5603_FULL_LINES_MAX) ? GC5603_FULL_LINES_MAX : u32IntTime;
    pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = ((u32Val & 0xFF00) >> 8); /* 202 */
    pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = (u32Val & 0xFF); /* 203 */

    return;
}

#define GC5603_REG_VAL_TAB_ROW    35
#define GC5603_REG_VAL_TAB_COL    9

static GK_U32 g_regValTable[GC5603_REG_VAL_TAB_ROW][GC5603_REG_VAL_TAB_COL] = {
   // 0614, 0615,  0225, 1447   1466, 1467, 1468, 00b8, 00b9
     { 0x00, 0x00,  0x00, 0x03,  0x18, 0x19, 0x19, 0x01, 0x00},
     { 0x90, 0x02,  0x00, 0x03,  0x18, 0x1b, 0x1b, 0x01, 0x0A},
     { 0x00, 0x00,  0x00, 0x01,  0x18, 0x19, 0x19, 0x01, 0x12},
     { 0x90, 0x02,  0x00, 0x01,  0x18, 0x1b, 0x1b, 0x01, 0x20},
     { 0x01, 0x00,  0x00, 0x01,  0x18, 0x19, 0x19, 0x01, 0x30},
     { 0x91, 0x02,  0x00, 0x01,  0x18, 0x1b, 0x1b, 0x02, 0x05},
     { 0x02, 0x00,  0x00, 0x01,  0x18, 0x1a, 0x1a, 0x02, 0x19},
     { 0x92, 0x02,  0x00, 0x01,  0x18, 0x1c, 0x1c, 0x02, 0x3F},
     { 0x03, 0x00,  0x00, 0x01,  0x18, 0x1a, 0x1a, 0x03, 0x20},
     { 0x93, 0x02,  0x00, 0x01,  0x18, 0x1d, 0x1d, 0x04, 0x0A}, //000
     { 0x00, 0x00,  0x01, 0x01,  0x18, 0x1d, 0x1d, 0x05, 0x02}, //
     { 0x90, 0x02,  0x01, 0x01,  0x18, 0x20, 0x20, 0x05, 0x39},
     { 0x01, 0x00,  0x01, 0x01,  0x18, 0x1e, 0x1e, 0x06, 0x3C}, //
     { 0x91, 0x02,  0x01, 0x01,  0x18, 0x20, 0x20, 0x08, 0x0D},
     { 0x02, 0x00,  0x01, 0x01,  0x18, 0x20, 0x20, 0x09, 0x21},
     { 0x92, 0x02,  0x01, 0x01,  0x18, 0x22, 0x22, 0x0B, 0x0F},
     { 0x03, 0x00,  0x01, 0x01,  0x18, 0x21, 0x21, 0x0D, 0x17},
     { 0x93, 0x02,  0x01, 0x01,  0x18, 0x23, 0x23, 0x0F, 0x33},
     { 0x04, 0x00,  0x01, 0x01,  0x18, 0x23, 0x23, 0x12, 0x30},
     { 0x94, 0x02,  0x01, 0x01,  0x18, 0x25, 0x25, 0x16, 0x10},
     { 0x05, 0x00,  0x01, 0x01,  0x18, 0x24, 0x24, 0x1A, 0x19},
     { 0x95, 0x02,  0x01, 0x01,  0x18, 0x26, 0x26, 0x1F, 0x13},
     { 0x06, 0x00,  0x01, 0x01,  0x18, 0x26, 0x26, 0x25, 0x08},
     { 0x96, 0x02,  0x01, 0x01,  0x18, 0x28, 0x28, 0x2C, 0x03},
     { 0xb6, 0x04,  0x01, 0x01,  0x18, 0x28, 0x28, 0x34, 0x0F},
     { 0x86, 0x06,  0x01, 0x01,  0x18, 0x2a, 0x2a, 0x3D, 0x3D},
     { 0x06, 0x08,  0x01, 0x01,  0x18, 0x2b, 0x2b, 0x49, 0x1F},
     { 0x4e, 0x09,  0x01, 0x01,  0x18, 0x2d, 0x2d, 0x57, 0x0A},
     { 0x5e, 0x0a,  0x01, 0x01,  0x18, 0x2f, 0x2f, 0x67, 0x19},
     { 0x3e, 0x0b,  0x01, 0x01,  0x18, 0x31, 0x31, 0x7A, 0x28},
     { 0x06, 0x0c,  0x01, 0x01,  0x18, 0x34, 0x34, 0x91, 0x1C},
     { 0xae, 0x0c,  0x01, 0x01,  0x18, 0x38, 0x38, 0xAC, 0x20},
     { 0x2e, 0x0d,  0x01, 0x01,  0x18, 0x3c, 0x3c, 0xCC, 0x26},
     { 0x9e, 0x0d,  0x01, 0x01,  0x18, 0x41, 0x41, 0xFF, 0x3F},
     { 0x06, 0x0e,  0x01, 0x01,  0x18, 0x47, 0x47, 0xFF, 0x3F},
};

#define GC5603_ANALOG_GAIN_TAB_RANGE    35
static GK_U32 g_analog_gain_table[GC5603_ANALOG_GAIN_TAB_RANGE] = {
    1024,
    1184,
    1312,
    1536,
    1792,
    2128,
    2448,
    3056,
    3584,
    4256,
    5152,
    6032,
    7104,
    8400,
    9744,
    11504,
    13680,
    16176,
    19200,
    22784,
    27024,
    32048,
    38016,
    45104,
    53488,
    63440,
    75248,
    89248,
    105872,
    125568,
    148928,
    176640,
    209504,
    248480,
    294704,
};

#define GC5603_ANALOG_GAIN_LUT_SIZE    35
static GK_VOID cmos_again_calc_table(VI_PIPE ViPipe, GK_U32 *pu32AgainLin, GK_U32 *pu32AgainDb)
{
    gk_u32 again;
    int i;
    static GK_U8 againmax = 28; /* adgain max 28 */

    CMOS_CHECK_POINTER_VOID(pu32AgainLin);
    CMOS_CHECK_POINTER_VOID(pu32AgainDb);

    again = *pu32AgainLin;

    if (again >= g_analog_gain_table[againmax]) {
        *pu32AgainLin = g_analog_gain_table[GC5603_ANALOG_GAIN_LUT_SIZE - 1];
        *pu32AgainDb = againmax;
    } else {
        for (i = 1; i < GC5603_ANALOG_GAIN_LUT_SIZE; i++) {
            if (again < g_analog_gain_table[i]) {
                *pu32AgainLin = g_analog_gain_table[i - 1];
                *pu32AgainDb = i - 1;
                break;
            }
        }
    }

    return;
}

static GK_VOID cmos_gains_update(VI_PIPE ViPipe, GK_U32 u32Again, GK_U32 u32Dgain)
{
    GK_U8 u8DgainHigh, u8DgainLow;

    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    GC5603_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u8DgainHigh = (u32Dgain >> 6) & 0x0f; /* shift 6 */
    u8DgainLow = (u32Dgain & 0x3f) << 2; /* shift 2 */

    pstSnsState->astRegsInfo[0].astI2cData[2].u32Data  = 0x2d; //31d
    pstSnsState->astRegsInfo[0].astI2cData[3].u32Data  = g_regValTable[u32Again][0]; //0614  3   //0x93, 0x02,  0x01, 0x01,  0x18, 0x23, 0x23, 0x11, 0x1F
    pstSnsState->astRegsInfo[0].astI2cData[4].u32Data  = g_regValTable[u32Again][1]; //0615 4
    pstSnsState->astRegsInfo[0].astI2cData[5].u32Data  = g_regValTable[u32Again][2]; //0225 5
    pstSnsState->astRegsInfo[0].astI2cData[6].u32Data  = 0x2e; //31d   6
    pstSnsState->astRegsInfo[0].astI2cData[7].u32Data  = g_regValTable[u32Again][3]; //1447  7
    pstSnsState->astRegsInfo[0].astI2cData[8].u32Data  = 0x28; //31d         8
    pstSnsState->astRegsInfo[0].astI2cData[9].u32Data  = g_regValTable[u32Again][4]; //1466  9
    pstSnsState->astRegsInfo[0].astI2cData[10].u32Data  = g_regValTable[u32Again][5]; //1467 10
    pstSnsState->astRegsInfo[0].astI2cData[11].u32Data  = g_regValTable[u32Again][6]; //1468 11

    pstSnsState->astRegsInfo[0].astI2cData[12].u32Data  = g_regValTable[u32Again][7]; //00b8 //12
    pstSnsState->astRegsInfo[0].astI2cData[13].u32Data  = g_regValTable[u32Again][8]; //00b9  //13

    pstSnsState->astRegsInfo[0].astI2cData[14].u32Data = u8DgainHigh; //0064  //14
    pstSnsState->astRegsInfo[0].astI2cData[15].u32Data = u8DgainLow; //0065    //15
    return;
}

static GK_VOID cmos_get_inttime_max(VI_PIPE ViPipe, GK_U16 u16ManRatioEnable,
    GK_U32 *au32Ratio, GK_U32 *au32IntTimeMax, GK_U32 *au32IntTimeMin, GK_U32 *pu32LFMaxIntTime)
{
    return;
}

/* Only used in LINE_WDR mode */
static GK_VOID cmos_ae_fswdr_attr_set(VI_PIPE ViPipe, AE_FSWDR_ATTR_S *pstAeFSWDRAttr)
{
    CMOS_CHECK_POINTER_VOID(pstAeFSWDRAttr);

    g_genFSWDRMode[ViPipe] = pstAeFSWDRAttr->enFSWDRMode;
    g_au32MaxTimeGetCnt[ViPipe] = 0;

    return;
}

static GK_S32 cmos_init_ae_exp_function(AE_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    CMOS_CHECK_POINTER(pstExpFuncs);

    (gk_void)memset_s(pstExpFuncs, sizeof(AE_SENSOR_EXP_FUNC_S), 0, sizeof(AE_SENSOR_EXP_FUNC_S));
    pstExpFuncs->pfn_cmos_get_ae_default    = cmos_get_ae_default;
    pstExpFuncs->pfn_cmos_fps_set           = cmos_fps_set;
    pstExpFuncs->pfn_cmos_slow_framerate_set = cmos_slow_framerate_set;
    pstExpFuncs->pfn_cmos_inttime_update    = cmos_inttime_update;
    pstExpFuncs->pfn_cmos_gains_update      = cmos_gains_update;
    pstExpFuncs->pfn_cmos_again_calc_table  = cmos_again_calc_table;
    pstExpFuncs->pfn_cmos_dgain_calc_table  = GK_NULL;
    pstExpFuncs->pfn_cmos_get_inttime_max   = cmos_get_inttime_max;
    pstExpFuncs->pfn_cmos_ae_fswdr_attr_set = cmos_ae_fswdr_attr_set;

    return GK_SUCCESS;
}

/* awb static param */
#define CALIBRATE_STATIC_TEMP                       5082
#define CALIBRATE_STATIC_WB_R_GAIN                  350//404
#define CALIBRATE_STATIC_WB_GR_GAIN                 256
#define CALIBRATE_STATIC_WB_GB_GAIN                 256
#define CALIBRATE_STATIC_WB_B_GAIN                  607//509

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1                            (-51)//13
#define CALIBRATE_AWB_P2                            293//213
#define CALIBRATE_AWB_Q1                            (-15)//(-30)
#define CALIBRATE_AWB_A1                            182819//186383
#define CALIBRATE_AWB_B1                            128
#define CALIBRATE_AWB_C1                            (-112549)//(-131654)
/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN                          0
#define GOLDEN_BGAIN                          0

static GK_S32 cmos_get_awb_default(VI_PIPE ViPipe, AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    GK_S32 i;

    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstAwbSnsDft);
    GC5603_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    (gk_void)memset_s(pstAwbSnsDft, sizeof(AWB_SENSOR_DEFAULT_S), 0, sizeof(AWB_SENSOR_DEFAULT_S));
    pstAwbSnsDft->u16WbRefTemp = CALIBRATE_STATIC_TEMP;

    pstAwbSnsDft->au16GainOffset[0] = CALIBRATE_STATIC_WB_R_GAIN;  /* 0: R channel 404 */
    pstAwbSnsDft->au16GainOffset[1] = CALIBRATE_STATIC_WB_GR_GAIN; /* 1: Gr channel 256 */
    pstAwbSnsDft->au16GainOffset[2] = CALIBRATE_STATIC_WB_GB_GAIN; /* 2: Gb channel 256 */
    pstAwbSnsDft->au16GainOffset[3] = CALIBRATE_STATIC_WB_B_GAIN;  /* 3: B channel 509 */

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
            (gk_void)memcpy_s(&pstAwbSnsDft->stCcm, sizeof(AWB_CCM_S), &g_stAwbCcm, sizeof(AWB_CCM_S));
            if (g_abIsIrMode[ViPipe] == GK_TRUE) {
                (gk_void)memcpy_s(&pstAwbSnsDft->stAgcTbl, sizeof(AWB_AGC_TABLE_S),
                                  &g_stAwbAgcTableIr, sizeof(AWB_AGC_TABLE_S));
            } else {
                (gk_void)memcpy_s(&pstAwbSnsDft->stAgcTbl, sizeof(AWB_AGC_TABLE_S),
                                  &g_stAwbAgcTable, sizeof(AWB_AGC_TABLE_S));
            }
            break;
    }
    pstAwbSnsDft->u16SampleRgain = g_au16SampleRgain[ViPipe];
    pstAwbSnsDft->u16SampleBgain = g_au16SampleBgain[ViPipe];
    pstAwbSnsDft->u16InitRgain = g_au16InitWBGain[ViPipe][0]; /* 0: Rgain */
    pstAwbSnsDft->u16InitGgain = g_au16InitWBGain[ViPipe][1]; /* 1: Ggain */
    pstAwbSnsDft->u16InitBgain = g_au16InitWBGain[ViPipe][2]; /* 2: Bgain */
    pstAwbSnsDft->u8AWBRunInterval = 4; /* RunInterval 4 */

    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        pstAwbSnsDft->au16InitCCM[i] = g_au16InitCCM[ViPipe][i];
    }

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

static GK_U32 Interpulate(GK_U32 u32Mid, GK_U32 u32Left, GK_U32 u32LValue, GK_U32 u32Right, GK_U32 u32RValue)
{
    GK_U32 u32Value;
    GK_U32 k;

    if (u32Mid <= u32Left) {
        u32Value = u32LValue;
        return u32Value;
    }

    if (u32Mid >= u32Right) {
        u32Value = u32RValue;
        return u32Value;
    }

    k = (u32Right - u32Left);
    u32Value = (((u32Right - u32Mid) * u32LValue + (u32Mid - u32Left) * u32RValue + (k >> 1)) / k);

    return u32Value;
}

static GK_VOID cmos_set_drc_init_str(VI_PIPE ViPipe)
{
    GK_U32 u32Level;
    GK_U32 au32DrcInitStr0[ISO_LUT_NUM] = { DRC_INIT_STR_NA };
    GK_U32 au32DrcInitStr1[ISO_LUT_NUM] = { DRC_INIT_STR_IR };

    for (u32Level = 0; u32Level < ISO_LUT_NUM; u32Level++) {
        if (g_au32InitIso[ViPipe] <= g_au32IsoLut[u32Level]) {
            break;
        }
    }

    if (u32Level == ISO_LUT_NUM) {
        u32Level = ISO_LUT_NUM - 1;
    }

    if (g_abIsIrMode[ViPipe] == GK_TRUE) {
        if ((u32Level == 0) || (u32Level == ISO_LUT_NUM - 1)) {
            g_stIspDRC.u16ManualStrength = au32DrcInitStr1[u32Level];
        } else {
            g_stIspDRC.u16ManualStrength = Interpulate(g_au32InitIso[ViPipe],
                                                       g_au32IsoLut[u32Level - 1], au32DrcInitStr1[u32Level - 1],
                                                       g_au32IsoLut[u32Level], au32DrcInitStr1[u32Level]);
        }
    } else {
        if ((u32Level == 0) || (u32Level == ISO_LUT_NUM - 1)) {
            g_stIspDRC.u16ManualStrength = au32DrcInitStr0[u32Level];
        } else {
            g_stIspDRC.u16ManualStrength = Interpulate(g_au32InitIso[ViPipe],
                                                       g_au32IsoLut[u32Level - 1], au32DrcInitStr0[u32Level - 1],
                                                       g_au32IsoLut[u32Level], au32DrcInitStr0[u32Level]);
        }
    }
}

static GK_VOID cmos_get_isp_dng_default(ISP_SNS_STATE_S *pstSnsState, ISP_CMOS_DEFAULT_S *pstDef)
{
    (gk_void)memcpy_s(&pstDef->stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S),
                      &g_stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S));

    switch (pstSnsState->u8ImgMode) {
        default:
        case GC5603_SENSOR_2880x1620_30FPS_LINEAR_MODE:
            pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 10; /* 10 bit  */
            pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 1023; /* max 1023  */
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
    pstDef->stSensorMode.bValidDngRawFormat = GK_TRUE;
    return;
}

static void cmos_get_isp_linear_default(VI_PIPE ViPipe, ISP_CMOS_DEFAULT_S *pstDef)
{
    pstDef->unKey.bit1Demosaic       = 1;
    pstDef->pstDemosaic              = &g_stIspDemosaic;
    pstDef->unKey.bit1Sharpen        = 1;
    pstDef->pstSharpen               = &g_stIspYuvSharpen;
    pstDef->unKey.bit1Drc            = 1;
    if (g_abIsIrMode[ViPipe] == GK_TRUE) {
        pstDef->pstDrc               = &g_stIspDRCIr;
    } else {
        pstDef->pstDrc               = &g_stIspDRC;
    }
    pstDef->unKey.bit1BayerNr        = 1;
    pstDef->pstBayerNr               = &g_stIspBayerNr;
    pstDef->unKey.bit1AntiFalseColor = 1;
    pstDef->pstAntiFalseColor        = &g_stIspAntiFalseColor;
    pstDef->unKey.bit1Ldci           = 1;
    pstDef->pstLdci                  = &g_stIspLdci;
    pstDef->unKey.bit1Gamma          = 1;
    if (g_abIsIrMode[ViPipe] == GK_TRUE) {
        pstDef->pstGamma             = &g_stIspGammaIr;
    } else {
        pstDef->pstGamma             = &g_stIspGamma;
    }
    pstDef->unKey.bit1Ge             = 1;
    pstDef->pstGe                    = &g_stIspGe;
    pstDef->unKey.bit1Dehaze         = 1;
    if (g_abIsIrMode[ViPipe] == GK_TRUE) {
        pstDef->pstDehaze            = &g_stIspDehazeIr;
    } else {
        pstDef->pstDehaze            = &g_stIspDehaze;
    }
    pstDef->unKey.bit1Lcac           = 1;
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
    GC5603_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);
    cmos_set_drc_init_str(ViPipe);
    (gk_void)memset_s(pstDef, sizeof(ISP_CMOS_DEFAULT_S), 0, sizeof(ISP_CMOS_DEFAULT_S));

    pstDef->unKey.bit1Ca       = 1;
    pstDef->pstCa              = &g_stIspCA;
    pstDef->unKey.bit1Dpc      = 1;
    pstDef->pstDpc             = &g_stCmosDpc;
    pstDef->unKey.bit1Wdr      = 1;
    pstDef->pstWdr             = &g_stIspWDR;
    pstDef->unKey.bit1Lsc      = 0;
    pstDef->pstLsc             = &g_stCmosLsc;
    pstDef->unKey.bit1PreGamma = 0;
    pstDef->pstPreGamma        = &g_stPreGamma;

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:
            cmos_get_isp_linear_default(ViPipe, pstDef);
            break;

    }

    pstDef->stSensorMode.u32SensorID = GC5603_ID;
    pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;
    cmos_get_isp_dng_default(pstSnsState, pstDef);

    return GK_SUCCESS;
}

static GK_S32 cmos_get_isp_black_level(VI_PIPE ViPipe, ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstBlackLevel);
    GC5603_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    /* Don't need to update black level when iso change */
    pstBlackLevel->bUpdate = GK_FALSE;
    pstBlackLevel->au16BlackLevel[0] = 256; /*  index 0 vaule 256  */
    pstBlackLevel->au16BlackLevel[1] = 256; /*  index 1 vaule 256  */
    pstBlackLevel->au16BlackLevel[2] = 256; /* index 2  vaule256 */
    pstBlackLevel->au16BlackLevel[3] = 256; /* index 3  vaule 256 */


    return GK_SUCCESS;
}

static GK_VOID cmos_set_pixel_detect(VI_PIPE ViPipe, GK_BOOL bEnable)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    GC5603_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);
    return;
}

static GK_S32 cmos_set_wdr_mode(VI_PIPE ViPipe, GK_U8 u8Mode)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    GC5603_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);
    pstSnsState->bSyncInit = GK_FALSE;

    switch (u8Mode) {
        case WDR_MODE_NONE:
            pstSnsState->enWDRMode = WDR_MODE_NONE;
            printf("GC5603_SENSOR_5M_30FPS_LINEAR_MODE\n");
            break;

        default:
            ISP_TRACE(MODULE_DBG_ERR, "cmos_set_wdr_mode_NOT support this mode!\n");
            return GK_FAILURE;
    }

    (gk_void)memset_s(pstSnsState->au32WDRIntTime,
                      sizeof(pstSnsState->au32WDRIntTime), 0, sizeof(pstSnsState->au32WDRIntTime));

    return GK_SUCCESS;
}

static GK_VOID cmos_comm_sns_reg_info_init(VI_PIPE ViPipe, ISP_SNS_STATE_S *pstSnsState)
{
    gk_u32 i;

    pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
    pstSnsState->astRegsInfo[0].unComBus.s8I2cDev = g_aunGc5603BusInfo[ViPipe].s8I2cDev;
    pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2; /*  u8Cfg2ValidDelayMax 3  */
    pstSnsState->astRegsInfo[0].u32RegNum = 18; /* RegNum 11 */

    for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
        pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = GK_TRUE;
        pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr = GC5603_I2C_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[i].u32AddrByteNum = GC5603_ADDR_BYTE;
        pstSnsState->astRegsInfo[0].astI2cData[i].u32DataByteNum = GC5603_DATA_BYTE;
    }
   // Linear Mode Regs
        pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr = 0x0202; //0

        pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr = 0x0203;//1

        pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum  = 1;
        pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr  = 0x031d;//2  //2d //1 frame

        pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum  = 1;
        pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr  = 0x0614; //614//3

        pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum  = 1;
        pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr  = 0x0615; //615//4

        pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr = 0x0225;//5

        pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum  = 1;
        pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr  = 0x031d;//6 //2e  //2 frame

        pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum  = 1;
        pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr  = 0x1447;//7

        pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum  = 1;
        pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr  = 0x031d;//8 //28 //liji

        pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr = 0x1466;//9

        pstSnsState->astRegsInfo[0].astI2cData[10].u8DelayFrmNum  = 1;
        pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr  = 0x1467;//10

        pstSnsState->astRegsInfo[0].astI2cData[11].u8DelayFrmNum  = 1;
        pstSnsState->astRegsInfo[0].astI2cData[11].u32RegAddr  = 0x1468;//11

        pstSnsState->astRegsInfo[0].astI2cData[12].u8DelayFrmNum  = 1;
        pstSnsState->astRegsInfo[0].astI2cData[12].u32RegAddr  = 0x00b8;//12

        pstSnsState->astRegsInfo[0].astI2cData[13].u8DelayFrmNum  = 1;
        pstSnsState->astRegsInfo[0].astI2cData[13].u32RegAddr  = 0x00b9;//13

        pstSnsState->astRegsInfo[0].astI2cData[14].u8DelayFrmNum  = 1;
        pstSnsState->astRegsInfo[0].astI2cData[14].u32RegAddr  = 0x0064; //64//14

        pstSnsState->astRegsInfo[0].astI2cData[15].u8DelayFrmNum  = 1;
        pstSnsState->astRegsInfo[0].astI2cData[15].u32RegAddr  = 0x0065; //65//15
	//vmax

        pstSnsState->astRegsInfo[0].astI2cData[16].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[16].u32RegAddr = 0x0340;//16

        pstSnsState->astRegsInfo[0].astI2cData[17].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[17].u32RegAddr = 0x0341;//17
    return;
}

static GK_VOID cmos_sns_reg_info_update(VI_PIPE ViPipe, ISP_SNS_STATE_S *pstSnsState)
{
    GK_U32 i;

    for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
        if (pstSnsState->astRegsInfo[0].astI2cData[i].u32Data ==
            pstSnsState->astRegsInfo[1].astI2cData[i].u32Data) {
            pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = GK_FALSE;
        } else {
            pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = GK_TRUE;
        }
    }

    return;
}

static GK_S32 cmos_get_sns_regs_info(VI_PIPE ViPipe, ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstSnsRegsInfo);
    GC5603_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    if ((pstSnsState->bSyncInit == GK_FALSE) || (pstSnsRegsInfo->bConfig == GK_FALSE)) {
        cmos_comm_sns_reg_info_init(ViPipe, pstSnsState);

        pstSnsState->bSyncInit = GK_TRUE;
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
    GC5603_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    u8SensorImageMode = pstSnsState->u8ImgMode;
    pstSnsState->bSyncInit = GK_FALSE;

    if ((pstSensorImageMode->u16Width <= 2960) && (pstSensorImageMode->u16Height <= 1664)) { /* w 1920   h 1080 */
        if (pstSensorImageMode->f32Fps > 30) { /*  30fps  */
            ISP_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, Fps:%f\n",
                          pstSensorImageMode->u16Width,
                          pstSensorImageMode->u16Height,
                          pstSensorImageMode->f32Fps);
            return GK_FAILURE;
        }

        if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
            u8SensorImageMode     = GC5603_SENSOR_2880x1620_30FPS_LINEAR_MODE;
            pstSnsState->u32FLStd = GC5603_VMAX_2880x1620_LINEAR;
        }  else {
            ISP_TRACE(MODULE_DBG_ERR, "Not support! \n");
        }
    } else {
        ISP_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, Fps:%f\n",
                      pstSensorImageMode->u16Width,
                      pstSensorImageMode->u16Height,
                      pstSensorImageMode->f32Fps);
        return GK_FAILURE;
    }

    if ((pstSnsState->bInit == GK_TRUE) && (u8SensorImageMode == pstSnsState->u8ImgMode)) {
        /* Don't need to switch SensorImageMode */
        return ISP_DO_NOT_NEED_SWITCH_IMAGEMODE;
    }
    pstSnsState->u8ImgMode = u8SensorImageMode;
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstSnsState->au32FL[1] = pstSnsState->au32FL[0];

    return GK_SUCCESS;
}

static GK_VOID sensor_global_init(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    GC5603_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->bInit     = GK_FALSE;
    pstSnsState->bSyncInit = GK_FALSE;
    pstSnsState->u8ImgMode = GC5603_SENSOR_2880x1620_30FPS_LINEAR_MODE;
    pstSnsState->enWDRMode = WDR_MODE_NONE;
    pstSnsState->u32FLStd = GC5603_VMAX_2880x1620_LINEAR * 2;
    pstSnsState->au32FL[0] = GC5603_VMAX_2880x1620_LINEAR * 2;
    pstSnsState->au32FL[1] = GC5603_VMAX_2880x1620_LINEAR * 2;

    (gk_void)memset_s(&pstSnsState->astRegsInfo[0], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
    (gk_void)memset_s(&pstSnsState->astRegsInfo[1], sizeof(ISP_SNS_REGS_INFO_S), 0, sizeof(ISP_SNS_REGS_INFO_S));
    return;
}

static GK_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    CMOS_CHECK_POINTER(pstSensorExpFunc);
    (gk_void)memset_s(pstSensorExpFunc, sizeof(ISP_SENSOR_EXP_FUNC_S), 0, sizeof(ISP_SENSOR_EXP_FUNC_S));
    pstSensorExpFunc->pfn_cmos_sensor_init = gc5603_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit = gc5603_exit;
    pstSensorExpFunc->pfn_cmos_sensor_global_init = sensor_global_init;
    pstSensorExpFunc->pfn_cmos_set_image_mode = cmos_set_image_mode;
    pstSensorExpFunc->pfn_cmos_set_wdr_mode = cmos_set_wdr_mode;

    pstSensorExpFunc->pfn_cmos_get_isp_default = cmos_get_isp_default;
    pstSensorExpFunc->pfn_cmos_get_isp_black_level = cmos_get_isp_black_level;
    pstSensorExpFunc->pfn_cmos_set_pixel_detect = cmos_set_pixel_detect;
    pstSensorExpFunc->pfn_cmos_get_sns_reg_info = cmos_get_sns_regs_info;

    return GK_SUCCESS;
}

static GK_S32 gc5603_set_bus_info(VI_PIPE ViPipe, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunGc5603BusInfo[ViPipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

    return GK_SUCCESS;
}

static GK_S32 sensor_ctx_init(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

    GC5603_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);

    if (pastSnsStateCtx == GK_NULL) {
        pastSnsStateCtx = (ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
        if (pastSnsStateCtx == GK_NULL) {
            ISP_TRACE(MODULE_DBG_ERR, "Isp[%d] SnsCtx malloc memory failed!\n", ViPipe);
            return ERR_CODE_ISP_NOMEM;
        }
    }

    (gk_void)memset_s(pastSnsStateCtx, sizeof(ISP_SNS_STATE_S), 0, sizeof(ISP_SNS_STATE_S));

    GC5603_SENSOR_SET_CTX(ViPipe, pastSnsStateCtx);

    return GK_SUCCESS;
}

static GK_VOID sensor_ctx_exit(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

    GC5603_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);
    SENSOR_FREE(pastSnsStateCtx);
    GC5603_SENSOR_RESET_CTX(ViPipe);
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

    stSnsAttrInfo.eSensorId = GC5603_ID;
    s32Ret = cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "cmos init exp function failed!\n");
        return GK_FAILURE;
    }
    s32Ret = GK_API_ISP_SensorRegCallBack(ViPipe, &stSnsAttrInfo, &stIspRegister);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor register callback function failed!\n");
        return s32Ret;
    }

    s32Ret = cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "cmos init ae exp function failed!\n");
        return GK_FAILURE;
    }
    s32Ret = GK_API_AE_SensorRegCallBack(ViPipe, pstAeLib, &stSnsAttrInfo, &stAeRegister);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor register callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "cmos init awb exp function failed!\n");
        return GK_FAILURE;
    }
    s32Ret = GK_API_AWB_SensorRegCallBack(ViPipe, pstAwbLib, &stSnsAttrInfo, &stAwbRegister);
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

    s32Ret = GK_API_ISP_SensorUnRegCallBack(ViPipe, GC5603_ID);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor unregister callback function failed!\n");
        return s32Ret;
    }

    s32Ret = GK_API_AE_SensorUnRegCallBack(ViPipe, pstAeLib, GC5603_ID);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = GK_API_AWB_SensorUnRegCallBack(ViPipe, pstAwbLib, GC5603_ID);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor unregister callback function to awb lib failed!\n");
        return s32Ret;
    }

    sensor_ctx_exit(ViPipe);

    return GK_SUCCESS;
}

static GK_S32 sensor_set_init(VI_PIPE ViPipe, ISP_INIT_ATTR_S *pstInitAttr)
{
    GK_S32 i;

    CMOS_CHECK_POINTER(pstInitAttr);

    if (pstInitAttr->u32InitIso) {
        g_au32InitIso[ViPipe]   = pstInitAttr->u32InitIso;
    }
    g_au32InitExposure[ViPipe]  = pstInitAttr->u32Exposure;
    g_au32LinesPer500ms[ViPipe] = pstInitAttr->u32LinesPer500ms;
    g_au16InitWBGain[ViPipe][0] = pstInitAttr->u16WBRgain; /* 0: Rgain */
    g_au16InitWBGain[ViPipe][1] = pstInitAttr->u16WBGgain; /* 1: Ggain */
    g_au16InitWBGain[ViPipe][2] = pstInitAttr->u16WBBgain; /* 2: Bgain */
    g_au16SampleRgain[ViPipe]   = pstInitAttr->u16SampleRgain;
    g_au16SampleBgain[ViPipe]   = pstInitAttr->u16SampleBgain;
    g_abIsIrMode[ViPipe]        = pstInitAttr->bIsIrMode;

    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        g_au16InitCCM[ViPipe][i] = pstInitAttr->au16CCM[i];
    }

    return GK_SUCCESS;
}

ISP_SNS_OBJ_S stSnsGc5603Obj = {
    .pfnRegisterCallback    = sensor_register_callback,
    .pfnUnRegisterCallback  = sensor_unregister_callback,
    .pfnStandby             = gc5603_standby,
    .pfnRestart             = gc5603_restart,
    .pfnMirrorFlip          = gc5603_mirror_flip,
    .pfnWriteReg            = gc5603_write_register,
    .pfnReadReg             = gc5603_read_register,
    .pfnSetBusInfo          = gc5603_set_bus_info,
    .pfnSetInit             = sensor_set_init
};

