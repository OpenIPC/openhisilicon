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

HI_S32 ISP_RegConfigInit(ISP_DEV IspDev, ISP_REG_CFG_S *pstRegCfgInfo)
{
    HI_S32 i, j;
    HI_U32 u32CombinWeiNum = 0;
    HI_U32 u32CombinWei = 0;

    if (pstRegCfgInfo->unKey.bit1AeCfg1)
    {
        /*modify by qlp*/
        hi_isp_ae_mem_wei_waddr_write(IspDev, 0);

        for (i = 0; i < 15; i++)
        {
            for (j = 0; j < 17; j++)
            {
                if (0 == u32CombinWeiNum)
                {
                    u32CombinWei = hi_isp_ae_mem_wei_wdata_read(IspDev);
                }

                pstRegCfgInfo->stAeRegCfg1.au8WeightTable[i][j] = ((u32CombinWei >> (8*u32CombinWeiNum)) & 0xff);

                u32CombinWeiNum++;

                if (HI_ISP_AE_WEI_COMBIN_COUNT == u32CombinWeiNum)
                {
                    u32CombinWeiNum = 0;
                }
            }
        }

        //pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[0] = hi_isp_metering_hist_thresh_0_1_read(IspDev);
        //pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[1] = hi_isp_metering_hist_thresh_1_2_read(IspDev);
        //pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[2] = hi_isp_metering_hist_thresh_3_4_read(IspDev);
        //pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[3] = hi_isp_metering_hist_thresh_4_5_read(IspDev);
        pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[0] = 0;
        pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[1] = 0;
        pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[2] = 0;
        pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[3] = 0;
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
        pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[0] = hi_isp_matrix_coefft_r_r_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[1] = hi_isp_matrix_coefft_r_g_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[2] = hi_isp_matrix_coefft_r_b_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[3] = hi_isp_matrix_coefft_g_r_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[4] = hi_isp_matrix_coefft_g_g_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[5] = hi_isp_matrix_coefft_g_b_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[6] = hi_isp_matrix_coefft_b_r_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[7] = hi_isp_matrix_coefft_b_g_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[8] = hi_isp_matrix_coefft_b_b_read(IspDev);

        pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[0] = hi_isp_white_balance_gain_r_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[1] = hi_isp_white_balance_gain_gr_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[2] = hi_isp_white_balance_gain_gb_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[3] = hi_isp_white_balance_gain_b_read(IspDev);
    }

    if (pstRegCfgInfo->unKey.bit1AwbCfg3)
    {
        //pstRegCfgInfo->stAwbRegCfg3.bAboveWhiteLevelClip = hi_isp_metering_max_clip_read();
        //pstRegCfgInfo->stAwbRegCfg3.bBelowBlackLevelClip = hi_isp_metering_min_clip_read();
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefMaxAwb = hi_isp_metering_cr_ref_max_awb_sum_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefMinAwb = hi_isp_metering_cr_ref_min_awb_sum_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefMaxAwb = hi_isp_metering_cb_ref_max_awb_sum_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefMinAwb = hi_isp_metering_cb_ref_min_awb_sum_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringWhiteLevelAwb = hi_isp_metering_max_threshold_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringBlackLevelAwb = hi_isp_metering_min_threshold_read(IspDev);
    }

    if (pstRegCfgInfo->unKey.bit1AfCfg)
    {
        //pstRegCfgInfo->stAfRegCfg.u16ThresholdWrite = hi_isp_metering_af_threshold_write_read();
        //pstRegCfgInfo->stAfRegCfg.u8MetricsShift = hi_isp_metering_af_metrics_shift_read();
        //pstRegCfgInfo->stAfRegCfg.u8NpOffset = hi_isp_metering_af_np_offset_read();
    }

    if (pstRegCfgInfo->unKey.bit1OffsetCfg)
    {
        //pstRegCfgInfo->stOffsetRegCfg.au16BlackLevelOffset[0] = (hi_isp_offset_black_00_read() >> 2);
        //pstRegCfgInfo->stOffsetRegCfg.au16BlackLevelOffset[1] = (hi_isp_offset_black_01_read() >> 2);
        //pstRegCfgInfo->stOffsetRegCfg.au16BlackLevelOffset[2] = (hi_isp_offset_black_10_read() >> 2);
        //pstRegCfgInfo->stOffsetRegCfg.au16BlackLevelOffset[3] = (hi_isp_offset_black_11_read() >> 2);
    }

    if (pstRegCfgInfo->unKey.bit1DrcCfg)
    {
        //pstRegCfgInfo->stDrcRegCfg.u8Strength = hi_isp_drc_strength_read();
    }

    if (pstRegCfgInfo->unKey.bit1SharpenCfg)
    {
        HI_U32 *pu32lumaWgt;
        //modify by qlp
        pstRegCfgInfo->stSharpenRegCfg.u8PixSel = hi_isp_sharpen_pixsel_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u16SharpAmt = hi_isp_sharp_amt_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u16EdgeAmt = hi_isp_edge_amt_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u16SharpThd1 = hi_isp_sharp_thd1_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u16SharpThd2 = hi_isp_sharp_thd2_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u16SharpThd1MulCoef = hi_isp_sharp_coef_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u16EdgeThd1 = hi_isp_edge_thd1_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u16EdgeThd2 = hi_isp_edge_thd2_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u16EdgeThd1MulCoef = hi_isp_edge_coef_read(IspDev);
        //pstRegCfgInfo->stSharpenRegCfg.u8LineThd1 = hi_isp_sharpen_line_thd1_read(IspDev);
        //pstRegCfgInfo->stSharpenRegCfg.u8LineThd2 = hi_isp_sharpen_line_thd2_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8OvershootAmt = hi_isp_over_amt_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8UndershootAmt = hi_isp_under_amt_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.s8ShtMaxShft = hi_isp_sharpen_maxshift_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.s8ShtMinShft = hi_isp_sharpen_minshift_read(IspDev);

        //juactrl
        pstRegCfgInfo->stSharpenRegCfg.bEnJagCtrl = hi_isp_sharpen_jagctrl_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u16VarJagThd1 = hi_isp_sharpen_varjagthd1_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u16VarJagThd2 = hi_isp_sharpen_varjagthd2_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u16EdgeJagAmt = hi_isp_sharpen_edgejagamt_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.s16EdgeJagMulCoef = hi_isp_sharpen_edgejagmulcoef_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8OshtJagAmt = hi_isp_sharpen_oshtjagamt_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.s16OshtJagMulCoef = hi_isp_sharpen_oshtjagmulcoef_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8UshtJagAmt = hi_isp_sharpen_ushtjagamt_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.s16UshtJagMulCoef = hi_isp_sharpen_ushtjagmulcoef_read(IspDev);

        pstRegCfgInfo->stSharpenRegCfg.bEnLumaCtrlNoise = hi_isp_sharpen_lumactrlnoise_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.bEnSkinCtrl = hi_isp_sharpen_skinctrl_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8dirRlyShft = hi_isp_sharpen_dirrlyshft_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8lumNoSt = hi_isp_sharpen_luma_nost_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8Limit = hi_isp_sharpen_luma_limit_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.au16OShtVarTh[0] = hi_isp_sharpen_overvarth0_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.au16OShtVarTh[1] = hi_isp_sharpen_overvarth1_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.au16UShtVarTh[0] = hi_isp_sharpen_undervarth0_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.au16UShtVarTh[1] = hi_isp_sharpen_undervarth1_read(IspDev);

        pstRegCfgInfo->stSharpenRegCfg.u8skinCntMulCoef = hi_isp_sharpen_skincntmulcoef_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8skin_cnt_th2 = hi_isp_sharpen_skin_cnt_th2_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8skin_cnt_th1 = hi_isp_sharpen_skin_cnt_th1_read(IspDev);

        pstRegCfgInfo->stSharpenRegCfg.u16skin_var_th2 = hi_isp_sharpen_skin_var_th2_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u16skin_var_th1 = hi_isp_sharpen_skin_var_th1_read(IspDev);

        pstRegCfgInfo->stSharpenRegCfg.s16skinVarWgtMulCoef = hi_isp_sharpen_skinvarwgtmulcoef_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8skin_var_wgt2 = hi_isp_sharpen_skin_var_wgt2_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8skin_var_wgt1 = hi_isp_sharpen_skin_var_wgt1_read(IspDev);

        pstRegCfgInfo->stSharpenRegCfg.u8skin_u_max = hi_isp_sharpen_skin_u_max_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8skin_u_min = hi_isp_sharpen_skin_u_min_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8skin_v_max = hi_isp_sharpen_skin_v_max_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8skin_v_min = hi_isp_sharpen_skin_v_min_read(IspDev);

        pstRegCfgInfo->stSharpenRegCfg.u8skin_len = hi_isp_sharpen_skin_len_read(IspDev);

        // luma for blending
        pstRegCfgInfo->stSharpenRegCfg.bEnLumaCtrl = hi_isp_sharpen_lumactrl_read(IspDev);
        pu32lumaWgt = (HI_U32*)&pstRegCfgInfo->stSharpenRegCfg.u8lumaWgt[0];
        for ( i = 0 ; i < ISP_YUV_SHPLUMA_NUM/sizeof(HI_U32); i++)
        {
            *pu32lumaWgt = hi_isp_sharpen_lumawgt_read(IspDev, i);
        }

        // var for shoot ctrl
        pstRegCfgInfo->stSharpenRegCfg.bEnShtCtrlVarMod = hi_isp_sharpen_shtvarctrl_read (IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8OShtVarAmt = hi_isp_sharpen_overvaramt_read (IspDev);
        pstRegCfgInfo->stSharpenRegCfg.s16OShtVarMul= hi_isp_sharpen_overvarmul_read (IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8UShtVarAmt = hi_isp_sharpen_undervaramt_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.s16UShtVarMul= hi_isp_sharpen_undervarmul_read(IspDev);

        pstRegCfgInfo->stSharpenRegCfg.G3[0][0] = hi_isp_sharpen_mid_tmp00_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.G3[0][1] = hi_isp_sharpen_mid_tmp01_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.G3[0][2] = hi_isp_sharpen_mid_tmp02_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.G3[1][0] = hi_isp_sharpen_mid_tmp10_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.G3[1][1] = hi_isp_sharpen_mid_tmp11_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.G3[1][2] = hi_isp_sharpen_mid_tmp12_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.G3[2][0] = hi_isp_sharpen_mid_tmp20_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.G3[2][1] = hi_isp_sharpen_mid_tmp21_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.G3[2][2] = hi_isp_sharpen_mid_tmp22_read(IspDev);
    }

    if (pstRegCfgInfo->unKey.bit1GeCfg)
    {
        //pstRegCfgInfo->stGeRegCfg.u8GeStrength = hi_isp_green_equalize_ge_strength_read();
    }

    if (pstRegCfgInfo->unKey.bit1DpCfg)
    {
#if 0
        pstRegCfgInfo->stDpRegCfg.u8DpEnable = hi_isp_defect_pixel_enable_read();
        pstRegCfgInfo->stDpRegCfg.u8GeDpEnable = hi_isp_green_equalize_dp_enable_read();
        pstRegCfgInfo->stDpRegCfg.u8DpPointerReset = hi_isp_defect_pixel_pointer_reset_read();
        pstRegCfgInfo->stDpRegCfg.u8BadPixelThreshold = hi_isp_green_equalize_dp_threshold_read();
        pstRegCfgInfo->stDpRegCfg.u8DpDetectionTrigger = hi_isp_defect_pixel_detection_trigger_read();
        pstRegCfgInfo->stDpRegCfg.u16DpCountIn = hi_isp_defect_pixel_defect_pixel_count_in_read();
        pstRegCfgInfo->stDpRegCfg.u8ShowStaticDefectPixels = hi_isp_defect_pixel_show_static_defect_pixels_read();
        pstRegCfgInfo->stDpRegCfg.u16DynamicDpSlope = hi_isp_green_equalize_dp_slope_read();
        pstRegCfgInfo->stDpRegCfg.u16DynamicDPThresh = hi_isp_green_equalize_dp_threshold_read();
#endif
    }

    if (pstRegCfgInfo->unKey.bit1WdrCfg)
    {
        //pstRegCfgInfo->stWdrRegCfg.u8TopChannelSwitch = hi_vi_top_channel_switch_read();
    }

    return HI_SUCCESS;
}

HI_S32 ISP_RegConfig(ISP_DEV IspDev, ISP_REG_CFG_S *pstRegCfgInfo)
{
    HI_S32 i, j;
    HI_U32 u32CombinWeight = 0;
    HI_U32 u32CombinWeightNum = 0;

    if (pstRegCfgInfo->unKey.bit1AeCfg1)
    {
        /*added by qlp*/
        hi_isp_ae_mem_wei_waddr_write(IspDev, 0);
        hi_isp_ae_wdr_wei_waddr_write(IspDev, 0, 0);
        hi_isp_ae_wdr_wei_waddr_write(IspDev, 1, 0);
        hi_isp_ae_wdr_wei_waddr_write(IspDev, 2, 0);
        hi_isp_ae_wdr_wei_waddr_write(IspDev, 3, 0);

        for (i=0; i<15; i++)
        {
            for (j=0; j<17; j++)
            {
                u32CombinWeight |= (pstRegCfgInfo->stAeRegCfg1.au8WeightTable[i][j] << (8*u32CombinWeightNum));
                u32CombinWeightNum++;


                if (u32CombinWeightNum == HI_ISP_AE_WEI_COMBIN_COUNT)
                {
                    hi_isp_ae_mem_wei_wdata_write(IspDev, u32CombinWeight);
                    hi_isp_ae_wdr_wei_wdata_write(IspDev, 0, u32CombinWeight);
                    hi_isp_ae_wdr_wei_wdata_write(IspDev, 1, u32CombinWeight);
                    hi_isp_ae_wdr_wei_wdata_write(IspDev, 2, u32CombinWeight);
                    hi_isp_ae_wdr_wei_wdata_write(IspDev, 3, u32CombinWeight);
                    u32CombinWeightNum = 0;
                    u32CombinWeight = 0;
                }
            }
        }

        if (u32CombinWeightNum != HI_ISP_AE_WEI_COMBIN_COUNT
            &&u32CombinWeightNum != 0)
        {
            hi_isp_ae_mem_wei_wdata_write(IspDev, u32CombinWeight);
            hi_isp_ae_wdr_wei_wdata_write(IspDev, 0, u32CombinWeight);
            hi_isp_ae_wdr_wei_wdata_write(IspDev, 1, u32CombinWeight);
            hi_isp_ae_wdr_wei_wdata_write(IspDev, 2, u32CombinWeight);
            hi_isp_ae_wdr_wei_wdata_write(IspDev, 3, u32CombinWeight);
        }
        //printk("####u32WriteNum == %d####\n", u32WriteNum);
    }

    if (pstRegCfgInfo->unKey.bit1AwbCfg1)
    {
        hi_isp_matrix_coefft_r_r_write(IspDev, CCM_CONVERT(CCM_CONVERT_PRE(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[0])));
        hi_isp_matrix_coefft_r_g_write(IspDev, CCM_CONVERT(CCM_CONVERT_PRE(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[1])));
        hi_isp_matrix_coefft_r_b_write(IspDev, CCM_CONVERT(CCM_CONVERT_PRE(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[2])));
        hi_isp_matrix_coefft_g_r_write(IspDev, CCM_CONVERT(CCM_CONVERT_PRE(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[3])));
        hi_isp_matrix_coefft_g_g_write(IspDev, CCM_CONVERT(CCM_CONVERT_PRE(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[4])));
        hi_isp_matrix_coefft_g_b_write(IspDev, CCM_CONVERT(CCM_CONVERT_PRE(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[5])));
        hi_isp_matrix_coefft_b_r_write(IspDev, CCM_CONVERT(CCM_CONVERT_PRE(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[6])));
        hi_isp_matrix_coefft_b_g_write(IspDev, CCM_CONVERT(CCM_CONVERT_PRE(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[7])));
        hi_isp_matrix_coefft_b_b_write(IspDev, CCM_CONVERT(CCM_CONVERT_PRE(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[8])));

        hi_isp_white_balance_gain_r_write(IspDev, pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[0]);
        hi_isp_white_balance_gain_gr_write(IspDev, pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[1]);
        hi_isp_white_balance_gain_gb_write(IspDev, pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[2]);
        hi_isp_white_balance_gain_b_write(IspDev, pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[3]);

        hi_isp_cc_prot_en_write(IspDev,pstRegCfgInfo->stAwbRegCfg1.stProDarkRegion.bCcmProtEn);
        hi_isp_cc_brit_thres0_write (IspDev, pstRegCfgInfo->stAwbRegCfg1.stProDarkRegion.s16ProtBritThres0);
        hi_isp_cc_brit_thres1_write (IspDev, pstRegCfgInfo->stAwbRegCfg1.stProDarkRegion.s16ProtBritThres1);
        hi_isp_cc_color_thres0_write(IspDev, pstRegCfgInfo->stAwbRegCfg1.stProDarkRegion.s16ProtColorThres0);
        hi_isp_cc_color_thres1_write(IspDev, pstRegCfgInfo->stAwbRegCfg1.stProDarkRegion.s16ProtColorThres1);
        hi_isp_cc_low_ratio_write   (IspDev, pstRegCfgInfo->stAwbRegCfg1.stProDarkRegion.s16ProtLowRatio);
        hi_isp_cc_color_ratio_write (IspDev, pstRegCfgInfo->stAwbRegCfg1.stProDarkRegion.s16ProtColorRatio);
        hi_isp_cc_brit_ratio_write  (IspDev, pstRegCfgInfo->stAwbRegCfg1.stProDarkRegion.s16ProtBritRatio);
        hi_isp_cc_max_rgb_thd_write(IspDev, pstRegCfgInfo->stAwbRegCfg1.stProDarkRegion.s16MaxRGBThd);
        hi_isp_cc_prot_ext_en_write(IspDev, pstRegCfgInfo->stAwbRegCfg1.stProDarkRegion.bCcmProtExtEn);
    }

    if (pstRegCfgInfo->unKey.bit1AwbCfg3)
    {
        //hi_isp_metering_max_clip_write(pstRegCfgInfo->stAwbRegCfg3.bAboveWhiteLevelClip);
        //hi_isp_metering_min_clip_write(pstRegCfgInfo->stAwbRegCfg3.bBelowBlackLevelClip);
        hi_isp_metering_cr_ref_max_awb_sum_write(IspDev, pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefMaxAwb);
        hi_isp_metering_cr_ref_min_awb_sum_write(IspDev, pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefMinAwb);
        hi_isp_metering_cb_ref_max_awb_sum_write(IspDev, pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefMaxAwb);
        hi_isp_metering_cb_ref_min_awb_sum_write(IspDev, pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefMinAwb);
        hi_isp_metering_max_threshold_write(IspDev, pstRegCfgInfo->stAwbRegCfg3.u16MeteringWhiteLevelAwb);
        hi_isp_metering_min_threshold_write(IspDev, pstRegCfgInfo->stAwbRegCfg3.u16MeteringBlackLevelAwb);
        hi_isp_awb_top_hypotenuse_k_write(IspDev, -4096);
        hi_isp_awb_top_hypotenuse_b_write(IspDev, 0x400);
        hi_isp_awb_bot_hypotenuse_k_write(IspDev, -4096);
        hi_isp_awb_bot_hypotenuse_b_write(IspDev, 0x40);
    }

    if (pstRegCfgInfo->unKey.bit1AfCfg)
    {
        //hi_isp_metering_af_threshold_write_write(pstRegCfgInfo->stAfRegCfg.u16ThresholdWrite);
        //hi_isp_metering_af_metrics_shift_write(pstRegCfgInfo->stAfRegCfg.u8MetricsShift);
        //hi_isp_metering_af_np_offset_write(pstRegCfgInfo->stAfRegCfg.u8NpOffset);
    }

    if (pstRegCfgInfo->unKey.bit1OffsetCfg)
    {
        /* config in isp_black_level.c */
    }

    if (pstRegCfgInfo->unKey.bit1DrcCfg)
    {
        hi_isp_drc_mixing_coring_write(IspDev, pstRegCfgInfo->stDrcRegCfg.u8MixingCoring);

        hi_isp_drc_mixing_dark_min_write(IspDev, pstRegCfgInfo->stDrcRegCfg.u8MixingDarkMin);
        hi_isp_drc_mixing_dark_max_write(IspDev, pstRegCfgInfo->stDrcRegCfg.u8MixingDarkMax);

        hi_isp_drc_mixing_bright_min_write(IspDev, pstRegCfgInfo->stDrcRegCfg.u8MixingBrightMin);
        hi_isp_drc_mixing_bright_max_write(IspDev, pstRegCfgInfo->stDrcRegCfg.u8MixingBrightMax);
        hi_isp_drc_pdw_hig_thr_write(IspDev, pstRegCfgInfo->stDrcRegCfg.u8PdwHighStr);

        hi_isp_drc_var_rng_fine_write(IspDev, pstRegCfgInfo->stDrcRegCfg.u8RangeVarFine);
        hi_isp_drc_var_spa_fine_write(IspDev, pstRegCfgInfo->stDrcRegCfg.u8SpatialVarFine);
        hi_isp_drc_var_rng_medium_write(IspDev, pstRegCfgInfo->stDrcRegCfg.u8RangeVarMedium);
        hi_isp_drc_var_spa_medium_write(IspDev, pstRegCfgInfo->stDrcRegCfg.u8SpatialVarMedium);
        hi_isp_drc_var_rng_coarse_write(IspDev, pstRegCfgInfo->stDrcRegCfg.u8RangeVarCoarse);
        hi_isp_drc_var_sap_coarse_write(IspDev, pstRegCfgInfo->stDrcRegCfg.u8SpatialVarCoarse);
        hi_isp_drc_bin_mix_medium_write(IspDev,pstRegCfgInfo->stDrcRegCfg.u8FltScaleCoarse);
        hi_isp_drc_bin_mix_coarse_write(IspDev,pstRegCfgInfo->stDrcRegCfg.u8FltScaleFine);
    }

//    if (pstRegCfgInfo->unKey.bit1NrCfg)
//    {
//        //hi_isp_2dnr_luma_gain_write(pstRegCfgInfo->stNrRegCfg.NrLumaNrGain);
//      hi_isp_2dnr_fcr_en_write(IspDev, pstRegCfgInfo->stNrRegCfg.falseColorEnable);
//      hi_isp_2dnr_fcrshift_write(IspDev, pstRegCfgInfo->stNrRegCfg.falseColorShift);
//      hi_isp_2dnr_fcrgain_write(IspDev, pstRegCfgInfo->stNrRegCfg.falseColorGain);
//      hi_isp_2dnr_fcrlimit_write(IspDev, pstRegCfgInfo->stNrRegCfg.falseColorLimit);
//
//      hi_isp_2dnr_luma_low_write(IspDev, pstRegCfgInfo->stNrRegCfg.rangeLumaLow);
//      hi_isp_2dnr_luma_high_write(IspDev, pstRegCfgInfo->stNrRegCfg.rangeLumaHig);
//
//      hi_isp_2dnr_limit_low_write(IspDev, 256-pstRegCfgInfo->stNrRegCfg.blendLowLimit);
//      hi_isp_2dnr_limit_high_write(IspDev, 256-pstRegCfgInfo->stNrRegCfg.blendHigLimit);
//
//      hi_isp_2dnr_nodir_gain_write(IspDev, pstRegCfgInfo->stNrRegCfg.u8NrNoDirGain);
//
//      for (i=0; i<17; i++)
//      {
//          hi_isp_2dnr_thres_low_write(IspDev, i, pstRegCfgInfo->stNrRegCfg.rangeLowLUT[i]);
//          hi_isp_2dnr_thres_high_write(IspDev, i, pstRegCfgInfo->stNrRegCfg.rangeHigLUT[i]);
//      }
//    }

    if (pstRegCfgInfo->unKey.bit1SharpenCfg)
    {
        HI_U32 *pu32lumaWgt;
        hi_isp_sharpen_pixsel_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8PixSel);
        hi_isp_sharp_amt_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16SharpAmt);
        hi_isp_edge_amt_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16EdgeAmt);
        hi_isp_sharp_thd1_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16SharpThd1);
        hi_isp_sharp_thd2_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16SharpThd2);
        hi_isp_sharp_coef_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16SharpThd1MulCoef);
        hi_isp_edge_thd1_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16EdgeThd1);
        hi_isp_edge_thd2_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16EdgeThd2);
        hi_isp_edge_coef_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16EdgeThd1MulCoef);
        //hi_isp_sharpen_line_thd1_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8LineThd1);
        //hi_isp_sharpen_line_thd2_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8LineThd2);
        hi_isp_over_amt_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8OvershootAmt);
        hi_isp_under_amt_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8UndershootAmt);
        hi_isp_sharpen_maxshift_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.s8ShtMaxShft);
        hi_isp_sharpen_minshift_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.s8ShtMinShft);

        //juactrl
        hi_isp_sharpen_jagctrl_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.bEnJagCtrl);
        hi_isp_sharpen_varjagthd1_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16VarJagThd1);
        hi_isp_sharpen_varjagthd2_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16VarJagThd2);
        hi_isp_sharpen_edgejagamt_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16EdgeJagAmt);
        hi_isp_sharpen_edgejagmulcoef_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.s16EdgeJagMulCoef);
        hi_isp_sharpen_oshtjagamt_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8OshtJagAmt);
        hi_isp_sharpen_oshtjagmulcoef_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.s16OshtJagMulCoef);
        hi_isp_sharpen_ushtjagamt_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8UshtJagAmt);
        hi_isp_sharpen_ushtjagmulcoef_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.s16UshtJagMulCoef);

        hi_isp_sharpen_lumactrlnoise_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.bEnLumaCtrlNoise);
        hi_isp_sharpen_skinctrl_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.bEnSkinCtrl);
        hi_isp_sharpen_dirrlyshft_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8dirRlyShft);
        hi_isp_sharpen_luma_nost_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8lumNoSt);
        hi_isp_sharpen_luma_limit_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8Limit);
        hi_isp_sharpen_overvarth0_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.au16OShtVarTh[0]);
        hi_isp_sharpen_overvarth1_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.au16OShtVarTh[1]);
        hi_isp_sharpen_undervarth0_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.au16UShtVarTh[0]);
        hi_isp_sharpen_undervarth1_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.au16UShtVarTh[1]);

        hi_isp_sharpen_skincntmulcoef_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8skinCntMulCoef);
        hi_isp_sharpen_skin_cnt_th2_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8skin_cnt_th2);
        hi_isp_sharpen_skin_cnt_th1_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8skin_cnt_th1);

        hi_isp_sharpen_skin_var_th2_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16skin_var_th2);
        hi_isp_sharpen_skin_var_th1_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16skin_var_th1);

        hi_isp_sharpen_skinvarwgtmulcoef_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.s16skinVarWgtMulCoef);
        hi_isp_sharpen_skin_var_wgt2_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8skin_var_wgt2);
        hi_isp_sharpen_skin_var_wgt1_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8skin_var_wgt1);

        hi_isp_sharpen_skin_u_max_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8skin_u_max);
        hi_isp_sharpen_skin_u_min_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8skin_u_min);
        hi_isp_sharpen_skin_v_max_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8skin_v_max);
        hi_isp_sharpen_skin_v_min_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8skin_v_min);

        hi_isp_sharpen_edgsm_en_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.bEnEdgSm);
        hi_isp_sharpen_edgsm_str_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8EdgSmStr);
        hi_isp_sharpen_diff_thd_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8DiffThd);
        hi_isp_sharpen_diff_mul_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8DiffMul);

        hi_isp_sharpen_skin_len_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8skin_len);

        // luma for blending
        hi_isp_sharpen_lumactrl_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.bEnLumaCtrl);

        pu32lumaWgt = (HI_U32*)&pstRegCfgInfo->stSharpenRegCfg.u8lumaWgt[0];
        for ( i = 0 ; i < ISP_YUV_SHPLUMA_NUM/sizeof(HI_U32); i++)
        {

            hi_isp_sharpen_lumawgt_write(IspDev, i, *pu32lumaWgt++);
        }

        // var for shoot ctrl
        hi_isp_sharpen_shtvarctrl_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.bEnShtCtrlVarMod);
        hi_isp_sharpen_overvaramt_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8OShtVarAmt);
        hi_isp_sharpen_overvarmul_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.s16OShtVarMul);
        hi_isp_sharpen_undervaramt_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8UShtVarAmt);
        hi_isp_sharpen_undervarmul_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.s16UShtVarMul);

        hi_isp_sharpen_mid_tmp00_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.G3[0][0]);
        hi_isp_sharpen_mid_tmp01_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.G3[0][1]);
        hi_isp_sharpen_mid_tmp02_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.G3[0][2]);
        hi_isp_sharpen_mid_tmp10_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.G3[1][0]);
        hi_isp_sharpen_mid_tmp11_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.G3[1][1]);
        hi_isp_sharpen_mid_tmp12_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.G3[1][2]);
        hi_isp_sharpen_mid_tmp20_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.G3[2][0]);
        hi_isp_sharpen_mid_tmp21_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.G3[2][1]);
        hi_isp_sharpen_mid_tmp22_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.G3[2][2]);

        hi_isp_sharpen_oshtvar_min_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8OvShtVarMin);
        hi_isp_sharpen_oshtvar_max_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8OvShtVarMax);
        hi_isp_sharpen_oshtsup_mul_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16OvShtSupMul);
        hi_isp_sharpen_oshtwgt_min_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8OvShtWgtMin);
        hi_isp_sharpen_ushtvar_min_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8UnShtVarMin);
        hi_isp_sharpen_ushtvar_max_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8UnShtVarMax);
        hi_isp_sharpen_ushtsup_mul_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16UnShtSupMul);
        hi_isp_sharpen_ushtwgt_min_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8UnShtWgtMin);
        hi_isp_sharpen_oshtdiff_min_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8OvShtDiffMin);
        hi_isp_sharpen_oshtdiff_max_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8OvShtDiffMax);
        hi_isp_sharpen_oshtsupdiff_mul_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16OvShtSupDiffMul);
        hi_isp_sharpen_oshtdiffwgt_min_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8OvShtDiffWgtMin);
        hi_isp_sharpen_ushtdiff_min_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8UnShtDiffMin);
        hi_isp_sharpen_ushtdiff_max_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8UnShtDiffMax);
        hi_isp_sharpen_ushtsupdiff_mul_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16UnShtSupDiffMul);
        hi_isp_sharpen_ushtdiffwgt_min_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8UnShtDiffWgtMin);
        hi_isp_sharpen_minnosthd_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8MinNoiseThd);
        hi_isp_sharpen_maxnosthd_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8MaxNoiseThd);
        hi_isp_sharpen_shtsupbldr_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8ShtSupBldr);
        hi_isp_sharpen_shtsup_en_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.bEnShtSup);
        hi_isp_sharpen_shtmax_en_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.bEnShtMax);
        hi_isp_sharpen_udhfgain_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16UDHfGain);
        hi_isp_sharpen_oshtmaxvargain_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8oShtMaxVarGain);
        hi_isp_sharpen_ushtmaxvargain_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8uShtMaxVarGain);
        hi_isp_sharpen_oshtmaxchg_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8oShtMaxChg);
        hi_isp_sharpen_ushtmaxchg_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8uShtMaxChg);
        hi_isp_sharpen_shadsup_en_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.bEnShadSup);
        hi_isp_sharpen_shadvar1_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16shadVar1);
        hi_isp_sharpen_shadvar2_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16shadVar2);
        hi_isp_sharpen_gradthd1_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8gradThd1);
        hi_isp_sharpen_gradthd2_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8gradThd2);
        hi_isp_sharpen_wgtthd1_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8shadWgt1);
        hi_isp_sharpen_wgtthd2_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8shadWgt2);
        hi_isp_sharpen_wgtmul_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8ShadWgtMul);

        hi_isp_sharpen_rgbshp_en_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.bEnRgbShp);
        hi_isp_sharpen_rcbmax_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8RcBmax);
        hi_isp_sharpen_rcrmax_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8RcRmax);
        hi_isp_sharpen_rcbmin_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8RcBmin);
        hi_isp_sharpen_rcrmin_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8RcRmin);
        hi_isp_sharpen_bcbmax_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8BcBmax);
        hi_isp_sharpen_bcrmax_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8BcRmax);
        hi_isp_sharpen_bcbmin_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8BcBmin);
        hi_isp_sharpen_bcrmin_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8BcRmin);
        hi_isp_sharpen_gcbmax_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8GcBmax);
        hi_isp_sharpen_gcrmax_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8GcRmax);
        hi_isp_sharpen_gcbmin_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8GcBmin);
        hi_isp_sharpen_gcrmin_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8GcRmin);
        hi_isp_sharpen_gwgt_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8gWgt);
        hi_isp_sharpen_bwgt_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8bWgt);
        hi_isp_sharpen_rwgt_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8rWgt);
        hi_isp_sharpen_bshpgainsft_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.s8bShpGainSft);
        hi_isp_sharpen_rshpgainsft_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.s8rShpGainSft);
        hi_isp_sharpen_bcbthd_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8BcBThd);
        hi_isp_sharpen_rcrthd_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8RcRThd);
        hi_isp_sharpen_bshpgainmul_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.s16bShpGainMul);
        hi_isp_sharpen_rshpgainmul_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.s16rShpGainMul);
        hi_isp_sharpen_lowbandesm_en_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.bEnLowBandEsm);
    }

    if (pstRegCfgInfo->unKey.bit1GeCfg)
    {
        hi_isp_ge_strength_write(IspDev,  pstRegCfgInfo->stGeRegCfg.u16GeStrength);
        hi_isp_ge_enable_write(IspDev, 0, pstRegCfgInfo->stGeRegCfg.ge_enable);
        hi_isp_ge_enable_write(IspDev, 1, pstRegCfgInfo->stGeRegCfg.ge_enable);
        hi_isp_ge_ct_th1_write(IspDev, 0, pstRegCfgInfo->stGeRegCfg.ct_th1);
        hi_isp_ge_ct_th1_write(IspDev, 1, pstRegCfgInfo->stGeRegCfg.ct_th1);
        hi_isp_ge_ct_th2_write(IspDev, 0, pstRegCfgInfo->stGeRegCfg.detail_th);
        hi_isp_ge_ct_th2_write(IspDev, 1, pstRegCfgInfo->stGeRegCfg.detail_th);
        hi_isp_ge_ct_th3_write(IspDev, 0, pstRegCfgInfo->stGeRegCfg.ge_th);
        hi_isp_ge_ct_th3_write(IspDev, 1, pstRegCfgInfo->stGeRegCfg.ge_th);
        hi_isp_ge_ct_slope_write(IspDev, 0, pstRegCfgInfo->stGeRegCfg.detail_slop, pstRegCfgInfo->stGeRegCfg.ge_th_slop);
        hi_isp_ge_ct_slope_write(IspDev, 1, pstRegCfgInfo->stGeRegCfg.detail_slop, pstRegCfgInfo->stGeRegCfg.ge_th_slop);
    }

    if (pstRegCfgInfo->unKey.bit1DpCfg)
    {
            //hi_isp_dp_in_soft_rst_write(pstRegCfgInfo->stDpRegCfg.u32DpccInSoftRst);
            //pstRegCfgInfo->stDpRegCfg.u32DpccregsBayerPat
            hi_isp_dp_bpt_ctrl_write(IspDev, pstRegCfgInfo->stDpRegCfg.u32DpccBptCtrl);
            //hi_isp_dpc_enable_write(pstRegCfgInfo->stDpRegCfg.u32DpccCfg);
            hi_isp_dpc_blend_write(IspDev, pstRegCfgInfo->stDpRegCfg.u32DpccAlpha);
            hi_isp_dp_mode_write(IspDev, pstRegCfgInfo->stDpRegCfg.u32DpccMode);
            hi_isp_dp_output_mode_write(IspDev, pstRegCfgInfo->stDpRegCfg.u32DpccOutputMode);
            hi_isp_dp_set_use_write(IspDev, pstRegCfgInfo->stDpRegCfg.u32DpccSetUse);
            hi_isp_dp_methods_set_1_write(IspDev, pstRegCfgInfo->stDpRegCfg.u32DpccMethodsSet1);
            hi_isp_dp_methods_set_2_write(IspDev, pstRegCfgInfo->stDpRegCfg.u32DpccMethodsSet2);
            hi_isp_dp_methods_set_3_write(IspDev, pstRegCfgInfo->stDpRegCfg.u32DpccMethodsSet3);
            //hi_isp_dp_line_thresh_1_write(IspDev, pstRegCfgInfo->stDpRegCfg.u32DpccLineThresh1);
            //hi_isp_dp_line_mad_fac_1_write(IspDev, pstRegCfgInfo->stDpRegCfg.u32DpccLineMadFac1);
            hi_isp_dp_pg_fac_1_write(IspDev, pstRegCfgInfo->stDpRegCfg.u32DpccPgFac1);
            hi_isp_dp_rnd_thresh_1_write(IspDev, pstRegCfgInfo->stDpRegCfg.u32DpccRndThresh1);
            hi_isp_dp_rg_fac_1_write(IspDev, pstRegCfgInfo->stDpRegCfg.u32DpccRgFac1);
            hi_isp_dp_line_thresh_2_write(IspDev, pstRegCfgInfo->stDpRegCfg.u32DpccLineThresh2);
            hi_isp_dp_line_mad_fac_2_write(IspDev, pstRegCfgInfo->stDpRegCfg.u32DpccLineMadFac2);
            hi_isp_dp_pg_fac_2_write(IspDev, pstRegCfgInfo->stDpRegCfg.u32DpccPgFac2);
            hi_isp_dp_rnd_thresh_2_write(IspDev, pstRegCfgInfo->stDpRegCfg.u32DpccRndThresh2);
            hi_isp_dp_rg_fac_2_write(IspDev, pstRegCfgInfo->stDpRegCfg.u32DpccRgFac2);
            hi_isp_dp_line_thresh_3_write(IspDev, pstRegCfgInfo->stDpRegCfg.u32DpccLineThresh3);
            hi_isp_dp_line_mad_fac_3_write(IspDev, pstRegCfgInfo->stDpRegCfg.u32DpccLineMadFac3);
            hi_isp_dp_pg_fac_3_write(IspDev, pstRegCfgInfo->stDpRegCfg.u32DpccPgFac3);
            hi_isp_dp_rnd_thresh_3_write(IspDev, pstRegCfgInfo->stDpRegCfg.u32DpccRndThresh3);
            hi_isp_dp_rg_fac_3_write(IspDev, pstRegCfgInfo->stDpRegCfg.u32DpccRgFac3 );
            hi_isp_dp_ro_limits_write(IspDev, pstRegCfgInfo->stDpRegCfg.u32DpccRoLimits);
            hi_isp_dp_rnd_offs_write(IspDev, pstRegCfgInfo->stDpRegCfg.u32DpccRndOffs);
            hi_isp_dp_bpt_thresh_write(IspDev, pstRegCfgInfo->stDpRegCfg.u32DpccBadThresh);
    }

    if (pstRegCfgInfo->unKey.bit1DehazeCfg)
    {

        if (pstRegCfgInfo->stDehazeRegCfg.u8DehazeEnable)
        {
            hi_isp_dehaze_air_r_write(IspDev, pstRegCfgInfo->stDehazeRegCfg.u16AirR);
            hi_isp_dehaze_air_g_write(IspDev, pstRegCfgInfo->stDehazeRegCfg.u16AirG);
            hi_isp_dehaze_air_b_write(IspDev, pstRegCfgInfo->stDehazeRegCfg.u16AirR);
        }

         hi_isp_dehaze_gstrth_write(IspDev, pstRegCfgInfo->stDehazeRegCfg.u8Strength);
         hi_isp_dehaze_pre_update_write(IspDev, pstRegCfgInfo->stDehazeRegCfg.u32Update);
    }

    if (pstRegCfgInfo->unKey.bit1WdrCfg)
    {
        //hi_vi_top_channel_switch_write(pstRegCfgInfo->stWdrRegCfg.u8TopChannelSwitch);
    }

    if (pstRegCfgInfo->unKey.bit1LscCfg)
    {
        hi_isp_lsc_cfg_enable_write(IspDev,HI_TRUE);
        /* write horizontal grid info */
        for (i = 0; i < 8; i++)
        {
            hi_isp_lsc_hwin_info_write(IspDev,i, (2*(pstRegCfgInfo->stLscRegCfg.xgrid[i]) - 1), pstRegCfgInfo->stLscRegCfg.xinvgrid[i]);
        }

        /* write vertical grid info */
        for (i = 0; i < 8; i++)
        {
            hi_isp_lsc_vwin_info_write(IspDev,i, (2*(pstRegCfgInfo->stLscRegCfg.ygrid[i]) - 1), pstRegCfgInfo->stLscRegCfg.yinvgrid[i]);
        }


        /* write gain data */
        hi_isp_lsc_grr_gain_waddr_write(IspDev, 0);
        for (i = 0; i < 289; i++)
        {
            hi_isp_lsc_grr_gain_wdata_write(IspDev, pstRegCfgInfo->stLscRegCfg.grr_gain[i]);
        }

        hi_isp_lsc_gbb_gain_waddr_write(IspDev, 0);
        for (i = 0; i < 289; i++)
        {
            hi_isp_lsc_gbb_gain_wdata_write(IspDev, pstRegCfgInfo->stLscRegCfg.gbb_gain[i]);
        }

        hi_isp_lsc_lut_update_write(IspDev,HI_TRUE);
        pstRegCfgInfo->unKey.bit1LscCfg = 0;
    }

    if(pstRegCfgInfo->unKey.bit1CacCfg)
    {
        hi_isp_demosaic_local_cac_enable_write(IspDev,pstRegCfgInfo->stCacRegCfg.bLocalCacEn);
        hi_isp_demosaic_r_luma_thr_write(IspDev, pstRegCfgInfo->stCacRegCfg.u16RLumaThr);
        hi_isp_demosaic_g_luma_thr_write(IspDev, pstRegCfgInfo->stCacRegCfg.u16GLumaThr);
        hi_isp_demosaic_b_luma_thr_write(IspDev, pstRegCfgInfo->stCacRegCfg.u16BLumaThr);
        hi_isp_demosaic_depurplectrcr_write(IspDev, pstRegCfgInfo->stCacRegCfg.u8DePurpleCtrCr);
        hi_isp_demosaic_depurplectrcb_write(IspDev, pstRegCfgInfo->stCacRegCfg.u8DePurpleCtrCb);
        hi_isp_demosaic_purple_var_thr_write(IspDev,pstRegCfgInfo->stCacRegCfg.u16VarThr);
        pstRegCfgInfo->unKey.bit1CacCfg = 0;
    }



    if (pstRegCfgInfo->unKey.bit1DemCfg)
    {
        hi_isp_demosaic_fcr_limit1_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16FcrLimit1);
        hi_isp_demosaic_fcr_limit2_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16FcrLimit2);
        hi_isp_demosaic_fcr_gain_write(IspDev, pstRegCfgInfo->stDemRegCfg.u8FcrGain);
        hi_isp_demosaic_fcr_ratio_write(IspDev, pstRegCfgInfo->stDemRegCfg.u8FcrRatio);
        hi_isp_demosaic_fcr_thr_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16FcrThr);
        hi_isp_nddm_fcr_det_low_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16FcrDetLow);
        hi_isp_demosaic_fcr_enable_write(IspDev, pstRegCfgInfo->stDemRegCfg.bFcrEnable);

        //hi_isp_demosaic_cfg_enable_write(IspDev, pstRegCfgInfo->stDemRegCfg.bEnable);
        hi_isp_demosaic_interp_ratio0_write(IspDev, pstRegCfgInfo->stDemRegCfg.u32hfIntpRatio);
        hi_isp_demosaic_interp_ratio1_write(IspDev, pstRegCfgInfo->stDemRegCfg.u32hfIntpRatio1);
        hi_isp_demosaic_hf_intp_th_low_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16hfIntpThLow);
        hi_isp_demosaic_hf_intp_th_high_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16hfIntpThHig);
        hi_isp_demosaic_hf_intp_bld_low_write(IspDev, pstRegCfgInfo->stDemRegCfg.u8hfIntpBldLow);

        hi_isp_demosaic_hf_intp_th_low1_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16hfIntpThLow1);
        hi_isp_demosaic_hf_intp_th_high1_write(IspDev,  pstRegCfgInfo->stDemRegCfg.u16hfIntpThHig1);

        hi_isp_demosaic_fake_cr_var_thr_low_write(IspDev,pstRegCfgInfo->stDemRegCfg.u16FakeCrVarThrLow);
        hi_isp_nddm_bldr_cbcr_write(IspDev, pstRegCfgInfo->stDemRegCfg.u8BldrCrCb);
        hi_isp_nddm_bldr_gf_str_write(IspDev, pstRegCfgInfo->stDemRegCfg.u8BldrGFStr);
        hi_isp_nddm_multi_mf_write(IspDev, pstRegCfgInfo->stDemRegCfg.u8MultiMF);
        hi_isp_nddm_fcr_gf_gain_write(IspDev, pstRegCfgInfo->stDemRegCfg.s8FcrGFGain);
        hi_isp_nddm_clip_usm_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16ClipUSM);

        hi_isp_nddm_chrm_fix_write(IspDev, pstRegCfgInfo->stDemRegCfg.u8ChrmGFFix);
        hi_isp_nddm_chrm_low_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16ChrmGFLow);
        hi_isp_nddm_chrm_high_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16ChrmGFHig);
        hi_isp_nddm_satu_th_fix_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16SatuThFix);
        hi_isp_nddm_satu_th_high_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16SatuThHig);
        hi_isp_nddm_satu_th_low_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16SatuThLow);
        hi_isp_nddm_gray_th_fix_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16GrayThFixLow);
        hi_isp_nddm_gray_th_fix2_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16GrayThFixHigh);
        hi_isp_nddm_gray_th_high_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16GrayThHig);
        hi_isp_nddm_gray_th_low_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16GrayThLow);
        hi_isp_nddm_bldr_gr_gb_write(IspDev, pstRegCfgInfo->stDemRegCfg.u8BldrGbGr);
        hi_isp_nddm_fcr_limit_high_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16FcrLimitHigh);
        hi_isp_nddm_fcr_limit_low_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16FcrLimitLow);
        hi_isp_nddm_dith_mask_write(IspDev, pstRegCfgInfo->stDemRegCfg.u8DitherMask);
        hi_isp_nddm_dith_max_write(IspDev, pstRegCfgInfo->stDemRegCfg.u8DitherMax);
        hi_isp_nddm_dith_ratio_write(IspDev, pstRegCfgInfo->stDemRegCfg.u8DitherRatio);

        hi_isp_nddm_bldr_var_str_write(IspDev, pstRegCfgInfo->stDemRegCfg.u8BldrVarStr);
        hi_isp_nddm_sht_ctrl_gain_write(IspDev, pstRegCfgInfo->stDemRegCfg.u8ShtCtrlGain);
        hi_isp_nddm_sht_ctrl_th_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16ShtCtrlTh);
        hi_isp_nddm_clip_r_ud_sht_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16ClipRUdSht);
        hi_isp_nddm_clip_r_ov_sht_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16ClipROvSht);
        hi_isp_nddm_clip_b_ov_sht_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16ClipBOvSht);
        hi_isp_nddm_clip_b_ud_sht_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16ClipBUdSht);
        hi_isp_nddm_multi_mf_r_write(IspDev, pstRegCfgInfo->stDemRegCfg.u8MultiMFRed);
        hi_isp_nddm_multi_mf_b_write(IspDev, pstRegCfgInfo->stDemRegCfg.u8MultiMFBlue);
        hi_isp_nddm_satu_r_th_low_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16SatuRThLow);
        hi_isp_nddm_satu_r_th_high_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16SatuRThHig);
        hi_isp_nddm_satu_r_th_fix_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16SatuRThFix);
        hi_isp_nddm_satu_b_th_low_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16SatuBThLow);
        hi_isp_nddm_satu_b_th_high_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16SatuBThHig);
        hi_isp_nddm_satu_b_th_fix_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16satuBThFix);
        hi_isp_nddm_satu_fix_ehcy_write(IspDev, pstRegCfgInfo->stDemRegCfg.s16SatuFixEhcY);

        hi_isp_nddm_clip_delta_gain_write(IspDev, pstRegCfgInfo->stDemRegCfg.u8ClipDeltaGain);
        hi_isp_nddm_clip_adjust_max_write(IspDev, pstRegCfgInfo->stDemRegCfg.u8ClipAdjustMax);
        hi_isp_nddm_clip_delta_intp_low_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16ClipDeltaIntpLow);
        hi_isp_nddm_clip_delta_intp_high_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16ClipDeltaIntpHigh);
        hi_isp_nddm_filter_str_intp_write(IspDev, pstRegCfgInfo->stDemRegCfg.u8FilterStrIntp);
        hi_isp_nddm_clip_delta_filt_low_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16ClipDeltaFiltLow);
        hi_isp_nddm_clip_delta_filt_high_write(IspDev, pstRegCfgInfo->stDemRegCfg.u16ClipDeltaFiltHigh);
        hi_isp_nddm_filter_str_filt_write(IspDev, pstRegCfgInfo->stDemRegCfg.u8FilterStrFilt);
        hi_isp_nddm_var_offset_gain_write(IspDev, pstRegCfgInfo->stDemRegCfg.u8VarOffsetGain);
        hi_isp_nddm_var_offset_low_write(IspDev, pstRegCfgInfo->stDemRegCfg.u32VarOffsetLow);

        hi_isp_nddm_gf_lut_waddr_write(IspDev, 0);
        for (i=0;i<HI_ISP_NDDM_LUT_LENGTH;i++)
        {
            hi_isp_nddm_gf_lut_wdata_write(IspDev, pstRegCfgInfo->stDemRegCfg.auGFBlurLut[i]);
        }

        hi_isp_nddm_gf_lut_update_write(IspDev, HI_TRUE);
    }

    if (pstRegCfgInfo->unKey.bit1BasCfg)
    {

    }

    if (pstRegCfgInfo->unKey.bit1FsWdrCfg)
    {
        hi_isp_wdr_fl_bmdtmnu_write (IspDev, pstRegCfgInfo->stWdrRegCfg.bFlBmdtMnu);
        hi_isp_wdr_fsnr_judge_write(IspDev, pstRegCfgInfo->stWdrRegCfg.u16MDSfNrThr);
        hi_isp_wdr_bmdtrefnos_write(IspDev, pstRegCfgInfo->stWdrRegCfg.bMDRefNoise);
        hi_isp_wdr_nosfactor_low_write(IspDev, pstRegCfgInfo->stWdrRegCfg.u8NosFactorLow);
        hi_isp_wdr_nosfactor_high_write(IspDev, pstRegCfgInfo->stWdrRegCfg.u8NosFactorHig);
        hi_isp_wdr_nosclipth_write(IspDev, pstRegCfgInfo->stWdrRegCfg.u16NosClipTH);
        hi_isp_wdr_bldrmdtmax_write(IspDev, pstRegCfgInfo->stWdrRegCfg.u8BldrMdtMax);

        if (pstRegCfgInfo->stWdrRegCfg.bUpdateNosLut)
        {
            hi_isp_wdr_noslut_waddr_write(IspDev, 0);

            for(i=0; i < WLUT_LENGTH; i++)
            {
                hi_isp_wdr_noslut_wdata_write(IspDev, pstRegCfgInfo->stWdrRegCfg.au16NosMDTLut[i]);
            }

            hi_isp_wdr_noslut_update_write(IspDev, HI_TRUE);

            pstRegCfgInfo->stWdrRegCfg.bUpdateNosLut = HI_FALSE;
        }
    }

    if (pstRegCfgInfo->unKey.bit1BayernrCfg)
    {
        HI_U32 u32Val;

        hi_isp_bnr_cratio_write(IspDev, pstRegCfgInfo->stBayernrRegCfg.u32RawnrCRatio);
        hi_isp_bnr_jnlm_gain_write(IspDev, pstRegCfgInfo->stBayernrRegCfg.u8JnlmGain);
        //hi_isp_bnr_coring_low_write(IspDev, pstRegCfgInfo->stBayernrRegCfg.u16JnlmCoringLow);
        hi_isp_bnr_coring_hig_write(IspDev, pstRegCfgInfo->stBayernrRegCfg.u16JnlmCoringHig);
        hi_isp_bnr_frm_num_write(IspDev, pstRegCfgInfo->stBayernrRegCfg.u8WdrFrameNum);
        hi_isp_bnr_en_write(IspDev, pstRegCfgInfo->stBayernrRegCfg.u8Enable);

        hi_isp_rggb_amed_lev_write(IspDev, pstRegCfgInfo->stBayernrRegCfg.u8AmedLevel[0]);
        hi_isp_grbg_amed_lev_write(IspDev, pstRegCfgInfo->stBayernrRegCfg.u8AmedLevel[1]);
        hi_isp_gbrg_amed_lev_write(IspDev, pstRegCfgInfo->stBayernrRegCfg.u8AmedLevel[2]);
        hi_isp_bgrg_amed_lev_write(IspDev, pstRegCfgInfo->stBayernrRegCfg.u8AmedLevel[3]);
        hi_isp_bgrg_amed_mode_write(IspDev, pstRegCfgInfo->stBayernrRegCfg.u8AmedMode);

        hi_isp_bnr_rlmt_rgain_write(IspDev, pstRegCfgInfo->stBayernrRegCfg.u16Rgain);
        hi_isp_bnr_rlmt_bgain_write(IspDev, pstRegCfgInfo->stBayernrRegCfg.u16Bgain);

        hi_isp_bnr_odd_rggb_waddr_write(IspDev, 0);
        hi_isp_bnr_odd_grbg_waddr_write(IspDev, 0);
        hi_isp_bnr_odd_gbrg_waddr_write(IspDev, 0);
        hi_isp_bnr_odd_bggr_waddr_write(IspDev, 0);
        hi_isp_bnr_odd_rlmt_waddr_write(IspDev, 0);
        for (i = 0; i < HI_ISP_BAYERNR_LUT_LENGTH; i+=2)
        {
            u32Val = pstRegCfgInfo->stBayernrRegCfg.u16JnlmLimit0Lut[0][i] & 0x7ff;
            hi_isp_bnr_odd_rggb_wdata_write(IspDev, u32Val);

            u32Val = pstRegCfgInfo->stBayernrRegCfg.u16JnlmLimit0Lut[1][i] & 0x7ff;
            hi_isp_bnr_odd_grbg_wdata_write(IspDev, u32Val);

            u32Val = pstRegCfgInfo->stBayernrRegCfg.u16JnlmLimit0Lut[2][i] & 0x7ff;
            hi_isp_bnr_odd_gbrg_wdata_write(IspDev, u32Val);

            u32Val = pstRegCfgInfo->stBayernrRegCfg.u16JnlmLimit0Lut[3][i] & 0x7ff;
            hi_isp_bnr_odd_bggr_wdata_write(IspDev, u32Val);

            u32Val = pstRegCfgInfo->stBayernrRegCfg.u16JNLMCoringLowLUT[i] & 0x3fff;
            hi_isp_bnr_odd_rlmt_wdata_write(IspDev, u32Val);
        }

        hi_isp_bnr_even_rggb_waddr_write(IspDev, 0);
        hi_isp_bnr_even_grbg_waddr_write(IspDev, 0);
        hi_isp_bnr_even_gbrg_waddr_write(IspDev, 0);
        hi_isp_bnr_even_bggr_waddr_write(IspDev, 0);
        hi_isp_bnr_even_rlmt_waddr_write(IspDev, 0);
        for (i = 1; i < HI_ISP_BAYERNR_LUT_LENGTH; i+=2)
        {
            u32Val = pstRegCfgInfo->stBayernrRegCfg.u16JnlmLimit0Lut[0][i] & 0x7ff;
            hi_isp_bnr_even_rggb_wdata_write(IspDev, u32Val);

            u32Val = pstRegCfgInfo->stBayernrRegCfg.u16JnlmLimit0Lut[1][i] & 0x7ff;
            hi_isp_bnr_even_grbg_wdata_write(IspDev, u32Val);

            u32Val = pstRegCfgInfo->stBayernrRegCfg.u16JnlmLimit0Lut[2][i] & 0x7ff;
            hi_isp_bnr_even_gbrg_wdata_write(IspDev, u32Val);

            u32Val = pstRegCfgInfo->stBayernrRegCfg.u16JnlmLimit0Lut[3][i] & 0x7ff;
            hi_isp_bnr_even_bggr_wdata_write(IspDev, u32Val);

            u32Val = pstRegCfgInfo->stBayernrRegCfg.u16JNLMCoringLowLUT[i] & 0x3fff;
            hi_isp_bnr_even_rlmt_wdata_write(IspDev, u32Val);
        }

        hi_isp_bnr_lut_update_write(IspDev, 1);
    }

    if (pstRegCfgInfo->unKey.bit1FlickerCfg)
    {
        hi_isp_flick_pre_avg_gr_write(IspDev, pstRegCfgInfo->stFlickerRegCfg.s32PreFrameAvgGr);
        hi_isp_flick_pre_avg_gb_write(IspDev, pstRegCfgInfo->stFlickerRegCfg.s32PreFrameAvgGb);
    }

    if (pstRegCfgInfo->unKey.bit1CaCfg)
    {
        hi_isp_ca_lumath_high_write(IspDev, pstRegCfgInfo->stCaRegCfg.u16LumaThdHigh);
        hi_isp_ca_lumaratio_high_write(IspDev, pstRegCfgInfo->stCaRegCfg.u16HighLumaRatio);
        hi_isp_ca_isoratio_write(IspDev, pstRegCfgInfo->stCaRegCfg.s16ISORatio);

        hi_isp_ca_gainlut_waddr_write(IspDev, 0);
        for (i=0;i<HI_ISP_CA_YRATIO_LUT_LENGTH;i++)
        {
            hi_isp_ca_gainlut_wdata_write(IspDev, pstRegCfgInfo->stCaRegCfg.au16YRatioLUT[i]);
        }

        hi_isp_ca_lut_update_write(IspDev, HI_TRUE);
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

        ISP_RegConfigInit(IspDev, &pstRegCfg->stRegCfg);

        pstRegCfg->bInit = HI_TRUE;
    }

    pstRegCfg->stRegCfg.unKey.u32Key = 0;
    pstRegCfg->stRegCfg.stKernelCfg.unKey.u32Key = 0;

    *ppCfg = &pstRegCfg->stRegCfg;

    return HI_SUCCESS;
}

