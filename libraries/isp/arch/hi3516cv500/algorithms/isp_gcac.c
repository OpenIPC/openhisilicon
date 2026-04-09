/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_gcac.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_alg.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_math_utils.h"

typedef struct {
    hi_bool global_cac_en;
    hi_bool coef_update_en;
    hi_s16  param_red_a; /* [-256,255] */
    hi_s16  param_red_b; /* [-256,255] */
    hi_s16  param_red_c; /* [-256,255] */
    hi_s16  param_blue_a; /* [-256,255] */
    hi_s16  param_blue_b; /* [-256,255] */
    hi_s16  param_blue_c; /* [-256,255] */

    hi_u8   ver_norm_shift; /* [0,7] */
    hi_u8   ver_norm_factor; /* [0,31] */
    hi_u8   hor_norm_shift; /* [0,7] */
    hi_u8   hor_norm_factor; /* [0,31] */
    hi_u16  center_coor_hor;
    hi_u16  center_coor_ver;
    hi_u16  cor_var_thr; /* [0, 4095]  edge */
} isp_gcac;

isp_gcac *g_gcac_ctx[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define gcac_get_ctx(dev, ctx)   ((ctx) = g_gcac_ctx[dev])
#define gcac_set_ctx(dev, ctx)   (g_gcac_ctx[dev] = (ctx))
#define gcac_reset_ctx(dev)      (g_gcac_ctx[dev] = HI_NULL)

hi_s32 gcac_ctx_init(hi_vi_pipe vi_pipe)
{
    isp_gcac *gcac_ctx = HI_NULL;

    gcac_get_ctx(vi_pipe, gcac_ctx);

    if (gcac_ctx == HI_NULL) {
        gcac_ctx = (isp_gcac *)isp_malloc(sizeof(isp_gcac));
        if (gcac_ctx == HI_NULL) {
            isp_err_trace("Isp[%d] gcac_ctx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset_s(gcac_ctx, sizeof(isp_gcac), 0, sizeof(isp_gcac));

    gcac_set_ctx(vi_pipe, gcac_ctx);

    return HI_SUCCESS;
}

hi_void gcac_ctx_exit(hi_vi_pipe vi_pipe)
{
    isp_gcac *gcac_ctx = HI_NULL;

    gcac_get_ctx(vi_pipe, gcac_ctx);
    isp_free(gcac_ctx);
    gcac_reset_ctx(vi_pipe);
}

static hi_void gcac_ext_regs_initialize(hi_vi_pipe vi_pipe)
{
    hi_u16 frame_width, frame_height;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    frame_width  = isp_ctx->block_attr.frame_rect.width;
    frame_height = isp_ctx->block_attr.frame_rect.height;

    /* global CAC */
    hi_ext_system_global_cac_enable_write(vi_pipe, HI_FALSE);
    hi_ext_system_global_cac_coef_update_en_write(vi_pipe, HI_TRUE);
    hi_ext_system_global_cac_coor_center_hor_write(vi_pipe, (frame_width - 1) >> 1);
    hi_ext_system_global_cac_coor_center_ver_write(vi_pipe, (frame_height - 1) >> 1);
    hi_ext_system_global_cac_param_red_a_write(vi_pipe, HI_ISP_GCAC_PARAM_RED_A_DEFAULT);
    hi_ext_system_global_cac_param_red_b_write(vi_pipe, HI_ISP_GCAC_PARAM_RED_B_DEFAULT);
    hi_ext_system_global_cac_param_red_c_write(vi_pipe, HI_ISP_GCAC_PARAM_RED_C_DEFAULT);
    hi_ext_system_global_cac_param_blue_a_write(vi_pipe, HI_ISP_GCAC_PARAM_BLUE_A_DEFAULT);
    hi_ext_system_global_cac_param_blue_b_write(vi_pipe, HI_ISP_GCAC_PARAM_BLUE_B_DEFAULT);
    hi_ext_system_global_cac_param_blue_c_write(vi_pipe, HI_ISP_GCAC_PARAM_BLUE_C_DEFAULT);
    hi_ext_system_global_cac_y_ns_norm_write(vi_pipe, HI_ISP_GCAC_Y_NORM_SHIFT_DEFAULT);
    hi_ext_system_global_cac_y_nf_norm_write(vi_pipe, HI_ISP_GCAC_Y_NORM_FACTOR_DEFAULT);
    hi_ext_system_global_cac_x_ns_norm_write(vi_pipe, HI_ISP_GCAC_X_NORM_SHIFT_DEFAULT);
    hi_ext_system_global_cac_x_nf_norm_write(vi_pipe, HI_ISP_GCAC_X_NORM_FACTOR_DEFAULT);
    hi_ext_system_global_cac_cor_thr_write(vi_pipe, HI_ISP_DEMOSAIC_GCAC_BLEND_THR_DEFAULT);
}

static hi_void gcac_static_regs_initialize(hi_vi_pipe vi_pipe, isp_global_cac_static_cfg *static_reg_cfg,
                                           isp_usr_ctx *isp_ctx)
{
    static_reg_cfg->gcac_ver_fil_en     = HI_ISP_GCAC_VER_FILT_EN_DEFAULT;
    static_reg_cfg->gcac_clip_mode_hor  = HI_ISP_GCAC_CLIP_MODE_HOR_DEFAULT;
    static_reg_cfg->gcac_clip_mode_ver  = HI_ISP_GCAC_CLIP_MODE_VER_DEFAULT;
    static_reg_cfg->gcac_blend_sel      = HI_ISP_GCAC_BLEND_SELECTION_DEFAULT;
    static_reg_cfg->gcac_chr_ver_mode   = HI_ISP_GCAC_CHR_VER_MODE_DEFAULT;
    static_reg_cfg->static_resh         = HI_TRUE;
}

static hi_void gcac_image_size(hi_u8 i, isp_global_cac_usr_cfg *usr_reg_cfg, isp_block_attr *block_attr)
{
    isp_rect block_rect;

    isp_get_block_rect(&block_rect, block_attr, i);
    usr_reg_cfg->start_coor_hor = block_rect.x;
    usr_reg_cfg->start_coor_ver = block_rect.y;

    return;
}

static hi_void gcac_usr_regs_initialize(hi_u8 i, isp_global_cac_usr_cfg *usr_reg_cfg, isp_usr_ctx *isp_ctx)
{
    hi_u16 frame_width, frame_height;

    frame_width  = isp_ctx->block_attr.frame_rect.width;
    frame_height = isp_ctx->block_attr.frame_rect.height;

    usr_reg_cfg->update_index    = 1;
    usr_reg_cfg->resh            = HI_TRUE;
    usr_reg_cfg->param_red_a     = HI_ISP_GCAC_PARAM_RED_A_DEFAULT;
    usr_reg_cfg->param_red_b     = HI_ISP_GCAC_PARAM_RED_B_DEFAULT;
    usr_reg_cfg->param_red_c     = HI_ISP_GCAC_PARAM_RED_C_DEFAULT;

    usr_reg_cfg->param_blue_a    = HI_ISP_GCAC_PARAM_BLUE_A_DEFAULT;
    usr_reg_cfg->param_blue_b    = HI_ISP_GCAC_PARAM_BLUE_B_DEFAULT;
    usr_reg_cfg->param_blue_c    = HI_ISP_GCAC_PARAM_BLUE_C_DEFAULT;

    usr_reg_cfg->ver_norm_shift   = HI_ISP_GCAC_Y_NORM_SHIFT_DEFAULT;
    usr_reg_cfg->ver_norm_factor  = HI_ISP_GCAC_Y_NORM_FACTOR_DEFAULT;
    usr_reg_cfg->hor_norm_shift   = HI_ISP_GCAC_X_NORM_SHIFT_DEFAULT;
    usr_reg_cfg->hor_norm_factor  = HI_ISP_GCAC_X_NORM_FACTOR_DEFAULT;
    usr_reg_cfg->cor_var_thr      = HI_ISP_DEMOSAIC_GCAC_BLEND_THR_DEFAULT;
    usr_reg_cfg->center_coor_ver  = (frame_height - 1) >> 1;
    usr_reg_cfg->center_coor_hor  = (frame_width  - 1) >> 1;

    gcac_image_size(i, usr_reg_cfg, &isp_ctx->block_attr);

    return;
}

static hi_void gcac_regs_initialize(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_u8 i;
    isp_usr_ctx  *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    for (i = 0; i < isp_ctx->block_attr.block_num; i++) {
        gcac_static_regs_initialize(vi_pipe, &reg_cfg->alg_reg_cfg[i].gcac_reg_cfg.static_reg_cfg, isp_ctx);

        gcac_usr_regs_initialize(i, &reg_cfg->alg_reg_cfg[i].gcac_reg_cfg.usr_reg_cfg, isp_ctx);

        reg_cfg->alg_reg_cfg[i].gcac_reg_cfg.global_cac_en = HI_FALSE;
    }

    reg_cfg->cfg_key.bit1_global_cac_cfg = 1;
}

static hi_void gcac_initialize(hi_vi_pipe vi_pipe)
{
    isp_gcac *global_cac = HI_NULL;

    gcac_get_ctx(vi_pipe, global_cac);

    global_cac->global_cac_en    = HI_FALSE;
}

static hi_s32 gcac_read_extregs(hi_vi_pipe vi_pipe)
{
    isp_gcac  *global_cac = HI_NULL;

    gcac_get_ctx(vi_pipe, global_cac);
    isp_check_pointer_return(global_cac);

    global_cac->coef_update_en = hi_ext_system_global_cac_coef_update_en_read(vi_pipe);
    hi_ext_system_global_cac_coef_update_en_write(vi_pipe, HI_FALSE);

    if (global_cac->coef_update_en) {
        global_cac->param_red_a      = hi_ext_system_global_cac_param_red_a_read(vi_pipe);
        global_cac->param_red_b      = hi_ext_system_global_cac_param_red_b_read(vi_pipe);
        global_cac->param_red_c      = hi_ext_system_global_cac_param_red_c_read(vi_pipe);
        global_cac->param_blue_a     = hi_ext_system_global_cac_param_blue_a_read(vi_pipe);
        global_cac->param_blue_b     = hi_ext_system_global_cac_param_blue_b_read(vi_pipe);
        global_cac->param_blue_c     = hi_ext_system_global_cac_param_blue_c_read(vi_pipe);
        global_cac->ver_norm_shift   = hi_ext_system_global_cac_y_ns_norm_read(vi_pipe);
        global_cac->ver_norm_factor  = hi_ext_system_global_cac_y_nf_norm_read(vi_pipe);
        global_cac->hor_norm_shift   = hi_ext_system_global_cac_x_ns_norm_read(vi_pipe);
        global_cac->hor_norm_factor  = hi_ext_system_global_cac_x_nf_norm_read(vi_pipe);
        global_cac->center_coor_hor  = hi_ext_system_global_cac_coor_center_hor_read(vi_pipe);
        global_cac->center_coor_ver  = hi_ext_system_global_cac_coor_center_ver_read(vi_pipe);
        global_cac->cor_var_thr      = hi_ext_system_global_cac_cor_thr_read(vi_pipe);
    }

    return HI_SUCCESS;
}

hi_s32 global_cac_usr_fw(hi_u8 cur_blk, isp_gcac *global_cac, isp_global_cac_usr_cfg *usr_reg_cfg)
{
    usr_reg_cfg->param_red_a     = global_cac->param_red_a;
    usr_reg_cfg->param_red_b     = global_cac->param_red_b;
    usr_reg_cfg->param_red_c     = global_cac->param_red_c;
    usr_reg_cfg->param_blue_a    = global_cac->param_blue_a;
    usr_reg_cfg->param_blue_b    = global_cac->param_blue_b;
    usr_reg_cfg->param_blue_c    = global_cac->param_red_c;
    usr_reg_cfg->ver_norm_shift  = global_cac->ver_norm_shift;
    usr_reg_cfg->ver_norm_factor = global_cac->ver_norm_factor;
    usr_reg_cfg->hor_norm_shift  = global_cac->hor_norm_shift;
    usr_reg_cfg->hor_norm_factor = global_cac->hor_norm_factor;
    usr_reg_cfg->cor_var_thr     = global_cac->cor_var_thr;
    usr_reg_cfg->center_coor_ver = global_cac->center_coor_ver;
    usr_reg_cfg->center_coor_hor = global_cac->center_coor_hor;
    usr_reg_cfg->resh            = HI_TRUE;
    usr_reg_cfg->update_index  += 1;

    return HI_SUCCESS;
}

static hi_bool __inline check_gcac_open(isp_gcac *global_cac)
{
    return (global_cac->global_cac_en == HI_TRUE);
}

hi_s32 isp_gcac_init(hi_vi_pipe vi_pipe, hi_void *reg_cfg)
{
    hi_s32 ret;
    isp_reg_cfg *local_reg_cfg = (isp_reg_cfg *)reg_cfg;

    ret = gcac_ctx_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    gcac_regs_initialize(vi_pipe, local_reg_cfg);
    gcac_ext_regs_initialize(vi_pipe);
    gcac_initialize(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_gcac_run(hi_vi_pipe vi_pipe, const hi_void *stat_info,
                    hi_void *reg_cfg, hi_s32 rsv)
{
    hi_u8         i;
    hi_s32        ret;
    isp_usr_ctx  *isp_ctx    = HI_NULL;
    isp_gcac     *global_cac = HI_NULL;
    isp_reg_cfg  *local_reg_cfg    = (isp_reg_cfg *)reg_cfg;

    isp_get_ctx(vi_pipe, isp_ctx);
    gcac_get_ctx(vi_pipe, global_cac);
    isp_check_pointer_return(global_cac);

    if (isp_ctx->linkage.defect_pixel) {
        return HI_SUCCESS;
    }

    global_cac->global_cac_en = hi_ext_system_global_cac_enable_read(vi_pipe);

    for (i = 0; i < local_reg_cfg->cfg_num; i++) {
        local_reg_cfg->alg_reg_cfg[i].gcac_reg_cfg.global_cac_en = global_cac->global_cac_en;
    }

    local_reg_cfg->cfg_key.bit1_global_cac_cfg = 1;

    /* check hardware setting */
    if (!check_gcac_open(global_cac)) {
        return HI_SUCCESS;
    }

    ret = gcac_read_extregs(vi_pipe);
    if (ret != HI_SUCCESS) {
        return HI_FAILURE;
    }

    if (global_cac->coef_update_en) {
        for (i = 0; i < isp_ctx->block_attr.block_num; i++) {
            global_cac_usr_fw(i, global_cac, &local_reg_cfg->alg_reg_cfg[i].gcac_reg_cfg.usr_reg_cfg);
        }
    }

    return HI_SUCCESS;
}

static hi_s32 gcac_image_res_write(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_u8 i;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    for (i = 0; i < isp_ctx->block_attr.block_num; i++) {
        gcac_image_size(i, &reg_cfg->alg_reg_cfg[i].gcac_reg_cfg.usr_reg_cfg, &isp_ctx->block_attr);
        reg_cfg->alg_reg_cfg[i].gcac_reg_cfg.usr_reg_cfg.update_index += 1;
        reg_cfg->alg_reg_cfg[i].gcac_reg_cfg.usr_reg_cfg.resh          = HI_TRUE;
    }

    reg_cfg->cfg_key.bit1_global_cac_cfg = 1;

    return HI_SUCCESS;
}

hi_s32 isp_gcac_ctrl(hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value)
{
    isp_reg_cfg_attr  *local_reg_cfg = HI_NULL;
    isp_usr_ctx       *isp_ctx = HI_NULL;

    switch (cmd) {
        case ISP_CHANGE_IMAGE_MODE_SET:
            isp_regcfg_get_ctx(vi_pipe, local_reg_cfg);
            isp_check_pointer_return(local_reg_cfg);
            gcac_image_res_write(vi_pipe, &local_reg_cfg->reg_cfg);
            break;
        case ISP_WDR_MODE_SET:
            isp_get_ctx(vi_pipe, isp_ctx);

            if (isp_ctx->block_attr.block_num != isp_ctx->block_attr.pre_block_num) {
                isp_regcfg_get_ctx(vi_pipe, local_reg_cfg);
                isp_check_pointer_return(local_reg_cfg);
                gcac_image_res_write(vi_pipe, &local_reg_cfg->reg_cfg);
            }
            break;
        default :
            break;
    }

    return HI_SUCCESS;
}

hi_s32 isp_gcac_exit(hi_vi_pipe vi_pipe)
{
    hi_u8 i;
    isp_reg_cfg_attr  *local_reg_cfg    = HI_NULL;

    isp_regcfg_get_ctx(vi_pipe, local_reg_cfg);

    for (i = 0; i < local_reg_cfg->reg_cfg.cfg_num; i++) {
        local_reg_cfg->reg_cfg.alg_reg_cfg[i].gcac_reg_cfg.global_cac_en = HI_FALSE;
    }

    local_reg_cfg->reg_cfg.cfg_key.bit1_global_cac_cfg = 1;

    gcac_ctx_exit(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_alg_register_gcac(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx  *isp_ctx = HI_NULL;
    isp_alg_node *algs    = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_alg_check_return(isp_ctx->alg_key.bit1_gcac);
    algs = isp_search_alg(isp_ctx->algs);
    isp_check_pointer_return(algs);

    algs->alg_type = ISP_ALG_GCAC;
    algs->alg_func.pfn_alg_init = isp_gcac_init;
    algs->alg_func.pfn_alg_run  = isp_gcac_run;
    algs->alg_func.pfn_alg_ctrl = isp_gcac_ctrl;
    algs->alg_func.pfn_alg_exit = isp_gcac_exit;
    algs->used = HI_TRUE;

    return HI_SUCCESS;
}
