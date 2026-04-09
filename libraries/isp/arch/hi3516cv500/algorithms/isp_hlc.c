/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_hlc.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_proc.h"

typedef struct {
    hi_bool hlc_mpi_update_en;
    hi_bool enable;                        /* RW; Range:[0, 1]; Format:1.0;Enable/Disable Edge Mark */
    hi_u8   luma_thr;                      /* RW; range: [0, 255];  Format:8.0; */
    hi_u8   luma_target;                   /* RW; range: [0, 255];  Format:8.0; */
} isp_hlc;

isp_hlc g_hlc_ctx[ISP_MAX_PIPE_NUM] = {{0}};
#define hi_hlc_get_ctx(dev, ctx)   ctx = &g_hlc_ctx[dev]

static hi_void hlc_ext_regs_initialize(hi_vi_pipe vi_pipe)
{
    isp_hlc *hlc = HI_NULL;

    hi_hlc_get_ctx(vi_pipe, hlc);

    hi_ext_system_hlc_mpi_update_en_write(vi_pipe, HI_FALSE);
    hi_ext_system_manual_hlc_en_write(vi_pipe, hlc->enable);
    hi_ext_system_manual_hlc_lumathr_write(vi_pipe, hlc->luma_thr);
    hi_ext_system_manual_hlc_lumatarget_write(vi_pipe, hlc->luma_target);
}

static hi_void hlc_check_reg(isp_hlc_reg_cfg *hlc_reg)
{
    hlc_reg->enable = MIN2(hlc_reg->enable, 0x1);
}

static hi_void hlc_reg_init(isp_hlc_reg_cfg *hlc_reg, isp_hlc *hlc)
{
    hlc_reg->y_max_value      = hlc->luma_thr;
    hlc_reg->y_max_load_value = hlc->luma_target;
    hlc_reg->y_min_value      = 0;
    hlc_reg->y_min_load_value = 0;
    hlc_reg->c_max_value      = 0xFF;
    hlc_reg->c_max_load_value = 0xFF;
    hlc_reg->c_min_value      = 0;
    hlc_reg->c_min_load_value = 0;
    hlc_check_reg(hlc_reg);
}

static hi_void hlc_regs_initialize(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_u32 i;
    isp_hlc *hlc = HI_NULL;

    hi_hlc_get_ctx(vi_pipe, hlc);

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        reg_cfg->alg_reg_cfg[i].hlc_reg_cfg.enable = hlc->enable;
        hlc_reg_init(&reg_cfg->alg_reg_cfg[i].hlc_reg_cfg, hlc);
    }

    reg_cfg->cfg_key.bit1_hlc_cfg = 1;
}

static hi_void hlc_read_ext_regs(hi_vi_pipe vi_pipe)
{
    isp_hlc *hlc = HI_NULL;

    hi_hlc_get_ctx(vi_pipe, hlc);
    hlc->hlc_mpi_update_en = hi_ext_system_hlc_mpi_update_en_read(vi_pipe);
    hi_ext_system_hlc_mpi_update_en_write(vi_pipe, HI_FALSE);

    if (hlc->hlc_mpi_update_en) {
        hlc->luma_thr    = hi_ext_system_manual_hlc_lumathr_read(vi_pipe);
        hlc->luma_target = hi_ext_system_manual_hlc_lumatarget_read(vi_pipe);
    }
}

static hi_s32 hlc_check_cmos_param(hi_vi_pipe vi_pipe, const hi_isp_cmos_hlc *cmos_hlc)
{
    isp_check_bool_return(cmos_hlc->enable);

    return HI_SUCCESS;
}

static hi_s32 hlc_initialize(hi_vi_pipe vi_pipe)
{
    hi_s32 ret;
    isp_hlc *hlc = HI_NULL;
    hi_isp_cmos_default *sns_dft = HI_NULL;

    hi_hlc_get_ctx(vi_pipe, hlc);
    isp_sensor_get_default(vi_pipe, &sns_dft);

    if (sns_dft->key.bit1_hlc) {
        isp_check_pointer_return(sns_dft->hlc);

        ret = hlc_check_cmos_param(vi_pipe, sns_dft->hlc);
        if (ret != HI_SUCCESS) {
            return ret;
        }

        hlc->enable = sns_dft->hlc->enable;
        hlc->luma_thr = sns_dft->hlc->luma_thr;
        hlc->luma_target = sns_dft->hlc->luma_target;
    } else {
        hlc->enable      = HI_FALSE;
        hlc->luma_thr    = 248; /* experience value 248 */
        hlc->luma_target = 10; /* experience value 10 */
    }

    return HI_SUCCESS;
}

