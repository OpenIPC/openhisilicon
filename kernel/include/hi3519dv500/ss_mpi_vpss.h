/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_VPSS_H
#define SS_MPI_VPSS_H

#include "ot_common.h"
#include "ot_common_video.h"
#include "ot_common_vb.h"
#include "ot_common_gdc.h"
#include "ot_common_vpss.h"

#ifdef __cplusplus
extern "C" {
#endif

/* module param settings */
td_s32 ss_mpi_vpss_set_mod_param(const ot_vpss_mod_param *mod_param);
td_s32 ss_mpi_vpss_get_mod_param(ot_vpss_mod_param *mod_param);

/* group settings */
td_s32 ss_mpi_vpss_create_grp(ot_vpss_grp grp, const ot_vpss_grp_attr *grp_attr);
td_s32 ss_mpi_vpss_destroy_grp(ot_vpss_grp grp);

td_s32 ss_mpi_vpss_start_grp(ot_vpss_grp grp);
td_s32 ss_mpi_vpss_stop_grp(ot_vpss_grp grp);

td_s32 ss_mpi_vpss_reset_grp(ot_vpss_grp grp);

td_s32 ss_mpi_vpss_set_grp_attr(ot_vpss_grp grp, const ot_vpss_grp_attr *grp_attr);
td_s32 ss_mpi_vpss_get_grp_attr(ot_vpss_grp grp, ot_vpss_grp_attr *grp_attr);

td_s32 ss_mpi_vpss_set_grp_3dnr_attr(ot_vpss_grp grp, const ot_3dnr_attr *attr);
td_s32 ss_mpi_vpss_get_grp_3dnr_attr(ot_vpss_grp grp, ot_3dnr_attr *attr);

td_s32 ss_mpi_vpss_set_grp_3dnr_param(ot_vpss_grp grp, const ot_3dnr_param *param);
td_s32 ss_mpi_vpss_get_grp_3dnr_param(ot_vpss_grp grp, ot_3dnr_param *param);

td_s32 ss_mpi_vpss_set_grp_param(ot_vpss_grp grp, const ot_vpss_grp_param *grp_param);
td_s32 ss_mpi_vpss_get_grp_param(ot_vpss_grp grp, ot_vpss_grp_param *grp_param);

td_s32 ss_mpi_vpss_set_grp_cfg(ot_vpss_grp grp, const ot_vpss_grp_cfg *grp_cfg);
td_s32 ss_mpi_vpss_get_grp_cfg(ot_vpss_grp grp, ot_vpss_grp_cfg *grp_cfg);

td_s32 ss_mpi_vpss_set_grp_crop(ot_vpss_grp grp, const ot_vpss_crop_info *crop_info);
td_s32 ss_mpi_vpss_get_grp_crop(ot_vpss_grp grp, ot_vpss_crop_info *crop_info);

td_s32 ss_mpi_vpss_send_frame(ot_vpss_grp grp, const ot_video_frame_info *frame_info, td_s32 milli_sec);
td_s32 ss_mpi_vpss_send_frame_ex(ot_vpss_grp grp, ot_vpss_grp_pipe pipe,
    const ot_video_frame_info *frame_info, td_s32 milli_sec);

td_s32 ss_mpi_vpss_get_grp_frame(ot_vpss_grp grp, ot_video_frame_info *frame_info, td_s32 milli_sec);
td_s32 ss_mpi_vpss_release_grp_frame(ot_vpss_grp grp, const ot_video_frame_info *frame_info);

td_s32 ss_mpi_vpss_get_grp_frame_ex(ot_vpss_grp grp, ot_vpss_grp_pipe pipe,
    ot_video_frame_info *frame_info, td_s32 milli_sec);
td_s32 ss_mpi_vpss_release_grp_frame_ex(ot_vpss_grp grp, ot_vpss_grp_pipe pipe,
    const ot_video_frame_info *frame_info);

td_s32 ss_mpi_vpss_enable_backup_frame(ot_vpss_grp grp);
td_s32 ss_mpi_vpss_disable_backup_frame(ot_vpss_grp grp);

td_s32 ss_mpi_vpss_enable_user_frame_rate_ctrl(ot_vpss_grp grp);
td_s32 ss_mpi_vpss_disable_user_frame_rate_ctrl(ot_vpss_grp grp);

td_s32 ss_mpi_vpss_set_grp_frame_interrupt_attr(ot_vpss_grp grp, const ot_frame_interrupt_attr *frame_interrupt_attr);
td_s32 ss_mpi_vpss_get_grp_frame_interrupt_attr(ot_vpss_grp grp, ot_frame_interrupt_attr *frame_interrupt_attr);

/* chn settings */
td_s32 ss_mpi_vpss_set_chn_attr(ot_vpss_grp grp, ot_vpss_chn chn, const ot_vpss_chn_attr *chn_attr);
td_s32 ss_mpi_vpss_get_chn_attr(ot_vpss_grp grp, ot_vpss_chn chn, ot_vpss_chn_attr *chn_attr);

td_s32 ss_mpi_vpss_enable_chn(ot_vpss_grp grp, ot_vpss_chn chn);
td_s32 ss_mpi_vpss_disable_chn(ot_vpss_grp grp, ot_vpss_chn chn);

td_s32 ss_mpi_vpss_set_chn_fmu_mode(ot_vpss_grp grp, ot_vpss_chn chn, ot_fmu_mode mode);
td_s32 ss_mpi_vpss_get_chn_fmu_mode(ot_vpss_grp grp, ot_vpss_chn chn, ot_fmu_mode *mode);

td_s32 ss_mpi_vpss_get_chn_rotation(ot_vpss_grp grp, ot_vpss_chn chn, ot_rotation_attr *rotation_attr);
td_s32 ss_mpi_vpss_set_chn_rotation(ot_vpss_grp grp, ot_vpss_chn chn, const ot_rotation_attr *rotation_attr);

td_s32 ss_mpi_vpss_get_chn_frame(ot_vpss_grp grp, ot_vpss_chn chn, ot_video_frame_info *frame_info, td_s32 milli_sec);
td_s32 ss_mpi_vpss_release_chn_frame(ot_vpss_grp grp, ot_vpss_chn chn, const ot_video_frame_info *frame_info);

td_s32 ss_mpi_vpss_get_chn_rgn_luma(ot_vpss_grp grp, ot_vpss_chn chn, const ot_vpss_rgn_info *luma_rgn,
    td_u64 *luma_data, td_s32 milli_sec);

td_s32 ss_mpi_vpss_set_chn_crop(ot_vpss_grp grp, ot_vpss_chn chn, const ot_vpss_crop_info *crop_info);
td_s32 ss_mpi_vpss_get_chn_crop(ot_vpss_grp grp, ot_vpss_chn chn, ot_vpss_crop_info *crop_info);

td_s32 ss_mpi_vpss_set_chn_vb_src(ot_vpss_grp grp, ot_vpss_chn chn, ot_vb_src vb_src);
td_s32 ss_mpi_vpss_get_chn_vb_src(ot_vpss_grp grp, ot_vpss_chn chn, ot_vb_src *vb_src);

td_s32 ss_mpi_vpss_attach_chn_vb_pool(ot_vpss_grp grp, ot_vpss_chn chn, ot_vb_pool vb_pool);
td_s32 ss_mpi_vpss_detach_chn_vb_pool(ot_vpss_grp grp, ot_vpss_chn chn);

td_s32 ss_mpi_vpss_set_chn_low_delay(ot_vpss_grp grp, ot_vpss_chn chn, const ot_low_delay_info *low_delay_info);
td_s32 ss_mpi_vpss_get_chn_low_delay(ot_vpss_grp grp, ot_vpss_chn chn, ot_low_delay_info *low_delay_info);

td_s32 ss_mpi_vpss_set_chn_stride_align(ot_vpss_grp grp, ot_vpss_chn chn, td_u32 align);
td_s32 ss_mpi_vpss_get_chn_stride_align(ot_vpss_grp grp, ot_vpss_chn chn, td_u32 *align);

td_s32 ss_mpi_vpss_set_ext_chn_attr(ot_vpss_grp grp, ot_vpss_chn chn, const ot_vpss_ext_chn_attr *ext_chn_attr);
td_s32 ss_mpi_vpss_get_ext_chn_attr(ot_vpss_grp grp, ot_vpss_chn chn, ot_vpss_ext_chn_attr *ext_chn_attr);

td_s32 ss_mpi_vpss_set_chn_scale_coef_type(ot_vpss_grp grp, ot_vpss_chn chn, ot_scale_coef_type type);
td_s32 ss_mpi_vpss_get_chn_scale_coef_type(ot_vpss_grp grp, ot_vpss_chn chn, ot_scale_coef_type *type);

/* gdc settings */
td_s32 ss_mpi_vpss_set_grp_gdc_param(ot_vpss_grp grp, const ot_gdc_param *gdc_param);
td_s32 ss_mpi_vpss_get_grp_gdc_param(ot_vpss_grp grp, ot_gdc_param *gdc_param);

td_s32 ss_mpi_vpss_set_grp_fisheye_cfg(ot_vpss_grp grp, const ot_fisheye_cfg *fisheye_cfg);
td_s32 ss_mpi_vpss_get_grp_fisheye_cfg(ot_vpss_grp grp, ot_fisheye_cfg *fisheye_cfg);

td_s32 ss_mpi_vpss_query_grp_fisheye_pos_dst_to_src(ot_vpss_grp grp, td_u32 rgn_index,
    const ot_point *dst, ot_point *src);
td_s32 ss_mpi_vpss_query_grp_ldc_pos_dst_to_src(ot_vpss_grp grp, const ot_point *dst, ot_point *src);
td_s32 ss_mpi_vpss_query_grp_ldc_pos_src_to_dst(ot_vpss_grp grp, const ot_point *src, ot_point *dst);

td_s32 ss_mpi_vpss_query_grp_gdc_pos_dst_to_src(ot_vpss_grp grp, td_u32 index,
    const ot_point *dst_point, ot_point *src_point);

td_s32 ss_mpi_vpss_set_grp_ldc(ot_vpss_grp grp, const ot_ldc_attr *ldc_attr);
td_s32 ss_mpi_vpss_get_grp_ldc(ot_vpss_grp grp, ot_ldc_attr *ldc_attr);

td_s32 ss_mpi_vpss_set_grp_spread(ot_vpss_grp grp, const ot_spread_attr *spread_attr);
td_s32 ss_mpi_vpss_get_grp_spread(ot_vpss_grp grp, ot_spread_attr *spread_attr);

td_s32 ss_mpi_vpss_set_grp_fisheye(ot_vpss_grp grp, const ot_fisheye_correction_attr *fisheye_attr);
td_s32 ss_mpi_vpss_get_grp_fisheye(ot_vpss_grp grp, ot_fisheye_correction_attr *fisheye_attr);

td_s32 ss_mpi_vpss_set_grp_rotation(ot_vpss_grp grp, const ot_rotation_attr *rotation_attr);
td_s32 ss_mpi_vpss_get_grp_rotation(ot_vpss_grp grp, ot_rotation_attr *rotation_attr);

td_s32 ss_mpi_vpss_set_grp_pmf(ot_vpss_grp grp, const ot_pmf_attr *pmf_attr);
td_s32 ss_mpi_vpss_get_grp_pmf(ot_vpss_grp grp, ot_pmf_attr *pmf_attr);

td_s32 ss_mpi_vpss_set_grp_fov(ot_vpss_grp grp, const ot_fov_attr *fov_attr);
td_s32 ss_mpi_vpss_get_grp_fov(ot_vpss_grp grp, ot_fov_attr *fov_attr);

td_s32 ss_mpi_vpss_set_grp_lut(ot_vpss_grp grp, const ot_lut_attr *lut_attr);
td_s32 ss_mpi_vpss_get_grp_lut(ot_vpss_grp grp, ot_lut_attr *lut_attr);

td_s32 ss_mpi_vpss_set_grp_zme(ot_vpss_grp grp, const ot_size *out_size);
td_s32 ss_mpi_vpss_get_grp_zme(ot_vpss_grp grp, ot_size *out_size);

td_s32 ss_mpi_vpss_set_grp_stitch(ot_vpss_grp grp, const ot_stitch_attr *stitch_attr);
td_s32 ss_mpi_vpss_get_grp_stitch(ot_vpss_grp grp, ot_stitch_attr *stitch_attr);

td_s32 ss_mpi_vpss_set_grp_stitch_blend_param(ot_vpss_grp grp, const ot_stitch_blend_param *blend_param);
td_s32 ss_mpi_vpss_get_grp_stitch_blend_param(ot_vpss_grp grp, ot_stitch_blend_param *blend_param);

td_s32 ss_mpi_vpss_start_grp_stitch_luma_sync(ot_vpss_grp grp, const ot_stitch_luma_sync_param *param);
td_s32 ss_mpi_vpss_stop_grp_stitch_luma_sync(ot_vpss_grp grp);

td_s32 ss_mpi_vpss_get_chn_fd(ot_vpss_grp grp, ot_vpss_chn chn);

td_s32 ss_mpi_vpss_set_grp_zoom_in_window(ot_vpss_grp grp, const ot_zoom_attr *zoom_attr);
td_s32 ss_mpi_vpss_get_grp_zoom_in_window(ot_vpss_grp grp, ot_zoom_attr *zoom_attr);
#ifdef __cplusplus
}
#endif

#endif /* SS_MPI_VPSS_H */
