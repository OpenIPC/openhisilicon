/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __COMM_VI_H__
#define __COMM_VI_H__

#include "common.h"
#include "errcode.h"
#include "comm_video.h"
#include "comm_gdc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define VI_MAX_ADCHN_NUM              (4UL)

#define VI_PMFCOEF_NUM                (9UL)
#define VI_COMPMASK_NUM               (2UL)
#define VI_PRO_MAX_FRAME_NUM          (8UL)

#define VI_INVALID_FRMRATE            (-1)
#define VI_CHN0                       0
#define VI_CHN1                       1
#define VI_CHN2                       2
#define VI_CHN3                       3
#define VI_INVALID_CHN                (-1)

#define VI_MAX_VC_NUM                 4

typedef struct VI_LOW_DELAY_INFO_S {
    GK_BOOL bEnable;
    GK_U32 u32LineCnt;
} VI_LOW_DELAY_INFO_S;

typedef struct VI_CMP_PARAM_S {
    GK_U8 au8CmpParam[VI_CMP_PARAM_SIZE];
} VI_CMP_PARAM_S;

typedef enum _VI_USERPIC_MODE_E {
    VI_USERPIC_MODE_PIC = 0,
    VI_USERPIC_MODE_BGC,
    VI_USERPIC_MODE_BUTT,
} VI_USERPIC_MODE_E;

typedef struct VI_USERPIC_BGC_S {
    GK_U32 u32BgColor;
} VI_USERPIC_BGC_S;

typedef struct VI_USERPIC_ATTR_S {
    VI_USERPIC_MODE_E       enUsrPicMode;
    union {
        VIDEO_FRAME_INFO_S  stUsrPicFrm;
        VI_USERPIC_BGC_S    stUsrPicBg;
    } unUsrPic;
} VI_USERPIC_ATTR_S;

typedef enum EN_VI_ERR_CODE_E {
    ERR_VI_FAILED_NOTENABLE = 64,
    ERR_VI_FAILED_NOTDISABLE,
    ERR_VI_FAILED_CHNOTDISABLE,
    ERR_VI_CFG_TIMEOUT,
    ERR_VI_NORM_UNMATCH,
    ERR_VI_INVALID_WAYID,
    ERR_VI_INVALID_PHYCHNID,
    ERR_VI_FAILED_NOTBIND,
    ERR_VI_FAILED_BINDED,
    ERR_VI_DIS_PROCESS_FAIL
} EN_VI_ERR_CODE_E;

#define ERR_CODE_VI_INVALID_PARA        DEFINE_ERR_CODE(MOD_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define ERR_CODE_VI_INVALID_DEVID       DEFINE_ERR_CODE(MOD_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)
#define ERR_CODE_VI_INVALID_PIPEID      DEFINE_ERR_CODE(MOD_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_PIPEID)
#define ERR_CODE_VI_INVALID_STITCHGRPID DEFINE_ERR_CODE(MOD_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_STITCHGRPID)
#define ERR_CODE_VI_INVALID_CHNID       DEFINE_ERR_CODE(MOD_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
#define ERR_CODE_VI_INVALID_NULL_PTR    DEFINE_ERR_CODE(MOD_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define ERR_CODE_VI_FAILED_NOTCONFIG    DEFINE_ERR_CODE(MOD_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)
#define ERR_CODE_VI_SYS_NOTREADY        DEFINE_ERR_CODE(MOD_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
#define ERR_CODE_VI_BUF_EMPTY           DEFINE_ERR_CODE(MOD_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
#define ERR_CODE_VI_BUF_FULL            DEFINE_ERR_CODE(MOD_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
#define ERR_CODE_VI_NOMEM               DEFINE_ERR_CODE(MOD_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
#define ERR_CODE_VI_NOT_SUPPORT         DEFINE_ERR_CODE(MOD_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
#define ERR_CODE_VI_BUSY                DEFINE_ERR_CODE(MOD_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)
#define ERR_CODE_VI_NOT_PERM            DEFINE_ERR_CODE(MOD_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)

#define ERR_CODE_VI_FAILED_NOTENABLE    DEFINE_ERR_CODE(MOD_ID_VI, EN_ERR_LEVEL_ERROR, ERR_VI_FAILED_NOTENABLE)
#define ERR_CODE_VI_FAILED_NOTDISABLE   DEFINE_ERR_CODE(MOD_ID_VI, EN_ERR_LEVEL_ERROR, ERR_VI_FAILED_NOTDISABLE)
#define ERR_CODE_VI_FAILED_CHNOTDISABLE DEFINE_ERR_CODE(MOD_ID_VI, EN_ERR_LEVEL_ERROR, ERR_VI_FAILED_CHNOTDISABLE)
#define ERR_CODE_VI_CFG_TIMEOUT         DEFINE_ERR_CODE(MOD_ID_VI, EN_ERR_LEVEL_ERROR, ERR_VI_CFG_TIMEOUT)
#define ERR_CODE_VI_NORM_UNMATCH        DEFINE_ERR_CODE(MOD_ID_VI, EN_ERR_LEVEL_ERROR, ERR_VI_NORM_UNMATCH)
#define ERR_CODE_VI_INVALID_WAYID       DEFINE_ERR_CODE(MOD_ID_VI, EN_ERR_LEVEL_ERROR, ERR_VI_INVALID_WAYID)
#define ERR_CODE_VI_INVALID_PHYCHNID    DEFINE_ERR_CODE(MOD_ID_VI, EN_ERR_LEVEL_ERROR, ERR_VI_INVALID_PHYCHNID)
#define ERR_CODE_VI_FAILED_NOTBIND      DEFINE_ERR_CODE(MOD_ID_VI, EN_ERR_LEVEL_ERROR, ERR_VI_FAILED_NOTBIND)
#define ERR_CODE_VI_FAILED_BINDED       DEFINE_ERR_CODE(MOD_ID_VI, EN_ERR_LEVEL_ERROR, ERR_VI_FAILED_BINDED)

#define ERR_CODE_VI_PIPE_EXIST          DEFINE_ERR_CODE(MOD_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST)
#define ERR_CODE_VI_PIPE_UNEXIST        DEFINE_ERR_CODE(MOD_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)

typedef enum VI_INTF_MODE_E {
    VI_MODE_BT656 = 0,
    VI_MODE_BT656_PACKED_YUV,
    VI_MODE_BT601,
    VI_MODE_DIGITAL_CAMERA,
    VI_MODE_BT1120_STANDARD,
    VI_MODE_BT1120_INTERLEAVED,
    VI_MODE_MIPI,
    VI_MODE_MIPI_YUV420_NORMAL,
    VI_MODE_MIPI_YUV420_LEGACY,
    VI_MODE_MIPI_YUV422,
    VI_MODE_LVDS,
    VI_MODE_HISPI,
    VI_MODE_SLVS,

    VI_MODE_BUTT
} VI_INTF_MODE_E;

typedef enum VI_INPUT_MODE_E {
    VI_INPUT_MODE_BT656 = 0,
    VI_INPUT_MODE_BT601,
    VI_INPUT_MODE_DIGITAL_CAMERA,
    VI_INPUT_MODE_INTERLEAVED,
    VI_INPUT_MODE_MIPI,
    VI_INPUT_MODE_LVDS,
    VI_INPUT_MODE_HISPI,
    VI_INPUT_MODE_SLVS,

    VI_INPUT_MODE_BUTT
} VI_INPUT_MODE_E;

typedef enum VI_WORK_MODE_E {
    VI_WORK_MODE_1Multiplex = 0,
    VI_WORK_MODE_2Multiplex,
    VI_WORK_MODE_3Multiplex,
    VI_WORK_MODE_4Multiplex,

    VI_WORK_MODE_BUTT
} VI_WORK_MODE_E;

typedef enum VI_SCAN_MODE_E {
    VI_SCAN_INTERLACED  = 0,
    VI_SCAN_PROGRESSIVE,

    VI_SCAN_BUTT
} VI_SCAN_MODE_E;

typedef enum VI_YUV_DATA_SEQ_E {
    VI_DATA_SEQ_VUVU = 0,
    VI_DATA_SEQ_UVUV,

    VI_DATA_SEQ_UYVY,
    VI_DATA_SEQ_VYUY,
    VI_DATA_SEQ_YUYV,
    VI_DATA_SEQ_YVYU,

    VI_DATA_SEQ_BUTT
} VI_YUV_DATA_SEQ_E;

typedef enum VI_CLK_EDGE_E {
    VI_CLK_EDGE_SINGLE_UP = 0,
    VI_CLK_EDGE_SINGLE_DOWN,

    VI_CLK_EDGE_BUTT
} VI_CLK_EDGE_E;

