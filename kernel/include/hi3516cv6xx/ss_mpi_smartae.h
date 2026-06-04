/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_SMARTAE_H
#define SS_MPI_SMARTAE_H

#include "ot_common_video.h"
#include "ot_common_smartae.h"

td_s32 ss_mpi_smartae_update_roi_info(ot_vi_pipe vi_pipe, const ot_video_frame_info *frame_info,
    ot_smartae_roi_info *smartinfo);

#endif /* SS_MPI_SMARTAE_H */
