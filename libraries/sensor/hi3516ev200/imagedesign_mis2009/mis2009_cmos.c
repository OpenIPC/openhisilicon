/*
 * (c) OpenIPC.org (c)
 *
 * ImageDesign MIS2009, 1080p30 linear RAW10 over 2-lane MIPI.
 *
 * Exposure, gain, frame rate and the ISP/AWB defaults follow the vendor
 * driver the stock firmware of the Zenointel SD-2N-4G (gk7205v510) runs
 * (`mis2009_cmos_*` in its `hunter`); mis2009_cmos_ex.h carries its tuning.
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "comm_sns.h"
#include "comm_video.h"
#include "sns_ctrl.h"
#include "gk_api_isp.h"
#include "gk_api_ae.h"
#include "gk_api_awb.h"
#include "mis2009_cmos_ex.h"
#include "hicompat.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define MIS2009_ID 2009
#define HIGH_8BITS(x) (((x)&0xFF00) >> 8)
#define LOW_8BITS(x) ((x)&0x00FF)

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ? (b) : (a))
#endif

ISP_SNS_STATE_S *g_pastMis2009[ISP_MAX_PIPE_NUM] = { GK_NULL };

#define MIS2009_SENSOR_GET_CTX(dev, pstCtx) (pstCtx = g_pastMis2009[dev])
#define MIS2009_SENSOR_SET_CTX(dev, pstCtx) (g_pastMis2009[dev] = pstCtx)
#define MIS2009_SENSOR_RESET_CTX(dev) (g_pastMis2009[dev] = GK_NULL)

ISP_SNS_COMMBUS_U g_aunMis2009BusInfo[ISP_MAX_PIPE_NUM] = { [0] = { .s8I2cDev = 0 },
                                [1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1 } };

static GK_U32 g_au32InitExposure[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U32 g_au32LinesPer500ms[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U16 g_au16InitWBGain[ISP_MAX_PIPE_NUM][3] = { { 0 } };
static GK_U16 g_au16SampleRgain[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U16 g_au16SampleBgain[ISP_MAX_PIPE_NUM] = { 0 };

/****************************************************************************
 * extern                                                                   *
 ****************************************************************************/
extern const unsigned char mis2009_i2c_addr;
extern const unsigned int mis2009_addr_byte;
extern const unsigned int mis2009_data_byte;

extern void mis2009_init(VI_PIPE ViPipe);
extern void mis2009_exit(VI_PIPE ViPipe);
extern void mis2009_standby(VI_PIPE ViPipe);
extern void mis2009_restart(VI_PIPE ViPipe);
extern int mis2009_write_register(VI_PIPE ViPipe, int addr, int data);
extern int mis2009_read_register(VI_PIPE ViPipe, int addr);

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
#define MIS2009_FULL_LINES_MAX_LINEAR (0xFFFF)

/******** MIS2009 Register Address ********/
#define MIS2009_EXP_ADDR (0x3100)   /* integration time, lines, 0x3100 high byte */
#define MIS2009_AGAIN_ADDR (0x3102) /* index into g_au32AgainTab */
#define MIS2009_VMAX_ADDR (0x3200)  /* frame length, lines, 0x3200 high byte */
#define MIS2009_ANA_GAIN_ADDR (0x3A02) /* analog settings that follow the gain ... */
#define MIS2009_ANA_GAIN2_ADDR (0x3A1C)
#define MIS2009_ANA_EXP_ADDR (0x3A07)  /* ... and the integration time */
#define MIS2009_FLIP_MIRROR_ADDR (0x3007)
#define MIS2009_WINDOW_ADDR (0x3204) /* row start/end, column start/end, 16 bit each */

/* the slots cmos_get_sns_regs_info hands the ISP, in the vendor's order */
enum {
    SLOT_EXP_H, SLOT_EXP_L, SLOT_AGAIN, SLOT_VMAX_H, SLOT_VMAX_L,
    SLOT_ANA_GAIN, SLOT_ANA_EXP, SLOT_ANA_GAIN2, SLOT_NUM
};

