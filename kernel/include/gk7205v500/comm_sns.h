/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __ISP_COMM_SNS_H__
#define __ISP_COMM_SNS_H__

#include "type.h"
#include "common.h"
#include "comm_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct ISP_CMOS_BLACK_LEVEL_S {
    XMEDIA_BOOL bUpdate;
    XMEDIA_U16  au16BlackLevel[ISP_BAYER_CHN_NUM];
} ISP_CMOS_BLACK_LEVEL_S;

typedef struct ISP_SNS_ATTR_INFO_S {
    SENSOR_ID            eSensorId;
} ISP_SNS_ATTR_INFO_S;

#define ISP_SPLIT_POINT_NUM    (5)
#define ISP_EXPANDER_POINT_NUM (5)

typedef struct ISP_CMOS_DEMOSAIC_S {
    XMEDIA_BOOL bEnable;
    XMEDIA_U8   au8NonDirStr[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8   au8NonDirMFDetailEhcStr[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8   au8NonDirHFDetailEhcStr[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8   au8DetailSmoothRange[ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_CMOS_DEMOSAIC_S;

#define WDR_MAX_FRAME          (4)

typedef struct ISP_CMOS_BAYERNR_S {
    XMEDIA_BOOL  bEnable;
    XMEDIA_BOOL  bBnrMonoSensorEn;
    XMEDIA_BOOL  bNrLscEnable;
    XMEDIA_U8    u8BnrLscMaxGain;
    XMEDIA_U16   u16BnrLscCmpStrength;

    XMEDIA_U8    au8LutFineStr[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8    au8ChromaStr[ISP_BAYER_CHN_NUM][ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8    au8WDRFrameStr[WDR_MAX_FRAME_NUM];
    XMEDIA_U8    au8FusionFrameStr[WDR_MAX_FRAME_NUM];
    XMEDIA_U16   au16CoarseStr[ISP_BAYER_CHN_NUM][ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U16   au16LutCoringWgt[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U16   au16LutCoringRatio[ISP_BAYERNR_LUT_LENGTH];
} ISP_CMOS_BAYERNR_S;

typedef struct ISP_CMOS_NOISE_CALIBRATION_S {
    XMEDIA_U16   u16CalibrationLutNum;
    XMEDIA_FLOAT afCalibrationCoef[BAYER_CALIBTAION_MAX_NUM][3];
} ISP_CMOS_NOISE_CALIBRATION_S;

typedef struct ISP_CMOS_DETAIL_S {
    ISP_DE_ATTR_S stAttr;
} ISP_CMOS_DETAIL_S;

typedef struct ISP_CMOS_SHARPEN_MANUAL_S {
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
    XMEDIA_U16 u16MaxSharpGain;
    XMEDIA_U8  u8WeakDetailGain;
} ISP_CMOS_SHARPEN_MANUAL_S;


typedef struct ISP_CMOS_SHARPEN_AUTO_S {
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
    XMEDIA_U16 au16MaxSharpGain[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8  au8WeakDetailGain[ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_CMOS_SHARPEN_AUTO_S;


typedef struct ISP_CMOS_SHARPEN_S {
    XMEDIA_U8 u8SkinUmin;
    XMEDIA_U8 u8SkinVmin;
    XMEDIA_U8 u8SkinUmax;
    XMEDIA_U8 u8SkinVmax;
    ISP_CMOS_SHARPEN_MANUAL_S stManual;
    ISP_CMOS_SHARPEN_AUTO_S   stAuto;
} ISP_CMOS_SHARPEN_S;


typedef struct ISP_CMOS_DRC_S {
    XMEDIA_BOOL bEnable;
    ISP_OP_TYPE_E enOpType;

    XMEDIA_U16  u16ManualStrength;
    XMEDIA_U16  u16AutoStrength;

    XMEDIA_U8   u8SpatialFltCoef;
    XMEDIA_U8   u8RangeFltCoef;
    XMEDIA_U8   u8ContrastControl;
    XMEDIA_S8   s8DetailAdjustFactor;
    XMEDIA_U8   u8RangeAdaMax;

    XMEDIA_U8   u8FltScaleFine;
    XMEDIA_U8   u8FltScaleCoarse;
    XMEDIA_U8   u8GradRevMax;
    XMEDIA_U8   u8GradRevThr;

    XMEDIA_U8   u8BrightGainLmt;
    XMEDIA_U8   u8BrightGainLmtStep;
    XMEDIA_U8   u8DarkGainLmtY;
    XMEDIA_U8   u8DarkGainLmtC;
    XMEDIA_U8   u8PDStrength;
    XMEDIA_U8   u8LocalMixingBrightMax;
    XMEDIA_U8   u8LocalMixingBrightMin;
    XMEDIA_U8   u8LocalMixingBrightThr;
    XMEDIA_S8   s8LocalMixingBrightSlo;
    XMEDIA_U8   u8LocalMixingDarkMax;
    XMEDIA_U8   u8LocalMixingDarkMin;
    XMEDIA_U8   u8LocalMixingDarkThr;
    XMEDIA_S8   s8LocalMixingDarkSlo;
    XMEDIA_U16  u16ColorCorrectionLut[33];
    XMEDIA_U16  u16ToneMappingValue[200];

    XMEDIA_U8   u8Asymmetry;
    XMEDIA_U8   u8SecondPole;
    XMEDIA_U8   u8Stretch;
    XMEDIA_U8   u8Compress;

    XMEDIA_U8   u8CurveSel;

    XMEDIA_U16 au16Xpoint[5];
    XMEDIA_U16 au16Ypoint[5];
    XMEDIA_U16 au16Slope[5];
} ISP_CMOS_DRC_S;

typedef struct ISP_CMOS_WDR_S {
    XMEDIA_BOOL  bFusionMode;
    XMEDIA_BOOL  bMotionComp;

    XMEDIA_U16   u16ShortThr;
    XMEDIA_U16   u16LongThr;

    XMEDIA_BOOL  bForceLong;
    XMEDIA_U16   u16ForceLongLowThr;
    XMEDIA_U16   u16ForceLongHigThr;
    XMEDIA_BOOL  bShortExpoChk;
    XMEDIA_U16   u16ShortCheckThd;

    XMEDIA_U8    au8MdThrLowGain[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8    au8MdThrHigGain[ISP_AUTO_ISO_STRENGTH_NUM];

    XMEDIA_U16   au16FusionThr[WDR_MAX_FRAME];
    XMEDIA_U8    u8MdtStillThd;
    XMEDIA_U8    u8MdtLongBlend;
} ISP_CMOS_WDR_S;

typedef struct ISP_CMOS_DEHAZE_S {
    XMEDIA_BOOL  bEnable;
    XMEDIA_BOOL  bUserLutEnable;

    ISP_OP_TYPE_E      enOpType;
    XMEDIA_U8    u8AutoStrength;
    XMEDIA_U8    u8ManualStrength;
} ISP_CMOS_DEHAZE_S;

typedef struct ISP_CMOS_PREGAMMA_S {
    XMEDIA_BOOL bEnable;
    XMEDIA_U32  au32PreGamma[PREGAMMA_NODE_NUM];
} ISP_CMOS_PREGAMMA_S;

#define GAMMA_NODE_NUMBER      257
#define GAMMA_EXT_NODE_NUMBER  1025
#define GAMMA_OUTSEG_NUMBER    8
typedef struct ISP_CMOS_GAMMA_S {
    XMEDIA_U16  au16Gamma[GAMMA_EXT_NODE_NUMBER];
} ISP_CMOS_GAMMA_S;

#define RGBIR_CVTMATRIX_NUMBER 12
typedef struct ISP_CMOS_RGBIR_S {
    XMEDIA_BOOL  bEnable;
    XMEDIA_S16  as16CvtMatrix[RGBIR_CVTMATRIX_NUMBER];
} ISP_CMOS_RGBIR_S;

typedef struct ISP_CMOS_SENSOR_MAX_RESOLUTION_S {
    XMEDIA_U32  u32MaxWidth;
    XMEDIA_U32  u32MaxHeight;
} ISP_CMOS_SENSOR_MAX_RESOLUTION_S;

typedef struct ISP_CMOS_DPC_S {
    XMEDIA_U16  au16Strength[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U16  au16BlendRatio[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8   u8DpcPos;
} ISP_CMOS_DPC_S;

typedef struct ISP_LSC_CABLI_TABLE_S {
    XMEDIA_U16 au16R_Gain[ISP_LSC_GRID_POINTS];
    XMEDIA_U16 au16Gr_Gain[ISP_LSC_GRID_POINTS];
    XMEDIA_U16 au16Gb_Gain[ISP_LSC_GRID_POINTS];
    XMEDIA_U16 au16B_Gain[ISP_LSC_GRID_POINTS];
} ISP_LSC_CABLI_TABLE_S;

typedef struct ISP_CMOS_LSC_S {
    XMEDIA_U16  u16MeshStrength;
    XMEDIA_U8   u8MeshScale;
    ISP_LSC_CABLI_TABLE_S astLscCalibTable[2];
} ISP_CMOS_LSC_S;

typedef struct ISP_CMOS_LCAC_MANUAL_S
{
    XMEDIA_U8 u8DePurpleCrStr;
	XMEDIA_U8 u8DePurpleCbStr;
}ISP_CMOS_LCAC_MANUAL_S;

typedef struct ISP_CMOS_LCAC_AUTO_S
{
    XMEDIA_U8 au8DePurpleCrStr[ISP_EXP_RATIO_STRENGTH_NUM];
	XMEDIA_U8 au8DePurpleCbStr[ISP_EXP_RATIO_STRENGTH_NUM];
}ISP_CMOS_LCAC_AUTO_S;

typedef struct ISP_CMOS_LCAC_S {
    XMEDIA_BOOL bEnable;
	XMEDIA_U16  u16PurpleDetRange;
	XMEDIA_U16  VarThr;
    XMEDIA_U16  au16RLumaThd[ISP_LCAC_DET_NUM];
    XMEDIA_U16  au16GLumaThd[ISP_LCAC_DET_NUM];
    XMEDIA_U16  au16BLumaThd[ISP_LCAC_DET_NUM];
    XMEDIA_U16  au16YLumaThd[ISP_LCAC_DET_NUM];
    XMEDIA_S16  as16CbCrRatio[ISP_LCAC_DET_NUM];
	ISP_OP_TYPE_E enOpMode;
	ISP_CMOS_LCAC_MANUAL_S stManual;
	ISP_CMOS_LCAC_AUTO_S   stAuto;
} ISP_CMOS_LCAC_S;

typedef struct ISP_CMOS_CA_S {
    XMEDIA_BOOL   bEnable;
    XMEDIA_U16    au16YRatioLut[ISP_CA_YRATIO_LUT_LENGTH];
    XMEDIA_S16    as16ISORatio[ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_CMOS_CA_S;

typedef struct ISP_CMOS_EXPANDER_POINT_S {
    XMEDIA_U16 u16X;
    XMEDIA_U32 u32Y;
} ISP_CMOS_EXPANDER_POINT_S;

typedef struct ISP_CMOS_EXPANDER_S {
    XMEDIA_BOOL bEnable;
    XMEDIA_U8   u8BitDepthIn;
    XMEDIA_U8   u8BitDepthOut;
    ISP_CMOS_EXPANDER_POINT_S astExpanderPoint[ISP_EXPANDER_POINT_NUM];
} ISP_CMOS_EXPANDER_S;

typedef struct ISP_CMOS_GE_S {
    XMEDIA_BOOL bEnable;
    XMEDIA_U8  u8Slope;
    XMEDIA_U8  u8SensiSlope;
    XMEDIA_U16 u16SensiThr;
    XMEDIA_U16 au16Threshold[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U16 au16Strength[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U16 au16NpOffset[ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_CMOS_GE_S;

typedef struct ISP_CMOS_ANTIFALSECOLOR_S {
    XMEDIA_BOOL bEnable;
    XMEDIA_U8   au8AntiFalseColorThreshold[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8   au8AntiFalseColorStrength[ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_CMOS_ANTIFALSECOLOR_S;

typedef struct ISP_CMOS_LDCI_S {
    XMEDIA_BOOL  bEnable;
    XMEDIA_U8    u8GaussLPFSigma;
    XMEDIA_U8    au8HePosWgt[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8    au8HePosSigma[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8    au8HePosMean[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8    au8HeNegWgt[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8    au8HeNegSigma[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U8    au8HeNegMean[ISP_AUTO_ISO_STRENGTH_NUM];
    XMEDIA_U16   au16BlcCtrl[ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_CMOS_LDCI_S;

typedef struct ISP_CMOS_SENSOR_MODE_S {
    XMEDIA_U32  u32SensorID;
    XMEDIA_U8   u8SensorMode;
    XMEDIA_BOOL bValidDngRawFormat;
    DNG_RAW_FORMAT_S stDngRawFormat;
} ISP_CMOS_SENSOR_MODE_S;

typedef struct ISP_CMOS_DNG_COLORPARAM_S {
    ISP_DNG_WBGAIN_S stWbGain1;
    ISP_DNG_WBGAIN_S stWbGain2;
} ISP_CMOS_DNG_COLORPARAM_S;

typedef struct ISP_CMOS_WDR_SWITCH_ATTR_S {
    XMEDIA_U32   au32ExpRatio[EXP_RATIO_NUM];
} ISP_CMOS_WDR_SWITCH_ATTR_S;

typedef union ISP_CMOS_ALG_KEY_U {
    XMEDIA_U64  u64Key;
    struct {
        XMEDIA_U64  bit1Drc             : 1 ;
        XMEDIA_U64  bit1Demosaic        : 1 ;
        XMEDIA_U64  bit1PreGamma        : 1 ;
        XMEDIA_U64  bit1Gamma           : 1 ;
        XMEDIA_U64  bit1Sharpen         : 1 ;
        XMEDIA_U64  bit1Ldci            : 1 ;
        XMEDIA_U64  bit1Dpc             : 1 ;
        XMEDIA_U64  bit1Lsc             : 1 ;
        XMEDIA_U64  bit1Ge              : 1 ;
        XMEDIA_U64  bit1AntiFalseColor  : 1 ;
        XMEDIA_U64  bit1BayerNr         : 1 ;
        XMEDIA_U64  bit1Ca              : 1 ;
        XMEDIA_U64  bit1Expander        : 1 ;
        XMEDIA_U64  bit1Wdr             : 1 ;
        XMEDIA_U64  bit1Dehaze          : 1 ;
        XMEDIA_U64  bit1Rgbir           : 1 ;
		XMEDIA_U64  bit1Lcac            : 1 ;
        XMEDIA_U64  bit44Rsv            : 47;
    };
} ISP_CMOS_ALG_KEY_U;

typedef struct ISP_CMOS_DEFAULT_S {
    ISP_CMOS_ALG_KEY_U               unKey;
    const ISP_CMOS_DRC_S             *pstDrc;
    const ISP_CMOS_DEMOSAIC_S        *pstDemosaic;
    const ISP_CMOS_PREGAMMA_S        *pstPreGamma;
    const ISP_CMOS_GAMMA_S           *pstGamma;
    const ISP_CMOS_SHARPEN_S         *pstSharpen;
    const ISP_CMOS_LDCI_S            *pstLdci;
    const ISP_CMOS_DPC_S             *pstDpc;
    const ISP_CMOS_LSC_S             *pstLsc;
    const ISP_CMOS_GE_S              *pstGe;
    const ISP_CMOS_ANTIFALSECOLOR_S  *pstAntiFalseColor;
    const ISP_CMOS_BAYERNR_S         *pstBayerNr;
    const ISP_CMOS_CA_S              *pstCa;
    const ISP_CMOS_EXPANDER_S        *pstExpander;
    const ISP_CMOS_RGBIR_S           *pstRgbir;
    const ISP_CMOS_WDR_S             *pstWdr;
    const ISP_CMOS_DEHAZE_S          *pstDehaze;
    const ISP_CMOS_LCAC_S            *pstLcac;
    ISP_CMOS_NOISE_CALIBRATION_S     stNoiseCalibration;
    ISP_CMOS_SENSOR_MAX_RESOLUTION_S stSensorMaxResolution;
    ISP_CMOS_SENSOR_MODE_S           stSensorMode;
    ISP_CMOS_DNG_COLORPARAM_S        stDngColorParam;
    ISP_CMOS_WDR_SWITCH_ATTR_S       stWdrSwitchAttr;
} ISP_CMOS_DEFAULT_S;

typedef struct ISP_CMOS_SENSOR_IMAGE_MODE_S {
    XMEDIA_U16   u16Width;
    XMEDIA_U16   u16Height;
    XMEDIA_FLOAT f32Fps;
    XMEDIA_U8    u8SnsMode;
} ISP_CMOS_SENSOR_IMAGE_MODE_S;

typedef struct ISP_SENSOR_EXP_FUNC_S {
    XMEDIA_VOID (*pfn_cmos_sensor_init)(VI_PIPE ViPipe);
    XMEDIA_VOID (*pfn_cmos_sensor_exit)(VI_PIPE ViPipe);
    XMEDIA_VOID (*pfn_cmos_sensor_global_init)(VI_PIPE ViPipe);
    XMEDIA_S32 (*pfn_cmos_set_image_mode)(VI_PIPE ViPipe, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode);
    XMEDIA_S32 (*pfn_cmos_set_wdr_mode)(VI_PIPE ViPipe, XMEDIA_U8 u8Mode);

    XMEDIA_S32 (*pfn_cmos_get_isp_default)(VI_PIPE ViPipe, ISP_CMOS_DEFAULT_S *pstDef);
    XMEDIA_S32 (*pfn_cmos_get_isp_black_level)(VI_PIPE ViPipe, ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel);
    XMEDIA_S32 (*pfn_cmos_get_sns_reg_info)(VI_PIPE ViPipe, ISP_SNS_REGS_INFO_S *pstSnsRegsInfo);

    XMEDIA_VOID (*pfn_cmos_set_pixel_detect)(VI_PIPE ViPipe, XMEDIA_BOOL bEnable);
} ISP_SENSOR_EXP_FUNC_S;

typedef struct ISP_SENSOR_REGISTER_S {
    ISP_SENSOR_EXP_FUNC_S stSnsExp;
} ISP_SENSOR_REGISTER_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
