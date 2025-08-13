/*
 * (c) OpenIPC.org (c)
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
#include "mis2008_cmos_ex.h"
#include "hicompat.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define MIS2008_ID 0x2008
#define HIGH_8BITS(x) (((x)&0xFF00) >> 8)
#define LOW_8BITS(x) ((x)&0x00FF)
#define LOWER_4BITS(x) (((x)&0x000F) << 4)
#define HIGHER_4BITS(x) (((x)&0xF000) >> 12)
#define HIGHER_8BITS(x) (((x)&0x0FF0) >> 4)

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ? (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ? (b) : (a))
#endif

ISP_SNS_STATE_S *g_pastMis2008[ISP_MAX_PIPE_NUM] = { GK_NULL };

#define MIS2008_SENSOR_GET_CTX(dev, pstCtx) (pstCtx = g_pastMis2008[dev])
#define MIS2008_SENSOR_SET_CTX(dev, pstCtx) (g_pastMis2008[dev] = pstCtx)
#define MIS2008_SENSOR_RESET_CTX(dev) (g_pastMis2008[dev] = GK_NULL)

ISP_SNS_COMMBUS_U g_aunMis2008BusInfo[ISP_MAX_PIPE_NUM] = { [0] = { .s8I2cDev = 0 },
                                [1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1 } };

static GK_U32 gu32MaxTimeGetCnt[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U32 g_au32InitExposure[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U32 g_au32LinesPer500ms[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U16 g_au16InitWBGain[ISP_MAX_PIPE_NUM][3] = { { 0 } };
static GK_U16 g_au16SampleRgain[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U16 g_au16SampleBgain[ISP_MAX_PIPE_NUM] = { 0 };
static GK_U32 au32WDRIntTime[4] = { 0 };

/****************************************************************************
 * extern                                                                   *
 ****************************************************************************/
extern const unsigned int mis2008_i2c_addr;
extern unsigned int mis2008_addr_byte;
extern unsigned int mis2008_data_byte;

extern void mis2008_init(VI_PIPE ViPipe);
extern void mis2008_exit(VI_PIPE ViPipe);
extern void mis2008_standby(VI_PIPE ViPipe);
extern void mis2008_restart(VI_PIPE ViPipe);
extern int mis2008_write_register(VI_PIPE ViPipe, int addr, int data);
extern int mis2008_read_register(VI_PIPE ViPipe, int addr);

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
#define MIS2008_FULL_LINES_MAX_LINEAR (0xFFFF)

/******** MIS2008 Register Address ********/
#define MIS2008_EXP_ADDR (0x3100)
#define MIS2008_AGAIN_ADDR (0x3102)
#define MIS2008_DGAIN_ADDR (0x3700)
#define MIS2008_VMAX_ADDR (0x3200)
#define MIS2008_FLIP_MIRROR_ADDR 0x3007

#define MIS2008_INCREASE_LINES (1) /* make real fps less than stand fps because NVR require*/

#define MIS2008_VMAX_1080P30_LINEAR (1125 + MIS2008_INCREASE_LINES)
#define MIS2008_FRAME_RATE_MIN (0x34BC)
#define EXP_OFFSET_LINEAR (8)

//sensor fps mode
#define MIS2008_SENSOR_1080P_30FPS_LINEAR_MODE (1)

#define MIS2008_RES_IS_1080P(w, h) ((w) <= 1920 && (h) <= 1080)

