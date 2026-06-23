/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_ISP_H
#define SS_MPI_ISP_H

#include "ot_type.h"
#include "ot_common_isp.h"
#include "ot_common_sns.h"
#include "ot_common_3a.h"
#include "ot_common_video.h"
#ifdef CONFIG_OT_SNAP_SUPPORT
#include "ot_common_snap.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Firmware Main Operation */
td_s32 ss_mpi_isp_init(ot_vi_pipe vi_pipe);
td_s32 ss_mpi_isp_mem_init(ot_vi_pipe vi_pipe);
td_s32 ss_mpi_isp_run(ot_vi_pipe vi_pipe);
td_s32 ss_mpi_isp_run_once(ot_vi_pipe vi_pipe);
td_s32 ss_mpi_isp_exit(ot_vi_pipe vi_pipe);
td_s32 ss_mpi_isp_mem_share(ot_vi_pipe vi_pipe, td_s32 pid);
td_s32 ss_mpi_isp_mem_unshare(ot_vi_pipe vi_pipe, td_s32 pid);
td_s32 ss_mpi_isp_mem_share_all(ot_vi_pipe vi_pipe);
td_s32 ss_mpi_isp_mem_unshare_all(ot_vi_pipe vi_pipe);
td_s32 ss_mpi_isp_sensor_reg_callback(ot_vi_pipe vi_pipe, ot_isp_sns_attr_info *sns_attr_info,
    const ot_isp_sns_register *sns_register);
td_s32 ss_mpi_isp_sensor_unreg_callback(ot_vi_pipe vi_pipe, ot_sensor_id sensor_id);

/* if have registered multy libs, set bind attr to appoint the active lib. */
td_s32 ss_mpi_isp_set_bind_attr(ot_vi_pipe vi_pipe, const ot_isp_bind_attr *bind_attr);
td_s32 ss_mpi_isp_get_bind_attr(ot_vi_pipe vi_pipe, ot_isp_bind_attr *bind_attr);
td_s32 ss_mpi_isp_ae_lib_reg_callback(ot_vi_pipe vi_pipe, const ot_isp_3a_alg_lib *ae_lib,
    const ot_isp_ae_register *ae_register);
td_s32 ss_mpi_isp_awb_lib_reg_callback(ot_vi_pipe vi_pipe, const ot_isp_3a_alg_lib *awb_lib,
    const ot_isp_awb_register *awb_register);
td_s32 ss_mpi_isp_ae_lib_unreg_callback(ot_vi_pipe vi_pipe, const ot_isp_3a_alg_lib *ae_lib);
td_s32 ss_mpi_isp_awb_lib_unreg_callback(ot_vi_pipe vi_pipe, const ot_isp_3a_alg_lib *awb_lib);

td_s32 ss_mpi_isp_get_lightbox_gain(ot_vi_pipe vi_pipe, ot_isp_awb_calibration_gain *awb_calibration_gain);

td_s32 ss_mpi_isp_set_pub_attr(ot_vi_pipe vi_pipe, const ot_isp_pub_attr *pub_attr);
td_s32 ss_mpi_isp_get_pub_attr(ot_vi_pipe vi_pipe, ot_isp_pub_attr *pub_attr);

td_s32 ss_mpi_isp_set_pipe_differ_attr(ot_vi_pipe vi_pipe, const ot_isp_pipe_diff_attr *pipe_differ);
td_s32 ss_mpi_isp_get_pipe_differ_attr(ot_vi_pipe vi_pipe, ot_isp_pipe_diff_attr *pipe_differ);

td_s32 ss_mpi_isp_get_vd_time_out(ot_vi_pipe vi_pipe, ot_isp_vd_type isp_vd_type, td_u32 milli_sec);
td_s32 ss_mpi_isp_query_inner_state_info(ot_vi_pipe vi_pipe, ot_isp_inner_state_info *inner_state_info);
td_s32 ss_mpi_isp_get_ae_stats(ot_vi_pipe vi_pipe, ot_isp_ae_stats *ae_stat);
td_s32 ss_mpi_isp_get_ae_stitch_stats(ot_vi_pipe vi_pipe, ot_isp_ae_stitch_stats *stitch_stat);
td_s32 ss_mpi_isp_get_mg_stats(ot_vi_pipe vi_pipe, ot_isp_mg_stats *mg_stat);

