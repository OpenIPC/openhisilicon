/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_cac.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/11/09
  Description   : 
  History       :
  1.Date        : 2015/11/09
    Author      : m00294153
    Modification: Created file

******************************************************************************/

#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_proc.h"
#include "hi_math.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

// Lobal CAC,default value
#define ISP_LCAC_LUMA_THR_R_WDR (1500)
#define ISP_LCAC_LUMA_THR_G_WDR (1500)
#define ISP_LCAC_LUMA_THR_B_WDR (2150)

#define ISP_LCAC_LUMA_THR_R_LINEAR (1500)
#define ISP_LCAC_LUMA_THR_G_LINEAR (1500)
#define ISP_LCAC_LUMA_THR_B_LINEAR (3500)

#define ISP_LCAC_DE_PURPLE_CB_STRENGTH_LINEAR (3) /*[0,8]*/
#define ISP_LCAC_DE_PURPLE_CR_STRENGTH_LINEAR (0) /*[0,8]*/

#define ISP_LCAC_DE_PURPLE_CB_STRENGTH_WDR (7) /*[0,8]*/
#define ISP_LCAC_DE_PURPLE_CR_STRENGTH_WDR (0) /*[0,8]*/


#define ISP_LCAC_CRCB_RATIO_HIGH_LIMIT_LINEAR (292)
#define ISP_LCAC_CRCB_RATIO_LOW_LIMIT_LINEAR  (-50)

#define ISP_LCAC_CRCB_RATIO_HIGH_LIMIT_WDR (292)
#define ISP_LCAC_CRCB_RATIO_LOW_LIMIT_WDR  (-50)

#define ISP_LCAC_PURPLE_DET_RANGE_DEFAULT_LINEAR (60)
#define ISP_LCAC_PURPLE_DET_RANGE_DEFAULT_WDR    (195)


static const HI_U32 g_au32ExpRatioLut[ISP_AUTO_ISO_STRENGTH_NUM] = {64,128,192,256,320,384,448,512,576,640,704,768,832,896,960,1023};   

static const HI_U8  g_au8WdrCbStrDefaultLut[ISP_AUTO_ISO_STRENGTH_NUM] = {0,0,4,4,5,5,5,5,5,5,5,5,5,5,5,5};   
static const HI_U8  g_au8WdrCrStrDefaultLut[ISP_AUTO_ISO_STRENGTH_NUM] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};   
static const HI_U8  g_au8LinearCbStrDefaultLut[ISP_AUTO_ISO_STRENGTH_NUM] = {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3};   
static const HI_U8  g_au8LinearCrStrDefaultLut[ISP_AUTO_ISO_STRENGTH_NUM] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 


static const HI_U16 g_au16RLuma[3] = {1500,1500,0};
static const HI_U16 g_au16GLuma[3] = {1500,1500,0};
static const HI_U16 g_au16BLuma[3] = {4100,1500,0};

static const HI_U16 g_au16PurpleDetRange[3] = {0,260,410};
#define RANGE_MAX_VALUE (3)

typedef struct hiISP_CAC_S
{
	HI_BOOL bLocalCacEn;
	HI_BOOL bCacManualEn;
	HI_U8 au8DePurpleCbStr[ISP_AUTO_ISO_STRENGTH_NUM];
	HI_U8 au8DePurpleCrStr[ISP_AUTO_ISO_STRENGTH_NUM];
	HI_U8 u8DePurpleCbStr;
	HI_U8 u8DePurpleCrStr;
	HI_U16 u16PurpleDetRange;
	HI_U16 u16PurpleVarThr;
}ISP_CAC_S;


ISP_CAC_S g_astCacCtx[ISP_MAX_DEV_NUM] = {{0}};

#define CAC_GET_CTX(dev, pstCtx)   pstCtx = &g_astCacCtx[dev]

