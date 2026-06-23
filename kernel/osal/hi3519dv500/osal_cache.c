/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <asm/cacheflush.h>
#include <asm/shmparam.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/mm.h>
#include <linux/dma-direction.h>
#include "ot_osal.h"
#include "osal_inner.h"

void osal_dcache_region_wb(void *kvirt, unsigned long phys_addr, unsigned long size)
{
    /* flush dcache byaddr */
#ifdef __aarch64__
    __flush_dcache_area(kvirt, (size_t)size);
#else
    __cpuc_flush_dcache_area(kvirt, (size_t)size);
#endif

#if defined(CONFIG_CACHE_HIL2V200) || defined(CONFIG_CACHE_L2X0)
    /* flush l2 cache, use paddr */
    /*
     * if length > L2 cache size, then this interface
     * will call <outer_flush_all>
     */
    outer_flush_range(phys_addr, phys_addr + size);
#endif
}
EXPORT_SYMBOL(osal_dcache_region_wb);

void osal_dcache_region_inv(void *addr, unsigned long size)
{
#ifdef __aarch64__
    __inval_dcache_area(addr, (size_t)size);
#else
    printk("dcache_region_inv not support for this framework\n");
#endif
    return;
}
EXPORT_SYMBOL(osal_dcache_region_inv);
