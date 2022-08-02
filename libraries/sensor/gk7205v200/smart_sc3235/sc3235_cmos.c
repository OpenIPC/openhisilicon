/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#if !defined(__SC3235_CMOS_H_)
#define __SC3235_CMOS_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "comm_sns.h"
#include "comm_video.h"
#include "sns_ctrl.h"
#include "gk_api_isp.h"
#include "gk_api_ae.h"
#include "gk_api_awb.h"

#include "sc3235_cmos_ex.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */


#define SC3235_ID                             3235

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/

#define HIGH_8BITS(x)   (((x) & 0xFF00) >> 8)
#define LOW_8BITS(x)    ( (x) & 0x00FF)
#define LOWER_4BITS(x)  ( (x) & 0x000F)
#define HIGHER_4BITS(x) (((x) & 0xF000) >> 12)
#define HIGHER_8BITS(x) (((x) & 0x0FF0) >> 4)

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ?  (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ?  (b) : (a))
#endif

ISP_SNS_STATE_S *g_pastSc3235[ISP_MAX_PIPE_NUM] = {GK_NULL};

#define SC3235_SENSOR_GET_CTX(dev, pstCtx)   (pstCtx = g_pastSc3235[dev])
#define SC3235_SENSOR_SET_CTX(dev, pstCtx)   (g_pastSc3235[dev] = pstCtx)
#define SC3235_SENSOR_RESET_CTX(dev)         (g_pastSc3235[dev] = GK_NULL)

ISP_SNS_COMMBUS_U g_aunSc3235BusInfo[ISP_MAX_PIPE_NUM] = {
    [0] = { .s8I2cDev = 0},
    [1 ... ISP_MAX_PIPE_NUM - 1] = { .s8I2cDev = -1}
};

static ISP_FSWDR_MODE_E genFSWDRMode[ISP_MAX_PIPE_NUM] = {
    [0 ... ISP_MAX_PIPE_NUM - 1] = ISP_FSWDR_NORMAL_MODE
};

static GK_U32 gu32MaxTimeGetCnt[ISP_MAX_PIPE_NUM] = {0};
static GK_U32 g_au32InitExposure[ISP_MAX_PIPE_NUM]  = {0};
static GK_U32 g_au32LinesPer500ms[ISP_MAX_PIPE_NUM] = {0};

static GK_U16 g_au16InitWBGain[ISP_MAX_PIPE_NUM][3] = {{0}};
static GK_U16 g_au16SampleRgain[ISP_MAX_PIPE_NUM] = {0};
static GK_U16 g_au16SampleBgain[ISP_MAX_PIPE_NUM] = {0};

/****************************************************************************
 * extern                                                                   *
 ****************************************************************************/
extern const unsigned int sc3235_i2c_addr;
extern unsigned int sc3235_addr_byte;
extern unsigned int sc3235_data_byte;

extern void sc3235_init(VI_PIPE ViPipe);
extern void sc3235_exit(VI_PIPE ViPipe);
extern void sc3235_standby(VI_PIPE ViPipe);
extern void sc3235_restart(VI_PIPE ViPipe);
extern int  sc3235_write_register(VI_PIPE ViPipe, int addr, int data);
extern int  sc3235_read_register(VI_PIPE ViPipe, int addr);

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/

/******** Sc3235 Register Address ********/
#define SC3235_EXP_H_ADDR                     (0x3e00)
#define SC3235_EXP_M_ADDR                     (0x3e01)
#define SC3235_EXP_L_ADDR                     (0x3e02)

#define SC3235_AGAIN_H_ADDR                   (0x3e08)
#define SC3235_AGAIN_L_ADDR                   (0x3e09)

#define SC3235_DGAIN_H_ADDR                   (0x3e06)
#define SC3235_DGAIN_L_ADDR                   (0x3e07)

#define SC3235_VMAX_H_ADDR                    (0x320e)
#define SC3235_VMAX_L_ADDR                    (0x320f)

#define SC3235_HMAX_H_ADDR                    (0x320c)
#define SC3235_HMAX_L_ADDR                    (0x320d)

#define SC3235_FULL_LINES_MAX                 (0x7FFF)
#define SC3235_FRAME_SLOW_MAX                 (0x4F12)


#define SC3235_INCREASE_LINES                 (0) /* make real fps less than stand fps because NVR require */
#define SC3235_VMAX_1296P30_LINEAR            (1350 + SC3235_INCREASE_LINES)

// sensor fps mode
#define SC3235_SENSOR_1296P_30FPS_LINEAR_MODE (1)
#define SC3235_RES_IS_1296P(w, h)      ((w) <= 2304 && (h) <= 1296)


