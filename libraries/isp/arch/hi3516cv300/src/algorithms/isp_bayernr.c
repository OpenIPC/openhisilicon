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
    Author      : l00271307
    Modification: Created file

******************************************************************************/
#include <math.h>
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_proc.h"
#include "isp_math_utils.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define HI_ISP_BAYERNR_BITDEP (16)
#define HI_ISP_BAYERNR_MAXLMT (11)
/*For Bayernr*/
#define HI_ISP_BAYERNR_EN (1)
#define HI_ISP_BAYERNR_JNLMSEL   (0x3)
#define HI_ISP_BAYERNR_LUT_DIVISOR (1000)
#define HI_ISP_BAYERNR_STRENGTH (1.0)
#define HI_ISP_BAYERNR_STRENGTH_DIVISOR (100)

/*For BAYERNR WDR STRENGTH*/
#define HI_ISP_BAYERNR_WDR_STRENGTH_0 (16)
#define HI_ISP_BAYERNR_WDR_STRENGTH_1 (40)

#define HI_BCOM_ALPHA (2)
#define HI_BDEC_ALPHA (4)

static HI_U32 g_au32BayernrIsoLut[ISP_AUTO_ISO_STRENGTH_NUM] = {100,200,400,800,1600,3200,6400,12800,25600,51200,102400,204800,409600,819200,1638400,3276800};
static HI_U16 g_au16LutCoringHigh[ISP_AUTO_ISO_STRENGTH_NUM] = {300, 300, 300, 300, 340, 450, 660, 900, 1170, 1600, 2160, 3200, 3200, 3200, 3200, 3200};
static HI_U8  g_au8ChromaRatio[BAYER_PATTER_NUM][ISP_AUTO_ISO_STRENGTH_NUM] = { {0, 0, 1, 2, 4, 6, 8, 9, 10, 15, 18, 20, 20, 20, 20, 20},   //ChromaRatioR
																				{0, 0, 0, 0, 1, 4, 6, 8, 10, 15, 18, 20, 20, 20, 20, 20},   //ChromaRatioGr
																				{0, 0, 0, 0, 1, 4, 6, 8, 10, 15, 18, 20, 20, 20, 20, 20},   //ChromaRatioGb
																				{0, 0, 1, 2, 4, 6, 8, 9, 10, 15, 18, 20, 20, 20, 20, 20}};   //ChromaRatioB 
static HI_FLOAT g_afLutB[ISP_AUTO_ISO_STRENGTH_NUM] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

typedef struct hiISP_BAYERNR_AUTO_ATTR_S
{
	HI_U8	 au8ChromaStr[BAYER_PATTER_NUM][ISP_AUTO_ISO_STRENGTH_NUM]; //[0, 3]      //Strength of Chrmoa noise reduction
	HI_U8    au8FineStr[ISP_AUTO_ISO_STRENGTH_NUM];                     //[0, 128]    //Strength of luma noise reduction
	HI_U16   au16CoringWeight[ISP_AUTO_ISO_STRENGTH_NUM];               //[0, 3200]   //Strength of reserving the random noise 
} ISP_BAYERNR_AUTO_ATTR_S;

typedef struct hiISP_BAYERNR_MANUAL_ATTR_S
{			 
	HI_U8	 u8ChromaStr[BAYER_PATTER_NUM];    //[0, 3]      //Strength of Chrmoa noise reduction
	HI_U8    u8FineStr;                        //[0, 128]    //Strength of luma noise reduction
	HI_U16   u16CoringWeight;                  //[0, 3200]   //Strength of reserving the random noise 
} ISP_BAYERNR_MANUAL_ATTR_S;

typedef struct hiISP_BAYERNR_WDR_ATTR_S
{			 
	HI_U8 au8WDRCoarseStr[WDR_MAX_FRAME_NUM];   //[0, 80] //Denoise strength of long frame and short frame in wdr mode
}ISP_BAYERNR_WDR_ATTR_S;

typedef struct hiISP_BAYERNR_S
{			 
	// Luminance filter
	ISP_OP_TYPE_E enOpType;
	HI_BOOL  bInit;
	HI_BOOL  bEnable;
	HI_BOOL  bLutUpdate;
	HI_U8	 u8JnlmSelect; 			 				 			 	
	HI_U32   u32PreIso;

	HI_U8    au8ChromaRatio[BAYER_PATTER_NUM][ISP_AUTO_ISO_STRENGTH_NUM];
	HI_FLOAT afStrength     [BAYER_PATTERN_BUTT];
	HI_FLOAT afLutCoringL   [ISP_AUTO_ISO_STRENGTH_NUM];   
	HI_FLOAT afLutCoringH   [ISP_AUTO_ISO_STRENGTH_NUM];
	HI_FLOAT afLutCoringLRatio[HI_ISP_BAYERNR_LUT_LENGTH];
	HI_FLOAT afLutB[ISP_AUTO_ISO_STRENGTH_NUM];

	HI_U16	 au16CoarseStr	 [ISP_AUTO_ISO_STRENGTH_NUM];   //[0, 1000]
	HI_U16   au16LutCoringLRatio[HI_ISP_BAYERNR_LUT_LENGTH];
	HI_U16   au16LUTCprClip   [ISP_AUTO_ISO_STRENGTH_NUM];
	HI_U16   au16CoringRatio[HI_ISP_BAYERNR_LUT_LENGTH];   //[0, 1000]
	HI_U16   au16ExpoValues[WDR_MAX_FRAME];
    HI_U16   au16WDR_frameTHs[WDR_MAX_FRAME];
	HI_U16   u16WDR_BlcThresh;
	HI_U8    u8WdrFramesMerge;
	HI_U8    u8WDR_eInitBlcNR;
	HI_U8    au8WDR_strengths[WDR_MAX_FRAME];

	ISP_BAYERNR_AUTO_ATTR_S stAuto;
	ISP_BAYERNR_MANUAL_ATTR_S stManual;
	ISP_BAYERNR_WDR_ATTR_S  stWDR;
} ISP_BAYERNR_S;

ISP_BAYERNR_S g_astBayernrCtx[ISP_MAX_DEV_NUM] = {{0}};
#define BAYERNR_GET_CTX(dev, pstCtx)   pstCtx = &g_astBayernrCtx[dev]

