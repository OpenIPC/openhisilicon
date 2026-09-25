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
#endif /* End of #ifdef __cplusplus */

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

typedef struct ISP_CMOS_SHARPEN_MANUAL_S { //
    XMEDIA_U8  au8LumaWgt[ISP_SHARPEN_LUMA_NUM];     /* RW; Range: XM76080200 =[0,127]|XM76060100=[0,127]| V100 = [0, 127]| V200 =[0, 31]; Format:0.5;Adjust the sharpen strength according to luma. Sharpen strength will be weaker when it decrease. */
    XMEDIA_U16 au16TextureStr[ISP_SHARPEN_GAIN_NUM];        /* RW; Range: [0, 4095]; Format:7.5;Undirectional sharpen strength for texture and detail enhancement */
    XMEDIA_U16 au16EdgeStr[ISP_SHARPEN_GAIN_NUM];        /* RW; Range: [0, 4095]; Format:7.5;Directional sharpen strength for edge enhancement */
    XMEDIA_U16 u16TextureFreq;         /* RW; Range: [0, 4095];Format:6.6; Texture frequency adjustment. Texture and detail will be finer when it increase */
    XMEDIA_U16 u16EdgeFreq;            /* RW; Range: [0, 4095];Format:6.6; Edge frequency adjustment. Edge will be narrower and thiner when it increase */
    XMEDIA_U8  u8OverShoot;            /* RW; Range: [0, 127]; Format:7.0;u8OvershootAmt */
    XMEDIA_U8  u8UnderShoot;           /* RW; Range: [0, 127]; Format:7.0;u8UndershootAmt */
    XMEDIA_U8  u8ShootSupStr;          /* RW; Range: [0, 255]; Format:8.0;overshoot and undershoot suppression strength, the amplitude and width of shoot will be decrease when shootSupSt increase */
    XMEDIA_U8  u8ShootSupAdj;          /* RW; Range: [0, 15]; Format:4.0;overshoot and undershoot suppression adjusting, adjust the edge shoot suppression strength */
    XMEDIA_U8  u8DetailCtrl;           /* RW; Range: [0, 255]; Format:8.0;Different sharpen strength for detail and edge. When it is bigger than 128, detail sharpen strength will be stronger than edge. */
    XMEDIA_U8  u8DetailCtrlThr;        /* RW; Range: [0, 255]; Format:8.0; The threshold of DetailCtrl, it is used to distinguish detail and edge. */
    XMEDIA_U8  u8EdgeFiltStr;          /* RW; Range: [0, 63]; Format:6.0;The strength of edge filtering. */
    XMEDIA_U8  u8EdgeFiltMaxCap;       /* RW; Range: [0, 47]; Format:6.0;The max capacity of edge filtering.*/
    XMEDIA_U8  u8RGain;                /* RW; Range: [0, 31];   Format:5.0;Sharpen Gain for Red Area */
    XMEDIA_U8  u8GGain;                /* RW; Range: [0, 255]; Format:8.0;Sharpen Gain for Green Area */
    XMEDIA_U8  u8BGain;                /* RW; Range: [0, 31];   Format:5.0;Sharpen Gain for Blue Area */
    XMEDIA_U8  u8SkinGain;             /* RW; Range: [0, 31]; Format:5.0;Sharpen Gain for Skin Area */
    XMEDIA_U16 u16MaxSharpGain;        /* RW; Range: [0, 0x7FF]; Format:8.3; Maximum sharpen gain */
    XMEDIA_U8  u8WeakDetailGain;       /* RW; Range: [0, 127]; Format:7.0;Only support for XM72050200; sharpen Gain for weakdetail */
} ISP_CMOS_SHARPEN_MANUAL_S;


typedef struct ISP_CMOS_SHARPEN_AUTO_S { //
    XMEDIA_U8  au8LumaWgt[ISP_SHARPEN_LUMA_NUM][ISP_AUTO_ISO_STRENGTH_NUM];      /* RW; Range:XM76080200 =[0,127]|XM76060100=[0,127]|V100 = [0, 127]|V200 =[0, 31];; Format:0.5;Adjust the sharpen strength according to luma. Sharpen strength will be weaker when it decrease. */
    XMEDIA_U16 au16TextureStr[ISP_SHARPEN_GAIN_NUM][ISP_AUTO_ISO_STRENGTH_NUM];     /* RW; Range: [0, 4095]; Format:7.5;Undirectional sharpen strength for texture and detail enhancement */
    XMEDIA_U16 au16EdgeStr[ISP_SHARPEN_GAIN_NUM][ISP_AUTO_ISO_STRENGTH_NUM];        /* RW; Range: [0, 4095]; Format:7.5;Directional sharpen strength for edge enhancement */
    XMEDIA_U16 au16TextureFreq[ISP_AUTO_ISO_STRENGTH_NUM];         /* RW; Range: [0, 4095]; Format:6.6;Texture frequency adjustment. Texture and detail will be finer when it increase */
    XMEDIA_U16 au16EdgeFreq[ISP_AUTO_ISO_STRENGTH_NUM];            /* RW; Range: [0, 4095]; Format:6.6;Edge frequency adjustment. Edge will be narrower and thiner when it increase */
    XMEDIA_U8  au8OverShoot[ISP_AUTO_ISO_STRENGTH_NUM];            /* RW; Range: [0, 127];  Format:7.0;u8OvershootAmt */
    XMEDIA_U8  au8UnderShoot[ISP_AUTO_ISO_STRENGTH_NUM];           /* RW; Range: [0, 127];  Format:7.0;u8UndershootAmt */
    XMEDIA_U8  au8ShootSupStr[ISP_AUTO_ISO_STRENGTH_NUM];          /* RW; Range: [0, 255]; Format:8.0;overshoot and undershoot suppression strength, the amplitude and width of shoot will be decrease when shootSupSt increase */
    XMEDIA_U8  au8ShootSupAdj[ISP_AUTO_ISO_STRENGTH_NUM];          /* RW; Range: [0, 15]; Format:4.0;overshoot and undershoot suppression adjusting, adjust the edge shoot suppression strength */
    XMEDIA_U8  au8DetailCtrl[ISP_AUTO_ISO_STRENGTH_NUM];           /* RW; Range: [0, 255]; Format:8.0;Different sharpen strength for detail and edge. When it is bigger than 128, detail sharpen strength will be stronger than edge. */
    XMEDIA_U8  au8DetailCtrlThr[ISP_AUTO_ISO_STRENGTH_NUM];        /* RW; Range: [0, 255]; Format:8.0; The threshold of DetailCtrl, it is used to distinguish detail and edge. */
    XMEDIA_U8  au8EdgeFiltStr[ISP_AUTO_ISO_STRENGTH_NUM];          /* RW; Range: [0, 63]; Format:6.0;The strength of edge filtering. */
    XMEDIA_U8  au8EdgeFiltMaxCap[ISP_AUTO_ISO_STRENGTH_NUM];        /* RW; Range: [0, 47]; Format:6.0;The max capacity of edge filtering.*/
    XMEDIA_U8  au8RGain[ISP_AUTO_ISO_STRENGTH_NUM];                /* RW; Range: [0, 31];   Format:5.0;Sharpen Gain for Red Area */
    XMEDIA_U8  au8GGain[ISP_AUTO_ISO_STRENGTH_NUM];                /* RW; Range: [0, 255]; Format:8.0;Sharpen Gain for Green Area */
    XMEDIA_U8  au8BGain[ISP_AUTO_ISO_STRENGTH_NUM];                /* RW; Range: [0, 31];   Format:5.0;Sharpen Gain for Blue Area */
    XMEDIA_U8  au8SkinGain[ISP_AUTO_ISO_STRENGTH_NUM];             /* RW; Range: [0, 31]; Format:5.0; */
    XMEDIA_U16 au16MaxSharpGain[ISP_AUTO_ISO_STRENGTH_NUM];        /* RW; Range: [0, 0x7FF]; Format:8.3; */
    XMEDIA_U8  au8WeakDetailGain[ISP_AUTO_ISO_STRENGTH_NUM];       /* RW; Range: [0, 127]; Format:7.0;Only support for XM72050200; sharpen Gain for weakdetail */
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
    XMEDIA_BOOL  bEnable;           /* RW;Range:[0,1];Format:1.0; */
    XMEDIA_BOOL  bUserLutEnable;    /* RW;Range:[0,1];0:Auto Lut 1:User Lut */

    ISP_OP_TYPE_E      enOpType;
    XMEDIA_U8    u8AutoStrength;
    XMEDIA_U8    u8ManualStrength;
} ISP_CMOS_DEHAZE_S;