static GK_S32 cmos_get_ae_default(VI_PIPE ViPipe, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstAeSnsDft);
    SC3235_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset(&pstAeSnsDft->stAERouteAttr, 0, sizeof(ISP_AE_ROUTE_S));

    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FlickerFreq = 50 * 256;
    pstAeSnsDft->u32FullLinesMax = SC3235_FULL_LINES_MAX;

    pstAeSnsDft->stIntTimeAccu.enAccuType = AE_ACCURACY_LINEAR;
    pstAeSnsDft->stIntTimeAccu.f32Accuracy = 1;
    pstAeSnsDft->stIntTimeAccu.f32Offset = 0;

    pstAeSnsDft->stAgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stAgainAccu.f32Accuracy = 1;

    pstAeSnsDft->stDgainAccu.enAccuType = AE_ACCURACY_TABLE;
    pstAeSnsDft->stDgainAccu.f32Accuracy = 1;

    pstAeSnsDft->u32ISPDgainShift = 8;
    pstAeSnsDft->u32MinISPDgainTarget = 1 << pstAeSnsDft->u32ISPDgainShift;
    pstAeSnsDft->u32MaxISPDgainTarget = 4 << pstAeSnsDft->u32ISPDgainShift;

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

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:   /* linear mode */
            pstAeSnsDft->au8HistThresh[0] = 0xd;
            pstAeSnsDft->au8HistThresh[1] = 0x28;
            pstAeSnsDft->au8HistThresh[2] = 0x60;
            pstAeSnsDft->au8HistThresh[3] = 0x80;

            pstAeSnsDft->u32MaxAgain = 16128;
            pstAeSnsDft->u32MinAgain = 1024;
            pstAeSnsDft->u32MaxAgainTarget = pstAeSnsDft->u32MaxAgain;
            pstAeSnsDft->u32MinAgainTarget = pstAeSnsDft->u32MinAgain;

            pstAeSnsDft->u32MaxDgain = 31744;
            pstAeSnsDft->u32MinDgain = 1024;
            pstAeSnsDft->u32MaxDgainTarget = pstAeSnsDft->u32MaxDgain;
            pstAeSnsDft->u32MinDgainTarget = pstAeSnsDft->u32MinDgain;

            pstAeSnsDft->u8AeCompensation = 0x40;
            pstAeSnsDft->enAeExpMode = AE_EXP_HIGHLIGHT_PRIOR;

            pstAeSnsDft->u32InitExposure = g_au32InitExposure[ViPipe] ? g_au32InitExposure[ViPipe] : 148859;

            pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8;
            pstAeSnsDft->u32MinIntTime = 2;
            pstAeSnsDft->u32MaxIntTimeTarget = 65535;
            pstAeSnsDft->u32MinIntTimeTarget = 2;
            break;
    }

    return GK_SUCCESS;
}


/* the function of sensor set fps */
static GK_VOID cmos_fps_set(VI_PIPE ViPipe, GK_FLOAT f32Fps, AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    GK_U32 u32FullLines;
    GK_U32 u32VMAX = SC3235_VMAX_1296P30_LINEAR;
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    SC3235_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    switch (pstSnsState->u8ImgMode) {
        case SC3235_SENSOR_1296P_30FPS_LINEAR_MODE:
            if ((f32Fps <= 30) && (f32Fps >= 4)) {
                u32VMAX = SC3235_VMAX_1296P30_LINEAR * 30 / DIV_0_TO_1_FLOAT(f32Fps);
            } else {
                ISP_TRACE(MODULE_DBG_ERR, "Not support Fps: %f\n", f32Fps);
                return;
            }
            break;
        default:
            break;
    }

    u32VMAX = (u32VMAX > SC3235_FULL_LINES_MAX) ? SC3235_FULL_LINES_MAX : u32VMAX;

    pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = LOW_8BITS(u32VMAX);
    pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = HIGH_8BITS(u32VMAX);

    u32FullLines = u32VMAX << 1;
    pstSnsState->u32FLStd = (u32FullLines > SC3235_FULL_LINES_MAX) ? SC3235_FULL_LINES_MAX : u32FullLines;
    pstAeSnsDft->f32Fps = f32Fps;
    pstAeSnsDft->u32LinesPer500ms = pstSnsState->u32FLStd * f32Fps / 2;
    pstAeSnsDft->u32FullLinesStd = pstSnsState->u32FLStd;
    pstAeSnsDft->u32MaxIntTime = pstSnsState->u32FLStd - 8;
    pstSnsState->au32FL[0] = pstSnsState->u32FLStd;
    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];

    return;
}

