/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_EXT_REG_ACCESS_H
#define ISP_EXT_REG_ACCESS_H

#include "mkp_isp.h"
#include "ot_type.h"
#include "ot_common.h"
#include "ot_common_isp.h"
#include "ot_common_isp_inner.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

td_void isp_sharpen_comm_attr_write(ot_vi_pipe vi_pipe, const ot_isp_sharpen_attr *shp_attr);
td_void isp_sharpen_auto_attr_write(ot_vi_pipe vi_pipe, const ot_isp_sharpen_auto_attr *auto_attr);
td_void isp_sharpen_manual_attr_write(ot_vi_pipe vi_pipe, const ot_isp_sharpen_manual_attr *manual_attr);

td_void isp_sharpen_comm_attr_read(ot_vi_pipe vi_pipe, ot_isp_sharpen_attr *shp_attr);
td_void isp_sharpen_manual_attr_read(ot_vi_pipe vi_pipe, ot_isp_sharpen_manual_attr *manual_attr);
td_void isp_sharpen_auto_attr_read(ot_vi_pipe vi_pipe, ot_isp_sharpen_auto_attr *auto_attr);

td_void isp_nr_comm_attr_write(ot_vi_pipe vi_pipe, const ot_isp_nr_attr *nr_attr);
td_void isp_nr_snr_attr_write(ot_vi_pipe pipe, const ot_isp_nr_snr_attr *snr_cfg);
td_void isp_nr_tnr_attr_write(ot_vi_pipe pipe, const ot_isp_nr_tnr_attr *tnr_cfg);
td_void isp_nr_wdr_attr_write(ot_vi_pipe vi_pipe, const ot_isp_nr_wdr_attr *wdr_cfg);
td_void isp_nr_dering_attr_write(ot_vi_pipe vi_pipe, const ot_isp_nr_dering_attr *dering_cfg);
td_void isp_nr_ynet_attr_write(ot_vi_pipe vi_pipe, const ot_isp_nr_ynet_attr *ynet_cfg);

td_void isp_nr_comm_attr_read(ot_vi_pipe vi_pipe, ot_isp_nr_attr *nr_attr);
td_void isp_nr_snr_attr_read(ot_vi_pipe pipe, ot_isp_nr_snr_attr *snr_cfg);
td_void isp_nr_tnr_attr_read(ot_vi_pipe pipe, ot_isp_nr_tnr_attr *tnr_cfg);
td_void isp_nr_wdr_attr_read(ot_vi_pipe vi_pipe, ot_isp_nr_wdr_attr *wdr_cfg);
td_void isp_nr_dering_attr_read(ot_vi_pipe vi_pipe, ot_isp_nr_dering_attr *dering_cfg);
td_void isp_nr_ynet_attr_read(ot_vi_pipe vi_pipe, ot_isp_nr_ynet_attr *ynet_cfg);

td_void isp_drc_attr_write(ot_vi_pipe vi_pipe, const ot_isp_drc_attr *drc_attr);
td_void isp_drc_attr_read(ot_vi_pipe vi_pipe, ot_isp_drc_attr *drc_attr);

td_void isp_drc_blend_write(ot_vi_pipe vi_pipe, const isp_drc_blend *drc_blend);
td_void isp_drc_blend_read(ot_vi_pipe vi_pipe, isp_drc_blend *drc_blend);

td_void isp_ldci_attr_write(ot_vi_pipe vi_pipe, const ot_isp_ldci_attr *ldci_attr);
td_void isp_ldci_attr_read(ot_vi_pipe vi_pipe, ot_isp_ldci_attr *ldci_attr);

