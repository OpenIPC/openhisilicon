/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "osal.h"
#include "common.h"
#include "comm_isp.h"
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
#include "i2c.h"
#include "spi.h"

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
GK_U32  g_PwmNumber[ISP_MAX_PIPE_NUM]  = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = 3 };
GK_U32  g_UpdatePos[ISP_MAX_PIPE_NUM]  = { 0 }; /* 0: frame start; 1: frame end */

/* The time(unit:ms) of interrupt timeout */
GK_U32  g_IntTimeout[ISP_MAX_PIPE_NUM] = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = 200 };

/* update isp statistic information per stat-intval frame, purpose to reduce CPU load */
GK_U32  g_StatIntvl[ISP_MAX_PIPE_NUM]  = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = 1 };

/* 0: close proc; n: write proc info's interval int num */
GK_U32  g_ProcParam[ISP_MAX_PIPE_NUM]  = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = 30 };
GK_U32  g_PortIntDelay[ISP_MAX_PIPE_NUM] = { 0 }; /* Port intertupt delay value */
GK_BOOL g_IntBothalf = GK_FALSE;  /* 1 to enable interrupt processing at bottom half */
GK_BOOL g_QuickStart = GK_FALSE;  /* 1 to enable interrupt processing at bottom half */
GK_BOOL g_bLongFrmIntEn = GK_FALSE; /* 1 : enable long frame  pipe interrupt*/
GK_BOOL g_UseBothalf = GK_FALSE;  /* 1 to use interrupt processing at bottom half */
GK_U32  g_IspExitTimeout = 2000;  /* The time(unit:ms) of exit be buffer timeout */
GK_U16  g_au16ResMaxWidth[ISP_MAX_PIPE_NUM]  = { ISP_RES_WIDTH_MAX };
GK_U16  g_au16ResMaxHeight[ISP_MAX_PIPE_NUM] = { ISP_RES_HEIGHT_MAX };

/* ldci temporal filter enable */
GK_BOOL  g_LdciTprFltEn[ISP_MAX_PIPE_NUM]  = { [0 ...(ISP_MAX_PIPE_NUM - 1)] = GK_FALSE };

// #define TEST_TIME

#ifdef TEST_TIME
GK_U32  g_test_phyaddr;
GK_U16 *g_test_pviraddr;
struct osal_timeval time1;
struct osal_timeval time2;
#endif



GK_VOID ISP_DRV_SlavePipeIntEnable(VI_PIPE ViPipe, GK_BOOL bEn)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    GK_U8 k;
    GK_U8 u8ChnNumMax;
    VI_PIPE ViPipeBind;
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    if(g_bLongFrmIntEn == GK_FALSE)
    {
        return ;
    }
    if(IS_FS_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode) == GK_FALSE)
    {
        return;
    }

    u8ChnNumMax = CLIP3(pstDrvCtx->stWDRAttr.stDevBindPipe.u32Num, 1, ISP_WDR_CHN_MAX);
    if(bEn)
    {
        for (k = 0; k < u8ChnNumMax; k++)
        {
            ViPipeBind = pstDrvCtx->stWDRAttr.stDevBindPipe.PipeId[k];
            if(ViPipeBind == ViPipe)
            {
                continue;
            }
            if (((ViPipeBind) < 0) || ((ViPipeBind) >= ISP_MAX_PIPE_NUM))
            {
                continue;
            }
            IO_RW_FE_ADDRESS(ViPipeBind, ISP_INT_FE) = 0xFF;
            IO_RW_FE_ADDRESS(ViPipeBind, ISP_INT_FE_MASK) |= ISP_INT_FE_FSTART;
            IO_RW_FE_ADDRESS(ViPipeBind, ISP_INT_FE_MASK) |= ISP_INT_FE_FEND;
        }
    }
    else
    {
        for (k = 0; k < u8ChnNumMax; k++)
        {
            ViPipeBind = pstDrvCtx->stWDRAttr.stDevBindPipe.PipeId[k];
            if(ViPipeBind == ViPipe)
            {
                continue;
            }
            if (((ViPipeBind) < 0) || ((ViPipeBind) >= ISP_MAX_PIPE_NUM))
            {
                continue;
            }
            IO_RW_FE_ADDRESS(ViPipeBind, ISP_INT_FE) = 0xFF;
            IO_RW_FE_ADDRESS(ViPipeBind, ISP_INT_FE_MASK) &= ~ISP_INT_FE_FSTART;
            IO_RW_FE_ADDRESS(ViPipeBind, ISP_INT_FE_MASK) &= ~ISP_INT_FE_FEND;
        }
    }
}

/****************************************************************************
 * INTERNAL FUNCTION DECLARATION                                            *
 ****************************************************************************/
GK_S32 ISP_DRV_SetIntEnable(VI_PIPE ViPipe, GK_BOOL bEn)
{
    GK_S32 ViDev;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    ViDev = pstDrvCtx->stWDRAttr.ViDev;

    ISP_CHECK_VIR_PIPE(ViPipe);

    if (bEn) {
        if (IS_FULL_WDR_MODE(pstDrvCtx->stWDRCfg.u8WDRMode) ||
            IS_HALF_WDR_MODE(pstDrvCtx->stWDRCfg.u8WDRMode)) {
            IO_RW_PT_ADDRESS(VI_PT_BASE(ViDev) + VI_PT_INT_MASK) |= VI_PT_INT_FSTART;
            IO_RW_PT_ADDRESS(VI_PT_BASE(ViDev) + VI_PT_FSTART_DLY) = g_PortIntDelay[ViPipe];
        }
        IO_RW_PT_ADDRESS(VI_PT_BASE(ViDev) + VI_PT_INT_MASK) |= VI_PT_INT_ERR;

        IO_RW_FE_ADDRESS(ViPipe, ISP_INT_FE) = 0xFF;
        IO_RW_FE_ADDRESS(ViPipe, ISP_INT_FE_MASK) |= ISP_INT_FE_FSTART;
        IO_RW_FE_ADDRESS(ViPipe, ISP_INT_FE_MASK) |= ISP_INT_FE_FEND;
    } else {
        if (IS_FULL_WDR_MODE(pstDrvCtx->stWDRCfg.u8WDRMode) ||
            IS_HALF_WDR_MODE(pstDrvCtx->stWDRCfg.u8WDRMode)) {
            IO_RW_PT_ADDRESS(VI_PT_BASE(ViDev) + VI_PT_INT_MASK) &= (~VI_PT_INT_FSTART);
            IO_RW_PT_ADDRESS(VI_PT_BASE(ViDev) + VI_PT_FSTART_DLY) = 0;
        }
        IO_RW_PT_ADDRESS(VI_PT_BASE(ViDev) + VI_PT_INT) = 0xF;
        IO_RW_PT_ADDRESS(VI_PT_BASE(ViDev) + VI_PT_INT_MASK) &= (~VI_PT_INT_ERR);

        IO_RW_FE_ADDRESS(ViPipe, ISP_INT_FE_MASK) &= (~ISP_INT_FE_FSTART);
        IO_RW_FE_ADDRESS(ViPipe, ISP_INT_FE_MASK) &= (~ISP_INT_FE_FEND);
    }
    ISP_DRV_SlavePipeIntEnable(ViPipe,bEn);
    return GK_SUCCESS;
}

GK_S32 ISP_DRV_WaitConditionCallback(const GK_VOID *pParam)
{
    GK_BOOL bCondition;

    bCondition = *(GK_BOOL *)pParam;

    return (bCondition == GK_TRUE);
}

GK_S32 ISP_DRV_WaitExitCallback(const GK_VOID *pParam)
{
    GK_S32 bCondition;

    bCondition = *(GK_S32 *)pParam;

    return (bCondition == 0);
}

GK_S32 ISP_GetFrameEdge(VI_PIPE ViPipe, GK_U32 *pu32Status)
{
    unsigned long u32Flags;
    GK_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pu32Status);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    s32Ret = osal_wait_event_timeout_interruptible(&pstDrvCtx->stIspWait, ISP_DRV_WaitConditionCallback, \
                                                   &pstDrvCtx->bEdge, g_IntTimeout[ViPipe]);

    if (s32Ret <= 0) {
        *pu32Status = 0;
        ISP_TRACE(MODULE_DBG_WARN, "Get Interrupt timeout failed!\n");
        return GK_FAILURE;
    }

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstDrvCtx->bEdge = GK_FALSE;
    *pu32Status = pstDrvCtx->u32Status;
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

static int ISP_GetVdStartTimeOut(VI_PIPE ViPipe, GK_U32 u32MilliSec, GK_U32 *pu32status)
{
    unsigned long u32Flags;
    GK_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (osal_down_interruptible(&pstDrvCtx->stIspSem)) {
        return -ERESTARTSYS;
    }

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstDrvCtx->bVdStart = GK_FALSE;
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    if (u32MilliSec != 0) {
        s32Ret = osal_wait_event_timeout_interruptible(&pstDrvCtx->stIspWaitVdStart, ISP_DRV_WaitConditionCallback, \
                                                       &pstDrvCtx->bVdStart, (u32MilliSec));

        if (s32Ret <= 0) {
            osal_up(&pstDrvCtx->stIspSem);
            return GK_FAILURE;
        }
    } else {
        s32Ret = osal_wait_event_interruptible(&pstDrvCtx->stIspWaitVdStart, ISP_DRV_WaitConditionCallback, \
                                               &pstDrvCtx->bVdStart);

        if (s32Ret) {
            osal_up(&pstDrvCtx->stIspSem);
            return GK_FAILURE;
        }
    }

    *pu32status = pstDrvCtx->u32Status;

    osal_up(&pstDrvCtx->stIspSem);

    return GK_SUCCESS;
}

static int ISP_GetVdEndTimeOut(VI_PIPE ViPipe, GK_U32 u32MilliSec, GK_U32 *pu32status)
{
    unsigned long u32Flags;
    GK_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (osal_down_interruptible(&pstDrvCtx->stIspSemVd)) {
        return -ERESTARTSYS;
    }

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstDrvCtx->bVdEnd = GK_FALSE;
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    if (u32MilliSec != 0) {
        s32Ret = osal_wait_event_timeout_interruptible(&pstDrvCtx->stIspWaitVdEnd, ISP_DRV_WaitConditionCallback, \
                                                       &pstDrvCtx->bVdEnd, (u32MilliSec));

        if (s32Ret <= 0) {
            osal_up(&pstDrvCtx->stIspSemVd);
            return GK_FAILURE;
        }
    } else {
        s32Ret = osal_wait_event_interruptible(&pstDrvCtx->stIspWaitVdEnd, ISP_DRV_WaitConditionCallback, \
                                               &pstDrvCtx->bVdEnd);

        if (s32Ret) {
            osal_up(&pstDrvCtx->stIspSemVd);
            return GK_FAILURE;
        }
    }

    *pu32status = pstDrvCtx->u32Status;

    osal_up(&pstDrvCtx->stIspSemVd);

    return GK_SUCCESS;
}

static int ISP_GetVdBeEndTimeOut(VI_PIPE ViPipe, GK_U32 u32MilliSec, GK_U32 *pu32status)
{
    unsigned long u32Flags;
    GK_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (osal_down_interruptible(&pstDrvCtx->stIspSemBeVd)) {
        return -ERESTARTSYS;
    }

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstDrvCtx->bVdBeEnd = GK_FALSE;
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    if (u32MilliSec != 0) {
        s32Ret = osal_wait_event_timeout_interruptible(&pstDrvCtx->stIspWaitVdBeEnd, ISP_DRV_WaitConditionCallback, \
                                                       &pstDrvCtx->bVdBeEnd, (u32MilliSec));

        if (s32Ret <= 0) {
            osal_up(&pstDrvCtx->stIspSemBeVd);
            return GK_FAILURE;
        }
    } else {
        s32Ret = osal_wait_event_interruptible(&pstDrvCtx->stIspWaitVdBeEnd, ISP_DRV_WaitConditionCallback, \
                                               &pstDrvCtx->bVdBeEnd);

        if (s32Ret) {
            osal_up(&pstDrvCtx->stIspSemBeVd);
            return GK_FAILURE;
        }
    }

    *pu32status = 1;

    osal_up(&pstDrvCtx->stIspSemBeVd);

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_WorkModeInit(VI_PIPE ViPipe, ISP_BLOCK_ATTR_S  *pstBlkAttr)
{
    GK_S32  s32Ret = GK_SUCCESS;
    unsigned long u32Flags;
    ISP_RUNNING_MODE_E enIspRunningMode = ISP_MODE_RUNNING_OFFLINE;
    VI_PIPE_SPLIT_ATTR_S stPipeSplitAttr;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    FRAME_INTERRUPT_ATTR_S stFrameIntAttr = { 0 };

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstBlkAttr);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    pstDrvCtx->stWorkMode.u8PreBlockNum = pstDrvCtx->stWorkMode.u8BlockNum;

    osal_memset(&stPipeSplitAttr, 0, sizeof(VI_PIPE_SPLIT_ATTR_S));

    if (CKFN_VI_GetSplitAttr()) {
        s32Ret = CALL_VI_GetSplitAttr(ViPipe, &stPipeSplitAttr);

        if (s32Ret != GK_SUCCESS) {
            ISP_TRACE(MODULE_DBG_ERR, "Pipe:%d CALL_VI_GetSplitAttr failed 0x%x!\n", ViPipe, s32Ret);
            return s32Ret;
        }
    }

    ISP_CHECK_BLOCK_NUM(stPipeSplitAttr.s32SplitNum);

    if ((stPipeSplitAttr.enMode == VI_PARALLEL_VPSS_OFFLINE) ||
        (stPipeSplitAttr.enMode == VI_PARALLEL_VPSS_PARALLEL)) {
        enIspRunningMode = ISP_MODE_RUNNING_SIDEBYSIDE;
    } else if ((stPipeSplitAttr.enMode == VI_ONLINE_VPSS_OFFLINE) ||
               (stPipeSplitAttr.enMode == VI_ONLINE_VPSS_ONLINE)) {
        enIspRunningMode = ISP_MODE_RUNNING_ONLINE;
    } else if ((stPipeSplitAttr.enMode == VI_OFFLINE_VPSS_OFFLINE) ||
               (stPipeSplitAttr.enMode == VI_OFFLINE_VPSS_ONLINE)) {
        if (stPipeSplitAttr.s32SplitNum == 1) {
            enIspRunningMode = ISP_MODE_RUNNING_OFFLINE;
        } else {
            enIspRunningMode = ISP_MODE_RUNNING_STRIPING;
        }
    } else {
        ISP_TRACE(MODULE_DBG_ERR, "Pipe:%d Not support Mode: %d!\n", ViPipe, stPipeSplitAttr.enMode);
        return GK_FAILURE;
    }

    if ((stPipeSplitAttr.enPixFmt == PIXEL_FORMAT_YUV_SEMIPLANAR_420) ||
        (stPipeSplitAttr.enPixFmt == PIXEL_FORMAT_YVU_SEMIPLANAR_420) ||
        (stPipeSplitAttr.enPixFmt == PIXEL_FORMAT_YUV_SEMIPLANAR_422) ||
        (stPipeSplitAttr.enPixFmt == PIXEL_FORMAT_YVU_SEMIPLANAR_422) ||
        (stPipeSplitAttr.enPixFmt == PIXEL_FORMAT_YUV_SEMIPLANAR_444) ||
        (stPipeSplitAttr.enPixFmt == PIXEL_FORMAT_YVU_SEMIPLANAR_444)) {
        pstDrvCtx->bYuvMode = GK_TRUE;
        pstDrvCtx->stWorkMode.enIspDataInputMode = ISP_MODE_BT1120_YUV;
    } else {
        pstDrvCtx->bYuvMode = GK_FALSE;
        pstDrvCtx->stWorkMode.enIspDataInputMode = ISP_MODE_RAW;
    }
    if (CKFN_VI_GetViFrameInterruptAttr()) {
        s32Ret = CALL_VI_GetViFrameInterruptAttr(ViPipe, &stFrameIntAttr);

        if (s32Ret != GK_SUCCESS) {
            ISP_TRACE(MODULE_DBG_ERR, "Pipe:%d Get Vi Int Attr err 0x%x!\n", ViPipe, s32Ret);
            return s32Ret;
        }
    }

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    pstDrvCtx->stWorkMode.u8BlockNum       = stPipeSplitAttr.s32SplitNum;
    pstDrvCtx->stWorkMode.enIspRunningMode = enIspRunningMode;
    pstDrvCtx->stWorkMode.u32OverLap       = stPipeSplitAttr.u32OverLap;

    pstBlkAttr->u8BlockNum       = stPipeSplitAttr.s32SplitNum;
    pstBlkAttr->u32OverLap       = stPipeSplitAttr.u32OverLap;
    pstBlkAttr->enIspRunningMode = enIspRunningMode;

    pstDrvCtx->stWorkMode.stFrameRect.u32Width  = stPipeSplitAttr.astWchOutRect.u32Width;
    pstDrvCtx->stWorkMode.stFrameRect.u32Height = stPipeSplitAttr.astWchOutRect.u32Height;

    osal_memcpy(&pstBlkAttr->stFrameRect, &pstDrvCtx->stWorkMode.stFrameRect, sizeof(SIZE_S));
    osal_memcpy(pstDrvCtx->stWorkMode.astBlockRect, stPipeSplitAttr.astRect, sizeof(RECT_S) * ISP_STRIPING_MAX_NUM);
    osal_memcpy(pstBlkAttr->astBlockRect, stPipeSplitAttr.astRect, sizeof(RECT_S) * ISP_STRIPING_MAX_NUM);
    pstDrvCtx->stFrameIntAttr = stFrameIntAttr;

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_WorkModeExit(VI_PIPE ViPipe)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_EXIT_STATE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    pstDrvCtx->stWorkMode.enIspRunningMode = ISP_MODE_RUNNING_OFFLINE;

    return GK_SUCCESS;
}

GK_U8 ISP_DRV_GetBlockNum(VI_PIPE ViPipe)
{
    return g_astIspDrvCtx[ViPipe].stWorkMode.u8BlockNum;
}

GK_S8 ISP_DRV_GetBlockId(VI_PIPE ViPipe, ISP_RUNNING_MODE_E enRunningMode)
{
    GK_S8 s8BlockId = 0;

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
                    return GK_FAILURE;
            }
            break;
    }

    return s8BlockId;
}

GK_S32 ISP_DRV_GetBeRegsAttr(VI_PIPE ViPipe, S_ISPBE_REGS_TYPE *apstBeReg[], ISP_BE_REGS_ATTR_S *pstBlkAttr)
{
    GK_U8  k = 0;
    GK_S8  s8BlockId, u8BlkDev, u8BlkNum;
    ISP_RUNNING_MODE_E enIspRunningMode;
    ISP_BE_WO_REG_CFG_S *pstIspBeRegCfg = GK_NULL;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstBlkAttr);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    enIspRunningMode = pstDrvCtx->stWorkMode.enIspRunningMode;
    u8BlkNum = ISP_DRV_GetBlockNum(ViPipe);
    u8BlkNum = DIV_0_TO_1(u8BlkNum);
    s8BlockId = ISP_DRV_GetBlockId(ViPipe, enIspRunningMode);
    if (s8BlockId == -1) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] Online Mode Pipe Err!\n", ViPipe);
        return GK_FAILURE;
    }

    u8BlkDev = (GK_U8)s8BlockId;
    pstBlkAttr->u8BlockDev = u8BlkDev;
    pstBlkAttr->u8BlockNum = u8BlkNum;

    switch (enIspRunningMode) {
        case ISP_MODE_RUNNING_ONLINE:
            ISP_DRV_BEREG_CTX(u8BlkDev, apstBeReg[u8BlkDev]);
            break;
        case ISP_MODE_RUNNING_OFFLINE:
            ISP_CHECK_BEBUF_INIT(ViPipe);
            if (pstDrvCtx->pstUseNode == GK_NULL) {
                ISP_TRACE(MODULE_DBG_WARN, "ISP[%d] UseNode is Null!\n", ViPipe);
                return GK_FAILURE;
            }
            pstIspBeRegCfg = (ISP_BE_WO_REG_CFG_S *)pstDrvCtx->pstUseNode->stBeCfgBuf.pVirAddr;
            apstBeReg[u8BlkDev] = &pstIspBeRegCfg->stBeRegCfg[u8BlkDev];
            break;
        case ISP_MODE_RUNNING_SIDEBYSIDE:
            for (k = 0; k < ISP_MAX_BE_NUM; k++) {
                ISP_DRV_BEREG_CTX(k, apstBeReg[k]);
            }
            break;
        case ISP_MODE_RUNNING_STRIPING:
            ISP_CHECK_BEBUF_INIT(ViPipe);
            if (pstDrvCtx->pstUseNode == GK_NULL) {
                ISP_TRACE(MODULE_DBG_WARN, "ISP[%d] UseNode is Null!\n", ViPipe);
                return GK_FAILURE;
            }
            pstIspBeRegCfg = (ISP_BE_WO_REG_CFG_S *)pstDrvCtx->pstUseNode->stBeCfgBuf.pVirAddr;
            for (k = 0; k < ISP_STRIPING_MAX_NUM; k++) {
                apstBeReg[k] = &pstIspBeRegCfg->stBeRegCfg[k];
            }
            break;
        default:
            ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] Running Mode Err!\n", ViPipe);
            return GK_FAILURE;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_ChnSelectWrite(VI_PIPE ViPipe, GK_U32 u32ChannelSel)
{
    GK_U8   k;
    GK_U8   u8BlkDev, u8BlkNum;
    GK_U32  au32ChnSwitch[5] = { 0 };
    GK_U32  s32Ret;
    ISP_BE_REGS_ATTR_S stIspBeRegsAttr = { 0 };
    S_ISPBE_REGS_TYPE *apstBeReg[ISP_STRIPING_MAX_NUM] = { GK_NULL };
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_PIPE(ViPipe);

    s32Ret = ISP_DRV_GetBeRegsAttr(ViPipe, apstBeReg, &stIspBeRegsAttr);

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] Get BeRegs Attr Err!\n", ViPipe);
        return GK_FAILURE;
    }

    if (pstDrvCtx->bYuvMode == GK_TRUE) {
        au32ChnSwitch[4] = 1;
    } else {
        au32ChnSwitch[4] = 0;
    }

    u8BlkDev = stIspBeRegsAttr.u8BlockDev;
    u8BlkNum = stIspBeRegsAttr.u8BlockNum;

    switch (u32ChannelSel & 0x3) {
        case 0:
            au32ChnSwitch[0] = pstDrvCtx->astChnSelAttr[0].au32WdrChnSel[0];
            au32ChnSwitch[1] = pstDrvCtx->astChnSelAttr[0].au32WdrChnSel[1];
            au32ChnSwitch[2] = pstDrvCtx->astChnSelAttr[0].au32WdrChnSel[2];
            au32ChnSwitch[3] = pstDrvCtx->astChnSelAttr[0].au32WdrChnSel[3];
            break;

        case 1:
            au32ChnSwitch[1] = pstDrvCtx->astChnSelAttr[0].au32WdrChnSel[0];
            au32ChnSwitch[0] = pstDrvCtx->astChnSelAttr[0].au32WdrChnSel[1];
            au32ChnSwitch[2] = pstDrvCtx->astChnSelAttr[0].au32WdrChnSel[2];
            au32ChnSwitch[3] = pstDrvCtx->astChnSelAttr[0].au32WdrChnSel[3];
            break;

        case 2:
            au32ChnSwitch[2] = pstDrvCtx->astChnSelAttr[0].au32WdrChnSel[0];
            au32ChnSwitch[1] = pstDrvCtx->astChnSelAttr[0].au32WdrChnSel[1];
            au32ChnSwitch[0] = pstDrvCtx->astChnSelAttr[0].au32WdrChnSel[2];
            au32ChnSwitch[3] = pstDrvCtx->astChnSelAttr[0].au32WdrChnSel[3];
            break;

        case 3:
            au32ChnSwitch[3] = pstDrvCtx->astChnSelAttr[0].au32WdrChnSel[0];
            au32ChnSwitch[2] = pstDrvCtx->astChnSelAttr[0].au32WdrChnSel[1];
            au32ChnSwitch[1] = pstDrvCtx->astChnSelAttr[0].au32WdrChnSel[2];
            au32ChnSwitch[0] = pstDrvCtx->astChnSelAttr[0].au32WdrChnSel[3];
            break;
    }

    if (IS_FS_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)) {
        for (k = 0; k < u8BlkNum; k++) {
            ISP_DRV_SetInputSel(apstBeReg[k + u8BlkDev], &au32ChnSwitch[0]);
        }
    }

    return GK_SUCCESS;
}

/* ISP FE read sta */
GK_S32 ISP_DRV_FE_StatisticsRead(VI_PIPE ViPipe, ISP_STAT_INFO_S *pstStatInfo)
{
    GK_U8          u8ChnNumMax;
    GK_U32         k;
    VI_PIPE        ViPipeBind;
    ISP_STAT_KEY_U unStatkey;
    ISP_DRV_CTX_S     *pstDrvCtx = GK_NULL;
    ISP_STAT_S        *pstStat   = GK_NULL;
    S_ISPFE_REGS_TYPE *pstFeReg  = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_VIR_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstStatInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    if (pstDrvCtx->stWDRAttr.bMastPipe == GK_FALSE) {
        return GK_SUCCESS;
    }

    pstStat = (ISP_STAT_S *)pstStatInfo->pVirtAddr;
    if (pstStat == GK_NULL) {
        return GK_FAILURE;
    }

    unStatkey.u64Key = pstStatInfo->unKey.bit32IsrAccess;

    u8ChnNumMax = CLIP3(pstDrvCtx->stWDRAttr.stDevBindPipe.u32Num, 1, ISP_WDR_CHN_MAX);

    if (IS_HALF_WDR_MODE(pstDrvCtx->stWDRAttr.enWDRMode)) {
        for (k = 0; k < u8ChnNumMax; k++) {
            /* get side statistics */
            ViPipeBind = pstDrvCtx->stWDRAttr.stDevBindPipe.PipeId[u8ChnNumMax - 1 - k];
            ISP_CHECK_PIPE(ViPipeBind);
            ISP_DRV_FEREG_CTX(ViPipeBind, pstFeReg);
            ISP_DRV_FE_APB_StatisticsRead(pstStat, pstFeReg, ViPipeBind, k, unStatkey);
            ISP_DRV_FE_STT_StatisticsRead(pstStat, pstFeReg, pstDrvCtx, ViPipeBind, k, unStatkey);
        }
    } else {
        for (k = 0; k < u8ChnNumMax; k++) {
            /* get side statistics */
            ViPipeBind = pstDrvCtx->stWDRAttr.stDevBindPipe.PipeId[k];
            ISP_CHECK_PIPE(ViPipeBind);
            ISP_DRV_FEREG_CTX(ViPipeBind, pstFeReg);
            ISP_DRV_FE_APB_StatisticsRead(pstStat, pstFeReg, ViPipeBind, k, unStatkey);
            ISP_DRV_FE_STT_StatisticsRead(pstStat, pstFeReg, pstDrvCtx, ViPipeBind, k, unStatkey);
        }
    }

    return GK_SUCCESS;
}

/* ISP BE read sta from FHY, online mode */
GK_S32 ISP_DRV_BeStatisticsRead(VI_PIPE ViPipe, ISP_STAT_INFO_S *pstStatInfo)
{
    GK_S32 s32Ret;
    GK_U8  u8BlkDev, u8BlkNum;
    ISP_STAT_S *pstStat = GK_NULL;

    ISP_STAT_KEY_U unStatkey;
    ISP_BE_REGS_ATTR_S stIspBeRegsAttr = { 0 };
    S_ISPBE_REGS_TYPE *apstBeReg[ISP_STRIPING_MAX_NUM] = { GK_NULL };

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstStatInfo);

    s32Ret = ISP_DRV_GetBeRegsAttr(ViPipe, apstBeReg, &stIspBeRegsAttr);
    if (s32Ret != GK_SUCCESS) {
        return s32Ret;
    }

    u8BlkDev = stIspBeRegsAttr.u8BlockDev;
    u8BlkNum = stIspBeRegsAttr.u8BlockNum;

    pstStat = (ISP_STAT_S *)pstStatInfo->pVirtAddr;

    if (pstStat == GK_NULL) {
        return GK_FAILURE;
    }

    unStatkey.u64Key = pstStatInfo->unKey.bit32IsrAccess;

    pstStat->bBEUpdate = GK_TRUE;

    ISP_DRV_BE_APB_StatisticsRead(pstStat, apstBeReg, u8BlkNum, u8BlkDev, unStatkey);
    ISP_DRV_BE_STT_StatisticsRead(ViPipe, pstStat, apstBeReg, u8BlkNum, u8BlkDev, unStatkey);

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_BeOfflineStitchStatisticsRead(VI_PIPE ViPipe, ISP_STAT_INFO_S *pstStatInfo)
{
    GK_S32 k = 0;
    GK_S32 s32Ret;
    GK_U8 u8BlkDev;

    ISP_STAT_KEY_U unStatkey;
    ISP_STAT_S *pstStat = GK_NULL;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    ISP_STITCH_SYNC_BE_STATS_S stBeStitch;
    ISP_BE_REGS_ATTR_S  stIspBeRegsAttr = { 0 };
    S_ISPBE_REGS_TYPE   *apstBeReg[ISP_STRIPING_MAX_NUM] = { GK_NULL };

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstStatInfo);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    unStatkey.u64Key = pstStatInfo->unKey.bit32IsrAccess;
    s32Ret = ISP_DRV_GetBeRegsAttr(ViPipe, apstBeReg, &stIspBeRegsAttr);
    if (s32Ret != GK_SUCCESS) {
        return s32Ret;
    }
    u8BlkDev = stIspBeRegsAttr.u8BlockDev;

    if ((pstDrvCtx->stStitchAttr.bMainPipe != GK_TRUE) && (pstDrvCtx->stStitchAttr.bStitchEnable != GK_TRUE)) {
        return  GK_SUCCESS;
    }

    pstStat = (ISP_STAT_S *)pstStatInfo->pVirtAddr;

    if (pstStat == GK_NULL) {
        return GK_FAILURE;
    }

    for (k = 0; k < pstDrvCtx->stStitchAttr.u8StitchPipeNum; k++) {
        stBeStitch.pstSyncBeStt[k] = \
            (S_ISP_STITCH_STT_REGS_TYPE *)pstDrvCtx->stBeOffStitSttAttr.astBeStitchBuf[k].pVirAddr;

        if (stBeStitch.pstSyncBeStt[k] == GK_NULL) {
            ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] apstBeStt is NULL point\n", ViPipe);
            return GK_FAILURE;
        }
    }

    /* VI   CallBack Function */
    if (CKFN_VI_GetStitchSyncBeSttInfo()) {
        s32Ret = CALL_VI_GetStitchSyncBeSttInfo(ViPipe,  &stBeStitch);

        if (s32Ret != GK_SUCCESS) {
            pstStat->bBEUpdate = GK_FALSE;
            ISP_TRACE(MODULE_DBG_WARN, "Pipe[%d] CALL_VI_GetStitchSyncBeSttInfo err 0x%x\n", ViPipe, s32Ret);
            return GK_FAILURE;
        }

        pstStat->bBEUpdate = GK_TRUE;
    } else {
        pstStat->bBEUpdate = GK_FALSE;
        ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] CALL_VI_GetStitchSyncBeSttInfo is NULL\n", ViPipe);
        return GK_FAILURE;
    }

    if (pstStat->bBEUpdate == GK_FALSE) {
        pstDrvCtx->stDrvDbgInfo.u32IspBeStaLost++;
    }

    if (unStatkey.bit1BEAeStiGloStat) {
        s32Ret = ISP_DRV_BeOfflineAEStitchGlobalStatisticsRead(pstStat, pstDrvCtx);
    }

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] ISP_DRV_BeOfflineAEStitchGlobalStatisticsRead err 0x%x\n", ViPipe, s32Ret);
        return GK_FAILURE;
    }

    if (unStatkey.bit1BEAeStiLocStat) {
        s32Ret = ISP_DRV_BeOfflineAEStitchLocalStatisticsRead(pstStat, pstDrvCtx);
    }

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] ISP_DRV_BeOfflineAEStitchLocalStatisticsRead err 0x%x\n", ViPipe, s32Ret);
        return GK_FAILURE;
    }

    /* BE AWB statistics */
    s32Ret = ISP_DRV_BeOfflineAWBStitchStatisticsRead(pstStat, apstBeReg, pstDrvCtx, u8BlkDev, unStatkey);

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] ISP_DRV_BeOfflineAWBStitchStatisticsRead err 0x%x\n", ViPipe, s32Ret);
        return GK_FAILURE;
    }

    return GK_SUCCESS;
}

