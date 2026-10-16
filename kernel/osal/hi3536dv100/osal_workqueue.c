#include <linux/workqueue.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include "hi_osal.h"

OSAL_LIST_HEAD(wq_list);
/* wq_list is walked by every work callback and changed by init/destroy, which
 * callers may reach from atomic context (hence GFP_ATOMIC below). */
static DEFINE_SPINLOCK(wq_lock);
struct wq_node{
	struct osal_work_struct *osal_work;
	struct work_struct *work;
	struct task_struct *runner;	/* task inside the callback, NULL otherwise */
	struct osal_list_head node;
};

/* 4.9 has no current_work(), so the callback records who is running it and
 * destroy can tell when it is being called from inside that callback. */
static void osal_work_set_runner(struct work_struct *work, struct task_struct *task)
{
	struct osal_list_head *this;
	unsigned long flags;

	spin_lock_irqsave(&wq_lock, flags);
	osal_list_for_each(this, &wq_list){
		struct wq_node *ws = osal_list_entry(this, struct wq_node, node);
		if(ws->work == work){
			ws->runner = task;
			break;
		}
	}
	spin_unlock_irqrestore(&wq_lock, flags);
}

static int osal_work_run_by_current(struct work_struct *work)
{
	struct osal_list_head *this;
	unsigned long flags;
	int self = 0;

	spin_lock_irqsave(&wq_lock, flags);
	osal_list_for_each(this, &wq_list){
		struct wq_node *ws = osal_list_entry(this, struct wq_node, node);
		if(ws->work == work){
			self = (ws->runner == current);
			break;
		}
	}
	spin_unlock_irqrestore(&wq_lock, flags);
	return self;
}

static struct osal_work_struct * osal_find_work(struct work_struct *work)
{
	struct osal_list_head *this;
	struct osal_work_struct *found = NULL;
	unsigned long flags;

	spin_lock_irqsave(&wq_lock, flags);
	osal_list_for_each(this, &wq_list){
		struct wq_node *ws = osal_list_entry(this, struct wq_node, node);
		if(ws->work == work){
			found = ws->osal_work;
			break;
		}
	}
	spin_unlock_irqrestore(&wq_lock, flags);
	if(found == NULL)
		osal_printk("find work failed!\n");
	return found;
}

static int osal_del_work(struct work_struct *work)
{
	struct osal_list_head *this, *next;
	struct wq_node *found = NULL;
	unsigned long flags;

	spin_lock_irqsave(&wq_lock, flags);
	osal_list_for_each_safe(this, next, &wq_list){
		struct wq_node *ws = osal_list_entry(this, struct wq_node, node);
		if(ws->work == work){
			osal_list_del(this);
			found = ws;
			break;
		}
	}
	spin_unlock_irqrestore(&wq_lock, flags);
	if(found == NULL){
		osal_printk("del work failed!\n");
		return -1;
	}
	kfree(found);
	return 0;
}

static void osal_work_func(struct work_struct *work)
{
	//struct osal_work_struct *ow = container_of(work, struct osal_work_struct, work);
	struct osal_work_struct *ow = osal_find_work(work);
	if(ow != NULL && ow->func != NULL){
		osal_work_set_runner(work, current);
		ow->func(ow);
		/* a no-op when the callback destroyed its own work: the node stays */
		osal_work_set_runner(work, NULL);
	}
}
/*
void osal_workqueue_init(){
	OSAL_INIT_LIST_HEAD(&wq_list);	
}
EXPORT_SYMBOL(osal_workqueue_init);
*/
int osal_init_work(struct osal_work_struct *work, osal_work_func_t func)
{
	struct work_struct *w = NULL;
	struct wq_node *w_node = NULL;
	w = kmalloc(sizeof(struct work_struct), GFP_ATOMIC);
	if(w == NULL){
		osal_printk("osal_init_work kmalloc failed!\n");
		return -1;
	}

	w_node = kmalloc(sizeof(struct wq_node), GFP_ATOMIC);
	if(w_node == NULL){
		osal_printk("osal_init_work kmalloc failed!\n");
		kfree(w);
		return -1;
	}
	INIT_WORK(w, osal_work_func);
	work->work = w;
	work->func = func;
	w_node->osal_work = work;
	w_node->work = w;
	w_node->runner = NULL;
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
	if(work!=NULL && work->work!=NULL)
		return (int)schedule_work(work->work);
	else
		return (int)false;
}
EXPORT_SYMBOL(osal_schedule_work);

/*
 * The work item may still be queued, or running on another CPU, when this is
 * called, so it is cancelled and waited for before it is freed (same fix as
 * gk7206v100). Waiting is impossible from interrupt context, and from inside
 * the work's own callback it would wait for itself; in both cases the item is
 * leaked rather than freed under a callback that may still use it.
 */
void osal_destroy_work(struct osal_work_struct *work)
{
	struct work_struct *w = NULL;

	if(work == NULL || work->work == NULL)
		return;
	w = work->work;
	if(in_interrupt() || osal_work_run_by_current(w)){
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


