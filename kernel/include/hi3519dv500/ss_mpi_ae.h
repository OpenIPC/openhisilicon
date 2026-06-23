/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_AE_H
#define SS_MPI_AE_H

#include "ot_common_isp.h"
#include "ot_common_3a.h"
#include "ot_common_ae.h"
#include "ot_common_sns.h"

#ifdef __cplusplus
extern "C" {
#endif

/* The interface of ae lib register to isp. */
td_s32 ss_mpi_ae_register(ot_vi_pipe vi_pipe, const ot_isp_3a_alg_lib *ae_lib);
td_s32 ss_mpi_ae_unregister(ot_vi_pipe vi_pipe, const ot_isp_3a_alg_lib *ae_lib);

/* The callback function of sensor register to ae lib. */
td_s32 ss_mpi_ae_sensor_reg_callback(ot_vi_pipe vi_pipe, ot_isp_3a_alg_lib *ae_lib,
    const ot_isp_sns_attr_info *sns_attr_info, ot_isp_ae_sensor_register *pregister);
td_s32 ss_mpi_ae_sensor_unreg_callback(ot_vi_pipe vi_pipe, const ot_isp_3a_alg_lib *ae_lib, ot_sensor_id sns_id);

td_s32 ss_mpi_isp_set_exposure_attr(ot_vi_pipe vi_pipe, const ot_isp_exposure_attr *exp_attr);
td_s32 ss_mpi_isp_get_exposure_attr(ot_vi_pipe vi_pipe, ot_isp_exposure_attr *exp_attr);

td_s32 ss_mpi_isp_set_wdr_exposure_attr(ot_vi_pipe vi_pipe, const ot_isp_wdr_exposure_attr *wdr_exp_attr);
td_s32 ss_mpi_isp_get_wdr_exposure_attr(ot_vi_pipe vi_pipe, ot_isp_wdr_exposure_attr *wdr_exp_attr);

td_s32 ss_mpi_isp_set_hdr_exposure_attr(ot_vi_pipe vi_pipe, const ot_isp_hdr_exposure_attr *hdr_exp_attr);
td_s32 ss_mpi_isp_get_hdr_exposure_attr(ot_vi_pipe vi_pipe, ot_isp_hdr_exposure_attr *hdr_exp_attr);

td_s32 ss_mpi_isp_set_ae_route_attr(ot_vi_pipe vi_pipe, const ot_isp_ae_route *ae_route_attr);
td_s32 ss_mpi_isp_get_ae_route_attr(ot_vi_pipe vi_pipe, ot_isp_ae_route *ae_route_attr);

td_s32 ss_mpi_isp_set_ae_route_sf_attr(ot_vi_pipe vi_pipe, const ot_isp_ae_route *ae_route_sf_attr);
td_s32 ss_mpi_isp_get_ae_route_sf_attr(ot_vi_pipe vi_pipe, ot_isp_ae_route *ae_route_sf_attr);

td_s32 ss_mpi_isp_query_exposure_info(ot_vi_pipe vi_pipe, ot_isp_exp_info *exp_info);

td_s32 ss_mpi_isp_set_iris_attr(ot_vi_pipe vi_pipe, const ot_isp_iris_attr *iris_attr);
td_s32 ss_mpi_isp_get_iris_attr(ot_vi_pipe vi_pipe, ot_isp_iris_attr *iris_attr);

td_s32 ss_mpi_isp_set_dciris_attr(ot_vi_pipe vi_pipe, const ot_isp_dciris_attr *dciris_attr);
td_s32 ss_mpi_isp_get_dciris_attr(ot_vi_pipe vi_pipe, ot_isp_dciris_attr *dciris_attr);

td_s32 ss_mpi_isp_set_piris_attr(ot_vi_pipe vi_pipe, const ot_isp_piris_attr *piris_attr);
td_s32 ss_mpi_isp_get_piris_attr(ot_vi_pipe vi_pipe, ot_isp_piris_attr *piris_attr);

td_s32 ss_mpi_isp_set_ae_route_attr_ex(ot_vi_pipe vi_pipe, const ot_isp_ae_route_ex *ae_route_attr_ex);
td_s32 ss_mpi_isp_get_ae_route_attr_ex(ot_vi_pipe vi_pipe, ot_isp_ae_route_ex *ae_route_attr_ex);

td_s32 ss_mpi_isp_set_ae_route_sf_attr_ex(ot_vi_pipe vi_pipe, const ot_isp_ae_route_ex *ae_route_sf_attr_ex);
td_s32 ss_mpi_isp_get_ae_route_sf_attr_ex(ot_vi_pipe vi_pipe, ot_isp_ae_route_ex *ae_route_sf_attr_ex);

td_s32 ss_mpi_isp_set_smart_exposure_attr(ot_vi_pipe vi_pipe, const ot_isp_smart_exposure_attr *smart_exp_attr);
td_s32 ss_mpi_isp_get_smart_exposure_attr(ot_vi_pipe vi_pipe, ot_isp_smart_exposure_attr *smart_exp_attr);
td_s32 ss_mpi_isp_set_exp_convert(ot_vi_pipe vi_pipe, const ot_isp_exp_conv_param *conv_param);
td_s32 ss_mpi_isp_get_exp_convert(ot_vi_pipe vi_pipe, ot_isp_exp_conv_param *conv_param);

td_s32 ss_mpi_isp_set_fast_face_ae_attr(ot_vi_pipe vi_pipe, const ot_isp_fast_face_ae_attr *fast_face_attr);
td_s32 ss_mpi_isp_get_fast_face_ae_attr(ot_vi_pipe vi_pipe, ot_isp_fast_face_ae_attr *fast_face_attr);
#ifdef __cplusplus
}
#endif

#endif
