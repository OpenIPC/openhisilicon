/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include "mkp_isp.h"
#include "ot_mpi_sys.h"
#include "isp_statistics.h"
#include "isp_ext_config.h"
#include "isp_main.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct ISP_STA_S {
    GK_U64  u64StatPhyaddr;
    GK_VOID *pStatVirtAddr;

    GK_BOOL bRead;
    ISP_STAT_INFO_S stStatInfo;
} ISP_STA_S;

ISP_STA_S g_astStatCtx[ISP_MAX_PIPE_NUM] = { { 0 } };
#define STATISTICS_GET_CTX(dev, pstCtx) pstCtx = &g_astStatCtx[dev]

extern GK_S32 g_as32IspFd[ISP_MAX_PIPE_NUM];

GK_S32 ISP_StatisticsInit(VI_PIPE ViPipe)
{
    GK_S32 s32Ret;
    ISP_STA_S *pstStat = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    STATISTICS_GET_CTX(ViPipe, pstStat);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_STAT_BUF_INIT, &pstStat->u64StatPhyaddr);

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] init statistics bufs failed!\n", ViPipe);
        return s32Ret;
    }

    pstStat->pStatVirtAddr = MPI_SYS_MmapCache(pstStat->u64StatPhyaddr,
                                                  sizeof(ISP_STAT_S) * MAX_ISP_STAT_BUF_NUM);

    if (pstStat->pStatVirtAddr == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] mmap statistics bufs failed!\n", ViPipe);

        s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_STAT_BUF_EXIT);

        if (s32Ret != GK_SUCCESS) {
            ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] exit statistics bufs failed!\n", ViPipe);
            return s32Ret;
        }

        return ERR_CODE_ISP_NOMEM;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_StatisticsExit(VI_PIPE ViPipe)
{
    GK_S32 s32Ret;
    ISP_STA_S *pstStat = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    STATISTICS_GET_CTX(ViPipe, pstStat);

    if (pstStat->pStatVirtAddr != GK_NULL) {
        MPI_SYS_Munmap(pstStat->pStatVirtAddr,
                          sizeof(ISP_STAT_S) * MAX_ISP_STAT_BUF_NUM);
        pstStat->pStatVirtAddr = GK_NULL;
    }

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_STAT_BUF_EXIT);

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] exit statistics bufs failed!\n", ViPipe);
        return s32Ret;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_StatisticsGetBuf(VI_PIPE ViPipe, GK_VOID **ppStat)
{
    GK_S32 s32Ret;
    ISP_STA_S *pstStat = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(ppStat);

    STATISTICS_GET_CTX(ViPipe, pstStat);

    if (!pstStat->bRead) {
        s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_STAT_BUF_GET, &pstStat->stStatInfo);
        if (s32Ret) {
            return s32Ret;
        }

        if (pstStat->pStatVirtAddr != GK_NULL) {
            pstStat->stStatInfo.pVirtAddr = (GK_VOID *)((GK_U8 *)pstStat->pStatVirtAddr +
                                                        (pstStat->stStatInfo.u64PhyAddr - pstStat->u64StatPhyaddr));
        } else {
            pstStat->stStatInfo.pVirtAddr = GK_NULL;
        }

        pstStat->bRead = GK_TRUE;
    }

    if (pstStat->stStatInfo.pVirtAddr == GK_NULL) {
        return GK_FAILURE;
    }

    *ppStat = pstStat->stStatInfo.pVirtAddr;

    return GK_SUCCESS;
}

GK_S32 ISP_StatisticsPutBuf(VI_PIPE ViPipe)
{
    GK_S32 s32Ret;
    GK_U32 u32KeyLowbit, u32KeyHighbit;
    ISP_STA_S *pstStat = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    STATISTICS_GET_CTX(ViPipe, pstStat);

    u32KeyLowbit  = ext_system_statistics_ctrl_lowbit_read(ViPipe);
    u32KeyHighbit = ext_system_statistics_ctrlghbit_read(ViPipe);
    pstStat->stStatInfo.unKey.u64Key = ((GK_U64)u32KeyHighbit << 32) + u32KeyLowbit;

    if (pstStat->bRead) {
        s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_STAT_BUF_PUT, &pstStat->stStatInfo);
        if (s32Ret) {
            ISP_TRACE(MODULE_DBG_ERR, "Release ISP[%d] stat buf Failed with ec %#x!\n", ViPipe, s32Ret);
        }
        pstStat->stStatInfo.pVirtAddr = GK_NULL;
        pstStat->bRead = GK_FALSE;
    }

    return GK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