td_s32 ss_mpi_isp_get_wb_stats(ot_vi_pipe vi_pipe, ot_isp_wb_stats *wb_stat);
td_s32 ss_mpi_isp_get_wb_stitch_stats(ot_vi_pipe vi_pipe, ot_isp_wb_stitch_stats *stitch_wb_stat);

td_s32 ss_mpi_isp_get_focus_stats(ot_vi_pipe vi_pipe, ot_isp_af_stats *af_stat);

td_s32 ss_mpi_isp_set_stats_cfg(ot_vi_pipe vi_pipe, const ot_isp_stats_cfg *stat_cfg);
td_s32 ss_mpi_isp_get_stats_cfg(ot_vi_pipe vi_pipe, ot_isp_stats_cfg *stat_cfg);

td_s32 ss_mpi_isp_set_fe_roi_cfg(ot_vi_pipe vi_pipe, const ot_isp_fe_roi_cfg *fe_roi_cfg);
td_s32 ss_mpi_isp_get_fe_roi_cfg(ot_vi_pipe vi_pipe, ot_isp_fe_roi_cfg *fe_roi_cfg);
td_s32 ss_mpi_isp_get_isp_reg_attr(ot_vi_pipe vi_pipe, ot_isp_reg_attr *isp_reg_attr);

td_s32 ss_mpi_isp_set_fmw_state(ot_vi_pipe vi_pipe, const ot_isp_fmw_state state);
td_s32 ss_mpi_isp_get_fmw_state(ot_vi_pipe vi_pipe, ot_isp_fmw_state *state);

td_s32 ss_mpi_isp_set_debug(ot_vi_pipe vi_pipe, const ot_isp_debug_info *isp_debug);
td_s32 ss_mpi_isp_get_debug(ot_vi_pipe vi_pipe, ot_isp_debug_info *isp_debug);

td_s32 ss_mpi_isp_set_mod_param(const ot_isp_mod_param *mod_param);
td_s32 ss_mpi_isp_get_mod_param(ot_isp_mod_param *mod_param);
td_s32 ss_mpi_isp_set_ctrl_param(ot_vi_pipe vi_pipe, const ot_isp_ctrl_param *isp_ctrl_param);
td_s32 ss_mpi_isp_get_ctrl_param(ot_vi_pipe vi_pipe, ot_isp_ctrl_param *isp_ctrl_param);

td_s32 ss_mpi_isp_set_fswdr_attr(ot_vi_pipe vi_pipe, const ot_isp_wdr_fs_attr *fswdr_attr);
td_s32 ss_mpi_isp_get_fswdr_attr(ot_vi_pipe vi_pipe, ot_isp_wdr_fs_attr *fswdr_attr);

td_s32 ss_mpi_isp_set_drc_attr(ot_vi_pipe vi_pipe, const ot_isp_drc_attr *drc_attr);
td_s32 ss_mpi_isp_get_drc_attr(ot_vi_pipe vi_pipe, ot_isp_drc_attr *drc_attr);

td_s32 ss_mpi_isp_set_dehaze_attr(ot_vi_pipe vi_pipe, const ot_isp_dehaze_attr *dehaze_attr);
td_s32 ss_mpi_isp_get_dehaze_attr(ot_vi_pipe vi_pipe, ot_isp_dehaze_attr *dehaze_attr);

td_s32 ss_mpi_isp_set_expander_attr(ot_vi_pipe vi_pipe, const ot_isp_expander_attr *expander_attr);
td_s32 ss_mpi_isp_get_expander_attr(ot_vi_pipe vi_pipe, ot_isp_expander_attr *expander_attr);

td_s32 ss_mpi_isp_set_ldci_attr(ot_vi_pipe vi_pipe, const ot_isp_ldci_attr *ldci_attr);
td_s32 ss_mpi_isp_get_ldci_attr(ot_vi_pipe vi_pipe, ot_isp_ldci_attr *ldci_attr);

