/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
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
    GK_BOOL bUpdate;
    GK_U16  au16BlackLevel[ISP_BAYER_CHN_NUM];
} ISP_CMOS_BLACK_LEVEL_S;

typedef struct ISP_SNS_ATTR_INFO_S {
    SENSOR_ID            eSensorId;
} ISP_SNS_ATTR_INFO_S;

#define ISP_SPLIT_POINT_NUM    (5)
#define ISP_EXPANDER_POINT_NUM (5)

typedef struct ISP_CMOS_DEMOSAIC_S {
    GK_BOOL bEnable;
    GK_U8   au8NonDirStr[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8   au8NonDirMFDetailEhcStr[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8   au8NonDirHFDetailEhcStr[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8   au8DetailSmoothRange[ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_CMOS_DEMOSAIC_S;

#define WDR_MAX_FRAME          (4)

typedef struct ISP_CMOS_BAYERNR_S {
    GK_BOOL  bEnable;
    GK_BOOL  bBnrMonoSensorEn;
    GK_BOOL  bNrLscEnable;
    GK_U8    u8BnrLscMaxGain;
    GK_U16   u16BnrLscCmpStrength;

    GK_U8    au8LutFineStr[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8    au8ChromaStr[ISP_BAYER_CHN_NUM][ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8    au8WDRFrameStr[WDR_MAX_FRAME_NUM];
    GK_U8    au8FusionFrameStr[WDR_MAX_FRAME_NUM];
    GK_U16   au16CoarseStr[ISP_BAYER_CHN_NUM][ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U16   au16LutCoringWgt[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U16   au16LutCoringRatio[ISP_BAYERNR_LUT_LENGTH];
} ISP_CMOS_BAYERNR_S;

typedef struct ISP_CMOS_NOISE_CALIBRATION_S {
    GK_U16   u16CalibrationLutNum;
    GK_FLOAT afCalibrationCoef[BAYER_CALIBTAION_MAX_NUM][3];
} ISP_CMOS_NOISE_CALIBRATION_S;

typedef struct ISP_CMOS_DETAIL_S {
    ISP_DE_ATTR_S stAttr;
} ISP_CMOS_DETAIL_S;

typedef struct ISP_CMOS_SHARPEN_MANUAL_S { //
    GK_U8  au8LumaWgt[ISP_SHARPEN_LUMA_NUM];     /* RW; Range: GK7608V200 =[0,127]|GK7606V100=[0,127]| V100 = [0, 127]| V200 =[0, 31]; Format:0.5;Adjust the sharpen strength according to luma. Sharpen strength will be weaker when it decrease. */
    GK_U16 au16TextureStr[ISP_SHARPEN_GAIN_NUM];        /* RW; Range: [0, 4095]; Format:7.5;Undirectional sharpen strength for texture and detail enhancement */
    GK_U16 au16EdgeStr[ISP_SHARPEN_GAIN_NUM];        /* RW; Range: [0, 4095]; Format:7.5;Directional sharpen strength for edge enhancement */
    GK_U16 u16TextureFreq;         /* RW; Range: [0, 4095];Format:6.6; Texture frequency adjustment. Texture and detail will be finer when it increase */
    GK_U16 u16EdgeFreq;            /* RW; Range: [0, 4095];Format:6.6; Edge frequency adjustment. Edge will be narrower and thiner when it increase */
    GK_U8  u8OverShoot;            /* RW; Range: [0, 127]; Format:7.0;u8OvershootAmt */
    GK_U8  u8UnderShoot;           /* RW; Range: [0, 127]; Format:7.0;u8UndershootAmt */
    GK_U8  u8ShootSupStr;          /* RW; Range: [0, 255]; Format:8.0;overshoot and undershoot suppression strength, the amplitude and width of shoot will be decrease when shootSupSt increase */
    GK_U8  u8ShootSupAdj;          /* RW; Range: [0, 15]; Format:4.0;overshoot and undershoot suppression adjusting, adjust the edge shoot suppression strength */
    GK_U8  u8DetailCtrl;           /* RW; Range: [0, 255]; Format:8.0;Different sharpen strength for detail and edge. When it is bigger than 128, detail sharpen strength will be stronger than edge. */
    GK_U8  u8DetailCtrlThr;        /* RW; Range: [0, 255]; Format:8.0; The threshold of DetailCtrl, it is used to distinguish detail and edge. */
    GK_U8  u8EdgeFiltStr;          /* RW; Range: [0, 63]; Format:6.0;The strength of edge filtering. */
    GK_U8  u8EdgeFiltMaxCap;       /* RW; Range: [0, 47]; Format:6.0;The max capacity of edge filtering.*/
    GK_U8  u8RGain;                /* RW; Range: [0, 31];   Format:5.0;Sharpen Gain for Red Area */
    GK_U8  u8GGain;                /* RW; Range: [0, 255]; Format:8.0;Sharpen Gain for Green Area */
    GK_U8  u8BGain;                /* RW; Range: [0, 31];   Format:5.0;Sharpen Gain for Blue Area */
    GK_U8  u8SkinGain;             /* RW; Range: [0, 31]; Format:5.0;Sharpen Gain for Skin Area */
    GK_U16 u16MaxSharpGain;        /* RW; Range: [0, 0x7FF]; Format:8.3; Maximum sharpen gain */
    GK_U8  u8WeakDetailGain;       /* RW; Range: [0, 127]; Format:7.0;Only support for GK7205V200; sharpen Gain for weakdetail */
} ISP_CMOS_SHARPEN_MANUAL_S;


typedef struct ISP_CMOS_SHARPEN_AUTO_S { //
    GK_U8  au8LumaWgt[ISP_SHARPEN_LUMA_NUM][ISP_AUTO_ISO_STRENGTH_NUM];      /* RW; Range:GK7608V200 =[0,127]|GK7606V100=[0,127]|V100 = [0, 127]|V200 =[0, 31];; Format:0.5;Adjust the sharpen strength according to luma. Sharpen strength will be weaker when it decrease. */
    GK_U16 au16TextureStr[ISP_SHARPEN_GAIN_NUM][ISP_AUTO_ISO_STRENGTH_NUM];     /* RW; Range: [0, 4095]; Format:7.5;Undirectional sharpen strength for texture and detail enhancement */
    GK_U16 au16EdgeStr[ISP_SHARPEN_GAIN_NUM][ISP_AUTO_ISO_STRENGTH_NUM];        /* RW; Range: [0, 4095]; Format:7.5;Directional sharpen strength for edge enhancement */
    GK_U16 au16TextureFreq[ISP_AUTO_ISO_STRENGTH_NUM];         /* RW; Range: [0, 4095]; Format:6.6;Texture frequency adjustment. Texture and detail will be finer when it increase */
    GK_U16 au16EdgeFreq[ISP_AUTO_ISO_STRENGTH_NUM];            /* RW; Range: [0, 4095]; Format:6.6;Edge frequency adjustment. Edge will be narrower and thiner when it increase */
    GK_U8  au8OverShoot[ISP_AUTO_ISO_STRENGTH_NUM];            /* RW; Range: [0, 127];  Format:7.0;u8OvershootAmt */
    GK_U8  au8UnderShoot[ISP_AUTO_ISO_STRENGTH_NUM];           /* RW; Range: [0, 127];  Format:7.0;u8UndershootAmt */
    GK_U8  au8ShootSupStr[ISP_AUTO_ISO_STRENGTH_NUM];          /* RW; Range: [0, 255]; Format:8.0;overshoot and undershoot suppression strength, the amplitude and width of shoot will be decrease when shootSupSt increase */
    GK_U8  au8ShootSupAdj[ISP_AUTO_ISO_STRENGTH_NUM];          /* RW; Range: [0, 15]; Format:4.0;overshoot and undershoot suppression adjusting, adjust the edge shoot suppression strength */
    GK_U8  au8DetailCtrl[ISP_AUTO_ISO_STRENGTH_NUM];           /* RW; Range: [0, 255]; Format:8.0;Different sharpen strength for detail and edge. When it is bigger than 128, detail sharpen strength will be stronger than edge. */
    GK_U8  au8DetailCtrlThr[ISP_AUTO_ISO_STRENGTH_NUM];        /* RW; Range: [0, 255]; Format:8.0; The threshold of DetailCtrl, it is used to distinguish detail and edge. */
    GK_U8  au8EdgeFiltStr[ISP_AUTO_ISO_STRENGTH_NUM];          /* RW; Range: [0, 63]; Format:6.0;The strength of edge filtering. */
    GK_U8  au8EdgeFiltMaxCap[ISP_AUTO_ISO_STRENGTH_NUM];        /* RW; Range: [0, 47]; Format:6.0;The max capacity of edge filtering.*/
    GK_U8  au8RGain[ISP_AUTO_ISO_STRENGTH_NUM];                /* RW; Range: [0, 31];   Format:5.0;Sharpen Gain for Red Area */
    GK_U8  au8GGain[ISP_AUTO_ISO_STRENGTH_NUM];                /* RW; Range: [0, 255]; Format:8.0;Sharpen Gain for Green Area */
    GK_U8  au8BGain[ISP_AUTO_ISO_STRENGTH_NUM];                /* RW; Range: [0, 31];   Format:5.0;Sharpen Gain for Blue Area */
    GK_U8  au8SkinGain[ISP_AUTO_ISO_STRENGTH_NUM];             /* RW; Range: [0, 31]; Format:5.0; */
    GK_U16 au16MaxSharpGain[ISP_AUTO_ISO_STRENGTH_NUM];        /* RW; Range: [0, 0x7FF]; Format:8.3; */
    GK_U8  au8WeakDetailGain[ISP_AUTO_ISO_STRENGTH_NUM];       /* RW; Range: [0, 127]; Format:7.0;Only support for GK7205V200; sharpen Gain for weakdetail */
} ISP_CMOS_SHARPEN_AUTO_S;


typedef struct ISP_CMOS_SHARPEN_S {
    GK_U8 u8SkinUmin;
    GK_U8 u8SkinVmin;
    GK_U8 u8SkinUmax;
    GK_U8 u8SkinVmax;
    ISP_CMOS_SHARPEN_MANUAL_S stManual;
    ISP_CMOS_SHARPEN_AUTO_S   stAuto;
} ISP_CMOS_SHARPEN_S;


typedef struct ISP_CMOS_DRC_S {
    GK_BOOL bEnable;
    ISP_OP_TYPE_E enOpType;

    GK_U16  u16ManualStrength;
    GK_U16  u16AutoStrength;

    GK_U8   u8SpatialFltCoef;
    GK_U8   u8RangeFltCoef;
    GK_U8   u8ContrastControl;
    GK_S8   s8DetailAdjustFactor;
    GK_U8   u8RangeAdaMax;

    GK_U8   u8FltScaleFine;
    GK_U8   u8FltScaleCoarse;
    GK_U8   u8GradRevMax;
    GK_U8   u8GradRevThr;

    GK_U8   u8BrightGainLmt;
    GK_U8   u8BrightGainLmtStep;
    GK_U8   u8DarkGainLmtY;
    GK_U8   u8DarkGainLmtC;
    GK_U8   u8PDStrength;
    GK_U8   u8LocalMixingBrightMax;
    GK_U8   u8LocalMixingBrightMin;
    GK_U8   u8LocalMixingBrightThr;
    GK_S8   s8LocalMixingBrightSlo;
    GK_U8   u8LocalMixingDarkMax;
    GK_U8   u8LocalMixingDarkMin;
    GK_U8   u8LocalMixingDarkThr;
    GK_S8   s8LocalMixingDarkSlo;
    GK_U16  u16ColorCorrectionLut[33];
    GK_U16  u16ToneMappingValue[200];

    GK_U8   u8Asymmetry;
    GK_U8   u8SecondPole;
    GK_U8   u8Stretch;
    GK_U8   u8Compress;

    GK_U8   u8CurveSel;

    GK_U16 au16Xpoint[5];
    GK_U16 au16Ypoint[5];
    GK_U16 au16Slope[5];
} ISP_CMOS_DRC_S;

typedef struct ISP_CMOS_WDR_S {
    GK_BOOL  bFusionMode;
    GK_BOOL  bMotionComp;

    GK_U16   u16ShortThr;
    GK_U16   u16LongThr;

    GK_BOOL  bForceLong;
    GK_U16   u16ForceLongLowThr;
    GK_U16   u16ForceLongHigThr;
    GK_BOOL  bShortExpoChk;
    GK_U16   u16ShortCheckThd;

    GK_U8    au8MdThrLowGain[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8    au8MdThrHigGain[ISP_AUTO_ISO_STRENGTH_NUM];

    GK_U16   au16FusionThr[WDR_MAX_FRAME];
    GK_U8    u8MdtStillThd;
    GK_U8    u8MdtLongBlend;
} ISP_CMOS_WDR_S;

typedef struct ISP_CMOS_DEHAZE_S {
    GK_BOOL  bEnable;           /* RW;Range:[0,1];Format:1.0; */
    GK_BOOL  bUserLutEnable;    /* RW;Range:[0,1];0:Auto Lut 1:User Lut */

    ISP_OP_TYPE_E      enOpType;
    GK_U8    u8AutoStrength;
    GK_U8    u8ManualStrength;
} ISP_CMOS_DEHAZE_S;

typedef struct ISP_CMOS_PREGAMMA_S {
    GK_BOOL bEnable;
    GK_U32  au32PreGamma[PREGAMMA_NODE_NUM];
} ISP_CMOS_PREGAMMA_S;

#define GAMMA_NODE_NUMBER      257      // Update NODE NUMBER
#define GAMMA_EXT_NODE_NUMBER  1025  // Update NODE NUMBER
#define GAMMA_OUTSEG_NUMBER    8        // Update NODE NUMBER
typedef struct ISP_CMOS_GAMMA_S {
    GK_U16  au16Gamma[GAMMA_EXT_NODE_NUMBER];
} ISP_CMOS_GAMMA_S;

#define RGBIR_CVTMATRIX_NUMBER 12
typedef struct ISP_CMOS_RGBIR_S {
    GK_BOOL  bEnable;
    GK_S16  as16CvtMatrix[RGBIR_CVTMATRIX_NUMBER];
} ISP_CMOS_RGBIR_S;

typedef struct ISP_CMOS_SENSOR_MAX_RESOLUTION_S {
    GK_U32  u32MaxWidth;
    GK_U32  u32MaxHeight;
} ISP_CMOS_SENSOR_MAX_RESOLUTION_S;

typedef struct ISP_CMOS_DPC_S {
    GK_U16  au16Strength[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U16  au16BlendRatio[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8   u8DpcPos;    /* DPC module position. Linear Mode Range: [0, 1]; WDR Mode Range: [0]. 
                            0 for after drc, 1 for before wdr. Default:0 */
} ISP_CMOS_DPC_S;

typedef struct ISP_LSC_CABLI_TABLE_S {
    GK_U16 au16R_Gain[ISP_LSC_GRID_POINTS];
    GK_U16 au16Gr_Gain[ISP_LSC_GRID_POINTS];
    GK_U16 au16Gb_Gain[ISP_LSC_GRID_POINTS];
    GK_U16 au16B_Gain[ISP_LSC_GRID_POINTS];
} ISP_LSC_CABLI_TABLE_S;

typedef struct ISP_CMOS_LSC_S {
    GK_U16  u16MeshStrength;
    GK_U8   u8MeshScale;
    ISP_LSC_CABLI_TABLE_S astLscCalibTable[2];
} ISP_CMOS_LSC_S;

typedef struct ISP_CMOS_LCAC_MANUAL_S
{
    GK_U8 u8DePurpleCrStr;
	GK_U8 u8DePurpleCbStr;
}ISP_CMOS_LCAC_MANUAL_S;

typedef struct ISP_CMOS_LCAC_AUTO_S
{
    GK_U8 au8DePurpleCrStr[ISP_EXP_RATIO_STRENGTH_NUM];
	GK_U8 au8DePurpleCbStr[ISP_EXP_RATIO_STRENGTH_NUM];          
}ISP_CMOS_LCAC_AUTO_S;

typedef struct ISP_CMOS_LCAC_S {
    GK_BOOL bEnable;
	GK_U16  u16PurpleDetRange;
	GK_U16  VarThr;
    GK_U16  au16RLumaThd[ISP_LCAC_DET_NUM];
    GK_U16  au16GLumaThd[ISP_LCAC_DET_NUM];
    GK_U16  au16BLumaThd[ISP_LCAC_DET_NUM];
    GK_U16  au16YLumaThd[ISP_LCAC_DET_NUM];
    GK_S16  as16CbCrRatio[ISP_LCAC_DET_NUM];	
	ISP_OP_TYPE_E enOpMode;
	ISP_CMOS_LCAC_MANUAL_S stManual;
	ISP_CMOS_LCAC_AUTO_S   stAuto;
} ISP_CMOS_LCAC_S;

typedef struct ISP_CMOS_CA_S {
    GK_BOOL   bEnable;
    GK_U16    au16YRatioLut[ISP_CA_YRATIO_LUT_LENGTH];  // 1.10bit  Y Ratio For UV ; Max = 2047 FW Limit
    GK_S16    as16ISORatio[ISP_AUTO_ISO_STRENGTH_NUM];     // 1.10bit  ISO Ratio  For UV,Max = 2047 FW Limi
} ISP_CMOS_CA_S;

typedef struct ISP_CMOS_EXPANDER_POINT_S {
    GK_U16 u16X;                    /* RW;Range:[0x0,0x81];Format:8.0;The X point of the knee */
    GK_U32 u32Y;                    /* RW;Range:[0x0,0x8000];Format:16.0;The Y point of the  knee */
} ISP_CMOS_EXPANDER_POINT_S;

typedef struct ISP_CMOS_EXPANDER_S {
    GK_BOOL bEnable;
    GK_U8   u8BitDepthIn;            /* RW;Range:[0xC,0x14];Format:5.0;The Bit depth of input */
    GK_U8   u8BitDepthOut;           /* RW;Range:[0xC,0x14];Format:5.0;The Bit depth of output */
    ISP_CMOS_EXPANDER_POINT_S astExpanderPoint[ISP_EXPANDER_POINT_NUM];
} ISP_CMOS_EXPANDER_S;

typedef struct ISP_CMOS_GE_S {
    GK_BOOL bEnable;                                 /* RW,Range: [   0, 1]      */
    GK_U8  u8Slope;                                  /* RW,Range: [   0, 0xE]    */
    GK_U8  u8SensiSlope;                             /* RW,Range: [   0, 0xE]    */
    GK_U16 u16SensiThr;                              /* RW,Range: [   0, 0x3FFF] */
    GK_U16 au16Threshold[ISP_AUTO_ISO_STRENGTH_NUM]; /* RW,Range: [   0, 0x3FFF] */
    GK_U16 au16Strength[ISP_AUTO_ISO_STRENGTH_NUM];  /* RW,Range: [   0, 0x100]  */
    GK_U16 au16NpOffset[ISP_AUTO_ISO_STRENGTH_NUM];  /* RW,Range: [0x200, 0x3FFF] */
} ISP_CMOS_GE_S;

typedef struct ISP_CMOS_ANTIFALSECOLOR_S {
    GK_BOOL bEnable;                                                     /* RW;Range:[0x0,0x1];Format:1.0; AntiFalseColor Enable */
    GK_U8   au8AntiFalseColorThreshold[ISP_AUTO_ISO_STRENGTH_NUM];       /* RW;Range:[0x0,0x20];Format:6.0;Threshold for antifalsecolor */
    GK_U8   au8AntiFalseColorStrength[ISP_AUTO_ISO_STRENGTH_NUM];        /* RW;Range:[0x0,0x1F];Format:5.0;Strength of antifalsecolor */
} ISP_CMOS_ANTIFALSECOLOR_S;

typedef struct ISP_CMOS_LDCI_S {
    GK_BOOL  bEnable;
    GK_U8    u8GaussLPFSigma;
    GK_U8    au8HePosWgt[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8    au8HePosSigma[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8    au8HePosMean[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8    au8HeNegWgt[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8    au8HeNegSigma[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U8    au8HeNegMean[ISP_AUTO_ISO_STRENGTH_NUM];
    GK_U16   au16BlcCtrl[ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_CMOS_LDCI_S;

typedef struct ISP_CMOS_SENSOR_MODE_S {
    GK_U32  u32SensorID;
    GK_U8   u8SensorMode;
    GK_BOOL bValidDngRawFormat;
    DNG_RAW_FORMAT_S stDngRawFormat;
} ISP_CMOS_SENSOR_MODE_S;

typedef struct ISP_CMOS_DNG_COLORPARAM_S {
    ISP_DNG_WBGAIN_S stWbGain1; /* the calibration White balance gain of colorcheker in A Light */
    ISP_DNG_WBGAIN_S stWbGain2; /* the calibration White balance gain of colorcheker in D50 Light */
} ISP_CMOS_DNG_COLORPARAM_S;

typedef struct ISP_CMOS_WDR_SWITCH_ATTR_S {
    GK_U32   au32ExpRatio[EXP_RATIO_NUM];
} ISP_CMOS_WDR_SWITCH_ATTR_S;

typedef union ISP_CMOS_ALG_KEY_U {
    GK_U64  u64Key;
    struct {
        GK_U64  bit1Drc             : 1 ;   /* [0] */
        GK_U64  bit1Demosaic        : 1 ;   /* [1] */
        GK_U64  bit1PreGamma        : 1 ;   /* [2] */
        GK_U64  bit1Gamma           : 1 ;   /* [3] */
        GK_U64  bit1Sharpen         : 1 ;   /* [4] */
        GK_U64  bit1Ldci            : 1 ;   /* [5] */
        GK_U64  bit1Dpc             : 1 ;   /* [6] */
        GK_U64  bit1Lsc             : 1 ;   /* [7] */
        GK_U64  bit1Ge              : 1 ;   /* [8] */
        GK_U64  bit1AntiFalseColor  : 1 ;   /* [9] */
        GK_U64  bit1BayerNr         : 1 ;   /* [10] */
        GK_U64  bit1Ca              : 1 ;   /* [11] */
        GK_U64  bit1Expander        : 1 ;   /* [12] */
        GK_U64  bit1Wdr             : 1 ;   /* [13] */
        GK_U64  bit1Dehaze          : 1 ;   /* [14] */
        GK_U64  bit1Rgbir           : 1 ;   /* [15] */
		GK_U64  bit1Lcac            : 1 ;   /* [16] */
        GK_U64  bit44Rsv            : 47;   /* [17:63] */
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
    GK_U16   u16Width;
    GK_U16   u16Height;
    GK_FLOAT f32Fps;
    GK_U8    u8SnsMode;
} ISP_CMOS_SENSOR_IMAGE_MODE_S;

typedef struct ISP_SENSOR_EXP_FUNC_S {
    GK_VOID (*pfn_cmos_sensor_init)(VI_PIPE ViPipe);
    GK_VOID (*pfn_cmos_sensor_exit)(VI_PIPE ViPipe);
    GK_VOID (*pfn_cmos_sensor_global_init)(VI_PIPE ViPipe);
    GK_S32 (*pfn_cmos_set_image_mode)(VI_PIPE ViPipe, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode);
    GK_S32 (*pfn_cmos_set_wdr_mode)(VI_PIPE ViPipe, GK_U8 u8Mode);

    /* the algs get data which is associated with sensor, except 3a */
    GK_S32 (*pfn_cmos_get_isp_default)(VI_PIPE ViPipe, ISP_CMOS_DEFAULT_S *pstDef);
    GK_S32 (*pfn_cmos_get_isp_black_level)(VI_PIPE ViPipe, ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel);
    GK_S32 (*pfn_cmos_get_sns_reg_info)(VI_PIPE ViPipe, ISP_SNS_REGS_INFO_S *pstSnsRegsInfo);

    /* the function of sensor set pixel detect */
    GK_VOID (*pfn_cmos_set_pixel_detect)(VI_PIPE ViPipe, GK_BOOL bEnable);
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
