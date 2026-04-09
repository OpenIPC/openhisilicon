/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/


#include "hi_osal.h"
#include "hi_common.h"
#include "hi_comm_isp.h"
#include "isp_drv_defines.h"
#include "mkp_isp.h"
#include "isp.h"
#include "isp_drv.h"
#include "isp_list.h"
#include "isp_drv_vreg.h"
#include "mm_ext.h"
#include "proc_ext.h"
#include "mod_ext.h"
#include "sys_ext.h"
#include "isp_ext.h"
#include "dev_ext.h"
#include "isp_reg_define.h"
#include "isp_stt_define.h"
#include "hi_i2c.h"
#include "hi_spi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */


/* ***************************************************************************
 * MACRO DEFINITION                                                         *
 *************************************************************************** */

int  ISP_IrqRoute(VI_PIPE ViPipe);
int  ISP_DoIntBottomHalf(int irq, void *id);


/* ***************************************************************************
 * GLOBAL VARIABLES                                                         *
 *************************************************************************** */
ISP_DRV_CTX_S           g_astIspDrvCtx[ISP_MAX_PIPE_NUM] = {{0}};

unsigned int            isp_fe_irq = ISP_FE_IRQ_NR;

osal_spinlock_t         g_stIspLock[ISP_MAX_PIPE_NUM];
osal_spinlock_t         g_stIspSyncLock[ISP_MAX_PIPE_NUM];


static ISP_VERSION_S    gs_stIspLibInfo;


/* ISP ModParam info */
HI_U32  g_PwmNumber[ISP_MAX_PIPE_NUM]  = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = 3 };
HI_U32  g_UpdatePos[ISP_MAX_PIPE_NUM]  = { 0 }; /* 0: frame start; 1: frame end */

/* The time(unit:ms) of interrupt timeout */
HI_U32  g_IntTimeout[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = 200 };

/* update isp statistic information per stat-intval frame, purpose to reduce CPU load */
HI_U32  g_StatIntvl[ISP_MAX_PIPE_NUM]  = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = 1 };

/* 0: close proc; n: write proc info's interval int num */
HI_U32  g_ProcParam[ISP_MAX_PIPE_NUM]  = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = 30 };
HI_U32  g_PortIntDelay[ISP_MAX_PIPE_NUM] = { 0 }; /* Port intertupt delay value */
HI_BOOL g_IntBothalf = HI_FALSE;  /* 1 to enable interrupt processing at bottom half */
HI_BOOL g_QuickStart = HI_FALSE;  /* 1 to enable interrupt processing at bottom half */

HI_BOOL g_UseBothalf = HI_FALSE;  /* 1 to use interrupt processing at bottom half */
HI_U32  g_IspExitTimeout = 2000;  /* The time(unit:ms) of exit be buffer timeout */
HI_U16  g_au16ResMaxWidth[ISP_MAX_PIPE_NUM]  = { ISP_RES_WIDTH_MAX };
HI_U16  g_au16ResMaxHeight[ISP_MAX_PIPE_NUM] = { ISP_RES_HEIGHT_MAX };

/* ldci temporal filter enable */
HI_BOOL  g_LdciTprFltEn[ISP_MAX_PIPE_NUM]  = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = HI_FALSE };

// #define TEST_TIME

#ifdef TEST_TIME
HI_U32  g_test_phyaddr;
HI_U16 *g_test_pviraddr;
struct osal_timeval time1;
struct osal_timeval time2;
#endif


/****************************************************************************
 * INTERNAL FUNCTION DECLARATION                                            *
 ****************************************************************************/

HI_S32 ISP_DRV_SetIntEnable(VI_PIPE ViPipe, HI_BOOL bEn)
{
    HI_S32 ViDev;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    ViDev = pstDrvCtx->stWDRAttr.vi_dev;

    ISP_CHECK_VIR_PIPE(ViPipe);

    if (bEn) {
        if (IS_FULL_WDR_MODE(pstDrvCtx->stWDRCfg.wdr_mode) ||
            IS_HALF_WDR_MODE(pstDrvCtx->stWDRCfg.wdr_mode)) {
            IO_RW_PT_ADDRESS(VI_PT_BASE(ViDev) + VI_PT_INT_MASK) |= VI_PT_INT_FSTART;
            IO_RW_PT_ADDRESS(VI_PT_BASE(ViDev) + VI_PT_FSTART_DLY) = g_PortIntDelay[ViPipe];
        }
        IO_RW_PT_ADDRESS(VI_PT_BASE(ViDev) + VI_PT_INT_MASK) |= VI_PT_INT_ERR;

        IO_RW_FE_ADDRESS(ViPipe, ISP_INT_FE) = 0xFF;
        IO_RW_FE_ADDRESS(ViPipe, ISP_INT_FE_MASK) |= ISP_INT_FE_FSTART;
        IO_RW_FE_ADDRESS(ViPipe, ISP_INT_FE_MASK) |= ISP_INT_FE_FEND;
    } else {
        if (IS_FULL_WDR_MODE(pstDrvCtx->stWDRCfg.wdr_mode) ||
            IS_HALF_WDR_MODE(pstDrvCtx->stWDRCfg.wdr_mode)) {
            IO_RW_PT_ADDRESS(VI_PT_BASE(ViDev) + VI_PT_INT_MASK) &= (~VI_PT_INT_FSTART);
            IO_RW_PT_ADDRESS(VI_PT_BASE(ViDev) + VI_PT_FSTART_DLY) = 0;
        }
        IO_RW_PT_ADDRESS(VI_PT_BASE(ViDev) + VI_PT_INT) = 0xF;
        IO_RW_PT_ADDRESS(VI_PT_BASE(ViDev) + VI_PT_INT_MASK) &= (~VI_PT_INT_ERR);

        IO_RW_FE_ADDRESS(ViPipe, ISP_INT_FE_MASK) &= (~ISP_INT_FE_FSTART);
        IO_RW_FE_ADDRESS(ViPipe, ISP_INT_FE_MASK) &= (~ISP_INT_FE_FEND);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_WaitConditionCallback(const HI_VOID *pParam)
{
    HI_BOOL bCondition;

    bCondition = *(HI_BOOL *)pParam;

    return (bCondition == HI_TRUE);
}

HI_S32 ISP_DRV_WaitExitCallback(const HI_VOID *pParam)
{
    HI_S32 bCondition;

    bCondition = *(HI_S32 *)pParam;

    return (bCondition == 0);
}

HI_S32 ISP_GetFrameEdge(VI_PIPE ViPipe, HI_U32 *pu32Status)
{
    unsigned long u32Flags;
    HI_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pu32Status);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    s32Ret = osal_wait_event_timeout_interruptible(&pstDrvCtx->stIspWait, ISP_DRV_WaitConditionCallback, \
                                                   &pstDrvCtx->bEdge, g_IntTimeout[ViPipe]);

    if (s32Ret <= 0) {
        *pu32Status = 0;
        ISP_WARN_TRACE("Get Interrupt timeout failed!\n");
        return HI_FAILURE;
    }

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstDrvCtx->bEdge = HI_FALSE;
    *pu32Status = pstDrvCtx->u32Status;
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

static int ISP_GetVdStartTimeOut(VI_PIPE ViPipe, HI_U32 u32MilliSec, HI_U32 *pu32status)
{
    unsigned long u32Flags;
    HI_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (osal_down_interruptible(&pstDrvCtx->stIspSem)) {
        return -ERESTARTSYS;
    }

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstDrvCtx->bVdStart = HI_FALSE;
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    if (u32MilliSec != 0) {
        s32Ret = osal_wait_event_timeout_interruptible(&pstDrvCtx->stIspWaitVdStart, ISP_DRV_WaitConditionCallback, \
                                                       &pstDrvCtx->bVdStart, (u32MilliSec));

        if (s32Ret <= 0) {
            osal_up(&pstDrvCtx->stIspSem);
            return HI_FAILURE;
        }
    } else {
        s32Ret = osal_wait_event_interruptible(&pstDrvCtx->stIspWaitVdStart, ISP_DRV_WaitConditionCallback, \
                                               &pstDrvCtx->bVdStart);

        if (s32Ret) {
            osal_up(&pstDrvCtx->stIspSem);
            return HI_FAILURE;
        }
    }

    *pu32status = pstDrvCtx->u32Status;

    osal_up(&pstDrvCtx->stIspSem);

    return HI_SUCCESS;
}

static int ISP_GetVdEndTimeOut(VI_PIPE ViPipe, HI_U32 u32MilliSec, HI_U32 *pu32status)
{
    unsigned long u32Flags;
    HI_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (osal_down_interruptible(&pstDrvCtx->stIspSemVd)) {
        return -ERESTARTSYS;
    }

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstDrvCtx->bVdEnd = HI_FALSE;
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    if (u32MilliSec != 0) {
        s32Ret = osal_wait_event_timeout_interruptible(&pstDrvCtx->stIspWaitVdEnd, ISP_DRV_WaitConditionCallback, \
                                                       &pstDrvCtx->bVdEnd, (u32MilliSec));

        if (s32Ret <= 0) {
            osal_up(&pstDrvCtx->stIspSemVd);
            return HI_FAILURE;
        }
    } else {
        s32Ret = osal_wait_event_interruptible(&pstDrvCtx->stIspWaitVdEnd, ISP_DRV_WaitConditionCallback, \
                                               &pstDrvCtx->bVdEnd);

        if (s32Ret) {
            osal_up(&pstDrvCtx->stIspSemVd);
            return HI_FAILURE;
        }
    }

    *pu32status = pstDrvCtx->u32Status;

    osal_up(&pstDrvCtx->stIspSemVd);

    return HI_SUCCESS;
}

static int ISP_GetVdBeEndTimeOut(VI_PIPE ViPipe, HI_U32 u32MilliSec, HI_U32 *pu32status)
{
    unsigned long u32Flags;
    HI_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (osal_down_interruptible(&pstDrvCtx->stIspSemBeVd)) {
        return -ERESTARTSYS;
    }

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstDrvCtx->bVdBeEnd = HI_FALSE;
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    if (u32MilliSec != 0) {
        s32Ret = osal_wait_event_timeout_interruptible(&pstDrvCtx->stIspWaitVdBeEnd, ISP_DRV_WaitConditionCallback, \
                                                       &pstDrvCtx->bVdBeEnd, (u32MilliSec));

        if (s32Ret <= 0) {
            osal_up(&pstDrvCtx->stIspSemBeVd);
            return HI_FAILURE;
        }
    } else {
        s32Ret = osal_wait_event_interruptible(&pstDrvCtx->stIspWaitVdBeEnd, ISP_DRV_WaitConditionCallback, \
                                               &pstDrvCtx->bVdBeEnd);

        if (s32Ret) {
            osal_up(&pstDrvCtx->stIspSemBeVd);
            return HI_FAILURE;
        }
    }

    *pu32status = 1;

    osal_up(&pstDrvCtx->stIspSemBeVd);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_WorkModeInit(VI_PIPE ViPipe, isp_block_attr  *pstBlkAttr)
{
    HI_S32  s32Ret = HI_SUCCESS;
    unsigned long u32Flags;
    isp_running_mode enIspRunningMode = ISP_MODE_RUNNING_OFFLINE;
    vi_pipe_split_attr stPipeSplitAttr;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    hi_frame_interrupt_attr stFrameIntAttr = { 0 };

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstBlkAttr);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    pstDrvCtx->stWorkMode.pre_block_num = pstDrvCtx->stWorkMode.block_num;

    osal_memset(&stPipeSplitAttr, 0, sizeof(vi_pipe_split_attr));

    if (ckfn_vi_get_split_attr()) {
        s32Ret = call_vi_get_split_attr(ViPipe, &stPipeSplitAttr);

        if (s32Ret != HI_SUCCESS) {
            ISP_ERR_TRACE("Pipe:%d call_vi_get_split_attr failed 0x%x!\n", ViPipe, s32Ret);
            return s32Ret;
        }
    }

    ISP_CHECK_BLOCK_NUM(stPipeSplitAttr.split_num);

    if ((stPipeSplitAttr.mode == VI_PARALLEL_VPSS_OFFLINE) ||
        (stPipeSplitAttr.mode == VI_PARALLEL_VPSS_PARALLEL)) {
        enIspRunningMode = ISP_MODE_RUNNING_SIDEBYSIDE;
    } else if ((stPipeSplitAttr.mode == VI_ONLINE_VPSS_OFFLINE) ||
               (stPipeSplitAttr.mode == VI_ONLINE_VPSS_ONLINE)) {
        enIspRunningMode = ISP_MODE_RUNNING_ONLINE;
    } else if ((stPipeSplitAttr.mode == VI_OFFLINE_VPSS_OFFLINE) ||
               (stPipeSplitAttr.mode == VI_OFFLINE_VPSS_ONLINE)) {
        if (stPipeSplitAttr.split_num == 1) {
            enIspRunningMode = ISP_MODE_RUNNING_OFFLINE;
        } else {
            enIspRunningMode = ISP_MODE_RUNNING_STRIPING;
        }
    } else {
        ISP_ERR_TRACE("Pipe:%d Not support Mode: %d!\n", ViPipe, stPipeSplitAttr.mode);
        return HI_FAILURE;
    }

    if ((stPipeSplitAttr.pix_fmt == PIXEL_FORMAT_YUV_SEMIPLANAR_420) ||
        (stPipeSplitAttr.pix_fmt == PIXEL_FORMAT_YVU_SEMIPLANAR_420) ||
        (stPipeSplitAttr.pix_fmt == PIXEL_FORMAT_YUV_SEMIPLANAR_422) ||
        (stPipeSplitAttr.pix_fmt == PIXEL_FORMAT_YVU_SEMIPLANAR_422) ||
        (stPipeSplitAttr.pix_fmt == PIXEL_FORMAT_YUV_SEMIPLANAR_444) ||
        (stPipeSplitAttr.pix_fmt == PIXEL_FORMAT_YVU_SEMIPLANAR_444)) {
        pstDrvCtx->bYuvMode = HI_TRUE;
        pstDrvCtx->stWorkMode.data_input_mode = ISP_MODE_BT1120_YUV;
    } else {
        pstDrvCtx->bYuvMode = HI_FALSE;
        pstDrvCtx->stWorkMode.data_input_mode = ISP_MODE_RAW;
    }
    if (ckfn_vi_get_vi_frame_interrupt_attr()) {
        s32Ret = call_vi_get_vi_frame_interrupt_attr(ViPipe, &stFrameIntAttr);

        if (s32Ret != HI_SUCCESS) {
            ISP_ERR_TRACE("Pipe:%d Get Vi Int Attr err 0x%x!\n", ViPipe, s32Ret);
            return s32Ret;
        }
    }

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    pstDrvCtx->stWorkMode.block_num    = stPipeSplitAttr.split_num;
    pstDrvCtx->stWorkMode.running_mode = enIspRunningMode;
    pstDrvCtx->stWorkMode.over_lap     = stPipeSplitAttr.over_lap;

    pstBlkAttr->block_num    = stPipeSplitAttr.split_num;
    pstBlkAttr->over_lap     = stPipeSplitAttr.over_lap;
    pstBlkAttr->running_mode = enIspRunningMode;

    pstDrvCtx->stWorkMode.frame_rect.width  = stPipeSplitAttr.wch_out_rect.width;
    pstDrvCtx->stWorkMode.frame_rect.height = stPipeSplitAttr.wch_out_rect.height;

    osal_memcpy(&pstBlkAttr->frame_rect, &pstDrvCtx->stWorkMode.frame_rect, sizeof(hi_size));
    osal_memcpy(pstDrvCtx->stWorkMode.block_rect, stPipeSplitAttr.rect, sizeof(hi_rect) * ISP_STRIPING_MAX_NUM);
    osal_memcpy(pstBlkAttr->block_rect, stPipeSplitAttr.rect, sizeof(hi_rect) * ISP_STRIPING_MAX_NUM);
    pstDrvCtx->stFrameIntAttr = stFrameIntAttr;

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_WorkModeExit(VI_PIPE ViPipe)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_EXIT_STATE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    pstDrvCtx->stWorkMode.running_mode = ISP_MODE_RUNNING_OFFLINE;

    return HI_SUCCESS;
}

HI_U8 ISP_DRV_GetBlockNum(VI_PIPE ViPipe)
{
    return g_astIspDrvCtx[ViPipe].stWorkMode.block_num;
}

HI_S8 ISP_DRV_GetBlockId(VI_PIPE ViPipe, isp_running_mode enRunningMode)
{
    HI_S8 s8BlockId = 0;

    switch (enRunningMode) {
        default:
        case ISP_MODE_RUNNING_OFFLINE:
        case ISP_MODE_RUNNING_SIDEBYSIDE:
        case ISP_MODE_RUNNING_STRIPING:
            s8BlockId = 0;
            break;
        case ISP_MODE_RUNNING_ONLINE:
            switch (ViPipe) {
                case ISP_BE0_PIPE_ID:
                    s8BlockId = 0;
                    break;
                case ISP_BE1_PIPE_ID:
                    s8BlockId = 1;
                    break;
                default:
                    return HI_FAILURE;
            }
            break;
    }

    return s8BlockId;
}

HI_S32 ISP_DRV_GetBeRegsAttr(VI_PIPE ViPipe, S_ISPBE_REGS_TYPE *apstBeReg[], isp_be_regs_attr *pstBlkAttr)
{
    HI_U8  k = 0;
    HI_S8  s8BlockId, u8BlkDev, u8BlkNum;
    isp_running_mode enIspRunningMode;
    isp_be_wo_reg_cfg *pstIspBeRegCfg = HI_NULL;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstBlkAttr);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    enIspRunningMode = pstDrvCtx->stWorkMode.running_mode;
    u8BlkNum = ISP_DRV_GetBlockNum(ViPipe);
    u8BlkNum = DIV_0_TO_1(u8BlkNum);
    s8BlockId = ISP_DRV_GetBlockId(ViPipe, enIspRunningMode);
    if (s8BlockId == -1) {
        ISP_ERR_TRACE("ISP[%d] Online Mode Pipe Err!\n", ViPipe);
        return HI_FAILURE;
    }

    u8BlkDev = (HI_U8)s8BlockId;
    pstBlkAttr->block_dev = u8BlkDev;
    pstBlkAttr->block_num = u8BlkNum;

    switch (enIspRunningMode) {
        case ISP_MODE_RUNNING_ONLINE:
            ISP_DRV_BEREG_CTX(u8BlkDev, apstBeReg[u8BlkDev]);
            break;
        case ISP_MODE_RUNNING_OFFLINE:
            ISP_CHECK_BEBUF_INIT(ViPipe);
            if (pstDrvCtx->pstUseNode == HI_NULL) {
                ISP_WARN_TRACE("ISP[%d] UseNode is Null!\n", ViPipe);
                return HI_FAILURE;
            }
            pstIspBeRegCfg = (isp_be_wo_reg_cfg *)pstDrvCtx->pstUseNode->stBeCfgBuf.vir_addr;
            apstBeReg[u8BlkDev] = &pstIspBeRegCfg->be_reg_cfg[u8BlkDev];
            break;
        case ISP_MODE_RUNNING_SIDEBYSIDE:
            for (k = 0; k < ISP_MAX_BE_NUM; k++) {
                ISP_DRV_BEREG_CTX(k, apstBeReg[k]);
            }
            break;
        case ISP_MODE_RUNNING_STRIPING:
            ISP_CHECK_BEBUF_INIT(ViPipe);
            if (pstDrvCtx->pstUseNode == HI_NULL) {
                ISP_WARN_TRACE("ISP[%d] UseNode is Null!\n", ViPipe);
                return HI_FAILURE;
            }
            pstIspBeRegCfg = (isp_be_wo_reg_cfg *)pstDrvCtx->pstUseNode->stBeCfgBuf.vir_addr;
            for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
                apstBeReg[k] = &pstIspBeRegCfg->be_reg_cfg[k];
            }
            break;
        default:
            ISP_ERR_TRACE("ISP[%d] Running Mode Err!\n", ViPipe);
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_ChnSelectWrite(VI_PIPE ViPipe, HI_U32 u32ChannelSel)
{
    HI_U8   k;
    HI_U8   u8BlkDev, u8BlkNum;
    HI_U32  au32ChnSwitch[5] = { 0 };
    HI_U32  s32Ret;
    isp_be_regs_attr stIspBeRegsAttr = { 0 };
    S_ISPBE_REGS_TYPE *apstBeReg[ISP_STRIPING_MAX_NUM] = { HI_NULL };
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_PIPE(ViPipe);

    s32Ret = ISP_DRV_GetBeRegsAttr(ViPipe, apstBeReg, &stIspBeRegsAttr);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] Get BeRegs Attr Err!\n", ViPipe);
        return HI_FAILURE;
    }

    if (pstDrvCtx->bYuvMode == HI_TRUE) {
        au32ChnSwitch[4] = 1;
    } else {
        au32ChnSwitch[4] = 0;
    }

    u8BlkDev = stIspBeRegsAttr.block_dev;
    u8BlkNum = stIspBeRegsAttr.block_num;

    switch (u32ChannelSel & 0x3) {
        case 0:
            au32ChnSwitch[0] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[0];
            au32ChnSwitch[1] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[1];
            au32ChnSwitch[2] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[2];
            au32ChnSwitch[3] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[3];
            break;

        case 1:
            au32ChnSwitch[1] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[0];
            au32ChnSwitch[0] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[1];
            au32ChnSwitch[2] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[2];
            au32ChnSwitch[3] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[3];
            break;

        case 2:
            au32ChnSwitch[2] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[0];
            au32ChnSwitch[1] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[1];
            au32ChnSwitch[0] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[2];
            au32ChnSwitch[3] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[3];
            break;

        case 3:
            au32ChnSwitch[3] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[0];
            au32ChnSwitch[2] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[1];
            au32ChnSwitch[1] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[2];
            au32ChnSwitch[0] = pstDrvCtx->astChnSelAttr[0].wdr_chn_sel[3];
            break;
    }

    if (IS_FS_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)) {
        for (k = 0; k < u8BlkNum; k++) {
            ISP_DRV_SetInputSel(apstBeReg[k + u8BlkDev], &au32ChnSwitch[0]);
        }
    }

    return HI_SUCCESS;
}

/* ISP FE read sta */
HI_S32 ISP_DRV_FE_StatisticsRead(VI_PIPE ViPipe, isp_stat_info *pstStatInfo)
{
    HI_U8          u8ChnNumMax;
    HI_U32         k;
    VI_PIPE        ViPipeBind;
    isp_stat_key unStatkey;
    ISP_DRV_CTX_S     *pstDrvCtx = HI_NULL;
    isp_stat        *pstStat   = HI_NULL;
    S_ISPFE_REGS_TYPE *pstFeReg  = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_VIR_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstStatInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    if (pstDrvCtx->stWDRAttr.mast_pipe == HI_FALSE) {
        return HI_SUCCESS;
    }

    pstStat = (isp_stat *)pstStatInfo->virt_addr;
    if (pstStat == HI_NULL) {
        return HI_FAILURE;
    }

    unStatkey.key = pstStatInfo->stat_key.bit32IsrAccess;

    u8ChnNumMax = CLIP3(pstDrvCtx->stWDRAttr.dev_bind_pipe.num, 1, ISP_WDR_CHN_MAX);
    
    for (k = 0; k < u8ChnNumMax; k++) {
        /* get side statistics */
        ViPipeBind = pstDrvCtx->stWDRAttr.dev_bind_pipe.pipe_id[k];
        ISP_CHECK_PIPE(ViPipeBind);
        ISP_DRV_FEREG_CTX(ViPipeBind, pstFeReg);
        ISP_DRV_FE_APB_StatisticsRead(pstStat, pstFeReg, pstDrvCtx, ViPipeBind, k, unStatkey);
        ISP_DRV_FE_STT_StatisticsRead(pstStat, pstFeReg, pstDrvCtx, ViPipeBind, k, unStatkey);
    }

    return HI_SUCCESS;
}

/* ISP BE read sta from FHY, online mode */
HI_S32 ISP_DRV_BeStatisticsRead(VI_PIPE ViPipe, isp_stat_info *pstStatInfo)
{
    HI_S32 s32Ret;
    HI_U8  u8BlkDev, u8BlkNum;
    isp_stat *pstStat = HI_NULL;

    isp_stat_key unStatkey;
    isp_be_regs_attr stIspBeRegsAttr = { 0 };
    S_ISPBE_REGS_TYPE *apstBeReg[ISP_STRIPING_MAX_NUM] = { HI_NULL };

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstStatInfo);

    s32Ret = ISP_DRV_GetBeRegsAttr(ViPipe, apstBeReg, &stIspBeRegsAttr);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    u8BlkDev = stIspBeRegsAttr.block_dev;
    u8BlkNum = stIspBeRegsAttr.block_num;

    pstStat = (isp_stat *)pstStatInfo->virt_addr;

    if (pstStat == HI_NULL) {
        return HI_FAILURE;
    }

    unStatkey.key = pstStatInfo->stat_key.bit32IsrAccess;

    pstStat->be_update = HI_TRUE;

    ISP_DRV_BE_APB_StatisticsRead(ViPipe, pstStat, apstBeReg, u8BlkNum, u8BlkDev, unStatkey);
    ISP_DRV_BE_STT_StatisticsRead(ViPipe, pstStat, apstBeReg, u8BlkNum, u8BlkDev, unStatkey);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_BeOfflineStitchStatisticsRead(VI_PIPE ViPipe, isp_stat_info *pstStatInfo)
{
    HI_S32 k = 0;
    HI_S32 s32Ret;
    HI_U8 u8BlkDev;

    isp_stat_key unStatkey;
    isp_stat *pstStat = HI_NULL;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    isp_stitch_sync_be_stats stBeStitch;
    isp_be_regs_attr  stIspBeRegsAttr = { 0 };
    S_ISPBE_REGS_TYPE   *apstBeReg[ISP_STRIPING_MAX_NUM] = { HI_NULL };

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstStatInfo);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    unStatkey.key = pstStatInfo->stat_key.bit32IsrAccess;
    s32Ret = ISP_DRV_GetBeRegsAttr(ViPipe, apstBeReg, &stIspBeRegsAttr);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    u8BlkDev = stIspBeRegsAttr.block_dev;

    if ((pstDrvCtx->stStitchAttr.main_pipe != HI_TRUE) && (pstDrvCtx->stStitchAttr.stitch_enable != HI_TRUE)) {
        return  HI_SUCCESS;
    }

    pstStat = (isp_stat *)pstStatInfo->virt_addr;

    if (pstStat == HI_NULL) {
        return HI_FAILURE;
    }

    for (k = 0; k < pstDrvCtx->stStitchAttr.stitch_pipe_num; k++) {
        stBeStitch.sync_be_stt[k] = \
            (S_ISP_STITCH_STT_REGS_TYPE *)pstDrvCtx->stBeOffStitSttAttr.be_stitch_buf[k].vir_addr;

        if (stBeStitch.sync_be_stt[k] == HI_NULL) {
            ISP_ERR_TRACE("Pipe[%d] apstBeStt is NULL point\n", ViPipe);
            return HI_FAILURE;
        }
    }

    /* VI   CallBack Function */
    if (ckfn_vi_get_stitch_sync_be_stt_info()) {
        s32Ret = call_vi_get_stitch_sync_be_stt_info(ViPipe, &stBeStitch);

        if (s32Ret != HI_SUCCESS) {
            pstStat->be_update = HI_FALSE;
            ISP_WARN_TRACE("Pipe[%d] call_vi_get_stitch_sync_be_stt_info err 0x%x\n", ViPipe, s32Ret);
            return HI_FAILURE;
        }

        pstStat->be_update = HI_TRUE;
    } else {
        pstStat->be_update = HI_FALSE;
        ISP_ERR_TRACE("Pipe[%d] vi_get_stitch_sync_be_stt_info is NULL\n", ViPipe);
        return HI_FAILURE;
    }

    if (pstStat->be_update == HI_FALSE) {
        pstDrvCtx->stDrvDbgInfo.u32IspBeStaLost++;
    }

    if (unStatkey.bit1BEAeStiGloStat) {
        s32Ret = ISP_DRV_BeOfflineAEStitchGlobalStatisticsRead(pstStat, pstDrvCtx);
    }

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("Pipe[%d] ISP_DRV_BeOfflineAEStitchGlobalStatisticsRead err 0x%x\n", ViPipe, s32Ret);
        return HI_FAILURE;
    }

    if (unStatkey.bit1BEAeStiLocStat) {
        s32Ret = ISP_DRV_BeOfflineAEStitchLocalStatisticsRead(pstStat, pstDrvCtx);
    }

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("Pipe[%d] ISP_DRV_BeOfflineAEStitchLocalStatisticsRead err 0x%x\n", ViPipe, s32Ret);
        return HI_FAILURE;
    }

    /* BE AWB statistics */
    s32Ret = ISP_DRV_BeOfflineAWBStitchStatisticsRead(pstStat, apstBeReg, pstDrvCtx, u8BlkDev, unStatkey);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("Pipe[%d] ISP_DRV_BeOfflineAWBStitchStatisticsRead err 0x%x\n", ViPipe, s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/* ISP BE read sta from VI, offline mode */
HI_S32 ISP_DRV_BeOfflineStatisticsRead(VI_PIPE ViPipe, isp_stat_info *pstStatInfo)
{
    HI_S32 k;
    HI_S32 s32Ret;
    HI_U8  u8BlkNum, u8BlkDev;
    HI_S8  s8BlockId = 0;
    isp_stat_key unStatkey;
    isp_stat *pstStat = HI_NULL;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    S_ISP_STT_REGS_TYPE *apstBeStt[ISP_STRIPING_MAX_NUM] = { [0 ...(ISP_STRIPING_MAX_NUM - 1)] = HI_NULL };
    isp_running_mode enIspRunningMode;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstStatInfo);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    u8BlkNum = ISP_DRV_GetBlockNum(ViPipe);
    u8BlkNum = DIV_0_TO_1(u8BlkNum);

    enIspRunningMode = pstDrvCtx->stWorkMode.running_mode;

    s8BlockId = ISP_DRV_GetBlockId(ViPipe, enIspRunningMode);

    if (s8BlockId == -1) {
        ISP_ERR_TRACE("ISP[%d] Online Mode Pipe Err!\n", ViPipe);
        return HI_FAILURE;
    }

    u8BlkDev = (HI_U8)s8BlockId;

    pstStat = (isp_stat *)pstStatInfo->virt_addr;

    if (pstStat == HI_NULL) {
        return HI_FAILURE;
    }

    for (k = 0; k < u8BlkNum; k++) {
        apstBeStt[k] = (S_ISP_STT_REGS_TYPE *)pstDrvCtx->stBeOffSttAttr.be_stt_buf[k].vir_addr;
        if (apstBeStt[k] == HI_NULL) {
            ISP_ERR_TRACE("apstBeStt is NULL point\n");
            return HI_FAILURE;
        }

        if (ckfn_vi_get_be_stt_info()) {
            s32Ret = call_vi_get_be_stt_info(ViPipe, k, apstBeStt[k]);
            if (s32Ret != HI_SUCCESS) {
                pstStat->be_update = HI_FALSE;
                ISP_ERR_TRACE("Pipe[%d] call_vi_get_be_stt_info err 0x%x\n", ViPipe, s32Ret);
                return HI_FAILURE;
            }

            pstStat->be_update = HI_TRUE;
        } else {
            pstStat->be_update = HI_FALSE;
            ISP_ERR_TRACE("Pipe[%d] vi_get_be_stt_info is NULL\n", ViPipe);
            return HI_FAILURE;
        }
    }

    if (pstStat->be_update == HI_FALSE) {
        pstDrvCtx->stDrvDbgInfo.u32IspBeStaLost++;
    }

    unStatkey.key = pstStatInfo->stat_key.bit32IsrAccess;
    /* BE comm statistics */
    if (unStatkey.bit1CommStat) {
    }

    /* BE AE statistics */
    if (unStatkey.bit1BEAeGloStat) {
        ISP_DRV_BE_AE_Global_OfflineStatisticsRead(pstStat, apstBeStt, u8BlkNum);
    }

    if (unStatkey.bit1BEAeLocStat) {
        ISP_DRV_BE_AE_Local_OfflineStatisticsRead(pstStat, apstBeStt, u8BlkNum);
    }

    if (unStatkey.bit1MgStat) {
        ISP_DRV_BE_MG_OfflineStatisticsRead(pstStat, apstBeStt, u8BlkNum);
    }

    /* BE AWB statistics */
    ISP_DRV_BE_AWB_OfflineStatisticsRead(ViPipe, pstStat,  apstBeStt, u8BlkNum, u8BlkDev, unStatkey);

    /* BE AF statistics */
    if (unStatkey.bit1BEAfStat) {
        ISP_DRV_BE_AF_OfflineStatisticsRead(ViPipe, pstStat, apstBeStt, u8BlkNum);
    }

    if (unStatkey.bit1Dehaze) {
        ISP_DRV_BE_Dehaze_OfflineStatisticsRead(pstStat, apstBeStt, u8BlkNum);
    }

    if (unStatkey.bit1DpStat) {
        ISP_DRV_DPC_OfflineCalibInfoRead(pstStat, apstBeStt, u8BlkNum);
    }

    if (unStatkey.bit1Flicker) {
        ISP_DRV_Flicker_OfflineStatisticsRead(pstStat, apstBeStt, u8BlkNum);
    }

    if (g_LdciTprFltEn[ViPipe] == HI_TRUE) {
        ISP_DRV_LDCI_OfflineStatisticsRead(ViPipe, pstStat);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_StatisticsRead(VI_PIPE ViPipe, isp_stat_info *pstStatInfo)
{
    HI_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstStatInfo);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    /* online snap, AE and AWB params set by the preview pipe.
      In order to get picture as fast as, dehaze don't used. */
    if (IS_ONLINE_MODE(pstDrvCtx->stWorkMode.running_mode) || \
        IS_SIDEBYSIDE_MODE(pstDrvCtx->stWorkMode.running_mode)) {
        if ((pstDrvCtx->stSnapAttr.picture_pipe_id == ViPipe) &&
            (pstDrvCtx->stSnapAttr.picture_pipe_id != pstDrvCtx->stSnapAttr.preview_pipe_id)) {
            return HI_SUCCESS;
        }
    }

    if (pstDrvCtx->stStitchAttr.stitch_enable == HI_TRUE) {
        if (pstDrvCtx->stStitchAttr.main_pipe == HI_TRUE) {
            ISP_DRV_FE_StitchStatisticsRead(ViPipe, pstStatInfo);
        }

        ISP_DRV_FE_StitchNonStatisticsRead(ViPipe, pstStatInfo);
    } else {
        s32Ret = ISP_DRV_FE_StatisticsRead(ViPipe, pstStatInfo);

        if (s32Ret != HI_SUCCESS) {
            ISP_ERR_TRACE("ISP_DRV_FE_StatisticsRead failed!\n");
            return HI_FAILURE;
        }
    }

    if (IS_ONLINE_MODE(pstDrvCtx->stWorkMode.running_mode) || \
        IS_SIDEBYSIDE_MODE(pstDrvCtx->stWorkMode.running_mode)) {
        // BE statistics for online
        s32Ret = ISP_DRV_BeStatisticsRead(ViPipe, pstStatInfo);
        if (s32Ret != HI_SUCCESS) {
            ISP_ERR_TRACE("ISP_DRV_BeStatisticsRead failed!\n");
            return HI_FAILURE;
        }

        if ((pstDrvCtx->stStitchAttr.main_pipe == HI_TRUE) && (pstDrvCtx->stStitchAttr.stitch_enable == HI_TRUE)) {
            ISP_DRV_BE_StitchStatisticsRead(ViPipe, pstStatInfo);
        }
    } else if (IS_OFFLINE_MODE(pstDrvCtx->stWorkMode.running_mode) || \
               IS_STRIPING_MODE(pstDrvCtx->stWorkMode.running_mode)) {
        // BE statistics for offline
        s32Ret = ISP_DRV_BeOfflineStatisticsRead(ViPipe, pstStatInfo);
        if (s32Ret) {
            ISP_ERR_TRACE("ISP_DRV_BeOfflineStatisticsRead failed!\n");
            return HI_FAILURE;
        }

        if ((pstDrvCtx->stStitchAttr.main_pipe == HI_TRUE) && (pstDrvCtx->stStitchAttr.stitch_enable == HI_TRUE)) {
            ISP_DRV_BeOfflineStitchStatisticsRead(ViPipe, pstStatInfo);
        }
    } else {
        ISP_ERR_TRACE("enIspOnlineMode err 0x%x!\n", pstDrvCtx->stWorkMode.running_mode);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_StatBufInit(VI_PIPE ViPipe, HI_U64 *pu64PhyAddr)
{
    HI_S32 s32Ret, i;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U64 u64PhyAddr, u64Size;
    HI_U8  *pu8VirAddr;
    HI_CHAR acName[MAX_MMZ_NAMELEN] = {0};
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pu64PhyAddr);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_INIT(ViPipe, pstDrvCtx->stStatisticsBuf.bInit);

    osal_snprintf(acName, sizeof(acName), "ISP[%d].Stat", ViPipe);

    u64Size = sizeof(isp_stat) * MAX_ISP_STAT_BUF_NUM;
    s32Ret = cmpi_mmz_malloc_cached(HI_NULL, acName, &u64PhyAddr, (HI_VOID **)&pu8VirAddr, u64Size);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("alloc ISP statistics buf err\n");
        return HI_ERR_ISP_NOMEM;
    }

    osal_memset(pu8VirAddr, 0, u64Size);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstDrvCtx->stStatisticsBuf.u64PhyAddr = u64PhyAddr;
    pstDrvCtx->stStatisticsBuf.pVirAddr = (HI_VOID *)pu8VirAddr;
    pstDrvCtx->stStatisticsBuf.u64Size  = u64Size;

    OSAL_INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stFreeList);
    OSAL_INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stBusyList);
    OSAL_INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stUserList);

    for (i = 0; i < MAX_ISP_STAT_BUF_NUM; i++) {
        pstDrvCtx->stStatisticsBuf.astNode[i].stStatInfo.phy_addr =
            u64PhyAddr + i * sizeof(isp_stat);
        pstDrvCtx->stStatisticsBuf.astNode[i].stStatInfo.virt_addr =
            (HI_VOID *)(pu8VirAddr + i * sizeof(isp_stat));

        pstDrvCtx->stStatisticsBuf.astNode[i].stStatInfo.stat_key.key =
            ((HI_U64)ISP_STATISTICS_KEY << 32) + ISP_STATISTICS_KEY;

        osal_list_add_tail(&pstDrvCtx->stStatisticsBuf.astNode[i].list,
                           &pstDrvCtx->stStatisticsBuf.stFreeList);
    }

    pstDrvCtx->stStatisticsBuf.bInit      = HI_TRUE;
    pstDrvCtx->stStatisticsBuf.u32BusyNum = 0;
    pstDrvCtx->stStatisticsBuf.u32UserNum = 0;
    pstDrvCtx->stStatisticsBuf.u32FreeNum = MAX_ISP_STAT_BUF_NUM;

    *pu64PhyAddr = pstDrvCtx->stStatisticsBuf.u64PhyAddr;

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_StatBufExit(VI_PIPE ViPipe)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U64 u64PhyAddr;
    HI_U8  *pu8VirAddr;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_EXIT_STATE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_EXIT(ViPipe, pstDrvCtx->stStatisticsBuf.bInit);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    u64PhyAddr = pstDrvCtx->stStatisticsBuf.u64PhyAddr;
    pu8VirAddr = (HI_U8 *)pstDrvCtx->stStatisticsBuf.pVirAddr;

    pstDrvCtx->stStatisticsBuf.pVirAddr = HI_NULL;
    pstDrvCtx->stStatisticsBuf.astNode[0].stStatInfo.virt_addr = HI_NULL;
    pstDrvCtx->stStatisticsBuf.astNode[1].stStatInfo.virt_addr = HI_NULL;
    pstDrvCtx->stStatisticsBuf.u64PhyAddr = 0;
    pstDrvCtx->stStatisticsBuf.astNode[0].stStatInfo.phy_addr = 0;
    pstDrvCtx->stStatisticsBuf.astNode[1].stStatInfo.phy_addr = 0;
    pstDrvCtx->stStatisticsBuf.bInit    = HI_FALSE;

    OSAL_INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stFreeList);
    OSAL_INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stBusyList);
    OSAL_INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stUserList);
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    if (u64PhyAddr != 0) {
        cmpi_mmz_free(u64PhyAddr, pu8VirAddr);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_StatBufUserGet(VI_PIPE ViPipe, isp_stat_info **ppstStatInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    struct osal_list_head *plist;
    ISP_STAT_NODE_S *pstNode = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_STABUF_INIT(ViPipe);
    ISP_CHECK_POINTER(ppstStatInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    if (osal_list_empty(&pstDrvCtx->stStatisticsBuf.stBusyList)) {
        ISP_WARN_TRACE("busy list empty\n");
        *ppstStatInfo = HI_NULL;
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return HI_FAILURE;
    }

    /* get busy */
    plist = pstDrvCtx->stStatisticsBuf.stBusyList.next;
    if (plist == HI_NULL) {
        ISP_WARN_TRACE("busy list empty\n");
        *ppstStatInfo = HI_NULL;
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return HI_FAILURE;
    }
    osal_list_del(plist);
    pstDrvCtx->stStatisticsBuf.u32BusyNum--;

    /* return info */
    pstNode = osal_list_entry(plist, ISP_STAT_NODE_S, list);
    *ppstStatInfo = &pstNode->stStatInfo;

    /* put user */
    osal_list_add_tail(plist, &pstDrvCtx->stStatisticsBuf.stUserList);
    pstDrvCtx->stStatisticsBuf.u32UserNum++;
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_StatBufUserPut(VI_PIPE ViPipe, isp_stat_info *pstStatInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    struct osal_list_head *plist = HI_NULL;
    ISP_STAT_NODE_S *pstNode = HI_NULL;
    HI_BOOL bValid = HI_FALSE;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_STABUF_INIT(ViPipe);
    ISP_CHECK_POINTER(pstStatInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    osal_list_for_each(plist, &pstDrvCtx->stStatisticsBuf.stUserList) {
        pstNode = osal_list_entry(plist, ISP_STAT_NODE_S, list);
        if (pstNode == HI_NULL) {
            ISP_ERR_TRACE("pstNode  null pointer\n");
            break;
        }

        if (pstNode->stStatInfo.phy_addr == pstStatInfo->phy_addr) {
            bValid = HI_TRUE;
            pstNode->stStatInfo.stat_key.key= pstStatInfo->stat_key.key;
            break;
        }
    }

    if (!bValid) {
        ISP_ERR_TRACE("invalid stat info, phy 0x%llx\n", pstStatInfo->phy_addr);
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return HI_FAILURE;
    }

    /* get user */
    if (plist == HI_NULL) {
        ISP_ERR_TRACE("user list empty\n");
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return HI_FAILURE;
    }
    osal_list_del(plist);
    pstDrvCtx->stStatisticsBuf.u32UserNum--;

    /* put free */
    osal_list_add_tail(plist, &pstDrvCtx->stStatisticsBuf.stFreeList);
    pstDrvCtx->stStatisticsBuf.u32FreeNum++;
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_SetLdciSttAddr(VI_PIPE ViPipe, HI_U64 u64ReadSttAddr, HI_U64 u64WriteSttAddr)
{
    HI_U8  k, u8BlkDev, u8BlkNum;
    HI_S32 s32Ret;
    HI_U32 u32Size = sizeof(isp_ldci_stat);
    HI_U64 u64WriteAddr;
    isp_be_regs_attr  stIspBeRegsAttr = { 0 };
    S_VIPROC_REGS_TYPE *apstViprocReg[ISP_STRIPING_MAX_NUM] = { HI_NULL };

    ISP_CHECK_PIPE(ViPipe);

    s32Ret = ISP_DRV_GetViprocRegsAttr(ViPipe, apstViprocReg, &stIspBeRegsAttr);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] Get ViprocRegs Attr Err!\n", ViPipe);
        return HI_FAILURE;
    }

    u8BlkDev = stIspBeRegsAttr.block_dev;
    u8BlkNum = stIspBeRegsAttr.block_num;
    for (k = 0; k < u8BlkNum; k++) {
        u64WriteAddr = u64WriteSttAddr + k * (u32Size / DIV_0_TO_1(u8BlkNum));
        ISP_DRV_SetLdciWriteSttAddr(apstViprocReg[k + u8BlkDev], u64WriteAddr);

        if (g_LdciTprFltEn[ViPipe] == HI_FALSE) {
            ISP_DRV_SetLdciReadSttAddr(apstViprocReg[k + u8BlkDev], u64ReadSttAddr);
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_UpdateLdciNormalOnlineAttr(VI_PIPE ViPipe)
{
    HI_U8 u8ReadBufIdx, u8WriteBufIdx;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    u8ReadBufIdx  = pstDrvCtx->stLdciReadBufAttr.buf_idx;
    u8WriteBufIdx = pstDrvCtx->stLdciWriteBufAttr.buf_idx;

    ISP_DRV_SetLdciSttAddr(ViPipe, pstDrvCtx->stLdciReadBufAttr.ldci_buf[u8ReadBufIdx].phy_addr,
                           pstDrvCtx->stLdciWriteBufAttr.ldci_buf[u8WriteBufIdx].phy_addr);

    pstDrvCtx->stLdciReadBufAttr.buf_idx  = u8WriteBufIdx;
    pstDrvCtx->stLdciWriteBufAttr.buf_idx = (u8WriteBufIdx + 1) % DIV_0_TO_1(pstDrvCtx->stLdciWriteBufAttr.buf_num);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_UpdateLdciTprOnlineAttr(VI_PIPE ViPipe, isp_stat *pstStat)
{
    HI_U8 u8CurReadBufIdx;
    ISP_DRV_CTX_S   *pstDrvCtx   = HI_NULL;
    isp_ldci_stat *pstLdciStat = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstStat);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    u8CurReadBufIdx = pstDrvCtx->stLdciWriteBufAttr.buf_idx;

    /* Get LDCI Statistics from WriteBuffer(copy statistics to pstStat), then update WriteSttAddr */
    pstLdciStat = (isp_ldci_stat *)pstDrvCtx->stLdciWriteBufAttr.ldci_buf[u8CurReadBufIdx].vir_addr;

    if (pstLdciStat != HI_NULL) {
        osal_memcpy(&pstStat->ldci_stat, pstLdciStat, sizeof(isp_ldci_stat));
    }

    ISP_DRV_SetLdciSttAddr(ViPipe, pstDrvCtx->stLdciReadBufAttr.ldci_buf[0].phy_addr,
                           pstDrvCtx->stLdciWriteBufAttr.ldci_buf[u8CurReadBufIdx].phy_addr);

    pstDrvCtx->stLdciWriteBufAttr.buf_idx = (u8CurReadBufIdx + 1) % DIV_0_TO_1(pstDrvCtx->stLdciWriteBufAttr.buf_num);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_UpdateLdciNormalOfflineAttr(VI_PIPE ViPipe, isp_be_sync_para *pstBeSyncPara)
{
    HI_U8  i, u8ReadBufIdx, u8WriteBufIdx, u8BlkNum;
    HI_U32 u32Size;
    HI_U64 u64WriteSttAddr, u64ReadSttAddr;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    u8BlkNum      = pstDrvCtx->stWorkMode.block_num;
    u32Size       = sizeof(isp_ldci_stat);
    u8ReadBufIdx  = pstDrvCtx->stLdciReadBufAttr.buf_idx;
    u8WriteBufIdx = pstDrvCtx->stLdciWriteBufAttr.buf_idx;
    u64WriteSttAddr = pstDrvCtx->stLdciWriteBufAttr.ldci_buf[u8WriteBufIdx].phy_addr;
    u64ReadSttAddr  = pstDrvCtx->stLdciReadBufAttr.ldci_buf[u8ReadBufIdx].phy_addr;

    for (i = 0; i < u8BlkNum; i++) {
        pstBeSyncPara->ldci_read_stt_addr[i]  = u64ReadSttAddr;
        pstBeSyncPara->ldci_write_stt_addr[i] = u64WriteSttAddr + i * (u32Size / DIV_0_TO_1(u8BlkNum));
    }

    pstDrvCtx->stLdciReadBufAttr.buf_idx  = u8WriteBufIdx;
    pstDrvCtx->stLdciWriteBufAttr.buf_idx = (u8WriteBufIdx + 1) % DIV_0_TO_1(pstDrvCtx->stLdciWriteBufAttr.buf_num);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_UpdateLdciTprOfflineAttr(VI_PIPE ViPipe, isp_be_sync_para *pstBeSyncPara)
{
    HI_U8 i;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    for (i = 0; i < pstDrvCtx->stWorkMode.block_num; i++) {
        pstBeSyncPara->ldci_read_stt_addr[i]  = pstDrvCtx->stLdciSttAddr.read_stt_addr[i];
        pstBeSyncPara->ldci_write_stt_addr[i] = pstDrvCtx->stLdciSttAddr.cur_write_stt_addr[i];
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_LdciOnlineAttrUpdate(VI_PIPE ViPipe, isp_stat *pstStat)
{
    ISP_CHECK_PIPE(ViPipe);

    if (g_LdciTprFltEn[ViPipe] == HI_TRUE) {
        /* Copy LDCI statistics information to pstStat, then update LDCI WriteSttAddr */
        ISP_DRV_UpdateLdciTprOnlineAttr(ViPipe, pstStat);
    } else {
        /* Only update LDCI ReadSttAddr and WriteSttAddr */
        ISP_DRV_UpdateLdciNormalOnlineAttr(ViPipe);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_StatBufBusyPut(VI_PIPE ViPipe)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    struct osal_list_head *plist;
    ISP_STAT_NODE_S *pstNode = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_STABUF_INIT(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    /* There should be one frame of the newest statistics info in busy list. */
    while (!osal_list_empty(&pstDrvCtx->stStatisticsBuf.stBusyList)) {
        plist = pstDrvCtx->stStatisticsBuf.stBusyList.next;
        if (plist == HI_NULL) {
            osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
            return HI_FAILURE;
        }
        osal_list_del(plist);
        pstDrvCtx->stStatisticsBuf.u32BusyNum--;

        osal_list_add_tail(plist, &pstDrvCtx->stStatisticsBuf.stFreeList);
        pstDrvCtx->stStatisticsBuf.u32FreeNum++;
    }

    if (osal_list_empty(&pstDrvCtx->stStatisticsBuf.stFreeList)) {
        ISP_WARN_TRACE("free list empty\n");
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

        return HI_FAILURE;
    }

    /* get free */
    plist = pstDrvCtx->stStatisticsBuf.stFreeList.next;
    if (plist == HI_NULL) {
        ISP_WARN_TRACE("free list empty\n");
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return HI_FAILURE;
    }
    osal_list_del(plist);
    pstDrvCtx->stStatisticsBuf.u32FreeNum--;

    /* read statistics */
    pstNode = osal_list_entry(plist, ISP_STAT_NODE_S, list);

    pstDrvCtx->stStatisticsBuf.pstActStat = &pstNode->stStatInfo;
    ISP_DRV_StatisticsRead(ViPipe, &pstNode->stStatInfo);

    /* put busy */
    osal_list_add_tail(plist, &pstDrvCtx->stStatisticsBuf.stBusyList);
    pstDrvCtx->stStatisticsBuf.u32BusyNum++;

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

static HI_VOID ISP_DRV_LdciBufAttrReset(ISP_DRV_CTX_S *pstDrvCtx)
{
    HI_U8 i;

    pstDrvCtx->stLdciReadBufAttr.init  = HI_FALSE;
    pstDrvCtx->stLdciWriteBufAttr.init = HI_FALSE;
    pstDrvCtx->stLdciReadBufAttr.buf_num = 0;
    pstDrvCtx->stLdciWriteBufAttr.buf_num = 0;

    for (i = 0; i < MAX_ISP_BE_BUF_NUM; i++) {
        pstDrvCtx->stLdciWriteBufAttr.ldci_buf[i].phy_addr = 0;
        pstDrvCtx->stLdciWriteBufAttr.ldci_buf[i].vir_addr = HI_NULL;
        pstDrvCtx->stLdciWriteBufAttr.ldci_buf[i].size     = 0;
    }

    for (i = 0; i < MAX_ISP_BE_BUF_NUM; i++) {
        pstDrvCtx->stLdciReadBufAttr.ldci_buf[i].phy_addr  = 0;
        pstDrvCtx->stLdciReadBufAttr.ldci_buf[i].vir_addr  = HI_NULL;
        pstDrvCtx->stLdciReadBufAttr.ldci_buf[i].size      = 0;
    }
}

HI_S32 ISP_DRV_LdciTprBufInit(VI_PIPE ViPipe)
{
    HI_U8 i, j, u8WriteBufNum, u8ReadBufNum, u8BufNumAll;
    HI_U8 *pu8VirAddr;
    HI_S32 s32Ret;
    HI_U64 u64PhyAddr, u64Size;
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_CHAR acName[MAX_MMZ_NAMELEN] = { 0 };

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    ISP_CHECK_BUF_INIT(ViPipe, pstDrvCtx->stLdciReadBufAttr.init);
    ISP_CHECK_BUF_INIT(ViPipe, pstDrvCtx->stLdciWriteBufAttr.init);

    osal_snprintf(acName, sizeof(acName), "ISP[%d].LDCI", ViPipe);
    u64Size = sizeof(isp_ldci_stat);

    if (IS_ONLINE_MODE(pstDrvCtx->stWorkMode.running_mode) ||
        IS_SIDEBYSIDE_MODE(pstDrvCtx->stWorkMode.running_mode)) {
        u8WriteBufNum = 2;
        u8ReadBufNum  = 2;
    } else {
        u8WriteBufNum = MAX_ISP_BE_BUF_NUM;
        u8ReadBufNum  = MAX_ISP_BE_BUF_NUM;
    }

    u8BufNumAll = u8WriteBufNum + u8ReadBufNum;

    s32Ret = cmpi_mmz_malloc_nocache(HI_NULL, acName, &u64PhyAddr, (HI_VOID **)&pu8VirAddr, u64Size * u8BufNumAll);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] MmzMalloc Ldci buffer Failure!\n", ViPipe);
        return HI_ERR_ISP_NOMEM;
    }

    osal_memset(pu8VirAddr, 0, u64Size * u8BufNumAll);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    pstDrvCtx->stLdciWriteBufAttr.init    = HI_TRUE;
    pstDrvCtx->stLdciWriteBufAttr.buf_num = u8WriteBufNum;
    pstDrvCtx->stLdciWriteBufAttr.buf_idx = 0;

    pstDrvCtx->stLdciReadBufAttr.init     = HI_TRUE;
    pstDrvCtx->stLdciReadBufAttr.buf_num = u8ReadBufNum;
    pstDrvCtx->stLdciReadBufAttr.buf_idx = 0;

    for (i = 0; i < u8WriteBufNum; i++) {
        pstDrvCtx->stLdciWriteBufAttr.ldci_buf[i].phy_addr = u64PhyAddr + i * u64Size;
        pstDrvCtx->stLdciWriteBufAttr.ldci_buf[i].vir_addr = (HI_VOID *)(pu8VirAddr + i * u64Size);
        pstDrvCtx->stLdciWriteBufAttr.ldci_buf[i].size     = u64Size;
    }

    for (i = 0; i < u8ReadBufNum; i++) {
        j = i + u8WriteBufNum;
        pstDrvCtx->stLdciReadBufAttr.ldci_buf[i].phy_addr = u64PhyAddr + j * u64Size;
        pstDrvCtx->stLdciReadBufAttr.ldci_buf[i].vir_addr = (HI_VOID *)(pu8VirAddr + j * u64Size);
        pstDrvCtx->stLdciReadBufAttr.ldci_buf[i].size     = u64Size;
    }

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    if (IS_ONLINE_MODE(pstDrvCtx->stWorkMode.running_mode) ||
        IS_SIDEBYSIDE_MODE(pstDrvCtx->stWorkMode.running_mode)) {
        s32Ret = ISP_DRV_SetLdciSttAddr(ViPipe, pstDrvCtx->stLdciReadBufAttr.ldci_buf[0].phy_addr,
                                        pstDrvCtx->stLdciWriteBufAttr.ldci_buf[0].phy_addr);

        if (s32Ret != HI_SUCCESS) {
            ISP_ERR_TRACE("ISP[%d] Set Ldci Param/OutParam addr Err!\n", ViPipe);
            goto fail0;
        }

        /* update Write Index */
        pstDrvCtx->stLdciWriteBufAttr.buf_idx= (pstDrvCtx->stLdciWriteBufAttr.buf_idx+ 1) %
                                                DIV_0_TO_1(pstDrvCtx->stLdciWriteBufAttr.buf_num);
    }

    return HI_SUCCESS;

fail0:
    ISP_DRV_LdciBufAttrReset(pstDrvCtx);
    if (u64PhyAddr != 0) {
        cmpi_mmz_free(u64PhyAddr, (HI_VOID *)pu8VirAddr);
    }
    return HI_FAILURE;
}

HI_S32 ISP_DRV_LdciTprBufExit(VI_PIPE ViPipe)
{
    HI_U64 u64PhyAddr;
    HI_VOID *pVirAddr;
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_EXIT(ViPipe, pstDrvCtx->stLdciReadBufAttr.init);
    ISP_CHECK_BUF_EXIT(ViPipe, pstDrvCtx->stLdciWriteBufAttr.init);

    u64PhyAddr = pstDrvCtx->stLdciWriteBufAttr.ldci_buf[0].phy_addr;
    pVirAddr   = pstDrvCtx->stLdciWriteBufAttr.ldci_buf[0].vir_addr;

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    ISP_DRV_LdciBufAttrReset(pstDrvCtx);
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    if (u64PhyAddr != 0) {
        cmpi_mmz_free(u64PhyAddr, pVirAddr);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_LdciNormalBufInit(VI_PIPE ViPipe)
{
    HI_U8 i, u8BufNum;
    HI_S32 s32Ret;
    HI_U64 u64PhyAddr, u64Size;
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_CHAR acName[MAX_MMZ_NAMELEN] = { 0 };

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_INIT(ViPipe, pstDrvCtx->stLdciReadBufAttr.init);
    ISP_CHECK_BUF_INIT(ViPipe, pstDrvCtx->stLdciWriteBufAttr.init);

    osal_snprintf(acName, sizeof(acName), "ISP[%d].LDCI", ViPipe);
    u64Size  = sizeof(isp_ldci_stat);

    if (IS_ONLINE_MODE(pstDrvCtx->stWorkMode.running_mode) ||
        IS_OFFLINE_MODE(pstDrvCtx->stWorkMode.running_mode) ||
        IS_SIDEBYSIDE_MODE(pstDrvCtx->stWorkMode.running_mode)) {
        u8BufNum = 1;
    } else {
        u8BufNum = 2;
    }

    u64PhyAddr = cmpi_mmz_malloc(HI_NULL, acName, u64Size * u8BufNum);
    if (u64PhyAddr == 0) {
        ISP_ERR_TRACE("alloc ISP[%d] Ldci buf err\n", ViPipe);
        return HI_ERR_ISP_NOMEM;
    }

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstDrvCtx->stLdciReadBufAttr.init    = HI_TRUE;
    pstDrvCtx->stLdciReadBufAttr.buf_num = u8BufNum;
    pstDrvCtx->stLdciReadBufAttr.buf_idx = 0;

    pstDrvCtx->stLdciWriteBufAttr.init    = HI_TRUE;
    pstDrvCtx->stLdciWriteBufAttr.buf_num = u8BufNum;
    pstDrvCtx->stLdciWriteBufAttr.buf_idx = MIN2(u8BufNum - 1, pstDrvCtx->stLdciReadBufAttr.buf_idx + 1);

    for (i = 0; i < u8BufNum; i++) {
        pstDrvCtx->stLdciReadBufAttr.ldci_buf[i].phy_addr  = u64PhyAddr + i * u64Size;
        pstDrvCtx->stLdciWriteBufAttr.ldci_buf[i].phy_addr = u64PhyAddr + i * u64Size;
    }

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    if (IS_SIDEBYSIDE_MODE(pstDrvCtx->stWorkMode.running_mode) ||
        IS_ONLINE_MODE(pstDrvCtx->stWorkMode.running_mode)) {
        s32Ret = ISP_DRV_SetLdciSttAddr(ViPipe, u64PhyAddr, u64PhyAddr);

        if (s32Ret != HI_SUCCESS) {
            ISP_ERR_TRACE("ISP[%d] Set Ldci stt addr Err!\n", ViPipe);
            goto fail0;
        }

        /* update Write & Read Index */
        pstDrvCtx->stLdciReadBufAttr.buf_idx = pstDrvCtx->stLdciWriteBufAttr.buf_idx;
        pstDrvCtx->stLdciWriteBufAttr.buf_idx = (pstDrvCtx->stLdciWriteBufAttr.buf_idx+ 1) %
                                                DIV_0_TO_1(pstDrvCtx->stLdciWriteBufAttr.buf_num);
    }

    return HI_SUCCESS;

fail0:
    ISP_DRV_LdciBufAttrReset(pstDrvCtx);

    if (u64PhyAddr != 0) {
        cmpi_mmz_free(u64PhyAddr, HI_NULL);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_LdciNormalBufExit(VI_PIPE ViPipe)
{
    HI_U64 u64PhyAddr;
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_EXIT(ViPipe, pstDrvCtx->stLdciReadBufAttr.init);
    ISP_CHECK_BUF_EXIT(ViPipe, pstDrvCtx->stLdciWriteBufAttr.init);

    u64PhyAddr = pstDrvCtx->stLdciWriteBufAttr.ldci_buf[0].phy_addr;

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    ISP_DRV_LdciBufAttrReset(pstDrvCtx);
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    if (u64PhyAddr != 0) {
        cmpi_mmz_free(u64PhyAddr, HI_NULL);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_LdciBufInit(VI_PIPE ViPipe)
{
    ISP_CHECK_PIPE(ViPipe);

    if (g_LdciTprFltEn[ViPipe] == HI_TRUE) {
        return ISP_DRV_LdciTprBufInit(ViPipe);
    } else {
        return ISP_DRV_LdciNormalBufInit(ViPipe);
    }
}

HI_S32 ISP_DRV_LdciBufExit(VI_PIPE ViPipe)
{
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_EXIT_STATE(ViPipe);

    if (g_LdciTprFltEn[ViPipe] == HI_TRUE) {
        return ISP_DRV_LdciTprBufExit(ViPipe);
    } else {
        return ISP_DRV_LdciNormalBufExit(ViPipe);
    }

    return HI_FAILURE;
}

HI_S32 ISP_DRV_LdciReadSttBufGet(VI_PIPE ViPipe, isp_ldci_read_stt_buf *pstLdciReadSttBuf)
{
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstLdciReadSttBuf);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstLdciReadSttBuf->buf_num       = pstDrvCtx->stLdciReadBufAttr.buf_num;
    pstLdciReadSttBuf->buf_idx       = 0;
    pstLdciReadSttBuf->head_phy_addr = pstDrvCtx->stLdciReadBufAttr.ldci_buf[0].phy_addr;

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_ClutBufInit(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    HI_U64 u64PhyAddr, u64Size;
    HI_U8 *pu8VirAddr;
    ISP_DRV_CTX_S      *pstDrvCtx = HI_NULL;
    HI_CHAR acName[MAX_MMZ_NAMELEN] = { 0 };

    ISP_CHECK_PIPE(ViPipe);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_INIT(ViPipe, pstDrvCtx->stClutBufAttr.init);

    osal_snprintf(acName, sizeof(acName), "ISP[%d].Clut", ViPipe);

    u64Size =  HI_ISP_CLUT_LUT_LENGTH * sizeof(HI_U32);

    s32Ret = cmpi_mmz_malloc_nocache(HI_NULL, acName, &u64PhyAddr, (HI_VOID **)&pu8VirAddr, u64Size);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] MmzMalloc Clut buffer Failure!\n", ViPipe);
        return HI_ERR_ISP_NOMEM;
    }

    osal_memset(pu8VirAddr, 0, u64Size);

    pstDrvCtx->stClutBufAttr.init              = HI_TRUE;
    pstDrvCtx->stClutBufAttr.clut_buf.size     = u64Size;
    pstDrvCtx->stClutBufAttr.clut_buf.phy_addr = u64PhyAddr;
    pstDrvCtx->stClutBufAttr.clut_buf.vir_addr = (HI_VOID *)pu8VirAddr;

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_ClutBufExit(VI_PIPE ViPipe)
{
    HI_U64   u64PhyAddr;
    HI_VOID *pVirAddr;
    ISP_DRV_CTX_S      *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_EXIT_STATE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_EXIT(ViPipe, pstDrvCtx->stClutBufAttr.init);

    u64PhyAddr = pstDrvCtx->stClutBufAttr.clut_buf.phy_addr;
    pVirAddr   = pstDrvCtx->stClutBufAttr.clut_buf.vir_addr;

    pstDrvCtx->stClutBufAttr.init              = HI_FALSE;
    pstDrvCtx->stClutBufAttr.clut_buf.size     = 0;
    pstDrvCtx->stClutBufAttr.clut_buf.phy_addr = 0;
    pstDrvCtx->stClutBufAttr.clut_buf.vir_addr = HI_NULL;

    if (u64PhyAddr != 0) {
        cmpi_mmz_free(u64PhyAddr, pVirAddr);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_SpecAwbBufInit(VI_PIPE ViPipe)
{
#ifdef CONFIG_HI_ISP_SPECAWB_SUPPORT
    HI_S32 s32Ret;
    HI_U64 u64PhyAddr, u64Size;
    HI_U8 *pu8VirAddr;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_CHAR acName[MAX_MMZ_NAMELEN] = { 0 };

    ISP_CHECK_PIPE(ViPipe);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_INIT(ViPipe, pstDrvCtx->stSpecAwbBufAttr.init);

    osal_snprintf(acName, sizeof(acName), "ISP[%d].SpecAwb", ViPipe);

    u64Size = ISP_SPECAWB_BUF_SIZE;
    s32Ret  = cmpi_mmz_malloc_nocache(HI_NULL, acName, &u64PhyAddr, (HI_VOID **)&pu8VirAddr, u64Size);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] MmzMalloc SpecAwb buffer Failure!\n", ViPipe);
        return HI_FAILURE;
    }

    osal_memset(pu8VirAddr, 0, u64Size);

    pstDrvCtx->stSpecAwbBufAttr.init                    = HI_TRUE;
    pstDrvCtx->stSpecAwbBufAttr.spec_awb_buf.size       = u64Size;
    pstDrvCtx->stSpecAwbBufAttr.spec_awb_buf.phy_addr   = u64PhyAddr;
    pstDrvCtx->stSpecAwbBufAttr.spec_awb_buf.vir_addr   = (HI_VOID *)pu8VirAddr;
#endif
    return HI_SUCCESS;
}

HI_S32 ISP_DRV_SpecAwbBufExit(VI_PIPE ViPipe)
{
#ifdef CONFIG_HI_ISP_SPECAWB_SUPPORT
    HI_U64   u64PhyAddr;
    HI_VOID *pVirAddr;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_EXIT_STATE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_EXIT(ViPipe, pstDrvCtx->stSpecAwbBufAttr.init);

    u64PhyAddr = pstDrvCtx->stSpecAwbBufAttr.spec_awb_buf.phy_addr;
    pVirAddr   = pstDrvCtx->stSpecAwbBufAttr.spec_awb_buf.vir_addr;

    pstDrvCtx->stSpecAwbBufAttr.init                  = HI_FALSE;
    pstDrvCtx->stSpecAwbBufAttr.spec_awb_buf.size     = 0;
    pstDrvCtx->stSpecAwbBufAttr.spec_awb_buf.phy_addr = 0;
    pstDrvCtx->stSpecAwbBufAttr.spec_awb_buf.vir_addr = HI_NULL;

    if (u64PhyAddr != 0) {
        cmpi_mmz_free(u64PhyAddr, pVirAddr);
    }
#endif
    return HI_SUCCESS;
}


HI_S32 ISP_DRV_BeBufInit(VI_PIPE ViPipe, HI_U64 *pu64PhyAddr)
{
    HI_S32 s32Ret;
    HI_U32 i;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_BE_BUF_NODE_S *pstNode = HI_NULL;
    HI_U64 u64PhyAddr, u64Size;
    HI_U8  *pu8VirAddr;
    HI_CHAR acName[MAX_MMZ_NAMELEN] = {0};
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pu64PhyAddr);
    ISP_CHECK_ONLINE_MODE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_INIT(ViPipe, pstDrvCtx->stBeBufInfo.init);

    osal_snprintf(acName, sizeof(acName), "ISP[%d].BeCfg", ViPipe);

    u64Size = sizeof(isp_be_wo_reg_cfg);

    s32Ret = cmpi_mmz_malloc_cached(HI_NULL, acName, &u64PhyAddr, (HI_VOID **)&pu8VirAddr, u64Size * MAX_ISP_BE_BUF_NUM);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("Pipe[%d] alloc ISP BeCfgBuf err!\n", ViPipe);
        return HI_ERR_ISP_NOMEM;
    }

    osal_memset(pu8VirAddr, 0, u64Size * MAX_ISP_BE_BUF_NUM);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    pstDrvCtx->stBeBufInfo.init                  = HI_TRUE;
    pstDrvCtx->stBeBufInfo.be_buf_haddr.phy_addr = u64PhyAddr;
    pstDrvCtx->stBeBufInfo.be_buf_haddr.vir_addr = (HI_VOID *)pu8VirAddr;
    pstDrvCtx->stBeBufInfo.be_buf_haddr.size     = u64Size * MAX_ISP_BE_BUF_NUM;

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    s32Ret = ISP_CreatBeBufQueue(&pstDrvCtx->stBeBufQueue, MAX_ISP_BE_BUF_NUM);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("Pipe[%d] CreatBeBufQueue fail!\n", ViPipe);

        goto fail0;
    }

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    for (i = 0; i < MAX_ISP_BE_BUF_NUM; i++) {
        pstNode = ISP_QueueGetFreeBeBuf(&pstDrvCtx->stBeBufQueue);

        if (pstNode == HI_NULL) {
            ISP_ERR_TRACE("Pipe[%d] Get QueueGetFreeBeBuf fail!\r\n", ViPipe);
            osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
            goto fail1;
        }

        pstNode->stBeCfgBuf.phy_addr = u64PhyAddr + i * u64Size;
        pstNode->stBeCfgBuf.vir_addr = (HI_VOID *)(pu8VirAddr + i * u64Size);
        pstNode->stBeCfgBuf.size     = u64Size;
        pstNode->s32HoldCnt          = 0;

        ISP_QueuePutFreeBeBuf(&pstDrvCtx->stBeBufQueue, pstNode);
    }

    pstDrvCtx->pstUseNode        = HI_NULL;
    pstDrvCtx->enIspRunningState = ISP_BE_BUF_STATE_INIT;
    pstDrvCtx->enIspExitState    = ISP_BE_BUF_READY;

    *pu64PhyAddr = pstDrvCtx->stBeBufInfo.be_buf_haddr.phy_addr;

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;

fail1:
    ISP_DestroyBeBufQueue(&pstDrvCtx->stBeBufQueue);

fail0:
    pstDrvCtx->stBeBufInfo.init                  = HI_FALSE;
    pstDrvCtx->stBeBufInfo.be_buf_haddr.phy_addr = 0;
    pstDrvCtx->stBeBufInfo.be_buf_haddr.vir_addr = HI_NULL;
    pstDrvCtx->stBeBufInfo.be_buf_haddr.size     = 0;

    if (u64PhyAddr != 0) {
        cmpi_mmz_free(u64PhyAddr, (HI_VOID *)pu8VirAddr);
    }

    return HI_FAILURE;
}

HI_S32 ISP_DRV_BeBufExit(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    HI_U64 u64PhyAddr;
    HI_VOID *pVirAddr;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_ONLINE_MODE(ViPipe);
    ISP_CHECK_EXIT_STATE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_EXIT(ViPipe, pstDrvCtx->stBeBufInfo.init);

    u64PhyAddr = pstDrvCtx->stBeBufInfo.be_buf_haddr.phy_addr;
    pVirAddr   = pstDrvCtx->stBeBufInfo.be_buf_haddr.vir_addr;

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstDrvCtx->enIspExitState = ISP_BE_BUF_WAITING;
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    if (CHECK_FUNC_ENTRY(HI_ID_VI) && ckfn_vi_update_vi_vpss_mode()) {
        /* Note: this function cannot be placed in the ISP lock, otherwise it will be deadlocked. */
        call_vi_isp_clear_input_queue(ViPipe);
    }

    s32Ret = osal_wait_event_timeout_interruptible(&pstDrvCtx->stIspExitWait, ISP_DRV_WaitExitCallback, \
                                                   &pstDrvCtx->stBeBufInfo.use_cnt, g_IspExitTimeout);

    if (s32Ret <= 0) {
        ISP_ERR_TRACE("Pipe:%d isp exit wait failed:s32Ret:%d!\n", ViPipe, s32Ret);
        return HI_FAILURE;
    }

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    ISP_DestroyBeBufQueue(&pstDrvCtx->stBeBufQueue);

    pstDrvCtx->stBeBufInfo.init                  = HI_FALSE;
    pstDrvCtx->stBeBufInfo.be_buf_haddr.phy_addr = 0;
    pstDrvCtx->stBeBufInfo.be_buf_haddr.vir_addr = HI_NULL;
    pstDrvCtx->stBeBufInfo.be_buf_haddr.size     = 0;
    pstDrvCtx->enIspExitState                    = ISP_BE_BUF_EXIT;

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    if (u64PhyAddr != 0) {
        cmpi_mmz_free(u64PhyAddr, pVirAddr);
    }

    return HI_SUCCESS;
}

osal_spinlock_t *ISP_DRV_GetSpinLock(VI_PIPE ViPipe)
{
    VI_PIPE MainPipeS;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstDrvCtx->stStitchAttr.stitch_enable != HI_TRUE) {
        return &g_stIspLock[ViPipe];
    } else {
        MainPipeS = pstDrvCtx->stStitchAttr.stitch_bind_id[0];
        return &g_stIspSyncLock[MainPipeS];
    }
}

HI_S32 ISP_DRV_GetReadyBeBuf(VI_PIPE ViPipe, isp_be_wo_cfg_buf *pstBeCfgBuf)
{
    osal_spinlock_t *pIspSpinLock;
    unsigned long u32Flags;
    ISP_BE_BUF_NODE_S *pstNode = HI_NULL;
    ISP_DRV_CTX_S *pstDrvCtx  = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstBeCfgBuf);
    ISP_CHECK_ONLINE_MODE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    pIspSpinLock = ISP_DRV_GetSpinLock(ViPipe);
    osal_spin_lock_irqsave(pIspSpinLock, &u32Flags);

    if ((pstDrvCtx->enIspExitState == ISP_BE_BUF_EXIT) ||
        (pstDrvCtx->enIspExitState == ISP_BE_BUF_WAITING)) {
        ISP_ERR_TRACE("ViPipe[%d] ISP BE Buf not existed!!!\n", ViPipe);
        osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);
        return HI_FAILURE;
    }

    pstNode = ISP_QueueQueryBusyBeBuf(&pstDrvCtx->stBeBufQueue);
    if (pstNode == HI_NULL) {
        ISP_ERR_TRACE("ViPipe[%d] QueueQueryBusyBeBuf pstNode is null!\n", ViPipe);
        osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);
        return HI_FAILURE;
    }

    pstNode->s32HoldCnt++;
    pstDrvCtx->stBeBufInfo.use_cnt++;

    osal_memcpy(pstBeCfgBuf, &pstNode->stBeCfgBuf, sizeof(isp_be_wo_cfg_buf));
    ISP_DRV_UpdateLdciTprSttAddr(ViPipe, (isp_be_wo_reg_cfg *)pstNode->stBeCfgBuf.vir_addr);
    pstDrvCtx->enIspExitState = ISP_BE_BUF_READY;

    osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);

    return HI_SUCCESS;

}

HI_VOID ISP_DRV_PutBusyToFree(VI_PIPE ViPipe, isp_be_wo_cfg_buf *pstBeCfgBuf)
{
    ISP_BE_BUF_NODE_S *pstNode;
    struct osal_list_head *pListTmp = HI_NULL;
    struct osal_list_head *pListNode = HI_NULL;
    ISP_DRV_CTX_S *pstDrvCtx  = HI_NULL;

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_list_for_each_safe(pListNode, pListTmp, &pstDrvCtx->stBeBufQueue.stBusyList) {
        pstNode = osal_list_entry(pListNode, ISP_BE_BUF_NODE_S, list);

        if (pstNode->stBeCfgBuf.phy_addr == pstBeCfgBuf->phy_addr) {
            if (pstNode->s32HoldCnt > 0) {
                pstNode->s32HoldCnt--;
            }

            if (pstNode->s32HoldCnt == 0) {
                if (ISP_QueueGetBusyNum(&pstDrvCtx->stBeBufQueue) > 1) {
                    ISP_QueueDelBusyBeBuf(&pstDrvCtx->stBeBufQueue, pstNode);
                    ISP_QueuePutFreeBeBuf(&pstDrvCtx->stBeBufQueue, pstNode);
                }
            }
        }
    }

    return;
}

HI_S32 ISP_DRV_PutFreeBeBuf(VI_PIPE ViPipe, isp_be_wo_cfg_buf *pstBeCfgBuf)
{
    osal_spinlock_t *pIspSpinLock = HI_NULL;
    ISP_DRV_CTX_S *pstDrvCtx  = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstBeCfgBuf);
    ISP_CHECK_ONLINE_MODE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    pIspSpinLock = ISP_DRV_GetSpinLock(ViPipe);
    osal_spin_lock_irqsave(pIspSpinLock, &u32Flags);

    if (pstDrvCtx->enIspExitState == ISP_BE_BUF_EXIT) {
        ISP_ERR_TRACE("ViPipe[%d] ISP BE Buf not existed!!!\n", ViPipe);
        osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);
        return HI_FAILURE;
    }

    ISP_DRV_PutBusyToFree(ViPipe, pstBeCfgBuf);

    if (pstDrvCtx->stBeBufInfo.use_cnt> 0) {
        pstDrvCtx->stBeBufInfo.use_cnt--;
    }
    if (g_LdciTprFltEn[ViPipe] == HI_TRUE) {
        ISP_DRV_UpdateLdciTprOfflineStat(ViPipe, (isp_be_wo_reg_cfg *)pstBeCfgBuf->vir_addr);
    }

    osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);

    osal_wakeup(&pstDrvCtx->stIspExitWait);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_HoldBusyBeBuf(VI_PIPE ViPipe, isp_be_wo_cfg_buf *pstBeCfgBuf)
{
    osal_spinlock_t *pIspSpinLock = HI_NULL;
    ISP_BE_BUF_NODE_S *pstNode;
    struct osal_list_head *pListTmp = HI_NULL;
    struct osal_list_head *pListNode = HI_NULL;
    ISP_DRV_CTX_S *pstDrvCtx  = HI_NULL;
    unsigned long  u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstBeCfgBuf);
    ISP_CHECK_ONLINE_MODE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    pIspSpinLock = ISP_DRV_GetSpinLock(ViPipe);
    osal_spin_lock_irqsave(pIspSpinLock, &u32Flags);

    if ((pstDrvCtx->enIspExitState == ISP_BE_BUF_EXIT) ||
        (pstDrvCtx->enIspExitState == ISP_BE_BUF_WAITING)) {
        ISP_ERR_TRACE("ViPipe[%d] ISP BE Buf not existed!!!\n", ViPipe);
        osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);
        return HI_FAILURE;
    }

    osal_list_for_each_safe(pListNode, pListTmp, &pstDrvCtx->stBeBufQueue.stBusyList) {
        pstNode = osal_list_entry(pListNode, ISP_BE_BUF_NODE_S, list);

        if (pstNode->stBeCfgBuf.phy_addr == pstBeCfgBuf->phy_addr) {
            pstNode->s32HoldCnt++;
            pstDrvCtx->stBeBufInfo.use_cnt++;
        }
    }

    pstDrvCtx->enIspExitState = ISP_BE_BUF_READY;

    osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_GetBeSyncPara(VI_PIPE ViPipe, isp_be_sync_para *pstBeSyncPara)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_S32 s32Ret;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstBeSyncPara);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstDrvCtx->stStitchAttr.stitch_enable == HI_TRUE) {
        s32Ret = ISP_DRV_GetStitchBeSyncPara(ViPipe, pstBeSyncPara);

        if (s32Ret != HI_SUCCESS) {
            return HI_FAILURE;
        }
    } else {
        osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
        osal_memcpy(pstBeSyncPara, &pstDrvCtx->stIspBeSyncPara, sizeof(isp_be_sync_para));
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
    }

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    if (g_LdciTprFltEn[ViPipe] == HI_TRUE) {
        ISP_DRV_UpdateLdciTprOfflineAttr(ViPipe, pstBeSyncPara);
    } else {
        ISP_DRV_UpdateLdciNormalOfflineAttr(ViPipe, pstBeSyncPara);
    }
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_BeBufSwitchState(VI_PIPE ViPipe)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;
    osal_spinlock_t *pIspSpinLock = HI_NULL;

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    pIspSpinLock = ISP_DRV_GetSpinLock(ViPipe);

    osal_spin_lock_irqsave(pIspSpinLock, &u32Flags);

    if (pstDrvCtx->stStitchAttr.stitch_enable == HI_TRUE) {
        pstDrvCtx->enIspRunningState = ISP_BE_BUF_STATE_SWITCH_START;
    }

    osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);

    return HI_SUCCESS;
}


HI_S32 ISP_DRV_SwitchMode(VI_PIPE ViPipe, ISP_DRV_CTX_S *pstDrvCtx)
{
    HI_U8   i, j, k;
    HI_U8   u8BlkDev, u8BlkNum;
    HI_S32  ViDev;
    HI_U32  s32Ret;
    HI_U32  au32ChnSwitch[5] = { 0 };
    ISP_SYNC_CFG_S *pstSyncCfg = HI_NULL;
    isp_be_regs_attr stIspBeRegsAttr = { 0 };
    S_ISPBE_REGS_TYPE *apstBeReg[ISP_STRIPING_MAX_NUM] = { HI_NULL };
    isp_sync_cfg_buf_node *pstCfgNode = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDrvCtx);
    ViDev = pstDrvCtx->stWDRAttr.vi_dev;

    s32Ret = ISP_DRV_GetBeRegsAttr(ViPipe, apstBeReg, &stIspBeRegsAttr);
    if (s32Ret != HI_SUCCESS) {
        return HI_SUCCESS;
    }

    u8BlkDev = stIspBeRegsAttr.block_dev;
    u8BlkNum = stIspBeRegsAttr.block_num;

    pstSyncCfg = &pstDrvCtx->stSyncCfg;
    pstSyncCfg->u8WDRMode = pstDrvCtx->stWDRCfg.wdr_mode;

    for (j = 0; j < 3; j++) {
        for (i = 0; i < CFG2VLD_DLY_LIMIT; i++) {
            pstSyncCfg->u32ExpRatio[j][i] = pstDrvCtx->stWDRCfg.exp_ratio[j];
        }
    }

    pstCfgNode = pstDrvCtx->stSyncCfg.apstNode[0];
    if (pstCfgNode != HI_NULL) {
        /* wdr fusion mode need update */
        osal_memcpy(&pstCfgNode->stWDRRegCfg, &pstDrvCtx->stWDRCfg.wdr_reg_cfg, sizeof(ISP_FSWDR_SYNC_CFG_S));
        ISP_DRV_RegConfigWdr(ViPipe, apstBeReg, pstDrvCtx, &pstCfgNode->stWDRRegCfg, u8BlkNum, u8BlkDev,
                             pstDrvCtx->stWDRCfg.exp_ratio);
        ISP_DRV_RegConfigDrc(ViPipe, apstBeReg, pstDrvCtx, &pstCfgNode->stDRCRegCfg, u8BlkNum, u8BlkDev,
                             pstDrvCtx->stWDRCfg.exp_ratio);
    }

    /* init cfg when modes change */
    osal_memset(&pstDrvCtx->stSyncCfg.stSyncCfgBuf, 0, sizeof(ISP_SYNC_CFG_BUF_S));
    osal_memset(&pstDrvCtx->stSyncCfg.apstNode, 0, sizeof(pstDrvCtx->stSyncCfg.apstNode));
    osal_memset(&pstDrvCtx->isp_int_info, 0, sizeof(isp_interrupt_info));

    pstSyncCfg->u8VCNum = 0;
    pstSyncCfg->u8VCCfgNum = 0;
    pstSyncCfg->u8Cfg2VldDlyMAX = 1;

    /* get N (N to 1 WDR) */
    switch (pstSyncCfg->u8WDRMode) {
        default:
        case WDR_MODE_NONE:
        case WDR_MODE_BUILT_IN:
        case WDR_MODE_2To1_LINE:
        case WDR_MODE_3To1_LINE:
        case WDR_MODE_4To1_LINE:
            pstSyncCfg->u8VCNumMax = 0;
            break;

        case WDR_MODE_2To1_FRAME:
        case WDR_MODE_2To1_FRAME_FULL_RATE:
            pstSyncCfg->u8VCNumMax = 1;
            break;

        case WDR_MODE_3To1_FRAME:
        case WDR_MODE_3To1_FRAME_FULL_RATE:
            pstSyncCfg->u8VCNumMax = 2;
            break;

        case WDR_MODE_4To1_FRAME:
        case WDR_MODE_4To1_FRAME_FULL_RATE:
            pstSyncCfg->u8VCNumMax = 3;
            break;
    }

    /* Channel Switch config */
    if (IS_FULL_WDR_MODE(pstSyncCfg->u8WDRMode)) {
        au32ChnSwitch[0] = 0;
        au32ChnSwitch[1] = 1;
        au32ChnSwitch[2] = 2;
        au32ChnSwitch[3] = 3;
    } else if (IS_HALF_WDR_MODE(pstSyncCfg->u8WDRMode)) {
        au32ChnSwitch[0] = 1 % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
        au32ChnSwitch[1] = (au32ChnSwitch[0] + 1) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
        au32ChnSwitch[2] = (au32ChnSwitch[0] + 2) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
        au32ChnSwitch[3] = (au32ChnSwitch[0] + 3) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
    } else if (IS_LINE_WDR_MODE(pstSyncCfg->u8WDRMode)) {
        if (IS_2to1_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)) {
            au32ChnSwitch[0] = 0;
            au32ChnSwitch[1] = 1;
            au32ChnSwitch[2] = 2;
            au32ChnSwitch[3] = 3;
        } else if (IS_3to1_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)) {
            au32ChnSwitch[0] = 0;
            au32ChnSwitch[1] = 1;
            au32ChnSwitch[2] = 2;
            au32ChnSwitch[3] = 3;
        } else if (IS_4to1_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)) {
            au32ChnSwitch[0] = 0;
            au32ChnSwitch[1] = 1;
            au32ChnSwitch[2] = 2;
            au32ChnSwitch[3] = 3;
        } else {
        }
    } else {
        au32ChnSwitch[0] = 0;
        au32ChnSwitch[1] = 1;
        au32ChnSwitch[2] = 2;
        au32ChnSwitch[3] = 3;
    }

    if (pstDrvCtx->bYuvMode == HI_TRUE) {
        au32ChnSwitch[4] = 1;
    } else {
        au32ChnSwitch[4] = 0;
    }

    for (k = 0; k < u8BlkNum; k++) {
        ISP_DRV_SetInputSel(apstBeReg[k + u8BlkDev], &au32ChnSwitch[0]);

        osal_memcpy(&pstDrvCtx->astChnSelAttr[k].wdr_chn_sel, au32ChnSwitch, sizeof(au32ChnSwitch));
    }

    /* pt_int_mask */
    if ((IS_FULL_WDR_MODE(pstSyncCfg->u8WDRMode)) || (IS_HALF_WDR_MODE(pstSyncCfg->u8WDRMode))) {
        IO_RW_PT_ADDRESS(VI_PT_BASE(ViDev) + VI_PT_INT_MASK) |= VI_PT_INT_FSTART;
    } else {
        IO_RW_PT_ADDRESS(VI_PT_BASE(ViDev) + VI_PT_INT_MASK) &= (~VI_PT_INT_FSTART);
    }

    pstSyncCfg->u8PreWDRMode = pstSyncCfg->u8WDRMode;

    return HI_SUCCESS;

}

