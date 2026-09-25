/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __VI_EXT_H__
#define __VI_EXT_H__

#include "type.h"
#include "common.h"
#include "comm_isp.h"
#include "comm_vi.h"
#include "comm_video.h"
#include "comm_sys.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* Pipe work mode */
typedef enum VI_PIPE_MODE_E {
    VI_PIPE_MODE_OFFLINE = 0, /* Offline mode */
    VI_PIPE_MODE_ONLINE, /* Online mode */
    VI_PIPE_MODE_PARALLEL, /* Parallel mode */

    VI_PIPE_MODE_BUTT
} VI_PIPE_MODE_E;

typedef struct VI_FLASH_CALLBACK_S {
    XMEDIA_S32 (*pfnViGetFlashStatus)(XMEDIA_VOID);
} VI_FLASH_CALLBACK_S;

typedef struct VI_PIPE_HDR_ATTR_S {
    DYNAMIC_RANGE_E enDynamicRange;
    PIXEL_FORMAT_E enFormat;
    XMEDIA_BOOL bMirror;
} VI_PIPE_HDR_ATTR_S;

typedef struct VI_PIPE_WDR_ATTR_S {
    VI_DEV ViDev;
    WDR_MODE_E enWDRMode;
    VI_DEV_BIND_PIPE_S stDevBindPipe;
    XMEDIA_BOOL bMastPipe;
} VI_PIPE_WDR_ATTR_S;

/*
Defines the ISP stitch attribute.
*/
typedef struct VI_STITCH_ATTR_S {
    XMEDIA_BOOL bStitchEnable;
    XMEDIA_BOOL bMainPipe;
    XMEDIA_U8 u8StitchPipeNum;
    XMEDIA_S8 as8StitchBindId[VI_MAX_PIPE_NUM];
} VI_STITCH_ATTR_S;

typedef struct ISP_STITCH_SYNC_BE_STATS_S {
    XMEDIA_VOID *pstSyncBeStt[VI_MAX_PIPE_NUM];
} ISP_STITCH_SYNC_BE_STATS_S;

typedef struct VI_SNAP_STATUS_S {
    XMEDIA_BOOL bDiscardFrame;
    XMEDIA_U32 u32FrameFlag;
} VI_SNAP_STATUS_S;

typedef struct VI_PIPE_SPLIT_ATTR_S {
    VI_VPSS_MODE_E enMode;
    XMEDIA_S32 s32SplitNum;
    XMEDIA_U32 u32OverLap;
    RECT_S astRect[VI_MAX_SPLIT_NODE_NUM];
    RECT_S astWchOutRect;
    PIXEL_FORMAT_E enPixFmt;
} VI_PIPE_SPLIT_ATTR_S;

typedef struct VI_PROC_MODULE_PARAM_S {
    XMEDIA_BOOL bViProcEn[VIPROC_IP_NUM];
} VI_PROC_MODULE_PARAM_S;

typedef enum SNAP_ISP_STATE_E {
    SNAP_STATE_NULL = 0,
    SNAP_STATE_CFG = 1,
    SNAP_STATE_BUTT
} SNAP_ISP_STATE_E;

typedef struct ISP_SNAP_INFO_S {
    SNAP_ISP_STATE_E enSnapState;
    XMEDIA_U32 u32ProIndex;
    ISP_CONFIG_INFO_S stIspCfgInfo;
} ISP_SNAP_INFO_S;

