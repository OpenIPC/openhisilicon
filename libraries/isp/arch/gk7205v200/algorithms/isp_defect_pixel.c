/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <stdio.h>
#include "isp_sensor.h"
#include "isp_alg.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_math_utils.h"
#include "isp_proc.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define ISP_DPCC_MODE            (35)
#ifdef CONFIG_ISP_DPC_STATIC_TABLE_SUPPORT
#define ISP_DPCC_HOT_MODE        (7)
#define ISP_DPCC_DEAD_MODE       (71)
#define ISP_DPCC_HIGHLIGHT_MODE  (160)
#define ISP_HOT_DEV_THRESH       (20)
#define ISP_DEAD_DEV_THRESH      (15)
#endif

#define ISP_DPC_SLOPE_GRADE      (5)
#define ISP_DPC_SOFT_SLOPE_GRADE (5)
#define ISP_DPC_HIGH_SLOPE_GRADE (27)

static const GK_U16 g_au16DpcStrength[ISP_AUTO_ISO_STRENGTH_NUM]   = {0, 0, 0, 152, 200, 200, 220, 220, 220, 220, 152, 152, 152, 152, 152, 152};
static const GK_U16 g_au16DpcBlendRatio[ISP_AUTO_ISO_STRENGTH_NUM] = {0, 0, 0,  0,  0,  0,  0,  0,  0,  0, 50, 50, 50, 50, 50, 50};

static const GK_U8  g_au8SlopeGrade[ISP_DPC_SLOPE_GRADE]          =   {0, 76, 99, 100, 127};

static const GK_U8  g_au8SoftSlopeGrade[ISP_DPC_SOFT_SLOPE_GRADE]   = {0, 76, 100, 115, 120};
static const GK_U16 g_au16SoftLineThr[ISP_DPC_SOFT_SLOPE_GRADE]     = {0x5454, 0x1818, 0x1212, 0x0a0a, 0x0a0a};
static const GK_U16 g_au16SoftLineMadFac[ISP_DPC_SOFT_SLOPE_GRADE]  = {0x1810, 0x1810, 0x1810, 0x1010, 0x0a0a};

typedef struct ISP_DEFECT_PIXEL_S {
    /* Public */
    GK_BOOL bEnable;                // enable dpc module
    GK_BOOL bStatEn;
    GK_U16  u16DpccMode;
    GK_U32  u32DpccBadThresh;

    GK_BOOL bStaticEnable;

#ifdef CONFIG_ISP_DPC_STATIC_TABLE_SUPPORT
    // static calib
    GK_BOOL bStaCalibrationEn;      // enable static calibration
    GK_U8   u8PixelDetectType;       // 0: hot pixel detect; 1: dead pixel detect;
    GK_U8   u8FrameCnt;
    GK_U8   u8StaticDPThresh;
    GK_U8   u8TrialCount;
    GK_U8   u8TrialCntLimit;
    GK_U8   u8CalibStarted;
    GK_U8   u8CalibFinished;
    GK_U8   u8HotDevThresh;
    GK_U8   u8DeadDevThresh;
    GK_U16  u16DpCountMax;
    GK_U16  u16DpCountMin;
    GK_U16  u16BptCalibNum;
    GK_U16  au16BlkBpCalibNum[ISP_STRIPING_MAX_NUM];
    GK_U32  *pau32BptCalibTable[ISP_STRIPING_MAX_NUM]; // for calibration,max size: ISP_STRIPING_MAX_NUM * ISP_DPC_MAX_BPT_NUM_NORMAL

    // static cor
    GK_BOOL bStaicShow;
    GK_BOOL bStaticAttrUpdate;
    GK_U16  u16BptCorNum;
    GK_U16  au16Offset[ISP_STRIPING_MAX_NUM + 1];
    GK_U16  au16OffsetForSplit[ISP_STRIPING_MAX_NUM + 1];
#endif

    // dynamic cor
    GK_BOOL bDynamicEnable;
    GK_BOOL bDynamicManual;
    GK_BOOL bSupTwinkleEn;
    GK_BOOL bDynaAttrUpdateEn;
    GK_U16  u16BlendRatio;
    GK_U16  u16Strength;
    GK_S8   s8SupTwinkleThr;
    GK_U8   u8SupTwinkleSlope;
    GK_BOOL bSupTwinkleEnAdapt;
    GK_U16  u16ProcInfoBlendRatio;
    GK_U16  u16ProcInfoStrength;

    ISP_CMOS_DPC_S stCmosDpc;
} ISP_DEFECT_PIXEL_S;

typedef struct {
    GK_U8  u8DpccSetUse;
    GK_U16 u16DpccMethodsSet1;
    GK_U16 u16DpccMethodsSet2;
    GK_U16 u16DpccMethodsSet3;
    GK_U32 u32DpccBadThresh;
} ISP_DPC_CFG_S;

static const ISP_DPC_CFG_S g_stDpcDefCfg[ISP_DPC_SLOPE_GRADE] = {
    {0x01, 0x1F1F, 0x0707, 0x1F1F, 0xff800080},  // 0~75
    {0x03, 0x1F1F, 0x0707, 0x1F1F, 0xff800080},  // ori set 1  (76)
    {0x03, 0x1F1F, 0x0707, 0x1F1F, 0xff800080},  // ori set 2 (99)
    {0x07, 0x1F1F, 0x0707, 0x1F1F, 0xff800080},  // set 23(RB set3, G set2) (100)
    {0x07, 0x1F1F, 0x0707, 0x1F1F, 0xff800080},  // 101 ~127
};

typedef struct {
    GK_U8  au8DpccLineThr[2][3];
    GK_U8  au8DpccLineMadFac[2][3];
    GK_U8  au8DpccPgFac[2][3];
    GK_U8  au8DpccRndThr[2][3];
    GK_U8  au8DpccRgFac[2][3];
    GK_U8  au8DpccRo[2][3];
    GK_U8  au8DpccRndOffs[2][3];
    GK_U16 au16DpccRndThrMtp[2][3];
    GK_U16 au16DpccRgFacMtp[2][3];
} ISP_DPCC_DERIVED_PARAM_S;

static const ISP_DPCC_DERIVED_PARAM_S g_stDpcDerParam[ISP_DPC_SLOPE_GRADE] = {
    // 0
    {
        {{0x54, 0x21, 0x20}, {0x54, 0x21, 0x20}},
        {{0x1B, 0x18, 0x04}, {0x1B, 0x10, 0x04}},
        {{0x08, 0x0B, 0x0A}, {0x08, 0x0B, 0x0A}},
        {{0x0A, 0x08, 0x08}, {0x0A, 0x08, 0x06}},
        {{0x26, 0x08, 0x04}, {0x26, 0x08, 0x04}},
        {{0x01, 0x02, 0x02}, {0x01, 0x02, 0x01}},
        {{0x02, 0x02, 0x02}, {0x02, 0x02, 0x02}}
    },

    // 76
    {
        {{0x08, 0x21, 0x20}, {0x08, 0x21, 0x20}},
        {{0x1B, 0x18, 0x04}, {0x1B, 0x10, 0x04}},
        {{0x08, 0x0B, 0x0A}, {0x08, 0x0B, 0x0A}},
        {{0x0A, 0x08, 0x08}, {0x0A, 0x08, 0x06}},
        {{0x26, 0x08, 0x04}, {0x26, 0x08, 0x04}},
        {{0x01, 0x02, 0x02}, {0x01, 0x02, 0x01}},
        {{0x02, 0x02, 0x02}, {0x02, 0x02, 0x02}}
    },

    // 99
    {
        {{0x08, 0x10, 0x20}, {0x08, 0x10, 0x20}},
        {{0x04, 0x18, 0x04}, {0x04, 0x10, 0x04}},
        {{0x08, 0x08, 0x0A}, {0x08, 0x06, 0x0A}},
        {{0x0A, 0x08, 0x08}, {0x0A, 0x08, 0x06}},
        {{0x20, 0x08, 0x04}, {0x20, 0x08, 0x04}},
        {{0x02, 0x03, 0x02}, {0x02, 0x03, 0x01}},
        {{0x03, 0x03, 0x03}, {0x03, 0x03, 0x03}}
    },

    // 100
    {
        {{0x08, 0x10, 0x20}, {0x08, 0x10, 0x20}},
        {{0x04, 0x18, 0x04}, {0x04, 0x10, 0x04}},
        {{0x08, 0x08, 0x0A}, {0x08, 0x06, 0x0A}},
        {{0x0A, 0x08, 0x08}, {0x0A, 0x08, 0x06}},
        {{0x20, 0x08, 0x04}, {0x20, 0x08, 0x04}},
        {{0x02, 0x03, 0x02}, {0x02, 0x03, 0x01}},
        {{0x03, 0x03, 0x03}, {0x03, 0x03, 0x03}}
    }, // 100

    // 127
    {
        {{0x01, 0x10, 0x20}, {0x01, 0x10, 0x20}},
        {{0x03, 0x18, 0x04}, {0x03, 0x10, 0x04}},
        {{0x03, 0x08, 0x0A}, {0x03, 0x06, 0x0A}},
        {{0x04, 0x08, 0x08}, {0x04, 0x08, 0x06}},
        {{0x08, 0x08, 0x04}, {0x08, 0x08, 0x04}},
        {{0x02, 0x03, 0x02}, {0x02, 0x03, 0x01}},
        {{0x03, 0x03, 0x03}, {0x03, 0x03, 0x03}}
    },
};

typedef struct {
    GK_U16 u16DpcLineThr1;
    GK_U16 u16DpcLineMadFac1;
    GK_U16 u16DpcPgFac1;
    GK_U16 u16DpcRndThr1;
    GK_U16 u16DpcRgFac1;
    GK_U16 u16DpcRoLimits1;
    GK_U16 u16DpcRndOffs1;
} ISP_DPCC_HIGH_DERIVED_PARAM_S;

static const ISP_DPCC_HIGH_DERIVED_PARAM_S g_stDpcHighDerParam[ISP_DPC_HIGH_SLOPE_GRADE] = {
    {0x0808, 0x0404, 0x0404, 0x0a0a, 0x1f1f, 0x0dfa, 0x0fff},
    {0x0808, 0x0404, 0x0404, 0x0a0a, 0x1e1e, 0x0dfa, 0x0fff},
    {0x0808, 0x0404, 0x0404, 0x0a0a, 0x1d1d, 0x0efe, 0x0fff},
    {0x0808, 0x0404, 0x0303, 0x0a0a, 0x1d1d, 0x0efe, 0x0fff},
    {0x0808, 0x0404, 0x0303, 0x0808, 0x1d1d, 0x0efe, 0x0fff},
    {0x0808, 0x0404, 0x0303, 0x0808, 0x1b1b, 0x0efe, 0x0fff},
    {0x0808, 0x0404, 0x0303, 0x0808, 0x1919, 0x0efe, 0x0fff},
    {0x0808, 0x0404, 0x0303, 0x0808, 0x1717, 0x0efe, 0x0fff},
    {0x0808, 0x0404, 0x0303, 0x0808, 0x1515, 0x0efe, 0x0fff},
    {0x0808, 0x0404, 0x0303, 0x0808, 0x1313, 0x0efe, 0x0fff},
    {0x0808, 0x0404, 0x0303, 0x0808, 0x1111, 0x0efe, 0x0fff},
    {0x0808, 0x0404, 0x0303, 0x0808, 0x1010, 0x0fff, 0x0fff},
    {0x0707, 0x0404, 0x0303, 0x0707, 0x1010, 0x0fff, 0x0fff},
    {0x0707, 0x0404, 0x0303, 0x0606, 0x1010, 0x0fff, 0x0fff},
    {0x0707, 0x0404, 0x0303, 0x0505, 0x1010, 0x0fff, 0x0fff},
    {0x0606, 0x0404, 0x0303, 0x0404, 0x1010, 0x0fff, 0x0fff},
    {0x0606, 0x0404, 0x0303, 0x0404, 0x0d0d, 0x0fff, 0x0fff},
    {0x0505, 0x0404, 0x0303, 0x0404, 0x0c0c, 0x0fff, 0x0fff},
    {0x0505, 0x0404, 0x0303, 0x0404, 0x0a0a, 0x0fff, 0x0fff},
    {0x0404, 0x0404, 0x0303, 0x0404, 0x0a0a, 0x0fff, 0x0fff},
    {0x0404, 0x0404, 0x0303, 0x0404, 0x0a0a, 0x0fff, 0x0fff},
    {0x0303, 0x0404, 0x0303, 0x0404, 0x0808, 0x0fff, 0x0fff},
    {0x0303, 0x0303, 0x0303, 0x0404, 0x0808, 0x0fff, 0x0fff},
    {0x0202, 0x0303, 0x0303, 0x0404, 0x0808, 0x0fff, 0x0fff},
    {0x0202, 0x0303, 0x0303, 0x0404, 0x0606, 0x0fff, 0x0fff},
    {0x0202, 0x0303, 0x0303, 0x0404, 0x0404, 0x0fff, 0x0fff},
    {0x0202, 0x0303, 0x0303, 0x0404, 0x0202, 0x0fff, 0x0fff}
};

ISP_DEFECT_PIXEL_S *g_pastDpCtx[ISP_MAX_PIPE_NUM] = {GK_NULL};

#define DP_GET_CTX(dev, pstCtx)   (pstCtx = g_pastDpCtx[dev])
#define DP_SET_CTX(dev, pstCtx)   (g_pastDpCtx[dev] = pstCtx)
#define DP_RESET_CTX(dev)         (g_pastDpCtx[dev] = GK_NULL)

static GK_S32 DpCtxInit(VI_PIPE ViPipe)
{
    ISP_DEFECT_PIXEL_S *pastDpCtx = GK_NULL;

    DP_GET_CTX(ViPipe, pastDpCtx);

    if (pastDpCtx == GK_NULL) {
        pastDpCtx = (ISP_DEFECT_PIXEL_S *)ISP_MALLOC(sizeof(ISP_DEFECT_PIXEL_S));
        if (pastDpCtx == GK_NULL) {
            ISP_TRACE(MODULE_DBG_ERR, "Isp[%d] DpCtx malloc memory failed!\n", ViPipe);
            return ERR_CODE_ISP_NOMEM;
        }
    }

    memset(pastDpCtx, 0, sizeof(ISP_DEFECT_PIXEL_S));

    DP_SET_CTX(ViPipe, pastDpCtx);

    return GK_SUCCESS;
}

static GK_VOID DpCtxExit(VI_PIPE ViPipe)
{
    ISP_DEFECT_PIXEL_S *pastDpCtx = GK_NULL;

    DP_GET_CTX(ViPipe, pastDpCtx);
    ISP_FREE(pastDpCtx);
    DP_RESET_CTX(ViPipe);
}

static GK_U8 DpGetChnNum(GK_U8 u8WDRMode)
{
    if (IS_LINEAR_MODE(u8WDRMode)) {
        return 1;
    } else if (IS_BUILT_IN_WDR_MODE(u8WDRMode)) {
        return 1;
    } else if (IS_2to1_WDR_MODE(u8WDRMode)) {
        return 1;
    } else {
        /* unknow u8Mode */
        return 1;
    }
}

#ifdef CONFIG_ISP_DPC_STATIC_TABLE_SUPPORT
static GK_VOID Dpc_SafeFreeCalibLut(ISP_DEFECT_PIXEL_S *pstDp, GK_U8 u8Cnt)
{
    GK_U8 i;

    for (i = 0; i < u8Cnt; i++) {
        ISP_FREE(pstDp->pau32BptCalibTable[i]);
    }
}
#endif

static GK_VOID ISP_DpEnableCfg(VI_PIPE ViPipe, GK_U8 u8CfgNum, ISP_REG_CFG_S *pstRegCfg)
{
    GK_U8     i, j, u8ChnNum;
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    u8ChnNum = DpGetChnNum(pstIspCtx->u8SnsWDRMode);

    for (i = 0; i < u8CfgNum; i++) {
        for (j = 0; j < 4; j++) {
            pstRegCfg->stAlgRegCfg[i].stDpRegCfg.abDpcEn[j] = (j < u8ChnNum) ? (GK_TRUE) : (GK_FALSE);
        }
    }
}

#ifdef CONFIG_ISP_DPC_STATIC_TABLE_SUPPORT
static GK_VOID DpStaticRegsInitialize(ISP_DPC_STATIC_CFG_S *pstStaticRegCfg)
{
    pstStaticRegCfg->u32DpccBptCtrl    = ISP_DPC_DEFAULT_BPT_CTRL;
    pstStaticRegCfg->u8DpccOutputMode  = ISP_DPC_DEFAULT_OUTPUT_MODE;
    pstStaticRegCfg->bStaticResh       = GK_TRUE;
}
#endif

static GK_VOID DpUsrRegsInitialize(VI_PIPE ViPipe, ISP_DPC_USR_CFG_S *pstUsrRegCfg)
{
    ISP_DEFECT_PIXEL_S *pstDp = GK_NULL;

    DP_GET_CTX(ViPipe, pstDp);

    pstUsrRegCfg->stUsrDynaCorRegCfg.bDpccHardThrEn         = ISP_DPC_DEFAULT_HARD_THR_ENABLE;
    pstUsrRegCfg->stUsrDynaCorRegCfg.s8DpccSupTwinkleThrMax = ISP_DPC_DEFAULT_SOFT_THR_MAX;
    pstUsrRegCfg->stUsrDynaCorRegCfg.s8DpccSupTwinkleThrMin = ISP_DPC_DEFAULT_SOFT_THR_MIN;
    pstUsrRegCfg->stUsrDynaCorRegCfg.u16DpccRakeRatio       = ISP_DPC_DEFAULT_SOFT_RAKE_RATIO;
    pstUsrRegCfg->stUsrDynaCorRegCfg.u8DPCPos               = pstDp->stCmosDpc.u8DpcPos;
    pstUsrRegCfg->stUsrDynaCorRegCfg.bResh                  = GK_TRUE;

    pstUsrRegCfg->stUsrStaCorRegCfg.u16DpccBptNumber        = ISP_DPC_DEFAULT_BPT_NUMBER;
    pstUsrRegCfg->stUsrStaCorRegCfg.u32UpdateIndex          = 1;
    pstUsrRegCfg->stUsrStaCorRegCfg.u8BufId                 = 0;
    pstUsrRegCfg->stUsrStaCorRegCfg.bResh                   = GK_FALSE;
}

static GK_VOID DpDynaRegsInitialize(VI_PIPE ViPipe, ISP_DPC_DYNA_CFG_S *pstDynaRegCfg)
{
    pstDynaRegCfg->bResh                 = GK_TRUE;
    pstDynaRegCfg->bDpcStatEn            = 0;
    pstDynaRegCfg->u32DpccAlpha          = ISP_DPC_DEFAULT_ALPHA;
    pstDynaRegCfg->u16DpccMode           = ISP_DPC_DEFAULT_MODE;
    pstDynaRegCfg->u8DpccSetUse         = ISP_DPC_DEFAULT_SET_USE;
    pstDynaRegCfg->u16DpccMethodsSet1    = ISP_DPC_DEFAULT_METHODS_SET_1;
    pstDynaRegCfg->u16DpccMethodsSet2    = ISP_DPC_DEFAULT_METHODS_SET_2;
    pstDynaRegCfg->u16DpccMethodsSet3    = ISP_DPC_DEFAULT_METHODS_SET_3;
    pstDynaRegCfg->au16DpccLineThr[0]    = ISP_DPC_DEFAULT_LINE_THRESH_1;
    pstDynaRegCfg->au16DpccLineMadFac[0] = ISP_DPC_DEFAULT_LINE_MAD_FAC_1;
    pstDynaRegCfg->au16DpccPgFac[0]      = ISP_DPC_DEFAULT_PG_FAC_1;
    pstDynaRegCfg->au16DpccRndThr[0]     = ISP_DPC_DEFAULT_RND_THRESH_1;
    pstDynaRegCfg->au16DpccRgFac[0]      = ISP_DPC_DEFAULT_RG_FAC_1;
    pstDynaRegCfg->au16DpccLineThr[1]    = ISP_DPC_DEFAULT_LINE_THRESH_2;
    pstDynaRegCfg->au16DpccLineMadFac[1] = ISP_DPC_DEFAULT_LINE_MAD_FAC_2;
    pstDynaRegCfg->au16DpccPgFac[1]      = ISP_DPC_DEFAULT_PG_FAC_2;
    pstDynaRegCfg->au16DpccRndThr[1]     = ISP_DPC_DEFAULT_RND_THRESH_2;
    pstDynaRegCfg->au16DpccRgFac[1]      = ISP_DPC_DEFAULT_RG_FAC_2;
    pstDynaRegCfg->au16DpccLineThr[2]    = ISP_DPC_DEFAULT_LINE_THRESH_3;
    pstDynaRegCfg->au16DpccLineMadFac[2] = ISP_DPC_DEFAULT_LINE_MAD_FAC_3;
    pstDynaRegCfg->au16DpccPgFac[2]      = ISP_DPC_DEFAULT_PG_FAC_3;
    pstDynaRegCfg->au16DpccRndThr[2]     = ISP_DPC_DEFAULT_RND_THRESH_3;
    pstDynaRegCfg->au16DpccRgFac[2]      = ISP_DPC_DEFAULT_RG_FAC_3;
    pstDynaRegCfg->u16DpccRoLimits       = ISP_DPC_DEFAULT_RO_LIMITS;
    pstDynaRegCfg->u16DpccRndOffs        = ISP_DPC_DEFAULT_RND_OFFS;
    pstDynaRegCfg->u32DpccBadThresh      = ISP_DPC_DEFAULT_BPT_THRESH;

    pstDynaRegCfg->au16DpccLineStdThr[0]  = ISP_DPC_DEFAULT_LINE_STD_THR_1 * 9;
    pstDynaRegCfg->au16DpccLineStdThr[1]  = ISP_DPC_DEFAULT_LINE_STD_THR_2 * 9;
    pstDynaRegCfg->au16DpccLineStdThr[2]  = ISP_DPC_DEFAULT_LINE_STD_THR_3 * 9;
    pstDynaRegCfg->au16DpccLineStdThr[3]  = ISP_DPC_DEFAULT_LINE_STD_THR_4 * 9;
    pstDynaRegCfg->au16DpccLineStdThr[4]  = ISP_DPC_DEFAULT_LINE_STD_THR_5 * 9;


    pstDynaRegCfg->au8DpccLineDiffThr[0] = ISP_DPC_DEFAULT_LINE_DIFF_THR_1;
    pstDynaRegCfg->au8DpccLineDiffThr[1] = ISP_DPC_DEFAULT_LINE_DIFF_THR_2;
    pstDynaRegCfg->au8DpccLineDiffThr[2] = ISP_DPC_DEFAULT_LINE_DIFF_THR_3;
    pstDynaRegCfg->au8DpccLineDiffThr[3] = ISP_DPC_DEFAULT_LINE_DIFF_THR_4;
    pstDynaRegCfg->au8DpccLineDiffThr[4] = ISP_DPC_DEFAULT_LINE_DIFF_THR_5;

    pstDynaRegCfg->au8DpccLineAverFac[0] = ISP_DPC_DEFAULT_LINE_AVER_FAC_1;
    pstDynaRegCfg->au8DpccLineAverFac[1] = ISP_DPC_DEFAULT_LINE_AVER_FAC_2;
    pstDynaRegCfg->au8DpccLineAverFac[2] = ISP_DPC_DEFAULT_LINE_AVER_FAC_3;
    pstDynaRegCfg->au8DpccLineAverFac[3] = ISP_DPC_DEFAULT_LINE_AVER_FAC_4;
    pstDynaRegCfg->au8DpccLineAverFac[4] = ISP_DPC_DEFAULT_LINE_AVER_FAC_5;

    pstDynaRegCfg->u8DpccLineKerdiffFac  = ISP_DPC_DEFAULT_LINE_KERDIFF_FAC;
    pstDynaRegCfg->u8DpccBlendMode       = ISP_DPC_DEFAULT_BLEND_MODE;
    pstDynaRegCfg->u8DpccBitDepthSel     = ISP_DPC_DEFAULT_BIT_DEPTH_SEL;
}

static GK_VOID DpRegsInitialize(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfg)
{
    GK_U8     i;

    for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
#ifdef CONFIG_ISP_DPC_STATIC_TABLE_SUPPORT
        DpStaticRegsInitialize(&pstRegCfg->stAlgRegCfg[i].stDpRegCfg.stStaticRegCfg);
#endif
        DpDynaRegsInitialize(ViPipe, &pstRegCfg->stAlgRegCfg[i].stDpRegCfg.stDynaRegCfg);
        DpUsrRegsInitialize(ViPipe, &pstRegCfg->stAlgRegCfg[i].stDpRegCfg.stUsrRegCfg);

        pstRegCfg->stAlgRegCfg[i].stDpRegCfg.bLut2SttEn = GK_TRUE;
    }

    ISP_DpEnableCfg(ViPipe, pstRegCfg->u8CfgNum, pstRegCfg);

    pstRegCfg->unKey.bit1DpCfg = 1;
}

static GK_VOID DpExtRegsInitialize(VI_PIPE ViPipe)
{
    GK_U8 i;
    ISP_DEFECT_PIXEL_S *pstDp     = GK_NULL;

    DP_GET_CTX(ViPipe,  pstDp);

    // dynamic attr
    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        ext_system_dpc_dynamic_strength_table_write(ViPipe, i, pstDp->stCmosDpc.au16Strength[i]);
        ext_system_dpc_dynamic_blend_ratio_table_write(ViPipe, i, pstDp->stCmosDpc.au16BlendRatio[i]);
    }

    ext_system_dpc_dynamic_cor_enable_write(ViPipe, pstDp->bEnable);
    ext_system_dpc_manual_mode_write(ViPipe, ISP_EXT_SYSTEM_DPC_MANU_MODE_DEFAULT);
    ext_system_dpc_dynamic_manual_enable_write(ViPipe, ISP_EXT_SYSTEM_DPC_DYNAMIC_MANUAL_ENABLE_DEFAULT);
    ext_system_dpc_dynamic_strength_write(ViPipe, ISP_EXT_SYSTEM_DPC_DYNAMIC_STRENGTH_DEFAULT);
    ext_system_dpc_dynamic_blend_ratio_write(ViPipe, ISP_EXT_SYSTEM_DPC_DYNAMIC_BLEND_RATIO_DEFAULT);
    ext_system_dpc_suppress_twinkle_enable_write(ViPipe, ISP_EXT_SYSTEM_DPC_SUPPRESS_TWINKLE_ENABLE_DEFAULT);
    ext_system_dpc_suppress_twinkle_thr_write(ViPipe, ISP_EXT_SYSTEM_DPC_SUPPRESS_TWINKLE_THR_DEFAULT);
    ext_system_dpc_suppress_twinkle_slope_write(ViPipe, ISP_EXT_SYSTEM_DPC_SUPPRESS_TWINKLE_SLOPE_DEFAULT);
    ext_system_dpc_dynamic_attr_update_write(ViPipe, GK_TRUE);

    // static calib
    ext_system_dpc_static_calib_enable_write(ViPipe, ISP_EXT_SYSTEM_DPC_STATIC_CALIB_ENABLE_DEFAULT);
    ext_system_dpc_count_max_write(ViPipe, ISP_EXT_SYSTEM_DPC_COUNT_MAX_DEFAULT);
    ext_system_dpc_count_min_write(ViPipe, ISP_EXT_SYSTEM_DPC_COUNT_MIN_DEFAULT);
    ext_system_dpc_start_thresh_write(ViPipe, ISP_EXT_SYSTEM_DPC_START_THRESH_DEFAULT);
    ext_system_dpc_trigger_status_write(ViPipe, ISP_EXT_SYSTEM_DPC_TRIGGER_STATUS_DEFAULT);
    ext_system_dpc_trigger_time_write(ViPipe, ISP_EXT_SYSTEM_DPC_TRIGGER_TIME_DEFAULT);
    ext_system_dpc_static_defect_type_write(ViPipe, ISP_EXT_SYSTEM_DPC_STATIC_DEFECT_TYPE_DEFAULT);
    ext_system_dpc_finish_thresh_write(ViPipe, ISP_EXT_SYSTEM_DPC_START_THRESH_DEFAULT);
    ext_system_dpc_bpt_calib_number_write(ViPipe, ISP_EXT_SYSTEM_DPC_BPT_CALIB_NUMBER_DEFAULT);
    // static attr
    ext_system_dpc_bpt_cor_number_write(ViPipe, ISP_EXT_SYSTEM_DPC_BPT_COR_NUMBER_DEFAULT);
    ext_system_dpc_static_cor_enable_write(ViPipe, pstDp->bEnable);
    ext_system_dpc_static_dp_show_write(ViPipe, ISP_EXT_SYSTEM_DPC_STATIC_DP_SHOW_DEFAULT);
    ext_system_dpc_static_attr_update_write(ViPipe, GK_TRUE);

    // debug
    ext_system_dpc_alpha0_rb_write(ViPipe, ISP_EXT_SYSTEM_DPC_DYNAMIC_ALPHA0_RB_DEFAULT);
    ext_system_dpc_alpha0_g_write(ViPipe, ISP_EXT_SYSTEM_DPC_DYNAMIC_ALPHA0_G_DEFAULT);
}

#ifdef CONFIG_ISP_DPC_STATIC_TABLE_SUPPORT
static GK_VOID DpcImageSize(VI_PIPE ViPipe, ISP_DEFECT_PIXEL_S *pstDp)
{
    GK_U8      i, u8BlockNum;
    ISP_RECT_S stBlockRect;
    ISP_CTX_S  *pstIspCtx = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    u8BlockNum = pstIspCtx->stBlockAttr.u8BlockNum;

    for (i = 0; i < u8BlockNum; i++) {
        ISP_GetBlockRect(&stBlockRect, &pstIspCtx->stBlockAttr, i);

        pstDp->au16Offset[i] = stBlockRect.s32X;
        pstDp->au16OffsetForSplit[i] = (i == 0) ? 0 : (pstDp->au16Offset[i] + pstIspCtx->stBlockAttr.u32OverLap);
    }

    pstDp->au16OffsetForSplit[u8BlockNum] = pstIspCtx->stBlockAttr.stFrameRect.u32Width;
    pstDp->au16Offset[u8BlockNum]         = pstIspCtx->stBlockAttr.stFrameRect.u32Width;
}
#endif

static GK_S32 DpcCheckCmosParam(VI_PIPE ViPipe, const ISP_CMOS_DPC_S *pstCmosDpc)
{
    GK_U8 i;
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    if (!IS_LINEAR_MODE(pstIspCtx->stWdrAttr.enWDRMode)) {
        if (pstCmosDpc->u8DpcPos != ISP_DPC_DEFAULT_POS_WDR) {
            ISP_TRACE(MODULE_DBG_ERR, "err u8DpcPos, only Support 0 in WDR mode!\n");
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
    } else {
        if ((pstCmosDpc->u8DpcPos != ISP_DPC_DEFAULT_POS_LINEAR) && (pstCmosDpc->u8DpcPos != ISP_DPC_DEFAULT_POS_WDR)) {
            ISP_TRACE(MODULE_DBG_ERR, "err u8DpcPos!\n");
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (pstCmosDpc->au16Strength[i] > 255) {
            ISP_TRACE(MODULE_DBG_ERR, "err au16Strength[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstCmosDpc->au16BlendRatio[i] > 0x80) {
            ISP_TRACE(MODULE_DBG_ERR, "err au16BlendRatio[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
    }

    return GK_SUCCESS;
}

static GK_S32 DpInitialize(VI_PIPE ViPipe)
{
    GK_S32 s32Ret;
    ISP_DEFECT_PIXEL_S  *pstDp      = GK_NULL;
    ISP_CTX_S           *pstIspCtx  = GK_NULL;
    ISP_CMOS_DEFAULT_S  *pstSnsDft  = GK_NULL;

    DP_GET_CTX(ViPipe, pstDp);
    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_SensorGetDefault(ViPipe, &pstSnsDft);

    ISP_CHECK_POINTER(pstDp);

    if (pstIspCtx->stHdrAttr.enDynamicRange == DYNAMIC_RANGE_XDR) {
        pstDp->bEnable = GK_FALSE;
    } else {
        pstDp->bEnable = GK_TRUE;
    }

    if (pstSnsDft->unKey.bit1Dpc) {
        ISP_CHECK_POINTER(pstSnsDft->pstDpc);

        s32Ret = DpcCheckCmosParam(ViPipe, pstSnsDft->pstDpc);
        if (s32Ret != GK_SUCCESS) {
            return s32Ret;
        }

        memcpy(&pstDp->stCmosDpc, pstSnsDft->pstDpc, sizeof(ISP_CMOS_DPC_S));
    } else {
        memcpy(pstDp->stCmosDpc.au16Strength,   g_au16DpcStrength,   ISP_AUTO_ISO_STRENGTH_NUM * sizeof(GK_U16));
        memcpy(pstDp->stCmosDpc.au16BlendRatio, g_au16DpcBlendRatio, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(GK_U16));
    }

#ifdef CONFIG_ISP_DPC_STATIC_TABLE_SUPPORT
    pstDp->u8TrialCount     = 0;
    pstDp->u8CalibStarted   = 0;
    pstDp->u8CalibFinished  = 0;
    pstDp->bStatEn          = 0;
    pstDp->u8HotDevThresh   = ISP_HOT_DEV_THRESH;
    pstDp->u8DeadDevThresh  = ISP_DEAD_DEV_THRESH;
    pstDp->u8FrameCnt       = 0;
    pstDp->u32DpccBadThresh = 0xff800080;

    DpcImageSize(ViPipe, pstDp);
#endif

    return GK_SUCCESS;
}

#ifdef CONFIG_ISP_DPC_STATIC_TABLE_SUPPORT
static GK_VOID DpEnter(VI_PIPE ViPipe, ISP_DEFECT_PIXEL_S *pstDp)
{
    ISP_SensorSetPixelDetect(ViPipe, GK_TRUE);
    pstDp->u8StaticDPThresh = ext_system_dpc_start_thresh_read(ViPipe);
    pstDp->u8CalibStarted   = 1;
}

static GK_VOID DpExit(VI_PIPE ViPipe, ISP_DEFECT_PIXEL_S *pstDp, ISP_REG_CFG_S *pstRegCfg)
{
    DpInitialize(ViPipe);
    ISP_SensorSetPixelDetect(ViPipe, GK_FALSE);
    pstDp->u8CalibStarted  = 0;
    pstDp->u8CalibFinished = 1;
}

static GK_VOID DpReadStaticCalibExtregs(VI_PIPE ViPipe)
{
    ISP_DEFECT_PIXEL_S *pstDp = GK_NULL;

    DP_GET_CTX(ViPipe, pstDp);

    pstDp->u8PixelDetectType = ext_system_dpc_static_defect_type_read(ViPipe);
    pstDp->u8TrialCntLimit   = (GK_U8)(ext_system_dpc_trigger_time_read(ViPipe) >> 3);
    pstDp->u16DpCountMax     = ext_system_dpc_count_max_read(ViPipe);
    pstDp->u16DpCountMin     = ext_system_dpc_count_min_read(ViPipe);
}

static GK_S32 Dpc_Read_Calib_Num(VI_PIPE ViPipe, GK_U8 u8BlkNum, ISP_DEFECT_PIXEL_S *pstDp, ISP_STAT_S *pStatInfo)
{
    GK_U8  i;
    GK_U16 u16BptCnt = 0, j;
    GK_U16 u16CntTemp = 0;
    GK_U16 au16BpCalibNum[ISP_STRIPING_MAX_NUM] = {0};
    GK_U32 u32BptValue;
    ISP_CTX_S *pstIspCtx  = GK_NULL;
    S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    /* u8blkNum == 1,only read au16BpCalibNum;
          u8blkNum >1,nead to exclude bad pixel in overlap area */
    if (IS_ONLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode)) {
    } else if (IS_SIDEBYSIDE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode)) {
        for (i = 0; i < u8BlkNum; i++) {
            ISP_CHECK_POINTER(pstDp->pau32BptCalibTable[i]);
            pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, i);
            ISP_CHECK_POINTER(pstBeReg);

            for (j = 0; j < u16CntTemp; j++) {
                if ((u32BptValue & 0x1FFF) < pstDp->au16Offset[i + 1]) {
                    pstDp->pau32BptCalibTable[i][au16BpCalibNum[i]++] = u32BptValue;
                    u16BptCnt++;
                }
            }

            pstDp->au16BlkBpCalibNum[i] = au16BpCalibNum[i];
        }

        pstDp->u16BptCalibNum = u16BptCnt;
    } else if (IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode)) {
        pstDp->u16BptCalibNum = pStatInfo->stDpStat.au16DefectPixelCount[0];
    } else if (IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode)) {
        for (i = 0; i < u8BlkNum; i++) {
            ISP_CHECK_POINTER(pstDp->pau32BptCalibTable[i]);
            u16CntTemp = pStatInfo->stDpStat.au16DefectPixelCount[i];

            for (j = 0; j < u16CntTemp; j++) {
                u32BptValue = pStatInfo->stDpStat.au32DefectPixelLut[i][j] + pstDp->au16Offset[i];

                if ((u32BptValue & 0x1FFF) < pstDp->au16Offset[i + 1]) {
                    pstDp->pau32BptCalibTable[i][au16BpCalibNum[i]++] = u32BptValue;
                    u16BptCnt++;
                }
            }

            pstDp->au16BlkBpCalibNum[i] = au16BpCalibNum[i];
        }

        pstDp->u16BptCalibNum = u16BptCnt;
    }

    return GK_SUCCESS;
}

static GK_VOID Dpc_Calib_TimeOut(VI_PIPE ViPipe, ISP_DEFECT_PIXEL_S *pstDp)
{
    printf("BAD PIXEL CALIBRATION TIME OUT  0x%x\n", pstDp->u8TrialCntLimit);
    pstDp->bStaCalibrationEn = GK_FALSE;
    ext_system_dpc_static_calib_enable_write(ViPipe, GK_FALSE);
    ext_system_dpc_finish_thresh_write(ViPipe, pstDp->u8StaticDPThresh);
    ext_system_dpc_trigger_status_write(ViPipe, 0x2);
}

static GK_VOID Dpc_Calib_Max(GK_U16 u16BadPixelsCount, ISP_DEFECT_PIXEL_S *pstDp)
{
    printf("BAD_PIXEL_COUNT_UPPER_LIMIT 0x%x, 0x%x\n", pstDp->u8StaticDPThresh, u16BadPixelsCount);
    pstDp->u8FrameCnt = 2;
    pstDp->u8TrialCount ++;
}

static GK_VOID Dpc_Calib_Min(GK_U16 u16BadPixelsCount, ISP_DEFECT_PIXEL_S *pstDp)
{
    printf("BAD_PIXEL_COUNT_LOWER_LIMIT 0x%x, 0x%x\n", pstDp->u8StaticDPThresh, u16BadPixelsCount);
    pstDp->u8FrameCnt = 2;
    pstDp->u8TrialCount ++;
}

static GK_S32 SortingDpCalibLut(GK_U32 *pu32Lut0, GK_U32 *pu32Lut1, GK_U16 u16Cnt0, GK_U16 u16Cnt1)
{
    GK_U16 i = 0;
    GK_U16 j = 0;
    GK_U16 u16CntSum = 0;
    GK_U32 *pu32TempLut = GK_NULL;

    pu32TempLut = (GK_U32 *)ISP_MALLOC((u16Cnt0 + u16Cnt1) * sizeof(GK_U32));

    if (pu32TempLut == GK_NULL) {
        return GK_FAILURE;
    }

    while ((i <  u16Cnt0) && (j < u16Cnt1)) {
        if (pu32Lut0[i] > (pu32Lut1[j])) {
            pu32TempLut[u16CntSum++] = pu32Lut1[j++];
        } else if (pu32Lut0[i] < (pu32Lut1[j])) {
            pu32TempLut[u16CntSum++] = pu32Lut0[i++];
        } else {
            pu32TempLut[u16CntSum++] = pu32Lut0[i];
            i++;
            j++;
        }
    }

    if (i >=  u16Cnt0) {
        while (j < u16Cnt1) {
            pu32TempLut[u16CntSum++] = pu32Lut1[j++];
        }
    }

    if (j >=  u16Cnt1) {
        while (i < u16Cnt0) {
            pu32TempLut[u16CntSum++] = pu32Lut0[i++];
        }
    }

    memcpy(pu32Lut0, pu32TempLut, u16CntSum * sizeof(GK_U32));

    ISP_FREE(pu32TempLut);

    return u16CntSum;
}

static GK_S32 MergingDpCalibLut(VI_PIPE ViPipe, ISP_DEFECT_PIXEL_S  *pstDp, GK_U8 u8BlkNum)
{
    GK_U8  k;
    GK_U16 i = 0;
    GK_U16 u16BpNum = 0;
    GK_S32 s32CntTemp = 0;
    GK_U32 *pu32BpTable = GK_NULL;

    for (k = 0; k < u8BlkNum; k++) {
        ISP_CHECK_POINTER(pstDp->pau32BptCalibTable[k]);
    }

    pu32BpTable = (GK_U32 *)ISP_MALLOC(pstDp->u16BptCalibNum * sizeof(GK_U32));
    memset(pu32BpTable, 0, pstDp->u16BptCalibNum * sizeof(GK_U32));

    if (pu32BpTable == GK_NULL) {
        return GK_FAILURE;
    }

    memcpy(pu32BpTable, pstDp->pau32BptCalibTable[0], pstDp->au16BlkBpCalibNum[0] * sizeof(GK_U32));
    u16BpNum = pstDp->au16BlkBpCalibNum[0];

    for (k = 1; k < u8BlkNum; k++) {
        s32CntTemp = SortingDpCalibLut(pu32BpTable, pstDp->pau32BptCalibTable[k], u16BpNum, pstDp->au16BlkBpCalibNum[k]);

        if (s32CntTemp < 0) {
            ISP_FREE(pu32BpTable);
            return GK_FAILURE;
        }

        u16BpNum = s32CntTemp;
    }

    for (i = 0; i < pstDp->u16BptCalibNum; i++) {
        ext_system_dpc_calib_bpt_write(ViPipe, i, pu32BpTable[i]);
    }

    ISP_FREE(pu32BpTable);

    return GK_SUCCESS;
}

static GK_VOID Dpc_Calib_Success(VI_PIPE ViPipe, GK_U8 u8BlkNum, ISP_DEFECT_PIXEL_S *pstDp, ISP_STAT_S *pStatInfo)
{
    GK_U16 j;

    ISP_CTX_S *pstIspCtx  = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    S_ISPBE_REGS_TYPE *pstBeReg = GK_NULL;

    printf("trial: 0x%x, findshed: 0x%x\n", pstDp->u8TrialCount, pstDp->u16BptCalibNum);

    ext_system_dpc_bpt_calib_number_write(ViPipe, pstDp->u16BptCalibNum);

    if (IS_ONLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode)) {
        pstBeReg = (S_ISPBE_REGS_TYPE *)ISP_GetBeVirAddr(ViPipe, 0);
        ISP_CHECK_POINTER_VOID(pstBeReg);

        for ( j = 0; j < pstDp->u16BptCalibNum; j++) {
        }
    } else if (IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode)) {
        for (j = 0; j < pstDp->u16BptCalibNum; j++) {
            ext_system_dpc_calib_bpt_write(ViPipe, j, pStatInfo->stDpStat.au32DefectPixelLut[0][j]);
        }
    } else {
        MergingDpCalibLut(ViPipe, pstDp, u8BlkNum);
    }

    pstDp->u16DpccMode = (ISP_DPCC_MODE & 0x3dd) + (pstDp->bStaticEnable << 5) + (pstDp->bDynamicEnable << 1);
    pstDp->bStatEn     = 0;
    pstDp->bStaCalibrationEn = GK_FALSE;
    ext_system_dpc_static_calib_enable_write(ViPipe, GK_FALSE);
    ext_system_dpc_finish_thresh_write(ViPipe, pstDp->u8StaticDPThresh);
    ext_system_dpc_trigger_status_write(ViPipe, 0x1);
}