HI_S32 ISP_DRV_GetSyncControlnfo(VI_PIPE ViPipe, ISP_SYNC_CFG_S *pstSyncCfg)
{
    HI_S32 ViDev;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    isp_sync_cfg_buf_node *pstCurNode = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstSyncCfg);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    ViDev = pstDrvCtx->stWDRAttr.vi_dev;

    pstSyncCfg->u8VCNum = (IO_RW_PT_ADDRESS(VI_PT_BASE(ViDev) + VI_PT0_ID) & 0x30) >> 4;

    if (pstSyncCfg->u8VCNumMax == 0) {
        pstSyncCfg->u8VCNum = 0;
    }

    if (pstSyncCfg->u8VCNum > pstSyncCfg->u8VCNumMax) {
        ISP_ERR_TRACE("err VC number(%d), can't be large than VC total(%d)!\n",
                  pstSyncCfg->u8VCNum, pstSyncCfg->u8VCNumMax);
    }

    /* get Cfg2VldDlyMAX */
    if (!ISPSyncBufIsEmpty(&pstSyncCfg->stSyncCfgBuf)) {
        pstCurNode = &pstSyncCfg->stSyncCfgBuf.stSyncCfgBufNode[pstSyncCfg->stSyncCfgBuf.u8BufRDFlag];

        if (pstCurNode != HI_NULL) {
            if (pstCurNode->valid) {
                pstSyncCfg->u8Cfg2VldDlyMAX = pstCurNode->stSnsRegsInfo.u8Cfg2ValidDelayMax;
            }
        }
    }

    if ((pstSyncCfg->u8Cfg2VldDlyMAX > CFG2VLD_DLY_LIMIT) || (pstSyncCfg->u8Cfg2VldDlyMAX < 1)) {
        ISP_WARN_TRACE("Delay of config to valid is:0x%x\n", pstSyncCfg->u8Cfg2VldDlyMAX);
        pstSyncCfg->u8Cfg2VldDlyMAX = 1;
    }

    /* calc VCCfgNum =  (Cfg2VldDlyMAX * (VCNumMax + 1) + VCNum - Cfg2VldDlyMAX) % (VCNumMax + 1) */
    pstSyncCfg->u8VCCfgNum = (pstSyncCfg->u8VCNum + pstSyncCfg->u8VCNumMax * pstSyncCfg->u8Cfg2VldDlyMAX) %
                             DIV_0_TO_1(pstSyncCfg->u8VCNumMax + 1);

    return HI_SUCCESS;
}

static HI_S32 ISP_DRV_CalcExpRatio(ISP_SYNC_CFG_S *pstSyncCfg, isp_sync_cfg_buf_node *pstCurNode,
                                   isp_sync_cfg_buf_node *pstPreNode, HI_U64 *au64CurSnsGain,
                                   HI_U64 *au64PreSnsGain)
{
    HI_U32 i, j;
    HI_U64 u64Tmp     = 0;
    HI_U64 u64Ratio   = 0;
    HI_U64 au64Exp[4] = { 0 };

    if (IS_FULL_WDR_MODE(pstSyncCfg->u8WDRMode)) {
        switch (pstSyncCfg->u8VCCfgNum) {
            case 0:
                au64Exp[0] = pstCurNode->stAERegCfg.u32IntTime[0] * au64CurSnsGain[0];
                au64Exp[1] = pstPreNode->stAERegCfg.u32IntTime[1] * au64PreSnsGain[0];
                au64Exp[2] = pstPreNode->stAERegCfg.u32IntTime[2] * au64PreSnsGain[0];
                au64Exp[3] = pstPreNode->stAERegCfg.u32IntTime[3] * au64PreSnsGain[0];
                break;

            case 1:
                au64Exp[0] = pstCurNode->stAERegCfg.u32IntTime[0] * au64CurSnsGain[0];
                au64Exp[1] = pstCurNode->stAERegCfg.u32IntTime[1] * au64CurSnsGain[0];
                au64Exp[2] = pstPreNode->stAERegCfg.u32IntTime[2] * au64PreSnsGain[0];
                au64Exp[3] = pstPreNode->stAERegCfg.u32IntTime[3] * au64PreSnsGain[0];
                break;

            case 2:
                au64Exp[0] = pstCurNode->stAERegCfg.u32IntTime[0] * au64CurSnsGain[0];
                au64Exp[1] = pstCurNode->stAERegCfg.u32IntTime[1] * au64CurSnsGain[0];
                au64Exp[2] = pstCurNode->stAERegCfg.u32IntTime[2] * au64CurSnsGain[0];
                au64Exp[3] = pstPreNode->stAERegCfg.u32IntTime[3] * au64PreSnsGain[0];
                break;

            case 3:
                au64Exp[0] = pstCurNode->stAERegCfg.u32IntTime[0] * au64CurSnsGain[0];
                au64Exp[1] = pstCurNode->stAERegCfg.u32IntTime[1] * au64CurSnsGain[0];
                au64Exp[2] = pstCurNode->stAERegCfg.u32IntTime[2] * au64CurSnsGain[0];
                au64Exp[3] = pstCurNode->stAERegCfg.u32IntTime[3] * au64CurSnsGain[0];
                break;
        }
    } else if (IS_2to1_WDR_MODE(pstSyncCfg->u8WDRMode)) {
        au64Exp[0] = pstCurNode->stAERegCfg.u32IntTime[0] * au64CurSnsGain[0];
        au64Exp[1] = pstCurNode->stAERegCfg.u32IntTime[1] * au64CurSnsGain[1];
        au64Exp[2] = pstCurNode->stAERegCfg.u32IntTime[2] * au64CurSnsGain[0];
        au64Exp[3] = pstCurNode->stAERegCfg.u32IntTime[3] * au64CurSnsGain[0];
    } else {
        au64Exp[0] = pstCurNode->stAERegCfg.u32IntTime[0] * au64CurSnsGain[0];
        au64Exp[1] = pstCurNode->stAERegCfg.u32IntTime[1] * au64CurSnsGain[0];
        au64Exp[2] = pstCurNode->stAERegCfg.u32IntTime[2] * au64CurSnsGain[0];
        au64Exp[3] = pstCurNode->stAERegCfg.u32IntTime[3] * au64CurSnsGain[0];
    }

    if ((IS_LINE_WDR_MODE(pstSyncCfg->u8WDRMode)) &&
        (pstCurNode->stAERegCfg.enFSWDRMode == ISP_FSWDR_LONG_FRAME_MODE)) {
        for (j = 0; j < 3; j++) {
            for (i = CFG2VLD_DLY_LIMIT - 1; i >= 1; i--) {
                pstSyncCfg->u32ExpRatio[j][i] = pstSyncCfg->u32ExpRatio[j][i - 1];
                pstSyncCfg->u32WDRGain[j][i] = pstSyncCfg->u32WDRGain[j][i - 1];
            }

            pstSyncCfg->u32ExpRatio[j][0] = 0x40;
            pstSyncCfg->u32WDRGain[j][0] = 0x100;
        }

        for (i = CFG2VLD_DLY_LIMIT - 1; i >= 1; i--) {
            pstSyncCfg->u8LFMode[i] = pstSyncCfg->u8LFMode[i - 1];
        }

        pstSyncCfg->u8LFMode[0] = 1;
        pstSyncCfg->u32WDRGain[3][0] = 0x100;
    } else if ((IS_LINE_WDR_MODE(pstSyncCfg->u8WDRMode)) &&
               (pstCurNode->stAERegCfg.enFSWDRMode == ISP_FSWDR_AUTO_LONG_FRAME_MODE)) {
        for (j = 0; j < 3; j++) {
            for (i = CFG2VLD_DLY_LIMIT - 1; i >= 1; i--) {
                pstSyncCfg->u32ExpRatio[j][i] = pstSyncCfg->u32ExpRatio[j][i - 1];
                pstSyncCfg->u32WDRGain[j][i] = pstSyncCfg->u32WDRGain[j][i - 1];
            }

            u64Ratio = au64Exp[j + 1];
            u64Tmp   = au64Exp[j];
            u64Tmp   = DIV_0_TO_1(u64Tmp);

            while (u64Ratio > (0x1LL << 25) || u64Tmp > (0x1LL << 25)) {
                u64Tmp >>= 1;
                u64Ratio >>= 1;
            }

            u64Ratio = (u64Ratio * pstCurNode->stAERegCfg.au32WDRGain[j + 1]) << WDR_EXP_RATIO_SHIFT;
            u64Tmp = (u64Tmp * pstCurNode->stAERegCfg.au32WDRGain[j]);

            while (u64Tmp > (0x1LL << 31)) {
                u64Tmp >>= 1;
                u64Ratio >>= 1;
            }

            u64Ratio = osal_div64_u64(u64Ratio, DIV_0_TO_1(u64Tmp));
            u64Ratio = u64Ratio < 0x45 ? 0x40 : u64Ratio;
            pstSyncCfg->u32ExpRatio[j][0] = (HI_U32)u64Ratio;
            pstSyncCfg->u32WDRGain[j][0] = pstCurNode->stAERegCfg.au32WDRGain[j];
        }

        for (i = CFG2VLD_DLY_LIMIT - 1; i >= 1; i--) {
            pstSyncCfg->u8LFMode[i] = pstSyncCfg->u8LFMode[i - 1];
            pstSyncCfg->u32WDRGain[3][i] = pstSyncCfg->u32WDRGain[3][i - 1];
        }

        pstSyncCfg->u8LFMode[0] = 2;
        pstSyncCfg->u32WDRGain[3][0] = 0x100;
    } else {
        for (j = 0; j < 3; j++) {
            for (i = CFG2VLD_DLY_LIMIT - 1; i >= 1; i--) {
                pstSyncCfg->u32ExpRatio[j][i] = pstSyncCfg->u32ExpRatio[j][i - 1];
                pstSyncCfg->u32WDRGain[j][i] = pstSyncCfg->u32WDRGain[j][i - 1];
            }

            u64Ratio = au64Exp[j + 1];
            u64Tmp = au64Exp[j];
            u64Tmp = DIV_0_TO_1(u64Tmp);

            while (u64Ratio > (0x1LL << 25) || u64Tmp > (0x1LL << 25)) {
                u64Tmp >>= 1;
                u64Ratio >>= 1;
            }

            u64Ratio = ((u64Ratio * pstCurNode->stAERegCfg.au32WDRGain[j + 1])) << WDR_EXP_RATIO_SHIFT;
            u64Tmp = (u64Tmp * pstCurNode->stAERegCfg.au32WDRGain[j]);

            while (u64Tmp > (0x1LL << 31)) {
                u64Tmp >>= 1;
                u64Ratio >>= 1;
            }

            u64Ratio = osal_div64_u64(u64Ratio, DIV_0_TO_1(u64Tmp));
            pstSyncCfg->u32ExpRatio[j][0] = (HI_U32)u64Ratio;
            pstSyncCfg->u32WDRGain[j][0]  = pstCurNode->stAERegCfg.au32WDRGain[j];
        }

        for (i = CFG2VLD_DLY_LIMIT - 1; i >= 1; i--) {
            pstSyncCfg->u8LFMode[i]      = pstSyncCfg->u8LFMode[i - 1];
            pstSyncCfg->u32WDRGain[3][i] = pstSyncCfg->u32WDRGain[3][i - 1];
        }

        pstSyncCfg->u8LFMode[0]      = 0;
        pstSyncCfg->u32WDRGain[3][0] = 0x100;
    }

    return HI_SUCCESS;
}

static HI_S32 ISP_DRV_CalcDrcComp(ISP_SYNC_CFG_S *pstSyncCfg, isp_sync_cfg_buf_node *pstCurNode,
                                  isp_sync_cfg_buf_node *pstPreNode)
{
    HI_U32 i;
    HI_U64 u64Tmp, u64CurExp, u64PreExp;

    if (IS_2to1_WDR_MODE(pstSyncCfg->u8WDRMode)) {
        u64CurExp = pstCurNode->stAERegCfg.u64ExposureSF * 256;
        u64PreExp = pstPreNode->stAERegCfg.u64ExposureSF * 256;
    } else {
        u64CurExp = pstCurNode->stAERegCfg.u64Exposure;
        u64PreExp = pstPreNode->stAERegCfg.u64Exposure;
        u64CurExp = u64CurExp * pstCurNode->stAERegCfg.au32WDRGain[0];
        u64PreExp = u64PreExp * pstPreNode->stAERegCfg.au32WDRGain[0];
    }
    while (u64CurExp > (0x1LL << 31) || u64PreExp > (0x1LL << 31)) {
        u64CurExp >>= 1;
        u64PreExp >>= 1;
    }

    for (i = CFG2VLD_DLY_LIMIT - 1; i >= 1; i--) {
        pstSyncCfg->u32DRCComp[i] = pstSyncCfg->u32DRCComp[i - 1];
    }

    u64CurExp = u64CurExp << DRC_COMP_SHIFT;
    u64Tmp = DIV_0_TO_1(u64PreExp);

    while (u64Tmp > (0x1LL << 31)) {
        u64Tmp >>= 1;
        u64CurExp >>= 1;
    }

    u64CurExp = osal_div64_u64(u64CurExp, u64Tmp);
    pstSyncCfg->u32DRCComp[0] = (HI_U32)u64CurExp;

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_CalcSyncCfg(ISP_SYNC_CFG_S *pstSyncCfg)
{
    HI_U32 i;
    isp_sync_cfg_buf_node *pstCurNode = HI_NULL;
    isp_sync_cfg_buf_node *pstPreNode = HI_NULL;
    HI_U64 au64CurSnsGain[2] = { 0 };
    HI_U64 au64PreSnsGain[2] = { 0 };
    HI_BOOL bErr = HI_FALSE;

    ISP_CHECK_POINTER(pstSyncCfg);

    /* update node when VCCfgNum is 0 */
    if (pstSyncCfg->u8VCCfgNum == 0) {
        for (i = CFG2VLD_DLY_LIMIT; i >= 1; i--) {
            pstSyncCfg->apstNode[i] = pstSyncCfg->apstNode[i - 1];
        }

        /* avoid skip effective AE results */
        if (ISPSyncBufIsErr(&pstSyncCfg->stSyncCfgBuf)) {
            bErr = HI_TRUE;
        }

        /* read the newest information */
        ISPSyncBufRead2(&pstSyncCfg->stSyncCfgBuf, &pstSyncCfg->apstNode[0]);
    }

    pstCurNode = pstSyncCfg->apstNode[0];

    if (pstCurNode == HI_NULL) {
        return HI_SUCCESS;
    }

    if (pstCurNode->valid == HI_FALSE) {
        return HI_SUCCESS;
    }

    if (bErr == HI_TRUE) {
        if (pstCurNode->stSnsRegsInfo.enSnsType == ISP_SNS_I2C_TYPE) {
            for (i = 0; i < pstCurNode->stSnsRegsInfo.u32RegNum; i++) {
                pstCurNode->stSnsRegsInfo.astI2cData[i].bUpdate = HI_TRUE;
            }
        } else if (pstCurNode->stSnsRegsInfo.enSnsType == ISP_SNS_SSP_TYPE) {
            for (i = 0; i < pstCurNode->stSnsRegsInfo.u32RegNum; i++) {
                pstCurNode->stSnsRegsInfo.astSspData[i].bUpdate = HI_TRUE;
            }
        } else {
            /* do nothing */
        }

        pstCurNode->stSnsRegsInfo.stSlvSync.bUpdate = HI_TRUE;
    }

    if (pstSyncCfg->apstNode[1] == HI_NULL) {
        pstPreNode = pstSyncCfg->apstNode[0];
    } else {
        if (pstSyncCfg->apstNode[1]->valid == HI_FALSE) {
            pstPreNode = pstSyncCfg->apstNode[0];
        } else {
            pstPreNode = pstSyncCfg->apstNode[1];
        }
    }

    if (IS_2to1_WDR_MODE(pstSyncCfg->u8WDRMode)) {
        if (pstSyncCfg->u8VCCfgNum == 0) {
            au64PreSnsGain[0] = pstPreNode->stAERegCfg.u64ExposureSF;
            au64PreSnsGain[0] = osal_div_u64(au64PreSnsGain[0], DIV_0_TO_1(pstPreNode->stAERegCfg.u32IntTime[0]));
            au64PreSnsGain[0] = au64PreSnsGain[0] << 8;
            au64PreSnsGain[0] = osal_div_u64(au64PreSnsGain[0], DIV_0_TO_1((HI_U64)pstPreNode->stAERegCfg.u32IspDgain * pstPreNode->stAERegCfg.au32WDRGain[0] >> 8));

            au64CurSnsGain[0] = pstCurNode->stAERegCfg.u64ExposureSF;
            au64CurSnsGain[0] = osal_div_u64(au64CurSnsGain[0], DIV_0_TO_1(pstCurNode->stAERegCfg.u32IntTime[0]));
            au64CurSnsGain[0] = au64CurSnsGain[0] << 8;
            au64CurSnsGain[0] = osal_div_u64(au64CurSnsGain[0], DIV_0_TO_1((HI_U64)pstCurNode->stAERegCfg.u32IspDgain * pstCurNode->stAERegCfg.au32WDRGain[0] >> 8));

            pstSyncCfg->u64PreSnsGainSF = au64PreSnsGain[0];
            pstSyncCfg->u64CurSnsGainSF = au64CurSnsGain[0];

            au64PreSnsGain[1] = pstPreNode->stAERegCfg.u64Exposure;
            au64PreSnsGain[1] = osal_div_u64(au64PreSnsGain[1], DIV_0_TO_1(pstPreNode->stAERegCfg.u32IntTime[1]));
            au64PreSnsGain[1] = au64PreSnsGain[1] << 8;
            au64PreSnsGain[1] = osal_div_u64(au64PreSnsGain[1], DIV_0_TO_1((HI_U64)pstPreNode->stAERegCfg.u32IspDgain * pstPreNode->stAERegCfg.au32WDRGain[1] >> 8));

            au64CurSnsGain[1] = pstCurNode->stAERegCfg.u64Exposure;
            au64CurSnsGain[1] = osal_div_u64(au64CurSnsGain[1], DIV_0_TO_1(pstCurNode->stAERegCfg.u32IntTime[1]));
            au64CurSnsGain[1] = au64CurSnsGain[1] << 8;
            au64CurSnsGain[1] = osal_div_u64(au64CurSnsGain[1], DIV_0_TO_1((HI_U64)pstCurNode->stAERegCfg.u32IspDgain * pstCurNode->stAERegCfg.au32WDRGain[1] >> 8));

            pstSyncCfg->u64PreSnsGain = au64PreSnsGain[1];
            pstSyncCfg->u64CurSnsGain = au64CurSnsGain[1];
        }

        au64PreSnsGain[0] = pstSyncCfg->u64PreSnsGainSF;
        au64CurSnsGain[0] = pstSyncCfg->u64CurSnsGainSF;
        au64PreSnsGain[1] = pstSyncCfg->u64PreSnsGain;
        au64CurSnsGain[1] = pstSyncCfg->u64CurSnsGain;
    } else {
        if (pstSyncCfg->u8VCCfgNum == 0) {
            au64PreSnsGain[0] = pstPreNode->stAERegCfg.u64Exposure;
            au64PreSnsGain[0] = osal_div_u64(au64PreSnsGain[0], DIV_0_TO_1(pstPreNode->stAERegCfg.u32IntTime[0]));
            au64PreSnsGain[0] = au64PreSnsGain[0] << 8;
            au64PreSnsGain[0] = osal_div_u64(au64PreSnsGain[0], DIV_0_TO_1(pstPreNode->stAERegCfg.u32IspDgain));

            au64CurSnsGain[0] = pstCurNode->stAERegCfg.u64Exposure;
            au64CurSnsGain[0] = osal_div_u64(au64CurSnsGain[0], DIV_0_TO_1(pstCurNode->stAERegCfg.u32IntTime[0]));
            au64CurSnsGain[0] = au64CurSnsGain[0] << 8;
            au64CurSnsGain[0] = osal_div_u64(au64CurSnsGain[0], DIV_0_TO_1(pstCurNode->stAERegCfg.u32IspDgain));

            pstSyncCfg->u64PreSnsGain = au64PreSnsGain[0];
            pstSyncCfg->u64CurSnsGain = au64CurSnsGain[0];
        }

        au64PreSnsGain[0] = pstSyncCfg->u64PreSnsGain;
        au64CurSnsGain[0] = pstSyncCfg->u64CurSnsGain;
    }

    /* calculate exposure ratio */
    ISP_DRV_CalcExpRatio(pstSyncCfg, pstCurNode, pstPreNode, au64CurSnsGain, au64PreSnsGain);

    /* calculate AlgProc */
    if (IS_LINE_WDR_MODE(pstSyncCfg->u8WDRMode)) {
        for (i = CFG2VLD_DLY_LIMIT - 1; i >= 1; i--) {
            pstSyncCfg->u8AlgProc[i] = pstSyncCfg->u8AlgProc[i - 1];
        }
        pstSyncCfg->u8AlgProc[0] = pstCurNode->stWDRRegCfg.bWDRMdtEn;
    }

    /* calculate DRC compensation */
    if (pstSyncCfg->u8VCCfgNum == 0) {
        ISP_DRV_CalcDrcComp(pstSyncCfg, pstCurNode, pstPreNode);
    }

    return HI_SUCCESS;
}

HI_U8 ISP_DRV_GetBeSyncIndex(ISP_DRV_CTX_S *pstDrvCtx)
{
    HI_U8 index;

    if (IS_ONLINE_MODE(pstDrvCtx->stWorkMode.running_mode) ||
        IS_SIDEBYSIDE_MODE(pstDrvCtx->stWorkMode.running_mode)) {
        index = (pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX - 1) / DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
    } else {
        if (pstDrvCtx->stFrameIntAttr.int_type == FRAME_INTERRUPT_START) {
            if (IS_HALF_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)) {
                index = pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX;
            } else {
                index = pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX + 1;
            }
        } else {
            if (IS_HALF_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)) {
                index = pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX - 1;
            } else {
                index = pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX;
            }
        }
    }
    return index;
}

HI_S32 ISP_DRV_RegConfigIsp(VI_PIPE ViPipe, ISP_DRV_CTX_S *pstDrvCtx)
{

    HI_S32  i;
    HI_U8   u8CfgNodeIdx, u8CfgNodeVC;
    HI_U32  u32IspDgain;
    HI_S32  s32Ret;
    HI_U32  u32Ratio[3] = { 0x40 };
    HI_U8   u8BlkDev, u8BlkNum;
    HI_U8   u8SnapBlkDev = 0;
    HI_U8   u8SnapBlkNum = 1;
    HI_U32  au32WDRGain[4]   = { 0x100, 0x100, 0x100, 0x100 };
    HI_U32  au32WDRWBGain[4] = { 0x100, 0x100, 0x100, 0x100 };
    HI_U32  au32WBViGain[4]  = { 0x100, 0x100, 0x100, 0x100 };
    HI_U16  au16CCM[9] = {
        0x01A7,  0x809D,  0x800A,
        0x804F,  0x018E,  0x803F,
        0x000B,  0x8098,  0x018D
    };
    ISP_AE_REG_CFG_2_S *pstAERegCfg = HI_NULL;
    ISP_AWB_REG_CFG_2_S *pstAWBRegCfg = HI_NULL;
    isp_sync_cfg_buf_node *pstCfgNode = HI_NULL;
    isp_be_regs_attr stIspBeRegsAttr = { 0 };
    isp_be_regs_attr stIspSnapBeRegsAttr = { 0 };
    S_ISPBE_REGS_TYPE *apstBeReg[ISP_STRIPING_MAX_NUM] = { HI_NULL };
    S_ISPBE_REGS_TYPE *apstSnapBeReg[ISP_STRIPING_MAX_NUM] = { HI_NULL };
    VI_PIPE MainPipeS;
    ISP_DRV_CTX_S *pstDrvCtxS = HI_NULL;
    VI_PIPE PicturePipe;
    ISP_DRV_CTX_S *pstDrvCtxPic = HI_NULL;
    isp_running_mode enPictureRunningMode = ISP_MODE_RUNNING_OFFLINE;
    HI_BOOL bOnlineHavePictruePipe = HI_FALSE;
    HI_U8   u8SyncIndex, u8ExpRatioIndex;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDrvCtx);

    s32Ret = ISP_DRV_GetBeRegsAttr(ViPipe, apstBeReg, &stIspBeRegsAttr);
    if (s32Ret != HI_SUCCESS) {
        return HI_SUCCESS;
    }

    if (pstDrvCtx->stSnapAttr.picture_pipe_id >= 0) {
        PicturePipe = pstDrvCtx->stSnapAttr.picture_pipe_id;
        pstDrvCtxPic = ISP_DRV_GET_CTX(PicturePipe);
        enPictureRunningMode = pstDrvCtxPic->stWorkMode.running_mode;

        if ((IS_ONLINE_MODE(enPictureRunningMode)) || \
            (IS_SIDEBYSIDE_MODE(enPictureRunningMode))) {
            if (PicturePipe != pstDrvCtx->stSnapAttr.preview_pipe_id) {
                bOnlineHavePictruePipe = HI_TRUE;
            }
        }
    }
    u8BlkDev = stIspBeRegsAttr.block_dev;
    u8BlkNum = stIspBeRegsAttr.block_num;

    /* config Chn Switch / Exp Ratio / ISP Dgain */
    /* delay of config2valid of isp reg is 1 */
    u8SyncIndex     = ISP_DRV_GetBeSyncIndex(pstDrvCtx);
    u8SyncIndex     = MIN2(u8SyncIndex, CFG2VLD_DLY_LIMIT - 1);
    u8ExpRatioIndex = u8SyncIndex;

    if (IS_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)) {
        /* Channal Switch */
        ISP_DRV_RegConfigChnSel(apstBeReg, pstDrvCtx, u8BlkNum, u8BlkDev);

        /* Ratio */
        if (IS_HALF_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)) {
            if (IS_OFFLINE_MODE(pstDrvCtx->stWorkMode.running_mode) || \
                IS_STRIPING_MODE(pstDrvCtx->stWorkMode.running_mode)) {
                u8ExpRatioIndex = u8SyncIndex + 2;
                u8ExpRatioIndex = MIN2(u8ExpRatioIndex, CFG2VLD_DLY_LIMIT - 1);
            }
        }

        for (i = 0; i < 3; i++) {
            u32Ratio[i] = pstDrvCtx->stSyncCfg.u32ExpRatio[i][u8ExpRatioIndex];
        }

        /* Cal CfgNodeIdx to ispDgain/ratio.. configs */
        if (IS_ONLINE_MODE(pstDrvCtx->stWorkMode.running_mode) || \
            IS_SIDEBYSIDE_MODE(pstDrvCtx->stWorkMode.running_mode)) {
            u8CfgNodeVC = (pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX - 1) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
        } else {
            u8CfgNodeVC = (pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX + 1) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
        }

        u8CfgNodeIdx = u8SyncIndex;
    } else {
        u8CfgNodeIdx = u8SyncIndex;
        u8CfgNodeVC  = 0;
    }

    /* when the data of sensor built-in WDR after decompand is 16bit, the ratio value is as follow. */
    if (IS_BUILT_IN_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)) {
        u32Ratio[0] = BUILT_IN_WDR_RATIO_VS_S;
        u32Ratio[1] = BUILT_IN_WDR_RATIO_S_M;
        u32Ratio[2] = BUILT_IN_WDR_RATIO_M_L;
    }

    for (i = 0; i < 3; i++) {
        u32Ratio[i] = CLIP3(u32Ratio[i], EXP_RATIO_MIN, EXP_RATIO_MAX);
    }

    pstCfgNode = pstDrvCtx->stSyncCfg.apstNode[u8CfgNodeIdx];

    if (pstCfgNode != HI_NULL) {
        if (pstDrvCtx->stSyncCfg.u8VCCfgNum == u8CfgNodeVC) {
            ISP_DRV_RegConfigWdr(ViPipe, apstBeReg, pstDrvCtx, &pstCfgNode->stWDRRegCfg, u8BlkNum, u8BlkDev, u32Ratio);

            /* config Ldci compensation */
            ISP_DRV_RegConfigLdci(apstBeReg, pstDrvCtx, u8BlkNum, u8BlkDev);
            ISP_DRV_RegConfigDe(ViPipe, apstBeReg, pstCfgNode);

            /* config drc strength */
            ISP_DRV_RegConfigDrc(ViPipe, apstBeReg, pstDrvCtx, &pstCfgNode->stDRCRegCfg, u8BlkNum, u8BlkDev, u32Ratio);
        }
    }

    if (bOnlineHavePictruePipe == HI_TRUE) {
        if (ViPipe == pstDrvCtx->stSnapAttr.picture_pipe_id) {
            return HI_SUCCESS;
        }
    }
    /* config isp_dgain & drc strength */
    pstCfgNode = pstDrvCtx->stSyncCfg.apstNode[u8CfgNodeIdx];

    if (pstDrvCtx->stSnapInfoLoad.snap_state == SNAP_STATE_CFG) {
        u32IspDgain = pstDrvCtx->stSnapInfoLoad.isp_cfg_info.isp_dgain;

        if (pstDrvCtx->stSnapAttr.load_ccm == HI_TRUE) {
            for (i = 0; i < CCM_MATRIX_SIZE; i++) {
                au16CCM[i] = pstDrvCtx->stSnapInfoLoad.isp_cfg_info.cap_ccm[i];
            }
        } else {
            if (pstCfgNode == HI_NULL) {
                return HI_SUCCESS;
            }

            for (i = 0; i < CCM_MATRIX_SIZE; i++) {
                au16CCM[i] = pstCfgNode->stAWBRegCfg.au16ColorMatrix[i];
            }
        }
    } else {
        if (pstCfgNode == HI_NULL) {
            return HI_SUCCESS;
        }

        if (pstDrvCtx->stStitchAttr.stitch_enable == HI_TRUE) {
            if (pstDrvCtx->stStitchAttr.main_pipe == HI_FALSE) {
                MainPipeS  = pstDrvCtx->stStitchAttr.stitch_bind_id[0];
                pstDrvCtxS = ISP_DRV_GET_CTX(MainPipeS);
                pstCfgNode = pstDrvCtxS->stSyncCfg.apstNode[u8CfgNodeIdx];

                if (pstCfgNode == HI_NULL) {
                    ISP_WARN_TRACE("pstCfgNode NULL point!\n");
                    return HI_SUCCESS;
                }
            }
        }

        if (pstDrvCtx->stSyncCfg.u8VCCfgNum == u8CfgNodeVC) {
            pstAERegCfg  = &pstCfgNode->stAERegCfg;
            pstAWBRegCfg = &pstCfgNode->stAWBRegCfg;
        }

        if (pstAERegCfg == HI_NULL) {
            return HI_SUCCESS;
        }

        if (pstAWBRegCfg == HI_NULL) {
            return HI_SUCCESS;
        }

        u32IspDgain = pstAERegCfg->u32IspDgain;

        for (i = 0; i < CCM_MATRIX_SIZE; i++) {
            if (pstAWBRegCfg != HI_NULL) {
                au16CCM[i] = pstAWBRegCfg->au16ColorMatrix[i];
            }
        }
    }

    u32IspDgain = CLIP3(u32IspDgain, ISP_DIGITAL_GAIN_MIN, ISP_DIGITAL_GAIN_MAX);
    if (bOnlineHavePictruePipe == HI_TRUE) {
        if (ViPipe == pstDrvCtx->stSnapAttr.preview_pipe_id) {
            ISP_CHECK_PIPE(pstDrvCtx->stSnapAttr.picture_pipe_id);
            s32Ret = ISP_DRV_GetBeRegsAttr(pstDrvCtx->stSnapAttr.picture_pipe_id,
                                           apstSnapBeReg, &stIspSnapBeRegsAttr);
            if (s32Ret == HI_SUCCESS) {
                u8SnapBlkDev = stIspSnapBeRegsAttr.block_dev;
                u8SnapBlkNum = stIspSnapBeRegsAttr.block_num;
                ISP_DRV_RegConfigDgain(apstSnapBeReg, u32IspDgain, u8SnapBlkNum, u8SnapBlkDev);
            }
        }
    }

    for (i = 0; i < 4; i++) {
        au32WDRGain[i] = pstDrvCtx->stSyncCfg.u32WDRGain[i][u8ExpRatioIndex];
        au32WDRGain[i] = CLIP3(au32WDRGain[i], ISP_DIGITAL_GAIN_MIN, ISP_DIGITAL_GAIN_MAX);

        if (pstAWBRegCfg != HI_NULL) {
            au32WDRWBGain[i] = pstAWBRegCfg->au32WDRWBGain[i];
            au32WBViGain[i]  = pstAWBRegCfg->au32BEWhiteBalanceGain[i];
        }
    }

    ISP_DRV_RegConfigDgain(apstBeReg, u32IspDgain, u8BlkNum, u8BlkDev);

    ISP_DRV_ConfigCCM(pstDrvCtx, au16CCM);

    if (pstDrvCtx->stSnapInfoLoad.snap_state == SNAP_STATE_CFG) {
        for (i = 0; i < 4; i++) {
            au32WBViGain[i]  = pstDrvCtx->stSnapInfoLoad.isp_cfg_info.white_balance_gain[i];
        }
        ISP_DRV_RegConfig4Dgain(ViPipe, apstBeReg, pstDrvCtx, au32WDRGain, u8BlkNum, u8BlkDev,
                                au32WDRWBGain, au32WBViGain);
        if ((g_UpdatePos[ViPipe] == 1) && (pstDrvCtx->stIntSch.u32IspIntStatus != 3)) {
            pstDrvCtx->stSnapInfoLoad.snap_state = SNAP_STATE_NULL;
        }
    } else {
        if (ViPipe != pstDrvCtx->stSnapAttr.picture_pipe_id) {
            ISP_DRV_RegConfig4Dgain(ViPipe, apstBeReg, pstDrvCtx, au32WDRGain, u8BlkNum, u8BlkDev,
                                    au32WDRWBGain, au32WBViGain);
        }
    }

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        pstDrvCtx->stIspBeSyncPara.isp_dgain[i] = u32IspDgain;
    }

    pstCfgNode = pstDrvCtx->stSyncCfg.apstNode[0];

    if (pstCfgNode == HI_NULL) {
        return HI_SUCCESS;
    }

    /* config Piris */
    if (pstAERegCfg != HI_NULL) {
        if (pstAERegCfg->bPirisValid == HI_TRUE) {
            if (pstDrvCtx->stPirisCb.pfn_piris_gpio_update != HI_NULL) {
                pstDrvCtx->stPirisCb.pfn_piris_gpio_update(ViPipe, &pstAERegCfg->s32PirisPos);
            }
        }
    }
    return HI_SUCCESS;
}

