/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2016-2019.
 * Description: media mem source file.
 * Author: Hisilicon multimedia software group
 * Create: 2016-11-11
 */

#include <generated/autoconf.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/version.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/spinlock.h>
#include <linux/vmalloc.h>
#include <asm/cacheflush.h>
#ifndef CONFIG_64BIT
#include <linux/highmem.h>
#include <asm/pgtable.h>
#endif
#include <linux/seq_file.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/time.h>
#include <linux/dma-mapping.h>
#include "mmz_proc.h"
#include "hi_osal.h"
#include "allocator.h"

#include "../../../compat/kernel_compat.h"

OSAL_LIST_HEAD(g_mmz_list);
OSAL_LIST_HEAD(g_map_mmz_list);

int anony = 0;
static compat_DEFINE_SEMAPHORE(g_mmz_lock);

module_param(anony, int, S_IRUGO);

#ifndef MODULE

char __initdata g_setup_zones[MMZ_SETUP_CMDLINE_LEN] = CONFIG_HISILICON_MMZ_DEFAULT;
int __init parse_kern_cmdline(char *line)
{
    compat_strlcpy(g_setup_zones, line, sizeof(g_setup_zones));

    return 1;
}
__setup("mmz=", parse_kern_cmdline);

static char __initdata g_setup_allocator[MMZ_ALLOCATOR_NAME_LEN];
static int __init parse_kern_allocator(char *line)
{
    compat_strlcpy(g_setup_allocator, line, sizeof(g_setup_allocator));
    return 1;
}
__setup("mmz_allocator=", parse_kern_allocator);

static char __initdata g_mmap_zones[MMZ_SETUP_CMDLINE_LEN] = '\0';
__setup("map_mmz=", parse_kern_cmdline);

#else
char g_setup_zones[MMZ_SETUP_CMDLINE_LEN] = {'\0'};
char g_mmap_zones[MMZ_SETUP_CMDLINE_LEN] = {'\0'};
char g_setup_allocator[MMZ_ALLOCATOR_NAME_LEN] = "hisi"; /* default setting */
module_param_string(mmz, g_setup_zones, MMZ_SETUP_CMDLINE_LEN, 0600);
module_param_string(map_mmz, g_mmap_zones, MMZ_SETUP_CMDLINE_LEN, 0600);
module_param_string(mmz_allocator, g_setup_allocator, MMZ_ALLOCATOR_NAME_LEN, 0600);
MODULE_PARM_DESC(mmz, "mmz_allocator=allocator mmz=name,0,start,size,type,eqsize:[others] map_mmz=start,size:[others]");
#endif

static struct mmz_allocator g_the_allocator;

hil_mmz_t *hil_mmz_create(const char *name,
                          unsigned long gfp,
                          unsigned long phys_start,
                          unsigned long nbytes)
{
    hil_mmz_t *p = NULL;

    mmz_trace_func();

    if (name == NULL) {
        printk(KERN_ERR "%s: 'name' should not be NULL!", __FUNCTION__);
        return NULL;
    }

    p = kmalloc(sizeof(hil_mmz_t) + 1, GFP_KERNEL);
    if (p == NULL) {
        printk(KERN_ERR "%s: System OOM!\n", __func__);
        return NULL;
    }

    memset(p, 0, sizeof(hil_mmz_t) + 1);
    compat_strlcpy(p->name, name, HIL_MMZ_NAME_LEN);
    p->gfp = gfp;
    p->phys_start = phys_start;
    p->nbytes = nbytes;

    OSAL_INIT_LIST_HEAD(&p->list);
    OSAL_INIT_LIST_HEAD(&p->mmb_list);

    p->destructor = kfree;

    return p;
}
EXPORT_SYMBOL(hil_mmz_create);

hil_mmz_t *hil_mmz_create_v2(const char *name,
                             unsigned long gfp,
                             unsigned long phys_start,
                             unsigned long nbytes,
                             unsigned int alloc_type,
                             unsigned long block_align)
{
    hil_mmz_t *p = NULL;

    mmz_trace_func();

    if (name == NULL) {
        printk(KERN_ERR "%s: 'name' can not be zero!", __FUNCTION__);
        return NULL;
    }

    p = kmalloc(sizeof(hil_mmz_t), GFP_KERNEL);
    if (p == NULL) {
        return NULL;
    }

    memset(p, 0, sizeof(hil_mmz_t));
    compat_strlcpy(p->name, name, HIL_MMZ_NAME_LEN);
    p->gfp = gfp;
    p->phys_start = phys_start;
    p->nbytes = nbytes;
    p->alloc_type = alloc_type;
    p->block_align = block_align;

    OSAL_INIT_LIST_HEAD(&p->list);
    OSAL_INIT_LIST_HEAD(&p->mmb_list);

    p->destructor = kfree;

    return p;
}

int hil_mmz_destroy(hil_mmz_t *zone)
{
    if (zone == NULL) {
        return -1;
    }

    if (zone->destructor) {
        zone->destructor(zone);
    }
    return 0;
}
EXPORT_SYMBOL(hil_mmz_destroy);

static int _check_mmz(hil_mmz_t *zone)
{
    hil_mmz_t *p = NULL;

    unsigned long new_start = zone->phys_start;
    unsigned long new_end = zone->phys_start + zone->nbytes;

    if (zone->nbytes == 0) {
        return -1;
    }

    if (!((new_start >= __pa((uintptr_t)high_memory)) ||
        ((new_start < PHYS_OFFSET) && (new_end <= PHYS_OFFSET)))) {
        printk(KERN_ERR "ERROR: Conflict MMZ:\n");
        printk(KERN_ERR HIL_MMZ_FMT_S "\n", hil_mmz_fmt_arg(zone));
        printk(KERN_ERR "MMZ conflict to kernel memory (0x%08lX, 0x%08lX)\n",
               (long unsigned int)PHYS_OFFSET,
               (long unsigned int)(__pa((uintptr_t)high_memory) - 1));
        return -1;
    }

    osal_list_for_each_entry(p, &g_mmz_list, list) {
        unsigned long start, end;
        start = p->phys_start;
        end   = p->phys_start + p->nbytes;
        if (new_start >= end) {
            continue;
        } else if (new_start < start && new_end <= start) {
            continue;
        } else {
        }

        printk(KERN_ERR "ERROR: Conflict MMZ:\n");
        printk(KERN_ERR "MMZ new:   " HIL_MMZ_FMT_S "\n", hil_mmz_fmt_arg(zone));
        printk(KERN_ERR "MMZ exist: " HIL_MMZ_FMT_S "\n", hil_mmz_fmt_arg(p));
        printk(KERN_ERR "Add new MMZ failed!\n");
        return -1;
    }

    return 0;
}

int hil_mmz_register(hil_mmz_t *zone)
{
    int ret;

    mmz_trace(MMZ_ERR_LEVEL, HIL_MMZ_FMT_S, hil_mmz_fmt_arg(zone));

    if (zone == NULL) {
        return -1;
    }

    down(&g_mmz_lock);

    if (strcmp(g_setup_allocator, "hisi") == 0) {
        ret = _check_mmz(zone);
        if (ret) {
            up(&g_mmz_lock);
            return ret;
        }
    }

    OSAL_INIT_LIST_HEAD(&zone->mmb_list);

    osal_list_add(&zone->list, &g_mmz_list);

    up(&g_mmz_lock);

    return 0;
}

int hil_mmz_unregister(hil_mmz_t *zone)
{
    int losts = 0;
    hil_mmb_t *p = NULL;

    if (zone == NULL) {
        return -1;
    }

    mmz_trace_func();

    down(&g_mmz_lock);
    osal_list_for_each_entry(p, &zone->mmb_list, list) {
        printk(KERN_WARNING "MB Lost: " HIL_MMB_FMT_S "\n",
               hil_mmb_fmt_arg(p));
        losts++;
    }

    if (losts != 0) {
        printk(KERN_ERR "%d mmbs not free, mmz<%s> can not be unregistered!\n",
               losts, zone->name);
        up(&g_mmz_lock);
        return -1;
    }

    osal_list_del(&zone->list);
    up(&g_mmz_lock);

    return 0;
}

hil_mmb_t *hil_mmb_alloc(const char *name,
                         unsigned long size,
                         unsigned long align,
                         unsigned long gfp,
                         const char *mmz_name)
{
    hil_mmb_t *mmb = NULL;

    down(&g_mmz_lock);
    mmb = g_the_allocator.mmb_alloc(name, size, align, gfp, mmz_name, NULL);
    up(&g_mmz_lock);

    return mmb;
}
EXPORT_SYMBOL(hil_mmb_alloc);

hil_mmb_t *hil_mmb_alloc_v2(const char *name,
                            unsigned long size,
                            unsigned long align,
                            unsigned long gfp,
                            const char *mmz_name,
                            unsigned int order)
{
    hil_mmb_t *mmb = NULL;

    down(&g_mmz_lock);
    mmb = g_the_allocator.mmb_alloc_v2(name, size, align,
                                       gfp, mmz_name, NULL, order);
    up(&g_mmz_lock);

    return mmb;
}
EXPORT_SYMBOL(hil_mmb_alloc_v2);

