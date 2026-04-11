/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_frame_switch_wdr.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/07/14
  Description   : 
  History       :
  1.Date        : 2015/07/14
    Author      : q00214668
    Modification: Created file

******************************************************************************/
#include <math.h>
#include "isp_config.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_ext_config.h"
#include "isp_math_utils.h"
#include "isp_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define HI_WDR_BITDEPTH   (14)

// Reconstruction Config  
#define HI_ISP_WDR_EXPORATIO_DEFAULT    (0x200)

static HI_S32 g_as32WdrIsoLut[ISP_AUTO_ISO_STRENGTH_NUM]   = {100,200,400,800,1600,3200,6400,12800,25600,25600,25600,25600,25600,25600,25600,25600};
static HI_S32 g_as32lutMDTNoise[ISP_AUTO_ISO_STRENGTH_NUM] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};                          // u4.2
static HI_S32 g_as32lutMDTClips[ISP_AUTO_ISO_STRENGTH_NUM] = {250, 250, 250, 500, 800, 1024, 2048, 4096, 8192, 8192, 8192, 8192, 8192, 8192, 8192, 8192};   // u14.0
static HI_S32 g_as32lutMDTLowTh[ISP_AUTO_ISO_STRENGTH_NUM] = {6, 6, 6, 12, 16, 18, 22, 26, 32, 32, 32, 32, 32, 32, 32, 32};                   // u4.2
static HI_S32 g_as32lutMDTHigTh[ISP_AUTO_ISO_STRENGTH_NUM] = {16, 16, 16, 28, 32, 36, 44, 52, 63, 63, 63, 63, 63, 63, 63, 63};                 // u4.2
static HI_S32 g_as32lutMDTMaxTH[ISP_AUTO_ISO_STRENGTH_NUM] = {12, 12, 12, 7, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0};                         // u4.2