static GK_VOID SetReadDpStatisKey(VI_PIPE ViPipe, GK_BOOL bIsStart)
{
    GK_U32    u32IsrAccess;
    ISP_CTX_S *pstIspCtx   = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    if (IS_OFFLINE_MODE(pstIspCtx->stBlockAttr.enIspRunningMode) || \
        IS_STRIPING_MODE(pstIspCtx->stBlockAttr.enIspRunningMode)) {
        u32IsrAccess = ext_system_statistics_ctrlghbit_read(ViPipe);

        if (bIsStart == GK_TRUE) {
            u32IsrAccess |= (1 << DP_STAT_KEY_BIT);
        } else {
            u32IsrAccess &= (~(1 << DP_STAT_KEY_BIT));
        }

        ext_system_statistics_ctrlghbit_write(ViPipe, u32IsrAccess);
    }
}

static GK_VOID Dpc_Hot_Calib(VI_PIPE ViPipe, ISP_DEFECT_PIXEL_S  *pstDp, GK_U8 u8BlkNum, ISP_REG_CFG_S *pstRegCfg, ISP_STAT_S *pStatInfo)
{
    if (pstDp->u8FrameCnt < 9) {
        if (pstDp->u8FrameCnt == 1) {
            ext_system_dpc_trigger_status_write(ViPipe, ISP_STATE_INIT);
            DpEnter(ViPipe, pstDp);
        }

        pstDp->u8FrameCnt++;

        if (pstDp->u8FrameCnt == 3) {
            pstDp->u32DpccBadThresh = ((GK_U32)pstDp->u8StaticDPThresh << 24) + (((50 + 0x80 * pstDp->u8HotDevThresh) / 100) << 16) + 0x00000080;
            pstDp->u16DpccMode      = (ISP_DPCC_HOT_MODE & 0x3dd) + (pstDp->bStaticEnable << 5) + (pstDp->bDynamicEnable << 1);
            pstDp->bStatEn          = 1;

            SetReadDpStatisKey(ViPipe, GK_TRUE);
        }

        /* calibrate Frame 4 */
        if (pstDp->u8FrameCnt == 5) {
            pstDp->u16DpccMode = (ISP_DPCC_MODE & 0x3dd) + (pstDp->bStaticEnable << 5) + (pstDp->bDynamicEnable << 1);
            pstDp->bStatEn     = 0;
        }

        if (pstDp->u8FrameCnt == 6) {
            Dpc_Read_Calib_Num(ViPipe, u8BlkNum, pstDp, pStatInfo);
            SetReadDpStatisKey(ViPipe, GK_FALSE);

            if (pstDp->u8TrialCount >= pstDp->u8TrialCntLimit) { /* TIMEOUT */
                Dpc_Calib_TimeOut(ViPipe, pstDp);
                DpExit(ViPipe, pstDp, pstRegCfg);
            } else if (pstDp->u16BptCalibNum > pstDp->u16DpCountMax) {
                Dpc_Calib_Max(pstDp->u16BptCalibNum, pstDp);
                pstDp->u8StaticDPThresh++;
            } else if (pstDp->u16BptCalibNum < pstDp->u16DpCountMin) {
                Dpc_Calib_Min(pstDp->u16BptCalibNum, pstDp);
                if (pstDp->u8StaticDPThresh == 1 ) {
                    pstDp->u8TrialCount = pstDp->u8TrialCntLimit;
                }

                pstDp->u8StaticDPThresh--;
            } else { /* SUCCESS */
                Dpc_Calib_Success(ViPipe, u8BlkNum, pstDp, pStatInfo);
                DpExit(ViPipe, pstDp, pstRegCfg);
            }
        }
    }
}

static GK_VOID Dpc_Dark_Calib(VI_PIPE ViPipe, ISP_DEFECT_PIXEL_S  *pstDp, GK_U8 u8BlkNum, ISP_REG_CFG_S *pstRegCfg, ISP_STAT_S *pStatInfo)
{
    if (pstDp->u8FrameCnt < 9) {
        if (pstDp->u8FrameCnt == 1) {
            ext_system_dpc_trigger_status_write(ViPipe, ISP_STATE_INIT);
            pstDp->u8CalibStarted = 1;
            pstDp->u8StaticDPThresh = ext_system_dpc_start_thresh_read(ViPipe);
        }

        pstDp->u8FrameCnt++;

        if (pstDp->u8FrameCnt == 3) {
            pstDp->u32DpccBadThresh = 0xFF800000 + ((GK_U32)pstDp->u8StaticDPThresh << 8) + ((0x80 * pstDp->u8DeadDevThresh) / 100);
            pstDp->u16DpccMode      = (ISP_DPCC_DEAD_MODE & 0x3dd) + (pstDp->bStaticEnable << 5) + (pstDp->bDynamicEnable << 1);
            pstDp->bStatEn          = 1;

            SetReadDpStatisKey(ViPipe, GK_TRUE);
        }

        if (pstDp->u8FrameCnt == 5) {
            pstDp->u16DpccMode = (ISP_DPCC_MODE & 0x3dd) + (pstDp->bStaticEnable << 5) + (pstDp->bDynamicEnable << 1);
            pstDp->bStatEn     = 0;
        }

        if (pstDp->u8FrameCnt == 6) {
            Dpc_Read_Calib_Num(ViPipe, u8BlkNum, pstDp, pStatInfo);
            SetReadDpStatisKey(ViPipe, GK_FALSE);

            if (pstDp->u8TrialCount >= pstDp->u8TrialCntLimit) {
                Dpc_Calib_TimeOut(ViPipe, pstDp);
                DpInitialize(ViPipe);
                pstDp->u8CalibStarted  = 0;
                pstDp->u8CalibFinished = 1;
            } else if (pstDp->u16BptCalibNum > pstDp->u16DpCountMax) {
                Dpc_Calib_Max(pstDp->u16BptCalibNum, pstDp);
                pstDp->u8StaticDPThresh--;
            } else if (pstDp->u16BptCalibNum < pstDp->u16DpCountMin) {
                Dpc_Calib_Min(pstDp->u16BptCalibNum, pstDp);
                if (pstDp->u8StaticDPThresh == 255) {
                    pstDp->u8TrialCount = pstDp->u8TrialCntLimit;
                } else {
                    pstDp->u8StaticDPThresh++;
                }
            } else {
                Dpc_Calib_Success(ViPipe, u8BlkNum, pstDp, pStatInfo);
                DpInitialize(ViPipe);

                pstDp->u8CalibStarted  = 0;
                pstDp->u8CalibFinished = 1;
            }
        }
    }
}

static GK_VOID ISP_Dpc_StaticCalibration(VI_PIPE ViPipe, ISP_DEFECT_PIXEL_S  *pstDp, GK_U8 u8BlkNum, ISP_REG_CFG_S *pstRegCfg, ISP_STAT_S *pStatInfo)
{
    GK_U8 i;
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    if ((pstDp->u8PixelDetectType != 1) && (pstDp->u8PixelDetectType != 0)) {
        ISP_TRACE(MODULE_DBG_ERR, "invalid static defect pixel detect type!\n");
        return;
    }

    if ((pstDp->u8CalibStarted == 0) && (u8BlkNum > 1)) {
        for (i = 0; i < u8BlkNum; i++) {
            pstDp->pau32BptCalibTable[i] = (GK_U32 *)ISP_MALLOC(STATIC_DP_COUNT_NORMAL * sizeof(GK_U32));

            if (pstDp->pau32BptCalibTable[i] == GK_NULL) {
                ISP_TRACE(MODULE_DBG_ERR, "malloc dpc calibration table buffer failed\n");
                Dpc_SafeFreeCalibLut(pstDp, u8BlkNum);
                return;
            }
        }
    }

    pstIspCtx->stLinkage.bDefectPixel = GK_TRUE;

    if (pstDp->u8PixelDetectType == 0) {
        Dpc_Hot_Calib(ViPipe, pstDp, u8BlkNum, pstRegCfg, pStatInfo);
        return;
    }

    if (pstDp->u8PixelDetectType == 1) {
        Dpc_Dark_Calib(ViPipe, pstDp, u8BlkNum, pstRegCfg, pStatInfo);
        return;
    }
}

static GK_VOID ISP_Dpc_Calib_Mode(VI_PIPE ViPipe, ISP_DEFECT_PIXEL_S *pstDp, ISP_REG_CFG_S *pstRegCfg, ISP_STAT_S *pStatInfo)
{
    GK_U8 i;

    DpReadStaticCalibExtregs(ViPipe);
    ISP_Dpc_StaticCalibration(ViPipe, pstDp, pstRegCfg->u8CfgNum, pstRegCfg, pStatInfo);

    for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
        pstRegCfg->stAlgRegCfg[i].stDpRegCfg.stDynaRegCfg.u32DpccBadThresh  = pstDp->u32DpccBadThresh;
        pstRegCfg->stAlgRegCfg[i].stDpRegCfg.stDynaRegCfg.u16DpccMode       = pstDp->u16DpccMode;
        pstRegCfg->stAlgRegCfg[i].stDpRegCfg.stDynaRegCfg.bDpcStatEn        = pstDp->bStatEn;
        pstRegCfg->stAlgRegCfg[i].stDpRegCfg.stDynaRegCfg.bResh             = GK_TRUE;
    }
}
#endif

static GK_VOID DpReadExtregs(VI_PIPE ViPipe)
{
    GK_U16 i;
    ISP_DEFECT_PIXEL_S *pstDp = GK_NULL;

    DP_GET_CTX(ViPipe, pstDp);

#ifdef CONFIG_ISP_DPC_STATIC_TABLE_SUPPORT
    pstDp->bStaticAttrUpdate    = ext_system_dpc_static_attr_update_read(ViPipe);
    ext_system_dpc_static_attr_update_write(ViPipe, GK_FALSE);

    if (pstDp->bStaticAttrUpdate) {
        pstDp->u16BptCorNum         = ext_system_dpc_bpt_cor_number_read(ViPipe);
        pstDp->bStaicShow           = ext_system_dpc_static_dp_show_read(ViPipe);
    }
#endif

    pstDp->bDynaAttrUpdateEn = ext_system_dpc_dynamic_attr_update_read(ViPipe);
    ext_system_dpc_dynamic_attr_update_write(ViPipe, GK_FALSE);

    if (pstDp->bDynaAttrUpdateEn) {
        pstDp->bDynamicManual       = ext_system_dpc_dynamic_manual_enable_read(ViPipe);

        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
            pstDp->stCmosDpc.au16Strength[i]    = ext_system_dpc_dynamic_strength_table_read(ViPipe, i);
            pstDp->stCmosDpc.au16BlendRatio[i]  = ext_system_dpc_dynamic_blend_ratio_table_read(ViPipe, i);
        }

        pstDp->u16BlendRatio        = ext_system_dpc_dynamic_blend_ratio_read(ViPipe);
        pstDp->u16Strength          = ext_system_dpc_dynamic_strength_read(ViPipe);
        pstDp->bSupTwinkleEn        = ext_system_dpc_suppress_twinkle_enable_read(ViPipe);
        pstDp->s8SupTwinkleThr      = ext_system_dpc_suppress_twinkle_thr_read(ViPipe);
        pstDp->u8SupTwinkleSlope    = ext_system_dpc_suppress_twinkle_slope_read(ViPipe);
    }
}