typedef struct ISP_CMOS_PREGAMMA_S {
    XMEDIA_BOOL bEnable;
    XMEDIA_U32  au32PreGamma[PREGAMMA_NODE_NUM];
} ISP_CMOS_PREGAMMA_S;

#define GAMMA_NODE_NUMBER      257      // Update NODE NUMBER
#define GAMMA_EXT_NODE_NUMBER  1025  // Update NODE NUMBER
#define GAMMA_OUTSEG_NUMBER    8        // Update NODE NUMBER
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
    XMEDIA_U8   u8DpcPos;    /* DPC module position. Linear Mode Range: [0, 1]; WDR Mode Range: [0]. 
                            0 for after drc, 1 for before wdr. Default:0 */
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
    XMEDIA_U16    au16YRatioLut[ISP_CA_YRATIO_LUT_LENGTH];  // 1.10bit  Y Ratio For UV ; Max = 2047 FW Limit
    XMEDIA_S16    as16ISORatio[ISP_AUTO_ISO_STRENGTH_NUM];     // 1.10bit  ISO Ratio  For UV,Max = 2047 FW Limi
} ISP_CMOS_CA_S;

typedef struct ISP_CMOS_EXPANDER_POINT_S {
    XMEDIA_U16 u16X;                    /* RW;Range:[0x0,0x81];Format:8.0;The X point of the knee */
    XMEDIA_U32 u32Y;                    /* RW;Range:[0x0,0x8000];Format:16.0;The Y point of the  knee */
} ISP_CMOS_EXPANDER_POINT_S;

typedef struct ISP_CMOS_EXPANDER_S {
    XMEDIA_BOOL bEnable;
    XMEDIA_U8   u8BitDepthIn;            /* RW;Range:[0xC,0x14];Format:5.0;The Bit depth of input */
    XMEDIA_U8   u8BitDepthOut;           /* RW;Range:[0xC,0x14];Format:5.0;The Bit depth of output */
    ISP_CMOS_EXPANDER_POINT_S astExpanderPoint[ISP_EXPANDER_POINT_NUM];
} ISP_CMOS_EXPANDER_S;

typedef struct ISP_CMOS_GE_S {
    XMEDIA_BOOL bEnable;                                 /* RW,Range: [   0, 1]      */
    XMEDIA_U8  u8Slope;                                  /* RW,Range: [   0, 0xE]    */
    XMEDIA_U8  u8SensiSlope;                             /* RW,Range: [   0, 0xE]    */
    XMEDIA_U16 u16SensiThr;                              /* RW,Range: [   0, 0x3FFF] */
    XMEDIA_U16 au16Threshold[ISP_AUTO_ISO_STRENGTH_NUM]; /* RW,Range: [   0, 0x3FFF] */
    XMEDIA_U16 au16Strength[ISP_AUTO_ISO_STRENGTH_NUM];  /* RW,Range: [   0, 0x100]  */
    XMEDIA_U16 au16NpOffset[ISP_AUTO_ISO_STRENGTH_NUM];  /* RW,Range: [0x200, 0x3FFF] */
} ISP_CMOS_GE_S;

typedef struct ISP_CMOS_ANTIFALSECOLOR_S {
    XMEDIA_BOOL bEnable;                                                     /* RW;Range:[0x0,0x1];Format:1.0; AntiFalseColor Enable */
    XMEDIA_U8   au8AntiFalseColorThreshold[ISP_AUTO_ISO_STRENGTH_NUM];       /* RW;Range:[0x0,0x20];Format:6.0;Threshold for antifalsecolor */
    XMEDIA_U8   au8AntiFalseColorStrength[ISP_AUTO_ISO_STRENGTH_NUM];        /* RW;Range:[0x0,0x1F];Format:5.0;Strength of antifalsecolor */
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
    ISP_DNG_WBGAIN_S stWbGain1; /* the calibration White balance gain of colorcheker in A Light */
    ISP_DNG_WBGAIN_S stWbGain2; /* the calibration White balance gain of colorcheker in D50 Light */
} ISP_CMOS_DNG_COLORPARAM_S;

