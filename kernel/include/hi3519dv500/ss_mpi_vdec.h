/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */


#ifndef SS_MPI_VDEC_H
#define SS_MPI_VDEC_H

#include "ot_common.h"
#include "ot_common_video.h"
#include "ot_common_vdec.h"

#ifdef __cplusplus
extern "C" {
#endif

td_s32 ss_mpi_vdec_set_mod_param(const ot_vdec_mod_param *mod_param);
td_s32 ss_mpi_vdec_get_mod_param(ot_vdec_mod_param *mod_param);

td_s32 ss_mpi_vdec_create_chn(ot_vdec_chn chn, const ot_vdec_chn_attr *attr);
td_s32 ss_mpi_vdec_destroy_chn(ot_vdec_chn chn);
td_s32 ss_mpi_vdec_reset_chn(ot_vdec_chn chn);

td_s32 ss_mpi_vdec_get_chn_attr(ot_vdec_chn chn, ot_vdec_chn_attr *attr);
td_s32 ss_mpi_vdec_set_chn_attr(ot_vdec_chn chn, const ot_vdec_chn_attr *attr);

td_s32 ss_mpi_vdec_start_recv_stream(ot_vdec_chn chn);
td_s32 ss_mpi_vdec_stop_recv_stream(ot_vdec_chn chn);

td_s32 ss_mpi_vdec_query_status(ot_vdec_chn chn, ot_vdec_chn_status *status);

td_s32 ss_mpi_vdec_send_stream(ot_vdec_chn chn, const ot_vdec_stream *stream, td_s32 milli_sec);

td_s32 ss_mpi_vdec_get_frame(ot_vdec_chn chn, ot_video_frame_info *frame_info, ot_vdec_supplement_info *supplement,
    td_s32 milli_sec);
td_s32 ss_mpi_vdec_release_frame(ot_vdec_chn chn, const ot_video_frame_info *frame_info);

td_s32 ss_mpi_vdec_set_chn_param(ot_vdec_chn chn, const ot_vdec_chn_param *chn_param);
td_s32 ss_mpi_vdec_get_chn_param(ot_vdec_chn chn, ot_vdec_chn_param *chn_param);

td_s32 ss_mpi_vdec_set_protocol_param(ot_vdec_chn chn, const ot_vdec_protocol_param *protocol_param);
td_s32 ss_mpi_vdec_get_protocol_param(ot_vdec_chn chn, ot_vdec_protocol_param *protocol_param);

td_s32 ss_mpi_vdec_set_chn_config(ot_vdec_chn chn, const ot_vdec_chn_config *chn_config);
td_s32 ss_mpi_vdec_get_chn_config(ot_vdec_chn chn, ot_vdec_chn_config *chn_config);

td_s32 ss_mpi_vdec_get_user_data(ot_vdec_chn chn, ot_vdec_user_data *user_data, td_s32 milli_sec);
td_s32 ss_mpi_vdec_release_user_data(ot_vdec_chn chn, const ot_vdec_user_data *user_data);

td_s32 ss_mpi_vdec_set_user_pic(ot_vdec_chn chn, const ot_video_frame_info *user_pic);
td_s32 ss_mpi_vdec_enable_user_pic(ot_vdec_chn chn, td_bool instant);
td_s32 ss_mpi_vdec_disable_user_pic(ot_vdec_chn chn);

td_s32 ss_mpi_vdec_set_display_mode(ot_vdec_chn chn, ot_video_display_mode display_mode);
td_s32 ss_mpi_vdec_get_display_mode(ot_vdec_chn chn, ot_video_display_mode *display_mode);

td_s32 ss_mpi_vdec_set_rotation(ot_vdec_chn chn, ot_rotation rotation);
td_s32 ss_mpi_vdec_get_rotation(ot_vdec_chn chn, ot_rotation *rotation);

td_s32 ss_mpi_vdec_attach_vb_pool(ot_vdec_chn chn, const ot_vdec_chn_pool *pool);
td_s32 ss_mpi_vdec_detach_vb_pool(ot_vdec_chn chn);

td_s32 ss_mpi_vdec_get_fd(ot_vdec_chn chn);

td_s32 ss_mpi_vdec_set_low_delay(ot_vdec_chn chn, const ot_low_delay_info *low_delay_attr);
td_s32 ss_mpi_vdec_get_low_delay(ot_vdec_chn chn, ot_low_delay_info *low_delay_attr);

#ifdef __cplusplus
}
#endif

#endif /* end of #ifndef  SS_MPI_VDEC_H */
