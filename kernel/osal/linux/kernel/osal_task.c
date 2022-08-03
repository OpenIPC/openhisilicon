/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include "osal.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/kthread.h>
#include <linux/slab.h>

osal_task_t *osal_kthread_create(threadfn_t thread, void *data,
				 const char *name)
{
	struct task_struct *k = NULL;
	osal_task_t *p =
		(osal_task_t *)kmalloc(sizeof(osal_task_t), GFP_KERNEL);
	if (p == NULL) {
		osal_printk("%s - kmalloc error!\n", __FUNCTION__);
		return NULL;
	}
	memset(p, 0, sizeof(osal_task_t));

	k = kthread_run(thread, data, name);
	if (IS_ERR(k)) {
		osal_printk("%s - kthread create error!\n", __FUNCTION__);
		kfree(p);
		return NULL;
	}
	p->task_struct = k;
	return p;
}
EXPORT_SYMBOL(osal_kthread_create);

void osal_kthread_destory(osal_task_t *task, unsigned int stop_flag)
{
	if (task == NULL) {
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return;
	}
	/* note: When you call the Kthread_stop function, the thread function cannot be finished, otherwise it will oops. */
	if (stop_flag != 0) {
		kthread_stop((struct task_struct *)(task->task_struct));
	}
	task->task_struct = NULL;
	kfree(task);
}
EXPORT_SYMBOL(osal_kthread_destory);