typedef struct ISP_CMOS_WDR_SWITCH_ATTR_S {
    XMEDIA_U32   au32ExpRatio[EXP_RATIO_NUM];
} ISP_CMOS_WDR_SWITCH_ATTR_S;

typedef union ISP_CMOS_ALG_KEY_U {
    XMEDIA_U64  u64Key;
    struct {
        XMEDIA_U64  bit1Drc             : 1 ;   /* [0] */
        XMEDIA_U64  bit1Demosaic        : 1 ;   /* [1] */
        XMEDIA_U64  bit1PreGamma        : 1 ;   /* [2] */
        XMEDIA_U64  bit1Gamma           : 1 ;   /* [3] */
        XMEDIA_U64  bit1Sharpen         : 1 ;   /* [4] */
        XMEDIA_U64  bit1Ldci            : 1 ;   /* [5] */
        XMEDIA_U64  bit1Dpc             : 1 ;   /* [6] */
        XMEDIA_U64  bit1Lsc             : 1 ;   /* [7] */
        XMEDIA_U64  bit1Ge              : 1 ;   /* [8] */
        XMEDIA_U64  bit1AntiFalseColor  : 1 ;   /* [9] */
        XMEDIA_U64  bit1BayerNr         : 1 ;   /* [10] */
        XMEDIA_U64  bit1Ca              : 1 ;   /* [11] */
        XMEDIA_U64  bit1Expander        : 1 ;   /* [12] */
        XMEDIA_U64  bit1Wdr             : 1 ;   /* [13] */
        XMEDIA_U64  bit1Dehaze          : 1 ;   /* [14] */
        XMEDIA_U64  bit1Rgbir           : 1 ;   /* [15] */
		XMEDIA_U64  bit1Lcac            : 1 ;   /* [16] */
        XMEDIA_U64  bit44Rsv            : 47;   /* [17:63] */
    };
} ISP_CMOS_ALG_KEY_U;

typedef struct ISP_CMOS_DEFAULT_S {
    ISP_CMOS_ALG_KEY_U               unKey;
    const ISP_CMOS_DRC_S             *pstDrc;
    const ISP_CMOS_DEMOSAIC_S        *pstDemosaic;
    const ISP_CMOS_PREGAMMA_S        *pstPreGamma;
    const ISP_CMOS_GAMMA_S           *pstGamma;
    const ISP_CMOS_SHARPEN_S         *pstSharpen;
    // const ISP_CMOS_EDGEMARK_S        *pstEdgeMark;
    // const ISP_CMOS_HLC_S             *pstHlc;
    const ISP_CMOS_LDCI_S            *pstLdci;
    const ISP_CMOS_DPC_S             *pstDpc;
    const ISP_CMOS_LSC_S             *pstLsc;
    const ISP_CMOS_GE_S              *pstGe;
    const ISP_CMOS_ANTIFALSECOLOR_S  *pstAntiFalseColor;
    const ISP_CMOS_BAYERNR_S         *pstBayerNr;
    // const ISP_CMOS_DETAIL_S          *pstDetail;
    const ISP_CMOS_CA_S              *pstCa;
    const ISP_CMOS_EXPANDER_S        *pstExpander;
    const ISP_CMOS_RGBIR_S           *pstRgbir;
    // const ISP_CMOS_CLUT_S            *pstClut;
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

    /* the algs get data which is associated with sensor, except 3a */
    XMEDIA_S32 (*pfn_cmos_get_isp_default)(VI_PIPE ViPipe, ISP_CMOS_DEFAULT_S *pstDef);
    XMEDIA_S32 (*pfn_cmos_get_isp_black_level)(VI_PIPE ViPipe, ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel);
    XMEDIA_S32 (*pfn_cmos_get_sns_reg_info)(VI_PIPE ViPipe, ISP_SNS_REGS_INFO_S *pstSnsRegsInfo);

    /* the function of sensor set pixel detect */
    XMEDIA_VOID (*pfn_cmos_set_pixel_detect)(VI_PIPE ViPipe, XMEDIA_BOOL bEnable);
} ISP_SENSOR_EXP_FUNC_S;

typedef struct ISP_SENSOR_REGISTER_S {
    ISP_SENSOR_EXP_FUNC_S stSnsExp;
} ISP_SENSOR_REGISTER_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_COMM_SNS_H__ */
