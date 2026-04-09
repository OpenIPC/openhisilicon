/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_list.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __ISP_LIST_H__
#define __ISP_LIST_H__
#include "hi_osal.h"
#include "mkp_isp.h"

typedef struct {
    hi_s32                  hold_cnt;
    isp_be_wo_cfg_buf       be_cfg_buf;
    struct osal_list_head   list;
} isp_be_buf_node;

typedef struct {
    isp_be_buf_node      *node_buf;
    struct osal_list_head   free_list;
    struct osal_list_head   busy_list;
    osal_spinlock_t         spin_lock;
    hi_u32                  free_num;
    hi_u32                  busy_num;
    hi_u32                  max_num;
} isp_be_buf_queue;

hi_s32 isp_creat_be_buf_queue(isp_be_buf_queue *queue, hi_u32 max_num);

hi_void isp_destroy_be_buf_queue(isp_be_buf_queue *queue);

isp_be_buf_node *isp_queue_get_free_be_buf(isp_be_buf_queue *queue);
isp_be_buf_node *isp_queue_get_free_be_buf_tail(isp_be_buf_queue *queue);

hi_void  isp_queue_put_busy_be_buf(isp_be_buf_queue *queue, isp_be_buf_node  *node);

isp_be_buf_node *isp_queue_query_busy_be_buf(isp_be_buf_queue *queue);

hi_void isp_queue_del_busy_be_buf(isp_be_buf_queue *queue, isp_be_buf_node  *node);

hi_void isp_queue_put_free_be_buf(isp_be_buf_queue *queue, isp_be_buf_node  *node);

hi_u32 isp_queue_get_free_num(isp_be_buf_queue *queue);

hi_u32 isp_queue_get_busy_num(isp_be_buf_queue *queue);

#endif /* End of #ifndef __ISP_LIST_H__ */