static HI_VOID CacExtRegsInitialize(ISP_DEV IspDev)
{	
	HI_U8 i;
	HI_U8 u8WDRMode;
	ISP_CTX_S *pstIspCtx;
	ISP_GET_CTX(IspDev, pstIspCtx);

    u8WDRMode = pstIspCtx->u8SnsWDRMode;
	hi_ext_system_localCAC_manual_enable_write(OP_TYPE_AUTO);
	hi_ext_system_localCAC_enable_write(HI_TRUE);
	hi_ext_system_localCAC_purple_var_thr_write(HI_ISP_LCAC_PURPLE_DET_THR_DEFAULT);

	if(IS_LINEAR_MODE(u8WDRMode))//linear mode
	{
		for(i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)//auto
		{
			hi_ext_system_localCAC_auto_cb_str_table_write(i,g_au8LinearCbStrDefaultLut[i]);
			hi_ext_system_localCAC_auto_cr_str_table_write(i,g_au8LinearCrStrDefaultLut[i]);
		}
		hi_ext_system_localCAC_manual_cb_str_write(ISP_LCAC_DE_PURPLE_CB_STRENGTH_LINEAR);
		hi_ext_system_localCAC_manual_cr_str_write(ISP_LCAC_DE_PURPLE_CR_STRENGTH_LINEAR);
		hi_ext_system_localCAC_purple_det_range_write(ISP_LCAC_PURPLE_DET_RANGE_DEFAULT_LINEAR);
	}
	else //WDR mode
	{
		for(i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)//auto
		{
			hi_ext_system_localCAC_auto_cb_str_table_write(i,g_au8WdrCbStrDefaultLut[i]);
			hi_ext_system_localCAC_auto_cr_str_table_write(i,g_au8WdrCrStrDefaultLut[i]);
		}
		hi_ext_system_localCAC_manual_cb_str_write(ISP_LCAC_DE_PURPLE_CB_STRENGTH_WDR);
		hi_ext_system_localCAC_manual_cr_str_write(ISP_LCAC_DE_PURPLE_CR_STRENGTH_WDR);	
		hi_ext_system_localCAC_purple_det_range_write(ISP_LCAC_PURPLE_DET_RANGE_DEFAULT_WDR);
	}
	
	return;
}

