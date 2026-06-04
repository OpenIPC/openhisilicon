/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_PARAM_CHECK_HAL_H
#define ISP_PARAM_CHECK_HAL_H
#include "ot_type.h"
#include "ot_common_isp.h"
#include "mkp_isp.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

td_s32 isp_sharpen_common_attr_arch_check(const char *src, const ot_isp_sharpen_attr *sharpen_attr);
td_s32 isp_sharpen_manual_attr_arch_check(const char *src, const ot_isp_sharpen_manual_attr *manual_attr);
td_s32 isp_sharpen_auto_attr_arch_check(const char *src, const ot_isp_sharpen_auto_attr *auto_attr);
td_s32 isp_drc_attr_arch_check(const char *src, const ot_isp_drc_attr *drc_attr);
td_s32 isp_ca_attr_arch_check(const char *src, const ot_isp_ca_attr *ca_attr);
td_s32 isp_nr_attr_arch_check(const char *src, const ot_isp_nr_attr *nr_attr);
td_s32 isp_nr_snr_attr_arch_check(const char *src, const ot_isp_nr_snr_attr *snr_cfg);
td_s32 isp_demosaic_manual_attr_arch_check(const char *src, const ot_isp_demosaic_manual_attr *manual_attr);
td_s32 isp_demosaic_auto_attr_arch_check(const char *src, const ot_isp_demosaic_auto_attr *auto_attr);
td_s32 isp_cac_acac_manual_attr_arch_check(const char *src, const ot_isp_cac_acac_manual_attr *acac_attr);
td_s32 isp_cac_acac_auto_attr_arch_check(const char *src, const ot_isp_cac_acac_auto_attr *acac_attr);
td_s32 isp_expander_attr_arch_check(const char *src, const ot_isp_expander_attr *expander_attr);
td_s32 isp_motion_stats_attr_arch_check(ot_vi_pipe vi_pipe, const ot_isp_motion_stats_cfg *motion_cfg);
#ifdef CONFIG_OT_VI_PIPE_FPN
td_s32 isp_fpn_correction_attr_arch_check(ot_vi_pipe vi_pipe, const ot_isp_fpn_attr *correction);
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif
