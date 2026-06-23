/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_SYS_H
#define SS_MPI_SYS_H

#include "ot_type.h"
#include "ot_common.h"
#include "ot_common_sys.h"
#include "ot_debug.h"

#ifdef __cplusplus
extern "C" {
#endif

td_s32 ss_mpi_log_set_level_cfg(const ot_log_level_cfg *level_cfg);
td_s32 ss_mpi_log_get_level_cfg(ot_log_level_cfg *level_cfg);

td_s32 ss_mpi_log_set_wait_flag(td_bool flag);
td_s32 ss_mpi_log_read(td_char *buf, td_u32 size);
td_void ss_mpi_log_close(td_void);

td_s32 ss_mpi_sys_init(td_void);
td_s32 ss_mpi_sys_exit(td_void);

td_s32 ss_mpi_sys_set_cfg(const ot_mpp_sys_cfg *sys_cfg);
td_s32 ss_mpi_sys_get_cfg(ot_mpp_sys_cfg *sys_cfg);

td_s32 ss_mpi_sys_get_version(ot_mpp_version *version);

td_s32 ss_mpi_sys_get_cur_pts(td_u64 *cur_pts);
td_s32 ss_mpi_sys_init_pts_base(td_u64 pts_base);
td_s32 ss_mpi_sys_sync_pts(td_u64 pts_base);

td_s32 ss_mpi_sys_close_fd(td_void);

td_s32 ss_mpi_sys_set_mem_cfg(const ot_mpp_chn *mpp_chn, const td_char *mmz_name);
td_s32 ss_mpi_sys_get_mem_cfg(const ot_mpp_chn *mpp_chn, td_char *mmz_name);

td_s32 ss_mpi_sys_set_scale_coef_level(const ot_scale_range *scale_range, const ot_scale_coef_level *scale_coef_level);
td_s32 ss_mpi_sys_get_scale_coef_level(const ot_scale_range *scale_range, ot_scale_coef_level *scale_coef_level);

td_s32 ss_mpi_sys_set_time_zone(td_s32 time_zone);
td_s32 ss_mpi_sys_get_time_zone(td_s32 *time_zone);

td_s32 ss_mpi_sys_set_gps_info(const ot_gps_info *gps_info);
td_s32 ss_mpi_sys_get_gps_info(ot_gps_info *gps_info);

td_s32 ss_mpi_sys_set_vi_vpss_mode(const ot_vi_vpss_mode *vi_vpss_mode);
td_s32 ss_mpi_sys_get_vi_vpss_mode(ot_vi_vpss_mode *vi_vpss_mode);

td_s32 ss_mpi_sys_set_vi_aiisp_mode(ot_vi_pipe vi_pipe, ot_vi_aiisp_mode aiisp_mode);
td_s32 ss_mpi_sys_get_vi_aiisp_mode(ot_vi_pipe vi_pipe, ot_vi_aiisp_mode *aiisp_mode);

td_s32 ss_mpi_sys_get_chip_id(td_u32 *chip_id);
td_s32 ss_mpi_sys_get_custom_code(td_u32 *custom_code);
td_s32 ss_mpi_sys_get_unique_id(ot_unique_id *unique_id);

td_s32 ss_mpi_sys_set_raw_frame_compress_param(const ot_raw_frame_compress_param *compress_param);
td_s32 ss_mpi_sys_get_raw_frame_compress_param(ot_raw_frame_compress_param *compress_param);

td_s32 ss_mpi_sys_set_tuning_connect(td_s32 connect);
td_s32 ss_mpi_sys_get_tuning_connect(td_s32 *connect);

td_s32 ss_mpi_sys_set_schedule_mode(const ot_schedule_mode *schedule_mode);
td_s32 ss_mpi_sys_get_schedule_mode(ot_schedule_mode *schedule_mode);

td_s32 ss_mpi_sys_set_fmu_attr(ot_fmu_id id, const ot_fmu_attr *fmu_attr);
td_s32 ss_mpi_sys_get_fmu_attr(ot_fmu_id id, ot_fmu_attr *fmu_attr);

td_s32 ss_mpi_sys_set_3dnr_pos(ot_3dnr_pos_type pos);
td_s32 ss_mpi_sys_get_3dnr_pos(ot_3dnr_pos_type *pos);

td_s32 ss_mpi_sys_start_sync_frame_rate_ctrl(const ot_mpp_chn *src_chn, const ot_mpp_chn *dst_chn);
td_s32 ss_mpi_sys_stop_sync_frame_rate_ctrl(const ot_mpp_chn *src_chn, const ot_mpp_chn *dst_chn);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SS_MPI_SYS_H */