hil_mmb_t *hil_mmb_alloc_in(const char *name,
                            unsigned long size,
                            unsigned long align,
                            hil_mmz_t *_user_mmz)
{
    hil_mmb_t *mmb = NULL;

    if (_user_mmz == NULL) {
        return NULL;
    }

    down(&g_mmz_lock);
    mmb = g_the_allocator.mmb_alloc(name, size, align,
                                    _user_mmz->gfp, _user_mmz->name, _user_mmz);
    up(&g_mmz_lock);

    return mmb;
}

hil_mmb_t *hil_mmb_alloc_in_v2(const char *name,
                               unsigned long size,
                               unsigned long align,
                               hil_mmz_t *_user_mmz,
                               unsigned int order)
{
    hil_mmb_t *mmb = NULL;

    if (_user_mmz == NULL) {
        return NULL;
    }

    down(&g_mmz_lock);
    mmb = g_the_allocator.mmb_alloc_v2(name, size, align, _user_mmz->gfp,
                                       _user_mmz->name, _user_mmz, order);
    up(&g_mmz_lock);

    return mmb;
}

void *hil_mmb_map2kern(hil_mmb_t *mmb)
{
    void *p = NULL;

    if (mmb == NULL) {
        return NULL;
    }

    down(&g_mmz_lock);
    p = g_the_allocator.mmb_map2kern(mmb, 0);
    up(&g_mmz_lock);

    return p;
}
EXPORT_SYMBOL(hil_mmb_map2kern);

/* mmf: media-memory fragment */
void *hil_mmf_map2kern_nocache(unsigned long phys, int len)
{
    void *virt = g_the_allocator.mmf_map(phys, len, 0);
    if (virt != NULL) {
        return virt;
    }

    return NULL;
}
EXPORT_SYMBOL(hil_mmf_map2kern_nocache);

void *hil_mmf_map2kern_cache(unsigned long phys, int len)
{
    void *virt = g_the_allocator.mmf_map(phys, len, 1);
    if (virt != NULL) {
        return virt;
    }

    return NULL;
}
EXPORT_SYMBOL(hil_mmf_map2kern_cache);

void hil_mmf_unmap(void *virt)
{
    g_the_allocator.mmf_unmap(virt);
}
EXPORT_SYMBOL(hil_mmf_unmap);

void *hil_mmb_map2kern_cached(hil_mmb_t *mmb)
{
    void *p = NULL;

    if (mmb == NULL) {
        return NULL;
    }

    down(&g_mmz_lock);
    p = g_the_allocator.mmb_map2kern(mmb, 1);
    up(&g_mmz_lock);

    return p;
}
EXPORT_SYMBOL(hil_mmb_map2kern_cached);

int hil_mmb_flush_dcache_byaddr(void *kvirt,
                                unsigned long phys_addr,
                                unsigned long length)
{
    if (kvirt == NULL) {
        return -EINVAL;
    }
    /*
     * Use flush range to instead flush_cache_all,
     * because flush_cache_all only flush local cpu.
     * And on_each_cpu macro cannot used to flush
     * all cpus with irq disabled.
     */
#ifdef CONFIG_64BIT
    __flush_dcache_area(kvirt, length);
#else
    /*
     * dmac_map_area is invalid in  hi3518ev200 kernel,
     * arm9 is not supported yet
     */
#if (HICHIP==0x3516A100)
    /* flush without clean */
    dmac_map_area(kvirt, length, DMA_TO_DEVICE);
#else
    __cpuc_flush_dcache_area(kvirt, length);
#endif
#endif

#if defined(CONFIG_CACHE_HIL2V200) || defined(CONFIG_CACHE_L2X0)
    /* flush l2 cache, use paddr */
    /*
     * if length > L2 cache size, then this interface
     * will call <outer_flush_all>
     */
    outer_flush_range(phys_addr, phys_addr + length);
#endif

    return 0;
}
EXPORT_SYMBOL(hil_mmb_flush_dcache_byaddr);

int hil_mmb_invalid_cache_byaddr(void *kvirt,
                                 unsigned long phys_addr,
                                 unsigned long length)
{
    if (kvirt == NULL) {
        return -EINVAL;
    }

#ifdef CONFIG_64BIT
    __flush_dcache_area(kvirt, length);
#else
    /*
     * dmac_map_area is invalid in  hi3518ev200 kernel,
     * arm9 is not supported yet
     */
#if (HICHIP == 0x3516A100)
    /* flush without clean */
    dmac_map_area(kvirt, length, DMA_FROM_DEVICE);
#else
    __cpuc_flush_dcache_area(kvirt, length);
#endif
#endif
    return 0;
}
EXPORT_SYMBOL(hil_mmb_invalid_cache_byaddr);

int hil_mmb_unmap(hil_mmb_t *mmb)
{
    int ref;

    if (mmb == NULL) {
        return -1;
    }

    down(&g_mmz_lock);

    ref = g_the_allocator.mmb_unmap(mmb);

    up(&g_mmz_lock);

    return ref;
}
EXPORT_SYMBOL(hil_mmb_unmap);

int hil_mmb_get(hil_mmb_t *mmb)
{
    int ref;

    if (mmb == NULL) {
        return -1;
    }
    down(&g_mmz_lock);

    if (mmb->flags & HIL_MMB_RELEASED) {
        printk(KERN_WARNING "hil_mmb_get: amazing, mmb<%s> is released!\n", mmb->name);
    }
    ref = ++mmb->phy_ref;

    up(&g_mmz_lock);

    return ref;
}

int hil_mmb_put(hil_mmb_t *mmb)
{
    int ref;

    if (mmb == NULL) {
        return -1;
    }

    down(&g_mmz_lock);

    if (mmb->flags & HIL_MMB_RELEASED) {
        printk(KERN_WARNING "hil_mmb_put: amazing, mmb<%s> is released!\n", mmb->name);
    }

    ref = --mmb->phy_ref;

    if ((mmb->flags & HIL_MMB_RELEASED) && (mmb->phy_ref == 0) && (mmb->map_ref == 0)) {
        g_the_allocator.mmb_free(mmb);
    }

    up(&g_mmz_lock);

    return ref;
}

int hil_mmb_free(hil_mmb_t *mmb)
{
    mmz_trace_func();

    if (mmb == NULL) {
        return -1;
    }

    mmz_trace(MMZ_ERR_LEVEL, HIL_MMB_FMT_S, hil_mmb_fmt_arg(mmb));
    down(&g_mmz_lock);

    if (mmb->flags & HIL_MMB_RELEASED) {
        printk(KERN_WARNING "hil_mmb_free: amazing, mmb<%s> has been released, but is still in use!\n", mmb->name);
        up(&g_mmz_lock);
        return 0;
    }

    if (mmb->phy_ref > 0) {
        printk(KERN_WARNING "hil_mmb_free: free mmb<%s> delayed for which ref-count is %d!\n",
               mmb->name, mmb->map_ref);
        mmb->flags |= HIL_MMB_RELEASED;
        up(&g_mmz_lock);
        return 0;
    }

    if (mmb->flags & HIL_MMB_MAP2KERN) {
        printk(KERN_WARNING "hil_mmb_free: free mmb<%s> delayed for which is kernel-mapped to 0x%pK with map_ref %d!\n",
               mmb->name, mmb->kvirt, mmb->map_ref);
        mmb->flags |= HIL_MMB_RELEASED;
        up(&g_mmz_lock);
        return 0;
    }
    g_the_allocator.mmb_free(mmb);
    up(&g_mmz_lock);
    return 0;
}
EXPORT_SYMBOL(hil_mmb_free);

#define MACH_MMB(p, val, member) do { \
    hil_mmz_t *__mach_mmb_zone__ = NULL; \
    (p) = NULL;\
    osal_list_for_each_entry(__mach_mmb_zone__, &g_mmz_list, list) { \
        hil_mmb_t *__mach_mmb__ = NULL;\
        osal_list_for_each_entry(__mach_mmb__, &__mach_mmb_zone__->mmb_list, list) { \
            if (__mach_mmb__->member == (val)) { \
                (p) = __mach_mmb__; \
                break;\
            } \
        } \
        if ((p) != NULL) { \
            break;\
        }\
    } \
} while (0)

hil_mmb_t *hil_mmb_getby_phys(unsigned long addr)
{
    hil_mmb_t *p = NULL;

    down(&g_mmz_lock);
    MACH_MMB(p, addr, phys_addr);
    up(&g_mmz_lock);
    return p;
}
EXPORT_SYMBOL(hil_mmb_getby_phys);

unsigned long usr_virt_to_phys(unsigned long virt)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 5, 0)
    /* pte_offset_map internals changed in 6.5 (__pte_offset_map not exported).
     * This function is only used for debug/diagnostic. Stub it. */
    struct page *page;
    unsigned long phys;
    int ret;

    if (virt & 0x3)
        return 0;
    if (virt >= PAGE_OFFSET)
        return 0;

    ret = get_user_pages_fast(virt, 1, 0, &page);
    if (ret <= 0)
        return 0;
    phys = page_to_phys(page) | (virt & ~PAGE_MASK);
    put_page(page);
    return phys;
