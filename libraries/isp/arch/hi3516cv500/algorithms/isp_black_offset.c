/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_black_offset.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_proc.h"

#define LUT_FACTOR 8

typedef struct {
    hi_bool pre_defect_pixel;
    hi_u8   black_level_change;
    hi_u8   wdr_mode_state;
    hi_u16  black_level[ISP_BAYER_CHN_NUM];
    hi_u16  rm_diff_black_level[ISP_BAYER_CHN_NUM];
    hi_u16  actual_black_level[ISP_BAYER_CHN_NUM];
    hi_isp_op_type op_type;
} isp_blacklevel_ctx;

typedef struct {
    hi_u8 shift;
    hi_u16 x0;
    hi_u16 x1;
    hi_s32 y0;
    hi_s32 y1;
} blc_linear_interp_cfg;

isp_blacklevel_ctx g_black_level_ctx[ISP_MAX_PIPE_NUM] = { { 0 } };
#define blacklevel_get_ctx(dev, blc_ctx) blc_ctx = &g_black_level_ctx[dev]

static hi_void blc_initialize(hi_vi_pipe vi_pipe)
{
    hi_u8  i;
    hi_u8  wdr_mode;
    isp_blacklevel_ctx      *blc_ctx = HI_NULL;
    hi_isp_cmos_black_level *sns_black_level = HI_NULL;
    isp_usr_ctx             *isp_ctx         = HI_NULL;

    blacklevel_get_ctx(vi_pipe, blc_ctx);
    isp_sensor_get_blc(vi_pipe, &sns_black_level);
    isp_get_ctx(vi_pipe, isp_ctx);

    wdr_mode = isp_ctx->sns_wdr_mode;

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        blc_ctx->black_level[i] = sns_black_level->black_level[i];
    }

    if (is_linear_mode(wdr_mode) || is_built_in_wdr_mode(wdr_mode)) {
        blc_ctx->wdr_mode_state = HI_FALSE;
    } else {
        blc_ctx->wdr_mode_state = HI_TRUE;
    }

    blc_ctx->pre_defect_pixel = HI_FALSE;
    blc_ctx->op_type          = OP_TYPE_AUTO;
}

static hi_void blc_ext_regs_initialize(hi_vi_pipe vi_pipe)
{
    isp_blacklevel_ctx *blc_ctx = HI_NULL;

    blacklevel_get_ctx(vi_pipe, blc_ctx);

    hi_ext_system_black_level_manual_mode_write(vi_pipe, HI_EXT_SYSTEM_BLACK_LEVEL_MANUAL_MODE_DEFAULT);
    hi_ext_system_black_level_change_write(vi_pipe, HI_EXT_SYSTEM_BLACK_LEVEL_CHANGE_DEFAULT);
    hi_ext_system_black_level_00_write(vi_pipe, blc_ctx->black_level[0]); /* 0:channel R */
    hi_ext_system_black_level_01_write(vi_pipe, blc_ctx->black_level[1]); /* 1:channel Gr */
    hi_ext_system_black_level_10_write(vi_pipe, blc_ctx->black_level[2]); /* 2:channel Gb */
    hi_ext_system_black_level_11_write(vi_pipe, blc_ctx->black_level[3]); /* 3:channel B */

    hi_ext_system_black_level_query_00_write(vi_pipe, blc_ctx->actual_black_level[0]); /* 0:channel R */
    hi_ext_system_black_level_query_01_write(vi_pipe, blc_ctx->actual_black_level[1]); /* 1:channel Gr */
    hi_ext_system_black_level_query_10_write(vi_pipe, blc_ctx->actual_black_level[2]); /* 2:channel Gb */
    hi_ext_system_black_level_query_11_write(vi_pipe, blc_ctx->actual_black_level[3]); /* 3:channel B */
}

static hi_void balance_black_level(isp_blacklevel_ctx *blc_ctx)
{
    hi_u8  i;
    hi_u16 offset_min;

    /* calulate Min blacklevel of RGrGbG channel */
    offset_min = blc_ctx->black_level[0];
    for (i = 1; i < ISP_BAYER_CHN_NUM; i++) {
        if (offset_min > blc_ctx->black_level[i]) {
            offset_min = blc_ctx->black_level[i];
        }
    }

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        blc_ctx->rm_diff_black_level[i] = offset_min;
    }
}

static hi_s32 blc_linear_interpol(hi_u16 xm, blc_linear_interp_cfg *intp_cfg)
{
    hi_s32 ym;
    hi_s32 y0 = intp_cfg->y0;
    hi_s32 y1 = intp_cfg->y1;

    if (xm <= (intp_cfg->x0 << intp_cfg->shift)) {
        return y0;
    }
    if (xm >= (intp_cfg->x1 << intp_cfg->shift)) {
        return y1;
    }

    ym = (y1 - y0) * (xm - (intp_cfg->x0 << intp_cfg->shift)) / div_0_to_1(intp_cfg->x1 - intp_cfg->x0) + y0;

    return ym;
}

static hi_void get_built_in_expander_blc(const hi_isp_cmos_expander *sns_expander,
                                         hi_u16 *sensor_blc, hi_u16 *expander_blc)
{
    hi_u8  i, j;
    hi_u8  shift   = 12 - LUT_FACTOR; /* 12: black level bit width */
    hi_u8  idx_up  = ISP_EXPANDER_POINT_NUM - 1;
    hi_u8  idx_low = 0;
    hi_u16 x[ISP_EXPANDER_POINT_NUM + 1] = {0};
    hi_u32 y[ISP_EXPANDER_POINT_NUM + 1] = {0};
    blc_linear_interp_cfg  intp_cfg;

    for (i = 1; i < ISP_EXPANDER_POINT_NUM + 1; i++) {
        x[i] = sns_expander->expander_point[i - 1].x;
        y[i] = sns_expander->expander_point[i - 1].y;
    }

    for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
        idx_up = ISP_EXPANDER_POINT_NUM - 1;

        for (i = 0; i < ISP_EXPANDER_POINT_NUM; i++) {
            if ((sensor_blc[j] >> shift) < x[i]) {
                idx_up = i;
                break;
            }
        }

        idx_low = (hi_u8)MAX2((hi_s8)idx_up - 1, 0);

        /* update blc_linear_interp_cfg */
        intp_cfg.x0 = x[idx_low];
        intp_cfg.x1 = x[idx_up];
        intp_cfg.y0 = y[idx_low];
        intp_cfg.y1 = y[idx_up];
        intp_cfg.shift = shift;
        expander_blc[j] = ((hi_u32)blc_linear_interpol(sensor_blc[j],  &intp_cfg)) >> (6 + shift); /* shift by 6 */
    }
}

static hi_void be_blc_dyna_regs_linear(hi_vi_pipe vi_pipe, isp_be_blc_dyna_cfg *dyna_blc, isp_blacklevel_ctx *blc_ctx)
{
    hi_u8 i, j;

    for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
        for (i = 0; i < ISP_WDR_CHN_MAX; i++) {
            dyna_blc->wdr_dg_blc[i].blc[j]  = blc_ctx->rm_diff_black_level[j] << 2; /* 4DG, shift by 2 */
            dyna_blc->wdr_blc[i].blc[j]     = blc_ctx->rm_diff_black_level[j];      /* WDR */
            dyna_blc->wdr_blc[i].out_blc    = 0;                                    /* WDR */
            dyna_blc->flicker_blc[i].blc[j] = 0;                                    /* flicker */
        }

        dyna_blc->expander_blc.blc[j] = 0;                                    /* expander */
        dyna_blc->lsc_blc.blc[j]      = blc_ctx->rm_diff_black_level[j] << 2; /* lsc, shift by 2 */
        dyna_blc->dg_blc.blc[j]       = blc_ctx->rm_diff_black_level[j] << 2; /* Dg, shift by 2 */
        dyna_blc->ae_blc.blc[j]       = blc_ctx->rm_diff_black_level[j] << 2; /* AE, shift by 2 */
        dyna_blc->mg_blc.blc[j]       = blc_ctx->rm_diff_black_level[j] << 2; /* MG, shift by 2 */
        dyna_blc->wb_blc.blc[j]       = blc_ctx->rm_diff_black_level[j] << 2; /* WB, shift by 2 */
    }
    /* bnr */
    dyna_blc->bnr_blc.blc[0]  = blc_ctx->rm_diff_black_level[0]; /* 12bits */

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        blc_ctx->actual_black_level[i] = blc_ctx->black_level[i];
    }
}