static HI_VOID BayernrExtRegsInitialize(ISP_DEV IspDev)
{
    HI_U8 i;
    ISP_BAYERNR_S *pstBayernr = HI_NULL;
	ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
	
    BAYERNR_GET_CTX(IspDev, pstBayernr);
    ISP_SensorGetDefault(IspDev, &pstSnsDft);
	
    hi_ext_system_bayernr_enable_write(HI_EXT_SYSTEM_BAYERNR_ENABLE_DEFAULT);
	
	for (i = 0; i < BAYER_PATTER_NUM; i++)
	{
		hi_ext_system_bayernr_coarse_strength_write(i, pstSnsDft->stBayerNr.au16CoarseStr[i]);
	}

	for(i = 0; i < HI_ISP_BAYERNR_LUT_LENGTH; i++)
	{
		hi_ext_system_bayernr_coring_low_ratio_write(i, pstSnsDft->stBayerNr.au16LutCoringLRatio[i]);
	}

	for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
	{	
		hi_ext_system_bayernr_auto_fine_strength_write(i, pstSnsDft->stBayerNr.au8LutFineStr[i]);
		hi_ext_system_bayernr_auto_coring_weight_write(i, pstSnsDft->stBayerNr.au16LutCoringWeight[i]);
		hi_ext_system_bayernr_auto_chroma_strength_r_write(i,pstSnsDft->stBayerNr.au8ChromaStr[0][i]);   //ChromaStrR
		hi_ext_system_bayernr_auto_chroma_strength_gr_write(i,pstSnsDft->stBayerNr.au8ChromaStr[1][i]);  //ChromaStrGr
		hi_ext_system_bayernr_auto_chroma_strength_gb_write(i,pstSnsDft->stBayerNr.au8ChromaStr[2][i]);  //ChromaStrGb
		hi_ext_system_bayernr_auto_chroma_strength_b_write(i,pstSnsDft->stBayerNr.au8ChromaStr[3][i]);   //ChromaStrB
	}
	
	hi_ext_system_bayernr_manual_mode_write(HI_EXT_SYSTEM_BAYERNR_MANU_MODE_DEFAULT);
	hi_ext_system_bayernr_manual_fine_strength_write(HI_EXT_SYSTEM_BAYERNR_MANU_STRENGTH_DEFAULT);  //FineStr
	hi_ext_system_bayernr_manual_coring_weight_write(HI_EXT_SYSTEM_BAYERNR_MANU_CORING_WEIGHT_DEFAULT); //CoringWeight
	hi_ext_system_bayernr_manual_chroma_strength_write(0,HI_EXT_SYSTEM_BAYERNR_MANU_CHROMA_STRENGTH_DEFAULT);
	hi_ext_system_bayernr_manual_chroma_strength_write(1,HI_EXT_SYSTEM_BAYERNR_MANU_CHROMA_STRENGTH_DEFAULT);
	hi_ext_system_bayernr_manual_chroma_strength_write(2,HI_EXT_SYSTEM_BAYERNR_MANU_CHROMA_STRENGTH_DEFAULT);
	hi_ext_system_bayernr_manual_chroma_strength_write(3,HI_EXT_SYSTEM_BAYERNR_MANU_CHROMA_STRENGTH_DEFAULT);


	hi_ext_system_bayernr_wdr_strength_write(0, HI_ISP_BAYERNR_WDR_STRENGTH_0);
	hi_ext_system_bayernr_wdr_strength_write(1, HI_ISP_BAYERNR_WDR_STRENGTH_1);
	hi_ext_system_bayernr_wdr_blc_thresh_write(HI_EXT_SYSTEM_BAYERNR_WDR_BLCTHRESH_DEFAULT);
	hi_ext_system_bayernr_wdr_blc_init_write(HI_EXT_SYSTEM_BAYERNR_WDR_BLCINIT_DEFAULT);

	pstBayernr->bInit = HI_TRUE;

    return;
}

