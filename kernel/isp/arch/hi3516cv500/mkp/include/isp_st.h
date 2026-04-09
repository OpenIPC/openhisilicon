/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_st.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __ISP_ST_H__
#define __ISP_ST_H__

#include "hi_osal.h"
#include "isp_ext.h"
#include "sys_ext.h"

typedef struct  {
    hi_u16  v1;
    hi_u16  h1;
    hi_u16  v2;
    hi_u16  h2;
    hi_u16  y;
    hi_u16  hl_cnt;
} isp_drv_focus_zone;

typedef struct  {
    isp_drv_focus_zone zone_metrics[DRV_WDR_CHN_MAX][DRV_AF_ZONE_ROW][DRV_AF_ZONE_COLUMN];
} isp_drv_fe_focus_statistics;

typedef struct {
    isp_drv_focus_zone zone_metrics[DRV_AF_ZONE_ROW][DRV_AF_ZONE_COLUMN];
} isp_drv_be_focus_statistics;

typedef struct {
    isp_drv_fe_focus_statistics      fe_af_stat;
    isp_drv_be_focus_statistics      be_af_stat;
} isp_drv_af_statistics;

typedef ISP_SYNC_TSK_METHOD_E isp_sync_tsk_method;

typedef struct {
    isp_sync_tsk_method  method;
    hi_s32 (* isp_sync_tsk_call_back) (hi_u64 data);
    hi_u64 data;
    const char *sz_id;
    struct osal_list_head list;
    isp_drv_af_statistics     *focus_stat;
} isp_sync_task_node;
typedef struct  {
    hi_u32 num;
    struct osal_list_head head;
} list_entry;
typedef struct  {
    VI_PIPE vi_pipe;
    struct osal_work_struct worker;
    list_entry hwirq_list;
    list_entry tsklet_list;
    list_entry workqueue_list;
    struct osal_semaphore sem;
} isp_sync_tsk_ctx;

void sync_task_init(VI_PIPE vi_pipe);
void sync_task_exit(VI_PIPE vi_pipe);
hi_s32 isp_sync_task_process(VI_PIPE vi_pipe);
hi_s32 isp_drv_sta_kernel_get(VI_PIPE vi_pipe, isp_drv_af_statistics *focus_stat);

hi_s32 isp_sync_task_register(VI_PIPE vi_pipe, isp_sync_task_node *new_node);
hi_s32 isp_sync_task_unregister(VI_PIPE vi_pipe, isp_sync_task_node *del_node);

#endif