#define MIS2009_VMAX_1080P30_LINEAR (1126)
#define MIS2009_LINE_RATE (33780) /* lines per second: VMAX = MIS2009_LINE_RATE / fps */
#define EXP_OFFSET_LINEAR (2)
#define MIS2009_INTTIME_TARGET_MAX (1978)
#define MIS2009_INIT_EXPOSURE (148859)

/* below this gain index (2x) and outside this integration window the vendor
 * switches the analog settings in 0x3A02/0x3A1C and 0x3A07 */
#define MIS2009_GAIN_IDX_LOW (31)
#define MIS2009_INTTIME_SHORT (150)
#define MIS2009_INTTIME_VMAX_MARGIN (200)

//sensor fps mode
#define MIS2009_SENSOR_1080P_30FPS_LINEAR_MODE (1)

#define MIS2009_RES_IS_1080P(w, h) ((w) <= 1920 && (h) <= 1080)

#define MIS2009_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState)                                                    \
    do {                                                                                                       \
        ISP_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",                \
              pstSensorImageMode->u16Width, pstSensorImageMode->u16Height, pstSensorImageMode->f32Fps, \
              pstSnsState->enWDRMode);                                                                 \
    } while (0)

static GK_S32 cmos_get_ae_default(VI_PIPE ViPipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstAeSnsDft);
    MIS2009_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset(&pstAeSnsDft->stAERouteAttr, 0, sizeof(ISP_AE_ROUTE_S));

    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FlickerFreq = 50 * 256;
    pstAeSnsDft->u32FullLinesMax = MIS2009_FULL_LINES_MAX_LINEAR;

    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
    pstAeSnsDft->stIntTimeAccu.f32Offset = 0;

    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 1;

    /* no sensor digital gain: fixed at 1x */
    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 1.0f / 64;

    pstAeSnsDft->u32ISPDgainShift = 8;
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 8 << pstAeSnsDft->u32ISPDgainShift;

    pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_0;
    pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_32_0;

    pstAeSnsDft->bAERouteExValid = GK_FALSE;
    pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
    pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;

    if (g_au32LinesPer500ms[ViPipe] == 0) {
        pstAeSnsDft->u32LinesPer500ms = pstSnsState->u32FLStd * 30 / 2;
    } else {
        pstAeSnsDft->u32LinesPer500ms = g_au32LinesPer500ms[ViPipe];
    }

    if (pstSnsState->enWDRMode != WDR_MODE_NONE) {
        ISP_TRACE(MODULE_DBG_ERR, "Mode is error!\n");
        return GK_SUCCESS;
    }

    pstAeSnsDft->au8HistThresh[0] = 0x0D;
    pstAeSnsDft->au8HistThresh[1] = 0x28;
    pstAeSnsDft->au8HistThresh[2] = 0x60;
    pstAeSnsDft->au8HistThresh[3] = 0x80;
    pstAeSnsDft->u8AeCompensation = 0x40;

    pstAeSnsDft->u32MaxAgain = 0x3F00; /* 15.75x */
    pstAeSnsDft->u32MinAgain = 0x400;
    pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
    pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

    pstAeSnsDft->u32MaxDgain = 64;
    pstAeSnsDft->u32MinDgain = 64;
    pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
    pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

    pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;

    pstAeSnsDft->u32InitExposure = g_au32InitExposure[ViPipe] ? g_au32InitExposure[ViPipe] : MIS2009_INIT_EXPOSURE;

    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - EXP_OFFSET_LINEAR;
    pstAeSnsDft->u32MinIntTime = 1;
    pstAeSnsDft->u32MaxIntTimeTarget = MIS2009_INTTIME_TARGET_MAX;
    pstAeSnsDft->u32MinIntTimeTarget = 1;

    return GK_SUCCESS;
}