typedef enum VI_COMPONENT_MODE_E {
    VI_COMPONENT_MODE_SINGLE = 0,
    VI_COMPONENT_MODE_DOUBLE,

    VI_COMPONENT_MODE_BUTT
} VI_COMPONENT_MODE_E;

typedef enum VI_COMBINE_MODE_E {
    VI_COMBINE_COMPOSITE = 0,
    VI_COMBINE_SEPARATE,

    VI_COMBINE_BUTT
} VI_COMBINE_MODE_E;

typedef enum VI_VSYNC_E {
    VI_VSYNC_FIELD = 0,
    VI_VSYNC_PULSE,

    VI_VSYNC_BUTT
} VI_VSYNC_E;

typedef enum VI_VSYNC_NEG_E {
    VI_VSYNC_NEG_HIGH = 0,
    VI_VSYNC_NEG_LOW,
    VI_VSYNC_NEG_BUTT
} VI_VSYNC_NEG_E;

typedef enum VI_HSYNC_E {
    VI_HSYNC_VALID_SINGNAL = 0,
    VI_HSYNC_PULSE,

    VI_HSYNC_BUTT
} VI_HSYNC_E;

typedef enum VI_HSYNC_NEG_E {
    VI_HSYNC_NEG_HIGH = 0,
    VI_HSYNC_NEG_LOW,
    VI_HSYNC_NEG_BUTT
} VI_HSYNC_NEG_E;

typedef enum VI_VSYNC_VALID_E {
    VI_VSYNC_NORM_PULSE = 0,
    VI_VSYNC_VALID_SINGAL,

    VI_VSYNC_VALID_BUTT
} VI_VSYNC_VALID_E;

typedef enum VI_VSYNC_VALID_NEG_E {
    VI_VSYNC_VALID_NEG_HIGH = 0,
    VI_VSYNC_VALID_NEG_LOW,
    VI_VSYNC_VALID_NEG_BUTT
} VI_VSYNC_VALID_NEG_E;

typedef struct VI_TIMING_BLANK_S {
    GK_U32 u32HsyncHfb ;
    GK_U32 u32HsyncAct ;
    GK_U32 u32HsyncHbb ;
    GK_U32 u32VsyncVfb ;
    GK_U32 u32VsyncVact ;
    GK_U32 u32VsyncVbb ;
    GK_U32 u32VsyncVbfb ;
    GK_U32 u32VsyncVbact ;
    GK_U32 u32VsyncVbbb ;
} VI_TIMING_BLANK_S;

typedef struct VI_SYNC_CFG_S {
    VI_VSYNC_E              enVsync;
    VI_VSYNC_NEG_E          enVsyncNeg;
    VI_HSYNC_E              enHsync;
    VI_HSYNC_NEG_E          enHsyncNeg;
    VI_VSYNC_VALID_E        enVsyncValid;
    VI_VSYNC_VALID_NEG_E    enVsyncValidNeg;
    VI_TIMING_BLANK_S       stTimingBlank;
} VI_SYNC_CFG_S;

typedef enum VI_BT656_FIXCODE_E {
    VI_BT656_FIXCODE_1 = 0,
    VI_BT656_FIXCODE_0,

    VI_BT656_FIXCODE_BUTT
} VI_BT656_FIXCODE_E;

typedef enum VI_BT656_FIELD_POLAR_E {
    VI_BT656_FIELD_POLAR_STD = 0,
    VI_BT656_FIELD_POLAR_NSTD,

    VI_BT656_FIELD_POLAR_BUTT
} VI_BT656_FIELD_POLAR_E;

typedef struct VI_BT656_SYNC_CFG_S {
    VI_BT656_FIXCODE_E     enFixCode;
    VI_BT656_FIELD_POLAR_E enFieldPolar;
} VI_BT656_SYNC_CFG_S;

typedef enum VI_DATA_TYPE_E {
    VI_DATA_TYPE_YUV = 0,
    VI_DATA_TYPE_RGB,

    VI_DATA_TYPE_BUTT
} VI_DATA_TYPE_E;

typedef enum VI_REPHASE_MODE_E {
    VI_REPHASE_MODE_NONE = 0,
    VI_REPHASE_MODE_SKIP_1_2,
    VI_REPHASE_MODE_SKIP_1_3,
    VI_REPHASE_MODE_BINNING_1_2,
    VI_REPHASE_MODE_BINNING_1_3,

    VI_REPHASE_MODE_BUTT
} VI_REPHASE_MODE_E;

typedef struct VI_BAS_REPHASE_ATTR_S {
    VI_REPHASE_MODE_E enHRephaseMode;
    VI_REPHASE_MODE_E enVRephaseMode;
} VI_BAS_REPHASE_ATTR_S;

typedef struct VI_BAS_SCALE_ATTR_S {
    SIZE_S stBasSize;
} VI_BAS_SCALE_ATTR_S;

typedef struct VI_BAS_ATTR_S {
    VI_BAS_SCALE_ATTR_S     stSacleAttr;
    VI_BAS_REPHASE_ATTR_S   stRephaseAttr;
} VI_BAS_ATTR_S;

typedef struct VI_WDR_ATTR_S {
    WDR_MODE_E  enWDRMode;
    GK_U32      u32CacheLine;
} VI_WDR_ATTR_S;

typedef struct VI_DEV_ATTR_EX_S {
    VI_INPUT_MODE_E         enInputMode;
    VI_WORK_MODE_E          enWorkMode;

    VI_COMBINE_MODE_E       enCombineMode;
    VI_COMPONENT_MODE_E     enComponentMode;
    VI_CLK_EDGE_E           enClkEdge;

    GK_U32                  au32ComponentMask[VI_COMPMASK_NUM];

    VI_SCAN_MODE_E          enScanMode;
    GK_S32                  as32AdChnId[VI_MAX_ADCHN_NUM];

    VI_YUV_DATA_SEQ_E       enDataSeq;
    VI_SYNC_CFG_S           stSynCfg;

    VI_BT656_SYNC_CFG_S     stBT656SynCfg;

    VI_DATA_TYPE_E          enInputDataType;

    GK_BOOL                 bDataReverse;

    SIZE_S                  stSize;

    VI_BAS_ATTR_S           stBasAttr;

    VI_WDR_ATTR_S           stWDRAttr;

    DATA_RATE_E             enDataRate;
} VI_DEV_ATTR_EX_S;

typedef struct VI_DEV_ATTR_S {
    VI_INTF_MODE_E      enIntfMode;
    VI_WORK_MODE_E      enWorkMode;

    GK_U32              au32ComponentMask[VI_COMPMASK_NUM];
    VI_SCAN_MODE_E      enScanMode;
    GK_S32              as32AdChnId[VI_MAX_ADCHN_NUM];

    VI_YUV_DATA_SEQ_E   enDataSeq;
    VI_SYNC_CFG_S       stSynCfg;

    VI_DATA_TYPE_E      enInputDataType;

    GK_BOOL             bDataReverse;

    SIZE_S              stSize;

    VI_BAS_ATTR_S       stBasAttr;

    VI_WDR_ATTR_S       stWDRAttr;

    DATA_RATE_E         enDataRate;
} VI_DEV_ATTR_S;

typedef struct VI_DEV_BIND_PIPE_S {
    GK_U32  u32Num;
    VI_PIPE PipeId[VI_MAX_PHY_PIPE_NUM];
} VI_DEV_BIND_PIPE_S;

typedef enum VI_NR_REF_SOURCE_E {
    VI_NR_REF_FROM_RFR = 0,
    VI_NR_REF_FROM_CHN0,

    VI_NR_REF_FROM_BUTT
} VI_NR_REF_SOURCE_E;

typedef enum VI_PIPE_BYPASS_MODE_E {
    VI_PIPE_BYPASS_NONE,
    VI_PIPE_BYPASS_FE,
    VI_PIPE_BYPASS_BE,

    VI_PIPE_BYPASS_BUTT
} VI_PIPE_BYPASS_MODE_E;

typedef struct VI_NR_ATTR_S {
    PIXEL_FORMAT_E      enPixFmt;
    DATA_BITWIDTH_E     enBitWidth;
    VI_NR_REF_SOURCE_E  enNrRefSource;
    COMPRESS_MODE_E     enCompressMode;
} VI_NR_ATTR_S;

