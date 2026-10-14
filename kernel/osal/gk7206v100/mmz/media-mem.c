/*
 * Copyright (c) XMEDIA. All rights reserved.
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

#include "osal.h"
#include "allocator.h"
#include "version.h"
#include <linux/lotus/kapi.h>


OSAL_LIST_HEAD(mmz_list);

static DEFINE_SEMAPHORE(mmz_lock);

#define MMZ_SETUP_CMDLINE_LEN     256
#define MMZ_ALLOCATOR_NAME_LEN    32
#define MMZ_DEFAULT_ALLOCATOR     "xmedia"
#ifndef MODULE

static char setup_zones[MMZ_SETUP_CMDLINE_LEN] = {'\0'};
static int __init parse_kern_mmz(char *line)
{
    strlcpy(setup_zones, line, sizeof(setup_zones));

    return 1;
}
__setup("mmz=", parse_kern_mmz);

static char setup_allocator[MMZ_ALLOCATOR_NAME_LEN] = MMZ_DEFAULT_ALLOCATOR;
static int __init parse_kern_allocator(char *line)
{
    strlcpy(setup_allocator, line, sizeof(setup_allocator));
    return 1;
}
__setup("mmz_allocator=", parse_kern_allocator);

#else
static char setup_zones[MMZ_SETUP_CMDLINE_LEN]={'\0'};
static char setup_allocator[MMZ_ALLOCATOR_NAME_LEN] = MMZ_DEFAULT_ALLOCATOR; //default setting
module_param_string(mmz, setup_zones, MMZ_SETUP_CMDLINE_LEN, 0600);
module_param_string(mmz_allocator, setup_allocator, MMZ_ALLOCATOR_NAME_LEN, 0600);
MODULE_PARM_DESC(mmz,"mmz_allocator=allocator mmz=name,0,start,size,type,eqsize:[others] map_mmz=start,size:[others]");
#endif

static struct mmz_allocator the_allocator;

mmz_mmz_t *mmz_mmz_create(const char *name,
        unsigned long gfp,
        unsigned long phys_start,
        unsigned long nbytes)
{
    mmz_mmz_t *p = NULL;

    mmz_trace_func();

    if(name == NULL) {
        printk(KERN_ERR "%s: 'name' should not be NULL!", __FUNCTION__);
        return NULL;
    }

    p = kmalloc(sizeof(mmz_mmz_t) + 1, GFP_KERNEL);
    if (p == NULL) {
        printk(KERN_ERR "%s: System OOM!\n", __func__);
        return NULL;
    }

    memset(p, 0, sizeof(mmz_mmz_t)+1);
    strlcpy(p->name, name, MMZ_MMZ_NAME_LEN);
    p->gfp = gfp;
    p->phys_start = phys_start;
    p->nbytes = nbytes;

    OSAL_INIT_LIST_HEAD(&p->list);
    OSAL_INIT_LIST_HEAD(&p->mmb_list);

    p->destructor = kfree;

    return p;
}
EXPORT_SYMBOL(mmz_mmz_create);

int mmz_mmz_destroy(mmz_mmz_t *zone)
{
    if(zone == NULL) {
        return -1;
    }

    if(zone->destructor) {
        zone->destructor(zone);
    }
    return 0;
}
EXPORT_SYMBOL(mmz_mmz_destroy);

static int _check_mmz(mmz_mmz_t *zone)
{
    mmz_mmz_t *p = NULL;

    unsigned long new_start=zone->phys_start;
    unsigned long new_end=zone->phys_start+zone->nbytes;

    if (zone->nbytes == 0) {
        return -1;
    }

    if (!((new_start >= __pa((uintptr_t)high_memory)) ||
            ((new_start < PHYS_OFFSET) && (new_end <= PHYS_OFFSET)))) {
        printk(KERN_ERR "ERROR: Conflict MMZ:\n");
        printk(KERN_ERR MMZ_MMZ_FMT_S "\n", mmz_mmz_fmt_arg(zone));
        printk(KERN_ERR "MMZ conflict to kernel memory (0x%08lX, 0x%08lX)\n",
                (long unsigned int)PHYS_OFFSET,
                (long unsigned int)(__pa((uintptr_t)high_memory) - 1));
        return -1;
    }

    osal_list_for_each_entry(p, &mmz_list, list) {
        unsigned long start, end;
        start = p->phys_start;
        end   = p->phys_start + p->nbytes;

        if(new_start >= end) {
            continue;
        } else if (new_start < start && new_end <= start) {
            continue;
        } else {
            ;
        }

        printk(KERN_ERR "ERROR: Conflict MMZ:\n");
        printk(KERN_ERR "MMZ new:   " MMZ_MMZ_FMT_S "\n", mmz_mmz_fmt_arg(zone));
        printk(KERN_ERR "MMZ exist: " MMZ_MMZ_FMT_S "\n", mmz_mmz_fmt_arg(p));
        printk(KERN_ERR "Add new MMZ failed!\n");
        return -1;
    }

    return 0;
}

int mmz_mmz_register(mmz_mmz_t *zone)
{
    int ret = 0;

    mmz_trace(1, MMZ_MMZ_FMT_S, mmz_mmz_fmt_arg(zone));

    if(zone == NULL) {
        return -1;
    }

    down(&mmz_lock);

    if (0 == strcmp(setup_allocator, "xmedia")) {
        ret = _check_mmz(zone);
        if (ret) {
            up(&mmz_lock);
            return ret;
        }
    }

    OSAL_INIT_LIST_HEAD(&zone->mmb_list);

    osal_list_add(&zone->list, &mmz_list);

    up(&mmz_lock);

    return 0;
}

int mmz_mmz_unregister(mmz_mmz_t *zone)
{
    int losts = 0;
    mmz_mmb_t *p = NULL;

    if(zone == NULL)
        return -1;

    mmz_trace_func();

    down(&mmz_lock);
    osal_list_for_each_entry(p, &zone->mmb_list, list) {
        printk(KERN_WARNING "MB Lost: " MMZ_MMB_FMT_S "\n",
                mmz_mmb_fmt_arg(p));
        losts++;
    }

    if (losts) {
        printk(KERN_ERR "%d mmbs not free, mmz<%s> can not be unregistered!\n",
                losts, zone->name);
        up(&mmz_lock);
        return -1;
    }

    osal_list_del(&zone->list);
    up(&mmz_lock);

    return 0;
}

mmz_mmb_t *mmz_mmb_alloc(const char *name,
            unsigned long size,
            unsigned long align,
            unsigned long gfp,
            const char *mmz_name)
{
    mmz_mmb_t *mmb = NULL;

    down(&mmz_lock);
    mmb = the_allocator.mmb_alloc(name, size, align, gfp, mmz_name, NULL);
    up(&mmz_lock);

    return mmb;
}
EXPORT_SYMBOL(mmz_mmb_alloc);

void *mmz_mmb_map2kern(mmz_mmb_t *mmb)
{
    void *p = NULL;

    if(mmb == NULL)
        return NULL;

    down(&mmz_lock);
    p = the_allocator.mmb_map2kern(mmb, 0);
    up(&mmz_lock);

    return p;
}
EXPORT_SYMBOL(mmz_mmb_map2kern);

/* mmf: media-memory fragment */
void *mmz_mmf_map2kern_nocache(unsigned long phys, int len)
{
    void *virt = the_allocator.mmf_map(phys, len, 0);
    if (virt != NULL)
        return virt;

    return NULL;
}
EXPORT_SYMBOL(mmz_mmf_map2kern_nocache);

void *mmz_mmf_map2kern_cache(unsigned long phys, int len)
{
    void *virt = the_allocator.mmf_map(phys, len, 1);
    if (virt != NULL)
        return virt;

    return NULL;
}
EXPORT_SYMBOL(mmz_mmf_map2kern_cache);

void mmz_mmf_unmap(void *virt)
{
    the_allocator.mmf_unmap(virt);
}
EXPORT_SYMBOL(mmz_mmf_unmap);

void *mmz_mmb_map2kern_cached(mmz_mmb_t *mmb)
{
    void *p = NULL;

    if(mmb == NULL)
        return NULL;

    down(&mmz_lock);
    p = the_allocator.mmb_map2kern(mmb, 1);
    up(&mmz_lock);

    return p;
}
EXPORT_SYMBOL(mmz_mmb_map2kern_cached);

int mmz_mmb_flush_dcache_byaddr(void *kvirt,
        unsigned long phys_addr,
        unsigned long length)
{
    if (kvirt == NULL)
        return -EINVAL;

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
     * dmac_map_area is invalid in   kernel,
     * arm9 is not supported yet
     */
    __cpuc_flush_dcache_area(kvirt, length);
#endif
#if defined(CONFIG_CACHE_L2V200) || defined(CONFIG_CACHE_L2X0)
    /* flush l2 cache, use paddr */
    /*
     * if length > L2 cache size, then this interface
     * will call <outer_flush_all>
     */
    outer_flush_range(phys_addr, phys_addr + length);
#endif

    return 0;
}
EXPORT_SYMBOL(mmz_mmb_flush_dcache_byaddr);

int mmz_mmb_invalid_cache_byaddr(void *kvirt,
        unsigned long phys_addr,
        unsigned long length)
{
    if (kvirt == NULL)
        return -EINVAL;

#ifdef CONFIG_64BIT
    __flush_dcache_area(kvirt, length);
#else
    /*
     * dmac_map_area is invalid in   kernel,
     * arm9 is not supported yet
     */
    __cpuc_flush_dcache_area(kvirt, length);
#endif
    return 0;
}
EXPORT_SYMBOL(mmz_mmb_invalid_cache_byaddr);

int mmz_mmb_unmap(mmz_mmb_t *mmb)
{
    int ref;

    if(mmb == NULL)
        return -1;

    down(&mmz_lock);

    ref = the_allocator.mmb_unmap(mmb);

    up(&mmz_lock);

    return ref;
}
EXPORT_SYMBOL(mmz_mmb_unmap);

int mmz_mmb_get(mmz_mmb_t *mmb)
{
    int ref;

    if(mmb == NULL)
        return -1;

    down(&mmz_lock);

    if(mmb->flags & MMZ_MMB_RELEASED)
        printk(KERN_WARNING "mmz_mmb_get: amazing, mmb<%s> is released!\n", mmb->name);
    ref = ++mmb->phy_ref;

    up(&mmz_lock);

    return ref;
}

int mmz_mmb_put(mmz_mmb_t *mmb)
{
    int ref;

    if(mmb == NULL)
        return -1;

    down(&mmz_lock);

    if(mmb->flags & MMZ_MMB_RELEASED)
        printk(KERN_WARNING "mmz_mmb_put: amazing, mmb<%s> is released!\n", mmb->name);

    ref = --mmb->phy_ref;

    if ((mmb->flags & MMZ_MMB_RELEASED) && (mmb->phy_ref ==0) && (mmb->map_ref ==0)) {
        the_allocator.mmb_free(mmb);
    }

    up(&mmz_lock);

    return ref;
}

int mmz_mmb_free(mmz_mmb_t *mmb)
{
    mmz_trace_func();

    if(mmb == NULL)
        return -1;

    mmz_trace(1,MMZ_MMB_FMT_S,mmz_mmb_fmt_arg(mmb));
    down(&mmz_lock);

    if(mmb->flags & MMZ_MMB_RELEASED) {
        printk(KERN_WARNING "mmz_mmb_free: amazing, mmb<%s> has been released,\
                but is still in use!\n", mmb->name);
        up(&mmz_lock);
        return 0;
    }

    if (mmb->phy_ref > 0) {
        printk(KERN_WARNING "mmz_mmb_free: free mmb<%s> delayed \
                for which ref-count is %d!\n",
                mmb->name, mmb->map_ref);
        mmb->flags |= MMZ_MMB_RELEASED;
        up(&mmz_lock);
        return 0;
    }

    if (mmb->flags & MMZ_MMB_MAP2KERN) {
        printk(KERN_WARNING "mmz_mmb_free: free mmb<%s> delayed for which \
                       is kernel-mapped to 0x%pK with map_ref %d!\n",
                mmb->name, mmb->kvirt, mmb->map_ref);
        mmb->flags |= MMZ_MMB_RELEASED;
        up(&mmz_lock);
        return 0;
    }
    the_allocator.mmb_free(mmb);
    up(&mmz_lock);
    return 0;
}
EXPORT_SYMBOL(mmz_mmb_free);

#define MACH_MMB(p, val, member) do {\
    mmz_mmz_t *__mach_mmb_zone__ = NULL; \
    (p) = NULL;\
    list_for_each_entry(__mach_mmb_zone__, &mmz_list, list) { \
        mmz_mmb_t *__mach_mmb__ = NULL;\
        list_for_each_entry(__mach_mmb__, &__mach_mmb_zone__->mmb_list, list) { \
            if (__mach_mmb__->member == (val)) { \
                (p) = __mach_mmb__; \
                break;\
            } \
        } \
        if (p != NULL)break;\
    } \
}while(0)

mmz_mmb_t *mmz_mmb_getby_phys(unsigned long addr)
{
    mmz_mmb_t *p = NULL;
    down(&mmz_lock);
    MACH_MMB(p, addr, phys_addr);
    up(&mmz_lock);
    return p;
}
EXPORT_SYMBOL(mmz_mmb_getby_phys);

unsigned long usr_virt_to_phys(unsigned long virt)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
    pgd_t* pgd = NULL;
    pud_t* pud = NULL;
#endif
    pmd_t* pmd = NULL;
    pte_t* pte = NULL;
    unsigned int cacheable = 0;
    unsigned long page_addr = 0;
    unsigned long page_offset = 0;
    unsigned long phys_addr = 0;

    if (virt & 0x3) {
        printk("invalid virt addr 0x%08lx[not 4 bytes align]\n", virt);
        return 0;
    }

    if (virt >= PAGE_OFFSET) {
        printk("invalid user space virt addr 0x%08lx\n", virt);
        return 0;
    }
// in kernel 5.10 just use pmd_off is ok
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
    pgd = pgd_offset(current->mm, virt);

    if (pgd_none(*pgd)) {
        printk("printk: not mapped in pgd!\n");
        return 0;
    }

    pud = pud_offset(pgd, virt);

    if (pud_none(*pud)) {
        printk("printk: not mapped in pud!\n");
        return 0;
    }

    pmd = pmd_offset(pud, virt);
#else
    pmd = pmd_off(current->mm, virt);
#endif
    if (pmd_none(*pmd)) {
        printk("printk: not mapped in pmd!\n");
        return 0;
    }

    pte = pte_offset_map(pmd, virt);

    if (pte_none(*pte)) {
        printk("printk: not mapped in pte!\n");
        pte_unmap(pte);
        return 0;
    }

    page_addr = (pte_val(*pte) & PHYS_MASK) & PAGE_MASK;
    page_offset = virt & ~PAGE_MASK;
    phys_addr = page_addr | page_offset;
#ifdef CONFIG_64BIT
    if (pte_val(*pte) & (1 << 4))
#else
    if (pte_val(*pte) & (1 << 3))
#endif
    { cacheable = 1; }

    /*
     * phys_addr: the lowest bit indicates its cache attribute
     * 1: cacheable
     * 0: uncacheable
     */
    phys_addr |= cacheable;

    pte_unmap(pte);

    return phys_addr;
}
EXPORT_SYMBOL(usr_virt_to_phys);

#define MACH_MMB_2(p, val, member, Outoffset) do {\
    mmz_mmz_t *__mach_mmb_zone__ = NULL; \
    (p) = NULL;\
    list_for_each_entry(__mach_mmb_zone__, &mmz_list, list) { \
        mmz_mmb_t *__mach_mmb__ = NULL;\
        list_for_each_entry(__mach_mmb__, &__mach_mmb_zone__->mmb_list, list) { \
            if ((__mach_mmb__->member <= (val)) && ((__mach_mmb__->length + __mach_mmb__->member) > (val))) { \
                (p) = __mach_mmb__; \
                Outoffset = val - __mach_mmb__->member;\
                break;\
            }\
        } \
        if (p != NULL)break;\
    } \
}while(0)


mmz_mmb_t *mmz_mmb_getby_kvirt(void *virt)
{
    mmz_mmb_t *p = NULL;
    unsigned long Outoffset;

    if (virt == NULL)
        return NULL;

    down(&mmz_lock);
    MACH_MMB_2(p, virt, kvirt, Outoffset);
    up(&mmz_lock);

    mmz_trace(1, "Outoffset %lu \n", Outoffset);

    return p;
}
EXPORT_SYMBOL(mmz_mmb_getby_kvirt);

mmz_mmb_t *mmz_mmb_getby_phys_2(unsigned long addr, unsigned long *Outoffset)
{
    mmz_mmb_t *p = NULL;

    down(&mmz_lock);
    MACH_MMB_2(p, addr, phys_addr, *Outoffset);
    up(&mmz_lock);
    return p;
}
EXPORT_SYMBOL(mmz_mmb_getby_phys_2);

mmz_mmz_t *mmz_mmz_find(unsigned long gfp, const char *mmz_name)
{
    mmz_mmz_t *p = NULL;

    down(&mmz_lock);
    begin_list_for_each_mmz(p, gfp, mmz_name)
    up(&mmz_lock);
    return p;
    end_list_for_each_mmz()
    up(&mmz_lock);

    return NULL;
}
EXPORT_SYMBOL(mmz_mmz_find);

unsigned long mmz_mmz_get_phys(const char *zone_name)
{
    mmz_mmz_t *zone = NULL;

    zone = mmz_mmz_find(0, zone_name);
    if (zone != NULL)
        return zone->phys_start;

    return 0;
}
EXPORT_SYMBOL(mmz_mmz_get_phys);

int mmz_map_mmz_unregister(mmz_mmz_t *zone)
{
    int losts = 0;
    mmz_mmb_t *p = NULL;

    if (zone == NULL)
        return -1;

    mmz_trace_func();

    down(&mmz_lock);
    osal_list_for_each_entry(p, &zone->mmb_list, list) {
        printk(KERN_WARNING "MB Lost: " MMZ_MMB_FMT_S "\n",
                mmz_mmb_fmt_arg(p));
        losts++;
    }

    if (losts) {
        printk(KERN_ERR "%d mmbs not free, mmz<%s> can not be unregistered!\n",
                losts, zone->name);
        up(&mmz_lock);
        return -1;
    }

    osal_list_del(&zone->list);
    up(&mmz_lock);

    return 0;
}

int mmz_vma_check(unsigned long vm_start, unsigned long vm_end)
{
    struct vm_area_struct *pvma1;
    struct vm_area_struct *pvma2;

    pvma1 = find_vma(current->mm, vm_start);
    if (pvma1 == NULL) {
        printk(KERN_ERR "ERROR: pvma1 is null\n");
        return -1;
    }

    pvma2 = find_vma(current->mm, vm_end-1);
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
        printk("cannot find corresponding vma, vm[%lx, %lx], user range[%lx,%lx]\n", pvma1->vm_start, pvma1->vm_end, vm_start, vm_end);
        return -1;
    }

    return 0;
}
EXPORT_SYMBOL(mmz_vma_check);

int mmz_is_phys_in_mmz(unsigned long addr_start, unsigned long addr_len)
{
    mmz_mmz_t *p = NULL;
    unsigned long addr_end = addr_start + addr_len;
    unsigned long temp_start, temp_end;

    osal_list_for_each_entry(p, &mmz_list, list) {
        temp_start = p->phys_start;
        temp_end   = p->phys_start + p->nbytes;
        if ((addr_start >= temp_start) && (addr_end <= temp_end)) {
            return 0;
        }
    }

    return -1;
}
EXPORT_SYMBOL(mmz_is_phys_in_mmz);


int mmz_mmb_flush_dcache_byaddr_safe(void *kvirt,
        unsigned long phys_addr,
        unsigned long length)
{
    int ret = 0;
    struct mm_struct *mm = current->mm;

    if (kvirt == NULL)
        return -EINVAL;

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
    down_read(&mm->mmap_sem);
#else
    down_read(&mm->mmap_lock);
#endif
    if (mmz_vma_check((unsigned long)(uintptr_t)kvirt, (unsigned long)(uintptr_t)kvirt+length)) {
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
        up_read(&mm->mmap_sem);
#else
        up_read(&mm->mmap_lock);
#endif
        return -EPERM;
    }

    ret = mmz_mmb_flush_dcache_byaddr(kvirt, phys_addr, length);

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
    up_read(&mm->mmap_sem);
#else
    up_read(&mm->mmap_lock);
#endif

    return ret;
}
EXPORT_SYMBOL(mmz_mmb_flush_dcache_byaddr_safe);

#define MEDIA_MEM_NAME  "media-mem"

#ifdef CONFIG_PROC_FS
static void mmz_proc_version(struct osal_proc_dir_entry *sfile)
{
    unsigned char mmz_version[128] = {0};
    VERSION_FORMAT(mmz_version, "MMZ", "V1.0", "");
    osal_seq_printf(sfile, "\n%s\n\n", mmz_version);

    return;
}

int mmz_seq_show(struct osal_proc_dir_entry *sfile)
{
    mmz_mmz_t *p = NULL;
    int len = 0;
    unsigned int zone_number = 0;
    unsigned int block_number = 0;
    unsigned int used_size = 0;
    unsigned int free_size = 0;
    unsigned int mmz_total_size = 0;

    mmz_trace_func();

    mmz_proc_version(sfile);

    down(&mmz_lock);
    list_for_each_entry(p, &mmz_list, list) {
        mmz_mmb_t *mmb = NULL;
        osal_seq_printf(sfile, "+---ZONE: " MMZ_MMZ_FMT_S "\n", mmz_mmz_fmt_arg(p));
        mmz_total_size += p->nbytes / 1024;
        ++zone_number;

        list_for_each_entry(mmb, &p->mmb_list, list) {
            osal_seq_printf(sfile, "   |-MMB: " MMZ_MMB_FMT_S "\n", mmz_mmb_fmt_arg(mmb));
            used_size += mmb->length / 1024;
            ++block_number;
        }
    }

    if (mmz_total_size != 0) {
        free_size = mmz_total_size - used_size;
        osal_seq_printf(sfile, "\n---MMZ_USE_INFO:\n total size=%dKB(%dMB),"
                "used=%dKB(%dMB + %dKB),free=%dKB(%dMB + %dKB),"
                "zone_number=%d,block_number=%d\n",
                mmz_total_size, mmz_total_size / 1024,
                used_size, used_size / 1024, used_size % 1024,
                free_size, free_size / 1024, free_size % 1024,
                zone_number, block_number);
    }
    up(&mmz_lock);

    return len;
}

static int __init media_mem_proc_init(void)
{
    osal_proc_entry_t *proc = NULL;

    proc = osal_create_proc_entry(MEDIA_MEM_NAME, NULL);
    if (proc == NULL) {
        printk(KERN_ERR "Create mmz proc fail!\n");
        return -1;
    }

    proc->read = mmz_seq_show;
    return 0;
}

static void __exit media_mem_proc_exit(void)
{
    osal_remove_proc_entry(MEDIA_MEM_NAME, NULL);
}

#else
static int __init media_mem_proc_init(void) { return 0; }
static void __exit media_mem_proc_exit(void) { }

#endif /* CONFIG_PROC_FS */

static void mmz_exit_check(void)
{
    mmz_mmz_t* pmmz = NULL;
    struct osal_list_head* p = NULL, *n = NULL;

    mmz_trace_func();

    list_for_each_safe(p, n, &mmz_list) {
        pmmz = list_entry(p,mmz_mmz_t,list);
        printk(KERN_WARNING "MMZ force removed: " MMZ_MMZ_FMT_S "\n",
                mmz_mmz_fmt_arg(pmmz));
        mmz_mmz_unregister(pmmz);
        mmz_mmz_destroy(pmmz);
    }
}

int __init media_mem_init(void)
{
    int ret = 0;

    printk(KERN_INFO "Media Memory Zone Manager\n");

    if (setup_zones[0] == '\0') {
        char *boot_mmz_para_start = NULL;
        char *boot_mmz_para_end = NULL;
        int mmz_para_len = 0;

        /* get parameter from bootargs*/
        boot_mmz_para_start = lotus_get_cmd_line();

        if (boot_mmz_para_start != NULL)
            boot_mmz_para_start = strstr(boot_mmz_para_start, "mmz_allocator=");

        printk("mmz allocator from bootargs:%s\n", boot_mmz_para_start == NULL ? "none" : boot_mmz_para_start);
        if (boot_mmz_para_start != NULL) {

            boot_mmz_para_start += strlen("mmz_allocator=");

            /*find mmz parameter end*/
            boot_mmz_para_end = strchr(boot_mmz_para_start, ' ');

            /*if this is the last bootargs, bootargs end is mmz parameter end*/
            if (boot_mmz_para_end == NULL) {
                mmz_para_len = strlen(boot_mmz_para_start);
            } else {
                mmz_para_len = boot_mmz_para_end - boot_mmz_para_start;
            }

            mmz_para_len = ((mmz_para_len > MMZ_ALLOCATOR_NAME_LEN) ? MMZ_ALLOCATOR_NAME_LEN : mmz_para_len);
            memcpy(setup_allocator, boot_mmz_para_start, mmz_para_len);

            boot_mmz_para_start = lotus_get_cmd_line();
            /*no need judge here*/
            boot_mmz_para_start = strstr(boot_mmz_para_start, "mmz=") + strlen("mmz=");

            printk("mmz zone setting from bootargs:%s\n", boot_mmz_para_start);
            if (boot_mmz_para_start != NULL) {

                /*find mmz parameter end*/
                boot_mmz_para_end = strchr(boot_mmz_para_start, ' ');

                /*if this is the last bootargs, bootargs end is mmz parameter end*/
                if (boot_mmz_para_end == NULL) {
                    mmz_para_len = strlen(boot_mmz_para_start);
                } else {
                    mmz_para_len = boot_mmz_para_end - boot_mmz_para_start;
                }

                mmz_para_len = ((mmz_para_len > MMZ_SETUP_CMDLINE_LEN) ? MMZ_SETUP_CMDLINE_LEN : mmz_para_len);
                memcpy(setup_zones, boot_mmz_para_start, mmz_para_len);
                printk("mmz cfg from bootargs allocator:%s zones:%s\n", setup_allocator, setup_zones);
            }
        }
    }

    if (strcmp(setup_allocator, "cma") == 0) {
#ifdef CONFIG_CMA
        ret = cma_allocator_setopt(&the_allocator);
#else
        pr_err("cma is not enabled in kernel, please check!\n");
        return -EINVAL;
#endif
    } else if (strcmp(setup_allocator, "xmedia") == 0) {
        ret = allocator_setopt(&the_allocator);
    } else {
        printk("The module param \"setup_allocator\" should be \"cma\" or \"xmedia\", which is \"%s\"\n",
                setup_allocator);
        mmz_exit_check();
        return -EINVAL;
    }

    ret = the_allocator.init(setup_zones);
    if (ret != 0) {
        mmz_exit_check();
        return ret;
    }

    media_mem_proc_init();
    mmz_userdev_init();

    return 0;
}

void __exit media_mem_exit(void)
{
    mmz_userdev_exit();
    mmz_exit_check();
    media_mem_proc_exit();
}

/*
module_init(media_mem_init);
module_exit(media_mem_exit);

MODULE_LICENSE("GPL");
*/
/*
#ifndef MODULE
subsys_initcall(media_mem_init);
#endif
*/