static hi_void be_blc_dyna_regs_wdr(hi_vi_pipe vi_pipe, isp_be_blc_dyna_cfg *dyna_blc, isp_blacklevel_ctx *blc_ctx)
{
    hi_u8  i, j;
    hi_u8  wdr_mode_state = blc_ctx->wdr_mode_state;
    hi_u16 wdr_out_blc = 0;

    if (wdr_mode_state == HI_FALSE) { /* reg value same as linear mode */
        be_blc_dyna_regs_linear(vi_pipe, dyna_blc, blc_ctx);
    } else if (wdr_mode_state == HI_TRUE) {
        wdr_out_blc = ((blc_ctx->rm_diff_black_level[0] >> 4) << 6); /* WDR outblc, shift by 6 - 4 */
        for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
            for (i = 0; i < ISP_WDR_CHN_MAX; i++) {
                dyna_blc->wdr_dg_blc[i].blc[j]  = blc_ctx->rm_diff_black_level[j] << 2; /* 4DG, shift 2 */
                dyna_blc->wdr_blc[i].blc[j]      = blc_ctx->rm_diff_black_level[j];    /* WDR */
                dyna_blc->wdr_blc[i].out_blc    = wdr_out_blc;                                  /* WDR */
                dyna_blc->flicker_blc[i].blc[j] = blc_ctx->rm_diff_black_level[j] << 2; /* flicker,2 */
            }

            dyna_blc->expander_blc.blc[j] = 0;              /* expander */
            dyna_blc->lsc_blc.blc[j]      = wdr_out_blc >> 6; /* lsc,shift by 6 */
            dyna_blc->dg_blc.blc[j]       = wdr_out_blc >> 6; /* Dg,shift by 6 */
            dyna_blc->ae_blc.blc[j]       = wdr_out_blc >> 6; /* AE,shift by 6 */
            dyna_blc->mg_blc.blc[j]       = wdr_out_blc >> 6; /* MG,shift by 6 */
            dyna_blc->wb_blc.blc[j]       = wdr_out_blc >> 6; /* WB,shift by 6 */
        }

        /* bnr */
        dyna_blc->bnr_blc.blc[0]  = wdr_out_blc >> 8; /* 12bits, shift by 8 bits */

        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            blc_ctx->actual_black_level[i] = blc_ctx->black_level[i];
        }
    }
}

static hi_s32 expander_check_cmos_param(hi_vi_pipe vi_pipe, const hi_isp_cmos_expander *cmos_expander)
{
    hi_u8 i;

    for (i = 0; i < ISP_EXPANDER_POINT_NUM; i++) {
        if (cmos_expander->expander_point[i].x > 0x101) {
            isp_err_trace("invalid expander_point[%d].x:%d!\n", i, cmos_expander->expander_point[i].x);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (cmos_expander->expander_point[i].y > 0x100000) {
            isp_err_trace("invalid expander_point[%d].y:%d!\n", i, cmos_expander->expander_point[i].y);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    return HI_SUCCESS;
}

static hi_s32 be_blc_dyna_regs_built_in(hi_vi_pipe vi_pipe, isp_be_blc_dyna_cfg *dyna_blc, isp_blacklevel_ctx *blc_ctx)
{
    hi_u8  i, j;
    hi_u16 black_level[ISP_BAYER_CHN_NUM] = {0};
    hi_u16 actual_black_level[ISP_BAYER_CHN_NUM] = {0};
    hi_s32 ret;
    hi_isp_cmos_default *sns_dft = HI_NULL;

    isp_sensor_get_default(vi_pipe, &sns_dft);

    if (sns_dft->key.bit1_expander) {
        isp_check_pointer_return(sns_dft->expander);
        ret = expander_check_cmos_param(vi_pipe, sns_dft->expander);
        if (ret != HI_SUCCESS) {
            return ret;
        }

        get_built_in_expander_blc(sns_dft->expander, blc_ctx->rm_diff_black_level, black_level);
        get_built_in_expander_blc(sns_dft->expander, blc_ctx->black_level, actual_black_level);
    } else {
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            black_level[i]        = blc_ctx->rm_diff_black_level[i] << 2; /* 14bits,shift by 2 */
            actual_black_level[i] = blc_ctx->black_level[i] << 2;      /* 14bits,shift by 2 */
        }
    }

    for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
        for (i = 0; i < ISP_WDR_CHN_MAX; i++) {
            dyna_blc->wdr_dg_blc[i].blc[j]  = 0; /* 4DG */
            dyna_blc->wdr_blc[i].blc[j]     = 0; /* WDR */
            dyna_blc->wdr_blc[i].out_blc    = 0; /* WDR */
            dyna_blc->flicker_blc[i].blc[j] = 0; /* flicker */
        }

        dyna_blc->expander_blc.blc[j] = 0;                 /* expander */
        dyna_blc->lsc_blc.blc[j]      = black_level[j]; /* lsc */
        dyna_blc->dg_blc.blc[j]       = black_level[j]; /* Dg */
        dyna_blc->ae_blc.blc[j]       = black_level[j]; /* AE */
        dyna_blc->mg_blc.blc[j]       = black_level[j]; /* MG */
        dyna_blc->wb_blc.blc[j]       = black_level[j]; /* WB */
    }

    /* bnr */
    dyna_blc->bnr_blc.blc[0] = black_level[0] >> 2; /* 12bits */

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        blc_ctx->actual_black_level[i] = actual_black_level[i]; /* Notice: Actual Blc is 14bits */
    }

    return HI_SUCCESS;
}

