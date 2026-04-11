/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_awb.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/05
  Description   : 
  History       :
  1.Date        : 2013/01/05
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#include <stdio.h>
#include "isp_alg.h"
#include "isp_ext_config.h"
#include "isp_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ?  (b) : (a))
#endif

static HI_VOID AwbRegsDefault(HI_VOID)
{
    /* still need to set awb default value */    
    hi_isp_metering_white_level_awb_write(0x3ac);
    hi_isp_metering_black_level_awb_write(0x40);
    hi_isp_metering_cr_ref_max_awb_write(256*2);
    hi_isp_metering_cr_ref_min_awb_write(256/2);
    hi_isp_metering_cb_ref_max_awb_write(256*2);
    hi_isp_metering_cb_ref_min_awb_write(256/2);
    hi_isp_metering_cr_ref_high_awb_write(256*2);
    hi_isp_metering_cr_ref_low_awb_write(256/2);
    hi_isp_metering_cb_ref_high_awb_write(256*2);
    hi_isp_metering_cb_ref_low_awb_write(256/2);
    hi_isp_metering_awb_nodes_used_horiz_write(AWB_ZONE_COLUMN);
    hi_isp_metering_awb_nodes_used_vert_write(AWB_ZONE_ROW);

    hi_isp_metering_max_threshold_write(0xfff);
    hi_isp_metering_min_threshold_write(0x0);
    hi_isp_metering_cr_ref_max_awb_sum_write(0x180);
    hi_isp_metering_cr_ref_min_awb_sum_write(0x40);
    hi_isp_metering_cb_ref_max_awb_sum_write(0x180);
    hi_isp_metering_cb_ref_min_awb_sum_write(0x40);
    hi_isp_metering_cr_ref_high_awb_sum_write(0x180);
    hi_isp_metering_cr_ref_low_awb_sum_write(0x40);
    hi_isp_metering_cb_ref_high_awb_sum_write(0x180);
    hi_isp_metering_cb_ref_low_awb_sum_write(0x40);
    hi_isp_metering_awb_sum_nodes_used_horiz_write(AWB_ZONE_COLUMN);
    hi_isp_metering_awb_sum_nodes_used_vert_write(AWB_ZONE_ROW);

    hi_isp_matrix_coefft_wb_r_write(0x100);
    hi_isp_matrix_coefft_wb_g_write(0x100);
    hi_isp_matrix_coefft_wb_b_write(0x100);
    
    return;
}

static HI_U16 AwbSqrt32(HI_U32 u32Arg)
{
    HI_U32 u32Mask = (HI_U32)1 << 15;
    HI_U16 u16Res  = 0;
    HI_U32 i = 0;

    for(i=0; i<16; i++)
    {
        if((u16Res + (u32Mask >> i))*(u16Res + (u32Mask >> i)) <= u32Arg)
        {
            u16Res = u16Res + (u32Mask >> i);
        }
    }

    /* rounding */
    if (u16Res * u16Res + u16Res < u32Arg)
    {
        ++u16Res;
    }
    
    return u16Res;
}

HI_S32 AwbCfgReg(ISP_AWB_RESULT_S *pstAwbResult, HI_U8 u8WDRMode,
    HI_U32 u32IspDgain, HI_U32 u32IspDgainShift, ISP_REG_CFG_S *pstRegCfg)
{
    HI_S32 i;
    HI_U32 au32WbGain[4];
    HI_U16 u16BlackLevel = hi_isp_offset_black_00_read();
    HI_U32 u32SqrtRes;
    
    if (pstAwbResult->stStatAttr.bChange)
    {
        if (IS_LINEAR_MODE(u8WDRMode))
        {
            pstRegCfg->stAwbRegCfg2.u16MeteringWhiteLevelAwb = MIN(pstAwbResult->stStatAttr.u16MeteringWhiteLevelAwb, 0x3FF);
            pstRegCfg->stAwbRegCfg2.u16MeteringBlackLevelAwb = MIN(pstAwbResult->stStatAttr.u16MeteringBlackLevelAwb, 0x3FF);
            pstRegCfg->stAwbRegCfg2.u16MeteringCrRefMaxAwb = MIN(pstAwbResult->stStatAttr.u16MeteringCrRefMaxAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg2.u16MeteringCrRefMinAwb = MIN(pstAwbResult->stStatAttr.u16MeteringCrRefMinAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg2.u16MeteringCbRefMaxAwb = MIN(pstAwbResult->stStatAttr.u16MeteringCbRefMaxAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg2.u16MeteringCbRefMinAwb = MIN(pstAwbResult->stStatAttr.u16MeteringCbRefMinAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg2.u16MeteringCrRefHighAwb = MIN(pstAwbResult->stStatAttr.u16MeteringCrRefHighAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg2.u16MeteringCrRefLowAwb = MIN(pstAwbResult->stStatAttr.u16MeteringCrRefLowAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg2.u16MeteringCbRefHighAwb = MIN(pstAwbResult->stStatAttr.u16MeteringCbRefHighAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg2.u16MeteringCbRefLowAwb = MIN(pstAwbResult->stStatAttr.u16MeteringCbRefLowAwb, 0xFFF);
        }
        else if (IS_WDR_MODE(u8WDRMode))
        {
            u32SqrtRes = AwbSqrt32((HI_U32)pstAwbResult->stStatAttr.u16MeteringWhiteLevelAwb << 10);
            pstRegCfg->stAwbRegCfg2.u16MeteringWhiteLevelAwb = MIN(u32SqrtRes, 0x3FF); 
            u32SqrtRes = AwbSqrt32((HI_U32)pstAwbResult->stStatAttr.u16MeteringBlackLevelAwb << 10);
            pstRegCfg->stAwbRegCfg2.u16MeteringBlackLevelAwb = MIN(u32SqrtRes, 0x3FF); 
            u32SqrtRes = AwbSqrt32((HI_U32)pstAwbResult->stStatAttr.u16MeteringCrRefMaxAwb << 8);
            pstRegCfg->stAwbRegCfg2.u16MeteringCrRefMaxAwb = MIN(u32SqrtRes, 0xFFF); 
            u32SqrtRes = AwbSqrt32((HI_U32)pstAwbResult->stStatAttr.u16MeteringCrRefMinAwb << 8);
            pstRegCfg->stAwbRegCfg2.u16MeteringCrRefMinAwb = MIN(u32SqrtRes, 0xFFF); 
            u32SqrtRes = AwbSqrt32((HI_U32)pstAwbResult->stStatAttr.u16MeteringCbRefMaxAwb << 8);
            pstRegCfg->stAwbRegCfg2.u16MeteringCbRefMaxAwb = MIN(u32SqrtRes, 0xFFF); 
            u32SqrtRes = AwbSqrt32((HI_U32)pstAwbResult->stStatAttr.u16MeteringCbRefMinAwb << 8);
            pstRegCfg->stAwbRegCfg2.u16MeteringCbRefMinAwb = MIN(u32SqrtRes, 0xFFF); 
            u32SqrtRes = AwbSqrt32((HI_U32)pstAwbResult->stStatAttr.u16MeteringCrRefHighAwb << 8);
            pstRegCfg->stAwbRegCfg2.u16MeteringCrRefHighAwb = MIN(u32SqrtRes, 0xFFF); 
            u32SqrtRes = AwbSqrt32((HI_U32)pstAwbResult->stStatAttr.u16MeteringCrRefLowAwb << 8);
            pstRegCfg->stAwbRegCfg2.u16MeteringCrRefLowAwb =  MIN(u32SqrtRes, 0xFFF); 
            u32SqrtRes = AwbSqrt32((HI_U32)pstAwbResult->stStatAttr.u16MeteringCbRefHighAwb << 8);
            pstRegCfg->stAwbRegCfg2.u16MeteringCbRefHighAwb = MIN(u32SqrtRes, 0xFFF); 
            u32SqrtRes = AwbSqrt32((HI_U32)pstAwbResult->stStatAttr.u16MeteringCbRefLowAwb << 8);
            pstRegCfg->stAwbRegCfg2.u16MeteringCbRefLowAwb = MIN(u32SqrtRes, 0xFFF); 
        }
        else
        {
            /* unknow u8WDRMode */
        }
        pstRegCfg->unKey.bit1AwbCfg2 = 1;
    }

    if (pstAwbResult->stRawStatAttr.bChange)
    {
        if (IS_LINEAR_MODE(u8WDRMode))
        {
            pstRegCfg->stAwbRegCfg3.bAboveWhiteLevelClip = pstAwbResult->stRawStatAttr.bAboveWhiteLevelClip;
            pstRegCfg->stAwbRegCfg3.bBelowBlackLevelClip = pstAwbResult->stRawStatAttr.bBelowBlackLevelClip;
            pstRegCfg->stAwbRegCfg3.u16MeteringWhiteLevelAwb = MIN(pstAwbResult->stRawStatAttr.u16MeteringWhiteLevelAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg3.u16MeteringBlackLevelAwb = pstAwbResult->stRawStatAttr.u16MeteringBlackLevelAwb;
            pstRegCfg->stAwbRegCfg3.u16MeteringCrRefMaxAwb = MIN(pstAwbResult->stRawStatAttr.u16MeteringCrRefMaxAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg3.u16MeteringCrRefMinAwb = MIN(pstAwbResult->stRawStatAttr.u16MeteringCrRefMinAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg3.u16MeteringCbRefMaxAwb = MIN(pstAwbResult->stRawStatAttr.u16MeteringCbRefMaxAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg3.u16MeteringCbRefMinAwb = MIN(pstAwbResult->stRawStatAttr.u16MeteringCbRefMinAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg3.u16MeteringCrRefHighAwb = MIN(pstAwbResult->stRawStatAttr.u16MeteringCrRefHighAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg3.u16MeteringCrRefLowAwb = MIN(pstAwbResult->stRawStatAttr.u16MeteringCrRefLowAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg3.u16MeteringCbRefHighAwb = MIN(pstAwbResult->stRawStatAttr.u16MeteringCbRefHighAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg3.u16MeteringCbRefLowAwb = MIN(pstAwbResult->stRawStatAttr.u16MeteringCbRefLowAwb, 0xFFF);
            
        }
        else if (IS_WDR_MODE(u8WDRMode))
        {
            pstRegCfg->stAwbRegCfg3.bAboveWhiteLevelClip = pstAwbResult->stRawStatAttr.bAboveWhiteLevelClip;
            pstRegCfg->stAwbRegCfg3.bBelowBlackLevelClip = pstAwbResult->stRawStatAttr.bBelowBlackLevelClip;
            pstRegCfg->stAwbRegCfg3.u16MeteringBlackLevelAwb = pstAwbResult->stRawStatAttr.u16MeteringBlackLevelAwb;   /*offset of black level*/

            u32SqrtRes = AwbSqrt32((HI_U32)pstAwbResult->stRawStatAttr.u16MeteringWhiteLevelAwb << 12); 
            pstRegCfg->stAwbRegCfg3.u16MeteringWhiteLevelAwb = MIN(u32SqrtRes, 0xFFF); 
            u32SqrtRes = AwbSqrt32((HI_U32)pstAwbResult->stRawStatAttr.u16MeteringCrRefMaxAwb << 8);
            pstRegCfg->stAwbRegCfg3.u16MeteringCrRefMaxAwb = MIN(u32SqrtRes, 0xFFF); 
            u32SqrtRes = AwbSqrt32((HI_U32)pstAwbResult->stRawStatAttr.u16MeteringCrRefMinAwb << 8);
            pstRegCfg->stAwbRegCfg3.u16MeteringCrRefMinAwb = MIN(u32SqrtRes, 0xFFF); 
            u32SqrtRes = AwbSqrt32((HI_U32)pstAwbResult->stRawStatAttr.u16MeteringCbRefMaxAwb << 8);
            pstRegCfg->stAwbRegCfg3.u16MeteringCbRefMaxAwb = MIN(u32SqrtRes, 0xFFF); 
            u32SqrtRes = AwbSqrt32((HI_U32)pstAwbResult->stRawStatAttr.u16MeteringCbRefMinAwb << 8);
            pstRegCfg->stAwbRegCfg3.u16MeteringCbRefMinAwb = MIN(u32SqrtRes, 0xFFF); 
            u32SqrtRes = AwbSqrt32((HI_U32)pstAwbResult->stRawStatAttr.u16MeteringCrRefHighAwb << 8);
            pstRegCfg->stAwbRegCfg3.u16MeteringCrRefHighAwb = MIN(u32SqrtRes, 0xFFF); 
            u32SqrtRes = AwbSqrt32((HI_U32)pstAwbResult->stRawStatAttr.u16MeteringCrRefLowAwb << 8);
            pstRegCfg->stAwbRegCfg3.u16MeteringCrRefLowAwb = MIN(u32SqrtRes, 0xFFF); 
            u32SqrtRes = AwbSqrt32((HI_U32)pstAwbResult->stRawStatAttr.u16MeteringCbRefHighAwb << 8);
            pstRegCfg->stAwbRegCfg3.u16MeteringCbRefHighAwb = MIN(u32SqrtRes, 0xFFF); 
            u32SqrtRes = AwbSqrt32((HI_U32)pstAwbResult->stRawStatAttr.u16MeteringCbRefLowAwb << 8);
            pstRegCfg->stAwbRegCfg3.u16MeteringCbRefLowAwb = MIN(u32SqrtRes, 0xFFF); 

        }
        else
        {
            /* unknow u8WDRMode */
        }
        pstRegCfg->stAwbRegCfg3.u16MeteringBlackLevelAwb += u16BlackLevel;
        pstRegCfg->stAwbRegCfg3.u16MeteringBlackLevelAwb = MIN(pstRegCfg->stAwbRegCfg3.u16MeteringBlackLevelAwb, 0xFFF); 
        pstRegCfg->unKey.bit1AwbCfg3 = 1;
    }

    for (i=0; i<9; i++)
    {
        pstRegCfg->stAwbRegCfg1.au16ColorMatrix[i] = pstAwbResult->au16ColorMatrix[i];
    }

    if (IS_LINEAR_MODE(u8WDRMode))
    {
        for (i=0; i<4; i++)
        {
            au32WbGain[i] = pstAwbResult->au32WhiteBalanceGain[i];           
            au32WbGain[i] = (au32WbGain[i] + 128) >> 8;
            /*avoid overflow*/
            au32WbGain[i] = (au32WbGain[i] > 0xFFF) ? 0xFFF : au32WbGain[i];

            pstRegCfg->stAwbRegCfg1.au32WhiteBalanceGain[i] = (HI_U16)au32WbGain[i];
        }
    }
    else if (IS_WDR_MODE(u8WDRMode))
    {
        for (i=0; i<4; i++)
        {
            au32WbGain[i] = pstAwbResult->au32WhiteBalanceGain[i];
            /*avoid overflow*/
            au32WbGain[i] = (au32WbGain[i] > 0xFFE000) ? 0xFFE000 : au32WbGain[i];

            pstRegCfg->stAwbRegCfg1.au32WhiteBalanceGain[i] = AwbSqrt32(au32WbGain[i]);
        }
    }
    else
    {
        /* unknow u8WDRMode */
    }

    pstRegCfg->unKey.bit1AwbCfg1 = 1;

    return HI_SUCCESS;
}
HI_S32 ISP_AwbCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue);
HI_S32 ISP_AwbInit(ISP_DEV IspDev)
{
    HI_S32 i;
    ISP_AWB_PARAM_S stAwbParam;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_LIB_NODE_S *pstLib = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    AwbRegsDefault();
    
    stAwbParam.SensorId = pstIspCtx->stBindAttr.SensorId;
    stAwbParam.u8WDRMode = pstIspCtx->u8SnsWDRMode;

    for (i=0; i<MAX_REGISTER_ALG_LIB_NUM; i++)
    {
        if (pstIspCtx->stAwbLibInfo.astLibs[i].bUsed)
        {
            pstLib = &pstIspCtx->stAwbLibInfo.astLibs[i];
            if (HI_NULL != pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_init)
            {
                pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_init(
                    pstLib->stAlgLib.s32Id, &stAwbParam);
            }
        }
    }
   
    return HI_SUCCESS;
}

HI_S32 ISP_AwbRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    HI_S32 i, s32Ret = HI_FAILURE;
    ISP_AWB_INFO_S   stAwbInfo = {0};
    ISP_AWB_RESULT_S stAwbResult = {{0}};
    ISP_CTX_S       *pstIspCtx   = HI_NULL;
    ISP_LIB_NODE_S  *pstLib      = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);
    pstLib = &pstIspCtx->stAwbLibInfo.astLibs[pstIspCtx->stAwbLibInfo.u32ActiveLib];

    if (pstIspCtx->stLinkage.bDefectPixel)
    {
        return HI_SUCCESS;
    }

    /* get statistics */
    stAwbInfo.u32FrameCnt = pstIspCtx->u32FrameCnt;

    stAwbInfo.pstAwbStat1 = &((ISP_STAT_S *)pStatInfo)->stAwbStat1;
    stAwbInfo.pstAwbStat2 = &((ISP_STAT_S *)pStatInfo)->stAwbStat2;
    stAwbInfo.pstAwbStat3 = &((ISP_STAT_S *)pStatInfo)->stAwbStat3;
    stAwbInfo.pstAwbStat4 = &((ISP_STAT_S *)pStatInfo)->stAwbStat4;

    /* linkage with the iso of ae */
    for (i=0; i<MAX_REGISTER_ALG_LIB_NUM; i++)
    {
        if (pstIspCtx->stAwbLibInfo.astLibs[i].bUsed)
        {
            pstLib = &pstIspCtx->stAwbLibInfo.astLibs[i];
            if (HI_NULL != pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl)
            {
                pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->stAlgLib.s32Id,
                    ISP_AWB_ISO_SET, (HI_VOID *)&pstIspCtx->stLinkage.u32Iso);
                pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->stAlgLib.s32Id,
                    ISP_AWB_INTTIME_SET, (HI_VOID *)&pstIspCtx->stLinkage.u32Inttime);
                pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->stAlgLib.s32Id,
                    ISP_AWB_PIRIS_SET, (HI_VOID *)&pstIspCtx->stLinkage.u32PirisGain);
            }
        }
    }

    pstLib = &pstIspCtx->stAwbLibInfo.astLibs[pstIspCtx->stAwbLibInfo.u32ActiveLib];
    if (HI_NULL != pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_run)
    {
        s32Ret = pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_run(
            pstLib->stAlgLib.s32Id, &stAwbInfo, &stAwbResult, 0);
        if (HI_SUCCESS != s32Ret)
        {
            printf("WARNING!! run awb lib err 0x%x!\n", s32Ret);
        }
    }
    
    AwbCfgReg(&stAwbResult, pstIspCtx->u8SnsWDRMode, pstIspCtx->stLinkage.u32IspDgain,
        pstIspCtx->stLinkage.u32IspDgainShift, (ISP_REG_CFG_S *)pRegCfg);
    
    return s32Ret;
}

HI_S32 ISP_AwbCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{    
    HI_S32  i, s32Ret = HI_FAILURE;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_LIB_NODE_S *pstLib = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    for (i=0; i<MAX_REGISTER_ALG_LIB_NUM; i++)
    {
        if (pstIspCtx->stAwbLibInfo.astLibs[i].bUsed)
        {
            pstLib = &pstIspCtx->stAwbLibInfo.astLibs[i];
            if (HI_NULL != pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl)
            {
                s32Ret = pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl(
                    pstLib->stAlgLib.s32Id, u32Cmd, pValue);
            }
        }
    }

    return s32Ret;
}

HI_S32 ISP_AwbExit(ISP_DEV IspDev)
{
    HI_S32 i;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_LIB_NODE_S *pstLib = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    for (i=0; i<MAX_REGISTER_ALG_LIB_NUM; i++)
    {
        if (pstIspCtx->stAwbLibInfo.astLibs[i].bUsed)
        {
            pstLib = &pstIspCtx->stAwbLibInfo.astLibs[i];
            if (HI_NULL != pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_exit)
            {
                pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_exit(
                    pstLib->stAlgLib.s32Id);
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterAwb(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_AWB;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_AwbInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_AwbRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_AwbCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_AwbExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

