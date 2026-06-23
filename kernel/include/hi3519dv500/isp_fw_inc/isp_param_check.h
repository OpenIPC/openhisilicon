/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_PARAM_CHECK_H
#define ISP_PARAM_CHECK_H


#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include "ot_type.h"
#include "ot_common_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

td_s32 isp_sharpen_comm_attr_check(const char *src, const ot_isp_sharpen_attr *shp_attr);
td_s32 isp_sharpen_auto_attr_check(const char *src, const ot_isp_sharpen_auto_attr *auto_attr);
td_s32 isp_sharpen_manual_attr_check(const char *src, const ot_isp_sharpen_manual_attr *manual_attr);

td_s32 isp_drc_attr_check(const char *src, const ot_isp_drc_attr *drc_attr);
#ifdef CONFIG_OT_ISP_PREGAMMA_SUPPORT
td_s32 isp_pregamma_attr_check(const char *src, const ot_isp_pregamma_attr *pregamma_attr);
#endif
td_s32 isp_bshp_comm_attr_check(const char *src, const ot_isp_bayershp_attr *bshp_attr);
td_s32 isp_bshp_auto_attr_check(const char *src, const ot_isp_bayershp_auto_attr *auto_attr);
td_s32 isp_bshp_manual_attr_check(const char *src, const ot_isp_bayershp_manual_attr *manual_attr);

td_s32 isp_anti_false_color_attr_check(const char *src, const ot_isp_anti_false_color_attr *anti_false_color);
td_s32 isp_demosaic_attr_check(const char *src, const ot_isp_demosaic_attr *dem_attr);
td_s32 isp_cac_comm_attr_check(const char *src, const ot_isp_cac_attr *cac_attr);
td_s32 isp_cac_acac_attr_check(const char *src, const ot_isp_cac_acac_attr *acac_cfg);
td_s32 isp_cac_lcac_attr_check(const char *src, const ot_isp_cac_lcac_attr *lcac_cfg);
#ifdef CONFIG_OT_ISP_RGBIR_SUPPORT
td_s32 isp_rgbir_attr_check(const char *src, ot_vi_pipe vi_pipe, const ot_isp_rgbir_attr *rgbir_attr);
#endif
td_s32 isp_nr_comm_attr_check(const char *src, const ot_isp_nr_attr *nr_attr);
td_s32 isp_nr_snr_attr_check(const char *src, const ot_isp_nr_snr_attr *snr_cfg);
td_s32 isp_nr_tnr_attr_check(const char *src, const ot_isp_nr_tnr_attr *tnr_cfg);
td_s32 isp_nr_wdr_attr_check(const char *src, const ot_isp_nr_wdr_attr *wdr_cfg);
td_s32 isp_nr_dering_attr_check(const char *src, const ot_isp_nr_dering_attr *dering_cfg);

td_s32 isp_ldci_attr_check(const char *src, const ot_isp_ldci_attr *ldci_attr);
td_s32 isp_fswdr_attr_check(const char *src, const ot_isp_wdr_fs_attr *fswdr_attr);
td_s32 isp_dehaze_attr_check(const char *src, const ot_isp_dehaze_attr *dehaze_attr);

td_s32 isp_ca_attr_check(const char *src, const ot_isp_ca_attr *ca_attr);
td_s32 isp_clut_attr_check(const char *src, const ot_isp_clut_attr *clut_attr);

td_s32 isp_dynamic_blc_attr_check(ot_vi_pipe vi_pipe, const char *src,
    const ot_isp_black_level_dynamic_attr *dynamic_attr, ot_isp_black_level_mode black_level_mode);
td_s32 isp_user_black_level_en_check(ot_vi_pipe vi_pipe, const td_bool user_black_level_en);
td_s32 isp_black_level_value_check(const char *src, const td_u16 (*black_level)[OT_ISP_BAYER_CHN_NUM]);

td_s32 isp_crosstalk_attr_check(const char *src, const ot_isp_cr_attr *cr_attr);

td_s32 isp_lblc_lut_attr_check(const char *src, ot_vi_pipe vi_pipe, const ot_isp_lblc_lut_attr *lut_attr);

td_s32 isp_mesh_shading_gain_lut_attr_check(const char *src, ot_vi_pipe vi_pipe,
    const ot_isp_shading_lut_attr *lut_attr);
td_s32 isp_mesh_shading_attr_check(const char *src, ot_vi_pipe vi_pipe,
    const ot_isp_shading_attr *shading_attr);

td_s32 isp_expander_attr_check(const char *src, ot_vi_pipe vi_pipe, const ot_isp_expander_attr *expander_attr);
td_s32 isp_dp_dynamic_attr_check(const char *src, const ot_isp_dp_dynamic_attr *dp_dynamic_attr);

td_s32 isp_dcf_info_check(const char *src, const ot_isp_dcf_info* dcf_info);

td_s32 isp_dng_param_check(const char *src, const ot_isp_dng_color_param *dng_color_param);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif

