/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef __SS_MPI_UVC_H__
#define __SS_MPI_UVC_H__

#include "ot_common.h"
#include "ot_common_video.h"
#include "ot_common_venc.h"
#include "ot_common_uvc.h"

#ifdef __cplusplus
extern "C" {
#endif

td_s32 ss_mpi_uvc_create_chn(ot_uvc_chn uvc_chn, const ot_uvc_chn_attr *chn_attr);
td_s32 ss_mpi_uvc_destroy_chn(ot_uvc_chn uvc_chn);

td_s32 ss_mpi_uvc_start_chn(ot_uvc_chn uvc_chn);
td_s32 ss_mpi_uvc_stop_chn(ot_uvc_chn uvc_chn);

td_s32 ss_mpi_uvc_get_chn_attr(ot_uvc_chn uvc_chn, ot_uvc_chn_attr *chn_attr);
td_s32 ss_mpi_uvc_send_stream(ot_uvc_chn uvc_chn, ot_venc_chn venc_chn, const ot_venc_stream *stream, td_s32 milli_sec);
td_s32 ss_mpi_uvc_send_frame(ot_uvc_chn uvc_chn, const ot_video_frame_info *frame, td_s32 milli_sec);
td_s32 ss_mpi_uvc_send_user_stream(ot_uvc_chn uvc_chn, const ot_uvc_user_stream *user_stream, td_s32 milli_sec);

#ifdef __cplusplus
}
#endif

#endif /* __SS_MPI_UVC_H__ */
