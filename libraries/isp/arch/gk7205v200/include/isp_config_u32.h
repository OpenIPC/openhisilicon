/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
static inline GK_VOID isp_crop_sizeout_u32_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uIspCropWidthOut, GK_U32 uIspCropHeightOut)
{
    U_ISP_CROP_SIZEOUT o_isp_crop_sizeout;
    o_isp_crop_sizeout.bits.isp_crop_width_out = uIspCropWidthOut;
    o_isp_crop_sizeout.bits.isp_crop_height_out = uIspCropHeightOut;
    pstFeReg->ISP_CROP_SIZEOUT.u32 = o_isp_crop_sizeout.u32;
}

static inline GK_VOID isp_crop_pos_u32_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uIspCropPosX, GK_U32 uIspCropPosY)
{
    U_ISP_CROP_POS o_isp_crop_pos;
    o_isp_crop_pos.bits.isp_crop_pos_x = uIspCropPosX;
    o_isp_crop_pos.bits.isp_crop_pos_y = uIspCropPosY;
    pstFeReg->ISP_CROP_POS.u32 = o_isp_crop_pos.u32;
}

static inline GK_VOID isp_fe_blk_size_u32_write(S_ISPFE_REGS_TYPE *pstFeReg, GK_U32 uIspFeBlkWidth, GK_U32 uIspFeBlkHeight)
{
    U_ISP_FE_BLK_SIZE o_isp_fe_blk_size;
    o_isp_fe_blk_size.bits.isp_fe_blk_width = uIspFeBlkWidth;
    o_isp_fe_blk_size.bits.isp_fe_blk_height = uIspFeBlkHeight;
    pstFeReg->ISP_FE_BLK_SIZE.u32 = o_isp_fe_blk_size.u32;
}

/******************************************************************************
******************************************************************************
******************************************************************************
********************BE BE BE BE BE BE BE BE BE  ******************************
******************************************************************************
******************************************************************************
******************************************************************************/

static inline GK_VOID isp_be_module_pos_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAeSel, GK_U32 uIspAwbSel, GK_U32 uIspAfSel, GK_U32 uIspDisSel, GK_U32 uIspClutSel, GK_U32 uIspDcgSel)
{
    U_ISP_BE_MODULE_POS o_isp_be_module_pos;
    o_isp_be_module_pos.bits.isp_ae_sel = uIspAeSel;
    o_isp_be_module_pos.bits.isp_awb_sel = uIspAwbSel;
    o_isp_be_module_pos.bits.isp_af_sel = uIspAfSel;
    o_isp_be_module_pos.bits.isp_dis_sel = uIspDisSel;
    o_isp_be_module_pos.bits.isp_clut_sel = uIspClutSel;
    o_isp_be_module_pos.bits.isp_dcg_sel = uIspDcgSel;
    pstBeReg->ISP_BE_MODULE_POS.u32 = o_isp_be_module_pos.u32;
}

static inline GK_VOID isp_be_fstart_delay_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDelay)
{
    U_ISP_BE_FSTART_DELAY o_isp_be_fstart_delay;
    o_isp_be_fstart_delay.bits.isp_delay = uIspDelay;
    pstBeReg->ISP_BE_FSTART_DELAY.u32 = o_isp_be_fstart_delay.u32;
}

static inline GK_VOID isp_be_input_mux_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspInput0Sel, GK_U32 uIspInput1Sel, GK_U32 uIspInput2Sel, GK_U32 uIspInput3Sel, GK_U32 uIspInput4Sel)
{
    U_ISP_BE_INPUT_MUX o_isp_be_input_mux;
    o_isp_be_input_mux.bits.isp_input0_sel = uIspInput0Sel;
    o_isp_be_input_mux.bits.isp_input1_sel = uIspInput1Sel;
    o_isp_be_input_mux.bits.isp_input2_sel = uIspInput2Sel;
    o_isp_be_input_mux.bits.isp_input3_sel = uIspInput3Sel;
    o_isp_be_input_mux.bits.isp_input4_sel = uIspInput4Sel;
    pstBeReg->ISP_BE_INPUT_MUX.u32 = o_isp_be_input_mux.u32;
}

static inline GK_VOID isp_be_sys_ctrl_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSttEn)
{
    U_ISP_BE_SYS_CTRL o_isp_be_sys_ctrl;
    o_isp_be_sys_ctrl.bits.isp_stt_en = uIspSttEn;
    pstBeReg->ISP_BE_SYS_CTRL.u32 = o_isp_be_sys_ctrl.u32;
}

static inline GK_VOID isp_be_user_define0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspUserDefine0)
{
    U_ISP_BE_USER_DEFINE0 o_isp_be_user_define0;
    o_isp_be_user_define0.bits.isp_user_define0 = uIspUserDefine0;
    pstBeReg->ISP_BE_USER_DEFINE0.u32 = o_isp_be_user_define0.u32;
}

static inline GK_VOID isp_be_user_define1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspUserDefine1)
{
    U_ISP_BE_USER_DEFINE1 o_isp_be_user_define1;
    o_isp_be_user_define1.bits.isp_user_define1 = uIspUserDefine1;
    pstBeReg->ISP_BE_USER_DEFINE1.u32 = o_isp_be_user_define1.u32;
}

static inline GK_VOID isp_be_format_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspFormat)
{
    U_ISP_BE_FORMAT o_isp_be_format;
    o_isp_be_format.bits.isp_format = uIspFormat;
    pstBeReg->ISP_BE_FORMAT.u32 = o_isp_be_format.u32;
}

static inline GK_VOID isp_clip_y_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspClipYMin, GK_U32 uIspClipYMax)
{
    U_ISP_CLIP_Y_CFG o_isp_clip_y_cfg;
    o_isp_clip_y_cfg.bits.isp_clip_y_min = uIspClipYMin;
    o_isp_clip_y_cfg.bits.isp_clip_y_max = uIspClipYMax;
    pstBeReg->ISP_CLIP_Y_CFG.u32 = o_isp_clip_y_cfg.u32;
}

static inline GK_VOID isp_clip_c_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspClipCMin, GK_U32 uIspClipCMax)
{
    U_ISP_CLIP_C_CFG o_isp_clip_c_cfg;
    o_isp_clip_c_cfg.bits.isp_clip_c_min = uIspClipCMin;
    o_isp_clip_c_cfg.bits.isp_clip_c_max = uIspClipCMax;
    pstBeReg->ISP_CLIP_C_CFG.u32 = o_isp_clip_c_cfg.u32;
}

static inline GK_VOID isp_csc_sum_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCscSumEn)
{
    U_ISP_CSC_SUM_CFG o_isp_csc_sum_cfg;
    o_isp_csc_sum_cfg.bits.isp_csc_sum_en = uIspCscSumEn;
    pstBeReg->ISP_CSC_SUM_CFG.u32 = o_isp_csc_sum_cfg.u32;
}

static inline GK_VOID isp_yuv444_sum_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspYuv444SumEn)
{
    U_ISP_YUV444_SUM_CFG o_isp_yuv444_sum_cfg;
    o_isp_yuv444_sum_cfg.bits.isp_yuv444_sum_en = uIspYuv444SumEn;
    pstBeReg->ISP_YUV444_SUM_CFG.u32 = o_isp_yuv444_sum_cfg.u32;
}

static inline GK_VOID isp_yuv422_sum_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspYuv422SumEn)
{
    U_ISP_YUV422_SUM_CFG o_isp_yuv422_sum_cfg;
    o_isp_yuv422_sum_cfg.bits.isp_yuv422_sum_en = uIspYuv422SumEn;
    pstBeReg->ISP_YUV422_SUM_CFG.u32 = o_isp_yuv422_sum_cfg.u32;
}

static inline GK_VOID isp_wdr_sum_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrSumEn)
{
    U_ISP_WDR_SUM_CFG o_isp_wdr_sum_cfg;
    o_isp_wdr_sum_cfg.bits.isp_wdr_sum_en = uIspWdrSumEn;
    pstBeReg->ISP_WDR_SUM_CFG.u32 = o_isp_wdr_sum_cfg.u32;
}

static inline GK_VOID isp_demosaic_sum_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicSumEn)
{
    U_ISP_DEMOSAIC_SUM_CFG o_isp_demosaic_sum_cfg;
    o_isp_demosaic_sum_cfg.bits.isp_demosaic_sum_en = uIspDemosaicSumEn;
    pstBeReg->ISP_DEMOSAIC_SUM_CFG.u32 = o_isp_demosaic_sum_cfg.u32;
}

static inline GK_VOID isp_dmnr_dither_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDmnrDitherEn, GK_U32 uIspDmnrDitherRound, GK_U32 uIspDmnrDitherSpatialMode, GK_U32 uIspDmnrDitherOutBits)
{
    U_ISP_DMNR_DITHER o_isp_dmnr_dither;
    o_isp_dmnr_dither.bits.isp_dmnr_dither_en = uIspDmnrDitherEn;
    o_isp_dmnr_dither.bits.isp_dmnr_dither_round = uIspDmnrDitherRound;
    o_isp_dmnr_dither.bits.isp_dmnr_dither_spatial_mode = uIspDmnrDitherSpatialMode;
    o_isp_dmnr_dither.bits.isp_dmnr_dither_out_bits = uIspDmnrDitherOutBits;
    pstBeReg->ISP_DMNR_DITHER.u32 = o_isp_dmnr_dither.u32;
}

static inline GK_VOID isp_acm_dither_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAcmDitherEn, GK_U32 uIspAcmDitherRound, GK_U32 uIspAcmDitherSpatialMode, GK_U32 uIspAcmDitherOutBits)
{
    U_ISP_ACM_DITHER o_isp_acm_dither;
    o_isp_acm_dither.bits.isp_acm_dither_en = uIspAcmDitherEn;
    o_isp_acm_dither.bits.isp_acm_dither_round = uIspAcmDitherRound;
    o_isp_acm_dither.bits.isp_acm_dither_spatial_mode = uIspAcmDitherSpatialMode;
    o_isp_acm_dither.bits.isp_acm_dither_out_bits = uIspAcmDitherOutBits;
    pstBeReg->ISP_ACM_DITHER.u32 = o_isp_acm_dither.u32;
}

static inline GK_VOID isp_drc_dither_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcDitherEn, GK_U32 uIspDrcDitherRound, GK_U32 uIspDrcDitherSpatialMode, GK_U32 uIspDrcDitherOutBits)
{
    U_ISP_DRC_DITHER o_isp_drc_dither;
    o_isp_drc_dither.bits.isp_drc_dither_en = uIspDrcDitherEn;
    o_isp_drc_dither.bits.isp_drc_dither_round = uIspDrcDitherRound;
    o_isp_drc_dither.bits.isp_drc_dither_spatial_mode = uIspDrcDitherSpatialMode;
    o_isp_drc_dither.bits.isp_drc_dither_out_bits = uIspDrcDitherOutBits;
    pstBeReg->ISP_DRC_DITHER.u32 = o_isp_drc_dither.u32;
}

static inline GK_VOID isp_sqrt1_dither_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSqrt1DitherEn, GK_U32 uIspSqrt1DitherRound, GK_U32 uIspSqrt1DitherSpatialMode, GK_U32 uIspSqrt1DitherOutBits)
{
    U_ISP_SQRT1_DITHER o_isp_sqrt1_dither;
    o_isp_sqrt1_dither.bits.isp_sqrt1_dither_en = uIspSqrt1DitherEn;
    o_isp_sqrt1_dither.bits.isp_sqrt1_dither_round = uIspSqrt1DitherRound;
    o_isp_sqrt1_dither.bits.isp_sqrt1_dither_spatial_mode = uIspSqrt1DitherSpatialMode;
    o_isp_sqrt1_dither.bits.isp_sqrt1_dither_out_bits = uIspSqrt1DitherOutBits;
    pstBeReg->ISP_SQRT1_DITHER.u32 = o_isp_sqrt1_dither.u32;
}

static inline GK_VOID isp_blk_hblank_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBlkBHblank, GK_U32 uIspBlkFHblank)
{
    U_ISP_BLK_HBLANK o_isp_blk_hblank;
    o_isp_blk_hblank.bits.isp_blk_b_hblank = uIspBlkBHblank;
    o_isp_blk_hblank.bits.isp_blk_f_hblank = uIspBlkFHblank;
    pstBeReg->ISP_BLK_HBLANK.u32 = o_isp_blk_hblank.u32;
}

static inline GK_VOID isp_blk_vblank_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBlkBVblank, GK_U32 uIspBlkFVblank)
{
    U_ISP_BLK_VBLANK o_isp_blk_vblank;
    o_isp_blk_vblank.bits.isp_blk_b_vblank = uIspBlkBVblank;
    o_isp_blk_vblank.bits.isp_blk_f_vblank = uIspBlkFVblank;
    pstBeReg->ISP_BLK_VBLANK.u32 = o_isp_blk_vblank.u32;
}

static inline GK_VOID isp_blc_offset1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBlcOfsgr, GK_U32 uIspBlcOfsr)
{
    U_ISP_BLC_OFFSET1 o_isp_blc_offset1;
    o_isp_blc_offset1.bits.isp_blc_ofsgr = uIspBlcOfsgr;
    o_isp_blc_offset1.bits.isp_blc_ofsr = uIspBlcOfsr;
    pstBeReg->ISP_BLC_OFFSET1.u32 = o_isp_blc_offset1.u32;
}

static inline GK_VOID isp_blc_offset2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBlcOfsgb, GK_U32 uIspBlcOfsb)
{
    U_ISP_BLC_OFFSET2 o_isp_blc_offset2;
    o_isp_blc_offset2.bits.isp_blc_ofsgb = uIspBlcOfsgb;
    o_isp_blc_offset2.bits.isp_blc_ofsb = uIspBlcOfsb;
    pstBeReg->ISP_BLC_OFFSET2.u32 = o_isp_blc_offset2.u32;
}

static inline GK_VOID isp_wb_blc_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWbEnIn, GK_U32 uIspWbEnOut)
{
    U_ISP_WB_BLC_CFG o_isp_wb_blc_cfg;
    o_isp_wb_blc_cfg.bits.isp_wb_en_in = uIspWbEnIn;
    o_isp_wb_blc_cfg.bits.isp_wb_en_out = uIspWbEnOut;
    pstBeReg->ISP_WB_BLC_CFG.u32 = o_isp_wb_blc_cfg.u32;
}

static inline GK_VOID isp_wb_gain1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWbGrgain, GK_U32 uIspWbRgain)
{
    U_ISP_WB_GAIN1 o_isp_wb_gain1;
    o_isp_wb_gain1.bits.isp_wb_grgain = uIspWbGrgain;
    o_isp_wb_gain1.bits.isp_wb_rgain = uIspWbRgain;
    pstBeReg->ISP_WB_GAIN1.u32 = o_isp_wb_gain1.u32;
}

static inline GK_VOID isp_wb_gain2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWbGbgain, GK_U32 uIspWbBgain)
{
    U_ISP_WB_GAIN2 o_isp_wb_gain2;
    o_isp_wb_gain2.bits.isp_wb_gbgain = uIspWbGbgain;
    o_isp_wb_gain2.bits.isp_wb_bgain = uIspWbBgain;
    pstBeReg->ISP_WB_GAIN2.u32 = o_isp_wb_gain2.u32;
}

static inline GK_VOID isp_wb_blc_offset1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWbOfsgr, GK_U32 uIspWbOfsr)
{
    U_ISP_WB_BLC_OFFSET1 o_isp_wb_blc_offset1;
    o_isp_wb_blc_offset1.bits.isp_wb_ofsgr = uIspWbOfsgr;
    o_isp_wb_blc_offset1.bits.isp_wb_ofsr = uIspWbOfsr;
    pstBeReg->ISP_WB_BLC_OFFSET1.u32 = o_isp_wb_blc_offset1.u32;
}

static inline GK_VOID isp_wb_blc_offset2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWbOfsgb, GK_U32 uIspWbOfsb)
{
    U_ISP_WB_BLC_OFFSET2 o_isp_wb_blc_offset2;
    o_isp_wb_blc_offset2.bits.isp_wb_ofsgb = uIspWbOfsgb;
    o_isp_wb_blc_offset2.bits.isp_wb_ofsb = uIspWbOfsb;
    pstBeReg->ISP_WB_BLC_OFFSET2.u32 = o_isp_wb_blc_offset2.u32;
}

static inline GK_VOID isp_wb_clip_value_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWbClipValue)
{
    U_ISP_WB_CLIP_VALUE o_isp_wb_clip_value;
    o_isp_wb_clip_value.bits.isp_wb_clip_value = uIspWbClipValue;
    pstBeReg->ISP_WB_CLIP_VALUE.u32 = o_isp_wb_clip_value.u32;
}

static inline GK_VOID isp_dg_blc_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDgEnIn, GK_U32 uIspDgEnOut)
{
    U_ISP_DG_BLC_CFG o_isp_dg_blc_cfg;
    o_isp_dg_blc_cfg.bits.isp_dg_en_in = uIspDgEnIn;
    o_isp_dg_blc_cfg.bits.isp_dg_en_out = uIspDgEnOut;
    pstBeReg->ISP_DG_BLC_CFG.u32 = o_isp_dg_blc_cfg.u32;
}

static inline GK_VOID isp_dg_gain1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDgGrgain, GK_U32 uIspDgRgain)
{
    U_ISP_DG_GAIN1 o_isp_dg_gain1;
    o_isp_dg_gain1.bits.isp_dg_grgain = uIspDgGrgain;
    o_isp_dg_gain1.bits.isp_dg_rgain = uIspDgRgain;
    pstBeReg->ISP_DG_GAIN1.u32 = o_isp_dg_gain1.u32;
}

static inline GK_VOID isp_dg_gain2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDgGbgain, GK_U32 uIspDgBgain)
{
    U_ISP_DG_GAIN2 o_isp_dg_gain2;
    o_isp_dg_gain2.bits.isp_dg_gbgain = uIspDgGbgain;
    o_isp_dg_gain2.bits.isp_dg_bgain = uIspDgBgain;
    pstBeReg->ISP_DG_GAIN2.u32 = o_isp_dg_gain2.u32;
}

static inline GK_VOID isp_dg_blc_offset1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDgOfsgr, GK_U32 uIspDgOfsr)
{
    U_ISP_DG_BLC_OFFSET1 o_isp_dg_blc_offset1;
    o_isp_dg_blc_offset1.bits.isp_dg_ofsgr = uIspDgOfsgr;
    o_isp_dg_blc_offset1.bits.isp_dg_ofsr = uIspDgOfsr;
    pstBeReg->ISP_DG_BLC_OFFSET1.u32 = o_isp_dg_blc_offset1.u32;
}

static inline GK_VOID isp_dg_blc_offset2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDgOfsgb, GK_U32 uIspDgOfsb)
{
    U_ISP_DG_BLC_OFFSET2 o_isp_dg_blc_offset2;
    o_isp_dg_blc_offset2.bits.isp_dg_ofsgb = uIspDgOfsgb;
    o_isp_dg_blc_offset2.bits.isp_dg_ofsb = uIspDgOfsb;
    pstBeReg->ISP_DG_BLC_OFFSET2.u32 = o_isp_dg_blc_offset2.u32;
}

static inline GK_VOID isp_dg_clip_value_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDgClipValue)
{
    U_ISP_DG_CLIP_VALUE o_isp_dg_clip_value;
    o_isp_dg_clip_value.bits.isp_dg_clip_value = uIspDgClipValue;
    pstBeReg->ISP_DG_CLIP_VALUE.u32 = o_isp_dg_clip_value.u32;
}

static inline GK_VOID isp_cc_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCcColortoneEn, GK_U32 uIspCcRecoverEn)
{
    U_ISP_CC_CFG o_isp_cc_cfg;
    o_isp_cc_cfg.bits.isp_cc_colortone_en = uIspCcColortoneEn;
    o_isp_cc_cfg.bits.isp_cc_recover_en = uIspCcRecoverEn;
    pstBeReg->ISP_CC_CFG.u32 = o_isp_cc_cfg.u32;
}

static inline GK_VOID isp_cc_coef0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCcCoef00, GK_U32 uIspCcCoef01)
{
    U_ISP_CC_COEF0 o_isp_cc_coef0;
    o_isp_cc_coef0.bits.isp_cc_coef00 = uIspCcCoef00;
    o_isp_cc_coef0.bits.isp_cc_coef01 = uIspCcCoef01;
    pstBeReg->ISP_CC_COEF0.u32 = o_isp_cc_coef0.u32;
}

static inline GK_VOID isp_cc_coef1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCcCoef02, GK_U32 uIspCcCoef10)
{
    U_ISP_CC_COEF1 o_isp_cc_coef1;
    o_isp_cc_coef1.bits.isp_cc_coef02 = uIspCcCoef02;
    o_isp_cc_coef1.bits.isp_cc_coef10 = uIspCcCoef10;
    pstBeReg->ISP_CC_COEF1.u32 = o_isp_cc_coef1.u32;
}

static inline GK_VOID isp_cc_coef2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCcCoef11, GK_U32 uIspCcCoef12)
{
    U_ISP_CC_COEF2 o_isp_cc_coef2;
    o_isp_cc_coef2.bits.isp_cc_coef11 = uIspCcCoef11;
    o_isp_cc_coef2.bits.isp_cc_coef12 = uIspCcCoef12;
    pstBeReg->ISP_CC_COEF2.u32 = o_isp_cc_coef2.u32;
}

static inline GK_VOID isp_cc_coef3_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCcCoef20, GK_U32 uIspCcCoef21)
{
    U_ISP_CC_COEF3 o_isp_cc_coef3;
    o_isp_cc_coef3.bits.isp_cc_coef20 = uIspCcCoef20;
    o_isp_cc_coef3.bits.isp_cc_coef21 = uIspCcCoef21;
    pstBeReg->ISP_CC_COEF3.u32 = o_isp_cc_coef3.u32;
}

static inline GK_VOID isp_cc_coef4_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCcCoef22)
{
    U_ISP_CC_COEF4 o_isp_cc_coef4;
    o_isp_cc_coef4.bits.isp_cc_coef22 = uIspCcCoef22;
    pstBeReg->ISP_CC_COEF4.u32 = o_isp_cc_coef4.u32;
}

static inline GK_VOID isp_cc_rcv_ctrl0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCcSoftClip0Step, GK_U32 uIspCcSoftClip1Step)
{
    U_ISP_CC_RCV_CTRL0 o_isp_cc_rcv_ctrl0;
    o_isp_cc_rcv_ctrl0.bits.isp_cc_soft_clip0_step = uIspCcSoftClip0Step;
    o_isp_cc_rcv_ctrl0.bits.isp_cc_soft_clip1_step = uIspCcSoftClip1Step;
    pstBeReg->ISP_CC_RCV_CTRL0.u32 = o_isp_cc_rcv_ctrl0.u32;
}

static inline GK_VOID isp_cc_rcv_ctrl1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCcDarkprev, GK_U32 uIspCcPeaksuppSat, GK_U32 uIspCcPeaksuppMax)
{
    U_ISP_CC_RCV_CTRL1 o_isp_cc_rcv_ctrl1;
    o_isp_cc_rcv_ctrl1.bits.isp_cc_darkprev = uIspCcDarkprev;
    o_isp_cc_rcv_ctrl1.bits.isp_cc_peaksupp_sat = uIspCcPeaksuppSat;
    o_isp_cc_rcv_ctrl1.bits.isp_cc_peaksupp_max = uIspCcPeaksuppMax;
    pstBeReg->ISP_CC_RCV_CTRL1.u32 = o_isp_cc_rcv_ctrl1.u32;
}

static inline GK_VOID isp_cc_lumafact_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCcLumaCoefr, GK_U32 uIspCcLumaCoefb, GK_U32 uIspCcLumaCoefrUp, GK_U32 uIspCcLumaCoefbUp)
{
    U_ISP_CC_LUMAFACT o_isp_cc_lumafact;
    o_isp_cc_lumafact.bits.isp_cc_luma_coefr = uIspCcLumaCoefr;
    o_isp_cc_lumafact.bits.isp_cc_luma_coefb = uIspCcLumaCoefb;
    o_isp_cc_lumafact.bits.isp_cc_luma_coefr_up = uIspCcLumaCoefrUp;
    o_isp_cc_lumafact.bits.isp_cc_luma_coefb_up = uIspCcLumaCoefbUp;
    pstBeReg->ISP_CC_LUMAFACT.u32 = o_isp_cc_lumafact.u32;
}

static inline GK_VOID isp_cc_colortone_rb_gain_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCcBGain, GK_U32 uIspCcRGain)
{
    U_ISP_CC_COLORTONE_RB_GAIN o_isp_cc_colortone_rb_gain;
    o_isp_cc_colortone_rb_gain.bits.isp_cc_b_gain = uIspCcBGain;
    o_isp_cc_colortone_rb_gain.bits.isp_cc_r_gain = uIspCcRGain;
    pstBeReg->ISP_CC_COLORTONE_RB_GAIN.u32 = o_isp_cc_colortone_rb_gain.u32;
}

static inline GK_VOID isp_cc_colortone_g_gain_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCcGGain)
{
    U_ISP_CC_COLORTONE_G_GAIN o_isp_cc_colortone_g_gain;
    o_isp_cc_colortone_g_gain.bits.isp_cc_g_gain = uIspCcGGain;
    pstBeReg->ISP_CC_COLORTONE_G_GAIN.u32 = o_isp_cc_colortone_g_gain.u32;
}

static inline GK_VOID isp_cc_in_dc0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCcInDc0)
{
    U_ISP_CC_IN_DC0 o_isp_cc_in_dc0;
    o_isp_cc_in_dc0.bits.isp_cc_in_dc0 = uIspCcInDc0;
    pstBeReg->ISP_CC_IN_DC0.u32 = o_isp_cc_in_dc0.u32;
}

static inline GK_VOID isp_cc_in_dc1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCcInDc1)
{
    U_ISP_CC_IN_DC1 o_isp_cc_in_dc1;
    o_isp_cc_in_dc1.bits.isp_cc_in_dc1 = uIspCcInDc1;
    pstBeReg->ISP_CC_IN_DC1.u32 = o_isp_cc_in_dc1.u32;
}

static inline GK_VOID isp_cc_in_dc2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCcInDc2)
{
    U_ISP_CC_IN_DC2 o_isp_cc_in_dc2;
    o_isp_cc_in_dc2.bits.isp_cc_in_dc2 = uIspCcInDc2;
    pstBeReg->ISP_CC_IN_DC2.u32 = o_isp_cc_in_dc2.u32;
}

static inline GK_VOID isp_cc_out_dc0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCcOutDc0)
{
    U_ISP_CC_OUT_DC0 o_isp_cc_out_dc0;
    o_isp_cc_out_dc0.bits.isp_cc_out_dc0 = uIspCcOutDc0;
    pstBeReg->ISP_CC_OUT_DC0.u32 = o_isp_cc_out_dc0.u32;
}

static inline GK_VOID isp_cc_out_dc1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCcOutDc1)
{
    U_ISP_CC_OUT_DC1 o_isp_cc_out_dc1;
    o_isp_cc_out_dc1.bits.isp_cc_out_dc1 = uIspCcOutDc1;
    pstBeReg->ISP_CC_OUT_DC1.u32 = o_isp_cc_out_dc1.u32;
}

static inline GK_VOID isp_cc_out_dc2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCcOutDc2)
{
    U_ISP_CC_OUT_DC2 o_isp_cc_out_dc2;
    o_isp_cc_out_dc2.bits.isp_cc_out_dc2 = uIspCcOutDc2;
    pstBeReg->ISP_CC_OUT_DC2.u32 = o_isp_cc_out_dc2.u32;
}

static inline GK_VOID isp_csc_coef0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCscCoef00, GK_U32 uIspCscCoef01)
{
    U_ISP_CSC_COEF0 o_isp_csc_coef0;
    o_isp_csc_coef0.bits.isp_csc_coef00 = uIspCscCoef00;
    o_isp_csc_coef0.bits.isp_csc_coef01 = uIspCscCoef01;
    pstBeReg->ISP_CSC_COEF0.u32 = o_isp_csc_coef0.u32;
}

static inline GK_VOID isp_csc_coef1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCscCoef02, GK_U32 uIspCscCoef10)
{
    U_ISP_CSC_COEF1 o_isp_csc_coef1;
    o_isp_csc_coef1.bits.isp_csc_coef02 = uIspCscCoef02;
    o_isp_csc_coef1.bits.isp_csc_coef10 = uIspCscCoef10;
    pstBeReg->ISP_CSC_COEF1.u32 = o_isp_csc_coef1.u32;
}

static inline GK_VOID isp_csc_coef2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCscCoef11, GK_U32 uIspCscCoef12)
{
    U_ISP_CSC_COEF2 o_isp_csc_coef2;
    o_isp_csc_coef2.bits.isp_csc_coef11 = uIspCscCoef11;
    o_isp_csc_coef2.bits.isp_csc_coef12 = uIspCscCoef12;
    pstBeReg->ISP_CSC_COEF2.u32 = o_isp_csc_coef2.u32;
}

static inline GK_VOID isp_csc_coef3_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCscCoef20, GK_U32 uIspCscCoef21)
{
    U_ISP_CSC_COEF3 o_isp_csc_coef3;
    o_isp_csc_coef3.bits.isp_csc_coef20 = uIspCscCoef20;
    o_isp_csc_coef3.bits.isp_csc_coef21 = uIspCscCoef21;
    pstBeReg->ISP_CSC_COEF3.u32 = o_isp_csc_coef3.u32;
}

static inline GK_VOID isp_csc_coef4_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCscCoef22)
{
    U_ISP_CSC_COEF4 o_isp_csc_coef4;
    o_isp_csc_coef4.bits.isp_csc_coef22 = uIspCscCoef22;
    pstBeReg->ISP_CSC_COEF4.u32 = o_isp_csc_coef4.u32;
}

static inline GK_VOID isp_csc_in_dc0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCscInDc0, GK_U32 uIspCscInDc1)
{
    U_ISP_CSC_IN_DC0 o_isp_csc_in_dc0;
    o_isp_csc_in_dc0.bits.isp_csc_in_dc0 = uIspCscInDc0;
    o_isp_csc_in_dc0.bits.isp_csc_in_dc1 = uIspCscInDc1;
    pstBeReg->ISP_CSC_IN_DC0.u32 = o_isp_csc_in_dc0.u32;
}

static inline GK_VOID isp_csc_in_dc1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCscInDc2)
{
    U_ISP_CSC_IN_DC1 o_isp_csc_in_dc1;
    o_isp_csc_in_dc1.bits.isp_csc_in_dc2 = uIspCscInDc2;
    pstBeReg->ISP_CSC_IN_DC1.u32 = o_isp_csc_in_dc1.u32;
}

static inline GK_VOID isp_csc_out_dc0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCscOutDc0, GK_U32 uIspCscOutDc1)
{
    U_ISP_CSC_OUT_DC0 o_isp_csc_out_dc0;
    o_isp_csc_out_dc0.bits.isp_csc_out_dc0 = uIspCscOutDc0;
    o_isp_csc_out_dc0.bits.isp_csc_out_dc1 = uIspCscOutDc1;
    pstBeReg->ISP_CSC_OUT_DC0.u32 = o_isp_csc_out_dc0.u32;
}

static inline GK_VOID isp_csc_out_dc1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCscOutDc2)
{
    U_ISP_CSC_OUT_DC1 o_isp_csc_out_dc1;
    o_isp_csc_out_dc1.bits.isp_csc_out_dc2 = uIspCscOutDc2;
    pstBeReg->ISP_CSC_OUT_DC1.u32 = o_isp_csc_out_dc1.u32;
}

static inline GK_VOID isp_hcds_coefh0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspHcdsCoefh1, GK_U32 uIspHcdsCoefh0)
{
    U_ISP_HCDS_COEFH0 o_isp_hcds_coefh0;
    o_isp_hcds_coefh0.bits.isp_hcds_coefh1 = uIspHcdsCoefh1;
    o_isp_hcds_coefh0.bits.isp_hcds_coefh0 = uIspHcdsCoefh0;
    pstBeReg->ISP_HCDS_COEFH0.u32 = o_isp_hcds_coefh0.u32;
}

static inline GK_VOID isp_hcds_coefh1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspHcdsCoefh3, GK_U32 uIspHcdsCoefh2)
{
    U_ISP_HCDS_COEFH1 o_isp_hcds_coefh1;
    o_isp_hcds_coefh1.bits.isp_hcds_coefh3 = uIspHcdsCoefh3;
    o_isp_hcds_coefh1.bits.isp_hcds_coefh2 = uIspHcdsCoefh2;
    pstBeReg->ISP_HCDS_COEFH1.u32 = o_isp_hcds_coefh1.u32;
}

static inline GK_VOID isp_hcds_coefh2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspHcdsCoefh5, GK_U32 uIspHcdsCoefh4)
{
    U_ISP_HCDS_COEFH2 o_isp_hcds_coefh2;
    o_isp_hcds_coefh2.bits.isp_hcds_coefh5 = uIspHcdsCoefh5;
    o_isp_hcds_coefh2.bits.isp_hcds_coefh4 = uIspHcdsCoefh4;
    pstBeReg->ISP_HCDS_COEFH2.u32 = o_isp_hcds_coefh2.u32;
}

static inline GK_VOID isp_hcds_coefh3_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspHcdsCoefh7, GK_U32 uIspHcdsCoefh6)
{
    U_ISP_HCDS_COEFH3 o_isp_hcds_coefh3;
    o_isp_hcds_coefh3.bits.isp_hcds_coefh7 = uIspHcdsCoefh7;
    o_isp_hcds_coefh3.bits.isp_hcds_coefh6 = uIspHcdsCoefh6;
    pstBeReg->ISP_HCDS_COEFH3.u32 = o_isp_hcds_coefh3.u32;
}

static inline GK_VOID isp_4dg_blc_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIsp4dgEnIn, GK_U32 uIsp4dgEnOut)
{
    U_ISP_4DG_BLC_CFG o_isp_4dg_blc_cfg;
    o_isp_4dg_blc_cfg.bits.isp_4dg_en_in = uIsp4dgEnIn;
    o_isp_4dg_blc_cfg.bits.isp_4dg_en_out = uIsp4dgEnOut;
    pstBeReg->ISP_4DG_BLC_CFG.u32 = o_isp_4dg_blc_cfg.u32;
}

static inline GK_VOID isp_4dg_0_gain1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIsp4dg0Grgain, GK_U32 uIsp4dg0Rgain)
{
    U_ISP_4DG_0_GAIN1 o_isp_4dg_0_gain1;
    o_isp_4dg_0_gain1.bits.isp_4dg0_grgain = uIsp4dg0Grgain;
    o_isp_4dg_0_gain1.bits.isp_4dg0_rgain = uIsp4dg0Rgain;
    pstBeReg->ISP_4DG_0_GAIN1.u32 = o_isp_4dg_0_gain1.u32;
}

static inline GK_VOID isp_4dg_0_gain2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIsp4dg0Gbgain, GK_U32 uIsp4dg0Bgain)
{
    U_ISP_4DG_0_GAIN2 o_isp_4dg_0_gain2;
    o_isp_4dg_0_gain2.bits.isp_4dg0_gbgain = uIsp4dg0Gbgain;
    o_isp_4dg_0_gain2.bits.isp_4dg0_bgain = uIsp4dg0Bgain;
    pstBeReg->ISP_4DG_0_GAIN2.u32 = o_isp_4dg_0_gain2.u32;
}

static inline GK_VOID isp_4dg_0_blc_offset1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIsp4dg0Ofsgr, GK_U32 uIsp4dg0Ofsr)
{
    U_ISP_4DG_0_BLC_OFFSET1 o_isp_4dg_0_blc_offset1;
    o_isp_4dg_0_blc_offset1.bits.isp_4dg0_ofsgr = uIsp4dg0Ofsgr;
    o_isp_4dg_0_blc_offset1.bits.isp_4dg0_ofsr = uIsp4dg0Ofsr;
    pstBeReg->ISP_4DG_0_BLC_OFFSET1.u32 = o_isp_4dg_0_blc_offset1.u32;
}

static inline GK_VOID isp_4dg_0_blc_offset2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIsp4dg0Ofsgb, GK_U32 uIsp4dg0Ofsb)
{
    U_ISP_4DG_0_BLC_OFFSET2 o_isp_4dg_0_blc_offset2;
    o_isp_4dg_0_blc_offset2.bits.isp_4dg0_ofsgb = uIsp4dg0Ofsgb;
    o_isp_4dg_0_blc_offset2.bits.isp_4dg0_ofsb = uIsp4dg0Ofsb;
    pstBeReg->ISP_4DG_0_BLC_OFFSET2.u32 = o_isp_4dg_0_blc_offset2.u32;
}

static inline GK_VOID isp_4dg_1_gain1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIsp4dg1Grgain, GK_U32 uIsp4dg1Rgain)
{
    U_ISP_4DG_1_GAIN1 o_isp_4dg_1_gain1;
    o_isp_4dg_1_gain1.bits.isp_4dg1_grgain = uIsp4dg1Grgain;
    o_isp_4dg_1_gain1.bits.isp_4dg1_rgain = uIsp4dg1Rgain;
    pstBeReg->ISP_4DG_1_GAIN1.u32 = o_isp_4dg_1_gain1.u32;
}

static inline GK_VOID isp_4dg_1_gain2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIsp4dg1Gbgain, GK_U32 uIsp4dg1Bgain)
{
    U_ISP_4DG_1_GAIN2 o_isp_4dg_1_gain2;
    o_isp_4dg_1_gain2.bits.isp_4dg1_gbgain = uIsp4dg1Gbgain;
    o_isp_4dg_1_gain2.bits.isp_4dg1_bgain = uIsp4dg1Bgain;
    pstBeReg->ISP_4DG_1_GAIN2.u32 = o_isp_4dg_1_gain2.u32;
}

static inline GK_VOID isp_4dg_1_blc_offset1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIsp4dg1Ofsgr, GK_U32 uIsp4dg1Ofsr)
{
    U_ISP_4DG_1_BLC_OFFSET1 o_isp_4dg_1_blc_offset1;
    o_isp_4dg_1_blc_offset1.bits.isp_4dg1_ofsgr = uIsp4dg1Ofsgr;
    o_isp_4dg_1_blc_offset1.bits.isp_4dg1_ofsr = uIsp4dg1Ofsr;
    pstBeReg->ISP_4DG_1_BLC_OFFSET1.u32 = o_isp_4dg_1_blc_offset1.u32;
}

static inline GK_VOID isp_4dg_1_blc_offset2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIsp4dg1Ofsgb, GK_U32 uIsp4dg1Ofsb)
{
    U_ISP_4DG_1_BLC_OFFSET2 o_isp_4dg_1_blc_offset2;
    o_isp_4dg_1_blc_offset2.bits.isp_4dg1_ofsgb = uIsp4dg1Ofsgb;
    o_isp_4dg_1_blc_offset2.bits.isp_4dg1_ofsb = uIsp4dg1Ofsb;
    pstBeReg->ISP_4DG_1_BLC_OFFSET2.u32 = o_isp_4dg_1_blc_offset2.u32;
}

static inline GK_VOID isp_4dg_0_clip_value_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIsp4dg0ClipValue)
{
    U_ISP_4DG_0_CLIP_VALUE o_isp_4dg_0_clip_value;
    o_isp_4dg_0_clip_value.bits.isp_4dg0_clip_value = uIsp4dg0ClipValue;
    pstBeReg->ISP_4DG_0_CLIP_VALUE.u32 = o_isp_4dg_0_clip_value.u32;
}

static inline GK_VOID isp_4dg_1_clip_value_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIsp4dg1ClipValue)
{
    U_ISP_4DG_1_CLIP_VALUE o_isp_4dg_1_clip_value;
    o_isp_4dg_1_clip_value.bits.isp_4dg1_clip_value = uIsp4dg1ClipValue;
    pstBeReg->ISP_4DG_1_CLIP_VALUE.u32 = o_isp_4dg_1_clip_value.u32;
}

static inline GK_VOID isp_ae_zone_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAeHnum, GK_U32 uIspAeVnum)
{
    U_ISP_AE_ZONE o_isp_ae_zone;
    o_isp_ae_zone.bits.isp_ae_hnum = uIspAeHnum;
    o_isp_ae_zone.bits.isp_ae_vnum = uIspAeVnum;
    pstBeReg->ISP_AE_ZONE.u32 = o_isp_ae_zone.u32;
}

static inline GK_VOID isp_ae_skip_crg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAeSkipX, GK_U32 uIspAeOffsetX, GK_U32 uIspAeSkipY, GK_U32 uIspAeOffsetY)
{
    U_ISP_AE_SKIP_CRG o_isp_ae_skip_crg;
    o_isp_ae_skip_crg.bits.isp_ae_skip_x = uIspAeSkipX;
    o_isp_ae_skip_crg.bits.isp_ae_offset_x = uIspAeOffsetX;
    o_isp_ae_skip_crg.bits.isp_ae_skip_y = uIspAeSkipY;
    o_isp_ae_skip_crg.bits.isp_ae_offset_y = uIspAeOffsetY;
    pstBeReg->ISP_AE_SKIP_CRG.u32 = o_isp_ae_skip_crg.u32;
}

static inline GK_VOID isp_ae_bitmove_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAeBitmove, GK_U32 uIspAeHistGammaMode, GK_U32 uIspAeAverGammaMode, GK_U32 uIspAeBlcEn, GK_U32 uIspAeGammaLimit, GK_U32 uIspAeFourplanemode)
{
    U_ISP_AE_BITMOVE o_isp_ae_bitmove;
    o_isp_ae_bitmove.bits.isp_ae_bitmove = uIspAeBitmove;
    o_isp_ae_bitmove.bits.isp_aest_gamma_mode = uIspAeHistGammaMode;
    o_isp_ae_bitmove.bits.isp_ae_aver_gamma_mode = uIspAeAverGammaMode;
    o_isp_ae_bitmove.bits.isp_ae_blc_en = uIspAeBlcEn;
    o_isp_ae_bitmove.bits.isp_ae_gamma_limit = uIspAeGammaLimit;
    o_isp_ae_bitmove.bits.isp_ae_fourplanemode = uIspAeFourplanemode;
    pstBeReg->ISP_AE_BITMOVE.u32 = o_isp_ae_bitmove.u32;
}

static inline GK_VOID isp_ae_offset_r_gr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAeOffsetR, GK_U32 uIspAeOffsetGr)
{
    U_ISP_AE_OFFSET_R_GR o_isp_ae_offset_r_gr;
    o_isp_ae_offset_r_gr.bits.isp_ae_offset_r = uIspAeOffsetR;
    o_isp_ae_offset_r_gr.bits.isp_ae_offset_gr = uIspAeOffsetGr;
    pstBeReg->ISP_AE_OFFSET_R_GR.u32 = o_isp_ae_offset_r_gr.u32;
}

static inline GK_VOID isp_ae_offset_gb_b_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAeOffsetGb, GK_U32 uIspAeOffsetB)
{
    U_ISP_AE_OFFSET_GB_B o_isp_ae_offset_gb_b;
    o_isp_ae_offset_gb_b.bits.isp_ae_offset_gb = uIspAeOffsetGb;
    o_isp_ae_offset_gb_b.bits.isp_ae_offset_b = uIspAeOffsetB;
    pstBeReg->ISP_AE_OFFSET_GB_B.u32 = o_isp_ae_offset_gb_b.u32;
}

static inline GK_VOID isp_ae_lut_update_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAeLutUpdate)
{
    U_ISP_AE_LUT_UPDATE o_isp_ae_lut_update;
    o_isp_ae_lut_update.bits.isp_ae_lut_update = uIspAeLutUpdate;
    pstBeReg->ISP_AE_LUT_UPDATE.u32 = o_isp_ae_lut_update.u32;
}

static inline GK_VOID isp_ae_stt_bst_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAeSttSize, GK_U32 uIspAeSttBst, GK_U32 uIspAeSttEn)
{
    U_ISP_AE_STT_BST o_isp_ae_stt_bst;
    o_isp_ae_stt_bst.bits.isp_ae_stt_size = uIspAeSttSize;
    o_isp_ae_stt_bst.bits.isp_ae_stt_bst = uIspAeSttBst;
    o_isp_ae_stt_bst.bits.isp_ae_stt_en = uIspAeSttEn;
    pstBeReg->ISP_AE_STT_BST.u32 = o_isp_ae_stt_bst.u32;
}

static inline GK_VOID isp_ae_crop_pos_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAeCropPosX, GK_U32 uIspAeCropPosY)
{
    U_ISP_AE_CROP_POS o_isp_ae_crop_pos;
    o_isp_ae_crop_pos.bits.isp_ae_crop_pos_x = uIspAeCropPosX;
    o_isp_ae_crop_pos.bits.isp_ae_crop_pos_y = uIspAeCropPosY;
    pstBeReg->ISP_AE_CROP_POS.u32 = o_isp_ae_crop_pos.u32;
}

static inline GK_VOID isp_ae_crop_outsize_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAeCropOutWidth, GK_U32 uIspAeCropOutHeight)
{
    U_ISP_AE_CROP_OUTSIZE o_isp_ae_crop_outsize;
    o_isp_ae_crop_outsize.bits.isp_ae_crop_out_width = uIspAeCropOutWidth;
    o_isp_ae_crop_outsize.bits.isp_ae_crop_out_height = uIspAeCropOutHeight;
    pstBeReg->ISP_AE_CROP_OUTSIZE.u32 = o_isp_ae_crop_outsize.u32;
}

static inline GK_VOID isp_aest_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAeHistRaddr)
{
    U_ISP_AE_HIST_RADDR o_isp_aest_raddr;
    o_isp_aest_raddr.bits.isp_aest_raddr = uIspAeHistRaddr;
    pstBeReg->ISP_AE_HIST_RADDR.u32 = o_isp_aest_raddr.u32;
}

static inline GK_VOID isp_ae_aver_r_gr_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAeAverRGrRaddr)
{
    U_ISP_AE_AVER_R_GR_RADDR o_isp_ae_aver_r_gr_raddr;
    o_isp_ae_aver_r_gr_raddr.bits.isp_ae_aver_r_gr_raddr = uIspAeAverRGrRaddr;
    pstBeReg->ISP_AE_AVER_R_GR_RADDR.u32 = o_isp_ae_aver_r_gr_raddr.u32;
}

static inline GK_VOID isp_ae_aver_gb_b_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAeAverGbBRaddr)
{
    U_ISP_AE_AVER_GB_B_RADDR o_isp_ae_aver_gb_b_raddr;
    o_isp_ae_aver_gb_b_raddr.bits.isp_ae_aver_gb_b_raddr = uIspAeAverGbBRaddr;
    pstBeReg->ISP_AE_AVER_GB_B_RADDR.u32 = o_isp_ae_aver_gb_b_raddr.u32;
}

static inline GK_VOID isp_ae_weight_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAeWeiWaddr)
{
    U_ISP_AE_WEIGHT_WADDR o_isp_ae_weight_waddr;
    o_isp_ae_weight_waddr.bits.isp_ae_wei_waddr = uIspAeWeiWaddr;
    pstBeReg->ISP_AE_WEIGHT_WADDR.u32 = o_isp_ae_weight_waddr.u32;
}

static inline GK_VOID isp_ae_weight_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAeWeiWdata)
{
    U_ISP_AE_WEIGHT_WDATA o_isp_ae_weight_wdata;
    o_isp_ae_weight_wdata.bits.isp_ae_wei_wdata = uIspAeWeiWdata;
    pstBeReg->ISP_AE_WEIGHT_WDATA.u32 = o_isp_ae_weight_wdata.u32;
}

static inline GK_VOID isp_awb_zone_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAwbHnum, GK_U32 uIspAwbVnum)
{
    U_ISP_AWB_ZONE o_isp_awb_zone;
    o_isp_awb_zone.bits.isp_awb_hnum = uIspAwbHnum;
    o_isp_awb_zone.bits.isp_awb_vnum = uIspAwbVnum;
    pstBeReg->ISP_AWB_ZONE.u32 = o_isp_awb_zone.u32;
}

static inline GK_VOID isp_awb_bitmove_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAwbBitmove)
{
    U_ISP_AWB_BITMOVE o_isp_awb_bitmove;
    o_isp_awb_bitmove.bits.isp_awb_bitmove = uIspAwbBitmove;
    pstBeReg->ISP_AWB_BITMOVE.u32 = o_isp_awb_bitmove.u32;
}

static inline GK_VOID isp_awb_thd_min_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAwbThresholdMin)
{
    U_ISP_AWB_THD_MIN o_isp_awb_thd_min;
    o_isp_awb_thd_min.bits.isp_awb_threshold_min = uIspAwbThresholdMin;
    pstBeReg->ISP_AWB_THD_MIN.u32 = o_isp_awb_thd_min.u32;
}

static inline GK_VOID isp_awb_thd_max_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAwbThresholdMax)
{
    U_ISP_AWB_THD_MAX o_isp_awb_thd_max;
    o_isp_awb_thd_max.bits.isp_awb_threshold_max = uIspAwbThresholdMax;
    pstBeReg->ISP_AWB_THD_MAX.u32 = o_isp_awb_thd_max.u32;
}

static inline GK_VOID isp_awb_cr_mm_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAwbCrRefMax, GK_U32 uIspAwbCrRefMin)
{
    U_ISP_AWB_CR_MM o_isp_awb_cr_mm;
    o_isp_awb_cr_mm.bits.isp_awb_cr_ref_max = uIspAwbCrRefMax;
    o_isp_awb_cr_mm.bits.isp_awb_cr_ref_min = uIspAwbCrRefMin;
    pstBeReg->ISP_AWB_CR_MM.u32 = o_isp_awb_cr_mm.u32;
}

static inline GK_VOID isp_awb_cb_mm_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAwbCbRefMax, GK_U32 uIspAwbCbRefMin)
{
    U_ISP_AWB_CB_MM o_isp_awb_cb_mm;
    o_isp_awb_cb_mm.bits.isp_awb_cb_ref_max = uIspAwbCbRefMax;
    o_isp_awb_cb_mm.bits.isp_awb_cb_ref_min = uIspAwbCbRefMin;
    pstBeReg->ISP_AWB_CB_MM.u32 = o_isp_awb_cb_mm.u32;
}

static inline GK_VOID isp_awb_offset_comp_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAwbOffsetComp)
{
    U_ISP_AWB_OFFSET_COMP o_isp_awb_offset_comp;
    o_isp_awb_offset_comp.bits.isp_awb_offset_comp = uIspAwbOffsetComp;
    pstBeReg->ISP_AWB_OFFSET_COMP.u32 = o_isp_awb_offset_comp.u32;
}

static inline GK_VOID isp_awb_stt_bst_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAwbSttSize, GK_U32 uIspAwbSttBst, GK_U32 uIspAwbSttEn)
{
    U_ISP_AWB_STT_BST o_isp_awb_stt_bst;
    o_isp_awb_stt_bst.bits.isp_awb_stt_size = uIspAwbSttSize;
    o_isp_awb_stt_bst.bits.isp_awb_stt_bst = uIspAwbSttBst;
    o_isp_awb_stt_bst.bits.isp_awb_stt_en = uIspAwbSttEn;
    pstBeReg->ISP_AWB_STT_BST.u32 = o_isp_awb_stt_bst.u32;
}

static inline GK_VOID isp_awb_stt_abn_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAwbSttClr)
{
    U_ISP_AWB_STT_ABN o_isp_awb_stt_abn;
    o_isp_awb_stt_abn.bits.isp_awb_stt_clr = uIspAwbSttClr;
    pstBeReg->ISP_AWB_STT_ABN.u32 = o_isp_awb_stt_abn.u32;
}

static inline GK_VOID isp_awb_crop_pos_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAwbCropPosX, GK_U32 uIspAwbCropPosY)
{
    U_ISP_AWB_CROP_POS o_isp_awb_crop_pos;
    o_isp_awb_crop_pos.bits.isp_awb_crop_pos_x = uIspAwbCropPosX;
    o_isp_awb_crop_pos.bits.isp_awb_crop_pos_y = uIspAwbCropPosY;
    pstBeReg->ISP_AWB_CROP_POS.u32 = o_isp_awb_crop_pos.u32;
}

static inline GK_VOID isp_awb_crop_outsize_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAwbCropOutWidth, GK_U32 uIspAwbCropOutHeight)
{
    U_ISP_AWB_CROP_OUTSIZE o_isp_awb_crop_outsize;
    o_isp_awb_crop_outsize.bits.isp_awb_crop_out_width = uIspAwbCropOutWidth;
    o_isp_awb_crop_outsize.bits.isp_awb_crop_out_height = uIspAwbCropOutHeight;
    pstBeReg->ISP_AWB_CROP_OUTSIZE.u32 = o_isp_awb_crop_outsize.u32;
}

static inline GK_VOID isp_awb_stat_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAwbStatRaddr)
{
    U_ISP_AWB_STAT_RADDR o_isp_awb_stat_raddr;
    o_isp_awb_stat_raddr.bits.isp_awb_stat_raddr = uIspAwbStatRaddr;
    pstBeReg->ISP_AWB_STAT_RADDR.u32 = o_isp_awb_stat_raddr.u32;
}

static inline GK_VOID isp_af_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfIir0En0, GK_U32 uIspAfIir0En1, GK_U32 uIspAfIir0En2, GK_U32 uIspAfIir1En0, GK_U32 uIspAfIir1En1, GK_U32 uIspAfIir1En2, GK_U32 uIspAfPeakMode, GK_U32 uIspAfSquMode, GK_U32 uIspAfOffsetEn, GK_U32 uIspAfCropEn, GK_U32 uIspAfLpfEn, GK_U32 uIspAfMeanEn, GK_U32 uIspAfRawMode, GK_U32 uIspAfBayerMode, GK_U32 uIspAfIir0DsEn, GK_U32 uIspAfIir1DsEn, GK_U32 uIspAfFir0LpfEn, GK_U32 uIspAfFir1LpfEn, GK_U32 uIspAfIir0LdgEn, GK_U32 uIspAfIir1LdgEn, GK_U32 uIspAfFir0LdgEn, GK_U32 uIspAfFir1LdgEn, GK_U32 uIspAfGamma)
{
    U_ISP_AF_CFG o_isp_af_cfg;
    o_isp_af_cfg.bits.isp_af_iir0_en0 = uIspAfIir0En0;
    o_isp_af_cfg.bits.isp_af_iir0_en1 = uIspAfIir0En1;
    o_isp_af_cfg.bits.isp_af_iir0_en2 = uIspAfIir0En2;
    o_isp_af_cfg.bits.isp_af_iir1_en0 = uIspAfIir1En0;
    o_isp_af_cfg.bits.isp_af_iir1_en1 = uIspAfIir1En1;
    o_isp_af_cfg.bits.isp_af_iir1_en2 = uIspAfIir1En2;
    o_isp_af_cfg.bits.isp_af_peak_mode = uIspAfPeakMode;
    o_isp_af_cfg.bits.isp_af_squ_mode = uIspAfSquMode;
    o_isp_af_cfg.bits.isp_af_offset_en = uIspAfOffsetEn;
    o_isp_af_cfg.bits.isp_af_crop_en = uIspAfCropEn;
    o_isp_af_cfg.bits.isp_af_lpf_en = uIspAfLpfEn;
    o_isp_af_cfg.bits.isp_af_mean_en = uIspAfMeanEn;
    o_isp_af_cfg.bits.isp_af_raw_mode = uIspAfRawMode;
    o_isp_af_cfg.bits.isp_af_bayer_mode = uIspAfBayerMode;
    o_isp_af_cfg.bits.isp_af_iir0_ds_en = uIspAfIir0DsEn;
    o_isp_af_cfg.bits.isp_af_iir1_ds_en = uIspAfIir1DsEn;
    o_isp_af_cfg.bits.isp_af_fir0_lpf_en = uIspAfFir0LpfEn;
    o_isp_af_cfg.bits.isp_af_fir1_lpf_en = uIspAfFir1LpfEn;
    o_isp_af_cfg.bits.isp_af_iir0_ldg_en = uIspAfIir0LdgEn;
    o_isp_af_cfg.bits.isp_af_iir1_ldg_en = uIspAfIir1LdgEn;
    o_isp_af_cfg.bits.isp_af_fir0_ldg_en = uIspAfFir0LdgEn;
    o_isp_af_cfg.bits.isp_af_fir1_ldg_en = uIspAfFir1LdgEn;
    o_isp_af_cfg.bits.isp_af_gamma = uIspAfGamma;
    pstBeReg->ISP_AF_CFG.u32 = o_isp_af_cfg.u32;
}

static inline GK_VOID isp_af_zone_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfHnum, GK_U32 uIspAfVnum)
{
    U_ISP_AF_ZONE o_isp_af_zone;
    o_isp_af_zone.bits.isp_af_hnum = uIspAfHnum;
    o_isp_af_zone.bits.isp_af_vnum = uIspAfVnum;
    pstBeReg->ISP_AF_ZONE.u32 = o_isp_af_zone.u32;
}

static inline GK_VOID isp_af_crop_start_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfPosX, GK_U32 uIspAfPosY)
{
    U_ISP_AF_CROP_START o_isp_af_crop_start;
    o_isp_af_crop_start.bits.isp_af_pos_x = uIspAfPosX;
    o_isp_af_crop_start.bits.isp_af_pos_y = uIspAfPosY;
    pstBeReg->ISP_AF_CROP_START.u32 = o_isp_af_crop_start.u32;
}

static inline GK_VOID isp_af_crop_size_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfCropHsize, GK_U32 uIspAfCropVsize)
{
    U_ISP_AF_CROP_SIZE o_isp_af_crop_size;
    o_isp_af_crop_size.bits.isp_af_crop_hsize = uIspAfCropHsize;
    o_isp_af_crop_size.bits.isp_af_crop_vsize = uIspAfCropVsize;
    pstBeReg->ISP_AF_CROP_SIZE.u32 = o_isp_af_crop_size.u32;
}

static inline GK_VOID isp_af_mean_thres_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfMeanThres)
{
    U_ISP_AF_MEAN_THRES o_isp_af_mean_thres;
    o_isp_af_mean_thres.bits.isp_af_mean_thres = uIspAfMeanThres;
    pstBeReg->ISP_AF_MEAN_THRES.u32 = o_isp_af_mean_thres.u32;
}

static inline GK_VOID isp_af_iirg0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfIirg00, GK_U32 uIspAfIirg01)
{
    U_ISP_AF_IIRG0 o_isp_af_iirg0;
    o_isp_af_iirg0.bits.isp_af_iirg0_0 = uIspAfIirg00;
    o_isp_af_iirg0.bits.isp_af_iirg0_1 = uIspAfIirg01;
    pstBeReg->ISP_AF_IIRG0.u32 = o_isp_af_iirg0.u32;
}

static inline GK_VOID isp_af_iirg1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfIirg10, GK_U32 uIspAfIirg11)
{
    U_ISP_AF_IIRG1 o_isp_af_iirg1;
    o_isp_af_iirg1.bits.isp_af_iirg1_0 = uIspAfIirg10;
    o_isp_af_iirg1.bits.isp_af_iirg1_1 = uIspAfIirg11;
    pstBeReg->ISP_AF_IIRG1.u32 = o_isp_af_iirg1.u32;
}

static inline GK_VOID isp_af_iirg2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfIirg20, GK_U32 uIspAfIirg21)
{
    U_ISP_AF_IIRG2 o_isp_af_iirg2;
    o_isp_af_iirg2.bits.isp_af_iirg2_0 = uIspAfIirg20;
    o_isp_af_iirg2.bits.isp_af_iirg2_1 = uIspAfIirg21;
    pstBeReg->ISP_AF_IIRG2.u32 = o_isp_af_iirg2.u32;
}

static inline GK_VOID isp_af_iirg3_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfIirg30, GK_U32 uIspAfIirg31)
{
    U_ISP_AF_IIRG3 o_isp_af_iirg3;
    o_isp_af_iirg3.bits.isp_af_iirg3_0 = uIspAfIirg30;
    o_isp_af_iirg3.bits.isp_af_iirg3_1 = uIspAfIirg31;
    pstBeReg->ISP_AF_IIRG3.u32 = o_isp_af_iirg3.u32;
}

static inline GK_VOID isp_af_iirg4_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfIirg40, GK_U32 uIspAfIirg41)
{
    U_ISP_AF_IIRG4 o_isp_af_iirg4;
    o_isp_af_iirg4.bits.isp_af_iirg4_0 = uIspAfIirg40;
    o_isp_af_iirg4.bits.isp_af_iirg4_1 = uIspAfIirg41;
    pstBeReg->ISP_AF_IIRG4.u32 = o_isp_af_iirg4.u32;
}

static inline GK_VOID isp_af_iirg5_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfIirg50, GK_U32 uIspAfIirg51)
{
    U_ISP_AF_IIRG5 o_isp_af_iirg5;
    o_isp_af_iirg5.bits.isp_af_iirg5_0 = uIspAfIirg50;
    o_isp_af_iirg5.bits.isp_af_iirg5_1 = uIspAfIirg51;
    pstBeReg->ISP_AF_IIRG5.u32 = o_isp_af_iirg5.u32;
}

static inline GK_VOID isp_af_iirg6_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfIirg60, GK_U32 uIspAfIirg61)
{
    U_ISP_AF_IIRG6 o_isp_af_iirg6;
    o_isp_af_iirg6.bits.isp_af_iirg6_0 = uIspAfIirg60;
    o_isp_af_iirg6.bits.isp_af_iirg6_1 = uIspAfIirg61;
    pstBeReg->ISP_AF_IIRG6.u32 = o_isp_af_iirg6.u32;
}

static inline GK_VOID isp_af_iirpl_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfIirplg0, GK_U32 uIspAfIirpls0, GK_U32 uIspAfIirplg1, GK_U32 uIspAfIirpls1)
{
    U_ISP_AF_IIRPL o_isp_af_iirpl;
    o_isp_af_iirpl.bits.isp_af_iirplg_0 = uIspAfIirplg0;
    o_isp_af_iirpl.bits.isp_af_iirpls_0 = uIspAfIirpls0;
    o_isp_af_iirpl.bits.isp_af_iirplg_1 = uIspAfIirplg1;
    o_isp_af_iirpl.bits.isp_af_iirpls_1 = uIspAfIirpls1;
    pstBeReg->ISP_AF_IIRPL.u32 = o_isp_af_iirpl.u32;
}

static inline GK_VOID isp_af_shift_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfIirshift00, GK_U32 uIspAfIirshift01, GK_U32 uIspAfIirshift02, GK_U32 uIspAfIirshift03, GK_U32 uIspAfIirshift10, GK_U32 uIspAfIirshift11, GK_U32 uIspAfIirshift12, GK_U32 uIspAfIirshift13)
{
    U_ISP_AF_SHIFT o_isp_af_shift;
    o_isp_af_shift.bits.isp_af_iirshift0_0 = uIspAfIirshift00;
    o_isp_af_shift.bits.isp_af_iirshift0_1 = uIspAfIirshift01;
    o_isp_af_shift.bits.isp_af_iirshift0_2 = uIspAfIirshift02;
    o_isp_af_shift.bits.isp_af_iirshift0_3 = uIspAfIirshift03;
    o_isp_af_shift.bits.isp_af_iirshift1_0 = uIspAfIirshift10;
    o_isp_af_shift.bits.isp_af_iirshift1_1 = uIspAfIirshift11;
    o_isp_af_shift.bits.isp_af_iirshift1_2 = uIspAfIirshift12;
    o_isp_af_shift.bits.isp_af_iirshift1_3 = uIspAfIirshift13;
    pstBeReg->ISP_AF_SHIFT.u32 = o_isp_af_shift.u32;
}

static inline GK_VOID isp_af_firh0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfFirh00, GK_U32 uIspAfFirh01)
{
    U_ISP_AF_FIRH0 o_isp_af_firh0;
    o_isp_af_firh0.bits.isp_af_firh0_0 = uIspAfFirh00;
    o_isp_af_firh0.bits.isp_af_firh0_1 = uIspAfFirh01;
    pstBeReg->ISP_AF_FIRH0.u32 = o_isp_af_firh0.u32;
}

static inline GK_VOID isp_af_firh1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfFirh10, GK_U32 uIspAfFirh11)
{
    U_ISP_AF_FIRH1 o_isp_af_firh1;
    o_isp_af_firh1.bits.isp_af_firh1_0 = uIspAfFirh10;
    o_isp_af_firh1.bits.isp_af_firh1_1 = uIspAfFirh11;
    pstBeReg->ISP_AF_FIRH1.u32 = o_isp_af_firh1.u32;
}

static inline GK_VOID isp_af_firh2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfFirh20, GK_U32 uIspAfFirh21)
{
    U_ISP_AF_FIRH2 o_isp_af_firh2;
    o_isp_af_firh2.bits.isp_af_firh2_0 = uIspAfFirh20;
    o_isp_af_firh2.bits.isp_af_firh2_1 = uIspAfFirh21;
    pstBeReg->ISP_AF_FIRH2.u32 = o_isp_af_firh2.u32;
}

static inline GK_VOID isp_af_firh3_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfFirh30, GK_U32 uIspAfFirh31)
{
    U_ISP_AF_FIRH3 o_isp_af_firh3;
    o_isp_af_firh3.bits.isp_af_firh3_0 = uIspAfFirh30;
    o_isp_af_firh3.bits.isp_af_firh3_1 = uIspAfFirh31;
    pstBeReg->ISP_AF_FIRH3.u32 = o_isp_af_firh3.u32;
}

static inline GK_VOID isp_af_firh4_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfFirh40, GK_U32 uIspAfFirh41)
{
    U_ISP_AF_FIRH4 o_isp_af_firh4;
    o_isp_af_firh4.bits.isp_af_firh4_0 = uIspAfFirh40;
    o_isp_af_firh4.bits.isp_af_firh4_1 = uIspAfFirh41;
    pstBeReg->ISP_AF_FIRH4.u32 = o_isp_af_firh4.u32;
}

static inline GK_VOID isp_af_stt_bst_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfSttSize, GK_U32 uIspAfSttBst, GK_U32 uIspAfSttEn)
{
    U_ISP_AF_STT_BST o_isp_af_stt_bst;
    o_isp_af_stt_bst.bits.isp_af_stt_size = uIspAfSttSize;
    o_isp_af_stt_bst.bits.isp_af_stt_bst = uIspAfSttBst;
    o_isp_af_stt_bst.bits.isp_af_stt_en = uIspAfSttEn;
    pstBeReg->ISP_AF_STT_BST.u32 = o_isp_af_stt_bst.u32;
}

static inline GK_VOID isp_af_stt_abn_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfSttClr)
{
    U_ISP_AF_STT_ABN o_isp_af_stt_abn;
    o_isp_af_stt_abn.bits.isp_af_stt_clr = uIspAfSttClr;
    pstBeReg->ISP_AF_STT_ABN.u32 = o_isp_af_stt_abn.u32;
}

static inline GK_VOID isp_af_acc_shift_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfAccShift0H, GK_U32 uIspAfAccShift1H, GK_U32 uIspAfAccShift0V, GK_U32 uIspAfAccShift1V, GK_U32 uIspAfAccShiftY)
{
    U_ISP_AF_ACC_SHIFT o_isp_af_acc_shift;
    o_isp_af_acc_shift.bits.isp_af_acc_shift0_h = uIspAfAccShift0H;
    o_isp_af_acc_shift.bits.isp_af_acc_shift1_h = uIspAfAccShift1H;
    o_isp_af_acc_shift.bits.isp_af_acc_shift0_v = uIspAfAccShift0V;
    o_isp_af_acc_shift.bits.isp_af_acc_shift1_v = uIspAfAccShift1V;
    o_isp_af_acc_shift.bits.isp_af_acc_shift_y = uIspAfAccShiftY;
    pstBeReg->ISP_AF_ACC_SHIFT.u32 = o_isp_af_acc_shift.u32;
}

static inline GK_VOID isp_af_cnt_shift_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfCntShift0H, GK_U32 uIspAfCntShift1H, GK_U32 uIspAfCntShift0V, GK_U32 uIspAfCntShift1V, GK_U32 uIspAfCntShiftY)
{
    U_ISP_AF_CNT_SHIFT o_isp_af_cnt_shift;
    o_isp_af_cnt_shift.bits.isp_af_cnt_shift0_h = uIspAfCntShift0H;
    o_isp_af_cnt_shift.bits.isp_af_cnt_shift1_h = uIspAfCntShift1H;
    o_isp_af_cnt_shift.bits.isp_af_cnt_shift0_v = uIspAfCntShift0V;
    o_isp_af_cnt_shift.bits.isp_af_cnt_shift1_v = uIspAfCntShift1V;
    o_isp_af_cnt_shift.bits.isp_af_cnt_shift_y = uIspAfCntShiftY;
    pstBeReg->ISP_AF_CNT_SHIFT.u32 = o_isp_af_cnt_shift.u32;
}

static inline GK_VOID isp_af_stat_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfStatRaddr)
{
    U_ISP_AF_STAT_RADDR o_isp_af_stat_raddr;
    o_isp_af_stat_raddr.bits.isp_af_stat_raddr = uIspAfStatRaddr;
    pstBeReg->ISP_AF_STAT_RADDR.u32 = o_isp_af_stat_raddr.u32;
}

static inline GK_VOID isp_af_iirthre_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfIirThre0L, GK_U32 uIspAfIirThre0H, GK_U32 uIspAfIirThre1L, GK_U32 uIspAfIirThre1H)
{
    U_ISP_AF_IIRTHRE o_isp_af_iirthre;
    o_isp_af_iirthre.bits.isp_af_iir_thre0_l = uIspAfIirThre0L;
    o_isp_af_iirthre.bits.isp_af_iir_thre0_h = uIspAfIirThre0H;
    o_isp_af_iirthre.bits.isp_af_iir_thre1_l = uIspAfIirThre1L;
    o_isp_af_iirthre.bits.isp_af_iir_thre1_h = uIspAfIirThre1H;
    pstBeReg->ISP_AF_IIRTHRE.u32 = o_isp_af_iirthre.u32;
}

static inline GK_VOID isp_af_iirgain_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfIirGain0L, GK_U32 uIspAfIirGain0H, GK_U32 uIspAfIirGain1L, GK_U32 uIspAfIirGain1H)
{
    U_ISP_AF_IIRGAIN o_isp_af_iirgain;
    o_isp_af_iirgain.bits.isp_af_iir_gain0_l = uIspAfIirGain0L;
    o_isp_af_iirgain.bits.isp_af_iir_gain0_h = uIspAfIirGain0H;
    o_isp_af_iirgain.bits.isp_af_iir_gain1_l = uIspAfIirGain1L;
    o_isp_af_iirgain.bits.isp_af_iir_gain1_h = uIspAfIirGain1H;
    pstBeReg->ISP_AF_IIRGAIN.u32 = o_isp_af_iirgain.u32;
}

static inline GK_VOID isp_af_iirslope_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfIirSlope0L, GK_U32 uIspAfIirSlope0H, GK_U32 uIspAfIirSlope1L, GK_U32 uIspAfIirSlope1H)
{
    U_ISP_AF_IIRSLOPE o_isp_af_iirslope;
    o_isp_af_iirslope.bits.isp_af_iir_slope0_l = uIspAfIirSlope0L;
    o_isp_af_iirslope.bits.isp_af_iir_slope0_h = uIspAfIirSlope0H;
    o_isp_af_iirslope.bits.isp_af_iir_slope1_l = uIspAfIirSlope1L;
    o_isp_af_iirslope.bits.isp_af_iir_slope1_h = uIspAfIirSlope1H;
    pstBeReg->ISP_AF_IIRSLOPE.u32 = o_isp_af_iirslope.u32;
}

static inline GK_VOID isp_af_iirdilate_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfIirDilate0, GK_U32 uIspAfIirDilate1)
{
    U_ISP_AF_IIRDILATE o_isp_af_iirdilate;
    o_isp_af_iirdilate.bits.isp_af_iir_dilate0 = uIspAfIirDilate0;
    o_isp_af_iirdilate.bits.isp_af_iir_dilate1 = uIspAfIirDilate1;
    pstBeReg->ISP_AF_IIRDILATE.u32 = o_isp_af_iirdilate.u32;
}

static inline GK_VOID isp_af_firthre_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfFirThre0L, GK_U32 uIspAfFirThre0H, GK_U32 uIspAfFirThre1L, GK_U32 uIspAfFirThre1H)
{
    U_ISP_AF_FIRTHRE o_isp_af_firthre;
    o_isp_af_firthre.bits.isp_af_fir_thre0_l = uIspAfFirThre0L;
    o_isp_af_firthre.bits.isp_af_fir_thre0_h = uIspAfFirThre0H;
    o_isp_af_firthre.bits.isp_af_fir_thre1_l = uIspAfFirThre1L;
    o_isp_af_firthre.bits.isp_af_fir_thre1_h = uIspAfFirThre1H;
    pstBeReg->ISP_AF_FIRTHRE.u32 = o_isp_af_firthre.u32;
}

static inline GK_VOID isp_af_firgain_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfFirGain0L, GK_U32 uIspAfFirGain0H, GK_U32 uIspAfFirGain1L, GK_U32 uIspAfFirGain1H)
{
    U_ISP_AF_FIRGAIN o_isp_af_firgain;
    o_isp_af_firgain.bits.isp_af_fir_gain0_l = uIspAfFirGain0L;
    o_isp_af_firgain.bits.isp_af_fir_gain0_h = uIspAfFirGain0H;
    o_isp_af_firgain.bits.isp_af_fir_gain1_l = uIspAfFirGain1L;
    o_isp_af_firgain.bits.isp_af_fir_gain1_h = uIspAfFirGain1H;
    pstBeReg->ISP_AF_FIRGAIN.u32 = o_isp_af_firgain.u32;
}

static inline GK_VOID isp_af_firslope_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfFirSlope0L, GK_U32 uIspAfFirSlope0H, GK_U32 uIspAfFirSlope1L, GK_U32 uIspAfFirSlope1H)
{
    U_ISP_AF_FIRSLOPE o_isp_af_firslope;
    o_isp_af_firslope.bits.isp_af_fir_slope0_l = uIspAfFirSlope0L;
    o_isp_af_firslope.bits.isp_af_fir_slope0_h = uIspAfFirSlope0H;
    o_isp_af_firslope.bits.isp_af_fir_slope1_l = uIspAfFirSlope1L;
    o_isp_af_firslope.bits.isp_af_fir_slope1_h = uIspAfFirSlope1H;
    pstBeReg->ISP_AF_FIRSLOPE.u32 = o_isp_af_firslope.u32;
}

static inline GK_VOID isp_af_iirthre_coring_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfIirThre0C, GK_U32 uIspAfIirThre1C)
{
    U_ISP_AF_IIRTHRE_CORING o_isp_af_iirthre_coring;
    o_isp_af_iirthre_coring.bits.isp_af_iir_thre0_c = uIspAfIirThre0C;
    o_isp_af_iirthre_coring.bits.isp_af_iir_thre1_c = uIspAfIirThre1C;
    pstBeReg->ISP_AF_IIRTHRE_CORING.u32 = o_isp_af_iirthre_coring.u32;
}

static inline GK_VOID isp_af_iirpeak_coring_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfIirPeak0C, GK_U32 uIspAfIirPeak1C)
{
    U_ISP_AF_IIRPEAK_CORING o_isp_af_iirpeak_coring;
    o_isp_af_iirpeak_coring.bits.isp_af_iir_peak0_c = uIspAfIirPeak0C;
    o_isp_af_iirpeak_coring.bits.isp_af_iir_peak1_c = uIspAfIirPeak1C;
    pstBeReg->ISP_AF_IIRPEAK_CORING.u32 = o_isp_af_iirpeak_coring.u32;
}

static inline GK_VOID isp_af_iirslope_coring_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfIirSlope0C, GK_U32 uIspAfIirSlope1C)
{
    U_ISP_AF_IIRSLOPE_CORING o_isp_af_iirslope_coring;
    o_isp_af_iirslope_coring.bits.isp_af_iir_slope0_c = uIspAfIirSlope0C;
    o_isp_af_iirslope_coring.bits.isp_af_iir_slope1_c = uIspAfIirSlope1C;
    pstBeReg->ISP_AF_IIRSLOPE_CORING.u32 = o_isp_af_iirslope_coring.u32;
}

static inline GK_VOID isp_af_firthre_coring_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfFirThre0C, GK_U32 uIspAfFirThre1C)
{
    U_ISP_AF_FIRTHRE_CORING o_isp_af_firthre_coring;
    o_isp_af_firthre_coring.bits.isp_af_fir_thre0_c = uIspAfFirThre0C;
    o_isp_af_firthre_coring.bits.isp_af_fir_thre1_c = uIspAfFirThre1C;
    pstBeReg->ISP_AF_FIRTHRE_CORING.u32 = o_isp_af_firthre_coring.u32;
}

static inline GK_VOID isp_af_firpeak_coring_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfFirPeak0C, GK_U32 uIspAfFirPeak1C)
{
    U_ISP_AF_FIRPEAK_CORING o_isp_af_firpeak_coring;
    o_isp_af_firpeak_coring.bits.isp_af_fir_peak0_c = uIspAfFirPeak0C;
    o_isp_af_firpeak_coring.bits.isp_af_fir_peak1_c = uIspAfFirPeak1C;
    pstBeReg->ISP_AF_FIRPEAK_CORING.u32 = o_isp_af_firpeak_coring.u32;
}

static inline GK_VOID isp_af_firslope_coring_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfFirSlope0C, GK_U32 uIspAfFirSlope1C)
{
    U_ISP_AF_FIRSLOPE_CORING o_isp_af_firslope_coring;
    o_isp_af_firslope_coring.bits.isp_af_fir_slope0_c = uIspAfFirSlope0C;
    o_isp_af_firslope_coring.bits.isp_af_fir_slope1_c = uIspAfFirSlope1C;
    pstBeReg->ISP_AF_FIRSLOPE_CORING.u32 = o_isp_af_firslope_coring.u32;
}

static inline GK_VOID isp_aflight_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfHilight)
{
    U_ISP_AF_LIGHT o_isp_aflight;
    o_isp_aflight.bits.isp_aflight = uIspAfHilight;
    pstBeReg->ISP_AF_LIGHT.u32 = o_isp_aflight.u32;
}

static inline GK_VOID isp_af_offset_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspAfOffsetGr, GK_U32 uIspAfOffsetGb)
{
    U_ISP_AF_OFFSET o_isp_af_offset;
    o_isp_af_offset.bits.isp_af_offset_gr = uIspAfOffsetGr;
    o_isp_af_offset.bits.isp_af_offset_gb = uIspAfOffsetGb;
    pstBeReg->ISP_AF_OFFSET.u32 = o_isp_af_offset.u32;
}

static inline GK_VOID isp_dis_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDisBlcEn)
{
    U_ISP_DIS_CFG o_isp_dis_cfg;
    o_isp_dis_cfg.bits.isp_dis_blc_en = uIspDisBlcEn;
    pstBeReg->ISP_DIS_CFG.u32 = o_isp_dis_cfg.u32;
}

static inline GK_VOID isp_dis_blk_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDisBlkSize, GK_U32 uIspDisSrchRange)
{
    U_ISP_DIS_BLK o_isp_dis_blk;
    o_isp_dis_blk.bits.isp_dis_blk_size = uIspDisBlkSize;
    o_isp_dis_blk.bits.isp_dis_srch_range = uIspDisSrchRange;
    pstBeReg->ISP_DIS_BLK.u32 = o_isp_dis_blk.u32;
}

static inline GK_VOID isp_dis_v0pos_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDisV0posHor, GK_U32 uIspDisV0posVer)
{
    U_ISP_DIS_V0POS o_isp_dis_v0pos;
    o_isp_dis_v0pos.bits.isp_dis_v0pos_hor = uIspDisV0posHor;
    o_isp_dis_v0pos.bits.isp_dis_v0pos_ver = uIspDisV0posVer;
    pstBeReg->ISP_DIS_V0POS.u32 = o_isp_dis_v0pos.u32;
}

static inline GK_VOID isp_dis_v4pos_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDisV4posHor, GK_U32 uIspDisV4posVer)
{
    U_ISP_DIS_V4POS o_isp_dis_v4pos;
    o_isp_dis_v4pos.bits.isp_dis_v4pos_hor = uIspDisV4posHor;
    o_isp_dis_v4pos.bits.isp_dis_v4pos_ver = uIspDisV4posVer;
    pstBeReg->ISP_DIS_V4POS.u32 = o_isp_dis_v4pos.u32;
}

static inline GK_VOID isp_dis_v8pos_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDisV8posHor, GK_U32 uIspDisV8posVer)
{
    U_ISP_DIS_V8POS o_isp_dis_v8pos;
    o_isp_dis_v8pos.bits.isp_dis_v8pos_hor = uIspDisV8posHor;
    o_isp_dis_v8pos.bits.isp_dis_v8pos_ver = uIspDisV8posVer;
    pstBeReg->ISP_DIS_V8POS.u32 = o_isp_dis_v8pos.u32;
}

static inline GK_VOID isp_dis_v0pose_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDisV0poseHor, GK_U32 uIspDisV0poseVer)
{
    U_ISP_DIS_V0POSE o_isp_dis_v0pose;
    o_isp_dis_v0pose.bits.isp_dis_v0pose_hor = uIspDisV0poseHor;
    o_isp_dis_v0pose.bits.isp_dis_v0pose_ver = uIspDisV0poseVer;
    pstBeReg->ISP_DIS_V0POSE.u32 = o_isp_dis_v0pose.u32;
}

static inline GK_VOID isp_dis_v4pose_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDisV4poseHor, GK_U32 uIspDisV4poseVer)
{
    U_ISP_DIS_V4POSE o_isp_dis_v4pose;
    o_isp_dis_v4pose.bits.isp_dis_v4pose_hor = uIspDisV4poseHor;
    o_isp_dis_v4pose.bits.isp_dis_v4pose_ver = uIspDisV4poseVer;
    pstBeReg->ISP_DIS_V4POSE.u32 = o_isp_dis_v4pose.u32;
}

static inline GK_VOID isp_dis_v8pose_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDisV8poseHor, GK_U32 uIspDisV8poseVer)
{
    U_ISP_DIS_V8POSE o_isp_dis_v8pose;
    o_isp_dis_v8pose.bits.isp_dis_v8pose_hor = uIspDisV8poseHor;
    o_isp_dis_v8pose.bits.isp_dis_v8pose_ver = uIspDisV8poseVer;
    pstBeReg->ISP_DIS_V8POSE.u32 = o_isp_dis_v8pose.u32;
}

static inline GK_VOID isp_dis_h0pos_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDisH0posHor, GK_U32 uIspDisH0posVer)
{
    U_ISP_DIS_H0POS o_isp_dis_h0pos;
    o_isp_dis_h0pos.bits.isp_dis_h0pos_hor = uIspDisH0posHor;
    o_isp_dis_h0pos.bits.isp_dis_h0pos_ver = uIspDisH0posVer;
    pstBeReg->ISP_DIS_H0POS.u32 = o_isp_dis_h0pos.u32;
}

static inline GK_VOID isp_dis_h4pos_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDisH4posHor, GK_U32 uIspDisH4posVer)
{
    U_ISP_DIS_H4POS o_isp_dis_h4pos;
    o_isp_dis_h4pos.bits.isp_dis_h4pos_hor = uIspDisH4posHor;
    o_isp_dis_h4pos.bits.isp_dis_h4pos_ver = uIspDisH4posVer;
    pstBeReg->ISP_DIS_H4POS.u32 = o_isp_dis_h4pos.u32;
}

static inline GK_VOID isp_dis_h8pos_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDisH8posHor, GK_U32 uIspDisH8posVer)
{
    U_ISP_DIS_H8POS o_isp_dis_h8pos;
    o_isp_dis_h8pos.bits.isp_dis_h8pos_hor = uIspDisH8posHor;
    o_isp_dis_h8pos.bits.isp_dis_h8pos_ver = uIspDisH8posVer;
    pstBeReg->ISP_DIS_H8POS.u32 = o_isp_dis_h8pos.u32;
}

static inline GK_VOID isp_dis_h0pose_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDisH0poseHor, GK_U32 uIspDisH0poseVer)
{
    U_ISP_DIS_H0POSE o_isp_dis_h0pose;
    o_isp_dis_h0pose.bits.isp_dis_h0pose_hor = uIspDisH0poseHor;
    o_isp_dis_h0pose.bits.isp_dis_h0pose_ver = uIspDisH0poseVer;
    pstBeReg->ISP_DIS_H0POSE.u32 = o_isp_dis_h0pose.u32;
}

static inline GK_VOID isp_dis_h4pose_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDisH4poseHor, GK_U32 uIspDisH4poseVer)
{
    U_ISP_DIS_H4POSE o_isp_dis_h4pose;
    o_isp_dis_h4pose.bits.isp_dis_h4pose_hor = uIspDisH4poseHor;
    o_isp_dis_h4pose.bits.isp_dis_h4pose_ver = uIspDisH4poseVer;
    pstBeReg->ISP_DIS_H4POSE.u32 = o_isp_dis_h4pose.u32;
}

static inline GK_VOID isp_dis_h8pose_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDisH8poseHor, GK_U32 uIspDisH8poseVer)
{
    U_ISP_DIS_H8POSE o_isp_dis_h8pose;
    o_isp_dis_h8pose.bits.isp_dis_h8pose_hor = uIspDisH8poseHor;
    o_isp_dis_h8pose.bits.isp_dis_h8pose_ver = uIspDisH8poseVer;
    pstBeReg->ISP_DIS_H8POSE.u32 = o_isp_dis_h8pose.u32;
}

static inline GK_VOID isp_dis_raw_luma_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDisRawLuma)
{
    U_ISP_DIS_RAW_LUMA o_isp_dis_raw_luma;
    o_isp_dis_raw_luma.bits.isp_dis_raw_luma = uIspDisRawLuma;
    pstBeReg->ISP_DIS_RAW_LUMA.u32 = o_isp_dis_raw_luma.u32;
}

static inline GK_VOID isp_dis_gamma_en_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDisGammaEn)
{
    U_ISP_DIS_GAMMA_EN o_isp_dis_gamma_en;
    o_isp_dis_gamma_en.bits.isp_dis_gamma_en = uIspDisGammaEn;
    pstBeReg->ISP_DIS_GAMMA_EN.u32 = o_isp_dis_gamma_en.u32;
}

static inline GK_VOID isp_dis_offset_gr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDisOffsetGr)
{
    U_ISP_DIS_OFFSET_GR o_isp_dis_offset_gr;
    o_isp_dis_offset_gr.bits.isp_dis_offset_gr = uIspDisOffsetGr;
    pstBeReg->ISP_DIS_OFFSET_GR.u32 = o_isp_dis_offset_gr.u32;
}

static inline GK_VOID isp_dis_offset_gb_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDisOffsetGb)
{
    U_ISP_DIS_OFFSET_GB o_isp_dis_offset_gb;
    o_isp_dis_offset_gb.bits.isp_dis_offset_gb = uIspDisOffsetGb;
    pstBeReg->ISP_DIS_OFFSET_GB.u32 = o_isp_dis_offset_gb.u32;
}

static inline GK_VOID isp_dis_scale_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDisHorScale, GK_U32 uIspDisVerScale)
{
    U_ISP_DIS_SCALE o_isp_dis_scale;
    o_isp_dis_scale.bits.isp_dis_hor_scale = uIspDisHorScale;
    o_isp_dis_scale.bits.isp_dis_ver_scale = uIspDisVerScale;
    pstBeReg->ISP_DIS_SCALE.u32 = o_isp_dis_scale.u32;
}

static inline GK_VOID isp_dis_refinfo_update_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDisRefinfoUpdate)
{
    U_ISP_DIS_REFINFO_UPDATE o_isp_dis_refinfo_update;
    o_isp_dis_refinfo_update.bits.isp_dis_refinfo_update = uIspDisRefinfoUpdate;
    pstBeReg->ISP_DIS_REFINFO_UPDATE.u32 = o_isp_dis_refinfo_update.u32;
}

static inline GK_VOID isp_dis_prj_shift_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDisHPrjShiftBits, GK_U32 uIspDisVPrjShiftBits)
{
    U_ISP_DIS_PRJ_SHIFT o_isp_dis_prj_shift;
    o_isp_dis_prj_shift.bits.isp_dis_h_prj_shift_bits = uIspDisHPrjShiftBits;
    o_isp_dis_prj_shift.bits.isp_dis_v_prj_shift_bits = uIspDisVPrjShiftBits;
    pstBeReg->ISP_DIS_PRJ_SHIFT.u32 = o_isp_dis_prj_shift.u32;
}

static inline GK_VOID isp_dis_stat_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDisStatRaddr)
{
    U_ISP_DIS_STAT_RADDR o_isp_dis_stat_raddr;
    o_isp_dis_stat_raddr.bits.isp_dis_stat_raddr = uIspDisStatRaddr;
    pstBeReg->ISP_DIS_STAT_RADDR.u32 = o_isp_dis_stat_raddr.u32;
}

static inline GK_VOID isp_dis_refinfo_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDisRefinfoWaddr)
{
    U_ISP_DIS_REFINFO_WADDR o_isp_dis_refinfo_waddr;
    o_isp_dis_refinfo_waddr.bits.isp_dis_refinfo_waddr = uIspDisRefinfoWaddr;
    pstBeReg->ISP_DIS_REFINFO_WADDR.u32 = o_isp_dis_refinfo_waddr.u32;
}

static inline GK_VOID isp_dis_refinfo_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDisRefinfoWdata)
{
    U_ISP_DIS_REFINFO_WDATA o_isp_dis_refinfo_wdata;
    o_isp_dis_refinfo_wdata.bits.isp_dis_refinfo_wdata = uIspDisRefinfoWdata;
    pstBeReg->ISP_DIS_REFINFO_WDATA.u32 = o_isp_dis_refinfo_wdata.u32;
}

static inline GK_VOID isp_dis_refinfo_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDisRefinfoRaddr)
{
    U_ISP_DIS_REFINFO_RADDR o_isp_dis_refinfo_raddr;
    o_isp_dis_refinfo_raddr.bits.isp_dis_refinfo_raddr = uIspDisRefinfoRaddr;
    pstBeReg->ISP_DIS_REFINFO_RADDR.u32 = o_isp_dis_refinfo_raddr.u32;
}

static inline GK_VOID isp_la_zone_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLaHnum, GK_U32 uIspLaVnum)
{
    U_ISP_LA_ZONE o_isp_la_zone;
    o_isp_la_zone.bits.isp_la_hnum = uIspLaHnum;
    o_isp_la_zone.bits.isp_la_vnum = uIspLaVnum;
    pstBeReg->ISP_LA_ZONE.u32 = o_isp_la_zone.u32;
}

static inline GK_VOID isp_la_bitmove_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLaBitmove, GK_U32 uIspLaGammaEn, GK_U32 uIspLaBlcEn)
{
    U_ISP_LA_BITMOVE o_isp_la_bitmove;
    o_isp_la_bitmove.bits.isp_la_bitmove = uIspLaBitmove;
    o_isp_la_bitmove.bits.isp_la_gamma_en = uIspLaGammaEn;
    o_isp_la_bitmove.bits.isp_la_blc_en = uIspLaBlcEn;
    pstBeReg->ISP_LA_BITMOVE.u32 = o_isp_la_bitmove.u32;
}

static inline GK_VOID isp_la_offset_r_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLaOffsetR)
{
    U_ISP_LA_OFFSET_R o_isp_la_offset_r;
    o_isp_la_offset_r.bits.isp_la_offset_r = uIspLaOffsetR;
    pstBeReg->ISP_LA_OFFSET_R.u32 = o_isp_la_offset_r.u32;
}

static inline GK_VOID isp_la_offset_gr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLaOffsetGr)
{
    U_ISP_LA_OFFSET_GR o_isp_la_offset_gr;
    o_isp_la_offset_gr.bits.isp_la_offset_gr = uIspLaOffsetGr;
    pstBeReg->ISP_LA_OFFSET_GR.u32 = o_isp_la_offset_gr.u32;
}

static inline GK_VOID isp_la_offset_gb_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLaOffsetGb)
{
    U_ISP_LA_OFFSET_GB o_isp_la_offset_gb;
    o_isp_la_offset_gb.bits.isp_la_offset_gb = uIspLaOffsetGb;
    pstBeReg->ISP_LA_OFFSET_GB.u32 = o_isp_la_offset_gb.u32;
}

static inline GK_VOID isp_la_offset_b_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLaOffsetB)
{
    U_ISP_LA_OFFSET_B o_isp_la_offset_b;
    o_isp_la_offset_b.bits.isp_la_offset_b = uIspLaOffsetB;
    pstBeReg->ISP_LA_OFFSET_B.u32 = o_isp_la_offset_b.u32;
}

static inline GK_VOID isp_la_gamma_limit_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLaGammaLimit)
{
    U_ISP_LA_GAMMA_LIMIT o_isp_la_gamma_limit;
    o_isp_la_gamma_limit.bits.isp_la_gamma_limit = uIspLaGammaLimit;
    pstBeReg->ISP_LA_GAMMA_LIMIT.u32 = o_isp_la_gamma_limit.u32;
}

static inline GK_VOID isp_la_crop_pos_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLaCropPosX, GK_U32 uIspLaCropPosY)
{
    U_ISP_LA_CROP_POS o_isp_la_crop_pos;
    o_isp_la_crop_pos.bits.isp_la_crop_pos_x = uIspLaCropPosX;
    o_isp_la_crop_pos.bits.isp_la_crop_pos_y = uIspLaCropPosY;
    pstBeReg->ISP_LA_CROP_POS.u32 = o_isp_la_crop_pos.u32;
}

static inline GK_VOID isp_la_crop_outsize_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLaCropOutWidth, GK_U32 uIspLaCropOutHeight)
{
    U_ISP_LA_CROP_OUTSIZE o_isp_la_crop_outsize;
    o_isp_la_crop_outsize.bits.isp_la_crop_out_width = uIspLaCropOutWidth;
    o_isp_la_crop_outsize.bits.isp_la_crop_out_height = uIspLaCropOutHeight;
    pstBeReg->ISP_LA_CROP_OUTSIZE.u32 = o_isp_la_crop_outsize.u32;
}

static inline GK_VOID isp_la_stt_bst_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLaSttSize, GK_U32 uIspLaSttBst, GK_U32 uIspLaSttEn)
{
    U_ISP_LA_STT_BST o_isp_la_stt_bst;
    o_isp_la_stt_bst.bits.isp_la_stt_size = uIspLaSttSize;
    o_isp_la_stt_bst.bits.isp_la_stt_bst = uIspLaSttBst;
    o_isp_la_stt_bst.bits.isp_la_stt_en = uIspLaSttEn;
    pstBeReg->ISP_LA_STT_BST.u32 = o_isp_la_stt_bst.u32;
}

static inline GK_VOID isp_la_stt_abn_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLaSttClr)
{
    U_ISP_LA_STT_ABN o_isp_la_stt_abn;
    o_isp_la_stt_abn.bits.isp_la_stt_clr = uIspLaSttClr;
    pstBeReg->ISP_LA_STT_ABN.u32 = o_isp_la_stt_abn.u32;
}

static inline GK_VOID isp_la_aver_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLaAverRaddr)
{
    U_ISP_LA_AVER_RADDR o_isp_la_aver_raddr;
    o_isp_la_aver_raddr.bits.isp_la_aver_raddr = uIspLaAverRaddr;
    pstBeReg->ISP_LA_AVER_RADDR.u32 = o_isp_la_aver_raddr.u32;
}

static inline GK_VOID isp_lsc_mesh_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLscMeshStr, GK_U32 uIspLscMeshScale)
{
    U_ISP_LSC_MESH o_isp_lsc_mesh;
    o_isp_lsc_mesh.bits.isp_lsc_mesh_str = uIspLscMeshStr;
    o_isp_lsc_mesh.bits.isp_lsc_mesh_scale = uIspLscMeshScale;
    pstBeReg->ISP_LSC_MESH.u32 = o_isp_lsc_mesh.u32;
}

static inline GK_VOID isp_lsc_blcen_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLscBlcInEn, GK_U32 uIspLscBlcOutEn)
{
    U_ISP_LSC_BLCEN o_isp_lsc_blcen;
    o_isp_lsc_blcen.bits.isp_lsc_blc_in_en = uIspLscBlcInEn;
    o_isp_lsc_blcen.bits.isp_lsc_blc_out_en = uIspLscBlcOutEn;
    pstBeReg->ISP_LSC_BLCEN.u32 = o_isp_lsc_blcen.u32;
}

static inline GK_VOID isp_lsc_blc0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLscBlcR, GK_U32 uIspLscBlcGr)
{
    U_ISP_LSC_BLC0 o_isp_lsc_blc0;
    o_isp_lsc_blc0.bits.isp_lsc_blc_r = uIspLscBlcR;
    o_isp_lsc_blc0.bits.isp_lsc_blc_gr = uIspLscBlcGr;
    pstBeReg->ISP_LSC_BLC0.u32 = o_isp_lsc_blc0.u32;
}

static inline GK_VOID isp_lsc_blc1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLscBlcB, GK_U32 uIspLscBlcGb)
{
    U_ISP_LSC_BLC1 o_isp_lsc_blc1;
    o_isp_lsc_blc1.bits.isp_lsc_blc_b = uIspLscBlcB;
    o_isp_lsc_blc1.bits.isp_lsc_blc_gb = uIspLscBlcGb;
    pstBeReg->ISP_LSC_BLC1.u32 = o_isp_lsc_blc1.u32;
}

static inline GK_VOID isp_ge0_ct_th1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspGeGe0CtTh1)
{
    U_ISP_GE0_CT_TH1 o_isp_ge0_ct_th1;
    o_isp_ge0_ct_th1.bits.isp_ge_ge0_ct_th1 = uIspGeGe0CtTh1;
    pstBeReg->ISP_GE0_CT_TH1.u32 = o_isp_ge0_ct_th1.u32;
}

static inline GK_VOID isp_ge0_ct_th2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspGeGe0CtTh2)
{
    U_ISP_GE0_CT_TH2 o_isp_ge0_ct_th2;
    o_isp_ge0_ct_th2.bits.isp_ge_ge0_ct_th2 = uIspGeGe0CtTh2;
    pstBeReg->ISP_GE0_CT_TH2.u32 = o_isp_ge0_ct_th2.u32;
}

static inline GK_VOID isp_ge0_ct_th3_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspGeGe0CtTh3)
{
    U_ISP_GE0_CT_TH3 o_isp_ge0_ct_th3;
    o_isp_ge0_ct_th3.bits.isp_ge_ge0_ct_th3 = uIspGeGe0CtTh3;
    pstBeReg->ISP_GE0_CT_TH3.u32 = o_isp_ge0_ct_th3.u32;
}

static inline GK_VOID isp_ge0_ct_slope_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspGeGe0CtSlope1, GK_U32 uIspGeGe0CtSlope2)
{
    U_ISP_GE0_CT_SLOPE o_isp_ge0_ct_slope;
    o_isp_ge0_ct_slope.bits.isp_ge_ge0_ct_slope1 = uIspGeGe0CtSlope1;
    o_isp_ge0_ct_slope.bits.isp_ge_ge0_ct_slope2 = uIspGeGe0CtSlope2;
    pstBeReg->ISP_GE0_CT_SLOPE.u32 = o_isp_ge0_ct_slope.u32;
}

static inline GK_VOID isp_ge_mode_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspGeGrEn, GK_U32 uIspGeGbEn, GK_U32 uIspGeGrGbEn)
{
    U_ISP_GE_MODE o_isp_ge_mode;
    o_isp_ge_mode.bits.isp_ge_gr_en = uIspGeGrEn;
    o_isp_ge_mode.bits.isp_ge_gb_en = uIspGeGbEn;
    o_isp_ge_mode.bits.isp_ge_gr_gb_en = uIspGeGrGbEn;
    pstBeReg->ISP_GE_MODE.u32 = o_isp_ge_mode.u32;
}

static inline GK_VOID isp_ge_strength_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspGeStrength)
{
    U_ISP_GE_STRENGTH o_isp_ge_strength;
    o_isp_ge_strength.bits.isp_ge_strength = uIspGeStrength;
    pstBeReg->ISP_GE_STRENGTH.u32 = o_isp_ge_strength.u32;
}

static inline GK_VOID isp_fpn_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspFpnCalibOffline)
{
    U_ISP_FPN_CFG o_isp_fpn_cfg;
    o_isp_fpn_cfg.bits.isp_fpn_calib_offline = uIspFpnCalibOffline;
    pstBeReg->ISP_FPN_CFG.u32 = o_isp_fpn_cfg.u32;
}

static inline GK_VOID isp_fpn_calib_start_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspFpnCalibStart)
{
    U_ISP_FPN_CALIB_START o_isp_fpn_calib_start;
    o_isp_fpn_calib_start.bits.isp_fpn_calib_start = uIspFpnCalibStart;
    pstBeReg->ISP_FPN_CALIB_START.u32 = o_isp_fpn_calib_start.u32;
}

static inline GK_VOID isp_fpn_corr_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspFpnCorrect0En)
{
    U_ISP_FPN_CORR_CFG o_isp_fpn_corr_cfg;
    o_isp_fpn_corr_cfg.bits.isp_fpn_correct0_en = uIspFpnCorrect0En;
    pstBeReg->ISP_FPN_CORR_CFG.u32 = o_isp_fpn_corr_cfg.u32;
}

static inline GK_VOID isp_fpn_white_level_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspFpnWhiteLevel)
{
    U_ISP_FPN_WHITE_LEVEL o_isp_fpn_white_level;
    o_isp_fpn_white_level.bits.isp_fpn_white_level = uIspFpnWhiteLevel;
    pstBeReg->ISP_FPN_WHITE_LEVEL.u32 = o_isp_fpn_white_level.u32;
}

static inline GK_VOID isp_fpn_divcoef_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspFpnDivcoef)
{
    U_ISP_FPN_DIVCOEF o_isp_fpn_divcoef;
    o_isp_fpn_divcoef.bits.isp_fpn_divcoef = uIspFpnDivcoef;
    pstBeReg->ISP_FPN_DIVCOEF.u32 = o_isp_fpn_divcoef.u32;
}

static inline GK_VOID isp_fpn_framelog2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspFpnFramelog2)
{
    U_ISP_FPN_FRAMELOG2 o_isp_fpn_framelog2;
    o_isp_fpn_framelog2.bits.isp_fpn_framelog2 = uIspFpnFramelog2;
    pstBeReg->ISP_FPN_FRAMELOG2.u32 = o_isp_fpn_framelog2.u32;
}

static inline GK_VOID isp_fpn_corr0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspFpnOffset0, GK_U32 uIspFpnStrength0)
{
    U_ISP_FPN_CORR0 o_isp_fpn_corr0;
    o_isp_fpn_corr0.bits.isp_fpn_offset0 = uIspFpnOffset0;
    o_isp_fpn_corr0.bits.isp_fpn_strength0 = uIspFpnStrength0;
    pstBeReg->ISP_FPN_CORR0.u32 = o_isp_fpn_corr0.u32;
}

static inline GK_VOID isp_fpn_shift_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspFpnShift, GK_U32 uIspFpnInShift, GK_U32 uIspFpnOutShift, GK_U32 uIspFpnFrameCalibShift)
{
    U_ISP_FPN_SHIFT o_isp_fpn_shift;
    o_isp_fpn_shift.bits.isp_fpn_shift = uIspFpnShift;
    o_isp_fpn_shift.bits.isp_fpn_in_shift = uIspFpnInShift;
    o_isp_fpn_shift.bits.isp_fpn_out_shift = uIspFpnOutShift;
    o_isp_fpn_shift.bits.isp_fpn_frame_calib_shift = uIspFpnFrameCalibShift;
    pstBeReg->ISP_FPN_SHIFT.u32 = o_isp_fpn_shift.u32;
}

static inline GK_VOID isp_fpn_max_o_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspFpnMaxO)
{
    U_ISP_FPN_MAX_O o_isp_fpn_max_o;
    o_isp_fpn_max_o.bits.isp_fpn_max_o = uIspFpnMaxO;
    pstBeReg->ISP_FPN_MAX_O.u32 = o_isp_fpn_max_o.u32;
}

static inline GK_VOID isp_fpn_overflowthr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspFpnOverflowthr)
{
    U_ISP_FPN_OVERFLOWTHR o_isp_fpn_overflowthr;
    o_isp_fpn_overflowthr.bits.isp_fpn_overflowthr = uIspFpnOverflowthr;
    pstBeReg->ISP_FPN_OVERFLOWTHR.u32 = o_isp_fpn_overflowthr.u32;
}

static inline GK_VOID isp_fpn_black_frame_out_sel_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspFpnData0OutSel)
{
    U_ISP_FPN_BLACK_FRAME_OUT_SEL o_isp_fpn_black_frame_out_sel;
    o_isp_fpn_black_frame_out_sel.bits.isp_fpn_data0_out_sel = uIspFpnData0OutSel;
    pstBeReg->ISP_FPN_BLACK_FRAME_OUT_SEL.u32 = o_isp_fpn_black_frame_out_sel.u32;
}

static inline GK_VOID isp_fpn_overlap_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspFpnOverlapWidthL, GK_U32 uIspFpnOverlapWidthR)
{
    U_ISP_FPN_OVERLAP_CFG o_isp_fpn_overlap_cfg;
    o_isp_fpn_overlap_cfg.bits.isp_fpn_overlap_width_l = uIspFpnOverlapWidthL;
    o_isp_fpn_overlap_cfg.bits.isp_fpn_overlap_width_r = uIspFpnOverlapWidthR;
    pstBeReg->ISP_FPN_OVERLAP_CFG.u32 = o_isp_fpn_overlap_cfg.u32;
}

static inline GK_VOID isp_fpn_calib_offline_set_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspFpnCalibOfflineSet)
{
    U_ISP_FPN_CALIB_OFFLINE_SET o_isp_fpn_calib_offline_set;
    o_isp_fpn_calib_offline_set.bits.isp_fpn_calib_offline_set = uIspFpnCalibOfflineSet;
    pstBeReg->ISP_FPN_CALIB_OFFLINE_SET.u32 = o_isp_fpn_calib_offline_set.u32;
}

static inline GK_VOID isp_fpn_calib_stat_clear_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspFpnCalibStatClear)
{
    U_ISP_FPN_CALIB_STAT_CLEAR o_isp_fpn_calib_stat_clear;
    o_isp_fpn_calib_stat_clear.bits.isp_fpn_calib_stat_clear = uIspFpnCalibStatClear;
    pstBeReg->ISP_FPN_CALIB_STAT_CLEAR.u32 = o_isp_fpn_calib_stat_clear.u32;
}

static inline GK_VOID isp_dpc_alpha_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcAlpha1G, GK_U32 uIspDpcAlpha1Rb, GK_U32 uIspDpcAlpha0G, GK_U32 uIspDpcAlpha0Rb)
{
    U_ISP_DPC_ALPHA o_isp_dpc_alpha;
    o_isp_dpc_alpha.bits.isp_dpc_alpha1_g = uIspDpcAlpha1G;
    o_isp_dpc_alpha.bits.isp_dpc_alpha1_rb = uIspDpcAlpha1Rb;
    o_isp_dpc_alpha.bits.isp_dpc_alpha0_g = uIspDpcAlpha0G;
    o_isp_dpc_alpha.bits.isp_dpc_alpha0_rb = uIspDpcAlpha0Rb;
    pstBeReg->ISP_DPC_ALPHA.u32 = o_isp_dpc_alpha.u32;
}

static inline GK_VOID isp_dpc_mode_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcDetEn, GK_U32 uIspDpcCorEn, GK_U32 uIspDpcGrayscaleMode, GK_U32 uIspDpcDpHightlightEn, GK_U32 uIspDpcIrPosition, GK_U32 uIspDpcIrChannel)
{
    U_ISP_DPC_MODE o_isp_dpc_mode;
    o_isp_dpc_mode.bits.isp_dpc_det_en = uIspDpcDetEn;
    o_isp_dpc_mode.bits.isp_dpc_cor_en = uIspDpcCorEn;
    o_isp_dpc_mode.bits.isp_dpc_grayscale_mode = uIspDpcGrayscaleMode;
    o_isp_dpc_mode.bits.isp_dpc_dpghtlight_en = uIspDpcDpHightlightEn;
    o_isp_dpc_mode.bits.isp_dpc_ir_position = uIspDpcIrPosition;
    o_isp_dpc_mode.bits.isp_dpc_ir_channel = uIspDpcIrChannel;
    pstBeReg->ISP_DPC_MODE.u32 = o_isp_dpc_mode.u32;
}

static inline GK_VOID isp_dpc_output_mode_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcStage1InclGrCenter, GK_U32 uIspDpcStage1InclRbCenter, GK_U32 uIspDpcStage1InclG3x3, GK_U32 uIspDpcStage1InclRb3x3)
{
    U_ISP_DPC_OUTPUT_MODE o_isp_dpc_output_mode;
    o_isp_dpc_output_mode.bits.isp_dpc_stage1_incl_gr_center = uIspDpcStage1InclGrCenter;
    o_isp_dpc_output_mode.bits.isp_dpc_stage1_incl_rb_center = uIspDpcStage1InclRbCenter;
    o_isp_dpc_output_mode.bits.isp_dpc_stage1_incl_g_3x3 = uIspDpcStage1InclG3x3;
    o_isp_dpc_output_mode.bits.isp_dpc_stage1_incl_rb_3x3 = uIspDpcStage1InclRb3x3;
    pstBeReg->ISP_DPC_OUTPUT_MODE.u32 = o_isp_dpc_output_mode.u32;
}

static inline GK_VOID isp_dpc_set_use_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcStage1UseSet1, GK_U32 uIspDpcStage1UseSet2, GK_U32 uIspDpcStage1UseSet3, GK_U32 uIspDpcStage1UseFixSet)
{
    U_ISP_DPC_SET_USE o_isp_dpc_set_use;
    o_isp_dpc_set_use.bits.isp_dpc_stage1_use_set1 = uIspDpcStage1UseSet1;
    o_isp_dpc_set_use.bits.isp_dpc_stage1_use_set2 = uIspDpcStage1UseSet2;
    o_isp_dpc_set_use.bits.isp_dpc_stage1_use_set3 = uIspDpcStage1UseSet3;
    o_isp_dpc_set_use.bits.isp_dpc_stage1_use_fix_set = uIspDpcStage1UseFixSet;
    pstBeReg->ISP_DPC_SET_USE.u32 = o_isp_dpc_set_use.u32;
}

static inline GK_VOID isp_dpc_methods_set_1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcPgGreen1Enable, GK_U32 uIspDpcLcGreen1Enable, GK_U32 uIspDpcRoGreen1Enable, GK_U32 uIspDpcRndGreen1Enable, GK_U32 uIspDpcRgGreen1Enable, GK_U32 uIspDpcPgRedBlue1Enable, GK_U32 uIspDpcLcRedBlue1Enable, GK_U32 uIspDpcRoRedBlue1Enable, GK_U32 uIspDpcRndRedBlue1Enable, GK_U32 uIspDpcRgRedBlue1Enable)
{
    U_ISP_DPC_METHODS_SET_1 o_isp_dpc_methods_set_1;
    o_isp_dpc_methods_set_1.bits.isp_dpc_pg_green1_enable = uIspDpcPgGreen1Enable;
    o_isp_dpc_methods_set_1.bits.isp_dpc_lc_green1_enable = uIspDpcLcGreen1Enable;
    o_isp_dpc_methods_set_1.bits.isp_dpc_ro_green1_enable = uIspDpcRoGreen1Enable;
    o_isp_dpc_methods_set_1.bits.isp_dpc_rnd_green1_enable = uIspDpcRndGreen1Enable;
    o_isp_dpc_methods_set_1.bits.isp_dpc_rg_green1_enable = uIspDpcRgGreen1Enable;
    o_isp_dpc_methods_set_1.bits.isp_dpc_pg_red_blue1_enable = uIspDpcPgRedBlue1Enable;
    o_isp_dpc_methods_set_1.bits.isp_dpc_lc_red_blue1_enable = uIspDpcLcRedBlue1Enable;
    o_isp_dpc_methods_set_1.bits.isp_dpc_ro_red_blue1_enable = uIspDpcRoRedBlue1Enable;
    o_isp_dpc_methods_set_1.bits.isp_dpc_rnd_red_blue1_enable = uIspDpcRndRedBlue1Enable;
    o_isp_dpc_methods_set_1.bits.isp_dpc_rg_red_blue1_enable = uIspDpcRgRedBlue1Enable;
    pstBeReg->ISP_DPC_METHODS_SET_1.u32 = o_isp_dpc_methods_set_1.u32;
}

static inline GK_VOID isp_dpc_methods_set_2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcPgGreen2Enable, GK_U32 uIspDpcLcGreen2Enable, GK_U32 uIspDpcRoGreen2Enable, GK_U32 uIspDpcRndGreen2Enable, GK_U32 uIspDpcRgGreen2Enable, GK_U32 uIspDpcPgRedBlue2Enable, GK_U32 uIspDpcLcRedBlue2Enable, GK_U32 uIspDpcRoRedBlue2Enable, GK_U32 uIspDpcRndRedBlue2Enable, GK_U32 uIspDpcRgRedBlue2Enable)
{
    U_ISP_DPC_METHODS_SET_2 o_isp_dpc_methods_set_2;
    o_isp_dpc_methods_set_2.bits.isp_dpc_pg_green2_enable = uIspDpcPgGreen2Enable;
    o_isp_dpc_methods_set_2.bits.isp_dpc_lc_green2_enable = uIspDpcLcGreen2Enable;
    o_isp_dpc_methods_set_2.bits.isp_dpc_ro_green2_enable = uIspDpcRoGreen2Enable;
    o_isp_dpc_methods_set_2.bits.isp_dpc_rnd_green2_enable = uIspDpcRndGreen2Enable;
    o_isp_dpc_methods_set_2.bits.isp_dpc_rg_green2_enable = uIspDpcRgGreen2Enable;
    o_isp_dpc_methods_set_2.bits.isp_dpc_pg_red_blue2_enable = uIspDpcPgRedBlue2Enable;
    o_isp_dpc_methods_set_2.bits.isp_dpc_lc_red_blue2_enable = uIspDpcLcRedBlue2Enable;
    o_isp_dpc_methods_set_2.bits.isp_dpc_ro_red_blue2_enable = uIspDpcRoRedBlue2Enable;
    o_isp_dpc_methods_set_2.bits.isp_dpc_rnd_red_blue2_enable = uIspDpcRndRedBlue2Enable;
    o_isp_dpc_methods_set_2.bits.isp_dpc_rg_red_blue2_enable = uIspDpcRgRedBlue2Enable;
    pstBeReg->ISP_DPC_METHODS_SET_2.u32 = o_isp_dpc_methods_set_2.u32;
}

static inline GK_VOID isp_dpc_methods_set_3_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcPgGreen3Enable, GK_U32 uIspDpcLcGreen3Enable, GK_U32 uIspDpcRoGreen3Enable, GK_U32 uIspDpcRndGreen3Enable, GK_U32 uIspDpcRgGreen3Enable, GK_U32 uIspDpcPgRedBlue3Enable, GK_U32 uIspDpcLcRedBlue3Enable, GK_U32 uIspDpcRoRedBlue3Enable, GK_U32 uIspDpcRndRedBlue3Enable, GK_U32 uIspDpcRgRedBlue3Enable)
{
    U_ISP_DPC_METHODS_SET_3 o_isp_dpc_methods_set_3;
    o_isp_dpc_methods_set_3.bits.isp_dpc_pg_green3_enable = uIspDpcPgGreen3Enable;
    o_isp_dpc_methods_set_3.bits.isp_dpc_lc_green3_enable = uIspDpcLcGreen3Enable;
    o_isp_dpc_methods_set_3.bits.isp_dpc_ro_green3_enable = uIspDpcRoGreen3Enable;
    o_isp_dpc_methods_set_3.bits.isp_dpc_rnd_green3_enable = uIspDpcRndGreen3Enable;
    o_isp_dpc_methods_set_3.bits.isp_dpc_rg_green3_enable = uIspDpcRgGreen3Enable;
    o_isp_dpc_methods_set_3.bits.isp_dpc_pg_red_blue3_enable = uIspDpcPgRedBlue3Enable;
    o_isp_dpc_methods_set_3.bits.isp_dpc_lc_red_blue3_enable = uIspDpcLcRedBlue3Enable;
    o_isp_dpc_methods_set_3.bits.isp_dpc_ro_red_blue3_enable = uIspDpcRoRedBlue3Enable;
    o_isp_dpc_methods_set_3.bits.isp_dpc_rnd_red_blue3_enable = uIspDpcRndRedBlue3Enable;
    o_isp_dpc_methods_set_3.bits.isp_dpc_rg_red_blue3_enable = uIspDpcRgRedBlue3Enable;
    pstBeReg->ISP_DPC_METHODS_SET_3.u32 = o_isp_dpc_methods_set_3.u32;
}

static inline GK_VOID isp_dpc_line_thresh_1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcLineThr1G, GK_U32 uIspDpcLineThr1Rb)
{
    U_ISP_DPC_LINE_THRESH_1 o_isp_dpc_line_thresh_1;
    o_isp_dpc_line_thresh_1.bits.isp_dpc_line_thr1_g = uIspDpcLineThr1G;
    o_isp_dpc_line_thresh_1.bits.isp_dpc_line_thr1_rb = uIspDpcLineThr1Rb;
    pstBeReg->ISP_DPC_LINE_THRESH_1.u32 = o_isp_dpc_line_thresh_1.u32;
}

static inline GK_VOID isp_dpc_line_mad_fac_1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcLineMadFac1G, GK_U32 uIspDpcLineMadFac1Rb)
{
    U_ISP_DPC_LINE_MAD_FAC_1 o_isp_dpc_line_mad_fac_1;
    o_isp_dpc_line_mad_fac_1.bits.isp_dpc_line_mad_fac_1_g = uIspDpcLineMadFac1G;
    o_isp_dpc_line_mad_fac_1.bits.isp_dpc_line_mad_fac_1_rb = uIspDpcLineMadFac1Rb;
    pstBeReg->ISP_DPC_LINE_MAD_FAC_1.u32 = o_isp_dpc_line_mad_fac_1.u32;
}

static inline GK_VOID isp_dpc_pg_fac_1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcPgFac1G, GK_U32 uIspDpcPgFac1Rb)
{
    U_ISP_DPC_PG_FAC_1 o_isp_dpc_pg_fac_1;
    o_isp_dpc_pg_fac_1.bits.isp_dpc_pg_fac_1_g = uIspDpcPgFac1G;
    o_isp_dpc_pg_fac_1.bits.isp_dpc_pg_fac_1_rb = uIspDpcPgFac1Rb;
    pstBeReg->ISP_DPC_PG_FAC_1.u32 = o_isp_dpc_pg_fac_1.u32;
}

static inline GK_VOID isp_dpc_rnd_thresh_1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcRndThr1G, GK_U32 uIspDpcRndThr1Rb)
{
    U_ISP_DPC_RND_THRESH_1 o_isp_dpc_rnd_thresh_1;
    o_isp_dpc_rnd_thresh_1.bits.isp_dpc_rnd_thr1_g = uIspDpcRndThr1G;
    o_isp_dpc_rnd_thresh_1.bits.isp_dpc_rnd_thr1_rb = uIspDpcRndThr1Rb;
    pstBeReg->ISP_DPC_RND_THRESH_1.u32 = o_isp_dpc_rnd_thresh_1.u32;
}

static inline GK_VOID isp_dpc_rg_fac_1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcRgFac1G, GK_U32 uIspDpcRgFac1Rb)
{
    U_ISP_DPC_RG_FAC_1 o_isp_dpc_rg_fac_1;
    o_isp_dpc_rg_fac_1.bits.isp_dpc_rg_fac_1_g = uIspDpcRgFac1G;
    o_isp_dpc_rg_fac_1.bits.isp_dpc_rg_fac_1_rb = uIspDpcRgFac1Rb;
    pstBeReg->ISP_DPC_RG_FAC_1.u32 = o_isp_dpc_rg_fac_1.u32;
}

static inline GK_VOID isp_dpc_line_thresh_2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcLineThr2G, GK_U32 uIspDpcLineThr2Rb)
{
    U_ISP_DPC_LINE_THRESH_2 o_isp_dpc_line_thresh_2;
    o_isp_dpc_line_thresh_2.bits.isp_dpc_line_thr2_g = uIspDpcLineThr2G;
    o_isp_dpc_line_thresh_2.bits.isp_dpc_line_thr2_rb = uIspDpcLineThr2Rb;
    pstBeReg->ISP_DPC_LINE_THRESH_2.u32 = o_isp_dpc_line_thresh_2.u32;
}

static inline GK_VOID isp_dpc_line_mad_fac_2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcLineMadFac2G, GK_U32 uIspDpcLineMadFac2Rb)
{
    U_ISP_DPC_LINE_MAD_FAC_2 o_isp_dpc_line_mad_fac_2;
    o_isp_dpc_line_mad_fac_2.bits.isp_dpc_line_mad_fac_2_g = uIspDpcLineMadFac2G;
    o_isp_dpc_line_mad_fac_2.bits.isp_dpc_line_mad_fac_2_rb = uIspDpcLineMadFac2Rb;
    pstBeReg->ISP_DPC_LINE_MAD_FAC_2.u32 = o_isp_dpc_line_mad_fac_2.u32;
}

static inline GK_VOID isp_dpc_pg_fac_2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcPgFac2G, GK_U32 uIspDpcPgFac2Rb)
{
    U_ISP_DPC_PG_FAC_2 o_isp_dpc_pg_fac_2;
    o_isp_dpc_pg_fac_2.bits.isp_dpc_pg_fac_2_g = uIspDpcPgFac2G;
    o_isp_dpc_pg_fac_2.bits.isp_dpc_pg_fac_2_rb = uIspDpcPgFac2Rb;
    pstBeReg->ISP_DPC_PG_FAC_2.u32 = o_isp_dpc_pg_fac_2.u32;
}

static inline GK_VOID isp_dpc_rnd_thresh_2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcRndThr2G, GK_U32 uIspDpcRndThr2Rb)
{
    U_ISP_DPC_RND_THRESH_2 o_isp_dpc_rnd_thresh_2;
    o_isp_dpc_rnd_thresh_2.bits.isp_dpc_rnd_thr2_g = uIspDpcRndThr2G;
    o_isp_dpc_rnd_thresh_2.bits.isp_dpc_rnd_thr2_rb = uIspDpcRndThr2Rb;
    pstBeReg->ISP_DPC_RND_THRESH_2.u32 = o_isp_dpc_rnd_thresh_2.u32;
}

static inline GK_VOID isp_dpc_rg_fac_2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcRgFac2G, GK_U32 uIspDpcRgFac2Rb)
{
    U_ISP_DPC_RG_FAC_2 o_isp_dpc_rg_fac_2;
    o_isp_dpc_rg_fac_2.bits.isp_dpc_rg_fac_2_g = uIspDpcRgFac2G;
    o_isp_dpc_rg_fac_2.bits.isp_dpc_rg_fac_2_rb = uIspDpcRgFac2Rb;
    pstBeReg->ISP_DPC_RG_FAC_2.u32 = o_isp_dpc_rg_fac_2.u32;
}

static inline GK_VOID isp_dpc_line_thresh_3_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcLineThr3G, GK_U32 uIspDpcLineThr3Rb)
{
    U_ISP_DPC_LINE_THRESH_3 o_isp_dpc_line_thresh_3;
    o_isp_dpc_line_thresh_3.bits.isp_dpc_line_thr3_g = uIspDpcLineThr3G;
    o_isp_dpc_line_thresh_3.bits.isp_dpc_line_thr3_rb = uIspDpcLineThr3Rb;
    pstBeReg->ISP_DPC_LINE_THRESH_3.u32 = o_isp_dpc_line_thresh_3.u32;
}

static inline GK_VOID isp_dpc_line_mad_fac_3_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcLineMadFac3G, GK_U32 uIspDpcLineMadFac3Rb)
{
    U_ISP_DPC_LINE_MAD_FAC_3 o_isp_dpc_line_mad_fac_3;
    o_isp_dpc_line_mad_fac_3.bits.isp_dpc_line_mad_fac_3_g = uIspDpcLineMadFac3G;
    o_isp_dpc_line_mad_fac_3.bits.isp_dpc_line_mad_fac_3_rb = uIspDpcLineMadFac3Rb;
    pstBeReg->ISP_DPC_LINE_MAD_FAC_3.u32 = o_isp_dpc_line_mad_fac_3.u32;
}

static inline GK_VOID isp_dpc_pg_fac_3_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcPgFac3G, GK_U32 uIspDpcPgFac3Rb)
{
    U_ISP_DPC_PG_FAC_3 o_isp_dpc_pg_fac_3;
    o_isp_dpc_pg_fac_3.bits.isp_dpc_pg_fac_3_g = uIspDpcPgFac3G;
    o_isp_dpc_pg_fac_3.bits.isp_dpc_pg_fac_3_rb = uIspDpcPgFac3Rb;
    pstBeReg->ISP_DPC_PG_FAC_3.u32 = o_isp_dpc_pg_fac_3.u32;
}

static inline GK_VOID isp_dpc_rnd_thresh_3_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcRndThr3G, GK_U32 uIspDpcRndThr3Rb)
{
    U_ISP_DPC_RND_THRESH_3 o_isp_dpc_rnd_thresh_3;
    o_isp_dpc_rnd_thresh_3.bits.isp_dpc_rnd_thr3_g = uIspDpcRndThr3G;
    o_isp_dpc_rnd_thresh_3.bits.isp_dpc_rnd_thr3_rb = uIspDpcRndThr3Rb;
    pstBeReg->ISP_DPC_RND_THRESH_3.u32 = o_isp_dpc_rnd_thresh_3.u32;
}

static inline GK_VOID isp_dpc_rg_fac_3_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcRgFac3G, GK_U32 uIspDpcRgFac3Rb)
{
    U_ISP_DPC_RG_FAC_3 o_isp_dpc_rg_fac_3;
    o_isp_dpc_rg_fac_3.bits.isp_dpc_rg_fac_3_g = uIspDpcRgFac3G;
    o_isp_dpc_rg_fac_3.bits.isp_dpc_rg_fac_3_rb = uIspDpcRgFac3Rb;
    pstBeReg->ISP_DPC_RG_FAC_3.u32 = o_isp_dpc_rg_fac_3.u32;
}

static inline GK_VOID isp_dpc_ro_limits_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcRo1G, GK_U32 uIspDpcRo1Rb, GK_U32 uIspDpcRo2G, GK_U32 uIspDpcRo2Rb, GK_U32 uIspDpcRo3G, GK_U32 uIspDpcRo3Rb)
{
    U_ISP_DPC_RO_LIMITS o_isp_dpc_ro_limits;
    o_isp_dpc_ro_limits.bits.isp_dpc_ro_1_g = uIspDpcRo1G;
    o_isp_dpc_ro_limits.bits.isp_dpc_ro_1_rb = uIspDpcRo1Rb;
    o_isp_dpc_ro_limits.bits.isp_dpc_ro_2_g = uIspDpcRo2G;
    o_isp_dpc_ro_limits.bits.isp_dpc_ro_2_rb = uIspDpcRo2Rb;
    o_isp_dpc_ro_limits.bits.isp_dpc_ro_3_g = uIspDpcRo3G;
    o_isp_dpc_ro_limits.bits.isp_dpc_ro_3_rb = uIspDpcRo3Rb;
    pstBeReg->ISP_DPC_RO_LIMITS.u32 = o_isp_dpc_ro_limits.u32;
}

static inline GK_VOID isp_dpc_rnd_offs_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcRndOffs1G, GK_U32 uIspDpcRndOffs1Rb, GK_U32 uIspDpcRndOffs2G, GK_U32 uIspDpcRndOffs2Rb, GK_U32 uIspDpcRndOffs3G, GK_U32 uIspDpcRndOffs3Rb)
{
    U_ISP_DPC_RND_OFFS o_isp_dpc_rnd_offs;
    o_isp_dpc_rnd_offs.bits.isp_dpc_rnd_offs_1_g = uIspDpcRndOffs1G;
    o_isp_dpc_rnd_offs.bits.isp_dpc_rnd_offs_1_rb = uIspDpcRndOffs1Rb;
    o_isp_dpc_rnd_offs.bits.isp_dpc_rnd_offs_2_g = uIspDpcRndOffs2G;
    o_isp_dpc_rnd_offs.bits.isp_dpc_rnd_offs_2_rb = uIspDpcRndOffs2Rb;
    o_isp_dpc_rnd_offs.bits.isp_dpc_rnd_offs_3_g = uIspDpcRndOffs3G;
    o_isp_dpc_rnd_offs.bits.isp_dpc_rnd_offs_3_rb = uIspDpcRndOffs3Rb;
    pstBeReg->ISP_DPC_RND_OFFS.u32 = o_isp_dpc_rnd_offs.u32;
}

static inline GK_VOID isp_dpc_bpt_ctrl_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcBpData)
{
    U_ISP_DPC_BPT_CTRL o_isp_dpc_bpt_ctrl;
    o_isp_dpc_bpt_ctrl.bits.isp_dpc_bp_data = uIspDpcBpData;
    pstBeReg->ISP_DPC_BPT_CTRL.u32 = o_isp_dpc_bpt_ctrl.u32;
}

static inline GK_VOID isp_dpc_rnd_thresh_1_mtp_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcRndThr1GMtp, GK_U32 uIspDpcRndThr1RbMtp)
{
    U_ISP_DPC_RND_THRESH_1_MTP o_isp_dpc_rnd_thresh_1_mtp;
    o_isp_dpc_rnd_thresh_1_mtp.bits.isp_dpc_rnd_thr1_g_mtp = uIspDpcRndThr1GMtp;
    o_isp_dpc_rnd_thresh_1_mtp.bits.isp_dpc_rnd_thr1_rb_mtp = uIspDpcRndThr1RbMtp;
    pstBeReg->ISP_DPC_RND_THRESH_1_MTP.u32 = o_isp_dpc_rnd_thresh_1_mtp.u32;
}

static inline GK_VOID isp_dpc_rnd_thresh_2_mtp_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcRndThr2GMtp, GK_U32 uIspDpcRndThr2RbMtp)
{
    U_ISP_DPC_RND_THRESH_2_MTP o_isp_dpc_rnd_thresh_2_mtp;
    o_isp_dpc_rnd_thresh_2_mtp.bits.isp_dpc_rnd_thr2_g_mtp = uIspDpcRndThr2GMtp;
    o_isp_dpc_rnd_thresh_2_mtp.bits.isp_dpc_rnd_thr2_rb_mtp = uIspDpcRndThr2RbMtp;
    pstBeReg->ISP_DPC_RND_THRESH_2_MTP.u32 = o_isp_dpc_rnd_thresh_2_mtp.u32;
}

static inline GK_VOID isp_dpc_rnd_thresh_3_mtp_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcRndThr3GMtp, GK_U32 uIspDpcRndThr3RbMtp)
{
    U_ISP_DPC_RND_THRESH_3_MTP o_isp_dpc_rnd_thresh_3_mtp;
    o_isp_dpc_rnd_thresh_3_mtp.bits.isp_dpc_rnd_thr3_g_mtp = uIspDpcRndThr3GMtp;
    o_isp_dpc_rnd_thresh_3_mtp.bits.isp_dpc_rnd_thr3_rb_mtp = uIspDpcRndThr3RbMtp;
    pstBeReg->ISP_DPC_RND_THRESH_3_MTP.u32 = o_isp_dpc_rnd_thresh_3_mtp.u32;
}

static inline GK_VOID isp_dpc_soft_thr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcExSoftThrMin, GK_U32 uIspDpcExSoftThrMax)
{
    U_ISP_DPC_SOFT_THR o_isp_dpc_soft_thr;
    o_isp_dpc_soft_thr.bits.isp_dpc_ex_soft_thr_min = uIspDpcExSoftThrMin;
    o_isp_dpc_soft_thr.bits.isp_dpc_ex_soft_thr_max = uIspDpcExSoftThrMax;
    pstBeReg->ISP_DPC_SOFT_THR.u32 = o_isp_dpc_soft_thr.u32;
}

static inline GK_VOID isp_dpc_bhardthr_en_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcExHardThrEn)
{
    U_ISP_DPC_BHARDTHR_EN o_isp_dpc_bhardthr_en;
    o_isp_dpc_bhardthr_en.bits.isp_dpc_ex_hard_thr_en = uIspDpcExHardThrEn;
    pstBeReg->ISP_DPC_BHARDTHR_EN.u32 = o_isp_dpc_bhardthr_en.u32;
}

static inline GK_VOID isp_dpc_rakeratio_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcExRakeRatio)
{
    U_ISP_DPC_RAKERATIO o_isp_dpc_rakeratio;
    o_isp_dpc_rakeratio.bits.isp_dpc_ex_rake_ratio = uIspDpcExRakeRatio;
    pstBeReg->ISP_DPC_RAKERATIO.u32 = o_isp_dpc_rakeratio.u32;
}

static inline GK_VOID isp_dpc_lch_thr_1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcLineAverFac1, GK_U32 uIspDpcLineDiffThr1, GK_U32 uIspDpcLineStdThr1)
{
    U_ISP_DPC_LCH_THR_1 o_isp_dpc_lch_thr_1;
    o_isp_dpc_lch_thr_1.bits.isp_dpc_line_aver_fac_1 = uIspDpcLineAverFac1;
    o_isp_dpc_lch_thr_1.bits.isp_dpc_line_diff_thr_1 = uIspDpcLineDiffThr1;
    o_isp_dpc_lch_thr_1.bits.isp_dpc_line_std_thr_1 = uIspDpcLineStdThr1;
    pstBeReg->ISP_DPC_LCH_THR_1.u32 = o_isp_dpc_lch_thr_1.u32;
}

static inline GK_VOID isp_dpc_lch_thr_2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcLineAverFac2, GK_U32 uIspDpcLineDiffThr2, GK_U32 uIspDpcLineStdThr2)
{
    U_ISP_DPC_LCH_THR_2 o_isp_dpc_lch_thr_2;
    o_isp_dpc_lch_thr_2.bits.isp_dpc_line_aver_fac_2 = uIspDpcLineAverFac2;
    o_isp_dpc_lch_thr_2.bits.isp_dpc_line_diff_thr_2 = uIspDpcLineDiffThr2;
    o_isp_dpc_lch_thr_2.bits.isp_dpc_line_std_thr_2 = uIspDpcLineStdThr2;
    pstBeReg->ISP_DPC_LCH_THR_2.u32 = o_isp_dpc_lch_thr_2.u32;
}

static inline GK_VOID isp_dpc_lch_thr_3_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcLineAverFac3, GK_U32 uIspDpcLineDiffThr3, GK_U32 uIspDpcLineStdThr3)
{
    U_ISP_DPC_LCH_THR_3 o_isp_dpc_lch_thr_3;
    o_isp_dpc_lch_thr_3.bits.isp_dpc_line_aver_fac_3 = uIspDpcLineAverFac3;
    o_isp_dpc_lch_thr_3.bits.isp_dpc_line_diff_thr_3 = uIspDpcLineDiffThr3;
    o_isp_dpc_lch_thr_3.bits.isp_dpc_line_std_thr_3 = uIspDpcLineStdThr3;
    pstBeReg->ISP_DPC_LCH_THR_3.u32 = o_isp_dpc_lch_thr_3.u32;
}

static inline GK_VOID isp_dpc_lch_thr_4_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcLineAverFac4, GK_U32 uIspDpcLineDiffThr4, GK_U32 uIspDpcLineStdThr4)
{
    U_ISP_DPC_LCH_THR_4 o_isp_dpc_lch_thr_4;
    o_isp_dpc_lch_thr_4.bits.isp_dpc_line_aver_fac_4 = uIspDpcLineAverFac4;
    o_isp_dpc_lch_thr_4.bits.isp_dpc_line_diff_thr_4 = uIspDpcLineDiffThr4;
    o_isp_dpc_lch_thr_4.bits.isp_dpc_line_std_thr_4 = uIspDpcLineStdThr4;
    pstBeReg->ISP_DPC_LCH_THR_4.u32 = o_isp_dpc_lch_thr_4.u32;
}

static inline GK_VOID isp_dpc_lch_thr_5_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcLineAverFac5, GK_U32 uIspDpcLineDiffThr5, GK_U32 uIspDpcLineStdThr5)
{
    U_ISP_DPC_LCH_THR_5 o_isp_dpc_lch_thr_5;
    o_isp_dpc_lch_thr_5.bits.isp_dpc_line_aver_fac_5 = uIspDpcLineAverFac5;
    o_isp_dpc_lch_thr_5.bits.isp_dpc_line_diff_thr_5 = uIspDpcLineDiffThr5;
    o_isp_dpc_lch_thr_5.bits.isp_dpc_line_std_thr_5 = uIspDpcLineStdThr5;
    pstBeReg->ISP_DPC_LCH_THR_5.u32 = o_isp_dpc_lch_thr_5.u32;
}

static inline GK_VOID isp_dpc_line_kerdiff_fac_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcLineKerdiffFac)
{
    U_ISP_DPC_LINE_KERDIFF_FAC o_isp_dpc_line_kerdiff_fac;
    o_isp_dpc_line_kerdiff_fac.bits.isp_dpc_line_kerdiff_fac = uIspDpcLineKerdiffFac;
    pstBeReg->ISP_DPC_LINE_KERDIFF_FAC.u32 = o_isp_dpc_line_kerdiff_fac.u32;
}

static inline GK_VOID isp_dpc_blend_mode_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcBlendMode)
{
    U_ISP_DPC_BLEND_MODE o_isp_dpc_blend_mode;
    o_isp_dpc_blend_mode.bits.isp_dpc_blend_mode = uIspDpcBlendMode;
    pstBeReg->ISP_DPC_BLEND_MODE.u32 = o_isp_dpc_blend_mode.u32;
}

static inline GK_VOID isp_dpc_bit_depth_sel_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcBitDepthSel)
{
    U_ISP_DPC_BIT_DEPTH_SEL o_isp_dpc_bit_depth_sel;
    o_isp_dpc_bit_depth_sel.bits.isp_dpc_bit_depth_sel = uIspDpcBitDepthSel;
    pstBeReg->ISP_DPC_BIT_DEPTH_SEL.u32 = o_isp_dpc_bit_depth_sel.u32;
}

static inline GK_VOID isp_dpc_rg_fac_1_mtp_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcRgFac1GMtp, GK_U32 uIspDpcRgFac1RbMtp)
{
    U_ISP_DPC_RG_FAC_1_MTP o_isp_dpc_rg_fac_1_mtp;
    o_isp_dpc_rg_fac_1_mtp.bits.isp_dpc_rg_fac_1_g_mtp = uIspDpcRgFac1GMtp;
    o_isp_dpc_rg_fac_1_mtp.bits.isp_dpc_rg_fac_1_rb_mtp = uIspDpcRgFac1RbMtp;
    pstBeReg->ISP_DPC_RG_FAC_1_MTP.u32 = o_isp_dpc_rg_fac_1_mtp.u32;
}

static inline GK_VOID isp_dpc_rg_fac_2_mtp_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcRgFac2GMtp, GK_U32 uIspDpcRgFac2RbMtp)
{
    U_ISP_DPC_RG_FAC_2_MTP o_isp_dpc_rg_fac_2_mtp;
    o_isp_dpc_rg_fac_2_mtp.bits.isp_dpc_rg_fac_2_g_mtp = uIspDpcRgFac2GMtp;
    o_isp_dpc_rg_fac_2_mtp.bits.isp_dpc_rg_fac_2_rb_mtp = uIspDpcRgFac2RbMtp;
    pstBeReg->ISP_DPC_RG_FAC_2_MTP.u32 = o_isp_dpc_rg_fac_2_mtp.u32;
}

static inline GK_VOID isp_dpc_rg_fac_3_mtp_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDpcRgFac3GMtp, GK_U32 uIspDpcRgFac3RbMtp)
{
    U_ISP_DPC_RG_FAC_3_MTP o_isp_dpc_rg_fac_3_mtp;
    o_isp_dpc_rg_fac_3_mtp.bits.isp_dpc_rg_fac_3_g_mtp = uIspDpcRgFac3GMtp;
    o_isp_dpc_rg_fac_3_mtp.bits.isp_dpc_rg_fac_3_rb_mtp = uIspDpcRgFac3RbMtp;
    pstBeReg->ISP_DPC_RG_FAC_3_MTP.u32 = o_isp_dpc_rg_fac_3_mtp.u32;
}

static inline GK_VOID isp_demosaic_cfg1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicAhdEn, GK_U32 uIspDemosaicLocalCacEn, GK_U32 uIspDemosaicFcrEn, GK_U32 uIspDemosaicCacCorMode, GK_U32 uIspDemosaicDeFakeEn, GK_U32 uIspDemosaicDesatEnable)
{
    U_ISP_DEMOSAIC_CFG1 o_isp_demosaic_cfg1;
    o_isp_demosaic_cfg1.bits.isp_demosaic_ahd_en = uIspDemosaicAhdEn;
    o_isp_demosaic_cfg1.bits.isp_demosaic_local_cac_en = uIspDemosaicLocalCacEn;
    o_isp_demosaic_cfg1.bits.isp_demosaic_fcr_en = uIspDemosaicFcrEn;
    o_isp_demosaic_cfg1.bits.isp_demosaic_cac_cor_mode = uIspDemosaicCacCorMode;
    o_isp_demosaic_cfg1.bits.isp_demosaic_de_fake_en = uIspDemosaicDeFakeEn;
    o_isp_demosaic_cfg1.bits.isp_demosaic_desat_enable = uIspDemosaicDesatEnable;
    pstBeReg->ISP_DEMOSAIC_CFG1.u32 = o_isp_demosaic_cfg1.u32;
}

static inline GK_VOID isp_demosaic_coef0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicBldLimit2, GK_U32 uIspDemosaicBldLimit1)
{
    U_ISP_DEMOSAIC_COEF0 o_isp_demosaic_coef0;
    o_isp_demosaic_coef0.bits.isp_demosaic_bld_limit2 = uIspDemosaicBldLimit2;
    o_isp_demosaic_coef0.bits.isp_demosaic_bld_limit1 = uIspDemosaicBldLimit1;
    pstBeReg->ISP_DEMOSAIC_COEF0.u32 = o_isp_demosaic_coef0.u32;
}

static inline GK_VOID isp_demosaic_coef2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicFcrLimit2, GK_U32 uIspDemosaicFcrLimit1)
{
    U_ISP_DEMOSAIC_COEF2 o_isp_demosaic_coef2;
    o_isp_demosaic_coef2.bits.isp_demosaic_fcr_limit2 = uIspDemosaicFcrLimit2;
    o_isp_demosaic_coef2.bits.isp_demosaic_fcr_limit1 = uIspDemosaicFcrLimit1;
    pstBeReg->ISP_DEMOSAIC_COEF2.u32 = o_isp_demosaic_coef2.u32;
}

static inline GK_VOID isp_demosaic_coef3_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicAhdPar2, GK_U32 uIspDemosaicAhdPar1)
{
    U_ISP_DEMOSAIC_COEF3 o_isp_demosaic_coef3;
    o_isp_demosaic_coef3.bits.isp_demosaic_ahd_par2 = uIspDemosaicAhdPar2;
    o_isp_demosaic_coef3.bits.isp_demosaic_ahd_par1 = uIspDemosaicAhdPar1;
    pstBeReg->ISP_DEMOSAIC_COEF3.u32 = o_isp_demosaic_coef3.u32;
}

static inline GK_VOID isp_demosaic_coef4_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicFcrGain)
{
    U_ISP_DEMOSAIC_COEF4 o_isp_demosaic_coef4;
    o_isp_demosaic_coef4.bits.isp_demosaic_fcr_gain = uIspDemosaicFcrGain;
    pstBeReg->ISP_DEMOSAIC_COEF4.u32 = o_isp_demosaic_coef4.u32;
}

static inline GK_VOID isp_demosaic_coef5_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicHvRatio)
{
    U_ISP_DEMOSAIC_COEF5 o_isp_demosaic_coef5;
    o_isp_demosaic_coef5.bits.isp_demosaic_hv_ratio = uIspDemosaicHvRatio;
    pstBeReg->ISP_DEMOSAIC_COEF5.u32 = o_isp_demosaic_coef5.u32;
}

static inline GK_VOID isp_demosaic_coef6_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicFcrRatio)
{
    U_ISP_DEMOSAIC_COEF6 o_isp_demosaic_coef6;
    o_isp_demosaic_coef6.bits.isp_demosaic_fcr_ratio = uIspDemosaicFcrRatio;
    pstBeReg->ISP_DEMOSAIC_COEF6.u32 = o_isp_demosaic_coef6.u32;
}

static inline GK_VOID isp_demosaic_sel_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicHvSel)
{
    U_ISP_DEMOSAIC_SEL o_isp_demosaic_sel;
    o_isp_demosaic_sel.bits.isp_demosaic_hv_sel = uIspDemosaicHvSel;
    pstBeReg->ISP_DEMOSAIC_SEL.u32 = o_isp_demosaic_sel.u32;
}

static inline GK_VOID isp_demosaic_lcac_cnt_thr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicRCounterThr, GK_U32 uIspDemosaicBCounterThr, GK_U32 uIspDemosaicGCounterThr)
{
    U_ISP_DEMOSAIC_LCAC_CNT_THR o_isp_demosaic_lcac_cnt_thr;
    o_isp_demosaic_lcac_cnt_thr.bits.isp_demosaic_r_counter_thr = uIspDemosaicRCounterThr;
    o_isp_demosaic_lcac_cnt_thr.bits.isp_demosaic_b_counter_thr = uIspDemosaicBCounterThr;
    o_isp_demosaic_lcac_cnt_thr.bits.isp_demosaic_g_counter_thr = uIspDemosaicGCounterThr;
    pstBeReg->ISP_DEMOSAIC_LCAC_CNT_THR.u32 = o_isp_demosaic_lcac_cnt_thr.u32;
}

static inline GK_VOID isp_demosaic_lcac_luma_rb_thr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicRLumaThr, GK_U32 uIspDemosaicBLumaThr)
{
    U_ISP_DEMOSAIC_LCAC_LUMA_RB_THR o_isp_demosaic_lcac_luma_rb_thr;
    o_isp_demosaic_lcac_luma_rb_thr.bits.isp_demosaic_r_luma_thr = uIspDemosaicRLumaThr;
    o_isp_demosaic_lcac_luma_rb_thr.bits.isp_demosaic_b_luma_thr = uIspDemosaicBLumaThr;
    pstBeReg->ISP_DEMOSAIC_LCAC_LUMA_RB_THR.u32 = o_isp_demosaic_lcac_luma_rb_thr.u32;
}

static inline GK_VOID isp_demosaic_lcac_luma_g_thr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicGLumaThr)
{
    U_ISP_DEMOSAIC_LCAC_LUMA_G_THR o_isp_demosaic_lcac_luma_g_thr;
    o_isp_demosaic_lcac_luma_g_thr.bits.isp_demosaic_g_luma_thr = uIspDemosaicGLumaThr;
    pstBeReg->ISP_DEMOSAIC_LCAC_LUMA_G_THR.u32 = o_isp_demosaic_lcac_luma_g_thr.u32;
}

static inline GK_VOID isp_demosaic_purple_var_thr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicPurpleVarThr)
{
    U_ISP_DEMOSAIC_PURPLE_VAR_THR o_isp_demosaic_purple_var_thr;
    o_isp_demosaic_purple_var_thr.bits.isp_demosaic_purple_var_thr = uIspDemosaicPurpleVarThr;
    pstBeReg->ISP_DEMOSAIC_PURPLE_VAR_THR.u32 = o_isp_demosaic_purple_var_thr.u32;
}

static inline GK_VOID isp_demosaic_fake_cr_var_thr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicFakeCrVarThrLow, GK_U32 uIspDemosaicFakeCrVarThrHigh)
{
    U_ISP_DEMOSAIC_FAKE_CR_VAR_THR o_isp_demosaic_fake_cr_var_thr;
    o_isp_demosaic_fake_cr_var_thr.bits.isp_demosaic_fake_cr_var_thr_low = uIspDemosaicFakeCrVarThrLow;
    o_isp_demosaic_fake_cr_var_thr.bits.isp_demosaic_fake_cr_var_thrgh = uIspDemosaicFakeCrVarThrHigh;
    pstBeReg->ISP_DEMOSAIC_FAKE_CR_VAR_THR.u32 = o_isp_demosaic_fake_cr_var_thr.u32;
}

static inline GK_VOID isp_demosaic_depurplectr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicDepurplectrcb, GK_U32 uIspDemosaicDepurplectrcr)
{
    U_ISP_DEMOSAIC_DEPURPLECTR o_isp_demosaic_depurplectr;
    o_isp_demosaic_depurplectr.bits.isp_demosaic_depurplectrcb = uIspDemosaicDepurplectrcb;
    o_isp_demosaic_depurplectr.bits.isp_demosaic_depurplectrcr = uIspDemosaicDepurplectrcr;
    pstBeReg->ISP_DEMOSAIC_DEPURPLECTR.u32 = o_isp_demosaic_depurplectr.u32;
}

static inline GK_VOID isp_demosaic_lpf_coef_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicLpfF0, GK_U32 uIspDemosaicLpfF1, GK_U32 uIspDemosaicLpfF2, GK_U32 uIspDemosaicLpfF3)
{
    U_ISP_DEMOSAIC_LPF_COEF o_isp_demosaic_lpf_coef;
    o_isp_demosaic_lpf_coef.bits.isp_demosaic_lpf_f0 = uIspDemosaicLpfF0;
    o_isp_demosaic_lpf_coef.bits.isp_demosaic_lpf_f1 = uIspDemosaicLpfF1;
    o_isp_demosaic_lpf_coef.bits.isp_demosaic_lpf_f2 = uIspDemosaicLpfF2;
    o_isp_demosaic_lpf_coef.bits.isp_demosaic_lpf_f3 = uIspDemosaicLpfF3;
    pstBeReg->ISP_DEMOSAIC_LPF_COEF.u32 = o_isp_demosaic_lpf_coef.u32;
}

static inline GK_VOID isp_demosaic_g_intp_ctrl_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicGClipSftBit)
{
    U_ISP_DEMOSAIC_G_INTP_CTRL o_isp_demosaic_g_intp_ctrl;
    o_isp_demosaic_g_intp_ctrl.bits.isp_demosaic_g_clip_sft_bit = uIspDemosaicGClipSftBit;
    pstBeReg->ISP_DEMOSAIC_G_INTP_CTRL.u32 = o_isp_demosaic_g_intp_ctrl.u32;
}

static inline GK_VOID isp_demosaic_cbcravgthld_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicCbcrAvgThld)
{
    U_ISP_DEMOSAIC_CBCRAVGTHLD o_isp_demosaic_cbcravgthld;
    o_isp_demosaic_cbcravgthld.bits.isp_demosaic_cbcr_avg_thld = uIspDemosaicCbcrAvgThld;
    pstBeReg->ISP_DEMOSAIC_CBCRAVGTHLD.u32 = o_isp_demosaic_cbcravgthld.u32;
}

static inline GK_VOID isp_demosaic_cc_hf_ratio_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicCcHfMinRatio, GK_U32 uIspDemosaicCcHfMaxRatio)
{
    U_ISP_DEMOSAIC_CC_HF_RATIO o_isp_demosaic_cc_hf_ratio;
    o_isp_demosaic_cc_hf_ratio.bits.isp_demosaic_cc_hf_min_ratio = uIspDemosaicCcHfMinRatio;
    o_isp_demosaic_cc_hf_ratio.bits.isp_demosaic_cc_hf_max_ratio = uIspDemosaicCcHfMaxRatio;
    pstBeReg->ISP_DEMOSAIC_CC_HF_RATIO.u32 = o_isp_demosaic_cc_hf_ratio.u32;
}

static inline GK_VOID isp_demosaic_gvar_blend_thld_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicVarthrforblend)
{
    U_ISP_DEMOSAIC_GVAR_BLEND_THLD o_isp_demosaic_gvar_blend_thld;
    o_isp_demosaic_gvar_blend_thld.bits.isp_demosaic_varthrforblend = uIspDemosaicVarthrforblend;
    pstBeReg->ISP_DEMOSAIC_GVAR_BLEND_THLD.u32 = o_isp_demosaic_gvar_blend_thld.u32;
}

static inline GK_VOID isp_demosaic_satu_thr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicSatuThr)
{
    U_ISP_DEMOSAIC_SATU_THR o_isp_demosaic_satu_thr;
    o_isp_demosaic_satu_thr.bits.isp_demosaic_satu_thr = uIspDemosaicSatuThr;
    pstBeReg->ISP_DEMOSAIC_SATU_THR.u32 = o_isp_demosaic_satu_thr.u32;
}

static inline GK_VOID isp_demosaic_cbcr_ratio_limit_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicCbcrRatioLowLimit, GK_U32 uIspDemosaicCbcrRatioHighLimit)
{
    U_ISP_DEMOSAIC_CBCR_RATIO_LIMIT o_isp_demosaic_cbcr_ratio_limit;
    o_isp_demosaic_cbcr_ratio_limit.bits.isp_demosaic_cbcr_ratio_low_limit = uIspDemosaicCbcrRatioLowLimit;
    o_isp_demosaic_cbcr_ratio_limit.bits.isp_demosaic_cbcr_ratiogh_limit = uIspDemosaicCbcrRatioHighLimit;
    pstBeReg->ISP_DEMOSAIC_CBCR_RATIO_LIMIT.u32 = o_isp_demosaic_cbcr_ratio_limit.u32;
}

static inline GK_VOID isp_demosaic_fcr_gray_ratio_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicFcrGrayRatio)
{
    U_ISP_DEMOSAIC_FCR_GRAY_RATIO o_isp_demosaic_fcr_gray_ratio;
    o_isp_demosaic_fcr_gray_ratio.bits.isp_demosaic_fcr_gray_ratio = uIspDemosaicFcrGrayRatio;
    pstBeReg->ISP_DEMOSAIC_FCR_GRAY_RATIO.u32 = o_isp_demosaic_fcr_gray_ratio.u32;
}

static inline GK_VOID isp_demosaic_fcr_sel_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicFcrDetgSel, GK_U32 uIspDemosaicFcrCmaxSel)
{
    U_ISP_DEMOSAIC_FCR_SEL o_isp_demosaic_fcr_sel;
    o_isp_demosaic_fcr_sel.bits.isp_demosaic_fcr_detg_sel = uIspDemosaicFcrDetgSel;
    o_isp_demosaic_fcr_sel.bits.isp_demosaic_fcr_cmax_sel = uIspDemosaicFcrCmaxSel;
    pstBeReg->ISP_DEMOSAIC_FCR_SEL.u32 = o_isp_demosaic_fcr_sel.u32;
}

static inline GK_VOID isp_demosaic_cc_var_thresh_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicCcVarThresh)
{
    U_ISP_DEMOSAIC_CC_VAR_THRESH o_isp_demosaic_cc_var_thresh;
    o_isp_demosaic_cc_var_thresh.bits.isp_demosaic_cc_var_thresh = uIspDemosaicCcVarThresh;
    pstBeReg->ISP_DEMOSAIC_CC_VAR_THRESH.u32 = o_isp_demosaic_cc_var_thresh.u32;
}

static inline GK_VOID isp_demosaic_hf_intp_blur_th_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicHfIntpBlurTh1, GK_U32 uIspDemosaicHfIntpBlurTh2)
{
    U_ISP_DEMOSAIC_HF_INTP_BLUR_TH o_isp_demosaic_hf_intp_blur_th;
    o_isp_demosaic_hf_intp_blur_th.bits.isp_demosaic_hf_intp_blur_th1 = uIspDemosaicHfIntpBlurTh1;
    o_isp_demosaic_hf_intp_blur_th.bits.isp_demosaic_hf_intp_blur_th2 = uIspDemosaicHfIntpBlurTh2;
    pstBeReg->ISP_DEMOSAIC_HF_INTP_BLUR_TH.u32 = o_isp_demosaic_hf_intp_blur_th.u32;
}

static inline GK_VOID isp_demosaic_hf_intp_blur_ratio_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicHfIntpBlurRatio)
{
    U_ISP_DEMOSAIC_HF_INTP_BLUR_RATIO o_isp_demosaic_hf_intp_blur_ratio;
    o_isp_demosaic_hf_intp_blur_ratio.bits.isp_demosaic_hf_intp_blur_ratio = uIspDemosaicHfIntpBlurRatio;
    pstBeReg->ISP_DEMOSAIC_HF_INTP_BLUR_RATIO.u32 = o_isp_demosaic_hf_intp_blur_ratio.u32;
}

static inline GK_VOID isp_demosaic_cac_cbcr_thr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicCbThr, GK_U32 uIspDemosaicLumaThr)
{
    U_ISP_DEMOSAIC_CAC_CBCR_THR o_isp_demosaic_cac_cbcr_thr;
    o_isp_demosaic_cac_cbcr_thr.bits.isp_demosaic_cb_thr = uIspDemosaicCbThr;
    o_isp_demosaic_cac_cbcr_thr.bits.isp_demosaic_luma_thr = uIspDemosaicLumaThr;
    pstBeReg->ISP_DEMOSAIC_CAC_CBCR_THR.u32 = o_isp_demosaic_cac_cbcr_thr.u32;
}

static inline GK_VOID isp_demosaic_cac_lumagh_cnt_thr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicCacLumaHighCntThr)
{
    U_ISP_DEMOSAIC_CAC_LUMA_HIGH_CNT_THR o_isp_demosaic_cac_lumagh_cnt_thr;
    o_isp_demosaic_cac_lumagh_cnt_thr.bits.isp_demosaic_cac_lumagh_cnt_thr = uIspDemosaicCacLumaHighCntThr;
    pstBeReg->ISP_DEMOSAIC_CAC_LUMA_HIGH_CNT_THR.u32 = o_isp_demosaic_cac_lumagh_cnt_thr.u32;
}

static inline GK_VOID isp_demosaic_cac_cnt_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicCacCbCntRatio, GK_U32 uIspDemosaicCacCbCntLowThr, GK_U32 uIspDemosaicCacCbCntHighThr)
{
    U_ISP_DEMOSAIC_CAC_CNT_CFG o_isp_demosaic_cac_cnt_cfg;
    o_isp_demosaic_cac_cnt_cfg.bits.isp_demosaic_cac_cb_cnt_ratio = uIspDemosaicCacCbCntRatio;
    o_isp_demosaic_cac_cnt_cfg.bits.isp_demosaic_cac_cb_cnt_low_thr = uIspDemosaicCacCbCntLowThr;
    o_isp_demosaic_cac_cnt_cfg.bits.isp_demosaic_cac_cb_cntgh_thr = uIspDemosaicCacCbCntHighThr;
    pstBeReg->ISP_DEMOSAIC_CAC_CNT_CFG.u32 = o_isp_demosaic_cac_cnt_cfg.u32;
}

static inline GK_VOID isp_demosaic_defcolor_coef_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicDefcolorCr, GK_U32 uIspDemosaicDefcolorCb)
{
    U_ISP_DEMOSAIC_DEFCOLOR_COEF o_isp_demosaic_defcolor_coef;
    o_isp_demosaic_defcolor_coef.bits.isp_demosaic_defcolor_cr = uIspDemosaicDefcolorCr;
    o_isp_demosaic_defcolor_coef.bits.isp_demosaic_defcolor_cb = uIspDemosaicDefcolorCb;
    pstBeReg->ISP_DEMOSAIC_DEFCOLOR_COEF.u32 = o_isp_demosaic_defcolor_coef.u32;
}

static inline GK_VOID isp_demosaic_cac_bld_avg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaciCacBldAvgCur)
{
    U_ISP_DEMOSAIC_CAC_BLD_AVG o_isp_demosaic_cac_bld_avg;
    o_isp_demosaic_cac_bld_avg.bits.isp_demosaci_cac_bld_avg_cur = uIspDemosaciCacBldAvgCur;
    pstBeReg->ISP_DEMOSAIC_CAC_BLD_AVG.u32 = o_isp_demosaic_cac_bld_avg.u32;
}

static inline GK_VOID isp_demosaic_fcr_hf_thr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicFcrThresh1, GK_U32 uIspDemosaicFcrThresh2)
{
    U_ISP_DEMOSAIC_FCR_HF_THR o_isp_demosaic_fcr_hf_thr;
    o_isp_demosaic_fcr_hf_thr.bits.isp_demosaic_fcr_thresh1 = uIspDemosaicFcrThresh1;
    o_isp_demosaic_fcr_hf_thr.bits.isp_demosaic_fcr_thresh2 = uIspDemosaicFcrThresh2;
    pstBeReg->ISP_DEMOSAIC_FCR_HF_THR.u32 = o_isp_demosaic_fcr_hf_thr.u32;
}

static inline GK_VOID isp_demosaic_depurplut_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicDepurplutWaddr)
{
    U_ISP_DEMOSAIC_DEPURPLUT_WADDR o_isp_demosaic_depurplut_waddr;
    o_isp_demosaic_depurplut_waddr.bits.isp_demosaic_depurplut_waddr = uIspDemosaicDepurplutWaddr;
    pstBeReg->ISP_DEMOSAIC_DEPURPLUT_WADDR.u32 = o_isp_demosaic_depurplut_waddr.u32;
}

static inline GK_VOID isp_demosaic_depurplut_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicDepurplutWdata)
{
    U_ISP_DEMOSAIC_DEPURPLUT_WDATA o_isp_demosaic_depurplut_wdata;
    o_isp_demosaic_depurplut_wdata.bits.isp_demosaic_depurplut_wdata = uIspDemosaicDepurplutWdata;
    pstBeReg->ISP_DEMOSAIC_DEPURPLUT_WDATA.u32 = o_isp_demosaic_depurplut_wdata.u32;
}

static inline GK_VOID isp_demosaic_depurplut_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicDepurplutRaddr)
{
    U_ISP_DEMOSAIC_DEPURPLUT_RADDR o_isp_demosaic_depurplut_raddr;
    o_isp_demosaic_depurplut_raddr.bits.isp_demosaic_depurplut_raddr = uIspDemosaicDepurplutRaddr;
    pstBeReg->ISP_DEMOSAIC_DEPURPLUT_RADDR.u32 = o_isp_demosaic_depurplut_raddr.u32;
}

static inline GK_VOID isp_demosaic_desat_thr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicDesatThresh1, GK_U32 uIspDemosaicDesatThresh2)
{
    U_ISP_DEMOSAIC_DESAT_THR o_isp_demosaic_desat_thr;
    o_isp_demosaic_desat_thr.bits.isp_demosaic_desat_thresh1 = uIspDemosaicDesatThresh1;
    o_isp_demosaic_desat_thr.bits.isp_demosaic_desat_thresh2 = uIspDemosaicDesatThresh2;
    pstBeReg->ISP_DEMOSAIC_DESAT_THR.u32 = o_isp_demosaic_desat_thr.u32;
}

static inline GK_VOID isp_demosaic_desat_bldr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicDesatHig, GK_U32 uIspDemosaicDesatLow)
{
    U_ISP_DEMOSAIC_DESAT_BLDR o_isp_demosaic_desat_bldr;
    o_isp_demosaic_desat_bldr.bits.isp_demosaic_desatg = uIspDemosaicDesatHig;
    o_isp_demosaic_desat_bldr.bits.isp_demosaic_desat_low = uIspDemosaicDesatLow;
    pstBeReg->ISP_DEMOSAIC_DESAT_BLDR.u32 = o_isp_demosaic_desat_bldr.u32;
}

static inline GK_VOID isp_demosaic_desat_bldr_ratio_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicDesatRatio)
{
    U_ISP_DEMOSAIC_DESAT_BLDR_RATIO o_isp_demosaic_desat_bldr_ratio;
    o_isp_demosaic_desat_bldr_ratio.bits.isp_demosaic_desat_ratio = uIspDemosaicDesatRatio;
    pstBeReg->ISP_DEMOSAIC_DESAT_BLDR_RATIO.u32 = o_isp_demosaic_desat_bldr_ratio.u32;
}

static inline GK_VOID isp_demosaic_desat_protect_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicDesatProtectSl, GK_U32 uIspDemosaicDesatProtectTh)
{
    U_ISP_DEMOSAIC_DESAT_PROTECT o_isp_demosaic_desat_protect;
    o_isp_demosaic_desat_protect.bits.isp_demosaic_desat_protect_sl = uIspDemosaicDesatProtectSl;
    o_isp_demosaic_desat_protect.bits.isp_demosaic_desat_protect_th = uIspDemosaicDesatProtectTh;
    pstBeReg->ISP_DEMOSAIC_DESAT_PROTECT.u32 = o_isp_demosaic_desat_protect.u32;
}

static inline GK_VOID isp_demosaic_lut_update_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDemosaicDepurplutUpdate)
{
    U_ISP_DEMOSAIC_LUT_UPDATE o_isp_demosaic_lut_update;
    o_isp_demosaic_lut_update.bits.isp_demosaic_depurplut_update = uIspDemosaicDepurplutUpdate;
    pstBeReg->ISP_DEMOSAIC_LUT_UPDATE.u32 = o_isp_demosaic_lut_update.u32;
}

static inline GK_VOID isp_rgbir_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspRgbirBlcInEn, GK_U32 uIspRgbirBlcOutEn, GK_U32 uIspRgbirIrPatternIn, GK_U32 uIspRgbirPatternOut)
{
    U_ISP_RGBIR_CFG o_isp_rgbir_cfg;
    o_isp_rgbir_cfg.bits.isp_rgbir_blc_in_en = uIspRgbirBlcInEn;
    o_isp_rgbir_cfg.bits.isp_rgbir_blc_out_en = uIspRgbirBlcOutEn;
    o_isp_rgbir_cfg.bits.isp_rgbir_ir_pattern_in = uIspRgbirIrPatternIn;
    o_isp_rgbir_cfg.bits.isp_rgbir_pattern_out = uIspRgbirPatternOut;
    pstBeReg->ISP_RGBIR_CFG.u32 = o_isp_rgbir_cfg.u32;
}

static inline GK_VOID isp_rgbir_thre_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspRgbirTh, GK_U32 uIspRgbirTv)
{
    U_ISP_RGBIR_THRE o_isp_rgbir_thre;
    o_isp_rgbir_thre.bits.isp_rgbir_th = uIspRgbirTh;
    o_isp_rgbir_thre.bits.isp_rgbir_tv = uIspRgbirTv;
    pstBeReg->ISP_RGBIR_THRE.u32 = o_isp_rgbir_thre.u32;
}

static inline GK_VOID isp_rgbir_exp_thr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspRgbirExpThr1, GK_U32 uIspRgbirExpThr2)
{
    U_ISP_RGBIR_EXP_THR o_isp_rgbir_exp_thr;
    o_isp_rgbir_exp_thr.bits.isp_rgbir_exp_thr1 = uIspRgbirExpThr1;
    o_isp_rgbir_exp_thr.bits.isp_rgbir_exp_thr2 = uIspRgbirExpThr2;
    pstBeReg->ISP_RGBIR_EXP_THR.u32 = o_isp_rgbir_exp_thr.u32;
}

static inline GK_VOID isp_rgbir_reci_ctl_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspRgbirReciCtl1, GK_U32 uIspRgbirReciCtl2)
{
    U_ISP_RGBIR_RECI_CTL o_isp_rgbir_reci_ctl;
    o_isp_rgbir_reci_ctl.bits.isp_rgbir_reci_ctl1 = uIspRgbirReciCtl1;
    o_isp_rgbir_reci_ctl.bits.isp_rgbir_reci_ctl2 = uIspRgbirReciCtl2;
    pstBeReg->ISP_RGBIR_RECI_CTL.u32 = o_isp_rgbir_reci_ctl.u32;
}

static inline GK_VOID isp_rgbir_gain_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspRgbirGainR, GK_U32 uIspRgbirGainB)
{
    U_ISP_RGBIR_GAIN o_isp_rgbir_gain;
    o_isp_rgbir_gain.bits.isp_rgbir_gain_r = uIspRgbirGainR;
    o_isp_rgbir_gain.bits.isp_rgbir_gain_b = uIspRgbirGainB;
    pstBeReg->ISP_RGBIR_GAIN.u32 = o_isp_rgbir_gain.u32;
}

static inline GK_VOID isp_rgbir_cvt01_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspRgbirMatrix0, GK_U32 uIspRgbirMatrix1)
{
    U_ISP_RGBIR_CVT01 o_isp_rgbir_cvt01;
    o_isp_rgbir_cvt01.bits.isp_rgbir_matrix0 = uIspRgbirMatrix0;
    o_isp_rgbir_cvt01.bits.isp_rgbir_matrix1 = uIspRgbirMatrix1;
    pstBeReg->ISP_RGBIR_CVT01.u32 = o_isp_rgbir_cvt01.u32;
}

static inline GK_VOID isp_rgbir_cvt23_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspRgbirMatrix2, GK_U32 uIspRgbirMatrix3)
{
    U_ISP_RGBIR_CVT23 o_isp_rgbir_cvt23;
    o_isp_rgbir_cvt23.bits.isp_rgbir_matrix2 = uIspRgbirMatrix2;
    o_isp_rgbir_cvt23.bits.isp_rgbir_matrix3 = uIspRgbirMatrix3;
    pstBeReg->ISP_RGBIR_CVT23.u32 = o_isp_rgbir_cvt23.u32;
}

static inline GK_VOID isp_rgbir_cvt45_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspRgbirMatrix4, GK_U32 uIspRgbirMatrix5)
{
    U_ISP_RGBIR_CVT45 o_isp_rgbir_cvt45;
    o_isp_rgbir_cvt45.bits.isp_rgbir_matrix4 = uIspRgbirMatrix4;
    o_isp_rgbir_cvt45.bits.isp_rgbir_matrix5 = uIspRgbirMatrix5;
    pstBeReg->ISP_RGBIR_CVT45.u32 = o_isp_rgbir_cvt45.u32;
}

static inline GK_VOID isp_rgbir_cvt67_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspRgbirMatrix6, GK_U32 uIspRgbirMatrix7)
{
    U_ISP_RGBIR_CVT67 o_isp_rgbir_cvt67;
    o_isp_rgbir_cvt67.bits.isp_rgbir_matrix6 = uIspRgbirMatrix6;
    o_isp_rgbir_cvt67.bits.isp_rgbir_matrix7 = uIspRgbirMatrix7;
    pstBeReg->ISP_RGBIR_CVT67.u32 = o_isp_rgbir_cvt67.u32;
}

static inline GK_VOID isp_rgbir_cvt89_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspRgbirMatrix8, GK_U32 uIspRgbirMatrix9)
{
    U_ISP_RGBIR_CVT89 o_isp_rgbir_cvt89;
    o_isp_rgbir_cvt89.bits.isp_rgbir_matrix8 = uIspRgbirMatrix8;
    o_isp_rgbir_cvt89.bits.isp_rgbir_matrix9 = uIspRgbirMatrix9;
    pstBeReg->ISP_RGBIR_CVT89.u32 = o_isp_rgbir_cvt89.u32;
}

static inline GK_VOID isp_rgbir_cvt1011_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspRgbirMatrix10, GK_U32 uIspRgbirMatrix11)
{
    U_ISP_RGBIR_CVT1011 o_isp_rgbir_cvt1011;
    o_isp_rgbir_cvt1011.bits.isp_rgbir_matrix10 = uIspRgbirMatrix10;
    o_isp_rgbir_cvt1011.bits.isp_rgbir_matrix11 = uIspRgbirMatrix11;
    pstBeReg->ISP_RGBIR_CVT1011.u32 = o_isp_rgbir_cvt1011.u32;
}

static inline GK_VOID isp_rgbir_blc_offset_r_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspRgbirBlcOffsetR)
{
    U_ISP_RGBIR_BLC_OFFSET_R o_isp_rgbir_blc_offset_r;
    o_isp_rgbir_blc_offset_r.bits.isp_rgbir_blc_offset_r = uIspRgbirBlcOffsetR;
    pstBeReg->ISP_RGBIR_BLC_OFFSET_R.u32 = o_isp_rgbir_blc_offset_r.u32;
}

static inline GK_VOID isp_rgbir_blc_offset_g_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspRgbirBlcOffsetG)
{
    U_ISP_RGBIR_BLC_OFFSET_G o_isp_rgbir_blc_offset_g;
    o_isp_rgbir_blc_offset_g.bits.isp_rgbir_blc_offset_g = uIspRgbirBlcOffsetG;
    pstBeReg->ISP_RGBIR_BLC_OFFSET_G.u32 = o_isp_rgbir_blc_offset_g.u32;
}

static inline GK_VOID isp_rgbir_blc_offset_b_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspRgbirBlcOffsetB)
{
    U_ISP_RGBIR_BLC_OFFSET_B o_isp_rgbir_blc_offset_b;
    o_isp_rgbir_blc_offset_b.bits.isp_rgbir_blc_offset_b = uIspRgbirBlcOffsetB;
    pstBeReg->ISP_RGBIR_BLC_OFFSET_B.u32 = o_isp_rgbir_blc_offset_b.u32;
}

static inline GK_VOID isp_rgbir_blc_offset_ir_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspRgbirBlcOffsetIr)
{
    U_ISP_RGBIR_BLC_OFFSET_IR o_isp_rgbir_blc_offset_ir;
    o_isp_rgbir_blc_offset_ir.bits.isp_rgbir_blc_offset_ir = uIspRgbirBlcOffsetIr;
    pstBeReg->ISP_RGBIR_BLC_OFFSET_IR.u32 = o_isp_rgbir_blc_offset_ir.u32;
}

static inline GK_VOID isp_sharpen_ctrl_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenBenlumactrl, GK_U32 uIspSharpenVcdsFilterv, GK_U32 uIspSharpenBenshtvarSel, GK_U32 uIspSharpenBenshtctrlbyvar, GK_U32 uIspSharpenBenskinctrl, GK_U32 uIspSharpenWeakdetailadj, GK_U32 uIspSharpenBenchrctrl, GK_U32 uIspSharpenDetailthdSel, GK_U32 uIspSharpenBendetailctrl, GK_U32 uIspSharpenBen8dirSel)
{
    U_ISP_SHARPEN_CTRL o_isp_sharpen_ctrl;
    o_isp_sharpen_ctrl.bits.isp_sharpen_benlumactrl = uIspSharpenBenlumactrl;
    o_isp_sharpen_ctrl.bits.isp_sharpen_vcds_filterv = uIspSharpenVcdsFilterv;
    o_isp_sharpen_ctrl.bits.isp_sharpen_benshtvar_sel = uIspSharpenBenshtvarSel;
    o_isp_sharpen_ctrl.bits.isp_sharpen_benshtctrlbyvar = uIspSharpenBenshtctrlbyvar;
    o_isp_sharpen_ctrl.bits.isp_sharpen_benskinctrl = uIspSharpenBenskinctrl;
    o_isp_sharpen_ctrl.bits.isp_sharpen_weakdetailadj = uIspSharpenWeakdetailadj;
    o_isp_sharpen_ctrl.bits.isp_sharpen_benchrctrl = uIspSharpenBenchrctrl;
    o_isp_sharpen_ctrl.bits.isp_sharpen_detailthd_sel = uIspSharpenDetailthdSel;
    o_isp_sharpen_ctrl.bits.isp_sharpen_bendetailctrl = uIspSharpenBendetailctrl;
    o_isp_sharpen_ctrl.bits.isp_sharpen_ben8dir_sel = uIspSharpenBen8dirSel;
    pstBeReg->ISP_SHARPEN_CTRL.u32 = o_isp_sharpen_ctrl.u32;
}

static inline GK_VOID isp_sharpen_mhfthd_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenMhfthdsftd, GK_U32 uIspSharpenMhfthdseld, GK_U32 uIspSharpenMhfthdsftud, GK_U32 uIspSharpenMhfthdselud)
{
    U_ISP_SHARPEN_MHFTHD o_isp_sharpen_mhfthd;
    o_isp_sharpen_mhfthd.bits.isp_sharpen_mhfthdsftd = uIspSharpenMhfthdsftd;
    o_isp_sharpen_mhfthd.bits.isp_sharpen_mhfthdseld = uIspSharpenMhfthdseld;
    o_isp_sharpen_mhfthd.bits.isp_sharpen_mhfthdsftud = uIspSharpenMhfthdsftud;
    o_isp_sharpen_mhfthd.bits.isp_sharpen_mhfthdselud = uIspSharpenMhfthdselud;
    pstBeReg->ISP_SHARPEN_MHFTHD.u32 = o_isp_sharpen_mhfthd.u32;
}

static inline GK_VOID isp_sharpen_weakdetail_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenDirrlythrlow, GK_U32 uIspSharpenDirrlythrhih, GK_U32 uIspSharpenWeakdetailgain)
{
    U_ISP_SHARPEN_WEAKDETAIL o_isp_sharpen_weakdetail;
    o_isp_sharpen_weakdetail.bits.isp_sharpen_dirrlythrlow = uIspSharpenDirrlythrlow;
    o_isp_sharpen_weakdetail.bits.isp_sharpen_dirrlythrhih = uIspSharpenDirrlythrhih;
    o_isp_sharpen_weakdetail.bits.isp_sharpen_weakdetailgain = uIspSharpenWeakdetailgain;
    pstBeReg->ISP_SHARPEN_WEAKDETAIL.u32 = o_isp_sharpen_weakdetail.u32;
}

static inline GK_VOID isp_sharpen_dirvar_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenDirvarsft, GK_U32 uIspSharpenDirvarscale, GK_U32 uIspSharpenDirrly0, GK_U32 uIspSharpenDirrly1)
{
    U_ISP_SHARPEN_DIRVAR o_isp_sharpen_dirvar;
    o_isp_sharpen_dirvar.bits.isp_sharpen_dirvarsft = uIspSharpenDirvarsft;
    o_isp_sharpen_dirvar.bits.isp_sharpen_dirvarscale = uIspSharpenDirvarscale;
    o_isp_sharpen_dirvar.bits.isp_sharpen_dirrly0 = uIspSharpenDirrly0;
    o_isp_sharpen_dirvar.bits.isp_sharpen_dirrly1 = uIspSharpenDirrly1;
    pstBeReg->ISP_SHARPEN_DIRVAR.u32 = o_isp_sharpen_dirvar.u32;
}

static inline GK_VOID isp_sharpen_dirdiff_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenDirdiffsft, GK_U32 uIspSharpenDirrt0, GK_U32 uIspSharpenDirrt1)
{
    U_ISP_SHARPEN_DIRDIFF o_isp_sharpen_dirdiff;
    o_isp_sharpen_dirdiff.bits.isp_sharpen_dirdiffsft = uIspSharpenDirdiffsft;
    o_isp_sharpen_dirdiff.bits.isp_sharpen_dirrt0 = uIspSharpenDirrt0;
    o_isp_sharpen_dirdiff.bits.isp_sharpen_dirrt1 = uIspSharpenDirrt1;
    pstBeReg->ISP_SHARPEN_DIRDIFF.u32 = o_isp_sharpen_dirdiff.u32;
}

static inline GK_VOID isp_sharpen_lumawgt0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenLumawgt0, GK_U32 uIspSharpenLumawgt1, GK_U32 uIspSharpenLumawgt2, GK_U32 uIspSharpenLumawgt3, GK_U32 uIspSharpenLumawgt4)
{
    U_ISP_SHARPEN_LUMAWGT0 o_isp_sharpen_lumawgt0;
    o_isp_sharpen_lumawgt0.bits.isp_sharpen_lumawgt0 = uIspSharpenLumawgt0;
    o_isp_sharpen_lumawgt0.bits.isp_sharpen_lumawgt1 = uIspSharpenLumawgt1;
    o_isp_sharpen_lumawgt0.bits.isp_sharpen_lumawgt2 = uIspSharpenLumawgt2;
    o_isp_sharpen_lumawgt0.bits.isp_sharpen_lumawgt3 = uIspSharpenLumawgt3;
    o_isp_sharpen_lumawgt0.bits.isp_sharpen_lumawgt4 = uIspSharpenLumawgt4;
    pstBeReg->ISP_SHARPEN_LUMAWGT0.u32 = o_isp_sharpen_lumawgt0.u32;
}

static inline GK_VOID isp_sharpen_lumawgt1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenLumawgt5, GK_U32 uIspSharpenLumawgt6, GK_U32 uIspSharpenLumawgt7, GK_U32 uIspSharpenLumawgt8, GK_U32 uIspSharpenLumawgt9)
{
    U_ISP_SHARPEN_LUMAWGT1 o_isp_sharpen_lumawgt1;
    o_isp_sharpen_lumawgt1.bits.isp_sharpen_lumawgt5 = uIspSharpenLumawgt5;
    o_isp_sharpen_lumawgt1.bits.isp_sharpen_lumawgt6 = uIspSharpenLumawgt6;
    o_isp_sharpen_lumawgt1.bits.isp_sharpen_lumawgt7 = uIspSharpenLumawgt7;
    o_isp_sharpen_lumawgt1.bits.isp_sharpen_lumawgt8 = uIspSharpenLumawgt8;
    o_isp_sharpen_lumawgt1.bits.isp_sharpen_lumawgt9 = uIspSharpenLumawgt9;
    pstBeReg->ISP_SHARPEN_LUMAWGT1.u32 = o_isp_sharpen_lumawgt1.u32;
}

static inline GK_VOID isp_sharpen_lumawgt2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenLumawgt10, GK_U32 uIspSharpenLumawgt11, GK_U32 uIspSharpenLumawgt12, GK_U32 uIspSharpenLumawgt13, GK_U32 uIspSharpenLumawgt14)
{
    U_ISP_SHARPEN_LUMAWGT2 o_isp_sharpen_lumawgt2;
    o_isp_sharpen_lumawgt2.bits.isp_sharpen_lumawgt10 = uIspSharpenLumawgt10;
    o_isp_sharpen_lumawgt2.bits.isp_sharpen_lumawgt11 = uIspSharpenLumawgt11;
    o_isp_sharpen_lumawgt2.bits.isp_sharpen_lumawgt12 = uIspSharpenLumawgt12;
    o_isp_sharpen_lumawgt2.bits.isp_sharpen_lumawgt13 = uIspSharpenLumawgt13;
    o_isp_sharpen_lumawgt2.bits.isp_sharpen_lumawgt14 = uIspSharpenLumawgt14;
    pstBeReg->ISP_SHARPEN_LUMAWGT2.u32 = o_isp_sharpen_lumawgt2.u32;
}

static inline GK_VOID isp_sharpen_lumawgt3_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenLumawgt15, GK_U32 uIspSharpenLumawgt16, GK_U32 uIspSharpenLumawgt17, GK_U32 uIspSharpenLumawgt18, GK_U32 uIspSharpenLumawgt19)
{
    U_ISP_SHARPEN_LUMAWGT3 o_isp_sharpen_lumawgt3;
    o_isp_sharpen_lumawgt3.bits.isp_sharpen_lumawgt15 = uIspSharpenLumawgt15;
    o_isp_sharpen_lumawgt3.bits.isp_sharpen_lumawgt16 = uIspSharpenLumawgt16;
    o_isp_sharpen_lumawgt3.bits.isp_sharpen_lumawgt17 = uIspSharpenLumawgt17;
    o_isp_sharpen_lumawgt3.bits.isp_sharpen_lumawgt18 = uIspSharpenLumawgt18;
    o_isp_sharpen_lumawgt3.bits.isp_sharpen_lumawgt19 = uIspSharpenLumawgt19;
    pstBeReg->ISP_SHARPEN_LUMAWGT3.u32 = o_isp_sharpen_lumawgt3.u32;
}

static inline GK_VOID isp_sharpen_lumawgt4_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenLumawgt20, GK_U32 uIspSharpenLumawgt21, GK_U32 uIspSharpenLumawgt22, GK_U32 uIspSharpenLumawgt23, GK_U32 uIspSharpenLumawgt24)
{
    U_ISP_SHARPEN_LUMAWGT4 o_isp_sharpen_lumawgt4;
    o_isp_sharpen_lumawgt4.bits.isp_sharpen_lumawgt20 = uIspSharpenLumawgt20;
    o_isp_sharpen_lumawgt4.bits.isp_sharpen_lumawgt21 = uIspSharpenLumawgt21;
    o_isp_sharpen_lumawgt4.bits.isp_sharpen_lumawgt22 = uIspSharpenLumawgt22;
    o_isp_sharpen_lumawgt4.bits.isp_sharpen_lumawgt23 = uIspSharpenLumawgt23;
    o_isp_sharpen_lumawgt4.bits.isp_sharpen_lumawgt24 = uIspSharpenLumawgt24;
    pstBeReg->ISP_SHARPEN_LUMAWGT4.u32 = o_isp_sharpen_lumawgt4.u32;
}

static inline GK_VOID isp_sharpen_lumawgt5_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenLumawgt25, GK_U32 uIspSharpenLumawgt26, GK_U32 uIspSharpenLumawgt27, GK_U32 uIspSharpenLumawgt28, GK_U32 uIspSharpenLumawgt29)
{
    U_ISP_SHARPEN_LUMAWGT5 o_isp_sharpen_lumawgt5;
    o_isp_sharpen_lumawgt5.bits.isp_sharpen_lumawgt25 = uIspSharpenLumawgt25;
    o_isp_sharpen_lumawgt5.bits.isp_sharpen_lumawgt26 = uIspSharpenLumawgt26;
    o_isp_sharpen_lumawgt5.bits.isp_sharpen_lumawgt27 = uIspSharpenLumawgt27;
    o_isp_sharpen_lumawgt5.bits.isp_sharpen_lumawgt28 = uIspSharpenLumawgt28;
    o_isp_sharpen_lumawgt5.bits.isp_sharpen_lumawgt29 = uIspSharpenLumawgt29;
    pstBeReg->ISP_SHARPEN_LUMAWGT5.u32 = o_isp_sharpen_lumawgt5.u32;
}

static inline GK_VOID isp_sharpen_lumawgt6_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenLumawgt30, GK_U32 uIspSharpenLumawgt31)
{
    U_ISP_SHARPEN_LUMAWGT6 o_isp_sharpen_lumawgt6;
    o_isp_sharpen_lumawgt6.bits.isp_sharpen_lumawgt30 = uIspSharpenLumawgt30;
    o_isp_sharpen_lumawgt6.bits.isp_sharpen_lumawgt31 = uIspSharpenLumawgt31;
    pstBeReg->ISP_SHARPEN_LUMAWGT6.u32 = o_isp_sharpen_lumawgt6.u32;
}

static inline GK_VOID isp_sharpen_shoot_amt_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenSelpixwgt, GK_U32 uIspSharpenOshtamt, GK_U32 uIspSharpenUshtamt)
{
    U_ISP_SHARPEN_SHOOT_AMT o_isp_sharpen_shoot_amt;
    o_isp_sharpen_shoot_amt.bits.isp_sharpen_selpixwgt = uIspSharpenSelpixwgt;
    o_isp_sharpen_shoot_amt.bits.isp_sharpen_oshtamt = uIspSharpenOshtamt;
    o_isp_sharpen_shoot_amt.bits.isp_sharpen_ushtamt = uIspSharpenUshtamt;
    pstBeReg->ISP_SHARPEN_SHOOT_AMT.u32 = o_isp_sharpen_shoot_amt.u32;
}

static inline GK_VOID isp_sharpen_shoot_maxchg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenOmaxchg, GK_U32 uIspSharpenUmaxchg)
{
    U_ISP_SHARPEN_SHOOT_MAXCHG o_isp_sharpen_shoot_maxchg;
    o_isp_sharpen_shoot_maxchg.bits.isp_sharpen_omaxchg = uIspSharpenOmaxchg;
    o_isp_sharpen_shoot_maxchg.bits.isp_sharpen_umaxchg = uIspSharpenUmaxchg;
    pstBeReg->ISP_SHARPEN_SHOOT_MAXCHG.u32 = o_isp_sharpen_shoot_maxchg.u32;
}

static inline GK_VOID isp_sharpen_shtvar_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenShtvarsft, GK_U32 uIspSharpenShtvar5x5Sft, GK_U32 uIspSharpenShtbldrt)
{
    U_ISP_SHARPEN_SHTVAR o_isp_sharpen_shtvar;
    o_isp_sharpen_shtvar.bits.isp_sharpen_shtvarsft = uIspSharpenShtvarsft;
    o_isp_sharpen_shtvar.bits.isp_sharpen_shtvar5x5_sft = uIspSharpenShtvar5x5Sft;
    o_isp_sharpen_shtvar.bits.isp_sharpen_shtbldrt = uIspSharpenShtbldrt;
    pstBeReg->ISP_SHARPEN_SHTVAR.u32 = o_isp_sharpen_shtvar.u32;
}

static inline GK_VOID isp_sharpen_oshtvar_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenShtvarthd0, GK_U32 uIspSharpenShtvarthd1, GK_U32 uIspSharpenShtvarwgt0, GK_U32 uIspSharpenShtvarwgt1)
{
    U_ISP_SHARPEN_OSHTVAR o_isp_sharpen_oshtvar;
    o_isp_sharpen_oshtvar.bits.isp_sharpen_shtvarthd0 = uIspSharpenShtvarthd0;
    o_isp_sharpen_oshtvar.bits.isp_sharpen_shtvarthd1 = uIspSharpenShtvarthd1;
    o_isp_sharpen_oshtvar.bits.isp_sharpen_shtvarwgt0 = uIspSharpenShtvarwgt0;
    o_isp_sharpen_oshtvar.bits.isp_sharpen_shtvarwgt1 = uIspSharpenShtvarwgt1;
    pstBeReg->ISP_SHARPEN_OSHTVAR.u32 = o_isp_sharpen_oshtvar.u32;
}

static inline GK_VOID isp_sharpen_shtvar_mul_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenShtvarmul, GK_U32 uIspSharpenShtvardiffmul)
{
    U_ISP_SHARPEN_SHTVAR_MUL o_isp_sharpen_shtvar_mul;
    o_isp_sharpen_shtvar_mul.bits.isp_sharpen_shtvarmul = uIspSharpenShtvarmul;
    o_isp_sharpen_shtvar_mul.bits.isp_sharpen_shtvardiffmul = uIspSharpenShtvardiffmul;
    pstBeReg->ISP_SHARPEN_SHTVAR_MUL.u32 = o_isp_sharpen_shtvar_mul.u32;
}

static inline GK_VOID isp_sharpen_oshtvardiff_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenShtvardiffthd0, GK_U32 uIspSharpenShtvardiffthd1, GK_U32 uIspSharpenShtvardiffwgt0, GK_U32 uIspSharpenShtvardiffwgt1)
{
    U_ISP_SHARPEN_OSHTVARDIFF o_isp_sharpen_oshtvardiff;
    o_isp_sharpen_oshtvardiff.bits.isp_sharpen_shtvardiffthd0 = uIspSharpenShtvardiffthd0;
    o_isp_sharpen_oshtvardiff.bits.isp_sharpen_shtvardiffthd1 = uIspSharpenShtvardiffthd1;
    o_isp_sharpen_oshtvardiff.bits.isp_sharpen_shtvardiffwgt0 = uIspSharpenShtvardiffwgt0;
    o_isp_sharpen_oshtvardiff.bits.isp_sharpen_shtvardiffwgt1 = uIspSharpenShtvardiffwgt1;
    pstBeReg->ISP_SHARPEN_OSHTVARDIFF.u32 = o_isp_sharpen_oshtvardiff.u32;
}

static inline GK_VOID isp_sharpen_lmtmf0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenLmtmf0, GK_U32 uIspSharpenLmtmf1, GK_U32 uIspSharpenLmtmf2, GK_U32 uIspSharpenLmtmf3)
{
    U_ISP_SHARPEN_LMTMF0 o_isp_sharpen_lmtmf0;
    o_isp_sharpen_lmtmf0.bits.isp_sharpen_lmtmf0 = uIspSharpenLmtmf0;
    o_isp_sharpen_lmtmf0.bits.isp_sharpen_lmtmf1 = uIspSharpenLmtmf1;
    o_isp_sharpen_lmtmf0.bits.isp_sharpen_lmtmf2 = uIspSharpenLmtmf2;
    o_isp_sharpen_lmtmf0.bits.isp_sharpen_lmtmf3 = uIspSharpenLmtmf3;
    pstBeReg->ISP_SHARPEN_LMTMF0.u32 = o_isp_sharpen_lmtmf0.u32;
}

static inline GK_VOID isp_sharpen_lmtmf1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenLmtmf4, GK_U32 uIspSharpenLmtmf5, GK_U32 uIspSharpenLmtmf6, GK_U32 uIspSharpenLmtmf7)
{
    U_ISP_SHARPEN_LMTMF1 o_isp_sharpen_lmtmf1;
    o_isp_sharpen_lmtmf1.bits.isp_sharpen_lmtmf4 = uIspSharpenLmtmf4;
    o_isp_sharpen_lmtmf1.bits.isp_sharpen_lmtmf5 = uIspSharpenLmtmf5;
    o_isp_sharpen_lmtmf1.bits.isp_sharpen_lmtmf6 = uIspSharpenLmtmf6;
    o_isp_sharpen_lmtmf1.bits.isp_sharpen_lmtmf7 = uIspSharpenLmtmf7;
    pstBeReg->ISP_SHARPEN_LMTMF1.u32 = o_isp_sharpen_lmtmf1.u32;
}

static inline GK_VOID isp_sharpen_lmthf0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenLmthf0, GK_U32 uIspSharpenLmthf1, GK_U32 uIspSharpenLmthf2, GK_U32 uIspSharpenLmthf3)
{
    U_ISP_SHARPEN_LMTHF0 o_isp_sharpen_lmthf0;
    o_isp_sharpen_lmthf0.bits.isp_sharpen_lmthf0 = uIspSharpenLmthf0;
    o_isp_sharpen_lmthf0.bits.isp_sharpen_lmthf1 = uIspSharpenLmthf1;
    o_isp_sharpen_lmthf0.bits.isp_sharpen_lmthf2 = uIspSharpenLmthf2;
    o_isp_sharpen_lmthf0.bits.isp_sharpen_lmthf3 = uIspSharpenLmthf3;
    pstBeReg->ISP_SHARPEN_LMTHF0.u32 = o_isp_sharpen_lmthf0.u32;
}

static inline GK_VOID isp_sharpen_lmthf1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenLmthf4, GK_U32 uIspSharpenLmthf5, GK_U32 uIspSharpenLmthf6, GK_U32 uIspSharpenLmthf7)
{
    U_ISP_SHARPEN_LMTHF1 o_isp_sharpen_lmthf1;
    o_isp_sharpen_lmthf1.bits.isp_sharpen_lmthf4 = uIspSharpenLmthf4;
    o_isp_sharpen_lmthf1.bits.isp_sharpen_lmthf5 = uIspSharpenLmthf5;
    o_isp_sharpen_lmthf1.bits.isp_sharpen_lmthf6 = uIspSharpenLmthf6;
    o_isp_sharpen_lmthf1.bits.isp_sharpen_lmthf7 = uIspSharpenLmthf7;
    pstBeReg->ISP_SHARPEN_LMTHF1.u32 = o_isp_sharpen_lmthf1.u32;
}

static inline GK_VOID isp_sharpen_mhfgaind_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenMhfgaindWaddr)
{
    U_ISP_SHARPEN_MHFGAIND_WADDR o_isp_sharpen_mhfgaind_waddr;
    o_isp_sharpen_mhfgaind_waddr.bits.isp_sharpen_mhfgaind_waddr = uIspSharpenMhfgaindWaddr;
    pstBeReg->ISP_SHARPEN_MHFGAIND_WADDR.u32 = o_isp_sharpen_mhfgaind_waddr.u32;
}

static inline GK_VOID isp_sharpen_mhfgaind_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenMhfgaindWdata)
{
    U_ISP_SHARPEN_MHFGAIND_WDATA o_isp_sharpen_mhfgaind_wdata;
    o_isp_sharpen_mhfgaind_wdata.bits.isp_sharpen_mhfgaind_wdata = uIspSharpenMhfgaindWdata;
    pstBeReg->ISP_SHARPEN_MHFGAIND_WDATA.u32 = o_isp_sharpen_mhfgaind_wdata.u32;
}

static inline GK_VOID isp_sharpen_mhfgaind_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenMhfgaindRaddr)
{
    U_ISP_SHARPEN_MHFGAIND_RADDR o_isp_sharpen_mhfgaind_raddr;
    o_isp_sharpen_mhfgaind_raddr.bits.isp_sharpen_mhfgaind_raddr = uIspSharpenMhfgaindRaddr;
    pstBeReg->ISP_SHARPEN_MHFGAIND_RADDR.u32 = o_isp_sharpen_mhfgaind_raddr.u32;
}

static inline GK_VOID isp_sharpen_mhfgainud_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenMhfgainudWaddr)
{
    U_ISP_SHARPEN_MHFGAINUD_WADDR o_isp_sharpen_mhfgainud_waddr;
    o_isp_sharpen_mhfgainud_waddr.bits.isp_sharpen_mhfgainud_waddr = uIspSharpenMhfgainudWaddr;
    pstBeReg->ISP_SHARPEN_MHFGAINUD_WADDR.u32 = o_isp_sharpen_mhfgainud_waddr.u32;
}

static inline GK_VOID isp_sharpen_mhfgainud_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenMhfgainudWdata)
{
    U_ISP_SHARPEN_MHFGAINUD_WDATA o_isp_sharpen_mhfgainud_wdata;
    o_isp_sharpen_mhfgainud_wdata.bits.isp_sharpen_mhfgainud_wdata = uIspSharpenMhfgainudWdata;
    pstBeReg->ISP_SHARPEN_MHFGAINUD_WDATA.u32 = o_isp_sharpen_mhfgainud_wdata.u32;
}

static inline GK_VOID isp_sharpen_mhfgainud_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenMhfgainudRaddr)
{
    U_ISP_SHARPEN_MHFGAINUD_RADDR o_isp_sharpen_mhfgainud_raddr;
    o_isp_sharpen_mhfgainud_raddr.bits.isp_sharpen_mhfgainud_raddr = uIspSharpenMhfgainudRaddr;
    pstBeReg->ISP_SHARPEN_MHFGAINUD_RADDR.u32 = o_isp_sharpen_mhfgainud_raddr.u32;
}

static inline GK_VOID isp_sharpen_skin_u_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenSkinminu, GK_U32 uIspSharpenSkinmaxu)
{
    U_ISP_SHARPEN_SKIN_U o_isp_sharpen_skin_u;
    o_isp_sharpen_skin_u.bits.isp_sharpen_skinminu = uIspSharpenSkinminu;
    o_isp_sharpen_skin_u.bits.isp_sharpen_skinmaxu = uIspSharpenSkinmaxu;
    pstBeReg->ISP_SHARPEN_SKIN_U.u32 = o_isp_sharpen_skin_u.u32;
}

static inline GK_VOID isp_sharpen_skin_v_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenSkinminv, GK_U32 uIspSharpenSkinmaxv)
{
    U_ISP_SHARPEN_SKIN_V o_isp_sharpen_skin_v;
    o_isp_sharpen_skin_v.bits.isp_sharpen_skinminv = uIspSharpenSkinminv;
    o_isp_sharpen_skin_v.bits.isp_sharpen_skinmaxv = uIspSharpenSkinmaxv;
    pstBeReg->ISP_SHARPEN_SKIN_V.u32 = o_isp_sharpen_skin_v.u32;
}

static inline GK_VOID isp_sharpen_skin_cnt_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenSkincntthd0, GK_U32 uIspSharpenSkincntthd1, GK_U32 uIspSharpenSkincntmul)
{
    U_ISP_SHARPEN_SKIN_CNT o_isp_sharpen_skin_cnt;
    o_isp_sharpen_skin_cnt.bits.isp_sharpen_skincntthd0 = uIspSharpenSkincntthd0;
    o_isp_sharpen_skin_cnt.bits.isp_sharpen_skincntthd1 = uIspSharpenSkincntthd1;
    o_isp_sharpen_skin_cnt.bits.isp_sharpen_skincntmul = uIspSharpenSkincntmul;
    pstBeReg->ISP_SHARPEN_SKIN_CNT.u32 = o_isp_sharpen_skin_cnt.u32;
}

static inline GK_VOID isp_sharpen_skin_edge_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenSkinedgesft, GK_U32 uIspSharpenSkinedgemul)
{
    U_ISP_SHARPEN_SKIN_EDGE o_isp_sharpen_skin_edge;
    o_isp_sharpen_skin_edge.bits.isp_sharpen_skinedgesft = uIspSharpenSkinedgesft;
    o_isp_sharpen_skin_edge.bits.isp_sharpen_skinedgemul = uIspSharpenSkinedgemul;
    pstBeReg->ISP_SHARPEN_SKIN_EDGE.u32 = o_isp_sharpen_skin_edge.u32;
}

static inline GK_VOID isp_sharpen_skin_edgethd_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenSkinedgethd0, GK_U32 uIspSharpenSkinedgethd1, GK_U32 uIspSharpenSkinedgewgt0, GK_U32 uIspSharpenSkinedgewgt1)
{
    U_ISP_SHARPEN_SKIN_EDGETHD o_isp_sharpen_skin_edgethd;
    o_isp_sharpen_skin_edgethd.bits.isp_sharpen_skinedgethd0 = uIspSharpenSkinedgethd0;
    o_isp_sharpen_skin_edgethd.bits.isp_sharpen_skinedgethd1 = uIspSharpenSkinedgethd1;
    o_isp_sharpen_skin_edgethd.bits.isp_sharpen_skinedgewgt0 = uIspSharpenSkinedgewgt0;
    o_isp_sharpen_skin_edgethd.bits.isp_sharpen_skinedgewgt1 = uIspSharpenSkinedgewgt1;
    pstBeReg->ISP_SHARPEN_SKIN_EDGETHD.u32 = o_isp_sharpen_skin_edgethd.u32;
}

static inline GK_VOID isp_sharpen_chrr_var_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenChrrsft0, GK_U32 uIspSharpenChrrsft1, GK_U32 uIspSharpenChrrsft2, GK_U32 uIspSharpenChrrsft3, GK_U32 uIspSharpenChrrvarshift)
{
    U_ISP_SHARPEN_CHRR_VAR o_isp_sharpen_chrr_var;
    o_isp_sharpen_chrr_var.bits.isp_sharpen_chrrsft0 = uIspSharpenChrrsft0;
    o_isp_sharpen_chrr_var.bits.isp_sharpen_chrrsft1 = uIspSharpenChrrsft1;
    o_isp_sharpen_chrr_var.bits.isp_sharpen_chrrsft2 = uIspSharpenChrrsft2;
    o_isp_sharpen_chrr_var.bits.isp_sharpen_chrrsft3 = uIspSharpenChrrsft3;
    o_isp_sharpen_chrr_var.bits.isp_sharpen_chrrvarshift = uIspSharpenChrrvarshift;
    pstBeReg->ISP_SHARPEN_CHRR_VAR.u32 = o_isp_sharpen_chrr_var.u32;
}

static inline GK_VOID isp_sharpen_chrr_thd_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenChrrori0, GK_U32 uIspSharpenChrrori1, GK_U32 uIspSharpenChrrthd0, GK_U32 uIspSharpenChrrthd1)
{
    U_ISP_SHARPEN_CHRR_THD o_isp_sharpen_chrr_thd;
    o_isp_sharpen_chrr_thd.bits.isp_sharpen_chrrori0 = uIspSharpenChrrori0;
    o_isp_sharpen_chrr_thd.bits.isp_sharpen_chrrori1 = uIspSharpenChrrori1;
    o_isp_sharpen_chrr_thd.bits.isp_sharpen_chrrthd0 = uIspSharpenChrrthd0;
    o_isp_sharpen_chrr_thd.bits.isp_sharpen_chrrthd1 = uIspSharpenChrrthd1;
    pstBeReg->ISP_SHARPEN_CHRR_THD.u32 = o_isp_sharpen_chrr_thd.u32;
}

static inline GK_VOID isp_sharpen_chrr_gain_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenChrrgain, GK_U32 uIspSharpenRmfscale, GK_U32 uIspSharpenChrrmul)
{
    U_ISP_SHARPEN_CHRR_GAIN o_isp_sharpen_chrr_gain;
    o_isp_sharpen_chrr_gain.bits.isp_sharpen_chrrgain = uIspSharpenChrrgain;
    o_isp_sharpen_chrr_gain.bits.isp_sharpen_rmfscale = uIspSharpenRmfscale;
    o_isp_sharpen_chrr_gain.bits.isp_sharpen_chrrmul = uIspSharpenChrrmul;
    pstBeReg->ISP_SHARPEN_CHRR_GAIN.u32 = o_isp_sharpen_chrr_gain.u32;
}

static inline GK_VOID isp_sharpen_chrg_mul_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenChrgmul, GK_U32 uIspSharpenChrgmfmul)
{
    U_ISP_SHARPEN_CHRG_MUL o_isp_sharpen_chrg_mul;
    o_isp_sharpen_chrg_mul.bits.isp_sharpen_chrgmul = uIspSharpenChrgmul;
    o_isp_sharpen_chrg_mul.bits.isp_sharpen_chrgmfmul = uIspSharpenChrgmfmul;
    pstBeReg->ISP_SHARPEN_CHRG_MUL.u32 = o_isp_sharpen_chrg_mul.u32;
}

static inline GK_VOID isp_sharpen_chrg_sft_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenChrgsft0, GK_U32 uIspSharpenChrgsft1, GK_U32 uIspSharpenChrgsft2, GK_U32 uIspSharpenChrgsft3)
{
    U_ISP_SHARPEN_CHRG_SFT o_isp_sharpen_chrg_sft;
    o_isp_sharpen_chrg_sft.bits.isp_sharpen_chrgsft0 = uIspSharpenChrgsft0;
    o_isp_sharpen_chrg_sft.bits.isp_sharpen_chrgsft1 = uIspSharpenChrgsft1;
    o_isp_sharpen_chrg_sft.bits.isp_sharpen_chrgsft2 = uIspSharpenChrgsft2;
    o_isp_sharpen_chrg_sft.bits.isp_sharpen_chrgsft3 = uIspSharpenChrgsft3;
    pstBeReg->ISP_SHARPEN_CHRG_SFT.u32 = o_isp_sharpen_chrg_sft.u32;
}

static inline GK_VOID isp_sharpen_chrg_thd_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenChrgori0, GK_U32 uIspSharpenChrgori1, GK_U32 uIspSharpenChrgthd0, GK_U32 uIspSharpenChrgthd1)
{
    U_ISP_SHARPEN_CHRG_THD o_isp_sharpen_chrg_thd;
    o_isp_sharpen_chrg_thd.bits.isp_sharpen_chrgori0 = uIspSharpenChrgori0;
    o_isp_sharpen_chrg_thd.bits.isp_sharpen_chrgori1 = uIspSharpenChrgori1;
    o_isp_sharpen_chrg_thd.bits.isp_sharpen_chrgthd0 = uIspSharpenChrgthd0;
    o_isp_sharpen_chrg_thd.bits.isp_sharpen_chrgthd1 = uIspSharpenChrgthd1;
    pstBeReg->ISP_SHARPEN_CHRG_THD.u32 = o_isp_sharpen_chrg_thd.u32;
}

static inline GK_VOID isp_sharpen_chrg_gain_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenChrggain, GK_U32 uIspSharpenChrgmfgain)
{
    U_ISP_SHARPEN_CHRG_GAIN o_isp_sharpen_chrg_gain;
    o_isp_sharpen_chrg_gain.bits.isp_sharpen_chrggain = uIspSharpenChrggain;
    o_isp_sharpen_chrg_gain.bits.isp_sharpen_chrgmfgain = uIspSharpenChrgmfgain;
    pstBeReg->ISP_SHARPEN_CHRG_GAIN.u32 = o_isp_sharpen_chrg_gain.u32;
}

static inline GK_VOID isp_sharpen_chrb_var_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenChrbsft0, GK_U32 uIspSharpenChrbsft1, GK_U32 uIspSharpenChrbsft2, GK_U32 uIspSharpenChrbsft3, GK_U32 uIspSharpenChrbvarshift)
{
    U_ISP_SHARPEN_CHRB_VAR o_isp_sharpen_chrb_var;
    o_isp_sharpen_chrb_var.bits.isp_sharpen_chrbsft0 = uIspSharpenChrbsft0;
    o_isp_sharpen_chrb_var.bits.isp_sharpen_chrbsft1 = uIspSharpenChrbsft1;
    o_isp_sharpen_chrb_var.bits.isp_sharpen_chrbsft2 = uIspSharpenChrbsft2;
    o_isp_sharpen_chrb_var.bits.isp_sharpen_chrbsft3 = uIspSharpenChrbsft3;
    o_isp_sharpen_chrb_var.bits.isp_sharpen_chrbvarshift = uIspSharpenChrbvarshift;
    pstBeReg->ISP_SHARPEN_CHRB_VAR.u32 = o_isp_sharpen_chrb_var.u32;
}

static inline GK_VOID isp_sharpen_chrb_thd_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenChrbori0, GK_U32 uIspSharpenChrbori1, GK_U32 uIspSharpenChrbthd0, GK_U32 uIspSharpenChrbthd1)
{
    U_ISP_SHARPEN_CHRB_THD o_isp_sharpen_chrb_thd;
    o_isp_sharpen_chrb_thd.bits.isp_sharpen_chrbori0 = uIspSharpenChrbori0;
    o_isp_sharpen_chrb_thd.bits.isp_sharpen_chrbori1 = uIspSharpenChrbori1;
    o_isp_sharpen_chrb_thd.bits.isp_sharpen_chrbthd0 = uIspSharpenChrbthd0;
    o_isp_sharpen_chrb_thd.bits.isp_sharpen_chrbthd1 = uIspSharpenChrbthd1;
    pstBeReg->ISP_SHARPEN_CHRB_THD.u32 = o_isp_sharpen_chrb_thd.u32;
}

static inline GK_VOID isp_sharpen_chrb_gain_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenChrbgain, GK_U32 uIspSharpenBmfscale, GK_U32 uIspSharpenChrbmul)
{
    U_ISP_SHARPEN_CHRB_GAIN o_isp_sharpen_chrb_gain;
    o_isp_sharpen_chrb_gain.bits.isp_sharpen_chrbgain = uIspSharpenChrbgain;
    o_isp_sharpen_chrb_gain.bits.isp_sharpen_bmfscale = uIspSharpenBmfscale;
    o_isp_sharpen_chrb_gain.bits.isp_sharpen_chrbmul = uIspSharpenChrbmul;
    pstBeReg->ISP_SHARPEN_CHRB_GAIN.u32 = o_isp_sharpen_chrb_gain.u32;
}

static inline GK_VOID isp_sharpen_gain_sft_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenMfgainSft, GK_U32 uIspSharpenLfgainwgt, GK_U32 uIspSharpenHfgainSft)
{
    U_ISP_SHARPEN_GAIN_SFT o_isp_sharpen_gain_sft;
    o_isp_sharpen_gain_sft.bits.isp_sharpen_mfgain_sft = uIspSharpenMfgainSft;
    o_isp_sharpen_gain_sft.bits.isp_sharpen_lfgainwgt = uIspSharpenLfgainwgt;
    o_isp_sharpen_gain_sft.bits.isp_sharpen_hfgain_sft = uIspSharpenHfgainSft;
    pstBeReg->ISP_SHARPEN_GAIN_SFT.u32 = o_isp_sharpen_gain_sft.u32;
}

static inline GK_VOID isp_sharpen_shoot_maxgain_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenOmaxgain, GK_U32 uIspSharpenUmaxgain)
{
    U_ISP_SHARPEN_SHOOT_MAXGAIN o_isp_sharpen_shoot_maxgain;
    o_isp_sharpen_shoot_maxgain.bits.isp_sharpen_omaxgain = uIspSharpenOmaxgain;
    o_isp_sharpen_shoot_maxgain.bits.isp_sharpen_umaxgain = uIspSharpenUmaxgain;
    pstBeReg->ISP_SHARPEN_SHOOT_MAXGAIN.u32 = o_isp_sharpen_shoot_maxgain.u32;
}

static inline GK_VOID isp_sharpen_detail_mul_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenDetlOshtmul, GK_U32 uIspSharpenDetlUshtmul)
{
    U_ISP_SHARPEN_DETAIL_MUL o_isp_sharpen_detail_mul;
    o_isp_sharpen_detail_mul.bits.isp_sharpen_detl_oshtmul = uIspSharpenDetlOshtmul;
    o_isp_sharpen_detail_mul.bits.isp_sharpen_detl_ushtmul = uIspSharpenDetlUshtmul;
    pstBeReg->ISP_SHARPEN_DETAIL_MUL.u32 = o_isp_sharpen_detail_mul.u32;
}

static inline GK_VOID isp_sharpen_osht_detail_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenOshtDtlThd0, GK_U32 uIspSharpenOshtDtlThd1, GK_U32 uIspSharpenOshtDtlWgt, GK_U32 uIspSharpenMaxVarClip)
{
    U_ISP_SHARPEN_OSHT_DETAIL o_isp_sharpen_osht_detail;
    o_isp_sharpen_osht_detail.bits.isp_sharpen_osht_dtl_thd0 = uIspSharpenOshtDtlThd0;
    o_isp_sharpen_osht_detail.bits.isp_sharpen_osht_dtl_thd1 = uIspSharpenOshtDtlThd1;
    o_isp_sharpen_osht_detail.bits.isp_sharpen_osht_dtl_wgt = uIspSharpenOshtDtlWgt;
    o_isp_sharpen_osht_detail.bits.isp_sharpen_max_var_clip = uIspSharpenMaxVarClip;
    pstBeReg->ISP_SHARPEN_OSHT_DETAIL.u32 = o_isp_sharpen_osht_detail.u32;
}

static inline GK_VOID isp_sharpen_usht_detail_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenUshtDtlThd0, GK_U32 uIspSharpenUshtDtlThd1, GK_U32 uIspSharpenUshtDtlWgt, GK_U32 uIspSharpenDtlThdsft)
{
    U_ISP_SHARPEN_USHT_DETAIL o_isp_sharpen_usht_detail;
    o_isp_sharpen_usht_detail.bits.isp_sharpen_usht_dtl_thd0 = uIspSharpenUshtDtlThd0;
    o_isp_sharpen_usht_detail.bits.isp_sharpen_usht_dtl_thd1 = uIspSharpenUshtDtlThd1;
    o_isp_sharpen_usht_detail.bits.isp_sharpen_usht_dtl_wgt = uIspSharpenUshtDtlWgt;
    o_isp_sharpen_usht_detail.bits.isp_sharpen_dtl_thdsft = uIspSharpenDtlThdsft;
    pstBeReg->ISP_SHARPEN_USHT_DETAIL.u32 = o_isp_sharpen_usht_detail.u32;
}

static inline GK_VOID isp_sharpen_stt2lut_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenStt2lutEn)
{
    U_ISP_SHARPEN_STT2LUT_CFG o_isp_sharpen_stt2lut_cfg;
    o_isp_sharpen_stt2lut_cfg.bits.isp_sharpen_stt2lut_en = uIspSharpenStt2lutEn;
    pstBeReg->ISP_SHARPEN_STT2LUT_CFG.u32 = o_isp_sharpen_stt2lut_cfg.u32;
}

static inline GK_VOID isp_sharpen_stt2lut_regnew_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenStt2lutRegnew)
{
    U_ISP_SHARPEN_STT2LUT_REGNEW o_isp_sharpen_stt2lut_regnew;
    o_isp_sharpen_stt2lut_regnew.bits.isp_sharpen_stt2lut_regnew = uIspSharpenStt2lutRegnew;
    pstBeReg->ISP_SHARPEN_STT2LUT_REGNEW.u32 = o_isp_sharpen_stt2lut_regnew.u32;
}

static inline GK_VOID isp_sharpen_stt2lut_abn_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenStt2lutClr)
{
    U_ISP_SHARPEN_STT2LUT_ABN o_isp_sharpen_stt2lut_abn;
    o_isp_sharpen_stt2lut_abn.bits.isp_sharpen_stt2lut_clr = uIspSharpenStt2lutClr;
    pstBeReg->ISP_SHARPEN_STT2LUT_ABN.u32 = o_isp_sharpen_stt2lut_abn.u32;
}

static inline GK_VOID isp_sharpen_dlpf_coef_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenDlpfcoef0, GK_U32 uIspSharpenDlpfcoef1, GK_U32 uIspSharpenDlpfcoef2, GK_U32 uIspSharpenDlpfsft)
{
    U_ISP_SHARPEN_DLPF_COEF o_isp_sharpen_dlpf_coef;
    o_isp_sharpen_dlpf_coef.bits.isp_sharpen_dlpfcoef0 = uIspSharpenDlpfcoef0;
    o_isp_sharpen_dlpf_coef.bits.isp_sharpen_dlpfcoef1 = uIspSharpenDlpfcoef1;
    o_isp_sharpen_dlpf_coef.bits.isp_sharpen_dlpfcoef2 = uIspSharpenDlpfcoef2;
    o_isp_sharpen_dlpf_coef.bits.isp_sharpen_dlpfsft = uIspSharpenDlpfsft;
    pstBeReg->ISP_SHARPEN_DLPF_COEF.u32 = o_isp_sharpen_dlpf_coef.u32;
}

static inline GK_VOID isp_sharpen_udlpf_coef_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenUdlpfcoef0, GK_U32 uIspSharpenUdlpfcoef1, GK_U32 uIspSharpenUdlpfcoef2, GK_U32 uIspSharpenUdlpfsft)
{
    U_ISP_SHARPEN_UDLPF_COEF o_isp_sharpen_udlpf_coef;
    o_isp_sharpen_udlpf_coef.bits.isp_sharpen_udlpfcoef0 = uIspSharpenUdlpfcoef0;
    o_isp_sharpen_udlpf_coef.bits.isp_sharpen_udlpfcoef1 = uIspSharpenUdlpfcoef1;
    o_isp_sharpen_udlpf_coef.bits.isp_sharpen_udlpfcoef2 = uIspSharpenUdlpfcoef2;
    o_isp_sharpen_udlpf_coef.bits.isp_sharpen_udlpfsft = uIspSharpenUdlpfsft;
    pstBeReg->ISP_SHARPEN_UDLPF_COEF.u32 = o_isp_sharpen_udlpf_coef.u32;
}

static inline GK_VOID isp_sharpen_udhsf_coef_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenUdhsfcoef0, GK_U32 uIspSharpenUdhsfcoef1, GK_U32 uIspSharpenUdhsfcoef2, GK_U32 uIspSharpenUdhsfsft)
{
    U_ISP_SHARPEN_UDHSF_COEF o_isp_sharpen_udhsf_coef;
    o_isp_sharpen_udhsf_coef.bits.isp_sharpen_udhsfcoef0 = uIspSharpenUdhsfcoef0;
    o_isp_sharpen_udhsf_coef.bits.isp_sharpen_udhsfcoef1 = uIspSharpenUdhsfcoef1;
    o_isp_sharpen_udhsf_coef.bits.isp_sharpen_udhsfcoef2 = uIspSharpenUdhsfcoef2;
    o_isp_sharpen_udhsf_coef.bits.isp_sharpen_udhsfsft = uIspSharpenUdhsfsft;
    pstBeReg->ISP_SHARPEN_UDHSF_COEF.u32 = o_isp_sharpen_udhsf_coef.u32;
}

static inline GK_VOID isp_sharpen_dhsf_2dshift_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenDhsfsft)
{
    U_ISP_SHARPEN_DHSF_2DSHIFT o_isp_sharpen_dhsf_2dshift;
    o_isp_sharpen_dhsf_2dshift.bits.isp_sharpen_dhsfsft = uIspSharpenDhsfsft;
    pstBeReg->ISP_SHARPEN_DHSF_2DSHIFT.u32 = o_isp_sharpen_dhsf_2dshift.u32;
}

static inline GK_VOID isp_sharpen_dhsf_2dcoef0_03_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenDhsfcoef00, GK_U32 uIspSharpenDhsfcoef01, GK_U32 uIspSharpenDhsfcoef02, GK_U32 uIspSharpenDhsfcoef03)
{
    U_ISP_SHARPEN_DHSF_2DCOEF0_03 o_isp_sharpen_dhsf_2dcoef0_03;
    o_isp_sharpen_dhsf_2dcoef0_03.bits.isp_sharpen_dhsfcoef0_0 = uIspSharpenDhsfcoef00;
    o_isp_sharpen_dhsf_2dcoef0_03.bits.isp_sharpen_dhsfcoef0_1 = uIspSharpenDhsfcoef01;
    o_isp_sharpen_dhsf_2dcoef0_03.bits.isp_sharpen_dhsfcoef0_2 = uIspSharpenDhsfcoef02;
    o_isp_sharpen_dhsf_2dcoef0_03.bits.isp_sharpen_dhsfcoef0_3 = uIspSharpenDhsfcoef03;
    pstBeReg->ISP_SHARPEN_DHSF_2DCOEF0_03.u32 = o_isp_sharpen_dhsf_2dcoef0_03.u32;
}

static inline GK_VOID isp_sharpen_dhsf_2dcoef0_47_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenDhsfcoef04, GK_U32 uIspSharpenDhsfcoef05, GK_U32 uIspSharpenDhsfcoef06, GK_U32 uIspSharpenDhsfcoef07)
{
    U_ISP_SHARPEN_DHSF_2DCOEF0_47 o_isp_sharpen_dhsf_2dcoef0_47;
    o_isp_sharpen_dhsf_2dcoef0_47.bits.isp_sharpen_dhsfcoef0_4 = uIspSharpenDhsfcoef04;
    o_isp_sharpen_dhsf_2dcoef0_47.bits.isp_sharpen_dhsfcoef0_5 = uIspSharpenDhsfcoef05;
    o_isp_sharpen_dhsf_2dcoef0_47.bits.isp_sharpen_dhsfcoef0_6 = uIspSharpenDhsfcoef06;
    o_isp_sharpen_dhsf_2dcoef0_47.bits.isp_sharpen_dhsfcoef0_7 = uIspSharpenDhsfcoef07;
    pstBeReg->ISP_SHARPEN_DHSF_2DCOEF0_47.u32 = o_isp_sharpen_dhsf_2dcoef0_47.u32;
}

static inline GK_VOID isp_sharpen_dhsf_2dcoef0_811_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenDhsfcoef08, GK_U32 uIspSharpenDhsfcoef09, GK_U32 uIspSharpenDhsfcoef010, GK_U32 uIspSharpenDhsfcoef011)
{
    U_ISP_SHARPEN_DHSF_2DCOEF0_811 o_isp_sharpen_dhsf_2dcoef0_811;
    o_isp_sharpen_dhsf_2dcoef0_811.bits.isp_sharpen_dhsfcoef0_8 = uIspSharpenDhsfcoef08;
    o_isp_sharpen_dhsf_2dcoef0_811.bits.isp_sharpen_dhsfcoef0_9 = uIspSharpenDhsfcoef09;
    o_isp_sharpen_dhsf_2dcoef0_811.bits.isp_sharpen_dhsfcoef0_10 = uIspSharpenDhsfcoef010;
    o_isp_sharpen_dhsf_2dcoef0_811.bits.isp_sharpen_dhsfcoef0_11 = uIspSharpenDhsfcoef011;
    pstBeReg->ISP_SHARPEN_DHSF_2DCOEF0_811.u32 = o_isp_sharpen_dhsf_2dcoef0_811.u32;
}

static inline GK_VOID isp_sharpen_dhsf_2dcoef0_1215_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenDhsfcoef012)
{
    U_ISP_SHARPEN_DHSF_2DCOEF0_1215 o_isp_sharpen_dhsf_2dcoef0_1215;
    o_isp_sharpen_dhsf_2dcoef0_1215.bits.isp_sharpen_dhsfcoef0_12 = uIspSharpenDhsfcoef012;
    pstBeReg->ISP_SHARPEN_DHSF_2DCOEF0_1215.u32 = o_isp_sharpen_dhsf_2dcoef0_1215.u32;
}

static inline GK_VOID isp_sharpen_dhsf_2dcoef1_03_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenDhsfcoef10, GK_U32 uIspSharpenDhsfcoef11, GK_U32 uIspSharpenDhsfcoef12, GK_U32 uIspSharpenDhsfcoef13)
{
    U_ISP_SHARPEN_DHSF_2DCOEF1_03 o_isp_sharpen_dhsf_2dcoef1_03;
    o_isp_sharpen_dhsf_2dcoef1_03.bits.isp_sharpen_dhsfcoef1_0 = uIspSharpenDhsfcoef10;
    o_isp_sharpen_dhsf_2dcoef1_03.bits.isp_sharpen_dhsfcoef1_1 = uIspSharpenDhsfcoef11;
    o_isp_sharpen_dhsf_2dcoef1_03.bits.isp_sharpen_dhsfcoef1_2 = uIspSharpenDhsfcoef12;
    o_isp_sharpen_dhsf_2dcoef1_03.bits.isp_sharpen_dhsfcoef1_3 = uIspSharpenDhsfcoef13;
    pstBeReg->ISP_SHARPEN_DHSF_2DCOEF1_03.u32 = o_isp_sharpen_dhsf_2dcoef1_03.u32;
}

static inline GK_VOID isp_sharpen_dhsf_2dcoef1_47_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenDhsfcoef14, GK_U32 uIspSharpenDhsfcoef15, GK_U32 uIspSharpenDhsfcoef16, GK_U32 uIspSharpenDhsfcoef17)
{
    U_ISP_SHARPEN_DHSF_2DCOEF1_47 o_isp_sharpen_dhsf_2dcoef1_47;
    o_isp_sharpen_dhsf_2dcoef1_47.bits.isp_sharpen_dhsfcoef1_4 = uIspSharpenDhsfcoef14;
    o_isp_sharpen_dhsf_2dcoef1_47.bits.isp_sharpen_dhsfcoef1_5 = uIspSharpenDhsfcoef15;
    o_isp_sharpen_dhsf_2dcoef1_47.bits.isp_sharpen_dhsfcoef1_6 = uIspSharpenDhsfcoef16;
    o_isp_sharpen_dhsf_2dcoef1_47.bits.isp_sharpen_dhsfcoef1_7 = uIspSharpenDhsfcoef17;
    pstBeReg->ISP_SHARPEN_DHSF_2DCOEF1_47.u32 = o_isp_sharpen_dhsf_2dcoef1_47.u32;
}

static inline GK_VOID isp_sharpen_dhsf_2dcoef1_811_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenDhsfcoef18, GK_U32 uIspSharpenDhsfcoef19, GK_U32 uIspSharpenDhsfcoef110, GK_U32 uIspSharpenDhsfcoef111)
{
    U_ISP_SHARPEN_DHSF_2DCOEF1_811 o_isp_sharpen_dhsf_2dcoef1_811;
    o_isp_sharpen_dhsf_2dcoef1_811.bits.isp_sharpen_dhsfcoef1_8 = uIspSharpenDhsfcoef18;
    o_isp_sharpen_dhsf_2dcoef1_811.bits.isp_sharpen_dhsfcoef1_9 = uIspSharpenDhsfcoef19;
    o_isp_sharpen_dhsf_2dcoef1_811.bits.isp_sharpen_dhsfcoef1_10 = uIspSharpenDhsfcoef110;
    o_isp_sharpen_dhsf_2dcoef1_811.bits.isp_sharpen_dhsfcoef1_11 = uIspSharpenDhsfcoef111;
    pstBeReg->ISP_SHARPEN_DHSF_2DCOEF1_811.u32 = o_isp_sharpen_dhsf_2dcoef1_811.u32;
}

static inline GK_VOID isp_sharpen_dhsf_2dcoef1_1215_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenDhsfcoef112)
{
    U_ISP_SHARPEN_DHSF_2DCOEF1_1215 o_isp_sharpen_dhsf_2dcoef1_1215;
    o_isp_sharpen_dhsf_2dcoef1_1215.bits.isp_sharpen_dhsfcoef1_12 = uIspSharpenDhsfcoef112;
    pstBeReg->ISP_SHARPEN_DHSF_2DCOEF1_1215.u32 = o_isp_sharpen_dhsf_2dcoef1_1215.u32;
}

static inline GK_VOID isp_sharpen_dhsf_2dcoef2_03_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenDhsfcoef20, GK_U32 uIspSharpenDhsfcoef21, GK_U32 uIspSharpenDhsfcoef22, GK_U32 uIspSharpenDhsfcoef23)
{
    U_ISP_SHARPEN_DHSF_2DCOEF2_03 o_isp_sharpen_dhsf_2dcoef2_03;
    o_isp_sharpen_dhsf_2dcoef2_03.bits.isp_sharpen_dhsfcoef2_0 = uIspSharpenDhsfcoef20;
    o_isp_sharpen_dhsf_2dcoef2_03.bits.isp_sharpen_dhsfcoef2_1 = uIspSharpenDhsfcoef21;
    o_isp_sharpen_dhsf_2dcoef2_03.bits.isp_sharpen_dhsfcoef2_2 = uIspSharpenDhsfcoef22;
    o_isp_sharpen_dhsf_2dcoef2_03.bits.isp_sharpen_dhsfcoef2_3 = uIspSharpenDhsfcoef23;
    pstBeReg->ISP_SHARPEN_DHSF_2DCOEF2_03.u32 = o_isp_sharpen_dhsf_2dcoef2_03.u32;
}

static inline GK_VOID isp_sharpen_dhsf_2dcoef2_47_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenDhsfcoef24, GK_U32 uIspSharpenDhsfcoef25, GK_U32 uIspSharpenDhsfcoef26, GK_U32 uIspSharpenDhsfcoef27)
{
    U_ISP_SHARPEN_DHSF_2DCOEF2_47 o_isp_sharpen_dhsf_2dcoef2_47;
    o_isp_sharpen_dhsf_2dcoef2_47.bits.isp_sharpen_dhsfcoef2_4 = uIspSharpenDhsfcoef24;
    o_isp_sharpen_dhsf_2dcoef2_47.bits.isp_sharpen_dhsfcoef2_5 = uIspSharpenDhsfcoef25;
    o_isp_sharpen_dhsf_2dcoef2_47.bits.isp_sharpen_dhsfcoef2_6 = uIspSharpenDhsfcoef26;
    o_isp_sharpen_dhsf_2dcoef2_47.bits.isp_sharpen_dhsfcoef2_7 = uIspSharpenDhsfcoef27;
    pstBeReg->ISP_SHARPEN_DHSF_2DCOEF2_47.u32 = o_isp_sharpen_dhsf_2dcoef2_47.u32;
}

static inline GK_VOID isp_sharpen_dhsf_2dcoef2_811_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenDhsfcoef28, GK_U32 uIspSharpenDhsfcoef29, GK_U32 uIspSharpenDhsfcoef210, GK_U32 uIspSharpenDhsfcoef211)
{
    U_ISP_SHARPEN_DHSF_2DCOEF2_811 o_isp_sharpen_dhsf_2dcoef2_811;
    o_isp_sharpen_dhsf_2dcoef2_811.bits.isp_sharpen_dhsfcoef2_8 = uIspSharpenDhsfcoef28;
    o_isp_sharpen_dhsf_2dcoef2_811.bits.isp_sharpen_dhsfcoef2_9 = uIspSharpenDhsfcoef29;
    o_isp_sharpen_dhsf_2dcoef2_811.bits.isp_sharpen_dhsfcoef2_10 = uIspSharpenDhsfcoef210;
    o_isp_sharpen_dhsf_2dcoef2_811.bits.isp_sharpen_dhsfcoef2_11 = uIspSharpenDhsfcoef211;
    pstBeReg->ISP_SHARPEN_DHSF_2DCOEF2_811.u32 = o_isp_sharpen_dhsf_2dcoef2_811.u32;
}

static inline GK_VOID isp_sharpen_dhsf_2dcoef2_1215_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenDhsfcoef212)
{
    U_ISP_SHARPEN_DHSF_2DCOEF2_1215 o_isp_sharpen_dhsf_2dcoef2_1215;
    o_isp_sharpen_dhsf_2dcoef2_1215.bits.isp_sharpen_dhsfcoef2_12 = uIspSharpenDhsfcoef212;
    pstBeReg->ISP_SHARPEN_DHSF_2DCOEF2_1215.u32 = o_isp_sharpen_dhsf_2dcoef2_1215.u32;
}

static inline GK_VOID isp_sharpen_dhsf_2dcoef3_03_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenDhsfcoef30, GK_U32 uIspSharpenDhsfcoef31, GK_U32 uIspSharpenDhsfcoef32, GK_U32 uIspSharpenDhsfcoef33)
{
    U_ISP_SHARPEN_DHSF_2DCOEF3_03 o_isp_sharpen_dhsf_2dcoef3_03;
    o_isp_sharpen_dhsf_2dcoef3_03.bits.isp_sharpen_dhsfcoef3_0 = uIspSharpenDhsfcoef30;
    o_isp_sharpen_dhsf_2dcoef3_03.bits.isp_sharpen_dhsfcoef3_1 = uIspSharpenDhsfcoef31;
    o_isp_sharpen_dhsf_2dcoef3_03.bits.isp_sharpen_dhsfcoef3_2 = uIspSharpenDhsfcoef32;
    o_isp_sharpen_dhsf_2dcoef3_03.bits.isp_sharpen_dhsfcoef3_3 = uIspSharpenDhsfcoef33;
    pstBeReg->ISP_SHARPEN_DHSF_2DCOEF3_03.u32 = o_isp_sharpen_dhsf_2dcoef3_03.u32;
}

static inline GK_VOID isp_sharpen_dhsf_2dcoef3_47_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenDhsfcoef34, GK_U32 uIspSharpenDhsfcoef35, GK_U32 uIspSharpenDhsfcoef36, GK_U32 uIspSharpenDhsfcoef37)
{
    U_ISP_SHARPEN_DHSF_2DCOEF3_47 o_isp_sharpen_dhsf_2dcoef3_47;
    o_isp_sharpen_dhsf_2dcoef3_47.bits.isp_sharpen_dhsfcoef3_4 = uIspSharpenDhsfcoef34;
    o_isp_sharpen_dhsf_2dcoef3_47.bits.isp_sharpen_dhsfcoef3_5 = uIspSharpenDhsfcoef35;
    o_isp_sharpen_dhsf_2dcoef3_47.bits.isp_sharpen_dhsfcoef3_6 = uIspSharpenDhsfcoef36;
    o_isp_sharpen_dhsf_2dcoef3_47.bits.isp_sharpen_dhsfcoef3_7 = uIspSharpenDhsfcoef37;
    pstBeReg->ISP_SHARPEN_DHSF_2DCOEF3_47.u32 = o_isp_sharpen_dhsf_2dcoef3_47.u32;
}

static inline GK_VOID isp_sharpen_dhsf_2dcoef3_811_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenDhsfcoef38, GK_U32 uIspSharpenDhsfcoef39, GK_U32 uIspSharpenDhsfcoef310, GK_U32 uIspSharpenDhsfcoef311)
{
    U_ISP_SHARPEN_DHSF_2DCOEF3_811 o_isp_sharpen_dhsf_2dcoef3_811;
    o_isp_sharpen_dhsf_2dcoef3_811.bits.isp_sharpen_dhsfcoef3_8 = uIspSharpenDhsfcoef38;
    o_isp_sharpen_dhsf_2dcoef3_811.bits.isp_sharpen_dhsfcoef3_9 = uIspSharpenDhsfcoef39;
    o_isp_sharpen_dhsf_2dcoef3_811.bits.isp_sharpen_dhsfcoef3_10 = uIspSharpenDhsfcoef310;
    o_isp_sharpen_dhsf_2dcoef3_811.bits.isp_sharpen_dhsfcoef3_11 = uIspSharpenDhsfcoef311;
    pstBeReg->ISP_SHARPEN_DHSF_2DCOEF3_811.u32 = o_isp_sharpen_dhsf_2dcoef3_811.u32;
}

static inline GK_VOID isp_sharpen_dhsf_2dcoef3_1215_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenDhsfcoef312)
{
    U_ISP_SHARPEN_DHSF_2DCOEF3_1215 o_isp_sharpen_dhsf_2dcoef3_1215;
    o_isp_sharpen_dhsf_2dcoef3_1215.bits.isp_sharpen_dhsfcoef3_12 = uIspSharpenDhsfcoef312;
    pstBeReg->ISP_SHARPEN_DHSF_2DCOEF3_1215.u32 = o_isp_sharpen_dhsf_2dcoef3_1215.u32;
}

static inline GK_VOID isp_sharpen_ldci_dither_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspSharpenLdciDitherRound)
{
    U_ISP_SHARPEN_LDCI_DITHER_CFG o_isp_sharpen_ldci_dither_cfg;
    o_isp_sharpen_ldci_dither_cfg.bits.isp_sharpen_ldci_dither_round = uIspSharpenLdciDitherRound;
    pstBeReg->ISP_SHARPEN_LDCI_DITHER_CFG.u32 = o_isp_sharpen_ldci_dither_cfg.u32;
}

static inline GK_VOID isp_nddm_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspNddmCacBlendEn)
{
    U_ISP_NDDM_CFG o_isp_nddm_cfg;
    o_isp_nddm_cfg.bits.isp_nddm_cac_blend_en = uIspNddmCacBlendEn;
    pstBeReg->ISP_NDDM_CFG.u32 = o_isp_nddm_cfg.u32;
}

static inline GK_VOID isp_nddm_gf_th_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspNddmGfThLow, GK_U32 uIspNddmGfThHigh)
{
    U_ISP_NDDM_GF_TH o_isp_nddm_gf_th;
    o_isp_nddm_gf_th.bits.isp_nddm_gf_th_low = uIspNddmGfThLow;
    o_isp_nddm_gf_th.bits.isp_nddm_gf_thgh = uIspNddmGfThHigh;
    pstBeReg->ISP_NDDM_GF_TH.u32 = o_isp_nddm_gf_th.u32;
}

static inline GK_VOID isp_nddm_awb_gf_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspNddmAwbGfGnLow, GK_U32 uIspNddmAwbGfGnHigh, GK_U32 uIspNddmAwbGfGnMax)
{
    U_ISP_NDDM_AWB_GF_CFG o_isp_nddm_awb_gf_cfg;
    o_isp_nddm_awb_gf_cfg.bits.isp_nddm_awb_gf_gn_low = uIspNddmAwbGfGnLow;
    o_isp_nddm_awb_gf_cfg.bits.isp_nddm_awb_gf_gngh = uIspNddmAwbGfGnHigh;
    o_isp_nddm_awb_gf_cfg.bits.isp_nddm_awb_gf_gn_max = uIspNddmAwbGfGnMax;
    pstBeReg->ISP_NDDM_AWB_GF_CFG.u32 = o_isp_nddm_awb_gf_cfg.u32;
}

static inline GK_VOID isp_nddm_fcr_gain_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspNddmFcrGfGain, GK_U32 uIspNddmFcrDetLow)
{
    U_ISP_NDDM_FCR_GAIN o_isp_nddm_fcr_gain;
    o_isp_nddm_fcr_gain.bits.isp_nddm_fcr_gf_gain = uIspNddmFcrGfGain;
    o_isp_nddm_fcr_gain.bits.isp_nddm_fcr_det_low = uIspNddmFcrDetLow;
    pstBeReg->ISP_NDDM_FCR_GAIN.u32 = o_isp_nddm_fcr_gain.u32;
}

static inline GK_VOID isp_nddm_dm_bldrate_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspNddmCacBlendRate)
{
    U_ISP_NDDM_DM_BLDRATE o_isp_nddm_dm_bldrate;
    o_isp_nddm_dm_bldrate.bits.isp_nddm_cac_blend_rate = uIspNddmCacBlendRate;
    pstBeReg->ISP_NDDM_DM_BLDRATE.u32 = o_isp_nddm_dm_bldrate.u32;
}

static inline GK_VOID isp_nddm_ehc_gray_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspNddmEhcGray)
{
    U_ISP_NDDM_EHC_GRAY o_isp_nddm_ehc_gray;
    o_isp_nddm_ehc_gray.bits.isp_nddm_ehc_gray = uIspNddmEhcGray;
    pstBeReg->ISP_NDDM_EHC_GRAY.u32 = o_isp_nddm_ehc_gray.u32;
}

static inline GK_VOID isp_nddm_dither_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspNddmDithMask, GK_U32 uIspNddmDithRatio, GK_U32 uIspNddmDithMax)
{
    U_ISP_NDDM_DITHER_CFG o_isp_nddm_dither_cfg;
    o_isp_nddm_dither_cfg.bits.isp_nddm_dith_mask = uIspNddmDithMask;
    o_isp_nddm_dither_cfg.bits.isp_nddm_dith_ratio = uIspNddmDithRatio;
    o_isp_nddm_dither_cfg.bits.isp_nddm_dith_max = uIspNddmDithMax;
    pstBeReg->ISP_NDDM_DITHER_CFG.u32 = o_isp_nddm_dither_cfg.u32;
}

static inline GK_VOID isp_nddm_gf_lut_update_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspNddmGfLutUpdate)
{
    U_ISP_NDDM_GF_LUT_UPDATE o_isp_nddm_gf_lut_update;
    o_isp_nddm_gf_lut_update.bits.isp_nddm_gf_lut_update = uIspNddmGfLutUpdate;
    pstBeReg->ISP_NDDM_GF_LUT_UPDATE.u32 = o_isp_nddm_gf_lut_update.u32;
}

static inline GK_VOID isp_nddm_gf_lut_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspNddmGfLutWaddr)
{
    U_ISP_NDDM_GF_LUT_WADDR o_isp_nddm_gf_lut_waddr;
    o_isp_nddm_gf_lut_waddr.bits.isp_nddm_gf_lut_waddr = uIspNddmGfLutWaddr;
    pstBeReg->ISP_NDDM_GF_LUT_WADDR.u32 = o_isp_nddm_gf_lut_waddr.u32;
}

static inline GK_VOID isp_nddm_gf_lut_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspNddmGfLutWdata)
{
    U_ISP_NDDM_GF_LUT_WDATA o_isp_nddm_gf_lut_wdata;
    o_isp_nddm_gf_lut_wdata.bits.isp_nddm_gf_lut_wdata = uIspNddmGfLutWdata;
    pstBeReg->ISP_NDDM_GF_LUT_WDATA.u32 = o_isp_nddm_gf_lut_wdata.u32;
}

static inline GK_VOID isp_nddm_gf_lut_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspNddmGfLutRaddr)
{
    U_ISP_NDDM_GF_LUT_RADDR o_isp_nddm_gf_lut_raddr;
    o_isp_nddm_gf_lut_raddr.bits.isp_nddm_gf_lut_raddr = uIspNddmGfLutRaddr;
    pstBeReg->ISP_NDDM_GF_LUT_RADDR.u32 = o_isp_nddm_gf_lut_raddr.u32;
}

static inline GK_VOID isp_bnr_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrMonoSensor)
{
    U_ISP_BNR_CFG o_isp_bnr_cfg;
    o_isp_bnr_cfg.bits.isp_bnr_mono_sensor = uIspBnrMonoSensor;
    pstBeReg->ISP_BNR_CFG.u32 = o_isp_bnr_cfg.u32;
}

static inline GK_VOID isp_bnr_jnlm0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrJnlmCoringhig, GK_U32 uIspBnrJnlmGain, GK_U32 uIspBnrJnlmSel)
{
    U_ISP_BNR_JNLM0 o_isp_bnr_jnlm0;
    o_isp_bnr_jnlm0.bits.isp_bnr_jnlm_coringhig = uIspBnrJnlmCoringhig;
    o_isp_bnr_jnlm0.bits.isp_bnr_jnlm_gain = uIspBnrJnlmGain;
    o_isp_bnr_jnlm0.bits.isp_bnr_jnlm_sel = uIspBnrJnlmSel;
    pstBeReg->ISP_BNR_JNLM0.u32 = o_isp_bnr_jnlm0.u32;
}

static inline GK_VOID isp_bnr_lsc_ctrl_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrLscbnrenable, GK_U32 uIspBnrLscmaxgain, GK_U32 uIspBnrLsccmpstrength)
{
    U_ISP_BNR_LSC_CTRL o_isp_bnr_lsc_ctrl;
    o_isp_bnr_lsc_ctrl.bits.isp_bnr_lscbnrenable = uIspBnrLscbnrenable;
    o_isp_bnr_lsc_ctrl.bits.isp_bnr_lscmaxgain = uIspBnrLscmaxgain;
    o_isp_bnr_lsc_ctrl.bits.isp_bnr_lsccmpstrength = uIspBnrLsccmpstrength;
    pstBeReg->ISP_BNR_LSC_CTRL.u32 = o_isp_bnr_lsc_ctrl.u32;
}

static inline GK_VOID isp_bnr_jnlm2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrJnlmMaxwtcoef)
{
    U_ISP_BNR_JNLM2 o_isp_bnr_jnlm2;
    o_isp_bnr_jnlm2.bits.isp_bnr_jnlm_maxwtcoef = uIspBnrJnlmMaxwtcoef;
    pstBeReg->ISP_BNR_JNLM2.u32 = o_isp_bnr_jnlm2.u32;
}

static inline GK_VOID isp_bnr_chroma_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrRlmtBlc, GK_U32 uIspBnrRlmtRgain, GK_U32 uIspBnrRlmtBgain)
{
    U_ISP_BNR_CHROMA o_isp_bnr_chroma;
    o_isp_bnr_chroma.bits.isp_bnr_rlmt_blc = uIspBnrRlmtBlc;
    o_isp_bnr_chroma.bits.isp_bnr_rlmt_rgain = uIspBnrRlmtRgain;
    o_isp_bnr_chroma.bits.isp_bnr_rlmt_bgain = uIspBnrRlmtBgain;
    pstBeReg->ISP_BNR_CHROMA.u32 = o_isp_bnr_chroma.u32;
}

static inline GK_VOID isp_bnr_lpfctrl_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrWindowsizesel, GK_U32 uIspBnrEncenterweight, GK_U32 uIspBnrPatternNoiseReduction, GK_U32 uIspBnrGainsad, GK_U32 uIspBnrImplsnrenable1, GK_U32 uIspBnrImplsnrenable2, GK_U32 uIspBnrImplsnrenable3, GK_U32 uIspBnrImplsnrstrength, GK_U32 uIspBnrEnablesymmsad, GK_U32 uIspBnrEnablepostproc, GK_U32 uIspBnrBilateralEnable, GK_U32 uIspBnrLumasel)
{
    U_ISP_BNR_LPFCTRL o_isp_bnr_lpfctrl;
    o_isp_bnr_lpfctrl.bits.isp_bnr_windowsizesel = uIspBnrWindowsizesel;
    o_isp_bnr_lpfctrl.bits.isp_bnr_encenterweight = uIspBnrEncenterweight;
    o_isp_bnr_lpfctrl.bits.isp_bnr_pattern_noise_reduction = uIspBnrPatternNoiseReduction;
    o_isp_bnr_lpfctrl.bits.isp_bnr_gainsad = uIspBnrGainsad;
    o_isp_bnr_lpfctrl.bits.isp_bnr_implsnrenable1 = uIspBnrImplsnrenable1;
    o_isp_bnr_lpfctrl.bits.isp_bnr_implsnrenable2 = uIspBnrImplsnrenable2;
    o_isp_bnr_lpfctrl.bits.isp_bnr_implsnrenable3 = uIspBnrImplsnrenable3;
    o_isp_bnr_lpfctrl.bits.isp_bnr_implsnrstrength = uIspBnrImplsnrstrength;
    o_isp_bnr_lpfctrl.bits.isp_bnr_enablesymmsad = uIspBnrEnablesymmsad;
    o_isp_bnr_lpfctrl.bits.isp_bnr_enablepostproc = uIspBnrEnablepostproc;
    o_isp_bnr_lpfctrl.bits.isp_bnr_bilateral_enable = uIspBnrBilateralEnable;
    o_isp_bnr_lpfctrl.bits.isp_bnr_lumasel = uIspBnrLumasel;
    pstBeReg->ISP_BNR_LPFCTRL.u32 = o_isp_bnr_lpfctrl.u32;
}

static inline GK_VOID isp_bnr_wdrctrl_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrWdrEnable, GK_U32 uIspBnrWdrEnfusion, GK_U32 uIspBnrWdrMapfltmod, GK_U32 uIspBnrWdrMapgain)
{
    U_ISP_BNR_WDRCTRL o_isp_bnr_wdrctrl;
    o_isp_bnr_wdrctrl.bits.isp_bnr_wdr_enable = uIspBnrWdrEnable;
    o_isp_bnr_wdrctrl.bits.isp_bnr_wdr_enfusion = uIspBnrWdrEnfusion;
    o_isp_bnr_wdrctrl.bits.isp_bnr_wdr_mapfltmod = uIspBnrWdrMapfltmod;
    o_isp_bnr_wdrctrl.bits.isp_bnr_wdr_mapgain = uIspBnrWdrMapgain;
    pstBeReg->ISP_BNR_WDRCTRL.u32 = o_isp_bnr_wdrctrl.u32;
}

static inline GK_VOID isp_bnr_jnlmgain0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrJnlmgainR0)
{
    U_ISP_BNR_JNLMGAIN0 o_isp_bnr_jnlmgain0;
    o_isp_bnr_jnlmgain0.bits.isp_bnr_jnlmgain_r0 = uIspBnrJnlmgainR0;
    pstBeReg->ISP_BNR_JNLMGAIN0.u32 = o_isp_bnr_jnlmgain0.u32;
}

static inline GK_VOID isp_bnr_jnlmgain1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrJnlmgainGr0)
{
    U_ISP_BNR_JNLMGAIN1 o_isp_bnr_jnlmgain1;
    o_isp_bnr_jnlmgain1.bits.isp_bnr_jnlmgain_gr0 = uIspBnrJnlmgainGr0;
    pstBeReg->ISP_BNR_JNLMGAIN1.u32 = o_isp_bnr_jnlmgain1.u32;
}

static inline GK_VOID isp_bnr_jnlmgain2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrJnlmgainGb0)
{
    U_ISP_BNR_JNLMGAIN2 o_isp_bnr_jnlmgain2;
    o_isp_bnr_jnlmgain2.bits.isp_bnr_jnlmgain_gb0 = uIspBnrJnlmgainGb0;
    pstBeReg->ISP_BNR_JNLMGAIN2.u32 = o_isp_bnr_jnlmgain2.u32;
}

static inline GK_VOID isp_bnr_jnlmgain3_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrJnlmgainB0)
{
    U_ISP_BNR_JNLMGAIN3 o_isp_bnr_jnlmgain3;
    o_isp_bnr_jnlmgain3.bits.isp_bnr_jnlmgain_b0 = uIspBnrJnlmgainB0;
    pstBeReg->ISP_BNR_JNLMGAIN3.u32 = o_isp_bnr_jnlmgain3.u32;
}

static inline GK_VOID isp_bnr_jnlmgain_s0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrJnlmgainRS)
{
    U_ISP_BNR_JNLMGAIN_S0 o_isp_bnr_jnlmgain_s0;
    o_isp_bnr_jnlmgain_s0.bits.isp_bnr_jnlmgain_r_s = uIspBnrJnlmgainRS;
    pstBeReg->ISP_BNR_JNLMGAIN_S0.u32 = o_isp_bnr_jnlmgain_s0.u32;
}

static inline GK_VOID isp_bnr_jnlmgain_s1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrJnlmgainGrS)
{
    U_ISP_BNR_JNLMGAIN_S1 o_isp_bnr_jnlmgain_s1;
    o_isp_bnr_jnlmgain_s1.bits.isp_bnr_jnlmgain_gr_s = uIspBnrJnlmgainGrS;
    pstBeReg->ISP_BNR_JNLMGAIN_S1.u32 = o_isp_bnr_jnlmgain_s1.u32;
}

static inline GK_VOID isp_bnr_jnlmgain_s2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrJnlmgainGbS)
{
    U_ISP_BNR_JNLMGAIN_S2 o_isp_bnr_jnlmgain_s2;
    o_isp_bnr_jnlmgain_s2.bits.isp_bnr_jnlmgain_gb_s = uIspBnrJnlmgainGbS;
    pstBeReg->ISP_BNR_JNLMGAIN_S2.u32 = o_isp_bnr_jnlmgain_s2.u32;
}

static inline GK_VOID isp_bnr_jnlmgain_s3_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrJnlmgainBS)
{
    U_ISP_BNR_JNLMGAIN_S3 o_isp_bnr_jnlmgain_s3;
    o_isp_bnr_jnlmgain_s3.bits.isp_bnr_jnlmgain_b_s = uIspBnrJnlmgainBS;
    pstBeReg->ISP_BNR_JNLMGAIN_S3.u32 = o_isp_bnr_jnlmgain_s3.u32;
}

static inline GK_VOID isp_bnr_shotratio_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrShotratio)
{
    U_ISP_BNR_SHOTRATIO o_isp_bnr_shotratio;
    o_isp_bnr_shotratio.bits.isp_bnr_shotratio = uIspBnrShotratio;
    pstBeReg->ISP_BNR_SHOTRATIO.u32 = o_isp_bnr_shotratio.u32;
}

static inline GK_VOID isp_bnr_stt2lut_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrStt2lutEn)
{
    U_ISP_BNR_STT2LUT_CFG o_isp_bnr_stt2lut_cfg;
    o_isp_bnr_stt2lut_cfg.bits.isp_bnr_stt2lut_en = uIspBnrStt2lutEn;
    pstBeReg->ISP_BNR_STT2LUT_CFG.u32 = o_isp_bnr_stt2lut_cfg.u32;
}

static inline GK_VOID isp_bnr_stt2lut_regnew_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrStt2lutRegnew)
{
    U_ISP_BNR_STT2LUT_REGNEW o_isp_bnr_stt2lut_regnew;
    o_isp_bnr_stt2lut_regnew.bits.isp_bnr_stt2lut_regnew = uIspBnrStt2lutRegnew;
    pstBeReg->ISP_BNR_STT2LUT_REGNEW.u32 = o_isp_bnr_stt2lut_regnew.u32;
}

static inline GK_VOID isp_bnr_stt2lut_abn_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrStt2lutClr)
{
    U_ISP_BNR_STT2LUT_ABN o_isp_bnr_stt2lut_abn;
    o_isp_bnr_stt2lut_abn.bits.isp_bnr_stt2lut_clr = uIspBnrStt2lutClr;
    pstBeReg->ISP_BNR_STT2LUT_ABN.u32 = o_isp_bnr_stt2lut_abn.u32;
}

static inline GK_VOID isp_bnr_lmt_even_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrLmtEvenWaddr)
{
    U_ISP_BNR_LMT_EVEN_WADDR o_isp_bnr_lmt_even_waddr;
    o_isp_bnr_lmt_even_waddr.bits.isp_bnr_lmt_even_waddr = uIspBnrLmtEvenWaddr;
    pstBeReg->ISP_BNR_LMT_EVEN_WADDR.u32 = o_isp_bnr_lmt_even_waddr.u32;
}

static inline GK_VOID isp_bnr_lmt_even_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 u32LutData)
{
    pstBeReg->ISP_BNR_LMT_EVEN_WDATA.u32 = u32LutData;
}

static inline GK_VOID isp_bnr_lmt_even_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrLmtEvenRaddr)
{
    U_ISP_BNR_LMT_EVEN_RADDR o_isp_bnr_lmt_even_raddr;
    o_isp_bnr_lmt_even_raddr.bits.isp_bnr_lmt_even_raddr = uIspBnrLmtEvenRaddr;
    pstBeReg->ISP_BNR_LMT_EVEN_RADDR.u32 = o_isp_bnr_lmt_even_raddr.u32;
}

static inline GK_VOID isp_bnr_lmt_even_rdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrLmtEvenRdata, GK_U32 uIspBnrLmtSEvenRdata)
{
    U_ISP_BNR_LMT_EVEN_RDATA o_isp_bnr_lmt_even_rdata;
    o_isp_bnr_lmt_even_rdata.bits.isp_bnr_lmt_even_rdata = uIspBnrLmtEvenRdata;
    o_isp_bnr_lmt_even_rdata.bits.isp_bnr_lmt_s_even_rdata = uIspBnrLmtSEvenRdata;
    pstBeReg->ISP_BNR_LMT_EVEN_RDATA.u32 = o_isp_bnr_lmt_even_rdata.u32;
}

static inline GK_VOID isp_bnr_lmt_odd_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrLmtOddWaddr)
{
    U_ISP_BNR_LMT_ODD_WADDR o_isp_bnr_lmt_odd_waddr;
    o_isp_bnr_lmt_odd_waddr.bits.isp_bnr_lmt_odd_waddr = uIspBnrLmtOddWaddr;
    pstBeReg->ISP_BNR_LMT_ODD_WADDR.u32 = o_isp_bnr_lmt_odd_waddr.u32;
}

static inline GK_VOID isp_bnr_lmt_odd_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 lutData)
{
    pstBeReg->ISP_BNR_LMT_ODD_WDATA.u32 = lutData;
}

static inline GK_VOID isp_bnr_lmt_odd_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrLmtOddRaddr)
{
    U_ISP_BNR_LMT_ODD_RADDR o_isp_bnr_lmt_odd_raddr;
    o_isp_bnr_lmt_odd_raddr.bits.isp_bnr_lmt_odd_raddr = uIspBnrLmtOddRaddr;
    pstBeReg->ISP_BNR_LMT_ODD_RADDR.u32 = o_isp_bnr_lmt_odd_raddr.u32;
}

static inline GK_VOID isp_bnr_lmt_odd_rdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrLmtOddRdata, GK_U32 uIspBnrLmtSOddRdata)
{
    U_ISP_BNR_LMT_ODD_RDATA o_isp_bnr_lmt_odd_rdata;
    o_isp_bnr_lmt_odd_rdata.bits.isp_bnr_lmt_odd_rdata = uIspBnrLmtOddRdata;
    o_isp_bnr_lmt_odd_rdata.bits.isp_bnr_lmt_s_odd_rdata = uIspBnrLmtSOddRdata;
    pstBeReg->ISP_BNR_LMT_ODD_RDATA.u32 = o_isp_bnr_lmt_odd_rdata.u32;
}

static inline GK_VOID isp_bnr_cor_even_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrCorEvenWaddr)
{
    U_ISP_BNR_COR_EVEN_WADDR o_isp_bnr_cor_even_waddr;
    o_isp_bnr_cor_even_waddr.bits.isp_bnr_cor_even_waddr = uIspBnrCorEvenWaddr;
    pstBeReg->ISP_BNR_COR_EVEN_WADDR.u32 = o_isp_bnr_cor_even_waddr.u32;
}

static inline GK_VOID isp_bnr_cor_even_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrCorEvenWdata)
{
    U_ISP_BNR_COR_EVEN_WDATA o_isp_bnr_cor_even_wdata;
    o_isp_bnr_cor_even_wdata.bits.isp_bnr_cor_even_wdata = uIspBnrCorEvenWdata;
    pstBeReg->ISP_BNR_COR_EVEN_WDATA.u32 = o_isp_bnr_cor_even_wdata.u32;
}

static inline GK_VOID isp_bnr_cor_even_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrCorEvenRaddr)
{
    U_ISP_BNR_COR_EVEN_RADDR o_isp_bnr_cor_even_raddr;
    o_isp_bnr_cor_even_raddr.bits.isp_bnr_cor_even_raddr = uIspBnrCorEvenRaddr;
    pstBeReg->ISP_BNR_COR_EVEN_RADDR.u32 = o_isp_bnr_cor_even_raddr.u32;
}

static inline GK_VOID isp_bnr_cor_odd_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrCorOddWaddr)
{
    U_ISP_BNR_COR_ODD_WADDR o_isp_bnr_cor_odd_waddr;
    o_isp_bnr_cor_odd_waddr.bits.isp_bnr_cor_odd_waddr = uIspBnrCorOddWaddr;
    pstBeReg->ISP_BNR_COR_ODD_WADDR.u32 = o_isp_bnr_cor_odd_waddr.u32;
}

static inline GK_VOID isp_bnr_cor_odd_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrCorOddWdata)
{
    U_ISP_BNR_COR_ODD_WDATA o_isp_bnr_cor_odd_wdata;
    o_isp_bnr_cor_odd_wdata.bits.isp_bnr_cor_odd_wdata = uIspBnrCorOddWdata;
    pstBeReg->ISP_BNR_COR_ODD_WDATA.u32 = o_isp_bnr_cor_odd_wdata.u32;
}

static inline GK_VOID isp_bnr_cor_odd_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrCorOddRaddr)
{
    U_ISP_BNR_COR_ODD_RADDR o_isp_bnr_cor_odd_raddr;
    o_isp_bnr_cor_odd_raddr.bits.isp_bnr_cor_odd_raddr = uIspBnrCorOddRaddr;
    pstBeReg->ISP_BNR_COR_ODD_RADDR.u32 = o_isp_bnr_cor_odd_raddr.u32;
}

static inline GK_VOID isp_bnr_gcor_even_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrGcorEvenWaddr)
{
    U_ISP_BNR_GCOR_EVEN_WADDR o_isp_bnr_gcor_even_waddr;
    o_isp_bnr_gcor_even_waddr.bits.isp_bnr_gcor_even_waddr = uIspBnrGcorEvenWaddr;
    pstBeReg->ISP_BNR_GCOR_EVEN_WADDR.u32 = o_isp_bnr_gcor_even_waddr.u32;
}

static inline GK_VOID isp_bnr_gcor_even_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrGcorEvenWdata)
{
    U_ISP_BNR_GCOR_EVEN_WDATA o_isp_bnr_gcor_even_wdata;
    o_isp_bnr_gcor_even_wdata.bits.isp_bnr_gcor_even_wdata = uIspBnrGcorEvenWdata;
    pstBeReg->ISP_BNR_GCOR_EVEN_WDATA.u32 = o_isp_bnr_gcor_even_wdata.u32;
}

static inline GK_VOID isp_bnr_gcor_even_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrGcorEvenRaddr)
{
    U_ISP_BNR_GCOR_EVEN_RADDR o_isp_bnr_gcor_even_raddr;
    o_isp_bnr_gcor_even_raddr.bits.isp_bnr_gcor_even_raddr = uIspBnrGcorEvenRaddr;
    pstBeReg->ISP_BNR_GCOR_EVEN_RADDR.u32 = o_isp_bnr_gcor_even_raddr.u32;
}

static inline GK_VOID isp_bnr_gcor_odd_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrGcorOddWaddr)
{
    U_ISP_BNR_GCOR_ODD_WADDR o_isp_bnr_gcor_odd_waddr;
    o_isp_bnr_gcor_odd_waddr.bits.isp_bnr_gcor_odd_waddr = uIspBnrGcorOddWaddr;
    pstBeReg->ISP_BNR_GCOR_ODD_WADDR.u32 = o_isp_bnr_gcor_odd_waddr.u32;
}

static inline GK_VOID isp_bnr_gcor_odd_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrGcorOddWdata)
{
    U_ISP_BNR_GCOR_ODD_WDATA o_isp_bnr_gcor_odd_wdata;
    o_isp_bnr_gcor_odd_wdata.bits.isp_bnr_gcor_odd_wdata = uIspBnrGcorOddWdata;
    pstBeReg->ISP_BNR_GCOR_ODD_WDATA.u32 = o_isp_bnr_gcor_odd_wdata.u32;
}

static inline GK_VOID isp_bnr_gcor_odd_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBnrGcorOddRaddr)
{
    U_ISP_BNR_GCOR_ODD_RADDR o_isp_bnr_gcor_odd_raddr;
    o_isp_bnr_gcor_odd_raddr.bits.isp_bnr_gcor_odd_raddr = uIspBnrGcorOddRaddr;
    pstBeReg->ISP_BNR_GCOR_ODD_RADDR.u32 = o_isp_bnr_gcor_odd_raddr.u32;
}

static inline GK_VOID isp_mlsc_stt2lut_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspMlscStt2lutEn)
{
    U_ISP_MLSC_STT2LUT_CFG o_isp_mlsc_stt2lut_cfg;
    o_isp_mlsc_stt2lut_cfg.bits.isp_mlsc_stt2lut_en = uIspMlscStt2lutEn;
    pstBeReg->ISP_MLSC_STT2LUT_CFG.u32 = o_isp_mlsc_stt2lut_cfg.u32;
}

static inline GK_VOID isp_mlsc_stt2lut_regnew_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspMlscStt2lutRegnew)
{
    U_ISP_MLSC_STT2LUT_REGNEW o_isp_mlsc_stt2lut_regnew;
    o_isp_mlsc_stt2lut_regnew.bits.isp_mlsc_stt2lut_regnew = uIspMlscStt2lutRegnew;
    pstBeReg->ISP_MLSC_STT2LUT_REGNEW.u32 = o_isp_mlsc_stt2lut_regnew.u32;
}

static inline GK_VOID isp_mlsc_stt2lut_abn_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspMlscStt2lutClr)
{
    U_ISP_MLSC_STT2LUT_ABN o_isp_mlsc_stt2lut_abn;
    o_isp_mlsc_stt2lut_abn.bits.isp_mlsc_stt2lut_clr = uIspMlscStt2lutClr;
    pstBeReg->ISP_MLSC_STT2LUT_ABN.u32 = o_isp_mlsc_stt2lut_abn.u32;
}

static inline GK_VOID isp_mlsc_winnum_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspMlscNumh, GK_U32 uIspMlscNumv)
{
    U_ISP_MLSC_WINNUM o_isp_mlsc_winnum;
    o_isp_mlsc_winnum.bits.isp_mlsc_numh = uIspMlscNumh;
    o_isp_mlsc_winnum.bits.isp_mlsc_numv = uIspMlscNumv;
    pstBeReg->ISP_MLSC_WINNUM.u32 = o_isp_mlsc_winnum.u32;
}

static inline GK_VOID isp_mlsc_width_offset_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspMlscWidthOffset)
{
    U_ISP_MLSC_WIDTH_OFFSET o_isp_mlsc_width_offset;
    o_isp_mlsc_width_offset.bits.isp_mlsc_width_offset = uIspMlscWidthOffset;
    pstBeReg->ISP_MLSC_WIDTH_OFFSET.u32 = o_isp_mlsc_width_offset.u32;
}

static inline GK_VOID isp_mlsc_mesh_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspMlscMeshStr, GK_U32 uIspMlscMeshScale)
{
    U_ISP_MLSC_MESH o_isp_mlsc_mesh;
    o_isp_mlsc_mesh.bits.isp_mlsc_mesh_str = uIspMlscMeshStr;
    o_isp_mlsc_mesh.bits.isp_mlsc_mesh_scale = uIspMlscMeshScale;
    pstBeReg->ISP_MLSC_MESH.u32 = o_isp_mlsc_mesh.u32;
}

static inline GK_VOID isp_mlsc_rgain_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspMlscRgainWaddr)
{
    U_ISP_MLSC_RGAIN_WADDR o_isp_mlsc_rgain_waddr;
    o_isp_mlsc_rgain_waddr.bits.isp_mlsc_rgain_waddr = uIspMlscRgainWaddr;
    pstBeReg->ISP_MLSC_RGAIN_WADDR.u32 = o_isp_mlsc_rgain_waddr.u32;
}

static inline GK_VOID isp_mlsc_rgain_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspMlscRgainWdata)
{
    U_ISP_MLSC_RGAIN_WDATA o_isp_mlsc_rgain_wdata;
    o_isp_mlsc_rgain_wdata.bits.isp_mlsc_rgain_wdata = uIspMlscRgainWdata;
    pstBeReg->ISP_MLSC_RGAIN_WDATA.u32 = o_isp_mlsc_rgain_wdata.u32;
}

static inline GK_VOID isp_mlsc_rgain_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspMlscRgainRaddr)
{
    U_ISP_MLSC_RGAIN_RADDR o_isp_mlsc_rgain_raddr;
    o_isp_mlsc_rgain_raddr.bits.isp_mlsc_rgain_raddr = uIspMlscRgainRaddr;
    pstBeReg->ISP_MLSC_RGAIN_RADDR.u32 = o_isp_mlsc_rgain_raddr.u32;
}

static inline GK_VOID isp_mlsc_grgain_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspMlscGrgainWaddr)
{
    U_ISP_MLSC_GRGAIN_WADDR o_isp_mlsc_grgain_waddr;
    o_isp_mlsc_grgain_waddr.bits.isp_mlsc_grgain_waddr = uIspMlscGrgainWaddr;
    pstBeReg->ISP_MLSC_GRGAIN_WADDR.u32 = o_isp_mlsc_grgain_waddr.u32;
}

static inline GK_VOID isp_mlsc_grgain_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspMlscGrgainWdata)
{
    U_ISP_MLSC_GRGAIN_WDATA o_isp_mlsc_grgain_wdata;
    o_isp_mlsc_grgain_wdata.bits.isp_mlsc_grgain_wdata = uIspMlscGrgainWdata;
    pstBeReg->ISP_MLSC_GRGAIN_WDATA.u32 = o_isp_mlsc_grgain_wdata.u32;
}

static inline GK_VOID isp_mlsc_grgain_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspMlscGrgainRaddr)
{
    U_ISP_MLSC_GRGAIN_RADDR o_isp_mlsc_grgain_raddr;
    o_isp_mlsc_grgain_raddr.bits.isp_mlsc_grgain_raddr = uIspMlscGrgainRaddr;
    pstBeReg->ISP_MLSC_GRGAIN_RADDR.u32 = o_isp_mlsc_grgain_raddr.u32;
}

static inline GK_VOID isp_mlsc_bgain_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspMlscBgainWaddr)
{
    U_ISP_MLSC_BGAIN_WADDR o_isp_mlsc_bgain_waddr;
    o_isp_mlsc_bgain_waddr.bits.isp_mlsc_bgain_waddr = uIspMlscBgainWaddr;
    pstBeReg->ISP_MLSC_BGAIN_WADDR.u32 = o_isp_mlsc_bgain_waddr.u32;
}

static inline GK_VOID isp_mlsc_bgain_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspMlscBgainWdata)
{
    U_ISP_MLSC_BGAIN_WDATA o_isp_mlsc_bgain_wdata;
    o_isp_mlsc_bgain_wdata.bits.isp_mlsc_bgain_wdata = uIspMlscBgainWdata;
    pstBeReg->ISP_MLSC_BGAIN_WDATA.u32 = o_isp_mlsc_bgain_wdata.u32;
}

static inline GK_VOID isp_mlsc_bgain_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspMlscBgainRaddr)
{
    U_ISP_MLSC_BGAIN_RADDR o_isp_mlsc_bgain_raddr;
    o_isp_mlsc_bgain_raddr.bits.isp_mlsc_bgain_raddr = uIspMlscBgainRaddr;
    pstBeReg->ISP_MLSC_BGAIN_RADDR.u32 = o_isp_mlsc_bgain_raddr.u32;
}

static inline GK_VOID isp_mlsc_gbgain_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspMlscGbgainWaddr)
{
    U_ISP_MLSC_GBGAIN_WADDR o_isp_mlsc_gbgain_waddr;
    o_isp_mlsc_gbgain_waddr.bits.isp_mlsc_gbgain_waddr = uIspMlscGbgainWaddr;
    pstBeReg->ISP_MLSC_GBGAIN_WADDR.u32 = o_isp_mlsc_gbgain_waddr.u32;
}

static inline GK_VOID isp_mlsc_gbgain_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspMlscGbgainWdata)
{
    U_ISP_MLSC_GBGAIN_WDATA o_isp_mlsc_gbgain_wdata;
    o_isp_mlsc_gbgain_wdata.bits.isp_mlsc_gbgain_wdata = uIspMlscGbgainWdata;
    pstBeReg->ISP_MLSC_GBGAIN_WDATA.u32 = o_isp_mlsc_gbgain_wdata.u32;
}

static inline GK_VOID isp_mlsc_gbgain_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspMlscGbgainRaddr)
{
    U_ISP_MLSC_GBGAIN_RADDR o_isp_mlsc_gbgain_raddr;
    o_isp_mlsc_gbgain_raddr.bits.isp_mlsc_gbgain_raddr = uIspMlscGbgainRaddr;
    pstBeReg->ISP_MLSC_GBGAIN_RADDR.u32 = o_isp_mlsc_gbgain_raddr.u32;
}

static inline GK_VOID isp_wdr_ctrl_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrFusionDataMode, GK_U32 uIspWdrFusionmode, GK_U32 uIspWdrErosionEn, GK_U32 uIspWdrMdtEn, GK_U32 uIspWdrGrayscaleMode)
{
    U_ISP_WDR_CTRL o_isp_wdr_ctrl;
    o_isp_wdr_ctrl.bits.isp_wdr_fusion_data_mode = uIspWdrFusionDataMode;
    o_isp_wdr_ctrl.bits.isp_wdr_fusionmode = uIspWdrFusionmode;
    o_isp_wdr_ctrl.bits.isp_wdr_erosion_en = uIspWdrErosionEn;
    o_isp_wdr_ctrl.bits.isp_wdr_mdt_en = uIspWdrMdtEn;
    o_isp_wdr_ctrl.bits.isp_wdr_grayscale_mode = uIspWdrGrayscaleMode;
    pstBeReg->ISP_WDR_CTRL.u32 = o_isp_wdr_ctrl.u32;
}

static inline GK_VOID isp_wdr_f0_inblc0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrF0InblcGr, GK_U32 uIspWdrF0InblcR)
{
    U_ISP_WDR_F0_INBLC0 o_isp_wdr_f0_inblc0;
    o_isp_wdr_f0_inblc0.bits.isp_wdr_f0_inblc_gr = uIspWdrF0InblcGr;
    o_isp_wdr_f0_inblc0.bits.isp_wdr_f0_inblc_r = uIspWdrF0InblcR;
    pstBeReg->ISP_WDR_F0_INBLC0.u32 = o_isp_wdr_f0_inblc0.u32;
}

static inline GK_VOID isp_wdr_f0_inblc1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrF0InblcB, GK_U32 uIspWdrF0InblcGb)
{
    U_ISP_WDR_F0_INBLC1 o_isp_wdr_f0_inblc1;
    o_isp_wdr_f0_inblc1.bits.isp_wdr_f0_inblc_b = uIspWdrF0InblcB;
    o_isp_wdr_f0_inblc1.bits.isp_wdr_f0_inblc_gb = uIspWdrF0InblcGb;
    pstBeReg->ISP_WDR_F0_INBLC1.u32 = o_isp_wdr_f0_inblc1.u32;
}

static inline GK_VOID isp_wdr_f1_inblc0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrF1InblcGr, GK_U32 uIspWdrF1InblcR)
{
    U_ISP_WDR_F1_INBLC0 o_isp_wdr_f1_inblc0;
    o_isp_wdr_f1_inblc0.bits.isp_wdr_f1_inblc_gr = uIspWdrF1InblcGr;
    o_isp_wdr_f1_inblc0.bits.isp_wdr_f1_inblc_r = uIspWdrF1InblcR;
    pstBeReg->ISP_WDR_F1_INBLC0.u32 = o_isp_wdr_f1_inblc0.u32;
}

static inline GK_VOID isp_wdr_f1_inblc1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrF1InblcB, GK_U32 uIspWdrF1InblcGb)
{
    U_ISP_WDR_F1_INBLC1 o_isp_wdr_f1_inblc1;
    o_isp_wdr_f1_inblc1.bits.isp_wdr_f1_inblc_b = uIspWdrF1InblcB;
    o_isp_wdr_f1_inblc1.bits.isp_wdr_f1_inblc_gb = uIspWdrF1InblcGb;
    pstBeReg->ISP_WDR_F1_INBLC1.u32 = o_isp_wdr_f1_inblc1.u32;
}

static inline GK_VOID isp_wdr_outblc_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrOutblc, GK_U32 uIspWdrBsaveblc)
{
    U_ISP_WDR_OUTBLC o_isp_wdr_outblc;
    o_isp_wdr_outblc.bits.isp_wdr_outblc = uIspWdrOutblc;
    o_isp_wdr_outblc.bits.isp_wdr_bsaveblc = uIspWdrBsaveblc;
    pstBeReg->ISP_WDR_OUTBLC.u32 = o_isp_wdr_outblc.u32;
}

static inline GK_VOID isp_wdr_expovalue_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrExpovalue1, GK_U32 uIspWdrExpovalue0)
{
    U_ISP_WDR_EXPOVALUE o_isp_wdr_expovalue;
    o_isp_wdr_expovalue.bits.isp_wdr_expovalue1 = uIspWdrExpovalue1;
    o_isp_wdr_expovalue.bits.isp_wdr_expovalue0 = uIspWdrExpovalue0;
    pstBeReg->ISP_WDR_EXPOVALUE.u32 = o_isp_wdr_expovalue.u32;
}

static inline GK_VOID isp_wdr_exporratio_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrExporratio0)
{
    U_ISP_WDR_EXPORRATIO o_isp_wdr_exporratio;
    o_isp_wdr_exporratio.bits.isp_wdr_exporratio0 = uIspWdrExporratio0;
    pstBeReg->ISP_WDR_EXPORRATIO.u32 = o_isp_wdr_exporratio.u32;
}

static inline GK_VOID isp_wdr_blc_comp_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrBlcComp0)
{
    U_ISP_WDR_BLC_COMP o_isp_wdr_blc_comp;
    o_isp_wdr_blc_comp.bits.isp_wdr_blc_comp0 = uIspWdrBlcComp0;
    pstBeReg->ISP_WDR_BLC_COMP.u32 = o_isp_wdr_blc_comp.u32;
}

static inline GK_VOID isp_wdr_maxratio_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrMaxratio)
{
    U_ISP_WDR_MAXRATIO o_isp_wdr_maxratio;
    o_isp_wdr_maxratio.bits.isp_wdr_maxratio = uIspWdrMaxratio;
    pstBeReg->ISP_WDR_MAXRATIO.u32 = o_isp_wdr_maxratio.u32;
}

static inline GK_VOID isp_wdr_saturate_thr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrSaturateThr)
{
    U_ISP_WDR_SATURATE_THR o_isp_wdr_saturate_thr;
    o_isp_wdr_saturate_thr.bits.isp_wdr_saturate_thr = uIspWdrSaturateThr;
    pstBeReg->ISP_WDR_SATURATE_THR.u32 = o_isp_wdr_saturate_thr.u32;
}

static inline GK_VOID isp_wdr_still_thr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrF0StillThr)
{
    U_ISP_WDR_STILL_THR o_isp_wdr_still_thr;
    o_isp_wdr_still_thr.bits.isp_wdr_f0_still_thr = uIspWdrF0StillThr;
    pstBeReg->ISP_WDR_STILL_THR.u32 = o_isp_wdr_still_thr.u32;
}

static inline GK_VOID isp_wdr_mdtlbld_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrMdtlbld)
{
    U_ISP_WDR_MDTLBLD o_isp_wdr_mdtlbld;
    o_isp_wdr_mdtlbld.bits.isp_wdr_mdtlbld = uIspWdrMdtlbld;
    pstBeReg->ISP_WDR_MDTLBLD.u32 = o_isp_wdr_mdtlbld.u32;
}

static inline GK_VOID isp_wdr_mdt_thr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrMdtFullThr, GK_U32 uIspWdrMdtStillThr)
{
    U_ISP_WDR_MDT_THR o_isp_wdr_mdt_thr;
    o_isp_wdr_mdt_thr.bits.isp_wdr_mdt_full_thr = uIspWdrMdtFullThr;
    o_isp_wdr_mdt_thr.bits.isp_wdr_mdt_still_thr = uIspWdrMdtStillThr;
    pstBeReg->ISP_WDR_MDT_THR.u32 = o_isp_wdr_mdt_thr.u32;
}

static inline GK_VOID isp_wdr_forcelong_para_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrForcelongHighThd, GK_U32 uIspWdrForcelongLowThd, GK_U32 uIspWdrForcelongSmoothEn, GK_U32 uIspWdrForcelongEn)
{
    U_ISP_WDR_FORCELONG_PARA o_isp_wdr_forcelong_para;
    o_isp_wdr_forcelong_para.bits.isp_wdr_forcelonggh_thd = uIspWdrForcelongHighThd;
    o_isp_wdr_forcelong_para.bits.isp_wdr_forcelong_low_thd = uIspWdrForcelongLowThd;
    o_isp_wdr_forcelong_para.bits.isp_wdr_forcelong_smooth_en = uIspWdrForcelongSmoothEn;
    o_isp_wdr_forcelong_para.bits.isp_wdr_forcelong_en = uIspWdrForcelongEn;
    pstBeReg->ISP_WDR_FORCELONG_PARA.u32 = o_isp_wdr_forcelong_para.u32;
}

static inline GK_VOID isp_wdr_forcelong_slope_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrForcelongSlope)
{
    U_ISP_WDR_FORCELONG_SLOPE o_isp_wdr_forcelong_slope;
    o_isp_wdr_forcelong_slope.bits.isp_wdr_forcelong_slope = uIspWdrForcelongSlope;
    pstBeReg->ISP_WDR_FORCELONG_SLOPE.u32 = o_isp_wdr_forcelong_slope.u32;
}

static inline GK_VOID isp_wdr_mdt_nosf_thr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrMdtNosfHigThr, GK_U32 uIspWdrMdtNosfLowThr)
{
    U_ISP_WDR_MDT_NOSF_THR o_isp_wdr_mdt_nosf_thr;
    o_isp_wdr_mdt_nosf_thr.bits.isp_wdr_mdt_nosfg_thr = uIspWdrMdtNosfHigThr;
    o_isp_wdr_mdt_nosf_thr.bits.isp_wdr_mdt_nosf_low_thr = uIspWdrMdtNosfLowThr;
    pstBeReg->ISP_WDR_MDT_NOSF_THR.u32 = o_isp_wdr_mdt_nosf_thr.u32;
}

static inline GK_VOID isp_wdr_gain_sum_thr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrGainSumHigThr, GK_U32 uIspWdrGainSumLowThr)
{
    U_ISP_WDR_GAIN_SUM_THR o_isp_wdr_gain_sum_thr;
    o_isp_wdr_gain_sum_thr.bits.isp_wdr_gain_sumg_thr = uIspWdrGainSumHigThr;
    o_isp_wdr_gain_sum_thr.bits.isp_wdr_gain_sum_low_thr = uIspWdrGainSumLowThr;
    pstBeReg->ISP_WDR_GAIN_SUM_THR.u32 = o_isp_wdr_gain_sum_thr.u32;
}

static inline GK_VOID isp_wdr_shortexpo_chk_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrShortchkThd, GK_U32 uIspWdrShortexpoChk)
{
    U_ISP_WDR_SHORTEXPO_CHK o_isp_wdr_shortexpo_chk;
    o_isp_wdr_shortexpo_chk.bits.isp_wdr_shortchk_thd = uIspWdrShortchkThd;
    o_isp_wdr_shortexpo_chk.bits.isp_wdr_shortexpo_chk = uIspWdrShortexpoChk;
    pstBeReg->ISP_WDR_SHORTEXPO_CHK.u32 = o_isp_wdr_shortexpo_chk.u32;
}

static inline GK_VOID isp_wdr_pix_avg_diff_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrPixelAvgMaxDiff)
{
    U_ISP_WDR_PIX_AVG_DIFF o_isp_wdr_pix_avg_diff;
    o_isp_wdr_pix_avg_diff.bits.isp_wdr_pixel_avg_max_diff = uIspWdrPixelAvgMaxDiff;
    pstBeReg->ISP_WDR_PIX_AVG_DIFF.u32 = o_isp_wdr_pix_avg_diff.u32;
}

static inline GK_VOID isp_wdr_mask_similar_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrMaskSimilarCnt, GK_U32 uIspWdrMaskSimilarThr)
{
    U_ISP_WDR_MASK_SIMILAR o_isp_wdr_mask_similar;
    o_isp_wdr_mask_similar.bits.isp_wdr_mask_similar_cnt = uIspWdrMaskSimilarCnt;
    o_isp_wdr_mask_similar.bits.isp_wdr_mask_similar_thr = uIspWdrMaskSimilarThr;
    pstBeReg->ISP_WDR_MASK_SIMILAR.u32 = o_isp_wdr_mask_similar.u32;
}

static inline GK_VOID isp_wdr_wgtidx_blendratio_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrBldrlhfidx)
{
    U_ISP_WDR_WGTIDX_BLENDRATIO o_isp_wdr_wgtidx_blendratio;
    o_isp_wdr_wgtidx_blendratio.bits.isp_wdr_bldrlhfidx = uIspWdrBldrlhfidx;
    pstBeReg->ISP_WDR_WGTIDX_BLENDRATIO.u32 = o_isp_wdr_wgtidx_blendratio.u32;
}

static inline GK_VOID isp_wdr_wgtidx_thr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrLongThr, GK_U32 uIspWdrShortThr)
{
    U_ISP_WDR_WGTIDX_THR o_isp_wdr_wgtidx_thr;
    o_isp_wdr_wgtidx_thr.bits.isp_wdr_long_thr = uIspWdrLongThr;
    o_isp_wdr_wgtidx_thr.bits.isp_wdr_short_thr = uIspWdrShortThr;
    pstBeReg->ISP_WDR_WGTIDX_THR.u32 = o_isp_wdr_wgtidx_thr.u32;
}

static inline GK_VOID isp_wdr_dftwgt_fl_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrDftwgtFl)
{
    U_ISP_WDR_DFTWGT_FL o_isp_wdr_dftwgt_fl;
    o_isp_wdr_dftwgt_fl.bits.isp_wdr_dftwgt_fl = uIspWdrDftwgtFl;
    pstBeReg->ISP_WDR_DFTWGT_FL.u32 = o_isp_wdr_dftwgt_fl.u32;
}

static inline GK_VOID isp_wdr_wgt_slope_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrWgtSlope)
{
    U_ISP_WDR_WGT_SLOPE o_isp_wdr_wgt_slope;
    o_isp_wdr_wgt_slope.bits.isp_wdr_wgt_slope = uIspWdrWgtSlope;
    pstBeReg->ISP_WDR_WGT_SLOPE.u32 = o_isp_wdr_wgt_slope.u32;
}

static inline GK_VOID isp_wdr_fusion_thr_r_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrFusionF1ThrR, GK_U32 uIspWdrFusionF0ThrR)
{
    U_ISP_WDR_FUSION_THR_R o_isp_wdr_fusion_thr_r;
    o_isp_wdr_fusion_thr_r.bits.isp_wdr_fusion_f1_thr_r = uIspWdrFusionF1ThrR;
    o_isp_wdr_fusion_thr_r.bits.isp_wdr_fusion_f0_thr_r = uIspWdrFusionF0ThrR;
    pstBeReg->ISP_WDR_FUSION_THR_R.u32 = o_isp_wdr_fusion_thr_r.u32;
}

static inline GK_VOID isp_wdr_fusion_thr_b_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrFusionF1ThrB, GK_U32 uIspWdrFusionF0ThrB)
{
    U_ISP_WDR_FUSION_THR_B o_isp_wdr_fusion_thr_b;
    o_isp_wdr_fusion_thr_b.bits.isp_wdr_fusion_f1_thr_b = uIspWdrFusionF1ThrB;
    o_isp_wdr_fusion_thr_b.bits.isp_wdr_fusion_f0_thr_b = uIspWdrFusionF0ThrB;
    pstBeReg->ISP_WDR_FUSION_THR_B.u32 = o_isp_wdr_fusion_thr_b.u32;
}

static inline GK_VOID isp_wdr_fusion_thr_g_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrFusionF1ThrG, GK_U32 uIspWdrFusionF0ThrG)
{
    U_ISP_WDR_FUSION_THR_G o_isp_wdr_fusion_thr_g;
    o_isp_wdr_fusion_thr_g.bits.isp_wdr_fusion_f1_thr_g = uIspWdrFusionF1ThrG;
    o_isp_wdr_fusion_thr_g.bits.isp_wdr_fusion_f0_thr_g = uIspWdrFusionF0ThrG;
    pstBeReg->ISP_WDR_FUSION_THR_G.u32 = o_isp_wdr_fusion_thr_g.u32;
}

static inline GK_VOID isp_wdr_fusion_sat_thd_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspWdrFusionSaturateThd)
{
    U_ISP_WDR_FUSION_SAT_THD o_isp_wdr_fusion_sat_thd;
    o_isp_wdr_fusion_sat_thd.bits.isp_wdr_fusion_saturate_thd = uIspWdrFusionSaturateThd;
    pstBeReg->ISP_WDR_FUSION_SAT_THD.u32 = o_isp_wdr_fusion_sat_thd.u32;
}

static inline GK_VOID isp_dehaze_blk_size_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazeBlockSizev, GK_U32 uIspDehazeBlockSizeh)
{
    U_ISP_DEHAZE_BLK_SIZE o_isp_dehaze_blk_size;
    o_isp_dehaze_blk_size.bits.isp_dehaze_block_sizev = uIspDehazeBlockSizev;
    o_isp_dehaze_blk_size.bits.isp_dehaze_block_sizeh = uIspDehazeBlockSizeh;
    pstBeReg->ISP_DEHAZE_BLK_SIZE.u32 = o_isp_dehaze_blk_size.u32;
}

static inline GK_VOID isp_dehaze_blk_sum_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazeBlockSum)
{
    U_ISP_DEHAZE_BLK_SUM o_isp_dehaze_blk_sum;
    o_isp_dehaze_blk_sum.bits.isp_dehaze_block_sum = uIspDehazeBlockSum;
    pstBeReg->ISP_DEHAZE_BLK_SUM.u32 = o_isp_dehaze_blk_sum.u32;
}

static inline GK_VOID isp_dehaze_dc_size_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazeDcNumv, GK_U32 uIspDehazeDcNumh)
{
    U_ISP_DEHAZE_DC_SIZE o_isp_dehaze_dc_size;
    o_isp_dehaze_dc_size.bits.isp_dehaze_dc_numv = uIspDehazeDcNumv;
    o_isp_dehaze_dc_size.bits.isp_dehaze_dc_numh = uIspDehazeDcNumh;
    pstBeReg->ISP_DEHAZE_DC_SIZE.u32 = o_isp_dehaze_dc_size.u32;
}

static inline GK_VOID isp_dehaze_x_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazePhaseX)
{
    U_ISP_DEHAZE_X o_isp_dehaze_x;
    o_isp_dehaze_x.bits.isp_dehaze_phase_x = uIspDehazePhaseX;
    pstBeReg->ISP_DEHAZE_X.u32 = o_isp_dehaze_x.u32;
}

static inline GK_VOID isp_dehaze_y_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazePhaseY)
{
    U_ISP_DEHAZE_Y o_isp_dehaze_y;
    o_isp_dehaze_y.bits.isp_dehaze_phase_y = uIspDehazePhaseY;
    pstBeReg->ISP_DEHAZE_Y.u32 = o_isp_dehaze_y.u32;
}

static inline GK_VOID isp_dehaze_air_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazeAirB, GK_U32 uIspDehazeAirG, GK_U32 uIspDehazeAirR)
{
    U_ISP_DEHAZE_AIR o_isp_dehaze_air;
    o_isp_dehaze_air.bits.isp_dehaze_air_b = uIspDehazeAirB;
    o_isp_dehaze_air.bits.isp_dehaze_air_g = uIspDehazeAirG;
    o_isp_dehaze_air.bits.isp_dehaze_air_r = uIspDehazeAirR;
    pstBeReg->ISP_DEHAZE_AIR.u32 = o_isp_dehaze_air.u32;
}

static inline GK_VOID isp_dehaze_thld_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazeThldTb, GK_U32 uIspDehazeThldTg, GK_U32 uIspDehazeThldTr)
{
    U_ISP_DEHAZE_THLD o_isp_dehaze_thld;
    o_isp_dehaze_thld.bits.isp_dehaze_thld_tb = uIspDehazeThldTb;
    o_isp_dehaze_thld.bits.isp_dehaze_thld_tg = uIspDehazeThldTg;
    o_isp_dehaze_thld.bits.isp_dehaze_thld_tr = uIspDehazeThldTr;
    pstBeReg->ISP_DEHAZE_THLD.u32 = o_isp_dehaze_thld.u32;
}

static inline GK_VOID isp_dehaze_gstrth_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazeGstrth)
{
    U_ISP_DEHAZE_GSTRTH o_isp_dehaze_gstrth;
    o_isp_dehaze_gstrth.bits.isp_dehaze_gstrth = uIspDehazeGstrth;
    pstBeReg->ISP_DEHAZE_GSTRTH.u32 = o_isp_dehaze_gstrth.u32;
}

static inline GK_VOID isp_dehaze_blthld_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazeBlthld)
{
    U_ISP_DEHAZE_BLTHLD o_isp_dehaze_blthld;
    o_isp_dehaze_blthld.bits.isp_dehaze_blthld = uIspDehazeBlthld;
    pstBeReg->ISP_DEHAZE_BLTHLD.u32 = o_isp_dehaze_blthld.u32;
}

static inline GK_VOID isp_dehaze_stt_bst_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazeSttSize, GK_U32 uIspDehazeSttBst, GK_U32 uIspDehazeSttEn)
{
    U_ISP_DEHAZE_STT_BST o_isp_dehaze_stt_bst;
    o_isp_dehaze_stt_bst.bits.isp_dehaze_stt_size = uIspDehazeSttSize;
    o_isp_dehaze_stt_bst.bits.isp_dehaze_stt_bst = uIspDehazeSttBst;
    o_isp_dehaze_stt_bst.bits.isp_dehaze_stt_en = uIspDehazeSttEn;
    pstBeReg->ISP_DEHAZE_STT_BST.u32 = o_isp_dehaze_stt_bst.u32;
}

static inline GK_VOID isp_dehaze_stt_abn_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazeSttClr)
{
    U_ISP_DEHAZE_STT_ABN o_isp_dehaze_stt_abn;
    o_isp_dehaze_stt_abn.bits.isp_dehaze_stt_clr = uIspDehazeSttClr;
    pstBeReg->ISP_DEHAZE_STT_ABN.u32 = o_isp_dehaze_stt_abn.u32;
}

static inline GK_VOID isp_dehaze_stt2lut_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazeStt2lutEn)
{
    U_ISP_DEHAZE_STT2LUT_CFG o_isp_dehaze_stt2lut_cfg;
    o_isp_dehaze_stt2lut_cfg.bits.isp_dehaze_stt2lut_en = uIspDehazeStt2lutEn;
    pstBeReg->ISP_DEHAZE_STT2LUT_CFG.u32 = o_isp_dehaze_stt2lut_cfg.u32;
}

static inline GK_VOID isp_dehaze_stt2lut_regnew_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazeStt2lutRegnew)
{
    U_ISP_DEHAZE_STT2LUT_REGNEW o_isp_dehaze_stt2lut_regnew;
    o_isp_dehaze_stt2lut_regnew.bits.isp_dehaze_stt2lut_regnew = uIspDehazeStt2lutRegnew;
    pstBeReg->ISP_DEHAZE_STT2LUT_REGNEW.u32 = o_isp_dehaze_stt2lut_regnew.u32;
}

static inline GK_VOID isp_dehaze_stt2lut_abn_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazeStt2lutClr)
{
    U_ISP_DEHAZE_STT2LUT_ABN o_isp_dehaze_stt2lut_abn;
    o_isp_dehaze_stt2lut_abn.bits.isp_dehaze_stt2lut_clr = uIspDehazeStt2lutClr;
    pstBeReg->ISP_DEHAZE_STT2LUT_ABN.u32 = o_isp_dehaze_stt2lut_abn.u32;
}

static inline GK_VOID isp_dehaze_smlmapoffset_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazeSmlmapoffset)
{
    U_ISP_DEHAZE_SMLMAPOFFSET o_isp_dehaze_smlmapoffset;
    o_isp_dehaze_smlmapoffset.bits.isp_dehaze_smlmapoffset = uIspDehazeSmlmapoffset;
    pstBeReg->ISP_DEHAZE_SMLMAPOFFSET.u32 = o_isp_dehaze_smlmapoffset.u32;
}

static inline GK_VOID isp_dehaze_stat_point_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazeStatstartx, GK_U32 uIspDehazeStatendx)
{
    U_ISP_DEHAZE_STAT_POINT o_isp_dehaze_stat_point;
    o_isp_dehaze_stat_point.bits.isp_dehaze_statstartx = uIspDehazeStatstartx;
    o_isp_dehaze_stat_point.bits.isp_dehaze_statendx = uIspDehazeStatendx;
    pstBeReg->ISP_DEHAZE_STAT_POINT.u32 = o_isp_dehaze_stat_point.u32;
}

static inline GK_VOID isp_dehaze_stat_num_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazeStatNumv, GK_U32 uIspDehazeStatNumh)
{
    U_ISP_DEHAZE_STAT_NUM o_isp_dehaze_stat_num;
    o_isp_dehaze_stat_num.bits.isp_dehaze_stat_numv = uIspDehazeStatNumv;
    o_isp_dehaze_stat_num.bits.isp_dehaze_stat_numh = uIspDehazeStatNumh;
    pstBeReg->ISP_DEHAZE_STAT_NUM.u32 = o_isp_dehaze_stat_num.u32;
}

static inline GK_VOID isp_dehaze_minstat_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazeMinstatWaddr)
{
    U_ISP_DEHAZE_MINSTAT_WADDR o_isp_dehaze_minstat_waddr;
    o_isp_dehaze_minstat_waddr.bits.isp_dehaze_minstat_waddr = uIspDehazeMinstatWaddr;
    pstBeReg->ISP_DEHAZE_MINSTAT_WADDR.u32 = o_isp_dehaze_minstat_waddr.u32;
}

static inline GK_VOID isp_dehaze_minstat_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazeMinstatWdataL, GK_U32 uIspDehazeMinstatWdataH)
{
    U_ISP_DEHAZE_MINSTAT_WDATA o_isp_dehaze_minstat_wdata;
    o_isp_dehaze_minstat_wdata.bits.isp_dehaze_minstat_wdata_l = uIspDehazeMinstatWdataL;
    o_isp_dehaze_minstat_wdata.bits.isp_dehaze_minstat_wdata_h = uIspDehazeMinstatWdataH;
    pstBeReg->ISP_DEHAZE_MINSTAT_WDATA.u32 = o_isp_dehaze_minstat_wdata.u32;
}

static inline GK_VOID isp_dehaze_minstat_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazeMinstatRaddr)
{
    U_ISP_DEHAZE_MINSTAT_RADDR o_isp_dehaze_minstat_raddr;
    o_isp_dehaze_minstat_raddr.bits.isp_dehaze_minstat_raddr = uIspDehazeMinstatRaddr;
    pstBeReg->ISP_DEHAZE_MINSTAT_RADDR.u32 = o_isp_dehaze_minstat_raddr.u32;
}

static inline GK_VOID isp_dehaze_maxstat_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazeMaxstatWaddr)
{
    U_ISP_DEHAZE_MAXSTAT_WADDR o_isp_dehaze_maxstat_waddr;
    o_isp_dehaze_maxstat_waddr.bits.isp_dehaze_maxstat_waddr = uIspDehazeMaxstatWaddr;
    pstBeReg->ISP_DEHAZE_MAXSTAT_WADDR.u32 = o_isp_dehaze_maxstat_waddr.u32;
}

static inline GK_VOID isp_dehaze_maxstat_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazeMaxstatWdata)
{
    U_ISP_DEHAZE_MAXSTAT_WDATA o_isp_dehaze_maxstat_wdata;
    o_isp_dehaze_maxstat_wdata.bits.isp_dehaze_maxstat_wdata = uIspDehazeMaxstatWdata;
    pstBeReg->ISP_DEHAZE_MAXSTAT_WDATA.u32 = o_isp_dehaze_maxstat_wdata.u32;
}

static inline GK_VOID isp_dehaze_maxstat_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazeMaxstatRaddr)
{
    U_ISP_DEHAZE_MAXSTAT_RADDR o_isp_dehaze_maxstat_raddr;
    o_isp_dehaze_maxstat_raddr.bits.isp_dehaze_maxstat_raddr = uIspDehazeMaxstatRaddr;
    pstBeReg->ISP_DEHAZE_MAXSTAT_RADDR.u32 = o_isp_dehaze_maxstat_raddr.u32;
}

static inline GK_VOID isp_dehaze_prestat_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazePrestatWaddr)
{
    U_ISP_DEHAZE_PRESTAT_WADDR o_isp_dehaze_prestat_waddr;
    o_isp_dehaze_prestat_waddr.bits.isp_dehaze_prestat_waddr = uIspDehazePrestatWaddr;
    pstBeReg->ISP_DEHAZE_PRESTAT_WADDR.u32 = o_isp_dehaze_prestat_waddr.u32;
}

static inline GK_VOID isp_dehaze_prestat_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazePrestatWdata)
{
    U_ISP_DEHAZE_PRESTAT_WDATA o_isp_dehaze_prestat_wdata;
    o_isp_dehaze_prestat_wdata.bits.isp_dehaze_prestat_wdata = uIspDehazePrestatWdata;
    pstBeReg->ISP_DEHAZE_PRESTAT_WDATA.u32 = o_isp_dehaze_prestat_wdata.u32;
}

static inline GK_VOID isp_dehaze_prestat_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazePrestatRaddr)
{
    U_ISP_DEHAZE_PRESTAT_RADDR o_isp_dehaze_prestat_raddr;
    o_isp_dehaze_prestat_raddr.bits.isp_dehaze_prestat_raddr = uIspDehazePrestatRaddr;
    pstBeReg->ISP_DEHAZE_PRESTAT_RADDR.u32 = o_isp_dehaze_prestat_raddr.u32;
}

static inline GK_VOID isp_dehaze_lut_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazeLutWaddr)
{
    U_ISP_DEHAZE_LUT_WADDR o_isp_dehaze_lut_waddr;
    o_isp_dehaze_lut_waddr.bits.isp_dehaze_lut_waddr = uIspDehazeLutWaddr;
    pstBeReg->ISP_DEHAZE_LUT_WADDR.u32 = o_isp_dehaze_lut_waddr.u32;
}

static inline GK_VOID isp_dehaze_lut_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazeLutWdata)
{
    U_ISP_DEHAZE_LUT_WDATA o_isp_dehaze_lut_wdata;
    o_isp_dehaze_lut_wdata.bits.isp_dehaze_lut_wdata = uIspDehazeLutWdata;
    pstBeReg->ISP_DEHAZE_LUT_WDATA.u32 = o_isp_dehaze_lut_wdata.u32;
}

static inline GK_VOID isp_dehaze_lut_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDehazeLutRaddr)
{
    U_ISP_DEHAZE_LUT_RADDR o_isp_dehaze_lut_raddr;
    o_isp_dehaze_lut_raddr.bits.isp_dehaze_lut_raddr = uIspDehazeLutRaddr;
    pstBeReg->ISP_DEHAZE_LUT_RADDR.u32 = o_isp_dehaze_lut_raddr.u32;
}

static inline GK_VOID isp_expander_bitw_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspExpanderBitwOut, GK_U32 uIspExpanderBitwIn)
{
    U_ISP_EXPANDER_BITW o_isp_expander_bitw;
    o_isp_expander_bitw.bits.isp_expander_bitw_out = uIspExpanderBitwOut;
    o_isp_expander_bitw.bits.isp_expander_bitw_in = uIspExpanderBitwIn;
    pstBeReg->ISP_EXPANDER_BITW.u32 = o_isp_expander_bitw.u32;
}

static inline GK_VOID isp_expander_lut_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspExpanderLutWaddr)
{
    U_ISP_EXPANDER_LUT_WADDR o_isp_expander_lut_waddr;
    o_isp_expander_lut_waddr.bits.isp_expander_lut_waddr = uIspExpanderLutWaddr;
    pstBeReg->ISP_EXPANDER_LUT_WADDR.u32 = o_isp_expander_lut_waddr.u32;
}

static inline GK_VOID isp_expander_lut_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspExpanderLutWdata)
{
    U_ISP_EXPANDER_LUT_WDATA o_isp_expander_lut_wdata;
    o_isp_expander_lut_wdata.bits.isp_expander_lut_wdata = uIspExpanderLutWdata;
    pstBeReg->ISP_EXPANDER_LUT_WDATA.u32 = o_isp_expander_lut_wdata.u32;
}

static inline GK_VOID isp_expander_lut_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspExpanderLutRaddr)
{
    U_ISP_EXPANDER_LUT_RADDR o_isp_expander_lut_raddr;
    o_isp_expander_lut_raddr.bits.isp_expander_lut_raddr = uIspExpanderLutRaddr;
    pstBeReg->ISP_EXPANDER_LUT_RADDR.u32 = o_isp_expander_lut_raddr.u32;
}

static inline GK_VOID isp_gamma_pos0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspGammaPos0, GK_U32 uIspGammaPos1)
{
    U_ISP_GAMMA_POS0 o_isp_gamma_pos0;
    o_isp_gamma_pos0.bits.isp_gamma_pos_0 = uIspGammaPos0;
    o_isp_gamma_pos0.bits.isp_gamma_pos_1 = uIspGammaPos1;
    pstBeReg->ISP_GAMMA_POS0.u32 = o_isp_gamma_pos0.u32;
}

static inline GK_VOID isp_gamma_pos1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspGammaPos2, GK_U32 uIspGammaPos3)
{
    U_ISP_GAMMA_POS1 o_isp_gamma_pos1;
    o_isp_gamma_pos1.bits.isp_gamma_pos_2 = uIspGammaPos2;
    o_isp_gamma_pos1.bits.isp_gamma_pos_3 = uIspGammaPos3;
    pstBeReg->ISP_GAMMA_POS1.u32 = o_isp_gamma_pos1.u32;
}

static inline GK_VOID isp_gamma_pos2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspGammaPos4, GK_U32 uIspGammaPos5)
{
    U_ISP_GAMMA_POS2 o_isp_gamma_pos2;
    o_isp_gamma_pos2.bits.isp_gamma_pos_4 = uIspGammaPos4;
    o_isp_gamma_pos2.bits.isp_gamma_pos_5 = uIspGammaPos5;
    pstBeReg->ISP_GAMMA_POS2.u32 = o_isp_gamma_pos2.u32;
}

static inline GK_VOID isp_gamma_pos3_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspGammaPos6, GK_U32 uIspGammaPos7)
{
    U_ISP_GAMMA_POS3 o_isp_gamma_pos3;
    o_isp_gamma_pos3.bits.isp_gamma_pos_6 = uIspGammaPos6;
    o_isp_gamma_pos3.bits.isp_gamma_pos_7 = uIspGammaPos7;
    pstBeReg->ISP_GAMMA_POS3.u32 = o_isp_gamma_pos3.u32;
}

static inline GK_VOID isp_gamma_inseg0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspGammaInseg0, GK_U32 uIspGammaInseg1)
{
    U_ISP_GAMMA_INSEG0 o_isp_gamma_inseg0;
    o_isp_gamma_inseg0.bits.isp_gamma_inseg_0 = uIspGammaInseg0;
    o_isp_gamma_inseg0.bits.isp_gamma_inseg_1 = uIspGammaInseg1;
    pstBeReg->ISP_GAMMA_INSEG0.u32 = o_isp_gamma_inseg0.u32;
}

static inline GK_VOID isp_gamma_inseg1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspGammaInseg2, GK_U32 uIspGammaInseg3)
{
    U_ISP_GAMMA_INSEG1 o_isp_gamma_inseg1;
    o_isp_gamma_inseg1.bits.isp_gamma_inseg_2 = uIspGammaInseg2;
    o_isp_gamma_inseg1.bits.isp_gamma_inseg_3 = uIspGammaInseg3;
    pstBeReg->ISP_GAMMA_INSEG1.u32 = o_isp_gamma_inseg1.u32;
}

static inline GK_VOID isp_gamma_inseg2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspGammaInseg4, GK_U32 uIspGammaInseg5)
{
    U_ISP_GAMMA_INSEG2 o_isp_gamma_inseg2;
    o_isp_gamma_inseg2.bits.isp_gamma_inseg_4 = uIspGammaInseg4;
    o_isp_gamma_inseg2.bits.isp_gamma_inseg_5 = uIspGammaInseg5;
    pstBeReg->ISP_GAMMA_INSEG2.u32 = o_isp_gamma_inseg2.u32;
}

static inline GK_VOID isp_gamma_inseg3_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspGammaInseg6, GK_U32 uIspGammaInseg7)
{
    U_ISP_GAMMA_INSEG3 o_isp_gamma_inseg3;
    o_isp_gamma_inseg3.bits.isp_gamma_inseg_6 = uIspGammaInseg6;
    o_isp_gamma_inseg3.bits.isp_gamma_inseg_7 = uIspGammaInseg7;
    pstBeReg->ISP_GAMMA_INSEG3.u32 = o_isp_gamma_inseg3.u32;
}

static inline GK_VOID isp_gamma_step_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspGammaStep0, GK_U32 uIspGammaStep1, GK_U32 uIspGammaStep2, GK_U32 uIspGammaStep3, GK_U32 uIspGammaStep4, GK_U32 uIspGammaStep5, GK_U32 uIspGammaStep6, GK_U32 uIspGammaStep7)
{
    U_ISP_GAMMA_STEP o_isp_gamma_step;
    o_isp_gamma_step.bits.isp_gamma_step0 = uIspGammaStep0;
    o_isp_gamma_step.bits.isp_gamma_step1 = uIspGammaStep1;
    o_isp_gamma_step.bits.isp_gamma_step2 = uIspGammaStep2;
    o_isp_gamma_step.bits.isp_gamma_step3 = uIspGammaStep3;
    o_isp_gamma_step.bits.isp_gamma_step4 = uIspGammaStep4;
    o_isp_gamma_step.bits.isp_gamma_step5 = uIspGammaStep5;
    o_isp_gamma_step.bits.isp_gamma_step6 = uIspGammaStep6;
    o_isp_gamma_step.bits.isp_gamma_step7 = uIspGammaStep7;
    pstBeReg->ISP_GAMMA_STEP.u32 = o_isp_gamma_step.u32;
}

static inline GK_VOID isp_gamma_stt2lut_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspGammaStt2lutEn)
{
    U_ISP_GAMMA_STT2LUT_CFG o_isp_gamma_stt2lut_cfg;
    o_isp_gamma_stt2lut_cfg.bits.isp_gamma_stt2lut_en = uIspGammaStt2lutEn;
    pstBeReg->ISP_GAMMA_STT2LUT_CFG.u32 = o_isp_gamma_stt2lut_cfg.u32;
}

static inline GK_VOID isp_gamma_stt2lut_regnew_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspGammaStt2lutRegnew)
{
    U_ISP_GAMMA_STT2LUT_REGNEW o_isp_gamma_stt2lut_regnew;
    o_isp_gamma_stt2lut_regnew.bits.isp_gamma_stt2lut_regnew = uIspGammaStt2lutRegnew;
    pstBeReg->ISP_GAMMA_STT2LUT_REGNEW.u32 = o_isp_gamma_stt2lut_regnew.u32;
}

static inline GK_VOID isp_gamma_stt2lut_abn_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspGammaStt2lutClr)
{
    U_ISP_GAMMA_STT2LUT_ABN o_isp_gamma_stt2lut_abn;
    o_isp_gamma_stt2lut_abn.bits.isp_gamma_stt2lut_clr = uIspGammaStt2lutClr;
    pstBeReg->ISP_GAMMA_STT2LUT_ABN.u32 = o_isp_gamma_stt2lut_abn.u32;
}

static inline GK_VOID isp_gamma_lut_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspGammaLutWaddr)
{
    U_ISP_GAMMA_LUT_WADDR o_isp_gamma_lut_waddr;
    o_isp_gamma_lut_waddr.bits.isp_gamma_lut_waddr = uIspGammaLutWaddr;
    pstBeReg->ISP_GAMMA_LUT_WADDR.u32 = o_isp_gamma_lut_waddr.u32;
}

static inline GK_VOID isp_gamma_lut_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspGammaLutWdata)
{
    U_ISP_GAMMA_LUT_WDATA o_isp_gamma_lut_wdata;
    o_isp_gamma_lut_wdata.bits.isp_gamma_lut_wdata = uIspGammaLutWdata;
    pstBeReg->ISP_GAMMA_LUT_WDATA.u32 = o_isp_gamma_lut_wdata.u32;
}

static inline GK_VOID isp_gamma_lut_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspGammaLutRaddr)
{
    U_ISP_GAMMA_LUT_RADDR o_isp_gamma_lut_raddr;
    o_isp_gamma_lut_raddr.bits.isp_gamma_lut_raddr = uIspGammaLutRaddr;
    pstBeReg->ISP_GAMMA_LUT_RADDR.u32 = o_isp_gamma_lut_raddr.u32;
}

static inline GK_VOID isp_bcom_alpha_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBcomAlpha)
{
    U_ISP_BCOM_ALPHA o_isp_bcom_alpha;
    o_isp_bcom_alpha.bits.isp_bcom_alpha = uIspBcomAlpha;
    pstBeReg->ISP_BCOM_ALPHA.u32 = o_isp_bcom_alpha.u32;
}

static inline GK_VOID isp_bdec_alpha_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspBdecAlpha)
{
    U_ISP_BDEC_ALPHA o_isp_bdec_alpha;
    o_isp_bdec_alpha.bits.isp_bdec_alpha = uIspBdecAlpha;
    pstBeReg->ISP_BDEC_ALPHA.u32 = o_isp_bdec_alpha.u32;
}

static inline GK_VOID isp_ca_ctrl_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCaSatadjEn, GK_U32 uIspCaSkinprocEn, GK_U32 uIspCaLlhcprocEn)
{
    U_ISP_CA_CTRL o_isp_ca_ctrl;
    o_isp_ca_ctrl.bits.isp_ca_satadj_en = uIspCaSatadjEn;
    o_isp_ca_ctrl.bits.isp_ca_skinproc_en = uIspCaSkinprocEn;
    o_isp_ca_ctrl.bits.isp_ca_llhcproc_en = uIspCaLlhcprocEn;
    pstBeReg->ISP_CA_CTRL.u32 = o_isp_ca_ctrl.u32;
}

static inline GK_VOID isp_ca_lumath_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCaLumathHigh, GK_U32 uIspCaLumathLow)
{
    U_ISP_CA_LUMATH o_isp_ca_lumath;
    o_isp_ca_lumath.bits.isp_ca_lumathgh = uIspCaLumathHigh;
    o_isp_ca_lumath.bits.isp_ca_lumath_low = uIspCaLumathLow;
    pstBeReg->ISP_CA_LUMATH.u32 = o_isp_ca_lumath.u32;
}

static inline GK_VOID isp_ca_darkchroma_th_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCaDarkchromathHigh, GK_U32 uIspCaDarkchromathLow)
{
    U_ISP_CA_DARKCHROMA_TH o_isp_ca_darkchroma_th;
    o_isp_ca_darkchroma_th.bits.isp_ca_darkchromathgh = uIspCaDarkchromathHigh;
    o_isp_ca_darkchroma_th.bits.isp_ca_darkchromath_low = uIspCaDarkchromathLow;
    pstBeReg->ISP_CA_DARKCHROMA_TH.u32 = o_isp_ca_darkchroma_th.u32;
}

static inline GK_VOID isp_ca_sdarkchroma_th_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCaSdarkchromathHigh, GK_U32 uIspCaSdarkchromathLow)
{
    U_ISP_CA_SDARKCHROMA_TH o_isp_ca_sdarkchroma_th;
    o_isp_ca_sdarkchroma_th.bits.isp_ca_sdarkchromathgh = uIspCaSdarkchromathHigh;
    o_isp_ca_sdarkchroma_th.bits.isp_ca_sdarkchromath_low = uIspCaSdarkchromathLow;
    pstBeReg->ISP_CA_SDARKCHROMA_TH.u32 = o_isp_ca_sdarkchroma_th.u32;
}

static inline GK_VOID isp_ca_llhc_ratio_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCaLumaratioHigh, GK_U32 uIspCaLumaratioLow)
{
    U_ISP_CA_LLHC_RATIO o_isp_ca_llhc_ratio;
    o_isp_ca_llhc_ratio.bits.isp_ca_lumaratiogh = uIspCaLumaratioHigh;
    o_isp_ca_llhc_ratio.bits.isp_ca_lumaratio_low = uIspCaLumaratioLow;
    pstBeReg->ISP_CA_LLHC_RATIO.u32 = o_isp_ca_llhc_ratio.u32;
}

static inline GK_VOID isp_ca_isoratio_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCaIsoratio)
{
    U_ISP_CA_ISORATIO o_isp_ca_isoratio;
    o_isp_ca_isoratio.bits.isp_ca_isoratio = uIspCaIsoratio;
    pstBeReg->ISP_CA_ISORATIO.u32 = o_isp_ca_isoratio.u32;
}

static inline GK_VOID isp_ca_yraratio_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCaYraratio)
{
    U_ISP_CA_YRARATIO o_isp_ca_yraratio;
    o_isp_ca_yraratio.bits.isp_ca_yraratio = uIspCaYraratio;
    pstBeReg->ISP_CA_YRARATIO.u32 = o_isp_ca_yraratio.u32;
}

static inline GK_VOID isp_ca_skinlluma_uth_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCaSkinllumaUmax, GK_U32 uIspCaSkinllumaUmin)
{
    U_ISP_CA_SKINLLUMA_UTH o_isp_ca_skinlluma_uth;
    o_isp_ca_skinlluma_uth.bits.isp_ca_skinlluma_umax = uIspCaSkinllumaUmax;
    o_isp_ca_skinlluma_uth.bits.isp_ca_skinlluma_umin = uIspCaSkinllumaUmin;
    pstBeReg->ISP_CA_SKINLLUMA_UTH.u32 = o_isp_ca_skinlluma_uth.u32;
}

static inline GK_VOID isp_ca_skinlluma_uyth_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCaSkinllumaUymax, GK_U32 uIspCaSkinllumaUymin)
{
    U_ISP_CA_SKINLLUMA_UYTH o_isp_ca_skinlluma_uyth;
    o_isp_ca_skinlluma_uyth.bits.isp_ca_skinlluma_uymax = uIspCaSkinllumaUymax;
    o_isp_ca_skinlluma_uyth.bits.isp_ca_skinlluma_uymin = uIspCaSkinllumaUymin;
    pstBeReg->ISP_CA_SKINLLUMA_UYTH.u32 = o_isp_ca_skinlluma_uyth.u32;
}

static inline GK_VOID isp_ca_skinhluma_uth_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCaSkinhlumaUmax, GK_U32 uIspCaSkinhlumaUmin)
{
    U_ISP_CA_SKINHLUMA_UTH o_isp_ca_skinhluma_uth;
    o_isp_ca_skinhluma_uth.bits.isp_ca_skinhluma_umax = uIspCaSkinhlumaUmax;
    o_isp_ca_skinhluma_uth.bits.isp_ca_skinhluma_umin = uIspCaSkinhlumaUmin;
    pstBeReg->ISP_CA_SKINHLUMA_UTH.u32 = o_isp_ca_skinhluma_uth.u32;
}

static inline GK_VOID isp_ca_skinhluma_uyth_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCaSkinhlumaUymax, GK_U32 uIspCaSkinhlumaUymin)
{
    U_ISP_CA_SKINHLUMA_UYTH o_isp_ca_skinhluma_uyth;
    o_isp_ca_skinhluma_uyth.bits.isp_ca_skinhluma_uymax = uIspCaSkinhlumaUymax;
    o_isp_ca_skinhluma_uyth.bits.isp_ca_skinhluma_uymin = uIspCaSkinhlumaUymin;
    pstBeReg->ISP_CA_SKINHLUMA_UYTH.u32 = o_isp_ca_skinhluma_uyth.u32;
}

static inline GK_VOID isp_ca_skinlluma_vth_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCaSkinllumaVmax, GK_U32 uIspCaSkinllumaVmin)
{
    U_ISP_CA_SKINLLUMA_VTH o_isp_ca_skinlluma_vth;
    o_isp_ca_skinlluma_vth.bits.isp_ca_skinlluma_vmax = uIspCaSkinllumaVmax;
    o_isp_ca_skinlluma_vth.bits.isp_ca_skinlluma_vmin = uIspCaSkinllumaVmin;
    pstBeReg->ISP_CA_SKINLLUMA_VTH.u32 = o_isp_ca_skinlluma_vth.u32;
}

static inline GK_VOID isp_ca_skinlluma_vyth_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCaSkinllumaVymax, GK_U32 uIspCaSkinllumaVymin)
{
    U_ISP_CA_SKINLLUMA_VYTH o_isp_ca_skinlluma_vyth;
    o_isp_ca_skinlluma_vyth.bits.isp_ca_skinlluma_vymax = uIspCaSkinllumaVymax;
    o_isp_ca_skinlluma_vyth.bits.isp_ca_skinlluma_vymin = uIspCaSkinllumaVymin;
    pstBeReg->ISP_CA_SKINLLUMA_VYTH.u32 = o_isp_ca_skinlluma_vyth.u32;
}

static inline GK_VOID isp_ca_skinhluma_vth_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCaSkinhlumaVmax, GK_U32 uIspCaSkinhlumaVmin)
{
    U_ISP_CA_SKINHLUMA_VTH o_isp_ca_skinhluma_vth;
    o_isp_ca_skinhluma_vth.bits.isp_ca_skinhluma_vmax = uIspCaSkinhlumaVmax;
    o_isp_ca_skinhluma_vth.bits.isp_ca_skinhluma_vmin = uIspCaSkinhlumaVmin;
    pstBeReg->ISP_CA_SKINHLUMA_VTH.u32 = o_isp_ca_skinhluma_vth.u32;
}

static inline GK_VOID isp_ca_skinhluma_vyth_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCaSkinhlumaVymax, GK_U32 uIspCaSkinhlumaVymin)
{
    U_ISP_CA_SKINHLUMA_VYTH o_isp_ca_skinhluma_vyth;
    o_isp_ca_skinhluma_vyth.bits.isp_ca_skinhluma_vymax = uIspCaSkinhlumaVymax;
    o_isp_ca_skinhluma_vyth.bits.isp_ca_skinhluma_vymin = uIspCaSkinhlumaVymin;
    pstBeReg->ISP_CA_SKINHLUMA_VYTH.u32 = o_isp_ca_skinhluma_vyth.u32;
}

static inline GK_VOID isp_ca_skin_uvdiff_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCaSkinUvdiff)
{
    U_ISP_CA_SKIN_UVDIFF o_isp_ca_skin_uvdiff;
    o_isp_ca_skin_uvdiff.bits.isp_ca_skin_uvdiff = uIspCaSkinUvdiff;
    pstBeReg->ISP_CA_SKIN_UVDIFF.u32 = o_isp_ca_skin_uvdiff.u32;
}

static inline GK_VOID isp_ca_skin_ratioth0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCaSkinratiothMid, GK_U32 uIspCaSkinratiothLow)
{
    U_ISP_CA_SKIN_RATIOTH0 o_isp_ca_skin_ratioth0;
    o_isp_ca_skin_ratioth0.bits.isp_ca_skinratioth_mid = uIspCaSkinratiothMid;
    o_isp_ca_skin_ratioth0.bits.isp_ca_skinratioth_low = uIspCaSkinratiothLow;
    pstBeReg->ISP_CA_SKIN_RATIOTH0.u32 = o_isp_ca_skin_ratioth0.u32;
}

static inline GK_VOID isp_ca_skin_ratioth1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCaSkinratiothHigh)
{
    U_ISP_CA_SKIN_RATIOTH1 o_isp_ca_skin_ratioth1;
    o_isp_ca_skin_ratioth1.bits.isp_ca_skinratiothgh = uIspCaSkinratiothHigh;
    pstBeReg->ISP_CA_SKIN_RATIOTH1.u32 = o_isp_ca_skin_ratioth1.u32;
}

static inline GK_VOID isp_ca_skin_betaratio_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCaSkinBetaratio)
{
    U_ISP_CA_SKIN_BETARATIO o_isp_ca_skin_betaratio;
    o_isp_ca_skin_betaratio.bits.isp_ca_skin_betaratio = uIspCaSkinBetaratio;
    pstBeReg->ISP_CA_SKIN_BETARATIO.u32 = o_isp_ca_skin_betaratio.u32;
}

static inline GK_VOID isp_ca_lut_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCaLutWaddr)
{
    U_ISP_CA_LUT_WADDR o_isp_ca_lut_waddr;
    o_isp_ca_lut_waddr.bits.isp_ca_lut_waddr = uIspCaLutWaddr;
    pstBeReg->ISP_CA_LUT_WADDR.u32 = o_isp_ca_lut_waddr.u32;
}

static inline GK_VOID isp_ca_lut_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCaLutWdata)
{
    U_ISP_CA_LUT_WDATA o_isp_ca_lut_wdata;
    o_isp_ca_lut_wdata.bits.isp_ca_lut_wdata = uIspCaLutWdata;
    pstBeReg->ISP_CA_LUT_WDATA.u32 = o_isp_ca_lut_wdata.u32;
}

static inline GK_VOID isp_ca_lut_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCaLutRaddr)
{
    U_ISP_CA_LUT_RADDR o_isp_ca_lut_raddr;
    o_isp_ca_lut_raddr.bits.isp_ca_lut_raddr = uIspCaLutRaddr;
    pstBeReg->ISP_CA_LUT_RADDR.u32 = o_isp_ca_lut_raddr.u32;
}

static inline GK_VOID isp_ca_stt2lut_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCaStt2lutEn)
{
    U_ISP_CA_STT2LUT_CFG o_isp_ca_stt2lut_cfg;
    o_isp_ca_stt2lut_cfg.bits.isp_ca_stt2lut_en = uIspCaStt2lutEn;
    pstBeReg->ISP_CA_STT2LUT_CFG.u32 = o_isp_ca_stt2lut_cfg.u32;
}

static inline GK_VOID isp_ca_stt2lut_regnew_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCaStt2lutRegnew)
{
    U_ISP_CA_STT2LUT_REGNEW o_isp_ca_stt2lut_regnew;
    o_isp_ca_stt2lut_regnew.bits.isp_ca_stt2lut_regnew = uIspCaStt2lutRegnew;
    pstBeReg->ISP_CA_STT2LUT_REGNEW.u32 = o_isp_ca_stt2lut_regnew.u32;
}

static inline GK_VOID isp_ca_stt2lut_abn_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspCaStt2lutClr)
{
    U_ISP_CA_STT2LUT_ABN o_isp_ca_stt2lut_abn;
    o_isp_ca_stt2lut_abn.bits.isp_ca_stt2lut_clr = uIspCaStt2lutClr;
    pstBeReg->ISP_CA_STT2LUT_ABN.u32 = o_isp_ca_stt2lut_abn.u32;
}

static inline GK_VOID isp_ldci_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciCalcEn, GK_U32 uIspLdciWrstatEn, GK_U32 uIspLdciRdstatEn)
{
    U_ISP_LDCI_CFG o_isp_ldci_cfg;
    o_isp_ldci_cfg.bits.isp_ldci_calc_en = uIspLdciCalcEn;
    o_isp_ldci_cfg.bits.isp_ldci_wrstat_en = uIspLdciWrstatEn;
    o_isp_ldci_cfg.bits.isp_ldci_rdstat_en = uIspLdciRdstatEn;
    pstBeReg->ISP_LDCI_CFG.u32 = o_isp_ldci_cfg.u32;
}

static inline GK_VOID isp_ldci_calc_small_offset_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciCalcMapOffsetx)
{
    U_ISP_LDCI_CALC_SMALL_OFFSET o_isp_ldci_calc_small_offset;
    o_isp_ldci_calc_small_offset.bits.isp_ldci_calc_map_offsetx = uIspLdciCalcMapOffsetx;
    pstBeReg->ISP_LDCI_CALC_SMALL_OFFSET.u32 = o_isp_ldci_calc_small_offset.u32;
}

static inline GK_VOID isp_ldci_zone_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciSmlmapstride, GK_U32 uIspLdciSmlmapheight, GK_U32 uIspLdciTotalZone)
{
    U_ISP_LDCI_ZONE o_isp_ldci_zone;
    o_isp_ldci_zone.bits.isp_ldci_smlmapstride = uIspLdciSmlmapstride;
    o_isp_ldci_zone.bits.isp_ldci_smlmapheight = uIspLdciSmlmapheight;
    o_isp_ldci_zone.bits.isp_ldci_total_zone = uIspLdciTotalZone;
    pstBeReg->ISP_LDCI_ZONE.u32 = o_isp_ldci_zone.u32;
}

static inline GK_VOID isp_ldci_lpfstt_bst_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciLpfsttSize, GK_U32 uIspLdciLpfsttBst, GK_U32 uIspLdciLpfsttEn)
{
    U_ISP_LDCI_LPFSTT_BST o_isp_ldci_lpfstt_bst;
    o_isp_ldci_lpfstt_bst.bits.isp_ldci_lpfstt_size = uIspLdciLpfsttSize;
    o_isp_ldci_lpfstt_bst.bits.isp_ldci_lpfstt_bst = uIspLdciLpfsttBst;
    o_isp_ldci_lpfstt_bst.bits.isp_ldci_lpfstt_en = uIspLdciLpfsttEn;
    pstBeReg->ISP_LDCI_LPFSTT_BST.u32 = o_isp_ldci_lpfstt_bst.u32;
}

static inline GK_VOID isp_ldci_lpfstt_abn_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciLpfsttClr)
{
    U_ISP_LDCI_LPFSTT_ABN o_isp_ldci_lpfstt_abn;
    o_isp_ldci_lpfstt_abn.bits.isp_ldci_lpfstt_clr = uIspLdciLpfsttClr;
    pstBeReg->ISP_LDCI_LPFSTT_ABN.u32 = o_isp_ldci_lpfstt_abn.u32;
}

static inline GK_VOID isp_ldci_scale_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciScalex, GK_U32 uIspLdciScaley)
{
    U_ISP_LDCI_SCALE o_isp_ldci_scale;
    o_isp_ldci_scale.bits.isp_ldci_scalex = uIspLdciScalex;
    o_isp_ldci_scale.bits.isp_ldci_scaley = uIspLdciScaley;
    pstBeReg->ISP_LDCI_SCALE.u32 = o_isp_ldci_scale.u32;
}

static inline GK_VOID isp_ldci_luma_sel_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciLumaSel)
{
    U_ISP_LDCI_LUMA_SEL o_isp_ldci_luma_sel;
    o_isp_ldci_luma_sel.bits.isp_ldci_luma_sel = uIspLdciLumaSel;
    pstBeReg->ISP_LDCI_LUMA_SEL.u32 = o_isp_ldci_luma_sel.u32;
}

static inline GK_VOID isp_ldci_blc_ctrl_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciBlcCtrl)
{
    U_ISP_LDCI_BLC_CTRL o_isp_ldci_blc_ctrl;
    o_isp_ldci_blc_ctrl.bits.isp_ldci_blc_ctrl = uIspLdciBlcCtrl;
    pstBeReg->ISP_LDCI_BLC_CTRL.u32 = o_isp_ldci_blc_ctrl.u32;
}

static inline GK_VOID isp_ldci_lpf_lpfcoef0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciLpfcoef0, GK_U32 uIspLdciLpfcoef1)
{
    U_ISP_LDCI_LPF_LPFCOEF0 o_isp_ldci_lpf_lpfcoef0;
    o_isp_ldci_lpf_lpfcoef0.bits.isp_ldci_lpfcoef0 = uIspLdciLpfcoef0;
    o_isp_ldci_lpf_lpfcoef0.bits.isp_ldci_lpfcoef1 = uIspLdciLpfcoef1;
    pstBeReg->ISP_LDCI_LPF_LPFCOEF0.u32 = o_isp_ldci_lpf_lpfcoef0.u32;
}

static inline GK_VOID isp_ldci_lpf_lpfcoef1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciLpfcoef2, GK_U32 uIspLdciLpfcoef3)
{
    U_ISP_LDCI_LPF_LPFCOEF1 o_isp_ldci_lpf_lpfcoef1;
    o_isp_ldci_lpf_lpfcoef1.bits.isp_ldci_lpfcoef2 = uIspLdciLpfcoef2;
    o_isp_ldci_lpf_lpfcoef1.bits.isp_ldci_lpfcoef3 = uIspLdciLpfcoef3;
    pstBeReg->ISP_LDCI_LPF_LPFCOEF1.u32 = o_isp_ldci_lpf_lpfcoef1.u32;
}

static inline GK_VOID isp_ldci_lpf_lpfcoef2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciLpfcoef4)
{
    U_ISP_LDCI_LPF_LPFCOEF2 o_isp_ldci_lpf_lpfcoef2;
    o_isp_ldci_lpf_lpfcoef2.bits.isp_ldci_lpfcoef4 = uIspLdciLpfcoef4;
    pstBeReg->ISP_LDCI_LPF_LPFCOEF2.u32 = o_isp_ldci_lpf_lpfcoef2.u32;
}

static inline GK_VOID isp_ldci_lpf_lpfsft_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciLpfsft)
{
    U_ISP_LDCI_LPF_LPFSFT o_isp_ldci_lpf_lpfsft;
    o_isp_ldci_lpf_lpfsft.bits.isp_ldci_lpfsft = uIspLdciLpfsft;
    pstBeReg->ISP_LDCI_LPF_LPFSFT.u32 = o_isp_ldci_lpf_lpfsft.u32;
}

static inline GK_VOID isp_ldci_he_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciHeWaddr)
{
    U_ISP_LDCI_HE_WADDR o_isp_ldci_he_waddr;
    o_isp_ldci_he_waddr.bits.isp_ldci_he_waddr = uIspLdciHeWaddr;
    pstBeReg->ISP_LDCI_HE_WADDR.u32 = o_isp_ldci_he_waddr.u32;
}

static inline GK_VOID isp_ldci_he_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciHeposWdata, GK_U32 uIspLdciHenegWdata)
{
    U_ISP_LDCI_HE_WDATA o_isp_ldci_he_wdata;
    o_isp_ldci_he_wdata.bits.isp_ldci_hepos_wdata = uIspLdciHeposWdata;
    o_isp_ldci_he_wdata.bits.isp_ldci_heneg_wdata = uIspLdciHenegWdata;
    pstBeReg->ISP_LDCI_HE_WDATA.u32 = o_isp_ldci_he_wdata.u32;
}

static inline GK_VOID isp_ldci_he_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciHeRaddr)
{
    U_ISP_LDCI_HE_RADDR o_isp_ldci_he_raddr;
    o_isp_ldci_he_raddr.bits.isp_ldci_he_raddr = uIspLdciHeRaddr;
    pstBeReg->ISP_LDCI_HE_RADDR.u32 = o_isp_ldci_he_raddr.u32;
}

static inline GK_VOID isp_ldci_he_rdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciHeposRdata, GK_U32 uIspLdciHenegRdata)
{
    U_ISP_LDCI_HE_RDATA o_isp_ldci_he_rdata;
    o_isp_ldci_he_rdata.bits.isp_ldci_hepos_rdata = uIspLdciHeposRdata;
    o_isp_ldci_he_rdata.bits.isp_ldci_heneg_rdata = uIspLdciHenegRdata;
    pstBeReg->ISP_LDCI_HE_RDATA.u32 = o_isp_ldci_he_rdata.u32;
}

static inline GK_VOID isp_ldci_de_usm_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciDeUsmWaddr)
{
    U_ISP_LDCI_DE_USM_WADDR o_isp_ldci_de_usm_waddr;
    o_isp_ldci_de_usm_waddr.bits.isp_ldci_de_usm_waddr = uIspLdciDeUsmWaddr;
    pstBeReg->ISP_LDCI_DE_USM_WADDR.u32 = o_isp_ldci_de_usm_waddr.u32;
}

static inline GK_VOID isp_ldci_de_usm_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciUsmposWdata, GK_U32 uIspLdciUsmnegWdata)
{
    U_ISP_LDCI_DE_USM_WDATA o_isp_ldci_de_usm_wdata;
    o_isp_ldci_de_usm_wdata.bits.isp_ldci_usmpos_wdata = uIspLdciUsmposWdata;
    o_isp_ldci_de_usm_wdata.bits.isp_ldci_usmneg_wdata = uIspLdciUsmnegWdata;
    pstBeReg->ISP_LDCI_DE_USM_WDATA.u32 = o_isp_ldci_de_usm_wdata.u32;
}

static inline GK_VOID isp_ldci_de_usm_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciDeUsmRaddr)
{
    U_ISP_LDCI_DE_USM_RADDR o_isp_ldci_de_usm_raddr;
    o_isp_ldci_de_usm_raddr.bits.isp_ldci_de_usm_raddr = uIspLdciDeUsmRaddr;
    pstBeReg->ISP_LDCI_DE_USM_RADDR.u32 = o_isp_ldci_de_usm_raddr.u32;
}

static inline GK_VOID isp_ldci_de_usm_rdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciUsmposRdata, GK_U32 uIspLdciUsmnegRdata)
{
    U_ISP_LDCI_DE_USM_RDATA o_isp_ldci_de_usm_rdata;
    o_isp_ldci_de_usm_rdata.bits.isp_ldci_usmpos_rdata = uIspLdciUsmposRdata;
    o_isp_ldci_de_usm_rdata.bits.isp_ldci_usmneg_rdata = uIspLdciUsmnegRdata;
    pstBeReg->ISP_LDCI_DE_USM_RDATA.u32 = o_isp_ldci_de_usm_rdata.u32;
}

static inline GK_VOID isp_ldci_cgain_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciCgainWaddr)
{
    U_ISP_LDCI_CGAIN_WADDR o_isp_ldci_cgain_waddr;
    o_isp_ldci_cgain_waddr.bits.isp_ldci_cgain_waddr = uIspLdciCgainWaddr;
    pstBeReg->ISP_LDCI_CGAIN_WADDR.u32 = o_isp_ldci_cgain_waddr.u32;
}

static inline GK_VOID isp_ldci_cgain_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciCgainWdata)
{
    U_ISP_LDCI_CGAIN_WDATA o_isp_ldci_cgain_wdata;
    o_isp_ldci_cgain_wdata.bits.isp_ldci_cgain_wdata = uIspLdciCgainWdata;
    pstBeReg->ISP_LDCI_CGAIN_WDATA.u32 = o_isp_ldci_cgain_wdata.u32;
}

static inline GK_VOID isp_ldci_cgain_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciCgainRaddr)
{
    U_ISP_LDCI_CGAIN_RADDR o_isp_ldci_cgain_raddr;
    o_isp_ldci_cgain_raddr.bits.isp_ldci_cgain_raddr = uIspLdciCgainRaddr;
    pstBeReg->ISP_LDCI_CGAIN_RADDR.u32 = o_isp_ldci_cgain_raddr.u32;
}

static inline GK_VOID isp_ldci_cgain_rdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciCgainRdata)
{
    U_ISP_LDCI_CGAIN_RDATA o_isp_ldci_cgain_rdata;
    o_isp_ldci_cgain_rdata.bits.isp_ldci_cgain_rdata = uIspLdciCgainRdata;
    pstBeReg->ISP_LDCI_CGAIN_RDATA.u32 = o_isp_ldci_cgain_rdata.u32;
}

static inline GK_VOID isp_ldci_stat_hpos_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciHstart, GK_U32 uIspLdciHend)
{
    U_ISP_LDCI_STAT_HPOS o_isp_ldci_stat_hpos;
    o_isp_ldci_stat_hpos.bits.isp_ldci_hstart = uIspLdciHstart;
    o_isp_ldci_stat_hpos.bits.isp_ldci_hend = uIspLdciHend;
    pstBeReg->ISP_LDCI_STAT_HPOS.u32 = o_isp_ldci_stat_hpos.u32;
}

static inline GK_VOID isp_ldci_stat_vpos_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciVstart, GK_U32 uIspLdciVend)
{
    U_ISP_LDCI_STAT_VPOS o_isp_ldci_stat_vpos;
    o_isp_ldci_stat_vpos.bits.isp_ldci_vstart = uIspLdciVstart;
    o_isp_ldci_stat_vpos.bits.isp_ldci_vend = uIspLdciVend;
    pstBeReg->ISP_LDCI_STAT_VPOS.u32 = o_isp_ldci_stat_vpos.u32;
}

static inline GK_VOID isp_ldci_stat_evratio_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciStatEvratio)
{
    U_ISP_LDCI_STAT_EVRATIO o_isp_ldci_stat_evratio;
    o_isp_ldci_stat_evratio.bits.isp_ldci_stat_evratio = uIspLdciStatEvratio;
    pstBeReg->ISP_LDCI_STAT_EVRATIO.u32 = o_isp_ldci_stat_evratio.u32;
}

static inline GK_VOID isp_ldci_stat_zone_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciStatSmlmapwidth, GK_U32 uIspLdciStatSmlmapheight, GK_U32 uIspLdciStatTotalZone)
{
    U_ISP_LDCI_STAT_ZONE o_isp_ldci_stat_zone;
    o_isp_ldci_stat_zone.bits.isp_ldci_stat_smlmapwidth = uIspLdciStatSmlmapwidth;
    o_isp_ldci_stat_zone.bits.isp_ldci_stat_smlmapheight = uIspLdciStatSmlmapheight;
    o_isp_ldci_stat_zone.bits.isp_ldci_stat_total_zone = uIspLdciStatTotalZone;
    pstBeReg->ISP_LDCI_STAT_ZONE.u32 = o_isp_ldci_stat_zone.u32;
}

static inline GK_VOID isp_ldci_blk_smlmapwidth0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciBlkSmlmapwidth0)
{
    U_ISP_LDCI_BLK_SMLMAPWIDTH0 o_isp_ldci_blk_smlmapwidth0;
    o_isp_ldci_blk_smlmapwidth0.bits.isp_ldci_blk_smlmapwidth0 = uIspLdciBlkSmlmapwidth0;
    pstBeReg->ISP_LDCI_BLK_SMLMAPWIDTH0.u32 = o_isp_ldci_blk_smlmapwidth0.u32;
}

static inline GK_VOID isp_ldci_blk_smlmapwidth1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciBlkSmlmapwidth1)
{
    U_ISP_LDCI_BLK_SMLMAPWIDTH1 o_isp_ldci_blk_smlmapwidth1;
    o_isp_ldci_blk_smlmapwidth1.bits.isp_ldci_blk_smlmapwidth1 = uIspLdciBlkSmlmapwidth1;
    pstBeReg->ISP_LDCI_BLK_SMLMAPWIDTH1.u32 = o_isp_ldci_blk_smlmapwidth1.u32;
}

static inline GK_VOID isp_ldci_blk_smlmapwidth2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciBlkSmlmapwidth2)
{
    U_ISP_LDCI_BLK_SMLMAPWIDTH2 o_isp_ldci_blk_smlmapwidth2;
    o_isp_ldci_blk_smlmapwidth2.bits.isp_ldci_blk_smlmapwidth2 = uIspLdciBlkSmlmapwidth2;
    pstBeReg->ISP_LDCI_BLK_SMLMAPWIDTH2.u32 = o_isp_ldci_blk_smlmapwidth2.u32;
}

static inline GK_VOID isp_ldci_chrdamp_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciChrposdamp, GK_U32 uIspLdciChrnegdamp)
{
    U_ISP_LDCI_CHRDAMP o_isp_ldci_chrdamp;
    o_isp_ldci_chrdamp.bits.isp_ldci_chrposdamp = uIspLdciChrposdamp;
    o_isp_ldci_chrdamp.bits.isp_ldci_chrnegdamp = uIspLdciChrnegdamp;
    pstBeReg->ISP_LDCI_CHRDAMP.u32 = o_isp_ldci_chrdamp.u32;
}

static inline GK_VOID isp_ldci_stt2lut_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciStt2lutEn)
{
    U_ISP_LDCI_STT2LUT_CFG o_isp_ldci_stt2lut_cfg;
    o_isp_ldci_stt2lut_cfg.bits.isp_ldci_stt2lut_en = uIspLdciStt2lutEn;
    pstBeReg->ISP_LDCI_STT2LUT_CFG.u32 = o_isp_ldci_stt2lut_cfg.u32;
}

static inline GK_VOID isp_ldci_stt2lut_regnew_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciStt2lutRegnew)
{
    U_ISP_LDCI_STT2LUT_REGNEW o_isp_ldci_stt2lut_regnew;
    o_isp_ldci_stt2lut_regnew.bits.isp_ldci_stt2lut_regnew = uIspLdciStt2lutRegnew;
    pstBeReg->ISP_LDCI_STT2LUT_REGNEW.u32 = o_isp_ldci_stt2lut_regnew.u32;
}

static inline GK_VOID isp_ldci_stt2lut_abn_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciStt2lutClr)
{
    U_ISP_LDCI_STT2LUT_ABN o_isp_ldci_stt2lut_abn;
    o_isp_ldci_stt2lut_abn.bits.isp_ldci_stt2lut_clr = uIspLdciStt2lutClr;
    pstBeReg->ISP_LDCI_STT2LUT_ABN.u32 = o_isp_ldci_stt2lut_abn.u32;
}

static inline GK_VOID isp_ldci_lpf_map_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspLdciLpfmapRaddr)
{
    U_ISP_LDCI_LPF_MAP_RADDR o_isp_ldci_lpf_map_raddr;
    o_isp_ldci_lpf_map_raddr.bits.isp_ldci_lpfmap_raddr = uIspLdciLpfmapRaddr;
    pstBeReg->ISP_LDCI_LPF_MAP_RADDR.u32 = o_isp_ldci_lpf_map_raddr.u32;
}

static inline GK_VOID isp_drc_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcWrstatEn, GK_U32 uIspDrcRdstatEn, GK_U32 uIspDrcVbifltEn, GK_U32 uIspDrcColorCorrEn, GK_U32 uIspDrcMonoChromaEn, GK_U32 uIspDrcPregammaEn, GK_U32 uIspDrcDpDetEn)
{
    U_ISP_DRC_CFG o_isp_drc_cfg;
    o_isp_drc_cfg.bits.isp_drc_wrstat_en = uIspDrcWrstatEn;
    o_isp_drc_cfg.bits.isp_drc_rdstat_en = uIspDrcRdstatEn;
    o_isp_drc_cfg.bits.isp_drc_vbiflt_en = uIspDrcVbifltEn;
    o_isp_drc_cfg.bits.isp_drc_color_corr_en = uIspDrcColorCorrEn;
    o_isp_drc_cfg.bits.isp_drc_mono_chroma_en = uIspDrcMonoChromaEn;
    o_isp_drc_cfg.bits.isp_drc_pregamma_en = uIspDrcPregammaEn;
    o_isp_drc_cfg.bits.isp_drc_dp_det_en = uIspDrcDpDetEn;
    pstBeReg->ISP_DRC_CFG.u32 = o_isp_drc_cfg.u32;
}

static inline GK_VOID isp_drc_lut_update0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcLutUpdate0)
{
    U_ISP_DRC_LUT_UPDATE0 o_isp_drc_lut_update0;
    o_isp_drc_lut_update0.bits.isp_drc_lut_update0 = uIspDrcLutUpdate0;
    pstBeReg->ISP_DRC_LUT_UPDATE0.u32 = o_isp_drc_lut_update0.u32;
}

static inline GK_VOID isp_drc_lut_update1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcLutUpdate1)
{
    U_ISP_DRC_LUT_UPDATE1 o_isp_drc_lut_update1;
    o_isp_drc_lut_update1.bits.isp_drc_lut_update1 = uIspDrcLutUpdate1;
    pstBeReg->ISP_DRC_LUT_UPDATE1.u32 = o_isp_drc_lut_update1.u32;
}

static inline GK_VOID isp_drc_zone_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcHnum, GK_U32 uIspDrcVnum)
{
    U_ISP_DRC_ZONE o_isp_drc_zone;
    o_isp_drc_zone.bits.isp_drc_hnum = uIspDrcHnum;
    o_isp_drc_zone.bits.isp_drc_vnum = uIspDrcVnum;
    pstBeReg->ISP_DRC_ZONE.u32 = o_isp_drc_zone.u32;
}

static inline GK_VOID isp_drc_zone_size_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcZoneHsize, GK_U32 uIspDrcChkX, GK_U32 uIspDrcZoneVsize, GK_U32 uIspDrcChkY)
{
    U_ISP_DRC_ZONE_SIZE o_isp_drc_zone_size;
    o_isp_drc_zone_size.bits.isp_drc_zone_hsize = uIspDrcZoneHsize;
    o_isp_drc_zone_size.bits.isp_drc_chk_x = uIspDrcChkX;
    o_isp_drc_zone_size.bits.isp_drc_zone_vsize = uIspDrcZoneVsize;
    o_isp_drc_zone_size.bits.isp_drc_chk_y = uIspDrcChkY;
    pstBeReg->ISP_DRC_ZONE_SIZE.u32 = o_isp_drc_zone_size.u32;
}

static inline GK_VOID isp_drc_zone_div0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcDivX0, GK_U32 uIspDrcDivY0)
{
    U_ISP_DRC_ZONE_DIV0 o_isp_drc_zone_div0;
    o_isp_drc_zone_div0.bits.isp_drc_div_x0 = uIspDrcDivX0;
    o_isp_drc_zone_div0.bits.isp_drc_div_y0 = uIspDrcDivY0;
    pstBeReg->ISP_DRC_ZONE_DIV0.u32 = o_isp_drc_zone_div0.u32;
}

static inline GK_VOID isp_drc_zone_div1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcDivX1, GK_U32 uIspDrcDivY1)
{
    U_ISP_DRC_ZONE_DIV1 o_isp_drc_zone_div1;
    o_isp_drc_zone_div1.bits.isp_drc_div_x1 = uIspDrcDivX1;
    o_isp_drc_zone_div1.bits.isp_drc_div_y1 = uIspDrcDivY1;
    pstBeReg->ISP_DRC_ZONE_DIV1.u32 = o_isp_drc_zone_div1.u32;
}

static inline GK_VOID isp_drc_bin_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcBin)
{
    U_ISP_DRC_BIN o_isp_drc_bin;
    o_isp_drc_bin.bits.isp_drc_bin = uIspDrcBin;
    pstBeReg->ISP_DRC_BIN.u32 = o_isp_drc_bin.u32;
}

static inline GK_VOID isp_drc_zone_init_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcAccXInit, GK_U32 uIspDrcCntXInit, GK_U32 uIspDrcIdxXInit, GK_U32 uIspDrcBigXInit)
{
    U_ISP_DRC_ZONE_INIT o_isp_drc_zone_init;
    o_isp_drc_zone_init.bits.isp_drc_acc_x_init = uIspDrcAccXInit;
    o_isp_drc_zone_init.bits.isp_drc_cnt_x_init = uIspDrcCntXInit;
    o_isp_drc_zone_init.bits.isp_drc_idx_x_init = uIspDrcIdxXInit;
    o_isp_drc_zone_init.bits.isp_drc_big_x_init = uIspDrcBigXInit;
    pstBeReg->ISP_DRC_ZONE_INIT.u32 = o_isp_drc_zone_init.u32;
}

static inline GK_VOID isp_drc_bin_scale_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcBinScale)
{
    U_ISP_DRC_BIN_SCALE o_isp_drc_bin_scale;
    o_isp_drc_bin_scale.bits.isp_drc_bin_scale = uIspDrcBinScale;
    pstBeReg->ISP_DRC_BIN_SCALE.u32 = o_isp_drc_bin_scale.u32;
}

static inline GK_VOID isp_drc_strength_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcStrength)
{
    U_ISP_DRC_STRENGTH o_isp_drc_strength;
    o_isp_drc_strength.bits.isp_drc_strength = uIspDrcStrength;
    pstBeReg->ISP_DRC_STRENGTH.u32 = o_isp_drc_strength.u32;
}

static inline GK_VOID isp_drc_stat_width_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcStatWidth, GK_U32 uIspDrcTotalWidth)
{
    U_ISP_DRC_STAT_WIDTH o_isp_drc_stat_width;
    o_isp_drc_stat_width.bits.isp_drc_stat_width = uIspDrcStatWidth;
    o_isp_drc_stat_width.bits.isp_drc_total_width = uIspDrcTotalWidth;
    pstBeReg->ISP_DRC_STAT_WIDTH.u32 = o_isp_drc_stat_width.u32;
}

static inline GK_VOID isp_drc_dark_gain_lmt_y_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcVal1Y, GK_U32 uIspDrcSft1Y, GK_U32 uIspDrcVal2Y, GK_U32 uIspDrcSft2Y)
{
    U_ISP_DRC_DARK_GAIN_LMT_Y o_isp_drc_dark_gain_lmt_y;
    o_isp_drc_dark_gain_lmt_y.bits.isp_drc_val1_y = uIspDrcVal1Y;
    o_isp_drc_dark_gain_lmt_y.bits.isp_drc_sft1_y = uIspDrcSft1Y;
    o_isp_drc_dark_gain_lmt_y.bits.isp_drc_val2_y = uIspDrcVal2Y;
    o_isp_drc_dark_gain_lmt_y.bits.isp_drc_sft2_y = uIspDrcSft2Y;
    pstBeReg->ISP_DRC_DARK_GAIN_LMT_Y.u32 = o_isp_drc_dark_gain_lmt_y.u32;
}

static inline GK_VOID isp_drc_dark_gain_lmt_c_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcVal1C, GK_U32 uIspDrcSft1C, GK_U32 uIspDrcVal2C, GK_U32 uIspDrcSft2C)
{
    U_ISP_DRC_DARK_GAIN_LMT_C o_isp_drc_dark_gain_lmt_c;
    o_isp_drc_dark_gain_lmt_c.bits.isp_drc_val1_c = uIspDrcVal1C;
    o_isp_drc_dark_gain_lmt_c.bits.isp_drc_sft1_c = uIspDrcSft1C;
    o_isp_drc_dark_gain_lmt_c.bits.isp_drc_val2_c = uIspDrcVal2C;
    o_isp_drc_dark_gain_lmt_c.bits.isp_drc_sft2_c = uIspDrcSft2C;
    pstBeReg->ISP_DRC_DARK_GAIN_LMT_C.u32 = o_isp_drc_dark_gain_lmt_c.u32;
}

static inline GK_VOID isp_drc_bright_gain_lmt_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcVal, GK_U32 uIspDrcSft)
{
    U_ISP_DRC_BRIGHT_GAIN_LMT o_isp_drc_bright_gain_lmt;
    o_isp_drc_bright_gain_lmt.bits.isp_drc_val = uIspDrcVal;
    o_isp_drc_bright_gain_lmt.bits.isp_drc_sft = uIspDrcSft;
    pstBeReg->ISP_DRC_BRIGHT_GAIN_LMT.u32 = o_isp_drc_bright_gain_lmt.u32;
}

static inline GK_VOID isp_drc_rgb_wgt_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcRWgt, GK_U32 uIspDrcGWgt, GK_U32 uIspDrcBWgt)
{
    U_ISP_DRC_RGB_WGT o_isp_drc_rgb_wgt;
    o_isp_drc_rgb_wgt.bits.isp_drc_r_wgt = uIspDrcRWgt;
    o_isp_drc_rgb_wgt.bits.isp_drc_g_wgt = uIspDrcGWgt;
    o_isp_drc_rgb_wgt.bits.isp_drc_b_wgt = uIspDrcBWgt;
    pstBeReg->ISP_DRC_RGB_WGT.u32 = o_isp_drc_rgb_wgt.u32;
}

static inline GK_VOID isp_drc_dp_dtc_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcDpDetThrmin, GK_U32 uIspDrcDpDetThrslo, GK_U32 uIspDrcDpDetG2rb, GK_U32 uIspDrcDpDetRb2rb, GK_U32 uIspDrcDpDetReplctr, GK_U32 uIspDrcDpDetRngrto)
{
    U_ISP_DRC_DP_DTC o_isp_drc_dp_dtc;
    o_isp_drc_dp_dtc.bits.isp_drc_dp_det_thrmin = uIspDrcDpDetThrmin;
    o_isp_drc_dp_dtc.bits.isp_drc_dp_det_thrslo = uIspDrcDpDetThrslo;
    o_isp_drc_dp_dtc.bits.isp_drc_dp_det_g2rb = uIspDrcDpDetG2rb;
    o_isp_drc_dp_dtc.bits.isp_drc_dp_det_rb2rb = uIspDrcDpDetRb2rb;
    o_isp_drc_dp_dtc.bits.isp_drc_dp_det_replctr = uIspDrcDpDetReplctr;
    o_isp_drc_dp_dtc.bits.isp_drc_dp_det_rngrto = uIspDrcDpDetRngrto;
    pstBeReg->ISP_DRC_DP_DTC.u32 = o_isp_drc_dp_dtc.u32;
}

static inline GK_VOID isp_drc_gain_clip_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcGainClipStep, GK_U32 uIspDrcGainClipKnee)
{
    U_ISP_DRC_GAIN_CLIP o_isp_drc_gain_clip;
    o_isp_drc_gain_clip.bits.isp_drc_gain_clip_step = uIspDrcGainClipStep;
    o_isp_drc_gain_clip.bits.isp_drc_gain_clip_knee = uIspDrcGainClipKnee;
    pstBeReg->ISP_DRC_GAIN_CLIP.u32 = o_isp_drc_gain_clip.u32;
}

static inline GK_VOID isp_drc_color_ctrl_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcCcLutCtrl)
{
    U_ISP_DRC_COLOR_CTRL o_isp_drc_color_ctrl;
    o_isp_drc_color_ctrl.bits.isp_drc_cc_lut_ctrl = uIspDrcCcLutCtrl;
    pstBeReg->ISP_DRC_COLOR_CTRL.u32 = o_isp_drc_color_ctrl.u32;
}

static inline GK_VOID isp_drc_global_corr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcCcGlobalCorr)
{
    U_ISP_DRC_GLOBAL_CORR o_isp_drc_global_corr;
    o_isp_drc_global_corr.bits.isp_drc_cc_global_corr = uIspDrcCcGlobalCorr;
    pstBeReg->ISP_DRC_GLOBAL_CORR.u32 = o_isp_drc_global_corr.u32;
}

static inline GK_VOID isp_drc_mixing_coring_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcMixingCoring)
{
    U_ISP_DRC_MIXING_CORING o_isp_drc_mixing_coring;
    o_isp_drc_mixing_coring.bits.isp_drc_mixing_coring = uIspDrcMixingCoring;
    pstBeReg->ISP_DRC_MIXING_CORING.u32 = o_isp_drc_mixing_coring.u32;
}

static inline GK_VOID isp_drc_mixing_dark_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcDarkMin, GK_U32 uIspDrcDarkMax, GK_U32 uIspDrcDarkThr, GK_U32 uIspDrcDarkSlo)
{
    U_ISP_DRC_MIXING_DARK o_isp_drc_mixing_dark;
    o_isp_drc_mixing_dark.bits.isp_drc_dark_min = uIspDrcDarkMin;
    o_isp_drc_mixing_dark.bits.isp_drc_dark_max = uIspDrcDarkMax;
    o_isp_drc_mixing_dark.bits.isp_drc_dark_thr = uIspDrcDarkThr;
    o_isp_drc_mixing_dark.bits.isp_drc_dark_slo = uIspDrcDarkSlo;
    pstBeReg->ISP_DRC_MIXING_DARK.u32 = o_isp_drc_mixing_dark.u32;
}

static inline GK_VOID isp_drc_mixing_bright_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcBrightMin, GK_U32 uIspDrcBrightMax, GK_U32 uIspDrcBrightThr, GK_U32 uIspDrcBrightSlo)
{
    U_ISP_DRC_MIXING_BRIGHT o_isp_drc_mixing_bright;
    o_isp_drc_mixing_bright.bits.isp_drc_bright_min = uIspDrcBrightMin;
    o_isp_drc_mixing_bright.bits.isp_drc_bright_max = uIspDrcBrightMax;
    o_isp_drc_mixing_bright.bits.isp_drc_bright_thr = uIspDrcBrightThr;
    o_isp_drc_mixing_bright.bits.isp_drc_bright_slo = uIspDrcBrightSlo;
    pstBeReg->ISP_DRC_MIXING_BRIGHT.u32 = o_isp_drc_mixing_bright.u32;
}

static inline GK_VOID isp_drc_tmlut0_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcTmlut0Waddr)
{
    U_ISP_DRC_TMLUT0_WADDR o_isp_drc_tmlut0_waddr;
    o_isp_drc_tmlut0_waddr.bits.isp_drc_tmlut0_waddr = uIspDrcTmlut0Waddr;
    pstBeReg->ISP_DRC_TMLUT0_WADDR.u32 = o_isp_drc_tmlut0_waddr.u32;
}

static inline GK_VOID isp_drc_tmlut0_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcTmlut0Wdata)
{
    U_ISP_DRC_TMLUT0_WDATA o_isp_drc_tmlut0_wdata;
    o_isp_drc_tmlut0_wdata.bits.isp_drc_tmlut0_wdata = uIspDrcTmlut0Wdata;
    pstBeReg->ISP_DRC_TMLUT0_WDATA.u32 = o_isp_drc_tmlut0_wdata.u32;
}

static inline GK_VOID isp_drc_tmlut0_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcTmlut0Raddr)
{
    U_ISP_DRC_TMLUT0_RADDR o_isp_drc_tmlut0_raddr;
    o_isp_drc_tmlut0_raddr.bits.isp_drc_tmlut0_raddr = uIspDrcTmlut0Raddr;
    pstBeReg->ISP_DRC_TMLUT0_RADDR.u32 = o_isp_drc_tmlut0_raddr.u32;
}

static inline GK_VOID isp_drc_cclut_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcCclutWaddr)
{
    U_ISP_DRC_CCLUT_WADDR o_isp_drc_cclut_waddr;
    o_isp_drc_cclut_waddr.bits.isp_drc_cclut_waddr = uIspDrcCclutWaddr;
    pstBeReg->ISP_DRC_CCLUT_WADDR.u32 = o_isp_drc_cclut_waddr.u32;
}

static inline GK_VOID isp_drc_cclut_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcCclutWdata)
{
    U_ISP_DRC_CCLUT_WDATA o_isp_drc_cclut_wdata;
    o_isp_drc_cclut_wdata.bits.isp_drc_cclut_wdata = uIspDrcCclutWdata;
    pstBeReg->ISP_DRC_CCLUT_WDATA.u32 = o_isp_drc_cclut_wdata.u32;
}

static inline GK_VOID isp_drc_cclut_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcCclutRaddr)
{
    U_ISP_DRC_CCLUT_RADDR o_isp_drc_cclut_raddr;
    o_isp_drc_cclut_raddr.bits.isp_drc_cclut_raddr = uIspDrcCclutRaddr;
    pstBeReg->ISP_DRC_CCLUT_RADDR.u32 = o_isp_drc_cclut_raddr.u32;
}

static inline GK_VOID isp_drc_pregamma_waddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcPregammaWaddr)
{
    U_ISP_DRC_PREGAMMA_WADDR o_isp_drc_pregamma_waddr;
    o_isp_drc_pregamma_waddr.bits.isp_drc_pregamma_waddr = uIspDrcPregammaWaddr;
    pstBeReg->ISP_DRC_PREGAMMA_WADDR.u32 = o_isp_drc_pregamma_waddr.u32;
}

static inline GK_VOID isp_drc_pregamma_wdata_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcPregammaWdata)
{
    U_ISP_DRC_PREGAMMA_WDATA o_isp_drc_pregamma_wdata;
    o_isp_drc_pregamma_wdata.bits.isp_drc_pregamma_wdata = uIspDrcPregammaWdata;
    pstBeReg->ISP_DRC_PREGAMMA_WDATA.u32 = o_isp_drc_pregamma_wdata.u32;
}

static inline GK_VOID isp_drc_pregamma_raddr_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcPregammaRaddr)
{
    U_ISP_DRC_PREGAMMA_RADDR o_isp_drc_pregamma_raddr;
    o_isp_drc_pregamma_raddr.bits.isp_drc_pregamma_raddr = uIspDrcPregammaRaddr;
    pstBeReg->ISP_DRC_PREGAMMA_RADDR.u32 = o_isp_drc_pregamma_raddr.u32;
}

static inline GK_VOID isp_drc_grad_rev_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcGradRevThres, GK_U32 uIspDrcGradRevMax, GK_U32 uIspDrcGradRevSlope, GK_U32 uIspDrcGradRevShift)
{
    U_ISP_DRC_GRAD_REV o_isp_drc_grad_rev;
    o_isp_drc_grad_rev.bits.isp_drc_grad_rev_thres = uIspDrcGradRevThres;
    o_isp_drc_grad_rev.bits.isp_drc_grad_rev_max = uIspDrcGradRevMax;
    o_isp_drc_grad_rev.bits.isp_drc_grad_rev_slope = uIspDrcGradRevSlope;
    o_isp_drc_grad_rev.bits.isp_drc_grad_rev_shift = uIspDrcGradRevShift;
    pstBeReg->ISP_DRC_GRAD_REV.u32 = o_isp_drc_grad_rev.u32;
}

static inline GK_VOID isp_drc_vbi_strength_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcVarRngFine, GK_U32 uIspDrcVarRngMedium, GK_U32 uIspDrcVarRngCoarse, GK_U32 uIspDrcVarSpaFine, GK_U32 uIspDrcVarSpaMedium, GK_U32 uIspDrcVarSpaCoarse)
{
    U_ISP_DRC_VBI_STRENGTH o_isp_drc_vbi_strength;
    o_isp_drc_vbi_strength.bits.isp_drc_var_rng_fine = uIspDrcVarRngFine;
    o_isp_drc_vbi_strength.bits.isp_drc_var_rng_medium = uIspDrcVarRngMedium;
    o_isp_drc_vbi_strength.bits.isp_drc_var_rng_coarse = uIspDrcVarRngCoarse;
    o_isp_drc_vbi_strength.bits.isp_drc_var_spa_fine = uIspDrcVarSpaFine;
    o_isp_drc_vbi_strength.bits.isp_drc_var_spa_medium = uIspDrcVarSpaMedium;
    o_isp_drc_vbi_strength.bits.isp_drc_var_spa_coarse = uIspDrcVarSpaCoarse;
    pstBeReg->ISP_DRC_VBI_STRENGTH.u32 = o_isp_drc_vbi_strength.u32;
}

static inline GK_VOID isp_drc_rng_ada_coef_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcFrAdaMax, GK_U32 uIspDrcDisOffsetCoef, GK_U32 uIspDrcThrCoefLow, GK_U32 uIspDrcThrCoefHigh)
{
    U_ISP_DRC_RNG_ADA_COEF o_isp_drc_rng_ada_coef;
    o_isp_drc_rng_ada_coef.bits.isp_drc_fr_ada_max = uIspDrcFrAdaMax;
    o_isp_drc_rng_ada_coef.bits.isp_drc_dis_offset_coef = uIspDrcDisOffsetCoef;
    o_isp_drc_rng_ada_coef.bits.isp_drc_thr_coef_low = uIspDrcThrCoefLow;
    o_isp_drc_rng_ada_coef.bits.isp_drc_thr_coefgh = uIspDrcThrCoefHigh;
    pstBeReg->ISP_DRC_RNG_ADA_COEF.u32 = o_isp_drc_rng_ada_coef.u32;
}

static inline GK_VOID isp_drc_prev_luma_0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcPrevLuma0)
{
    U_ISP_DRC_PREV_LUMA_0 o_isp_drc_prev_luma_0;
    o_isp_drc_prev_luma_0.bits.isp_drc_prev_luma_0 = uIspDrcPrevLuma0;
    pstBeReg->ISP_DRC_PREV_LUMA_0.u32 = o_isp_drc_prev_luma_0.u32;
}

static inline GK_VOID isp_drc_prev_luma_1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcPrevLuma1)
{
    U_ISP_DRC_PREV_LUMA_1 o_isp_drc_prev_luma_1;
    o_isp_drc_prev_luma_1.bits.isp_drc_prev_luma_1 = uIspDrcPrevLuma1;
    pstBeReg->ISP_DRC_PREV_LUMA_1.u32 = o_isp_drc_prev_luma_1.u32;
}

static inline GK_VOID isp_drc_prev_luma_2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcPrevLuma2)
{
    U_ISP_DRC_PREV_LUMA_2 o_isp_drc_prev_luma_2;
    o_isp_drc_prev_luma_2.bits.isp_drc_prev_luma_2 = uIspDrcPrevLuma2;
    pstBeReg->ISP_DRC_PREV_LUMA_2.u32 = o_isp_drc_prev_luma_2.u32;
}

static inline GK_VOID isp_drc_prev_luma_3_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcPrevLuma3)
{
    U_ISP_DRC_PREV_LUMA_3 o_isp_drc_prev_luma_3;
    o_isp_drc_prev_luma_3.bits.isp_drc_prev_luma_3 = uIspDrcPrevLuma3;
    pstBeReg->ISP_DRC_PREV_LUMA_3.u32 = o_isp_drc_prev_luma_3.u32;
}

static inline GK_VOID isp_drc_prev_luma_4_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcPrevLuma4)
{
    U_ISP_DRC_PREV_LUMA_4 o_isp_drc_prev_luma_4;
    o_isp_drc_prev_luma_4.bits.isp_drc_prev_luma_4 = uIspDrcPrevLuma4;
    pstBeReg->ISP_DRC_PREV_LUMA_4.u32 = o_isp_drc_prev_luma_4.u32;
}

static inline GK_VOID isp_drc_prev_luma_5_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcPrevLuma5)
{
    U_ISP_DRC_PREV_LUMA_5 o_isp_drc_prev_luma_5;
    o_isp_drc_prev_luma_5.bits.isp_drc_prev_luma_5 = uIspDrcPrevLuma5;
    pstBeReg->ISP_DRC_PREV_LUMA_5.u32 = o_isp_drc_prev_luma_5.u32;
}

static inline GK_VOID isp_drc_prev_luma_6_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcPrevLuma6)
{
    U_ISP_DRC_PREV_LUMA_6 o_isp_drc_prev_luma_6;
    o_isp_drc_prev_luma_6.bits.isp_drc_prev_luma_6 = uIspDrcPrevLuma6;
    pstBeReg->ISP_DRC_PREV_LUMA_6.u32 = o_isp_drc_prev_luma_6.u32;
}

static inline GK_VOID isp_drc_prev_luma_7_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcPrevLuma7)
{
    U_ISP_DRC_PREV_LUMA_7 o_isp_drc_prev_luma_7;
    o_isp_drc_prev_luma_7.bits.isp_drc_prev_luma_7 = uIspDrcPrevLuma7;
    pstBeReg->ISP_DRC_PREV_LUMA_7.u32 = o_isp_drc_prev_luma_7.u32;
}

static inline GK_VOID isp_drc_suppress_bright_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcSuppressBrightMax, GK_U32 uIspDrcSuppressBrightMin, GK_U32 uIspDrcSuppressBrightThr, GK_U32 uIspDrcSuppressBrightSlo)
{
    U_ISP_DRC_SUPPRESS_BRIGHT o_isp_drc_suppress_bright;
    o_isp_drc_suppress_bright.bits.isp_drc_suppress_bright_max = uIspDrcSuppressBrightMax;
    o_isp_drc_suppress_bright.bits.isp_drc_suppress_bright_min = uIspDrcSuppressBrightMin;
    o_isp_drc_suppress_bright.bits.isp_drc_suppress_bright_thr = uIspDrcSuppressBrightThr;
    o_isp_drc_suppress_bright.bits.isp_drc_suppress_bright_slo = uIspDrcSuppressBrightSlo;
    pstBeReg->ISP_DRC_SUPPRESS_BRIGHT.u32 = o_isp_drc_suppress_bright.u32;
}

static inline GK_VOID isp_drc_suppress_dark_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcSuppressDarkMax, GK_U32 uIspDrcSuppressDarkMin, GK_U32 uIspDrcSuppressDarkThr, GK_U32 uIspDrcSuppressDarkSlo)
{
    U_ISP_DRC_SUPPRESS_DARK o_isp_drc_suppress_dark;
    o_isp_drc_suppress_dark.bits.isp_drc_suppress_dark_max = uIspDrcSuppressDarkMax;
    o_isp_drc_suppress_dark.bits.isp_drc_suppress_dark_min = uIspDrcSuppressDarkMin;
    o_isp_drc_suppress_dark.bits.isp_drc_suppress_dark_thr = uIspDrcSuppressDarkThr;
    o_isp_drc_suppress_dark.bits.isp_drc_suppress_dark_slo = uIspDrcSuppressDarkSlo;
    pstBeReg->ISP_DRC_SUPPRESS_DARK.u32 = o_isp_drc_suppress_dark.u32;
}

static inline GK_VOID isp_drc_idxbase0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcIdxbase0, GK_U32 uIspDrcIdxbase1, GK_U32 uIspDrcIdxbase2, GK_U32 uIspDrcIdxbase3)
{
    U_ISP_DRC_IDXBASE0 o_isp_drc_idxbase0;
    o_isp_drc_idxbase0.bits.isp_drc_idxbase0 = uIspDrcIdxbase0;
    o_isp_drc_idxbase0.bits.isp_drc_idxbase1 = uIspDrcIdxbase1;
    o_isp_drc_idxbase0.bits.isp_drc_idxbase2 = uIspDrcIdxbase2;
    o_isp_drc_idxbase0.bits.isp_drc_idxbase3 = uIspDrcIdxbase3;
    pstBeReg->ISP_DRC_IDXBASE0.u32 = o_isp_drc_idxbase0.u32;
}

static inline GK_VOID isp_drc_idxbase1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcIdxbase4, GK_U32 uIspDrcIdxbase5, GK_U32 uIspDrcIdxbase6, GK_U32 uIspDrcIdxbase7)
{
    U_ISP_DRC_IDXBASE1 o_isp_drc_idxbase1;
    o_isp_drc_idxbase1.bits.isp_drc_idxbase4 = uIspDrcIdxbase4;
    o_isp_drc_idxbase1.bits.isp_drc_idxbase5 = uIspDrcIdxbase5;
    o_isp_drc_idxbase1.bits.isp_drc_idxbase6 = uIspDrcIdxbase6;
    o_isp_drc_idxbase1.bits.isp_drc_idxbase7 = uIspDrcIdxbase7;
    pstBeReg->ISP_DRC_IDXBASE1.u32 = o_isp_drc_idxbase1.u32;
}

static inline GK_VOID isp_drc_maxval0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcMaxval0, GK_U32 uIspDrcMaxval1, GK_U32 uIspDrcMaxval2, GK_U32 uIspDrcMaxval3)
{
    U_ISP_DRC_MAXVAL0 o_isp_drc_maxval0;
    o_isp_drc_maxval0.bits.isp_drc_maxval0 = uIspDrcMaxval0;
    o_isp_drc_maxval0.bits.isp_drc_maxval1 = uIspDrcMaxval1;
    o_isp_drc_maxval0.bits.isp_drc_maxval2 = uIspDrcMaxval2;
    o_isp_drc_maxval0.bits.isp_drc_maxval3 = uIspDrcMaxval3;
    pstBeReg->ISP_DRC_MAXVAL0.u32 = o_isp_drc_maxval0.u32;
}

static inline GK_VOID isp_drc_maxval1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcMaxval4, GK_U32 uIspDrcMaxval5, GK_U32 uIspDrcMaxval6, GK_U32 uIspDrcMaxval7)
{
    U_ISP_DRC_MAXVAL1 o_isp_drc_maxval1;
    o_isp_drc_maxval1.bits.isp_drc_maxval4 = uIspDrcMaxval4;
    o_isp_drc_maxval1.bits.isp_drc_maxval5 = uIspDrcMaxval5;
    o_isp_drc_maxval1.bits.isp_drc_maxval6 = uIspDrcMaxval6;
    o_isp_drc_maxval1.bits.isp_drc_maxval7 = uIspDrcMaxval7;
    pstBeReg->ISP_DRC_MAXVAL1.u32 = o_isp_drc_maxval1.u32;
}

static inline GK_VOID isp_drc_pregamma_idxbase0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcPregammaIdxbase0, GK_U32 uIspDrcPregammaIdxbase1, GK_U32 uIspDrcPregammaIdxbase2, GK_U32 uIspDrcPregammaIdxbase3)
{
    U_ISP_DRC_PREGAMMA_IDXBASE0 o_isp_drc_pregamma_idxbase0;
    o_isp_drc_pregamma_idxbase0.bits.isp_drc_pregamma_idxbase0 = uIspDrcPregammaIdxbase0;
    o_isp_drc_pregamma_idxbase0.bits.isp_drc_pregamma_idxbase1 = uIspDrcPregammaIdxbase1;
    o_isp_drc_pregamma_idxbase0.bits.isp_drc_pregamma_idxbase2 = uIspDrcPregammaIdxbase2;
    o_isp_drc_pregamma_idxbase0.bits.isp_drc_pregamma_idxbase3 = uIspDrcPregammaIdxbase3;
    pstBeReg->ISP_DRC_PREGAMMA_IDXBASE0.u32 = o_isp_drc_pregamma_idxbase0.u32;
}

static inline GK_VOID isp_drc_pregamma_idxbase1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcPregammaIdxbase4, GK_U32 uIspDrcPregammaIdxbase5, GK_U32 uIspDrcPregammaIdxbase6, GK_U32 uIspDrcPregammaIdxbase7)
{
    U_ISP_DRC_PREGAMMA_IDXBASE1 o_isp_drc_pregamma_idxbase1;
    o_isp_drc_pregamma_idxbase1.bits.isp_drc_pregamma_idxbase4 = uIspDrcPregammaIdxbase4;
    o_isp_drc_pregamma_idxbase1.bits.isp_drc_pregamma_idxbase5 = uIspDrcPregammaIdxbase5;
    o_isp_drc_pregamma_idxbase1.bits.isp_drc_pregamma_idxbase6 = uIspDrcPregammaIdxbase6;
    o_isp_drc_pregamma_idxbase1.bits.isp_drc_pregamma_idxbase7 = uIspDrcPregammaIdxbase7;
    pstBeReg->ISP_DRC_PREGAMMA_IDXBASE1.u32 = o_isp_drc_pregamma_idxbase1.u32;
}

static inline GK_VOID isp_drc_pregamma_idxbase2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcPregammaIdxbase8, GK_U32 uIspDrcPregammaIdxbase9, GK_U32 uIspDrcPregammaIdxbase10, GK_U32 uIspDrcPregammaIdxbase11)
{
    U_ISP_DRC_PREGAMMA_IDXBASE2 o_isp_drc_pregamma_idxbase2;
    o_isp_drc_pregamma_idxbase2.bits.isp_drc_pregamma_idxbase8 = uIspDrcPregammaIdxbase8;
    o_isp_drc_pregamma_idxbase2.bits.isp_drc_pregamma_idxbase9 = uIspDrcPregammaIdxbase9;
    o_isp_drc_pregamma_idxbase2.bits.isp_drc_pregamma_idxbase10 = uIspDrcPregammaIdxbase10;
    o_isp_drc_pregamma_idxbase2.bits.isp_drc_pregamma_idxbase11 = uIspDrcPregammaIdxbase11;
    pstBeReg->ISP_DRC_PREGAMMA_IDXBASE2.u32 = o_isp_drc_pregamma_idxbase2.u32;
}

static inline GK_VOID isp_drc_pregamma_maxval0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcPregammaMaxval0, GK_U32 uIspDrcPregammaMaxval1)
{
    U_ISP_DRC_PREGAMMA_MAXVAL0 o_isp_drc_pregamma_maxval0;
    o_isp_drc_pregamma_maxval0.bits.isp_drc_pregamma_maxval0 = uIspDrcPregammaMaxval0;
    o_isp_drc_pregamma_maxval0.bits.isp_drc_pregamma_maxval1 = uIspDrcPregammaMaxval1;
    pstBeReg->ISP_DRC_PREGAMMA_MAXVAL0.u32 = o_isp_drc_pregamma_maxval0.u32;
}

static inline GK_VOID isp_drc_pregamma_maxval1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcPregammaMaxval2, GK_U32 uIspDrcPregammaMaxval3)
{
    U_ISP_DRC_PREGAMMA_MAXVAL1 o_isp_drc_pregamma_maxval1;
    o_isp_drc_pregamma_maxval1.bits.isp_drc_pregamma_maxval2 = uIspDrcPregammaMaxval2;
    o_isp_drc_pregamma_maxval1.bits.isp_drc_pregamma_maxval3 = uIspDrcPregammaMaxval3;
    pstBeReg->ISP_DRC_PREGAMMA_MAXVAL1.u32 = o_isp_drc_pregamma_maxval1.u32;
}

static inline GK_VOID isp_drc_pregamma_maxval2_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcPregammaMaxval4, GK_U32 uIspDrcPregammaMaxval5)
{
    U_ISP_DRC_PREGAMMA_MAXVAL2 o_isp_drc_pregamma_maxval2;
    o_isp_drc_pregamma_maxval2.bits.isp_drc_pregamma_maxval4 = uIspDrcPregammaMaxval4;
    o_isp_drc_pregamma_maxval2.bits.isp_drc_pregamma_maxval5 = uIspDrcPregammaMaxval5;
    pstBeReg->ISP_DRC_PREGAMMA_MAXVAL2.u32 = o_isp_drc_pregamma_maxval2.u32;
}

static inline GK_VOID isp_drc_pregamma_maxval3_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcPregammaMaxval6, GK_U32 uIspDrcPregammaMaxval7)
{
    U_ISP_DRC_PREGAMMA_MAXVAL3 o_isp_drc_pregamma_maxval3;
    o_isp_drc_pregamma_maxval3.bits.isp_drc_pregamma_maxval6 = uIspDrcPregammaMaxval6;
    o_isp_drc_pregamma_maxval3.bits.isp_drc_pregamma_maxval7 = uIspDrcPregammaMaxval7;
    pstBeReg->ISP_DRC_PREGAMMA_MAXVAL3.u32 = o_isp_drc_pregamma_maxval3.u32;
}

static inline GK_VOID isp_drc_pregamma_maxval4_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcPregammaMaxval8, GK_U32 uIspDrcPregammaMaxval9)
{
    U_ISP_DRC_PREGAMMA_MAXVAL4 o_isp_drc_pregamma_maxval4;
    o_isp_drc_pregamma_maxval4.bits.isp_drc_pregamma_maxval8 = uIspDrcPregammaMaxval8;
    o_isp_drc_pregamma_maxval4.bits.isp_drc_pregamma_maxval9 = uIspDrcPregammaMaxval9;
    pstBeReg->ISP_DRC_PREGAMMA_MAXVAL4.u32 = o_isp_drc_pregamma_maxval4.u32;
}

static inline GK_VOID isp_drc_pregamma_maxval5_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcPregammaMaxval10, GK_U32 uIspDrcPregammaMaxval11)
{
    U_ISP_DRC_PREGAMMA_MAXVAL5 o_isp_drc_pregamma_maxval5;
    o_isp_drc_pregamma_maxval5.bits.isp_drc_pregamma_maxval10 = uIspDrcPregammaMaxval10;
    o_isp_drc_pregamma_maxval5.bits.isp_drc_pregamma_maxval11 = uIspDrcPregammaMaxval11;
    pstBeReg->ISP_DRC_PREGAMMA_MAXVAL5.u32 = o_isp_drc_pregamma_maxval5.u32;
}

static inline GK_VOID isp_drc_flt_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcFltSpaFine, GK_U32 uIspDrcFltSpaMedium, GK_U32 uIspDrcFltSpaCoarse, GK_U32 uIspDrcFltRngFine, GK_U32 uIspDrcFltRngMedium, GK_U32 uIspDrcFltRngCoarse)
{
    U_ISP_DRC_FLT_CFG o_isp_drc_flt_cfg;
    o_isp_drc_flt_cfg.bits.isp_drc_flt_spa_fine = uIspDrcFltSpaFine;
    o_isp_drc_flt_cfg.bits.isp_drc_flt_spa_medium = uIspDrcFltSpaMedium;
    o_isp_drc_flt_cfg.bits.isp_drc_flt_spa_coarse = uIspDrcFltSpaCoarse;
    o_isp_drc_flt_cfg.bits.isp_drc_flt_rng_fine = uIspDrcFltRngFine;
    o_isp_drc_flt_cfg.bits.isp_drc_flt_rng_medium = uIspDrcFltRngMedium;
    o_isp_drc_flt_cfg.bits.isp_drc_flt_rng_coarse = uIspDrcFltRngCoarse;
    pstBeReg->ISP_DRC_FLT_CFG.u32 = o_isp_drc_flt_cfg.u32;
}

static inline GK_VOID isp_drc_bin_factor_coarse_0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcBinMixFactorCoarse0, GK_U32 uIspDrcBinMixFactorCoarse1, GK_U32 uIspDrcBinMixFactorCoarse2, GK_U32 uIspDrcBinMixFactorCoarse3)
{
    U_ISP_DRC_BIN_FACTOR_COARSE_0 o_isp_drc_bin_factor_coarse_0;
    o_isp_drc_bin_factor_coarse_0.bits.isp_drc_bin_mix_factor_coarse_0 = uIspDrcBinMixFactorCoarse0;
    o_isp_drc_bin_factor_coarse_0.bits.isp_drc_bin_mix_factor_coarse_1 = uIspDrcBinMixFactorCoarse1;
    o_isp_drc_bin_factor_coarse_0.bits.isp_drc_bin_mix_factor_coarse_2 = uIspDrcBinMixFactorCoarse2;
    o_isp_drc_bin_factor_coarse_0.bits.isp_drc_bin_mix_factor_coarse_3 = uIspDrcBinMixFactorCoarse3;
    pstBeReg->ISP_DRC_BIN_FACTOR_COARSE_0.u32 = o_isp_drc_bin_factor_coarse_0.u32;
}

static inline GK_VOID isp_drc_bin_factor_coarse_1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcBinMixFactorCoarse4, GK_U32 uIspDrcBinMixFactorCoarse5, GK_U32 uIspDrcBinMixFactorCoarse6, GK_U32 uIspDrcBinMixFactorCoarse7)
{
    U_ISP_DRC_BIN_FACTOR_COARSE_1 o_isp_drc_bin_factor_coarse_1;
    o_isp_drc_bin_factor_coarse_1.bits.isp_drc_bin_mix_factor_coarse_4 = uIspDrcBinMixFactorCoarse4;
    o_isp_drc_bin_factor_coarse_1.bits.isp_drc_bin_mix_factor_coarse_5 = uIspDrcBinMixFactorCoarse5;
    o_isp_drc_bin_factor_coarse_1.bits.isp_drc_bin_mix_factor_coarse_6 = uIspDrcBinMixFactorCoarse6;
    o_isp_drc_bin_factor_coarse_1.bits.isp_drc_bin_mix_factor_coarse_7 = uIspDrcBinMixFactorCoarse7;
    pstBeReg->ISP_DRC_BIN_FACTOR_COARSE_1.u32 = o_isp_drc_bin_factor_coarse_1.u32;
}

static inline GK_VOID isp_drc_bin_factor_medium_0_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcBinMixFactorMedium0, GK_U32 uIspDrcBinMixFactorMedium1, GK_U32 uIspDrcBinMixFactorMedium2, GK_U32 uIspDrcBinMixFactorMedium3)
{
    U_ISP_DRC_BIN_FACTOR_MEDIUM_0 o_isp_drc_bin_factor_medium_0;
    o_isp_drc_bin_factor_medium_0.bits.isp_drc_bin_mix_factor_medium_0 = uIspDrcBinMixFactorMedium0;
    o_isp_drc_bin_factor_medium_0.bits.isp_drc_bin_mix_factor_medium_1 = uIspDrcBinMixFactorMedium1;
    o_isp_drc_bin_factor_medium_0.bits.isp_drc_bin_mix_factor_medium_2 = uIspDrcBinMixFactorMedium2;
    o_isp_drc_bin_factor_medium_0.bits.isp_drc_bin_mix_factor_medium_3 = uIspDrcBinMixFactorMedium3;
    pstBeReg->ISP_DRC_BIN_FACTOR_MEDIUM_0.u32 = o_isp_drc_bin_factor_medium_0.u32;
}

static inline GK_VOID isp_drc_bin_factor_medium_1_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcBinMixFactorMedium4, GK_U32 uIspDrcBinMixFactorMedium5, GK_U32 uIspDrcBinMixFactorMedium6, GK_U32 uIspDrcBinMixFactorMedium7)
{
    U_ISP_DRC_BIN_FACTOR_MEDIUM_1 o_isp_drc_bin_factor_medium_1;
    o_isp_drc_bin_factor_medium_1.bits.isp_drc_bin_mix_factor_medium_4 = uIspDrcBinMixFactorMedium4;
    o_isp_drc_bin_factor_medium_1.bits.isp_drc_bin_mix_factor_medium_5 = uIspDrcBinMixFactorMedium5;
    o_isp_drc_bin_factor_medium_1.bits.isp_drc_bin_mix_factor_medium_6 = uIspDrcBinMixFactorMedium6;
    o_isp_drc_bin_factor_medium_1.bits.isp_drc_bin_mix_factor_medium_7 = uIspDrcBinMixFactorMedium7;
    pstBeReg->ISP_DRC_BIN_FACTOR_MEDIUM_1.u32 = o_isp_drc_bin_factor_medium_1.u32;
}

static inline GK_VOID isp_drc_detail_sub_factor_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcDetailSubFactor)
{
    U_ISP_DRC_DETAIL_SUB_FACTOR o_isp_drc_detail_sub_factor;
    o_isp_drc_detail_sub_factor.bits.isp_drc_detail_sub_factor = uIspDrcDetailSubFactor;
    pstBeReg->ISP_DRC_DETAIL_SUB_FACTOR.u32 = o_isp_drc_detail_sub_factor.u32;
}

static inline GK_VOID isp_drc_wgt_box_tri_sel_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcWgtBoxTriSel)
{
    U_ISP_DRC_WGT_BOX_TRI_SEL o_isp_drc_wgt_box_tri_sel;
    o_isp_drc_wgt_box_tri_sel.bits.isp_drc_wgt_box_tri_sel = uIspDrcWgtBoxTriSel;
    pstBeReg->ISP_DRC_WGT_BOX_TRI_SEL.u32 = o_isp_drc_wgt_box_tri_sel.u32;
}

static inline GK_VOID isp_drc_blk_wgt_init_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcBlkWgtInit)
{
    U_ISP_DRC_BLK_WGT_INIT o_isp_drc_blk_wgt_init;
    o_isp_drc_blk_wgt_init.bits.isp_drc_blk_wgt_init = uIspDrcBlkWgtInit;
    pstBeReg->ISP_DRC_BLK_WGT_INIT.u32 = o_isp_drc_blk_wgt_init.u32;
}

static inline GK_VOID isp_drc_shp_cfg_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcShpLog, GK_U32 uIspDrcShpExp)
{
    U_ISP_DRC_SHP_CFG o_isp_drc_shp_cfg;
    o_isp_drc_shp_cfg.bits.isp_drc_shp_log = uIspDrcShpLog;
    o_isp_drc_shp_cfg.bits.isp_drc_shp_exp = uIspDrcShpExp;
    pstBeReg->ISP_DRC_SHP_CFG.u32 = o_isp_drc_shp_cfg.u32;
}

static inline GK_VOID isp_drc_div_denom_log_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcDivDenomLog)
{
    U_ISP_DRC_DIV_DENOM_LOG o_isp_drc_div_denom_log;
    o_isp_drc_div_denom_log.bits.isp_drc_div_denom_log = uIspDrcDivDenomLog;
    pstBeReg->ISP_DRC_DIV_DENOM_LOG.u32 = o_isp_drc_div_denom_log.u32;
}

static inline GK_VOID isp_drc_denom_exp_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcDenomExp)
{
    U_ISP_DRC_DENOM_EXP o_isp_drc_denom_exp;
    o_isp_drc_denom_exp.bits.isp_drc_denom_exp = uIspDrcDenomExp;
    pstBeReg->ISP_DRC_DENOM_EXP.u32 = o_isp_drc_denom_exp.u32;
}

static inline GK_VOID isp_drc_debug_info_u32_write(S_ISPBE_REGS_TYPE *pstBeReg, GK_U32 uIspDrcDebugInfo)
{
    U_ISP_DRC_DEBUG_INFO o_isp_drc_debug_info;
    o_isp_drc_debug_info.bits.isp_drc_debug_info = uIspDrcDebugInfo;
    pstBeReg->ISP_DRC_DEBUG_INFO.u32 = o_isp_drc_debug_info.u32;
}
