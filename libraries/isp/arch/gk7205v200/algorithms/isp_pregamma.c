/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <math.h>
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_proc.h"
#include "isp_ext_config.h"
#include "math_fun.h"
#include "isp_math_utils.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define ISP_PREGAMMA_SHP_LOG_DEFAULT (8)

#define PREGAMMA_FRAC_BIT_DEP           (16)
#define PREGAMMA_DENOM_SHIFT_LOG        (20)
#define PREGAMMA_LOG_BIT_DEP            (16)

static const GK_U32 g_u32PreGammaDivDenomLog[16] = {
    52429, 55188,  58254,  61681,  65536,  69905,  74898, 80659, \
    87379, 95319, 104843, 116472, 130980, 149557, 174114, 207870
};

static const  GK_U16 g_au16PreGammaLutLog[257] = {
    0, 113, 224, 334, 442, 549, 654, 759, 861, 963, 1063, 1162, 1259, 1355, 1450,
    1544, 1636, 1727, 1817, 1905, 1992, 2078, 2163, 2246, 2329, 2410, 2490, 2568, 2646, 2722,
    2797, 2871, 2944, 3016, 3087, 3156, 3224, 3292, 3358, 3423, 3487, 3550, 3611, 3672, 3732,
    3790, 3848, 3905, 3960, 4015, 4068, 4121, 4172, 4223, 4272, 4321, 4368, 4415, 4460, 4505,
    4549, 4591, 4633, 4674, 4714, 4753, 4791, 4828, 4864, 4900, 4934, 4968, 5001, 5032, 5063,
    5093, 5123, 5151, 5178, 5205, 5231, 5256, 5280, 5303, 5326, 5348, 5368, 5388, 5408, 5426,
    5444, 5461, 5477, 5492, 5507, 5520, 5533, 5546, 5557, 5568, 5578, 5587, 5595, 5603, 5610,
    5616, 5622, 5627, 5631, 5634, 5637, 5639, 5640, 5641, 5641, 5640, 5638, 5636, 5633, 5630,
    5626, 5621, 5615, 5609, 5602, 5595, 5586, 5578, 5568, 5558, 5547, 5536, 5524, 5511, 5498,
    5484, 5470, 5455, 5439, 5422, 5406, 5388, 5370, 5351, 5332, 5312, 5291, 5270, 5249, 5226,
    5203, 5180, 5156, 5132, 5106, 5081, 5054, 5028, 5000, 4972, 4944, 4915, 4885, 4855, 4825,
    4794, 4762, 4730, 4697, 4664, 4630, 4596, 4561, 4525, 4490, 4453, 4416, 4379, 4341, 4303,
    4264, 4224, 4184, 4144, 4103, 4062, 4020, 3978, 3935, 3892, 3848, 3804, 3759, 3714, 3668,
    3622, 3575, 3528, 3481, 3433, 3384, 3335, 3286, 3236, 3186, 3135, 3084, 3033, 2981, 2928,
    2875, 2822, 2768, 2714, 2659, 2604, 2549, 2493, 2437, 2380, 2323, 2265, 2207, 2149, 2090,
    2031, 1971, 1911, 1851, 1790, 1729, 1667, 1605, 1542, 1480, 1416, 1353, 1289, 1224, 1159,
    1094, 1029, 963, 896, 830, 763, 695, 627, 559, 490, 421, 352, 282, 212, 142,
    71, 0
}; // 13bit

