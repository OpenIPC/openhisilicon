/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
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
#include "mmz-proc.h"
#include "osal.h"
#include "allocator.h"

#include "../../../../compat/compat.h"

#define EXPORT_COMPAT_MMZ(common_name)    \
	EXPORT_SYMBOL(mmz_##common_name); \
	EXPORT_ALIAS(mmz_##common_name, hil_##common_name)

OSAL_LIST_HEAD(mmz_list);

extern struct osal_list_head map_mmz_list;
OSAL_LIST_HEAD(map_mmz_list);

int anony = 0;
static DEFINE_SEMAPHORE(mmz_lock);

module_param(anony, int, S_IRUGO);
int zone_number = 0;
int block_number = 0;

#define MMZ_SETUP_CMDLINE_LEN 256
#define MMZ_ALLOCATOR_NAME_LEN 32

#ifndef MODULE

static char __initdata setup_zones[MMZ_SETUP_CMDLINE_LEN] =
	CONFIG_VENDOR_MMZ_DEFAULT;
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

static char __initdata mmap_zones[MMZ_SETUP_CMDLINE_LEN] = '\0';
__setup("map_mmz=", parse_kern_cmdline);

#else
static char setup_zones[MMZ_SETUP_CMDLINE_LEN] = { '\0' };
static char mmap_zones[MMZ_SETUP_CMDLINE_LEN] = { '\0' };
static char setup_allocator[MMZ_ALLOCATOR_NAME_LEN] =
	DEFAULT_ALLOCATOR; //default setting
module_param_string(mmz, setup_zones, MMZ_SETUP_CMDLINE_LEN, 0600);
module_param_string(map_mmz, mmap_zones, MMZ_SETUP_CMDLINE_LEN, 0600);
module_param_string(mmz_allocator, setup_allocator, MMZ_ALLOCATOR_NAME_LEN,
		    0600);
MODULE_PARM_DESC(
	mmz,
	"mmz_allocator=allocator mmz=name,0,start,size,type,eqsize:[others] map_mmz=start,size:[others]");
#endif

int allocator_type;
static struct mmz_allocator the_allocator;

mmz_mmz_t *mmz_mmz_create(const char *name, unsigned long gfp,
			  unsigned long phys_start, unsigned long nbytes)
{
	mmz_mmz_t *p = NULL;

	mmz_trace_func();

	if (name == NULL) {
		printk(KERN_ERR "%s: 'name' should not be NULL!", __FUNCTION__);
		return NULL;
	}

	p = kmalloc(sizeof(mmz_mmz_t) + 1, GFP_KERNEL);
	if (p == NULL) {
		printk(KERN_ERR "%s: System OOM!\n", __func__);
		return NULL;
	}

	memset(p, 0, sizeof(mmz_mmz_t) + 1);
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

mmz_mmz_t *mmz_mmz_create_v2(const char *name, unsigned long gfp,
			     unsigned long phys_start, unsigned long nbytes,
			     unsigned int alloc_type, unsigned long block_align)
{
	mmz_mmz_t *p = NULL;

	mmz_trace_func();

	if (name == NULL) {
		printk(KERN_ERR "%s: 'name' can not be zero!", __FUNCTION__);
		return NULL;
	}

	p = kmalloc(sizeof(mmz_mmz_t), GFP_KERNEL);
	if (p == NULL) {
		return NULL;
	}

	memset(p, 0, sizeof(mmz_mmz_t));
	strlcpy(p->name, name, MMZ_MMZ_NAME_LEN);
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

int mmz_mmz_destroy(mmz_mmz_t *zone)
{
	if (zone == NULL) {
		return -1;
	}

	if (zone->destructor) {
		zone->destructor(zone);
	}
	return 0;
}
EXPORT_SYMBOL(mmz_mmz_destroy);

static int _check_mmz(mmz_mmz_t *zone)
{
	mmz_mmz_t *p = NULL;

	unsigned long new_start = zone->phys_start;
	unsigned long new_end = zone->phys_start + zone->nbytes;

	if (zone->nbytes == 0) {
		return -1;
	}

	if (!((new_start >= __pa((uintptr_t)high_memory)) ||
	      ((new_start < PHYS_OFFSET) && (new_end <= PHYS_OFFSET)))) {
		printk(KERN_ERR "ERROR: Conflict MMZ:\n");
		printk(KERN_ERR MMZ_MMZ_FMT_S "\n", mmz_mmz_fmt_arg(zone));
		printk(KERN_ERR
		       "MMZ conflict to kernel memory (0x%08lX, 0x%08lX)\n",
		       (long unsigned int)PHYS_OFFSET,
		       (long unsigned int)(__pa((uintptr_t)high_memory) - 1));
		return -1;
	}

	osal_list_for_each_entry(p, &mmz_list, list)
	{
		unsigned long start, end;
		start = p->phys_start;
		end = p->phys_start + p->nbytes;

		if (new_start >= end) {
			continue;
		} else if (new_start < start && new_end <= start) {
			continue;
		} else {
			;
		}

		printk(KERN_ERR "ERROR: Conflict MMZ:\n");
		printk(KERN_ERR "MMZ new:   " MMZ_MMZ_FMT_S "\n",
		       mmz_mmz_fmt_arg(zone));
		printk(KERN_ERR "MMZ exist: " MMZ_MMZ_FMT_S "\n",
		       mmz_mmz_fmt_arg(p));
		printk(KERN_ERR "Add new MMZ failed!\n");
		return -1;
	}

	return 0;
}

int mmz_mmz_register(mmz_mmz_t *zone)
{
	int ret = 0;

	mmz_trace(1, MMZ_MMZ_FMT_S, mmz_mmz_fmt_arg(zone));

	if (zone == NULL) {
		return -1;
	}

	down(&mmz_lock);

	if (0 == strcmp(setup_allocator, DEFAULT_ALLOCATOR)) {
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

	if (zone == NULL)
		return -1;

	mmz_trace_func();

	down(&mmz_lock);
	osal_list_for_each_entry(p, &zone->mmb_list, list)
	{
		printk(KERN_WARNING "MB Lost: " MMZ_MMB_FMT_S "\n",
		       mmz_mmb_fmt_arg(p));
		losts++;
	}

	if (losts) {
		printk(KERN_ERR
		       "%d mmbs not free, mmz<%s> can not be unregistered!\n",
		       losts, zone->name);
		up(&mmz_lock);
		return -1;
	}

	osal_list_del(&zone->list);
	up(&mmz_lock);

	return 0;
}

mmz_mmb_t *mmz_mmb_alloc(const char *name, unsigned long size,
			 unsigned long align, unsigned long gfp,
			 const char *mmz_name)
{
	mmz_mmb_t *mmb = NULL;

	down(&mmz_lock);
	mmb = the_allocator.mmb_alloc(name, size, align, gfp, mmz_name, NULL);
	up(&mmz_lock);

	return mmb;
}
EXPORT_COMPAT_MMZ(mmb_alloc);

mmz_mmb_t *mmz_mmb_alloc_v2(const char *name, unsigned long size,
			    unsigned long align, unsigned long gfp,
			    const char *mmz_name, unsigned int order)
{
	mmz_mmb_t *mmb = NULL;

	down(&mmz_lock);
	mmb = the_allocator.mmb_alloc_v2(name, size, align, gfp, mmz_name, NULL,
					 order);
	up(&mmz_lock);

	return mmb;
}
EXPORT_SYMBOL(mmz_mmb_alloc_v2);

mmz_mmb_t *mmz_mmb_alloc_in(const char *name, unsigned long size,
			    unsigned long align, mmz_mmz_t *_user_mmz)
{
	mmz_mmb_t *mmb = NULL;

	if (_user_mmz == NULL)
		return NULL;

	down(&mmz_lock);
	mmb = the_allocator.mmb_alloc(name, size, align, _user_mmz->gfp,
				      _user_mmz->name, _user_mmz);
	up(&mmz_lock);

	return mmb;
}

mmz_mmb_t *mmz_mmb_alloc_in_v2(const char *name, unsigned long size,
			       unsigned long align, mmz_mmz_t *_user_mmz,
			       unsigned int order)
{
	mmz_mmb_t *mmb = NULL;

	if (_user_mmz == NULL)
		return NULL;

	down(&mmz_lock);
	mmb = the_allocator.mmb_alloc_v2(name, size, align, _user_mmz->gfp,
					 _user_mmz->name, _user_mmz, order);
	up(&mmz_lock);

	return mmb;
}

void *mmz_mmb_map2kern(mmz_mmb_t *mmb)
{
	void *p = NULL;

	if (mmb == NULL)
		return NULL;

	down(&mmz_lock);
	p = the_allocator.mmb_map2kern(mmb, 0);
	up(&mmz_lock);

	return p;
}
EXPORT_COMPAT_MMZ(mmb_map2kern);

/* mmf: media-memory fragment */
void *mmz_mmf_map2kern_nocache(unsigned long phys, int len)
{
	void *virt = the_allocator.mmf_map(phys, len, 0);
	if (virt != NULL)
		return virt;

	return NULL;
}
EXPORT_COMPAT_MMZ(mmf_map2kern_nocache);

void *mmz_mmf_map2kern_cache(unsigned long phys, int len)
{
	void *virt = the_allocator.mmf_map(phys, len, 1);
	if (virt != NULL)
		return virt;

	return NULL;
}
EXPORT_COMPAT_MMZ(mmf_map2kern_cache);

void mmz_mmf_unmap(void *virt)
{
	the_allocator.mmf_unmap(virt);
}
EXPORT_COMPAT_MMZ(mmf_unmap);

void *mmz_mmb_map2kern_cached(mmz_mmb_t *mmb)
{
	void *p = NULL;

	if (mmb == NULL)
		return NULL;

	down(&mmz_lock);
	p = the_allocator.mmb_map2kern(mmb, 1);
	up(&mmz_lock);

	return p;
}
EXPORT_COMPAT_MMZ(mmb_map2kern_cached);

int mmz_mmb_flush_dcache_byaddr(void *kvirt, unsigned long phys_addr,
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
     * dmac_map_area is invalid in  hi3518ev200 kernel,
     * arm9 is not supported yet
     */
#if defined(hi3516av100)
	/* flush without clean */
	dmac_map_area(kvirt, length, DMA_TO_DEVICE);
#else
	__cpuc_flush_dcache_area(kvirt, length);
#endif
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
EXPORT_COMPAT_MMZ(mmb_flush_dcache_byaddr);

int mmz_mmb_invalid_cache_byaddr(void *kvirt, unsigned long phys_addr,
				 unsigned long length)
{
	if (kvirt == NULL)
		return -EINVAL;

#ifdef CONFIG_64BIT
	__flush_dcache_area(kvirt, length);
#else
		/*
     * dmac_map_area is invalid in  hi3518ev200 kernel,
     * arm9 is not supported yet
     */
#if defined(hi3516av100)
	/* flush without clean */
	dmac_map_area(kvirt, length, DMA_FROM_DEVICE);
#else
	__cpuc_flush_dcache_area(kvirt, length);
#endif
#endif
	return 0;
}
EXPORT_COMPAT_MMZ(mmb_invalid_cache_byaddr);

int mmz_mmb_unmap(mmz_mmb_t *mmb)
{
	int ref;

	if (mmb == NULL)
		return -1;

	down(&mmz_lock);

	ref = the_allocator.mmb_unmap(mmb);

	up(&mmz_lock);

	return ref;
}
EXPORT_COMPAT_MMZ(mmb_unmap);

int mmz_mmb_get(mmz_mmb_t *mmb)
{
	int ref;

	if (mmb == NULL)
		return -1;

	down(&mmz_lock);

	if (mmb->flags & MMZ_MMB_RELEASED)
		printk(KERN_WARNING
		       "mmz_mmb_get: amazing, mmb<%s> is released!\n",
		       mmb->name);
	ref = ++mmb->phy_ref;

	up(&mmz_lock);

	return ref;
}

int mmz_mmb_put(mmz_mmb_t *mmb)
{
	int ref;

	if (mmb == NULL)
		return -1;

	down(&mmz_lock);

	if (mmb->flags & MMZ_MMB_RELEASED)
		printk(KERN_WARNING
		       "mmz_mmb_put: amazing, mmb<%s> is released!\n",
		       mmb->name);

	ref = --mmb->phy_ref;

	if ((mmb->flags & MMZ_MMB_RELEASED) && (mmb->phy_ref == 0) &&
	    (mmb->map_ref == 0)) {
		the_allocator.mmb_free(mmb);
	}

	up(&mmz_lock);

	return ref;
}

int mmz_mmb_free(mmz_mmb_t *mmb)
{
	mmz_trace_func();

	if (mmb == NULL)
		return -1;

	mmz_trace(1, MMZ_MMB_FMT_S, mmz_mmb_fmt_arg(mmb));
	down(&mmz_lock);

	if (mmb->flags & MMZ_MMB_RELEASED) {
		printk(KERN_WARNING
		       "mmz_mmb_free: amazing, mmb<%s> has been released,\
                but is still in use!\n",
		       mmb->name);
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
		printk(KERN_WARNING
		       "mmz_mmb_free: free mmb<%s> delayed for which \
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
EXPORT_COMPAT_MMZ(mmb_free);

#define MACH_MMB(p, val, member)                                          \
	do {                                                              \
		mmz_mmz_t *__mach_mmb_zone__ = NULL;                      \
		(p) = NULL;                                               \
		list_for_each_entry(__mach_mmb_zone__, &mmz_list, list) { \
			mmz_mmb_t *__mach_mmb__ = NULL;                   \
			list_for_each_entry(__mach_mmb__,                 \
					    &__mach_mmb_zone__->mmb_list, \
					    list) {                       \
				if (__mach_mmb__->member == (val)) {      \
					(p) = __mach_mmb__;               \
					break;                            \
				}                                         \
			}                                                 \
			if (p != NULL)                                    \
				break;                                    \
		}                                                         \
	} while (0)

mmz_mmb_t *mmz_mmb_getby_phys(unsigned long addr)
{
	mmz_mmb_t *p = NULL;
	down(&mmz_lock);
	MACH_MMB(p, addr, phys_addr);
	up(&mmz_lock);
	return p;
}
EXPORT_COMPAT_MMZ(mmb_getby_phys);

unsigned long usr_virt_to_phys(unsigned long virt)
{
	pgd_t *pgd = NULL;
	pud_t *pud = NULL;
	pmd_t *pmd = NULL;
	pte_t *pte = NULL;
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
	{
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
}
EXPORT_SYMBOL(usr_virt_to_phys);

#define MACH_MMB_2(p, val, member, Outoffset)                               \
	do {                                                                \
		mmz_mmz_t *__mach_mmb_zone__ = NULL;                        \
		(p) = NULL;                                                 \
		list_for_each_entry(__mach_mmb_zone__, &mmz_list, list) {   \
			mmz_mmb_t *__mach_mmb__ = NULL;                     \
			list_for_each_entry(__mach_mmb__,                   \
					    &__mach_mmb_zone__->mmb_list,   \
					    list) {                         \
				if ((__mach_mmb__->member <= (val)) &&      \
				    ((__mach_mmb__->length +                \
				      __mach_mmb__->member) > (val))) {     \
					(p) = __mach_mmb__;                 \
					Outoffset =                         \
						val - __mach_mmb__->member; \
					break;                              \
				}                                           \
			}                                                   \
			if (p != NULL)                                      \
				break;                                      \
		}                                                           \
	} while (0)

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
EXPORT_COMPAT_MMZ(mmb_getby_kvirt);

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
	begin_list_for_each_mmz(p, gfp, mmz_name) up(&mmz_lock);
	return p;
	end_list_for_each_mmz() up(&mmz_lock);

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

static unsigned long _strtoul_ex(const char *s, char **ep, unsigned int base)
{
	char *__end_p = NULL;
	unsigned long __value;

	__value = simple_strtoul(s, &__end_p, base);

	switch (*__end_p) {
	case 'm':
	case 'M':
		__value <<= 10;
	case 'k':
	case 'K':
		__value <<= 10;
		if (ep != NULL)
			(*ep) = __end_p + 1;
	default:
		break;
	}

	return __value;
}

static int _check_map_mmz(mmz_mmz_t *zone)
{
	mmz_mmz_t *p = NULL;

	unsigned long new_start = zone->phys_start;
	unsigned long new_end = zone->phys_start + zone->nbytes;

	if (zone->nbytes == 0)
		return -1;

	osal_list_for_each_entry(p, &map_mmz_list, list)
	{
		unsigned long start, end;
		start = p->phys_start;
		end = p->phys_start + p->nbytes;

		if (new_start >= end)
			continue;
		else if ((new_start < start) && (new_end <= start))
			continue;
		else
			;

		printk(KERN_ERR "ERROR: Conflict MMZ:\n");
		printk(KERN_ERR "MMZ new:   " MMZ_MMZ_FMT_S "\n",
		       mmz_mmz_fmt_arg(zone));
		printk(KERN_ERR "MMZ exist: " MMZ_MMZ_FMT_S "\n",
		       mmz_mmz_fmt_arg(p));
		printk(KERN_ERR "Add new MMZ failed!\n");
		return -1;
	}

	return 0;
}

int mmz_map_mmz_register(mmz_mmz_t *zone)
{
	int ret = 0;

	mmz_trace(1, MMZ_MMZ_FMT_S, mmz_mmz_fmt_arg(zone));

	if (zone == NULL)
		return -1;

	down(&mmz_lock);

	if (0 == strcmp(setup_allocator, DEFAULT_ALLOCATOR)) {
		ret = _check_mmz(zone);
		if (ret) {
			up(&mmz_lock);
			return ret;
		}
	}

	ret = _check_map_mmz(zone);
	if (ret) {
		up(&mmz_lock);
		return ret;
	}

	OSAL_INIT_LIST_HEAD(&zone->mmb_list);

	osal_list_add(&zone->list, &map_mmz_list);

	up(&mmz_lock);

	return 0;
}

int mmz_map_mmz_unregister(mmz_mmz_t *zone)
{
	int losts = 0;
	mmz_mmb_t *p = NULL;

	if (zone == NULL)
		return -1;

	mmz_trace_func();

	down(&mmz_lock);
	osal_list_for_each_entry(p, &zone->mmb_list, list)
	{
		printk(KERN_WARNING "MB Lost: " MMZ_MMB_FMT_S "\n",
		       mmz_mmb_fmt_arg(p));
		losts++;
	}

	if (losts) {
		printk(KERN_ERR
		       "%d mmbs not free, mmz<%s> can not be unregistered!\n",
		       losts, zone->name);
		up(&mmz_lock);
		return -1;
	}

	osal_list_del(&zone->list);
	up(&mmz_lock);

	return 0;
}

static int map_mmz_init(char *s)
{
	mmz_mmz_t *zone = NULL;
	char *line = NULL;

	if (s[0] == '\0') {
		return 0;
	}

	while ((line = strsep(&s, ":")) != NULL) {
		int i;
		char *argv[2];

		for (i = 0; (argv[i] = strsep(&line, ",")) != NULL;)
			if (++i == ARRAY_SIZE(argv))
				break;

		if (i == 2) {
			zone = mmz_mmz_create("null", 0, 0, 0);
			if (zone == NULL)
				continue;
			zone->phys_start = _strtoul_ex(argv[0], NULL, 0);
			zone->nbytes = _strtoul_ex(argv[1], NULL, 0);
		} else {
			printk(KERN_ERR "error parameters\n");
			return -EINVAL;
		}

		if (mmz_map_mmz_register(zone)) {
			printk(KERN_WARNING "Add MMZ failed: " MMZ_MMZ_FMT_S
					    "\n",
			       mmz_mmz_fmt_arg(zone));
			mmz_mmz_destroy(zone);
		}
		zone = NULL;
	}

	return 0;
}

static void map_mmz_exit(void)
{
	mmz_mmz_t *pmmz = NULL;
	struct osal_list_head *p = NULL, *n = NULL;

	mmz_trace_func();

	list_for_each_safe(p, n, &map_mmz_list) {
		pmmz = list_entry(p, mmz_mmz_t, list);
		printk(KERN_WARNING "MMZ force removed: " MMZ_MMZ_FMT_S "\n",
		       mmz_mmz_fmt_arg(pmmz));
		mmz_map_mmz_unregister(pmmz);
		mmz_mmz_destroy(pmmz);
	}
}

int mmz_map_mmz_check_phys(unsigned long addr_start, unsigned long addr_len)
{
	mmz_mmz_t *p = NULL;
	unsigned long addr_end = addr_start + addr_len;
	unsigned long temp_start, temp_end;

	osal_list_for_each_entry(p, &map_mmz_list, list)
	{
		temp_start = p->phys_start;
		temp_end = p->phys_start + p->nbytes;
		if ((addr_start >= temp_start) && (addr_end <= temp_end)) {
			return 0;
		}
	}

	printk(KERN_ERR "ERROR: MMAP ADDR: 0x%lx-0x%lx\n", addr_start,
	       addr_end);

	return -1;
}
EXPORT_COMPAT_MMZ(map_mmz_check_phys);

int mmz_vma_check(unsigned long vm_start, unsigned long vm_end)
{
	struct vm_area_struct *pvma1;
	struct vm_area_struct *pvma2;

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
		printk(KERN_ERR
		       "ERROR: pvma1:[0x%lx,0x%lx) and pvma2:[0x%lx,0x%lx) are not equal\n",
		       pvma1->vm_start, pvma1->vm_end, pvma2->vm_start,
		       pvma2->vm_end);
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
EXPORT_SYMBOL(mmz_vma_check);

int mmz_is_phys_in_mmz(unsigned long addr_start, unsigned long addr_len)
{
	mmz_mmz_t *p = NULL;
	unsigned long addr_end = addr_start + addr_len;
	unsigned long temp_start, temp_end;

	osal_list_for_each_entry(p, &mmz_list, list)
	{
		temp_start = p->phys_start;
		temp_end = p->phys_start + p->nbytes;
		if ((addr_start >= temp_start) && (addr_end <= temp_end)) {
			return 0;
		}
	}

	return -1;
}
EXPORT_COMPAT_MMZ(is_phys_in_mmz);

int mmz_mmb_flush_dcache_byaddr_safe(void *kvirt, unsigned long phys_addr,
				     unsigned long length)
{
	int ret = 0;
	struct mm_struct *mm = current->mm;

	if (kvirt == NULL)
		return -EINVAL;

	down_read(&mm->mmap_sem);

	if (mmz_vma_check((unsigned long)(uintptr_t)kvirt,
			  (unsigned long)(uintptr_t)kvirt + length)) {
		up_read(&mm->mmap_sem);
		return -EPERM;
	}

	ret = mmz_mmb_flush_dcache_byaddr(kvirt, phys_addr, length);

	up_read(&mm->mmap_sem);

	return ret;
}
EXPORT_COMPAT_MMZ(mmb_flush_dcache_byaddr_safe);

#define MEDIA_MEM_NAME "media-mem"

#ifdef CONFIG_PROC_FS

int mmz_read_proc(struct seq_file *sfile)
{
	mmz_mmz_t *p = NULL;
	int len = 0;
	unsigned int zone_number = 0;
	unsigned int block_number = 0;
	unsigned int used_size = 0;
	unsigned int free_size = 0;
	unsigned int mmz_total_size = 0;

	mmz_trace_func();

	down(&mmz_lock);
	list_for_each_entry(p, &mmz_list, list) {
		mmz_mmb_t *mmb = NULL;
		seq_printf(sfile, "+---ZONE: " MMZ_MMZ_FMT_S "\n",
			   mmz_mmz_fmt_arg(p));
		mmz_total_size += p->nbytes / 1024;
		++zone_number;

		list_for_each_entry(mmb, &p->mmb_list, list) {
			seq_printf(sfile, "   |-MMB: " MMZ_MMB_FMT_S "\n",
				   mmz_mmb_fmt_arg(mmb));
			used_size += mmb->length / 1024;
			++block_number;
		}
	}

	if (mmz_total_size != 0) {
		free_size = mmz_total_size - used_size;
		seq_printf(sfile,
			   "\n---MMZ_USE_INFO:\n total size=%dKB(%dMB),"
			   "used=%dKB(%dMB + %dKB),remain=%dKB(%dMB + %dKB),"
			   "zone_number=%d,block_number=%d\n",
			   mmz_total_size, mmz_total_size / 1024, used_size,
			   used_size / 1024, used_size % 1024, free_size,
			   free_size / 1024, free_size % 1024, zone_number,
			   block_number);
		mmz_total_size = 0;
		zone_number = 0;
		block_number = 0;
	}
	up(&mmz_lock);

	return len;
}

#define MMZ_PROC_ROOT NULL

static const struct seq_operations mmz_seq_ops = { .start = mmz_seq_start,
						   .next = mmz_seq_next,
						   .stop = mmz_seq_stop,
						   .show = mmz_seq_show };

static int mmz_proc_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &mmz_seq_ops);
}

static struct file_operations mmz_proc_ops = {
	.owner = THIS_MODULE,
	.open = mmz_proc_open,
	.read = seq_read,
	.release = seq_release,
};

static int __init media_mem_proc_init(void)
{
	struct proc_dir_entry *p = NULL;

	p = proc_create(MEDIA_MEM_NAME, 0, MMZ_PROC_ROOT, &mmz_proc_ops);
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
static int __init media_mem_proc_init(void)
{
	return 0;
}
static void __exit media_mem_proc_exit(void)
{
}

#endif /* CONFIG_PROC_FS */

/* this function is used by osal.c */
int mem_check_module_param(void)
{
	if (anony != 1) {
		printk("The module param \"anony\" should only be 1 which is %d \n",
		       anony);
		return -1;
	}
	return 0;
}

static void __exit mmz_exit_check(void)
{
	mmz_mmz_t *pmmz = NULL;
	struct osal_list_head *p = NULL, *n = NULL;

	mmz_trace_func();

	list_for_each_safe(p, n, &mmz_list) {
		pmmz = list_entry(p, mmz_mmz_t, list);
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

	if (anony != 1) {
		printk("The module param \"anony\" should only be 1 which is %d\n",
		       anony);
		return -EINVAL;
	}

	if (strcmp(setup_allocator, "cma") == 0) {
		ret = cma_allocator_setopt(&the_allocator);
		allocator_type = 1;
	} else if (strcmp(setup_allocator, DEFAULT_ALLOCATOR) == 0) {
		ret = allocator_setopt(&the_allocator);
	} else {
		printk("The module param \"setup_allocator\" should be \"cma\" or \"" DEFAULT_ALLOCATOR
		       "\", which is \"%s\"\n",
		       setup_allocator);
		mmz_exit_check();
		return -EINVAL;
	}

	// ret = media_mem_parse_cmdline(setup_zones);
	ret = the_allocator.init(setup_zones);
	if (ret != 0) {
		mmz_exit_check();
		return ret;
	}

	media_mem_proc_init();

	mmz_userdev_init();

	map_mmz_init(mmap_zones);

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

/*
module_init(media_mem_init);
module_exit(media_mem_exit);

MODULE_LICENSE("GPL");
*/
#else

subsys_initcall(media_mem_init);

#endif
