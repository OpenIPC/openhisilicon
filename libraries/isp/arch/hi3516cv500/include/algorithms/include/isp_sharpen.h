/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_sharpen.h
 * Author: ISP SW
 * Create: 2012/06/28
 */
#ifndef __ISP_SHARPEN_H__
#define __ISP_SHARPEN_H__

#include "isp_config.h"
#include "hi_isp_debug.h"
#include "isp_ext_config.h"
#include "isp_math_utils.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct {
    hi_bool sharpen_mpi_update_en;
    hi_u32  iso_last;
    hi_u32  iso;
    hi_u8   sft;
    hi_u32  wgt_pre;
    hi_u32  wgt_cur;
    hi_s32  idx_cur;
    hi_s32  idx_pre;

    /* sharpening yuv */
    /* tmp registers */
    hi_u8   mf_thd_sft_d;
    hi_u8   dir_var_sft;
    hi_u8   range_thd0;       /* U8.0 */
    hi_u8   range_dir_wgt0;    /* U0.7 */

    hi_u8   sel_pix_wgt;
    hi_u8   rmf_gain_scale;
    hi_u8   bmf_gain_scale;

    hi_u8   mf_thd_sel_ud;
    hi_u8   mf_thd_sft_ud;
    hi_u8   hf_eps_var_thr0;
    hi_u8   hf_eps_var_thr1;
    hi_u8   hf_eps_val1;

    hi_u8   osht_var_wgt0;
    hi_u8   usht_var_wgt0;
    hi_u8   osht_var_diff_thd0;
    hi_u8   osht_var_diff_thd1;
    hi_u8   osht_var_diff_wgt1;
    hi_u8   usht_var_diff_wgt1;

    hi_isp_sharpen_manual_attr actual; /* actual param */

    /* MPI */
    hi_isp_sharpen_attr mpi_cfg;  /* param read from mpi */
} isp_sharpen_ctx;

hi_s32 isp_sharpen_param_init(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg, isp_sharpen_ctx *shp_ctx);

hi_void isp_sharpen_read_extregs(hi_vi_pipe vi_pipe, isp_sharpen_ctx *shp_ctx);
hi_void isp_sharpen_read_pro_mode(hi_vi_pipe vi_pipe, isp_sharpen_ctx *shp_ctx);
hi_void isp_sharpen_reg_update(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg, isp_sharpen_ctx *shp_ctx);
hi_s32  sharpen_proc_write(hi_vi_pipe vi_pipe, hi_isp_ctrl_proc_write *proc, isp_sharpen_ctx *shp_ctx);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
