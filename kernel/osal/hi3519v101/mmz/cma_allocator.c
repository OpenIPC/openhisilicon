/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2016-2019.
 * Description: cma allocator source file.
 * Author: Hisilicon multimedia software group
 * Create: 2016-11-11
 */

#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/list.h>
#include <asm/cacheflush.h>

#include <asm/memory.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 16, 0)
#include <linux/cma.h>
#endif
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 16, 0)
#include <linux/dma-contiguous.h>
#endif
#include <linux/dma-mapping.h>
#include <asm/memory.h>
#include <asm/tlbflush.h>
#include <asm/pgtable.h>
#include <linux/vmalloc.h>

#include "allocator.h"
#include "mmz_arm.h"

#include "../../../compat/kernel_compat.h"

/* Externs from media-mem.c — V3A osal_mmz.h doesn't publish them. */
extern struct osal_list_head mmz_list;
extern int anony;

#define __use_vmalloc_space 1

struct cma_zone {
    struct device pdev;
    char name[NAME_LEN_MAX];
    unsigned long gfp;
    unsigned long phys_start;
    unsigned long nbytes;
    unsigned int alloc_type;
    unsigned long block_align;
};

long long g_max_malloc_size = 0x40000000UL;           /* 1GB */

static int do_mmb_alloc(hil_mmb_t *mmb)
{
    hil_mmb_t *p = NULL;

    mmz_trace_func();

    /* add mmb into zone, sorted */
    osal_list_for_each_entry(p, &mmb->zone->mmb_list, list) {
        if (mmb->phys_addr < p->phys_addr) {
            break;
        }

        if (mmb->phys_addr == p->phys_addr) {
            pr_err("ERROR:cma allocator bad in %s(%s, %d)",
                   mmb->zone->name, __func__, __LINE__);
            return -EFAULT;
        }
    }

    osal_list_add(&mmb->list, p->list.prev);
    mmz_trace(1, HIL_MMB_FMT_S, hil_mmb_fmt_arg(mmb));

    return 0;
}

static hil_mmb_t *__mmb_alloc(const char *name,
                              unsigned long size,
                              unsigned long align,
                              unsigned long gfp,
                              const char *mmz_name,
                              hil_mmz_t *_user_mmz)
{
    hil_mmz_t *mmz = NULL;
    hil_mmb_t *mmb = NULL;
    unsigned long order;
    size_t count;
    struct page *page = NULL;

    mmz_trace_func();

    /*
     * No zero length memory request, no supper
     * large memory (4GB and above) request.
     */
    if ((size == 0) || (size > g_max_malloc_size)) {
        return NULL;
    }

    if (align == 0) {
        align = MMZ_GRAIN;
    }

    size = mmz_grain_align(size);
    order = get_order(size);
    count = size >> PAGE_SHIFT;

    mmz_trace(1, "anonymous=%s,size=%luKB,align=%lu", mmz_name, size / SZ_1K, align);

    begin_list_for_each_mmz(mmz, gfp, mmz_name)

    if ((_user_mmz != NULL) && (_user_mmz != mmz)) {
        continue;
    }

    page = compat_dma_alloc_from_contiguous(mmz->cma_dev, count, order);
    if (page == NULL) {
        return NULL;
    }

    /* Get it */
    break;
    end_list_for_each_mmz()

    if (page == NULL) {
        return NULL;
    }

    /* clear the allocated buffer, if needed! */
    dma_buffer_clear(page, size);

    mmb = kmalloc(sizeof(hil_mmb_t), GFP_KERNEL);
    if (mmb == NULL) {
        pr_err("<%s,%d> mmb struct alloc failed\n", __func__, __LINE__);
        goto cma_free;
    }

    memset(mmb, 0, sizeof(hil_mmb_t));
    mmb->zone = mmz;
    mmb->phys_addr = page_to_phys(page);
    mmb->length = size;

    if (name != NULL) {
        compat_strlcpy(mmb->name, name, HIL_MMB_NAME_LEN);
    } else {
        strncpy(mmb->name, "<null>", HIL_MMB_NAME_LEN - 1);
    }

    if (do_mmb_alloc(mmb)) {
        goto mmb_free;
    }

    return mmb;

mmb_free:
    kfree(mmb);
    mmb = NULL;
cma_free:
    dma_release_from_contiguous(mmz->cma_dev, page, count);
    return NULL;
}

static hil_mmb_t *__mmb_alloc_v2(const char *name,
                                 unsigned long size,
                                 unsigned long align,
                                 unsigned long gfp,
                                 const char *mmz_name,
                                 hil_mmz_t *_user_mmz,
                                 unsigned int order)
{
    hil_mmz_t *mmz = NULL;
    hil_mmb_t *mmb = NULL;
    unsigned int i;
    unsigned long cma_order;
    size_t count = size >> PAGE_SHIFT;
    struct page *page = NULL;

    mmz_trace_func();

    /*
     * No zero length memory request, no supper
     * large memory (4GB and above) request.
     */
    if ((size == 0) || (size > g_max_malloc_size)) {
        return NULL;
    }

    if (align == 0) {
        align = 1;
    }

    size = mmz_grain_align(size);

    mmz_trace(1, "size=%luKB, align=%lu", size / SZ_1K, align);

    begin_list_for_each_mmz(mmz, gfp, mmz_name)

    if ((_user_mmz != NULL) && (_user_mmz != mmz)) {
        continue;
    }

    if (mmz->alloc_type == SLAB_ALLOC) {
        if ((size - 1) & size) {
            for (i = 1; i < 32; i++) { /* 32: the max size is 2^(32-1) */
                if (!((size >> i) & ~0)) {
                    size = 1 << i;
                    break;
                }
            }
        }
    } else if (mmz->alloc_type == EQ_BLOCK_ALLOC) {
        size = mmz_align2(size, mmz->block_align);
    }

    cma_order = get_order(size);
    page = compat_dma_alloc_from_contiguous(mmz->cma_dev, count, cma_order);
    if (page == NULL) {
        return NULL;
    }

    /* Get it */
    break;
    end_list_for_each_mmz()

    if (page == NULL) {
        return NULL;
    }

    /* clear the allocated buffer, if needed! */
    dma_buffer_clear(page, size);

    mmb = kmalloc(sizeof(hil_mmb_t), GFP_KERNEL);
    if (mmb == NULL) {
        pr_err("<%s,%d> mmb struct alloc failed\n", __func__, __LINE__);
        goto cma_free;
    }

    memset(mmb, 0, sizeof(hil_mmb_t));
    mmb->zone = mmz;
    mmb->phys_addr = page_to_phys(page);
    mmb->length = size;
    mmb->order = order;

    if (name != NULL) {
        compat_strlcpy(mmb->name, name, HIL_MMB_NAME_LEN);
    } else {
        strncpy(mmb->name, "<null>", HIL_MMB_NAME_LEN - 1);
    }

    if (do_mmb_alloc(mmb)) {
        goto mmb_free;
    }

    return mmb;

mmb_free:
    kfree(mmb);
    mmb = NULL;
cma_free:
    dma_release_from_contiguous(mmz->cma_dev, page, count);
    return NULL;
}

static void __mmb_free(hil_mmb_t *mmb)
{
    size_t count = mmb->length >> PAGE_SHIFT;
    struct page *page = phys_to_page(mmb->phys_addr);
    hil_mmz_t *mmz = mmb->zone;

    if (mmb->flags & HIL_MMB_MAP2KERN_CACHED) {
        mmb_dcache_flush(mmb);
    }

    dma_release_from_contiguous(mmz->cma_dev, page, count);

    osal_list_del(&mmb->list);
    kfree(mmb);
}

static void *__mmb_map2kern(hil_mmb_t *mmb, int cached)
{
    pgprot_t prot;
    struct page *page = phys_to_page(mmb->phys_addr);

    mmz_trace(1, "mmb[phys_addr=%lx,kvirt=%pK,length=%lx]",
              mmb->phys_addr, mmb->kvirt, mmb->length);

    if (mmb->flags & HIL_MMB_MAP2KERN) {
        if ((!!cached * HIL_MMB_MAP2KERN_CACHED) != (mmb->flags & HIL_MMB_MAP2KERN_CACHED)) {
            pr_err("mmb<%s> has been kernel-mapped %s, can not be re-mapped as %s.",
                   mmb->name,
                   (mmb->flags & HIL_MMB_MAP2KERN_CACHED) ? "cached" : "non-cached",
                   (cached) ? "cached" : "non-cached");
            return NULL;
        }

        mmz_trace(1, "map[%lx --> %pK]", mmb->phys_addr, mmb->kvirt);

        mmb->map_ref++;
        return mmb->kvirt;
    }

    prot = arch_kern_pgprot(cached);

#if __use_vmalloc_space
    /*
     * Map into vmalloc space
     */
    {
        int i;
        struct page **pages = NULL;
        unsigned int pagesnr = mmb->length / PAGE_SIZE;
        struct page *tmp = page;
        int array_size = sizeof(struct page *) * pagesnr;

        /*
         * Noted: mmb->length would be very large in some cases(for example:
         * more than one Giga Bytes). and array_size would be very large as
         * well. So, don't use kmalloc here.
         */
        pages = vmalloc(array_size);
        if (pages == NULL) {
            pr_err("ptr array(0x%x) vmalloc failed.\n", array_size);
            return NULL;
        }

        for (i = 0; i < pagesnr; i++) {
            *(pages + i) = tmp;
            tmp++;
        }

        mmb->kvirt = vmap(pages, pagesnr, VM_MAP, prot);
        vfree(pages);
        pages = NULL;
    }
#else
    /*
     * Map into linear space
     */
    {
        dma_pages_remap(page, mmb->length, prot);
        mmb->kvirt = __va(mmb->phys_addr);
    }
#endif
    if (mmb->kvirt == NULL) {
        /*
         * you should never get here.
         */
        printk(KERN_ERR "mmb[0x%lx, 0x%lx] map to kernel failed\n",
               mmb->phys_addr, mmb->length);
        return NULL;
    }

    if (cached) {
        mmb->flags |= HIL_MMB_MAP2KERN_CACHED;
    } else {
        mmb->flags &= ~HIL_MMB_MAP2KERN_CACHED;
    }
    mmb->flags |= HIL_MMB_MAP2KERN;
    mmb->map_ref++;

    mmz_trace(1, "map[%lx --> %pK]", mmb->phys_addr, mmb->kvirt);

    return mmb->kvirt;
}

static int __mmb_unmap(hil_mmb_t *mmb)
{
    int ref;

    mmz_trace(1, "mmb[phys_addr=%lx,kvirt=%pK,length=%lx]",
              mmb->phys_addr, mmb->kvirt, mmb->length);

    if (mmb->flags & HIL_MMB_MAP2KERN_CACHED) {
        mmb_dcache_flush(mmb);
    }

    if (mmb->flags & HIL_MMB_MAP2KERN) {
        ref = --mmb->map_ref;
        if (mmb->map_ref != 0) {
            return ref;
        }
    }

#if __use_vmalloc_space
    /*
     * unmap from vmalloc space.
     */
    {
        vunmap(mmb->kvirt);
    }
#endif
    mmb->kvirt = NULL;
    mmb->flags &= ~HIL_MMB_MAP2KERN;
    mmb->flags &= ~HIL_MMB_MAP2KERN_CACHED;

    if ((mmb->flags & HIL_MMB_RELEASED) && (mmb->phy_ref == 0)) {
        __mmb_free(mmb);
    }

    return 0;
}

/*
 * Map any valid phys address passed by users, to virtual address.
 * The input phys may not be 4Kbytes aligned.
 * FIX ME
 */
static void *__mmf_map(phys_addr_t phys, int len, int cache)
{
    struct page **pages = NULL;
    unsigned int pagesnr = len / PAGE_SIZE;
    int i;
    void *virt = NULL;
    pgprot_t prot;
    struct page *page = phys_to_page(phys);
    struct page *tmp = page;
    int array_size;

    mmz_trace(1, "phys=%lx, len=0x%x, cache=%d", (unsigned long)phys, len, cache);

    /*
     * if the given phys is not page aligned, we need to
     * map one more page for the leftover.
     */
    if (((phys & 0xfff) + len) > PAGE_SIZE) {
        pagesnr += 1;
    }

    array_size = sizeof(struct page *) * pagesnr;

    prot = arch_kern_pgprot(cache);

#if __use_vmalloc_space
    /*
     * Map into vmalloc space.
     */
    {
        /*
         * Noted: length of region may be very large in some cases(for example:
         * more than one Giga Bytes). and array_size would be very large as
         * well. So, don't use kmalloc here.
         */
        pages = vmalloc(array_size);
        if (pages == NULL) {
            printk(KERN_ERR "ptr vmalloc %d failed.\n", array_size);
            return NULL;
        }

        for (i = 0; i < pagesnr; i++) {
            *(pages + i) = tmp;
            tmp++;
        }

        virt = vmap(pages, pagesnr, VM_MAP, prot);
        vfree(pages);
        pages = NULL;

        if (virt == NULL) {
            return NULL;
        }

        virt += (phys & 0xfff);
    }
#else
    /*
     * Map into linear space
     */
    {
        dma_pages_remap(page, len, prot);
        virt = __va(phys);
    }
#endif

    mmz_trace(1, "map[%lx --> %pK]", (unsigned long)phys, virt);

    return virt;
}

