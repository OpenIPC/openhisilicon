/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_AUDIO_H
#define SS_MPI_AUDIO_H

#include "ot_type.h"
#include "ot_common.h"
#include "ot_common_aio.h"
#include "ot_common_aenc.h"
#include "ot_common_adec.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OT_AENC_ADAPT_MAGIC 0Xfcfcfcfc

/* Audio function api. */
td_s32 ss_mpi_audio_init(td_void);
td_s32 ss_mpi_audio_exit(td_void);

td_s32 ss_mpi_audio_set_mod_param(const ot_audio_mod_param *mod_param);
td_s32 ss_mpi_audio_get_mod_param(ot_audio_mod_param *mod_param);

td_s32 ss_mpi_audio_register_vqe_mod(const ot_audio_vqe_register *vqe_register);

/* AI function api. */
td_s32 ss_mpi_ai_set_pub_attr(ot_audio_dev ai_dev, const ot_aio_attr *attr);
td_s32 ss_mpi_ai_get_pub_attr(ot_audio_dev ai_dev, ot_aio_attr *attr);
td_s32 ss_mpi_ai_clr_pub_attr(ot_audio_dev ai_dev);

td_s32 ss_mpi_ai_enable(ot_audio_dev ai_dev);
td_s32 ss_mpi_ai_disable(ot_audio_dev ai_dev);

td_s32 ss_mpi_ai_set_dmic_gain(ot_audio_dev ai_dev, td_s32 gain);
td_s32 ss_mpi_ai_get_dmic_gain(ot_audio_dev ai_dev, td_s32 *gain);

td_s32 ss_mpi_ai_set_chn_attr(ot_audio_dev ai_dev, ot_ai_chn ai_chn, const ot_ai_chn_attr *chn_attr);
td_s32 ss_mpi_ai_get_chn_attr(ot_audio_dev ai_dev, ot_ai_chn ai_chn, ot_ai_chn_attr *chn_attr);

td_s32 ss_mpi_ai_set_chn_param(ot_audio_dev ai_dev, ot_ai_chn ai_chn, const ot_ai_chn_param *chn_param);
td_s32 ss_mpi_ai_get_chn_param(ot_audio_dev ai_dev, ot_ai_chn ai_chn, ot_ai_chn_param *chn_param);

td_s32 ss_mpi_ai_enable_chn(ot_audio_dev ai_dev, ot_ai_chn ai_chn);
td_s32 ss_mpi_ai_disable_chn(ot_audio_dev ai_dev, ot_ai_chn ai_chn);

td_s32 ss_mpi_ai_get_frame(ot_audio_dev ai_dev, ot_ai_chn ai_chn, ot_audio_frame *frame, ot_aec_frame *aec_frame,
    td_s32 milli_sec);
td_s32 ss_mpi_ai_release_frame(ot_audio_dev ai_dev, ot_ai_chn ai_chn, const ot_audio_frame *frame,
    const ot_aec_frame *aec_frame);

td_s32 ss_mpi_ai_get_raw_frame(ot_audio_dev ai_dev, ot_ai_chn ai_chn, ot_audio_frame *frame,
    ot_aec_frame *aec_frame, td_s32 milli_sec);
td_s32 ss_mpi_ai_release_raw_frame(ot_audio_dev ai_dev, ot_ai_chn ai_chn, const ot_audio_frame *frame,
    const ot_aec_frame *aec_frame);

td_s32 ss_mpi_ai_set_talk_vqe_v2_attr(ot_audio_dev ai_dev, ot_ai_chn ai_chn, ot_audio_dev ao_dev, ot_ao_chn ao_chn,
    const ot_ai_talk_vqe_v2_cfg *vqe_cfg);
td_s32 ss_mpi_ai_get_talk_vqe_v2_attr(ot_audio_dev ai_dev, ot_ai_chn ai_chn, ot_ai_talk_vqe_v2_cfg *vqe_cfg);

td_s32 ss_mpi_ai_set_record_vqe_attr(ot_audio_dev ai_dev, ot_ai_chn ai_chn, const ot_ai_record_vqe_cfg *vqe_cfg);
td_s32 ss_mpi_ai_get_record_vqe_attr(ot_audio_dev ai_dev, ot_ai_chn ai_chn, ot_ai_record_vqe_cfg *vqe_cfg);

