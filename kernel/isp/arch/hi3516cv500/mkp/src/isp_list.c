/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_list.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_list.h"
#include "hi_osal.h"


hi_s32 isp_creat_be_buf_queue(isp_be_buf_queue *queue, hi_u32 max_num)
{
    hi_u32 i;
    unsigned long flags;

    isp_be_buf_node *node = HI_NULL;

    osal_spin_lock_init(&queue->spin_lock);

    /* Malloc node buffer(cached) */
    queue->node_buf = osal_kmalloc(max_num * sizeof(isp_be_buf_node), osal_gfp_kernel);

    if (queue->node_buf == HI_NULL) {
        return HI_FAILURE;
    }

    osal_spin_lock_irqsave(&queue->spin_lock, &flags);

    /* Init list head */
    OSAL_INIT_LIST_HEAD(&queue->free_list);
    OSAL_INIT_LIST_HEAD(&queue->busy_list);

    /* The node buffer add freelist tail */
    node = queue->node_buf;

    for (i = 0; i < max_num; i++) {
        osal_list_add_tail(&node->list, &queue->free_list);
        node++;
    }

    /* init queue others parameters */
    queue->free_num = max_num;
    queue->max_num = max_num;
    queue->busy_num = 0;

    osal_spin_unlock_irqrestore(&queue->spin_lock, &flags);

    return HI_SUCCESS;
}

hi_void isp_destroy_be_buf_queue(isp_be_buf_queue *queue)
{
    unsigned long flags;

    osal_spin_lock_irqsave(&queue->spin_lock, &flags);

    /* Init list head */
    OSAL_INIT_LIST_HEAD(&queue->free_list);
    OSAL_INIT_LIST_HEAD(&queue->busy_list);

    queue->free_num = 0;
    queue->busy_num = 0;
    queue->max_num = 0;

    osal_spin_unlock_irqrestore(&queue->spin_lock, &flags);

    osal_spin_lock_destory(&queue->spin_lock);

    if (queue->node_buf) {
        osal_kfree(queue->node_buf);
        queue->node_buf = HI_NULL;
    }

    return;
}

isp_be_buf_node *isp_queue_get_free_be_buf(isp_be_buf_queue *queue)
{
    unsigned long flags;
    struct osal_list_head *plist = HI_NULL;
    isp_be_buf_node *node = HI_NULL;

    osal_spin_lock_irqsave(&queue->spin_lock, &flags);

    if (osal_list_empty(&queue->free_list)) {
        osal_spin_unlock_irqrestore(&queue->spin_lock, &flags);

        return HI_NULL;
    }

    plist = queue->free_list.next;
    osal_list_del(plist);

    node = osal_list_entry(plist, isp_be_buf_node, list);

    queue->free_num--;
    osal_spin_unlock_irqrestore(&queue->spin_lock, &flags);

    return node;
}

isp_be_buf_node *isp_queue_get_free_be_buf_tail(isp_be_buf_queue *queue)
{
    unsigned long flags;

    struct osal_list_head *plist = HI_NULL;
    isp_be_buf_node *node = HI_NULL;

    osal_spin_lock_irqsave(&queue->spin_lock, &flags);

    if (osal_list_empty(&queue->free_list)) {
        osal_spin_unlock_irqrestore(&queue->spin_lock, &flags);

        return HI_NULL;
    }

    plist = queue->free_list.prev;

    if (plist == HI_NULL) {
        osal_spin_unlock_irqrestore(&queue->spin_lock, &flags);

        return HI_NULL;
    }

    osal_list_del(plist);
    node = osal_list_entry(plist, isp_be_buf_node, list);

    queue->free_num--;

    osal_spin_unlock_irqrestore(&queue->spin_lock, &flags);

    return node;
}

hi_void  isp_queue_put_busy_be_buf(isp_be_buf_queue *queue, isp_be_buf_node  *node)
{
    unsigned long flags;

    osal_spin_lock_irqsave(&queue->spin_lock, &flags);

    osal_list_add_tail(&node->list, &queue->busy_list);
    queue->busy_num++;

    osal_spin_unlock_irqrestore(&queue->spin_lock, &flags);
    return;
}

hi_void isp_queue_del_busy_be_buf(isp_be_buf_queue *queue, isp_be_buf_node  *node)
{
    unsigned long flags;

    osal_spin_lock_irqsave(&queue->spin_lock, &flags);

    if (osal_list_empty(&queue->busy_list)) {
        osal_spin_unlock_irqrestore(&queue->spin_lock, &flags);

        return;
    }

    osal_list_del(&node->list);

    queue->busy_num--;

    osal_spin_unlock_irqrestore(&queue->spin_lock, &flags);
}

isp_be_buf_node *isp_queue_query_busy_be_buf(isp_be_buf_queue *queue)
{
    unsigned long flags;
    struct osal_list_head *plist = HI_NULL;
    isp_be_buf_node *node = HI_NULL;

    osal_spin_lock_irqsave(&queue->spin_lock, &flags);

    if (osal_list_empty(&queue->busy_list)) {
        osal_spin_unlock_irqrestore(&queue->spin_lock, &flags);

        return HI_NULL;
    }

    plist = queue->busy_list.prev;

    if (plist == HI_NULL) {
        osal_spin_unlock_irqrestore(&queue->spin_lock, &flags);

        return HI_NULL;
    }

    node = osal_list_entry(plist, isp_be_buf_node, list);
    osal_spin_unlock_irqrestore(&queue->spin_lock, &flags);

    return node;
}

hi_void isp_queue_put_free_be_buf(isp_be_buf_queue *queue, isp_be_buf_node *node)
{
    unsigned long flags;

    osal_spin_lock_irqsave(&queue->spin_lock, &flags);

    osal_list_add_tail(&node->list, &queue->free_list);
    queue->free_num++;

    osal_spin_unlock_irqrestore(&queue->spin_lock, &flags);
    return;
}

hi_u32 isp_queue_get_free_num(isp_be_buf_queue *queue)
{
    hi_u32 free_num;
    unsigned long flags;

    osal_spin_lock_irqsave(&queue->spin_lock, &flags);

    free_num = queue->free_num;

    osal_spin_unlock_irqrestore(&queue->spin_lock, &flags);
    return free_num;
}

hi_u32 isp_queue_get_busy_num(isp_be_buf_queue *queue)
{
    hi_u32 busy_num;
    unsigned long flags;

    osal_spin_lock_irqsave(&queue->spin_lock, &flags);

    busy_num = queue->busy_num;

    osal_spin_unlock_irqrestore(&queue->spin_lock, &flags);
    return busy_num;
}

