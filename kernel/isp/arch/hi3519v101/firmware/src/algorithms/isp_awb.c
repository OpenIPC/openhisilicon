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

/* Convert to direct presentation (used in the ISP) */
static HI_U16 AwbComplementToDirect(HI_S16 s16Value)
{
    HI_U16 u16Result;

    if (s16Value >= 0)
    {
        u16Result = s16Value;
    }
    else
    {
        u16Result = -s16Value;
        u16Result |= (1 << 15);
    }
    return u16Result;
}

/* Convert to complement presentation (used in the firmware for calculations) */
static HI_S16 AwbDirectToComplement(HI_U16 u16Value)
{
    HI_S16 s16Result;

    s16Result = u16Value & (~(1 << 15));

    if(u16Value &  (1 << 15))
    {
        s16Result = - s16Result;
    }

    return s16Result;
}

/* Multiply the two matrixes a1[dim1 x dim2] a2[dim2 x dim3] */
static HI_VOID AwbMatrixMultiply(HI_S16 *ps16Matrix1, HI_S16 *ps16Matrix2,
    HI_S16 *ps16Result, HI_S32 s32Dim1, HI_S32 s32Dim2, HI_S32 s32Dim3)
{
    HI_S32 i, j, k;
    HI_S32 s32Temp;

    for (i=0; i<s32Dim1; ++i)
    {
        for (j=0; j<s32Dim3; ++j)
        {
            s32Temp = 0;
            for (k=0; k<s32Dim2; ++k)
            {
                s32Temp += (((HI_S32)ps16Matrix1[i * s32Dim2 + k] * ps16Matrix2[k * s32Dim3 + j]));
            }
            ps16Result[i * s32Dim3 + j] = (HI_S16)((s32Temp+ 128) >> 8);
        }
    }

    for (i=0; i<s32Dim1; ++i)
    {
        s32Temp = 0;        
        for (j=0; j<s32Dim3; ++j)
        {
            s32Temp += (HI_S32)ps16Result[i * s32Dim3 + j]; 
        } 
        
        if (0x0 != s32Temp)
        {
            for (j=0; j<s32Dim3; ++j)
            {
                ps16Result[i * s32Dim3 + j] = (HI_S16)(ps16Result[i * s32Dim3 + j] * 0x100 / s32Temp); 
            } 
        }
    }    
	
    for (i=0; i<s32Dim1; ++i)
    {
        s32Temp = 0;        
        for (j=0; j<s32Dim3; ++j)
        {
            s32Temp += (HI_S16)ps16Result[i * s32Dim3 + j]; 
        } 

        if (0x100 != s32Temp)
        {
            ps16Result[i * s32Dim3 + i] += (0x100 - s32Temp);
        }
    }    

    return;
}

static HI_VOID AwbRegsDefault(ISP_DEV IspDev, HI_U8 u8AWBZoneRow, HI_U8 u8AWBZoneCol)
{
    hi_isp_metering_awb_nodes_used_horiz_write(IspDev, u8AWBZoneCol);
    hi_isp_metering_awb_nodes_used_vert_write(IspDev, u8AWBZoneRow);
    hi_isp_metering_awb_sum_nodes_used_horiz_write(IspDev, u8AWBZoneCol);
    hi_isp_metering_awb_sum_nodes_used_vert_write(IspDev, u8AWBZoneRow);

    /* still need to set awb default value */    
    hi_isp_metering_white_level_awb_write(IspDev, 0x3ac);
    hi_isp_metering_black_level_awb_write(IspDev, 0x40);
    hi_isp_metering_cr_ref_max_awb_write(IspDev, 256*2);
    hi_isp_metering_cr_ref_min_awb_write(IspDev, 256/2);
    hi_isp_metering_cb_ref_max_awb_write(IspDev, 256*2);
    hi_isp_metering_cb_ref_min_awb_write(IspDev, 256/2);
    hi_isp_metering_cr_ref_high_awb_write(IspDev, 256*2);
    hi_isp_metering_cr_ref_low_awb_write(IspDev, 256/2);
    hi_isp_metering_cb_ref_high_awb_write(IspDev, 256*2);
    hi_isp_metering_cb_ref_low_awb_write(IspDev, 256/2);

    hi_isp_metering_max_threshold_write(IspDev, 0xfff);
    hi_isp_metering_min_threshold_write(IspDev, 0x0);
    hi_isp_metering_cr_ref_max_awb_sum_write(IspDev, 0x180);
    hi_isp_metering_cr_ref_min_awb_sum_write(IspDev, 0x40);
    hi_isp_metering_cb_ref_max_awb_sum_write(IspDev, 0x180);
    hi_isp_metering_cb_ref_min_awb_sum_write(IspDev, 0x40);
    hi_isp_metering_cr_ref_high_awb_sum_write(IspDev, 0x180);
    hi_isp_metering_cr_ref_low_awb_sum_write(IspDev, 0x40);
    hi_isp_metering_cb_ref_high_awb_sum_write(IspDev, 0x180);
    hi_isp_metering_cb_ref_low_awb_sum_write(IspDev, 0x40);

    hi_ext_system_bayer_statistics_blacklevel_write(IspDev, HI_EXT_SYSTEM_BAYER_STATISTICS_BLACKLEVEL_DEFAULT);
    hi_ext_system_bayer_statistics_whitelevel_write(IspDev, HI_EXT_SYSTEM_BAYER_STATISTICS_WHITELEVEL_DEFAULT);

    hi_isp_matrix_coefft_wb_r_write(IspDev, 0x100);
    hi_isp_matrix_coefft_wb_g_write(IspDev, 0x100);
    hi_isp_matrix_coefft_wb_b_write(IspDev, 0x100);
    hi_isp_metering_awb_stats_mode_write(IspDev, 1);
    
    hi_isp_top_awbsum_switch_write(IspDev, 1);
    
    return;
}

#if 0
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
#endif

#if 0
/*Alpha if fixed to 0x100, input is 12 bit width, output is 12 bit width*/
static HI_S32 GammaFe(HI_U16 u16x)
{
    return ((HI_U32)u16x << 20) / ((1 << 16) + (15 * (HI_U32)u16x << 4));
}
#endif

HI_S32 AwbCfgReg(ISP_DEV IspDev, ISP_AWB_RESULT_S *pstAwbResult, HI_U8 u8WDRMode,
    HI_U32 u32IspDgain, HI_U32 u32IspDgainShift, ISP_REG_CFG_S *pstRegCfg)
{
    HI_S32 i;
    HI_U32 au32WbGain[4];
    ISP_CTX_S       *pstIspCtx   = HI_NULL;    
    ISP_GET_CTX(IspDev, pstIspCtx);
    
    if (pstAwbResult->stStatAttr.bChange)
    {
        //if (IS_LINEAR_MODE(u8WDRMode))
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
       
        pstRegCfg->unKey.bit1AwbCfg2 = 1;
    }

    if (pstAwbResult->stRawStatAttr.bChange)
    {
        //if (IS_LINEAR_MODE(u8WDRMode))
        {
            pstRegCfg->stAwbRegCfg3.bAboveWhiteLevelClip = pstAwbResult->stRawStatAttr.bAboveWhiteLevelClip;
            pstRegCfg->stAwbRegCfg3.bBelowBlackLevelClip = pstAwbResult->stRawStatAttr.bBelowBlackLevelClip;
            pstRegCfg->stAwbRegCfg3.u16MeteringCrRefMaxAwb = MIN(pstAwbResult->stRawStatAttr.u16MeteringCrRefMaxAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg3.u16MeteringCrRefMinAwb = MIN(pstAwbResult->stRawStatAttr.u16MeteringCrRefMinAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg3.u16MeteringCbRefMaxAwb = MIN(pstAwbResult->stRawStatAttr.u16MeteringCbRefMaxAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg3.u16MeteringCbRefMinAwb = MIN(pstAwbResult->stRawStatAttr.u16MeteringCbRefMinAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg3.u16MeteringCrRefHighAwb = MIN(pstAwbResult->stRawStatAttr.u16MeteringCrRefHighAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg3.u16MeteringCrRefLowAwb = MIN(pstAwbResult->stRawStatAttr.u16MeteringCrRefLowAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg3.u16MeteringCbRefHighAwb = MIN(pstAwbResult->stRawStatAttr.u16MeteringCbRefHighAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg3.u16MeteringCbRefLowAwb = MIN(pstAwbResult->stRawStatAttr.u16MeteringCbRefLowAwb, 0xFFF);
            
        }       

        pstRegCfg->unKey.bit1AwbCfg3 = 1;
    }

    if (1)
    {
        HI_U16 u16WhiteLevel = MIN(hi_ext_system_bayer_statistics_whitelevel_read(IspDev), 0xFFF);
        HI_U16 u16BlackLevel = MIN(hi_ext_system_bayer_statistics_blacklevel_read(IspDev), 0xFFF);

        if (u16WhiteLevel != pstRegCfg->stAwbRegCfg3.u16MeteringWhiteLevelAwb)
        {
            pstRegCfg->stAwbRegCfg3.u16MeteringWhiteLevelAwb = u16WhiteLevel;
            hi_isp_metering_max_threshold_write(IspDev, pstRegCfg->stAwbRegCfg3.u16MeteringWhiteLevelAwb);
        }

        if (u16BlackLevel != pstRegCfg->stAwbRegCfg3.u16MeteringBlackLevelAwb)
        {
            pstRegCfg->stAwbRegCfg3.u16MeteringBlackLevelAwb = u16BlackLevel;
            hi_isp_metering_min_threshold_write(IspDev, pstRegCfg->stAwbRegCfg3.u16MeteringBlackLevelAwb);
        }     
    }

    for (i=0; i<9; i++)
    {
        pstRegCfg->stAwbRegCfg1.au16ColorMatrix[i] = pstAwbResult->au16ColorMatrix[i];
    }

    //if (IS_LINEAR_MODE(u8WDRMode))
    if (HI_FALSE == pstIspCtx->stLinkage.bCapState)
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

    pstRegCfg->unKey.bit1ACMCfg = pstAwbResult->stAcmGainAttr.bChange;
    if (pstAwbResult->stAcmGainAttr.bChange)
    {
        pstRegCfg->stAcmRegCfg.u32GainLuma = pstAwbResult->stAcmGainAttr.u16GainLuma;
        pstRegCfg->stAcmRegCfg.u32GainHue = pstAwbResult->stAcmGainAttr.u16GainHue;
        pstRegCfg->stAcmRegCfg.u32GainSat = pstAwbResult->stAcmGainAttr.u16GainSat;
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

    stAwbParam.SensorId = pstIspCtx->stBindAttr.SensorId;
    stAwbParam.u8WDRMode = pstIspCtx->u8SnsWDRMode;
    if (ISP_MODE_STITCH == g_astIspCtx[0].enIspMode) 
    {
        stAwbParam.u8AWBZoneRow = AWB_ZONE_ROW_STITCH;
        stAwbParam.u8AWBZoneCol = AWB_ZONE_COLUMN_STITCH;
        AwbRegsDefault(IspDev, AWB_ZONE_ROW_STITCH, AWB_ZONE_COLUMN_STITCH / 2);
    }
    else
    {
        stAwbParam.u8AWBZoneRow = AWB_ZONE_ROW;
        stAwbParam.u8AWBZoneCol = AWB_ZONE_COLUMN;
        AwbRegsDefault(IspDev, AWB_ZONE_ROW, AWB_ZONE_COLUMN);
    }

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
    HI_U32 u32DiffGain;
    HI_BOOL bUseInfoCCM;
    AWB_CCM_CONFIG_S stCCMConf,stCCMConfDef;
    
    ISP_GET_CTX(IspDev, pstIspCtx);
    pstLib = &pstIspCtx->stAwbLibInfo.astLibs[pstIspCtx->stAwbLibInfo.u32ActiveLib];

    if (pstIspCtx->stLinkage.bDefectPixel)
    {
        return HI_SUCCESS;
    }

    bUseInfoCCM = hi_ext_system_snap_use_info_ccm_enable_read(IspDev);
    if (HI_TRUE == pstIspCtx->stLinkage.bCapState)
    {
        if (HI_TRUE == bUseInfoCCM)
        {
            return HI_SUCCESS;
        }
        else
        {
            pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->stAlgLib.s32Id,
                    AWB_CCM_CONFIG_GET, (HI_VOID *)&stCCMConfDef);
            stCCMConf.bAWBBypassEn = HI_TRUE;
            stCCMConf.bManualTempEn = HI_TRUE;
            stCCMConf.u32ManualTempValue = pstIspCtx->stLinkage.u32ColorTemp;
            stCCMConf.u16CCMSpeed = 0xfff;

            stCCMConf.bManualSatEn = stCCMConfDef.bManualSatEn;
            stCCMConf.u32ManualSatValue = stCCMConfDef.u32ManualSatValue;
            pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->stAlgLib.s32Id,
                    AWB_CCM_CONFIG_SET, (HI_VOID *)&stCCMConf);
        }
    }

    if (((SNAP_TYPE_SYNC == pstIspCtx->stLinkage.enSnapType) || (SNAP_TYPE_SYNC_PRO == pstIspCtx->stLinkage.enSnapType)) && (ISP_SNAP_PICTURE == pstIspCtx->stLinkage.enSnapPipeMode))
    {
        return HI_SUCCESS;
    }

    /* get statistics */
    stAwbInfo.u32FrameCnt = pstIspCtx->u32FrameCnt;

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
            }
        }
    }

    pstLib = &pstIspCtx->stAwbLibInfo.astLibs[pstIspCtx->stAwbLibInfo.u32ActiveLib];

    switch (g_astIspCtx[0].enIspMode) 
    {
        default:
        case ISP_MODE_NORMAL:
            stAwbInfo.pstAwbStat1 = &((ISP_STAT_S *)pStatInfo)->stAwbStat1;
            stAwbInfo.pstAwbStat2 = &((ISP_STAT_S *)pStatInfo)->stAwbStat2;
            stAwbInfo.pstAwbStat3 = &((ISP_STAT_S *)pStatInfo)->stAwbStat3;
            stAwbInfo.pstAwbStat4 = &((ISP_STAT_S *)pStatInfo)->stAwbStat4;
            
            if (HI_NULL != pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_run)
            {
                s32Ret = pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_run(
                    pstLib->stAlgLib.s32Id, &stAwbInfo, &stAwbResult, 0);
                if (HI_SUCCESS != s32Ret)
                {
                    printf("WARNING!! run awb lib err 0x%x!\n", s32Ret);
                }
            }            
        break;
        
        case ISP_MODE_STITCH:
            if(IS_MAIN_CHANNAL(IspDev))    
            {
                stAwbInfo.pstAwbStat1 = &((ISP_STAT_S *)pStatInfo)->stAwbStat1;
                stAwbInfo.pstAwbStat2 = &((ISP_STAT_S *)pStatInfo)->stAwbStat2;
                stAwbInfo.pstAwbStat3 = &((ISP_STAT_S *)pStatInfo)->stAwbStat3;
                stAwbInfo.pstAwbStat4 = &((ISP_STAT_S *)pStatInfo)->stAwbStat4;
    
                if (HI_NULL != pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_run)
                {
                    s32Ret = pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_run(
                        pstLib->stAlgLib.s32Id, &stAwbInfo, &stAwbResult, 0);
                    if (HI_SUCCESS != s32Ret)
                    {
                        printf("WARNING!! run awb lib err 0x%x!\n", s32Ret);
                    }
                } 
                memcpy(&g_astIspCtx[IspDev].stAwbResult, &stAwbResult, sizeof(ISP_AWB_RESULT_S));
            }
            else
            {
                memcpy(&stAwbResult, &g_astIspCtx[0].stAwbResult, sizeof(ISP_AWB_RESULT_S));
            }
        break;
    }


    pstIspCtx->stLinkage.u32ColorTemp = stAwbResult.u32ColorTemp;
    pstIspCtx->stLinkage.au32WhiteBalanceGain[0] = stAwbResult.au32WhiteBalanceGain[0];
    pstIspCtx->stLinkage.au32WhiteBalanceGain[1] = stAwbResult.au32WhiteBalanceGain[1];
    pstIspCtx->stLinkage.au32WhiteBalanceGain[2] = stAwbResult.au32WhiteBalanceGain[2];
    pstIspCtx->stLinkage.au32WhiteBalanceGain[3] = stAwbResult.au32WhiteBalanceGain[3];
    for (i=0;i<9;i++)
    {
        pstIspCtx->stLinkage.au16CCM[i] = stAwbResult.au16ColorMatrix[i];
    }    

    if (ISP_MODE_STITCH == g_astIspCtx[0].enIspMode)
    {
        HI_S16 as16CalcCCM[9], as16DiffCCM[9];
        HI_S16 as16ResCCM[9] = {0};

        /* TODO: Dual pipe different config WBgain/CCM */  
        for(i = 0; i < 4; i++)
        {
            u32DiffGain = hi_ext_system_isp_pipe_diff_gain_read(IspDev, i);
            stAwbResult.au32WhiteBalanceGain[i] = (stAwbResult.au32WhiteBalanceGain[i] * u32DiffGain) >> 8;
        }
        
        for (i = 0; i < 9; i++)
        {
            as16CalcCCM[i] = AwbDirectToComplement(stAwbResult.au16ColorMatrix[i]);
            as16DiffCCM[i] = hi_ext_system_isp_pipe_diff_ccm_read(IspDev, i);
            as16DiffCCM[i] = AwbDirectToComplement(as16DiffCCM[i]);
        }
        
        AwbMatrixMultiply(as16CalcCCM, as16DiffCCM, as16ResCCM, 3, 3, 3);
        
        for (i = 0; i < 9; i++)
        {
            stAwbResult.au16ColorMatrix[i] = AwbComplementToDirect(as16ResCCM[i]);
        } 
    }

    AwbCfgReg(IspDev, &stAwbResult, pstIspCtx->u8SnsWDRMode, pstIspCtx->stLinkage.u32IspDgain,
        pstIspCtx->stLinkage.u32IspDgainShift, (ISP_REG_CFG_S *)pRegCfg);

    
    if (HI_TRUE == pstIspCtx->stLinkage.bCapState)
    {
        pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->stAlgLib.s32Id,
                    AWB_CCM_CONFIG_SET, (HI_VOID *)&stCCMConfDef);
    }

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