static GK_VOID cmos_slow_framerate_set(VI_PIPE ViPipe, GK_U32 u32FullLines,
                                       AE_SENSOR_DEFAULT_S *pstAeSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER_VOID(pstAeSnsDft);
    SC3235_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32FullLines = (u32FullLines > SC3235_FRAME_SLOW_MAX) ? SC3235_FRAME_SLOW_MAX : u32FullLines;
    pstSnsState->au32FL[0] = u32FullLines;
    pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = LOW_8BITS((pstSnsState->au32FL[0] + 8) >> 1);
    pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = HIGH_8BITS((pstSnsState->au32FL[0] + 8) >> 1);

    pstAeSnsDft->u32FullLines = pstSnsState->au32FL[0];
    pstAeSnsDft->u32MaxIntTime = pstSnsState->au32FL[0] - 8;

    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static GK_VOID cmos_inttime_update(VI_PIPE ViPipe, GK_U32 u32IntTime)
{
    GK_U32 u32Val;
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;
    SC3235_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32Val = (u32IntTime > SC3235_FULL_LINES_MAX) ? SC3235_FULL_LINES_MAX : u32IntTime;

    pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = LOWER_4BITS(u32Val) << 4;
    pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = HIGHER_8BITS(u32Val);
    pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = HIGHER_4BITS(u32Val);

    return;
}

static GK_U32 again_index[128] = {
    1024, 1056, 1088, 1120, 1152, 1184, 1216, 1248, 1280, 1312, 1344, 1376, 1408, 1440, 1472, 1504, 1536, 1568, 1600, 1632, 1664, 1696, 1728, 1760, 1792, 1824, 1856, 1888, 1920, 1952, 1984, 2016, 2048,
    2112, 2176, 2240, 2304, 2368, 2432, 2496, 2560, 2624, 2688, 2752, 2816, 2880, 2944, 3008, 3072, 3136, 3200, 3264, 3328, 3392, 3456, 3520, 3584, 3648, 3712, 3776, 3840, 3904, 3968, 4032, 4096, 4224,
    4352, 4480, 4608, 4736, 4864, 4992, 5120, 5248, 5376, 5504, 5632, 5760, 5888, 6016, 6144, 6272, 6400, 6528, 6656, 6784, 6912, 7040, 7168, 7296, 7424, 7552, 7680, 7808, 7936, 8064, 8192, 8448, 8704,
    8960, 9216, 9472, 9728, 9984, 10240, 10496, 10752, 11008, 11264, 11520, 11776, 12032, 12288, 12544, 12800, 13056, 13312, 13568, 13824, 14080, 14336, 14592, 14848, 15104, 15360, 15616, 15872, 16128,

};

static GK_U32 dgain_index[80] = {
    1024, 1088, 1152, 1216, 1280, 1344, 1408, 1472, 1536, 1600, 1664, 1728, 1792, 1856, 1920, 1984,
    2048, 2176, 2304, 2432, 2560, 2688, 2816, 2944, 3072, 3200, 3328, 3456, 3584, 3712, 3840, 3968,
    4096, 4352, 4608, 4864, 5120, 5376, 5632, 5888, 6144, 6400, 6656, 6912, 7168, 7424, 7680, 7936,
    8192, 8704, 9216, 9728, 10240, 10752, 11264, 11776, 12288, 12800, 13312, 13824, 14336, 14848, 15360, 15872,
    16384, 17408, 18432, 19456, 20480, 21504, 22528, 23552, 24576, 25600, 26624, 27648, 28672, 29696, 30720, 31744
};

static GK_U32 again_table[128] = {
    0x320, 0x321, 0x322, 0x323, 0x324, 0x325, 0x326, 0x327, 0x328, 0x329, 0x32a, 0x32b, 0x32c, 0x32d, 0x32e, 0x32f, 0x330, 0x331, 0x332, 0x333, 0x334, 0x335, 0x336, 0x337, 0x338, 0x339, 0x33a, 0x33b, 0x33c, 0x33d, 0x33e, 0x33f,
    0x720, 0x721, 0x722, 0x723, 0x724, 0x725, 0x726, 0x727, 0x728, 0x729, 0x72a, 0x72b, 0x72c, 0x72d, 0x72e, 0x72f, 0x730, 0x731, 0x732, 0x733, 0x734, 0x735, 0x736, 0x737, 0x738, 0x739, 0x73a, 0x73b, 0x73c, 0x73d, 0x73e, 0x73f,
    0xf20, 0xf21, 0xf22, 0xf23, 0xf24, 0xf25, 0xf26, 0xf27, 0xf28, 0xf29, 0xf2a, 0xf2b, 0xf2c, 0xf2d, 0xf2e, 0xf2f, 0xf30, 0xf31, 0xf32, 0xf33, 0xf34, 0xf35, 0xf36, 0xf37, 0xf38, 0xf39, 0xf3a, 0xf3b, 0xf3c, 0xf3d, 0xf3e, 0xf3f,
    0x1f20, 0x1f21, 0x1f22, 0x1f23, 0x1f24, 0x1f25, 0x1f26, 0x1f27, 0x1f28, 0x1f29, 0x1f2a, 0x1f2b, 0x1f2c, 0x1f2d, 0x1f2e, 0x1f2f, 0x1f30, 0x1f31, 0x1f32, 0x1f33, 0x1f34, 0x1f35, 0x1f36, 0x1f37, 0x1f38, 0x1f39, 0x1f3a, 0x1f3b, 0x1f3c, 0x1f3d, 0x1f3e, 0x1f3f,

};

static GK_U32 dgain_table[80] = {
    0x080, 0x088, 0x090, 0x098, 0x0a0, 0x0a8, 0x0b0, 0x0b8, 0x0c0, 0x0c8, 0x0d0, 0x0d8, 0x0e0, 0x0e8, 0x0f0, 0x0f8,
    0x180, 0x188, 0x190, 0x198, 0x1a0, 0x1a8, 0x1b0, 0x1b8, 0x1c0, 0x1c8, 0x1d0, 0x1d8, 0x1e0, 0x1e8, 0x1f0, 0x1f8,
    0x380, 0x388, 0x390, 0x398, 0x3a0, 0x3a8, 0x3b0, 0x3b8, 0x3c0, 0x3c8, 0x3d0, 0x3d8, 0x3e0, 0x3e8, 0x3f0, 0x3f8,
    0x780, 0x788, 0x790, 0x798, 0x7a0, 0x7a8, 0x7b0, 0x7b8, 0x7c0, 0x7c8, 0x7d0, 0x7d8, 0x7e0, 0x7e8, 0x7f0, 0x7f8,
    0xf80, 0xf88, 0xf90, 0xf98, 0xfa0, 0xfa8, 0xfb0, 0xfb8, 0xfc0, 0xfc8, 0xfd0, 0xfd8, 0xfe0, 0xfe8, 0xff0, 0xff8
};

static GK_VOID cmos_again_calc_table(VI_PIPE ViPipe, GK_U32 *pu32AgainLin, GK_U32 *pu32AgainDb)
{
    int i;

    CMOS_CHECK_POINTER_VOID(pu32AgainLin);
    CMOS_CHECK_POINTER_VOID(pu32AgainDb);

    if (*pu32AgainLin >= again_index[127]) {
        *pu32AgainLin = again_index[127];
        *pu32AgainDb = again_table[127];
        return;
    }

    for (i = 1; i < 128; i++) {
        if (*pu32AgainLin < again_index[i]) {
            *pu32AgainLin = again_index[i - 1];
            *pu32AgainDb = again_table[i - 1];
            break;
        }
    }

    return;
}

static GK_VOID cmos_dgain_calc_table(VI_PIPE ViPipe, GK_U32 *pu32DgainLin, GK_U32 *pu32DgainDb)
{
    int i;

    CMOS_CHECK_POINTER_VOID(pu32DgainLin);
    CMOS_CHECK_POINTER_VOID(pu32DgainDb);

    if (*pu32DgainLin >= dgain_index[79]) {
        *pu32DgainLin = dgain_index[79];
        *pu32DgainDb = dgain_table[79];
        return;
    }

    for (i = 1; i < 80; i++) {
        if (*pu32DgainLin < dgain_index[i]) {
            *pu32DgainLin = dgain_index[i - 1];
            *pu32DgainDb = dgain_table[i - 1];
            break;
        }
    }

    return;
}

static GK_VOID cmos_gains_update(VI_PIPE ViPipe, GK_U32 u32Again, GK_U32 u32Dgain)
{
    GK_U8 u8Reg0x3e09, u8Reg0x3e08, u8Reg0x3e07, u8Reg0x3e06;
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    SC3235_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u8Reg0x3e06 = HIGH_8BITS(u32Dgain);
    u8Reg0x3e07 = LOW_8BITS(u32Dgain);
    u8Reg0x3e08 = HIGH_8BITS(u32Again);
    u8Reg0x3e09 = LOW_8BITS(u32Again);

    pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = u8Reg0x3e09;
    pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = u8Reg0x3e08;
    pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = u8Reg0x3e07;
    pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = u8Reg0x3e06;

    // logic
    if (u8Reg0x3e08 < 0x07) { // 0x0320~0x033f  gain<2x
        pstSnsState->astRegsInfo[0].astI2cData[10].u32Data =  0x18;  // R0x3632
        pstSnsState->astRegsInfo[0].astI2cData[11].u32Data =  0x50;  // R0x3306
        pstSnsState->astRegsInfo[0].astI2cData[12].u32Data =  0xd0;  // R0x330b
    } else if ((u8Reg0x3e08 >= 0x07) && (u8Reg0x3e08 < 0x0f)) { // 0x0720~0x073f 4x>gain >= 2x
        pstSnsState->astRegsInfo[0].astI2cData[10].u32Data =  0x58;  // R0x3632
        pstSnsState->astRegsInfo[0].astI2cData[11].u32Data =  0x58;  // R0x3306
        pstSnsState->astRegsInfo[0].astI2cData[12].u32Data =  0xd8;  // R0x330b
    } else if ((0x0f <= u8Reg0x3e08) && (0x1f > u8Reg0x3e08)) { // 0x0f20~0x0f3f 8x>gain >= 4x
        pstSnsState->astRegsInfo[0].astI2cData[10].u32Data =  0x58;  // R0x3632
        pstSnsState->astRegsInfo[0].astI2cData[11].u32Data =  0x58;  // R0x3306
        pstSnsState->astRegsInfo[0].astI2cData[12].u32Data =  0xd8;  // R0x330b
    } else if ((0x1f == u8Reg0x3e08) && (0x00 == u8Reg0x3e06)
               &&  (0x80 == u8Reg0x3e07)) { // 0x1f20~0x1f3f  15.5x>gain >= 8x
        pstSnsState->astRegsInfo[0].astI2cData[10].u32Data =  0xd8;  // R0x3632
        pstSnsState->astRegsInfo[0].astI2cData[11].u32Data =  0x5c;  // R0x3306
        pstSnsState->astRegsInfo[0].astI2cData[12].u32Data =  0xdb;  // R0x330b
    } else {
        pstSnsState->astRegsInfo[0].astI2cData[10].u32Data =  0xd8;  // R0x3632
        pstSnsState->astRegsInfo[0].astI2cData[11].u32Data =  0x5c;  // R0x3306
        pstSnsState->astRegsInfo[0].astI2cData[12].u32Data =  0xdb;  // R0x330b
    }
    return;
}

static GK_VOID cmos_get_inttime_max(VI_PIPE ViPipe, GK_U16 u16ManRatioEnable, GK_U32 *au32Ratio, GK_U32 *au32IntTimeMax, GK_U32 *au32IntTimeMin, GK_U32 *pu32LFMaxIntTime)
{
    return;
}

/* Only used in LINE_WDR mode */
static GK_VOID cmos_ae_fswdr_attr_set(VI_PIPE ViPipe, AE_FSWDR_ATTR_S *pstAeFSWDRAttr)
{
    CMOS_CHECK_POINTER_VOID(pstAeFSWDRAttr);

    genFSWDRMode[ViPipe] = pstAeFSWDRAttr->enFSWDRMode;
    gu32MaxTimeGetCnt[ViPipe] = 0;

    return;
}

static GK_S32 cmos_init_ae_exp_function(AE_SENSOR_EXP_FUNC_S *pstExpFuncs)
{
    CMOS_CHECK_POINTER(pstExpFuncs);

    memset(pstExpFuncs, 0, sizeof(AE_SENSOR_EXP_FUNC_S));

    pstExpFuncs->pfn_cmos_get_ae_default     = cmos_get_ae_default;
    pstExpFuncs->pfn_cmos_fps_set            = cmos_fps_set;
    pstExpFuncs->pfn_cmos_slow_framerate_set = cmos_slow_framerate_set;
    pstExpFuncs->pfn_cmos_inttime_update     = cmos_inttime_update;
    pstExpFuncs->pfn_cmos_gains_update       = cmos_gains_update;
    pstExpFuncs->pfn_cmos_again_calc_table   = cmos_again_calc_table;
    pstExpFuncs->pfn_cmos_dgain_calc_table   = cmos_dgain_calc_table;
    pstExpFuncs->pfn_cmos_get_inttime_max    = cmos_get_inttime_max;
    pstExpFuncs->pfn_cmos_ae_fswdr_attr_set  = cmos_ae_fswdr_attr_set;

    return GK_SUCCESS;
}

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN                          0
#define GOLDEN_BGAIN                          0
static GK_S32 cmos_get_awb_default(VI_PIPE ViPipe, AWB_SENSOR_DEFAULT_S *pstAwbSnsDft)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstAwbSnsDft);
    SC3235_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset(pstAwbSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));
    pstAwbSnsDft->u16WbRefTemp = 5000;

    pstAwbSnsDft->au16GainOffset[0] = 0x18D;
    pstAwbSnsDft->au16GainOffset[1] = 0x100;
    pstAwbSnsDft->au16GainOffset[2] = 0x100;
    pstAwbSnsDft->au16GainOffset[3] = 0x19F;

    pstAwbSnsDft->as32WbPara[0] = 113;
    pstAwbSnsDft->as32WbPara[1] = 47;
    pstAwbSnsDft->as32WbPara[2] = -96;
    pstAwbSnsDft->as32WbPara[3] = 244446;
    pstAwbSnsDft->as32WbPara[4] = 128;
    pstAwbSnsDft->as32WbPara[5] = -194273;
    pstAwbSnsDft->u16GoldenRgain = GOLDEN_RGAIN;
    pstAwbSnsDft->u16GoldenBgain = GOLDEN_BGAIN;

    switch (pstSnsState->enWDRMode) {
        default:
        case WDR_MODE_NONE:
            memcpy(&pstAwbSnsDft->stCcm, &g_stAwbCcm, sizeof(AWB_CCM_S));
            memcpy(&pstAwbSnsDft->stAgcTbl, &g_stAwbAgcTable, sizeof(AWB_AGC_TABLE_S));
            break;
    }

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

