/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include "isp_config.h"
#include "isp_debug.h"
#include "isp_ext_config.h"
#include "isp_math_utils.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_proc.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define MCDS_EN          (1)
#define MCDS_FILTER_MODE (1)     // 1: filter mode; 0: discard mode

static GK_VOID McdsStaticRegInit(VI_PIPE ViPipe, ISP_MCDS_STATIC_REG_CFG_S *pstStaticRegCfg, ISP_CTX_S *pstIspCtx)
{
    static const GK_S32 Coeff_Filter_8tap_H[2][8] = {{ -16, 0, 145, 254, 145, 0, -16, 0}, {0, 0, 0, 256, 256, 0, 0, 0}};
    static const GK_S32 Coeff_Discard_8pixel_H[8] = {0, 0, 0, 512, 0, 0, 0, 0};

    GK_S32 s32HCoef[8];
    GK_S32 s32EnFilterV = 1;
    GK_U32  i;

    if (MCDS_FILTER_MODE) {         // Filter Mode
        if (pstIspCtx->stHdrAttr.enDynamicRange == DYNAMIC_RANGE_XDR) {     // HDR mode
            for (i = 0; i < 8; i++) {

                s32HCoef[i] = Coeff_Filter_8tap_H[1][i];
            }
        } else { // SDR mode
            for (i = 0; i < 8; i++) {
                s32HCoef[i] = Coeff_Filter_8tap_H[0][i];
            }
        }
        s32EnFilterV = 1;
    } else {    // discard Mode
        for (i = 0; i < 8; i++) {
            s32HCoef[i] = Coeff_Discard_8pixel_H[i];
        }
        s32EnFilterV = 0;
    }

    pstStaticRegCfg->bHcdsEn    = 1;
    pstStaticRegCfg->s32HCoef0  = s32HCoef[0];
    pstStaticRegCfg->s32HCoef1  = s32HCoef[1];
    pstStaticRegCfg->s32HCoef2  = s32HCoef[2];
    pstStaticRegCfg->s32HCoef3  = s32HCoef[3];
    pstStaticRegCfg->s32HCoef4  = s32HCoef[4];
    pstStaticRegCfg->s32HCoef5  = s32HCoef[5];
    pstStaticRegCfg->s32HCoef6  = s32HCoef[6];
    pstStaticRegCfg->s32HCoef7  = s32HCoef[7];
    pstStaticRegCfg->s32EnFilterV = s32EnFilterV;
    pstStaticRegCfg->bStaticResh    = GK_TRUE;
}

static GK_VOID McdsDynaRegInit(ISP_MCDS_DYNA_REG_CFG_S *pstDynaRegCfg, ISP_CTX_S *pstIspCtx)
{
    pstDynaRegCfg->bDynaResh = 1;

    if (pstIspCtx->stHdrAttr.enFormat == PIXEL_FORMAT_YVU_SEMIPLANAR_422) {
        pstDynaRegCfg->bVcdsEn    = GK_FALSE;
    } else { // 400 is the same with 420
        pstDynaRegCfg->bVcdsEn    = GK_TRUE;
    }
}

static GK_VOID McdsRegsInitialize(VI_PIPE ViPipe, ISP_REG_CFG_S *pRegCfg)
{
    GK_U32 i;

    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    for (i = 0; i < pRegCfg->u8CfgNum; i++) {
        pRegCfg->stAlgRegCfg[i].stMcdsRegCfg.bMCDSen = GK_TRUE;
        McdsStaticRegInit(ViPipe, &(pRegCfg->stAlgRegCfg[i].stMcdsRegCfg.stStaticRegCfg), pstIspCtx);
        McdsDynaRegInit(&(pRegCfg->stAlgRegCfg[i].stMcdsRegCfg.stDynaRegCfg), pstIspCtx);
    }

    pRegCfg->unKey.bit1McdsCfg = 1;

    return;
}

GK_S32 McdsProcWrite(VI_PIPE ViPipe, ISP_CTRL_PROC_WRITE_S *pstProc)
{
    return GK_SUCCESS;
}

GK_S32 ISP_McdsInit(VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
    ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;

    McdsRegsInitialize(ViPipe, pstRegCfg);

    return GK_SUCCESS;
}

GK_S32 ISP_McdsRun(VI_PIPE ViPipe, const GK_VOID *pStatInfo, GK_VOID *pRegCfg, GK_S32 s32Rsv)
{
    GK_U8  i;
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_ALG_CHECK(pstIspCtx->unAlgKey.bit1Mcds);

    if (pstIspCtx->stLinkage.bDefectPixel) {
        return GK_SUCCESS;
    }

    /* calculate every two interrupts */
    if ((pstIspCtx->u32FrameCnt % 2 != 0) && (pstIspCtx->stLinkage.bSnapState != GK_TRUE)) {
        return GK_SUCCESS;
    }

    for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
        pstRegCfg->stAlgRegCfg[i].stMcdsRegCfg.bMCDSen = GK_TRUE;
        pstRegCfg->stAlgRegCfg[i].stMcdsRegCfg.stDynaRegCfg.bDynaResh = 1;
        if (pstIspCtx->stHdrAttr.enFormat == PIXEL_FORMAT_YVU_SEMIPLANAR_422) {
            pstRegCfg->stAlgRegCfg[i].stMcdsRegCfg.stDynaRegCfg.bVcdsEn = GK_FALSE;
        } else { // 400 is the same with 420
            pstRegCfg->stAlgRegCfg[i].stMcdsRegCfg.stDynaRegCfg.bVcdsEn    = GK_TRUE;
        }
    }
    pstRegCfg->unKey.bit1McdsCfg = 1;

    return GK_SUCCESS;
}

GK_S32 ISP_McdsCtrl(VI_PIPE ViPipe, GK_U32 u32Cmd, GK_VOID *pValue)
{
    ISP_REGCFG_S  *pRegCfg = GK_NULL;
    switch (u32Cmd) {
        case ISP_PROC_WRITE:
            McdsProcWrite(ViPipe, (ISP_CTRL_PROC_WRITE_S *)pValue);
            break;
        case  ISP_CHANGE_IMAGE_MODE_SET:
            ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
            ISP_CHECK_POINTER(pRegCfg);
            ISP_McdsInit(ViPipe, &pRegCfg->stRegCfg);
        default :
            break;
    }
    return GK_SUCCESS;
}

GK_S32 ISP_McdsExit(VI_PIPE ViPipe)
{
    GK_U8 i;
    ISP_REGCFG_S  *pRegCfg = GK_NULL;

    ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

    for (i = 0; i < pRegCfg->stRegCfg.u8CfgNum; i++) {
        pRegCfg->stRegCfg.stAlgRegCfg[i].stMcdsRegCfg.bMCDSen = GK_FALSE;
    }

    pRegCfg->stRegCfg.unKey.bit1McdsCfg = 1;
    return GK_SUCCESS;
}

GK_S32 ISP_AlgRegisterMcds(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_ALG_NODE_S *pstAlgs = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    ISP_ALG_CHECK(pstIspCtx->unAlgKey.bit1Mcds);
    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_MCDS;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_McdsInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_McdsRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_McdsCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_McdsExit;
    pstAlgs->bUsed = GK_TRUE;

    return GK_SUCCESS;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
