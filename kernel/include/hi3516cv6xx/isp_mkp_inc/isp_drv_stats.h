/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */


#ifndef ISP_STATS_H
#define ISP_STATS_H

#include "ot_type.h"
#include "ot_common.h"
#include "mkp_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

td_s32  isp_drv_fe_stats_buf_busy_put(ot_vi_pipe vi_pipe);
td_s32  isp_drv_stats_buf_busy_put(ot_vi_pipe vi_pipe);
td_void isp_drv_stats_be_end_read(ot_vi_pipe vi_pipe);
td_void isp_drv_stats_be_af_offline_end_int_read(ot_vi_pipe vi_pipe);

td_s32 isp_drv_stats_buf_init(ot_vi_pipe vi_pipe, td_phys_addr_t *point_phy_addr);
td_s32 isp_drv_stats_buf_exit(ot_vi_pipe vi_pipe);
td_s32 isp_drv_stats_buf_user_get(ot_vi_pipe vi_pipe, isp_stat_info *stat);
td_s32 isp_drv_stats_buf_user_put(ot_vi_pipe vi_pipe, isp_stat_info *stat_info);
td_s32 isp_drv_stats_query_active(ot_vi_pipe vi_pipe, isp_stat_info *stat_info);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif

