/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_dgain.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_ext_config.h"

typedef struct {
    hi_bool enable;
} isp_dg;

isp_dg g_dg_ctx[ISP_MAX_PIPE_NUM] = {{0}};
#define dg_get_ctx(dev, ctx)   ctx = &g_dg_ctx[dev]
#define DG_CHANNEL_NUM 4
#define DG_ISP_DGAIN_SHIFT 8

static hi_void be_dg_static_init(hi_vi_pipe vi_vipe, isp_dg_static_cfg *static_reg_cfg)
{
    static_reg_cfg->resh = HI_TRUE;
}

static hi_void be_dg_dyna_init(hi_vi_pipe vi_vipe, isp_dg_dyna_cfg *dyna_reg_cfg, hi_u16 gain)
{
    dyna_reg_cfg->clip_value = 0xFFFFF;
    dyna_reg_cfg->gain_r     = gain;
    dyna_reg_cfg->gain_gr    = gain;
    dyna_reg_cfg->gain_gb    = gain;
    dyna_reg_cfg->gain_b     = gain;
    dyna_reg_cfg->resh       = HI_TRUE;
}

static hi_void wdr_dg_static_init(hi_vi_pipe vi_vipe, hi_u8 i, isp_4dg_static_cfg *static_reg_cfg)
{
    static_reg_cfg->gain_r0  = 0x100;
    static_reg_cfg->gain_gr0 = 0x100;
    static_reg_cfg->gain_gb0 = 0x100;
    static_reg_cfg->gain_b0  = 0x100;
    static_reg_cfg->gain_r1  = 0x100;
    static_reg_cfg->gain_gr1 = 0x100;
    static_reg_cfg->gain_gb1 = 0x100;
    static_reg_cfg->gain_b1  = 0x100;
    static_reg_cfg->gain_r2  = 0x100;
    static_reg_cfg->gain_gr2 = 0x100;
    static_reg_cfg->gain_gb2 = 0x100;
    static_reg_cfg->gain_b2  = 0x100;
    static_reg_cfg->gain_r3  = 0x100;
    static_reg_cfg->gain_gr3 = 0x100;
    static_reg_cfg->gain_gb3 = 0x100;
    static_reg_cfg->gain_b3  = 0x100;
    static_reg_cfg->resh     = HI_TRUE;
}

static hi_void wdr_dg_dyna_init(hi_vi_pipe vi_vipe, isp_4dg_dyna_cfg *dyna_reg_cfg)
{
    dyna_reg_cfg->clip_value0 = 0xFFFFF;
    dyna_reg_cfg->clip_value1 = 0xFFFFF;
    dyna_reg_cfg->clip_value2 = 0xFFFFF;
    dyna_reg_cfg->clip_value3 = 0xFFFFF;
    dyna_reg_cfg->resh = HI_TRUE;
}

static hi_void fe_dg_dyna_init(hi_vi_pipe vi_vipe, isp_fe_dg_dyna_cfg *dyna_reg_cfg, hi_u16 gain)
{
    hi_u8 i = 0;

    for (i = 0; i < DG_CHANNEL_NUM; i++) {
        dyna_reg_cfg->gain_r[i]  = gain;
        dyna_reg_cfg->gain_gr[i] = gain;
        dyna_reg_cfg->gain_gb[i] = gain;
        dyna_reg_cfg->gain_b[i]  = gain;
    }
    dyna_reg_cfg->clip_value  = 0xFFFFF;
    dyna_reg_cfg->resh        = HI_TRUE;
}

static hi_void wdr_dg_en_init(hi_vi_pipe vi_vipe, hi_u8 i, isp_reg_cfg *reg_cfg)
{
    hi_u8 wdr_mode;
    isp_usr_ctx *isp_ctx = HI_NULL;

    isp_get_ctx(vi_vipe, isp_ctx);

    wdr_mode = isp_ctx->sns_wdr_mode;

    if (is_linear_mode(wdr_mode)) {
        reg_cfg->alg_reg_cfg[i].four_dg_reg_cfg.enable = HI_FALSE;
    } else if (is_2to1_wdr_mode(wdr_mode) || is_3to1_wdr_mode(wdr_mode) || is_4to1_wdr_mode(wdr_mode)) {
        reg_cfg->alg_reg_cfg[i].four_dg_reg_cfg.enable = HI_TRUE;
    } else if (is_built_in_wdr_mode(wdr_mode)) {
        reg_cfg->alg_reg_cfg[i].four_dg_reg_cfg.enable = HI_FALSE;
    }
}

static hi_void dg_regs_initialize(hi_vi_pipe vi_vipe, isp_reg_cfg *reg_cfg)
{
    hi_u8  i;
    hi_u16 gain;
    hi_isp_cmos_black_level *sns_black_level = HI_NULL;

    isp_sensor_get_blc(vi_vipe, &sns_black_level);

    gain = CLIP3(0xFFF * 0x100 / div_0_to_1(0xFFF - sns_black_level->black_level[1]) + 1, 0x100, 0x200);

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        be_dg_static_init(vi_vipe, &reg_cfg->alg_reg_cfg[i].dg_reg_cfg.static_reg_cfg);
        be_dg_dyna_init(vi_vipe, &reg_cfg->alg_reg_cfg[i].dg_reg_cfg.dyna_reg_cfg, gain);

        wdr_dg_static_init(vi_vipe, i, &reg_cfg->alg_reg_cfg[i].four_dg_reg_cfg.static_reg_cfg);
        wdr_dg_dyna_init(vi_vipe, &reg_cfg->alg_reg_cfg[i].four_dg_reg_cfg.dyna_reg_cfg);

        wdr_dg_en_init(vi_vipe, i, reg_cfg);

        reg_cfg->alg_reg_cfg[i].dg_reg_cfg.dg_en = HI_TRUE;
        reg_cfg->cfg_key.bit1_dg_cfg = 1;
        reg_cfg->cfg_key.bit1_wdr_dg_cfg = 1;
    }

    fe_dg_dyna_init(vi_vipe, &reg_cfg->alg_reg_cfg[0].fe_dg_reg_cfg.dyna_reg_cfg, gain);
    reg_cfg->alg_reg_cfg[0].fe_dg_reg_cfg.dg_en = HI_TRUE;
    reg_cfg->cfg_key.bit1_fe_dg_cfg                 = 1;
}

static hi_void dg_ext_regs_initialize(hi_vi_pipe vi_vipe)
{
    hi_ext_system_isp_dgain_enable_write(vi_vipe, HI_TRUE);
}

static hi_void dg_initialize(hi_vi_pipe vi_vipe)
{
    isp_dg *dg = HI_NULL;

    dg_get_ctx(vi_vipe, dg);

    dg->enable = HI_TRUE;
}

static hi_void isp_dg_wdr_mode_set(hi_vi_pipe vi_vipe, hi_void *reg_cfg_input)
{
    hi_u8  i, j;
    hi_u16 gain;
    isp_reg_cfg *reg_cfg = (isp_reg_cfg *)reg_cfg_input;
    hi_isp_cmos_black_level *sns_black_level = HI_NULL;

    isp_sensor_get_blc(vi_vipe, &sns_black_level);

    gain = CLIP3(0xFFF * 0x100 / div_0_to_1(0xFFF - sns_black_level->black_level[1]) + 1, 0x100, 0x200);

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        wdr_dg_en_init(vi_vipe, i, reg_cfg);
        reg_cfg->alg_reg_cfg[i].dg_reg_cfg.static_reg_cfg.resh       = HI_TRUE;
        reg_cfg->alg_reg_cfg[i].four_dg_reg_cfg.static_reg_cfg.resh  = HI_TRUE;

        reg_cfg->alg_reg_cfg[i].dg_reg_cfg.dyna_reg_cfg.gain_r  = gain;
        reg_cfg->alg_reg_cfg[i].dg_reg_cfg.dyna_reg_cfg.gain_gr = gain;
        reg_cfg->alg_reg_cfg[i].dg_reg_cfg.dyna_reg_cfg.gain_gb = gain;
        reg_cfg->alg_reg_cfg[i].dg_reg_cfg.dyna_reg_cfg.gain_b  = gain;
    }

    reg_cfg->cfg_key.bit1_wdr_dg_cfg = 1;
    reg_cfg->cfg_key.bit1_fe_dg_cfg  = 1;
    reg_cfg->cfg_key.bit1_dg_cfg     = 1;

    for (j = 0; j < DG_CHANNEL_NUM; j++) {
        reg_cfg->alg_reg_cfg[0].fe_dg_reg_cfg.dyna_reg_cfg.gain_r[j]  = gain;
        reg_cfg->alg_reg_cfg[0].fe_dg_reg_cfg.dyna_reg_cfg.gain_gr[j] = gain;
        reg_cfg->alg_reg_cfg[0].fe_dg_reg_cfg.dyna_reg_cfg.gain_gb[j] = gain;
        reg_cfg->alg_reg_cfg[0].fe_dg_reg_cfg.dyna_reg_cfg.gain_b[j]  = gain;
    }
    reg_cfg->alg_reg_cfg[0].fe_dg_reg_cfg.dyna_reg_cfg.resh = HI_TRUE;
}

