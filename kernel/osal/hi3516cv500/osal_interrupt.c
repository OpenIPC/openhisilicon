/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2016-2019.
 * Description: osal interrupt source file.
 * Author: Hisilicon multimedia software group
 * Create: 2016-11-11
 */

#include "hi_osal.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/interrupt.h>

int osal_request_irq(unsigned int irq, osal_irq_handler_t handler, osal_irq_handler_t thread_fn, const char *name,
                     void *dev)
{
    unsigned long flags = IRQF_SHARED;

    return request_threaded_irq(irq, (irq_handler_t)handler, (irq_handler_t)thread_fn, flags, name, dev);
}
EXPORT_SYMBOL(osal_request_irq);
void osal_free_irq(unsigned int irq, void *dev)
{
    free_irq(irq, dev);
}
EXPORT_SYMBOL(osal_free_irq);

int osal_in_interrupt(void)
{
    return in_interrupt();
}
EXPORT_SYMBOL(osal_in_interrupt);
