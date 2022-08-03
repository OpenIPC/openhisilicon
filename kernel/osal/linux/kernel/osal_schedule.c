/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include "osal.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>

void osal_yield(void)
{
	// yield();
	cond_resched();
}
EXPORT_SYMBOL(osal_yield);