static ISP_CMOS_DNG_COLORPARAM_S g_stDngColorParam = {
    {378, 256, 430},
    {439, 256, 439}
};

static GK_S32 cmos_get_isp_default(VI_PIPE ViPipe, ISP_CMOS_DEFAULT_S *pstDef)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstDef);
    SC3235_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    memset(pstDef, 0, sizeof(ISP_CMOS_DEFAULT_S));

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
            memcpy(&pstDef->stNoiseCalibration,   &g_stIspNoiseCalibratio, sizeof(ISP_CMOS_NOISE_CALIBRATION_S));
            break;
    }

    pstDef->stSensorMode.u32SensorID = SC3235_ID;
    pstDef->stSensorMode.u8SensorMode = pstSnsState->u8ImgMode;


    memcpy(&pstDef->stDngColorParam, &g_stDngColorParam, sizeof(ISP_CMOS_DNG_COLORPARAM_S));

    switch (pstSnsState->u8ImgMode) {
        default:
        case SC3235_SENSOR_1296P_30FPS_LINEAR_MODE:
            pstDef->stSensorMode.stDngRawFormat.u8BitsPerSample = 10;
            pstDef->stSensorMode.stDngRawFormat.u32WhiteLevel = 4095;
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
    GK_S32  i;
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstBlackLevel);
    SC3235_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    /* Don't need to update black level when iso change */
    pstBlackLevel->bUpdate = GK_FALSE;

    for (i = 0; i < 4; i++) {
        pstBlackLevel->au16BlackLevel[i] = 0x100;
    }

    return GK_SUCCESS;
}

