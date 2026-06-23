/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <asm/barrier.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include "ot_osal.h"

void osal_mb(void)
{
    mb();
}
EXPORT_SYMBOL(osal_mb);
void osal_rmb(void)
{
    rmb();
}
EXPORT_SYMBOL(osal_rmb);
void osal_wmb(void)
{
    wmb();
}
EXPORT_SYMBOL(osal_wmb);
void osal_smp_mb(void)
{
    smp_mb();
}
EXPORT_SYMBOL(osal_smp_mb);
void osal_smp_rmb(void)
{
    smp_rmb();
}
EXPORT_SYMBOL(osal_smp_rmb);
void osal_smp_wmb(void)
{
    smp_wmb();
}
EXPORT_SYMBOL(osal_smp_wmb);
void osal_isb(void)
{
    isb();
}
EXPORT_SYMBOL(osal_isb);
void osal_dsb(void)
{
#ifdef CONFIG_64BIT
    dsb(sy);
#else
    dsb();
#endif
}
EXPORT_SYMBOL(osal_dsb);
void osal_dmb(void)
{
#ifdef CONFIG_64BIT
    dmb(sy);
#else
    dmb();
#endif
}
EXPORT_SYMBOL(osal_dmb);