static void __mmf_unmap(void *virt)
{
    unsigned long vaddr = (unsigned long)(uintptr_t)virt;

    mmz_trace(1, "virt=%pK", virt);

    /*
     * virtual address to be vunmaped should be page aligned
     */
    vaddr &= 0xfffff000;

    if ((vaddr >= VMALLOC_START) && (vaddr < VMALLOC_END)) {
        vunmap ((void *)(uintptr_t)vaddr);
    }
}

static int __allocator_init(char *s)
{
#ifdef CONFIG_CMA
    hil_mmz_t *zone = NULL;
    char *line = NULL;
    struct cma_zone *cma_zone = NULL;
    int attempted = 0, registered = 0;

    while ((line = strsep(&s, ":")) != NULL) {
        int i;
        char *argv[6]; /* 6: cmdline include six arguments */
        /*
         * FIX ME: We got 4 args in "line", formated as
         * "argv[0],argv[1],argv[2],argv[3],argv[4]".
         * eg: "<mmz_name>,<gfp>,<phys_start>,<size>,<alloc_type>"
         * For more convenient, "hard code" are used such as "arg[0]", i.e.
         */
        for (i = 0; (argv[i] = strsep(&line, ",")) != NULL;)
            if (++i == ARRAY_SIZE(argv)) {
                break;
            }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 16, 0)
        /* Mainline kernels: use default CMA area, no vendor zone lookup */
        cma_zone = NULL;
        {
            struct cma *default_cma = dev_get_cma_area(NULL);
            if (default_cma) {
                static struct cma_zone default_zone;
                default_zone.gfp = 0;
                default_zone.phys_start = cma_get_base(default_cma);
                default_zone.nbytes = cma_get_size(default_cma);
                cma_zone = &default_zone;
            }
        }
#else
        {
            extern struct cma_zone *hisi_get_cma_zone(const char *name);
            cma_zone = hisi_get_cma_zone(argv[0]);
        }
#endif
        if (cma_zone == NULL) {
            printk(KERN_ERR "can't get cma zone info:%s\n", argv[0]);
            continue;
        }

        if (i == 4) { /* 4: had parse four args */
            zone = hil_mmz_create("null", 0, 0, 0);
            if (zone == NULL) {
                continue;
            }

            compat_strlcpy(zone->name, argv[0], HIL_MMZ_NAME_LEN);

            printk("cmz zone gfp 0x%lx, phys 0x%lx, nbytes 0x%lx\n",
                   cma_zone->gfp,
                   cma_zone->phys_start,
                   cma_zone->nbytes);
            zone->gfp = cma_zone->gfp;
            zone->phys_start = cma_zone->phys_start;
            zone->nbytes = cma_zone->nbytes;
            zone->cma_dev = &cma_zone->pdev;

            if (zone->nbytes > g_max_malloc_size) {
                g_max_malloc_size = zone->nbytes;
            }
        } else if (i == 6) { /* 6: had parse six args */
            zone = hil_mmz_create_v2("null", 0, 0, 0, 0, 0);
            if (zone == NULL) {
                continue;
            }

            compat_strlcpy(zone->name, argv[0], HIL_MMZ_NAME_LEN);

            zone->gfp = cma_zone->gfp;
            zone->phys_start = cma_zone->phys_start;
            zone->nbytes = cma_zone->nbytes;
            zone->alloc_type = cma_zone->alloc_type;
            zone->block_align = cma_zone->block_align;
            zone->cma_dev = &cma_zone->pdev;

            if (zone->nbytes > g_max_malloc_size) {
                g_max_malloc_size = zone->nbytes;
            }
        } else {
            printk(KERN_ERR "Input parameter num incorrect!\n");
            continue;
        }

        attempted++;
        if (hil_mmz_register(zone)) {
            printk(KERN_WARNING "Add MMZ failed: " HIL_MMZ_FMT_S "\n",
                   hil_mmz_fmt_arg(zone));
            hil_mmz_destroy(zone);
        } else {
            registered++;
        }

        zone = NULL;
    }

    if (attempted > 0 && registered == 0) {
        printk(KERN_ERR "MMZ: all %d configured zone(s) failed to register; refusing to load\n",
                attempted);
        return -ENODEV;
    }
#endif

    return 0;
}

int cma_allocator_setopt(struct mmz_allocator *allocator)
{
    allocator->init = __allocator_init;
    allocator->mmb_alloc = __mmb_alloc;
    allocator->mmb_alloc_v2 = __mmb_alloc_v2;
    allocator->mmb_map2kern = __mmb_map2kern;
    allocator->mmb_unmap = __mmb_unmap;
    allocator->mmb_free = __mmb_free;
    allocator->mmf_map = __mmf_map;
    allocator->mmf_unmap = __mmf_unmap;

    return 0;
}