static HI_VOID CacRegsInitialize(ISP_DEV IspDev)
{
	HI_U8 u8WDRMode;
	ISP_CTX_S *pstIspCtx;
	HI_U16 u16Width = 0, u16Height = 0;
	ISP_GET_CTX(IspDev, pstIspCtx);

    u8WDRMode = pstIspCtx->u8SnsWDRMode;
	
	u16Width = hi_ext_sync_total_width_read();
	u16Height = hi_ext_sync_total_height_read();
	//global cac
	hi_isp_gcac_enable_write(IspDev, HI_FALSE);
	hi_isp_gcac_height_write(IspDev, u16Height-1);
	hi_isp_gcac_width_write(IspDev, u16Width-1);
	hi_isp_gcac_cnt_start_ver_write(IspDev, (u16Height-1)>>1);
	hi_isp_gcac_cnt_start_hor_write(IspDev, (u16Width-1)>>1);	
	hi_isp_gcac_param_red_A_write(IspDev, HI_ISP_GCAC_PARAM_RED_A_DEFAULT);
	hi_isp_gcac_param_red_B_write(IspDev, HI_ISP_GCAC_PARAM_RED_B_DEFAULT);
	hi_isp_gcac_param_red_C_write(IspDev, HI_ISP_GCAC_PARAM_RED_C_DEFAULT);

	hi_isp_gcac_param_blue_A_write(IspDev, HI_ISP_GCAC_PARAM_BLUE_A_DEFAULT);
	hi_isp_gcac_param_blue_B_write(IspDev, HI_ISP_GCAC_PARAM_BLUE_B_DEFAULT);
	hi_isp_gcac_param_blue_C_write(IspDev, HI_ISP_GCAC_PARAM_BLUE_C_DEFAULT);

	hi_isp_gcac_y_ns_norm_write(IspDev, HI_ISP_GCAC_Y_NORM_SHIFT_DEFAULT);
	hi_isp_gcac_y_nf_norm_write(IspDev, HI_ISP_GCAC_Y_NORM_FACTOR_DEFAULT);
	hi_isp_gcac_x_ns_norm_write(IspDev, HI_ISP_GCAC_X_NORM_SHIFT_DEFAULT);
	hi_isp_gcac_x_nf_norm_write(IspDev, HI_ISP_GCAC_X_NORM_FACTOR_DEFAULT);

	hi_isp_gcac_cor_start_ver_write(IspDev, HI_ISP_GCAC_COORDINATE_START_Y_DEFAULT);
	hi_isp_gcac_cor_start_hor_write(IspDev, HI_ISP_GCAC_COORDINATE_START_X_DEFAULT);

	hi_isp_gcac_ver_filt_en_write(IspDev, HI_ISP_GCAC_VER_FILT_EN_DEFAULT);
	hi_isp_gcac_chr_ver_mode_write(IspDev, HI_ISP_GCAC_CHR_VER_MODE_DEFAULT);
	hi_isp_gcac_ver_clip_mode_write(IspDev, HI_ISP_GCAC_CLIP_MODE_VER_DEFAULT);
	hi_isp_gcac_hor_clip_mode_write(IspDev, HI_ISP_GCAC_CLIP_MODE_HOR_DEFAULT);
	hi_isp_demosaic_gcac_blend_select_enable_write(IspDev, HI_ISP_GCAC_BLEND_SELECTION_DEFAULT);
	hi_isp_demosaic_varthrforblend_write(IspDev, HI_ISP_DEMOSAIC_GCAC_BLEND_THR_DEFAULT);

	//local cac
	hi_isp_demosaic_local_cac_enable_write(IspDev, HI_TRUE);
	if (IS_LINEAR_MODE(u8WDRMode))
	{
		hi_isp_demosaic_r_luma_thr_write(IspDev, ISP_LCAC_LUMA_THR_R_LINEAR);
		hi_isp_demosaic_g_luma_thr_write(IspDev, ISP_LCAC_LUMA_THR_G_LINEAR);
		hi_isp_demosaic_b_luma_thr_write(IspDev, ISP_LCAC_LUMA_THR_B_LINEAR);
		hi_isp_demosaic_depurplectrcr_write(IspDev, 8 - ISP_LCAC_DE_PURPLE_CR_STRENGTH_LINEAR);
		hi_isp_demosaic_depurplectrcb_write(IspDev, 8 - ISP_LCAC_DE_PURPLE_CB_STRENGTH_LINEAR);
		hi_isp_demosaic_cbcr_ratio_lmt_high_write(IspDev, ISP_LCAC_CRCB_RATIO_HIGH_LIMIT_LINEAR);
		hi_isp_demosaic_cbcr_ratio_lmt_low_write(IspDev, ISP_LCAC_CRCB_RATIO_LOW_LIMIT_LINEAR);
	}
	else
	{
		hi_isp_demosaic_r_luma_thr_write(IspDev, ISP_LCAC_LUMA_THR_R_WDR);
		hi_isp_demosaic_g_luma_thr_write(IspDev, ISP_LCAC_LUMA_THR_G_WDR);
		hi_isp_demosaic_b_luma_thr_write(IspDev, ISP_LCAC_LUMA_THR_B_WDR);
		hi_isp_demosaic_depurplectrcr_write(IspDev, 8 - ISP_LCAC_DE_PURPLE_CR_STRENGTH_WDR);
		hi_isp_demosaic_depurplectrcb_write(IspDev, 8 - ISP_LCAC_DE_PURPLE_CB_STRENGTH_WDR);
		hi_isp_demosaic_cbcr_ratio_lmt_high_write(IspDev, ISP_LCAC_CRCB_RATIO_HIGH_LIMIT_WDR);
		hi_isp_demosaic_cbcr_ratio_lmt_low_write(IspDev, ISP_LCAC_CRCB_RATIO_LOW_LIMIT_WDR);
	}

	hi_isp_demosaic_r_counter_thr_write(IspDev, HI_ISP_LCAC_COUNT_THR_R_DEFAULT);
	hi_isp_demosaic_g_counter_thr_write(IspDev, HI_ISP_LCAC_COUNT_THR_G_DEFAULT);
	hi_isp_demosaic_b_counter_thr_write(IspDev, HI_ISP_LCAC_COUNT_THR_B_DEFAULT);

	hi_isp_demosaic_purple_var_thr_write(IspDev, HI_ISP_LCAC_PURPLE_DET_THR_DEFAULT);
	hi_isp_demosaic_satu_thr_write(IspDev, HI_ISP_LCAC_SATU_THR_DEFAULT);

    return;
}

HI_S32 Linear_Interpol(HI_S32 xm,HI_S32 x0,HI_S32 y0,HI_S32 x1,HI_S32 y1)
{
	HI_S32 ym;

	if ( xm <= x0 ) return y0;
	if ( xm >= x1 ) return y1;

	ym = (y1 - y0) * (xm - x0) / (x1 - x0) + y0;
	return ym;
}

static HI_S32 CacReadExtregs(ISP_DEV IspDev)
{   		
	HI_U8 i;
	ISP_CAC_S *pstCAC=HI_NULL;
	
	CAC_GET_CTX(IspDev,pstCAC);
	for(i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
	{
		pstCAC->au8DePurpleCbStr[i] = hi_ext_system_localCAC_auto_cb_str_table_read(i);
		pstCAC->au8DePurpleCrStr[i] = hi_ext_system_localCAC_auto_cr_str_table_read(i);
	}
	pstCAC->bLocalCacEn			= hi_ext_system_localCAC_enable_read();
	pstCAC->bCacManualEn 		= hi_ext_system_localCAC_manual_enable_read();
	pstCAC->u8DePurpleCbStr 	= hi_ext_system_localCAC_manual_cb_str_read();
	pstCAC->u8DePurpleCrStr 	= hi_ext_system_localCAC_manual_cr_str_read();
	pstCAC->u16PurpleDetRange 	= hi_ext_system_localCAC_purple_det_range_read();
	pstCAC->u16PurpleVarThr		= hi_ext_system_localCAC_purple_var_thr_read();
	return 0;
}

HI_S32 cac_fw(ISP_CAC_S *pstCAC,ISP_CAC_REG_CFG_S *pstCacHwCfg,ISP_DEV IspDev)
{
	HI_S32 s32ExpRatioIndexUpper,s32ExpRatioIndexLower;
	HI_S32 DePurpleCtrCb; // [0, 8]
	HI_S32 DePurpleCtrCr; // [0, 8]
	HI_U32 u32ExpRatio;
	HI_U32 i;
	HI_U16 u16RLumaThr,u16GLumaThr,u16BLumaThr;

	HI_S32 s32RangeIdxUp,s32RangeIdxLow;
	HI_S32 s32RangeUpper,s32RangeLower;

	s32RangeIdxUp = RANGE_MAX_VALUE- 1;
	for (i = 0; i < RANGE_MAX_VALUE;i++)
	{
		if (pstCAC->u16PurpleDetRange < g_au16PurpleDetRange[i])
		{
			s32RangeIdxUp = i;
			break;
		}
	}
	s32RangeIdxLow = MAX2(s32RangeIdxUp - 1, 0);

	s32RangeUpper = g_au16PurpleDetRange[s32RangeIdxUp];
	s32RangeLower = g_au16PurpleDetRange[s32RangeIdxLow];


	u16RLumaThr = (HI_U16)Linear_Interpol(pstCAC->u16PurpleDetRange,	
								  		  s32RangeLower,g_au16RLuma[s32RangeIdxLow],
								  		  s32RangeUpper,g_au16RLuma[s32RangeIdxUp]);
	u16GLumaThr = (HI_U16)Linear_Interpol(pstCAC->u16PurpleDetRange,	
								  		  s32RangeLower,g_au16GLuma[s32RangeIdxLow],
								  		  s32RangeUpper,g_au16GLuma[s32RangeIdxUp]);
	u16BLumaThr = (HI_U16)Linear_Interpol(pstCAC->u16PurpleDetRange,	
								  		  s32RangeLower,g_au16BLuma[s32RangeIdxLow],
								  		  s32RangeUpper,g_au16BLuma[s32RangeIdxUp]);
	u16BLumaThr = CLIP_MAX(u16BLumaThr,4095);

	u32ExpRatio = hi_isp_wdr_exporatio0_read(IspDev);
	if(pstCAC->bCacManualEn)
	{
		DePurpleCtrCb = pstCAC->u8DePurpleCbStr;
		DePurpleCtrCr = pstCAC->u8DePurpleCrStr;
	}
	else
	{
		s32ExpRatioIndexUpper = ISP_AUTO_ISO_STRENGTH_NUM-1;
		for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
		{
			if (u32ExpRatio <= g_au32ExpRatioLut[i])
			{
				s32ExpRatioIndexUpper = i;
				break;
			}
		}
		
		s32ExpRatioIndexLower = MAX2(s32ExpRatioIndexUpper - 1, 0);

		DePurpleCtrCb = Linear_Interpol(u32ExpRatio,	
										g_au32ExpRatioLut[s32ExpRatioIndexLower],pstCAC->au8DePurpleCbStr[s32ExpRatioIndexLower],
										g_au32ExpRatioLut[s32ExpRatioIndexUpper],pstCAC->au8DePurpleCbStr[s32ExpRatioIndexUpper]);
		DePurpleCtrCr = Linear_Interpol(u32ExpRatio,	
										g_au32ExpRatioLut[s32ExpRatioIndexLower],pstCAC->au8DePurpleCrStr[s32ExpRatioIndexLower],
										g_au32ExpRatioLut[s32ExpRatioIndexUpper],pstCAC->au8DePurpleCrStr[s32ExpRatioIndexUpper]);
	}


	pstCacHwCfg->bLocalCacEn		= pstCAC->bLocalCacEn;
	pstCacHwCfg->u16RLumaThr		= u16RLumaThr;
	pstCacHwCfg->u16GLumaThr		= u16GLumaThr;
	pstCacHwCfg->u16BLumaThr		= u16BLumaThr;
	pstCacHwCfg->u8DePurpleCtrCr	= 8 - DePurpleCtrCr;
	pstCacHwCfg->u8DePurpleCtrCb	= 8 - DePurpleCtrCb;
	pstCacHwCfg->u16VarThr			= pstCAC->u16PurpleVarThr;
	return 0;
}


HI_S32 ISP_CacInit(ISP_DEV IspDev)
{
    CacRegsInitialize(IspDev);
    CacExtRegsInitialize(IspDev);
    
    return HI_SUCCESS;
}


HI_S32 ISP_CacRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
	ISP_CAC_S *pstCAC=HI_NULL;
	ISP_REG_CFG_S       *pstRegCfg  = (ISP_REG_CFG_S *)pRegCfg;
    ISP_CTX_S *pstIspCtx = HI_NULL;
	
	CAC_GET_CTX(IspDev,pstCAC);
	ISP_GET_CTX(IspDev, pstIspCtx);

	if (pstIspCtx->stLinkage.bDefectPixel)
    {
        return HI_SUCCESS;
    }
	
	if(0 == hi_ext_system_localCAC_enable_read())
	{
		hi_isp_demosaic_local_cac_enable_write(IspDev,HI_FALSE);
		return HI_SUCCESS;
	}
	
	CacReadExtregs(IspDev);

	cac_fw(pstCAC,&pstRegCfg->stCacRegCfg,IspDev);

	pstRegCfg->unKey.bit1CacCfg	= HI_TRUE;
	
    return HI_SUCCESS;
}

static __inline HI_S32 CACImageResWrite(ISP_DEV IspDev, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstRes)
{
	hi_isp_gcac_height_write(IspDev, pstRes->u16Height-1);
	hi_isp_gcac_width_write(IspDev,pstRes->u16Width-1 );

    return HI_SUCCESS;
}


HI_S32 ISP_CacCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{	
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :
			ISP_CacInit(IspDev);
            break;
        case ISP_PROC_WRITE:
			break;
        case ISP_CHANGE_IMAGE_MODE_SET:
			CACImageResWrite(IspDev,(ISP_CMOS_SENSOR_IMAGE_MODE_S *)pValue);
            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_CacExit(ISP_DEV IspDev)
{
	hi_isp_gcac_enable_write(IspDev, HI_FALSE);
	hi_isp_demosaic_local_cac_enable_write(IspDev, HI_FALSE);
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterCac(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_CAC;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_CacInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_CacRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_CacCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_CacExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