#ifdef CONFIG_ISP_DPC_STATIC_TABLE_SUPPORT
static GK_VOID  SplitDpCorLut(VI_PIPE ViPipe, ISP_DEFECT_PIXEL_S  *pstDp, ISP_REG_CFG_S *pstRegCfg, GK_U8 u8BlkNum)
{
    GK_S8  j;
    GK_U16 au16BptNum[ISP_STRIPING_MAX_NUM] = {0};
    GK_U16 i;
    GK_U16 u16XValue;
    GK_U32 u32BptValue;

    for (j = 0; j < u8BlkNum; j++) {
        memset(pstRegCfg->stAlgRegCfg[j].stDpRegCfg.stUsrRegCfg.stUsrStaCorRegCfg.au32DpccBpTable, 0, STATIC_DP_COUNT_NORMAL * sizeof(GK_U32));
    }

    for (j = (GK_S8)u8BlkNum - 1; j >= 0; j--) {
        for (i = 0; i < pstDp->u16BptCorNum; i++) {
            u32BptValue = ext_system_dpc_cor_bpt_read(ViPipe, i);

            u16XValue = u32BptValue & 0x1FFF;

            if ((u16XValue >= (pstDp->au16OffsetForSplit[j])) && (u16XValue < pstDp->au16OffsetForSplit[j + 1])) {
                pstRegCfg->stAlgRegCfg[j].stDpRegCfg.stUsrRegCfg.stUsrStaCorRegCfg.au32DpccBpTable[au16BptNum[j]] = u32BptValue - pstDp->au16Offset[j];
                au16BptNum[j]++;

                if (au16BptNum[j] >= STATIC_DP_COUNT_NORMAL) {
                    break;
                }
            }
        }
    }

    for (j = 0; j < (GK_S8)u8BlkNum; j++) {
        pstRegCfg->stAlgRegCfg[j].stDpRegCfg.stUsrRegCfg.stUsrStaCorRegCfg.u16DpccBptNumber = au16BptNum[j];
        pstRegCfg->stAlgRegCfg[j].stDpRegCfg.stUsrRegCfg.stUsrStaCorRegCfg.bResh            = GK_TRUE;
        pstRegCfg->stAlgRegCfg[j].stDpRegCfg.stUsrRegCfg.stUsrStaCorRegCfg.u32UpdateIndex  += 1;
    }

    return;
}
#endif

static GK_S32 CalcRakeRatio(GK_S32 x0, GK_S32 y0, GK_S32 x1, GK_S32 y1, GK_S32 shift)
{
    if (x0 == x1) {
        return 0;
    } else {
        return ((y1 - y0) * (1 << shift)) / DIV_0_TO_1(x1 - x0);
    }
}

static GK_VOID ISP_Dpc_Usr_Cfg(VI_PIPE ViPipe, ISP_DEFECT_PIXEL_S *pstDp, ISP_REG_CFG_S *pstRegCfg)
{
    GK_U8 i;
    ISP_DPC_USR_DYNA_COR_CFG_S *pstUsrDynaCorRegCfg = GK_NULL;

#ifdef CONFIG_ISP_DPC_STATIC_TABLE_SUPPORT
    if (pstDp->bStaticAttrUpdate) {
        if (pstDp->bStaicShow || pstDp->bStaticEnable) {
            SplitDpCorLut(ViPipe, pstDp, pstRegCfg, pstRegCfg->u8CfgNum);
        }
    }
#endif

    for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
        pstUsrDynaCorRegCfg = &pstRegCfg->stAlgRegCfg[i].stDpRegCfg.stUsrRegCfg.stUsrDynaCorRegCfg;

        pstUsrDynaCorRegCfg->bDpccHardThrEn         = pstDp->bSupTwinkleEnAdapt ? (GK_FALSE) : (GK_TRUE);
        pstUsrDynaCorRegCfg->s8DpccSupTwinkleThrMax = CLIP3(pstDp->s8SupTwinkleThr, -128, 127);
        pstUsrDynaCorRegCfg->s8DpccSupTwinkleThrMin = CLIP3(pstUsrDynaCorRegCfg->s8DpccSupTwinkleThrMax - pstDp->u8SupTwinkleSlope, -128, 127);
        pstUsrDynaCorRegCfg->u16DpccRakeRatio       = CalcRakeRatio(pstUsrDynaCorRegCfg->s8DpccSupTwinkleThrMin, 0, pstUsrDynaCorRegCfg->s8DpccSupTwinkleThrMax, 128, 2);
        pstUsrDynaCorRegCfg->bResh                  = GK_TRUE;
    }
}

static GK_VOID DpcResSwitch(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfg)
{
    ISP_DEFECT_PIXEL_S  *pstDp     = GK_NULL;

    DP_GET_CTX(ViPipe, pstDp);
    ISP_CHECK_POINTER_VOID(pstDp);

#ifdef CONFIG_ISP_DPC_STATIC_TABLE_SUPPORT
    DpcImageSize(ViPipe, pstDp);

    pstDp->bStaticAttrUpdate = GK_TRUE;
#endif

    ISP_Dpc_Usr_Cfg(ViPipe, pstDp, pstRegCfg);
}

#ifdef CONFIG_ISP_DPC_STATIC_TABLE_SUPPORT
static GK_VOID ISP_Dpc_Show_Mode(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfg, ISP_CTX_S *pstIspCtx)
{
    GK_U8 i;

    pstIspCtx->stLinkage.bDefectPixel = GK_FALSE;

    for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
        pstRegCfg->stAlgRegCfg[i].stDpRegCfg.stDynaRegCfg.u16DpccMode = ISP_DPCC_HIGHLIGHT_MODE;
        pstRegCfg->stAlgRegCfg[i].stDpRegCfg.stDynaRegCfg.bResh       = GK_TRUE;
    }
}
#endif

static GK_VOID soft_inter(ISP_DPC_DYNA_CFG_S *pstDpcHwCfg, GK_U8 u8DpccStat)
{
    GK_U8  u8StatIdxUp, u8StatIdxLow;
    GK_U8  u8StatUpper, u8StatLower;
    GK_U8  u8DpccLineThrRb1, u8DpccLineThrG1, u8DpccLineMadFacRb1, u8DpccLineMadFacG1;
    GK_U8  i;

    u8StatIdxUp = ISP_DPC_SOFT_SLOPE_GRADE - 1;
    for (i = 0; i < ISP_DPC_SOFT_SLOPE_GRADE; i++) {
        if (u8DpccStat < g_au8SoftSlopeGrade[i]) {
            u8StatIdxUp = i;
            break;
        }
    }
    u8StatIdxLow = MAX2((GK_S8)u8StatIdxUp - 1, 0);

    u8StatUpper = g_au8SoftSlopeGrade[u8StatIdxUp];
    u8StatLower = g_au8SoftSlopeGrade[u8StatIdxLow];

    u8DpccLineThrRb1    = (GK_U8)LinearInter(u8DpccStat, \
                                             u8StatLower, (g_au16SoftLineThr[u8StatIdxLow] & 0xFF00) >> 8, \
                                             u8StatUpper, (g_au16SoftLineThr[u8StatIdxUp]  & 0xFF00) >> 8);
    u8DpccLineThrG1     = (GK_U8)LinearInter(u8DpccStat, \
                                             u8StatLower, g_au16SoftLineThr[u8StatIdxLow] & 0xFF, \
                                             u8StatUpper, g_au16SoftLineThr[u8StatIdxUp]  & 0xFF);

    u8DpccLineMadFacRb1 = (GK_U8)LinearInter(u8DpccStat, \
                                             u8StatLower, (g_au16SoftLineMadFac[u8StatIdxLow] & 0xFF00) >> 8, \
                                             u8StatUpper, (g_au16SoftLineMadFac[u8StatIdxUp]  & 0xFF00) >> 8);
    u8DpccLineMadFacG1  = (GK_U8)LinearInter(u8DpccStat, \
                                             u8StatLower, g_au16SoftLineMadFac[u8StatIdxLow] & 0xFF, \
                                             u8StatUpper, g_au16SoftLineMadFac[u8StatIdxUp] & 0xFF);
    pstDpcHwCfg->au16DpccLineThr[0]    = (((GK_U16)u8DpccLineThrRb1) << 8) + u8DpccLineThrG1;
    pstDpcHwCfg->au16DpccLineMadFac[0] = (((GK_U16)(u8DpccLineMadFacRb1 & 0x3F)) << 8) + (u8DpccLineMadFacG1 & 0x3F);
}