static GK_VOID cmos_set_pixel_detect(VI_PIPE ViPipe, GK_BOOL bEnable)
{
    GK_U32 u32FullLines_5Fps, u32MaxIntTime_5Fps;
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    SC3235_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    u32FullLines_5Fps = (SC3235_VMAX_1296P30_LINEAR * 30) / 5;
    u32MaxIntTime_5Fps = pstSnsState->u32FLStd - 8;

    if (bEnable) { /* setup for ISP pixel calibration mode */
        sc3235_write_register(ViPipe, SC3235_AGAIN_L_ADDR, 0x20);
        sc3235_write_register(ViPipe, SC3235_AGAIN_H_ADDR, 0x03);
        sc3235_write_register(ViPipe, SC3235_DGAIN_L_ADDR, 0x80);
        sc3235_write_register(ViPipe, SC3235_DGAIN_H_ADDR, 0x00);
        sc3235_write_register(ViPipe, SC3235_EXP_L_ADDR,  LOWER_4BITS(u32MaxIntTime_5Fps) << 4);
        sc3235_write_register(ViPipe, SC3235_EXP_M_ADDR,  HIGHER_8BITS(u32MaxIntTime_5Fps));
        sc3235_write_register(ViPipe, SC3235_EXP_H_ADDR,  HIGHER_4BITS(u32MaxIntTime_5Fps));
        sc3235_write_register(ViPipe, SC3235_VMAX_L_ADDR, LOW_8BITS(u32FullLines_5Fps));
        sc3235_write_register(ViPipe, SC3235_VMAX_H_ADDR, HIGH_8BITS(u32FullLines_5Fps));
    } else { /* setup for ISP 'normal mode' */
        pstSnsState->u32FLStd = (pstSnsState->u32FLStd > SC3235_FULL_LINES_MAX * 2) ? (SC3235_FULL_LINES_MAX * 2) : pstSnsState->u32FLStd;
        sc3235_write_register(ViPipe, SC3235_VMAX_L_ADDR, (pstSnsState->u32FLStd & 0xFF));
        sc3235_write_register(ViPipe, SC3235_VMAX_H_ADDR, (pstSnsState->u32FLStd & 0xFF00) >> 8);
        pstSnsState->bSyncInit = GK_FALSE;
    }

    return;
}

