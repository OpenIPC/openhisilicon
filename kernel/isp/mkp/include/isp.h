/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __ISP_H__
#define __ISP_H__

#include "osal.h"
#include "mkp_isp.h"
#include "isp_ext.h"
#include "isp_vreg.h"
#include "isp_block.h"
#include "vi_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct ISP_STAT_NODE_S {
    ISP_STAT_INFO_S stStatInfo;
    struct osal_list_head list;
} ISP_STAT_NODE_S;

typedef struct ISP_STAT_BUF_S {
    GK_BOOL bInit;
    GK_U64  u64PhyAddr;
    GK_VOID *pVirAddr;
    GK_U64  u64Size;
    ISP_STAT_NODE_S astNode[MAX_ISP_STAT_BUF_NUM];
    ISP_STAT_INFO_S *pstActStat;

    GK_U32 u32UserNum;
    GK_U32 u32BusyNum;
    GK_U32 u32FreeNum;
    struct osal_list_head stUserList;
    struct osal_list_head stBusyList;
    struct osal_list_head stFreeList;
} ISP_STAT_BUF_S;

typedef enum ISP_RUNNING_STATE_E {
    ISP_BE_BUF_STATE_INIT = 0,
    ISP_BE_BUF_STATE_RUNNING,
    ISP_BE_BUF_STATE_FINISH,
    ISP_BE_BUF_STATE_SWITCH_START,
    ISP_BE_BUF_STATE_SWITCH,
    ISP_BE_BUF_STATE_BUTT
} ISP_RUNNING_STATE_E;

typedef enum ISP_EXIT_STATE_E {
    ISP_BE_BUF_READY = 0,
    ISP_BE_BUF_WAITING,
    ISP_BE_BUF_EXIT,
    ISP_BE_BUF_BUTT
} ISP_EXIT_STATE_E;

typedef enum ISP_CFG_SENSOR_TIME_E {
    ISP_CFG_SNS_VERY_SHORT_FRAME = 0, //main pipe
    ISP_CFG_SNS_SHORT_FRAME,
    ISP_CFG_SNS_MIDDLE_FRAME,
    ISP_CFG_SNS_LONG_FRAME,
    ISP_CFG_SNS_BUTT
} ISP_CFG_SENSOR_TIME_E;


/* Sync Cfg */
#define EXP_RATIO_MAX          0xFFF /* max expratio 64X */
#define EXP_RATIO_MIN          0x40  /* min expratio 1X */

#define ISP_DIGITAL_GAIN_SHIFT 8    /* ISP digital gain register shift */
#define ISP_DIGITAL_GAIN_MAX   0xFFFF  /* max ISP digital gain 16X */
#define ISP_DIGITAL_GAIN_MIN   0x100  /* min ISP digital gain 1X */

typedef struct ISP_SYNC_CFG_BUF_S {
    GK_U8                   u8BufWRFlag;    /* FW write node */
    GK_U8                   u8BufRDFlag;    /* ISR read, then write to ISP reg or sensor */
    ISP_SYNC_CFG_BUF_NODE_S stSyncCfgBufNode[ISP_SYNC_BUF_NODE_NUM];
} ISP_SYNC_CFG_BUF_S;

typedef struct ISP_SYNC_CFG_S {
    GK_U8  u8WDRMode;
    GK_U8  u8PreWDRMode;
    GK_U8  u8VCNum;          /* if 3to1, u8VCNum = 0,1,2,0,1,...... */
    GK_U8  u8VCNumMax;       /* if 3to1, u8VCNumMax = 2 */
    GK_U8  u8VCCfgNum;
    GK_U8  u8Cfg2VldDlyMAX;

    GK_U64 u64PreSnsGain;
    GK_U64 u64CurSnsGain;
    GK_U64 u64PreSnsGainSF;
    GK_U64 u64CurSnsGainSF;
    GK_U32 u32DRCComp[CFG2VLD_DLY_LIMIT];  /* drc exposure compensation */
    GK_U32 u32ExpRatio[3][CFG2VLD_DLY_LIMIT];
    GK_U8  u8LFMode[CFG2VLD_DLY_LIMIT];
    GK_U32 u32WDRGain[4][CFG2VLD_DLY_LIMIT];
    GK_U8  u8AlgProc[CFG2VLD_DLY_LIMIT];
    ISP_SYNC_CFG_BUF_NODE_S  *apstNode[CFG2VLD_DLY_LIMIT + 1];

    ISP_SYNC_CFG_BUF_S   stSyncCfgBuf;
} ISP_SYNC_CFG_S;