typedef XMEDIA_S32 FN_VI_RegisterFlashCallBack(XMEDIA_S32 ViDev, VI_FLASH_CALLBACK_S *pstFlashCb);
typedef XMEDIA_S32 FN_VI_GetPipeWDRAttr(VI_PIPE ViPipe, VI_PIPE_WDR_ATTR_S *pstWDRAttr);
typedef XMEDIA_S32 FN_VI_GetPipeHDRAttr(VI_PIPE ViPipe, VI_PIPE_HDR_ATTR_S *pstHDRAttr);
typedef XMEDIA_S32 FN_VI_GetPipeBindDevId(VI_PIPE ViPipe, VI_DEV *ViDev);
typedef XMEDIA_S32 FN_VI_GetPipeBindDevId(VI_PIPE ViPipe, VI_DEV *ViDev);
typedef XMEDIA_S32 FN_VI_GetBeSttInfo(VI_PIPE ViPipe, XMEDIA_U8 u8Block, XMEDIA_VOID *pstBeStt);
typedef XMEDIA_S32 FN_VI_GetPipeStitchAttr(VI_PIPE ViPipe, VI_STITCH_ATTR_S *pstStitchAttr);
typedef XMEDIA_S32 FN_VI_GetStitchSyncBeSttInfo(VI_PIPE ViPipe, ISP_STITCH_SYNC_BE_STATS_S *pstStitchSyncStats);
typedef XMEDIA_S32 FN_VI_UpdateViVpssMode(VI_VPSS_MODE_S *pstViVpssMode);
typedef XMEDIA_S32 FN_VI_GetPipeSnapStatus(VI_PIPE ViPipe, VI_SNAP_STATUS_S *pstViSnapStatus);
typedef XMEDIA_S32 FN_VI_GetSplitAttr(VI_PIPE ViPipe, VI_PIPE_SPLIT_ATTR_S *pstSplitAttr);
typedef XMEDIA_S32 FN_VI_GetProcModuleParam(VI_PROC_MODULE_PARAM_S *pstProcModuleParam);
typedef XMEDIA_S32 FN_VI_DownSemaphore(VI_PIPE ViPipe);
typedef XMEDIA_S32 FN_VI_UpSemaphore(VI_PIPE ViPipe);
typedef XMEDIA_BOOL FN_VI_IsPipeExisted(VI_PIPE ViPipe);
typedef XMEDIA_S32 FN_VI_GetPipeBindDevSize(VI_PIPE ViPipe, SIZE_S *pstSize);
typedef XMEDIA_S32 FN_VI_GetPipeNRXStatus(VI_PIPE ViPipe, XMEDIA_BOOL *pbNrxEn);
typedef XMEDIA_S32 FN_VI_SetVpssFrameInterruptAttr(VI_PIPE ViPipe, const FRAME_INTERRUPT_ATTR_S *pstFrameIntAttr);
typedef XMEDIA_S32 FN_VI_GetViFrameInterruptAttr(VI_PIPE ViPipe, FRAME_INTERRUPT_ATTR_S *pstFrameIntAttr);
typedef XMEDIA_S32 FN_VI_IspClearInputQueue(VI_PIPE ViPipe);
typedef XMEDIA_S32 FN_VI_GetIspConfig(VI_PIPE ViPipe, ISP_SNAP_INFO_S *pstSnapIspInfo);

typedef struct VI_EXPORT_FUNC_S {
    FN_VI_GetStitchSyncBeSttInfo *pfnViGetStitchSyncBeSttInfo;
    FN_VI_GetPipeStitchAttr *pfnViGetPipeStitchAttr;
    FN_VI_GetPipeWDRAttr *pfnViGetPipeWDRAttr;
    FN_VI_GetPipeHDRAttr *pfnViGetPipeHDRAttr;
    FN_VI_GetPipeBindDevId *pfnViGetPipeBindDevId;
    FN_VI_GetBeSttInfo *pfnViGetBeSttInfo;
    FN_VI_RegisterFlashCallBack *pfnViRegisterFlashCallBack;
    FN_VI_UpdateViVpssMode *pfnVIUpdateViVpssMode;
    FN_VI_GetPipeSnapStatus *pfnViGetPipeSnapStatus;
    FN_VI_GetSplitAttr *pfnViGetSplitAttr;
    FN_VI_GetProcModuleParam *pfnViGetProcModuleParam;
    FN_VI_DownSemaphore *pfnViDownSemaphore;
    FN_VI_UpSemaphore *pfnViUpSemaphore;
    FN_VI_IsPipeExisted *pfnViIsPipeExisted;
    FN_VI_GetPipeBindDevSize *pfnViGetPipeBindDevSize;
    FN_VI_GetPipeNRXStatus *pfnViGetPipeNRXStatus;
    FN_VI_SetVpssFrameInterruptAttr *pfnViSetVpssFrameInterruptAttr;
    FN_VI_GetViFrameInterruptAttr *pfnViGetViFrameInterruptAttr;
    FN_VI_IspClearInputQueue *pfnViIspClearInputQueue;
    FN_VI_GetIspConfig *pfnViGetIspConfig;

} VI_EXPORT_FUNC_S;

#define CKFN_VI_IspClearInputQueue() \
    (FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViIspClearInputQueue != XMEDIA_NULL)
#define CALL_VI_IspClearInputQueue(ViPipe) \
    FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViIspClearInputQueue(ViPipe)

#define CKFN_VI_GetViFrameInterruptAttr() \
    (FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViGetViFrameInterruptAttr != XMEDIA_NULL)
#define CALL_VI_GetViFrameInterruptAttr(ViPipe, pstFrameIntAttr) \
    FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViGetViFrameInterruptAttr(ViPipe, pstFrameIntAttr)

#define CKFN_VI_SetVpssFrameInterruptAttr() \
    (FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViSetVpssFrameInterruptAttr != XMEDIA_NULL)
#define CALL_VI_SetVpssFrameInterruptAttr(ViPipe, pstFrameIntAttr) \
    FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViSetVpssFrameInterruptAttr(ViPipe, pstFrameIntAttr)

#define CKFN_VI_UpdateViVpssMode() \
    (FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnVIUpdateViVpssMode != XMEDIA_NULL)
#define CALL_VI_UpdateViVpssMode(pstViVpssMode) \
    FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnVIUpdateViVpssMode(pstViVpssMode)

#define CKFN_VI_GetPipeStitchAttr() \
    (FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViGetPipeStitchAttr != XMEDIA_NULL)
#define CALL_VI_GetPipeStitchAttr(ViPipe, pstStitchAttr) \
    FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViGetPipeStitchAttr(ViPipe, pstStitchAttr)

#define CKFN_VI_GetStitchSyncBeSttInfo() \
    (FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViGetStitchSyncBeSttInfo != XMEDIA_NULL)
#define CALL_VI_GetStitchSyncBeSttInfo(ViPipe, pstStitchSyncStats) \
    FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViGetStitchSyncBeSttInfo(ViPipe, pstStitchSyncStats)

#define CKFN_VI_GetPipeWDRAttr() \
    (FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViGetPipeWDRAttr != XMEDIA_NULL)
#define CALL_VI_GetPipeWDRAttr(ViPipe, pstWDRAttr) \
    FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViGetPipeWDRAttr(ViPipe, pstWDRAttr)

#define CKFN_VI_GetPipeHDRAttr() \
    (FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViGetPipeHDRAttr != XMEDIA_NULL)
#define CALL_VI_GetPipeHDRAttr(ViPipe, pstHDRAttr) \
    FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViGetPipeHDRAttr(ViPipe, pstHDRAttr)

#define CKFN_VI_GetPipeBindDevId() \
    (FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViGetPipeBindDevId != XMEDIA_NULL)
#define CALL_VI_GetPipeBindDevId(ViPipe, ViDev) \
    FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViGetPipeBindDevId(ViPipe, ViDev)

#define CKFN_VI_GetBeSttInfo() \
    (FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViGetBeSttInfo != XMEDIA_NULL)
#define CALL_VI_GetBeSttInfo(ViPipe, u8Block, pstBeStt) \
    FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViGetBeSttInfo(ViPipe, u8Block, pstBeStt)

#define CKFN_VI_RegisterFlashCallBack() \
    (FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViRegisterFlashCallBack != XMEDIA_NULL)
#define CALL_VI_RegisterFlashCallBack(ViDev, pstFlashCb) \
    FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViRegisterFlashCallBack(ViDev, pstFlashCb)

#define CKFN_VI_GetSnapStatus() \
    (FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViGetPipeSnapStatus != XMEDIA_NULL)
#define CALL_VI_GetSnapStatus(ViPipe, pstViSnapStatus) \
    FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViGetPipeSnapStatus(ViPipe, pstViSnapStatus)

#define CKFN_VI_GetSplitAttr() \
    (FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViGetSplitAttr != XMEDIA_NULL)
#define CALL_VI_GetSplitAttr(ViPipe, pstSplitAttr) \
    FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViGetSplitAttr(ViPipe, pstSplitAttr)

#define CKFN_VI_GetProcModuleParam() \
    (FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViGetProcModuleParam != XMEDIA_NULL)
#define CALL_VI_GetProcModuleParam(pstProcModuleParam) \
    FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViGetProcModuleParam(pstProcModuleParam)

#define CKFN_VI_DownSemaphore() \
    (FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViDownSemaphore != XMEDIA_NULL)
#define CALL_VI_DownSemaphore(ViPipe) \
    FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViDownSemaphore(ViPipe)

