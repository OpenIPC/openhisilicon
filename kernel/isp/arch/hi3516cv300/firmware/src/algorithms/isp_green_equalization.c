/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_green_equalization.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/04/24
  Description   : 
  History       :
  1.Date        : 2013/04/24
    Author      : n00168968
    Modification: Created file

******************************************************************************/
#include <math.h>
#include "isp_alg.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_sensor.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define HI_ISP_GE_MAX_BLK_NUM   (1024)
#define HI_ISP_GE_MAX_BLK_NUM_X (32)
#define HI_ISP_GE_MAX_BLK_NUM_Y (32)

#define HI_MINIISP_BITDEPTH 14


static HI_U32 g_au32GeIsoLut[ISP_AUTO_ISO_STRENGTH_NUM] = {100,200,400,800,1600,3200,6400,12800,25600,51200,102400,204800,409600,819200,1638400,3276800};

typedef struct hiISP_GREEN_EQUALIZATION_S
{

    HI_BOOL bEnable;

	HI_S32 grgb_w;
    HI_S32 grgb_h;
	HI_U32 bitDepth;
	HI_U32 gDiffBuff_w;
	HI_U32 gDiffBuff_h;
	HI_U32 *pGdiffBuf;
	HI_U16 pGrAvg[HI_ISP_GE_MAX_BLK_NUM];
    HI_U16 pGbAvg[HI_ISP_GE_MAX_BLK_NUM];

    HI_U16 au16NpOffset[ISP_AUTO_ISO_STRENGTH_NUM];	
    HI_U16 au16Strength[ISP_AUTO_ISO_STRENGTH_NUM];     
    HI_U16 au16Threshold[ISP_AUTO_ISO_STRENGTH_NUM];                          
    HI_U8  u8Slope;                              
    HI_U8  u8SensiSlope;                         
    HI_U16 u16SensiThr;     /*edge detail*/	
} ISP_GREEN_EQUALIZATION_S;

ISP_GREEN_EQUALIZATION_S g_astGeCtx[ISP_MAX_DEV_NUM] = {{0}};
#define GE_GET_CTX(dev, pstCtx)   pstCtx = &g_astGeCtx[dev]

static HI_VOID GeExtRegsDefault(ISP_DEV IspDev)
{
    return;
}

static HI_VOID GeRegsDefault(ISP_DEV IspDev) 
{
	hi_isp_ge_enable_write(IspDev, 0, HI_ISP_GE_ENABLE_DEFAULT);
	hi_isp_ge_enable_write(IspDev, 1, HI_ISP_GE_ENABLE_DEFAULT);
	hi_isp_ge_mode_write(IspDev, HI_ISP_GE_MODE_DEFAULT); 
	hi_isp_ge_aver_enable_write(IspDev, HI_TRUE);
	hi_isp_ge_correct_enable_write(IspDev, HI_TRUE);
		
    return;
}

static HI_VOID GeExtRegsInitialize(ISP_DEV IspDev)
{	
    HI_U32 i;

    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
	
    ISP_SensorGetDefault(IspDev, &pstSnsDft);			

    /* initial ext register of Ge */ 

	hi_ext_system_ge_enable_write(pstSnsDft->stGe.bEnable);

    hi_ext_system_ge_slope_write   (pstSnsDft->stGe.u8Slope);
    hi_ext_system_ge_sensitivity_write   (pstSnsDft->stGe.u8SensiSlope);
    hi_ext_system_ge_sensithreshold_write(pstSnsDft->stGe.u16SensiThr);
    
    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
    {
        hi_ext_system_ge_strength_write(i, pstSnsDft->stGe.au16Strength[i]);	
        hi_ext_system_ge_npoffset_write(i, pstSnsDft->stGe.au16NpOffset[i]);
        hi_ext_system_ge_threshold_write(i, pstSnsDft->stGe.au16Threshold[i]);                
    }

    return;
}