HI_S32 ISP_RegCfgSet(ISP_DEV IspDev)
{
    ISP_REGCFG_S *pstRegCfg = HI_NULL;

    REGCFG_GET_CTX(IspDev, pstRegCfg);

    ISP_RegConfig(IspDev, &pstRegCfg->stRegCfg);

    return HI_SUCCESS;
}

HI_S32 ISP_SyncCfgSet(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_REGCFG_S *pstRegCfg = HI_NULL;
    ISP_SNS_REGS_INFO_S *pstSnsRegsInfo = NULL;

    REGCFG_GET_CTX(IspDev, pstRegCfg);

    ISP_SensorUpdateSnsReg(IspDev);
    ISP_SensorGetSnsReg(IspDev, &pstSnsRegsInfo);
    memcpy(&pstRegCfg->stSyncCfgNode.stSnsRegsInfo, pstSnsRegsInfo, sizeof(ISP_SNS_REGS_INFO_S));
    HI_MPI_ISP_SnsRegsCfg(IspDev, &pstRegCfg->stSyncCfgNode.stSnsRegsInfo);

    memcpy(&pstRegCfg->stSyncCfgNode.stAERegCfg, &pstRegCfg->stRegCfg.stAeRegCfg2, sizeof(ISP_AE_REG_CFG_2_S));
    memcpy(&pstRegCfg->stSyncCfgNode.stWDRRegCfg, &pstRegCfg->stRegCfg.stWdrRegCfg, sizeof(ISP_WDR_REG_CFG_S));
    pstRegCfg->stSyncCfgNode.bValid = HI_TRUE;

    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_SYNC_CFG_SET, &pstRegCfg->stSyncCfgNode);
    if (HI_SUCCESS != s32Ret)
    {
        printf("Config Sync register Failed with ec %#x!\n", s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