static HI_VOID BayernrRegsInitialize(ISP_DEV IspDev)
{
	HI_U8 i;
	HI_U8 u8WDRMode;
	HI_U8 u8JnlmBitShift;
	ISP_CTX_S *pstIspCtx = HI_NULL;
	ISP_BAYERNR_S *pstBayernr = HI_NULL;
	ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
	ISP_CMOS_BLACK_LEVEL_S *pstSnsBlackLevel = HI_NULL;
	
	HI_U32 u32Width = 0, u32Height = 0;

	ISP_GET_CTX(IspDev, pstIspCtx);
    BAYERNR_GET_CTX(IspDev, pstBayernr);
    ISP_SensorGetDefault(IspDev, &pstSnsDft);
	ISP_SensorGetBlc(IspDev, &pstSnsBlackLevel);

	u8WDRMode = pstIspCtx->u8SnsWDRMode;

    pstBayernr->enOpType   = OP_TYPE_AUTO;
	pstBayernr->bEnable    = HI_ISP_BAYERNR_EN;
	pstBayernr->bInit      = HI_FALSE;
	pstBayernr->au8WDR_strengths[0] = HI_ISP_BAYERNR_WDR_STRENGTH_0;
	pstBayernr->au8WDR_strengths[1] = HI_ISP_BAYERNR_WDR_STRENGTH_1;

    for(i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
    {
		pstBayernr->afLutCoringH[i] = (HI_FLOAT)g_au16LutCoringHigh[i]/(HI_FLOAT)HI_ISP_BAYERNR_STRENGTH_DIVISOR; 
		pstBayernr->afLutB[i]       = g_afLutB[i];

		pstBayernr->au8ChromaRatio[0][i] = g_au8ChromaRatio[0][i];
		pstBayernr->au8ChromaRatio[1][i] = g_au8ChromaRatio[1][i];
		pstBayernr->au8ChromaRatio[2][i] = g_au8ChromaRatio[2][i];
		pstBayernr->au8ChromaRatio[3][i] = g_au8ChromaRatio[3][i];
    }

	for(i = 0; i < HI_ISP_BAYERNR_LUT_LENGTH; i++)
	{
		pstBayernr->afLutCoringLRatio[i] = pstSnsDft->stBayerNr.au16LutCoringLRatio[i];
	}

	for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
	{	
	}

	if (IS_2to1_WDR_MODE(u8WDRMode))
	{
		pstBayernr->u8WdrFramesMerge = 2;
		u8JnlmBitShift = HI_ISP_BAYERNR_BITDEP - 10;
		
		hi_isp_bnr_satu_ratio_write(IspDev, HI_ISP_BNR_SATU_RATIO_WDR_DEFAULT);
		hi_isp_bnr_jnlm_symcoef_write(IspDev, HI_ISP_BNR_JNLM_SYMCOEF_WDR_DEFAULT);
	}
	else
	{
		pstBayernr->u8WdrFramesMerge = 1;
		u8JnlmBitShift = HI_ISP_BAYERNR_BITDEP - 8;

		hi_isp_bnr_satu_ratio_write(IspDev, HI_ISP_BNR_SATU_RATIO_LINEAR_DEFAULT);
		hi_isp_bnr_jnlm_symcoef_write(IspDev, HI_ISP_BNR_JNLM_SYMCOEF_LINEAR_DEFAULT);
	}

	hi_isp_bnr_jnlm_shift_write(IspDev, u8JnlmBitShift);
	hi_isp_bnr_jnlm_sel_write(IspDev, HI_ISP_BAYERNR_JNLMSEL);
	
	u32Width = hi_ext_sync_total_width_read();
	u32Height = hi_ext_sync_total_height_read();

	hi_isp_bnr_width_write(IspDev, u32Width-1);
	hi_isp_bnr_height_write(IspDev, u32Height-1);
	hi_isp_bnr_en_write(IspDev, pstBayernr->bEnable);
	
	hi_isp_module_nr_sel_write(IspDev, HI_ISP_MOUDLE_POS_NR_DEFAULT);	
	hi_isp_bnr_rlmt_blc_write(IspDev,pstSnsBlackLevel->au16BlackLevel[0] >> 4);  //blc-8bit
	hi_isp_bnr_satu_thhig1_write(IspDev, HI_ISP_BNR_SATU_THHIG1_DEFAULT);
	hi_isp_bnr_satu_thlow1_write(IspDev, HI_ISP_BNR_SATU_THLOW1_DEFAULT);
	hi_isp_bnr_satu_thhig2_write(IspDev, HI_ISP_BNR_SATU_THHIG2_DEFAULT);
	hi_isp_bnr_satu_thlow2_write(IspDev, HI_ISP_BNR_SATU_THLOW2_DEFAULT);
    
	if (pstBayernr->bEnable && hi_isp_wdr_enable_read(IspDev))
	{
		hi_isp_bcom_enable_write(IspDev, HI_TRUE);
		hi_isp_bdec_enable_write(IspDev, HI_TRUE);
		hi_isp_bcom_alpha_write(IspDev, HI_BCOM_ALPHA);
		hi_isp_bdec_alpha_write(IspDev, HI_BDEC_ALPHA);
	}
    return;
}

static HI_S32 BayernrReadExtregs(ISP_DEV IspDev)
{   
	HI_U8 i;
    ISP_BAYERNR_S *pstBayernr = HI_NULL;
    
    BAYERNR_GET_CTX(IspDev, pstBayernr);
	
	pstBayernr->bEnable = hi_ext_system_bayernr_enable_read();
	pstBayernr->enOpType = hi_ext_system_bayernr_manual_mode_read();
	pstBayernr->u16WDR_BlcThresh = hi_ext_system_bayernr_wdr_blc_thresh_read();
	pstBayernr->u8WDR_eInitBlcNR = hi_ext_system_bayernr_wdr_blc_init_read();
	
	memset(&pstBayernr->au16WDR_frameTHs[0], 0, sizeof(HI_U16)*(WDR_MAX_FRAME));
	
	for (i = 0; i < BAYER_PATTERN_BUTT; i++)
	{
		pstBayernr->au16CoarseStr[i] = hi_ext_system_bayernr_coarse_strength_read(i);
		pstBayernr->afStrength[i] = (HI_FLOAT)pstBayernr->au16CoarseStr[i] /(HI_FLOAT)HI_ISP_BAYERNR_STRENGTH_DIVISOR;
	}

	for(i = 0; i < HI_ISP_BAYERNR_LUT_LENGTH; i++)
	{
		pstBayernr->au16LutCoringLRatio[i] = hi_ext_system_bayernr_coring_low_ratio_read(i);
		pstBayernr->afLutCoringLRatio[i] = (HI_FLOAT)pstBayernr->au16LutCoringLRatio[i]/(HI_FLOAT)HI_ISP_BAYERNR_LUT_DIVISOR ;
	}
			
	if(OP_TYPE_AUTO == pstBayernr->enOpType)
	{
		for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
		{	
			pstBayernr->stAuto.au8ChromaStr[0][i] = hi_ext_system_bayernr_auto_chroma_strength_r_read(i);
			pstBayernr->stAuto.au8ChromaStr[1][i] =	hi_ext_system_bayernr_auto_chroma_strength_gr_read(i);
			pstBayernr->stAuto.au8ChromaStr[2][i] =  hi_ext_system_bayernr_auto_chroma_strength_gb_read(i);
			pstBayernr->stAuto.au8ChromaStr[3][i] =	hi_ext_system_bayernr_auto_chroma_strength_b_read(i);
			pstBayernr->stAuto.au8FineStr[i]      = hi_ext_system_bayernr_auto_fine_strength_read(i);		
			pstBayernr->stAuto.au16CoringWeight[i]= hi_ext_system_bayernr_auto_coring_weight_read(i);
			pstBayernr->afLutCoringL[i]	          = (HI_FLOAT)pstBayernr->stAuto.au16CoringWeight[i]/(HI_FLOAT)HI_ISP_BAYERNR_STRENGTH_DIVISOR; 
		}
	}
	else if(OP_TYPE_MANUAL == pstBayernr->enOpType)
	{
	    pstBayernr->stManual.u8ChromaStr[0]  = hi_ext_system_bayernr_manual_chroma_strength_read(0);
	    pstBayernr->stManual.u8ChromaStr[1]  = hi_ext_system_bayernr_manual_chroma_strength_read(1);
	    pstBayernr->stManual.u8ChromaStr[2]  = hi_ext_system_bayernr_manual_chroma_strength_read(2);
		pstBayernr->stManual.u8ChromaStr[3]  = hi_ext_system_bayernr_manual_chroma_strength_read(3);
	
		pstBayernr->stManual.u8FineStr       = hi_ext_system_bayernr_manual_fine_strength_read();
		pstBayernr->stManual.u16CoringWeight = hi_ext_system_bayernr_manual_coring_weight_read();
		for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
		{	
		pstBayernr->afLutCoringL[i] = (HI_FLOAT)pstBayernr->stManual.u16CoringWeight /(HI_FLOAT)HI_ISP_BAYERNR_STRENGTH_DIVISOR; 
		}
	}

	for(i=0;i< WDR_MAX_FRAME; i++)
	{
	  pstBayernr->stWDR.au8WDRCoarseStr[i]=  hi_ext_system_bayernr_wdr_strength_read(i);
	  pstBayernr->au8WDR_strengths[i]     = pstBayernr->stWDR.au8WDRCoarseStr[i];
	}

	pstBayernr->au16WDR_frameTHs[0] = hi_isp_wdr_flrgtth_low_read(IspDev);
	pstBayernr->au16WDR_frameTHs[1] = hi_isp_wdr_flrgtth_high_read(IspDev);

    return 0;
}

HI_S32 BayernrProcWrite(ISP_DEV IspDev, ISP_CTRL_PROC_WRITE_S *pstProc)
{
    ISP_CTRL_PROC_WRITE_S stProcTmp;
    ISP_BAYERNR_S *pstBayernr = HI_NULL;
    
    BAYERNR_GET_CTX(IspDev, pstBayernr);
 
    if ((HI_NULL == pstProc->pcProcBuff)
        || (0 == pstProc->u32BuffLen))
    {
        return HI_FAILURE;
    }

    stProcTmp.pcProcBuff = pstProc->pcProcBuff;
    stProcTmp.u32BuffLen = pstProc->u32BuffLen;

	BayernrReadExtregs(IspDev);
    
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "-----BAYERNR INFO-------------------------------------------------------------\n");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
            "%10s\n",
            "Enable");
	
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
            "%10d\n",
            pstBayernr->bEnable);

    pstProc->u32WriteLen += 1;
    
    return HI_SUCCESS;
}


HI_S32 ISP_BayernrInit(ISP_DEV IspDev)
{
    BayernrRegsInitialize(IspDev);
    BayernrExtRegsInitialize(IspDev); 
    return HI_SUCCESS;
}

HI_U8 getValueFromLut (HI_U32 x, HI_U32 * pLutX, HI_U8 * pLutY, HI_U32 length)
{
    HI_S32 j;

    if (x <= pLutX[0]) 
    {
        return pLutY[0];
    }

    for (j = 1; j < length; j++) 
	{
        if (x <= pLutX[j]) 
		{
			if (pLutY[j] < pLutY[j-1])
			{
				return pLutY[j-1] - (pLutY[j-1] - pLutY[j]) * (x - pLutX[j-1]) / DIV_0_TO_1(pLutX[j] - pLutX[j-1]);
			}
			else
			{				
				return pLutY[j-1] + (pLutY[j] - pLutY[j-1]) * (x - pLutX[j-1]) / DIV_0_TO_1(pLutX[j] - pLutX[j-1]);
			}            
        }
    }
    return pLutY[length-1];
}

HI_U16 getValueFromLut_U16 (HI_U32 x, HI_U32 * pLutX, HI_U16 * pLutY, HI_U32 length)
{
    HI_S32 j;

    if (x <= pLutX[0]) 
    {
        return pLutY[0];
    }

    for (j = 1; j < length; j++) 
	{
        if (x <= pLutX[j]) 
		{
			if (pLutY[j] < pLutY[j-1])
			{
				return pLutY[j-1] - (pLutY[j-1] - pLutY[j]) * (x - pLutX[j-1]) / DIV_0_TO_1(pLutX[j] - pLutX[j-1]);
			}
			else
			{				
				return pLutY[j-1] + (pLutY[j] - pLutY[j-1]) * (x - pLutX[j-1]) / DIV_0_TO_1(pLutX[j] - pLutX[j-1]);
			}            
        }
    }
    return pLutY[length-1];
}


HI_FLOAT getValueFromLut_f (HI_U32 x, HI_U32 * pLutX, HI_FLOAT * pLutY, HI_U32 length)
{
    HI_S32 j;

    if (x <= pLutX[0]) 
    {
        return pLutY[0];
    }
    for (j = 1; j < length; j++) 
	{
        if (x <= pLutX[j]) 
		{
			if (pLutY[j] < pLutY[j-1])
			{
				return pLutY[j-1] - (pLutY[j-1] - pLutY[j]) * (HI_FLOAT)(x - pLutX[j-1]) / (HI_FLOAT)(DIV_0_TO_1(pLutX[j] - pLutX[j-1]));
			}
			else
			{
				return pLutY[j-1] + (pLutY[j] - pLutY[j-1]) * (HI_FLOAT)(x - pLutX[j-1]) / (HI_FLOAT)(DIV_0_TO_1(pLutX[j] - pLutX[j-1]));
			}            
        }
    }
    return pLutY[length-1];
}

#define  EPS (0.000001f)
#define  COL_ISO      0
#define  COL_K        1
static HI_FLOAT getKfromNoiseLut(HI_FLOAT (*pRecord)[2], HI_U16 recordNum, HI_S32 iso)
{
    HI_S32 i = 0;
    HI_FLOAT  y_diff , x_diff;
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


static HI_U8 BayernrGetIsoIndex(HI_U32 u32Iso)
{   
	HI_U8 u8Index;
	
    for (u8Index = 0; u8Index < ISP_AUTO_ISO_STRENGTH_NUM-1; u8Index++)
    {
        if (u32Iso <= g_au32BayernrIsoLut[u8Index])
        {
            break;
        }
    }
    return u8Index;
}

#if 0
static __inline HI_U16 BayernrOffsetCalculate(
	const HI_U16 u16Y2,
	const HI_U16 u16Y1,
	const HI_U32 u32X2,
	const HI_U32 u32X1,
	const HI_U32 u32Iso)    
{
	HI_U32 u32Offset;
	if(u32X1==u32X2)
	{
		u32Offset = u16Y2;
	}
	else if(u16Y1 <= u16Y2)
	{
		u32Offset = u16Y1+(ABS(u16Y2 - u16Y1) * ABS(u32Iso - u32X1)+ ABS((u32X2 - u32X1)/2))/ABS((u32X2 - u32X1));
	}
	else if(u16Y1> u16Y2)
	{
		u32Offset = u16Y1-(ABS(u16Y2 - u16Y1) * ABS(u32Iso - u32X1)+ ABS((u32X2 - u32X1)/2))/ABS((u32X2 - u32X1));
	}
	return (HI_U16)u32Offset;
}
#endif

HI_S32 ISP_BayernrFw(HI_U32 u32Iso, ISP_DEV IspDev, ISP_BAYERNR_REG_CFG_S *pstBayernrCfg)
{
	HI_U8  u8AmedModeRg,u8AmedModeGr,u8AmedModeGb,u8AmedModeBg;
	HI_U8  u8ChromaRatio[BAYER_PATTER_NUM];	
	HI_U32 i, j;
	HI_U32 u32IsoLevel;
	HI_U32 maxLimit = (1 << HI_ISP_BAYERNR_MAXLMT) - 1;
	HI_U32 u32sigmaTemp;
    HI_FLOAT k, b,sigma;
	HI_FLOAT coringLtmp;
	HI_U32  u32sigma[BAYER_PATTER_NUM];
	HI_U32  u32Strength[BAYER_PATTER_NUM];
	HI_U16  u16CoringWeightActual;
	
	ISP_BAYERNR_S *pstBayernr = HI_NULL;
	ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
	ISP_SensorGetDefault(IspDev, &pstSnsDft);
	BAYERNR_GET_CTX(IspDev, pstBayernr);

	k = 256 * getKfromNoiseLut(pstSnsDft->stBayerNr.afCalibrationCoef, pstSnsDft->stBayerNr.u16CalibrationLutNum, u32Iso);
	b = getValueFromLut_f (u32Iso, g_au32BayernrIsoLut, g_afLutB, ISP_AUTO_ISO_STRENGTH_NUM);

    // Noise LUT
	u32Strength[0] = (HI_U32)(pstBayernr->afStrength[0] * 128);		
	u32Strength[1] = (HI_U32)(pstBayernr->afStrength[1] * 128);
	u32Strength[2] = (HI_U32)(pstBayernr->afStrength[2] * 128);
	u32Strength[3] = (HI_U32)(pstBayernr->afStrength[3] * 128);
	
    for (j = 0; j < HI_ISP_BAYERNR_LUT_LENGTH; j++) 
	{
		sigma = 2*k*j + 64*b;
        sigma = sqrtf(sigma);
		u32sigmaTemp = (HI_U32)(sigma * (1 << 13));
		
		u32sigma[0] = (u32sigmaTemp * u32Strength[0] * 3 + 1024) >> 20;	
		u32sigma[1] = (u32sigmaTemp * u32Strength[1] * 3 + 1024) >> 20;	
		u32sigma[2] = (u32sigmaTemp * u32Strength[2] * 3 + 1024) >> 20;	
		u32sigma[3] = (u32sigmaTemp * u32Strength[3] * 3 + 1024) >> 20;	
		pstBayernrCfg->u16JnlmLimit0Lut[0][j] = (pstBayernrCfg->u16JnlmLimit0Lut[0][j] > maxLimit) ? maxLimit:u32sigma[0];
		pstBayernrCfg->u16JnlmLimit0Lut[1][j] = (pstBayernrCfg->u16JnlmLimit0Lut[1][j] > maxLimit) ? maxLimit:u32sigma[1];
		pstBayernrCfg->u16JnlmLimit0Lut[2][j] = (pstBayernrCfg->u16JnlmLimit0Lut[2][j] > maxLimit) ? maxLimit:u32sigma[2];
		pstBayernrCfg->u16JnlmLimit0Lut[3][j] = (pstBayernrCfg->u16JnlmLimit0Lut[3][j] > maxLimit) ? maxLimit:u32sigma[3];

		u32sigma[0] = (u32sigmaTemp * u32Strength[0] * 4344 + (1 << 15)) >> 30;
		u32sigma[1] = (u32sigmaTemp * u32Strength[1] * 4344 + (1 << 15)) >> 30;
		u32sigma[2] = (u32sigmaTemp * u32Strength[2] * 4344 + (1 << 15)) >> 30;
		u32sigma[3] = (u32sigmaTemp * u32Strength[3] * 4344 + (1 << 15)) >> 30;
        pstBayernrCfg->u16JnlmLimit1Lut[0][j] = (pstBayernrCfg->u16JnlmLimit1Lut[0][j] > maxLimit) ? maxLimit:u32sigma[0];
        pstBayernrCfg->u16JnlmLimit1Lut[1][j] = (pstBayernrCfg->u16JnlmLimit1Lut[1][j] > maxLimit) ? maxLimit:u32sigma[1];
        pstBayernrCfg->u16JnlmLimit1Lut[2][j] = (pstBayernrCfg->u16JnlmLimit1Lut[2][j] > maxLimit) ? maxLimit:u32sigma[2];
        pstBayernrCfg->u16JnlmLimit1Lut[3][j] = (pstBayernrCfg->u16JnlmLimit1Lut[3][j] > maxLimit) ? maxLimit:u32sigma[3];
	}

	for (i = 0; i < BAYER_PATTERN_BUTT; i++)
	{
		pstBayernrCfg->u16JnlmLimit0Lut[i][0] = pstBayernrCfg->u16JnlmLimit0Lut[i][2];
		pstBayernrCfg->u16JnlmLimit0Lut[i][1] = pstBayernrCfg->u16JnlmLimit0Lut[i][2];
	
		pstBayernrCfg->u16JnlmLimit1Lut[i][0] = pstBayernrCfg->u16JnlmLimit1Lut[i][2];
		pstBayernrCfg->u16JnlmLimit1Lut[i][1] = pstBayernrCfg->u16JnlmLimit1Lut[i][2];
	}

	for (j = 0; j < ISP_AUTO_ISO_STRENGTH_NUM; j++) 
	{
	    u8ChromaRatio[BAYER_PATTERN_RG] = getValueFromLut (u32Iso, g_au32BayernrIsoLut, pstBayernr->au8ChromaRatio[0], ISP_AUTO_ISO_STRENGTH_NUM);
		u8ChromaRatio[BAYER_PATTERN_GR] = getValueFromLut (u32Iso, g_au32BayernrIsoLut, pstBayernr->au8ChromaRatio[1], ISP_AUTO_ISO_STRENGTH_NUM);
		u8ChromaRatio[BAYER_PATTERN_GB] = getValueFromLut (u32Iso, g_au32BayernrIsoLut, pstBayernr->au8ChromaRatio[2], ISP_AUTO_ISO_STRENGTH_NUM);
		u8ChromaRatio[BAYER_PATTERN_BG] = getValueFromLut (u32Iso, g_au32BayernrIsoLut, pstBayernr->au8ChromaRatio[3], ISP_AUTO_ISO_STRENGTH_NUM);

		pstBayernrCfg->u32RawnrCRatio  = ((HI_U32)(u8ChromaRatio[BAYER_PATTERN_RG]&0x3f) << (BAYER_PATTERN_RG*8));
		pstBayernrCfg->u32RawnrCRatio |= ((HI_U32)(u8ChromaRatio[BAYER_PATTERN_GR]&0x3f) << (BAYER_PATTERN_GR*8));
		pstBayernrCfg->u32RawnrCRatio |= ((HI_U32)(u8ChromaRatio[BAYER_PATTERN_GB]&0x3f) << (BAYER_PATTERN_GB*8));
		pstBayernrCfg->u32RawnrCRatio |= ((HI_U32)(u8ChromaRatio[BAYER_PATTERN_BG]&0x3f) << (BAYER_PATTERN_BG*8));
	}

    // AMED filter    
	if (OP_TYPE_AUTO == pstBayernr->enOpType)
	{
	   if (u32Iso > g_au32BayernrIsoLut[ISP_AUTO_ISO_STRENGTH_NUM-1])
		{
			u32IsoLevel = ISP_AUTO_ISO_STRENGTH_NUM-1;
		}
		else if (u32Iso <= g_au32BayernrIsoLut[0])
		{
			u32IsoLevel = 0;
		}	
		else
		{
			u32IsoLevel = BayernrGetIsoIndex(u32Iso);
		}
	    
		pstBayernrCfg->u8AmedLevel[0] = pstBayernr->stAuto.au8ChromaStr[0][u32IsoLevel];
		pstBayernrCfg->u8AmedLevel[1] = pstBayernr->stAuto.au8ChromaStr[1][u32IsoLevel];
		pstBayernrCfg->u8AmedLevel[2] = pstBayernr->stAuto.au8ChromaStr[2][u32IsoLevel];
		pstBayernrCfg->u8AmedLevel[3] = pstBayernr->stAuto.au8ChromaStr[3][u32IsoLevel];

	}
	else
	{
		pstBayernrCfg->u8AmedLevel[0] =  pstBayernr->stManual.u8ChromaStr[0];
		pstBayernrCfg->u8AmedLevel[1] =  pstBayernr->stManual.u8ChromaStr[1];
		pstBayernrCfg->u8AmedLevel[2] =  pstBayernr->stManual.u8ChromaStr[2];
		pstBayernrCfg->u8AmedLevel[3] =  pstBayernr->stManual.u8ChromaStr[3];			
	}

	u8AmedModeRg = (u32Iso < 5000) ? 0 : (u32Iso < 50000) ? 1 : 1;
	u8AmedModeGr = (u32Iso < 5000) ? 0 : (u32Iso < 50000) ? 0 : 0;
	u8AmedModeGb = (u32Iso < 5000) ? 0 : (u32Iso < 50000) ? 0 : 0;
	u8AmedModeBg = (u32Iso < 5000) ? 0 : (u32Iso < 50000) ? 1 : 1;
	
	pstBayernrCfg->u8AmedMode  = (u8AmedModeRg<<BAYER_PATTERN_RG);
	pstBayernrCfg->u8AmedMode |= (u8AmedModeGr<<BAYER_PATTERN_GR);
	pstBayernrCfg->u8AmedMode |= (u8AmedModeGb<<BAYER_PATTERN_GB);
	pstBayernrCfg->u8AmedMode |= (u8AmedModeBg<<BAYER_PATTERN_BG);

	if (OP_TYPE_AUTO == pstBayernr->enOpType)
	{
		pstBayernrCfg->u8JnlmGain = getValueFromLut(u32Iso, g_au32BayernrIsoLut, pstBayernr->stAuto.au8FineStr, ISP_AUTO_ISO_STRENGTH_NUM);
		u16CoringWeightActual = getValueFromLut_U16(u32Iso, g_au32BayernrIsoLut,pstBayernr->stAuto.au16CoringWeight,ISP_AUTO_ISO_STRENGTH_NUM);

	}
	else
	{
		pstBayernrCfg->u8JnlmGain = pstBayernr->stManual.u8FineStr;
		u16CoringWeightActual = pstBayernr->stManual.u16CoringWeight;
		
	}

	{
		hi_ext_system_bayernr_actual_chroma_strength_write(0,pstBayernrCfg->u8AmedLevel[0]);
		hi_ext_system_bayernr_actual_chroma_strength_write(1,pstBayernrCfg->u8AmedLevel[1]);
		hi_ext_system_bayernr_actual_chroma_strength_write(2,pstBayernrCfg->u8AmedLevel[2]);
		hi_ext_system_bayernr_actual_chroma_strength_write(3,pstBayernrCfg->u8AmedLevel[3]);
		hi_ext_system_bayernr_actual_fine_strength_write(pstBayernrCfg->u8JnlmGain);
		hi_ext_system_bayernr_actual_coring_weight_write(u16CoringWeightActual);

	}

    // JNLM filter
    pstBayernrCfg->u16JnlmCoringLow = (HI_U16) (256 * getValueFromLut_f (u32Iso, g_au32BayernrIsoLut, pstBayernr->afLutCoringL, ISP_AUTO_ISO_STRENGTH_NUM));
    pstBayernrCfg->u16JnlmCoringHig = (HI_U16) (256 * getValueFromLut_f (u32Iso, g_au32BayernrIsoLut, pstBayernr->afLutCoringH, ISP_AUTO_ISO_STRENGTH_NUM));    
	pstBayernrCfg->u8WdrFrameNum = pstBayernr->u8WdrFramesMerge;

	// luma adaptive coring
    coringLtmp = 256 * getValueFromLut_f (u32Iso, g_au32BayernrIsoLut, pstBayernr->afLutCoringL, ISP_AUTO_ISO_STRENGTH_NUM);
    for (i=0; i<HI_ISP_BAYERNR_LUT_LENGTH; i++)
    {
        pstBayernrCfg->u16JNLMCoringLowLUT[i] = (HI_U16)(coringLtmp*pstBayernr->afLutCoringLRatio[i]);
    }

	pstBayernrCfg->u8Enable  = pstBayernr->bEnable;
	pstBayernrCfg->u16Rgain = hi_isp_white_balance_gain_r_read(IspDev);
	pstBayernrCfg->u16Bgain = hi_isp_white_balance_gain_b_read(IspDev);
	
    return	HI_SUCCESS;
}

// WDR FW: ADJ_C(2) + ADJ_D(4) = 6
#define  ADJ_C  2
#define  ADJ_D  4

HI_S32 BCOM(HI_S64 x) 
{
	HI_S64 out = (x << 22) / DIV_0_TO_1((x << 6) + (((1 << 20) - x) << ADJ_C));
	return (HI_S32)out;
}

// 16bit -> 20bit
HI_S32 BDEC(HI_S64 y) 
{
	HI_S64 out = (y << 26) / DIV_0_TO_1((y << 6) + (((1 << 16) - y) << (ADJ_D + 6)));
	return (HI_S32)out;
}

HI_S32 hiisp_bayernr_fw_wdr(HI_U32 u32Iso, ISP_DEV IspDev, ISP_BAYERNR_REG_CFG_S *pstBayernrCfg)
{
	HI_U8 n, i, j;
	HI_U8  u8AmedModeRg,u8AmedModeGr,u8AmedModeGb,u8AmedModeBg;
	HI_U8 u8ChromaRatio[BAYER_PATTER_NUM];
	HI_S32 s32WDR_PxValue ,s32n8b_cur_axs;
	HI_U32 u32IsoLevel;
    HI_U32 au32WDR_frameTHs[2];
	HI_FLOAT sigma , cprClip;
	//luma adaptive coring
    HI_FLOAT coringLtmp;
	HI_FLOAT k, strength;
	HI_FLOAT n8b_pre_pos, n8b_pre_axs;
	HI_S32 s32n8b_pre_pos, s32n8b_pre_axs;
    HI_U32 maxLimit = (1 << HI_ISP_BAYERNR_MAXLMT) - 1; 
	HI_U32 u32LutVal;
	HI_U16  u16CoringWeightActual;
	
	ISP_BAYERNR_S *pstBayernr = HI_NULL;
	ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
	ISP_SensorGetDefault(IspDev, &pstSnsDft);
	BAYERNR_GET_CTX(IspDev, pstBayernr);

	strength = (HI_DOUBLE)hi_ext_system_bayernr_manual_fine_strength_read()/(HI_DOUBLE)HI_ISP_BAYERNR_STRENGTH_DIVISOR;

    // WDR info	
	cprClip = 1.0f / ISP_BITFIX(ADJ_D); //the slope of the beginning of Compression curve
  
    // IR info
    // Noise LUT
    // 34220: fixed
	k = getKfromNoiseLut(pstSnsDft->stBayerNr.afCalibrationCoef, pstSnsDft->stBayerNr.u16CalibrationLutNum, u32Iso);
    
    if(2 == pstBayernr->u8WdrFramesMerge)
    {
		// intensity threshold
		pstBayernr->au16ExpoValues[0] = 1;
		pstBayernr->au16ExpoValues[1] = (hi_isp_wdr_exporatio0_read(IspDev)+32)/64;

		if (0 == pstBayernr->au16ExpoValues[1])
		{
			pstBayernr->au16ExpoValues[1] = 1;
		}        

		au32WDR_frameTHs[0] = (HI_S32)((pstBayernr->au16WDR_frameTHs[0] << 6) / DIV_0_TO_1(pstBayernr->au16ExpoValues[1])) + pstBayernr->u16WDR_BlcThresh;
		au32WDR_frameTHs[1] = (HI_S32)((pstBayernr->au16WDR_frameTHs[1] << 6) / DIV_0_TO_1(pstBayernr->au16ExpoValues[1])) + pstBayernr->u16WDR_BlcThresh;

       	pstBayernr->au16WDR_frameTHs[0]  = BCOM((HI_S64)au32WDR_frameTHs[0]);
		pstBayernr->au16WDR_frameTHs[1]  = BCOM((HI_S64)au32WDR_frameTHs[1]);        

		for(n = 0; n <= 128; n++) 
		{				
			s32WDR_PxValue = n << 9;
			s32n8b_cur_axs = n << 9;

			s32n8b_pre_pos = MAX2((BDEC(s32n8b_cur_axs) - (HI_S32)pstBayernr->u16WDR_BlcThresh), 0); 
			s32n8b_pre_axs = MAX2(BDEC(s32n8b_cur_axs), 0); 
			n8b_pre_pos = (HI_FLOAT)s32n8b_pre_pos / (HI_DOUBLE)(ISP_BITFIX(12));
			n8b_pre_axs = (HI_FLOAT)s32n8b_pre_axs / (HI_DOUBLE)(ISP_BITFIX(12));

			sigma  = (HI_FLOAT)(k * (n8b_pre_pos + pstBayernr->u8WDR_eInitBlcNR));                                                     
			sigma *= (HI_FLOAT)pow((s32n8b_cur_axs >> 8) / MAX2(n8b_pre_axs, cprClip), 2);                               
 
			sigma  =  sqrtf(sigma);
			sigma *=  (HI_FLOAT)strength;

			if(s32WDR_PxValue < pstBayernr->au16WDR_frameTHs[0])  
			{
				
				sigma /= sqrtf(DIV_0_TO_1(pstBayernr->au16ExpoValues[1])); 
				sigma  = sigma * pstBayernr->au8WDR_strengths[1] / 16;
			}
			else if(s32WDR_PxValue > pstBayernr->au16WDR_frameTHs[0] && s32WDR_PxValue < pstBayernr->au16WDR_frameTHs[1])
			{
				HI_FLOAT sgmFS = sigma / sqrtf(DIV_0_TO_1(pstBayernr->au16ExpoValues[0]));
				HI_FLOAT sgmFL = sigma / sqrtf(DIV_0_TO_1(pstBayernr->au16ExpoValues[1])); 
				HI_FLOAT bldr  = (HI_FLOAT)(s32WDR_PxValue - (HI_S32)pstBayernr->au16WDR_frameTHs[0]) / MAX2((HI_S32)pstBayernr->au16WDR_frameTHs[1] - (HI_S32)pstBayernr->au16WDR_frameTHs[0], 1);

				sgmFS = sgmFS * pstBayernr->au8WDR_strengths[0] / 16; 
				sgmFL = sgmFL * pstBayernr->au8WDR_strengths[1] / 16; 
				sigma = sgmFS * bldr + sgmFL * (1.0f - bldr);
			}
			else
			{  
				sigma /= sqrtf(DIV_0_TO_1(pstBayernr->au16ExpoValues[0]));
				sigma  = sigma * pstBayernr->au8WDR_strengths[0] / 16;
			}
			
			u32LutVal = (HI_U32)(sigma*3*4 + 0.5);
			
			pstBayernrCfg->u16JnlmLimit0Lut[n/33][n%33] = (u32LutVal > maxLimit) ? maxLimit : u32LutVal;

			u32LutVal = (HI_U32)(sigma * 1.414 * 3 + 0.5);
			pstBayernrCfg->u16JnlmLimit1Lut[n/33][n%33] = (u32LutVal > maxLimit) ? maxLimit : u32LutVal;
		}
    } 


	if(0 == pstBayernr->u8WDR_eInitBlcNR)
	{
		pstBayernrCfg->u16JnlmLimit0Lut[0][0] = pstBayernrCfg->u16JnlmLimit0Lut[0][3];
		pstBayernrCfg->u16JnlmLimit0Lut[0][1] = pstBayernrCfg->u16JnlmLimit0Lut[0][3];
		pstBayernrCfg->u16JnlmLimit0Lut[0][2] = pstBayernrCfg->u16JnlmLimit0Lut[0][3];
	}
	
	// Separation filter 
	for (j = 0; j < ISP_AUTO_ISO_STRENGTH_NUM; j++) 
	{
	    u8ChromaRatio[BAYER_PATTERN_RG] = getValueFromLut (u32Iso, g_au32BayernrIsoLut, pstBayernr->au8ChromaRatio[0], ISP_AUTO_ISO_STRENGTH_NUM);
		u8ChromaRatio[BAYER_PATTERN_GR] = getValueFromLut (u32Iso, g_au32BayernrIsoLut, pstBayernr->au8ChromaRatio[1], ISP_AUTO_ISO_STRENGTH_NUM);
		u8ChromaRatio[BAYER_PATTERN_GB] = getValueFromLut (u32Iso, g_au32BayernrIsoLut, pstBayernr->au8ChromaRatio[2], ISP_AUTO_ISO_STRENGTH_NUM);
		u8ChromaRatio[BAYER_PATTERN_BG] = getValueFromLut (u32Iso, g_au32BayernrIsoLut, pstBayernr->au8ChromaRatio[3], ISP_AUTO_ISO_STRENGTH_NUM);

		pstBayernrCfg->u32RawnrCRatio  = ((HI_U32)(u8ChromaRatio[BAYER_PATTERN_RG]&0x3f) << (BAYER_PATTERN_RG*8));
		pstBayernrCfg->u32RawnrCRatio |= ((HI_U32)(u8ChromaRatio[BAYER_PATTERN_GR]&0x3f) << (BAYER_PATTERN_GR*8));
		pstBayernrCfg->u32RawnrCRatio |= ((HI_U32)(u8ChromaRatio[BAYER_PATTERN_GB]&0x3f) << (BAYER_PATTERN_GB*8));
		pstBayernrCfg->u32RawnrCRatio |= ((HI_U32)(u8ChromaRatio[BAYER_PATTERN_BG]&0x3f) << (BAYER_PATTERN_BG*8));
	}


    // AMED filter    
	if (OP_TYPE_AUTO == pstBayernr->enOpType)
	{
		if (u32Iso > g_au32BayernrIsoLut[ISP_AUTO_ISO_STRENGTH_NUM-1])
		{
			u32IsoLevel = ISP_AUTO_ISO_STRENGTH_NUM-1;
		}
		else if (u32Iso <= g_au32BayernrIsoLut[0])
		{
			u32IsoLevel = 0;
		}	
		else
		{
			u32IsoLevel = BayernrGetIsoIndex(u32Iso);
		}
		pstBayernrCfg->u8AmedLevel[0] = pstBayernr->stAuto.au8ChromaStr[0][u32IsoLevel];	
		pstBayernrCfg->u8AmedLevel[1] = pstBayernr->stAuto.au8ChromaStr[1][u32IsoLevel];	
		pstBayernrCfg->u8AmedLevel[2] = pstBayernr->stAuto.au8ChromaStr[2][u32IsoLevel];	
		pstBayernrCfg->u8AmedLevel[3] = pstBayernr->stAuto.au8ChromaStr[3][u32IsoLevel];	
	}
	else
	{
		pstBayernrCfg->u8AmedLevel[0] = pstBayernr->stManual.u8ChromaStr[0];		
		pstBayernrCfg->u8AmedLevel[1] = pstBayernr->stManual.u8ChromaStr[1];		
		pstBayernrCfg->u8AmedLevel[2] = pstBayernr->stManual.u8ChromaStr[2];		
		pstBayernrCfg->u8AmedLevel[3] = pstBayernr->stManual.u8ChromaStr[3];		
	}

	u8AmedModeRg = (u32Iso < 5000) ? 0 : (u32Iso < 50000) ? 1 : 1;
	u8AmedModeGr = (u32Iso < 5000) ? 0 : (u32Iso < 50000) ? 0 : 0;
	u8AmedModeGb = (u32Iso < 5000) ? 0 : (u32Iso < 50000) ? 0 : 0;
	u8AmedModeBg = (u32Iso < 5000) ? 0 : (u32Iso < 50000) ? 1 : 1;

	pstBayernrCfg->u8AmedMode  = (u8AmedModeRg<<BAYER_PATTERN_RG);
	pstBayernrCfg->u8AmedMode |= (u8AmedModeGr<<BAYER_PATTERN_GR);
	pstBayernrCfg->u8AmedMode |= (u8AmedModeGb<<BAYER_PATTERN_GB);
	pstBayernrCfg->u8AmedMode |= (u8AmedModeBg<<BAYER_PATTERN_BG);

	if (OP_TYPE_AUTO == pstBayernr->enOpType)
	{
		pstBayernrCfg->u8JnlmGain = getValueFromLut(u32Iso, g_au32BayernrIsoLut, pstBayernr->stAuto.au8FineStr, ISP_AUTO_ISO_STRENGTH_NUM);
		u16CoringWeightActual = getValueFromLut_U16(u32Iso, g_au32BayernrIsoLut,pstBayernr->stAuto.au16CoringWeight,ISP_AUTO_ISO_STRENGTH_NUM);
	}
	else
	{	
		pstBayernrCfg->u8JnlmGain = pstBayernr->stManual.u8FineStr;
		u16CoringWeightActual = pstBayernr->stManual.u16CoringWeight;
		
	}



	{
		hi_ext_system_bayernr_actual_chroma_strength_write(0,pstBayernrCfg->u8AmedLevel[0]);
		hi_ext_system_bayernr_actual_chroma_strength_write(1,pstBayernrCfg->u8AmedLevel[1]);
		hi_ext_system_bayernr_actual_chroma_strength_write(2,pstBayernrCfg->u8AmedLevel[2]);
		hi_ext_system_bayernr_actual_chroma_strength_write(3,pstBayernrCfg->u8AmedLevel[3]);
		hi_ext_system_bayernr_actual_fine_strength_write(pstBayernrCfg->u8JnlmGain);
		hi_ext_system_bayernr_actual_coring_weight_write(u16CoringWeightActual);
	
	}

	
    // JNLM filter
    pstBayernrCfg->u16JnlmCoringLow = (HI_U16) (256 * getValueFromLut_f (u32Iso, g_au32BayernrIsoLut, pstBayernr->afLutCoringL, ISP_AUTO_ISO_STRENGTH_NUM));
    pstBayernrCfg->u16JnlmCoringHig = (HI_U16) (256 * getValueFromLut_f (u32Iso, g_au32BayernrIsoLut, pstBayernr->afLutCoringH, ISP_AUTO_ISO_STRENGTH_NUM));  

	// luma adaptive coring
    coringLtmp = 256 * getValueFromLut_f (u32Iso, g_au32BayernrIsoLut, pstBayernr->afLutCoringL, ISP_AUTO_ISO_STRENGTH_NUM);
    for (i=0; i<HI_ISP_BAYERNR_LUT_LENGTH; i++)
    {
        pstBayernrCfg->u16JNLMCoringLowLUT[i] = (HI_U16)(coringLtmp*pstBayernr->afLutCoringLRatio[i]);
    }
           
    pstBayernrCfg->u8WdrFrameNum = pstBayernr->u8WdrFramesMerge;
	pstBayernrCfg->u8Enable  = pstBayernr->bEnable;

	pstBayernrCfg->u16Rgain = hi_isp_white_balance_gain_r_read(IspDev);
	pstBayernrCfg->u16Bgain = hi_isp_white_balance_gain_b_read(IspDev);
	
    return HI_SUCCESS;
}

HI_S32 ISP_BayernrRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{    
	ISP_REG_CFG_S *pstReg;
    ISP_CTX_S *pstIspCtx = HI_NULL;
	ISP_BAYERNR_S *pstBayernr = HI_NULL;

	BAYERNR_GET_CTX(IspDev, pstBayernr);
    ISP_GET_CTX(IspDev, pstIspCtx);

    /* calculate every two interrupts */
	if (!pstBayernr->bInit)
	{
		return HI_SUCCESS;
	}
	
    if (0 != pstIspCtx->u32FrameCnt % 2)
    {
        return HI_SUCCESS;
    }

	BayernrReadExtregs(IspDev);

	pstReg = (ISP_REG_CFG_S *)pRegCfg;

    if (HI_TRUE == pstBayernr->bEnable)
    {
		if (pstBayernr->u8WdrFramesMerge > 1)
		{
			hiisp_bayernr_fw_wdr(pstIspCtx->stLinkage.u32Iso, IspDev, &pstReg->stBayernrRegCfg);
		}
		else
		{
			ISP_BayernrFw(pstIspCtx->stLinkage.u32Iso, IspDev, &pstReg->stBayernrRegCfg);
		}	

		((ISP_REG_CFG_S *)pRegCfg)->unKey.bit1BayernrCfg = 1;	
    }
	else
	{
		hi_isp_bnr_en_write(IspDev, 0);
	}
	
	return HI_SUCCESS;
}


static __inline HI_S32 BayerNrImageResWrite(ISP_DEV IspDev, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstRes)
{
	hi_isp_bnr_width_write(IspDev, pstRes->u16Width - 1);
	hi_isp_bnr_height_write(IspDev, pstRes->u16Height -1);

    return HI_SUCCESS;
}


HI_S32 ISP_BayernrCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
	ISP_BAYERNR_S *pstBayernr = HI_NULL;
		
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :
			BAYERNR_GET_CTX(IspDev, pstBayernr);
			pstBayernr->bInit = HI_FALSE;
			ISP_BayernrInit(IspDev);
            break;
		case ISP_CHANGE_IMAGE_MODE_SET:   
			BayerNrImageResWrite(IspDev, (ISP_CMOS_SENSOR_IMAGE_MODE_S *)pValue);  
			break;
        case ISP_PROC_WRITE:
            BayernrProcWrite(IspDev, (ISP_CTRL_PROC_WRITE_S *)pValue);
            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_BayernrExit(ISP_DEV IspDev)
{
	hi_isp_bnr_en_write(IspDev, HI_FALSE);
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterBayernr(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_BAYERNR;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_BayernrInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_BayernrRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_BayernrCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_BayernrExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


