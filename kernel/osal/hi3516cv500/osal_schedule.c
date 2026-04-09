/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2016-2019.
 * Description: osal schedule source file.
 * Author: Hisilicon multimedia software group
 * Create: 2016-11-11
 */

#include "hi_osal.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>

void osal_yield(void)
{
    cond_resched();
}
EXPORT_SYMBOL(osal_yield);
