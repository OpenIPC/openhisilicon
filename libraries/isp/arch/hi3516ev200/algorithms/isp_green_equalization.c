/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <math.h>
#include "isp_alg.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_sensor.h"
#include "isp_math_utils.h"
#include "isp_proc.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define ISP_MINIISP_BITDEPTH        (16)
#define ISP_GE_SLOPE_DEFAULT       (13)
#define ISP_GE_SENSI_SLOPE_DEFAULT (13)
#define ISP_GE_SENSI_THR_DEFAULT   (4800)


static const  GK_U16 g_au16Threshold[ISP_AUTO_ISO_STRENGTH_NUM] = { 4800, 4800, 4800, 4800, 4960, 4960, 4960, 4960, 5120, 5120, 5120, 5120, 5280, 5280, 5280, 5280};
static const  GK_U16 g_au16Strength[ISP_AUTO_ISO_STRENGTH_NUM]  = { 128,  128,  128,  128,  129,  129,  129,  129,  130,  130,  130,  130,  131,  131,  131,  131};

static const  GK_U16 g_au16NpOffset[ISP_AUTO_ISO_STRENGTH_NUM]  = {16384, 16384, 16384, 16384, 16384, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768};

typedef struct ISP_GE_S {
    GK_BOOL bEnable;
    GK_BOOL bCoefUpdateEn;

    GK_U32  bitDepth;
    GK_U16 u16GeProcCtTh1;
    GK_U16 u16GeProcCtTh3;
    GK_U16 u16GeProcStrength;

    ISP_CMOS_GE_S stCmosGe;
} ISP_GE_S;

ISP_GE_S *g_pastGeCtx[ISP_MAX_PIPE_NUM] = {GK_NULL};

#define GE_GET_CTX(dev, pstCtx)   (pstCtx = g_pastGeCtx[dev])
#define GE_SET_CTX(dev, pstCtx)   (g_pastGeCtx[dev] = pstCtx)
#define GE_RESET_CTX(dev)         (g_pastGeCtx[dev] = GK_NULL)

static GK_S32 GeCtxInit(VI_PIPE ViPipe)
{
    ISP_GE_S *pastGeCtx = GK_NULL;

    GE_GET_CTX(ViPipe, pastGeCtx);

    if (pastGeCtx == GK_NULL) {
        pastGeCtx = (ISP_GE_S *)ISP_MALLOC(sizeof(ISP_GE_S));
        if (pastGeCtx == GK_NULL) {
            ISP_TRACE(MODULE_DBG_ERR, "Isp[%d] GeCtx malloc memory failed!\n", ViPipe);
            return ERR_CODE_ISP_NOMEM;
        }
    }

    memset(pastGeCtx, 0, sizeof(ISP_GE_S));

    GE_SET_CTX(ViPipe, pastGeCtx);

    return GK_SUCCESS;
}

static GK_VOID GeCtxExit(VI_PIPE ViPipe)
{
    ISP_GE_S *pastGeCtx = GK_NULL;

    GE_GET_CTX(ViPipe, pastGeCtx);
    ISP_FREE(pastGeCtx);
    GE_RESET_CTX(ViPipe);
}

