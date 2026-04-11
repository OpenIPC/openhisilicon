/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mkp_isp.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/12/19
  Description   : 
  History       :
  1.Date        : 2012/12/19
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#ifndef __MKP_ISP_H__
#define __MKP_ISP_H__

#include "hi_type.h"
#include "hi_comm_3a.h"

#define IOC_TYPE_ISP   'I'

#define MAX_ISP_STAT_BUF_NUM    2

typedef enum hiIOC_NR_ISP_E
{
    IOC_NR_ISP_SET_FD = 0,
    IOC_NR_ISP_GET_FRAME_EDGE,
    IOC_NR_ISP_GET_VD_TIMEOUT,
    IOC_NR_ISP_GET_VD_END_TIMEOUT,
    IOC_NR_ISP_SET_REGNEWER,
    IOC_NR_ISP_SET_INT_ENABLE,
    IOC_NR_ISP_GET_VERSION,

    IOC_NR_ISP_STAT_BUF_INIT,
    IOC_NR_ISP_STAT_BUF_EXIT,
    IOC_NR_ISP_STAT_BUF_GET,
    IOC_NR_ISP_STAT_BUF_PUT,
    IOC_NR_ISP_STAT_ACT_GET,
    IOC_NR_ISP_STAT_SHADOW_MEMPHY_GET,
    IOC_NR_ISP_STAT_SHADOW_MEMSTATE_SET,
    IOC_NR_ISP_REG_CFG_INIT,
    IOC_NR_ISP_REG_CFG_SET,
    IOC_NR_ISP_PROC_INIT,
    IOC_NR_ISP_PROC_WRITE_ING,
    IOC_NR_ISP_PROC_WRITE_OK,
    IOC_NR_ISP_PROC_EXIT,
    IOC_NR_ISP_PROC_PARAM_GET,
    IOC_NR_ISP_SYNC_CFG_SET,
    IOC_NR_WDR_SYNC_CFG_SET,
    IOC_NR_ISP_UPDATEINFO_BUF_INIT,
    IOC_NR_ISP_UPDATEINFO_BUF_EXIT,
    IOC_NR_ACM_ATTR_GET,
    IOC_NR_ACM_ATTR_SET,
    IOC_NR_ACM_LUT_PHY_ADDR_GET,
    IOC_NR_ACM_COEF_SET,
    IOC_NR_ISP_MEM_INFO_SET,
    IOC_NR_ISP_MEM_INFO_GET,
    IOC_NR_ISP_PWM_NUM_GET,
    IOC_NR_ISP_RES_SWITCH_SET,
    IOC_NR_ISP_SET_MOD_PARAM,
    IOC_NR_ISP_GET_MOD_PARAM,

    IOC_NR_ISP_BUTT,
} IOC_NR_ISP_E;

typedef struct hiISP_VD_TIMEOUT_S
{
   ISP_VD_INFO_S stVdInfo;
   HI_U32  u32MilliSec;     /*RW, Range: [0x0, 0xFFFFFFFF], the number of the */
   HI_S32  s32IntStatus;    /*RO, when there is ISP interrupt, then the s32IntStatus is 1 */
}ISP_VD_TIMEOUT_S;

typedef struct hiISP_VERSION_S
{
    HI_U8 au8MppVersion[4];
    HI_U8 au8Date[20];
    HI_U8 au8Time[20];
    HI_U32 u32Magic;
} ISP_VERSION_S;

/* the statistics of isp */
typedef struct hiISP_DP_STAT_S
{
    HI_U16  u16DefectPixelCount;
} ISP_DP_STAT_S;

typedef struct hiISP_DEHAZE_STAT_S
{
    //statics for dehaze run ,lsw
    //HI_U32 au32DehazeMinStatDout[256];
    //HI_U32 au32DehazeMaxStatDout[1024];
    //HI_U32 au32Hist[32];
    HI_U32  au32DehazeMinDout[DEFOG_ZONE_NUM/2];
} ISP_DEHAZE_STAT_S;

typedef struct hiISP_MG_STAT_S
{    
    HI_U16  au16ZoneAvg[MG_ZONE_ROW][MG_ZONE_COLUMN][4];    
} ISP_MG_STAT_S;


typedef struct hiISP_COMM_STAT_S
{
    HI_U16  au16WhiteBalanceGain[4];    
} ISP_COMM_STAT_S;

typedef struct hiISP_STAT_S
{
    ISP_AE_STAT_1_S     stAeStat1;   /* not support */  
    ISP_AE_STAT_2_S     stAeStat2;   /* not support */ 
    ISP_AE_STAT_3_S     stAeStat3;
    ISP_AE_STAT_4_S     stAeStat4;
    ISP_AE_STAT_5_S     stAeStat5;
    ISP_AE_STAT_6_S     stAeStat6;
    ISP_AWB_STAT_1_S    stAwbStat1;    
    ISP_AWB_STAT_2_S    stAwbStat2;
    ISP_AWB_STAT_3_S    stAwbStat3;    
    ISP_AWB_STAT_4_S    stAwbStat4;
    ISP_AF_STAT_S       stAfStat;
    ISP_DP_STAT_S       stDpStat;
    ISP_COMM_STAT_S     stCommStat;
    ISP_DEHAZE_STAT_S   stDehazeStat;
	ISP_MG_STAT_S 		stMgStat;
} ISP_STAT_S;

typedef union hiISP_STAT_KEY_U
{
    HI_U32  u32Key;
    struct
    {
        HI_U32  bit1AeStat1     : 1 ;   /* [0] not support */
        HI_U32  bit1AeStat2     : 1 ;   /* [1] not support */
        HI_U32  bit1AeStat3     : 1 ;   /* [2] */
        HI_U32  bit1AeStat4     : 1 ;   /* [3] */
        HI_U32  bit1AeStat5     : 1 ;   /* [4] */
        HI_U32  bit1AeStat6     : 1 ;   /* [5] */
        HI_U32  bit1AwbStat1    : 1 ;   /* [6] */
        HI_U32  bit1AwbStat2    : 1 ;   /* [7] */
        HI_U32  bit1AwbStat3    : 1 ;   /* [8] */
        HI_U32  bit1AwbStat4    : 1 ;   /* [9] */
        HI_U32  bit1AfStat      : 1 ;   /* [10] */
        HI_U32  bit1DpStat      : 1 ;   /* [11] */
        HI_U32  bit1CommStat    : 1 ;   /* [12] */
        HI_U32  bit1Dehaze      : 1 ;   /* [13] */
		HI_U32  bit1MgStat		: 1 ;   /* [14] */
        HI_U32  bit1Rsv  		: 1 ;   /* [15] */
        HI_U32  bit16IsrAccess  : 16;   /* [16:31] */
    };
}ISP_STAT_KEY_U;

typedef struct hiISP_STAT_INFO_S
{
    ISP_STAT_KEY_U  unKey;    
    HI_U32  u32PhyAddr;
    HI_VOID *pVirtAddr;                 /* (ISP_STAT_S *) */
} ISP_STAT_INFO_S;


typedef struct hiISP_STAT_SHADOW_MEM_S
{
    HI_U32  u32PhyAddr;
    HI_VOID *pVirtAddr; 
    HI_U32 u32Size;
    HI_BOOL bUsrAccess;
} ISP_STAT_SHADOW_MEM_S;