/* ISP BE read sta from VI, offline mode */
GK_S32 ISP_DRV_BeOfflineStatisticsRead(VI_PIPE ViPipe, ISP_STAT_INFO_S *pstStatInfo)
{
    GK_S32 k;
    GK_S32 s32Ret;
    GK_U8  u8BlkNum, u8BlkDev;
    GK_S8  s8BlockId = 0;
    ISP_STAT_KEY_U unStatkey;
    ISP_STAT_S *pstStat = GK_NULL;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    S_ISP_STT_REGS_TYPE *apstBeStt[ISP_STRIPING_MAX_NUM] = { [0 ...(ISP_STRIPING_MAX_NUM - 1)] = GK_NULL };
    ISP_RUNNING_MODE_E enIspRunningMode;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstStatInfo);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    u8BlkNum = ISP_DRV_GetBlockNum(ViPipe);
    u8BlkNum = DIV_0_TO_1(u8BlkNum);

    enIspRunningMode = pstDrvCtx->stWorkMode.enIspRunningMode;

    s8BlockId = ISP_DRV_GetBlockId(ViPipe, enIspRunningMode);

    if (s8BlockId == -1) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] Online Mode Pipe Err!\n", ViPipe);
        return GK_FAILURE;
    }

    u8BlkDev = (GK_U8)s8BlockId;

    pstStat = (ISP_STAT_S *)pstStatInfo->pVirtAddr;

    if (pstStat == GK_NULL) {
        return GK_FAILURE;
    }

    for (k = 0; k < u8BlkNum; k++) {
        apstBeStt[k] = (S_ISP_STT_REGS_TYPE *)pstDrvCtx->stBeOffSttAttr.astBeSttBuf[k].pVirAddr;
        if (apstBeStt[k] == GK_NULL) {
            ISP_TRACE(MODULE_DBG_ERR, "apstBeStt is NULL point\n");
            return GK_FAILURE;
        }

        if (CKFN_VI_GetBeSttInfo()) {
            s32Ret = CALL_VI_GetBeSttInfo(ViPipe, k, apstBeStt[k]);
            if (s32Ret != GK_SUCCESS) {
                pstStat->bBEUpdate = GK_FALSE;
                ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] CALL_VI_GetBeSttInfo err 0x%x\n", ViPipe, s32Ret);
                return GK_FAILURE;
            }

            pstStat->bBEUpdate = GK_TRUE;
        } else {
            pstStat->bBEUpdate = GK_FALSE;
            ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] VI_GetBeSttInfo is NULL\n", ViPipe);
            return GK_FAILURE;
        }
    }

    if (pstStat->bBEUpdate == GK_FALSE) {
        pstDrvCtx->stDrvDbgInfo.u32IspBeStaLost++;
    }

    unStatkey.u64Key = pstStatInfo->unKey.bit32IsrAccess;
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

    if (g_LdciTprFltEn[ViPipe] == GK_TRUE) {
        ISP_DRV_LDCI_OfflineStatisticsRead(ViPipe, pstStat);
    }

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_StatisticsRead(VI_PIPE ViPipe, ISP_STAT_INFO_S *pstStatInfo)
{
    GK_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstStatInfo);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    /* online snap, AE and AWB params set by the preview pipe.
      In order to get picture as fast as, dehaze don't used. */
    if (IS_ONLINE_MODE(pstDrvCtx->stWorkMode.enIspRunningMode) || \
        IS_SIDEBYSIDE_MODE(pstDrvCtx->stWorkMode.enIspRunningMode)) {
        if ((pstDrvCtx->stSnapAttr.s32PicturePipeId == ViPipe) &&
            (pstDrvCtx->stSnapAttr.s32PicturePipeId != pstDrvCtx->stSnapAttr.s32PreviewPipeId)) {
            return GK_SUCCESS;
        }
    }

    if (pstDrvCtx->stStitchAttr.bStitchEnable == GK_TRUE) {
        if (pstDrvCtx->stStitchAttr.bMainPipe == GK_TRUE) {
            ISP_DRV_FE_StitchStatisticsRead(ViPipe, pstStatInfo);
        }

        ISP_DRV_FE_StitchNonStatisticsRead(ViPipe, pstStatInfo);
    } else {
        s32Ret = ISP_DRV_FE_StatisticsRead(ViPipe, pstStatInfo);

        if (s32Ret != GK_SUCCESS) {
            ISP_TRACE(MODULE_DBG_ERR, "ISP_DRV_FE_StatisticsRead failed!\n");
            return GK_FAILURE;
        }
    }

    if (IS_ONLINE_MODE(pstDrvCtx->stWorkMode.enIspRunningMode) || \
        IS_SIDEBYSIDE_MODE(pstDrvCtx->stWorkMode.enIspRunningMode)) {
        // BE statistics for online
        s32Ret = ISP_DRV_BeStatisticsRead(ViPipe, pstStatInfo);
        if (s32Ret != GK_SUCCESS) {
            ISP_TRACE(MODULE_DBG_ERR, "ISP_DRV_BeStatisticsRead failed!\n");
            return GK_FAILURE;
        }

        if ((pstDrvCtx->stStitchAttr.bMainPipe == GK_TRUE) && (pstDrvCtx->stStitchAttr.bStitchEnable == GK_TRUE)) {
            ISP_DRV_BE_StitchStatisticsRead(ViPipe, pstStatInfo);
        }
    } else if (IS_OFFLINE_MODE(pstDrvCtx->stWorkMode.enIspRunningMode) || \
               IS_STRIPING_MODE(pstDrvCtx->stWorkMode.enIspRunningMode)) {
        // BE statistics for offline
        s32Ret = ISP_DRV_BeOfflineStatisticsRead(ViPipe, pstStatInfo);
        if (s32Ret) {
            ISP_TRACE(MODULE_DBG_ERR, "ISP_DRV_BeOfflineStatisticsRead failed!\n");
            return GK_FAILURE;
        }

        if ((pstDrvCtx->stStitchAttr.bMainPipe == GK_TRUE) && (pstDrvCtx->stStitchAttr.bStitchEnable == GK_TRUE)) {
            ISP_DRV_BeOfflineStitchStatisticsRead(ViPipe, pstStatInfo);
        }
    } else {
        ISP_TRACE(MODULE_DBG_ERR, "enIspOnlineMode err 0x%x!\n", pstDrvCtx->stWorkMode.enIspRunningMode);
        return GK_FAILURE;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_StatBufInit(VI_PIPE ViPipe, GK_U64 *pu64PhyAddr)
{
    GK_S32 s32Ret, i;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    GK_U64 u64PhyAddr, u64Size;
    GK_U8  *pu8VirAddr;
    GK_CHAR acName[MAX_MMZ_NAMELEN] = {0};
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pu64PhyAddr);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_INIT(ViPipe, pstDrvCtx->stStatisticsBuf.bInit);

    osal_snprintf(acName, sizeof(acName), "ISP[%d].Stat", ViPipe);

    u64Size = sizeof(ISP_STAT_S) * MAX_ISP_STAT_BUF_NUM;
    s32Ret = CMPI_MmzMallocCached(GK_NULL, acName, &u64PhyAddr, (GK_VOID **)&pu8VirAddr, u64Size);

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "alloc ISP statistics buf err\n");
        return ERR_CODE_ISP_NOMEM;
    }

    osal_memset(pu8VirAddr, 0, u64Size);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstDrvCtx->stStatisticsBuf.u64PhyAddr = u64PhyAddr;
    pstDrvCtx->stStatisticsBuf.pVirAddr = (GK_VOID *)pu8VirAddr;
    pstDrvCtx->stStatisticsBuf.u64Size  = u64Size;

    OSAL_INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stFreeList);
    OSAL_INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stBusyList);
    OSAL_INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stUserList);

    for (i = 0; i < MAX_ISP_STAT_BUF_NUM; i++) {
        pstDrvCtx->stStatisticsBuf.astNode[i].stStatInfo.u64PhyAddr   =
            u64PhyAddr + i * sizeof(ISP_STAT_S);
        pstDrvCtx->stStatisticsBuf.astNode[i].stStatInfo.pVirtAddr    =
            (GK_VOID *)(pu8VirAddr + i * sizeof(ISP_STAT_S));

        pstDrvCtx->stStatisticsBuf.astNode[i].stStatInfo.unKey.u64Key =
            ((GK_U64)ISP_STATISTICS_KEY << 32) + ISP_STATISTICS_KEY;

        osal_list_add_tail(&pstDrvCtx->stStatisticsBuf.astNode[i].list,
                           &pstDrvCtx->stStatisticsBuf.stFreeList);
    }

    pstDrvCtx->stStatisticsBuf.bInit      = GK_TRUE;
    pstDrvCtx->stStatisticsBuf.u32BusyNum = 0;
    pstDrvCtx->stStatisticsBuf.u32UserNum = 0;
    pstDrvCtx->stStatisticsBuf.u32FreeNum = MAX_ISP_STAT_BUF_NUM;

    *pu64PhyAddr = pstDrvCtx->stStatisticsBuf.u64PhyAddr;

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_StatBufExit(VI_PIPE ViPipe)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    GK_U64 u64PhyAddr;
    GK_U8  *pu8VirAddr;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_EXIT_STATE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_EXIT(ViPipe, pstDrvCtx->stStatisticsBuf.bInit);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    u64PhyAddr = pstDrvCtx->stStatisticsBuf.u64PhyAddr;
    pu8VirAddr = (GK_U8 *)pstDrvCtx->stStatisticsBuf.pVirAddr;

    pstDrvCtx->stStatisticsBuf.pVirAddr = GK_NULL;
    pstDrvCtx->stStatisticsBuf.astNode[0].stStatInfo.pVirtAddr = GK_NULL;
    pstDrvCtx->stStatisticsBuf.astNode[1].stStatInfo.pVirtAddr = GK_NULL;
    pstDrvCtx->stStatisticsBuf.u64PhyAddr = 0;
    pstDrvCtx->stStatisticsBuf.astNode[0].stStatInfo.u64PhyAddr = 0;
    pstDrvCtx->stStatisticsBuf.astNode[1].stStatInfo.u64PhyAddr = 0;
    pstDrvCtx->stStatisticsBuf.bInit    = GK_FALSE;

    OSAL_INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stFreeList);
    OSAL_INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stBusyList);
    OSAL_INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stUserList);
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    if (u64PhyAddr != 0) {
        CMPI_MmzFree(u64PhyAddr, pu8VirAddr);
    }

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_StatBufUserGet(VI_PIPE ViPipe, ISP_STAT_INFO_S **ppstStatInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    struct osal_list_head *plist;
    ISP_STAT_NODE_S *pstNode = GK_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_STABUF_INIT(ViPipe);
    ISP_CHECK_POINTER(ppstStatInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    if (osal_list_empty(&pstDrvCtx->stStatisticsBuf.stBusyList)) {
        ISP_TRACE(MODULE_DBG_WARN, "busy list empty\n");
        *ppstStatInfo = GK_NULL;
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return GK_FAILURE;
    }

    /* get busy */
    plist = pstDrvCtx->stStatisticsBuf.stBusyList.next;
    if (plist == GK_NULL) {
        ISP_TRACE(MODULE_DBG_WARN, "busy list empty\n");
        *ppstStatInfo = GK_NULL;
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return GK_FAILURE;
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

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_StatBufUserPut(VI_PIPE ViPipe, ISP_STAT_INFO_S *pstStatInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    struct osal_list_head *plist = GK_NULL;
    ISP_STAT_NODE_S *pstNode = GK_NULL;
    GK_BOOL bValid = GK_FALSE;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_STABUF_INIT(ViPipe);
    ISP_CHECK_POINTER(pstStatInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    osal_list_for_each(plist, &pstDrvCtx->stStatisticsBuf.stUserList) {
        pstNode = osal_list_entry(plist, ISP_STAT_NODE_S, list);
        if (pstNode == GK_NULL) {
            ISP_TRACE(MODULE_DBG_ERR, "pstNode  null pointer\n");
            break;
        }

        if (pstNode->stStatInfo.u64PhyAddr == pstStatInfo->u64PhyAddr) {
            bValid = GK_TRUE;
            pstNode->stStatInfo.unKey.u64Key = pstStatInfo->unKey.u64Key;
            break;
        }
    }

    if (!bValid) {
        ISP_TRACE(MODULE_DBG_ERR, "invalid stat info, phy 0x%llx\n", pstStatInfo->u64PhyAddr);
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return GK_FAILURE;
    }

    /* get user */
    if (plist == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "user list empty\n");
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return GK_FAILURE;
    }
    osal_list_del(plist);
    pstDrvCtx->stStatisticsBuf.u32UserNum--;

    /* put free */
    osal_list_add_tail(plist, &pstDrvCtx->stStatisticsBuf.stFreeList);
    pstDrvCtx->stStatisticsBuf.u32FreeNum++;
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_SetLdciSttAddr(VI_PIPE ViPipe, GK_U64 u64ReadSttAddr, GK_U64 u64WriteSttAddr)
{
    GK_U8  k, u8BlkDev, u8BlkNum;
    GK_S32 s32Ret;
    GK_U32 u32Size = sizeof(ISP_LDCI_STAT_S);
    GK_U64 u64WriteAddr;
    ISP_BE_REGS_ATTR_S  stIspBeRegsAttr = { 0 };
    S_VIPROC_REGS_TYPE *apstViprocReg[ISP_STRIPING_MAX_NUM] = { GK_NULL };

    ISP_CHECK_PIPE(ViPipe);

    s32Ret = ISP_DRV_GetViprocRegsAttr(ViPipe, apstViprocReg, &stIspBeRegsAttr);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] Get ViprocRegs Attr Err!\n", ViPipe);
        return GK_FAILURE;
    }

    u8BlkDev = stIspBeRegsAttr.u8BlockDev;
    u8BlkNum = stIspBeRegsAttr.u8BlockNum;
    for (k = 0; k < u8BlkNum; k++) {
        u64WriteAddr = u64WriteSttAddr + k * (u32Size / DIV_0_TO_1(u8BlkNum));
        ISP_DRV_SetLdciWriteSttAddr(apstViprocReg[k + u8BlkDev], u64WriteAddr);

        if (g_LdciTprFltEn[ViPipe] == GK_FALSE) {
            ISP_DRV_SetLdciReadSttAddr(apstViprocReg[k + u8BlkDev], u64ReadSttAddr);
        }
    }

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_UpdateLdciNormalOnlineAddr(VI_PIPE ViPipe)
{
    GK_U8 u8ReadBufIdx, u8WriteBufIdx;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    u8ReadBufIdx  = pstDrvCtx->stLdciReadBufAttr.u8BufIdx;
    u8WriteBufIdx = pstDrvCtx->stLdciWriteBufAttr.u8BufIdx;

    ISP_DRV_SetLdciSttAddr(ViPipe, pstDrvCtx->stLdciReadBufAttr.astLdciBuf[u8ReadBufIdx].u64PhyAddr,
                           pstDrvCtx->stLdciWriteBufAttr.astLdciBuf[u8WriteBufIdx].u64PhyAddr);

    pstDrvCtx->stLdciReadBufAttr.u8BufIdx  = u8WriteBufIdx;
    pstDrvCtx->stLdciWriteBufAttr.u8BufIdx = (u8WriteBufIdx + 1) % DIV_0_TO_1(pstDrvCtx->stLdciWriteBufAttr.u8BufNum);

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_CopyLdciStat(VI_PIPE ViPipe, ISP_STAT_S *pstStat)
{
    ISP_DRV_CTX_S   *pstDrvCtx   = GK_NULL;
    ISP_LDCI_STAT_S *pstLdciStat = GK_NULL;
    GK_U8 u8CurReadBufIdx;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstStat);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    u8CurReadBufIdx = pstDrvCtx->stLdciWriteBufAttr.u8BufIdx;
    pstLdciStat = (ISP_LDCI_STAT_S *)pstDrvCtx->stLdciWriteBufAttr.astLdciBuf[u8CurReadBufIdx].pVirAddr;
    if (pstLdciStat != GK_NULL) {
        osal_memcpy(&pstStat->stLdciStat, pstLdciStat, sizeof(ISP_LDCI_STAT_S));
    }
    return GK_SUCCESS;
}
GK_S32 ISP_DRV_UpdateLdciTprOnlineAddr(VI_PIPE ViPipe)
{
    GK_U8 u8CurReadBufIdx;
    ISP_DRV_CTX_S   *pstDrvCtx   = GK_NULL;
    ISP_CHECK_PIPE(ViPipe);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    u8CurReadBufIdx = pstDrvCtx->stLdciWriteBufAttr.u8BufIdx;
    ISP_DRV_SetLdciSttAddr(ViPipe, pstDrvCtx->stLdciReadBufAttr.astLdciBuf[0].u64PhyAddr,
                           pstDrvCtx->stLdciWriteBufAttr.astLdciBuf[u8CurReadBufIdx].u64PhyAddr);
    pstDrvCtx->stLdciWriteBufAttr.u8BufIdx = (u8CurReadBufIdx + 1) % DIV_0_TO_1(pstDrvCtx->stLdciWriteBufAttr.u8BufNum);
    return GK_SUCCESS;
}
GK_S32 ISP_DRV_UpdateLdciNormalOfflineAttr(VI_PIPE ViPipe, ISP_BE_SYNC_PARA_S *pstBeSyncPara)
{
    GK_U8  i, u8ReadBufIdx, u8WriteBufIdx, u8BlkNum;
    GK_U32 u32Size;
    GK_U64 u64WriteSttAddr, u64ReadSttAddr;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    u8BlkNum      = pstDrvCtx->stWorkMode.u8BlockNum;
    u32Size       = sizeof(ISP_LDCI_STAT_S);
    u8ReadBufIdx  = pstDrvCtx->stLdciReadBufAttr.u8BufIdx;
    u8WriteBufIdx = pstDrvCtx->stLdciWriteBufAttr.u8BufIdx;
    u64WriteSttAddr = pstDrvCtx->stLdciWriteBufAttr.astLdciBuf[u8WriteBufIdx].u64PhyAddr;
    u64ReadSttAddr  = pstDrvCtx->stLdciReadBufAttr.astLdciBuf[u8ReadBufIdx].u64PhyAddr;

    for (i = 0; i < u8BlkNum; i++) {
        pstBeSyncPara->au64LdciReadSttAddr[i]  = u64ReadSttAddr;
        pstBeSyncPara->au64LdciWriteSttAddr[i] = u64WriteSttAddr + i * (u32Size / DIV_0_TO_1(u8BlkNum));
    }

    pstDrvCtx->stLdciReadBufAttr.u8BufIdx  = u8WriteBufIdx;
    pstDrvCtx->stLdciWriteBufAttr.u8BufIdx = (u8WriteBufIdx + 1) % DIV_0_TO_1(pstDrvCtx->stLdciWriteBufAttr.u8BufNum);

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_UpdateLdciTprOfflineAttr(VI_PIPE ViPipe, ISP_BE_SYNC_PARA_S *pstBeSyncPara)
{
    GK_U8 i;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    for (i = 0; i < pstDrvCtx->stWorkMode.u8BlockNum; i++) {
        pstBeSyncPara->au64LdciReadSttAddr[i]  = pstDrvCtx->stLdciSttAddr.au64ReadSttAddr[i];
        pstBeSyncPara->au64LdciWriteSttAddr[i] = pstDrvCtx->stLdciSttAddr.au64CurWriteSttAddr[i];
    }

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_LdciOnlineAttrUpdate(VI_PIPE ViPipe, ISP_STAT_S *pstStat)
{
    ISP_CHECK_PIPE(ViPipe);
    if (g_LdciTprFltEn[ViPipe] == GK_TRUE) {
        /* Copy LDCI statistics information to pstStat, then update LDCI WriteSttAddr */
        ISP_DRV_CopyLdciStat(ViPipe, pstStat);
    }
    return GK_SUCCESS;
}

GK_S32 ISP_DRV_StatBufBusyPut(VI_PIPE ViPipe)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    struct osal_list_head *plist;
    ISP_STAT_NODE_S *pstNode = GK_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_STABUF_INIT(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    /* There should be one frame of the newest statistics info in busy list. */
    while (!osal_list_empty(&pstDrvCtx->stStatisticsBuf.stBusyList)) {
        plist = pstDrvCtx->stStatisticsBuf.stBusyList.next;
        if (plist == GK_NULL) {
            osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
            return GK_FAILURE;
        }
        osal_list_del(plist);
        pstDrvCtx->stStatisticsBuf.u32BusyNum--;

        osal_list_add_tail(plist, &pstDrvCtx->stStatisticsBuf.stFreeList);
        pstDrvCtx->stStatisticsBuf.u32FreeNum++;
    }

    if (osal_list_empty(&pstDrvCtx->stStatisticsBuf.stFreeList)) {
        ISP_TRACE(MODULE_DBG_WARN, "free list empty\n");
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

        return GK_FAILURE;
    }

    /* get free */
    plist = pstDrvCtx->stStatisticsBuf.stFreeList.next;
    if (plist == GK_NULL) {
        ISP_TRACE(MODULE_DBG_WARN, "free list empty\n");
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return GK_FAILURE;
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

    return GK_SUCCESS;
}

static GK_VOID ISP_DRV_LdciBufAttrReset(ISP_DRV_CTX_S *pstDrvCtx)
{
    GK_U8 i;

    pstDrvCtx->stLdciReadBufAttr.bInit  = GK_FALSE;
    pstDrvCtx->stLdciWriteBufAttr.bInit = GK_FALSE;
    pstDrvCtx->stLdciReadBufAttr.u8BufNum  = 0;
    pstDrvCtx->stLdciWriteBufAttr.u8BufNum = 0;

    for (i = 0; i < MAX_ISP_BE_BUF_NUM; i++) {
        pstDrvCtx->stLdciWriteBufAttr.astLdciBuf[i].u64PhyAddr = 0;
        pstDrvCtx->stLdciWriteBufAttr.astLdciBuf[i].pVirAddr   = GK_NULL;
        pstDrvCtx->stLdciWriteBufAttr.astLdciBuf[i].u64Size    = 0;
    }

    for (i = 0; i < MAX_ISP_BE_BUF_NUM; i++) {
        pstDrvCtx->stLdciReadBufAttr.astLdciBuf[i].u64PhyAddr = 0;
        pstDrvCtx->stLdciReadBufAttr.astLdciBuf[i].pVirAddr   = GK_NULL;
        pstDrvCtx->stLdciReadBufAttr.astLdciBuf[i].u64Size    = 0;
    }
}

GK_S32 ISP_DRV_LdciTprBufInit(VI_PIPE ViPipe)
{
    GK_U8 i, j, u8WriteBufNum, u8ReadBufNum, u8BufNumAll;
    GK_U8 *pu8VirAddr;
    GK_S32 s32Ret;
    GK_U64 u64PhyAddr, u64Size;
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    GK_CHAR acName[MAX_MMZ_NAMELEN] = { 0 };

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    ISP_CHECK_BUF_INIT(ViPipe, pstDrvCtx->stLdciReadBufAttr.bInit);
    ISP_CHECK_BUF_INIT(ViPipe, pstDrvCtx->stLdciWriteBufAttr.bInit);

    osal_snprintf(acName, sizeof(acName), "ISP[%d].LDCI", ViPipe);
    u64Size = sizeof(ISP_LDCI_STAT_S);

    if (IS_ONLINE_MODE(pstDrvCtx->stWorkMode.enIspRunningMode) ||
        IS_SIDEBYSIDE_MODE(pstDrvCtx->stWorkMode.enIspRunningMode)) {
        u8WriteBufNum = 2;
        u8ReadBufNum  = 2;
    } else {
        u8WriteBufNum = MAX_ISP_BE_BUF_NUM;
        u8ReadBufNum  = MAX_ISP_BE_BUF_NUM;
    }

    u8BufNumAll = u8WriteBufNum + u8ReadBufNum;

    s32Ret = CMPI_MmzMallocNocache(GK_NULL, acName, &u64PhyAddr, (GK_VOID **)&pu8VirAddr, u64Size * u8BufNumAll);

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] MmzMalloc Ldci buffer Failure!\n", ViPipe);
        return ERR_CODE_ISP_NOMEM;
    }

    osal_memset(pu8VirAddr, 0, u64Size * u8BufNumAll);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    pstDrvCtx->stLdciWriteBufAttr.bInit    = GK_TRUE;
    pstDrvCtx->stLdciWriteBufAttr.u8BufNum = u8WriteBufNum;
    pstDrvCtx->stLdciWriteBufAttr.u8BufIdx = 0;

    pstDrvCtx->stLdciReadBufAttr.bInit    = GK_TRUE;
    pstDrvCtx->stLdciReadBufAttr.u8BufNum = u8ReadBufNum;
    pstDrvCtx->stLdciReadBufAttr.u8BufIdx = 0;

    for (i = 0; i < u8WriteBufNum; i++) {
        pstDrvCtx->stLdciWriteBufAttr.astLdciBuf[i].u64PhyAddr = u64PhyAddr + i * u64Size;
        pstDrvCtx->stLdciWriteBufAttr.astLdciBuf[i].pVirAddr   = (GK_VOID *)(pu8VirAddr + i * u64Size);
        pstDrvCtx->stLdciWriteBufAttr.astLdciBuf[i].u64Size    = u64Size;
    }

    for (i = 0; i < u8ReadBufNum; i++) {
        j = i + u8WriteBufNum;
        pstDrvCtx->stLdciReadBufAttr.astLdciBuf[i].u64PhyAddr = u64PhyAddr + j * u64Size;
        pstDrvCtx->stLdciReadBufAttr.astLdciBuf[i].pVirAddr   = (GK_VOID *)(pu8VirAddr + j * u64Size);
        pstDrvCtx->stLdciReadBufAttr.astLdciBuf[i].u64Size    = u64Size;
    }

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    if (IS_ONLINE_MODE(pstDrvCtx->stWorkMode.enIspRunningMode) ||
        IS_SIDEBYSIDE_MODE(pstDrvCtx->stWorkMode.enIspRunningMode)) {
        s32Ret = ISP_DRV_SetLdciSttAddr(ViPipe, pstDrvCtx->stLdciReadBufAttr.astLdciBuf[0].u64PhyAddr,
                                        pstDrvCtx->stLdciWriteBufAttr.astLdciBuf[0].u64PhyAddr);

        if (s32Ret != GK_SUCCESS) {
            ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] Set Ldci Param/OutParam addr Err!\n", ViPipe);
            goto fail0;
        }

        /* update Write Index */
        pstDrvCtx->stLdciWriteBufAttr.u8BufIdx = (pstDrvCtx->stLdciWriteBufAttr.u8BufIdx + 1) %
                                                 DIV_0_TO_1(pstDrvCtx->stLdciWriteBufAttr.u8BufNum);
    }

    return GK_SUCCESS;

fail0:
    ISP_DRV_LdciBufAttrReset(pstDrvCtx);
    if (u64PhyAddr != 0) {
        CMPI_MmzFree(u64PhyAddr, (GK_VOID *)pu8VirAddr);
    }
    return GK_FAILURE;
}

GK_S32 ISP_DRV_LdciTprBufExit(VI_PIPE ViPipe)
{
    GK_U64 u64PhyAddr;
    GK_VOID *pVirAddr;
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_EXIT(ViPipe, pstDrvCtx->stLdciReadBufAttr.bInit);
    ISP_CHECK_BUF_EXIT(ViPipe, pstDrvCtx->stLdciWriteBufAttr.bInit);

    u64PhyAddr = pstDrvCtx->stLdciWriteBufAttr.astLdciBuf[0].u64PhyAddr;
    pVirAddr   = pstDrvCtx->stLdciWriteBufAttr.astLdciBuf[0].pVirAddr;

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    ISP_DRV_LdciBufAttrReset(pstDrvCtx);
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    if (u64PhyAddr != 0) {
        CMPI_MmzFree(u64PhyAddr, pVirAddr);
    }

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_LdciNormalBufInit(VI_PIPE ViPipe)
{
    GK_U8 i, u8BufNum;
    GK_S32 s32Ret;
    GK_U64 u64PhyAddr, u64Size;
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    GK_CHAR acName[MAX_MMZ_NAMELEN] = { 0 };

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_INIT(ViPipe, pstDrvCtx->stLdciReadBufAttr.bInit);
    ISP_CHECK_BUF_INIT(ViPipe, pstDrvCtx->stLdciWriteBufAttr.bInit);

    osal_snprintf(acName, sizeof(acName), "ISP[%d].LDCI", ViPipe);
    u64Size  = sizeof(ISP_LDCI_STAT_S);

    if (IS_ONLINE_MODE(pstDrvCtx->stWorkMode.enIspRunningMode) ||
        IS_OFFLINE_MODE(pstDrvCtx->stWorkMode.enIspRunningMode) ||
        IS_SIDEBYSIDE_MODE(pstDrvCtx->stWorkMode.enIspRunningMode)) {
        u8BufNum = 1;
    } else {
        u8BufNum = 2;
    }

    u64PhyAddr = CMPI_MmzMalloc(GK_NULL, acName, u64Size * u8BufNum);
    if (u64PhyAddr == 0) {
        ISP_TRACE(MODULE_DBG_ERR, "alloc ISP[%d] Ldci buf err\n", ViPipe);
        return ERR_CODE_ISP_NOMEM;
    }

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstDrvCtx->stLdciReadBufAttr.bInit    = GK_TRUE;
    pstDrvCtx->stLdciReadBufAttr.u8BufNum = u8BufNum;
    pstDrvCtx->stLdciReadBufAttr.u8BufIdx = 0;

    pstDrvCtx->stLdciWriteBufAttr.bInit    = GK_TRUE;
    pstDrvCtx->stLdciWriteBufAttr.u8BufNum = u8BufNum;
    pstDrvCtx->stLdciWriteBufAttr.u8BufIdx = MIN2(u8BufNum - 1, pstDrvCtx->stLdciReadBufAttr.u8BufIdx + 1);

    for (i = 0; i < u8BufNum; i++) {
        pstDrvCtx->stLdciReadBufAttr.astLdciBuf[i].u64PhyAddr  = u64PhyAddr + i * u64Size;
        pstDrvCtx->stLdciWriteBufAttr.astLdciBuf[i].u64PhyAddr = u64PhyAddr + i * u64Size;
    }

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    if (IS_SIDEBYSIDE_MODE(pstDrvCtx->stWorkMode.enIspRunningMode) ||
        IS_ONLINE_MODE(pstDrvCtx->stWorkMode.enIspRunningMode)) {
        s32Ret = ISP_DRV_SetLdciSttAddr(ViPipe, u64PhyAddr, u64PhyAddr);

        if (s32Ret != GK_SUCCESS) {
            ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] Set Ldci stt addr Err!\n", ViPipe);
            goto fail0;
        }

        /* update Write & Read Index */
        pstDrvCtx->stLdciReadBufAttr.u8BufIdx  = pstDrvCtx->stLdciWriteBufAttr.u8BufIdx;
        pstDrvCtx->stLdciWriteBufAttr.u8BufIdx = (pstDrvCtx->stLdciWriteBufAttr.u8BufIdx + 1) %
                                                 DIV_0_TO_1(pstDrvCtx->stLdciWriteBufAttr.u8BufNum);
    }

    return GK_SUCCESS;

fail0:
    ISP_DRV_LdciBufAttrReset(pstDrvCtx);

    if (u64PhyAddr != 0) {
        CMPI_MmzFree(u64PhyAddr, GK_NULL);
    }

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_LdciNormalBufExit(VI_PIPE ViPipe)
{
    GK_U64 u64PhyAddr;
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_EXIT(ViPipe, pstDrvCtx->stLdciReadBufAttr.bInit);
    ISP_CHECK_BUF_EXIT(ViPipe, pstDrvCtx->stLdciWriteBufAttr.bInit);

    u64PhyAddr = pstDrvCtx->stLdciWriteBufAttr.astLdciBuf[0].u64PhyAddr;

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    ISP_DRV_LdciBufAttrReset(pstDrvCtx);
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    if (u64PhyAddr != 0) {
        CMPI_MmzFree(u64PhyAddr, GK_NULL);
    }

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_LdciBufInit(VI_PIPE ViPipe)
{
    ISP_CHECK_PIPE(ViPipe);

    if (g_LdciTprFltEn[ViPipe] == GK_TRUE) {
        return ISP_DRV_LdciTprBufInit(ViPipe);
    } else {
        return ISP_DRV_LdciNormalBufInit(ViPipe);
    }
}

GK_S32 ISP_DRV_LdciBufExit(VI_PIPE ViPipe)
{
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_EXIT_STATE(ViPipe);

    if (g_LdciTprFltEn[ViPipe] == GK_TRUE) {
        return ISP_DRV_LdciTprBufExit(ViPipe);
    } else {
        return ISP_DRV_LdciNormalBufExit(ViPipe);
    }

    return GK_FAILURE;
}

GK_S32 ISP_DRV_LdciReadSttBufGet(VI_PIPE ViPipe, ISP_LDCI_READ_STT_BUF_S *pstLdciReadSttBuf)
{
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstLdciReadSttBuf);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstLdciReadSttBuf->u8BufNum       = pstDrvCtx->stLdciReadBufAttr.u8BufNum;
    pstLdciReadSttBuf->u8BufIdx       = 0;
    pstLdciReadSttBuf->u64HeadPhyAddr = pstDrvCtx->stLdciReadBufAttr.astLdciBuf[0].u64PhyAddr;

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_ClutBufInit(VI_PIPE ViPipe)
{
    GK_S32 s32Ret;
    GK_U64 u64PhyAddr, u64Size;
    GK_U8 *pu8VirAddr;
    ISP_DRV_CTX_S      *pstDrvCtx = GK_NULL;
    GK_CHAR acName[MAX_MMZ_NAMELEN] = { 0 };

    ISP_CHECK_PIPE(ViPipe);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_INIT(ViPipe, pstDrvCtx->stClutBufAttr.bInit);

    osal_snprintf(acName, sizeof(acName), "ISP[%d].Clut", ViPipe);

    u64Size =  ISP_CLUT_LUT_LENGTH * sizeof(GK_U32);

    s32Ret = CMPI_MmzMallocNocache(GK_NULL, acName, &u64PhyAddr, (GK_VOID **)&pu8VirAddr, u64Size);

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] MmzMalloc Clut buffer Failure!\n", ViPipe);
        return ERR_CODE_ISP_NOMEM;
    }

    osal_memset(pu8VirAddr, 0, u64Size);

    pstDrvCtx->stClutBufAttr.bInit                = GK_TRUE;
    pstDrvCtx->stClutBufAttr.stClutBuf.u64Size    = u64Size;
    pstDrvCtx->stClutBufAttr.stClutBuf.u64PhyAddr = u64PhyAddr;
    pstDrvCtx->stClutBufAttr.stClutBuf.pVirAddr   = (GK_VOID *)pu8VirAddr;

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_ClutBufExit(VI_PIPE ViPipe)
{
    GK_U64   u64PhyAddr;
    GK_VOID *pVirAddr;
    ISP_DRV_CTX_S      *pstDrvCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_EXIT_STATE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_EXIT(ViPipe, pstDrvCtx->stClutBufAttr.bInit);

    u64PhyAddr = pstDrvCtx->stClutBufAttr.stClutBuf.u64PhyAddr;
    pVirAddr   = pstDrvCtx->stClutBufAttr.stClutBuf.pVirAddr;

    pstDrvCtx->stClutBufAttr.bInit                = GK_FALSE;
    pstDrvCtx->stClutBufAttr.stClutBuf.u64Size    = 0;
    pstDrvCtx->stClutBufAttr.stClutBuf.u64PhyAddr = 0;
    pstDrvCtx->stClutBufAttr.stClutBuf.pVirAddr   = GK_NULL;

    if (u64PhyAddr != 0) {
        CMPI_MmzFree(u64PhyAddr, pVirAddr);
    }

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_SpecAwbBufInit(VI_PIPE ViPipe)
{
#ifdef CONFIG_ISP_SPECAWB_SUPPORT
    GK_S32 s32Ret;
    GK_U64 u64PhyAddr, u64Size;
    GK_U8 *pu8VirAddr;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    GK_CHAR acName[MAX_MMZ_NAMELEN] = { 0 };

    ISP_CHECK_PIPE(ViPipe);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_INIT(ViPipe, pstDrvCtx->stSpecAwbBufAttr.bInit);

    osal_snprintf(acName, sizeof(acName), "ISP[%d].SpecAwb", ViPipe);

    u64Size = ISP_SPECAWB_BUF_SIZE;
    s32Ret  = CMPI_MmzMallocNocache(GK_NULL, acName, &u64PhyAddr, (GK_VOID **)&pu8VirAddr, u64Size);

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] MmzMalloc SpecAwb buffer Failure!\n", ViPipe);
        return GK_FAILURE;
    }

    osal_memset(pu8VirAddr, 0, u64Size);

    pstDrvCtx->stSpecAwbBufAttr.bInit                   = GK_TRUE;
    pstDrvCtx->stSpecAwbBufAttr.stSpecAwbBuf.u64Size    = u64Size;
    pstDrvCtx->stSpecAwbBufAttr.stSpecAwbBuf.u64PhyAddr = u64PhyAddr;
    pstDrvCtx->stSpecAwbBufAttr.stSpecAwbBuf.pVirAddr   = (GK_VOID *)pu8VirAddr;
#endif
    return GK_SUCCESS;
}

GK_S32 ISP_DRV_SpecAwbBufExit(VI_PIPE ViPipe)
{
#ifdef CONFIG_ISP_SPECAWB_SUPPORT
    GK_U64   u64PhyAddr;
    GK_VOID *pVirAddr;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_EXIT_STATE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_EXIT(ViPipe, pstDrvCtx->stSpecAwbBufAttr.bInit);

    u64PhyAddr = pstDrvCtx->stSpecAwbBufAttr.stSpecAwbBuf.u64PhyAddr;
    pVirAddr   = pstDrvCtx->stSpecAwbBufAttr.stSpecAwbBuf.pVirAddr;

    pstDrvCtx->stSpecAwbBufAttr.bInit                   = GK_FALSE;
    pstDrvCtx->stSpecAwbBufAttr.stSpecAwbBuf.u64Size    = 0;
    pstDrvCtx->stSpecAwbBufAttr.stSpecAwbBuf.u64PhyAddr = 0;
    pstDrvCtx->stSpecAwbBufAttr.stSpecAwbBuf.pVirAddr   = GK_NULL;

    if (u64PhyAddr != 0) {
        CMPI_MmzFree(u64PhyAddr, pVirAddr);
    }
#endif
    return GK_SUCCESS;
}


GK_S32 ISP_DRV_BeBufInit(VI_PIPE ViPipe, GK_U64 *pu64PhyAddr)
{
    GK_S32 s32Ret;
    GK_U32 i;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    ISP_BE_BUF_NODE_S *pstNode = GK_NULL;
    GK_U64 u64PhyAddr, u64Size;
    GK_U8  *pu8VirAddr;
    GK_CHAR acName[MAX_MMZ_NAMELEN] = {0};
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pu64PhyAddr);
    ISP_CHECK_ONLINE_MODE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_INIT(ViPipe, pstDrvCtx->stBeBufInfo.bInit);

    osal_snprintf(acName, sizeof(acName), "ISP[%d].BeCfg", ViPipe);

    u64Size = sizeof(ISP_BE_WO_REG_CFG_S);

    s32Ret = CMPI_MmzMallocCached(GK_NULL, acName, &u64PhyAddr, (GK_VOID **)&pu8VirAddr, u64Size * MAX_ISP_BE_BUF_NUM);

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] alloc ISP BeCfgBuf err!\n", ViPipe);
        return ERR_CODE_ISP_NOMEM;
    }

    osal_memset(pu8VirAddr, 0, u64Size * MAX_ISP_BE_BUF_NUM);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    pstDrvCtx->stBeBufInfo.bInit                   = GK_TRUE;
    pstDrvCtx->stBeBufInfo.stBeBufHaddr.u64PhyAddr = u64PhyAddr;
    pstDrvCtx->stBeBufInfo.stBeBufHaddr.pVirAddr   = (GK_VOID *)pu8VirAddr;
    pstDrvCtx->stBeBufInfo.stBeBufHaddr.u64Size    = u64Size * MAX_ISP_BE_BUF_NUM;

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    s32Ret = ISP_CreatBeBufQueue(&pstDrvCtx->stBeBufQueue, MAX_ISP_BE_BUF_NUM);

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] CreatBeBufQueue fail!\n", ViPipe);

        goto fail0;
    }

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    for (i = 0; i < MAX_ISP_BE_BUF_NUM; i++) {
        pstNode = ISP_QueueGetFreeBeBuf(&pstDrvCtx->stBeBufQueue);

        if (pstNode == GK_NULL) {
            ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] Get QueueGetFreeBeBuf fail!\r\n", ViPipe);
            osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
            goto fail1;
        }

        pstNode->stBeCfgBuf.u64PhyAddr = u64PhyAddr + i * u64Size;
        pstNode->stBeCfgBuf.pVirAddr   = (GK_VOID *)(pu8VirAddr + i * u64Size);
        pstNode->stBeCfgBuf.u64Size    = u64Size;
        pstNode->s32HoldCnt            = 0;

        ISP_QueuePutFreeBeBuf(&pstDrvCtx->stBeBufQueue, pstNode);
    }

    pstDrvCtx->pstUseNode        = GK_NULL;
    pstDrvCtx->enIspRunningState = ISP_BE_BUF_STATE_INIT;
    pstDrvCtx->enIspExitState    = ISP_BE_BUF_READY;

    *pu64PhyAddr = pstDrvCtx->stBeBufInfo.stBeBufHaddr.u64PhyAddr;

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;

fail1:
    ISP_DestroyBeBufQueue(&pstDrvCtx->stBeBufQueue);

fail0:
    pstDrvCtx->stBeBufInfo.bInit                   = GK_FALSE;
    pstDrvCtx->stBeBufInfo.stBeBufHaddr.u64PhyAddr = 0;
    pstDrvCtx->stBeBufInfo.stBeBufHaddr.pVirAddr   = GK_NULL;
    pstDrvCtx->stBeBufInfo.stBeBufHaddr.u64Size    = 0;

    if (u64PhyAddr != 0) {
        CMPI_MmzFree(u64PhyAddr, (GK_VOID *)pu8VirAddr);
    }

    return GK_FAILURE;
}

GK_S32 ISP_DRV_BeBufExit(VI_PIPE ViPipe)
{
    GK_S32 s32Ret;
    GK_U64 u64PhyAddr;
    GK_VOID *pVirAddr;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_ONLINE_MODE(ViPipe);
    ISP_CHECK_EXIT_STATE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_EXIT(ViPipe, pstDrvCtx->stBeBufInfo.bInit);

    u64PhyAddr = pstDrvCtx->stBeBufInfo.stBeBufHaddr.u64PhyAddr;
    pVirAddr   = pstDrvCtx->stBeBufInfo.stBeBufHaddr.pVirAddr;

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstDrvCtx->enIspExitState = ISP_BE_BUF_WAITING;
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    if (CHECK_FUNC_ENTRY(MOD_ID_VI) && CKFN_VI_UpdateViVpssMode()) {
        /* Note: this function cannot be placed in the ISP lock, otherwise it will be deadlocked. */
        CALL_VI_IspClearInputQueue(ViPipe);
    }

    s32Ret = osal_wait_event_timeout_interruptible(&pstDrvCtx->stIspExitWait, ISP_DRV_WaitExitCallback, \
                                                   &pstDrvCtx->stBeBufInfo.s32UseCnt, g_IspExitTimeout);

    if (s32Ret <= 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Pipe:%d isp exit wait failed:s32Ret:%d!\n", ViPipe, s32Ret);
        return GK_FAILURE;
    }

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    ISP_DestroyBeBufQueue(&pstDrvCtx->stBeBufQueue);

    pstDrvCtx->stBeBufInfo.bInit                   = GK_FALSE;
    pstDrvCtx->stBeBufInfo.stBeBufHaddr.u64PhyAddr = 0;
    pstDrvCtx->stBeBufInfo.stBeBufHaddr.pVirAddr   = GK_NULL;
    pstDrvCtx->stBeBufInfo.stBeBufHaddr.u64Size    = 0;
    pstDrvCtx->enIspExitState                      = ISP_BE_BUF_EXIT;

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    if (u64PhyAddr != 0) {
        CMPI_MmzFree(u64PhyAddr, pVirAddr);
    }

    return GK_SUCCESS;
}

osal_spinlock_t *ISP_DRV_GetSpinLock(VI_PIPE ViPipe)
{
    VI_PIPE MainPipeS;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstDrvCtx->stStitchAttr.bStitchEnable != GK_TRUE) {
        return &g_stIspLock[ViPipe];
    } else {
        MainPipeS = pstDrvCtx->stStitchAttr.as8StitchBindId[0];
        return &g_stIspSyncLock[MainPipeS];
    }
}

GK_S32 ISP_DRV_GetReadyBeBuf(VI_PIPE ViPipe, ISP_BE_WO_CFG_BUF_S *pstBeCfgBuf)
{
    osal_spinlock_t *pIspSpinLock;
    unsigned long u32Flags;
    ISP_BE_BUF_NODE_S *pstNode = GK_NULL;
    ISP_DRV_CTX_S *pstDrvCtx  = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstBeCfgBuf);
    ISP_CHECK_ONLINE_MODE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    pIspSpinLock = ISP_DRV_GetSpinLock(ViPipe);
    osal_spin_lock_irqsave(pIspSpinLock, &u32Flags);

    if ((pstDrvCtx->enIspExitState == ISP_BE_BUF_EXIT) ||
        (pstDrvCtx->enIspExitState == ISP_BE_BUF_WAITING)) {
        ISP_TRACE(MODULE_DBG_ERR, "ViPipe[%d] ISP BE Buf not existed!!!\n", ViPipe);
        osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);
        return GK_FAILURE;
    }

    pstNode = ISP_QueueQueryBusyBeBuf(&pstDrvCtx->stBeBufQueue);
    if (pstNode == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "ViPipe[%d] QueueQueryBusyBeBuf pstNode is null!\n", ViPipe);
        osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);
        return GK_FAILURE;
    }

    pstNode->s32HoldCnt++;
    pstDrvCtx->stBeBufInfo.s32UseCnt++;

    osal_memcpy(pstBeCfgBuf, &pstNode->stBeCfgBuf, sizeof(ISP_BE_WO_CFG_BUF_S));
    ISP_DRV_UpdateLdciTprSttAddr(ViPipe, (ISP_BE_WO_REG_CFG_S *)pstNode->stBeCfgBuf.pVirAddr);
    pstDrvCtx->enIspExitState = ISP_BE_BUF_READY;

    osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);

    return GK_SUCCESS;

}

GK_VOID ISP_DRV_PutBusyToFree(VI_PIPE ViPipe, ISP_BE_WO_CFG_BUF_S *pstBeCfgBuf)
{
    ISP_BE_BUF_NODE_S *pstNode;
    struct osal_list_head *pListTmp = GK_NULL;
    struct osal_list_head *pListNode = GK_NULL;
    ISP_DRV_CTX_S *pstDrvCtx  = GK_NULL;

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_list_for_each_safe(pListNode, pListTmp, &pstDrvCtx->stBeBufQueue.stBusyList) {
        pstNode = osal_list_entry(pListNode, ISP_BE_BUF_NODE_S, list);

        if (pstNode->stBeCfgBuf.u64PhyAddr == pstBeCfgBuf->u64PhyAddr) {
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

GK_S32 ISP_DRV_PutFreeBeBuf(VI_PIPE ViPipe, ISP_BE_WO_CFG_BUF_S *pstBeCfgBuf)
{
    osal_spinlock_t *pIspSpinLock = GK_NULL;
    ISP_DRV_CTX_S *pstDrvCtx  = GK_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstBeCfgBuf);
    ISP_CHECK_ONLINE_MODE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    pIspSpinLock = ISP_DRV_GetSpinLock(ViPipe);
    osal_spin_lock_irqsave(pIspSpinLock, &u32Flags);

    if (pstDrvCtx->enIspExitState == ISP_BE_BUF_EXIT) {
        ISP_TRACE(MODULE_DBG_ERR, "ViPipe[%d] ISP BE Buf not existed!!!\n", ViPipe);
        osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);
        return GK_FAILURE;
    }

    ISP_DRV_PutBusyToFree(ViPipe, pstBeCfgBuf);

    if (pstDrvCtx->stBeBufInfo.s32UseCnt > 0) {
        pstDrvCtx->stBeBufInfo.s32UseCnt--;
    }
    if (g_LdciTprFltEn[ViPipe] == GK_TRUE) {
        ISP_DRV_UpdateLdciTprOfflineStat(ViPipe, (ISP_BE_WO_REG_CFG_S *)pstBeCfgBuf->pVirAddr);
    }

    osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);

    osal_wakeup(&pstDrvCtx->stIspExitWait);

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_HoldBusyBeBuf(VI_PIPE ViPipe, ISP_BE_WO_CFG_BUF_S *pstBeCfgBuf)
{
    osal_spinlock_t *pIspSpinLock = GK_NULL;
    ISP_BE_BUF_NODE_S *pstNode;
    struct osal_list_head *pListTmp = GK_NULL;
    struct osal_list_head *pListNode = GK_NULL;
    ISP_DRV_CTX_S *pstDrvCtx  = GK_NULL;
    unsigned long  u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstBeCfgBuf);
    ISP_CHECK_ONLINE_MODE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    pIspSpinLock = ISP_DRV_GetSpinLock(ViPipe);
    osal_spin_lock_irqsave(pIspSpinLock, &u32Flags);

    if ((pstDrvCtx->enIspExitState == ISP_BE_BUF_EXIT) ||
        (pstDrvCtx->enIspExitState == ISP_BE_BUF_WAITING)) {
        ISP_TRACE(MODULE_DBG_ERR, "ViPipe[%d] ISP BE Buf not existed!!!\n", ViPipe);
        osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);
        return GK_FAILURE;
    }

    osal_list_for_each_safe(pListNode, pListTmp, &pstDrvCtx->stBeBufQueue.stBusyList) {
        pstNode = osal_list_entry(pListNode, ISP_BE_BUF_NODE_S, list);

        if (pstNode->stBeCfgBuf.u64PhyAddr == pstBeCfgBuf->u64PhyAddr) {
            pstNode->s32HoldCnt++;
            pstDrvCtx->stBeBufInfo.s32UseCnt++;
        }
    }

    pstDrvCtx->enIspExitState = ISP_BE_BUF_READY;

    osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_GetBeSyncPara(VI_PIPE ViPipe, ISP_BE_SYNC_PARA_S *pstBeSyncPara)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    GK_S32 s32Ret;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstBeSyncPara);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstDrvCtx->stStitchAttr.bStitchEnable == GK_TRUE) {
        s32Ret = ISP_DRV_GetStitchBeSyncPara(ViPipe, pstBeSyncPara);

        if (s32Ret != GK_SUCCESS) {
            return GK_FAILURE;
        }
    } else {
        osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
        osal_memcpy(pstBeSyncPara, &pstDrvCtx->stIspBeSyncPara, sizeof(ISP_BE_SYNC_PARA_S));
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
    }

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    if (g_LdciTprFltEn[ViPipe] == GK_TRUE) {
        ISP_DRV_UpdateLdciTprOfflineAttr(ViPipe, pstBeSyncPara);
    } else {
        ISP_DRV_UpdateLdciNormalOfflineAttr(ViPipe, pstBeSyncPara);
    }
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_BeBufSwitchState(VI_PIPE ViPipe)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    unsigned long u32Flags;
    osal_spinlock_t *pIspSpinLock = GK_NULL;

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    pIspSpinLock = ISP_DRV_GetSpinLock(ViPipe);

    osal_spin_lock_irqsave(pIspSpinLock, &u32Flags);

    if (pstDrvCtx->stStitchAttr.bStitchEnable == GK_TRUE) {
        pstDrvCtx->enIspRunningState = ISP_BE_BUF_STATE_SWITCH_START;
    }

    osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);

    return GK_SUCCESS;
}


GK_S32 ISP_DRV_SwitchMode(VI_PIPE ViPipe, ISP_DRV_CTX_S *pstDrvCtx)
{
    GK_U8   i, j, k;
    GK_U8   u8BlkDev, u8BlkNum;
    GK_S32  ViDev;
    GK_U32  s32Ret;
    GK_U32  au32ChnSwitch[5] = { 0 };
    ISP_SYNC_CFG_S *pstSyncCfg = GK_NULL;
    ISP_BE_REGS_ATTR_S stIspBeRegsAttr = { 0 };
    S_ISPBE_REGS_TYPE *apstBeReg[ISP_STRIPING_MAX_NUM] = { GK_NULL };

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDrvCtx);
    ViDev = pstDrvCtx->stWDRAttr.ViDev;



    s32Ret = ISP_DRV_GetBeRegsAttr(ViPipe, apstBeReg, &stIspBeRegsAttr);
    if (s32Ret != GK_SUCCESS) {
        return GK_SUCCESS;
    }

    u8BlkDev = stIspBeRegsAttr.u8BlockDev;
    u8BlkNum = stIspBeRegsAttr.u8BlockNum;

    pstSyncCfg = &pstDrvCtx->stSyncCfg;
    pstSyncCfg->u8WDRMode = pstDrvCtx->stWDRCfg.u8WDRMode;

    for (j = 0; j < 3; j++) {
        for (i = 0; i < CFG2VLD_DLY_LIMIT; i++) {
            pstSyncCfg->u32ExpRatio[j][i] = pstDrvCtx->stWDRCfg.au32ExpRatio[j];
        }
    }

    /* init cfg when modes change */
    osal_memset(&pstDrvCtx->stSyncCfg.stSyncCfgBuf, 0, sizeof(ISP_SYNC_CFG_BUF_S));
    osal_memset(&pstDrvCtx->stSyncCfg.apstNode, 0, sizeof(pstDrvCtx->stSyncCfg.apstNode));

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

    if (pstDrvCtx->bYuvMode == GK_TRUE) {
        au32ChnSwitch[4] = 1;
    } else {
        au32ChnSwitch[4] = 0;
    }

    for (k = 0; k < u8BlkNum; k++) {
        ISP_DRV_SetInputSel(apstBeReg[k + u8BlkDev], &au32ChnSwitch[0]);

        osal_memcpy(&pstDrvCtx->astChnSelAttr[k].au32WdrChnSel, au32ChnSwitch, sizeof(au32ChnSwitch));
    }

    /* pt_int_mask */
    if ((IS_FULL_WDR_MODE(pstSyncCfg->u8WDRMode)) || (IS_HALF_WDR_MODE(pstSyncCfg->u8WDRMode))) {
        IO_RW_PT_ADDRESS(VI_PT_BASE(ViDev) + VI_PT_INT_MASK) |= VI_PT_INT_FSTART;
    } else {
        IO_RW_PT_ADDRESS(VI_PT_BASE(ViDev) + VI_PT_INT_MASK) &= (~VI_PT_INT_FSTART);
    }

    pstSyncCfg->u8PreWDRMode = pstSyncCfg->u8WDRMode;

    return GK_SUCCESS;

}

GK_S32 ISP_DRV_GetSyncControlnfo(VI_PIPE ViPipe, ISP_SYNC_CFG_S *pstSyncCfg)
{
    GK_S32 ViDev;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    ISP_SYNC_CFG_BUF_NODE_S *pstCurNode = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstSyncCfg);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    ViDev = pstDrvCtx->stWDRAttr.ViDev;

    pstSyncCfg->u8VCNum = (IO_RW_PT_ADDRESS(VI_PT_BASE(ViDev) + VI_PT0_ID) & 0x30) >> 4;

    if (pstSyncCfg->u8VCNumMax == 0) {
        pstSyncCfg->u8VCNum = 0;
    }

    if (pstSyncCfg->u8VCNum > pstSyncCfg->u8VCNumMax) {
        ISP_TRACE(MODULE_DBG_ERR, "err VC number(%d), can't be large than VC total(%d)!\n",
                  pstSyncCfg->u8VCNum, pstSyncCfg->u8VCNumMax);
    }

    /* get Cfg2VldDlyMAX */
    if (!ISPSyncBufIsEmpty(&pstSyncCfg->stSyncCfgBuf)) {
        pstCurNode = &pstSyncCfg->stSyncCfgBuf.stSyncCfgBufNode[pstSyncCfg->stSyncCfgBuf.u8BufRDFlag];

        if (pstCurNode != GK_NULL) {
            if (pstCurNode->bValid) {
                pstSyncCfg->u8Cfg2VldDlyMAX = pstCurNode->stSnsRegsInfo.u8Cfg2ValidDelayMax;
            }
        }
    }

    if ((pstSyncCfg->u8Cfg2VldDlyMAX > CFG2VLD_DLY_LIMIT) || (pstSyncCfg->u8Cfg2VldDlyMAX < 1)) {
        ISP_TRACE(MODULE_DBG_WARN, "Delay of config to valid is:0x%x\n", pstSyncCfg->u8Cfg2VldDlyMAX);
        pstSyncCfg->u8Cfg2VldDlyMAX = 1;
    }

    /* calc VCCfgNum =  (Cfg2VldDlyMAX * (VCNumMax + 1) + VCNum - Cfg2VldDlyMAX) % (VCNumMax + 1) */
    pstSyncCfg->u8VCCfgNum = (pstSyncCfg->u8VCNum + pstSyncCfg->u8VCNumMax * pstSyncCfg->u8Cfg2VldDlyMAX) %
                             DIV_0_TO_1(pstSyncCfg->u8VCNumMax + 1);

    return GK_SUCCESS;
}

static GK_S32 ISP_DRV_CalcExpRatio(ISP_SYNC_CFG_S *pstSyncCfg, ISP_SYNC_CFG_BUF_NODE_S *pstCurNode,
                                   ISP_SYNC_CFG_BUF_NODE_S *pstPreNode, GK_U64 *au64CurSnsGain,
                                   GK_U64 *au64PreSnsGain)
{
    GK_U32 i, j;
    GK_U64 u64Tmp     = 0;
    GK_U64 u64Ratio   = 0;
    GK_U64 au64Exp[4] = { 0 };

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
            pstSyncCfg->u32ExpRatio[j][0] = (GK_U32)u64Ratio;
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
            pstSyncCfg->u32ExpRatio[j][0] = (GK_U32)u64Ratio;
            pstSyncCfg->u32WDRGain[j][0]  = pstCurNode->stAERegCfg.au32WDRGain[j];
        }

        for (i = CFG2VLD_DLY_LIMIT - 1; i >= 1; i--) {
            pstSyncCfg->u8LFMode[i]      = pstSyncCfg->u8LFMode[i - 1];
            pstSyncCfg->u32WDRGain[3][i] = pstSyncCfg->u32WDRGain[3][i - 1];
        }

        pstSyncCfg->u8LFMode[0]      = 0;
        pstSyncCfg->u32WDRGain[3][0] = 0x100;
    }

    return GK_SUCCESS;
}

