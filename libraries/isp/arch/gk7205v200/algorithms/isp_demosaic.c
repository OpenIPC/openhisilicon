/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_proc.h"
#include "isp_math_utils.h"
#include <math.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define ISP_DEMOSAIC_BITDEPTH (12)

static const  GK_U32 au32DemosaicIsoLut[ISP_AUTO_ISO_STRENGTH_NUM] = {100, 200, 400, 800, 1600, 3200, 6400, 12800, 25600, 51200, 102400, 204800, 409600, 819200, 1638400, 3276800};
static const  GK_S32 g_as32DemosaicFilterCoef[7][3] = {{ -1, 4, 26}, { -1, 5, 24}, { -1, 6, 22}, { -1, 7, 20}, {0, 7, 18}, { -1, 8, 18}, {0, 8, 16}};

static const  GK_U8  g_au8NonDirStr[ISP_AUTO_ISO_STRENGTH_NUM]             = {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64};
static const  GK_U8  g_au8NonDirMFDetailEhcStr[ISP_AUTO_ISO_STRENGTH_NUM]  = {32, 24, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16};
static const  GK_U8  g_au8NonDirHFDetailEhcStr[ISP_AUTO_ISO_STRENGTH_NUM]  = {0, 0, 3, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7};
static const  GK_U8  g_au8DetailSmoothRange[ISP_AUTO_ISO_STRENGTH_NUM]     = {2, 2, 2, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 7};
static const  GK_U8  g_au8ColorNoiseThdY[ISP_AUTO_ISO_STRENGTH_NUM]        = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
static const  GK_U8  g_au8ColorNoiseStrY[ISP_AUTO_ISO_STRENGTH_NUM]        = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
static const  GK_U8  g_au8InitColorNoiseThdY[ISP_AUTO_ISO_STRENGTH_NUM]    = { 1,  1, 14, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15};
static const  GK_U8  g_au8InitColorNoiseStrY[ISP_AUTO_ISO_STRENGTH_NUM]    = {10, 10, 32, 48, 56, 58, 60, 63, 63, 63, 63, 63, 63, 63, 63, 63};
static const  GK_U8  g_au8ColorNoiseThdF[ISP_AUTO_ISO_STRENGTH_NUM]        = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static const  GK_U8  g_au8ColorNoiseStrF[ISP_AUTO_ISO_STRENGTH_NUM]        = {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8};