/* the register config of isp */
typedef struct hiISP_AE_REG_CFG_1_S
{
    HI_U8   au8MeteringHistThresh[4];
    HI_U8   au8WeightTable[15][17];
} ISP_AE_REG_CFG_1_S;

typedef struct hiISP_AE_REG_CFG_2_S
{
    HI_U64  u64Exposure;
    HI_U32  u32IntTime[4];  /* 0: Shortest shutter(SS), 1: Short shutter(S), 2: long shutter(L), 3: longest shutter(LL) */
    HI_U32  u32IspDgain;

    HI_BOOL bPirisValid;
    HI_S32  s32PirisPos;
    ISP_FSWDR_MODE_E enFSWDRMode;
	HI_U32  u32WDRGain[4];
} ISP_AE_REG_CFG_2_S;

typedef struct hiISP_AWB_REG_CFG_1_S
{
    HI_U32  au32WhiteBalanceGain[4];
    HI_U16  au16ColorMatrix[9];
	AWB_DARKREGION_PROT_S stProDarkRegion;
} ISP_AWB_REG_CFG_1_S;

typedef struct hiISP_AWB_REG_CFG_2_S
{
    HI_U16  u16MeteringWhiteLevelAwb;
    HI_U16  u16MeteringBlackLevelAwb;
    HI_U16  u16MeteringCrRefMaxAwb;
    HI_U16  u16MeteringCbRefMaxAwb;
    HI_U16  u16MeteringCrRefMinAwb;
    HI_U16  u16MeteringCbRefMinAwb;
    HI_U16  u16MeteringCrRefHighAwb;
    HI_U16  u16MeteringCrRefLowAwb;
    HI_U16  u16MeteringCbRefHighAwb;
    HI_U16  u16MeteringCbRefLowAwb;
} ISP_AWB_REG_CFG_2_S;

typedef struct hiISP_AWB_REG_CFG_3_S
{
    HI_BOOL bAboveWhiteLevelClip;
    HI_BOOL bBelowBlackLevelClip;
    HI_U16  u16MeteringWhiteLevelAwb;
    HI_U16  u16MeteringBlackLevelAwb;
    HI_U16  u16MeteringCrRefMaxAwb;
    HI_U16  u16MeteringCbRefMaxAwb;
    HI_U16  u16MeteringCrRefMinAwb;
    HI_U16  u16MeteringCbRefMinAwb;
    HI_U16  u16MeteringCrRefHighAwb;
    HI_U16  u16MeteringCrRefLowAwb;
    HI_U16  u16MeteringCbRefHighAwb;
    HI_U16  u16MeteringCbRefLowAwb;
} ISP_AWB_REG_CFG_3_S;

typedef struct hiISP_AF_REG_CFG_S
{
    HI_U16  u16ThresholdWrite;
    HI_U8   u8MetricsShift;
    HI_U8   u8NpOffset;
} ISP_AF_REG_CFG_S;

typedef struct hiISP_OFFSET_REG_CFG_S
{
    HI_U16  au16BlackLevelOffset[4];
} ISP_OFFSET_REG_CFG_S;

typedef struct hiISP_DRC_REG_CFG_S
{
	HI_U8   u8MixingCoring;
	HI_U8   u8MixingDarkMin;
	HI_U8   u8MixingDarkMax;

	HI_U8   u8MixingBrightMin;
	HI_U8   u8MixingBrightMax;
	HI_U8   u8PdwHighStr;
	HI_U8   u8Enable;
	HI_U8   u8RangeVarFine;
    HI_U8   u8SpatialVarFine;
    HI_U8   u8RangeVarMedium;
    HI_U8   u8SpatialVarMedium;
    HI_U8   u8RangeVarCoarse;
    HI_U8   u8SpatialVarCoarse;
    HI_U8   u8FltScaleFine;
    HI_U8   u8FltScaleCoarse;
	HI_U32  FrameExposure;
} ISP_DRC_REG_CFG_S;

typedef struct hiISP_DEHAZE_REG_CFG_S
{
    HI_U8   u8Strength;
    HI_U8   u8DehazeEnable;
    HI_U16  u16AirR;
	HI_U16  u16AirG;
	HI_U16  u16AirB;
    HI_U32  u32Update;
} ISP_DEHAZE_REG_CFG_S;

/*
typedef struct hiISP_GAMMA_REG_CFG_S
{
    HI_U16  au16GammaLut[257];
    HI_U8  u8GammaBankSel;
} ISP_GAMMA_REG_CFG_S;
*/

typedef struct hiISP_DP_REG_CFG_S
{
	HI_U32 u32DpccInSoftRst;		//U1.0
	HI_U32 u32DpccregsBayerPat;   //U2.0
	HI_U32 u32DpccCfg;              //U1.0  
	HI_U32 u32DpccAlpha;		      //U32.0
	HI_U32 u32DpccMode;             //U3.0    
	HI_U32 u32DpccOutputMode;      //U4.0    
	HI_U32 u32DpccSetUse;          //U4.0    
	HI_U32 u32DpccMethodsSet1;    //U13.0    
	HI_U32 u32DpccMethodsSet2;    //U13.0    
	HI_U32 u32DpccMethodsSet3;    //U13.0    
	HI_U32 u32DpccLineThresh1;    //U16.0    
	HI_U32 u32DpccLineMadFac1;   //U16.0    
	HI_U32 u32DpccPgFac1;         //U16.0    
	HI_U32 u32DpccRndThresh1;     //U16.0    
	HI_U32 u32DpccRgFac1;         //U16.0    
	HI_U32 u32DpccLineThresh2;    //U16.0    
	HI_U32 u32DpccLineMadFac2;   //U16.0    
	HI_U32 u32DpccPgFac2;         //U16.0    
	HI_U32 u32DpccRndThresh2;     //U16.0   
	HI_U32 u32DpccRgFac2;         //U16.0    
	HI_U32 u32DpccLineThresh3;    //U16.0    
	HI_U32 u32DpccLineMadFac3;   //U16.0    
	HI_U32 u32DpccPgFac3;         //U16.0    
	HI_U32 u32DpccRndThresh3;     //U16.0   
	HI_U32 u32DpccRgFac3;         //U16.0    
	HI_U32 u32DpccRoLimits;        //U12.0    
	HI_U32 u32DpccRndOffs;         //U12.0    
	HI_U32 u32DpccBptCtrl;         //U12.0    
	HI_U32 u32DpccBadThresh;       //U32.0    
	HI_U32 u32DpccBptNumber;       //U12.0
	HI_U32 au32DpccBpTable[2048];
	HI_U32 u32DpccHardThrEnable;
	HI_S8  s8DpccSupTwinkleThrMax;
	HI_S8  s8DpccSupTwinkleThrMin;
	HI_U16 u16DpccRakeRatio;
} ISP_DP_REG_CFG_S;

typedef struct hiISP_NR_REG_CFG_S
{
	HI_U32	 rangeLowThres;	  // low  threshold for range -> blend			//10bit;u,10.0
	HI_U32	 rangeHigThres;	  // high threshold for range -> blend			//10bit;u,10.0
	
	HI_S32	 rangeGain;		  // luma adaptive								//13bit;s,12.0	[0, 4096]
	HI_U16	 rangeLumaLow; 	  // luma adaptive								//10bit;u,10.0
	HI_U16	 rangeLumaHig; 	  // luma adaptive								//10bit;u,10.0
	
	HI_S32	 rangeLowLUT[17];    // 10bit
	HI_S32	 rangeHigLUT[17];    // 10bit
	HI_U16	 blendLowLimit;	  // lower blend ratio	  0x000 => NR 100%		//9bit;u,9.0  [0, 0x100]
	HI_U16	 blendHigLimit;	  // higher blend ratio   0x100 => DM 100%		//9bit;u,9.0 
 
	HI_U8	 falseColorEnable;   // 1bit
	HI_U8	 falseColorShift;    // 8bit
	HI_U8	 falseColorGain;	   // 8bit
	HI_U8	 falseColorLimit;    // 8bit
	
	HI_U8    NrLutEnable;	   // 1bit Luma Noise Reduction Enable
	//HI_U8    lumaNREnable;
	HI_U8    NrLumaNrGain;      // 

    //HI_U8    u8NrNoDirEn;
	HI_U8    u8NrNoDirGain;

} ISP_NR_REG_CFG_S;