static HI_VOID GeRegsInitialize(ISP_DEV IspDev)
{    
	ISP_GREEN_EQUALIZATION_S *pstGe = HI_NULL;
	HI_U32 u32Width = 0, u32Height = 0;	   	  
	
	GE_GET_CTX(IspDev, pstGe);
    	
	u32Width = hi_ext_sync_total_width_read();
	u32Height = hi_ext_sync_total_height_read();
    
	pstGe->grgb_w     = HI_ISP_GE_MAX_BLK_NUM_X;
	pstGe->grgb_h     = HI_ISP_GE_MAX_BLK_NUM_Y;

	
	if (pstGe->grgb_w*pstGe->grgb_h <= 1920*1080)
	{
		pstGe->grgb_w = 17;
		pstGe->grgb_h = 15;
	}
	
	pstGe->bitDepth   = HI_MINIISP_BITDEPTH;
	pstGe->gDiffBuff_w= pstGe->grgb_w + 2;
	pstGe->gDiffBuff_h= pstGe->grgb_h + 2;
	
	memset(&pstGe->pGbAvg, 0, HI_ISP_GE_MAX_BLK_NUM*sizeof(HI_U16));
	memset(&pstGe->pGrAvg, 0, HI_ISP_GE_MAX_BLK_NUM*sizeof(HI_U16));

	pstGe->pGdiffBuf = (HI_U32 *)malloc(pstGe->gDiffBuff_w*pstGe->gDiffBuff_h*sizeof(HI_U32));
	if(NULL == pstGe->pGdiffBuf)
	{
		printf("hi_isp_ge_info:malloc mem for ge failure!\n");
		return;
	}
	
	memset(pstGe->pGdiffBuf,0,pstGe->gDiffBuff_w*pstGe->gDiffBuff_h*sizeof(HI_U32));

	hi_isp_ge_hnum_write(IspDev, pstGe->grgb_w);
	hi_isp_ge_vnum_write(IspDev, pstGe->grgb_h);
	hi_isp_ge_width_write(IspDev, u32Width-1);
	hi_isp_ge_height_write(IspDev, u32Height-1);	
	
	hi_isp_ge_strength_write(IspDev, HI_ISP_GE_STRENGTH_DEFAULT); 		
        hi_isp_ge_ct_th1_write(IspDev, 0, HI_ISP_GE_NPOFFSET_DEFAULT);
	hi_isp_ge_ct_th1_write(IspDev, 1, HI_ISP_GE_NPOFFSET_DEFAULT);
	hi_isp_ge_ct_th2_write(IspDev, 0, HI_ISP_GE_SENSITHR_DEFAULT);
	hi_isp_ge_ct_th2_write(IspDev, 1, HI_ISP_GE_SENSITHR_DEFAULT);
	hi_isp_ge_ct_th3_write(IspDev, 0, HI_ISP_GE_THRESHOLD_DEFAULT);
	hi_isp_ge_ct_th3_write(IspDev, 1, HI_ISP_GE_THRESHOLD_DEFAULT);
	hi_isp_ge_ct_slope_write(IspDev, 0,HI_ISP_GE_SENSISLOPE_DEFAULT, HI_ISP_GE_SLOPE_DEFAULT);
	hi_isp_ge_ct_slope_write(IspDev, 1,HI_ISP_GE_SENSISLOPE_DEFAULT, HI_ISP_GE_SLOPE_DEFAULT);
		
    return;
}
#if 0
static HI_VOID GeGetStat(ISP_DEV IspDev)
{
	HI_U32 i, aver_data;
	ISP_GREEN_EQUALIZATION_S *pstGe = HI_NULL;
    GE_GET_CTX(IspDev, pstGe);	
	hi_isp_ge_aver_raddr_write(IspDev, 0, 0);
	for(i = 0; i < HI_ISP_GE_MAX_BLK_NUM; i++)
	{
		aver_data = hi_isp_ge_aver_rdata_read(IspDev, 0);
		pstGe->pGrAvg[i] = (aver_data & 0x0fffffff) >> 14;
		pstGe->pGbAvg[i] = (aver_data & 0x0003fff);
	}
    return;
}
#endif
static HI_U32 _linearinter (HI_U32 v, HI_U32 x0, HI_U32 x1, HI_U32 y0, HI_U32 y1)
{
    if (v <= x0)    return y0;
    if (v >= x1)    return y1;

    if(y1 >= y0)
    {
        return  (y0 + (y1-y0)*(v-x0)/(x1-x0));  // 0 protect
    }
    else
    {
        return  (y1 + (y0-y1)*(x1-v)/(x1-x0));  // 0 protect
    }
}

static HI_U8 GeGetIsoIndex(HI_U32 u32Iso)
{   
	HI_U8 u8Index;
	
    for (u8Index = 0; u8Index < ISP_AUTO_ISO_STRENGTH_NUM-1; u8Index++)
    {
        if (u32Iso <= g_au32GeIsoLut[u8Index])
        {
            break;
        }
    }
	
    return u8Index;
}

static HI_U16 GeGetStrength(HI_U32 u32Iso, ISP_GREEN_EQUALIZATION_S *pstGe)
{
    HI_U8 u8Index = GeGetIsoIndex(u32Iso);
    
    if (0 == u8Index
        || (ISP_AUTO_ISO_STRENGTH_NUM - 1) == u8Index)
    {
        return pstGe->au16Strength[u8Index];
    }

    return _linearinter(u32Iso, g_au32GeIsoLut[u8Index-1], g_au32GeIsoLut[u8Index], pstGe->au16Strength[u8Index-1], pstGe->au16Strength[u8Index]);
}

static HI_U16 GeGetNpOffset(HI_U32 u32Iso, ISP_GREEN_EQUALIZATION_S *pstGe)
{
    HI_U8 u8Index = GeGetIsoIndex(u32Iso);
    
    if (0 == u8Index
        || (ISP_AUTO_ISO_STRENGTH_NUM - 1) == u8Index)
    {
        return pstGe->au16NpOffset[u8Index];
    }

    return _linearinter(u32Iso, g_au32GeIsoLut[u8Index-1], g_au32GeIsoLut[u8Index], pstGe->au16NpOffset[u8Index-1], pstGe->au16NpOffset[u8Index]);
}

static HI_U16 GeGetThreshold(HI_U32 u32Iso, ISP_GREEN_EQUALIZATION_S *pstGe)
{
    HI_U8 u8Index = GeGetIsoIndex(u32Iso);
    
    if (0 == u8Index
        || (ISP_AUTO_ISO_STRENGTH_NUM - 1) == u8Index)
    {
        return pstGe->au16Threshold[u8Index];
    }

    return _linearinter(u32Iso, g_au32GeIsoLut[u8Index-1], g_au32GeIsoLut[u8Index], pstGe->au16Threshold[u8Index-1], pstGe->au16Threshold[u8Index]);
}

static HI_VOID Ge_GetRegCfg(ISP_DEV IspDev, ISP_GE_REG_CFG_S* pstGeReg, HI_U32 iso)
{
	ISP_GREEN_EQUALIZATION_S *pstGe = HI_NULL;
    GE_GET_CTX(IspDev, pstGe);
	
		pstGeReg->detail_th   = CLIP3((HI_S32)pstGe->u16SensiThr ,0,(1<<pstGe->bitDepth));        
		pstGeReg->detail_slop = CLIP3((HI_S32)pstGe->u8SensiSlope,0,    pstGe->bitDepth) ;  
		pstGeReg->ge_th       = CLIP3((HI_S32)GeGetThreshold(iso, pstGe),0,(1<<pstGe->bitDepth));
		pstGeReg->ge_th_slop  = CLIP3((HI_S32)pstGe->u8Slope,0,    pstGe->bitDepth) ;          	
	
	pstGeReg->u16GeStrength = GeGetStrength(iso, pstGe);
	pstGeReg->ct_th1 = GeGetNpOffset(iso, pstGe);
	pstGeReg->ge_enable = pstGe->bEnable;
	
        
	return;
}

