/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <stdio.h>
#include <sys/ioctl.h>
#include "mkp_isp.h"
#include "ot_mpi_sys.h"
#include "isp_dcfinfo.h"
#include "isp_ext_config.h"
#include "isp_main.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

extern GK_S32 g_as32IspFd[ISP_MAX_PIPE_NUM];

GK_S32 ISP_UpdateInfoInit(VI_PIPE ViPipe)
{
    GK_U64 u64PhyAddr;
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    u64PhyAddr = pstIspCtx->stIspTransInfo.stUpdateInfo.u64PhyAddr;

    ext_system_update_infogh_phyaddr_write(ViPipe, ((u64PhyAddr >> 32) & 0xFFFFFFFF));
    ext_system_update_info_low_phyaddr_write(ViPipe, (u64PhyAddr & 0xFFFFFFFF));

    pstIspCtx->stUpdateInfoCtrl.pstIspUpdateInfo = MPI_SYS_Mmap(u64PhyAddr,
                                                                   (sizeof(ISP_DCF_UPDATE_INFO_S) * ISP_MAX_UPDATEINFO_BUF_NUM + sizeof(ISP_DCF_CONST_INFO_S)));

    if (pstIspCtx->stUpdateInfoCtrl.pstIspUpdateInfo == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "isp[%d] mmap update info buf failed!\n", ViPipe);
        return ERR_CODE_ISP_NOMEM;
    }

    pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo =
       (ISP_DCF_CONST_INFO_S *)(pstIspCtx->stUpdateInfoCtrl.pstIspUpdateInfo + ISP_MAX_UPDATEINFO_BUF_NUM);

    return GK_SUCCESS;
}

GK_S32 ISP_UpdateInfoExit(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    if (pstIspCtx->stUpdateInfoCtrl.pstIspUpdateInfo != GK_NULL) {
        MPI_SYS_Munmap(pstIspCtx->stUpdateInfoCtrl.pstIspUpdateInfo,
                          (sizeof(ISP_DCF_UPDATE_INFO_S) * ISP_MAX_UPDATEINFO_BUF_NUM + sizeof(ISP_DCF_CONST_INFO_S)));
        pstIspCtx->stUpdateInfoCtrl.pstIspUpdateInfo = GK_NULL;
        pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo = GK_NULL;
    }

    return GK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