typedef struct hiISP_SHARPEN_REG_CFG_S
{
    HI_U8  G3[3][3];               /* mid-band template */
    HI_U8  u8PixSel;               /* switch for selecting local max and min value in overshoot/undershoot control */
    HI_U16 u16SharpAmt;            /* mid-band sharpen strength */
    HI_U16 u16EdgeAmt;                // high-band sharpen strength                             // 12-bit unsigned integer // U4.8
    HI_U16 u16UDHfGain;			//undirectional high band detail control  [0,1023]      // 10-bit unsigned integer        // U10.0
    HI_U16 u16SharpThd1;              // threshold for mid-band sharpen strength                // 10-bit unsigned integer // U10.0
    HI_U16 u16SharpThd2;              // threshold for mid-band sharpen strength                // 10-bit unsigned integer // U10.0
    HI_U16 u16SharpThd1MulCoef;       // convert 1/(sharpThd1-sharpThd2) to multiplication      // 12-bit unsigned integer // U12.0
    HI_U16 u16EdgeThd1;               // threshold for high-band sharpen strength               // 10-bit unsigned integer // U10.0
    HI_U16 u16EdgeThd2;               // threshold for high-band sharpen strength               // 10-bit unsigned integer // U10.0
    HI_U16 u16EdgeThd1MulCoef;        // convert 1/(edgeThd1-edgeThd2) to multiplication.       // 12-bit unsigned integer // u12.0
    HI_U8 u8LineThd1;               // threshold for line detection                           // 8-bit unsigned integer  // U8.0
    HI_U8 u8LineThd2;               // threshold for line detection                           // 8-bit unsigned integer  // U8.0
    HI_U8 u8OvershootAmt;           // overshoot amplitude                                    // 8-bit unsigned integer  // U8.0
    HI_U8 u8UndershootAmt;          // undershoot amplitude                                   // 8-bit unsigned integer  // U8.0

    HI_S8  s8ShtMaxShft;            // shift for local max pixel, [-128, 127]                 // 8-bit signed integer    // S8.0
    HI_S8  s8ShtMinShft;            // shift for local min pixel, [-128, 127]                 // 8-bit signed integer    // S8.0
	
	HI_BOOL  bEnJagCtrl;			// switch for jag control	// 1-bit unsigned integer  // U1.0
	HI_U16 u16VarJagThd1;				// threshold for jag control						   // 10-bit unsigned integer  // U10.0
	HI_U16 u16VarJagThd2;				// threshold for jag control						   // 10-bit unsigned integer  // U10.0
	HI_U16 u16EdgeJagAmt;			// attenuation of Edge amplitude for jag control		   // 12-bit unsigned integer  // U12.0
	HI_S16 s16EdgeJagMulCoef;		 // convert (EdgeJagAmt-edgeAmt)/(varJagThd2-varJagThd1) to multiplication. 	  // 13-bit signed integer // s13.0
	HI_U8 u8OshtJagAmt;		   // amplify overshoot amplitude for jag control			// 8-bit unsigned integer  // U8.0
	HI_S16 s16OshtJagMulCoef;		 // convert (OshtJagAmt-overshootAmt)/(varJagThd2-varJagThd1) to multiplication.	// 13-bit signed integer // s8.4
	HI_U8 u8UshtJagAmt;		   // amplify undershoot amplitude for jag control			 // 8-bit unsigned integer	// U8.0
    HI_S16   s16UshtJagMulCoef;        // convert (UshtJagAmt-undershootAmt)/(varJagThd2-varJagThd1) to multiplication.  // 13-bit signed integer // s8.4

    // control sharpen based on luma
      HI_BOOL   bEnLumaCtrlNoise;	// switch for using local luma to control noise threshold   // 1-bit unsigned integer  // U1.0
	// control sharpen based on luma
	HI_BOOL   bEnLumaCtrl;           // switch for using local luma to control sharpen strength   // 1-bit unsigned integer  // U1.0
	HI_U8 u8lumaWgt[ISP_YUV_SHPLUMA_NUM];            // weight of sharpen result, [0, 255]                        // 8-bit unsigned integer  // U8.0 
	HI_U8 u8lumNoSt;			// 3-bit unsigned integer  // U3.0

    // control shoot based on var
    HI_BOOL   bEnShtCtrlVarMod;      // switch for using local var to control shoot                // 1-bit unsigned integer  // U1.0
    HI_U16  au16OShtVarTh[2];          // local var threshold, [0, 1023]                             // 10-bit unsigned integer // U10.0 x 2
    HI_U8  u8OShtVarAmt;            // overshoot amplitude , [0, 255]                             // 8-bit unsigned integer  // U8.0
    HI_S16 s16OShtVarMul;            // convert (oShtVarAmt-overshootAmt)/(oShtVarTh[i+1]-oShtVarTh[i]) to multiplication.    // 12-bit signed integer // S8.4
    HI_U16  au16UShtVarTh[2];          // local var threshold, [0, 1023]                             // 10-bit unsigned integer // U10.0 x 2 
    HI_U8  u8UShtVarAmt;            // undershoot amplitude, [0, 255]                             // 8-bit unsigned integer  // U8.0
    HI_S16  s16UShtVarMul;            // convert (uShtVarAmt-undershootAmt)/(uShtVarTh[i+1]-uShtVarTh[i]) to multiplication.   // 12-bit signed integer // S8.4
     
    // MF coring
    HI_U8  u8Limit;                 // [0,31]      // 4-bit unsigned integer        // U5.0 
    HI_U8  u8dirRlyShft;				 // [0,6]      // 3-bit unsigned integer        // U3.0

    //Skin detection
	HI_BOOL bEnSkinCtrl 	;		// Switch of skin detection and control			// [0,1]      // 1-bit unsigned integer        // U1.0
	HI_U8  u8skin_cnt_th1  	;		// threshold of number of skin pixels			// [0,15]      // 4-bit unsigned integer        // U4.0
	HI_U8  u8skin_cnt_th2  	;		// threshold of number of skin pixels			// [0,15]      // 4-bit unsigned integer        // U4.0
	HI_U8  u8skinCntMulCoef	;        // convert 31/(skin_cnt_th2-skin_cnt_th1) to multiplication.       // 5-bit unsigned integer // u5.0
	HI_U16  u16skin_var_th1  	;		// threshold of skin variance			                          // 10-bit unsigned integer  // U10.0
	HI_U16  u16skin_var_th2  	;		 // threshold of skin variance                           // 10-bit unsigned integer  // U10.0
	HI_U8  u8skin_var_wgt1 	;		// threshold of skin weight based on variance			// [0,255]      // 8-bit unsigned integer        // U8.0
	HI_U8  u8skin_var_wgt2 	;		// threshold of skin weight based on variance			// [0,255]      // 8-bit unsigned integer        // U8.0
	HI_S16  s16skinVarWgtMulCoef;        // convert (skin_var_wgt2-skin_var_wgt1)/(skin_var_th2-skin_var_th1) to multiplication.  // 13-bit signed integer // s8.4

	HI_U8  u8skin_u_max;		// Max Threshold of U for Skin detection, [0, 255]                        // 8-bit unsigned integer 
	HI_U8  u8skin_u_min;		// Min Threshold of U for Skin detection, [0, 255]                        // 8-bit unsigned integer 
	HI_U8  u8skin_v_max;		// Max Threshold of V for Skin detection, [0, 255]                        // 8-bit unsigned integer 
	HI_U8  u8skin_v_min;		// Min Threshold of V for Skin detection, [0, 255]                        // 8-bit unsigned integer 
	HI_U8  u8skin_len;		//length of skin for Skin detection, [1,12]      // 4-bit unsigned integer        // U4.0

	HI_BOOL  bEnEdgSm;
	HI_U8    u8DiffMul;	
	HI_U8    u8DiffThd;
	HI_U8    u8EdgSmStr;

	HI_BOOL  bEnShtSup;				// switch for shoot suppression   // 1-bit unsigned integer  // U1.0
	HI_U8  u8OvShtVarMin ;               // Min var threshold for overshoot suppression                           // 8-bit unsigned integer  // U8.0
	HI_U8  u8OvShtVarMax ;               // Max var threshold for overshoot suppression                           // 8-bit unsigned integer  // U8.0
	HI_U8  u8OvShtWgtMin ;               // Min weight for overshoot suppression                           // 8-bit unsigned integer  // U8.0
	HI_U16 u16OvShtSupMul;					// convert (255 - oShtWgtMin)/(oShtVarMax - oShtVarMin) to multiplication.    // 12-bit unsigned integer // U8.4
	HI_U8  u8UnShtVarMin ;               // Min var threshold for undershoot suppression                           // 8-bit unsigned integer  // U8.0
	HI_U8  u8UnShtVarMax ;               // Max var threshold for undershoot suppression                           // 8-bit unsigned integer  // U8.0
	HI_U8  u8UnShtWgtMin ;               // Min weight for undershoot suppression                           // 8-bit unsigned integer  // U8.0
	HI_U16 u16UnShtSupMul;					// convert (255 - uShtWgtMin)/(uShtVarMax - uShtVarMin) to multiplication.    // 12-bit unsigned integer // U8.4

	HI_U8  u8UnShtDiffMax ;               // Max var difference threshold for undershoot suppression                           // 8-bit unsigned integer  // U8.0
	HI_U8  u8UnShtDiffMin ;               // Min var difference threshold for undershoot suppression                          // 8-bit unsigned integer  // U8.0
	HI_U8  u8UnShtDiffWgtMin;					// Min weight of var difference for undershoot suppression                          // 8-bit unsigned integer  // U8.0
	HI_U16 u16UnShtSupDiffMul;					// convert (uShtDiffWgtMin - 255)/(uShtDiffMax - uShtDiffMin) to multiplication.    // 13-bit signed integer // S8.4
	HI_U8  u8OvShtDiffMax ;               // Max var difference threshold for overshoot suppression                           // 8-bit unsigned integer  // U8.0
	HI_U8  u8OvShtDiffMin ;               // Min var difference threshold for overshoot suppression                          // 8-bit unsigned integer  // U8.0
	HI_U8  u8OvShtDiffWgtMin;					// Min weight of var difference for overshoot suppression                          // 8-bit unsigned integer  // U8.0
	HI_U16 u16OvShtSupDiffMul;					// convert (oShtDiffWgtMin - 255)/(oShtDiffMax - oShtDiffMin) to multiplication.    // 13-bit signed integer // S8.4
	HI_U8  u8MaxNoiseThd ;               // Max noise variance threshold for shoot suppression                           // 8-bit unsigned integer  // U8.0
	HI_U8  u8MinNoiseThd ;               // Min noise variance threshold for shoot suppression                          // 8-bit unsigned integer  // U8.0
	HI_U8  u8ShtSupBldr ;               // the blend ratio of min variance and difference                          // 4-bit unsigned integer  // U4.0

	HI_BOOL bEnShtMax;			// switch for shoot Max control   // 1-bit unsigned integer  // U1.0
	HI_U8    u8uShtMaxChg;		//[0, 255], U8.0	//Max change of original pixel for undershoot and overshoot
	HI_U8    u8oShtMaxChg;		//[0, 255], U8.0	//Max change of original pixel for undershoot and overshoot
	HI_U8    u8uShtMaxVarGain;	//[0, 255], U8.0	//the gain of Max change based on the max variance of original pixel for undershoot and overshoot
	HI_U8    u8oShtMaxVarGain;	//[0, 255], U8.0	//the gain of Max change based on the max variance of original pixel for undershoot and overshoot
	
	HI_BOOL bEnShadSup;
	HI_U8 u8gradThd1;
	HI_U8 u8gradThd2;
	HI_U8 u8shadWgt1;
	HI_U8 u8shadWgt2;	
	HI_U8 u8ShadWgtMul;
	HI_U16 u16shadVar1;
	HI_U16 u16shadVar2;

	HI_BOOL  bEnRgbShp		;			
	HI_U8    u8RcBmin   	;       
	HI_U8    u8RcBmax   	;
	HI_U8    u8RcRmin   	;       
	HI_U8    u8RcRmax   	;       
	HI_U8    u8RcRThd   	;
	HI_U8    u8rWgt     	;       
	HI_S8    s8rShpGainSft	;
	HI_U8    u8BcBmin		;       
	HI_U8    u8BcBmax		;       
	HI_U8    u8BcRmin		;
	HI_U8    u8BcRmax		;       
	HI_U8    u8BcBThd		;
	HI_U8    u8bWgt  		;       
	HI_S8    s8bShpGainSft	;       
	HI_U8    u8GcBmin		;
	HI_U8    u8GcBmax		;       
	HI_U8    u8GcRmin		;
	HI_U8    u8GcRmax		;       
	HI_U8    u8gWgt  		;   
	HI_S16  s16rShpGainMul;
	HI_S16  s16bShpGainMul;
	HI_BOOL  bEnLowBandEsm;

} ISP_SHARPEN_REG_CFG_S;

typedef struct hiISP_GE_REG_CFG_S
{    
    HI_BOOL ge_enable;
    HI_U16  ct_th1;         //u14.0  
    HI_U16  ge_th;          //u14.0
    HI_U8   ge_th_slop;    //u4.0
    HI_U16  detail_th;      //u14.0
    HI_U8   detail_slop;   //u4.0
    HI_U16 u16GeStrength;
} ISP_GE_REG_CFG_S;

