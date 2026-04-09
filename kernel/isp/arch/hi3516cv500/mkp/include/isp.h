/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/


#ifndef __ISP_H__
#define __ISP_H__

#include "hi_osal.h"
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

typedef struct hiISP_STAT_NODE_S {
    isp_stat_info stStatInfo;
    struct osal_list_head list;
} ISP_STAT_NODE_S;

typedef struct hiISP_STAT_BUF_S {
    HI_BOOL bInit;
    HI_U64  u64PhyAddr;
    HI_VOID *pVirAddr;
    HI_U64  u64Size;
    ISP_STAT_NODE_S astNode[MAX_ISP_STAT_BUF_NUM];
    isp_stat_info *pstActStat;

    HI_U32 u32UserNum;
    HI_U32 u32BusyNum;
    HI_U32 u32FreeNum;
    struct osal_list_head stUserList;
    struct osal_list_head stBusyList;
    struct osal_list_head stFreeList;
} ISP_STAT_BUF_S;

typedef enum hiISP_RUNNING_STATE_E {
    ISP_BE_BUF_STATE_INIT = 0,
    ISP_BE_BUF_STATE_RUNNING,
    ISP_BE_BUF_STATE_FINISH,
    ISP_BE_BUF_STATE_SWITCH_START,
    ISP_BE_BUF_STATE_SWITCH,
    ISP_BE_BUF_STATE_BUTT
} ISP_RUNNING_STATE_E;

typedef enum hiISP_EXIT_STATE_E {
    ISP_BE_BUF_READY = 0,
    ISP_BE_BUF_WAITING,
    ISP_BE_BUF_EXIT,
    ISP_BE_BUF_BUTT
} ISP_EXIT_STATE_E;

/* Sync Cfg */
#define EXP_RATIO_MAX          0xFFF /* max expratio 64X */
#define EXP_RATIO_MIN          0x40  /* min expratio 1X */

#define ISP_DIGITAL_GAIN_SHIFT 8    /* ISP digital gain register shift */
#define ISP_DIGITAL_GAIN_MAX   0xFFFF  /* max ISP digital gain 16X */
#define ISP_DIGITAL_GAIN_MIN   0x100  /* min ISP digital gain 1X */

typedef struct hiISP_SYNC_CFG_BUF_S {
    HI_U8                   u8BufWRFlag;    /* FW write node */
    HI_U8                   u8BufRDFlag;    /* ISR read, then write to ISP reg or sensor */
    isp_sync_cfg_buf_node stSyncCfgBufNode[ISP_SYNC_BUF_NODE_NUM];
} ISP_SYNC_CFG_BUF_S;

typedef struct hiISP_SYNC_CFG_S {
    HI_U8  u8WDRMode;
    HI_U8  u8PreWDRMode;
    HI_U8  u8VCNum;          /* if 3to1, u8VCNum = 0,1,2,0,1,...... */
    HI_U8  u8VCNumMax;       /* if 3to1, u8VCNumMax = 2 */
    HI_U8  u8VCCfgNum;
    HI_U8  u8Cfg2VldDlyMAX;

    HI_U64 u64PreSnsGain;
    HI_U64 u64CurSnsGain;
    HI_U64 u64PreSnsGainSF;
    HI_U64 u64CurSnsGainSF;
    HI_U32 u32DRCComp[CFG2VLD_DLY_LIMIT];  /* drc exposure compensation */
    HI_U32 u32ExpRatio[3][CFG2VLD_DLY_LIMIT];
    HI_U8  u8LFMode[CFG2VLD_DLY_LIMIT];
    HI_U32 u32WDRGain[4][CFG2VLD_DLY_LIMIT];
    HI_U8  u8AlgProc[CFG2VLD_DLY_LIMIT];
    isp_sync_cfg_buf_node  *apstNode[CFG2VLD_DLY_LIMIT + 1];

    ISP_SYNC_CFG_BUF_S   stSyncCfgBuf;
} ISP_SYNC_CFG_S;

typedef struct hiISP_SYNC_STITCH_CTRL_S {
    HI_S8  s8StitchFlag;
    HI_U8  u8ReadAll;
    HI_U8  u8FirstReadAll;
    HI_U8  u8FirstOrder;
    HI_U8  u8AllInit;
    HI_U32 u32ViCallbackCount;
    HI_U64 u64PTS;
} ISP_SYNC_STITCH_CTRL_S;

typedef struct hiISP_DRV_DBG_INFO_S {
    HI_U64 u64IspLastIntTime;           /* Time of last interrupt, for debug */
    HI_U64 u64IspLastRateTime;          /* Time of last interrupt rate, for debug */
    HI_U32 u32IspIntCnt;                /* Count of interrupt, for debug */
    HI_U32 u32IspIntTime;               /* Process time of interrupt, for debug */
    HI_U32 u32IspIntTimeMax;            /* Maximal process time of interrupt, for debug */
    HI_U32 u32IspIntGapTime;            /* Gap of two interrupts, for debug */
    HI_U32 u32IspIntGapTimeMax;         /* Maximal gap of two interrupts, for debug */
    HI_U32 u32IspRateIntCnt;            /* Count of interrupt rate, for debug */
    HI_U32 u32IspRate;                  /* Interrupt Rate, interrupt count per sencond, for debug */

    HI_U64 u64PtLastIntTime;            /* Time of last interrupt, for debug */
    HI_U64 u64PtLastRateTime;           /* Time of last interrupt rate, for debug */
    HI_U32 u32PtIntCnt;                 /* Count of interrupt, for debug */
    HI_U32 u32PtIntTime;                /* Process time of interrupt, for debug */
    HI_U32 u32PtIntTimeMax;             /* Maximal process time of interrupt, for debug */
    HI_U32 u32PtIntGapTime;             /* Gap of two interrupts, for debug */
    HI_U32 u32PtIntGapTimeMax;          /* Maximal gap of two interrupts, for debug */
    HI_U32 u32PtRateIntCnt;             /* Count of interrupt rate, for debug */
    HI_U32 u32PtRate;                   /* Interrupt Rate, interrupt count per sencond, for debug */

    HI_U32 u32SensorCfgTime;            /* Time of sensor config, for debug */
    HI_U32 u32SensorCfgTimeMax;         /* Maximal time of sensor config, for debug */

    HI_U32 u32IspResetCnt;              /* Count of ISP reset when vi width or height changed */
    HI_U32 u32IspBeStaLost;             /* Count of ISP BE statistics lost number When the ISP processing is not timely, for debug */
} ISP_DRV_DBG_INFO_S;

typedef struct hiISP_INTERRUPT_SCH_S {
    HI_U32 u32PortIntStatus;
    HI_U32 u32IspIntStatus;
    HI_U32 u32PortIntErr;
    HI_U32 u32WCHIntStatus;
} ISP_INTERRUPT_SCH_S;

typedef struct {
    ISP_INTERRUPT_SCH_S int_sch[ISP_INTERRUPTS_SAVEINFO_MAX];
    hi_u8 read_flag;
    hi_u8 write_flag;
} isp_interrupt_info;

