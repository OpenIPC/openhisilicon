/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __ISP_MAIN_H__
#define __ISP_MAIN_H__

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

#include <pthread.h>

#include "comm_3a.h"
#include "isp_debug.h"
#include "mkp_isp.h"
#include "isp_dcfinfo.h"
#include "isp_frameinfo.h"
#include "isp_dnginfo.h"
#include "isp_block.h"
#include "vi_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define ISP_MAX_ALGS_NUM     64

typedef struct ISP_LIB_NODE_S {
    XMEDIA_BOOL bUsed;
    ALG_LIB_S stAlgLib;
    union {
        ISP_AE_REGISTER_S stAeRegsiter;
        ISP_AWB_REGISTER_S stAwbRegsiter;
    };
} ISP_LIB_NODE_S;

typedef struct ISP_LIB_INFO_S {
    XMEDIA_U32         u32ActiveLib; /* the lib used present. */
    ISP_LIB_NODE_S astLibs[MAX_REGISTER_ALG_LIB_NUM];
} ISP_LIB_INFO_S;

typedef struct ISP_ALG_FUNC_S {
    XMEDIA_S32 (*pfn_alg_init)(VI_PIPE ViPipe, XMEDIA_VOID *pRegCfg);
    XMEDIA_S32 (*pfn_alg_run)(VI_PIPE ViPipe,
                          const XMEDIA_VOID *pStatInfo,
                          XMEDIA_VOID *pRegCfg,
                          XMEDIA_S32 s32Rsv);
    XMEDIA_S32 (*pfn_alg_ctrl)(VI_PIPE ViPipe, XMEDIA_U32 u32Cmd, XMEDIA_VOID *pValue);
    XMEDIA_S32 (*pfn_alg_exit)(VI_PIPE ViPipe);
} ISP_ALG_FUNC_S;

typedef struct ISP_ALG_NODE_S {
    XMEDIA_BOOL        bUsed;
    ISP_ALG_MOD_E  enAlgType;
    ISP_ALG_FUNC_S stAlgFunc;
} ISP_ALG_NODE_S;

#define ISP_SYNC_ISO_BUF_MAX 6

typedef struct ISP_DNG_CCM_S {
    XMEDIA_U16 au16HighCcm[9];
    XMEDIA_U16 au16LowCcm[9];
    XMEDIA_U16 u16HighColorTemp;
    XMEDIA_U16 u16LowColorTemp;
} ISP_DNG_CCM_S;

typedef struct ISP_LINKAGE_S {
    XMEDIA_BOOL bDefectPixel;  // each alg need to check this status

    XMEDIA_U32  u32IspDgain;
    XMEDIA_U32  u32Again;
    XMEDIA_U32  u32Dgain;
    XMEDIA_U32  u32IspDgainShift;
    XMEDIA_U32  u32Iso;
    XMEDIA_U32  u32IsoMax;
    XMEDIA_U32  u32SensorIso;
    XMEDIA_U32  u32ExpRatio;
    XMEDIA_U32  au32ExpRatio[3];
    XMEDIA_U32  au32WDRGain[4];
    XMEDIA_U32  u32Inttime;
    XMEDIA_U32  u32PirisGain;
    XMEDIA_U8   u8AERunInterval;
    ISP_FSWDR_MODE_E enFSWDRMode;
    ISP_FSWDR_MODE_E enPreFSWDRMode;
    XMEDIA_U32  u32ColorTemp;
    XMEDIA_U32  au32WhiteBalanceGain[4];
    XMEDIA_U16  au16CCM[9];
    XMEDIA_U8   u8Saturation;
    XMEDIA_BOOL bSnapState;
    XMEDIA_BOOL bProTrigger;
    XMEDIA_BOOL bLoadCCM;
    SNAP_TYPE_E enSnapType;
    ISP_SNAP_PIPE_MODE_E enSnapPipeMode;
    XMEDIA_S32  s32PreviewPipeId;
    XMEDIA_S32  s32PicturePipeId;
    ISP_RUNNING_MODE_E enPreviewRunningMode;
    ISP_RUNNING_MODE_E enPictureRunningMode;
    XMEDIA_U32  u32UpdatePos;
    XMEDIA_U32  au32SyncIsoBuf[ISP_SYNC_ISO_BUF_MAX];
    XMEDIA_U32  au32SyncAllExpRatioBuf[ISP_SYNC_ISO_BUF_MAX];
    XMEDIA_U32  au32SyncExpRatioBuf[3][ISP_SYNC_ISO_BUF_MAX];
    XMEDIA_U32  au8ProIndexBuf[ISP_SYNC_ISO_BUF_MAX];
    XMEDIA_U8   u8ProIndex;
    XMEDIA_U32  u32IsoDoneFrmcnt;
    XMEDIA_U8   u8Cfg2ValidDelayMax;
    XMEDIA_BOOL bFlicker;
    XMEDIA_U8   u8Freqresult;
    XMEDIA_BOOL bStatReady;
    XMEDIA_BOOL bRunOnce;
} ISP_LINKAGE_S;

