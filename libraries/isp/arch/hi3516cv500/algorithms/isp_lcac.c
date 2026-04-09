/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_lcac.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_alg.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_math_utils.h"
#include "isp_proc.h"
/* Lobal CAC,default value */
#define ISP_LCAC_LUMA_THR_R_WDR                  1500
#define ISP_LCAC_LUMA_THR_G_WDR                  1500
#define ISP_LCAC_LUMA_THR_B_WDR                  2150

#define ISP_LCAC_LUMA_THR_R_LINEAR               1500
#define ISP_LCAC_LUMA_THR_G_LINEAR               1500
#define ISP_LCAC_LUMA_THR_B_LINEAR               3500

#define ISP_LCAC_DE_PURPLE_CB_STRENGTH_LINEAR    3 /* [0,8] */
#define ISP_LCAC_DE_PURPLE_CR_STRENGTH_LINEAR    0 /* [0,8] */

#define ISP_LCAC_DE_PURPLE_CB_STRENGTH_WDR       7 /* [0,8] */
#define ISP_LCAC_DE_PURPLE_CR_STRENGTH_WDR       0 /* [0,8] */

#define ISP_LCAC_CRCB_RATIO_HIGH_LIMIT_LINEAR    292
#define ISP_LCAC_CRCB_RATIO_LOW_LIMIT_LINEAR     (-50)

#define ISP_LCAC_CRCB_RATIO_HIGH_LIMIT_WDR       292
#define ISP_LCAC_CRCB_RATIO_LOW_LIMIT_WDR        (-50)

#define ISP_LCAC_PURPLE_DET_RANGE_DEFAULT_LINEAR 60
#define ISP_LCAC_PURPLE_DET_RANGE_DEFAULT_WDR    60

#define ISP_LCAC_MAX_STRENGTH                    8
#define RANGE_MAX_VALUE                          3

#define MIN_EXP_RATIO                            64
#define MAX_EXP_RATIO                            1023

static const hi_u32 g_exp_ratio_lut[ISP_EXP_RATIO_STRENGTH_NUM] = {
    64, 128, 192, 256, 320, 384, 448, 512, 576, 640, 704, 768, 832, 896, 960, 1023
};

static const hi_u8  g_wdr_cb_strdef_lut[ISP_EXP_RATIO_STRENGTH_NUM] = {0, 0, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
static const hi_u8  g_wdr_cr_strdef_lut[ISP_EXP_RATIO_STRENGTH_NUM] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static const hi_u8  g_lin_cb_strdef_lut[ISP_EXP_RATIO_STRENGTH_NUM] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
static const hi_u8  g_lin_cr_strdef_lut[ISP_EXP_RATIO_STRENGTH_NUM] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static const hi_u8  g_de_purp_str_lut[LCAC_STRENGTH_NUM]            = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static const hi_u16 g_r_luma[RANGE_MAX_VALUE] = {1500, 1500, 0};
static const hi_u16 g_g_luma[RANGE_MAX_VALUE] = {1500, 1500, 0};
static const hi_u16 g_b_luma[RANGE_MAX_VALUE] = {4095, 1500, 0};
static const hi_u16 g_y_luma[RANGE_MAX_VALUE] = {3200, 1500, 0};
static const hi_s16 g_cbcrl[RANGE_MAX_VALUE] = { -50,  -50, -50};
static const hi_u16 g_purple_det_range[RANGE_MAX_VALUE] = {0, 260, 410};

typedef struct {
    hi_bool local_cac_en;
    hi_bool cac_manual_en;
    hi_bool coef_update_en;
    hi_u8   auto_de_purple_str[LCAC_STRENGTH_NUM];
    hi_u8   auto_depurple_cb_str[ISP_EXP_RATIO_STRENGTH_NUM];
    hi_u8   auto_depurple_cr_str[ISP_EXP_RATIO_STRENGTH_NUM];
    hi_u8   manual_depurple_cb_str;
    hi_u8   manual_depurple_cr_str;
    hi_u8   actual_depurple_cb_str;
    hi_u8   actual_depurple_cr_str;
    hi_u8   luma_high_cnt_thr;    /* u8.0, [0,153] */
    hi_u8   cb_cnt_high_thr;      /* u7.0, [0,68] */
    hi_u8   cb_cnt_low_thr;       /* u7.0, [0,68] */
    hi_u8   bld_avg_cur;          /* u4.0, [0, 8] */
    hi_u16  luma_thr;             /* u12.0, [0,4095] */
    hi_u16  cb_thr;               /* u12.0, [0,4095] */
    hi_u16  purple_det_range;

    hi_u16  r_luma_thr[ISP_LCAC_DET_NUM];
    hi_u16  g_luma_thr[ISP_LCAC_DET_NUM];
    hi_u16  b_luma_thr[ISP_LCAC_DET_NUM];
    hi_u16  y_luma_thr[ISP_LCAC_DET_NUM];
    hi_s16  cbcr_ratio[ISP_LCAC_DET_NUM];

    hi_u16  purple_var_thr;
} isp_lcac;

isp_lcac *g_lcac_ctx[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define lcac_get_ctx(dev, ctx)   ((ctx) = g_lcac_ctx[dev])
#define lcac_set_ctx(dev, ctx)   (g_lcac_ctx[dev] = (ctx))
#define lcac_reset_ctx(dev)      (g_lcac_ctx[dev] = HI_NULL)

static hi_u32 lcac_get_exp_ratio(hi_vi_pipe vi_pipe, hi_u8 cur_blk, hi_u8 wdr_mode)
{
    hi_u32 exp_ratio;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    exp_ratio = isp_ctx->linkage.exp_ratio;

    if (is_linear_mode(wdr_mode)) {
        return MIN_EXP_RATIO;
    } else if (is_built_in_wdr_mode(wdr_mode)) {
        return MAX_EXP_RATIO;
    } else if (is_2to1_wdr_mode(wdr_mode)) {
        if ((isp_ctx->linkage.fswdr_mode == ISP_FSWDR_LONG_FRAME_MODE) ||
            (isp_ctx->linkage.fswdr_mode == ISP_FSWDR_AUTO_LONG_FRAME_MODE)) {
            return MIN_EXP_RATIO;
        } else {
            return exp_ratio;
        }
    } else {
        return MIN_EXP_RATIO;
    }
}

hi_s32 lcac_ctx_init(hi_vi_pipe vi_pipe)
{
    isp_lcac *lcac_ctx = HI_NULL;

    lcac_get_ctx(vi_pipe, lcac_ctx);

    if (lcac_ctx == HI_NULL) {
        lcac_ctx = (isp_lcac *)isp_malloc(sizeof(isp_lcac));
        if (lcac_ctx == HI_NULL) {
            isp_err_trace("isp[%d] lcac_ctx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset_s(lcac_ctx, sizeof(isp_lcac), 0, sizeof(isp_lcac));

    lcac_set_ctx(vi_pipe, lcac_ctx);

    return HI_SUCCESS;
}

hi_void lcac_ctx_exit(hi_vi_pipe vi_pipe)
{
    isp_lcac *lcac_ctx = HI_NULL;

    lcac_get_ctx(vi_pipe, lcac_ctx);
    isp_free(lcac_ctx);
    lcac_reset_ctx(vi_pipe);
}

static hi_void lcac_ext_regs_initialize(hi_vi_pipe vi_pipe)
{
    hi_u8 i;
    hi_u8 wdr_mode;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_get_ctx(vi_pipe, isp_ctx);

    wdr_mode = isp_ctx->sns_wdr_mode;
    hi_ext_system_local_cac_manual_mode_enable_write(vi_pipe, OP_TYPE_AUTO);
    hi_ext_system_local_cac_enable_write(vi_pipe, HI_TRUE);
    hi_ext_system_local_cac_coef_update_en_write(vi_pipe, HI_TRUE);
    hi_ext_system_local_cac_purple_var_thr_write(vi_pipe, HI_ISP_LCAC_PURPLE_DET_THR_DEFAULT);
    hi_ext_system_local_cac_luma_high_cnt_thr_write(vi_pipe, HI_ISP_DEMOSAIC_CAC_LUMA_HIGH_CNT_THR_DEFAULT);
    hi_ext_system_local_cac_cb_cnt_high_thr_write(vi_pipe, HI_ISP_DEMOSAIC_CAC_CB_CNT_HIGH_THR_DEFAULT);
    hi_ext_system_local_cac_cb_cnt_low_thr_write(vi_pipe, HI_ISP_DEMOSAIC_CAC_CB_CNT_LOW_THR_DEFAULT);
    hi_ext_system_local_cac_luma_thr_write(vi_pipe, HI_ISP_DEMOSAIC_LUMA_THR_DEFAULT);
    hi_ext_system_local_cac_cb_thr_write(vi_pipe, HI_ISP_DEMOSAIC_CB_THR_DEFAULT);
    hi_ext_system_local_cac_bld_avg_cur_write(vi_pipe, HI_ISP_DEMOSAIC_CAC_BLD_AVG_CUR_DEFAULT);
    hi_ext_system_local_cac_defcolor_cr_write(vi_pipe, HI_ISP_DEMOSAIC_DEFCOLOR_CR_DEFAULT);
    hi_ext_system_local_cac_defcolor_cb_write(vi_pipe, HI_ISP_DEMOSAIC_DEFCOLOR_CB_DEFAULT);

    for (i = 0; i < ISP_LCAC_DET_NUM; i++) {
        hi_ext_system_local_cac_r_thd_table_write(vi_pipe, i, g_r_luma[i]);
        hi_ext_system_local_cac_g_thd_table_write(vi_pipe, i, g_g_luma[i]);
        hi_ext_system_local_cac_b_thd_table_write(vi_pipe, i, g_b_luma[i]);
        hi_ext_system_local_cac_luma_thd_table_write(vi_pipe, i, g_y_luma[i]);
        hi_ext_system_local_cac_cbcr_ratio_table_write(vi_pipe, i, g_cbcrl[i]);
    }

    if (is_linear_mode(wdr_mode)) { /* linear mode */
        for (i = 0; i < ISP_EXP_RATIO_STRENGTH_NUM; i++) {
            hi_ext_system_local_cac_auto_cb_str_table_write(vi_pipe, i, g_lin_cb_strdef_lut[i]);
            hi_ext_system_local_cac_auto_cr_str_table_write(vi_pipe, i, g_lin_cr_strdef_lut[i]);
        }

        hi_ext_system_local_cac_manual_cb_str_write(vi_pipe, ISP_LCAC_DE_PURPLE_CB_STRENGTH_LINEAR);
        hi_ext_system_local_cac_manual_cr_str_write(vi_pipe, ISP_LCAC_DE_PURPLE_CR_STRENGTH_LINEAR);
        hi_ext_system_local_cac_purple_det_range_write(vi_pipe, ISP_LCAC_PURPLE_DET_RANGE_DEFAULT_LINEAR);
    } else { /* WDR mode */
        for (i = 0; i < ISP_EXP_RATIO_STRENGTH_NUM; i++) {
            hi_ext_system_local_cac_auto_cb_str_table_write(vi_pipe, i, g_wdr_cb_strdef_lut[i]);
            hi_ext_system_local_cac_auto_cr_str_table_write(vi_pipe, i, g_wdr_cr_strdef_lut[i]);
        }

        hi_ext_system_local_cac_manual_cb_str_write(vi_pipe, ISP_LCAC_DE_PURPLE_CB_STRENGTH_WDR);
        hi_ext_system_local_cac_manual_cr_str_write(vi_pipe, ISP_LCAC_DE_PURPLE_CR_STRENGTH_WDR);
        hi_ext_system_local_cac_purple_det_range_write(vi_pipe, ISP_LCAC_PURPLE_DET_RANGE_DEFAULT_WDR);
    }
}

static hi_void lcac_static_regs_initialize(isp_local_cac_static_cfg  *static_reg_cfg)
{
    hi_u8 i;

    static_reg_cfg->nddm_cac_blend_en   = HI_TRUE;
    static_reg_cfg->nddm_cac_blend_rate = HI_ISP_NDDM_CAC_BLEND_RATE_DEFAULT;
    static_reg_cfg->r_counter_thr       = HI_ISP_LCAC_COUNT_THR_R_DEFAULT;
    static_reg_cfg->g_counter_thr       = HI_ISP_LCAC_COUNT_THR_G_DEFAULT;
    static_reg_cfg->b_counter_thr       = HI_ISP_LCAC_COUNT_THR_B_DEFAULT;
    static_reg_cfg->satu_thr            = HI_ISP_LCAC_SATU_THR_DEFAULT;

    for (i = 0; i < LCAC_STRENGTH_NUM; i++) {
        static_reg_cfg->de_purple_str[i] = ISP_LCAC_MAX_STRENGTH - g_de_purp_str_lut[i];
    }

    static_reg_cfg->static_resh         = HI_TRUE;
}

static hi_void lcac_usr_regs_initialize(hi_vi_pipe vi_pipe, hi_u8 wdr_mode,
                                        isp_local_cac_usr_cfg *usr_reg_cfg, isp_lcac *lcac_ctx)
{
    hi_isp_cmos_default *sns_dft = HI_NULL;

    isp_sensor_get_default(vi_pipe, &sns_dft);

    usr_reg_cfg->resh              = HI_TRUE;
    usr_reg_cfg->update_index      = 1;
    usr_reg_cfg->var_thr           = HI_ISP_LCAC_PURPLE_DET_THR_DEFAULT;
    usr_reg_cfg->luma_high_cnt_thr = HI_ISP_DEMOSAIC_CAC_LUMA_HIGH_CNT_THR_DEFAULT;
    usr_reg_cfg->cb_cnt_high_thr   = HI_ISP_DEMOSAIC_CAC_CB_CNT_HIGH_THR_DEFAULT;
    usr_reg_cfg->cb_cnt_low_thr    = HI_ISP_DEMOSAIC_CAC_CB_CNT_LOW_THR_DEFAULT;
    usr_reg_cfg->bld_avg_cur       = HI_ISP_DEMOSAIC_CAC_BLD_AVG_CUR_DEFAULT;
    usr_reg_cfg->cb_thr            = HI_ISP_DEMOSAIC_CB_THR_DEFAULT;

    if (is_linear_mode(wdr_mode)) {
        usr_reg_cfg->cb_cr_ratio_lmt_high  = ISP_LCAC_CRCB_RATIO_HIGH_LIMIT_LINEAR;
    } else {
        usr_reg_cfg->cb_cr_ratio_lmt_high  = ISP_LCAC_CRCB_RATIO_HIGH_LIMIT_WDR;
    }

    /* Read from cmos or init from pre value */
    if (sns_dft->key.bit1_lcac) {
        usr_reg_cfg->var_thr = lcac_ctx->purple_var_thr;
    } else {
        usr_reg_cfg->var_thr = HI_ISP_LCAC_PURPLE_DET_THR_DEFAULT;
    }
}

static hi_void lcac_dyna_update_from_ctx(hi_vi_pipe vi_pipe, isp_local_cac_dyna_cfg *dyna_reg_cfg,
                                         hi_u8 cur_blk, isp_lcac *lcac_ctx, hi_u8 wdr_mode)
{
    /* Loading from cmos has already distinguish WDR mode and non WDR mode */
    /* Converting cmos paramters to dynamic reg value */
    hi_u8 i, de_purple_ctr_cb, de_purple_ctr_cr;
    hi_u16 pdet_range = lcac_ctx->purple_det_range;

    hi_s32 r_low, r_up, range_idx_up, range_idx_low, ratio_idx_up, ratio_idx_low;

    range_idx_up = RANGE_MAX_VALUE - 1;
    for (i = 0; i < RANGE_MAX_VALUE; i++) {
        if (pdet_range < g_purple_det_range[i]) {
            range_idx_up = i; break;
        }
    }

    range_idx_low = MAX2(range_idx_up - 1, 0);
    r_up          = g_purple_det_range[range_idx_up]; r_low  = g_purple_det_range[range_idx_low];

    hi_u16 r_luma_thr = (hi_u16)linear_inter(pdet_range, r_low, lcac_ctx->r_luma_thr[range_idx_low],
                                             r_up, lcac_ctx->r_luma_thr[range_idx_up]);
    hi_u16 g_luma_thr = (hi_u16)linear_inter(pdet_range, r_low, lcac_ctx->g_luma_thr[range_idx_low],
                                             r_up, lcac_ctx->g_luma_thr[range_idx_up]);
    hi_u16 b_luma_thr = (hi_u16)linear_inter(pdet_range, r_low, lcac_ctx->b_luma_thr[range_idx_low],
                                             r_up, lcac_ctx->b_luma_thr[range_idx_up]);
    hi_u16 luma_thr   = (hi_u16)linear_inter(pdet_range, r_low, lcac_ctx->y_luma_thr[range_idx_low],
                                             r_up, lcac_ctx->y_luma_thr[range_idx_up]);
    hi_s16 cb_cr_l    =         linear_inter(pdet_range, r_low, lcac_ctx->cbcr_ratio[range_idx_low],
                                             r_up, lcac_ctx->cbcr_ratio[range_idx_up]);

    hi_u32 exp_ratio = lcac_get_exp_ratio(vi_pipe, cur_blk, wdr_mode);

    if (lcac_ctx->cac_manual_en) {
        de_purple_ctr_cb = lcac_ctx->manual_depurple_cb_str;  de_purple_ctr_cr = lcac_ctx->manual_depurple_cr_str;
    } else {
        ratio_idx_up = ISP_EXP_RATIO_STRENGTH_NUM - 1;

        for (i = 0; i < ISP_EXP_RATIO_STRENGTH_NUM; i++) {
            if (exp_ratio <= g_exp_ratio_lut[i]) {
                ratio_idx_up = i;
                break;
            }
        }

        ratio_idx_low = MAX2(ratio_idx_up - 1, 0);

        de_purple_ctr_cb = linear_inter(exp_ratio,
                                        g_exp_ratio_lut[ratio_idx_low], lcac_ctx->auto_depurple_cb_str[ratio_idx_low],
                                        g_exp_ratio_lut[ratio_idx_up],  lcac_ctx->auto_depurple_cb_str[ratio_idx_up]);
        de_purple_ctr_cr = linear_inter(exp_ratio,
                                        g_exp_ratio_lut[ratio_idx_low], lcac_ctx->auto_depurple_cr_str[ratio_idx_low],
                                        g_exp_ratio_lut[ratio_idx_up],  lcac_ctx->auto_depurple_cr_str[ratio_idx_up]);
    }

    dyna_reg_cfg->r_luma_thr = r_luma_thr;  dyna_reg_cfg->g_luma_thr = g_luma_thr;
    dyna_reg_cfg->b_luma_thr = b_luma_thr;  dyna_reg_cfg->luma_thr   = luma_thr;
    dyna_reg_cfg->cb_cr_ratio_lmt_low = cb_cr_l;
    dyna_reg_cfg->de_purple_ctr_cb    = ISP_LCAC_MAX_STRENGTH - de_purple_ctr_cb;
    dyna_reg_cfg->de_purple_ctr_cr    = ISP_LCAC_MAX_STRENGTH - de_purple_ctr_cr;

    return;
}

static hi_void lcac_dyna_regs_initialize(hi_vi_pipe vi_pipe, hi_u8 wdr_mode, hi_u8 cur_blk,
                                         isp_local_cac_dyna_cfg *dyna_reg_cfg, isp_lcac *lcac_ctx)
{
    hi_isp_cmos_default *sns_dft = HI_NULL;

    isp_sensor_get_default(vi_pipe, &sns_dft);

    if (sns_dft->key.bit1_lcac) {
        lcac_dyna_update_from_ctx(vi_pipe, dyna_reg_cfg, cur_blk, lcac_ctx, wdr_mode);
    } else {
        if (is_linear_mode(wdr_mode)) {
            dyna_reg_cfg->r_luma_thr          = ISP_LCAC_LUMA_THR_R_LINEAR;
            dyna_reg_cfg->g_luma_thr          = ISP_LCAC_LUMA_THR_G_LINEAR;
            dyna_reg_cfg->b_luma_thr          = ISP_LCAC_LUMA_THR_B_LINEAR;
            dyna_reg_cfg->de_purple_ctr_cb    = ISP_LCAC_MAX_STRENGTH - ISP_LCAC_DE_PURPLE_CB_STRENGTH_LINEAR;
            dyna_reg_cfg->de_purple_ctr_cr    = ISP_LCAC_MAX_STRENGTH - ISP_LCAC_DE_PURPLE_CR_STRENGTH_LINEAR;
            dyna_reg_cfg->cb_cr_ratio_lmt_low = ISP_LCAC_CRCB_RATIO_LOW_LIMIT_LINEAR;
        } else {
            dyna_reg_cfg->r_luma_thr          = ISP_LCAC_LUMA_THR_R_WDR;
            dyna_reg_cfg->g_luma_thr          = ISP_LCAC_LUMA_THR_G_WDR;
            dyna_reg_cfg->b_luma_thr          = ISP_LCAC_LUMA_THR_B_WDR;
            dyna_reg_cfg->de_purple_ctr_cb    = ISP_LCAC_MAX_STRENGTH - ISP_LCAC_DE_PURPLE_CB_STRENGTH_WDR;
            dyna_reg_cfg->de_purple_ctr_cr    = ISP_LCAC_MAX_STRENGTH - ISP_LCAC_DE_PURPLE_CR_STRENGTH_WDR;
            dyna_reg_cfg->cb_cr_ratio_lmt_low = ISP_LCAC_CRCB_RATIO_LOW_LIMIT_WDR;
        }
    }

    dyna_reg_cfg->luma_thr = HI_ISP_DEMOSAIC_LUMA_THR_DEFAULT;
    dyna_reg_cfg->resh     = HI_TRUE;
}

static hi_s32  lcac_set_long_frame_mode(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_u8 i;
    isp_usr_ctx            *isp_ctx = HI_NULL;
    isp_local_cac_dyna_cfg *dyna_reg_cfg = HI_NULL;
    isp_local_cac_usr_cfg  *usr_reg_cfg = HI_NULL;
    isp_get_ctx(vi_pipe, isp_ctx);

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        usr_reg_cfg = &reg_cfg->alg_reg_cfg[i].lcac_reg_cfg.usr_reg_cfg;
        dyna_reg_cfg = &reg_cfg->alg_reg_cfg[i].lcac_reg_cfg.dyna_reg_cfg;

        if (is_linear_mode(isp_ctx->sns_wdr_mode) ||
            (isp_ctx->linkage.fswdr_mode == ISP_FSWDR_LONG_FRAME_MODE) ||
            (isp_ctx->linkage.fswdr_mode == ISP_FSWDR_AUTO_LONG_FRAME_MODE)) {
            dyna_reg_cfg->r_luma_thr          = ISP_LCAC_LUMA_THR_R_LINEAR;
            dyna_reg_cfg->g_luma_thr          = ISP_LCAC_LUMA_THR_G_LINEAR;
            dyna_reg_cfg->b_luma_thr          = ISP_LCAC_LUMA_THR_B_LINEAR;
            dyna_reg_cfg->de_purple_ctr_cb    = ISP_LCAC_MAX_STRENGTH - ISP_LCAC_DE_PURPLE_CB_STRENGTH_LINEAR;
            dyna_reg_cfg->de_purple_ctr_cr    = ISP_LCAC_MAX_STRENGTH - ISP_LCAC_DE_PURPLE_CR_STRENGTH_LINEAR;
            dyna_reg_cfg->cb_cr_ratio_lmt_low = ISP_LCAC_CRCB_RATIO_LOW_LIMIT_LINEAR;

            usr_reg_cfg->cb_cr_ratio_lmt_high = ISP_LCAC_CRCB_RATIO_HIGH_LIMIT_LINEAR;
        } else {
            dyna_reg_cfg->r_luma_thr          = ISP_LCAC_LUMA_THR_R_WDR;
            dyna_reg_cfg->g_luma_thr          = ISP_LCAC_LUMA_THR_G_WDR;
            dyna_reg_cfg->b_luma_thr          = ISP_LCAC_LUMA_THR_B_WDR;
            dyna_reg_cfg->de_purple_ctr_cb    = ISP_LCAC_MAX_STRENGTH - ISP_LCAC_DE_PURPLE_CB_STRENGTH_WDR;
            dyna_reg_cfg->de_purple_ctr_cr    = ISP_LCAC_MAX_STRENGTH - ISP_LCAC_DE_PURPLE_CR_STRENGTH_WDR;
            dyna_reg_cfg->cb_cr_ratio_lmt_low = ISP_LCAC_CRCB_RATIO_LOW_LIMIT_WDR;

            usr_reg_cfg->cb_cr_ratio_lmt_high = ISP_LCAC_CRCB_RATIO_HIGH_LIMIT_WDR;
        }

        usr_reg_cfg->resh = HI_TRUE;
    }

    return HI_SUCCESS;
}

static hi_void lcac_regs_initialize(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_u8 i;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_lcac    *lcac_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    lcac_get_ctx(vi_pipe, lcac_ctx);

    /* local CAC */
    for (i = 0; i < reg_cfg->cfg_num; i++) {
        lcac_static_regs_initialize(&reg_cfg->alg_reg_cfg[i].lcac_reg_cfg.static_reg_cfg);
        lcac_usr_regs_initialize(vi_pipe, isp_ctx->sns_wdr_mode,
                                 &reg_cfg->alg_reg_cfg[i].lcac_reg_cfg.usr_reg_cfg, lcac_ctx);
        lcac_dyna_regs_initialize(vi_pipe, isp_ctx->sns_wdr_mode, i,
                                  &reg_cfg->alg_reg_cfg[i].lcac_reg_cfg.dyna_reg_cfg, lcac_ctx);

        reg_cfg->alg_reg_cfg[i].lcac_reg_cfg.local_cac_en = HI_TRUE;
    }

    reg_cfg->cfg_key.bit1_local_cac_cfg  = 1;
}

static hi_void local_cac_read_extregs(hi_vi_pipe vi_pipe)
{
    hi_u8 i;
    isp_lcac *local_cac = HI_NULL;

    lcac_get_ctx(vi_pipe, local_cac);

    local_cac->coef_update_en = hi_ext_system_local_cac_coef_update_en_read(vi_pipe);
    hi_ext_system_local_cac_coef_update_en_write(vi_pipe, HI_FALSE);

    if (local_cac->coef_update_en) {
        for (i = 0; i < ISP_EXP_RATIO_STRENGTH_NUM; i++) {
            local_cac->auto_depurple_cb_str[i] = hi_ext_system_local_cac_auto_cb_str_table_read(vi_pipe, i);
            local_cac->auto_depurple_cr_str[i] = hi_ext_system_local_cac_auto_cr_str_table_read(vi_pipe, i);
        }

        for (i = 0; i < ISP_LCAC_DET_NUM; i++) {
            local_cac->r_luma_thr[i] = hi_ext_system_local_cac_r_thd_table_read(vi_pipe, i);
            local_cac->g_luma_thr[i] = hi_ext_system_local_cac_g_thd_table_read(vi_pipe, i);
            local_cac->b_luma_thr[i] = hi_ext_system_local_cac_b_thd_table_read(vi_pipe, i);
            local_cac->y_luma_thr[i] = hi_ext_system_local_cac_luma_thd_table_read(vi_pipe, i);
            local_cac->cbcr_ratio[i] = hi_ext_system_local_cac_cbcr_ratio_table_read(vi_pipe, i);
        }

        local_cac->cac_manual_en          = hi_ext_system_local_cac_manual_mode_enable_read(vi_pipe);
        local_cac->manual_depurple_cb_str = hi_ext_system_local_cac_manual_cb_str_read(vi_pipe);
        local_cac->manual_depurple_cr_str = hi_ext_system_local_cac_manual_cr_str_read(vi_pipe);
        local_cac->purple_det_range       = hi_ext_system_local_cac_purple_det_range_read(vi_pipe);
        local_cac->purple_var_thr         = hi_ext_system_local_cac_purple_var_thr_read(vi_pipe);

        local_cac->luma_high_cnt_thr = hi_ext_system_local_cac_luma_high_cnt_thr_read(vi_pipe);
        local_cac->cb_cnt_high_thr   = hi_ext_system_local_cac_cb_cnt_high_thr_read(vi_pipe);
        local_cac->cb_cnt_low_thr    = hi_ext_system_local_cac_cb_cnt_low_thr_read(vi_pipe);
        local_cac->bld_avg_cur       = hi_ext_system_local_cac_bld_avg_cur_read(vi_pipe);
        local_cac->luma_thr          = hi_ext_system_local_cac_luma_thr_read(vi_pipe);
        local_cac->cb_thr            = hi_ext_system_local_cac_cb_thr_read(vi_pipe);
    }

    return;
}

static hi_u32 lcac_check_cmos_param(hi_vi_pipe vi_pipe, const hi_isp_cmos_lcac *cmos_lcac)
{
    hi_u16 i;
    const hi_u16 max_det_range = 410;
    const hi_u16 max_var_thr   = 4095;
    const hi_u8  max_cbcr_str  = 8;
    const hi_u16 max_luma_thr  = 4095;
    const hi_s16 max_cbcr = 2047;
    const hi_s16 min_cbcr = -2048;

    if (cmos_lcac->purple_det_range > max_det_range) {
        isp_err_trace("err purple_det_range!!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (cmos_lcac->var_thr > max_var_thr) {
        isp_err_trace("err var_thr!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for (i = 0; i < ISP_LCAC_DET_NUM; i++) {
        if (cmos_lcac->r_luma_thd[i] > max_luma_thr) {
            isp_err_trace("err r_luma_thr!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (cmos_lcac->g_luma_thd[i] > max_luma_thr) {
            isp_err_trace("err g_luma_thr!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (cmos_lcac->b_luma_thd[i] > max_luma_thr) {
            isp_err_trace("err b_luma_thr!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (cmos_lcac->y_luma_thd[i] > max_luma_thr) {
            isp_err_trace("err y_luma_thr!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if ((cmos_lcac->cbcr_ratio[i] > max_cbcr) || (cmos_lcac->cbcr_ratio[i] < min_cbcr)) {
            isp_err_trace("err cbcr_ratio!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    for (i = 0 ; i < ISP_EXP_RATIO_STRENGTH_NUM ; i++) {
        if (cmos_lcac->auto_attr.de_purple_cr_str[i] > max_cbcr_str) {
            isp_err_trace("err auto_attr.de_purple_cr_str!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (cmos_lcac->auto_attr.de_purple_cb_str[i] > max_cbcr_str) {
            isp_err_trace("err auto_attr.de_purple_cb_str!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    if (cmos_lcac->manual_attr.de_purple_cr_str > max_cbcr_str) {
        isp_err_trace("err manual_attr.de_purple_cr_str!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (cmos_lcac->manual_attr.de_purple_cb_str > max_cbcr_str) {
        isp_err_trace("err manual_attr.de_purple_cb_str!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

static hi_void lcac_initialize(hi_vi_pipe vi_pipe)
{
    hi_u16 i;
    hi_s32 ret;

    isp_lcac *local_cac = HI_NULL;
    hi_isp_cmos_default *sns_dft = HI_NULL;

    isp_sensor_get_default(vi_pipe, &sns_dft);
    lcac_get_ctx(vi_pipe, local_cac);

    if (sns_dft->key.bit1_lcac) {
        if (sns_dft->lcac == HI_NULL) {
            isp_err_trace("null pointer!\n");
            return;
        }

        ret = lcac_check_cmos_param(vi_pipe, sns_dft->lcac);
        if (ret != HI_SUCCESS) {
            isp_err_trace("checking lcac_cmos_param failed!\n");
            return;
        }

        hi_ext_system_local_cac_enable_write(vi_pipe, sns_dft->lcac->enable);
        hi_ext_system_local_cac_manual_mode_enable_write(vi_pipe, sns_dft->lcac->op_mode);
        hi_ext_system_local_cac_purple_det_range_write(vi_pipe, sns_dft->lcac->purple_det_range);
        hi_ext_system_local_cac_purple_var_thr_write(vi_pipe, sns_dft->lcac->var_thr);

        hi_ext_system_local_cac_manual_cb_str_write(vi_pipe, sns_dft->lcac->manual_attr.de_purple_cb_str);
        hi_ext_system_local_cac_manual_cr_str_write(vi_pipe, sns_dft->lcac->manual_attr.de_purple_cr_str);

        for (i = 0; i < ISP_EXP_RATIO_STRENGTH_NUM; i++) {
            hi_ext_system_local_cac_auto_cb_str_table_write(vi_pipe, i, sns_dft->lcac->auto_attr.de_purple_cb_str[i]);
            hi_ext_system_local_cac_auto_cr_str_table_write(vi_pipe, i, sns_dft->lcac->auto_attr.de_purple_cr_str[i]);
            local_cac->auto_depurple_cb_str[i] = sns_dft->lcac->auto_attr.de_purple_cb_str[i];
            local_cac->auto_depurple_cr_str[i] = sns_dft->lcac->auto_attr.de_purple_cr_str[i];
        }

        for (i = 0; i < ISP_LCAC_DET_NUM; i++) {
            hi_ext_system_local_cac_r_thd_table_write(vi_pipe, i, sns_dft->lcac->r_luma_thd[i]);
            hi_ext_system_local_cac_g_thd_table_write(vi_pipe, i, sns_dft->lcac->g_luma_thd[i]);
            hi_ext_system_local_cac_b_thd_table_write(vi_pipe, i, sns_dft->lcac->b_luma_thd[i]);
            hi_ext_system_local_cac_luma_thd_table_write(vi_pipe, i, sns_dft->lcac->y_luma_thd[i]);
            hi_ext_system_local_cac_cbcr_ratio_table_write(vi_pipe, i, sns_dft->lcac->cbcr_ratio[i]);

            local_cac->r_luma_thr[i] = sns_dft->lcac->r_luma_thd[i];
            local_cac->g_luma_thr[i] = sns_dft->lcac->g_luma_thd[i];
            local_cac->b_luma_thr[i] = sns_dft->lcac->b_luma_thd[i];
            local_cac->y_luma_thr[i] = sns_dft->lcac->y_luma_thd[i];
            local_cac->cbcr_ratio[i] = sns_dft->lcac->cbcr_ratio[i];
        }

        // upate local_cac CTX
        local_cac->purple_det_range     = sns_dft->lcac->purple_det_range;
        local_cac->purple_var_thr       = sns_dft->lcac->var_thr;
        local_cac->manual_depurple_cb_str = sns_dft->lcac->manual_attr.de_purple_cb_str;
        local_cac->manual_depurple_cr_str = sns_dft->lcac->manual_attr.de_purple_cr_str;
        local_cac->cac_manual_en        = sns_dft->lcac->op_mode;
        local_cac->local_cac_en         = sns_dft->lcac->enable;
    } else {
        local_cac->local_cac_en = HI_TRUE;
    }

    return;
}

static hi_void local_cac_usr_fw(isp_local_cac_usr_cfg  *usr_reg_cfg, isp_lcac *local_cac)
{
    usr_reg_cfg->resh              = HI_TRUE;
    usr_reg_cfg->var_thr           = local_cac->purple_var_thr;
    usr_reg_cfg->luma_high_cnt_thr = local_cac->luma_high_cnt_thr;
    usr_reg_cfg->bld_avg_cur       = local_cac->bld_avg_cur;
    usr_reg_cfg->cb_thr            = local_cac->cb_thr;

    usr_reg_cfg->cb_cnt_high_thr   = local_cac->cb_cnt_high_thr;
    usr_reg_cfg->cb_cnt_low_thr    = local_cac->cb_cnt_low_thr;

    usr_reg_cfg->update_index  += 1;

    return;
}

static hi_void local_cac_dyna_fw(hi_vi_pipe vi_pipe, hi_u8 cur_blk, isp_lcac *local_cac,
                                 isp_local_cac_dyna_cfg *dyna_reg_cfg, hi_u8 wdr_mode)
{
    lcac_dyna_update_from_ctx(vi_pipe, dyna_reg_cfg, cur_blk, local_cac, wdr_mode);

    local_cac->actual_depurple_cb_str = ISP_LCAC_MAX_STRENGTH - dyna_reg_cfg->de_purple_ctr_cb;
    local_cac->actual_depurple_cr_str = ISP_LCAC_MAX_STRENGTH - dyna_reg_cfg->de_purple_ctr_cr;

    dyna_reg_cfg->resh = HI_TRUE;

    return;
}

static hi_bool __inline check_lcac_open(isp_lcac *local_cac)
{
    return (local_cac->local_cac_en == HI_TRUE);
}

hi_s32 isp_lcac_init(hi_vi_pipe vi_pipe, hi_void *reg_cfg)
{
    hi_s32 ret;
    isp_reg_cfg *local_reg_cfg = (isp_reg_cfg *)reg_cfg;

    ret = lcac_ctx_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    lcac_ext_regs_initialize(vi_pipe);
    lcac_initialize(vi_pipe);
    lcac_regs_initialize(vi_pipe, local_reg_cfg);

    return HI_SUCCESS;
}

static hi_void isp_lcac_wdr_mode_set(hi_vi_pipe vi_pipe, hi_void *reg_cfg)
{
    hi_u8 i;
    hi_s32 ret;
    hi_u32 update_idx[ISP_STRIPING_MAX_NUM] = {0};
    isp_reg_cfg *local_reg_cfg = (isp_reg_cfg *)reg_cfg;

    for (i = 0; i < local_reg_cfg->cfg_num; i++) {
        update_idx[i] = local_reg_cfg->alg_reg_cfg[i].lcac_reg_cfg.usr_reg_cfg.update_index;
    }

    ret = isp_lcac_init(vi_pipe, reg_cfg);
    if (ret != HI_SUCCESS) {
        isp_err_trace("isp[%d] isp_lcac_init failed!\n", vi_pipe);
        return;
    }

    for (i = 0; i < local_reg_cfg->cfg_num; i++) {
        local_reg_cfg->alg_reg_cfg[i].lcac_reg_cfg.usr_reg_cfg.update_index = update_idx[i] + 1;
    }
}

hi_s32 isp_lcac_run(hi_vi_pipe vi_pipe, const hi_void *stat_info,
                    hi_void *reg_cfg, hi_s32 rsv)
{
    hi_u8        i;
    isp_lcac     *local_cac     = HI_NULL;
    isp_reg_cfg  *local_reg_cfg = (isp_reg_cfg *)reg_cfg;
    isp_usr_ctx  *isp_ctx       = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    lcac_get_ctx(vi_pipe, local_cac);
    isp_check_pointer_return(local_cac);

    if (isp_ctx->linkage.defect_pixel) {
        return HI_SUCCESS;
    }

    local_cac->local_cac_en = hi_ext_system_local_cac_enable_read(vi_pipe);

    for (i = 0; i < local_reg_cfg->cfg_num; i++) {
        local_reg_cfg->alg_reg_cfg[i].lcac_reg_cfg.local_cac_en = local_cac->local_cac_en;
    }

    local_reg_cfg->cfg_key.bit1_local_cac_cfg = 1;

    /* check hardware setting */
    if (!check_lcac_open(local_cac)) {
        return HI_SUCCESS;
    }

    local_cac_read_extregs(vi_pipe);

    if (isp_ctx->linkage.fswdr_mode != isp_ctx->linkage.pre_fswdr_mode) {
        lcac_set_long_frame_mode(vi_pipe, reg_cfg);
    }

    if (local_cac->coef_update_en) {
        for (i = 0; i < local_reg_cfg->cfg_num; i++) {
            local_cac_usr_fw(&local_reg_cfg->alg_reg_cfg[i].lcac_reg_cfg.usr_reg_cfg, local_cac);
        }
    }

    for (i = 0; i < local_reg_cfg->cfg_num; i++) {
        local_cac_dyna_fw(vi_pipe, i, local_cac, &local_reg_cfg->alg_reg_cfg[i].lcac_reg_cfg.dyna_reg_cfg,
                          isp_ctx->sns_wdr_mode);
    }

    return HI_SUCCESS;
}

static hi_void lcac_proc_write(hi_vi_pipe vi_pipe, hi_isp_ctrl_proc_write *proc)
{
    hi_isp_ctrl_proc_write  proc_tmp;
    isp_lcac *local_cac = HI_NULL;

    lcac_get_ctx(vi_pipe, local_cac);
    isp_check_pointer_void_return(local_cac);

    if ((proc->proc_buff == HI_NULL) || (proc->buff_len == 0)) {
        return;
    }

    proc_tmp.proc_buff = proc->proc_buff;
    proc_tmp.buff_len  = proc->buff_len;

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "-----LCAC INFO-----------------------------------\n");

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "%12s" "%12s" "%12s\n", "Enable", "CrStr", "CbStr");

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "%12u" "%12u" "%12u\n",
                    local_cac->local_cac_en, local_cac->actual_depurple_cr_str, local_cac->actual_depurple_cb_str);

    proc->write_len += 1;
}

hi_s32 isp_lcac_ctrl(hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value)
{
    isp_reg_cfg_attr  *local_reg_cfg = HI_NULL;

    switch (cmd) {
        case ISP_WDR_MODE_SET:
            isp_regcfg_get_ctx(vi_pipe, local_reg_cfg);
            isp_check_pointer_return(local_reg_cfg);
            isp_lcac_wdr_mode_set(vi_pipe, (hi_void *)&local_reg_cfg->reg_cfg);
            break;
        case ISP_PROC_WRITE:
            lcac_proc_write(vi_pipe, (hi_isp_ctrl_proc_write *)value);
            break;
        default:
            break;
    }
    return HI_SUCCESS;
}

hi_s32 isp_lcac_exit(hi_vi_pipe vi_pipe)
{
    hi_u8 i;
    isp_reg_cfg_attr  *local_reg_cfg = HI_NULL;

    isp_regcfg_get_ctx(vi_pipe, local_reg_cfg);

    for (i = 0; i < local_reg_cfg->reg_cfg.cfg_num; i++) {
        local_reg_cfg->reg_cfg.alg_reg_cfg[i].lcac_reg_cfg.local_cac_en = HI_FALSE;
    }

    local_reg_cfg->reg_cfg.cfg_key.bit1_local_cac_cfg = 1;

    lcac_ctx_exit(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_alg_register_lcac(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_alg_node *algs = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_alg_check_return(isp_ctx->alg_key.bit1_lcac);
    algs = isp_search_alg(isp_ctx->algs);
    isp_check_pointer_return(algs);

    algs->alg_type = ISP_ALG_LCAC;
    algs->alg_func.pfn_alg_init = isp_lcac_init;
    algs->alg_func.pfn_alg_run  = isp_lcac_run;
    algs->alg_func.pfn_alg_ctrl = isp_lcac_ctrl;
    algs->alg_func.pfn_alg_exit = isp_lcac_exit;
    algs->used = HI_TRUE;

    return HI_SUCCESS;
}

