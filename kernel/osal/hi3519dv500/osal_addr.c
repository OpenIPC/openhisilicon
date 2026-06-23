/**
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <asm/io.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/mm.h>
#include "ot_osal.h"
#include "osal_inner.h"

#define OSAL_ADDR_RESERVED_SIZE_MAX 0xF000000

static unsigned int osal_convert_gfp_flag(unsigned int osal_gfp_flag)
{
    unsigned int gfp_flag = 0;

    if (osal_gfp_flag & OSAL_GFP_KERNEL) {
        gfp_flag = GFP_KERNEL;
    } else if (osal_gfp_flag & OSAL_GFP_ATOMIC) {
        gfp_flag = GFP_ATOMIC;
    } else if (osal_gfp_flag & OSAL_GFP_DMA) {
        gfp_flag = GFP_DMA;
    } else {
        gfp_flag = GFP_KERNEL;
    }

    if (osal_gfp_flag & OSAL_GFP_ZERO) {
        gfp_flag |= __GFP_ZERO;
    }
    return gfp_flag;
}

void *osal_kmalloc(unsigned long size, unsigned int osal_gfp_flag)
{
    unsigned int gfp_flag = osal_convert_gfp_flag(osal_gfp_flag);
    return kmalloc(size, gfp_flag);
}
EXPORT_SYMBOL(osal_kmalloc);

void osal_kfree(void *addr)
{
    if (addr != NULL) {
        kfree(addr);
    }
    return;
}
EXPORT_SYMBOL(osal_kfree);

void *osal_vmalloc(unsigned long size)
{
    return vmalloc(size);
}
EXPORT_SYMBOL(osal_vmalloc);

void osal_vfree(void *addr)
{
    if (addr != NULL) {
        vfree(addr);
    }
    return;
}
EXPORT_SYMBOL(osal_vfree);

void *osal_ioremap(unsigned long phys_addr, unsigned long size)
{
    return ioremap(phys_addr, size);
}
EXPORT_SYMBOL(osal_ioremap);

void *osal_ioremap_nocache(unsigned long phys_addr, unsigned long size)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
    return ioremap(phys_addr, size);
#else
    return ioremap_nocache(phys_addr, size);
#endif
}
EXPORT_SYMBOL(osal_ioremap_nocache);

void *osal_ioremap_wc(unsigned long phys_addr, unsigned long size)
{
    return ioremap_wc(phys_addr, size);
}
EXPORT_SYMBOL(osal_ioremap_wc);

void *osal_ioremap_cached(unsigned long phys_addr, unsigned long size)
{
    return ioremap_cache(phys_addr, size);
}
EXPORT_SYMBOL(osal_ioremap_cached);

void osal_iounmap(void *addr, unsigned long size)
{
    iounmap(addr);
}
EXPORT_SYMBOL(osal_iounmap);

unsigned long osal_copy_from_user(void *to, const void *from, unsigned long n)
{
    return copy_from_user(to, from, n);
}
EXPORT_SYMBOL(osal_copy_from_user);

unsigned long osal_copy_to_user(void *to, const void *from, unsigned long n)
{
    return copy_to_user(to, from, n);
}
EXPORT_SYMBOL(osal_copy_to_user);

int osal_access_ok(int type, const void *addr, unsigned long size)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
    return access_ok(addr, size);
#else
    return access_ok(type, addr, size);
#endif
}
EXPORT_SYMBOL(osal_access_ok);

void *osal_phys_to_virt(unsigned long addr)
{
    return phys_to_virt(addr);
}
EXPORT_SYMBOL(osal_phys_to_virt);

unsigned long osal_virt_to_phys(const void *virt_addr)
{
    return virt_to_phys(virt_addr);
}
EXPORT_SYMBOL(osal_virt_to_phys);

static void *osal_blockmem_vmap_proc(bool is_cache, unsigned long phys_addr, unsigned long size)
{
    unsigned int i;
    unsigned int page_count;
    struct page **pages = NULL;
    void *vaddr = NULL;
    pgprot_t pgprot = PAGE_KERNEL;

    if ((phys_addr == 0) || (size == 0) || size > OSAL_ADDR_RESERVED_SIZE_MAX) {
        osal_log("invalid vmap address: 0x%lX or size:%lu!\n", phys_addr, size);
        return NULL;
    }

    page_count = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    pages = vmalloc(page_count * sizeof(struct page *));
    if (pages == NULL) {
        osal_log("vmap malloc pages failed!\n");
        return NULL;
    }

    for (i = 0; i < page_count; i++) {
        pages[i] = phys_to_page(phys_addr + i * PAGE_SIZE);
    }

    if (!is_cache) {
        pgprot = pgprot_writecombine(PAGE_KERNEL);
    }
    vaddr = vmap(pages, page_count, VM_MAP, pgprot);
    if (vaddr == NULL) {
        osal_log("vmap failed phys_addr:0x%lX, size:%lu!\n", phys_addr, size);
    }

    vfree(pages);
    pages = NULL;

    return vaddr;
}

void *osal_blockmem_vmap(unsigned long phys_addr, unsigned long size)
{
    return osal_blockmem_vmap_proc(true, phys_addr, size);
}
EXPORT_SYMBOL(osal_blockmem_vmap);

void *osal_blockmem_vmap_nocache(unsigned long phys_addr, unsigned long size)
{
    return osal_blockmem_vmap_proc(false, phys_addr, size);
}
EXPORT_SYMBOL(osal_blockmem_vmap_nocache);

/*
 * Free the virtually contiguous memory area starting at @virt_addr
 * which was created from the phys_addr passed to osal_vunmap()
 * Must not be called in interrupt context.
 * */