static const  GK_S8 g_as8PreGammaLutLogDelta[257] = {
    113, 111, 110, 108, 107, 105, 105, 102, 102, 100, 99, 97, 96, 95, 94,
    92, 91, 90, 88, 87, 86, 85, 83, 83, 81, 80, 78, 78, 76, 75,
    74, 73, 72, 71, 69, 68, 68, 66, 65, 64, 63, 61, 61, 60, 58,
    58, 57, 55, 55, 53, 53, 51, 51, 49, 49, 47, 47, 45, 45, 44,
    42, 42, 41, 40, 39, 38, 37, 36, 36, 34, 34, 33, 31, 31, 30,
    30, 28, 27, 27, 26, 25, 24, 23, 23, 22, 20, 20, 20, 18, 18,
    17, 16, 15, 15, 13, 13, 13, 11, 11, 10, 9, 8, 8, 7, 6,
    6, 5, 4, 3, 3, 2, 1, 1, 0, -1, -2, -2, -3, -3, -4,
    -5, -6, -6, -7, -7, -9, -8, -10, -10, -11, -11, -12, -13, -13, -14,
    -14, -15, -16, -17, -16, -18, -18, -19, -19, -20, -21, -21, -21, -23, -23,
    -23, -24, -24, -26, -25, -27, -26, -28, -28, -28, -29, -30, -30, -30, -31,
    -32, -32, -33, -33, -34, -34, -35, -36, -35, -37, -37, -37, -38, -38, -39,
    -40, -40, -40, -41, -41, -42, -42, -43, -43, -44, -44, -45, -45, -46, -46,
    -47, -47, -47, -48, -49, -49, -49, -50, -50, -51, -51, -51, -52, -53, -53,
    -53, -54, -54, -55, -55, -55, -56, -56, -57, -57, -58, -58, -58, -59, -59,
    -60, -60, -60, -61, -61, -62, -62, -63, -62, -64, -63, -64, -65, -65, -65,
    -65, -66, -67, -66, -67, -68, -68, -68, -69, -69, -69, -70, -70, -70, -71,
    -71, -71

}; // 8bit

/*
- Input: 32-bit int, 8-bit ShpLog parameter
- Output: 5-bit int, 20-bit frac
*/
static GK_U32 PreGammaNonStandardLog2(GK_U32 x, GK_U8 ShpLog)
{
    GK_S8  k;
    GK_U8  StepShift = 7;
    GK_U32 IntPart, FracPart;
    GK_U32 NumerOffset, DivDenomLog;
    GK_S32 Offset, LUTf;
    GK_U32 y;

    NumerOffset = (ShpLog << PREGAMMA_FRAC_BIT_DEP);
    DivDenomLog = g_u32PreGammaDivDenomLog[ShpLog];

    x = (GK_U32)MIN2((1LL << 32) - 1, (GK_U64)x + (1LL << ShpLog));

    // Determine highest non-zero bit (i.e., IntPart)
    IntPart = 0;
    for (k = 31; k >= 0; k--) {
        if (x & (1U << k)) {
            IntPart = k;
            break;
        }
    }

    // Calculate f << 15, where f = x/2^(IntPart + 1) - 1
    FracPart = (IntPart + 1) <= PREGAMMA_FRAC_BIT_DEP ? (x << (PREGAMMA_FRAC_BIT_DEP - (IntPart + 1))) : (x >> ((IntPart + 1) - PREGAMMA_FRAC_BIT_DEP));
    FracPart = FracPart & (0x7fff);     // get bit 0~14 (total 15 bits) LUT
    Offset = FracPart - ((FracPart >> StepShift) << StepShift); // 15 bit
    LUTf = (GK_S32)g_au16PreGammaLutLog[FracPart >> StepShift] + \
           (GK_S32)SignedRightShift(((GK_S64)g_as8PreGammaLutLogDelta[FracPart >> StepShift] * Offset + (1 << (StepShift - 1))), StepShift);
    y = (GK_U32)((IntPart << PREGAMMA_FRAC_BIT_DEP) + (FracPart << 1) + LUTf);
    y = (GK_U32)((((GK_U64)y - NumerOffset) * DivDenomLog) >> (PREGAMMA_DENOM_SHIFT_LOG + PREGAMMA_FRAC_BIT_DEP - PREGAMMA_LOG_BIT_DEP)); // y > NumerOffset for sure

    return y;
}

