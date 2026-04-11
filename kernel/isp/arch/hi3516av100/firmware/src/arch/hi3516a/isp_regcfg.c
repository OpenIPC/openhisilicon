/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_regcfg.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/05/07
  Description   : 
  History       :
  1.Date        : 2013/05/07
    Author      : n00168968
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include "mkp_isp.h"
#include "isp_regcfg.h"
#include "isp_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

ISP_REGCFG_S g_astRegCfgCtx[ISP_MAX_DEV_NUM] = {{0}};
#define REGCFG_GET_CTX(dev, pstCtx)   pstCtx = &g_astRegCfgCtx[dev]

extern HI_S32 g_as32IspFd[ISP_MAX_DEV_NUM];

HI_S32 ISP_RegConfigInit(ISP_REG_CFG_S *pstRegCfgInfo)
{
    HI_S32 i, j;

    if (pstRegCfgInfo->unKey.bit1AeCfg1)
    {
        for (i = 0; i < 15; i++)
        {
            for (j = 0; j < 17; j++)
            {
                pstRegCfgInfo->stAeRegCfg1.au8WeightTable[i][j] = hi_isp_zones_aexp_weight_read(i, j);
            }
        }

        pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[0] = hi_isp_metering_hist_thresh_0_1_read();
        pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[1] = hi_isp_metering_hist_thresh_1_2_read();
        pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[2] = hi_isp_metering_hist_thresh_3_4_read();
        pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[3] = hi_isp_metering_hist_thresh_4_5_read();
    }

    if (pstRegCfgInfo->unKey.bit1AeCfg2)
    {
        pstRegCfgInfo->stAeRegCfg2.u32IntTime[0] = 2;
        pstRegCfgInfo->stAeRegCfg2.u32IntTime[1] = 2;
        pstRegCfgInfo->stAeRegCfg2.u32IntTime[2] = 2;
        pstRegCfgInfo->stAeRegCfg2.u32IntTime[3] = 2;
        pstRegCfgInfo->stAeRegCfg2.u32IspDgain = 1;
    }

    if (pstRegCfgInfo->unKey.bit1AwbCfg1)
    {
        pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[0] = hi_isp_matrix_coefft_r_r_read();
        pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[1] = hi_isp_matrix_coefft_r_g_read();
        pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[2] = hi_isp_matrix_coefft_r_b_read();
        pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[3] = hi_isp_matrix_coefft_g_r_read();
        pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[4] = hi_isp_matrix_coefft_g_g_read();
        pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[5] = hi_isp_matrix_coefft_g_b_read();
        pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[6] = hi_isp_matrix_coefft_b_r_read();
        pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[7] = hi_isp_matrix_coefft_b_g_read();
        pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[8] = hi_isp_matrix_coefft_b_b_read();

        pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[0] = hi_isp_white_balance_gain_00_read();
        pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[1] = hi_isp_white_balance_gain_01_read();
        pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[2] = hi_isp_white_balance_gain_10_read();
        pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[3] = hi_isp_white_balance_gain_11_read();
    }

    if (pstRegCfgInfo->unKey.bit1AwbCfg2)
    {
        pstRegCfgInfo->stAwbRegCfg2.u16MeteringCrRefMaxAwb = hi_isp_metering_cr_ref_max_awb_read();
        pstRegCfgInfo->stAwbRegCfg2.u16MeteringCrRefMinAwb = hi_isp_metering_cr_ref_min_awb_read();
        pstRegCfgInfo->stAwbRegCfg2.u16MeteringCbRefMaxAwb = hi_isp_metering_cb_ref_max_awb_read();
        pstRegCfgInfo->stAwbRegCfg2.u16MeteringCbRefMinAwb = hi_isp_metering_cb_ref_min_awb_read();
        pstRegCfgInfo->stAwbRegCfg2.u16MeteringWhiteLevelAwb = hi_isp_metering_white_level_awb_read();
        pstRegCfgInfo->stAwbRegCfg2.u16MeteringBlackLevelAwb = hi_isp_metering_black_level_awb_read();
        pstRegCfgInfo->stAwbRegCfg2.u16MeteringCrRefHighAwb = hi_isp_metering_cr_ref_high_awb_read();
        pstRegCfgInfo->stAwbRegCfg2.u16MeteringCrRefLowAwb = hi_isp_metering_cr_ref_low_awb_read();
        pstRegCfgInfo->stAwbRegCfg2.u16MeteringCbRefHighAwb = hi_isp_metering_cb_ref_high_awb_read();
        pstRegCfgInfo->stAwbRegCfg2.u16MeteringCbRefLowAwb = hi_isp_metering_cb_ref_low_awb_read();
    }

    if (pstRegCfgInfo->unKey.bit1AwbCfg3)
    {
        pstRegCfgInfo->stAwbRegCfg3.bAboveWhiteLevelClip = hi_isp_metering_max_clip_read();
        pstRegCfgInfo->stAwbRegCfg3.bBelowBlackLevelClip = hi_isp_metering_min_clip_read();
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefMaxAwb = hi_isp_metering_cr_ref_max_awb_sum_read();
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefMinAwb = hi_isp_metering_cr_ref_min_awb_sum_read();
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefMaxAwb = hi_isp_metering_cb_ref_max_awb_sum_read();
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefMinAwb = hi_isp_metering_cb_ref_min_awb_sum_read();
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringWhiteLevelAwb = hi_isp_metering_max_threshold_read();
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringBlackLevelAwb = hi_isp_metering_min_threshold_read();
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefHighAwb = hi_isp_metering_cr_ref_high_awb_sum_read();
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefLowAwb = hi_isp_metering_cr_ref_low_awb_sum_read();
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefHighAwb = hi_isp_metering_cb_ref_high_awb_sum_read();
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefLowAwb = hi_isp_metering_cb_ref_low_awb_sum_read();
    }

    if (pstRegCfgInfo->unKey.bit1AfCfg)
    {

    }

    if (pstRegCfgInfo->unKey.bit1OffsetCfg)
    {
        pstRegCfgInfo->stOffsetRegCfg.au16BlackLevelOffset[0] = hi_isp_offset_black_00_read();
        pstRegCfgInfo->stOffsetRegCfg.au16BlackLevelOffset[1] = hi_isp_offset_black_01_read();
        pstRegCfgInfo->stOffsetRegCfg.au16BlackLevelOffset[2] = hi_isp_offset_black_10_read();
        pstRegCfgInfo->stOffsetRegCfg.au16BlackLevelOffset[3] = hi_isp_offset_black_11_read();
    }

    if (pstRegCfgInfo->unKey.bit1DrcCfg)
    {
        pstRegCfgInfo->stDrcRegCfg.u32Strength = hi_isp_drc_strength_read();
    }

    if (pstRegCfgInfo->unKey.bit1NrCfg)
    {
        pstRegCfgInfo->stNrRegCfg.u16NpExpThresh = hi_isp_noise_profile_exp_thresh_read(); 
        pstRegCfgInfo->stNrRegCfg.u8NpShortRatio = hi_isp_noise_profile_short_ratio_read();
        pstRegCfgInfo->stNrRegCfg.u8NpLongRatio = hi_isp_noise_profile_long_ratio_read();

        pstRegCfgInfo->stNrRegCfg.u8SinterThreshShort = hi_isp_nr_thresh_short_read();
        pstRegCfgInfo->stNrRegCfg.u8SinterThreshLong = hi_isp_nr_thresh_long_read();
        pstRegCfgInfo->stNrRegCfg.u8NpOffset = hi_isp_demosaic_np_offset_read();
    }

    if (pstRegCfgInfo->unKey.bit1SharpenCfg)
    {        
        pstRegCfgInfo->stSharpenRegCfg.u16LumThresh = hi_isp_demosaic_lum_thresh_read();  
        pstRegCfgInfo->stSharpenRegCfg.u8SharpAltD = hi_isp_demosaic_sharp_alt_d_read();    
        pstRegCfgInfo->stSharpenRegCfg.u8SharpAltUd = hi_isp_demosaic_sharp_alt_ud_read();
        pstRegCfgInfo->stSharpenRegCfg.u8RGBsharpshrength = hi_isp_sharpen_strength_read();
    }

    if (pstRegCfgInfo->unKey.bit1GeCfg)
    {
        pstRegCfgInfo->stGeRegCfg.u8GeStrength = hi_isp_green_equalize_ge_strength_read();
    }

    if (pstRegCfgInfo->unKey.bit1DpCfg)
    {
        pstRegCfgInfo->stDpRegCfg.u8DpEnable = hi_isp_defect_pixel_enable_read();
        pstRegCfgInfo->stDpRegCfg.u8DpPointerReset = hi_isp_defect_pixel_pointer_reset_read();
        pstRegCfgInfo->stDpRegCfg.u16DPThresh = hi_isp_green_equalize_dp_threshold_read();
        pstRegCfgInfo->stDpRegCfg.u8DpDetectionTrigger = hi_isp_defect_pixel_detection_trigger_read();
        pstRegCfgInfo->stDpRegCfg.u16DpCountIn = hi_isp_defect_pixel_defect_pixel_count_in_read();
        pstRegCfgInfo->stDpRegCfg.u8ShowStaticDefectPixels = hi_isp_defect_pixel_show_static_defect_pixels_read();
        pstRegCfgInfo->stDpRegCfg.u16DpSlope = hi_isp_green_equalize_dp_slope_read();
    }

    if (pstRegCfgInfo->unKey.bit1WdrCfg)
    {
        
    }

    return HI_SUCCESS;
}

HI_S32 ISP_RegConfig(ISP_REG_CFG_S *pstRegCfgInfo)
{
    HI_S32 i, j; 

    if (pstRegCfgInfo->unKey.bit1AeCfg1)
    {
        for (i = 0; i < 15; i++)
        {
            for (j = 0; j < 17; j++)
            {
                hi_isp_zones_aexp_weight_write(i, j, pstRegCfgInfo->stAeRegCfg1.au8WeightTable[i][j]);
            }
        }

        hi_isp_metering_hist_thresh_0_1_write(pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[0]);
        hi_isp_metering_hist_thresh_1_2_write(pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[1]);
        hi_isp_metering_hist_thresh_3_4_write(pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[2]);
        hi_isp_metering_hist_thresh_4_5_write(pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[3]);
    }

    if (pstRegCfgInfo->unKey.bit1AwbCfg1)
    {
        hi_isp_matrix_coefft_r_r_write(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[0]);
        hi_isp_matrix_coefft_r_g_write(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[1]);
        hi_isp_matrix_coefft_r_b_write(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[2]);
        hi_isp_matrix_coefft_g_r_write(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[3]);
        hi_isp_matrix_coefft_g_g_write(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[4]);
        hi_isp_matrix_coefft_g_b_write(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[5]);
        hi_isp_matrix_coefft_b_r_write(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[6]);
        hi_isp_matrix_coefft_b_g_write(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[7]);
        hi_isp_matrix_coefft_b_b_write(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[8]);
        #if 1
        hi_isp_white_balance_gain_00_write(pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[0]);
        hi_isp_white_balance_gain_01_write(pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[1]);
        hi_isp_white_balance_gain_10_write(pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[2]);
        hi_isp_white_balance_gain_11_write(pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[3]);
        #endif
    }

    if (pstRegCfgInfo->unKey.bit1AwbCfg2)
    {
        hi_isp_metering_cr_ref_max_awb_write(pstRegCfgInfo->stAwbRegCfg2.u16MeteringCrRefMaxAwb);
        hi_isp_metering_cr_ref_min_awb_write(pstRegCfgInfo->stAwbRegCfg2.u16MeteringCrRefMinAwb);
        hi_isp_metering_cb_ref_max_awb_write(pstRegCfgInfo->stAwbRegCfg2.u16MeteringCbRefMaxAwb);
        hi_isp_metering_cb_ref_min_awb_write(pstRegCfgInfo->stAwbRegCfg2.u16MeteringCbRefMinAwb);
        hi_isp_metering_white_level_awb_write(pstRegCfgInfo->stAwbRegCfg2.u16MeteringWhiteLevelAwb);
        hi_isp_metering_black_level_awb_write(pstRegCfgInfo->stAwbRegCfg2.u16MeteringBlackLevelAwb);
        hi_isp_metering_cr_ref_high_awb_write(pstRegCfgInfo->stAwbRegCfg2.u16MeteringCrRefHighAwb);
        hi_isp_metering_cr_ref_low_awb_write(pstRegCfgInfo->stAwbRegCfg2.u16MeteringCrRefLowAwb);
        hi_isp_metering_cb_ref_high_awb_write(pstRegCfgInfo->stAwbRegCfg2.u16MeteringCbRefHighAwb);
        hi_isp_metering_cb_ref_low_awb_write(pstRegCfgInfo->stAwbRegCfg2.u16MeteringCbRefLowAwb);
    }

    if (pstRegCfgInfo->unKey.bit1AwbCfg3)
    {
        hi_isp_metering_max_clip_write(pstRegCfgInfo->stAwbRegCfg3.bAboveWhiteLevelClip);
        hi_isp_metering_min_clip_write(pstRegCfgInfo->stAwbRegCfg3.bBelowBlackLevelClip);
        hi_isp_metering_cr_ref_max_awb_sum_write(pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefMaxAwb);
        hi_isp_metering_cr_ref_min_awb_sum_write(pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefMinAwb);
        hi_isp_metering_cb_ref_max_awb_sum_write(pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefMaxAwb);
        hi_isp_metering_cb_ref_min_awb_sum_write(pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefMinAwb);
        hi_isp_metering_max_threshold_write(pstRegCfgInfo->stAwbRegCfg3.u16MeteringWhiteLevelAwb);
        hi_isp_metering_min_threshold_write(pstRegCfgInfo->stAwbRegCfg3.u16MeteringBlackLevelAwb);
        hi_isp_metering_cr_ref_high_awb_sum_write(pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefHighAwb);
        hi_isp_metering_cr_ref_low_awb_sum_write(pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefLowAwb);
        hi_isp_metering_cb_ref_high_awb_sum_write(pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefHighAwb);
        hi_isp_metering_cb_ref_low_awb_sum_write(pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefLowAwb);
    }
    
    if (pstRegCfgInfo->unKey.bit1AfCfg)
    {

    }

    if (pstRegCfgInfo->unKey.bit1OffsetCfg)
    {
        /* config in isp_black_level.c */
    }

    if (pstRegCfgInfo->unKey.bit1DrcCfg)
    {
        //hi_isp_drc_strength_write(pstRegCfgInfo->stDrcRegCfg.u32Strength);
    }

    if (pstRegCfgInfo->unKey.bit1NrCfg)
    {
        hi_isp_noise_profile_exp_thresh_write(pstRegCfgInfo->stNrRegCfg.u16NpExpThresh); 
        hi_isp_noise_profile_short_ratio_write(pstRegCfgInfo->stNrRegCfg.u8NpShortRatio);
        hi_isp_noise_profile_long_ratio_write(pstRegCfgInfo->stNrRegCfg.u8NpLongRatio);

        hi_isp_nr_thresh_short_write(pstRegCfgInfo->stNrRegCfg.u8SinterThreshShort);
        hi_isp_nr_thresh_long_write(pstRegCfgInfo->stNrRegCfg.u8SinterThreshLong);
        hi_isp_demosaic_np_offset_write(pstRegCfgInfo->stNrRegCfg.u8NpOffset);
    }

    if (pstRegCfgInfo->unKey.bit1SharpenCfg)
    {        
        hi_isp_demosaic_lum_thresh_write(pstRegCfgInfo->stSharpenRegCfg.u16LumThresh);  
        hi_isp_demosaic_sharp_alt_d_write(pstRegCfgInfo->stSharpenRegCfg.u8SharpAltD);    
        hi_isp_demosaic_sharp_alt_ud_write(pstRegCfgInfo->stSharpenRegCfg.u8SharpAltUd);  
        hi_isp_sharpen_strength_write(pstRegCfgInfo->stSharpenRegCfg.u8RGBsharpshrength);
    }

    if (pstRegCfgInfo->unKey.bit1GeCfg)
    {
        hi_isp_green_equalize_ge_strength_write(pstRegCfgInfo->stGeRegCfg.u8GeStrength);
    }

    if (pstRegCfgInfo->unKey.bit1DpCfg)
    {
        hi_isp_defect_pixel_enable_write(pstRegCfgInfo->stDpRegCfg.u8DpEnable);
        hi_isp_defect_pixel_pointer_reset_write(pstRegCfgInfo->stDpRegCfg.u8DpPointerReset);
        hi_isp_green_equalize_dp_threshold_write(pstRegCfgInfo->stDpRegCfg.u16DPThresh);
        hi_isp_defect_pixel_detection_trigger_write(pstRegCfgInfo->stDpRegCfg.u8DpDetectionTrigger);
        hi_isp_defect_pixel_defect_pixel_count_in_write(pstRegCfgInfo->stDpRegCfg.u16DpCountIn);
        hi_isp_defect_pixel_show_static_defect_pixels_write(pstRegCfgInfo->stDpRegCfg.u8ShowStaticDefectPixels);
        hi_isp_green_equalize_dp_slope_write(pstRegCfgInfo->stDpRegCfg.u16DpSlope);
        //hi_isp_green_equalize_dp_threshold_write(pstRegCfgInfo->stDpRegCfg.u16DynamicDPThresh);
    }

    if (pstRegCfgInfo->unKey.bit1DehazeCfg)
    {
        if (pstRegCfgInfo->stDehazeRegCfg.u8DehazeEnable)
        {
            hi_vi_dehaze_air_write(pstRegCfgInfo->stDehazeRegCfg.u32DehazeAir);
        }

        hi_vi_dehaze_gstrth_write(pstRegCfgInfo->stDehazeRegCfg.u8Strength);
        hi_vi_dehaze_update_write(pstRegCfgInfo->stDehazeRegCfg.u32Update);
    }
    
    if (pstRegCfgInfo->unKey.bit1WdrCfg)
    {
        
    }

    return HI_SUCCESS;
}

