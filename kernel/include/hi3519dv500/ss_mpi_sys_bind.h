/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_SYS_BIND_H
#define SS_MPI_SYS_BIND_H

#include "ot_common.h"
#include "ot_common_sys_bind.h"

#ifdef __cplusplus
extern "C" {
#endif

td_s32 ss_mpi_sys_bind(const ot_mpp_chn *src_chn, const ot_mpp_chn *dst_chn);
td_s32 ss_mpi_sys_unbind(const ot_mpp_chn *src_chn, const ot_mpp_chn *dst_chn);

td_s32 ss_mpi_sys_get_bind_by_dst(const ot_mpp_chn *dst_chn, ot_mpp_chn *src_chn);
td_s32 ss_mpi_sys_get_bind_by_src(const ot_mpp_chn *src_chn, ot_mpp_bind_dst *bind_dst);

#ifdef __cplusplus
}
#endif
#endif /* SS_MPI_SYS_BIND_H */
