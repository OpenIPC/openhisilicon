/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_clut.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "mpi_sys.h"

#define HI_ISP_CLUT_GAIN        128
#define HI_ISP_CLUT_GAIN_MAX    4095
#define HI_ISP_CLUT_FRAME_CNT   2

static const  hi_u16 g_gain_r = HI_ISP_CLUT_GAIN;
static const  hi_u16 g_gain_g = HI_ISP_CLUT_GAIN;
static const  hi_u16 g_gain_b = HI_ISP_CLUT_GAIN;

typedef struct {
    hi_bool         clut_lut_update_en;
    hi_bool         clut_ctrl_update_en;
    hi_u32          *vir_addr;
    hi_isp_clut_attr clut_ctrl;
} isp_clut_ctx;

isp_clut_ctx g_clut_ctx[ISP_MAX_PIPE_NUM] = {{0}};

#define clut_get_ctx(dev, ctx)   ctx = &g_clut_ctx[dev]

static hi_void clut_ext_regs_initialize(hi_vi_pipe vi_pipe)
{
    isp_clut_ctx *clut_ctx = HI_NULL;
    clut_get_ctx(vi_pipe, clut_ctx);

    hi_ext_system_clut_en_write(vi_pipe, clut_ctx->clut_ctrl.enable);
    hi_ext_system_clut_gain_r_write(vi_pipe, (hi_u16)clut_ctx->clut_ctrl.gain_r);
    hi_ext_system_clut_gain_g_write(vi_pipe, (hi_u16)clut_ctx->clut_ctrl.gain_g);
    hi_ext_system_clut_gain_b_write(vi_pipe, (hi_u16)clut_ctx->clut_ctrl.gain_b);
    hi_ext_system_clut_ctrl_update_en_write(vi_pipe, HI_FALSE);
    hi_ext_system_clut_lut_update_en_write(vi_pipe, HI_FALSE);
}

static hi_void clut_usr_coef_regs_initialize(hi_vi_pipe vi_pipe, isp_clut_usr_coef_cfg *usr_coef_reg_cfg)
{
    hi_s32 ret;
    isp_clut_ctx  *clut_ctx  = HI_NULL;
    clut_get_ctx(vi_pipe, clut_ctx);

    ret = memcpy_s(usr_coef_reg_cfg->lu_stt, HI_ISP_CLUT_LUT_LENGTH * sizeof(hi_u32),
                   clut_ctx->vir_addr, HI_ISP_CLUT_LUT_LENGTH * sizeof(hi_u32));
    if (ret != 0) {
        isp_err_trace("ISP[%d] init clut lut failed!\n", vi_pipe);
    }

    usr_coef_reg_cfg->resh          = HI_TRUE;
    usr_coef_reg_cfg->buf_id        = 0;
    usr_coef_reg_cfg->update_index = 1;
}

static hi_void clut_usr_ctrl_regs_initialize(hi_vi_pipe vi_pipe, isp_clut_usr_ctrl_cfg *usr_ctrl_reg_cfg)
{
    usr_ctrl_reg_cfg->demo_mode      = HI_FALSE;
    usr_ctrl_reg_cfg->gain_r         = HI_ISP_CLUT_GAIN;
    usr_ctrl_reg_cfg->gain_b         = HI_ISP_CLUT_GAIN;
    usr_ctrl_reg_cfg->gain_g         = HI_ISP_CLUT_GAIN;
    usr_ctrl_reg_cfg->demo_enable    = HI_FALSE;
    usr_ctrl_reg_cfg->resh           = HI_TRUE;
}
static hi_void clut_usr_regs_initialize(hi_vi_pipe vi_pipe, isp_clut_usr_cfg *usr_reg_cfg)
{
    clut_usr_coef_regs_initialize(vi_pipe, &usr_reg_cfg->clut_usr_coef_cfg);
    clut_usr_ctrl_regs_initialize(vi_pipe, &usr_reg_cfg->clut_usr_ctrl_cfg);
}

hi_s32 clut_regs_initialize(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_bool offline_mode;
    hi_u8   i;
    isp_clut_ctx *clut_ctx = HI_NULL;
    isp_usr_ctx      *isp_ctx  = HI_NULL;

    clut_get_ctx(vi_pipe, clut_ctx);
    isp_get_ctx(vi_pipe, isp_ctx);

    offline_mode = (is_offline_mode(isp_ctx->block_attr.running_mode) ||
                    is_striping_mode(isp_ctx->block_attr.running_mode));

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        clut_usr_regs_initialize(vi_pipe, &reg_cfg->alg_reg_cfg[i].clut_cfg.usr_reg_cfg);
        reg_cfg->alg_reg_cfg[i].clut_cfg.enable = offline_mode ? (clut_ctx->clut_ctrl.enable) : (HI_FALSE);
    }

    reg_cfg->cfg_key.bit1_clut_cfg   = 1;

    return HI_SUCCESS;
}

static hi_s32 clut_read_extregs(hi_vi_pipe vi_pipe)
{
    isp_clut_ctx *clut_ctx = HI_NULL;

    clut_get_ctx(vi_pipe, clut_ctx);

    clut_ctx->clut_ctrl_update_en = hi_ext_system_clut_ctrl_update_en_read(vi_pipe);
    hi_ext_system_clut_ctrl_update_en_write(vi_pipe, HI_FALSE);

    if (clut_ctx->clut_ctrl_update_en) {
        clut_ctx->clut_ctrl.enable = hi_ext_system_clut_en_read(vi_pipe);
        clut_ctx->clut_ctrl.gain_r = hi_ext_system_clut_gain_r_read(vi_pipe);
        clut_ctx->clut_ctrl.gain_g = hi_ext_system_clut_gain_g_read(vi_pipe);
        clut_ctx->clut_ctrl.gain_b = hi_ext_system_clut_gain_b_read(vi_pipe);
    }

    clut_ctx->clut_lut_update_en = hi_ext_system_clut_lut_update_en_read(vi_pipe);
    hi_ext_system_clut_lut_update_en_write(vi_pipe, HI_FALSE);

    return HI_SUCCESS;
}

static hi_s32 clut_check_cmos_param(hi_vi_pipe vi_pipe, const hi_isp_cmos_clut *cmos_clut)
{
    isp_check_bool_return(cmos_clut->enable);

    if (cmos_clut->gain_r > HI_ISP_CLUT_GAIN_MAX) {
        isp_err_trace("Invalid gain_r!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (cmos_clut->gain_g > HI_ISP_CLUT_GAIN_MAX) {
        isp_err_trace("Invalid gain_g!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (cmos_clut->gain_b > HI_ISP_CLUT_GAIN_MAX) {
        isp_err_trace("Invalid gain_b!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

static hi_s32 clut_initialize(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    isp_mmz_buf_ex clut_buf;
    isp_clut_ctx  *clut_ctx = HI_NULL;
    hi_isp_cmos_default *sns_dft  = HI_NULL;

    clut_get_ctx(vi_pipe, clut_ctx);
    isp_sensor_get_default(vi_pipe, &sns_dft);

    clut_ctx->vir_addr = HI_NULL;

    if (ioctl(isp_get_fd(vi_pipe), ISP_CLUT_BUF_GET, &clut_buf.phy_addr) != HI_SUCCESS) {
        isp_err_trace("Get Clut Buffer Err\n");
        return HI_FAILURE;
    }

    clut_buf.vir_addr = HI_MPI_SYS_Mmap(clut_buf.phy_addr, HI_ISP_CLUT_LUT_LENGTH * sizeof(hi_u32));

    if (!clut_buf.vir_addr) {
        return HI_FAILURE;
    }

    clut_ctx->vir_addr = (hi_u32 *)clut_buf.vir_addr;

    if (sns_dft->key.bit1_clut) {
        isp_check_pointer_return(sns_dft->clut);

        ret = clut_check_cmos_param(vi_pipe, sns_dft->clut);
        if (ret != HI_SUCCESS) {
            return ret;
        }

        clut_ctx->clut_ctrl.enable = sns_dft->clut->enable;
        clut_ctx->clut_ctrl.gain_r = sns_dft->clut->gain_r;
        clut_ctx->clut_ctrl.gain_g = sns_dft->clut->gain_g;
        clut_ctx->clut_ctrl.gain_b = sns_dft->clut->gain_b;

        memcpy_s(clut_ctx->vir_addr, HI_ISP_CLUT_LUT_LENGTH * sizeof(hi_u32),
                 sns_dft->clut->clut_lut.lut, HI_ISP_CLUT_LUT_LENGTH * sizeof(hi_u32));
    } else {
        clut_ctx->clut_ctrl.enable = HI_FALSE;
        clut_ctx->clut_ctrl.gain_r = (hi_u32)g_gain_r;
        clut_ctx->clut_ctrl.gain_g = (hi_u32)g_gain_g;
        clut_ctx->clut_ctrl.gain_b = (hi_u32)g_gain_b;

        memset_s(clut_ctx->vir_addr, HI_ISP_CLUT_LUT_LENGTH * sizeof(hi_u32),
                 0, HI_ISP_CLUT_LUT_LENGTH * sizeof(hi_u32));
    }

    return HI_SUCCESS;
}

hi_void isp_clut_usr_coef_fw(isp_clut_ctx *clut_ctx, isp_clut_usr_coef_cfg *clut_usr_coef_cfg)
{
    hi_s32 ret;
    ret = memcpy_s(clut_usr_coef_cfg->lu_stt, HI_ISP_CLUT_LUT_LENGTH * sizeof(hi_u32),
                   clut_ctx->vir_addr, HI_ISP_CLUT_LUT_LENGTH * sizeof(hi_u32));
    if (ret != 0) {
        isp_err_trace("ISP update clut lut failed!\n");
    }

    clut_usr_coef_cfg->resh  = HI_TRUE;
    clut_usr_coef_cfg->update_index += 1;
}

hi_void isp_clut_usr_ctrl_fw(hi_isp_clut_attr *clut_ctrl, isp_clut_usr_ctrl_cfg *clut_usr_ctrl_cfg)
{
    clut_usr_ctrl_cfg->gain_r       = clut_ctrl->gain_r;
    clut_usr_ctrl_cfg->gain_g       = clut_ctrl->gain_g;
    clut_usr_ctrl_cfg->gain_b       = clut_ctrl->gain_b;
    clut_usr_ctrl_cfg->resh         = HI_TRUE;
}

static hi_bool __inline check_clut_open(isp_clut_ctx *clut_ctx)
{
    return (clut_ctx->clut_ctrl.enable == HI_TRUE);
}

static hi_s32 isp_clut_init(hi_vi_pipe vi_pipe, hi_void *reg_cfg_info)
{
    hi_s32 ret;
    isp_reg_cfg *reg_cfg = (isp_reg_cfg *)reg_cfg_info;

    ret = clut_initialize(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    ret = clut_regs_initialize(vi_pipe, reg_cfg);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    clut_ext_regs_initialize(vi_pipe);

    return HI_SUCCESS;
}

static hi_s32 isp_clut_run(hi_vi_pipe vi_pipe, const hi_void *stat_info,
                           hi_void *reg_cfg, hi_s32 rsv)
{
    hi_u8 i;
    isp_usr_ctx  *isp_ctx  = HI_NULL;
    isp_clut_ctx *clut_ctx = HI_NULL;
    isp_reg_cfg  *reg      = (isp_reg_cfg *)reg_cfg;
    hi_s32 ret;

    isp_get_ctx(vi_pipe, isp_ctx);
    clut_get_ctx(vi_pipe, clut_ctx);

    /* calculate every two interrupts */
    if ((isp_ctx->frame_cnt % HI_ISP_CLUT_FRAME_CNT != 0) && (isp_ctx->linkage.snap_state != HI_TRUE)) {
        return HI_SUCCESS;
    }

    clut_ctx->clut_ctrl.enable = hi_ext_system_clut_en_read(vi_pipe);

    for (i = 0; i < reg->cfg_num; i++) {
        reg->alg_reg_cfg[i].clut_cfg.enable = clut_ctx->clut_ctrl.enable;
    }

    reg->cfg_key.bit1_clut_cfg = 1;

    /* check hardware setting */
    if (!check_clut_open(clut_ctx)) {
        return HI_SUCCESS;
    }

    ret = clut_read_extregs(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    if (clut_ctx->clut_ctrl_update_en) {
        for (i = 0; i < reg->cfg_num; i++) {
            isp_clut_usr_ctrl_fw(&clut_ctx->clut_ctrl, &reg->alg_reg_cfg[i].clut_cfg.usr_reg_cfg.clut_usr_ctrl_cfg);
        }
    }

    if (clut_ctx->clut_lut_update_en) {
        for (i = 0; i < reg->cfg_num; i++) {
            if (!clut_ctx->vir_addr) {
                return HI_FAILURE;
            }

            isp_clut_usr_coef_fw(clut_ctx, &reg->alg_reg_cfg[i].clut_cfg.usr_reg_cfg.clut_usr_coef_cfg);
        }
    }

    return HI_SUCCESS;
}
static hi_s32 isp_clut_ctrl(hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value)
{
    return HI_SUCCESS;
}

static hi_s32 isp_clut_exit(hi_vi_pipe vi_pipe)
{
    hi_u8 i;
    isp_reg_cfg_attr *reg_cfg = HI_NULL;
    isp_clut_ctx *clut_ctx = HI_NULL;

    clut_get_ctx(vi_pipe, clut_ctx);

    isp_regcfg_get_ctx(vi_pipe, reg_cfg);

    for (i = 0; i < reg_cfg->reg_cfg.cfg_num; i++) {
        reg_cfg->reg_cfg.alg_reg_cfg[i].clut_cfg.enable = HI_FALSE;
    }

    reg_cfg->reg_cfg.cfg_key.bit1_clut_cfg = 1;

    if (clut_ctx->vir_addr != HI_NULL) {
        HI_MPI_SYS_Munmap((hi_void *)clut_ctx->vir_addr, HI_ISP_CLUT_LUT_LENGTH * sizeof(hi_u32));
    }

    return HI_SUCCESS;
}

hi_s32 isp_alg_register_clut(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_alg_node *algs = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_alg_check_return(isp_ctx->alg_key.bit1_clut);
    algs = isp_search_alg(isp_ctx->algs);
    isp_check_pointer_return(algs);

    algs->alg_type = ISP_ALG_CLUT;
    algs->alg_func.pfn_alg_init = isp_clut_init;
    algs->alg_func.pfn_alg_run  = isp_clut_run;
    algs->alg_func.pfn_alg_ctrl = isp_clut_ctrl;
    algs->alg_func.pfn_alg_exit = isp_clut_exit;
    algs->used = HI_TRUE;

    return HI_SUCCESS;
}

