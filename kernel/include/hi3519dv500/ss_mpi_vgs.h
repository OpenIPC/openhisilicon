/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_VGS_H
#define SS_MPI_VGS_H

#include "ot_common.h"
#include "ot_common_video.h"
#include "ot_common_vgs.h"

#ifdef __cplusplus
extern "C" {
#endif

td_s32 ss_mpi_vgs_begin_job(ot_vgs_handle *handle);

td_s32 ss_mpi_vgs_end_job(ot_vgs_handle handle);

td_s32 ss_mpi_vgs_cancel_job(ot_vgs_handle handle);

td_s32 ss_mpi_vgs_add_scale_task(ot_vgs_handle handle, const ot_vgs_task_attr *task, ot_vgs_scale_coef_mode mode);

td_s32 ss_mpi_vgs_add_draw_line_task(ot_vgs_handle handle, const ot_vgs_task_attr *task, const ot_vgs_line line[],
    td_u32 array_size);

td_s32 ss_mpi_vgs_add_cover_task(ot_vgs_handle handle, const ot_vgs_task_attr *task, const ot_cover cover[],
    td_u32 array_size);

td_s32 ss_mpi_vgs_add_mosaic_task(ot_vgs_handle handle, const ot_vgs_task_attr *task, const ot_mosaic mosaic[],
    td_u32 array_size);

td_s32 ss_mpi_vgs_add_osd_task(ot_vgs_handle handle, const ot_vgs_task_attr *task, const ot_vgs_osd osd[],
    td_u32 array_size);

td_s32 ss_mpi_vgs_add_rotation_task(ot_vgs_handle handle, const ot_vgs_task_attr *task, ot_rotation angle);

td_s32 ss_mpi_vgs_add_luma_task(ot_vgs_handle handle, const ot_vgs_task_attr *task, const ot_rect luma_rect[],
    td_u32 array_size, td_u64 luma_data[]);

td_s32 ss_mpi_vgs_add_corner_rect_task(ot_vgs_handle handle, const ot_vgs_task_attr *task,
    const ot_corner_rect corner_rect[], const ot_corner_rect_attr *corner_rect_attr, td_u32 array_size);

td_s32 ss_mpi_vgs_add_stitch_task(ot_vgs_handle handle, const ot_vgs_stitch_task_attr *task, td_u32 pic_num);

td_s32 ss_mpi_vgs_add_online_task(ot_vgs_handle handle, const ot_vgs_task_attr *task, const ot_vgs_online *online);

#ifdef __cplusplus
}
#endif

#endif /* end of SS_MPI_VGS_H */
