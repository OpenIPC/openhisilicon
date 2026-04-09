/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_alg.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __ISP_ALG_H__
#define __ISP_ALG_H__

#include <stdlib.h>
#include <string.h>
#include "hi_common.h"
#include "hi_comm_isp.h"
#include "isp_main.h"
#include "hi_math.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

hi_s32 isp_alg_register_ae(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_awb(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_af(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_dpc(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_ge(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_frame_wdr(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_bayer_nr(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_flicker(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_hrs(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_dg(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_fe_lsc(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_lsc(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_rlsc(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_acs(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_rc(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_drc(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_dehaze(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_lcac(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_gcac(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_split(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_expander(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_demosaic(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_fcr(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_gamma(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_csc(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_sharpen(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_edge_mark(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_mcds(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_dci(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_ldci(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_ca(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_fpn(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_pre_gamma(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_blc(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_fe_log_lut(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_log_lut(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_clut(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_hlc(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_detail(hi_vi_pipe vi_pipe);
hi_s32 isp_alg_register_rgbir(hi_vi_pipe vi_pipe);

hi_s32 isp_find_lib(const isp_lib_node *lib_node, const hi_isp_alg_lib *lib);
isp_lib_node *isp_search_lib(isp_lib_node *lib_node);
isp_alg_node *isp_search_alg(isp_alg_node *algs);

hi_void isp_algs_init(const isp_alg_node *algs, hi_vi_pipe vi_pipe, hi_void *reg_cfg);
hi_void isp_algs_run(const isp_alg_node *algs, hi_vi_pipe vi_pipe,
                     const hi_void *stat_info, hi_void *reg_cfg_info, hi_s32 rsv);
hi_void isp_algs_ctrl(const isp_alg_node *algs, hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value);
hi_void isp_algs_exit(hi_vi_pipe vi_pipe, const isp_alg_node *algs);

hi_void isp_algs_register(hi_vi_pipe vi_pipe);
hi_void isp_yuv_algs_register(hi_vi_pipe vi_pipe);

hi_void isp_algs_unregister(hi_vi_pipe vi_pipe);
hi_void isp_libs_unregister(hi_vi_pipe vi_pipe);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
