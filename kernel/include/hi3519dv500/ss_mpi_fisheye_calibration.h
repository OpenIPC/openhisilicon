/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#ifndef SS_MPI_FISHEYE_CALIBRATION_H
#define SS_MPI_FISHEYE_CALIBRATION_H

#include "ot_common_video.h"
#include "ot_common_fisheye_calibration.h"

#ifdef __cplusplus
extern "C" {
#endif

td_s32 ss_mpi_fisheye_calibration_compute(const ot_video_frame *frame_in, ot_fisheye_calibration_level level,
    ot_fisheye_calibration_output *calibration_out);

td_s32 ss_mpi_fisheye_calibration_mark(const ot_video_frame *frame_in, const ot_video_frame *frame_out,
    const ot_fisheye_calibration_result *calibration_result);

#ifdef __cplusplus
}
#endif

#endif /* SS_MPI_FISHEYE_CALIBRATION_H */
