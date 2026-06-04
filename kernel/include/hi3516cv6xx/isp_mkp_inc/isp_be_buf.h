/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_BE_BUF_H
#define ISP_BE_BUF_H

#include "ot_osal.h"
#include "mkp_isp.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct {
    td_s32                  hold_cnt;
    isp_be_wo_cfg_buf       be_cfg_buf;
    struct osal_list_head   list;
} isp_be_buf_node;

typedef struct {
    isp_be_buf_node         *node_buf;
    struct osal_list_head   free_list;
    struct osal_list_head   busy_list;
    osal_spinlock         spin_lock;
    td_u32                  free_num;
    td_u32                  busy_num;
    td_u32                  max_num;
    td_u32                  hold_num_max;
} isp_be_buf_queue;

td_s32 isp_drv_get_be_last_buf(ot_vi_pipe vi_pipe, td_phys_addr_t *point_phy_addr);
td_s32 isp_drv_query_be_free_buf(ot_vi_pipe vi_pipe, isp_be_wo_cfg_buf *be_wo_cfg_buf);
td_s32 isp_drv_get_free_be_buf(ot_vi_pipe vi_pipe);
td_s32 isp_drv_get_be_buf_first(ot_vi_pipe vi_pipe, td_phys_addr_t *point_phy_addr);
td_s32 isp_drv_all_be_buf_init(ot_vi_pipe vi_pipe);
td_s32 isp_drv_be_buf_exit(ot_vi_pipe vi_pipe);
td_s32 isp_drv_be_buf_init(ot_vi_pipe vi_pipe, isp_mmz_buf_ex *be_cfg_buf_info);
td_s32 isp_drv_be_buf_switch_state(ot_vi_pipe vi_pipe);
td_s32 isp_drv_be_buf_switch_finish_state(ot_vi_pipe vi_pipe);
td_s32 isp_drv_be_buf_ctl(ot_vi_pipe vi_pipe);
td_s32 isp_drv_be_buf_run_state(ot_vi_pipe vi_pipe);
td_void isp_drv_be_buf_queue_put_busy(ot_vi_pipe vi_pipe);

// for isp kernel extern interface
td_s32 isp_drv_get_ready_be_buf(ot_vi_pipe vi_pipe, isp_be_wo_cfg_buf *be_cfg_buf);
td_s32 isp_drv_put_free_be_buf(ot_vi_pipe vi_pipe, isp_be_wo_cfg_buf *be_cfg_buf);
td_s32 isp_drv_hold_busy_be_buf(ot_vi_pipe vi_pipe, isp_be_wo_cfg_buf *be_cfg_buf);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* ISP_BE_BUF_H */
