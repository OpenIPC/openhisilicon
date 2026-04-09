/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of mpi_awb_adapt.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __MPI_AWB_ADAPT_H__
#define __MPI_AWB_ADAPT_H__

#include "hi_comm_isp_adapt.h"
#include "hi_comm_3a_adapt.h"
#include "hi_awb_comm_adapt.h"
#include "hi_comm_sns_adapt.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

MPI_STATIC hi_s32 hi_mpi_isp_set_wb_attr(hi_vi_pipe vi_pipe, const hi_isp_wb_attr *wb_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_wb_attr(hi_vi_pipe vi_pipe, hi_isp_wb_attr *wb_attr);

MPI_STATIC hi_s32 hi_mpi_isp_set_awb_attr_ex(hi_vi_pipe vi_pipe, hi_isp_awb_attr_ex *awb_attr_ex);
MPI_STATIC hi_s32 hi_mpi_isp_get_awb_attr_ex(hi_vi_pipe vi_pipe, hi_isp_awb_attr_ex *awb_attr_ex);

MPI_STATIC hi_s32 hi_mpi_isp_set_ccm_attr(hi_vi_pipe vi_pipe, const hi_isp_color_matrix_attr *ccm_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_ccm_attr(hi_vi_pipe vi_pipe, hi_isp_color_matrix_attr *ccm_attr);

MPI_STATIC hi_s32 hi_mpi_isp_set_saturation_attr(hi_vi_pipe vi_pipe, const hi_isp_saturation_attr *sat_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_saturation_attr(hi_vi_pipe vi_pipe, hi_isp_saturation_attr *sat_attr);

MPI_STATIC hi_s32 hi_mpi_isp_query_wb_info(hi_vi_pipe vi_pipe, hi_isp_wb_info *wb_info);
MPI_STATIC hi_s32 hi_mpi_isp_cal_gain_by_temp(hi_vi_pipe vi_pipe, const hi_isp_wb_attr *wb_attr, hi_u16 color_temp,
                                              hi_s16 shift, hi_u16 *awb_gain);
MPI_STATIC hi_s32 hi_mpi_isp_set_spec_awb_attr(hi_vi_pipe vi_pipe, const hi_isp_spec_awb_attr *spec_awb_attr);
MPI_STATIC hi_s32 hi_mpi_isp_set_caa_control_attr(hi_vi_pipe vi_pipe,
                                                  const hi_isp_spec_awb_caa_control *spec_awb_caa_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_caa_control_attr(hi_vi_pipe vi_pipe, hi_isp_spec_awb_caa_control *spec_awb_caa_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_spec_awb_attr(hi_vi_pipe vi_pipe, hi_isp_spec_awb_attr *spec_awb_attr);
MPI_STATIC hi_s32 hi_mpi_isp_set_spec_awb_control_attr(hi_vi_pipe vi_pipe, hi_isp_spec_awb_control *spec_awb_control);
MPI_STATIC hi_s32 hi_mpi_isp_get_spec_awb_control_attr(hi_vi_pipe vi_pipe, hi_isp_spec_awb_control *spec_awb_control);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif
