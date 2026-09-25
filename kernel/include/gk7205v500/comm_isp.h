/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __ISP_COMM_ISP_H__
#define __ISP_COMM_ISP_H__

#include "type.h"
#include "errcode.h"
#include "common.h"
#include "isp_debug.h"
#include "comm_video.h"
#include "isp_defines.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#define RES_WIDTH_MAX(dev)              (g_au16ResWMax[dev])
#define RES_WIDTH_MIN                   (120)
#define RES_HEIGHT_MAX(dev)             (g_au16ResHMax[dev])
#define RES_HEIGHT_MIN                  (120)
#define FRAME_RATE_MAX                  (65535.0)

#define VREG_MAX_NUM                    (16)

#define AWB_ZONE_ORIG_ROW               (32)
#define AWB_ZONE_ORIG_COLUMN            (32)
#define AE_ZONE_ROW                     (15)
#define AE_ZONE_COLUMN                  (17)
#define MG_ZONE_ROW                     (15)
#define MG_ZONE_COLUMN                  (17)
#define AWB_ZONE_NUM                    (AWB_ZONE_ORIG_ROW * AWB_ZONE_ORIG_COLUMN * AWB_ZONE_BIN)
#define AWB_ZONE_STITCH_MAX             (AWB_ZONE_NUM * ISP_MAX_STITCH_NUM)
#define AF_ZONE_ROW                     (15)
#define AF_ZONE_COLUMN                  (17)
#define LIGHTSOURCE_NUM                 (4)
#define MAX_AWB_LIB_NUM                 (AWB_LIB_NUM)
#define MAX_AE_LIB_NUM                  (AE_LIB_NUM)

#define AE_MIN_WIDTH                    (256)
#define AE_MIN_HEIGHT                   (120)
#define AWB_MIN_WIDTH                   (60)
#define AWB_MIN_HEIGHT                  (14)

#define GAMMA_NODE_NUM                  (1025)
#define PREGAMMA_NODE_NUM               (257)
#define PREGAMMA_SEG_NUM                (8)

#define LCAC_STRENGTH_NUM               (16)

#define ISP_AUTO_ISO_STRENGTH_NUM       (16)
#define ISP_SHARPEN_LUMA_NUM            (32)
#define ISP_SHARPEN_GAIN_NUM            (32)

#define ISP_MAX_SNS_REGS                (32)

#define ISP_RLSC_POINTS                (129)
#define ISP_RLSC_DEFAULT_RADIAL_STR    (4096)
#define ISP_RLSC_DEFAULT_SCALE         (3)
#define ISP_RLSC_DEFAULT_MANUAL_WEIGHT (256)
#define ISP_RLSC_DEFAULT_WBGAIN        (256)
#define ISP_RLSC_DEFAULT_LIGHT         (0)
#define ISP_RLSC_WEIGHT_Q_BITS         (8)

#define ISP_VREG_SIZE_BIN               (0x20000)
#define ALG_LIB_VREG_SIZE_BIN           (0x1000)
#define WDR_MAX_FRAME_NUM               (4)

#define BAYER_CALIBTAION_MAX_NUM        (50)
#define ISP_BAYERNR_STRENGTH_DIVISOR (100)
#define ISP_BAYERNR_CORINGLOW_STRENGTH_DIVISOR (10000)
#define AI_MAX_STEP_FNO_NUM             (1024)

#define ISP_BAS_TAPS_ROW_NUM            (17)
#define ISP_BAS_TAPS_COL_6              (6)
#define ISP_BAS_TAPS_COL_4              (4)
#define ISP_BAYERNR_LUT_LENGTH       (33)
#define ISP_BAYERNR_LMTLUTNUM        (129)
#define PRO_MAX_FRAME_NUM               (8)

#define ISP_DEMOSAIC_LUT_LENGTH      (17)
#define ISP_BAYER_CHN_NUM               (4)
#define BAYER_CHN_R                     0
#define BAYER_CHN_GR                    1
#define BAYER_CHN_GB                    2
#define BAYER_CHN_B                     3
#define LDCI_LPF_LUT_SIZE               (9)
#define LDCI_HE_LUT_SIZE                (33)
#define LDCI_DE_USM_LUT_SIZE            (33)
#define LDCI_COLOR_GAIN_LUT_SIZE        (65)

#define NoiseSet_EleNum                 (7)
#define CCM_MATRIX_SIZE                 (9)
#define CCM_MATRIX_NUM                  (7)

#define ISP_DRC_CUBIC_POINT_NUM      (5)
#define ISP_DRC_CC_NODE_NUM          (33)
#define ISP_DRC_TM_NODE_NUM          (200)
#define ISP_DRC_TM_SEG_NUM           (8)
#define ISP_DRC_EXP_COMP_SAMPLE_NUM  (8)
#define ISP_DRC_SHP_LOG_CONFIG_NUM   (16)

#define LOG_LUT_SIZE                    (1025)
#define PRE_LOG_LUT_SIZE                (1025)

#define ISP_DE_LUMA_GAIN_LUT_N       (17)
#define ISP_EXP_RATIO_STRENGTH_NUM      (16)
#define ISP_LCAC_DET_NUM                (3)

#define ISP_DO_NOT_NEED_SWITCH_IMAGEMODE (-2)


#define ISP_TOP_RGGB_START_R_GR_GB_B           (0)
#define ISP_TOP_RGGB_START_GR_R_B_GB           (1)
#define ISP_TOP_RGGB_START_GB_B_R_GR           (2)
#define ISP_TOP_RGGB_START_B_GB_GR_R           (3)

#define ISP_TOP_AE_SELECT_AFTER_DG             (0)
#define ISP_TOP_AE_SELECT_AFTER_WB             (1)
#define ISP_TOP_AE_SELECT_AFTER_DRC            (2)


#define ISP_AWB_OFFSET_COMP_DEF                (0)
#define ISP_CCM_COLORTONE_EN_DEFAULT           (1)
#define ISP_CCM_COLORTONE_RGAIN_DEFAULT        (256)
#define ISP_CCM_COLORTONE_BGAIN_DEFAULT        (256)
#define ISP_CCM_COLORTONE_GGAIN_DEFAULT        (256)

#define ISP_CCM_PROT_EN_DEFAULT                (0)
#define ISP_CCM_CC_THD0_DEFAULT                (0)
#define ISP_CCM_CC_THD1_DEFAULT                (0x1E)
#define ISP_CCM_CC_PROT_RATIO_DEFAULT          (10)
#define ISP_CCM_RR_THD0_DEFAULT                (0x1E)
#define ISP_CCM_RR_THD1_DEFAULT                (0x64)
#define ISP_CCM_GG_THD0_DEFAULT                (0x6)
#define ISP_CCM_GG_THD1_DEFAULT                (0x3C)
#define ISP_CCM_BB_THD0_DEFAULT                (0x1E)
#define ISP_CCM_BB_THD1_DEFAULT                (0x64)
#define ISP_CCM_MAX_RGB_DEFAULT                (0xC8)
#define ISP_CCM_RGB_PROT_RATIO_DEFAULT         (10)
#define ISP_CCM_RECOVER_EN_DEFAULT             (1)
#define ISP_CCM_LUM_RATIO_DEFAULT              (256)
#define ISP_CCM_HUE_RATIO_DEFAULT              (256)

#define CCM_CONVERT_PRE(value)  ((value & 0x8000) | (value << 2))
#define CCM_CONVERT(value)      ((value & 0x8000)?((~(value & 0x7FFF)) + 1):(value))

typedef enum ISP_ERR_CODE_E {
    ERR_ISP_NOT_INIT                = 0x40,
    ERR_ISP_MEM_NOT_INIT            = 0x41,
    ERR_ISP_ATTR_NOT_CFG            = 0x42,
    ERR_ISP_SNS_UNREGISTER          = 0x43,
    ERR_ISP_INVALID_ADDR            = 0x44,
    ERR_ISP_NOMEM                   = 0x45,
    ERR_ISP_NO_INT                  = 0x46,
} ISP_ERR_CODE_E;

#define ERR_CODE_ISP_NULL_PTR         DEFINE_ERR_CODE(MOD_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define ERR_CODE_ISP_ILLEGAL_PARAM    DEFINE_ERR_CODE(MOD_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define ERR_CODE_ISP_NOT_SUPPORT      DEFINE_ERR_CODE(MOD_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)

#define ERR_CODE_ISP_NOT_INIT         DEFINE_ERR_CODE(MOD_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_ISP_NOT_INIT)
#define ERR_CODE_ISP_MEM_NOT_INIT     DEFINE_ERR_CODE(MOD_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_ISP_MEM_NOT_INIT)
#define ERR_CODE_ISP_ATTR_NOT_CFG     DEFINE_ERR_CODE(MOD_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_ISP_ATTR_NOT_CFG)
#define ERR_CODE_ISP_SNS_UNREGISTER   DEFINE_ERR_CODE(MOD_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_ISP_SNS_UNREGISTER)
#define ERR_CODE_ISP_INVALID_ADDR     DEFINE_ERR_CODE(MOD_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_ISP_INVALID_ADDR)
#define ERR_CODE_ISP_NOMEM            DEFINE_ERR_CODE(MOD_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_ISP_NOMEM)
#define ERR_CODE_ISP_NO_INT           DEFINE_ERR_CODE(MOD_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_ISP_NO_INT)

typedef struct ISP_MOD_PARAM_S {
    XMEDIA_U32      u32IntBotHalf;
    XMEDIA_U32      u32QuickStart;
    XMEDIA_BOOL     bLongFrmIntEn;
} ISP_MOD_PARAM_S;

typedef struct ISP_CTRL_PARAM_S {
    XMEDIA_U32      u32ProcParam;
    XMEDIA_U32      u32StatIntvl;
    XMEDIA_U32      u32UpdatePos;
    XMEDIA_U32      u32IntTimeOut;
    XMEDIA_U32      u32PwmNumber;
    XMEDIA_U32      u32PortIntDelay;
    XMEDIA_BOOL     bLdciTprFltEn;
} ISP_CTRL_PARAM_S;

typedef enum ISP_OP_TYPE_E {
    OP_TYPE_AUTO    = 0,
    OP_TYPE_MANUAL  = 1,
    OP_TYPE_BUTT
} ISP_OP_TYPE_E;


typedef enum ISP_PRIOR_FRAME_E {
    LONG_FRAME      = 0,
    SHORT_FRAME       = 1,
    PRIOR_FRAME_BUTT
} ISP_PRIOR_FRAME_E;

typedef enum ISP_STATE_E {
    ISP_STATE_INIT     = 0,
    ISP_STATE_SUCCESS  = 1,
    ISP_STATE_TIMEOUT  = 2,
    ISP_STATE_BUTT
} ISP_STATUS_E;

typedef struct ISP_PIPE_DIFF_ATTR_S {
    XMEDIA_S32 as32Offset[ISP_BAYER_CHN_NUM];
    XMEDIA_U32 au32Gain[ISP_BAYER_CHN_NUM];
    XMEDIA_U16 au16ColorMatrix[CCM_MATRIX_SIZE];
} ISP_PIPE_DIFF_ATTR_S;

typedef enum ISP_BAYER_FORMAT_E {
    BAYER_RGGB    = 0,
    BAYER_GRBG    = 1,
    BAYER_GBRG    = 2,
    BAYER_BGGR    = 3,
    BAYER_BUTT
} ISP_BAYER_FORMAT_E;

typedef enum ISP_BAYER_RAWBIT_E {
    BAYER_RAWBIT_8BIT    = 8,
    BAYER_RAWBIT_10BIT   = 10,
    BAYER_RAWBIT_12BIT   = 12,
    BAYER_RAWBIT_14BIT   = 14,
    BAYER_RAWBIT_16BIT   = 16,
    BAYER_RAWBIT_BUTT

} ISP_BAYER_RAWBIT_E;



typedef struct ISP_PUB_ATTR_S {
    RECT_S          stWndRect;
    SIZE_S          stSnsSize;
    XMEDIA_FLOAT        f32FrameRate;
    ISP_BAYER_FORMAT_E  enBayer;
    WDR_MODE_E      enWDRMode;
    XMEDIA_U8           u8SnsMode;
} ISP_PUB_ATTR_S;

typedef struct ISP_SLAVE_SNS_SYNC_S {
    union {
        struct {
            XMEDIA_U32  bit16Rsv          :  16;
            XMEDIA_U32  bitHInv           :  1;
            XMEDIA_U32  bitVInv           :  1;
            XMEDIA_U32  bit12Rsv          :  12;
            XMEDIA_U32  bitHEnable        :  1;
            XMEDIA_U32  bitVEnable        :  1;
        } stBits;
        XMEDIA_U32 u32Bytes;
    } unCfg;

    XMEDIA_U32  u32VsTime;
    XMEDIA_U32  u32HsTime;
    XMEDIA_U32  u32VsCyc;
    XMEDIA_U32  u32HsCyc;
    XMEDIA_U32  u32SlaveModeTime;

} ISP_SLAVE_SNS_SYNC_S;

typedef enum ISP_FMW_STATE_E {
    ISP_FMW_STATE_RUN = 0,
    ISP_FMW_STATE_FREEZE,
    ISP_FMW_STATE_BUTT
} ISP_FMW_STATE_E;

typedef struct ISP_WDR_MODE_S {
    WDR_MODE_E  enWDRMode;
} ISP_WDR_MODE_S;

