/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#ifndef SS_MPI_GDC_H
#define SS_MPI_GDC_H

#include "ot_common.h"
#include "ot_common_video.h"
#include "ot_common_gdc.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

td_s32 ss_mpi_gdc_begin_job(ot_gdc_handle *handle);

td_s32 ss_mpi_gdc_end_job(ot_gdc_handle handle);

td_s32 ss_mpi_gdc_cancel_job(ot_gdc_handle handle);

td_s32 ss_mpi_gdc_add_correction_task(ot_gdc_handle handle, const ot_gdc_task_attr *task,
    const ot_fisheye_attr *fisheye_attr);

td_s32 ss_mpi_gdc_add_correction_ex_task(ot_gdc_handle handle, const ot_gdc_task_attr *task,
    const ot_fisheye_attr_ex *fisheye_attr_ex, td_bool check_mode);

td_s32 ss_mpi_gdc_set_cfg(ot_gdc_handle handle, const ot_gdc_fisheye_job_cfg *job_cfg);

td_s32 ss_mpi_gdc_add_pmf_task(ot_gdc_handle handle, const ot_gdc_task_attr *task,
    const ot_gdc_pmf_attr *gdc_pmf_attr);

td_s32 ss_mpi_gdc_fisheye_pos_query_dst_to_src(const ot_gdc_fisheye_point_query_attr *fisheye_point_query_attr,
    const ot_gdc_query_pic_info *pic_info, const ot_point *dst_point, ot_point *src_point);

td_s32 ss_mpi_gdc_add_lut_task(ot_gdc_handle handle, const ot_gdc_task_attr *task,
    const ot_lut_attr *lut_attr);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MPI_GDC_ADAPT_H */