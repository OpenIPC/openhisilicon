/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_fpn.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_config.h"
#include "isp_alg.h"
#include "isp_ext_config.h"

#include "hi_comm_vi.h"
#include "mpi_vi.h"
#include "vi_ext.h"
#include "isp_proc.h"

/* MACRO DEFINITION */
#define FPN_OVERFLOWTHR                     0x7C0
#define ISP_FPN_MAX_O                       0xFFF
#define FPN_OVERFLOWTHR_OFF                 0x3FFF
#define FPN_CHN_NUM                         2
#define ISP_FPN_MODE_CORRECTION             0x0
#define ISP_FPN_MODE_CALIBRATE              0x1

#define isp_fpn_clip(min, max, x)             ((x) <= (min) ? (min) : ((x) > (max) ? (max) : (x)))

typedef enum {
    ISP_SENSOR_8BIT  = 8,
    ISP_SENSOR_10BIT = 10,
    ISP_SENSOR_12BIT = 12,
    ISP_SENSOR_14BIT = 14,
    ISP_SENSOR_16BIT = 16,
    ISP_SENSOR_32BIT = 32,
    ISP_SENSOR_BUTT
} isp_sensor_bit_width;

hi_s32  isp_set_calibrate_attr(hi_vi_pipe vi_pipe, hi_isp_fpn_calibrate_attr *calibrate)
{
    vi_fpn_attr fpn_attr;
    hi_s32 ret;
    hi_u8 wdr_mode;

    wdr_mode = hi_ext_system_sensor_wdr_mode_read(vi_pipe);
    if (is_wdr_mode(wdr_mode)) {
        isp_err_trace("do not support FPN calibration in WDR mode!\n");
        return HI_FAILURE;
    }

    fpn_attr.fpn_work_mode = FPN_MODE_CALIBRATE;
    memcpy_s(&fpn_attr.calibrate_attr, sizeof(hi_isp_fpn_calibrate_attr),
             calibrate, sizeof(hi_isp_fpn_calibrate_attr));

    hi_ext_system_fpn_cablibrate_enable_write(vi_pipe, 1);

    ret = mpi_vi_set_fpn_attr(vi_pipe, &fpn_attr);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    memcpy_s(calibrate, sizeof(hi_isp_fpn_calibrate_attr),
             &fpn_attr.calibrate_attr, sizeof(hi_isp_fpn_calibrate_attr));
    calibrate->fpn_cali_frame.iso = hi_ext_system_fpn_sensor_iso_read(vi_pipe);
    hi_ext_system_fpn_cablibrate_enable_write(vi_pipe, 0);

    return HI_SUCCESS;
}

hi_u32 isp_fpn_get_strength(hi_u32 iso, hi_u32 calibrate_iso)
{
    hi_u32 strength;

    strength = 0x100 * iso / div_0_to_1(calibrate_iso);

    return strength;
}

hi_s32 isp_set_correction_attr(hi_vi_pipe vi_pipe, const hi_isp_fpn_attr *correction)
{
    hi_s32 ret;
    hi_u8 wdr_mode;
    vi_fpn_attr fpn_attr;

    wdr_mode = hi_ext_system_sensor_wdr_mode_read(vi_pipe);
    if (is_wdr_mode(wdr_mode)) {
        isp_err_trace("do not support FPN correction in WDR mode!\n");
        return HI_FAILURE;
    }

    fpn_attr.fpn_work_mode = FPN_MODE_CORRECTION;
    memcpy_s(&fpn_attr.correction_attr, sizeof(hi_isp_fpn_attr), correction, sizeof(hi_isp_fpn_attr));

    ret = mpi_vi_set_fpn_attr(vi_pipe, &fpn_attr);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    hi_ext_system_manual_fpn_opmode_write(vi_pipe, correction->op_type);
    hi_ext_system_manual_fpn_enable_write(vi_pipe, correction->enable);
    hi_ext_system_manual_fpn_iso_write(vi_pipe, correction->fpn_frm_info.iso);
    hi_ext_system_manual_fpn_gain_write(vi_pipe, correction->manual_attr.strength);

    return ret;
}

hi_s32 isp_get_correction_attr(hi_vi_pipe vi_pipe, hi_isp_fpn_attr *correction)
{
    hi_u8 blk_dev = 0;
    hi_u8 index = 0;
    hi_s32 ret;
    vi_fpn_attr temp_vi_fpn_attr;

    ret = mpi_vi_get_fpn_attr(vi_pipe, &temp_vi_fpn_attr);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    memcpy_s(correction, sizeof(hi_isp_fpn_attr),
             &temp_vi_fpn_attr.correction_attr, sizeof(hi_isp_fpn_attr));
    correction->auto_attr.strength = (isp_fpn_corr_gainoffset_read(vi_pipe, blk_dev, index) >> 0x10);

    return HI_SUCCESS;
}

static hi_void fpn_ext_regs_default(hi_vi_pipe vi_pipe)
{
    hi_ext_system_fpn_sensor_iso_write(vi_pipe, HI_EXT_SYSTEM_FPN_SENSOR_ISO_DEFAULT);
    hi_ext_system_manual_fpn_iso_write(vi_pipe, HI_EXT_SYSTEM_FPN_MANU_ISO_DEFAULT);

    hi_ext_system_manual_fpn_corr_cfg_write(vi_pipe, HI_EXT_SYSTEM_FPN_MANU_CORRCFG_DEFAULT);
    hi_ext_system_manual_fpn_gain_write(vi_pipe, HI_EXT_SYSTEM_FPN_STRENGTH_DEFAULT);
    hi_ext_system_manual_fpn_opmode_write(vi_pipe, HI_EXT_SYSTEM_FPN_OPMODE_DEFAULT);
    hi_ext_system_manual_fpn_update_write(vi_pipe, HI_EXT_SYSTEM_FPN_MANU_UPDATE_DEFAULT);

    hi_ext_system_manual_fpn_type_write(vi_pipe, 0);
    hi_ext_system_manual_fpn_offset_write(vi_pipe, 0);
    hi_ext_system_manual_fpn_enable_write(vi_pipe, 0);
    hi_ext_system_manual_fpn_pixel_format_write(vi_pipe, 0);
    hi_ext_system_fpn_cablibrate_enable_write(vi_pipe, 0);

    return;
}

static hi_void fpn_regs_default(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    reg_cfg->cfg_key.bit1_fpn_cfg = 1;

    return;
}

static hi_void fpn_ext_regs_initialize(hi_vi_pipe vi_pipe)
{
    return;
}

static hi_void fpn_regs_initialize(hi_vi_pipe vi_pipe)
{
    return;
}

static hi_s32 fpn_read_extregs(hi_vi_pipe vi_pipe)
{
    return 0;
}

hi_void fpn_update_ext_regs(hi_vi_pipe vi_pipe)
{
    hi_u8 i = 0;
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_reg_cfg_attr *reg_cfg = HI_NULL;
    hi_u8 fpn_op_mode;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_regcfg_get_ctx(vi_pipe, reg_cfg);

    hi_ext_system_fpn_sensor_iso_write(vi_pipe, isp_ctx->linkage.sensor_iso);
    fpn_op_mode = hi_ext_system_manual_fpn_opmode_read(vi_pipe);

    for (i = 0; i < reg_cfg->reg_cfg.cfg_num; i++) {
        if (hi_ext_system_fpn_cablibrate_enable_read(vi_pipe)) {
            reg_cfg->reg_cfg.alg_reg_cfg[i].fpn_reg_cfg.dyna_reg_cfg.isp_fpn_calib_corr = ISP_FPN_MODE_CALIBRATE;
        } else {
            reg_cfg->reg_cfg.alg_reg_cfg[i].fpn_reg_cfg.dyna_reg_cfg.isp_fpn_calib_corr = ISP_FPN_MODE_CORRECTION;
        }

        if (is_wdr_mode(isp_ctx->pre_sns_wdr_mode) && (fpn_op_mode == OP_TYPE_AUTO)) {
            reg_cfg->reg_cfg.alg_reg_cfg[i].fpn_reg_cfg.dyna_reg_cfg.isp_fpn_overflow_thr = FPN_OVERFLOWTHR;
        } else {
            reg_cfg->reg_cfg.alg_reg_cfg[i].fpn_reg_cfg.dyna_reg_cfg.isp_fpn_overflow_thr = FPN_OVERFLOWTHR_OFF;
        }
    }

    return;
}

