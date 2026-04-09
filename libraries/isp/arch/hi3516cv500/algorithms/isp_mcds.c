/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_mcds.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_alg.h"

#define MCDS_EN             1
#define MCDS_FILTER_MODE    1    /* 1: filter mode; 0: discard mode */

static hi_void mcds_static_reg_init(hi_vi_pipe vi_pipe, isp_mcds_static_reg_cfg *static_reg_cfg, isp_usr_ctx *isp_ctx)
{
    hi_s32 ret;
    static const hi_s16 coeff_filter_8tap_h[2][8] = { /* table size 2*8 */
        { -16, 0, 145, 254, 145, 0, -16, 0},          /* 1st -16, 0, 145, 254, 145, 0, -16, 0 */
        {  0, 0,   0, 256, 256, 0,   0, 0}            /* 2nd   0, 0,   0, 256, 256, 0,   0, 0 */
    };
    static const hi_s16 coeff_discard_8pixel_h[8] = {0, 0, 0, 512, 0, 0, 0, 0};   /* table 8  4th 512  */
    static const hi_s8 coeff_filer_4tap_v[2][4]   = {{4, 4, 6, 6}, {3, 3, 3, 3}}; /* table 2*4 4, 6, 3 */
    static const hi_s8 coeff_discard_4tap_v[4]    = {5, 6, 6, 6};                 /* table   4 5, 6,   */

    if (MCDS_FILTER_MODE) { /* filter mode */
        memcpy_s(static_reg_cfg->h_coef, 8 * sizeof(hi_s16), coeff_filter_8tap_h[0], 8 * sizeof(hi_s16)); /* table 8 */
        ret = memcpy_s(static_reg_cfg->v_coef, 2 * sizeof(hi_s8),    /* table 2 */
                       coeff_filer_4tap_v[0],   2 * sizeof(hi_s8));  /* table 2 */
        if (ret != 0) {
            isp_err_trace("ISP[%d] init mcds v_coef failed!\n", vi_pipe);
        }
    } else {    /* discard mode */
        memcpy_s(static_reg_cfg->h_coef, 8 * sizeof(hi_s16), coeff_discard_8pixel_h, 8 * sizeof(hi_s16)); /* table 8 */
        memcpy_s(static_reg_cfg->v_coef, 2 * sizeof(hi_s8), coeff_discard_4tap_v,    2 * sizeof(hi_s8));  /* table 2 */
    }

    static_reg_cfg->hcds_en      = 1;
    static_reg_cfg->coring_limit = 0;
    static_reg_cfg->midf_bldr    = 0x8;
    static_reg_cfg->static_resh  = HI_TRUE;
}

static hi_void mcds_dyna_reg_init(isp_mcds_dyna_reg_cfg *dyna_reg_cfg, isp_usr_ctx *isp_ctx)
{
    dyna_reg_cfg->midf_en   = 1;
    dyna_reg_cfg->dyna_resh = 1;
    if (isp_ctx->hdr_attr.format == PIXEL_FORMAT_YVU_SEMIPLANAR_422) {
        dyna_reg_cfg->vcds_en    = HI_FALSE;
    } else if (isp_ctx->hdr_attr.format == PIXEL_FORMAT_YVU_SEMIPLANAR_420) {
        dyna_reg_cfg->vcds_en    = HI_TRUE;      /* 422: 0; 420: 1 */
    } else { /* 400 is the same with 420 */
        dyna_reg_cfg->vcds_en    = HI_TRUE;
    }
}

static hi_void mcds_regs_initialize(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_u32 i;

    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        reg_cfg->alg_reg_cfg[i].mcds_reg_cfg.mcds_en = HI_TRUE;
        mcds_static_reg_init(vi_pipe, &(reg_cfg->alg_reg_cfg[i].mcds_reg_cfg.static_reg_cfg), isp_ctx);
        mcds_dyna_reg_init(&(reg_cfg->alg_reg_cfg[i].mcds_reg_cfg.dyna_reg_cfg), isp_ctx);
    }

    reg_cfg->cfg_key.bit1_mcds_cfg = 1;

    return;
}

hi_s32 mcds_proc_write(hi_vi_pipe vi_pipe, hi_isp_ctrl_proc_write *proc)
{
    return HI_SUCCESS;
}