static GK_VOID cmos_set_vmax(ISP_SNS_STATE_S *pstSnsState, GK_U32 u32FullLines)
{
    pstSnsState->au32FL[0] = u32FullLines;
    pstSnsState->astRegsInfo[0].astI2cData[SLOT_VMAX_H].u32Data = HIGH_8BITS(u32FullLines);
    pstSnsState->astRegsInfo[0].astI2cData[SLOT_VMAX_L].u32Data = LOW_8BITS(u32FullLines);
}

/* the function of sensor set fps */
static GK_VOID cmos_fps_set(VI_PIPE ViPipe, GK_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;
    GK_U32 u32FullLines;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    MIS2009_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (pstSnsState->u8ImgMode != MIS2009_SENSOR_1080P_30FPS_LINEAR_MODE) {
        ISP_TRACE(MODULE_DBG_ERR, "Not support this Mode\n");
        return;
    }
    if ((f32Fps > 30) || (f32Fps < 2.75)) {
        ISP_TRACE(MODULE_DBG_ERR, "Not support Fps: %f\n", f32Fps);
        return;
    }

    u32FullLines = MIS2009_LINE_RATE / DIV_0_TO_1_FLOAT(f32Fps);
    u32FullLines = MIN(u32FullLines, MIS2009_FULL_LINES_MAX_LINEAR);

    pstSnsState->u32FLStd = u32FullLines;
    cmos_set_vmax(pstSnsState, u32FullLines);

    pstAeSnsDft->f32Fps = f32Fps;
    pstAeSnsDft->u32LinesPer500ms = f32Fps * u32FullLines / 2;
    pstAeSnsDft->u32FullLinesStd = u32FullLines;
    pstAeSnsDft->u32FullLines = u32FullLines;
    pstAeSnsDft->u32MaxIntTime = u32FullLines - EXP_OFFSET_LINEAR;
}

static GK_VOID cmos_slow_framerate_set(VI_PIPE ViPipe, GK_U32 u32FullLines, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    MIS2009_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32FullLines = MIN(u32FullLines, MIS2009_FULL_LINES_MAX_LINEAR);
    cmos_set_vmax(pstSnsState, u32FullLines);

    pstAeSnsDft->u32FullLines = u32FullLines;
    pstAeSnsDft->u32MaxIntTime = u32FullLines - EXP_OFFSET_LINEAR;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static GK_VOID cmos_inttime_update(VI_PIPE ViPipe, GK_U32 u32IntTime)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;
    GK_U32 u32Vmax;
    GK_BOOL bMid;

    MIS2009_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->astRegsInfo[0].astI2cData[SLOT_EXP_H].u32Data = HIGH_8BITS(u32IntTime);
    pstSnsState->astRegsInfo[0].astI2cData[SLOT_EXP_L].u32Data = LOW_8BITS(u32IntTime);

    /* The vendor reads VMAX back from the sensor here, every frame; the
     * value it reads is the one this state last queued. */
    u32Vmax = pstSnsState->au32FL[0];
    bMid = (u32IntTime > MIS2009_INTTIME_SHORT) && (u32Vmax > MIS2009_INTTIME_VMAX_MARGIN) &&
           (u32IntTime < u32Vmax - MIS2009_INTTIME_VMAX_MARGIN);
    pstSnsState->astRegsInfo[0].astI2cData[SLOT_ANA_EXP].u32Data = bMid ? 0x4C : 0xCC;
}

/* 1x .. 15.75x, 1/32 steps per octave: 0x400 + 0x20*i, 0x800 + 0x40*i, ... */
#define MIS2009_AGAIN_NUM (128)
static GK_U32 g_au32AgainTab[MIS2009_AGAIN_NUM];

static GK_VOID cmos_again_table_init(GK_VOID)
{
    GK_U32 i;

    for (i = 0; i < MIS2009_AGAIN_NUM; i++) {
        g_au32AgainTab[i] = (0x400 << (i / 32)) + (0x20 << (i / 32)) * (i % 32);
    }
}

