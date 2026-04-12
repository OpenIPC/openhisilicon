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
#include "isp_ext_config.h"
#include "isp_main.h"


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

    if (pstRegCfgInfo->unKey.bit1AeCfg1)
    {
        for (i = 0; i < 15; i++)
        {
            for (j = 0; j < 17; j++)
            {
                pstRegCfgInfo->stAeRegCfg1.au8WeightTable[i][j] = hi_isp_zones_aexp_weight_read(IspDev, i, j);
            }
        }

        pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[0] = hi_isp_metering_hist_thresh_0_1_read(IspDev);
        pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[1] = hi_isp_metering_hist_thresh_1_2_read(IspDev);
        pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[2] = hi_isp_metering_hist_thresh_3_4_read(IspDev);
        pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[3] = hi_isp_metering_hist_thresh_4_5_read(IspDev);
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

        pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[0] = hi_isp_white_balance_gain_00_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[1] = hi_isp_white_balance_gain_01_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[2] = hi_isp_white_balance_gain_10_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[3] = hi_isp_white_balance_gain_11_read(IspDev);
    }

    if (pstRegCfgInfo->unKey.bit1AwbCfg2)
    {
        pstRegCfgInfo->stAwbRegCfg2.u16MeteringCrRefMaxAwb = hi_isp_metering_cr_ref_max_awb_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg2.u16MeteringCrRefMinAwb = hi_isp_metering_cr_ref_min_awb_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg2.u16MeteringCbRefMaxAwb = hi_isp_metering_cb_ref_max_awb_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg2.u16MeteringCbRefMinAwb = hi_isp_metering_cb_ref_min_awb_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg2.u16MeteringWhiteLevelAwb = hi_isp_metering_white_level_awb_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg2.u16MeteringBlackLevelAwb = hi_isp_metering_black_level_awb_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg2.u16MeteringCrRefHighAwb = hi_isp_metering_cr_ref_high_awb_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg2.u16MeteringCrRefLowAwb = hi_isp_metering_cr_ref_low_awb_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg2.u16MeteringCbRefHighAwb = hi_isp_metering_cb_ref_high_awb_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg2.u16MeteringCbRefLowAwb = hi_isp_metering_cb_ref_low_awb_read(IspDev);
    }

    if (pstRegCfgInfo->unKey.bit1AwbCfg3)
    {
        pstRegCfgInfo->stAwbRegCfg3.bAboveWhiteLevelClip = hi_isp_metering_max_clip_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg3.bBelowBlackLevelClip = hi_isp_metering_min_clip_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefMaxAwb = hi_isp_metering_cr_ref_max_awb_sum_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefMinAwb = hi_isp_metering_cr_ref_min_awb_sum_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefMaxAwb = hi_isp_metering_cb_ref_max_awb_sum_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefMinAwb = hi_isp_metering_cb_ref_min_awb_sum_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringWhiteLevelAwb = hi_isp_metering_max_threshold_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringBlackLevelAwb = hi_isp_metering_min_threshold_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefHighAwb = hi_isp_metering_cr_ref_high_awb_sum_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefLowAwb = hi_isp_metering_cr_ref_low_awb_sum_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefHighAwb = hi_isp_metering_cb_ref_high_awb_sum_read(IspDev);
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefLowAwb = hi_isp_metering_cb_ref_low_awb_sum_read(IspDev);
    }

    if (pstRegCfgInfo->unKey.bit1AfCfg)
    {

    }

    if (pstRegCfgInfo->unKey.bit1OffsetCfg)
    {
        pstRegCfgInfo->stOffsetRegCfg.au16BlackLevelOffset[0] = (hi_isp_offset_black_00_read(IspDev) >> 8);
        pstRegCfgInfo->stOffsetRegCfg.au16BlackLevelOffset[1] = (hi_isp_offset_black_01_read(IspDev) >> 8);
        pstRegCfgInfo->stOffsetRegCfg.au16BlackLevelOffset[2] = (hi_isp_offset_black_10_read(IspDev) >> 8);
        pstRegCfgInfo->stOffsetRegCfg.au16BlackLevelOffset[3] = (hi_isp_offset_black_11_read(IspDev) >> 8);
    }

    if (pstRegCfgInfo->unKey.bit1DrcCfg)
    {
        //pstRegCfgInfo->stDrcRegCfg.u32Strength = hi_isp_drc_strength_read(IspDev);
        pstRegCfgInfo->stDrcRegCfg.u32Strength = hi_isp_drc_strength_inroi_read(IspDev);
    }

    if (pstRegCfgInfo->unKey.bit1NrCfg)
    {
        pstRegCfgInfo->stNrRegCfg.u16NpExpThresh = hi_isp_noise_profile_exp_thresh_read(IspDev);
        pstRegCfgInfo->stNrRegCfg.u8NpShortRatio = hi_isp_noise_profile_short_ratio_read(IspDev);
        pstRegCfgInfo->stNrRegCfg.u8NpLongRatio = hi_isp_noise_profile_long_ratio_read(IspDev);

        pstRegCfgInfo->stNrRegCfg.u8SinterThreshShort = hi_isp_nr_thresh_short_read(IspDev);
        pstRegCfgInfo->stNrRegCfg.u8SinterThreshLong = hi_isp_nr_thresh_long_read(IspDev);
        pstRegCfgInfo->stNrRegCfg.u8NpOffset = hi_isp_demosaic_np_offset_read(IspDev);
    }

    if (pstRegCfgInfo->unKey.bit1BayerShpCfg)
    {
        pstRegCfgInfo->stSharpenRegCfg.u16LumThresh = hi_isp_demosaic_lum_thresh_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8SharpAltD = hi_isp_demosaic_sharp_alt_d_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8SharpAltUd = hi_isp_demosaic_sharp_alt_ud_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.bShpAlgSel = hi_isp_demosaic_sharpen_alg_select_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8SharpenHF = hi_isp_demosaic_sharp_alt_ld_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8SharpenMF = hi_isp_demosaic_sharp_alt_ldu_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8SharpenLF = hi_isp_demosaic_sharp_alt_lu_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8SadAmplifier = hi_isp_demosaic_sad_amp_read(IspDev);
    }

    if (pstRegCfgInfo->unKey.bit1YuvShpCfg)
    {
        pstRegCfgInfo->stSharpenRegCfg.u16EdgeAmt = hi_isp_edge_amt_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u16SharpAmt = hi_isp_sharp_amt_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u16EdgeThd1 = hi_isp_edge_thd1_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u16SharpThd1 = hi_isp_sharp_thd1_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u16EdgeThd1MulCoef = hi_isp_edge_coef_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u16SharpThd1MulCoef= hi_isp_sharp_coef_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8OvershootAmt = hi_isp_over_amt_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8UndershootAmt = hi_isp_under_amt_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u16VarJagThd2 = hi_isp_sharpen_varjagthd2_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u16VarJagThd1 = hi_isp_sharpen_varjagthd1_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.s16EdgeJagMulCoef = hi_isp_sharpen_edgejagmulcoef_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u16EdgeJagAmt = hi_isp_sharpen_edgejagamt_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.s16OshtJagMulCoef = hi_isp_sharpen_oshtjagmulcoef_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8OshtJagAmt = hi_isp_sharpen_oshtjagamt_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.s16UshtJagMulCoef = hi_isp_sharpen_ushtjagmulcoef_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8UshtJagAmt = hi_isp_sharpen_ushtjagamt_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.G3[0][0] = hi_isp_sharpen_mid_tmp00_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.G3[0][1] = hi_isp_sharpen_mid_tmp01_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.G3[0][2] = hi_isp_sharpen_mid_tmp02_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.G3[1][0] = hi_isp_sharpen_mid_tmp10_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.G3[1][1] = hi_isp_sharpen_mid_tmp11_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.G3[1][2] = hi_isp_sharpen_mid_tmp12_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.G3[2][0] = hi_isp_sharpen_mid_tmp20_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.G3[2][1] = hi_isp_sharpen_mid_tmp21_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.G3[2][2] = hi_isp_sharpen_mid_tmp22_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8LineThd2 = hi_isp_sharpen_line_thd2_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8LineThd1 = hi_isp_sharpen_line_thd1_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u16EdgeThd2 = hi_isp_edge_thd2_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u16SharpThd2 = hi_isp_sharp_thd2_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.bEnJagCtrl = hi_isp_sharpen_jagctrl_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.bEnShtCtrlLumaMod = hi_isp_sharpen_shtlumamod_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.bEnShtCtrlVarMod = hi_isp_sharpen_shtvarctrl_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.bEnLumaCtrl = hi_isp_sharpen_lumactrl_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.enPixSel = hi_isp_sharpen_enpixsel_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8LumaTh[3] = hi_isp_sharpen_lumath3_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8LumaTh[2] = hi_isp_sharpen_lumath2_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8LumaTh[1] = hi_isp_sharpen_lumath1_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8LumaTh[0] = hi_isp_sharpen_lumath0_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8LumaWgt[3] = hi_isp_sharpen_lumawgt3_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8LumaWgt[2] = hi_isp_sharpen_lumawgt2_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8LumaWgt[1] = hi_isp_sharpen_lumawgt1_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8LumaWgt[0] = hi_isp_sharpen_lumawgt0_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.s16LumaMul[2] = hi_isp_sharpen_lumamul2_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.s16LumaMul[1] = hi_isp_sharpen_lumamul1_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.s16LumaMul[0] = hi_isp_sharpen_lumamul0_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u16OShtVarTh[1] = hi_isp_sharpen_overvarth1_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u16OShtVarTh[0] = hi_isp_sharpen_overvarth0_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.s16OShtVarMul = hi_isp_sharpen_overvarmul_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8OShtVarAmt = hi_isp_sharpen_overvaramt_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u16UShtVarTh[1] = hi_isp_sharpen_undervarth1_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u16UShtVarTh[0] = hi_isp_sharpen_undervarth0_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.s16UShtVarMul = hi_isp_sharpen_undervarmul_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8UShtVarAmt = hi_isp_sharpen_undervaramt_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8OShtLumaWgt[1] = hi_isp_sharpen_overlumawgt1_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8OShtLumaWgt[0] = hi_isp_sharpen_overlumawgt0_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8OShtLumaTh[1] = hi_isp_sharpen_overlumath1_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8OShtLumaTh[0] = hi_isp_sharpen_overlumath0_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8UShtLumaWgt[1] = hi_isp_sharpen_underlumawgt1_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8UShtLumaWgt[0] = hi_isp_sharpen_underlumawgt0_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8UShtLumaTh[1] = hi_isp_sharpen_underlumath1_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8UShtLumaTh[0] = hi_isp_sharpen_underlumath0_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.s16UShtLumaMul = hi_isp_sharpen_underlumamul_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.s16OShtLumaMul = hi_isp_sharpen_overlumamul_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8Limit = hi_isp_sharpen_limit_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8ShtMaxShft = hi_isp_sharpen_maxshift_read(IspDev);
        pstRegCfgInfo->stSharpenRegCfg.u8ShtMinShft = hi_isp_sharpen_minshift_read(IspDev);
    }

    if (pstRegCfgInfo->unKey.bit1GeCfg)
    {
        pstRegCfgInfo->stGeRegCfg.u8GeStrength = hi_isp_green_equalize_ge_strength_read(IspDev);
    }

    if (pstRegCfgInfo->unKey.bit1DpCfg)
    {
        pstRegCfgInfo->stDpRegCfg.u8DpEnable = hi_isp_defect_pixel_enable_read(IspDev);
        pstRegCfgInfo->stDpRegCfg.u8DpPointerReset = hi_isp_defect_pixel_pointer_reset_read(IspDev);
        pstRegCfgInfo->stDpRegCfg.u16DPThresh = hi_isp_green_equalize_dp_threshold_read(IspDev);
        pstRegCfgInfo->stDpRegCfg.u8DpDetectionTrigger = hi_isp_defect_pixel_detection_trigger_read(IspDev);
        pstRegCfgInfo->stDpRegCfg.u16DpCountIn = hi_isp_defect_pixel_defect_pixel_count_in_read(IspDev);
        pstRegCfgInfo->stDpRegCfg.u8ShowStaticDefectPixels = hi_isp_defect_pixel_show_static_defect_pixels_read(IspDev);
        pstRegCfgInfo->stDpRegCfg.u16DpSlope = hi_isp_green_equalize_dp_slope_read(IspDev);
    }

    if (pstRegCfgInfo->unKey.bit1WdrCfg)
    {

    }

    return HI_SUCCESS;
}

