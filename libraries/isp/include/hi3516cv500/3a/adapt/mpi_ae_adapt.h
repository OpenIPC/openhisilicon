/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of mpi_ae_adapt.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __MPI_AE_ADAPT_H__
#define __MPI_AE_ADAPT_H__

#include "hi_comm_isp_adapt.h"
#include "hi_comm_3a_adapt.h"
#include "hi_ae_comm_adapt.h"
#include "hi_comm_sns_adapt.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

MPI_STATIC hi_s32 hi_mpi_isp_set_exposure_attr(hi_vi_pipe vi_pipe, const hi_isp_exposure_attr *exp_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_exposure_attr(hi_vi_pipe vi_pipe, hi_isp_exposure_attr *exp_attr);

MPI_STATIC hi_s32 hi_mpi_isp_set_wdr_exposure_attr(hi_vi_pipe vi_pipe, const hi_isp_wdr_exposure_attr *wdr_exp_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_wdr_exposure_attr(hi_vi_pipe vi_pipe, hi_isp_wdr_exposure_attr *wdr_exp_attr);

MPI_STATIC hi_s32 hi_mpi_isp_set_hdr_exposure_attr(hi_vi_pipe vi_pipe, const hi_isp_hdr_exposure_attr *hdr_exp_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_hdr_exposure_attr(hi_vi_pipe vi_pipe, hi_isp_hdr_exposure_attr *hdr_exp_attr);

MPI_STATIC hi_s32 hi_mpi_isp_set_ae_route_attr(hi_vi_pipe vi_pipe, const hi_isp_ae_route *ae_route_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_ae_route_attr(hi_vi_pipe vi_pipe, hi_isp_ae_route *ae_route_attr);

MPI_STATIC hi_s32 hi_mpi_isp_set_ae_route_sf_attr(hi_vi_pipe vi_pipe, const hi_isp_ae_route *ae_route_sf_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_ae_route_sf_attr(hi_vi_pipe vi_pipe, hi_isp_ae_route *ae_route_sf_attr);

MPI_STATIC hi_s32 hi_mpi_isp_query_exposure_info(hi_vi_pipe vi_pipe, hi_isp_exp_info *exp_info);

MPI_STATIC hi_s32 hi_mpi_isp_set_iris_attr(hi_vi_pipe vi_pipe, const hi_isp_iris_attr *iris_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_iris_attr(hi_vi_pipe vi_pipe, hi_isp_iris_attr *iris_attr);

MPI_STATIC hi_s32 hi_mpi_isp_set_dciris_attr(hi_vi_pipe vi_pipe, const hi_isp_dciris_attr *dciris_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_dciris_attr(hi_vi_pipe vi_pipe, hi_isp_dciris_attr *dciris_attr);

MPI_STATIC hi_s32 hi_mpi_isp_set_piris_attr(hi_vi_pipe vi_pipe, const hi_isp_piris_attr *piris_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_piris_attr(hi_vi_pipe vi_pipe, hi_isp_piris_attr *piris_attr);

MPI_STATIC hi_s32 hi_mpi_isp_set_ae_route_attr_ex(hi_vi_pipe vi_pipe, const hi_isp_ae_route_ex *ae_route_attr_ex);
MPI_STATIC hi_s32 hi_mpi_isp_get_ae_route_attr_ex(hi_vi_pipe vi_pipe, hi_isp_ae_route_ex *ae_route_attr_ex);

MPI_STATIC hi_s32 hi_mpi_isp_set_ae_route_sf_attr_ex(hi_vi_pipe vi_pipe, const hi_isp_ae_route_ex *ae_route_sf_attr_ex);
MPI_STATIC hi_s32 hi_mpi_isp_get_ae_route_sf_attr_ex(hi_vi_pipe vi_pipe, hi_isp_ae_route_ex *ae_route_sf_attr_ex);

MPI_STATIC hi_s32 hi_mpi_isp_set_smart_exposure_attr(hi_vi_pipe vi_pipe,
                                                     const hi_isp_smart_exposure_attr *smart_exp_attr);
MPI_STATIC hi_s32 hi_mpi_isp_get_smart_exposure_attr(hi_vi_pipe vi_pipe, hi_isp_smart_exposure_attr *smart_exp_attr);
MPI_STATIC hi_s32 hi_mpi_isp_set_exp_convert(hi_vi_pipe vi_pipe, hi_isp_exp_conv_param *conv_param);
MPI_STATIC hi_s32 hi_mpi_isp_get_exp_convert(hi_vi_pipe vi_pipe, hi_isp_exp_conv_param *conv_param);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