#define MIS2008_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState)                                                    \
    do {                                                                                                       \
        ISP_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",                \
              pstSensorImageMode->u16Width, pstSensorImageMode->u16Height, pstSensorImageMode->f32Fps, \
              pstSnsState->enWDRMode);                                                                 \
    } while (0)

GK_U16 Dgain_mapping_realgain_to_sensorgain(GK_FLOAT gain_real);

GK_U16 Dgain_mapping_realgain_to_sensorgain(GK_FLOAT gain_real)
{
    GK_U16 u16sensorgain;
    GK_U32 u32Dgain = gain_real * 128;
    GK_U8 dCoarseGain = 0;
    GK_U8 dFineGain = 0;
    GK_U8 u8Reg0x3e06 = 0;
    for (dCoarseGain = 1; dCoarseGain <= 16; dCoarseGain = dCoarseGain * 2) //1,2,4,8,16
    {
        if (u32Dgain < (128 * 2 * dCoarseGain)) {
            break;
        }
    }
    dFineGain = u32Dgain / dCoarseGain;

    for (; dCoarseGain >= 2; dCoarseGain = dCoarseGain / 2) {
        u8Reg0x3e06 = (u8Reg0x3e06 << 1) | 0x01;
    }
    u16sensorgain = ((GK_U16)u8Reg0x3e06) << 8;
    u16sensorgain += dFineGain;

    return u16sensorgain;
}

static GK_S32 cmos_get_ae_default(VI_PIPE ViPipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstAeSnsDft);
    MIS2008_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset(&pstAeSnsDft->stAERouteAttr, 0, sizeof(ISP_AE_ROUTE_S));

    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
    pstAeSnsDft->stIntTimeAccu.f32Offset = 0;

    pstSnsState->u32FLStd = MIS2008_VMAX_1080P30_LINEAR ;
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FlickerFreq = 50 * 256;
    pstAeSnsDft->u32FullLinesMax = MIS2008_FULL_LINES_MAX_LINEAR;
    pstAeSnsDft->u32HmaxTimes = (1000000) / (pstSnsState->u32FLStd * 30);

    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 1; //1/128 0.0078125

    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 1; //1/128

    pstAeSnsDft->u32ISPDgainShift = 8;
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 2 << pstAeSnsDft->u32ISPDgainShift;

    pstAeSnsDft->enMaxIrisFNO = ISP_IRIS_F_NO_1_4;
    pstAeSnsDft->enMinIrisFNO = ISP_IRIS_F_NO_5_6;

    pstAeSnsDft->bAERouteExValid = GK_FALSE;
    pstAeSnsDft->stAERouteAttr.u32TotalNum = 0;
    pstAeSnsDft->stAERouteAttrEx.u32TotalNum = 0;

    if (g_au32LinesPer500ms[ViPipe] == 0) {
        pstAeSnsDft->u32LinesPer500ms = pstSnsState->u32FLStd * 30 / 2;
    } else {
        pstAeSnsDft->u32LinesPer500ms = g_au32LinesPer500ms[ViPipe];
    }

    pstAeSnsDft->au8HistThresh[0] = 0x0D;
    pstAeSnsDft->au8HistThresh[1] = 0x28;
    pstAeSnsDft->au8HistThresh[2] = 0x60;
    pstAeSnsDft->au8HistThresh[3] = 0x80;

    pstAeSnsDft->u32MaxAgain = 16128; //  1.81*128*32
    pstAeSnsDft->u32MinAgain = 1024;
    pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
    pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

    pstAeSnsDft->u32MaxDgain = 16320; //4*128
    pstAeSnsDft->u32MinDgain = 1024;
    pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
    pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

    pstAeSnsDft->u8AeCompensation = 40;
    pstAeSnsDft->u32InitAESpeed = 64;
    pstAeSnsDft->u32InitAETolerance = 5;

    pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;

    pstAeSnsDft->u32InitExposure = g_au32InitExposure[ViPipe] ? g_au32InitExposure[ViPipe] : 76151; //148859

    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 1;
    pstAeSnsDft->u32MinIntTime = 1;
    pstAeSnsDft->u32MaxIntTimeTarget = 65535;
    pstAeSnsDft->u32MinIntTimeTarget = 1;

    return GK_SUCCESS;
}

/* the function of sensor set fps */
static GK_VOID cmos_fps_set(VI_PIPE ViPipe, GK_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;
    GK_FLOAT f32maxFps;
    GK_U32 u32FullLines;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    MIS2008_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    switch (pstSnsState->u8ImgMode) {
    case MIS2008_SENSOR_1080P_30FPS_LINEAR_MODE:
        f32maxFps = 30;
        if ((f32Fps <= 30) && (f32Fps >= 2.5)) { /* Minimum 2.5fps */
            u32FullLines = MIS2008_VMAX_1080P30_LINEAR * f32maxFps / DIV_0_TO_1_FLOAT(f32Fps);
        } else {
            ISP_TRACE(MODULE_DBG_ERR, "Not support Fps: %f\n", f32Fps);
            return;
        }
        u32FullLines = (u32FullLines > MIS2008_FULL_LINES_MAX_LINEAR) ? MIS2008_FULL_LINES_MAX_LINEAR :
                                        u32FullLines;
        break;

    default:
        printf("Not support this Mode\n");
        return;
        break;
    }

    pstSnsState->u32FLStd = u32FullLines;

    pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = HIGH_8BITS(u32FullLines);
    pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = LOW_8BITS(u32FullLines);

    pstAeSnsDft->f32Fps = f32Fps;
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];

    pstAeSnsDft->u32MaxIntTime = (pstSnsState->u32FLStd << 1) - EXP_OFFSET_LINEAR;
    // pstAeSnsDft->u32HmaxTimes = (1000000) / (pstSnsState->u32FLStd * DIV_0_TO_1_FLOAT(f32Fps));

    return;
}

