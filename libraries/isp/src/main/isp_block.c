/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include "isp_block.h"
#include "mkp_isp.h"
#include "isp_debug.h"
#include "isp_main.h"
#include "isp_ext_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

GK_S32 ISP_GetWorkMode(VI_PIPE ViPipe, ISP_WORKING_MODE_S *pstIspWorkMode)
{
    return GK_SUCCESS;
}

static GK_S32 ISP_GetBlockHwAttr(VI_PIPE ViPipe, ISP_BLOCK_ATTR_S *pstBlock)
{
    GK_S32 s32Ret = GK_FAILURE;
    ISP_BLOCK_ATTR_S stBlkAttr = { 0 };

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_WORK_MODE_INIT, &stBlkAttr);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "s32Ret:%d!\n", s32Ret);
        return s32Ret;
    }

    ISP_CHECK_RUNNING_MODE(stBlkAttr.enIspRunningMode);
    ISP_CHECK_BLOCK_NUM(stBlkAttr.u8BlockNum);

    if (((stBlkAttr.enIspRunningMode == ISP_MODE_RUNNING_OFFLINE ) ||
         (stBlkAttr.enIspRunningMode == ISP_MODE_RUNNING_ONLINE )) &&
        (stBlkAttr.u8BlockNum != ISP_NORMAL_BLOCK_NUM )) {
        ISP_TRACE(MODULE_DBG_ERR, "ViPipe :%d,When enIspRunningMode = %d,u8BlockNum should be equal to %d!\n",
                  ViPipe, stBlkAttr.enIspRunningMode, ISP_NORMAL_BLOCK_NUM);
        return GK_FAILURE;
    } else if (( stBlkAttr.enIspRunningMode == ISP_MODE_RUNNING_SIDEBYSIDE ) &&
               (stBlkAttr.u8BlockNum != ISP_SBS_BLOCK_NUM  )) {
        ISP_TRACE(MODULE_DBG_ERR, "ViPipe :%d,When enIspRunningMode = %d,u8BlockNum should be equal to %d!\n",
                  ViPipe, stBlkAttr.enIspRunningMode, ISP_SBS_BLOCK_NUM);
        return GK_FAILURE;
    } else if ((stBlkAttr.enIspRunningMode == ISP_MODE_RUNNING_STRIPING ) && (stBlkAttr.u8BlockNum < 2)) {
        ISP_TRACE(MODULE_DBG_ERR, "ViPipe :%d,When enIspRunningMode = %d,u8BlockNum should not be less than %d!\n",
                  ViPipe, stBlkAttr.enIspRunningMode, 2);
        return GK_FAILURE;
    }

    pstBlock->enIspRunningMode = stBlkAttr.enIspRunningMode;
    pstBlock->u8BlockNum       = stBlkAttr.u8BlockNum;
    pstBlock->u32OverLap       = stBlkAttr.u32OverLap;

    pstBlock->stFrameRect.u32Width  = stBlkAttr.stFrameRect.u32Width;
    pstBlock->stFrameRect.u32Height = stBlkAttr.stFrameRect.u32Height;

    memcpy(pstBlock->astBlockRect, stBlkAttr.astBlockRect, sizeof(RECT_S) * ISP_STRIPING_MAX_NUM);

    ext_system_be_total_width_write(ViPipe,  pstBlock->stFrameRect.u32Width);
    ext_system_be_total_height_write(ViPipe, pstBlock->stFrameRect.u32Height);

    return GK_SUCCESS;
}

GK_S32 ISP_BlockInit(VI_PIPE ViPipe, ISP_BLOCK_ATTR_S *pstBlock)
{
    GK_S32 s32Ret = GK_FAILURE;

    s32Ret = ISP_GetBlockHwAttr(ViPipe, pstBlock);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "get isp block HW attr failed!\n");
        return s32Ret;
    }

    pstBlock->u8PreBlockNum = pstBlock->u8BlockNum;

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_PRE_BLK_NUM_UPDATE, &pstBlock->u8PreBlockNum);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d]:update pre block num failed\n", ViPipe);
        return s32Ret;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_BlockUpdate(VI_PIPE ViPipe, ISP_BLOCK_ATTR_S *pstBlock)
{
    GK_S32 s32Ret = GK_FAILURE;

    s32Ret = ISP_GetBlockHwAttr(ViPipe, pstBlock);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "get isp block HW attr failed!\n");
        return s32Ret;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_BlockExit(VI_PIPE ViPipe)
{
    GK_S32 s32Ret = GK_FAILURE;

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_WORK_MODE_EXIT);
    if (s32Ret != GK_SUCCESS) {
        return s32Ret;
    }

    return GK_SUCCESS;
}

GK_U32 ISP_GetBlockRect(ISP_RECT_S *pstBlockRect, ISP_BLOCK_ATTR_S *pstBlock, GK_U8 u8BlockId)
{
    ISP_CHECK_BLOCK_ID(u8BlockId);

    if (pstBlock->u8BlockNum == 1) {
        pstBlockRect->s32X      = 0;
        pstBlockRect->s32Y      = 0;
        pstBlockRect->u32Width  = pstBlock->stFrameRect.u32Width;
        pstBlockRect->u32Height = pstBlock->stFrameRect.u32Height;

        return GK_SUCCESS;
    }

    pstBlockRect->s32X      = pstBlock->astBlockRect[u8BlockId].s32X;
    pstBlockRect->s32Y      = pstBlock->astBlockRect[u8BlockId].s32Y;
    pstBlockRect->u32Width  = pstBlock->astBlockRect[u8BlockId].u32Width;
    pstBlockRect->u32Height = pstBlock->astBlockRect[u8BlockId].u32Height;

    return GK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
