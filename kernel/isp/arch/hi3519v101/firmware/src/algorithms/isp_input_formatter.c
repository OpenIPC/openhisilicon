/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_input_formatter.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/06/24
  Description   : 
  History       :
  1.Date        : 2015/06/24
    Author      : h00191408
    Modification: Created file

******************************************************************************/

#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_ext_config.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define INPUT_FORMATTER_MODE_IN_LINEAR 0x0
#define INPUT_FORMATTER_MODE_IN_CHANNEL_CODED 0x1
#define INPUT_FORMATTER_MODE_IN_LOG 0x2
#define INPUT_FORMATTER_MODE_IN_COMPANDED 0x3

#define INPUT_FORMATTER_MODE_OUT_PASS_THROUGH 0x0
#define INPUT_FORMATTER_MODE_OUT_MAP_2_CHANNEL 0x1
#define INPUT_FORMATTER_MODE_OUT_MAP_3_CHANNEL 0x2
#define INPUT_FORMATTER_MODE_OUT_MAP_4_CHANNEL 0x3

static HI_VOID IFExtRegsDefault(ISP_DEV IspDev)
{
    return;
}

static HI_VOID IFRegsDefault(ISP_DEV IspDev) 
{
    return;
}

static HI_VOID IFExtRegsInitialize(ISP_DEV IspDev)
{
    return;
}

static HI_VOID IFRegsInitialize(ISP_DEV IspDev)
{ 
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
    ISP_CTX_S    *pstIspCtx = HI_NULL;
    ISP_GET_CTX(IspDev, pstIspCtx);
    HI_U8 u8WDRMode = pstIspCtx->u8SnsWDRMode;
    
    ISP_SensorGetDefault(IspDev, &pstSnsDft);

    if (pstSnsDft->stInputFormatter.bValid)
    { 
        hi_isp_input_formatter_knee_point0_write(IspDev, pstSnsDft->stInputFormatter.au16KneePoint[0]);
        hi_isp_input_formatter_knee_point1_write(IspDev, pstSnsDft->stInputFormatter.au16KneePoint[1]);
        hi_isp_input_formatter_knee_point2_write(IspDev, pstSnsDft->stInputFormatter.au16KneePoint[2]);
        hi_isp_input_formatter_slope0_select_write(IspDev, pstSnsDft->stInputFormatter.au8SlopeSelect[0]);
        hi_isp_input_formatter_slope1_select_write(IspDev, pstSnsDft->stInputFormatter.au8SlopeSelect[1]);
        hi_isp_input_formatter_slope2_select_write(IspDev, pstSnsDft->stInputFormatter.au8SlopeSelect[2]);
        hi_isp_input_formatter_slope3_select_write(IspDev, pstSnsDft->stInputFormatter.au8SlopeSelect[3]);
    }

    if (IS_LINEAR_MODE(u8WDRMode))
    {
        hi_isp_input_formatter_mode_in_write(IspDev, INPUT_FORMATTER_MODE_IN_LINEAR);
        hi_isp_input_formatter_mode_out_write(IspDev, INPUT_FORMATTER_MODE_OUT_PASS_THROUGH);
        hi_isp_input_formatter_input_bitwidth_select_write(IspDev, 0);
    }
    else if (IS_BUILT_IN_WDR_MODE(u8WDRMode))
    {
        hi_isp_input_formatter_mode_in_write(IspDev, INPUT_FORMATTER_MODE_IN_COMPANDED);
        hi_isp_input_formatter_mode_out_write(IspDev, INPUT_FORMATTER_MODE_OUT_MAP_3_CHANNEL);
        //hi_isp_input_formatter_mode_out_write(IspDev, INPUT_FORMATTER_MODE_OUT_MAP_2_CHANNEL);
        hi_isp_input_formatter_input_bitwidth_select_write(IspDev, 0);
    }
    else if (IS_FS_WDR_MODE(u8WDRMode))
    {
        hi_isp_input_formatter_mode_in_write(IspDev, INPUT_FORMATTER_MODE_IN_LINEAR);
        hi_isp_input_formatter_mode_out_write(IspDev, INPUT_FORMATTER_MODE_OUT_PASS_THROUGH);
        hi_isp_input_formatter_input_bitwidth_select_write(IspDev, 0);
    }
    else
    {
        /* unknow u8WDRMode */
    }

    return;
}

static HI_S32 IFReadExtregs(ISP_DEV IspDev)
{
    return 0;
}

HI_S32 ISP_IFInit(ISP_DEV IspDev)
{
    IFRegsDefault(IspDev);
    IFExtRegsDefault(IspDev);
    IFRegsInitialize(IspDev);
    IFExtRegsInitialize(IspDev);
    
    return HI_SUCCESS;
}

HI_S32 ISP_IFRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    IFReadExtregs(IspDev);

    return HI_SUCCESS;
}

HI_S32 ISP_IFCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :
            ISP_IFInit(IspDev);
            break;
            
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_IFExit(ISP_DEV IspDev)
{
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterIF(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_IF;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_IFInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_IFRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_IFCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_IFExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


