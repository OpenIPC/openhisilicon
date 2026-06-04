/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef MPI_ISP_H
#define MPI_ISP_H

#include "ot_common_isp.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

td_s32 mpi_isp_set_pub_attr(ot_vi_pipe vi_pipe, const ot_isp_pub_attr *pub_attr);
td_s32 mpi_isp_get_pub_attr(ot_vi_pipe vi_pipe, ot_isp_pub_attr *pub_attr);
td_s32 mpi_isp_set_module_ctrl(ot_vi_pipe vi_pipe, const ot_isp_module_ctrl *mod_ctrl);
td_s32 mpi_isp_get_module_ctrl(ot_vi_pipe vi_pipe, ot_isp_module_ctrl *mod_ctrl);
td_s32 mpi_isp_set_fmw_state(ot_vi_pipe vi_pipe, const ot_isp_fmw_state state);
td_s32 mpi_isp_get_fmw_state(ot_vi_pipe vi_pipe, ot_isp_fmw_state *state);
td_s32 mpi_isp_set_ldci_attr(ot_vi_pipe vi_pipe, const ot_isp_ldci_attr *ldci_attr);
td_s32 mpi_isp_get_ldci_attr(ot_vi_pipe vi_pipe, ot_isp_ldci_attr *ldci_attr);
td_s32 mpi_isp_set_drc_attr(ot_vi_pipe vi_pipe, const ot_isp_drc_attr *drc_attr);
td_s32 mpi_isp_get_drc_attr(ot_vi_pipe vi_pipe, ot_isp_drc_attr *drc_attr);
td_s32 mpi_isp_set_dehaze_attr(ot_vi_pipe vi_pipe, const ot_isp_dehaze_attr *dehaze_attr);
td_s32 mpi_isp_get_dehaze_attr(ot_vi_pipe vi_pipe, ot_isp_dehaze_attr *dehaze_attr);
td_s32 mpi_isp_set_fswdr_attr(ot_vi_pipe vi_pipe, const ot_isp_wdr_fs_attr *fswdr_attr);
td_s32 mpi_isp_get_fswdr_attr(ot_vi_pipe vi_pipe, ot_isp_wdr_fs_attr *fswdr_attr);
td_s32 mpi_isp_get_focus_stats(ot_vi_pipe vi_pipe, ot_isp_af_stats *af_stat);
td_s32 mpi_isp_get_ae_stats(ot_vi_pipe vi_pipe, ot_isp_ae_stats *ae_stat);
#ifdef CONFIG_OT_VI_STITCH_GRP
td_s32 mpi_isp_get_ae_stitch_stats(ot_vi_pipe vi_pipe, ot_isp_ae_stitch_stats *ae_stitch_stat);
td_s32 mpi_isp_get_wb_stitch_stats(ot_vi_pipe vi_pipe, ot_isp_wb_stitch_stats *stitch_wb_stat);
#endif
td_s32 mpi_isp_get_wb_stats(ot_vi_pipe vi_pipe, ot_isp_wb_stats *wb_stat);
td_s32 mpi_isp_set_fe_roi_cfg(ot_vi_pipe vi_pipe, const ot_isp_fe_roi_cfg *fe_roi_cfg);
td_s32 mpi_isp_get_fe_roi_cfg(ot_vi_pipe vi_pipe, ot_isp_fe_roi_cfg *fe_roi_cfg);
td_s32 mpi_isp_set_stats_cfg(ot_vi_pipe vi_pipe, const ot_isp_stats_cfg *stat_cfg);
td_s32 mpi_isp_get_stats_cfg(ot_vi_pipe vi_pipe, ot_isp_stats_cfg *stat_cfg);
td_s32 mpi_isp_set_gamma_attr(ot_vi_pipe vi_pipe, const ot_isp_gamma_attr *gamma_attr);
td_s32 mpi_isp_get_gamma_attr(ot_vi_pipe vi_pipe, ot_isp_gamma_attr *gamma_attr);
td_s32 mpi_isp_set_pregamma_attr(ot_vi_pipe vi_pipe, const ot_isp_pregamma_attr *pregamma_attr);
td_s32 mpi_isp_get_pregamma_attr(ot_vi_pipe vi_pipe, ot_isp_pregamma_attr *pregamma_attr);
td_s32 mpi_isp_set_csc_attr(ot_vi_pipe vi_pipe, const ot_isp_csc_attr *csc_attr);
td_s32 mpi_isp_get_csc_attr(ot_vi_pipe vi_pipe, ot_isp_csc_attr *csc_attr);
#ifdef CONFIG_OT_ISP_DPC_STATIC_TABLE_SUPPORT
td_s32 mpi_isp_set_dp_calibrate(ot_vi_pipe vi_pipe, const ot_isp_dp_static_calibrate *dp_calibrate);
td_s32 mpi_isp_get_dp_calibrate(ot_vi_pipe vi_pipe, ot_isp_dp_static_calibrate *dp_calibrate);
td_s32 mpi_isp_set_dp_static_attr(ot_vi_pipe vi_pipe, const ot_isp_dp_static_attr *dp_static_attr);
td_s32 mpi_isp_get_dp_static_attr(ot_vi_pipe vi_pipe, ot_isp_dp_static_attr *dp_static_attr);
#endif
td_s32 mpi_isp_set_dp_dynamic_attr(ot_vi_pipe vi_pipe, const ot_isp_dp_dynamic_attr *dp_dynamic_attr);
td_s32 mpi_isp_get_dp_dynamic_attr(ot_vi_pipe vi_pipe, ot_isp_dp_dynamic_attr *dp_dynamic_attr);
#ifdef CONFIG_OT_ISP_LBLC_SUPPORT
td_s32 mpi_isp_set_lblc_attr(ot_vi_pipe vi_pipe, const ot_isp_lblc_attr *lblc_attr);
td_s32 mpi_isp_get_lblc_attr(ot_vi_pipe vi_pipe, ot_isp_lblc_attr *lblc_attr);

td_s32 mpi_isp_set_lblc_lut_attr(ot_vi_pipe vi_pipe, const ot_isp_lblc_lut_attr *lblc_lut_attr);
td_s32 mpi_isp_get_lblc_lut_attr(ot_vi_pipe vi_pipe, ot_isp_lblc_lut_attr *lblc_lut_attr);
#endif
td_s32 mpi_isp_usr_set_mesh_shading_attr(ot_vi_pipe vi_pipe, const ot_isp_shading_attr *shading_attr);
td_s32 mpi_isp_usr_get_mesh_shading_attr(ot_vi_pipe vi_pipe, ot_isp_shading_attr *shading_attr);
td_s32 mpi_isp_usr_set_mesh_shading_lut_attr(ot_vi_pipe vi_pipe,
    const ot_isp_shading_lut_attr *shading_lut_attr);
td_s32 mpi_isp_usr_get_mesh_shading_lut_attr(ot_vi_pipe vi_pipe, ot_isp_shading_lut_attr *shading_lut_attr);
td_s32 mpi_isp_set_auto_color_shading_attr(ot_vi_pipe vi_pipe, const ot_isp_acs_attr *acs_attr);
td_s32 mpi_isp_get_auto_color_shading_attr(ot_vi_pipe vi_pipe, ot_isp_acs_attr *acs_attr);
td_s32 mpi_isp_set_cac_attr(ot_vi_pipe vi_pipe, const ot_isp_cac_attr *cac_attr);
td_s32 mpi_isp_get_cac_attr(ot_vi_pipe vi_pipe, ot_isp_cac_attr *cac_attr);
#ifdef  CONFIG_OT_ISP_BAYER_SHP_SUPPORT
td_s32 mpi_isp_set_bayershp_attr(ot_vi_pipe vi_pipe, const ot_isp_bayershp_attr *bshp_attr);
td_s32 mpi_isp_get_bayershp_attr(ot_vi_pipe vi_pipe, ot_isp_bayershp_attr *bshp_attr);
#endif
td_s32 mpi_isp_set_nr_attr(ot_vi_pipe vi_pipe, const ot_isp_nr_attr *nr_attr);
td_s32 mpi_isp_get_nr_attr(ot_vi_pipe vi_pipe, ot_isp_nr_attr *nr_attr);
#ifdef CONFIG_OT_ISP_RGBIR_SUPPORT
td_s32 mpi_isp_set_rgbir_attr(ot_vi_pipe vi_pipe, const ot_isp_rgbir_attr *rgbir_attr);
td_s32 mpi_isp_get_rgbir_attr(ot_vi_pipe vi_pipe, ot_isp_rgbir_attr *rgbir_attr);
#endif
td_s32 mpi_isp_set_color_tone_attr(ot_vi_pipe vi_pipe, const ot_isp_color_tone_attr *ct_attr);
td_s32 mpi_isp_get_color_tone_attr(ot_vi_pipe vi_pipe, ot_isp_color_tone_attr *ct_attr);
td_s32 mpi_isp_set_sharpen_attr(ot_vi_pipe vi_pipe, const ot_isp_sharpen_attr *shp_attr);
td_s32 mpi_isp_get_sharpen_attr(ot_vi_pipe vi_pipe, ot_isp_sharpen_attr *shp_attr);
#ifdef CONFIG_OT_ISP_CR_SUPPORT
td_s32 mpi_isp_set_crosstalk_attr(ot_vi_pipe vi_pipe, const ot_isp_cr_attr *cr_attr);
td_s32 mpi_isp_get_crosstalk_attr(ot_vi_pipe vi_pipe, ot_isp_cr_attr *cr_attr);
#endif
td_s32 mpi_isp_set_anti_false_color_attr(ot_vi_pipe vi_pipe, const ot_isp_anti_false_color_attr *anti_false_color);
td_s32 mpi_isp_get_anti_false_color_attr(ot_vi_pipe vi_pipe, ot_isp_anti_false_color_attr *anti_false_color);
td_s32 mpi_isp_set_demosaic_attr(ot_vi_pipe vi_pipe, const ot_isp_demosaic_attr *demosaic_attr);
td_s32 mpi_isp_get_demosaic_attr(ot_vi_pipe vi_pipe, ot_isp_demosaic_attr *demosaic_attr);
#ifdef CONFIG_OT_ISP_CA_SUPPORT
td_s32 mpi_isp_set_ca_attr(ot_vi_pipe vi_pipe, const ot_isp_ca_attr *ca_attr);
td_s32 mpi_isp_get_ca_attr(ot_vi_pipe vi_pipe, ot_isp_ca_attr *ca_attr);
#endif
td_s32 mpi_isp_set_black_level_attr(ot_vi_pipe vi_pipe, const ot_isp_black_level_attr *black_level);
td_s32 mpi_isp_get_black_level_attr(ot_vi_pipe vi_pipe, ot_isp_black_level_attr *black_level);
#ifdef CONFIG_OT_VI_PIPE_FPN
td_s32 mpi_isp_fpn_calibrate(ot_vi_pipe vi_pipe, ot_isp_fpn_calibrate_attr *calibrate_attr);
td_s32 mpi_isp_set_fpn_attr(ot_vi_pipe vi_pipe, const ot_isp_fpn_attr *correction);
#endif
td_s32 mpi_isp_get_reg_attr(ot_vi_pipe vi_pipe, ot_isp_reg_attr *isp_reg_attr);
td_s32 mpi_isp_query_inner_state_info(ot_vi_pipe vi_pipe, ot_isp_inner_state_info *inner_state_info);
td_s32 mpi_isp_get_dng_image_static_info(ot_vi_pipe vi_pipe, ot_isp_dng_image_static_info *dng_image_static_info);
td_s32 mpi_isp_set_dng_color_param(ot_vi_pipe vi_pipe, const ot_isp_dng_color_param *dng_color_param);
td_s32 mpi_isp_get_dng_color_param(ot_vi_pipe vi_pipe, ot_isp_dng_color_param *dng_color_param);
td_s32 mpi_isp_set_smart_info(ot_vi_pipe vi_pipe, const ot_isp_smart_info *smart_info);
td_s32 mpi_isp_get_smart_info(ot_vi_pipe vi_pipe, ot_isp_smart_info *smart_info);
td_s32 mpi_isp_calc_flicker_type(ot_vi_pipe vi_pipe, ot_isp_calc_flicker_input *input_param,
                                 ot_isp_calc_flicker_output *output_param, ot_video_frame_info frame[],
                                 td_u32 array_size);
td_s32 mpi_isp_set_expander_attr(ot_vi_pipe vi_pipe, const ot_isp_expander_attr *expander_attr);
td_s32 mpi_isp_get_expander_attr(ot_vi_pipe vi_pipe, ot_isp_expander_attr *expander_attr);
td_s32 mpi_isp_set_be_frame_pos_attr(ot_vi_pipe vi_pipe, const ot_isp_be_frame_attr *be_frame_attr);
td_s32 mpi_isp_get_be_frame_pos_attr(ot_vi_pipe vi_pipe, ot_isp_be_frame_attr *be_frame_attr);
td_s32 mpi_isp_get_noise_calibration(ot_vi_pipe vi_pipe, ot_isp_noise_calibration *noise_calibration);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