static const GK_U32 g_au32PreGammaLinearDefault[PREGAMMA_NODE_NUM] = {
    4,      16,     32,     48,     64,     80,     96,     112,    128,    160,    192,    224,    256,    288,    320,    352,
    384,    416,    448,    480,    512,    576,    640,    704,    768,    832,    896,    960,    1024,   1088,   1152,   1216,
    1280,   1344,   1408,   1472,   1536,   1664,   1792,   1920,   2048,   2176,   2304,   2432,   2560,   2688,   2816,   2944,
    3072,   3200,   3328,   3456,   3584,   3840,   4096,   4352,   4608,   4864,   5120,   5376,   5632,   5888,   6144,   6400,
    6656,   6912,   7168,   7424,   7680,   8192,   8704,   9216,   9728,   10240,  10752,  11264,  11776,  12288,  12800,  13312,
    13824,  14336,  14848,  15360,  15872,  16384,  16896,  17408,  17920,  18432,  18944,  19456,  19968,  20480,  20992,  21504,
    22016,  22528,  23040,  23552,  24064,  24576,  25600,  26624,  27648,  28672,  29696,  30720,  31744,  32768,  33792,  34816,
    35840,  36864,  37888,  38912,  39936,  40960,  41984,  43008,  44032,  45056,  46080,  47104,  48128,  49152,  50176,  51200,
    52224,  53248,  54272,  55296,  56320,  57344,  59392,  61440,  63488,  65536,  67584,  69632,  71680,  73728,  75776,  77824,
    79872,  81920,  83968,  86016,  88064,  90112,  92160,  94208,  96256,  98304,  100352, 102400, 104448, 106496, 108544, 110592,
    112640, 114688, 116736, 118784, 120832, 122880, 126976, 131072, 135168, 139264, 143360, 147456, 151552, 155648, 159744, 163840,
    167936, 172032, 176128, 180224, 184320, 188416, 192512, 196608, 200704, 204800, 208896, 212992, 217088, 221184, 225280, 229376,
    233472, 237568, 241664, 245760, 249856, 253952, 258048, 262144, 266240, 270336, 278528, 286720, 294912, 303104, 311296, 319488,
    327680, 335872, 344064, 352256, 360448, 368640, 376832, 385024, 393216, 401408, 409600, 417792, 425984, 434176, 442368, 450560,
    458752, 466944, 475136, 483328, 491520, 499712, 507904, 516096, 524288, 540672, 557056, 573440, 589824, 606208, 622592, 638976,
    655360, 671744, 688128, 704512, 720896, 737280, 753664, 770048, 786432, 819200, 851968, 884736, 917504, 950272, 983040, 1015808,
    1048575
};

static const GK_U8 g_au8PregammaSegIdxBase[ISP_PREGAMMA_SEG_NUM] = {0, 4, 12, 24, 38, 53, 77, 105, 135, 168, 200, 224};
static const GK_U16 g_au16PregammaSegMaxVal[ISP_PREGAMMA_SEG_NUM] = {1, 4, 12, 28, 60, 192, 448, 960, 2112, 4096, 6144, 8192};

typedef struct ISP_PREGAMMA_S {
    GK_BOOL bEnable;
    GK_BOOL bLutUpdate;

    GK_U8   u8ShpLog;
    GK_U8   u8ShpLogPrev;
    GK_U16  au16PreGammaHw[PREGAMMA_NODE_NUM];
} ISP_PREGAMMA_S;

ISP_PREGAMMA_S g_astPreGammaCtx[ISP_MAX_PIPE_NUM] = {{0}};
#define PREGAMMA_GET_CTX(dev, pstCtx)   pstCtx = &g_astPreGammaCtx[dev]