typedef struct hiISP_DRV_CTX_S {
    HI_BOOL                  bEdge;
    HI_BOOL                  bVdStart;
    HI_BOOL                  bVdEnd;
    HI_BOOL                  bYuvMode;
    HI_BOOL                  bVdBeEnd;
    HI_BOOL                  bMemInit;
    HI_BOOL                  bIspInit;
    hi_bool                  pub_attr_ok;
    HI_BOOL                  bStitchSync;
    HI_BOOL                  bDngInfoInit;
    HI_BOOL                  bProEnable;
    HI_BOOL                  bProStart;
    HI_BOOL                  bRunOnceOk;
    HI_BOOL                  bRunOnceFlag;
    HI_BOOL                  bYUVRunOnceOk;
    HI_BOOL                  bYUVRunOnceFlag;
    HI_BOOL                  bIspRunFlag;
    HI_CHAR                  acName[8];
    HI_U8                    u8Vcnum;
    HI_U32                   u32IntPos;
    HI_U32                   u32FrameCnt;
    HI_U32                   u32Status;
    HI_U32                   u32bit16IsrAccess;
    HI_U32                   u32ProTrigFlag;
    HI_U32                   u32RegCfgInfoFlag;
    HI_U8                    *pu8SaveSttStat;

    isp_chn_switch           astChnSelAttr[ISP_STRIPING_MAX_NUM];
    ISP_RUNNING_STATE_E      enIspRunningState;
    ISP_EXIT_STATE_E         enIspExitState;
    isp_working_mode         stWorkMode;
    vi_stitch_attr           stStitchAttr;
    vi_pipe_wdr_attr         stWDRAttr;
    isp_wdr_cfg              stWDRCfg;
    ISP_SYNC_CFG_S           stSyncCfg;
    ISP_STAT_BUF_S           stStatisticsBuf;
    isp_kernel_reg_cfg       astKernelCfg[2]; /* ping pong reg cfg info */
    isp_be_cfg_buf_info      stBeBufInfo;
    ISP_BE_BUF_QUEUE_S       stBeBufQueue;
    ISP_BE_BUF_NODE_S       *pstUseNode;
    isp_be_sync_para         stIspBeSyncPara;
    ISP_SYNC_STITCH_CTRL_S   stIspSyncCtrl;
    isp_be_sync_para         stIspBePreSyncPara;
    isp_be_lut2stt_attr      stBeLut2SttAttr;
    isp_fe_stt_attr          stFeSttAttr;
    isp_fe_stit_stt_attr     stFeStitSttAttr;
    isp_be_online_stt_info   stBeOnlineSttBuf;
    isp_be_off_stt_attr      stBeOffSttAttr;
    isp_be_stit_stt_attr     stBeOffStitSttAttr;
    isp_ldci_buf             stLdciReadBufAttr;
    isp_ldci_buf             stLdciWriteBufAttr;
    isp_ldci_stt_addr        stLdciSttAddr;
    isp_clut_buf             stClutBufAttr;
    isp_spec_awb_buf         stSpecAwbBufAttr;
    isp_bus_callback         stBusCb;
    isp_piris_callback       stPirisCb;
    isp_vibus_callback       stViBusCb;
    ISP_DRV_DBG_INFO_S       stDrvDbgInfo;
    hi_isp_pub_attr          stProcPubInfo;
    isp_trans_info_buf       stTransInfo;
    isp_pro_info_buf         stProInfo;
    isp_proc_mem             stPorcMem;
    struct osal_semaphore    stProcSem;
    ISP_INTERRUPT_SCH_S      stIntSch;   /* isp interrupt schedule */
    isp_interrupt_info       isp_int_info;

    osal_wait_t              stIspWait;
    osal_wait_t              stIspWaitVdStart;
    osal_wait_t              stIspWaitVdEnd;
    osal_wait_t              stIspWaitVdBeEnd;
    osal_wait_t              stIspExitWait;
    struct osal_semaphore    stProInfoSem;
    struct osal_semaphore    stIspSem;
    struct osal_semaphore    stIspSemVd;
    struct osal_semaphore    stIspSemBeVd;
    hi_isp_config_info       astSnapInfoSave[ISP_SAVEINFO_MAX]; /* Frame end and start use index 0 and 1 respectively. */
    isp_snap_info            stSnapInfoLoad;
    isp_snap_attr            stSnapAttr;
    hi_dng_image_dynamic_info stDngImageDynamicInfo[2];
    hi_frame_interrupt_attr  stFrameIntAttr;
    hi_isp_ir_bayer_format   enIspRgbirFormat;
    hi_isp_dcf_update_info   stUpdateInfo;
    hi_isp_frame_info        stFrameInfo;
} ISP_DRV_CTX_S;

extern ISP_DRV_CTX_S   g_astIspDrvCtx[ISP_MAX_PIPE_NUM];

#define ISP_DRV_GET_CTX(ViPipe) (&g_astIspDrvCtx[ViPipe])

#ifdef CONFIG_HI_ISP_LITEOS_BOTTOM_HALF_SUPPORT
typedef struct hiISP_WORKQUEUE_CTX_S {
    VI_PIPE ViPipe;
    struct osal_work_struct worker;
    struct osal_semaphore sem;
} ISP_WORKQUEUE_CTX_S;
ISP_WORKQUEUE_CTX_S g_astIspWorkQueueCtx;
#endif

/*  * * * * * * * Buffer control begin * * * * * * *  */
static HI_S32 __inline ISPSyncBufInit(ISP_SYNC_CFG_BUF_S *pstSyncCfgBuf)
{
    osal_memset(pstSyncCfgBuf, 0, sizeof(ISP_SYNC_CFG_BUF_S));
    return 0;
}

static HI_S32 __inline ISPSyncBufIsFull(ISP_SYNC_CFG_BUF_S *pstSyncCfgBuf)
{
    return ((pstSyncCfgBuf->u8BufWRFlag + 1) % ISP_SYNC_BUF_NODE_NUM) == pstSyncCfgBuf->u8BufRDFlag;
}

static HI_S32 __inline ISPSyncBufIsEmpty(ISP_SYNC_CFG_BUF_S *pstSyncCfgBuf)
{
    return pstSyncCfgBuf->u8BufWRFlag == pstSyncCfgBuf->u8BufRDFlag;
}

static hi_s32 __inline isp_interrupt_buf_is_full(isp_interrupt_info *isp_int_info)
{
    return ((isp_int_info->write_flag + 1) % ISP_INTERRUPTS_SAVEINFO_MAX) == isp_int_info->read_flag;
}

static hi_s32 __inline isp_interrupt_buf_is_empty(isp_interrupt_info *isp_int_info)
{
    return isp_int_info->write_flag == isp_int_info->read_flag;
}

static HI_S32 __inline ISPSyncBufIsErr(ISP_SYNC_CFG_BUF_S *pstSyncCfgBuf)
{
    if (ISPSyncBufIsEmpty(pstSyncCfgBuf)) {
        return 0;
    }

    if (((pstSyncCfgBuf->u8BufRDFlag + 1) % ISP_SYNC_BUF_NODE_NUM) == pstSyncCfgBuf->u8BufWRFlag) {
        return 0;
    }

    return 1;
}

static HI_S32 __inline ISPSyncBufWrite(ISP_SYNC_CFG_BUF_S *pstSyncCfgBuf,
                                       isp_sync_cfg_buf_node *pstSyncCfgBufNode)
{
    if (ISPSyncBufIsFull(pstSyncCfgBuf)) {
        return -1;
    }

    osal_memcpy(&pstSyncCfgBuf->stSyncCfgBufNode[pstSyncCfgBuf->u8BufWRFlag], pstSyncCfgBufNode,
                sizeof(isp_sync_cfg_buf_node));
    pstSyncCfgBuf->u8BufWRFlag = (pstSyncCfgBuf->u8BufWRFlag + 1) % ISP_SYNC_BUF_NODE_NUM;

    return 0;
}

static HI_S32 __inline ISPSyncBufRead(ISP_SYNC_CFG_BUF_S *pstSyncCfgBuf, isp_sync_cfg_buf_node **ppstSyncCfgBufNode)
{
    if (ISPSyncBufIsEmpty(pstSyncCfgBuf)) {
        return -1;
    }

    *ppstSyncCfgBufNode = &pstSyncCfgBuf->stSyncCfgBufNode[pstSyncCfgBuf->u8BufRDFlag];
    pstSyncCfgBuf->u8BufRDFlag = (pstSyncCfgBuf->u8BufRDFlag + 1) % ISP_SYNC_BUF_NODE_NUM;

    return 0;
}

static HI_S32 __inline ISPSyncBufRead2(ISP_SYNC_CFG_BUF_S *pstSyncCfgBuf, isp_sync_cfg_buf_node **ppstSyncCfgBufNode)
{
    HI_U8 u8Tmp = 0;

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
HI_S32 IspSyncTaskProcess(VI_PIPE ViPipe);
HI_S32 ISP_DRV_StaKernelGet(VI_PIPE ViPipe, ISP_DRV_AF_STATISTICS_S *pstFocusStat);
HI_U8 ISP_DRV_GetBeSyncIndex(ISP_DRV_CTX_S *pstDrvCtx);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_H__ */