td_s32 ss_mpi_isp_set_sns_slave_attr(ot_slave_dev slave_dev, const ot_isp_slave_sns_sync *sns_sync);
td_s32 ss_mpi_isp_get_sns_slave_attr(ot_slave_dev slave_dev, ot_isp_slave_sns_sync *sns_sync);

td_s32 ss_mpi_isp_set_module_ctrl(ot_vi_pipe vi_pipe, const ot_isp_module_ctrl *mod_ctrl);
td_s32 ss_mpi_isp_get_module_ctrl(ot_vi_pipe vi_pipe, ot_isp_module_ctrl *mod_ctrl);

td_s32 ss_mpi_isp_set_dp_calibrate(ot_vi_pipe vi_pipe, const ot_isp_dp_static_calibrate *dp_calibrate);
td_s32 ss_mpi_isp_get_dp_calibrate(ot_vi_pipe vi_pipe, ot_isp_dp_static_calibrate *dp_calibrate);

td_s32 ss_mpi_isp_set_dp_static_attr(ot_vi_pipe vi_pipe, const ot_isp_dp_static_attr *dp_static_attr);
td_s32 ss_mpi_isp_get_dp_static_attr(ot_vi_pipe vi_pipe, ot_isp_dp_static_attr *dp_static_attr);

td_s32 ss_mpi_isp_set_dp_dynamic_attr(ot_vi_pipe vi_pipe, const ot_isp_dp_dynamic_attr *dp_dynamic_attr);
td_s32 ss_mpi_isp_get_dp_dynamic_attr(ot_vi_pipe vi_pipe, ot_isp_dp_dynamic_attr *dp_dynamic_attr);

td_s32 ss_mpi_isp_set_lblc_attr(ot_vi_pipe vi_pipe, const ot_isp_lblc_attr *lblc_attr);
td_s32 ss_mpi_isp_get_lblc_attr(ot_vi_pipe vi_pipe, ot_isp_lblc_attr *lblc_attr);

td_s32 ss_mpi_isp_set_lblc_lut_attr(ot_vi_pipe vi_pipe, const ot_isp_lblc_lut_attr *lblc_lut_attr);
td_s32 ss_mpi_isp_get_lblc_lut_attr(ot_vi_pipe vi_pipe, ot_isp_lblc_lut_attr *lblc_lut_attr);

td_s32 ss_mpi_isp_set_mesh_shading_attr(ot_vi_pipe vi_pipe, const ot_isp_shading_attr *shading_attr);
td_s32 ss_mpi_isp_get_mesh_shading_attr(ot_vi_pipe vi_pipe, ot_isp_shading_attr *shading_attr);

td_s32 ss_mpi_isp_set_mesh_shading_gain_lut_attr(ot_vi_pipe vi_pipe, const ot_isp_shading_lut_attr *shading_lut_attr);
td_s32 ss_mpi_isp_get_mesh_shading_gain_lut_attr(ot_vi_pipe vi_pipe, ot_isp_shading_lut_attr *shading_lut_attr);

td_s32 ss_mpi_isp_set_auto_color_shading_attr(ot_vi_pipe vi_pipe, const ot_isp_acs_attr *acs_attr);
td_s32 ss_mpi_isp_get_auto_color_shading_attr(ot_vi_pipe vi_pipe, ot_isp_acs_attr *acs_attr);

td_s32 ss_mpi_isp_mesh_shading_calibration(ot_vi_pipe vi_pipe, const td_u16 *src_raw,
    ot_isp_mlsc_calibration_cfg *mlsc_cali_cfg, ot_isp_mesh_shading_table *mlsc_table);

td_s32 ss_mpi_isp_set_nr_attr(ot_vi_pipe vi_pipe, const ot_isp_nr_attr *nr_attr);
td_s32 ss_mpi_isp_get_nr_attr(ot_vi_pipe vi_pipe, ot_isp_nr_attr *nr_attr);

