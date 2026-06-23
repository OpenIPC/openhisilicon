/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#ifndef SS_MPI_MOTIONFUSION_H
#define SS_MPI_MOTIONFUSION_H

#include "ot_type.h"
#include "ot_common_motionfusion.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MOTIONFUSION_DEVICE_ID_0 0
#define MOTIONFUSION_DEVICE_ID_1 1

td_s32 ss_mpi_mfusion_set_attr(const td_u32 fusion_id, const ot_mfusion_attr *mfusion_attr);
td_s32 ss_mpi_mfusion_get_attr(const td_u32 fusion_id, ot_mfusion_attr *mfusion_attr);

td_s32 ss_mpi_mfusion_set_gyro_drift(const td_u32 fusion_id, const ot_mfusion_drift *drift);
td_s32 ss_mpi_mfusion_get_gyro_drift(const td_u32 fusion_id, ot_mfusion_drift *drift);

td_s32 ss_mpi_mfusion_set_gyro_six_side_calibration(const td_u32 fusion_id,
    const ot_mfusion_six_side_calibration *six_side_calibration);
td_s32 ss_mpi_mfusion_get_gyro_six_side_calibration(const td_u32 fusion_id,
    ot_mfusion_six_side_calibration *six_side_calibration);

td_s32 ss_mpi_mfusion_set_gyro_temperature_drift(const td_u32 fusion_id,
    const ot_mfusion_temperature_drift *temperature_drift);
td_s32 ss_mpi_mfusion_get_gyro_temperature_drift(const td_u32 fusion_id,
    ot_mfusion_temperature_drift *temperature_drift);

td_s32 ss_mpi_mfusion_set_gyro_online_temperature_drift(const td_u32 fusion_id,
    const ot_mfusion_temperature_drift *temperature_drift);
td_s32 ss_mpi_mfusion_get_gyro_online_temperature_drift(const td_u32 fusion_id,
    ot_mfusion_temperature_drift *temperature_drift);

td_s32 ss_mpi_mfusion_set_gyro_online_drift(const td_u32 fusion_id, const ot_mfusion_drift *drift);
td_s32 ss_mpi_mfusion_get_gyro_online_drift(const td_u32 fusion_id, ot_mfusion_drift *drift);

td_s32 ss_mpi_mfusion_init_rotation_compensation(const td_u32 fusion_id,
    const ot_mfusion_rotation_cfg *cfg);
td_s32 ss_mpi_mfusion_deinit_rotation_compensation(const td_u32 fusion_id);
td_s32 ss_mpi_mfusion_get_rotation_compensation(const td_u32 fusion_id,
    ot_mfusion_rotation_compensation *compensation);
td_s32 ss_mpi_mfusion_set_rotation_compensation(const td_u32 fusion_id,
    const ot_mfusion_rotation_compensation *compensation);
td_s32 ss_mpi_mfusion_send_quaternion(const td_u32 fusion_id, ot_mfusion_quaternion_buf *quaternion_buf);
td_s32 ss_mpi_mfusion_get_quaternion(const td_u32 fusion_id, td_u64 begin_pts, td_u64 end_pts,
    ot_mfusion_quaternion_buf *quaternion_buf);

#ifdef __cplusplus
}
#endif

#endif /* End of #ifndef SS_MPI_MONTIONFUSION_H */