static hi_void be_blc_dyna_regs(hi_vi_pipe vi_pipe, hi_u8 wdr_mode, isp_be_blc_dyna_cfg *dyna_blc,
                                isp_blacklevel_ctx *blc_ctx)
{
    blc_ctx->wdr_mode_state = hi_ext_system_wdr_en_read(vi_pipe);

    if (is_linear_mode(wdr_mode)) {
        be_blc_dyna_regs_linear(vi_pipe, dyna_blc, blc_ctx);
    } else if (is_2to1_wdr_mode(wdr_mode) || is_3to1_wdr_mode(wdr_mode) || is_4to1_wdr_mode(wdr_mode)) {
        be_blc_dyna_regs_wdr(vi_pipe, dyna_blc, blc_ctx);
    } else if (is_built_in_wdr_mode(wdr_mode)) {
        be_blc_dyna_regs_built_in(vi_pipe, dyna_blc, blc_ctx);
    }

    dyna_blc->de_blc.blc[0] = blc_ctx->rm_diff_black_level[0];

    dyna_blc->resh_dyna = HI_TRUE;
}

static hi_void be_blc_static_regs(hi_u8 wdr_mode, isp_be_blc_static_cfg *static_blc)
{
    hi_u8 i;

    /* 4DG */
    for (i = 0; i < ISP_WDR_CHN_MAX; i++) {
        static_blc->wdr_dg_blc[i].blc_in  = HI_TRUE;
        static_blc->wdr_dg_blc[i].blc_out = HI_TRUE;
    }

    /* WDR */
    static_blc->wdr_blc[0].blc_out = HI_TRUE;
    /* lsc */
    static_blc->lsc_blc.blc_in     = HI_TRUE;
    static_blc->lsc_blc.blc_out    = HI_TRUE;
    /* Dg */
    static_blc->dg_blc.blc_in      = HI_TRUE;
    static_blc->dg_blc.blc_out     = HI_FALSE;
    /* AE */
    static_blc->ae_blc.blc_in      = HI_FALSE;
    /* MG */
    static_blc->mg_blc.blc_in      = HI_FALSE;
    /* WB */
    static_blc->wb_blc.blc_in      = HI_FALSE;
    static_blc->wb_blc.blc_out     = HI_FALSE;

    static_blc->resh_static = HI_TRUE;
}