hi_s32 isp_dg_init(hi_vi_pipe vi_vipe, hi_void *reg_cfg_input)
{
    isp_reg_cfg *reg_cfg = (isp_reg_cfg *)reg_cfg_input;

    dg_regs_initialize(vi_vipe, reg_cfg);
    dg_ext_regs_initialize(vi_vipe);
    dg_initialize(vi_vipe);

    return HI_SUCCESS;
}

static hi_bool __inline check_dg_open(isp_dg *dg)
{
    return (dg->enable == HI_TRUE);
}

hi_s32 isp_dg_run(hi_vi_pipe vi_vipe, const hi_void *stat_info,
                  hi_void *reg_cfg_input, hi_s32 rsv)
{
    hi_s32 i;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_dg *dg = HI_NULL;
    isp_reg_cfg *reg_cfg = (isp_reg_cfg *)reg_cfg_input;

    isp_get_ctx(vi_vipe, isp_ctx);
    dg_get_ctx(vi_vipe, dg);

    if (isp_ctx->linkage.defect_pixel &&
        (hi_ext_system_dpc_static_defect_type_read(vi_vipe) == 0)) {
        for (i = 0; i < reg_cfg->cfg_num; i++) {
            reg_cfg->alg_reg_cfg[i].dg_reg_cfg.dg_en = HI_FALSE;
        }

        reg_cfg->cfg_key.bit1_dg_cfg   = 1;

        return HI_SUCCESS;
    }

    dg->enable = hi_ext_system_isp_dgain_enable_read(vi_vipe);

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        reg_cfg->alg_reg_cfg[i].dg_reg_cfg.dg_en = dg->enable;
    }

    reg_cfg->alg_reg_cfg[0].fe_dg_reg_cfg.dg_en = dg->enable;

    reg_cfg->cfg_key.bit1_fe_dg_cfg = 1;
    reg_cfg->cfg_key.bit1_dg_cfg   = 1;

    /* check hardware setting */
    if (!check_dg_open(dg)) {
        return HI_SUCCESS;
    }

    return HI_SUCCESS;
}

hi_s32 isp_dg_ctrl(hi_vi_pipe vi_vipe, hi_u32 cmd, hi_void *value)
{
    isp_reg_cfg_attr *reg_cfg = HI_NULL;

    switch (cmd) {
        case ISP_CHANGE_IMAGE_MODE_SET:
        case ISP_WDR_MODE_SET:
            isp_regcfg_get_ctx(vi_vipe, reg_cfg);
            isp_check_pointer_return(reg_cfg);
            isp_dg_wdr_mode_set(vi_vipe, (hi_void *)&reg_cfg->reg_cfg);
            break;

        default:
            break;
    }

    return HI_SUCCESS;
}

hi_s32 isp_dg_exit(hi_vi_pipe vi_vipe)
{
    return HI_SUCCESS;
}

hi_s32 isp_alg_register_dg(hi_vi_pipe vi_vipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_alg_node *algs = HI_NULL;

    isp_get_ctx(vi_vipe, isp_ctx);
    isp_alg_check_return(isp_ctx->alg_key.bit1_dg);
    algs = isp_search_alg(isp_ctx->algs);
    isp_check_pointer_return(algs);

    algs->alg_type = ISP_ALG_DG;
    algs->alg_func.pfn_alg_init = isp_dg_init;
    algs->alg_func.pfn_alg_run  = isp_dg_run;
    algs->alg_func.pfn_alg_ctrl = isp_dg_ctrl;
    algs->alg_func.pfn_alg_exit = isp_dg_exit;
    algs->used = HI_TRUE;

    return HI_SUCCESS;
}
