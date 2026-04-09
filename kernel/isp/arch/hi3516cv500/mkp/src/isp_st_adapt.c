/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_st_adapt.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_st.h"

/* isp sync task */
#define  ISP_SYNC_TSK_MAX_NODES  16
#define  ISP_SYNC_TSK_MAX_NUM    200

typedef void (*ISP_FUNC_PTR)(unsigned long data);

isp_sync_tsk_ctx g_isp_sync_tsk_ctx[ISP_MAX_PIPE_NUM];
#define isp_sync_tsk_get_ctx(dev) (&g_isp_sync_tsk_ctx[dev])

hi_void isp_sync_task_find_and_execute(hi_vi_pipe vi_pipe, struct osal_list_head *head)
{
    struct osal_list_head *pos = HI_NULL;
    struct osal_list_head *next = HI_NULL;
    isp_sync_task_node *sync_tsk_node = HI_NULL;

    if (!osal_list_empty(head)) {
        osal_list_for_each_safe(pos, next, head) {
            sync_tsk_node = osal_list_entry(pos, isp_sync_task_node, list);
            if (sync_tsk_node->focus_stat) {
                isp_drv_sta_kernel_get(vi_pipe, sync_tsk_node->focus_stat);
            }
            if (sync_tsk_node->isp_sync_tsk_call_back) {
                sync_tsk_node->isp_sync_tsk_call_back(sync_tsk_node->data);
            }
        }
    }

    return;
}

static void work_queue_handler(struct osal_work_struct *worker)
{
    isp_sync_tsk_ctx *sync_tsk = osal_container_of((void *)worker, isp_sync_tsk_ctx, worker);

    if (osal_down_interruptible(&sync_tsk->sem)) {
        return;
    }

    isp_sync_task_find_and_execute(sync_tsk->vi_pipe, &sync_tsk->workqueue_list.head);

    osal_up(&sync_tsk->sem);

    return;
}

hi_s32 isp_sync_task_process(hi_vi_pipe vi_pipe)
{
    isp_sync_tsk_ctx *sync_tsk = isp_sync_tsk_get_ctx(vi_pipe);

    if (sync_tsk->hwirq_list.num) {
        isp_sync_task_find_and_execute(vi_pipe, &sync_tsk->hwirq_list.head);
    }

    if (sync_tsk->tsklet_list.num) {
    }

    if (sync_tsk->workqueue_list.num) {
        osal_schedule_work(&sync_tsk->worker);
    }

    return HI_SUCCESS;
}

static struct osal_list_head *search_node(struct osal_list_head *head, const char *id)
{
    struct osal_list_head *pos = HI_NULL;
    struct osal_list_head *next = HI_NULL;
    isp_sync_task_node *sync_tsk_node = HI_NULL;

    osal_list_for_each_safe(pos, next, head) {
        sync_tsk_node = osal_list_entry(pos, isp_sync_task_node, list);
        if (!osal_strncmp(sync_tsk_node->sz_id, id, HI_ISP_SYNC_TASK_ID_MAX_LENGTH)) {
            return pos;
        }
    }

    return HI_NULL;
}

hi_s32 isp_sync_task_register(hi_vi_pipe vi_pipe, isp_sync_task_node *new_node)
{
    isp_sync_tsk_ctx *sync_tsk = isp_sync_tsk_get_ctx(vi_pipe);
    struct osal_list_head *target_list = HI_NULL;
    struct osal_list_head *pos = HI_NULL;
    list_entry *list_entry_tmp = HI_NULL;

    isp_check_pointer_return(new_node);

    if (new_node->method == ISP_SYNC_TSK_METHOD_HW_IRQ) {
        target_list = &sync_tsk->hwirq_list.head;
    } else {
        target_list = &sync_tsk->workqueue_list.head;
    }

    list_entry_tmp = osal_list_entry(target_list, list_entry, head);
    if (list_entry_tmp == HI_NULL) {
        return HI_FAILURE;
    }

    pos = search_node(target_list, new_node->sz_id);
    if (pos) {
        return HI_FAILURE;
    }

    if (osal_down_interruptible(&sync_tsk->sem)) {
        return -ERESTARTSYS;
    }

    osal_list_add_tail(&new_node->list, target_list);

    if (list_entry_tmp->num < ISP_SYNC_TSK_MAX_NUM) {
        list_entry_tmp->num++;
    }

    osal_up(&sync_tsk->sem);

    return HI_SUCCESS;
}

hi_s32 isp_sync_task_unregister(hi_vi_pipe vi_pipe, isp_sync_task_node *del_node)
{
    isp_sync_tsk_ctx *sync_tsk = isp_sync_tsk_get_ctx(vi_pipe);
    struct osal_list_head *target_list = HI_NULL;
    list_entry *list_entry_tmp = HI_NULL;
    struct osal_list_head *pos = HI_NULL;
    hi_s32 del_success = HI_FAILURE;

    isp_check_pointer_return(del_node);

    if (del_node->method == ISP_SYNC_TSK_METHOD_HW_IRQ) {
        target_list = &sync_tsk->hwirq_list.head;
    } else {
        target_list = &sync_tsk->workqueue_list.head;
    }
    list_entry_tmp = osal_list_entry(target_list, list_entry, head);
    if (list_entry_tmp == HI_NULL) {
        return HI_FAILURE;
    }

    if (osal_down_interruptible(&sync_tsk->sem)) {
        return -ERESTARTSYS;
    }

    pos = search_node(target_list, del_node->sz_id);
    if (pos) {
        osal_list_del(pos);
        if (list_entry_tmp->num > 0) {
            list_entry_tmp->num = list_entry_tmp->num - 1;
        }

        del_success = HI_SUCCESS;
    }

    osal_up(&sync_tsk->sem);

    return del_success;
}

void sync_task_init(hi_vi_pipe vi_pipe)
{
    isp_sync_tsk_ctx *sync_tsk = isp_sync_tsk_get_ctx(vi_pipe);

    OSAL_INIT_LIST_HEAD(&sync_tsk->hwirq_list.head);
    OSAL_INIT_LIST_HEAD(&sync_tsk->tsklet_list.head);
    OSAL_INIT_LIST_HEAD(&sync_tsk->workqueue_list.head);

    sync_tsk->hwirq_list.num     = 0;
    sync_tsk->tsklet_list.num    = 0;
    sync_tsk->workqueue_list.num = 0;
    osal_sema_init(&sync_tsk->sem, 1);

    OSAL_INIT_WORK(&sync_tsk->worker, work_queue_handler);

    return;
}

void sync_task_exit(hi_vi_pipe vi_pipe)
{
    isp_sync_tsk_ctx *sync_tsk = isp_sync_tsk_get_ctx(vi_pipe);

    osal_destroy_work(&sync_tsk->worker);
    osal_sema_destory(&sync_tsk->sem);

    return;
}