typedef struct ISP_DEMOSAIC_AUTO_S {
    GK_U8   au8NonDirStr[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8   au8NonDirMFDetailEhcStr[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8   au8NonDirHFDetailEhcStr[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8   au8DetailSmoothRange[ISP_AUTO_ISO_STRENGTH_NUM];
	GK_U8   au8ColorNoiseThdF[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8   au8ColorNoiseStrF[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8   au8ColorNoiseThdY[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8   au8ColorNoiseStrY[ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_DEMOSAIC_AUTO_S;

typedef struct ISP_DEMOSAIC_MANUAL_S {
    GK_U8   u8NonDirStr;
    GK_U8   u8NonDirMFDetailEhcStr;
    GK_U8   u8NonDirHFDetailEhcStr;
    GK_U8   u8DetailSmoothRange;
	GK_U8   u8ColorNoiseThdF;
    GK_U8   u8ColorNoiseStrF;
    GK_U8   u8ColorNoiseThdY;
    GK_U8   u8ColorNoiseStrY;
} ISP_DEMOSAIC_MANUAL_S;

typedef struct ISP_DEMOSAIC_S {
    // Processing Depth
    GK_BOOL  bEnable;           // u1.0
    GK_BOOL  bVhdmEnable;       // u1.0
    GK_BOOL  bNddmEnable;       // u1.0
    GK_BOOL  bGFCtrlEnable;     // u1.0
    GK_BOOL  bInit;
    GK_BOOL  bDemAttrUpdate;

    GK_U8    u8BitDepthPrc;    // u5.0
    GK_U8    u8WdrMode;
    GK_U16   u16NddmStrength;
    GK_U32   au32sigma[ISP_DEMOSAIC_LUT_LENGTH];

    GK_U8    au8LutAwbGFGainLow [ISP_AUTO_ISO_STRENGTH_NUM]; // u3.4,
    GK_U8    au8LutAwbGFGainHig [ISP_AUTO_ISO_STRENGTH_NUM]; // u3.4,
    GK_U8    au8LutAwbGFGainMax [ISP_AUTO_ISO_STRENGTH_NUM]; // u4.0,
    GK_U8    au8LutCcHFMaxRatio [ISP_AUTO_ISO_STRENGTH_NUM]; // u5.0, 0~16
    GK_U8    au8LutCcHFMinRatio [ISP_AUTO_ISO_STRENGTH_NUM]; // u5.0, 0~16
    GK_S8    as8LutFcrGFGain    [ISP_AUTO_ISO_STRENGTH_NUM]; // s3.2, fcr control
    GK_U16   au16LutDeSatLow    [ISP_AUTO_ISO_STRENGTH_NUM]; // u9.0, 0~256
    GK_U16   au16LutDeSatProtTh [ISP_AUTO_ISO_STRENGTH_NUM]; // u10.0, 0~1023
    GK_U16   au16LutFcrDetLow   [ISP_AUTO_ISO_STRENGTH_NUM]; // u12.0,  fcr det thresh
    GK_U16   au16HfIntpBlurThLow[ISP_AUTO_ISO_STRENGTH_NUM]; // u10.0
    GK_U16   au16HfIntpBlurThHig[ISP_AUTO_ISO_STRENGTH_NUM]; // u10.0,

    GK_U8   u8ProcNonDirStr;
    GK_U8   u8ProcNonDirMFDetailEhcStr;
    GK_U8   u8ProcNonDirHFDetailEhcStr;
    GK_U8   u8ProcDetailSmoothRange;

    ISP_OP_TYPE_E enOpType;
    ISP_DEMOSAIC_AUTO_S  stAuto;
    ISP_DEMOSAIC_MANUAL_S stManual;
} ISP_DEMOSAIC_S;

ISP_DEMOSAIC_S *g_pastDemosaicCtx[ISP_MAX_PIPE_NUM] = {GK_NULL};

#define DEMOSAIC_GET_CTX(dev, pstCtx)   (pstCtx = g_pastDemosaicCtx[dev])
#define DEMOSAIC_SET_CTX(dev, pstCtx)   (g_pastDemosaicCtx[dev] = pstCtx)
#define DEMOSAIC_RESET_CTX(dev)         (g_pastDemosaicCtx[dev] = GK_NULL)

GK_S32 DemosaicCtxInit(VI_PIPE ViPipe)
{
    ISP_DEMOSAIC_S *pastDemosaicCtx = GK_NULL;

    DEMOSAIC_GET_CTX(ViPipe, pastDemosaicCtx);

    if (pastDemosaicCtx == GK_NULL) {
        pastDemosaicCtx = (ISP_DEMOSAIC_S *)ISP_MALLOC(sizeof(ISP_DEMOSAIC_S));
        if (pastDemosaicCtx == GK_NULL) {
            ISP_TRACE(MODULE_DBG_ERR, "Isp[%d] DemosaicCtx malloc memory failed!\n", ViPipe);
            return ERR_CODE_ISP_NOMEM;
        }
    }

    memset(pastDemosaicCtx, 0, sizeof(ISP_DEMOSAIC_S));

    DEMOSAIC_SET_CTX(ViPipe, pastDemosaicCtx);

    return GK_SUCCESS;
}

GK_VOID DemosaicCtxExit(VI_PIPE ViPipe)
{
    ISP_DEMOSAIC_S *pastDemosaicCtx = GK_NULL;

    DEMOSAIC_GET_CTX(ViPipe, pastDemosaicCtx);
    ISP_FREE(pastDemosaicCtx);
    DEMOSAIC_RESET_CTX(ViPipe);
}

static GK_U32 DemosaicGetIsoIndex(GK_U32 u32Iso)
{
    GK_U32 u32Index;

    for (u32Index = 1; u32Index < ISP_AUTO_ISO_STRENGTH_NUM - 1; u32Index++) {
        if (u32Iso <= au32DemosaicIsoLut[u32Index]) {
            break;
        }
    }

    return u32Index;
}

GK_U32 DemosaicGetValueFromLut(GK_U32 u32IsoLevel, GK_S32 s32Y2, GK_S32 s32Y1, GK_S32 s32X2, GK_S32 s32X1, GK_S32 s32Iso)
{
    GK_U32 u32Offset = 0;

    if (s32X1 == s32X2) {
        u32Offset = s32Y2;
    } else if (s32Y1 <= s32Y2) {
        u32Offset = s32Y1 + (ABS(s32Y2 - s32Y1) * ABS(s32Iso - s32X1) + ABS((s32X2 - s32X1) / 2)) / ABS((s32X2 - s32X1));
    } else if (s32Y1 > s32Y2) {
        u32Offset = s32Y1 - (ABS(s32Y2 - s32Y1) * ABS(s32Iso - s32X1) + ABS((s32X2 - s32X1) / 2)) / ABS((s32X2 - s32X1));
    }

    return u32Offset;
}

static GK_VOID  DemosaicInitFwLinear(VI_PIPE ViPipe)
{
    ISP_DEMOSAIC_S *pstDemosaic = GK_NULL;

    GK_U8   au8LutCcHFMaxRatio[ISP_AUTO_ISO_STRENGTH_NUM]        = {3, 4, 5, 6, 6, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8};
    GK_U8   au8LutCcHFMinRatio[ISP_AUTO_ISO_STRENGTH_NUM]        = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    GK_S8   as8LutFcrGFGain[ISP_AUTO_ISO_STRENGTH_NUM]           = {12, 12, 12, 10, 8, 6, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    GK_U8   au8LutAwbGFGainLow[ISP_AUTO_ISO_STRENGTH_NUM]        = {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32 };
    GK_U8   au8LutAwbGFGainHig[ISP_AUTO_ISO_STRENGTH_NUM]        = {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64 };
    GK_U8   au8LutAwbGFGainMax[ISP_AUTO_ISO_STRENGTH_NUM]        = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    GK_U16  au16LutFcrDetLow[ISP_AUTO_ISO_STRENGTH_NUM]          = {150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150};
    GK_U16  au16LutDeSatLow[ISP_AUTO_ISO_STRENGTH_NUM]           = {166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166};
    GK_U16  au16LutDeSatProtTh[ISP_AUTO_ISO_STRENGTH_NUM]        = {128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128};
    GK_U16  au16HfIntpBlurThLow[ISP_AUTO_ISO_STRENGTH_NUM]       = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};
    GK_U16  au16HfIntpBlurThHig[ISP_AUTO_ISO_STRENGTH_NUM]       = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};

    DEMOSAIC_GET_CTX(ViPipe, pstDemosaic);
    ISP_CHECK_POINTER_VOID(pstDemosaic);

    memcpy(pstDemosaic->au8LutCcHFMaxRatio,        au8LutCcHFMaxRatio,        sizeof(GK_U8)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstDemosaic->au8LutCcHFMinRatio,        au8LutCcHFMinRatio,        sizeof(GK_U8)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstDemosaic->as8LutFcrGFGain,           as8LutFcrGFGain,           sizeof(GK_S8)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstDemosaic->au8LutAwbGFGainLow,        au8LutAwbGFGainLow,        sizeof(GK_U8)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstDemosaic->au8LutAwbGFGainHig,        au8LutAwbGFGainHig,        sizeof(GK_U8)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstDemosaic->au8LutAwbGFGainMax,        au8LutAwbGFGainMax,        sizeof(GK_U8)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstDemosaic->au16LutFcrDetLow,          au16LutFcrDetLow,          sizeof(GK_U16)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstDemosaic->au16LutDeSatLow,           au16LutDeSatLow,           sizeof(GK_U16)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstDemosaic->au16LutDeSatProtTh,        au16LutDeSatProtTh,        sizeof(GK_U16)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstDemosaic->au16HfIntpBlurThLow,       au16HfIntpBlurThLow,       sizeof(GK_U16)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstDemosaic->au16HfIntpBlurThHig,       au16HfIntpBlurThHig,       sizeof(GK_U16)*ISP_AUTO_ISO_STRENGTH_NUM);


}

static GK_VOID  DemosaicInitFwWdr(VI_PIPE ViPipe)
{
    ISP_DEMOSAIC_S *pstDemosaic = GK_NULL;

    GK_U8   au8LutCcHFMaxRatio[ISP_AUTO_ISO_STRENGTH_NUM]        = {8, 8, 8, 8, 10, 10, 10, 12, 12, 14, 14, 16, 16, 16, 16, 16};
    GK_U8   au8LutCcHFMinRatio[ISP_AUTO_ISO_STRENGTH_NUM]        = {0, 0, 0, 0, 0, 0, 2, 2, 4, 4, 4, 8, 10, 10, 12, 14};
    GK_S8   as8LutFcrGFGain[ISP_AUTO_ISO_STRENGTH_NUM]           = { -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0};
    GK_U8   au8LutAwbGFGainLow[ISP_AUTO_ISO_STRENGTH_NUM]        = {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32 };
    GK_U8   au8LutAwbGFGainHig[ISP_AUTO_ISO_STRENGTH_NUM]        = {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64 };
    GK_U8   au8LutAwbGFGainMax[ISP_AUTO_ISO_STRENGTH_NUM]        = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 };
    GK_U16  au16LutFcrDetLow[ISP_AUTO_ISO_STRENGTH_NUM]          = {150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150};
    GK_U16  au16LutDeSatLow[ISP_AUTO_ISO_STRENGTH_NUM]           = {166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166};
    GK_U16  au16LutDeSatProtTh[ISP_AUTO_ISO_STRENGTH_NUM]        = {128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128};
    GK_U16  au16HfIntpBlurThLow[ISP_AUTO_ISO_STRENGTH_NUM]       = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};
    GK_U16  au16HfIntpBlurThHig[ISP_AUTO_ISO_STRENGTH_NUM]       = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};

    DEMOSAIC_GET_CTX(ViPipe, pstDemosaic);

    memcpy(pstDemosaic->au8LutCcHFMaxRatio,        au8LutCcHFMaxRatio,        sizeof(GK_U8)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstDemosaic->au8LutCcHFMinRatio,        au8LutCcHFMinRatio,        sizeof(GK_U8)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstDemosaic->as8LutFcrGFGain,           as8LutFcrGFGain,           sizeof(GK_U8)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstDemosaic->au8LutAwbGFGainLow,        au8LutAwbGFGainLow,        sizeof(GK_U8)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstDemosaic->au8LutAwbGFGainHig,        au8LutAwbGFGainHig,        sizeof(GK_U8)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstDemosaic->au8LutAwbGFGainMax,        au8LutAwbGFGainMax,        sizeof(GK_U8)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstDemosaic->au16LutFcrDetLow,          au16LutFcrDetLow,          sizeof(GK_U16)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstDemosaic->au16LutDeSatLow,           au16LutDeSatLow,           sizeof(GK_U16)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstDemosaic->au16LutDeSatProtTh,        au16LutDeSatProtTh,        sizeof(GK_U16)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstDemosaic->au16HfIntpBlurThLow,       au16HfIntpBlurThLow,       sizeof(GK_U16)*ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy(pstDemosaic->au16HfIntpBlurThHig,       au16HfIntpBlurThHig,       sizeof(GK_U16)*ISP_AUTO_ISO_STRENGTH_NUM);


}

static GK_VOID DemosaicStaticRegsInitialize(VI_PIPE ViPipe, ISP_DEMOSAIC_STATIC_CFG_S *pstDemosaicStaticRegCfg, GK_U32 i)
{
    pstDemosaicStaticRegCfg->bDeSatEnable         = ISP_DEMOSAIC_DESAT_ENABLE_DEFAULT;
    pstDemosaicStaticRegCfg->bAHDEnable           = ISP_DEMOSAIC_AND_EN_DEFAULT;
    pstDemosaicStaticRegCfg->bDeFakeEnable        = ISP_DEMOSAIC_DE_FAKE_EN_DEFAULT;
    pstDemosaicStaticRegCfg->u8Lpff3              = ISP_DEMOSAIC_LPF_F3_DEFAULT;
    pstDemosaicStaticRegCfg->u8hvBlendLimit1      = ISP_DEMOSAIC_BLENDLIMIT1_DEFAULT;
    pstDemosaicStaticRegCfg->u8hvBlendLimit2      = ISP_DEMOSAIC_BLENDLIMIT2_DEFAULT;
    pstDemosaicStaticRegCfg->u8hvColorRatio       = ISP_DEMOSAIC_HV_RATIO_DEFAULT;
    pstDemosaicStaticRegCfg->u8hvSelection        = ISP_DEMOSAIC_HV_SEL_DEFAULT;
    pstDemosaicStaticRegCfg->u8GClipBitSft        = ISP_DEMOSAIC_G_CLIP_SFT_BIT_DEFAULT;
    pstDemosaicStaticRegCfg->u8DitherMask         = ISP_DITHER_MASK_DEFAULT;
    pstDemosaicStaticRegCfg->u8DitherRatio        = ISP_DITHER_RATIO_DEFAULT;
    pstDemosaicStaticRegCfg->s16CcVarThresh       = ISP_DEMOSAIC_CC_VAR_THRESH_DEFAULT;
    pstDemosaicStaticRegCfg->u16DeSatThresh1      = ISP_DEMOSAIC_DESAT_THRESH1_DEFAULT;
    pstDemosaicStaticRegCfg->u16DeSatThresh2      = ISP_DEMOSAIC_DESAT_THRESH2_DEFAULT;
    pstDemosaicStaticRegCfg->u16DeSatHig          = ISP_DEMOSAIC_DESAT_HIG_DEFAULT;
    pstDemosaicStaticRegCfg->u16DeSatProtSl       = ISP_DEMOSAIC_DESAT_PROTECT_SL_DEFAULT;
    pstDemosaicStaticRegCfg->u16AhdPart1          = ISP_DEMOSAIC_AHDPART1_DEFAULT;
    pstDemosaicStaticRegCfg->u16AhdPart2          = ISP_DEMOSAIC_AHDPART2_DEFAULT;
    pstDemosaicStaticRegCfg->u16GFThLow           = ISP_NDDM_GF_TH_LOW_DEFAULT;
    pstDemosaicStaticRegCfg->u16GFThHig           = ISP_NDDM_GF_TH_HIGH_DEFAULT;
    pstDemosaicStaticRegCfg->u16CbCrAvgThr        = ISP_DEMOSAIC_CBCR_AVG_THLD_DEFAULT;

    pstDemosaicStaticRegCfg->bResh = GK_TRUE;


}

static GK_VOID DemosaicColorNoiseInit(VI_PIPE ViPipe, ISP_DEMOSAIC_DYNA_CFG_S *pstDmCfg)
{
    GK_U32 u32InitIso;
    GK_U32 u32IsoLevel;
    GK_U32 u32ISO1 = 0;
    GK_U32 u32ISO2 = 0;
    GK_S32 s32Y1, s32Y2;
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    u32InitIso = pstIspCtx->stLinkage.u32Iso;

    if (u32InitIso > au32DemosaicIsoLut[ISP_AUTO_ISO_STRENGTH_NUM - 1]) {
        u32IsoLevel = ISP_AUTO_ISO_STRENGTH_NUM - 1;
        u32ISO1 = au32DemosaicIsoLut[ISP_AUTO_ISO_STRENGTH_NUM - 1];
        u32ISO2 = au32DemosaicIsoLut[ISP_AUTO_ISO_STRENGTH_NUM - 1];
    } else if (u32InitIso <= au32DemosaicIsoLut[0]) {
        u32IsoLevel = 0;
        u32ISO1 = au32DemosaicIsoLut[0];
        u32ISO2 = au32DemosaicIsoLut[0];
    } else {
        u32IsoLevel = DemosaicGetIsoIndex(u32InitIso);
        u32ISO1 = au32DemosaicIsoLut[u32IsoLevel - 1];
        u32ISO2 = au32DemosaicIsoLut[u32IsoLevel];
    }

    s32Y1 = (u32IsoLevel) ? (GK_S32)g_au8InitColorNoiseThdY[u32IsoLevel - 1] : (GK_S32)g_au8InitColorNoiseThdY[0];
    s32Y2 = (GK_S32)g_au8InitColorNoiseThdY[u32IsoLevel];
    pstDmCfg->u8ColorNoiseThdY = (GK_U8)DemosaicGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32InitIso);

    s32Y1 = (u32IsoLevel) ? (GK_S32)g_au8InitColorNoiseStrY[u32IsoLevel - 1] : (GK_S32)g_au8InitColorNoiseStrY[0];
    s32Y2 = (GK_S32)g_au8InitColorNoiseStrY[u32IsoLevel];
    pstDmCfg->u8ColorNoiseStrY = (GK_U8)DemosaicGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32InitIso);
}

static GK_VOID DemosaicDynaRegsInitialize(VI_PIPE ViPipe, ISP_DEMOSAIC_DYNA_CFG_S *pstDemosaicDynaRegCfg)
{
    pstDemosaicDynaRegCfg->u8Lpff0              = ISP_DEMOSAIC_LPF_F0_DEFAULT;
    pstDemosaicDynaRegCfg->u8Lpff1              = ISP_DEMOSAIC_LPF_F1_DEFAULT;
    pstDemosaicDynaRegCfg->u8Lpff2              = ISP_DEMOSAIC_LPF_F2_DEFAULT;
    pstDemosaicDynaRegCfg->u8CcHFMaxRatio       = ISP_DEMOSAIC_CC_HF_MAX_RATIO_DEFAULT;
    pstDemosaicDynaRegCfg->u8CcHFMinRatio       = ISP_DEMOSAIC_CC_HF_MIN_RATIO_DEFAULT;
    pstDemosaicDynaRegCfg->u8DitherMax          = ISP_DITH_MAX_DEFAULT;
    pstDemosaicDynaRegCfg->u8FcrGFGain          = ISP_NDDM_FCR_GF_GAIN_DEFAULT;
    pstDemosaicDynaRegCfg->u8AwbGFGainLow       = ISP_NDDM_AWB_GF_GN_LOW_DEFAULT;
    pstDemosaicDynaRegCfg->u8AwbGFGainHig       = ISP_NDDM_AWB_GF_GN_HIGH_DEFAULT;
    pstDemosaicDynaRegCfg->u8AwbGFGainMax       = ISP_NDDM_AWB_GF_GN_MAX_DEFAULT;
    pstDemosaicDynaRegCfg->u8EhcGray            = ISP_NDDM_EHC_GRAY_DEFAULT;
    pstDemosaicDynaRegCfg->u16DeSatLow          = ISP_DEMOSAIC_DESAT_LOW_DEFAULT;
    pstDemosaicDynaRegCfg->s16DeSatRatio        = ISP_DEMOSAIC_DESAT_RATIO_DEFAULT;
    pstDemosaicDynaRegCfg->u16DeSatProtTh       = ISP_DEMOSAIC_DESAT_PROTECT_TH_DEFAULT;
    pstDemosaicDynaRegCfg->u16HfIntpBlurThLow   = ISP_DEMOSAIC_HF_INPT_BLUR_TH_LOW_DEFAULT;
    pstDemosaicDynaRegCfg->u16HfIntpBlurThHig   = ISP_DEMOSAIC_HF_INPT_BLUR_TH_HIGH_DEFAULT;
    pstDemosaicDynaRegCfg->u16HfIntpBlurRatio   = ISP_DEMOSAIC_HF_INPT_BLUR_RATIO_DEFAULT;
    pstDemosaicDynaRegCfg->u16FcrDetLow         = ISP_NDDM_FCR_DET_LOW_DEFAULT;
    pstDemosaicDynaRegCfg->u16FakeCrVarThrLow   = ISP_DEMOSAIC_FAKECR_VAR_THR_LOW_DEFAULT;
    pstDemosaicDynaRegCfg->u16FakeCrVarThrHigh  = ISP_DEMOSAIC_FAKECR_VAR_THR_HIGH_DEFAULT;
    pstDemosaicDynaRegCfg->u8DeFColorCb         = ISP_DEMOSAIC_COLORNOISE_STR_MAX - ISP_DEMOSAIC_DEFCOLOR_CB_DEFAULT;
    pstDemosaicDynaRegCfg->u8DeFColorCr         = ISP_DEMOSAIC_COLORNOISE_STR_MAX - ISP_DEMOSAIC_DEFCOLOR_CR_DEFAULT;

    DemosaicColorNoiseInit(ViPipe, pstDemosaicDynaRegCfg);

    memset_s(pstDemosaicDynaRegCfg->au16GFBlurLut, ISP_DEMOSAIC_LUT_LENGTH * sizeof(GK_U16), 0, ISP_DEMOSAIC_LUT_LENGTH * sizeof(GK_U16));

    pstDemosaicDynaRegCfg->bUpdateGF            = GK_TRUE;
    pstDemosaicDynaRegCfg->bResh                = GK_TRUE;

    return;
}

static GK_VOID DemosaicRegsInitialize(VI_PIPE ViPipe, ISP_REG_CFG_S  *pstRegCfg)
{
    GK_U32 i;
    ISP_DEMOSAIC_STATIC_CFG_S *pstDemosaicStaticRegCfg = GK_NULL;
    ISP_DEMOSAIC_DYNA_CFG_S   *pstDemosaicDynaRegCfg   = GK_NULL;

    for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
        pstDemosaicStaticRegCfg = &pstRegCfg->stAlgRegCfg[i].stDemRegCfg.stStaticRegCfg;
        pstDemosaicDynaRegCfg   = &pstRegCfg->stAlgRegCfg[i].stDemRegCfg.stDynaRegCfg;
        pstRegCfg->stAlgRegCfg[i].stDemRegCfg.bVhdmEnable = GK_TRUE;
        pstRegCfg->stAlgRegCfg[i].stDemRegCfg.bNddmEnable = GK_TRUE;
        DemosaicStaticRegsInitialize(ViPipe, pstDemosaicStaticRegCfg, i);
        DemosaicDynaRegsInitialize(ViPipe, pstDemosaicDynaRegCfg);
    }

    pstRegCfg->unKey.bit1DemCfg = 1;
}

static GK_S32 DemosaicCheckCmosParam(VI_PIPE ViPipe, const ISP_CMOS_DEMOSAIC_S *pstDemosaic)
{
    GK_U8 i;

    ISP_CHECK_BOOL(pstDemosaic->bEnable);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (pstDemosaic->au8NonDirStr[i] > 0xFF) {
            ISP_TRACE(MODULE_DBG_ERR, "err au8NonDirStr[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstDemosaic->au8NonDirMFDetailEhcStr[i] > 0x7F) {
            ISP_TRACE(MODULE_DBG_ERR, "err au8NonDirMFDetailEhcStr[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstDemosaic->au8NonDirHFDetailEhcStr[i] > 0x10) {
            ISP_TRACE(MODULE_DBG_ERR, "err au8NonDirHFDetailEhcStr[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if ((pstDemosaic->au8DetailSmoothRange[i] > ISP_DEMOSAIC_DETAIL_SMOOTH_RANGE_MAX) || (pstDemosaic->au8DetailSmoothRange[i] < ISP_DEMOSAIC_DETAIL_SMOOTH_RANGE_MIN)) {
            ISP_TRACE(MODULE_DBG_ERR, "err au8DetailSmoothRange[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
    }

    return GK_SUCCESS;
}

static GK_S32 DemosaicExtRegsInitialize(VI_PIPE ViPipe)
{
    GK_U8  i;
    GK_S32 s32Ret;
    GK_FLOAT n_cur, n_pre, n_fct;
    GK_FLOAT drc_compensate = 0.6f;
    GK_FLOAT afsigma[ISP_DEMOSAIC_LUT_LENGTH];
    ISP_DEMOSAIC_S     *pstDemosaic = GK_NULL;
    ISP_CMOS_DEFAULT_S *pstSnsDft   = GK_NULL;
    ISP_CTX_S          *pstIspCtx   = GK_NULL;
#ifdef VREG_OLD
    GK_U32 u32Addr;
    GK_U8 *pVirtAddr = GK_NULL;
    GK_U8 *pVirtAddrTmp = GK_NULL;
#endif

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_SensorGetDefault(ViPipe, &pstSnsDft);
    DEMOSAIC_GET_CTX(ViPipe, pstDemosaic);

    pstDemosaic->u8WdrMode = pstIspCtx->u8SnsWDRMode;
    pstDemosaic->u8BitDepthPrc = ISP_DEMOSAIC_BITDEPTH;

    if (pstDemosaic->u8WdrMode != 0) {
        DemosaicInitFwWdr(ViPipe);

        for (i = 0; i < ISP_DEMOSAIC_LUT_LENGTH; i++) {
            n_cur = (GK_FLOAT)(i * 16);
            n_pre = (GK_FLOAT)(256.0 * pow(n_cur / 256.0, 1.0f / DIV_0_TO_1_FLOAT(drc_compensate))) + 0.5f;
            n_fct = (GK_FLOAT)(ISP_SQR(n_cur / DIV_0_TO_1_FLOAT(n_pre)));

            afsigma[i]  =  n_cur * n_fct;
            pstDemosaic->au32sigma[i] = (GK_U32)(afsigma[i] * ISP_BITFIX(10));
        }
    } else {
        DemosaicInitFwLinear(ViPipe);

        for (i = 0; i < ISP_DEMOSAIC_LUT_LENGTH; i++) {
            pstDemosaic->au32sigma[i]  = (GK_U32)(i * 16 * ISP_BITFIX(10));
        }
    }

    ext_system_demosaic_enable_write(ViPipe, GK_TRUE);
    ext_system_demosaic_manual_mode_write(ViPipe, ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_MODE_DEFAULT);

#ifndef VREG_OLD
    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) { // Auto
        pstDemosaic->stAuto.au8NonDirStr[i]            = g_au8NonDirStr[i];
        pstDemosaic->stAuto.au8NonDirMFDetailEhcStr[i] = g_au8NonDirMFDetailEhcStr[i];
        pstDemosaic->stAuto.au8NonDirHFDetailEhcStr[i] = g_au8NonDirHFDetailEhcStr[i];
        pstDemosaic->stAuto.au8DetailSmoothRange[i]    = g_au8DetailSmoothRange[i];
        pstDemosaic->stAuto.au8ColorNoiseThdF[i]       = g_au8ColorNoiseThdF[i];
        pstDemosaic->stAuto.au8ColorNoiseStrF[i]       = g_au8ColorNoiseStrF[i];
        pstDemosaic->stAuto.au8ColorNoiseThdY[i]       = g_au8ColorNoiseThdY[i];
        pstDemosaic->stAuto.au8ColorNoiseStrY[i]       = g_au8ColorNoiseStrY[i];
    }
#else
    memcpy_s(pstDemosaic->stAuto.au8NonDirStr, ISP_AUTO_ISO_STRENGTH_NUM, g_au8NonDirStr, ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(pstDemosaic->stAuto.au8NonDirMFDetailEhcStr, ISP_AUTO_ISO_STRENGTH_NUM, g_au8NonDirMFDetailEhcStr, ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(pstDemosaic->stAuto.au8NonDirHFDetailEhcStr, ISP_AUTO_ISO_STRENGTH_NUM, g_au8NonDirHFDetailEhcStr, ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(pstDemosaic->stAuto.au8DetailSmoothRange, ISP_AUTO_ISO_STRENGTH_NUM, g_au8DetailSmoothRange, ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(pstDemosaic->stAuto.au8ColorNoiseThdY, ISP_AUTO_ISO_STRENGTH_NUM, g_au8ColorNoiseThdY, ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(pstDemosaic->stAuto.au8ColorNoiseStrY, ISP_AUTO_ISO_STRENGTH_NUM, g_au8ColorNoiseStrY, ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(pstDemosaic->stAuto.au8ColorNoiseThdF, ISP_AUTO_ISO_STRENGTH_NUM, g_au8ColorNoiseThdF, ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(pstDemosaic->stAuto.au8ColorNoiseStrF, ISP_AUTO_ISO_STRENGTH_NUM, g_au8ColorNoiseStrF, ISP_AUTO_ISO_STRENGTH_NUM);

#endif
    if (IS_WDR_MODE(pstIspCtx->u8SnsWDRMode)) {    // Manual:WDR mode
        pstDemosaic->stManual.u8NonDirStr            = ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_NONDIRECTION_STRENTH_WDR_DEFAULT;
        pstDemosaic->stManual.u8NonDirMFDetailEhcStr = ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_NONDIRECTION_MIDFREQ_DETAILENHANCE_STRENGTH_WDR_DEFAULT;
        pstDemosaic->stManual.u8NonDirHFDetailEhcStr = ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_NONDIRECTION_HIGFREQ_DETAILENHANCE_STRENGTH_WDR_DEFAULT;
        pstDemosaic->stManual.u8DetailSmoothRange    = ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_DETAIL_SMOOTH_RANGE_WDR_DEFAULT;
    } else {   // Manual:Linear Mode
        pstDemosaic->stManual.u8NonDirStr            = ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_NONDIRECTION_STRENTH_LINEAR_DEFAULT;
        pstDemosaic->stManual.u8NonDirMFDetailEhcStr = ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_NONDIRECTION_MIDFREQ_DETAILENHANCE_STRENGTH_LINEAR_DEFAULT;
        pstDemosaic->stManual.u8NonDirHFDetailEhcStr = ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_NONDIRECTION_HIGFREQ_DETAILENHANCE_STRENGTH_LINEAR_DEFAULT;
        pstDemosaic->stManual.u8DetailSmoothRange    = ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_DETAIL_SMOOTH_RANGE_LINEAR_DEFAULT;
    }
    pstDemosaic->stManual.u8ColorNoiseThdF = ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_COLORNOISE_THDF_DEFAULT;
    pstDemosaic->stManual.u8ColorNoiseStrF = ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_COLORNOISE_STRF_DEFAULT;
    pstDemosaic->stManual.u8ColorNoiseThdY = ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_DESAT_DARK_RANGE_DEFAULT;
    pstDemosaic->stManual.u8ColorNoiseStrY = ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_DESAT_DARK_STRENGTH_DEFAULT;
    ext_system_demosaic_manual_nondirection_midfreq_detailenhance_strength_write(ViPipe, pstDemosaic->stManual.u8NonDirMFDetailEhcStr);
    ext_system_demosaic_manual_nondirectiongfreq_detailenhance_strength_write(ViPipe, pstDemosaic->stManual.u8NonDirHFDetailEhcStr);
    ext_system_demosaic_manual_nondirection_strength_write(ViPipe, pstDemosaic->stManual.u8NonDirStr);
    ext_system_demosaic_manual_detail_smooth_range_write(ViPipe, pstDemosaic->stManual.u8DetailSmoothRange);
    ext_system_demosaic_manual_colornoise_thdf_write(ViPipe, pstDemosaic->stManual.u8ColorNoiseThdF);
    ext_system_demosaic_manual_colornoise_strf_write(ViPipe, pstDemosaic->stManual.u8ColorNoiseStrF);
    ext_system_demosaic_manual_desat_dark_range_write(ViPipe, pstDemosaic->stManual.u8ColorNoiseThdY);
    ext_system_demosaic_manual_desat_dark_strength_write(ViPipe, pstDemosaic->stManual.u8ColorNoiseStrY);
    if (pstSnsDft->unKey.bit1Demosaic) {
        ISP_CHECK_POINTER(pstSnsDft->pstDemosaic);

        s32Ret = DemosaicCheckCmosParam(ViPipe, pstSnsDft->pstDemosaic);

        if (s32Ret != GK_SUCCESS) {
            return s32Ret;
        }

        ext_system_demosaic_enable_write(ViPipe, pstSnsDft->pstDemosaic->bEnable);
#ifndef VREG_OLD
        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) { // Auto
            pstDemosaic->stAuto.au8NonDirStr[i]            = pstSnsDft->pstDemosaic->au8NonDirStr[i];
            pstDemosaic->stAuto.au8NonDirMFDetailEhcStr[i] = pstSnsDft->pstDemosaic->au8NonDirMFDetailEhcStr[i];
            pstDemosaic->stAuto.au8NonDirHFDetailEhcStr[i] = pstSnsDft->pstDemosaic->au8NonDirHFDetailEhcStr[i];
            pstDemosaic->stAuto.au8DetailSmoothRange[i]    = pstSnsDft->pstDemosaic->au8DetailSmoothRange[i];
        }
#else
        memcpy_s(pstDemosaic->stAuto.au8NonDirStr, ISP_AUTO_ISO_STRENGTH_NUM, pstSnsDft->pstDemosaic->au8NonDirStr, ISP_AUTO_ISO_STRENGTH_NUM);
        memcpy_s(pstDemosaic->stAuto.au8NonDirMFDetailEhcStr, ISP_AUTO_ISO_STRENGTH_NUM, pstSnsDft->pstDemosaic->au8NonDirMFDetailEhcStr, ISP_AUTO_ISO_STRENGTH_NUM);
        memcpy_s(pstDemosaic->stAuto.au8NonDirHFDetailEhcStr, ISP_AUTO_ISO_STRENGTH_NUM, pstSnsDft->pstDemosaic->au8NonDirHFDetailEhcStr, ISP_AUTO_ISO_STRENGTH_NUM);
        memcpy_s(pstDemosaic->stAuto.au8DetailSmoothRange, ISP_AUTO_ISO_STRENGTH_NUM, pstSnsDft->pstDemosaic->au8DetailSmoothRange, ISP_AUTO_ISO_STRENGTH_NUM);

#endif
    }

#ifndef VREG_OLD
    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) { // Auto
        ext_system_demosaic_auto_nondirection_strength_write(ViPipe, i, pstDemosaic->stAuto.au8NonDirStr[i]);
        ext_system_demosaic_auto_nondirection_midfreq_detailenhance_strength_write(ViPipe, i, pstDemosaic->stAuto.au8NonDirMFDetailEhcStr[i]);
        ext_system_demosaic_auto_nondirectiongfreq_detailenhance_strength_write(ViPipe, i, pstDemosaic->stAuto.au8NonDirHFDetailEhcStr[i]);
        ext_system_demosaic_auto_detail_smooth_range_write(ViPipe, i, pstDemosaic->stAuto.au8DetailSmoothRange[i]);
        ext_system_demosaic_auto_desat_dark_range_write(ViPipe, i, pstDemosaic->stAuto.au8ColorNoiseThdY[i]);
        ext_system_demosaic_auto_desat_dark_strength_write(ViPipe, i, pstDemosaic->stAuto.au8ColorNoiseStrY[i]);
        ext_system_demosaic_auto_colornoise_thdf_write(ViPipe, i, pstDemosaic->stAuto.au8ColorNoiseThdF[i]);
        ext_system_demosaic_auto_colornoise_strf_write(ViPipe, i, pstDemosaic->stAuto.au8ColorNoiseStrF[i]);
    }
#else
    u32Addr  = VREG_DM_BASE(ViPipe);
    pVirtAddrTmp = (GK_U8 *)VReg_GetVirtAddr(u32Addr);

    pVirtAddr    = pVirtAddrTmp + 0x40;
    memcpy_s((GK_U8 *)pVirtAddr, ISP_AUTO_ISO_STRENGTH_NUM, pstDemosaic->stAuto.au8NonDirStr, ISP_AUTO_ISO_STRENGTH_NUM);

    pVirtAddr    = pVirtAddrTmp + 0x20;
    memcpy_s((GK_U8 *)pVirtAddr, ISP_AUTO_ISO_STRENGTH_NUM, pstDemosaic->stAuto.au8NonDirMFDetailEhcStr, ISP_AUTO_ISO_STRENGTH_NUM);

    pVirtAddr    = pVirtAddrTmp + 0x30;
    memcpy_s((GK_U8 *)pVirtAddr, ISP_AUTO_ISO_STRENGTH_NUM, pstDemosaic->stAuto.au8NonDirHFDetailEhcStr, ISP_AUTO_ISO_STRENGTH_NUM);

    pVirtAddr    = pVirtAddrTmp + 0x50;
    memcpy_s((GK_U8 *)pVirtAddr, ISP_AUTO_ISO_STRENGTH_NUM, pstDemosaic->stAuto.au8DetailSmoothRange, ISP_AUTO_ISO_STRENGTH_NUM);
    pVirtAddr    = pVirtAddrTmp + 0xa0;
    memcpy_s((GK_U8 *)pVirtAddr, ISP_AUTO_ISO_STRENGTH_NUM, pstDemosaic->stAuto.au8ColorNoiseThdY, ISP_AUTO_ISO_STRENGTH_NUM);
    pVirtAddr    = pVirtAddrTmp + 0xb0;
    memcpy_s((GK_U8 *)pVirtAddr, ISP_AUTO_ISO_STRENGTH_NUM, pstDemosaic->stAuto.au8ColorNoiseStrY, ISP_AUTO_ISO_STRENGTH_NUM);
    pVirtAddr    = pVirtAddrTmp + 0x80;
    memcpy_s((GK_U8 *)pVirtAddr, ISP_AUTO_ISO_STRENGTH_NUM, pstDemosaic->stAuto.au8ColorNoiseThdF, ISP_AUTO_ISO_STRENGTH_NUM);
    pVirtAddr    = pVirtAddrTmp + 0x90;
    memcpy_s((GK_U8 *)pVirtAddr, ISP_AUTO_ISO_STRENGTH_NUM, pstDemosaic->stAuto.au8ColorNoiseStrF, ISP_AUTO_ISO_STRENGTH_NUM);
#endif


    return GK_SUCCESS;
}

static GK_S32 Demosaic_SetLongFrameMode(VI_PIPE ViPipe)
{
    GK_U8  i;
    GK_FLOAT n_cur, n_pre, n_fct;
    GK_FLOAT drc_compensate = 0.6f;
    GK_FLOAT afsigma[ISP_DEMOSAIC_LUT_LENGTH];
    ISP_DEMOSAIC_S     *pstDemosaic = GK_NULL;
    ISP_CTX_S          *pstIspCtx   = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    DEMOSAIC_GET_CTX(ViPipe, pstDemosaic);

    if (IS_LINEAR_MODE(pstIspCtx->u8SnsWDRMode) ||
        (pstIspCtx->stLinkage.enFSWDRMode == ISP_FSWDR_LONG_FRAME_MODE) ||
        (pstIspCtx->stLinkage.enFSWDRMode == ISP_FSWDR_AUTO_LONG_FRAME_MODE)) {
        DemosaicInitFwLinear(ViPipe);

        for (i = 0; i < ISP_DEMOSAIC_LUT_LENGTH; i++) {
            pstDemosaic->au32sigma[i]  = (GK_U32)(i * 16 * ISP_BITFIX(10));
        }

    } else {
        DemosaicInitFwWdr(ViPipe);

        for (i = 0; i < ISP_DEMOSAIC_LUT_LENGTH; i++) {
            n_cur = (GK_FLOAT)(i * 16);
            n_pre = (GK_FLOAT)(256.0 * pow(n_cur / 256.0, 1.0f / DIV_0_TO_1_FLOAT(drc_compensate))) + 0.5f;
            n_fct = (GK_FLOAT)(ISP_SQR(n_cur / DIV_0_TO_1_FLOAT(n_pre)));
            afsigma[i]  =  n_cur * n_fct;
            pstDemosaic->au32sigma[i] = (GK_U32)(afsigma[i] * ISP_BITFIX(10));
        }
    }

    return GK_SUCCESS;

}


static GK_S32 DemosaicReadExtregs(VI_PIPE ViPipe)
{
    ISP_DEMOSAIC_S *pstDemosaic = GK_NULL;
    GK_U8 i;

    DEMOSAIC_GET_CTX(ViPipe, pstDemosaic);

    pstDemosaic->bDemAttrUpdate = ext_system_demosaic_attr_update_en_read(ViPipe);

    ext_system_demosaic_attr_update_en_write(ViPipe, GK_FALSE);

    if (pstDemosaic->bDemAttrUpdate) {
        pstDemosaic->enOpType        = ext_system_demosaic_manual_mode_read(ViPipe);

        if (pstDemosaic->enOpType == OP_TYPE_AUTO) {
            for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
                pstDemosaic->stAuto.au8NonDirStr[i]            = ext_system_demosaic_auto_nondirection_strength_read(ViPipe, i);
                pstDemosaic->stAuto.au8NonDirMFDetailEhcStr[i] = ext_system_demosaic_auto_nondirection_midfreq_detailenhance_strength_read(ViPipe, i);
                pstDemosaic->stAuto.au8NonDirHFDetailEhcStr[i] = ext_system_demosaic_auto_nondirectiongfreq_detailenhance_strength_read(ViPipe, i);
                pstDemosaic->stAuto.au8DetailSmoothRange[i]    = ext_system_demosaic_auto_detail_smooth_range_read(ViPipe, i);
                pstDemosaic->stAuto.au8ColorNoiseThdY[i]       = ext_system_demosaic_auto_desat_dark_range_read(ViPipe, i);
                pstDemosaic->stAuto.au8ColorNoiseStrY[i]       = ext_system_demosaic_auto_desat_dark_strength_read(ViPipe, i);
                pstDemosaic->stAuto.au8ColorNoiseThdF[i]       = ext_system_demosaic_auto_colornoise_thdf_read(ViPipe, i);
                pstDemosaic->stAuto.au8ColorNoiseStrF[i]       = ext_system_demosaic_auto_colornoise_strf_read(ViPipe, i);
            }
        } else if (pstDemosaic->enOpType == OP_TYPE_MANUAL) {
            pstDemosaic->stManual.u8NonDirStr            = ext_system_demosaic_manual_nondirection_strength_read(ViPipe);
            pstDemosaic->stManual.u8NonDirMFDetailEhcStr = ext_system_demosaic_manual_nondirection_midfreq_detailenhance_strength_read(ViPipe);
            pstDemosaic->stManual.u8NonDirHFDetailEhcStr = ext_system_demosaic_manual_nondirectiongfreq_detailenhance_strength_read(ViPipe);
            pstDemosaic->stManual.u8DetailSmoothRange    = ext_system_demosaic_manual_detail_smooth_range_read(ViPipe);
            pstDemosaic->stManual.u8ColorNoiseThdY       = ext_system_demosaic_manual_desat_dark_range_read(ViPipe);
            pstDemosaic->stManual.u8ColorNoiseStrY       = ext_system_demosaic_manual_desat_dark_strength_read(ViPipe);
            pstDemosaic->stManual.u8ColorNoiseThdF       = ext_system_demosaic_manual_colornoise_thdf_read(ViPipe);
            pstDemosaic->stManual.u8ColorNoiseStrF       = ext_system_demosaic_manual_colornoise_strf_read(ViPipe);
        }
    }
    return GK_SUCCESS;
}

GK_S32 DemosaicProcWrite(VI_PIPE ViPipe, ISP_CTRL_PROC_WRITE_S *pstProc)
{
    ISP_CTRL_PROC_WRITE_S stProcTmp;
    ISP_DEMOSAIC_S *pstDemosaic = GK_NULL;

    GK_U8   u8NonDirStr;
    GK_U8   u8NonDirMFDetailEhcStr;
    GK_U8   u8NonDirHFDetailEhcStr;
    GK_U8   u8DetailSmoothRange;


    DEMOSAIC_GET_CTX(ViPipe, pstDemosaic);
    ISP_CHECK_POINTER(pstDemosaic);

    if ((pstProc->pcProcBuff == GK_NULL)
        || (pstProc->u32BuffLen == 0)) {
        return GK_FAILURE;
    }

    stProcTmp.pcProcBuff = pstProc->pcProcBuff;
    stProcTmp.u32BuffLen = pstProc->u32BuffLen;

    u8NonDirStr            = pstDemosaic->u8ProcNonDirStr;
    u8NonDirMFDetailEhcStr = pstDemosaic->u8ProcNonDirMFDetailEhcStr;
    u8NonDirHFDetailEhcStr = pstDemosaic->u8ProcNonDirHFDetailEhcStr;
    u8DetailSmoothRange    = pstDemosaic->u8ProcDetailSmoothRange;

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
                    "-----DEMOSAIC INFO-------------------------------------------------------------\n");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
                    "%12s" "%10s" "%12s" "%12s" "%10s\n",
                    "Enable", "NoDirStr", "NoDirMFStr", "NoDirHFStr", "DeSmthRng");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
                    "%12u" "%10u" "%12u" "%12u" "%10u\n",
                    pstDemosaic->bEnable, u8NonDirStr, u8NonDirMFDetailEhcStr, u8NonDirHFDetailEhcStr, u8DetailSmoothRange);

    pstProc->u32WriteLen += 1;

    return GK_SUCCESS;
}


GK_S32 ISP_DemosaicInit(VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
    GK_S32 s32Ret = GK_SUCCESS;
    ISP_DEMOSAIC_S *pstDemosaic = GK_NULL;
    ISP_REG_CFG_S  *pstRegCfg   = (ISP_REG_CFG_S *)pRegCfg;


    s32Ret = DemosaicCtxInit(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        return s32Ret;
    }

    DEMOSAIC_GET_CTX(ViPipe, pstDemosaic);
    ISP_CHECK_POINTER(pstDemosaic);

    DemosaicRegsInitialize(ViPipe, pstRegCfg);
    s32Ret = DemosaicExtRegsInitialize(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        return s32Ret;
    }

    pstDemosaic->bInit = GK_TRUE;
    return GK_SUCCESS;
}

#define  EPS (0.000001f)
#define  COL_ISO      0
#define  COL_K        1
#define  COL_B        2
static GK_FLOAT Demosaic_getKfromNoiseLut(GK_FLOAT (*pRecord)[3], GK_U16 recordNum, GK_S32 iso)
{
    GK_S32 i = 0;
    GK_FLOAT  y_diff = 0, x_diff = 0;
    GK_FLOAT k = 0.0f;

    if (recordNum > BAYER_CALIBTAION_MAX_NUM) {
        k = pRecord[BAYER_CALIBTAION_MAX_NUM - 1][COL_K];
        return k;
    }

    // record: iso - k
    if (iso <= pRecord[0][COL_ISO]) {
        k = pRecord[0][COL_K];
    }

    if (iso >= pRecord[recordNum - 1][COL_ISO]) {
        k = pRecord[recordNum - 1][COL_K];
    }

    for (i = 0; i < recordNum - 1; i++) {
        if (iso >= pRecord[i][COL_ISO] && iso <= pRecord[i + 1][COL_ISO]) {
            x_diff = pRecord[i + 1][COL_ISO] - pRecord[i][COL_ISO];  // iso diff
            y_diff = pRecord[i + 1][COL_K]  - pRecord[i][COL_K];     // k diff
            k = pRecord[i][COL_K] + y_diff * (iso - pRecord[i][COL_ISO]) / DIV_0_TO_1_FLOAT(x_diff + EPS);
        }
    }

    return k;
}

GK_S32 Demosaic_GFBlurLut(ISP_DEMOSAIC_S *pstDemosaic, VI_PIPE ViPipe, ISP_DEMOSAIC_DYNA_CFG_S *pstDmCfg, GK_U8 u8NonDirStr, GK_U32 u32Iso)
{
    GK_U8  n = 0;
    GK_U32 alpha, sigma;
    GK_U64 u64sigma;
    ISP_CMOS_DEFAULT_S *pstSnsDft = GK_NULL;
    ISP_SensorGetDefault(ViPipe, &pstSnsDft);
    GK_U32   bitScale;
    GK_FLOAT fCalibrationCoef = 0.0f;

    bitScale = ISP_BITFIX(ABS(pstDemosaic->u8BitDepthPrc - 8));

    fCalibrationCoef = Demosaic_getKfromNoiseLut(pstSnsDft->stNoiseCalibration.afCalibrationCoef, pstSnsDft->stNoiseCalibration.u16CalibrationLutNum, u32Iso);
    alpha = (GK_U32)(fCalibrationCoef * ISP_BITFIX(10));
    for (n = 0; n < ISP_DEMOSAIC_LUT_LENGTH; n++) {
        u64sigma  = (GK_U64)pstDemosaic->au32sigma[n] * alpha;
        u64sigma *= u8NonDirStr;
        sigma  = MIN2((u64sigma >> 14), 0xffffffff);
        sigma  = (GK_U32)Sqrt32(sigma);
        sigma  = (sigma * bitScale) >> 5;
        pstDmCfg->au16GFBlurLut[n] = MIN2(sigma, ISP_BITMASK(pstDemosaic->u8BitDepthPrc));
    }

    pstDmCfg->au16GFBlurLut[0] = pstDmCfg->au16GFBlurLut[1] / 2;
    pstDmCfg->bUpdateGF       = GK_TRUE;

    return GK_SUCCESS;
}

GK_S32 DemosaicCfg(ISP_DEMOSAIC_DYNA_CFG_S *pstDmCfg, ISP_DEMOSAIC_STATIC_CFG_S *pstStaticDmCfg, ISP_DEMOSAIC_S *pstDemosaic, GK_U32 u32IsoLevel, GK_U32 u32ISO2, GK_U32 u32ISO1, GK_U32 u32Iso)
{
    GK_S32 s32Y1, s32Y2;

    s32Y1 = (u32IsoLevel) ? (GK_S32)pstDemosaic->au8LutCcHFMaxRatio[u32IsoLevel - 1] : (GK_S32)pstDemosaic->au8LutCcHFMaxRatio[0];
    s32Y2 = (GK_S32)pstDemosaic->au8LutCcHFMaxRatio[u32IsoLevel];
    pstDmCfg->u8CcHFMaxRatio   = (GK_U8)DemosaicGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

    s32Y1 = (u32IsoLevel) ? (GK_S32)pstDemosaic->au8LutCcHFMinRatio[u32IsoLevel - 1] : (GK_S32)pstDemosaic->au8LutCcHFMinRatio[0];
    s32Y2 = (GK_S32)pstDemosaic->au8LutCcHFMinRatio[u32IsoLevel];
    pstDmCfg->u8CcHFMinRatio   = (GK_U16)DemosaicGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

    s32Y1 = (u32IsoLevel) ? (GK_S32)pstDemosaic->as8LutFcrGFGain[u32IsoLevel - 1] : (GK_S32)pstDemosaic->as8LutFcrGFGain[0];
    s32Y2 = (GK_S32)pstDemosaic->as8LutFcrGFGain[u32IsoLevel];
    pstDmCfg->u8FcrGFGain = (GK_U8)DemosaicGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

    s32Y1 = (u32IsoLevel) ? (GK_S32)pstDemosaic->au8LutAwbGFGainLow[u32IsoLevel - 1] : (GK_S32)pstDemosaic->au8LutAwbGFGainLow[0];
    s32Y2 = (GK_S32)pstDemosaic->au8LutAwbGFGainLow[u32IsoLevel];
    pstDmCfg->u8AwbGFGainLow = (GK_U8)DemosaicGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

    s32Y1 = (u32IsoLevel) ? (GK_S32)pstDemosaic->au8LutAwbGFGainHig[u32IsoLevel - 1] : (GK_S32)pstDemosaic->au8LutAwbGFGainHig[0];
    s32Y2 = (GK_S32)pstDemosaic->au8LutAwbGFGainHig[u32IsoLevel];
    pstDmCfg->u8AwbGFGainHig = (GK_U8)DemosaicGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

    s32Y1 = (u32IsoLevel) ? (GK_S32)pstDemosaic->au8LutAwbGFGainMax[u32IsoLevel - 1] : (GK_S32)pstDemosaic->au8LutAwbGFGainMax[0];
    s32Y2 = (GK_S32)pstDemosaic->au8LutAwbGFGainMax[u32IsoLevel];
    pstDmCfg->u8AwbGFGainMax = (GK_U8)DemosaicGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

    s32Y1 = (u32IsoLevel) ? (GK_S32)pstDemosaic->au16LutFcrDetLow[u32IsoLevel - 1] : (GK_S32)pstDemosaic->au16LutFcrDetLow[0];
    s32Y2 = (GK_S32)pstDemosaic->au16LutFcrDetLow[u32IsoLevel];
    pstDmCfg->u16FcrDetLow     = (GK_U16)DemosaicGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

    s32Y1 = (u32IsoLevel) ? (GK_S32)pstDemosaic->au16LutDeSatLow[u32IsoLevel - 1] : (GK_S32)pstDemosaic->au16LutDeSatLow[0];
    s32Y2 = (GK_S32)pstDemosaic->au16LutDeSatLow[u32IsoLevel];
    pstDmCfg->u16DeSatLow = (GK_U16)DemosaicGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

    s32Y1 = (u32IsoLevel) ? (GK_S32)pstDemosaic->au16LutDeSatProtTh[u32IsoLevel - 1] : (GK_S32)pstDemosaic->au16LutDeSatProtTh[0];
    s32Y2 = (GK_S32)pstDemosaic->au16LutDeSatProtTh[u32IsoLevel];
    pstDmCfg->u16DeSatProtTh = (GK_U16)DemosaicGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

    if (pstStaticDmCfg->u16DeSatThresh1 == pstStaticDmCfg->u16DeSatThresh2) {
        pstDmCfg->s16DeSatRatio = 0;
    } else {
        pstDmCfg->s16DeSatRatio = 16 * ((GK_S16)pstDmCfg->u16DeSatLow - (GK_S16)pstStaticDmCfg->u16DeSatHig) / (GK_S16)DIV_0_TO_1(pstStaticDmCfg->u16DeSatThresh2 - pstStaticDmCfg->u16DeSatThresh1);
    }

    s32Y1 = (u32IsoLevel) ? (GK_S32)pstDemosaic->au16HfIntpBlurThLow[u32IsoLevel - 1] : (GK_S32)pstDemosaic->au16HfIntpBlurThLow[0];
    s32Y2 = (GK_S32)pstDemosaic->au16HfIntpBlurThLow[u32IsoLevel];
    pstDmCfg->u16HfIntpBlurThLow = (GK_U16)DemosaicGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

    s32Y1 = (u32IsoLevel) ? (GK_S32)pstDemosaic->au16HfIntpBlurThHig[u32IsoLevel - 1] : (GK_S32)pstDemosaic->au16HfIntpBlurThHig[0];
    s32Y2 = (GK_S32)pstDemosaic->au16HfIntpBlurThHig[u32IsoLevel];
    pstDmCfg->u16HfIntpBlurThHig   = (GK_U16)DemosaicGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

    if (pstDmCfg->u16HfIntpBlurThHig == pstDmCfg->u16HfIntpBlurThLow) {
        pstDmCfg->u16HfIntpBlurRatio = 0;
    } else {
        pstDmCfg->u16HfIntpBlurRatio  = 256 * 16 / DIV_0_TO_1(pstDmCfg->u16HfIntpBlurThHig - pstDmCfg->u16HfIntpBlurThLow);
    }

    return GK_SUCCESS;
}

GK_S32 ISP_DemosaicFw(GK_U32 u32Iso, VI_PIPE ViPipe, GK_U8 u8CurBlk, ISP_REG_CFG_S *pstReg)
{
    ISP_DEMOSAIC_DYNA_CFG_S *pstDmCfg = &pstReg->stAlgRegCfg[u8CurBlk].stDemRegCfg.stDynaRegCfg;
    ISP_DEMOSAIC_STATIC_CFG_S *pstStaticDmCfg = &pstReg->stAlgRegCfg[u8CurBlk].stDemRegCfg.stStaticRegCfg;
    ISP_DEMOSAIC_S *pstDemosaic = GK_NULL;
    ISP_CTX_S *pstIspCtx = GK_NULL;

    DEMOSAIC_GET_CTX(ViPipe, pstDemosaic);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    GK_U8  u8FilterCoefIndex = 0;
    GK_U8  u8NonDirStr = 0;
    GK_U32 u32IsoLevel;
    GK_U32 u32ISO1 = 0;
    GK_U32 u32ISO2 = 0;
    GK_S32 s32Y1, s32Y2;

    if (u32Iso > au32DemosaicIsoLut[ISP_AUTO_ISO_STRENGTH_NUM - 1]) {
        u32IsoLevel = ISP_AUTO_ISO_STRENGTH_NUM - 1;
        u32ISO1 = au32DemosaicIsoLut[ISP_AUTO_ISO_STRENGTH_NUM - 1];
        u32ISO2 = au32DemosaicIsoLut[ISP_AUTO_ISO_STRENGTH_NUM - 1];
    } else if (u32Iso <= au32DemosaicIsoLut[0]) {
        u32IsoLevel = 0;
        u32ISO1 = 0;
        u32ISO2 = au32DemosaicIsoLut[0];
    } else {
        u32IsoLevel = DemosaicGetIsoIndex(u32Iso);
        u32ISO1 = au32DemosaicIsoLut[u32IsoLevel - 1];
        u32ISO2 = au32DemosaicIsoLut[u32IsoLevel];
    }

    if (pstDemosaic->enOpType == OP_TYPE_AUTO) {
        s32Y1 = (u32IsoLevel) ? (GK_S32)pstDemosaic->stAuto.au8NonDirStr[u32IsoLevel - 1] : (GK_S32)pstDemosaic->stAuto.au8NonDirStr[0];
        s32Y2 = (GK_S32)pstDemosaic->stAuto.au8NonDirStr[u32IsoLevel];
        u8NonDirStr = (GK_U8)DemosaicGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

        s32Y1 = (u32IsoLevel) ? (GK_S32)pstDemosaic->stAuto.au8NonDirMFDetailEhcStr[u32IsoLevel - 1] : (GK_S32)pstDemosaic->stAuto.au8NonDirMFDetailEhcStr[0];
        s32Y2 = (GK_S32)pstDemosaic->stAuto.au8NonDirMFDetailEhcStr[u32IsoLevel];
        pstDmCfg->u8EhcGray = (GK_U8)DemosaicGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

        s32Y1 = (u32IsoLevel) ? (GK_S32)pstDemosaic->stAuto.au8NonDirHFDetailEhcStr[u32IsoLevel - 1] : (GK_S32)pstDemosaic->stAuto.au8NonDirHFDetailEhcStr[0];
        s32Y2 = (GK_S32)pstDemosaic->stAuto.au8NonDirHFDetailEhcStr[u32IsoLevel];
        pstDmCfg->u8DitherMax = (GK_U8)DemosaicGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

        s32Y1 = (u32IsoLevel) ? (GK_S32)pstDemosaic->stAuto.au8DetailSmoothRange[u32IsoLevel - 1] : (GK_S32)pstDemosaic->stAuto.au8DetailSmoothRange[0];
        s32Y2 = (GK_S32)pstDemosaic->stAuto.au8DetailSmoothRange[u32IsoLevel];
        u8FilterCoefIndex = (GK_U8)DemosaicGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

        if (pstIspCtx->u32FrameCnt > 1) {
            s32Y1 = (u32IsoLevel) ? (GK_S32)pstDemosaic->stAuto.au8ColorNoiseThdY[u32IsoLevel - 1] : (GK_S32)pstDemosaic->stAuto.au8ColorNoiseThdY[0];
            s32Y2 = (GK_S32)pstDemosaic->stAuto.au8ColorNoiseThdY[u32IsoLevel];
            pstDmCfg->u8ColorNoiseThdY = (GK_U8)DemosaicGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

            s32Y1 = (u32IsoLevel) ? (GK_S32)pstDemosaic->stAuto.au8ColorNoiseStrY[u32IsoLevel - 1] : (GK_S32)pstDemosaic->stAuto.au8ColorNoiseStrY[0];
            s32Y2 = (GK_S32)pstDemosaic->stAuto.au8ColorNoiseStrY[u32IsoLevel];
            pstDmCfg->u8ColorNoiseStrY = (GK_U8)DemosaicGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);
        }

        s32Y1 = (u32IsoLevel) ? (GK_S32)pstDemosaic->stAuto.au8ColorNoiseThdF[u32IsoLevel - 1] : (GK_S32)pstDemosaic->stAuto.au8ColorNoiseThdF[0];
        s32Y2 = (GK_S32)pstDemosaic->stAuto.au8ColorNoiseThdF[u32IsoLevel];
        pstDmCfg->u16FakeCrVarThrLow = (GK_U16)DemosaicGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);
        
        s32Y1 = (u32IsoLevel) ? (GK_S32)pstDemosaic->stAuto.au8ColorNoiseStrF[u32IsoLevel - 1] : (GK_S32)pstDemosaic->stAuto.au8ColorNoiseStrF[0];
        s32Y2 = (GK_S32)pstDemosaic->stAuto.au8ColorNoiseStrF[u32IsoLevel];
        pstDmCfg->u8DeFColorCr = ISP_DEMOSAIC_COLORNOISE_STR_MAX - (GK_U8)DemosaicGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);
        pstDmCfg->u8DeFColorCb = ISP_DEMOSAIC_COLORNOISE_STR_MAX - (GK_U8)DemosaicGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

    } else if (pstDemosaic->enOpType == OP_TYPE_MANUAL) {
        u8NonDirStr               = pstDemosaic->stManual.u8NonDirStr;
        pstDmCfg->u8EhcGray       = pstDemosaic->stManual.u8NonDirMFDetailEhcStr;
        pstDmCfg->u8DitherMax     = pstDemosaic->stManual.u8NonDirHFDetailEhcStr;
        u8FilterCoefIndex         = pstDemosaic->stManual.u8DetailSmoothRange;
        pstDmCfg->u8ColorNoiseThdY = pstDemosaic->stManual.u8ColorNoiseThdY;
        pstDmCfg->u8ColorNoiseStrY = pstDemosaic->stManual.u8ColorNoiseStrY;
        pstDmCfg->u16FakeCrVarThrLow = (GK_U16)pstDemosaic->stManual.u8ColorNoiseThdF;
        pstDmCfg->u8DeFColorCr    = ISP_DEMOSAIC_COLORNOISE_STR_MAX - pstDemosaic->stManual.u8ColorNoiseStrF;
        pstDmCfg->u8DeFColorCb    = ISP_DEMOSAIC_COLORNOISE_STR_MAX - pstDemosaic->stManual.u8ColorNoiseStrF;
    }

    pstDemosaic->u8ProcNonDirStr            = u8NonDirStr;
    pstDemosaic->u8ProcNonDirMFDetailEhcStr = pstDmCfg->u8EhcGray;
    pstDemosaic->u8ProcNonDirHFDetailEhcStr = pstDmCfg->u8DitherMax;
    pstDemosaic->u8ProcDetailSmoothRange    = u8FilterCoefIndex;

    DemosaicCfg(pstDmCfg, pstStaticDmCfg, pstDemosaic, u32IsoLevel, u32ISO2, u32ISO1, u32Iso);
    Demosaic_GFBlurLut(pstDemosaic, ViPipe, pstDmCfg, u8NonDirStr, u32Iso);

    if (u8FilterCoefIndex < 1) {
        u8FilterCoefIndex = 1;
    }
    if (u8FilterCoefIndex > 7) {
        u8FilterCoefIndex = 7;
    }

    pstDmCfg->u8Lpff0 = g_as32DemosaicFilterCoef[u8FilterCoefIndex - 1][0];
    pstDmCfg->u8Lpff1 = g_as32DemosaicFilterCoef[u8FilterCoefIndex - 1][1];
    pstDmCfg->u8Lpff2 = g_as32DemosaicFilterCoef[u8FilterCoefIndex - 1][2];
    pstDmCfg->u16FakeCrVarThrHigh = ISP_DEMOSAIC_FAKECR_VAR_THR_HIGH_DEFAULT;

    pstDmCfg->bResh = GK_TRUE;

    return  GK_SUCCESS;
}

static GK_BOOL __inline CheckDemosaicOpen(ISP_DEMOSAIC_S *pstDemosaic)
{
    return (pstDemosaic->bEnable == GK_TRUE);
}

GK_S32 ISP_DemosaicRun(VI_PIPE ViPipe, const GK_VOID *pStatInfo, GK_VOID *pRegCfg, GK_S32 s32Rsv)
{
    GK_U8  i;
    ISP_REG_CFG_S *pstReg = (ISP_REG_CFG_S *)pRegCfg;
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_DEMOSAIC_S *pstDemosaic = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    DEMOSAIC_GET_CTX(ViPipe, pstDemosaic);
    ISP_CHECK_POINTER(pstDemosaic);

    /* calculate every two interrupts */
    if (!pstDemosaic->bInit) {
        return GK_SUCCESS;
    }

    if (pstIspCtx->stLinkage.enFSWDRMode != pstIspCtx->stLinkage.enPreFSWDRMode) {
        Demosaic_SetLongFrameMode(ViPipe);
    }

    if ((pstIspCtx->u32FrameCnt % 2 == 0) && (pstIspCtx->stLinkage.bSnapState != GK_TRUE)) {
        return GK_SUCCESS;
    }

    pstDemosaic->bEnable = ext_system_demosaic_enable_read(ViPipe);

    for (i = 0; i < pstReg->u8CfgNum; i++) {
        pstReg->stAlgRegCfg[i].stDemRegCfg.bVhdmEnable = pstDemosaic->bEnable;
        pstReg->stAlgRegCfg[i].stDemRegCfg.bNddmEnable = pstDemosaic->bEnable;
    }

    pstReg->unKey.bit1DemCfg = 1;

    /* check hardware setting */
    if (!CheckDemosaicOpen(pstDemosaic)) {
        return GK_SUCCESS;
    }

    DemosaicReadExtregs(ViPipe);

    for (i = 0; i < pstIspCtx->stBlockAttr.u8BlockNum; i++) {
        ISP_DemosaicFw(pstIspCtx->stLinkage.u32Iso, ViPipe, i, pstReg);
    }

    return GK_SUCCESS;
}

GK_S32 ISP_DemosaicCtrl(VI_PIPE ViPipe, GK_U32 u32Cmd, GK_VOID *pValue)
{
    ISP_DEMOSAIC_S *pstDemosaic = GK_NULL;
    ISP_REGCFG_S   *pRegCfg     = GK_NULL;

    switch (u32Cmd) {
        case ISP_WDR_MODE_SET :
            ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
            DEMOSAIC_GET_CTX(ViPipe, pstDemosaic);
            ISP_CHECK_POINTER(pRegCfg);
            ISP_CHECK_POINTER(pstDemosaic);

            pstDemosaic->bInit = GK_FALSE;
            ISP_DemosaicInit(ViPipe, (GK_VOID *)&pRegCfg->stRegCfg);
            break;

        case ISP_PROC_WRITE:
            DemosaicProcWrite(ViPipe, (ISP_CTRL_PROC_WRITE_S *)pValue);
            break;

        default :
            break;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_DemosaicExit(VI_PIPE ViPipe)
{
    GK_U16 i;
    ISP_REGCFG_S *pRegCfg    = GK_NULL;


    ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

    for (i = 0; i < pRegCfg->stRegCfg.u8CfgNum; i++) {
        pRegCfg->stRegCfg.stAlgRegCfg[i].stDemRegCfg.bNddmEnable = GK_FALSE;
        pRegCfg->stRegCfg.stAlgRegCfg[i].stDemRegCfg.bVhdmEnable = GK_FALSE;
    }

    pRegCfg->stRegCfg.unKey.bit1DemCfg = 1;

    DemosaicCtxExit(ViPipe);

    return GK_SUCCESS;
}

GK_S32 ISP_AlgRegisterDemosaic(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_ALG_NODE_S *pstAlgs = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_ALG_CHECK(pstIspCtx->unAlgKey.bit1Demosaic);
    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_DEMOSAIC;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_DemosaicInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_DemosaicRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_DemosaicCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_DemosaicExit;
    pstAlgs->bUsed = GK_TRUE;

    return GK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
