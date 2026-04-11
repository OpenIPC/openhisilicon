#if 0
/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_noise_reduction.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/01/16
  Description   : 
  History       :
  1.Date        : 2015/01/16
    Author      : q00214668
    Modification: Created file

******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_math_utils.h"
#include "hi_math.h"
#include "isp_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

/***************************************************************************/
/********************GLOBAL VAR DEFINED FOR NOISE FW************************/
#define  HI_FW_NOISE_BIT_DEPTH   (12)
#define  HI_FW_ISO_TABLE_DIVISOR (1000)
#define  HI_FW_ISO_MAX           (80000)

#define HI_ISP_NR_EXPO	  (39942)
#define HI_ISP_NR_EXPOTH  (200)
#define HI_ISP_NR_EXPOTH1 (2000)
#define HI_ISP_NR_EXPOTH2 (10000)
#define HI_ISP_NR_EXPOLO  (4)
#define HI_ISP_NR_EXPOGN  (24)

static HI_U32 g_au32NRIsoLut[ISP_AUTO_ISO_STRENGTH_NUM] = {400,800,1600,3200,6400,12000,36000,64000,82000,82000,82000,82000,82000,82000,82000,82000};

/***************************************************************************/
/********************************STRUCT************************************/
typedef struct hiISP_NOISE_REDUCTION_S
{
	HI_U8   u8SensorIndex; /*0:34220  1:9m034*/
	HI_BOOL bFcrEn;
	HI_U8  u8ManuMode;
	HI_U8  u8FalseColorShift;    // 8bit
	HI_U8  u8FalseColorGain;	 // 8bit
	HI_U8  u8FalseColorLimit;    // 8bit
	HI_U16 u16BlendLowLmt;
	HI_U16 u16BlendHighLmt;	
	HI_DOUBLE dRangeLowCoef;
	HI_DOUBLE dRangeHighCoef;
	HI_U8  au8LumaGainLut[HI_ISP_NR_LUMA_LUT_LENGTH];
	HI_U32 au32LumaIsoLut[HI_ISP_NR_LUMA_LUT_LENGTH];
	/*noise reduction calibration para*/
	ISP_NR_CABLI_PARA_S     stNrCaliPara;
} ISP_NOISE_REDUCTION_S;

ISP_NOISE_REDUCTION_S g_astNrCtx[ISP_MAX_DEV_NUM];
#define NR_GET_CTX(dev, pstCtx)   pstCtx = &g_astNrCtx[dev] 

static HI_VOID NrExtRegsDefault(HI_VOID)
{
	hi_ext_system_nr_manu_mode_write(HI_EXT_SYSTEM_NR_MANU_MODE_DEFAULT);
	hi_ext_system_nr_manual_fcrshift_write(HI_EXT_SYSTEM_NR_MANUAL_FCR_SHIFT_DEFAULT);
	hi_ext_system_nr_manual_fcrgain_write(HI_EXT_SYSTEM_NR_MANUAL_FCR_GAIN_DEFAULT);
	hi_ext_system_nr_manual_fcrlimit_write(HI_EXT_SYSTEM_NR_MANUAL_FCR_LIMIT_DEFAULT);
	hi_ext_system_nr_manual_blendlowlmt_write(HI_EXT_SYSTEM_NR_MANUAL_BLENDLOWLMT_DEFAULT);
	hi_ext_system_nr_manual_blendhighlmt_write(HI_EXT_SYSTEM_NR_MANUAL_BLENDHIGHLMT_DEFAULT);
	hi_ext_system_nr_manual_rangecoeflow_write(HI_EXT_SYSTEM_NR_RANGELOW_MANUAL_DEFAULT);
	hi_ext_system_nr_manual_rangecoefhigh_write(HI_EXT_SYSTEM_NR_RANGEHIGH_MANUAL_DEFAULT);
	hi_ext_system_nr_nodir_low_thd_write(HI_EXT_SYSTEM_NR_NODIR_LOW_THD_DEFAULT);
	hi_ext_system_nr_nodir_hig_thd_write(HI_EXT_SYSTEM_NR_NODIR_HIG_THD_DEFAULT);
	hi_ext_system_nr_nodir_gain_manual_write(HI_EXT_SYSTEM_NR_NODIR_GAIN_MANUAL_DEFAULT);
	hi_ext_system_nr_fcr_en_write(HI_EXT_SYSTEM_NR_FCR_EN_DEFAULT);
    return;
}

