/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_VO_H
#define SS_MPI_VO_H

#include "ot_common_vo.h"

#ifdef __cplusplus
extern "C" {
#endif
td_s32 ss_mpi_vo_enable_video_layer(ot_vo_layer layer);
td_s32 ss_mpi_vo_disable_video_layer(ot_vo_layer layer);
td_s32 ss_mpi_vo_set_video_layer_attr(ot_vo_layer layer, const ot_vo_video_layer_attr *layer_attr);
td_s32 ss_mpi_vo_get_video_layer_attr(ot_vo_layer layer, ot_vo_video_layer_attr *layer_attr);
td_s32 ss_mpi_vo_set_video_layer_param(ot_vo_layer layer, const ot_vo_layer_param *layer_param);
td_s32 ss_mpi_vo_get_video_layer_param(ot_vo_layer layer, ot_vo_layer_param *layer_param);
td_s32 ss_mpi_vo_set_video_layer_mirror(ot_vo_layer layer, ot_vo_mirror_mode mirror_mode);
td_s32 ss_mpi_vo_get_video_layer_mirror(ot_vo_layer layer, ot_vo_mirror_mode *mirror_mode);
td_s32 ss_mpi_vo_set_video_layer_crop(ot_vo_layer layer, const ot_crop_info *crop_info);
td_s32 ss_mpi_vo_get_video_layer_crop(ot_vo_layer layer, ot_crop_info *crop_info);
td_s32 ss_mpi_vo_set_video_layer_early_display(ot_vo_layer layer, td_bool early_display_en);
td_s32 ss_mpi_vo_get_video_layer_early_display(ot_vo_layer layer, td_bool *early_display_en);
td_s32 ss_mpi_vo_set_play_toleration(ot_vo_layer layer, td_u32 toleration);
td_s32 ss_mpi_vo_get_play_toleration(ot_vo_layer layer, td_u32 *toleration);
td_s32 ss_mpi_vo_batch_begin(ot_vo_layer layer);
td_s32 ss_mpi_vo_batch_end(ot_vo_layer layer);
td_s32 ss_mpi_vo_get_screen_frame(ot_vo_layer layer, ot_video_frame_info *frame_info, td_s32 milli_sec);
td_s32 ss_mpi_vo_release_screen_frame(ot_vo_layer layer, const ot_video_frame_info *frame_info);

td_s32 ss_mpi_vo_set_layer_hsharpen(ot_vo_layer layer, const ot_vo_layer_hsharpen_param *hsharpen_param);
td_s32 ss_mpi_vo_get_layer_hsharpen(ot_vo_layer layer, ot_vo_layer_hsharpen_param *hsharpen_param);

td_s32 ss_mpi_vo_set_layer_csc(ot_vo_layer layer, const ot_vo_csc *csc);
td_s32 ss_mpi_vo_get_layer_csc(ot_vo_layer layer, ot_vo_csc *csc);

td_s32 ss_mpi_vo_enable_chn(ot_vo_layer layer, ot_vo_chn chn);
td_s32 ss_mpi_vo_disable_chn(ot_vo_layer layer, ot_vo_chn chn);
td_s32 ss_mpi_vo_set_chn_attr(ot_vo_layer layer, ot_vo_chn chn, const ot_vo_chn_attr *chn_attr);
td_s32 ss_mpi_vo_get_chn_attr(ot_vo_layer layer, ot_vo_chn chn, ot_vo_chn_attr *chn_attr);
td_s32 ss_mpi_vo_set_chn_param(ot_vo_layer layer, ot_vo_chn chn, const ot_vo_chn_param *chn_param);
td_s32 ss_mpi_vo_get_chn_param(ot_vo_layer layer, ot_vo_chn chn, ot_vo_chn_param *chn_param);
td_s32 ss_mpi_vo_set_chn_display_pos(ot_vo_layer layer, ot_vo_chn chn, const ot_point *display_pos);
td_s32 ss_mpi_vo_get_chn_display_pos(ot_vo_layer layer, ot_vo_chn chn, ot_point *display_pos);
td_s32 ss_mpi_vo_set_chn_frame_rate(ot_vo_layer layer, ot_vo_chn chn, td_s32 frame_rate);
td_s32 ss_mpi_vo_get_chn_frame_rate(ot_vo_layer layer, ot_vo_chn chn, td_s32 *frame_rate);
td_s32 ss_mpi_vo_get_chn_frame(ot_vo_layer layer, ot_vo_chn chn, ot_video_frame_info *frame_info, td_s32 milli_sec);
td_s32 ss_mpi_vo_release_chn_frame(ot_vo_layer layer, ot_vo_chn chn, const ot_video_frame_info *frame_info);
td_s32 ss_mpi_vo_pause_chn(ot_vo_layer layer, ot_vo_chn chn);
td_s32 ss_mpi_vo_resume_chn(ot_vo_layer layer, ot_vo_chn chn);
td_s32 ss_mpi_vo_step_chn(ot_vo_layer layer, ot_vo_chn chn);
td_s32 ss_mpi_vo_refresh_chn(ot_vo_layer layer, ot_vo_chn chn);
td_s32 ss_mpi_vo_show_chn(ot_vo_layer layer, ot_vo_chn chn);
td_s32 ss_mpi_vo_hide_chn(ot_vo_layer layer, ot_vo_chn chn);
td_s32 ss_mpi_vo_set_zoom_in_window(ot_vo_layer layer, ot_vo_chn chn, const ot_vo_zoom_attr *zoom_attr);
td_s32 ss_mpi_vo_get_zoom_in_window(ot_vo_layer layer, ot_vo_chn chn, ot_vo_zoom_attr *zoom_attr);
td_s32 ss_mpi_vo_get_chn_pts(ot_vo_layer layer, ot_vo_chn chn, td_u64 *chn_pts);
td_s32 ss_mpi_vo_query_chn_status(ot_vo_layer layer, ot_vo_chn chn, ot_vo_chn_status *status);
td_s32 ss_mpi_vo_send_frame(ot_vo_layer layer, ot_vo_chn chn, const ot_video_frame_info *frame_info, td_s32 milli_sec);
td_s32 ss_mpi_vo_clear_chn_buf(ot_vo_layer layer, ot_vo_chn chn, td_bool is_clear_all);
td_s32 ss_mpi_vo_set_chn_border(ot_vo_layer layer, ot_vo_chn chn, const ot_vo_border *border);
td_s32 ss_mpi_vo_get_chn_border(ot_vo_layer layer, ot_vo_chn chn, ot_vo_border *border);
td_s32 ss_mpi_vo_set_chn_recv_threshold(ot_vo_layer layer, ot_vo_chn chn, td_u32 threshold);
td_s32 ss_mpi_vo_get_chn_recv_threshold(ot_vo_layer layer, ot_vo_chn chn, td_u32 *threshold);
td_s32 ss_mpi_vo_set_chn_rotation(ot_vo_layer layer, ot_vo_chn chn, ot_rotation rotation);
td_s32 ss_mpi_vo_get_chn_rotation(ot_vo_layer layer, ot_vo_chn chn, ot_rotation *rotation);
td_s32 ss_mpi_vo_set_chn_mirror(ot_vo_layer layer, ot_vo_chn chn, ot_vo_mirror_mode mirror_mode);
td_s32 ss_mpi_vo_get_chn_mirror(ot_vo_layer layer, ot_vo_chn chn, ot_vo_mirror_mode *mirror_mode);
td_s32 ss_mpi_vo_get_chn_rgn_luma(ot_vo_layer layer, ot_vo_chn chn, td_u32 num, const ot_rect *rgn, td_u64 *luma_data,
    td_s32 milli_sec);


#ifdef __cplusplus
}
#endif

#endif /* SS_MPI_VO_H */