typedef struct ISP_PARA_REC_S {
    XMEDIA_BOOL bWDRCfg;
    XMEDIA_BOOL bPubCfg;
    XMEDIA_BOOL bInit;
    XMEDIA_BOOL bRun;
    XMEDIA_BOOL bStitchSync;

    XMEDIA_BOOL bRunEn;
} ISP_PARA_REC_S;

typedef struct ISP_SPECIAL_OPT_S {
    XMEDIA_BOOL abBeOnSttUpdate[ISP_MAX_BE_NUM];
    XMEDIA_BOOL bFeSttUpdate;
} ISP_SPECIAL_OPT_S;

typedef struct ISP_SENSOR_IMAGE_MODE_S {
    XMEDIA_U16   u16Width;
    XMEDIA_U16   u16Height;
    XMEDIA_FLOAT f32Fps;
    XMEDIA_U8 u8SnsMode;
} ISP_SENSOR_IMAGE_MODE_S;
typedef struct ISP_BAS_OUT_IMAGE_MODE_S {
    XMEDIA_U16 u16OutWidth;
    XMEDIA_U16 u16OutHeight;
} ISP_BAS_OUT_IMAGE_MODE_S;

typedef union ISP_ALG_KEY_U {
    XMEDIA_U64 u64Key;
    struct {
        XMEDIA_U64  bit1Ae          : 1 ;   /* [0] */
        XMEDIA_U64  bit1Awb         : 1 ;   /* [1] */
        XMEDIA_U64  bit1Af          : 1 ;   /* [2] */
        XMEDIA_U64  bit1Dp          : 1 ;   /* [3] */
        XMEDIA_U64  bit1Ge          : 1 ;   /* [4] */
        XMEDIA_U64  bit1Wdr         : 1 ;   /* [5] */
        XMEDIA_U64  bit1Expander    : 1 ;   /* [6] */
        XMEDIA_U64  bit1Blc         : 1 ;   /* [7] */
        XMEDIA_U64  bit1Bayernr     : 1 ;   /* [8] */
        XMEDIA_U64  bit1Split       : 1 ;   /* [9] */
        XMEDIA_U64  bit1Flicker     : 1 ;   /* [10] */
        XMEDIA_U64  bit1Dg          : 1 ;   /* [11] */
        XMEDIA_U64  bit1Hrs         : 1 ;   /* [12] */
        XMEDIA_U64  bit1FeLsc       : 1 ;   /* [13] */
        XMEDIA_U64  bit1Lsc         : 1 ;   /* [14] */
        XMEDIA_U64  bit1RLsc        : 1 ;   /* [15] */
        XMEDIA_U64  bit1Rc          : 1 ;   /* [16] */
        XMEDIA_U64  bit1Drc         : 1 ;   /* [17] */
        XMEDIA_U64  bit1Dehaze      : 1 ;   /* [18] */
        XMEDIA_U64  bit1LCac        : 1 ;   /* [19] */
        XMEDIA_U64  bit1GCac        : 1 ;   /* [20] */
        XMEDIA_U64  bit1Demosaic    : 1 ;   /* [21] */
        XMEDIA_U64  bit1Fcr         : 1 ;   /* [22] */
        XMEDIA_U64  bit1Gamma       : 1 ;   /* [23] */
        XMEDIA_U64  bit1Csc         : 1 ;   /* [24] */
        XMEDIA_U64  bit1Ca          : 1 ;   /* [25] */
        XMEDIA_U64  bit1Fpn         : 1;    /* [26] */
        XMEDIA_U64  bit1Sharpen     : 1;    /* [27] */
        XMEDIA_U64  bit1EdgeMark    : 1 ;   /* [28] */
        XMEDIA_U64  bit1Mcds        : 1 ;   /* [29] */
        XMEDIA_U64  bit1Ldci        : 1 ;   /* [30] */
        XMEDIA_U64  bit1PreGamma    : 1 ;   /* [31] */
        XMEDIA_U64  bit1FeLogLUT    : 1 ;   /* [32] */
        XMEDIA_U64  bit1LogLUT      : 1 ;   /* [33] */
        XMEDIA_U64  bit1Clut        : 1 ;   /* [34] */
        XMEDIA_U64  bit1Hlc         : 1 ;   /* [35] */
        XMEDIA_U64  bit1Detail      : 1 ;   /* [36] */
        XMEDIA_U64  bit1Rgbir       : 1 ;   /* [37] */
        XMEDIA_U64  bit26Rsv        : 26;   /* [38:63] */
    };
} ISP_ALG_KEY_U;

typedef struct ISP_CTX_S {
    /* 1. ctrl param */
    XMEDIA_BOOL                  bMemInit;
    XMEDIA_BOOL                  bSnsReg;
    XMEDIA_BOOL                  bISPYuvMode;
    ISP_PARA_REC_S           stIspParaRec;
    RECT_S                   stSysRect;
    ISP_BLOCK_ATTR_S         stBlockAttr;
    pthread_mutex_t          stLock;
    ISP_SPECIAL_OPT_S        stSpecialOpt;

    /* 2. algorithm ctrl param */
    XMEDIA_BOOL                  bFreezeFw;
    XMEDIA_BOOL                  bChangeWdrMode;
    XMEDIA_BOOL                  bChangeImageMode;
    XMEDIA_BOOL                  bChangeIspRes;
    XMEDIA_BOOL                  bIsp0P2En;
    XMEDIA_BOOL                  bLdciTprFltEn;
    XMEDIA_U8                    u8Count;
    XMEDIA_U8                    u8PreSnsWDRMode;
    XMEDIA_U8                    u8SnsWDRMode;
    XMEDIA_U32                   u32FrameCnt;    /* frame count for 3a libs which can decide the interval of calculate. */
    ISP_BAYER_FORMAT_E       enBayer;
    ISP_ALG_KEY_U            unAlgKey;

    XMEDIA_U8                    u8IspImageModeFlag;
    ISP_SENSOR_IMAGE_MODE_S  stPreSnsImageMode;
    ISP_SENSOR_IMAGE_MODE_S  stSnsImageMode;

    ISP_AE_RESULT_S          stAeResult;
    ISP_AWB_RESULT_S         stAwbResult;

    ISP_BAS_OUT_IMAGE_MODE_S stPreBasImageMode;
    ISP_BAS_OUT_IMAGE_MODE_S stCurBasImageMode;
    SIZE_S                   stPipeSize;
    VI_PIPE_HDR_ATTR_S       stHdrAttr;
    VI_PIPE_WDR_ATTR_S       stWdrAttr;
    ISP_STITCH_ATTR_S        stStitchAttr;
    COLOR_GAMUT_E            eColorGamut;
    ISP_SMART_INFO_S         stSmartInfo;

    /* 3. 3a register info */
    ISP_BIND_ATTR_S          stBindAttr;
    ISP_LIB_INFO_S           stAeLibInfo;
    ISP_LIB_INFO_S           stAwbLibInfo;
    ISP_LIB_INFO_S           stAfLibInfo;

    /* 4. algorithm ctrl */
    ISP_ALG_NODE_S           astAlgs[ISP_MAX_ALGS_NUM];
    ISP_LINKAGE_S            stLinkage;

    /* 5. debug info */
    ISP_DBG_CTRL_S           stIspDbg;

    /* 6. DCF info */
    ISP_TRANS_INFO_BUF_S     stIspTransInfo;
    ISP_PRO_INFO_BUF_S       stIspProInfo;

    ISP_UPDATE_INFO_CTRL_S   stUpdateInfoCtrl;
    ISP_FRAME_INFO_CTRL_S    stFrameInfoCtrl;
    ISP_ATTACH_INFO_CTRL_S   stAttachInfoCtrl;
    ISP_GAMMUT_INFO_CTRL_S   stGamutInfoCtrl;
    ISP_PRO_NR_PARAM_CTRL_S  stProNrParamCtrl;
    ISP_PRO_SHP_PARAM_CTRL_S stProShpParamCtrl;
    ISP_DCF_UPDATE_INFO_S    stDcfUpdateInfo;
    ISP_FRAME_INFO_S         stFrameInfo;

    /* DNG info */
    ISP_DNG_INFO_CTRL_S      stDngInfoCtrl;
    ISP_DNG_COLORPARAM_S     stPreDngColorParam;
    ISP_DNG_CCM_S            stPreDngCcm;

    ISP_PRO_PARAM_S          stProParam;
    XMEDIA_U8                    u8ProFrmCnt;
    ISP_SNAP_INFO_S          stSnapIspInfo;

    /* 7. ViProc/BE Addr */
    XMEDIA_VOID                 *pIspBeVirtAddr[ISP_STRIPING_MAX_NUM];
    XMEDIA_VOID                 *pViProcVirtAddr[ISP_STRIPING_MAX_NUM];
} ISP_CTX_S;

