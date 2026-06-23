/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_THERMO_H
#define SS_MPI_THERMO_H

#include "ot_common_isp.h"
#include "ot_common_3a.h"
#include "ot_common_ae.h"
#include "ot_common_sns.h"

#ifdef __cplusplus
extern "C" {
#endif

/* The interface of ae lib register to isp. */
td_s32 ss_mpi_thermo_register(ot_vi_pipe vi_pipe, const ot_isp_3a_alg_lib *thermo_lib);
td_s32 ss_mpi_thermo_unregister(ot_vi_pipe vi_pipe, const ot_isp_3a_alg_lib *thermo_lib);

/* The callback function of sensor register to ae lib. */
td_s32 ss_mpi_thermo_sensor_reg_callback(ot_vi_pipe vi_pipe, ot_isp_3a_alg_lib *thermo_lib,
    const ot_isp_sns_attr_info *sns_attr_info, ot_isp_ae_sensor_register *pregister);
td_s32 ss_mpi_thermo_sensor_unreg_callback(ot_vi_pipe vi_pipe, const ot_isp_3a_alg_lib *thermo_lib,
    ot_sensor_id sns_id);
td_s32 ss_mpi_isp_set_thermo_attr(ot_vi_pipe vi_pipe, const ot_isp_thermo_attr *thermo_attr);
td_s32 ss_mpi_isp_get_thermo_attr(ot_vi_pipe vi_pipe, ot_isp_thermo_attr *thermo_attr);
td_s32 ss_mpi_thermo_query_exposure_info(ot_vi_pipe vi_pipe, ot_isp_thermo_info *thermo_info);
td_s32 ss_mpi_isp_set_thermo_curve_attr(ot_vi_pipe vi_pipe, ot_isp_thermo_curve_attr *curve_attr);
td_s32 ss_mpi_isp_get_thermo_curve_attr(ot_vi_pipe vi_pipe, ot_isp_thermo_curve_attr *curve_attr);
#ifdef __cplusplus
}
#endif

#endif