static GK_VOID cmos_slow_framerate_set(VI_PIPE ViPipe, GK_U32 u32FullLines, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    MIS2008_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32FullLines = (u32FullLines + EXP_OFFSET_LINEAR) >> 1;
    u32FullLines = (u32FullLines > MIS2008_FRAME_RATE_MIN) ? MIS2008_FRAME_RATE_MIN : u32FullLines;
    pstSnsState->au32FL[0] = u32FullLines;
    pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = HIGH_8BITS(u32FullLines);
    pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = LOW_8BITS(u32FullLines);

    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];

    pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - EXP_OFFSET_LINEAR;

    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static GK_VOID cmos_inttime_update(VI_PIPE ViPipe, GK_U32 u32IntTime)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;
    static GK_BOOL bFirst = GK_TRUE;
    static GK_U32 u32ShortIntTime = 0;
    static GK_U32 u32LongIntTime = 0;
    MIS2008_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    // Somehow values after this leads to picture loss
    // XM limits this to 0x10000, but actually never sets higher also
    if (u32IntTime > 0x545){
        u32IntTime = 0x545;
    }

    pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = HIGH_8BITS(u32IntTime);
    pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = LOW_8BITS(u32IntTime);

    return;
}

static const GK_U16 u16AgainTab[64] = { 1024,  1088,  1152,  1216,  1280,  1344,  1408,	 1472,	1536,  1600,  1664,
                    1728,  1792,  1856,  1920,  1984,  2048,  2176,	 2304,	2432,  2560,  2688,
                    2816,  2944,  3072,  3200,  3328,  3456,  3584,	 3712,	3840,  3968,  4096,
                    4352,  4608,  4864,  5120,  5376,  5632,  5888,	 6144,	6400,  6656,  6912,
                    7168,  7424,  7680,  7936,  8192,  8704,  9216,	 9728,	10240, 10752, 11264,
                    11776, 12288, 12800, 13312, 13824, 14336, 14848, 15360, 15872 };

static GK_U32 Dgain_table[] = {
    1024,  1088,  1152,  1216,  1280,  1344,  1408,	 1472,	1536,  1600,  1664,  1728,  1792,  1856,  1920,	 1984,
    2048,  2112,  2176,  2240,  2304,  2368,  2432,	 2496,	2560,  2624,  2688,  2752,  2816,  2880,  2944,	 3008,
    3072,  3136,  3200,  3264,  3328,  3392,  3456,	 3520,	3584,  3648,  3712,  3776,  3840,  3904,  3968,	 4032,
    4096,  4160,  4224,  4288,  4352,  4416,  4480,	 4544,	4608,  4672,  4736,  4800,  4864,  4928,  4992,	 5056,
    5120,  5184,  5248,  5312,  5376,  5440,  5504,	 5568,	5632,  5696,  5760,  5824,  5888,  5952,  6016,	 6080,
    6144,  6208,  6272,  6336,  6400,  6464,  6528,	 6592,	6656,  6720,  6784,  6848,  6912,  6976,  7040,	 7104,
    7168,  7232,  7296,  7360,  7424,  7488,  7552,	 7616,	7680,  7744,  7808,  7872,  7936,  8000,  8064,	 8128,
    8192,  8256,  8320,  8384,  8448,  8512,  8576,	 8640,	8704,  8768,  8832,  8896,  8960,  9024,  9088,	 9152,
    9216,  9280,  9344,  9408,  9472,  9536,  9600,	 9664,	9728,  9792,  9856,  9920,  9984,  10048, 10112, 10176,
    10240, 10304, 10368, 10432, 10496, 10560, 10624, 10688, 10752, 10816, 10880, 10944, 11008, 11072, 11136, 11200,
    11264, 11328, 11392, 11456, 11520, 11584, 11648, 11712, 11776, 11840, 11904, 11968, 12032, 12096, 12160, 12224,
    12288, 12352, 12416, 12480, 12544, 12608, 12672, 12736, 12800, 12864, 12928, 12992, 13056, 13120, 13184, 13248,
    13312, 13376, 13440, 13504, 13568, 13632, 13696, 13760, 13824, 13888, 13952, 14016, 14080, 14144, 14208, 14272,
    14336, 14400, 14464, 14528, 14592, 14656, 14720, 14784, 14848, 14912, 14976, 15040, 15104, 15168, 15232, 15296,
    15360, 15424, 15488, 15552, 15616, 15680, 15744, 15808, 15872, 15936, 16000, 16064, 16128, 16192, 16256, 16320
};

struct gain_tbl_info_s {
    GK_U16 gainMax;
    GK_U16 idxBase;
    GK_U8 regGain;
    GK_U8 regGainFineBase;
    GK_U8 regGainFineStep;
};

static struct gain_tbl_info_s DgainInfo[] = {
    {
        .gainMax = 1984,
        .idxBase = 0,
        .regGain = 0x01,
        .regGainFineBase = 0x10,
        .regGainFineStep = 1,
    },
    {
        .gainMax = 3008,
        .idxBase = 16,
        .regGain = 0x02,
        .regGainFineBase = 0x10,
        .regGainFineStep = 1,
    },
    {
        .gainMax = 4032,
        .idxBase = 32,
        .regGain = 0x03,
        .regGainFineBase = 0x10,
        .regGainFineStep = 1,
    },
    {
        .gainMax = 5056,
        .idxBase = 48,
        .regGain = 0x04,
        .regGainFineBase = 0x10,
        .regGainFineStep = 1,
    },
    {
        .gainMax = 6080,
        .idxBase = 64,
        .regGain = 0x05,
        .regGainFineBase = 0x10,
        .regGainFineStep = 1,
    },
    {
        .gainMax = 7104,
        .idxBase = 80,
        .regGain = 0x06,
        .regGainFineBase = 0x10,
        .regGainFineStep = 1,
    },
    {
        .gainMax = 8128,
        .idxBase = 96,
        .regGain = 0x07,
        .regGainFineBase = 0x10,
        .regGainFineStep = 1,
    },
    {
        .gainMax = 9152,
        .idxBase = 112,
        .regGain = 0x08,
        .regGainFineBase = 0x10,
        .regGainFineStep = 1,
    },
    {
        .gainMax = 10176,
        .idxBase = 128,
        .regGain = 0x09,
        .regGainFineBase = 0x10,
        .regGainFineStep = 1,
    },
    {
        .gainMax = 11200,
        .idxBase = 144,
        .regGain = 0x0a,
        .regGainFineBase = 0x10,
        .regGainFineStep = 1,
    },
    {
        .gainMax = 12224,
        .idxBase = 160,
        .regGain = 0x0b,
        .regGainFineBase = 0x10,
        .regGainFineStep = 1,
    },
    {
        .gainMax = 13248,
        .idxBase = 176,
        .regGain = 0x0c,
        .regGainFineBase = 0x10,
        .regGainFineStep = 1,
    },
    {
        .gainMax = 14272,
        .idxBase = 192,
        .regGain = 0x0d,
        .regGainFineBase = 0x10,
        .regGainFineStep = 1,
    },
    {
        .gainMax = 15296,
        .idxBase = 208,
        .regGain = 0x0e,
        .regGainFineBase = 0x10,
        .regGainFineStep = 1,
    },
    {
        .gainMax = 16320,
        .idxBase = 224,
        .regGain = 0x0f,
        .regGainFineBase = 0x10,
        .regGainFineStep = 1,
    },
};


static GK_VOID cmos_again_calc_table(VI_PIPE ViPipe, GK_U32 *pu32AgainLin, GK_U32 *pu32AgainDb)
{
    GK_U32 i;
    static GK_U8 again_table_size = 63;

    CMOS_CHECK_POINTER_VOID(pu32AgainLin);
    CMOS_CHECK_POINTER_VOID(pu32AgainDb);

    if (*pu32AgainLin >= u16AgainTab[again_table_size - 1]) {
        *pu32AgainLin = u16AgainTab[again_table_size - 1];
        *pu32AgainDb = again_table_size - 1;
        return;
    }

    for (i = 1; i < again_table_size; i++) {
        if (*pu32AgainLin < u16AgainTab[i]) {
            *pu32AgainLin = u16AgainTab[i - 1];
            *pu32AgainDb = i - 1;
            break;
        }
    }
    return;
}

static GK_VOID cmos_dgain_calc_table(VI_PIPE ViPipe, GK_U32 *pu32DgainLin, GK_U32 *pu32DgainDb)
{
    GK_U32 i;
    static GK_U8 dgain_table_size = 255;

    CMOS_CHECK_POINTER_VOID(pu32DgainLin);
    CMOS_CHECK_POINTER_VOID(pu32DgainDb);

    if (*pu32DgainLin >= Dgain_table[dgain_table_size - 1]) {
        *pu32DgainLin = Dgain_table[dgain_table_size - 1];
        *pu32DgainDb = dgain_table_size - 1;
        return;
    }

    for (i = 1; i < dgain_table_size; i++) {
        if (*pu32DgainLin < Dgain_table[i]) {
            *pu32DgainLin = Dgain_table[i - 1];
            *pu32DgainDb = i - 1;
            break;
        }
    }
    return;
}

static GK_VOID cmos_gains_update(VI_PIPE ViPipe, GK_U32 u32Again, GK_U32 u32Dgain)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;
    struct gain_tbl_info_s *info;
    int i, tbl_num;

    MIS2008_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = (u32Again & 0x7F);

    /* find Dgain register setting. */
    tbl_num = sizeof(DgainInfo) / sizeof(struct gain_tbl_info_s);
    for (i = tbl_num - 1; i >= 0; i--) {
        info = &DgainInfo[i];

        if (u32Dgain >= info->idxBase)
            break;
    }

    pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = ((info->regGain & 0x0e) >> 1);
    u32Dgain = info->regGainFineBase + (u32Dgain - info->idxBase) * info->regGainFineStep;
    pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = ((info->regGain & 0x01) << 7) | ((u32Dgain & 0x0F) << 3);
 
     return;
}

static GK_VOID cmos_get_inttime_max(VI_PIPE ViPipe, GK_U16 u16ManRatioEnable, GK_U32 *au32Ratio, GK_U32 *au32IntTimeMax,
                    GK_U32 *au32IntTimeMin, GK_U32 *pu32LFMaxIntTime)
{
    GK_U32 u32ShortTimeMinLimit = 0;
    GK_U32 u32ShortTimeMaxlimit = 0;

    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER_VOID(au32Ratio);
    CMOS_CHECK_POINTER_VOID(au32IntTimeMax);
    CMOS_CHECK_POINTER_VOID(au32IntTimeMin);
    CMOS_CHECK_POINTER_VOID(pu32LFMaxIntTime);
    MIS2008_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);
    u32ShortTimeMinLimit = 1;

    return;
}

static GK_S32 cmos_init_ae_exp_function(AE_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    CMOS_CHECK_POINTER(pstExpFuncs);

    memset(pstExpFuncs, 0, sizeof(AE_SENSOR_EXP_FUNC_S));

    pstExpFuncs->pfn_cmos_get_ae_default = cmos_get_ae_default;
    pstExpFuncs->pfn_cmos_fps_set = cmos_fps_set;
    // pstExpFuncs->pfn_cmos_slow_framerate_set = cmos_slow_framerate_set;
    pstExpFuncs->pfn_cmos_inttime_update = cmos_inttime_update;
    pstExpFuncs->pfn_cmos_gains_update = cmos_gains_update;
    pstExpFuncs->pfn_cmos_again_calc_table = cmos_again_calc_table;
    pstExpFuncs->pfn_cmos_dgain_calc_table = cmos_dgain_calc_table;
    pstExpFuncs->pfn_cmos_get_inttime_max = cmos_get_inttime_max;
    // pstExpFuncs->pfn_cmos_ae_fswdr_attr_set = cmos_ae_fswdr_attr_set;

    return GK_SUCCESS;
}

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0
static GK_S32 cmos_get_awb_default(VI_PIPE ViPipe, AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstAwbSnsDft);
    MIS2008_SENSOR_GET_CTX(ViPipe, pstSnsState);
    memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));

    pstAwbSnsDft->u16InitGgain = 1024;
    pstAwbSnsDft->u8AWBRunInterval = 1;
    CMOS_CHECK_POINTER(pstSnsState);

    memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));
    pstAwbSnsDft->u16WbRefTemp = 5082;

    pstAwbSnsDft->au16GainOffset[0] = 404;
    pstAwbSnsDft->au16GainOffset[1] = 256;
    pstAwbSnsDft->au16GainOffset[2] = 256;
    pstAwbSnsDft->au16GainOffset[3] = 509;

    pstAwbSnsDft->as32WbPara[0] = 12;
    pstAwbSnsDft->as32WbPara[1] = 213;
    pstAwbSnsDft->as32WbPara[2] = -30;
    pstAwbSnsDft->as32WbPara[3] = 186383;
    pstAwbSnsDft->as32WbPara[4] = 128;
    pstAwbSnsDft->as32WbPara[5] = -131654;
    pstAwbSnsDft->u16GoldenRgain = GOLDEN_RGAIN;
    pstAwbSnsDft->u16GoldenBgain = GOLDEN_BGAIN;

    memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcm, sizeof(AWB_CCM_S));
    memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTable, sizeof(AWB_AGC_TABLE_S));

    pstAwbSnsDft->u16InitRgain = g_au16InitWBGain[ViPipe][0];
    pstAwbSnsDft->u16InitGgain = g_au16InitWBGain[ViPipe][1];
    pstAwbSnsDft->u16InitBgain = g_au16InitWBGain[ViPipe][2];
    pstAwbSnsDft->u16SampleRgain = g_au16SampleRgain[ViPipe];
    pstAwbSnsDft->u16SampleBgain = g_au16SampleBgain[ViPipe];

    return GK_SUCCESS;
}

static GK_S32 cmos_init_awb_exp_function(AWB_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    CMOS_CHECK_POINTER(pstExpFuncs);

    memset(pstExpFuncs, 0, sizeof(AWB_SENSOR_EXP_FUNC_S));
    pstExpFuncs->pfn_cmos_get_awb_default = cmos_get_awb_default;

    return GK_SUCCESS;
}

static ISP_CMOS_DNG_COLORPARAM_S g_stDngColorParam = { { 378, 256, 430 }, { 439, 256, 439 } };

static GK_S32 cmos_get_isp_default(VI_PIPE ViPipe, ISP_CMOS_DEFAULT_S *pstDef)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstDef);
    MIS2008_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset(pstDef, 0, sizeof(ISP_CMOS_DEFAULT_S));

    pstDef->unKey.bit1Ca = 1;
    pstDef->pstCa = &g_stIspCA;
    pstDef->unKey.bit1Dpc = 1;
    pstDef->pstDpc = &g_stCmosDpc;
    pstDef->unKey.bit1Wdr = 0;
    pstDef->pstWdr = &g_stIspWDR;
    pstDef->unKey.bit1Lsc = 0;
    pstDef->pstLsc = &g_stCmosLsc;
    switch (pstSnsState->enWDRMode) {
    default:
    case WDR_MODE_NONE:
        pstDef->unKey.bit1Demosaic = 1;
        pstDef->pstDemosaic = &g_stIspDemosaic;
        pstDef->unKey.bit1Sharpen = 1;
        pstDef->pstSharpen = &g_stIspYuvSharpen;
        pstDef->unKey.bit1Drc = 1;
        pstDef->pstDrc = &g_stIspDRC;
        pstDef->unKey.bit1BayerNr = 1;
        pstDef->pstBayerNr = &g_stIspBayerNr;
        pstDef->unKey.bit1Gamma = 1;
        pstDef->pstGamma = &g_stIspGamma;
#ifdef CONFIG_ISP_CR_SUPPORT
        pstDef->unKey.bit1Ge = 1;
        pstDef->pstGe = &g_stIspGe;
#endif
        pstDef->unKey.bit1AntiFalseColor = 1;
        pstDef->pstAntiFalseColor = &g_stIspAntiFalseColor;
        pstDef->unKey.bit1Ldci = 1;
        pstDef->pstLdci = &g_stIspLdci;
        pstDef->unKey.bit1Dehaze = 1;
        pstDef->pstDehaze = &g_stIspDehaze;
        pstDef->unKey.bit1Lcac = 1;
        pstDef->pstLcac = &g_stIspLCac;
        memcpy(&pstDef->stNoiseCalibration, &g_stIspNoiseCalibration, sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
        break;
    }

    pstDef->stSensorMode.u32SensorID = MIS2008_ID;
    pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;

    memcpy(&pstDef->stDngColorParam, &g_stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S));

    switch (pstSnsState->u8ImgMode) {
    default:
    case MIS2008_SENSOR_1080P_30FPS_LINEAR_MODE:
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
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstBlackLevel);
    MIS2008_SENSOR_GET_CTX(ViPipe, pstSnsState);
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

    MIS2008_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    if (MIS2008_SENSOR_1080P_30FPS_LINEAR_MODE == pstSnsState->u8ImgMode) {
        u32FullLines_5Fps = MIS2008_VMAX_1080P30_LINEAR * 30 / 5;
    } else {
        return;
    }

    u32MaxIntTime_5Fps = u32FullLines_5Fps * 2 - EXP_OFFSET_LINEAR;

    if (bEnable) { /* setup for ISP pixel calibration mode */
        mis2008_write_register(ViPipe, MIS2008_VMAX_ADDR, HIGH_8BITS(u32FullLines_5Fps)); /* 5fps */
        mis2008_write_register(ViPipe, MIS2008_VMAX_ADDR + 1, LOW_8BITS(u32FullLines_5Fps)); /* 5fps */
        mis2008_write_register(ViPipe, MIS2008_EXP_ADDR,
                       HIGHER_4BITS(u32MaxIntTime_5Fps)); /* max exposure lines */
        mis2008_write_register(ViPipe, MIS2008_EXP_ADDR + 1,
                       HIGHER_8BITS(u32MaxIntTime_5Fps)); /* max exposure lines */
        // mis2008_write_register(ViPipe, MIS2008_EXP_ADDR + 2,
        // 		       LOWER_4BITS(u32MaxIntTime_5Fps)); /* max exposure lines */

        mis2008_write_register(ViPipe, MIS2008_DGAIN_ADDR, 0x00);
        mis2008_write_register(ViPipe, MIS2008_DGAIN_ADDR + 1, 0x80);
        mis2008_write_register(ViPipe, MIS2008_AGAIN_ADDR, 0x00);
        // mis2008_write_register(ViPipe, MIS2008_AGAIN_ADDR+1, 0x40);
    } else { /* setup for ISP 'normal mode' */
        pstSnsState->u32FLStd = (pstSnsState->u32FLStd > MIS2008_FULL_LINES_MAX_LINEAR * 2) ?
                        (MIS2008_FULL_LINES_MAX_LINEAR * 2) :
                        pstSnsState->u32FLStd;
        pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
        mis2008_write_register(ViPipe, MIS2008_VMAX_ADDR, HIGH_8BITS(pstSnsState->u32FLStd >> 1));
        mis2008_write_register(ViPipe, MIS2008_VMAX_ADDR + 1, LOW_8BITS(pstSnsState->u32FLStd >> 1));
        pstSnsState->bSyncInit = GK_FALSE;
    }

    return;
}

static GK_S32 cmos_get_sns_regs_info(VI_PIPE ViPipe, ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
    GK_S32 i;
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstSnsRegsInfo);
    MIS2008_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    if ((pstSnsState->bSyncInit == GK_FALSE) || (pstSnsRegsInfo->bConfig == GK_FALSE)) {
        pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
        pstSnsState->astRegsInfo[0].unComBus.s8I2cDev = g_aunMis2008BusInfo[ViPipe].s8I2cDev;
        pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;
        pstSnsState->astRegsInfo[0].u32RegNum = 8;

        for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
            pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = GK_TRUE;
            pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr = mis2008_i2c_addr;
            pstSnsState->astRegsInfo[0].astI2cData[i].u32AddrByteNum = mis2008_addr_byte;
            pstSnsState->astRegsInfo[0].astI2cData[i].u32DataByteNum = mis2008_data_byte;
        }

        //Linear Mode Regs
        pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr = MIS2008_EXP_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr = MIS2008_EXP_ADDR + 1;
        // pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0;
        // pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr = MIS2008_EXP_ADDR + 2;

        pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr = MIS2008_AGAIN_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr = MIS2008_DGAIN_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr = MIS2008_DGAIN_ADDR + 1;

        pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr = MIS2008_VMAX_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr = MIS2008_VMAX_ADDR + 1;
        pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr = MIS2008_FLIP_MIRROR_ADDR;
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
    GK_U8 u8SensorImageMode = 0;
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstSensorImageMode);
    MIS2008_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    u8SensorImageMode = pstSnsState->u8ImgMode;
    pstSnsState->bSyncInit = GK_FALSE;

    if (pstSensorImageMode->f32Fps <= 30) {
        if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
            if (MIS2008_RES_IS_1080P(pstSensorImageMode->u16Width, pstSensorImageMode->u16Height)) {
                u8SensorImageMode = MIS2008_SENSOR_1080P_30FPS_LINEAR_MODE;
                // pstSnsState->u32FLStd =	MIS2008_VMAX_1080P30_LINEAR * 2;
            } else {
                MIS2008_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState);
                return GK_FAILURE;
            }
        } else {
            MIS2008_ERR_MODE_PRINT(pstSensorImageMode, pstSnsState);
            return GK_FAILURE;
        }
    } else {
    }

    if ((pstSnsState->bInit == GK_TRUE) && (u8SensorImageMode == pstSnsState->u8ImgMode)) {
        /* Don't need to switch SensorImageMode */
        return ISP_DO_NOT_NEED_SWITCH_IMAGEMODE;
    }

    pstSnsState->u8ImgMode = u8SensorImageMode;

    return GK_SUCCESS;
}

static GK_VOID sensor_mirror_flip(VI_PIPE ViPipe, ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;
    MIS2008_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);
    GK_U8 FRAME_H_ST, FRAME_H_END;
    GK_U8 FRAME_W_ST, FRAME_W_END;
    GK_U8 value = 0;
    switch (eSnsMirrorFlip) {
    case ISP_SNS_NORMAL:
        value |= 0;
        FRAME_H_ST = 4;
        FRAME_H_END = 0x3f;
        FRAME_W_ST = 9;
        FRAME_W_END = 0x88;
        break;
    case ISP_SNS_MIRROR:
        value |= 0x01;
        FRAME_H_ST = 4;
        FRAME_H_END = 0x3f;
        FRAME_W_ST = 0xa;
        FRAME_W_END = 0x89;
        break;
    case ISP_SNS_FLIP:
        value |= 0x02;
        FRAME_H_ST = 5;
        FRAME_H_END = 0x40;
        FRAME_W_ST = 9;
        FRAME_W_END = 0x88;
        break;
    case ISP_SNS_MIRROR_FLIP:
        value |= 0x03;
        FRAME_H_ST = 5;
        FRAME_H_END = 0x40;
        FRAME_W_ST = 0xa;
        FRAME_W_END = 0x89;
        break;
    default:
        return;
    }

    mis2008_write_register(ViPipe, 0x3205, FRAME_H_ST);
    mis2008_write_register(ViPipe, 0x3207, FRAME_H_END);
    mis2008_write_register(ViPipe, 0x3209, FRAME_W_ST);
    mis2008_write_register(ViPipe, 0x320b, FRAME_W_END);
    mis2008_write_register(ViPipe, MIS2008_FLIP_MIRROR_ADDR, value);
}

