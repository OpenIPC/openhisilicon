/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_AWB_H
#define SS_MPI_AWB_H

#include "ot_common_isp.h"
#include "ot_common_3a.h"
#include "ot_common_awb.h"
#include "ot_common_sns.h"

#ifdef __cplusplus
extern "C" {
#endif

/* The interface of awb lib register to isp. */
td_s32 ss_mpi_awb_unregister(ot_vi_pipe vi_pipe, const ot_isp_3a_alg_lib *awb_lib);
td_s32 ss_mpi_awb_register(ot_vi_pipe vi_pipe, const ot_isp_3a_alg_lib *awb_lib);

/* The callback function of sensor register to awb lib. */
td_s32 ss_mpi_awb_sensor_reg_callback(ot_vi_pipe vi_pipe, ot_isp_3a_alg_lib *awb_lib,
    ot_isp_sns_attr_info *sns_attr_info, ot_isp_awb_sensor_register *awb_sns_register);
td_s32 ss_mpi_awb_sensor_unreg_callback(ot_vi_pipe vi_pipe, const ot_isp_3a_alg_lib *awb_lib, ot_sensor_id sns_id);

td_s32 ss_mpi_isp_set_wb_attr(ot_vi_pipe vi_pipe, const ot_isp_wb_attr *wb_attr);
td_s32 ss_mpi_isp_get_wb_attr(ot_vi_pipe vi_pipe, ot_isp_wb_attr *wb_attr);

td_s32 ss_mpi_isp_set_awb_attr_ex(ot_vi_pipe vi_pipe, const ot_isp_awb_attr_ex *awb_attr_ex);
td_s32 ss_mpi_isp_get_awb_attr_ex(ot_vi_pipe vi_pipe, ot_isp_awb_attr_ex *awb_attr_ex);

td_s32 ss_mpi_isp_set_ccm_attr(ot_vi_pipe vi_pipe, const ot_isp_color_matrix_attr *ccm_attr);
td_s32 ss_mpi_isp_get_ccm_attr(ot_vi_pipe vi_pipe, ot_isp_color_matrix_attr *ccm_attr);

td_s32 ss_mpi_isp_set_saturation_attr(ot_vi_pipe vi_pipe, const ot_isp_saturation_attr *sat_attr);
td_s32 ss_mpi_isp_get_saturation_attr(ot_vi_pipe vi_pipe, ot_isp_saturation_attr *sat_attr);

td_s32 ss_mpi_isp_query_wb_info(ot_vi_pipe vi_pipe, ot_isp_wb_info *wb_info);
td_s32 ss_mpi_isp_cal_gain_by_temp(ot_vi_pipe vi_pipe, const ot_isp_wb_attr *wb_attr, td_u16 color_temp, td_s16 shift,
    td_u16 *awb_gain);

#ifdef __cplusplus
}
#endif

#endif
