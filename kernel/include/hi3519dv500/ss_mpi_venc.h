/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#ifndef SS_MPI_VENC_H
#define SS_MPI_VENC_H

#include "ot_common.h"
#include "ot_common_video.h"
#include "ot_common_venc.h"
#include "ot_common_vb.h"

#ifdef __cplusplus
extern "C" {
#endif

td_s32 ss_mpi_venc_create_chn(ot_venc_chn chn, const ot_venc_chn_attr *attr);
td_s32 ss_mpi_venc_destroy_chn(ot_venc_chn chn);
td_s32 ss_mpi_venc_reset_chn(ot_venc_chn chn);
td_s32 ss_mpi_venc_start_chn(ot_venc_chn chn, const ot_venc_start_param *recv_param);
td_s32 ss_mpi_venc_stop_chn(ot_venc_chn chn);

td_s32 ss_mpi_venc_set_chn_attr(ot_venc_chn chn, const ot_venc_chn_attr *chn_attr);
td_s32 ss_mpi_venc_get_chn_attr(ot_venc_chn chn, ot_venc_chn_attr *chn_attr);
td_s32 ss_mpi_venc_set_chn_param(ot_venc_chn chn, const ot_venc_chn_param *chn_param);
td_s32 ss_mpi_venc_get_chn_param(ot_venc_chn chn, ot_venc_chn_param *chn_param);
td_s32 ss_mpi_venc_query_status(ot_venc_chn chn, ot_venc_chn_status *status);
td_s32 ss_mpi_venc_get_stream(ot_venc_chn chn, ot_venc_stream *stream, td_s32 milli_sec);
td_s32 ss_mpi_venc_release_stream(ot_venc_chn chn, const ot_venc_stream *stream);
td_s32 ss_mpi_venc_get_stream_buf_info(ot_venc_chn chn, ot_venc_stream_buf_info *stream_buf_info);

td_s32 ss_mpi_venc_insert_user_data(ot_venc_chn chn, td_u8 *data, td_u32 len);
td_s32 ss_mpi_venc_send_frame(ot_venc_chn chn, const ot_video_frame_info *frame, td_s32 milli_sec);
td_s32 ss_mpi_venc_send_frame_ex(ot_venc_chn chn, const ot_venc_user_frame_info *frame, td_s32 milli_sec);
td_s32 ss_mpi_venc_send_multi_frame(ot_venc_chn chn, const ot_venc_multi_frame_info *frame, td_s32 milli_sec);

td_s32 ss_mpi_venc_set_chn_config(ot_venc_chn chn, const ot_venc_chn_config *chn_config);
td_s32 ss_mpi_venc_get_chn_config(ot_venc_chn chn, ot_venc_chn_config *chn_config);

td_s32 ss_mpi_venc_request_vi(ot_venc_chn chn);
td_s32 ss_mpi_venc_request_idr(ot_venc_chn chn, td_bool instant);
td_s32 ss_mpi_venc_enable_idr(ot_venc_chn chn, td_bool enable_idr);

td_s32 ss_mpi_venc_get_fd(ot_venc_chn chn);

td_s32 ss_mpi_venc_set_mod_param(const ot_venc_mod_param *mod_param);
td_s32 ss_mpi_venc_get_mod_param(ot_venc_mod_param *mod_param);

td_s32 ss_mpi_venc_set_roi_attr(ot_venc_chn chn, const ot_venc_roi_attr *roi_attr);
td_s32 ss_mpi_venc_get_roi_attr(ot_venc_chn chn, td_u32 idx, ot_venc_roi_attr *roi_attr);
td_s32 ss_mpi_venc_set_roi_attr_ex(ot_venc_chn chn, const ot_venc_roi_attr_ex *roi_attr_ex);
td_s32 ss_mpi_venc_get_roi_attr_ex(ot_venc_chn chn, td_u32 idx, ot_venc_roi_attr_ex *roi_attr_ex);
td_s32 ss_mpi_venc_set_jpeg_roi_attr(ot_venc_chn chn, const ot_venc_jpeg_roi_attr *roi_attr);
td_s32 ss_mpi_venc_get_jpeg_roi_attr(ot_venc_chn chn, td_u32 idx, ot_venc_jpeg_roi_attr *roi_attr);
td_s32 ss_mpi_venc_set_roi_bg_frame_rate(ot_venc_chn chn, const ot_venc_roi_bg_frame_rate *roi_bg_frame_rate);
td_s32 ss_mpi_venc_get_roi_bg_frame_rate(ot_venc_chn chn, ot_venc_roi_bg_frame_rate *roi_bg_frame_rate);

td_s32 ss_mpi_venc_set_h264_intra_pred(ot_venc_chn chn, const ot_venc_h264_intra_pred *h264_intra_pred);
td_s32 ss_mpi_venc_get_h264_intra_pred(ot_venc_chn chn, ot_venc_h264_intra_pred *h264_intra_pred);
td_s32 ss_mpi_venc_set_h264_trans(ot_venc_chn chn, const ot_venc_h264_trans *h264_trans);
td_s32 ss_mpi_venc_get_h264_trans(ot_venc_chn chn, ot_venc_h264_trans *h264_trans);
td_s32 ss_mpi_venc_set_h264_entropy(ot_venc_chn chn, const ot_venc_h264_entropy *h264_entropy);
td_s32 ss_mpi_venc_get_h264_entropy(ot_venc_chn chn, ot_venc_h264_entropy *h264_entropy);
td_s32 ss_mpi_venc_set_h264_dblk(ot_venc_chn chn, const ot_venc_h264_dblk *h264_dblk);
td_s32 ss_mpi_venc_get_h264_dblk(ot_venc_chn chn, ot_venc_h264_dblk *h264_dblk);
td_s32 ss_mpi_venc_set_h264_vui(ot_venc_chn chn, const ot_venc_h264_vui *h264_vui);
td_s32 ss_mpi_venc_get_h264_vui(ot_venc_chn chn, ot_venc_h264_vui *h264_vui);
td_s32 ss_mpi_venc_set_h264_poc(ot_venc_chn chn, const ot_venc_h264_poc *h264_poc);
td_s32 ss_mpi_venc_get_h264_poc(ot_venc_chn chn, ot_venc_h264_poc *h264_poc);

td_s32 ss_mpi_venc_set_h265_vui(ot_venc_chn chn, const ot_venc_h265_vui *h265_vui);
td_s32 ss_mpi_venc_get_h265_vui(ot_venc_chn chn, ot_venc_h265_vui *h265_vui);
td_s32 ss_mpi_venc_set_h265_pu(ot_venc_chn chn, const ot_venc_h265_pu *pu);
td_s32 ss_mpi_venc_get_h265_pu(ot_venc_chn chn, ot_venc_h265_pu *pu);
td_s32 ss_mpi_venc_set_h265_trans(ot_venc_chn chn, const ot_venc_h265_trans *h265_trans);
td_s32 ss_mpi_venc_get_h265_trans(ot_venc_chn chn, ot_venc_h265_trans *h265_trans);
td_s32 ss_mpi_venc_set_h265_entropy(ot_venc_chn chn, const ot_venc_h265_entropy *h265_entropy);
td_s32 ss_mpi_venc_get_h265_entropy(ot_venc_chn chn, ot_venc_h265_entropy *h265_entropy);
td_s32 ss_mpi_venc_set_h265_dblk(ot_venc_chn chn, const ot_venc_h265_dblk *h265_dblk);
td_s32 ss_mpi_venc_get_h265_dblk(ot_venc_chn chn, ot_venc_h265_dblk *h265_dblk);
td_s32 ss_mpi_venc_set_h265_sao(ot_venc_chn chn, const ot_venc_h265_sao *h265_sao);
td_s32 ss_mpi_venc_get_h265_sao(ot_venc_chn chn, ot_venc_h265_sao *h265_sao);

td_s32 ss_mpi_venc_set_jpeg_param(ot_venc_chn chn, const ot_venc_jpeg_param *jpeg_param);
td_s32 ss_mpi_venc_get_jpeg_param(ot_venc_chn chn, ot_venc_jpeg_param *jpeg_param);
td_s32 ss_mpi_venc_set_mjpeg_param(ot_venc_chn chn, const ot_venc_mjpeg_param *mjpeg_param);
td_s32 ss_mpi_venc_get_mjpeg_param(ot_venc_chn chn, ot_venc_mjpeg_param *mjpeg_param);
td_s32 ss_mpi_venc_set_jpeg_dering_attr(ot_venc_chn chn, const ot_venc_jpeg_dering_attr dering_attr);
td_s32 ss_mpi_venc_get_jpeg_dering_attr(ot_venc_chn chn, ot_venc_jpeg_dering_attr *dering_attr);
td_s32 ss_mpi_venc_set_jpeg_enc_mode(ot_venc_chn chn, const ot_venc_jpeg_enc_mode enc_mode);
td_s32 ss_mpi_venc_get_jpeg_enc_mode(ot_venc_chn chn, ot_venc_jpeg_enc_mode *enc_mode);

td_s32 ss_mpi_venc_set_rc_param(ot_venc_chn chn, const ot_venc_rc_param *rc_param);
td_s32 ss_mpi_venc_get_rc_param(ot_venc_chn chn, ot_venc_rc_param *rc_param);
td_s32 ss_mpi_venc_set_rc_adv_param(ot_venc_chn chn, const ot_venc_rc_adv_param *rc_adv_param);
td_s32 ss_mpi_venc_get_rc_adv_param(ot_venc_chn chn, ot_venc_rc_adv_param *rc_adv_param);
td_s32 ss_mpi_venc_set_frame_lost_strategy(ot_venc_chn chn, const ot_venc_frame_lost_strategy *lost_param);
td_s32 ss_mpi_venc_get_frame_lost_strategy(ot_venc_chn chn, ot_venc_frame_lost_strategy *lost_param);
td_s32 ss_mpi_venc_set_super_frame_strategy(ot_venc_chn chn, const ot_venc_super_frame_strategy *super_frame_param);
td_s32 ss_mpi_venc_get_super_frame_strategy(ot_venc_chn chn, ot_venc_super_frame_strategy *super_frame_param);
td_s32 ss_mpi_venc_set_scene_mode(ot_venc_chn chn, const ot_venc_scene_mode scene_mode);
td_s32 ss_mpi_venc_get_scene_mode(ot_venc_chn chn, ot_venc_scene_mode *scene_mode);

td_s32 ss_mpi_venc_attach_vb_pool(ot_venc_chn chn, const ot_venc_chn_pool *pool);
td_s32 ss_mpi_venc_detach_vb_pool(ot_venc_chn chn);

td_s32 ss_mpi_venc_set_ref_param(ot_venc_chn chn, const ot_venc_ref_param *ref_param);
td_s32 ss_mpi_venc_get_ref_param(ot_venc_chn chn, ot_venc_ref_param *ref_param);
td_s32 ss_mpi_venc_set_intra_refresh(ot_venc_chn chn, const ot_venc_intra_refresh *intra_refresh);
td_s32 ss_mpi_venc_get_intra_refresh(ot_venc_chn chn, ot_venc_intra_refresh *intra_refresh);
td_s32 ss_mpi_venc_set_sse_rgn(ot_venc_chn chn, const ot_venc_sse_rgn *sse_cfg);
td_s32 ss_mpi_venc_get_sse_rgn(ot_venc_chn chn, td_u32 idx, ot_venc_sse_rgn *sse_cfg);
td_s32 ss_mpi_venc_set_fg_protect(ot_venc_chn chn, const ot_venc_fg_protect *fg_protect);
td_s32 ss_mpi_venc_get_fg_protect(ot_venc_chn chn, ot_venc_fg_protect *fg_protect);
td_s32 ss_mpi_venc_set_cu_pred(ot_venc_chn chn, const ot_venc_cu_pred *cu_pred);
td_s32 ss_mpi_venc_get_cu_pred(ot_venc_chn chn, ot_venc_cu_pred *cu_pred);
td_s32 ss_mpi_venc_set_skip_bias(ot_venc_chn chn, const ot_venc_skip_bias *skip_bias);
td_s32 ss_mpi_venc_get_skip_bias(ot_venc_chn chn, ot_venc_skip_bias *skip_bias);
td_s32 ss_mpi_venc_set_debreath_effect(ot_venc_chn chn, const ot_venc_debreath_effect *debreath_effect);
td_s32 ss_mpi_venc_get_debreath_effect(ot_venc_chn chn, ot_venc_debreath_effect *debreath_effect);
td_s32 ss_mpi_venc_set_hierarchical_qp(ot_venc_chn chn, const ot_venc_hierarchical_qp *hierarchical_qp);
td_s32 ss_mpi_venc_get_hierarchical_qp(ot_venc_chn chn, ot_venc_hierarchical_qp *hierarchical_qp);
td_s32 ss_mpi_venc_set_slice_split(ot_venc_chn chn, const ot_venc_slice_split *slice_split);
td_s32 ss_mpi_venc_get_slice_split(ot_venc_chn chn, ot_venc_slice_split *slice_split);
td_s32 ss_mpi_venc_set_param_set_id(ot_venc_chn chn, const ot_venc_param_set_id *param_set_id);
td_s32 ss_mpi_venc_get_param_set_id(ot_venc_chn chn, ot_venc_param_set_id *param_set_id);
td_s32 ss_mpi_venc_set_search_window(ot_venc_chn chn, const ot_venc_search_window *search_window);
td_s32 ss_mpi_venc_get_search_window(ot_venc_chn chn, ot_venc_search_window *search_window);

td_s32 ss_mpi_venc_enable_svc(ot_venc_chn chn, td_bool enable);
td_s32 ss_mpi_venc_set_svc_param(ot_venc_chn chn, const ot_venc_svc_param *svc_param);
td_s32 ss_mpi_venc_get_svc_param(ot_venc_chn chn, ot_venc_svc_param *svc_param);
td_s32 ss_mpi_venc_send_svc_region(ot_venc_chn chn, const ot_venc_svc_rect_info *svc_region);

td_s32 ss_mpi_venc_set_deblur(ot_venc_chn chn, const ot_venc_deblur_param *deblur_param);
td_s32 ss_mpi_venc_get_deblur(ot_venc_chn chn, ot_venc_deblur_param *deblur_param);
td_s32 ss_mpi_venc_set_adv_deblur(ot_venc_chn chn, const ot_venc_adv_deblur *adv_deblur);
td_s32 ss_mpi_venc_get_adv_deblur(ot_venc_chn chn, ot_venc_adv_deblur *adv_deblur);
td_s32 ss_mpi_venc_set_md(ot_venc_chn chn, const ot_venc_md_param *md_param);
td_s32 ss_mpi_venc_get_md(ot_venc_chn chn, ot_venc_md_param *md_param);
td_s32 ss_mpi_venc_omx_send_frame(ot_venc_chn chn, const ot_omx_user_frame_info *frame, td_s32 milli_sec);
td_s32 ss_mpi_venc_set_quality_balance(ot_venc_chn chn, const ot_venc_quality_balance *quality_balance);
td_s32 ss_mpi_venc_get_quality_balance(ot_venc_chn chn, ot_venc_quality_balance *quality_balance);
td_s32 ss_mpi_venc_set_watermark(ot_venc_chn chn, const ot_venc_watermark *watermark);
td_s32 ss_mpi_venc_get_watermark(ot_venc_chn chn, ot_venc_watermark *watermark);
td_s32 ss_mpi_venc_set_watermark_roi(ot_venc_chn chn, const ot_venc_watermark_roi *watermark_roi);
td_s32 ss_mpi_venc_get_watermark_roi(ot_venc_chn chn, ot_venc_watermark_roi *watermark_roi);
td_s32 ss_mpi_venc_enable_jpeg_dblk(ot_venc_chn chn, td_bool enable);

#ifdef __cplusplus
}
#endif

#endif /* MPI_VENC_H */