static GK_S32 cmos_set_wdr_mode(VI_PIPE ViPipe, GK_U8 u8Mode)
{
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    SC3235_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    pstSnsState->bSyncInit = GK_FALSE;

    switch (u8Mode) {
        case WDR_MODE_NONE:
            pstSnsState->enWDRMode = WDR_MODE_NONE;
            printf("linear mode\n");
            break;
        default:
            ISP_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return GK_FAILURE;
    }

    memset(pstSnsState->au32WDRIntTime, 0, sizeof(pstSnsState->au32WDRIntTime));

    return GK_SUCCESS;
}

static GK_S32 cmos_get_sns_regs_info(VI_PIPE ViPipe, ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{

    GK_S32 i;
    ISP_SNS_STATE_S *pstSnsState = GK_NULL;

    CMOS_CHECK_POINTER(pstSnsRegsInfo);
    SC3235_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    if ((pstSnsState->bSyncInit == GK_FALSE) || (pstSnsRegsInfo->bConfig == GK_FALSE)) {
        pstSnsState->astRegsInfo[0].enSnsType = ISP_SNS_I2C_TYPE;
        pstSnsState->astRegsInfo[0].unComBus.s8I2cDev = g_aunSc3235BusInfo[ViPipe].s8I2cDev;
        pstSnsState->astRegsInfo[0].u8Cfg2ValidDelayMax = 2;
        pstSnsState->astRegsInfo[0].u32RegNum = 14;

        for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
            pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = GK_TRUE;
            pstSnsState->astRegsInfo[0].astI2cData[i].u8DevAddr = sc3235_i2c_addr;
            pstSnsState->astRegsInfo[0].astI2cData[i].u32AddrByteNum = sc3235_addr_byte;
            pstSnsState->astRegsInfo[0].astI2cData[i].u32DataByteNum = sc3235_data_byte;
        }

        // Linear Mode Regs
        pstSnsState->astRegsInfo[0].astI2cData[0].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[0].u32RegAddr = SC3235_EXP_L_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[1].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[1].u32RegAddr = SC3235_EXP_M_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[2].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[2].u32RegAddr = SC3235_EXP_H_ADDR;

        pstSnsState->astRegsInfo[0].astI2cData[3].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[3].u32RegAddr = SC3235_AGAIN_L_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[4].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[4].u32RegAddr = SC3235_AGAIN_H_ADDR;

        pstSnsState->astRegsInfo[0].astI2cData[5].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[5].u32RegAddr = SC3235_DGAIN_L_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[6].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[6].u32RegAddr = SC3235_DGAIN_H_ADDR;

        pstSnsState->astRegsInfo[0].astI2cData[7].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[7].u32RegAddr = SC3235_VMAX_L_ADDR;
        pstSnsState->astRegsInfo[0].astI2cData[8].u8DelayFrmNum = 0;
        pstSnsState->astRegsInfo[0].astI2cData[8].u32RegAddr = SC3235_VMAX_H_ADDR;

        pstSnsState->astRegsInfo[0].astI2cData[9].u8DelayFrmNum  = 1;
        pstSnsState->astRegsInfo[0].astI2cData[9].u32RegAddr  = 0x3812; /* Grouphold Logic */
        pstSnsState->astRegsInfo[0].astI2cData[10].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astI2cData[10].u32RegAddr = 0x3632;
        pstSnsState->astRegsInfo[0].astI2cData[11].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astI2cData[11].u32RegAddr = 0x3306;
        pstSnsState->astRegsInfo[0].astI2cData[12].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astI2cData[12].u32RegAddr = 0x330b;
        pstSnsState->astRegsInfo[0].astI2cData[13].u8DelayFrmNum = 1;
        pstSnsState->astRegsInfo[0].astI2cData[13].u32RegAddr = 0x3812;

        pstSnsState->bSyncInit = GK_TRUE;
    } else {
        for (i = 0; i < pstSnsState->astRegsInfo[0].u32RegNum; i++) {
            if (pstSnsState->astRegsInfo[0].astI2cData[i].u32Data == pstSnsState->astRegsInfo[1].astI2cData[i].u32Data) {
                pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = GK_FALSE;
            } else {
                pstSnsState->astRegsInfo[0].astI2cData[i].bUpdate = GK_TRUE;
            }
        }

        if (GK_TRUE == (pstSnsState->astRegsInfo[0].astI2cData[3].bUpdate\
                        || pstSnsState->astRegsInfo[0].astI2cData[4].bUpdate)
            || pstSnsState->astRegsInfo[0].astI2cData[5].bUpdate
            || pstSnsState->astRegsInfo[0].astI2cData[6].bUpdate) {
            pstSnsState->astRegsInfo[0].astI2cData[9].bUpdate  = GK_TRUE;
            pstSnsState->astRegsInfo[0].astI2cData[10].bUpdate = GK_TRUE;
            pstSnsState->astRegsInfo[0].astI2cData[11].bUpdate = GK_TRUE;
            pstSnsState->astRegsInfo[0].astI2cData[12].bUpdate = GK_TRUE;
            pstSnsState->astRegsInfo[0].astI2cData[13].bUpdate = GK_TRUE;
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
    SC3235_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER(pstSnsState);

    u8SensorImageMode = pstSnsState->u8ImgMode;
    pstSnsState->bSyncInit = GK_FALSE;

    if (pstSensorImageMode->f32Fps <= 30) {
        if (pstSnsState->enWDRMode == WDR_MODE_NONE) {
            if (SC3235_RES_IS_1296P(pstSensorImageMode->u16Width, pstSensorImageMode->u16Height)) {
                u8SensorImageMode     = SC3235_SENSOR_1296P_30FPS_LINEAR_MODE;
                pstSnsState->u32FLStd = SC3235_VMAX_1296P30_LINEAR * 2;
            } else {
                ISP_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
                          pstSensorImageMode->u16Width,
                          pstSensorImageMode->u16Height,
                          pstSensorImageMode->f32Fps,
                          pstSnsState->enWDRMode);
                return GK_FAILURE;
            }
        } else {
            ISP_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n",
                      pstSensorImageMode->u16Width,
                      pstSensorImageMode->u16Height,
                      pstSensorImageMode->f32Fps,
                      pstSnsState->enWDRMode);
            return GK_FAILURE;
        }
    } else {}

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

    SC3235_SENSOR_GET_CTX(ViPipe, pstSnsState);
    CMOS_CHECK_POINTER_VOID(pstSnsState);

    pstSnsState->bInit     = GK_FALSE;
    pstSnsState->bSyncInit = GK_FALSE;
    pstSnsState->u8ImgMode = SC3235_SENSOR_1296P_30FPS_LINEAR_MODE;
    pstSnsState->enWDRMode = WDR_MODE_NONE;
    pstSnsState->u32FLStd  = SC3235_VMAX_1296P30_LINEAR * 2;
    pstSnsState->au32FL[0] = SC3235_VMAX_1296P30_LINEAR * 2;
    pstSnsState->au32FL[1] = SC3235_VMAX_1296P30_LINEAR * 2;

    memset(&pstSnsState->astRegsInfo[0], 0, sizeof(ISP_SNS_REGS_INFO_S));
    memset(&pstSnsState->astRegsInfo[1], 0, sizeof(ISP_SNS_REGS_INFO_S));

    pstSnsState->astRegsInfo[0].astI2cData[0].u32Data = 0x40;   // intt
    pstSnsState->astRegsInfo[0].astI2cData[1].u32Data = 0xa8;
    pstSnsState->astRegsInfo[0].astI2cData[2].u32Data = 0x00;

    pstSnsState->astRegsInfo[0].astI2cData[3].u32Data = 0x20;   // gain
    pstSnsState->astRegsInfo[0].astI2cData[4].u32Data = 0x03;
    pstSnsState->astRegsInfo[0].astI2cData[5].u32Data = 0x80;
    pstSnsState->astRegsInfo[0].astI2cData[6].u32Data = 0x00;

    pstSnsState->astRegsInfo[0].astI2cData[7].u32Data = 0x46;   // vts
    pstSnsState->astRegsInfo[0].astI2cData[8].u32Data = 0x05;

    pstSnsState->astRegsInfo[0].astI2cData[9].u32Data  = 0x00; // group hold
    pstSnsState->astRegsInfo[0].astI2cData[10].u32Data = 0x18;
    pstSnsState->astRegsInfo[0].astI2cData[11].u32Data = 0x50;
    pstSnsState->astRegsInfo[0].astI2cData[12].u32Data = 0xd0;
    pstSnsState->astRegsInfo[0].astI2cData[13].u32Data = 0x30;


}