static GK_VOID cmos_again_calc_table(VI_PIPE ViPipe, GK_U32 *pu32AgainLin, GK_U32 *pu32AgainDb)
{
    GK_U32 i;

    CMOS_CHECK_POINTER_VOID(pu32AgainLin);
    CMOS_CHECK_POINTER_VOID(pu32AgainDb);

    if (g_au32AgainTab[0] == 0) {
        cmos_again_table_init();
    }

    /* the largest entry not above the request */
    for (i = 1; i < MIS2009_AGAIN_NUM; i++) {
        if (*pu32AgainLin < g_au32AgainTab[i]) {
            break;
        }
    }
    *pu32AgainLin = g_au32AgainTab[i - 1];
    *pu32AgainDb = i - 1;
}

static GK_VOID cmos_gains_update(VI_PIPE ViPipe, GK_U32 u32Again, GK_U32 u32Dgain)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;
    GK_BOOL bLow = (u32Again <= MIS2009_GAIN_IDX_LOW);

    MIS2009_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->astRegsInfo[0].astI2cData[SLOT_AGAIN].u32Data = LOW_8BITS(u32Again);
    pstSnsState->astRegsInfo[0].astI2cData[SLOT_ANA_GAIN].u32Data = bLow ? 0x0B : 0x0A;
    pstSnsState->astRegsInfo[0].astI2cData[SLOT_ANA_GAIN2].u32Data = bLow ? 0x1F : 0x30;
}

static GK_S32 cmos_init_ae_exp_function(AE_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    CMOS_CHECK_POINTER(pstExpFuncs);

    memset(pstExpFuncs, 0, sizeof(AE_SENSOR_EXP_FUNC_S));

    pstExpFuncs->pfn_cmos_get_ae_default = cmos_get_ae_default;
    pstExpFuncs->pfn_cmos_fps_set = cmos_fps_set;
    pstExpFuncs->pfn_cmos_slow_framerate_set = cmos_slow_framerate_set;
    pstExpFuncs->pfn_cmos_inttime_update = cmos_inttime_update;
    pstExpFuncs->pfn_cmos_gains_update = cmos_gains_update;
    pstExpFuncs->pfn_cmos_again_calc_table = cmos_again_calc_table;

    return GK_SUCCESS;
}

static GK_S32 cmos_get_awb_default(VI_PIPE ViPipe, AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    static const GK_U16 au16GainOffset[] = MIS2009_AWB_GAIN_OFFSET;
    static const GK_S32 as32WbPara[] = MIS2009_AWB_WB_PARA;
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstAwbSnsDft);
    MIS2009_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));
    pstAwbSnsDft->u16WbRefTemp = MIS2009_AWB_REF_TEMP;
    memcpy(pstAwbSnsDft->au16GainOffset, au16GainOffset, sizeof(au16GainOffset));
    memcpy(pstAwbSnsDft->as32WbPara, as32WbPara, sizeof(as32WbPara));

    memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcm, sizeof(AWB_CCM_S));
    memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTable, sizeof(AWB_AGC_TABLE_S));

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

static const ISP_CMOS_DNG_COLORPARAM_S g_stDngColorParam = { { 378, 256, 430 }, { 439, 256, 439 } };

