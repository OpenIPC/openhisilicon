/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include "osal.h"
#include "mm_ext.h"
#include "isp_drv_vreg.h"
#include "isp_vreg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */


static DRV_VREG_ARGS_S g_stVreg[ISP_MAX_PIPE_NUM][VREG_MAX_NUM] = {0};
DRV_VREG_ARGS_S *VREG_DRV_Search(VI_PIPE ViPipe, GK_U64 u64BaseAddr)
{
    GK_S32  i;

    for (i = 0; i < VREG_MAX_NUM; i++) {
        if ((g_stVreg[ViPipe][i].u64PhyAddr != 0)
            && (u64BaseAddr == g_stVreg[ViPipe][i].u64BaseAddr)) {
            return &g_stVreg[ViPipe][i];
        }
    }

    return GK_NULL;
}

DRV_VREG_ARGS_S *VREG_DRV_Query(GK_U64 u64BaseAddr)
{
    GK_S32  i, j;

    for (j = 0; j < ISP_MAX_PIPE_NUM; j++) {
        for (i = 0; i < VREG_MAX_NUM; i++) {
            if ((g_stVreg[j][i].u64PhyAddr != 0)
                && (u64BaseAddr == g_stVreg[j][i].u64BaseAddr)) {
                return &g_stVreg[j][i];
            }
        }
    }

    return GK_NULL;
}

GK_S32 VREG_DRV_Init(VI_PIPE ViPipe, GK_U64 u64BaseAddr, GK_U64 u64Size)
{
    GK_S32 s32Ret;
    GK_S32  i;
    GK_U64 u64PhyAddr;
    GK_U8 *pu8VirAddr;
    GK_CHAR acName[16] = {0};
    DRV_VREG_ARGS_S *pstVreg = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);

    /* check param */
    if (u64Size == 0) {
        ISP_TRACE(MODULE_DBG_ERR, "The vreg's size is 0!\n");
        return GK_FAILURE;
    }

    pstVreg = VREG_DRV_Search(ViPipe, u64BaseAddr);
    if (pstVreg != GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "The vreg of u64BaseAddr 0x%llx has registerd!\n", u64BaseAddr);
        return GK_FAILURE;
    }

    /* search pos */
    for (i = 0; i < VREG_MAX_NUM; i++) {
        if (g_stVreg[ViPipe][i].u64PhyAddr == 0) {
            pstVreg = &g_stVreg[ViPipe][i];
            break;
        }
    }

    if (pstVreg == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "The vreg is too many, can't register!\n");
        return GK_FAILURE;
    }

    /* Mmz malloc memory */
    osal_snprintf(acName, sizeof(acName), "ISP[%d].Vreg[%d]", ViPipe, i);
    s32Ret = CMPI_MmzMallocNocache(GK_NULL, acName, &u64PhyAddr, (GK_VOID **)&pu8VirAddr, u64Size);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "alloc virt regs buf err\n");
        return GK_FAILURE;
    }

    osal_memset(pu8VirAddr, 0, u64Size);

    pstVreg->u64PhyAddr  = u64PhyAddr;
    pstVreg->pVirtAddr   = (GK_VOID *)pu8VirAddr;
    pstVreg->u64BaseAddr = u64BaseAddr;

    return GK_SUCCESS;
}

GK_S32 VREG_DRV_Exit(VI_PIPE ViPipe, GK_U64 u64BaseAddr)
{
    DRV_VREG_ARGS_S *pstVreg = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);

    pstVreg = VREG_DRV_Search(ViPipe, u64BaseAddr);
    if (pstVreg == GK_NULL) {
        ISP_TRACE(MODULE_DBG_WARN, "The vreg of u64BaseAddr 0x%llx has not registerd!\n", u64BaseAddr);
        return GK_FAILURE;
    }

    if (pstVreg->u64PhyAddr != 0) {
        CMPI_MmzFree(pstVreg->u64PhyAddr, pstVreg->pVirtAddr);
        pstVreg->u64PhyAddr  = 0;
        pstVreg->u64Size     = 0;
        pstVreg->u64BaseAddr = 0;
        pstVreg->pVirtAddr   = GK_NULL;
    }

    return GK_SUCCESS;
}

