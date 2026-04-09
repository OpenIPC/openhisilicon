/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_bayernr.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_bayernr.h"
#include <math.h>
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_proc.h"
#include "isp_math_utils.h"

#define HI_ISP_BAYERNR_BITDEP 16
#define HI_WDR_EINIT_BLCNR    64

static const hi_u16 g_lut_coring_ratio[HI_ISP_BAYERNR_LUT_LENGTH] = {
    60, 60, 60, 60, 65, 65, 65, 65, 70, 70, 70, 70, 70, 70, 70, 70, 80,
    80, 80, 85, 85, 85, 90, 90, 90, 95, 95, 95, 100, 100, 100, 100, 100
};
static const hi_u8 g_lut_fine_str[ISP_AUTO_ISO_STRENGTH_NUM] = {
    70, 70, 70, 50, 48, 37, 28, 24, 20, 20, 20, 16, 16, 16, 16, 16
};
static const hi_u8 g_chroma_str[ISP_BAYER_CHN_NUM][ISP_AUTO_ISO_STRENGTH_NUM] = {
    { 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3 },
    { 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
    { 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
    { 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3 }
};
static const hi_u16 g_lut_coring_wgt[ISP_AUTO_ISO_STRENGTH_NUM] = {
    30, 35, 40, 80, 100, 140, 200, 240, 280, 280, 300, 400, 400, 400, 400, 400
};
static const hi_u16 g_coarse_str[ISP_BAYER_CHN_NUM][ISP_AUTO_ISO_STRENGTH_NUM] = {
    { 120, 120, 120, 120, 120, 120, 120, 140, 160, 160, 180, 200, 200, 200, 200, 200 },
    { 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110 },
    { 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110 },
    { 120, 120, 120, 120, 120, 120, 120, 140, 160, 160, 180, 200, 200, 200, 200, 200 }
};
static const hi_u8 g_wdr_frame_str[WDR_MAX_FRAME_NUM] = { 10, 16, 28 };
static const hi_u8 g_fusion_frame_str[WDR_MAX_FRAME_NUM] = { 26, 16 };

static isp_bayernr *g_bayer_nr_ctx[ISP_MAX_PIPE_NUM] = { HI_NULL };

#define bayernr_get_ctx(dev, ctx) ((ctx) = g_bayer_nr_ctx[dev])
#define bayernr_set_ctx(dev, ctx) (g_bayer_nr_ctx[dev] = (ctx))
#define bayernr_reset_ctx(dev)    (g_bayer_nr_ctx[dev] = HI_NULL)

static hi_s32 bayer_nr_ctx_init(hi_vi_pipe vi_pipe)
{
    isp_bayernr *bayer_nr_ctx = HI_NULL;

    bayernr_get_ctx(vi_pipe, bayer_nr_ctx);

    if (bayer_nr_ctx == HI_NULL) {
        bayer_nr_ctx = (isp_bayernr *)isp_malloc(sizeof(isp_bayernr));
        if (bayer_nr_ctx == HI_NULL) {
            isp_err_trace("isp[%d] bayer_nr_ctx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset_s(bayer_nr_ctx, sizeof(isp_bayernr), 0, sizeof(isp_bayernr));

    bayernr_set_ctx(vi_pipe, bayer_nr_ctx);

    return HI_SUCCESS;
}

static hi_void bayer_nr_ctx_exit(hi_vi_pipe vi_pipe)
{
    isp_bayernr *bayer_nr_ctx = HI_NULL;

    bayernr_get_ctx(vi_pipe, bayer_nr_ctx);
    isp_free(bayer_nr_ctx);
    bayernr_reset_ctx(vi_pipe);
}

static hi_void nr_init_fw(hi_vi_pipe vi_pipe, isp_bayernr *bayernr)
{
    hi_u8 lut_chroma_ratio[ISP_BAYER_CHN_NUM][ISP_AUTO_ISO_STRENGTH_NUM] = {
        { 0, 0, 0, 0, 0, 2, 2, 4, 8, 10, 14, 18, 20, 20, 26, 32 },  /* chroma_ratio_r */
        { 0, 0, 0, 0, 0, 0, 0, 2, 4, 6,  8,  10, 14, 16, 20, 20 },    /* chroma_ratio_gr */
        { 0, 0, 0, 0, 0, 0, 0, 2, 4, 6,  8,  10, 14, 16, 20, 20 },    /* chroma_ratio_gb */
        { 0, 0, 0, 0, 0, 2, 2, 4, 8, 10, 14, 18, 20, 20, 26, 32 }   /* chroma_ratio_b */
    };
    hi_u8 lut_wdr_chroma_ratio[ISP_BAYER_CHN_NUM][ISP_AUTO_ISO_STRENGTH_NUM] = {
        { 0, 0, 0, 2, 4, 6, 8, 9, 10, 10, 10, 10, 10, 10, 10, 10 },  /* chroma_ratio_r */
        { 0, 0, 0, 2, 4, 6, 8, 9, 10, 10, 10, 10, 10, 10, 10, 10 },  /* chroma_ratio_gr */
        { 0, 0, 0, 2, 4, 6, 8, 9, 10, 10, 10, 10, 10, 10, 10, 10 },  /* chroma_ratio_gb */
        { 0, 0, 0, 2, 4, 6, 8, 9, 10, 10, 10, 10, 10, 10, 10, 10 }   /* chroma_ratio_b */
    };
    hi_u8 lut_g_coring_gain[HI_ISP_BAYERNR_LUT_LENGTH] = {
        8, 8, 8, 8, 16, 16, 16, 16, 16, 16, 20, 76, 120, 230, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 16, 16, 16, 16, 16, 16, 16, 16
    };
    hi_u16 lut_center_lmt[ISP_AUTO_ISO_STRENGTH_NUM] = {
        128, 128, 154, 205, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256
    };
    hi_u16 lut_coring_hig[ISP_AUTO_ISO_STRENGTH_NUM] = {
        3200, 3200, 3200, 3200, 3200, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400
    };

    memcpy_s(bayernr->lut_chroma_ratio, sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM * ISP_BAYER_CHN_NUM,
             lut_chroma_ratio, sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM * ISP_BAYER_CHN_NUM);
    memcpy_s(bayernr->lut_wdr_chroma_ratio, sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM * ISP_BAYER_CHN_NUM,
             lut_wdr_chroma_ratio, sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM * ISP_BAYER_CHN_NUM);
    memcpy_s(bayernr->lut_g_coring_gain, sizeof(hi_u8) * HI_ISP_BAYERNR_LUT_LENGTH,
             lut_g_coring_gain, sizeof(hi_u8) * HI_ISP_BAYERNR_LUT_LENGTH);
    memcpy_s(bayernr->lut_center_lmt, sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM,
             lut_center_lmt, sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(bayernr->lut_coring_hig, sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM,
             lut_coring_hig, sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM);
}

static hi_s32 bayernr_check_cmos_param(hi_vi_pipe vi_pipe, const hi_isp_cmos_bayernr *bayer_nr)
{
    hi_u8 i, j;

    isp_check_bool_return(bayer_nr->enable);
    isp_check_bool_return(bayer_nr->bnr_mono_sensor_en);
    isp_check_bool_return(bayer_nr->nr_lsc_enable);

    if (bayer_nr->bnr_lsc_cmp_strength > 0x100) {
        isp_err_trace("invalid bnr_lsc_cmp_strength!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (bayer_nr->bnr_lsc_max_gain > 0xBF) {
        isp_err_trace("invalid bnr_lsc_max_gain!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (bayer_nr->lut_fine_str[i] > 0x80) { /* 128 */
            isp_err_trace("invalid lut_fine_str[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (bayer_nr->lut_coring_wgt[i] > 0xC80) { /* 3200 */
            isp_err_trace("invalid coring_wgt[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    for (i = 0; i < HI_ISP_BAYERNR_LUT_LENGTH; i++) {
        if (bayer_nr->lut_coring_ratio[i] > 0x3ff) {
            isp_err_trace("invalid coring_ratio[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
            if (bayer_nr->chroma_str[j][i] > 0x3) { /* 3 */
                isp_err_trace("invalid chroma_str[%d][%d]!\n", j, i);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }

            if (bayer_nr->coarse_str[j][i] > 0x360) {
                isp_err_trace("invalid coarse_str[%d][%d]!\n", j, i);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
        }
    }

    for (i = 0; i < WDR_MAX_FRAME_NUM; i++) {
        if (bayer_nr->wdr_frame_str[i] > 0x50) { /* 80 */
            isp_err_trace("invalid wdr_frame_str[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (bayer_nr->fusion_frame_str[i] > 0x50) { /* 80 */
            isp_err_trace("invalid fusion_frame_str[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    return HI_SUCCESS;
}

static hi_s32 bayernr_ext_regs_initialize(hi_vi_pipe vi_pipe, isp_bayernr *bayernr)
{
    hi_u8 i;
    hi_s32 ret;
    hi_isp_nr_auto_attr *autom = &bayernr->mpi_cfg.auto_attr;
    hi_isp_cmos_default *sns_dft = HI_NULL;

    isp_sensor_get_default(vi_pipe, &sns_dft);

    nr_init_fw(vi_pipe, bayernr);

    hi_ext_system_bayernr_manual_mode_write(vi_pipe, HI_EXT_SYSTEM_BAYERNR_MANU_MODE_DEFAULT);

    /* manual */
    bayernr->mpi_cfg.manual_attr.fine_str = HI_EXT_SYSTEM_BAYERNR_MANU_FINE_STRENGTH_DEFAULT;
    bayernr->mpi_cfg.manual_attr.coring_wgt = HI_EXT_SYSTEM_BAYERNR_MANU_CORING_WEIGHT_DEFAULT;

    hi_ext_system_bayernr_manual_fine_strength_write(vi_pipe, bayernr->mpi_cfg.manual_attr.fine_str);
    hi_ext_system_bayernr_manual_coring_weight_write(vi_pipe, bayernr->mpi_cfg.manual_attr.coring_wgt);

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        bayernr->mpi_cfg.manual_attr.chroma_str[i] = HI_EXT_SYSTEM_BAYERNR_MANU_CHROMA_STRENGTH_DEFAULT;
        bayernr->mpi_cfg.manual_attr.coarse_str[i] = HI_EXT_SYSTEM_BAYERNR_MANU_COARSE_STRENGTH_DEFAULT;

        hi_ext_system_bayernr_manual_chroma_strength_write(vi_pipe, i, bayernr->mpi_cfg.manual_attr.chroma_str[i]);
        hi_ext_system_bayernr_manual_coarse_strength_write(vi_pipe, i, bayernr->mpi_cfg.manual_attr.coarse_str[i]);
    }

    if (sns_dft->key.bit1_bayer_nr) {
        isp_check_pointer_return(sns_dft->bayer_nr);

        ret = bayernr_check_cmos_param(vi_pipe, sns_dft->bayer_nr);
        if (ret != HI_SUCCESS) {
            return ret;
        }

        hi_ext_system_bayernr_enable_write(vi_pipe, sns_dft->bayer_nr->enable);
        hi_ext_system_bayernr_lsc_enable_write(vi_pipe, sns_dft->bayer_nr->nr_lsc_enable);
        hi_ext_system_bayernr_lsc_max_gain_write(vi_pipe, sns_dft->bayer_nr->bnr_lsc_max_gain);
        hi_ext_system_bayernr_lsc_cmp_strength_write(vi_pipe, sns_dft->bayer_nr->bnr_lsc_cmp_strength);
        hi_ext_system_bayernr_mono_sensor_write(vi_pipe, sns_dft->bayer_nr->bnr_mono_sensor_en);

        memcpy_s(bayernr->mpi_cfg.coring_ratio, HI_ISP_BAYERNR_LUT_LENGTH * sizeof(hi_u16),
                 sns_dft->bayer_nr->lut_coring_ratio, HI_ISP_BAYERNR_LUT_LENGTH * sizeof(hi_u16));
        memcpy_s(autom->fine_str, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
                 sns_dft->bayer_nr->lut_fine_str, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
        memcpy_s(autom->chroma_str, ISP_BAYER_CHN_NUM * ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
                 sns_dft->bayer_nr->chroma_str, ISP_BAYER_CHN_NUM * ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
        memcpy_s(autom->coarse_str, ISP_BAYER_CHN_NUM * ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16),
                 sns_dft->bayer_nr->coarse_str, ISP_BAYER_CHN_NUM * ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16));
        memcpy_s(autom->coring_wgt, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16),
                 sns_dft->bayer_nr->lut_coring_wgt, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16));
        memcpy_s(bayernr->mpi_cfg.wdr_attr.wdr_frame_str, WDR_MAX_FRAME_NUM * sizeof(hi_u8),
                 sns_dft->bayer_nr->wdr_frame_str, WDR_MAX_FRAME_NUM * sizeof(hi_u8));
        memcpy_s(bayernr->mpi_cfg.wdr_attr.fusion_frame_str, WDR_MAX_FRAME_NUM * sizeof(hi_u8),
                 sns_dft->bayer_nr->fusion_frame_str, WDR_MAX_FRAME_NUM * sizeof(hi_u8));
    } else {
        hi_ext_system_bayernr_enable_write(vi_pipe, HI_EXT_SYSTEM_BAYERNR_ENABLE_DEFAULT);
        hi_ext_system_bayernr_lsc_enable_write(vi_pipe, HI_EXT_SYSTEM_BAYERNR_LSC_ENABLE_DEFAULT);
        hi_ext_system_bayernr_lsc_max_gain_write(vi_pipe, HI_EXT_SYSTEM_BAYERNR_LSC_MAX_GAIN_DEFAULT);
        hi_ext_system_bayernr_lsc_cmp_strength_write(vi_pipe, HI_EXT_SYSTEM_BAYERNR_LSC_CMP_STRENGTH_DEFAULT);
        hi_ext_system_bayernr_mono_sensor_write(vi_pipe, HI_EXT_SYSTEM_BAYERNR_MONO_SENSOR_ENABLE_DEFAULT);

        memcpy_s(bayernr->mpi_cfg.coring_ratio, HI_ISP_BAYERNR_LUT_LENGTH * sizeof(hi_u16),
                 g_lut_coring_ratio, HI_ISP_BAYERNR_LUT_LENGTH * sizeof(hi_u16));
        memcpy_s(autom->fine_str, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
                 g_lut_fine_str, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
        memcpy_s(autom->chroma_str, ISP_BAYER_CHN_NUM * ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
                 g_chroma_str, ISP_BAYER_CHN_NUM * ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
        memcpy_s(autom->coarse_str, ISP_BAYER_CHN_NUM * ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16),
                 g_coarse_str, ISP_BAYER_CHN_NUM * ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16));
        memcpy_s(autom->coring_wgt, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16),
                 g_lut_coring_wgt, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16));
        memcpy_s(bayernr->mpi_cfg.wdr_attr.wdr_frame_str, WDR_MAX_FRAME_NUM * sizeof(hi_u8),
                 g_wdr_frame_str, WDR_MAX_FRAME_NUM * sizeof(hi_u8));
        memcpy_s(bayernr->mpi_cfg.wdr_attr.fusion_frame_str, WDR_MAX_FRAME_NUM * sizeof(hi_u8),
                 g_fusion_frame_str, WDR_MAX_FRAME_NUM * sizeof(hi_u8));
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {  /* autom */
        hi_ext_system_bayernr_auto_fine_strength_write(vi_pipe, i, autom->fine_str[i]);
        hi_ext_system_bayernr_auto_chroma_strength_r_write(vi_pipe, i, autom->chroma_str[BAYER_RGGB][i]);
        hi_ext_system_bayernr_auto_chroma_strength_gr_write(vi_pipe, i, autom->chroma_str[BAYER_GRBG][i]);
        hi_ext_system_bayernr_auto_chroma_strength_gb_write(vi_pipe, i, autom->chroma_str[BAYER_GBRG][i]);
        hi_ext_system_bayernr_auto_chroma_strength_b_write(vi_pipe, i, autom->chroma_str[BAYER_BGGR][i]);
        hi_ext_system_bayernr_auto_coarse_strength_r_write(vi_pipe, i, autom->coarse_str[BAYER_RGGB][i]);
        hi_ext_system_bayernr_auto_coarse_strength_gr_write(vi_pipe, i, autom->coarse_str[BAYER_GRBG][i]);
        hi_ext_system_bayernr_auto_coarse_strength_gb_write(vi_pipe, i, autom->coarse_str[BAYER_GBRG][i]);
        hi_ext_system_bayernr_auto_coarse_strength_b_write(vi_pipe, i, autom->coarse_str[BAYER_BGGR][i]);
        hi_ext_system_bayernr_auto_coring_weight_write(vi_pipe, i, autom->coring_wgt[i]);
    }

    for (i = 0; i < WDR_MAX_FRAME_NUM; i++) {
        hi_ext_system_bayernr_wdr_frame_strength_write(vi_pipe, i, bayernr->mpi_cfg.wdr_attr.wdr_frame_str[i]);
        hi_ext_system_bayernr_fusion_frame_strength_write(vi_pipe, i, bayernr->mpi_cfg.wdr_attr.fusion_frame_str[i]);
    }

    for (i = 0; i < HI_ISP_BAYERNR_LUT_LENGTH; i++) {
        hi_ext_system_bayernr_coring_ratio_write(vi_pipe, i, bayernr->mpi_cfg.coring_ratio[i]);
    }

    bayernr->init = HI_TRUE;

    return HI_SUCCESS;
}

static hi_void bayernr_static_regs_initialize(hi_vi_pipe vi_pipe, isp_bayernr_static_cfg *bayernr_static_reg_cfg,
                                              hi_u8 i)
{
    bayernr_static_reg_cfg->jnlm_sel = HI_ISP_BNR_DEFAULT_JNLM_SEL;
    bayernr_static_reg_cfg->sad_win_size_sel = HI_ISP_BNR_DEFAULT_SAD_WINSIZE_SEL;
    bayernr_static_reg_cfg->jnlm_max_wt_coef = HI_ISP_BNR_DEFAULT_JNLM_MAX_WT_COEF;
    bayernr_static_reg_cfg->resh = HI_TRUE;
}

static hi_void bayernr_dyna_regs_initialize(hi_vi_pipe vi_pipe, isp_bayernr_dyna_cfg *bayernr_dyna_reg_cfg,
                                            isp_usr_ctx *isp_ctx, isp_bayernr *bayernr)
{
    hi_u16 j;

    bayernr_dyna_reg_cfg->medc_enable = HI_TRUE;
    bayernr_dyna_reg_cfg->wdr_mode_en = HI_FALSE;
    bayernr_dyna_reg_cfg->wdr_fusion_en = HI_FALSE;
    bayernr_dyna_reg_cfg->center_wgt_en = HI_FALSE;
    bayernr_dyna_reg_cfg->wdr_map_flt_mode = HI_ISP_BNR_DEFAULT_LINEAR_WDRMAP_FLTMODE;
    bayernr_dyna_reg_cfg->c_ratio[BAYER_RGGB] = HI_ISP_BNR_DEFAULT_C_RATIO_R;
    bayernr_dyna_reg_cfg->c_ratio[BAYER_GRBG] = HI_ISP_BNR_DEFAULT_C_RATIO_GR;
    bayernr_dyna_reg_cfg->c_ratio[BAYER_GBRG] = HI_ISP_BNR_DEFAULT_C_RATIO_GB;
    bayernr_dyna_reg_cfg->c_ratio[BAYER_BGGR] = HI_ISP_BNR_DEFAULT_C_RATIO_B;
    bayernr_dyna_reg_cfg->amed_mode[BAYER_RGGB] = HI_ISP_BNR_DEFAULT_AMED_MODE_R;
    bayernr_dyna_reg_cfg->amed_mode[BAYER_GRBG] = HI_ISP_BNR_DEFAULT_AMED_MODE_GR;
    bayernr_dyna_reg_cfg->amed_mode[BAYER_GBRG] = HI_ISP_BNR_DEFAULT_AMED_MODE_GB;
    bayernr_dyna_reg_cfg->amed_mode[BAYER_BGGR] = HI_ISP_BNR_DEFAULT_AMED_MODE_B;
    bayernr_dyna_reg_cfg->amed_level[BAYER_RGGB] = HI_ISP_BNR_DEFAULT_AMED_LEVEL_R;
    bayernr_dyna_reg_cfg->amed_level[BAYER_GRBG] = HI_ISP_BNR_DEFAULT_AMED_LEVEL_GR;
    bayernr_dyna_reg_cfg->amed_level[BAYER_GBRG] = HI_ISP_BNR_DEFAULT_AMED_LEVEL_GB;
    bayernr_dyna_reg_cfg->amed_level[BAYER_BGGR] = HI_ISP_BNR_DEFAULT_AMED_LEVEL_B;
    bayernr_dyna_reg_cfg->jnlm_gain = HI_ISP_BNR_DEFAULT_JNLM_GAIN;
    bayernr_dyna_reg_cfg->jnlm_coring_hig = HI_ISP_BNR_DEFAULT_JNLM_CORING_HIGH;
    bayernr_dyna_reg_cfg->shot_ratio = HI_ISP_BNR_DEFAULT_SHOT_RATIO;
    bayernr_dyna_reg_cfg->r_lmt_rgain = HI_ISP_BNR_DEFAULT_RLMT_RGAIN;
    bayernr_dyna_reg_cfg->r_lmt_bgain = HI_ISP_BNR_DEFAULT_RLMT_BGAIN;
    bayernr_dyna_reg_cfg->edge_det_thr = HI_ISP_BNR_DEFAULT_EDGE_DET_THRESHOLD_LINEAR;
    bayernr->edge_det_thr = HI_ISP_BNR_DEFAULT_EDGE_DET_THRESHOLD_LINEAR;
    bayernr->wdr_map_gain = HI_ISP_BNR_DEFAULT_LINEAR_WDRMAP_GAIN;

    for (j = 0; j < HI_ISP_BAYERNR_LMTLUTNUM; j++) {
        bayernr_dyna_reg_cfg->jnlm_limit_lut[j] = 0;
        bayernr_dyna_reg_cfg->jnlm_limit_s_lut[j] = 0;
    }
    for (j = 0; j < HI_ISP_BAYERNR_LUT_LENGTH; j++) {
        bayernr_dyna_reg_cfg->jnlm_coring_low_lut[j] = 0;
        bayernr_dyna_reg_cfg->g_coring_gain_lut[j] = 16; /* 16 */
    }

    for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
        bayernr_dyna_reg_cfg->jnlm_limit_mult_gain[j] = 0;
        bayernr_dyna_reg_cfg->jnlm_limit_mult_gain_s[j] = 0;
    }
    bayernr_dyna_reg_cfg->bnr_lut_update_en = HI_TRUE;

    if ((hi_ext_system_wdr_en_read(vi_pipe) == HI_TRUE) && is_2to1_wdr_mode(isp_ctx->sns_wdr_mode)) {
        bayernr->wdr_frames_merge = 2; /* 2 */
        bayernr->expo_values[0]   = 64; /* 64 */
        bayernr->wdr_map_gain                  = HI_ISP_BNR_DEFAULT_WDR_WDRMAP_GAIN;
        bayernr_dyna_reg_cfg->wdr_map_gain     = HI_ISP_BNR_DEFAULT_WDR_WDRMAP_GAIN;
        bayernr_dyna_reg_cfg->wdr_map_flt_mode = HI_ISP_BNR_DEFAULT_WDR_WDRMAP_FLTMODE;
        bayernr_dyna_reg_cfg->jnlm_sym_coef    = HI_ISP_BNR_DEFAULT_JNLM_SYMCOEF_WDR;
        bayernr_dyna_reg_cfg->edge_det_thr     = HI_ISP_BNR_DEFAULT_EDGE_DET_THRESHOLD_WDR;
        bayernr->edge_det_thr                  = HI_ISP_BNR_DEFAULT_EDGE_DET_THRESHOLD_WDR;
    } else {
        bayernr->wdr_frames_merge           = 1;
        bayernr->wdr_blc_thr                = 0;
        bayernr_dyna_reg_cfg->jnlm_sym_coef = HI_ISP_BNR_DEFAULT_JNLM_SYMCOEF_LINEAR;
        bayernr_dyna_reg_cfg->edge_det_thr  = HI_ISP_BNR_DEFAULT_EDGE_DET_THRESHOLD_LINEAR;
        bayernr->edge_det_thr               = HI_ISP_BNR_DEFAULT_EDGE_DET_THRESHOLD_LINEAR;
    }

    bayernr_dyna_reg_cfg->resh = HI_TRUE;
}

static hi_void bayernr_usr_regs_initialize(isp_bayernr_usr_cfg *usr_reg_cfg, hi_isp_cmos_default *sns_dft)
{
    isp_check_pointer_void_return(sns_dft->bayer_nr);

    usr_reg_cfg->bnr_lsc_en = (sns_dft->bayer_nr->nr_lsc_enable == 0) ? 0 : 3; /* 0 3 */
    usr_reg_cfg->bnr_mono_sensor_en = sns_dft->bayer_nr->bnr_mono_sensor_en;
    usr_reg_cfg->bnr_lsc_max_gain = sns_dft->bayer_nr->bnr_lsc_max_gain + 64; /* 64 */
    usr_reg_cfg->bnr_lsc_cmp_strength = sns_dft->bayer_nr->bnr_lsc_cmp_strength;
    usr_reg_cfg->resh = HI_TRUE;
}

static hi_void bayernr_regs_initialize(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg, isp_bayernr *bayernr)
{
    hi_u8 i;
    isp_usr_ctx *isp_ctx = HI_NULL;
    hi_isp_cmos_default *sns_dft = HI_NULL;
    hi_isp_cmos_black_level *sns_black_level = HI_NULL;
    isp_bayernr_static_cfg *bayernr_static_reg_cfg = HI_NULL;
    isp_bayernr_dyna_cfg *bayernr_dyna_reg_cfg = HI_NULL;
    isp_bayernr_usr_cfg *bayernr_usr_reg_cfg = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_sensor_get_default(vi_pipe, &sns_dft);
    isp_sensor_get_blc(vi_pipe, &sns_black_level);

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        bayernr_static_reg_cfg = &reg_cfg->alg_reg_cfg[i].bnr_reg_cfg.static_reg_cfg;
        bayernr_dyna_reg_cfg = &reg_cfg->alg_reg_cfg[i].bnr_reg_cfg.dyna_reg_cfg;
        bayernr_usr_reg_cfg = &reg_cfg->alg_reg_cfg[i].bnr_reg_cfg.usr_reg_cfg;

        reg_cfg->alg_reg_cfg[i].bnr_reg_cfg.bnr_enable = HI_TRUE;
        reg_cfg->alg_reg_cfg[i].de_reg_cfg.kernel_reg_cfg.bnr_enable = HI_TRUE;
        bayernr_static_regs_initialize(vi_pipe, bayernr_static_reg_cfg, i);
        bayernr_dyna_regs_initialize(vi_pipe, bayernr_dyna_reg_cfg, isp_ctx, bayernr);
        bayernr_usr_regs_initialize(bayernr_usr_reg_cfg, sns_dft);
    }

    reg_cfg->cfg_key.bit1_bayernr_cfg = 1;
}

hi_void isp_bayernr_read_extregs(hi_vi_pipe vi_pipe, isp_be_blc_cfg *be_blc_cfg, isp_bayernr *bayernr)
{
    hi_u8 i;
    hi_u32 exp_ratio[3] = { 0, 0, 0 }; /* 3 0 */
    isp_usr_ctx *isp_ctx = HI_NULL;
    hi_isp_nr_auto_attr   *autom  = &bayernr->mpi_cfg.auto_attr;
    hi_isp_nr_manual_attr *manual = &bayernr->mpi_cfg.manual_attr;
    isp_get_ctx(vi_pipe, isp_ctx);

    bayernr->mpi_cfg.op_type = hi_ext_system_bayernr_manual_mode_read(vi_pipe);
    bayernr->mpi_cfg.nr_lsc_enable = hi_ext_system_bayernr_lsc_enable_read(vi_pipe);
    bayernr->bnr_mono_sensor_en = hi_ext_system_bayernr_mono_sensor_read(vi_pipe);
    bayernr->wdr_fusion_en = hi_ext_system_fusion_mode_read(vi_pipe);
    bayernr->mpi_cfg.bnr_lsc_max_gain = hi_ext_system_bayernr_lsc_max_gain_read(vi_pipe);
    bayernr->mpi_cfg.bnr_lsc_cmp_strength = hi_ext_system_bayernr_lsc_cmp_strength_read(vi_pipe);

    bayernr->mpi_cfg.nr_lsc_ratio = hi_ext_system_bayernr_lsc_nr_ratio_read(vi_pipe);

    for (i = 0; i < HI_ISP_BAYERNR_LUT_LENGTH; i++) {
        bayernr->mpi_cfg.coring_ratio[i] = hi_ext_system_bayernr_coring_ratio_read(vi_pipe, i);
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        autom->fine_str[i] = hi_ext_system_bayernr_auto_fine_strength_read(vi_pipe, i);
        autom->coring_wgt[i] = hi_ext_system_bayernr_auto_coring_weight_read(vi_pipe, i);
        autom->chroma_str[BAYER_RGGB][i] = hi_ext_system_bayernr_auto_chroma_strength_r_read(vi_pipe, i);
        autom->chroma_str[BAYER_GRBG][i] = hi_ext_system_bayernr_auto_chroma_strength_gr_read(vi_pipe, i);
        autom->chroma_str[BAYER_GBRG][i] = hi_ext_system_bayernr_auto_chroma_strength_gb_read(vi_pipe, i);
        autom->chroma_str[BAYER_BGGR][i] = hi_ext_system_bayernr_auto_chroma_strength_b_read(vi_pipe, i);
        autom->coarse_str[BAYER_RGGB][i] = hi_ext_system_bayernr_auto_coarse_strength_r_read(vi_pipe, i);
        autom->coarse_str[BAYER_GRBG][i] = hi_ext_system_bayernr_auto_coarse_strength_gr_read(vi_pipe, i);
        autom->coarse_str[BAYER_GBRG][i] = hi_ext_system_bayernr_auto_coarse_strength_gb_read(vi_pipe, i);
        autom->coarse_str[BAYER_BGGR][i] = hi_ext_system_bayernr_auto_coarse_strength_b_read(vi_pipe, i);
    }

    manual->fine_str = hi_ext_system_bayernr_manual_fine_strength_read(vi_pipe);
    manual->coring_wgt = hi_ext_system_bayernr_manual_coring_weight_read(vi_pipe);
    manual->chroma_str[BAYER_RGGB] = hi_ext_system_bayernr_manual_chroma_strength_read(vi_pipe, BAYER_RGGB);
    manual->chroma_str[BAYER_GRBG] = hi_ext_system_bayernr_manual_chroma_strength_read(vi_pipe, BAYER_GRBG);
    manual->chroma_str[BAYER_GBRG] = hi_ext_system_bayernr_manual_chroma_strength_read(vi_pipe, BAYER_GBRG);
    manual->chroma_str[BAYER_BGGR] = hi_ext_system_bayernr_manual_chroma_strength_read(vi_pipe, BAYER_BGGR);
    manual->coarse_str[BAYER_RGGB] = hi_ext_system_bayernr_manual_coarse_strength_read(vi_pipe, BAYER_RGGB);
    manual->coarse_str[BAYER_GRBG] = hi_ext_system_bayernr_manual_coarse_strength_read(vi_pipe, BAYER_GRBG);
    manual->coarse_str[BAYER_GBRG] = hi_ext_system_bayernr_manual_coarse_strength_read(vi_pipe, BAYER_GBRG);
    manual->coarse_str[BAYER_BGGR] = hi_ext_system_bayernr_manual_coarse_strength_read(vi_pipe, BAYER_BGGR);

    for (i = 0; i < WDR_MAX_FRAME_NUM; i++) {
        bayernr->mpi_cfg.wdr_attr.wdr_frame_str[i] = hi_ext_system_bayernr_wdr_frame_strength_read(vi_pipe, i);
        bayernr->mpi_cfg.wdr_attr.fusion_frame_str[i] = hi_ext_system_bayernr_fusion_frame_strength_read(vi_pipe, i);
    }

    memcpy_s(exp_ratio, 3 * sizeof(hi_u32), isp_ctx->linkage.exp_ratio_lut, 3 * sizeof(hi_u32)); /* 3 */

    if (is_2to1_wdr_mode(isp_ctx->sns_wdr_mode)) {
        bayernr->expo_values[1] = exp_ratio[0];
        bayernr->wdr_blc_thr = be_blc_cfg->dyna_blc.wdr_blc[0].out_blc;
    }
}

hi_s32 isp_bayernr_read_pro_mode(hi_vi_pipe vi_pipe, isp_bayernr *bayernr)
{
    hi_u8 i;
    hi_u8 index = 0;
    hi_u8 index_max_value = 0;
    isp_usr_ctx *isp_ctx = HI_NULL;
    hi_isp_nr_auto_attr *autom = &bayernr->mpi_cfg.auto_attr;

    isp_check_pointer_return(bayernr);
    isp_get_ctx(vi_pipe, isp_ctx);

    if (isp_ctx->pro_nr_param_ctrl.pro_nr_param->enable == HI_TRUE) {
        index = isp_ctx->linkage.pro_index;
        index_max_value = MIN2(isp_ctx->pro_shp_param_ctrl.pro_shp_param->param_num, PRO_MAX_FRAME_NUM);
        if (index > index_max_value) {
            index = index_max_value;
        }

        if (index < 1) {
            return HI_SUCCESS;
        }
        index -= 1;
    } else {
        return HI_SUCCESS;
    }
    bayernr->mpi_cfg.op_type = OP_TYPE_AUTO;
    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        autom->fine_str[i] = isp_ctx->pro_nr_param_ctrl.pro_nr_param->nr_attr[index].fine_str[i];
        autom->coring_wgt[i] = isp_ctx->pro_nr_param_ctrl.pro_nr_param->nr_attr[index].coring_wgt[i];
        autom->chroma_str[BAYER_RGGB][i] =
            isp_ctx->pro_nr_param_ctrl.pro_nr_param->nr_attr[index].chroma_str[BAYER_RGGB][i];
        autom->chroma_str[BAYER_GRBG][i] =
            isp_ctx->pro_nr_param_ctrl.pro_nr_param->nr_attr[index].chroma_str[BAYER_GRBG][i];
        autom->chroma_str[BAYER_GBRG][i] =
            isp_ctx->pro_nr_param_ctrl.pro_nr_param->nr_attr[index].chroma_str[BAYER_GBRG][i];
        autom->chroma_str[BAYER_BGGR][i] =
            isp_ctx->pro_nr_param_ctrl.pro_nr_param->nr_attr[index].chroma_str[BAYER_BGGR][i];
        autom->coarse_str[BAYER_RGGB][i] =
            isp_ctx->pro_nr_param_ctrl.pro_nr_param->nr_attr[index].coarse_str[BAYER_RGGB][i];
        autom->coarse_str[BAYER_GRBG][i] =
            isp_ctx->pro_nr_param_ctrl.pro_nr_param->nr_attr[index].coarse_str[BAYER_GRBG][i];
        autom->coarse_str[BAYER_GBRG][i] =
            isp_ctx->pro_nr_param_ctrl.pro_nr_param->nr_attr[index].coarse_str[BAYER_GBRG][i];
        autom->coarse_str[BAYER_BGGR][i] =
            isp_ctx->pro_nr_param_ctrl.pro_nr_param->nr_attr[index].coarse_str[BAYER_BGGR][i];
    }
    return HI_SUCCESS;
}

hi_s32 bayernr_proc_write(hi_vi_pipe vi_pipe, hi_isp_ctrl_proc_write *proc, isp_bayernr *bayernr)
{
    hi_isp_ctrl_proc_write proc_tmp;

    if ((proc->proc_buff == HI_NULL) || (proc->buff_len == 0)) {
        return HI_FAILURE;
    }

    proc_tmp.proc_buff = proc->proc_buff;
    proc_tmp.buff_len = proc->buff_len;

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "-----BAYERNR INFO----------------------------------------------------------------------------\n");

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "%16s" "%16s" "%16s" "%16s" "%16s" "%16s\n",
                    "Enable", "NrLscEnable", "CoarseStr0", "CoarseStr1", "CoarseStr2", "CoarseStr3");

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "%16u" "%16u" "%16u" "%16u" "%16u" "%16u\n",
                    bayernr->mpi_cfg.enable,
                    (hi_u16)bayernr->mpi_cfg.nr_lsc_enable,
                    (hi_u16)bayernr->lmt_strength[BAYER_RGGB],
                    (hi_u16)bayernr->lmt_strength[BAYER_GRBG],
                    (hi_u16)bayernr->lmt_strength[BAYER_GBRG],
                    (hi_u16)bayernr->lmt_strength[BAYER_BGGR]);

    proc->write_len += 1;

    return HI_SUCCESS;
}

hi_s32 isp_bayernr_param_init(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg, isp_bayernr *bayernr)
{
    hi_s32 ret;
    ret = bayernr_ext_regs_initialize(vi_pipe, bayernr);
    if (ret != HI_SUCCESS) {
        return ret;
    }
    bayernr_regs_initialize(vi_pipe, reg_cfg, bayernr);

    return HI_SUCCESS;
}

static hi_s32 isp_bayernr_init(hi_vi_pipe vi_pipe, hi_void *reg_cfg)
{
    hi_s32 ret;
    isp_bayernr *bayernr = HI_NULL;

    ret = bayer_nr_ctx_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    bayernr_get_ctx(vi_pipe, bayernr);
    isp_check_pointer_return(bayernr);

    return isp_bayernr_param_init(vi_pipe, (isp_reg_cfg *)reg_cfg, bayernr);
}

static hi_u16 nr_get_value_from_lut_fix(hi_u32 x, hi_u32 *lut_x, hi_u16 *lut_y, hi_u32 length)
{
    hi_s32 j;

    if (x <= lut_x[0]) {
        return lut_y[0];
    }
    for (j = 1; j < length; j++) {
        if (x <= lut_x[j]) {
            if (lut_y[j] < lut_y[j - 1]) {
                return (hi_u16)(lut_y[j - 1] - (lut_y[j - 1] - lut_y[j]) * (hi_u16)(x - lut_x[j - 1]) \
                    / div_0_to_1 ((hi_u16)(lut_x[j] - lut_x[j - 1])));
            } else {
                return (hi_u16)(lut_y[j - 1] + (lut_y[j] - lut_y[j - 1]) * (hi_u16)(x - lut_x[j - 1]) \
                    / div_0_to_1 ((hi_u16)(lut_x[j] - lut_x[j - 1])));
            }
        }
    }

    return lut_y[length - 1];
}

#define BNR_EPS     0.000001f
#define BNR_COL_ISO 0
#define BNR_COL_K   1
#define BNR_COL_B   2

static hi_float bayernr_get_kfrom_noise_lut(hi_float (*record)[3], hi_u16 record_num, hi_s32 iso) /* 3 */
{
    hi_s32 i = 0;
    hi_float y_diff = 0;
    hi_float x_diff = 0;
    hi_float k = 0.0f;

    if (record_num > BAYER_CALIBTAION_MAX_NUM) {
        k = record[BAYER_CALIBTAION_MAX_NUM - 1][BNR_COL_K];
        return k;
    }

    /* record: iso - k */
    if (iso <= record[0][BNR_COL_ISO]) {
        k = record[0][BNR_COL_K];
    }

    if (iso >= record[record_num - 1][BNR_COL_ISO]) {
        k = record[record_num - 1][BNR_COL_K];
    }

    for (i = 0; i < record_num - 1; i++) {
        if (iso >= record[i][BNR_COL_ISO] && iso <= record[i + 1][BNR_COL_ISO]) {
            x_diff = record[i + 1][BNR_COL_ISO] - record[i][BNR_COL_ISO];  /* iso diff */
            y_diff = record[i + 1][BNR_COL_K] - record[i][BNR_COL_K];      /* k diff */
            k = record[i][BNR_COL_K] + y_diff * (iso - record[i][BNR_COL_ISO]) / div_0_to_1_float(x_diff + BNR_EPS);
        }
    }

    return k;
}

static hi_float bayernr_get_bfrom_noise_lut(hi_float (*record)[3], hi_u16 record_num, hi_s32 iso) /* 3 */
{
    hi_s32 i;
    hi_float y_diff, x_diff;
    hi_float b;

    i = 0;
    x_diff = 0;
    y_diff = 0;
    b = 0.0f;

    if (record_num > BAYER_CALIBTAION_MAX_NUM) {
        b = record[BAYER_CALIBTAION_MAX_NUM - 1][BNR_COL_B];
        return b;
    }
    /* record: iso - b */
    if (iso <= record[0][BNR_COL_ISO]) {
        b = record[0][BNR_COL_B];
    }

    if (iso >= record[record_num - 1][BNR_COL_ISO]) {
        b = record[record_num - 1][BNR_COL_B];
    }

    for (i = 0; i < record_num - 1; i++) {
        if (iso >= record[i][BNR_COL_ISO] && iso <= record[i + 1][BNR_COL_ISO]) {
            x_diff = record[i + 1][BNR_COL_ISO] - record[i][BNR_COL_ISO];  /* iso diff */
            y_diff = record[i + 1][BNR_COL_B] - record[i][BNR_COL_B];      /* k diff */
            b = record[i][BNR_COL_B] + y_diff * (iso - record[i][BNR_COL_ISO]) / div_0_to_1_float(x_diff + BNR_EPS);
        }
    }
    return b;
}

static hi_u32 nr_sqrt64(hi_u64 arg)
{
    hi_u64 mask = (hi_u64)1 << 31; /* 1 31 */
    hi_u32 res = 0;
    hi_u32 i = 0;

    for (i = 0; i < 32; i++) { /* 0 32 */
        if ((res + (mask >> i)) * (res + (mask >> i)) <= arg) {
            res = res + (mask >> i);
        }
    }

    /* rounding */
    if ((hi_u64)res * res + res < arg) {
        ++res;
    }

    return res;
}

static void nr_cfg(hi_vi_pipe vi_pipe, isp_bayernr_dyna_cfg *dyna_cfg, isp_bayernr *bayernr, isp_bnr_iso iso_struc)
{
    hi_u8 max_c_raio, max_c_raio01, max_c_raio23;
    isp_usr_ctx *isp_ctx = HI_NULL;

    hi_u8 idx_high, idx_low;
    hi_u32 iso, iso1, iso2;

    idx_high = iso_struc.iso_index_upper;
    idx_low = iso_struc.iso_index_lower;
    iso = iso_struc.iso;
    iso1 = iso_struc.iso1;
    iso2 = iso_struc.iso2;

    isp_get_ctx(vi_pipe, isp_ctx);

    if ((bayernr->wdr_frames_merge == 1) ||
        (isp_ctx->linkage.fswdr_mode == ISP_FSWDR_LONG_FRAME_MODE) ||
        (isp_ctx->linkage.fswdr_mode == ISP_FSWDR_AUTO_LONG_FRAME_MODE)) {
        dyna_cfg->c_ratio[BAYER_RGGB] = (hi_u8)linear_inter(iso, iso1, bayernr->lut_chroma_ratio[BAYER_RGGB][idx_low],
            iso2, bayernr->lut_chroma_ratio[BAYER_RGGB][idx_high]);

        dyna_cfg->c_ratio[BAYER_GRBG] = (hi_u8)linear_inter(iso, iso1, bayernr->lut_chroma_ratio[BAYER_GRBG][idx_low],
            iso2, bayernr->lut_chroma_ratio[BAYER_GRBG][idx_high]);

        dyna_cfg->c_ratio[BAYER_GBRG] = (hi_u8)linear_inter(iso, iso1, bayernr->lut_chroma_ratio[BAYER_GBRG][idx_low],
            iso2, bayernr->lut_chroma_ratio[BAYER_GBRG][idx_high]);

        dyna_cfg->c_ratio[BAYER_BGGR] = (hi_u8)linear_inter(iso, iso1, bayernr->lut_chroma_ratio[BAYER_BGGR][idx_low],
            iso2, bayernr->lut_chroma_ratio[BAYER_BGGR][idx_high]);
        bayernr->wdr_map_gain      = HI_ISP_BNR_DEFAULT_LINEAR_WDRMAP_GAIN;
        dyna_cfg->wdr_map_gain     = HI_ISP_BNR_DEFAULT_LINEAR_WDRMAP_GAIN;
        dyna_cfg->wdr_map_flt_mode = HI_ISP_BNR_DEFAULT_LINEAR_WDRMAP_FLTMODE;
        dyna_cfg->jnlm_sym_coef    = HI_ISP_BNR_DEFAULT_JNLM_SYMCOEF_LINEAR;
        dyna_cfg->edge_det_thr     = HI_ISP_BNR_DEFAULT_EDGE_DET_THRESHOLD_LINEAR;
        bayernr->edge_det_thr      = HI_ISP_BNR_DEFAULT_EDGE_DET_THRESHOLD_LINEAR;
    } else {
        dyna_cfg->c_ratio[BAYER_RGGB] = (hi_u8)linear_inter(iso, iso1,
            bayernr->lut_wdr_chroma_ratio[BAYER_RGGB][idx_low],
            iso2, bayernr->lut_wdr_chroma_ratio[BAYER_RGGB][idx_high]);

        dyna_cfg->c_ratio[BAYER_GRBG] = (hi_u8)linear_inter(iso, iso1,
            bayernr->lut_wdr_chroma_ratio[BAYER_GRBG][idx_low],
            iso2, bayernr->lut_wdr_chroma_ratio[BAYER_GRBG][idx_high]);

        dyna_cfg->c_ratio[BAYER_GBRG] = (hi_u8)linear_inter(iso, iso1,
            bayernr->lut_wdr_chroma_ratio[BAYER_GBRG][idx_low],
            iso2, bayernr->lut_wdr_chroma_ratio[BAYER_GBRG][idx_high]);

        dyna_cfg->c_ratio[BAYER_BGGR] = (hi_u8)linear_inter(iso, iso1,
            bayernr->lut_wdr_chroma_ratio[BAYER_BGGR][idx_low],
            iso2, bayernr->lut_wdr_chroma_ratio[BAYER_BGGR][idx_high]);
    }

    bayernr->center_lmt = (hi_u16)linear_inter(iso, iso1, bayernr->lut_center_lmt[idx_low],
                                               iso2, bayernr->lut_center_lmt[idx_high]);
    dyna_cfg->jnlm_coring_hig = (hi_u16)linear_inter(iso, iso1, bayernr->lut_coring_hig[idx_low],
                                                     iso2, bayernr->lut_coring_hig[idx_high]);
    dyna_cfg->jnlm_coring_hig = (hi_u16)(256 * ((hi_float)dyna_cfg->jnlm_coring_hig /* 256 */
                                        / (hi_float)HI_ISP_BAYERNR_STRENGTH_DIVISOR));

    dyna_cfg->amed_mode[BAYER_RGGB] = (iso < 5000) ? 0 : 1; /* 5000 1 */
    dyna_cfg->amed_mode[BAYER_GRBG] = 0;
    dyna_cfg->amed_mode[BAYER_GBRG] = 0;
    dyna_cfg->amed_mode[BAYER_BGGR] = (iso < 5000) ? 0 : 1; /* 5000 1 */

    max_c_raio01 = MAX2(dyna_cfg->c_ratio[BAYER_RGGB], dyna_cfg->c_ratio[BAYER_GRBG]);
    max_c_raio23 = MAX2(dyna_cfg->c_ratio[BAYER_GBRG], dyna_cfg->c_ratio[BAYER_BGGR]);
    max_c_raio = MAX2(max_c_raio01, max_c_raio23);
    if (max_c_raio <= 4) { /* 4 */
        dyna_cfg->medc_enable = HI_FALSE;
    } else {
        dyna_cfg->medc_enable = HI_TRUE;
    }
}

static void nr_ext_cfg(hi_vi_pipe vi_pipe, isp_bayernr_dyna_cfg *dyna_cfg, isp_bayernr *bayernr,
                       isp_bnr_iso iso_struc)
{
    hi_u32 i = 0;
    hi_u32 coring_low = 1;
    hi_u8 idx_high, idx_low;
    hi_u32 iso, iso1, iso2;
    hi_isp_nr_auto_attr *autom = &bayernr->mpi_cfg.auto_attr;

    idx_high = iso_struc.iso_index_upper;
    idx_low = iso_struc.iso_index_lower;
    iso = iso_struc.iso;
    iso1 = iso_struc.iso1;
    iso2 = iso_struc.iso2;

    if (bayernr->mpi_cfg.op_type == OP_TYPE_AUTO) {
        bayernr->coring_low = (hi_u16)linear_inter(iso, iso1, autom->coring_wgt[idx_low],
                                                   iso2, autom->coring_wgt[idx_high]);

        coring_low = 256 * (hi_u32)bayernr->coring_low; /* 256 */

        dyna_cfg->jnlm_gain = (hi_u8)linear_inter(iso, iso1, autom->fine_str[idx_low], iso2, autom->fine_str[idx_high]);

        bayernr->fine_str = dyna_cfg->jnlm_gain;

        dyna_cfg->amed_level[BAYER_RGGB] = (hi_u8)linear_inter(iso, iso1, autom->chroma_str[BAYER_RGGB][idx_low],
            iso2, autom->chroma_str[BAYER_RGGB][idx_high]);

        dyna_cfg->amed_level[BAYER_GRBG] = (hi_u8)linear_inter(iso, iso1, autom->chroma_str[BAYER_GRBG][idx_low],
            iso2, autom->chroma_str[BAYER_GRBG][idx_high]);

        dyna_cfg->amed_level[BAYER_GBRG] = (hi_u8)linear_inter(iso, iso1, autom->chroma_str[BAYER_GBRG][idx_low],
            iso2, autom->chroma_str[BAYER_GBRG][idx_high]);

        dyna_cfg->amed_level[BAYER_BGGR] = (hi_u8)linear_inter(iso, iso1, autom->chroma_str[BAYER_BGGR][idx_low],
            iso2, autom->chroma_str[BAYER_BGGR][idx_high]);

        bayernr->lmt_strength[BAYER_RGGB] = (hi_u16)linear_inter(iso, iso1, autom->coarse_str[BAYER_RGGB][idx_low],
            iso2, autom->coarse_str[BAYER_RGGB][idx_high]);

        bayernr->lmt_strength[BAYER_GRBG] = (hi_u16)linear_inter(iso, iso1, autom->coarse_str[BAYER_GRBG][idx_low],
            iso2, autom->coarse_str[BAYER_GRBG][idx_high]);

        bayernr->lmt_strength[BAYER_GBRG] = (hi_u16)linear_inter(iso, iso1, autom->coarse_str[BAYER_GBRG][idx_low],
            iso2, autom->coarse_str[BAYER_GBRG][idx_high]);

        bayernr->lmt_strength[BAYER_BGGR] = (hi_u16)linear_inter(iso, iso1, autom->coarse_str[BAYER_BGGR][idx_low],
            iso2, autom->coarse_str[BAYER_BGGR][idx_high]);
    } else if (bayernr->mpi_cfg.op_type == OP_TYPE_MANUAL) {
        bayernr->coring_low = bayernr->mpi_cfg.manual_attr.coring_wgt;
        coring_low = 256 * (hi_u32)bayernr->coring_low; /* 256 */
        dyna_cfg->jnlm_gain = bayernr->mpi_cfg.manual_attr.fine_str;
        dyna_cfg->amed_level[BAYER_RGGB] = bayernr->mpi_cfg.manual_attr.chroma_str[BAYER_RGGB];
        dyna_cfg->amed_level[BAYER_GRBG] = bayernr->mpi_cfg.manual_attr.chroma_str[BAYER_GRBG];
        dyna_cfg->amed_level[BAYER_GBRG] = bayernr->mpi_cfg.manual_attr.chroma_str[BAYER_GBRG];
        dyna_cfg->amed_level[BAYER_BGGR] = bayernr->mpi_cfg.manual_attr.chroma_str[BAYER_BGGR];

        bayernr->lmt_strength[BAYER_RGGB] = bayernr->mpi_cfg.manual_attr.coarse_str[BAYER_RGGB];
        bayernr->lmt_strength[BAYER_GRBG] = bayernr->mpi_cfg.manual_attr.coarse_str[BAYER_GRBG];
        bayernr->lmt_strength[BAYER_GBRG] = bayernr->mpi_cfg.manual_attr.coarse_str[BAYER_GBRG];
        bayernr->lmt_strength[BAYER_BGGR] = bayernr->mpi_cfg.manual_attr.coarse_str[BAYER_BGGR];
    }

    for (i = 0; i < HI_ISP_BAYERNR_LUT_LENGTH; i++) {
        dyna_cfg->jnlm_coring_low_lut[i] = (hi_u16)(bayernr->mpi_cfg.coring_ratio[i] * coring_low /
                                                         HI_ISP_BAYERNR_CORINGLOW_STRENGTH_DIVISOR);
        dyna_cfg->jnlm_coring_low_lut[i] = MIN2(0x3fff, dyna_cfg->jnlm_coring_low_lut[i]);
        dyna_cfg->g_coring_gain_lut[i] = bayernr->lut_g_coring_gain[i];
    }
}

static hi_void nr_multi_gain_wdr(isp_bayernr_dyna_cfg *dyna_cfg, isp_bayernr *bayernr,
                                 hi_u8 sad_fac, hi_u16 jnlm_scale)
{
    hi_u8 i;
    hi_u32 mult_gain_l, mult_gain;

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        mult_gain_l = ((hi_u32)bayernr->lmt_np_thresh * bayernr->lmt_strength[i] * sad_fac) >> 7; /* 7 */
        mult_gain = ((hi_u32)bayernr->lmt_np_thresh_s * bayernr->lmt_strength[i] * sad_fac) >> 7; /* 7 */

        mult_gain_l = (mult_gain_l * jnlm_scale) >> 7; /* 7 */
        mult_gain = (mult_gain * jnlm_scale) >> 7; /* 7 */

        dyna_cfg->jnlm_limit_mult_gain[i] = MIN2(mult_gain_l >> 4, 0x01fffff);  /* 4 */
        dyna_cfg->jnlm_limit_mult_gain_s[i] = MIN2(mult_gain >> 4, 0x01fffff);  /* 4 */
    }
}

static hi_void nr_multi_gain_linear(isp_bayernr_dyna_cfg *dyna_cfg, isp_bayernr *bayernr,
                                    hi_u8 sad_fac, hi_u16 jnlm_scale)
{
    hi_u8 i;

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        dyna_cfg->jnlm_limit_mult_gain[i] = (bayernr->lmt_np_thresh * bayernr->lmt_strength[i] * sad_fac) \
            >> 7; /* 7 */
        dyna_cfg->jnlm_limit_mult_gain[i] = (dyna_cfg->jnlm_limit_mult_gain[i] * jnlm_scale) >> 7; /* 7 */
        dyna_cfg->jnlm_limit_mult_gain[i] = MIN2(dyna_cfg->jnlm_limit_mult_gain[i], 0x01ffffff);
        dyna_cfg->jnlm_limit_mult_gain[i] = dyna_cfg->jnlm_limit_mult_gain[i] >> 4;  /* u21.0 4 */
        dyna_cfg->jnlm_limit_mult_gain_s[i] = dyna_cfg->jnlm_limit_mult_gain[i];
    }
}

static hi_void nr_multi_gain_cfg(hi_vi_pipe vi_pipe, isp_bayernr_dyna_cfg *dyna_cfg, isp_bayernr *bayernr)
{
    hi_u8 sad_fac = 25;
    hi_u16 jnlm_scale = 49;
    hi_u16 jnlm_shot_scale;
    hi_u16 shot_coef = 2;
    hi_u32 center_lmt_ratio = 40;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    jnlm_shot_scale = 128 + CLIP3((jnlm_scale * shot_coef), 0, 255); /* 128 255 */
    jnlm_scale = jnlm_scale + 128; /* 128 */

    if ((bayernr->wdr_mode_en == HI_FALSE) ||
        (isp_ctx->linkage.fswdr_mode == ISP_FSWDR_LONG_FRAME_MODE) ||
        (isp_ctx->linkage.fswdr_mode == ISP_FSWDR_AUTO_LONG_FRAME_MODE)) {
        nr_multi_gain_linear(dyna_cfg, bayernr, sad_fac, jnlm_scale);
    } else {
        nr_multi_gain_wdr(dyna_cfg, bayernr, sad_fac, jnlm_scale);
    }

    dyna_cfg->edge_det_thr = (bayernr->edge_det_thr * sad_fac) >> 5; /* 5 */

    if (bayernr->center_wgt_en == HI_TRUE) {
        center_lmt_ratio = center_lmt_ratio * (hi_u32)bayernr->center_lmt;
        dyna_cfg->shot_ratio = center_lmt_ratio >> 4; /* 4 */
    } else {
        dyna_cfg->shot_ratio = jnlm_shot_scale * 256 / div_0_to_1(jnlm_scale);  /* u2.8 256 */
    }
    dyna_cfg->shot_ratio = MIN2(dyna_cfg->shot_ratio, 1023); /* 1023 */
}

static hi_void nr_limit_lut(hi_vi_pipe vi_pipe, isp_bayernr_dyna_cfg *dyna_cfg, isp_bayernr *bayernr, hi_u32 iso,
                            hi_u16 black_level)
{
    hi_u16 str;
    hi_u32 lmt_np_thresh;
    hi_u32 i;
    hi_u32 n = 0;
    hi_u16 bit_mask = ((1 << (HI_ISP_BAYERNR_BITDEP - 1)) - 1);
    hi_u32 lut_n[2] = { 16, 45 }; /* 2 */
    hi_u32 k, b;
    hi_u32 sigma, sigma_max;
    hi_u16 dark_strength = 230;       /* 1.8f*128 */
    hi_u16 lut_str[2] = { 96, 128 };  /* {0.75f, 1.0f}*128 */
    hi_float f_calibration_coef;

    hi_isp_cmos_default *sns_dft = HI_NULL;
    isp_sensor_get_default(vi_pipe, &sns_dft);

    f_calibration_coef = bayernr_get_kfrom_noise_lut(sns_dft->noise_calibration.calibration_coef,
                                                     sns_dft->noise_calibration.calibration_lut_num, iso);
    k = (hi_u32)(f_calibration_coef * isp_bitfix(14)); /* 14 */
    f_calibration_coef = bayernr_get_bfrom_noise_lut(sns_dft->noise_calibration.calibration_coef,
                                                     sns_dft->noise_calibration.calibration_lut_num, iso);
    b = (hi_u32)(f_calibration_coef * isp_bitfix(14)); /* 14 */
    sigma_max = (hi_u32)(k * (hi_u32)MAX2 (((hi_s32)(255 - (black_level >> 4))), 0) + b); /* 255 4 */
    sigma_max = (hi_u32)sqrt32(sigma_max);

    lmt_np_thresh = (hi_u32)(sigma_max * (1 << (HI_ISP_BAYERNR_BITDEP - 8 - 7)));  /* sad win size, move to hw 1 8 7 */
    bayernr->lmt_np_thresh = (hi_u16)((lmt_np_thresh > bit_mask) ? bit_mask : lmt_np_thresh);

    lut_str[0] = dark_strength;

    for (i = 0; i < HI_ISP_BAYERNR_LMTLUTNUM; i++) {
        sigma = (hi_u32)(k * (hi_u32)MAX2 ((((hi_s32)(i * 255 - 128 * (black_level >> 4))) /* 255 128 4 */
                         / (hi_s32)128), 0) + b); /* 128 0 */
        sigma = (hi_u32)sqrt32(sigma);
        str = nr_get_value_from_lut_fix(2 * i, lut_n, lut_str, 2); /* 2 */
        sigma = sigma * str;

        dyna_cfg->jnlm_limit_lut[i] = (hi_u8)((sigma + sigma_max / 2) / div_0_to_1(sigma_max)); /* 2 */
    }

    /* copy the first non-zero value to its left side */
    for (i = 0; i < HI_ISP_BAYERNR_LMTLUTNUM; i++) {
        if (dyna_cfg->jnlm_limit_lut[i] > 0) {
            n = i;
            break;
        }
    }

    for (i = 0; i < n; i++) {
        dyna_cfg->jnlm_limit_lut[i] = dyna_cfg->jnlm_limit_lut[n];
    }

    for (i = 0; i < HI_ISP_BAYERNR_LMTLUTNUM; i++) {
        dyna_cfg->jnlm_limit_s_lut[i] = dyna_cfg->jnlm_limit_lut[i];
    }
}

static hi_s32 hiisp_bayernr_fw(hi_u32 iso, hi_u32 sensor_iso, hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg,
                               isp_bayernr *bayernr)
{
    isp_bnr_iso iso_struc;
    static hi_u32 rgain_iir = 65536;
    static hi_u32 bgain_iir = 65536;

    hi_isp_cmos_default *sns_dft = HI_NULL;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_bayernr_dyna_cfg *dyna_cfg = HI_NULL;
    isp_bayernr_usr_cfg *usr_cfg = HI_NULL;

    dyna_cfg = &reg_cfg->alg_reg_cfg[0].bnr_reg_cfg.dyna_reg_cfg;
    usr_cfg  = &reg_cfg->alg_reg_cfg[0].bnr_reg_cfg.usr_reg_cfg;

    isp_sensor_get_default(vi_pipe, &sns_dft);
    isp_get_ctx(vi_pipe, isp_ctx);

    usr_cfg->bnr_mono_sensor_en = bayernr->bnr_mono_sensor_en;  /* mono_sensor, waiting to get */
    usr_cfg->bnr_lsc_en = (bayernr->mpi_cfg.nr_lsc_enable == 0) ? 0 : 3; /* 0 3 */
    usr_cfg->bnr_lsc_max_gain = bayernr->mpi_cfg.bnr_lsc_max_gain + 64; /* 64 */
    usr_cfg->bnr_lsc_cmp_strength = bayernr->mpi_cfg.bnr_lsc_cmp_strength;

    iso_struc.iso = iso;
    iso_struc.iso_index_upper = get_iso_index(iso_struc.iso);
    iso_struc.iso_index_lower = MAX2((hi_s8)iso_struc.iso_index_upper - 1, 0);
    iso_struc.iso1 = get_iso(iso_struc.iso_index_lower);
    iso_struc.iso2 = get_iso(iso_struc.iso_index_upper);

    nr_limit_lut(vi_pipe, dyna_cfg, bayernr, sensor_iso, reg_cfg->alg_reg_cfg[0].be_blc_cfg.dyna_blc.bnr_blc.blc[0]);
    nr_cfg(vi_pipe, dyna_cfg, bayernr, iso_struc);
    nr_ext_cfg(vi_pipe, dyna_cfg, bayernr, iso_struc);
    nr_multi_gain_cfg(vi_pipe, dyna_cfg, bayernr);

    if (usr_cfg->bnr_mono_sensor_en == HI_TRUE) {
        dyna_cfg->c_ratio[BAYER_RGGB] = HI_FALSE;
        dyna_cfg->c_ratio[BAYER_GRBG] = HI_FALSE;
        dyna_cfg->c_ratio[BAYER_GBRG] = HI_FALSE;
        dyna_cfg->c_ratio[BAYER_BGGR] = HI_FALSE;
        dyna_cfg->amed_level[BAYER_RGGB] = HI_FALSE;
        dyna_cfg->amed_level[BAYER_GRBG] = HI_FALSE;
        dyna_cfg->amed_level[BAYER_GBRG] = HI_FALSE;
        dyna_cfg->amed_level[BAYER_BGGR] = HI_FALSE;
        dyna_cfg->medc_enable = HI_FALSE;
    }

    dyna_cfg->bnr_lut_update_en = HI_TRUE;

    rgain_iir = (7 * rgain_iir + isp_ctx->linkage.white_balance_gain[0]) >> 3; /* 7 0 3 */
    bgain_iir = (7 * bgain_iir + isp_ctx->linkage.white_balance_gain[3]) >> 3; /* 7 3 */

    dyna_cfg->r_lmt_rgain = (rgain_iir + 128) >> 8; /* 128 8 */
    dyna_cfg->r_lmt_bgain = (bgain_iir + 128) >> 8; /* 128 8 */
    dyna_cfg->r_lmt_rgain = (dyna_cfg->r_lmt_rgain > 1023) ? 1023 : dyna_cfg->r_lmt_rgain; /* 1023 */
    dyna_cfg->r_lmt_bgain = (dyna_cfg->r_lmt_bgain > 1023) ? 1023 : dyna_cfg->r_lmt_bgain; /* 1023 */

    dyna_cfg->resh = HI_TRUE;
    usr_cfg->resh = HI_TRUE;

    return HI_SUCCESS;
}

/* WDR FW: ADJ_C(2) + ADJ_D(4) = 6 */
#define ADJ_C 2
#define ADJ_D 4

/* 16bit -> 20bit */
static hi_u32 bdec(hi_u64 y)
{
    hi_u64 out = (y << 26) / div_0_to_1((y << 6) + (((1 << 16) - y) << (ADJ_D + 6))); /* 26 6 16 */
    return (hi_u32)out;
}

static hi_void nr_limit_lut_wdr2to1(isp_bayernr_dyna_cfg *dyna_cfg, isp_bayernr *bayernr, hi_u32 k,
                                    hi_u32 b, hi_u16 black_level)
{
    hi_u32 i;
    hi_u16 bit_mask = ((1 << (HI_ISP_BAYERNR_BITDEP - 1)) - 1);
    hi_u64 u64Pvb = 0;
    hi_u32 u32Pvb = 0;
    hi_u16 wdr_frame_thr[WDR_MAX_FRAME_NUM + 2]; /* 2 */
    hi_u16 wdr_frame_thr_diff, sigma_l_max, sigma_s_max;
    hi_u32 nrm_gain, bcom_gain, pvb_long, pvb_short;
    hi_u32 sigma_l, sigma_s;
    hi_u16 sigma_l_arr[HI_ISP_BAYERNR_LMTLUTNUM], sigma_s_arr[HI_ISP_BAYERNR_LMTLUTNUM];
    hi_u8 strength;

    sigma_l_max = 0;
    sigma_s_max = 0;

    bayernr->expo_values[1] = (bayernr->expo_values[1] == 0) ? 64 : bayernr->expo_values[1]; /* 64 */

    wdr_frame_thr[0] = isp_bitfix(4) * MAX2(0, (bayernr->wdr_frame_thr[0] - black_level));  /* low 4 */
    wdr_frame_thr[1] = isp_bitfix(4) * MAX2(0, (bayernr->wdr_frame_thr[1] - black_level));  /* high 4 */

    nrm_gain = (hi_u64)(isp_bitfix(20) - bayernr->wdr_blc_thr) * isp_bitfix(12) /* 20 12 */
                        / div_0_to_1((isp_bitfix(14) - (hi_u32)black_level) * bayernr->expo_values[1]); /* 14 */

    for (i = 0; i < HI_ISP_BAYERNR_LMTLUTNUM; i++) {
        u64Pvb = i * 512; /* 512 */
        u32Pvb = (hi_u32)bdec(u64Pvb);

        if (i == 0) {
            bcom_gain = 128; /* 128 */
        } else {
            bcom_gain = u64Pvb * isp_bitfix(7) / div_0_to_1(u32Pvb); /* 7 */
        }

        u32Pvb = MAX2(((hi_s32)u32Pvb - bayernr->wdr_blc_thr), 0);
        pvb_long = u32Pvb * isp_bitfix(8) / div_0_to_1(nrm_gain); /* 8 */
        pvb_short = (hi_u32)((hi_u64)u32Pvb * isp_bitfix(14) /* 14 */
                            / div_0_to_1(nrm_gain * bayernr->expo_values[1]));

        sigma_l = (hi_u32)sqrt32(pvb_long * k / isp_bitfix(8) + b) * 64; /* 8 64 */
        sigma_s = (hi_u32)sqrt32(pvb_short * k / isp_bitfix(8) + b) * bayernr->expo_values[1]; /* 8 */

        sigma_l = (hi_u16)MIN2(0xffff, ((hi_u64)sigma_l * nrm_gain * bcom_gain * bcom_gain / isp_bitfix(27))); /* 27 */
        sigma_s = (hi_u16)MIN2(0xffff, ((hi_u64)sigma_s * nrm_gain * bcom_gain * bcom_gain / isp_bitfix(27))); /* 27 */

        if (pvb_long < wdr_frame_thr[0]) {
            strength = bayernr->mpi_cfg.wdr_attr.wdr_frame_str[2]; /* 2 */
        } else if (pvb_long > wdr_frame_thr[1]) {
            strength = bayernr->mpi_cfg.wdr_attr.wdr_frame_str[0];
        } else {
            wdr_frame_thr_diff = wdr_frame_thr[1] - wdr_frame_thr[0];
            if (wdr_frame_thr_diff < 25) { /* 25 */
                strength = bayernr->mpi_cfg.wdr_attr.wdr_frame_str[0];
            } else {
                strength = ((wdr_frame_thr[1] - pvb_long) * bayernr->mpi_cfg.wdr_attr.wdr_frame_str[2] + /* 1 2 */
                            (pvb_long - wdr_frame_thr[0]) * bayernr->mpi_cfg.wdr_attr.wdr_frame_str[0]) /
                           wdr_frame_thr_diff;
            }
        }

        sigma_l = MIN2(65535, sigma_l * bayernr->mpi_cfg.wdr_attr.wdr_frame_str[1] / isp_bitfix(2)); /* 65535 1 2 */
        sigma_s = MIN2(65535, sigma_s * strength / isp_bitfix(2)); /* 65535 2 */
        sigma_l_max = (sigma_l_max < sigma_l) ? sigma_l : sigma_l_max;
        sigma_s_max = (sigma_s_max < sigma_s) ? sigma_s : sigma_s_max;

        sigma_l_arr[i] = sigma_l;
        sigma_s_arr[i] = sigma_s;
    }

    bayernr->lmt_np_thresh = ((sigma_l_max / 4) > bit_mask) ? bit_mask : (sigma_l_max / 4); /* 4 */
    bayernr->lmt_np_thresh_s = ((sigma_s_max / 4) > bit_mask) ? bit_mask : (sigma_s_max / 4); /* 4 */

    for (i = 0; i < HI_ISP_BAYERNR_LMTLUTNUM; i++) {
        dyna_cfg->jnlm_limit_lut[i] = (hi_u8)MIN2(0xff,
            ((hi_u64)sigma_l_arr[i] * 128 + sigma_l_max / 2) / div_0_to_1(sigma_l_max)); /* 128 2 */
        dyna_cfg->jnlm_limit_s_lut[i] = (hi_u8)MIN2(0xff,
            ((hi_u64)sigma_s_arr[i] * 128 + sigma_s_max / 2) / div_0_to_1((hi_u32)sigma_s_max)); /* 128 2 */
    }

    dyna_cfg->center_wgt_en = HI_FALSE;
}

static hi_s32 nr_limit_lut_fusion(isp_bayernr_dyna_cfg *dyna_cfg, isp_bayernr *bayernr, hi_u32 k, hi_u32 b,
                                  hi_u16 black_level)
{
    hi_u32 i;
    hi_u16 bit_mask = ((1 << (HI_ISP_BAYERNR_BITDEP - 1)) - 1);
    hi_u64 u64Pvb = 0;
    hi_u32 u32Pvb = 0;
    hi_u32 nrm_gain, bcom_gain;
    hi_u32 pvb_long, pvb_short;
    hi_u16 sigma_m_max = 0;
    hi_u16 sigma_s_max = 0;
    hi_u32 sigma_l, sigma_m, sigma_s;
    hi_u16 sigma_m_arr[HI_ISP_BAYERNR_LMTLUTNUM], sigma_s_arr[HI_ISP_BAYERNR_LMTLUTNUM];

    bayernr->expo_values[1] = (bayernr->expo_values[1] == 0) ? 64 : bayernr->expo_values[1]; /* 64 */
    nrm_gain = (hi_u64)(isp_bitfix(20) - bayernr->wdr_blc_thr) * isp_bitfix(12) /* 20 12 */
                / div_0_to_1((isp_bitfix(14) - (hi_u32)black_level) * (bayernr->expo_values[1] + 64)); /* 14 64 */

    for (i = 0; i < HI_ISP_BAYERNR_LMTLUTNUM; i++) {
        u64Pvb = i * 512; /* 512 */
        u32Pvb = (hi_u32)bdec(u64Pvb);

        if (i == 0) {
            bcom_gain = 128; /* 128 */
        } else {
            bcom_gain = u64Pvb * isp_bitfix(7) / u32Pvb; /* 7 */
        }

        u32Pvb = MAX2(((hi_s32)u32Pvb - bayernr->wdr_blc_thr), 0);
        pvb_long = (hi_u32)MIN2(0xffffffff,
            (hi_u64)u32Pvb * bayernr->expo_values[1] * isp_bitfix(8) / div_0_to_1(nrm_gain * /* 8 */
            (bayernr->expo_values[1] + 64))); /* 64 */

        pvb_short = (hi_u32)MIN2(0xffffffff,
            (hi_u64)u32Pvb * isp_bitfix(14) / div_0_to_1(nrm_gain * (bayernr->expo_values[1] + 64))); /* 14 64 */

        sigma_l = (hi_u32)sqrt32(pvb_long * k / isp_bitfix(8) + b) * 64; /* 8 64 */
        sigma_s = (hi_u32)sqrt32(pvb_short * k / isp_bitfix(8) + b) * 64; /* 8 64 */
        sigma_m = nr_sqrt64((hi_u64)sigma_l * sigma_l + (hi_u64)sigma_s * sigma_s);  /* long+short */
        sigma_s = sigma_s * (bayernr->expo_values[1] + 64) / isp_bitfix(6); /* 64 6 */

        sigma_m = (hi_u16)MIN2(0xffff, ((hi_u64)sigma_m * nrm_gain * bcom_gain * bcom_gain / isp_bitfix(27))); /* 27 */
        sigma_s = (hi_u16)MIN2(0xffff, ((hi_u64)sigma_s * nrm_gain * bcom_gain * bcom_gain / isp_bitfix(27))); /* 27 */

        sigma_m = MIN2(65535, sigma_m * bayernr->mpi_cfg.wdr_attr.fusion_frame_str[1] / isp_bitfix(4)); /* 65535 4 */
        sigma_s = MIN2(65535, sigma_s * bayernr->mpi_cfg.wdr_attr.fusion_frame_str[0] / isp_bitfix(4)); /* 65535 4 */

        sigma_m_max = (sigma_m_max < sigma_m) ? sigma_m : sigma_m_max;
        sigma_s_max = (sigma_s_max < sigma_s) ? sigma_s : sigma_s_max;

        sigma_m_arr[i] = sigma_m;
        sigma_s_arr[i] = sigma_s;
    }

    bayernr->lmt_np_thresh = (sigma_m_max > (hi_u32)bit_mask) ? bit_mask : sigma_m_max;
    bayernr->lmt_np_thresh_s = (sigma_s_max > (hi_u32)bit_mask) ? bit_mask : sigma_s_max;

    for (i = 0; i < HI_ISP_BAYERNR_LMTLUTNUM; i++) {
        dyna_cfg->jnlm_limit_lut[i] = (hi_u8)MIN2(0xff,
            ((hi_u64)sigma_m_arr[i] * 128 + sigma_m_max / 2) / div_0_to_1(sigma_m_max)); /* 128 2 */
        dyna_cfg->jnlm_limit_s_lut[i] = (hi_u8)MIN2(0xff,
            ((hi_u64)sigma_s_arr[i] * 128 + sigma_s_max / 2) / div_0_to_1(sigma_s_max)); /* 128 2 */
    }

    dyna_cfg->center_wgt_en = HI_TRUE;
    dyna_cfg->wdr_map_flt_mode = HI_FALSE;

    return HI_SUCCESS;
}

static hi_void isp_bayernr_get_sync_framethr(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx, isp_bayernr *bayernr)
{
    hi_u8 sync_index;
    hi_s8 i;
    hi_isp_sns_regs_info *sns_regs_info = HI_NULL;
    isp_sensor_get_sns_reg(vi_pipe, &sns_regs_info);

    if (is_offline_mode(isp_ctx->block_attr.running_mode) ||
        is_striping_mode(isp_ctx->block_attr.running_mode)) { /* offline mode */
        if (is_half_wdr_mode(isp_ctx->sns_wdr_mode)) {
            sync_index = MIN2(sns_regs_info->cfg2_valid_delay_max, CFG2VLD_DLY_LIMIT - 1);
        } else {
            sync_index = MIN2(sns_regs_info->cfg2_valid_delay_max + 1, CFG2VLD_DLY_LIMIT - 1);
        }
    } else { /* online mode */
        if (is_half_wdr_mode(isp_ctx->sns_wdr_mode)) {
            sync_index = CLIP3((hi_s8)sns_regs_info->cfg2_valid_delay_max - 1, 0, CFG2VLD_DLY_LIMIT - 1);
        } else {
            sync_index = MIN2(sns_regs_info->cfg2_valid_delay_max, CFG2VLD_DLY_LIMIT - 1);
        }
    }

    for (i = CFG2VLD_DLY_LIMIT - 2; i >= 0; i--) { /* 2 */
        bayernr->wdr_sync_frame_thr[i + 1][0] = bayernr->wdr_sync_frame_thr[i][0];
        bayernr->wdr_sync_frame_thr[i + 1][1] = bayernr->wdr_sync_frame_thr[i][1];
        bayernr->wdr_sync_fusion_en[i + 1] = bayernr->wdr_sync_fusion_en[i];
    }

    bayernr->wdr_sync_frame_thr[0][0] = hi_ext_system_wdr_longthr_read(vi_pipe);
    bayernr->wdr_sync_frame_thr[0][1] = hi_ext_system_wdr_shortthr_read(vi_pipe);
    bayernr->wdr_sync_fusion_en[0] = hi_ext_system_fusion_mode_read(vi_pipe);

    memcpy_s(bayernr->wdr_frame_thr, (WDR_MAX_FRAME_NUM + 2) * sizeof(hi_u16), /* 2 */
             bayernr->wdr_sync_frame_thr[sync_index], (WDR_MAX_FRAME_NUM + 2) * sizeof(hi_u16)); /* 2 */
    bayernr->wdr_fusion_en = bayernr->wdr_sync_fusion_en[sync_index];
}

static hi_s32 hiisp_bayernr_fw_wdr(hi_u32 iso, hi_u32 sensor_iso, hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg,
                                   isp_bayernr *bayernr)
{
    hi_u8 i;
    hi_u16 black_level; /* 12bits */
    hi_u32 k, b;
    isp_bnr_iso iso_struc;

    hi_float f_calibration_coef;
    static hi_u32 rgain_iir = 65536;
    static hi_u32 bgain_iir = 65536;

    hi_isp_cmos_default *sns_dft = HI_NULL;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_bayernr_dyna_cfg *dyna_cfg = HI_NULL;
    isp_bayernr_usr_cfg *usr_cfg = HI_NULL;

    dyna_cfg = &reg_cfg->alg_reg_cfg[0].bnr_reg_cfg.dyna_reg_cfg;
    usr_cfg  = &reg_cfg->alg_reg_cfg[0].bnr_reg_cfg.usr_reg_cfg;

    isp_sensor_get_default(vi_pipe, &sns_dft);
    isp_get_ctx(vi_pipe, isp_ctx);

    isp_bayernr_get_sync_framethr(vi_pipe, isp_ctx, bayernr);
    dyna_cfg->wdr_mode_en = bayernr->wdr_mode_en;
    dyna_cfg->wdr_fusion_en = bayernr->wdr_fusion_en;
    usr_cfg->bnr_mono_sensor_en = bayernr->bnr_mono_sensor_en;  /* mono_sensor, waiting to get */
    usr_cfg->bnr_lsc_en = (bayernr->mpi_cfg.nr_lsc_enable == 0) ? 0 : 3; /* 0 3 */
    usr_cfg->bnr_lsc_max_gain = bayernr->mpi_cfg.bnr_lsc_max_gain + 64; /* 64 */
    usr_cfg->bnr_lsc_cmp_strength = bayernr->mpi_cfg.bnr_lsc_cmp_strength;

    /* noise LUT */
    f_calibration_coef = bayernr_get_kfrom_noise_lut(sns_dft->noise_calibration.calibration_coef,
                                                     sns_dft->noise_calibration.calibration_lut_num, sensor_iso);
    k = (hi_u32)(f_calibration_coef * isp_bitfix(14)); /* 14 */
    f_calibration_coef = bayernr_get_bfrom_noise_lut(sns_dft->noise_calibration.calibration_coef,
                                                     sns_dft->noise_calibration.calibration_lut_num, sensor_iso);
    b = (hi_u32)(f_calibration_coef * isp_bitfix(14)); /* 14 */

    iso_struc.iso = iso;
    iso_struc.iso_index_upper = get_iso_index(iso_struc.iso);
    iso_struc.iso_index_lower = MAX2((hi_s8)iso_struc.iso_index_upper - 1, 0);
    iso_struc.iso1 = get_iso(iso_struc.iso_index_lower);
    iso_struc.iso2 = get_iso(iso_struc.iso_index_upper);

    nr_cfg(vi_pipe, dyna_cfg, bayernr, iso_struc);
    nr_ext_cfg(vi_pipe, dyna_cfg, bayernr, iso_struc);

    if ((isp_ctx->linkage.fswdr_mode == ISP_FSWDR_LONG_FRAME_MODE) ||
        (isp_ctx->linkage.fswdr_mode == ISP_FSWDR_AUTO_LONG_FRAME_MODE)) {
        black_level = reg_cfg->alg_reg_cfg[0].be_blc_cfg.dyna_blc.wdr_blc[0].out_blc >> 8; /* right shift 8 */
        nr_limit_lut(vi_pipe, dyna_cfg, bayernr, sensor_iso, black_level);
    } else {
        black_level = reg_cfg->alg_reg_cfg[0].be_blc_cfg.dyna_blc.wdr_blc[0].blc[0]; /* 12bits */
        if (bayernr->wdr_fusion_en == HI_FALSE) {
            nr_limit_lut_wdr2to1(dyna_cfg, bayernr, k, b, black_level); /* WDR mode */
        } else {
            nr_limit_lut_fusion(dyna_cfg, bayernr, k, b, black_level); /* fusion mode */
        }
    }

    nr_multi_gain_cfg(vi_pipe, dyna_cfg, bayernr);

    if (HI_WDR_EINIT_BLCNR == 0) {
        dyna_cfg->jnlm_limit_lut[0] = dyna_cfg->jnlm_limit_lut[3]; /* 0 3 */
        dyna_cfg->jnlm_limit_lut[1] = dyna_cfg->jnlm_limit_lut[3]; /* 1 3 */
        dyna_cfg->jnlm_limit_lut[2] = dyna_cfg->jnlm_limit_lut[3]; /* 2 3 */
    }

    if (bayernr->wdr_fusion_en == HI_TRUE) {  /* fusion */
        dyna_cfg->wdr_map_gain = (hi_u8)(((hi_u16)bayernr->wdr_map_gain * 512 + 25) / 50); /* 512 25 50 */
    } else {
        dyna_cfg->wdr_map_gain = (hi_u8)((bayernr->wdr_map_gain * 128 + 7) / 14); /* 128 7 14 */
    }
    if (usr_cfg->bnr_mono_sensor_en == 1) {
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            dyna_cfg->medc_enable = HI_FALSE;
            dyna_cfg->c_ratio[i] = 0;
            dyna_cfg->amed_level[i] = 0;
        }
    }

    dyna_cfg->bnr_lut_update_en = HI_TRUE;

    rgain_iir = (7 * rgain_iir + isp_ctx->linkage.white_balance_gain[0]) >> 3; /* 7 3 */
    bgain_iir = (7 * bgain_iir + isp_ctx->linkage.white_balance_gain[3]) >> 3; /* 7 3 */

    dyna_cfg->r_lmt_rgain = (rgain_iir + 128) >> 8; /* 128 8 */
    dyna_cfg->r_lmt_bgain = (bgain_iir + 128) >> 8; /* 128 8 */

    dyna_cfg->r_lmt_rgain = (dyna_cfg->r_lmt_rgain > 1023) ? 1023 : dyna_cfg->r_lmt_rgain; /* 1023 */
    dyna_cfg->r_lmt_bgain = (dyna_cfg->r_lmt_bgain > 1023) ? 1023 : dyna_cfg->r_lmt_bgain; /* 1023 */

    dyna_cfg->resh = HI_TRUE;
    usr_cfg->resh = HI_TRUE;

    return HI_SUCCESS;
}

static hi_bool __inline check_bnr_open(isp_bayernr *bayernr)
{
    return (bayernr->mpi_cfg.enable == HI_TRUE);
}

static hi_void bayernr_wdr_mode(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg, isp_bayernr *bayernr)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_bayernr_dyna_cfg *dyna_cfg = &reg_cfg->alg_reg_cfg[0].bnr_reg_cfg.dyna_reg_cfg;

    isp_get_ctx(vi_pipe, isp_ctx);

    if ((reg_cfg->alg_reg_cfg[0].wdr_reg_cfg.wdr_en == HI_TRUE) && is_2to1_wdr_mode(isp_ctx->sns_wdr_mode)) {
        bayernr->wdr_frames_merge  = 2; /* 2 */
        bayernr->wdr_map_gain      = HI_ISP_BNR_DEFAULT_WDR_WDRMAP_GAIN;
        dyna_cfg->wdr_map_gain     = HI_ISP_BNR_DEFAULT_WDR_WDRMAP_GAIN;
        dyna_cfg->wdr_map_flt_mode = HI_ISP_BNR_DEFAULT_WDR_WDRMAP_FLTMODE;
        dyna_cfg->jnlm_sym_coef    = HI_ISP_BNR_DEFAULT_JNLM_SYMCOEF_WDR;
        dyna_cfg->edge_det_thr     = HI_ISP_BNR_DEFAULT_EDGE_DET_THRESHOLD_WDR;
        bayernr->edge_det_thr      = HI_ISP_BNR_DEFAULT_EDGE_DET_THRESHOLD_WDR;
    } else {
        bayernr->wdr_frames_merge  = 1; /* 1 */
        bayernr->wdr_blc_thr       = 0;
        bayernr->wdr_map_gain      = HI_ISP_BNR_DEFAULT_LINEAR_WDRMAP_GAIN;
        dyna_cfg->wdr_map_gain     = HI_ISP_BNR_DEFAULT_LINEAR_WDRMAP_GAIN;
        dyna_cfg->wdr_map_flt_mode = HI_ISP_BNR_DEFAULT_LINEAR_WDRMAP_FLTMODE;
        dyna_cfg->jnlm_sym_coef    = HI_ISP_BNR_DEFAULT_JNLM_SYMCOEF_LINEAR;
        dyna_cfg->edge_det_thr     = HI_ISP_BNR_DEFAULT_EDGE_DET_THRESHOLD_LINEAR;
        bayernr->edge_det_thr      = HI_ISP_BNR_DEFAULT_EDGE_DET_THRESHOLD_LINEAR;
    }

    bayernr->wdr_mode_en = reg_cfg->alg_reg_cfg[0].wdr_reg_cfg.wdr_en;
}

hi_void isp_bayernr_reg_update(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg, isp_bayernr *bayernr)
{
    hi_u8  i;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    bayernr_wdr_mode(vi_pipe, reg_cfg, bayernr);

    if (bayernr->wdr_frames_merge > 1) {
        hiisp_bayernr_fw_wdr(isp_ctx->linkage.iso, isp_ctx->linkage.sensor_iso, vi_pipe, reg_cfg, bayernr);
    } else {
        hiisp_bayernr_fw(isp_ctx->linkage.iso, isp_ctx->linkage.sensor_iso, vi_pipe, reg_cfg, bayernr);
    }

    for (i = 1; i < isp_ctx->block_attr.block_num; i++) {
        memcpy_s(&reg_cfg->alg_reg_cfg[i].bnr_reg_cfg.dyna_reg_cfg, sizeof(isp_bayernr_dyna_cfg),
                 &reg_cfg->alg_reg_cfg[0].bnr_reg_cfg.dyna_reg_cfg, sizeof(isp_bayernr_dyna_cfg));

        memcpy_s(&reg_cfg->alg_reg_cfg[i].bnr_reg_cfg.usr_reg_cfg, sizeof(isp_bayernr_usr_cfg),
                 &reg_cfg->alg_reg_cfg[0].bnr_reg_cfg.usr_reg_cfg, sizeof(isp_bayernr_usr_cfg));
    }
}

static hi_void isp_bayernr_actual_update(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg, isp_bayernr *bayernr)
{
    hi_u8 i;

    isp_bayernr_dyna_cfg *dyna_cfg = HI_NULL;

    dyna_cfg = &reg_cfg->alg_reg_cfg[0].bnr_reg_cfg.dyna_reg_cfg;

    hi_ext_system_bayernr_actual_coring_weight_write(vi_pipe, bayernr->coring_low);
    hi_ext_system_bayernr_actual_fine_strength_write(vi_pipe, dyna_cfg->jnlm_gain);
    hi_ext_system_bayernr_actual_nr_lsc_ratio_write(vi_pipe, bayernr->mpi_cfg.nr_lsc_ratio);

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        hi_ext_system_bayernr_actual_coarse_strength_write(vi_pipe, i, bayernr->lmt_strength[i]);
        hi_ext_system_bayernr_actual_chroma_strength_write(vi_pipe, i, dyna_cfg->amed_level[i]);
    }

    for (i = 0; i < WDR_MAX_FRAME_NUM; i++) {
        hi_ext_system_bayernr_actual_wdr_frame_strength_write(vi_pipe, i, bayernr->mpi_cfg.wdr_attr.wdr_frame_str[i]);
    }
}

static hi_s32 isp_bayernr_run(hi_vi_pipe vi_pipe, const hi_void *stat_info, hi_void *reg_cfg_info, hi_s32 rsv)
{
    hi_u8  i;
    hi_s32 ret;
    isp_reg_cfg *reg_cfg = (isp_reg_cfg *)reg_cfg_info;
    isp_bayernr *bayernr = HI_NULL;

    bayernr_get_ctx(vi_pipe, bayernr);
    isp_check_pointer_return(bayernr);

    /* calculate every two interrupts */
    if (!bayernr->init) {
        return HI_SUCCESS;
    }

    bayernr->mpi_cfg.enable = hi_ext_system_bayernr_enable_read(vi_pipe);

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        reg_cfg->alg_reg_cfg[i].bnr_reg_cfg.bnr_enable = bayernr->mpi_cfg.enable;
        reg_cfg->alg_reg_cfg[i].de_reg_cfg.kernel_reg_cfg.bnr_enable = bayernr->mpi_cfg.enable;
    }

    reg_cfg->cfg_key.bit1_bayernr_cfg = 1;

    /* check hardware setting */
    if (!check_bnr_open(bayernr)) {
        return HI_SUCCESS;
    }

    isp_bayernr_read_extregs(vi_pipe, &reg_cfg->alg_reg_cfg[0].be_blc_cfg, bayernr);
    ret = isp_bayernr_read_pro_mode(vi_pipe, bayernr);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    isp_bayernr_reg_update(vi_pipe, reg_cfg, bayernr);

    isp_bayernr_actual_update(vi_pipe, reg_cfg, bayernr);

    return HI_SUCCESS;
}

static hi_s32 isp_bayernr_ctrl(hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value)
{
    isp_bayernr *bayernr = HI_NULL;
    isp_reg_cfg_attr *reg_cfg = HI_NULL;

    switch (cmd) {
        case ISP_WDR_MODE_SET:
            isp_regcfg_get_ctx(vi_pipe, reg_cfg);
            bayernr_get_ctx(vi_pipe, bayernr);
            isp_check_pointer_return(reg_cfg);
            isp_check_pointer_return(bayernr);

            bayernr->init = HI_FALSE;
            isp_bayernr_init(vi_pipe, (hi_void *)&reg_cfg->reg_cfg);
            break;
        case ISP_PROC_WRITE:
            bayernr_get_ctx(vi_pipe, bayernr);
            isp_check_pointer_return(bayernr);
            bayernr_proc_write(vi_pipe, (hi_isp_ctrl_proc_write *)value, bayernr);
            break;
        default:
            break;
    }
    return HI_SUCCESS;
}

static hi_s32 isp_bayernr_exit(hi_vi_pipe vi_pipe)
{
    hi_u8 i;
    isp_reg_cfg_attr *reg_cfg = HI_NULL;

    isp_regcfg_get_ctx(vi_pipe, reg_cfg);

    for (i = 0; i < reg_cfg->reg_cfg.cfg_num; i++) {
        reg_cfg->reg_cfg.alg_reg_cfg[i].bnr_reg_cfg.bnr_enable = HI_FALSE;
    }

    reg_cfg->reg_cfg.cfg_key.bit1_bayernr_cfg = 1;

    bayer_nr_ctx_exit(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_alg_register_bayer_nr(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_alg_node *algs = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_alg_check_return(isp_ctx->alg_key.bit1_bayernr);
    algs = isp_search_alg(isp_ctx->algs);
    isp_check_pointer_return(algs);

    algs->alg_type = ISP_ALG_BAYERNR;
    algs->alg_func.pfn_alg_init = isp_bayernr_init;
    algs->alg_func.pfn_alg_run  = isp_bayernr_run;
    algs->alg_func.pfn_alg_ctrl = isp_bayernr_ctrl;
    algs->alg_func.pfn_alg_exit = isp_bayernr_exit;
    algs->used = HI_TRUE;

    return HI_SUCCESS;
}