static GK_S32 PreGammaCheckCmosParam(VI_PIPE ViPipe, const ISP_CMOS_PREGAMMA_S *pstPreGamma)
{
    GK_U16 i;

    ISP_CHECK_BOOL(pstPreGamma->bEnable);

    for (i = 0; i < PREGAMMA_NODE_NUM; i++) {
        if (pstPreGamma->au32PreGamma[i] > ISP_PREGAMMA_LUT_MAX) {
            ISP_TRACE(MODULE_DBG_ERR, "err au32PreGamma[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
    }

    return GK_SUCCESS;
}

static GK_S32 PreGammaRegsInitialize(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfg)
{
    GK_U8  u8BlockNum;

    GK_S32 s32Ret;
    GK_U16 i;

    ISP_PREGAMMA_REG_CFG_S *pstPreGammaCfg = GK_NULL;
    ISP_PREGAMMA_S         *pstPreGammaCtx = GK_NULL;
    ISP_CTX_S              *pstIspCtx      = GK_NULL;
    ISP_CMOS_DEFAULT_S     *pstSnsDft      = GK_NULL;

    ISP_SensorGetDefault(ViPipe, &pstSnsDft);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    PREGAMMA_GET_CTX(ViPipe, pstPreGammaCtx);

    pstPreGammaCtx->bLutUpdate = GK_FALSE;

    u8BlockNum = pstIspCtx->stBlockAttr.u8BlockNum;

    if (pstSnsDft->unKey.bit1Drc) {
        pstPreGammaCtx->u8ShpLog = pstSnsDft->pstDrc->u8ContrastControl;
    } else {
        pstPreGammaCtx->u8ShpLog = ISP_PREGAMMA_SHP_LOG_DEFAULT;
    }
    pstPreGammaCtx->u8ShpLogPrev = pstPreGammaCtx->u8ShpLog;

    /* Read from CMOS */
    if (pstSnsDft->unKey.bit1PreGamma) {
        ISP_CHECK_POINTER(pstSnsDft->pstPreGamma);

        s32Ret = PreGammaCheckCmosParam(ViPipe, pstSnsDft->pstPreGamma);
        if (s32Ret != GK_SUCCESS) {
            return s32Ret;
        }

        for (i = 0; i < PREGAMMA_NODE_NUM; i++) {
            pstPreGammaCtx->au16PreGammaHw[i] = (GK_U16)MIN2(65535, PreGammaNonStandardLog2(pstSnsDft->pstPreGamma->au32PreGamma[i], pstPreGammaCtx->u8ShpLog));
        }
#ifndef VREG_OLD
        for (i = 0; i < PREGAMMA_NODE_NUM; i++) {
            ext_system_pregamma_lut_write(ViPipe, i, pstSnsDft->pstPreGamma->au32PreGamma[i]);
        }
#else
        u32Addr   = VREG_PREGAMMA_BASE(ViPipe) + 0x4;
        pVirtAddr = VReg_GetVirtAddr(u32Addr);
        memcpy((GK_U32 *)pVirtAddr, pstSnsDft->pstPreGamma->au32PreGamma, PREGAMMA_NODE_NUM * sizeof(GK_U32));
#endif
        pstPreGammaCtx->bEnable    = pstSnsDft->pstPreGamma->bEnable;
    } else {
        for (i = 0; i < PREGAMMA_NODE_NUM; i++) {
            pstPreGammaCtx->au16PreGammaHw[i] = (GK_U16)MIN2(65535, PreGammaNonStandardLog2(g_au32PreGammaLinearDefault[i], pstPreGammaCtx->u8ShpLog));
        }


#ifndef VREG_OLD
        for (i = 0; i < PREGAMMA_NODE_NUM; i++) {
            ext_system_pregamma_lut_write(ViPipe, i, g_au32PreGammaLinearDefault[i]);
        }
#else
        u32Addr   = VREG_PREGAMMA_BASE(ViPipe) + 0x4;
        pVirtAddr = VReg_GetVirtAddr(u32Addr);
        memcpy((GK_U32 *)pVirtAddr, g_au32PreGammaLinearDefault, PREGAMMA_NODE_NUM * sizeof(GK_U32));
#endif
        pstPreGammaCtx->bEnable =  GK_FALSE;
    }

    for (i = 0; i < u8BlockNum; i++) {
        pstPreGammaCfg = &pstRegCfg->stAlgRegCfg[i].stPreGammaCfg;

        /* Static */
        pstPreGammaCfg->stStaticRegCfg.bStaticResh   = GK_TRUE;
        memcpy(pstPreGammaCfg->stStaticRegCfg.au8SegIdxBase, g_au8PregammaSegIdxBase, ISP_PREGAMMA_SEG_NUM * sizeof(GK_U8));
        memcpy(pstPreGammaCfg->stStaticRegCfg.au16SegMaxVal,  g_au16PregammaSegMaxVal,  ISP_PREGAMMA_SEG_NUM * sizeof(GK_U16));

        /* Dynamic */
        // Enable Gamma
        pstPreGammaCfg->bPreGammaEn                       = GK_FALSE;
        pstPreGammaCfg->stDynaRegCfg.bPreGammaLutUpdateEn = GK_TRUE;
        pstPreGammaCfg->stDynaRegCfg.u32UpdateIndex       = 1;
        pstPreGammaCfg->stDynaRegCfg.u8BufId              = 0;
        memcpy(pstPreGammaCfg->stDynaRegCfg.au16PreGammaLUT, pstPreGammaCtx->au16PreGammaHw, PREGAMMA_NODE_NUM * sizeof(GK_U16));
    }

    pstRegCfg->unKey.bit1PreGammaCfg = 1;

    return GK_SUCCESS;
}

static GK_VOID PreGammaExtRegsInitialize(VI_PIPE ViPipe)
{
    ISP_PREGAMMA_S         *pstPreGammaCtx = GK_NULL;

    PREGAMMA_GET_CTX(ViPipe, pstPreGammaCtx);

    ext_system_pregamma_en_write(ViPipe, pstPreGammaCtx->bEnable);
    ext_system_pregamma_lut_update_write(ViPipe, GK_FALSE);
}


GK_S32 ISP_PreGammaInit(VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
    GK_S32 s32Ret;
    ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;


    s32Ret = PreGammaRegsInitialize(ViPipe, pstRegCfg);
    if (s32Ret != GK_SUCCESS) {
        return s32Ret;
    }

    PreGammaExtRegsInitialize(ViPipe);

    return GK_SUCCESS;
}

static GK_VOID ISP_PreGammaWdrModeSet(VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
    GK_U8  i;
    GK_U32 au32UpdateIdx[ISP_STRIPING_MAX_NUM] = {0};
    ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;

    for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
        au32UpdateIdx[i] = pstRegCfg->stAlgRegCfg[i].stPreGammaCfg.stDynaRegCfg.u32UpdateIndex;
    }

    ISP_PreGammaInit(ViPipe, pRegCfg);

    for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
        pstRegCfg->stAlgRegCfg[i].stPreGammaCfg.stDynaRegCfg.u32UpdateIndex = au32UpdateIdx[i] + 1;
        pstRegCfg->stAlgRegCfg[i].stPreGammaCfg.stDynaRegCfg.bSwitchMode          = GK_TRUE;
    }
}