static GK_S32 cmos_init_sensor_exp_function(ISP_SENSOR_EXP_FUNC_S *pstSensorExpFunc)
{
    CMOS_CHECK_POINTER(pstSensorExpFunc);

    memset(pstSensorExpFunc, 0, sizeof(ISP_SENSOR_EXP_FUNC_S));

    pstSensorExpFunc->pfn_cmos_sensor_init = sc3235_init;
    pstSensorExpFunc->pfn_cmos_sensor_exit = sc3235_exit;
    pstSensorExpFunc->pfn_cmos_sensor_global_init = sensor_global_init;
    pstSensorExpFunc->pfn_cmos_set_image_mode = cmos_set_image_mode;
    pstSensorExpFunc->pfn_cmos_set_wdr_mode = cmos_set_wdr_mode;

    pstSensorExpFunc->pfn_cmos_get_isp_default = cmos_get_isp_default;
    pstSensorExpFunc->pfn_cmos_get_isp_black_level = cmos_get_isp_black_level;
    pstSensorExpFunc->pfn_cmos_set_pixel_detect = cmos_set_pixel_detect;
    pstSensorExpFunc->pfn_cmos_get_sns_reg_info = cmos_get_sns_regs_info;

    return GK_SUCCESS;
}

/****************************************************************************
 * callback structure                                                       *
 ****************************************************************************/

