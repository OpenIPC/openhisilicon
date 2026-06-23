/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_MCF_CALIBRATION_H
#define SS_MPI_MCF_CALIBRATION_H

#include "ot_common_mcf_calibration.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

td_s32 ss_mpi_mcf_calibration(const ot_video_frame *pic_in_match, const ot_video_frame *pic_in_refer,
    ot_mcf_calibration_mode mode, ot_mcf_feature_info *feature_info, ot_mcf_calibration *calibration_info);
#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
