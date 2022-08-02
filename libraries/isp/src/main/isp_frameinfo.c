/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <stdio.h>
#include <sys/ioctl.h>
#include "mkp_isp.h"
#include "ot_mpi_sys.h"
#include "isp_frameinfo.h"
#include "isp_main.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

extern GK_S32 g_as32IspFd[ISP_MAX_PIPE_NUM];

GK_S32 ISP_TransInfoInit(VI_PIPE ViPipe, ISP_TRANS_INFO_BUF_S *pstTransInfo)
{
    GK_S32 s32Ret;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstTransInfo);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_TRANS_BUF_INIT, pstTransInfo);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "isp[%d] init trans info bufs failed %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_TransInfoExit(VI_PIPE ViPipe)
{
    GK_S32 s32Ret;
    ISP_CHECK_PIPE(ViPipe);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_TRANS_BUF_EXIT);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "isp[%d] exit trans info buf failed %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_FrameInfoInit(VI_PIPE ViPipe)
{
    GK_U64 u64PhyAddr;
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    u64PhyAddr = pstIspCtx->stIspTransInfo.stFrameInfo.u64PhyAddr;

    pstIspCtx->stFrameInfoCtrl.pstIspFrame = MPI_SYS_Mmap(u64PhyAddr,
                                                             sizeof(ISP_FRAME_INFO_S) * ISP_MAX_FRAMEINFO_BUF_NUM);

    if (pstIspCtx->stFrameInfoCtrl.pstIspFrame == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "isp[%d] mmap frame info buf failed!\n", ViPipe);
        return ERR_CODE_ISP_NOMEM;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_FrameInfoExit(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    if (pstIspCtx->stFrameInfoCtrl.pstIspFrame != GK_NULL) {
        MPI_SYS_Munmap(pstIspCtx->stFrameInfoCtrl.pstIspFrame, sizeof(ISP_FRAME_INFO_S) * ISP_MAX_FRAMEINFO_BUF_NUM);
        pstIspCtx->stFrameInfoCtrl.pstIspFrame = GK_NULL;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_AttachInfoInit(VI_PIPE ViPipe)
{
    GK_U64 u64PhyAddr;
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    u64PhyAddr = pstIspCtx->stIspTransInfo.stAttaInfo.u64PhyAddr;

    pstIspCtx->stAttachInfoCtrl.pstAttachInfo = MPI_SYS_Mmap(u64PhyAddr, sizeof(ISP_ATTACH_INFO_S));

    if (pstIspCtx->stAttachInfoCtrl.pstAttachInfo == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "isp[%d] mmap attach info buf failed!\n", ViPipe);
        return ERR_CODE_ISP_NOMEM;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_AttachInfoExit(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    if (pstIspCtx->stAttachInfoCtrl.pstAttachInfo != GK_NULL) {
        MPI_SYS_Munmap(pstIspCtx->stAttachInfoCtrl.pstAttachInfo, sizeof(ISP_ATTACH_INFO_S));
        pstIspCtx->stAttachInfoCtrl.pstAttachInfo = GK_NULL;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_ColorGamutInfoInit(VI_PIPE ViPipe)
{
    GK_U64 u64PhyAddr;
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    u64PhyAddr = pstIspCtx->stIspTransInfo.stColorGammutInfo.u64PhyAddr;

    pstIspCtx->stGamutInfoCtrl.pstColorGamutInfo = MPI_SYS_Mmap(u64PhyAddr, sizeof(ISP_COLORGAMMUT_INFO_S));

    if (pstIspCtx->stGamutInfoCtrl.pstColorGamutInfo == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "isp[%d] mmap color gamut info buf failed!\n", ViPipe);
        return ERR_CODE_ISP_NOMEM;
    }

    pstIspCtx->stGamutInfoCtrl.pstColorGamutInfo->enColorGamut = COLOR_GAMUT_BT709;

    return GK_SUCCESS;
}

GK_S32 ISP_ColorGamutInfoExit(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    if (pstIspCtx->stGamutInfoCtrl.pstColorGamutInfo != GK_NULL) {
        MPI_SYS_Munmap(pstIspCtx->stGamutInfoCtrl.pstColorGamutInfo, sizeof(ISP_COLORGAMMUT_INFO_S));
        pstIspCtx->stGamutInfoCtrl.pstColorGamutInfo = GK_NULL;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_ProInfoInit(VI_PIPE ViPipe, ISP_PRO_INFO_BUF_S *pstProInfo)
{
    GK_S32 s32Ret;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstProInfo);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_PRO_BUF_INIT, pstProInfo);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "isp[%d] init pro info bufs failed %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_ProInfoExit(VI_PIPE ViPipe)
{
    GK_S32 s32Ret;
    ISP_CHECK_PIPE(ViPipe);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_PRO_BUF_EXIT);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "isp[%d] exit pro info buf failed %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_ProNrParamInit(VI_PIPE ViPipe)
{
    GK_U64 u64PhyAddr;
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    u64PhyAddr = pstIspCtx->stIspProInfo.stProNrParamInfo.u64PhyAddr;

    pstIspCtx->stProNrParamCtrl.pstProNrParam = MPI_SYS_Mmap(u64PhyAddr, sizeof(ISP_PRO_NR_PARAM_S));

    if (pstIspCtx->stProNrParamCtrl.pstProNrParam == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "isp[%d] mmap pro nr paramt buf failed!\n", ViPipe);
        return ERR_CODE_ISP_NOMEM;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_ProNrParamExit(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    if (pstIspCtx->stProNrParamCtrl.pstProNrParam != GK_NULL) {
        MPI_SYS_Munmap(pstIspCtx->stProNrParamCtrl.pstProNrParam, sizeof(ISP_PRO_NR_PARAM_S));
        pstIspCtx->stProNrParamCtrl.pstProNrParam = GK_NULL;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_ProShpParamInit(VI_PIPE ViPipe)
{
    GK_U64 u64PhyAddr;
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    u64PhyAddr = pstIspCtx->stIspProInfo.stProShpParamInfo.u64PhyAddr;

    pstIspCtx->stProShpParamCtrl.pstProShpParam = MPI_SYS_Mmap(u64PhyAddr, sizeof(ISP_PRO_SHP_PARAM_S));

    if (pstIspCtx->stProShpParamCtrl.pstProShpParam == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "isp[%d] mmap pro shp paramt buf failed!\n", ViPipe);
        return ERR_CODE_ISP_NOMEM;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_ProShpParamExit(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    if (pstIspCtx->stProShpParamCtrl.pstProShpParam != GK_NULL) {
        MPI_SYS_Munmap(pstIspCtx->stProShpParamCtrl.pstProShpParam, sizeof(ISP_PRO_SHP_PARAM_S));
        pstIspCtx->stProShpParamCtrl.pstProShpParam = GK_NULL;
    }

    return GK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