static GK_S32 sc3235_set_bus_info(VI_PIPE ViPipe, ISP_SNS_COMMBUS_U unSNSBusInfo)
{
    g_aunSc3235BusInfo[ViPipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

    return GK_SUCCESS;
}

static GK_S32 sensor_ctx_init(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

    SC3235_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);

    if (pastSnsStateCtx == GK_NULL) {
        pastSnsStateCtx = (ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
        if (pastSnsStateCtx == GK_NULL) {
            ISP_TRACE(MODULE_DBG_ERR, "Isp[%d] SnsCtx malloc memory failed!\n", ViPipe);
            return ERR_CODE_ISP_NOMEM;
        }
    }

    memset(pastSnsStateCtx, 0, sizeof(ISP_SNS_STATE_S));

    SC3235_SENSOR_SET_CTX(ViPipe, pastSnsStateCtx);

    return GK_SUCCESS;
}

static GK_VOID sensor_ctx_exit(VI_PIPE ViPipe)
{
    ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

    SC3235_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);
    SENSOR_FREE(pastSnsStateCtx);
    SC3235_SENSOR_RESET_CTX(ViPipe);
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

    stSnsAttrInfo.eSensorId = SC3235_ID;

    s32Ret  = cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
    s32Ret |= GK_API_ISP_SensorRegCallBack(ViPipe, &stSnsAttrInfo, &stIspRegister);

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor register callback function failed!\n");
        return s32Ret;
    }

    s32Ret  = cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
    s32Ret |= GK_API_AE_SensorRegCallBack(ViPipe, pstAeLib, &stSnsAttrInfo, &stAeRegister);

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor register callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret  = cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
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

    s32Ret = GK_API_ISP_SensorUnRegCallBack(ViPipe, SC3235_ID);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor unregister callback function failed!\n");
        return s32Ret;
    }

    s32Ret = GK_API_AE_SensorUnRegCallBack(ViPipe, pstAeLib, SC3235_ID);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "sensor unregister callback function to ae lib failed!\n");
        return s32Ret;
    }

    s32Ret = GK_API_AWB_SensorUnRegCallBack(ViPipe, pstAwbLib, SC3235_ID);
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
    g_au16InitWBGain[ViPipe][2] = pstInitAttr->u16WBBgain;
    g_au16SampleRgain[ViPipe]   = pstInitAttr->u16SampleRgain;
    g_au16SampleBgain[ViPipe]   = pstInitAttr->u16SampleBgain;

    return GK_SUCCESS;
}

ISP_SNS_OBJ_S stSnsSc3235Obj = {
    .pfnRegisterCallback    = sensor_register_callback,
    .pfnUnRegisterCallback  = sensor_unregister_callback,
    .pfnStandby             = sc3235_standby,
    .pfnRestart             = sc3235_restart,
    .pfnMirrorFlip          = GK_NULL,
    .pfnWriteReg            = sc3235_write_register,
    .pfnReadReg             = sc3235_read_register,
    .pfnSetBusInfo          = sc3235_set_bus_info,
    .pfnSetInit             = sensor_set_init
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __SC3235_CMOS_H_ */
