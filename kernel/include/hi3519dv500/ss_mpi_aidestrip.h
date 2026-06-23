/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_AIDESTRIP_H
#define SS_MPI_AIDESTRIP_H

#include "ot_common_aiisp.h"
#include "ot_common_aidestrip.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

td_s32 ss_mpi_aidestrip_init(td_void);
td_s32 ss_mpi_aidestrip_exit(td_void);

td_s32 ss_mpi_aidestrip_enable(td_s32 pipe);
td_s32 ss_mpi_aidestrip_disable(td_s32 pipe);
td_s32 ss_mpi_aidestrip_query_status(td_s32 pipe, ot_aidestrip_status *status);
td_s32 ss_mpi_aidestrip_set_attr(td_s32 pipe, const ot_aidestrip_attr *attr);
td_s32 ss_mpi_aidestrip_get_attr(td_s32 pipe, ot_aidestrip_attr *attr);
td_s32 ss_mpi_aidestrip_set_cfg(td_s32 pipe, const ot_aidestrip_cfg *cfg);
td_s32 ss_mpi_aidestrip_get_cfg(td_s32 pipe, ot_aidestrip_cfg *cfg);

td_s32 ss_mpi_aidestrip_load_model(const ot_aidestrip_model *model, td_s32 *model_id);
td_s32 ss_mpi_aidestrip_unload_model(td_s32 model_id);

td_s32 ss_mpi_aidestrip_send_frame(td_s32 pipe, const ot_video_frame_info *frame, td_s32 milli_sec);

td_s32 ss_mpi_aidestrip_get_frame(td_s32 pipe, ot_video_frame_info *frame, td_s32 milli_sec);
td_s32 ss_mpi_aidestrip_release_frame(td_s32 pipe, const ot_video_frame_info *frame);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SS_MPI_aidestrip_H */