static GK_S32 ISP_DRV_CalcDrcComp(ISP_SYNC_CFG_S *pstSyncCfg, ISP_SYNC_CFG_BUF_NODE_S *pstCurNode,
                                  ISP_SYNC_CFG_BUF_NODE_S *pstPreNode)
{
    GK_U32 i;
    GK_U64 u64Tmp, u64CurExp, u64PreExp;

    if (IS_2to1_WDR_MODE(pstSyncCfg->u8WDRMode)) {
        u64CurExp = pstCurNode->stAERegCfg.u64ExposureSF;
        u64PreExp = pstPreNode->stAERegCfg.u64ExposureSF;
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
    pstSyncCfg->u32DRCComp[0] = (GK_U32)u64CurExp;

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_CalcSyncCfg(ISP_SYNC_CFG_S *pstSyncCfg)
{
    GK_U32 i;
    ISP_SYNC_CFG_BUF_NODE_S *pstCurNode = GK_NULL;
    ISP_SYNC_CFG_BUF_NODE_S *pstPreNode = GK_NULL;
    GK_U64 au64CurSnsGain[2] = { 0 };
    GK_U64 au64PreSnsGain[2] = { 0 };
    GK_BOOL bErr = GK_FALSE;

    ISP_CHECK_POINTER(pstSyncCfg);

    /* update node when VCCfgNum is 0 */
    if (pstSyncCfg->u8VCCfgNum == 0) {
        for (i = CFG2VLD_DLY_LIMIT; i >= 1; i--) {
            pstSyncCfg->apstNode[i] = pstSyncCfg->apstNode[i - 1];
        }

        /* avoid skip effective AE results */
        if (ISPSyncBufIsErr(&pstSyncCfg->stSyncCfgBuf)) {
            bErr = GK_TRUE;
        }

        /* read the newest information */
        ISPSyncBufRead2(&pstSyncCfg->stSyncCfgBuf, &pstSyncCfg->apstNode[0]);
    }

    pstCurNode = pstSyncCfg->apstNode[0];

    if (pstCurNode == GK_NULL) {
        return GK_SUCCESS;
    }

    if (pstCurNode->bValid == GK_FALSE) {
        return GK_SUCCESS;
    }

    if (bErr == GK_TRUE) {
        if (pstCurNode->stSnsRegsInfo.enSnsType == ISP_SNS_I2C_TYPE) {
            for (i = 0; i < pstCurNode->stSnsRegsInfo.u32RegNum; i++) {
                pstCurNode->stSnsRegsInfo.astI2cData[i].bUpdate = GK_TRUE;
            }
        } else if (pstCurNode->stSnsRegsInfo.enSnsType == ISP_SNS_SSP_TYPE) {
            for (i = 0; i < pstCurNode->stSnsRegsInfo.u32RegNum; i++) {
                pstCurNode->stSnsRegsInfo.astSspData[i].bUpdate = GK_TRUE;
            }
        } else {
            /* do nothing */
        }

        pstCurNode->stSnsRegsInfo.stSlvSync.bUpdate = GK_TRUE;
    }

    if (pstSyncCfg->apstNode[1] == GK_NULL) {
        pstPreNode = pstSyncCfg->apstNode[0];
    } else {
        if (pstSyncCfg->apstNode[1]->bValid == GK_FALSE) {
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
            au64PreSnsGain[0] = osal_div_u64(au64PreSnsGain[0], DIV_0_TO_1((GK_U64)pstPreNode->stAERegCfg.u32IspDgain * pstPreNode->stAERegCfg.au32WDRGain[0] >> 8));

            au64CurSnsGain[0] = pstCurNode->stAERegCfg.u64ExposureSF;
            au64CurSnsGain[0] = osal_div_u64(au64CurSnsGain[0], DIV_0_TO_1(pstCurNode->stAERegCfg.u32IntTime[0]));
            au64CurSnsGain[0] = au64CurSnsGain[0] << 8;
            au64CurSnsGain[0] = osal_div_u64(au64CurSnsGain[0], DIV_0_TO_1((GK_U64)pstCurNode->stAERegCfg.u32IspDgain * pstCurNode->stAERegCfg.au32WDRGain[0] >> 8));

            pstSyncCfg->u64PreSnsGainSF = au64PreSnsGain[0];
            pstSyncCfg->u64CurSnsGainSF = au64CurSnsGain[0];

            au64PreSnsGain[1] = pstPreNode->stAERegCfg.u64Exposure;
            au64PreSnsGain[1] = osal_div_u64(au64PreSnsGain[1], DIV_0_TO_1(pstPreNode->stAERegCfg.u32IntTime[1]));
            au64PreSnsGain[1] = au64PreSnsGain[1] << 8;
            au64PreSnsGain[1] = osal_div_u64(au64PreSnsGain[1], DIV_0_TO_1((GK_U64)pstPreNode->stAERegCfg.u32IspDgain * pstPreNode->stAERegCfg.au32WDRGain[1] >> 8));

            au64CurSnsGain[1] = pstCurNode->stAERegCfg.u64Exposure;
            au64CurSnsGain[1] = osal_div_u64(au64CurSnsGain[1], DIV_0_TO_1(pstCurNode->stAERegCfg.u32IntTime[1]));
            au64CurSnsGain[1] = au64CurSnsGain[1] << 8;
            au64CurSnsGain[1] = osal_div_u64(au64CurSnsGain[1], DIV_0_TO_1((GK_U64)pstCurNode->stAERegCfg.u32IspDgain * pstCurNode->stAERegCfg.au32WDRGain[1] >> 8));

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

    return GK_SUCCESS;
}

GK_U8 isp_drv_get_fe_sync_index(ISP_DRV_CTX_S *drv_ctx)
{
    GK_U8 index;

    index = (drv_ctx->stSyncCfg.u8Cfg2VldDlyMAX - 1) / DIV_0_TO_1(drv_ctx->stSyncCfg.u8VCNumMax + 1);
    return index;
}

GK_U8 ISP_DRV_GetBeSyncIndex(ISP_DRV_CTX_S *pstDrvCtx)
{
    GK_U8 index;

    if (IS_ONLINE_MODE(pstDrvCtx->stWorkMode.enIspRunningMode) ||
        IS_SIDEBYSIDE_MODE(pstDrvCtx->stWorkMode.enIspRunningMode)) {
        index = (pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX - 1) / DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
    } else {
        if (pstDrvCtx->stFrameIntAttr.enIntType == FRAME_INTERRUPT_START) {
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

GK_S32 ISP_DRV_RegConfigIsp(VI_PIPE ViPipe, ISP_DRV_CTX_S *pstDrvCtx)
{

    GK_S32  i;
    GK_U8   u8CfgNodeIdx, u8CfgNodeVC;
    GK_U32  u32IspDgain;
    GK_S32  s32Ret;
    GK_U32  u32Ratio[3] = { 0x40 };
    GK_U8   u8BlkDev, u8BlkNum;
    GK_U8   u8SnapBlkDev = 0;
    GK_U8   u8SnapBlkNum = 1;
    GK_U32  au32WDRGain[4]   = { 0x100, 0x100, 0x100, 0x100 };
    GK_U32  au32WDRWBGain[4] = { 0x100, 0x100, 0x100, 0x100 };
    GK_U32  au32WBViGain[4]  = { 0x100, 0x100, 0x100, 0x100 };
    GK_U16  au16CCM[9] = {
        0x01A7,  0x809D,  0x800A,
        0x804F,  0x018E,  0x803F,
        0x000B,  0x8098,  0x018D
    };
    ISP_AE_REG_CFG_2_S *pstAERegCfg = GK_NULL;
    ISP_AWB_REG_CFG_2_S *pstAWBRegCfg = GK_NULL;
    ISP_SYNC_CFG_BUF_NODE_S *pstCfgNode = GK_NULL;
    ISP_BE_REGS_ATTR_S stIspBeRegsAttr = { 0 };
    ISP_BE_REGS_ATTR_S stIspSnapBeRegsAttr = { 0 };
    S_ISPBE_REGS_TYPE *apstBeReg[ISP_STRIPING_MAX_NUM] = { GK_NULL };
    S_ISPBE_REGS_TYPE *apstSnapBeReg[ISP_STRIPING_MAX_NUM] = { GK_NULL };
    VI_PIPE MainPipeS;
    ISP_DRV_CTX_S *pstDrvCtxS = GK_NULL;
    VI_PIPE PicturePipe;
    ISP_DRV_CTX_S *pstDrvCtxPic = GK_NULL;
    ISP_RUNNING_MODE_E enPictureRunningMode = ISP_MODE_RUNNING_OFFLINE;
    GK_BOOL bOnlineHavePictruePipe = GK_FALSE;
    GK_U8   u8SyncIndex, u8ExpRatioIndex;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDrvCtx);

    s32Ret = ISP_DRV_GetBeRegsAttr(ViPipe, apstBeReg, &stIspBeRegsAttr);
    if (s32Ret != GK_SUCCESS) {
        return GK_SUCCESS;
    }

    if (pstDrvCtx->stSnapAttr.s32PicturePipeId >= 0) {
        PicturePipe = pstDrvCtx->stSnapAttr.s32PicturePipeId;
        pstDrvCtxPic = ISP_DRV_GET_CTX(PicturePipe);
        enPictureRunningMode = pstDrvCtxPic->stWorkMode.enIspRunningMode;

        if ((IS_ONLINE_MODE(enPictureRunningMode)) || \
            (IS_SIDEBYSIDE_MODE(enPictureRunningMode))) {
            if (PicturePipe != pstDrvCtx->stSnapAttr.s32PreviewPipeId) {
                bOnlineHavePictruePipe = GK_TRUE;
            }
        }
    }
    u8BlkDev = stIspBeRegsAttr.u8BlockDev;
    u8BlkNum = stIspBeRegsAttr.u8BlockNum;

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
            if (IS_OFFLINE_MODE(pstDrvCtx->stWorkMode.enIspRunningMode) || \
                IS_STRIPING_MODE(pstDrvCtx->stWorkMode.enIspRunningMode)) {
                u8ExpRatioIndex = u8SyncIndex + 2;
                u8ExpRatioIndex = MIN2(u8ExpRatioIndex, CFG2VLD_DLY_LIMIT - 1);
            }
        }

        for (i = 0; i < 3; i++) {
            u32Ratio[i] = pstDrvCtx->stSyncCfg.u32ExpRatio[i][u8ExpRatioIndex];
        }

        /* Cal CfgNodeIdx to ispDgain/ratio.. configs */
        if (IS_ONLINE_MODE(pstDrvCtx->stWorkMode.enIspRunningMode) || \
            IS_SIDEBYSIDE_MODE(pstDrvCtx->stWorkMode.enIspRunningMode)) {
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

    isp_drv_reg_config_fe_blc(ViPipe, pstDrvCtx);

    pstCfgNode = pstDrvCtx->stSyncCfg.apstNode[u8CfgNodeIdx];

    if (pstCfgNode != GK_NULL) {
        if (pstDrvCtx->stSyncCfg.u8VCCfgNum == u8CfgNodeVC) {
            isp_drv_reg_config_be_blc(ViPipe, apstBeReg, pstDrvCtx, pstCfgNode, &stIspBeRegsAttr);
            ISP_DRV_RegConfigWdr(ViPipe, apstBeReg, pstDrvCtx, &pstCfgNode->stWDRRegCfg, u8BlkNum, u8BlkDev, u32Ratio);

            /* config Ldci compensation */
            ISP_DRV_RegConfigLdci(apstBeReg, pstDrvCtx, u8BlkNum, u8BlkDev);
            ISP_DRV_RegConfigDe(ViPipe, apstBeReg, pstCfgNode);

            /* config drc strength */
            ISP_DRV_RegConfigDrc(ViPipe, apstBeReg, pstDrvCtx, &pstCfgNode->stDRCRegCfg, u8BlkNum, u8BlkDev, u32Ratio);
        }
    }

    if (bOnlineHavePictruePipe == GK_TRUE) {
        if (ViPipe == pstDrvCtx->stSnapAttr.s32PicturePipeId) {
            return GK_SUCCESS;
        }
    }
    /* config isp_dgain & drc strength */
    pstCfgNode = pstDrvCtx->stSyncCfg.apstNode[u8CfgNodeIdx];

    if (pstDrvCtx->stSnapInfoLoad.enSnapState == SNAP_STATE_CFG) {
        u32IspDgain = pstDrvCtx->stSnapInfoLoad.stIspCfgInfo.u32IspDgain;

        if (pstDrvCtx->stSnapAttr.bLoadCCM == GK_TRUE) {
            for (i = 0; i < CCM_MATRIX_SIZE; i++) {
                au16CCM[i] = pstDrvCtx->stSnapInfoLoad.stIspCfgInfo.au16CapCCM[i];
            }
        } else {
            if (pstCfgNode == GK_NULL) {
                return GK_SUCCESS;
            }

            for (i = 0; i < CCM_MATRIX_SIZE; i++) {
                au16CCM[i] = pstCfgNode->stAWBRegCfg.au16ColorMatrix[i];
            }
        }
    } else {
        if (pstCfgNode == GK_NULL) {
            return GK_SUCCESS;
        }

        if (pstDrvCtx->stStitchAttr.bStitchEnable == GK_TRUE) {
            if (pstDrvCtx->stStitchAttr.bMainPipe == GK_FALSE) {
                MainPipeS  = pstDrvCtx->stStitchAttr.as8StitchBindId[0];
                pstDrvCtxS = ISP_DRV_GET_CTX(MainPipeS);
                pstCfgNode = pstDrvCtxS->stSyncCfg.apstNode[u8CfgNodeIdx];

                if (pstCfgNode == GK_NULL) {
                    ISP_TRACE(MODULE_DBG_WARN, "pstCfgNode NULL point!\n");
                    return GK_SUCCESS;
                }
            }
        }

        if (pstDrvCtx->stSyncCfg.u8VCCfgNum == u8CfgNodeVC) {
            pstAERegCfg  = &pstCfgNode->stAERegCfg;
            pstAWBRegCfg = &pstCfgNode->stAWBRegCfg;
        }

        if (pstAERegCfg == GK_NULL) {
            return GK_SUCCESS;
        }

        if (pstAWBRegCfg == GK_NULL) {
            return GK_SUCCESS;
        }

        u32IspDgain = pstAERegCfg->u32IspDgain;

        for (i = 0; i < CCM_MATRIX_SIZE; i++) {
            if (pstAWBRegCfg != GK_NULL) {
                au16CCM[i] = pstAWBRegCfg->au16ColorMatrix[i];
            }
        }
    }

    u32IspDgain = CLIP3(u32IspDgain, ISP_DIGITAL_GAIN_MIN, ISP_DIGITAL_GAIN_MAX);
    if (bOnlineHavePictruePipe == GK_TRUE) {
        if (ViPipe == pstDrvCtx->stSnapAttr.s32PreviewPipeId) {
            ISP_CHECK_PIPE(pstDrvCtx->stSnapAttr.s32PicturePipeId);
            s32Ret = ISP_DRV_GetBeRegsAttr(pstDrvCtx->stSnapAttr.s32PicturePipeId,
                                           apstSnapBeReg, &stIspSnapBeRegsAttr);
            if (s32Ret == GK_SUCCESS) {
                u8SnapBlkDev = stIspSnapBeRegsAttr.u8BlockDev;
                u8SnapBlkNum = stIspSnapBeRegsAttr.u8BlockNum;
                ISP_DRV_RegConfigDgain(apstSnapBeReg, u32IspDgain, u8SnapBlkNum, u8SnapBlkDev);
            }
        }
    }

    for (i = 0; i < 4; i++) {
        au32WDRGain[i] = pstDrvCtx->stSyncCfg.u32WDRGain[i][u8ExpRatioIndex];
        au32WDRGain[i] = CLIP3(au32WDRGain[i], ISP_DIGITAL_GAIN_MIN, ISP_DIGITAL_GAIN_MAX);

        if (pstAWBRegCfg != GK_NULL) {
            au32WDRWBGain[i] = pstAWBRegCfg->au32WDRWBGain[i];
            au32WBViGain[i]  = pstAWBRegCfg->au32BEWhiteBalanceGain[i];
        }
    }

    ISP_DRV_RegConfigDgain(apstBeReg, u32IspDgain, u8BlkNum, u8BlkDev);

    ISP_DRV_ConfigCCM(pstDrvCtx, au16CCM);

    if (pstDrvCtx->stSnapInfoLoad.enSnapState == SNAP_STATE_CFG) {
        for (i = 0; i < 4; i++) {
            au32WBViGain[i]  = pstDrvCtx->stSnapInfoLoad.stIspCfgInfo.au32WhiteBalanceGain[i];
        }
        ISP_DRV_RegConfig4Dgain(ViPipe, apstBeReg, pstDrvCtx, au32WDRGain, u8BlkNum, u8BlkDev,
                                au32WDRWBGain, au32WBViGain);
        if ((g_UpdatePos[ViPipe] == 1) && (pstDrvCtx->stIntSch.u32IspIntStatus != 3)) {
            pstDrvCtx->stSnapInfoLoad.enSnapState = SNAP_STATE_NULL;
        }
    } else {
        if (ViPipe != pstDrvCtx->stSnapAttr.s32PicturePipeId) {
            ISP_DRV_RegConfig4Dgain(ViPipe, apstBeReg, pstDrvCtx, au32WDRGain, u8BlkNum, u8BlkDev,
                                    au32WDRWBGain, au32WBViGain);
        }
    }

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        pstDrvCtx->stIspBeSyncPara.au32IspDgain[i] = u32IspDgain;
    }

    isp_drv_reg_config_be_regup(ViPipe);

    pstCfgNode = pstDrvCtx->stSyncCfg.apstNode[0];

    if (pstCfgNode == GK_NULL) {
        return GK_SUCCESS;
    }

    /* config Piris */
    if (pstAERegCfg != GK_NULL) {
        if (pstAERegCfg->bPirisValid == GK_TRUE) {
            if (pstDrvCtx->stPirisCb.pfn_piris_gpio_update != GK_NULL) {
                pstDrvCtx->stPirisCb.pfn_piris_gpio_update(ViPipe, &pstAERegCfg->s32PirisPos);
            }
        }
    }
    return GK_SUCCESS;
}

ISP_SYNC_CFG_BUF_NODE_S *ISP_DRV_GetSnsRegConfigNode(ISP_DRV_CTX_S *pstDrvCtx, GK_U8 u8DelayFrmNum)
{
    GK_U8 u8WDRMode, u8CfgNodeIdx, u8CfgNodeVC;
    ISP_SYNC_CFG_BUF_NODE_S *pstCfgNode = GK_NULL;
    ISP_SYNC_CFG_BUF_NODE_S *pstPreCfgNode = GK_NULL;

    u8WDRMode = pstDrvCtx->stWDRCfg.u8WDRMode;
    u8CfgNodeIdx = u8DelayFrmNum / DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
    u8CfgNodeVC  = u8DelayFrmNum % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);

    if (pstDrvCtx->stSyncCfg.u8VCCfgNum == u8CfgNodeVC) {
        if (u8CfgNodeIdx > CFG2VLD_DLY_LIMIT - 1) {
            return GK_NULL;
        }

        pstCfgNode    = pstDrvCtx->stSyncCfg.apstNode[u8CfgNodeIdx];
        pstPreCfgNode = pstDrvCtx->stSyncCfg.apstNode[u8CfgNodeIdx + 1];

        if (pstCfgNode == GK_NULL) {
            return GK_NULL;
        }

        if (pstPreCfgNode == GK_NULL) {
        } else {
            if ((IS_LINEAR_MODE(u8WDRMode)) || (IS_BUILT_IN_WDR_MODE(u8WDRMode))) {
                /* not config sensor when cur == pre */
                if (pstCfgNode == pstPreCfgNode) {
                    return GK_NULL;
                }
            }
        }
    }

    return pstCfgNode;
}

GK_S32 ISP_DRV_StitchRegsCfgSensor(VI_PIPE ViPipe, ISP_DRV_CTX_S *pstDrvCtx, GK_U32 u32UpdatePos)
{
    GK_S32 i;
    GK_U32 u32SlaveDev;
    VI_PIPE MainPipeS = 0;
    ISP_SYNC_CFG_BUF_NODE_S *pstCurNode = GK_NULL;
    ISP_SYNC_CFG_BUF_NODE_S *pstCfgNode = GK_NULL;

    ISP_I2C_DATA_S *pstI2cData = GK_NULL;
    ISP_SSP_DATA_S *pstSspData = GK_NULL;
    GK_S8 s8I2cDev, s8SspDev, s8SspCs;

    pstCurNode = pstDrvCtx->stSyncCfg.apstNode[0];

    if (pstCurNode == GK_NULL) {
        ISP_TRACE(MODULE_DBG_WARN, "NULL point Stitch!\n");
        return GK_FAILURE;
    }

    if (pstCurNode->bValid == GK_FALSE) {
        ISP_TRACE(MODULE_DBG_WARN, "Invalid Node Stitch!\n");
        return GK_FAILURE;
    }

    if (pstCurNode->stSnsRegsInfo.u32RegNum == 0) {
        ISP_TRACE(MODULE_DBG_WARN, "Err u32RegNum Stitch!\n");
        return GK_FAILURE;
    }

    u32SlaveDev = pstCurNode->stSnsRegsInfo.stSlvSync.u32SlaveBindDev;

    if (pstCurNode->stSnsRegsInfo.enSnsType == ISP_SNS_I2C_TYPE) {
        s8I2cDev = pstCurNode->stSnsRegsInfo.unComBus.s8I2cDev;

        if (s8I2cDev == -1) {
            return 0;
        }

        if (pstDrvCtx->stStitchAttr.bMainPipe == GK_FALSE) {
            MainPipeS  = pstDrvCtx->stStitchAttr.as8StitchBindId[0];
            pstDrvCtx  = ISP_DRV_GET_CTX(MainPipeS);
            pstCurNode = pstDrvCtx->stSyncCfg.apstNode[0];

            if (pstCurNode == GK_NULL) {
                ISP_TRACE(MODULE_DBG_WARN, "pstCurNodeS NULL point Stitch!\n");
                return GK_FAILURE;
            }
        }

        for (i = 0; i < pstCurNode->stSnsRegsInfo.u32RegNum; i++) {
            pstCfgNode = ISP_DRV_GetSnsRegConfigNode(pstDrvCtx, pstCurNode->stSnsRegsInfo.astI2cData[i].u8DelayFrmNum);

            if (!pstCfgNode) {
                continue;
            }

            if (pstCfgNode->bValid == GK_TRUE) {
                pstI2cData = &pstCfgNode->stSnsRegsInfo.astI2cData[i];
                if ((pstI2cData->bUpdate == GK_TRUE) && (u32UpdatePos == pstI2cData->u8IntPos)) {
                    if (pstDrvCtx->stBusCb.pfnISPWriteI2CData != GK_NULL) {
                        pstDrvCtx->stBusCb.pfnISPWriteI2CData(s8I2cDev, pstI2cData->u8DevAddr,
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

        if (pstDrvCtx->stStitchAttr.bMainPipe == GK_FALSE) {
            MainPipeS  = pstDrvCtx->stStitchAttr.as8StitchBindId[0];
            pstDrvCtx  = ISP_DRV_GET_CTX(MainPipeS);
            pstCurNode = pstDrvCtx->stSyncCfg.apstNode[0];

            if (pstCurNode == GK_NULL) {
                ISP_TRACE(MODULE_DBG_WARN, "pstCurNodeS NULL point Stitch!\n");
                return GK_FAILURE;
            }
        }

        for (i = 0; i < pstCurNode->stSnsRegsInfo.u32RegNum; i++) {
            pstCfgNode = ISP_DRV_GetSnsRegConfigNode(pstDrvCtx, pstCurNode->stSnsRegsInfo.astSspData[i].u8DelayFrmNum);

            if (!pstCfgNode) {
                continue;
            }

            if (pstCfgNode->bValid == GK_TRUE) {
                pstSspData = &pstCfgNode->stSnsRegsInfo.astSspData[i];

                if ((pstSspData->bUpdate == GK_TRUE) && (u32UpdatePos == pstSspData->u8IntPos)) {
                    if (pstDrvCtx->stBusCb.pfnISPWriteSSPData != GK_NULL) {
                        pstDrvCtx->stBusCb.pfnISPWriteSSPData(s8SspDev, s8SspCs, pstSspData->u32DevAddr,
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

    if (pstCfgNode == GK_NULL) {
        return GK_FAILURE;
    }

    if ((pstCfgNode->bValid == GK_TRUE) && (pstCfgNode->stSnsRegsInfo.stSlvSync.bUpdate == GK_TRUE)) {
        /* adjust the relationship between slavedev and vipipe */
        IO_RW_PT_ADDRESS(VICAP_SLAVE_VSTIME(u32SlaveDev)) = pstCfgNode->stSnsRegsInfo.stSlvSync.u32SlaveVsTime;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_NormalRegsCfgSensor(VI_PIPE ViPipe, ISP_DRV_CTX_S *pstDrvCtx)
{
    GK_S32 i = 0;
    GK_U32 u32SlaveDev = 0;
    ISP_SYNC_CFG_BUF_NODE_S *pstCurNode = GK_NULL;
    ISP_SYNC_CFG_BUF_NODE_S *pstCfgNode = GK_NULL;
    ISP_I2C_DATA_S *pstI2cData = GK_NULL;
    ISP_SSP_DATA_S *pstSspData = GK_NULL;
    GK_S8 s8I2cDev, s8SspDev, s8SspCs;

    pstCurNode = pstDrvCtx->stSyncCfg.apstNode[0];

    if (pstCurNode == GK_NULL) {
        ISP_TRACE(MODULE_DBG_WARN, "NULL point Normal!\n");
        return GK_FAILURE;
    }

    if (pstCurNode->bValid == GK_FALSE) {
        ISP_TRACE(MODULE_DBG_WARN, "Invalid Node Normal!\n");
        return GK_FAILURE;
    }

    if (pstCurNode->stSnsRegsInfo.u32RegNum == 0) {
        ISP_TRACE(MODULE_DBG_WARN, "Err u32RegNum Normal!\n");
        return GK_FAILURE;
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

            if (pstCfgNode->bValid == GK_TRUE) {
                pstI2cData = &pstCfgNode->stSnsRegsInfo.astI2cData[i];

                if ((pstI2cData->bUpdate == GK_TRUE) && (pstDrvCtx->u32IntPos == pstI2cData->u8IntPos)) {
                    if (pstDrvCtx->stBusCb.pfnISPWriteI2CData != GK_NULL) {
                        pstDrvCtx->stBusCb.pfnISPWriteI2CData(s8I2cDev, pstI2cData->u8DevAddr,
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

            if (pstCfgNode->bValid == GK_TRUE) {
                pstSspData = &pstCfgNode->stSnsRegsInfo.astSspData[i];

                if ((pstSspData->bUpdate == GK_TRUE) && (pstDrvCtx->u32IntPos == pstSspData->u8IntPos)) {
                    if (pstDrvCtx->stBusCb.pfnISPWriteSSPData != GK_NULL) {
                        pstDrvCtx->stBusCb.pfnISPWriteSSPData(s8SspDev, s8SspCs, pstSspData->u32DevAddr,
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

    if (pstCfgNode == GK_NULL) {
        return GK_FAILURE;
    }

    if ((pstCfgNode->bValid == GK_TRUE) && (pstCfgNode->stSnsRegsInfo.stSlvSync.bUpdate == GK_TRUE)) {
        /* adjust the relationship between slavedev and vipipe */
        u32SlaveDev = pstCfgNode->stSnsRegsInfo.stSlvSync.u32SlaveBindDev;
        IO_RW_PT_ADDRESS(VICAP_SLAVE_VSTIME(u32SlaveDev)) = pstCfgNode->stSnsRegsInfo.stSlvSync.u32SlaveVsTime;
    }

    return GK_SUCCESS;
}


GK_S32 ISP_DRV_RegConfigSensor(VI_PIPE ViPipe, ISP_DRV_CTX_S *pstDrvCtx)
{
    GK_S32 s32Ret;
    volatile  GK_U32 u32UpdatePos;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDrvCtx);

    u32UpdatePos = pstDrvCtx->u32IntPos;

    if (pstDrvCtx->stStitchAttr.bStitchEnable == GK_TRUE) {
        s32Ret = ISP_DRV_StitchRegsCfgSensor(ViPipe, pstDrvCtx, u32UpdatePos);
        if (s32Ret != GK_SUCCESS) {
            ISP_TRACE(MODULE_DBG_WARN, "ISP_DRV_StitchRegsCfgSensor failure!\n");
            return s32Ret;
        }
    } else {
        s32Ret = ISP_DRV_NormalRegsCfgSensor(ViPipe, pstDrvCtx);
        if (s32Ret != GK_SUCCESS) {
            ISP_TRACE(MODULE_DBG_WARN, "ISP_DRV_NormalRegsCfgSensor failure!\n");
            return s32Ret;
        }
    }

    return GK_SUCCESS;
}

GK_S32 ISP_RegisterBusCallBack(VI_PIPE ViPipe,
                               ISP_BUS_TYPE_E enType, ISP_BUS_CALLBACK_S *pstBusCb)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    unsigned long u32Flags;

    ISP_CHECK_POINTER(pstBusCb);
    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    if (enType == ISP_BUS_TYPE_I2C) {
        pstDrvCtx->stBusCb.pfnISPWriteI2CData = pstBusCb->pfnISPWriteI2CData;
    } else if (enType == ISP_BUS_TYPE_SSP) {
        pstDrvCtx->stBusCb.pfnISPWriteSSPData = pstBusCb->pfnISPWriteSSPData;
    } else {
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        osal_printk("The bus type %d registerd to isp is err!\n", enType);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
    return GK_SUCCESS;
}

GK_S32 ISP_RegisterPirisCallBack(VI_PIPE ViPipe, ISP_PIRIS_CALLBACK_S *pstPirisCb)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    unsigned long u32Flags;

    ISP_CHECK_POINTER(pstPirisCb);
    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstDrvCtx->stPirisCb.pfn_piris_gpio_update = pstPirisCb->pfn_piris_gpio_update;
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

GK_S32 ISP_RegisterViBusCallBack(GK_S32 ViPipe, ISP_VIBUS_CALLBACK_S *pstViBusCb)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    unsigned long u32Flags;
    ISP_CHECK_POINTER(pstViBusCb);
    ISP_CHECK_PIPE(ViPipe);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstDrvCtx->stViBusCb.pfnIspBeStaRead = pstViBusCb->pfnIspBeStaRead;
    pstDrvCtx->stViBusCb.pfnIspBeCfgRead = pstViBusCb->pfnIspBeCfgRead;
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

/* vi get isp_config from isp */
GK_S32 ISP_SaveSnapConfig(VI_PIPE ViPipe, ISP_CONFIG_INFO_S *pstSnapInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    unsigned long u32Flags;
    GK_U8 u8CfgDlyMax;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstSnapInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    u8CfgDlyMax = MAX2(pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX, 2);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    if (g_UpdatePos[ViPipe] == 0) {  /* frame start */
        osal_memcpy(pstSnapInfo, &pstDrvCtx->astSnapInfoSave[u8CfgDlyMax - 1], sizeof(ISP_CONFIG_INFO_S));
    } else {                 /* frame end */
        osal_memcpy(pstSnapInfo, &pstDrvCtx->astSnapInfoSave[u8CfgDlyMax - 2], sizeof(ISP_CONFIG_INFO_S));
    }
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

/* vi send Proenable */
GK_S32 ISP_SetProEnable(VI_PIPE ViPipe)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    unsigned long u32Flags;
    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstDrvCtx->bProEnable = GK_TRUE;
    pstDrvCtx->bProStart  = GK_FALSE;
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
    return GK_SUCCESS;
}

/* vi get pubAttr */
GK_S32 ISP_GetPubAttr(VI_PIPE ViPipe, ISP_PUB_ATTR_S *pstPubAttr)
{
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstPubAttr);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    if(!pstDrvCtx->bIspInit)
    {
        return GK_FAILURE;
    }
    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    osal_memcpy(pstPubAttr, &pstDrvCtx->stProcPubInfo, sizeof(ISP_PUB_ATTR_S));
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

/* vi get ProCtrl */
GK_BOOL ISP_GetProCtrl(VI_PIPE ViPipe, ISP_PRO_CTRL_S *pstProCtrl)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    unsigned long u32Flags;
    GK_U8 u8CfgDlyMax;
    GK_BOOL bProStart = GK_FALSE;

    if ((ViPipe < 0) || (ViPipe >= ISP_MAX_PIPE_NUM)) {
        ISP_TRACE(MODULE_DBG_ERR, "Err isp pipe %d!\n", ViPipe);
        return bProStart;
    }

    if (pstProCtrl == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "Null Pointer!\n");
        return bProStart;
    }

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    u8CfgDlyMax = MAX2(pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX, 1);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    if (g_UpdatePos[ViPipe] == 0) {
        pstProCtrl->u8Vcnum = u8CfgDlyMax;
    } else {
        pstProCtrl->u8Vcnum = u8CfgDlyMax - 1;
    }
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    if (pstDrvCtx->u32ProTrigFlag == 2) {
        bProStart = pstDrvCtx->bProStart;
    } else {
        bProStart = GK_FALSE;
    }

    return bProStart;
}

GK_S32 ISP_SetSnapAttr(VI_PIPE ViPipe, ISP_SNAP_ATTR_S *pstSnapAttr)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    unsigned long u32Flags;
    GK_U8 i;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstSnapAttr);

    for (i = 0; i < ISP_MAX_PIPE_NUM; i++) {
        pstDrvCtx = ISP_DRV_GET_CTX(i);

        if (!pstDrvCtx->bMemInit) {
            continue;
        }

        if ((i == pstSnapAttr->s32PicturePipeId) || (i == pstSnapAttr->s32PreviewPipeId)) {
            osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
            osal_memcpy(&pstDrvCtx->stSnapAttr, pstSnapAttr, sizeof(ISP_SNAP_ATTR_S));
            osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        }
    }
    return GK_SUCCESS;
}

static GK_S32 ISP_CheckProNrParam(ISP_NR_AUTO_ATTR_S *pstNrAttr)
{
    GK_U8 i;

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (pstNrAttr->au8ChromaStr[0][i] > 3) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid  au8ChromaStr[0][%d] Input!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstNrAttr->au8ChromaStr[1][i] > 3) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid  au8ChromaStr[1][%d] Input!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstNrAttr->au8ChromaStr[2][i] > 3) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid  au8ChromaStr[2][%d] Input!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstNrAttr->au8ChromaStr[3][i] > 3) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid  au8ChromaStr[3][%d] Input!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstNrAttr->au16CoarseStr[0][i] > 0x360) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid  au16CoarseStr[0][%d] Input!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstNrAttr->au16CoarseStr[1][i] > 0x360) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid  au16CoarseStr[1][%d] Input!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstNrAttr->au16CoarseStr[2][i] > 0x360) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid  au16CoarseStr[2][%d] Input!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstNrAttr->au16CoarseStr[3][i] > 0x360) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid  au16CoarseStr[3][%d] Input!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstNrAttr->au8FineStr[i] > 128) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid  au8FineStr[%d] Input!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstNrAttr->au16CoringWgt[i] > 3200) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid  au16CoringWgt[%d] Input!\n", i);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
    }

    return GK_SUCCESS;
}

GK_S32 ISP_SetProNrParam(VI_PIPE ViPipe, const ISP_PRO_BNR_PARAM_S *pstProNrParam)
{
    GK_S32 s32Ret;
    GK_U32 u32ParamIdex = 0;
    GK_U32 u32ParamNum;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    ISP_PRO_NR_PARAM_S *pstProNrParamCtx = GK_NULL;
    ISP_NR_AUTO_ATTR_S *pstProNrAttr = GK_NULL;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstProNrParam);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstProNrParam->pastNrAttr == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "NrParam buf address can't null!\n");
        s32Ret =  ERR_CODE_ISP_NULL_PTR;
        goto failed0;
    }

    s32Ret = read_user_linear_space_valid((GK_U8 *)(pstProNrParam->pastNrAttr), sizeof(ISP_NR_AUTO_ATTR_S));
    if (s32Ret == GK_FALSE) {
        ISP_TRACE(MODULE_DBG_ERR, "Pipe:%d pastShpAttr address is invalid!\n", ViPipe);
        s32Ret =  ERR_CODE_ISP_INVALID_ADDR;
        goto failed0;
    }

    ISP_CHECK_BOOL(pstProNrParam->bEnable);

    u32ParamNum = pstProNrParam->u32ParamNum;

    if ((u32ParamNum > PRO_MAX_FRAME_NUM) || (u32ParamNum < 1)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u32ParamNum value %d! Value range: [1, %d]\n", u32ParamNum, PRO_MAX_FRAME_NUM);
        s32Ret = ERR_CODE_ISP_ILLEGAL_PARAM;
        goto failed0;
    }

    pstProNrAttr = osal_vmalloc(sizeof(ISP_NR_AUTO_ATTR_S) * u32ParamNum);
    if (pstProNrAttr == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "Pipe:%d vmalloc for pstProNrAttr failed!\n", ViPipe);
        s32Ret = ERR_CODE_ISP_NOMEM;
        goto failed0;
    }

    s32Ret = osal_copy_from_user(pstProNrAttr, pstProNrParam->pastNrAttr, sizeof(ISP_NR_AUTO_ATTR_S) * u32ParamNum);
    if (s32Ret != 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Pipe:%d,copy from user faild !\n", ViPipe);
        s32Ret = GK_FAILURE;
        goto failed0;
    }

    for (u32ParamIdex = 0; u32ParamIdex < u32ParamNum; u32ParamIdex++) {
        s32Ret = ISP_CheckProNrParam(&pstProNrAttr[u32ParamIdex]);
        if (s32Ret != GK_SUCCESS) {
            goto failed0;
        }
    }

    if (osal_down_interruptible(&pstDrvCtx->stProInfoSem)) {
        s32Ret = -ERESTARTSYS;
        goto failed0;
    }

    if (pstDrvCtx->stProInfo.bInit == GK_FALSE) {
        ISP_TRACE(MODULE_DBG_WARN, "ProInfo buf doesn't initialized!\n");
        s32Ret = ERR_CODE_ISP_NOT_INIT;
        goto failed1;
    }

    if (pstDrvCtx->stProInfo.stProNrParamInfo.pVirAddr == GK_NULL) {
        ISP_TRACE(MODULE_DBG_WARN, "ShpParam buf don't init ok!\n");
        s32Ret = ERR_CODE_ISP_NOT_INIT;
        goto failed1;
    }

    pstProNrParamCtx = (ISP_PRO_NR_PARAM_S *)pstDrvCtx->stProInfo.stProNrParamInfo.pVirAddr;

    pstProNrParamCtx->bEnable = pstProNrParam->bEnable;
    pstProNrParamCtx->u32ParamNum = u32ParamNum;
    osal_memcpy(&pstProNrParamCtx->astNrAttr, pstProNrAttr, sizeof(ISP_NR_AUTO_ATTR_S) * u32ParamNum);

    osal_up(&pstDrvCtx->stProInfoSem);

    if (pstProNrAttr != GK_NULL) {
        osal_vfree(pstProNrAttr);
    }

    return GK_SUCCESS;

failed1:
    osal_up(&pstDrvCtx->stProInfoSem);
failed0:
    if (pstProNrAttr != GK_NULL) {
        osal_vfree(pstProNrAttr);
    }

    return s32Ret;
}

GK_S32 ISP_GetProNrParam(VI_PIPE ViPipe, ISP_PRO_BNR_PARAM_S *pstProNrParam)
{
    GK_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    ISP_PRO_NR_PARAM_S *pstProNrParamCtx = GK_NULL;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstProNrParam);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstProNrParam->pastNrAttr == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "NrParam buf address can't null!\n");
        return ERR_CODE_ISP_NULL_PTR;
    }
    s32Ret = write_user_linear_space_valid((GK_U8 *)(pstProNrParam->pastNrAttr), sizeof(ISP_NR_AUTO_ATTR_S));
    if (s32Ret == GK_FALSE) {
        ISP_TRACE(MODULE_DBG_ERR, "Pipe:%d pastNrAttr address is invalid!\n", ViPipe);
        return ERR_CODE_ISP_INVALID_ADDR;
    }

    if (osal_down_interruptible(&pstDrvCtx->stProInfoSem)) {
        return -ERESTARTSYS;
    }

    if (pstDrvCtx->stProInfo.bInit == GK_FALSE) {
        ISP_TRACE(MODULE_DBG_WARN, "ProInfo buf doesn't initialized!\n");
        osal_up(&pstDrvCtx->stProInfoSem);
        return ERR_CODE_ISP_NOT_INIT;
    }

    if (pstDrvCtx->stProInfo.stProNrParamInfo.pVirAddr == GK_NULL) {
        ISP_TRACE(MODULE_DBG_WARN, "ShpParam buf don't init ok!\n");
        osal_up(&pstDrvCtx->stProInfoSem);
        return ERR_CODE_ISP_NOT_INIT;
    }

    pstProNrParamCtx = (ISP_PRO_NR_PARAM_S *)pstDrvCtx->stProInfo.stProNrParamInfo.pVirAddr;

    if (pstProNrParamCtx->u32ParamNum > PRO_MAX_FRAME_NUM) {
        pstProNrParamCtx->u32ParamNum = PRO_MAX_FRAME_NUM;
    }

    if (pstProNrParamCtx->u32ParamNum < 1) {
        pstProNrParamCtx->u32ParamNum = 1;
    }

    pstProNrParam->bEnable     = pstProNrParamCtx->bEnable;
    pstProNrParam->u32ParamNum = pstProNrParamCtx->u32ParamNum;

    s32Ret = osal_copy_to_user(pstProNrParam->pastNrAttr, &pstProNrParamCtx->astNrAttr, \
                               sizeof(ISP_NR_AUTO_ATTR_S) * pstProNrParamCtx->u32ParamNum);
    if (s32Ret != 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Pipe:%d,copy to user faild !\n", ViPipe);
        osal_up(&pstDrvCtx->stProInfoSem);
        return GK_FAILURE;
    }

    osal_up(&pstDrvCtx->stProInfoSem);

    return GK_SUCCESS;
}

static GK_S32 ISP_CheckProShpParam(ISP_SHARPEN_AUTO_ATTR_S *pstShpAttr)
{
    GK_U8 i, j;

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
            if (pstShpAttr->au16TextureStr[j][i] > 4095) {
                ISP_TRACE(MODULE_DBG_ERR, "Invalid au16TextureStr[%d][%d]:%d! Range:[0, 4095]\n",
                          j, i, pstShpAttr->au16TextureStr[j][i]);
                return ERR_CODE_ISP_ILLEGAL_PARAM;
            }
            if (pstShpAttr->au16EdgeStr[j][i] > 4095) {
                ISP_TRACE(MODULE_DBG_ERR, "Invalid au16EdgeStr[%d][%d]:%d! Range:[0, 4095]\n",
                          j, i, pstShpAttr->au16EdgeStr[j][i]);
                return ERR_CODE_ISP_ILLEGAL_PARAM;
            }
            if (pstShpAttr->au8LumaWgt[j][i] > 127) {
                ISP_TRACE(MODULE_DBG_ERR, "Invalid au16EdgeStr[%d][%d]:%d! Range:[0, 127]\n",
                          j, i, pstShpAttr->au8LumaWgt[j][i]);
                return ERR_CODE_ISP_ILLEGAL_PARAM;
            }
        }
        if (pstShpAttr->au16TextureFreq[i] > 4095) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au16TextureFreq[%d]:%d! Range:[0, 4095]\n",
                      i, pstShpAttr->au16TextureFreq[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstShpAttr->au16EdgeFreq[i] > 4095) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au16EdgeFreq[%d]:%d! Range:[0, 4095]\n", i, pstShpAttr->au16EdgeFreq[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstShpAttr->au8OverShoot[i] > 127) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au8OverShoot[%d]:%d! Range:[0, 127]\n", i, pstShpAttr->au8OverShoot[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        if (pstShpAttr->au8UnderShoot[i] > 127) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au8UnderShoot[%d]:%d! Range:[0, 127]\n", i, pstShpAttr->au8UnderShoot[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstShpAttr->au8EdgeFiltStr[i] > 63) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au8EdgeFiltStr[%d]:%d! Range:[0, 63]\n", i, pstShpAttr->au8EdgeFiltStr[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstShpAttr->au8RGain[i] > ISP_SHARPEN_RGAIN) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au8RGain[%d]:%d! Range:[0, %d]\n",
                      i, pstShpAttr->au8RGain[i], ISP_SHARPEN_RGAIN);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstShpAttr->au8BGain[i] > ISP_SHARPEN_BGAIN) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au8BGain[%d]:%d! Range:[0, %d]\n",
                      i, pstShpAttr->au8BGain[i], ISP_SHARPEN_BGAIN);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }

        if (pstShpAttr->au8SkinGain[i] > 31) {
            ISP_TRACE(MODULE_DBG_ERR, "Invalid au8SkinGain[%d]:%d! Range:[0, 31]\n", i, pstShpAttr->au8SkinGain[i]);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
    }

    return GK_SUCCESS;
}

GK_S32 ISP_SetProShpParam(VI_PIPE ViPipe, const ISP_PRO_SHARPEN_PARAM_S *pstProShpParam)
{
    GK_S32 s32Ret;
    GK_U32 u32ParamIdex = 0;
    GK_U32 u32ParamNum;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    ISP_PRO_SHP_PARAM_S *pstProShpParamCtx = GK_NULL;
    ISP_SHARPEN_AUTO_ATTR_S *pastProShpAttr = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstProShpParam);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstProShpParam->pastShpAttr == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "ShpParam buf address can't null!\n");
        s32Ret = ERR_CODE_ISP_NULL_PTR;
        goto failed0;
    }

    s32Ret = read_user_linear_space_valid((GK_U8 *)(pstProShpParam->pastShpAttr), sizeof(ISP_SHARPEN_AUTO_ATTR_S));
    if (s32Ret == GK_FALSE) {
        ISP_TRACE(MODULE_DBG_ERR, "Pipe:%d pastShpAttr address is invalid!\n", ViPipe);
        s32Ret = ERR_CODE_ISP_INVALID_ADDR;
        goto failed0;
    }

    ISP_CHECK_BOOL(pstProShpParam->bEnable);

    u32ParamNum = pstProShpParam->u32ParamNum;

    if ((u32ParamNum > PRO_MAX_FRAME_NUM) || (u32ParamNum < 1)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid u32ParamNum value %d! Value range of u32ParamNum is [1, %d]\n",
                  u32ParamNum, PRO_MAX_FRAME_NUM);
        s32Ret = ERR_CODE_ISP_ILLEGAL_PARAM;
        goto failed0;
    }

    pastProShpAttr = osal_vmalloc(sizeof(ISP_SHARPEN_AUTO_ATTR_S) * u32ParamNum);
    if (pastProShpAttr == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "Pipe:%d vmalloc for pastProShpAttr failed!\n", ViPipe);
        s32Ret = ERR_CODE_ISP_NOMEM;
        goto failed0;
    }

    s32Ret = osal_copy_from_user(pastProShpAttr, pstProShpParam->pastShpAttr,
                                 sizeof(ISP_SHARPEN_AUTO_ATTR_S) * u32ParamNum);

    if (s32Ret != 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Pipe:%d,copy from user faild !\n", ViPipe);
        s32Ret = GK_FAILURE;
        goto failed0;
    }

    for (u32ParamIdex = 0; u32ParamIdex < u32ParamNum; u32ParamIdex++) {
        s32Ret = ISP_CheckProShpParam(&pastProShpAttr[u32ParamIdex]);
        if (s32Ret != GK_SUCCESS) {
            goto failed0;
        }
    }

    if (osal_down_interruptible(&pstDrvCtx->stProInfoSem)) {
        s32Ret = -ERESTARTSYS;
        goto failed0;
    }

    if (pstDrvCtx->stProInfo.bInit == GK_FALSE) {
        ISP_TRACE(MODULE_DBG_WARN, "ProInfo buf doesn't initialized!\n");
        s32Ret = ERR_CODE_ISP_NOT_INIT;
        goto failed1;
    }

    if (pstDrvCtx->stProInfo.stProShpParamInfo.pVirAddr == GK_NULL) {
        ISP_TRACE(MODULE_DBG_WARN, "ShpParam buf don't init ok!\n");
        s32Ret = ERR_CODE_ISP_NOT_INIT;
        goto failed1;
    }

    pstProShpParamCtx = (ISP_PRO_SHP_PARAM_S *)pstDrvCtx->stProInfo.stProShpParamInfo.pVirAddr;

    pstProShpParamCtx->bEnable     = pstProShpParam->bEnable;
    pstProShpParamCtx->u32ParamNum = u32ParamNum;
    osal_memcpy(&pstProShpParamCtx->astShpAttr, pastProShpAttr, sizeof(ISP_SHARPEN_AUTO_ATTR_S) * u32ParamNum);

    osal_up(&pstDrvCtx->stProInfoSem);

    if (pastProShpAttr != GK_NULL) {
        osal_vfree(pastProShpAttr);
    }

    return GK_SUCCESS;

failed1:
    osal_up(&pstDrvCtx->stProInfoSem);
failed0:
    if (pastProShpAttr != GK_NULL) {
        osal_vfree(pastProShpAttr);
    }

    return s32Ret;
}

GK_S32 ISP_GetProShpParam(VI_PIPE ViPipe, ISP_PRO_SHARPEN_PARAM_S *pstProShpParam)
{
    GK_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    ISP_PRO_SHP_PARAM_S *pstProShpParamCtx = GK_NULL;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstProShpParam);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstProShpParam->pastShpAttr == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "ShpParam buf address can't null!\n");
        return ERR_CODE_ISP_NULL_PTR;
    }

    s32Ret = write_user_linear_space_valid((GK_U8 *)(pstProShpParam->pastShpAttr), sizeof(ISP_SHARPEN_AUTO_ATTR_S));
    if (s32Ret == GK_FALSE) {
        ISP_TRACE(MODULE_DBG_ERR, "Pipe:%d pastShpAttr address is invalid!\n", ViPipe);
        return ERR_CODE_ISP_INVALID_ADDR;
    }

    if (osal_down_interruptible(&pstDrvCtx->stProInfoSem)) {
        return -ERESTARTSYS;
    }

    if (pstDrvCtx->stProInfo.bInit == GK_FALSE) {
        ISP_TRACE(MODULE_DBG_WARN, "ProInfo buf doesn't initialized!\n");
        osal_up(&pstDrvCtx->stProInfoSem);
        return ERR_CODE_ISP_NOT_INIT;
    }

    if (pstDrvCtx->stProInfo.stProShpParamInfo.pVirAddr == GK_NULL) {
        ISP_TRACE(MODULE_DBG_WARN, "ShpParam buf don't init ok!\n");
        osal_up(&pstDrvCtx->stProInfoSem);
        return ERR_CODE_ISP_NOT_INIT;
    }

    pstProShpParamCtx = (ISP_PRO_SHP_PARAM_S *)pstDrvCtx->stProInfo.stProShpParamInfo.pVirAddr;

    if (pstProShpParamCtx->u32ParamNum > PRO_MAX_FRAME_NUM) {
        pstProShpParamCtx->u32ParamNum = PRO_MAX_FRAME_NUM;
    }

    if (pstProShpParamCtx->u32ParamNum <  1) {
        pstProShpParamCtx->u32ParamNum = 1;
    }

    pstProShpParam->bEnable     = pstProShpParamCtx->bEnable;
    pstProShpParam->u32ParamNum = pstProShpParamCtx->u32ParamNum;

    s32Ret = osal_copy_to_user(pstProShpParam->pastShpAttr, &pstProShpParamCtx->astShpAttr, \
                               sizeof(ISP_SHARPEN_AUTO_ATTR_S) * pstProShpParamCtx->u32ParamNum);
    if (s32Ret != 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Pipe:%d,copy to user faild !\n", ViPipe);
        osal_up(&pstDrvCtx->stProInfoSem);
        return GK_FAILURE;
    }

    osal_up(&pstDrvCtx->stProInfoSem);

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_BeEndIntProc(VI_PIPE ViPipe)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstDrvCtx->bRunOnceOk == GK_TRUE) {
        ISP_DRV_StatBufBusyPut(ViPipe);
        pstDrvCtx->bRunOnceOk = GK_FALSE;
    }

    if (pstDrvCtx->bYUVRunOnceOk == GK_TRUE) {
        ISP_DRV_StatBufBusyPut(ViPipe);
        pstDrvCtx->bYUVRunOnceOk = GK_FALSE;
    }

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstDrvCtx->bVdBeEnd = GK_TRUE;
    osal_wakeup(&pstDrvCtx->stIspWaitVdBeEnd);
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_ProcInit(VI_PIPE ViPipe, ISP_PROC_MEM_S *pstIspProcMem)
{
    GK_S32 s32Ret;
    GK_U64 u64PhyAddr;
    GK_U8  *pu8VirAddr;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    GK_CHAR acName[MAX_MMZ_NAMELEN] = { 0 };

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstIspProcMem);

    if (g_ProcParam[ViPipe] == 0) {
        return GK_SUCCESS;
    }

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_INIT(ViPipe, pstDrvCtx->stPorcMem.bInit);

    osal_snprintf(acName, sizeof(acName), "ISP[%d].Proc", ViPipe);

    s32Ret = CMPI_MmzMallocNocache(GK_NULL, acName, &u64PhyAddr, (GK_VOID **)&pu8VirAddr, ISP_PROC_SIZE);

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "alloc proc buf err\n");
        return ERR_CODE_ISP_NOMEM;
    }

    ((GK_CHAR *)pu8VirAddr)[0] = '\0';
    ((GK_CHAR *)pu8VirAddr)[ISP_PROC_SIZE - 1] = '\0';

    if (osal_down_interruptible(&pstDrvCtx->stProcSem)) {
        if (u64PhyAddr != 0) {
            CMPI_MmzFree(u64PhyAddr, pu8VirAddr);
        }
        return -ERESTARTSYS;
    }

    pstDrvCtx->stPorcMem.bInit          = GK_TRUE;
    pstDrvCtx->stPorcMem.u64ProcPhyAddr = u64PhyAddr;
    pstDrvCtx->stPorcMem.u32ProcSize    = ISP_PROC_SIZE;
    pstDrvCtx->stPorcMem.pProcVirtAddr  = (GK_VOID *)pu8VirAddr;

    osal_memcpy(pstIspProcMem, &pstDrvCtx->stPorcMem, sizeof(ISP_PROC_MEM_S));

    osal_up(&pstDrvCtx->stProcSem);

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_ProcExit(VI_PIPE ViPipe)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    GK_U64 u64PhyAddr;
    GK_U8  *pu8VirAddr;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_EXIT_STATE(ViPipe);

    if (g_ProcParam[ViPipe] == 0) {
        return GK_SUCCESS;
    }

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_EXIT(ViPipe, pstDrvCtx->stPorcMem.bInit);

    u64PhyAddr = pstDrvCtx->stPorcMem.u64ProcPhyAddr;
    pu8VirAddr = (GK_U8 *)pstDrvCtx->stPorcMem.pProcVirtAddr;

    if (osal_down_interruptible(&pstDrvCtx->stProcSem)) {
        return -ERESTARTSYS;
    }

    pstDrvCtx->stPorcMem.bInit          = GK_FALSE;
    pstDrvCtx->stPorcMem.u64ProcPhyAddr = 0;
    pstDrvCtx->stPorcMem.u32ProcSize    = 0;
    pstDrvCtx->stPorcMem.pProcVirtAddr  = GK_NULL;
    osal_up(&pstDrvCtx->stProcSem);

    if (u64PhyAddr != 0) {
        CMPI_MmzFree(u64PhyAddr, pu8VirAddr);
    }

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_ProcPrintf(VI_PIPE ViPipe, osal_proc_entry_t *s)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    GK_U32 u32ProcBufLen;
    const GK_CHAR *pszStr;
    GK_CHAR *pszBuf;

    ISP_CHECK_PIPE(ViPipe);

    if (g_ProcParam[ViPipe] == 0) {
        return GK_SUCCESS;
    }

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (osal_down_interruptible(&pstDrvCtx->stProcSem)) {
        return -ERESTARTSYS;
    }

    if (pstDrvCtx->stPorcMem.pProcVirtAddr != GK_NULL) {
        pszBuf = osal_kmalloc((PROC_PRT_SLICE_SIZE + 1), osal_gfp_atomic);

        if (!pszBuf) {
            ISP_TRACE(MODULE_DBG_ERR, "ISP_DRV_ProcPrintf malloc slice buf err\n");
            osal_up(&pstDrvCtx->stProcSem);
            return ERR_CODE_ISP_NULL_PTR;
        }

        pszBuf[PROC_PRT_SLICE_SIZE] = '\0';
        pszStr = (GK_CHAR *)pstDrvCtx->stPorcMem.pProcVirtAddr;
        u32ProcBufLen = osal_strlen((GK_CHAR *)pstDrvCtx->stPorcMem.pProcVirtAddr);

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

        osal_kfree((GK_VOID *)pszBuf);
    }

    osal_up(&pstDrvCtx->stProcSem);

    return GK_SUCCESS;
}

GK_S32 ISP_UpdateInfoSync(VI_PIPE ViPipe, ISP_DCF_UPDATE_INFO_S *pstIspUpdateInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    unsigned long u32Flags;
    GK_S32 i;
    ISP_DCF_UPDATE_INFO_S *pUpdateInfoVirAddr = GK_NULL;
    ISP_CHECK_PIPE(ViPipe);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    if (pstDrvCtx->stTransInfo.stUpdateInfo.pVirAddr == GK_NULL) {
        ISP_TRACE(MODULE_DBG_WARN, "UpdateInfo buf don't init ok!\n");
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return ERR_CODE_ISP_NOT_INIT;
    }

    pUpdateInfoVirAddr = (ISP_DCF_UPDATE_INFO_S *)pstDrvCtx->stTransInfo.stUpdateInfo.pVirAddr;

    for (i = ISP_MAX_UPDATEINFO_BUF_NUM - 1; i >= 1; i--) {
        osal_memcpy(pUpdateInfoVirAddr + i, pUpdateInfoVirAddr + i - 1, sizeof(ISP_DCF_UPDATE_INFO_S));
    }
    osal_memcpy(pUpdateInfoVirAddr, pstIspUpdateInfo, sizeof(ISP_DCF_UPDATE_INFO_S));
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

GK_S32 ISP_frameInfoSync(VI_PIPE ViPipe, ISP_FRAME_INFO_S *pstIspframeInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    unsigned long u32Flags;
    GK_S32 i;
    ISP_FRAME_INFO_S *pframeInfoVirAddr = GK_NULL;
    ISP_CHECK_PIPE(ViPipe);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    if (pstDrvCtx->stTransInfo.stFrameInfo.pVirAddr == GK_NULL) {
        ISP_TRACE(MODULE_DBG_WARN, "UpdateInfo buf don't init ok!\n");
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return ERR_CODE_ISP_NOT_INIT;
    }

    pframeInfoVirAddr = (ISP_FRAME_INFO_S *)pstDrvCtx->stTransInfo.stFrameInfo.pVirAddr;

    for (i = ISP_MAX_FRAMEINFO_BUF_NUM - 1; i >= 1; i--) {
        osal_memcpy(pframeInfoVirAddr + i, pframeInfoVirAddr + i - 1, sizeof(ISP_FRAME_INFO_S));
    }
    osal_memcpy(pframeInfoVirAddr, pstIspframeInfo, sizeof(ISP_FRAME_INFO_S));
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}


GK_S32 ISP_CalSyncInfoIndex(VI_PIPE ViPipe, GK_S32 *ps32Index)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    GK_U8 u8CfgDlyMax;
    ISP_CHECK_PIPE(ViPipe);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    u8CfgDlyMax = MAX2(pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX, 2);
    if (g_UpdatePos[ViPipe] == 0) {
        *ps32Index = u8CfgDlyMax - 1;
    } else {
        *ps32Index = u8CfgDlyMax - 2;
    }

    *ps32Index = CLIP3(*ps32Index, 0, CFG2VLD_DLY_LIMIT - 1);

    return GK_SUCCESS;
}

GK_S32 ISP_GetDCFInfo(VI_PIPE ViPipe, ISP_DCF_INFO_S *pstIspDCF)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    ISP_DCF_UPDATE_INFO_S *pstIspUpdateInfo = GK_NULL;
    ISP_DCF_CONST_INFO_S *pstIspDCFConstInfo = GK_NULL;
    unsigned long u32Flags;
    GK_S32 s32Index = 0;
    ISP_DCF_UPDATE_INFO_S *pUpdateInfoVirAddr = GK_NULL;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_TRANBUF_INIT(ViPipe);
    ISP_CHECK_POINTER(pstIspDCF);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    if (pstDrvCtx->stTransInfo.stUpdateInfo.pVirAddr == GK_NULL) {
        ISP_TRACE(MODULE_DBG_WARN, "UpdateInfo buf don't init ok!\n");
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return ERR_CODE_ISP_NOT_INIT;
    }

    pUpdateInfoVirAddr = (ISP_DCF_UPDATE_INFO_S *)pstDrvCtx->stTransInfo.stUpdateInfo.pVirAddr;
    ISP_CalSyncInfoIndex(ViPipe, &s32Index);

    pstIspUpdateInfo = pUpdateInfoVirAddr + s32Index;

    pstIspDCFConstInfo = (ISP_DCF_CONST_INFO_S *)(pUpdateInfoVirAddr + ISP_MAX_UPDATEINFO_BUF_NUM);

    osal_memcpy(&pstIspDCF->stIspDCFConstInfo, pstIspDCFConstInfo, sizeof(ISP_DCF_CONST_INFO_S));
    osal_memcpy(&pstIspDCF->stIspDCFUpdateInfo, pstIspUpdateInfo, sizeof(ISP_DCF_UPDATE_INFO_S));
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

GK_S32 ISP_SetDCFInfo(VI_PIPE ViPipe, ISP_DCF_INFO_S *pstIspDCF)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    ISP_DCF_UPDATE_INFO_S *pstIspUpdateInfo = GK_NULL;
    ISP_DCF_CONST_INFO_S *pstIspDCFConstInfo = GK_NULL;
    unsigned long u32Flags;
    GK_S32 s32Index = 0;
    ISP_DCF_UPDATE_INFO_S *pUpdateInfoVirAddr = GK_NULL;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_TRANBUF_INIT(ViPipe);
    ISP_CHECK_POINTER(pstIspDCF);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    if (pstDrvCtx->stTransInfo.stUpdateInfo.pVirAddr == GK_NULL) {
        ISP_TRACE(MODULE_DBG_WARN, "UpdateInfo buf don't init ok!\n");
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return ERR_CODE_ISP_NOT_INIT;
    }

    pUpdateInfoVirAddr = (ISP_DCF_UPDATE_INFO_S *)pstDrvCtx->stTransInfo.stUpdateInfo.pVirAddr;

    ISP_CalSyncInfoIndex(ViPipe, &s32Index);
    pstIspUpdateInfo = pUpdateInfoVirAddr + s32Index;

    pstIspDCFConstInfo = (ISP_DCF_CONST_INFO_S *)(pUpdateInfoVirAddr + ISP_MAX_UPDATEINFO_BUF_NUM);

    osal_memcpy(pstIspDCFConstInfo, &pstIspDCF->stIspDCFConstInfo, sizeof(ISP_DCF_CONST_INFO_S));
    osal_memcpy(pstIspUpdateInfo, &pstIspDCF->stIspDCFUpdateInfo, sizeof(ISP_DCF_UPDATE_INFO_S));
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

GK_S32 ISP_GetIspUpdateInfo(VI_PIPE ViPipe, ISP_DCF_UPDATE_INFO_S *pstIspUpdateInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    unsigned long u32Flags;
    ISP_DCF_UPDATE_INFO_S *pUpdateInfoVirAddr = GK_NULL;
    GK_S32 s32Index = 0;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_TRANBUF_INIT(ViPipe);
    ISP_CHECK_POINTER(pstIspUpdateInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    if (pstDrvCtx->stTransInfo.stUpdateInfo.pVirAddr == GK_NULL) {
        ISP_TRACE(MODULE_DBG_WARN, "UpdateInfo buf don't init ok!\n");
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return ERR_CODE_ISP_NOT_INIT;
    }

    pUpdateInfoVirAddr = (ISP_DCF_UPDATE_INFO_S *)pstDrvCtx->stTransInfo.stUpdateInfo.pVirAddr;

    ISP_CalSyncInfoIndex(ViPipe, &s32Index);
    osal_memcpy(pstIspUpdateInfo, pUpdateInfoVirAddr + s32Index, sizeof(ISP_DCF_UPDATE_INFO_S));

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

GK_S32 ISP_SetIspUpdateInfo(VI_PIPE ViPipe, ISP_DCF_UPDATE_INFO_S *pstIspUpdateInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    unsigned long u32Flags;
    GK_S32 s32Index = 0;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_TRANBUF_INIT(ViPipe);
    ISP_CHECK_POINTER(pstIspUpdateInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    if (pstDrvCtx->stTransInfo.stUpdateInfo.pVirAddr == GK_NULL) {
        ISP_TRACE(MODULE_DBG_WARN, "UpdateInfo buf don't init ok!\n");
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return ERR_CODE_ISP_NOT_INIT;
    }

    ISP_CalSyncInfoIndex(ViPipe, &s32Index);
    osal_memcpy((ISP_DCF_UPDATE_INFO_S *)pstDrvCtx->stTransInfo.stUpdateInfo.pVirAddr + s32Index, \
                pstIspUpdateInfo, sizeof(ISP_DCF_UPDATE_INFO_S));

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_ProInfoBufInit(VI_PIPE ViPipe, ISP_PRO_INFO_BUF_S *pstProInfo)
{
    GK_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    GK_U64 u64PhyAddr;
    GK_U64 u64Size = 0;
    GK_U8  *pu8VirAddr;
    GK_U32 u32SizeProNrParam, u32SizeProShpParam;
    GK_CHAR acName[MAX_MMZ_NAMELEN] = { 0 };

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstProInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_INIT(ViPipe, pstDrvCtx->stProInfo.bInit);

    osal_snprintf(acName, sizeof(acName), "ISP[%d].Pro", ViPipe);

    u32SizeProNrParam  = sizeof(ISP_PRO_NR_PARAM_S);
    u32SizeProShpParam = sizeof(ISP_PRO_SHP_PARAM_S);

    u64Size += u32SizeProNrParam;
    u64Size += u32SizeProShpParam;

    s32Ret = CMPI_MmzMallocNocache(GK_NULL, acName, &u64PhyAddr, (GK_VOID **)&pu8VirAddr, u64Size);

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] alloc ISP Pro info buf err\n", ViPipe);
        return ERR_CODE_ISP_NOMEM;
    }

    osal_memset(pu8VirAddr, 0, u64Size);

    if (osal_down_interruptible(&pstDrvCtx->stProInfoSem)) {
        if (u64PhyAddr != 0) {
            CMPI_MmzFree(u64PhyAddr, pu8VirAddr);
        }
        return -ERESTARTSYS;
    }
    pstDrvCtx->stProInfo.bInit                        = GK_TRUE;

    pstDrvCtx->stProInfo.stProNrParamInfo.u64PhyAddr  = u64PhyAddr;
    pstDrvCtx->stProInfo.stProNrParamInfo.pVirAddr    = (GK_VOID *)pu8VirAddr;

    pstDrvCtx->stProInfo.stProShpParamInfo.u64PhyAddr = \
            pstDrvCtx->stProInfo.stProNrParamInfo.u64PhyAddr + u32SizeProNrParam;;
    pstDrvCtx->stProInfo.stProShpParamInfo.pVirAddr   = \
            (GK_VOID *)((GK_U8 *)pstDrvCtx->stProInfo.stProNrParamInfo.pVirAddr + u32SizeProNrParam);

    osal_memcpy(pstProInfo, &pstDrvCtx->stProInfo, sizeof(ISP_PRO_INFO_BUF_S));

    osal_up(&pstDrvCtx->stProInfoSem);

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_ProInfoBufExit(VI_PIPE ViPipe)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    GK_U64 u64PhyAddr;
    GK_U8  *pu8VirAddr;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_EXIT_STATE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_EXIT(ViPipe, pstDrvCtx->stProInfo.bInit);

    u64PhyAddr = pstDrvCtx->stProInfo.stProNrParamInfo.u64PhyAddr;
    pu8VirAddr = (GK_U8 *)pstDrvCtx->stProInfo.stProNrParamInfo.pVirAddr;

    if (osal_down_interruptible(&pstDrvCtx->stProInfoSem)) {
        return -ERESTARTSYS;
    }

    pstDrvCtx->stProInfo.bInit                        = GK_FALSE;

    pstDrvCtx->stProInfo.stProNrParamInfo.u64PhyAddr  = 0;
    pstDrvCtx->stProInfo.stProNrParamInfo.pVirAddr    = GK_NULL;

    pstDrvCtx->stProInfo.stProShpParamInfo.u64PhyAddr = 0;
    pstDrvCtx->stProInfo.stProShpParamInfo.pVirAddr   = GK_NULL;

    osal_up(&pstDrvCtx->stProInfoSem);

    if (u64PhyAddr != 0) {
        CMPI_MmzFree(u64PhyAddr, pu8VirAddr);
    }

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_TransInfoBufInit(VI_PIPE ViPipe, ISP_TRANS_INFO_BUF_S *pstTransInfo)
{
    GK_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    GK_U64 u64PhyAddr;
    GK_U64 u64Size = 0;
    GK_U8  *pu8VirAddr;
    GK_U32 u32SizeDngInfo;
    GK_U32 u32SizeUpdateInfo, u32SizeFrameInfo;
    GK_U32 u32SizeAttachInfo, u32SizeColorGammut;
    GK_CHAR acName[MAX_MMZ_NAMELEN] = { 0 };
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstTransInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_INIT(ViPipe, pstDrvCtx->stTransInfo.bInit);

    osal_snprintf(acName, sizeof(acName), "ISP[%d].Trans", ViPipe);

    u32SizeDngInfo     = sizeof(DNG_IMAGE_STATIC_INFO_S);
    u32SizeAttachInfo  = sizeof(ISP_ATTACH_INFO_S);
    u32SizeColorGammut = sizeof(ISP_COLORGAMMUT_INFO_S);
    u32SizeFrameInfo   = sizeof(ISP_FRAME_INFO_S) * ISP_MAX_FRAMEINFO_BUF_NUM;
    u32SizeUpdateInfo  = sizeof(ISP_DCF_UPDATE_INFO_S) * ISP_MAX_UPDATEINFO_BUF_NUM + sizeof(ISP_DCF_CONST_INFO_S);

    u64Size += u32SizeDngInfo;
    u64Size += u32SizeAttachInfo;
    u64Size += u32SizeColorGammut;
    u64Size += u32SizeFrameInfo;
    u64Size += u32SizeUpdateInfo;

    s32Ret = CMPI_MmzMallocNocache(GK_NULL, acName, &u64PhyAddr, (GK_VOID **)&pu8VirAddr, u64Size);

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] alloc ISP Trans info buf err\n", ViPipe);
        return ERR_CODE_ISP_NOMEM;
    }

    osal_memset(pu8VirAddr, 0, u64Size);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    pstDrvCtx->stTransInfo.bInit                        = GK_TRUE;

    pstDrvCtx->stTransInfo.stDngInfo.u64PhyAddr         = u64PhyAddr;
    pstDrvCtx->stTransInfo.stDngInfo.pVirAddr           = (GK_VOID *)pu8VirAddr;

    pstDrvCtx->stTransInfo.stAttaInfo.u64PhyAddr        = \
            pstDrvCtx->stTransInfo.stDngInfo.u64PhyAddr + u32SizeDngInfo;
    pstDrvCtx->stTransInfo.stAttaInfo.pVirAddr          = \
            (GK_VOID *)((GK_U8 *)pstDrvCtx->stTransInfo.stDngInfo.pVirAddr + u32SizeDngInfo);

    pstDrvCtx->stTransInfo.stColorGammutInfo.u64PhyAddr = \
            pstDrvCtx->stTransInfo.stAttaInfo.u64PhyAddr + u32SizeAttachInfo;
    pstDrvCtx->stTransInfo.stColorGammutInfo.pVirAddr   = \
            (GK_VOID *)((GK_U8 *)pstDrvCtx->stTransInfo.stAttaInfo.pVirAddr + u32SizeAttachInfo);

    pstDrvCtx->stTransInfo.stFrameInfo.u64PhyAddr       = \
            pstDrvCtx->stTransInfo.stColorGammutInfo.u64PhyAddr + u32SizeColorGammut;;
    pstDrvCtx->stTransInfo.stFrameInfo.pVirAddr         = \
            (GK_VOID *)((GK_U8 *)pstDrvCtx->stTransInfo.stColorGammutInfo.pVirAddr + u32SizeColorGammut);

    pstDrvCtx->stTransInfo.stUpdateInfo.u64PhyAddr      = \
            pstDrvCtx->stTransInfo.stFrameInfo.u64PhyAddr + u32SizeFrameInfo;;
    pstDrvCtx->stTransInfo.stUpdateInfo.pVirAddr        = \
            (GK_VOID *)((GK_U8 *)pstDrvCtx->stTransInfo.stFrameInfo.pVirAddr + u32SizeFrameInfo);

    osal_memcpy(pstTransInfo, &pstDrvCtx->stTransInfo, sizeof(ISP_TRANS_INFO_BUF_S));

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_TransInfoBufExit(VI_PIPE ViPipe)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    GK_U64 u64PhyAddr;
    GK_U8  *pu8VirAddr;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_EXIT_STATE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    ISP_CHECK_BUF_EXIT(ViPipe, pstDrvCtx->stTransInfo.bInit);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    u64PhyAddr = pstDrvCtx->stTransInfo.stDngInfo.u64PhyAddr;
    pu8VirAddr = (GK_U8 *)pstDrvCtx->stTransInfo.stDngInfo.pVirAddr;

    pstDrvCtx->stTransInfo.bInit                        = GK_FALSE;

    pstDrvCtx->stTransInfo.stDngInfo.u64PhyAddr         = 0;
    pstDrvCtx->stTransInfo.stDngInfo.pVirAddr           = GK_NULL;

    pstDrvCtx->stTransInfo.stAttaInfo.u64PhyAddr        = 0;
    pstDrvCtx->stTransInfo.stAttaInfo.pVirAddr          = GK_NULL;

    pstDrvCtx->stTransInfo.stColorGammutInfo.u64PhyAddr = 0;
    pstDrvCtx->stTransInfo.stColorGammutInfo.pVirAddr   = GK_NULL;

    pstDrvCtx->stTransInfo.stFrameInfo.u64PhyAddr       = 0;
    pstDrvCtx->stTransInfo.stFrameInfo.pVirAddr         = GK_NULL;

    pstDrvCtx->stTransInfo.stUpdateInfo.u64PhyAddr      = 0;
    pstDrvCtx->stTransInfo.stUpdateInfo.pVirAddr        = GK_NULL;

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    if (u64PhyAddr != 0) {
        CMPI_MmzFree(u64PhyAddr, pu8VirAddr);
    }

    return GK_SUCCESS;
}

GK_S32 ISP_SetFrameInfo(VI_PIPE ViPipe, ISP_FRAME_INFO_S *pstIspFrame)
{
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    GK_S32 s32Index = 0;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_TRANBUF_INIT(ViPipe);
    ISP_CHECK_POINTER(pstIspFrame);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    if (pstDrvCtx->stTransInfo.stFrameInfo.pVirAddr == GK_NULL) {
        ISP_TRACE(MODULE_DBG_WARN, "frameinfo buf don't init ok!\n");
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return ERR_CODE_ISP_NOT_INIT;
    }
    ISP_CalSyncInfoIndex(ViPipe, &s32Index);

    osal_memcpy((ISP_FRAME_INFO_S *)pstDrvCtx->stTransInfo.stFrameInfo.pVirAddr, pstIspFrame,
                sizeof(ISP_FRAME_INFO_S));
    osal_memcpy((ISP_FRAME_INFO_S *)pstDrvCtx->stTransInfo.stFrameInfo.pVirAddr + s32Index, \
                pstIspFrame, sizeof(ISP_FRAME_INFO_S));

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

GK_S32 ISP_GetFrameInfo(VI_PIPE ViPipe, ISP_FRAME_INFO_S *pstIspFrame)
{
    GK_U8 u8ViPipeS;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    ISP_DRV_CTX_S *pstDrvCtxS = GK_NULL;
    unsigned long u32Flags;
    GK_S32 s32Index = 0;
    ISP_FRAME_INFO_S *pFrameInfoVirAddr = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_TRANBUF_INIT(ViPipe);
    ISP_CHECK_POINTER(pstIspFrame);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if ((pstDrvCtx->bMemInit == GK_FALSE) && (pstDrvCtx->stWDRAttr.enWDRMode == WDR_MODE_NONE)) {
        for (u8ViPipeS = 0; u8ViPipeS < ISP_MAX_PIPE_NUM; u8ViPipeS++) {
            pstDrvCtxS = ISP_DRV_GET_CTX(u8ViPipeS);

            if ((pstDrvCtxS->bMemInit == GK_TRUE) && (IS_WDR_MODE(pstDrvCtxS->stWDRAttr.enWDRMode))) {

                osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

                if (pstIspFrame && pstDrvCtxS->stTransInfo.stFrameInfo.pVirAddr) {
                    pFrameInfoVirAddr = (ISP_FRAME_INFO_S *)pstDrvCtxS->stTransInfo.stFrameInfo.pVirAddr;
                    ISP_CalSyncInfoIndex(ViPipe, &s32Index);
                    osal_memcpy(pstIspFrame, pFrameInfoVirAddr + s32Index, sizeof(ISP_FRAME_INFO_S));
                }

                osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
            }
        }
    } else {
        osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

        if (pstIspFrame && pstDrvCtx->stTransInfo.stFrameInfo.pVirAddr) {
            pFrameInfoVirAddr = (ISP_FRAME_INFO_S *)pstDrvCtx->stTransInfo.stFrameInfo.pVirAddr;
            ISP_CalSyncInfoIndex(ViPipe, &s32Index);
            osal_memcpy(pstIspFrame, pFrameInfoVirAddr + s32Index, sizeof(ISP_FRAME_INFO_S));
        }

        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
    }

    return GK_SUCCESS;
}

GK_S32 ISP_GetAttachInfo(VI_PIPE ViPipe, ISP_ATTACH_INFO_S *pstIspAttachInfo)
{
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_TRANBUF_INIT(ViPipe);
    ISP_CHECK_POINTER(pstIspAttachInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    if (pstIspAttachInfo && pstDrvCtx->stTransInfo.stAttaInfo.pVirAddr) {
        osal_memcpy(pstIspAttachInfo, (ISP_ATTACH_INFO_S *)pstDrvCtx->stTransInfo.stAttaInfo.pVirAddr, \
                    sizeof(ISP_ATTACH_INFO_S));
    }

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

GK_S32 ISP_GetColorGamutInfo(VI_PIPE ViPipe, ISP_COLORGAMMUT_INFO_S *pstIspColorGamutInfo)
{
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_TRANBUF_INIT(ViPipe);
    ISP_CHECK_POINTER(pstIspColorGamutInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    if (pstIspColorGamutInfo && pstDrvCtx->stTransInfo.stColorGammutInfo.pVirAddr) {
        osal_memcpy(pstIspColorGamutInfo, (ISP_COLORGAMMUT_INFO_S *)pstDrvCtx->stTransInfo.stColorGammutInfo.pVirAddr,
                    sizeof(ISP_COLORGAMMUT_INFO_S));
    }

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_GetDngInfo(VI_PIPE ViPipe, DNG_IMAGE_STATIC_INFO_S *pstDngInfo)
{
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_TRANBUF_INIT(ViPipe);
    ISP_CHECK_POINTER(pstDngInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    if (pstDngInfo && pstDrvCtx->stTransInfo.stDngInfo.pVirAddr) {
        osal_memcpy(pstDngInfo, (DNG_IMAGE_STATIC_INFO_S *)pstDrvCtx->stTransInfo.stDngInfo.pVirAddr, \
                    sizeof(DNG_IMAGE_STATIC_INFO_S));
    }

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_GetDngImageDynamicInfo(VI_PIPE ViPipe, DNG_IMAGE_DYNAMIC_INFO_S *pstDngImageDynamicInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (g_UpdatePos[ViPipe] == 0) {  /* frame start */
        osal_memcpy(pstDngImageDynamicInfo, &pstDrvCtx->stDngImageDynamicInfo[1], sizeof(DNG_IMAGE_DYNAMIC_INFO_S));
    } else {
        osal_memcpy(pstDngImageDynamicInfo, &pstDrvCtx->stDngImageDynamicInfo[0], sizeof(DNG_IMAGE_DYNAMIC_INFO_S));
    }

    return GK_SUCCESS;
}

GK_S32 ISP_SetModParam(ISP_MOD_PARAM_S *pstModParam)
{
    VI_PIPE ViPipe;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    ISP_CHECK_POINTER(pstModParam);

    for (ViPipe = 0; ViPipe < ISP_MAX_PIPE_NUM; ViPipe++) {
        pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

        if (pstDrvCtx->bMemInit == GK_TRUE) {
            ISP_TRACE(MODULE_DBG_ERR, "Does not support changed after isp init!\n");
            return ERR_CODE_ISP_NOT_SUPPORT;
        }
    }

    if ((pstModParam->u32IntBotHalf != 0) && (pstModParam->u32IntBotHalf != 1)) {
        ISP_TRACE(MODULE_DBG_ERR, "u32IntBotHalf must be 0 or 1.\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if ((pstModParam->u32QuickStart != 0) && (pstModParam->u32QuickStart != 1)) {
        ISP_TRACE(MODULE_DBG_ERR, "u32QuickStart must be 0 or 1.\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if ((pstModParam->bLongFrmIntEn != 0) && (pstModParam->bLongFrmIntEn != 1)) {
        ISP_TRACE(MODULE_DBG_ERR, "bLongFrmIntEn must be 0 or 1.\n");
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }


    g_IntBothalf = pstModParam->u32IntBotHalf;
    g_QuickStart = pstModParam->u32QuickStart;
    g_bLongFrmIntEn = pstModParam->bLongFrmIntEn;
#ifndef __LITEOS__
    if (g_IntBothalf) {
        g_UseBothalf = GK_TRUE;
    }
#else
#ifdef CONFIG_ISP_LITEOS_BOTTOM_HALF_SUPPORT
    if (g_IntBothalf) {
        g_UseBothalf = GK_TRUE;
    }
#endif
#endif

    return GK_SUCCESS;
}

GK_S32 ISP_GetModParam(ISP_MOD_PARAM_S *pstModParam)
{
    ISP_CHECK_POINTER(pstModParam);

    pstModParam->u32IntBotHalf = g_IntBothalf;
    pstModParam->u32QuickStart = g_QuickStart;
    pstModParam->bLongFrmIntEn = g_bLongFrmIntEn;
    return GK_SUCCESS;
}

GK_S32 ISP_SetCtrlParam(VI_PIPE ViPipe, ISP_CTRL_PARAM_S *pstIspCtrlParam)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstIspCtrlParam);
    ISP_CHECK_BOOL(pstIspCtrlParam->bLdciTprFltEn);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    /* isp proc can be closed before mem init */
    /* if isp proc is opened, cannot change u32ProcParam to 0 after mem init */
    if ((g_ProcParam[ViPipe] != 0) && (pstIspCtrlParam->u32ProcParam == 0) && (pstDrvCtx->bMemInit == GK_TRUE)) {
        ISP_TRACE(MODULE_DBG_ERR, "Vipipe:%d u32ProcParam do not support to change %d to 0.\n",
                  ViPipe, g_ProcParam[ViPipe]);
        return ERR_CODE_ISP_NOT_SUPPORT;
    }

    /* if isp proc is closed, cannot change u32ProcParam to Non-0 after mem init */
    if ((g_ProcParam[ViPipe] == 0) && (pstIspCtrlParam->u32ProcParam != 0) && (pstDrvCtx->bMemInit == GK_TRUE)) {
        ISP_TRACE(MODULE_DBG_ERR, "Vipipe:%d u32ProcParam do not support to change %d to Non-0.\n",
                  ViPipe, g_ProcParam[ViPipe]);
        return ERR_CODE_ISP_NOT_SUPPORT;
    }

    if (!pstIspCtrlParam->u32StatIntvl) {
        ISP_TRACE(MODULE_DBG_ERR, "Vipipe:%d u32StatIntvl must be larger than 0.\n", ViPipe);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if ((pstIspCtrlParam->u32UpdatePos != 0) && (pstIspCtrlParam->u32UpdatePos != 1)) {
        ISP_TRACE(MODULE_DBG_ERR, "Vipipe:%d u32UpdatePos must be 0 or 1.\n", ViPipe);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (g_UpdatePos[ViPipe] != pstIspCtrlParam->u32UpdatePos) {
        if (pstDrvCtx->bMemInit == GK_TRUE) {
            ISP_TRACE(MODULE_DBG_ERR, "Vipipe:%d Does not support changed after isp init (u32UpdatePos)!\n", ViPipe);
            return ERR_CODE_ISP_NOT_SUPPORT;
        }
    }

    if (g_PwmNumber[ViPipe] != pstIspCtrlParam->u32PwmNumber) {
        if (pstDrvCtx->bMemInit == GK_TRUE) {
            ISP_TRACE(MODULE_DBG_ERR, "Vipipe:%d Does not support changed after isp init (u32PwmNumber)!\n", ViPipe);
            return ERR_CODE_ISP_NOT_SUPPORT;
        }
    }

    if (g_PortIntDelay[ViPipe] != pstIspCtrlParam->u32PortIntDelay) {
        if (pstDrvCtx->bMemInit == GK_TRUE) {
            ISP_TRACE(MODULE_DBG_ERR, "Vipipe:%d Does not support changed after isp init (u32PortIntDelay)!\n", ViPipe);
            return ERR_CODE_ISP_NOT_SUPPORT;
        }
    }

    if (g_LdciTprFltEn[ViPipe] != pstIspCtrlParam->bLdciTprFltEn) {
        if (pstDrvCtx->bMemInit == GK_TRUE) {
            ISP_TRACE(MODULE_DBG_ERR, "Vipipe:%d Does not support changed after isp init (bLdciTmpFltEn)!\n", ViPipe);
            return ERR_CODE_ISP_NOT_SUPPORT;
        }
    }

    g_ProcParam[ViPipe]    = pstIspCtrlParam->u32ProcParam;
    g_StatIntvl[ViPipe]    = pstIspCtrlParam->u32StatIntvl;
    g_UpdatePos[ViPipe]    = pstIspCtrlParam->u32UpdatePos;
    g_IntTimeout[ViPipe]   = pstIspCtrlParam->u32IntTimeOut;
    g_PwmNumber[ViPipe]    = pstIspCtrlParam->u32PwmNumber;
    g_PortIntDelay[ViPipe] = pstIspCtrlParam->u32PortIntDelay;
    g_LdciTprFltEn[ViPipe] = pstIspCtrlParam->bLdciTprFltEn;

    return GK_SUCCESS;
}

GK_S32 ISP_GetCtrlParam(VI_PIPE ViPipe, ISP_CTRL_PARAM_S *pstIspCtrlParam)
{
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstIspCtrlParam);

    pstIspCtrlParam->u32ProcParam    = g_ProcParam[ViPipe];
    pstIspCtrlParam->u32StatIntvl    = g_StatIntvl[ViPipe];
    pstIspCtrlParam->u32UpdatePos    = g_UpdatePos[ViPipe];
    pstIspCtrlParam->u32IntTimeOut   = g_IntTimeout[ViPipe];
    pstIspCtrlParam->u32PwmNumber    = g_PwmNumber[ViPipe];
    pstIspCtrlParam->u32PortIntDelay = g_PortIntDelay[ViPipe];
    pstIspCtrlParam->bLdciTprFltEn   = g_LdciTprFltEn[ViPipe];

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_StitchSyncEx(VI_PIPE ViPipe)
{
    GK_U8 k;
    VI_PIPE ViPipeId;
    ISP_DRV_CTX_S *pstDrvCtx  = GK_NULL;
    ISP_DRV_CTX_S *pstDrvCtxS = GK_NULL;

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    for (k = 0; k < pstDrvCtx->stStitchAttr.u8StitchPipeNum; k++) {
        ViPipeId = pstDrvCtx->stStitchAttr.as8StitchBindId[k];
        pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeId);

        if (pstDrvCtxS->bStitchSync != GK_TRUE) {
            return GK_FAILURE;
        }
    }

    return GK_SUCCESS;

}

GK_S32 ISP_DRV_StitchSync(VI_PIPE ViPipe)
{
    GK_U8 k;
    VI_PIPE ViPipeId;
    ISP_DRV_CTX_S *pstDrvCtx  = GK_NULL;
    ISP_DRV_CTX_S *pstDrvCtxS = GK_NULL;

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    for (k = 0; k < pstDrvCtx->stStitchAttr.u8StitchPipeNum; k++) {
        ViPipeId = pstDrvCtx->stStitchAttr.as8StitchBindId[k];
        pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeId);

        if (pstDrvCtxS->bIspInit != GK_TRUE) {
            return GK_FAILURE;
        }
    }

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_GetVersion(ISP_VERSION_S *pVersion)
{
    ISP_CHECK_POINTER(pVersion);

    osal_memcpy(&gs_stIspLibInfo, pVersion, sizeof(ISP_VERSION_S));

    gs_stIspLibInfo.u32Magic = VERSION_MAGIC + ISP_MAGIC_OFFSET;
    osal_memcpy(pVersion, &gs_stIspLibInfo, sizeof(ISP_VERSION_S));

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_SetDngInfo(VI_PIPE ViPipe, DNG_IMAGE_DYNAMIC_INFO_S *pstDngImgDynInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstDngImgDynInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_memcpy(&pstDrvCtx->stDngImageDynamicInfo[1], &pstDrvCtx->stDngImageDynamicInfo[0], \
                sizeof(DNG_IMAGE_DYNAMIC_INFO_S));
    osal_memcpy(&pstDrvCtx->stDngImageDynamicInfo[0], pstDngImgDynInfo, \
                sizeof(DNG_IMAGE_DYNAMIC_INFO_S));

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_GetUserStatBuf(VI_PIPE ViPipe, ISP_STAT_INFO_S *pstStat)
{
    ISP_STAT_INFO_S *pstIspStatInfo = GK_NULL;
    ISP_CHECK_POINTER(pstStat);

    ISP_DRV_StatBufUserGet(ViPipe, &pstIspStatInfo);

    if (pstIspStatInfo == GK_NULL) {
        return GK_FAILURE;
    }

    osal_memcpy(pstStat, pstIspStatInfo, sizeof(ISP_STAT_INFO_S));

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_GetRgbirFormat(VI_PIPE ViPipe, ISP_IRBAYER_FORMAT_E *pRgbirFormat)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pRgbirFormat);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    if(!pstDrvCtx->bIspInit)
    {
        return GK_FAILURE;
    }
    *pRgbirFormat = pstDrvCtx->enIspRgbirFormat;
    return GK_SUCCESS;
}
GK_S32 ISP_DRV_SetOnLineStaticsSttAddr(VI_PIPE ViPipe,GK_U32 u32ViProcIrqStatus)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    GK_U32 u32CurReadFlag;
    ISP_CHECK_PIPE(ViPipe);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    if(!pstDrvCtx->bIspInit)
    {
        return GK_FAILURE;
    }

    if((u32ViProcIrqStatus & VIPROC_FRAME_START_INT) == 0)
    {

        return GK_SUCCESS;
    }

    if (IS_OFFLINE_MODE(pstDrvCtx->stWorkMode.enIspRunningMode) ||
        IS_STRIPING_MODE(pstDrvCtx->stWorkMode.enIspRunningMode)) {
        return GK_SUCCESS;
    }
    u32CurReadFlag =  1 - pstDrvCtx->stBeOnlineSttBuf.u32CurWriteFlag;  //org test 0
    //u32CurReadFlag =  pstDrvCtx->stBeOnlineSttBuf.u32CurWriteFlag; /* test  1*/

    ISP_DRV_SetOnlineSttAddr(ViPipe, pstDrvCtx->stBeOnlineSttBuf.astBeSttBuf[u32CurReadFlag].u64PhyAddr);

    if (g_LdciTprFltEn[ViPipe] == GK_TRUE) {
        ISP_DRV_UpdateLdciTprOnlineAddr(ViPipe);
    } else {
        ISP_DRV_UpdateLdciNormalOnlineAddr(ViPipe);
    }

    return GK_SUCCESS;
}
GK_S32 ISP_DRV_GetStatInfoActive(VI_PIPE ViPipe, ISP_STAT_INFO_S *pstIspStatInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    ISP_STAT_INFO_S stActStatInfo;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstIspStatInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (!pstDrvCtx->stStatisticsBuf.pstActStat) {
        ISP_TRACE(MODULE_DBG_WARN, "Pipe[%d] get statistic active buffer err, stat not ready!\n", ViPipe);
        return GK_FAILURE;
    }

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    osal_memcpy(&stActStatInfo, pstDrvCtx->stStatisticsBuf.pstActStat, sizeof(ISP_STAT_INFO_S));
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    osal_memcpy(pstIspStatInfo, &stActStatInfo, sizeof(ISP_STAT_INFO_S));

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_SetRegKernelCfgs(VI_PIPE ViPipe, ISP_KERNEL_REG_CFG_S *pstRegKernelCfg)
{
    GK_U32 u32Flag;
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstRegKernelCfg);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if ((pstDrvCtx->u32RegCfgInfoFlag != 0) &&
        (pstDrvCtx->u32RegCfgInfoFlag != 1)) {
        ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] Err u32RegCfgInfoFlag != 0/1 !!!\n", ViPipe);
    }

    u32Flag = 1 - pstDrvCtx->u32RegCfgInfoFlag;
    osal_memcpy(&pstDrvCtx->astKernelCfg[u32Flag], pstRegKernelCfg, sizeof(ISP_KERNEL_REG_CFG_S));

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstDrvCtx->u32RegCfgInfoFlag = u32Flag;
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_SetConfigInfo(VI_PIPE ViPipe, ISP_CONFIG_INFO_S *pstIspConfigInfo)
{
    GK_U32 i;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstIspConfigInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    for (i = ISP_SAVEINFO_MAX - 1; i >= 1; i--) {
        osal_memcpy(&pstDrvCtx->astSnapInfoSave[i], &pstDrvCtx->astSnapInfoSave[i - 1], sizeof(ISP_CONFIG_INFO_S));
    }

    osal_memcpy(&pstDrvCtx->astSnapInfoSave[0], pstIspConfigInfo, sizeof(ISP_CONFIG_INFO_S));

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_GetSnapInfo(VI_PIPE ViPipe, ISP_SNAP_INFO_S *pstIspSnapInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstIspSnapInfo);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_memcpy(pstIspSnapInfo, &pstDrvCtx->stSnapInfoLoad, sizeof(ISP_SNAP_INFO_S));

    if (pstDrvCtx->stSnapInfoLoad.enSnapState == SNAP_STATE_CFG) {
        if (g_UpdatePos[ViPipe] == 0) {
            pstDrvCtx->stSnapInfoLoad.enSnapState = SNAP_STATE_NULL;
        }
    }

    return GK_SUCCESS;
}

GK_VOID ISP_DRV_BeBufQueuePutBusy(VI_PIPE ViPipe)
{
    GK_U64  u64PhyAddr;
    GK_U64  u64Size;
    GK_VOID *pVirAddr;
    ISP_BE_BUF_NODE_S *pstNode;
    struct osal_list_head *pListTmp = GK_NULL;
    struct osal_list_head *pListNode = GK_NULL;
    ISP_DRV_CTX_S *pstDrvCtx  = GK_NULL;

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

    u64PhyAddr = pstDrvCtx->pstUseNode->stBeCfgBuf.u64PhyAddr;
    pVirAddr   = pstDrvCtx->pstUseNode->stBeCfgBuf.pVirAddr;
    u64Size    = pstDrvCtx->pstUseNode->stBeCfgBuf.u64Size;

    osal_flush_dcache_area(pVirAddr, u64PhyAddr, u64Size);

    ISP_QueuePutBusyBeBuf(&pstDrvCtx->stBeBufQueue, pstDrvCtx->pstUseNode);

    pstDrvCtx->pstUseNode = GK_NULL;

    return;
}

GK_S32 ISP_DRV_RunOnceProcess(VI_PIPE ViPipe)
{
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    ISP_SYNC_CFG_S *pstSyncCfg = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstDrvCtx->bMemInit == GK_FALSE) {
        return ERR_CODE_ISP_MEM_NOT_INIT;
    }

    pstSyncCfg = &pstDrvCtx->stSyncCfg;

    ISP_DRV_GetSyncControlnfo(ViPipe, pstSyncCfg);
    ISP_DRV_CalcSyncCfg(pstSyncCfg);
    ISP_DRV_RegConfigIsp(ViPipe, pstDrvCtx);
    ISP_DRV_RegConfigSensor(ViPipe, pstDrvCtx);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    if (pstDrvCtx->bRunOnceFlag == GK_TRUE) {
        ISP_DRV_BeBufQueuePutBusy(ViPipe);
    }
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    pstDrvCtx->bRunOnceOk = GK_TRUE;

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_YUVRunOnceProcess(VI_PIPE ViPipe)
{
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstDrvCtx->bMemInit == GK_FALSE) {
        return ERR_CODE_ISP_MEM_NOT_INIT;
    }

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    if (pstDrvCtx->bYUVRunOnceFlag == GK_TRUE) {
        ISP_DRV_BeBufQueuePutBusy(ViPipe);
    }
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    pstDrvCtx->bYUVRunOnceOk = GK_TRUE;

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_OptRunOnceInfo(VI_PIPE ViPipe, GK_BOOL *pbRunOnceFlag)
{
    GK_BOOL bRunOnceFlag;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pbRunOnceFlag);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    bRunOnceFlag = *pbRunOnceFlag;

    if (bRunOnceFlag == GK_TRUE) {
        if (pstDrvCtx->pstUseNode) {
            osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
            ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] isp is running!\r\n", ViPipe);
            return GK_FAILURE;
        }

        pstDrvCtx->pstUseNode = ISP_QueueGetFreeBeBuf(&pstDrvCtx->stBeBufQueue);

        if (pstDrvCtx->pstUseNode == GK_NULL) {
            osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
            ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] get FreeBeBuf is fail!\r\n", ViPipe);

            return GK_FAILURE;
        }
    }

    pstDrvCtx->bRunOnceFlag = bRunOnceFlag;

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_YUVRunOnceInfo(VI_PIPE ViPipe, GK_BOOL *pbRunOnceFlag)
{
    GK_BOOL bYUVRunOnceFlag;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pbRunOnceFlag);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    bYUVRunOnceFlag = *pbRunOnceFlag;

    if (bYUVRunOnceFlag == GK_TRUE) {
        if (pstDrvCtx->pstUseNode) {
            osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
            ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] isp is running!\r\n", ViPipe);
            return GK_FAILURE;
        }

        pstDrvCtx->pstUseNode = ISP_QueueGetFreeBeBuf(&pstDrvCtx->stBeBufQueue);

        if (pstDrvCtx->pstUseNode == GK_NULL) {
            osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
            ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] get FreeBeBuf is fail!\r\n", ViPipe);

            return GK_FAILURE;
        }
    }

    pstDrvCtx->bYUVRunOnceFlag = bYUVRunOnceFlag;

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

GK_VOID ISP_DRV_StitchBeBufCtl(VI_PIPE ViPipe)
{
    GK_S32  i;
    GK_S32  s32Ret;
    VI_PIPE ViPipeS;
    VI_PIPE MainPipeS;
    ISP_DRV_CTX_S *pstDrvCtx  = GK_NULL;
    ISP_DRV_CTX_S *pstDrvCtxS = GK_NULL;
    unsigned long u32Flags;

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    MainPipeS = pstDrvCtx->stStitchAttr.as8StitchBindId[0];

    osal_spin_lock_irqsave(&g_stIspSyncLock[MainPipeS], &u32Flags);

    if (pstDrvCtx->enIspRunningState != ISP_BE_BUF_STATE_RUNNING) {
        osal_spin_unlock_irqrestore(&g_stIspSyncLock[MainPipeS], &u32Flags);
        return;
    }

    pstDrvCtx->enIspRunningState = ISP_BE_BUF_STATE_FINISH;

    s32Ret = ISP_DRV_StitchSync(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        osal_spin_unlock_irqrestore(&g_stIspSyncLock[MainPipeS], &u32Flags);
        return;
    }

    for (i = 0; i < pstDrvCtx->stStitchAttr.u8StitchPipeNum; i++) {
        ViPipeS = pstDrvCtx->stStitchAttr.as8StitchBindId[i];
        pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);

        if (pstDrvCtxS->enIspRunningState != ISP_BE_BUF_STATE_FINISH) {
            osal_spin_unlock_irqrestore(&g_stIspSyncLock[MainPipeS], &u32Flags);
            return;
        }
    }

    for (i = 0; i < pstDrvCtx->stStitchAttr.u8StitchPipeNum; i++) {
        ViPipeS = pstDrvCtx->stStitchAttr.as8StitchBindId[i];
        pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);

        if (pstDrvCtxS->stBeBufInfo.bInit != GK_TRUE) {
            ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] BeBuf (bInit != TRUE) !\n", ViPipe);
            osal_spin_unlock_irqrestore(&g_stIspSyncLock[MainPipeS], &u32Flags);
            return;
        }

        if (pstDrvCtxS->bRunOnceFlag != GK_TRUE) {
            ISP_DRV_BeBufQueuePutBusy(ViPipeS);
        }
        pstDrvCtxS->enIspRunningState = ISP_BE_BUF_STATE_INIT;
    }

    osal_spin_unlock_irqrestore(&g_stIspSyncLock[MainPipeS], &u32Flags);

    return;
}

GK_S32 ISP_DRV_WriteAllLdciSttAddr(VI_PIPE ViPipe)
{
    GK_U8  i, k, u8WriteBufIdx, u8BlkNum, u8FreeNum, u8WriteBufNum;
    GK_U32 u32Size;
    GK_U64 u64WriteSttHeadAddr, u64WriteSttAddr;
    ISP_BE_WO_REG_CFG_S *pstBeRegCfg = GK_NULL;
    ISP_BE_BUF_NODE_S   *pstNode     = GK_NULL;
    ISP_DRV_CTX_S       *pstDrvCtx   = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    u8BlkNum      = pstDrvCtx->stWorkMode.u8BlockNum;
    u32Size       = pstDrvCtx->stLdciWriteBufAttr.astLdciBuf[0].u64Size;
    u8WriteBufNum = pstDrvCtx->stLdciWriteBufAttr.u8BufNum;
    u8WriteBufIdx = pstDrvCtx->stLdciWriteBufAttr.u8BufIdx;
    u64WriteSttHeadAddr = pstDrvCtx->stLdciWriteBufAttr.astLdciBuf[u8WriteBufIdx].u64PhyAddr;

    pstBeRegCfg = (ISP_BE_WO_REG_CFG_S *)pstDrvCtx->pstUseNode->stBeCfgBuf.pVirAddr;

    for (i = 0; i < u8BlkNum; i++) {
        u64WriteSttAddr = u64WriteSttHeadAddr + i * (u32Size / DIV_0_TO_1(u8BlkNum));
        ISP_DRV_SetLdciWriteSttAddr(&pstBeRegCfg->stBeRegCfg[i].stViProcReg, u64WriteSttAddr);
    }

    pstDrvCtx->stLdciWriteBufAttr.u8BufIdx = (u8WriteBufIdx + 1) % DIV_0_TO_1(u8WriteBufNum);

    u8FreeNum = ISP_QueueGetFreeNum(&pstDrvCtx->stBeBufQueue);

    for (k = 0; k < u8FreeNum; k++) {
        pstNode = ISP_QueueGetFreeBeBuf(&pstDrvCtx->stBeBufQueue);
        if (pstNode == GK_NULL) {
            ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] Get QueueGetFreeBeBuf fail!\r\n", ViPipe);
            return GK_FAILURE;;
        }

        pstBeRegCfg = (ISP_BE_WO_REG_CFG_S *)pstNode->stBeCfgBuf.pVirAddr;
        u8WriteBufIdx = pstDrvCtx->stLdciWriteBufAttr.u8BufIdx;
        u64WriteSttHeadAddr = pstDrvCtx->stLdciWriteBufAttr.astLdciBuf[u8WriteBufIdx].u64PhyAddr;

        for (i = 0; i < u8BlkNum; i++) {
            u64WriteSttAddr = u64WriteSttHeadAddr + i * (u32Size / DIV_0_TO_1(u8BlkNum));
            ISP_DRV_SetLdciWriteSttAddr(&pstBeRegCfg->stBeRegCfg[i].stViProcReg, u64WriteSttAddr);
        }

        pstDrvCtx->stLdciWriteBufAttr.u8BufIdx = (u8WriteBufIdx + 1) % DIV_0_TO_1(u8WriteBufNum);
        ISP_QueuePutFreeBeBuf(&pstDrvCtx->stBeBufQueue, pstNode);
    }

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_WriteBeFreeBuf(VI_PIPE ViPipe)
{
    GK_S32 i;
    GK_S32 s32FreeNum, s32Ret;
    ISP_RUNNING_MODE_E enIspRunningMode;
    ISP_BE_BUF_NODE_S *pstNode = GK_NULL;
    ISP_BE_WO_REG_CFG_S *pstBeRegCfgSrc = GK_NULL;
    ISP_BE_WO_REG_CFG_S *pstBeRegCfgDst = GK_NULL;
    ISP_DRV_CTX_S *pstDrvCtx  = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_BEBUF_INIT(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstDrvCtx->pstUseNode == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] pstCurNode is null for init!\r\n", ViPipe);
        return GK_FAILURE;
    }

    pstBeRegCfgSrc   = pstDrvCtx->pstUseNode->stBeCfgBuf.pVirAddr;
    enIspRunningMode = pstDrvCtx->stWorkMode.enIspRunningMode;

    s32FreeNum = ISP_QueueGetFreeNum(&pstDrvCtx->stBeBufQueue);

    for (i = 0; i < s32FreeNum; i++) {
        pstNode = ISP_QueueGetFreeBeBuf(&pstDrvCtx->stBeBufQueue);

        if (pstNode == GK_NULL) {
            ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] Get QueueGetFreeBeBuf fail!\r\n", ViPipe);
            return GK_FAILURE;;
        }

        pstBeRegCfgDst = (ISP_BE_WO_REG_CFG_S *)pstNode->stBeCfgBuf.pVirAddr;

        if ((enIspRunningMode == ISP_MODE_RUNNING_SIDEBYSIDE) || (enIspRunningMode == ISP_MODE_RUNNING_STRIPING)) {
            osal_memcpy(pstBeRegCfgDst, pstBeRegCfgSrc, sizeof(ISP_BE_WO_REG_CFG_S));
        } else {
            osal_memcpy(&pstBeRegCfgDst->stBeRegCfg[0], &pstBeRegCfgSrc->stBeRegCfg[0], sizeof(S_ISPBE_REGS_TYPE));
        }

        ISP_QueuePutFreeBeBuf(&pstDrvCtx->stBeBufQueue, pstNode);
    }

    s32Ret = ISP_DRV_WriteAllLdciSttAddr(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        return GK_FAILURE;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_BeBufSwitchFinishState(VI_PIPE ViPipe)
{
    GK_S32  i;
    VI_PIPE ViPipeS;
    ISP_DRV_CTX_S *pstDrvCtxS = GK_NULL;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    unsigned long u32Flags;
    osal_spinlock_t *pIspSpinLock;

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    pIspSpinLock = ISP_DRV_GetSpinLock(ViPipe);
    osal_spin_lock_irqsave(pIspSpinLock, &u32Flags);

    if (pstDrvCtx->stStitchAttr.bStitchEnable == GK_TRUE) {
        if (pstDrvCtx->enIspRunningState != ISP_BE_BUF_STATE_SWITCH_START) {
            osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);
            ISP_TRACE(MODULE_DBG_WARN, "Pipe[%d] isp isn't init state!\n", ViPipe);
            return GK_FAILURE;
        }

        pstDrvCtx->enIspRunningState = ISP_BE_BUF_STATE_SWITCH;

        for (i = 0; i < pstDrvCtx->stStitchAttr.u8StitchPipeNum; i++) {
            ViPipeS = pstDrvCtx->stStitchAttr.as8StitchBindId[i];
            pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);

            if (pstDrvCtxS->enIspRunningState != ISP_BE_BUF_STATE_SWITCH) {
                osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);
                ISP_TRACE(MODULE_DBG_WARN, "Pipe[%d] isp isn't  finish state!\n", ViPipe);
                return GK_FAILURE;
            }
        }

        for (i = 0; i < pstDrvCtx->stStitchAttr.u8StitchPipeNum; i++) {
            ViPipeS = pstDrvCtx->stStitchAttr.as8StitchBindId[i];
            pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);
            pstDrvCtxS->enIspRunningState = ISP_BE_BUF_STATE_INIT;
        }

    }

    osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);

    return GK_SUCCESS;
}