td_s32 ss_mpi_isp_set_color_tone_attr(ot_vi_pipe vi_pipe, const ot_isp_color_tone_attr *ct_attr);
td_s32 ss_mpi_isp_get_color_tone_attr(ot_vi_pipe vi_pipe, ot_isp_color_tone_attr *ct_attr);

td_s32 ss_mpi_isp_set_gamma_attr(ot_vi_pipe vi_pipe, const ot_isp_gamma_attr *gamma_attr);
td_s32 ss_mpi_isp_get_gamma_attr(ot_vi_pipe vi_pipe, ot_isp_gamma_attr *gamma_attr);

td_s32 ss_mpi_isp_set_pregamma_attr(ot_vi_pipe vi_pipe, const ot_isp_pregamma_attr *pregamma_attr);
td_s32 ss_mpi_isp_get_pregamma_attr(ot_vi_pipe vi_pipe, ot_isp_pregamma_attr *pregamma_attr);

td_s32 ss_mpi_isp_set_cac_attr(ot_vi_pipe vi_pipe, const ot_isp_cac_attr *cac_attr);
td_s32 ss_mpi_isp_get_cac_attr(ot_vi_pipe vi_pipe, ot_isp_cac_attr *cac_attr);

td_s32 ss_mpi_isp_set_bayershp_attr(ot_vi_pipe vi_pipe, const ot_isp_bayershp_attr *bshp_attr);
td_s32 ss_mpi_isp_get_bayershp_attr(ot_vi_pipe vi_pipe, ot_isp_bayershp_attr *bshp_attr);

td_s32 ss_mpi_isp_set_rc_attr(ot_vi_pipe vi_pipe, const ot_isp_rc_attr *rc_attr);
td_s32 ss_mpi_isp_get_rc_attr(ot_vi_pipe vi_pipe, ot_isp_rc_attr *rc_attr);

td_s32 ss_mpi_isp_set_sharpen_attr(ot_vi_pipe vi_pipe, const ot_isp_sharpen_attr *shp_attr);
td_s32 ss_mpi_isp_get_sharpen_attr(ot_vi_pipe vi_pipe, ot_isp_sharpen_attr *shp_attr);

td_s32 ss_mpi_isp_set_crosstalk_attr(ot_vi_pipe vi_pipe, const ot_isp_cr_attr *cr_attr);
td_s32 ss_mpi_isp_get_crosstalk_attr(ot_vi_pipe vi_pipe, ot_isp_cr_attr *cr_attr);

td_s32 ss_mpi_isp_set_ca_attr(ot_vi_pipe vi_pipe, const ot_isp_ca_attr *ca_attr);
td_s32 ss_mpi_isp_get_ca_attr(ot_vi_pipe vi_pipe, ot_isp_ca_attr *ca_attr);

td_s32 ss_mpi_isp_set_anti_false_color_attr(ot_vi_pipe vi_pipe, const ot_isp_anti_false_color_attr *anti_false_color);
td_s32 ss_mpi_isp_get_anti_false_color_attr(ot_vi_pipe vi_pipe, ot_isp_anti_false_color_attr *anti_false_color);

td_s32 ss_mpi_isp_set_demosaic_attr(ot_vi_pipe vi_pipe, const ot_isp_demosaic_attr *demosaic_attr);
td_s32 ss_mpi_isp_get_demosaic_attr(ot_vi_pipe vi_pipe, ot_isp_demosaic_attr *demosaic_attr);

td_s32 ss_mpi_isp_set_black_level_attr(ot_vi_pipe vi_pipe, const ot_isp_black_level_attr *black_level);
td_s32 ss_mpi_isp_get_black_level_attr(ot_vi_pipe vi_pipe, ot_isp_black_level_attr *black_level);

td_s32 ss_mpi_isp_set_clut_attr(ot_vi_pipe vi_pipe, const ot_isp_clut_attr *clut_attr);
td_s32 ss_mpi_isp_get_clut_attr(ot_vi_pipe vi_pipe, ot_isp_clut_attr *clut_attr);

td_s32 ss_mpi_isp_set_clut_coeff(ot_vi_pipe vi_pipe, const ot_isp_clut_lut *clut_lut);
td_s32 ss_mpi_isp_get_clut_coeff(ot_vi_pipe vi_pipe, ot_isp_clut_lut *clut_lut);

td_s32 ss_mpi_isp_set_dcf_info(ot_vi_pipe vi_pipe, const ot_isp_dcf_info *isp_dcf);
td_s32 ss_mpi_isp_get_dcf_info(ot_vi_pipe vi_pipe, ot_isp_dcf_info *isp_dcf);

td_s32 ss_mpi_isp_set_csc_attr(ot_vi_pipe vi_pipe, const ot_isp_csc_attr *csc_attr);
td_s32 ss_mpi_isp_get_csc_attr(ot_vi_pipe vi_pipe, ot_isp_csc_attr *csc_attr);

td_s32 ss_mpi_isp_set_rgbir_attr(ot_vi_pipe vi_pipe, const ot_isp_rgbir_attr *rgbir_attr);
td_s32 ss_mpi_isp_get_rgbir_attr(ot_vi_pipe vi_pipe, ot_isp_rgbir_attr *rgbir_attr);

/* block, return fpn frame when this is function is called. */
td_s32 ss_mpi_isp_fpn_calibrate(ot_vi_pipe vi_pipe, ot_isp_fpn_calibrate_attr *calibrate_attr);
td_s32 ss_mpi_isp_set_fpn_attr(ot_vi_pipe vi_pipe, const ot_isp_fpn_attr *fpn_attr);
td_s32 ss_mpi_isp_get_fpn_attr(ot_vi_pipe vi_pipe, ot_isp_fpn_attr *fpn_attr);

td_s32 ss_mpi_isp_get_dng_image_static_info(ot_vi_pipe vi_pipe, ot_isp_dng_image_static_info *dng_image_static_info);
td_s32 ss_mpi_isp_set_dng_color_param(ot_vi_pipe vi_pipe, const ot_isp_dng_color_param *dng_color_param);
td_s32 ss_mpi_isp_get_dng_color_param(ot_vi_pipe vi_pipe, ot_isp_dng_color_param *dng_color_param);

td_s32 ss_mpi_isp_set_smart_info(ot_vi_pipe vi_pipe, const ot_isp_smart_info *smart_info);
td_s32 ss_mpi_isp_get_smart_info(ot_vi_pipe vi_pipe, ot_isp_smart_info *smart_info);

td_s32 ss_mpi_isp_set_frame_info(ot_vi_pipe vi_pipe, const ot_isp_frame_info *isp_frame);
td_s32 ss_mpi_isp_get_frame_info(ot_vi_pipe vi_pipe, ot_isp_frame_info *isp_frame);
td_s32 ss_mpi_isp_calc_flicker_type(ot_vi_pipe vi_pipe, ot_isp_calc_flicker_input *input_param,
    ot_isp_calc_flicker_output *output_param, ot_video_frame_info frame[], td_u32 array_size);
td_s32 ss_mpi_isp_ir_auto(ot_vi_pipe vi_pipe, ot_isp_ir_auto_attr *ir_attr);

td_s32 ss_mpi_isp_set_be_frame_attr(ot_vi_pipe vi_pipe, const ot_isp_be_frame_attr *be_frame_attr);
td_s32 ss_mpi_isp_get_be_frame_attr(ot_vi_pipe vi_pipe, ot_isp_be_frame_attr *be_frame_attr);

td_s32 ss_mpi_isp_get_noise_calibration(ot_vi_pipe vi_pipe, ot_isp_noise_calibration *noise_calibration);
td_s32 ss_mpi_isp_set_detail_stats_cfg(ot_vi_pipe vi_pipe,
    const ot_isp_detail_stats_cfg *detail_stats_cfg);
td_s32 ss_mpi_isp_get_detail_stats_cfg(ot_vi_pipe vi_pipe, ot_isp_detail_stats_cfg *detail_stats_cfg);
td_s32 ss_mpi_isp_get_detail_stats(ot_vi_pipe vi_pipe, ot_isp_detail_stats *detail_stats);
#ifdef __cplusplus
}
#endif

#endif /* SS_MPI_ISP_H */
