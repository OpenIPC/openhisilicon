/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_detail.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include <math.h>
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_proc.h"
#include "isp_math_utils.h"

#define HI_ISP_DE_LUT_LEN 16

typedef struct {
    hi_bool init;
    hi_bool enable;
    hi_bool bnr_en;
    hi_bool de_attr_update;

    hi_u16 luma_gain_lut[HI_ISP_DE_LUMA_GAIN_LUT_N];  /* U1.8  [0,256] */

    hi_u16 proc_global_gain;
    hi_u16 proc_gain_lf;
    hi_u16 proc_gain_hf;

    hi_isp_op_type op_type;
    hi_isp_de_auto_attr autom;
    hi_isp_de_manual_attr manual;
} isp_detail;

isp_detail *g_detail_ctx[ISP_MAX_PIPE_NUM] = { HI_NULL };

#define detail_get_ctx(dev, ctx) ((ctx) = g_detail_ctx[dev])
#define detail_set_ctx(dev, ctx) (g_detail_ctx[dev] = (ctx))
#define detail_reset_ctx(dev)    (g_detail_ctx[dev] = HI_NULL)

static hi_u16 g_au16sw_luma_gain_lut[HI_ISP_DE_LUMA_GAIN_LUT_N] = {
    256, 192, 180, 170, 162, 154, 146, 138, 132, 128, 128, 128, 128, 128, 128, 128, 128
};
static hi_u16 g_sgm4_gain[ISP_AUTO_ISO_STRENGTH_NUM] = {
    12,  12,  12,  12,  18,  30,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12
};
static hi_u16 g_sgm3_gain[ISP_AUTO_ISO_STRENGTH_NUM] = {
    4,   4,   4,   4,   8,   10,  4,   4,   4,   4,   4,   4,   4,   4,   4,   4
};
static hi_u16 g_sgm2_gain[ISP_AUTO_ISO_STRENGTH_NUM] = {
    2,   2,   2,   2,   4,   5,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2
};

typedef struct {
    hi_u8 iso_index_upper, iso_index_lower;
    hi_u32 iso, iso1, iso2;
} isp_detail_iso;