void osal_blockmem_vunmap(const void *virt_addr)
{
    if (virt_addr == NULL) {
        osal_log("vumap failed: virt_addr is NULL!\n");
        return;
    }

    vunmap(virt_addr);
}
EXPORT_SYMBOL(osal_blockmem_vunmap);

/*
 * Free the reserved memory which has been defined in product
 **/
void osal_blockmem_free(unsigned long phys_addr, unsigned long size)
{
    unsigned long pfn_start;
    unsigned long pfn_end;

    if ((phys_addr == 0) || (size == 0)) {
        osal_log("Free block memory failed: phys_addr 0x%lX,size %lu!\n", phys_addr, size);
        return;
    }

    pfn_start = __phys_to_pfn(phys_addr);
    pfn_end = __phys_to_pfn(phys_addr + size);

    for (; pfn_start < pfn_end; pfn_start++) {
        struct page *page = pfn_to_page(pfn_start);
        ClearPageReserved(page);
        init_page_count(page);
        __free_page(page);
        adjust_managed_page_count(page, 1); /* 1 block mem page count */
    }

    return;
}
EXPORT_SYMBOL(osal_blockmem_free);

osal_blockmem_status osal_blockmem_get_status(unsigned long phyaddr, unsigned int size)
{
    unsigned long pfn_start;
    unsigned long pfn_end;
    unsigned int valid_pages = 0;
    osal_blockmem_status status;

    pfn_start = __phys_to_pfn(phyaddr);
    pfn_end = __phys_to_pfn(phyaddr + size);

    for (; pfn_start < pfn_end; pfn_start++) {
        struct page *page = pfn_to_page(pfn_start);
        if (PageReserved(page) == 0) {
            break;
        }
        valid_pages++;
    }

    if (valid_pages == (size >> PAGE_SHIFT)) {
        status = OSAL_BLOCKMEM_VALID;
    } else if (valid_pages == 0) {
        status = OSAL_BLOCKMEM_INVALID_PHYADDR;
    } else {
        status = OSAL_BLOCKMEM_INVALID_SIZE;
    }

    return status;
}
EXPORT_SYMBOL(osal_blockmem_get_status);
