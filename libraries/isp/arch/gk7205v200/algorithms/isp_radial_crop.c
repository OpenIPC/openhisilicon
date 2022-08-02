/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <math.h>
#include <stdio.h>
#include "isp_alg.h"
#include "isp_ext_config.h"
#include "isp_config.h"
#include "isp_sensor.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef struct {
    GK_BOOL bRcEn;
    GK_BOOL bCoefUpdateEn;
    GK_U16  u16CenterVerCoor;
    GK_U16  u16CenterHorCoor;
    GK_U32  u32Radius;
} ISP_RC_S;

ISP_RC_S g_astRcCtx[ISP_MAX_PIPE_NUM] = {{0}};
#define RC_GET_CTX(dev, pstCtx)   pstCtx = &g_astRcCtx[dev]

static GK_VOID RcUsrRegsInitialize(ISP_RC_USR_CFG_S *pstUsrRegCfg, ISP_RC_S *pstRc)
{
    pstUsrRegCfg->u16CenterHorCoor = pstRc->u16CenterHorCoor;
    pstUsrRegCfg->u16CenterVerCoor = pstRc->u16CenterVerCoor;
    pstUsrRegCfg->u32SquareRadius  = pstRc->u32Radius * pstRc->u32Radius;
    pstUsrRegCfg->bResh            = GK_TRUE;
}

static GK_VOID RcRegsInitialize(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfg)
{
    ISP_RC_S   *pstRc = GK_NULL;

    RC_GET_CTX(ViPipe, pstRc);

    RcUsrRegsInitialize(&pstRegCfg->stAlgRegCfg[0].stRcRegCfg.stUsrRegCfg, pstRc);

    pstRegCfg->stAlgRegCfg[0].stRcRegCfg.bRcEn = pstRc->bRcEn;

    pstRegCfg->unKey.bit1RcCfg = 1;

    return;
}

static GK_VOID RcExtRegsInitialize(VI_PIPE ViPipe)
{
    ISP_RC_S   *pstRc = GK_NULL;

    RC_GET_CTX(ViPipe, pstRc);

    ext_system_rc_en_write(ViPipe, pstRc->bRcEn);
    ext_system_rc_center_hor_coor_write(ViPipe, pstRc->u16CenterHorCoor);
    ext_system_rc_center_ver_coor_write(ViPipe, pstRc->u16CenterVerCoor);
    ext_system_rc_radius_write(ViPipe, pstRc->u32Radius);
    ext_system_rc_coef_update_en_write(ViPipe, GK_FALSE);

    return;
}

static GK_VOID RcInitialize(VI_PIPE ViPipe)
{
    GK_U32     u32HorCoor, u32VerCoor;
    ISP_RC_S   *pstRc = GK_NULL;
    ISP_CTX_S  *pstIspCtx = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    RC_GET_CTX(ViPipe, pstRc);

    u32HorCoor  = pstIspCtx->stBlockAttr.stFrameRect.u32Width >> 1;
    u32VerCoor  = pstIspCtx->stBlockAttr.stFrameRect.u32Height >> 1;
    pstRc->u16CenterHorCoor = u32HorCoor;
    pstRc->u16CenterVerCoor = u32VerCoor;
    pstRc->u32Radius        = Sqrt32(u32HorCoor * u32HorCoor + u32VerCoor * u32VerCoor) + 1;
    pstRc->bRcEn   = GK_FALSE;
}

static GK_BOOL __inline CheckRcOpen(ISP_RC_S  *pstRc)
{
    return (pstRc->bRcEn == GK_TRUE);
}

static GK_VOID RcReadExtRegs(VI_PIPE ViPipe)
{
    ISP_RC_S *pstRc      = GK_NULL;
    RC_GET_CTX(ViPipe, pstRc);

    pstRc->bCoefUpdateEn    = ext_system_rc_coef_update_en_read(ViPipe);

    ext_system_rc_coef_update_en_write(ViPipe, GK_FALSE);

    if (pstRc->bCoefUpdateEn) {
        pstRc->u16CenterHorCoor = ext_system_rc_center_hor_coor_read(ViPipe);
        pstRc->u16CenterVerCoor = ext_system_rc_center_ver_coor_read(ViPipe);
        pstRc->u32Radius        = ext_system_rc_radius_read(ViPipe);
    }
}

GK_VOID Rc_Usr_Fw(ISP_RC_USR_CFG_S *pstUsrRegCfg, ISP_RC_S *pstRc)
{
    pstUsrRegCfg->u16CenterHorCoor = pstRc->u16CenterHorCoor;
    pstUsrRegCfg->u16CenterVerCoor = pstRc->u16CenterVerCoor;
    pstUsrRegCfg->u32SquareRadius  = pstRc->u32Radius * pstRc->u32Radius;
    pstUsrRegCfg->bResh            = GK_TRUE;
}

static GK_VOID ISP_RcWdrModeSet(VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
    ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;
    GK_U8  u8WDRMode;
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_RC_S  *pstRc       = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    u8WDRMode = pstIspCtx->u8SnsWDRMode;
    RC_GET_CTX(ViPipe, pstRc);

    if(IS_2to1_WDR_MODE(u8WDRMode) && (pstRc->bRcEn)) {
        ISP_TRACE(MODULE_DBG_ERR, "Rc not support in Wdr mode\n");
    }
    pstRegCfg->unKey.bit1RcCfg = 1;
    pstRegCfg->stAlgRegCfg[0].stRcRegCfg.stUsrRegCfg.bResh = GK_TRUE;
}

GK_S32 ISP_RcInit(VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
    ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;

    RcInitialize(ViPipe);
    RcRegsInitialize(ViPipe, pstRegCfg);
    RcExtRegsInitialize(ViPipe);

    return GK_SUCCESS;
}

GK_S32 ISP_RcRun(VI_PIPE ViPipe, const GK_VOID *pStatInfo,
                 GK_VOID *pRegCfg, GK_S32 s32Rsv)
{
    ISP_RC_S  *pstRc       = GK_NULL;
    ISP_CTX_S *pstIspCtx   = GK_NULL;
    ISP_REG_CFG_S *pstRegCfg  = (ISP_REG_CFG_S *)pRegCfg;

    ISP_GET_CTX(ViPipe, pstIspCtx);
    RC_GET_CTX(ViPipe, pstRc);

    /* calculate every two interrupts */
    if ((pstIspCtx->u32FrameCnt % 2 != 0) && (pstIspCtx->stLinkage.bSnapState != GK_TRUE)) {
        return GK_SUCCESS;
    }

    pstRc->bRcEn = ext_system_rc_en_read(ViPipe);
    pstRegCfg->stAlgRegCfg[0].stRcRegCfg.bRcEn = pstRc->bRcEn;
    pstRegCfg->unKey.bit1RcCfg = 1;

    /* check hardware setting */
    if (!CheckRcOpen(pstRc)) {
        return GK_SUCCESS;
    }

    RcReadExtRegs(ViPipe);

    if (pstRc->bCoefUpdateEn) {
        Rc_Usr_Fw(&pstRegCfg->stAlgRegCfg[0].stRcRegCfg.stUsrRegCfg, pstRc);
    }

    return GK_SUCCESS;
}

GK_S32 ISP_RcCtrl(VI_PIPE ViPipe, GK_U32 u32Cmd, GK_VOID *pValue)
{
    ISP_REGCFG_S  *pRegCfg = GK_NULL;

    switch (u32Cmd) {
        case ISP_WDR_MODE_SET :
            ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
            ISP_CHECK_POINTER(pRegCfg);
            ISP_RcWdrModeSet(ViPipe, (GK_VOID *)&pRegCfg->stRegCfg);
            break;
        default :
            break;
    }
    return GK_SUCCESS;
}

GK_S32 ISP_RcExit(VI_PIPE ViPipe)
{
    ISP_REGCFG_S  *pRegCfg = GK_NULL;

    ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

    pRegCfg->stRegCfg.stAlgRegCfg[0].stRcRegCfg.bRcEn = GK_FALSE;
    pRegCfg->stRegCfg.unKey.bit1RcCfg                 = 1;

    return GK_SUCCESS;
}

GK_S32 ISP_AlgRegisterRc(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_ALG_NODE_S *pstAlgs = GK_NULL;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_RC;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_RcInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_RcRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_RcCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_RcExit;
    pstAlgs->bUsed = GK_TRUE;

    return GK_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