hi_s32 detail_ctx_init(hi_vi_pipe vi_pipe)
{
    isp_detail *detail = HI_NULL;

    detail_get_ctx(vi_pipe, detail);

    if (detail == HI_NULL) {
        detail = (isp_detail *)isp_malloc(sizeof(isp_detail));
        if (detail == HI_NULL) {
            isp_err_trace("isp[%d] detail_ctx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset_s(detail, sizeof(isp_detail), 0, sizeof(isp_detail));

    detail_set_ctx(vi_pipe, detail);

    return HI_SUCCESS;
}

hi_void detail_ctx_exit(hi_vi_pipe vi_pipe)
{
    isp_detail *detail = HI_NULL;

    detail_get_ctx(vi_pipe, detail);
    isp_free(detail);
    detail_reset_ctx(vi_pipe);
}

static hi_s32 detail_check_cmos_param(hi_vi_pipe vi_pipe, const hi_isp_cmos_detail *cmos_detail)
{
    hi_u8 i;

    isp_check_bool_return(cmos_detail->attr.enable);

    if (cmos_detail->attr.op_type >= OP_TYPE_BUTT) {
        isp_err_trace("invalid op_type!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for (i = 0; i < HI_ISP_DE_LUMA_GAIN_LUT_N; i++) {
        if (cmos_detail->attr.luma_gain_lut[i] > 0x100) {
            isp_err_trace("invalid luma_gain_lut[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (cmos_detail->attr.auto_attr.global_gain[i] > 0x100) {
            isp_err_trace("invalid global_gain[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (cmos_detail->attr.auto_attr.gain_lf[i] > 0x20) {
            isp_err_trace("invalid gain_lf[%d]\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (cmos_detail->attr.auto_attr.gain_hf[i] > 0x20) {
            isp_err_trace("invalid gain_hf[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    if (cmos_detail->attr.manual_attr.global_gain > 0x100) {
        isp_err_trace("invalid global_gain!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (cmos_detail->attr.manual_attr.gain_lf > 0x20) {
        isp_err_trace("invalid gain_lf!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (cmos_detail->attr.manual_attr.gain_hf > 0x20) {
        isp_err_trace("invalid gain_hf!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

static hi_s32 detail_ext_regs_initialize(hi_vi_pipe vi_pipe)
{
    hi_u8 i;
    hi_s32 ret;
    isp_detail *detail = HI_NULL;
    hi_isp_cmos_default *sns_dft = HI_NULL;

    detail_get_ctx(vi_pipe, detail);
    isp_sensor_get_default(vi_pipe, &sns_dft);

    detail->bnr_en = hi_ext_system_bayernr_enable_read(vi_pipe);

    if (sns_dft->key.bit1_detail) {
        isp_check_pointer_return(sns_dft->detail);

        ret = detail_check_cmos_param(vi_pipe, sns_dft->detail);
        if (ret != HI_SUCCESS) {
            return ret;
        }

        detail->enable  = (detail->bnr_en == HI_FALSE) ? (HI_FALSE) : (sns_dft->detail->attr.enable);
        detail->op_type = sns_dft->detail->attr.op_type;

        memcpy_s(detail->luma_gain_lut, HI_ISP_DE_LUMA_GAIN_LUT_N * sizeof(hi_u16),
                 sns_dft->detail->attr.luma_gain_lut, HI_ISP_DE_LUMA_GAIN_LUT_N * sizeof(hi_u16));
        memcpy_s(detail->autom.global_gain, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16),
                 sns_dft->detail->attr.auto_attr.global_gain, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16));
        memcpy_s(detail->autom.gain_lf, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16),
                 sns_dft->detail->attr.auto_attr.gain_lf, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16));
        memcpy_s(detail->autom.gain_hf, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16),
                 sns_dft->detail->attr.auto_attr.gain_hf, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16));

        detail->manual.global_gain = sns_dft->detail->attr.manual_attr.global_gain;
        detail->manual.gain_lf = sns_dft->detail->attr.manual_attr.gain_lf;
        detail->manual.gain_hf = sns_dft->detail->attr.manual_attr.gain_hf;
    } else {
        detail->enable = (detail->bnr_en == HI_FALSE) ? (HI_FALSE) : (HI_EXT_SYSTEM_DETAIL_ENABLE_DEFAULT);
        detail->op_type = HI_EXT_SYSTEM_DETAIL_MANU_MODE_DEFAULT;

        memcpy_s(detail->luma_gain_lut, HI_ISP_DE_LUMA_GAIN_LUT_N * sizeof(hi_u16),
                 g_au16sw_luma_gain_lut, HI_ISP_DE_LUMA_GAIN_LUT_N * sizeof(hi_u16));

        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
            detail->autom.global_gain[i] = HI_EXT_SYSTEM_DETAIL_MANUAL_GLOBAL_GAIN_DEFAULT;
            detail->autom.gain_lf[i] = HI_EXT_SYSTEM_DETAIL_MANUAL_GAIN_LF_DEFAULT;
            detail->autom.gain_hf[i] = HI_EXT_SYSTEM_DETAIL_MANUAL_GAIN_HF_DEFAULT;
        }

        /* manual */
        detail->manual.global_gain = HI_EXT_SYSTEM_DETAIL_MANUAL_GLOBAL_GAIN_DEFAULT;
        detail->manual.gain_lf = HI_EXT_SYSTEM_DETAIL_MANUAL_GAIN_LF_DEFAULT;
        detail->manual.gain_hf = HI_EXT_SYSTEM_DETAIL_MANUAL_GAIN_HF_DEFAULT;
    }

    hi_ext_system_detail_enable_write(vi_pipe, detail->enable);
    hi_ext_system_detail_manual_mode_write(vi_pipe, detail->op_type);

    for (i = 0; i < HI_ISP_DE_LUMA_GAIN_LUT_N; i++) {
        hi_ext_system_detail_luma_gain_lut_write(vi_pipe, i, detail->luma_gain_lut[i]);
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        hi_ext_system_detail_auto_global_gain_write(vi_pipe, i, detail->autom.global_gain[i]);
        hi_ext_system_detail_auto_gain_lf_write(vi_pipe, i, detail->autom.gain_lf[i]);
        hi_ext_system_detail_auto_gain_hf_write(vi_pipe, i, detail->autom.gain_hf[i]);
    }

    hi_ext_system_detail_manual_global_gain_write(vi_pipe, detail->manual.global_gain);
    hi_ext_system_detail_manual_gain_lf_write(vi_pipe, detail->manual.gain_lf);
    hi_ext_system_detail_manual_gain_hf_write(vi_pipe, detail->manual.gain_hf);
    hi_ext_system_detail_attr_update_write(vi_pipe, HI_FALSE);

    detail->init = HI_TRUE;

    return HI_SUCCESS;
}

static hi_void detail_static_regs_initialize(isp_detail_static_cfg *static_reg_cfg)
{
    static_reg_cfg->de_neg_clip = HI_ISP_BNR_DE_NEGCLIP_DEFAULT;
    static_reg_cfg->de_pos_clip = HI_ISP_BNR_DE_POSCLIP_DEFAULT;
    static_reg_cfg->clip_ratio = 0x50; /* 80 */
    static_reg_cfg->luma_scale_x0 = 0x46; /* 70 */
    static_reg_cfg->luma_scale_x1 = 0xD2; /* 210 */
    static_reg_cfg->luma_scale_y0 = 0x100; /* 256 */
    static_reg_cfg->luma_scale_y1 = 0x28; /* 40 */
    static_reg_cfg->luma_scale_k = 0x18; /* 24 */
    static_reg_cfg->satu_gain_x0 = 0x8c; /* 140 */
    static_reg_cfg->satu_gain_x1 = 0xC8; /* 200 */
    static_reg_cfg->satu_gain_y0 = 0x100; /* 256 */
    static_reg_cfg->satu_gain_y1 = 0x50; /* 80 */
    static_reg_cfg->satu_gain_k = 0x19; /* 25 */
    static_reg_cfg->edge_scale_x0 = 0x0; /* 0 */
    static_reg_cfg->edge_scale_x1 = 0x100; /* 256 */
    static_reg_cfg->edge_scale_y0 = 0x100; /* 256 */
    static_reg_cfg->edge_scale_y1 = 0x100; /* 256 */
    static_reg_cfg->edge_scale_k = 0x0; /* 0 */
    static_reg_cfg->global_ratio = 0x10; /* 16 */
    static_reg_cfg->sgm_mode = HI_TRUE;
    static_reg_cfg->edge_mode = HI_FALSE;
    static_reg_cfg->luma_gain_mode = HI_FALSE;
    static_reg_cfg->luma_bit_mode = HI_FALSE;

    static_reg_cfg->resh = HI_TRUE;
}

static hi_void detail_dyna_regs_initialize(hi_vi_pipe vi_pipe, isp_detail_dyna_cfg *detail_dyna_reg_cfg,
                                           isp_usr_ctx *isp_ctx)
{
    detail_dyna_reg_cfg->sgm_pos4_gain = 0xC; /* 12; */
    detail_dyna_reg_cfg->sgm_pos3_gain = 0x4; /* 4; */
    detail_dyna_reg_cfg->sgm_pos2_gain = 0x2; /* 2; */
    detail_dyna_reg_cfg->sgm_neg4_gain = 0xC; /* 12; */
    detail_dyna_reg_cfg->sgm_neg3_gain = 0x4; /* 4; */
    detail_dyna_reg_cfg->sgm_neg2_gain = 0x2; /* 2; */
    detail_dyna_reg_cfg->gain_lf = 0x9; /* 9; */
    detail_dyna_reg_cfg->gain_hf_pos = 0xC; /* 12; */
    detail_dyna_reg_cfg->gain_hf_neg = 0xC; /* 12; */

    detail_dyna_reg_cfg->resh = HI_TRUE;
}

static hi_void detail_usr_regs_initialize(isp_detail_usr_cfg *detail_usr_reg_cfg)
{
    hi_u16 i;

    for (i = 0; i < HI_ISP_DE_LUMA_GAIN_LUT_N; i++) {
        detail_usr_reg_cfg->luma_gain_lut[i] = g_au16sw_luma_gain_lut[i];
    }

    detail_usr_reg_cfg->resh = HI_TRUE;
}

static hi_void detail_regs_initialize(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_u8 i;
    hi_u8 block_num;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_detail *detail = HI_NULL;
    isp_detail_static_cfg *static_reg_cfg = HI_NULL;
    isp_detail_dyna_cfg *dyna_reg_cfg = HI_NULL;
    isp_detail_usr_cfg *usr_reg_cfg = HI_NULL;

    detail_get_ctx(vi_pipe, detail);
    isp_get_ctx(vi_pipe, isp_ctx);

    block_num = isp_ctx->block_attr.block_num;

    for (i = 0; i < block_num; i++) {
        static_reg_cfg = &reg_cfg->alg_reg_cfg[i].de_reg_cfg.static_reg_cfg;
        dyna_reg_cfg = &reg_cfg->alg_reg_cfg[i].de_reg_cfg.dyna_reg_cfg;
        usr_reg_cfg = &reg_cfg->alg_reg_cfg[i].de_reg_cfg.usr_reg_cfg;

        reg_cfg->alg_reg_cfg[i].de_reg_cfg.de_enable = detail->enable;
        reg_cfg->alg_reg_cfg[i].de_reg_cfg.kernel_reg_cfg.de_enable = detail->enable;
        detail_static_regs_initialize(static_reg_cfg);
        detail_dyna_regs_initialize(vi_pipe, dyna_reg_cfg, isp_ctx);
        detail_usr_regs_initialize(usr_reg_cfg);
    }

    reg_cfg->cfg_key.bit1_detail_cfg = 1;
}

static hi_bool __inline check_de_open(isp_detail *detail)
{
    return (detail->enable == HI_TRUE);
}

static void detail_read_extregs(hi_vi_pipe vi_pipe)
{
    hi_u8 i;
    hi_u32 exp_ratio[3] = { 0 }; /* 3 0 */
    isp_detail *detail = HI_NULL;
    isp_usr_ctx *isp_ctx = HI_NULL;

    detail_get_ctx(vi_pipe, detail);
    isp_get_ctx(vi_pipe, isp_ctx);

    detail->de_attr_update = hi_ext_system_detail_attr_update_read(vi_pipe);
    hi_ext_system_detail_attr_update_write(vi_pipe, HI_FALSE);

    if (detail->de_attr_update == HI_TRUE) {
        detail->op_type = hi_ext_system_detail_manual_mode_read(vi_pipe);

        for (i = 0; i < HI_ISP_DE_LUMA_GAIN_LUT_N; i++) {
            detail->luma_gain_lut[i] = hi_ext_system_detail_luma_gain_lut_read(vi_pipe, i);
        }

        memcpy_s(exp_ratio, 3 * sizeof(hi_u32), isp_ctx->linkage.exp_ratio_lut, 3 * sizeof(hi_u32));  /* 3 */

        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
            detail->autom.global_gain[i] = hi_ext_system_detail_auto_global_gain_read(vi_pipe, i);
            detail->autom.gain_lf[i] = hi_ext_system_detail_auto_gain_lf_read(vi_pipe, i);
            detail->autom.gain_hf[i] = hi_ext_system_detail_auto_gain_hf_read(vi_pipe, i);
        }

        detail->manual.global_gain = hi_ext_system_detail_manual_global_gain_read(vi_pipe);
        detail->manual.gain_lf = hi_ext_system_detail_manual_gain_lf_read(vi_pipe);
        detail->manual.gain_hf = hi_ext_system_detail_manual_gain_hf_read(vi_pipe);
    }
}

hi_s32 detail_proc_write(hi_vi_pipe vi_pipe, hi_isp_ctrl_proc_write *proc)
{
    hi_isp_ctrl_proc_write proc_tmp;
    isp_detail *detail = HI_NULL;

    detail_get_ctx(vi_pipe, detail);
    isp_check_pointer_return(detail);

    if ((proc->proc_buff == HI_NULL) ||
        (proc->buff_len == 0)) {
        return HI_FAILURE;
    }

    proc_tmp.proc_buff = proc->proc_buff;
    proc_tmp.buff_len = proc->buff_len;

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "-----DetailEnhance INFO----------------------------------------------------------\n");

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "%16s" "%16s" "%16s" "%16s" "%16s\n",
                    "Enable", "enOpType", "GlobalGain", "GainHf", "GainLf");

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "%16u" "%16u" "%16u" "%16u" "%16u\n",
                    detail->enable, detail->op_type,
                    (hi_u16)detail->proc_global_gain,
                    (hi_u16)detail->proc_gain_hf,
                    (hi_u16)detail->proc_gain_lf);

    proc->write_len += 1;

    return HI_SUCCESS;
}

hi_s32 isp_detail_init(hi_vi_pipe vi_pipe, hi_void *reg_cfg)
{
    hi_s32 ret;
    isp_reg_cfg *pt_reg_cfg = (isp_reg_cfg *)reg_cfg;

    ret = detail_ctx_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    ret = detail_ext_regs_initialize(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    detail_regs_initialize(vi_pipe, pt_reg_cfg);
    return HI_SUCCESS;
}

static void de_ext_cfg(hi_vi_pipe vi_pipe, isp_detail_dyna_cfg *detail_dyna_cfg, isp_detail_usr_cfg *detail_usr_cfg,
                       isp_detail *detail, isp_detail_iso iso_struc)
{
    hi_u32 i = 0;
    hi_s32 sw_global_gain;

    hi_u8 iso_index_upper, iso_index_lower;
    hi_u32 iso, iso1, iso2;

    iso_index_upper = iso_struc.iso_index_upper;
    iso_index_lower = iso_struc.iso_index_lower;
    iso = iso_struc.iso;
    iso1 = iso_struc.iso1;
    iso2 = iso_struc.iso2;

    if (detail->op_type == OP_TYPE_AUTO) {
        detail_dyna_cfg->gain_lf = (hi_u16)linear_inter(iso, iso1, detail->autom.gain_lf[iso_index_lower],
                                                        iso2, detail->autom.gain_lf[iso_index_upper]);
        detail_dyna_cfg->gain_hf_pos = (hi_u16)linear_inter(iso, iso1, detail->autom.gain_hf[iso_index_lower],
                                                            iso2, detail->autom.gain_hf[iso_index_upper]);
        detail_dyna_cfg->gain_hf_neg = (hi_u16)detail_dyna_cfg->gain_hf_pos;
        sw_global_gain = (hi_s32)linear_inter(iso, iso1, detail->autom.global_gain[iso_index_lower],
                                              iso2, detail->autom.global_gain[iso_index_upper]);
    } else {
        detail_dyna_cfg->gain_lf = (hi_s32)detail->manual.gain_lf;
        detail_dyna_cfg->gain_hf_pos = (hi_s32)detail->manual.gain_hf;
        detail_dyna_cfg->gain_hf_neg = (hi_s32)detail->manual.gain_hf;
        sw_global_gain = (hi_s32)detail->manual.global_gain;
    }

    detail_dyna_cfg->sgm_pos4_gain = (hi_u8)linear_inter(iso, iso1, g_sgm4_gain[iso_index_lower],
                                                         iso2, g_sgm4_gain[iso_index_upper]);
    detail_dyna_cfg->sgm_pos3_gain = (hi_u8)linear_inter(iso, iso1, g_sgm3_gain[iso_index_lower],
                                                         iso2, g_sgm3_gain[iso_index_upper]);
    detail_dyna_cfg->sgm_pos2_gain = (hi_u8)linear_inter(iso, iso1, g_sgm2_gain[iso_index_lower],
                                                         iso2, g_sgm2_gain[iso_index_upper]);
    detail_dyna_cfg->sgm_neg4_gain = detail_dyna_cfg->sgm_pos4_gain;
    detail_dyna_cfg->sgm_neg3_gain = detail_dyna_cfg->sgm_pos3_gain;
    detail_dyna_cfg->sgm_neg2_gain = detail_dyna_cfg->sgm_pos2_gain;

    for (i = 0; i < HI_ISP_DE_LUMA_GAIN_LUT_N; i++) {
        detail_usr_cfg->luma_gain_lut[i] = (hi_s32)detail->luma_gain_lut[i] * sw_global_gain / 256; /* 256 */
    }

    detail->proc_gain_hf = detail_dyna_cfg->gain_hf_pos;
    detail->proc_gain_lf = detail_dyna_cfg->gain_lf;
    detail->proc_global_gain = sw_global_gain;
}

static void hiisp_detail_fw(hi_u32 iso, hi_vi_pipe vi_pipe, hi_u8 cur_blk, isp_detail_dyna_cfg *detail_dyna_cfg,
                            isp_detail_usr_cfg *detail_usr_cfg)
{
    isp_detail *detail = HI_NULL;
    isp_detail_iso iso_struc;

    detail_get_ctx(vi_pipe, detail);

    iso_struc.iso = iso;
    iso_struc.iso_index_upper = get_iso_index(iso_struc.iso);
    iso_struc.iso_index_lower = MAX2((hi_s8)iso_struc.iso_index_upper - 1, 0);
    iso_struc.iso1 = get_iso(iso_struc.iso_index_lower);
    iso_struc.iso2 = get_iso(iso_struc.iso_index_upper);

    de_ext_cfg(vi_pipe, detail_dyna_cfg, detail_usr_cfg, detail, iso_struc);

    detail_dyna_cfg->resh = HI_TRUE;
    detail_usr_cfg->resh = HI_TRUE;
}

hi_s32 isp_detail_run(hi_vi_pipe vi_pipe, const hi_void *stat_info,
                      hi_void *reg_cfg, hi_s32 rsv)
{
    hi_u8 i;
    isp_reg_cfg *pt_reg_cfg = (isp_reg_cfg *)reg_cfg;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_detail *detail = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    detail_get_ctx(vi_pipe, detail);
    isp_check_pointer_return(detail);

    /* calculate every two interrupts */
    if (!detail->init) {
        return HI_SUCCESS;
    }

    for (i = 0; i < pt_reg_cfg->cfg_num; i++) {
        if (pt_reg_cfg->alg_reg_cfg[0].bnr_reg_cfg.bnr_enable == HI_TRUE) {
            detail->enable = hi_ext_system_detail_enable_read(vi_pipe);
            pt_reg_cfg->alg_reg_cfg[i].de_reg_cfg.de_enable = detail->enable;
        } else {
            detail->enable = HI_FALSE;
            pt_reg_cfg->alg_reg_cfg[i].de_reg_cfg.de_enable = HI_FALSE;
        }

        pt_reg_cfg->alg_reg_cfg[i].de_reg_cfg.kernel_reg_cfg.de_enable = \
                pt_reg_cfg->alg_reg_cfg[i].de_reg_cfg.de_enable;
    }

    pt_reg_cfg->cfg_key.bit1_detail_cfg = 1;

    /* check hardware setting */
    if (!check_de_open(detail)) {
        return HI_SUCCESS;
    }

    detail_read_extregs(vi_pipe);

    for (i = 0; i < isp_ctx->block_attr.block_num; i++) {
        hiisp_detail_fw(isp_ctx->linkage.iso, vi_pipe, i, &pt_reg_cfg->alg_reg_cfg[i].de_reg_cfg.dyna_reg_cfg,
                        &pt_reg_cfg->alg_reg_cfg[i].de_reg_cfg.usr_reg_cfg);
    }

    return HI_SUCCESS;
}

static hi_s32 isp_detail_ctrl(hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value)
{
    isp_detail *detail = HI_NULL;
    isp_reg_cfg_attr *reg_cfg = HI_NULL;

    switch (cmd) {
        case ISP_WDR_MODE_SET:
            isp_regcfg_get_ctx(vi_pipe, reg_cfg);
            detail_get_ctx(vi_pipe, detail);
            isp_check_pointer_return(reg_cfg);
            isp_check_pointer_return(detail);

            detail->init = HI_FALSE;
            isp_detail_init(vi_pipe, (hi_void *)&reg_cfg->reg_cfg);
            break;
        case ISP_PROC_WRITE:
            detail_proc_write(vi_pipe, (hi_isp_ctrl_proc_write *)value);
            break;
        default:
            break;
    }
    return HI_SUCCESS;
}

hi_s32 isp_detail_exit(hi_vi_pipe vi_pipe)
{
    hi_u8 i;
    isp_reg_cfg_attr *reg_cfg = HI_NULL;

    isp_regcfg_get_ctx(vi_pipe, reg_cfg);

    for (i = 0; i < reg_cfg->reg_cfg.cfg_num; i++) {
        reg_cfg->reg_cfg.alg_reg_cfg[i].de_reg_cfg.de_enable = HI_FALSE;
    }

    reg_cfg->reg_cfg.cfg_key.bit1_detail_cfg = 1;

    detail_ctx_exit(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_alg_register_detail(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_alg_node *algs = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_alg_check_return(isp_ctx->alg_key.bit1_detail);
    algs = isp_search_alg(isp_ctx->algs);
    isp_check_pointer_return(algs);

    algs->alg_type = ISP_ALG_DETAIL;
    algs->alg_func.pfn_alg_init = isp_detail_init;
    algs->alg_func.pfn_alg_run  = isp_detail_run;
    algs->alg_func.pfn_alg_ctrl = isp_detail_ctrl;
    algs->alg_func.pfn_alg_exit = isp_detail_exit;
    algs->used = HI_TRUE;

    return HI_SUCCESS;
}