hi_void isp_set_strength(hi_vi_pipe vi_pipe)
{
    hi_u32  iso, gain, i;
    hi_u32  calibrate_iso;
    hi_u8   fpn_op_mode, fpn_en;
    isp_reg_cfg_attr *reg_cfg = HI_NULL;

    isp_regcfg_get_ctx(vi_pipe, reg_cfg);

    fpn_en = isp_ext_system_manual_fpn_enable_read(vi_pipe);
    if (!fpn_en) {
        return;
    }

    fpn_op_mode = hi_ext_system_manual_fpn_opmode_read(vi_pipe);
    if (fpn_op_mode == OP_TYPE_MANUAL) {
        for (i = 0; i < FPN_CHN_NUM; i++) {
            reg_cfg->reg_cfg.alg_reg_cfg[0].fpn_reg_cfg.dyna_reg_cfg.isp_fpn_strength[i] =
                isp_ext_system_manual_fpn_gain_read(vi_pipe);
        }
        reg_cfg->reg_cfg.cfg_key.bit1_fpn_cfg = 1;
        return;
    }

    iso = hi_ext_system_fpn_sensor_iso_read(vi_pipe);
    calibrate_iso = hi_ext_system_manual_fpn_iso_read(vi_pipe);
    gain = isp_fpn_get_strength(iso, calibrate_iso);
    gain = isp_fpn_clip(0, 0x3FF, gain);

    for (i = 0; i < FPN_CHN_NUM; i++) {
        reg_cfg->reg_cfg.alg_reg_cfg[0].fpn_reg_cfg.dyna_reg_cfg.isp_fpn_strength[i] = gain;
    }

    reg_cfg->reg_cfg.cfg_key.bit1_fpn_cfg = 1;
}

hi_s32 isp_fpn_init(hi_vi_pipe vi_pipe, hi_void *reg_cfg)
{
    fpn_regs_default(vi_pipe, (isp_reg_cfg *)reg_cfg);
    fpn_ext_regs_default(vi_pipe);
    fpn_read_extregs(vi_pipe);
    fpn_regs_initialize(vi_pipe);
    fpn_ext_regs_initialize(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_fpn_run(hi_vi_pipe vi_pipe, const hi_void *stat_info, hi_void *reg_cfg, hi_s32 rsv)
{
    fpn_update_ext_regs(vi_pipe);
    isp_set_strength(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 fpn_proc_write(hi_vi_pipe vi_pipe, hi_isp_ctrl_proc_write *proc)
{
    hi_u8 blk_dev = 0; /* BE num, for SBS mode, BE0 & BE1 is the same FPN config, so just 0 is ok */
    hi_isp_ctrl_proc_write proc_tmp;
    hi_u32 offset;
    hi_u32 strength;

    if ((proc->proc_buff == HI_NULL) ||
        (proc->buff_len == 0)) {
        return HI_FAILURE;
    }

    proc_tmp.proc_buff = proc->proc_buff;
    proc_tmp.buff_len = proc->buff_len;

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "-----FPN CORRECT INFO------------------------------------------------------------\n");

    isp_proc_printf(&proc_tmp, proc->write_len,
                    "%8s" "%7s"     "%9s"       "%8s",
                    "En", "OpType", "Strength", "Offset\n");

    offset = isp_fpn_corr_gainoffset_read(vi_pipe, blk_dev, 0) & 0xfff;
    strength = (isp_fpn_corr_gainoffset_read(vi_pipe, blk_dev, 0) >> 0x10) & 0xffff;

    if (isp_ext_system_manual_fpn_enable_read(vi_pipe)) {
        isp_proc_printf(&proc_tmp, proc->write_len,
                        "%8d" "%4d"  "%9d"  "%8d",
                        isp_ext_system_manual_fpn_enable_read(vi_pipe),
                        hi_ext_system_manual_fpn_opmode_read(vi_pipe),
                        strength,
                        offset);
    } else {
        isp_proc_printf(&proc_tmp, proc->write_len,
                        "%8d" "%4s"  "%9s"  "%8s\n",
                        isp_fpn_en_read(vi_pipe, blk_dev),
                        "--",
                        "--",
                        "--");
    }

    proc->write_len += 1;

    return HI_SUCCESS;
}

hi_s32 isp_fpn_ctrl(hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value)
{
    switch (cmd) {
        case ISP_PROC_WRITE:
            fpn_proc_write(vi_pipe, (hi_isp_ctrl_proc_write *)value);
            break;

        default:
            break;
    }
    return HI_SUCCESS;
}

hi_s32 isp_fpn_exit(hi_vi_pipe vi_pipe)
{
    return HI_SUCCESS;
}

hi_s32 isp_alg_register_fpn(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_alg_node *algs = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_alg_check_return(isp_ctx->alg_key.bit1_fpn);
    algs = isp_search_alg(isp_ctx->algs);
    isp_check_pointer_return(algs);

    algs->alg_type = ISP_ALG_FPN;
    algs->alg_func.pfn_alg_init = isp_fpn_init;
    algs->alg_func.pfn_alg_run  = isp_fpn_run;
    algs->alg_func.pfn_alg_ctrl = isp_fpn_ctrl;
    algs->alg_func.pfn_alg_exit = isp_fpn_exit;
    algs->used = HI_TRUE;

    return HI_SUCCESS;
}