hi_s32 isp_hlc_init(hi_vi_pipe vi_pipe, hi_void *reg_cfg)
{
    hi_s32 ret;

    ret = hlc_initialize(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    hlc_regs_initialize(vi_pipe, (isp_reg_cfg *)reg_cfg);
    hlc_ext_regs_initialize(vi_pipe);

    return HI_SUCCESS;
}

static void hlc_mpi2reg(isp_hlc_reg_cfg *hlc_reg_cfg, isp_hlc *hlc)
{
    hlc_reg_cfg->y_max_value      = hlc->luma_thr;
    hlc_reg_cfg->y_max_load_value = hlc->luma_target;

    hlc_check_reg(hlc_reg_cfg);
}

static hi_bool __inline hlc_check_open(isp_hlc *hlc)
{
    return (hlc->enable == HI_TRUE);
}

hi_s32 hlc_proc_write(hi_vi_pipe vi_pipe, hi_isp_ctrl_proc_write *proc)
{
    hi_isp_ctrl_proc_write proc_tmp;

    isp_hlc *hlc = HI_NULL;

    hi_hlc_get_ctx(vi_pipe, hlc);

    if ((proc->proc_buff == HI_NULL) || (proc->buff_len == 0)) {
        return HI_FAILURE;
    }

    proc_tmp.proc_buff = proc->proc_buff;
    proc_tmp.buff_len  = proc->buff_len;

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "-----HLC INFO--------------------------------------------------------------\n");

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "%16s"    "%16s"        "%16s\n",
                    "Enable", "LumaThr", "LumaTarget");

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "%16u"  "%16u"      "%16u\n",
                    (hi_u16)hlc->enable,
                    (hi_u16)hlc->luma_thr,
                    (hi_u16)hlc->luma_target);

    proc->write_len += 1;

    return HI_SUCCESS;
}

hi_s32 isp_hlc_run(hi_vi_pipe vi_pipe, const hi_void *stat_info, hi_void *reg_cfg, hi_s32 rsv)
{
    hi_u8  i;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_hlc *hlc = HI_NULL;
    isp_reg_cfg *reg_cfg_2 = (isp_reg_cfg *)reg_cfg;

    isp_get_ctx(vi_pipe, isp_ctx);
    hi_hlc_get_ctx(vi_pipe, hlc);

    if (isp_ctx->linkage.defect_pixel) {
        return HI_SUCCESS;
    }
    if ((isp_ctx->frame_cnt % 2 != 0) && (isp_ctx->linkage.snap_state != HI_TRUE)) { /* every 2 interrupts */
        return HI_SUCCESS;
    }

    hlc->enable = hi_ext_system_manual_hlc_en_read(vi_pipe);

    for (i = 0; i < reg_cfg_2->cfg_num; i++) {
        reg_cfg_2->alg_reg_cfg[i].hlc_reg_cfg.enable = hlc->enable;
    }

    reg_cfg_2->cfg_key.bit1_hlc_cfg = 1;

    /* check hardware setting */
    if (!hlc_check_open(hlc)) {
        return HI_SUCCESS;
    }

    hlc_read_ext_regs(vi_pipe);

    if (hlc->hlc_mpi_update_en) {
        for (i = 0; i < reg_cfg_2->cfg_num; i++) {
            hlc_mpi2reg(&(reg_cfg_2->alg_reg_cfg[i].hlc_reg_cfg), hlc);
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_hlc_ctrl(hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value)
{
    switch (cmd) {
        case ISP_PROC_WRITE:
            hlc_proc_write(vi_pipe, (hi_isp_ctrl_proc_write *)value);
            break;

        default:
            break;
    }

    return HI_SUCCESS;
}

hi_s32 isp_hlc_exit(hi_vi_pipe vi_pipe)
{
    hi_u8 i;
    isp_reg_cfg_attr *reg_cfg = HI_NULL;

    isp_regcfg_get_ctx(vi_pipe, reg_cfg);

    for (i = 0; i < reg_cfg->reg_cfg.cfg_num; i++) {
        reg_cfg->reg_cfg.alg_reg_cfg[i].hlc_reg_cfg.enable = HI_FALSE;
    }

    reg_cfg->reg_cfg.cfg_key.bit1_hlc_cfg = 1;

    return HI_SUCCESS;
}

hi_s32 isp_alg_register_hlc(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_alg_node *algs = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_alg_check_return(isp_ctx->alg_key.bit1_hlc);
    algs = isp_search_alg(isp_ctx->algs);
    isp_check_pointer_return(algs);

    algs->alg_type =  ISP_ALG_EDGEAMRK;
    algs->alg_func.pfn_alg_init = isp_hlc_init;
    algs->alg_func.pfn_alg_run  = isp_hlc_run;
    algs->alg_func.pfn_alg_ctrl = isp_hlc_ctrl;
    algs->alg_func.pfn_alg_exit = isp_hlc_exit;
    algs->used = HI_TRUE;

    return HI_SUCCESS;
}
