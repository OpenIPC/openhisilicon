/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_AIBNR_H
#define SS_MPI_AIBNR_H

#include "ot_common_aiisp.h"
#include "ot_common_aibnr.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

td_s32 ss_mpi_aibnr_init(td_void);
td_s32 ss_mpi_aibnr_exit(td_void);
td_s32 ss_mpi_aibnr_set_thread_attr(const ot_aiisp_thread_attr *thread_attr);
td_s32 ss_mpi_aibnr_get_thread_attr(ot_aiisp_thread_attr *thread_attr);

td_s32 ss_mpi_aibnr_enable(td_s32 pipe);
td_s32 ss_mpi_aibnr_disable(td_s32 pipe);
td_s32 ss_mpi_aibnr_query_status(td_s32 pipe, ot_aibnr_status *status);
td_s32 ss_mpi_aibnr_set_attr(td_s32 pipe, const ot_aibnr_attr *attr);
td_s32 ss_mpi_aibnr_get_attr(td_s32 pipe, ot_aibnr_attr *attr);
td_s32 ss_mpi_aibnr_set_cfg(td_s32 pipe, const ot_aibnr_cfg *cfg);
td_s32 ss_mpi_aibnr_get_cfg(td_s32 pipe, ot_aibnr_cfg *cfg);

td_s32 ss_mpi_aibnr_load_model(const ot_aibnr_model *model, td_s32 *model_id);
td_s32 ss_mpi_aibnr_unload_model(td_s32 model_id);
td_s32 ss_mpi_aibnr_set_model_attr(td_bool is_wdr_mode, const ot_aibnr_model_attr *attr);
td_s32 ss_mpi_aibnr_get_model_attr(td_bool is_wdr_mode, ot_aibnr_model_attr *attr);
td_s32 ss_mpi_aibnr_query_model_list(ot_aibnr_model_list *list);

td_s32 ss_mpi_aibnr_get_input_frame(td_s32 pipe, ot_video_frame_info *frame, td_s32 milli_sec);
td_s32 ss_mpi_aibnr_release_input_frame(td_s32 pipe, ot_video_frame_info *frame);
td_s32 ss_mpi_aibnr_get_output_frame(td_s32 pipe, ot_video_frame_info *frame, td_s32 milli_sec);
td_s32 ss_mpi_aibnr_release_output_frame(td_s32 pipe, ot_video_frame_info *frame);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SS_MPI_AIBNR_H */