HI_S32 ISP_RegCfgInit(ISP_DEV IspDev, HI_VOID **ppCfg)
{
    ISP_REGCFG_S *pstRegCfg = HI_NULL;

    REGCFG_GET_CTX(IspDev, pstRegCfg);

    if (!pstRegCfg->bInit)
    {
        pstRegCfg->stRegCfg.unKey.u32Key = 0x3FFF;

        ISP_RegConfigInit(&pstRegCfg->stRegCfg);

        pstRegCfg->bInit = HI_TRUE;
    }
    
    pstRegCfg->stRegCfg.unKey.u32Key = 0;
    
    *ppCfg = &pstRegCfg->stRegCfg;
    
    return HI_SUCCESS;
}

HI_S32 ISP_RegCfgSet(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_REGCFG_S *pstRegCfg = HI_NULL;

    REGCFG_GET_CTX(IspDev, pstRegCfg);

    ISP_RegConfig(&pstRegCfg->stRegCfg);

    if (pstRegCfg->stRegCfg.stKernelCfg.unKey.u32Key)
    {
        s32Ret = ioctl(g_as32IspFd[IspDev], ISP_REG_CFG_SET, &pstRegCfg->stRegCfg.stKernelCfg);
        if (HI_SUCCESS != s32Ret)
        {
            printf("Config ISP register Failed with ec %#x!\n", s32Ret);
            return s32Ret;
        }
    }

    return HI_SUCCESS;
}

