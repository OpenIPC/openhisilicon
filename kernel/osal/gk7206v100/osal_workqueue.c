/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#include <linux/workqueue.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include "osal.h"

OSAL_LIST_HEAD(wq_list);
/* wq_list is walked by every work callback and changed by init/destroy, which
 * callers may reach from atomic context (hence GFP_ATOMIC below). */
static DEFINE_SPINLOCK(wq_lock);
struct wq_node {
    struct osal_work_struct *osal_work;
    struct work_struct *work;
    struct osal_list_head node;
};

static struct osal_work_struct *osal_find_work(struct work_struct *work)
{
    struct osal_list_head *this = NULL;
    struct osal_work_struct *found = NULL;
    unsigned long flags;

    spin_lock_irqsave(&wq_lock, flags);
    osal_list_for_each(this, &wq_list) {
        struct wq_node *ws = osal_list_entry(this, struct wq_node, node);
        if (ws->work == work) {
            found = ws->osal_work;
            break;
        }
    }
    spin_unlock_irqrestore(&wq_lock, flags);
    if (found == NULL) {
        osal_printk("find work failed!\n");
    }
    return found;
}

static int osal_del_work(struct work_struct *work)
{
    struct osal_list_head *this = NULL;
    struct osal_list_head *next = NULL;
    struct wq_node *found = NULL;
    unsigned long flags;

    spin_lock_irqsave(&wq_lock, flags);
    osal_list_for_each_safe(this, next, &wq_list) {
        struct wq_node *ws = osal_list_entry(this, struct wq_node, node);
        if (ws->work == work) {
            osal_list_del(this);
            found = ws;
            break;
        }
    }
    spin_unlock_irqrestore(&wq_lock, flags);
    if (found == NULL) {
        osal_printk("del work failed!\n");
        return -1;
    }
    kfree(found);
    return 0;
}

static void osal_work_func(struct work_struct *work)
{
    struct osal_work_struct *ow = osal_find_work(work);
    if ((ow != NULL) && (ow->func != NULL)) {
        ow->func(ow);
    }
}

int osal_init_work(struct osal_work_struct *work, osal_work_func_t func)
{
    struct work_struct *w = NULL;
    struct wq_node *w_node = NULL;
    w = kmalloc(sizeof(struct work_struct), GFP_ATOMIC);
    if (w == NULL) {
        osal_printk("osal_init_work kmalloc failed!\n");
        return -1;
    }

    w_node = kmalloc(sizeof(struct wq_node), GFP_ATOMIC);
    if (w_node == NULL) {
        osal_printk("osal_init_work kmalloc failed!\n");
        kfree(w);
        return -1;
    }
    INIT_WORK(w, osal_work_func);
    work->work = w;
    work->func = func;
    w_node->osal_work = work;
    w_node->work = w;
    {
        unsigned long flags;

        spin_lock_irqsave(&wq_lock, flags);
        osal_list_add(&(w_node->node), &wq_list);
        spin_unlock_irqrestore(&wq_lock, flags);
    }
    return 0;
}
EXPORT_SYMBOL(osal_init_work);

int osal_schedule_work(struct osal_work_struct *work)
{
    if ((work != NULL) && (work->work != NULL)) {
        return (int)schedule_work(work->work);
    } else {
        return (int)false;
    }
}
EXPORT_SYMBOL(osal_schedule_work);

/*
 * The work item may still be queued, or running on the other CPU, when this is
 * called (the motion-sensor driver schedules it from its interrupt and destroys
 * it on stop), so it is cancelled and waited for before it is freed. Waiting is
 * impossible from interrupt context, and from inside the work's own callback it
 * would wait for itself; in both cases the item is leaked rather than freed
 * under a callback that may still use it.
 */
void osal_destroy_work(struct osal_work_struct *work)
{
    struct work_struct *w = NULL;

    if ((work == NULL) || (work->work == NULL)) {
        return;
    }
    w = work->work;
    if (in_interrupt() || current_work() == w) {
        osal_printk("%s - cannot wait for the work here, leaked rather than freed\n",
                    __FUNCTION__);
        return;
    }
    cancel_work_sync(w);
    osal_del_work(w);
    kfree(w);
    work->work = NULL;
}
EXPORT_SYMBOL(osal_destroy_work);