static GK_S32 PreGammaReadExtRegs(VI_PIPE ViPipe)
{
    ISP_PREGAMMA_S *pstPreGammaCtx = GK_NULL;

    PREGAMMA_GET_CTX(ViPipe, pstPreGammaCtx);

    pstPreGammaCtx->u8ShpLogPrev = pstPreGammaCtx->u8ShpLog;
    pstPreGammaCtx->u8ShpLog = ext_system_drc_shp_log_read(ViPipe);

    pstPreGammaCtx->bLutUpdate = ext_system_pregamma_lut_update_read(ViPipe);
    ext_system_pregamma_lut_update_write(ViPipe, GK_FALSE);

    return GK_SUCCESS;
}

GK_S32 PreGammaProcWrite(VI_PIPE ViPipe, ISP_CTRL_PROC_WRITE_S *pstProc)
{
    ISP_CTRL_PROC_WRITE_S stProcTmp;
    ISP_PREGAMMA_S *pstPreGamma = GK_NULL;

    PREGAMMA_GET_CTX(ViPipe, pstPreGamma);

    if ((pstProc->pcProcBuff == GK_NULL)
        || (pstProc->u32BuffLen == 0)) {
        return GK_FAILURE;
    }

    stProcTmp.pcProcBuff = pstProc->pcProcBuff;
    stProcTmp.u32BuffLen = pstProc->u32BuffLen;

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
                    "-----PreGamma INFO--------------------------------------------------------------\n");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen, "%16s\n", "Enable");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen, "%16u\n", pstPreGamma->bEnable);

    pstProc->u32WriteLen += 1;

    return GK_SUCCESS;
}

static GK_BOOL __inline CheckPreGammaOpen(ISP_PREGAMMA_S *pstPreGamma)
{
    return (pstPreGamma->bEnable == GK_TRUE);
}

GK_S32 ISP_PreGammaRun(VI_PIPE ViPipe, const GK_VOID *pStatInfo,
                       GK_VOID *pRegCfg, GK_S32 s32Rsv)
{
    GK_U16 i, j;
    ISP_PREGAMMA_S *pstPreGammaCtx = GK_NULL;
    ISP_REG_CFG_S *pstReg = (ISP_REG_CFG_S *)pRegCfg;
    ISP_PREGAMMA_DYNA_CFG_S   *pstDynaRegCfg = GK_NULL;

    GK_U32 u32PreGammaVal = 0;

    PREGAMMA_GET_CTX(ViPipe, pstPreGammaCtx);

    pstPreGammaCtx->bEnable = ext_system_pregamma_en_read(ViPipe);

    for (i = 0; i < pstReg->u8CfgNum; i++) {
        pstReg->stAlgRegCfg[i].stPreGammaCfg.bPreGammaEn = pstPreGammaCtx->bEnable;
    }

    pstReg->unKey.bit1PreGammaCfg = 1;

    /* check hardware setting */
    if (!CheckPreGammaOpen(pstPreGammaCtx)) {
        return GK_SUCCESS;
    }

    PreGammaReadExtRegs(ViPipe);

    for (i = 0; i < pstReg->u8CfgNum; i++) {
        pstDynaRegCfg = &pstReg->stAlgRegCfg[i].stPreGammaCfg.stDynaRegCfg;
        if (pstPreGammaCtx->bLutUpdate || pstPreGammaCtx->u8ShpLogPrev != pstPreGammaCtx->u8ShpLog) {
            for (j = 0; j < PREGAMMA_NODE_NUM; j++) {
                u32PreGammaVal = MIN2(ext_system_pregamma_lut_read(ViPipe, j), ISP_PREGAMMA_LUT_MAX);
                pstDynaRegCfg->au16PreGammaLUT[j] = (GK_U16)MIN2(65535, PreGammaNonStandardLog2(u32PreGammaVal, pstPreGammaCtx->u8ShpLog));
            }

            pstDynaRegCfg->bPreGammaLutUpdateEn = GK_TRUE;
            pstDynaRegCfg->u32UpdateIndex      += 1;
        }
    }

    return GK_SUCCESS;
}

GK_S32 ISP_PreGammaCtrl(VI_PIPE ViPipe, GK_U32 u32Cmd, GK_VOID *pValue)
{
    ISP_REGCFG_S  *pRegCfg   = GK_NULL;

    switch (u32Cmd) {
        case ISP_WDR_MODE_SET :
            ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
            ISP_CHECK_POINTER(pRegCfg);
            ISP_PreGammaWdrModeSet(ViPipe, (GK_VOID *)&pRegCfg->stRegCfg);
            break;
        case ISP_PROC_WRITE:
            PreGammaProcWrite(ViPipe, (ISP_CTRL_PROC_WRITE_S *)pValue);
            break;
        default :
            break;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_PreGammaExit(VI_PIPE ViPipe)
{
    GK_U8 i;
    ISP_REGCFG_S *pRegCfg   = GK_NULL;

    ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

    for (i = 0; i < pRegCfg->stRegCfg.u8CfgNum; i++) {
        pRegCfg->stRegCfg.stAlgRegCfg[i].stPreGammaCfg.bPreGammaEn = GK_FALSE;
    }

    pRegCfg->stRegCfg.unKey.bit1PreGammaCfg = 1;
    return GK_SUCCESS;
}


GK_S32 ISP_AlgRegisterPreGamma(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_ALG_NODE_S *pstAlgs = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_ALG_CHECK(pstIspCtx->unAlgKey.bit1PreGamma);
    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_PREGAMMA;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_PreGammaInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_PreGammaRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_PreGammaCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_PreGammaExit;
    pstAlgs->bUsed = GK_TRUE;

    return GK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