typedef struct ISP_SYNC_STITCH_CTRL_S {
    GK_S8  s8StitchFlag;
    GK_U8  u8ReadAll;
    GK_U8  u8FirstReadAll;
    GK_U8  u8FirstOrder;
    GK_U8  u8AllInit;
    GK_U32 u32ViCallbackCount;
    GK_U64 u64PTS;
} ISP_SYNC_STITCH_CTRL_S;

typedef struct ISP_DRV_DBG_INFO_S {
    GK_U64 u64IspLastIntTime;           /* Time of last interrupt, for debug */
    GK_U64 u64IspLastRateTime;          /* Time of last interrupt rate, for debug */
    GK_U32 u32IspIntCnt;                /* Count of interrupt, for debug */
    GK_U32 u32IspIntTime;               /* Process time of interrupt, for debug */
    GK_U32 u32IspIntTimeMax;            /* Maximal process time of interrupt, for debug */
    GK_U32 u32IspIntGapTime;            /* Gap of two interrupts, for debug */
    GK_U32 u32IspIntGapTimeMax;         /* Maximal gap of two interrupts, for debug */
    GK_U32 u32IspRateIntCnt;            /* Count of interrupt rate, for debug */
    GK_U32 u32IspRate;                  /* Interrupt Rate, interrupt count per sencond, for debug */

    GK_U64 u64PtLastIntTime;            /* Time of last interrupt, for debug */
    GK_U64 u64PtLastRateTime;           /* Time of last interrupt rate, for debug */
    GK_U32 u32PtIntCnt;                 /* Count of interrupt, for debug */
    GK_U32 u32PtIntTime;                /* Process time of interrupt, for debug */
    GK_U32 u32PtIntTimeMax;             /* Maximal process time of interrupt, for debug */
    GK_U32 u32PtIntGapTime;             /* Gap of two interrupts, for debug */
    GK_U32 u32PtIntGapTimeMax;          /* Maximal gap of two interrupts, for debug */
    GK_U32 u32PtRateIntCnt;             /* Count of interrupt rate, for debug */
    GK_U32 u32PtRate;                   /* Interrupt Rate, interrupt count per sencond, for debug */

    GK_U32 u32SensorCfgTime;            /* Time of sensor config, for debug */
    GK_U32 u32SensorCfgTimeMax;         /* Maximal time of sensor config, for debug */

    GK_U32 u32IspResetCnt;              /* Count of ISP reset when vi width or height changed */
    GK_U32 u32IspBeStaLost;             /* Count of ISP BE statistics lost number When the ISP processing is not timely, for debug */
} ISP_DRV_DBG_INFO_S;

typedef struct ISP_INTERRUPT_SCH_S {
    GK_U32 u32PortIntStatus;
    GK_U32 u32IspIntStatus;
    GK_U32 u32PortIntErr;
    GK_U32 u32WCHIntStatus;
} ISP_INTERRUPT_SCH_S;