static GK_S32 GeCheckCmosParam(VI_PIPE ViPipe, const ISP_CMOS_GE_S *pstCmosGe)
{
    GK_U8 i;

    ISP_CHECK_BOOL(pstCmosGe->bEnable);

    if (pstCmosGe->u8Slope > ISP_CR_SLOPE_MAX) {
        ISP_TRACE(MODULE_DBG_ERR, "err u8Slope!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstCmosGe->u8SensiSlope > ISP_CR_SLOPE_MAX) {
        ISP_TRACE(MODULE_DBG_ERR, "err u8SensiSlope!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstCmosGe->u16SensiThr > ISP_CR_THR_MAX) {
        ISP_TRACE(MODULE_DBG_ERR, "err u16SensiThr!\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (pstCmosGe->au16Strength[i] > 0x100) {
            ISP_TRACE(MODULE_DBG_ERR, "err au16Strength[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstCmosGe->au16NpOffset[i] > ISP_CR_NPOFFSET_MAX || pstCmosGe->au16NpOffset[i] < ISP_CR_NPOFFSET_MIN) {
            ISP_TRACE(MODULE_DBG_ERR, "err au16NpOffset[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstCmosGe->au16Threshold[i] > ISP_CR_THR_MAX) {
            ISP_TRACE(MODULE_DBG_ERR, "err au16Threshold[%d]!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
    }

    return GK_SUCCESS;
}

static GK_S32 GeInitialize(VI_PIPE ViPipe)
{
    GK_S32     s32Ret;
    ISP_GE_S   *pstGe = GK_NULL;
    ISP_CMOS_DEFAULT_S  *pstSnsDft = GK_NULL;

    GE_GET_CTX(ViPipe, pstGe);
    ISP_SensorGetDefault(ViPipe, &pstSnsDft);


    if (pstSnsDft->unKey.bit1Ge) {
        ISP_CHECK_POINTER(pstSnsDft->pstGe);

        s32Ret = GeCheckCmosParam(ViPipe, pstSnsDft->pstGe);
        if (s32Ret != GK_SUCCESS) {
            return s32Ret;
        }

        memcpy(&pstGe->stCmosGe, pstSnsDft->pstGe, sizeof(ISP_CMOS_GE_S));
    } else {
        pstGe->stCmosGe.bEnable      = GK_TRUE;
        pstGe->stCmosGe.u8Slope      = ISP_GE_SLOPE_DEFAULT;
        pstGe->stCmosGe.u8SensiSlope = ISP_GE_SENSI_SLOPE_DEFAULT;
        pstGe->stCmosGe.u16SensiThr  = ISP_GE_SENSI_THR_DEFAULT;
        memcpy(pstGe->stCmosGe.au16Strength,  g_au16Strength,  ISP_AUTO_ISO_STRENGTH_NUM * sizeof(GK_U16));
        memcpy(pstGe->stCmosGe.au16Threshold, g_au16Threshold, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(GK_U16));
        memcpy(pstGe->stCmosGe.au16NpOffset,  g_au16NpOffset,  ISP_AUTO_ISO_STRENGTH_NUM * sizeof(GK_U16));
    }

    pstGe->bEnable = pstGe->stCmosGe.bEnable;

    pstGe->bitDepth   = ISP_MINIISP_BITDEPTH;

    return GK_SUCCESS;
}

static GK_VOID GeExtRegsInitialize(VI_PIPE ViPipe)
{
    GK_U32 i;
    ISP_GE_S *pstGe = GK_NULL;

    GE_GET_CTX(ViPipe, pstGe);


    /* initial ext register of Ge */
    ext_system_ge_enable_write(ViPipe, pstGe->bEnable);
    ext_system_ge_slope_write(ViPipe, pstGe->stCmosGe.u8Slope);
    ext_system_ge_sensitivity_write(ViPipe, pstGe->stCmosGe.u8SensiSlope);
    ext_system_ge_sensithreshold_write(ViPipe, pstGe->stCmosGe.u16SensiThr);
    ext_system_ge_coef_update_en_write(ViPipe, GK_TRUE);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        ext_system_ge_strength_write(ViPipe, i, pstGe->stCmosGe.au16Strength[i]);
        ext_system_ge_npoffset_write(ViPipe, i, pstGe->stCmosGe.au16NpOffset[i]);
        ext_system_ge_threshold_write(ViPipe, i, pstGe->stCmosGe.au16Threshold[i]);
    }

    return;
}

static GK_VOID GeStaticRegsInitialize(VI_PIPE ViPipe, ISP_GE_STATIC_CFG_S *pstGeStaticRegCfg)
{
    ISP_GE_S   *pstGe = GK_NULL;

    GE_GET_CTX(ViPipe, pstGe);
    ISP_CHECK_POINTER_VOID(pstGe);

    pstGeStaticRegCfg->bGeGrGbEn           = GK_TRUE;
    pstGeStaticRegCfg->bGeGrEn             = GK_FALSE;
    pstGeStaticRegCfg->bGeGbEn             = GK_FALSE;

    pstGeStaticRegCfg->bStaticResh = GK_TRUE;
}

static GK_VOID GeUsrRegsInitialize(VI_PIPE ViPipe, GK_U8 i, ISP_GE_USR_CFG_S *pstGeUsrRegCfg, ISP_CTX_S *pstIspCtx)
{
    pstGeUsrRegCfg->au16GeCtTh2   = ISP_GE_SENSITHR_DEFAULT;
    pstGeUsrRegCfg->au8GeCtSlope1 = ISP_GE_SENSISLOPE_DEFAULT;
    pstGeUsrRegCfg->au8GeCtSlope2 = ISP_GE_SLOPE_DEFAULT;


    pstGeUsrRegCfg->bResh = GK_TRUE;
}

static GK_VOID GeDynaRegsInitialize(ISP_GE_DYNA_CFG_S *pstGeDynaRegCfg)
{
    pstGeDynaRegCfg->au16GeCtTh1 = ISP_GE_NPOFFSET_DEFAULT;
    pstGeDynaRegCfg->au16GeCtTh3 = ISP_GE_THRESHOLD_DEFAULT;
    pstGeDynaRegCfg->u16GeStrength = ISP_GE_STRENGTH_DEFAULT;
    pstGeDynaRegCfg->bResh         = GK_TRUE;
}

static GK_VOID GeRegsInitialize(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfg)
{
    GK_U8      i;
    ISP_CTX_S  *pstIspCtx    = GK_NULL;
    ISP_GE_S   *pstGe = GK_NULL;


    GE_GET_CTX(ViPipe, pstGe);
    ISP_GET_CTX(ViPipe, pstIspCtx);


    for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
        GeStaticRegsInitialize(ViPipe, &pstRegCfg->stAlgRegCfg[i].stGeRegCfg.stStaticRegCfg);
        GeUsrRegsInitialize(ViPipe, i, &pstRegCfg->stAlgRegCfg[i].stGeRegCfg.stUsrRegCfg, pstIspCtx);
        GeDynaRegsInitialize(&pstRegCfg->stAlgRegCfg[i].stGeRegCfg.stDynaRegCfg);

        pstRegCfg->stAlgRegCfg[i].stGeRegCfg.abGeEn = pstGe->bEnable;
    }

    pstRegCfg->unKey.bit1GeCfg = 1;

    return;
}

static GK_S32 GeReadExtregs(VI_PIPE ViPipe)
{
    GK_U32 i;
    ISP_GE_S *pstGe = GK_NULL;

    GE_GET_CTX(ViPipe, pstGe);
    ISP_CHECK_POINTER(pstGe);

    /* read ext register of Ge */
    pstGe->bCoefUpdateEn         = ext_system_ge_coef_update_en_read(ViPipe);
    ext_system_ge_coef_update_en_write(ViPipe, GK_FALSE);

    if (pstGe->bCoefUpdateEn) {
        pstGe->stCmosGe.u8Slope      = ext_system_ge_slope_read(ViPipe);
        pstGe->stCmosGe.u8SensiSlope = ext_system_ge_sensitivity_read(ViPipe);
        pstGe->stCmosGe.u16SensiThr  = ext_system_ge_sensithreshold_read(ViPipe);

        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
            pstGe->stCmosGe.au16Strength[i] = ext_system_ge_strength_read(ViPipe, i);
            pstGe->stCmosGe.au16NpOffset[i] = ext_system_ge_npoffset_read(ViPipe, i);
            pstGe->stCmosGe.au16Threshold[i] = ext_system_ge_threshold_read(ViPipe, i);
        }
    }

    return GK_SUCCESS;
}

static GK_VOID Ge_Usr_Fw(ISP_GE_S *pstGe, ISP_GE_USR_CFG_S *pstGeUsrRegCfg)
{
    pstGeUsrRegCfg->au16GeCtTh2   = MIN2(pstGe->stCmosGe.u16SensiThr, (1 << pstGe->bitDepth));
    pstGeUsrRegCfg->au8GeCtSlope1 = MIN2(pstGe->stCmosGe.u8SensiSlope, pstGe->bitDepth);
    pstGeUsrRegCfg->au8GeCtSlope2 = MIN2(pstGe->stCmosGe.u8Slope, pstGe->bitDepth);

    pstGeUsrRegCfg->bResh    = GK_TRUE;
}

static GK_VOID Ge_Dyna_Fw(GK_U32 u32Iso, ISP_GE_S *pstGe, ISP_GE_DYNA_CFG_S *pstGeDynaRegCfg)
{

    GK_U8 u8IndexUpper = GetIsoIndex(u32Iso);
    GK_U8 u8IndexLower = MAX2((GK_S8)u8IndexUpper - 1, 0);


    pstGeDynaRegCfg->au16GeCtTh3 = CLIP3(LinearInter(u32Iso, g_au32IsoLut[u8IndexLower], pstGe->stCmosGe.au16Threshold[u8IndexLower], \
                                                     g_au32IsoLut[u8IndexUpper], pstGe->stCmosGe.au16Threshold[u8IndexUpper]), 0, (1 << pstGe->bitDepth));

    pstGeDynaRegCfg->au16GeCtTh1 = (GK_U16)LinearInter(u32Iso, g_au32IsoLut[u8IndexLower], pstGe->stCmosGe.au16NpOffset[u8IndexLower], \
                                                       g_au32IsoLut[u8IndexUpper], pstGe->stCmosGe.au16NpOffset[u8IndexUpper]);

    pstGeDynaRegCfg->u16GeStrength  = (GK_U16)LinearInter(u32Iso, g_au32IsoLut[u8IndexLower], pstGe->stCmosGe.au16Strength[u8IndexLower], \
                                                          g_au32IsoLut[u8IndexUpper], pstGe->stCmosGe.au16Strength[u8IndexUpper]);

    pstGe->u16GeProcCtTh1    = pstGeDynaRegCfg->au16GeCtTh1;
    pstGe->u16GeProcCtTh3    = pstGeDynaRegCfg->au16GeCtTh3;
    pstGe->u16GeProcStrength = pstGeDynaRegCfg->u16GeStrength;

    pstGeDynaRegCfg->bResh          = GK_TRUE;
}

static GK_BOOL __inline CheckGeOpen(ISP_GE_S *pstGe)
{
    return (pstGe->bEnable == GK_TRUE);
}

static GK_VOID Ge_Bypass(ISP_REG_CFG_S *pstReg, ISP_GE_S *pstGe)
{
    GK_U8 i;

    for (i = 0; i < pstReg->u8CfgNum; i++) {
        pstReg->stAlgRegCfg[i].stGeRegCfg.abGeEn = GK_FALSE;
    }

    pstReg->unKey.bit1GeCfg = 1;
}

GK_S32 ISP_GeInit(VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
    GK_S32 s32Ret = GK_SUCCESS;
    ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;

    s32Ret = GeCtxInit(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        return s32Ret;
    }

    s32Ret = GeInitialize(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        return s32Ret;
    }

    GeRegsInitialize(ViPipe, pstRegCfg);
    GeExtRegsInitialize(ViPipe);

    return GK_SUCCESS;
}

GK_S32 ISP_GeRun(VI_PIPE ViPipe, const GK_VOID *pStatInfo,
                 GK_VOID *pRegCfg, GK_S32 s32Rsv)
{
    GK_U8 i;
    ISP_GE_S  *pstGe = GK_NULL;
    ISP_CTX_S *pstIspCtx  = GK_NULL;
    ISP_REG_CFG_S *pstReg = (ISP_REG_CFG_S *)pRegCfg;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    GE_GET_CTX(ViPipe, pstGe);
    ISP_CHECK_POINTER(pstGe);

    /* calculate every two interrupts */

    if ((pstIspCtx->u32FrameCnt % 2 != 0) && (pstIspCtx->stLinkage.bSnapState != GK_TRUE)) {
        return GK_SUCCESS;
    }

    if (pstIspCtx->stLinkage.bDefectPixel) {
        Ge_Bypass(pstReg, pstGe);
        return GK_SUCCESS;
    }

    pstGe->bEnable = ext_system_ge_enable_read(ViPipe);

    for (i = 0; i < pstReg->u8CfgNum; i++) {
        pstReg->stAlgRegCfg[i].stGeRegCfg.abGeEn = pstGe->bEnable;
    }

    pstReg->unKey.bit1GeCfg = 1;

    /* check hardware setting */
    if (!CheckGeOpen(pstGe)) {
        return GK_SUCCESS;
    }

    GeReadExtregs(ViPipe);

    if (pstGe->bCoefUpdateEn) {
        for (i = 0; i < pstReg->u8CfgNum; i++) {
            Ge_Usr_Fw(pstGe, &pstReg->stAlgRegCfg[i].stGeRegCfg.stUsrRegCfg);
        }
    }

    for (i = 0; i < pstReg->u8CfgNum; i++) {
        Ge_Dyna_Fw(pstIspCtx->stLinkage.u32Iso, pstGe, &pstReg->stAlgRegCfg[i].stGeRegCfg.stDynaRegCfg);
    }

    return GK_SUCCESS;
}

GK_S32 GeProcWrite(VI_PIPE ViPipe, ISP_CTRL_PROC_WRITE_S *pstProc)
{
    ISP_CTRL_PROC_WRITE_S stProcTmp;
    ISP_GE_S  *pstGe = GK_NULL;


    GE_GET_CTX(ViPipe, pstGe);
    ISP_CHECK_POINTER(pstGe);

    if ((pstProc->pcProcBuff == GK_NULL)
        || (pstProc->u32BuffLen == 0)) {
        return GK_FAILURE;
    }

    stProcTmp.pcProcBuff = pstProc->pcProcBuff;
    stProcTmp.u32BuffLen = pstProc->u32BuffLen;

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
                    "-----GE INFO-------------------------------------------------------------\n");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
                    "%12s" "%12s" "%12s" "%12s\n",
                    "Enable", "NpOffset", "Threshold", "Strength");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
                    "%12u" "%12u" "%12u" "%12u\n",
                    pstGe->bEnable, pstGe->u16GeProcCtTh1, pstGe->u16GeProcCtTh3, pstGe->u16GeProcStrength);

    pstProc->u32WriteLen += 1;

    return GK_SUCCESS;
}

