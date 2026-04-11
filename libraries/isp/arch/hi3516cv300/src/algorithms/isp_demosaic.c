/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_demosaic.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/07/21
  Description   : 
  History       :
  1.Date        : 2015/07/21
    Author      : q00214668
    Modification: Created file

******************************************************************************/

#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_proc.h"
#include "isp_math_utils.h"
#include <math.h>
#include "hi_math.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define HI_ISP_DEMOSAIC_LUT_LEN (16)
#define HI_DEMOSAIC_BITDEPTH    (12)
#define HI_ISP_DEMOSAIC_LUT_LENGTH  (17) 

#define HI_DEMOSAIC_FCRISOTH1 (20000)
#define HI_DEMOSAIC_FCRISOTH2 (30000)

#define	HI_ISP_DEMOSAIC_DIFHVBLENDLIMIT1 (20)
#define HI_ISP_DEMOSAIC_FCR_EN_MPI (1)
#define	HI_ISP_DEMOSAIC_FCR_LIMIT1_RANGE (HI_ISP_DEMOSAIC_FCR_LIMIT1_DEFAULT)
#define	HI_ISP_DEMOSAIC_FCR_GAIN_MAX (31)
#define	HI_ISP_DEMOSAIC_FCR_GAIN_RANGE (HI_ISP_DEMOSAIC_FCR_GAIN_MAX)
#define	HI_ISP_DEMOSAIC_FCR_RATIO (24)
#define	HI_ISP_DEMOSAIC_FCR_RATIO_MAX (63)
#define	HI_ISP_DEMOSAIC_FCR_RATIO_RANGE (HI_ISP_DEMOSAIC_FCR_RATIO_MAX-HI_ISP_DEMOSAIC_FCR_RATIO)
#define	HI_ISP_DEMOSAIC_FCR_STRENGTH_DEFAULT  (8)
#define HI_ISP_DEMOSAIC_FCR_THRESHOLD_DEFAULT (24)

#ifndef HI_DEMOSAIC_MAX
#define HI_DEMOSAIC_MAX(a, b) (((a) < (b)) ?  (b) : (a))
#endif

#ifndef HI_DEMOSAIC_MIN
#define HI_DEMOSAIC_MIN(a, b) (((a) > (b)) ?  (b) : (a))
#endif

#define HI_DEMOSAIC_CLIP3(low, high, x) (HI_DEMOSAIC_MAX( HI_DEMOSAIC_MIN((x), high), low))

static HI_U32 au32DemosaicIsoLut[ISP_AUTO_ISO_STRENGTH_NUM] = {100,200,400,800,1600,3200,6400,12800,25600,51200,102400,204800,409600,819200,1638400,3276800};
static HI_U32 u32FcrShift = (HI_DEMOSAIC_BITDEPTH - 10 >= 0) ? (HI_DEMOSAIC_BITDEPTH - 10) : (10 - HI_DEMOSAIC_BITDEPTH);

typedef struct hiISP_DEMOSAIC_AUTO_S
{
	HI_U16  au16EdgeSmoothThr   [ISP_AUTO_ISO_STRENGTH_NUM]; //[0x0, 0x3FE]    //Threshold for smoothing the rough edges or edge noise
	HI_U16  au16EdgeSmoothSlope [ISP_AUTO_ISO_STRENGTH_NUM]; //[0x0, 0x3FF]    //Slope of decreasing the edge smooth strength
	HI_U16  au16AntiAliasThr    [ISP_AUTO_ISO_STRENGTH_NUM]; //[0x0, 0x3FE]    //Threshold to decreasing the strength of moire pattern removing in high frequency areas
	HI_U16  au16AntiAliasSlope  [ISP_AUTO_ISO_STRENGTH_NUM]; //[0x0, 0x3FF]    //Slope of increasing strength for removing the moire pattern 

	HI_U16  au16NrCoarseStr    [ISP_AUTO_ISO_STRENGTH_NUM];  //[0, 0x100]      //Coarse strength for noise suppressing and detail enhancement	          		  
	HI_U16  au16NoiseSuppressStr[ISP_AUTO_ISO_STRENGTH_NUM]; //[0, 0x3FF]      //Fine strength for noise suppressing 
	HI_U8   au8DetailEnhanceStr [ISP_AUTO_ISO_STRENGTH_NUM]; //[0, 0xa]        //Strength for weak detail enhancement	   
 	HI_U16  au16SharpenLumaStr  [ISP_AUTO_ISO_STRENGTH_NUM]; //[0, 0x200]      //Strength of luma sharpening in high saturation regions
 	HI_U16  au16ColorNoiseCtrlThr[ISP_AUTO_ISO_STRENGTH_NUM]; //[0, 0x12c]    //Threshold for dark color noise control
}ISP_DEMOSAIC_AUTO_S;

typedef struct hiISP_DEMOSAIC_MANUAL_S
{
    HI_U16  u16EdgeSmoothThr;   //[0x0, 0x3FE]    //Threshold for smoothing the rough edges or edge noise
    HI_U16  u16EdgeSmoothSlope; //[0x0, 0x3FF]    //Slope of decreasing the edge smooth strength
    HI_U16  u16AntiAliasThr;    //[0x0, 0x3FE]    //Threshold to decreasing the strength of moire pattern removing in high frequency areas
    HI_U16  u16AntiAliasSlope;  //[0x0, 0x3FF] 	  //Slope of increasing strength for removing the moire pattern 

	HI_U16  u16NrCoarseStr;     //[0, 0x100]      //Coarse strength for noise suppressing and detail enhancement
    HI_U16  u16NoiseSuppressStr;//[0, 0x3FF]      //Fine strength for noise suppressing
    HI_U8   u8DetailEnhanceStr; //[0, 0xa]        //Strength for weak detail enhancement	   
    HI_U16  u16SharpenLumaStr;  //[0, 0x200]      //Strength of luma sharpening in high saturation regions
    HI_U16  u16ColorNoiseCtrlThr; //[0, 0x12c]    //Threshold for dark color noise control
}ISP_DEMOSAIC_MANUAL_S;


typedef struct hiISP_DEMOSAIC_S
{
	// Processing Depth 
	HI_BOOL bEnable;                //[0, 1]         
	HI_BOOL bInit;
	HI_U8 	u8BitDepthPrc; 			// u5
	HI_U8   u8WdrMode;
	HI_BOOL bUpdateEhcLut;

	/*False Color*/
    HI_U8   au8FcrAutoStrength[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8   au8FcrAutoThreshold[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8   au16FcrAutoOffset[ISP_AUTO_ISO_STRENGTH_NUM];    
    HI_U8   u8FcrManuStrength;
    HI_U8   u8FcrManuThreshold;
    HI_U16  u16FcrManuOffset;
    HI_U16  u16FcrOffset;    
	HI_BOOL bFcrEnable;
	HI_U16  u16FcrThr;	
	HI_U8   u8FcrRatio;    	
	HI_U8   u8FcrGain;
	HI_U16  u16FcrLimit1;
	HI_U16  u16FcrLimit2;

	// FCR refer to ISO, used in FW, to weak 'falseColorGain' from setting value to zero             
	HI_U32  u32FcrISOTH1;
	HI_U32  u32FcrISOTH2;
	
	HI_U8   s8FcrGFGain;     // s2.2,	fcr control
	HI_U8   au8EhcGainLut[HI_ISP_DEMOSAIC_LUT_LENGTH]; // u8.0,	usm gain at each luma										 	
	HI_U16  u16ClipUSM;		 // u12.0,	usm clip	
	HI_U16  u16ClipDeltaIntp; // u12.0,	apt interpolation thresh
	HI_U16  u16ClipDeltaFilt; // u12.0,	apt filtering thresh			
	HI_S32  s32Strength;

	HI_U8   au8LutBldrCrCb[HI_ISP_DEMOSAIC_LUT_LEN];//in
	HI_U8   au8LutBldrGrGb[HI_ISP_DEMOSAIC_LUT_LEN]; //in
	HI_U8   au8LutBldrGFStr[HI_ISP_DEMOSAIC_LUT_LEN];//in
	HI_U8   au8LutBldrVarStr[HI_ISP_DEMOSAIC_LUT_LEN];	//in
	HI_U8   au8LutMultiMFRed[HI_ISP_DEMOSAIC_LUT_LEN];
	HI_U8   au8LutMultiMFBlue[HI_ISP_DEMOSAIC_LUT_LEN];
	HI_U8 	au8LutDithMask[HI_ISP_DEMOSAIC_LUT_LEN];//in
	HI_U8 	au8LutDithMax[HI_ISP_DEMOSAIC_LUT_LEN];	
	HI_U8   au8LutChrmGFFix[HI_ISP_DEMOSAIC_LUT_LEN];
	HI_U8   au8LutShtCtrlGain[HI_ISP_DEMOSAIC_LUT_LEN];//in
	HI_U8   au8LutVarOffsetGain[HI_ISP_DEMOSAIC_LUT_LEN];
	HI_S8   as8LutFcrGFGain[HI_ISP_DEMOSAIC_LUT_LEN];  //u8
	HI_U8   au8LutClipDeltaGain[HI_ISP_DEMOSAIC_LUT_LEN];
	HI_U8   au8LutClipAdjustMax[HI_ISP_DEMOSAIC_LUT_LEN];//8
	HI_U8   au8LutFilterStrIntp[HI_ISP_DEMOSAIC_LUT_LEN];
	HI_U16	au16LutDithRatio[HI_ISP_DEMOSAIC_LUT_LEN];//in
	HI_U16  au16LutClipUSM[HI_ISP_DEMOSAIC_LUT_LEN];//in

	HI_U16  au16LutChrmGFLow[HI_ISP_DEMOSAIC_LUT_LEN];//in
	HI_U16  au16LutChrmGFHig[HI_ISP_DEMOSAIC_LUT_LEN];//in
	HI_U16  au16LutSatuThFix[HI_ISP_DEMOSAIC_LUT_LEN];
	HI_U16  au16LutSatuThLow[HI_ISP_DEMOSAIC_LUT_LEN];//in
	HI_U16  au16LutSatuThHig[HI_ISP_DEMOSAIC_LUT_LEN];//in
	HI_U16  au16LutGrayThFixLow[HI_ISP_DEMOSAIC_LUT_LEN];
	HI_U16  au16LutGrayThFixHig[HI_ISP_DEMOSAIC_LUT_LEN];//in
	HI_U16  au16LutGrayThLow[HI_ISP_DEMOSAIC_LUT_LEN];//in
	HI_U16  au16LutGrayThHig[HI_ISP_DEMOSAIC_LUT_LEN];//in
	HI_U16  au16LutFcrDetLow[HI_ISP_DEMOSAIC_LUT_LEN];//in
	HI_U16  au16LutFcrLimitLow[HI_ISP_DEMOSAIC_LUT_LEN];//in
	HI_U16  au16LutFcrLimitHigh[HI_ISP_DEMOSAIC_LUT_LEN];//in
	HI_U16  au16LutShtCtrlTh[HI_ISP_DEMOSAIC_LUT_LEN];	//in
	HI_U16  au16LutClipBUdSht[HI_ISP_DEMOSAIC_LUT_LEN];//in
	HI_U16  au16LutClipBOvSht[HI_ISP_DEMOSAIC_LUT_LEN];//in
	HI_U16  au16LutClipRUdSht[HI_ISP_DEMOSAIC_LUT_LEN];//in
	HI_U16  au16LutClipROvSht[HI_ISP_DEMOSAIC_LUT_LEN];//in
	HI_U16  au16LutSatuRThLow[HI_ISP_DEMOSAIC_LUT_LEN];//in
	HI_U16  au16LutSatuRThHig[HI_ISP_DEMOSAIC_LUT_LEN];//in
	HI_U16  au16LutSatuRThFix[HI_ISP_DEMOSAIC_LUT_LEN];
	HI_U16  au16LutSatuBThLow[HI_ISP_DEMOSAIC_LUT_LEN];//in
	HI_U16  au16LutSatuBThHig[HI_ISP_DEMOSAIC_LUT_LEN];//in
	HI_U16  au16LutSatuBThFix[HI_ISP_DEMOSAIC_LUT_LEN];
	HI_S32  as32LutVarOffsetLow[HI_ISP_DEMOSAIC_LUT_LEN];	//in
	HI_U16  au16LutClipDeltaIntpLow[HI_ISP_DEMOSAIC_LUT_LEN];//in
	HI_U16  au16LutClipDeltaIntpRange[HI_ISP_DEMOSAIC_LUT_LEN];//in
	HI_U16  au16LutClipDeltaIntpHigh[HI_ISP_DEMOSAIC_LUT_LEN];//in

	HI_FLOAT afsigma[HI_ISP_DEMOSAIC_LUT_LENGTH];

	ISP_OP_TYPE_E enOpTypeFcr;    
    ISP_OP_TYPE_E enOpType;
	ISP_DEMOSAIC_AUTO_S  stAuto;
	ISP_DEMOSAIC_MANUAL_S stManual;
} ISP_DEMOSAIC_S;

ISP_DEMOSAIC_S g_astDemosaicCtx[ISP_MAX_DEV_NUM] = {{0}};
#define DEMOSAIC_GET_CTX(dev, pstCtx)   pstCtx = &g_astDemosaicCtx[dev]

static HI_VOID DemosaicExtRegsInitialize(ISP_DEV IspDev)
{ 
    HI_U8 i;

    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
	ISP_CTX_S *pstIspCtx = HI_NULL;
	ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_SensorGetDefault(IspDev, &pstSnsDft);

	hi_ext_system_demosaic_enable_write(pstSnsDft->stDemosaic.bEnable);
	hi_ext_system_demosaic_manual_mode_write(HI_EXT_SYSTEM_DEMOSAIC_MANUAL_MODE_DEFAULT);
	hi_ext_system_fcr_manual_mode_write(HI_EXT_SYSTEM_FCR_MANUAL_MODE_DEFAULT);	
	
	for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
	{
		hi_ext_system_fcr_enable_write(pstSnsDft->stFcr.bEnable);
		hi_ext_system_auto_fcr_strength_write(i, pstSnsDft->stFcr.au8Strength[i]);
		hi_ext_system_auto_fcr_threshold_write(i, pstSnsDft->stFcr.au8Threshold[i]);
		hi_ext_system_auto_fcr_offset_write(i, pstSnsDft->stFcr.au16Offset[i]); 		 
	}
	hi_ext_system_manual_fcr_strength_write(pstSnsDft->stFcr.au8Strength[0]);
	hi_ext_system_manual_fcr_threshold_write(pstSnsDft->stFcr.au8Threshold[0]);
	hi_ext_system_manual_fcr_offset_write(pstSnsDft->stFcr.au16Offset[0]);
	hi_ext_system_fcr_enable_write(HI_EXT_SYSTEM_FCR_ENABLE_DEFAULT);

	for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
	{
		hi_ext_system_demosaic_auto_edge_smooth_thresh_write(i, pstSnsDft->stDemosaic.au16EdgeSmoothThr[i]);
		hi_ext_system_demosaic_auto_edge_smooth_slope_write(i, pstSnsDft->stDemosaic.au16EdgeSmoothSlope[i]);
		hi_ext_system_demosaic_auto_antialias_thresh_write(i, pstSnsDft->stDemosaic.au16AntiAliasThr[i]);
		hi_ext_system_demosaic_auto_antialias_slope_write(i, pstSnsDft->stDemosaic.au16AntiAliasSlope[i]);
		
		hi_ext_system_demosaic_auto_gain_write(i, pstSnsDft->stDemosaic.au16NrCoarseStr[i]);
		hi_ext_system_demosaic_auto_noise_suppress_strength_write(i, pstSnsDft->stDemosaic.au16NoiseSuppressStr[i]);
		hi_ext_system_demosaic_auto_detail_enhance_write(i, pstSnsDft->stDemosaic.au8DetailEnhanceStr[i]);
		hi_ext_system_satufixehcy_lut_write(i, pstSnsDft->stDemosaic.au16SharpenLumaStr[i]); 
        hi_ext_system_demosaic_auto_colornoise_ctrl_thresh_write(i, pstSnsDft->stDemosaic.au16ColorNoiseCtrlThr[i]);
	}
		
	if (IS_WDR_MODE(pstIspCtx->u8SnsWDRMode))	   // WDR mode
	{
		hi_ext_system_demosaic_manual_edge_smooth_thresh_write(HI_EXT_SYSTEM_DEMOSAIC_MANU_EDGE_SMOOTH_TH_WDR_DEFAULT);
		hi_ext_system_demosaic_manual_edge_smooth_slope_write(HI_EXT_SYSTEM_DEMOSAIC_MANU_EDGE_SMOOTH_SLOPE_WDR_DEFAULT);
		hi_ext_system_demosaic_manual_antialias_thresh_write(HI_EXT_SYSTEM_DEMOSAIC_MANU_ANTIALIAS_TH_WDR_DEFAULT);
		hi_ext_system_demosaic_manual_antialias_slope_write(HI_EXT_SYSTEM_DEMOSAIC_MANU_ANTIALIAS_SLOPE_WDR_DEFAULT);
		
		hi_ext_system_demosaic_manual_gain_write(HI_EXT_SYSTEM_DEMOSAIC_MANUAL_GAIN_WDR_DEFAULT);
		hi_ext_system_demosaic_manual_detail_enhance_write(HI_EXT_SYSTEM_DEMOSAIC_MANUAL_DETAIL_ENHANCE_WDR_DEFAULT);
		hi_ext_system_demosaic_manual_satufixehcy_write(HI_EXT_SYSTEM_DEMOSAIC_MANUAL_SATUFIXEHCY_WDR_DEFAULT);
		hi_ext_system_demosaic_manual_noise_suppress_strength_write(HI_EXT_SYSTEM_DEMOSAIC_MANUAL_NOISE_SUPPRESS_STR_WDR_DEFAULT);
        hi_ext_system_demosaic_manual_colornoise_ctrl_thresh_write(HI_EXT_SYSTEM_DEMOSAIC_MANUAL_COLORNOISE_CTRL_THRESH_WDR_DEFAULT);
	}
	else	   //Linear Mode
	{	
		hi_ext_system_demosaic_manual_edge_smooth_thresh_write(HI_EXT_SYSTEM_DEMOSAIC_MANU_EDGE_SMOOTH_TH_LINEAR_DEFAULT);
		hi_ext_system_demosaic_manual_edge_smooth_slope_write(HI_EXT_SYSTEM_DEMOSAIC_MANU_EDGE_SMOOTH_SLOPE_LINEAR_DEFAULT);
		hi_ext_system_demosaic_manual_antialias_thresh_write(HI_EXT_SYSTEM_DEMOSAIC_MANU_ANTIALIAS_TH_LINEAR_DEFAULT);
		hi_ext_system_demosaic_manual_antialias_slope_write(HI_EXT_SYSTEM_DEMOSAIC_MANU_ANTIALIAS_SLOPE_LINEAR_DEFAULT);
		
		hi_ext_system_demosaic_manual_gain_write(HI_EXT_SYSTEM_DEMOSAIC_MANUAL_GAIN_LINEAR_DEFAULT);
		hi_ext_system_demosaic_manual_detail_enhance_write(HI_EXT_SYSTEM_DEMOSAIC_MANUAL_DETAIL_ENHANCE_LINEAR_DEFAULT);
		hi_ext_system_demosaic_manual_satufixehcy_write(HI_EXT_SYSTEM_DEMOSAIC_MANUAL_SATUFIXEHCY_LINEAR_DEFAULT);
		hi_ext_system_demosaic_manual_noise_suppress_strength_write(HI_EXT_SYSTEM_DEMOSAIC_MANUAL_NOISE_SUPPRESS_STR_LINEAR_DEFAULT);
        hi_ext_system_demosaic_manual_colornoise_ctrl_thresh_write(HI_EXT_SYSTEM_DEMOSAIC_MANUAL_COLORNOISE_CTRL_THRESH_LINEAR_DEFAULT);
	}
    return;
}

static HI_VOID  DemosaicInitFwLinear(ISP_DEV IspDev)
{	
	ISP_DEMOSAIC_S *pstDemosaic = HI_NULL;
	
	HI_U16  au16LutNddmGain[HI_ISP_DEMOSAIC_LUT_LEN]    = {128, 128, 128, 64, 64, 32, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16};
	HI_U8   au8LutBldrCrCb[HI_ISP_DEMOSAIC_LUT_LEN]     = {8, 8, 8, 6, 6, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	HI_U8   au8LutBldrGrGb[HI_ISP_DEMOSAIC_LUT_LEN]     = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
	HI_U8   au8LutBldrGFStr[HI_ISP_DEMOSAIC_LUT_LEN]    = {0, 0, 0, 0, 0, 4, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	HI_U8   au8LutBldrVarStr[HI_ISP_DEMOSAIC_LUT_LEN]   = {0, 0, 0, 6, 6, 4, 2, 1, 1, 1, 1, 1 ,1, 1, 1, 1};	
	HI_U8   au8LutMultiMF[HI_ISP_DEMOSAIC_LUT_LEN]      = {4, 4, 4, 4, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	HI_U8   au8LutMultiMFRed[HI_ISP_DEMOSAIC_LUT_LEN]   = {10, 10, 10, 6, 6, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0};
	HI_U8   au8LutMultiMFBlue[HI_ISP_DEMOSAIC_LUT_LEN]  = {3, 3, 3, 3, 3, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	HI_U8 	au8LutDithMask[HI_ISP_DEMOSAIC_LUT_LEN] 	= {15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15};
	HI_U8 	au8LutDithMax[HI_ISP_DEMOSAIC_LUT_LEN]		= {5, 5, 5, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8};
	HI_S8   as8LutFcrGFGain[HI_ISP_DEMOSAIC_LUT_LEN]    = {4, 4, 4, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	HI_U8   au8LutChrmGFFix[HI_ISP_DEMOSAIC_LUT_LEN]    = {6, 6, 6, 6, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	HI_U8   au8LutShtCtrlGain[HI_ISP_DEMOSAIC_LUT_LEN]  = {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6};	
	HI_U8   au8LutVarOffsetGain[HI_ISP_DEMOSAIC_LUT_LEN]= {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16};
	HI_U8   au8LutFilterStrIntp[HI_ISP_DEMOSAIC_LUT_LEN]= {8, 8, 8, 8, 8, 8, 8, 10, 10, 10, 10, 10, 10, 10, 10, 10};
	HI_U8   au8LutClipDeltaGain[HI_ISP_DEMOSAIC_LUT_LEN]= {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
	HI_U8   au8LutClipAdjustMax[HI_ISP_DEMOSAIC_LUT_LEN]= {40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40};
	HI_U8   au8EhcGainLut[HI_ISP_DEMOSAIC_LUT_LENGTH]   = {0, 200, 255, 100, 80, 75, 70, 64, 36, 32, 28, 28, 24, 24, 16, 16, 16};

	HI_U16  au16LutClipUSM[HI_ISP_DEMOSAIC_LUT_LEN]     = {100, 100, 100, 96, 72, 36, 18, 6, 1, 0, 0, 0, 0, 0, 0, 0};
	HI_U16  au16LutChrmGFLow[HI_ISP_DEMOSAIC_LUT_LEN]   = {130, 130, 130, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140};
	HI_U16  au16LutChrmGFHig[HI_ISP_DEMOSAIC_LUT_LEN]   = {190, 180, 180, 180, 180, 180, 180, 180, 180};
	HI_U16  au16LutSatuThFix[HI_ISP_DEMOSAIC_LUT_LEN]   = {90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90};
	HI_U16  au16LutSatuThLow[HI_ISP_DEMOSAIC_LUT_LEN]   = {110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110};
	HI_U16  au16LutSatuThHig[HI_ISP_DEMOSAIC_LUT_LEN]   = {200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200};
	HI_U16  au16LutGrayThFixLow[HI_ISP_DEMOSAIC_LUT_LEN]= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	HI_U16  au16LutGrayThFixHig[HI_ISP_DEMOSAIC_LUT_LEN]= {256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256};
	HI_U16  au16LutGrayThLow[HI_ISP_DEMOSAIC_LUT_LEN]   = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
	HI_U16  au16LutGrayThHig[HI_ISP_DEMOSAIC_LUT_LEN]   = {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20};
	HI_U16  au16LutFcrDetLow[HI_ISP_DEMOSAIC_LUT_LEN]   = {120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120};
	HI_U16  au16LutFcrLimitLow[HI_ISP_DEMOSAIC_LUT_LEN] = {128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128};
	HI_U16  au16LutFcrLimitHigh[HI_ISP_DEMOSAIC_LUT_LEN]= {256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256};
	HI_U16  au16LutShtCtrlTh[HI_ISP_DEMOSAIC_LUT_LEN]   = {50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50};	
	HI_U16	au16LutDithRatio[HI_ISP_DEMOSAIC_LUT_LEN]	= {128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128};

	HI_U16  au16LutClipBUdSht[HI_ISP_DEMOSAIC_LUT_LEN]  = {500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500};
	HI_U16  au16LutClipBOvSht[HI_ISP_DEMOSAIC_LUT_LEN]  = {500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500};
	HI_U16  au16LutClipRUdSht[HI_ISP_DEMOSAIC_LUT_LEN]  = {1024, 1024, 1024, 1024, 1024, 1024, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256};
	HI_U16  au16LutClipROvSht[HI_ISP_DEMOSAIC_LUT_LEN]  = {256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256};
	HI_U16  au16LutSatuRThLow[HI_ISP_DEMOSAIC_LUT_LEN]  = {130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130};
	HI_U16  au16LutSatuRThHig[HI_ISP_DEMOSAIC_LUT_LEN]  = {200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200};
	HI_U16  au16LutSatuRThFix[HI_ISP_DEMOSAIC_LUT_LEN]  = {90, 90, 90, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92};
	HI_U16  au16LutSatuBThLow[HI_ISP_DEMOSAIC_LUT_LEN]  = {120, 120, 120, 120, 120, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130};
	HI_U16  au16LutSatuBThHig[HI_ISP_DEMOSAIC_LUT_LEN]  = {200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200};
	HI_U16  au16LutSatuBThFix[HI_ISP_DEMOSAIC_LUT_LEN]  = {92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92};
	HI_S16  as16LutSatuFixEhcY[HI_ISP_DEMOSAIC_LUT_LEN] = {0+256, 0+256, 0+256, 0+256, 0+256, 0+256, 0+256, 0+256, 0+256, 0+256, 0+256, 0+256, 0+256, 0+256, 0+256, 0+256};    //[-256, 256]	
	HI_S32  as32LutVarOffsetLow[HI_ISP_DEMOSAIC_LUT_LEN]= {1000, 1000, 1000, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300};  
	
	HI_U16  au16LutClipDeltaIntpLow[HI_ISP_DEMOSAIC_LUT_LEN]  =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	HI_U16  au16LutClipDeltaIntpRange[HI_ISP_DEMOSAIC_LUT_LEN]=  {0, 0, 10, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20};	

	DEMOSAIC_GET_CTX(IspDev,pstDemosaic);

	memcpy(pstDemosaic->au8LutBldrCrCb,   au8LutBldrCrCb,   sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au8LutBldrGrGb,   au8LutBldrGrGb,   sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au8LutBldrGFStr,  au8LutBldrGFStr,  sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au8LutBldrVarStr, au8LutBldrVarStr, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);	
	memcpy(pstDemosaic->au8LutDithMask, au8LutDithMask, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au8LutShtCtrlGain, au8LutShtCtrlGain, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au8LutMultiMFRed, au8LutMultiMFRed, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au8LutMultiMFBlue, au8LutMultiMFBlue, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au8LutDithMax,     au8LutDithMax, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au8LutChrmGFFix,  au8LutChrmGFFix, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au8LutVarOffsetGain, au8LutVarOffsetGain, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au8LutFilterStrIntp,       au8LutFilterStrIntp, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au8LutClipDeltaGain , au8LutClipDeltaGain, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au8LutClipAdjustMax,au8LutClipAdjustMax, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au8EhcGainLut, au8EhcGainLut, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LENGTH);
	memcpy(pstDemosaic->as8LutFcrGFGain,   as8LutFcrGFGain, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);

	memcpy(pstDemosaic->au16LutClipUSM, au16LutClipUSM, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutChrmGFLow, au16LutChrmGFLow, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutChrmGFHig, au16LutChrmGFHig, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutSatuThLow, au16LutSatuThLow, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutSatuThHig, au16LutSatuThHig, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutGrayThFixLow,  au16LutGrayThFixLow, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutGrayThFixHig, au16LutGrayThFixHig, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutGrayThLow, au16LutGrayThLow, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutGrayThHig, au16LutGrayThHig, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutFcrDetLow, au16LutFcrDetLow, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutFcrLimitLow,  au16LutFcrLimitLow,  sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutFcrLimitHigh, au16LutFcrLimitHigh, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutShtCtrlTh,  au16LutShtCtrlTh,  sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);	
	memcpy(pstDemosaic->au16LutClipBUdSht, au16LutClipBUdSht, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutClipBOvSht, au16LutClipBOvSht, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutClipRUdSht, au16LutClipRUdSht, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutClipROvSht, au16LutClipROvSht, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutSatuRThLow, au16LutSatuRThLow, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutSatuRThHig, au16LutSatuRThHig, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutSatuBThLow, au16LutSatuBThLow, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutSatuBThHig, au16LutSatuBThHig, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutDithRatio,  au16LutDithRatio,  sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutSatuThFix, au16LutSatuThFix, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutSatuRThFix, au16LutSatuRThFix, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutSatuBThFix, au16LutSatuBThFix, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->as32LutVarOffsetLow, as32LutVarOffsetLow, sizeof(HI_S32)*HI_ISP_DEMOSAIC_LUT_LEN);		
	memcpy(pstDemosaic->au16LutClipDeltaIntpRange,     au16LutClipDeltaIntpRange, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	
	memcpy(pstDemosaic->stAuto.au8DetailEnhanceStr,  au8LutMultiMF, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->stAuto.au16NrCoarseStr, au16LutNddmGain, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->stAuto.au16SharpenLumaStr,  as16LutSatuFixEhcY, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->stAuto.au16NoiseSuppressStr,     au16LutClipDeltaIntpLow, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);

	return;
}

static HI_VOID  DemosaicInitFwWdr(ISP_DEV IspDev)
{	
	ISP_DEMOSAIC_S *pstDemosaic = HI_NULL;
	
	HI_U16  au16LutNddmGain[HI_ISP_DEMOSAIC_LUT_LEN]    = {128, 128, 128, 128, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16};
	HI_U8   au8LutBldrCrCb[HI_ISP_DEMOSAIC_LUT_LEN]     = {6, 6, 6, 4, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	HI_U8   au8LutBldrGrGb[HI_ISP_DEMOSAIC_LUT_LEN]     = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
	HI_U8   au8LutBldrGFStr[HI_ISP_DEMOSAIC_LUT_LEN]    = {0, 0, 0, 5, 4, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	HI_U8   au8LutBldrVarStr[HI_ISP_DEMOSAIC_LUT_LEN]   = {6, 6, 6, 5, 4, 2, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1};	
	HI_U8   au8LutMultiMF[HI_ISP_DEMOSAIC_LUT_LEN]      = {20, 20, 20, 10, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	HI_U8   au8LutMultiMFRed[HI_ISP_DEMOSAIC_LUT_LEN]   = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	HI_U8   au8LutMultiMFBlue[HI_ISP_DEMOSAIC_LUT_LEN]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	HI_U8 	au8LutDithMask[HI_ISP_DEMOSAIC_LUT_LEN] 	= {15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15};
	HI_U8 	au8LutDithMax[HI_ISP_DEMOSAIC_LUT_LEN]		= {12, 12, 12, 12, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8};
	HI_S8   as8LutFcrGFGain[HI_ISP_DEMOSAIC_LUT_LEN]     = {3, 3, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	HI_U8   au8LutChrmGFFix[HI_ISP_DEMOSAIC_LUT_LEN]    = {4, 4, 4, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	HI_U8   au8LutShtCtrlGain[HI_ISP_DEMOSAIC_LUT_LEN]  = {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6};	
	HI_U8   au8LutVarOffsetGain[HI_ISP_DEMOSAIC_LUT_LEN]= {32, 32, 32, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16};
	HI_U8   au8LutFilterStrIntp[HI_ISP_DEMOSAIC_LUT_LEN]= {8, 8, 8, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
	HI_U8   au8LutClipDeltaGain[HI_ISP_DEMOSAIC_LUT_LEN]= {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
	HI_U8   au8LutClipAdjustMax[HI_ISP_DEMOSAIC_LUT_LEN]= {40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40};
	HI_U8   au8EhcGainLut[HI_ISP_DEMOSAIC_LUT_LENGTH]   = {255, 220, 200, 180, 164, 128, 96, 80, 72, 64, 36, 36, 24, 24, 16, 16, 16};

	HI_U16  au16LutClipUSM[HI_ISP_DEMOSAIC_LUT_LEN]     = {160, 160, 160, 144, 64, 32, 16, 8, 0, 0, 0, 0, 0, 0, 0, 0};
	HI_U16  au16LutChrmGFLow[HI_ISP_DEMOSAIC_LUT_LEN]   = {130, 130, 130, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140};
	HI_U16  au16LutChrmGFHig[HI_ISP_DEMOSAIC_LUT_LEN]   = {190, 190, 190, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180};
	HI_U16  au16LutSatuThFix[HI_ISP_DEMOSAIC_LUT_LEN]   = {164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164};
	HI_U16  au16LutSatuThLow[HI_ISP_DEMOSAIC_LUT_LEN]   = {130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130};
	HI_U16  au16LutSatuThHig[HI_ISP_DEMOSAIC_LUT_LEN]   = {200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200};
	HI_U16  au16LutGrayThFixLow[HI_ISP_DEMOSAIC_LUT_LEN]= {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32};
	HI_U16  au16LutGrayThFixHig[HI_ISP_DEMOSAIC_LUT_LEN]= {256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256};
	HI_U16  au16LutGrayThLow[HI_ISP_DEMOSAIC_LUT_LEN]   = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	HI_U16  au16LutGrayThHig[HI_ISP_DEMOSAIC_LUT_LEN]   = {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20};
	HI_U16  au16LutFcrDetLow[HI_ISP_DEMOSAIC_LUT_LEN]   = {150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150};
	HI_U16  au16LutFcrLimitLow[HI_ISP_DEMOSAIC_LUT_LEN] = {128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128};
	HI_U16  au16LutFcrLimitHigh[HI_ISP_DEMOSAIC_LUT_LEN]= {256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256};
	HI_U16  au16LutShtCtrlTh[HI_ISP_DEMOSAIC_LUT_LEN]   = {512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512};	
	HI_U16	au16LutDithRatio[HI_ISP_DEMOSAIC_LUT_LEN]	= {128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128};

	HI_U16  au16LutClipBUdSht[HI_ISP_DEMOSAIC_LUT_LEN]  = {1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024};
	HI_U16  au16LutClipBOvSht[HI_ISP_DEMOSAIC_LUT_LEN]  = {1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024};
	HI_U16  au16LutClipRUdSht[HI_ISP_DEMOSAIC_LUT_LEN]  = {1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024};
	HI_U16  au16LutClipROvSht[HI_ISP_DEMOSAIC_LUT_LEN]  = {256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256};
	HI_U16  au16LutSatuRThLow[HI_ISP_DEMOSAIC_LUT_LEN]  = {130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130};
	HI_U16  au16LutSatuRThHig[HI_ISP_DEMOSAIC_LUT_LEN]  = {200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200};
	HI_U16  au16LutSatuRThFix[HI_ISP_DEMOSAIC_LUT_LEN]  = {92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92};
	HI_U16  au16LutSatuBThLow[HI_ISP_DEMOSAIC_LUT_LEN]  = {130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130};
	HI_U16  au16LutSatuBThHig[HI_ISP_DEMOSAIC_LUT_LEN]  = {200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200};
	HI_U16  au16LutSatuBThFix[HI_ISP_DEMOSAIC_LUT_LEN]  = {92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92};
	HI_S16  as16LutSatuFixEhcY[HI_ISP_DEMOSAIC_LUT_LEN] = {64+256, 64+256, 64+256, 64+256, 64+256, 64+256, 64+256, 64+256, 64+256, 64+256, 64+256, 64+256, 64+256, 64+256, 64+256, 64+256};    //[-256, 256]	
	HI_S32  as32LutVarOffsetLow[HI_ISP_DEMOSAIC_LUT_LEN]= {300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300};  
	
	HI_U16  au16LutClipDeltaIntpLow[HI_ISP_DEMOSAIC_LUT_LEN]  =  {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20};
	HI_U16  au16LutClipDeltaIntpRange[HI_ISP_DEMOSAIC_LUT_LEN]=  {10, 10, 10, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20};	

	DEMOSAIC_GET_CTX(IspDev,pstDemosaic);

	memcpy(pstDemosaic->au8LutBldrCrCb,   au8LutBldrCrCb,   sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au8LutBldrGrGb,   au8LutBldrGrGb,   sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au8LutBldrGFStr,  au8LutBldrGFStr,  sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au8LutBldrVarStr, au8LutBldrVarStr, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);	
	memcpy(pstDemosaic->au8LutDithMask, au8LutDithMask, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au8LutShtCtrlGain, au8LutShtCtrlGain, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au8LutMultiMFRed, au8LutMultiMFRed, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au8LutMultiMFBlue, au8LutMultiMFBlue, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au8LutDithMax,     au8LutDithMax, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au8LutChrmGFFix,  au8LutChrmGFFix, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au8LutVarOffsetGain, au8LutVarOffsetGain, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au8LutFilterStrIntp,       au8LutFilterStrIntp, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au8LutClipDeltaGain , au8LutClipDeltaGain, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au8LutClipAdjustMax,au8LutClipAdjustMax, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au8EhcGainLut, au8EhcGainLut, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LENGTH);
	memcpy(pstDemosaic->as8LutFcrGFGain,   as8LutFcrGFGain, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);

	memcpy(pstDemosaic->au16LutClipUSM, au16LutClipUSM, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutChrmGFLow, au16LutChrmGFLow, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutChrmGFHig, au16LutChrmGFHig, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutSatuThLow, au16LutSatuThLow, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutSatuThHig, au16LutSatuThHig, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutGrayThFixLow,  au16LutGrayThFixLow, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutGrayThFixHig, au16LutGrayThFixHig, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutGrayThLow, au16LutGrayThLow, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutGrayThHig, au16LutGrayThHig, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutFcrDetLow, au16LutFcrDetLow, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutFcrLimitLow,  au16LutFcrLimitLow,  sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutFcrLimitHigh, au16LutFcrLimitHigh, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutShtCtrlTh,  au16LutShtCtrlTh,  sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);	
	memcpy(pstDemosaic->au16LutClipBUdSht, au16LutClipBUdSht, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutClipBOvSht, au16LutClipBOvSht, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutClipRUdSht, au16LutClipRUdSht, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutClipROvSht, au16LutClipROvSht, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutSatuRThLow, au16LutSatuRThLow, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutSatuRThHig, au16LutSatuRThHig, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutSatuBThLow, au16LutSatuBThLow, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutSatuBThHig, au16LutSatuBThHig, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutDithRatio,  au16LutDithRatio,  sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutSatuThFix, au16LutSatuThFix, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutSatuRThFix, au16LutSatuRThFix, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->au16LutSatuBThFix, au16LutSatuBThFix, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->as32LutVarOffsetLow, as32LutVarOffsetLow, sizeof(HI_S32)*HI_ISP_DEMOSAIC_LUT_LEN);		
	memcpy(pstDemosaic->au16LutClipDeltaIntpRange,     au16LutClipDeltaIntpRange, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);	
	
	memcpy(pstDemosaic->stAuto.au8DetailEnhanceStr,  au8LutMultiMF, sizeof(HI_U8)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->stAuto.au16NrCoarseStr, au16LutNddmGain, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->stAuto.au16SharpenLumaStr,  as16LutSatuFixEhcY, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);
	memcpy(pstDemosaic->stAuto.au16NoiseSuppressStr,     au16LutClipDeltaIntpLow, sizeof(HI_U16)*HI_ISP_DEMOSAIC_LUT_LEN);	

	return;
}

static HI_VOID DemosaicRegsInitialize(ISP_DEV IspDev)
{
	HI_U32 i, n;
    HI_S16 s16FcrStrengthTemp = HI_ISP_DEMOSAIC_FCR_GAIN_DEFAULT;
    HI_S16 s16FcrThresholdTemp= HI_ISP_DEMOSAIC_FCR_RATIO_DEFAULT;
	HI_U32 u32Width, u32Height;	
	HI_FLOAT n_cur, n_pre, n_fct;
	
	ISP_CTX_S *pstIspCtx = HI_NULL;
	ISP_DEMOSAIC_S *pstDemosaic = HI_NULL;

	HI_FLOAT drc_compensate = 0.6f;

	ISP_GET_CTX(IspDev, pstIspCtx);
    DEMOSAIC_GET_CTX(IspDev, pstDemosaic);
	
	hi_isp_demosaic_cfg_enable_write(IspDev, HI_ISP_DEMOSAIC_CFG_EN_DEFAULT);
	hi_isp_demosaic_ahd_enable_write(IspDev, HI_ISP_DEMOSAIC_AND_EN_DEFAULT);
	hi_isp_nddm_enable_write(IspDev, HI_TRUE);
	hi_isp_nddm_cac_blend_en_write(IspDev, HI_TRUE);
	
	pstDemosaic->u8WdrMode = pstIspCtx->u8SnsWDRMode;

	pstDemosaic->u8BitDepthPrc = HI_DEMOSAIC_BITDEPTH;
	pstDemosaic->u32FcrISOTH1  = HI_DEMOSAIC_FCRISOTH1;
	pstDemosaic->u32FcrISOTH2  = HI_DEMOSAIC_FCRISOTH2;

	pstDemosaic->u8FcrGain    = HI_ISP_DEMOSAIC_FCR_GAIN_DEFAULT;
	pstDemosaic->u16FcrLimit1 = HI_ISP_DEMOSAIC_FCR_LIMIT1_DEFAULT;
	pstDemosaic->u16FcrLimit2 = HI_ISP_DEMOSAIC_FCR_LIMIT2_DEFAULT;
	
    for(i=0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
    {	
	    pstDemosaic->au8FcrAutoStrength[i]   = HI_ISP_DEMOSAIC_FCR_STRENGTH_DEFAULT;	
	    pstDemosaic->au8FcrAutoThreshold[i]  = HI_ISP_DEMOSAIC_FCR_THRESHOLD_DEFAULT;
	    pstDemosaic->au16FcrAutoOffset[i]  = HI_ISP_EXT_FCR_OFFSET_DEFAULT;
	} 	
	pstDemosaic->u8FcrManuStrength  = HI_ISP_DEMOSAIC_FCR_STRENGTH_DEFAULT;	
	pstDemosaic->u8FcrManuThreshold  = HI_ISP_DEMOSAIC_FCR_THRESHOLD_DEFAULT;
	pstDemosaic->u16FcrManuOffset  = HI_ISP_EXT_FCR_OFFSET_DEFAULT;	    
	pstDemosaic->u16FcrOffset  = pstDemosaic->au16FcrAutoOffset[0]; 	    
	pstDemosaic->bFcrEnable = HI_ISP_DEMOSAIC_FCR_EN_MPI;

	hi_isp_demosaic_bld_limit1_write(IspDev, HI_ISP_DEMOSAIC_BLENDLIMIT1_DEFAULT);
	hi_isp_demosaic_bld_limit2_write(IspDev, HI_ISP_DEMOSAIC_BLENDLIMIT2_DEFAULT);
	hi_isp_demosaic_bld_ratio1_write(IspDev, HI_ISP_DEMOSAIC_BLENDRATIO1_DEFAULT);
	hi_isp_demosaic_bld_ratio2_write(IspDev, HI_ISP_DEMOSAIC_BLENDRATIO2_DEFAULT);
	hi_isp_demosaic_hv_ratio_write(IspDev, HI_ISP_DEMOSAIC_HV_RATIO_DEFAULT);
	hi_isp_demosaic_hv_sel_write(IspDev, HI_ISP_DEMOSAIC_HV_SEL_DEFAULT);

	hi_isp_demosaic_ahd_par1_write(IspDev, HI_ISP_DEMOSAIC_AHDPART1_DEFAULT);
	hi_isp_demosaic_ahd_par2_write(IspDev, HI_ISP_DEMOSAIC_AHDPART2_DEFAULT);

	hi_isp_demosaic_fcr_cmax_sel_write(IspDev, HI_ISP_DEMOSAIC_FCR_CMAX_SEL_DEFAULT);
	hi_isp_demosaic_fcr_detg_sel_write(IspDev, HI_ISP_DEMOSAIC_FCR_DETG_SEL_DEFAULT);	
	hi_isp_demosaic_fcr_gray_ratio_write(IspDev, HI_ISP_DEMOSAIC_FCRGRAYRATIO_DEFAULT);	
	
	hi_isp_demosaic_var_blend_th_write(IspDev, HI_ISP_DEMOSAIC_VARBLENDTHR_DEFAULT);
	hi_isp_demosaic_dif_blend_ratio1_write(IspDev, HI_ISP_DEMOSAIC_DIF_BLEND_RATIO1_DEFAULT);
	hi_isp_demosaic_dif_blend_ratio0_write(IspDev, HI_ISP_DEMOSAIC_DIF_BLEND_RATIO0_DEFAULT);
	hi_isp_demosaic_dif_blend_limit1_write(IspDev, HI_ISP_DEMOSAIC_DIF_BLEND_LIMIT1_DEFAULT);
	hi_isp_demosaic_dif_blend_limit0_write(IspDev, HI_ISP_DEMOSAIC_DIF_BLEND_LIMIT0_DEFAULT);
	hi_isp_demosaic_dif2max_res_th_write(IspDev, HI_ISP_DEMOSAIC_DIF2MAX_RES_TH_DEFAULT);

	hi_isp_demosaic_fake_cr_var_thr_high_write(IspDev, HI_ISP_DEMOSAIC_FAKE_CR_THR_HIGH_DEFAULT);
	hi_isp_demosaic_fake_cr_var_thr_low_write(IspDev, HI_ISP_DEMOSAIC_FAKE_CR_THR_LOW_DEFAULT);
	hi_isp_demosaic_cbcr_avg_thld_write(IspDev, HI_ISP_DEMOSAIC_CB_CR_AVG_THR_DEFAULT);
	hi_isp_demosaic_tot_point_thld_write(IspDev, HI_ISP_DEMOSAIC_TOT_POINT_AVG_THR_DEFAULT);
	hi_isp_demosaic_sig_point_thld_write(IspDev, HI_ISP_DEMOSAIC_SIG_POINT_AVG_THR_DEFAULT);
	hi_isp_demosaic_bld_adjust_write(IspDev, HI_ISP_DEMOSAIC_BLEND_ADJUST_DEFAULT);
	hi_isp_demosaic_colorthld_write(IspDev, HI_ISP_DEMOSAIC_COLOR_THR_DEFAULT);
	hi_isp_demosaic_g_gap_thld_write(IspDev, HI_ISP_DEMOSAIC_G_GAP_THR_DEFAULT);
	hi_isp_demosaic_cr_gap_thld_write(IspDev, HI_ISP_DEMOSAIC_CR_GAP_THR_DEFAULT);	
	hi_isp_demosaic_hv_dir_enable_write(IspDev, HI_ISP_DEMOSAIC_HV_DIR_EN_DEFAULT);
	hi_isp_demosaic_mid_filter_enable_write(IspDev, HI_ISP_DEMOSAIC_MID_FILTER_EN_DEFAULT);
	hi_isp_demosaic_de_fake_enable_write(IspDev, HI_ISP_DEMOSAIC_FAKE_EN_DEFAULT);

	hi_isp_demosaic_hf_intp_th_low_write(IspDev, HI_ISP_DEMOSAIC_HF_INTP_TH_LOW_DEFAULT);
	hi_isp_demosaic_hf_intp_th_high_write(IspDev, HI_ISP_DEMOSAIC_HF_INTP_TH_HIGH_DEFAULT);
	hi_isp_demosaic_hf_intp_th_low1_write(IspDev, HI_ISP_DEMOSAIC_HF_INTP_TH_LOW1_LINEAR_DEFAULT);
	hi_isp_demosaic_hf_intp_th_high1_write(IspDev, HI_ISP_DEMOSAIC_HF_INTP_TH_HIGH1_LINEAR_DEFAULT);
	hi_isp_demosaic_hf_intp_bld_low_write(IspDev, HI_ISP_DEMOSAIC_HF_INTP_BLD_LOW_DEFAULT);
	hi_isp_demosaic_hf_intp_bld_high_write(IspDev, HI_ISP_DEMOSAIC_HF_INTP_BLD_HIGH_DEFAULT);
	hi_isp_demosaic_interp_ratio0_write(IspDev, HI_ISP_DEMOSAIC_INTERP_RATIO0_DEFAULT);
	hi_isp_demosaic_interp_ratio1_write(IspDev, HI_ISP_DEMOSAIC_INTERP_RATIO1_DEFAULT);

	hi_isp_nddm_gf_th_high_write(IspDev, HI_ISP_NDDM_GF_TH_HIGH_DEFAULT);
	hi_isp_nddm_gf_th_low_write(IspDev, HI_ISP_NDDM_GF_TH_LOW_DEFAULT);
	hi_isp_nddm_bldr_cbcr_write(IspDev, HI_ISP_NDDM_BLDR_CBCR_DEFAULT);
	hi_isp_nddm_chrm_fix_write(IspDev, HI_ISP_NDDM_CHRM_FIX_DEFAULT);
	hi_isp_nddm_chrm_low_write(IspDev, HI_ISP_NDDM_CHRM_LOW_DEFAULT);
	hi_isp_nddm_chrm_high_write(IspDev, HI_ISP_NDDM_CHRM_HIGH_DEFAULT);
	hi_isp_nddm_bldr_gf_str_write(IspDev, HI_ISP_NDDM_BLDR_GF_STR_DEFAULT);
	hi_isp_nddm_clip_usm_write(IspDev, HI_ISP_NDDM_CLIP_USM_DEFAULT);
	hi_isp_nddm_multi_mf_write(IspDev, HI_ISP_NDDM_MULTI_MF_DEFAULT);
	hi_isp_nddm_satu_th_fix_write(IspDev, HI_ISP_NDDM_SATU_TH_FIX_DEFAULT);
	hi_isp_nddm_satu_th_high_write(IspDev, HI_ISP_NDDM_SATU_TH_HIGH_DEFAULT);
	hi_isp_nddm_satu_th_low_write(IspDev, HI_ISP_NDDM_SATU_TH_LOW_DEFAULT);
	hi_isp_nddm_gray_th_fix_write(IspDev, HI_ISP_NDDM_GRAY_THFIX_LOW_DEFAULT);
	hi_isp_nddm_gray_th_fix2_write(IspDev, HI_ISP_NDDM_GRAY_THFIX_HIGH_DEFAULT);
	hi_isp_nddm_gray_th_high_write(IspDev, HI_ISP_NDDM_GRAY_TH_HIGH_DEFAULT);
	hi_isp_nddm_gray_th_low_write(IspDev, HI_ISP_NDDM_GRAY_TH_LOW_DEFAULT);
	hi_isp_nddm_bldr_gr_gb_write(IspDev, HI_ISP_NDDM_BLDR_GRGB_DEFAULT);
	hi_isp_nddm_fcr_gf_gain_write(IspDev, HI_ISP_NDDM_FCR_GF_GAIN_DEFAULT);
	hi_isp_nddm_fcr_limit_high_write(IspDev, HI_ISP_NDDM_FCR_LIMIT_HIGH_DEFAULT);
	hi_isp_nddm_fcr_limit_low_write(IspDev, HI_ISP_NDDM_FCR_LIMIT_LOW_DEFAULT);
	hi_isp_nddm_scale_write(IspDev, HI_ISP_NDDM_FCR_SCALE_DEFAULT);
	hi_isp_nddm_cac_blend_rate_write(IspDev, HI_ISP_NDDM_CACBLENDRATE_DEFAULT);
	hi_isp_nddm_clip_adjust_max_write(IspDev, HI_ISP_NDDM_CLIP_ADJUST_MAX_DEFAULT);
	hi_isp_nddm_clip_delta_gain_write(IspDev, HI_ISP_NDDM_CLIP_DELTA_GAIN_DEFAULT);
	hi_isp_nddm_clip_delta_intp_low_write(IspDev, HI_ISP_NDDM_CLIP_DELTA_INTP_LOW_DEFAULT);
	hi_isp_nddm_clip_delta_intp_high_write(IspDev, HI_ISP_NDDM_CLIP_DELTA_INTP_HIGH_DEFAULT);
	hi_isp_nddm_filter_str_intp_write(IspDev, HI_ISP_NDDM_FILTER_STR_INTP_DEFAULT);
	hi_isp_nddm_clip_delta_filt_low_write(IspDev, HI_ISP_NDDM_CLIP_DELTA_FILT_LOW_DEFAULT);
	hi_isp_nddm_clip_delta_filt_high_write(IspDev, HI_ISP_NDDM_CLIP_DELTA_FILT_HIGH_DEFAULT);
	hi_isp_nddm_filter_str_filt_write(IspDev, HI_ISP_NDDM_FILTER_STR_FILT_DEFAULT);
	hi_isp_nddm_var_offset_gain_write(IspDev, HI_ISP_NDDM_VAR_OFFSET_GAIN_DEFAULT);
	hi_isp_nddm_var_offset_low_write(IspDev, HI_ISP_NDDM_VAR_OFFSET_LOW_DEFAULT);

	/*False Color*/	        
    s16FcrStrengthTemp   = (HI_S16)(pstDemosaic->au8FcrAutoStrength[0]);
    s16FcrThresholdTemp  = (HI_S16)(pstDemosaic->au8FcrAutoThreshold[0]);
    
    if( s16FcrStrengthTemp > HI_ISP_DEMOSAIC_FCR_GAIN_RANGE )  
	{
		pstDemosaic->u8FcrGain    = HI_ISP_DEMOSAIC_FCR_GAIN_MAX;
	}
    else                                                      
	{
		pstDemosaic->u8FcrGain    = (HI_U8)s16FcrStrengthTemp;
	}
		                	           	        
    s16FcrStrengthTemp-=HI_ISP_DEMOSAIC_FCR_GAIN_RANGE;        
	pstDemosaic->u16FcrLimit1 = HI_ISP_DEMOSAIC_FCR_LIMIT1_DEFAULT ;	
    if( s16FcrStrengthTemp > HI_ISP_DEMOSAIC_FCR_LIMIT1_RANGE)
	{
		pstDemosaic->u16FcrLimit1 = 0;
	}
    else if(s16FcrStrengthTemp > 0)                           
	{
		pstDemosaic->u16FcrLimit1 = (HI_U16)(pstDemosaic->u16FcrLimit1 - s16FcrStrengthTemp);
	}	        

    s16FcrStrengthTemp-=HI_ISP_DEMOSAIC_FCR_LIMIT1_RANGE;       
	pstDemosaic->u16FcrLimit2 = HI_ISP_DEMOSAIC_FCR_LIMIT2_DEFAULT;      
    if(s16FcrStrengthTemp > 0)                                
	{
		pstDemosaic->u16FcrLimit2 = (HI_U16)(pstDemosaic->u16FcrLimit2 + s16FcrStrengthTemp);
	}
	        
    pstDemosaic->u16FcrLimit1 <<= u32FcrShift ;
    pstDemosaic->u16FcrLimit2 <<= u32FcrShift ;                   

    if( s16FcrThresholdTemp > HI_ISP_DEMOSAIC_FCR_RATIO_RANGE )
	{
		pstDemosaic->u8FcrRatio  = HI_ISP_DEMOSAIC_FCR_RATIO_MAX;
	}
    else                                                       
	{
		pstDemosaic->u8FcrRatio  = (HI_U8)s16FcrThresholdTemp;
	}

	s16FcrThresholdTemp-=HI_ISP_DEMOSAIC_FCR_RATIO_RANGE;
	pstDemosaic->u16FcrThr   = HI_ISP_DEMOSAIC_FCR_THR_DEFAULT; 	        
	if(s16FcrThresholdTemp > 0)                          
	{
		pstDemosaic->u16FcrThr   = (HI_U16)(pstDemosaic->u16FcrThr + s16FcrThresholdTemp);
	}   

     	
	if (1 == pstDemosaic->u8WdrMode)
	{
		DemosaicInitFwWdr(IspDev);
		
		for(n = 0; n < HI_ISP_DEMOSAIC_LUT_LENGTH; n++) 						
		{
			n_cur = (HI_FLOAT)(n * 16);
			n_pre = (HI_FLOAT)(256 * pow(n_cur / 256, 1.0f / drc_compensate)) + 0.5f;
			n_fct = (HI_FLOAT)(ISP_SQR(n_cur / DIV_0_TO_1(n_pre)));

			pstDemosaic->afsigma[n]  =  n_cur * n_fct;	
	    }
	}
	else
	{
		DemosaicInitFwLinear(IspDev);
		
		for(n = 0; n < HI_ISP_DEMOSAIC_LUT_LENGTH; n++) 						
		{
			pstDemosaic->afsigma[n]  =  n * 16;
		}
	}

	hi_isp_nddm_usm_lut_waddr_write(IspDev, 0);
	for(i=0; i < HI_ISP_DEMOSAIC_LUT_LENGTH; i++)
	{
		hi_isp_nddm_usm_lut_wdata_write(IspDev, pstDemosaic->au8EhcGainLut[i]);
	}

	hi_isp_nddm_usm_lut_update_write(IspDev, HI_TRUE);

	pstDemosaic->bUpdateEhcLut = HI_FALSE;
	
	if(pstDemosaic->u8BitDepthPrc >= 10)         // adaptive                                             
	{ 
		pstDemosaic->u16FcrLimit1 = pstDemosaic->u16FcrLimit1 << (pstDemosaic->u8BitDepthPrc - 10); // u12
		pstDemosaic->u16FcrLimit2 = pstDemosaic->u16FcrLimit2 << (pstDemosaic->u8BitDepthPrc - 10); // u12
	}
	else
	{
		pstDemosaic->u16FcrLimit1 = pstDemosaic->u16FcrLimit1 >> (10 - pstDemosaic->u8BitDepthPrc);   
		pstDemosaic->u16FcrLimit2 = pstDemosaic->u16FcrLimit2 >> (10 - pstDemosaic->u8BitDepthPrc);   
	}

	hi_isp_demosaic_fcr_limit1_write(IspDev, pstDemosaic->u16FcrLimit1);
	hi_isp_demosaic_fcr_limit2_write(IspDev, pstDemosaic->u16FcrLimit2);
	hi_isp_demosaic_fcr_gain_write(IspDev, pstDemosaic->u8FcrGain);
	hi_isp_demosaic_fcr_ratio_write(IspDev, pstDemosaic->u8FcrRatio);	
	hi_isp_demosaic_fcr_thr_write(IspDev, pstDemosaic->u16FcrThr);
	hi_isp_nddm_fcr_det_low_write(IspDev, pstDemosaic->u16FcrOffset);
		
	u32Width = hi_ext_sync_total_width_read();
	u32Height = hi_ext_sync_total_height_read();
	
	hi_isp_demosaic_width_write(IspDev, u32Width-1);
	hi_isp_demosaic_height_write(IspDev, u32Height-1);

    return;
}

static HI_S32 DemosaicReadExtregs(ISP_DEV IspDev)
{   
	ISP_DEMOSAIC_S *pstDemosaic = HI_NULL;
	HI_U8 i;
    DEMOSAIC_GET_CTX(IspDev, pstDemosaic);

    pstDemosaic->bFcrEnable = hi_ext_system_fcr_enable_read();
	pstDemosaic->bEnable = hi_ext_system_demosaic_enable_read();
	pstDemosaic->enOpType = hi_ext_system_demosaic_manual_mode_read();
	pstDemosaic->enOpTypeFcr = hi_ext_system_fcr_manual_mode_read();
	
	if(OP_TYPE_AUTO == pstDemosaic->enOpTypeFcr)
	{
		for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
		{	
		    pstDemosaic->au8FcrAutoStrength[i]   = hi_ext_system_auto_fcr_strength_read(i);	
		    pstDemosaic->au8FcrAutoThreshold[i]  = hi_ext_system_auto_fcr_threshold_read(i);
		    pstDemosaic->au16FcrAutoOffset[i]  = hi_ext_system_auto_fcr_offset_read(i);   
		}
	}
    else if(OP_TYPE_MANUAL == pstDemosaic->enOpTypeFcr)
    {
	    pstDemosaic->u8FcrManuStrength   = hi_ext_system_manual_fcr_strength_read();	
	    pstDemosaic->u8FcrManuThreshold  = hi_ext_system_manual_fcr_threshold_read();
	    pstDemosaic->u16FcrManuOffset  = hi_ext_system_manual_fcr_offset_read();
	}
	
	if(OP_TYPE_AUTO == pstDemosaic->enOpType)
	{
		for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
		{	
			pstDemosaic->stAuto.au16EdgeSmoothThr[i] = hi_ext_system_demosaic_auto_edge_smooth_thresh_read(i);
			pstDemosaic->stAuto.au16EdgeSmoothSlope[i] = hi_ext_system_demosaic_auto_edge_smooth_slope_read(i);
			pstDemosaic->stAuto.au16AntiAliasThr[i] = hi_ext_system_demosaic_auto_antialias_thresh_read(i);
			pstDemosaic->stAuto.au16AntiAliasSlope[i] = hi_ext_system_demosaic_auto_antialias_slope_read(i);

			pstDemosaic->stAuto.au16NrCoarseStr[i]       = hi_ext_system_demosaic_auto_gain_read(i); //au16LutNddmGain
			pstDemosaic->stAuto.au16NoiseSuppressStr[i]   = hi_ext_system_demosaic_auto_noise_suppress_strength_read(i); //au16LutClipDeltaIntpLow
			pstDemosaic->stAuto.au8DetailEnhanceStr[i]   = hi_ext_system_demosaic_auto_detail_enhance_read(i);//au8LutMultiMF
			pstDemosaic->stAuto.au16SharpenLumaStr[i]  = hi_ext_system_demosaic_auto_satufixehcy_lut_read(i);//as16LutSatuFixEhcY
			pstDemosaic->stAuto.au16ColorNoiseCtrlThr[i]  = hi_ext_system_demosaic_auto_colornoise_ctrl_thresh_read(i);//au16ColorNoiseCtrlThr
		}
	}
    else if(OP_TYPE_MANUAL == pstDemosaic->enOpType)
    {
		pstDemosaic->stManual.u16EdgeSmoothThr = hi_ext_system_demosaic_manual_edge_smooth_thresh_read();
		pstDemosaic->stManual.u16EdgeSmoothSlope = hi_ext_system_demosaic_manual_edge_smooth_slope_read();
		pstDemosaic->stManual.u16AntiAliasThr = hi_ext_system_demosaic_manual_antialias_thresh_read();
		pstDemosaic->stManual.u16AntiAliasSlope = hi_ext_system_demosaic_manual_antialias_slope_read();

		pstDemosaic->stManual.u16NrCoarseStr       = hi_ext_system_demosaic_manual_gain_read();
		pstDemosaic->stManual.u16NoiseSuppressStr  = hi_ext_system_demosaic_manual_noise_suppress_strength_read();
		pstDemosaic->stManual.u8DetailEnhanceStr  = hi_ext_system_demosaic_manual_detail_enhance_read();
		pstDemosaic->stManual.u16SharpenLumaStr  = hi_ext_system_demosaic_manual_satufixehcy_read();
		pstDemosaic->stManual.u16ColorNoiseCtrlThr= hi_ext_system_demosaic_manual_colornoise_ctrl_thresh_read();
	}	
    return 0;
}

HI_S32 DemosaicProcWrite(ISP_DEV IspDev, ISP_CTRL_PROC_WRITE_S *pstProc)
{
    ISP_CTRL_PROC_WRITE_S stProcTmp;

	ISP_DEMOSAIC_S *pstDemosaic = HI_NULL;
	DEMOSAIC_GET_CTX(IspDev, pstDemosaic);
 
    if ((HI_NULL == pstProc->pcProcBuff)
        || (0 == pstProc->u32BuffLen))
    {
        return HI_FAILURE;
    }

    stProcTmp.pcProcBuff = pstProc->pcProcBuff;
    stProcTmp.u32BuffLen = pstProc->u32BuffLen;

	DemosaicReadExtregs(IspDev);
    
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "-----DEMOSAIC INFO-------------------------------------------------------------\n");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
            "%16s"       "%32s"           "%32s"            "%32s\n",      
            "Enable", "EdgeSmoothThr", "AntiAliasThr", "DetailEnhanceStr");
	
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
            "%16u" "%32u"  "%32u"  "%32u\n", 
            pstDemosaic->bEnable,
            (HI_U16)hi_isp_demosaic_hf_intp_th_low_read(IspDev), 
            (HI_U16)hi_isp_demosaic_hf_intp_th_low1_read(IspDev), 
		    (HI_U16)hi_isp_nddm_multi_mf_read(IspDev)
            );

    pstProc->u32WriteLen += 1;
    
    return HI_SUCCESS;
}


HI_S32 ISP_DemosaicInit(ISP_DEV IspDev)
{
	ISP_DEMOSAIC_S *pstDemosaic = HI_NULL;

    DEMOSAIC_GET_CTX(IspDev, pstDemosaic);
	
    DemosaicRegsInitialize(IspDev);
    DemosaicExtRegsInitialize(IspDev);

	pstDemosaic->bInit = HI_TRUE;
    return HI_SUCCESS;
}

static HI_U8 DemosaicGetIsoIndex(HI_U32 u32Iso)
{   
	HI_U8 u8Index;
	
    for (u8Index = 1; u8Index < HI_ISP_DEMOSAIC_LUT_LEN-1; u8Index++)
    {
        if (u32Iso <= au32DemosaicIsoLut[u8Index])
        {
            break;
        }
    }
	
    return u8Index;
}

HI_U32 DemosaicGetValueFromLut (HI_U32 u32IsoLevel , HI_S32 s32Y2, HI_S32 s32Y1, HI_S32 s32X2, HI_S32 s32X1, HI_S32 s32Iso)
{		
	HI_U32 u32Offset;
    
	if(s32X1==s32X2)
	{
		u32Offset = s32Y2;
	}
	else if(s32Y1 <= s32Y2)
	{
		u32Offset = s32Y1+(ABS(s32Y2 - s32Y1) * ABS(s32Iso - s32X1)+ ABS((s32X2 - s32X1)/2))/ABS((s32X2 - s32X1));
	}
	else if(s32Y1> s32Y2)
	{
		u32Offset = s32Y1-(ABS(s32Y2 - s32Y1) * ABS(s32Iso - s32X1)+ ABS((s32X2 - s32X1)/2))/ABS((s32X2 - s32X1));
	}
	return u32Offset;
}

#define  EPS (0.000001f)
#define  COL_ISO      0
#define  COL_K        1
static HI_FLOAT getAlphafromNoiseLut(HI_FLOAT (*pRecord)[2], HI_U16 recordNum, HI_S32 iso)
{
    HI_S32 i = 0;
    HI_FLOAT  y_diff, x_diff;
    HI_FLOAT k = 0.0f;
	
    // record: iso - k
    if(iso <= pRecord[0][COL_ISO])
	{
		k = pRecord[0][COL_K];
        //return k;
	}
	
	if(iso >= pRecord[recordNum - 1][COL_ISO])
	{
		k = pRecord[recordNum - 1][COL_K];
        //return k;
	}
	
	for(i = 0; i < recordNum - 1; i++)
	{
		if(iso >= pRecord[i][COL_ISO] && iso <= pRecord[i + 1][COL_ISO])
		{
			x_diff = pRecord[i + 1][COL_ISO] - pRecord[i][COL_ISO];  // iso diff
			y_diff = pRecord[i + 1][COL_K]  - pRecord[i][COL_K];     // k diff
		    k = pRecord[i][COL_K] + y_diff * (iso - pRecord[i][COL_ISO]) / DIV_0_TO_1(x_diff + EPS);
			//return k;
		}
	}
	return k;
}

HI_S32 Demosaic_GFBlurLut(ISP_DEMOSAIC_S *pstDemosaic, ISP_DEV IspDev, ISP_DEMOSAIC_REG_CFG_S *pstDmCfg, HI_U16 u16NddmStrength, HI_U32 u32Iso)
{	
	HI_U8  n = 0;
	HI_U32 u32sigma;
	HI_FLOAT alpha, sigma; 
	ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
	ISP_SensorGetDefault(IspDev, &pstSnsDft);
	HI_U32   bitScale   =  ISP_BITFIX(pstDemosaic->u8BitDepthPrc - 8);

	alpha = getAlphafromNoiseLut(pstSnsDft->stBayerNr.afCalibrationCoef, pstSnsDft->stBayerNr.u16CalibrationLutNum, u32Iso);

	for(n = 0; n < HI_ISP_DEMOSAIC_LUT_LENGTH; n++)                         
	{
		sigma = pstDemosaic->afsigma[n] * alpha;				
		sigma *=  u16NddmStrength;
		sigma  =  sqrtf((sigma));
		u32sigma  =  ((HI_U32)(sigma *bitScale)) >> 2;
		pstDmCfg->auGFBlurLut[n] = CLIP_MAX(u32sigma, ISP_BITMASK(pstDemosaic->u8BitDepthPrc));    
	}

	pstDmCfg->auGFBlurLut[0] = pstDmCfg->auGFBlurLut[3];
	pstDmCfg->auGFBlurLut[1] = pstDmCfg->auGFBlurLut[3];
	pstDmCfg->auGFBlurLut[2] = pstDmCfg->auGFBlurLut[3];
	pstDmCfg->bUpdateGF = HI_TRUE;

	return HI_SUCCESS;
}

HI_S32 Demosaic_HFIntp(ISP_DEMOSAIC_REG_CFG_S *pstDmCfg, ISP_DEV IspDev)
{
	HI_U16 u16hfIntpBldHig, u16hfIntpBldLow;
	
	u16hfIntpBldLow = hi_isp_demosaic_hf_intp_bld_low_read(IspDev);
	u16hfIntpBldHig = hi_isp_demosaic_hf_intp_bld_high_read(IspDev);
	
	if (pstDmCfg->u16hfIntpThHig == pstDmCfg->u16hfIntpThLow)
	{
		pstDmCfg->u16hfIntpThHig = pstDmCfg->u16hfIntpThLow + 1;
	}
	pstDmCfg->u32hfIntpRatio = (u16hfIntpBldHig - u16hfIntpBldLow) * 128 / HI_DEMOSAIC_MAX((pstDmCfg->u16hfIntpThHig - pstDmCfg->u16hfIntpThLow), 1);
	
	if(pstDmCfg->u16hfIntpThLow1 <= pstDmCfg->u16hfIntpThHig)
	{
		pstDmCfg->u16hfIntpThLow1 = pstDmCfg->u16hfIntpThHig;
	}
	if (pstDmCfg->u16hfIntpThHig1 == pstDmCfg->u16hfIntpThLow1)
	{
		pstDmCfg->u16hfIntpThHig1 = pstDmCfg->u16hfIntpThLow1 + 1;
	}
	pstDmCfg->u32hfIntpRatio1 = (u16hfIntpBldHig - u16hfIntpBldLow) * 128 / HI_DEMOSAIC_MAX((pstDmCfg->u16hfIntpThHig1 - pstDmCfg->u16hfIntpThLow1), 1);
	
	return HI_SUCCESS;
}

HI_S32 ISP_FCRFw(ISP_DEMOSAIC_REG_CFG_S *pstDmCfg, ISP_DEMOSAIC_S *pstDemosaic, HI_U32 u32IsoLevel, HI_U32 u32ISO2, HI_U32 u32ISO1, HI_U32 u32Iso)
{
	HI_S32 s32Y1, s32Y2;		
	HI_S16 s16FcrStrengthTemp,s16FcrThresholdTemp ;

	if(hi_ext_system_fcr_enable_read()) 
	{ 
		pstDmCfg->bFcrEnable      =  1;
		if (OP_TYPE_AUTO == hi_ext_system_fcr_manual_mode_read())
		{	
			
			s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au8FcrAutoStrength[u32IsoLevel-1]:(HI_S32)pstDemosaic->au8FcrAutoStrength[0];		
			s32Y2 = 			  (HI_S32)pstDemosaic->au8FcrAutoStrength[u32IsoLevel];	
			s16FcrStrengthTemp  = (HI_S16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

			s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au8FcrAutoThreshold[u32IsoLevel-1]:(HI_S32)pstDemosaic->au8FcrAutoThreshold[0];		
			s32Y2 = 			  (HI_S32)pstDemosaic->au8FcrAutoThreshold[u32IsoLevel];	
			s16FcrThresholdTemp = (HI_S16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

			s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au16FcrAutoOffset[u32IsoLevel-1]:(HI_S32)pstDemosaic->au16FcrAutoOffset[0];		
			s32Y2 = 			  (HI_S32)pstDemosaic->au16FcrAutoOffset[u32IsoLevel];	
			pstDemosaic->u16FcrOffset 	    = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);					 		       		    
		}
		else
		{
		    s16FcrStrengthTemp   =  (HI_S16)pstDemosaic->u8FcrManuStrength;            	    
		    s16FcrThresholdTemp  =  (HI_S16)pstDemosaic->u8FcrManuThreshold;  
		    pstDemosaic->u16FcrOffset  =  (HI_S16)pstDemosaic->u16FcrManuOffset; 		    
		}
            
		if(s16FcrStrengthTemp > HI_ISP_DEMOSAIC_FCR_GAIN_RANGE )
		{ 
			pstDmCfg->u8FcrGain = HI_ISP_DEMOSAIC_FCR_GAIN_MAX ;
		}
		else 
		{
			pstDmCfg->u8FcrGain = (HI_U8)s16FcrStrengthTemp;
		}
		
		s16FcrStrengthTemp-=HI_ISP_DEMOSAIC_FCR_GAIN_RANGE;
		pstDmCfg->u16FcrLimit1 = HI_ISP_DEMOSAIC_FCR_LIMIT1_DEFAULT;				  
		if(s16FcrStrengthTemp > HI_ISP_DEMOSAIC_FCR_LIMIT1_RANGE)
		{ 
			pstDmCfg->u16FcrLimit1 = 0	  ;
		}
		else if(s16FcrStrengthTemp > 0) 
		{ 
			pstDmCfg->u16FcrLimit1 = (HI_U16)(pstDmCfg->u16FcrLimit1 - s16FcrStrengthTemp) ;
		}			
	
		s16FcrStrengthTemp-=HI_ISP_DEMOSAIC_FCR_LIMIT1_RANGE;
		pstDmCfg->u16FcrLimit2 = HI_ISP_DEMOSAIC_FCR_LIMIT2_DEFAULT;	  
		if(s16FcrStrengthTemp > 0)
		{ 
			pstDmCfg->u16FcrLimit2 = (HI_U16)(pstDmCfg->u16FcrLimit2 + s16FcrStrengthTemp) ;
		}
					
		pstDmCfg->u16FcrLimit1 <<= u32FcrShift ;
		pstDmCfg->u16FcrLimit2 <<= u32FcrShift ;				   
		
		if( s16FcrThresholdTemp > HI_ISP_DEMOSAIC_FCR_RATIO_RANGE )
		{
			pstDmCfg->u8FcrRatio  = HI_ISP_DEMOSAIC_FCR_RATIO_MAX	 ;
		}
		else  
		{
			pstDmCfg->u8FcrRatio  = (HI_U8)s16FcrThresholdTemp	;
		}
					
		s16FcrThresholdTemp-=HI_ISP_DEMOSAIC_FCR_RATIO_RANGE; 
		pstDmCfg->u16FcrThr   = HI_ISP_DEMOSAIC_FCR_THR_DEFAULT ;			
		if(s16FcrThresholdTemp > 0)  
		{ 
			pstDmCfg->u16FcrThr   = (HI_U16)(pstDmCfg->u16FcrThr + s16FcrThresholdTemp);
		} 
	}
	else
	{
		pstDmCfg->u16FcrThr=0;
	}

	pstDmCfg->u16FcrDetLow = HI_DEMOSAIC_MIN(pstDemosaic->u16FcrOffset,4095);
	return HI_SUCCESS;
}

HI_S32 DemosaicCfg(ISP_DEMOSAIC_REG_CFG_S *pstDmCfg, ISP_DEMOSAIC_S *pstDemosaic, HI_U32 u32IsoLevel, HI_U32 u32ISO2, HI_U32 u32ISO1, HI_U32 u32Iso)
{
	HI_S32 s32Y1, s32Y2;
	
	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au8LutBldrCrCb[u32IsoLevel-1]:(HI_S32)pstDemosaic->au8LutBldrCrCb[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au8LutBldrCrCb[u32IsoLevel];	
    pstDmCfg->u8BldrCrCb      = (HI_U8)DemosaicGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au8LutBldrGrGb[u32IsoLevel-1]:(HI_S32)pstDemosaic->au8LutBldrGrGb[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au8LutBldrGrGb[u32IsoLevel];	
	pstDmCfg->u8BldrGbGr       = (HI_U8)DemosaicGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au8LutBldrGFStr[u32IsoLevel-1]:(HI_S32)pstDemosaic->au8LutBldrGFStr[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au8LutBldrGFStr[u32IsoLevel];	
    pstDmCfg->u8BldrGFStr      = (HI_U8)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au8LutBldrVarStr[u32IsoLevel-1]:(HI_S32)pstDemosaic->au8LutBldrVarStr[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au8LutBldrVarStr[u32IsoLevel];	
	pstDmCfg->u8BldrVarStr     = (HI_U8)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1, u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au8LutMultiMFRed[u32IsoLevel-1]:(HI_S32)pstDemosaic->au8LutMultiMFRed[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au8LutMultiMFRed[u32IsoLevel];	
	pstDmCfg->u8MultiMFRed     = (HI_U8)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1,u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au8LutMultiMFBlue[u32IsoLevel-1]:(HI_S32)pstDemosaic->au8LutMultiMFBlue[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au8LutMultiMFBlue[u32IsoLevel];	
	pstDmCfg->u8MultiMFBlue    = (HI_U8)DemosaicGetValueFromLut(u32IsoLevel, s32Y2, s32Y1,u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au8LutDithMask[u32IsoLevel-1]:(HI_S32)pstDemosaic->au8LutDithMask[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au8LutDithMask[u32IsoLevel];	
	pstDmCfg->u8DitherMask     = (HI_U8)DemosaicGetValueFromLut(u32IsoLevel, s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au16LutDithRatio[u32IsoLevel-1]:(HI_S32)pstDemosaic->au16LutDithRatio[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au16LutDithRatio[u32IsoLevel];	
	pstDmCfg->u8DitherRatio    = (HI_U8)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au8LutDithMax[u32IsoLevel-1]:(HI_S32)pstDemosaic->au8LutDithMax[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au8LutDithMax[u32IsoLevel];	
	pstDmCfg->u8DitherMax      = (HI_U8)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au16LutClipUSM[u32IsoLevel-1]:(HI_S32)pstDemosaic->au16LutClipUSM[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au16LutClipUSM[u32IsoLevel];	
	pstDmCfg->u16ClipUSM       = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au8LutChrmGFFix[u32IsoLevel-1]:(HI_S32)pstDemosaic->au8LutChrmGFFix[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au8LutChrmGFFix[u32IsoLevel];	
    pstDmCfg->u8ChrmGFFix      = (HI_U8)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au16LutChrmGFLow[u32IsoLevel-1]:(HI_S32)pstDemosaic->au16LutChrmGFLow[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au16LutChrmGFLow[u32IsoLevel];	
	pstDmCfg->u16ChrmGFLow     = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au16LutChrmGFHig[u32IsoLevel-1]:(HI_S32)pstDemosaic->au16LutChrmGFHig[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au16LutChrmGFHig[u32IsoLevel];	
	pstDmCfg->u16ChrmGFHig     = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au16LutSatuThFix[u32IsoLevel-1]:(HI_S32)pstDemosaic->au16LutSatuThFix[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au16LutSatuThFix[u32IsoLevel];	
	pstDmCfg->u16SatuThFix     = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au16LutSatuThLow[u32IsoLevel-1]:(HI_S32)pstDemosaic->au16LutSatuThLow[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au16LutSatuThLow[u32IsoLevel];	
	pstDmCfg->u16SatuThLow     = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au16LutSatuThHig[u32IsoLevel-1]:(HI_S32)pstDemosaic->au16LutSatuThHig[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au16LutSatuThHig[u32IsoLevel];	
	pstDmCfg->u16SatuThHig     = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au16LutGrayThFixLow[u32IsoLevel-1]:(HI_S32)pstDemosaic->au16LutGrayThFixLow[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au16LutGrayThFixLow[u32IsoLevel];	
	pstDmCfg->u16GrayThFixLow  = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au16LutGrayThFixHig[u32IsoLevel-1]:(HI_S32)pstDemosaic->au16LutGrayThFixHig[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au16LutGrayThFixHig[u32IsoLevel];	
	pstDmCfg->u16GrayThFixHigh = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au16LutGrayThLow[u32IsoLevel-1]:(HI_S32)pstDemosaic->au16LutGrayThLow[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au16LutGrayThLow[u32IsoLevel];	
	pstDmCfg->u16GrayThLow     = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au16LutGrayThHig[u32IsoLevel-1]:(HI_S32)pstDemosaic->au16LutGrayThHig[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au16LutGrayThHig[u32IsoLevel];	
	pstDmCfg->u16GrayThHig     = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au16LutFcrDetLow[u32IsoLevel-1]:(HI_S32)pstDemosaic->au16LutFcrDetLow[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au16LutFcrDetLow[u32IsoLevel];	
	pstDmCfg->u16FcrDetLow     = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au16LutFcrLimitLow[u32IsoLevel-1]:(HI_S32)pstDemosaic->au16LutFcrLimitLow[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au16LutFcrLimitLow[u32IsoLevel];	
	pstDmCfg->u16FcrLimitLow   = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au16LutFcrLimitHigh[u32IsoLevel-1]:(HI_S32)pstDemosaic->au16LutFcrLimitHigh[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au16LutFcrLimitHigh[u32IsoLevel];	
	pstDmCfg->u16FcrLimitHigh  = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au16LutShtCtrlTh[u32IsoLevel-1]:(HI_S32)pstDemosaic->au16LutShtCtrlTh[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au16LutShtCtrlTh[u32IsoLevel];	
	pstDmCfg->u16ShtCtrlTh     = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au8LutShtCtrlGain[u32IsoLevel-1]:(HI_S32)pstDemosaic->au8LutShtCtrlGain[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au8LutShtCtrlGain[u32IsoLevel];	
	pstDmCfg->u8ShtCtrlGain    = (HI_U8)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au8LutClipDeltaGain[u32IsoLevel-1]:(HI_S32)pstDemosaic->au8LutClipDeltaGain[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au8LutClipDeltaGain[u32IsoLevel];	
	pstDmCfg->u8ClipDeltaGain  = (HI_U8)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au16LutClipBUdSht[u32IsoLevel-1]:(HI_S32)pstDemosaic->au16LutClipBUdSht[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au16LutClipBUdSht[u32IsoLevel];	
	pstDmCfg->u16ClipBUdSht    = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au16LutClipBOvSht[u32IsoLevel-1]:(HI_S32)pstDemosaic->au16LutClipBOvSht[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au16LutClipBOvSht[u32IsoLevel];	
	pstDmCfg->u16ClipBOvSht    = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au16LutClipRUdSht[u32IsoLevel-1]:(HI_S32)pstDemosaic->au16LutClipRUdSht[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au16LutClipRUdSht[u32IsoLevel];	
	pstDmCfg->u16ClipRUdSht    = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au16LutClipROvSht[u32IsoLevel-1]:(HI_S32)pstDemosaic->au16LutClipROvSht[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au16LutClipROvSht[u32IsoLevel];	
	pstDmCfg->u16ClipROvSht    = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au16LutSatuRThFix[u32IsoLevel-1]:(HI_S32)pstDemosaic->au16LutSatuRThFix[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au16LutSatuRThFix[u32IsoLevel];	
	pstDmCfg->u16SatuRThFix    = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au16LutSatuRThLow[u32IsoLevel-1]:(HI_S32)pstDemosaic->au16LutSatuRThLow[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au16LutSatuRThLow[u32IsoLevel];	
	pstDmCfg->u16SatuRThLow    = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au16LutSatuRThHig[u32IsoLevel-1]:(HI_S32)pstDemosaic->au16LutSatuRThHig[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au16LutSatuRThHig[u32IsoLevel];	
	pstDmCfg->u16SatuRThHig    = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au16LutSatuBThFix[u32IsoLevel-1]:(HI_S32)pstDemosaic->au16LutSatuBThFix[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au16LutSatuBThFix[u32IsoLevel];	
	pstDmCfg->u16satuBThFix    = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au16LutSatuBThLow[u32IsoLevel-1]:(HI_S32)pstDemosaic->au16LutSatuBThLow[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au16LutSatuBThLow[u32IsoLevel];	
	pstDmCfg->u16SatuBThLow    = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au16LutSatuBThHig[u32IsoLevel-1]:(HI_S32)pstDemosaic->au16LutSatuBThHig[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au16LutSatuBThHig[u32IsoLevel];	
	pstDmCfg->u16SatuBThHig    = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->as32LutVarOffsetLow[u32IsoLevel-1]:(HI_S32)pstDemosaic->as32LutVarOffsetLow[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->as32LutVarOffsetLow[u32IsoLevel];	
	pstDmCfg->u32VarOffsetLow  = (HI_U32)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au8LutVarOffsetGain[u32IsoLevel-1]:(HI_S32)pstDemosaic->au8LutVarOffsetGain[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au8LutVarOffsetGain[u32IsoLevel];	
	pstDmCfg->u8VarOffsetGain  = (HI_U8)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au8LutFilterStrIntp[u32IsoLevel-1]:(HI_S32)pstDemosaic->au8LutFilterStrIntp[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au8LutFilterStrIntp[u32IsoLevel];	
	pstDmCfg->u8FilterStrIntp  = (HI_U8)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);
    pstDmCfg->u8FilterStrFilt  = pstDmCfg->u8FilterStrIntp;	

	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au8LutClipAdjustMax[u32IsoLevel-1]:(HI_S32)pstDemosaic->au8LutClipAdjustMax[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au8LutClipAdjustMax[u32IsoLevel];	
	pstDmCfg->u8ClipAdjustMax  = (HI_U8)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);
	
    return HI_SUCCESS;
}

HI_S32 ISP_DemosaicFw(HI_U32 u32Iso, ISP_DEV IspDev, ISP_DEMOSAIC_REG_CFG_S *pstDmCfg)
{
	HI_U16 u16LutClipDeltaIntpRange;
	HI_U16 u16NddmStrength = 16;	

	ISP_DEMOSAIC_S *pstDemosaic = HI_NULL;
	DEMOSAIC_GET_CTX(IspDev, pstDemosaic);

    HI_U32 u32IsoLevel;
	HI_U32 u32ISO1 = 0;
    HI_U32 u32ISO2 = 0;
	HI_S32 s32Y1, s32Y2;
	
	if (u32Iso > au32DemosaicIsoLut[HI_ISP_DEMOSAIC_LUT_LEN-1])
	{
	 u32IsoLevel = HI_ISP_DEMOSAIC_LUT_LEN-1;
	 u32ISO1 = au32DemosaicIsoLut[HI_ISP_DEMOSAIC_LUT_LEN-1];
	 u32ISO2 = au32DemosaicIsoLut[HI_ISP_DEMOSAIC_LUT_LEN-1];	 
	}
    else if (u32Iso <= au32DemosaicIsoLut[0])
	{
	    u32IsoLevel = 0;
	    u32ISO1 = 0;
	    u32ISO2 = au32DemosaicIsoLut[0]; 		
	}		
    else
    {
        u32IsoLevel = DemosaicGetIsoIndex(u32Iso);
        u32ISO1 = au32DemosaicIsoLut[u32IsoLevel-1];
        u32ISO2 = au32DemosaicIsoLut[u32IsoLevel]; 
    }

    DEMOSAIC_GET_CTX(IspDev, pstDemosaic);

    if(0 == hi_ext_system_demosaic_enable_read())
    {
       hi_isp_demosaic_cfg_enable_write(IspDev, 0);
	   hi_isp_nddm_enable_write(IspDev, 0);
	}
	else if(1 == hi_ext_system_demosaic_enable_read())
	{
		hi_isp_demosaic_cfg_enable_write(IspDev, 1);
		hi_isp_nddm_enable_write(IspDev, 1);
	}

    DemosaicCfg(pstDmCfg, pstDemosaic, u32IsoLevel, u32ISO2, u32ISO1, u32Iso);
	
	s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->au16LutClipDeltaIntpRange[u32IsoLevel-1]:(HI_S32)pstDemosaic->au16LutClipDeltaIntpRange[0];		
	s32Y2 = 			  (HI_S32)pstDemosaic->au16LutClipDeltaIntpRange[u32IsoLevel];	
	u16LutClipDeltaIntpRange   = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

	if(OP_TYPE_AUTO == pstDemosaic->enOpType)
	{
		s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->stAuto.au16NrCoarseStr[u32IsoLevel-1]:(HI_S32)pstDemosaic->stAuto.au16NrCoarseStr[0];		
		s32Y2 = 			  (HI_S32)pstDemosaic->stAuto.au16NrCoarseStr[u32IsoLevel];	
		u16NddmStrength           = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

		s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->stAuto.au8DetailEnhanceStr[u32IsoLevel-1]:(HI_S32)pstDemosaic->stAuto.au8DetailEnhanceStr[0];		
		s32Y2 = 			  (HI_S32)pstDemosaic->stAuto.au8DetailEnhanceStr[u32IsoLevel];	
		pstDmCfg->u8MultiMF       = (HI_U8)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);

		s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->stAuto.au16SharpenLumaStr[u32IsoLevel-1]:(HI_S32)pstDemosaic->stAuto.au16SharpenLumaStr[0];		
		s32Y2 = 			  (HI_S32)pstDemosaic->stAuto.au16SharpenLumaStr[u32IsoLevel];	
		pstDmCfg->s16SatuFixEhcY  = (HI_S16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);
		pstDmCfg->s16SatuFixEhcY  -= 256;

		s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->stAuto.au16EdgeSmoothThr[u32IsoLevel-1]:(HI_S32)pstDemosaic->stAuto.au16EdgeSmoothThr[0];		
		s32Y2 = 			  (HI_S32)pstDemosaic->stAuto.au16EdgeSmoothThr[u32IsoLevel];	
		pstDmCfg->u16hfIntpThLow  = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);
		pstDmCfg->u16hfIntpThLow = HI_DEMOSAIC_MIN(pstDmCfg->u16hfIntpThLow,1022);

		s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->stAuto.au16EdgeSmoothSlope[u32IsoLevel-1]:(HI_S32)pstDemosaic->stAuto.au16EdgeSmoothSlope[0];		
		s32Y2 = 			  (HI_S32)pstDemosaic->stAuto.au16EdgeSmoothSlope[u32IsoLevel];	
		pstDmCfg->u16hfIntpThHig  = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);
        pstDmCfg->u16hfIntpThHig  = pstDmCfg->u16hfIntpThHig + pstDmCfg->u16hfIntpThLow;
		pstDmCfg->u16hfIntpThHig = HI_DEMOSAIC_MIN(pstDmCfg->u16hfIntpThHig,1023);

		s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->stAuto.au16AntiAliasThr[u32IsoLevel-1]:(HI_S32)pstDemosaic->stAuto.au16AntiAliasThr[0];		
		s32Y2 = 			  (HI_S32)pstDemosaic->stAuto.au16AntiAliasThr[u32IsoLevel];	
		pstDmCfg->u16hfIntpThLow1 = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);
        pstDmCfg->u16hfIntpThLow1 = HI_DEMOSAIC_MIN(pstDmCfg->u16hfIntpThLow1,1022);

		s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->stAuto.au16AntiAliasSlope[u32IsoLevel-1]:(HI_S32)pstDemosaic->stAuto.au16AntiAliasSlope[0];		
		s32Y2 = 			  (HI_S32)pstDemosaic->stAuto.au16AntiAliasSlope[u32IsoLevel];	
		pstDmCfg->u16hfIntpThHig1 = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);
		pstDmCfg->u16hfIntpThHig1 = HI_DEMOSAIC_MIN((pstDmCfg->u16hfIntpThLow1 + HI_DEMOSAIC_MIN(pstDmCfg->u16hfIntpThHig1,1023)), 1023);

		s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->stAuto.au16NoiseSuppressStr[u32IsoLevel-1]:(HI_S32)pstDemosaic->stAuto.au16NoiseSuppressStr[0];		
		s32Y2 = 			  (HI_S32)pstDemosaic->stAuto.au16NoiseSuppressStr[u32IsoLevel];	
		pstDmCfg->u16ClipDeltaFiltLow   = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);
		pstDmCfg->u16ClipDeltaFiltHigh  = pstDmCfg->u16ClipDeltaFiltLow + u16LutClipDeltaIntpRange;
        pstDmCfg->u16ClipDeltaIntpLow	= pstDmCfg->u16ClipDeltaFiltLow;	
		pstDmCfg->u16ClipDeltaIntpHigh	= pstDmCfg->u16ClipDeltaFiltHigh;	
		s32Y1 = (u32IsoLevel)?(HI_S32)pstDemosaic->stAuto.au16ColorNoiseCtrlThr[u32IsoLevel-1]:(HI_S32)pstDemosaic->stAuto.au16ColorNoiseCtrlThr[0];		
		s32Y2 = 			  (HI_S32)pstDemosaic->stAuto.au16ColorNoiseCtrlThr[u32IsoLevel];	   
		pstDmCfg->u16FakeCrVarThrLow    = (HI_U16)DemosaicGetValueFromLut(u32IsoLevel,s32Y2, s32Y1, u32ISO2, u32ISO1,u32Iso);
	}
    else if(OP_TYPE_MANUAL == pstDemosaic->enOpType)
    {
	    pstDmCfg->u16hfIntpThLow = pstDemosaic->stManual.u16EdgeSmoothThr;
	    pstDmCfg->u16hfIntpThLow  = HI_DEMOSAIC_MIN(pstDmCfg->u16hfIntpThLow,1022);
		
	    pstDmCfg->u16hfIntpThHig = (HI_U16)(pstDemosaic->stManual.u16EdgeSmoothSlope);
	    pstDmCfg->u16hfIntpThHig = pstDmCfg->u16hfIntpThHig + pstDmCfg->u16hfIntpThLow;
	    pstDmCfg->u16hfIntpThHig  = HI_DEMOSAIC_MIN(pstDmCfg->u16hfIntpThHig,1023);  
		
	    pstDmCfg->u16hfIntpThLow1  = pstDemosaic->stManual.u16AntiAliasThr;
	    pstDmCfg->u16hfIntpThLow1  = HI_DEMOSAIC_MIN(pstDmCfg->u16hfIntpThLow1,1022);
	          
	    pstDmCfg->u16hfIntpThHig1  = pstDemosaic->stManual.u16AntiAliasSlope;
		pstDmCfg->u16hfIntpThHig1  = HI_DEMOSAIC_MIN((pstDmCfg->u16hfIntpThLow1 + HI_DEMOSAIC_MIN(pstDmCfg->u16hfIntpThHig1,1023)), 1023);	

		u16NddmStrength = pstDemosaic->stManual.u16NrCoarseStr;		
        pstDmCfg->u8MultiMF		        = pstDemosaic->stManual.u8DetailEnhanceStr;     
		pstDmCfg->u16ClipDeltaFiltLow	= pstDemosaic->stManual.u16NoiseSuppressStr; 
		pstDmCfg->u16ClipDeltaFiltHigh    = pstDmCfg->u16ClipDeltaFiltLow + u16LutClipDeltaIntpRange;
        pstDmCfg->u16ClipDeltaIntpLow	  = pstDmCfg->u16ClipDeltaFiltLow;	
	    pstDmCfg->u16ClipDeltaIntpHigh	  = pstDmCfg->u16ClipDeltaFiltHigh;	
        pstDmCfg->s16SatuFixEhcY		= (HI_S16)(pstDemosaic->stManual.u16SharpenLumaStr) - 256;
        pstDmCfg->u16FakeCrVarThrLow    = pstDemosaic->stManual.u16ColorNoiseCtrlThr;
	}

	Demosaic_HFIntp(pstDmCfg, IspDev);
    ISP_FCRFw(pstDmCfg, pstDemosaic, u32IsoLevel, u32ISO2, u32ISO1, u32Iso);
    Demosaic_GFBlurLut(pstDemosaic, IspDev, pstDmCfg, u16NddmStrength, u32Iso);

	if (pstDemosaic->bUpdateEhcLut)
	{
		memcpy(pstDmCfg->au8EhcGainLut, pstDemosaic->au8EhcGainLut,HI_ISP_DEMOSAIC_LUT_LENGTH);
		pstDmCfg->bUpdateEhc = HI_TRUE;
		pstDemosaic->bUpdateEhcLut = HI_FALSE;
		hi_ext_system_ehcgainlut_update_write(HI_FALSE);
	}
				
	return	HI_SUCCESS;
}

static __inline HI_S32 DemosaicImageResWrite(ISP_DEV IspDev, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstRes)
{

	hi_isp_demosaic_width_write(IspDev, pstRes->u16Width-1);
	hi_isp_demosaic_height_write(IspDev,  pstRes->u16Height-1);

    return HI_SUCCESS;
}

HI_S32 ISP_DemosaicRun(ISP_DEV IspDev, const HI_VOID *pStatInfo, HI_VOID *pRegCfg, HI_S32 s32Rsv)
{	 
	ISP_REG_CFG_S *pstReg;
	ISP_CTX_S *pstIspCtx = HI_NULL;
	ISP_DEMOSAIC_S *pstDemosaic = HI_NULL;

	ISP_GET_CTX(IspDev, pstIspCtx);
	DEMOSAIC_GET_CTX(IspDev, pstDemosaic);

	/* calculate every two interrupts */
	if (!pstDemosaic->bInit)
	{
		return HI_SUCCESS;
	}
	
	if (0 != pstIspCtx->u32FrameCnt % 2)
	{
		return HI_SUCCESS;
	}

	DemosaicReadExtregs(IspDev);

	pstReg = (ISP_REG_CFG_S *)pRegCfg;
	ISP_DemosaicFw(pstIspCtx->stLinkage.u32Iso, IspDev, &pstReg->stDemRegCfg);

	((ISP_REG_CFG_S *)pRegCfg)->unKey.bit1DemCfg = 1;		
		
	return HI_SUCCESS;
}		
		
HI_S32 ISP_DemosaicCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
	ISP_DEMOSAIC_S *pstDemosaic = HI_NULL;
	
	switch (u32Cmd)
	{
		case ISP_WDR_MODE_SET :
			DEMOSAIC_GET_CTX(IspDev, pstDemosaic);
			pstDemosaic->bInit = HI_FALSE;
			ISP_DemosaicInit(IspDev);
			break;
		case ISP_PROC_WRITE:
			DemosaicProcWrite(IspDev, (ISP_CTRL_PROC_WRITE_S *)pValue);
			break;
		case ISP_CHANGE_IMAGE_MODE_SET:   
			DemosaicImageResWrite(IspDev, (ISP_CMOS_SENSOR_IMAGE_MODE_S *)pValue);  
			break;
		default :
			break;
	}
	return HI_SUCCESS;
}

HI_S32 ISP_DemosaicExit(ISP_DEV IspDev)
{
	hi_isp_demosaic_cfg_enable_write(IspDev, HI_FALSE);
	hi_isp_demosaic_fcr_enable_write(IspDev, HI_FALSE);
	hi_isp_demosaic_ahd_enable_write(IspDev, HI_FALSE);
	hi_isp_nddm_enable_write(IspDev, HI_FALSE);
	return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterDemosaic(ISP_DEV IspDev)
{
	ISP_CTX_S *pstIspCtx = HI_NULL;
	ISP_ALG_NODE_S *pstAlgs = HI_NULL;
	
	ISP_GET_CTX(IspDev, pstIspCtx);

	pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
	ISP_CHECK_POINTER(pstAlgs);

	pstAlgs->enAlgType = ISP_ALG_DEMOSAIC;
	pstAlgs->stAlgFunc.pfn_alg_init = ISP_DemosaicInit;		
	pstAlgs->stAlgFunc.pfn_alg_run	= ISP_DemosaicRun;
	pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_DemosaicCtrl;
	pstAlgs->stAlgFunc.pfn_alg_exit = ISP_DemosaicExit;
	pstAlgs->bUsed = HI_TRUE;

	return HI_SUCCESS;
}
		
#ifdef __cplusplus
#if __cplusplus
		}
#endif
#endif /* End of #ifdef __cplusplus */
		

