/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_pregamma.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   : 
  History       :
  1.Date        : 2013/01/16
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#include "isp_alg.h"
#include "isp_sensor.h"

#include "isp_config.h"
#include "isp_gamma_be0_mem_config.h"
#include "isp_gamma_be1_mem_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

static HI_VOID GammaBeExtRegsDefault(ISP_DEV IspDev)
{
    return;
}

static HI_VOID GammaBeRegsDefault(ISP_DEV IspDev) 
{
    return;
}

static HI_VOID GammaBeExtRegsInitialize(ISP_DEV IspDev)
{
    return;
}

static HI_VOID GammaBeRegsInitialize(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    HI_U8 u8WDRMode;
    
    ISP_GET_CTX(IspDev, pstIspCtx);
    u8WDRMode = pstIspCtx->u8SnsWDRMode;

    if (IS_LINEAR_MODE(u8WDRMode))
    {
        hi_isp_top_bypass_gamma_be_write(IspDev, 1);
        hi_isp_gamma_be_enable_0_write(IspDev, 0);
        hi_isp_gamma_be_enable_1_write(IspDev, 0);
        hi_isp_gamma_be_enable_dl_write(IspDev, 0);
    }
    else if (IS_FS_WDR_MODE(u8WDRMode) || IS_BUILT_IN_WDR_MODE(u8WDRMode))
    {
        hi_isp_top_bypass_gamma_be_write(IspDev, 1);
        hi_isp_gamma_be_enable_0_write(IspDev, 0);
        hi_isp_gamma_be_enable_1_write(IspDev, 0);
        hi_isp_top_bypass_gamma_be_dl_write(IspDev, 0);
        hi_isp_gamma_be_enable_dl_write(IspDev, 1);
        hi_isp_gamma_be_alpha_write(IspDev, 0x10000);
    }
    else
    {
        /* unknow u8WDRMode */
    }
    
    return;
}

static HI_S32 GammaBeReadExtregs(ISP_DEV IspDev)
{
    return 0;
}

HI_S32 ISP_GammaBeInit(ISP_DEV IspDev)
{
    GammaBeRegsDefault(IspDev);
    GammaBeExtRegsDefault(IspDev);
    GammaBeRegsInitialize(IspDev);
    GammaBeExtRegsInitialize(IspDev);

    return HI_SUCCESS;
}

HI_S32 ISP_GammaBeRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{   
    GammaBeReadExtregs(IspDev);
    
    return HI_SUCCESS;
}

HI_S32 ISP_GammaBeCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET:
            ISP_GammaBeInit(IspDev);
            break;
        default :
            break;
    }
    
    return HI_SUCCESS;
}

HI_S32 ISP_GammaBeExit(ISP_DEV IspDev)
{
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterGammaBe(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_GAMMABE;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_GammaBeInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_GammaBeRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_GammaBeCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_GammaBeExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