GK_S32 ISP_GeCtrl(VI_PIPE ViPipe, GK_U32 u32Cmd, GK_VOID *pValue)
{
    ISP_REGCFG_S  *pRegCfg = GK_NULL;


    switch (u32Cmd) {
        case ISP_WDR_MODE_SET :
            ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
            ISP_CHECK_POINTER(pRegCfg);
            ISP_GeInit(ViPipe, (GK_VOID *)&pRegCfg->stRegCfg);
            break;
        case ISP_CHANGE_IMAGE_MODE_SET:
            ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
            ISP_CHECK_POINTER(pRegCfg);
            break;
        case ISP_PROC_WRITE:
            GeProcWrite(ViPipe, (ISP_CTRL_PROC_WRITE_S *)pValue);
            break;

        default :
            break;
    }
    return GK_SUCCESS;
}

GK_S32 ISP_GeExit(VI_PIPE ViPipe)
{
    GK_U8 i;
    ISP_REGCFG_S  *pRegCfg = GK_NULL;


    ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

    for (i = 0; i < pRegCfg->stRegCfg.u8CfgNum; i++) {
        pRegCfg->stRegCfg.stAlgRegCfg[i].stGeRegCfg.abGeEn = GK_FALSE;
    }

    pRegCfg->stRegCfg.unKey.bit1GeCfg = 1;

    GeCtxExit(ViPipe);

    return GK_SUCCESS;
}

GK_S32 ISP_AlgRegisterGe(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_ALG_NODE_S *pstAlgs = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_ALG_CHECK(pstIspCtx->unAlgKey.bit1Ge);
    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_GE;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_GeInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_GeRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_GeCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_GeExit;
    pstAlgs->bUsed = GK_TRUE;

    return GK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