HI_S32 ISP_RegConfig(ISP_DEV IspDev, ISP_REG_CFG_S *pstRegCfgInfo)
{
    HI_S32 i, j;
    HI_S32 s32Ret;

    if (pstRegCfgInfo->unKey.bit1AeCfg1)
    {
        for (i = 0; i < 15; i++)
        {
            for (j = 0; j < 17; j++)
            {
                hi_isp_zones_aexp_weight_write(IspDev, i, j, pstRegCfgInfo->stAeRegCfg1.au8WeightTable[i][j]);
                hi_isp_zones_aexp_weight_pe_write(IspDev, i, j, pstRegCfgInfo->stAeRegCfg1.au8WeightTable[i][j]);
            }
        }

        hi_isp_metering_hist_thresh_0_1_write(IspDev, pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[0]);
        hi_isp_metering_hist_thresh_1_2_write(IspDev, pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[1]);
        hi_isp_metering_hist_thresh_3_4_write(IspDev, pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[2]);
        hi_isp_metering_hist_thresh_4_5_write(IspDev, pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[3]);
    }

    if (pstRegCfgInfo->unKey.bit1AwbCfg1)
    {
        hi_isp_matrix_coefft_r_r_write(IspDev, pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[0]);
        hi_isp_matrix_coefft_r_g_write(IspDev, pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[1]);
        hi_isp_matrix_coefft_r_b_write(IspDev, pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[2]);
        hi_isp_matrix_coefft_g_r_write(IspDev, pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[3]);
        hi_isp_matrix_coefft_g_g_write(IspDev, pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[4]);
        hi_isp_matrix_coefft_g_b_write(IspDev, pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[5]);
        hi_isp_matrix_coefft_b_r_write(IspDev, pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[6]);
        hi_isp_matrix_coefft_b_g_write(IspDev, pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[7]);
        hi_isp_matrix_coefft_b_b_write(IspDev, pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[8]);
        #if 1
        hi_isp_white_balance_gain_00_write(IspDev, pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[0]);
        hi_isp_white_balance_gain_01_write(IspDev, pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[1]);
        hi_isp_white_balance_gain_10_write(IspDev, pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[2]);
        hi_isp_white_balance_gain_11_write(IspDev, pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[3]);
        #endif
    }

    if (pstRegCfgInfo->unKey.bit1AwbCfg2)
    {
        hi_isp_metering_cr_ref_max_awb_write(IspDev, pstRegCfgInfo->stAwbRegCfg2.u16MeteringCrRefMaxAwb);
        hi_isp_metering_cr_ref_min_awb_write(IspDev, pstRegCfgInfo->stAwbRegCfg2.u16MeteringCrRefMinAwb);
        hi_isp_metering_cb_ref_max_awb_write(IspDev, pstRegCfgInfo->stAwbRegCfg2.u16MeteringCbRefMaxAwb);
        hi_isp_metering_cb_ref_min_awb_write(IspDev, pstRegCfgInfo->stAwbRegCfg2.u16MeteringCbRefMinAwb);
        hi_isp_metering_white_level_awb_write(IspDev, pstRegCfgInfo->stAwbRegCfg2.u16MeteringWhiteLevelAwb);
        hi_isp_metering_black_level_awb_write(IspDev, pstRegCfgInfo->stAwbRegCfg2.u16MeteringBlackLevelAwb);
        hi_isp_metering_cr_ref_high_awb_write(IspDev, pstRegCfgInfo->stAwbRegCfg2.u16MeteringCrRefHighAwb);
        hi_isp_metering_cr_ref_low_awb_write(IspDev, pstRegCfgInfo->stAwbRegCfg2.u16MeteringCrRefLowAwb);
        hi_isp_metering_cb_ref_high_awb_write(IspDev, pstRegCfgInfo->stAwbRegCfg2.u16MeteringCbRefHighAwb);
        hi_isp_metering_cb_ref_low_awb_write(IspDev, pstRegCfgInfo->stAwbRegCfg2.u16MeteringCbRefLowAwb);
    }

    if (pstRegCfgInfo->unKey.bit1AwbCfg3)
    {
        hi_isp_metering_max_clip_write(IspDev, pstRegCfgInfo->stAwbRegCfg3.bAboveWhiteLevelClip);
        hi_isp_metering_min_clip_write(IspDev, pstRegCfgInfo->stAwbRegCfg3.bBelowBlackLevelClip);
        hi_isp_metering_cr_ref_max_awb_sum_write(IspDev, pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefMaxAwb);
        hi_isp_metering_cr_ref_min_awb_sum_write(IspDev, pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefMinAwb);
        hi_isp_metering_cb_ref_max_awb_sum_write(IspDev, pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefMaxAwb);
        hi_isp_metering_cb_ref_min_awb_sum_write(IspDev, pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefMinAwb);
        hi_isp_metering_max_threshold_write(IspDev, pstRegCfgInfo->stAwbRegCfg3.u16MeteringWhiteLevelAwb);
        hi_isp_metering_min_threshold_write(IspDev, pstRegCfgInfo->stAwbRegCfg3.u16MeteringBlackLevelAwb);
        hi_isp_metering_cr_ref_high_awb_sum_write(IspDev, pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefHighAwb);
        hi_isp_metering_cr_ref_low_awb_sum_write(IspDev, pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefLowAwb);
        hi_isp_metering_cb_ref_high_awb_sum_write(IspDev, pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefHighAwb);
        hi_isp_metering_cb_ref_low_awb_sum_write(IspDev, pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefLowAwb);
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
        //hi_isp_drc_strength_inroi_write(IspDev, pstRegCfgInfo->stDrcRegCfg.u32Strength);
        //hi_isp_drc_strength_outroi_write(IspDev, pstRegCfgInfo->stDrcRegCfg.u32Strength);
    }

    if (pstRegCfgInfo->unKey.bit1NrCfg)
    {
        hi_isp_noise_profile_exp_thresh_write(IspDev, pstRegCfgInfo->stNrRegCfg.u16NpExpThresh);
        hi_isp_noise_profile_short_ratio_write(IspDev, pstRegCfgInfo->stNrRegCfg.u8NpShortRatio);
        hi_isp_noise_profile_long_ratio_write(IspDev, pstRegCfgInfo->stNrRegCfg.u8NpLongRatio);

        hi_isp_nr_thresh_short_write(IspDev, pstRegCfgInfo->stNrRegCfg.u8SinterThreshShort);
        hi_isp_nr_thresh_long_write(IspDev, pstRegCfgInfo->stNrRegCfg.u8SinterThreshLong);
        hi_isp_demosaic_np_offset_write(IspDev, pstRegCfgInfo->stNrRegCfg.u8NpOffset);
    }

    if (pstRegCfgInfo->unKey.bit1BayerShpCfg)
    {
        hi_isp_demosaic_lum_thresh_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16LumThresh);
        hi_isp_demosaic_sharp_alt_d_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8SharpAltD);
        hi_isp_demosaic_sharp_alt_ud_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8SharpAltUd);
        hi_isp_demosaic_sharpen_alg_select_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.bShpAlgSel);
        hi_isp_demosaic_sharp_alt_ld_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8SharpenHF);
        hi_isp_demosaic_sharp_alt_ldu_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8SharpenMF);
        hi_isp_demosaic_sharp_alt_lu_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8SharpenLF);
        hi_isp_demosaic_sad_amp_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8SadAmplifier);
    }

    if (pstRegCfgInfo->unKey.bit1YuvShpCfg)
    {
        hi_isp_edge_amt_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16EdgeAmt);
        hi_isp_sharp_amt_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16SharpAmt);
        hi_isp_edge_thd1_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16EdgeThd1);
        hi_isp_sharp_thd1_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16SharpThd1);
        hi_isp_edge_coef_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16EdgeThd1MulCoef);
        hi_isp_sharp_coef_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16SharpThd1MulCoef);
        hi_isp_over_amt_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8OvershootAmt);
        hi_isp_under_amt_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8UndershootAmt);
        hi_isp_sharpen_varjagthd2_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16VarJagThd2);
        hi_isp_sharpen_varjagthd1_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16VarJagThd1);
        hi_isp_sharpen_edgejagmulcoef_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.s16EdgeJagMulCoef);
        hi_isp_sharpen_edgejagamt_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16EdgeJagAmt);
        hi_isp_sharpen_oshtjagmulcoef_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.s16OshtJagMulCoef);
        hi_isp_sharpen_oshtjagamt_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8OshtJagAmt);
        hi_isp_sharpen_ushtjagmulcoef_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.s16UshtJagMulCoef);
        hi_isp_sharpen_ushtjagamt_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8UshtJagAmt);
        hi_isp_sharpen_mid_tmp00_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.G3[0][0]);
        hi_isp_sharpen_mid_tmp01_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.G3[0][1]);
        hi_isp_sharpen_mid_tmp02_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.G3[0][2]);
        hi_isp_sharpen_mid_tmp10_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.G3[1][0]);
        hi_isp_sharpen_mid_tmp11_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.G3[1][1]);
        hi_isp_sharpen_mid_tmp12_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.G3[1][2]);
        hi_isp_sharpen_mid_tmp20_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.G3[2][0]);
        hi_isp_sharpen_mid_tmp21_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.G3[2][1]);
        hi_isp_sharpen_mid_tmp22_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.G3[2][2]);
        hi_isp_sharpen_line_thd2_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8LineThd2);
        hi_isp_sharpen_line_thd1_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8LineThd1);
        hi_isp_edge_thd2_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16EdgeThd2);
        hi_isp_sharp_thd2_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16SharpThd2);
        hi_isp_sharpen_jagctrl_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.bEnJagCtrl);
        hi_isp_sharpen_shtlumamod_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.bEnShtCtrlLumaMod);
        hi_isp_sharpen_shtvarctrl_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.bEnShtCtrlVarMod);
        hi_isp_sharpen_lumactrl_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.bEnLumaCtrl);
        hi_isp_sharpen_enpixsel_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.enPixSel);
        hi_isp_sharpen_lumath3_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8LumaTh[3]);
        hi_isp_sharpen_lumath2_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8LumaTh[2]);
        hi_isp_sharpen_lumath1_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8LumaTh[1]);
        hi_isp_sharpen_lumath0_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8LumaTh[0]);
        hi_isp_sharpen_lumawgt3_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8LumaWgt[3]);
        hi_isp_sharpen_lumawgt2_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8LumaWgt[2]);
        hi_isp_sharpen_lumawgt1_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8LumaWgt[1]);
        hi_isp_sharpen_lumawgt0_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8LumaWgt[0]);
        hi_isp_sharpen_lumamul2_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.s16LumaMul[2]);
        hi_isp_sharpen_lumamul1_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.s16LumaMul[1]);
        hi_isp_sharpen_lumamul0_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.s16LumaMul[0]);
        hi_isp_sharpen_overvarth1_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16OShtVarTh[1]);
        hi_isp_sharpen_overvarth0_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16OShtVarTh[0]);
        hi_isp_sharpen_overvarmul_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.s16OShtVarMul);
        hi_isp_sharpen_overvaramt_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8OShtVarAmt);
        hi_isp_sharpen_undervarth1_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16UShtVarTh[1]);
        hi_isp_sharpen_undervarth0_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u16UShtVarTh[0]);
        hi_isp_sharpen_undervarmul_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.s16UShtVarMul);
        hi_isp_sharpen_undervaramt_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8UShtVarAmt);
        hi_isp_sharpen_overlumawgt1_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8OShtLumaWgt[1]);
        hi_isp_sharpen_overlumawgt0_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8OShtLumaWgt[0]);
        hi_isp_sharpen_overlumath1_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8OShtLumaTh[1]);
        hi_isp_sharpen_overlumath0_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8OShtLumaTh[0]);
        hi_isp_sharpen_underlumawgt1_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8UShtLumaWgt[1]);
        hi_isp_sharpen_underlumawgt0_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8UShtLumaWgt[0]);
        hi_isp_sharpen_underlumath1_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8UShtLumaTh[1]);
        hi_isp_sharpen_underlumath0_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8UShtLumaTh[0]);
        hi_isp_sharpen_underlumamul_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.s16UShtLumaMul);
        hi_isp_sharpen_overlumamul_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.s16OShtLumaMul);
        hi_isp_sharpen_limit_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8Limit);
        hi_isp_sharpen_maxshift_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8ShtMaxShft);
        hi_isp_sharpen_minshift_write(IspDev, pstRegCfgInfo->stSharpenRegCfg.u8ShtMinShft);
    }

    if (pstRegCfgInfo->unKey.bit1GeCfg)
    {
        hi_isp_green_equalize_ge_strength_write(IspDev, pstRegCfgInfo->stGeRegCfg.u8GeStrength);
        hi_isp_raw_frontend_2_ge_strength_write(IspDev, pstRegCfgInfo->stGeRegCfg.u8GeStrength);
    }
/*
    if (pstRegCfgInfo->unKey.bit1DpCfg)
    {
        hi_isp_defect_pixel_enable_write(IspDev, pstRegCfgInfo->stDpRegCfg.u8DpEnable);
        hi_isp_defect_pixel_pointer_reset_write(IspDev, pstRegCfgInfo->stDpRegCfg.u8DpPointerReset);
        hi_isp_green_equalize_dp_threshold_write(IspDev, pstRegCfgInfo->stDpRegCfg.u16DPThresh);
        hi_isp_defect_pixel_detection_trigger_write(IspDev, pstRegCfgInfo->stDpRegCfg.u8DpDetectionTrigger);
        hi_isp_defect_pixel_defect_pixel_count_in_write(IspDev, pstRegCfgInfo->stDpRegCfg.u16DpCountIn);
        hi_isp_defect_pixel_show_static_defect_pixels_write(IspDev, pstRegCfgInfo->stDpRegCfg.u8ShowStaticDefectPixels);
        hi_isp_green_equalize_dp_slope_write(IspDev, pstRegCfgInfo->stDpRegCfg.u16DpSlope);
        //hi_isp_green_equalize_dp_threshold_write(IspDev, pstRegCfgInfo->stDpRegCfg.u16DynamicDPThresh);
        hi_isp_raw_frontend_2_dp_threshold_write(IspDev, pstRegCfgInfo->stDpRegCfg.u16DPThresh);
        hi_isp_raw_frontend_2_dp_slope_write(IspDev, pstRegCfgInfo->stDpRegCfg.u16DpSlope);
    }
 */
    if (pstRegCfgInfo->unKey.bit1DehazeCfg)
    {
        if (pstRegCfgInfo->stDehazeRegCfg.u8DehazeEnable)
        {
            hi_isp_dehaze_air_r_write(IspDev, pstRegCfgInfo->stDehazeRegCfg.u16AirR);
            hi_isp_dehaze_air_g_write(IspDev, pstRegCfgInfo->stDehazeRegCfg.u16AirG);
            hi_isp_dehaze_air_b_write(IspDev, pstRegCfgInfo->stDehazeRegCfg.u16AirB);

        }
        hi_isp_dehaze_en_write(IspDev, pstRegCfgInfo->stDehazeRegCfg.u8DehazeEnable);
        hi_isp_dehaze_gstrth_write(IspDev, pstRegCfgInfo->stDehazeRegCfg.u8Strength);
        hi_isp_dehaze_pre_update_write(IspDev, pstRegCfgInfo->stDehazeRegCfg.u32Update);
    }

    if (pstRegCfgInfo->unKey.bit1WdrCfg)
    {

    }

    if (pstRegCfgInfo->unKey.bit1ACMCfg)
    {
        ISP_ACM_ATTR_S stAcmAttr = {0};

        s32Ret = ISP_ACM_GetAttr(IspDev, &stAcmAttr);
        if (s32Ret)
        {
            return s32Ret;
        }
        stAcmAttr.u32GainLuma = pstRegCfgInfo->stAcmRegCfg.u32GainLuma;
        stAcmAttr.u32GainHue = pstRegCfgInfo->stAcmRegCfg.u32GainHue;
        stAcmAttr.u32GainSat = pstRegCfgInfo->stAcmRegCfg.u32GainSat;
        s32Ret = ISP_ACM_SetAttr(IspDev, &stAcmAttr);
        if (s32Ret)
        {
            return s32Ret;
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_RegCfgInit(ISP_DEV IspDev, HI_VOID **ppCfg)
{
    ISP_REGCFG_S *pstRegCfg = HI_NULL;

    REGCFG_GET_CTX(IspDev, pstRegCfg);

    if (!pstRegCfg->bInit)
    {
        pstRegCfg->stRegCfg.unKey.u32Key = 0xFFFF;

        ISP_RegConfigInit(IspDev, &pstRegCfg->stRegCfg);

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
    SNAP_TYPE_E enSnapType;
    ISP_SNAP_PIPE_MODE_E enSnapPipeMode;
    HI_U8 i;

    enSnapType = hi_ext_system_snap_type_read(IspDev);
    enSnapPipeMode = hi_ext_system_snap_pipe_mode_read(IspDev);

    REGCFG_GET_CTX(IspDev, pstRegCfg);

    if (((SNAP_TYPE_SYNC_PRO == enSnapType) || (SNAP_TYPE_SYNC == enSnapType)) && (ISP_SNAP_PREVIEW == enSnapPipeMode))
    {
        for (i=0; i<ISP_MAX_DEV_NUM; i++)
        {
            ISP_RegConfig(i, &pstRegCfg->stRegCfg);
        }
    }
    else if ((ISP_SNAP_PICTURE != enSnapPipeMode) || ((SNAP_TYPE_SYNC != enSnapType) && (SNAP_TYPE_SYNC_PRO != enSnapType)))
    {
        ISP_RegConfig(IspDev, &pstRegCfg->stRegCfg);
    }

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

HI_VOID ISP_SensorUpdateSnsRegDelay(ISP_DEV IspDev, ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
    HI_S32 i = 0;
    HI_U32 u32SnsDlyNumCtrl = hi_ext_sns_delay_num_ctrl_read(IspDev);

    if (HI_EXT_SNS_DELAY_NUM_CTRL_USE_CMOS == u32SnsDlyNumCtrl)
    {
        return;
    }
    else if (HI_EXT_SNS_DELAY_NUM_CTRL_USE_EXTREG == u32SnsDlyNumCtrl)
    {
        for (i=0; i<pstSnsRegsInfo->u32RegNum; i++)
        {
            if (pstSnsRegsInfo->enSnsType == ISP_SNS_I2C_TYPE)
            {
                pstSnsRegsInfo->astI2cData[i].u8DelayFrmNum = hi_ext_sns_delay_num_read(IspDev, i);
            }
            else if (pstSnsRegsInfo->enSnsType == ISP_SNS_SSP_TYPE)
            {
                pstSnsRegsInfo->astSspData[i].u8DelayFrmNum = hi_ext_sns_delay_num_read(IspDev, i);
            }
            else
            {}
        }

        pstSnsRegsInfo->u8Cfg2ValidDelayMax = hi_ext_sns_delay_num_read(IspDev, ISP_MAX_SNS_REGS);
        pstSnsRegsInfo->stSlvSync.u8DelayFrmNum = hi_ext_sns_delay_num_read(IspDev, ISP_MAX_SNS_REGS + 1);

    }
    else if (HI_EXT_SNS_DELAY_NUM_CTRL_UPDATE_EXTREG == u32SnsDlyNumCtrl)
    {
        for (i=0; i<pstSnsRegsInfo->u32RegNum; i++)
        {
            if (pstSnsRegsInfo->enSnsType == ISP_SNS_I2C_TYPE)
            {
                hi_ext_sns_delay_num_write(IspDev, i, pstSnsRegsInfo->astI2cData[i].u8DelayFrmNum);
            }
            else if (pstSnsRegsInfo->enSnsType == ISP_SNS_SSP_TYPE)
            {
                hi_ext_sns_delay_num_write(IspDev, i, pstSnsRegsInfo->astSspData[i].u8DelayFrmNum);
            }
            else
            {}
        }

        hi_ext_sns_delay_num_write(IspDev, ISP_MAX_SNS_REGS, pstSnsRegsInfo->u8Cfg2ValidDelayMax);
        hi_ext_sns_delay_num_write(IspDev, ISP_MAX_SNS_REGS + 1, pstSnsRegsInfo->stSlvSync.u8DelayFrmNum);
    }

    return;
}

HI_S32 ISP_SyncCfgSet(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_REGCFG_S *pstRegCfg = HI_NULL;
    ISP_SNS_REGS_INFO_S *pstSnsRegsInfo = NULL;
    HI_U32 u32DevSta = 0, u32DevEnd = 0;
    ISP_CHECK_DEV(IspDev);

    if(ISP_MODE_STITCH == g_astIspCtx[0].enIspMode)
    {
        if(IS_MAIN_CHANNAL(IspDev))
        {
            u32DevSta = IspDev;
            u32DevEnd = IspDev + 1;
        }
        else
        {
            u32DevSta = IspDev;
            u32DevEnd = IspDev - 1;
        }
    }
    else
    {
        u32DevSta = IspDev;
        u32DevEnd = IspDev;
    }

    while(u32DevSta <= u32DevEnd)
    {
        IspDev = u32DevSta;
        REGCFG_GET_CTX(IspDev, pstRegCfg);
        ISP_CHECK_OPEN(IspDev);

        if (HI_SUCCESS != ISP_SensorUpdateSnsReg(IspDev))
        {
            /* If Users need to config AE sync info themselves, they can set pfn_cmos_get_sns_reg_info to NULL in cmos.c */
            /* Then there will be NO AE sync configs in kernel of firmware */
            return HI_SUCCESS;
        }

        ISP_SensorGetSnsReg(IspDev, &pstSnsRegsInfo);
        ISP_SensorUpdateSnsRegDelay(IspDev, pstSnsRegsInfo);
        memcpy(&pstRegCfg->stSyncCfgNode.stSnsRegsInfo, pstSnsRegsInfo, sizeof(ISP_SNS_REGS_INFO_S));
        ISP_SnsRegsInfoCheck(IspDev, &pstRegCfg->stSyncCfgNode.stSnsRegsInfo);
        memcpy(&pstRegCfg->stSyncCfgNode.stAERegCfg, &pstRegCfg->stRegCfg.stAeRegCfg2, sizeof(ISP_AE_REG_CFG_2_S));
        memcpy(&pstRegCfg->stSyncCfgNode.stDRCRegCfg, &pstRegCfg->stRegCfg.stDrcRegCfg, sizeof(ISP_DRC_REG_CFG_S));

        if(ISP_MODE_STITCH == g_astIspCtx[0].enIspMode)
        {
            if(!IS_MAIN_CHANNAL(IspDev))
            {
                memcpy(&pstRegCfg->stSyncCfgNode.stSnsRegsInfo, &g_astRegCfgCtx[0].stSyncCfgNode.stSnsRegsInfo, sizeof(ISP_SNS_REGS_INFO_S));
                memcpy(&pstRegCfg->stSyncCfgNode.stSnsRegsInfo.unComBus, &pstSnsRegsInfo->unComBus, sizeof(ISP_SNS_COMMBUS_U));
                memcpy(&pstRegCfg->stSyncCfgNode.stAERegCfg, &g_astRegCfgCtx[0].stRegCfg.stAeRegCfg2, sizeof(ISP_AE_REG_CFG_2_S));
            }
        }

        pstRegCfg->stSyncCfgNode.bValid = HI_TRUE;

        s32Ret = ioctl(g_as32IspFd[IspDev], ISP_SYNC_CFG_SET, &pstRegCfg->stSyncCfgNode);
        if (HI_SUCCESS != s32Ret)
        {
            //printf("Config Sync register Failed with ec %#x!\n", s32Ret);
            return s32Ret;
        }
        pstSnsRegsInfo->bConfig = HI_TRUE;

        u32DevSta++;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_CapRegCfgSet(ISP_DEV IspDev, ISP_CONFIG_INFO_S *pstCapInfo)
{
    HI_S32 s32Ret;

    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_CAP_INFO_SET, pstCapInfo);
    if (HI_SUCCESS != s32Ret)
    {
        //printf("Config Sync register Failed with ec %#x!\n", s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}
HI_S32 ISP_CapRegCfgGet(ISP_DEV IspDev, ISP_CONFIG_INFO_S *pstCapInfo)
{
    HI_S32 s32Ret;
    HI_U32 au32WbGain[4],i;
    HI_BOOL bUseInfoCCM;
    ISP_REGCFG_S *pstRegCfg = HI_NULL;

    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_CAP_INFO_GET, pstCapInfo);
    if (HI_SUCCESS != s32Ret)
    {
        //printf("Config Sync register Failed with ec %#x!\n", s32Ret);
        return s32Ret;
    }

    if (SNAP_STATE_CFG == pstCapInfo->enCapState)
    {
        for (i=0; i<4; i++)
        {
            au32WbGain[i] = pstCapInfo->au32WhiteBalanceGain[i];
            au32WbGain[i] = (au32WbGain[i] + 128) >> 8;
            /*avoid overflow*/
            au32WbGain[i] = (au32WbGain[i] > 0xFFF) ? 0xFFF : au32WbGain[i];
        }

        bUseInfoCCM = hi_ext_system_snap_use_info_ccm_enable_read(IspDev);
        if (HI_TRUE == bUseInfoCCM)
        {
            hi_isp_matrix_coefft_r_r_write(IspDev, pstCapInfo->au16CapCCM[0]);
            hi_isp_matrix_coefft_r_g_write(IspDev, pstCapInfo->au16CapCCM[1]);
            hi_isp_matrix_coefft_r_b_write(IspDev, pstCapInfo->au16CapCCM[2]);
            hi_isp_matrix_coefft_g_r_write(IspDev, pstCapInfo->au16CapCCM[3]);
            hi_isp_matrix_coefft_g_g_write(IspDev, pstCapInfo->au16CapCCM[4]);
            hi_isp_matrix_coefft_g_b_write(IspDev, pstCapInfo->au16CapCCM[5]);
            hi_isp_matrix_coefft_b_r_write(IspDev, pstCapInfo->au16CapCCM[6]);
            hi_isp_matrix_coefft_b_g_write(IspDev, pstCapInfo->au16CapCCM[7]);
            hi_isp_matrix_coefft_b_b_write(IspDev, pstCapInfo->au16CapCCM[8]);

            hi_isp_white_balance_gain_00_write(IspDev, au32WbGain[0]);
            hi_isp_white_balance_gain_01_write(IspDev, au32WbGain[1]);
            hi_isp_white_balance_gain_10_write(IspDev, au32WbGain[2]);
            hi_isp_white_balance_gain_11_write(IspDev, au32WbGain[3]);
        }
        else
        {
            REGCFG_GET_CTX(IspDev, pstRegCfg);
            ISP_CHECK_OPEN(IspDev);
            pstRegCfg->stRegCfg.stAwbRegCfg1.au32WhiteBalanceGain[0] = au32WbGain[0];
            pstRegCfg->stRegCfg.stAwbRegCfg1.au32WhiteBalanceGain[1] = au32WbGain[1];
            pstRegCfg->stRegCfg.stAwbRegCfg1.au32WhiteBalanceGain[2] = au32WbGain[2];
            pstRegCfg->stRegCfg.stAwbRegCfg1.au32WhiteBalanceGain[3] = au32WbGain[3];
        }

        hi_isp_digital_gain_gain_write(IspDev, pstCapInfo->u32IspDgain);
    }

    return HI_SUCCESS;
}

HI_BOOL ISP_ProTriggerGet(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    HI_BOOL bEnable;

    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_PRO_TRIGGER_GET, &bEnable);
    if (HI_SUCCESS != s32Ret)
    {
        //printf("Config Sync register Failed with ec %#x!\n", s32Ret);
        return HI_FALSE;
    }

    return bEnable;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