typedef union ISP_MODULE_CTRL_U {
    XMEDIA_U64  u64Key;
    struct {
        XMEDIA_U64  bitBypassISPDGain      : 1 ;
        XMEDIA_U64  bitBypassAntiFC        : 1 ;
        XMEDIA_U64  bitBypassCrosstalkR    : 1 ;
        XMEDIA_U64  bitBypassDPC           : 1 ;
        XMEDIA_U64  bitBypassNR            : 1 ;
        XMEDIA_U64  bitBypassDehaze        : 1 ;
        XMEDIA_U64  bitBypassWBGain        : 1 ;
        XMEDIA_U64  bitBypassMeshShading   : 1 ;
        XMEDIA_U64  bitBypassDRC           : 1 ;
        XMEDIA_U64  bitBypassDemosaic      : 1 ;
        XMEDIA_U64  bitBypassColorMatrix   : 1 ;
        XMEDIA_U64  bitBypassGamma         : 1 ;
        XMEDIA_U64  bitBypassFSWDR         : 1 ;
        XMEDIA_U64  bitBypassCA            : 1 ;
        XMEDIA_U64  bitBypassCsConv        : 1 ;
        XMEDIA_U64  bitBypassRadialCrop    : 1 ;
        XMEDIA_U64  bitBypassSharpen       : 1 ;
        XMEDIA_U64  bitBypassLCAC          : 1 ;
        XMEDIA_U64  bitBypassGCAC          : 1 ;
        XMEDIA_U64  bit2ChnSelect          : 2 ;
        XMEDIA_U64  bitBypassLdci          : 1 ;
        XMEDIA_U64  bitBypassPreGamma      : 1 ;
        XMEDIA_U64  bitBypassRadialShading : 1 ;
        XMEDIA_U64  bitBypassAEStatFE      : 1 ;
        XMEDIA_U64  bitBypassAEStatBE      : 1 ;
        XMEDIA_U64  bitBypassMGStat        : 1 ;
        XMEDIA_U64  bitBypassDE            : 1 ;
        XMEDIA_U64  bitBypassAFStatFE      : 1 ;
        XMEDIA_U64  bitBypassAFStatBE      : 1 ;
        XMEDIA_U64  bitBypassAWBStat       : 1 ;
        XMEDIA_U64  bitBypassCLUT          : 1 ;
        XMEDIA_U64  bitBypassHLC           : 1 ;
        XMEDIA_U64  bitBypassEdgeMark      : 1 ;
        XMEDIA_U64  bitBypassRGBIR         : 1 ;
        XMEDIA_U64  bitRsv29               : 29 ;
    };
} ISP_MODULE_CTRL_U;
typedef enum ISP_COMBINE_MODE_E {
    FS_WDR_COMBINE_SHORT_FIRST  = 0,
    FS_WDR_COMBINE_LONG_FIRST   = 1,
    FS_WDR_COMBINE_BUTT
} ISP_COMBINE_MODE_E;

typedef enum ISP_WDR_MERGE_MODE_E {
    MERGE_WDR_MODE      = 0,
    MERGE_FUSION_MODE   = 1,
    MERGE_BUTT
} ISP_WDR_MERGE_MODE_E;

typedef enum ISP_BNR_MODE_E {
    BNR_OFF_MODE         = 0,
    BNR_ON_MODE          = 1,
    BNR_BUTT
} ISP_BNR_MODE_E;

typedef enum ISP_WDR_WBGAIN_POSITION_E {
    WDR_WBGAIN_IN_DG1         = 0,
    WDR_WBGAIN_IN_WB          = 1,
    WDR_WBGAIN_POSITION_BUTT
} ISP_WDR_WBGAIN_POSITION_E;

typedef struct ISP_FSWDR_MANUAL_ATTR_S {
    XMEDIA_U8 u8MdThrLowGain;
    XMEDIA_U8 u8MdThrHigGain;
} ISP_FSWDR_MANUAL_ATTR_S;