td_void isp_black_level_manual_attr_write(ot_vi_pipe vi_pipe, const ot_isp_black_level_manual_attr *manual_attr);
td_void isp_black_level_dynamic_attr_write(ot_vi_pipe vi_pipe, const ot_isp_black_level_dynamic_attr *dynamic_attr);
td_void isp_user_black_level_write(ot_vi_pipe vi_pipe, const td_u16 (*user_black_level)[OT_ISP_BAYER_CHN_NUM]);
td_void isp_black_level_manual_attr_read(ot_vi_pipe vi_pipe, ot_isp_black_level_manual_attr *manual_attr);
td_void isp_black_level_dynamic_attr_read(ot_vi_pipe vi_pipe, ot_isp_black_level_dynamic_attr *dynamic_attr);
td_void isp_user_black_level_read(ot_vi_pipe vi_pipe, td_u16 (*user_black_level)[OT_ISP_BAYER_CHN_NUM]);
td_void black_level_actual_value_write(ot_vi_pipe vi_pipe, const isp_blc_actual_info *actual);
td_void isp_black_level_actual_value_read(ot_vi_pipe vi_pipe, td_u16 black_level_actual[][4]);
td_void sns_black_level_actual_value_read(ot_vi_pipe vi_pipe, td_u16 (*sns_black_level)[OT_ISP_BAYER_CHN_NUM]);
td_void ori_black_level_actual_value_read(ot_vi_pipe vi_pipe, td_u16 (*ori_black_level)[OT_ISP_BAYER_CHN_NUM]);

td_void isp_bayershp_auto_attr_write(ot_vi_pipe vi_pipe, const ot_isp_bayershp_auto_attr *auto_attr);
td_void isp_bayershp_manual_attr_write(ot_vi_pipe vi_pipe, const ot_isp_bayershp_manual_attr *manual_attr);
td_void isp_bayershp_common_attr_write(ot_vi_pipe vi_pipe, const ot_isp_bayershp_attr *bshp_attr);

td_void isp_bayershp_manual_attr_read(ot_vi_pipe vi_pipe, ot_isp_bayershp_manual_attr *manual_attr);
td_void isp_bayershp_auto_attr_read(ot_vi_pipe vi_pipe, ot_isp_bayershp_auto_attr *auto_attr);
td_void isp_bayershp_common_attr_read(ot_vi_pipe vi_pipe, ot_isp_bayershp_attr *bshp_attr);

td_void isp_demosaic_attr_write(ot_vi_pipe vi_pipe, const ot_isp_demosaic_attr *dm_attr);
td_void isp_demosaic_attr_read(ot_vi_pipe vi_pipe, ot_isp_demosaic_attr *dm_attr);

td_void isp_cac_acac_attr_write(ot_vi_pipe vi_pipe, const ot_isp_cac_acac_attr *acac_cfg);
td_void isp_cac_lcac_attr_write(ot_vi_pipe vi_pipe, const ot_isp_cac_lcac_attr *lcac_cfg);

td_void isp_cac_acac_attr_read(ot_vi_pipe vi_pipe, ot_isp_cac_acac_attr *acac_cfg);
td_void isp_cac_lcac_attr_read(ot_vi_pipe vi_pipe, ot_isp_cac_lcac_attr *lcac_cfg);

td_void isp_anti_false_color_attr_write(ot_vi_pipe vi_pipe, const ot_isp_anti_false_color_attr *anti_false_color);
td_void isp_anti_false_color_attr_read(ot_vi_pipe vi_pipe, ot_isp_anti_false_color_attr *anti_false_color);

td_void isp_rgbir_attr_write(ot_vi_pipe vi_pipe, const ot_isp_rgbir_attr *rgbir_attr);
td_void isp_rgbir_attr_read(ot_vi_pipe vi_pipe, ot_isp_rgbir_attr *rgbir_attr);

td_void isp_af_stats_config_write(ot_vi_pipe vi_pipe, const ot_isp_focus_stats_cfg *af_cfg);

td_void isp_fswdr_attr_write(ot_vi_pipe vi_pipe, const ot_isp_wdr_fs_attr *fswdr_attr);
td_void isp_fswdr_attr_read(ot_vi_pipe vi_pipe, ot_isp_wdr_fs_attr *fswdr_attr);

td_void isp_expander_attr_write(ot_vi_pipe vi_pipe, const ot_isp_expander_attr *expander_attr);
td_void isp_expander_attr_read(ot_vi_pipe vi_pipe, ot_isp_expander_attr *expander_attr);

td_void isp_get_fe_roi_config(ot_vi_pipe vi_pipe, ot_isp_fe_roi_cfg *fe_roi_cfg);
td_void isp_set_mesh_shading_gain_lut_attr(ot_vi_pipe vi_pipe,
    const ot_isp_shading_lut_attr *shading_lut_attr);
td_void isp_set_mesh_shading_attr(ot_vi_pipe vi_pipe, const ot_isp_shading_attr *shading_attr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