static hi_void fe_blc_dyna_regs(isp_fe_blc_dyna_cfg *dyna_blc, isp_blacklevel_ctx *blc_ctx)
{
    hi_u8 j;

    for (j = 0; j < ISP_BAYER_CHN_NUM; j++) {
        dyna_blc->fe_blc.blc[j] = (blc_ctx->black_level[j] - blc_ctx->rm_diff_black_level[j]) << 2; /* 2 */
        dyna_blc->fe_dg_blc.blc[j] = blc_ctx->rm_diff_black_level[j] << 2; /* Fe Dg, shift by 2 */
        dyna_blc->fe_wb_blc.blc[j] = blc_ctx->rm_diff_black_level[j] << 2; /* Fe WB, shift by 2 */
        dyna_blc->fe_ae_blc.blc[j] = blc_ctx->rm_diff_black_level[j] << 2; /* Fe AE, shift by 2 */
    }

    dyna_blc->resh_dyna = HI_TRUE;
}

static hi_void fe_blc_static_regs(isp_fe_blc_static_cfg *static_blc)
{
    /* Fe Dg */
    static_blc->fe_dg_blc.blc_in  = HI_TRUE;
    static_blc->fe_dg_blc.blc_out = HI_TRUE;
    /* Fe WB */
    static_blc->fe_wb_blc.blc_in  = HI_TRUE;
    static_blc->fe_wb_blc.blc_out = HI_TRUE;
    /* Fe AE */
    static_blc->fe_ae_blc.blc_in  = HI_FALSE;
    /* Fe BLC */
    static_blc->fe_blc.blc_in     = HI_TRUE;

    static_blc->resh_static = HI_TRUE;
}

static hi_void update_wdr_sync_offset(isp_fswdr_sync_cfg *sync_reg_cfg, isp_blc_dyna_cfg *wdr_blc)
{
    sync_reg_cfg->offset0 = wdr_blc[0].blc[1];
}

static hi_void blc_regs_initialize(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_u8  i, wdr_mode;
    isp_blacklevel_ctx *blc_ctx = HI_NULL;
    isp_usr_ctx        *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    blacklevel_get_ctx(vi_pipe, blc_ctx);

    wdr_mode = isp_ctx->sns_wdr_mode;

    balance_black_level(blc_ctx);

    /* BE */
    for (i = 0; i < reg_cfg->cfg_num; i++) {
        be_blc_dyna_regs(vi_pipe, wdr_mode, &reg_cfg->alg_reg_cfg[i].be_blc_cfg.dyna_blc, blc_ctx);
        be_blc_static_regs(wdr_mode, &reg_cfg->alg_reg_cfg[i].be_blc_cfg.static_blc);
        update_wdr_sync_offset(&reg_cfg->alg_reg_cfg[i].wdr_reg_cfg.sync_reg_cfg,
                               reg_cfg->alg_reg_cfg[i].be_blc_cfg.dyna_blc.wdr_blc);
        reg_cfg->alg_reg_cfg[i].be_blc_cfg.resh_dyna_init = HI_TRUE;
    }
    reg_cfg->cfg_key.bit1_be_blc_cfg = 1;

    /* FE */
    fe_blc_dyna_regs(&reg_cfg->alg_reg_cfg[0].fe_blc_cfg.dyna_blc, blc_ctx);
    fe_blc_static_regs(&reg_cfg->alg_reg_cfg[0].fe_blc_cfg.static_blc);
    reg_cfg->alg_reg_cfg[0].fe_blc_cfg.resh_dyna_init = HI_TRUE;
    reg_cfg->cfg_key.bit1_fe_blc_cfg                  = 1;
}

static hi_void blc_read_extregs(hi_vi_pipe vi_pipe)
{
    isp_blacklevel_ctx *blc_ctx = HI_NULL;
    blacklevel_get_ctx(vi_pipe, blc_ctx);

    blc_ctx->black_level_change = hi_ext_system_black_level_change_read(vi_pipe);
    hi_ext_system_black_level_change_write(vi_pipe, HI_FALSE);

    if (blc_ctx->black_level_change) {
        blc_ctx->op_type         = hi_ext_system_black_level_manual_mode_read(vi_pipe);
        blc_ctx->black_level[0]  = hi_ext_system_black_level_00_read(vi_pipe); /* black level[0] */
        blc_ctx->black_level[1]  = hi_ext_system_black_level_01_read(vi_pipe); /* black level[1] */
        blc_ctx->black_level[2]  = hi_ext_system_black_level_10_read(vi_pipe); /* black level[2] */
        blc_ctx->black_level[3]  = hi_ext_system_black_level_11_read(vi_pipe); /* black level[3] */
    }
}

static hi_void blc_write_actual_value(hi_vi_pipe vi_pipe, hi_u16 *black_level)
{
    hi_ext_system_black_level_query_00_write(vi_pipe, black_level[0]); /* black level[0] */
    hi_ext_system_black_level_query_01_write(vi_pipe, black_level[1]); /* black level[1] */
    hi_ext_system_black_level_query_10_write(vi_pipe, black_level[2]); /* black level[2] */
    hi_ext_system_black_level_query_11_write(vi_pipe, black_level[3]); /* black level[3] */
}

static hi_s32 isp_blc_proc_write(hi_vi_pipe vi_pipe, hi_isp_ctrl_proc_write *proc)
{
    hi_isp_ctrl_proc_write proc_tmp;
    isp_blacklevel_ctx *blc_ctx = HI_NULL;

    blacklevel_get_ctx(vi_pipe, blc_ctx);

    if ((proc->proc_buff == HI_NULL) || (proc->buff_len == 0)) {
        return HI_FAILURE;
    }

    proc_tmp.proc_buff = proc->proc_buff;
    proc_tmp.buff_len  = proc->buff_len;

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "-----Black Level Actual INFO--------------------------------------------------------------\n");

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "%16s"     "%16s"     "%16s"     "%16s\n",
                    "BlcR", "BlcGr", "BlcGb", "BlcB");

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "%16u"     "%16u"     "%16u"     "%16u\n",
                    blc_ctx->actual_black_level[0],  /* black level[0] */
                    blc_ctx->actual_black_level[1],  /* black level[1] */
                    blc_ctx->actual_black_level[2],  /* black level[2] */
                    blc_ctx->actual_black_level[3]);  /* black level[3] */

    proc->write_len += 1;

    return HI_SUCCESS;
}

static hi_bool check_wdr_state(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx, isp_blacklevel_ctx *blc_ctx)
{
    hi_u8   wdr_en;
    hi_bool wdr_state_change;

    wdr_en = hi_ext_system_wdr_en_read(vi_pipe);

    if (is_2to1_wdr_mode(isp_ctx->sns_wdr_mode) ||
        is_3to1_wdr_mode(isp_ctx->sns_wdr_mode) ||
        is_4to1_wdr_mode(isp_ctx->sns_wdr_mode)) {
        wdr_state_change = (blc_ctx->wdr_mode_state == wdr_en) ? HI_FALSE : HI_TRUE;
    } else {
        wdr_state_change = HI_FALSE;
    }

    blc_ctx->wdr_mode_state = wdr_en;

    return wdr_state_change;
}

static hi_s32 isp_blc_wdr_mode_set(hi_vi_pipe vi_pipe, hi_void *reg_cfg_info)
{
    hi_u8 i;
    isp_usr_ctx             *isp_ctx         = HI_NULL;
    isp_blacklevel_ctx      *blc_ctx         = HI_NULL;
    hi_isp_cmos_black_level *sns_black_level = HI_NULL;
    isp_reg_cfg             *reg_cfg         = (isp_reg_cfg *)reg_cfg_info;

    isp_get_ctx(vi_pipe, isp_ctx);
    blacklevel_get_ctx(vi_pipe, blc_ctx);
    isp_sensor_get_blc(vi_pipe, &sns_black_level);

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        blc_ctx->black_level[i] = sns_black_level->black_level[i];
    }

    balance_black_level(blc_ctx);

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        be_blc_dyna_regs(vi_pipe, isp_ctx->sns_wdr_mode, &reg_cfg->alg_reg_cfg[i].be_blc_cfg.dyna_blc, blc_ctx);
        update_wdr_sync_offset(&reg_cfg->alg_reg_cfg[i].wdr_reg_cfg.sync_reg_cfg,
                               reg_cfg->alg_reg_cfg[i].be_blc_cfg.dyna_blc.wdr_blc);
        reg_cfg->alg_reg_cfg[i].be_blc_cfg.resh_dyna_init = HI_TRUE;
    }

    fe_blc_dyna_regs(&reg_cfg->alg_reg_cfg[0].fe_blc_cfg.dyna_blc, blc_ctx);

    blc_ext_regs_initialize(vi_pipe);

    reg_cfg->cfg_key.bit1_be_blc_cfg = 1;
    reg_cfg->cfg_key.bit1_fe_blc_cfg = 1;
    reg_cfg->alg_reg_cfg[0].fe_blc_cfg.resh_dyna_init         = HI_TRUE;
    reg_cfg->alg_reg_cfg[0].fe_blc_cfg.dyna_blc.resh_dyna     = HI_TRUE;
    reg_cfg->alg_reg_cfg[0].fe_blc_cfg.static_blc.resh_static = HI_TRUE;

    return HI_SUCCESS;
}

hi_s32 isp_blc_init(hi_vi_pipe vi_pipe, hi_void *reg_cfg_info)
{
    isp_reg_cfg *reg_cfg = (isp_reg_cfg *)reg_cfg_info;

    blc_initialize(vi_pipe);
    blc_regs_initialize(vi_pipe, reg_cfg);
    blc_ext_regs_initialize(vi_pipe);

    return HI_SUCCESS;
}

static hi_void dp_calib_mode_blc_cfg(isp_usr_ctx *isp_ctx, isp_blacklevel_ctx *blc_ctx, isp_reg_cfg *reg_cfg)
{
    hi_u8 i;

    if (isp_ctx->linkage.defect_pixel) {
        if (blc_ctx->pre_defect_pixel == HI_FALSE) {
            for (i = 0; i < reg_cfg->cfg_num; i++) {
                reg_cfg->alg_reg_cfg[i].be_blc_cfg.static_blc.wb_blc.blc_in = HI_TRUE;
                reg_cfg->alg_reg_cfg[i].be_blc_cfg.static_blc.resh_static   = HI_TRUE;
            }
        }
    } else if (blc_ctx->pre_defect_pixel) {
        for (i = 0; i < reg_cfg->cfg_num; i++) {
            reg_cfg->alg_reg_cfg[i].be_blc_cfg.static_blc.wb_blc.blc_in = HI_FALSE;
            reg_cfg->alg_reg_cfg[i].be_blc_cfg.static_blc.resh_static   = HI_TRUE;
        }
    }

    blc_ctx->pre_defect_pixel = isp_ctx->linkage.defect_pixel;
}

static hi_void blc_regs_run(hi_vi_pipe vi_pipe, isp_usr_ctx *isp_ctx, isp_blacklevel_ctx *blc_ctx, isp_reg_cfg *reg_cfg)
{
    hi_u8 i;

    balance_black_level(blc_ctx);
    fe_blc_dyna_regs(&reg_cfg->alg_reg_cfg[0].fe_blc_cfg.dyna_blc, blc_ctx);

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        be_blc_dyna_regs(vi_pipe, isp_ctx->sns_wdr_mode, &reg_cfg->alg_reg_cfg[i].be_blc_cfg.dyna_blc, blc_ctx);
        update_wdr_sync_offset(&reg_cfg->alg_reg_cfg[i].wdr_reg_cfg.sync_reg_cfg,
                               reg_cfg->alg_reg_cfg[i].be_blc_cfg.dyna_blc.wdr_blc);
    }

    blc_write_actual_value(vi_pipe, blc_ctx->actual_black_level);
}

hi_s32 isp_blc_run(hi_vi_pipe vi_pipe, const hi_void *stat_info, hi_void *reg_cfg_info, hi_s32 rsv)
{
    hi_bool wdr_state_change;
    hi_vi_pipe main_pipe = 0;
    isp_usr_ctx             *isp_ctx         = HI_NULL;
    hi_isp_cmos_black_level *sns_black_level = HI_NULL;
    isp_blacklevel_ctx      *blc_ctx         = HI_NULL;
    isp_reg_cfg             *reg_cfg         = (isp_reg_cfg *)reg_cfg_info;

    isp_get_ctx(vi_pipe, isp_ctx);
    blacklevel_get_ctx(vi_pipe, blc_ctx);

    wdr_state_change = check_wdr_state(vi_pipe, isp_ctx, blc_ctx);

    if (hi_ext_system_dpc_static_defect_type_read(vi_pipe) == 0) { /* hot pixel */
        dp_calib_mode_blc_cfg(isp_ctx, blc_ctx, reg_cfg);
    }

    blc_read_extregs(vi_pipe);

    reg_cfg->cfg_key.bit1_fe_blc_cfg = 1;
    reg_cfg->cfg_key.bit1_be_blc_cfg = 1;

    /* mannual mode update */
    if (blc_ctx->op_type == OP_TYPE_MANUAL) {
        blc_regs_run(vi_pipe, isp_ctx, blc_ctx, reg_cfg);
        return HI_SUCCESS;
    }

    /* some sensors's blacklevel is changed with iso. */
    isp_sensor_get_blc(vi_pipe, &sns_black_level);

    /* sensors's blacklevel is changed by cmos. */
    if (sns_black_level->update == HI_TRUE) {
        if (isp_ctx->linkage.snap_pipe_mode != ISP_SNAP_NONE) {
            if (vi_pipe == isp_ctx->linkage.picture_pipe_id) {
                main_pipe = isp_ctx->linkage.preview_pipe_id;
                isp_check_pipe_return(main_pipe);
                isp_sensor_get_blc(main_pipe, &sns_black_level);
            } else {
                isp_sensor_get_blc(vi_pipe, &sns_black_level);
            }

            memcpy_s(&blc_ctx->black_level[0], ISP_BAYER_CHN_NUM * sizeof(hi_u16),
                     &sns_black_level->black_level[0], ISP_BAYER_CHN_NUM * sizeof(hi_u16));
        }

        isp_sensor_update_blc(vi_pipe);
        memcpy_s(&blc_ctx->black_level[0], ISP_BAYER_CHN_NUM * sizeof(hi_u16),
                 &sns_black_level->black_level[0], ISP_BAYER_CHN_NUM * sizeof(hi_u16));

        blc_regs_run(vi_pipe, isp_ctx, blc_ctx, reg_cfg);

        return HI_SUCCESS;
    }

    /* sensors's blacklevel is changed by mpi. */
    if (((blc_ctx->black_level_change == HI_TRUE) && (blc_ctx->op_type == OP_TYPE_AUTO)) ||
        (wdr_state_change == HI_TRUE)) {
        isp_sensor_update_blc(vi_pipe);
        memcpy_s(&blc_ctx->black_level[0], ISP_BAYER_CHN_NUM * sizeof(hi_u16),
                 &sns_black_level->black_level[0], ISP_BAYER_CHN_NUM * sizeof(hi_u16));

        blc_regs_run(vi_pipe, isp_ctx, blc_ctx, reg_cfg);
    }

    return HI_SUCCESS;
}

hi_s32 isp_blc_ctrl(hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value)
{
    isp_reg_cfg_attr *reg_cfg_attr = HI_NULL;

    switch (cmd) {
        case ISP_CHANGE_IMAGE_MODE_SET:
        case ISP_WDR_MODE_SET:
            isp_regcfg_get_ctx(vi_pipe, reg_cfg_attr);
            isp_check_pointer_return(reg_cfg_attr);
            isp_blc_wdr_mode_set(vi_pipe, (hi_void *)&reg_cfg_attr->reg_cfg);
            break;
        case ISP_PROC_WRITE:
            isp_blc_proc_write(vi_pipe, (hi_isp_ctrl_proc_write *)value);
            break;
        default:
            break;
    }
    return HI_SUCCESS;
}

hi_s32 isp_blc_exit(hi_vi_pipe vi_pipe)
{
    return HI_SUCCESS;
}

hi_s32 isp_alg_register_blc(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_alg_node *algs = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_alg_check_return(isp_ctx->alg_key.bit1_blc);
    algs = isp_search_alg(isp_ctx->algs);
    isp_check_pointer_return(algs);

    algs->alg_type = ISP_ALG_BLC;
    algs->alg_func.pfn_alg_init = isp_blc_init;
    algs->alg_func.pfn_alg_run  = isp_blc_run;
    algs->alg_func.pfn_alg_ctrl = isp_blc_ctrl;
    algs->alg_func.pfn_alg_exit = isp_blc_exit;
    algs->used = HI_TRUE;

    return HI_SUCCESS;
}

