/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2016-2019.
 * Description: osal debug source file.
 * Author: Hisilicon multimedia software group
 * Create: 2016-11-11
 */

#include "hi_osal.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>

int osal_printk(const char *fmt, ...)
{
    va_list args;
    int r;

    va_start(args, fmt);
    r = vprintk(fmt, args);
    va_end(args);

    return r;
}
EXPORT_SYMBOL(osal_printk);

void osal_panic(const char *fmt, const char *fun, int line, const char *cond)
{
    panic(fmt, fun, line, cond);
}
EXPORT_SYMBOL(osal_panic);