td_s32 ss_mpi_ai_set_talk_vqe_attr(ot_audio_dev ai_dev, ot_ai_chn ai_chn, ot_audio_dev ao_dev, ot_ao_chn ao_chn,
    const ot_ai_talk_vqe_cfg *vqe_cfg);
td_s32 ss_mpi_ai_get_talk_vqe_attr(ot_audio_dev ai_dev, ot_ai_chn ai_chn, ot_ai_talk_vqe_cfg *vqe_cfg);

td_s32 ss_mpi_ai_enable_vqe(ot_audio_dev ai_dev, ot_ai_chn ai_chn);
td_s32 ss_mpi_ai_disable_vqe(ot_audio_dev ai_dev, ot_ai_chn ai_chn);

td_s32 ss_mpi_ai_enable_resample(ot_audio_dev ai_dev, ot_ai_chn ai_chn, ot_audio_sample_rate out_sample_rate);
td_s32 ss_mpi_ai_disable_resample(ot_audio_dev ai_dev, ot_ai_chn ai_chn);

td_s32 ss_mpi_ai_set_track_mode(ot_audio_dev ai_dev, ot_audio_track_mode track_mode);
td_s32 ss_mpi_ai_get_track_mode(ot_audio_dev ai_dev, ot_audio_track_mode *track_mode);

td_s32 ss_mpi_ai_save_file(ot_audio_dev ai_dev, ot_ai_chn ai_chn, const ot_audio_save_file_info *save_file_info);
td_s32 ss_mpi_ai_query_file_status(ot_audio_dev ai_dev, ot_ai_chn ai_chn, ot_audio_file_status *file_status);

td_s32 ss_mpi_ai_get_fd(ot_audio_dev ai_dev, ot_ai_chn ai_chn);

td_s32 ss_mpi_ai_enable_aec_ref_frame(ot_audio_dev ai_dev, ot_ai_chn ai_chn, ot_audio_dev ao_dev, ot_ao_chn ao_chn);
td_s32 ss_mpi_ai_disable_aec_ref_frame(ot_audio_dev ai_dev, ot_ai_chn ai_chn);

/* AO function api. */
td_s32 ss_mpi_ao_set_pub_attr(ot_audio_dev ao_dev, const ot_aio_attr *attr);
td_s32 ss_mpi_ao_get_pub_attr(ot_audio_dev ao_dev, ot_aio_attr *attr);
td_s32 ss_mpi_ao_clr_pub_attr(ot_audio_dev ao_dev);

td_s32 ss_mpi_ao_enable(ot_audio_dev ao_dev);
td_s32 ss_mpi_ao_disable(ot_audio_dev ao_dev);

td_s32 ss_mpi_ao_enable_chn(ot_audio_dev ao_dev, ot_ao_chn ao_chn);
td_s32 ss_mpi_ao_disable_chn(ot_audio_dev ao_dev, ot_ao_chn ao_chn);

td_s32 ss_mpi_ao_pause_chn(ot_audio_dev ao_dev, ot_ao_chn ao_chn);
td_s32 ss_mpi_ao_resume_chn(ot_audio_dev ao_dev, ot_ao_chn ao_chn);

td_s32 ss_mpi_ao_send_frame(ot_audio_dev ao_dev, ot_ao_chn ao_chn, const ot_audio_frame *data, td_s32 milli_sec);

td_s32 ss_mpi_ao_enable_resample(ot_audio_dev ao_dev, ot_ao_chn ao_chn, ot_audio_sample_rate in_sample_rate);
td_s32 ss_mpi_ao_disable_resample(ot_audio_dev ao_dev, ot_ao_chn ao_chn);

td_s32 ss_mpi_ao_clr_chn_buf(ot_audio_dev ao_dev, ot_ao_chn ao_chn);
td_s32 ss_mpi_ao_query_chn_status(ot_audio_dev ao_dev, ot_ao_chn ao_chn, ot_ao_chn_state *status);

td_s32 ss_mpi_ao_get_chn_delay(ot_audio_dev ao_dev, ot_ao_chn ao_chn, td_u32 *milli_sec);

td_s32 ss_mpi_ao_set_volume(ot_audio_dev ao_dev, td_s32 volume_db);
td_s32 ss_mpi_ao_get_volume(ot_audio_dev ao_dev, td_s32 *volume_db);

td_s32 ss_mpi_ao_set_mute(ot_audio_dev ao_dev, td_bool enable, const ot_audio_fade *fade);
td_s32 ss_mpi_ao_get_mute(ot_audio_dev ao_dev, td_bool *enable, ot_audio_fade *fade);

