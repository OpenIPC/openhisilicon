/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/workqueue.h>
#include <linux/slab.h>
#include <linux/module.h>
#include "ot_osal.h"
#include "osal_inner.h"

static DEFINE_SPINLOCK(lock);
OSAL_LIST_HEAD(dw_list);

struct dw_node {
    osal_delayedwork *osal_work;
    struct delayed_work *dwork;
    struct osal_list_head node;
};

static osal_delayedwork *osal_find_dwork(const struct work_struct *wwork)
{
    struct osal_list_head *o_this = NULL;
    struct delayed_work *dwork = NULL;
    unsigned long flags;

    if (wwork == NULL) {
        osal_log("parameter invalid!\n");
        return NULL;
    }

    spin_lock_irqsave(&lock, flags);
    dwork = osal_list_entry(wwork, struct delayed_work, work);
    if (osal_list_empty(&dw_list)) {
        osal_log("find work failed! wq_list is empty!\n");
        spin_unlock_irqrestore(&lock, flags);
        return NULL;
    }
    osal_list_for_each(o_this, &dw_list) {
        struct dw_node *ws = osal_list_entry(o_this, struct dw_node, node);
        if (ws->dwork == dwork) {
            spin_unlock_irqrestore(&lock, flags);
            return ws->osal_work;
        }
    }

    osal_log("find work failed!\n");
    spin_unlock_irqrestore(&lock, flags);
    return NULL;
}

static void osal_delayed_handler(struct work_struct *work)
{
    osal_delayedwork *ow = osal_find_dwork(work);
    if ((ow != NULL) && (ow->handler != NULL)) {
        ow->handler(ow);
    }
}

int osal_delayedwork_init(osal_delayedwork *work, osal_delayedwork_handler handler)
{
    struct delayed_work *w = NULL;
    struct dw_node *dw_node = NULL;
    unsigned long flags;

    if (work == NULL || work->work != NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }

    w = kmalloc(sizeof(struct delayed_work), GFP_ATOMIC);
    if (w == NULL) {
        osal_log("kmalloc failed!\n");
        return OSAL_FAILURE;
    }

    dw_node = kmalloc(sizeof(struct dw_node), GFP_ATOMIC);
    if (dw_node == NULL) {
        osal_log("kmalloc failed!\n");
        kfree(w);
        return OSAL_FAILURE;
    }
    INIT_DELAYED_WORK(w, osal_delayed_handler);
    work->work = w;
    work->handler = handler;
    dw_node->osal_work = work;
    dw_node->dwork = w;
    spin_lock_irqsave(&lock, flags);
    osal_list_add(&(dw_node->node), &dw_list);
    spin_unlock_irqrestore(&lock, flags);

    return OSAL_SUCCESS;
}
EXPORT_SYMBOL(osal_delayedwork_init);

static int osal_del_dwork(const struct delayed_work *dwork)
{
    struct osal_list_head *o_this = NULL;
    struct osal_list_head *next = NULL;
    unsigned long flags;

    if (dwork == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }

    spin_lock_irqsave(&lock, flags);
    if (osal_list_empty(&dw_list)) {
        osal_log("find work failed! wq_list is empty!\n");
        spin_unlock_irqrestore(&lock, flags);
        return OSAL_FAILURE;
    }

    osal_list_for_each_safe(o_this, next, &dw_list) {
        struct dw_node *ws = osal_list_entry(o_this, struct dw_node, node);
        if (ws->dwork == dwork) {
            spin_unlock_irqrestore(&lock, flags);
            osal_list_del(o_this);
            kfree(ws);
            return OSAL_SUCCESS;
        }
    }
    osal_log("del work failed!\n");
    spin_unlock_irqrestore(&lock, flags);

    return OSAL_FAILURE;
}

void osal_delayedwork_destroy(osal_delayedwork *work)
{
    if ((work != NULL) && (work->work != NULL)) {
        osal_del_dwork(work->work);
        kfree((struct delayed_work *)work->work);
        work->work = NULL;
    }
}
EXPORT_SYMBOL(osal_delayedwork_destroy);

int osal_delayedwork_schedule(osal_delayedwork *work, int timeout)
{
    if (work == NULL || work->work == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    schedule_delayed_work(work->work, osal_msecs_to_jiffies(timeout));
    return OSAL_SUCCESS;
}
EXPORT_SYMBOL(osal_delayedwork_schedule);

int osal_delayedwork_cancel_sync(osal_delayedwork *work)
{
    if (work == NULL || work->work == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    cancel_delayed_work_sync(work->work);
    return OSAL_SUCCESS;
}
EXPORT_SYMBOL(osal_delayedwork_cancel_sync);
