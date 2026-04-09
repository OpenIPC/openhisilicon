/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_green_equalization.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include <math.h>
#include "isp_alg.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_sensor.h"
#include "isp_math_utils.h"
#include "isp_proc.h"

#define HI_ISP_GE_MAX_BLK_NUM_X    17
#define HI_ISP_GE_MAX_BLK_NUM_Y    15
#define HI_MINIISP_BITDEPTH        14
#define HI_MAX_1_SLOPE             12
#define ISP_GE_SLOPE_DEFAULT       9
#define ISP_GE_SENSI_SLOPE_DEFAULT 9
#define ISP_GE_SENSI_THR_DEFAULT   300

static const  hi_u16 g_threshold[ISP_AUTO_ISO_STRENGTH_NUM] = {
    300,  300,  300,  300,  310,  310,  310,  310,  320,  320,  320,  320,  330,  330,  330,  330
};
static const  hi_u16 g_strength[ISP_AUTO_ISO_STRENGTH_NUM]  = {
    128,  128,  128,  128,  129,  129,  129,  129,  130,  130,  130,  130,  131,  131,  131,  131
};
static const  hi_u16 g_np_offset[ISP_AUTO_ISO_STRENGTH_NUM]  = {
    1024, 1024, 1024, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048
};

typedef struct {
    hi_bool enable;
    hi_bool coef_update_en;
    hi_u8   ge_chn_num;

    hi_u16  actual_threshold;  /* Range: [0, 0x3FFF] */
    hi_u16  actual_strength;   /* Range: [0, 0x100]  */
    hi_u16  actual_np_offset;  /* Range: [0x200, 0x3FFF] */

    hi_s32  grgb_w;
    hi_s32  grgb_h;
    hi_u32  bit_depth;
    hi_isp_cmos_ge cmos_ge;
} isp_ge;