static GK_S32 cmos_get_isp_default(VI_PIPE ViPipe, ISP_CMOS_DEFAULT_S *pstDef)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstDef);
    MIS2009_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset(pstDef, 0, sizeof(ISP_CMOS_DEFAULT_S));

    /* the vendor's modules: no LSC, LCAC or pre-gamma */
    pstDef->unKey.bit1Drc = 1;
    pstDef->pstDrc = &g_stIspDRC;
    pstDef->unKey.bit1Demosaic = 1;
    pstDef->pstDemosaic = &g_stIspDemosaic;
    pstDef->unKey.bit1Gamma = 1;
    pstDef->pstGamma = &g_stIspGamma;
    pstDef->unKey.bit1Sharpen = 1;
    pstDef->pstSharpen = &g_stIspYuvSharpen;
    pstDef->unKey.bit1Ldci = 1;
    pstDef->pstLdci = &g_stIspLdci;
    pstDef->unKey.bit1Dpc = 1;
    pstDef->pstDpc = &g_stCmosDpc;
    pstDef->unKey.bit1Ge = 1;
    pstDef->pstGe = &g_stIspGe;
    pstDef->unKey.bit1AntiFalseColor = 1;
    pstDef->pstAntiFalseColor = &g_stIspAntiFalseColor;
    pstDef->unKey.bit1BayerNr = 1;
    pstDef->pstBayerNr = &g_stIspBayerNr;
    pstDef->unKey.bit1Ca = 1;
    pstDef->pstCa = &g_stIspCA;
    pstDef->unKey.bit1Wdr = 1;
    pstDef->pstWdr = &g_stIspWDR;
    pstDef->unKey.bit1Dehaze = 1;
    pstDef->pstDehaze = &g_stIspDehaze;
    memcpy(&pstDef->stNoiseCalibration, &g_stIspNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S));

    pstDef->stSensorMode.u32SensorID = MIS2009_ID;
    pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

    memcpy(&pstDef->stDngColorParam, &g_stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S));

    pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 10;
    pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 1023;
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
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstBlackLevel);
    MIS2009_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    /* Don't need to update black level when iso change */
    pstBlackLevel->bUpdate = GK_FALSE;

    /* black level of linear mode */
    pstBlackLevel->au16BlackLevel[0] = 64;
    pstBlackLevel->au16BlackLevel[1] = 64;
    pstBlackLevel->au16BlackLevel[2] = 64;
    pstBlackLevel->au16BlackLevel[3] = 64;

    return GK_SUCCESS;
}

static GK_VOID cmos_set_pixel_detect(VI_PIPE ViPipe, GK_BOOL bEnable)
{
    GK_U32 u32FullLines_5Fps, u32MaxIntTime_5Fps;
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    MIS2009_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (MIS2009_SENSOR_1080P_30FPS_LINEAR_MODE != pstSnsState->u8ImgMode) {
        return;
    }

    u32FullLines_5Fps = MIS2009_LINE_RATE / 5;
    u32MaxIntTime_5Fps = u32FullLines_5Fps - EXP_OFFSET_LINEAR;

    if (bEnable) { /* setup for ISP pixel calibration mode */
        mis2009_write_register(ViPipe, MIS2009_VMAX_ADDR, HIGH_8BITS(u32FullLines_5Fps)); /* 5fps */
        mis2009_write_register(ViPipe, MIS2009_VMAX_ADDR + 1, LOW_8BITS(u32FullLines_5Fps));
        mis2009_write_register(ViPipe, MIS2009_EXP_ADDR, HIGH_8BITS(u32MaxIntTime_5Fps)); /* max exposure lines */
        mis2009_write_register(ViPipe, MIS2009_EXP_ADDR + 1, LOW_8BITS(u32MaxIntTime_5Fps));
        mis2009_write_register(ViPipe, MIS2009_AGAIN_ADDR, 0x00); /* 1x */
        mis2009_write_register(ViPipe, MIS2009_ANA_GAIN_ADDR, 0x0B);
        mis2009_write_register(ViPipe, MIS2009_ANA_GAIN2_ADDR, 0x1F);
    } else { /* setup for ISP 'normal mode' */
        pstSnsState->u32FLStd = MIN(pstSnsState->u32FLStd, MIS2009_FULL_LINES_MAX_LINEAR);
        pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
        mis2009_write_register(ViPipe, MIS2009_VMAX_ADDR, HIGH_8BITS(pstSnsState->u32FLStd));
        mis2009_write_register(ViPipe, MIS2009_VMAX_ADDR + 1, LOW_8BITS(pstSnsState->u32FLStd));
        pstSnsState->bSyncInit = GK_FALSE;
    }
}