typedef struct VI_PIPE_ATTR_S {
    VI_PIPE_BYPASS_MODE_E enPipeBypassMode;
    GK_BOOL               bYuvSkip;
    GK_BOOL               bIspBypass;
    GK_U32                u32MaxW;
    GK_U32                u32MaxH;
    PIXEL_FORMAT_E        enPixFmt;
    COMPRESS_MODE_E       enCompressMode;
    DATA_BITWIDTH_E       enBitWidth;
    GK_BOOL               bNrEn;
    VI_NR_ATTR_S          stNrAttr;
    GK_BOOL               bSharpenEn;
    FRAME_RATE_CTRL_S     stFrameRate;
    GK_BOOL               bDiscardProPic;
} VI_PIPE_ATTR_S;

typedef enum VI_STITCH_ISP_CFG_MODE_E {
    VI_STITCH_ISP_CFG_NORMAL = 0,
    VI_STITCH_ISP_CFG_SYNC,
    VI_STITCH_ISP_CFG_BUTT
} VI_STITCH_ISP_CFG_MODE_E;

typedef struct VI_STITCH_GRP_ATTR_S {
    GK_BOOL                     bStitch;
    VI_STITCH_ISP_CFG_MODE_E    enMode;
    GK_U32                      u32MaxPTSGap;
    GK_U32                      u32PipeNum;
    VI_PIPE                     PipeId[VI_MAX_PIPE_NUM];
} VI_STITCH_GRP_ATTR_S;

typedef enum VI_PIPE_REPEAT_MODE_E {
    VI_PIPE_REPEAT_NONE = 0,
    VI_PIPE_REPEAT_ONCE = 1,
    VI_PIPE_REPEAT_BUTT
} VI_PIPE_REPEAT_MODE_E;

typedef struct {
    GK_U8   IES;
    GK_U8   IESS;
    GK_U16  IEDZ;
} tV1_IEy;

typedef struct {
    GK_U8   SBF     : 2;
    GK_U8   STR     : 4;
    GK_U8   STHp    : 2;
    GK_U8   SFT     : 5;
    GK_U8   kPro    : 3;

    GK_U16  STH[3];
    GK_U16  SBS[3];
    GK_U16  SDS[3];

} tV1_SFy;

typedef struct {
    GK_U16  MATH    : 10;
    GK_U16  MATE    : 4;
    GK_U16  MATW    : 2;
    GK_U8   MASW    : 4;
    GK_U8   MABW    : 3;
    GK_U8   MAXN    : 1;

} tV1_MDy;

typedef struct {
    GK_U8   TFR[4];
    GK_U16  TDZ : 14;
    GK_U16  TDX : 2;
    GK_U16  TFS : 6;
} tV1_TFy;

typedef struct {
    GK_U16  SFC     : 10;
    GK_U16  TFC     : 6;
    GK_U16  CSFS    : 14;
    GK_U16  CSFk    : 2;
    GK_U16  CTFS    : 4;
    GK_U16  CIIR    : 1;
    GK_U16  CTFR    : 11;

} tV1_NRc;

typedef struct {
    tV1_IEy  IEy;
    tV1_SFy  SFy[5];
    tV1_MDy  MDy[2];
    tV1_TFy  TFy[2];

    GK_U16  HdgType : 1;
    GK_U16  BriType : 1;
    GK_U16  HdgMode : 2;
    GK_U16  kTab2   : 1;
    GK_U16  HdgWnd  : 1;
    GK_U16  kTab3   : 1;
    GK_U16  HdgSFR  : 4;
    GK_U16  nOut    : 5;
    GK_U8   HdgIES;
    GK_U8   nRef    : 1;

    GK_U8   IEyMode : 1;
    GK_U8   IEyEx[4];

    GK_U8   SFRi[4];
    GK_U8   SFRk[4];
    GK_U16  SBSk2[32];
    GK_U16  SBSk3[32];
    GK_U16  SDSk2[32];
    GK_U16  SDSk3[32];
    GK_U16  BriThr[16];

    tV1_NRc NRc;
} VI_PIPE_NRX_PARAM_V1_S;

typedef enum VI_NR_VERSION_E {
    VI_NR_V1 = 1,
    VI_NR_V2 = 2,
    VI_NR_V3 = 3,
    VI_NR_V4 = 4,
    VI_NR_BUTT
} VI_NR_VERSION_E;

typedef struct NRX_PARAM_MANUAL_V1_S {
    VI_PIPE_NRX_PARAM_V1_S stNRXParamV1;
} NRX_PARAM_MANUAL_V1_S;