typedef struct hiISP_MCDS_REG_CFG_S
{
	HI_BOOL bMidfEn;		//0: Med Filter close; 1: Med Filter open
	HI_BOOL bUv2cMode;		//UV2C bypass Mode, 0: U, 1: V
	HI_BOOL bUv2cEn;		//0: uv2c close; 1: uv2c open
	HI_BOOL bVcdsEn;		//UV vertical DS enable; 0: close; 1: open
	HI_BOOL bHcdsEn;		//UV Horizontal DS enable; 0: close; 1: open
	HI_S32 s32WidthOut;		
	HI_S32 s32WidthIn;	
	HI_S32 s32HCoef0;
	HI_S32 s32HCoef1;
	HI_S32 s32HCoef2;
	HI_S32 s32HCoef3;
	HI_S32 s32HCoef4;
	HI_S32 s32HCoef5;
	HI_S32 s32HCoef6;
	HI_S32 s32HCoef7;
	HI_S32 s32VCoef0;
	HI_S32 s32VCoef1;
	HI_S32 s32VCoef2;
	HI_S32 s32VCoef3;
	HI_S32 s32MfHeight;
	HI_S32 s32MfWidth;
	HI_U8  u8Limit;           
	HI_U8  u8Blendratio; 
	HI_S32 s32CpiFixTimingStat;
	HI_BOOL bCpiAutoTimingEn;
} ISP_MCDS_REG_CFG_S;
#define     NLUT_LENGTH         33
#define     WLUT_LENGTH         33
#define     WDR_LUT_LENGTH      128
#define     WDR_MAX_FRAME       2
#define     WDR_CHL_CHNLN       5

#define	    WDR_FRAME0          0
#define	    WDR_FRAME1          1

#define	    WDR_CHL_Rg          0
#define	    WDR_CHL_Gr          1
#define     WDR_CHL_Gb          2
#define     WDR_CHL_Bg          3
#define     WDR_CHL_MAX         4

typedef struct hiISP_WDR_REG_CFG_S
{	
	HI_BOOL	     bFlBmdtMnu;	
	HI_BOOL	     bUpdateNosLut;
	HI_U8	     u8NosFactorLow;                                // u4.2,  motion low factor 
	HI_U8        u8NosFactorHig;                                // u4.2,  motion high factor
	// comm reg									         
	HI_U16        au16InBLC[WDR_MAX_FRAME];                        // s15.0, in blc threshold
	HI_U32        u32OutBLC;                                      // u20.0, out blc threshold
	// motion reg								         
	HI_U8         u8BldrMdtMax;
	HI_U16       u16NosClipTH;                                   // u14.0, clip frame-difference map
	HI_U16       au16NosMDTLut[NLUT_LENGTH];                      // u16.0, noise model lut	
	HI_U8	     bAlgProcMDT;     	
	HI_U16       u16ShortThr;              /* RW,u14.0, Range: [0x0, 0x3FFF]. Data above this threshold will be taken from short exposure only.
	                                              This value is normally not changed and should be left at the default value */
	HI_U16       u16LongThr;             /* RW,u14.0, Range: [0x0, u16ShortThresh]. Data below this threshold will be taken from long exposure only.
	                                               This value is normally not changed and should be left at the default value */
	HI_U16       u16MDSfNrThr;           /*RW,u9.0,Range:[0,0x1F].If the motion degree is below this threshold,the short frame will be denoised*/
	HI_BOOL     bMDRefNoise;             /*RW,HI_TRUE: the motion detection refer to the noise LUT;HI_FALSE:the motion detection will not refer to the noise LUT*/													   	

} ISP_WDR_REG_CFG_S;

typedef struct hiISP_LSC_REG_CFG
{	
	//HI_BOOL bLscLutUpdateEn;
	HI_U32 grr_gain[289];
	HI_U32 gbb_gain[289];
	HI_U32 xgrid[8];
	HI_U32 ygrid[8];
	HI_U32 xinvgrid[8];
	HI_U32 yinvgrid[8];
}ISP_LSC_REG_CFG_S;

typedef struct hiISP_CAC_REG_CFG
{	
	HI_BOOL bLocalCacEn;
	HI_U16 u16RLumaThr;
	HI_U16 u16GLumaThr;
	HI_U16 u16BLumaThr;
	HI_U8  u8DePurpleCtrCr;
	HI_U8  u8DePurpleCtrCb;
	HI_U16 u16VarThr;
}ISP_CAC_REG_CFG_S;


typedef struct hiISP_DEMOSAIC_REG_CFG
{	         
    HI_U8   u8FcrGain;
	HI_U16  u16AndPart1;  			                                                                                                                                                                                                                                                                                                                                                         	                                                                                                                                                                                                                                                                                                                                       
    HI_U32  u32hfIntpRatio;
    HI_U32  u32hfIntpRatio1;
	HI_U8   u8hfIntpBldLow;
	HI_U16  u16hfIntpThLow;
	HI_U16  u16hfIntpThHig;
	HI_U16  u16hfIntpThLow1;
	HI_U16  u16hfIntpThHig1;
	HI_U8   u8hvColorRatio;
	HI_U8   u8difhvBlendLimit2;
	HI_U8   u8varBlrLowTh;
	HI_U8   u8hvBlendLimit1;
    HI_U8   u8hvBlendLimit2;  
	
    HI_BOOL bFcrEnable;
	HI_U16 u16FcrDetLow;                    
    HI_U8   u8FcrRatio     ;
    HI_U16  u16FcrLimit1   ;
    HI_U16  u16FcrLimit2   ;
    HI_U16  u16FcrThr      ;

	HI_BOOL bUpdateEhc;
	HI_BOOL bUpdateGF;

	HI_U8 u8ChrmGFFix;
	HI_U16 u16ClipDeltaFiltLow;  
	HI_U16 u16ClipDeltaIntpLow;
	HI_U16 u16ClipDeltaFiltHigh; 
	HI_U16 u16ClipDeltaIntpHigh; 
	HI_U8 u8ClipDeltaGain; 	
	HI_U8 u8ClipAdjustMax; 							
	HI_U8 u8FilterStrFilt; 	 
 	HI_U8 u8FilterStrIntp;	 
	HI_U8 u8VarOffsetGain;
	HI_U16 u16ClipUSM;		 // u12.0,	usm clip	
	HI_U8  u8DitherMask;  // u8.0
	HI_U8  u8DitherRatio; // u8.0
	HI_U8 u8DitherMax;   // u8.0
	HI_U8 u8MultiMF;		 // u3.2,	usm mf-gray gain
	HI_U16 u16GrayThFixLow;  
	HI_U16 u16SatuThFix; 
	HI_U8 u8MultiMFRed;
	HI_U8 u8MultiMFBlue;
	HI_U16 u16SatuRThFix; 	
	HI_U16 u16satuBThFix; 
	HI_S16 s16SatuFixEhcY;				
	HI_U8 u8BldrGbGr;
	HI_S8 s8FcrGFGain;		 // s2.2,	fcr control
	HI_U8 u8BldrCrCb;		 // u5.0,	gf cr-cb strength
	HI_U8 u8BldrGFStr;		 // u5.0,	gf r-g-b strength										 

	HI_U8 au8EhcGainLut[HI_ISP_NDDM_LUT_LENGTH]; // u8.0,	
	HI_U16 auGFBlurLut[HI_ISP_NDDM_LUT_LENGTH]; // u12.0,							 	
  
	HI_U16 u16ChrmGFLow;  
	HI_U16 u16ChrmGFHig;  
	HI_U16 u16SatuThLow;  
	HI_U16 u16SatuThHig;  
	HI_U16 u16GrayThFixHigh;
	HI_U16 u16GrayThLow;  
	HI_U16 u16GrayThHig;  	    
	HI_U16 u16FcrLimitLow; 
	HI_U16 u16FcrLimitHigh;

	HI_U8  u8BldrVarStr;  
	HI_U8  u8ShtCtrlGain;
	HI_U16 u16ShtCtrlTh;  	 
	HI_U16 u16ClipBUdSht; 
	HI_U16 u16ClipBOvSht; 
	HI_U16 u16ClipRUdSht; 
	HI_U16 u16ClipROvSht; 
	HI_U16 u16SatuRThLow; 
	HI_U16 u16SatuRThHig; 
	HI_U16 u16SatuBThLow; 
	HI_U16 u16SatuBThHig; 
	HI_U32 u32VarOffsetLow;	
    HI_U16 u16FakeCrVarThrLow;
}ISP_DEMOSAIC_REG_CFG_S;

typedef struct hiISP_CA_REG_CFG_S
{
	HI_U16 u16LumaThdHigh;
	HI_U16 u16HighLumaRatio;//U1.10   High Lightness Thresh' Ratio in Y-Alpha 
	HI_U16 au16YRatioLUT[HI_ISP_CA_YRATIO_LUT_LENGTH];  //1.10bit  Y Ratio For UV ; Max = 2047 FW Limit
	HI_S16 s16ISORatio; //1.10bit  ISO Ratio  For UV ,Max = 2047 FW Limi
} ISP_CA_REG_CFG_S;

typedef struct hiISP_BAS_REG_CFG
{
    HI_U32  u32BasMode;                              /*RW,Range: [ 0x0, 0x1FFFFF]   */
    HI_U32  u32BasOutWidth;                          /*RW,Range: [ 0x0, 0x1FFFFFFF] */
	HI_U32  u32BasOutHeight;                         /*RW,Range: [ 0x0, 0x1FFFFFFF] */
    HI_U32  u32BasIncH;                              /*RW,Range: [ 0x0, 0x1FFFFFFF] */
    HI_U32  u32BasIncV;                              /*RW,Range: [ 0x0, 0x1FFFFFFF] */
    HI_U32  u32RphWeiH;                              /*RW,Range: [ 0x0, 0x3FFFFF]   */
    HI_U32  u32RphWeiV;                              /*RW,Range: [ 0x0, 0x3FFFFF]   */
    HI_U32  au32BasTapsH6LutL[ISP_BAS_TAPS_ROW_NUM]; /*RW,Range: [ 0x0, 0xFFFFF]    */
    HI_U32  au32BasTapsH6LutH[ISP_BAS_TAPS_ROW_NUM]; /*RW,Range: [ 0x0, 0xFFFFF]    */
    HI_U32  au32BasTapsV6LutL[ISP_BAS_TAPS_ROW_NUM]; /*RW,Range: [ 0x0, 0xFFFFF]    */
    HI_U32  au32BasTapsV6LutH[ISP_BAS_TAPS_ROW_NUM]; /*RW,Range: [ 0x0, 0xFFFFF]    */
    HI_U32  au32BasTapsV4LutL[ISP_BAS_TAPS_ROW_NUM]; /*RW,Range: [ 0x0, 0x7FFF]     */
    HI_U32  au32BasTapsV4LutH[ISP_BAS_TAPS_ROW_NUM]; /*RW,Range: [ 0x0, 0x7FFF]     */
}ISP_BAS_REG_CFG_S;

typedef enum EMBayerPattern 
{
	BAYER_PATTERN_RG = 0x0,
	BAYER_PATTERN_GR = 0x1,
	BAYER_PATTERN_GB = 0x2,
	BAYER_PATTERN_BG = 0x3,
	BAYER_PATTERN_BUTT = 0x4,
}ISP_BayerPattern;

typedef struct hiISP_BAYERNR_REG_CFG_S
{
    HI_U8     u8Enable;
	// Sep Filter 
	HI_U32	  u32RawnrCRatio;			 
	// Chroma filter
	HI_U8     u8AmedMode;
	HI_U8	  u8AmedLevel[BAYER_PATTERN_BUTT];			 

	// Luminance filter			 
	HI_U16	  u16JnlmLimit0Lut[BAYER_PATTERN_BUTT][HI_ISP_BAYERNR_LUT_LENGTH];	 
	HI_U16	  u16JnlmLimit1Lut[BAYER_PATTERN_BUTT][HI_ISP_BAYERNR_LUT_LENGTH];	 

	HI_U8     u8WdrFrameNum;
	HI_U8	  u8JnlmGain;	
	HI_U8	  u8JnlmSadSel;
	HI_U16    u16JnlmMask;
	HI_U16	  u16JnlmCoringLow; 		 
	HI_U16	  u16JnlmCoringHig; 	

	HI_U16    u16JNLMCoringLowLUT[HI_ISP_BAYERNR_LUT_LENGTH];  //u14.0
	HI_U8     u8Blc;                      //u5.0
	HI_U16    u16Rgain;                    //u10.0
	HI_U16    u16Bgain;                    //u10.0

	HI_U16    u16satuRatio;                //u9.0, adjust noise reduction strength	    
	HI_U16    u16SatuThLow1;               // u9.0, saturation threshold low1
	HI_U16    u16satuThHig1;               // u9.0, saturation threshold high1	    
	HI_U16    u16SatuThLow2;               // u9.0, saturation threshold low2
	HI_U16    u16SatuThHig2;               // u9.0, saturation threshold high2
}ISP_BAYERNR_REG_CFG_S;

typedef struct hiISP_FLICKER_REG_CFG_S
{
	HI_S32 s32PreFrameAvgGr;  //s15.0, previous Frame Gr Avg 
	HI_S32 s32PreFrameAvgGb;  //s15.0, previous Frame Gb Avg
}ISP_FLICKER_REG_CFG_S;

typedef union hiISP_REG_KERNEL_CFG_KEY_U
{
    HI_U32  u32Key;
    struct
    {
        HI_U32  bit1UvnrCfg     : 1 ;   /* [0] */
		HI_U32  bit1McdsCfg     : 1 ;   /* [1] */
        HI_U32  bit30Rsv        : 30;   /* [2:31] */
    };
} ISP_REG_KERNEL_CFG_KEY_U;

typedef struct hiISP_REG_KERNEL_CFG_S
{
    ISP_REG_KERNEL_CFG_KEY_U    unKey;
	ISP_MCDS_REG_CFG_S      stMcdsRegCfg;
} ISP_REG_KERNEL_CFG_S;

typedef union hiISP_REG_CFG_KEY_U
{
    HI_U32  u32Key;
    struct
    {
        HI_U32  bit1AeCfg1      : 1 ;   /* [0] */
        HI_U32  bit1AeCfg2      : 1 ;   /* [1] */
        HI_U32  bit1AwbCfg1     : 1 ;   /* [2] */
        HI_U32  bit1AwbCfg2     : 1 ;   /* [3] */
        HI_U32  bit1AfCfg       : 1 ;   /* [4] */
        HI_U32  bit1OffsetCfg   : 1 ;   /* [5] */
        HI_U32  bit1DrcCfg      : 1 ;   /* [6] */
        //HI_U32  bit1NrCfg       : 1 ;   /* [7] */
        HI_U32  bit1DpCfg       : 1 ;   /* [8] */
        HI_U32  bit1SharpenCfg  : 1 ;   /* [9] */
        HI_U32  bit1GeCfg       : 1 ;   /* [10] */
        HI_U32  bit1WdrCfg      : 1 ;   /* [11] */
        HI_U32  bit1DehazeCfg   : 1 ;   /* [12] */
        HI_U32  bit1AwbCfg3     : 1 ;   /* [13] */
		HI_U32  bit1UvnrCfg     : 1 ;   /* [14] */
		HI_U32	bit1LscCfg		: 1 ;	/* [15] */
		HI_U32	bit1DemCfg		: 1 ;	/* [16] */
		HI_U32  bit1McdsCfg     : 1 ;   /* [17] */
		HI_U32  bit1BasCfg      : 1 ;   /* [18] */
		HI_U32  bit1FsWdrCfg    : 1 ;   /* [19] */
		HI_U32  bit1BayernrCfg  : 1 ;   /* [20] */
		HI_U32  bit1FlickerCfg  : 1 ;   /* [21] */
		HI_U32  bit1CaCfg       : 1 ;   /* [22] */
		HI_U32  bit1NddmCfg     : 1 ;   /* [23] */
		HI_U32  bit1CacCfg      : 1 ;   /* [24] */
        HI_U32  bit14Rsv        : 8;    /* [25:31] */
    };
}ISP_REG_CFG_KEY_U;

typedef struct hiISP_REG_CFG_S
{
    ISP_REG_CFG_KEY_U       unKey;

    ISP_AE_REG_CFG_1_S      stAeRegCfg1;
    ISP_AE_REG_CFG_2_S      stAeRegCfg2;
    ISP_AWB_REG_CFG_1_S     stAwbRegCfg1;
    ISP_AWB_REG_CFG_2_S     stAwbRegCfg2;
    ISP_AWB_REG_CFG_3_S     stAwbRegCfg3;
    ISP_AF_REG_CFG_S        stAfRegCfg;
    ISP_DP_REG_CFG_S        stDpRegCfg;
    ISP_DRC_REG_CFG_S       stDrcRegCfg;
    ISP_NR_REG_CFG_S        stNrRegCfg;
    ISP_GE_REG_CFG_S        stGeRegCfg;
    ISP_SHARPEN_REG_CFG_S   stSharpenRegCfg;
    ISP_OFFSET_REG_CFG_S    stOffsetRegCfg;
    ISP_WDR_REG_CFG_S       stWdrRegCfg;
	ISP_DEHAZE_REG_CFG_S    stDehazeRegCfg;
	ISP_MCDS_REG_CFG_S      stMcdsRegCfg;
	ISP_LSC_REG_CFG_S		stLscRegCfg;
	ISP_DEMOSAIC_REG_CFG_S  stDemRegCfg;
	ISP_BAS_REG_CFG_S       stBasRegCfg;
	ISP_BAYERNR_REG_CFG_S   stBayernrRegCfg;
	ISP_FLICKER_REG_CFG_S   stFlickerRegCfg;
	ISP_CA_REG_CFG_S        stCaRegCfg;
    ISP_REG_KERNEL_CFG_S    stKernelCfg;
	ISP_CAC_REG_CFG_S		stCacRegCfg;
} ISP_REG_CFG_S;

#define ISP_SYNC_BUF_NODE_NUM 5
#define CFG2VLD_DLY_LIMIT 4
#define WDR_EXP_RATIO_SHIFT 6
#define DRC_COMP_SHIFT 12

#define IS_LINEAR_MODE(mode)      (WDR_MODE_NONE == (mode))
#define IS_WDR_MODE(mode)         ((!IS_LINEAR_MODE(mode)) && ((mode) < WDR_MODE_BUTT))
#define IS_BUILT_IN_WDR_MODE(mode) (WDR_MODE_BUILT_IN == (mode))
#define IS_FS_WDR_MODE(mode)      ((!IS_LINEAR_MODE(mode))&& (!IS_BUILT_IN_WDR_MODE(mode)) && ((mode) < WDR_MODE_BUTT))
#define IS_2to1_WDR_MODE(mode)    ((WDR_MODE_2To1_FRAME == (mode)) || (WDR_MODE_2To1_FRAME_FULL_RATE == (mode)) || (WDR_MODE_2To1_LINE == (mode)))
#define IS_3to1_WDR_MODE(mode)    ((WDR_MODE_3To1_FRAME == (mode)) || (WDR_MODE_3To1_FRAME_FULL_RATE == (mode)) || (WDR_MODE_3To1_LINE == (mode)))
#define IS_4to1_WDR_MODE(mode)    ((WDR_MODE_4To1_FRAME == (mode)) || (WDR_MODE_4To1_FRAME_FULL_RATE == (mode)) || (WDR_MODE_4To1_LINE == (mode)))
#define IS_FULL_WDR_MODE(mode)    ((WDR_MODE_2To1_FRAME_FULL_RATE == (mode)) || (WDR_MODE_3To1_FRAME_FULL_RATE == (mode)) || (WDR_MODE_4To1_FRAME_FULL_RATE == (mode)))
#define IS_HALF_WDR_MODE(mode)    ((WDR_MODE_2To1_FRAME == (mode)) || (WDR_MODE_3To1_FRAME == (mode)) || (WDR_MODE_4To1_FRAME == (mode)))
#define IS_LINE_WDR_MODE(mode)    ((WDR_MODE_2To1_LINE == (mode)) || (WDR_MODE_3To1_LINE == (mode)) || (WDR_MODE_4To1_LINE == (mode)))


#define ISP_SET_MOD_PARAM      _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_MOD_PARAM, ISP_MOD_PARAM_S)
#define ISP_GET_MOD_PARAM      _IOR(IOC_TYPE_ISP, IOC_NR_ISP_GET_MOD_PARAM, ISP_MOD_PARAM_S)


typedef struct hiISP_SYNC_CFG_BUF_NODE_S
{
    HI_BOOL bValid;
    
    ISP_SNS_REGS_INFO_S stSnsRegsInfo;
    ISP_AE_REG_CFG_2_S  stAERegCfg;
	ISP_WDR_REG_CFG_S stWDRRegCfg;
} ISP_SYNC_CFG_BUF_NODE_S;

typedef struct hiISP_REGCFG_S
{
    HI_BOOL bInit;
    ISP_REG_CFG_S stRegCfg;
    ISP_SYNC_CFG_BUF_NODE_S stSyncCfgNode;
} ISP_REGCFG_S;

typedef struct hiISP_WDR_CFG_S
{
    HI_U8 u8WDRMode;
	HI_U32 au32ExpRatio[3];
} ISP_WDR_CFG_S;

typedef struct hiISP_RES_SWITCH_MODE_S
{
    HI_U16   u16Width;
    HI_U16   u16Height;
    HI_FLOAT f32Fps;
}ISP_RES_SWITCH_MODE_S;


typedef struct hiISP_PROC_MEM_S
{
    HI_U32 u32ProcSize;
    HI_U32 u32ProcPhyAddr;
    HI_VOID *pProcVirtAddr;
} ISP_PROC_MEM_S;

typedef struct hi_ISP_ACM_MMZ_S
{
    HI_U32      u32AcmSize  ;
    HI_VOID*    pVirAddr    ;
    HI_U32      u32PhyAddr  ;
}ISP_ACM_BUFFER_S;


#define ISP_1ST_INT                0x1
#define ISP_2ND_INT                0x2
#define ISP_UNDEF_INT              0xF4
                                   
#define ISP_DEV_SET_FD             _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_FD, HI_S32)
#define ISP_GET_FRAME_EDGE         _IOR(IOC_TYPE_ISP, IOC_NR_ISP_GET_FRAME_EDGE, HI_S32)
#define ISP_GET_VD_TIMEOUT         _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_VD_TIMEOUT, ISP_VD_TIMEOUT_S)
#define ISP_GET_VD_END_TIMEOUT     _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_VD_END_TIMEOUT, ISP_VD_TIMEOUT_S)
#define ISP_SET_REGNEWER           _IO(IOC_TYPE_ISP, IOC_NR_ISP_SET_REGNEWER)
#define ISP_SET_INT_ENABLE         _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_INT_ENABLE, HI_BOOL)
#define ISP_GET_VERSION            _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_VERSION, ISP_VERSION_S)
#define ISP_STAT_BUF_INIT          _IOR(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_INIT, HI_U32)
#define ISP_STAT_BUF_EXIT          _IO(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_EXIT)
#define ISP_STAT_BUF_GET           _IOR(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_GET, ISP_STAT_INFO_S)
#define ISP_STAT_BUF_PUT           _IOW(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_PUT, ISP_STAT_INFO_S)
#define ISP_STAT_ACT_GET           _IOR(IOC_TYPE_ISP, IOC_NR_ISP_STAT_ACT_GET, ISP_STAT_INFO_S)
#define ISP_STAT_SHADOW_MEMPHY_GET     _IOR(IOC_TYPE_ISP, IOC_NR_ISP_STAT_SHADOW_MEMPHY_GET, HI_U32)
#define ISP_STAT_SHADOW_MEMSTATE_SET   _IOW(IOC_TYPE_ISP, IOC_NR_ISP_STAT_SHADOW_MEMSTATE_SET, HI_BOOL)

#define ISP_REG_CFG_INIT           _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_REG_CFG_INIT, ISP_REG_CFG_S)
#define ISP_REG_CFG_SET            _IOW(IOC_TYPE_ISP, IOC_NR_ISP_REG_CFG_SET, ISP_REG_KERNEL_CFG_S)
#define ISP_MEM_INFO_SET           _IOW(IOC_TYPE_ISP, IOC_NR_ISP_MEM_INFO_SET, HI_BOOL)
#define ISP_MEM_INFO_GET           _IOR(IOC_TYPE_ISP, IOC_NR_ISP_MEM_INFO_GET, HI_BOOL)

#define ISP_PROC_INIT              _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PROC_INIT, ISP_PROC_MEM_S)
#define ISP_PROC_WRITE_ING         _IO(IOC_TYPE_ISP, IOC_NR_ISP_PROC_WRITE_ING)
#define ISP_PROC_WRITE_OK          _IO(IOC_TYPE_ISP, IOC_NR_ISP_PROC_WRITE_OK)
#define ISP_PROC_EXIT              _IO(IOC_TYPE_ISP, IOC_NR_ISP_PROC_EXIT)
#define ISP_PROC_PARAM_GET         _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PROC_PARAM_GET, HI_U32)
#define ISP_UPDATEINFO_BUF_INIT    _IOR(IOC_TYPE_ISP, IOC_NR_ISP_UPDATEINFO_BUF_INIT, HI_U32)
#define ISP_UPDATEINFO_BUF_EXIT    _IO(IOC_TYPE_ISP, IOC_NR_ISP_UPDATEINFO_BUF_EXIT)
                                   
#define ISP_SYNC_CFG_SET           _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SYNC_CFG_SET, ISP_SYNC_CFG_BUF_NODE_S)
#define ISP_WDR_CFG_SET            _IOW(IOC_TYPE_ISP, IOC_NR_WDR_SYNC_CFG_SET, ISP_WDR_CFG_S)
#define ISP_RES_SWITCH_SET         _IOW(IOC_TYPE_ISP, IOC_NR_ISP_RES_SWITCH_SET, ISP_RES_SWITCH_MODE_S)
                                   
#define ISP_ACM_ATTR_GET           _IOR(IOC_TYPE_ISP, IOC_NR_ACM_ATTR_GET, ISP_ACM_ATTR_S)
#define ISP_ACM_ATTR_SET           _IOW(IOC_TYPE_ISP, IOC_NR_ACM_ATTR_SET, ISP_ACM_ATTR_S)

/* because ACM coef size is large than 2^14bit(16384) */
#define ISP_ACM_LUT_PHY_ADDR_GET        0xFE//_IOWR(IOC_TYPE_ISP, IOC_NR_ACM_COEF_GET, ISP_ACM_BUFFER_S)
#define ISP_ACM_COEF_SET        0xFF//_IOW(IOC_TYPE_ISP, IOC_NR_ACM_COEF_SET, ISP_ACM_BUFFER_S)

#define ISP_PWM_NUM_GET            _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PWM_NUM_GET, HI_U32)

#define ISP_GET_DEV(f)             (*((HI_U32*)(f)))             

#define ISP_CHECK_DEV(dev)\
do {\
    if (0 != dev)\
    {\
        ISP_TRACE(HI_DBG_ERR, "Err isp dev %d!\n", dev);\
        return HI_ERR_ISP_ILLEGAL_PARAM;\
    }\
}while(0)    



#define ISP_CHECK_ISP_STATBUFF_INIT(dev)\
do{\
	if (g_astIspDrvCtx[dev].stStatisticsBuf.bInit != HI_TRUE)\
	{\
		ISP_TRACE(HI_DBG_ERR, "Isp[%d] statistics buffer doesn't initialized in %s!\n", dev, __FUNCTION__);\
		return HI_ERR_ISP_MEM_NOT_INIT;\
	}\
}while(0)


#define ISP_CHECK_POINTER(ptr)\
do {\
    if (HI_NULL == ptr)\
    {\
        ISP_TRACE(HI_DBG_ERR, "Null Pointer!\n");\
        return HI_ERR_ISP_NULL_PTR;\
    }\
}while(0)

#define ISP_CHECK_BOOL(bool)\
do {\
    if (( HI_TRUE != bool)&&( HI_FALSE != bool))\
    {\
        ISP_TRACE(HI_DBG_ERR, "Invalid Isp Bool Type %d!\n", bool);\
        return HI_ERR_ISP_ILLEGAL_PARAM;\
    }\
}while(0)

#define ISP_DBG(bEn, fmt...)\
do{\
    if (bEn)\
        printf(fmt);\
}while(0)

#endif /* __HI3518_ISP_DEF_H__ */