static HI_S32 GeReadExtregs(ISP_DEV IspDev)
{
    HI_U32 i;
    ISP_GREEN_EQUALIZATION_S *pstGe = HI_NULL;
	
    GE_GET_CTX(IspDev, pstGe);	
    
    /* initial ext register of Ge */ 
    pstGe->bEnable = hi_ext_system_ge_enable_read();
	  
    pstGe->u8Slope   = hi_ext_system_ge_slope_read();
    pstGe->u8SensiSlope     = hi_ext_system_ge_sensitivity_read();
    pstGe->u16SensiThr = hi_ext_system_ge_sensithreshold_read();
    
    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
    {
        pstGe->au16Strength[i] = hi_ext_system_ge_strength_read (i);
        pstGe->au16NpOffset[i] = hi_ext_system_ge_npoffset_read (i);
        pstGe->au16Threshold[i]= hi_ext_system_ge_threshold_read(i);                	
    }
    
    return 0;
}

HI_S32 ISP_GeInit(ISP_DEV IspDev)
{
    GeRegsDefault(IspDev);
    GeExtRegsDefault(IspDev);
    GeRegsInitialize(IspDev);
    GeExtRegsInitialize(IspDev);
    
    return HI_SUCCESS;
}



HI_S32 ISP_GeRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
	ISP_CTX_S *pstIspCtx = HI_NULL;
	ISP_REG_CFG_S *pstReg = HI_NULL;
	
	ISP_GET_CTX(IspDev, pstIspCtx);

	/* calculate every two interrupts */	
    if (0 != pstIspCtx->u32FrameCnt % 2)
    {
        return HI_SUCCESS;
    }

	pstReg = (ISP_REG_CFG_S *)pRegCfg;

	GeReadExtregs(IspDev);
	//GeGetStat(IspDev);
	Ge_GetRegCfg(IspDev, &pstReg->stGeRegCfg, pstIspCtx->stLinkage.u32Iso);
	
	pstReg->unKey.bit1GeCfg = 1;
    
    return HI_SUCCESS;
}

static __inline HI_S32 GeImageResWrite(ISP_DEV IspDev, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstRes)
{
    hi_isp_ge_width_write(IspDev, pstRes->u16Width-1);
	hi_isp_ge_height_write(IspDev,  pstRes->u16Height-1);

    return HI_SUCCESS;
}


HI_S32 ISP_GeCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
	HI_U8 u8WdrMode;
	
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :
			if (HI_NULL == pValue)
			{
				break;
			}

			u8WdrMode= *((HI_U8 *)pValue);

			if (IS_WDR_MODE(u8WdrMode))
			{
				hi_isp_ge_enable_write(IspDev, 0, HI_TRUE);
				hi_isp_ge_enable_write(IspDev, 1, HI_TRUE);
			}
			else
			{
				hi_isp_ge_enable_write(IspDev, 0, HI_TRUE);
				hi_isp_ge_enable_write(IspDev, 1, HI_FALSE);
			}
			
            break;

	    case ISP_CHANGE_IMAGE_MODE_SET:   
			GeImageResWrite(IspDev, (ISP_CMOS_SENSOR_IMAGE_MODE_S *)pValue);  
			break; 
	
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_GeExit(ISP_DEV IspDev)
{
	ISP_GREEN_EQUALIZATION_S *pstGe = HI_NULL;
    GE_GET_CTX(IspDev, pstGe);

	hi_isp_ge_aver_enable_write(IspDev, HI_FALSE);
	hi_isp_ge_correct_enable_write(IspDev, HI_FALSE);
	hi_isp_ge_enable_write(IspDev, 0, HI_FALSE);   
	hi_isp_ge_enable_write(IspDev, 1, HI_FALSE);   

	if (pstGe->pGdiffBuf)
	{
		free(pstGe->pGdiffBuf);
		pstGe->pGdiffBuf = HI_NULL;
	}
	
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterGe(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_GE;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_GeInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_GeRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_GeCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_GeExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */



