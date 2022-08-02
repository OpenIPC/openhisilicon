/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include "ot_mpi_sys.h"
#include "mkp_isp.h"
#include "isp_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct ISP_PROC_S {
    GK_U32 u32IntCount;
    GK_U32 u32ProcParam;
    ISP_PROC_MEM_S stProcMem;
} ISP_PROC_S;

ISP_PROC_S g_astProcCtx[ISP_MAX_PIPE_NUM] = { { 0 } };
#define PROC_GET_CTX(dev, pstCtx) pstCtx = &g_astProcCtx[dev]

GK_S32 ISP_UpdateProcParam(VI_PIPE ViPipe);

GK_S32 ISP_ProcInit(VI_PIPE ViPipe)
{
    GK_S32 s32Ret;
    ISP_PROC_S *pstProc = GK_NULL;

    PROC_GET_CTX(ViPipe, pstProc);

    ISP_CHECK_OPEN(ViPipe);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_PROC_PARAM_GET, &pstProc->u32ProcParam);
    if (s32Ret) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] get proc param %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    if (pstProc->u32ProcParam == 0) {
        return GK_SUCCESS;
    }

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_PROC_INIT, &pstProc->stProcMem);
    if (s32Ret) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] init proc ec %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    pstProc->stProcMem.pProcVirtAddr = MPI_SYS_Mmap(pstProc->stProcMem.u64ProcPhyAddr,
                                                       pstProc->stProcMem.u32ProcSize);
    if (pstProc->stProcMem.pProcVirtAddr == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] mmap proc mem failed!\n", ViPipe);
        s32Ret = ERR_CODE_ISP_NOMEM;
        goto freeproc;
    }
    pstProc->u32IntCount = 0;

    return GK_SUCCESS;

freeproc:
    if (ioctl(g_as32IspFd[ViPipe], ISP_PROC_EXIT) != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] exit proc failed!\n", ViPipe);
        return GK_FAILURE;
    }
    return s32Ret;
}

GK_S32 ISP_ProcWrite(const ISP_ALG_NODE_S *astAlgs, VI_PIPE ViPipe)
{
    GK_S32 s32Ret, i;
    ISP_PROC_S *pstProc = GK_NULL;
    ISP_CTRL_PROC_WRITE_S stProcCtrl;

    PROC_GET_CTX(ViPipe, pstProc);

    ISP_CHECK_OPEN(ViPipe);

    ISP_UpdateProcParam(ViPipe);

    if (pstProc->u32ProcParam == 0) {
        return GK_SUCCESS;
    }

    if (pstProc->stProcMem.pProcVirtAddr == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] the proc hasn't init!\n", ViPipe);
        return GK_FAILURE;
    }

    /* write proc info 1s a time */
    pstProc->u32IntCount++;
    if (pstProc->u32IntCount < pstProc->u32ProcParam) {
        return GK_SUCCESS;
    }
    pstProc->u32IntCount = 0;

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_PROC_WRITE_ING);
    if (s32Ret) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] write proc failed, ec %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    stProcCtrl.pcProcBuff = (GK_CHAR *)pstProc->stProcMem.pProcVirtAddr;
    stProcCtrl.u32BuffLen = pstProc->stProcMem.u32ProcSize - 1;
    stProcCtrl.u32WriteLen = 0;

    for (i = 0; i < ISP_MAX_ALGS_NUM; i++) {
        if (astAlgs[i].bUsed) {
            if (astAlgs[i].stAlgFunc.pfn_alg_ctrl != GK_NULL) {
                astAlgs[i].stAlgFunc.pfn_alg_ctrl(ViPipe, ISP_PROC_WRITE, &stProcCtrl);
            }

            if (stProcCtrl.u32WriteLen > stProcCtrl.u32BuffLen) {
                ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] Warning!! Proc buff overflow!\n", ViPipe);
                stProcCtrl.u32WriteLen = stProcCtrl.u32BuffLen;
                break;
            }

            if (stProcCtrl.u32WriteLen != 0) {
                if (stProcCtrl.pcProcBuff[stProcCtrl.u32WriteLen - 1] != '\0') {
                    ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] Warning!! alg %d's proc doesn't finished with endl!\n", ViPipe, astAlgs[i].enAlgType);
                }
                stProcCtrl.pcProcBuff[stProcCtrl.u32WriteLen - 1] = '\n';
            }

            /* update the proc ctrl */
            stProcCtrl.pcProcBuff = &stProcCtrl.pcProcBuff[stProcCtrl.u32WriteLen];
            stProcCtrl.u32BuffLen = stProcCtrl.u32BuffLen - stProcCtrl.u32WriteLen;
            stProcCtrl.u32WriteLen = 0;
            if (stProcCtrl.u32BuffLen == 0) {
                break;
            }
        }
    }

    stProcCtrl.pcProcBuff[stProcCtrl.u32WriteLen] = '\0';
    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_PROC_WRITE_OK);
    if (s32Ret) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] write proc failed, ec %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_ProcExit(VI_PIPE ViPipe)
{
    GK_S32 s32Ret;
    GK_VOID *pVirtAddr;
    ISP_PROC_S *pstProc = GK_NULL;

    PROC_GET_CTX(ViPipe, pstProc);

    ISP_CHECK_OPEN(ViPipe);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_PROC_PARAM_GET, &pstProc->u32ProcParam);
    if (s32Ret) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] get proc param %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    if (pstProc->u32ProcParam == 0) {
        return GK_SUCCESS;
    }

    if (pstProc->stProcMem.pProcVirtAddr != GK_NULL) {
        pVirtAddr = pstProc->stProcMem.pProcVirtAddr;
        pstProc->stProcMem.pProcVirtAddr = GK_NULL;
        MPI_SYS_Munmap(pVirtAddr, pstProc->stProcMem.u32ProcSize);
    }

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_PROC_EXIT);
    if (s32Ret) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] exit proc ec %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_UpdateProcParam(VI_PIPE ViPipe)
{
    GK_S32 s32Ret;
    ISP_PROC_S *pstProc = GK_NULL;

    PROC_GET_CTX(ViPipe, pstProc);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_PROC_PARAM_GET, &pstProc->u32ProcParam);
    if (s32Ret) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] get proc param %x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    return GK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
