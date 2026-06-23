/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_ALG_H
#define ISP_ALG_H

#include <stdlib.h>
#include <string.h>
#include "ot_common.h"
#include "ot_common_isp.h"
#include "isp_main.h"
#include "ot_math.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

td_s32 isp_alg_register_ae(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_awb(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_af(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_dpc(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_ge(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_frame_wdr(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_bayer_nr(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_flicker(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_hrs(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_dg(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_lsc(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_acs(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_rc(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_drc(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_dehaze(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_cac(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_bayershp(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_expander(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_demosaic(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_fcr(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_gamma(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_csc(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_sharpen(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_mcds(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_ldci(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_ca(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_fpn(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_pregamma(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_blc(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_clut(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_rgbir(ot_vi_pipe vi_pipe);
td_s32 isp_alg_register_lblc(ot_vi_pipe vi_pipe);

td_s32 isp_find_lib(const isp_lib_node *lib_node, const ot_isp_3a_alg_lib *lib);
isp_lib_node *isp_search_lib(isp_lib_node *lib_node);
isp_alg_node *isp_search_alg(isp_alg_node *algs);

td_void isp_algs_init(const isp_alg_node *algs, ot_vi_pipe vi_pipe, td_void *reg_cfg);
td_void isp_algs_run(const isp_alg_node *algs, ot_vi_pipe vi_pipe,
                     const td_void *stat_info, td_void *reg_cfg_info, td_s32 rsv);
td_void isp_algs_ctrl(const isp_alg_node *algs, ot_vi_pipe vi_pipe, td_u32 cmd, td_void *value);
td_void isp_algs_exit(ot_vi_pipe vi_pipe, const isp_alg_node *algs);

td_void isp_algs_register(ot_vi_pipe vi_pipe);
td_void isp_yuv_algs_register(ot_vi_pipe vi_pipe);
td_void isp_fe_algs_register(ot_vi_pipe vi_pipe);

td_void isp_algs_unregister(ot_vi_pipe vi_pipe);
td_void isp_libs_unregister(ot_vi_pipe vi_pipe);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