static GK_VOID set_dpcc_parameters_inter(ISP_DPC_DYNA_CFG_S *pstIspDpccHwCfg, GK_U8 u8DpccStat)
{
    GK_U8 i, j;
    GK_U8 u8StatUpper, u8StatLower;
    GK_U8 u8StatIdxUp, u8StatIdxLow;
    ISP_DPCC_DERIVED_PARAM_S stDpcDerParam;

    u8StatIdxUp = ISP_DPC_SLOPE_GRADE - 1;
    for (i = 0; i < ISP_DPC_SLOPE_GRADE; i++) {
        if (u8DpccStat < g_au8SlopeGrade[i]) {
            u8StatIdxUp = i;
            break;
        }
    }
    u8StatIdxLow = MAX2((GK_S8)u8StatIdxUp - 1, 0);

    u8StatUpper = g_au8SlopeGrade[u8StatIdxUp];
    u8StatLower = g_au8SlopeGrade[u8StatIdxLow];

    pstIspDpccHwCfg->u8DpccSetUse       = g_stDpcDefCfg[u8StatIdxLow].u8DpccSetUse;
    pstIspDpccHwCfg->u16DpccMethodsSet1 = g_stDpcDefCfg[u8StatIdxLow].u16DpccMethodsSet1;
    pstIspDpccHwCfg->u16DpccMethodsSet2 = g_stDpcDefCfg[u8StatIdxLow].u16DpccMethodsSet2;
    pstIspDpccHwCfg->u16DpccMethodsSet3 = g_stDpcDefCfg[u8StatIdxLow].u16DpccMethodsSet3;
    pstIspDpccHwCfg->u32DpccBadThresh   = g_stDpcDefCfg[u8StatIdxLow].u32DpccBadThresh;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            stDpcDerParam.au8DpccLineThr[i][j]    = (GK_U8)LinearInter(u8DpccStat, \
                                                                       u8StatLower, g_stDpcDerParam[u8StatIdxLow].au8DpccLineThr[i][j], \
                                                                       u8StatUpper, g_stDpcDerParam[u8StatIdxUp].au8DpccLineThr[i][j]);
            stDpcDerParam.au8DpccLineMadFac[i][j] = (GK_U8)LinearInter(u8DpccStat, \
                                                                       u8StatLower, g_stDpcDerParam[u8StatIdxLow].au8DpccLineMadFac[i][j], \
                                                                       u8StatUpper, g_stDpcDerParam[u8StatIdxUp].au8DpccLineMadFac[i][j]);
            stDpcDerParam.au8DpccPgFac[i][j]      = (GK_U8)LinearInter(u8DpccStat, \
                                                                       u8StatLower, g_stDpcDerParam[u8StatIdxLow].au8DpccPgFac[i][j], \
                                                                       u8StatUpper, g_stDpcDerParam[u8StatIdxUp].au8DpccPgFac[i][j]);
            stDpcDerParam.au8DpccRgFac[i][j]      = (GK_U8)LinearInter(u8DpccStat, \
                                                                       u8StatLower, g_stDpcDerParam[u8StatIdxLow].au8DpccRgFac[i][j], \
                                                                       u8StatUpper, g_stDpcDerParam[u8StatIdxUp].au8DpccRgFac[i][j]);
            stDpcDerParam.au8DpccRndThr[i][j]     = (GK_U8)LinearInter(u8DpccStat, \
                                                                       u8StatLower, g_stDpcDerParam[u8StatIdxLow].au8DpccRndThr[i][j], \
                                                                       u8StatUpper, g_stDpcDerParam[u8StatIdxUp].au8DpccRndThr[i][j]);
            stDpcDerParam.au8DpccRndOffs[i][j]    = (GK_U8)LinearInter(u8DpccStat, \
                                                                       u8StatLower, g_stDpcDerParam[u8StatIdxLow].au8DpccRndOffs[i][j], \
                                                                       u8StatUpper, g_stDpcDerParam[u8StatIdxUp].au8DpccRndOffs[i][j]);
            stDpcDerParam.au8DpccRo[i][j]         = (GK_U8)LinearInter(u8DpccStat, \
                                                                       u8StatLower, g_stDpcDerParam[u8StatIdxLow].au8DpccRo[i][j], \
                                                                       u8StatUpper, g_stDpcDerParam[u8StatIdxUp].au8DpccRo[i][j]);

            stDpcDerParam.au16DpccRndThrMtp[i][j] = stDpcDerParam.au8DpccRndThr[i][j] * 3;
            stDpcDerParam.au16DpccRgFacMtp[i][j]  = stDpcDerParam.au8DpccRgFac[i][j] * 11;
        }
    }

    for (j = 0; j < 3; j++) {
        pstIspDpccHwCfg->au16DpccLineThr[j]    = ((GK_U16)(stDpcDerParam.au8DpccLineThr[0][j]) << 8) + (stDpcDerParam.au8DpccLineThr[1][j]);
        pstIspDpccHwCfg->au16DpccLineMadFac[j] = ((GK_U16)(stDpcDerParam.au8DpccLineMadFac[0][j] & 0x3F) << 8) + (stDpcDerParam.au8DpccLineMadFac[1][j] & 0x3F);
        pstIspDpccHwCfg->au16DpccPgFac[j]      = ((GK_U16)(stDpcDerParam.au8DpccPgFac[0][j]      & 0x3F) << 8) + (stDpcDerParam.au8DpccPgFac[1][j]      & 0x3F);
        pstIspDpccHwCfg->au16DpccRndThr[j]     = ((GK_U16)(stDpcDerParam.au8DpccRndThr[0][j]) << 8) + (stDpcDerParam.au8DpccRndThr[1][j]);
        pstIspDpccHwCfg->au16DpccRgFac[j]      = ((GK_U16)(stDpcDerParam.au8DpccRgFac[0][j]      & 0x3F) << 8) + (stDpcDerParam.au8DpccRgFac[1][j]     & 0x3F);
        pstIspDpccHwCfg->au32DpccRndThrMtp[j]  = ((GK_U16)(stDpcDerParam.au16DpccRndThrMtp[0][j] & 0x3FF) << 10) + (stDpcDerParam.au16DpccRndThrMtp[1][j] & 0x3FF);
        pstIspDpccHwCfg->au32DpccRgFacMtp[j]   = ((GK_U16)(stDpcDerParam.au16DpccRgFacMtp[0][j]  & 0x3FF) << 10) + (stDpcDerParam.au16DpccRgFacMtp[1][j]  & 0x3FF);
    }

    pstIspDpccHwCfg->u16DpccRoLimits    = ((GK_U16)(stDpcDerParam.au8DpccRo[0][2] & 0x3) << 10) + ((GK_U16)(stDpcDerParam.au8DpccRo[1][2] & 0x3) << 8) + \
                                          ((stDpcDerParam.au8DpccRo[0][1] & 0x3) << 6) + ((stDpcDerParam.au8DpccRo[1][1] & 0x3) << 4) + \
                                          ((stDpcDerParam.au8DpccRo[0][0] & 0x3) << 2) + (stDpcDerParam.au8DpccRo[1][0] & 0x3);

    pstIspDpccHwCfg->u16DpccRndOffs     = ((GK_U16)(stDpcDerParam.au8DpccRndOffs[0][2] & 0x3) << 10) + ((GK_U16)(stDpcDerParam.au8DpccRndOffs[1][2] & 0x3) << 8) + \
                                          ((stDpcDerParam.au8DpccRndOffs[0][1] & 0x3) << 6) + ((stDpcDerParam.au8DpccRndOffs[1][1] & 0x3) << 4) + \
                                          ((stDpcDerParam.au8DpccRndOffs[0][0] & 0x3) << 2) + (stDpcDerParam.au8DpccRndOffs[1][0] & 0x3);
}

static GK_S32 ISP_Dynamic_set(GK_U32 u32Iso, ISP_DPC_DYNA_CFG_S *pstDpcHwCfg, ISP_DEFECT_PIXEL_S *pstDpcFwCfg)
{
    GK_U8  u8Alpha0RB = 0;
    GK_U8  u8Alpha0G  = 0;  /* the blend ratio of 5 & 9 */
    GK_U8  u8Alpha1RB = 0;
    GK_U8  u8Alpha1G  = 0;  /* the blend ratio of input data and filtered result */
    GK_U8  u8DpccStat;
    GK_U8  u8IsoIndexUpper, u8IsoIndexLower;
    GK_U16 u16BlendRatio = 0x0;
    GK_U16 u16Strength   = 0;
    GK_U16 u16DpccMode   = pstDpcFwCfg->u16DpccMode;
    ISP_CMOS_DPC_S *pstDpc = &pstDpcFwCfg->stCmosDpc;
    ISP_DPCC_DERIVED_PARAM_S stDpcDerParam;

    if (pstDpcFwCfg->bDynamicManual) {
        u16Strength = pstDpcFwCfg->u16Strength;
        u16BlendRatio = pstDpcFwCfg->u16BlendRatio;
    } else {
        u8IsoIndexUpper = GetIsoIndex(u32Iso);
        u8IsoIndexLower = MAX2((GK_S8)u8IsoIndexUpper - 1, 0);

        u16Strength     = (GK_U16)LinearInter(u32Iso, \
                                              g_au32IsoLut[u8IsoIndexLower], (GK_S32)pstDpc->au16Strength[u8IsoIndexLower], \
                                              g_au32IsoLut[u8IsoIndexUpper], (GK_S32)pstDpc->au16Strength[u8IsoIndexUpper]);
        u16BlendRatio   = (GK_U16)LinearInter(u32Iso, \
                                              g_au32IsoLut[u8IsoIndexLower], (GK_S32)pstDpc->au16BlendRatio[u8IsoIndexLower], \
                                              g_au32IsoLut[u8IsoIndexUpper], (GK_S32)pstDpc->au16BlendRatio[u8IsoIndexUpper]);
    }

    u8DpccStat = u16Strength >> 1;
    set_dpcc_parameters_inter(pstDpcHwCfg, u8DpccStat);
    if (u8DpccStat == 0) {
        u16DpccMode &= 0xFFFC;
    } else if (u8DpccStat > 100) {
        pstDpcHwCfg->u8DpccSetUse          = 0x7;
        pstDpcHwCfg->u16DpccMethodsSet1    = 0x1f1f;
        pstDpcHwCfg->au16DpccLineThr[0]    = g_stDpcHighDerParam[u8DpccStat - 101].u16DpcLineThr1;
        pstDpcHwCfg->au16DpccLineMadFac[0] = g_stDpcHighDerParam[u8DpccStat - 101].u16DpcLineMadFac1;
        pstDpcHwCfg->au16DpccPgFac[0]      = g_stDpcHighDerParam[u8DpccStat - 101].u16DpcPgFac1;
        pstDpcHwCfg->au16DpccRndThr[0]     = g_stDpcHighDerParam[u8DpccStat - 101].u16DpcRndThr1;
        pstDpcHwCfg->au16DpccRgFac[0]      = g_stDpcHighDerParam[u8DpccStat - 101].u16DpcRgFac1;
        pstDpcHwCfg->u16DpccRoLimits       = g_stDpcHighDerParam[u8DpccStat - 101].u16DpcRoLimits1;
        pstDpcHwCfg->u16DpccRndOffs        = g_stDpcHighDerParam[u8DpccStat - 101].u16DpcRndOffs1;

        stDpcDerParam.au8DpccRndThr[0][0]      = ((pstDpcHwCfg->au16DpccRndThr[0] & 0x0000FF00) >> 8);
        stDpcDerParam.au8DpccRndThr[1][0]      = (pstDpcHwCfg->au16DpccRndThr[0] & 0x000000FF);

        stDpcDerParam.au8DpccRgFac[0][0]       = ((pstDpcHwCfg->au16DpccRgFac[0] & 0x00003F00) >> 8);
        stDpcDerParam.au8DpccRgFac[1][0]       = (pstDpcHwCfg->au16DpccRgFac[0] & 0x0000003F);

        stDpcDerParam.au16DpccRndThrMtp[0][0]  = stDpcDerParam.au8DpccRndThr[0][0] * 3;
        stDpcDerParam.au16DpccRndThrMtp[1][0]  = stDpcDerParam.au8DpccRndThr[1][0] * 3;

        stDpcDerParam.au16DpccRgFacMtp[0][0]   =  stDpcDerParam.au8DpccRgFac[0][0] * 11;
        stDpcDerParam.au16DpccRgFacMtp[1][0]   =  stDpcDerParam.au8DpccRgFac[1][0] * 11;

        pstDpcHwCfg->au32DpccRndThrMtp[0]      = ((GK_U16)(stDpcDerParam.au16DpccRndThrMtp[0][0] & 0x3FF) << 10) + (stDpcDerParam.au16DpccRndThrMtp[1][0] & 0x3FF);
        pstDpcHwCfg->au32DpccRgFacMtp[0]       = ((GK_U16)(stDpcDerParam.au16DpccRgFacMtp[0][0]  & 0x3FF) << 10) + (stDpcDerParam.au16DpccRgFacMtp[1][0]  & 0x3FF);

    }

    pstDpcFwCfg->bSupTwinkleEnAdapt = pstDpcFwCfg->bSupTwinkleEn;
    if (pstDpcFwCfg->bSupTwinkleEn) {
        if ((u8DpccStat == 0) || !((pstDpcFwCfg->u16DpccMode & 0x2) >> 1)) {
            pstDpcFwCfg->bSupTwinkleEnAdapt = 0;
        } else {
            soft_inter(pstDpcHwCfg, u8DpccStat);
        }
    }

    if (!((u16DpccMode & 0x2) >> 1)) {
        u16BlendRatio = 0;
    }
    u8Alpha0RB = (u16BlendRatio > 0x80) ? (u16BlendRatio - 0x80) : 0x0;
    u8Alpha1RB = (u16BlendRatio > 0x80) ? 0x80 : u16BlendRatio;
    pstDpcHwCfg->u32DpccAlpha  = ((GK_U32)u8Alpha0RB << 24) + (u8Alpha0G << 16) + (u8Alpha1RB << 8) + u8Alpha1G;
    pstDpcHwCfg->u16DpccMode   = u16DpccMode;

    pstDpcFwCfg->u16ProcInfoBlendRatio = u16BlendRatio;
    pstDpcFwCfg->u16ProcInfoStrength = u16Strength;
    return GK_SUCCESS;
}

