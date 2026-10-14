/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/semaphore.h>
#include <linux/slab.h>
#include "osal.h"
#include "osal_mmz.h"
#define MMZ_NAME "anonymous"

unsigned long long mmz_alloc(const char *mmz_name, const char *buf_name, unsigned long size)
{
    mmz_mmb_t *pmmb = NULL;

    pmmb = mmz_mmb_alloc(buf_name, size, 0, 0, mmz_name);
    if (pmmb == NULL) {
        osal_printk("Mmz malloc failed!\n");
        return 0UL;
    }

    return mmz_mmb_phys(pmmb);
}
EXPORT_SYMBOL(mmz_alloc);

void mmz_free(unsigned long long phy_addr)
{
    mmz_mmb_freeby_phys(phy_addr);
}
EXPORT_SYMBOL(mmz_free);

void *mmz_map(unsigned long long phy_addr, unsigned long size, bool cached)
{
    mmz_mmb_t *pmmb = NULL;
    void *vir_addr = NULL;
    unsigned long offset;

    pmmb = mmz_mmb_getby_phys_2(phy_addr, &offset);
    if ( pmmb == NULL ) {
        osal_printk("mmz remap cache get mmz_mmb failed\n");
        return NULL;
    }

    if (size == 0 || size + offset > pmmb->length) {
        osal_printk("mmz remap size(0x%lx) is zero or bigger than mmb size(0x%lx)", size, pmmb->length);
        return NULL;
    }

    if ( cached == 1 ) {
        vir_addr = mmz_mmb_map2kern_cached(pmmb);
    } else {
        vir_addr = mmz_mmb_map2kern(pmmb);
    }
    if (vir_addr == NULL) {
        osal_printk("mmz remap failed!\n");
        return NULL;
    }

    return vir_addr + offset;
}
EXPORT_SYMBOL(mmz_map);

void mmz_unmap(void *vir_addr)
{
    if (vir_addr != NULL) {
        mmz_mmb_t *pmmb = mmz_mmb_getby_kvirt(vir_addr);

        if (pmmb != NULL) {
            mmz_mmb_unmap(pmmb);
        }
    }
}
EXPORT_SYMBOL(mmz_unmap);

int mmz_check_phyaddr(unsigned long long phy_addr, unsigned long len)
{
    /* if address in mmz of current system */
    if (mmz_is_phys_in_mmz(phy_addr, len))
        return -1;

    return 0;
}
EXPORT_SYMBOL(mmz_check_phyaddr);

int mmz_flush_cache(unsigned long phy_addr, void *kvirt, unsigned long length)
{
    mmz_mmb_t *pmmb = NULL;
    unsigned long offset;

    /*err address flush maybe panic so judge input parameter*/
    pmmb = mmz_mmb_getby_phys_2(phy_addr, &offset);
    if ( pmmb == NULL ) {
        osal_printk("mmz flush cache get mmz_mmb failed\n");
        return -1;
    }

    if ( mmz_mmb_flush_dcache_byaddr(kvirt, phy_addr, length) != 0 )
        return -1;

   return 0;
}
EXPORT_SYMBOL(mmz_flush_cache);