extern XMEDIA_S32    g_as32IspFd[ISP_MAX_PIPE_NUM];
extern ISP_CTX_S g_astIspCtx[ISP_MAX_PIPE_NUM];
extern ISP_REGCFG_S *g_pastRegCfgCtx[ISP_MAX_PIPE_NUM];
extern const XMEDIA_U32 g_au32IsoLut[ISP_AUTO_ISO_STRENGTH_NUM];

#define ISP_REGCFG_GET_CTX(dev, pstCtx) (pstCtx = g_pastRegCfgCtx[dev])
#define ISP_REGCFG_SET_CTX(dev, pstCtx) (g_pastRegCfgCtx[dev] = pstCtx)
#define ISP_REGCFG_RESET_CTX(dev)       (g_pastRegCfgCtx[dev] = XMEDIA_NULL)

#define ISP_CHECK_OPEN(dev)                    \
    do {                                       \
        XMEDIA_S32 s32Ret = ISP_CheckDevOpen(dev); \
        if (s32Ret != XMEDIA_SUCCESS) {            \
            return s32Ret;                     \
        }                                      \
    } while (0)

#define ISP_CHECK_MEM_INIT(dev)                \
    do {                                       \
        XMEDIA_S32 s32Ret = ISP_CheckMemInit(dev); \
        if (s32Ret != XMEDIA_SUCCESS) {            \
            return s32Ret;                     \
        }                                      \
    } while (0)

#define ISP_CHECK_ISP_WDR_CFG(dev)                                            \
    do {                                                                      \
        if (g_astIspCtx[dev].stIspParaRec.bWDRCfg != XMEDIA_TRUE) {               \
            ISP_TRACE(MODULE_DBG_ERR, "Isp[%d] WDR mode doesn't config!\n", dev); \
            return ERR_CODE_ISP_ATTR_NOT_CFG;                                   \
        }                                                                     \
    } while (0)

#define ISP_CHECK_ISP_PUB_ATTR_CFG(dev)                                       \
    do {                                                                      \
        if (g_astIspCtx[dev].stIspParaRec.bPubCfg != XMEDIA_TRUE) {               \
            ISP_TRACE(MODULE_DBG_ERR, "Isp[%d] Pub Attr doesn't config!\n", dev); \
            return ERR_CODE_ISP_ATTR_NOT_CFG;                                   \
        }                                                                     \
    } while (0)

#define ISP_CHECK_ISP_INIT(dev)                                           \
    do {                                                                  \
        if (g_astIspCtx[dev].stIspParaRec.bInit != XMEDIA_TRUE) {             \
            ISP_TRACE(MODULE_DBG_ERR, "Isp[%d] doesn't initialized!\n", dev); \
            return ERR_CODE_ISP_NOT_INIT;                                   \
        }                                                                 \
    } while (0)

#define ISP_CHECK_OFFLINE_MODE(dev)                                                \
    do {                                                                           \
        if ((IS_ONLINE_MODE(g_astIspCtx[dev].stBlockAttr.enIspRunningMode)) ||  \
            (IS_SIDEBYSIDE_MODE(g_astIspCtx[dev].stBlockAttr.enIspRunningMode))) { \
            return XMEDIA_SUCCESS;                                                     \
        }                                                                          \
    } while (0)

#define ISP_GET_CTX(dev, pstIspCtx)    \
    do {                               \
        pstIspCtx = &g_astIspCtx[dev]; \
    } while (0)

/*
 * HIGH LEVEL functions used to set-up and run ISP.
 */
/*
 * This function executes the auto-exposure algorithm
 * and its results are used to drive sensor parameters,
 * drc engine and noise reduction algorithms,
 * Results are based on the collected image histogram data.
 */
XMEDIA_S32 ISP_Run(VI_PIPE ViPipe);
XMEDIA_S32 ISP_Exit(VI_PIPE ViPipe);
XMEDIA_S32 ISP_StitchSyncExit(VI_PIPE ViPipe);

XMEDIA_S32 ISP_SwitchWDR(VI_PIPE ViPipe);
XMEDIA_S32 ISP_SwitchWDRMode(VI_PIPE ViPipe);
XMEDIA_S32 ISP_SwitchImageMode(VI_PIPE ViPipe);
XMEDIA_S32 ISP_SwitchMode(VI_PIPE ViPipe);

XMEDIA_S32 ISP_WDRCfgSet(VI_PIPE ViPipe);
XMEDIA_U32 ISP_FrameCntGet(VI_PIPE ViPipe);

XMEDIA_S32 ISP_CheckDevOpen(ISP_DEV dev);
XMEDIA_S32 ISP_CheckMemInit(ISP_DEV dev);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_MAIN_H__ */