static HI_VOID NrRegsDefault(ISP_DEV IspDev) 
{ 
    ISP_CTX_S *pstIspCtx = HI_NULL;    
	HI_U8 u8WDRMode;
	ISP_GET_CTX(IspDev, pstIspCtx);
    u8WDRMode = pstIspCtx->u8SnsWDRMode;
	  
	hi_isp_2dnr_cfg_enable_write(IspDev, HI_FALSE); 
	hi_isp_2dnr_var_mode_write(IspDev, HI_ISP_2DNR_VAR_MODE_DEFAULT);
	hi_isp_2dnr_nodir_en_write(IspDev, HI_ISP_2DNR_NODIR_EN_DEFAULT);	
	hi_isp_2dnr_moire_en_write(IspDev, HI_ISP_2DNR_MOIRE_EN_DEFAULT);
	hi_isp_2dnr_moire_fcrscale_write(IspDev, HI_ISP_2DNR_MOIRE_FCRSCALE_DEFAULT);
	hi_isp_2dnr_moire_high_write(IspDev, HI_ISP_2DNR_MOIRE_HIGH_DEFAULT);
	hi_isp_2dnr_moire_low_write(IspDev, HI_ISP_2DNR_MOIRE_LOW_DEFAULT);
		
	if(IS_FS_WDR_MODE(u8WDRMode))
	{
		hi_isp_2dnr_blendterm_en_write(IspDev, HI_ISP_NEW_BLEND_TERM_EN_DEFAULT_WDR);
	}
	else
  	{
     	hi_isp_2dnr_blendterm_en_write(IspDev, HI_ISP_NEW_BLEND_TERM_EN_DEFAULT_LINEAR);
  	}	
	
	hi_isp_2dnr_dm_bldr_write(IspDev, HI_ISP_DM_BLEND_RATIO_DEFAULT);
    return;
}

static HI_VOID NrExtRegsInitialize(ISP_DEV IspDev)
{
	/* initial register of noise reduction */
	HI_U32 i;
	ISP_NOISE_REDUCTION_S *pstNr = HI_NULL;
	ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
	HI_U32 u32Width = 0, u32Height = 0;
		
    NR_GET_CTX(IspDev, pstNr);
	
    ISP_SensorGetDefault(IspDev, &pstSnsDft);

	/* initial ext auto register of noise reduction */             
    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
	{	             
        hi_ext_system_nr_auto_fcrgain_write(i, pstSnsDft->stNoiseTbl.stIsoParaTable.au8FalseColorGain[i]);
		hi_ext_system_nr_auto_fcrshift_write(i, pstSnsDft->stNoiseTbl.stIsoParaTable.au8FalseColorShift[i]);        
        hi_ext_system_nr_auto_fcrlimit_write(i, pstSnsDft->stNoiseTbl.stIsoParaTable.au8FalseColorLimit[i]); 
		hi_ext_system_nr_auto_blendlowlmt_write(i, pstSnsDft->stNoiseTbl.stIsoParaTable.au16BlendLowLmt[i]); 
		hi_ext_system_nr_auto_blendhighlmt_write(i, pstSnsDft->stNoiseTbl.stIsoParaTable.au16BlendHighLmt[i]); 
		hi_ext_system_nr_auto_rangecoefhigh_write(i, pstSnsDft->stNoiseTbl.stIsoParaTable.au32RangeHighCoef[i]);
		hi_ext_system_nr_auto_rangecoeflow_write(i, pstSnsDft->stNoiseTbl.stIsoParaTable.au32RangeLowCoef[i]);
    }	

	for (i = 0; i < HI_ISP_NR_LUMA_LUT_LENGTH; i++)
	{	     
        hi_ext_system_nr_auto_lumaisolut_write(i, pstSnsDft->stNoiseTbl.stIsoParaTable.au32LumaIsoLut[i]);        
        hi_ext_system_nr_auto_lumagainlut_write(i, pstSnsDft->stNoiseTbl.stIsoParaTable.au8LumaGainLut[i]);
	}

	u32Width = hi_ext_sync_total_width_read();
	u32Height = hi_ext_sync_total_height_read();

	hi_isp_2dnr_width_write(IspDev, u32Width-1);
	hi_isp_2dnr_height_write(IspDev, u32Height-1);

	pstNr->u8SensorIndex = pstSnsDft->stNoiseTbl.u8SensorIndex;
	pstNr->u8ManuMode = hi_ext_system_nr_manu_mode_read();
	memcpy(&pstNr->stNrCaliPara, &pstSnsDft->stNoiseTbl.stNrCaliPara, sizeof(ISP_NR_CABLI_PARA_S));
	
    return;
}

static HI_S32 NrReadExtregs(ISP_DEV IspDev)
{
	HI_U8 i;
	ISP_NOISE_REDUCTION_S *pstNr = HI_NULL; 

	NR_GET_CTX(IspDev, pstNr);

	pstNr->u8ManuMode = hi_ext_system_nr_manu_mode_read();
	
	for (i = 0; i < HI_ISP_NR_LUMA_LUT_LENGTH; i++)
	{
		pstNr->au32LumaIsoLut[i] = hi_ext_system_nr_auto_lumaisolut_read(i);
		pstNr->au8LumaGainLut[i] = hi_ext_system_nr_auto_lumagainlut_read(i);
	}
	
	return HI_SUCCESS;
}

HI_S32 NrProcWrite(ISP_DEV IspDev, ISP_CTRL_PROC_WRITE_S *pstProc)
{
    ISP_CTRL_PROC_WRITE_S stProcTmp;

    if ((HI_NULL == pstProc->pcProcBuff)
        || (0 == pstProc->u32BuffLen))
    {
        return HI_FAILURE;
    }

	NrReadExtregs(IspDev);

    stProcTmp.pcProcBuff = pstProc->pcProcBuff;
    stProcTmp.u32BuffLen = pstProc->u32BuffLen;
    
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "-----NR INFO-------------------------------------------------------------------\n");
           
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%8s\n",
        "En");
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%8d\n",
        hi_isp_2dnr_cfg_enable_read(IspDev));

    pstProc->u32WriteLen += 1;
   
    return HI_SUCCESS;
}

static HI_S32 ISP_NrInit(ISP_DEV IspDev)
{
    NrRegsDefault(IspDev);
    NrExtRegsDefault();
    NrExtRegsInitialize(IspDev);

    return HI_SUCCESS;
}

// z00319880: interpolate alpha and beta using calibration data
#define  EPS (0.000001f)
#define  COL_ISO      0
#define  COL_ISOLOG   1
#define  COL_ALPHA    2
#define  COL_BETA     3
static HI_VOID DMNR_Alpha_Beta(HI_FLOAT (*pRecord)[4], HI_S32 recordNum, HI_S32 iso, HI_FLOAT *pAlpha, HI_FLOAT *pBeta)
{
	HI_S32 i = 0;
	HI_FLOAT  y_diff = 0, x_diff = 0, iso_log = (HI_FLOAT)log10((HI_FLOAT)iso);

	// record: iso - log(iso) - alpha - beta
	if(iso <= pRecord[0][COL_ISO])
	{
		*pAlpha = pRecord[0][COL_ALPHA];
		*pBeta  = pRecord[0][COL_BETA];
		 return;
	}

	if(iso >= pRecord[recordNum - 1][COL_ISO])
	{
		*pAlpha = pRecord[recordNum - 1][COL_ALPHA];
		*pBeta  = pRecord[recordNum - 1][COL_BETA];
		 return;
	}

	for(i = 0; i < recordNum - 1; i++)
	{
		if(iso >= pRecord[i][COL_ISO] && iso <= pRecord[i + 1][COL_ISO])
		{
			x_diff = pRecord[i + 1][COL_ISOLOG] - pRecord[i][COL_ISOLOG];  // log(iso) diff
			y_diff = pRecord[i + 1][COL_ALPHA]  - pRecord[i][COL_ALPHA];   // alpha diff
		   *pAlpha = pRecord[i][COL_ALPHA] + y_diff * (iso_log - pRecord[i][COL_ISOLOG]) / (x_diff + EPS);

		    x_diff = pRecord[i + 1][COL_ISO]  - pRecord[i][COL_ISO];       // iso diff
		    y_diff = pRecord[i + 1][COL_BETA] - pRecord[i][COL_BETA];      // beta diff
		   *pBeta  = pRecord[i][COL_BETA] + y_diff * (iso - pRecord[i][COL_ISO]) / (x_diff + EPS);

			return;
		}
	}
}

HI_S32 NrBlendRate(HI_S32 v, HI_S32 x0, HI_S32 x1, HI_S32 y0, HI_S32 y1)
{
	if (v <= x0)  return (HI_S32) y0;
	if (v >= x1)  return (HI_S32) y1;
	if (x0 == x1) return (HI_S32) y1;

	return (HI_S32) (y0 + (y1-y0)*(v-x0)/(x1-x0));
}

static HI_U8 NrGetIsoIndex(HI_U32 u32Iso)
{
	HI_U8 i;
    
    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
    {
        if (u32Iso <= g_au32NRIsoLut[i])
        {
            return i;
        }
    }

	i = (i>=ISP_AUTO_ISO_STRENGTH_NUM)?(ISP_AUTO_ISO_STRENGTH_NUM-1):i;

	return i;
}

static HI_VOID NrUpdateParByIso(HI_U32 u32Iso, ISP_NOISE_REDUCTION_S *pstNr)
{
	HI_U8 u8Index;

	if (HI_NULL == pstNr)
	{
		printf("Unable to handle null point in nr moudle!\n");
		return;
	}
	
	u8Index = NrGetIsoIndex(u32Iso);

	if (pstNr->u8ManuMode)
	{
		pstNr->u16BlendHighLmt   = hi_ext_system_nr_manual_blendhighlmt_read();
		pstNr->u16BlendLowLmt    = hi_ext_system_nr_manual_blendlowlmt_read();
		pstNr->u8FalseColorGain  = hi_ext_system_nr_manual_fcrgain_read();
		pstNr->u8FalseColorLimit = hi_ext_system_nr_manual_fcrlimit_read();
		pstNr->u8FalseColorShift = hi_ext_system_nr_manual_fcrshift_read();

		pstNr->dRangeHighCoef  = (HI_DOUBLE)hi_ext_system_nr_manual_rangecoefhigh_read()/(HI_DOUBLE)HI_FW_ISO_TABLE_DIVISOR;
		pstNr->dRangeLowCoef   = (HI_DOUBLE)hi_ext_system_nr_manual_rangecoeflow_read()/(HI_DOUBLE)HI_FW_ISO_TABLE_DIVISOR;
	}
	else
	{		
		pstNr->u16BlendHighLmt   = hi_ext_system_nr_auto_blendhighlmt_read(u8Index);
		pstNr->u16BlendLowLmt    = hi_ext_system_nr_auto_blendlowlmt_read(u8Index);
		pstNr->u8FalseColorGain  = hi_ext_system_nr_auto_fcrgain_read(u8Index);
		pstNr->u8FalseColorLimit = hi_ext_system_nr_auto_fcrlimit_read(u8Index);
		pstNr->u8FalseColorShift = hi_ext_system_nr_auto_fcrshift_read(u8Index);

		pstNr->dRangeHighCoef  = (HI_DOUBLE)hi_ext_system_nr_auto_rangecoefhigh_read(u8Index)/(HI_DOUBLE)HI_FW_ISO_TABLE_DIVISOR;
		pstNr->dRangeLowCoef   = (HI_DOUBLE)hi_ext_system_nr_auto_rangecoeflow_read(u8Index)/(HI_DOUBLE)HI_FW_ISO_TABLE_DIVISOR;
	}

	pstNr->bFcrEn = hi_ext_system_nr_fcr_en_read();
}

// z00219880: luma NR gain
static HI_S32 LumaNR_Gain(HI_S32* pISOLUT, HI_S8* pGainLUT, HI_S32 ISO)
{
	HI_S32 i = 0, gain_ret = 0, gain_diff = 0, ISO_diff = 0;

	for(i = 0; i < 5; i++)
	{
		if(ISO >= pISOLUT[i] && ISO <= pISOLUT[i + 1])
		{
			ISO_diff  = pISOLUT[i + 1] - pISOLUT[i];
			gain_diff = pGainLUT[i + 1] - pGainLUT[i];
			gain_ret  = pGainLUT[i] + gain_diff * (ISO - pISOLUT[i]) / ISO_diff;
			break;
		}
	}

	return gain_ret;
}
  
static HI_VOID NrRegisterGuider(HI_U32 u32Iso, ISP_DEV IspDev, ISP_NR_REG_CFG_S *pstReg)
{  
	HI_DOUBLE iso_log;
	HI_FLOAT  pAlpha = 0.02f, pBeta = 28.0f, v0;
	HI_S32 rate    = 1;

	u32Iso = 20000;

	HI_S32 s32Iso		 = (HI_S32)u32Iso;
	HI_S32 n;
	HI_S32 bitDepth = HI_FW_NOISE_BIT_DEPTH;
	HI_S32 msk = (1 << bitDepth)-1;
	ISP_NOISE_REDUCTION_S *pstNr = HI_NULL; 
    

	HI_S32 s32NrNoDirLowThd = 8;
	HI_S32 s32NrNoDirHigThd = 14;
		
    NR_GET_CTX(IspDev, pstNr);
	
	switch(pstNr->u8SensorIndex)
	{
		case HI_ISP_NR_SENSOR_INDEX_MN34220:
			{
				// 34220: fixd 
				s32Iso = MIN2(s32Iso, HI_FW_ISO_MAX);
				iso_log = log10((HI_DOUBLE)(s32Iso));
				
				pAlpha = (HI_FLOAT)((s32Iso < 100) ? 0.0129 : (0.0270*iso_log*iso_log - 0.1096*iso_log + 0.1241));   // blc
				pBeta  = (HI_FLOAT)((s32Iso < 100) ? 3.6706 : 0.0018*s32Iso+3.4906);
				
				if(s32Iso > 10000 && s32Iso < 20000)
				{
					HI_FLOAT pAlpha_hiso = 0.160966 * iso_log * iso_log - 1.293411 * iso_log + 2.701196;
					HI_FLOAT pBeta_hiso  = 0.000708 * s32Iso + 17.590376;
				
					HI_S32 bldr_hiso = 256 * (s32Iso - 10000) / 10000;
				
					pAlpha = ((256 - bldr_hiso) * pAlpha + bldr_hiso * pAlpha_hiso) / 256 ;
					pBeta  = ((256 - bldr_hiso) * pBeta  + bldr_hiso * pBeta_hiso) / 256;
				}
				else if(s32Iso >= 20000)
				{
					HI_FLOAT pAlpha_hiso = 0.160966 * iso_log * iso_log - 1.293411 * iso_log + 2.701196;
					HI_FLOAT pBeta_hiso  = 0.000708 * s32Iso + 17.590376;
				
					pAlpha = pAlpha_hiso;
					pBeta  = pBeta_hiso;
				}	

				break;
			}
		case HI_ISP_NR_SENSOR_INDEX_9M034:	
		case HI_ISP_NR_SENSOR_INDEX_IMX222:
		case HI_ISP_NR_SENSOR_INDEX_IMX122:
			{
				if (pstNr->stNrCaliPara.pCalibcoef)
				{
					DMNR_Alpha_Beta((HI_FLOAT (*)[4])pstNr->stNrCaliPara.pCalibcoef, pstNr->stNrCaliPara.u8CalicoefRow, s32Iso, &pAlpha, &pBeta);
				}
								
				break;
			}
		default:
			{
				pAlpha = 0.02f;
				pBeta  = 28.0f;
				break;
			}
	}

	/************************************************************/
	// range threshold 
	rate = 0x40;
	if (bitDepth > 10) 
	{
		rate <<= (bitDepth - 10);
	}
	else if (bitDepth < 10)
	{
		rate >>= (10 - bitDepth);
	}

	v0 = 512 * pAlpha + pBeta;
	v0 = v0*rate/64;

	pstReg->rangeGain =  (HI_S32)(pAlpha * 4096);
	
	pstReg->rangeLumaLow = 32;
	pstReg->rangeLumaHig = (1 << bitDepth)-1; 

	NrUpdateParByIso(u32Iso, pstNr);

	pstReg->falseColorEnable = pstNr->bFcrEn; 	  // 1bit
	pstReg->falseColorShift  = pstNr->u8FalseColorShift; 	  // 8bit (unit of 10bit)
	pstReg->falseColorGain   = pstNr->u8FalseColorGain;  // 8bit (0x40 == 100% )
	pstReg->falseColorLimit  = pstNr->u8FalseColorLimit;  // 8bit (unit of 10bit)

	pstReg->blendLowLimit = pstNr->u16BlendLowLmt;
	pstReg->blendHigLimit = pstNr->u16BlendHighLmt;
	
    pstReg->rangeLowThres    = (HI_S32)(v0 * pstNr->dRangeLowCoef);
    pstReg->rangeHigThres    = (HI_S32)(v0 * pstNr->dRangeHighCoef);
	
	// adaptive parameter based on iso and exposure
	if(u32Iso <= HI_ISP_NR_EXPOTH)
	{
		HI_S32 rLow  = NrBlendRate(HI_ISP_NR_EXPO, HI_ISP_NR_EXPOTH1, HI_ISP_NR_EXPOTH2, 16, HI_ISP_NR_EXPOLO);
		HI_S32 rGain = NrBlendRate(HI_ISP_NR_EXPO, HI_ISP_NR_EXPOTH1, HI_ISP_NR_EXPOTH2, 16, HI_ISP_NR_EXPOGN);

		pstReg->blendLowLimit = (pstReg->blendLowLimit * rLow)  / 16;
		pstReg->rangeLowThres = (pstReg->rangeLowThres * rGain) / 16;
		pstReg->rangeHigThres = (pstReg->rangeHigThres * rGain) / 16;
	}

    //pstReg->lumaNREnable = 0;//pstNr->stCmosNRTable.stNrCommPara.u8lumaNREnable;

	pstReg->NrLumaNrGain = LumaNR_Gain((HI_S32*)pstNr->au32LumaIsoLut, (HI_S8*)pstNr->au8LumaGainLut, u32Iso);

	for (n=0; n<=16; n++) 
	{
		pstReg->rangeLowLUT[n] = pstReg->rangeLowThres + (n-8)*rate*pstReg->rangeGain/4096;
		pstReg->rangeHigLUT[n] = pstReg->rangeHigThres + (n-8)*rate*pstReg->rangeGain/4096;

		pstReg->rangeLowLUT[n] = CLIP3(pstReg->rangeLowLUT[n], 0, msk);
		pstReg->rangeHigLUT[n] = CLIP3(pstReg->rangeHigLUT[n], 0, msk);
	}

	if (OP_TYPE_AUTO == hi_ext_system_nr_manu_mode_read())
	{
		s32NrNoDirLowThd = hi_ext_system_nr_nodir_low_thd_read();
		s32NrNoDirHigThd = hi_ext_system_nr_nodir_hig_thd_read();
		
		if(s32Iso <= g_au32NRIsoLut[1])
	    {

	        pstReg->u8NrNoDirGain = s32NrNoDirLowThd;  
	    }
		else if ((s32Iso > g_au32NRIsoLut[1]) && (s32Iso < g_au32NRIsoLut[4]))
		{
			pstReg->u8NrNoDirGain = s32NrNoDirLowThd + (s32NrNoDirHigThd - s32NrNoDirLowThd)*(s32Iso - (HI_S32)g_au32NRIsoLut[1])/((HI_S32)g_au32NRIsoLut[4] - (HI_S32)g_au32NRIsoLut[1]);   
		}
	    else if((s32Iso >= g_au32NRIsoLut[4]) && (s32Iso <= g_au32NRIsoLut[11]))
	    {
	    	pstReg->u8NrNoDirGain = s32NrNoDirHigThd;
	    }
		else if ((s32Iso > g_au32NRIsoLut[11]) && (s32Iso < g_au32NRIsoLut[12]))
		{
			pstReg->u8NrNoDirGain = s32NrNoDirHigThd - (s32NrNoDirHigThd - (s32NrNoDirLowThd + 5))*(s32Iso - (HI_S32)g_au32NRIsoLut[11])/((HI_S32)g_au32NRIsoLut[12] - (HI_S32)g_au32NRIsoLut[11]);
		}	
		else
		{
			pstReg->u8NrNoDirGain = s32NrNoDirLowThd + 5;
		}				
	}
	else
	{
		pstReg->u8NrNoDirGain = hi_ext_system_nr_nodir_gain_manual_read();
	}
}

