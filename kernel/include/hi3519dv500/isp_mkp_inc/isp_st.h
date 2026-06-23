/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_ST_H
#define ISP_ST_H

#include "ot_osal.h"
#include "isp_ext.h"
#include "sys_ext.h"

typedef struct  {
    td_u32 num;
    struct osal_list_head head;
} list_entry;
typedef struct  {
    ot_vi_pipe vi_pipe;
    osal_workqueue worker;
    list_entry hwirq_list;
    list_entry tsklet_list;
    list_entry workqueue_list;
    osal_semaphore sem;
} isp_sync_tsk_ctx;

void sync_task_init(ot_vi_pipe vi_pipe);
void sync_task_exit(ot_vi_pipe vi_pipe);
td_s32 isp_sync_task_process(ot_vi_pipe vi_pipe);
td_s32 isp_drv_af_stats_kernel_get(ot_vi_pipe vi_pipe, ot_isp_drv_af_statistics *focus_stat);
td_s32 isp_drv_ae_stats_kernel_get(ot_vi_pipe vi_pipe, ot_isp_ae_stats *ae_stat);

#endif
