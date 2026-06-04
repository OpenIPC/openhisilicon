/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_LIST_H
#define ISP_LIST_H
#include "mkp_isp.h"
#include "isp_be_buf.h"

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