typedef struct NRX_PARAM_AUTO_V1_S {
    GK_U32                              u32ParamNum;
    GK_U32                  ATTRIBUTE   *pau32ISO;
    VI_PIPE_NRX_PARAM_V1_S  ATTRIBUTE   *pastNRXParamV1;
} NRX_PARAM_AUTO_V1_S;

typedef struct NRX_PARAM_V1_S {
    OPERATION_MODE_E        enOptMode;
    NRX_PARAM_MANUAL_V1_S   stNRXManualV1;
    NRX_PARAM_AUTO_V1_S     stNRXAutoV1;
} NRX_PARAM_V1_S;

typedef struct {
    GK_U8  IES0, IES1, IES2, IES3;
    GK_U16 IEDZ : 10, _rb_ : 6;
} tV2_VI_IEy;

typedef struct {
    GK_U8  SPN6 : 3, SFR  : 5;
    GK_U8  SBN6 : 3, PBR6 : 5;
    GK_U16 SRT0 : 5, SRT1 : 5, JMODE : 3, DeIdx : 3;
    GK_U8  DeRate, SFR6[3];
    GK_U8  SFS1, SFT1, SBR1;
    GK_U8  SFS2, SFT2, SBR2;
    GK_U8  SFS4, SFT4, SBR4;

    GK_U16 STH1 : 9, SFN1 : 3, NRyEn : 1, SFN0  : 3;
    GK_U16 STH2 : 9, SFN2 : 3, BWSF4 : 1, kMode : 3;
    GK_U16 STH3 : 9, SFN3 : 3, TriTh : 1, _rb0_ : 3;
} tV2_VI_SFy;

typedef struct {
    tV2_VI_IEy IEy;
    tV2_VI_SFy SFy;
} VI_PIPE_NRX_PARAM_V2_S;

typedef struct NRX_PARAM_MANUAL_V2_S {
    VI_PIPE_NRX_PARAM_V2_S stNRXParamV2;
} NRX_PARAM_MANUAL_V2_S;

typedef struct NRX_PARAM_AUTO_V2_S {
    GK_U32                              u32ParamNum;
    GK_U32                  ATTRIBUTE   *pau32ISO;
    VI_PIPE_NRX_PARAM_V2_S  ATTRIBUTE   *pastNRXParamV2;
} NRX_PARAM_AUTO_V2_S;

typedef struct NRX_PARAM_V2_S {
    OPERATION_MODE_E        enOptMode;
    NRX_PARAM_MANUAL_V2_S   stNRXManualV2;
    NRX_PARAM_AUTO_V2_S     stNRXAutoV2;
} NRX_PARAM_V2_S;

typedef struct VI_PIPE_NRX_PARAM_S {
    VI_NR_VERSION_E enNRVersion;
    union {
        NRX_PARAM_V1_S stNRXParamV1;
        NRX_PARAM_V2_S stNRXParamV2;
    };
} VI_PIPE_NRX_PARAM_S;

typedef struct VI_CHN_ATTR_S {
    SIZE_S              stSize;
    PIXEL_FORMAT_E      enPixelFormat;
    DYNAMIC_RANGE_E     enDynamicRange;
    VIDEO_FORMAT_E      enVideoFormat;
    COMPRESS_MODE_E     enCompressMode;
    GK_BOOL             bMirror;
    GK_BOOL             bFlip;
    GK_U32              u32Depth;
    FRAME_RATE_CTRL_S   stFrameRate;
} VI_CHN_ATTR_S;

typedef struct VI_PIPE_STATUS_S {
    GK_BOOL bEnable;
    GK_U32  u32IntCnt;
    GK_U32  u32FrameRate;
    GK_U32  u32LostFrame;
    GK_U32  u32VbFail;
    SIZE_S  stSize;
} VI_PIPE_STATUS_S;

typedef enum VI_VS_SIGNAL_MODE_E {
    VI_VS_SIGNAL_ONCE = 0,
    VI_VS_SIGNAL_FREQ,

    VI_VS_SIGNAL_MODE_BUTT
} VI_VS_SIGNAL_MODE_E;

typedef struct VI_VS_SIGNAL_ATTR_S {
    VI_VS_SIGNAL_MODE_E enMode;
    GK_U32              u32StartTime;
    GK_U32              u32Duration;
    GK_U32              u32CapFrmIndex;
    GK_U32              u32Interval;
} VI_VS_SIGNAL_ATTR_S;

typedef struct BNR_DUMP_ATTR_S {
    GK_BOOL bEnable;
    GK_U32 u32Depth;
} BNR_DUMP_ATTR_S;

typedef enum VI_EXT_CHN_SOURCE_E {
    VI_EXT_CHN_SOURCE_TAIL,
    VI_EXT_CHN_SOURCE_HEAD,

    VI_EXT_CHN_SOURCE_BUTT
} VI_EXT_CHN_SOURCE_E;

typedef struct VI_EXT_CHN_ATTR_S {
    VI_EXT_CHN_SOURCE_E enSource;
    VI_CHN              s32BindChn;
    SIZE_S              stSize;
    PIXEL_FORMAT_E      enPixFormat;
    DYNAMIC_RANGE_E     enDynamicRange;
    COMPRESS_MODE_E     enCompressMode;
    GK_U32              u32Depth;
    FRAME_RATE_CTRL_S   stFrameRate;
} VI_EXT_CHN_ATTR_S;

typedef enum VI_CROP_COORDINATE_E {
    VI_CROP_RATIO_COOR = 0,
    VI_CROP_ABS_COOR,
    VI_CROP_BUTT
} VI_CROP_COORDINATE_E;

typedef struct VI_CROP_INFO_S {
    GK_BOOL                 bEnable;
    VI_CROP_COORDINATE_E    enCropCoordinate;
    RECT_S                  stCropRect;
} VI_CROP_INFO_S;

typedef struct VI_LDC_ATTR_S {
    GK_BOOL bEnable;
    LDC_ATTR_S stAttr;
} VI_LDC_ATTR_S;

typedef struct VI_LDCV2_ATTR_S {
    GK_BOOL bEnable;
    LDCV2_ATTR_S stAttr;
} VI_LDCV2_ATTR_S;

typedef struct VI_LDCV3_ATTR_S {
    GK_BOOL bEnable;
    LDCV3_ATTR_S stAttr;
} VI_LDCV3_ATTR_S;

typedef struct VI_ROTATION_EX_ATTR_S {
    GK_BOOL       bEnable;
    ROTATION_EX_S stRotationEx;
} VI_ROTATION_EX_ATTR_S;

typedef struct VI_CHN_STATUS_S {
    GK_BOOL bEnable;
    GK_U32  u32FrameRate;
    GK_U32  u32LostFrame;
    GK_U32  u32VbFail;
    SIZE_S  stSize;

} VI_CHN_STATUS_S;

typedef struct VI_PMF_ATTR_S {
    GK_BOOL bEnable;
    SIZE_S  stDestSize;
    GK_S64  as64PMFCoef[VI_PMFCOEF_NUM];
} VI_PMF_ATTR_S;

typedef enum VI_DUMP_TYPE_E {
    VI_DUMP_TYPE_RAW = 0,
    VI_DUMP_TYPE_YUV = 1,
    VI_DUMP_TYPE_IR = 2,
    VI_DUMP_TYPE_BUTT
} VI_DUMP_TYPE_E;

typedef struct VI_DUMP_ATTR_S {
    GK_BOOL         bEnable;
    GK_U32          u32Depth;
    VI_DUMP_TYPE_E  enDumpType;
} VI_DUMP_ATTR_S;

typedef enum VI_PIPE_FRAME_SOURCE_E {
    VI_PIPE_FRAME_SOURCE_DEV = 0,
    VI_PIPE_FRAME_SOURCE_USER_FE,
    VI_PIPE_FRAME_SOURCE_USER_BE,

    VI_PIPE_FRAME_SOURCE_BUTT
} VI_PIPE_FRAME_SOURCE_E;

typedef struct _VI_RAW_INFO_S {
    VIDEO_FRAME_INFO_S      stVideoFrame;
    ISP_CONFIG_INFO_S       stIspInfo;
} VI_RAW_INFO_S;

typedef struct VI_MOD_PARAM_S {
    GK_S32      s32DetectErrFrame;
    GK_U32      u32DropErrFrame;
    VB_SOURCE_E enViVbSource;
} VI_MOD_PARAM_S;

typedef struct VI_DEV_TIMING_ATTR_S {
    GK_BOOL bEnable;
    GK_S32  s32FrmRate;
} VI_DEV_TIMING_ATTR_S;

typedef struct VI_EARLY_INTERRUPT_S {
    GK_BOOL bEnable;
    GK_U32 u32LineCnt;
} VI_EARLY_INTERRUPT_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif


