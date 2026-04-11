/******************************************************************************

  Copyright (C), 2004-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_split.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2016/01/16
  Description   : 
  History       :
  1.Date        : 2016/01/16
    Author      : q00214668
    Modification: Created file

******************************************************************************/

#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_proc.h"
#include "isp_math_utils.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

/***************************************************************************

****************************************************************************/
static HI_U8 SplitGetBitInWidth(HI_U8 u8InputWidthSel)
{
	switch (u8InputWidthSel)
	{
		case 0:
			return 12;
		case 1:
			return 14;
		case 2:
			return 16;
		case 3:
			return 20;
		default :
			return 12;			
	}
	
}

static HI_VOID SplitRegsInitialize(ISP_DEV IspDev)
{
	HI_U8 u8WDRMode;	
	HI_U8 u8ModeIn, u8ModeOut, u8InputWidthSel, u8InputWidth;
	ISP_CTX_S *pstIspCtx = HI_NULL;
	ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;	

    	ISP_GET_CTX(IspDev, pstIspCtx);	
	ISP_SensorGetDefault(IspDev, &pstSnsDft);
	
	u8WDRMode = pstIspCtx->u8SnsWDRMode;

	if (!pstSnsDft->stSplit.bEnable
		|| IS_LINEAR_MODE(u8WDRMode))
	{
		hi_isp_wdrsplit_en_write(IspDev, HI_FALSE);
		return;
	}    

	u8ModeIn  		= pstSnsDft->stSplit.u8ModeIn;
	u8ModeOut 		= pstSnsDft->stSplit.u8ModeOut;
	u8InputWidthSel	= pstSnsDft->stSplit.u8InputWidthSel;

	u8InputWidth = SplitGetBitInWidth(u8InputWidthSel);
		
	if (u8ModeIn == 0)
	{
		if ((u8InputWidthSel == 0)|(u8InputWidthSel == 1))
		{
			u8ModeOut = 0;			
		}
	}

	hi_isp_wdrsplit_bitw_write(IspDev, pstSnsDft->stSplit.u16OutBits, u8InputWidth);
	hi_isp_wdrsplit_mode_write(IspDev, u8ModeOut, u8ModeIn);
	hi_isp_wdrsplit_offset_r_write(IspDev, pstSnsDft->stSplit.u32OffsetR);
	hi_isp_wdrsplit_offset_gr_write(IspDev, pstSnsDft->stSplit.u32OffsetGR);
	hi_isp_wdrsplit_offset_gb_write(IspDev, pstSnsDft->stSplit.u32OffsetGB);
	hi_isp_wdrsplit_offset_b_write(IspDev, pstSnsDft->stSplit.u32OffsetB);
	hi_isp_wdrsplit_blc_write(IspDev, pstSnsDft->stSplit.u32OutBlc);
	hi_isp_wdrsplit_en_write(IspDev, pstSnsDft->stSplit.bEnable);
		
    return;
}

HI_S32 ISP_SplitInit(ISP_DEV IspDev)
{
    SplitRegsInitialize(IspDev);
    
    return HI_SUCCESS;
}

HI_S32 ISP_SplitRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{	
    return HI_SUCCESS;
}

HI_S32 ISP_SplitCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :
            ISP_SplitInit(IspDev);
            break;
        case ISP_PROC_WRITE:
            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_SplitExit(ISP_DEV IspDev)
{
	hi_isp_wdrsplit_en_write(IspDev, HI_FALSE);
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterSplit(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_SPLIT;
    pstAlgs->stAlgFunc.pfn_alg_init	= ISP_SplitInit;
    pstAlgs->stAlgFunc.pfn_alg_run  	= ISP_SplitRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl 	= ISP_SplitCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit 	= ISP_SplitExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


