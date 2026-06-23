/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_LIST_H
#define ISP_LIST_H
#include "ot_osal.h"
#include "mkp_isp.h"

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
} isp_be_buf_queue;

td_s32 isp_creat_be_buf_queue(isp_be_buf_queue *queue, td_u32 max_num);

td_void isp_destroy_be_buf_queue(isp_be_buf_queue *queue);

isp_be_buf_node *isp_queue_get_free_be_buf(isp_be_buf_queue *queue);
isp_be_buf_node *isp_queue_get_free_be_buf_tail(isp_be_buf_queue *queue);

td_void  isp_queue_put_busy_be_buf(isp_be_buf_queue *queue, isp_be_buf_node  *node);

isp_be_buf_node *isp_queue_query_busy_be_buf(isp_be_buf_queue *queue);

td_void isp_queue_del_busy_be_buf(isp_be_buf_queue *queue, isp_be_buf_node  *node);

td_void isp_queue_put_free_be_buf(isp_be_buf_queue *queue, isp_be_buf_node  *node);

td_u32 isp_queue_get_free_num(isp_be_buf_queue *queue);

td_u32 isp_queue_get_busy_num(isp_be_buf_queue *queue);

#endif /* End of #ifndef ISP_LIST_H */