GK_S32 VREG_DRV_GetAddr(VI_PIPE ViPipe, GK_U64 u64BaseAddr, GK_U64 *pu64PhyAddr)
{
    DRV_VREG_ARGS_S *pstVreg = GK_NULL;

    pstVreg = VREG_DRV_Query(u64BaseAddr);
    if (pstVreg == GK_NULL) {
        ISP_TRACE(MODULE_DBG_WARN, "The vreg of u64BaseAddr 0x%llx has not registerd!\n", u64BaseAddr);
        return GK_FAILURE;
    }

    *pu64PhyAddr = pstVreg->u64PhyAddr;

    return GK_SUCCESS;
}

GK_S32 VREG_DRV_ReleaseAll(VI_PIPE ViPipe)
{
    GK_S32  i;

    ISP_CHECK_PIPE(ViPipe);

    for (i = 0; i < VREG_MAX_NUM; i++) {
        if (g_stVreg[ViPipe][i].u64PhyAddr != 0) {
            CMPI_MmzFree(g_stVreg[ViPipe][i].u64PhyAddr, g_stVreg[ViPipe][i].pVirtAddr);
            g_stVreg[ViPipe][i].u64PhyAddr  = 0;
            g_stVreg[ViPipe][i].u64BaseAddr = 0;
            g_stVreg[ViPipe][i].u64Size     = 0;
            g_stVreg[ViPipe][i].pVirtAddr   = GK_NULL;
        }
    }

    return GK_SUCCESS;
}

long VREG_DRV_ioctl(unsigned int cmd, unsigned long arg, void *private_data)
{
    VI_PIPE ViPipe;
    unsigned int *argp = (unsigned int *)(GK_UINTPTR_T)arg;

    ViPipe = ISP_GET_DEV(private_data);

    switch (cmd) {
        case VREG_DRV_FD : {
            ISP_CHECK_POINTER(arg);
            *((GK_U32 *)(private_data)) = *(GK_U32 *)argp;

            return GK_SUCCESS;
        }
        /* malloc memory for vregs, and record information in kernel. */
        case VREG_DRV_INIT : {
            DRV_VREG_ARGS_S *pstVreg = GK_NULL;
            ISP_CHECK_POINTER(argp);
            pstVreg = (DRV_VREG_ARGS_S *)argp;

            return VREG_DRV_Init(ViPipe, pstVreg->u64BaseAddr, pstVreg->u64Size);
        }
        /* free the memory of vregs, and clean information in kernel. */
        case VREG_DRV_EXIT : {
            DRV_VREG_ARGS_S *pstVreg = GK_NULL;
            ISP_CHECK_POINTER(argp);
            pstVreg = (DRV_VREG_ARGS_S *)argp;

            return VREG_DRV_Exit(ViPipe, pstVreg->u64BaseAddr);
        }
        /* free the memory of vregs, and clean information in kernel. */
        case VREG_DRV_RELEASE_ALL : {
            DRV_VREG_ARGS_S *pstVreg = GK_NULL;
            ISP_CHECK_POINTER(argp);
            pstVreg = (DRV_VREG_ARGS_S *)argp;

            if ((pstVreg->u64Size == ISP_VREG_SIZE) && \
                (pstVreg->u64BaseAddr == ISP_VREG_BASE)) {
                VREG_DRV_ReleaseAll(ViPipe);
            }

            return GK_SUCCESS;
        }
        /* get the mapping relation between vreg addr and physical addr. */
        case VREG_DRV_GETADDR : {
            DRV_VREG_ARGS_S *pstVreg = GK_NULL;
            ISP_CHECK_POINTER(argp);
            pstVreg = (DRV_VREG_ARGS_S *)argp;

            return VREG_DRV_GetAddr(ViPipe, pstVreg->u64BaseAddr, &pstVreg->u64PhyAddr);
        }
        default: {
            return GK_FAILURE;
        }
    }

    return GK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
