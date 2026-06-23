/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_NUC_H
#define SS_MPI_NUC_H

#include "ot_common_aiisp.h"
#include "ot_common_nuc.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

td_s32 ss_mpi_nuc_init(td_void);
td_s32 ss_mpi_nuc_exit(td_void);

td_s32 ss_mpi_nuc_enable(td_s32 pipe);
td_s32 ss_mpi_nuc_disable(td_s32 pipe);
td_s32 ss_mpi_nuc_set_cfg(td_s32 pipe, const ot_nuc_cfg *cfg);
td_s32 ss_mpi_nuc_get_cfg(td_s32 pipe, ot_nuc_cfg *cfg);

td_s32 ss_mpi_nuc_load_model(const ot_nuc_model *model, td_s32 *model_id);
td_s32 ss_mpi_nuc_unload_model(td_s32 model_id);

td_s32 ss_mpi_nuc_send_frame(td_s32 pipe, const ot_video_frame_info *frame,
    const ot_video_frame_info *fpn_frame, td_s32 milli_sec);

td_s32 ss_mpi_nuc_get_frame(td_s32 pipe, ot_video_frame_info *frame, td_s32 milli_sec);
td_s32 ss_mpi_nuc_release_frame(td_s32 pipe, const ot_video_frame_info *frame);

td_s32 ss_mpi_nuc_query_status(td_s32 pipe, td_bool *enable);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SS_MPI_NUC_H */
