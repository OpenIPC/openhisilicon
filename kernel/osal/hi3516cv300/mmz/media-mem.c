/* media-mem.c
*
* Copyright (c) 2006 Hisilicon Co., Ltd.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*
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
#include <linux/seq_file.h>

#include <linux/string.h>
#include <linux/list.h>

#include <linux/time.h>
#include <asm/outercache.h>
#include <linux/dma-mapping.h>
#include <linux/highmem.h>

#include "osal_mmz.h"
#include "mmz-proc.h"
#include "hi_osal.h"
#include "allocator.h"



OSAL_LIST_HEAD(mmz_list);
static DEFINE_SEMAPHORE(mmz_lock);

int anony = 0;
module_param(anony, int, S_IRUGO);
//static int mmz_info_phys_start = -1;
int zone_number = 0;
int block_number = 0;
unsigned int mmb_number=0; /*for mmb id*/


#define MMZ_SETUP_CMDLINE_LEN   256
#define MMZ_ALLOCATOR_NAME_LEN  32

#ifndef MODULE

static char __initdata setup_zones[MMZ_SETUP_CMDLINE_LEN] = CONFIG_HISILICON_MMZ_DEFAULT;
static int __init parse_kern_cmdline(char *line)
{
    strlcpy(setup_zones, line, sizeof(setup_zones));

    return 1;
}
__setup("mmz=", parse_kern_cmdline);

static char __initdata setup_allocator[MMZ_ALLOCATOR_NAME_LEN];
static int __init parse_kern_allocator(char *line)
{
    strlcpy(setup_allocator, line, sizeof(setup_allocator));
    return 1;
}
__setup("mmz_allocator=", parse_kern_allocator);

#else
char setup_zones[MMZ_SETUP_CMDLINE_LEN]={'\0'};
char setup_allocator[MMZ_ALLOCATOR_NAME_LEN]="hisi";
module_param_string(mmz, setup_zones, MMZ_SETUP_CMDLINE_LEN, 0600);
module_param_string(mmz_allocator, setup_allocator, MMZ_ALLOCATOR_NAME_LEN, 0600);
MODULE_PARM_DESC(mmz,"mmz_allocator=allocator mmz=name,0,start,size,type,eqsize:[others]");
#endif

static struct mmz_allocator the_allocator;

hil_mmz_t *hil_mmz_create(const char *name,
        unsigned long gfp,
        unsigned long phys_start,
        unsigned long nbytes)
{
    hil_mmz_t *p = NULL;

    mmz_trace_func();

    if(name == NULL) {
        printk(KERN_ERR "%s: 'name' can not be zero!", __FUNCTION__);
        return NULL;
    }

    p = kmalloc(sizeof(hil_mmz_t) + 1, GFP_KERNEL);
    if (p == NULL)  {
        return NULL;
    }

    memset(p, 0, sizeof(hil_mmz_t)+1);
    strlcpy(p->name, name, HIL_MMZ_NAME_LEN);
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
    hil_mmz_t *p;

    mmz_trace_func();

    if(name == NULL) {
        printk(KERN_ERR "%s: 'name' can not be zero!", __FUNCTION__);
        return NULL;
    }

    p = kmalloc(sizeof(hil_mmz_t), GFP_KERNEL);
    if (p == NULL) {
        return NULL;
    }

    memset(p, 0, sizeof(hil_mmz_t));
    strlcpy(p->name, name, HIL_MMZ_NAME_LEN);
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
    if(zone == NULL)
        return -1;
    if(zone->destructor)
        zone->destructor(zone);
    return 0;
}
EXPORT_SYMBOL(hil_mmz_destroy);

static int _check_mmz(hil_mmz_t *zone)
{
    hil_mmz_t *p;

    unsigned long new_start=zone->phys_start;
    unsigned long new_end=zone->phys_start+zone->nbytes;

    if (zone->nbytes == 0)
        return -1;

    if (!((new_start>=__pa(high_memory))
            || (new_start<PHYS_OFFSET && new_end<=PHYS_OFFSET))) {
        printk(KERN_ERR "ERROR: Conflict MMZ:\n");
        printk(KERN_ERR HIL_MMZ_FMT_S "\n", hil_mmz_fmt_arg(zone));
        printk(KERN_ERR "MMZ conflict to kernel memory (0x%08lX, 0x%08lX)\n",
                (long unsigned int)PHYS_OFFSET,
                (long unsigned int)(__pa(high_memory) - 1));
        return -1;
    }

    osal_list_for_each_entry(p,&mmz_list, list) {
        unsigned long start, end;
        start = p->phys_start;
        end   = p->phys_start + p->nbytes;

        if(new_start >= end)
            continue;
        else if (new_start < start && new_end <= start)
            continue;
        else
            ;

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
    int ret = 0;

    mmz_trace(1, HIL_MMZ_FMT_S, hil_mmz_fmt_arg(zone));

    if(zone == NULL)
        return -1;

    down(&mmz_lock);

    if (0 == strcmp(setup_allocator, "hisi"))
    {
        ret = _check_mmz(zone);
        if (ret)
        {
            up(&mmz_lock);
            return ret;
        }
    }

    OSAL_INIT_LIST_HEAD(&zone->mmb_list);

    osal_list_add(&zone->list, &mmz_list);

    up(&mmz_lock);

    return 0;
}

int hil_mmz_unregister(hil_mmz_t *zone)
{
    int losts = 0;
    hil_mmb_t *p;

    if(zone == NULL)
        return -1;

    mmz_trace_func();

    down(&mmz_lock);
    osal_list_for_each_entry(p,&zone->mmb_list, list) {
        printk(KERN_WARNING "          MB Lost: " HIL_MMB_FMT_S "\n", hil_mmb_fmt_arg(p));
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

hil_mmb_t *hil_mmb_alloc(const char *name, unsigned long size, unsigned long align,
        unsigned long gfp, const char *mmz_name)
{
    hil_mmb_t *mmb;

    down(&mmz_lock);
    mmb = the_allocator.mmb_alloc(name, size, align, gfp, mmz_name, NULL);
    up(&mmz_lock);

    return mmb;
}
EXPORT_SYMBOL(hil_mmb_alloc);



hil_mmb_t *hil_mmb_alloc_in(const char *name, unsigned long size, unsigned long align,
        hil_mmz_t *_user_mmz)
{
    hil_mmb_t *mmb;

    if(_user_mmz==NULL)
        return NULL;

    down(&mmz_lock);
    mmb = the_allocator.mmb_alloc(name, size, align, _user_mmz->gfp, _user_mmz->name, _user_mmz);
    up(&mmz_lock);

    return mmb;
}

void *hil_mmb_map2kern(hil_mmb_t *mmb)
{
    void *p;

    if(mmb == NULL)
        return NULL;

    down(&mmz_lock);
    p = the_allocator.mmb_map2kern(mmb, 0);
    up(&mmz_lock);

    return p;
}
EXPORT_SYMBOL(hil_mmb_map2kern);

/* mmf: media-memory fragment */
void *hil_mmf_map2kern_nocache(unsigned long phys, int len)
{
    void *virt;
    down(&mmz_lock);
    virt = the_allocator.mmf_map(phys, len, 0);
    if (virt)
    {
        up(&mmz_lock);
        return virt;
    }

    up(&mmz_lock);
    return NULL;
}
EXPORT_SYMBOL(hil_mmf_map2kern_nocache);

void *hil_mmf_map2kern_cache(unsigned long phys, int len)
{
    void *virt;
    down(&mmz_lock);
    virt = the_allocator.mmf_map(phys, len, 1);
    if (virt)
    {
        up(&mmz_lock);
        return virt;
    }

    up(&mmz_lock);
    return NULL;
}
EXPORT_SYMBOL(hil_mmf_map2kern_cache);

void hil_mmf_unmap(void *virt)
{
    down(&mmz_lock);
    the_allocator.mmf_unmap(virt);
    up(&mmz_lock);
}
EXPORT_SYMBOL(hil_mmf_unmap);

void *hil_mmb_map2kern_cached(hil_mmb_t *mmb)
{
    void *p;

    if(mmb == NULL)
        return NULL;

    down(&mmz_lock);
    p = the_allocator.mmb_map2kern(mmb, 1);
    up(&mmz_lock);

    return p;
}
EXPORT_SYMBOL(hil_mmb_map2kern_cached);

int hil_mmb_flush_dcache_byaddr(void *kvirt, unsigned long phys_addr, unsigned long length)
{
    if (NULL == kvirt)
        return -EINVAL;

    /*Use flush range to instead flush_cache_all, because flush_cache_all only flush
      local cpu. And on_each_cpu macro cannot used to flush all cpus with irq disabled.*/

    /* dmac_map_area is invalid in  hi3518ev200 kernel, arm9 is not supported yet */
#if (HICHIP==0x3516A100)
    /* flush without clean */
    dmac_map_area(kvirt, length, DMA_TO_DEVICE);
#else
    __cpuc_flush_dcache_area(kvirt, length);
#endif

#if defined(CONFIG_CACHE_HIL2V200) || defined(CONFIG_CACHE_L2X0)
    /*flush l2 cache, use paddr*/
    /* if length > L2 cache size, then this interface will call <outer_flush_all> */
    outer_flush_range(phys_addr, phys_addr + length);
#endif

    return 0;
}
EXPORT_SYMBOL(hil_mmb_flush_dcache_byaddr);

int hil_mmb_invalid_cache_byaddr(void *kvirt,
        unsigned long phys_addr,
        unsigned long length)
{
    if (NULL == kvirt)
        return -EINVAL;

    /* dmac_map_area is invalid in  hi3518ev200 kernel, arm9 is not supported yet */
#if (HICHIP==0x3516A100)
    /* flush without clean */
    dmac_map_area(kvirt, length, DMA_FROM_DEVICE);
#else
    __cpuc_flush_dcache_area(kvirt, length);
#endif

    return 0;
}
EXPORT_SYMBOL(hil_mmb_invalid_cache_byaddr);

int hil_mmb_unmap(hil_mmb_t *mmb)
{
    int ref;

    if(mmb == NULL)
        return -1;

    down(&mmz_lock);

    ref = the_allocator.mmb_unmap(mmb);

    up(&mmz_lock);

    return 0;
}
EXPORT_SYMBOL(hil_mmb_unmap);

int hil_mmb_get(hil_mmb_t *mmb)
{
    int ref;

    if(mmb == NULL)
        return -1;

    down(&mmz_lock);

    if(mmb->flags & HIL_MMB_RELEASED)
        printk(KERN_WARNING "hil_mmb_get: amazing, mmb<%s> is released!\n", mmb->name);
    ref = ++mmb->phy_ref;

    up(&mmz_lock);

    return ref;
}

int hil_mmb_put(hil_mmb_t *mmb)
{
    int ref;

    if(mmb == NULL)
        return -1;

    down(&mmz_lock);

    if(mmb->flags & HIL_MMB_RELEASED)
        printk(KERN_WARNING "hil_mmb_put: amazing, mmb<%s> is released!\n", mmb->name);

    ref = --mmb->phy_ref;

    if((mmb->flags & HIL_MMB_RELEASED) && mmb->phy_ref ==0 && mmb->map_ref ==0) {
        the_allocator.mmb_free(mmb);
    }

    up(&mmz_lock);

    return ref;
}

int hil_mmb_free(hil_mmb_t *mmb)
{
    mmz_trace_func();
    if(mmb == NULL)
        return -1;
    mmz_trace(1,HIL_MMB_FMT_S,hil_mmb_fmt_arg(mmb));
    down(&mmz_lock);

    if(mmb->flags & HIL_MMB_RELEASED) {
    //  printk(KERN_WARNING "hil_mmb_free: amazing, mmb<%s> has been released,
    //          but is still in use!\n", mmb->name);
        up(&mmz_lock);
        return 0;
    }

    if (mmb->phy_ref > 0) {
    //  printk(KERN_WARNING "hil_mmb_free: free mmb<%s> delayed
    //          for which ref-count is %d!\n",
    //          mmb->name, mmb->map_ref);
        mmb->flags |= HIL_MMB_RELEASED;
        up(&mmz_lock);
        return 0;
    }

    if(mmb->flags & HIL_MMB_MAP2KERN) {
    //  printk(KERN_WARNING "hil_mmb_free: free mmb<%s> delayed for which
    //              is kernel-mapped to 0x%p with map_ref %d!\n",
    //          mmb->name, mmb->kvirt, mmb->map_ref);
        mmb->flags |= HIL_MMB_RELEASED;
        up(&mmz_lock);
        return 0;
    }
    the_allocator.mmb_free(mmb);
    up(&mmz_lock);
    return 0;
}
EXPORT_SYMBOL(hil_mmb_free);

#define MACH_MMB(p, val, member) do{\
    hil_mmz_t *__mach_mmb_zone__; \
    (p) = NULL;\
    list_for_each_entry(__mach_mmb_zone__,&mmz_list, list) { \
        hil_mmb_t *__mach_mmb__;\
        list_for_each_entry(__mach_mmb__,&__mach_mmb_zone__->mmb_list, list) { \
            if(__mach_mmb__->member == (val)){ \
                (p) = __mach_mmb__; \
                break;\
            } \
        } \
        if(p)break;\
    } \
}while(0)

hil_mmb_t *hil_mmb_getby_phys(unsigned long addr)
{
    hil_mmb_t *p;
    down(&mmz_lock);
    MACH_MMB(p, addr, phys_addr);
    up(&mmz_lock);
    return p;
}
EXPORT_SYMBOL(hil_mmb_getby_phys);
unsigned long usr_virt_to_phys(unsigned long virt)
{
    pgd_t *pgd;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *pte;
    unsigned int cacheable = 0;
    unsigned long page_addr = 0;
    unsigned long page_offset = 0;
    unsigned long phys_addr = 0;

    if (virt & 0x3) {
        printk("Invalid virtual address 0x%lx[not 4 bytes align]\n", virt);
        return 0;
    }

    if (virt >= CONFIG_PAGE_OFFSET) {
        printk("invalid usr virtual address 0x%lx\n", virt);
        return 0;
    }

    pgd = pgd_offset(current->mm, virt);
    if (pgd_none(*pgd)) {
        printk("error: not mapped in pgd!\n");
        return 0;
    }

    /* 5-level paging (5.0+) inserted p4d between pgd and pud. On 32-bit
     * ARM p4d is folded so p4d_offset(pgd, virt) is identity. */
    pud = pud_offset(p4d_offset(pgd, virt), virt);
    if (pud_none(*pud)) {
        printk("error: not mapped in pud!\n");
        return 0;
    }

    pmd = pmd_offset(pud, virt);
    if (pmd_none(*pmd)) {
        printk("error: not mapped in pmd!\n");
        return 0;
    }

    /* pte_offset_map became __pte_offset_map (rcu-protected) in 6.5+ and
     * is no longer exported to modules. pte_offset_kernel skips the rcu
     * unmap dance and works for kernel-mode page tables (which is what
     * mmz walks via current->mm). */
    pte = pte_offset_kernel(pmd, virt);
    if (pte_none(*pte)) {
        printk("error: not mapped in pte!\n");
        return 0;
    }

    page_addr = pte_val(*pte) & PAGE_MASK;
    page_offset = virt & ~PAGE_MASK;
    phys_addr = page_addr | page_offset;

    if (pte_val(*pte) & (1<<3))
        cacheable = 1;

    /*
     * phys_addr: the lowest bit indicates its cache attribute
     * 1: cacheable
     * 0: uncacheable
     */
    phys_addr |= cacheable;

    //printk("cached[%d]!!\n", cacheable);
    //printk("--------     result phys 0x%lx\n", phys_addr);

    return phys_addr;
}
EXPORT_SYMBOL(usr_virt_to_phys);

#define MACH_MMB_2(p, val, member, Outoffset) do{\
    hil_mmz_t *__mach_mmb_zone__; \
    (p) = NULL;\
    list_for_each_entry(__mach_mmb_zone__,&mmz_list, list) { \
        hil_mmb_t *__mach_mmb__;\
        list_for_each_entry(__mach_mmb__,&__mach_mmb_zone__->mmb_list, list) { \
            if ((__mach_mmb__->member <= (val)) && ((__mach_mmb__->length + __mach_mmb__->member) > (val))){ \
                (p) = __mach_mmb__; \
                Outoffset = val - __mach_mmb__->member;\
                break;\
            }\
        } \
        if(p)break;\
    } \
}while(0)


hil_mmb_t *hil_mmb_getby_kvirt(void *virt)
{
    hil_mmb_t *p;
    unsigned long Outoffset;

    if(virt == NULL)
        return NULL;

    down(&mmz_lock);
    MACH_MMB_2(p, virt, kvirt, Outoffset);
    up(&mmz_lock);

    return p;
}
EXPORT_SYMBOL(hil_mmb_getby_kvirt);


hil_mmb_t *hil_mmb_getby_phys_2(unsigned long addr, unsigned long *Outoffset)
{
    hil_mmb_t *p;

    down(&mmz_lock);
    MACH_MMB_2(p, addr, phys_addr, *Outoffset);
    up(&mmz_lock);
    return p;
}
EXPORT_SYMBOL(hil_mmb_getby_phys_2);

hil_mmz_t *hil_mmz_find(unsigned long gfp, const char *mmz_name)
{
    hil_mmz_t *p;

    down(&mmz_lock);
    begin_list_for_each_mmz(p, gfp, mmz_name)
        up(&mmz_lock);
        return p;
    end_list_for_each_mmz()
    up(&mmz_lock);

    return NULL;
}


#define MEDIA_MEM_NAME  "media-mem"

#ifdef CONFIG_PROC_FS


int mmz_read_proc(struct seq_file *sfile)
{
    hil_mmz_t *p;
    int len = 0;
    unsigned int zone_number = 0;
    unsigned int block_number = 0;
    unsigned int used_size = 0;
    unsigned int free_size = 0;
    unsigned int mmz_total_size = 0;

    mmz_trace_func();

    down(&mmz_lock);
    osal_list_for_each_entry(p,&mmz_list, list) {
        hil_mmb_t *mmb;
        seq_printf(sfile, "+---ZONE: " HIL_MMZ_FMT_S "\n", hil_mmz_fmt_arg(p));
        mmz_total_size += p->nbytes / 1024;
        ++zone_number;

        osal_list_for_each_entry(mmb,&p->mmb_list, list) {
            seq_printf(sfile, "   |-MMB: " HIL_MMB_FMT_S "\n", hil_mmb_fmt_arg(mmb));
            used_size += mmb->length / 1024;
            ++block_number;
        }
    }

    if (0 != mmz_total_size) {
        free_size = mmz_total_size - used_size;
        seq_printf(sfile, "\n---MMZ_USE_INFO:\n total size=%dKB(%dMB),"
                "used=%dKB(%dMB + %dKB),remain=%dKB(%dMB + %dKB),"
                "zone_number=%d,block_number=%d\n",
                mmz_total_size, mmz_total_size / 1024,
                used_size, used_size / 1024, used_size % 1024,
                free_size, free_size / 1024, free_size % 1024,
                zone_number, block_number);
        //mmz_total_size = 0;
        //zone_number = 0;
        //block_number = 0;
    }
    up(&mmz_lock);

    return len;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 0)
static int mmz_write_proc(struct file *file, const char __user *buffer,
                                   unsigned long count, void *data)
#else
static ssize_t mmz_write_proc(struct file *file, const char __user *buffer,
                                   size_t count, loff_t *data)
#endif

{
    char buf[256];

    if(count >= sizeof(buf)) {
        printk(KERN_ERR "MMZ: your parameter string is too long!\n");
        return -EIO;
    }

    memset(buf, 0, sizeof(buf));
    if (copy_from_user(buf, buffer, count)) {
        printk("\nmmz_userdev_ioctl: copy_from_user error.\n");
        return 0;
    }

    the_allocator.init(buf);
    //media_mem_parse_cmdline(buf);

    return count;
}

#define MMZ_PROC_ROOT  NULL

static const struct seq_operations mmz_seq_ops = {
    .start = mmz_seq_start,
    .next = mmz_seq_next,
    .stop = mmz_seq_stop,
    .show = mmz_seq_show
};

static int mmz_proc_open(struct inode *inode, struct file *file)
{
    return seq_open(file, &mmz_seq_ops);
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 0)
static struct file_operations mmz_proc_ops = {
    .owner = THIS_MODULE,
    .open = mmz_proc_open,
    .read = seq_read,
    .release = seq_release,
};
static int __init media_mem_proc_init(void)
{
    struct proc_dir_entry *p;

    p = create_proc_entry(MEDIA_MEM_NAME, 0644, MMZ_PROC_ROOT);
    if(p == NULL)
        return -1;
    p->write_proc = mmz_write_proc;
    p->proc_fops = &mmz_proc_ops;

    return 0;
}
#else

#ifdef COMPAT_USE_PROC_OPS
/* 5.6+: struct proc_ops replaces struct file_operations for proc files. */
static struct proc_ops mmz_proc_ops = {
    .proc_open    = mmz_proc_open,
    .proc_read    = seq_read,
    .proc_write   = mmz_write_proc,
    .proc_release = seq_release,
};
#else
static struct file_operations mmz_proc_ops = {
    .owner = THIS_MODULE,
    .open = mmz_proc_open,
    .read = seq_read,
    .write = mmz_write_proc,
    .release = seq_release,
};
#endif

static int __init media_mem_proc_init(void)
{
    struct proc_dir_entry *p;

    p = proc_create(MEDIA_MEM_NAME, 0, MMZ_PROC_ROOT, &mmz_proc_ops);
    if (!p){
        printk(KERN_ERR "Create mmz proc fail!\n");
        return -1;
    }
    return 0;
}
#endif

static void __exit media_mem_proc_exit(void)
{
    remove_proc_entry(MEDIA_MEM_NAME, MMZ_PROC_ROOT);
}

#else
static int __init media_mem_proc_init(void){ return 0; }
static void __exit media_mem_proc_exit(void){ }

#endif /* CONFIG_PROC_FS */


extern int mmz_userdev_init(void);
extern void mmz_userdev_exit(void);

static void mmz_exit_check(void)
{
    hil_mmz_t *pmmz;
    struct osal_list_head *p, *n;

    mmz_trace_func();

    list_for_each_safe(p, n, &mmz_list) {
        pmmz = osal_list_entry(p,hil_mmz_t,list);
        printk(KERN_WARNING "MMZ force removed: " HIL_MMZ_FMT_S "\n", hil_mmz_fmt_arg(pmmz));
        hil_mmz_unregister(pmmz);
        hil_mmz_destroy(pmmz);
    }
}

int mem_check_module_param(void)
{
    if(1 != anony)
    {
        printk("The module param \"anony\" should only be 1 which is %d \n",anony);
        return -1;
    }
    return 0;
}

int media_mem_init(void)
{
    int ret = 0;

    printk(KERN_INFO "Hisilicon Media Memory Zone Manager\n");

    if (1 != anony) {
        printk("The module param \"anony\" should only be 1 which is %d\n", anony);
        return -1;
    }

    if (0 == strcmp(setup_allocator, "cma"))
    {
        printk("The module param setup_allocator is %s \n",setup_allocator);
        ret = cma_allocator_setopt(&the_allocator);
    }
    else if (0 == strcmp(setup_allocator, "hisi"))
    {
        printk("The module param setup_allocator is %s \n",setup_allocator);
        ret = hisi_allocator_setopt(&the_allocator);
    } else {
        printk("The module param \"setup_allocator\" should be \"cma\" or \"hisi\", which is \"%s\"\n",
                setup_allocator);
        mmz_exit_check();
        return -EINVAL;
    }

#if 1
    //ret = media_mem_parse_cmdline(setup_zones);
    ret = the_allocator.init(setup_zones);
    if (ret != 0)
    {
        mmz_exit_check();
        return ret;
    }
#endif

    media_mem_proc_init();

    mmz_userdev_init();

    return 0;
}

#ifdef MODULE

void media_mem_exit(void)
{
    mmz_userdev_exit();

    mmz_exit_check();

    media_mem_proc_exit();
}
/*
module_init(media_mem_init);
module_exit(media_mem_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hisilicon");
*/
#else

subsys_initcall(media_mem_init);

#endif