typedef struct ISP_FSWDR_AUTO_ATTR_S {
    XMEDIA_U8 au8MdThrLowGain[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8 au8MdThrHigGain[ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_FSWDR_AUTO_ATTR_S;

typedef struct ISP_MDT_ATTR_S {
    XMEDIA_BOOL bShortExpoChk;
    XMEDIA_U16  u16ShortCheckThd;
    XMEDIA_BOOL bMDRefFlicker;
    XMEDIA_U8   u8MdtStillThd;
    XMEDIA_U8   u8MdtFullThd;
    XMEDIA_U8   u8MdtLongBlend;
    ISP_OP_TYPE_E enOpType;
    ISP_FSWDR_MANUAL_ATTR_S stManual;
    ISP_FSWDR_AUTO_ATTR_S   stAuto;
} ISP_FSWDR_MDT_ATTR_S;


typedef struct ISP_WDR_COMBINE_ATTR_S {
    XMEDIA_BOOL bMotionComp;
    XMEDIA_U16  u16ShortThr;
    XMEDIA_U16  u16LongThr;
    XMEDIA_BOOL bForceLong;
    XMEDIA_U16  u16ForceLongLowThr;
    XMEDIA_U16  u16ForceLongHigThr;
    ISP_FSWDR_MDT_ATTR_S stWDRMdt;
} ISP_WDR_COMBINE_ATTR_S;

typedef struct ISP_FUSION_ATTR_S {
    XMEDIA_U16  au16FusionThr[4];
} ISP_FUSION_ATTR_S;

typedef struct ISP_WDR_BNR_ATTR_S {
    ISP_BNR_MODE_E enBnrMode;
    XMEDIA_BOOL        bShortFrameNR;
    XMEDIA_U8          u8ShortFrameNRStr;
    XMEDIA_U8          u8FusionBnrStr;
    XMEDIA_U8          au8NoiseFloor[NoiseSet_EleNum];
    XMEDIA_U8          u8FullMdtSigGWgt;
    XMEDIA_U8          u8FullMdtSigRBWgt;
    XMEDIA_U8          u8FullMdtSigWgt;
    XMEDIA_U8          au8GsigmaGain[3];
    XMEDIA_U8          au8RBsigmaGain[3];
} ISP_WDR_BNR_ATTR_S;

typedef struct ISP_WDR_FS_ATTR_S {
    ISP_WDR_MERGE_MODE_E enWDRMergeMode;
    ISP_WDR_COMBINE_ATTR_S stWDRCombine;
    ISP_WDR_BNR_ATTR_S stBnr;
    ISP_FUSION_ATTR_S stFusion;
    ISP_WDR_WBGAIN_POSITION_E enWDRWbgainPosition;
} ISP_WDR_FS_ATTR_S;

typedef struct ISP_DRC_CUBIC_POINT_ATTR_S {
    XMEDIA_U16 u16X;
    XMEDIA_U16 u16Y;
    XMEDIA_U16 u16Slope;
} ISP_DRC_CUBIC_POINT_ATTR_S;

typedef struct ISP_DRC_ASYMMETRY_CURVE_ATTR_S {
    XMEDIA_U8   u8Asymmetry;
    XMEDIA_U8   u8SecondPole;
    XMEDIA_U8   u8Stretch;
    XMEDIA_U8   u8Compress;
} ISP_DRC_ASYMMETRY_CURVE_ATTR_S;

typedef struct ISP_DRC_MANUAL_ATTR_S {
    XMEDIA_U16  u16Strength;
} ISP_DRC_MANUAL_ATTR_S;

typedef struct ISP_DRC_AUTO_ATTR_S {
    XMEDIA_U16  u16Strength;

    XMEDIA_U16 u16StrengthMax;
    XMEDIA_U16 u16StrengthMin;

} ISP_DRC_AUTO_ATTR_S;


typedef enum ISP_DRC_CURVE_SELECT_E {
    DRC_CURVE_ASYMMETRY = 0x0,
    DRC_CURVE_CUBIC,
    DRC_CURVE_USER,
    DRC_CURVE_BUTT
} ISP_DRC_CURVE_SELECT_E;

typedef struct ISP_DRC_ATTR_S {
    XMEDIA_BOOL bEnable;
    ISP_DRC_CURVE_SELECT_E enCurveSelect;

    XMEDIA_U8  u8PDStrength;
    XMEDIA_U8  u8LocalMixingBrightMax;
    XMEDIA_U8  u8LocalMixingBrightMin;
    XMEDIA_U8  u8LocalMixingBrightThr;
    XMEDIA_S8  s8LocalMixingBrightSlo;
    XMEDIA_U8  u8LocalMixingDarkMax;
    XMEDIA_U8  u8LocalMixingDarkMin;
    XMEDIA_U8  u8LocalMixingDarkThr;
    XMEDIA_S8  s8LocalMixingDarkSlo;

    XMEDIA_U8  u8DetailBrightStr;
    XMEDIA_U8  u8DetailDarkStr;
    XMEDIA_U8  u8DetailBrightStep;
    XMEDIA_U8  u8DetailDarkStep;

    XMEDIA_U8  u8BrightGainLmt;
    XMEDIA_U8  u8BrightGainLmtStep;
    XMEDIA_U8  u8DarkGainLmtY;
    XMEDIA_U8  u8DarkGainLmtC;
    XMEDIA_U16 au16ColorCorrectionLut[ISP_DRC_CC_NODE_NUM];
    XMEDIA_U16 au16ToneMappingValue[ISP_DRC_TM_NODE_NUM];

    XMEDIA_U8  u8FltScaleCoarse;
    XMEDIA_U8  u8FltScaleFine;
    XMEDIA_U8  u8ContrastControl;
    XMEDIA_S8  s8DetailAdjustFactor;

    XMEDIA_U8  u8SpatialFltCoef;
    XMEDIA_U8  u8RangeFltCoef;
    XMEDIA_U8  u8RangeAdaMax;

    XMEDIA_U8  u8GradRevMax;
    XMEDIA_U8  u8GradRevThr;

    XMEDIA_U8  u8DpDetectRangeRatio;
    XMEDIA_U8  u8DpDetectThrSlo;
    XMEDIA_U16 u16DpDetectThrMin;

    ISP_OP_TYPE_E enOpType;
    ISP_DRC_MANUAL_ATTR_S stManual;
    ISP_DRC_AUTO_ATTR_S   stAuto;
    ISP_DRC_CUBIC_POINT_ATTR_S astCubicPoint[ISP_DRC_CUBIC_POINT_NUM];
    ISP_DRC_ASYMMETRY_CURVE_ATTR_S stAsymmetryCurve;

} ISP_DRC_ATTR_S;

typedef struct ISP_LDCI_GAUSS_COEF_ATTR_S {
    XMEDIA_U8   u8Wgt;
    XMEDIA_U8   u8Sigma;
    XMEDIA_U8   u8Mean;
} ISP_LDCI_GAUSS_COEF_ATTR_S;

typedef struct ISP_LDCI_HE_WGT_ATTR_S {
    ISP_LDCI_GAUSS_COEF_ATTR_S  stHePosWgt;
    ISP_LDCI_GAUSS_COEF_ATTR_S  stHeNegWgt;
} ISP_LDCI_HE_WGT_ATTR_S;

typedef struct ISP_LDCI_MANUAL_ATTR_S {
    ISP_LDCI_HE_WGT_ATTR_S  stHeWgt;
    XMEDIA_U16                  u16BlcCtrl;
} ISP_LDCI_MANUAL_ATTR_S;

typedef struct ISP_LDCI_AUTO_ATTR_S {
    ISP_LDCI_HE_WGT_ATTR_S  astHeWgt[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U16                  au16BlcCtrl[ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_LDCI_AUTO_ATTR_S;

typedef struct ISP_LDCI_ATTR_S {
    XMEDIA_BOOL                 bEnable;
    XMEDIA_U8                   u8GaussLPFSigma;
    ISP_OP_TYPE_E           enOpType;
    ISP_LDCI_MANUAL_ATTR_S  stManual;
    ISP_LDCI_AUTO_ATTR_S    stAuto;
    XMEDIA_U16                  u16TprIncrCoef;
    XMEDIA_U16                  u16TprDecrCoef;
} ISP_LDCI_ATTR_S;

typedef enum ISP_CA_TYPE_E {
    ISP_CA_ENABLE = 0x0,
    ISP_CP_ENABLE,
    ISP_CA_BUTT
} ISP_CA_TYPE_E;

typedef struct ISP_CA_LUT_S {
    XMEDIA_U32  au32YRatioLut[ISP_CA_YRATIO_LUT_LENGTH];
    XMEDIA_S32  as32ISORatio[ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_CA_LUT_S;

typedef struct ISP_CP_LUT_S {
    XMEDIA_U8   au8CPLutY[ISP_CA_YRATIO_LUT_LENGTH];
    XMEDIA_U8   au8CPLutU[ISP_CA_YRATIO_LUT_LENGTH];
    XMEDIA_U8   au8CPLutV[ISP_CA_YRATIO_LUT_LENGTH];
} ISP_CP_LUT_S;

typedef struct ISP_CA_ATTR_S {
    XMEDIA_BOOL bEnable;
    ISP_CA_TYPE_E eCaCpEn;
    ISP_CA_LUT_S stCA;
    ISP_CP_LUT_S stCP;
} ISP_CA_ATTR_S;


typedef struct CSC_MATRX_S {
    XMEDIA_S16 as16CSCIdc[3];
    XMEDIA_S16 as16CSCOdc[3];
    XMEDIA_S16 as16CSCCoef[9];
} CSC_MATRX_S;

typedef struct ISP_CSC_ATTR_S {
    XMEDIA_BOOL bEnable;

    COLOR_GAMUT_E enColorGamut;
    XMEDIA_U8   u8Hue;
    XMEDIA_U8   u8Luma;
    XMEDIA_U8   u8Contr;
    XMEDIA_U8   u8Satu;
    XMEDIA_BOOL bLimitedRangeEn;
    XMEDIA_BOOL bExtCscEn;
    XMEDIA_BOOL bCtModeEn;
    CSC_MATRX_S stCscMagtrx;

} ISP_CSC_ATTR_S;


typedef struct ISP_CLUT_ATTR_S {
    XMEDIA_BOOL         bEnable         ;
    XMEDIA_U32          u32GainR        ;
    XMEDIA_U32          u32GainG        ;
    XMEDIA_U32          u32GainB        ;
} ISP_CLUT_ATTR_S;


typedef struct ISP_CLUT_LUT_S {
    XMEDIA_U32 au32lut[ISP_CLUT_LUT_LENGTH];
} ISP_CLUT_LUT_S;



typedef enum ISP_STATIC_DP_TYPE_E {
    ISP_STATIC_DP_BRIGHT = 0x0,
    ISP_STATIC_DP_DARK,
    ISP_STATIC_DP_BUTT
} ISP_STATIC_DP_TYPE_E;


typedef struct ISP_DP_STATIC_CALIBRATE_S {
    XMEDIA_BOOL bEnableDetect;
    ISP_STATIC_DP_TYPE_E enStaticDPType;
    XMEDIA_U8   u8StartThresh;
    XMEDIA_U16  u16CountMax;
    XMEDIA_U16  u16CountMin;
    XMEDIA_U16  u16TimeLimit;

    XMEDIA_U32      au32Table[STATIC_DP_COUNT_MAX];
    XMEDIA_U8       u8FinishThresh;
    XMEDIA_U16      u16Count;
    ISP_STATUS_E enStatus;
} ISP_DP_STATIC_CALIBRATE_S;

typedef struct ISP_DP_STATIC_ATTR_S {
    XMEDIA_BOOL bEnable;
    XMEDIA_U16  u16BrightCount;
    XMEDIA_U16  u16DarkCount;
    XMEDIA_U32  au32BrightTable[STATIC_DP_COUNT_MAX];

    XMEDIA_U32  au32DarkTable[STATIC_DP_COUNT_MAX];
    XMEDIA_BOOL bShow;
} ISP_DP_STATIC_ATTR_S;

typedef struct ISP_DP_DYNAMIC_MANUAL_ATTR_S {
    XMEDIA_U16  u16Strength;
    XMEDIA_U16  u16BlendRatio;
} ISP_DP_DYNAMIC_MANUAL_ATTR_S;

typedef struct ISP_DP_DYNAMIC_AUTO_ATTR_S {
    XMEDIA_U16  au16Strength[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U16  au16BlendRatio[ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_DP_DYNAMIC_AUTO_ATTR_S;

typedef struct ISP_DP_DYNAMIC_ATTR_S {
    XMEDIA_BOOL bEnable;
    XMEDIA_BOOL bSupTwinkleEn;
    XMEDIA_S8   s8SoftThr;
    XMEDIA_U8   u8SoftSlope;
    ISP_OP_TYPE_E enOpType;
    ISP_DP_DYNAMIC_MANUAL_ATTR_S stManual;
    ISP_DP_DYNAMIC_AUTO_ATTR_S   stAuto;
} ISP_DP_DYNAMIC_ATTR_S;

typedef struct ISP_DIS_ATTR_S {
    XMEDIA_BOOL bEnable;
} ISP_DIS_ATTR_S;


typedef struct ISP_SHADING_ATTR_S {
    XMEDIA_BOOL bEnable;
    XMEDIA_U16  u16MeshStr;
    XMEDIA_U16  u16BlendRatio;
} ISP_SHADING_ATTR_S;

typedef struct ISP_SHADING_LUT_S {
    XMEDIA_U16  au16RGain[ISP_LSC_GRID_POINTS];
    XMEDIA_U16  au16GrGain[ISP_LSC_GRID_POINTS];
    XMEDIA_U16  au16GbGain[ISP_LSC_GRID_POINTS];
    XMEDIA_U16  au16BGain[ISP_LSC_GRID_POINTS];
} ISP_SHADING_GAIN_LUT_S;

typedef struct ISP_BNR_LSC_GAIN_LUT_S {
    XMEDIA_U16  au16RGain[ISP_RLSC_POINTS];
    XMEDIA_U16  au16GrGain[ISP_RLSC_POINTS];
    XMEDIA_U16  au16GbGain[ISP_RLSC_POINTS];
    XMEDIA_U16  au16BGain[ISP_RLSC_POINTS];
} ISP_BNR_LSC_GAIN_LUT_S;

typedef struct ISP_SHADING_LUT_ATTR_S {
    XMEDIA_U8   u8MeshScale;
    XMEDIA_U16  au16XGridWidth[(ISP_LSC_GRID_COL - 1) / 2];
    XMEDIA_U16  au16YGridWidth[(ISP_LSC_GRID_ROW - 1) / 2];
    ISP_SHADING_GAIN_LUT_S astLscGainLut[2];
    ISP_BNR_LSC_GAIN_LUT_S stBNRLscGainLut;
} ISP_SHADING_LUT_ATTR_S;

typedef struct ISP_MLSC_CALIBRATION_CFG_S {
    ISP_BAYER_FORMAT_E enBayer;
    ISP_BAYER_RAWBIT_E enRawBit;

    XMEDIA_U16  u16ImgHeight;
    XMEDIA_U16  u16ImgWidth;

    XMEDIA_U16  u16DstImgHeight;
    XMEDIA_U16  u16DstImgWidth;
    XMEDIA_U16  u16OffsetX;
    XMEDIA_U16  u16OffsetY;

    XMEDIA_U32  u32MeshScale;

    XMEDIA_U16  u16BLCOffsetR;
    XMEDIA_U16  u16BLCOffsetGr;
    XMEDIA_U16  u16BLCOffsetGb;
    XMEDIA_U16  u16BLCOffsetB;
} ISP_MLSC_CALIBRATION_CFG_S;

typedef struct ISP_MESH_SHADING_TABLE_S {
    XMEDIA_U8   u8MeshScale;
    XMEDIA_U16  au16XGridWidth[(ISP_LSC_GRID_COL - 1) / 2];
    XMEDIA_U16  au16YGridWidth[(ISP_LSC_GRID_ROW - 1) / 2];
    ISP_SHADING_GAIN_LUT_S stLscGainLut;
    ISP_BNR_LSC_GAIN_LUT_S stBNRLscGainLut;
} ISP_MESH_SHADING_TABLE_S;

typedef struct ISP_RADIAL_SHADING_ATTR_S {
    XMEDIA_BOOL bEnable;
    XMEDIA_U16  u16RadialStr;
} ISP_RADIAL_SHADING_ATTR_S;

typedef struct ISP_RADIAL_SHADING_LUT_S {
    XMEDIA_U16  au16RGain[ISP_RLSC_POINTS];
    XMEDIA_U16  au16GrGain[ISP_RLSC_POINTS];
    XMEDIA_U16  au16GbGain[ISP_RLSC_POINTS];
    XMEDIA_U16  au16BGain[ISP_RLSC_POINTS];
} ISP_RADIAL_SHADING_GAIN_LUT_S;

typedef struct ISP_RADIAL_SHADING_LUT_ATTR_S {
    OPERATION_MODE_E enLightMode;
    XMEDIA_U16  u16BlendRatio;
    XMEDIA_U8   u8LightType1;
    XMEDIA_U8   u8LightType2;
    XMEDIA_U8   u8RadialScale;
    XMEDIA_U16  u16CenterRX;
    XMEDIA_U16  u16CenterRY;
    XMEDIA_U16  u16CenterGrX;
    XMEDIA_U16  u16CenterGrY;
    XMEDIA_U16  u16CenterGbX;
    XMEDIA_U16  u16CenterGbY;
    XMEDIA_U16  u16CenterBX;
    XMEDIA_U16  u16CenterBY;
    XMEDIA_U16  u16OffCenterR;
    XMEDIA_U16  u16OffCenterGr;
    XMEDIA_U16  u16OffCenterGb;
    XMEDIA_U16  u16OffCenterB;
    ISP_RADIAL_SHADING_GAIN_LUT_S astRLscGainLut[3];
} ISP_RADIAL_SHADING_LUT_ATTR_S;

typedef struct ISP_NR_MANUAL_ATTR_S {
    XMEDIA_U8   au8ChromaStr[ISP_BAYER_CHN_NUM];
    XMEDIA_U8   u8FineStr;
    XMEDIA_U16  u16CoringWgt;
    XMEDIA_U16  au16CoarseStr[ISP_BAYER_CHN_NUM];
} ISP_NR_MANUAL_ATTR_S;

typedef struct ISP_NR_AUTO_ATTR_S {
    XMEDIA_U8   au8ChromaStr[ISP_BAYER_CHN_NUM][ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8   au8FineStr[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U16  au16CoringWgt[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U16  au16CoarseStr[ISP_BAYER_CHN_NUM][ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_NR_AUTO_ATTR_S;

typedef struct ISP_NR_WDR_ATTR_S {
    XMEDIA_U8    au8WDRFrameStr[WDR_MAX_FRAME_NUM];
    XMEDIA_U8    au8FusionFrameStr[WDR_MAX_FRAME_NUM];
} ISP_NR_WDR_ATTR_S;

typedef struct ISP_NR_ATTR_S {
    XMEDIA_BOOL  bEnable;
    XMEDIA_BOOL  bLowPowerEnable;
    XMEDIA_BOOL  bNrLscEnable;
    XMEDIA_U8    u8NrLscRatio;
    XMEDIA_U8    u8BnrLscMaxGain;
    XMEDIA_U16   u16BnrLscCmpStrength;
    XMEDIA_U16   au16CoringRatio[ISP_BAYERNR_LUT_LENGTH];

    ISP_OP_TYPE_E enOpType;
    ISP_NR_AUTO_ATTR_S stAuto;
    ISP_NR_MANUAL_ATTR_S stManual;
    ISP_NR_WDR_ATTR_S  stWdr;
} ISP_NR_ATTR_S;

typedef struct ISP_DE_MANUAL_ATTR_S {
    XMEDIA_U16       u16GlobalGain;
    XMEDIA_U16       u16GainLF;
    XMEDIA_U16       u16GainHF;
} ISP_DE_MANUAL_ATTR_S;

typedef struct ISP_DE_AUTO_ATTR_S {
    XMEDIA_U16       au16GlobalGain[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U16       au16GainLF[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U16       au16GainHF[ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_DE_AUTO_ATTR_S;


typedef struct ISP_DE_ATTR_S {
    XMEDIA_BOOL       bEnable;
    XMEDIA_U16        au16LumaGainLut[ISP_DE_LUMA_GAIN_LUT_N];

    ISP_OP_TYPE_E        enOpType;
    ISP_DE_AUTO_ATTR_S   stAuto;
    ISP_DE_MANUAL_ATTR_S stManual;
} ISP_DE_ATTR_S;

#define ISP_CVTMAT_NUM                            12
#define ISP_EXP_CTRL_NUM                          2

typedef enum ISP_IRBAYER_FORMAT_E {
    IRBAYER_GRGBI = 0,
    IRBAYER_RGBGI = 1,
    IRBAYER_GBGRI = 2,
    IRBAYER_BGRGI = 3,
    IRBAYER_IGRGB = 4,
    IRBAYER_IRGBG = 5,
    IRBAYER_IBGRG = 6,
    IRBAYER_IGBGR = 7,
    IRBAYER_BUTT

} ISP_IRBAYER_FORMAT_E;

typedef enum ISP_IR_CVTMAT_MODE_E {
    ISP_IR_CVTMAT_MODE_NORMAL = 0,
    ISP_IR_CVTMAT_MODE_MONO,
    ISP_IR_CVTMAT_MODE_USER,
    ISP_IR_CVTMAT_MODE_BUTT

} ISP_IR_CVTMAT_MODE_E;

typedef struct ISP_RGBIR_ATTR_S {
    XMEDIA_BOOL              bEnable    ;
    ISP_IRBAYER_FORMAT_E enInPattern;
    ISP_BAYER_FORMAT_E   enOutPattern;

    XMEDIA_U16               au16ExpCtrl[ISP_EXP_CTRL_NUM];
    XMEDIA_U16               au16Gain[ISP_EXP_CTRL_NUM];

    ISP_IR_CVTMAT_MODE_E enIRCvtMatMode;
    XMEDIA_S16               as16CvtMatrix[ISP_CVTMAT_NUM];
} ISP_RGBIR_ATTR_S;

typedef enum ISP_GAMMA_CURVE_TYPE_E {
    ISP_GAMMA_CURVE_DEFAULT = 0x0,
    ISP_GAMMA_CURVE_SRGB,
    ISP_GAMMA_CURVE_HDR,
    ISP_GAMMA_CURVE_USER_DEFINE,
    ISP_GAMMA_CURVE_BUTT
} ISP_GAMMA_CURVE_TYPE_E;
typedef struct ISP_GAMMA_ATTR_S {
    XMEDIA_BOOL   bEnable;
    XMEDIA_U16    u16Table[GAMMA_NODE_NUM];

    ISP_GAMMA_CURVE_TYPE_E enCurveType;
} ISP_GAMMA_ATTR_S;

typedef struct ISP_PREGAMMA_ATTR_S {
    XMEDIA_BOOL   bEnable;
    XMEDIA_U32    au32Table[PREGAMMA_NODE_NUM];
} ISP_PREGAMMA_ATTR_S;

typedef struct ISP_PRELOGLUT_ATTR_S {
    XMEDIA_BOOL                 bEnable;
} ISP_PRELOGLUT_ATTR_S;

typedef struct ISP_LOGLUT_ATTR_S {
    XMEDIA_BOOL                 bEnable;
} ISP_LOGLUT_ATTR_S;

#define     ISP_SHARPEN_FREQ_CORING_LENGTH      (8)
#define     SHRP_GAIN_LUT_SIZE                  (64)

typedef struct ISP_SHARPEN_MANUAL_ATTR_S {
    XMEDIA_U8  au8LumaWgt[ISP_SHARPEN_LUMA_NUM];
    XMEDIA_U16 au16TextureStr[ISP_SHARPEN_GAIN_NUM];
    XMEDIA_U16 au16EdgeStr[ISP_SHARPEN_GAIN_NUM];
    XMEDIA_U16 u16TextureFreq;
    XMEDIA_U16 u16EdgeFreq;
    XMEDIA_U8  u8OverShoot;
    XMEDIA_U8  u8UnderShoot;
    XMEDIA_U8  u8ShootSupStr;
    XMEDIA_U8  u8ShootSupAdj;
    XMEDIA_U8  u8DetailCtrl;
    XMEDIA_U8  u8DetailCtrlThr;
    XMEDIA_U8  u8EdgeFiltStr;
    XMEDIA_U8  u8EdgeFiltMaxCap;
    XMEDIA_U8  u8RGain;
    XMEDIA_U8  u8GGain;
    XMEDIA_U8  u8BGain;
    XMEDIA_U8  u8SkinGain;
    XMEDIA_U16  u16MaxSharpGain;
    XMEDIA_U8  u8WeakDetailGain;
} ISP_SHARPEN_MANUAL_ATTR_S;


typedef struct ISP_SHARPEN_AUTO_ATTR_S {
    XMEDIA_U8  au8LumaWgt[ISP_SHARPEN_LUMA_NUM][ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U16 au16TextureStr[ISP_SHARPEN_GAIN_NUM][ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U16 au16EdgeStr[ISP_SHARPEN_GAIN_NUM][ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U16 au16TextureFreq[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U16 au16EdgeFreq[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8  au8OverShoot[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8  au8UnderShoot[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8  au8ShootSupStr[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8  au8ShootSupAdj[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8  au8DetailCtrl[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8  au8DetailCtrlThr[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8  au8EdgeFiltStr[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8  au8EdgeFiltMaxCap[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8  au8RGain[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8  au8GGain[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8  au8BGain[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8  au8SkinGain[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U16  au16MaxSharpGain[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8  au8WeakDetailGain[ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_SHARPEN_AUTO_ATTR_S;

typedef struct ISP_SHARPEN_ATTR_S {
    XMEDIA_BOOL bEnable;
    XMEDIA_U8 u8SkinUmin;
    XMEDIA_U8 u8SkinVmin;
    XMEDIA_U8 u8SkinUmax;
    XMEDIA_U8 u8SkinVmax;
    ISP_OP_TYPE_E enOpType;
    ISP_SHARPEN_MANUAL_ATTR_S stManual;
    ISP_SHARPEN_AUTO_ATTR_S   stAuto;
} ISP_SHARPEN_ATTR_S;

typedef struct ISP_EDGEMARK_ATTR_S {
    XMEDIA_BOOL bEnable;
    XMEDIA_U8   u8Threshold;
    XMEDIA_U32  u32Color;
} ISP_EDGEMARK_ATTR_S;

typedef struct ISP_HLC_ATTR_S {
    XMEDIA_BOOL bEnable;
    XMEDIA_U8   u8LumaThr;
    XMEDIA_U8   u8LumaTarget;
} ISP_HLC_ATTR_S;

typedef struct ISP_CR_ATTR_S {
    XMEDIA_BOOL  bEnable;
    XMEDIA_U8    u8Slope;
    XMEDIA_U8    u8SensiSlope;
    XMEDIA_U16   u16SensiThr;
    XMEDIA_U16   au16Strength[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U16   au16Threshold[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U16   au16NpOffset[ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_CR_ATTR_S;

typedef struct ISP_ANTIFALSECOLOR_MANUAL_ATTR_S {
    XMEDIA_U8    u8AntiFalseColorThreshold;
    XMEDIA_U8    u8AntiFalseColorStrength;
} ISP_ANTIFALSECOLOR_MANUAL_ATTR_S;

typedef struct ISP_ANTIFALSECOLOR_AUTO_ATTR_S {
    XMEDIA_U8   au8AntiFalseColorThreshold[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8   au8AntiFalseColorStrength[ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_ANTIFALSECOLOR_AUTO_ATTR_S;

typedef struct ISP_ANTIFALSECOLOR_ATTR_S {
    XMEDIA_BOOL  bEnable;
    ISP_OP_TYPE_E enOpType;
    ISP_ANTIFALSECOLOR_AUTO_ATTR_S stAuto;
    ISP_ANTIFALSECOLOR_MANUAL_ATTR_S stManual;
} ISP_ANTIFALSECOLOR_ATTR_S;

typedef struct ISP_DEMOSAIC_MANUAL_ATTR_S {
    XMEDIA_U8   u8NonDirStr;
    XMEDIA_U8   u8NonDirMFDetailEhcStr;
    XMEDIA_U8   u8NonDirHFDetailEhcStr;
    XMEDIA_U8   u8DetailSmoothRange;
    XMEDIA_U16  u16DetailSmoothStr;
	XMEDIA_U8   u8ColorNoiseThdF;
    XMEDIA_U8   u8ColorNoiseStrF;
    XMEDIA_U8   u8ColorNoiseThdY;
    XMEDIA_U8   u8ColorNoiseStrY;
} ISP_DEMOSAIC_MANUAL_ATTR_S;

typedef struct ISP_DEMOSAIC_AUTO_ATTR_S {
    XMEDIA_U8   au8NonDirStr[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8   au8NonDirMFDetailEhcStr[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8   au8NonDirHFDetailEhcStr[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8   au8DetailSmoothRange[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U16  au16DetailSmoothStr[ISP_AUTO_ISO_STRENGTH_NUM];
	XMEDIA_U8   au8ColorNoiseThdF[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8   au8ColorNoiseStrF[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8   au8ColorNoiseThdY[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8   au8ColorNoiseStrY[ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_DEMOSAIC_AUTO_ATTR_S;

typedef struct ISP_DEMOSAIC_ATTR_S {
    XMEDIA_BOOL bEnable;
    ISP_OP_TYPE_E enOpType;
    ISP_DEMOSAIC_MANUAL_ATTR_S stManual;
    ISP_DEMOSAIC_AUTO_ATTR_S stAuto;
} ISP_DEMOSAIC_ATTR_S;

typedef struct ISP_BLACK_LEVEL_S {
    ISP_OP_TYPE_E enOpType;
    XMEDIA_U16 au16BlackLevel[ISP_BAYER_CHN_NUM];
} ISP_BLACK_LEVEL_S;

typedef enum ISP_FPN_TYPE_E {
    ISP_FPN_TYPE_FRAME = 0,
    ISP_FPN_TYPE_LINE = 1,
    ISP_FPN_TYPE_BUTT
} ISP_FPN_TYPE_E;

typedef struct ISP_FPN_FRAME_INFO_S {
    XMEDIA_U32              u32Iso;
    XMEDIA_U32              u32Offset[VI_MAX_SPLIT_NODE_NUM];
    XMEDIA_U32              u32FrmSize;
    VIDEO_FRAME_INFO_S  stFpnFrame;
} ISP_FPN_FRAME_INFO_S;

typedef struct ISP_FPN_CALIBRATE_ATTR_S {
    XMEDIA_U32                          u32Threshold;
    XMEDIA_U32                          u32FrameNum;
    ISP_FPN_TYPE_E                  enFpnType;
    ISP_FPN_FRAME_INFO_S            stFpnCaliFrame;
} ISP_FPN_CALIBRATE_ATTR_S;
typedef struct ISP_FPN_MANUAL_ATTR_S {
    XMEDIA_U32               u32Strength;
} ISP_FPN_MANUAL_ATTR_S;

typedef struct ISP_FPN_AUTO_ATTR_S {
    XMEDIA_U32               u32Strength;
} ISP_FPN_AUTO_ATTR_S;

typedef struct ISP_FPN_ATTR_S {
    XMEDIA_BOOL               bEnable;
    ISP_OP_TYPE_E         enOpType;
    ISP_FPN_TYPE_E        enFpnType;
    ISP_FPN_FRAME_INFO_S  stFpnFrmInfo;
    ISP_FPN_MANUAL_ATTR_S stManual;
    ISP_FPN_AUTO_ATTR_S   stAuto;
} ISP_FPN_ATTR_S;

typedef struct ISP_DEHAZE_MANUAL_ATTR_S {
    XMEDIA_U8              u8strength;
} ISP_DEHAZE_MANUAL_ATTR_S;

typedef struct ISP_DEHAZE_AUTO_ATTR_S {
    XMEDIA_U8              u8strength;
} ISP_DEHAZE_AUTO_ATTR_S;

typedef struct ISP_DEHAZE_ATTR_S {
    XMEDIA_BOOL  bEnable;
    XMEDIA_BOOL  bUserLutEnable;
    XMEDIA_U8    au8DehazeLut[256];
    ISP_OP_TYPE_E      enOpType;
    ISP_DEHAZE_MANUAL_ATTR_S stManual;
    ISP_DEHAZE_AUTO_ATTR_S   stAuto;
    XMEDIA_U16       u16TmprfltIncrCoef;
    XMEDIA_U16       u16TmprfltDecrCoef;

} ISP_DEHAZE_ATTR_S;

typedef struct ISP_DEPURPLESTR_MANUAL_ATTR_S {
    XMEDIA_U8 u8DePurpleCrStr;
    XMEDIA_U8 u8DePurpleCbStr;
} ISP_DEPURPLESTR_MANUAL_ATTR_S;

typedef struct ISP_DEPURPLESTR_AUTO_ATTR_S {
    XMEDIA_U8 au8DePurpleCrStr[ISP_EXP_RATIO_STRENGTH_NUM];
    XMEDIA_U8 au8DePurpleCbStr[ISP_EXP_RATIO_STRENGTH_NUM];
} ISP_DEPURPLESTR_AUTO_ATTR_S;

typedef struct ISP_LOCAL_CAC_ATTR_S {
    XMEDIA_BOOL bEnable;
    XMEDIA_U16  u16PurpleDetRange;
    XMEDIA_U16  u16VarThr;
    XMEDIA_U16  u16RDetThr[ISP_LCAC_DET_NUM];
    XMEDIA_U16  u16GDetThr[ISP_LCAC_DET_NUM];
    XMEDIA_U16  u16BDetThr[ISP_LCAC_DET_NUM];
    XMEDIA_U16  u16LumaDetThr[ISP_LCAC_DET_NUM];
    XMEDIA_S16  s16CbCrRatio[ISP_LCAC_DET_NUM];
    ISP_OP_TYPE_E      enOpType;
    ISP_DEPURPLESTR_MANUAL_ATTR_S stManual;
    ISP_DEPURPLESTR_AUTO_ATTR_S   stAuto;
} ISP_LOCAL_CAC_ATTR_S;

typedef struct ISP_GLOBAL_CAC_ATTR_S {
    XMEDIA_BOOL bEnable;
    XMEDIA_U16  u16VerCoordinate;
    XMEDIA_U16  u16HorCoordinate;
    XMEDIA_S16  s16ParamRedA;
    XMEDIA_S16  s16ParamRedB;
    XMEDIA_S16  s16ParamRedC;
    XMEDIA_S16  s16ParamBlueA;
    XMEDIA_S16  s16ParamBlueB;
    XMEDIA_S16  s16ParamBlueC;
    XMEDIA_U8   u8VerNormShift;
    XMEDIA_U8   u8VerNormFactor;
    XMEDIA_U8   u8HorNormShift;
    XMEDIA_U8   u8HorNormFactor;
    XMEDIA_U16  u16CorVarThr;
} ISP_GLOBAL_CAC_ATTR_S;

typedef struct ISP_RC_ATTR_S {
    XMEDIA_BOOL bEnable;
    POINT_S stCenterCoor;
    XMEDIA_U32  u32Radius;
} ISP_RC_ATTR_S;

typedef struct ISP_INNER_STATE_INFO_S {

    XMEDIA_U16 au16TextureStr[ISP_SHARPEN_GAIN_NUM];
    XMEDIA_U16 au16EdgeStr[ISP_SHARPEN_GAIN_NUM];
    XMEDIA_U16 u16TextureFreq;
    XMEDIA_U16 u16EdgeFreq;
    XMEDIA_U8  u8OverShoot;
    XMEDIA_U8  u8UnderShoot;
    XMEDIA_U8  u8ShootSupStr;

    XMEDIA_U8   u8NrLscRatio;
    XMEDIA_U16  au16CoarseStr[ISP_BAYER_CHN_NUM];
    XMEDIA_U8   au8WDRFrameStr[WDR_MAX_FRAME_NUM];
    XMEDIA_U8   au8ChromaStr[ISP_BAYER_CHN_NUM];
    XMEDIA_U8   u8FineStr;
    XMEDIA_U16  u16CoringWgt;

    XMEDIA_U16 u16DeHazeStrengthActual;
    XMEDIA_U16 u16DrcStrengthActual;
    XMEDIA_U32 u32WDRExpRatioActual[3];
    XMEDIA_BOOL bWDRSwitchFinish;
    XMEDIA_BOOL bResSwitchFinish;
    XMEDIA_U16 au16BLActual[ISP_BAYER_CHN_NUM];
} ISP_INNER_STATE_INFO_S;

typedef enum ISP_AE_SWITCH_E {
    ISP_AE_AFTER_DG = 0,
    ISP_AE_AFTER_WB,
    ISP_AE_AFTER_DRC,
    ISP_AE_SWITCH_BUTT
} ISP_AE_SWITCH_E;

typedef enum ISP_AE_FOUR_PLANE_MODE_E {
    ISP_AE_FOUR_PLANE_MODE_DISABLE = 0,
    ISP_AE_FOUR_PLANE_MODE_ENABLE,
    ISP_AE_FOUR_PLANE_MODE_BUTT
} ISP_AE_FOUR_PLANE_MODE_E;

typedef enum ISP_AE_HIST_SKIP_E {
    ISP_AE_HIST_SKIP_EVERY_PIXEL = 0,
    ISP_AE_HIST_SKIP_EVERY_2ND_PIXEL,
    ISP_AE_HIST_SKIP_EVERY_3RD_PIXEL,
    ISP_AE_HIST_SKIP_EVERY_4TH_PIXEL,
    ISP_AE_HIST_SKIP_EVERY_5TH_PIXEL,
    ISP_AE_HIST_SKIP_EVERY_8TH_PIXEL,
    ISP_AE_HIST_SKIP_EVERY_9TH_PIXEL,
    ISP_AE_HIST_SKIP_BUTT
} ISP_AE_HIST_SKIP_E;

typedef enum ISP_AE_HIST_OFFSET_X_E {
    ISP_AE_HIST_START_FORM_FIRST_COLUMN = 0,
    ISP_AE_HIST_START_FORM_SECOND_COLUMN,
    ISP_AE_HIST_OFFSET_X_BUTT
} ISP_AE_HIST_OFFSET_X_E;

typedef enum ISP_AE_HIST_OFFSET_Y_E {
    ISP_AE_HIST_START_FORM_FIRST_ROW = 0,
    ISP_AE_HIST_START_FORM_SECOND_ROW,
    ISP_AE_HIST_OFFSET_Y_BUTT
} ISP_AE_HIST_OFFSET_Y_E;

typedef struct ISP_AE_HIST_CONFIG_S {
    ISP_AE_HIST_SKIP_E enHistSkipX;
    ISP_AE_HIST_SKIP_E enHistSkipY;
    ISP_AE_HIST_OFFSET_X_E enHistOffsetX;
    ISP_AE_HIST_OFFSET_Y_E enHistOffsetY;
} ISP_AE_HIST_CONFIG_S;

typedef enum ISP_AE_STAT_MODE__E {
    ISP_AE_NORMAL = 0,
    ISP_AE_ROOT,
    ISP_AE_MODE_BUTT
} ISP_AE_STAT_MODE_E;

typedef struct ISP_AE_CROP_S {
    XMEDIA_BOOL bEnable;
    XMEDIA_U16  u16X;
    XMEDIA_U16  u16Y;
    XMEDIA_U16  u16W;
    XMEDIA_U16  u16H;
} ISP_AE_CROP_S;

#define HIST_THRESH_NUM                           (4)
typedef struct ISP_AE_STATISTICS_CFG_S {
    ISP_AE_SWITCH_E          enAESwitch;
    ISP_AE_HIST_CONFIG_S     stHistConfig;
    ISP_AE_FOUR_PLANE_MODE_E enFourPlaneMode;
    ISP_AE_STAT_MODE_E       enHistMode;
    ISP_AE_STAT_MODE_E       enAverMode;
    ISP_AE_STAT_MODE_E       enMaxGainMode;
    ISP_AE_CROP_S            stCrop;
    XMEDIA_U8 au8Weight[AE_ZONE_ROW][AE_ZONE_COLUMN];
} ISP_AE_STATISTICS_CFG_S;

#define SMART_CLASS_MAX                           (2)

typedef struct ISP_SMART_ROI_S {
    XMEDIA_BOOL  bEnable;
    XMEDIA_BOOL  bAvailable;
    XMEDIA_U8    u8Luma ;
} ISP_SMART_ROI_S;

typedef struct ISP_SMART_INFO_S {
    ISP_SMART_ROI_S  stROI[SMART_CLASS_MAX];
} ISP_SMART_INFO_S;

typedef enum ISP_AF_PEAK_MODE_E {
    ISP_AF_STA_NORM         = 0,
    ISP_AF_STA_PEAK,
    ISP_AF_STA_BUTT
} ISP_AF_PEAK_MODE_E;

typedef enum ISP_AF_SQU_MODE_E {
    ISP_AF_STA_SUM_NORM     = 0,
    ISP_AF_STA_SUM_SQU,
    ISP_AF_STA_SUM_BUTT
} ISP_AF_SQU_MODE_E;
typedef struct ISP_AF_CROP_S {
    XMEDIA_BOOL bEnable;
    XMEDIA_U16  u16X;
    XMEDIA_U16  u16Y;
    XMEDIA_U16  u16W;
    XMEDIA_U16  u16H;
} ISP_AF_CROP_S;

typedef enum ISP_AF_STATISTICS_POS_E {
    ISP_AF_STATISTICS_AFTER_DGAIN    = 0,
    ISP_AF_STATISTICS_AFTER_DRC,
    ISP_AF_STATISTICS_AFTER_CSC,
    ISP_AF_STATISTICS_BUTT
} ISP_AF_STATISTICS_POS_E;

typedef struct ISP_AF_RAW_CFG_S {
    XMEDIA_U8               GammaGainLimit;
    XMEDIA_U8               GammaValue;
    ISP_BAYER_FORMAT_E  enPattern;
} ISP_AF_RAW_CFG_S;
typedef struct ISP_AF_PRE_FILTER_CFG_S {
    XMEDIA_BOOL             bEn;
    XMEDIA_U16              u16strength;
} ISP_AF_PRE_FILTER_CFG_S;

typedef struct ISP_AF_CFG_S {
    XMEDIA_BOOL                 bEnable;
    XMEDIA_U16                  u16Hwnd;
    XMEDIA_U16                  u16Vwnd;
    XMEDIA_U16                  u16Hsize;
    XMEDIA_U16                  u16Vsize;
    ISP_AF_PEAK_MODE_E      enPeakMode;
    ISP_AF_SQU_MODE_E       enSquMode;
    ISP_AF_CROP_S           stCrop;
    ISP_AF_STATISTICS_POS_E enStatisticsPos;
    ISP_AF_RAW_CFG_S        stRawCfg;
    ISP_AF_PRE_FILTER_CFG_S stPreFltCfg;
    XMEDIA_U16                  u16HighLumaTh;
} ISP_AF_CFG_S;
typedef struct ISP_AF_LD_S {
    XMEDIA_BOOL     bLdEn;
    XMEDIA_U16      u16ThLow;
    XMEDIA_U16      u16GainLow;
    XMEDIA_U16      u16SlpLow;
    XMEDIA_U16      u16ThHigh;
    XMEDIA_U16      u16GainHigh;
    XMEDIA_U16      u16SlpHigh;
} ISP_AF_LD_S;
typedef struct ISP_AF_CORING_S {
    XMEDIA_U16      u16Th;
    XMEDIA_U16      u16Slp;
    XMEDIA_U16      u16Lmt;
} ISP_AF_CORING_S ;


#define IIR_EN_NUM                                (3)
#define IIR_GAIN_NUM                              (7)
#define IIR_SHIFT_NUM                             (4)
typedef struct ISP_AF_H_PARAM_S {
    XMEDIA_BOOL         bNarrowBand;
    XMEDIA_BOOL         abIIREn[IIR_EN_NUM];
    XMEDIA_U8           u8IIRShift;
    XMEDIA_S16          as16IIRGain[IIR_GAIN_NUM];
    XMEDIA_U16          au16IIRShift[IIR_SHIFT_NUM];
    ISP_AF_LD_S     stLd;
    ISP_AF_CORING_S stCoring;
} ISP_AF_H_PARAM_S;

#define FIR_GAIN_NUM                              (5)

typedef struct ISP_AF_V_PARAM_S {
    XMEDIA_S16          as16FIRH[FIR_GAIN_NUM];
    ISP_AF_LD_S     stLd;
    ISP_AF_CORING_S stCoring;
} ISP_AF_V_PARAM_S;

#define ACC_SHIFT_H_NUM                           (2)
#define ACC_SHIFT_V_NUM                           (2)

typedef struct ISP_AF_FV_PARAM_S {
    XMEDIA_U16 u16AccShiftY;
    XMEDIA_U16 au16AccShiftH[ACC_SHIFT_H_NUM];
    XMEDIA_U16 au16AccShiftV[ACC_SHIFT_V_NUM];
    XMEDIA_U16 u16HlCntShift;
} ISP_AF_FV_PARAM_S;

typedef struct ISP_FOCUS_STATISTICS_CFG_S {
    ISP_AF_CFG_S        stConfig;
    ISP_AF_H_PARAM_S    stHParam_IIR0;
    ISP_AF_H_PARAM_S    stHParam_IIR1;
    ISP_AF_V_PARAM_S    stVParam_FIR0;
    ISP_AF_V_PARAM_S    stVParam_FIR1;
    ISP_AF_FV_PARAM_S   stFVParam;
} ISP_FOCUS_STATISTICS_CFG_S;

typedef union ISP_STATISTICS_CTRL_U {
    XMEDIA_U64  u64Key;
    struct {
        XMEDIA_U64  bit1FEAeGloStat     : 1 ;
        XMEDIA_U64  bit1FEAeLocStat     : 1 ;
        XMEDIA_U64  bit1FEAeStiGloStat  : 1 ;
        XMEDIA_U64  bit1FEAeStiLocStat  : 1 ;
        XMEDIA_U64  bit1BEAeGloStat     : 1 ;
        XMEDIA_U64  bit1BEAeLocStat     : 1 ;
        XMEDIA_U64  bit1BEAeStiGloStat  : 1 ;
        XMEDIA_U64  bit1BEAeStiLocStat  : 1 ;
        XMEDIA_U64  bit1AwbStat1        : 1 ;
        XMEDIA_U64  bit1AwbStat2        : 1 ;
        XMEDIA_U64  bit2Rsv0            : 2 ;
        XMEDIA_U64  bit1FEAfStat        : 1 ;
        XMEDIA_U64  bit1BEAfStat        : 1 ;
        XMEDIA_U64  bit2Rsv1            : 2 ;
        XMEDIA_U64  bit1Dehaze          : 1 ;
        XMEDIA_U64  bit1MgStat          : 1 ;
        XMEDIA_U64  bit14Rsv            : 14;
        XMEDIA_U64  bit32IsrAccess      : 32;
    };
} ISP_STATISTICS_CTRL_U;


#define HIST_NUM                                  (1024)
#define BAYER_PATTERN_NUM                         (4)
#define WDR_CHN_MAX                               (4)
#define ISP_CHN_MAX_NUM                           (4)

typedef struct ISP_AE_GRID_INFO_S {
    XMEDIA_U16 au16GridYPos[AE_ZONE_ROW + 1];
    XMEDIA_U16 au16GridXPos[AE_ZONE_COLUMN + 1];
    XMEDIA_U8  u8Status;
} ISP_AE_GRID_INFO_S;

typedef struct ISP_MG_GRID_INFO_S {
    XMEDIA_U16 au16GridYPos[MG_ZONE_ROW + 1];
    XMEDIA_U16 au16GridXPos[MG_ZONE_COLUMN + 1];
    XMEDIA_U8  u8Status;
} ISP_MG_GRID_INFO_S;

typedef struct ISP_AWB_GRID_INFO_S {
    XMEDIA_U16 au16GridYPos[AWB_ZONE_ORIG_ROW  + 1];
    XMEDIA_U16 au16GridXPos[AWB_ZONE_ORIG_COLUMN + 1];
    XMEDIA_U8  u8Status;
} ISP_AWB_GRID_INFO_S;

typedef struct ISP_FOCUS_GRID_INFO_S {
    XMEDIA_U16 au16GridYPos[AF_ZONE_ROW + 1];
    XMEDIA_U16 au16GridXPos[AF_ZONE_COLUMN + 1];
    XMEDIA_U8  u8Status;
} ISP_FOCUS_GRID_INFO_S;


typedef struct ISP_AE_STATISTICS_S {
    XMEDIA_U32 au32FEHist1024Value[ISP_CHN_MAX_NUM][HIST_NUM];
    XMEDIA_U16 au16FEGlobalAvg[ISP_CHN_MAX_NUM][BAYER_PATTERN_NUM];
    XMEDIA_U16 au16FEZoneAvg[ISP_CHN_MAX_NUM][AE_ZONE_ROW][AE_ZONE_COLUMN][BAYER_PATTERN_NUM];
    XMEDIA_U32 au32BEHist1024Value[HIST_NUM];
    XMEDIA_U16 au16BEGlobalAvg[BAYER_PATTERN_NUM];
    XMEDIA_U16 au16BEZoneAvg[AE_ZONE_ROW][AE_ZONE_COLUMN][BAYER_PATTERN_NUM];
    ISP_AE_GRID_INFO_S stFEGridInfo;
    ISP_AE_GRID_INFO_S stBEGridInfo;
} ISP_AE_STATISTICS_S;

typedef struct ISP_AE_STITCH_STATISTICS_S {
    XMEDIA_U32 au32FEHist1024Value[ISP_CHN_MAX_NUM][HIST_NUM];
    XMEDIA_U16 au16FEGlobalAvg[ISP_CHN_MAX_NUM][BAYER_PATTERN_NUM];
    XMEDIA_U16 au16FEZoneAvg[VI_MAX_PIPE_NUM][ISP_CHN_MAX_NUM][AE_ZONE_ROW][AE_ZONE_COLUMN][BAYER_PATTERN_NUM];
    XMEDIA_U32 au32BEHist1024Value[HIST_NUM];
    XMEDIA_U16 au16BEGlobalAvg[BAYER_PATTERN_NUM];
    XMEDIA_U16 au16BEZoneAvg[VI_MAX_PIPE_NUM][AE_ZONE_ROW][AE_ZONE_COLUMN][BAYER_PATTERN_NUM];
} ISP_AE_STITCH_STATISTICS_S;

typedef struct ISP_MG_STATISTICS_S {
    XMEDIA_U16 au16ZoneAvg[MG_ZONE_ROW][MG_ZONE_COLUMN][BAYER_PATTERN_NUM];
    ISP_MG_GRID_INFO_S stGridInfo;
} ISP_MG_STATISTICS_S;



typedef enum ISP_AWB_SWITCH_E {
    ISP_AWB_AFTER_DG                   = 0,
    ISP_AWB_AFTER_Expander,
    ISP_AWB_AFTER_DRC,
    ISP_AWB_SWITCH_BUTT
} ISP_AWB_SWITCH_E;

typedef struct ISP_AWB_CROP_S {
    XMEDIA_BOOL bEnable;
    XMEDIA_U16  u16X;
    XMEDIA_U16  u16Y;
    XMEDIA_U16  u16W;
    XMEDIA_U16  u16H;
} ISP_AWB_CROP_S;

typedef struct ISP_WB_STATISTICS_CFG_S {
    ISP_AWB_SWITCH_E enAWBSwitch;
    XMEDIA_U16 u16ZoneRow;
    XMEDIA_U16 u16ZoneCol;
    XMEDIA_U16 u16ZoneBin;
    XMEDIA_U16 au16HistBinThresh[4];
    XMEDIA_U16 u16WhiteLevel;
    XMEDIA_U16 u16BlackLevel;
    XMEDIA_U16 u16CbMax;
    XMEDIA_U16 u16CbMin;
    XMEDIA_U16 u16CrMax;
    XMEDIA_U16 u16CrMin;
    ISP_AWB_CROP_S stCrop;
} ISP_WB_STATISTICS_CFG_S;

typedef struct ISP_WB_STITCH_STATISTICS_S {
    XMEDIA_U16  u16ZoneRow;
    XMEDIA_U16  u16ZoneCol;
    XMEDIA_U16  au16ZoneAvgR[AWB_ZONE_STITCH_MAX];
    XMEDIA_U16  au16ZoneAvgG[AWB_ZONE_STITCH_MAX];
    XMEDIA_U16  au16ZoneAvgB[AWB_ZONE_STITCH_MAX];
    XMEDIA_U16  au16ZoneCountAll[AWB_ZONE_STITCH_MAX];
} ISP_WB_STITCH_STATISTICS_S;

typedef struct ISP_WB_STATISTICS_S {
    XMEDIA_U16 u16GlobalR;
    XMEDIA_U16 u16GlobalG;
    XMEDIA_U16 u16GlobalB;
    XMEDIA_U16 u16CountAll;

    XMEDIA_U16 au16ZoneAvgR[AWB_ZONE_NUM];
    XMEDIA_U16 au16ZoneAvgG[AWB_ZONE_NUM];
    XMEDIA_U16 au16ZoneAvgB[AWB_ZONE_NUM];
    XMEDIA_U16 au16ZoneCountAll[AWB_ZONE_NUM];
    ISP_AWB_GRID_INFO_S stGridInfo;
} ISP_WB_STATISTICS_S;

typedef struct ISP_FOCUS_ZONE_S {
    XMEDIA_U16  u16v1;
    XMEDIA_U16  u16h1;
    XMEDIA_U16  u16v2;
    XMEDIA_U16  u16h2;
    XMEDIA_U16  u16y;
    XMEDIA_U16  u16HlCnt;
} ISP_FOCUS_ZONE_S;

typedef struct ISP_FE_FOCUS_STATISTICS_S {
    ISP_FOCUS_ZONE_S stZoneMetrics[WDR_CHN_MAX][AF_ZONE_ROW][AF_ZONE_COLUMN];
} ISP_FE_FOCUS_STATISTICS_S;

typedef struct ISP_BE_FOCUS_STATISTICS_S {
    ISP_FOCUS_ZONE_S stZoneMetrics[AF_ZONE_ROW][AF_ZONE_COLUMN];
} ISP_BE_FOCUS_STATISTICS_S;

typedef struct ISP_AF_STATISTICS_S {
    ISP_FE_FOCUS_STATISTICS_S      stFEAFStat;
    ISP_BE_FOCUS_STATISTICS_S      stBEAFStat;
    ISP_FOCUS_GRID_INFO_S          stFEAFGridInfo;
    ISP_FOCUS_GRID_INFO_S          stBEAFGridInfo;
} ISP_AF_STATISTICS_S;


typedef struct ISP_STATISTICS_CFG_S {
    ISP_STATISTICS_CTRL_U       unKey;
    ISP_AE_STATISTICS_CFG_S     stAECfg;
    ISP_WB_STATISTICS_CFG_S     stWBCfg;
    ISP_FOCUS_STATISTICS_CFG_S  stFocusCfg;
} ISP_STATISTICS_CFG_S;

typedef struct ISP_INIT_ATTR_S {
    XMEDIA_BOOL bIsIrMode;
    XMEDIA_U32 u32ExpTime;
    XMEDIA_U32 u32AGain;
    XMEDIA_U32 u32DGain;
    XMEDIA_U32 u32ISPDGain;
    XMEDIA_U32 u32Exposure;
    XMEDIA_U32 u32InitIso;
    XMEDIA_U32 u32LinesPer500ms;
    XMEDIA_U32 u32PirisFNO;
    XMEDIA_U16 u16WBRgain;
    XMEDIA_U16 u16WBGgain;
    XMEDIA_U16 u16WBBgain;
    XMEDIA_U16 u16SampleRgain;
    XMEDIA_U16 u16SampleBgain;
    XMEDIA_U16 au16CCM[CCM_MATRIX_SIZE];
} ISP_INIT_ATTR_S ;

typedef struct ISP_DEBUG_INFO_S {
    XMEDIA_BOOL bDebugEn;
    XMEDIA_U64  u64PhyAddr;
    XMEDIA_U32  u32Depth;
} ISP_DEBUG_INFO_S;

typedef struct ISP_DBG_ATTR_S {
    XMEDIA_U32  u32Rsv;
} ISP_DBG_ATTR_S;

typedef struct ISP_DBG_STATUS_S {
    XMEDIA_U32  u32FrmNumBgn;
    XMEDIA_U32  u32Rsv;
    XMEDIA_U32  u32FrmNumEnd;
} ISP_DBG_STATUS_S;

typedef enum ISP_SNS_TYPE_E {
    ISP_SNS_I2C_TYPE = 0,
    ISP_SNS_SSP_TYPE,

    ISP_SNS_TYPE_BUTT,
} ISP_SNS_TYPE_E;

typedef union ISP_SNS_COMMBUS_U {
    XMEDIA_S8   s8I2cDev;
    struct {
        XMEDIA_S8  bit4SspDev       : 4;
        XMEDIA_S8  bit4SspCs        : 4;
    } s8SspDev;
} ISP_SNS_COMMBUS_U;

typedef struct ISP_I2C_DATA_S {
    XMEDIA_BOOL bUpdate;
    XMEDIA_U8   u8DelayFrmNum;
    XMEDIA_U8   u8IntPos;

    XMEDIA_U8   u8DevAddr;
    XMEDIA_U32  u32RegAddr;
    XMEDIA_U32  u32AddrByteNum;
    XMEDIA_U32  u32Data;
    XMEDIA_U32  u32DataByteNum;
} ISP_I2C_DATA_S;

typedef struct ISP_SSP_DATA_S {
    XMEDIA_BOOL bUpdate;
    XMEDIA_U8   u8DelayFrmNum;
    XMEDIA_U8   u8IntPos;
    XMEDIA_U32  u32DevAddr;
    XMEDIA_U32  u32DevAddrByteNum;
    XMEDIA_U32  u32RegAddr;
    XMEDIA_U32  u32RegAddrByteNum;
    XMEDIA_U32  u32Data;
    XMEDIA_U32  u32DataByteNum;
} ISP_SSP_DATA_S;

typedef struct ISP_SNS_REGS_INFO_S {
    ISP_SNS_TYPE_E enSnsType;
    XMEDIA_U32  u32RegNum;
    XMEDIA_U8   u8Cfg2ValidDelayMax;
    ISP_SNS_COMMBUS_U  unComBus;
    union {
        ISP_I2C_DATA_S astI2cData[ISP_MAX_SNS_REGS];
        ISP_SSP_DATA_S astSspData[ISP_MAX_SNS_REGS];
    };

    struct {
        XMEDIA_BOOL bUpdate;
        XMEDIA_U8   u8DelayFrmNum;
        XMEDIA_U32  u32SlaveVsTime;
        XMEDIA_U32  u32SlaveBindDev;
    } stSlvSync;

    XMEDIA_BOOL bConfig;
} ISP_SNS_REGS_INFO_S;

typedef enum ISP_VD_TYPE_E {
    ISP_VD_FE_START   = 0,
    ISP_VD_FE_END,
    ISP_VD_BE_END,

    ISP_VD_BUTT
} ISP_VD_TYPE_E;

typedef struct ISP_REG_ATTR_S {
    XMEDIA_VOID *pIspExtRegAddr;
    XMEDIA_U32 u32IspExtRegSize;
    XMEDIA_VOID *pAeExtRegAddr;
    XMEDIA_U32 u32AeExtRegSize;
    XMEDIA_VOID *pAwbExtRegAddr;
    XMEDIA_U32 u32AwbExtRegSize;
} ISP_REG_ATTR_S;


typedef enum ISP_IRIS_TYPE_E {
    ISP_IRIS_DC_TYPE = 0,
    ISP_IRIS_P_TYPE,

    ISP_IRIS_TYPE_BUTT,
} ISP_IRIS_TYPE_E;

typedef enum ISP_IRIS_F_NO_E {
    ISP_IRIS_F_NO_32_0 = 0,
    ISP_IRIS_F_NO_22_0,
    ISP_IRIS_F_NO_16_0,
    ISP_IRIS_F_NO_11_0,
    ISP_IRIS_F_NO_8_0,
    ISP_IRIS_F_NO_5_6,
    ISP_IRIS_F_NO_4_0,
    ISP_IRIS_F_NO_2_8,
    ISP_IRIS_F_NO_2_0,
    ISP_IRIS_F_NO_1_4,
    ISP_IRIS_F_NO_1_0,

    ISP_IRIS_F_NO_BUTT,
} ISP_IRIS_F_NO_E;

typedef struct ISP_DCIRIS_ATTR_S {
    XMEDIA_S32 s32Kp;
    XMEDIA_S32 s32Ki;
    XMEDIA_S32 s32Kd;
    XMEDIA_U32 u32MinPwmDuty;
    XMEDIA_U32 u32MaxPwmDuty;
    XMEDIA_U32 u32OpenPwmDuty;
} ISP_DCIRIS_ATTR_S;

typedef struct ISP_PIRIS_ATTR_S {
    XMEDIA_BOOL bStepFNOTableChange;
    XMEDIA_BOOL bZeroIsMax;
    XMEDIA_U16 u16TotalStep;
    XMEDIA_U16 u16StepCount;
    XMEDIA_U16 au16StepFNOTable[AI_MAX_STEP_FNO_NUM];
    ISP_IRIS_F_NO_E  enMaxIrisFNOTarget;
    ISP_IRIS_F_NO_E  enMinIrisFNOTarget;

    XMEDIA_BOOL bFNOExValid;
    XMEDIA_U32 u32MaxIrisFNOTarget;
    XMEDIA_U32 u32MinIrisFNOTarget;
} ISP_PIRIS_ATTR_S;

typedef struct ISP_MI_ATTR_S {
    XMEDIA_U32  u32HoldValue;
    ISP_IRIS_F_NO_E  enIrisFNO;
} ISP_MI_ATTR_S;

typedef enum ISP_IRIS_STATUS_E {
    ISP_IRIS_KEEP  = 0,
    ISP_IRIS_OPEN  = 1,
    ISP_IRIS_CLOSE = 2,
    ISP_IRIS_BUTT
} ISP_IRIS_STATUS_E;

typedef struct ISP_IRIS_ATTR_S {
    XMEDIA_BOOL bEnable;
    ISP_OP_TYPE_E   enOpType;
    ISP_IRIS_TYPE_E enIrisType;
    ISP_IRIS_STATUS_E enIrisStatus;
    ISP_MI_ATTR_S   stMIAttr;
} ISP_IRIS_ATTR_S;


typedef enum ISP_AE_MODE_E {
    AE_MODE_SLOW_SHUTTER = 0,
    AE_MODE_FIX_FRAME_RATE  = 1,
    AE_MODE_BUTT
} ISP_AE_MODE_E;
typedef enum ISP_AE_STRATEGY_E {
    AE_EXP_HIGHLIGHT_PRIOR = 0,
    AE_EXP_LOWLIGHT_PRIOR  = 1,
    AE_STRATEGY_MODE_BUTT
} ISP_AE_STRATEGY_E;
typedef struct ISP_AE_RANGE_S {
    XMEDIA_U32 u32Max;
    XMEDIA_U32 u32Min;
} ISP_AE_RANGE_S;

typedef struct ISP_AE_DELAY_S {
    XMEDIA_U16 u16BlackDelayFrame;
    XMEDIA_U16 u16WhiteDelayFrame;
} ISP_AE_DELAY_S;

typedef enum ISP_ANTIFLICKER_MODE_E {
    ISP_ANTIFLICKER_NORMAL_MODE = 0x0,
    ISP_ANTIFLICKER_AUTO_MODE   = 0x1,
    ISP_ANTIFLICKER_MODE_BUTT
} ISP_ANTIFLICKER_MODE_E;

typedef struct ISP_ANTIFLICKER_S {
    XMEDIA_BOOL bEnable;
    XMEDIA_U8   u8Frequency;
    ISP_ANTIFLICKER_MODE_E  enMode;
} ISP_ANTIFLICKER_S;

typedef struct ISP_SUBFLICKER_S {
    XMEDIA_BOOL bEnable;

    XMEDIA_U8   u8LumaDiff;
} ISP_SUBFLICKER_S;

typedef enum ISP_FSWDR_MODE_E {
    ISP_FSWDR_NORMAL_MODE          = 0x0,
    ISP_FSWDR_LONG_FRAME_MODE      = 0x1,
    ISP_FSWDR_AUTO_LONG_FRAME_MODE = 0x2,
    ISP_FSWDR_MODE_BUTT
} ISP_FSWDR_MODE_E;


typedef struct ISP_AE_ATTR_S {
    ISP_AE_RANGE_S stExpTimeRange;
    ISP_AE_RANGE_S stAGainRange;
    ISP_AE_RANGE_S stDGainRange;
    ISP_AE_RANGE_S stISPDGainRange;
    ISP_AE_RANGE_S stSysGainRange;
    XMEDIA_U32 u32GainThreshold;

    XMEDIA_U8  u8Speed;
    XMEDIA_U16  u16BlackSpeedBias;
    XMEDIA_U8  u8Tolerance;
    XMEDIA_U8  u8Compensation;
    XMEDIA_U16  u16EVBias;
    ISP_AE_STRATEGY_E enAEStrategyMode;
    XMEDIA_U16  u16HistRatioSlope;
    XMEDIA_U8   u8MaxHistOffset;

    ISP_AE_MODE_E  enAEMode;
    ISP_ANTIFLICKER_S stAntiflicker;
    ISP_SUBFLICKER_S stSubflicker;
    ISP_AE_DELAY_S stAEDelayAttr;

    XMEDIA_BOOL bManualExpValue;
    XMEDIA_U32 u32ExpValue;

    ISP_FSWDR_MODE_E enFSWDRMode;
    XMEDIA_BOOL bWDRQuick;

    XMEDIA_U16  u16ISOCalCoef;
} ISP_AE_ATTR_S;

typedef struct ISP_ME_ATTR_S {
    ISP_OP_TYPE_E enExpTimeOpType;
    ISP_OP_TYPE_E enAGainOpType;
    ISP_OP_TYPE_E enDGainOpType;
    ISP_OP_TYPE_E enISPDGainOpType;

    XMEDIA_U32 u32ExpTime;
    XMEDIA_U32 u32AGain;
    XMEDIA_U32 u32DGain;
    XMEDIA_U32 u32ISPDGain;
} ISP_ME_ATTR_S;

typedef struct ISP_EXPOSURE_ATTR_S {
    XMEDIA_BOOL         bByPass;
    ISP_OP_TYPE_E   enOpType;
    XMEDIA_U8     u8AERunInterval;
    XMEDIA_BOOL   bHistStatAdjust;
    XMEDIA_BOOL   bAERouteExValid;
    ISP_ME_ATTR_S   stManual;
    ISP_AE_ATTR_S   stAuto;
    ISP_PRIOR_FRAME_E enPriorFrame;
    XMEDIA_BOOL   bAEGainSepCfg;
} ISP_EXPOSURE_ATTR_S;


#define ISP_AE_ROUTE_MAX_NODES                    (16)
typedef struct ISP_AE_ROUTE_NODE_S {
    XMEDIA_U32  u32IntTime;
    XMEDIA_U32  u32SysGain;
    ISP_IRIS_F_NO_E  enIrisFNO;
    XMEDIA_U32  u32IrisFNOLin;
} ISP_AE_ROUTE_NODE_S;

typedef struct ISP_AE_ROUTE_S {
    XMEDIA_U32 u32TotalNum;
    ISP_AE_ROUTE_NODE_S astRouteNode[ISP_AE_ROUTE_MAX_NODES];
} ISP_AE_ROUTE_S;

#define ISP_AE_ROUTE_EX_MAX_NODES                 (16)
typedef struct ISP_AE_ROUTE_EX_NODE_S {
    XMEDIA_U32  u32IntTime;
    XMEDIA_U32  u32Again;
    XMEDIA_U32  u32Dgain;
    XMEDIA_U32  u32IspDgain;
    ISP_IRIS_F_NO_E  enIrisFNO;
    XMEDIA_U32  u32IrisFNOLin;
} ISP_AE_ROUTE_EX_NODE_S;

typedef struct ISP_AE_ROUTE_EX_S {
    XMEDIA_U32 u32TotalNum;
    ISP_AE_ROUTE_EX_NODE_S astRouteExNode[ISP_AE_ROUTE_EX_MAX_NODES];
} ISP_AE_ROUTE_EX_S;

typedef struct ISP_EXP_INFO_S {
    XMEDIA_U32 u32ExpTime;
    XMEDIA_U32 u32ShortExpTime;
    XMEDIA_U32 u32MedianExpTime;
    XMEDIA_U32 u32LongExpTime;
    XMEDIA_U32 u32AGain;
    XMEDIA_U32 u32DGain;
    XMEDIA_U32 u32AGainSF;
    XMEDIA_U32 u32DGainSF;
    XMEDIA_U32 u32ISPDGain;
    XMEDIA_U32 u32Exposure;
    XMEDIA_BOOL bExposureIsMAX;
    XMEDIA_S16 s16HistError;
    XMEDIA_U32 au32AE_Hist1024Value[HIST_NUM];

    XMEDIA_U8  u8AveLum;
    XMEDIA_U32 u32LinesPer500ms;
    XMEDIA_U32 u32PirisFNO;
    XMEDIA_U32 u32Fps;
    XMEDIA_U32 u32ISO;
    XMEDIA_U32 u32ISOSF;
    XMEDIA_U32 u32ISOCalibrate;
    XMEDIA_U32 u32RefExpRatio;
    XMEDIA_U32 u32FirstStableTime;
    ISP_AE_ROUTE_S stAERoute;
    ISP_AE_ROUTE_EX_S stAERouteEx;
    ISP_AE_ROUTE_S stAERouteSF;
    ISP_AE_ROUTE_EX_S stAERouteSFEx;
} ISP_EXP_INFO_S;


#define EXP_RATIO_NUM                             (3)
typedef struct ISP_WDR_EXPOSURE_ATTR_S {
    ISP_OP_TYPE_E enExpRatioType;
    XMEDIA_U32 au32ExpRatio[EXP_RATIO_NUM];
    XMEDIA_U32 u32ExpRatioMax;
    XMEDIA_U32 u32ExpRatioMin;
    XMEDIA_U16 u16Tolerance;
    XMEDIA_U16 u16Speed;
    XMEDIA_U16 u16RatioBias;

} ISP_WDR_EXPOSURE_ATTR_S;

typedef struct ISP_HDR_EXPOSURE_ATTR_S {
    ISP_OP_TYPE_E enExpHDRLvType;
    XMEDIA_U32 u32ExpHDRLv;
    XMEDIA_U32 u32ExpHDRLvMax;
    XMEDIA_U32 u32ExpHDRLvMin;
    XMEDIA_U32 u32ExpHDRLvWeight;

} ISP_HDR_EXPOSURE_ATTR_S;

typedef struct ISP_SMART_EXPOSURE_ATTR_S {
    XMEDIA_BOOL bEnable;
    XMEDIA_BOOL bIRMode;
    ISP_OP_TYPE_E enSmartExpType;
    XMEDIA_U16  u16ExpCoef;
    XMEDIA_U8   u8LumaTarget;
    XMEDIA_U16  u16ExpCoefMax;
    XMEDIA_U16  u16ExpCoefMin;
    XMEDIA_U8   u8SmartInterval;
    XMEDIA_U8   u8SmartSpeed;
    XMEDIA_U16  u16SmartDelayNum;
} ISP_SMART_EXPOSURE_ATTR_S;


typedef struct ISP_AWB_Calibration_Gain_S {
    XMEDIA_U16 u16AvgRgain;
    XMEDIA_U16 u16AvgBgain;
} ISP_AWB_Calibration_Gain_S;
typedef enum ISP_AWB_ALG_TYPE_E {
    AWB_ALG_LOWCOST = 0,
    AWB_ALG_ADVANCE = 1,
    AWB_ALG_BUTT
} ISP_AWB_ALG_TYPE_E;

typedef enum ISP_AWB_MULTI_LS_TYPE_E {
    AWB_MULTI_LS_SAT = 0,
    AWB_MULTI_LS_CCM = 1,
    AWB_MULTI_LS_BUTT
} ISP_AWB_MULTI_LS_TYPE_E;

typedef enum ISP_AWB_INDOOR_OUTDOOR_STATUS_E {
    AWB_INDOOR_MODE = 0,
    AWB_OUTDOOR_MODE = 1,
    AWB_INDOOR_OUTDOOR_BUTT
} ISP_AWB_INDOOR_OUTDOOR_STATUS_E;

typedef struct ISP_AWB_CT_LIMIT_ATTR_S {
    XMEDIA_BOOL bEnable;
    ISP_OP_TYPE_E   enOpType;

    XMEDIA_U16 u16HighRgLimit;
    XMEDIA_U16 u16HighBgLimit;
    XMEDIA_U16 u16LowRgLimit;
    XMEDIA_U16 u16LowBgLimit;
} ISP_AWB_CT_LIMIT_ATTR_S;

typedef struct ISP_AWB_IN_OUT_ATTR_S {
    XMEDIA_BOOL bEnable;
    ISP_OP_TYPE_E   enOpType;
    ISP_AWB_INDOOR_OUTDOOR_STATUS_E  enOutdoorStatus;
    XMEDIA_U32 u32OutThresh;
    XMEDIA_U16 u16LowStart;
    XMEDIA_U16 u16LowStop;
    XMEDIA_U16 u16HighStart;
    XMEDIA_U16 u16HighStop;
    XMEDIA_BOOL bGreenEnhanceEn;
    XMEDIA_U8   u8OutShiftLimit;
} ISP_AWB_IN_OUT_ATTR_S;

typedef struct ISP_AWB_CBCR_TRACK_ATTR_S {
    XMEDIA_BOOL bEnable;

    XMEDIA_U16  au16CrMax[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U16  au16CrMin[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U16  au16CbMax[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U16  au16CbMin[ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_AWB_CBCR_TRACK_ATTR_S;

#define AWB_LUM_HIST_NUM                          (6)
typedef struct ISP_AWB_LUM_HISTGRAM_ATTR_S {
    XMEDIA_BOOL bEnable;
    ISP_OP_TYPE_E   enOpType;
    XMEDIA_U8   au8HistThresh[AWB_LUM_HIST_NUM];
    XMEDIA_U16  au16HistWt[AWB_LUM_HIST_NUM];
} ISP_AWB_LUM_HISTGRAM_ATTR_S;

typedef struct ISP_AWB_LIGHTSOURCE_INFO_S {
    XMEDIA_U16 u16WhiteRgain;
    XMEDIA_U16 u16WhiteBgain;
    XMEDIA_U16 u16ExpQuant;
    XMEDIA_U8  u8LightStatus;
    XMEDIA_U8  u8Radius;
} ISP_AWB_EXTRA_LIGHTSOURCE_INFO_S;

#define AWB_LS_NUM                                (4)
#define AWB_MULTI_CT_NUM                          (8)
typedef struct ISP_AWB_ATTR_EX_S {
    XMEDIA_U8  u8Tolerance;
    XMEDIA_U8  u8ZoneRadius;
    XMEDIA_U16 u16CurveLLimit;
    XMEDIA_U16 u16CurveRLimit;

    XMEDIA_BOOL  bExtraLightEn;
    ISP_AWB_EXTRA_LIGHTSOURCE_INFO_S    stLightInfo[AWB_LS_NUM];
    ISP_AWB_IN_OUT_ATTR_S               stInOrOut;

    XMEDIA_BOOL bMultiLightSourceEn;
    ISP_AWB_MULTI_LS_TYPE_E enMultiLSType;
    XMEDIA_U16  u16MultiLSScaler;
    XMEDIA_U16  au16MultiCTBin[AWB_MULTI_CT_NUM];
    XMEDIA_U16  au16MultiCTWt[AWB_MULTI_CT_NUM];

    XMEDIA_BOOL bFineTunEn;
    XMEDIA_U8   u8FineTunStrength;
} ISP_AWB_ATTR_EX_S;

#define AWB_CURVE_PARA_NUM                        (6)
typedef struct ISP_AWB_ATTR_S {
    XMEDIA_BOOL bEnable;

    XMEDIA_U16 u16RefColorTemp;
    XMEDIA_U16 au16StaticWB[ISP_BAYER_CHN_NUM];
    XMEDIA_S32 as32CurvePara[AWB_CURVE_PARA_NUM];

    ISP_AWB_ALG_TYPE_E          enAlgType;

    XMEDIA_U8  u8RGStrength;
    XMEDIA_U8  u8BGStrength;
    XMEDIA_U16 u16Speed;
    XMEDIA_U16 u16ZoneSel;
    XMEDIA_U16 u16HighColorTemp;
    XMEDIA_U16 u16LowColorTemp;
    ISP_AWB_CT_LIMIT_ATTR_S   stCTLimit;
    XMEDIA_BOOL bShiftLimitEn;
    XMEDIA_U8  u8ShiftLimit;
    XMEDIA_BOOL bGainNormEn;
    XMEDIA_BOOL bNaturalCastEn;

    ISP_AWB_CBCR_TRACK_ATTR_S stCbCrTrack;
    ISP_AWB_LUM_HISTGRAM_ATTR_S stLumaHist;
    XMEDIA_BOOL bAWBZoneWtEn;
    XMEDIA_U8   au8ZoneWt[AWB_ZONE_ORIG_ROW *AWB_ZONE_ORIG_COLUMN];
} ISP_AWB_ATTR_S;

#define SPECAWB_FACTTBL_ELMNUM                    (7)
#define SPECAWB_FACTTBL_SIZE                      (64)
#define SPECAWB_BBL_SIZE                          (40)

typedef struct ISP_SPECAWB_BBL_TBL_S {
    XMEDIA_S16 s16Kelvin;
    XMEDIA_S16 s16Wbr;
    XMEDIA_S16 s16Wbb;
} ISP_SPECAWB_BBL_TBL_S;

typedef struct ISP_SPECAWB_KELVIN_DBB_MAP_S {
    XMEDIA_S16 s16Kelvin;
    XMEDIA_S16 s16DBB;
} ISP_SPECAWB_KELVIN_DBB_MAP_S;

typedef struct ISP_SPECKCWB_S {
    XMEDIA_S32    s32RGain;
    XMEDIA_S32    s32BGain;
} ISP_SPECKCWB_S;

typedef struct ISP_SPECKCWBS16_S {
    XMEDIA_S16    s16RGain;
    XMEDIA_S16    s16BGain;
} ISP_SPECKCWBS16_S;

typedef struct ISP_SPECAWB_FACTTBL_ELEMENT_S {
    XMEDIA_S16    s16Bv;
    XMEDIA_U8     u8FactTbl[SPECAWB_FACTTBL_SIZE][SPECAWB_FACTTBL_SIZE];
} ISP_SPECAWB_FACTTBL_ELEMENT_S;

typedef struct ISP_SPECAWB_ATTR_S {
    ISP_SPECKCWB_S                       stWBCenter;
    ISP_SPECKCWB_S                       stWBMin;
    ISP_SPECKCWB_S                       stWBMax;
    ISP_SPECKCWB_S                       stLogFact;
    XMEDIA_S32                               s32LimitFactMin;
    XMEDIA_S32                               s32LimitFactMax;
    XMEDIA_S32                               s32LimitFactLimit;
    XMEDIA_S32                               s32FactOffset;
    XMEDIA_S32                               s32BFact;
    XMEDIA_S32                               s32ACntMin;
    ISP_SPECKCWB_S                       stWBNeutral;
    XMEDIA_U8                                bLastIsFlash;
    XMEDIA_S16                               s16BvFlashELimit;
    XMEDIA_S32                               s32APercent;
    ISP_SPECKCWBS16_S                    stWBCnvTbl[SPECAWB_FACTTBL_SIZE][SPECAWB_FACTTBL_SIZE];
    XMEDIA_U8                                u8ElementNum;
    ISP_SPECAWB_FACTTBL_ELEMENT_S        stFactElement[SPECAWB_FACTTBL_ELMNUM];
    ISP_SPECAWB_KELVIN_DBB_MAP_S         stKelvinDBBTbl[SPECAWB_FACTTBL_SIZE][SPECAWB_FACTTBL_SIZE];
    ISP_SPECAWB_BBL_TBL_S                stBlackBodyTbl[SPECAWB_BBL_SIZE];
    XMEDIA_U16                               u16Fno;
} ISP_SPECAWB_ATTR_S;

typedef struct ISP_SPECAWB_CONTROL_ATTR_S {
    XMEDIA_S16 s16BlendHighBvThresh;
    XMEDIA_U16 u16BlendHighBvWt;
    XMEDIA_S16 s16BlendLowBvThresh;
    XMEDIA_U16 u16BlendLowBvWt;
} ISP_SPECAWB_CONTROL_ATTR_S;

#define SPECAWB_MAX_CAA_NUM                       3
#define SPECAWB_KEVIN_CONVER_MAX_NUM              8

typedef struct ISP_SPECAWB_CAA_CONVERSION_S {
    XMEDIA_S32 s32SrcKelvin;
    XMEDIA_S32 s32DstKelvin;
} ISP_SPECAWB_CAA_CONVERSION_S;
typedef struct ISP_SPECAWB_CAA_TBL_S {
    XMEDIA_U8   u8Enable;
    XMEDIA_S16  s16Bv;
    ISP_SPECAWB_CAA_CONVERSION_S stKelvinCon[SPECAWB_KEVIN_CONVER_MAX_NUM];
    ISP_SPECKCWBS16_S stWBCnvTbl[SPECAWB_FACTTBL_SIZE][SPECAWB_FACTTBL_SIZE];
} ISP_SPECAWB_CAA_TBL_S;

typedef struct ISP_SPECAWB_CAA_CONTROl_S {
    ISP_SPECAWB_CAA_TBL_S stControl[SPECAWB_MAX_CAA_NUM];
} ISP_SPECAWB_CAA_CONTROl_S;
typedef struct ISP_MWB_ATTR_S {
    XMEDIA_U16 u16Rgain;
    XMEDIA_U16 u16Grgain;
    XMEDIA_U16 u16Gbgain;
    XMEDIA_U16 u16Bgain;
} ISP_MWB_ATTR_S;

typedef enum ISP_AWB_ALG_E {
    ALG_AWB         = 0,
    ALG_AWB_SPEC    = 1,
    ALG_BUTT
} ISP_AWB_ALG_E;
typedef struct ISP_WB_ATTR_S {
    XMEDIA_BOOL bByPass;
    XMEDIA_U8   u8AWBRunInterval;
    ISP_OP_TYPE_E   enOpType;
    ISP_MWB_ATTR_S  stManual;
    ISP_AWB_ATTR_S  stAuto;
    ISP_AWB_ALG_E enAlgType;
} ISP_WB_ATTR_S;

typedef struct ISP_COLORMATRIX_MANUAL_S {
    XMEDIA_BOOL bSatEn;
    XMEDIA_U16 au16CCM[CCM_MATRIX_SIZE];
} ISP_COLORMATRIX_MANUAL_S;

typedef struct ISP_COLORMATRIX_PARAM_S {
    XMEDIA_U16 u16ColorTemp;
    XMEDIA_U16 au16CCM[CCM_MATRIX_SIZE];
} ISP_COLORMATRIX_PARAM_S;

typedef struct ISP_COLORMATRIX_AUTO_S {
    XMEDIA_BOOL bISOActEn;
    XMEDIA_BOOL bTempActEn;
    XMEDIA_U16  u16CCMTabNum;
    ISP_COLORMATRIX_PARAM_S astCCMTab[CCM_MATRIX_NUM];
} ISP_COLORMATRIX_AUTO_S;

typedef struct ISP_COLORMATRIX_ATTR_S {
    ISP_OP_TYPE_E enOpType;
    ISP_COLORMATRIX_MANUAL_S stManual;
    ISP_COLORMATRIX_AUTO_S stAuto;
} ISP_COLORMATRIX_ATTR_S;

typedef struct ISP_SATURATION_MANUAL_S {
    XMEDIA_U8   u8Saturation;
} ISP_SATURATION_MANUAL_S;

typedef struct ISP_SATURATION_AUTO_S {
    XMEDIA_U8   au8Sat[ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_SATURATION_AUTO_S;

typedef struct ISP_SATURATION_ATTR_S {
    ISP_OP_TYPE_E enOpType;
    ISP_SATURATION_MANUAL_S stManual;
    ISP_SATURATION_AUTO_S stAuto;
} ISP_SATURATION_ATTR_S;

typedef struct ISP_COLOR_TONE_ATTR_S {
    XMEDIA_U16 u16RedCastGain;
    XMEDIA_U16 u16GreenCastGain;
    XMEDIA_U16 u16BlueCastGain;
} ISP_COLOR_TONE_ATTR_S;

typedef struct ISP_WB_INFO_S {
    XMEDIA_U16 u16Rgain;
    XMEDIA_U16 u16Grgain;
    XMEDIA_U16 u16Gbgain;
    XMEDIA_U16 u16Bgain;
    XMEDIA_U16 u16Saturation;
    XMEDIA_U16 u16ColorTemp;
    XMEDIA_U16 au16CCM[CCM_MATRIX_SIZE];

    XMEDIA_U16 u16LS0CT;
    XMEDIA_U16 u16LS1CT;
    XMEDIA_U16 u16LS0Area;
    XMEDIA_U16 u16LS1Area;
    XMEDIA_U8  u8MultiDegree;
    XMEDIA_U16 u16ActiveShift;
    XMEDIA_U32 u32FirstStableTime;
    ISP_AWB_INDOOR_OUTDOOR_STATUS_E enInOutStatus;
    XMEDIA_S16  s16Bv;
} ISP_WB_INFO_S;




typedef struct ISP_AF_ATTR_S {
    XMEDIA_S32 s32DistanceMax;
    XMEDIA_S32 s32DistanceMin;
    XMEDIA_U8 u8Weight[AF_ZONE_ROW][AF_ZONE_COLUMN];
} ISP_AF_ATTR_S;
typedef struct ISP_MF_ATTR_S {
    XMEDIA_S32 s32DefaultSpeed;
} ISP_MF_ATTR_S;
typedef struct ISP_FOCUS_ATTR_S {
    ISP_OP_TYPE_E  enOpType;
    ISP_MF_ATTR_S  stManual;
    ISP_AF_ATTR_S  stAuto;
} ISP_FOCUS_ATTR_S;

typedef enum DNG_CFALAYOUT_TYPE_E {
    CFALAYOUT_TYPE_RECTANGULAR = 1,
    CFALAYOUT_TYPE_A,
    CFALAYOUT_TYPE_B,
    CFALAYOUT_TYPE_C,
    CFALAYOUT_TYPE_D,
    CFALAYOUT_TYPE_E,
    CFALAYOUT_TYPE_F,
    CFALAYOUT_TYPE_G,
    CFALAYOUT_TYPE_H,
    CFALAYOUT_TYPE_BUTT
} DNG_CFALAYOUT_TYPE_E;

typedef struct DNG_SRATIONAL_S {
    XMEDIA_S32 s32Numerator;
    XMEDIA_S32 s32Denominator;
} DNG_SRATIONAL_S;

typedef struct DNG_BLCREPEATDIM_S {
    XMEDIA_U16 u16BlcRepeatRows;
    XMEDIA_U16 u16BlcRepeatCols;
} DNG_BLCREPEATDIM_S;

typedef struct DNG_DEFAULTSCALE_S {
    DNG_RATIONAL_S stDefaultScaleH;
    DNG_RATIONAL_S stDefaultScaleV;
} DNG_DEFAULTSCALE_S;

typedef struct DNG_REPEATPATTERNDIM_S {
    XMEDIA_U16 u16RepeatPatternDimRows;
    XMEDIA_U16 u16RepeatPatternDimCols;
} DNG_REPEATPATTERNDIM_S;

typedef struct DNG_RAW_FORMAT_S {
    XMEDIA_U8 u8BitsPerSample;
    XMEDIA_U8 au8CfaPlaneColor[CFACOLORPLANE];
    DNG_CFALAYOUT_TYPE_E enCfaLayout;
    DNG_BLCREPEATDIM_S stBlcRepeatDim;
    XMEDIA_U32 u32WhiteLevel;
    DNG_DEFAULTSCALE_S stDefaultScale;
    DNG_REPEATPATTERNDIM_S stCfaRepeatPatternDim;
    XMEDIA_U8 au8CfaPattern[ISP_BAYER_CHN];
} DNG_RAW_FORMAT_S;

typedef struct DNG_IMAGE_STATIC_INFO_S {
    DNG_RAW_FORMAT_S stDngRawFormat;
    DNG_SRATIONAL_S astColorMatrix1[CCM_MATRIX_SIZE];
    DNG_SRATIONAL_S astColorMatrix2[CCM_MATRIX_SIZE];
    DNG_SRATIONAL_S astCameraCalibration1[CCM_MATRIX_SIZE];
    DNG_SRATIONAL_S astCameraCalibration2[CCM_MATRIX_SIZE];
    DNG_SRATIONAL_S astForwadMatrix1[CCM_MATRIX_SIZE];
    DNG_SRATIONAL_S astForwadMatrix2[CCM_MATRIX_SIZE];

    XMEDIA_U8  u8CalibrationIlluminant1;
    XMEDIA_U8  u8CalibrationIlluminant2;
} DNG_IMAGE_STATIC_INFO_S;

typedef struct ISP_DNG_WBGAIN_S {
    XMEDIA_U16 u16Rgain;
    XMEDIA_U16 u16Ggain;
    XMEDIA_U16 u16Bgain;
} ISP_DNG_WBGAIN_S;

typedef struct ISP_DNG_COLORPARAM_S {
    ISP_DNG_WBGAIN_S stWbGain1;
    ISP_DNG_WBGAIN_S stWbGain2;
} ISP_DNG_COLORPARAM_S;


typedef enum ISP_IR_STATUS_E {
    ISP_IR_STATUS_NORMAL = 0,
    ISP_IR_STATUS_IR     = 1,
    ISP_IR_BUTT
} ISP_IR_STATUS_E;

typedef enum ISP_IR_SWITCH_STATUS_E {
    ISP_IR_SWITCH_NONE      = 0,
    ISP_IR_SWITCH_TO_NORMAL = 1,
    ISP_IR_SWITCH_TO_IR     = 2,
    ISP_IR_SWITCH_BUTT
} ISP_IR_SWITCH_STATUS_E;


typedef struct ISP_IR_AUTO_ATTR_S {
    XMEDIA_BOOL bEnable;
    XMEDIA_U32  u32Normal2IrIsoThr;
    XMEDIA_U32  u32Ir2NormalIsoThr;
    XMEDIA_U32  u32RGMax;
    XMEDIA_U32  u32RGMin;
    XMEDIA_U32  u32BGMax;
    XMEDIA_U32  u32BGMin;

    ISP_IR_STATUS_E enIrStatus;

    ISP_IR_SWITCH_STATUS_E enIrSwitch;
} ISP_IR_AUTO_ATTR_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
