/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_BLA_H
#define SS_MPI_BLA_H

#include "ot_common_video.h"
#include "ot_common_bla.h"

td_s32 ss_mpi_bla_calibration(const ot_video_frame *master_frame, const ot_video_frame *slave_frame,
    const td_float down_scale_ratio, ot_point_f *master_coord);

td_s32 ss_mpi_bla_check_calibration_info(td_u32 num, const ot_bla_calibration_info *calibration_info, td_bool *valid);

td_s32 ss_mpi_bla_calculate_motor_coord(td_u32 num, const ot_bla_calibration_info *calibration_info,
    const ot_point_f *master_coord, ot_point_f *slave_coord);

#endif /* SS_MPI_BLA_H */