GK_S32 ISP_DRV_StitchWriteBeBufAll(VI_PIPE ViPipe)
{
    GK_S32 i;
    GK_S32 s32Ret;
    VI_PIPE ViPipeS;
    VI_PIPE MainPipeS;
    ISP_DRV_CTX_S *pstDrvCtx  = GK_NULL;
    ISP_DRV_CTX_S *pstDrvCtxS = GK_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    MainPipeS = pstDrvCtx->stStitchAttr.as8StitchBindId[0];

    s32Ret = ISP_DRV_WriteBeFreeBuf(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] ISP_DRV_WriteBeFreeBuf fail!\n", ViPipe);
        return GK_FAILURE;
    }

    osal_spin_lock_irqsave(&g_stIspSyncLock[MainPipeS], &u32Flags);

    s32Ret = ISP_DRV_StitchSyncEx(ViPipe);
    if (s32Ret != GK_SUCCESS) {
        osal_spin_unlock_irqrestore(&g_stIspSyncLock[MainPipeS], &u32Flags);

        return GK_SUCCESS;
    }

    if (pstDrvCtx->enIspRunningState == ISP_BE_BUF_STATE_SWITCH_START) {
        pstDrvCtx->enIspRunningState =  ISP_BE_BUF_STATE_SWITCH;
        for (i = 0; i < pstDrvCtx->stStitchAttr.u8StitchPipeNum; i++) {
            ViPipeS = pstDrvCtx->stStitchAttr.as8StitchBindId[i];
            pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);

            if (pstDrvCtxS->enIspRunningState != ISP_BE_BUF_STATE_SWITCH) {
                osal_spin_unlock_irqrestore(&g_stIspSyncLock[MainPipeS], &u32Flags);
                ISP_TRACE(MODULE_DBG_WARN, "Pipe[%d] isp isn't  finish state!\n", ViPipe);
                pstDrvCtx->enIspRunningState = ISP_BE_BUF_STATE_SWITCH_START;
                return GK_SUCCESS;
            }
        }
        pstDrvCtx->enIspRunningState = ISP_BE_BUF_STATE_SWITCH_START;
    }

    for (i = 0; i < pstDrvCtx->stStitchAttr.u8StitchPipeNum; i++) {
        ViPipeS = pstDrvCtx->stStitchAttr.as8StitchBindId[i];
        pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);

        if (pstDrvCtxS->stBeBufInfo.bInit != GK_TRUE) {
            osal_spin_unlock_irqrestore(&g_stIspSyncLock[MainPipeS], &u32Flags);
            ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] BeBuf (bInit != TRUE) !\n", ViPipe);
            return GK_FAILURE;
        }

        if (pstDrvCtxS->pstUseNode == GK_NULL) {
            osal_spin_unlock_irqrestore(&g_stIspSyncLock[MainPipeS], &u32Flags);
            ISP_TRACE(MODULE_DBG_ERR, "ISP stitch exit without success! pstUseNode is %p!\n", pstDrvCtxS->pstUseNode);
            return GK_FAILURE;
        }

        ISP_QueuePutBusyBeBuf(&pstDrvCtxS->stBeBufQueue, pstDrvCtxS->pstUseNode);
        pstDrvCtxS->pstUseNode = GK_NULL;
        pstDrvCtxS->enIspRunningState = ISP_BE_BUF_STATE_INIT;
    }

    osal_spin_unlock_irqrestore(&g_stIspSyncLock[MainPipeS], &u32Flags);

    return GK_SUCCESS;

}

GK_S32 ISP_DRV_GetBeFreeBuf(VI_PIPE ViPipe, ISP_BE_WO_CFG_BUF_S *pstBeWoCfgBuf)
{
    osal_spinlock_t *pIspSpinLock = GK_NULL;
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    ISP_BE_WO_CFG_BUF_S *pstCurNodeBuf = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_BEBUF_INIT(ViPipe);
    ISP_CHECK_POINTER(pstBeWoCfgBuf);
    ISP_CHECK_ONLINE_MODE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    pIspSpinLock = ISP_DRV_GetSpinLock(ViPipe);

    osal_spin_lock_irqsave(pIspSpinLock, &u32Flags);

    if (pstDrvCtx->pstUseNode == GK_NULL) {
        osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);
        return GK_FAILURE;
    }

    pstCurNodeBuf = &pstDrvCtx->pstUseNode->stBeCfgBuf;
    osal_memcpy(pstBeWoCfgBuf, pstCurNodeBuf, sizeof(ISP_BE_WO_CFG_BUF_S));

    osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_GetBeBufFirst(VI_PIPE ViPipe, GK_U64 *pu64PhyAddr)
{
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    ISP_BE_BUF_NODE_S *pstNode = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_BEBUF_INIT(ViPipe);
    ISP_CHECK_POINTER(pu64PhyAddr);
    ISP_CHECK_ONLINE_MODE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    pstNode = ISP_QueueGetFreeBeBuf(&pstDrvCtx->stBeBufQueue);

    if (pstNode == GK_NULL) {
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

        ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] Get FreeBeBuf to user fail!\r\n", ViPipe);
        return GK_FAILURE;
    }

    pstDrvCtx->pstUseNode = pstNode;

    *pu64PhyAddr = pstDrvCtx->pstUseNode->stBeCfgBuf.u64PhyAddr;

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_GetBeLastBuf(VI_PIPE ViPipe, GK_U64 *pu64PhyAddr)
{
    GK_U8  i;
    ISP_DRV_CTX_S     *pstDrvCtx = GK_NULL;
    ISP_BE_BUF_NODE_S *pstNode   = GK_NULL;
    ISP_BE_WO_REG_CFG_S   *pstBeRegCfgDst = GK_NULL;
    struct osal_list_head *pListTmp = GK_NULL;
    struct osal_list_head *pListNode = GK_NULL;
    unsigned long u32Flags;
    osal_spinlock_t *pIspSpinLock = GK_NULL;

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

        if (pstNode == GK_NULL) {
            osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);
            ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] Get LastBeBuf fail!\r\n", ViPipe);
            return GK_FAILURE;
        }

        pstDrvCtx->pstUseNode = pstNode;
    }

    pstBeRegCfgDst = (ISP_BE_WO_REG_CFG_S *)pstDrvCtx->pstUseNode->stBeCfgBuf.pVirAddr;

    for (i = pstDrvCtx->stWorkMode.u8PreBlockNum; i < pstDrvCtx->stWorkMode.u8BlockNum; i++) {
        osal_memcpy(&pstBeRegCfgDst->stBeRegCfg[i], &pstBeRegCfgDst->stBeRegCfg[0], sizeof(S_ISPBE_REGS_TYPE));
    }

    *pu64PhyAddr = pstDrvCtx->pstUseNode->stBeCfgBuf.u64PhyAddr;

    osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_BeBufRunState(VI_PIPE ViPipe)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    unsigned long u32Flags;
    osal_spinlock_t *pIspSpinLock = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_BEBUF_INIT(ViPipe);
    ISP_CHECK_ONLINE_MODE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    pIspSpinLock = ISP_DRV_GetSpinLock(ViPipe);

    osal_spin_lock_irqsave(pIspSpinLock, &u32Flags);

    if (pstDrvCtx->stStitchAttr.bStitchEnable == GK_TRUE) {
        if (pstDrvCtx->enIspRunningState != ISP_BE_BUF_STATE_INIT) {
            osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);

            ISP_TRACE(MODULE_DBG_WARN, "Pipe[%d] isp isn't init state!\n", ViPipe);
            return GK_FAILURE;
        }

        pstDrvCtx->enIspRunningState = ISP_BE_BUF_STATE_RUNNING;
    }

    osal_spin_unlock_irqrestore(pIspSpinLock, &u32Flags);

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_BeBufCtl(VI_PIPE ViPipe)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_ONLINE_MODE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstDrvCtx->stStitchAttr.bStitchEnable != GK_TRUE) {
        if (pstDrvCtx->stBeBufInfo.bInit != GK_TRUE) {
            ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] BeBuf (bInit != TRUE) !\n", ViPipe);
            return GK_FAILURE;
        }

        osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
        if (pstDrvCtx->bRunOnceFlag != GK_TRUE) {
            ISP_DRV_BeBufQueuePutBusy(ViPipe);
        }
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
    } else {
        ISP_DRV_StitchBeBufCtl(ViPipe);
    }

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_AllBeBufInit(VI_PIPE ViPipe)
{
    GK_S32  s32Ret = GK_SUCCESS;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_ONLINE_MODE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstDrvCtx->stStitchAttr.bStitchEnable != GK_TRUE) {
        if (pstDrvCtx->stBeBufInfo.bInit != GK_TRUE) {
            ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] BeBuf (bInit != TRUE) !\n", ViPipe);
            return GK_FAILURE;
        }

        osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

        s32Ret = ISP_DRV_WriteBeFreeBuf(ViPipe);
        if (s32Ret != GK_SUCCESS) {
            osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

            ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] ISP_DRV_WriteBeFreeBuf fail!\n", ViPipe);
            return s32Ret;
        }

        ISP_QueuePutBusyBeBuf(&pstDrvCtx->stBeBufQueue, pstDrvCtx->pstUseNode);
        pstDrvCtx->pstUseNode = GK_NULL;

        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
    } else {
        s32Ret = ISP_DRV_StitchWriteBeBufAll(ViPipe);
        if (s32Ret != GK_SUCCESS) {
            ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] ISP_DRV_StitchWriteBeBufAll fail!\n", ViPipe);
            return s32Ret;
        }
    }

    return s32Ret;
}

GK_S32 ISP_DRV_SyncCfgSet(VI_PIPE ViPipe, ISP_SYNC_CFG_BUF_NODE_S *pstIspSyncCfgBuf)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    ISP_SYNC_CFG_BUF_S  *pstSyncCfgBuf = GK_NULL;
    ISP_SYNC_CFG_BUF_NODE_S  *pstCurNode = GK_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstIspSyncCfgBuf);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstSyncCfgBuf = &pstDrvCtx->stSyncCfg.stSyncCfgBuf;

    if (ISPSyncBufIsFull(pstSyncCfgBuf)) {
        ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] isp sync buffer is full\n", ViPipe);
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return GK_FAILURE;
    }

    if ((pstIspSyncCfgBuf->stSnsRegsInfo.enSnsType >= ISP_SNS_TYPE_BUTT) || \
        (pstIspSyncCfgBuf->stAERegCfg.enFSWDRMode >= ISP_FSWDR_MODE_BUTT) || \
        (pstIspSyncCfgBuf->stSnsRegsInfo.u8Cfg2ValidDelayMax > CFG2VLD_DLY_LIMIT) || \
        (pstIspSyncCfgBuf->stSnsRegsInfo.u8Cfg2ValidDelayMax < 1) ||
        (pstIspSyncCfgBuf->stSnsRegsInfo.stSlvSync.u8DelayFrmNum > CFG2VLD_DLY_LIMIT) || \
        (pstIspSyncCfgBuf->stSnsRegsInfo.u32RegNum > ISP_MAX_SNS_REGS)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid SnsRegsInfo!\n");
        osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        return GK_FAILURE;
    }

    pstCurNode = &pstSyncCfgBuf->stSyncCfgBufNode[pstSyncCfgBuf->u8BufWRFlag];
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    osal_memcpy(pstCurNode, pstIspSyncCfgBuf, sizeof(ISP_SYNC_CFG_BUF_NODE_S));

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    pstSyncCfgBuf->u8BufWRFlag = (pstSyncCfgBuf->u8BufWRFlag + 1) % ISP_SYNC_BUF_NODE_NUM;
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_SetWdrCfg(VI_PIPE ViPipe, ISP_WDR_CFG_S *pstIspWdrCfg)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstIspWdrCfg);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (pstIspWdrCfg->u8WDRMode >= WDR_MODE_BUTT) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid WDR mode %d!\n", pstIspWdrCfg->u8WDRMode);
        return GK_FAILURE;
    }

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    osal_memcpy(&pstDrvCtx->stWDRCfg, pstIspWdrCfg, sizeof(ISP_WDR_CFG_S));
    ISP_DRV_SwitchMode(ViPipe, pstDrvCtx);
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_GetStitchAttr(VI_PIPE ViPipe, VI_STITCH_ATTR_S *pstStitchAttr)
{
    GK_U8   i;
    GK_S32  s32Ret;
    ISP_DRV_CTX_S    *pstDrvCtx = GK_NULL;
    VI_STITCH_ATTR_S stStitchAttr = { 0 };

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstStitchAttr);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (!CKFN_VI_GetPipeStitchAttr()) {
        ISP_TRACE(MODULE_DBG_ERR, "pipe[%d] CKFN_VI_GetPipeWDRAttr is null\n", ViPipe);
        return GK_FAILURE;
    }

    s32Ret = CALL_VI_GetPipeStitchAttr(ViPipe, &stStitchAttr);

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "pipe[%d] CALL_VI_GetPipeStitchAttr failed 0x%x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    if (stStitchAttr.bStitchEnable) {
        if ((stStitchAttr.u8StitchPipeNum < 1) || (stStitchAttr.u8StitchPipeNum > ISP_MAX_PIPE_NUM)) {
            ISP_TRACE(MODULE_DBG_ERR, "pipe[%d] err stitch num %d\n", ViPipe, stStitchAttr.u8StitchPipeNum);
            return ERR_CODE_ISP_ILLEGAL_PARAM;
        }
        for (i = 0; i < stStitchAttr.u8StitchPipeNum; i++) {
            ISP_CHECK_PIPE(stStitchAttr.as8StitchBindId[i]);
        }
    }

    /* Don't support stitch different wdr mode(check)! */

    osal_memcpy(&pstDrvCtx->stStitchAttr, &stStitchAttr, sizeof(VI_STITCH_ATTR_S));
    osal_memcpy(pstStitchAttr, &stStitchAttr, sizeof(VI_STITCH_ATTR_S));

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_GetPipeSize(VI_PIPE ViPipe, SIZE_S *pstPipeSize)
{
    GK_S32  s32Ret;
    SIZE_S  stPipeSize = { 0 };

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstPipeSize);
    ISP_CHECK_VIR_PIPE(ViPipe);
    if (!CKFN_VI_GetPipeBindDevSize()) {
        ISP_TRACE(MODULE_DBG_ERR, "pipe[%d] CKFN_VI_GetPipeBindDevSize is null\n", ViPipe);
        return GK_FAILURE;
    }

    s32Ret = CALL_VI_GetPipeBindDevSize(ViPipe, &stPipeSize);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "pipe[%d] CALL_VI_GetPipeBindDevSize failed 0x%x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    if ((stPipeSize.u32Width < RES_WIDTH_MIN) ||
        (stPipeSize.u32Width > SENSOR_RES_WIDTH_MAX) ||
        (stPipeSize.u32Height < RES_HEIGHT_MIN) ||
        (stPipeSize.u32Height > SENSOR_RES_HEIGHT_MAX)) {
        ISP_TRACE(MODULE_DBG_ERR, "pipe[%d]: Image Width should between [%d, %d], Height should between[%d, %d]\n",
                  ViPipe, RES_WIDTH_MIN, SENSOR_RES_WIDTH_MAX, RES_HEIGHT_MIN, SENSOR_RES_HEIGHT_MAX);
        return GK_FAILURE;
    }

    osal_memcpy(pstPipeSize, &stPipeSize, sizeof(SIZE_S));

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_GetHdrAttr(VI_PIPE ViPipe, VI_PIPE_HDR_ATTR_S *pstHDRAttr)
{
    GK_S32  s32Ret;
    VI_PIPE_HDR_ATTR_S stHDRAttr = { 0 };

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstHDRAttr);

    if (!CKFN_VI_GetPipeHDRAttr()) {
        ISP_TRACE(MODULE_DBG_ERR, "pipe[%d] CKFN_VI_GetPipeHDRAttr is null\n", ViPipe);
        return GK_FAILURE;
    }

    s32Ret = CALL_VI_GetPipeHDRAttr(ViPipe, &stHDRAttr);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "pipe[%d] CALL_VI_GetPipeHDRAttr failed 0x%x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    osal_memcpy(pstHDRAttr, &stHDRAttr, sizeof(VI_PIPE_HDR_ATTR_S));

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_GetWdrAttr(VI_PIPE ViPipe, VI_PIPE_WDR_ATTR_S *pstWDRAttr)
{
    GK_U8   i;
    GK_U32  u32Num;
    GK_S32  s32Ret;
    VI_PIPE_WDR_ATTR_S stWDRAttr;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstWDRAttr);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (!CKFN_VI_GetPipeWDRAttr()) {
        ISP_TRACE(MODULE_DBG_ERR, "pipe[%d] CKFN_VI_GetPipeWDRAttr is null\n", ViPipe);
        return GK_FAILURE;
    }

    s32Ret = CALL_VI_GetPipeWDRAttr(ViPipe, &stWDRAttr);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "pipe[%d] CALL_VI_GetPipeWDRAttr failed 0x%x!\n", ViPipe, s32Ret);
        return s32Ret;
    }

    /* Not WDR mode,BindPipe attr update */
    if ((!IS_FS_WDR_MODE(stWDRAttr.enWDRMode)) &&
        (stWDRAttr.stDevBindPipe.u32Num != 1)) {
        stWDRAttr.stDevBindPipe.u32Num    = 1;
        stWDRAttr.stDevBindPipe.PipeId[0] = ViPipe;
    }

    u32Num = stWDRAttr.stDevBindPipe.u32Num;
    if ((u32Num < 1) || (u32Num > ISP_WDR_CHN_MAX)) {
        ISP_TRACE(MODULE_DBG_ERR, "Err wdr bind num %d!\n", u32Num);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    for (i = 0; i < u32Num; i++) {
        ISP_CHECK_PIPE(stWDRAttr.stDevBindPipe.PipeId[i]);
    }
    if ((stWDRAttr.enWDRMode > WDR_MODE_BUTT)) {
        ISP_TRACE(MODULE_DBG_ERR, "Err wdr mode %d!\n", stWDRAttr.enWDRMode);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }
    osal_memcpy(&pstDrvCtx->stWDRAttr, &stWDRAttr, sizeof(VI_PIPE_WDR_ATTR_S));
    osal_memcpy(pstWDRAttr, &stWDRAttr, sizeof(VI_PIPE_WDR_ATTR_S));

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_ChnSelectCfg(VI_PIPE ViPipe, GK_U32 u32ChnSel)
{
    GK_U32 i;
    GK_S32 s32Ret = GK_SUCCESS;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    for (i = 0; i < ISP_STRIPING_MAX_NUM; i++) {
        pstDrvCtx->astChnSelAttr[i].u32ChannelSel = u32ChnSel;
    }

    if (IS_ONLINE_MODE(pstDrvCtx->stWorkMode.enIspRunningMode) ||
        IS_SIDEBYSIDE_MODE(pstDrvCtx->stWorkMode.enIspRunningMode)) {
        s32Ret = ISP_DRV_ChnSelectWrite(ViPipe, u32ChnSel);

        if (s32Ret != GK_SUCCESS) {
            osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
            ISP_TRACE(MODULE_DBG_ERR, "isp[%d] ChnSelect Write err!\n", ViPipe);
            return s32Ret;
        }
    }

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return s32Ret;
}

GK_S32 ISP_DRV_SetIspRunState(VI_PIPE ViPipe, GK_U64 *pu64HandSignal)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pu64HandSignal);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (*pu64HandSignal == ISP_INIT_HAND_SIGNAL) {
        pstDrvCtx->bIspRunFlag = GK_TRUE;
    } else if (*pu64HandSignal == ISP_EXIT_HAND_SIGNAL) {
        pstDrvCtx->bIspRunFlag = GK_FALSE;
        ISP_DRV_ResetFeCfg(ViPipe);
    } else {
        ISP_TRACE(MODULE_DBG_ERR, "ISP[%d] set isp run state failed!\n", ViPipe);
        return GK_FAILURE;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_ResetCtx(VI_PIPE ViPipe)
{
    GK_U8 i;
    VI_PIPE ViPipeS = 0;
    unsigned long u32Flags;
    ISP_DRV_CTX_S *pstDrvCtx  = GK_NULL;
    ISP_DRV_CTX_S *pstDrvCtxS = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

    pstDrvCtx->bMemInit     = GK_FALSE;
    pstDrvCtx->bIspInit     = GK_FALSE;
    pstDrvCtx->bRunOnceOk   = GK_FALSE;
    pstDrvCtx->bRunOnceFlag = GK_FALSE;
    pstDrvCtx->bYUVRunOnceOk   = GK_FALSE;
    pstDrvCtx->bYUVRunOnceFlag = GK_FALSE;

    if (pstDrvCtx->stStitchAttr.bStitchEnable == GK_TRUE) {
        if (pstDrvCtx->stStitchAttr.bMainPipe == GK_TRUE) {
            for (i = 0; i < pstDrvCtx->stStitchAttr.u8StitchPipeNum; i++) {
                ViPipeS = pstDrvCtx->stStitchAttr.as8StitchBindId[i];
                ISP_CHECK_PIPE(ViPipeS);
                pstDrvCtxS = ISP_DRV_GET_CTX(ViPipeS);
                pstDrvCtxS->bStitchSync  = GK_FALSE;
            }
        }
    }

    pstDrvCtx->stStitchAttr.bStitchEnable = GK_FALSE;

    for (i = 0; i < ISP_STRIPING_MAX_NUM; i++) {
        pstDrvCtx->astChnSelAttr[i].u32ChannelSel = 0;
    }

    pstDrvCtx->stSnapAttr.enSnapType          = SNAP_TYPE_NORMAL;
    pstDrvCtx->stSnapAttr.s32PicturePipeId    = -1;
    pstDrvCtx->stSnapAttr.s32PreviewPipeId    = -1;
    pstDrvCtx->stSnapAttr.bLoadCCM            = GK_TRUE;
    pstDrvCtx->stSnapAttr.stProParam.enOperationMode = OPERATION_MODE_AUTO;

    pstDrvCtx->bEdge    = GK_FALSE;
    pstDrvCtx->bVdStart = GK_FALSE;
    pstDrvCtx->bVdEnd   = GK_FALSE;
    pstDrvCtx->bVdBeEnd = GK_FALSE;

    g_IntBothalf = GK_FALSE;
    g_UseBothalf = GK_FALSE;
    g_QuickStart = GK_FALSE;
    g_bLongFrmIntEn = GK_FALSE;

    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

GK_S32 ISP_SetPubAttrInfo(VI_PIPE ViPipe, ISP_PUB_ATTR_S *pstPubAttr)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);
    ISP_CHECK_POINTER(pstPubAttr);

    if ((pstPubAttr->stWndRect.u32Width < RES_WIDTH_MIN) ||
        (pstPubAttr->stWndRect.u32Width > g_au16ResMaxWidth[ViPipe]) ||
        (pstPubAttr->stWndRect.u32Width % 4 != 0)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid Image Width:%d!\n", pstPubAttr->stWndRect.u32Width);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if ((pstPubAttr->stWndRect.u32Height < RES_HEIGHT_MIN) ||
        (pstPubAttr->stWndRect.u32Height > g_au16ResMaxHeight[ViPipe]) ||
        (pstPubAttr->stWndRect.u32Height % 4 != 0)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid Image Height:%d!\n", pstPubAttr->stWndRect.u32Height);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if ((pstPubAttr->stSnsSize.u32Width < RES_WIDTH_MIN) || (pstPubAttr->stSnsSize.u32Width > SENSOR_RES_WIDTH_MAX)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid Sensor Image Width:%d!\n", pstPubAttr->stSnsSize.u32Width);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if ((pstPubAttr->stSnsSize.u32Height < RES_HEIGHT_MIN) ||
        (pstPubAttr->stSnsSize.u32Height > SENSOR_RES_HEIGHT_MAX)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid Sensor Image Height:%d!\n", pstPubAttr->stSnsSize.u32Height);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if ((pstPubAttr->stWndRect.s32X < 0) ||
        (pstPubAttr->stWndRect.s32X > g_au16ResMaxWidth[ViPipe] - RES_WIDTH_MIN)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid Image X:%d!\n", pstPubAttr->stWndRect.s32X);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if ((pstPubAttr->stWndRect.s32Y < 0) ||
        (pstPubAttr->stWndRect.s32Y > g_au16ResMaxHeight[ViPipe] - RES_HEIGHT_MIN)) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid Image Y:%d!\n", pstPubAttr->stWndRect.s32Y);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstPubAttr->enBayer >= BAYER_BUTT) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid Bayer Pattern:%d!\n", pstPubAttr->enBayer);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    if (pstPubAttr->enWDRMode >= WDR_MODE_BUTT) {
        ISP_TRACE(MODULE_DBG_ERR, "Invalid WDR mode %d!\n", pstPubAttr->enWDRMode);
        return ERR_CODE_ISP_ILLEGAL_PARAM;
    }

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);
    osal_memcpy(&pstDrvCtx->stProcPubInfo, pstPubAttr, sizeof(ISP_PUB_ATTR_S));
    osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);

    return GK_SUCCESS;
}

