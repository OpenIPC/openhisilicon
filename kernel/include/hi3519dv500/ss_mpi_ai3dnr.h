/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_AI3DNR_H
#define SS_MPI_AI3DNR_H

#include "ot_common_aiisp.h"
#include "ot_common_ai3dnr.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

td_s32 ss_mpi_ai3dnr_init(td_void);
td_s32 ss_mpi_ai3dnr_exit(td_void);
td_s32 ss_mpi_ai3dnr_set_thread_attr(const ot_aiisp_thread_attr *thread_attr);
td_s32 ss_mpi_ai3dnr_get_thread_attr(ot_aiisp_thread_attr *thread_attr);

td_s32 ss_mpi_ai3dnr_enable(td_s32 pipe);
td_s32 ss_mpi_ai3dnr_disable(td_s32 pipe);
td_s32 ss_mpi_ai3dnr_query_status(td_s32 pipe, ot_ai3dnr_status *status);
td_s32 ss_mpi_ai3dnr_set_attr(td_s32 pipe, const ot_ai3dnr_attr *attr);
td_s32 ss_mpi_ai3dnr_get_attr(td_s32 pipe, ot_ai3dnr_attr *attr);
td_s32 ss_mpi_ai3dnr_set_cfg(td_s32 pipe, const ot_ai3dnr_cfg *cfg);
td_s32 ss_mpi_ai3dnr_get_cfg(td_s32 pipe, ot_ai3dnr_cfg *cfg);
td_s32 ss_mpi_ai3dnr_set_smooth(td_s32 pipe, const ot_ai3dnr_smooth *smooth);
td_s32 ss_mpi_ai3dnr_get_smooth(td_s32 pipe, ot_ai3dnr_smooth *smooth);

td_s32 ss_mpi_ai3dnr_load_model(const ot_ai3dnr_model *model, td_s32 *model_id);
td_s32 ss_mpi_ai3dnr_unload_model(td_s32 model_id);

td_s32 ss_mpi_ai3dnr_get_input_frame(td_s32 pipe, ot_video_frame_info *frame, td_s32 milli_sec);
td_s32 ss_mpi_ai3dnr_release_input_frame(td_s32 pipe, ot_video_frame_info *frame);
td_s32 ss_mpi_ai3dnr_get_output_frame(td_s32 pipe, ot_video_frame_info *frame, td_s32 milli_sec);
td_s32 ss_mpi_ai3dnr_release_output_frame(td_s32 pipe, ot_video_frame_info *frame);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SS_MPI_AI3DNR_H */
