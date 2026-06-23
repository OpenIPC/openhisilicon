/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_AIDRC_H
#define SS_MPI_AIDRC_H

#include "ot_common_aiisp.h"
#include "ot_common_aidrc.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

td_s32 ss_mpi_aidrc_init(td_void);
td_s32 ss_mpi_aidrc_exit(td_void);
td_s32 ss_mpi_aidrc_set_thread_attr(const ot_aiisp_thread_attr *thread_attr);
td_s32 ss_mpi_aidrc_get_thread_attr(ot_aiisp_thread_attr *thread_attr);

td_s32 ss_mpi_aidrc_enable(td_s32 pipe);
td_s32 ss_mpi_aidrc_disable(td_s32 pipe);
td_s32 ss_mpi_aidrc_query_status(td_s32 pipe, ot_aidrc_status *status);
td_s32 ss_mpi_aidrc_set_attr(td_s32 pipe, const ot_aidrc_attr *attr);
td_s32 ss_mpi_aidrc_get_attr(td_s32 pipe, ot_aidrc_attr *attr);
td_s32 ss_mpi_aidrc_set_cfg(td_s32 pipe, const ot_aidrc_cfg *cfg);
td_s32 ss_mpi_aidrc_get_cfg(td_s32 pipe, ot_aidrc_cfg *cfg);

td_s32 ss_mpi_aidrc_load_model(const ot_aidrc_model *model, td_s32 *model_id);
td_s32 ss_mpi_aidrc_unload_model(td_s32 model_id);

td_s32 ss_mpi_aidrc_set_smooth(td_s32 pipe, const ot_aidrc_smooth *smooth);
td_s32 ss_mpi_aidrc_get_smooth(td_s32 pipe, ot_aidrc_smooth *smooth);

td_s32 ss_mpi_aidrc_get_output_frame(td_s32 pipe,          ot_video_frame_info *frame,
    ot_aiisp_mem_info *out_data[], td_u32 out_data_num, td_s32 milli_sec);
td_s32 ss_mpi_aidrc_release_output_frame(td_s32 pipe, ot_video_frame_info *frame);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SS_MPI_AIDRC_H */
