/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include "osal.h"
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

#if 0
#ifdef CONFIG_ARM_GIC

#define DIS_IRQ_CNT 2
struct gic_sgi_handle {
    unsigned int irq;
    void (*handle)(unsigned int cpu_intrf,
                   unsigned int irq_num,
                   struct pt_regs *regs);
};
int osal_register_gic_handle(unsigned int index, unsigned int irq,
    osal_gic_handle_t handle, const char *name, void *dev)
{
    extern struct gic_sgi_handle dis_irq_handle[DIS_IRQ_CNT];


    if (index >= DIS_IRQ_CNT) {
        printk("index(%d) is error.\n", index);
        return -1;
    }
    dis_irq_handle[index].irq = irq;
    dis_irq_handle[index].handle = (void (*)(unsigned int, unsigned int, struct pt_regs *))(handle);

    return 0;
}
EXPORT_SYMBOL(osal_register_gic_handle);

int osal_unregister_gic_handle(unsigned int index, unsigned int irq, void *dev)
{
    extern struct gic_sgi_handle dis_irq_handle[DIS_IRQ_CNT];

    if (index >= DIS_IRQ_CNT) {
        printk("index(%d) is error.\n", index);
        return -1;
    }
    dis_irq_handle[index].irq = 0xF;
    dis_irq_handle[index].handle = NULL;

    return 0;
}
EXPORT_SYMBOL(osal_unregister_gic_handle);
#endif
#endif