static GK_VOID sensor_global_init(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    MIS2008_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->bInit = GK_FALSE;
    pstSnsState->bSyncInit = GK_FALSE;
    pstSnsState->u8ImgMode = MIS2008_SENSOR_1080P_30FPS_LINEAR_MODE;
    pstSnsState->enWDRMode = WDR_MODE_NONE;
    pstSnsState->u32FLStd = MIS2008_VMAX_1080P30_LINEAR;
    pstSnsState->au32FL[0] = MIS2008_VMAX_1080P30_LINEAR;
    pstSnsState->au32FL[1] = MIS2008_VMAX_1080P30_LINEAR;

    memset(&pstSnsState->astRegsInfo[0], 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset(&pstSnsState->astRegsInfo[1], 0, sizeof(ISP_SNS_REGS_INFO_S));

    pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = 0x05;
    pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = 0x45;

}

static GK_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    CMOS_CHECK_POINTER(pstSensorExpFunc);

    memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init = mis2008_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit = mis2008_exit;
    pstSensorExpFunc->pfn_cmos_sensor_global_init = sensor_global_init;
    pstSensorExpFunc->pfn_cmos_set_image_mode = cmos_set_image_mode;
    // pstSensorExpFunc->pfn_cmos_set_wdr_mode = cmos_set_wdr_mode;

    pstSensorExpFunc->pfn_cmos_get_isp_default = cmos_get_isp_default;
    pstSensorExpFunc->pfn_cmos_get_isp_black_level = cmos_get_isp_black_level;
    pstSensorExpFunc->pfn_cmos_set_pixel_detect = cmos_set_pixel_detect;
    pstSensorExpFunc->pfn_cmos_get_sns_reg_info = cmos_get_sns_regs_info;

    return GK_SUCCESS;
}

static GK_S32 sensor_set_bus_info(VI_PIPE ViPipe, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunMis2008BusInfo[ViPipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

    return GK_SUCCESS;
}

static GK_S32 sensor_ctx_init(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

    MIS2008_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);

    if (pastSnsStateCtx == GK_NULL) {
        pastSnsStateCtx = (ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
        if (pastSnsStateCtx == GK_NULL) {
            ISP_TRACE(MODULE_DBG_ERR, "Isp[%d] SnsCtx malloc memory failed!\n", ViPipe);
            return ERR_CODE_ISP_NOMEM;
        }
    }

    memset(pastSnsStateCtx, 0, sizeof(ISP_SNS_STATE_S));

    MIS2008_SENSOR_SET_CTX(ViPipe, pastSnsStateCtx);

    return GK_SUCCESS;
}

static GK_VOID sensor_ctx_exit(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

    MIS2008_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);
    SENSOR_FREE(pastSnsStateCtx);
    MIS2008_SENSOR_RESET_CTX(ViPipe);
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

    stSnsAttrInfo.eSensorId = MIS2008_ID;
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

    s32Ret = GK_API_ISP_SensorUnRegCallBack(ViPipe, MIS2008_ID);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor unregister callback function failed!\n");
        return s32Ret;
    }

    s32Ret = GK_API_AE_SensorUnRegCallBack(ViPipe, pstAeLib, MIS2008_ID);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = GK_API_AWB_SensorUnRegCallBack(ViPipe, pstAwbLib, MIS2008_ID);
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

ISP_SNS_OBJ_S stSnsMis2008Obj = { .pfnRegisterCallback = sensor_register_callback,
                  .pfnUnRegisterCallback = sensor_unregister_callback,
                  .pfnStandby = mis2008_standby,
                  .pfnRestart = mis2008_restart,
                  .pfnMirrorFlip = sensor_mirror_flip,
                  .pfnWriteReg = mis2008_write_register,
                  .pfnReadReg = mis2008_read_register,
                  .pfnSetBusInfo = sensor_set_bus_info,
                  .pfnSetInit = sensor_set_init };

#ifdef __cplusplus
#if __cplusplus
}
#endif

#endif /* End of #ifdef __cplusplus */
