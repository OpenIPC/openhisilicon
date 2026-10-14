/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __MPI_VI_H__
#define __MPI_VI_H__

#include "xmedia_video_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

xmedia_s32 mpi_vi_start_stitch_luma_sync(xmedia_video_stitch_luma_sync_attr *sync_luma_param,
                                         xmedia_video_point over_lap_point[][4]);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MPI_VI_H__ */
