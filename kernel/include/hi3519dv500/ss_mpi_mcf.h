/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_MCF_H
#define SS_MPI_MCF_H
#include "ot_common_video.h"
#include "ot_common_vb.h"
#include "ot_common_mcf.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

td_s32 ss_mpi_mcf_create_grp(ot_mcf_grp grp, const ot_mcf_grp_attr *grp_attr);
td_s32 ss_mpi_mcf_destroy_grp(ot_mcf_grp grp);

td_s32 ss_mpi_mcf_reset_grp(ot_mcf_grp grp);

td_s32 ss_mpi_mcf_start_grp(ot_mcf_grp grp);
td_s32 ss_mpi_mcf_stop_grp(ot_mcf_grp grp);

td_s32 ss_mpi_mcf_set_grp_attr(ot_mcf_grp grp, const ot_mcf_grp_attr *grp_attr);
td_s32 ss_mpi_mcf_get_grp_attr(ot_mcf_grp grp, ot_mcf_grp_attr *grp_attr);

td_s32 ss_mpi_mcf_set_grp_crop(ot_mcf_grp grp, const ot_mcf_crop_info *crop_info);
td_s32 ss_mpi_mcf_get_grp_crop(ot_mcf_grp grp, ot_mcf_crop_info *crop_info);

td_s32 ss_mpi_mcf_set_alg_param(ot_mcf_grp grp, const ot_mcf_alg_param *alg_param);
td_s32 ss_mpi_mcf_get_alg_param(ot_mcf_grp grp, ot_mcf_alg_param *alg_param);

td_s32 ss_mpi_mcf_send_pipe_frame(ot_mcf_grp grp, ot_mcf_pipe mcf_pipe, const ot_video_frame_info *video_frame,
    td_s32 milli_sec);
td_s32 ss_mpi_mcf_set_chn_attr(ot_mcf_grp grp, ot_mcf_chn chn, const ot_mcf_chn_attr *chn_attr);
td_s32 ss_mpi_mcf_get_chn_attr(ot_mcf_grp grp, ot_mcf_chn chn, ot_mcf_chn_attr *chn_attr);

td_s32 ss_mpi_mcf_enable_chn(ot_mcf_grp grp, ot_mcf_chn chn);
td_s32 ss_mpi_mcf_disable_chn(ot_mcf_grp grp, ot_mcf_chn chn);

td_s32 ss_mpi_mcf_get_chn_frame(ot_mcf_grp grp, ot_mcf_chn chn, ot_video_frame_info *video_frame, td_s32 milli_sec);
td_s32 ss_mpi_mcf_release_chn_frame(ot_mcf_grp grp, ot_mcf_chn chn, const ot_video_frame_info *video_frame);

td_s32 ss_mpi_mcf_set_chn_low_delay(ot_mcf_grp grp, ot_mcf_chn chn, const ot_low_delay_info *low_delay_info);
td_s32 ss_mpi_mcf_get_chn_low_delay(ot_mcf_grp grp, ot_mcf_chn chn, ot_low_delay_info *low_delay_info);

td_s32 ss_mpi_mcf_chn_attach_vb_pool(ot_mcf_grp grp, ot_mcf_chn chn, ot_vb_pool vb_pool);
td_s32 ss_mpi_mcf_chn_detach_vb_pool(ot_mcf_grp grp, ot_mcf_chn chn);

td_s32 ss_mpi_mcf_chn_set_vb_src(ot_mcf_grp grp, ot_mcf_chn chn, ot_vb_src vb_src);
td_s32 ss_mpi_mcf_chn_get_vb_src(ot_mcf_grp grp, ot_mcf_chn chn, ot_vb_src *vb_src);

td_s32 ss_mpi_mcf_set_chn_stride_align(ot_mcf_grp grp, ot_mcf_chn chn, td_u32 align);
td_s32 ss_mpi_mcf_get_chn_stride_align(ot_mcf_grp grp, ot_mcf_chn chn, td_u32 *align);

td_s32 ss_mpi_mcf_set_ext_chn_attr(ot_mcf_grp grp, ot_mcf_chn chn, const ot_mcf_ext_chn_attr *ext_chn_attr);
td_s32 ss_mpi_mcf_get_ext_chn_attr(ot_mcf_grp grp, ot_mcf_chn chn, ot_mcf_ext_chn_attr *ext_chn_attr);

td_s32 ss_mpi_mcf_set_chn_crop(ot_mcf_grp grp, ot_mcf_chn chn, const ot_mcf_crop_info *crop_info);
td_s32 ss_mpi_mcf_get_chn_crop(ot_mcf_grp grp, ot_mcf_chn chn, ot_mcf_crop_info *crop_info);

td_s32 ss_mpi_mcf_get_chn_rgn_luma(ot_mcf_grp grp, ot_mcf_chn chn, td_u32 num, const ot_rect *rgn, td_u64 *luma_data,
    td_s32 milli_sec);

td_s32 ss_mpi_mcf_set_chn_rotation(ot_mcf_grp grp, ot_mcf_chn chn, ot_rotation rotation);
td_s32 ss_mpi_mcf_get_chn_rotation(ot_mcf_grp grp, ot_mcf_chn chn, ot_rotation *rotation);

td_s32 ss_mpi_mcf_get_chn_fd(ot_mcf_grp grp, ot_mcf_chn chn);

td_s32 ss_mpi_mcf_get_grp_frame(ot_mcf_grp grp, ot_video_frame_info *color_frame,
    ot_video_frame_info *mono_frame, td_s32 milli_sec);
td_s32 ss_mpi_mcf_release_grp_frame(ot_mcf_grp grp, ot_video_frame_info *color_frame,
    ot_video_frame_info *mono_frame);

td_s32 ss_mpi_mcf_set_vi_attr(ot_mcf_grp grp, const ot_mcf_vi_attr *mcf_vi_attr);
td_s32 ss_mpi_mcf_get_vi_attr(ot_mcf_grp grp, ot_mcf_vi_attr *mcf_vi_attr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MPI_MCF_H */