HI_S32 ISP_NrRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
	ISP_REG_CFG_S *pstReg;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_GET_CTX(IspDev, pstIspCtx);

    if (pstIspCtx->stLinkage.bDefectPixel)
    {
        return HI_SUCCESS;
    }

    /* calculate every two interrupts */	
    if (0 != pstIspCtx->u32FrameCnt % 2)
    {
        return HI_SUCCESS;
    }

	pstReg = (ISP_REG_CFG_S *)pRegCfg;

	NrReadExtregs(IspDev);
                
	NrRegisterGuider(pstIspCtx->stLinkage.u32Iso, IspDev, &pstReg->stNrRegCfg);	
	
	((ISP_REG_CFG_S *)pRegCfg)->unKey.bit1NrCfg = 1;
     
    return HI_SUCCESS;
}

static HI_VOID ISP_Nr_Bas_ModeSet(ISP_DEV IspDev)
{
	HI_U16 u16Width, u16Height;

	u16Width  = hi_ext_system_bas_outwidth_read();
	u16Height = hi_ext_system_bas_outheight_read();

	hi_isp_2dnr_width_write(IspDev, u16Width-1);
	hi_isp_2dnr_height_write(IspDev, u16Height-1);
}

HI_S32 ISP_NrCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :
            ISP_NrInit(IspDev);
            break;
        case ISP_PROC_WRITE:
            NrProcWrite(IspDev, (ISP_CTRL_PROC_WRITE_S *)pValue);
            break;
		case ISP_BAS_MODE_SET:
			ISP_Nr_Bas_ModeSet(IspDev);
			break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_NrExit(ISP_DEV IspDev)
{
	hi_isp_2dnr_cfg_enable_write(IspDev, HI_FALSE);
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterNr(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_NR;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_NrInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_NrRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_NrCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_NrExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif

