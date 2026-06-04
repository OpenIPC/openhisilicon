/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_SYSKOL_H
#define SS_MPI_SYSKOL_H

#include "ot_common.h"
#include "ot_common_syskol.h"

#ifdef __cplusplus
extern "C" {
#endif

td_s32 ss_mpi_syskol_set_param(const ot_syskol_param *param);
td_s32 ss_mpi_syskol_get_param(ot_syskol_param *param);

td_s32 ss_mpi_syskol_sleep(td_s32 milli_sec);
td_s32 ss_mpi_syskol_wakeup(td_void);

td_s32 ss_mpi_syskol_suspend(td_void);

#ifdef __cplusplus
}
#endif
#endif /* SS_MPI_SYSKOL_H */