#define CKFN_VI_UpSemaphore() \
    (FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViUpSemaphore != XMEDIA_NULL)
#define CALL_VI_UpSemaphore(ViPipe) \
    FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViUpSemaphore(ViPipe)

#define CKFN_VI_IsPipeExisted() \
    (FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViIsPipeExisted != XMEDIA_NULL)
#define CALL_VI_IsPipeExisted(ViPipe) \
    FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViIsPipeExisted(ViPipe)

#define CKFN_VI_GetPipeBindDevSize() \
    (FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViGetPipeBindDevSize != XMEDIA_NULL)
#define CALL_VI_GetPipeBindDevSize(ViPipe, pstSize) \
    FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViGetPipeBindDevSize(ViPipe, pstSize)

#define CKFN_VI_GetPipeNRXStatus() \
    (FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViGetPipeNRXStatus != XMEDIA_NULL)
#define CALL_VI_GetPipeNRXStatus(ViPipe, pbNrxEn) \
    FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViGetPipeNRXStatus(ViPipe, pbNrxEn)

#define CKFN_VI_GetIspConfig() \
    (FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViGetIspConfig != XMEDIA_NULL)
#define CALL_VI_GetIspConfig(ViPipe, pstSnapIspInfo) \
    FUNC_ENTRY(VI_EXPORT_FUNC_S, MOD_ID_VI)->pfnViGetIspConfig(ViPipe, pstSnapIspInfo)

typedef struct _VI_CAP_PIC_INFO_S {
    VIDEO_FRAME_INFO_S stVideoFrame;
    ISP_CONFIG_INFO_S stIspConfigInfo;
    ISP_DCF_UPDATE_INFO_S stDCFUpdateInfo;
    XMEDIA_U64 u64StartTime;
    XMEDIA_U64 u64EndTime;
    XMEDIA_U64 vicap_start_time;
    XMEDIA_U64 left_end_time;
} VI_CAP_PIC_INFO_S;

typedef struct VI_BUF_INFO_S {
    VI_CAP_PIC_INFO_S stPicInfo;
    XMEDIA_BOOL bBufOk;
    XMEDIA_BOOL bCapOK;
    XMEDIA_BOOL bSended;
    XMEDIA_BOOL bLost;
} VI_BUF_INFO_S;

typedef enum VI_FRAME_INFO_E {
    FRAME_NORMAL = 0x0,
    FRAME_LOWDELAY,
    FRAME_LOWDELAY_FIN,
} VI_FRAME_INFO_E;

typedef enum VI_FRAME_SOURCE_TYPE_E {
    VI_FRAME_SOURCE_NORMAL = 0x0,
    VI_FRAME_SOURCE_USERPIC,
    VI_FRAME_SOURCE_BUTT,
} VI_FRAME_SOURCE_TYPE_E;

typedef struct _VI_PIC_INFO_S {
    VIDEO_FRAME_INFO_S stVideoFrame;
    VIDEO_FRAME_INFO_S stSingleYFrame;
    XMEDIA_U64 u64StartTime;
    XMEDIA_U64 u64EndTime;
    VI_FRAME_INFO_E enFrameInfo;
    VI_FRAME_SOURCE_TYPE_E enFrameSource;
} VI_PIC_INFO_S;

typedef struct _VI_SEND_INFO_S {
    VI_PIPE ViPipe;
    VI_CHN ViChn;
    VI_PIC_INFO_S *pstDestPicInfo;
} VI_SEND_INFO_S;

typedef enum VI_FPN_WORK_MODE_E {
    FPN_MODE_NONE = 0x0,
    FPN_MODE_CORRECTION,
    FPN_MODE_CALIBRATE,
    FPN_MODE_BUTT
} VI_FPN_WORK_MODE_E;

typedef struct VI_FPN_ATTR_S {
    VI_FPN_WORK_MODE_E enFpnWorkMode;
    union {
        ISP_FPN_CALIBRATE_ATTR_S stCalibrateAttr;
        ISP_FPN_ATTR_S stCorrectionAttr;
    };
} VI_FPN_ATTR_S;

XMEDIA_S32 MPI_VI_SetFPNAttr(VI_PIPE ViPipe, VI_FPN_ATTR_S *pstFPNAttr);
XMEDIA_S32 MPI_VI_GetFPNAttr(VI_PIPE ViPipe, VI_FPN_ATTR_S *pstFPNAttr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