#else
    pgd_t *pgd = NULL;
    pud_t *pud = NULL;
    pmd_t *pmd = NULL;
    pte_t *pte = NULL;
    unsigned int cacheable = 0;
    unsigned long page_addr;
    unsigned long page_offset;
    unsigned long phys_addr;

    if (virt & 0x3) {
        printk("invalid virt addr 0x%08lx[not 4 bytes align]\n", virt);
        return 0;
    }

    if (virt >= PAGE_OFFSET) {
        printk("invalid user space virt addr 0x%08lx\n", virt);
        return 0;
    }

    pgd = pgd_offset(current->mm, virt);
    if (pgd_none(*pgd)) {
        printk("printk: not mapped in pgd!\n");
        return 0;
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
    {
        p4d_t *p4d;
        p4d = p4d_offset(pgd, virt);
        if (p4d_none(*p4d)) {
            printk("printk: not mapped in p4d!\n");
            return 0;
        }
        pud = pud_offset(p4d, virt);
    }
#else
    pud = pud_offset(pgd, virt);
#endif
    if (pud_none(*pud)) {
        printk("printk: not mapped in pud!\n");
        return 0;
    }

    pmd = pmd_offset(pud, virt);
    if (pmd_none(*pmd)) {
        printk("printk: not mapped in pmd!\n");
        return 0;
    }

    pte = pte_offset_map(pmd, virt);
    if (!pte || pte_none(*pte)) {
        printk("printk: not mapped in pte!\n");
        pte_unmap(pte);
        return 0;
    }

    page_addr = (pte_val(*pte) & PHYS_MASK) & PAGE_MASK;
    page_offset = virt & ~PAGE_MASK;
    phys_addr = page_addr | page_offset;
#ifdef CONFIG_64BIT
    if (pte_val(*pte) & (1 << 4)) { /* 4: cacheable flag of 64-bit linux is bit 4 */
#else
    if (pte_val(*pte) & (1 << 3)) { /* 3: cacheable flag of 32-bit linux is bit 3 */
#endif
        cacheable = 1;
    }

    /*
     * phys_addr: the lowest bit indicates its cache attribute
     * 1: cacheable
     * 0: uncacheable
     */
    phys_addr |= cacheable;

    pte_unmap(pte);

    return phys_addr;
#endif /* >= 6.5 */
}
EXPORT_SYMBOL(usr_virt_to_phys);

#define MACH_MMB_2(p, val, member, Outoffset) do {\
    hil_mmz_t *__mach_mmb_zone__ = NULL; \
    (p) = NULL;\
    osal_list_for_each_entry(__mach_mmb_zone__, &g_mmz_list, list) { \
        hil_mmb_t *__mach_mmb__ = NULL;\
        osal_list_for_each_entry(__mach_mmb__, &__mach_mmb_zone__->mmb_list, list) { \
            if ((__mach_mmb__->member <= (val)) && ((__mach_mmb__->length + __mach_mmb__->member) > (val))) { \
                (p) = __mach_mmb__; \
                (Outoffset) = (val) - __mach_mmb__->member;\
                break;\
            }\
        } \
        if ((p) != NULL) { \
            break;\
        } \
    } \
} while (0)

hil_mmb_t *hil_mmb_getby_kvirt(void *virt)
{
    hil_mmb_t *p = NULL;
    unsigned long out_offset;

    if (virt == NULL) {
        return NULL;
    }
    down(&g_mmz_lock);
    MACH_MMB_2(p, virt, kvirt, out_offset);
    up(&g_mmz_lock);

    mmz_trace(MMZ_ERR_LEVEL, "Outoffset %lu \n", out_offset);

    return p;
}
EXPORT_SYMBOL(hil_mmb_getby_kvirt);

hil_mmb_t *hil_mmb_getby_phys_2(unsigned long addr, unsigned long *out_offset)
{
    hil_mmb_t *p = NULL;

    down(&g_mmz_lock);
    MACH_MMB_2(p, addr, phys_addr, *out_offset);
    up(&g_mmz_lock);
    return p;
}
EXPORT_SYMBOL(hil_mmb_getby_phys_2);

hil_mmz_t *hil_mmz_find(unsigned long gfp, const char *mmz_name)
{
    hil_mmz_t *p = NULL;

    down(&g_mmz_lock);
    begin_list_for_each_mmz(p, gfp, mmz_name)
    up(&g_mmz_lock);
    return p;
    end_list_for_each_mmz()
    up(&g_mmz_lock);

    return NULL;
}
EXPORT_SYMBOL(hil_mmz_find);

unsigned long hil_mmz_get_phys(const char *zone_name)
{
    hil_mmz_t *zone = NULL;

    zone = hil_mmz_find(0, zone_name);
    if (zone != NULL) {
        return zone->phys_start;
    }

    return 0;
}
EXPORT_SYMBOL(hil_mmz_get_phys);

static unsigned long _strtoul_ex(const char *s, char **ep, unsigned int base)
{
    char *__end_p = NULL;
    unsigned long __value;

    __value = simple_strtoul(s, &__end_p, base);

    switch (*__end_p) {
        case 'm':
        case 'M':
            __value <<= 10; /* 10: 1M=1024k, left shift 10bit */
            /* fall-through */
        case 'k':
        case 'K':
            __value <<= 10; /* 10: 1K=1024Byte, left shift 10bit */
            if (ep != NULL) {
                (*ep) = __end_p + 1;
            }
            /* fall-through */
        default:
            break;
    }

    return __value;
}

static int _check_map_mmz(hil_mmz_t *zone)
{
    hil_mmz_t *p = NULL;

    unsigned long new_start = zone->phys_start;
    unsigned long new_end = zone->phys_start + zone->nbytes;

    if (zone->nbytes == 0) {
        return -1;
    }

    osal_list_for_each_entry(p, &g_map_mmz_list, list) {
        unsigned long start, end;
        start = p->phys_start;
        end   = p->phys_start + p->nbytes;

        if (new_start >= end) {
            continue;
        } else if ((new_start < start) && (new_end <= start)) {
            continue;
        } else {
        }

        printk(KERN_ERR "ERROR: Conflict MMZ:\n");
        printk(KERN_ERR "MMZ new:   " HIL_MMZ_FMT_S "\n", hil_mmz_fmt_arg(zone));
        printk(KERN_ERR "MMZ exist: " HIL_MMZ_FMT_S "\n", hil_mmz_fmt_arg(p));
        printk(KERN_ERR "Add new MMZ failed!\n");
        return -1;
    }

    return 0;
}

int hil_map_mmz_register(hil_mmz_t *zone)
{
    int ret;

    mmz_trace(MMZ_ERR_LEVEL, HIL_MMZ_FMT_S, hil_mmz_fmt_arg(zone));

    if (zone == NULL) {
        return -1;
    }

    down(&g_mmz_lock);

    if (strcmp(g_setup_allocator, "hisi") == 0) {
        ret = _check_mmz(zone);
        if (ret != 0) {
            up(&g_mmz_lock);
            return ret;
        }
    }

    ret = _check_map_mmz(zone);
    if (ret) {
        up(&g_mmz_lock);
        return ret;
    }

    OSAL_INIT_LIST_HEAD(&zone->mmb_list);

    osal_list_add(&zone->list, &g_map_mmz_list);

    up(&g_mmz_lock);

    return 0;
}

int hil_map_mmz_unregister(hil_mmz_t *zone)
{
    int losts = 0;
    hil_mmb_t *p = NULL;

    if (zone == NULL) {
        return -1;
    }

    mmz_trace_func();

    down(&g_mmz_lock);
    osal_list_for_each_entry(p, &zone->mmb_list, list) {
        printk(KERN_WARNING "MB Lost: " HIL_MMB_FMT_S "\n",
               hil_mmb_fmt_arg(p));
        losts++;
    }

    if (losts) {
        printk(KERN_ERR "%d mmbs not free, mmz<%s> can not be unregistered!\n",
               losts, zone->name);
        up(&g_mmz_lock);
        return -1;
    }

    osal_list_del(&zone->list);
    up(&g_mmz_lock);

    return 0;
}

static int map_mmz_init(char *s)
{
    hil_mmz_t *zone = NULL;
    char *line = NULL;

    if (s[0] == '\0') {
        return 0;
    }

    while ((line = strsep(&s, ":")) != NULL) {
        int i;
        char *argv[2]; /* 2: map mmz has two arguments */

        for (i = 0; (argv[i] = strsep(&line, ",")) != NULL;) {
            if (++i == ARRAY_SIZE(argv)) {
                break;
            }
        }

        if (i == 2) { /* 2: had parse two args */
            zone = hil_mmz_create("null", 0, 0, 0);
            if (zone == NULL) {
                continue;
            }
            zone->phys_start = _strtoul_ex(argv[0], NULL, 0);
            zone->nbytes = _strtoul_ex(argv[1], NULL, 0);
        } else {
            printk(KERN_ERR "error parameters\n");
            return -EINVAL;
        }

        if (hil_map_mmz_register(zone)) {
            printk(KERN_WARNING "Add MMZ failed: " HIL_MMZ_FMT_S "\n", hil_mmz_fmt_arg(zone));
            hil_mmz_destroy(zone);
        }
        zone = NULL;
    }

    return 0;
}

static void map_mmz_exit(void)
{
    hil_mmz_t *pmmz = NULL;
    struct osal_list_head* p = NULL, *n = NULL;

    mmz_trace_func();

    osal_list_for_each_safe(p, n, &g_map_mmz_list) {
        pmmz = osal_list_entry(p, hil_mmz_t, list);
        printk(KERN_WARNING "MMZ force removed: " HIL_MMZ_FMT_S "\n",
               hil_mmz_fmt_arg(pmmz));
        hil_map_mmz_unregister(pmmz);
        hil_mmz_destroy(pmmz);
    }
}

