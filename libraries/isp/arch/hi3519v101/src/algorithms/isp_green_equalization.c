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

#include "isp_alg.h"
#include "isp_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

static HI_VOID GeExtRegsDefault(ISP_DEV IspDev)
{
    return;
}

static HI_VOID GeRegsDefault(ISP_DEV IspDev) 
{
    hi_isp_green_equalize_dp_enable_write(IspDev, 1);   /* added by d00181140, 2012.04.21, green equalize */

    /* Green Equalization */
    hi_isp_green_equalize_ge_enable_write(IspDev, 1);
    hi_isp_green_equalize_ge_strength_write(IspDev, 0x55);
    hi_isp_raw_frontend_2_ge_strength_write(IspDev, 0x55);
    
    return;
}

static HI_VOID GeExtRegsInitialize(ISP_DEV IspDev)
{
    return;
}

static HI_VOID GeRegsInitialize(ISP_DEV IspDev)
{    
    return;
}

static HI_S32 GeReadExtregs(ISP_DEV IspDev)
{
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
    GeReadExtregs(IspDev);

    hi_isp_raw_frontend_2_ge_enable_write(IspDev, hi_isp_green_equalize_ge_enable_read(IspDev));
    hi_isp_raw_frontend_2_dp_enable_write(IspDev, hi_isp_green_equalize_dp_enable_read(IspDev));
    hi_isp_raw_frontend_2_ge_threshold_write(IspDev, hi_isp_green_equalize_ge_threshold_read(IspDev));
    hi_isp_raw_frontend_2_ge_slope_write(IspDev, hi_isp_green_equalize_ge_slope_read(IspDev));
    hi_isp_raw_frontend_2_ge_sens_write(IspDev, hi_isp_green_equalize_ge_sens_read(IspDev));
    hi_isp_raw_frontend_2_dpdev_threshold_write(IspDev, hi_isp_raw_frontend_hpdev_threshold_read(IspDev));
    hi_isp_raw_frontend_2_line_thresh_write(IspDev, hi_isp_raw_frontend_line_thresh_read(IspDev));
    hi_isp_raw_frontend_2_dp_blend_write(IspDev, hi_isp_raw_frontend_hp_blend_read(IspDev));
    //hi_isp_raw_frontend_2_sigma_in_write(hi_isp_raw_frontend_sigma_in_read());    
    
    return HI_SUCCESS;
}

HI_S32 ISP_GeCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :
            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_GeExit(ISP_DEV IspDev)
{
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