typedef struct ISP_SLAVE_PIPE_INT_STATUS_S{
    VI_PIPE ViPipeId;
    GK_U32 u32IspIntStatus;
}ISP_SLAVE_PIPE_INT_STATUS_S;
typedef struct ISP_DRV_CTX_S {
    GK_BOOL                  bEdge;
    GK_BOOL                  bVdStart;
    GK_BOOL                  bVdEnd;
    GK_BOOL                  bYuvMode;
    GK_BOOL                  bVdBeEnd;
    GK_BOOL                  bMemInit;
    GK_BOOL                  bIspInit;
    GK_BOOL                  bStitchSync;
    GK_BOOL                  bDngInfoInit;
    GK_BOOL                  bProEnable;
    GK_BOOL                  bProStart;
    GK_BOOL                  bRunOnceOk;
    GK_BOOL                  bRunOnceFlag;
    GK_BOOL                  bYUVRunOnceOk;
    GK_BOOL                  bYUVRunOnceFlag;
    GK_BOOL                  bIspRunFlag;
    GK_CHAR                  acName[8];
    GK_U8                    u8Vcnum;
    GK_U32                   u32IntPos;
    GK_U32                   u32FrameCnt;
    GK_U32                   u32Status;
    GK_U32                   u32bit16IsrAccess;
    GK_U32                   u32ProTrigFlag;
    GK_U32                   u32RegCfgInfoFlag;
    GK_U8                    *pu8SaveSttStat;

    ISP_CHN_SWITCH_S         astChnSelAttr[ISP_STRIPING_MAX_NUM];
    ISP_RUNNING_STATE_E      enIspRunningState;
    ISP_EXIT_STATE_E         enIspExitState;
    ISP_WORKING_MODE_S       stWorkMode;
    VI_STITCH_ATTR_S         stStitchAttr;
    VI_PIPE_WDR_ATTR_S       stWDRAttr;
    ISP_WDR_CFG_S            stWDRCfg;
    ISP_SYNC_CFG_S           stSyncCfg;
    ISP_STAT_BUF_S           stStatisticsBuf;
    ISP_KERNEL_REG_CFG_S     astKernelCfg[2]; /* ping pong reg cfg info */
    ISP_BE_CFG_BUF_INFO_S    stBeBufInfo;
    ISP_BE_BUF_QUEUE_S       stBeBufQueue;
    ISP_BE_BUF_NODE_S       *pstUseNode;
    ISP_BE_SYNC_PARA_S       stIspBeSyncPara;
    ISP_SYNC_STITCH_CTRL_S   stIspSyncCtrl;
    ISP_BE_SYNC_PARA_S       stIspBePreSyncPara;
    ISP_BE_LUT2STT_ATTR_S    stBeLut2SttAttr;
    ISP_FE_STT_ATTR_S        stFeSttAttr;
    ISP_FE_STIT_STT_ATTR_S   stFeStitSttAttr;
    ISP_BE_ONLINE_STT_INFO_S stBeOnlineSttBuf;
    ISP_BE_OFF_STT_ATTR_S    stBeOffSttAttr;
    ISP_BE_STIT_STT_ATTR_S   stBeOffStitSttAttr;
    ISP_LDCI_BUF_S           stLdciReadBufAttr;
    ISP_LDCI_BUF_S           stLdciWriteBufAttr;
    ISP_LDCI_STT_ADDR_S      stLdciSttAddr;
    ISP_CLUT_BUF_S           stClutBufAttr;
    ISP_SPEC_AWB_BUF_S       stSpecAwbBufAttr;
    ISP_BUS_CALLBACK_S       stBusCb;
    ISP_PIRIS_CALLBACK_S     stPirisCb;
    ISP_VIBUS_CALLBACK_S     stViBusCb;
    ISP_DRV_DBG_INFO_S       stDrvDbgInfo;
    ISP_PUB_ATTR_S           stProcPubInfo;
    ISP_TRANS_INFO_BUF_S     stTransInfo;
    ISP_PRO_INFO_BUF_S       stProInfo;
    ISP_PROC_MEM_S           stPorcMem;
    struct osal_semaphore    stProcSem;
    ISP_INTERRUPT_SCH_S      stIntSch;   /* isp interrupt schedule */

    osal_wait_t              stIspWait;
    osal_wait_t              stIspWaitVdStart;
    osal_wait_t              stIspWaitVdEnd;
    osal_wait_t              stIspWaitVdBeEnd;
    osal_wait_t              stIspExitWait;
    struct osal_semaphore    stProInfoSem;
    struct osal_semaphore    stIspSem;
    struct osal_semaphore    stIspSemVd;
    struct osal_semaphore    stIspSemBeVd;
    ISP_CONFIG_INFO_S        astSnapInfoSave[ISP_SAVEINFO_MAX]; /* Frame end and start use index 0 and 1 respectively. */
    ISP_SNAP_INFO_S          stSnapInfoLoad;
    ISP_SNAP_ATTR_S          stSnapAttr;
    DNG_IMAGE_DYNAMIC_INFO_S stDngImageDynamicInfo[2];
    FRAME_INTERRUPT_ATTR_S   stFrameIntAttr;
    ISP_IRBAYER_FORMAT_E     enIspRgbirFormat;
    ISP_DCF_UPDATE_INFO_S    stUpdateInfo;
    ISP_FRAME_INFO_S         stFrameInfo;
    ISP_SLAVE_PIPE_INT_STATUS_S stSlavePipeIntStatus[ISP_WDR_CHN_MAX -1];
} ISP_DRV_CTX_S;

extern ISP_DRV_CTX_S   g_astIspDrvCtx[ISP_MAX_PIPE_NUM];

#define ISP_DRV_GET_CTX(ViPipe) (&g_astIspDrvCtx[ViPipe])

#ifdef CONFIG_ISP_LITEOS_BOTTOM_HALF_SUPPORT
typedef struct ISP_WORKQUEUE_CTX_S {
    VI_PIPE ViPipe;
    struct osal_work_struct worker;
    struct osal_semaphore sem;
} ISP_WORKQUEUE_CTX_S;
ISP_WORKQUEUE_CTX_S g_astIspWorkQueueCtx;
#endif

/*  * * * * * * * Buffer control begin * * * * * * *  */
static GK_S32 __inline ISPSyncBufInit(ISP_SYNC_CFG_BUF_S *pstSyncCfgBuf)
{
    osal_memset(pstSyncCfgBuf, 0, sizeof(ISP_SYNC_CFG_BUF_S));
    return 0;
}

static GK_S32 __inline ISPSyncBufIsFull(ISP_SYNC_CFG_BUF_S *pstSyncCfgBuf)
{
    return ((pstSyncCfgBuf->u8BufWRFlag + 1) % ISP_SYNC_BUF_NODE_NUM) == pstSyncCfgBuf->u8BufRDFlag;
}

static GK_S32 __inline ISPSyncBufIsEmpty(ISP_SYNC_CFG_BUF_S *pstSyncCfgBuf)
{
    return pstSyncCfgBuf->u8BufWRFlag == pstSyncCfgBuf->u8BufRDFlag;
}

static GK_S32 __inline ISPSyncBufIsErr(ISP_SYNC_CFG_BUF_S *pstSyncCfgBuf)
{
    if (ISPSyncBufIsEmpty(pstSyncCfgBuf)) {
        return 0;
    }

    if (((pstSyncCfgBuf->u8BufRDFlag + 1) % ISP_SYNC_BUF_NODE_NUM) == pstSyncCfgBuf->u8BufWRFlag) {
        return 0;
    }

    return 1;
}

static GK_S32 __inline ISPSyncBufWrite(ISP_SYNC_CFG_BUF_S *pstSyncCfgBuf,
                                       ISP_SYNC_CFG_BUF_NODE_S *pstSyncCfgBufNode)
{
    if (ISPSyncBufIsFull(pstSyncCfgBuf)) {
        return -1;
    }

    osal_memcpy(&pstSyncCfgBuf->stSyncCfgBufNode[pstSyncCfgBuf->u8BufWRFlag], pstSyncCfgBufNode,
                sizeof(ISP_SYNC_CFG_BUF_NODE_S));
    pstSyncCfgBuf->u8BufWRFlag = (pstSyncCfgBuf->u8BufWRFlag + 1) % ISP_SYNC_BUF_NODE_NUM;

    return 0;
}

static GK_S32 __inline ISPSyncBufRead(ISP_SYNC_CFG_BUF_S *pstSyncCfgBuf, ISP_SYNC_CFG_BUF_NODE_S **ppstSyncCfgBufNode)
{
    if (ISPSyncBufIsEmpty(pstSyncCfgBuf)) {
        return -1;
    }

    *ppstSyncCfgBufNode = &pstSyncCfgBuf->stSyncCfgBufNode[pstSyncCfgBuf->u8BufRDFlag];
    pstSyncCfgBuf->u8BufRDFlag = (pstSyncCfgBuf->u8BufRDFlag + 1) % ISP_SYNC_BUF_NODE_NUM;

    return 0;
}

static GK_S32 __inline ISPSyncBufRead2(ISP_SYNC_CFG_BUF_S *pstSyncCfgBuf, ISP_SYNC_CFG_BUF_NODE_S **ppstSyncCfgBufNode)
{
    GK_U8 u8Tmp = 0;

    if (ISPSyncBufIsEmpty(pstSyncCfgBuf)) {
        return -1;
    }

    u8Tmp = (pstSyncCfgBuf->u8BufWRFlag + ISP_SYNC_BUF_NODE_NUM - 1) % ISP_SYNC_BUF_NODE_NUM;

    *ppstSyncCfgBufNode = &pstSyncCfgBuf->stSyncCfgBufNode[u8Tmp];
    pstSyncCfgBuf->u8BufRDFlag = (u8Tmp + 1) % ISP_SYNC_BUF_NODE_NUM;

    return 0;
}

// isp sync task
extern void SyncTaskInit(VI_PIPE ViPipe);
extern void SyncTaskExit(VI_PIPE ViPipe);
GK_S32 IspSyncTaskProcess(VI_PIPE ViPipe);
GK_S32 ISP_DRV_StaKernelGet(VI_PIPE ViPipe, ISP_DRV_AF_STATISTICS_S *pstFocusStat);
GK_U8 ISP_DRV_GetBeSyncIndex(ISP_DRV_CTX_S *pstDrvCtx);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_H__ */
