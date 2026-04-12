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
#include "isp_gamma_fe0_mem_config.h"
#include "isp_gamma_fe1_mem_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

static HI_VOID GammaFeExtRegsDefault(ISP_DEV IspDev)
{
    return;
}

static HI_VOID GammaFeRegsDefault(ISP_DEV IspDev) 
{
    return;
}

static HI_VOID GammaFeExtRegsInitialize(ISP_DEV IspDev)
{
    return;
}

static HI_VOID GammaFeRegsInitialize(ISP_DEV IspDev)
{
    HI_S32 i;
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;

    ISP_CTX_S *pstIspCtx = HI_NULL;
    HI_U8 u8WDRMode;
    
    ISP_GET_CTX(IspDev, pstIspCtx);
    u8WDRMode = pstIspCtx->u8SnsWDRMode;
    
    ISP_SensorGetDefault(IspDev, &pstSnsDft);

    if (pstSnsDft->stGammafe.bValid)
    {    
        hi_isp_gammafe_mcu_priority_write(IspDev, 1);

        for(i=0; i<GAMMA_FE0_NODE_NUM; i++)
        {
            hi_gamma_fe0_mem_array_data_write(IspDev, i, (HI_U32)(pstSnsDft->stGammafe.au32Gammafe0[i]));
        }
                
        for(i=0; i<GAMMA_FE1_NODE_NUM; i++)
        {
            hi_gamma_fe1_mem_array_data_write(IspDev, i, (HI_U32)(pstSnsDft->stGammafe.au32Gammafe1[i]));
        }
        hi_isp_gammafe_mcu_priority_write(IspDev, 0);
    }

    if (IS_LINEAR_MODE(u8WDRMode))
    {
        hi_isp_top_bypass_gamma_fe_write(IspDev, 1);
        hi_isp_gammafe_enable_write(IspDev, 0);
        hi_isp_gammafe_enable1_write(IspDev, 0);
        hi_isp_gamma_fe_enable_dl_write(IspDev, 0);
    }
    else if (IS_FS_WDR_MODE(u8WDRMode) || IS_BUILT_IN_WDR_MODE(u8WDRMode))
    {
        hi_isp_top_bypass_gamma_fe_write(IspDev, 1);
        hi_isp_gammafe_enable_write(IspDev, 0);
        hi_isp_gammafe_enable1_write(IspDev, 0);
        hi_isp_top_bypass_gamma_fe_dl_write(IspDev, 0);
        hi_isp_gamma_fe_enable_dl_write(IspDev, 1);
        hi_isp_gamma_fe_alpha_write(IspDev, 0x100);
    }
    else
    {
        /* unknow u8WDRMode */
    }

    return;
}

static HI_S32 GammaFeReadExtregs(ISP_DEV IspDev)
{
    return 0;
}

HI_S32 ISP_GammaFeInit(ISP_DEV IspDev)
{
    GammaFeRegsDefault(IspDev);
    GammaFeExtRegsDefault(IspDev);
    GammaFeRegsInitialize(IspDev);
    GammaFeExtRegsInitialize(IspDev);

    return HI_SUCCESS;
}

HI_S32 ISP_GammaFeRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    GammaFeReadExtregs(IspDev);
    
    return HI_SUCCESS;
}

HI_S32 ISP_GammaFeCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET:
            ISP_GammaFeInit(IspDev);

            break;
        default :
            break;
    }
    
    return HI_SUCCESS;
}

HI_S32 ISP_GammaFeExit(ISP_DEV IspDev)
{
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterGammaFe(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_GAMMAFE;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_GammaFeInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_GammaFeRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_GammaFeCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_GammaFeExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