#define ISP_DATA_REVERSE(value, bytenum)\
do{\
    HI_S32 j, s32Revs = 0;\
    for(j=0;j<(bytenum*8);j++)\
    {\
        s32Revs = s32Revs + ((((value)>>j)&(0x01))<<((bytenum*8-1)-j));\
    }\
    value = s32Revs;\
}while(0)

HI_VOID ISP_SnsRegsInfoCheck(ISP_DEV IspDev, ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
    //if ((pstSnsRegsInfo->enSnsType >= ISP_SNS_TYPE_BUTT)
    //    || (pstSnsRegsInfo->enSnsType < ISP_SNS_I2C_TYPE))
    if (pstSnsRegsInfo->enSnsType >= ISP_SNS_TYPE_BUTT)
    {
        printf("senor's regs info invalid, enSnsType %d\n", pstSnsRegsInfo->enSnsType);
        return;
    }

    if (pstSnsRegsInfo->u32RegNum > ISP_MAX_SNS_REGS)
    {
        printf("senor's regs info invalid, u32RegNum %d\n", pstSnsRegsInfo->u32RegNum);
        return;
    }

    return;
}

HI_S32 ISP_SyncCfgSet(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_REGCFG_S *pstRegCfg = HI_NULL;
    ISP_SNS_REGS_INFO_S *pstSnsRegsInfo = NULL;

    REGCFG_GET_CTX(IspDev, pstRegCfg);

    if (HI_SUCCESS != ISP_SensorUpdateSnsReg(IspDev))
    {
        /* If Users need to config AE sync info themselves, they can set pfn_cmos_get_sns_reg_info to NULL in cmos.c */
        /* Then there will be NO AE sync configs in kernel of firmware */
        return HI_SUCCESS;
    }
    ISP_SensorGetSnsReg(IspDev, &pstSnsRegsInfo);
    memcpy(&pstRegCfg->stSyncCfgNode.stSnsRegsInfo, pstSnsRegsInfo, sizeof(ISP_SNS_REGS_INFO_S));
    ISP_SnsRegsInfoCheck(IspDev, &pstRegCfg->stSyncCfgNode.stSnsRegsInfo);

    memcpy(&pstRegCfg->stSyncCfgNode.stAERegCfg, &pstRegCfg->stRegCfg.stAeRegCfg2, sizeof(ISP_AE_REG_CFG_2_S));
    memcpy(&pstRegCfg->stSyncCfgNode.stDRCRegCfg, &pstRegCfg->stRegCfg.stDrcRegCfg, sizeof(ISP_DRC_REG_CFG_S));
    pstRegCfg->stSyncCfgNode.bValid = HI_TRUE;

    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_SYNC_CFG_SET, &pstRegCfg->stSyncCfgNode);
    if (HI_SUCCESS != s32Ret)
    {
        //printf("Config Sync register Failed with ec %#x!\n", s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