isp_ge *g_ge_ctx[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define ge_get_ctx(dev, ctx)   ((ctx) = g_ge_ctx[dev])
#define ge_set_ctx(dev, ctx)   (g_ge_ctx[dev] = (ctx))
#define ge_reset_ctx(dev)         (g_ge_ctx[dev] = HI_NULL)

static hi_s32 ge_ctx_init(hi_vi_pipe vi_pipe)
{
    isp_ge *ge_ctx = HI_NULL;

    ge_get_ctx(vi_pipe, ge_ctx);

    if (ge_ctx == HI_NULL) {
        ge_ctx = (isp_ge *)isp_malloc(sizeof(isp_ge));
        if (ge_ctx == HI_NULL) {
            isp_err_trace("isp[%d] ge_ctx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }
    }

    memset_s(ge_ctx, sizeof(isp_ge), 0, sizeof(isp_ge));

    ge_set_ctx(vi_pipe, ge_ctx);

    return HI_SUCCESS;
}

static hi_void ge_ctx_exit(hi_vi_pipe vi_pipe)
{
    isp_ge *ge_ctx = HI_NULL;

    ge_get_ctx(vi_pipe, ge_ctx);
    isp_free(ge_ctx);
    ge_reset_ctx(vi_pipe);
}

static hi_s32 ge_check_cmos_param(hi_vi_pipe vi_pipe, const hi_isp_cmos_ge *cmos_ge)
{
    hi_u8 i;

    isp_check_bool_return(cmos_ge->enable);

    if (cmos_ge->slope > 0xE) {
        isp_err_trace("invalid slope!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (cmos_ge->sensi_slope > 0xE) {
        isp_err_trace("invalid sensi_slope!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (cmos_ge->sensi_thr > 0x3FFF) {
        isp_err_trace("invalid sensi_thr!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        if (cmos_ge->strength[i] > 0x100) {
            isp_err_trace("invalid strength[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (cmos_ge->np_offset[i] > 0x3FFF || cmos_ge->np_offset[i] < 0x200) {
            isp_err_trace("invalid np_offset[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (cmos_ge->threshold[i] > 0x3FFF) {
            isp_err_trace("invalid threshold[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    return HI_SUCCESS;
}

static hi_s32 ge_initialize(hi_vi_pipe vi_pipe)
{
    hi_s32     ret;
    isp_ge   *ge = HI_NULL;
    hi_isp_cmos_default  *sns_dft = HI_NULL;

    ge_get_ctx(vi_pipe, ge);
    isp_sensor_get_default(vi_pipe, &sns_dft);

    if (sns_dft->key.bit1_ge) {
        isp_check_pointer_return(sns_dft->ge);

        ret = ge_check_cmos_param(vi_pipe, sns_dft->ge);
        if (ret != HI_SUCCESS) {
            return ret;
        }

        memcpy_s(&ge->cmos_ge,  sizeof(hi_isp_cmos_ge), sns_dft->ge, sizeof(hi_isp_cmos_ge));
    } else {
        ge->cmos_ge.enable      = HI_TRUE;
        ge->cmos_ge.slope       = ISP_GE_SLOPE_DEFAULT;
        ge->cmos_ge.sensi_slope = ISP_GE_SENSI_SLOPE_DEFAULT;
        ge->cmos_ge.sensi_thr   = ISP_GE_SENSI_THR_DEFAULT;
        memcpy_s(ge->cmos_ge.strength, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16),
                 g_strength,  ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16));
        memcpy_s(ge->cmos_ge.threshold, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16),
                 g_threshold, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16));
        memcpy_s(ge->cmos_ge.np_offset, ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16),
                 g_np_offset,  ISP_AUTO_ISO_STRENGTH_NUM * sizeof(hi_u16));
    }

    ge->enable = ge->cmos_ge.enable;

    ge->grgb_w     = HI_ISP_GE_MAX_BLK_NUM_X;
    ge->grgb_h     = HI_ISP_GE_MAX_BLK_NUM_Y;

    ge->bit_depth   = HI_MINIISP_BITDEPTH;

    return HI_SUCCESS;
}

static hi_void ge_ext_regs_initialize(hi_vi_pipe vi_pipe)
{
    hi_u32 i;
    isp_ge *ge = HI_NULL;

    ge_get_ctx(vi_pipe, ge);

    /* initial ext register of ge */
    hi_ext_system_ge_enable_write(vi_pipe, ge->enable);
    hi_ext_system_ge_slope_write(vi_pipe, ge->cmos_ge.slope);
    hi_ext_system_ge_sensitivity_write(vi_pipe, ge->cmos_ge.sensi_slope);
    hi_ext_system_ge_sensithreshold_write(vi_pipe, ge->cmos_ge.sensi_thr);
    hi_ext_system_ge_coef_update_en_write(vi_pipe, HI_TRUE);

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
        hi_ext_system_ge_strength_write(vi_pipe, i, ge->cmos_ge.strength[i]);
        hi_ext_system_ge_npoffset_write(vi_pipe, i, ge->cmos_ge.np_offset[i]);
        hi_ext_system_ge_threshold_write(vi_pipe, i, ge->cmos_ge.threshold[i]);
    }
}

static hi_u8 ge_get_chn_num(hi_u8 wdr_mode)
{
    hi_u8 chn_num;

    if (is_linear_mode(wdr_mode)) {
        chn_num = 0x1;
    } else if (is_built_in_wdr_mode(wdr_mode)) {
        chn_num = 0x1;
    } else if (is_2to1_wdr_mode(wdr_mode)) {
        chn_num = 0x2;
    } else {
        /* unknow mode */
        chn_num = 0x1;
    }

    return chn_num;
}

static hi_void ge_static_regs_initialize(hi_vi_pipe vi_pipe, isp_ge_static_cfg *ge_static_reg_cfg)
{
    isp_ge   *ge = HI_NULL;

    ge_get_ctx(vi_pipe, ge);

    ge_static_reg_cfg->ge_gr_gb_en = HI_TRUE;
    ge_static_reg_cfg->ge_gr_en    = HI_FALSE;
    ge_static_reg_cfg->ge_gb_en    = HI_FALSE;
    ge_static_reg_cfg->ge_num_v    = ge->grgb_h;
    ge_static_reg_cfg->static_resh = HI_TRUE;
}

static hi_void ge_image_size(hi_vi_pipe vi_pipe, hi_u8 i, isp_ge_usr_cfg *usr_reg_cfg, isp_usr_ctx *isp_ctx)
{
    hi_u8    blk_num = isp_ctx->block_attr.block_num;
    hi_u16   overlap;
    isp_rect block_rect;
    isp_ge   *ge = HI_NULL;

    ge_get_ctx(vi_pipe, ge);
    isp_check_pointer_void_return(ge);

    overlap = isp_ctx->block_attr.over_lap;
    if (i < (ge->grgb_w % div_0_to_1(blk_num))) {
        usr_reg_cfg->ge_num_h = ge->grgb_w / div_0_to_1(blk_num) + 1;
    } else {
        usr_reg_cfg->ge_num_h = ge->grgb_w / div_0_to_1(blk_num);
    }

    isp_get_block_rect(&block_rect, &isp_ctx->block_attr, i);

    usr_reg_cfg->ge_crop_pos_y      = 0;
    usr_reg_cfg->ge_crop_out_height = block_rect.height;

    if (i == 0) {
        if (blk_num > 1) {
            usr_reg_cfg->ge_crop_pos_x      = 0;
            usr_reg_cfg->ge_crop_out_width  = block_rect.width - overlap;
        } else {
            usr_reg_cfg->ge_crop_pos_x      = 0;
            usr_reg_cfg->ge_crop_out_width  = block_rect.width;
        }
    } else if (i == (blk_num - 1)) {
        usr_reg_cfg->ge_crop_pos_x      = overlap;
        usr_reg_cfg->ge_crop_out_width  = block_rect.width - overlap;
    } else {
        usr_reg_cfg->ge_crop_pos_x      = overlap;
        usr_reg_cfg->ge_crop_out_width  = block_rect.width - (overlap << 1);
    }
}

static hi_void ge_usr_regs_initialize(hi_vi_pipe vi_pipe, hi_u8 i, hi_u8 chn_num, isp_ge_usr_cfg *ge_usr_reg_cfg,
                                      isp_usr_ctx *isp_ctx)
{
    hi_u8 j;

    for (j = 0; j < chn_num; j++) {
        ge_usr_reg_cfg->ge_ct_th2[j]    = HI_ISP_GE_SENSITHR_DEFAULT;
        ge_usr_reg_cfg->ge_ct_slope1[j] = HI_ISP_GE_SENSISLOPE_DEFAULT;
        ge_usr_reg_cfg->ge_ct_slope2[j] = HI_ISP_GE_SLOPE_DEFAULT;
    }

    ge_image_size(vi_pipe, i, ge_usr_reg_cfg, isp_ctx);
    ge_usr_reg_cfg->resh = HI_TRUE;
}

static hi_void ge_dyna_regs_initialize(hi_u8 chn_num, isp_ge_dyna_cfg *ge_dyna_reg_cfg)
{
    hi_u8 i;

    for (i = 0; i < chn_num; i++) {
        ge_dyna_reg_cfg->ge_ct_th1[i] = HI_ISP_GE_NPOFFSET_DEFAULT;
        ge_dyna_reg_cfg->ge_ct_th3[i] = HI_ISP_GE_THRESHOLD_DEFAULT;
    }
    ge_dyna_reg_cfg->ge_strength = HI_ISP_GE_STRENGTH_DEFAULT;
    ge_dyna_reg_cfg->resh        = HI_TRUE;
}

static hi_void ge_regs_initialize(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_u8 i, j;
    isp_usr_ctx  *isp_ctx = HI_NULL;
    isp_ge *ge = HI_NULL;
    hi_u8 ge_chn_num;

    ge_get_ctx(vi_pipe, ge);
    isp_get_ctx(vi_pipe, isp_ctx);

    ge_chn_num = ge_get_chn_num(isp_ctx->sns_wdr_mode);
    ge->ge_chn_num = ge_chn_num;

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        ge_static_regs_initialize(vi_pipe, &reg_cfg->alg_reg_cfg[i].ge_reg_cfg.static_reg_cfg);
        ge_usr_regs_initialize(vi_pipe, i, ge_chn_num, &reg_cfg->alg_reg_cfg[i].ge_reg_cfg.usr_reg_cfg, isp_ctx);
        ge_dyna_regs_initialize(ge_chn_num, &reg_cfg->alg_reg_cfg[i].ge_reg_cfg.dyna_reg_cfg);
        reg_cfg->alg_reg_cfg[i].ge_reg_cfg.chn_num = ge_chn_num;

        for (j = 0; j < ge_chn_num; j++) {
            reg_cfg->alg_reg_cfg[i].ge_reg_cfg.ge_en[j] = ge->enable;
        }

        for (j = ge_chn_num; j < 0x4; j++) {
            reg_cfg->alg_reg_cfg[i].ge_reg_cfg.ge_en[j] = HI_FALSE;
        }
    }

    reg_cfg->cfg_key.bit1_ge_cfg = 1;
}

static hi_void ge_read_extregs(hi_vi_pipe vi_pipe)
{
    hi_u32 i;
    isp_ge *ge = HI_NULL;

    ge_get_ctx(vi_pipe, ge);

    /* read ext register of ge */
    ge->coef_update_en         = hi_ext_system_ge_coef_update_en_read(vi_pipe);
    hi_ext_system_ge_coef_update_en_write(vi_pipe, HI_FALSE);

    if (ge->coef_update_en) {
        ge->cmos_ge.slope       = hi_ext_system_ge_slope_read(vi_pipe);
        ge->cmos_ge.sensi_slope = hi_ext_system_ge_sensitivity_read(vi_pipe);
        ge->cmos_ge.sensi_thr   = hi_ext_system_ge_sensithreshold_read(vi_pipe);

        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++) {
            ge->cmos_ge.strength[i]  = hi_ext_system_ge_strength_read(vi_pipe, i);
            ge->cmos_ge.np_offset[i]  = hi_ext_system_ge_npoffset_read(vi_pipe, i);
            ge->cmos_ge.threshold[i] = hi_ext_system_ge_threshold_read(vi_pipe, i);
        }
    }

    return;
}

static hi_u8 ge_slope_clip(hi_u8 slope, hi_u8 chn)
{
    if (chn == 0) {
        return MIN2(slope, HI_MINIISP_BITDEPTH);
    } else {
        return MIN2(slope, HI_MAX_1_SLOPE);
    }
}

static hi_void ge_usr_fw(isp_ge *ge, isp_ge_usr_cfg *ge_usr_reg_cfg)
{
    hi_u8 j;

    for (j = 0; j < ge->ge_chn_num; j++) {
        ge_usr_reg_cfg->ge_ct_th2[j]   = MIN2(ge->cmos_ge.sensi_thr, (1 << ge->bit_depth));
        ge_usr_reg_cfg->ge_ct_slope1[j] = ge_slope_clip(ge->cmos_ge.sensi_slope, j);
        ge_usr_reg_cfg->ge_ct_slope2[j] = ge_slope_clip(ge->cmos_ge.slope, j);
    }

    ge_usr_reg_cfg->resh    = HI_TRUE;
}

static hi_void ge_dyna_fw(hi_u32 iso, isp_ge *ge, isp_ge_dyna_cfg *ge_dyna_reg_cfg)
{
    hi_u8 i;
    hi_u8 index_upper = get_iso_index(iso);
    hi_u8 index_lower = MAX2((hi_s8)index_upper - 1, 0);
    hi_u32 iso_low  = get_iso(index_lower);
    hi_u32 iso_high = get_iso(index_upper);

    for (i = 0; i < ge->ge_chn_num; i++) {
        ge_dyna_reg_cfg->ge_ct_th3[i] = CLIP3(linear_inter(iso,
            iso_low, ge->cmos_ge.threshold[index_lower],
            iso_high, ge->cmos_ge.threshold[index_upper]), 0, (1 << ge->bit_depth));

        ge_dyna_reg_cfg->ge_ct_th1[i] = (hi_u16)linear_inter(iso,
            iso_low, ge->cmos_ge.np_offset[index_lower],
            iso_high, ge->cmos_ge.np_offset[index_upper]);
    }

    ge_dyna_reg_cfg->ge_strength  = (hi_u16)linear_inter(iso, iso_low, ge->cmos_ge.strength[index_lower],
                                                         iso_high, ge->cmos_ge.strength[index_upper]);
    ge->actual_threshold  = ge_dyna_reg_cfg->ge_ct_th3[0];
    ge->actual_np_offset  = ge_dyna_reg_cfg->ge_ct_th1[0];
    ge->actual_strength   = ge_dyna_reg_cfg->ge_strength;
    ge_dyna_reg_cfg->resh = HI_TRUE;
}

static hi_bool __inline check_ge_open(isp_ge *ge)
{
    return (ge->enable == HI_TRUE);
}

static hi_void ge_bypass(isp_reg_cfg *reg, isp_ge *ge)
{
    hi_u8 i, j;

    for (i = 0; i < reg->cfg_num; i++) {
        for (j = 0; j < ge->ge_chn_num; j++) {
            reg->alg_reg_cfg[i].ge_reg_cfg.ge_en[j] = HI_FALSE;
        }
    }

    reg->cfg_key.bit1_ge_cfg = 1;
}

static hi_s32 ge_image_res_write(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_u8 i;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);

    for (i = 0; i < isp_ctx->block_attr.block_num; i++) {
        ge_image_size(vi_pipe, i, &reg_cfg->alg_reg_cfg[i].ge_reg_cfg.usr_reg_cfg, isp_ctx);
        reg_cfg->alg_reg_cfg[i].ge_reg_cfg.usr_reg_cfg.resh = HI_TRUE;
    }

    reg_cfg->cfg_key.bit1_ge_cfg = 1;

    return HI_SUCCESS;
}

hi_s32 isp_ge_init(hi_vi_pipe vi_pipe, hi_void *reg_cfg_in)
{
    hi_s32 ret;
    isp_reg_cfg *reg_cfg = (isp_reg_cfg *)reg_cfg_in;

    ret = ge_ctx_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    ret = ge_initialize(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    ge_regs_initialize(vi_pipe, reg_cfg);
    ge_ext_regs_initialize(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_ge_run(hi_vi_pipe vi_pipe, const hi_void *stat_info,
                  hi_void *reg_cfg, hi_s32 rsv)
{
    hi_u8 i, j;
    isp_ge  *ge = HI_NULL;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_reg_cfg *reg = (isp_reg_cfg *)reg_cfg;

    isp_get_ctx(vi_pipe, isp_ctx);
    ge_get_ctx(vi_pipe, ge);
    isp_check_pointer_return(ge);

    if ((isp_ctx->frame_cnt % 0x2 != 0) && (isp_ctx->linkage.snap_state != HI_TRUE)) {
        return HI_SUCCESS;
    }

    if (isp_ctx->linkage.defect_pixel) {
        ge_bypass(reg, ge);
        return HI_SUCCESS;
    }

    ge->enable = hi_ext_system_ge_enable_read(vi_pipe);

    for (i = 0; i < reg->cfg_num; i++) {
        for (j = 0; j < ge->ge_chn_num; j++) {
            reg->alg_reg_cfg[i].ge_reg_cfg.ge_en[j] = ge->enable;
        }

        for (j = ge->ge_chn_num; j < 0x4; j++) {
            reg->alg_reg_cfg[i].ge_reg_cfg.ge_en[j] = HI_FALSE;
        }
    }

    reg->cfg_key.bit1_ge_cfg = 1;

    /* check hardware setting */
    if (!check_ge_open(ge)) {
        return HI_SUCCESS;
    }

    ge_read_extregs(vi_pipe);

    if (ge->coef_update_en) {
        for (i = 0; i < reg->cfg_num; i++) {
            ge_usr_fw(ge, &reg->alg_reg_cfg[i].ge_reg_cfg.usr_reg_cfg);
        }
    }

    for (i = 0; i < reg->cfg_num; i++) {
        ge_dyna_fw(isp_ctx->linkage.iso, ge, &reg->alg_reg_cfg[i].ge_reg_cfg.dyna_reg_cfg);
    }

    return HI_SUCCESS;
}

static hi_void ge_proc_write(hi_vi_pipe vi_pipe, hi_isp_ctrl_proc_write *proc)
{
    hi_isp_ctrl_proc_write proc_tmp;
    isp_ge  *ge_ctx = HI_NULL;

    ge_get_ctx(vi_pipe, ge_ctx);
    isp_check_pointer_void_return(ge_ctx);

    if ((proc->proc_buff == HI_NULL) || (proc->buff_len == 0)) {
        return;
    }

    proc_tmp.proc_buff = proc->proc_buff;
    proc_tmp.buff_len  = proc->buff_len;

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "-----CrossTalk INFO-------------------------------------------------------------\n");

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "%12s" "%12s" "%12s" "%12s\n", "Enable", "NpOffset", "Threshold", "Strength");

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "%12u" "%12u" "%12u" "%12u\n",
                    ge_ctx->enable, ge_ctx->actual_np_offset, ge_ctx->actual_threshold, ge_ctx->actual_strength);

    proc->write_len += 1;
}

hi_s32 isp_ge_ctrl(hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value)
{
    isp_reg_cfg_attr *reg_cfg = HI_NULL;

    switch (cmd) {
        case ISP_WDR_MODE_SET:
            isp_regcfg_get_ctx(vi_pipe, reg_cfg);
            isp_check_pointer_return(reg_cfg);
            isp_ge_init(vi_pipe, (hi_void *)&reg_cfg->reg_cfg);
            break;
        case ISP_CHANGE_IMAGE_MODE_SET:
            isp_regcfg_get_ctx(vi_pipe, reg_cfg);
            isp_check_pointer_return(reg_cfg);
            ge_image_res_write(vi_pipe, &reg_cfg->reg_cfg);
            break;
        case ISP_PROC_WRITE:
            ge_proc_write(vi_pipe, (hi_isp_ctrl_proc_write *)value);
            break;
        default:
            break;
    }
    return HI_SUCCESS;
}

hi_s32 isp_ge_exit(hi_vi_pipe vi_pipe)
{
    hi_u8 i;
    isp_reg_cfg_attr *reg_cfg = HI_NULL;

    isp_regcfg_get_ctx(vi_pipe, reg_cfg);

    for (i = 0; i < reg_cfg->reg_cfg.cfg_num; i++) {
        reg_cfg->reg_cfg.alg_reg_cfg[i].ge_reg_cfg.ge_en[0] = HI_FALSE; /* [0] */
        reg_cfg->reg_cfg.alg_reg_cfg[i].ge_reg_cfg.ge_en[1] = HI_FALSE; /* [1] */
        reg_cfg->reg_cfg.alg_reg_cfg[i].ge_reg_cfg.ge_en[2] = HI_FALSE; /* [2] */
        reg_cfg->reg_cfg.alg_reg_cfg[i].ge_reg_cfg.ge_en[3] = HI_FALSE; /* [3] */
    }

    reg_cfg->reg_cfg.cfg_key.bit1_ge_cfg = 1;

    ge_ctx_exit(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_alg_register_ge(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_alg_node *algs = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_alg_check_return(isp_ctx->alg_key.bit1_ge);
    algs = isp_search_alg(isp_ctx->algs);
    isp_check_pointer_return(algs);

    algs->alg_type = ISP_ALG_GE;
    algs->alg_func.pfn_alg_init = isp_ge_init;
    algs->alg_func.pfn_alg_run  = isp_ge_run;
    algs->alg_func.pfn_alg_ctrl = isp_ge_ctrl;
    algs->alg_func.pfn_alg_exit = isp_ge_exit;
    algs->used = HI_TRUE;

    return HI_SUCCESS;
}

