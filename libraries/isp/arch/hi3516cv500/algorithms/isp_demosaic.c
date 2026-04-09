/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_demosaic.c
 * Author: ISP SW
 * Create: 2012/06/28
 */
#include "isp_demosaic.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_proc.h"
#include "isp_math_utils.h"
#include <math.h>

#define HI_DEMOSAIC_BITDEPTH    12
#define DEMOSAIC_FILTER_NUM     7
#define DEMOSAIC_FILTER_COEF    3
#define BITFIX_NUM             10

#define dm_clip3(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

static const  hi_s32 g_demosaic_filtcoef[DEMOSAIC_FILTER_NUM][DEMOSAIC_FILTER_COEF] = {
    { 0, 0, 32},
    { -1, 4, 26},
    { -1, 5, 24},
    { -1, 6, 22},
    { -1, 7, 20},
    { -1, 8, 18},
    { 0, 8, 16}
};

static const  hi_u8  g_nddm_str[ISP_AUTO_ISO_STRENGTH_NUM] = {
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};
static const  hi_u8  g_nddm_mf_detailehc_str[ISP_AUTO_ISO_STRENGTH_NUM] = {
    32, 24, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16
};
static const  hi_u8  g_nddm_hf_detailehc_str[ISP_AUTO_ISO_STRENGTH_NUM] = {
    0, 0, 3, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
};
static const  hi_u8  g_detail_smooth_range[ISP_AUTO_ISO_STRENGTH_NUM] = {
    2, 2, 2, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 7
};
static const  hi_u8  g_color_noise_thdf[ISP_AUTO_ISO_STRENGTH_NUM] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
static const  hi_u8  g_color_noise_strf[ISP_AUTO_ISO_STRENGTH_NUM] = {
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
};
static const  hi_u8  g_color_noise_thdy[ISP_AUTO_ISO_STRENGTH_NUM] = {
    30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30
};
static const  hi_u8  g_color_noise_stry[ISP_AUTO_ISO_STRENGTH_NUM] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// Linear parameters
static const hi_u8   g_lut_cc_hf_max_ratio_lin[ISP_AUTO_ISO_STRENGTH_NUM] = {
    3, 4, 5, 6, 6, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8
};
static const hi_u8   g_lut_cc_hf_min_ratio_lin[ISP_AUTO_ISO_STRENGTH_NUM] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
static const hi_s8   g_lut_fcr_gf_gain_lin[ISP_AUTO_ISO_STRENGTH_NUM]     = {
    12, 12, 12, 10, 8, 6, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
static const hi_u8   g_lut_awb_gfgain_max_lin[ISP_AUTO_ISO_STRENGTH_NUM]  = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};
static const hi_u8   g_lut_awb_gfgain_low_lin[ISP_AUTO_ISO_STRENGTH_NUM]  = {
    32, 32, 30, 26, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20
};
static const hi_u8   g_lut_awb_gfgain_hig_lin[ISP_AUTO_ISO_STRENGTH_NUM]  = {
    64, 64, 60, 56, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48
};
static const hi_u16  g_lut_fcr_det_low_lin[ISP_AUTO_ISO_STRENGTH_NUM]     = {
    150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150
};
static const hi_u16  g_lut_desat_low_lin[ISP_AUTO_ISO_STRENGTH_NUM]       = {
    166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166
};
static const hi_u16  g_lut_desat_prot_th_lin[ISP_AUTO_ISO_STRENGTH_NUM]   = {
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128
};
static const hi_u16  g_hf_intp_blur_th_low_lin[ISP_AUTO_ISO_STRENGTH_NUM] = {
    1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023
};
static const hi_u16  g_hf_intp_blur_th_hig_lin[ISP_AUTO_ISO_STRENGTH_NUM] = {
    1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023
};

// wdr parameters
static const hi_u8   g_lut_cc_hf_max_ratio_wdr[ISP_AUTO_ISO_STRENGTH_NUM] = {
    8, 8, 8, 8, 10, 10, 10, 12, 12, 14, 14, 16, 16, 16, 16, 16
};
static const hi_u8   g_lut_cc_hf_min_ratio_wdr[ISP_AUTO_ISO_STRENGTH_NUM] = {
    0, 0, 0, 0, 0, 0, 2, 2, 4, 4, 4, 8, 10, 10, 12, 14
};
static const hi_s8   g_lut_fcr_gf_gain_wdr[ISP_AUTO_ISO_STRENGTH_NUM]     = {
    -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
static const hi_u8   g_lut_awb_gfgain_low_wdr[ISP_AUTO_ISO_STRENGTH_NUM]  = {
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32
};
static const hi_u8   g_lut_awb_gfgain_hig_wdr[ISP_AUTO_ISO_STRENGTH_NUM]  = {
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};
static const hi_u8   g_lut_awb_gfgain_max_wdr[ISP_AUTO_ISO_STRENGTH_NUM]  = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};
static const hi_u16  g_lut_fcr_det_low_wdr[ISP_AUTO_ISO_STRENGTH_NUM]     = {
    150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150
};
static const hi_u16  g_lut_desat_low_wdr[ISP_AUTO_ISO_STRENGTH_NUM]       = {
    166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166
};
static const hi_u16  g_lut_desat_prot_th_wdr[ISP_AUTO_ISO_STRENGTH_NUM]   = {
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128
};
static const hi_u16  g_hf_intp_blur_th_low_wdr[ISP_AUTO_ISO_STRENGTH_NUM] = {
    1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023
};
static const hi_u16  g_hf_intp_blur_th_hig_wdr[ISP_AUTO_ISO_STRENGTH_NUM] = {
    1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023
};

