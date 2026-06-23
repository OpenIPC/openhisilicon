/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_VI_H
#define SS_MPI_VI_H

#include "ot_common_vi.h"
#include "ot_common_gdc.h"
#include "ot_common_dis.h"
#include "ot_common_vb.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* device interface */
td_s32 ss_mpi_vi_set_dev_attr(ot_vi_dev vi_dev, const ot_vi_dev_attr *dev_attr);
td_s32 ss_mpi_vi_get_dev_attr(ot_vi_dev vi_dev, ot_vi_dev_attr *dev_attr);

td_s32 ss_mpi_vi_set_dev_irq_affinity(ot_vi_dev vi_dev, td_u32 irq_index);
td_s32 ss_mpi_vi_get_dev_irq_affinity(ot_vi_dev vi_dev, td_u32 *irq_index);

td_s32 ss_mpi_vi_enable_dev(ot_vi_dev vi_dev);
td_s32 ss_mpi_vi_disable_dev(ot_vi_dev vi_dev);

td_s32 ss_mpi_vi_set_thermo_sns_attr(ot_vi_dev vi_dev, const ot_vi_thermo_sns_attr *sns_attr);
td_s32 ss_mpi_vi_get_thermo_sns_attr(ot_vi_dev vi_dev, ot_vi_thermo_sns_attr *sns_attr);

td_s32 ss_mpi_vi_enable_dev_send_frame(ot_vi_dev vi_dev);
td_s32 ss_mpi_vi_disable_dev_send_frame(ot_vi_dev vi_dev);
td_s32 ss_mpi_vi_send_dev_frame(ot_vi_dev vi_dev, const ot_video_frame_info *frame_info, td_s32 milli_sec);

td_s32 ss_mpi_vi_set_dev_timing_attr(ot_vi_dev vi_dev, const ot_vi_dev_timing_attr *timing_attr);
td_s32 ss_mpi_vi_get_dev_timing_attr(ot_vi_dev vi_dev, ot_vi_dev_timing_attr *timing_attr);

td_s32 ss_mpi_vi_set_dev_data_attr(ot_vi_dev vi_dev, const ot_vi_dev_data_attr *data_attr);
td_s32 ss_mpi_vi_get_dev_data_attr(ot_vi_dev vi_dev, ot_vi_dev_data_attr *data_attr);

/* vi dev bind pipe interface */
td_s32 ss_mpi_vi_bind(ot_vi_dev vi_dev, ot_vi_pipe vi_pipe);
td_s32 ss_mpi_vi_unbind(ot_vi_dev vi_dev, ot_vi_pipe vi_pipe);
td_s32 ss_mpi_vi_get_bind_by_dev(ot_vi_dev vi_dev, ot_vi_bind_pipe *bind_pipe);
td_s32 ss_mpi_vi_get_bind_by_pipe(ot_vi_pipe vi_pipe, ot_vi_dev *vi_dev);

/* WDR fusion group */
td_s32 ss_mpi_vi_set_wdr_fusion_grp_attr(ot_vi_grp fusion_grp, const ot_vi_wdr_fusion_grp_attr *grp_attr);
td_s32 ss_mpi_vi_get_wdr_fusion_grp_attr(ot_vi_grp fusion_grp, ot_vi_wdr_fusion_grp_attr *grp_attr);

/* pipe interface */
td_s32 ss_mpi_vi_create_pipe(ot_vi_pipe vi_pipe, const ot_vi_pipe_attr *pipe_attr);
td_s32 ss_mpi_vi_destroy_pipe(ot_vi_pipe vi_pipe);

td_s32 ss_mpi_vi_set_pipe_attr(ot_vi_pipe vi_pipe, const ot_vi_pipe_attr *pipe_attr);
td_s32 ss_mpi_vi_get_pipe_attr(ot_vi_pipe vi_pipe, ot_vi_pipe_attr *pipe_attr);

td_s32 ss_mpi_vi_start_pipe(ot_vi_pipe vi_pipe);
td_s32 ss_mpi_vi_stop_pipe(ot_vi_pipe vi_pipe);

td_s32 ss_mpi_vi_set_pipe_3dnr_attr(ot_vi_pipe vi_pipe, const ot_3dnr_attr *attr);
td_s32 ss_mpi_vi_get_pipe_3dnr_attr(ot_vi_pipe vi_pipe, ot_3dnr_attr *attr);

td_s32 ss_mpi_vi_set_pipe_3dnr_param(ot_vi_pipe vi_pipe, const ot_3dnr_param *param);
td_s32 ss_mpi_vi_get_pipe_3dnr_param(ot_vi_pipe vi_pipe, ot_3dnr_param *param);

td_s32 ss_mpi_vi_set_pipe_pre_crop(ot_vi_pipe vi_pipe, const ot_crop_info *crop_info);
td_s32 ss_mpi_vi_get_pipe_pre_crop(ot_vi_pipe vi_pipe, ot_crop_info *crop_info);

td_s32 ss_mpi_vi_set_pipe_post_crop(ot_vi_pipe vi_pipe, const ot_crop_info *crop_info);
td_s32 ss_mpi_vi_get_pipe_post_crop(ot_vi_pipe vi_pipe, ot_crop_info *crop_info);

td_s32 ss_mpi_vi_set_pipe_frame_dump_attr(ot_vi_pipe vi_pipe, const ot_vi_frame_dump_attr *dump_attr);
td_s32 ss_mpi_vi_get_pipe_frame_dump_attr(ot_vi_pipe vi_pipe, ot_vi_frame_dump_attr *dump_attr);

td_s32 ss_mpi_vi_get_pipe_frame(ot_vi_pipe vi_pipe, ot_video_frame_info *frame_info, td_s32 milli_sec);
td_s32 ss_mpi_vi_release_pipe_frame(ot_vi_pipe vi_pipe, const ot_video_frame_info *frame_info);

td_s32 ss_mpi_vi_set_pipe_private_data_dump_attr(ot_vi_pipe vi_pipe, const ot_vi_private_data_dump_attr *dump_attr);
td_s32 ss_mpi_vi_get_pipe_private_data_dump_attr(ot_vi_pipe vi_pipe, ot_vi_private_data_dump_attr *dump_attr);

td_s32 ss_mpi_vi_get_pipe_private_data(ot_vi_pipe vi_pipe, ot_vi_private_data_info *data_info, td_s32 milli_sec);
td_s32 ss_mpi_vi_release_pipe_private_data(ot_vi_pipe vi_pipe, const ot_vi_private_data_info *data_info);

td_s32 ss_mpi_vi_set_pipe_param(ot_vi_pipe vi_pipe, const ot_vi_pipe_param *pipe_param);
td_s32 ss_mpi_vi_get_pipe_param(ot_vi_pipe vi_pipe, ot_vi_pipe_param *pipe_param);

td_s32 ss_mpi_vi_set_pipe_bnr_buf_num(ot_vi_pipe vi_pipe, td_u32 buf_num);
td_s32 ss_mpi_vi_get_pipe_bnr_buf_num(ot_vi_pipe vi_pipe, td_u32 *buf_num);

td_s32 ss_mpi_vi_set_pipe_frame_source(ot_vi_pipe vi_pipe, const ot_vi_pipe_frame_source frame_source);
td_s32 ss_mpi_vi_get_pipe_frame_source(ot_vi_pipe vi_pipe, ot_vi_pipe_frame_source *frame_source);

td_s32 ss_mpi_vi_send_pipe_yuv(ot_vi_pipe vi_pipe, const ot_video_frame_info *frame_info, td_s32 milli_sec);
td_s32 ss_mpi_vi_send_pipe_raw(ot_vi_pipe vi_pipe, const ot_video_frame_info *frame_info[], td_u32 frame_num,
    td_s32 milli_sec);

td_s32 ss_mpi_vi_query_pipe_status(ot_vi_pipe vi_pipe, ot_vi_pipe_status *pipe_status);

td_s32 ss_mpi_vi_enable_pipe_interrupt(ot_vi_pipe vi_pipe);
td_s32 ss_mpi_vi_disable_pipe_interrupt(ot_vi_pipe vi_pipe);

td_s32 ss_mpi_vi_set_pipe_vc_number(ot_vi_pipe vi_pipe, td_u32 vc_number);
td_s32 ss_mpi_vi_get_pipe_vc_number(ot_vi_pipe vi_pipe, td_u32 *vc_number);

td_s32 ss_mpi_vi_set_pipe_low_delay(ot_vi_pipe vi_pipe, const ot_low_delay_info *low_delay_info);
td_s32 ss_mpi_vi_get_pipe_low_delay(ot_vi_pipe vi_pipe, ot_low_delay_info *low_delay_info);

td_s32 ss_mpi_vi_set_pipe_frame_interrupt_attr(ot_vi_pipe vi_pipe, const ot_frame_interrupt_attr *interrupt_attr);
td_s32 ss_mpi_vi_get_pipe_frame_interrupt_attr(ot_vi_pipe vi_pipe, ot_frame_interrupt_attr *interrupt_attr);

td_s32 ss_mpi_vi_get_pipe_compress_param(ot_vi_pipe vi_pipe, ot_vi_compress_param *compress_param);

td_s32 ss_mpi_vi_set_pipe_user_pic(ot_vi_pipe vi_pipe, const ot_video_frame_info *user_pic_info);
td_s32 ss_mpi_vi_enable_pipe_user_pic(ot_vi_pipe vi_pipe);
td_s32 ss_mpi_vi_disable_pipe_user_pic(ot_vi_pipe vi_pipe);

td_s32 ss_mpi_vi_set_pipe_vb_src(ot_vi_pipe vi_pipe, ot_vb_src vb_src);
td_s32 ss_mpi_vi_get_pipe_vb_src(ot_vi_pipe vi_pipe, ot_vb_src *vb_src);

td_s32 ss_mpi_vi_attach_pipe_vb_pool(ot_vi_pipe vi_pipe, ot_vb_pool vb_pool);
td_s32 ss_mpi_vi_detach_pipe_vb_pool(ot_vi_pipe vi_pipe);

td_s32 ss_mpi_vi_attach_aiisp_vb_pool(ot_vi_pipe vi_pipe, ot_aiisp_pool *pool_attr);
td_s32 ss_mpi_vi_detach_aiisp_vb_pool(ot_vi_pipe vi_pipe, ot_aiisp_type aiisp_type);

td_s32 ss_mpi_vi_get_pipe_fd(ot_vi_pipe vi_pipe);

/* channel interface */
td_s32 ss_mpi_vi_set_chn_attr(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, const ot_vi_chn_attr *chn_attr);
td_s32 ss_mpi_vi_get_chn_attr(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_vi_chn_attr *chn_attr);

td_s32 ss_mpi_vi_enable_chn(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn);
td_s32 ss_mpi_vi_disable_chn(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn);

td_s32 ss_mpi_vi_set_chn_crop(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, const ot_vi_crop_info *crop_info);
td_s32 ss_mpi_vi_get_chn_crop(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_vi_crop_info *crop_info);

td_s32 ss_mpi_vi_set_chn_dis_cfg(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, const ot_dis_cfg *dis_cfg);
td_s32 ss_mpi_vi_get_chn_dis_cfg(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_dis_cfg *dis_cfg);
td_s32 ss_mpi_vi_set_chn_dis_attr(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, const ot_dis_attr *dis_attr);
td_s32 ss_mpi_vi_get_chn_dis_attr(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_dis_attr *dis_attr);
td_s32 ss_mpi_vi_get_chn_dis_out_size(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_size *out_size);
td_s32 ss_mpi_vi_set_chn_dis_param(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, const ot_dis_param *dis_param);
td_s32 ss_mpi_vi_get_chn_dis_param(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_dis_param *dis_param);
td_s32 ss_mpi_vi_set_chn_dis_alg_attr(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, const ot_dis_alg_attr *dis_alg_attr);
td_s32 ss_mpi_vi_get_chn_dis_alg_attr(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_dis_alg_attr *dis_alg_attr);

td_s32 ss_mpi_vi_get_chn_frame(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_video_frame_info *frame_info,
    td_s32 milli_sec);
td_s32 ss_mpi_vi_release_chn_frame(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, const ot_video_frame_info *frame_info);

td_s32 ss_mpi_vi_set_chn_low_delay(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, const ot_low_delay_info *low_delay_info);
td_s32 ss_mpi_vi_get_chn_low_delay(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_low_delay_info *low_delay_info);

td_s32 ss_mpi_vi_set_chn_align(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, td_u32 align);
td_s32 ss_mpi_vi_get_chn_align(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, td_u32 *align);

td_s32 ss_mpi_vi_set_chn_fmu_mode(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_fmu_mode fmu_mode);
td_s32 ss_mpi_vi_get_chn_fmu_mode(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_fmu_mode *fmu_mode);

td_s32 ss_mpi_vi_set_chn_vb_src(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_vb_src vb_src);
td_s32 ss_mpi_vi_get_chn_vb_src(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_vb_src *vb_src);

td_s32 ss_mpi_vi_attach_chn_vb_pool(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_vb_pool vb_pool);
td_s32 ss_mpi_vi_detach_chn_vb_pool(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn);

td_s32 ss_mpi_vi_query_chn_status(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn, ot_vi_chn_status *chn_status);

td_s32 ss_mpi_vi_get_chn_fd(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn);

/* stitch group interface */
td_s32 ss_mpi_vi_set_stitch_grp_attr(ot_vi_grp stitch_grp, const ot_vi_stitch_grp_attr *stitch_grp_attr);
td_s32 ss_mpi_vi_get_stitch_grp_attr(ot_vi_grp stitch_grp, ot_vi_stitch_grp_attr *stitch_grp_attr);

/* vi module interface */
td_s32 ss_mpi_vi_set_mod_param(const ot_vi_mod_param *mod_param);
td_s32 ss_mpi_vi_get_mod_param(ot_vi_mod_param *mod_param);

td_s32 ss_mpi_vi_enable_chn_motion_denoise(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn);
td_s32 ss_mpi_vi_disable_chn_motion_denoise(ot_vi_pipe vi_pipe, ot_vi_chn vi_chn);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SS_MPI_VI_H */