td_s32 ss_mpi_ao_set_track_mode(ot_audio_dev ao_dev, ot_audio_track_mode track_mode);
td_s32 ss_mpi_ao_get_track_mode(ot_audio_dev ao_dev, ot_audio_track_mode *track_mode);

td_s32 ss_mpi_ao_get_fd(ot_audio_dev ao_dev, ot_ao_chn ao_chn);

td_s32 ss_mpi_ao_set_vqe_attr(ot_audio_dev ao_dev, ot_ao_chn ao_chn, const ot_ao_vqe_cfg *vqe_cfg);
td_s32 ss_mpi_ao_get_vqe_attr(ot_audio_dev ao_dev, ot_ao_chn ao_chn, ot_ao_vqe_cfg *vqe_cfg);

td_s32 ss_mpi_ao_enable_vqe(ot_audio_dev ao_dev, ot_ao_chn ao_chn);
td_s32 ss_mpi_ao_disable_vqe(ot_audio_dev ao_dev, ot_ao_chn ao_chn);

td_s32 ss_mpi_ao_save_file(ot_audio_dev ao_dev, ot_ao_chn ao_chn, const ot_audio_save_file_info *save_file_info);
td_s32 ss_mpi_ao_query_file_status(ot_audio_dev ao_dev, ot_ao_chn ao_chn, ot_audio_file_status *file_status);

/* AENC function api. */
td_s32 ss_mpi_aenc_register_encoder(td_s32 *handle, const ot_aenc_encoder *encoder);
td_s32 ss_mpi_aenc_unregister_encoder(td_s32 handle);

td_s32 ss_mpi_aenc_create_chn(ot_aenc_chn aenc_chn, const ot_aenc_chn_attr *attr);
td_s32 ss_mpi_aenc_destroy_chn(ot_aenc_chn aenc_chn);

td_s32 ss_mpi_aenc_send_frame(ot_aenc_chn aenc_chn, const ot_audio_frame *frame, const ot_aec_frame *aec_frame);

td_s32 ss_mpi_aenc_get_stream(ot_aenc_chn aenc_chn, ot_audio_stream *stream, td_s32 milli_sec);
td_s32 ss_mpi_aenc_release_stream(ot_aenc_chn aenc_chn, const ot_audio_stream *stream);

td_s32 ss_mpi_aenc_get_fd(ot_aenc_chn aenc_chn);

td_s32 ss_mpi_aenc_get_stream_buf_info(ot_aenc_chn aenc_chn, td_phys_addr_t *phys_addr, td_u32 *size);

td_s32 ss_mpi_aenc_set_mute(ot_aenc_chn aenc_chn, td_bool enable);
td_s32 ss_mpi_aenc_get_mute(ot_aenc_chn aenc_chn, td_bool *enable);

td_s32 ss_mpi_aenc_clr_chn_buf(ot_aenc_chn aenc_chn);

/* ADEC function api. */
td_s32 ss_mpi_adec_register_decoder(td_s32 *handle, const ot_adec_decoder *decoder);
td_s32 ss_mpi_adec_unregister_decoder(td_s32 handle);

td_s32 ss_mpi_adec_create_chn(ot_adec_chn adec_chn, const ot_adec_chn_attr *attr);
td_s32 ss_mpi_adec_destroy_chn(ot_adec_chn adec_chn);

td_s32 ss_mpi_adec_send_stream(ot_adec_chn adec_chn, const ot_audio_stream *stream, td_bool block);

td_s32 ss_mpi_adec_clr_chn_buf(ot_adec_chn adec_chn);

td_s32 ss_mpi_adec_get_frame(ot_adec_chn adec_chn, ot_audio_frame_info *frame_info, td_bool block);
td_s32 ss_mpi_adec_release_frame(ot_adec_chn adec_chn, const ot_audio_frame_info *frame_info);

td_s32 ss_mpi_adec_send_end_of_stream(ot_adec_chn adec_chn, td_bool instant);
td_s32 ss_mpi_adec_query_chn_status(ot_adec_chn adec_chn, ot_adec_chn_state *buf_status);

td_s32 ss_mpi_adec_get_frame_info(ot_adec_chn adec_chn, td_void *decode_frame_info);

#ifdef __cplusplus
}
#endif

#endif /* SS_MPI_AI_H */
