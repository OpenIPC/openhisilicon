/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_SNAP_H
#define SS_MPI_SNAP_H

#include "ot_common_snap.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

td_s32 ss_mpi_snap_set_pipe_attr(ot_vi_pipe vi_pipe, const ot_snap_attr *snap_attr);
td_s32 ss_mpi_snap_get_pipe_attr(ot_vi_pipe vi_pipe, ot_snap_attr *snap_attr);

td_s32 ss_mpi_snap_enable_pipe(ot_vi_pipe vi_pipe);
td_s32 ss_mpi_snap_disable_pipe(ot_vi_pipe vi_pipe);

td_s32 ss_mpi_snap_trigger_pipe(ot_vi_pipe vi_pipe);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SS_MPI_SNAP_H */