static GK_S32 cmos_get_sns_regs_info(VI_PIPE ViPipe, ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
    static const GK_U32 au32SlotAddr[SLOT_NUM] = {
        [SLOT_EXP_H] = MIS2009_EXP_ADDR,       [SLOT_EXP_L] = MIS2009_EXP_ADDR + 1,
        [SLOT_AGAIN] = MIS2009_AGAIN_ADDR,     [SLOT_VMAX_H] = MIS2009_VMAX_ADDR,
        [SLOT_VMAX_L] = MIS2009_VMAX_ADDR + 1, [SLOT_ANA_GAIN] = MIS2009_ANA_GAIN_ADDR,
        [SLOT_ANA_EXP] = MIS2009_ANA_EXP_ADDR, [SLOT_ANA_GAIN2] = MIS2009_ANA_GAIN2_ADDR,
    };
    GK_U32 i;
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstSnsRegsInfo);
    MIS2009_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    if ((pstSnsState->bSyncInit == GK_FALSE) || (pstSnsRegsInfo->bConfig == GK_FALSE)) {
        pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
        pstSnsState->astRegsInfo[0].unComBus.s8I2cDev = g_aunMis2009BusInfo[ViPipe].s8I2cDev;
        pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;
        pstSnsState->astRegsInfo[0].u32RegNum = SLOT_NUM;

        for (i = 0; i < SLOT_NUM; i++) {
            pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = GK_TRUE;
            pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr = mis2009_i2c_addr;
            pstSnsState->astRegsInfo[0].astI2cData[i].u32AddrByteNum = mis2009_addr_byte;
            pstSnsState->astRegsInfo[0].astI2cData[i].u32DataByteNum = mis2009_data_byte;
            pstSnsState->astRegsInfo[0].astI2cData[i].u8DelayFrmNum = 0;
            pstSnsState->astRegsInfo[0].astI2cData[i].u32RegAddr = au32SlotAddr[i];
        }
        pstSnsState->bSyncInit = GK_TRUE;
    } else {
        for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
            if (pstSnsState->astRegsInfo[0].astI2cData[i].u32Data == pstSnsState->astRegsInfo[1].astI2cData[i].u32Data) {
                pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = GK_FALSE;
            } else {
                pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = GK_TRUE;
            }
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
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstSensorImageMode);
    MIS2009_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    pstSnsState->bSyncInit = GK_FALSE;

    if ((pstSensorImageMode->f32Fps > 30) || (pstSnsState->enWDRMode != WDR_MODE_NONE) ||
        !MIS2009_RES_IS_1080P(pstSensorImageMode->u16Width, pstSensorImageMode->u16Height)) {
        MIS2009_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState);
        return GK_FAILURE;
    }

    if ((pstSnsState->bInit == GK_TRUE) && (pstSnsState->u8ImgMode == MIS2009_SENSOR_1080P_30FPS_LINEAR_MODE)) {
        /* Don't need to switch SensorImageMode */
        return ISP_DO_NOT_NEED_SWITCH_IMAGEMODE;
    }

    pstSnsState->u8ImgMode = MIS2009_SENSOR_1080P_30FPS_LINEAR_MODE;
    pstSnsState->u32FLStd = MIS2009_VMAX_1080P30_LINEAR;
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstSnsState->au32FL[1] = pstSnsState->u32FLStd;

    return GK_SUCCESS;
}

static GK_S32 cmos_set_wdr_mode(VI_PIPE ViPipe, GK_U8 u8Mode)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    MIS2009_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    pstSnsState->bSyncInit = GK_FALSE;
    if (u8Mode != WDR_MODE_NONE) {
        ISP_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
        return GK_FAILURE;
    }

    pstSnsState->enWDRMode = WDR_MODE_NONE;
    pstSnsState->u32FLStd = MIS2009_VMAX_1080P30_LINEAR;
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstSnsState->au32FL[1] = pstSnsState->u32FLStd;

    return GK_SUCCESS;
}

/*
 * The vendor mirrors and flips in VPSS and never touches the sensor. Here the
 * sensor does it, and each flipped axis moves the readout window by one pixel
 * so the colour filter phase the ISP is configured for stays put.
 */
