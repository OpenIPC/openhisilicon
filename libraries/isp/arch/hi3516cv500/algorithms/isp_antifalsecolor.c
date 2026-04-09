/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_antifalsecolor.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_math_utils.h"
#include "isp_proc.h"
#include <math.h>

static const hi_u8 g_afc_thd[ISP_AUTO_ISO_STRENGTH_NUM] = {8, 8, 8, 8, 7, 7, 7, 6, 6, 6, 5, 4, 3, 2, 1, 0};
static const hi_u8 g_afc_str[ISP_AUTO_ISO_STRENGTH_NUM] = {8, 8, 8, 8, 7, 7, 7, 6, 6, 6, 5, 4, 3, 2, 1, 0};

typedef struct {
    hi_bool enable;  /* RW;range:[0x0,0x1];format:1.0; anti_false_color enable */
    hi_u8   wdr_mode;
    hi_u8   actual_threshold;
    hi_u8   actual_strength;
    hi_u8   lut_afc_gray_ratio[ISP_AUTO_ISO_STRENGTH_NUM];             /* u5.0, */
    hi_u8   lut_afc_color_cmax_sel[ISP_AUTO_ISO_STRENGTH_NUM];               /* u4.0, */
    hi_u8   lut_afc_detg_sel[ISP_AUTO_ISO_STRENGTH_NUM];               /* u4.0, */
    hi_u16  lut_afc_hf_thresh_low[ISP_AUTO_ISO_STRENGTH_NUM];             /* 10.0, */
    hi_u16  lut_afc_hf_thresh_hig[ISP_AUTO_ISO_STRENGTH_NUM];             /* 10.0, */

    hi_isp_op_type op_type;
    hi_isp_antifalsecolor_auto_attr   auto_attr;
    hi_isp_antifalsecolor_manual_attr manual_attr;
} isp_antifalsecolor;

typedef struct {
    hi_u8  iso_index_upper;
    hi_u8  iso_index_lower;
    hi_u32 iso;
    hi_u32 iso1;
    hi_u32 iso2;
} isp_antifalsecolor_intp;

isp_antifalsecolor *g_afc_ctx[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define antifalsecolor_get_ctx(dev, ctx)   ((ctx) = g_afc_ctx[dev])
#define antifalsecolor_set_ctx(dev, ctx)   (g_afc_ctx[dev] = (ctx))
#define antifalsecolor_reset_ctx(dev)      (g_afc_ctx[dev] = HI_NULL)

hi_s32 anti_false_color_ctx_init(hi_vi_pipe vi_pipe)
{
    isp_antifalsecolor *fcr_ctx = HI_NULL;

    antifalsecolor_get_ctx(vi_pipe, fcr_ctx);

    if (fcr_ctx == HI_NULL) {
        fcr_ctx = (isp_antifalsecolor *)isp_malloc(sizeof(isp_antifalsecolor));
        if (fcr_ctx == HI_NULL) {
            isp_err_trace("Isp[%d] anti_false_color_ctx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset_s(fcr_ctx, sizeof(isp_antifalsecolor), 0, sizeof(isp_antifalsecolor));

    antifalsecolor_set_ctx(vi_pipe, fcr_ctx);

    return HI_SUCCESS;
}

hi_void anti_false_color_ctx_exit(hi_vi_pipe vi_pipe)
{
    isp_antifalsecolor *afc_ctx = HI_NULL;

    antifalsecolor_get_ctx(vi_pipe, afc_ctx);
    isp_free(afc_ctx);
    antifalsecolor_reset_ctx(vi_pipe);
}

static hi_void anti_false_color_init_fw_wdr(hi_vi_pipe vi_pipe)
{
    isp_antifalsecolor *anti_false_color = HI_NULL;

    hi_u8   lut_afc_gray_ratio[ISP_AUTO_ISO_STRENGTH_NUM] = {
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4
    };
    hi_u8   lut_afc_color_cmax_sel[ISP_AUTO_ISO_STRENGTH_NUM] = {
        4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
    };
    hi_u8   lut_afc_detg_sel[ISP_AUTO_ISO_STRENGTH_NUM] = {
        4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    hi_u16  lut_afc_hf_thd_low[ISP_AUTO_ISO_STRENGTH_NUM] = {
        96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96
    };
    hi_u16  lut_afc_hf_thd_hig[ISP_AUTO_ISO_STRENGTH_NUM] = {
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256
    };

    antifalsecolor_get_ctx(vi_pipe, anti_false_color);

    memcpy_s(anti_false_color->lut_afc_gray_ratio, sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM,
             lut_afc_gray_ratio, sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(anti_false_color->lut_afc_color_cmax_sel, sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM,
             lut_afc_color_cmax_sel, sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(anti_false_color->lut_afc_detg_sel, sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM,
             lut_afc_detg_sel, sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(anti_false_color->lut_afc_hf_thresh_low, sizeof(hi_u16)*ISP_AUTO_ISO_STRENGTH_NUM,
             lut_afc_hf_thd_low, sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(anti_false_color->lut_afc_hf_thresh_hig, sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM,
             lut_afc_hf_thd_hig, sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM);
}

static hi_void anti_false_color_init_fw_linear(hi_vi_pipe vi_pipe)
{
    isp_antifalsecolor *anti_false_color = HI_NULL;

    hi_u8   lut_afc_gray_ratio[ISP_AUTO_ISO_STRENGTH_NUM] = {
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4
    };
    hi_u8   lut_afc_color_cmax_sel[ISP_AUTO_ISO_STRENGTH_NUM] = {
        4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
    };
    hi_u8   lut_afc_detg_sel[ISP_AUTO_ISO_STRENGTH_NUM] = {
        4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    hi_u16  lut_afc_hf_thd_low[ISP_AUTO_ISO_STRENGTH_NUM] = {
        30, 30, 35, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48
    };
    hi_u16  lut_afc_hf_thd_hig[ISP_AUTO_ISO_STRENGTH_NUM] = {
        128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128
    };

    antifalsecolor_get_ctx(vi_pipe, anti_false_color);

    memcpy_s(anti_false_color->lut_afc_gray_ratio, sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM,
             lut_afc_gray_ratio, sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(anti_false_color->lut_afc_color_cmax_sel, sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM,
             lut_afc_color_cmax_sel, sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(anti_false_color->lut_afc_detg_sel, sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM,
             lut_afc_detg_sel, sizeof(hi_u8) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(anti_false_color->lut_afc_hf_thresh_low, sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM,
             lut_afc_hf_thd_low, sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM);
    memcpy_s(anti_false_color->lut_afc_hf_thresh_hig, sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM,
             lut_afc_hf_thd_hig, sizeof(hi_u16) * ISP_AUTO_ISO_STRENGTH_NUM);
}

static hi_s32 anti_false_color_set_long_frame_mode(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    if (is_linear_mode(isp_ctx->sns_wdr_mode) ||
        (isp_ctx->linkage.fswdr_mode == ISP_FSWDR_LONG_FRAME_MODE) ||
        (isp_ctx->linkage.fswdr_mode == ISP_FSWDR_AUTO_LONG_FRAME_MODE)) {
        anti_false_color_init_fw_linear(vi_pipe);
    } else {
        anti_false_color_init_fw_wdr(vi_pipe);
    }

    return HI_SUCCESS;
}

static hi_s32 anti_false_color_check_cmos_param(hi_vi_pipe vi_pipe, const hi_isp_cmos_afc *afc_cmos)
{
    hi_u8 i;

    isp_check_bool_return(afc_cmos->enable);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (afc_cmos->threshold[i] > 0x20) {
            isp_err_trace("err anti_false_color_threshold[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (afc_cmos->strength[i] > 0x1f) {
            isp_err_trace("err anti_false_color_strength[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    return HI_SUCCESS;
}

static hi_s32 anti_false_color_ext_regs_initialize(hi_vi_pipe vi_pipe)
{
    hi_u32 i;
    hi_s32 ret;
    isp_antifalsecolor *local_anti_false_color = HI_NULL;
    hi_isp_cmos_default *sns_dft = HI_NULL;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_sensor_get_default(vi_pipe, &sns_dft);
    antifalsecolor_get_ctx(vi_pipe, local_anti_false_color);

    local_anti_false_color->wdr_mode = isp_ctx->sns_wdr_mode;

    if (local_anti_false_color->wdr_mode != 0) {
        anti_false_color_init_fw_wdr(vi_pipe);
    } else {
        anti_false_color_init_fw_linear(vi_pipe);
    }

    hi_ext_system_afc_manual_mode_write(vi_pipe, HI_EXT_SYSTEM_ANTIFALSECOLOR_MANUAL_MODE_DEFAULT);

    local_anti_false_color->manual_attr.threshold = HI_EXT_SYSTEM_ANTIFALSECOLOR_MANUAL_THRESHOLD_DEFAULT;
    local_anti_false_color->manual_attr.strength  = HI_EXT_SYSTEM_ANTIFALSECOLOR_MANUAL_STRENGTH_DEFAULT;

    hi_ext_system_afc_manual_thd_write(vi_pipe, local_anti_false_color->manual_attr.threshold);
    hi_ext_system_afc_manual_str_write(vi_pipe, local_anti_false_color->manual_attr.strength);

    if (sns_dft->key.bit1_anti_false_color) {
        isp_check_pointer_return(sns_dft->anti_false_color);

        ret = anti_false_color_check_cmos_param(vi_pipe, sns_dft->anti_false_color);
        if (ret != HI_SUCCESS) {
            return ret;
        }

        hi_ext_system_afc_enable_write(vi_pipe, sns_dft->anti_false_color->enable);

        memcpy_s(local_anti_false_color->auto_attr.threshold, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
                 sns_dft->anti_false_color->threshold,  ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
        memcpy_s(local_anti_false_color->auto_attr.strength,  ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
                 sns_dft->anti_false_color->strength,   ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
    } else {
        hi_ext_system_afc_enable_write(vi_pipe, HI_EXT_SYSTEM_ANTIFALSECOLOR_ENABLE_DEFAULT);
        memcpy_s(local_anti_false_color->auto_attr.threshold, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
                 g_afc_thd, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
        memcpy_s(local_anti_false_color->auto_attr.strength, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8),
                 g_afc_str, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u8));
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        hi_ext_system_afc_auto_thd_write(vi_pipe, i, local_anti_false_color->auto_attr.threshold[i]);
        hi_ext_system_afc_auto_str_write(vi_pipe, i, local_anti_false_color->auto_attr.strength[i]);
    }

    return HI_SUCCESS;
}

static hi_void anti_false_color_regs_initialize(hi_vi_pipe vi_pipe, isp_reg_cfg  *reg_cfg)
{
    hi_u32 i;
    isp_antifalsecolor_static_cfg *static_reg_cfg = HI_NULL;
    isp_antifalsecolor_dyna_cfg   *dyna_reg_cfg   = HI_NULL;

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        reg_cfg->alg_reg_cfg[i].anti_false_color_reg_cfg.fcr_enable = HI_ISP_DEMOSAIC_FCR_EN_DEFAULT;
        static_reg_cfg = &reg_cfg->alg_reg_cfg[i].anti_false_color_reg_cfg.static_reg_cfg;
        dyna_reg_cfg   = &reg_cfg->alg_reg_cfg[i].anti_false_color_reg_cfg.dyna_reg_cfg;

        /* static */
        static_reg_cfg->resh       = HI_TRUE;
        static_reg_cfg->fcr_limit1 = HI_ISP_DEMOSAIC_FCR_LIMIT1_DEFAULT;
        static_reg_cfg->fcr_limit2 = HI_ISP_DEMOSAIC_FCR_LIMIT2_DEFAULT;
        static_reg_cfg->fcr_thr    = HI_ISP_DEMOSAIC_FCR_THR_DEFAULT;

        /* dynamic */
        dyna_reg_cfg->resh              = HI_TRUE;
        dyna_reg_cfg->fcr_gain          = HI_ISP_DEMOSAIC_FCR_GAIN_DEFAULT;
        dyna_reg_cfg->fcr_ratio         = HI_ISP_DEMOSAIC_FCR_RATIO_DEFAULT;
        dyna_reg_cfg->fcr_gray_ratio    = HI_ISP_DEMOSAIC_FCR_GRAY_RATIO_DEFAULT;
        dyna_reg_cfg->fcr_cmax_sel      = HI_ISP_DEMOSAIC_FCR_CMAX_SEL_DEFAULT;
        dyna_reg_cfg->fcr_detg_sel      = HI_ISP_DEMOSAIC_FCR_DETG_SEL_DEFAULT;
        dyna_reg_cfg->fcr_hf_thresh_low = HI_ISP_DEMOSAIC_FCR_HF_THRESH_LOW_DEFAULT;
        dyna_reg_cfg->fcr_hf_thresh_hig = HI_ISP_DEMOSAIC_FCR_HF_THRESH_HIGH_DEFAULT;
    }

    reg_cfg->cfg_key.bit1_fcr_cfg = 1;
}

static hi_void anti_false_color_read_extregs(hi_vi_pipe vi_pipe)
{
    hi_u32 i;
    isp_antifalsecolor *local_anti_false_color = HI_NULL;

    antifalsecolor_get_ctx(vi_pipe, local_anti_false_color);

    local_anti_false_color->op_type = hi_ext_system_afc_manual_mode_read(vi_pipe);

    if (local_anti_false_color->op_type == OP_TYPE_AUTO) {
        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
            local_anti_false_color->auto_attr.threshold[i] = hi_ext_system_afc_auto_thd_read(vi_pipe, i);
            local_anti_false_color->auto_attr.strength[i]  = hi_ext_system_afc_auto_str_read(vi_pipe, i);
        }
    } else if (local_anti_false_color->op_type == OP_TYPE_MANUAL) {
        local_anti_false_color->manual_attr.threshold      = hi_ext_system_afc_manual_thd_read(vi_pipe);
        local_anti_false_color->manual_attr.strength       = hi_ext_system_afc_manual_str_read(vi_pipe);
    }

    return;
}

hi_s32 isp_anti_false_color_init(hi_vi_pipe vi_pipe, hi_void *reg_cfg)
{
    hi_s32 ret;
    isp_reg_cfg  *st_reg_cfg  = (isp_reg_cfg *)reg_cfg;

    ret = anti_false_color_ctx_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    anti_false_color_regs_initialize(vi_pipe, st_reg_cfg);
    ret = anti_false_color_ext_regs_initialize(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    return HI_SUCCESS;
}

static hi_bool __inline check_anti_false_color_open(isp_antifalsecolor *anti_false_color)
{
    return (anti_false_color->enable == HI_TRUE);
}

static hi_void anti_false_color_cfg(isp_antifalsecolor_dyna_cfg *anti_false_color_dyna_cfg,
                                    isp_antifalsecolor *anti_false_color,
                                    isp_antifalsecolor_intp *afc_intp_param)
{
    hi_u32 iso, iso2, iso1;
    hi_u8  iso_index_upper, iso_index_lower;

    iso = afc_intp_param->iso;
    iso1 = afc_intp_param->iso1;
    iso2 = afc_intp_param->iso2;
    iso_index_upper = afc_intp_param->iso_index_upper;
    iso_index_lower = afc_intp_param->iso_index_lower;

    anti_false_color_dyna_cfg->fcr_gray_ratio = (hi_u8)linear_inter(iso,
        iso1, anti_false_color->lut_afc_gray_ratio[iso_index_lower],
        iso2, anti_false_color->lut_afc_gray_ratio[iso_index_upper]);
    anti_false_color_dyna_cfg->fcr_cmax_sel   = (hi_u8)linear_inter(iso,
        iso1, anti_false_color->lut_afc_color_cmax_sel[iso_index_lower],
        iso2, anti_false_color->lut_afc_color_cmax_sel[iso_index_upper]);
    anti_false_color_dyna_cfg->fcr_detg_sel   = (hi_u8)linear_inter(iso,
        iso1, anti_false_color->lut_afc_detg_sel[iso_index_lower],
        iso2, anti_false_color->lut_afc_detg_sel[iso_index_upper]);
    anti_false_color_dyna_cfg->fcr_hf_thresh_low = (hi_u16)linear_inter(iso,
        iso1, anti_false_color->lut_afc_hf_thresh_low[iso_index_lower],
        iso2, anti_false_color->lut_afc_hf_thresh_low[iso_index_upper]);
    anti_false_color_dyna_cfg->fcr_hf_thresh_hig = (hi_u16)linear_inter(iso,
        iso1, anti_false_color->lut_afc_hf_thresh_hig[iso_index_lower],
        iso2, anti_false_color->lut_afc_hf_thresh_hig[iso_index_upper]);
    anti_false_color_dyna_cfg->resh = HI_TRUE;
}

static hi_void isp_anti_false_color_fw(hi_u32 iso, hi_vi_pipe vi_pipe, hi_u8 cur_blk, isp_reg_cfg *reg)
{
    hi_u8  iso_index_upper, iso_index_lower;
    hi_u32 iso1;
    hi_u32 iso2;
    isp_antifalsecolor_dyna_cfg *anti_false_color_dyna_cfg =
                                     &reg->alg_reg_cfg[cur_blk].anti_false_color_reg_cfg.dyna_reg_cfg;
    isp_antifalsecolor *anti_false_color = HI_NULL;
    isp_antifalsecolor_intp afc_intp_param;

    antifalsecolor_get_ctx(vi_pipe, anti_false_color);

    iso_index_upper = get_iso_index(iso);
    iso_index_lower = MAX2((hi_s8)iso_index_upper - 1, 0);
    iso1 = get_iso(iso_index_lower);
    iso2 = get_iso(iso_index_upper);
    iso  = iso;

    afc_intp_param.iso_index_upper = iso_index_upper;
    afc_intp_param.iso_index_lower = iso_index_lower;
    afc_intp_param.iso1 = iso1;
    afc_intp_param.iso2 = iso2;
    afc_intp_param.iso = iso;

    anti_false_color_cfg(anti_false_color_dyna_cfg, anti_false_color, &afc_intp_param);

    if (anti_false_color->op_type == OP_TYPE_AUTO) {
        anti_false_color_dyna_cfg->fcr_ratio = (hi_u8)linear_inter(iso,
            iso1, anti_false_color->auto_attr.threshold[iso_index_lower],
            iso2, anti_false_color->auto_attr.threshold[iso_index_upper]);
        anti_false_color_dyna_cfg->fcr_gain  = (hi_u8)linear_inter(iso,
            iso1, anti_false_color->auto_attr.strength[iso_index_lower],
            iso2, anti_false_color->auto_attr.strength[iso_index_upper]);
    } else if (anti_false_color->op_type == OP_TYPE_MANUAL) {
        anti_false_color_dyna_cfg->fcr_ratio = anti_false_color->manual_attr.threshold;
        anti_false_color_dyna_cfg->fcr_gain  = anti_false_color->manual_attr.strength;
    }

    anti_false_color->actual_threshold = anti_false_color_dyna_cfg->fcr_ratio;
    anti_false_color->actual_strength  = anti_false_color_dyna_cfg->fcr_gain;

    anti_false_color_dyna_cfg->resh = HI_TRUE;
}

hi_s32 isp_anti_false_color_run(hi_vi_pipe vi_pipe, const hi_void *stat_info, hi_void *reg_cfg, hi_s32 rsv)
{
    hi_u8 i;
    hi_u8 cnt_num = 2;
    isp_reg_cfg *reg = (isp_reg_cfg *)reg_cfg;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_antifalsecolor *anti_false_color = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    antifalsecolor_get_ctx(vi_pipe, anti_false_color);
    isp_check_pointer_return(anti_false_color);

    if (isp_ctx->linkage.fswdr_mode != isp_ctx->linkage.pre_fswdr_mode) {
        anti_false_color_set_long_frame_mode(vi_pipe);
    }

    /* calculate every two interrupts */
    if ((isp_ctx->frame_cnt % cnt_num != 0) && (isp_ctx->linkage.snap_state != HI_TRUE)) {
        return HI_SUCCESS;
    }

    anti_false_color->enable = hi_ext_system_afc_enable_read(vi_pipe);

    for (i = 0; i < reg->cfg_num; i++) {
        reg->alg_reg_cfg[i].anti_false_color_reg_cfg.fcr_enable = anti_false_color->enable;
    }

    reg->cfg_key.bit1_fcr_cfg = 1;

    /* check hardware setting */
    if (!check_anti_false_color_open(anti_false_color)) {
        return HI_SUCCESS;
    }

    anti_false_color_read_extregs(vi_pipe);

    for (i = 0; i < reg->cfg_num; i++) {
        isp_anti_false_color_fw(isp_ctx->linkage.iso, vi_pipe, i, reg);
    }

    return HI_SUCCESS;
}

static hi_void anti_false_color_proc_write(hi_vi_pipe vi_pipe, hi_isp_ctrl_proc_write *proc)
{
    hi_isp_ctrl_proc_write proc_tmp;
    isp_antifalsecolor *anti_false_color = HI_NULL;

    antifalsecolor_get_ctx(vi_pipe, anti_false_color);
    isp_check_pointer_void_return(anti_false_color);

    if ((proc->proc_buff == HI_NULL) || (proc->buff_len == 0)) {
        return;
    }

    proc_tmp.proc_buff = proc->proc_buff;
    proc_tmp.buff_len  = proc->buff_len;

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "-----Anti False Color INFO-------------------------------------------------------------\n");

    isp_proc_printf(&proc_tmp, proc->write_len, "%12s" "%12s" "%12s\n", "Enable", "Threshold", "Strength");

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "%12u" "%12u" "%12u\n",
                    anti_false_color->enable, anti_false_color->actual_threshold, anti_false_color->actual_strength);

    proc->write_len += 1;
}

hi_s32 isp_anti_false_color_ctrl(hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value)
{
    isp_reg_cfg_attr  *local_reg_cfg   = HI_NULL;

    switch (cmd) {
        case ISP_WDR_MODE_SET:
            isp_regcfg_get_ctx(vi_pipe, local_reg_cfg);
            isp_check_pointer_return(local_reg_cfg);

            isp_anti_false_color_init(vi_pipe, (hi_void *)&local_reg_cfg->reg_cfg);
            break;

        case ISP_PROC_WRITE:
            anti_false_color_proc_write(vi_pipe, (hi_isp_ctrl_proc_write *)value);
            break;

        default:
            break;
    }

    return HI_SUCCESS;
}

hi_s32 isp_anti_false_color_exit(hi_vi_pipe vi_pipe)
{
    hi_u16 i;
    isp_reg_cfg_attr *local_reg_cfg = HI_NULL;

    isp_regcfg_get_ctx(vi_pipe, local_reg_cfg);

    for (i = 0; i < local_reg_cfg->reg_cfg.cfg_num; i++) {
        local_reg_cfg->reg_cfg.alg_reg_cfg[i].anti_false_color_reg_cfg.fcr_enable = HI_FALSE;
    }

    local_reg_cfg->reg_cfg.cfg_key.bit1_fcr_cfg = 1;

    anti_false_color_ctx_exit(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_alg_register_fcr(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_alg_node *algs = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_alg_check_return(isp_ctx->alg_key.bit1_fcr);

    algs = isp_search_alg(isp_ctx->algs);
    isp_check_pointer_return(algs);

    algs->alg_type = ISP_ALG_ANTIFALSECOLOR;
    algs->alg_func.pfn_alg_init = isp_anti_false_color_init;
    algs->alg_func.pfn_alg_run  = isp_anti_false_color_run;
    algs->alg_func.pfn_alg_ctrl = isp_anti_false_color_ctrl;
    algs->alg_func.pfn_alg_exit = isp_anti_false_color_exit;
    algs->used = HI_TRUE;

    return HI_SUCCESS;
}