static long ISP_ioctl(unsigned int cmd, unsigned long arg, void *private_data)
{
    VI_PIPE ViPipe = ISP_GET_DEV(private_data);

    switch (cmd) {
        case ISP_DEV_SET_FD: {
            ISP_CHECK_POINTER(arg);
            *((GK_U32 *)(private_data)) = *(GK_U32 *)(GK_UINTPTR_T)arg;

            return GK_SUCCESS;
        }

        case ISP_GET_VERSION: {
            ISP_VERSION_S *pVersion = GK_NULL;
            pVersion = (ISP_VERSION_S *)(GK_UINTPTR_T)arg;

            return ISP_DRV_GetVersion(pVersion);
        }

        case ISP_GET_FRAME_EDGE: {
            GK_U32 *pu32Status = GK_NULL;
            pu32Status = (GK_U32 *)(GK_UINTPTR_T)arg;

            return ISP_GetFrameEdge(ViPipe, pu32Status);
        }

        case ISP_GET_VD_TIMEOUT: {
            ISP_VD_TIMEOUT_S stIspVdTimeOut;
            ISP_VD_TIMEOUT_S *pstVdTimeOut = GK_NULL;

            ISP_CHECK_POINTER(arg);

            pstVdTimeOut = (ISP_VD_TIMEOUT_S *)(GK_UINTPTR_T)arg;
            osal_memcpy(&stIspVdTimeOut, pstVdTimeOut, sizeof(ISP_VD_TIMEOUT_S));
            ISP_GetVdStartTimeOut(ViPipe, stIspVdTimeOut.u32MilliSec, &stIspVdTimeOut.u32IntStatus);
            osal_memcpy(pstVdTimeOut, &stIspVdTimeOut, sizeof(ISP_VD_TIMEOUT_S));

            return GK_SUCCESS;
        }

        case ISP_GET_VD_END_TIMEOUT: {
            ISP_VD_TIMEOUT_S stIspVdTimeOut;
            ISP_VD_TIMEOUT_S *pstVdTimeOut = GK_NULL;

            ISP_CHECK_POINTER(arg);

            pstVdTimeOut = (ISP_VD_TIMEOUT_S *)(GK_UINTPTR_T)arg;
            osal_memcpy(&stIspVdTimeOut, pstVdTimeOut, sizeof(ISP_VD_TIMEOUT_S));
            ISP_GetVdEndTimeOut(ViPipe, stIspVdTimeOut.u32MilliSec, &stIspVdTimeOut.u32IntStatus);
            osal_memcpy(pstVdTimeOut, &stIspVdTimeOut, sizeof(ISP_VD_TIMEOUT_S));

            return GK_SUCCESS;
        }

        case ISP_GET_VD_BEEND_TIMEOUT: {
            ISP_VD_TIMEOUT_S stIspVdTimeOut;
            ISP_VD_TIMEOUT_S *pstVdTimeOut = GK_NULL;

            ISP_CHECK_POINTER(arg);

            pstVdTimeOut = (ISP_VD_TIMEOUT_S *)(GK_UINTPTR_T)arg;
            osal_memcpy(&stIspVdTimeOut, pstVdTimeOut, sizeof(ISP_VD_TIMEOUT_S));
            ISP_GetVdBeEndTimeOut(ViPipe, stIspVdTimeOut.u32MilliSec, &stIspVdTimeOut.u32IntStatus);
            osal_memcpy(pstVdTimeOut, &stIspVdTimeOut, sizeof(ISP_VD_TIMEOUT_S));

            return GK_SUCCESS;
        }

        case ISP_UPDATE_INFO_SET: {
            ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
            ISP_DCF_UPDATE_INFO_S *pstIspUpdateInfo = GK_NULL;
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
            pstIspUpdateInfo = (ISP_DCF_UPDATE_INFO_S *)(GK_UINTPTR_T)arg;
            osal_memcpy(&pstDrvCtx->stUpdateInfo, pstIspUpdateInfo, sizeof(ISP_DCF_UPDATE_INFO_S));
            return GK_SUCCESS;
        }

        case ISP_FRAME_INFO_SET: {
            ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
            ISP_FRAME_INFO_S *pstIspFrameInfo = GK_NULL;
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
            pstIspFrameInfo = (ISP_FRAME_INFO_S *)(GK_UINTPTR_T)arg;
            osal_memcpy(&pstDrvCtx->stFrameInfo, pstIspFrameInfo, sizeof(ISP_FRAME_INFO_S));
            return GK_SUCCESS;
        }

        case ISP_FRAME_INFO_GET: {
            ISP_FRAME_INFO_S *pstIspFrameInfo = GK_NULL;
            pstIspFrameInfo = (ISP_FRAME_INFO_S *)(GK_UINTPTR_T)arg;

            return ISP_GetFrameInfo(ViPipe, pstIspFrameInfo);
        }

        case ISP_DNG_INFO_SET: {
            DNG_IMAGE_DYNAMIC_INFO_S *pstDngImgDynInfo = GK_NULL;
            pstDngImgDynInfo = (DNG_IMAGE_DYNAMIC_INFO_S *)(GK_UINTPTR_T)arg;

            return ISP_DRV_SetDngInfo(ViPipe, pstDngImgDynInfo);
        }

        case ISP_DNG_INFO_GET: {
            DNG_IMAGE_STATIC_INFO_S *pstDngStaticInfo = GK_NULL;
            pstDngStaticInfo = (DNG_IMAGE_STATIC_INFO_S *)(GK_UINTPTR_T)arg;

            return ISP_DRV_GetDngInfo(ViPipe, pstDngStaticInfo);
        }

        case ISP_SET_INT_ENABLE: {
            GK_BOOL bEn = GK_FALSE;
            ISP_CHECK_POINTER(arg);
            bEn = *(GK_BOOL *)(GK_UINTPTR_T)arg;

            return ISP_DRV_SetIntEnable(ViPipe, bEn);
        }

        case ISP_TRANS_BUF_INIT: {
            ISP_TRANS_INFO_BUF_S *pstTransInfo = GK_NULL;
            pstTransInfo = (ISP_TRANS_INFO_BUF_S *)(GK_UINTPTR_T)arg;

            return ISP_DRV_TransInfoBufInit(ViPipe, pstTransInfo);
        }

        case ISP_TRANS_BUF_EXIT: {
            return ISP_DRV_TransInfoBufExit(ViPipe);
        }

        case ISP_PRO_BUF_INIT: {
            ISP_PRO_INFO_BUF_S *pstProInfo = GK_NULL;
            pstProInfo = (ISP_PRO_INFO_BUF_S *)(GK_UINTPTR_T)arg;

            return ISP_DRV_ProInfoBufInit(ViPipe, pstProInfo);
        }

        case ISP_PRO_BUF_EXIT: {
            return ISP_DRV_ProInfoBufExit(ViPipe);
        }

        case ISP_STAT_BUF_INIT: {
            GK_U64 *pu64StatPhyAddr = GK_NULL;
            pu64StatPhyAddr = (GK_U64 *)(GK_UINTPTR_T)arg;

            return ISP_DRV_StatBufInit(ViPipe, pu64StatPhyAddr);
        }

        case ISP_STAT_BUF_EXIT: {
            return ISP_DRV_StatBufExit(ViPipe);
        }

        case ISP_STAT_BUF_GET: {
            ISP_STAT_INFO_S *pstStat = GK_NULL;
            pstStat = (ISP_STAT_INFO_S *)(GK_UINTPTR_T)arg;

            return ISP_DRV_GetUserStatBuf(ViPipe, pstStat);
        }

        case ISP_STAT_BUF_PUT: {
            ISP_STAT_INFO_S *pstStatInfo = GK_NULL;
            pstStatInfo = (ISP_STAT_INFO_S *)(GK_UINTPTR_T)arg;

            return ISP_DRV_StatBufUserPut(ViPipe, pstStatInfo);
        }

        case ISP_STAT_ACT_GET: {
            ISP_STAT_INFO_S *pstIspStatInfo = GK_NULL;
            pstIspStatInfo = (ISP_STAT_INFO_S *)(GK_UINTPTR_T)arg;

            return ISP_DRV_GetStatInfoActive(ViPipe, pstIspStatInfo);
        }

        case ISP_REG_CFG_SET: {
            ISP_KERNEL_REG_CFG_S *pstRegKernelCfg = GK_NULL;
            pstRegKernelCfg = (ISP_KERNEL_REG_CFG_S *)(GK_UINTPTR_T)arg;

            return ISP_DRV_SetRegKernelCfgs(ViPipe, pstRegKernelCfg);
        }

        case ISP_BE_CFG_BUF_INIT: {
            GK_U64 *pu64BePhyAddr = GK_NULL;
            pu64BePhyAddr = (GK_U64 *)(GK_UINTPTR_T)arg;

            return ISP_DRV_BeBufInit(ViPipe, pu64BePhyAddr);
        }

        case ISP_GET_BE_BUF_FIRST: {
            GK_U64 *pu64PhyAddr = GK_NULL;
            pu64PhyAddr = (GK_U64 *)(GK_UINTPTR_T)arg;

            return ISP_DRV_GetBeBufFirst(ViPipe, pu64PhyAddr);
        }

        case ISP_BE_FREE_BUF_GET: {
            ISP_BE_WO_CFG_BUF_S *pstBeWoCfgBuf = GK_NULL;
            pstBeWoCfgBuf = (ISP_BE_WO_CFG_BUF_S *)(GK_UINTPTR_T)arg;

            return ISP_DRV_GetBeFreeBuf(ViPipe, pstBeWoCfgBuf);
        }

        case ISP_BE_LAST_BUF_GET: {
            GK_U64 *pu64PhyAddr = GK_NULL;
            pu64PhyAddr = (GK_U64 *)(GK_UINTPTR_T)arg;

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
            ISP_SYNC_CFG_BUF_NODE_S *pstIspSyncCfgBuf = GK_NULL;
            pstIspSyncCfgBuf = (ISP_SYNC_CFG_BUF_NODE_S *)(GK_UINTPTR_T)arg;

            return ISP_DRV_SyncCfgSet(ViPipe, pstIspSyncCfgBuf);
        }

        case ISP_WDR_CFG_SET: {
            ISP_WDR_CFG_S *pstIspWdrCfg = GK_NULL;
            pstIspWdrCfg = (ISP_WDR_CFG_S *)(GK_UINTPTR_T)arg;

            return ISP_DRV_SetWdrCfg(ViPipe, pstIspWdrCfg);
        }

        case ISP_BE_SYNC_PARAM_INIT: {
            ISP_DRV_CTX_S      *pstDrvCtx      = GK_NULL;
            ISP_BE_SYNC_PARA_S *pstBeSyncParam = GK_NULL;
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
            pstBeSyncParam = (ISP_BE_SYNC_PARA_S *)(GK_UINTPTR_T)arg;
            osal_memcpy(&pstDrvCtx->stIspBeSyncPara, pstBeSyncParam, sizeof(ISP_BE_SYNC_PARA_S));
            return GK_SUCCESS;
        }
        case ISP_MODE_SWITCH_SET: {
            ISP_CHECK_PIPE(ViPipe);
            ISP_DRV_BeBufSwitchState(ViPipe);
            return GK_SUCCESS;
        }

        case ISP_RES_SWITCH_SET: {
            ISP_CHECK_PIPE(ViPipe);
            ISP_DRV_BeBufSwitchState(ViPipe);
            return GK_SUCCESS;
        }

        case ISP_BE_SWITCH_FINISH_STATE_SET: {
            return ISP_DRV_BeBufSwitchFinishState(ViPipe);
        }

        case ISP_CHN_SELECT_CFG: {
            GK_U32 u32ChannelSel = 0;
            ISP_CHECK_POINTER(arg);
            u32ChannelSel = *(GK_U32 *)(GK_UINTPTR_T)arg;

            return ISP_DRV_ChnSelectCfg(ViPipe, u32ChannelSel);
        }

        case ISP_PROC_INIT: {
            ISP_PROC_MEM_S *pstIspProcMem = GK_NULL;
            pstIspProcMem = (ISP_PROC_MEM_S *)(GK_UINTPTR_T)arg;

            return ISP_DRV_ProcInit(ViPipe, pstIspProcMem);
        }

        case ISP_PROC_WRITE_ING: {
            ISP_CHECK_PIPE(ViPipe);

            if (osal_down_interruptible(&g_astIspDrvCtx[ViPipe].stProcSem)) {
                return -ERESTARTSYS;
            }
            return GK_SUCCESS;
        }

        case ISP_PROC_WRITE_OK: {
            ISP_CHECK_PIPE(ViPipe);

            osal_up(&g_astIspDrvCtx[ViPipe].stProcSem);
            return GK_SUCCESS;
        }

        case ISP_PROC_EXIT: {
            return ISP_DRV_ProcExit(ViPipe);
        }

        case ISP_PROC_PARAM_GET: {
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            *(GK_U32 *)(GK_UINTPTR_T)arg = g_ProcParam[ViPipe];

            return GK_SUCCESS;
        }

        case ISP_MEM_INFO_SET: {
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            ISP_CHECK_BOOL(*(GK_BOOL *)(GK_UINTPTR_T)arg);
            g_astIspDrvCtx[ViPipe].bMemInit = *(GK_BOOL *)(GK_UINTPTR_T)arg;

            return GK_SUCCESS;
        }

        case ISP_MEM_INFO_GET: {
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            *(GK_BOOL *)(GK_UINTPTR_T)arg = g_astIspDrvCtx[ViPipe].bMemInit;

            return GK_SUCCESS;
        }

        case ISP_P2EN_INFO_GET: {
            GK_BOOL *pbP2En = GK_NULL;

            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            pbP2En = (GK_BOOL *)(GK_UINTPTR_T)arg;

            return ISP_DRV_GetP2EnInfo(ViPipe, pbP2En);
        }

        case ISP_SYNC_INIT_SET: {
            ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            ISP_CHECK_BOOL(*(GK_BOOL *)(GK_UINTPTR_T)arg);
            pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

            pstDrvCtx->bStitchSync = GK_FALSE;
            if (pstDrvCtx->stStitchAttr.bStitchEnable == GK_TRUE) {
                pstDrvCtx->bStitchSync = *(GK_BOOL *)(GK_UINTPTR_T)arg;
            }

            return GK_SUCCESS;
        }

        case ISP_INIT_INFO_SET: {
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            ISP_CHECK_BOOL(*(GK_BOOL *)(GK_UINTPTR_T)arg);
            g_astIspDrvCtx[ViPipe].bIspInit = *(GK_BOOL *)(GK_UINTPTR_T)arg;

            return GK_SUCCESS;
        }

        case ISP_RESET_CTX: {
            return ISP_DRV_ResetCtx(ViPipe);
        }

        case ISP_RUN_STATE_SET: {
            GK_U64 *pu64HandSignal = GK_NULL;
            pu64HandSignal = (GK_U64 *)(GK_UINTPTR_T)arg;

            return ISP_DRV_SetIspRunState(ViPipe, pu64HandSignal);
        }

        case ISP_CONFIG_INFO_SET: {
            ISP_CONFIG_INFO_S *pstIspConfigInfo = GK_NULL;
            pstIspConfigInfo = (ISP_CONFIG_INFO_S *)(GK_UINTPTR_T)arg;

            return ISP_DRV_SetConfigInfo(ViPipe, pstIspConfigInfo);
        }

        case ISP_SNAP_INFO_GET: {
            ISP_SNAP_INFO_S *pstIspSnapInfo = GK_NULL;
            pstIspSnapInfo = (ISP_SNAP_INFO_S *)(GK_UINTPTR_T)arg;

            return ISP_DRV_GetSnapInfo(ViPipe, pstIspSnapInfo);
        }

        case ISP_PRO_TRIGGER_GET: {
            ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
            *(GK_BOOL *)(GK_UINTPTR_T)arg = pstDrvCtx->bProEnable;

            if (pstDrvCtx->bProEnable == GK_TRUE) {
                pstDrvCtx->bProEnable = GK_FALSE;
                pstDrvCtx->u32ProTrigFlag = 1;
            }

            return GK_SUCCESS;
        }

        case ISP_SNAP_ATTR_GET: {
            ISP_SNAP_ATTR_S *pstIspSnapAttr = GK_NULL;
            ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            pstIspSnapAttr = (ISP_SNAP_ATTR_S *)(GK_UINTPTR_T)arg;
            pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
            osal_memcpy(pstIspSnapAttr, &pstDrvCtx->stSnapAttr, sizeof(ISP_SNAP_ATTR_S));

            return GK_SUCCESS;
        }

        case ISP_UPDATE_POS_GET: {
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            *(GK_U32 *)(GK_UINTPTR_T)arg = g_UpdatePos[ViPipe];

            return GK_SUCCESS;
        }

        case ISP_FRAME_CNT_GET: {
            ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
            *(GK_U32 *)(GK_UINTPTR_T)arg = pstDrvCtx->u32FrameCnt;

            return GK_SUCCESS;
        }
        case ISP_PUB_ATTR_INFO: {
            ISP_PUB_ATTR_S *pstIspPubAttr = GK_NULL;
            pstIspPubAttr = (ISP_PUB_ATTR_S *)(GK_UINTPTR_T)arg;

            return ISP_SetPubAttrInfo(ViPipe, pstIspPubAttr);
        }

        case ISP_PWM_NUM_GET: {
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            *(GK_U32 *)(GK_UINTPTR_T)arg = g_PwmNumber[ViPipe];

            return GK_SUCCESS;
        }

        case ISP_SET_MOD_PARAM: {
            ISP_MOD_PARAM_S *pstModeParam = GK_NULL;
            pstModeParam = (ISP_MOD_PARAM_S *)(GK_UINTPTR_T)arg;

            return ISP_SetModParam(pstModeParam);
        }

        case ISP_GET_MOD_PARAM: {
            ISP_MOD_PARAM_S *pstModeParam = GK_NULL;
            pstModeParam = (ISP_MOD_PARAM_S *)(GK_UINTPTR_T)arg;

            return ISP_GetModParam(pstModeParam);
        }

        case ISP_SET_CTRL_PARAM: {
            ISP_CTRL_PARAM_S *pstCtrlParam = GK_NULL;
            pstCtrlParam = (ISP_CTRL_PARAM_S *)(GK_UINTPTR_T)arg;

            return ISP_SetCtrlParam(ViPipe, pstCtrlParam);
        }

        case ISP_GET_CTRL_PARAM: {
            ISP_CTRL_PARAM_S *pstCtrlParam = GK_NULL;
            pstCtrlParam = (ISP_CTRL_PARAM_S *)(GK_UINTPTR_T)arg;

            return ISP_GetCtrlParam(ViPipe, pstCtrlParam);
        }

        case ISP_WORK_MODE_INIT: {
            ISP_BLOCK_ATTR_S *pstIspBlockAttr = GK_NULL;
            pstIspBlockAttr = (ISP_BLOCK_ATTR_S *)(GK_UINTPTR_T)arg;

            return ISP_DRV_WorkModeInit(ViPipe, pstIspBlockAttr);
        }

        case ISP_WORK_MODE_EXIT: {
            return ISP_DRV_WorkModeExit(ViPipe);
        }

        case ISP_WORK_MODE_GET: {
            ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
            ISP_WORKING_MODE_S *pstIspWorkMode = GK_NULL;
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            pstIspWorkMode = (ISP_WORKING_MODE_S *)(GK_UINTPTR_T)arg;
            pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
            osal_memcpy(pstIspWorkMode, &pstDrvCtx->stWorkMode, sizeof(ISP_WORKING_MODE_S));

            return GK_SUCCESS;
        }

        case ISP_PRE_BLK_NUM_UPDATE: {
            ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
            pstDrvCtx->stWorkMode.u8PreBlockNum = *(GK_U8 *)(GK_UINTPTR_T)arg;

            return GK_SUCCESS;
        }

        case ISP_GET_PIPE_SIZE: {
            SIZE_S *pstPipeSize = GK_NULL;
            pstPipeSize = (SIZE_S *)(GK_UINTPTR_T)arg;

            return ISP_DRV_GetPipeSize(ViPipe, pstPipeSize);
        }

        case ISP_GET_HDR_ATTR: {
            VI_PIPE_HDR_ATTR_S *pstHDRAttr = GK_NULL;
            pstHDRAttr = (VI_PIPE_HDR_ATTR_S *)(GK_UINTPTR_T)arg;

            return ISP_DRV_GetHdrAttr(ViPipe, pstHDRAttr);
        }

        case ISP_GET_WDR_ATTR: {
            VI_PIPE_WDR_ATTR_S *pstWDRAttr = GK_NULL;
            pstWDRAttr = (VI_PIPE_WDR_ATTR_S *)(GK_UINTPTR_T)arg;

            return ISP_DRV_GetWdrAttr(ViPipe, pstWDRAttr);
        }

        case ISP_GET_STITCH_ATTR: {
            VI_STITCH_ATTR_S *pstStitchAttr = GK_NULL;
            pstStitchAttr = (VI_STITCH_ATTR_S *)(GK_UINTPTR_T)arg;

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
            if (g_astIspDrvCtx[ViPipe].stClutBufAttr.bInit == GK_FALSE) {
                return ERR_CODE_ISP_NOMEM;
            }
            *(GK_U64 *)(GK_UINTPTR_T)arg = g_astIspDrvCtx[ViPipe].stClutBufAttr.stClutBuf.u64PhyAddr;

            return GK_SUCCESS;
        }

        case ISP_LDCI_BUF_INIT: {
            return ISP_DRV_LdciBufInit(ViPipe);
        }

        case ISP_LDCI_BUF_EXIT: {
            return ISP_DRV_LdciBufExit(ViPipe);
        }

        case ISP_LDCI_READ_STT_BUF_GET: {
            return ISP_DRV_LdciReadSttBufGet(ViPipe, (ISP_LDCI_READ_STT_BUF_S *)(GK_UINTPTR_T)arg);
        }

        case ISP_OPT_RUNONCE_INFO: {
            return ISP_DRV_OptRunOnceInfo(ViPipe, (GK_BOOL *)(GK_UINTPTR_T)arg);
        }

        case ISP_YUV_RUNONCE_INFO: {
            return ISP_DRV_YUVRunOnceInfo(ViPipe, (GK_BOOL *)(GK_UINTPTR_T)arg);
        }

        case ISP_KERNEL_RUNONCE: {
            return ISP_DRV_RunOnceProcess(ViPipe);
        }
        case ISP_KERNEL_YUV_RUNONCE: {
            return ISP_DRV_YUVRunOnceProcess(ViPipe);
        }
        case ISP_SET_PROCALCDONE: {
            ISP_CHECK_PIPE(ViPipe);
            g_astIspDrvCtx[ViPipe].bProStart = GK_TRUE;

            return GK_SUCCESS;
        }

        case ISP_STT_BUF_INIT: {
            return ISP_DRV_SttBufInit(ViPipe);
        }

        case ISP_STT_ADDR_INIT: {
            ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
            ISP_CHECK_PIPE(ViPipe);

            pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

            if (pstDrvCtx->stStitchAttr.bStitchEnable == GK_FALSE) {
                return ISP_DRV_FeSttAddrInit(ViPipe);
            } else {
                return ISP_DRV_FeStitchSttAddrInit(ViPipe);
            }
        }

        case ISP_BE_LUT_STT_BUF_GET: {
            GK_U64 u64PhyAddr;
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);

            if (g_astIspDrvCtx[ViPipe].stBeLut2SttAttr.bInit == GK_FALSE) {
                return ERR_CODE_ISP_NOMEM;
            }
            u64PhyAddr = g_astIspDrvCtx[ViPipe].stBeLut2SttAttr.astBeLutSttBuf[0].astLutSttBuf[0].u64PhyAddr;
            *(GK_U64 *)(GK_UINTPTR_T)arg = u64PhyAddr;

            return GK_SUCCESS;
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
#ifdef CONFIG_ISP_SPECAWB_SUPPORT
            ISP_CHECK_PIPE(ViPipe);
            ISP_CHECK_POINTER(arg);
            if (g_astIspDrvCtx[ViPipe].stSpecAwbBufAttr.bInit == GK_FALSE) {
                return ERR_CODE_ISP_NOMEM;
            }
            *(GK_U64 *)(GK_UINTPTR_T)arg = g_astIspDrvCtx[ViPipe].stSpecAwbBufAttr.stSpecAwbBuf.u64PhyAddr;

            return GK_SUCCESS;
#else
            return ERR_CODE_ISP_NOT_SUPPORT;
#endif
        }
        case ISP_SET_RGBIR_FORMAT: {
            ISP_CHECK_PIPE(ViPipe);
            g_astIspDrvCtx[ViPipe].enIspRgbirFormat = *(GK_U32 *)(GK_UINTPTR_T)arg;
            return GK_SUCCESS;
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
            ISP_BE_WO_CFG_BUF_S *pstBeWoCfgBuf = (ISP_BE_WO_CFG_BUF_S *)(GK_UINTPTR_T)arg;
            COMPAT_POINTER(pstBeWoCfgBuf->pVirAddr, GK_VOID *);
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

#ifdef CONFIG_SNAPSHOT_BOOT
static GK_S32 ISP_Freeze(osal_dev_t *pdev)
{
    return GK_SUCCESS;
}

static GK_S32 ISP_Restore(osal_dev_t *pdev)
{
    VI_PIPE ViPipe;

    for (ViPipe = 0; ViPipe < ISP_MAX_PIPE_NUM; ViPipe++) {
        /* enable port int & isp int */
        IO_RW_PT_ADDRESS(VICAP_HD_MASK) |= VICAP_INT_MASK_PT(ViPipe);
        IO_RW_PT_ADDRESS(VICAP_HD_MASK) |= VICAP_INT_MASK_ISP(ViPipe);

        IO_RW_FE_ADDRESS(ViPipe, ISP_INT_FE_MASK) = (0x0);
    }

    return GK_SUCCESS;
}
#else
static GK_S32 ISP_Freeze(osal_dev_t *pdev)
{
    return GK_SUCCESS;
}

static GK_S32 ISP_Restore(osal_dev_t *pdev)
{
    return GK_SUCCESS;
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

struct osal_pmops stISPDrvOps = {
    .pm_freeze      = ISP_Freeze,
    .pm_restore     = ISP_Restore
};

static osal_dev_t *s_pstISPDevice = GK_NULL;

#ifdef CONFIG_ISP_LITEOS_BOTTOM_HALF_SUPPORT
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

static inline int ISP_ISR(int irq, void *id)
{
    GK_U32  i;
    GK_S32  ViDev;
    VI_PIPE ViPipe;
    GK_U32 u32PortIntStatus = 0;
    GK_U32 u32PortIntFStart = 0;
    GK_U32 u32PortIntErr = 0;
    GK_U32 u32IspRawIntStatus = 0;
    GK_U32 u32IspIntStatus = 0;
    GK_U32 u32WCHIntStatus = 0;
    GK_U32 u32WCHIntFStart = 0;
    GK_U64 u64IspIrqBeginTime;
    GK_BOOL bViCapInt;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    bViCapInt      = (irq == isp_fe_irq);

    u64IspIrqBeginTime = CALL_SYS_GetTimeStamp();

    /* Isp FE Interrupt Process Begin */
    if (bViCapInt) {
        for (i = 0; i < ISP_MAX_PHY_PIPE_NUM; i++) {
            ViPipe = i;

            pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
            ViDev     = pstDrvCtx->stWDRAttr.ViDev;

            /* read interrupt status */
            u32PortIntStatus  = IO_RW_PT_ADDRESS(VI_PT_BASE(ViDev) + VI_PT_INT);
            u32PortIntStatus &= IO_RW_PT_ADDRESS(VI_PT_BASE(ViDev) + VI_PT_INT_MASK);
            u32PortIntFStart  = u32PortIntStatus & VI_PT_INT_FSTART;
            u32PortIntErr     = u32PortIntStatus & VI_PT_INT_ERR;

            u32WCHIntStatus   = IO_RW_CH_ADDRESS(ViPipe, VI_WCH_INT);
            u32WCHIntFStart   = u32WCHIntStatus & VI_WCH_INT_FSTART;

            u32IspRawIntStatus  = IO_RW_FE_ADDRESS(ViPipe, ISP_INT_FE);
            u32IspIntStatus     = u32IspRawIntStatus & IO_RW_FE_ADDRESS(ViPipe, ISP_INT_FE_MASK);

            pstDrvCtx->stIntSch.u32IspIntStatus  = u32IspIntStatus;
            pstDrvCtx->stIntSch.u32PortIntStatus = u32PortIntFStart;
            pstDrvCtx->stIntSch.u32PortIntErr    = u32PortIntErr;
            pstDrvCtx->stIntSch.u32WCHIntStatus  = u32WCHIntFStart;

            pstDrvCtx->stIspSyncCtrl.u64PTS = u64IspIrqBeginTime;

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
#ifdef CONFIG_ISP_LITEOS_BOTTOM_HALF_SUPPORT
#ifdef __LITEOS__
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

GK_VOID ISP_DRV_WakeUpThread(VI_PIPE ViPipe)
{
    GK_BOOL bWakeUpTread = GK_TRUE;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if (IS_OFFLINE_MODE(pstDrvCtx->stWorkMode.enIspRunningMode) ||
        IS_STRIPING_MODE(pstDrvCtx->stWorkMode.enIspRunningMode)) {
        if (pstDrvCtx->stStitchAttr.bStitchEnable == GK_TRUE) {
            if (pstDrvCtx->enIspRunningState != ISP_BE_BUF_STATE_INIT) {
                bWakeUpTread = GK_FALSE;
            }
        }
    }

    if (bWakeUpTread == GK_TRUE) {
        pstDrvCtx->bEdge = GK_TRUE;
        pstDrvCtx->bVdStart = GK_TRUE;

        osal_wakeup(&pstDrvCtx->stIspWait);
        osal_wakeup(&pstDrvCtx->stIspWaitVdStart);
    }

    return;
}

GK_S32 ISP_DRV_GetUseNode(VI_PIPE ViPipe, GK_U32 u32IspIntStatus, GK_U32 u32WCHIntStatus)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    ISP_SYNC_CFG_S *pstSyncCfg = GK_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx  = ISP_DRV_GET_CTX(ViPipe);
    pstSyncCfg = &pstDrvCtx->stSyncCfg;

    if (IS_OFFLINE_MODE(pstDrvCtx->stWorkMode.enIspRunningMode) ||
        IS_STRIPING_MODE(pstDrvCtx->stWorkMode.enIspRunningMode)) {
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

                return GK_FAILURE;
            }

            osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

            pstDrvCtx->pstUseNode = ISP_QueueGetFreeBeBuf(&pstDrvCtx->stBeBufQueue);

            if (pstDrvCtx->pstUseNode == GK_NULL) {
                /* Need to configure the sensor registers. */
                ISP_DRV_RegConfigSensor(ViPipe, pstDrvCtx);
                osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
                ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] get FreeBeBuf is fail!\r\n", ViPipe);

                return GK_FAILURE;
            }

            osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        }

        if ((pstDrvCtx->bYuvMode == GK_TRUE) && u32WCHIntStatus&& (pstDrvCtx->bIspRunFlag == GK_TRUE)) {
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

                return GK_FAILURE;
            }

            osal_spin_lock_irqsave(&g_stIspLock[ViPipe], &u32Flags);

            pstDrvCtx->pstUseNode = ISP_QueueGetFreeBeBuf(&pstDrvCtx->stBeBufQueue);

            if (pstDrvCtx->pstUseNode == GK_NULL) {
                osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
                ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] get FreeBeBuf is fail!\r\n", ViPipe);

                return GK_FAILURE;
            }

            osal_spin_unlock_irqrestore(&g_stIspLock[ViPipe], &u32Flags);
        }
    }

    return GK_SUCCESS;
}

ISP_CFG_SENSOR_TIME_E ISP_DRV_GetSnsCfgIntPos(ISP_DRV_CTX_S *pstDrvCtx,VI_PIPE ViPipeBind)
{
    VI_PIPE WdrShortFramePipeId;
    VI_PIPE WdrLongFramePipeId;

    if(pstDrvCtx->stWDRAttr.enWDRMode == WDR_MODE_2To1_LINE)
    {
        return ISP_CFG_SNS_SHORT_FRAME;
    }
    if(pstDrvCtx->stWDRAttr.enWDRMode == WDR_MODE_3To1_LINE)
    {
        if(pstDrvCtx->stSlavePipeIntStatus[0].ViPipeId <  pstDrvCtx->stSlavePipeIntStatus[1].ViPipeId)
        {
            WdrShortFramePipeId = pstDrvCtx->stSlavePipeIntStatus[0].ViPipeId;
        }
        else
        {
            WdrShortFramePipeId = pstDrvCtx->stSlavePipeIntStatus[1].ViPipeId;
        }
        if(ViPipeBind == WdrShortFramePipeId)
        {
            return ISP_CFG_SNS_SHORT_FRAME;
        }
        else
        {
            return ISP_CFG_SNS_MIDDLE_FRAME;
        }
    }

    else if(pstDrvCtx->stWDRAttr.enWDRMode == WDR_MODE_4To1_LINE)
    {
        if((pstDrvCtx->stSlavePipeIntStatus[0].ViPipeId <  pstDrvCtx->stSlavePipeIntStatus[1].ViPipeId) &&
           (pstDrvCtx->stSlavePipeIntStatus[0].ViPipeId <  pstDrvCtx->stSlavePipeIntStatus[2].ViPipeId))
        {
            WdrShortFramePipeId = pstDrvCtx->stSlavePipeIntStatus[0].ViPipeId;
        }
        else  if((pstDrvCtx->stSlavePipeIntStatus[1].ViPipeId <  pstDrvCtx->stSlavePipeIntStatus[0].ViPipeId) &&
                 (pstDrvCtx->stSlavePipeIntStatus[1].ViPipeId <  pstDrvCtx->stSlavePipeIntStatus[2].ViPipeId))
        {
            WdrShortFramePipeId = pstDrvCtx->stSlavePipeIntStatus[1].ViPipeId;
        }
        else
        {
            WdrShortFramePipeId = pstDrvCtx->stSlavePipeIntStatus[2].ViPipeId;
        }


        if((pstDrvCtx->stSlavePipeIntStatus[0].ViPipeId >  pstDrvCtx->stSlavePipeIntStatus[1].ViPipeId) &&
           (pstDrvCtx->stSlavePipeIntStatus[0].ViPipeId >  pstDrvCtx->stSlavePipeIntStatus[2].ViPipeId))
        {
            WdrLongFramePipeId = pstDrvCtx->stSlavePipeIntStatus[0].ViPipeId;
        }
        else  if((pstDrvCtx->stSlavePipeIntStatus[1].ViPipeId >  pstDrvCtx->stSlavePipeIntStatus[0].ViPipeId) &&
                 (pstDrvCtx->stSlavePipeIntStatus[1].ViPipeId >  pstDrvCtx->stSlavePipeIntStatus[2].ViPipeId))
        {
            WdrLongFramePipeId = pstDrvCtx->stSlavePipeIntStatus[1].ViPipeId;
        }
        else
        {
            WdrLongFramePipeId = pstDrvCtx->stSlavePipeIntStatus[2].ViPipeId;
        }
        if(ViPipeBind == WdrShortFramePipeId)
        {
            return ISP_CFG_SNS_SHORT_FRAME;
        }
        else if(ViPipeBind == WdrLongFramePipeId)
        {
            return ISP_CFG_SNS_LONG_FRAME;
        }
        else
        {
            return ISP_CFG_SNS_MIDDLE_FRAME;
        }

    }
    return ISP_CFG_SNS_SHORT_FRAME;
}
GK_VOID ISP_LongFrmCfgSensor(VI_PIPE ViPipe)
{
    ISP_DRV_CTX_S *pstDrvCtx;
    GK_U32 u32BindPipeIntStatus;
    GK_U32 u32PipeIntPosSave;
    GK_U32 u32PipeIntStatusSave;
    GK_U32 k;
    VI_PIPE ViPipeBind;
    GK_U32 u32CfgSnsIntPos;
    if(g_bLongFrmIntEn == GK_FALSE)
    {
        return ;
    }
    pstDrvCtx  = ISP_DRV_GET_CTX(ViPipe);
    if(IS_FS_WDR_MODE(pstDrvCtx->stWDRAttr.enWDRMode) == GK_FALSE)
    {
        return;
    }

    u32PipeIntStatusSave = pstDrvCtx->u32Status ;
    u32PipeIntPosSave = pstDrvCtx->u32IntPos;

    for(k=0;k<ISP_WDR_CHN_MAX-1;k++)
    {
        u32BindPipeIntStatus = pstDrvCtx->stSlavePipeIntStatus[k].u32IspIntStatus;
        ViPipeBind = pstDrvCtx->stSlavePipeIntStatus[k].ViPipeId;
        u32CfgSnsIntPos = ISP_DRV_GetSnsCfgIntPos(pstDrvCtx,ViPipeBind);
        if(u32BindPipeIntStatus & ISP_1ST_INT)
        {
            pstDrvCtx->u32IntPos = ((u32CfgSnsIntPos<<4)&0xf0)+0; //frame start
            pstDrvCtx->u32Status= u32BindPipeIntStatus;
            ISP_DRV_RegConfigSensor(ViPipe, pstDrvCtx);
        }
        if(u32BindPipeIntStatus & ISP_2ND_INT)
        {
            pstDrvCtx->u32IntPos = ((u32CfgSnsIntPos<<4)& 0xf0) + 1;
            pstDrvCtx->u32Status= u32BindPipeIntStatus;
            ISP_DRV_RegConfigSensor(ViPipe, pstDrvCtx);
        }
    }
    pstDrvCtx->u32Status = u32PipeIntStatusSave;
    pstDrvCtx->u32IntPos = u32PipeIntPosSave;
    return ;
}


GK_S32 ISP_DRV_StitchGetUseNode(VI_PIPE ViPipe, GK_U32 u32IspIntStatus)
{
    VI_PIPE MainPipeS;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    ISP_SYNC_CFG_S *pstSyncCfg = GK_NULL;
    unsigned long u32Flags;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx  = ISP_DRV_GET_CTX(ViPipe);
    pstSyncCfg = &pstDrvCtx->stSyncCfg;
    MainPipeS  = pstDrvCtx->stStitchAttr.as8StitchBindId[0];

    if (IS_OFFLINE_MODE(pstDrvCtx->stWorkMode.enIspRunningMode) ||
        IS_STRIPING_MODE(pstDrvCtx->stWorkMode.enIspRunningMode)) {
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

                return GK_FAILURE;
            }

            pstDrvCtx->pstUseNode = ISP_QueueGetFreeBeBuf(&pstDrvCtx->stBeBufQueue);

            if (pstDrvCtx->pstUseNode == GK_NULL) {
                /* Need to configure the sensor registers. */
                ISP_DRV_RegConfigSensor(ViPipe, pstDrvCtx);
                osal_spin_unlock_irqrestore(&g_stIspSyncLock[MainPipeS], &u32Flags);
                ISP_TRACE(MODULE_DBG_ERR, "Pipe[%d] get FreeBeBuf is fail!\r\n", ViPipe);

                return GK_FAILURE;
            }

            osal_spin_unlock_irqrestore(&g_stIspSyncLock[MainPipeS], &u32Flags);
        }
    }

    return GK_SUCCESS;
}

GK_S32 ISP_DRV_GetBeBufUseNode(VI_PIPE ViPipe, GK_U32 u32IspIntStatus)
{
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    GK_U32 u32WCHIntFStart;
    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    u32WCHIntFStart = pstDrvCtx->stIntSch.u32WCHIntStatus;
    if (pstDrvCtx->stStitchAttr.bStitchEnable != GK_TRUE) {
        return ISP_DRV_GetUseNode(ViPipe, u32IspIntStatus, u32WCHIntFStart);
    } else {
        return ISP_DRV_StitchGetUseNode(ViPipe, u32IspIntStatus);
    }
}

int  ISP_IrqRoute(VI_PIPE ViPipe)
{
    GK_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    ISP_SYNC_CFG_S *pstSyncCfg = GK_NULL;
    GK_U64 u64SensorCfgTime1 = 0;
    GK_U64 u64SensorCfgTime2 = 0;
    GK_U32 u32PortIntFStart, u32IspIntStatus, u32WCHIntFStart, u32SensorCfgInt;
    GK_U64 u64PtTime1 = 0;
    GK_U64 u64PtTime2 = 0;
    GK_U64 u64IspTime1 = 0;
    GK_U64 u64IspTime2 = 0;

    ISP_CHECK_PIPE(ViPipe);

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
    pstSyncCfg = &pstDrvCtx->stSyncCfg;

    u32PortIntFStart = pstDrvCtx->stIntSch.u32PortIntStatus;
    u32IspIntStatus  = pstDrvCtx->stIntSch.u32IspIntStatus;
    u32WCHIntFStart = pstDrvCtx->stIntSch.u32WCHIntStatus;

    if (u32PortIntFStart) {  /* port int proc */
        pstDrvCtx->stDrvDbgInfo.u32PtIntCnt++;
        u64PtTime1 = CALL_SYS_GetTimeStamp();

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
        u64IspTime1 = CALL_SYS_GetTimeStamp();

        if (pstDrvCtx->stDrvDbgInfo.u64IspLastIntTime) {   // not first int
            pstDrvCtx->stDrvDbgInfo.u32IspIntGapTime = u64IspTime1 - pstDrvCtx->stDrvDbgInfo.u64IspLastIntTime;

            if (pstDrvCtx->stDrvDbgInfo.u32IspIntGapTime > pstDrvCtx->stDrvDbgInfo.u32IspIntGapTimeMax) {
                pstDrvCtx->stDrvDbgInfo.u32IspIntGapTimeMax = pstDrvCtx->stDrvDbgInfo.u32IspIntGapTime;
            }
        }

        pstDrvCtx->stDrvDbgInfo.u64IspLastIntTime = u64IspTime1;
    }

    pstDrvCtx->u32IntPos = 0;

    s32Ret = ISP_DRV_GetBeBufUseNode(ViPipe, u32IspIntStatus);//only frame start interrup active
    if (s32Ret != GK_SUCCESS) {
        return OSAL_IRQ_HANDLED;
    }

    if (u32IspIntStatus & ISP_1ST_INT) {
        if (ViPipe == pstDrvCtx->stSnapAttr.s32PicturePipeId) {
            if (CKFN_VI_GetIspConfig()) {
                CALL_VI_GetIspConfig(ViPipe, &pstDrvCtx->stSnapInfoLoad);
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

            u64SensorCfgTime1 = CALL_SYS_GetTimeStamp();
            ISP_DRV_RegConfigSensor(ViPipe, pstDrvCtx);
            u64SensorCfgTime2 = CALL_SYS_GetTimeStamp();
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

            u64SensorCfgTime1 = CALL_SYS_GetTimeStamp();
            ISP_DRV_RegConfigSensor(ViPipe, pstDrvCtx);
            u64SensorCfgTime2 = CALL_SYS_GetTimeStamp();
            pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime = u64SensorCfgTime2 - u64SensorCfgTime1;
        }
    }

    if (u32IspIntStatus & ISP_2ND_INT) {
        pstDrvCtx->u32IntPos = 1;
        pstDrvCtx->u32Status = u32IspIntStatus;

        ISP_DRV_RegConfigSensor(ViPipe, pstDrvCtx);

        pstDrvCtx->bVdEnd = GK_TRUE;
        if (pstDrvCtx->u32ProTrigFlag == 1) {
            pstDrvCtx->u32ProTrigFlag++;
        }
        ISP_UpdateInfoSync(ViPipe, &pstDrvCtx->stUpdateInfo);
        ISP_frameInfoSync(ViPipe, &pstDrvCtx->stFrameInfo);
        osal_wakeup(&pstDrvCtx->stIspWaitVdEnd);
    }
    ISP_LongFrmCfgSensor(ViPipe);
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
    if (u32WCHIntFStart && (pstDrvCtx->bYuvMode == GK_TRUE)&& (pstDrvCtx->bIspRunFlag == GK_TRUE)) {  /* WCH int */
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
        u64PtTime2 = CALL_SYS_GetTimeStamp();
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
        u64IspTime2 = CALL_SYS_GetTimeStamp();
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

GK_VOID ISP_GetSlavePipeIntStatus(VI_PIPE ViPipe,GK_BOOL *bSlavePipeIntActive)
{
    GK_U8 u8ChnNumMax;
    GK_U8 k;
    GK_U8 u8IntStatusIndex;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    GK_U32  u32IspIntStatus = 0;
    GK_U32 isp_raw_int = 0;
    VI_PIPE ViPipeBind;
    if(g_bLongFrmIntEn == GK_FALSE)
    {
        *bSlavePipeIntActive = GK_FALSE;
        return ;
    }

    pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

    if((pstDrvCtx->stWDRAttr.bMastPipe == GK_FALSE) ||
       (IS_FS_WDR_MODE(pstDrvCtx->stWDRAttr.enWDRMode) == GK_FALSE))
    {
        osal_memset(&(pstDrvCtx->stSlavePipeIntStatus),0,sizeof(ISP_SLAVE_PIPE_INT_STATUS_S)*(ISP_WDR_CHN_MAX-1));
        *bSlavePipeIntActive = GK_FALSE;
        return ;
    }
    else
    {
        u8ChnNumMax = CLIP3(pstDrvCtx->stWDRAttr.stDevBindPipe.u32Num, 1, ISP_WDR_CHN_MAX);
        u8IntStatusIndex = 0;
        for(k=0;k<u8ChnNumMax;k++)
        {
            ViPipeBind = pstDrvCtx->stWDRAttr.stDevBindPipe.PipeId[k];
            if(ViPipe == ViPipeBind)
            {
                continue;
            }
            else
            {
                if (ViPipeBind >= ISP_MAX_PHY_PIPE_NUM) {
                    continue;
                }
                /* read fe interrupt status */
                isp_raw_int = IO_RW_FE_ADDRESS(ViPipeBind, ISP_INT_FE);
                u32IspIntStatus = isp_raw_int & IO_RW_FE_ADDRESS(ViPipeBind, ISP_INT_FE_MASK);
                if (isp_raw_int) {  /* clear interrupts */
                    IO_RW_FE_ADDRESS(ViPipeBind, ISP_INT_FE) = isp_raw_int;
                }

                pstDrvCtx->stSlavePipeIntStatus[u8IntStatusIndex].ViPipeId= ViPipeBind;
                pstDrvCtx->stSlavePipeIntStatus[u8IntStatusIndex].u32IspIntStatus = u32IspIntStatus ;
                u8IntStatusIndex++;
                if(u32IspIntStatus != 0)
                {
                    *bSlavePipeIntActive = GK_TRUE;
                }
            }
        }
    }
    return ;
}

int  ISP_DoIntBottomHalf(int irq, void *id)
{
    VI_PIPE ViPipe, ViPipeS;
    GK_U32 i, j;
    GK_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    ISP_DRV_CTX_S *pstDrvCtxS = GK_NULL;
    GK_U32  u32PortIntStatus = 0;
    GK_U32  u32PortIntErr = 0;
    GK_U32  u32IspIntStatus = 0;
    GK_U32  u32WCHIntFStart = 0;
    GK_BOOL bSlavePipeIntActive;

    for (i = 0; i < ISP_MAX_PHY_PIPE_NUM; i++) {
        ViPipe = i;
        pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);

        u32IspIntStatus  = pstDrvCtx->stIntSch.u32IspIntStatus;
        u32PortIntStatus = pstDrvCtx->stIntSch.u32PortIntStatus;
        u32PortIntErr    = pstDrvCtx->stIntSch.u32PortIntErr;
        u32WCHIntFStart  = pstDrvCtx->stIntSch.u32WCHIntStatus;

        ISP_GetSlavePipeIntStatus(ViPipe,&bSlavePipeIntActive);

        if ((!u32PortIntStatus) && (!u32IspIntStatus) && (!u32WCHIntFStart) &&(bSlavePipeIntActive == GK_FALSE)) {
            continue;
        }

        if (!pstDrvCtx->bMemInit) {
            continue;
        }

        if (pstDrvCtx->stStitchAttr.bStitchEnable == GK_TRUE) {
            if (pstDrvCtx->stStitchAttr.bMainPipe == GK_TRUE) {
                s32Ret = ISP_DRV_StitchSync(ViPipe);

                if (s32Ret != GK_SUCCESS) {
                    continue;
                }

                for (j = 0; j < pstDrvCtx->stStitchAttr.u8StitchPipeNum; j++) {
                    ViPipeS = pstDrvCtx->stStitchAttr.as8StitchBindId[j];
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
    GK_S32 s32Ret;

    s32Ret = ISP_DRV_BeRemap();

    if (s32Ret == GK_FAILURE) {
        return s32Ret;
    }

    s32Ret = ISP_DRV_VicapRemap();
    if (s32Ret == GK_FAILURE) {
        return s32Ret;
    }

    s32Ret = ISP_DRV_FeRemap();

    if (s32Ret == GK_FAILURE) {
        return s32Ret;
    }
#ifdef CONFIG_ISP_LITEOS_BOTTOM_HALF_SUPPORT
    ISP_DRV_WorkQueueInit();
#endif
    return 0;
}

static int ISP_DRV_Exit(void)
{
    ISP_DRV_BeUnmap();

    ISP_DRV_VicapUnmap();

    ISP_DRV_FeUnmap();
#ifdef CONFIG_ISP_LITEOS_BOTTOM_HALF_SUPPORT
    ISP_DRV_WorkQueueExit();
#endif

    return 0;
}

#ifdef TEST_TIME
static int ISP_Test_Init(void)
{
    CMPI_MmzMallocNocache(GK_NULL, "ISPStatTest", &g_test_phyaddr, (GK_VOID **)&g_test_pviraddr, 256 * 2);
}

static int ISP_Test_Exit(void)
{
    CMPI_MmzFree(g_test_phyaddr, g_test_pviraddr);
}
#endif


#ifndef DISABLE_DEBUG_INFO
static int ISP_ProcShow(osal_proc_entry_t *s)
{
    GK_U8   i;
    GK_U64  u64BeBufSize;
    VI_PIPE ViPipe = 0;
    ISP_DRV_CTX_S *pstDrvCtx = GK_NULL;
    ISP_SNAP_PIPE_MODE_E enSnapPipeMode = ISP_SNAP_NONE;

    osal_seq_printf(s, "\n");

    do {
        pstDrvCtx = ISP_DRV_GET_CTX(ViPipe);
        if (!pstDrvCtx->bMemInit) {
            continue;
        }

        if ((pstDrvCtx->stSnapAttr.s32PicturePipeId == pstDrvCtx->stSnapAttr.s32PreviewPipeId) &&
            (pstDrvCtx->stSnapAttr.s32PreviewPipeId != -1)) {
            enSnapPipeMode = ISP_SNAP_PREVIEW_PICTURE;
        } else if (pstDrvCtx->stSnapAttr.s32PicturePipeId == ViPipe) {
            enSnapPipeMode = ISP_SNAP_PICTURE;
        } else if (pstDrvCtx->stSnapAttr.s32PreviewPipeId == ViPipe) {
            enSnapPipeMode = ISP_SNAP_PREVIEW;
        } else {
            enSnapPipeMode = ISP_SNAP_NONE;
        }

        osal_seq_printf(s, "------------------------------------------------------------------------------\n");
        osal_seq_printf(s, "------------------------------ ISP PROC PIPE[%d] -----------------------------\n", ViPipe);
        osal_seq_printf(s, "------------------------------------------------------------------------------\n");
        osal_seq_printf(s, "\n");

        osal_seq_printf(s, "-----MODULE/CONTROL PARAM-----------------------------------------------------\n");
        osal_seq_printf(s, " %12s" " %12s" " %12s" " %12s" " %12s" " %12s" " %14s" " %12s" " %12s" " %12s""\n",  "ProcParam",
                        "StatIntvl", "UpdatePos", "IntBothalf", "IntTimeout",
                        "PwmNumber", "PortIntDelay", "QuickStart", "LdciTprFltEn","LongFrmIntEn");
        osal_seq_printf(s, " %12u" " %12u" " %12u" " %12u" " %12u" " %12u" " %14u" " %12d" " %12d" " %12d""\n",
                        g_ProcParam[ViPipe], g_StatIntvl[ViPipe], g_UpdatePos[ViPipe], g_UseBothalf, g_IntTimeout[ViPipe],
                        g_PwmNumber[ViPipe], g_PortIntDelay[ViPipe], g_QuickStart, g_LdciTprFltEn[ViPipe],g_bLongFrmIntEn);
        osal_seq_printf(s, "\n");

        osal_seq_printf(s, "-----ISP Mode------------------------------------------------------------------\n");
        osal_seq_printf(s, " %15s" " %15s" "\n",  "StitchMode", "RunningMode");
        osal_seq_printf(s, " %15s" " %15s" "\n",
                        pstDrvCtx->stStitchAttr.bStitchEnable ? "STITCH" : "NORMAL",
                        (pstDrvCtx->stWorkMode.enIspRunningMode == ISP_MODE_RUNNING_OFFLINE) ? "OFFLINE"  :
                        (pstDrvCtx->stWorkMode.enIspRunningMode == ISP_MODE_RUNNING_ONLINE) ? "ONLINE"   :
                        (pstDrvCtx->stWorkMode.enIspRunningMode == ISP_MODE_RUNNING_SIDEBYSIDE) ? "SBS"      :
                        (pstDrvCtx->stWorkMode.enIspRunningMode == ISP_MODE_RUNNING_STRIPING) ? "STRIPING" : "BUTT");
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
                        (pstDrvCtx->stFrameIntAttr.enIntType == FRAME_INTERRUPT_START) ? "Start" : "Other",
                        pstDrvCtx->stDrvDbgInfo.u32PtIntCnt,
                        pstDrvCtx->stDrvDbgInfo.u32PtIntTime,
                        pstDrvCtx->stDrvDbgInfo.u32PtIntTimeMax,
                        pstDrvCtx->stDrvDbgInfo.u32PtIntGapTime,
                        pstDrvCtx->stDrvDbgInfo.u32PtIntGapTimeMax,
                        pstDrvCtx->stDrvDbgInfo.u32PtRate,
                        pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime,
                        pstDrvCtx->stDrvDbgInfo.u32SensorCfgTimeMax);

        if ((IS_OFFLINE_MODE(pstDrvCtx->stWorkMode.enIspRunningMode) ||
             IS_STRIPING_MODE(pstDrvCtx->stWorkMode.enIspRunningMode)) &&
            (pstDrvCtx->stBeBufInfo.bInit == GK_TRUE)) {

            osal_seq_printf(s, "\n");
            osal_seq_printf(s, "-----BeCfg Phy Addr-------------------------------------------------------- \n");

            for (i = 0; i < MAX_ISP_BE_BUF_NUM; i++) {
                osal_seq_printf(s, "%16s""%d""%s", "BeCfg[", i, "]");
            }

            osal_seq_printf(s, "\n");
            u64BeBufSize = sizeof(ISP_BE_WO_REG_CFG_S);
            for (i = 0; i < MAX_ISP_BE_BUF_NUM; i++) {
                //osal_seq_printf(s, "%#18llx", pstDrvCtx->stBeBufInfo.be_buf_haddr.phy_addr + i * be_buf_size);
                osal_seq_printf(s, "%#18llx", pstDrvCtx->stBeBufInfo.stBeBufHaddr.u64PhyAddr + i * u64BeBufSize);
            }
        }

        osal_seq_printf(s, "\n\n");

        /*  show isp attribute here. width/height/bayer_format, etc..
                  Read parameter from memory directly. */
        osal_seq_printf(s, "-----PubAttr INFO--------------------------------------------------------------\n");

        osal_seq_printf(s, "%12s" "%12s" "%12s" "%12s" "%12s" "%12s" "%12s\n",
                        "WndX", "WndY", "WndW", "WndH", "SnsW", "SnsH", "Bayer");

        osal_seq_printf(s, "%12d" "%12d" "%12d" "%12d" "%12d" "%12d" "%12s\n\n",
                        pstDrvCtx->stProcPubInfo.stWndRect.s32X,
                        pstDrvCtx->stProcPubInfo.stWndRect.s32Y,
                        pstDrvCtx->stProcPubInfo.stWndRect.u32Width,
                        pstDrvCtx->stProcPubInfo.stWndRect.u32Height,
                        pstDrvCtx->stProcPubInfo.stSnsSize.u32Width,
                        pstDrvCtx->stProcPubInfo.stSnsSize.u32Height,
                        (pstDrvCtx->stProcPubInfo.enBayer == 0) ? "RGGB" :
                        (pstDrvCtx->stProcPubInfo.enBayer == 1) ? "GRBG" :
                        (pstDrvCtx->stProcPubInfo.enBayer == 2) ? "GBRG" :
                        (pstDrvCtx->stProcPubInfo.enBayer == 3) ? "BGGR" : "BUTT");
        osal_seq_printf(s, "\n");

        /* show isp snap attribute here. SnapType/PipeMode/OPType/ProFrmNum, etc.. */
        osal_seq_printf(s, "-----SNAPATTR INFO-----------------------------------------------------------------\n");

        osal_seq_printf(s, "%12s" "%12s" "%12s" "%12s\n", "SnapType", "PipeMode", "OPType", "ProFrmNum");
        osal_seq_printf(s, "%12s" "%12s" "%12s" "%12d\n\n",
                        (pstDrvCtx->stSnapAttr.enSnapType == 0) ? "NORMAL" :
                        (pstDrvCtx->stSnapAttr.enSnapType == 1) ? "PRO" : "BUTT",
                        (enSnapPipeMode == 0) ? "NONE" :
                        (enSnapPipeMode == 1) ? "PREVIEW" :
                        (enSnapPipeMode == 2) ? "PICTURE" : "PRE_PIC",
                        (pstDrvCtx->stSnapAttr.stProParam.enOperationMode == 0) ? "Auto" : "Manul",
                        pstDrvCtx->stSnapAttr.stProParam.u8ProFrameNum);

        ISP_DRV_ProcPrintf(ViPipe, s);

        osal_seq_printf(s, "-----------------------------------------------------------------------------\n");
        osal_seq_printf(s, "----------------------------------- ISP PROC END[%d] ------------------------\n", ViPipe);
        osal_seq_printf(s, "-----------------------------------------------------------------------------\n");
        osal_seq_printf(s, "\n\n");
    } while (++ViPipe < ISP_MAX_PIPE_NUM);

    return 0;
}
#endif

GK_S32 ISP_KernInit(void *p)
{
    GK_U32 ViPipe;

    for (ViPipe = 0; ViPipe < ISP_MAX_PIPE_NUM; ViPipe++) {
        osal_memset(&g_astIspDrvCtx[ViPipe].stDrvDbgInfo, 0, sizeof(ISP_DRV_DBG_INFO_S));
    }

    return GK_SUCCESS;
}

GK_VOID ISP_KernExit(void)
{
    GK_U32 ViPipe;

    for (ViPipe = 0; ViPipe < ISP_MAX_PIPE_NUM; ViPipe++) {
        if ((reg_vicap_base_va != GK_NULL) && (reg_ispfe_base_va[ViPipe] != GK_NULL)) {
            ISP_DRV_SetIntEnable(ViPipe, GK_FALSE);
        }
        osal_memset(&g_astIspDrvCtx[ViPipe].stDrvDbgInfo, 0, sizeof(ISP_DRV_DBG_INFO_S));
    }

    return;
}

static GK_U32 ISP_GetVerMagic(GK_VOID)
{
    return VERSION_MAGIC;
}

ISP_EXPORT_FUNC_S g_stIspExpFunc   = {
    .pfnISPRegisterBusCallBack     = ISP_RegisterBusCallBack,
    .pfnISPRegisterPirisCallBack   = ISP_RegisterPirisCallBack,
    .pfnISPRegisterViBusCallBack   = ISP_RegisterViBusCallBack,
    .pfnISP_GetDCFInfo             = ISP_GetDCFInfo,
    .pfnISP_SetDCFInfo             = ISP_SetDCFInfo,
    .pfnISP_GetIspUpdateInfo       = ISP_GetIspUpdateInfo,
    .pfnISP_SetIspUpdateInfo       = ISP_SetIspUpdateInfo,
    .pfnISP_GetFrameInfo           = ISP_GetFrameInfo,
    .pfnISP_SetFrameInfo           = ISP_SetFrameInfo,
    .pfnISP_GetAttachInfo          = ISP_GetAttachInfo,
    .pfnISP_GetColorGamutInfo      = ISP_GetColorGamutInfo,
    .pfnISP_GetDngImageDynamicInfo = ISP_DRV_GetDngImageDynamicInfo,
    .pfnISP_GetProCtrl             = ISP_GetProCtrl,
    .pfnISP_SetSnapAttr            = ISP_SetSnapAttr,
    .pfnISP_SetProNrParam          = ISP_SetProNrParam,
    .pfnISP_SetProShpParam         = ISP_SetProShpParam,
    .pfnISP_GetProNrParam          = ISP_GetProNrParam,
    .pfnISP_GetProShpParam         = ISP_GetProShpParam,
    .pfnISP_SaveSnapConfig         = ISP_SaveSnapConfig,
    .pfnISP_SetProEnable           = ISP_SetProEnable,
    .pfnISP_DRV_GetReadyBeBuf      = ISP_DRV_GetReadyBeBuf,
    .pfnISP_DRV_PutFreeBeBuf       = ISP_DRV_PutFreeBeBuf,
    .pfnISP_DRV_HoldBusyBeBuf      = ISP_DRV_HoldBusyBeBuf,
    .pfnISP_DRV_GetBeSyncPara      = ISP_DRV_GetBeSyncPara,
    .pfnISP_DRV_BeEndIntProc       = ISP_DRV_BeEndIntProc,
    .pfnISPRegisterSyncTask        = isp_sync_task_register,
    .pfnISPUnRegisterSyncTask      = isp_sync_task_unregister,
    .pfnISP_IntBottomHalf          = ISP_IntBottomHalf,
    .pfnISP_ISR                    = ISP_ISR,
    .pfnISP_GetPubAttr             = ISP_GetPubAttr,
    .pfnISP_DRV_GetRgbirFormat     = ISP_DRV_GetRgbirFormat,
    .pfnISP_DRV_SetOnLineStaticsSttAddr   = ISP_DRV_SetOnLineStaticsSttAddr,
};


static UMAP_MODULE_S s_stModule    = {
    .enModId         = MOD_ID_ISP,
    .aModName        = "isp",

    .pfnInit         = ISP_KernInit,
    .pfnExit         = ISP_KernExit,
    .pfnVerChecker   = ISP_GetVerMagic,
    .pstExportFuncs  = &g_stIspExpFunc,
    .pData           = GK_NULL,
};

int ISP_ModInit(void)
{
    GK_U32  ViPipe;

#ifndef DISABLE_DEBUG_INFO
    osal_proc_entry_t *proc = GK_NULL;
#endif

    s_pstISPDevice = osal_createdev("isp_dev");
    s_pstISPDevice->fops = &stIspFileOp;
    s_pstISPDevice->osal_pmops = &stISPDrvOps;
    s_pstISPDevice->minor = UMAP_ISP_MINOR_BASE;

    if (osal_registerdevice(s_pstISPDevice) < 0) {
        GK_PRINT("Kernel: Could not register isp devices\n");
        return GK_FAILURE;
    }

#ifndef DISABLE_DEBUG_INFO
    proc = osal_create_proc_entry(PROC_ENTRY_ISP, GK_NULL);

    if (proc == GK_NULL) {
        GK_PRINT("Kernel: Register isp proc failed!\n");
        goto OUT2;
    }

    proc->read = ISP_ProcShow;
#endif

    if (CMPI_RegisterModule(&s_stModule)) {
        goto OUT1;;
    }

    for (ViPipe = 0; ViPipe < ISP_MAX_PIPE_NUM; ViPipe++) {
        osal_spin_lock_init(&g_stIspLock[ViPipe]);
        osal_spin_lock_init(&g_stIspSyncLock[ViPipe]);
    }

    if (ISP_DRV_Init() != 0) {
        GK_PRINT("isp init failed\n");
        goto OUT1;
    }

#ifdef TEST_TIME
    ISP_Test_Init();
#endif
    GK_PRINT("ISP Mod init!\n");
    return GK_SUCCESS;

OUT1:
#ifndef DISABLE_DEBUG_INFO
    osal_remove_proc_entry(PROC_ENTRY_ISP, GK_NULL);
#endif
OUT2:
    osal_deregisterdevice(s_pstISPDevice);
    osal_destroydev(s_pstISPDevice);

    GK_PRINT("ISP Mod init failed!\n");
    return GK_FAILURE;
}

void ISP_ModExit(void)
{
    int i;

    ISP_DRV_Exit();

    for (i = 0; i < ISP_MAX_PIPE_NUM; i++) {
        osal_spin_lock_destory(&g_stIspLock[i]);
        osal_spin_lock_destory(&g_stIspSyncLock[i]);
    }

    CMPI_UnRegisterModule(MOD_ID_ISP);

#ifndef DISABLE_DEBUG_INFO
    osal_remove_proc_entry(PROC_ENTRY_ISP, GK_NULL);
#endif
    osal_deregisterdevice(s_pstISPDevice);
    osal_destroydev(s_pstISPDevice);

#ifdef TEST_TIME
    ISP_Test_Exit();
#endif

    GK_PRINT("ISP Mod Exit!\n");
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