typedef struct hiISP_FS_WDR_S
{
	/* Public */
	// fw input	
	HI_BOOL		bMotionComp; 
	HI_BOOL		bMotionComMode;
	HI_BOOL		bMDRefNoise;
	HI_U16		u16ShortThr;
	HI_U16		u16LongThr;
	HI_U16		u16MDSfNrThr;        
	HI_U8		u8BitDepth;
	HI_U8    		u8FramesMerge;
	HI_U32  		u32PreIso;
	HI_S32  		s32PreMDTNoise;
	HI_S32  		as32LutMDTNoise[ISP_AUTO_ISO_STRENGTH_NUM];
	HI_S32  		as32LutMDTLowTh[ISP_AUTO_ISO_STRENGTH_NUM];
	HI_S32  		as32LutMDTHigTh[ISP_AUTO_ISO_STRENGTH_NUM];
	HI_S32  		as32LutMDTClips[ISP_AUTO_ISO_STRENGTH_NUM];
	HI_S32  		as32LutMDTMaxTH[ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_FS_WDR_S;

ISP_FS_WDR_S g_astFSWDRCtx[ISP_MAX_DEV_NUM] = {{0}};
#define FS_WDR_GET_CTX(dev, pstCtx)   pstCtx = &g_astFSWDRCtx[dev]

static HI_VOID FrameWDRExtRegsInitialize(ISP_DEV IspDev)
{
	HI_U8 i;
	ISP_FS_WDR_S *pstFSWDRCtx = HI_NULL;
	ISP_CMOS_BLACK_LEVEL_S  *pstSnsBlackLevel = HI_NULL;		

	FS_WDR_GET_CTX(IspDev, pstFSWDRCtx);
   	ISP_SensorGetBlc(IspDev, &pstSnsBlackLevel); 
	hi_ext_system_wdr_manual_mode_write(0);
	hi_ext_system_manual_wdr_lutmdtlowth_write(HI_EXT_SYSTEM_MANUAL_WDR_LUTMDTLOWTH_WRITE_DEFAULT);
	hi_ext_system_manual_wdr_lutmdthigth_write(HI_EXT_SYSTEM_MANUAL_WDR_LUTMDTHIGTH_WRITE_DEFAULT);
	hi_ext_system_manual_wdr_lutmdtclip_write (HI_EXT_SYSTEM_MANUAL_WDR_LUTMDTCLIP_WRITE_DEFAULT );
	hi_ext_system_manual_wdr_mdtmaxth_write(HI_EXT_SYSTEM_MANUAL_WDR_MDTMAXTH_WRITE_DEFAULT);

	hi_ext_system_wdr_mdtcomp_write(HI_EXT_SYSTEM_WDR_MDTCOMP_WRITE_DEFAULT);
	hi_ext_system_wdr_mdtnosref_write(HI_EXT_SYSTEM_WDR_MDTNOSREF_WRITE_DEFAULT);
	hi_ext_system_wdr_mdtcomp_mode_write(HI_EXT_SYSTEM_WDR_MDTCOMP_MODE_WRITE_DEFAULT);
	hi_ext_system_wdr_longthr_write(HI_EXT_SYSTEM_WDR_LONGTHR_WRITE_DEFAULT);
   	hi_ext_system_wdr_shortthr_write(HI_EXT_SYSTEM_WDR_SHORTTHR_WRITE_DEFAULT);
    	hi_ext_system_wdr_snthr_write(HI_EXT_SYSTEM_WDR_SHORTNOSTHR_WRITE_DEFAULT);

	hi_ext_system_wdr_blc0_write(pstSnsBlackLevel->au16BlackLevel[0] << 2);
	hi_ext_system_wdr_blc1_write(pstSnsBlackLevel->au16BlackLevel[1] << 2);	
    hi_ext_system_chnswitch_write(HI_EXT_SYSTEM_CHNSWITCH_WRITE_DEFAULT);

	hi_ext_system_wdr_out_blc_write(pstSnsBlackLevel->au16BlackLevel[0] << 2);	
	for (i=0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
	{
		pstFSWDRCtx->as32LutMDTNoise[i] 	=  g_as32lutMDTNoise[i];
		pstFSWDRCtx->as32LutMDTLowTh[i]	=  g_as32lutMDTLowTh[i];
		pstFSWDRCtx->as32LutMDTHigTh[i] 	=  g_as32lutMDTHigTh[i];
		pstFSWDRCtx->as32LutMDTClips[i] 	=  g_as32lutMDTClips[i];
		pstFSWDRCtx->as32LutMDTMaxTH[i]	=  g_as32lutMDTMaxTH[i];
		
		hi_ext_system_wdr_lutmdtnoise_write	(i, pstFSWDRCtx->as32LutMDTNoise[i]);
		hi_ext_system_wdr_lutmdtlowth_write	(i, pstFSWDRCtx->as32LutMDTLowTh[i]);
		hi_ext_system_wdr_lutmdthigth_write	(i, pstFSWDRCtx->as32LutMDTHigTh[i]);
		hi_ext_system_wdr_lutmdtclip_write	(i, pstFSWDRCtx->as32LutMDTClips[i]);
		hi_ext_system_wdr_mdtmaxth_write   	(i, pstFSWDRCtx->as32LutMDTMaxTH[i]);
	}

	return;
}

static HI_VOID FrameWDRRegsInitialize(ISP_DEV IspDev)
{	
	HI_U32 u32ExpoRatio;
	ISP_FS_WDR_S *pstFSWDRCtx;

	FS_WDR_GET_CTX(IspDev, pstFSWDRCtx);
	
	hi_isp_wdr_bmdtstrong_write	(IspDev, HI_ISP_WDR_MDT_STRONG_DEFAULT	);
	hi_isp_wdr_fl_bmdtmnu_write	(IspDev, HI_ISP_WDR_FL_MDTMNU_DEFAULT	);
	hi_isp_wdr_zoomblc_write     	(IspDev, HI_ISP_WDR_ZOOMBLC_DEFAULT	);
	hi_isp_wdr_bldrlhfidx_write   	(IspDev, HI_ISP_WDR_BLDRLHFIDX_DEFAULT	);
	hi_isp_wdr_bldrclridx_write	(IspDev, HI_ISP_WDR_BLDRCLRIDX_DEFAULT	);
	hi_isp_wdr_flrgtth_low_write 	(IspDev, HI_ISP_WDR_RGTLOWTHFL_DEFAULT	);
	hi_isp_wdr_flrgtth_high_write	(IspDev, HI_ISP_WDR_RGTHIGTHFL_DEFAULT	);
	hi_isp_wdr_fl_dftwgt_write   	(IspDev, HI_ISP_WDR_DFTWGTTHFL_DEFAULT	);
	hi_isp_wdr_bmdthf_write      	(IspDev, HI_ISP_WDR_BMDTHF_DEFAULT		);
	hi_isp_wdr_fsnr_judge_write   (IspDev, HI_ISP_WDR_FSNR_JUDGE_DEFAULT	);
	hi_isp_wdr_fsnrth_high_write 	(IspDev, HI_ISP_WDR_FSNRTH_HIGHT_DEFAULT);
	hi_isp_wdr_fsnrth_low_write  	(IspDev, HI_ISP_WDR_FSNRTH_LOW_DEFAULT	);
	hi_isp_wdr_fsnrgn_high_write 	(IspDev, HI_ISP_WDR_FSNRGN_HIGH_DEFAULT);
	hi_isp_wdr_fsnrgn_low_write  	(IspDev, HI_ISP_WDR_FSNRGN_LOW_DEFAULT	);
	hi_isp_wdr_bmdtrefnos_write  (IspDev, HI_ISP_WDR_BMDTREFNOS_DEFAULT	);
	hi_isp_wdr_balgprocmdt_write	(IspDev, HI_ISP_WDR_FL_MDTPROC_DEFAULT	);
	hi_isp_wdr_width_write 		(IspDev, hi_ext_sync_total_width_read () - 1);
	hi_isp_wdr_height_write		(IspDev, hi_ext_sync_total_height_read() - 1);
	
	pstFSWDRCtx->u8BitDepth  		= HI_WDR_BITDEPTH;	
	pstFSWDRCtx->u32PreIso    	= 0;
	pstFSWDRCtx->s32PreMDTNoise 	= 0;
	
	u32ExpoRatio = CLIP3((HI_S32)(ISP_BITFIX(10) * 64.0 / HI_ISP_WDR_EXPORATIO_DEFAULT), 0, ISP_BITFIX(10));
	hi_isp_wdr_exporatio0_write(IspDev, HI_ISP_WDR_EXPORATIO_DEFAULT);
	hi_isp_flick_exp_ratios0_write(IspDev, HI_ISP_WDR_EXPORATIO_DEFAULT);
	hi_isp_wdr_exporatio_r_write(IspDev, u32ExpoRatio);
		
    return;
}

static HI_S32 FrameWDRReadExtRegs(ISP_DEV IspDev)
{
	HI_U8 i;
	ISP_FS_WDR_S *pstFSWDRCtx;

	FS_WDR_GET_CTX(IspDev, pstFSWDRCtx);	

	pstFSWDRCtx->bMotionComp	=	hi_ext_system_wdr_mdtcomp_read();
	pstFSWDRCtx->bMDRefNoise	=	hi_ext_system_wdr_mdtnosref_read();
	pstFSWDRCtx->u16LongThr		=	hi_isp_wdr_flrgtth_low_read(IspDev);
	pstFSWDRCtx->u16ShortThr		=	hi_isp_wdr_flrgtth_high_read(IspDev);
	pstFSWDRCtx->u16MDSfNrThr	=	hi_ext_system_wdr_snthr_read();
	pstFSWDRCtx->bMotionComMode=	hi_ext_system_wdr_mdtcomp_mode_read();

	for (i=0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
	{
		pstFSWDRCtx->as32LutMDTNoise[i]	=  hi_ext_system_wdr_lutmdtnoise_read(i);	
		pstFSWDRCtx->as32LutMDTLowTh[i] 	=  hi_ext_system_wdr_lutmdtlowth_read(i);	
		pstFSWDRCtx->as32LutMDTHigTh[i] 	=  hi_ext_system_wdr_lutmdthigth_read(i);		
		pstFSWDRCtx->as32LutMDTClips[i] 	=  hi_ext_system_wdr_lutmdtclip_read (i);
		pstFSWDRCtx->as32LutMDTMaxTH[i] 	=  hi_ext_system_wdr_mdtmaxth_read (i);
	}
	return HI_SUCCESS;
}

HI_S32 ISP_FrameWDRInit(ISP_DEV IspDev)
{	
	HI_U8 u8WDRMode;
   	ISP_CTX_S *pstIspCtx = HI_NULL;
	ISP_FS_WDR_S *pstFSWDRCtx = HI_NULL;

	FS_WDR_GET_CTX(IspDev, pstFSWDRCtx);	
    	ISP_GET_CTX(IspDev, pstIspCtx);	
	u8WDRMode = pstIspCtx->u8SnsWDRMode;   
    
    	FrameWDRRegsInitialize(IspDev);
    	FrameWDRExtRegsInitialize(IspDev);
	if (IS_LINEAR_MODE(u8WDRMode) || IS_BUILT_IN_WDR_MODE(u8WDRMode))
    	{
        	pstFSWDRCtx->u8FramesMerge = 1;
		hi_isp_wdr_enable_write(IspDev, HI_FALSE);
		hi_isp_flick_en_write(IspDev, HI_FALSE);
		hi_isp_bcom_enable_write(IspDev, HI_FALSE);
		hi_isp_bdec_enable_write(IspDev, HI_FALSE);
    	}
    	else if (IS_2to1_WDR_MODE(u8WDRMode))
    	{			
    		pstFSWDRCtx->u8FramesMerge = 2;
		hi_isp_wdr_bsaveblc_write(IspDev, HI_TRUE);
		hi_isp_wdr_esaveblc_write(IspDev, pstFSWDRCtx->u8FramesMerge-1);
		hi_isp_wdr_enable_write(IspDev, HI_TRUE);
		hi_isp_flick_en_write(IspDev, HI_TRUE);
    	}
    	else
    	{
    		pstFSWDRCtx->u8FramesMerge = 1;
    		hi_isp_wdr_enable_write(IspDev, HI_FALSE);
		hi_isp_flick_en_write(IspDev, HI_FALSE);      
    	}

	hi_isp_flick_frm_merge_write(IspDev, pstFSWDRCtx->u8FramesMerge);
	return HI_SUCCESS;
}

static HI_S32 GetValueFromLut(HI_S32 x, HI_S32 *pLutX, HI_S32 *pLutY, HI_S32 length)
{
	HI_S32 n = 0;

	if(x <= pLutX[0]) return pLutY[0];

	for(n = 1; n < length; n++) 
	{
		if(x <= pLutX[n]) 
		{
			return (pLutY[n-1] + (pLutY[n] - pLutY[n - 1]) * (x - pLutX[n - 1]) / DIV_0_TO_1(pLutX[n] - pLutX[n - 1]));
		}
	}

	return pLutY[length - 1];
}
#define  EPS (0.000001f)
#define  COL_ISO      0
#define  COL_K        1
static HI_FLOAT getKfromNoiseLut(HI_FLOAT (*pRecord)[2], HI_U16 recordNum, HI_S32 iso)
{
    HI_S32 i = 0;
    HI_FLOAT  y_diff, x_diff;
    HI_FLOAT k = 0.0f;
	
    // record: iso - k
    if(iso <= pRecord[0][COL_ISO])
	{
		k = pRecord[0][COL_K];  
	}
	
	if(iso >= pRecord[recordNum - 1][COL_ISO])
	{
		k = pRecord[recordNum - 1][COL_K];
	}
	
	for(i = 0; i < recordNum - 1; i++)
	{
		if(iso >= pRecord[i][COL_ISO] && iso <= pRecord[i + 1][COL_ISO])
		{
			x_diff = pRecord[i + 1][COL_ISO] - pRecord[i][COL_ISO];  // iso diff
			y_diff = pRecord[i + 1][COL_K]  - pRecord[i][COL_K];     // k diff
            k	  = pRecord[i][COL_K] + y_diff * (iso - pRecord[i][COL_ISO]) / DIV_0_TO_1(x_diff + EPS);
		
		}
	}
	return k;
}

static HI_VOID hiisp_wdr_func(ISP_DEV IspDev, HI_U32 u32Iso, ISP_FS_WDR_S* pstFsWdr, ISP_WDR_REG_CFG_S* pstWDRReg)
{
	HI_S32 s32MDTNoise, s32MDTClip, s32MDTLowTh, s32MDTHigTh, s32MDTMaxTh;	
	HI_S32 s32CurISO =  (HI_S32)u32Iso;	
	

	ISP_CMOS_BLACK_LEVEL_S  *pstSnsBlackLevel = HI_NULL;	

	ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
	ISP_SensorGetDefault(IspDev, &pstSnsDft);
	
    	ISP_SensorGetBlc(IspDev, &pstSnsBlackLevel); 

	pstWDRReg->au16InBLC[0] = (pstSnsBlackLevel->au16BlackLevel[0] << 2);
	pstWDRReg->au16InBLC[1] = (pstSnsBlackLevel->au16BlackLevel[1] << 2);
	pstWDRReg->u32OutBLC = (((HI_U32)pstSnsBlackLevel->au16BlackLevel[0]) << 2);

	if (hi_ext_system_wdr_manual_mode_read())
	{
		pstWDRReg->au16InBLC[0] 	= hi_ext_system_wdr_blc0_read();
		pstWDRReg->au16InBLC[1] 	= hi_ext_system_wdr_blc1_read();	
		pstWDRReg->u32OutBLC	 	= hi_ext_system_wdr_out_blc_read();		
		pstWDRReg->u16NosClipTH	= hi_ext_system_manual_wdr_lutmdtclip_read ();                                
	       pstWDRReg->u8NosFactorLow	= hi_ext_system_manual_wdr_lutmdtlowth_read();                               
	       pstWDRReg->u8NosFactorHig	= hi_ext_system_manual_wdr_lutmdthigth_read();
	       pstWDRReg->u8BldrMdtMax	= hi_ext_system_manual_wdr_mdtmaxth_read();
	}
	else
	{
		s32MDTClip	= GetValueFromLut(s32CurISO, g_as32WdrIsoLut, &pstFsWdr->as32LutMDTClips[0], ISP_AUTO_ISO_STRENGTH_NUM);          // UI control
		s32MDTLowTh = GetValueFromLut(s32CurISO, g_as32WdrIsoLut, &pstFsWdr->as32LutMDTLowTh[0], ISP_AUTO_ISO_STRENGTH_NUM);          // UI control
		s32MDTHigTh 	= GetValueFromLut(s32CurISO, g_as32WdrIsoLut, &pstFsWdr->as32LutMDTHigTh[0], ISP_AUTO_ISO_STRENGTH_NUM);          // UI control
		s32MDTMaxTh	= GetValueFromLut(s32CurISO, g_as32WdrIsoLut, &pstFsWdr->as32LutMDTMaxTH[0], ISP_AUTO_ISO_STRENGTH_NUM);          // UI control
	
		pstWDRReg->u16NosClipTH   	=  s32MDTClip;                                
		pstWDRReg->u8NosFactorLow	=  s32MDTLowTh;                                 
		pstWDRReg->u8NosFactorHig	=  s32MDTHigTh;  
		pstWDRReg->u8BldrMdtMax	=  s32MDTMaxTh; 
	}

	s32MDTNoise = GetValueFromLut(s32CurISO, g_as32WdrIsoLut, &pstFsWdr->as32LutMDTNoise[0], ISP_AUTO_ISO_STRENGTH_NUM);          // UI control	
	
	pstWDRReg->bAlgProcMDT		=	pstFsWdr->bMotionComp;
	pstWDRReg->bFlBmdtMnu		=	pstFsWdr->bMotionComMode;
	pstWDRReg->bMDRefNoise		=	pstFsWdr->bMDRefNoise;
	pstWDRReg->u16LongThr		=	pstFsWdr->u16LongThr;
	pstWDRReg->u16ShortThr		=	pstFsWdr->u16ShortThr;
	pstWDRReg->u16MDSfNrThr	=	pstFsWdr->u16MDSfNrThr;
	pstWDRReg->bUpdateNosLut 	= 	HI_FALSE;
	
	if(pstWDRReg->bAlgProcMDT)                           
	{		
		static HI_DOUBLE k ;
		k = getKfromNoiseLut(pstSnsDft->stBayerNr.afCalibrationCoef, pstSnsDft->stBayerNr.u16CalibrationLutNum, s32CurISO);
		HI_S32 n = 0, bitScale = ISP_BITFIX(pstFsWdr->u8BitDepth - 8);
		HI_FLOAT sigma ;		

		if ((u32Iso != pstFsWdr->u32PreIso)|| (s32MDTNoise != pstFsWdr->s32PreMDTNoise))
		{
			pstFsWdr->u32PreIso 			= u32Iso;
			pstFsWdr->s32PreMDTNoise 	= s32MDTNoise;
			pstWDRReg->bUpdateNosLut	= HI_TRUE;
		}

		if (pstWDRReg->bUpdateNosLut)
		{
			for(n = 0; n < NLUT_LENGTH; n++)                         
			{
				sigma  = (HI_FLOAT)(k  * n * 256 / (NLUT_LENGTH - 1));
				sigma  = sqrtf(sigma);
				sigma *= bitScale;				
				pstWDRReg->au16NosMDTLut[n] = CLIP3((HI_S32)sigma, 0, ISP_BITMASK(14));
			}
		}
	}
}


HI_S32 ISP_FrameWDRRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
	ISP_CTX_S    		*pstIspCtx	= HI_NULL;
	ISP_FS_WDR_S 	*pstFSWDR	= HI_NULL;
	ISP_REG_CFG_S 	*pstRegCfg	= (ISP_REG_CFG_S *)pRegCfg;
	static HI_U8 u8ch0=0,u8ch1=1;
	static HI_U8 u8switchflag=0;
	
    	ISP_GET_CTX(IspDev, pstIspCtx);
    	FS_WDR_GET_CTX(IspDev, pstFSWDR);

    	if (pstIspCtx->stLinkage.bDefectPixel)
    	{
       	return HI_SUCCESS;
    	}
    
    	FrameWDRReadExtRegs(IspDev);
	
      	if(0 == hi_isp_wdr_enable_read(IspDev))
      	{
      		pstRegCfg->unKey.bit1FsWdrCfg = 0;
            	return HI_SUCCESS;
      	}
		

	if(0  == hi_ext_system_chnswitch_read())
	{
		u8ch0 = hi_isp_top_chn_switch0_read(IspDev);
		u8ch1 = hi_isp_top_chn_switch1_read(IspDev);

		hi_isp_top_chn_switch0_write(IspDev,u8ch0);
		hi_isp_top_chn_switch1_write(IspDev,u8ch1);
		if(1 == u8switchflag)
		{
			hi_isp_top_chn_switch0_write(IspDev,u8ch1);
			hi_isp_top_chn_switch1_write(IspDev,u8ch0);
			u8switchflag = 0;
		}
	 
	}
	else
	{
		hi_isp_top_chn_switch0_write(IspDev,u8ch1);
		hi_isp_top_chn_switch1_write(IspDev,u8ch0);
		u8switchflag = 1;
	
	}
	

	hiisp_wdr_func(IspDev, pstIspCtx->stLinkage.u32Iso, pstFSWDR, &pstRegCfg->stWdrRegCfg);

	//pstRegCfg->stWdrRegCfg.bFlBmdtMnu = pstIspCtx->stLinkage.bFlicker;
	pstRegCfg->unKey.bit1FsWdrCfg = 1;

    	return HI_SUCCESS;
}

static __inline HI_S32 FrameWDRImageResWrite(ISP_DEV IspDev, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstRes)
{
	// update Wdr width and height
	hi_isp_wdr_width_write (IspDev, pstRes->u16Width - 1);
	hi_isp_wdr_height_write(IspDev, pstRes->u16Height - 1);

	return HI_SUCCESS;
}

HI_S32 FrameWDRProcWrite(ISP_DEV IspDev, ISP_CTRL_PROC_WRITE_S *pstProc)
{
	ISP_CTRL_PROC_WRITE_S stProcTmp;
   	ISP_FS_WDR_S 	*pstFSWDR	= HI_NULL;

    	FS_WDR_GET_CTX(IspDev, pstFSWDR);

	if ((HI_NULL == pstProc->pcProcBuff)
            || (0 == pstProc->u32BuffLen))
      	{
      		return HI_FAILURE;
    	}

    	stProcTmp.pcProcBuff = pstProc->pcProcBuff;
   	stProcTmp.u32BuffLen = pstProc->u32BuffLen;
    
    	ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "-----FrameWDR INFO------------------------------------------------------------------\n");
           
    	ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%16s"		 "%16s"		"%16s"    "%16s"	"%16s\n",
        "CompEn", "CompMode", "LongThr", "ShortThr","NosRefEn");

    	ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%16d"  "%16d"     "%16d" 	"%16d"   "%16d\n",
        pstFSWDR->bMotionComp,
        pstFSWDR->bMotionComMode,
        pstFSWDR->u16LongThr,
        pstFSWDR->u16ShortThr,
        pstFSWDR->bMDRefNoise);

    	pstProc->u32WriteLen += 1;
    
    return HI_SUCCESS;
}

HI_S32 ISP_FrameWDRCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{ 
    
    switch (u32Cmd)
    {
    	case ISP_WDR_MODE_SET:			
			ISP_FrameWDRInit(IspDev);
            	break;
		case ISP_CHANGE_IMAGE_MODE_SET:
				FrameWDRImageResWrite(IspDev, (ISP_CMOS_SENSOR_IMAGE_MODE_S *)pValue);
			break;
		case ISP_PROC_WRITE:
	    		FrameWDRProcWrite(IspDev, (ISP_CTRL_PROC_WRITE_S *)pValue);
	        	break;
        default :
        	break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_FrameWDRExit(ISP_DEV IspDev)
{
	hi_isp_bcom_enable_write(IspDev, HI_FALSE);
	hi_isp_bdec_enable_write(IspDev, HI_FALSE);
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterFrameWDR(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_FrameWDR;
    pstAlgs->stAlgFunc.pfn_alg_init	= ISP_FrameWDRInit;
    pstAlgs->stAlgFunc.pfn_alg_run  	= ISP_FrameWDRRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl 	= ISP_FrameWDRCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit 	= ISP_FrameWDRExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