int hil_map_mmz_check_phys(unsigned long addr_start, unsigned long addr_len)
{
    hil_mmz_t *p = NULL;
    unsigned long addr_end = addr_start + addr_len;
    unsigned long temp_start, temp_end;

    if ((addr_len > 0) && (addr_end > addr_start)) {
        osal_list_for_each_entry(p, &g_map_mmz_list, list) {
            temp_start = p->phys_start;
            temp_end   = p->phys_start + p->nbytes;
            if ((addr_start >= temp_start) && (addr_end <= temp_end)) {
                return 0;
            }
        }
    }

    printk(KERN_ERR "ERROR: MMAP ADDR: 0x%lx-0x%lx\n", addr_start, addr_end);

    return -1;
}
EXPORT_SYMBOL(hil_map_mmz_check_phys);

int hil_vma_check(unsigned long vm_start, unsigned long vm_end)
{
    struct vm_area_struct *pvma1 = NULL;
    struct vm_area_struct *pvma2 = NULL;

    pvma1 = find_vma(current->mm, vm_start);
    if (pvma1 == NULL) {
        printk(KERN_ERR "ERROR: pvma1 is null\n");
        return -1;
    }

    pvma2 = find_vma(current->mm, vm_end - 1);
    if (pvma2 == NULL) {
        printk(KERN_ERR "ERROR: pvma2 is null\n");
        return -1;
    }

    if (pvma1 != pvma2) {
        printk(KERN_ERR "ERROR: pvma1:[0x%lx,0x%lx) and pvma2:[0x%lx,0x%lx) are not equal\n",
            pvma1->vm_start, pvma1->vm_end, pvma2->vm_start, pvma2->vm_end);
        return -1;
    }

    if (!(pvma1->vm_flags & VM_WRITE)) {
        printk(KERN_ERR "ERROR vma flag:0x%lx\n", pvma1->vm_flags);
        return -1;
    }

    if (pvma1->vm_start > vm_start) {
        printk("cannot find corresponding vma, vm[%lx, %lx], user range[%lx,%lx]\n",
            pvma1->vm_start, pvma1->vm_end, vm_start, vm_end);
        return -1;
    }

    return 0;
}
EXPORT_SYMBOL(hil_vma_check);

int hil_is_phys_in_mmz(unsigned long addr_start, unsigned long addr_len)
{
    hil_mmz_t *p = NULL;
    unsigned long addr_end = addr_start + addr_len;
    unsigned long temp_start, temp_end;

    if ((addr_len > 0) && (addr_end > addr_start)) {
        osal_list_for_each_entry(p, &g_mmz_list, list) {
            temp_start = p->phys_start;
            temp_end   = p->phys_start + p->nbytes;
            if ((addr_start >= temp_start) && (addr_end <= temp_end)) {
                return 0;
            }
        }
    }

    return -1;
}
EXPORT_SYMBOL(hil_is_phys_in_mmz);

int hil_mmb_flush_dcache_byaddr_safe(void *kvirt,
                                     unsigned long phys_addr,
                                     unsigned long length)
{
    int ret;
    struct mm_struct *mm = current->mm;

    if (kvirt == NULL) {
        return -EINVAL;
    }

    compat_mmap_read_lock(mm);

    if (hil_vma_check((unsigned long)(uintptr_t)kvirt, (unsigned long)(uintptr_t)kvirt + length) != 0) {
        compat_mmap_read_unlock(mm);
        return -EPERM;
    }

    ret = hil_mmb_flush_dcache_byaddr(kvirt, phys_addr, length);

    compat_mmap_read_unlock(mm);

    return ret;
}
EXPORT_SYMBOL(hil_mmb_flush_dcache_byaddr_safe);

#define MEDIA_MEM_NAME  "media-mem"

#ifdef CONFIG_PROC_FS