isp_sync_cfg_buf_node *ISP_DRV_GetSnsRegConfigNode(ISP_DRV_CTX_S *pstDrvCtx, HI_U8 u8DelayFrmNum)
{
    HI_U8 u8WDRMode, u8CfgNodeIdx, u8CfgNodeVC;
    isp_sync_cfg_buf_node *pstCfgNode = HI_NULL;
    isp_sync_cfg_buf_node *pstPreCfgNode = HI_NULL;

    u8WDRMode = pstDrvCtx->stWDRCfg.wdr_mode;
    u8CfgNodeIdx = u8DelayFrmNum / DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
    u8CfgNodeVC  = u8DelayFrmNum % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);

    if (pstDrvCtx->stSyncCfg.u8VCCfgNum == u8CfgNodeVC) {
        if (u8CfgNodeIdx > CFG2VLD_DLY_LIMIT - 1) {
            return HI_NULL;
        }

        pstCfgNode    = pstDrvCtx->stSyncCfg.apstNode[u8CfgNodeIdx];
        pstPreCfgNode = pstDrvCtx->stSyncCfg.apstNode[u8CfgNodeIdx + 1];

        if (pstCfgNode == HI_NULL) {
            return HI_NULL;
        }

        if (pstPreCfgNode == HI_NULL) {
        } else {
            if ((IS_LINEAR_MODE(u8WDRMode)) || (IS_BUILT_IN_WDR_MODE(u8WDRMode))) {
                /* not config sensor when cur == pre */
                if (pstCfgNode == pstPreCfgNode) {
                    return HI_NULL;
                }
            }
        }
    }

    return pstCfgNode;
}

HI_S32 ISP_DRV_StitchRegsCfgSensor(VI_PIPE ViPipe, ISP_DRV_CTX_S *pstDrvCtx, HI_U32 u32UpdatePos)
{
    HI_S32 i;
    HI_U32 u32SlaveDev;
    VI_PIPE MainPipeS = 0;
    isp_sync_cfg_buf_node *pstCurNode = HI_NULL;
    isp_sync_cfg_buf_node *pstCfgNode = HI_NULL;

    ISP_I2C_DATA_S *pstI2cData = HI_NULL;
    ISP_SSP_DATA_S *pstSspData = HI_NULL;
    HI_S8 s8I2cDev, s8SspDev, s8SspCs;

    pstCurNode = pstDrvCtx->stSyncCfg.apstNode[0];

    if (pstCurNode == HI_NULL) {
        ISP_WARN_TRACE("NULL point Stitch!\n");
        return HI_FAILURE;
    }

    if (pstCurNode->valid == HI_FALSE) {
        ISP_WARN_TRACE("Invalid Node Stitch!\n");
        return HI_FAILURE;
    }

    if (pstCurNode->stSnsRegsInfo.u32RegNum == 0) {
        ISP_WARN_TRACE("Err u32RegNum Stitch!\n");
        return HI_FAILURE;
    }

    u32SlaveDev = pstCurNode->stSnsRegsInfo.stSlvSync.u32SlaveBindDev;

    if (pstCurNode->stSnsRegsInfo.enSnsType == ISP_SNS_I2C_TYPE) {
        s8I2cDev = pstCurNode->stSnsRegsInfo.unComBus.s8I2cDev;

        if (s8I2cDev == -1) {
            return 0;
        }

        if (pstDrvCtx->stStitchAttr.main_pipe == HI_FALSE) {
            MainPipeS  = pstDrvCtx->stStitchAttr.stitch_bind_id[0];
            pstDrvCtx  = ISP_DRV_GET_CTX(MainPipeS);
            pstCurNode = pstDrvCtx->stSyncCfg.apstNode[0];

            if (pstCurNode == HI_NULL) {
                ISP_WARN_TRACE("pstCurNodeS NULL point Stitch!\n");
                return HI_FAILURE;
            }
        }

        for (i = 0; i < pstCurNode->stSnsRegsInfo.u32RegNum; i++) {
            pstCfgNode = ISP_DRV_GetSnsRegConfigNode(pstDrvCtx, pstCurNode->stSnsRegsInfo.astI2cData[i].u8DelayFrmNum);

            if (!pstCfgNode) {
                continue;
            }

            if (pstCfgNode->valid == HI_TRUE) {
                pstI2cData = &pstCfgNode->stSnsRegsInfo.astI2cData[i];

                if ((pstI2cData->bUpdate == HI_TRUE) && (u32UpdatePos == pstI2cData->u8IntPos)) {
                    if (pstDrvCtx->stBusCb.pfn_isp_write_i2c_data!= HI_NULL) {
                        pstDrvCtx->stBusCb.pfn_isp_write_i2c_data(s8I2cDev, pstI2cData->u8DevAddr,
                                                                  pstI2cData->u32RegAddr, pstI2cData->u32AddrByteNum,
                                                                  pstI2cData->u32Data, pstI2cData->u32DataByteNum);
                    }
                }
            }
        }
    } else if (pstCurNode->stSnsRegsInfo.enSnsType == ISP_SNS_SSP_TYPE) {
        s8SspDev = pstCurNode->stSnsRegsInfo.unComBus.s8SspDev.bit4SspDev;

        if (s8SspDev == -1) {
            return 0;
        }

        s8SspDev = pstCurNode->stSnsRegsInfo.unComBus.s8SspDev.bit4SspDev;
        s8SspCs = pstCurNode->stSnsRegsInfo.unComBus.s8SspDev.bit4SspCs;

        if (pstDrvCtx->stStitchAttr.main_pipe == HI_FALSE) {
            MainPipeS  = pstDrvCtx->stStitchAttr.stitch_bind_id[0];
            pstDrvCtx  = ISP_DRV_GET_CTX(MainPipeS);
            pstCurNode = pstDrvCtx->stSyncCfg.apstNode[0];

            if (pstCurNode == HI_NULL) {
                ISP_WARN_TRACE("pstCurNodeS NULL point Stitch!\n");
                return HI_FAILURE;
            }
        }

        for (i = 0; i < pstCurNode->stSnsRegsInfo.u32RegNum; i++) {
            pstCfgNode = ISP_DRV_GetSnsRegConfigNode(pstDrvCtx, pstCurNode->stSnsRegsInfo.astSspData[i].u8DelayFrmNum);

            if (!pstCfgNode) {
                continue;
            }

            if (pstCfgNode->valid == HI_TRUE) {
                pstSspData = &pstCfgNode->stSnsRegsInfo.astSspData[i];

                if ((pstSspData->bUpdate == HI_TRUE) && (u32UpdatePos == pstSspData->u8IntPos)) {
                    if (pstDrvCtx->stBusCb.pfn_isp_write_ssp_data!= HI_NULL) {
                        pstDrvCtx->stBusCb.pfn_isp_write_ssp_data(s8SspDev, s8SspCs, pstSspData->u32DevAddr,
                                                                  pstSspData->u32DevAddrByteNum, pstSspData->u32RegAddr,
                                                                  pstSspData->u32RegAddrByteNum, pstSspData->u32Data,
                                                                  pstSspData->u32DataByteNum);
                    }
                }
            }
        }
    }

    /* write slave sns vmax sync */
    pstCfgNode = ISP_DRV_GetSnsRegConfigNode(pstDrvCtx, pstCurNode->stSnsRegsInfo.stSlvSync.u8DelayFrmNum);

    if (pstCfgNode == HI_NULL) {
        return HI_FAILURE;
    }

    if ((pstCfgNode->valid == HI_TRUE) && (pstCfgNode->stSnsRegsInfo.stSlvSync.bUpdate == HI_TRUE)) {
        /* adjust the relationship between slavedev and vipipe */
        IO_RW_PT_ADDRESS(VICAP_SLAVE_VSTIME(u32SlaveDev)) = pstCfgNode->stSnsRegsInfo.stSlvSync.u32SlaveVsTime;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_NormalRegsCfgSensor(VI_PIPE ViPipe, ISP_DRV_CTX_S *pstDrvCtx)
{
    HI_S32 i = 0;
    HI_U32 u32SlaveDev = 0;
    isp_sync_cfg_buf_node *pstCurNode = HI_NULL;
    isp_sync_cfg_buf_node *pstCfgNode = HI_NULL;
    ISP_I2C_DATA_S *pstI2cData = HI_NULL;
    ISP_SSP_DATA_S *pstSspData = HI_NULL;
    HI_S8 s8I2cDev, s8SspDev, s8SspCs;

    pstCurNode = pstDrvCtx->stSyncCfg.apstNode[0];

    if (pstCurNode == HI_NULL) {
        ISP_WARN_TRACE("NULL point Normal!\n");
        return HI_FAILURE;
    }

    if (pstCurNode->valid == HI_FALSE) {
        ISP_WARN_TRACE("Invalid Node Normal!\n");
        return HI_FAILURE;
    }

    if (pstCurNode->stSnsRegsInfo.u32RegNum == 0) {
        ISP_WARN_TRACE("Err u32RegNum Normal!\n");
        return HI_FAILURE;
    }

    if (pstCurNode->stSnsRegsInfo.enSnsType == ISP_SNS_I2C_TYPE) {
        for (i = 0; i < pstCurNode->stSnsRegsInfo.u32RegNum; i++) {
            pstCfgNode = ISP_DRV_GetSnsRegConfigNode(pstDrvCtx, pstCurNode->stSnsRegsInfo.astI2cData[i].u8DelayFrmNum);

            if (!pstCfgNode) {
                continue;
            }

            s8I2cDev = pstCfgNode->stSnsRegsInfo.unComBus.s8I2cDev;

            if (s8I2cDev == -1) {
                return 0;
            }

            if (pstCfgNode->valid == HI_TRUE) {
                pstI2cData = &pstCfgNode->stSnsRegsInfo.astI2cData[i];

                if ((pstI2cData->bUpdate == HI_TRUE) && (pstDrvCtx->u32IntPos == pstI2cData->u8IntPos)) {
                    if (pstDrvCtx->stBusCb.pfn_isp_write_i2c_data != HI_NULL) {
                        pstDrvCtx->stBusCb.pfn_isp_write_i2c_data(s8I2cDev, pstI2cData->u8DevAddr,
                                                                  pstI2cData->u32RegAddr, pstI2cData->u32AddrByteNum,
                                                                  pstI2cData->u32Data, pstI2cData->u32DataByteNum);
                    }
                }
            }
        }
    } else if (pstCurNode->stSnsRegsInfo.enSnsType == ISP_SNS_SSP_TYPE) {
        for (i = 0; i < pstCurNode->stSnsRegsInfo.u32RegNum; i++) {
            pstCfgNode = ISP_DRV_GetSnsRegConfigNode(pstDrvCtx, pstCurNode->stSnsRegsInfo.astSspData[i].u8DelayFrmNum);

            if (!pstCfgNode) {
                continue;
            }

            s8SspDev = pstCfgNode->stSnsRegsInfo.unComBus.s8SspDev.bit4SspDev;

            if (s8SspDev == -1) {
                return 0;
            }

            s8SspDev = pstCfgNode->stSnsRegsInfo.unComBus.s8SspDev.bit4SspDev;
            s8SspCs = pstCfgNode->stSnsRegsInfo.unComBus.s8SspDev.bit4SspCs;

            if (pstCfgNode->valid == HI_TRUE) {
                pstSspData = &pstCfgNode->stSnsRegsInfo.astSspData[i];

                if ((pstSspData->bUpdate == HI_TRUE) && (pstDrvCtx->u32IntPos == pstSspData->u8IntPos)) {
                    if (pstDrvCtx->stBusCb.pfn_isp_write_ssp_data!= HI_NULL) {
                        pstDrvCtx->stBusCb.pfn_isp_write_ssp_data(s8SspDev, s8SspCs, pstSspData->u32DevAddr,
                                                                  pstSspData->u32DevAddrByteNum, pstSspData->u32RegAddr,
                                                                  pstSspData->u32RegAddrByteNum, pstSspData->u32Data,
                                                                  pstSspData->u32DataByteNum);
                    }
                }
            }
        }
    }

    /* write slave sns vmax sync */
    pstCfgNode = ISP_DRV_GetSnsRegConfigNode(pstDrvCtx, pstCurNode->stSnsRegsInfo.stSlvSync.u8DelayFrmNum);

    if (pstCfgNode == HI_NULL) {
        return HI_FAILURE;
    }

    if ((pstCfgNode->valid == HI_TRUE) && (pstCfgNode->stSnsRegsInfo.stSlvSync.bUpdate == HI_TRUE)) {
        /* adjust the relationship between slavedev and vipipe */
        u32SlaveDev = pstCfgNode->stSnsRegsInfo.stSlvSync.u32SlaveBindDev;
        IO_RW_PT_ADDRESS(VICAP_SLAVE_VSTIME(u32SlaveDev)) = pstCfgNode->stSnsRegsInfo.stSlvSync.u32SlaveVsTime;
    }

    return HI_SUCCESS;
}


HI_S32 ISP_DRV_RegConfigSensor(VI_PIPE ViPipe, ISP_DRV_CTX_S *pstDrvCtx)
{
    HI_S32 s32Ret;
    volatile  HI_U32 u32UpdatePos;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDrvCtx);

    u32UpdatePos = pstDrvCtx->u32IntPos;

    if (pstDrvCtx->stStitchAttr.stitch_enable == HI_TRUE) {
        s32Ret = ISP_DRV_StitchRegsCfgSensor(ViPipe, pstDrvCtx, u32UpdatePos);
        if (s32Ret != HI_SUCCESS) {
            ISP_WARN_TRACE("ISP_DRV_StitchRegsCfgSensor failure!\n");
            return s32Ret;
        }
    } else {
        s32Ret = ISP_DRV_NormalRegsCfgSensor(ViPipe, pstDrvCtx);
        if (s32Ret != HI_SUCCESS) {
            ISP_WARN_TRACE("ISP_DRV_NormalRegsCfgSensor failure!\n");
            return s32Ret;
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_RegisterBusCallBack(VI_PIPE ViPipe,
                               isp_bus_type enType, isp_bus_callback *pstBusCb)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_POINTER(pstBusCb);
    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    if (enType == ISP_BUS_TYPE_I2C) {
        pstDrvCtx->stBusCb.pfn_isp_write_i2c_data = pstBusCb->pfn_isp_write_i2c_data;
    } else if (enType == ISP_BUS_TYPE_SSP) {
        pstDrvCtx->stBusCb.pfn_isp_write_ssp_data = pstBusCb->pfn_isp_write_ssp_data;
    } else {
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        osal_printk("The bus type %d registerd to isp is err!\n", enType);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
    return HI_SUCCESS;
}

HI_S32 ISP_RegisterPirisCallBack(VI_PIPE ViPipe, isp_piris_callback *pstPirisCb)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_POINTER(pstPirisCb);
    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstDrvCtx->stPirisCb.pfn_piris_gpio_update = pstPirisCb->pfn_piris_gpio_update;
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_RegisterViBusCallBack(HI_S32 ViPipe, isp_vibus_callback *pstViBusCb)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;
    ISP_CHECK_POINTER(pstViBusCb);
    ISP_CHECK_PIPE(ViPipe);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstDrvCtx->stViBusCb.pfn_isp_be_sta_read = pstViBusCb->pfn_isp_be_sta_read;
    pstDrvCtx->stViBusCb.pfn_isp_be_cfg_read = pstViBusCb->pfn_isp_be_cfg_read;
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

/* vi get isp_config from isp */
HI_S32 ISP_SaveSnapConfig(VI_PIPE ViPipe, hi_isp_config_info *pstSnapInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;
    HI_U8 u8CfgDlyMax;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstSnapInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    u8CfgDlyMax = MAX2(pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX, 2);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    if (g_UpdatePos[ViPipe] == 0) {  /* frame start */
        osal_memcpy(pstSnapInfo, &pstDrvCtx->astSnapInfoSave[u8CfgDlyMax - 1], sizeof(hi_isp_config_info));
    } else {                 /* frame end */
        osal_memcpy(pstSnapInfo, &pstDrvCtx->astSnapInfoSave[u8CfgDlyMax - 2], sizeof(hi_isp_config_info));
    }
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

/* vi send Proenable */
HI_S32 ISP_SetProEnable(VI_PIPE ViPipe)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;
    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstDrvCtx->bProEnable = HI_TRUE;
    pstDrvCtx->bProStart  = HI_FALSE;
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
    return HI_SUCCESS;
}

/* vi get pubAttr */
HI_S32 ISP_GetPubAttr(VI_PIPE ViPipe, hi_isp_pub_attr *pstPubAttr)
{
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstPubAttr);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    if(!pstDrvCtx->pub_attr_ok)
    {
        return HI_FAILURE;
    }
    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    osal_memcpy(pstPubAttr, &pstDrvCtx->stProcPubInfo, sizeof(hi_isp_pub_attr));
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

/* vi get ProCtrl */
HI_BOOL ISP_GetProCtrl(VI_PIPE ViPipe, isp_pro_ctrl *pstProCtrl)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;
    HI_U8 u8CfgDlyMax;
    HI_BOOL bProStart = HI_FALSE;

    if ((ViPipe < 0) || (ViPipe >= ISP_MAX_PIPE_NUM)) {
        ISP_ERR_TRACE("Err isp pipe %d!\n", ViPipe);
        return bProStart;
    }

    if (pstProCtrl == HI_NULL) {
        ISP_ERR_TRACE("Null Pointer!\n");
        return bProStart;
    }

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    u8CfgDlyMax = MAX2(pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX, 1);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    if (g_UpdatePos[ViPipe] == 0) {
        pstProCtrl->vc_num = u8CfgDlyMax;
    } else {
        pstProCtrl->vc_num = u8CfgDlyMax - 1;
    }
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    if (pstDrvCtx->u32ProTrigFlag == 2) {
        bProStart = pstDrvCtx->bProStart;
    } else {
        bProStart = HI_FALSE;
    }

    return bProStart;
}

HI_S32 ISP_SetSnapAttr(VI_PIPE ViPipe, isp_snap_attr *pstSnapAttr)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;
    HI_U8 i;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstSnapAttr);

    for (i = 0; i < ISP_MAX_PIPE_NUM; i++) {
        pstDrvCtx = ISP_DRV_GET_CTX(i);

        if (!pstDrvCtx->bMemInit) {
            continue;
        }

        if ((i == pstSnapAttr->picture_pipe_id) || (i == pstSnapAttr->preview_pipe_id)) {
            osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
            osal_memcpy(&pstDrvCtx->stSnapAttr, pstSnapAttr, sizeof(isp_snap_attr));
            osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        }
    }
    return HI_SUCCESS;
}

static HI_S32 ISP_CheckProNrParam(hi_isp_nr_auto_attr *pstNrAttr)
{
    HI_U8 i;

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (pstNrAttr->chroma_str[0][i] > 3) {
            ISP_ERR_TRACE("Invalid chroma_str[0][%d] Input!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (pstNrAttr->chroma_str[1][i] > 3) {
            ISP_ERR_TRACE("Invalid chroma_str[1][%d] Input!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (pstNrAttr->chroma_str[2][i] > 3) {
            ISP_ERR_TRACE("Invalid chroma_str[2][%d] Input!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (pstNrAttr->chroma_str[3][i] > 3) {
            ISP_ERR_TRACE("Invalid chroma_str[3][%d] Input!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (pstNrAttr->coarse_str[0][i] > 0x360) {
            ISP_ERR_TRACE("Invalid coarse_str[0][%d] Input!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (pstNrAttr->coarse_str[1][i] > 0x360) {
            ISP_ERR_TRACE("Invalid coarse_str[1][%d] Input!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (pstNrAttr->coarse_str[2][i] > 0x360) {
            ISP_ERR_TRACE("Invalid coarse_str[2][%d] Input!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (pstNrAttr->coarse_str[3][i] > 0x360) {
            ISP_ERR_TRACE("Invalid coarse_str[3][%d] Input!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (pstNrAttr->fine_str[i] > 128) {
            ISP_ERR_TRACE("Invalid fine_str[%d] Input!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (pstNrAttr->coring_wgt[i] > 3200) {
            ISP_ERR_TRACE("Invalid coring_wgt[%d] Input!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SetProNrParam(VI_PIPE ViPipe, const hi_isp_pro_bnr_param *pstProNrParam)
{
    HI_S32 s32Ret;
    HI_U32 u32ParamIdex = 0;
    HI_U32 u32ParamNum;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    isp_pro_nr_param *pstProNrParamCtx = HI_NULL;
    hi_isp_nr_auto_attr *pstProNrAttr = HI_NULL;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstProNrParam);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstProNrParam->nr_auto_attr == HI_NULL) {
        ISP_ERR_TRACE("NrParam buf address can't null!\n");
        s32Ret =  HI_ERR_ISP_NULL_PTR;
        goto failed0;
    }

    s32Ret = read_user_linear_space_valid((HI_U8 *)(pstProNrParam->nr_auto_attr), sizeof(hi_isp_nr_auto_attr));
    if (s32Ret == HI_FALSE) {
        ISP_ERR_TRACE("Pipe:%d pastShpAttr address is invalid!\n", ViPipe);
        s32Ret =  HI_ERR_ISP_INVALID_ADDR;
        goto failed0;
    }

    ISP_CHECK_BOOL(pstProNrParam->enable);

    u32ParamNum = pstProNrParam->param_num;

    if ((u32ParamNum > PRO_MAX_FRAME_NUM) || (u32ParamNum < 1)) {
        ISP_ERR_TRACE("Invalid u32ParamNum value %d! Value range: [1, %d]\n", u32ParamNum, PRO_MAX_FRAME_NUM);
        s32Ret = HI_ERR_ISP_ILLEGAL_PARAM;
        goto failed0;
    }

    pstProNrAttr = osal_vmalloc(sizeof(hi_isp_nr_auto_attr) * u32ParamNum);
    if (pstProNrAttr == HI_NULL) {
        ISP_ERR_TRACE("Pipe:%d vmalloc for pstProNrAttr failed!\n", ViPipe);
        s32Ret = HI_ERR_ISP_NOMEM;
        goto failed0;
    }

    s32Ret = osal_copy_from_user(pstProNrAttr, pstProNrParam->nr_auto_attr, sizeof(hi_isp_nr_auto_attr) * u32ParamNum);
    if (s32Ret != 0) {
        ISP_ERR_TRACE("Pipe:%d,copy from user faild !\n", ViPipe);
        s32Ret = HI_FAILURE;
        goto failed0;
    }

    for (u32ParamIdex = 0; u32ParamIdex < u32ParamNum; u32ParamIdex++) {
        s32Ret = ISP_CheckProNrParam(&pstProNrAttr[u32ParamIdex]);
        if (s32Ret != HI_SUCCESS) {
            goto failed0;
        }
    }

    if (osal_down_interruptible(&pstDrvCtx->stProInfoSem)) {
        s32Ret = -ERESTARTSYS;
        goto failed0;
    }

    if (pstDrvCtx->stProInfo.init == HI_FALSE) {
        ISP_WARN_TRACE("ProInfo buf doesn't initialized!\n");
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto failed1;
    }

    if (pstDrvCtx->stProInfo.pro_nr_param_info.vir_addr == HI_NULL) {
        ISP_WARN_TRACE("ShpParam buf don't init ok!\n");
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto failed1;
    }

    pstProNrParamCtx = (isp_pro_nr_param *)pstDrvCtx->stProInfo.pro_nr_param_info.vir_addr;

    pstProNrParamCtx->enable = pstProNrParam->enable;
    pstProNrParamCtx->param_num = u32ParamNum;
    osal_memcpy(&pstProNrParamCtx->nr_attr, pstProNrAttr, sizeof(hi_isp_nr_auto_attr) * u32ParamNum);

    osal_up(&pstDrvCtx->stProInfoSem);

    if (pstProNrAttr != HI_NULL) {
        osal_vfree(pstProNrAttr);
    }

    return HI_SUCCESS;

failed1:
    osal_up(&pstDrvCtx->stProInfoSem);
failed0:
    if (pstProNrAttr != HI_NULL) {
        osal_vfree(pstProNrAttr);
    }

    return s32Ret;
}

HI_S32 ISP_GetProNrParam(VI_PIPE ViPipe, hi_isp_pro_bnr_param *pstProNrParam)
{
    HI_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    isp_pro_nr_param *pstProNrParamCtx = HI_NULL;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstProNrParam);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstProNrParam->nr_auto_attr == HI_NULL) {
        ISP_ERR_TRACE("NrParam buf address can't null!\n");
        return HI_ERR_ISP_NULL_PTR;
    }
    s32Ret = write_user_linear_space_valid((HI_U8 *)(pstProNrParam->nr_auto_attr), sizeof(hi_isp_nr_auto_attr));
    if (s32Ret == HI_FALSE) {
        ISP_ERR_TRACE("Pipe:%d pastNrAttr address is invalid!\n", ViPipe);
        return HI_ERR_ISP_INVALID_ADDR;
    }

    if (osal_down_interruptible(&pstDrvCtx->stProInfoSem)) {
        return -ERESTARTSYS;
    }

    if (pstDrvCtx->stProInfo.init == HI_FALSE) {
        ISP_WARN_TRACE("ProInfo buf doesn't initialized!\n");
        osal_up(&pstDrvCtx->stProInfoSem);
        return HI_ERR_ISP_NOT_INIT;
    }

    if (pstDrvCtx->stProInfo.pro_nr_param_info.vir_addr == HI_NULL) {
        ISP_WARN_TRACE("ShpParam buf don't init ok!\n");
        osal_up(&pstDrvCtx->stProInfoSem);
        return HI_ERR_ISP_NOT_INIT;
    }

    pstProNrParamCtx = (isp_pro_nr_param *)pstDrvCtx->stProInfo.pro_nr_param_info.vir_addr;

    if (pstProNrParamCtx->param_num > PRO_MAX_FRAME_NUM) {
        pstProNrParamCtx->param_num = PRO_MAX_FRAME_NUM;
    }

    if (pstProNrParamCtx->param_num < 1) {
        pstProNrParamCtx->param_num = 1;
    }

    pstProNrParam->enable    = pstProNrParamCtx->enable;
    pstProNrParam->param_num = pstProNrParamCtx->param_num;

    s32Ret = osal_copy_to_user(pstProNrParam->nr_auto_attr, &pstProNrParamCtx->nr_attr, \
                               sizeof(hi_isp_nr_auto_attr) * pstProNrParamCtx->param_num);
    if (s32Ret != 0) {
        ISP_ERR_TRACE("Pipe:%d,copy to user faild !\n", ViPipe);
        osal_up(&pstDrvCtx->stProInfoSem);
        return HI_FAILURE;
    }

    osal_up(&pstDrvCtx->stProInfoSem);

    return HI_SUCCESS;
}

static HI_S32 ISP_CheckProShpParam(hi_isp_sharpen_auto_attr *shp_attr)
{
    hi_u8 i, j;

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
            if (shp_attr->texture_str[j][i] > 4095) {
                ISP_ERR_TRACE("Invalid texture_str[%d][%d]:%d! Range:[0, 4095]\n", j, i, shp_attr->texture_str[j][i]);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
            if (shp_attr->edge_str[j][i] > 4095) {
                ISP_ERR_TRACE("Invalid edge_str[%d][%d]:%d! Range:[0, 4095]\n", j, i, shp_attr->edge_str[j][i]);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
        }

        for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
            if (shp_attr->luma_wgt[j][i] > HI_ISP_SHARPEN_LUMAWGT_MAX) {
                ISP_ERR_TRACE("Invalid luma_wgt[%d][%d]:%d! Range:[0, %d]\n",
                              j, i, shp_attr->luma_wgt[j][i], HI_ISP_SHARPEN_LUMAWGT_MAX);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
        }

        if (shp_attr->texture_freq[i] > 4095) {
            ISP_ERR_TRACE("Invalid texture_freq[%d]:%d! Range:[0, 4095]\n", i, shp_attr->texture_freq[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (shp_attr->edge_freq[i] > 4095) {
            ISP_ERR_TRACE("Invalid edge_freq[%d]:%d! Range:[0, 4095]\n", i, shp_attr->edge_freq[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (shp_attr->over_shoot[i] > 127) {
            ISP_ERR_TRACE("Invalid over_shoot[%d]:%d! Range:[0, 127]\n", i, shp_attr->over_shoot[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (shp_attr->under_shoot[i] > 127) {
            ISP_ERR_TRACE("Invalid under_shoot[%d]:%d! Range:[0, 127]\n", i, shp_attr->under_shoot[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (shp_attr->shoot_sup_adj[i] > 15) {
            ISP_ERR_TRACE("Invalid shoot_sup_adj[%d]:%d! Range:[0, 15]\n", i, shp_attr->shoot_sup_adj[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (shp_attr->edge_filt_str[i] > 63) {
            ISP_ERR_TRACE("Invalid edge_filt_str[%d]:%d! Range:[0, 63]\n", i, shp_attr->edge_filt_str[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (shp_attr->edge_filt_max_cap[i] > 47) {
            ISP_ERR_TRACE("Invalid edge_filt_max_cap[%d]:%d! Range:[0, 47]\n", i, shp_attr->edge_filt_max_cap[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (shp_attr->r_gain[i] > HI_ISP_SHARPEN_RGAIN) {
            ISP_ERR_TRACE("Invalid r_gain[%d]:%d! Range:[0, %d]\n", i, shp_attr->r_gain[i], HI_ISP_SHARPEN_RGAIN);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (shp_attr->b_gain[i] > HI_ISP_SHARPEN_BGAIN) {
            ISP_ERR_TRACE("Invalid b_gain[%d]:%d! Range:[0, %d]\n", i, shp_attr->b_gain[i], HI_ISP_SHARPEN_BGAIN);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (shp_attr->skin_gain[i] > 31) {
            ISP_ERR_TRACE("Invalid skin_gain[%d]:%d! Range:[0, 31]\n", i, shp_attr->skin_gain[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (shp_attr->max_sharp_gain[i] > 0x7FF) {
            ISP_ERR_TRACE("Invalid max_sharp_gain[%d]:%d! Range:[0, 0x7FF]\n", i, shp_attr->max_sharp_gain[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (shp_attr->weak_detail_gain[i] > 127) {
            ISP_ERR_TRACE("Invalid weak_detail_gain[%d]:%d! Range:[0, 127]\n", i, shp_attr->weak_detail_gain[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SetProShpParam(VI_PIPE ViPipe, const hi_isp_pro_sharpen_param *pstProShpParam)
{
    HI_S32 s32Ret;
    HI_U32 u32ParamIdex = 0;
    HI_U32 u32ParamNum;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    isp_pro_shp_param *pstProShpParamCtx = HI_NULL;
    hi_isp_sharpen_auto_attr *pastProShpAttr = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstProShpParam);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstProShpParam->shp_auto_attr == HI_NULL) {
        ISP_ERR_TRACE("ShpParam buf address can't null!\n");
        s32Ret = HI_ERR_ISP_NULL_PTR;
        goto failed0;
    }

    s32Ret = read_user_linear_space_valid((HI_U8 *)(pstProShpParam->shp_auto_attr), sizeof(hi_isp_sharpen_auto_attr));
    if (s32Ret == HI_FALSE) {
        ISP_ERR_TRACE("Pipe:%d pastShpAttr address is invalid!\n", ViPipe);
        s32Ret = HI_ERR_ISP_INVALID_ADDR;
        goto failed0;
    }

    ISP_CHECK_BOOL(pstProShpParam->enable);

    u32ParamNum = pstProShpParam->param_num;

    if ((u32ParamNum > PRO_MAX_FRAME_NUM) || (u32ParamNum < 1)) {
        ISP_ERR_TRACE("Invalid u32ParamNum value %d! Value range of u32ParamNum is [1, %d]\n",
                  u32ParamNum, PRO_MAX_FRAME_NUM);
        s32Ret = HI_ERR_ISP_ILLEGAL_PARAM;
        goto failed0;
    }

    pastProShpAttr = osal_vmalloc(sizeof(hi_isp_sharpen_auto_attr) * u32ParamNum);
    if (pastProShpAttr == HI_NULL) {
        ISP_ERR_TRACE("Pipe:%d vmalloc for pastProShpAttr failed!\n", ViPipe);
        s32Ret = HI_ERR_ISP_NOMEM;
        goto failed0;
    }

    s32Ret = osal_copy_from_user(pastProShpAttr, pstProShpParam->shp_auto_attr,
                                 sizeof(hi_isp_sharpen_auto_attr) * u32ParamNum);

    if (s32Ret != 0) {
        ISP_ERR_TRACE("Pipe:%d,copy from user faild !\n", ViPipe);
        s32Ret = HI_FAILURE;
        goto failed0;
    }

    for (u32ParamIdex = 0; u32ParamIdex < u32ParamNum; u32ParamIdex++) {
        s32Ret = ISP_CheckProShpParam(&pastProShpAttr[u32ParamIdex]);
        if (s32Ret != HI_SUCCESS) {
            goto failed0;
        }
    }

    if (osal_down_interruptible(&pstDrvCtx->stProInfoSem)) {
        s32Ret = -ERESTARTSYS;
        goto failed0;
    }

    if (pstDrvCtx->stProInfo.init == HI_FALSE) {
        ISP_WARN_TRACE("ProInfo buf doesn't initialized!\n");
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto failed1;
    }

    if (pstDrvCtx->stProInfo.pro_shp_param_info.vir_addr == HI_NULL) {
        ISP_WARN_TRACE("ShpParam buf don't init ok!\n");
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto failed1;
    }

    pstProShpParamCtx = (isp_pro_shp_param *)pstDrvCtx->stProInfo.pro_shp_param_info.vir_addr;

    pstProShpParamCtx->enable = pstProShpParam->enable;
    pstProShpParamCtx->param_num = u32ParamNum;
    osal_memcpy(&pstProShpParamCtx->shp_attr, pastProShpAttr, sizeof(hi_isp_sharpen_auto_attr) * u32ParamNum);

    osal_up(&pstDrvCtx->stProInfoSem);

    if (pastProShpAttr != HI_NULL) {
        osal_vfree(pastProShpAttr);
    }

    return HI_SUCCESS;

failed1:
    osal_up(&pstDrvCtx->stProInfoSem);
failed0:
    if (pastProShpAttr != HI_NULL) {
        osal_vfree(pastProShpAttr);
    }

    return s32Ret;
}

HI_S32 ISP_GetProShpParam(VI_PIPE ViPipe, hi_isp_pro_sharpen_param *pstProShpParam)
{
    HI_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    isp_pro_shp_param *pstProShpParamCtx = HI_NULL;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstProShpParam);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstProShpParam->shp_auto_attr == HI_NULL) {
        ISP_ERR_TRACE("ShpParam buf address can't null!\n");
        return HI_ERR_ISP_NULL_PTR;
    }

    s32Ret = write_user_linear_space_valid((HI_U8 *)(pstProShpParam->shp_auto_attr), sizeof(ISP_SHARPEN_AUTO_ATTR_S));
    if (s32Ret == HI_FALSE) {
        ISP_ERR_TRACE("Pipe:%d pastShpAttr address is invalid!\n", ViPipe);
        return HI_ERR_ISP_INVALID_ADDR;
    }

    if (osal_down_interruptible(&pstDrvCtx->stProInfoSem)) {
        return -ERESTARTSYS;
    }

    if (pstDrvCtx->stProInfo.init == HI_FALSE) {
        ISP_WARN_TRACE("ProInfo buf doesn't initialized!\n");
        osal_up(&pstDrvCtx->stProInfoSem);
        return HI_ERR_ISP_NOT_INIT;
    }

    if (pstDrvCtx->stProInfo.pro_shp_param_info.vir_addr == HI_NULL) {
        ISP_WARN_TRACE("ShpParam buf don't init ok!\n");
        osal_up(&pstDrvCtx->stProInfoSem);
        return HI_ERR_ISP_NOT_INIT;
    }

    pstProShpParamCtx = (isp_pro_shp_param *)pstDrvCtx->stProInfo.pro_shp_param_info.vir_addr;

    if (pstProShpParamCtx->param_num > PRO_MAX_FRAME_NUM) {
        pstProShpParamCtx->param_num = PRO_MAX_FRAME_NUM;
    }

    if (pstProShpParamCtx->param_num <  1) {
        pstProShpParamCtx->param_num = 1;
    }

    pstProShpParam->enable    = pstProShpParamCtx->enable;
    pstProShpParam->param_num = pstProShpParamCtx->param_num;

    s32Ret = osal_copy_to_user(pstProShpParam->shp_auto_attr, &pstProShpParamCtx->shp_attr, \
                               sizeof(hi_isp_sharpen_auto_attr) * pstProShpParamCtx->param_num);
    if (s32Ret != 0) {
        ISP_ERR_TRACE("Pipe:%d,copy to user faild !\n", ViPipe);
        osal_up(&pstDrvCtx->stProInfoSem);
        return HI_FAILURE;
    }

    osal_up(&pstDrvCtx->stProInfoSem);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_BeEndIntProc(VI_PIPE ViPipe)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstDrvCtx->bRunOnceOk == HI_TRUE) {
        ISP_DRV_StatBufBusyPut(ViPipe);
        pstDrvCtx->bRunOnceOk = HI_FALSE;
    }

    if (pstDrvCtx->bYUVRunOnceOk == HI_TRUE) {
        ISP_DRV_StatBufBusyPut(ViPipe);
        pstDrvCtx->bYUVRunOnceOk = HI_FALSE;
    }

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstDrvCtx->bVdBeEnd = HI_TRUE;
    osal_wakeup(&pstDrvCtx->stIspWaitVdBeEnd);
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_ProcInit(VI_PIPE ViPipe, isp_proc_mem *pstIspProcMem)
{
    HI_S32 s32Ret;
    HI_U64 u64PhyAddr;
    HI_U8  *pu8VirAddr;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_CHAR acName[MAX_MMZ_NAMELEN] = { 0 };

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstIspProcMem);

    if (g_ProcParam[ViPipe] == 0) {
        return HI_SUCCESS;
    }

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_INIT(ViPipe, pstDrvCtx->stPorcMem.init);

    osal_snprintf(acName, sizeof(acName), "ISP[%d].Proc", ViPipe);

    s32Ret = cmpi_mmz_malloc_nocache(HI_NULL, acName, &u64PhyAddr, (HI_VOID **)&pu8VirAddr, ISP_PROC_SIZE);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("alloc proc buf err\n");
        return HI_ERR_ISP_NOMEM;
    }

    ((HI_CHAR *)pu8VirAddr)[0] = '\0';
    ((HI_CHAR *)pu8VirAddr)[ISP_PROC_SIZE - 1] = '\0';

    if (osal_down_interruptible(&pstDrvCtx->stProcSem)) {
        if (u64PhyAddr != 0) {
            cmpi_mmz_free(u64PhyAddr, pu8VirAddr);
        }
        return -ERESTARTSYS;
    }

    pstDrvCtx->stPorcMem.init      = HI_TRUE;
    pstDrvCtx->stPorcMem.phy_addr  = u64PhyAddr;
    pstDrvCtx->stPorcMem.size      = ISP_PROC_SIZE;
    pstDrvCtx->stPorcMem.virt_addr = (HI_VOID *)pu8VirAddr;

    osal_memcpy(pstIspProcMem, &pstDrvCtx->stPorcMem, sizeof(isp_proc_mem));

    osal_up(&pstDrvCtx->stProcSem);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_ProcExit(VI_PIPE ViPipe)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U64 u64PhyAddr;
    HI_U8  *pu8VirAddr;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_EXIT_STATE(ViPipe);

    if (g_ProcParam[ViPipe] == 0) {
        return HI_SUCCESS;
    }

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_EXIT(ViPipe, pstDrvCtx->stPorcMem.init);

    u64PhyAddr = pstDrvCtx->stPorcMem.phy_addr;
    pu8VirAddr = (HI_U8 *)pstDrvCtx->stPorcMem.virt_addr;

    if (osal_down_interruptible(&pstDrvCtx->stProcSem)) {
        return -ERESTARTSYS;
    }

    pstDrvCtx->stPorcMem.init      = HI_FALSE;
    pstDrvCtx->stPorcMem.phy_addr  = 0;
    pstDrvCtx->stPorcMem.size      = 0;
    pstDrvCtx->stPorcMem.virt_addr = HI_NULL;
    osal_up(&pstDrvCtx->stProcSem);

    if (u64PhyAddr != 0) {
        cmpi_mmz_free(u64PhyAddr, pu8VirAddr);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_ProcPrintf(VI_PIPE ViPipe, osal_proc_entry_t *s)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U32 u32ProcBufLen;
    const HI_CHAR *pszStr;
    HI_CHAR *pszBuf;

    ISP_CHECK_PIPE(ViPipe);

    if (g_ProcParam[ViPipe] == 0) {
        return HI_SUCCESS;
    }

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (osal_down_interruptible(&pstDrvCtx->stProcSem)) {
        return -ERESTARTSYS;
    }

    if (pstDrvCtx->stPorcMem.virt_addr != HI_NULL) {
        pszBuf = osal_kmalloc((PROC_PRT_SLICE_SIZE + 1), osal_gfp_atomic);

        if (!pszBuf) {
            ISP_ERR_TRACE("ISP_DRV_ProcPrintf malloc slice buf err\n");
            osal_up(&pstDrvCtx->stProcSem);
            return HI_ERR_ISP_NULL_PTR;
        }

        pszBuf[PROC_PRT_SLICE_SIZE] = '\0';
        pszStr = (HI_CHAR *)pstDrvCtx->stPorcMem.virt_addr;
        u32ProcBufLen = osal_strlen((HI_CHAR *)pstDrvCtx->stPorcMem.virt_addr);

        while (u32ProcBufLen) {
            osal_strncpy(pszBuf, pszStr, PROC_PRT_SLICE_SIZE);
            osal_seq_printf(s, "%s", pszBuf);
            pszStr += PROC_PRT_SLICE_SIZE;

            if (u32ProcBufLen < PROC_PRT_SLICE_SIZE) {
                u32ProcBufLen = 0;
            } else {
                u32ProcBufLen -= PROC_PRT_SLICE_SIZE;
            }
        }

        osal_kfree((HI_VOID *)pszBuf);
    }

    osal_up(&pstDrvCtx->stProcSem);

    return HI_SUCCESS;
}

HI_S32 ISP_UpdateInfoSync(VI_PIPE ViPipe, hi_isp_dcf_update_info *pstIspUpdateInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;
    HI_S32 i;
    hi_isp_dcf_update_info *pUpdateInfoVirAddr = HI_NULL;
    ISP_CHECK_PIPE(ViPipe);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    if (pstDrvCtx->stTransInfo.update_info.vir_addr == HI_NULL) {
        ISP_WARN_TRACE("UpdateInfo buf don't init ok!\n");
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return HI_ERR_ISP_NOT_INIT;
    }

    pUpdateInfoVirAddr = (hi_isp_dcf_update_info *)pstDrvCtx->stTransInfo.update_info.vir_addr;

    for (i = ISP_MAX_UPDATEINFO_BUF_NUM - 1; i >= 1; i--) {
        osal_memcpy(pUpdateInfoVirAddr + i, pUpdateInfoVirAddr + i - 1, sizeof(hi_isp_dcf_update_info));
    }
    osal_memcpy(pUpdateInfoVirAddr, pstIspUpdateInfo, sizeof(hi_isp_dcf_update_info));
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_frameInfoSync(VI_PIPE ViPipe, hi_isp_frame_info *pstIspframeInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;
    HI_S32 i;
    hi_isp_frame_info *pframeInfoVirAddr = HI_NULL;
    ISP_CHECK_PIPE(ViPipe);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    if (pstDrvCtx->stTransInfo.frame_info.vir_addr == HI_NULL) {
        ISP_WARN_TRACE("UpdateInfo buf don't init ok!\n");
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return HI_ERR_ISP_NOT_INIT;
    }

    pframeInfoVirAddr = (hi_isp_frame_info *)pstDrvCtx->stTransInfo.frame_info.vir_addr;

    for (i = ISP_MAX_FRAMEINFO_BUF_NUM - 1; i >= 1; i--) {
        osal_memcpy(pframeInfoVirAddr + i, pframeInfoVirAddr + i - 1, sizeof(hi_isp_frame_info));
    }
    osal_memcpy(pframeInfoVirAddr, pstIspframeInfo, sizeof(hi_isp_frame_info));
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}


HI_S32 ISP_CalSyncInfoIndex(VI_PIPE ViPipe, HI_S32 *ps32Index)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U8 u8CfgDlyMax;
    ISP_CHECK_PIPE(ViPipe);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    u8CfgDlyMax = MAX2(pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX, 2);
    if (g_UpdatePos[ViPipe] == 0) {
        *ps32Index = u8CfgDlyMax - 1;
    } else {
        *ps32Index = u8CfgDlyMax - 2;
    }

    *ps32Index = CLIP3(*ps32Index, 0, CFG2VLD_DLY_LIMIT - 1);

    return HI_SUCCESS;
}

HI_S32 ISP_GetDCFInfo(VI_PIPE ViPipe, hi_isp_dcf_info *pstIspDCF)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    hi_isp_dcf_update_info *pstIspUpdateInfo = HI_NULL;
    hi_isp_dcf_const_info *pstIspDCFConstInfo = HI_NULL;
    unsigned long u32Flags;
    HI_S32 s32Index = 0;
    hi_isp_dcf_update_info *pUpdateInfoVirAddr = HI_NULL;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_TRANBUF_INIT(ViPipe);
    ISP_CHECK_POINTER(pstIspDCF);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    if (pstDrvCtx->stTransInfo.update_info.vir_addr == HI_NULL) {
        ISP_WARN_TRACE("UpdateInfo buf don't init ok!\n");
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return HI_ERR_ISP_NOT_INIT;
    }

    pUpdateInfoVirAddr = (hi_isp_dcf_update_info *)pstDrvCtx->stTransInfo.update_info.vir_addr;
    ISP_CalSyncInfoIndex(ViPipe, &s32Index);

    pstIspUpdateInfo = pUpdateInfoVirAddr + s32Index;

    pstIspDCFConstInfo = (hi_isp_dcf_const_info *)(pUpdateInfoVirAddr + ISP_MAX_UPDATEINFO_BUF_NUM);

    osal_memcpy(&pstIspDCF->isp_dcf_const_info, pstIspDCFConstInfo, sizeof(hi_isp_dcf_const_info));
    osal_memcpy(&pstIspDCF->isp_dcf_update_info, pstIspUpdateInfo, sizeof(hi_isp_dcf_update_info));
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_SetDCFInfo(VI_PIPE ViPipe, hi_isp_dcf_info *pstIspDCF)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    hi_isp_dcf_update_info *pstIspUpdateInfo = HI_NULL;
    hi_isp_dcf_const_info *pstIspDCFConstInfo = HI_NULL;
    unsigned long u32Flags;
    HI_S32 s32Index = 0;
    hi_isp_dcf_update_info *pUpdateInfoVirAddr = HI_NULL;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_TRANBUF_INIT(ViPipe);
    ISP_CHECK_POINTER(pstIspDCF);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    if (pstDrvCtx->stTransInfo.update_info.vir_addr == HI_NULL) {
        ISP_WARN_TRACE("UpdateInfo buf don't init ok!\n");
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return HI_ERR_ISP_NOT_INIT;
    }

    pUpdateInfoVirAddr = (hi_isp_dcf_update_info *)pstDrvCtx->stTransInfo.update_info.vir_addr;

    ISP_CalSyncInfoIndex(ViPipe, &s32Index);
    pstIspUpdateInfo = pUpdateInfoVirAddr + s32Index;

    pstIspDCFConstInfo = (hi_isp_dcf_const_info *)(pUpdateInfoVirAddr + ISP_MAX_UPDATEINFO_BUF_NUM);

    osal_memcpy(pstIspDCFConstInfo, &pstIspDCF->isp_dcf_const_info, sizeof(hi_isp_dcf_const_info));
    osal_memcpy(pstIspUpdateInfo, &pstIspDCF->isp_dcf_update_info, sizeof(hi_isp_dcf_update_info));
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_GetIspUpdateInfo(VI_PIPE ViPipe, hi_isp_dcf_update_info *pstIspUpdateInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;
    hi_isp_dcf_update_info *pUpdateInfoVirAddr = HI_NULL;
    HI_S32 s32Index = 0;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_TRANBUF_INIT(ViPipe);
    ISP_CHECK_POINTER(pstIspUpdateInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    if (pstDrvCtx->stTransInfo.update_info.vir_addr == HI_NULL) {
        ISP_WARN_TRACE("UpdateInfo buf don't init ok!\n");
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return HI_ERR_ISP_NOT_INIT;
    }

    pUpdateInfoVirAddr = (hi_isp_dcf_update_info *)pstDrvCtx->stTransInfo.update_info.vir_addr;

    ISP_CalSyncInfoIndex(ViPipe, &s32Index);
    osal_memcpy(pstIspUpdateInfo, pUpdateInfoVirAddr + s32Index, sizeof(hi_isp_dcf_update_info));

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_SetIspUpdateInfo(VI_PIPE ViPipe, hi_isp_dcf_update_info *pstIspUpdateInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;
    HI_S32 s32Index = 0;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_TRANBUF_INIT(ViPipe);
    ISP_CHECK_POINTER(pstIspUpdateInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    if (pstDrvCtx->stTransInfo.update_info.vir_addr == HI_NULL) {
        ISP_WARN_TRACE("UpdateInfo buf don't init ok!\n");
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return HI_ERR_ISP_NOT_INIT;
    }

    ISP_CalSyncInfoIndex(ViPipe, &s32Index);
    osal_memcpy((hi_isp_dcf_update_info *)pstDrvCtx->stTransInfo.update_info.vir_addr + s32Index, \
                pstIspUpdateInfo, sizeof(hi_isp_dcf_update_info));

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_ProInfoBufInit(VI_PIPE ViPipe, isp_pro_info_buf *pstProInfo)
{
    HI_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U64 u64PhyAddr;
    HI_U64 u64Size = 0;
    HI_U8  *pu8VirAddr;
    HI_U32 u32SizeProNrParam, u32SizeProShpParam;
    HI_CHAR acName[MAX_MMZ_NAMELEN] = { 0 };

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstProInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_INIT(ViPipe, pstDrvCtx->stProInfo.init);

    osal_snprintf(acName, sizeof(acName), "ISP[%d].Pro", ViPipe);

    u32SizeProNrParam  = sizeof(isp_pro_nr_param);
    u32SizeProShpParam = sizeof(isp_pro_shp_param);

    u64Size += u32SizeProNrParam;
    u64Size += u32SizeProShpParam;

    s32Ret = cmpi_mmz_malloc_nocache(HI_NULL, acName, &u64PhyAddr, (HI_VOID **)&pu8VirAddr, u64Size);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] alloc ISP Pro info buf err\n", ViPipe);
        return HI_ERR_ISP_NOMEM;
    }

    osal_memset(pu8VirAddr, 0, u64Size);

    if (osal_down_interruptible(&pstDrvCtx->stProInfoSem)) {
        if (u64PhyAddr != 0) {
            cmpi_mmz_free(u64PhyAddr, pu8VirAddr);
        }
        return -ERESTARTSYS;
    }
    pstDrvCtx->stProInfo.init= HI_TRUE;

    pstDrvCtx->stProInfo.pro_nr_param_info.phy_addr = u64PhyAddr;
    pstDrvCtx->stProInfo.pro_nr_param_info.vir_addr = (HI_VOID *)pu8VirAddr;

    pstDrvCtx->stProInfo.pro_shp_param_info.phy_addr = \
            pstDrvCtx->stProInfo.pro_nr_param_info.phy_addr+ u32SizeProNrParam;;
    pstDrvCtx->stProInfo.pro_shp_param_info.vir_addr = \
            (HI_VOID *)((HI_U8 *)pstDrvCtx->stProInfo.pro_nr_param_info.vir_addr + u32SizeProNrParam);

    osal_memcpy(pstProInfo, &pstDrvCtx->stProInfo, sizeof(isp_pro_info_buf));

    osal_up(&pstDrvCtx->stProInfoSem);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_ProInfoBufExit(VI_PIPE ViPipe)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U64 u64PhyAddr;
    HI_U8  *pu8VirAddr;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_EXIT_STATE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_EXIT(ViPipe, pstDrvCtx->stProInfo.init);

    u64PhyAddr = pstDrvCtx->stProInfo.pro_nr_param_info.phy_addr;
    pu8VirAddr = (HI_U8 *)pstDrvCtx->stProInfo.pro_nr_param_info.vir_addr;

    if (osal_down_interruptible(&pstDrvCtx->stProInfoSem)) {
        return -ERESTARTSYS;
    }

    pstDrvCtx->stProInfo.init                        = HI_FALSE;

    pstDrvCtx->stProInfo.pro_nr_param_info.phy_addr  = 0;
    pstDrvCtx->stProInfo.pro_nr_param_info.vir_addr  = HI_NULL;

    pstDrvCtx->stProInfo.pro_shp_param_info.phy_addr = 0;
    pstDrvCtx->stProInfo.pro_shp_param_info.vir_addr = HI_NULL;

    osal_up(&pstDrvCtx->stProInfoSem);

    if (u64PhyAddr != 0) {
        cmpi_mmz_free(u64PhyAddr, pu8VirAddr);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_TransInfoBufInit(VI_PIPE ViPipe, isp_trans_info_buf *pstTransInfo)
{
    HI_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U64 u64PhyAddr;
    HI_U64 u64Size = 0;
    HI_U8  *pu8VirAddr;
    HI_U32 u32SizeDngInfo;
    HI_U32 u32SizeUpdateInfo, u32SizeFrameInfo;
    HI_U32 u32SizeAttachInfo, u32SizeColorGammut;
    HI_CHAR acName[MAX_MMZ_NAMELEN] = { 0 };
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstTransInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_INIT(ViPipe, pstDrvCtx->stTransInfo.init);

    osal_snprintf(acName, sizeof(acName), "ISP[%d].Trans", ViPipe);

    u32SizeDngInfo     = sizeof(hi_isp_dng_image_static_info);
    u32SizeAttachInfo  = sizeof(hi_isp_attach_info);
    u32SizeColorGammut = sizeof(hi_isp_colorgammut_info);
    u32SizeFrameInfo   = sizeof(hi_isp_frame_info) * ISP_MAX_FRAMEINFO_BUF_NUM;
    u32SizeUpdateInfo  = sizeof(hi_isp_dcf_update_info) * ISP_MAX_UPDATEINFO_BUF_NUM + sizeof(hi_isp_dcf_const_info);

    u64Size += u32SizeDngInfo;
    u64Size += u32SizeAttachInfo;
    u64Size += u32SizeColorGammut;
    u64Size += u32SizeFrameInfo;
    u64Size += u32SizeUpdateInfo;

    s32Ret = cmpi_mmz_malloc_nocache(HI_NULL, acName, &u64PhyAddr, (HI_VOID **)&pu8VirAddr, u64Size);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] alloc ISP Trans info buf err\n", ViPipe);
        return HI_ERR_ISP_NOMEM;
    }

    osal_memset(pu8VirAddr, 0, u64Size);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    pstDrvCtx->stTransInfo.init               = HI_TRUE;

    pstDrvCtx->stTransInfo.dng_info.phy_addr  = u64PhyAddr;
    pstDrvCtx->stTransInfo.dng_info.vir_addr  = (HI_VOID *)pu8VirAddr;

    pstDrvCtx->stTransInfo.atta_info.phy_addr = \
            pstDrvCtx->stTransInfo.dng_info.phy_addr+ u32SizeDngInfo;
    pstDrvCtx->stTransInfo.atta_info.vir_addr = \
            (HI_VOID *)((HI_U8 *)pstDrvCtx->stTransInfo.dng_info.vir_addr+ u32SizeDngInfo);

    pstDrvCtx->stTransInfo.color_gammut_info.phy_addr = \
            pstDrvCtx->stTransInfo.atta_info.phy_addr+ u32SizeAttachInfo;
    pstDrvCtx->stTransInfo.color_gammut_info.vir_addr = \
            (HI_VOID *)((HI_U8 *)pstDrvCtx->stTransInfo.atta_info.vir_addr+ u32SizeAttachInfo);

    pstDrvCtx->stTransInfo.frame_info.phy_addr = \
            pstDrvCtx->stTransInfo.color_gammut_info.phy_addr+ u32SizeColorGammut;;
    pstDrvCtx->stTransInfo.frame_info.vir_addr = \
            (HI_VOID *)((HI_U8 *)pstDrvCtx->stTransInfo.color_gammut_info.vir_addr+ u32SizeColorGammut);

    pstDrvCtx->stTransInfo.update_info.phy_addr = \
            pstDrvCtx->stTransInfo.frame_info.phy_addr+ u32SizeFrameInfo;;
    pstDrvCtx->stTransInfo.update_info.vir_addr = \
            (HI_VOID *)((HI_U8 *)pstDrvCtx->stTransInfo.frame_info.vir_addr+ u32SizeFrameInfo);

    osal_memcpy(pstTransInfo, &pstDrvCtx->stTransInfo, sizeof(isp_trans_info_buf));

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_TransInfoBufExit(VI_PIPE ViPipe)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U64 u64PhyAddr;
    HI_U8  *pu8VirAddr;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_EXIT_STATE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_EXIT(ViPipe, pstDrvCtx->stTransInfo.init);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    u64PhyAddr = pstDrvCtx->stTransInfo.dng_info.phy_addr;
    pu8VirAddr = (HI_U8 *)pstDrvCtx->stTransInfo.dng_info.vir_addr;

    pstDrvCtx->stTransInfo.init                       = HI_FALSE;

    pstDrvCtx->stTransInfo.dng_info.phy_addr          = 0;
    pstDrvCtx->stTransInfo.dng_info.vir_addr          = HI_NULL;

    pstDrvCtx->stTransInfo.atta_info.phy_addr         = 0;
    pstDrvCtx->stTransInfo.atta_info.vir_addr         = HI_NULL;

    pstDrvCtx->stTransInfo.color_gammut_info.phy_addr = 0;
    pstDrvCtx->stTransInfo.color_gammut_info.vir_addr = HI_NULL;

    pstDrvCtx->stTransInfo.frame_info.phy_addr        = 0;
    pstDrvCtx->stTransInfo.frame_info.vir_addr        = HI_NULL;

    pstDrvCtx->stTransInfo.update_info.phy_addr       = 0;
    pstDrvCtx->stTransInfo.update_info.vir_addr       = HI_NULL;

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    if (u64PhyAddr != 0) {
        cmpi_mmz_free(u64PhyAddr, pu8VirAddr);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SetFrameInfo(VI_PIPE ViPipe, hi_isp_frame_info *pstIspFrame)
{
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_S32 s32Index = 0;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_TRANBUF_INIT(ViPipe);
    ISP_CHECK_POINTER(pstIspFrame);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    if (pstDrvCtx->stTransInfo.frame_info.vir_addr == HI_NULL) {
        ISP_WARN_TRACE("frameinfo buf don't init ok!\n");
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return HI_ERR_ISP_NOT_INIT;
    }
    ISP_CalSyncInfoIndex(ViPipe, &s32Index);

    osal_memcpy((hi_isp_frame_info *)pstDrvCtx->stTransInfo.frame_info.vir_addr, pstIspFrame,
                sizeof(hi_isp_frame_info));
    osal_memcpy((hi_isp_frame_info *)pstDrvCtx->stTransInfo.frame_info.vir_addr + s32Index, \
                pstIspFrame, sizeof(hi_isp_frame_info));

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_GetFrameInfo(VI_PIPE ViPipe, hi_isp_frame_info *pstIspFrame)
{
    HI_U8 u8ViPipeS;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_DRV_CTX_S *pstDrvCtxS = HI_NULL;
    unsigned long u32Flags;
    HI_S32 s32Index = 0;
    hi_isp_frame_info *pFrameInfoVirAddr = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_TRANBUF_INIT(ViPipe);
    ISP_CHECK_POINTER(pstIspFrame);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if ((pstDrvCtx->bMemInit == HI_FALSE) && (pstDrvCtx->stWDRAttr.wdr_mode == WDR_MODE_NONE)) {
        for (u8ViPipeS = 0; u8ViPipeS < ISP_MAX_PIPE_NUM; u8ViPipeS++) {
            pstDrvCtxS = ISP_DRV_GET_CTX(u8ViPipeS);

            if ((pstDrvCtxS->bMemInit == HI_TRUE) && (IS_WDR_MODE(pstDrvCtxS->stWDRAttr.wdr_mode))) {

                osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

                if (pstIspFrame && pstDrvCtxS->stTransInfo.frame_info.vir_addr) {
                    pFrameInfoVirAddr = (hi_isp_frame_info *)pstDrvCtxS->stTransInfo.frame_info.vir_addr;
                    ISP_CalSyncInfoIndex(ViPipe, &s32Index);
                    osal_memcpy(pstIspFrame, pFrameInfoVirAddr + s32Index, sizeof(hi_isp_frame_info));
                }

                osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
            }
        }
    } else {
        osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

        if (pstIspFrame && pstDrvCtx->stTransInfo.frame_info.vir_addr) {
            pFrameInfoVirAddr = (hi_isp_frame_info *)pstDrvCtx->stTransInfo.frame_info.vir_addr;
            ISP_CalSyncInfoIndex(ViPipe, &s32Index);
            osal_memcpy(pstIspFrame, pFrameInfoVirAddr + s32Index, sizeof(hi_isp_frame_info));
        }

        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_GetAttachInfo(VI_PIPE ViPipe, hi_isp_attach_info *pstIspAttachInfo)
{
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_TRANBUF_INIT(ViPipe);
    ISP_CHECK_POINTER(pstIspAttachInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    if (pstIspAttachInfo && pstDrvCtx->stTransInfo.atta_info.vir_addr) {
        osal_memcpy(pstIspAttachInfo, (hi_isp_attach_info *)pstDrvCtx->stTransInfo.atta_info.vir_addr, \
                    sizeof(hi_isp_attach_info));
    }

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_GetColorGamutInfo(VI_PIPE ViPipe, hi_isp_colorgammut_info *pstIspColorGamutInfo)
{
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_TRANBUF_INIT(ViPipe);
    ISP_CHECK_POINTER(pstIspColorGamutInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    if (pstIspColorGamutInfo && pstDrvCtx->stTransInfo.color_gammut_info.vir_addr) {
        osal_memcpy(pstIspColorGamutInfo, (hi_isp_colorgammut_info *)pstDrvCtx->stTransInfo.color_gammut_info.vir_addr,
                    sizeof(hi_isp_colorgammut_info));
    }

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_GetDngInfo(VI_PIPE ViPipe, hi_isp_dng_image_static_info *pstDngInfo)
{
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_TRANBUF_INIT(ViPipe);
    ISP_CHECK_POINTER(pstDngInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    if (pstDngInfo && pstDrvCtx->stTransInfo.dng_info.vir_addr) {
        osal_memcpy(pstDngInfo, (hi_isp_dng_image_static_info *)pstDrvCtx->stTransInfo.dng_info.vir_addr, \
                    sizeof(hi_isp_dng_image_static_info));
    }

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_GetDngImageDynamicInfo(VI_PIPE ViPipe, hi_dng_image_dynamic_info *pstDngImageDynamicInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (g_UpdatePos[ViPipe] == 0) {  /* frame start */
        osal_memcpy(pstDngImageDynamicInfo, &pstDrvCtx->stDngImageDynamicInfo[1], sizeof(hi_dng_image_dynamic_info));
    } else {
        osal_memcpy(pstDngImageDynamicInfo, &pstDrvCtx->stDngImageDynamicInfo[0], sizeof(hi_dng_image_dynamic_info));
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SetModParam(hi_isp_mod_param *pstModParam)
{
    VI_PIPE ViPipe;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_POINTER(pstModParam);

    for (ViPipe = 0; ViPipe < ISP_MAX_PIPE_NUM; ViPipe++) {
        pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

        if (pstDrvCtx->bMemInit == HI_TRUE) {
            ISP_ERR_TRACE("Does not support changed after isp init!\n");
            return HI_ERR_ISP_NOT_SUPPORT;
        }
    }

    if ((pstModParam->int_bot_half != 0) && (pstModParam->int_bot_half != 1)) {
        ISP_ERR_TRACE("u32IntBotHalf must be 0 or 1.\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstModParam->quick_start != 0) && (pstModParam->quick_start != 1)) {
        ISP_ERR_TRACE("u32QuickStart must be 0 or 1.\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    g_IntBothalf = pstModParam->int_bot_half;
    g_QuickStart = pstModParam->quick_start;
#ifndef __HuaweiLite__
    if (g_IntBothalf) {
        g_UseBothalf = HI_TRUE;
    }
#else
#ifdef CONFIG_HI_ISP_LITEOS_BOTTOM_HALF_SUPPORT
    if (g_IntBothalf) {
        g_UseBothalf = HI_TRUE;
    }
#endif
#endif

    return HI_SUCCESS;
}

HI_S32 ISP_GetModParam(hi_isp_mod_param *pstModParam)
{
    ISP_CHECK_POINTER(pstModParam);

    pstModParam->int_bot_half = g_IntBothalf;
    pstModParam->quick_start  = g_QuickStart;
    return HI_SUCCESS;
}

HI_S32 ISP_SetCtrlParam(VI_PIPE ViPipe, hi_isp_ctrl_param *pstIspCtrlParam)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstIspCtrlParam);
    ISP_CHECK_BOOL(pstIspCtrlParam->ldci_tpr_flt_en);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    /* isp proc can be closed before mem init */
    /* if isp proc is opened, cannot change u32ProcParam to 0 after mem init */
    if ((g_ProcParam[ViPipe] != 0) && (pstIspCtrlParam->proc_param == 0) && (pstDrvCtx->bMemInit == HI_TRUE)) {
        ISP_ERR_TRACE("Vipipe:%d proc_param do not support to change %d to 0.\n",
                  ViPipe, g_ProcParam[ViPipe]);
        return HI_ERR_ISP_NOT_SUPPORT;
    }

    /* if isp proc is closed, cannot change u32ProcParam to Non-0 after mem init */
    if ((g_ProcParam[ViPipe] == 0) && (pstIspCtrlParam->proc_param != 0) && (pstDrvCtx->bMemInit == HI_TRUE)) {
        ISP_ERR_TRACE("Vipipe:%d proc_param do not support to change %d to Non-0.\n",
                  ViPipe, g_ProcParam[ViPipe]);
        return HI_ERR_ISP_NOT_SUPPORT;
    }

    if (!pstIspCtrlParam->stat_intvl) {
        ISP_ERR_TRACE("Vipipe:%d stat_intvl must be larger than 0.\n", ViPipe);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstIspCtrlParam->update_pos != 0) && (pstIspCtrlParam->update_pos != 1)) {
        ISP_ERR_TRACE("Vipipe:%d update_pos must be 0 or 1.\n", ViPipe);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (g_UpdatePos[ViPipe] != pstIspCtrlParam->update_pos) {
        if (pstDrvCtx->bMemInit == HI_TRUE) {
            ISP_ERR_TRACE("Vipipe:%d Does not support changed after isp init (update_pos)!\n", ViPipe);
            return HI_ERR_ISP_NOT_SUPPORT;
        }
    }

    if (g_PwmNumber[ViPipe] != pstIspCtrlParam->pwm_number) {
        if (pstDrvCtx->bMemInit == HI_TRUE) {
            ISP_ERR_TRACE("Vipipe:%d Does not support changed after isp init (pwm_number)!\n", ViPipe);
            return HI_ERR_ISP_NOT_SUPPORT;
        }
    }

    if (g_PortIntDelay[ViPipe] != pstIspCtrlParam->port_int_delay) {
        if (pstDrvCtx->bMemInit == HI_TRUE) {
            ISP_ERR_TRACE("Vipipe:%d Does not support changed after isp init (port_int_delay)!\n", ViPipe);
            return HI_ERR_ISP_NOT_SUPPORT;
        }
    }

    if (g_LdciTprFltEn[ViPipe] != pstIspCtrlParam->ldci_tpr_flt_en) {
        if (pstDrvCtx->bMemInit == HI_TRUE) {
            ISP_ERR_TRACE("Vipipe:%d Does not support changed after isp init (ldci_tpr_flt_en)!\n", ViPipe);
            return HI_ERR_ISP_NOT_SUPPORT;
        }
    }

    g_ProcParam[ViPipe]    = pstIspCtrlParam->proc_param;
    g_StatIntvl[ViPipe]    = pstIspCtrlParam->stat_intvl;
    g_UpdatePos[ViPipe]    = pstIspCtrlParam->update_pos;
    g_IntTimeout[ViPipe]   = pstIspCtrlParam->int_time_out;
    g_PwmNumber[ViPipe]    = pstIspCtrlParam->pwm_number;
    g_PortIntDelay[ViPipe] = pstIspCtrlParam->port_int_delay;
    g_LdciTprFltEn[ViPipe] = pstIspCtrlParam->ldci_tpr_flt_en;

    return HI_SUCCESS;
}

HI_S32 ISP_GetCtrlParam(VI_PIPE ViPipe, hi_isp_ctrl_param *pstIspCtrlParam)
{
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstIspCtrlParam);

    pstIspCtrlParam->proc_param      = g_ProcParam[ViPipe];
    pstIspCtrlParam->stat_intvl      = g_StatIntvl[ViPipe];
    pstIspCtrlParam->update_pos      = g_UpdatePos[ViPipe];
    pstIspCtrlParam->int_time_out    = g_IntTimeout[ViPipe];
    pstIspCtrlParam->pwm_number      = g_PwmNumber[ViPipe];
    pstIspCtrlParam->port_int_delay  = g_PortIntDelay[ViPipe];
    pstIspCtrlParam->ldci_tpr_flt_en = g_LdciTprFltEn[ViPipe];

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_StitchSyncEx(VI_PIPE ViPipe)
{
    HI_U8 k;
    VI_PIPE ViPipeId;
    ISP_DRV_CTX_S *pstDrvCtx  = HI_NULL;
    ISP_DRV_CTX_S *pstDrvCtxS = HI_NULL;

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    for (k = 0; k < pstDrvCtx->stStitchAttr.stitch_pipe_num; k++) {
        ViPipeId = pstDrvCtx->stStitchAttr.stitch_bind_id[k];
        pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeId);

        if (pstDrvCtxS->bStitchSync != HI_TRUE) {
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;

}

HI_S32 ISP_DRV_StitchSync(VI_PIPE ViPipe)
{
    HI_U8 k;
    VI_PIPE ViPipeId;
    ISP_DRV_CTX_S *pstDrvCtx  = HI_NULL;
    ISP_DRV_CTX_S *pstDrvCtxS = HI_NULL;

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    for (k = 0; k < pstDrvCtx->stStitchAttr.stitch_pipe_num; k++) {
        ViPipeId = pstDrvCtx->stStitchAttr.stitch_bind_id[k];
        pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeId);

        if (pstDrvCtxS->bIspInit != HI_TRUE) {
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_GetVersion(ISP_VERSION_S *pVersion)
{
    ISP_CHECK_POINTER(pVersion);

    osal_memcpy(&gs_stIspLibInfo, pVersion, sizeof(ISP_VERSION_S));

    gs_stIspLibInfo.u32Magic = VERSION_MAGIC + ISP_MAGIC_OFFSET;
    osal_memcpy(pVersion, &gs_stIspLibInfo, sizeof(ISP_VERSION_S));

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_SetDngInfo(VI_PIPE ViPipe, hi_dng_image_dynamic_info *pstDngImgDynInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDngImgDynInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_memcpy(&pstDrvCtx->stDngImageDynamicInfo[1], &pstDrvCtx->stDngImageDynamicInfo[0], \
                sizeof(hi_dng_image_dynamic_info));
    osal_memcpy(&pstDrvCtx->stDngImageDynamicInfo[0], pstDngImgDynInfo, \
                sizeof(hi_dng_image_dynamic_info));

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_GetUserStatBuf(VI_PIPE ViPipe, isp_stat_info *pstStat)
{
    isp_stat_info *pstIspStatInfo = HI_NULL;
    ISP_CHECK_POINTER(pstStat);

    ISP_DRV_StatBufUserGet(ViPipe, &pstIspStatInfo);

    if (pstIspStatInfo == HI_NULL) {
        return HI_FAILURE;
    }

    osal_memcpy(pstStat, pstIspStatInfo, sizeof(isp_stat_info));

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_GetRgbirFormat(VI_PIPE ViPipe, hi_isp_ir_bayer_format *pRgbirFormat)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pRgbirFormat);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    if(!pstDrvCtx->bIspInit)
    {
        return HI_FAILURE;
    }
    *pRgbirFormat = pstDrvCtx->enIspRgbirFormat;
    return HI_SUCCESS;
}
HI_S32 ISP_DRV_GetStatInfoActive(VI_PIPE ViPipe, isp_stat_info *pstIspStatInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    isp_stat_info stActStatInfo;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstIspStatInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (!pstDrvCtx->stStatisticsBuf.pstActStat) {
        ISP_WARN_TRACE("Pipe[%d] get statistic active buffer err, stat not ready!\n", ViPipe);
        return HI_FAILURE;
    }

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    osal_memcpy(&stActStatInfo, pstDrvCtx->stStatisticsBuf.pstActStat, sizeof(isp_stat_info));
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    osal_memcpy(pstIspStatInfo, &stActStatInfo, sizeof(isp_stat_info));

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_SetRegKernelCfgs(VI_PIPE ViPipe, isp_kernel_reg_cfg *pstRegKernelCfg)
{
    HI_U32 u32Flag;
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstRegKernelCfg);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if ((pstDrvCtx->u32RegCfgInfoFlag != 0) &&
        (pstDrvCtx->u32RegCfgInfoFlag != 1)) {
        ISP_ERR_TRACE("Pipe[%d] Err u32RegCfgInfoFlag != 0/1 !!!\n", ViPipe);
    }

    u32Flag = 1 - pstDrvCtx->u32RegCfgInfoFlag;
    osal_memcpy(&pstDrvCtx->astKernelCfg[u32Flag], pstRegKernelCfg, sizeof(isp_kernel_reg_cfg));

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstDrvCtx->u32RegCfgInfoFlag = u32Flag;
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_SetConfigInfo(VI_PIPE ViPipe, hi_isp_config_info *pstIspConfigInfo)
{
    HI_U32 i;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstIspConfigInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    for (i = ISP_SAVEINFO_MAX - 1; i >= 1; i--) {
        osal_memcpy(&pstDrvCtx->astSnapInfoSave[i], &pstDrvCtx->astSnapInfoSave[i - 1], sizeof(hi_isp_config_info));
    }

    osal_memcpy(&pstDrvCtx->astSnapInfoSave[0], pstIspConfigInfo, sizeof(hi_isp_config_info));

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_GetSnapInfo(VI_PIPE ViPipe, isp_snap_info *pstIspSnapInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstIspSnapInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_memcpy(pstIspSnapInfo, &pstDrvCtx->stSnapInfoLoad, sizeof(isp_snap_info));

    if (pstDrvCtx->stSnapInfoLoad.snap_state == SNAP_STATE_CFG) {
        if (g_UpdatePos[ViPipe] == 0) {
            pstDrvCtx->stSnapInfoLoad.snap_state = SNAP_STATE_NULL;
        }
    }

    return HI_SUCCESS;
}

HI_VOID ISP_DRV_BeBufQueuePutBusy(VI_PIPE ViPipe)
{
    HI_U64  u64PhyAddr;
    HI_U64  u64Size;
    HI_VOID *pVirAddr;
    ISP_BE_BUF_NODE_S *pstNode;
    struct osal_list_head *pListTmp = HI_NULL;
    struct osal_list_head *pListNode = HI_NULL;
    ISP_DRV_CTX_S *pstDrvCtx  = HI_NULL;

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (!pstDrvCtx->pstUseNode) {
        return;
    }

    osal_list_for_each_safe(pListNode, pListTmp, &pstDrvCtx->stBeBufQueue.stBusyList) {
        pstNode = osal_list_entry(pListNode, ISP_BE_BUF_NODE_S, list);

        if (pstNode->s32HoldCnt == 0) {
            ISP_QueueDelBusyBeBuf(&pstDrvCtx->stBeBufQueue, pstNode);
            ISP_QueuePutFreeBeBuf(&pstDrvCtx->stBeBufQueue, pstNode);
        }
    }

    u64PhyAddr = pstDrvCtx->pstUseNode->stBeCfgBuf.phy_addr;
    pVirAddr   = pstDrvCtx->pstUseNode->stBeCfgBuf.vir_addr;
    u64Size    = pstDrvCtx->pstUseNode->stBeCfgBuf.size;

    osal_flush_dcache_area(pVirAddr, u64PhyAddr, u64Size);

    ISP_QueuePutBusyBeBuf(&pstDrvCtx->stBeBufQueue, pstDrvCtx->pstUseNode);

    pstDrvCtx->pstUseNode = HI_NULL;

    return;
}

HI_S32 ISP_DRV_RunOnceProcess(VI_PIPE ViPipe)
{
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_SYNC_CFG_S *pstSyncCfg = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstDrvCtx->bMemInit == HI_FALSE) {
        return HI_ERR_ISP_MEM_NOT_INIT;
    }

    pstSyncCfg = &pstDrvCtx->stSyncCfg;

    ISP_DRV_GetSyncControlnfo(ViPipe, pstSyncCfg);
    ISP_DRV_CalcSyncCfg(pstSyncCfg);
    ISP_DRV_RegConfigIsp(ViPipe, pstDrvCtx);
    ISP_DRV_RegConfigSensor(ViPipe, pstDrvCtx);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    if (pstDrvCtx->bRunOnceFlag == HI_TRUE) {
        ISP_DRV_BeBufQueuePutBusy(ViPipe);
    }
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    pstDrvCtx->bRunOnceOk = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_YUVRunOnceProcess(VI_PIPE ViPipe)
{
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstDrvCtx->bMemInit == HI_FALSE) {
        return HI_ERR_ISP_MEM_NOT_INIT;
    }

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    if (pstDrvCtx->bYUVRunOnceFlag == HI_TRUE) {
        ISP_DRV_BeBufQueuePutBusy(ViPipe);
    }
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    pstDrvCtx->bYUVRunOnceOk = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_OptRunOnceInfo(VI_PIPE ViPipe, HI_BOOL *pbRunOnceFlag)
{
    HI_BOOL bRunOnceFlag;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pbRunOnceFlag);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    bRunOnceFlag = *pbRunOnceFlag;

    if (bRunOnceFlag == HI_TRUE) {
        if (pstDrvCtx->pstUseNode) {
            osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
            ISP_ERR_TRACE("Pipe[%d] isp is running!\r\n", ViPipe);
            return HI_FAILURE;
        }

        pstDrvCtx->pstUseNode = ISP_QueueGetFreeBeBuf(&pstDrvCtx->stBeBufQueue);

        if (pstDrvCtx->pstUseNode == HI_NULL) {
            osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
            ISP_ERR_TRACE("Pipe[%d] get FreeBeBuf is fail!\r\n", ViPipe);

            return HI_FAILURE;
        }
    }

    pstDrvCtx->bRunOnceFlag = bRunOnceFlag;

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_YUVRunOnceInfo(VI_PIPE ViPipe, HI_BOOL *pbRunOnceFlag)
{
    HI_BOOL bYUVRunOnceFlag;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pbRunOnceFlag);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    bYUVRunOnceFlag = *pbRunOnceFlag;

    if (bYUVRunOnceFlag == HI_TRUE) {
        if (pstDrvCtx->pstUseNode) {
            osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
            ISP_ERR_TRACE("Pipe[%d] isp is running!\r\n", ViPipe);
            return HI_FAILURE;
        }

        pstDrvCtx->pstUseNode = ISP_QueueGetFreeBeBuf(&pstDrvCtx->stBeBufQueue);

        if (pstDrvCtx->pstUseNode == HI_NULL) {
            osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
            ISP_ERR_TRACE("Pipe[%d] get FreeBeBuf is fail!\r\n", ViPipe);

            return HI_FAILURE;
        }
    }

    pstDrvCtx->bYUVRunOnceFlag = bYUVRunOnceFlag;

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

HI_VOID ISP_DRV_StitchBeBufCtl(VI_PIPE ViPipe)
{
    HI_S32  i;
    HI_S32  s32Ret;
    VI_PIPE ViPipeS;
    VI_PIPE MainPipeS;
    ISP_DRV_CTX_S *pstDrvCtx  = HI_NULL;
    ISP_DRV_CTX_S *pstDrvCtxS = HI_NULL;
    unsigned long u32Flags;

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    MainPipeS = pstDrvCtx->stStitchAttr.stitch_bind_id[0];

    osal_spin_lock_irqsave(&g_stIspSyncLock[MainPipeS], &u32Flags);

    if (pstDrvCtx->enIspRunningState != ISP_BE_BUF_STATE_RUNNING) {
        osal_spin_unlock_irqrestore(&g_stIspSyncLock[MainPipeS], &u32Flags);
        return;
    }

    pstDrvCtx->enIspRunningState = ISP_BE_BUF_STATE_FINISH;

    s32Ret = ISP_DRV_StitchSync(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        osal_spin_unlock_irqrestore(&g_stIspSyncLock[MainPipeS], &u32Flags);
        return;
    }

    for (i = 0; i < pstDrvCtx->stStitchAttr.stitch_pipe_num; i++) {
        ViPipeS = pstDrvCtx->stStitchAttr.stitch_bind_id[i];
        pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);

        if (pstDrvCtxS->enIspRunningState != ISP_BE_BUF_STATE_FINISH) {
            osal_spin_unlock_irqrestore(&g_stIspSyncLock[MainPipeS], &u32Flags);
            return;
        }
    }

    for (i = 0; i < pstDrvCtx->stStitchAttr.stitch_pipe_num; i++) {
        ViPipeS = pstDrvCtx->stStitchAttr.stitch_bind_id[i];
        pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);

        if (pstDrvCtxS->stBeBufInfo.init != HI_TRUE) {
            ISP_ERR_TRACE("Pipe[%d] BeBuf (bInit != TRUE) !\n", ViPipe);
            osal_spin_unlock_irqrestore(&g_stIspSyncLock[MainPipeS], &u32Flags);
            return;
        }

        if (pstDrvCtxS->bRunOnceFlag != HI_TRUE) {
            ISP_DRV_BeBufQueuePutBusy(ViPipeS);
        }
        pstDrvCtxS->enIspRunningState = ISP_BE_BUF_STATE_INIT;
    }

    osal_spin_unlock_irqrestore(&g_stIspSyncLock[MainPipeS], &u32Flags);

    return;
}

HI_S32 ISP_DRV_WriteAllLdciSttAddr(VI_PIPE ViPipe)
{
    HI_U8  i, k, u8WriteBufIdx, u8BlkNum, u8FreeNum, u8WriteBufNum;
    HI_U32 u32Size;
    HI_U64 u64WriteSttHeadAddr, u64WriteSttAddr;
    isp_be_wo_reg_cfg *pstBeRegCfg = HI_NULL;
    ISP_BE_BUF_NODE_S   *pstNode     = HI_NULL;
    ISP_DRV_CTX_S       *pstDrvCtx   = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    u8BlkNum      = pstDrvCtx->stWorkMode.block_num;
    u32Size       = pstDrvCtx->stLdciWriteBufAttr.ldci_buf[0].size;
    u8WriteBufNum = pstDrvCtx->stLdciWriteBufAttr.buf_num;
    u8WriteBufIdx = pstDrvCtx->stLdciWriteBufAttr.buf_idx;
    u64WriteSttHeadAddr = pstDrvCtx->stLdciWriteBufAttr.ldci_buf[u8WriteBufIdx].phy_addr;

    pstBeRegCfg = (isp_be_wo_reg_cfg *)pstDrvCtx->pstUseNode->stBeCfgBuf.vir_addr;

    for (i = 0; i < u8BlkNum; i++) {
        u64WriteSttAddr = u64WriteSttHeadAddr + i * (u32Size / DIV_0_TO_1(u8BlkNum));
        ISP_DRV_SetLdciWriteSttAddr(&pstBeRegCfg->be_reg_cfg[i].stViProcReg, u64WriteSttAddr);
    }

    pstDrvCtx->stLdciWriteBufAttr.buf_idx = (u8WriteBufIdx + 1) % DIV_0_TO_1(u8WriteBufNum);

    u8FreeNum = ISP_QueueGetFreeNum(&pstDrvCtx->stBeBufQueue);

    for (k = 0; k < u8FreeNum; k++) {
        pstNode = ISP_QueueGetFreeBeBuf(&pstDrvCtx->stBeBufQueue);
        if (pstNode == HI_NULL) {
            ISP_ERR_TRACE("ISP[%d] Get QueueGetFreeBeBuf fail!\r\n", ViPipe);
            return HI_FAILURE;;
        }

        pstBeRegCfg = (isp_be_wo_reg_cfg *)pstNode->stBeCfgBuf.vir_addr;
        u8WriteBufIdx = pstDrvCtx->stLdciWriteBufAttr.buf_idx;
        u64WriteSttHeadAddr = pstDrvCtx->stLdciWriteBufAttr.ldci_buf[u8WriteBufIdx].phy_addr;

        for (i = 0; i < u8BlkNum; i++) {
            u64WriteSttAddr = u64WriteSttHeadAddr + i * (u32Size / DIV_0_TO_1(u8BlkNum));
            ISP_DRV_SetLdciWriteSttAddr(&pstBeRegCfg->be_reg_cfg[i].stViProcReg, u64WriteSttAddr);
        }

        pstDrvCtx->stLdciWriteBufAttr.buf_idx = (u8WriteBufIdx + 1) % DIV_0_TO_1(u8WriteBufNum);
        ISP_QueuePutFreeBeBuf(&pstDrvCtx->stBeBufQueue, pstNode);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_WriteBeFreeBuf(VI_PIPE ViPipe)
{
    HI_S32 i;
    HI_S32 s32FreeNum, s32Ret;
    isp_running_mode enIspRunningMode;
    ISP_BE_BUF_NODE_S *pstNode = HI_NULL;
    isp_be_wo_reg_cfg *pstBeRegCfgSrc = HI_NULL;
    isp_be_wo_reg_cfg *pstBeRegCfgDst = HI_NULL;
    ISP_DRV_CTX_S *pstDrvCtx  = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_BEBUF_INIT(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstDrvCtx->pstUseNode == HI_NULL) {
        ISP_ERR_TRACE("Pipe[%d] pstCurNode is null for init!\r\n", ViPipe);
        return HI_FAILURE;
    }

    pstBeRegCfgSrc   = pstDrvCtx->pstUseNode->stBeCfgBuf.vir_addr;
    enIspRunningMode = pstDrvCtx->stWorkMode.running_mode;

    s32FreeNum = ISP_QueueGetFreeNum(&pstDrvCtx->stBeBufQueue);

    for (i = 0; i < s32FreeNum; i++) {
        pstNode = ISP_QueueGetFreeBeBuf(&pstDrvCtx->stBeBufQueue);

        if (pstNode == HI_NULL) {
            ISP_ERR_TRACE("Pipe[%d] Get QueueGetFreeBeBuf fail!\r\n", ViPipe);
            return HI_FAILURE;;
        }

        pstBeRegCfgDst = (isp_be_wo_reg_cfg *)pstNode->stBeCfgBuf.vir_addr;

        if ((enIspRunningMode == ISP_MODE_RUNNING_SIDEBYSIDE) || (enIspRunningMode == ISP_MODE_RUNNING_STRIPING)) {
            osal_memcpy(pstBeRegCfgDst, pstBeRegCfgSrc, sizeof(isp_be_wo_reg_cfg));
        } else {
            osal_memcpy(&pstBeRegCfgDst->be_reg_cfg[0], &pstBeRegCfgSrc->be_reg_cfg[0], sizeof(S_ISPBE_REGS_TYPE));
        }

        ISP_QueuePutFreeBeBuf(&pstDrvCtx->stBeBufQueue, pstNode);
    }

    s32Ret = ISP_DRV_WriteAllLdciSttAddr(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_BeBufSwitchFinishState(VI_PIPE ViPipe)
{
    HI_S32  i;
    VI_PIPE ViPipeS;
    ISP_DRV_CTX_S *pstDrvCtxS = HI_NULL;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;
    osal_spinlock_t *pIspSpinLock;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    pIspSpinLock = ISP_DRV_GetSpinLock(ViPipe);
    osal_spin_lock_irqsave(pIspSpinLock, &u32Flags);

    if (pstDrvCtx->stStitchAttr.stitch_enable == HI_TRUE) {
        if (pstDrvCtx->enIspRunningState != ISP_BE_BUF_STATE_SWITCH_START) {
            osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);
            ISP_WARN_TRACE("Pipe[%d] isp isn't init state!\n", ViPipe);
            return HI_FAILURE;
        }

        pstDrvCtx->enIspRunningState = ISP_BE_BUF_STATE_SWITCH;

        for (i = 0; i < pstDrvCtx->stStitchAttr.stitch_pipe_num; i++) {
            ViPipeS = pstDrvCtx->stStitchAttr.stitch_bind_id[i];
            pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);

            if (pstDrvCtxS->enIspRunningState != ISP_BE_BUF_STATE_SWITCH) {
                osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);
                ISP_WARN_TRACE("Pipe[%d] isp isn't  finish state!\n", ViPipe);
                return HI_FAILURE;
            }
        }

        for (i = 0; i < pstDrvCtx->stStitchAttr.stitch_pipe_num; i++) {
            ViPipeS = pstDrvCtx->stStitchAttr.stitch_bind_id[i];
            pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);
            pstDrvCtxS->enIspRunningState = ISP_BE_BUF_STATE_INIT;
        }

    }

    osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);

    return HI_SUCCESS;
}


HI_S32 ISP_DRV_StitchWriteBeBufAll(VI_PIPE ViPipe)
{
    HI_S32 i;
    HI_S32 s32Ret;
    VI_PIPE ViPipeS;
    VI_PIPE MainPipeS;
    ISP_DRV_CTX_S *pstDrvCtx  = HI_NULL;
    ISP_DRV_CTX_S *pstDrvCtxS = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    MainPipeS = pstDrvCtx->stStitchAttr.stitch_bind_id[0];

    s32Ret = ISP_DRV_WriteBeFreeBuf(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("Pipe[%d] ISP_DRV_WriteBeFreeBuf fail!\n", ViPipe);
        return HI_FAILURE;
    }

    osal_spin_lock_irqsave(&g_stIspSyncLock[MainPipeS], &u32Flags);

    s32Ret = ISP_DRV_StitchSyncEx(ViPipe);
    if (s32Ret != HI_SUCCESS) {
        osal_spin_unlock_irqrestore(&g_stIspSyncLock[MainPipeS], &u32Flags);

        return HI_SUCCESS;
    }

    if (pstDrvCtx->enIspRunningState == ISP_BE_BUF_STATE_SWITCH_START) {
        pstDrvCtx->enIspRunningState =  ISP_BE_BUF_STATE_SWITCH;
        for (i = 0; i < pstDrvCtx->stStitchAttr.stitch_pipe_num; i++) {
            ViPipeS = pstDrvCtx->stStitchAttr.stitch_bind_id[i];
            pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);

            if (pstDrvCtxS->enIspRunningState != ISP_BE_BUF_STATE_SWITCH) {
                osal_spin_unlock_irqrestore(&g_stIspSyncLock[MainPipeS], &u32Flags);
                ISP_WARN_TRACE("Pipe[%d] isp isn't  finish state!\n", ViPipe);
                pstDrvCtx->enIspRunningState = ISP_BE_BUF_STATE_SWITCH_START;
                return HI_SUCCESS;
            }
        }
        pstDrvCtx->enIspRunningState = ISP_BE_BUF_STATE_SWITCH_START;
    }

    for (i = 0; i < pstDrvCtx->stStitchAttr.stitch_pipe_num; i++) {
        ViPipeS = pstDrvCtx->stStitchAttr.stitch_bind_id[i];
        pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);

        if (pstDrvCtxS->stBeBufInfo.init != HI_TRUE) {
            osal_spin_unlock_irqrestore(&g_stIspSyncLock[MainPipeS], &u32Flags);
            ISP_ERR_TRACE("Pipe[%d] BeBuf (bInit != TRUE) !\n", ViPipe);
            return HI_FAILURE;
        }

        if (pstDrvCtxS->pstUseNode == HI_NULL) {
            osal_spin_unlock_irqrestore(&g_stIspSyncLock[MainPipeS], &u32Flags);
            ISP_ERR_TRACE("ISP stitch exit without success! pstUseNode is %p!\n", pstDrvCtxS->pstUseNode);
            return HI_FAILURE;
        }

        ISP_QueuePutBusyBeBuf(&pstDrvCtxS->stBeBufQueue, pstDrvCtxS->pstUseNode);
        pstDrvCtxS->pstUseNode = HI_NULL;
        pstDrvCtxS->enIspRunningState = ISP_BE_BUF_STATE_INIT;
    }

    osal_spin_unlock_irqrestore(&g_stIspSyncLock[MainPipeS], &u32Flags);

    return HI_SUCCESS;

}

HI_S32 ISP_DRV_GetBeFreeBuf(VI_PIPE ViPipe, isp_be_wo_cfg_buf *pstBeWoCfgBuf)
{
    osal_spinlock_t *pIspSpinLock = HI_NULL;
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    isp_be_wo_cfg_buf *pstCurNodeBuf = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_BEBUF_INIT(ViPipe);
    ISP_CHECK_POINTER(pstBeWoCfgBuf);
    ISP_CHECK_ONLINE_MODE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    pIspSpinLock = ISP_DRV_GetSpinLock(ViPipe);

    osal_spin_lock_irqsave(pIspSpinLock, &u32Flags);

    if (pstDrvCtx->pstUseNode == HI_NULL) {
        osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);
        return HI_FAILURE;
    }

    pstCurNodeBuf = &pstDrvCtx->pstUseNode->stBeCfgBuf;
    osal_memcpy(pstBeWoCfgBuf, pstCurNodeBuf, sizeof(isp_be_wo_cfg_buf));

    osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_GetBeBufFirst(VI_PIPE ViPipe, HI_U64 *pu64PhyAddr)
{
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_BE_BUF_NODE_S *pstNode = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_BEBUF_INIT(ViPipe);
    ISP_CHECK_POINTER(pu64PhyAddr);
    ISP_CHECK_ONLINE_MODE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    pstNode = ISP_QueueGetFreeBeBuf(&pstDrvCtx->stBeBufQueue);

    if (pstNode == HI_NULL) {
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

        ISP_ERR_TRACE("Pipe[%d] Get FreeBeBuf to user fail!\r\n", ViPipe);
        return HI_FAILURE;
    }

    pstDrvCtx->pstUseNode = pstNode;

    *pu64PhyAddr = pstDrvCtx->pstUseNode->stBeCfgBuf.phy_addr;

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_GetBeLastBuf(VI_PIPE ViPipe, HI_U64 *pu64PhyAddr)
{
    HI_U8  i;
    ISP_DRV_CTX_S     *pstDrvCtx = HI_NULL;
    ISP_BE_BUF_NODE_S *pstNode   = HI_NULL;
    isp_be_wo_reg_cfg   *pstBeRegCfgDst = HI_NULL;
    struct osal_list_head *pListTmp = HI_NULL;
    struct osal_list_head *pListNode = HI_NULL;
    unsigned long u32Flags;
    osal_spinlock_t *pIspSpinLock = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_BEBUF_INIT(ViPipe);
    ISP_CHECK_POINTER(pu64PhyAddr);

    ISP_CHECK_ONLINE_MODE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    pIspSpinLock = ISP_DRV_GetSpinLock(ViPipe);

    osal_spin_lock_irqsave(pIspSpinLock, &u32Flags);

    osal_list_for_each_safe(pListNode, pListTmp, &pstDrvCtx->stBeBufQueue.stBusyList) {
        pstNode = osal_list_entry(pListNode, ISP_BE_BUF_NODE_S, list);

        pstNode->s32HoldCnt = 0;

        ISP_QueueDelBusyBeBuf(&pstDrvCtx->stBeBufQueue, pstNode);
        ISP_QueuePutFreeBeBuf(&pstDrvCtx->stBeBufQueue, pstNode);
    }

    if (!pstDrvCtx->pstUseNode) {
        pstNode = ISP_QueueGetFreeBeBufTail(&pstDrvCtx->stBeBufQueue);

        if (pstNode == HI_NULL) {
            osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);
            ISP_ERR_TRACE("Pipe[%d] Get LastBeBuf fail!\r\n", ViPipe);
            return HI_FAILURE;
        }

        pstDrvCtx->pstUseNode = pstNode;
    }

    pstBeRegCfgDst = (isp_be_wo_reg_cfg *)pstDrvCtx->pstUseNode->stBeCfgBuf.vir_addr;

    for (i = pstDrvCtx->stWorkMode.pre_block_num; i < pstDrvCtx->stWorkMode.block_num; i++) {
        osal_memcpy(&pstBeRegCfgDst->be_reg_cfg[i], &pstBeRegCfgDst->be_reg_cfg[0], sizeof(S_ISPBE_REGS_TYPE));
    }

    *pu64PhyAddr = pstDrvCtx->pstUseNode->stBeCfgBuf.phy_addr;

    osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_BeBufRunState(VI_PIPE ViPipe)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;
    osal_spinlock_t *pIspSpinLock = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_BEBUF_INIT(ViPipe);
    ISP_CHECK_ONLINE_MODE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    pIspSpinLock = ISP_DRV_GetSpinLock(ViPipe);

    osal_spin_lock_irqsave(pIspSpinLock, &u32Flags);

    if (pstDrvCtx->stStitchAttr.stitch_enable == HI_TRUE) {
        if (pstDrvCtx->enIspRunningState != ISP_BE_BUF_STATE_INIT) {
            osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);

            ISP_WARN_TRACE("Pipe[%d] isp isn't init state!\n", ViPipe);
            return HI_FAILURE;
        }

        pstDrvCtx->enIspRunningState = ISP_BE_BUF_STATE_RUNNING;
    }

    osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_BeBufCtl(VI_PIPE ViPipe)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_ONLINE_MODE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstDrvCtx->stStitchAttr.stitch_enable != HI_TRUE) {
        if (pstDrvCtx->stBeBufInfo.init != HI_TRUE) {
            ISP_ERR_TRACE("Pipe[%d] BeBuf (bInit != TRUE) !\n", ViPipe);
            return HI_FAILURE;
        }

        osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
        if (pstDrvCtx->bRunOnceFlag != HI_TRUE) {
            ISP_DRV_BeBufQueuePutBusy(ViPipe);
        }
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
    } else {
        ISP_DRV_StitchBeBufCtl(ViPipe);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_AllBeBufInit(VI_PIPE ViPipe)
{
    HI_S32  s32Ret = HI_SUCCESS;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_ONLINE_MODE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstDrvCtx->stStitchAttr.stitch_enable != HI_TRUE) {
        if (pstDrvCtx->stBeBufInfo.init != HI_TRUE) {
            ISP_ERR_TRACE("Pipe[%d] BeBuf (bInit != TRUE) !\n", ViPipe);
            return HI_FAILURE;
        }

        osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

        s32Ret = ISP_DRV_WriteBeFreeBuf(ViPipe);
        if (s32Ret != HI_SUCCESS) {
            osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

            ISP_ERR_TRACE("Pipe[%d] ISP_DRV_WriteBeFreeBuf fail!\n", ViPipe);
            return s32Ret;
        }

        ISP_QueuePutBusyBeBuf(&pstDrvCtx->stBeBufQueue, pstDrvCtx->pstUseNode);
        pstDrvCtx->pstUseNode = HI_NULL;

        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
    } else {
        s32Ret = ISP_DRV_StitchWriteBeBufAll(ViPipe);
        if (s32Ret != HI_SUCCESS) {
            ISP_ERR_TRACE("Pipe[%d] ISP_DRV_StitchWriteBeBufAll fail!\n", ViPipe);
            return s32Ret;
        }
    }

    return s32Ret;
}

HI_S32 ISP_DRV_SyncCfgSet(VI_PIPE ViPipe, isp_sync_cfg_buf_node *pstIspSyncCfgBuf)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_SYNC_CFG_BUF_S  *pstSyncCfgBuf = HI_NULL;
    isp_sync_cfg_buf_node  *pstCurNode = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstIspSyncCfgBuf);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstSyncCfgBuf = &pstDrvCtx->stSyncCfg.stSyncCfgBuf;

    if (ISPSyncBufIsFull(pstSyncCfgBuf)) {
        ISP_ERR_TRACE("Pipe[%d] isp sync buffer is full\n", ViPipe);
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return HI_FAILURE;
    }

    if ((pstIspSyncCfgBuf->stSnsRegsInfo.enSnsType >= ISP_SNS_TYPE_BUTT) || \
        (pstIspSyncCfgBuf->stAERegCfg.enFSWDRMode >= ISP_FSWDR_MODE_BUTT) || \
        (pstIspSyncCfgBuf->stSnsRegsInfo.u8Cfg2ValidDelayMax > CFG2VLD_DLY_LIMIT) || \
        (pstIspSyncCfgBuf->stSnsRegsInfo.u8Cfg2ValidDelayMax < 1) ||
        (pstIspSyncCfgBuf->stSnsRegsInfo.stSlvSync.u8DelayFrmNum > CFG2VLD_DLY_LIMIT) || \
        (pstIspSyncCfgBuf->stSnsRegsInfo.u32RegNum > ISP_MAX_SNS_REGS)) {
        ISP_ERR_TRACE("Invalid SnsRegsInfo!\n");
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return HI_FAILURE;
    }

    pstCurNode = &pstSyncCfgBuf->stSyncCfgBufNode[pstSyncCfgBuf->u8BufWRFlag];
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    osal_memcpy(pstCurNode, pstIspSyncCfgBuf, sizeof(isp_sync_cfg_buf_node));

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstSyncCfgBuf->u8BufWRFlag = (pstSyncCfgBuf->u8BufWRFlag + 1) % ISP_SYNC_BUF_NODE_NUM;
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_SetWdrCfg(VI_PIPE ViPipe, isp_wdr_cfg *pstIspWdrCfg)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstIspWdrCfg);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstIspWdrCfg->wdr_mode >= WDR_MODE_BUTT) {
        ISP_ERR_TRACE("Invalid WDR mode %d!\n", pstIspWdrCfg->wdr_mode);
        return HI_FAILURE;
    }

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    osal_memcpy(&pstDrvCtx->stWDRCfg, pstIspWdrCfg, sizeof(isp_wdr_cfg));
    ISP_DRV_SwitchMode(ViPipe, pstDrvCtx);
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_GetStitchAttr(VI_PIPE ViPipe, vi_stitch_attr *pstStitchAttr)
{
    HI_U8   i;
    HI_S32  s32Ret;
    ISP_DRV_CTX_S    *pstDrvCtx = HI_NULL;
    vi_stitch_attr   stStitchAttr = { 0 };

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstStitchAttr);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (!ckfn_vi_get_pipe_stitch_attr()) {
        ISP_ERR_TRACE("pipe[%d] ckfn_vi_get_pipe_stitch_attr is null\n", ViPipe);
        return HI_FAILURE;
    }

    s32Ret = call_vi_get_pipe_stitch_attr(ViPipe, &stStitchAttr);

    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("pipe[%d] call_vi_get_pipe_stitch_attr failed 0x%x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    if (stStitchAttr.stitch_enable) {
        if ((stStitchAttr.stitch_pipe_num < 1) || (stStitchAttr.stitch_pipe_num > ISP_MAX_PIPE_NUM)) {
            ISP_ERR_TRACE("pipe[%d] err stitch num %d\n", ViPipe, stStitchAttr.stitch_pipe_num);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        for (i = 0; i < stStitchAttr.stitch_pipe_num; i++) {
            ISP_CHECK_PIPE(stStitchAttr.stitch_bind_id[i]);
        }
    }

    /* Don't support stitch different wdr mode(check)! */

    osal_memcpy(&pstDrvCtx->stStitchAttr, &stStitchAttr, sizeof(vi_stitch_attr));
    osal_memcpy(pstStitchAttr, &stStitchAttr, sizeof(vi_stitch_attr));

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_GetPipeSize(VI_PIPE ViPipe, hi_size *pstPipeSize)
{
    HI_S32  s32Ret;
    hi_size stPipeSize = { 0 };

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstPipeSize);
    ISP_CHECK_VIR_PIPE(ViPipe);
    if (!ckfn_vi_get_pipe_bind_dev_size()) {
        ISP_ERR_TRACE("pipe[%d] ckfn_vi_get_pipe_bind_dev_size is null\n", ViPipe);
        return HI_FAILURE;
    }

    s32Ret = call_vi_get_pipe_bind_dev_size(ViPipe, &stPipeSize);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("pipe[%d] call_vi_get_pipe_bind_dev_size failed 0x%x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    if ((stPipeSize.width < RES_WIDTH_MIN) ||
        (stPipeSize.width > SENSOR_RES_WIDTH_MAX) ||
        (stPipeSize.height < RES_HEIGHT_MIN) ||
        (stPipeSize.height > SENSOR_RES_HEIGHT_MAX)) {
        ISP_ERR_TRACE("pipe[%d]: Image Width should between [%d, %d], Height should between[%d, %d]\n",
                  ViPipe, RES_WIDTH_MIN, SENSOR_RES_WIDTH_MAX, RES_HEIGHT_MIN, SENSOR_RES_HEIGHT_MAX);
        return HI_FAILURE;
    }

    osal_memcpy(pstPipeSize, &stPipeSize, sizeof(hi_size));

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_GetHdrAttr(VI_PIPE ViPipe, vi_pipe_hdr_attr *pstHDRAttr)
{
    HI_S32  s32Ret;
    vi_pipe_hdr_attr stHDRAttr = { 0 };

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstHDRAttr);

    if (!ckfn_vi_get_pipe_hdr_attr()) {
        ISP_ERR_TRACE("pipe[%d] ckfn_vi_get_pipe_hdr_attr is null\n", ViPipe);
        return HI_FAILURE;
    }

    s32Ret = call_vi_get_pipe_hdr_attr(ViPipe, &stHDRAttr);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("pipe[%d] call_vi_get_pipe_hdr_attr failed 0x%x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    osal_memcpy(pstHDRAttr, &stHDRAttr, sizeof(vi_pipe_hdr_attr));

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_GetWdrAttr(VI_PIPE ViPipe, vi_pipe_wdr_attr *pstWDRAttr)
{
    HI_U8   i;
    HI_U32  u32Num;
    HI_S32  s32Ret;
    vi_pipe_wdr_attr stWDRAttr;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstWDRAttr);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (!ckfn_vi_get_pipe_wdr_attr()) {
        ISP_ERR_TRACE("pipe[%d] ckfn_vi_get_pipe_wdr_attr is null\n", ViPipe);
        return HI_FAILURE;
    }

    s32Ret = call_vi_get_pipe_wdr_attr(ViPipe, &stWDRAttr);
    if (s32Ret != HI_SUCCESS) {
        ISP_ERR_TRACE("pipe[%d] call_vi_get_pipe_wdr_attr failed 0x%x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    /* Not WDR mode,BindPipe attr update */
    if ((!IS_FS_WDR_MODE(stWDRAttr.wdr_mode)) &&
        (stWDRAttr.dev_bind_pipe.num != 1)) {
        stWDRAttr.dev_bind_pipe.num    = 1;
        stWDRAttr.dev_bind_pipe.pipe_id[0] = ViPipe;
    }

    u32Num = stWDRAttr.dev_bind_pipe.num;
    if ((u32Num < 1) || (u32Num > ISP_WDR_CHN_MAX)) {
        ISP_ERR_TRACE("Err wdr bind num %d!\n", u32Num);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    for (i = 0; i < u32Num; i++) {
        ISP_CHECK_PIPE(stWDRAttr.dev_bind_pipe.pipe_id[i]);
    }
    if ((stWDRAttr.wdr_mode > WDR_MODE_BUTT)) {
        ISP_ERR_TRACE("Err wdr mode %d!\n", stWDRAttr.wdr_mode);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    osal_memcpy(&pstDrvCtx->stWDRAttr, &stWDRAttr, sizeof(vi_pipe_wdr_attr));
    osal_memcpy(pstWDRAttr, &stWDRAttr, sizeof(vi_pipe_wdr_attr));

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_ChnSelectCfg(VI_PIPE ViPipe, HI_U32 u32ChnSel)
{
    HI_U32 i;
    HI_S32 s32Ret = HI_SUCCESS;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    for (i = 0; i < ISP_STRIPING_MAX_NUM; i++) {
        pstDrvCtx->astChnSelAttr[i].channel_sel = u32ChnSel;
    }

    if (IS_ONLINE_MODE(pstDrvCtx->stWorkMode.running_mode) ||
        IS_SIDEBYSIDE_MODE(pstDrvCtx->stWorkMode.running_mode)) {
        s32Ret = ISP_DRV_ChnSelectWrite(ViPipe, u32ChnSel);

        if (s32Ret != HI_SUCCESS) {
            osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
            ISP_ERR_TRACE("isp[%d] ChnSelect Write err!\n", ViPipe);
            return s32Ret;
        }
    }

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return s32Ret;
}

HI_S32 ISP_DRV_SetIspRunState(VI_PIPE ViPipe, HI_U64 *pu64HandSignal)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pu64HandSignal);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (*pu64HandSignal == ISP_INIT_HAND_SIGNAL) {
        pstDrvCtx->bIspRunFlag = HI_TRUE;
    } else if (*pu64HandSignal == ISP_EXIT_HAND_SIGNAL) {
        pstDrvCtx->bIspRunFlag = HI_FALSE;
        ISP_DRV_ResetFeCfg(ViPipe);
    } else {
        ISP_ERR_TRACE("ISP[%d] set isp run state failed!\n", ViPipe);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_ResetCtx(VI_PIPE ViPipe)
{
    HI_U8 i;
    VI_PIPE ViPipeS = 0;
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx  = HI_NULL;
    ISP_DRV_CTX_S *pstDrvCtxS = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    pstDrvCtx->bMemInit     = HI_FALSE;
    pstDrvCtx->bIspInit     = HI_FALSE;
    pstDrvCtx->pub_attr_ok  = HI_FALSE;
    pstDrvCtx->bRunOnceOk   = HI_FALSE;
    pstDrvCtx->bRunOnceFlag = HI_FALSE;
    pstDrvCtx->bYUVRunOnceOk   = HI_FALSE;
    pstDrvCtx->bYUVRunOnceFlag = HI_FALSE;

    if (pstDrvCtx->stStitchAttr.stitch_enable == HI_TRUE) {
        if (pstDrvCtx->stStitchAttr.main_pipe == HI_TRUE) {
            for (i = 0; i < pstDrvCtx->stStitchAttr.stitch_pipe_num; i++) {
                ViPipeS = pstDrvCtx->stStitchAttr.stitch_bind_id[i];
                ISP_CHECK_PIPE(ViPipeS);
                pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);
                pstDrvCtxS->bStitchSync  = HI_FALSE;
            }
        }
    }

    pstDrvCtx->stStitchAttr.stitch_enable = HI_FALSE;

    for (i = 0; i < ISP_STRIPING_MAX_NUM; i++) {
        pstDrvCtx->astChnSelAttr[i].channel_sel = 0;
    }

    pstDrvCtx->stSnapAttr.snap_type = SNAP_TYPE_NORMAL;
    pstDrvCtx->stSnapAttr.picture_pipe_id    = -1;
    pstDrvCtx->stSnapAttr.preview_pipe_id    = -1;
    pstDrvCtx->stSnapAttr.load_ccm           = HI_TRUE;
    pstDrvCtx->stSnapAttr.pro_param.operation_mode = OPERATION_MODE_AUTO;

    pstDrvCtx->bEdge    = HI_FALSE;
    pstDrvCtx->bVdStart = HI_FALSE;
    pstDrvCtx->bVdEnd   = HI_FALSE;
    pstDrvCtx->bVdBeEnd = HI_FALSE;

    g_IntBothalf = HI_FALSE;
    g_UseBothalf = HI_FALSE;
    g_QuickStart = HI_FALSE;


    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return HI_SUCCESS;
}

static hi_s32 isp_set_vi_bayer_format(VI_PIPE vi_pipe, hi_isp_bayer_format bayer_format)
{
    hi_s32  ret;

    if (ckfn_vi_update_bayer_format()) {
        ret = call_vi_update_bayer_format(vi_pipe, bayer_format);

        if (ret != HI_SUCCESS) {
            ISP_ERR_TRACE("ISP[%d] call_vi_update_bayer_format failed!\n", vi_pipe);
            return ret;
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SetPubAttrInfo(VI_PIPE ViPipe, hi_isp_pub_attr *pstPubAttr)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;
    hi_s32 ret;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstPubAttr);

    if ((pstPubAttr->wnd_rect.width < RES_WIDTH_MIN) ||
        (pstPubAttr->wnd_rect.width > g_au16ResMaxWidth[ViPipe]) ||
        (pstPubAttr->wnd_rect.width % 4 != 0)) {
        ISP_ERR_TRACE("Invalid Image Width:%d!\n", pstPubAttr->wnd_rect.width);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstPubAttr->wnd_rect.height < RES_HEIGHT_MIN) ||
        (pstPubAttr->wnd_rect.height > g_au16ResMaxHeight[ViPipe]) ||
        (pstPubAttr->wnd_rect.height % 4 != 0)) {
        ISP_ERR_TRACE("Invalid Image Height:%d!\n", pstPubAttr->wnd_rect.height);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstPubAttr->sns_size.width < RES_WIDTH_MIN) || (pstPubAttr->sns_size.width > SENSOR_RES_WIDTH_MAX)) {
        ISP_ERR_TRACE("Invalid Sensor Image Width:%d!\n", pstPubAttr->sns_size.width);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstPubAttr->sns_size.height < RES_HEIGHT_MIN) ||
        (pstPubAttr->sns_size.height > SENSOR_RES_HEIGHT_MAX)) {
        ISP_ERR_TRACE("Invalid Sensor Image Height:%d!\n", pstPubAttr->sns_size.height);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstPubAttr->wnd_rect.x < 0) ||
        (pstPubAttr->wnd_rect.x > g_au16ResMaxWidth[ViPipe] - RES_WIDTH_MIN)) {
        ISP_ERR_TRACE("Invalid Image X:%d!\n", pstPubAttr->wnd_rect.x);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstPubAttr->wnd_rect.y < 0) ||
        (pstPubAttr->wnd_rect.y > g_au16ResMaxHeight[ViPipe] - RES_HEIGHT_MIN)) {
        ISP_ERR_TRACE("Invalid Image Y:%d!\n", pstPubAttr->wnd_rect.y);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstPubAttr->bayer >= BAYER_BUTT) {
        ISP_ERR_TRACE("Invalid Bayer Pattern:%d!\n", pstPubAttr->bayer);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstPubAttr->wdr_mode >= WDR_MODE_BUTT) {
        ISP_ERR_TRACE("Invalid WDR mode %d!\n", pstPubAttr->wdr_mode);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    osal_memcpy(&pstDrvCtx->stProcPubInfo, pstPubAttr, sizeof(hi_isp_pub_attr));
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    ret = isp_set_vi_bayer_format(ViPipe, pstPubAttr->bayer);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    pstDrvCtx->pub_attr_ok = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 ISP_SetAwbStaEnable(VI_PIPE ViPipe, HI_BOOL bEnable)
{
    ISP_DRV_CTX_S* pstDrvCtx = HI_NULL;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if ((IS_ONLINE_MODE(pstDrvCtx->stWorkMode.running_mode)) || \
        (IS_SIDEBYSIDE_MODE(pstDrvCtx->stWorkMode.running_mode))) {
        ISP_DRV_RegConfigAwbEn(ViPipe, bEnable);
    }

    return HI_SUCCESS;
}
static long ISP_ioctl(unsigned int cmd, unsigned long arg, void *private_data)
{
    VI_PIPE ViPipe = ISP_GET_DEV(private_data);

    switch (cmd) {
        case ISP_DEV_SET_FD: {
            ISP_CHECK_POINTER(arg);
            *((HI_U32 *)(private_data)) = *(HI_U32 *)(HI_UINTPTR_T)arg;

            return HI_SUCCESS;
        }

        case ISP_GET_VERSION: {
            ISP_VERSION_S *pVersion = HI_NULL;
            pVersion = (ISP_VERSION_S *)(HI_UINTPTR_T)arg;

            return ISP_DRV_GetVersion(pVersion);
        }

        case ISP_GET_FRAME_EDGE: {
            HI_U32 *pu32Status = HI_NULL;
            pu32Status = (HI_U32 *)(HI_UINTPTR_T)arg;

            return ISP_GetFrameEdge(ViPipe, pu32Status);
        }

        case ISP_GET_VD_TIMEOUT: {
            isp_vd_timeout stIspVdTimeOut;
            isp_vd_timeout *pstVdTimeOut = HI_NULL;

            ISP_CHECK_POINTER(arg);

            pstVdTimeOut = (isp_vd_timeout *)(HI_UINTPTR_T)arg;
            osal_memcpy(&stIspVdTimeOut, pstVdTimeOut, sizeof(isp_vd_timeout));
            ISP_GetVdStartTimeOut(ViPipe, stIspVdTimeOut.milli_sec, &stIspVdTimeOut.int_status);
            osal_memcpy(pstVdTimeOut, &stIspVdTimeOut, sizeof(isp_vd_timeout));

            return HI_SUCCESS;
        }

        case ISP_GET_VD_END_TIMEOUT: {
            isp_vd_timeout stIspVdTimeOut;
            isp_vd_timeout *pstVdTimeOut = HI_NULL;

            ISP_CHECK_POINTER(arg);

            pstVdTimeOut = (isp_vd_timeout *)(HI_UINTPTR_T)arg;
            osal_memcpy(&stIspVdTimeOut, pstVdTimeOut, sizeof(isp_vd_timeout));
            ISP_GetVdEndTimeOut(ViPipe, stIspVdTimeOut.milli_sec, &stIspVdTimeOut.int_status);
            osal_memcpy(pstVdTimeOut, &stIspVdTimeOut, sizeof(isp_vd_timeout));

            return HI_SUCCESS;
        }

        case ISP_GET_VD_BEEND_TIMEOUT: {
            isp_vd_timeout stIspVdTimeOut;
            isp_vd_timeout *pstVdTimeOut = HI_NULL;

            ISP_CHECK_POINTER(arg);

            pstVdTimeOut = (isp_vd_timeout *)(HI_UINTPTR_T)arg;
            osal_memcpy(&stIspVdTimeOut, pstVdTimeOut, sizeof(isp_vd_timeout));
            ISP_GetVdBeEndTimeOut(ViPipe, stIspVdTimeOut.milli_sec, &stIspVdTimeOut.int_status);
            osal_memcpy(pstVdTimeOut, &stIspVdTimeOut, sizeof(isp_vd_timeout));

            return HI_SUCCESS;
        }

        case ISP_UPDATE_INFO_SET: {
            ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
            hi_isp_dcf_update_info *pstIspUpdateInfo = HI_NULL;
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
            pstIspUpdateInfo = (hi_isp_dcf_update_info *)(HI_UINTPTR_T)arg;
            osal_memcpy(&pstDrvCtx->stUpdateInfo, pstIspUpdateInfo, sizeof(hi_isp_dcf_update_info));
            return HI_SUCCESS;
        }

        case ISP_FRAME_INFO_SET: {
            ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
            hi_isp_frame_info *pstIspFrameInfo = HI_NULL;
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
            pstIspFrameInfo = (hi_isp_frame_info *)(HI_UINTPTR_T)arg;
            osal_memcpy(&pstDrvCtx->stFrameInfo, pstIspFrameInfo, sizeof(hi_isp_frame_info));
            return HI_SUCCESS;
        }

        case ISP_FRAME_INFO_GET: {
            hi_isp_frame_info *pstIspFrameInfo = HI_NULL;
            pstIspFrameInfo = (hi_isp_frame_info *)(HI_UINTPTR_T)arg;

            return ISP_GetFrameInfo(ViPipe, pstIspFrameInfo);
        }

        case ISP_DNG_INFO_SET: {
            hi_dng_image_dynamic_info *pstDngImgDynInfo = HI_NULL;
            pstDngImgDynInfo = (hi_dng_image_dynamic_info *)(HI_UINTPTR_T)arg;

            return ISP_DRV_SetDngInfo(ViPipe, pstDngImgDynInfo);
        }

        case ISP_DNG_INFO_GET: {
            hi_isp_dng_image_static_info *pstDngStaticInfo = HI_NULL;
            pstDngStaticInfo = (hi_isp_dng_image_static_info *)(HI_UINTPTR_T)arg;

            return ISP_DRV_GetDngInfo(ViPipe, pstDngStaticInfo);
        }

        case ISP_SET_INT_ENABLE: {
            HI_BOOL bEn = HI_FALSE;
            ISP_CHECK_POINTER(arg);
            bEn = *(HI_BOOL *)(HI_UINTPTR_T)arg;

            return ISP_DRV_SetIntEnable(ViPipe, bEn);
        }

        case ISP_TRANS_BUF_INIT: {
            isp_trans_info_buf *pstTransInfo = HI_NULL;
            pstTransInfo = (isp_trans_info_buf *)(HI_UINTPTR_T)arg;

            return ISP_DRV_TransInfoBufInit(ViPipe, pstTransInfo);
        }

        case ISP_TRANS_BUF_EXIT: {
            return ISP_DRV_TransInfoBufExit(ViPipe);
        }

        case ISP_PRO_BUF_INIT: {
            isp_pro_info_buf *pstProInfo = HI_NULL;
            pstProInfo = (isp_pro_info_buf *)(HI_UINTPTR_T)arg;

            return ISP_DRV_ProInfoBufInit(ViPipe, pstProInfo);
        }

        case ISP_PRO_BUF_EXIT: {
            return ISP_DRV_ProInfoBufExit(ViPipe);
        }

        case ISP_STAT_BUF_INIT: {
            HI_U64 *pu64StatPhyAddr = HI_NULL;
            pu64StatPhyAddr = (HI_U64 *)(HI_UINTPTR_T)arg;

            return ISP_DRV_StatBufInit(ViPipe, pu64StatPhyAddr);
        }

        case ISP_STAT_BUF_EXIT: {
            return ISP_DRV_StatBufExit(ViPipe);
        }

        case ISP_STAT_BUF_GET: {
            isp_stat_info *pstStat = HI_NULL;
            pstStat = (isp_stat_info *)(HI_UINTPTR_T)arg;

            return ISP_DRV_GetUserStatBuf(ViPipe, pstStat);
        }

        case ISP_STAT_BUF_PUT: {
            isp_stat_info *pstStatInfo = HI_NULL;
            pstStatInfo = (isp_stat_info *)(HI_UINTPTR_T)arg;

            return ISP_DRV_StatBufUserPut(ViPipe, pstStatInfo);
        }

        case ISP_STAT_ACT_GET: {
            isp_stat_info *pstIspStatInfo = HI_NULL;
            pstIspStatInfo = (isp_stat_info *)(HI_UINTPTR_T)arg;

            return ISP_DRV_GetStatInfoActive(ViPipe, pstIspStatInfo);
        }

        case ISP_REG_CFG_SET: {
            isp_kernel_reg_cfg *pstRegKernelCfg = HI_NULL;
            pstRegKernelCfg = (isp_kernel_reg_cfg *)(HI_UINTPTR_T)arg;

            return ISP_DRV_SetRegKernelCfgs(ViPipe, pstRegKernelCfg);
        }

        case ISP_BE_CFG_BUF_INIT: {
            HI_U64 *pu64BePhyAddr = HI_NULL;
            pu64BePhyAddr = (HI_U64 *)(HI_UINTPTR_T)arg;

            return ISP_DRV_BeBufInit(ViPipe, pu64BePhyAddr);
        }

        case ISP_GET_BE_BUF_FIRST: {
            HI_U64 *pu64PhyAddr = HI_NULL;
            pu64PhyAddr = (HI_U64 *)(HI_UINTPTR_T)arg;

            return ISP_DRV_GetBeBufFirst(ViPipe, pu64PhyAddr);
        }

        case ISP_BE_FREE_BUF_GET: {
            isp_be_wo_cfg_buf *pstBeWoCfgBuf = HI_NULL;
            pstBeWoCfgBuf = (isp_be_wo_cfg_buf *)(HI_UINTPTR_T)arg;

            return ISP_DRV_GetBeFreeBuf(ViPipe, pstBeWoCfgBuf);
        }

        case ISP_BE_LAST_BUF_GET: {
            HI_U64 *pu64PhyAddr = HI_NULL;
            pu64PhyAddr = (HI_U64 *)(HI_UINTPTR_T)arg;

            return ISP_DRV_GetBeLastBuf(ViPipe, pu64PhyAddr);
        }

        case ISP_BE_CFG_BUF_EXIT: {
            return ISP_DRV_BeBufExit(ViPipe);
        }

        case ISP_BE_CFG_BUF_RUNNING: {
            return ISP_DRV_BeBufRunState(ViPipe);
        }

        case ISP_BE_CFG_BUF_CTL: {
            return ISP_DRV_BeBufCtl(ViPipe);
        }

        case ISP_BE_All_BUF_INIT: {
            return ISP_DRV_AllBeBufInit(ViPipe);
        }

        case ISP_SYNC_CFG_SET: {
            isp_sync_cfg_buf_node *pstIspSyncCfgBuf = HI_NULL;
            pstIspSyncCfgBuf = (isp_sync_cfg_buf_node *)(HI_UINTPTR_T)arg;

            return ISP_DRV_SyncCfgSet(ViPipe, pstIspSyncCfgBuf);
        }

        case ISP_WDR_CFG_SET: {
            isp_wdr_cfg *pstIspWdrCfg = HI_NULL;
            pstIspWdrCfg = (isp_wdr_cfg *)(HI_UINTPTR_T)arg;

            return ISP_DRV_SetWdrCfg(ViPipe, pstIspWdrCfg);
        }

        case ISP_BE_SYNC_PARAM_INIT: {
            ISP_DRV_CTX_S      *pstDrvCtx      = HI_NULL;
            isp_be_sync_para   *pstBeSyncParam = HI_NULL;
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
            pstBeSyncParam = (isp_be_sync_para *)(HI_UINTPTR_T)arg;
            osal_memcpy(&pstDrvCtx->stIspBeSyncPara, pstBeSyncParam, sizeof(isp_be_sync_para));
            return HI_SUCCESS;
        }
        case ISP_MODE_SWITCH_SET: {
            ISP_CHECK_PIPE(ViPipe);
            ISP_DRV_BeBufSwitchState(ViPipe);
            return HI_SUCCESS;
        }

        case ISP_RES_SWITCH_SET: {
            ISP_CHECK_PIPE(ViPipe);
            ISP_DRV_BeBufSwitchState(ViPipe);
            return HI_SUCCESS;
        }

        case ISP_BE_SWITCH_FINISH_STATE_SET: {
            return ISP_DRV_BeBufSwitchFinishState(ViPipe);
        }

        case ISP_CHN_SELECT_CFG: {
            HI_U32 u32ChannelSel = 0;
            ISP_CHECK_POINTER(arg);
            u32ChannelSel = *(HI_U32 *)(HI_UINTPTR_T)arg;

            return ISP_DRV_ChnSelectCfg(ViPipe, u32ChannelSel);
        }

        case ISP_PROC_INIT: {
            isp_proc_mem *pstIspProcMem = HI_NULL;
            pstIspProcMem = (isp_proc_mem *)(HI_UINTPTR_T)arg;

            return ISP_DRV_ProcInit(ViPipe, pstIspProcMem);
        }

        case ISP_PROC_WRITE_ING: {
            ISP_CHECK_PIPE(ViPipe);

            if (osal_down_interruptible(&g_astIspDrvCtx[ViPipe].stProcSem)) {
                return -ERESTARTSYS;
            }
            return HI_SUCCESS;
        }

        case ISP_PROC_WRITE_OK: {
            ISP_CHECK_PIPE(ViPipe);

            osal_up(&g_astIspDrvCtx[ViPipe].stProcSem);
            return HI_SUCCESS;
        }

        case ISP_PROC_EXIT: {
            return ISP_DRV_ProcExit(ViPipe);
        }

        case ISP_PROC_PARAM_GET: {
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            *(HI_U32 *)(HI_UINTPTR_T)arg = g_ProcParam[ViPipe];

            return HI_SUCCESS;
        }

        case ISP_MEM_INFO_SET: {
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            ISP_CHECK_BOOL(*(HI_BOOL *)(HI_UINTPTR_T)arg);
            g_astIspDrvCtx[ViPipe].bMemInit = *(HI_BOOL *)(HI_UINTPTR_T)arg;

            return HI_SUCCESS;
        }

        case ISP_MEM_INFO_GET: {
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            *(HI_BOOL *)(HI_UINTPTR_T)arg = g_astIspDrvCtx[ViPipe].bMemInit;

            return HI_SUCCESS;
        }

        case ISP_P2EN_INFO_GET: {
            HI_BOOL *pbP2En = HI_NULL;

            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            pbP2En = (HI_BOOL *)(HI_UINTPTR_T)arg;

            return ISP_DRV_GetP2EnInfo(ViPipe, pbP2En);
        }

        case ISP_SYNC_INIT_SET: {
            ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            ISP_CHECK_BOOL(*(HI_BOOL *)(HI_UINTPTR_T)arg);
            pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

            pstDrvCtx->bStitchSync = HI_FALSE;
            if (pstDrvCtx->stStitchAttr.stitch_enable == HI_TRUE) {
                pstDrvCtx->bStitchSync = *(HI_BOOL *)(HI_UINTPTR_T)arg;
            }

            return HI_SUCCESS;
        }

        case ISP_INIT_INFO_SET: {
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            ISP_CHECK_BOOL(*(HI_BOOL *)(HI_UINTPTR_T)arg);
            g_astIspDrvCtx[ViPipe].bIspInit = *(HI_BOOL *)(HI_UINTPTR_T)arg;

            return HI_SUCCESS;
        }

        case ISP_RESET_CTX: {
            return ISP_DRV_ResetCtx(ViPipe);
        }

        case ISP_RUN_STATE_SET: {
            HI_U64 *pu64HandSignal = HI_NULL;
            pu64HandSignal = (HI_U64 *)(HI_UINTPTR_T)arg;

            return ISP_DRV_SetIspRunState(ViPipe, pu64HandSignal);
        }

        case ISP_CONFIG_INFO_SET: {
            hi_isp_config_info *pstIspConfigInfo = HI_NULL;
            pstIspConfigInfo = (hi_isp_config_info *)(HI_UINTPTR_T)arg;

            return ISP_DRV_SetConfigInfo(ViPipe, pstIspConfigInfo);
        }

        case ISP_SNAP_INFO_GET: {
            isp_snap_info *pstIspSnapInfo = HI_NULL;
            pstIspSnapInfo = (isp_snap_info *)(HI_UINTPTR_T)arg;

            return ISP_DRV_GetSnapInfo(ViPipe, pstIspSnapInfo);
        }

        case ISP_PRO_TRIGGER_GET: {
            ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
            *(HI_BOOL *)(HI_UINTPTR_T)arg = pstDrvCtx->bProEnable;

            if (pstDrvCtx->bProEnable == HI_TRUE) {
                pstDrvCtx->bProEnable = HI_FALSE;
                pstDrvCtx->u32ProTrigFlag = 1;
            }

            return HI_SUCCESS;
        }

        case ISP_SNAP_ATTR_GET: {
            isp_snap_attr *pstIspSnapAttr = HI_NULL;
            ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            pstIspSnapAttr = (isp_snap_attr *)(HI_UINTPTR_T)arg;
            pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
            osal_memcpy(pstIspSnapAttr, &pstDrvCtx->stSnapAttr, sizeof(isp_snap_attr));

            return HI_SUCCESS;
        }

        case ISP_UPDATE_POS_GET: {
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            *(HI_U32 *)(HI_UINTPTR_T)arg = g_UpdatePos[ViPipe];

            return HI_SUCCESS;
        }

        case ISP_FRAME_CNT_GET: {
            ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
            *(HI_U32 *)(HI_UINTPTR_T)arg = pstDrvCtx->u32FrameCnt;

            return HI_SUCCESS;
        }
        case ISP_PUB_ATTR_INFO: {
            hi_isp_pub_attr *pstIspPubAttr = HI_NULL;
            pstIspPubAttr = (hi_isp_pub_attr *)(HI_UINTPTR_T)arg;

            return ISP_SetPubAttrInfo(ViPipe, pstIspPubAttr);
        }

        case ISP_PWM_NUM_GET: {
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            *(HI_U32 *)(HI_UINTPTR_T)arg = g_PwmNumber[ViPipe];

            return HI_SUCCESS;
        }

        case ISP_SET_MOD_PARAM: {
            hi_isp_mod_param *pstModeParam = HI_NULL;
            pstModeParam = (hi_isp_mod_param *)(HI_UINTPTR_T)arg;

            return ISP_SetModParam(pstModeParam);
        }

        case ISP_GET_MOD_PARAM: {
            hi_isp_mod_param *pstModeParam = HI_NULL;
            pstModeParam = (hi_isp_mod_param *)(HI_UINTPTR_T)arg;

            return ISP_GetModParam(pstModeParam);
        }

        case ISP_SET_CTRL_PARAM: {
            hi_isp_ctrl_param *pstCtrlParam = HI_NULL;
            pstCtrlParam = (hi_isp_ctrl_param *)(HI_UINTPTR_T)arg;

            return ISP_SetCtrlParam(ViPipe, pstCtrlParam);
        }

        case ISP_GET_CTRL_PARAM: {
            hi_isp_ctrl_param *pstCtrlParam = HI_NULL;
            pstCtrlParam = (hi_isp_ctrl_param *)(HI_UINTPTR_T)arg;

            return ISP_GetCtrlParam(ViPipe, pstCtrlParam);
        }

        case ISP_WORK_MODE_INIT: {
            isp_block_attr *pstIspBlockAttr = HI_NULL;
            pstIspBlockAttr = (isp_block_attr *)(HI_UINTPTR_T)arg;

            return ISP_DRV_WorkModeInit(ViPipe, pstIspBlockAttr);
        }

        case ISP_WORK_MODE_EXIT: {
            return ISP_DRV_WorkModeExit(ViPipe);
        }

        case ISP_WORK_MODE_GET: {
            ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
            isp_working_mode *pstIspWorkMode = HI_NULL;
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            pstIspWorkMode = (isp_working_mode *)(HI_UINTPTR_T)arg;
            pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
            osal_memcpy(pstIspWorkMode, &pstDrvCtx->stWorkMode, sizeof(isp_working_mode));

            return HI_SUCCESS;
        }

        case ISP_PRE_BLK_NUM_UPDATE: {
            ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
            pstDrvCtx->stWorkMode.pre_block_num = *(HI_U8 *)(HI_UINTPTR_T)arg;

            return HI_SUCCESS;
        }

        case ISP_GET_PIPE_SIZE: {
            hi_size *pstPipeSize = HI_NULL;
            pstPipeSize = (hi_size *)(HI_UINTPTR_T)arg;

            return ISP_DRV_GetPipeSize(ViPipe, pstPipeSize);
        }

        case ISP_GET_HDR_ATTR: {
            vi_pipe_hdr_attr *pstHDRAttr = HI_NULL;
            pstHDRAttr = (vi_pipe_hdr_attr *)(HI_UINTPTR_T)arg;

            return ISP_DRV_GetHdrAttr(ViPipe, pstHDRAttr);
        }

        case ISP_GET_WDR_ATTR: {
            vi_pipe_wdr_attr *pstWDRAttr = HI_NULL;
            pstWDRAttr = (vi_pipe_wdr_attr *)(HI_UINTPTR_T)arg;

            return ISP_DRV_GetWdrAttr(ViPipe, pstWDRAttr);
        }

        case ISP_GET_STITCH_ATTR: {
            vi_stitch_attr *pstStitchAttr = HI_NULL;
            pstStitchAttr = (vi_stitch_attr *)(HI_UINTPTR_T)arg;

            return ISP_DRV_GetStitchAttr(ViPipe, pstStitchAttr);
        }

        case ISP_SYNC_STITCH_PARAM_INIT: {
            return ISP_DRV_StitchSyncCtrlInit(ViPipe);
        }

        case ISP_CLUT_BUF_INIT: {
            return ISP_DRV_ClutBufInit(ViPipe);
        }

        case ISP_CLUT_BUF_EXIT: {
            return ISP_DRV_ClutBufExit(ViPipe);
        }

        case ISP_CLUT_BUF_GET: {
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);
            if (g_astIspDrvCtx[ViPipe].stClutBufAttr.init == HI_FALSE) {
                return HI_ERR_ISP_NOMEM;
            }
            *(HI_U64 *)(HI_UINTPTR_T)arg = g_astIspDrvCtx[ViPipe].stClutBufAttr.clut_buf.phy_addr;

            return HI_SUCCESS;
        }

        case ISP_LDCI_BUF_INIT: {
            return ISP_DRV_LdciBufInit(ViPipe);
        }

        case ISP_LDCI_BUF_EXIT: {
            return ISP_DRV_LdciBufExit(ViPipe);
        }

        case ISP_LDCI_READ_STT_BUF_GET: {
            return ISP_DRV_LdciReadSttBufGet(ViPipe, (isp_ldci_read_stt_buf *)(HI_UINTPTR_T)arg);
        }

        case ISP_OPT_RUNONCE_INFO: {
            return ISP_DRV_OptRunOnceInfo(ViPipe, (HI_BOOL *)(HI_UINTPTR_T)arg);
        }

        case ISP_YUV_RUNONCE_INFO: {
            return ISP_DRV_YUVRunOnceInfo(ViPipe, (HI_BOOL *)(HI_UINTPTR_T)arg);
        }

        case ISP_KERNEL_RUNONCE: {
            return ISP_DRV_RunOnceProcess(ViPipe);
        }
        case ISP_KERNEL_YUV_RUNONCE: {
            return ISP_DRV_YUVRunOnceProcess(ViPipe);
        }
        case ISP_SET_PROCALCDONE: {
            ISP_CHECK_PIPE(ViPipe);
            g_astIspDrvCtx[ViPipe].bProStart = HI_TRUE;

            return HI_SUCCESS;
        }

        case ISP_STT_BUF_INIT: {
            return ISP_DRV_SttBufInit(ViPipe);
        }

        case ISP_STT_ADDR_INIT: {
            ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
            ISP_CHECK_PIPE(ViPipe);

            pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

            if (pstDrvCtx->stStitchAttr.stitch_enable == HI_FALSE) {
                return ISP_DRV_FeSttAddrInit(ViPipe);
            } else {
                return ISP_DRV_FeStitchSttAddrInit(ViPipe);
            }
        }

        case ISP_BE_LUT_STT_BUF_GET: {
            HI_U64 u64PhyAddr;
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            if (g_astIspDrvCtx[ViPipe].stBeLut2SttAttr.init == HI_FALSE) {
                return HI_ERR_ISP_NOMEM;
            }
            u64PhyAddr = g_astIspDrvCtx[ViPipe].stBeLut2SttAttr.be_lut_stt_buf[0].lut_stt_buf[0].phy_addr;
            *(HI_U64 *)(HI_UINTPTR_T)arg = u64PhyAddr;

            return HI_SUCCESS;
        }

        case ISP_STT_BUF_EXIT: {
            return ISP_DRV_SttBufExit(ViPipe);
        }

        case ISP_SPECAWB_BUF_INIT: {
            return ISP_DRV_SpecAwbBufInit(ViPipe);
        }

        case ISP_SPECAWB_BUF_EXIT: {
            return ISP_DRV_SpecAwbBufExit(ViPipe);
        }

        case ISP_SPECAWB_BUF_GET: {
#ifdef CONFIG_HI_ISP_SPECAWB_SUPPORT
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);
            if (g_astIspDrvCtx[ViPipe].stSpecAwbBufAttr.init == HI_FALSE) {
                return HI_ERR_ISP_NOMEM;
            }
            *(HI_U64 *)(HI_UINTPTR_T)arg = g_astIspDrvCtx[ViPipe].stSpecAwbBufAttr.spec_awb_buf.phy_addr;

            return HI_SUCCESS;
#else
            return HI_ERR_ISP_NOT_SUPPORT;
#endif
        }
        case ISP_SET_RGBIR_FORMAT: {
            ISP_CHECK_PIPE(ViPipe);
            g_astIspDrvCtx[ViPipe].enIspRgbirFormat = *(HI_U32 *)(HI_UINTPTR_T)arg;
            return HI_SUCCESS;
        }
        case ISP_AWB_EN_SET: {
            HI_BOOL bEnable;

            ISP_CHECK_POINTER(arg);
            ISP_CHECK_BOOL(*(HI_BOOL *)(HI_UINTPTR_T)arg);
            bEnable = *(HI_BOOL*)(HI_UINTPTR_T)(arg);

            return ISP_SetAwbStaEnable(ViPipe, bEnable);
        }

        default: {
            return VREG_DRV_ioctl(cmd, arg, private_data);
        }

    }

    return 0;
}

#ifdef CONFIG_COMPAT
static long ISP_compat_ioctl(unsigned int cmd, unsigned long arg, void *private_data)
{
    switch (cmd) {
        case ISP_DEV_SET_FD: {
            break;
        }

        case ISP_GET_VERSION: {
            break;
        }

        case ISP_GET_FRAME_EDGE: {
            break;
        }

        case ISP_GET_VD_TIMEOUT: {
            break;
        }

        case ISP_GET_VD_END_TIMEOUT: {
            break;
        }

        case ISP_GET_VD_BEEND_TIMEOUT: {
            break;
        }

        case ISP_FRAME_INFO_SET: {
            break;
        }

        case ISP_FRAME_INFO_GET: {
            break;
        }

        case IOC_NR_ISP_FRAME_CNT_GET: {
            break;
        }
        case ISP_SET_INT_ENABLE: {
            break;
        }

        case ISP_STAT_BUF_INIT: {
            break;
        }

        case ISP_STAT_BUF_EXIT: {
            break;
        }

        case ISP_STAT_BUF_GET: {
            break;
        }

        case ISP_STAT_BUF_PUT: {
            break;
        }

        case ISP_STAT_ACT_GET: {
            break;
        }

        case ISP_REG_CFG_SET: {
            break;
        }

        case ISP_BE_CFG_BUF_INIT: {
            isp_be_wo_cfg_buf *pstBeWoCfgBuf = (isp_be_wo_cfg_buf *)(HI_UINTPTR_T)arg;
            COMPAT_POINTER(pstBeWoCfgBuf->vir_addr, HI_VOID *);
            break;
        }

        case ISP_GET_BE_BUF_FIRST: {
            break;
        }

        case ISP_BE_FREE_BUF_GET: {
            break;
        }

        case ISP_BE_CFG_BUF_EXIT: {
            break;
        }

        case ISP_BE_CFG_BUF_RUNNING: {
            break;
        }

        case ISP_BE_CFG_BUF_CTL: {
            break;
        }

        case ISP_BE_All_BUF_INIT: {
            break;
        }

        case ISP_SYNC_CFG_SET: {
            break;
        }

        case ISP_WDR_CFG_SET: {
            break;
        }

        case ISP_BE_SYNC_PARAM_INIT: {
            break;
        }

        case ISP_RES_SWITCH_SET: {
            break;
        }

        case ISP_CHN_SELECT_CFG: {
            break;
        }

        case ISP_PROC_INIT: {
            break;
        }

        case ISP_PROC_WRITE_ING: {
            break;
        }

        case ISP_PROC_WRITE_OK: {
            break;
        }

        case ISP_PROC_EXIT: {
            break;
        }

        case ISP_PROC_PARAM_GET: {
            break;
        }

        case ISP_MEM_INFO_SET: {
            break;
        }

        case ISP_MEM_INFO_GET: {
            break;
        }

        case ISP_P2EN_INFO_GET: {
            break;
        }

        case ISP_SYNC_INIT_SET: {
            break;
        }

        case ISP_RESET_CTX: {
            break;
        }

        case ISP_CONFIG_INFO_SET: {
            break;
        }

        case ISP_SNAP_INFO_GET: {
            break;
        }

        case ISP_PRO_TRIGGER_GET: {
            break;
        }

        case ISP_SNAP_ATTR_GET: {
            break;
        }

        case ISP_UPDATE_POS_GET: {
            break;
        }

        case ISP_PUB_ATTR_INFO: {
            break;
        }

        case ISP_PWM_NUM_GET: {
            break;
        }

        case ISP_SET_MOD_PARAM: {
            break;
        }

        case ISP_GET_MOD_PARAM: {
            break;
        }

        case ISP_WORK_MODE_INIT: {
            break;
        }

        case ISP_WORK_MODE_EXIT: {
            break;
        }

        case ISP_WORK_MODE_GET: {
            break;
        }

        case ISP_GET_PIPE_SIZE: {
            break;
        }

        case ISP_GET_HDR_ATTR: {
            break;
        }

        case ISP_GET_WDR_ATTR: {
            break;
        }

        case ISP_GET_STITCH_ATTR: {
            break;
        }

        case ISP_KERNEL_RUNONCE: {
            break;
        }

        case ISP_SET_PROCALCDONE: {
            break;
        }

        case ISP_LDCI_BUF_INIT: {
            break;
        }

        case ISP_LDCI_READ_STT_BUF_GET: {
            break;
        }

        case ISP_LDCI_BUF_EXIT: {
            break;
        }

        default: {
            break;
        }
    }

    return ISP_ioctl(cmd, arg, private_data);
}
#endif

static int ISP_open(void *data)
{
    return 0;
}

static int ISP_close(void *data)
{
    return 0;
}

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static HI_S32 ISP_Freeze(osal_dev_t *pdev)
{
    return HI_SUCCESS;
}

static HI_S32 ISP_Restore(osal_dev_t *pdev)
{
    VI_PIPE ViPipe;

    for (ViPipe = 0; ViPipe < ISP_MAX_PIPE_NUM; ViPipe++) {
        /* enable port int & isp int */
        IO_RW_PT_ADDRESS(VICAP_HD_MASK) |= VICAP_INT_MASK_PT(ViPipe);
        IO_RW_PT_ADDRESS(VICAP_HD_MASK) |= VICAP_INT_MASK_ISP(ViPipe);

        IO_RW_FE_ADDRESS(ViPipe, ISP_INT_FE_MASK) = (0x0);
    }

    return HI_SUCCESS;
}
#else
static HI_S32 ISP_Freeze(osal_dev_t *pdev)
{
    return HI_SUCCESS;
}

static HI_S32 ISP_Restore(osal_dev_t *pdev)
{
    return HI_SUCCESS;
}
#endif

static struct osal_fileops stIspFileOp = {
    .unlocked_ioctl   = ISP_ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl     = ISP_compat_ioctl,
#endif
    .open             = ISP_open,
    .release          = ISP_close
};

struct osal_pmops stHiISPDrvOps = {
    .pm_freeze      = ISP_Freeze,
    .pm_restore     = ISP_Restore
};

static osal_dev_t *s_pstHiISPDevice = HI_NULL;

#ifdef CONFIG_HI_ISP_LITEOS_BOTTOM_HALF_SUPPORT
static void ISP_DRV_WorkQueueHandler(struct osal_work_struct *pstWorker)
{
    ISP_WORKQUEUE_CTX_S *pstIspWorkQueue = osal_container_of((void *)pstWorker, ISP_WORKQUEUE_CTX_S, worker);
    if (osal_down_interruptible(&pstIspWorkQueue->sem)) {
        return;
    }
    ISP_DoIntBottomHalf(0, 0);
    osal_up(&pstIspWorkQueue->sem);

    return;
}

void ISP_DRV_WorkQueueRun(VI_PIPE ViPipe)
{
    osal_schedule_work(&g_astIspWorkQueueCtx.worker);
}

void ISP_DRV_WorkQueueInit(void)
{
    osal_sema_init(&g_astIspWorkQueueCtx.sem, 1);
    OSAL_INIT_WORK(&g_astIspWorkQueueCtx.worker, ISP_DRV_WorkQueueHandler);
}

void ISP_DRV_WorkQueueExit(void)
{
    osal_sema_destory(&g_astIspWorkQueueCtx.sem);
    osal_destroy_work(&g_astIspWorkQueueCtx.worker);
}
#endif

static hi_void isp_update_interrupt_info(VI_PIPE vi_pipe, ISP_DRV_CTX_S *drv_ctx, ISP_INTERRUPT_SCH_S *int_sch)
{
    hi_u8 write_flag;
    unsigned long u32Flags;

    osal_spin_lock_irqsave(&g_stIspLock[vi_pipe], &u32Flags);
    if (isp_interrupt_buf_is_full(&drv_ctx->isp_int_info)) {
        ISP_WARN_TRACE("ISP[%d] interrupts buf is full!\n", vi_pipe);
        osal_memset(&drv_ctx->isp_int_info, 0, sizeof(isp_interrupt_info));
    }

    osal_memset(&drv_ctx->isp_int_info.int_sch[drv_ctx->isp_int_info.write_flag], 0, sizeof(ISP_INTERRUPT_SCH_S));

    if (int_sch->u32IspIntStatus || int_sch->u32PortIntStatus ||
        ((int_sch->u32WCHIntStatus != 0) && (drv_ctx->bYuvMode == HI_TRUE))) {
        write_flag = drv_ctx->isp_int_info.write_flag;
        osal_memcpy(&drv_ctx->isp_int_info.int_sch[write_flag], int_sch, sizeof(ISP_INTERRUPT_SCH_S));
        drv_ctx->isp_int_info.write_flag = (write_flag + 1) % ISP_INTERRUPTS_SAVEINFO_MAX;
    }

    osal_spin_unlock_irqrestore(&g_stIspLock[vi_pipe], &u32Flags);
}

static hi_s32 isp_get_interrupt_info(VI_PIPE vi_pipe, ISP_DRV_CTX_S *drv_ctx)
{
    hi_u8 read_flag;
    unsigned long u32Flags;
    ISP_INTERRUPT_SCH_S *int_sch = HI_NULL;

    osal_spin_lock_irqsave(&g_stIspLock[vi_pipe], &u32Flags);
    read_flag = drv_ctx->isp_int_info.read_flag;
    int_sch   = &drv_ctx->isp_int_info.int_sch[read_flag];

    if ((!int_sch->u32IspIntStatus) && (!int_sch->u32PortIntStatus) &&
        (!(int_sch->u32WCHIntStatus && (drv_ctx->bYuvMode == HI_TRUE)))) {
        osal_spin_unlock_irqrestore(&g_stIspLock[vi_pipe], &u32Flags);
        return 0;
    }

    if (isp_interrupt_buf_is_empty(&drv_ctx->isp_int_info)) {
        ISP_WARN_TRACE("ISP[%d] interrupts buf is empty\n", vi_pipe);
        osal_spin_unlock_irqrestore(&g_stIspLock[vi_pipe], &u32Flags);
        return 0;
    }

    osal_memcpy(&drv_ctx->stIntSch, &drv_ctx->isp_int_info.int_sch[read_flag], sizeof(ISP_INTERRUPT_SCH_S));
    osal_memset(&drv_ctx->isp_int_info.int_sch[read_flag], 0, sizeof(ISP_INTERRUPT_SCH_S));
    drv_ctx->isp_int_info.read_flag = (read_flag + 1) % ISP_INTERRUPTS_SAVEINFO_MAX;
    osal_spin_unlock_irqrestore(&g_stIspLock[vi_pipe], &u32Flags);
    return 1;
}

static inline int ISP_ISR(int irq, void *id)
{
    HI_U32  i;
    HI_S32  ViDev;
    VI_PIPE ViPipe;
    HI_U32 u32PortIntStatus = 0;
    HI_U32 u32PortIntFStart = 0;
    HI_U32 u32PortIntErr = 0;
    HI_U32 u32IspRawIntStatus = 0;
    HI_U32 u32IspIntStatus = 0;
    HI_U32 u32WCHIntStatus = 0;
    HI_U32 u32WCHIntFStart = 0;
    HI_U64 u64IspIrqBeginTime;
    HI_BOOL bViCapInt;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_INTERRUPT_SCH_S int_sch = { 0 };

    bViCapInt = (irq == isp_fe_irq);

    u64IspIrqBeginTime = call_sys_get_time_stamp();

    /* Isp FE Interrupt Process Begin */
    if (bViCapInt) {
        for (i = 0; i < ISP_MAX_PHY_PIPE_NUM; i++) {
            ViPipe = i;

            pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
            ViDev     = pstDrvCtx->stWDRAttr.vi_dev;

            /* read interrupt status */
            u32PortIntStatus  = IO_RW_PT_ADDRESS(VI_PT_BASE(ViDev) + VI_PT_INT);
            u32PortIntStatus &= IO_RW_PT_ADDRESS(VI_PT_BASE(ViDev) + VI_PT_INT_MASK);
            u32PortIntFStart  = u32PortIntStatus & VI_PT_INT_FSTART;
            u32PortIntErr     = u32PortIntStatus & VI_PT_INT_ERR;

            u32WCHIntStatus   = IO_RW_CH_ADDRESS(ViPipe, VI_WCH_INT);
            u32WCHIntFStart   = u32WCHIntStatus & VI_WCH_INT_FSTART;

            u32IspRawIntStatus  = IO_RW_FE_ADDRESS(ViPipe, ISP_INT_FE);
            u32IspIntStatus     = u32IspRawIntStatus & IO_RW_FE_ADDRESS(ViPipe, ISP_INT_FE_MASK);

            int_sch.u32IspIntStatus  = u32IspIntStatus;
            int_sch.u32PortIntStatus = u32PortIntFStart;
            int_sch.u32PortIntErr    = u32PortIntErr;
            int_sch.u32WCHIntStatus  = u32WCHIntFStart;

            pstDrvCtx->stIspSyncCtrl.u64PTS = u64IspIrqBeginTime;

            isp_update_interrupt_info(ViPipe, pstDrvCtx, &int_sch);

            /* clear interrupt */
            if (u32PortIntStatus || u32IspRawIntStatus) {
                if (u32PortIntStatus) {
                    IO_RW_PT_ADDRESS(VI_PT_BASE(ViDev) + VI_PT_INT) = u32PortIntStatus;
                }

                if (u32IspRawIntStatus) {
                    IO_RW_FE_ADDRESS(ViPipe, ISP_INT_FE) = u32IspRawIntStatus;
                }
            } else {
                continue;
            }

            if (u32PortIntErr) {
                pstDrvCtx->stDrvDbgInfo.u32IspResetCnt++;
                continue;
            }
        }

        if (!g_UseBothalf) {
            ISP_DoIntBottomHalf(irq, id);
        }
#ifdef CONFIG_HI_ISP_LITEOS_BOTTOM_HALF_SUPPORT
#ifdef __HuaweiLite__
        else {
            ISP_DRV_WorkQueueRun(ViPipe);
        }
#endif
#endif
    }

    return OSAL_IRQ_WAKE_THREAD;
}

int ISP_IntBottomHalf(int irq, void *id)
{
    if (g_UseBothalf) {
        return ISP_DoIntBottomHalf(irq, id);
    } else {
        return OSAL_IRQ_HANDLED;
    }
}

HI_VOID ISP_DRV_WakeUpThread(VI_PIPE ViPipe)
{
    HI_BOOL bWakeUpTread = HI_TRUE;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (IS_OFFLINE_MODE(pstDrvCtx->stWorkMode.running_mode) ||
        IS_STRIPING_MODE(pstDrvCtx->stWorkMode.running_mode)) {
        if (pstDrvCtx->stStitchAttr.stitch_enable == HI_TRUE) {
            if (pstDrvCtx->enIspRunningState != ISP_BE_BUF_STATE_INIT) {
                bWakeUpTread = HI_FALSE;
            }
        }
    }

    if (bWakeUpTread == HI_TRUE) {
        pstDrvCtx->bEdge = HI_TRUE;
        pstDrvCtx->bVdStart = HI_TRUE;

        osal_wakeup(&pstDrvCtx->stIspWait);
        osal_wakeup(&pstDrvCtx->stIspWaitVdStart);
    }

    return;
}

HI_S32 ISP_DRV_GetUseNode(VI_PIPE ViPipe, HI_U32 u32IspIntStatus, HI_U32 u32WCHIntStatus)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_SYNC_CFG_S *pstSyncCfg = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx  = ISP_DRV_GET_CTX(ViPipe);
    pstSyncCfg = &pstDrvCtx->stSyncCfg;

    if (IS_OFFLINE_MODE(pstDrvCtx->stWorkMode.running_mode) ||
        IS_STRIPING_MODE(pstDrvCtx->stWorkMode.running_mode)) {
        ISP_CHECK_BEBUF_INIT(ViPipe);

        if (u32IspIntStatus & ISP_1ST_INT) {
            if (pstDrvCtx->pstUseNode) {
                /* Need to configure the sensor registers and get statistics for AE/AWB. */
                osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

                ISP_DRV_GetSyncControlnfo(ViPipe, pstSyncCfg);
                ISP_DRV_CalcSyncCfg(pstSyncCfg);
                ISP_DRV_RegConfigIsp(ViPipe, pstDrvCtx);
                ISP_DRV_RegConfigSensor(ViPipe, pstDrvCtx);

                pstDrvCtx->u32Status = u32IspIntStatus;

                osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

                if (pstDrvCtx->u32FrameCnt++ % DIV_0_TO_1(g_StatIntvl[ViPipe]) == 0) {
                    ISP_DRV_StatBufBusyPut(ViPipe);
                }

                osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
                ISP_DRV_WakeUpThread(ViPipe);
                osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
                IspSyncTaskProcess(ViPipe);

                return HI_FAILURE;
            }

            osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

            pstDrvCtx->pstUseNode = ISP_QueueGetFreeBeBuf(&pstDrvCtx->stBeBufQueue);

            if (pstDrvCtx->pstUseNode == HI_NULL) {
                /* Need to configure the sensor registers. */
                ISP_DRV_RegConfigSensor(ViPipe, pstDrvCtx);
                osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
                ISP_ERR_TRACE("Pipe[%d] get FreeBeBuf is fail!\r\n", ViPipe);

                return HI_FAILURE;
            }

            osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        }

        if ((pstDrvCtx->bYuvMode == HI_TRUE) && u32WCHIntStatus && (pstDrvCtx->bIspRunFlag == HI_TRUE)) {
            if (pstDrvCtx->pstUseNode) {
                osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

                ISP_DRV_GetSyncControlnfo(ViPipe, pstSyncCfg);
                ISP_DRV_CalcSyncCfg(pstSyncCfg);
                ISP_DRV_RegConfigIsp(ViPipe, pstDrvCtx);

                pstDrvCtx->u32Status = u32IspIntStatus;

                osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

                if (pstDrvCtx->u32FrameCnt++ % DIV_0_TO_1(g_StatIntvl[ViPipe]) == 0) {
                    ISP_DRV_StatBufBusyPut(ViPipe);
                }

                osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
                ISP_DRV_WakeUpThread(ViPipe);
                osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
                IspSyncTaskProcess(ViPipe);

                return HI_FAILURE;
            }

            osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

            pstDrvCtx->pstUseNode = ISP_QueueGetFreeBeBuf(&pstDrvCtx->stBeBufQueue);

            if (pstDrvCtx->pstUseNode == HI_NULL) {
                osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
                ISP_ERR_TRACE("Pipe[%d] get FreeBeBuf is fail!\r\n", ViPipe);

                return HI_FAILURE;
            }

            osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_StitchGetUseNode(VI_PIPE ViPipe, HI_U32 u32IspIntStatus)
{
    VI_PIPE MainPipeS;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_SYNC_CFG_S *pstSyncCfg = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx  = ISP_DRV_GET_CTX(ViPipe);
    pstSyncCfg = &pstDrvCtx->stSyncCfg;
    MainPipeS  = pstDrvCtx->stStitchAttr.stitch_bind_id[0];

    if (IS_OFFLINE_MODE(pstDrvCtx->stWorkMode.running_mode) ||
        IS_STRIPING_MODE(pstDrvCtx->stWorkMode.running_mode)) {
        ISP_CHECK_BEBUF_INIT(ViPipe);

        if (u32IspIntStatus & ISP_1ST_INT) {
            osal_spin_lock_irqsave(&g_stIspSyncLock[MainPipeS], &u32Flags);

            if (pstDrvCtx->pstUseNode) {
                /* Need to configure the sensor registers and get statistics for AE/AWB. */
                ISP_DRV_GetSyncControlnfo(ViPipe, pstSyncCfg);
                ISP_DRV_CalcSyncCfg(pstSyncCfg);
                ISP_DRV_RegConfigIsp(ViPipe, pstDrvCtx);
                ISP_DRV_RegConfigSensor(ViPipe, pstDrvCtx);

                pstDrvCtx->u32Status = u32IspIntStatus;

                if (pstDrvCtx->u32FrameCnt++ % DIV_0_TO_1(g_StatIntvl[ViPipe]) == 0) {
                    ISP_DRV_StatBufBusyPut(ViPipe);
                }

                ISP_DRV_WakeUpThread(ViPipe);
                osal_spin_unlock_irqrestore(&g_stIspSyncLock[MainPipeS], &u32Flags);
                IspSyncTaskProcess(ViPipe);

                return HI_FAILURE;
            }

            pstDrvCtx->pstUseNode = ISP_QueueGetFreeBeBuf(&pstDrvCtx->stBeBufQueue);

            if (pstDrvCtx->pstUseNode == HI_NULL) {
                /* Need to configure the sensor registers. */
                ISP_DRV_RegConfigSensor(ViPipe, pstDrvCtx);
                osal_spin_unlock_irqrestore(&g_stIspSyncLock[MainPipeS], &u32Flags);
                ISP_ERR_TRACE("Pipe[%d] get FreeBeBuf is fail!\r\n", ViPipe);

                return HI_FAILURE;
            }

            osal_spin_unlock_irqrestore(&g_stIspSyncLock[MainPipeS], &u32Flags);
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_GetBeBufUseNode(VI_PIPE ViPipe, HI_U32 u32IspIntStatus)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U32 u32WCHIntFStart;
    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    u32WCHIntFStart = pstDrvCtx->stIntSch.u32WCHIntStatus;
    if (pstDrvCtx->stStitchAttr.stitch_enable != HI_TRUE) {
        return ISP_DRV_GetUseNode(ViPipe, u32IspIntStatus, u32WCHIntFStart);
    } else {
        return ISP_DRV_StitchGetUseNode(ViPipe, u32IspIntStatus);
    }
}

int  ISP_IrqRoute(VI_PIPE ViPipe)
{
    HI_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_SYNC_CFG_S *pstSyncCfg = HI_NULL;
    HI_U64 u64SensorCfgTime1 = 0;
    HI_U64 u64SensorCfgTime2 = 0;
    HI_U32 u32PortIntFStart, u32IspIntStatus, u32WCHIntFStart, u32SensorCfgInt;
    HI_U64 u64PtTime1 = 0;
    HI_U64 u64PtTime2 = 0;
    HI_U64 u64IspTime1 = 0;
    HI_U64 u64IspTime2 = 0;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    pstSyncCfg = &pstDrvCtx->stSyncCfg;

    u32PortIntFStart = pstDrvCtx->stIntSch.u32PortIntStatus;
    u32IspIntStatus  = pstDrvCtx->stIntSch.u32IspIntStatus;
    u32WCHIntFStart  = pstDrvCtx->stIntSch.u32WCHIntStatus;

    if (u32PortIntFStart) {  /* port int proc */
        pstDrvCtx->stDrvDbgInfo.u32PtIntCnt++;
        u64PtTime1 = call_sys_get_time_stamp();

        if (pstDrvCtx->stDrvDbgInfo.u64PtLastIntTime) {  // not first int
            pstDrvCtx->stDrvDbgInfo.u32PtIntGapTime = u64PtTime1 - pstDrvCtx->stDrvDbgInfo.u64PtLastIntTime;

            if (pstDrvCtx->stDrvDbgInfo.u32PtIntGapTime > pstDrvCtx->stDrvDbgInfo.u32PtIntGapTimeMax) {
                pstDrvCtx->stDrvDbgInfo.u32PtIntGapTimeMax = pstDrvCtx->stDrvDbgInfo.u32PtIntGapTime;
            }
        }

        pstDrvCtx->stDrvDbgInfo.u64PtLastIntTime = u64PtTime1;
    }

    if (u32IspIntStatus & ISP_1ST_INT) {  /* isp int proc */
        pstDrvCtx->stDrvDbgInfo.u32IspIntCnt++;
        u64IspTime1 = call_sys_get_time_stamp();

        if (pstDrvCtx->stDrvDbgInfo.u64IspLastIntTime) {   // not first int
            pstDrvCtx->stDrvDbgInfo.u32IspIntGapTime = u64IspTime1 - pstDrvCtx->stDrvDbgInfo.u64IspLastIntTime;

            if (pstDrvCtx->stDrvDbgInfo.u32IspIntGapTime > pstDrvCtx->stDrvDbgInfo.u32IspIntGapTimeMax) {
                pstDrvCtx->stDrvDbgInfo.u32IspIntGapTimeMax = pstDrvCtx->stDrvDbgInfo.u32IspIntGapTime;
            }
        }

        pstDrvCtx->stDrvDbgInfo.u64IspLastIntTime = u64IspTime1;
    }

    pstDrvCtx->u32IntPos = 0;

    s32Ret = ISP_DRV_GetBeBufUseNode(ViPipe, u32IspIntStatus);
    if (s32Ret != HI_SUCCESS) {
        return OSAL_IRQ_HANDLED;
    }

    if (u32IspIntStatus & ISP_1ST_INT) {
        if (ViPipe == pstDrvCtx->stSnapAttr.picture_pipe_id) {
            if (ckfn_vi_get_isp_config()) {
                call_vi_get_isp_config(ViPipe, &pstDrvCtx->stSnapInfoLoad);
            }
        }
    }

    if (u32IspIntStatus & ISP_1ST_INT) {
        if (IS_FULL_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)) {
            ISP_DRV_GetSyncControlnfo(ViPipe, pstSyncCfg);
            ISP_DRV_CalcSyncCfg(pstSyncCfg);
            ISP_DRV_RegConfigIsp(ViPipe, pstDrvCtx);
            ISP_DRV_RegConfigSensor(ViPipe, pstDrvCtx);
        }
    }

    if (u32PortIntFStart) {
        if (IS_HALF_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)) {
            ISP_DRV_GetSyncControlnfo(ViPipe, pstSyncCfg);
            ISP_DRV_CalcSyncCfg(pstSyncCfg);
            ISP_DRV_RegConfigIsp(ViPipe, pstDrvCtx);
            ISP_DRV_RegConfigSensor(ViPipe, pstDrvCtx);
        }
    }

    /*  * * * * * * * isp_int1_process * * * * * * *  */
    if (u32IspIntStatus & ISP_1ST_INT) {
        if (IS_LINE_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)) {
            ISP_DRV_GetSyncControlnfo(ViPipe, pstSyncCfg);
            ISP_DRV_CalcSyncCfg(pstSyncCfg);
            ISP_DRV_RegConfigIsp(ViPipe, pstDrvCtx);

            u64SensorCfgTime1 = call_sys_get_time_stamp();
            ISP_DRV_RegConfigSensor(ViPipe, pstDrvCtx);
            u64SensorCfgTime2 = call_sys_get_time_stamp();
            pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime = u64SensorCfgTime2 - u64SensorCfgTime1;
        }
    }

    u32SensorCfgInt = (u32IspIntStatus & ISP_2ND_INT);

    if (g_UpdatePos[ViPipe] == 0) {  /* frame start */
        u32SensorCfgInt = (u32IspIntStatus & ISP_1ST_INT);
    }

    /*  * * * * * * * isp_int2_process * * * * * * *  */
    if (u32SensorCfgInt) {
        /* In linear mode or built-in WDR mode,
                  config sensor and vi(isp) register with isp_int(frame start interrupt) */
        if (IS_LINEAR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode) || IS_BUILT_IN_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)) {
            ISP_DRV_GetSyncControlnfo(ViPipe, pstSyncCfg);
            ISP_DRV_CalcSyncCfg(pstSyncCfg);
            ISP_DRV_RegConfigIsp(ViPipe, pstDrvCtx);

            u64SensorCfgTime1 = call_sys_get_time_stamp();
            ISP_DRV_RegConfigSensor(ViPipe, pstDrvCtx);
            u64SensorCfgTime2 = call_sys_get_time_stamp();
            pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime = u64SensorCfgTime2 - u64SensorCfgTime1;
        }
    }

    if (u32IspIntStatus & ISP_2ND_INT) {
        pstDrvCtx->u32IntPos = 1;
        pstDrvCtx->u32Status = u32IspIntStatus;

        ISP_DRV_RegConfigSensor(ViPipe, pstDrvCtx);

        pstDrvCtx->bVdEnd = HI_TRUE;
        if (pstDrvCtx->u32ProTrigFlag == 1) {
            pstDrvCtx->u32ProTrigFlag++;
        }
        ISP_UpdateInfoSync(ViPipe, &pstDrvCtx->stUpdateInfo);
        ISP_frameInfoSync(ViPipe, &pstDrvCtx->stFrameInfo);
        osal_wakeup(&pstDrvCtx->stIspWaitVdEnd);
    }

    if (pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime > pstDrvCtx->stDrvDbgInfo.u32SensorCfgTimeMax) {
        pstDrvCtx->stDrvDbgInfo.u32SensorCfgTimeMax = pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime;
    }

    /*  * * * * * * * isp_int_process * * * * * * *  */
    if (u32IspIntStatus & ISP_1ST_INT) {  /* ISP int */
        /* N to 1 fullrate frame WDR mode, get statistics only in the last frame(N-1) */
        if (IS_FULL_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)) {
            if (pstDrvCtx->stSyncCfg.u8VCNum != pstDrvCtx->stSyncCfg.u8VCNumMax) {
                return OSAL_IRQ_HANDLED;
            }
        }

        pstDrvCtx->u32Status = u32IspIntStatus;

        /* first config register. */
        if (pstDrvCtx->u32FrameCnt++ % DIV_0_TO_1(g_StatIntvl[ViPipe]) == 0) {
            ISP_DRV_StatBufBusyPut(ViPipe);
        }

        ISP_DRV_WakeUpThread(ViPipe);
        /* Sync  task AF statistics */
        IspSyncTaskProcess(ViPipe);
    }

    /*  * * * * * * * isp_int_process * * * * * * *  */
    if (u32WCHIntFStart && (pstDrvCtx->bYuvMode == HI_TRUE) && (pstDrvCtx->bIspRunFlag == HI_TRUE)) {  /* WCH int */
        pstDrvCtx->u32Status = u32WCHIntFStart;

        /* first config register. */
        if (pstDrvCtx->u32FrameCnt++ % DIV_0_TO_1(g_StatIntvl[ViPipe]) == 0) {
            ISP_DRV_StatBufBusyPut(ViPipe);
        }

        ISP_DRV_WakeUpThread(ViPipe);
        /* Sync  task AF statistics */
        IspSyncTaskProcess(ViPipe);
    }

    if (u32PortIntFStart) { /* port int proc */
        u64PtTime2 = call_sys_get_time_stamp();
        pstDrvCtx->stDrvDbgInfo.u32PtIntTime = u64PtTime2 - u64PtTime1;

        if (pstDrvCtx->stDrvDbgInfo.u32PtIntTime > pstDrvCtx->stDrvDbgInfo.u32PtIntTimeMax) {
            pstDrvCtx->stDrvDbgInfo.u32PtIntTimeMax = pstDrvCtx->stDrvDbgInfo.u32PtIntTime;
        }

        if ((u64PtTime2 - pstDrvCtx->stDrvDbgInfo.u64PtLastRateTime) >= 1000000ul) {
            pstDrvCtx->stDrvDbgInfo.u64PtLastRateTime = u64PtTime2;
            pstDrvCtx->stDrvDbgInfo.u32PtRate = pstDrvCtx->stDrvDbgInfo.u32PtRateIntCnt;
            pstDrvCtx->stDrvDbgInfo.u32PtRateIntCnt = 0;
        }

        pstDrvCtx->stDrvDbgInfo.u32PtRateIntCnt++;
    }

    if (u32IspIntStatus & ISP_1ST_INT) { /* isp int proc */
        u64IspTime2 = call_sys_get_time_stamp();
        pstDrvCtx->stDrvDbgInfo.u32IspIntTime = u64IspTime2 - u64IspTime1;

        if (pstDrvCtx->stDrvDbgInfo.u32IspIntTime > pstDrvCtx->stDrvDbgInfo.u32IspIntTimeMax) {
            pstDrvCtx->stDrvDbgInfo.u32IspIntTimeMax = pstDrvCtx->stDrvDbgInfo.u32IspIntTime;
        }

        if ((u64IspTime2 - pstDrvCtx->stDrvDbgInfo.u64IspLastRateTime) >= 1000000ul) {
            pstDrvCtx->stDrvDbgInfo.u64IspLastRateTime = u64IspTime2;
            pstDrvCtx->stDrvDbgInfo.u32IspRate = pstDrvCtx->stDrvDbgInfo.u32IspRateIntCnt;
            pstDrvCtx->stDrvDbgInfo.u32IspRateIntCnt = 0;
        }

        pstDrvCtx->stDrvDbgInfo.u32IspRateIntCnt++;
    }

    return OSAL_IRQ_HANDLED;
}

int  ISP_DoIntBottomHalf(int irq, void *id)
{
    VI_PIPE ViPipe, ViPipeS;
    HI_U32 i, j;
    HI_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_DRV_CTX_S *pstDrvCtxS = HI_NULL;
    HI_U32  u32PortIntStatus = 0;
    HI_U32  u32PortIntErr = 0;
    HI_U32  u32IspIntStatus = 0;
    HI_U32  u32WCHIntFStart = 0;
    for (i = 0; i < ISP_MAX_PHY_PIPE_NUM; i++) {
        ViPipe = i;
        pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

        if (isp_get_interrupt_info(ViPipe, pstDrvCtx) == 0) {
            continue;
        }

        u32IspIntStatus  = pstDrvCtx->stIntSch.u32IspIntStatus;
        u32PortIntStatus = pstDrvCtx->stIntSch.u32PortIntStatus;
        u32PortIntErr    = pstDrvCtx->stIntSch.u32PortIntErr;
        u32WCHIntFStart  = pstDrvCtx->stIntSch.u32WCHIntStatus;

        if ((!u32PortIntStatus) && (!u32IspIntStatus) && (!u32WCHIntFStart)) {
            continue;
        }

        if (!pstDrvCtx->bMemInit) {
            continue;
        }

        if (pstDrvCtx->stStitchAttr.stitch_enable == HI_TRUE) {
            if (pstDrvCtx->stStitchAttr.main_pipe == HI_TRUE) {
                s32Ret = ISP_DRV_StitchSync(ViPipe);

                if (s32Ret != HI_SUCCESS) {
                    continue;
                }

                for (j = 0; j < pstDrvCtx->stStitchAttr.stitch_pipe_num; j++) {
                    ViPipeS = pstDrvCtx->stStitchAttr.stitch_bind_id[j];
                    pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);

                    pstDrvCtxS->stIntSch.u32IspIntStatus  = u32IspIntStatus;
                    pstDrvCtxS->stIntSch.u32PortIntStatus = u32PortIntStatus;
                    pstDrvCtxS->stIntSch.u32PortIntErr    = u32PortIntErr;

                    ISP_IrqRoute(ViPipeS);
                }
            } else {
                continue;
            }
        } else {
            ISP_IrqRoute(ViPipe);
        }

    }

    return OSAL_IRQ_HANDLED;

}


static int ISP_DRV_Init(void)
{
    HI_S32 s32Ret;

    s32Ret = ISP_DRV_BeRemap();

    if (s32Ret == HI_FAILURE) {
        return s32Ret;
    }

    s32Ret = ISP_DRV_VicapRemap();
    if (s32Ret == HI_FAILURE) {
        return s32Ret;
    }

    s32Ret = ISP_DRV_FeRemap();

    if (s32Ret == HI_FAILURE) {
        return s32Ret;
    }
#ifdef CONFIG_HI_ISP_LITEOS_BOTTOM_HALF_SUPPORT
    ISP_DRV_WorkQueueInit();
#endif
    return 0;
}

static int ISP_DRV_Exit(void)
{
    ISP_DRV_BeUnmap();

    ISP_DRV_VicapUnmap();

    ISP_DRV_FeUnmap();
#ifdef CONFIG_HI_ISP_LITEOS_BOTTOM_HALF_SUPPORT
    ISP_DRV_WorkQueueExit();
#endif

    return 0;
}

#ifdef TEST_TIME
static int ISP_Test_Init(void)
{
    cmpi_mmz_malloc_nocache(HI_NULL, "ISPStatTest", &g_test_phyaddr, (HI_VOID **)&g_test_pviraddr, 256 * 2);
}

static int ISP_Test_Exit(void)
{
    cmpi_mmz_free(g_test_phyaddr, g_test_pviraddr);
}
#endif


#ifndef DISABLE_DEBUG_INFO
static int ISP_ProcShow(osal_proc_entry_t *s)
{
    hi_u8   i;
    hi_u64  be_buf_size;
    VI_PIPE ViPipe = 0;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    isp_snap_pipe_mode enSnapPipeMode = ISP_SNAP_NONE;

    // osal_seq_printf(s, "\n[ISP] Version: ["MPP_VERSION"], Build Time["__DATE__", "__TIME__"]\n\n");
    osal_seq_printf(s, "\n[ISP] Version: ["MPP_VERSION"], Build Time[202009291840]\n\n");
    osal_seq_printf(s, "\n");

    do {
        pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
        if (!pstDrvCtx->bMemInit) {
            continue;
        }

        if ((pstDrvCtx->stSnapAttr.picture_pipe_id == pstDrvCtx->stSnapAttr.preview_pipe_id) &&
            (pstDrvCtx->stSnapAttr.preview_pipe_id != -1)) {
            enSnapPipeMode = ISP_SNAP_PREVIEW_PICTURE;
        } else if (pstDrvCtx->stSnapAttr.picture_pipe_id == ViPipe) {
            enSnapPipeMode = ISP_SNAP_PICTURE;
        } else if (pstDrvCtx->stSnapAttr.preview_pipe_id == ViPipe) {
            enSnapPipeMode = ISP_SNAP_PREVIEW;
        } else {
            enSnapPipeMode = ISP_SNAP_NONE;
        }

        osal_seq_printf(s, "------------------------------------------------------------------------------\n");
        osal_seq_printf(s, "------------------------------ ISP PROC PIPE[%d] -----------------------------\n", ViPipe);
        osal_seq_printf(s, "------------------------------------------------------------------------------\n");
        osal_seq_printf(s, "\n");

        osal_seq_printf(s, "-----MODULE/CONTROL PARAM-----------------------------------------------------\n");
        osal_seq_printf(s, " %12s" " %12s" " %12s" " %12s" " %12s" " %12s" " %14s" " %12s" " %12s""\n",  "ProcParam",
                        "StatIntvl", "UpdatePos", "IntBothalf", "IntTimeout",
                        "PwmNumber", "PortIntDelay", "QuickStart", "LdciTprFltEn");
        osal_seq_printf(s, " %12u" " %12u" " %12u" " %12u" " %12u" " %12u" " %14u" " %12d" " %12d" "\n",
                        g_ProcParam[ViPipe], g_StatIntvl[ViPipe], g_UpdatePos[ViPipe], g_UseBothalf, g_IntTimeout[ViPipe],
                        g_PwmNumber[ViPipe], g_PortIntDelay[ViPipe], g_QuickStart, g_LdciTprFltEn[ViPipe]);
        osal_seq_printf(s, "\n");

        osal_seq_printf(s, "-----ISP Mode------------------------------------------------------------------\n");
        osal_seq_printf(s, " %15s" " %15s" "\n",  "StitchMode", "RunningMode");
        osal_seq_printf(s, " %15s" " %15s" "\n",
                        pstDrvCtx->stStitchAttr.stitch_enable ? "STITCH" : "NORMAL",
                        (pstDrvCtx->stWorkMode.running_mode == ISP_MODE_RUNNING_OFFLINE) ? "OFFLINE"  :
                        (pstDrvCtx->stWorkMode.running_mode == ISP_MODE_RUNNING_ONLINE) ? "ONLINE"   :
                        (pstDrvCtx->stWorkMode.running_mode == ISP_MODE_RUNNING_SIDEBYSIDE) ? "SBS"      :
                        (pstDrvCtx->stWorkMode.running_mode == ISP_MODE_RUNNING_STRIPING) ? "STRIPING" : "BUTT");
        osal_seq_printf(s, "\n");
        osal_seq_printf(s, "-----DRV INFO-------------------------------------------------------------------\n");

        osal_seq_printf(s, "%11s" "%11s" "%11s" "%11s" "%11s" "%11s" "%9s" "%12s" "%14s\n",
                        "ViPipe", "IntCnt", "IntT", "MaxIntT", "IntGapT", "MaxGapT", "IntRat",
                        "IspResetCnt", "IspBeStaLost");

        osal_seq_printf(s, "%11d" "%11d" "%11d" "%11d" "%11d" "%11d" "%9d" "%12d" "%14d\n", ViPipe,
                        pstDrvCtx->stDrvDbgInfo.u32IspIntCnt,
                        pstDrvCtx->stDrvDbgInfo.u32IspIntTime,
                        pstDrvCtx->stDrvDbgInfo.u32IspIntTimeMax,
                        pstDrvCtx->stDrvDbgInfo.u32IspIntGapTime,
                        pstDrvCtx->stDrvDbgInfo.u32IspIntGapTimeMax,
                        pstDrvCtx->stDrvDbgInfo.u32IspRate,
                        pstDrvCtx->stDrvDbgInfo.u32IspResetCnt,
                        pstDrvCtx->stDrvDbgInfo.u32IspBeStaLost);

        osal_seq_printf(s, "\n");

        osal_seq_printf(s, "%11s" "%11s" "%11s" "%11s" "%11s" "%11s" "%9s" "%11s" "%12s\n", "IntTpye", "PtIntCnt",
                        "PtIntT", "PtMaxIntT", "PtIntGapT", "PtMaxGapT", "PtIntRat", "SensorCfgT", "SensorMaxT");

        osal_seq_printf(s, "%11s" "%11d" "%11d" "%11d" "%11d" "%11d" "%9d" "%11d" "%12d\n",
                        (pstDrvCtx->stFrameIntAttr.int_type == FRAME_INTERRUPT_START) ? "Start" : "Other",
                        pstDrvCtx->stDrvDbgInfo.u32PtIntCnt,
                        pstDrvCtx->stDrvDbgInfo.u32PtIntTime,
                        pstDrvCtx->stDrvDbgInfo.u32PtIntTimeMax,
                        pstDrvCtx->stDrvDbgInfo.u32PtIntGapTime,
                        pstDrvCtx->stDrvDbgInfo.u32PtIntGapTimeMax,
                        pstDrvCtx->stDrvDbgInfo.u32PtRate,
                        pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime,
                        pstDrvCtx->stDrvDbgInfo.u32SensorCfgTimeMax);

        if ((IS_OFFLINE_MODE(pstDrvCtx->stWorkMode.running_mode) ||
            IS_STRIPING_MODE(pstDrvCtx->stWorkMode.running_mode)) &&
            (pstDrvCtx->stBeBufInfo.init == HI_TRUE)) {

            osal_seq_printf(s, "\n");
            osal_seq_printf(s, "-----BeCfg Phy Addr-------------------------------------------------------- \n");

            for (i = 0; i < MAX_ISP_BE_BUF_NUM; i++) {
                osal_seq_printf(s, "%16s""%d""%s", "BeCfg[", i, "]");
            }

            osal_seq_printf(s, "\n");
            be_buf_size = sizeof(isp_be_wo_reg_cfg);
            for (i = 0; i < MAX_ISP_BE_BUF_NUM; i++) {
                osal_seq_printf(s, "%#18llx", pstDrvCtx->stBeBufInfo.be_buf_haddr.phy_addr + i * be_buf_size);
            }
        }

        osal_seq_printf(s, "\n\n");

        /*  show isp attribute here. width/height/bayer_format, etc..
                  Read parameter from memory directly. */
        osal_seq_printf(s, "-----PubAttr INFO--------------------------------------------------------------\n");

        osal_seq_printf(s, "%12s" "%12s" "%12s" "%12s" "%12s" "%12s" "%12s\n",
                        "WndX", "WndY", "WndW", "WndH", "SnsW", "SnsH", "Bayer");

        osal_seq_printf(s, "%12d" "%12d" "%12d" "%12d" "%12d" "%12d" "%12s\n\n",
                        pstDrvCtx->stProcPubInfo.wnd_rect.x,
                        pstDrvCtx->stProcPubInfo.wnd_rect.y,
                        pstDrvCtx->stProcPubInfo.wnd_rect.width,
                        pstDrvCtx->stProcPubInfo.wnd_rect.height,
                        pstDrvCtx->stProcPubInfo.sns_size.width,
                        pstDrvCtx->stProcPubInfo.sns_size.height,
                        (pstDrvCtx->stProcPubInfo.bayer == 0) ? "RGGB" :
                        (pstDrvCtx->stProcPubInfo.bayer == 1) ? "GRBG" :
                        (pstDrvCtx->stProcPubInfo.bayer == 2) ? "GBRG" :
                        (pstDrvCtx->stProcPubInfo.bayer == 3) ? "BGGR" : "BUTT");
        osal_seq_printf(s, "\n");

        /* show isp snap attribute here. SnapType/PipeMode/OPType/ProFrmNum, etc.. */
        osal_seq_printf(s, "-----SNAPATTR INFO-----------------------------------------------------------------\n");

        osal_seq_printf(s, "%12s" "%12s" "%12s" "%12s\n", "SnapType", "PipeMode", "OPType", "ProFrmNum");
        osal_seq_printf(s, "%12s" "%12s" "%12s" "%12d\n\n",
                        (pstDrvCtx->stSnapAttr.snap_type == 0) ? "NORMAL" :
                        (pstDrvCtx->stSnapAttr.snap_type == 1) ? "PRO" : "BUTT",
                        (enSnapPipeMode == 0) ? "NONE" :
                        (enSnapPipeMode == 1) ? "PREVIEW" :
                        (enSnapPipeMode == 2) ? "PICTURE" : "PRE_PIC",
                        (pstDrvCtx->stSnapAttr.pro_param.operation_mode == 0) ? "Auto" : "Manul",
                        pstDrvCtx->stSnapAttr.pro_param.pro_frame_num);

        ISP_DRV_ProcPrintf(ViPipe, s);

        osal_seq_printf(s, "-----------------------------------------------------------------------------\n");
        osal_seq_printf(s, "----------------------------------- ISP PROC END[%d] ------------------------\n", ViPipe);
        osal_seq_printf(s, "-----------------------------------------------------------------------------\n");
        osal_seq_printf(s, "\n\n");
    } while (++ViPipe < ISP_MAX_PIPE_NUM);

    return 0;
}
#endif

HI_S32 ISP_KernInit(void *p)
{
    HI_U32 ViPipe;

    for (ViPipe = 0; ViPipe < ISP_MAX_PIPE_NUM; ViPipe++) {
        osal_memset(&g_astIspDrvCtx[ViPipe].stDrvDbgInfo, 0, sizeof(ISP_DRV_DBG_INFO_S));
    }

    return HI_SUCCESS;
}

HI_VOID ISP_KernExit(void)
{
    HI_U32 ViPipe;

    for (ViPipe = 0; ViPipe < ISP_MAX_PIPE_NUM; ViPipe++) {
        if ((reg_vicap_base_va != HI_NULL) && (reg_ispfe_base_va[ViPipe] != HI_NULL)) {
            ISP_DRV_SetIntEnable(ViPipe, HI_FALSE);
        }
        osal_memset(&g_astIspDrvCtx[ViPipe].stDrvDbgInfo, 0, sizeof(ISP_DRV_DBG_INFO_S));
    }

    return;
}

static HI_U32 ISP_GetVerMagic(HI_VOID)
{
    return VERSION_MAGIC;
}

isp_export_func g_stIspExpFunc   = {
    .pfn_isp_register_bus_callback       = ISP_RegisterBusCallBack,
    .pfn_isp_register_piris_callback     = ISP_RegisterPirisCallBack,
    .pfn_isp_register_vi_bus_callback    = ISP_RegisterViBusCallBack,
    .pfn_isp_get_dcf_info                = ISP_GetDCFInfo,
    .pfn_isp_set_dcf_info                = ISP_SetDCFInfo,
    .pfn_isp_get_update_info             = ISP_GetIspUpdateInfo,
    .pfn_isp_set_update_info             = ISP_SetIspUpdateInfo,
    .pfn_isp_get_frame_info              = ISP_GetFrameInfo,
    .pfn_isp_set_frame_info              = ISP_SetFrameInfo,
    .pfn_isp_get_attach_info             = ISP_GetAttachInfo,
    .pfn_isp_get_color_gamut_info        = ISP_GetColorGamutInfo,
    .pfn_isp_get_dng_image_dynamic_info  = ISP_DRV_GetDngImageDynamicInfo,
    .pfn_isp_get_pro_ctrl                = ISP_GetProCtrl,
    .pfn_isp_set_snap_attr               = ISP_SetSnapAttr,
    .pfn_isp_set_pro_nr_param            = ISP_SetProNrParam,
    .pfn_isp_set_pro_shp_param           = ISP_SetProShpParam,
    .pfn_isp_get_pro_nr_param            = ISP_GetProNrParam,
    .pfn_isp_get_pro_shp_param           = ISP_GetProShpParam,
    .pfn_isp_save_snap_config            = ISP_SaveSnapConfig,
    .pfn_isp_set_pro_enable              = ISP_SetProEnable,
    .pfn_isp_drv_get_ready_be_buf        = ISP_DRV_GetReadyBeBuf,
    .pfn_isp_drv_put_free_be_buf         = ISP_DRV_PutFreeBeBuf,
    .pfn_isp_drv_hold_busy_be_buf        = ISP_DRV_HoldBusyBeBuf,
    .pfn_isp_drv_get_be_sync_para        = ISP_DRV_GetBeSyncPara,
    .pfn_isp_drv_be_end_int_proc         = ISP_DRV_BeEndIntProc,
    .pfn_isp_register_sync_task          = hi_isp_sync_task_register,
    .pfn_isp_unregister_sync_task        = hi_isp_sync_task_unregister,
    .pfn_isp_int_bottom_half             = ISP_IntBottomHalf,
    .pfn_isp_isr                         = ISP_ISR,
    .pfn_isp_get_pub_attr                = ISP_GetPubAttr,
    .pfn_isp_drv_get_rgbir_format        = ISP_DRV_GetRgbirFormat,
};

static umap_module s_stModule    = {
    .mod_id           = HI_ID_ISP,
    .mod_name         = "isp",

    .pfn_init         = ISP_KernInit,
    .pfn_exit         = ISP_KernExit,
    .pfn_ver_checker  = ISP_GetVerMagic,
    .export_funcs     = &g_stIspExpFunc,
    .data             = HI_NULL,
};

int ISP_ModInit(void)
{
    HI_U32  ViPipe;

#ifndef DISABLE_DEBUG_INFO
    osal_proc_entry_t *proc = HI_NULL;
#endif

    s_pstHiISPDevice = osal_createdev("isp_dev");
    s_pstHiISPDevice->fops = &stIspFileOp;
    s_pstHiISPDevice->osal_pmops = &stHiISPDrvOps;
    s_pstHiISPDevice->minor = UMAP_ISP_MINOR_BASE;

    if (osal_registerdevice(s_pstHiISPDevice) < 0) {
        HI_PRINT("Kernel: Could not register isp devices\n");
        return HI_FAILURE;
    }

#ifndef DISABLE_DEBUG_INFO
    proc = osal_create_proc_entry(PROC_ENTRY_ISP, HI_NULL);

    if (proc == HI_NULL) {
        HI_PRINT("Kernel: Register isp proc failed!\n");
        goto OUT2;
    }

    proc->read = ISP_ProcShow;
#endif

    if (cmpi_register_module(&s_stModule)) {
        goto OUT1;;
    }

    for (ViPipe = 0; ViPipe < ISP_MAX_PIPE_NUM; ViPipe++) {
        osal_spin_lock_init(&g_stIspLock[ViPipe]);
        osal_spin_lock_init(&g_stIspSyncLock[ViPipe]);
    }

    if (ISP_DRV_Init() != 0) {
        HI_PRINT("isp init failed\n");
        goto OUT1;
    }

#ifdef TEST_TIME
    ISP_Test_Init();
#endif
    HI_PRINT("ISP Mod init!\n");
    return HI_SUCCESS;

OUT1:
#ifndef DISABLE_DEBUG_INFO
    osal_remove_proc_entry(PROC_ENTRY_ISP, HI_NULL);
#endif
OUT2:
    osal_deregisterdevice(s_pstHiISPDevice);
    osal_destroydev(s_pstHiISPDevice);

    HI_PRINT("ISP Mod init failed!\n");
    return HI_FAILURE;
}

void ISP_ModExit(void)
{
    int i;

    ISP_DRV_Exit();

    for (i = 0; i < ISP_MAX_PIPE_NUM; i++) {
        osal_spin_lock_destory(&g_stIspLock[i]);
        osal_spin_lock_destory(&g_stIspSyncLock[i]);
    }

    cmpi_unregister_module(HI_ID_ISP);

#ifndef DISABLE_DEBUG_INFO
    osal_remove_proc_entry(PROC_ENTRY_ISP, HI_NULL);
#endif
    osal_deregisterdevice(s_pstHiISPDevice);
    osal_destroydev(s_pstHiISPDevice);

#ifdef TEST_TIME
    ISP_Test_Exit();
#endif

    HI_PRINT("ISP Mod Exit!\n");
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
