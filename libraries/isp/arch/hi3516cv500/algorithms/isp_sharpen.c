/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_sharpen.c
 * Author: ISP SW
 * Create: 2012/06/28
 */
#include "isp_sharpen.h"
#include "isp_config.h"
#include "hi_isp_debug.h"
#include "isp_ext_config.h"
#include "isp_math_utils.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_proc.h"

#define     SHRP_DIR_RANGE_MUL_PRECS            4
#define     SHRP_HF_EPS_MUL_PRECS               4
#define     SHRP_SHT_VAR_MUL_PRECS              4
#define     SHRP_SKIN_EDGE_MUL_PRECS            4
#define     SHRP_SKIN_ACCUM_MUL_PRECS           3
#define     SHRP_CHR_MUL_SFT                    4
#define     SHRP_DETAIL_SHT_MUL_PRECS           4
#define     SHRP_DETAIL_CTRL_THR_DELTA          16
#define     SHRP_ISO_NUM                        ISP_AUTO_ISO_STRENGTH_NUM

static isp_sharpen_ctx *g_sharpen_ctx[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define sharpen_get_ctx(dev, ctx)   ((ctx) = g_sharpen_ctx[dev])
#define sharpen_set_ctx(dev, ctx)   (g_sharpen_ctx[dev] = (ctx))
#define sharpen_reset_ctx(dev)      (g_sharpen_ctx[dev] = HI_NULL)

static hi_s32 shrp_blend(hi_u8 sft, hi_s32 wgt1, hi_s32 v1, hi_s32 wgt2, hi_s32 v2)
{
    hi_s32 res;
    res = signed_right_shift(((hi_s64)v1 * wgt1) + ((hi_s64)v2 * wgt2), sft);
    return res;
}

static hi_s32 sharpen_ctx_init(hi_vi_pipe vi_pipe)
{
    isp_sharpen_ctx *sharpen_ctx = HI_NULL;

    sharpen_get_ctx(vi_pipe, sharpen_ctx);

    if (sharpen_ctx == HI_NULL) {
        sharpen_ctx = (isp_sharpen_ctx *)isp_malloc(sizeof(isp_sharpen_ctx));
        if (sharpen_ctx == HI_NULL) {
            isp_err_trace("isp[%d] sharpen_ctx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset_s(sharpen_ctx, sizeof(isp_sharpen_ctx), 0, sizeof(isp_sharpen_ctx));

    sharpen_set_ctx(vi_pipe, sharpen_ctx);

    return HI_SUCCESS;
}

static hi_void sharpen_ctx_exit(hi_vi_pipe vi_pipe)
{
    isp_sharpen_ctx *sharpen_ctx = HI_NULL;

    sharpen_get_ctx(vi_pipe, sharpen_ctx);
    isp_free(sharpen_ctx);
    sharpen_reset_ctx(vi_pipe);
}

static hi_s32 sharpen_check_cmos_auto_strength_param(const hi_isp_cmos_sharpen *sharpen)
{
    hi_u8 i, j;

    for (i = 0; i < SHRP_ISO_NUM; i++) {
        for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
            if (sharpen->auto_attr.texture_str[j][i] > 4095) { /* Range: [0, 4095] */
                isp_err_trace("invalid auto.texture_str[%d][%d]! range:[0, 4095]\n", j, i);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
            if (sharpen->auto_attr.edge_str[j][i] > 4095) { /* Range: [0, 4095] */
                isp_err_trace("invalid auto.edge_str[%d][%d]! range:[0, 4095]\n", j, i);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
        }

        for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
            if (sharpen->auto_attr.luma_wgt[j][i] > 127) { /* Range: [0, 127] */
                isp_err_trace("invalid auto.luma_wgt[%d][%d]! range:[0, 127]\n", j, i);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
        }

        if (sharpen->auto_attr.texture_freq[i] > 4095) { /* Range: [0, 4095] */
            isp_err_trace("invalid auto.texture_freq[%d]! range:[0, 4095]\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (sharpen->auto_attr.edge_freq[i] > 4095) { /* Range: [0, 4095] */
            isp_err_trace("invalid auto.edge_freq[%d]! range:[0, 4095]\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    return HI_SUCCESS;
}

static hi_s32 sharpen_check_cmos_auto_shoot_param(const hi_isp_cmos_sharpen *sharpen)
{
    hi_u8 i;

    for (i = 0; i < SHRP_ISO_NUM; i++) {
        if (sharpen->auto_attr.over_shoot[i] > 127) { /* Range: [0, 127] */
            isp_err_trace("invalid auto.over_shoot[%d]! range:[0, 127]\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (sharpen->auto_attr.under_shoot[i] > 127) { /* Range: [0, 127] */
            isp_err_trace("invalid auto.under_shoot[%d]! range:[0, 127]\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (sharpen->auto_attr.shoot_sup_adj[i] > 15) { /* Range: [0, 15] */
            isp_err_trace("invalid auto.shoot_sup_adj[%d]! range:[0, 15]\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (sharpen->auto_attr.edge_filt_str[i] > 63) { /* Range: [0, 63] */
            isp_err_trace("invalid auto.edge_filt_str[%d]! range:[0, 63]\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (sharpen->auto_attr.edge_filt_max_cap[i] > HI_ISP_SHARPEN_EdgeFiltMaxCap_MAX) {
            isp_err_trace("invalid auto.edge_filt_max_cap[%d]! range:[0, %d]\n", i, HI_ISP_SHARPEN_EdgeFiltMaxCap_MAX);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (sharpen->auto_attr.r_gain[i] > HI_ISP_SHARPEN_RGAIN_MAX) {
            isp_err_trace("invalid auto.r_gain[%d]! range:[0, %d]\n", i, HI_ISP_SHARPEN_RGAIN_MAX);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (sharpen->auto_attr.g_gain[i] > HI_ISP_SHARPEN_GGAIN_MAX) {
            isp_err_trace("invalid auto.g_gain[%d]! range:[0, %d]\n", i, HI_ISP_SHARPEN_GGAIN_MAX);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (sharpen->auto_attr.b_gain[i] > HI_ISP_SHARPEN_BGAIN_MAX) {
            isp_err_trace("invalid auto.b_gain[%d]! range:[0, %d]\n", i, HI_ISP_SHARPEN_BGAIN_MAX);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (sharpen->auto_attr.skin_gain[i] > 31) { /* Range: [0, 31] */
            isp_err_trace("invalid auto.skin_gain[%d]! range:[0, 31]\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (sharpen->auto_attr.max_sharp_gain[i] > 0x7FF) { /* Range: [0, 0x7FF] */
            isp_err_trace("invalid auto.max_sharp_gain[%d]! range:[0, 0x7FF]\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    return HI_SUCCESS;
}

static hi_s32 sharpen_check_cmos_manual_strength_param(const hi_isp_cmos_sharpen *sharpen)
{
    hi_u8 j;

    for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
        if (sharpen->manual_attr.texture_str[j] > 4095) { /* Range: [0, 4095] */
            isp_err_trace("invalid manual.texture_str[%d]! range:[0, 4095]\n", j);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        if (sharpen->manual_attr.edge_str[j] > 4095) { /* Range: [0, 4095] */
            isp_err_trace("invalid manual.edge_str[%d]! range:[0, 4095]\n", j);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
        if (sharpen->manual_attr.luma_wgt[j] > 127) { /* Range: [0, 127] */
            isp_err_trace("invalid manual.luma_wgt[%d]! range:[0, 127]\n", j);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    if (sharpen->manual_attr.texture_freq > 4095) { /* Range: [0, 4095] */
        isp_err_trace("invalid manual.texture_freq! range:[0, 4095]\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (sharpen->manual_attr.edge_freq > 4095) { /* Range: [0, 4095] */
        isp_err_trace("invalid manual.edge_freq! range:[0, 4095]\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

static hi_s32 sharpen_check_cmos_manual_shoot_param(const hi_isp_cmos_sharpen *sharpen)
{
    if (sharpen->manual_attr.over_shoot > 127) { /* Range: [0, 127] */
        isp_err_trace("invalid manual.over_shoot! range:[0, 127]\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (sharpen->manual_attr.under_shoot > 127) { /* Range: [0, 127] */
        isp_err_trace("invalid manual.under_shoot! range:[0, 127]\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (sharpen->manual_attr.shoot_sup_adj > 15) { /* Range: [0, 15] */
        isp_err_trace("invalid manual.shoot_sup_adj! range:[0, 15]\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (sharpen->manual_attr.edge_filt_str > 63) { /* Range: [0, 63] */
        isp_err_trace("invalid manual.edge_filt_str! range:[0, 63]\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (sharpen->manual_attr.edge_filt_max_cap > HI_ISP_SHARPEN_EdgeFiltMaxCap_MAX) {
        isp_err_trace("invalid manual.edge_filt_max_cap! range:[0, %d]\n", HI_ISP_SHARPEN_EdgeFiltMaxCap_MAX);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (sharpen->manual_attr.r_gain > HI_ISP_SHARPEN_RGAIN_MAX) {
        isp_err_trace("invalid manual.r_gain! range:[0, %d]\n", HI_ISP_SHARPEN_RGAIN_MAX);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (sharpen->manual_attr.g_gain > HI_ISP_SHARPEN_GGAIN_MAX) {
        isp_err_trace("invalid manual.g_gain! range:[0, %d]\n", HI_ISP_SHARPEN_GGAIN_MAX);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (sharpen->manual_attr.b_gain > HI_ISP_SHARPEN_BGAIN_MAX) {
        isp_err_trace("invalid manual.b_gain! range:[0, %d]\n", HI_ISP_SHARPEN_BGAIN_MAX);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (sharpen->manual_attr.skin_gain > 31) { /* Range: [0, 31] */
        isp_err_trace("invalid manual.skin_gain! range:[0, 31]\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (sharpen->manual_attr.max_sharp_gain > 0x7FF) { /* Range: [0, 0x7FF] */
        isp_err_trace("invalid manual.max_sharp_gain! range:[0, 0x7FF]\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

static hi_s32 sharpen_check_cmos_param(hi_vi_pipe vi_pipe, const hi_isp_cmos_sharpen *sharpen)
{
    hi_s32 ret;

    ret = sharpen_check_cmos_auto_strength_param(sharpen);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    ret = sharpen_check_cmos_auto_shoot_param(sharpen);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    ret = sharpen_check_cmos_manual_strength_param(sharpen);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    ret = sharpen_check_cmos_manual_shoot_param(sharpen);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    return HI_SUCCESS;
}

static hi_void sharpen_ctx_cmos_auto_initialize(const hi_isp_cmos_sharpen_auto *cmos_auto,
                                                hi_isp_sharpen_auto_attr *auto_ctx)
{
    memcpy_s(auto_ctx->luma_wgt, ISP_SHARPEN_LUMA_NUM * ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
             cmos_auto->luma_wgt, ISP_SHARPEN_LUMA_NUM * ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
    memcpy_s(auto_ctx->texture_str, ISP_SHARPEN_GAIN_NUM * ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16),
             cmos_auto->texture_str, ISP_SHARPEN_GAIN_NUM * ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16));
    memcpy_s(auto_ctx->edge_str, ISP_SHARPEN_GAIN_NUM * ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16),
             cmos_auto->edge_str, ISP_SHARPEN_GAIN_NUM * ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16));
    memcpy_s(auto_ctx->texture_freq, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16),
             cmos_auto->texture_freq, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16));
    memcpy_s(auto_ctx->edge_freq, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16),
             cmos_auto->edge_freq, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16));
    memcpy_s(auto_ctx->over_shoot, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
             cmos_auto->over_shoot, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
    memcpy_s(auto_ctx->under_shoot, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
             cmos_auto->under_shoot, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
    memcpy_s(auto_ctx->shoot_sup_str, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
             cmos_auto->shoot_sup_str, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
    memcpy_s(auto_ctx->shoot_sup_adj, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
             cmos_auto->shoot_sup_adj, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
    memcpy_s(auto_ctx->detail_ctrl, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
             cmos_auto->detail_ctrl, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
    memcpy_s(auto_ctx->detail_ctrl_thr, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
             cmos_auto->detail_ctrl_thr, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
    memcpy_s(auto_ctx->edge_filt_str, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
             cmos_auto->edge_filt_str, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
    memcpy_s(auto_ctx->edge_filt_max_cap, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
             cmos_auto->edge_filt_max_cap, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
    memcpy_s(auto_ctx->r_gain, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
             cmos_auto->r_gain, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
    memcpy_s(auto_ctx->g_gain, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
             cmos_auto->g_gain, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
    memcpy_s(auto_ctx->b_gain, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
             cmos_auto->b_gain, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
    memcpy_s(auto_ctx->skin_gain, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
             cmos_auto->skin_gain, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
    memcpy_s(auto_ctx->max_sharp_gain, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16),
             cmos_auto->max_sharp_gain, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16));
}

static hi_void sharpen_ctx_cmos_manual_initialize(const hi_isp_cmos_sharpen_manual *cmos_manual,
                                                  hi_isp_sharpen_manual_attr *manual_ctx)
{
    memcpy_s(manual_ctx->luma_wgt, ISP_SHARPEN_LUMA_NUM * sizeof(hi_u8),
             cmos_manual->luma_wgt, ISP_SHARPEN_LUMA_NUM * sizeof(hi_u8));
    memcpy_s(manual_ctx->texture_str, ISP_SHARPEN_GAIN_NUM * sizeof(hi_u16),
             cmos_manual->texture_str, ISP_SHARPEN_GAIN_NUM * sizeof(hi_u16));
    memcpy_s(manual_ctx->edge_str, ISP_SHARPEN_GAIN_NUM * sizeof(hi_u16),
             cmos_manual->edge_str, ISP_SHARPEN_GAIN_NUM * sizeof(hi_u16));

    manual_ctx->texture_freq      = cmos_manual->texture_freq;
    manual_ctx->edge_freq         = cmos_manual->edge_freq;
    manual_ctx->over_shoot        = cmos_manual->over_shoot;
    manual_ctx->under_shoot       = cmos_manual->under_shoot;
    manual_ctx->shoot_sup_str     = cmos_manual->shoot_sup_str;
    manual_ctx->shoot_sup_adj     = cmos_manual->shoot_sup_adj;
    manual_ctx->detail_ctrl       = cmos_manual->detail_ctrl;
    manual_ctx->detail_ctrl_thr   = cmos_manual->detail_ctrl_thr;
    manual_ctx->edge_filt_str     = cmos_manual->edge_filt_str;
    manual_ctx->edge_filt_max_cap = cmos_manual->edge_filt_max_cap;
    manual_ctx->r_gain            = cmos_manual->r_gain;
    manual_ctx->g_gain            = cmos_manual->g_gain;
    manual_ctx->b_gain            = cmos_manual->b_gain;
    manual_ctx->skin_gain         = cmos_manual->skin_gain;
    manual_ctx->max_sharp_gain    = cmos_manual->max_sharp_gain;
}

static hi_void sharpen_ctx_def_auto_initialize(hi_isp_sharpen_auto_attr *auto_ctx)
{
    hi_u8 i, j;
    for (i = 0; i < SHRP_ISO_NUM; i++) {
        for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
            auto_ctx->texture_str[j][i] = HI_EXT_SYSTEM_MANUAL_SHARPEN_TEXTURESTR_DEF;
            auto_ctx->edge_str[j][i]    = HI_EXT_SYSTEM_MANUAL_SHARPEN_EDGESTR_DEF;
        }
        for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
            auto_ctx->luma_wgt[j][i]    = HI_EXT_SYSTEM_MANUAL_SHARPEN_LUMAWGT_DEF;
        }
        auto_ctx->texture_freq[i]       = HI_EXT_SYSTEM_MANUAL_SHARPEN_TEXTUREFREQ_DEF;
        auto_ctx->edge_freq[i]          = HI_EXT_SYSTEM_MANUAL_SHARPEN_EDGEFREQ_DEF;
        auto_ctx->over_shoot[i]         = HI_EXT_SYSTEM_MANUAL_SHARPEN_OVERSHOOT_DEF;
        auto_ctx->under_shoot[i]        = HI_EXT_SYSTEM_MANUAL_SHARPEN_UNDERSHOOT_DEF;
        auto_ctx->shoot_sup_str[i]      = HI_EXT_SYSTEM_MANUAL_SHARPEN_SHOOTSUPSTR_DEF;
        auto_ctx->detail_ctrl[i]        = HI_EXT_SYSTEM_MANUAL_SHARPEN_DETAILCTRL_DEF;
        auto_ctx->edge_filt_str[i]      = HI_EXT_SYSTEM_MANUAL_SHARPEN_EDGEFILTSTR_DEF;
        auto_ctx->edge_filt_max_cap[i]  = HI_EXT_SYSTEM_MANUAL_SHARPEN_EDGEFILTMAXCAP_DEF;
        auto_ctx->r_gain[i]             = HI_EXT_SYSTEM_MANUAL_SHARPEN_RGAIN_DEF;
        auto_ctx->g_gain[i]             = HI_EXT_SYSTEM_MANUAL_SHARPEN_GGAIN_DEF;
        auto_ctx->b_gain[i]             = HI_EXT_SYSTEM_MANUAL_SHARPEN_BGAIN_DEF;
        auto_ctx->skin_gain[i]          = HI_EXT_SYSTEM_MANUAL_SHARPEN_SKINGAIN_DEF;
        auto_ctx->shoot_sup_adj[i]      = HI_EXT_SYSTEM_MANUAL_SHARPEN_SHOOTSUPADJ_DEF;
        auto_ctx->max_sharp_gain[i]     = HI_EXT_SYSTEM_MANUAL_SHARPEN_MAXSHARPGAIN_DEF;
        auto_ctx->detail_ctrl_thr[i]    = HI_EXT_SYSTEM_MANUAL_SHARPEN_DETAILCTRLTHR_DEF;
    }
}

static hi_void sharpen_ctx_def_manual_initialize(hi_isp_sharpen_manual_attr *manual_ctx)
{
    hi_u8 i;

    for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
        manual_ctx->texture_str[i] = HI_EXT_SYSTEM_MANUAL_SHARPEN_TEXTURESTR_DEF;
        manual_ctx->edge_str[i]    = HI_EXT_SYSTEM_MANUAL_SHARPEN_EDGESTR_DEF;
    }
    for (i = 0; i < ISP_SHARPEN_LUMA_NUM; i++) {
        manual_ctx->luma_wgt[i]    = HI_EXT_SYSTEM_MANUAL_SHARPEN_LUMAWGT_DEF;
    }
    manual_ctx->texture_freq       = HI_EXT_SYSTEM_MANUAL_SHARPEN_TEXTUREFREQ_DEF;
    manual_ctx->edge_freq          = HI_EXT_SYSTEM_MANUAL_SHARPEN_EDGEFREQ_DEF;
    manual_ctx->over_shoot         = HI_EXT_SYSTEM_MANUAL_SHARPEN_OVERSHOOT_DEF;
    manual_ctx->under_shoot        = HI_EXT_SYSTEM_MANUAL_SHARPEN_UNDERSHOOT_DEF;
    manual_ctx->shoot_sup_str      = HI_EXT_SYSTEM_MANUAL_SHARPEN_SHOOTSUPSTR_DEF;
    manual_ctx->detail_ctrl        = HI_EXT_SYSTEM_MANUAL_SHARPEN_DETAILCTRL_DEF;
    manual_ctx->edge_filt_str      = HI_EXT_SYSTEM_MANUAL_SHARPEN_EDGEFILTSTR_DEF;
    manual_ctx->edge_filt_max_cap  = HI_EXT_SYSTEM_MANUAL_SHARPEN_EDGEFILTMAXCAP_DEF;
    manual_ctx->r_gain             = HI_EXT_SYSTEM_MANUAL_SHARPEN_RGAIN_DEF;
    manual_ctx->g_gain             = HI_EXT_SYSTEM_MANUAL_SHARPEN_GGAIN_DEF;
    manual_ctx->b_gain             = HI_EXT_SYSTEM_MANUAL_SHARPEN_BGAIN_DEF;
    manual_ctx->skin_gain          = HI_EXT_SYSTEM_MANUAL_SHARPEN_SKINGAIN_DEF;
    manual_ctx->shoot_sup_adj      = HI_EXT_SYSTEM_MANUAL_SHARPEN_SHOOTSUPADJ_DEF;
    manual_ctx->max_sharp_gain     = HI_EXT_SYSTEM_MANUAL_SHARPEN_MAXSHARPGAIN_DEF;
    manual_ctx->detail_ctrl_thr    = HI_EXT_SYSTEM_MANUAL_SHARPEN_DETAILCTRLTHR_DEF;
}

static hi_s32 isp_sharpen_ctx_initialize(hi_vi_pipe vi_pipe, isp_sharpen_ctx *shp_ctx)
{
    hi_s32 ret;
    hi_isp_cmos_default *sns_dft = HI_NULL;

    isp_sensor_get_default(vi_pipe, &sns_dft);

    shp_ctx->mpi_cfg.enable        = HI_TRUE;
    shp_ctx->sharpen_mpi_update_en = HI_TRUE;
    shp_ctx->iso_last              = 0;
    shp_ctx->mpi_cfg.op_type       = OP_TYPE_AUTO;

    shp_ctx->mf_thd_sft_d       = 0;  /* mf thd sft d       0  */
    shp_ctx->dir_var_sft        = 10; /* dir var sft        10 */
    shp_ctx->range_thd0         = 25; /* range thd0         25 */
    shp_ctx->range_dir_wgt0     = 0;  /* range dir wgt0     0  */
    shp_ctx->sel_pix_wgt        = 31; /* sel pix wgt        31 */

    shp_ctx->rmf_gain_scale     = 2;  /* rmf gain scale     2  */
    shp_ctx->bmf_gain_scale     = 4;  /* bmf gain scale     4  */
    shp_ctx->mf_thd_sel_ud      = 2;  /* mf thd sel ud      2  */
    shp_ctx->mf_thd_sft_ud      = 0;  /* mf thd sft ud      0  */
    shp_ctx->hf_eps_var_thr0    = 20; /* hf eps var thr0    20 */
    shp_ctx->hf_eps_var_thr1    = 30; /* hf eps var thr1    30 */
    shp_ctx->hf_eps_val1        = 20; /* hf eps val1        20 */

    shp_ctx->osht_var_wgt0      = 10; /* osht var wgt0      10 */
    shp_ctx->usht_var_wgt0      = 20; /* usht var wgt0      20 */
    shp_ctx->osht_var_diff_thd0 = 20; /* osht var diff thd0 20 */
    shp_ctx->osht_var_diff_thd1 = 35; /* osht var diff thd1 35 */
    shp_ctx->osht_var_diff_wgt1 = 20; /* osht var diff wgt1 20 */
    shp_ctx->usht_var_diff_wgt1 = 35; /* usht var diff wgt1 35 */

    /* auto ext_regs initial */
    if (sns_dft->key.bit1_sharpen) {
        isp_check_pointer_return(sns_dft->sharpen);

        ret = sharpen_check_cmos_param(vi_pipe, sns_dft->sharpen);
        if (ret != HI_SUCCESS) {
            return ret;
        }

        shp_ctx->mpi_cfg.skin_umax = sns_dft->sharpen->skin_umax;
        shp_ctx->mpi_cfg.skin_umin = sns_dft->sharpen->skin_umin;
        shp_ctx->mpi_cfg.skin_vmax = sns_dft->sharpen->skin_vmax;
        shp_ctx->mpi_cfg.skin_vmin = sns_dft->sharpen->skin_vmin;
        sharpen_ctx_cmos_auto_initialize(&sns_dft->sharpen->auto_attr, &shp_ctx->mpi_cfg.auto_attr);
        sharpen_ctx_cmos_manual_initialize(&sns_dft->sharpen->manual_attr, &shp_ctx->mpi_cfg.manual_attr);
    } else {
        shp_ctx->mpi_cfg.skin_umax = HI_EXT_SYSTEM_MANUAL_SHARPEN_SKINUMAX_DEF;
        shp_ctx->mpi_cfg.skin_umin = HI_EXT_SYSTEM_MANUAL_SHARPEN_SKINUMIN_DEF;
        shp_ctx->mpi_cfg.skin_vmin = HI_EXT_SYSTEM_MANUAL_SHARPEN_SKINVMIN_DEF;
        shp_ctx->mpi_cfg.skin_vmax = HI_EXT_SYSTEM_MANUAL_SHARPEN_SKINVMAX_DEF;

        sharpen_ctx_def_auto_initialize(&shp_ctx->mpi_cfg.auto_attr);
        sharpen_ctx_def_manual_initialize(&shp_ctx->mpi_cfg.manual_attr);
    }

    return HI_SUCCESS;
}

static hi_void sharpen_ext_regs_auto_initialize(hi_vi_pipe vi_pipe, hi_isp_sharpen_auto_attr *auto_ctx)
{
    hi_u16 i, j, index;

    for (i = 0; i < SHRP_ISO_NUM; i++) {
        for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
            index = i + j * SHRP_ISO_NUM;
            hi_ext_system_sharpen_texture_str_write(vi_pipe, index, auto_ctx->texture_str[j][i]);
            hi_ext_system_sharpen_edge_str_write(vi_pipe, index, auto_ctx->edge_str[j][i]);
        }
        for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
            index = i + j * SHRP_ISO_NUM;
            hi_ext_system_sharpen_luma_wgt_write(vi_pipe, index, auto_ctx->luma_wgt[j][i]);
        }
        hi_ext_system_sharpen_texture_freq_write     (vi_pipe, i, auto_ctx->texture_freq[i]);
        hi_ext_system_sharpen_edge_freq_write        (vi_pipe, i, auto_ctx->edge_freq[i]);
        hi_ext_system_sharpen_over_shoot_write       (vi_pipe, i, auto_ctx->over_shoot[i]);
        hi_ext_system_sharpen_under_shoot_write      (vi_pipe, i, auto_ctx->under_shoot[i]);
        hi_ext_system_sharpen_shoot_sup_str_write    (vi_pipe, i, auto_ctx->shoot_sup_str[i]);
        hi_ext_system_sharpen_detailctrl_write       (vi_pipe, i, auto_ctx->detail_ctrl[i]);
        hi_ext_system_sharpen_edge_filt_str_write    (vi_pipe, i, auto_ctx->edge_filt_str[i]);
        hi_ext_system_sharpen_edge_filt_max_cap_write(vi_pipe, i, auto_ctx->edge_filt_max_cap[i]);
        hi_ext_system_sharpen_r_gain_write           (vi_pipe, i, auto_ctx->r_gain[i]);
        hi_ext_system_sharpen_g_gain_write           (vi_pipe, i, auto_ctx->g_gain[i]);
        hi_ext_system_sharpen_b_gain_write           (vi_pipe, i, auto_ctx->b_gain[i]);
        hi_ext_system_sharpen_skin_gain_write        (vi_pipe, i, auto_ctx->skin_gain[i]);
        hi_ext_system_sharpen_shoot_sup_adj_write    (vi_pipe, i, auto_ctx->shoot_sup_adj[i]);
        hi_ext_system_sharpen_max_sharp_gain_write   (vi_pipe, i, auto_ctx->max_sharp_gain[i]);
        hi_ext_system_sharpen_detailctrl_thr_write   (vi_pipe, i, auto_ctx->detail_ctrl_thr[i]);
    }
}

static hi_void sharpen_ext_regs_manual_initialize(hi_vi_pipe vi_pipe, hi_isp_sharpen_manual_attr *manual_ctx)
{
    hi_u16 i;

    for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
        hi_ext_system_manual_sharpen_texture_str_write(vi_pipe, i, manual_ctx->texture_str[i]);
        hi_ext_system_manual_sharpen_edge_str_write(vi_pipe, i, manual_ctx->edge_str[i]);
    }

    for (i = 0; i < ISP_SHARPEN_LUMA_NUM; i++) {
        hi_ext_system_manual_sharpen_luma_wgt_write(vi_pipe, i, manual_ctx->luma_wgt[i]);
    }

    hi_ext_system_manual_sharpen_texture_freq_write(vi_pipe, manual_ctx->texture_freq);
    hi_ext_system_manual_sharpen_edge_freq_write(vi_pipe, manual_ctx->edge_freq);
    hi_ext_system_manual_sharpen_over_shoot_write(vi_pipe, manual_ctx->over_shoot);
    hi_ext_system_manual_sharpen_under_shoot_write(vi_pipe, manual_ctx->under_shoot);
    hi_ext_system_manual_sharpen_shoot_sup_str_write(vi_pipe, manual_ctx->shoot_sup_str);
    hi_ext_system_manual_sharpen_detailctrl_write(vi_pipe, manual_ctx->detail_ctrl);
    hi_ext_system_manual_sharpen_edge_filt_str_write(vi_pipe, manual_ctx->edge_filt_str);
    hi_ext_system_manual_sharpen_edge_filt_max_cap_write(vi_pipe, manual_ctx->edge_filt_max_cap);
    hi_ext_system_manual_sharpen_r_gain_write(vi_pipe, manual_ctx->r_gain);
    hi_ext_system_manual_sharpen_g_gain_write(vi_pipe, manual_ctx->g_gain);
    hi_ext_system_manual_sharpen_b_gain_write(vi_pipe, manual_ctx->b_gain);
    hi_ext_system_manual_sharpen_skin_gain_write(vi_pipe, manual_ctx->skin_gain);
    hi_ext_system_manual_sharpen_shoot_sup_adj_write(vi_pipe, manual_ctx->shoot_sup_adj);
    hi_ext_system_manual_sharpen_max_sharp_gain_write(vi_pipe, manual_ctx->max_sharp_gain);
    hi_ext_system_manual_sharpen_detailctrl_thr_write(vi_pipe, manual_ctx->detail_ctrl_thr);

    /* for InnerState */
    hi_ext_system_actual_sharpen_overshoot_amt_write(vi_pipe, manual_ctx->over_shoot);
    hi_ext_system_actual_sharpen_undershoot_amt_write(vi_pipe, manual_ctx->under_shoot);
    hi_ext_system_actual_sharpen_shoot_sup_write(vi_pipe, manual_ctx->shoot_sup_str);
    hi_ext_system_actual_sharpen_edge_frequence_write(vi_pipe, manual_ctx->edge_freq);
    hi_ext_system_actual_sharpen_texture_frequence_write(vi_pipe, manual_ctx->texture_freq);

    for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
        hi_ext_system_actual_sharpen_edge_str_write(vi_pipe, i, manual_ctx->edge_str[i]);
        hi_ext_system_actual_sharpen_texture_str_write(vi_pipe, i, manual_ctx->texture_str[i]);
    }
}

static hi_void sharpen_ext_regs_initialize(hi_vi_pipe vi_pipe, isp_sharpen_ctx *shp_ctx)
{
    hi_ext_system_sharpen_manu_mode_write(vi_pipe, shp_ctx->mpi_cfg.op_type);
    hi_ext_system_manual_sharpen_en_write(vi_pipe, shp_ctx->mpi_cfg.enable);
    hi_ext_system_sharpen_mpi_update_en_write(vi_pipe, shp_ctx->sharpen_mpi_update_en);

    hi_ext_system_manual_sharpen_skin_umax_write(vi_pipe, shp_ctx->mpi_cfg.skin_umax);
    hi_ext_system_manual_sharpen_skin_umin_write(vi_pipe, shp_ctx->mpi_cfg.skin_umin);
    hi_ext_system_manual_sharpen_skin_vmax_write(vi_pipe, shp_ctx->mpi_cfg.skin_vmax);
    hi_ext_system_manual_sharpen_skin_vmin_write(vi_pipe, shp_ctx->mpi_cfg.skin_vmin);

    sharpen_ext_regs_auto_initialize(vi_pipe, &shp_ctx->mpi_cfg.auto_attr);
    sharpen_ext_regs_manual_initialize(vi_pipe, &shp_ctx->mpi_cfg.manual_attr);
}

/* sharpen hardware regs that will not change */
static hi_void sharpen_static_shoot_reg_init(isp_sharpen_static_reg_cfg *static_cfg)
{
    hi_u8 i;
    static_cfg->static_resh      = HI_TRUE;
    static_cfg->hf_thd_sel_d     = 1;
    static_cfg->mf_thd_sel_d     = 1;
    static_cfg->dir_var_scale    = 0;
    static_cfg->dir_rly[0]       = 127; /* dir rly max 127    */
    static_cfg->dir_rly[1]       = 0;
    static_cfg->range_thd1       = 55;  /* range thd1 55      */
    static_cfg->range_dir_wgt1   = 127; /* range dir wgt1 127 */
    static_cfg->en_hf_eps        = 1;
    static_cfg->hf_eps_val0      = 127; /* hf eps val0 127    */
    static_cfg->max_var_clip_min = 3;   /* max var clip min 3 */
    static_cfg->o_max_chg        = 180; /* oshoot max chg 180 */
    static_cfg->u_max_chg        = 200; /* ushoot max chg 200 */
    static_cfg->sht_var_sft      = 0;

    for (i = 0; i < ISP_SHARPEN_FREQ_CORING_LENGTH; i++) {
        static_cfg->lmt_mf[i]    = 0;
        static_cfg->lmt_hf[i]    = 0;
    }

    static_cfg->osht_var_wgt1      = 127; /* oshoot var wgt1 127 */
    static_cfg->usht_var_wgt1      = 127; /* ushoot var wgt1 127 */
    static_cfg->usht_var_diff_wgt0 = 127; /* ushoot dif wgt0 127 */
    static_cfg->osht_var_diff_wgt0 = 127; /* oshoot dif wgt0 127 */
    static_cfg->osht_var_thd0      = 0;
    static_cfg->usht_var_thd0      = 0;
    static_cfg->luma_src_sel       = 0;
    static_cfg->sht_noise_min      = 0;
    static_cfg->sht_noise_max      = 0;

    static_cfg->en_shp8_dir      = 1;

    static_cfg->hf_gain_sft      = 5; /* hf gain sft 5 */
    static_cfg->mf_gain_sft      = 5; /* mf gain sft 5 */
    static_cfg->sht_var_sel      = 1;
    static_cfg->sht_var5x5_sft   = 1;
    static_cfg->detail_thd_sel   = 0;
    static_cfg->detail_thd_sft   = 0;
}

static hi_void sharpen_static_chrome_reg_init(isp_sharpen_static_reg_cfg *static_cfg)
{
    static_cfg->chr_r_var_sft     = 7;   /* chr rvar sft 7  */
    static_cfg->rc_bmin           = 0;
    static_cfg->rc_bmax           = 255; /* chr rc bmax 255 */
    static_cfg->rc_rmin           = 0;
    static_cfg->rc_rmax           = 255; /* chr rc rmax 255 */
    static_cfg->chr_r_ori_cb      = 120; /* chr rori cb 120 */
    static_cfg->chr_r_ori_cr      = 220; /* chr rori cr 220 */
    static_cfg->chr_r_sft[0]      = 7;   /* chr red sft0 7  */
    static_cfg->chr_r_sft[1]      = 7;   /* chr red sft1 7  */
    static_cfg->chr_r_sft[2]      = 7;   /* chr red sft2 7  */
    static_cfg->chr_r_sft[3]      = 6;   /* chr red sft3 6  */
    static_cfg->chr_r_thd[0]      = 40;  /* chr red thd0 40 */
    static_cfg->chr_r_thd[1]      = 60;  /* chr red thd1 60 */

    static_cfg->chr_b_var_sft     = 2;   /* chr bvar sft 2  */
    static_cfg->bc_bmin           = 0;
    static_cfg->bc_bmax           = 255; /* chr bc bmax 255 */
    static_cfg->bc_rmin           = 0;
    static_cfg->bc_rmax           = 255; /* chr bc rmax 255 */
    static_cfg->chr_b_ori_cb      = 200; /* chr bori cb 200 */
    static_cfg->chr_b_ori_cr      = 64;  /* chr bori cr 64  */
    static_cfg->chr_b_sft[0]      = 7;   /* chr blu sft0 7  */
    static_cfg->chr_b_sft[1]      = 7;   /* chr blu sft1 7  */
    static_cfg->chr_b_sft[2]      = 7;   /* chr blu sft2 7  */
    static_cfg->chr_b_sft[3]      = 7;   /* chr blu sft3 7  */
    static_cfg->chr_b_thd[0]      = 40;  /* chr blu thd0 40 */
    static_cfg->chr_b_thd[1]      = 90;  /* chr blu thd1 90 */

    static_cfg->gc_bmin           = 60;  /* chr gc bmax  60 */
    static_cfg->gc_bmax           = 120; /* chr gc bmax 120 */
    static_cfg->gc_rmin           = 90;  /* chr gc bmax  90 */
    static_cfg->gc_rmax           = 130; /* chr gc rmax 130 */
    static_cfg->chr_g_ori_cb      = 90;  /* chr gori cb  90 */
    static_cfg->chr_g_ori_cr      = 110; /* chr gori cr 110 */
    static_cfg->chr_g_sft[0]      = 4;   /* chr gre sft0 4  */
    static_cfg->chr_g_sft[1]      = 7;   /* chr gre sft1 7  */
    static_cfg->chr_g_sft[2]      = 4;   /* chr gre sft2 4  */
    static_cfg->chr_g_sft[3]      = 7;   /* chr gre sft3 7  */
    static_cfg->chr_g_thd[0]      = 20;  /* chr gre thd0 20 */
    static_cfg->chr_g_thd[1]      = 40;  /* chr gre thd1 40 */
}

static hi_void sharpen_static_hsf_reg_init(isp_sharpen_static_reg_cfg *static_cfg)
{
    static_cfg->hsf_coef_ud[0]   = -2;   /* hsf coef ud0  -2 */
    static_cfg->hsf_coef_ud[1]   = 9;    /* hsf coef ud1  9  */
    static_cfg->hsf_coef_ud[2]   = 18;   /* hsf coef ud2  18 */

    static_cfg->hsf_coef_d0[0]   = -1;   /* hsf coef d00  -1 */
    static_cfg->hsf_coef_d0[1]   = -2;   /* hsf coef d01  -2 */
    static_cfg->hsf_coef_d0[2]   = -3;   /* hsf coef d02  -3 */
    static_cfg->hsf_coef_d0[3]   = -2;   /* hsf coef d03  -2 */
    static_cfg->hsf_coef_d0[4]   = -1;   /* hsf coef d04  -1 */
    static_cfg->hsf_coef_d0[5]   = 12;   /* hsf coef d05  12 */
    static_cfg->hsf_coef_d0[6]   = 27;   /* hsf coef d06  27 */
    static_cfg->hsf_coef_d0[7]   = 36;   /* hsf coef d07  36 */
    static_cfg->hsf_coef_d0[8]   = 27;   /* hsf coef d08  27 */
    static_cfg->hsf_coef_d0[9]   = 12;   /* hsf coef d09  12 */
    static_cfg->hsf_coef_d0[10]  = 31;   /* hsf coef d010 31 */
    static_cfg->hsf_coef_d0[11]  = 72;   /* hsf coef d011 72 */
    static_cfg->hsf_coef_d0[12]  = 96;   /* hsf coef d012 96 */

    static_cfg->hsf_coef_d1[0]   = -2;   /* hsf coef d10  -2 */
    static_cfg->hsf_coef_d1[1]   = -3;   /* hsf coef d11  -3 */
    static_cfg->hsf_coef_d1[2]   = -1;   /* hsf coef d12  -1 */
    static_cfg->hsf_coef_d1[3]   = 6;    /* hsf coef d13  6  */
    static_cfg->hsf_coef_d1[4]   = 6;    /* hsf coef d14  6  */
    static_cfg->hsf_coef_d1[5]   = 0;    /* hsf coef d15  0  */
    static_cfg->hsf_coef_d1[6]   = 13;   /* hsf coef d16  13 */
    static_cfg->hsf_coef_d1[7]   = 41;   /* hsf coef d17  41 */
    static_cfg->hsf_coef_d1[8]   = 47;   /* hsf coef d18  47 */
    static_cfg->hsf_coef_d1[9]   = 23;   /* hsf coef d19  23 */
    static_cfg->hsf_coef_d1[10]  = 18;   /* hsf coef d110 18 */
    static_cfg->hsf_coef_d1[11]  = 62;   /* hsf coef d111 62 */
    static_cfg->hsf_coef_d1[12]  = 92;   /* hsf coef d112 92 */

    static_cfg->hsf_coef_d2[0]   = -1;   /* hsf coef d20  -1 */
    static_cfg->hsf_coef_d2[1]   = -3;   /* hsf coef d21  -3 */
    static_cfg->hsf_coef_d2[2]   = 6;    /* hsf coef d22  6  */
    static_cfg->hsf_coef_d2[3]   = 16;   /* hsf coef d23  16 */
    static_cfg->hsf_coef_d2[4]   = 10;   /* hsf coef d24  10 */
    static_cfg->hsf_coef_d2[5]   = -3;   /* hsf coef d25  -3 */
    static_cfg->hsf_coef_d2[6]   = 10;   /* hsf coef d26  10 */
    static_cfg->hsf_coef_d2[7]   = 50;   /* hsf coef d27  50 */
    static_cfg->hsf_coef_d2[8]   = 53;   /* hsf coef d28  53 */
    static_cfg->hsf_coef_d2[9]   = 16;   /* hsf coef d29  16 */
    static_cfg->hsf_coef_d2[10]  = 6;    /* hsf coef d210 6  */
    static_cfg->hsf_coef_d2[11]  = 50;   /* hsf coef d211 50 */
    static_cfg->hsf_coef_d2[12]  = 92;   /* hsf coef d212 92 */
}

static hi_void sharpen_static_lpf_reg_init(isp_sharpen_static_reg_cfg *static_cfg)
{
    static_cfg->hsf_coef_d3[0]   = -2;   /* hsf coef d30  -2 */
    static_cfg->hsf_coef_d3[1]   = 0;    /* hsf coef d31  0  */
    static_cfg->hsf_coef_d3[2]   = 18;   /* hsf coef d32  18 */
    static_cfg->hsf_coef_d3[3]   = 23;   /* hsf coef d33  23 */
    static_cfg->hsf_coef_d3[4]   = 6;    /* hsf coef d34  6  */
    static_cfg->hsf_coef_d3[5]   = -3;   /* hsf coef d35  -3 */
    static_cfg->hsf_coef_d3[6]   = 13;   /* hsf coef d36  13 */
    static_cfg->hsf_coef_d3[7]   = 62;   /* hsf coef d37  62 */
    static_cfg->hsf_coef_d3[8]   = 47;   /* hsf coef d38  47 */
    static_cfg->hsf_coef_d3[9]   = 6;    /* hsf coef d39  6  */
    static_cfg->hsf_coef_d3[10]  = -1;   /* hsf coef d310 -1 */
    static_cfg->hsf_coef_d3[11]  = 41;   /* hsf coef d311 41 */
    static_cfg->hsf_coef_d3[12]  = 92;   /* hsf coef d312 92 */

    static_cfg->lpf_coef_ud[0]   = 5;    /* lpf coef ud0  5  */
    static_cfg->lpf_coef_ud[1]   = 7;    /* lpf coef ud1  7  */
    static_cfg->lpf_coef_ud[2]   = 8;    /* lpf coef ud2  8  */

    static_cfg->lpf_coef_d[0]    = 4;    /* lpf coef d0   4  */
    static_cfg->lpf_coef_d[1]    = 7;    /* lpf coef d1   7  */
    static_cfg->lpf_coef_d[2]    = 10;   /* lpf coef d2  10  */

    static_cfg->lpf_sft_ud       = 5;    /* lpf sft ud    5  */
    static_cfg->lpf_sft_d        = 5;    /* lpf sft d     5  */
    static_cfg->hsf_sft_ud       = 5;    /* hsf sft ud    5  */
    static_cfg->hsf_sft_d        = 9;    /* hsf sft d     9  */

    static_cfg->skin_src_sel      = 0;   /* skin src sel   0 */
    static_cfg->skin_cnt_thd[0]   = 5;   /* skin cnt thd0  5 */
    static_cfg->skin_edge_thd[0]  = 10;  /* skin edg thd0 10 */
    static_cfg->skin_accum_thd[0] = 0;   /* skin acc thd0  0 */
    static_cfg->skin_accum_wgt[0] = 0;   /* skin acc wgt0  0 */
    static_cfg->skin_cnt_thd[1]   = 8;   /* skin cnt thd1  8 */
    static_cfg->skin_edge_thd[1]  = 30;  /* skin edg thd1 30 */
    static_cfg->skin_accum_thd[1] = 10;  /* skin acc thd1 10 */
    static_cfg->skin_accum_wgt[1] = 20;  /* skin acc wgt1 20 */
    static_cfg->skin_edge_sft     = 1;   /* skin edg sft   1 */

    static_cfg->skin_cnt_mul   = calc_mul_coef(static_cfg->skin_cnt_thd[0], 0,
                                               static_cfg->skin_cnt_thd[1], 31, 0); /* Range: [0, 31] */
    static_cfg->skin_accum_mul = calc_mul_coef(static_cfg->skin_accum_thd[0], static_cfg->skin_accum_wgt[0],
                                               static_cfg->skin_accum_thd[1], static_cfg->skin_accum_wgt[1],
                                               SHRP_SKIN_ACCUM_MUL_PRECS);
}

/* sharpen hardware regs that will change with MPI and ISO */
static hi_void sharpen_mpi_dyna_reg_init(isp_sharpen_mpi_dyna_reg_cfg *mpi_dyna_cfg)
{
    hi_u8 i;

    for (i = 0; i < SHRP_GAIN_LUT_SIZE; i++) {
        mpi_dyna_cfg->mf_gain_d[i]  = 300;  /* mf gain d  300 */
        mpi_dyna_cfg->mf_gain_ud[i] = 200;  /* mf gain ud 200 */
        mpi_dyna_cfg->hf_gain_d[i]  = 450;  /* hf gain d  450 */
        mpi_dyna_cfg->hf_gain_ud[i] = 400;  /* hf gain ud 400 */
    }

    mpi_dyna_cfg->osht_amt           = 100; /* over  shoot amt 100  */
    mpi_dyna_cfg->usht_amt           = 127; /* under shoot amt 127  */
    mpi_dyna_cfg->en_sht_ctrl_by_var = 1;   /* sht ctrl by var 1    */
    mpi_dyna_cfg->sht_bld_rt         = 9;   /* shoot blend rt  9    */
    mpi_dyna_cfg->osht_var_thd1      = 5;   /* oshoot var thd1 5    */
    mpi_dyna_cfg->usht_var_thd1      = 5;   /* ushoot var thd1 5    */

    mpi_dyna_cfg->en_chr_ctrl        = 1;   /* en chrome ctrl   1   */
    mpi_dyna_cfg->chr_r_gain         = 6;   /* chrome red gain  6   */
    mpi_dyna_cfg->chr_g_gain         = 32;  /* chrome gre gain  32  */
    mpi_dyna_cfg->chr_gmf_gain       = 32;  /* chrome gmf gain  32  */
    mpi_dyna_cfg->chr_b_gain         = 14;  /* chrome blue gain 14  */
    mpi_dyna_cfg->en_skin_ctrl       = 0;   /* en skin ctrl     0   */
    mpi_dyna_cfg->skin_edge_wgt[1]   = 31;  /* skin edge wgt1   31  */
    mpi_dyna_cfg->skin_edge_wgt[0]   = 31;  /* skin edge wgt0   31  */

    mpi_dyna_cfg->en_luma_ctrl       = 0;   /* en luma   ctrl   0   */
    mpi_dyna_cfg->en_detail_ctrl     = 0;   /* en detail ctrl   0   */
    mpi_dyna_cfg->detail_osht_amt    = 100; /* detail osht amt  100 */
    mpi_dyna_cfg->detail_usht_amt    = 127; /* detail usht amt  127 */
    mpi_dyna_cfg->dir_diff_sft       = 10;  /* dir diff asft    10  */
    mpi_dyna_cfg->dir_rt[0]          = 6;   /* direction rt0    6   */
    mpi_dyna_cfg->dir_rt[1]          = 18;  /* direction rt1    18  */
    mpi_dyna_cfg->skin_max_u         = 127; /* skin max u       127 */
    mpi_dyna_cfg->skin_min_u         = 95;  /* skin min u       95  */
    mpi_dyna_cfg->skin_max_v         = 155; /* skin max v       155 */
    mpi_dyna_cfg->skin_min_v         = 135; /* skin min v       135 */
    mpi_dyna_cfg->o_max_gain         = 160; /* oshoot max gain  160 */
    mpi_dyna_cfg->u_max_gain         = 160; /* ushoot max gain  160 */
    mpi_dyna_cfg->detail_osht_thr[0] = 65;  /* detail osht thr0 65  */
    mpi_dyna_cfg->detail_osht_thr[1] = 90;  /* detail osht thr1 90  */
    mpi_dyna_cfg->detail_usht_thr[0] = 65;  /* detail usht thr0 65  */
    mpi_dyna_cfg->detail_usht_thr[1] = 90;  /* detail usht thr1 90  */

    for (i = 0; i < ISP_SHARPEN_LUMA_NUM; i++) {
        mpi_dyna_cfg->luma_wgt[i]  = 127;   /* luma weight      127 */
    }

    mpi_dyna_cfg->update_index      = 1;
    mpi_dyna_cfg->buf_id            = 0;
    mpi_dyna_cfg->resh              = HI_TRUE;
    mpi_dyna_cfg->pre_reg_new_en    = HI_FALSE;
    mpi_dyna_cfg->switch_mode       = HI_TRUE;
}

/* sharpen hardware regs that will change only with ISO */
static hi_void sharpen_default_dyna_reg_init(isp_sharpen_default_dyna_reg_cfg *def_dyna_cfg)
{
    def_dyna_cfg->resh                 = HI_TRUE;
    def_dyna_cfg->mf_thd_sft_d         = 0;  /* mf thd sft d       0  */
    def_dyna_cfg->mf_thd_sel_ud        = 2;  /* mf thd sel ud      2  */
    def_dyna_cfg->mf_thd_sft_ud        = 0;  /* mf thd sft ud      0  */
    def_dyna_cfg->hf_thd_sft_d         = 0;  /* hf thd sft d       0  */
    def_dyna_cfg->hf_thd_sel_ud        = 2;  /* hf thd sel ud      2  */
    def_dyna_cfg->hf_thd_sft_ud        = 0;  /* hf thd sft ud      0  */
    def_dyna_cfg->dir_var_sft          = 10; /* dir var sft        10 */
    def_dyna_cfg->range_thd0           = 25; /* range thd0         25 */
    def_dyna_cfg->range_dir_wgt0       = 0;  /* range dir wgt0     0  */
    def_dyna_cfg->hf_eps_var_thr0      = 20; /* hf eps var thr0    20 */
    def_dyna_cfg->hf_eps_var_thr1      = 30; /* hf eps var thr1    30 */
    def_dyna_cfg->hf_eps_val1          = 20; /* hf eps val1        20 */
    def_dyna_cfg->sel_pix_wgt          = 31; /* sel pix wgt        31 */
    def_dyna_cfg->osht_var_diff_thd[0] = 20; /* osht var diff thd0 20 */
    def_dyna_cfg->usht_var_diff_thd[0] = 20; /* usht var diff thd0 20 */
    def_dyna_cfg->osht_var_wgt0        = 0;  /* osht var wgt0      0  */
    def_dyna_cfg->usht_var_wgt0        = 0;  /* usht var wgt0      0  */
    def_dyna_cfg->osht_var_diff_thd[1] = 35; /* osht var diff thd1 35 */
    def_dyna_cfg->osht_var_diff_wgt1   = 5;  /* osht var diff wgt1 5  */
    def_dyna_cfg->usht_var_diff_thd[1] = 35; /* usht var diff thd1 35 */
    def_dyna_cfg->usht_var_diff_wgt1   = 10; /* usht var diff wgt1 10 */
    def_dyna_cfg->rmf_gain_scale       = 16; /* rmf gain scale     16 */
    def_dyna_cfg->bmf_gain_scale       = 10; /* bmf gain scale     10 */
}

static hi_void sharpen_dyna_reg_init(isp_sharpen_reg_cfg *sharpen_reg_cfg)
{
    isp_sharpen_default_dyna_reg_cfg *def_cfg = HI_NULL;
    isp_sharpen_mpi_dyna_reg_cfg     *mpi_cfg = HI_NULL;
    isp_sharpen_static_reg_cfg       *sta_cfg = HI_NULL;

    def_cfg = &sharpen_reg_cfg->dyna_reg_cfg.default_dyna_reg_cfg;
    mpi_cfg = &sharpen_reg_cfg->dyna_reg_cfg.mpi_dyna_reg_cfg;
    sta_cfg = &sharpen_reg_cfg->static_reg_cfg;

    sharpen_default_dyna_reg_init(def_cfg);
    sharpen_mpi_dyna_reg_init(mpi_cfg);

    /* calc all mul_coef */
    /* mpi */
    mpi_cfg->osht_var_mul      = calc_mul_coef(sta_cfg->osht_var_thd0,  def_cfg->osht_var_wgt0,
                                               mpi_cfg->osht_var_thd1, sta_cfg->osht_var_wgt1,
                                               SHRP_SHT_VAR_MUL_PRECS);
    mpi_cfg->usht_var_mul      = calc_mul_coef(sta_cfg->usht_var_thd0,  def_cfg->usht_var_wgt0,
                                               mpi_cfg->usht_var_thd1, sta_cfg->usht_var_wgt1,
                                               SHRP_SHT_VAR_MUL_PRECS);
    mpi_cfg->chr_r_mul         = calc_mul_coef(sta_cfg->chr_r_thd[0], mpi_cfg->chr_r_gain,
                                               sta_cfg->chr_r_thd[1], 32, /* chrome red gain max 32 */
                                               SHRP_CHR_MUL_SFT);
    mpi_cfg->chr_g_mul         = calc_mul_coef(sta_cfg->chr_g_thd[0], mpi_cfg->chr_g_gain,
                                               sta_cfg->chr_g_thd[1], 32, /* chrome gre gain max 32 */
                                               SHRP_CHR_MUL_SFT);
    mpi_cfg->chr_gmf_mul       = calc_mul_coef(sta_cfg->chr_g_thd[0], mpi_cfg->chr_gmf_gain,
                                               sta_cfg->chr_g_thd[1], 32, /* chrome gmf gain max 32 */
                                               SHRP_CHR_MUL_SFT);
    mpi_cfg->chr_b_mul         = calc_mul_coef(sta_cfg->chr_b_thd[0], mpi_cfg->chr_b_gain,
                                               sta_cfg->chr_b_thd[1], 32, /* chrome blu gain max 32 */
                                               SHRP_CHR_MUL_SFT);
    mpi_cfg->skin_edge_mul     = calc_mul_coef(sta_cfg->skin_edge_thd[0], mpi_cfg->skin_edge_wgt[0],
                                               sta_cfg->skin_edge_thd[1], mpi_cfg->skin_edge_wgt[1],
                                               SHRP_SKIN_EDGE_MUL_PRECS);
    mpi_cfg->detail_osht_mul   = calc_mul_coef(mpi_cfg->detail_osht_thr[0], mpi_cfg->detail_osht_amt,
                                               mpi_cfg->detail_osht_thr[1], mpi_cfg->osht_amt,
                                               SHRP_DETAIL_SHT_MUL_PRECS);
    mpi_cfg->detail_usht_mul   = calc_mul_coef(mpi_cfg->detail_usht_thr[0], mpi_cfg->detail_usht_amt,
                                               mpi_cfg->detail_usht_thr[1], mpi_cfg->usht_amt,
                                               SHRP_DETAIL_SHT_MUL_PRECS);

    /* defalut */
    def_cfg->hf_eps_mul        = calc_mul_coef(def_cfg->hf_eps_var_thr0, sta_cfg->hf_eps_val0,
                                               def_cfg->hf_eps_var_thr1, def_cfg->hf_eps_val1,
                                               SHRP_HF_EPS_MUL_PRECS);
    def_cfg->osht_var_diff_mul = calc_mul_coef(def_cfg->osht_var_diff_thd[0], sta_cfg->osht_var_diff_wgt0,
                                               def_cfg->osht_var_diff_thd[1], def_cfg->osht_var_diff_wgt1,
                                               SHRP_SHT_VAR_MUL_PRECS);

    def_cfg->usht_var_diff_mul = calc_mul_coef(def_cfg->usht_var_diff_thd[0], sta_cfg->usht_var_diff_wgt0,
                                               def_cfg->usht_var_diff_thd[1], def_cfg->usht_var_diff_wgt1,
                                               SHRP_SHT_VAR_MUL_PRECS);
    def_cfg->range_dir_mul     = calc_mul_coef(def_cfg->range_thd0, def_cfg->range_dir_wgt0,
                                               sta_cfg->range_thd1, sta_cfg->range_dir_wgt1,
                                               SHRP_DIR_RANGE_MUL_PRECS);
}

static hi_void sharpen_regs_initialize(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_u32 i;

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        reg_cfg->alg_reg_cfg[i].sharpen_reg_cfg.enable      = HI_TRUE;
        reg_cfg->alg_reg_cfg[i].sharpen_reg_cfg.lut2_stt_en = HI_TRUE;

        sharpen_static_shoot_reg_init(&(reg_cfg->alg_reg_cfg[i].sharpen_reg_cfg.static_reg_cfg));
        sharpen_static_chrome_reg_init(&(reg_cfg->alg_reg_cfg[i].sharpen_reg_cfg.static_reg_cfg));
        sharpen_static_hsf_reg_init(&(reg_cfg->alg_reg_cfg[i].sharpen_reg_cfg.static_reg_cfg));
        sharpen_static_lpf_reg_init(&(reg_cfg->alg_reg_cfg[i].sharpen_reg_cfg.static_reg_cfg));

        sharpen_dyna_reg_init(&(reg_cfg->alg_reg_cfg[i].sharpen_reg_cfg));
    }

    reg_cfg->cfg_key.bit1_sharpen_cfg = 1;
}

static hi_void sharpen_read_auto_extregs(hi_vi_pipe vi_pipe, hi_isp_sharpen_auto_attr *shp_auto)
{
    hi_u16 i, j, index;

    for (i = 0; i < SHRP_ISO_NUM; i++) {
        for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
            index = i + j * SHRP_ISO_NUM;
            shp_auto->texture_str[j][i] = hi_ext_system_sharpen_texture_str_read(vi_pipe, index);
            shp_auto->edge_str[j][i]    = hi_ext_system_sharpen_edge_str_read(vi_pipe, index);
        }
        for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
            index = i + j * SHRP_ISO_NUM;
            shp_auto->luma_wgt[j][i] = hi_ext_system_sharpen_luma_wgt_read(vi_pipe, index);
        }

        shp_auto->texture_freq[i]  = hi_ext_system_sharpen_texture_freq_read(vi_pipe, i);
        shp_auto->edge_freq[i]     = hi_ext_system_sharpen_edge_freq_read(vi_pipe, i);
        shp_auto->over_shoot[i]    = hi_ext_system_sharpen_over_shoot_read(vi_pipe, i);
        shp_auto->under_shoot[i]   = hi_ext_system_sharpen_under_shoot_read(vi_pipe, i);
        shp_auto->shoot_sup_str[i] = hi_ext_system_sharpen_shoot_sup_str_read(vi_pipe, i);
        shp_auto->detail_ctrl[i]   = hi_ext_system_sharpen_detailctrl_read(vi_pipe, i);
        shp_auto->edge_filt_str[i] = hi_ext_system_sharpen_edge_filt_str_read(vi_pipe, i);
        shp_auto->edge_filt_max_cap[i] = hi_ext_system_sharpen_edge_filt_max_cap_read(vi_pipe, i);
        shp_auto->r_gain[i]         = hi_ext_system_sharpen_r_gain_read(vi_pipe, i);
        shp_auto->g_gain[i]         = hi_ext_system_sharpen_g_gain_read(vi_pipe, i);
        shp_auto->b_gain[i]         = hi_ext_system_sharpen_b_gain_read(vi_pipe, i);
        shp_auto->skin_gain[i]      = hi_ext_system_sharpen_skin_gain_read(vi_pipe, i);
        shp_auto->shoot_sup_adj[i]  = hi_ext_system_sharpen_shoot_sup_adj_read(vi_pipe, i);
        shp_auto->max_sharp_gain[i] = hi_ext_system_sharpen_max_sharp_gain_read(vi_pipe, i);
        shp_auto->detail_ctrl_thr[i] = hi_ext_system_sharpen_detailctrl_thr_read(vi_pipe, i);
    }
}

static hi_void sharpen_read_manual_extregs(hi_vi_pipe vi_pipe, hi_isp_sharpen_manual_attr *shp_manual)
{
    hi_u8 j;

    for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
        shp_manual->texture_str[j] = hi_ext_system_manual_sharpen_texture_str_read(vi_pipe, j);
        shp_manual->edge_str[j]    = hi_ext_system_manual_sharpen_edge_str_read(vi_pipe, j);
    }

    for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
        shp_manual->luma_wgt[j]     = hi_ext_system_manual_sharpen_luma_wgt_read(vi_pipe, j);
    }

    shp_manual->texture_freq       = hi_ext_system_manual_sharpen_texture_freq_read(vi_pipe);
    shp_manual->edge_freq          = hi_ext_system_manual_sharpen_edge_freq_read(vi_pipe);
    shp_manual->over_shoot         = hi_ext_system_manual_sharpen_over_shoot_read(vi_pipe);
    shp_manual->under_shoot        = hi_ext_system_manual_sharpen_under_shoot_read(vi_pipe);
    shp_manual->shoot_sup_str      = hi_ext_system_manual_sharpen_shoot_sup_str_read(vi_pipe);
    shp_manual->detail_ctrl        = hi_ext_system_manual_sharpen_detailctrl_read(vi_pipe);
    shp_manual->edge_filt_str      = hi_ext_system_manual_sharpen_edge_filt_str_read(vi_pipe);
    shp_manual->edge_filt_max_cap  = hi_ext_system_manual_sharpen_edge_filt_max_cap_read(vi_pipe);
    shp_manual->r_gain             = hi_ext_system_manual_sharpen_r_gain_read(vi_pipe);
    shp_manual->g_gain             = hi_ext_system_manual_sharpen_g_gain_read(vi_pipe);
    shp_manual->b_gain             = hi_ext_system_manual_sharpen_b_gain_read(vi_pipe);
    shp_manual->skin_gain          = hi_ext_system_manual_sharpen_skin_gain_read(vi_pipe);
    shp_manual->shoot_sup_adj      = hi_ext_system_manual_sharpen_shoot_sup_adj_read(vi_pipe);
    shp_manual->detail_ctrl_thr    = hi_ext_system_manual_sharpen_detailctrl_thr_read(vi_pipe);
    shp_manual->max_sharp_gain     = hi_ext_system_manual_sharpen_max_sharp_gain_read(vi_pipe);
}

hi_void isp_sharpen_read_extregs(hi_vi_pipe vi_pipe, isp_sharpen_ctx *shp_ctx)
{
    shp_ctx->sharpen_mpi_update_en = hi_ext_system_sharpen_mpi_update_en_read(vi_pipe);
    hi_ext_system_sharpen_mpi_update_en_write(vi_pipe, HI_FALSE);
    shp_ctx->mpi_cfg.skin_umax    = hi_ext_system_manual_sharpen_skin_umax_read(vi_pipe);
    shp_ctx->mpi_cfg.skin_umin    = hi_ext_system_manual_sharpen_skin_umin_read(vi_pipe);
    shp_ctx->mpi_cfg.skin_vmax    = hi_ext_system_manual_sharpen_skin_vmax_read(vi_pipe);
    shp_ctx->mpi_cfg.skin_vmin    = hi_ext_system_manual_sharpen_skin_vmin_read(vi_pipe);

    if (shp_ctx->sharpen_mpi_update_en != HI_TRUE) {
        return;
    }

    shp_ctx->mpi_cfg.op_type = hi_ext_system_sharpen_manu_mode_read(vi_pipe);

    if (shp_ctx->mpi_cfg.op_type == OP_TYPE_MANUAL) {
        sharpen_read_manual_extregs(vi_pipe, &shp_ctx->mpi_cfg.manual_attr);
    } else {
        sharpen_read_auto_extregs(vi_pipe, &shp_ctx->mpi_cfg.auto_attr);
    }
}

hi_void isp_sharpen_read_pro_mode(hi_vi_pipe vi_pipe, isp_sharpen_ctx *shp_ctx)
{
    hi_u8 i, j, index, index_max_value;
    isp_usr_ctx       *isp_ctx  = HI_NULL;
    isp_pro_shp_param *pro_shp_param = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    pro_shp_param = isp_ctx->pro_shp_param_ctrl.pro_shp_param;

    if (pro_shp_param->enable == HI_TRUE) {
        index = isp_ctx->linkage.pro_index;
        index_max_value = MIN2(pro_shp_param->param_num, PRO_MAX_FRAME_NUM);
        if (index > index_max_value) {
            index =  index_max_value;
        }
        if (index < 1) {
            return;
        }
        index -= 1;
    } else {
        return;
    }

    shp_ctx->mpi_cfg.op_type       = OP_TYPE_AUTO;
    shp_ctx->sharpen_mpi_update_en = HI_TRUE;

    for (i = 0; i < SHRP_ISO_NUM; i++) {
        for (j = 0; j < ISP_SHARPEN_GAIN_NUM; j++) {
            shp_ctx->mpi_cfg.auto_attr.texture_str[j][i] = pro_shp_param->shp_attr[index].texture_str[j][i];
            shp_ctx->mpi_cfg.auto_attr.edge_str[j][i]    = pro_shp_param->shp_attr[index].edge_str[j][i];
        }
        for (j = 0; j < ISP_SHARPEN_LUMA_NUM; j++) {
            shp_ctx->mpi_cfg.auto_attr.luma_wgt[j][i]    = pro_shp_param->shp_attr[index].luma_wgt[j][i];
        }
        shp_ctx->mpi_cfg.auto_attr.texture_freq[i]       = pro_shp_param->shp_attr[index].texture_freq[i];
        shp_ctx->mpi_cfg.auto_attr.edge_freq[i]          = pro_shp_param->shp_attr[index].edge_freq[i];
        shp_ctx->mpi_cfg.auto_attr.over_shoot[i]         = pro_shp_param->shp_attr[index].over_shoot[i];
        shp_ctx->mpi_cfg.auto_attr.under_shoot[i]        = pro_shp_param->shp_attr[index].under_shoot[i];
        shp_ctx->mpi_cfg.auto_attr.shoot_sup_str[i]      = pro_shp_param->shp_attr[index].shoot_sup_str[i];
        shp_ctx->mpi_cfg.auto_attr.detail_ctrl[i]        = pro_shp_param->shp_attr[index].detail_ctrl[i];
        shp_ctx->mpi_cfg.auto_attr.edge_filt_str[i]      = pro_shp_param->shp_attr[index].edge_filt_str[i];
        shp_ctx->mpi_cfg.auto_attr.edge_filt_max_cap[i]  = pro_shp_param->shp_attr[index].edge_filt_max_cap[i];
        shp_ctx->mpi_cfg.auto_attr.r_gain[i]             = pro_shp_param->shp_attr[index].r_gain[i];
        shp_ctx->mpi_cfg.auto_attr.g_gain[i]             = pro_shp_param->shp_attr[index].g_gain[i];
        shp_ctx->mpi_cfg.auto_attr.b_gain[i]             = pro_shp_param->shp_attr[index].b_gain[i];
        shp_ctx->mpi_cfg.auto_attr.skin_gain[i]          = pro_shp_param->shp_attr[index].skin_gain[i];
        shp_ctx->mpi_cfg.auto_attr.shoot_sup_adj[i]      = pro_shp_param->shp_attr[index].shoot_sup_adj[i];
        shp_ctx->mpi_cfg.auto_attr.max_sharp_gain[i]     = pro_shp_param->shp_attr[index].max_sharp_gain[i];
        shp_ctx->mpi_cfg.auto_attr.detail_ctrl_thr[i]    = pro_shp_param->shp_attr[index].detail_ctrl_thr[i];
    }
}

hi_s32 isp_sharpen_param_init(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg, isp_sharpen_ctx *shp_ctx)
{
    hi_s32 ret;

    ret = isp_sharpen_ctx_initialize(vi_pipe, shp_ctx);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    sharpen_regs_initialize(vi_pipe, reg_cfg);
    sharpen_ext_regs_initialize(vi_pipe, shp_ctx);

    return HI_SUCCESS;
}

static hi_s32 isp_sharpen_init(hi_vi_pipe vi_pipe, hi_void *reg_cfg)
{
    hi_s32 ret;
    isp_sharpen_ctx *shp_ctx = HI_NULL;

    ret = sharpen_ctx_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    sharpen_get_ctx(vi_pipe, shp_ctx);
    isp_check_pointer_return(shp_ctx);

    return isp_sharpen_param_init(vi_pipe, (isp_reg_cfg *)reg_cfg, shp_ctx);
}

static hi_void isp_sharpen_wdr_mode_set(hi_vi_pipe vi_pipe, hi_void *reg_cfg_info)
{
    hi_u8  i;
    hi_u32 update_idx[ISP_STRIPING_MAX_NUM] = {0};
    hi_s32 ret;
    isp_reg_cfg *reg_cfg = (isp_reg_cfg *)reg_cfg_info;

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        update_idx[i] = reg_cfg->alg_reg_cfg[i].sharpen_reg_cfg.dyna_reg_cfg.mpi_dyna_reg_cfg.update_index;
    }

    ret = isp_sharpen_init(vi_pipe, reg_cfg);
    if (ret != HI_SUCCESS) {
        return;
    }

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        reg_cfg->alg_reg_cfg[i].sharpen_reg_cfg.dyna_reg_cfg.mpi_dyna_reg_cfg.update_index = update_idx[i] + 1;
        reg_cfg->alg_reg_cfg[i].sharpen_reg_cfg.dyna_reg_cfg.mpi_dyna_reg_cfg.switch_mode  = HI_TRUE;
    }
}

static hi_void isp_sharpen_get_linear_default_reg_cfg(isp_sharpen_ctx *sharpen_para)
{
    /* linear mode defalt regs */
    hi_u8  mf_thd_sel_ud[SHRP_ISO_NUM]      = { 2,  2,  2,  2,  2,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1};
    hi_u8  mf_thd_sft_ud[SHRP_ISO_NUM]      = { 0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1};
    hi_u8  hf_eps_var_thr0[SHRP_ISO_NUM]    = {20, 20, 22, 25, 28, 30, 30, 40, 50, 60, 60, 60, 60, 60, 60, 60};
    hi_u8  hf_eps_var_thr1[SHRP_ISO_NUM]    = {30, 30, 32, 35, 38, 40, 40, 50, 60, 70, 70, 70, 70, 70, 70, 70};

    hi_u8  osht_var_wgt0[SHRP_ISO_NUM]      = {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20};
    hi_u8  usht_var_wgt0[SHRP_ISO_NUM]      = {30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30};

    hi_u8  osht_var_diff_thd0[SHRP_ISO_NUM] = {20, 22, 23, 25, 28, 30, 32, 36, 37, 38, 39, 40, 40, 40, 40, 40};
    hi_u8  osht_var_diff_thd1[SHRP_ISO_NUM] = {35, 37, 38, 40, 40, 43, 43, 46, 47, 48, 49, 50, 50, 50, 50, 50};
    hi_u8  osht_var_diff_wgt1[SHRP_ISO_NUM] = { 0,  0,  0,  0,  5, 10, 15, 18, 20, 20, 20, 20, 20, 20, 20, 20};
    hi_u8  usht_var_diff_wgt1[SHRP_ISO_NUM] = {30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 20, 10, 10, 10, 10, 10};

    hi_u8  sft     = sharpen_para->sft;
    hi_u16 wgt_pre = sharpen_para->wgt_pre;
    hi_u16 wgt_cur = sharpen_para->wgt_cur;
    hi_u32 idx_cur = sharpen_para->idx_cur;
    hi_u32 idx_pre = sharpen_para->idx_pre;

    sharpen_para->mf_thd_sel_ud      = shrp_blend(sft, wgt_pre, mf_thd_sel_ud[idx_pre],
                                                  wgt_cur, mf_thd_sel_ud[idx_cur]);
    sharpen_para->mf_thd_sft_ud      = shrp_blend(sft, wgt_pre, mf_thd_sft_ud[idx_pre],
                                                  wgt_cur, mf_thd_sft_ud[idx_cur]);
    sharpen_para->hf_eps_var_thr0    = shrp_blend(sft, wgt_pre, hf_eps_var_thr0[idx_pre],
                                                  wgt_cur, hf_eps_var_thr0[idx_cur]);
    sharpen_para->hf_eps_var_thr1    = shrp_blend(sft, wgt_pre, hf_eps_var_thr1[idx_pre],
                                                  wgt_cur, hf_eps_var_thr1[idx_cur]);
    sharpen_para->osht_var_wgt0      = shrp_blend(sft, wgt_pre, osht_var_wgt0[idx_pre],
                                                  wgt_cur, osht_var_wgt0[idx_cur]);
    sharpen_para->usht_var_wgt0      = shrp_blend(sft, wgt_pre, usht_var_wgt0[idx_pre],
                                                  wgt_cur, usht_var_wgt0[idx_cur]);
    sharpen_para->osht_var_diff_thd0 = shrp_blend(sft, wgt_pre, osht_var_diff_thd0[idx_pre],
                                                  wgt_cur, osht_var_diff_thd0[idx_cur]);
    sharpen_para->osht_var_diff_thd1 = shrp_blend(sft, wgt_pre, osht_var_diff_thd1[idx_pre],
                                                  wgt_cur, osht_var_diff_thd1[idx_cur]);
    sharpen_para->osht_var_diff_wgt1 = shrp_blend(sft, wgt_pre, osht_var_diff_wgt1[idx_pre],
                                                  wgt_cur, osht_var_diff_wgt1[idx_cur]);
    sharpen_para->usht_var_diff_wgt1 = shrp_blend(sft, wgt_pre, usht_var_diff_wgt1[idx_pre],
                                                  wgt_cur, usht_var_diff_wgt1[idx_cur]);
}

static hi_void isp_sharpen_get_wdr_default_reg_cfg(isp_sharpen_ctx *sharpen_para)
{
    /* WDR mode defalt regs */
    hi_u8  mf_thd_sel_ud_wdr[SHRP_ISO_NUM]      = { 2,  2,  2,  2,  2,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1};
    hi_u8  mf_thd_sft_ud_wdr[SHRP_ISO_NUM]      = { 0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1};
    hi_u8  hf_eps_var_thr0_wdr[SHRP_ISO_NUM]    = {20, 20, 22, 25, 28, 30, 30, 40, 50, 60, 60, 60, 60, 60, 60, 60};
    hi_u8  hf_eps_var_thr1_wdr[SHRP_ISO_NUM]    = {30, 30, 32, 35, 38, 40, 40, 50, 60, 70, 70, 70, 70, 70, 70, 70};

    hi_u8  osht_var_wgt0_wdr[SHRP_ISO_NUM]      = {60, 60, 60, 60, 60, 50, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20};
    hi_u8  usht_var_wgt0_wdr[SHRP_ISO_NUM]      = {70, 70, 70, 70, 70, 60, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30};
    hi_u8  osht_var_diff_thd0_wdr[SHRP_ISO_NUM] = {20, 22, 23, 25, 28, 30, 32, 36, 37, 38, 39, 40, 40, 40, 40, 40};
    hi_u8  osht_var_diff_thd1_wdr[SHRP_ISO_NUM] = {35, 37, 38, 40, 40, 43, 43, 46, 47, 48, 49, 50, 50, 50, 50, 50};
    hi_u8  osht_var_diff_wgt1_wdr[SHRP_ISO_NUM] = { 5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5};
    hi_u8  usht_var_diff_wgt1_wdr[SHRP_ISO_NUM] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};

    hi_u8  sft     = sharpen_para->sft;
    hi_u16 wgt_pre = sharpen_para->wgt_pre;
    hi_u16 wgt_cur = sharpen_para->wgt_cur;
    hi_u32 idx_cur = sharpen_para->idx_cur;
    hi_u32 idx_pre = sharpen_para->idx_pre;

    sharpen_para->mf_thd_sel_ud      = shrp_blend(sft, wgt_pre, mf_thd_sel_ud_wdr[idx_pre],
                                                  wgt_cur, mf_thd_sel_ud_wdr[idx_cur]);
    sharpen_para->mf_thd_sft_ud      = shrp_blend(sft, wgt_pre, mf_thd_sft_ud_wdr[idx_pre],
                                                  wgt_cur, mf_thd_sft_ud_wdr[idx_cur]);
    sharpen_para->hf_eps_var_thr0    = shrp_blend(sft, wgt_pre, hf_eps_var_thr0_wdr[idx_pre],
                                                  wgt_cur, hf_eps_var_thr0_wdr[idx_cur]);
    sharpen_para->hf_eps_var_thr1    = shrp_blend(sft, wgt_pre, hf_eps_var_thr1_wdr[idx_pre],
                                                  wgt_cur, hf_eps_var_thr1_wdr[idx_cur]);
    sharpen_para->osht_var_wgt0      = shrp_blend(sft, wgt_pre, osht_var_wgt0_wdr[idx_pre],
                                                  wgt_cur, osht_var_wgt0_wdr[idx_cur]);
    sharpen_para->usht_var_wgt0      = shrp_blend(sft, wgt_pre, usht_var_wgt0_wdr[idx_pre],
                                                  wgt_cur, usht_var_wgt0_wdr[idx_cur]);
    sharpen_para->osht_var_diff_thd0 = shrp_blend(sft, wgt_pre, osht_var_diff_thd0_wdr[idx_pre],
                                                  wgt_cur, osht_var_diff_thd0_wdr[idx_cur]);
    sharpen_para->osht_var_diff_thd1 = shrp_blend(sft, wgt_pre, osht_var_diff_thd1_wdr[idx_pre],
                                                  wgt_cur, osht_var_diff_thd1_wdr[idx_cur]);
    sharpen_para->osht_var_diff_wgt1 = shrp_blend(sft, wgt_pre, osht_var_diff_wgt1_wdr[idx_pre],
                                                  wgt_cur, osht_var_diff_wgt1_wdr[idx_cur]);
    sharpen_para->usht_var_diff_wgt1 = shrp_blend(sft, wgt_pre, usht_var_diff_wgt1_wdr[idx_pre],
                                                  wgt_cur, usht_var_diff_wgt1_wdr[idx_cur]);
}

static hi_void isp_sharpen_get_default_common_reg_cfg(isp_sharpen_ctx *sharpen_para)
{
    /* common regs */
    hi_u8 mf_sft[SHRP_ISO_NUM]   = {  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1};
    hi_u8 dir_sft[SHRP_ISO_NUM]  = { 10,  10,  10,  10,  10,  10,  10,   8,   7,   6,   5,   4,   3,   3,   3,   3};
    hi_u8 rge_thd0[SHRP_ISO_NUM] = { 10,  10,  15,  22,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25,  25};
    hi_u8 rge_wgt0[SHRP_ISO_NUM] = {127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127};
    hi_u8 hf_val1[SHRP_ISO_NUM]  = { 20,  20,  20,  20,  20,  20,  20,  50,  90, 120, 120, 120, 120, 120, 120, 120};
    hi_u8 sel_wgt[SHRP_ISO_NUM]  = { 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31};
    hi_u16 rmf_gsl[SHRP_ISO_NUM] = { 24,  24,  24,  24,  24,  20,  14,  10,   6,   4,   2,   2,   2,   2,   2,   2};
    hi_u16 bmf_gsl[SHRP_ISO_NUM] = { 16,  12,  10,   6,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4};

    hi_u8  sft     = sharpen_para->sft;
    hi_u16 wgt_pre = sharpen_para->wgt_pre;
    hi_u16 wgt_cur = sharpen_para->wgt_cur;
    hi_u32 idx_cur = sharpen_para->idx_cur;
    hi_u32 idx_pre = sharpen_para->idx_pre;

    sharpen_para->mf_thd_sft_d   = shrp_blend(sft, wgt_pre, mf_sft[idx_pre],   wgt_cur, mf_sft[idx_cur]);
    sharpen_para->dir_var_sft    = shrp_blend(sft, wgt_pre, dir_sft[idx_pre],  wgt_cur, dir_sft[idx_cur]);
    sharpen_para->range_thd0     = shrp_blend(sft, wgt_pre, rge_thd0[idx_pre], wgt_cur, rge_thd0[idx_cur]);
    sharpen_para->range_dir_wgt0 = shrp_blend(sft, wgt_pre, rge_wgt0[idx_pre], wgt_cur, rge_wgt0[idx_cur]);
    sharpen_para->hf_eps_val1    = shrp_blend(sft, wgt_pre, hf_val1[idx_pre],  wgt_cur, hf_val1[idx_cur]);
    sharpen_para->sel_pix_wgt    = shrp_blend(sft, wgt_pre, sel_wgt[idx_pre],  wgt_cur, sel_wgt[idx_cur]);
    sharpen_para->rmf_gain_scale = shrp_blend(sft, wgt_pre, rmf_gsl[idx_pre],  wgt_cur, rmf_gsl[idx_cur]);
    sharpen_para->bmf_gain_scale = shrp_blend(sft, wgt_pre, bmf_gsl[idx_pre],  wgt_cur, bmf_gsl[idx_cur]);
}

static hi_void isp_sharpen_get_default_reg_cfg(hi_vi_pipe vi_pipe, isp_sharpen_ctx *shp_ctx)
{
    hi_u8  wdr_mode;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    wdr_mode = isp_ctx->sns_wdr_mode;

    isp_sharpen_get_default_common_reg_cfg(shp_ctx);

    /* linear mode default regs */
    if (is_linear_mode(wdr_mode) || (isp_ctx->linkage.fswdr_mode == ISP_FSWDR_LONG_FRAME_MODE) ||
        (isp_ctx->linkage.fswdr_mode == ISP_FSWDR_AUTO_LONG_FRAME_MODE)) {
        isp_sharpen_get_linear_default_reg_cfg(shp_ctx);
    } else { /* WDR mode default regs */
        isp_sharpen_get_wdr_default_reg_cfg(shp_ctx);
    }
}

static hi_void isp_sharpen_get_mpi_auto_inter_result(isp_sharpen_ctx *shp_ctx)
{
    hi_u8  i;
    hi_u8  sft     = shp_ctx->sft;
    hi_u16 wgt_pre = shp_ctx->wgt_pre;
    hi_u16 wgt_cur = shp_ctx->wgt_cur;
    hi_u32 idx_cur = shp_ctx->idx_cur;
    hi_u32 idx_pre = shp_ctx->idx_pre;
    hi_isp_sharpen_auto_attr   *auto_ctx = &shp_ctx->mpi_cfg.auto_attr;
    hi_isp_sharpen_manual_attr *actual   = &shp_ctx->actual;

    for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
        actual->texture_str[i] = shrp_blend(sft, wgt_pre, auto_ctx->texture_str[i][idx_pre],
                                            wgt_cur, auto_ctx->texture_str[i][idx_cur]);
        actual->edge_str[i]    = shrp_blend(sft, wgt_pre, auto_ctx->edge_str[i][idx_pre],
                                            wgt_cur, auto_ctx->edge_str[i][idx_cur]);
    }

    for (i = 0; i < ISP_SHARPEN_LUMA_NUM; i++) {
        actual->luma_wgt[i]    = shrp_blend(sft, wgt_pre, auto_ctx->luma_wgt[i][idx_pre],
                                            wgt_cur, auto_ctx->luma_wgt[i][idx_cur]);
    }

    actual->texture_freq       = shrp_blend(sft, wgt_pre, auto_ctx->texture_freq[idx_pre],
                                            wgt_cur, auto_ctx->texture_freq[idx_cur]);
    actual->edge_freq          = shrp_blend(sft, wgt_pre, auto_ctx->edge_freq[idx_pre],
                                            wgt_cur, auto_ctx->edge_freq[idx_cur]);
    actual->over_shoot         = shrp_blend(sft, wgt_pre, auto_ctx->over_shoot[idx_pre],
                                            wgt_cur, auto_ctx->over_shoot[idx_cur]);
    actual->under_shoot        = shrp_blend(sft, wgt_pre, auto_ctx->under_shoot[idx_pre],
                                            wgt_cur, auto_ctx->under_shoot[idx_cur]);
    actual->shoot_sup_str      = shrp_blend(sft, wgt_pre, auto_ctx->shoot_sup_str[idx_pre],
                                            wgt_cur, auto_ctx->shoot_sup_str[idx_cur]);
    actual->detail_ctrl        = shrp_blend(sft, wgt_pre, auto_ctx->detail_ctrl[idx_pre],
                                            wgt_cur, auto_ctx->detail_ctrl[idx_cur]);
    actual->edge_filt_str      = shrp_blend(sft, wgt_pre, auto_ctx->edge_filt_str[idx_pre],
                                            wgt_cur, auto_ctx->edge_filt_str[idx_cur]);
    actual->edge_filt_max_cap  = shrp_blend(sft, wgt_pre, auto_ctx->edge_filt_max_cap[idx_pre],
                                            wgt_cur, auto_ctx->edge_filt_max_cap[idx_cur]);
    actual->r_gain             = shrp_blend(sft, wgt_pre, auto_ctx->r_gain[idx_pre],
                                            wgt_cur, auto_ctx->r_gain[idx_cur]);
    actual->g_gain             = shrp_blend(sft, wgt_pre, auto_ctx->g_gain[idx_pre],
                                            wgt_cur, auto_ctx->g_gain[idx_cur]);
    actual->b_gain             = shrp_blend(sft, wgt_pre, auto_ctx->b_gain[idx_pre],
                                            wgt_cur, auto_ctx->b_gain[idx_cur]);
    actual->skin_gain          = shrp_blend(sft, wgt_pre, auto_ctx->skin_gain[idx_pre],
                                            wgt_cur, auto_ctx->skin_gain[idx_cur]);
    actual->shoot_sup_adj      = shrp_blend(sft, wgt_pre, auto_ctx->shoot_sup_adj[idx_pre],
                                            wgt_cur, auto_ctx->shoot_sup_adj[idx_cur]);
    actual->detail_ctrl_thr    = shrp_blend(sft, wgt_pre, auto_ctx->detail_ctrl_thr[idx_pre],
                                            wgt_cur, auto_ctx->detail_ctrl_thr[idx_cur]);
    actual->max_sharp_gain     = shrp_blend(sft, wgt_pre, auto_ctx->max_sharp_gain[idx_pre],
                                            wgt_cur, auto_ctx->max_sharp_gain[idx_cur]);
}

static hi_void sharpen_mpi2reg_def(isp_sharpen_reg_cfg *sharpen_reg_cfg, isp_sharpen_ctx *shp_ctx)
{
    isp_sharpen_default_dyna_reg_cfg *def_cfg      = HI_NULL;
    isp_sharpen_dyna_reg_cfg         *dyna_reg_cfg = HI_NULL;

    dyna_reg_cfg = &sharpen_reg_cfg->dyna_reg_cfg;
    def_cfg      = &dyna_reg_cfg->default_dyna_reg_cfg;

    def_cfg->mf_thd_sft_d         = shp_ctx->mf_thd_sft_d;
    def_cfg->mf_thd_sel_ud        = shp_ctx->mf_thd_sel_ud;
    def_cfg->mf_thd_sft_ud        = shp_ctx->mf_thd_sft_ud;
    def_cfg->hf_thd_sft_d         = def_cfg->mf_thd_sft_d;
    def_cfg->hf_thd_sel_ud        = def_cfg->mf_thd_sel_ud;
    def_cfg->hf_thd_sft_ud        = def_cfg->mf_thd_sft_ud;
    def_cfg->dir_var_sft          = shp_ctx->dir_var_sft;
    def_cfg->range_thd0           = shp_ctx->range_thd0;
    def_cfg->range_dir_wgt0       = shp_ctx->range_dir_wgt0;
    def_cfg->sel_pix_wgt          = shp_ctx->sel_pix_wgt;
    def_cfg->osht_var_diff_thd[0] = shp_ctx->osht_var_diff_thd0;
    def_cfg->usht_var_diff_thd[0] = def_cfg->osht_var_diff_thd[0];
    def_cfg->osht_var_wgt0        = shp_ctx->osht_var_wgt0;
    def_cfg->usht_var_wgt0        = shp_ctx->usht_var_wgt0;
    def_cfg->osht_var_diff_thd[1] = shp_ctx->osht_var_diff_thd1;
    def_cfg->osht_var_diff_wgt1   = shp_ctx->osht_var_diff_wgt1;
    def_cfg->usht_var_diff_thd[1] = def_cfg->osht_var_diff_thd[1];
    def_cfg->usht_var_diff_wgt1   = shp_ctx->usht_var_diff_wgt1;
    def_cfg->rmf_gain_scale       = shp_ctx->rmf_gain_scale;
    def_cfg->bmf_gain_scale       = shp_ctx->bmf_gain_scale;
}

static hi_void sharpen_mpi2reg_mf_gain_d(isp_sharpen_reg_cfg *sharpen_reg_cfg, isp_sharpen_ctx *shp_ctx)
{
    hi_u8  i, j;
    hi_u16 *eg_str = shp_ctx->actual.edge_str;

    isp_sharpen_mpi_dyna_reg_cfg *mpi_cfg      = HI_NULL;
    isp_sharpen_static_reg_cfg   *static_cfg   = HI_NULL;
    isp_sharpen_dyna_reg_cfg     *dyna_reg_cfg = HI_NULL;

    dyna_reg_cfg = &sharpen_reg_cfg->dyna_reg_cfg;
    mpi_cfg      = &dyna_reg_cfg->mpi_dyna_reg_cfg;
    static_cfg   = &sharpen_reg_cfg->static_reg_cfg;

    if (static_cfg->mf_thd_sel_d == 1) {
        j = 0;
        for (i = 0; i < 16; i++) {  /* 1st segment [0,16)  */
            mpi_cfg->mf_gain_d[j++] = MIN2(0xFFF, (0x20 + eg_str[i]));
        }
        for (i = 16; i < 24; i++) { /* 2nd segment [16,24) */
            mpi_cfg->mf_gain_d[j++] = MIN2(0xFFF, (0x20 + eg_str[i]));
            mpi_cfg->mf_gain_d[j++] = MIN2(0xFFF, (0x20 + ((eg_str[i] + eg_str[i + 1]) >> 1)));          /* 1 1 2 */
        }
        for (i = 24; i < 28; i++) { /* 3rd segment [24,28) */
            mpi_cfg->mf_gain_d[j++] = MIN2(0xFFF, (0x20 + eg_str[i]));
            mpi_cfg->mf_gain_d[j++] = MIN2(0xFFF, (0x20 + ((eg_str[i]) * 2 + (eg_str[i + 1])) / 3));     /* 2 1 3 */
            mpi_cfg->mf_gain_d[j++] = MIN2(0xFFF, (0x20 + ((eg_str[i]) + (eg_str[i + 1]) * 2) / 3));     /* 1 2 3 */
        }
        for (i = 28; i < 31; i++) { /* 4th segment [28,31) */
            mpi_cfg->mf_gain_d[j++] = MIN2(0xFFF, (0x20 + eg_str[i]));
            mpi_cfg->mf_gain_d[j++] = MIN2(0xFFF, (0x20 + ((eg_str[i]) * 3 + (eg_str[i + 1]) * 1) / 4)); /* 3 1 4 */
            mpi_cfg->mf_gain_d[j++] = MIN2(0xFFF, (0x20 + ((eg_str[i]) * 2 + (eg_str[i + 1]) * 2) / 4)); /* 2 2 4 */
            mpi_cfg->mf_gain_d[j++] = MIN2(0xFFF, (0x20 + ((eg_str[i]) * 1 + (eg_str[i + 1]) * 3) / 4)); /* 1 3 4 */
        }
        for (i = 31; j < 64; j++) { /* 5th segment [31,64) */
            mpi_cfg->mf_gain_d[j] = MIN2(0xFFF, (0x20 + eg_str[i]));
        }
    } else {
        for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
            mpi_cfg->mf_gain_d[i] = MIN2(0xFFF, (0x20 + eg_str[i]));
        }
        for (i = ISP_SHARPEN_GAIN_NUM; i < SHRP_GAIN_LUT_SIZE; i++) {
            mpi_cfg->mf_gain_d[i] = MIN2(0xFFF, (0x20 + eg_str[ISP_SHARPEN_GAIN_NUM - 1]));
        }
    }
}

static hi_void sharpen_mpi2reg_mf_thd_sel_ud_enable(isp_sharpen_reg_cfg *sharpen_reg_cfg, isp_sharpen_ctx *shp_ctx)
{
    hi_u8  i, j;
    hi_u16 *tx_str = shp_ctx->actual.texture_str;

    isp_sharpen_mpi_dyna_reg_cfg *mpi_cfg      = HI_NULL;
    isp_sharpen_dyna_reg_cfg     *dyna_reg_cfg = HI_NULL;

    dyna_reg_cfg = &sharpen_reg_cfg->dyna_reg_cfg;
    mpi_cfg      = &dyna_reg_cfg->mpi_dyna_reg_cfg;

    j = 0;

    for (i = 0; i < 16; i++) {  /* 1st segment [0,16)  */
        mpi_cfg->mf_gain_ud[j++] = MIN2(0xFFF, (0x20 + tx_str[i]));
    }
    for (i = 16; i < 24; i++) { /* 2nd segment [16,24) */
        mpi_cfg->mf_gain_ud[j++] = MIN2(0xFFF, (0x20 + tx_str[i]));
        mpi_cfg->mf_gain_ud[j++] = MIN2(0xFFF, (0x20 + ((tx_str[i] + tx_str[i + 1]) >> 1)));
    }
    for (i = 24; i < 28; i++) { /* 3rd segment [24,28) */
        mpi_cfg->mf_gain_ud[j++] = MIN2(0xFFF, (0x20 + tx_str[i]));
        mpi_cfg->mf_gain_ud[j++] = MIN2(0xFFF, (0x20 + ((tx_str[i]) * 2 + (tx_str[i + 1])) / 3));     /* (2+1)/3 */
        mpi_cfg->mf_gain_ud[j++] = MIN2(0xFFF, (0x20 + ((tx_str[i]) + (tx_str[i + 1]) * 2) / 3));     /* (1+2)/3 */
    }
    for (i = 28; i < 31; i++) { /* 4th segment [28,31) */
        mpi_cfg->mf_gain_ud[j++] = MIN2(0xFFF, (0x20 + tx_str[i]));
        mpi_cfg->mf_gain_ud[j++] = MIN2(0xFFF, (0x20 + ((tx_str[i]) * 4 + (tx_str[i + 1]) * 1) / 5)); /* (4+1)/5 */
        mpi_cfg->mf_gain_ud[j++] = MIN2(0xFFF, (0x20 + ((tx_str[i]) * 3 + (tx_str[i + 1]) * 2) / 5)); /* (3+2)/5 */
        mpi_cfg->mf_gain_ud[j++] = MIN2(0xFFF, (0x20 + ((tx_str[i]) * 2 + (tx_str[i + 1]) * 3) / 5)); /* (2+3)/5 */
        mpi_cfg->mf_gain_ud[j++] = MIN2(0xFFF, (0x20 + ((tx_str[i]) * 1 + (tx_str[i + 1]) * 4) / 5)); /* (1+4)/5 */
    }
    for (i = 31; j < 64; j++) { /* 5th segment [31,64) */
        mpi_cfg->mf_gain_ud[j]   = MIN2(0xFFF, (0x20 + tx_str[i]));
    }
}

static hi_void sharpen_mpi2reg_mf_thd_sel_ud_disable(isp_sharpen_reg_cfg *sharpen_reg_cfg, isp_sharpen_ctx *shp_ctx)
{
    hi_u8  i, j;
    hi_u16 *tx_str = shp_ctx->actual.texture_str;

    isp_sharpen_mpi_dyna_reg_cfg     *mpi_cfg      = HI_NULL;
    isp_sharpen_dyna_reg_cfg         *dyna_reg_cfg = HI_NULL;

    dyna_reg_cfg = &sharpen_reg_cfg->dyna_reg_cfg;
    mpi_cfg      = &dyna_reg_cfg->mpi_dyna_reg_cfg;

    j = 0;
    for (i = 0; i < 16; i = i + 2) { /* 1st segment [0,16), 2 bins */
        mpi_cfg->mf_gain_ud[j++] = MIN2(0xFFF, (0x40 + tx_str[(i)] + tx_str[(i) + 1]) >> 1);
    }
    for (i = 16; i < 20; i++) { /* 2nd segment [16,20) */
        mpi_cfg->mf_gain_ud[j++] = MIN2(0xFFF, (0x20 + tx_str[i]));
    }
    for (i = 20; i < 24; i++) { /* 3rd segment [20,24) */
        mpi_cfg->mf_gain_ud[j++] = MIN2(0xFFF, (0x20 + tx_str[i]));
        mpi_cfg->mf_gain_ud[j++] = MIN2(0xFFF, (0x20 + ((tx_str[i] + tx_str[i + 1]) >> 1)));
    }
    for (i = 24; i < 26; i++) { /* 4th segment [24,26) */
        mpi_cfg->mf_gain_ud[j++] = MIN2(0xFFF, (0x20 + tx_str[i]));
        mpi_cfg->mf_gain_ud[j++] = MIN2(0xFFF, (0x20 + ((tx_str[i]) * 2 + (tx_str[i + 1])) / 3));     /* (2+1)/3 */
        mpi_cfg->mf_gain_ud[j++] = MIN2(0xFFF, (0x20 + ((tx_str[i]) + (tx_str[i + 1]) * 2) / 3));     /* (1+2)/3 */
    }
    for (i = 26; i < 28; i++) { /* 5th segment [26,28) */
        mpi_cfg->mf_gain_ud[j++] = MIN2(0xFFF, (0x20 + tx_str[i]));
        mpi_cfg->mf_gain_ud[j++] = MIN2(0xFFF, (0x20 + ((tx_str[i]) * 3 + (tx_str[i + 1]) * 1) / 4)); /* (3+1)/4 */
        mpi_cfg->mf_gain_ud[j++] = MIN2(0xFFF, (0x20 + ((tx_str[i]) * 2 + (tx_str[i + 1]) * 2) / 4)); /* (2+2)/4 */
        mpi_cfg->mf_gain_ud[j++] = MIN2(0xFFF, (0x20 + ((tx_str[i]) * 1 + (tx_str[i + 1]) * 3) / 4)); /* (1+3)/4 */
    }
    for (i = 28; i < 31; i++) { /* 6th segment [28,31) */
        mpi_cfg->mf_gain_ud[j++] = MIN2(0xFFF, (0x20 + tx_str[i]));
        mpi_cfg->mf_gain_ud[j++] = MIN2(0xFFF, (0x20 + ((tx_str[i]) * 4 + (tx_str[i + 1]) * 1) / 5)); /* (4+1)/5 */
        mpi_cfg->mf_gain_ud[j++] = MIN2(0xFFF, (0x20 + ((tx_str[i]) * 3 + (tx_str[i + 1]) * 2) / 5)); /* (3+2)/5 */
        mpi_cfg->mf_gain_ud[j++] = MIN2(0xFFF, (0x20 + ((tx_str[i]) * 2 + (tx_str[i + 1]) * 3) / 5)); /* (2+3)/5 */
        mpi_cfg->mf_gain_ud[j++] = MIN2(0xFFF, (0x20 + ((tx_str[i]) * 1 + (tx_str[i + 1]) * 4) / 5)); /* (1+4)/5 */
    }
    for (i = 31; j < 64; j++) { /* 7th segment [31,64) */
        mpi_cfg->mf_gain_ud[j] = MIN2(0xFFF, (0x20 + tx_str[i]));
    }
}

static hi_void sharpen_mpi2reg_mpi_chr(isp_sharpen_reg_cfg *sharpen_reg_cfg, isp_sharpen_ctx *shp_ctx)
{
    hi_u8  i;
    hi_u32 tmp;

    isp_sharpen_mpi_dyna_reg_cfg *mpi_cfg      = HI_NULL;
    isp_sharpen_dyna_reg_cfg     *dyna_reg_cfg = HI_NULL;

    dyna_reg_cfg = &sharpen_reg_cfg->dyna_reg_cfg;
    mpi_cfg      = &dyna_reg_cfg->mpi_dyna_reg_cfg;

    for (i = 0; i < SHRP_GAIN_LUT_SIZE; i++) {
        tmp = (hi_u32)mpi_cfg->mf_gain_ud[i] * shp_ctx->actual.texture_freq;
        mpi_cfg->hf_gain_ud[i] = (hi_u16)MIN2(0xFFF, (tmp >> 0x6));
        tmp = (hi_u32)mpi_cfg->mf_gain_d[i] * shp_ctx->actual.edge_freq;
        mpi_cfg->hf_gain_d[i]  = (hi_u16)MIN2(0xFFF, (tmp >> 0x6));
    }

    mpi_cfg->osht_amt = shp_ctx->actual.over_shoot;
    mpi_cfg->usht_amt = shp_ctx->actual.under_shoot;
    /* skin ctrl */
    if (shp_ctx->actual.skin_gain == 0x1F) {
        mpi_cfg->en_skin_ctrl     = 0;
    } else {
        mpi_cfg->en_skin_ctrl     = 1;
        mpi_cfg->skin_edge_wgt[1] = CLIP3((0x1F - shp_ctx->actual.skin_gain), 0, 0x1F);
        mpi_cfg->skin_edge_wgt[0] = MIN2(0x1F, (mpi_cfg->skin_edge_wgt[1] << 1));
    }
    /* chr ctrl */
    if ((shp_ctx->actual.g_gain == 0x20) && (shp_ctx->actual.r_gain == 0x1F) && (shp_ctx->actual.b_gain == 0x1F)) {
        mpi_cfg->en_chr_ctrl  = 0;
    } else {
        mpi_cfg->en_chr_ctrl  = 1;
        mpi_cfg->chr_r_gain   = shp_ctx->actual.r_gain;
        mpi_cfg->chr_g_gain   = shp_ctx->actual.g_gain;
        mpi_cfg->chr_gmf_gain = shp_ctx->actual.g_gain;
        mpi_cfg->chr_b_gain   = shp_ctx->actual.b_gain;
    }

    if (shp_ctx->actual.detail_ctrl == 0x80) {
        mpi_cfg->en_detail_ctrl = 0;
    } else {
        mpi_cfg->en_detail_ctrl = 1;
    }
}

static hi_void sharpen_mpi2reg_mpi_detail(isp_sharpen_reg_cfg *sharpen_reg_cfg, isp_sharpen_ctx *shp_ctx)
{
    hi_u8  i;

    isp_sharpen_mpi_dyna_reg_cfg *mpi_cfg      = HI_NULL;
    isp_sharpen_dyna_reg_cfg     *dyna_reg_cfg = HI_NULL;
    hi_isp_sharpen_manual_attr   *shp_actual   = &shp_ctx->actual;

    dyna_reg_cfg = &sharpen_reg_cfg->dyna_reg_cfg;
    mpi_cfg      = &dyna_reg_cfg->mpi_dyna_reg_cfg;

    mpi_cfg->detail_osht_amt = CLIP3((mpi_cfg->osht_amt) + (shp_actual->detail_ctrl) - 0x80, 0, 0x7F);
    mpi_cfg->detail_usht_amt = CLIP3((mpi_cfg->usht_amt) + (shp_actual->detail_ctrl) - 0x80, 0, 0x7F);
    mpi_cfg->dir_diff_sft    = 0x3F - shp_actual->edge_filt_str;

    if (shp_actual->edge_filt_max_cap <= 12) { /* max cap thd0 12 */
        mpi_cfg->dir_rt[1] = shp_actual->edge_filt_max_cap;
        mpi_cfg->dir_rt[0] = (shp_actual->edge_filt_max_cap) >> 1;
    } else if (shp_actual->edge_filt_max_cap <= 30) { /* max cap thd1 30 */
        mpi_cfg->dir_rt[1] = shp_actual->edge_filt_max_cap;
        mpi_cfg->dir_rt[0] = 0x6;
    } else {
        mpi_cfg->dir_rt[1] = 0x1E;
        mpi_cfg->dir_rt[0] = shp_actual->edge_filt_max_cap - 0x18;
    }

    if (shp_actual->shoot_sup_str == 0) {
        mpi_cfg->en_sht_ctrl_by_var = 0;
        mpi_cfg->sht_bld_rt         = 0;
        mpi_cfg->osht_var_thd1      = 0;
    } else {
        mpi_cfg->en_sht_ctrl_by_var = 1;
        mpi_cfg->sht_bld_rt         = shp_actual->shoot_sup_adj;
        mpi_cfg->osht_var_thd1      = shp_actual->shoot_sup_str;
    }
    mpi_cfg->usht_var_thd1      = mpi_cfg->osht_var_thd1;
    mpi_cfg->o_max_gain         = shp_actual->max_sharp_gain;
    mpi_cfg->u_max_gain         = shp_actual->max_sharp_gain;
    mpi_cfg->skin_max_u         = shp_ctx->mpi_cfg.skin_umax;
    mpi_cfg->skin_min_u         = shp_ctx->mpi_cfg.skin_umin;
    mpi_cfg->skin_max_v         = shp_ctx->mpi_cfg.skin_vmax;
    mpi_cfg->skin_min_v         = shp_ctx->mpi_cfg.skin_vmin;
    mpi_cfg->detail_osht_thr[0] = shp_actual->detail_ctrl_thr;
    mpi_cfg->detail_osht_thr[1] = MIN2(0xFF, shp_actual->detail_ctrl_thr + SHRP_DETAIL_CTRL_THR_DELTA);
    mpi_cfg->detail_usht_thr[0] = shp_actual->detail_ctrl_thr;
    mpi_cfg->detail_usht_thr[1] = MIN2(0xFF, shp_actual->detail_ctrl_thr + SHRP_DETAIL_CTRL_THR_DELTA);
    mpi_cfg->en_luma_ctrl = 0;
    for (i = 0; i < ISP_SHARPEN_LUMA_NUM; i++) {
        mpi_cfg->luma_wgt[i] = shp_actual->luma_wgt[i];

        if (mpi_cfg->luma_wgt[i] < 0x7F) {
            mpi_cfg->en_luma_ctrl = 1;
        }
    }
}

static hi_void sharpen_mpi2reg_mul_coef(isp_sharpen_reg_cfg *sharpen_reg_cfg)
{
    isp_sharpen_default_dyna_reg_cfg *def_cfg      = HI_NULL;
    isp_sharpen_mpi_dyna_reg_cfg     *mpi_cfg      = HI_NULL;
    isp_sharpen_static_reg_cfg       *static_cfg   = HI_NULL;
    isp_sharpen_dyna_reg_cfg         *dyna_reg_cfg = HI_NULL;

    dyna_reg_cfg = &sharpen_reg_cfg->dyna_reg_cfg;
    def_cfg      = &dyna_reg_cfg->default_dyna_reg_cfg;
    mpi_cfg      = &dyna_reg_cfg->mpi_dyna_reg_cfg;
    static_cfg   = &sharpen_reg_cfg->static_reg_cfg;

    /* calc all mul_coef */
    /* defalut */
    def_cfg->hf_eps_mul        = calc_mul_coef(def_cfg->hf_eps_var_thr0, static_cfg->hf_eps_val0,
                                               def_cfg->hf_eps_var_thr1, def_cfg->hf_eps_val1,
                                               SHRP_HF_EPS_MUL_PRECS);
    def_cfg->osht_var_diff_mul = calc_mul_coef(def_cfg->osht_var_diff_thd[0], static_cfg->osht_var_diff_wgt0,
                                               def_cfg->osht_var_diff_thd[1], def_cfg->osht_var_diff_wgt1,
                                               SHRP_SHT_VAR_MUL_PRECS);
    def_cfg->usht_var_diff_mul = calc_mul_coef(def_cfg->usht_var_diff_thd[0], static_cfg->usht_var_diff_wgt0,
                                               def_cfg->usht_var_diff_thd[1], def_cfg->usht_var_diff_wgt1,
                                               SHRP_SHT_VAR_MUL_PRECS);
    def_cfg->range_dir_mul     = calc_mul_coef(def_cfg->range_thd0, def_cfg->range_dir_wgt0,
                                               static_cfg->range_thd1, static_cfg->range_dir_wgt1,
                                               SHRP_DIR_RANGE_MUL_PRECS);

    /* mpi */
    mpi_cfg->osht_var_mul      = calc_mul_coef(static_cfg->osht_var_thd0, def_cfg->osht_var_wgt0,
                                               mpi_cfg->osht_var_thd1, static_cfg->osht_var_wgt1,
                                               SHRP_SHT_VAR_MUL_PRECS);
    mpi_cfg->usht_var_mul      = calc_mul_coef(static_cfg->usht_var_thd0, def_cfg->usht_var_wgt0,
                                               mpi_cfg->usht_var_thd1, static_cfg->usht_var_wgt1,
                                               SHRP_SHT_VAR_MUL_PRECS);
    mpi_cfg->detail_osht_mul   = calc_mul_coef(mpi_cfg->detail_osht_thr[0], mpi_cfg->detail_osht_amt,
                                               mpi_cfg->detail_osht_thr[1], mpi_cfg->osht_amt,
                                               SHRP_DETAIL_SHT_MUL_PRECS);
    mpi_cfg->detail_usht_mul   = calc_mul_coef(mpi_cfg->detail_usht_thr[0], mpi_cfg->detail_usht_amt,
                                               mpi_cfg->detail_usht_thr[1], mpi_cfg->usht_amt,
                                               SHRP_DETAIL_SHT_MUL_PRECS);
    mpi_cfg->chr_r_mul         = calc_mul_coef(static_cfg->chr_r_thd[0], mpi_cfg->chr_r_gain,
                                               static_cfg->chr_r_thd[1], 0x20,
                                               SHRP_CHR_MUL_SFT);
    mpi_cfg->chr_g_mul         = calc_mul_coef(static_cfg->chr_g_thd[0], mpi_cfg->chr_g_gain,
                                               static_cfg->chr_g_thd[1], 0x20,
                                               SHRP_CHR_MUL_SFT);
    mpi_cfg->chr_gmf_mul       = calc_mul_coef(static_cfg->chr_g_thd[0], mpi_cfg->chr_gmf_gain,
                                               static_cfg->chr_g_thd[1], 0x20,
                                               SHRP_CHR_MUL_SFT);
    mpi_cfg->chr_b_mul         = calc_mul_coef(static_cfg->chr_b_thd[0], mpi_cfg->chr_b_gain,
                                               static_cfg->chr_b_thd[1], 0x20,
                                               SHRP_CHR_MUL_SFT);
    mpi_cfg->skin_edge_mul     = calc_mul_coef(static_cfg->skin_edge_thd[0], mpi_cfg->skin_edge_wgt[0],
                                               static_cfg->skin_edge_thd[1], mpi_cfg->skin_edge_wgt[1],
                                               SHRP_SKIN_EDGE_MUL_PRECS);
}

static hi_void sharpen_mpi2reg(isp_sharpen_reg_cfg *sharpen_reg_cfg, isp_sharpen_ctx *shp_ctx)
{
    isp_sharpen_default_dyna_reg_cfg *def_cfg      = HI_NULL;
    isp_sharpen_mpi_dyna_reg_cfg     *mpi_cfg      = HI_NULL;
    isp_sharpen_dyna_reg_cfg         *dyna_reg_cfg = HI_NULL;

    dyna_reg_cfg = &sharpen_reg_cfg->dyna_reg_cfg;
    mpi_cfg      = &dyna_reg_cfg->mpi_dyna_reg_cfg;
    def_cfg      = &dyna_reg_cfg->default_dyna_reg_cfg;

    if (def_cfg->resh) {
        sharpen_mpi2reg_def(sharpen_reg_cfg, shp_ctx);
    }

    if (mpi_cfg->resh) {
        sharpen_mpi2reg_mf_gain_d(sharpen_reg_cfg, shp_ctx);
        if (def_cfg->mf_thd_sel_ud == 1) {
            sharpen_mpi2reg_mf_thd_sel_ud_enable(sharpen_reg_cfg, shp_ctx);
        } else {
            sharpen_mpi2reg_mf_thd_sel_ud_disable(sharpen_reg_cfg, shp_ctx);
        }
        sharpen_mpi2reg_mpi_chr(sharpen_reg_cfg, shp_ctx);
        sharpen_mpi2reg_mpi_detail(sharpen_reg_cfg, shp_ctx);
    }

    sharpen_mpi2reg_mul_coef(sharpen_reg_cfg);
}

static hi_bool __inline check_sharpen_open(isp_sharpen_ctx *shp_ctx)
{
    return (shp_ctx->mpi_cfg.enable == HI_TRUE);
}

static hi_void isp_sharpen_actual_update(hi_vi_pipe vi_pipe, hi_isp_sharpen_manual_attr *actual)
{
    hi_u8 i;

    hi_ext_system_actual_sharpen_overshoot_amt_write(vi_pipe, actual->over_shoot);
    hi_ext_system_actual_sharpen_undershoot_amt_write(vi_pipe, actual->under_shoot);
    hi_ext_system_actual_sharpen_shoot_sup_write(vi_pipe, actual->shoot_sup_str);
    hi_ext_system_actual_sharpen_edge_frequence_write(vi_pipe, actual->edge_freq);
    hi_ext_system_actual_sharpen_texture_frequence_write(vi_pipe, actual->texture_freq);

    for (i = 0; i < ISP_SHARPEN_GAIN_NUM; i++) {
        hi_ext_system_actual_sharpen_edge_str_write(vi_pipe, i, actual->edge_str[i]);
        hi_ext_system_actual_sharpen_texture_str_write(vi_pipe, i, actual->texture_str[i]);
    }
}

hi_s32 sharpen_proc_write(hi_vi_pipe vi_pipe, hi_isp_ctrl_proc_write *proc, isp_sharpen_ctx *shp_ctx)
{
    hi_u8 i, index;
    hi_isp_ctrl_proc_write proc_tmp;
    hi_isp_sharpen_manual_attr *shp_actual = HI_NULL;

    shp_actual = &shp_ctx->actual;

    if ((proc->proc_buff == HI_NULL) || (proc->buff_len == 0)) {
        return HI_FAILURE;
    }

    proc_tmp.proc_buff = proc->proc_buff;
    proc_tmp.buff_len = proc->buff_len;

    isp_proc_printf(&proc_tmp, proc->write_len, "-----SHARPEN INFO-------------------------------------------------\n");
    isp_proc_printf(&proc_tmp, proc->write_len, "%16s\n", "bSharpenEn");
    isp_proc_printf(&proc_tmp, proc->write_len, "%16u\n", (hi_u16)shp_ctx->mpi_cfg.enable);

    for (i = 0; i < 0x4; i++) {
        index = i * 0x8;
        isp_proc_printf(&proc_tmp, proc->write_len, "%s"  "%d"  "--"  "%d:\n", "LumaWgt ", index, index + 0x7);
        isp_proc_printf(&proc_tmp, proc->write_len, "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u\n\n",
                        (hi_u16)shp_actual->luma_wgt[index + 0x0], (hi_u16)shp_actual->luma_wgt[index + 0x1],
                        (hi_u16)shp_actual->luma_wgt[index + 0x2], (hi_u16)shp_actual->luma_wgt[index + 0x3],
                        (hi_u16)shp_actual->luma_wgt[index + 0x4], (hi_u16)shp_actual->luma_wgt[index + 0x5],
                        (hi_u16)shp_actual->luma_wgt[index + 0x6], (hi_u16)shp_actual->luma_wgt[index + 0x7]);
    }

    for (i = 0; i < 0x4; i++) {
        index = i * 0x8;
        isp_proc_printf(&proc_tmp, proc->write_len, "%s"  "%d"  "--"  "%d:\n", "TextureStr ", index, index + 0x7);
        isp_proc_printf(&proc_tmp, proc->write_len, "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u\n\n",
                        shp_actual->texture_str[index + 0x0], shp_actual->texture_str[index + 0x1],
                        shp_actual->texture_str[index + 0x2], shp_actual->texture_str[index + 0x3],
                        shp_actual->texture_str[index + 0x4], shp_actual->texture_str[index + 0x5],
                        shp_actual->texture_str[index + 0x6], shp_actual->texture_str[index + 0x7]);
    }

    for (i = 0; i < 0x4; i++) {
        index = i * 0x8;
        isp_proc_printf(&proc_tmp, proc->write_len, "%s"  "%d"  "--"  "%d:\n", "EdgeStr ", index, index + 0x7);
        isp_proc_printf(&proc_tmp, proc->write_len, "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u"  "%8u\n\n",
                        shp_actual->edge_str[index + 0x0], shp_actual->edge_str[index + 0x1],
                        shp_actual->edge_str[index + 0x2], shp_actual->edge_str[index + 0x3],
                        shp_actual->edge_str[index + 0x4], shp_actual->edge_str[index + 0x5],
                        shp_actual->edge_str[index + 0x6], shp_actual->edge_str[index + 0x7]);
    }

    isp_proc_printf(&proc_tmp, proc->write_len, "%12s" "%12s" "%12s" "%12s" "%12s" "%12s \n",
                    "TextureFreq", "EdgeFreq", "OverShoot", "UnderShoot", "ShootSupStr", "DetailCtrl");

    isp_proc_printf(&proc_tmp, proc->write_len, "%12u"  "%12u"  "%12u"  "%12u"  "%12u"  "%12u\n\n",
                    shp_actual->texture_freq, shp_actual->edge_freq, shp_actual->over_shoot,
                    shp_actual->under_shoot, shp_actual->shoot_sup_str, shp_actual->detail_ctrl);

    isp_proc_printf(&proc_tmp, proc->write_len, "%12s" "%16s" "%12s" "%12s" "%12s" "%12s\n",
                    "EdgeFiltStr", "EdgeFiltMaxCap", "RGain", "GGain", "BGain", "SkinGain");

    isp_proc_printf(&proc_tmp, proc->write_len, "%12u"  "%16u"  "%12u" "%12u"  "%12u" "%12u\n\n",
                    shp_actual->edge_filt_str, shp_actual->edge_filt_max_cap, shp_actual->r_gain,
                    shp_actual->g_gain, shp_actual->b_gain, shp_actual->skin_gain);

    isp_proc_printf(&proc_tmp, proc->write_len, "%12s" "%14s" "%14s" "%12s" "%12s" "%12s"  "%12s\n",
                    "ShootSupAdj", "DetailCtrlThr", "MaxSharpGain", "SkinUmax", "SkinUmin", "SkinVmax", "SkinVmin");

    isp_proc_printf(&proc_tmp, proc->write_len, "%12u"  "%14u"  "%14u"  "%12u"  "%12u"  "%12u"   "%12u\n\n",
                    shp_actual->shoot_sup_adj, shp_actual->detail_ctrl_thr, shp_actual->max_sharp_gain,
                    shp_ctx->mpi_cfg.skin_umax, shp_ctx->mpi_cfg.skin_umin, shp_ctx->mpi_cfg.skin_vmax,
                    shp_ctx->mpi_cfg.skin_vmin);

    proc->write_len += 1;

    return HI_SUCCESS;
}

static hi_void isp_sharpen_actual_calc(isp_sharpen_ctx *shp_ctx)
{
    if (shp_ctx->mpi_cfg.op_type == OP_TYPE_MANUAL) {
        memcpy_s(&shp_ctx->actual, sizeof(hi_isp_sharpen_manual_attr),
                 &shp_ctx->mpi_cfg.manual_attr, sizeof(hi_isp_sharpen_manual_attr));
    } else {
        isp_sharpen_get_mpi_auto_inter_result(shp_ctx);
    }
}

static hi_void sharpen_iso_update(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg, isp_sharpen_ctx *shp_ctx)
{
    hi_u8  i;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if ((shp_ctx->iso != shp_ctx->iso_last) ||
        (isp_ctx->linkage.fswdr_mode != isp_ctx->linkage.pre_fswdr_mode)) {   /* will not work if ISO is the same */
        isp_sharpen_get_default_reg_cfg(vi_pipe, shp_ctx);
        for (i = 0; i < reg_cfg->cfg_num; i++) {
            reg_cfg->alg_reg_cfg[i].sharpen_reg_cfg.dyna_reg_cfg.default_dyna_reg_cfg.resh = HI_TRUE;
        }
    }

    if (shp_ctx->sharpen_mpi_update_en) {
        isp_sharpen_actual_calc(shp_ctx);

        for (i = 0; i < reg_cfg->cfg_num; i++) {
            reg_cfg->alg_reg_cfg[i].sharpen_reg_cfg.dyna_reg_cfg.mpi_dyna_reg_cfg.resh           = HI_TRUE;
            reg_cfg->alg_reg_cfg[i].sharpen_reg_cfg.dyna_reg_cfg.mpi_dyna_reg_cfg.update_index  += 1;
            sharpen_mpi2reg(&reg_cfg->alg_reg_cfg[i].sharpen_reg_cfg, shp_ctx);
        }
    } else if (shp_ctx->iso != shp_ctx->iso_last) {
        isp_sharpen_actual_calc(shp_ctx);
        if (shp_ctx->mpi_cfg.op_type == OP_TYPE_AUTO) {  /* auto mode */
            for (i = 0; i < reg_cfg->cfg_num; i++) {
                reg_cfg->alg_reg_cfg[i].sharpen_reg_cfg.dyna_reg_cfg.mpi_dyna_reg_cfg.resh          = HI_TRUE;
                reg_cfg->alg_reg_cfg[i].sharpen_reg_cfg.dyna_reg_cfg.mpi_dyna_reg_cfg.update_index += 1;
                sharpen_mpi2reg(&reg_cfg->alg_reg_cfg[i].sharpen_reg_cfg, shp_ctx);
            }
        } else {
            for (i = 0; i < reg_cfg->cfg_num; i++) {
                sharpen_mpi2reg(&reg_cfg->alg_reg_cfg[i].sharpen_reg_cfg, shp_ctx);
            }
        }
    }

    shp_ctx->iso_last = shp_ctx->iso;    /* will not work if ISO is the same */
}

static hi_void sharpen_inter_wgt_calc(isp_usr_ctx *isp_ctx, isp_sharpen_ctx *shp_ctx)
{
    hi_u8  sft = 0x8;
    hi_s32 iso_lvl_cur, iso_lvl_pre;

    /* sharpen strength linkage with the iso calculated by ae */
    shp_ctx->sft = sft;
    shp_ctx->iso = isp_ctx->linkage.iso;
    shp_ctx->idx_cur = get_iso_index(shp_ctx->iso);
    shp_ctx->idx_pre = (shp_ctx->idx_cur == 0) ? 0 : MAX2(shp_ctx->idx_cur - 1, 0);

    iso_lvl_cur = get_iso(shp_ctx->idx_cur);
    iso_lvl_pre = get_iso(shp_ctx->idx_pre);
    if (shp_ctx->iso <= iso_lvl_pre) {
        shp_ctx->wgt_pre = signed_left_shift(1, sft);
    } else if (shp_ctx->iso >= iso_lvl_cur) {
        shp_ctx->wgt_pre = 0;
    } else {
        shp_ctx->wgt_pre = signed_left_shift((iso_lvl_cur - shp_ctx->iso), sft) / (iso_lvl_cur - iso_lvl_pre);
    }
    shp_ctx->wgt_cur = signed_left_shift(1, sft) - shp_ctx->wgt_pre;
}

hi_void isp_sharpen_reg_update(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg, isp_sharpen_ctx *shp_ctx)
{
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    sharpen_inter_wgt_calc(isp_ctx, shp_ctx);
    sharpen_iso_update(vi_pipe, reg_cfg, shp_ctx);
}

static hi_s32 isp_sharpen_run(hi_vi_pipe vi_pipe, const hi_void *stat_info, hi_void *reg_cfg_info, hi_s32 rsv)
{
    hi_u8   i;
    isp_usr_ctx     *isp_ctx = HI_NULL;
    isp_sharpen_ctx *shp_ctx = HI_NULL;
    isp_reg_cfg     *reg_cfg = (isp_reg_cfg *)reg_cfg_info;

    isp_get_ctx(vi_pipe, isp_ctx);
    sharpen_get_ctx(vi_pipe, shp_ctx);
    isp_check_pointer_return(shp_ctx);

    if (isp_ctx->linkage.defect_pixel) {
        return HI_SUCCESS;
    }

    shp_ctx->mpi_cfg.enable = hi_ext_system_manual_sharpen_en_read(vi_pipe);

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        reg_cfg->alg_reg_cfg[i].sharpen_reg_cfg.enable = shp_ctx->mpi_cfg.enable;
        reg_cfg->alg_reg_cfg[i].sharpen_reg_cfg.dyna_reg_cfg.mpi_dyna_reg_cfg.switch_mode = HI_FALSE;
    }

    reg_cfg->cfg_key.bit1_sharpen_cfg = 1;

    /* check hardware setting */
    if (!check_sharpen_open(shp_ctx)) {
        return HI_SUCCESS;
    }

    isp_sharpen_read_extregs(vi_pipe, shp_ctx);
    isp_sharpen_read_pro_mode(vi_pipe, shp_ctx);

    isp_sharpen_reg_update(vi_pipe, reg_cfg, shp_ctx);
    isp_sharpen_actual_update(vi_pipe, &shp_ctx->actual);

    return HI_SUCCESS;
}

static hi_s32 isp_sharpen_ctrl(hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value)
{
    isp_reg_cfg_attr *reg_cfg_attr = HI_NULL;
    isp_sharpen_ctx *shp_ctx = HI_NULL;

    switch (cmd) {
        case ISP_WDR_MODE_SET:
            isp_regcfg_get_ctx(vi_pipe, reg_cfg_attr);
            isp_check_pointer_return(reg_cfg_attr);
            isp_sharpen_wdr_mode_set(vi_pipe, (hi_void *)&reg_cfg_attr->reg_cfg);
            break;
        case ISP_PROC_WRITE:
            sharpen_get_ctx(vi_pipe, shp_ctx);
            isp_check_pointer_return(shp_ctx);
            sharpen_proc_write(vi_pipe, (hi_isp_ctrl_proc_write *)value, shp_ctx);
            break;
        default:
            break;
    }

    return HI_SUCCESS;
}

static hi_s32 isp_sharpen_exit(hi_vi_pipe vi_pipe)
{
    hi_u8 i;
    isp_reg_cfg_attr *reg_cfg   = HI_NULL;
    isp_regcfg_get_ctx(vi_pipe, reg_cfg);

    for (i = 0; i < reg_cfg->reg_cfg.cfg_num; i++) {
        reg_cfg->reg_cfg.alg_reg_cfg[i].sharpen_reg_cfg.enable = HI_FALSE;
    }

    reg_cfg->reg_cfg.cfg_key.bit1_sharpen_cfg = 1;

    hi_ext_system_sharpen_manu_mode_write(vi_pipe, HI_FALSE);

    sharpen_ctx_exit(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_alg_register_sharpen(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_alg_node *algs = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_alg_check_return(isp_ctx->alg_key.bit1_sharpen);
    algs = isp_search_alg(isp_ctx->algs);
    isp_check_pointer_return(algs);

    algs->alg_type = ISP_ALG_SHARPEN;
    algs->alg_func.pfn_alg_init = isp_sharpen_init;
    algs->alg_func.pfn_alg_run  = isp_sharpen_run;
    algs->alg_func.pfn_alg_ctrl = isp_sharpen_ctrl;
    algs->alg_func.pfn_alg_exit = isp_sharpen_exit;
    algs->used = HI_TRUE;

    return HI_SUCCESS;
}