static GK_VOID sensor_mirror_flip(VI_PIPE ViPipe, ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;
    GK_U32 u32Row = 8, u32Col = 7; /* the vendor init table's window */
    GK_U8 u8Value;

    MIS2009_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    switch (eSnsMirrorFlip) {
    case ISP_SNS_NORMAL:
        u8Value = 0;
        break;
    case ISP_SNS_MIRROR:
        u8Value = 0x01;
        break;
    case ISP_SNS_FLIP:
        u8Value = 0x02;
        break;
    case ISP_SNS_MIRROR_FLIP:
        u8Value = 0x03;
        break;
    default:
        return;
    }
    u32Col += (u8Value & 0x01) ? 1 : 0;
    u32Row += (u8Value & 0x02) ? 1 : 0;

    mis2009_write_register(ViPipe, MIS2009_WINDOW_ADDR + 0, HIGH_8BITS(u32Row));
    mis2009_write_register(ViPipe, MIS2009_WINDOW_ADDR + 1, LOW_8BITS(u32Row));
    mis2009_write_register(ViPipe, MIS2009_WINDOW_ADDR + 2, HIGH_8BITS(u32Row + 1079));
    mis2009_write_register(ViPipe, MIS2009_WINDOW_ADDR + 3, LOW_8BITS(u32Row + 1079));
    mis2009_write_register(ViPipe, MIS2009_WINDOW_ADDR + 4, HIGH_8BITS(u32Col));
    mis2009_write_register(ViPipe, MIS2009_WINDOW_ADDR + 5, LOW_8BITS(u32Col));
    mis2009_write_register(ViPipe, MIS2009_WINDOW_ADDR + 6, HIGH_8BITS(u32Col + 1919));
    mis2009_write_register(ViPipe, MIS2009_WINDOW_ADDR + 7, LOW_8BITS(u32Col + 1919));
    mis2009_write_register(ViPipe, MIS2009_FLIP_MIRROR_ADDR, u8Value);
}

static GK_VOID sensor_global_init(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;
    ISP_I2C_DATA_S *pstData;

    MIS2009_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->bInit = GK_FALSE;
    pstSnsState->bSyncInit = GK_FALSE;
    pstSnsState->u8ImgMode = MIS2009_SENSOR_1080P_30FPS_LINEAR_MODE;
    pstSnsState->enWDRMode = WDR_MODE_NONE;
    pstSnsState->u32FLStd = MIS2009_VMAX_1080P30_LINEAR;
    pstSnsState->au32FL[0] = MIS2009_VMAX_1080P30_LINEAR;
    pstSnsState->au32FL[1] = MIS2009_VMAX_1080P30_LINEAR;

    memset(&pstSnsState->astRegsInfo[0], 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset(&pstSnsState->astRegsInfo[1], 0, sizeof(ISP_SNS_REGS_INFO_S));

    /* what the slots hold until AE first runs: the initial exposure at 1x */
    pstData = pstSnsState->astRegsInfo[0].astI2cData;
    pstData[SLOT_EXP_H].u32Data = HIGH_8BITS(MIS2009_INIT_EXPOSURE / 1024);
    pstData[SLOT_EXP_L].u32Data = LOW_8BITS(MIS2009_INIT_EXPOSURE / 1024);
    pstData[SLOT_AGAIN].u32Data = 0;
    pstData[SLOT_VMAX_H].u32Data = HIGH_8BITS(MIS2009_VMAX_1080P30_LINEAR);
    pstData[SLOT_VMAX_L].u32Data = LOW_8BITS(MIS2009_VMAX_1080P30_LINEAR);
    pstData[SLOT_ANA_GAIN].u32Data = 0x0B;
    pstData[SLOT_ANA_EXP].u32Data = 0xCC;
    pstData[SLOT_ANA_GAIN2].u32Data = 0x1F;
}

static GK_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    CMOS_CHECK_POINTER(pstSensorExpFunc);

    memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init = mis2009_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit = mis2009_exit;
    pstSensorExpFunc->pfn_cmos_sensor_global_init = sensor_global_init;
    pstSensorExpFunc->pfn_cmos_set_image_mode = cmos_set_image_mode;
    pstSensorExpFunc->pfn_cmos_set_wdr_mode = cmos_set_wdr_mode;

    pstSensorExpFunc->pfn_cmos_get_isp_default = cmos_get_isp_default;
    pstSensorExpFunc->pfn_cmos_get_isp_black_level = cmos_get_isp_black_level;
    pstSensorExpFunc->pfn_cmos_set_pixel_detect = cmos_set_pixel_detect;
    pstSensorExpFunc->pfn_cmos_get_sns_reg_info = cmos_get_sns_regs_info;

    return GK_SUCCESS;
}

static GK_S32 sensor_set_bus_info(VI_PIPE ViPipe, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunMis2009BusInfo[ViPipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

    return GK_SUCCESS;
}

static GK_S32 sensor_ctx_init(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

    MIS2009_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);

    if (pastSnsStateCtx == GK_NULL) {
        pastSnsStateCtx = (ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
        if (pastSnsStateCtx == GK_NULL) {
            ISP_TRACE(MODULE_DBG_ERR, "Isp[%d] SnsCtx malloc memory failed!\n", ViPipe);
            return ERR_CODE_ISP_NOMEM;
        }
    }

    memset(pastSnsStateCtx, 0, sizeof(ISP_SNS_STATE_S));

    MIS2009_SENSOR_SET_CTX(ViPipe, pastSnsStateCtx);

    return GK_SUCCESS;
}

static GK_VOID sensor_ctx_exit(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

    MIS2009_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);
    SENSOR_FREE(pastSnsStateCtx);
    MIS2009_SENSOR_RESET_CTX(ViPipe);
    return;
}

static GK_S32 sensor_register_callback(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib)
{
    GK_S32 s32Ret;

    ISP_SENSOR_REGISTER_S stIspRegister;
    AE_SENSOR_REGISTER_S stAeRegister;
    AWB_SENSOR_REGISTER_S stAwbRegister;
    ISP_SNS_ATTR_INFO_S stSnsAttrInfo;

    CMOS_CHECK_POINTER(pstAeLib);
    CMOS_CHECK_POINTER(pstAwbLib);

    s32Ret = sensor_ctx_init(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        return GK_FAILURE;
    }

    stSnsAttrInfo.eSensorId = MIS2009_ID;
    s32Ret = cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
    s32Ret |= GK_API_ISP_SensorRegCallBack(ViPipe, &stSnsAttrInfo, &stIspRegister);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor register callback function failed!\n");
        return s32Ret;
    }

    s32Ret = cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
    s32Ret |= GK_API_AE_SensorRegCallBack(ViPipe, pstAeLib, &stSnsAttrInfo, &stAeRegister);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor register callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
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

    s32Ret = GK_API_ISP_SensorUnRegCallBack(ViPipe, MIS2009_ID);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor unregister callback function failed!\n");
        return s32Ret;
    }

    s32Ret = GK_API_AE_SensorUnRegCallBack(ViPipe, pstAeLib, MIS2009_ID);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = GK_API_AWB_SensorUnRegCallBack(ViPipe, pstAwbLib, MIS2009_ID);
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

ISP_SNS_OBJ_S stSnsMis2009Obj = { .pfnRegisterCallback = sensor_register_callback,
                  .pfnUnRegisterCallback = sensor_unregister_callback,
                  .pfnStandby = mis2009_standby,
                  .pfnRestart = mis2009_restart,
                  .pfnMirrorFlip = sensor_mirror_flip,
                  .pfnWriteReg = mis2009_write_register,
                  .pfnReadReg = mis2009_read_register,
                  .pfnSetBusInfo = sensor_set_bus_info,
                  .pfnSetInit = sensor_set_init };

#ifdef __cplusplus
#if __cplusplus
}
#endif

#endif /* End of #ifdef __cplusplus */