static isp_demosaic *g_demosaic_ctx[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define demosaic_get_ctx(dev, ctx)   ((ctx) = g_demosaic_ctx[dev])
#define demosaic_set_ctx(dev, ctx)   (g_demosaic_ctx[dev] = (ctx))
#define demosaic_reset_ctx(dev)      (g_demosaic_ctx[dev] = HI_NULL)

static hi_s32 demosaic_ctx_init(hi_vi_pipe vi_pipe)
{
    isp_demosaic *demosaic_ctx = HI_NULL;

    demosaic_get_ctx(vi_pipe, demosaic_ctx);

    if (demosaic_ctx == HI_NULL) {
        demosaic_ctx = (isp_demosaic *)isp_malloc(sizeof(isp_demosaic));
        if (demosaic_ctx == HI_NULL) {
            isp_err_trace("isp[%d] demosaic_ctx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset_s(demosaic_ctx, sizeof(isp_demosaic), 0, sizeof(isp_demosaic));

    demosaic_set_ctx(vi_pipe, demosaic_ctx);

    return HI_SUCCESS;
}

static hi_void demosaic_ctx_exit(hi_vi_pipe vi_pipe)
{
    isp_demosaic *demosaic_ctx = HI_NULL;

    demosaic_get_ctx(vi_pipe, demosaic_ctx);
    isp_free(demosaic_ctx);
    demosaic_reset_ctx(vi_pipe);
}

static hi_void demosaic_init_fw_linear(hi_vi_pipe vi_pipe, isp_demosaic *demosaic)
{
    memcpy_s(demosaic->lut_cc_hf_max_ratio, sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM,
             g_lut_cc_hf_max_ratio_lin,     sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(demosaic->lut_cc_hf_min_ratio, sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM,
             g_lut_cc_hf_min_ratio_lin,     sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(demosaic->lut_fcr_gf_gain,     sizeof(hi_s8) * ISP_AUTO_ISO_STRENGTH_NUM,
             g_lut_fcr_gf_gain_lin,         sizeof(hi_s8) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(demosaic->lut_awb_gfgain_low,  sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM,
             g_lut_awb_gfgain_low_lin,      sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(demosaic->lut_awb_gfgain_hig,  sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM,
             g_lut_awb_gfgain_hig_lin,      sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(demosaic->lut_awb_gfgain_max,  sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM,
             g_lut_awb_gfgain_max_lin,      sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(demosaic->lut_fcr_det_low,     sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM,
             g_lut_fcr_det_low_lin,         sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(demosaic->lut_desat_low,       sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM,
             g_lut_desat_low_lin,           sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(demosaic->lut_desat_prot_th,   sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM,
             g_lut_desat_prot_th_lin,       sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(demosaic->hf_intp_blur_th_low, sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM,
             g_hf_intp_blur_th_low_lin,     sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(demosaic->hf_intp_blur_th_hig, sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM,
             g_hf_intp_blur_th_hig_lin,     sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM);
}

static hi_void demosaic_init_fw_wdr(hi_vi_pipe vi_pipe, isp_demosaic *demosaic)
{
    memcpy_s(demosaic->lut_cc_hf_max_ratio, sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM,
             g_lut_cc_hf_max_ratio_wdr,     sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(demosaic->lut_cc_hf_min_ratio, sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM,
             g_lut_cc_hf_min_ratio_wdr,     sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(demosaic->lut_fcr_gf_gain,     sizeof(hi_s8) * ISP_AUTO_ISO_STRENGTH_NUM,
             g_lut_fcr_gf_gain_wdr,         sizeof(hi_s8) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(demosaic->lut_awb_gfgain_low,  sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM,
             g_lut_awb_gfgain_low_wdr,      sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(demosaic->lut_awb_gfgain_hig,  sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM,
             g_lut_awb_gfgain_hig_wdr,      sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(demosaic->lut_awb_gfgain_max,  sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM,
             g_lut_awb_gfgain_max_wdr,      sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(demosaic->lut_fcr_det_low,     sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM,
             g_lut_fcr_det_low_wdr,         sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(demosaic->lut_desat_low,       sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM,
             g_lut_desat_low_wdr,           sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(demosaic->lut_desat_prot_th,   sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM,
             g_lut_desat_prot_th_wdr,       sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(demosaic->hf_intp_blur_th_low, sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM,
             g_hf_intp_blur_th_low_wdr,     sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(demosaic->hf_intp_blur_th_hig, sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM,
             g_hf_intp_blur_th_hig_wdr,     sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM);
}

static hi_void demosaic_static_regs_initialize(hi_vi_pipe vi_pipe, isp_demosaic_static_cfg *static_reg_cfg, hi_u32 i)
{
    static_reg_cfg->de_sat_enable      = HI_ISP_DEMOSAIC_DESAT_ENABLE_DEFAULT;
    static_reg_cfg->g_intp_ctrl_enable = HI_ISP_DEMOSAIC_G_INTP_CONTROL_DEFAULT;
    static_reg_cfg->ahd_enable         = HI_ISP_DEMOSAIC_AND_EN_DEFAULT;
    static_reg_cfg->de_fake_enable     = HI_ISP_DEMOSAIC_DE_FAKE_EN_DEFAULT;
    static_reg_cfg->lpff3              = HI_ISP_DEMOSAIC_LPF_F3_DEFAULT;
    static_reg_cfg->hv_blend_limit1    = HI_ISP_DEMOSAIC_BLENDLIMIT1_DEFAULT;
    static_reg_cfg->hv_blend_limit2    = HI_ISP_DEMOSAIC_BLENDLIMIT2_DEFAULT;
    static_reg_cfg->hv_color_ratio     = HI_ISP_DEMOSAIC_HV_RATIO_DEFAULT;
    static_reg_cfg->hv_selection       = HI_ISP_DEMOSAIC_HV_SEL_DEFAULT;
    static_reg_cfg->g_clip_bit_sft     = HI_ISP_DEMOSAIC_G_CLIP_SFT_BIT_DEFAULT;
    static_reg_cfg->dither_mask        = HI_ISP_DITHER_MASK_DEFAULT;
    static_reg_cfg->dither_ratio       = HI_ISP_DITHER_RATIO_DEFAULT;
    static_reg_cfg->cx_var_max_rate    = HI_ISP_DEMOSAIC_CX_VAR_MAX_RATE_DEFAULT;
    static_reg_cfg->cx_var_min_rate    = HI_ISP_DEMOSAIC_CX_VAR_MIN_RATE_DEFAULT;
    static_reg_cfg->de_sat_thresh1     = HI_ISP_DEMOSAIC_DESAT_THRESH1_DEFAULT;
    static_reg_cfg->de_sat_thresh2     = HI_ISP_DEMOSAIC_DESAT_THRESH2_DEFAULT;
    static_reg_cfg->de_sat_hig         = HI_ISP_DEMOSAIC_DESAT_HIG_DEFAULT;
    static_reg_cfg->de_sat_prot_sl     = HI_ISP_DEMOSAIC_DESAT_PROTECT_SL_DEFAULT;
    static_reg_cfg->ahd_part1          = HI_ISP_DEMOSAIC_AHDPART1_DEFAULT;
    static_reg_cfg->ahd_part2          = HI_ISP_DEMOSAIC_AHDPART2_DEFAULT;
    static_reg_cfg->gf_th_low          = HI_ISP_NDDM_GF_TH_LOW_DEFAULT;
    static_reg_cfg->gf_th_hig          = HI_ISP_NDDM_GF_TH_HIGH_DEFAULT;
    static_reg_cfg->cb_cr_avg_thr      = HI_ISP_DEMOSAIC_CBCR_AVG_THLD_DEFAULT;

    static_reg_cfg->resh = HI_TRUE;
}

static hi_void demosaic_dyna_regs_initialize(isp_demosaic_dyna_cfg *dyna_reg_cfg)
{
    hi_u32 n;

    dyna_reg_cfg->lpff0                = HI_ISP_DEMOSAIC_LPF_F0_DEFAULT;
    dyna_reg_cfg->lpff1                = HI_ISP_DEMOSAIC_LPF_F1_DEFAULT;
    dyna_reg_cfg->lpff2                = HI_ISP_DEMOSAIC_LPF_F2_DEFAULT;
    dyna_reg_cfg->cc_hf_max_ratio      = HI_ISP_DEMOSAIC_CC_HF_MAX_RATIO_DEFAULT;
    dyna_reg_cfg->cc_hf_min_ratio      = HI_ISP_DEMOSAIC_CC_HF_MIN_RATIO_DEFAULT;
    dyna_reg_cfg->dither_max           = HI_ISP_DITH_MAX_DEFAULT;
    dyna_reg_cfg->fcr_gf_gain          = HI_ISP_NDDM_FCR_GF_GAIN_DEFAULT;
    dyna_reg_cfg->awb_gf_gain_low      = HI_ISP_NDDM_AWB_GF_GN_LOW_DEFAULT;
    dyna_reg_cfg->awb_gf_gain_hig      = HI_ISP_NDDM_AWB_GF_GN_HIGH_DEFAULT;
    dyna_reg_cfg->awb_gf_gain_max      = HI_ISP_NDDM_AWB_GF_GN_MAX_DEFAULT;
    dyna_reg_cfg->ehc_gray             = HI_ISP_NDDM_EHC_GRAY_DEFAULT;
    dyna_reg_cfg->de_sat_low           = HI_ISP_DEMOSAIC_DESAT_LOW_DEFAULT;
    dyna_reg_cfg->de_sat_prot_th       = HI_ISP_DEMOSAIC_DESAT_PROTECT_TH_DEFAULT;
    dyna_reg_cfg->hf_intp_blur_th_low  = HI_ISP_DEMOSAIC_HF_INPT_BLUR_TH_LOW_DEFAULT;
    dyna_reg_cfg->hf_intp_blur_th_hig  = HI_ISP_DEMOSAIC_HF_INPT_BLUR_TH_HIGH_DEFAULT;
    dyna_reg_cfg->fcr_det_low          = HI_ISP_NDDM_FCR_DET_LOW_DEFAULT;
    dyna_reg_cfg->fake_cr_var_thr_low  = HI_ISP_DEMOSAIC_FAKECR_VAR_THR_LOW_DEFAULT;
    dyna_reg_cfg->fake_cr_var_thr_high = HI_ISP_DEMOSAIC_FAKECR_VAR_THR_HIGH_DEFAULT;
    dyna_reg_cfg->de_f_color_cb        = HI_ISP_DEMOSAIC_COLORNOISE_STR_MAX - HI_ISP_DEMOSAIC_DEFCOLOR_CB_DEFAULT;
    dyna_reg_cfg->de_f_color_cr        = HI_ISP_DEMOSAIC_COLORNOISE_STR_MAX - HI_ISP_DEMOSAIC_DEFCOLOR_CR_DEFAULT;

    for (n = 0; n < HI_ISP_DEMOSAIC_LUT_LENGTH; n++) {
        dyna_reg_cfg->gf_blur_luts[n] = 0;
    }

    dyna_reg_cfg->color_noise_thd_y              = HI_EXT_SYSTEM_DEMOSAIC_MANUAL_DESAT_DARK_RANGE_DEFAULT;
    dyna_reg_cfg->color_noise_str_y              = HI_EXT_SYSTEM_DEMOSAIC_MANUAL_DESAT_DARK_STRENGTH_DEFAULT;
    dyna_reg_cfg->pro_dark_region.ccm_prot_en    = HI_ISP_CCM_PROT_EN_DEFAULT;
    dyna_reg_cfg->pro_dark_region.data_cc_thd0   = HI_ISP_CCM_CC_THD0_DEFAULT;
    dyna_reg_cfg->pro_dark_region.data_cc_thd1   = HI_ISP_CCM_CC_THD1_DEFAULT;
    dyna_reg_cfg->pro_dark_region.cc_prot_ratio  = HI_ISP_CCM_CC_PROT_RATIO_DEFAULT;
    dyna_reg_cfg->pro_dark_region.data_rr_thd0   = HI_ISP_CCM_RR_THD0_DEFAULT;
    dyna_reg_cfg->pro_dark_region.data_rr_thd1   = HI_ISP_CCM_RR_THD1_DEFAULT;
    dyna_reg_cfg->pro_dark_region.data_gg_thd0   = HI_ISP_CCM_GG_THD0_DEFAULT;
    dyna_reg_cfg->pro_dark_region.data_gg_thd1   = HI_ISP_CCM_GG_THD1_DEFAULT;
    dyna_reg_cfg->pro_dark_region.data_bb_thd0   = HI_ISP_CCM_BB_THD0_DEFAULT;
    dyna_reg_cfg->pro_dark_region.data_bb_thd1   = HI_ISP_CCM_BB_THD1_DEFAULT;
    dyna_reg_cfg->pro_dark_region.max_rgb_thd    = HI_ISP_CCM_MAX_RGB_DEFAULT;
    dyna_reg_cfg->pro_dark_region.rgb_prot_ratio = HI_ISP_CCM_RGB_PROT_RATIO_DEFAULT;

    dyna_reg_cfg->update_gf = HI_TRUE;
    dyna_reg_cfg->resh      = HI_TRUE;
}

static hi_void demosaic_regs_initialize(hi_vi_pipe vi_pipe, isp_reg_cfg  *reg_cfg)
{
    hi_u32 i;
    isp_demosaic_static_cfg *static_reg_cfg = HI_NULL;
    isp_demosaic_dyna_cfg   *dyna_reg_cfg   = HI_NULL;

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        static_reg_cfg = &reg_cfg->alg_reg_cfg[i].dem_reg_cfg.static_reg_cfg;
        dyna_reg_cfg   = &reg_cfg->alg_reg_cfg[i].dem_reg_cfg.dyna_reg_cfg;
        reg_cfg->alg_reg_cfg[i].dem_reg_cfg.vhdm_enable = HI_TRUE;
        reg_cfg->alg_reg_cfg[i].dem_reg_cfg.nddm_enable = HI_TRUE;
        demosaic_static_regs_initialize(vi_pipe, static_reg_cfg, i);
        demosaic_dyna_regs_initialize(dyna_reg_cfg);
    }

    reg_cfg->cfg_key.bit1_dem_cfg = 1;
}

static hi_s32 demosaic_check_cmos_param(hi_vi_pipe vi_pipe, const hi_isp_cmos_demosaic *cmos_demosaic)
{
    hi_u8 i;

    isp_check_bool_return(cmos_demosaic->enable);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (cmos_demosaic->non_dir_mf_detail_ehc_str[i] > HI_ISP_DEMOSAIC_NONDIR_MFDETALEHC_STR_MAX) {
            isp_err_trace("Invalid non_dir_mf_detail_ehc_str[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (cmos_demosaic->non_dir_hf_detail_ehc_str[i] > 0x10) {
            isp_err_trace("Invalid non_dir_hf_detail_ehc_str[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if ((cmos_demosaic->detail_smooth_range[i] > HI_ISP_DEMOSAIC_DETAIL_SMOOTH_RANGE_MAX) ||
            (cmos_demosaic->detail_smooth_range[i] < HI_ISP_DEMOSAIC_DETAIL_SMOOTH_RANGE_MIN)) {
            isp_err_trace("Invalid detail_smooth_range[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    return HI_SUCCESS;
}

static hi_s32 demosaic_update_from_sns(hi_vi_pipe vi_pipe, hi_isp_cmos_default *sns_dft, isp_demosaic *demosaic)
{
    hi_s32 ret;

    if (sns_dft->key.bit1_demosaic) {
        isp_check_pointer_return(sns_dft->demosaic);

        ret = demosaic_check_cmos_param(vi_pipe, sns_dft->demosaic);
        if (ret != HI_SUCCESS) {
            return ret;
        }

        demosaic->mpi_cfg.enable = sns_dft->demosaic->enable;

        memcpy_s(demosaic->mpi_cfg.auto_attr.non_dir_str,               ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
                 sns_dft->demosaic->non_dir_str,                        ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
        memcpy_s(demosaic->mpi_cfg.auto_attr.non_dir_mf_detail_ehc_str, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
                 sns_dft->demosaic->non_dir_mf_detail_ehc_str,          ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
        memcpy_s(demosaic->mpi_cfg.auto_attr.non_dir_hf_detail_ehc_str, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
                 sns_dft->demosaic->non_dir_hf_detail_ehc_str,          ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
        memcpy_s(demosaic->mpi_cfg.auto_attr.detail_smooth_range,       ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
                 sns_dft->demosaic->detail_smooth_range,                ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
    } else {
        demosaic->mpi_cfg.enable = HI_TRUE;

        memcpy_s(demosaic->mpi_cfg.auto_attr.non_dir_str,               ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
                 g_nddm_str,                                            ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
        memcpy_s(demosaic->mpi_cfg.auto_attr.non_dir_mf_detail_ehc_str, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
                 g_nddm_mf_detailehc_str,                               ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
        memcpy_s(demosaic->mpi_cfg.auto_attr.non_dir_hf_detail_ehc_str, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
                 g_nddm_hf_detailehc_str,                               ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
        memcpy_s(demosaic->mpi_cfg.auto_attr.detail_smooth_range,       ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
                 g_detail_smooth_range,                                 ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
    }

    memcpy_s(demosaic->mpi_cfg.auto_attr.color_noise_ctrl_thdf, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
             g_color_noise_thdf,                                ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
    memcpy_s(demosaic->mpi_cfg.auto_attr.color_noise_ctrl_strf, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
             g_color_noise_strf,                                ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
    memcpy_s(demosaic->mpi_cfg.auto_attr.color_noise_ctrl_thdy, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
             g_color_noise_thdy,                                ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
    memcpy_s(demosaic->mpi_cfg.auto_attr.color_noise_ctrl_stry, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
             g_color_noise_stry,                                ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));

    demosaic->mpi_cfg.manual_attr.color_noise_ctrl_thdf = HI_EXT_SYSTEM_DEMOSAIC_COLORNOISE_CTRL_THDF_DEFAULT;
    demosaic->mpi_cfg.manual_attr.color_noise_ctrl_strf = HI_EXT_SYSTEM_DEMOSAIC_COLORNOISE_CTRL_STRF_DEFAULT;
    demosaic->mpi_cfg.manual_attr.color_noise_ctrl_thdy = HI_EXT_SYSTEM_DEMOSAIC_MANUAL_DESAT_DARK_RANGE_DEFAULT;
    demosaic->mpi_cfg.manual_attr.color_noise_ctrl_stry = HI_EXT_SYSTEM_DEMOSAIC_MANUAL_DESAT_DARK_STRENGTH_DEFAULT;

    return HI_SUCCESS;
}

static hi_s32 isp_demosaic_ctx_initialize(hi_vi_pipe vi_pipe, isp_demosaic *demosaic)
{
    hi_u8 i;
    hi_s32 ret;
    const hi_s32   sigma_norm_factor = 16;
    hi_float       n_cur, n_pre, n_fct;
    hi_float       drc_compensate = 0.6f;
    const hi_float max_num = 256.0f;
    hi_float       afsigma[HI_ISP_DEMOSAIC_LUT_LENGTH];

    hi_isp_cmos_default *sns_dft = HI_NULL;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_sensor_get_default(vi_pipe, &sns_dft);

    demosaic->wdr_mode = isp_ctx->sns_wdr_mode;
    demosaic->bit_depth_prc = HI_DEMOSAIC_BITDEPTH;

    if (demosaic->wdr_mode != 0) {
        demosaic_init_fw_wdr(vi_pipe, demosaic);

        for (i = 0; i < HI_ISP_DEMOSAIC_LUT_LENGTH; i++) {
            n_cur = (hi_float)(i * sigma_norm_factor);
            n_pre = (hi_float)(max_num * pow(n_cur / max_num, 1.0f / div_0_to_1_float(drc_compensate))) + 0.5f;
            n_fct = (hi_float)(isp_sqr(n_cur / div_0_to_1_float(n_pre)));

            afsigma[i] = n_cur * n_fct;
            demosaic->sigma_demcfg[i] = (hi_u32)(afsigma[i] * isp_bitfix(BITFIX_NUM));
        }
    } else {
        demosaic_init_fw_linear(vi_pipe, demosaic);

        for (i = 0; i < HI_ISP_DEMOSAIC_LUT_LENGTH; i++) {
            demosaic->sigma_demcfg[i]  = (hi_u32)(i * sigma_norm_factor * isp_bitfix(BITFIX_NUM));
        }
    }

    if (is_wdr_mode(demosaic->wdr_mode)) {    /* Manual:WDR mode */
        demosaic->mpi_cfg.manual_attr.non_dir_str               = HI_EXT_SYSTEM_DEMOSAIC_MANUAL_NDDM_STR_WDRDFT;
        demosaic->mpi_cfg.manual_attr.non_dir_mf_detail_ehc_str = HI_EXT_SYSTEM_DEMOSAIC_MANUAL_NDDM_MF_EHNC_STR_WDRDFT;
        demosaic->mpi_cfg.manual_attr.non_dir_hf_detail_ehc_str = HI_EXT_SYSTEM_DEMOSAIC_MANUAL_NDDM_HF_EHNC_STR_WDRDFT;
        demosaic->mpi_cfg.manual_attr.detail_smooth_range       = HI_EXT_SYSTEM_DEMOSAIC_MANUAL_DTLSMOOTH_RANGE_WDRDFT;
    } else {   /* Manual:Linear Mode */
        demosaic->mpi_cfg.manual_attr.non_dir_str               = HI_EXT_SYSTEM_DEMOSAIC_MANUAL_NDDM_STR_LINDFT;
        demosaic->mpi_cfg.manual_attr.non_dir_mf_detail_ehc_str = HI_EXT_SYSTEM_DEMOSAIC_MANUAL_NDDM_MF_EHNC_STR_LINDFT;
        demosaic->mpi_cfg.manual_attr.non_dir_hf_detail_ehc_str = HI_EXT_SYSTEM_DEMOSAIC_MANUAL_NDDM_HF_EHNC_STR_LINDFT;
        demosaic->mpi_cfg.manual_attr.detail_smooth_range       = HI_EXT_SYSTEM_DEMOSAIC_MANUAL_DTLSMOOTH_RANGE_LINDFT;
    }

    // ctx loading from sns
    ret = demosaic_update_from_sns(vi_pipe, sns_dft, demosaic);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    return HI_SUCCESS;
}

static hi_s32 demosaic_ext_regs_initialize(hi_vi_pipe vi_pipe, isp_demosaic *demosaic)
{
    hi_u8 i;
    hi_isp_demosaic_auto_attr   *att_auto   = &demosaic->mpi_cfg.auto_attr;
    hi_isp_demosaic_manual_attr *att_manual = &demosaic->mpi_cfg.manual_attr;

    hi_ext_system_demosaic_enable_write(vi_pipe, demosaic->mpi_cfg.enable);
    hi_ext_system_demosaic_manual_mode_write(vi_pipe, HI_EXT_SYSTEM_DEMOSAIC_MANUAL_MODE_DEFAULT);

    hi_ext_system_demosaic_manual_nddm_mfehnc_str_write(vi_pipe, att_manual->non_dir_mf_detail_ehc_str);
    hi_ext_system_demosaic_manual_nddm_hfehnc_str_write(vi_pipe, att_manual->non_dir_hf_detail_ehc_str);
    hi_ext_system_demosaic_manual_nddm_str_write(vi_pipe, att_manual->non_dir_str);
    hi_ext_system_demosaic_manual_dtlsmooth_range_write(vi_pipe, att_manual->detail_smooth_range);
    hi_ext_system_demosaic_manual_colornoise_thdf_write(vi_pipe, att_manual->color_noise_ctrl_thdf);
    hi_ext_system_demosaic_manual_colornoise_strf_write(vi_pipe, att_manual->color_noise_ctrl_strf);
    hi_ext_system_demosaic_manual_desat_dark_range_write(vi_pipe, att_manual->color_noise_ctrl_thdy);
    hi_ext_system_demosaic_manual_desat_dark_strength_write(vi_pipe, att_manual->color_noise_ctrl_stry);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) { /* Auto */
        hi_ext_system_demosaic_auto_nddm_str_write(vi_pipe, i, att_auto->non_dir_str[i]);
        hi_ext_system_demosaic_auto_nddm_mfehnc_str_write(vi_pipe, i, att_auto->non_dir_mf_detail_ehc_str[i]);
        hi_ext_system_demosaic_auto_nddm_hfehnc_str_write(vi_pipe, i, att_auto->non_dir_hf_detail_ehc_str[i]);
        hi_ext_system_demosaic_auto_dtlsmooth_range_write(vi_pipe, i, att_auto->detail_smooth_range[i]);
        hi_ext_system_demosaic_auto_colornoise_thdf_write(vi_pipe, i, att_auto->color_noise_ctrl_thdf[i]);
        hi_ext_system_demosaic_auto_colornoise_strf_write(vi_pipe, i, att_auto->color_noise_ctrl_strf[i]);
        hi_ext_system_demosaic_auto_desat_dark_range_write(vi_pipe, i, att_auto->color_noise_ctrl_thdy[i]);
        hi_ext_system_demosaic_auto_desat_dark_strength_write(vi_pipe, i, att_auto->color_noise_ctrl_stry[i]);
    }

    return HI_SUCCESS;
}

hi_void isp_demosaic_set_long_frame_mode(hi_vi_pipe vi_pipe, isp_demosaic *demosaic)
{
    hi_u8          i;
    const hi_s32   sigma_norm_factor = 16;
    hi_float       n_cur, n_pre, n_fct;
    hi_float       drc_compensate = 0.6f;
    const hi_float max_num = 256.0f;
    hi_float       afsigma[HI_ISP_DEMOSAIC_LUT_LENGTH];
    isp_usr_ctx   *isp_ctx  = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (isp_ctx->linkage.fswdr_mode == isp_ctx->linkage.pre_fswdr_mode) {
        return;
    }

    if (is_linear_mode(isp_ctx->sns_wdr_mode) ||
        (isp_ctx->linkage.fswdr_mode == ISP_FSWDR_LONG_FRAME_MODE) ||
        (isp_ctx->linkage.fswdr_mode == ISP_FSWDR_AUTO_LONG_FRAME_MODE)) {
        demosaic_init_fw_linear(vi_pipe, demosaic);

        for (i = 0; i < HI_ISP_DEMOSAIC_LUT_LENGTH; i++) {
            demosaic->sigma_demcfg[i]  = (hi_u32)(i * sigma_norm_factor * isp_bitfix(BITFIX_NUM));
        }
    } else {
        demosaic_init_fw_wdr(vi_pipe, demosaic);

        for (i = 0; i < HI_ISP_DEMOSAIC_LUT_LENGTH; i++) {
            n_cur = (hi_float)(i * sigma_norm_factor);
            n_pre = (hi_float)(max_num * pow(n_cur / max_num, 1.0f / div_0_to_1_float(drc_compensate))) + 0.5f;
            n_fct = (hi_float)(isp_sqr(n_cur / div_0_to_1_float(n_pre)));

            afsigma[i]  =  n_cur * n_fct;
            demosaic->sigma_demcfg[i] = (hi_u32)(afsigma[i] * isp_bitfix(BITFIX_NUM));
        }
    }
}

hi_void isp_demosaic_read_extregs(hi_vi_pipe vi_pipe, isp_demosaic *demosaic)
{
    hi_u8 i;
    hi_isp_demosaic_auto_attr *att_auto = &demosaic->mpi_cfg.auto_attr;
    hi_isp_demosaic_manual_attr *att_manual = &demosaic->mpi_cfg.manual_attr;

    demosaic->dem_attr_update = hi_ext_system_demosaic_attr_update_en_read(vi_pipe);
    hi_ext_system_demosaic_attr_update_en_write(vi_pipe, HI_FALSE);

    if (demosaic->dem_attr_update) {
        demosaic->mpi_cfg.op_type = hi_ext_system_demosaic_manual_mode_read(vi_pipe);

        if (demosaic->mpi_cfg.op_type == OP_TYPE_AUTO) {
            for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
                att_auto->non_dir_str[i] = hi_ext_system_demosaic_auto_nddm_str_read(vi_pipe, i);
                att_auto->non_dir_mf_detail_ehc_str[i] = hi_ext_system_demosaic_auto_nddm_mfehnc_str_read(vi_pipe, i);
                att_auto->non_dir_hf_detail_ehc_str[i] = hi_ext_system_demosaic_auto_nddm_hfehnc_str_read(vi_pipe, i);
                att_auto->detail_smooth_range[i] = hi_ext_system_demosaic_auto_dtlsmooth_range_read(vi_pipe, i);
                att_auto->color_noise_ctrl_thdf[i] = hi_ext_system_demosaic_auto_colornoise_thdf_read(vi_pipe, i);
                att_auto->color_noise_ctrl_strf[i] = hi_ext_system_demosaic_auto_colornoise_strf_read(vi_pipe, i);
                att_auto->color_noise_ctrl_thdy[i] = hi_ext_system_demosaic_auto_desat_dark_range_read(vi_pipe, i);
                att_auto->color_noise_ctrl_stry[i] = hi_ext_system_demosaic_auto_desat_dark_strength_read(vi_pipe, i);
            }
        } else if (demosaic->mpi_cfg.op_type == OP_TYPE_MANUAL) {
            att_manual->non_dir_str = hi_ext_system_demosaic_manual_nddm_str_read(vi_pipe);
            att_manual->non_dir_mf_detail_ehc_str = hi_ext_system_demosaic_manual_nddm_mfehnc_str_read(vi_pipe);
            att_manual->non_dir_hf_detail_ehc_str = hi_ext_system_demosaic_manual_nddm_hfehnc_str_read(vi_pipe);
            att_manual->detail_smooth_range = hi_ext_system_demosaic_manual_dtlsmooth_range_read(vi_pipe);
            att_manual->color_noise_ctrl_thdf = hi_ext_system_demosaic_manual_colornoise_thdf_read(vi_pipe);
            att_manual->color_noise_ctrl_strf = hi_ext_system_demosaic_manual_colornoise_strf_read(vi_pipe);
            att_manual->color_noise_ctrl_thdy = hi_ext_system_demosaic_manual_desat_dark_range_read(vi_pipe);
            att_manual->color_noise_ctrl_stry = hi_ext_system_demosaic_manual_desat_dark_strength_read(vi_pipe);
        }
    }
}

hi_s32 demosaic_proc_write(hi_vi_pipe vi_pipe, hi_isp_ctrl_proc_write *proc, isp_demosaic *demosaic)
{
    hi_isp_ctrl_proc_write proc_tmp;

    if ((proc->proc_buff == HI_NULL) || (proc->buff_len == 0)) {
        return HI_FAILURE;
    }

    proc_tmp.proc_buff = proc->proc_buff;
    proc_tmp.buff_len  = proc->buff_len;

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "-----DEMOSAIC INFO-------------------------------------------------------------\n");

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "%16s" "%16s" "%16s" "%16s" "%16s\n",
                    "enable", "NonDirStr", "NonDirMfStr", "NonDirHfStr", "DeSmthRange");

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "%16u" "%16u" "%16u" "%16u" "%16u\n",
                    demosaic->mpi_cfg.enable, demosaic->actual.non_dir_str, demosaic->actual.non_dir_mf_detail_ehc_str,
                    demosaic->actual.non_dir_hf_detail_ehc_str, demosaic->actual.detail_smooth_range);

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "%16s" "%16s" "%16s" "%16s\n",
                    "ColorNoiseThdF", "ColorNoiseStrF", "ColorNoiseThdY", "ColorNoiseStrY");

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "%16u" "%16u" "%16u" "%16u\n",
                    demosaic->actual.color_noise_ctrl_thdf, demosaic->actual.color_noise_ctrl_strf,
                    demosaic->actual.color_noise_ctrl_thdy, demosaic->actual.color_noise_ctrl_stry);

    proc->write_len += 1;

    return HI_SUCCESS;
}

hi_s32 isp_demosaic_param_init(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg, isp_demosaic *demosaic_ctx)
{
    hi_s32 ret;

    ret = isp_demosaic_ctx_initialize(vi_pipe, demosaic_ctx);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    demosaic_regs_initialize(vi_pipe, reg_cfg);
    ret = demosaic_ext_regs_initialize(vi_pipe, demosaic_ctx);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    demosaic_ctx->init = HI_TRUE;
    return HI_SUCCESS;
}

static hi_s32 isp_demosaic_init(hi_vi_pipe vi_pipe, hi_void *reg_cfg)
{
    hi_s32 ret;
    isp_demosaic *demosaic = HI_NULL;

    ret = demosaic_ctx_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    demosaic_get_ctx(vi_pipe, demosaic);
    isp_check_pointer_return(demosaic);

    return isp_demosaic_param_init(vi_pipe, (isp_reg_cfg *)reg_cfg, demosaic);
}

#define  EPS  0.000001f
#define  COL_ISO      0
#define  COL_K        1
#define  COL_B        2
#define  RECORD_NUM   3
static hi_float demosaic_alpha_lut_convert(hi_float (*record)[RECORD_NUM], hi_u16 record_num, hi_s32 iso)
{
    hi_s32 i;
    hi_float  y_diff, x_diff;
    hi_float k = 0.0f;

    if (record_num > BAYER_CALIBTAION_MAX_NUM) {
        k = record[BAYER_CALIBTAION_MAX_NUM - 1][COL_K];
        return k;
    }

    /* record: iso - k */
    if (iso <= record[0][COL_ISO]) {
        k = record[0][COL_K];
    }

    if (iso >= record[record_num - 1][COL_ISO]) {
        k = record[record_num - 1][COL_K];
    }

    for (i = 0; i < record_num - 1; i++) {
        if (iso >= record[i][COL_ISO] && iso <= record[i + 1][COL_ISO]) {
            x_diff = record[i + 1][COL_ISO] - record[i][COL_ISO];  /* iso diff */
            y_diff = record[i + 1][COL_K]  - record[i][COL_K];     /* k diff */
            k = record[i][COL_K] + y_diff * (iso - record[i][COL_ISO]) / div_0_to_1_float(x_diff + EPS);
        }
    }

    return k;
}

#define GFBLUR_PROCESS_BIT 8
static hi_void demosaic_gf_blur_lut(isp_demosaic *demosaic, gf_blur_lut_cfg *info_cfg, isp_demosaic_dyna_cfg *dm_cfg)
{
    hi_u8    n;
    const hi_u8 shift_sigma1 = 14;
    const hi_u8 shift_sigma2 = 5;
    hi_u8    non_dir_str = info_cfg->non_dir_str;
    hi_s32   bit_scale   = isp_bitfix(demosaic->bit_depth_prc - GFBLUR_PROCESS_BIT);
    hi_u32   max_sigma = 0xffffffff;
    hi_u32   iso = info_cfg->iso;
    hi_u32   alpha, sigma;
    hi_u64   sigma_long;
    hi_vi_pipe  vi_pipe = info_cfg->vi_pipe;
    hi_float calibration_coef;
    hi_isp_cmos_default *sns_dft = HI_NULL;
    isp_sensor_get_default(vi_pipe, &sns_dft);

    calibration_coef = demosaic_alpha_lut_convert(sns_dft->noise_calibration.calibration_coef,
                                                  sns_dft->noise_calibration.calibration_lut_num, iso);
    alpha = (hi_u32)(calibration_coef * isp_bitfix(BITFIX_NUM));

    for (n = 0; n < HI_ISP_DEMOSAIC_LUT_LENGTH; n++) {
        sigma_long  = (hi_u64)demosaic->sigma_demcfg[n] * alpha;
        sigma_long *= non_dir_str;
        sigma  = MIN2((sigma_long >> shift_sigma1), max_sigma);
        sigma  = (hi_u32)sqrt32(sigma);
        sigma = (sigma * bit_scale) >> shift_sigma2;
        dm_cfg->gf_blur_luts[n] = MIN2(sigma, isp_bitmask(demosaic->bit_depth_prc));
    }

    dm_cfg->gf_blur_luts[0] = dm_cfg->gf_blur_luts[1] / 2; /* algorihtm related, [0] must be [1] / 2 */
    dm_cfg->update_gf       = HI_TRUE;

    return;
}

#define RGB_THD_MIN    2
#define RGB_THD_MAX    511
#define TRANSIT_RB_THD 70
#define TRANSIT_G_THD  54
#define PROT_G_THD     24
#define PROT_G_THD_START 48
#define DARK_PROT_BITSHIFT 3
#define DARK_PROT_RATIO_MAX 257
static hi_void demosaic_dark_pro_cfg(isp_demosaic_dyna_cfg *dm_cfg)
{
    hi_u32 data_rb_thd0;
    hi_u32 data_g_thd0;
    hi_u32 data_rb_thd1;
    hi_u32 data_g_thd1;

    if (dm_cfg->color_noise_str_y == 0) {
        dm_cfg->pro_dark_region.ccm_prot_en = 0;
    } else {
        dm_cfg->pro_dark_region.ccm_prot_en = 1;
    }

    dm_cfg->pro_dark_region.cc_prot_ratio  = DARK_PROT_RATIO_MAX - dm_cfg->color_noise_str_y;
    dm_cfg->pro_dark_region.rgb_prot_ratio = DARK_PROT_RATIO_MAX - dm_cfg->color_noise_str_y;

    data_rb_thd0 = dm_cfg->color_noise_thd_y;

    if (dm_cfg->color_noise_thd_y > PROT_G_THD_START) {
        data_g_thd0 = dm_cfg->color_noise_thd_y - PROT_G_THD;
    } else {
        data_g_thd0 = dm_cfg->color_noise_thd_y >> 1;
    }

    data_rb_thd0 = MAX2(data_rb_thd0, RGB_THD_MIN);
    data_g_thd0  = MAX2(data_g_thd0, RGB_THD_MIN);
    data_rb_thd0 = MIN2(data_rb_thd0, RGB_THD_MAX);
    data_g_thd0  = MIN2(data_g_thd0, RGB_THD_MAX);
    data_rb_thd1 = data_rb_thd0 + TRANSIT_RB_THD;
    data_g_thd1  = data_g_thd0 + TRANSIT_G_THD;
    data_rb_thd1 = MIN2(data_rb_thd1, RGB_THD_MAX);
    data_g_thd1  = MIN2(data_g_thd1, RGB_THD_MAX);

    dm_cfg->pro_dark_region.data_cc_thd0 = 0;
    dm_cfg->pro_dark_region.data_cc_thd1 = data_rb_thd0 << DARK_PROT_BITSHIFT;
    dm_cfg->pro_dark_region.data_rr_thd0 = data_rb_thd0 << DARK_PROT_BITSHIFT;
    dm_cfg->pro_dark_region.data_rr_thd1 = data_rb_thd1 << DARK_PROT_BITSHIFT;
    dm_cfg->pro_dark_region.data_gg_thd0 = data_g_thd0  << DARK_PROT_BITSHIFT;
    dm_cfg->pro_dark_region.data_gg_thd1 = data_g_thd1  << DARK_PROT_BITSHIFT;
    dm_cfg->pro_dark_region.data_bb_thd0 = data_rb_thd0 << DARK_PROT_BITSHIFT;
    dm_cfg->pro_dark_region.data_bb_thd1 = data_rb_thd1 << DARK_PROT_BITSHIFT;
    dm_cfg->pro_dark_region.max_rgb_thd  = 0xFFF;
}

static hi_void demosaic_get_iso_info(hi_u32 *iso_low, hi_u32 *iso_up, hi_u8 *idx_up, hi_u8 *idx_lo, hi_u32 input_iso)
{
    hi_u32 inner_idx[2], inner_iso[2]; // inner_idx and inner_iso length is 2

    inner_idx[0] = get_iso_index(input_iso);
    inner_idx[1] = MAX2((hi_s8)inner_idx[0] - 1, 0);
    inner_iso[0] = get_iso(inner_idx[0]);
    inner_iso[1] = get_iso(inner_idx[1]);

    *idx_up  = inner_idx[0];
    *idx_lo  = inner_idx[1];
    *iso_up  = inner_iso[0];
    *iso_low = inner_iso[1];
}

static hi_void demosaic_cfg(isp_demosaic_dyna_cfg *dm_cfg, isp_demosaic *demosaic, hi_u32 iso)
{
    hi_u8  iso_index_upper;
    hi_u8  iso_index_lower;
    hi_u32 iso2;
    hi_u32 iso1;

    /* Calculate iso_index_upper & iso_index_lower by using input iso */
    iso_index_upper = get_iso_index(iso);
    iso_index_lower = MAX2((hi_s8)iso_index_upper - 1, 0);

    /* Calculate iso_index_upper & iso_index_lower by using input iso */
    iso1 = get_iso(iso_index_lower);
    iso2 = get_iso(iso_index_upper);

    dm_cfg->cc_hf_max_ratio     = (hi_u8) linear_inter(iso, iso1, demosaic->lut_cc_hf_max_ratio[iso_index_lower],
                                                       iso2, demosaic->lut_cc_hf_max_ratio[iso_index_upper]);
    dm_cfg->cc_hf_min_ratio     = (hi_u8) linear_inter(iso, iso1, demosaic->lut_cc_hf_min_ratio[iso_index_lower],
                                                       iso2, demosaic->lut_cc_hf_min_ratio[iso_index_upper]);
    dm_cfg->fcr_gf_gain         = (hi_u8) linear_inter(iso, iso1, demosaic->lut_fcr_gf_gain[iso_index_lower],
                                                       iso2, demosaic->lut_fcr_gf_gain[iso_index_upper]);
    dm_cfg->awb_gf_gain_low     = (hi_u8) linear_inter(iso, iso1, demosaic->lut_awb_gfgain_low[iso_index_lower],
                                                       iso2, demosaic->lut_awb_gfgain_low[iso_index_upper]);
    dm_cfg->awb_gf_gain_hig     = (hi_u8) linear_inter(iso, iso1, demosaic->lut_awb_gfgain_hig[iso_index_lower],
                                                       iso2, demosaic->lut_awb_gfgain_hig[iso_index_upper]);
    dm_cfg->awb_gf_gain_max     = (hi_u8) linear_inter(iso, iso1, demosaic->lut_awb_gfgain_max[iso_index_lower],
                                                       iso2, demosaic->lut_awb_gfgain_max[iso_index_upper]);
    dm_cfg->fcr_det_low         = (hi_u16)linear_inter(iso, iso1, demosaic->lut_fcr_det_low[iso_index_lower],
                                                       iso2, demosaic->lut_fcr_det_low[iso_index_upper]);
    dm_cfg->de_sat_low          = (hi_u16)linear_inter(iso, iso1, demosaic->lut_desat_low[iso_index_lower],
                                                       iso2, demosaic->lut_desat_low[iso_index_upper]);
    dm_cfg->de_sat_prot_th      = (hi_u16)linear_inter(iso, iso1, demosaic->lut_desat_prot_th[iso_index_lower],
                                                       iso2, demosaic->lut_desat_prot_th[iso_index_upper]);
    dm_cfg->hf_intp_blur_th_low = (hi_u16)linear_inter(iso, iso1, demosaic->hf_intp_blur_th_low[iso_index_lower],
                                                       iso2, demosaic->hf_intp_blur_th_low[iso_index_upper]);
    dm_cfg->hf_intp_blur_th_hig = (hi_u16)linear_inter(iso, iso1, demosaic->hf_intp_blur_th_hig[iso_index_lower],
                                                       iso2, demosaic->hf_intp_blur_th_hig[iso_index_upper]);
}

static hi_void demosaic_get_dynareg_params(hi_vi_pipe vi_pipe, isp_demosaic_dyna_cfg *dm_cfg, isp_demosaic *demosaic)
{
    hi_u8            idx_up, idx_lo, temp_de_f, filter_coef_index, non_dir_str;
    hi_u32           iso1, iso2, iso;
    isp_usr_ctx     *isp_ctx = HI_NULL;
    gf_blur_lut_cfg  gfblur_info_cfg;
    hi_isp_demosaic_auto_attr *att_auto = &demosaic->mpi_cfg.auto_attr;
    hi_isp_demosaic_manual_attr *att_manual = &demosaic->mpi_cfg.manual_attr;

    isp_get_ctx(vi_pipe, isp_ctx); iso = isp_ctx->linkage.iso;
    demosaic_get_iso_info(&iso1, &iso2, &idx_up, &idx_lo, iso);
    memset_s(&gfblur_info_cfg, sizeof(gf_blur_lut_cfg), 0, sizeof(gf_blur_lut_cfg));

    filter_coef_index = 0; non_dir_str = 0;

    if (demosaic->mpi_cfg.op_type == OP_TYPE_AUTO) {
        dm_cfg->ehc_gray   = (hi_u8)linear_inter(iso, iso1, att_auto->non_dir_mf_detail_ehc_str[idx_lo],
                                                 iso2, att_auto->non_dir_mf_detail_ehc_str[idx_up]);
        dm_cfg->dither_max = (hi_u8)linear_inter(iso, iso1, att_auto->non_dir_hf_detail_ehc_str[idx_lo],
                                                 iso2, att_auto->non_dir_hf_detail_ehc_str[idx_up]);
        dm_cfg->fake_cr_var_thr_low = (hi_u16)linear_inter(iso, iso1, att_auto->color_noise_ctrl_thdf[idx_lo],
                                                           iso2, att_auto->color_noise_ctrl_thdf[idx_up]);

        non_dir_str        = (hi_u8)linear_inter(iso, iso1, att_auto->non_dir_str[idx_lo],
                                                 iso2, att_auto->non_dir_str[idx_up]);
        filter_coef_index  = (hi_u8)linear_inter(iso, iso1, att_auto->detail_smooth_range[idx_lo],
                                                 iso2, att_auto->detail_smooth_range[idx_up]);
        temp_de_f          = (hi_u8)linear_inter(iso, iso1, att_auto->color_noise_ctrl_strf[idx_lo],
                                                 iso2, att_auto->color_noise_ctrl_strf[idx_up]);

        dm_cfg->de_f_color_cr = HI_ISP_DEMOSAIC_COLORNOISE_STR_MAX - temp_de_f;
        dm_cfg->de_f_color_cb = HI_ISP_DEMOSAIC_COLORNOISE_STR_MAX - temp_de_f;

        dm_cfg->color_noise_thd_y = (hi_u8)linear_inter(iso, iso1, att_auto->color_noise_ctrl_thdy[idx_lo],
                                                        iso2, att_auto->color_noise_ctrl_thdy[idx_up]);
        dm_cfg->color_noise_str_y = (hi_u8)linear_inter(iso, iso1, att_auto->color_noise_ctrl_stry[idx_lo],
                                                        iso2, att_auto->color_noise_ctrl_stry[idx_up]);
    } else if (demosaic->mpi_cfg.op_type == OP_TYPE_MANUAL) {
        non_dir_str           = att_manual->non_dir_str;
        filter_coef_index     = att_manual->detail_smooth_range;

        dm_cfg->ehc_gray            = att_manual->non_dir_mf_detail_ehc_str;
        dm_cfg->dither_max          = att_manual->non_dir_hf_detail_ehc_str;
        dm_cfg->fake_cr_var_thr_low = (hi_u16)att_manual->color_noise_ctrl_thdf;

        dm_cfg->de_f_color_cb     = HI_ISP_DEMOSAIC_COLORNOISE_STR_MAX - att_manual->color_noise_ctrl_strf;
        dm_cfg->de_f_color_cr     = HI_ISP_DEMOSAIC_COLORNOISE_STR_MAX - att_manual->color_noise_ctrl_strf;
        dm_cfg->color_noise_thd_y = att_manual->color_noise_ctrl_thdy;
        dm_cfg->color_noise_str_y = att_manual->color_noise_ctrl_stry;
    }

    demosaic_dark_pro_cfg(dm_cfg); demosaic_cfg(dm_cfg, demosaic, iso);

    gfblur_info_cfg.iso = iso; gfblur_info_cfg.vi_pipe = vi_pipe; gfblur_info_cfg.non_dir_str = non_dir_str;
    demosaic_gf_blur_lut(demosaic, &gfblur_info_cfg, dm_cfg);

    filter_coef_index = dm_clip3(filter_coef_index, 1, DEMOSAIC_FILTER_NUM);
    dm_cfg->lpff0 = g_demosaic_filtcoef[filter_coef_index - 1][0]; /* lpff0 */
    dm_cfg->lpff1 = g_demosaic_filtcoef[filter_coef_index - 1][1]; /* lpff1 */
    dm_cfg->lpff2 = g_demosaic_filtcoef[filter_coef_index - 1][2]; /* lpff2 */
    dm_cfg->resh = HI_TRUE;
}

static hi_void demosaic_get_actual_params(hi_vi_pipe vi_pipe, isp_demosaic_dyna_cfg *dm_cfg, isp_demosaic *demosaic)
{
    hi_u8         non_dir_str, filter_coef_index, temp_de_f;
    hi_u8         idx_up, idx_lo;
    hi_u32        iso1, iso2, iso;
    isp_usr_ctx  *isp_ctx = HI_NULL;
    hi_isp_demosaic_auto_attr *att_auto = &demosaic->mpi_cfg.auto_attr;

    isp_get_ctx(vi_pipe, isp_ctx); iso = isp_ctx->linkage.iso;
    demosaic_get_iso_info(&iso1, &iso2, &idx_up, &idx_lo, iso);

    if (demosaic->mpi_cfg.op_type == OP_TYPE_AUTO) {
        non_dir_str       = (hi_u8)linear_inter(iso, iso1, att_auto->non_dir_str[idx_lo],
                                                iso2, att_auto->non_dir_str[idx_up]);
        filter_coef_index = (hi_u8)linear_inter(iso, iso1, att_auto->detail_smooth_range[idx_lo],
                                                iso2, att_auto->detail_smooth_range[idx_up]);
        temp_de_f         = (hi_u8)linear_inter(iso, iso1, att_auto->color_noise_ctrl_strf[idx_lo],
                                                iso2, att_auto->color_noise_ctrl_strf[idx_up]);
    } else {
        non_dir_str       = demosaic->mpi_cfg.manual_attr.non_dir_str;
        filter_coef_index = demosaic->mpi_cfg.manual_attr.detail_smooth_range;
        temp_de_f         = demosaic->mpi_cfg.manual_attr.color_noise_ctrl_strf;
    }

    demosaic->actual.color_noise_ctrl_strf     = temp_de_f;
    demosaic->actual.non_dir_str               = non_dir_str;
    demosaic->actual.detail_smooth_range       = filter_coef_index;
    demosaic->actual.non_dir_mf_detail_ehc_str = dm_cfg->ehc_gray;
    demosaic->actual.non_dir_hf_detail_ehc_str = dm_cfg->dither_max;
    demosaic->actual.color_noise_ctrl_thdf     = dm_cfg->fake_cr_var_thr_low;
    demosaic->actual.color_noise_ctrl_thdy     = dm_cfg->color_noise_thd_y;
    demosaic->actual.color_noise_ctrl_stry     = dm_cfg->color_noise_str_y;
}

hi_void isp_demosaic_reg_update(hi_vi_pipe vi_pipe, isp_reg_cfg *reg, isp_demosaic *demosaic)
{
    hi_u8 i;
    isp_demosaic_dyna_cfg *dm_cfg  = HI_NULL;

    isp_usr_ctx  *isp_ctx = HI_NULL;
    isp_get_ctx(vi_pipe, isp_ctx);

    for (i = 0; i < isp_ctx->block_attr.block_num; i++) {
        dm_cfg = &reg->alg_reg_cfg[i].dem_reg_cfg.dyna_reg_cfg;
        demosaic_get_dynareg_params(vi_pipe, dm_cfg, demosaic);
    }

    demosaic_get_actual_params(vi_pipe, &reg->alg_reg_cfg[0].dem_reg_cfg.dyna_reg_cfg, demosaic);
}

static hi_bool __inline check_demosaic_open(isp_demosaic *demosaic)
{
    return (demosaic->mpi_cfg.enable == HI_TRUE);
}

static hi_s32 isp_demosaic_run(hi_vi_pipe vi_pipe, const hi_void *stat_info, hi_void *reg_cfg, hi_s32 rsv)
{
    hi_u8  i;
    isp_reg_cfg *reg = (isp_reg_cfg *)reg_cfg;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_demosaic *demosaic = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    demosaic_get_ctx(vi_pipe, demosaic);
    isp_check_pointer_return(demosaic);

    /* calculate every two interrupts */
    if (!demosaic->init) {
        return HI_SUCCESS;
    }

    isp_demosaic_set_long_frame_mode(vi_pipe, demosaic);

    if ((isp_ctx->frame_cnt % 2 != 0) && (isp_ctx->linkage.snap_state != HI_TRUE)) { /* proc every 2 frames */
        return HI_SUCCESS;
    }

    demosaic->mpi_cfg.enable = hi_ext_system_demosaic_enable_read(vi_pipe);

    for (i = 0; i < reg->cfg_num; i++) {
        reg->alg_reg_cfg[i].dem_reg_cfg.vhdm_enable = demosaic->mpi_cfg.enable;
        reg->alg_reg_cfg[i].dem_reg_cfg.nddm_enable = demosaic->mpi_cfg.enable;
    }

    reg->cfg_key.bit1_dem_cfg = 1;

    /* check hardware setting */
    if (!check_demosaic_open(demosaic)) {
        return HI_SUCCESS;
    }

    isp_demosaic_read_extregs(vi_pipe, demosaic);

    isp_demosaic_reg_update(vi_pipe, reg, demosaic);

    return HI_SUCCESS;
}

static hi_s32 isp_demosaic_ctrl(hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value)
{
    isp_demosaic *demosaic = HI_NULL;
    isp_reg_cfg_attr *local_reg_cfg = HI_NULL;

    switch (cmd) {
        case ISP_WDR_MODE_SET:
            isp_regcfg_get_ctx(vi_pipe, local_reg_cfg);
            demosaic_get_ctx(vi_pipe, demosaic);
            isp_check_pointer_return(local_reg_cfg);
            isp_check_pointer_return(demosaic);

            demosaic->init = HI_FALSE;
            isp_demosaic_init(vi_pipe, (hi_void *)&local_reg_cfg->reg_cfg);
            break;

        case ISP_PROC_WRITE:
            demosaic_get_ctx(vi_pipe, demosaic);
            isp_check_pointer_return(demosaic);
            demosaic_proc_write(vi_pipe, (hi_isp_ctrl_proc_write *)value, demosaic);
            break;

        default:
            break;
    }

    return HI_SUCCESS;
}

static hi_s32 isp_demosaic_exit(hi_vi_pipe vi_pipe)
{
    hi_u16 i;
    isp_reg_cfg_attr *local_reg_cfg = HI_NULL;

    isp_regcfg_get_ctx(vi_pipe, local_reg_cfg);

    for (i = 0; i < local_reg_cfg->reg_cfg.cfg_num; i++) {
        local_reg_cfg->reg_cfg.alg_reg_cfg[i].dem_reg_cfg.nddm_enable = HI_FALSE;
        local_reg_cfg->reg_cfg.alg_reg_cfg[i].dem_reg_cfg.vhdm_enable = HI_FALSE;
    }

    local_reg_cfg->reg_cfg.cfg_key.bit1_dem_cfg = 1;

    demosaic_ctx_exit(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_alg_register_demosaic(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_alg_node *algs = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_alg_check_return(isp_ctx->alg_key.bit1_demosaic);
    algs = isp_search_alg(isp_ctx->algs);
    isp_check_pointer_return(algs);

    algs->alg_type = ISP_ALG_DEMOSAIC;
    algs->alg_func.pfn_alg_init = isp_demosaic_init;
    algs->alg_func.pfn_alg_run  = isp_demosaic_run;
    algs->alg_func.pfn_alg_ctrl = isp_demosaic_ctrl;
    algs->alg_func.pfn_alg_exit = isp_demosaic_exit;
    algs->used = HI_TRUE;

    return HI_SUCCESS;
}