hi_s32 isp_mcds_init(hi_vi_pipe vi_pipe, hi_void *reg_cfg_usr)
{
    isp_reg_cfg *reg_cfg = (isp_reg_cfg *)reg_cfg_usr;

    mcds_regs_initialize(vi_pipe, reg_cfg);

    return HI_SUCCESS;
}

hi_s32 isp_mcds_run(hi_vi_pipe vi_pipe, const hi_void *stat_info, hi_void *reg_cfg_usr, hi_s32 rsv)
{
    hi_bool en_edge_mark_read;
    hi_u8   i;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_reg_cfg *reg_cfg = (isp_reg_cfg *)reg_cfg_usr;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (isp_ctx->linkage.defect_pixel) {
        return HI_SUCCESS;
    }

    /* calculate every two interrupts */
    if ((isp_ctx->frame_cnt % 0x2 != 0) && (isp_ctx->linkage.snap_state != HI_TRUE)) {
        return HI_SUCCESS;
    }

    en_edge_mark_read = hi_ext_system_manual_edgemark_en_read(vi_pipe);

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        reg_cfg->alg_reg_cfg[i].mcds_reg_cfg.mcds_en = HI_TRUE;

        reg_cfg->alg_reg_cfg[i].mcds_reg_cfg.dyna_reg_cfg.dyna_resh = 1;

        if (isp_ctx->hdr_attr.format == PIXEL_FORMAT_YVU_SEMIPLANAR_422) {
            reg_cfg->alg_reg_cfg[i].mcds_reg_cfg.dyna_reg_cfg.vcds_en = HI_FALSE;
        } else if (isp_ctx->hdr_attr.format == PIXEL_FORMAT_YVU_SEMIPLANAR_420) {
            reg_cfg->alg_reg_cfg[i].mcds_reg_cfg.dyna_reg_cfg.vcds_en    = HI_TRUE;  /* 422: 0; 420: 1 */
        } else { /* 400 is the same with 420 */
            reg_cfg->alg_reg_cfg[i].mcds_reg_cfg.dyna_reg_cfg.vcds_en    = HI_TRUE;
        }

        if (en_edge_mark_read) { /* to close median filter when edgemark is open */
            reg_cfg->alg_reg_cfg[i].mcds_reg_cfg.dyna_reg_cfg.midf_en = 0;
        } else {
            reg_cfg->alg_reg_cfg[i].mcds_reg_cfg.dyna_reg_cfg.midf_en = 1;
        }
    }

    reg_cfg->cfg_key.bit1_mcds_cfg = 1;

    return HI_SUCCESS;
}

hi_s32 isp_mcds_ctrl(hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value)
{
    isp_reg_cfg_attr *reg_cfg = HI_NULL;

    switch (cmd) {
        case ISP_PROC_WRITE:
            mcds_proc_write(vi_pipe, (hi_isp_ctrl_proc_write *)value);
            break;
        case ISP_CHANGE_IMAGE_MODE_SET:
            isp_regcfg_get_ctx(vi_pipe, reg_cfg);
            isp_check_pointer_return(reg_cfg);
            isp_mcds_init(vi_pipe, &reg_cfg->reg_cfg);
            break;
        default:
            break;
    }
    return HI_SUCCESS;
}

hi_s32 isp_mcds_exit(hi_vi_pipe vi_pipe)
{
    hi_u8 i;
    isp_reg_cfg_attr *reg_cfg = HI_NULL;

    isp_regcfg_get_ctx(vi_pipe, reg_cfg);

    for (i = 0; i < reg_cfg->reg_cfg.cfg_num; i++) {
        reg_cfg->reg_cfg.alg_reg_cfg[i].mcds_reg_cfg.mcds_en = HI_FALSE;
    }

    reg_cfg->reg_cfg.cfg_key.bit1_mcds_cfg = 1;
    return HI_SUCCESS;
}

hi_s32 isp_alg_register_mcds(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_alg_node *algs = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_alg_check_return(isp_ctx->alg_key.bit1_mcds);
    algs = isp_search_alg(isp_ctx->algs);
    isp_check_pointer_return(algs);

    algs->alg_type = ISP_ALG_MCDS;
    algs->alg_func.pfn_alg_init = isp_mcds_init;
    algs->alg_func.pfn_alg_run  = isp_mcds_run;
    algs->alg_func.pfn_alg_ctrl = isp_mcds_ctrl;
    algs->alg_func.pfn_alg_exit = isp_mcds_exit;
    algs->used = HI_TRUE;

    return HI_SUCCESS;
}