static GK_S32 ISP_Dpc_Normal_Mode(VI_PIPE ViPipe, ISP_DEFECT_PIXEL_S *pstDp, ISP_REG_CFG_S *pstRegCfg, ISP_CTX_S *pstIspCtx)
{
    GK_U8 i;

    pstIspCtx->stLinkage.bDefectPixel = GK_FALSE;

    pstDp->u16DpccMode = (ISP_DPCC_MODE & 0x3dd) + (pstDp->bStaticEnable << 5) + (pstDp->bDynamicEnable << 1);

    for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
        ISP_Dynamic_set(pstIspCtx->stLinkage.u32Iso, &pstRegCfg->stAlgRegCfg[i].stDpRegCfg.stDynaRegCfg, pstDp);
        pstRegCfg->stAlgRegCfg[i].stDpRegCfg.stDynaRegCfg.bResh = GK_TRUE;
    }

    return GK_SUCCESS;
}

static GK_VOID ISP_DpWdrModeSet(VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
    ISP_REG_CFG_S *pstRegCfg  = (ISP_REG_CFG_S *)pRegCfg;
    ISP_CTX_S     *pstIspCtx  = GK_NULL;
    ISP_DEFECT_PIXEL_S  *pstDp = GK_NULL;
    ISP_CMOS_DEFAULT_S  *pstSnsDft  = GK_NULL;
    ISP_DPC_USR_CFG_S *pstUsrRegCfg;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    DP_GET_CTX(ViPipe, pstDp);
    ISP_SensorGetDefault(ViPipe, &pstSnsDft);

    ISP_DpEnableCfg(ViPipe, pstRegCfg->u8CfgNum, pstRegCfg);

    if (pstIspCtx->stBlockAttr.u8BlockNum != pstIspCtx->stBlockAttr.u8PreBlockNum) {
        DpcResSwitch(ViPipe, pstRegCfg);
    }

    pstUsrRegCfg = &pstRegCfg->stAlgRegCfg[0].stDpRegCfg.stUsrRegCfg;

    if (pstSnsDft->unKey.bit1Dpc) {
        GK_S32 s32Ret = GK_FAILURE;

        if (pstSnsDft->pstDpc != GK_NULL) {
            s32Ret = DpcCheckCmosParam(ViPipe, pstSnsDft->pstDpc);
        }
        if (s32Ret == GK_SUCCESS) {
            memcpy(&pstDp->stCmosDpc, pstSnsDft->pstDpc, sizeof(ISP_CMOS_DPC_S));
            pstUsrRegCfg->stUsrDynaCorRegCfg.u8DPCPos = pstDp->stCmosDpc.u8DpcPos;
            pstUsrRegCfg->stUsrDynaCorRegCfg.bResh = GK_TRUE;
        }
    }

    if (!IS_LINEAR_MODE(pstIspCtx->stWdrAttr.enWDRMode)) {
        pstUsrRegCfg->stUsrDynaCorRegCfg.u8DPCPos = ISP_DPC_DEFAULT_POS_WDR;
        pstUsrRegCfg->stUsrDynaCorRegCfg.bResh = GK_TRUE;
    }

    pstRegCfg->unKey.bit1DpCfg = 1;
}

GK_S32 ISP_DpInit(VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
    GK_S32 s32Ret = GK_SUCCESS;
    ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;

    s32Ret = DpCtxInit(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        return s32Ret;
    }

    s32Ret = DpInitialize(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        return s32Ret;
    }

    DpRegsInitialize(ViPipe, pstRegCfg);
    DpExtRegsInitialize(ViPipe);

    return GK_SUCCESS;
}

GK_S32 ISP_DpRun(VI_PIPE ViPipe, const GK_VOID *pStatInfo,
                 GK_VOID *pRegCfg, GK_S32 s32Rsv)
{
    ISP_CTX_S           *pstIspCtx  = GK_NULL;
    ISP_DEFECT_PIXEL_S  *pstDp      = GK_NULL;
    ISP_REG_CFG_S       *pstRegCfg  = (ISP_REG_CFG_S *)pRegCfg;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    DP_GET_CTX(ViPipe, pstDp);
    ISP_CHECK_POINTER(pstDp);

    if (pstIspCtx->stLinkage.bStatReady == GK_FALSE) {
        return GK_SUCCESS;
    }

    pstRegCfg->unKey.bit1DpCfg = 1;

#ifdef CONFIG_ISP_DPC_STATIC_TABLE_SUPPORT
    pstDp->bStaCalibrationEn = ext_system_dpc_static_calib_enable_read(ViPipe);
    pstDp->bStaticEnable     = ext_system_dpc_static_cor_enable_read(ViPipe);
#else
    pstDp->bStaticEnable = 0;
#endif
    pstDp->bDynamicEnable    = ext_system_dpc_dynamic_cor_enable_read(ViPipe);

#ifdef CONFIG_ISP_DPC_STATIC_TABLE_SUPPORT
    if ((!pstDp->bStaCalibrationEn) && (pstDp->u8CalibStarted == 1)) {
        DpExit(ViPipe, pstDp, pstRegCfg);
        Dpc_SafeFreeCalibLut(pstDp, pstRegCfg->u8CfgNum);
    }

    if (pstDp->bStaCalibrationEn) { /* calibration mode */
        ISP_Dpc_Calib_Mode(ViPipe, pstDp, pstRegCfg, (ISP_STAT_S *)pStatInfo);

        return GK_SUCCESS;
    }

    if (pstDp->u8CalibFinished == 1) {
        Dpc_SafeFreeCalibLut(pstDp, pstRegCfg->u8CfgNum);
    }
#endif

    DpReadExtregs(ViPipe);

#ifdef CONFIG_ISP_DPC_STATIC_TABLE_SUPPORT
    if (pstDp->bStaicShow) { /* highlight static defect pixels mode */
        ISP_Dpc_Show_Mode(ViPipe, pstRegCfg, pstIspCtx);
    } else /* normal detection and correction mode */
#endif
    {
        ISP_Dpc_Normal_Mode(ViPipe, pstDp, pstRegCfg, pstIspCtx);
    }

    ISP_Dpc_Usr_Cfg(ViPipe, pstDp, pstRegCfg);

    return GK_SUCCESS;
}


GK_S32 DpcProcWrite(VI_PIPE ViPipe, ISP_CTRL_PROC_WRITE_S *pstProc)
{
    ISP_CTRL_PROC_WRITE_S stProcTmp;
    ISP_DEFECT_PIXEL_S  *pstDp = GK_NULL;

    DP_GET_CTX(ViPipe, pstDp);
    ISP_CHECK_POINTER(pstDp);

    if ((pstProc->pcProcBuff == GK_NULL)
        || (pstProc->u32BuffLen == 0)) {
        return GK_FAILURE;
    }

    stProcTmp.pcProcBuff = pstProc->pcProcBuff;
    stProcTmp.u32BuffLen = pstProc->u32BuffLen;

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
                    "-----DEFECT INFO-----------------------------------\n");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
                    "%12s" "%12s" "%12s\n",
                    "Enable", "Strength", "BlendRatio");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
                    "%12u" "%12u"  "%12u\n",
                    pstDp->bDynamicEnable, pstDp->u16ProcInfoStrength, pstDp->u16ProcInfoBlendRatio);

    pstProc->u32WriteLen += 1;

    return GK_SUCCESS;
}

GK_S32 ISP_DpCtrl(VI_PIPE ViPipe, GK_U32 u32Cmd, GK_VOID *pValue)
{
    ISP_REGCFG_S        *pRegCfg   = GK_NULL;

    switch (u32Cmd) {
        case ISP_WDR_MODE_SET:
            ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
            ISP_CHECK_POINTER(pRegCfg);
            ISP_DpWdrModeSet(ViPipe, (GK_VOID *)&pRegCfg->stRegCfg);
            break;
        case ISP_CHANGE_IMAGE_MODE_SET:
            ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
            ISP_CHECK_POINTER(pRegCfg);
            DpcResSwitch(ViPipe, &pRegCfg->stRegCfg);
            break;
        case ISP_PROC_WRITE:
            DpcProcWrite(ViPipe, (ISP_CTRL_PROC_WRITE_S *)pValue);
            break;

        default :
            break;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_DpExit(VI_PIPE ViPipe)
{
    GK_U8 i;
    ISP_REGCFG_S  *pRegCfg   = GK_NULL;
    ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

    for (i = 0; i < pRegCfg->stRegCfg.u8CfgNum; i++) {
        pRegCfg->stRegCfg.stAlgRegCfg[i].stDpRegCfg.abDpcEn[0] = GK_FALSE;
        pRegCfg->stRegCfg.stAlgRegCfg[i].stDpRegCfg.abDpcEn[1] = GK_FALSE;
        pRegCfg->stRegCfg.stAlgRegCfg[i].stDpRegCfg.abDpcEn[2] = GK_FALSE;
        pRegCfg->stRegCfg.stAlgRegCfg[i].stDpRegCfg.abDpcEn[3] = GK_FALSE;
    }

    pRegCfg->stRegCfg.unKey.bit1DpCfg = 1;

    DpCtxExit(ViPipe);

    return GK_SUCCESS;
}

GK_S32 ISP_AlgRegisterDp(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_ALG_NODE_S *pstAlgs = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_ALG_CHECK(pstIspCtx->unAlgKey.bit1Dp);
    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_DP;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_DpInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_DpRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_DpCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_DpExit;
    pstAlgs->bUsed = GK_TRUE;

    return GK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