int mmz_read_proc(struct seq_file *sfile)
{
    hil_mmz_t *p = NULL;
    const int len = 0;
    unsigned int zone_number = 0;
    unsigned int block_number = 0;
    unsigned int used_size = 0;
    unsigned int free_size;
    unsigned int mmz_total_size = 0;

    mmz_trace_func();

    down(&g_mmz_lock);
    osal_list_for_each_entry(p, &g_mmz_list, list) {
        hil_mmb_t *mmb = NULL;
        seq_printf(sfile, "+---ZONE: " HIL_MMZ_FMT_S "\n", hil_mmz_fmt_arg(p));
        mmz_total_size += p->nbytes / 1024; /* 1024: 1KByte = 1024Byte */
        ++zone_number;

        osal_list_for_each_entry(mmb, &p->mmb_list, list) {
            seq_printf(sfile, "   |-MMB: " HIL_MMB_FMT_S "\n", hil_mmb_fmt_arg(mmb));
            used_size += mmb->length / 1024; /* 1024: 1KByte = 1024Byte */
            ++block_number;
        }
    }

    if (mmz_total_size != 0) {
        free_size = mmz_total_size - used_size;
        seq_printf(sfile, "\n---MMZ_USE_INFO:\n total size=%uKB(%uMB),"
                   "used=%uKB(%uMB + %uKB),remain=%uKB(%uMB + %uKB),"
                   "zone_number=%u,block_number=%u\n",
                   mmz_total_size, mmz_total_size / 1024, /* 1024: 1MByte = 1024KByte */
                   used_size, used_size / 1024, used_size % 1024, /* 1024: 1MByte = 1024KByte */
                   free_size, free_size / 1024, free_size % 1024, /* 1024: 1MByte = 1024KByte */
                   zone_number, block_number);

        mmz_total_size = 0;
        zone_number = 0;
        block_number = 0;
    }
    up(&g_mmz_lock);

    return len;
}

#define MMZ_PROC_ROOT  NULL

static const struct seq_operations g_mmz_seq_ops = {
    .start = mmz_seq_start,
    .next = mmz_seq_next,
    .stop = mmz_seq_stop,
    .show = mmz_seq_show
};

static int mmz_proc_open(struct inode *inode, struct file *file)
{
    return seq_open(file, &g_mmz_seq_ops);
}

#ifdef COMPAT_USE_PROC_OPS
static struct proc_ops g_mmz_proc_ops = {
    .proc_open = mmz_proc_open,
    .proc_read = seq_read,
    .proc_release = seq_release,
};
#else
static struct file_operations g_mmz_proc_ops = {
    .owner = THIS_MODULE,
    .open = mmz_proc_open,
    .read = seq_read,
    .release = seq_release,
};
#endif

static int __init media_mem_proc_init(void)
{
    struct proc_dir_entry *p = NULL;

    p = proc_create(MEDIA_MEM_NAME, 0, MMZ_PROC_ROOT, &g_mmz_proc_ops);
    if (p == NULL) {
        printk(KERN_ERR "Create mmz proc fail!\n");
        return -1;
    }
    return 0;
}

static void __exit media_mem_proc_exit(void)
{
    remove_proc_entry(MEDIA_MEM_NAME, MMZ_PROC_ROOT);
}

#else
static int __init media_mem_proc_init(void) { return 0; }
static void __exit media_mem_proc_exit(void) { }

#endif /* CONFIG_PROC_FS */

/* this function is used by osal.c */
int mem_check_module_param(void)
{
    if (anony != 1) {
        printk("The module param \"anony\" should only be 1 which is %d \n", anony);
        return -1;
    }
    return 0;
}

static void __exit mmz_exit_check(void)
{
    hil_mmz_t* pmmz = NULL;
    struct osal_list_head *p = NULL;
    struct osal_list_head *n = NULL;

    mmz_trace_func();

    osal_list_for_each_safe(p, n, &g_mmz_list) {
        pmmz = osal_list_entry(p, hil_mmz_t, list);
        printk(KERN_WARNING "MMZ force removed: " HIL_MMZ_FMT_S "\n",
               hil_mmz_fmt_arg(pmmz));
        hil_mmz_unregister(pmmz);
        hil_mmz_destroy(pmmz);
    }
}

int __init media_mem_init(void)
{
    int ret = 0;

    printk(KERN_INFO "Hisilicon Media Memory Zone Manager\n");

    if (anony != 1) {
        printk("The module param \"anony\" should only be 1 which is %d\n", anony);
        return -EINVAL;
    }

    if (strcmp(g_setup_allocator, "cma") == 0) {
#ifdef CONFIG_CMA
        ret = cma_allocator_setopt(&g_the_allocator);
#else
        pr_err("cma is not enabled in kernel, please check!\n");
        return -EINVAL;
#endif
    } else {
        printk("The module param \"setup_allocator\" should be \"cma\" or \"hisi\", which is \"%s\"\n",
                g_setup_allocator);
        mmz_exit_check();
        return -EINVAL;
    }

    ret = g_the_allocator.init(g_setup_zones);
    if (ret != 0) {
        mmz_exit_check();
        return ret;
    }

    media_mem_proc_init();

    mmz_userdev_init();

    map_mmz_init(g_mmap_zones);

    return 0;
}

#ifdef MODULE
void __exit media_mem_exit(void)
{
    map_mmz_exit();

    mmz_userdev_exit();

    mmz_exit_check();

    media_mem_proc_exit();
}
#else
subsys_initcall(media_mem_init);
#endif
