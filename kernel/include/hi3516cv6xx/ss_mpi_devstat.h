/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_DEVSTAT_H
#define SS_MPI_DEVSTAT_H

#include "ot_common_devstat.h"

#ifdef __cplusplus
extern "C" {
#endif

td_s32 ss_mpi_devstat_query_device_status(ot_devstat_device_status *device_status);

#ifdef __cplusplus
}
#endif

#endif /* SS_MPI_DEVSTAT_H */