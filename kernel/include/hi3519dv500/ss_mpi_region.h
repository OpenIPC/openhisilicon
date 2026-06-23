/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_REGION_H
#define SS_MPI_REGION_H

#include "ot_debug.h"
#include "ot_common_region.h"

#ifdef __cplusplus
extern "C" {
#endif

td_s32 ss_mpi_rgn_create(ot_rgn_handle handle, const ot_rgn_attr *rgn_attr);
td_s32 ss_mpi_rgn_destroy(ot_rgn_handle handle);

td_s32 ss_mpi_rgn_get_attr(ot_rgn_handle handle, ot_rgn_attr *rgn_attr);
td_s32 ss_mpi_rgn_set_attr(ot_rgn_handle handle, const ot_rgn_attr *rgn_attr);

td_s32 ss_mpi_rgn_set_bmp(ot_rgn_handle handle, const ot_bmp *bmp);

td_s32 ss_mpi_rgn_attach_to_chn(ot_rgn_handle handle, const ot_mpp_chn *chn, const ot_rgn_chn_attr *chn_attr);
td_s32 ss_mpi_rgn_detach_from_chn(ot_rgn_handle handle, const ot_mpp_chn *chn);

td_s32 ss_mpi_rgn_attach_to_dev(ot_rgn_handle handle, const ot_mpp_chn *chn, const ot_rgn_chn_attr *chn_attr);
td_s32 ss_mpi_rgn_detach_from_dev(ot_rgn_handle handle, const ot_mpp_chn *chn);

td_s32 ss_mpi_rgn_set_chn_display_attr(ot_rgn_handle handle, const ot_mpp_chn *chn, const ot_rgn_chn_attr *chn_attr);
td_s32 ss_mpi_rgn_get_chn_display_attr(ot_rgn_handle handle, const ot_mpp_chn *chn, ot_rgn_chn_attr *chn_attr);

td_s32 ss_mpi_rgn_set_dev_display_attr(ot_rgn_handle handle, const ot_mpp_chn *chn, const ot_rgn_chn_attr *chn_attr);
td_s32 ss_mpi_rgn_get_dev_display_attr(ot_rgn_handle handle, const ot_mpp_chn *chn, ot_rgn_chn_attr *chn_attr);

td_s32 ss_mpi_rgn_get_canvas_info(ot_rgn_handle handle, ot_rgn_canvas_info *canvas_info);
td_s32 ss_mpi_rgn_update_canvas(ot_rgn_handle handle);

td_s32 ss_mpi_rgn_batch_begin(ot_rgn_handle_grp *grp, td_u32 handle_num, const ot_rgn_handle handle[]);
td_s32 ss_mpi_rgn_batch_end(ot_rgn_handle_grp grp);

#ifdef __cplusplus
}
#endif

#endif