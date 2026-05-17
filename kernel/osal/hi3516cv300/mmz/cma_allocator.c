#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/list.h>
#include <asm/cacheflush.h>

#include <asm/memory.h>
/* dma-contiguous.h merged into dma-map-ops.h in 5.16; kernel_compat.h
 * (force-included via -include) picks the right header per version. */
#include <linux/dma-mapping.h>
#include <asm/memory.h>
#include <asm/highmem.h>
#include <asm/tlbflush.h>
#include <asm/pgtable.h>
#include <linux/device.h>
#include <linux/vmalloc.h>

//#include "media-mem.h"
#include "osal_mmz.h"
#include "allocator.h"


struct cma_zone{
	struct device pdev;
	char name[NAME_LEN_MAX];
	unsigned long gfp;
	unsigned long phys_start;
	unsigned long nbytes;
	unsigned int alloc_type;
	unsigned long block_align;
};

extern struct osal_list_head mmz_list;

extern int anony;
extern int mmb_number; /* for mmb id */

#ifdef CONFIG_CMA
#if 1
/* apply_to_page_range() callback signature: 5.0+ dropped the pgtable_t
 * token middle arg. Define two flavours of __dma_update_pte. */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)
static int __dma_update_pte(pte_t *pte, unsigned long addr, void *data)
{
	struct page *page = virt_to_page(addr);
	pgprot_t prot = *(pgprot_t *)data;

	set_pte_ext(pte, mk_pte(page, prot), 0);
	return 0;
}
#else
static int __dma_update_pte(pte_t *pte, pgtable_t token,
		unsigned long addr, void *data)
{
	struct page *page = virt_to_page(addr);
	pgprot_t prot = *(pgprot_t *)data;

	set_pte_ext(pte, mk_pte(page, prot), 0);
	return 0;
}
#endif
#endif

/* Vendor BSP exported __dma_clear_buffer as a kernel symbol; on mainline
 * it lives inside arch/arm/mm/dma-mapping.c with no module export.
 * On modern kernels (>=5.0) CMA pages from dma_alloc_from_contiguous come
 * with the right cache state — the manual flush is unnecessary and the
 * underlying dmac_flush_range maps to arm926_dma_flush_range which isn't
 * EXPORT_SYMBOL'd to modules. No-op on modern kernels. */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)
static inline void __dma_clear_buffer(struct page *page, size_t size) { (void)page; (void)size; }
#else
static void __dma_clear_buffer(struct page *page, size_t size)
{
	void *ptr;
	/*
	 * Ensure that the allocated pages are zeroed, and that any data
	 * lurking in the kernel direct-mapped region is invalidated.
	 */
	ptr = page_address(page);

	if (ptr) {
		/* remove memset for speed
		memset(ptr, 0, size);
		*/
		
		if (size <= SZ_1M)
		{
			dmac_flush_range(ptr, ptr + size);
			outer_flush_range(__pa(ptr), __pa(ptr) + size);
		}
		else
		{
			flush_cache_all();
			outer_flush_all();
		}
	}
}
#endif

/* Vendor BSP had hisi_flush_tlb_kernel_range + init_mm export; mainline
 * provides flush_tlb_kernel_range with the same semantics and init_mm is
 * not exported to modules at all. Skip the manual pte_update on modern
 * kernels — CMA pages from dma_alloc_from_contiguous are already mapped
 * with the right page protections by the core. */
#if 1
static void __dma_remap(struct page *page, size_t size, pgprot_t prot)
{
#if defined(CONFIG_CMA) && LINUX_VERSION_CODE < KERNEL_VERSION(5, 0, 0)
	unsigned long start = (unsigned long) page_address(page);
	unsigned end = start + size;

	apply_to_page_range(&init_mm, start, size, __dma_update_pte, &prot);
	dsb();
	hisi_flush_tlb_kernel_range(start, end);
#endif
}
#endif
#endif


static int do_mmb_alloc(hil_mmb_t *mmb)
{
	hil_mmb_t *p = NULL;
	mmz_trace_func();

	/* add mmb sorted */
	osal_list_for_each_entry(p,&mmb->zone->mmb_list, list) {
		if (mmb->phys_addr < p->phys_addr)
			break;

        //BUG_ON(mmb->phys_addr != p->phys_addr);
        if (mmb->phys_addr == p->phys_addr)
        {
	        printk("ERROR: media-mem allocator bad in %s! (%s, %d)",
                mmb->zone->name,  __FUNCTION__, __LINE__);

			return -1;
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
#ifdef CONFIG_CMA
	hil_mmz_t *mmz;
	hil_mmb_t *mmb;

	unsigned long order;
	size_t count;
	struct page *page = NULL;

	unsigned long fixed_start = 0;

	hil_mmz_t *fixed_mmz = NULL;

	mmz_trace_func();

	/*
	 * no more than 1GB
	 */
	if (size == 0 || size > 0x40000000UL)
		return NULL;

	if (align == 0)
		align = MMZ_GRAIN;

	size = mmz_grain_align(size);
	order = get_order(size);
	count = size>>PAGE_SHIFT;

	mmz_trace(1,"size=%luKB, align=%lu", size/SZ_1K, align);

	begin_list_for_each_mmz(mmz, gfp, mmz_name)

		if ((_user_mmz != NULL) && (_user_mmz != mmz))
			continue;

		page = compat_dma_alloc_from_contiguous(mmz->cma_dev, count, order);
		if (!page)
			break;
		fixed_mmz = mmz;
		fixed_start = page_to_phys(page);
		break;

	end_list_for_each_mmz()

	if (fixed_mmz == NULL)
		return NULL;
	if (page)
		__dma_clear_buffer(page, size);

	mmb = kmalloc(sizeof(hil_mmb_t), GFP_KERNEL);
	if (mmb == NULL)
		goto cma_free;
	memset(mmb, 0, sizeof(hil_mmb_t));

	mmb->zone = fixed_mmz;
	mmb->phys_addr = fixed_start;
	mmb->length = size;
	mmb->id = ++mmb_number;

	if (name)
		strlcpy(mmb->name, name, HIL_MMB_NAME_LEN);
	else
		strncpy(mmb->name, "<null>", HIL_MMB_NAME_LEN - 1);

	if (do_mmb_alloc(mmb))
		goto mmb_free;

	return mmb;

mmb_free:
	kfree(mmb);
cma_free:
	dma_release_from_contiguous(mmz->cma_dev, page, count);
#endif
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
	hil_mmz_t *mmz;
	hil_mmb_t *mmb;
	unsigned int i;

	unsigned long cma_order;
	size_t count = size >> PAGE_SHIFT;
	struct page *page = NULL;

	unsigned long fixed_start = 0;

	hil_mmz_t *fixed_mmz=NULL;

	mmz_trace_func();

	if ((size == 0) || (size > 0x40000000UL))
		return NULL;

	if (align == 0)
		align = 1;

	size = mmz_grain_align(size);

	mmz_trace(1,"size=%luKB, align=%lu", size/SZ_1K, align);

	begin_list_for_each_mmz(mmz, gfp, mmz_name)
		if ((_user_mmz != NULL) && (_user_mmz != mmz))
			continue;

		if (mmz->alloc_type == SLAB_ALLOC) {
			if ((size-1) & size) {
				for (i = 1; i <= 32; i++) {
					if (!((size >> i) & ~0)) {
						size = 1 << i;	
						break;
					}						
				}	
			}
		} else if (mmz->alloc_type == EQ_BLOCK_ALLOC) {
			size = mmz_align2(size,mmz->block_align);
		}

		cma_order = get_order(size);		     	

		page = compat_dma_alloc_from_contiguous(mmz->cma_dev, count, cma_order);
		if (!page)
			return NULL;

		fixed_mmz = mmz;
		fixed_start = page_to_phys(page);
		break;
	end_list_for_each_mmz()

	if (fixed_mmz == NULL)
		return NULL;

	mmb = kmalloc(sizeof(hil_mmb_t), GFP_KERNEL);
	
	//mark by liucan, need fix.
	if (mmb == NULL)
		goto cma_free;

	memset(mmb, 0, sizeof(hil_mmb_t));

	mmb->zone = fixed_mmz;
	mmb->phys_addr = fixed_start;
	mmb->length = size;
	mmb->order = order;

	if (name)
		strlcpy(mmb->name, name, HIL_MMB_NAME_LEN);
	else
		strncpy(mmb->name, "<null>", HIL_MMB_NAME_LEN - 1);

	if (do_mmb_alloc(mmb))
		goto mmb_free;

	return mmb;

mmb_free:
    if(mmb)
    {
	    kfree(mmb);
    }
cma_free:
    if(page)
    {
	    dma_release_from_contiguous(mmz->cma_dev, page, count);
    }

	return NULL;
}

static void __mmb_free(hil_mmb_t *mmb)
{
	size_t count = mmb->length >> PAGE_SHIFT;
	struct page *page = phys_to_page(mmb->phys_addr);

	hil_mmz_t *mmz = mmb->zone;

	if (mmb->flags & HIL_MMB_MAP2KERN_CACHED) {
		__cpuc_flush_dcache_area((void *)mmb->kvirt, (size_t)mmb->length);
		outer_flush_range(mmb->phys_addr, mmb->phys_addr + mmb->length);
	}

	dma_release_from_contiguous(mmz->cma_dev, page, count);

	osal_list_del(&mmb->list);
	kfree(mmb);
}

static int __mmb_unmap(hil_mmb_t* mmb);
#define MAX_MALLOC_PAGENR  400

static void *__mmb_map2kern(hil_mmb_t *mmb, int cached)
{
#ifdef CONFIG_CMA
	pgprot_t prot;
	struct page* page = phys_to_page(mmb->phys_addr);

	if (mmb->flags & HIL_MMB_MAP2KERN) {
		/*
		 * if already mapped to kernel address space.
		 */
		if((cached * HIL_MMB_MAP2KERN_CACHED)
				!= (mmb->flags & HIL_MMB_MAP2KERN_CACHED)) {
			printk(KERN_ERR "mmb<%s> has been kernel-mapped %s, \
					can not be re-mapped as %s.",
					mmb->name,
					(mmb->flags&HIL_MMB_MAP2KERN_CACHED) ? "cached" : "non-cached",
					(cached) ? "cached" : "non-cached" );
			return NULL;
		}
		mmb->map_ref++;
		return mmb->kvirt;
	}

	if (cached) {
		mmb->flags |= HIL_MMB_MAP2KERN_CACHED;
		prot = pgprot_kernel;
	} else {
		mmb->flags &= ~HIL_MMB_MAP2KERN_CACHED;
		prot = pgprot_noncached(pgprot_kernel);
	}

	if (PageHighMem(page)) {
		int i;
		struct page **pages;
		unsigned int pagesnr = mmb->length / PAGE_SIZE;
		struct page *tmp = page;
		int array_size = sizeof(struct page *) * pagesnr;
		struct vm_struct *area;

		/*
		 * Noted: mmb->length would be very large in some cases(for example:
		 * more than one Giga Bytes). and array_size would be very large as
		 * well. So, don't use kmalloc here.
		 */
		pages = vmalloc(array_size);
		if (pages == NULL) {
			printk(KERN_ERR "ptr array(0x%x) vmalloc failed.\n", array_size);
			return NULL;
		}

		for (i = 0; i < pagesnr; i++) {
			*(pages+i) = tmp;
			tmp++;
		}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 8, 0)
		/* __get_vm_area + map_vm_area were removed; vmap() does both. */
		{
			void *vaddr = vmap(pages, pagesnr, VM_MAP, prot);
			if (!vaddr) {
				printk(KERN_ERR "vmap to mmz pages failed!\n");
				vfree(pages);
				return NULL;
			}
			mmb->kvirt = vaddr;
		}
#else
		area = __get_vm_area((pagesnr << PAGE_SHIFT), VM_MAP,
				VMALLOC_START, VMALLOC_END);
		if (!area) {
			printk(KERN_ERR "get vm area from high failed!\n");
			vfree(pages);
			return NULL;
		}

		if (map_vm_area(area, prot, pages)) {
			printk(KERN_ERR "map vm area to mmz pages failed!\n");
			vunmap(area->addr);
			vfree(pages);
			return NULL;
		}
		mmb->kvirt = area->addr;
#endif
		//mmb->kvirt = vmap(pages, pagesnr, VM_MAP, prot);

		vfree(pages);
	} else {
		__dma_remap(page, mmb->length, prot);
		mmb->kvirt = __va(mmb->phys_addr);
	}

	if (NULL == mmb->kvirt) {
		/*
		 * you never get here.
		 */
		printk(KERN_ERR "mmb[0x%lx, 0x%lx] map to kernel failed\n",
				mmb->phys_addr, mmb->length);
		return NULL;
	}

	mmb->flags |= HIL_MMB_MAP2KERN;
	mmb->map_ref++;

	return mmb->kvirt;
    #else
    return NULL;
    #endif
}

static int __mmb_unmap(hil_mmb_t *mmb)
{
	int ref;
	struct page* page = phys_to_page(mmb->phys_addr);

	if (mmb->flags & HIL_MMB_MAP2KERN_CACHED) {
		__cpuc_flush_dcache_area((void *)mmb->kvirt, (size_t)mmb->length);
		outer_flush_range(mmb->phys_addr, mmb->phys_addr + mmb->length);
	}

	if (mmb->flags & HIL_MMB_MAP2KERN) {
		ref = --mmb->map_ref;
		if (mmb->map_ref != 0)
			return ref;

		//__dma_remap(page, mmb->length, pgprot_kernel);
		// iounmap(mmb->kvirt);
	}

	if (PageHighMem(page))
		vunmap(mmb->kvirt);
	mmb->kvirt = NULL;
	mmb->flags &= ~HIL_MMB_MAP2KERN;
	mmb->flags &= ~HIL_MMB_MAP2KERN_CACHED;

	if ((mmb->flags & HIL_MMB_RELEASED) && (mmb->phy_ref == 0))
		__mmb_free(mmb);

	return 0;
}

/*
 * this function is added for some extra-requirements from
 * some customers, and the implementation is not strictly
 * in accordance with our early design idea, logically. FIXME
 */
static void *__mmf_map(unsigned long phys, int len, int cache)
{
#ifdef CONFIG_CMA
	struct page **pages;
	unsigned int pagesnr = len / PAGE_SIZE;
	int i;
	void *virt;
	pgprot_t prot;
	struct page* page = phys_to_page(phys);	
	struct page *tmp = page;
	int array_size = sizeof(struct page *) * pagesnr;

	if (cache) {
		prot = pgprot_kernel;
	} else {
		prot = pgprot_noncached(pgprot_kernel);
	}

	if (PageHighMem(page)) {
		struct vm_struct *area;
		/*
		 * Noted: length of region may be very large in some cases(for example:
		 * more than one Giga Bytes). and array_size would be very large as
		 * well. So, don't use kmalloc here.
		 */
		pages = vmalloc(array_size);
		if (pages == NULL)	{
			printk(KERN_ERR "ptr array(0x%x) vmalloc failed.\n", array_size);
			return NULL;
		}

		for (i = 0; i < pagesnr; i++) {
			*(pages+i) = tmp;
			tmp++;
		}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 8, 0)
		{
			void *vaddr = vmap(pages, pagesnr, VM_MAP, prot);
			if (!vaddr) {
				printk(KERN_ERR "vmap to mmz pages failed!\n");
				vfree(pages);
				return NULL;
			}
			virt = vaddr;
		}
#else
		area = __get_vm_area((pagesnr << PAGE_SHIFT), VM_MAP,
				VMALLOC_START, VMALLOC_END);
		if (!area) {
			printk(KERN_ERR "get vm area from high failed!\n");
			vfree(pages);
			return NULL;
		}

		if (map_vm_area(area, prot, pages)) {
			printk(KERN_ERR "map vm area to mmz pages failed!\n");
			vunmap(area->addr);
			vfree(pages);
			return NULL;
		}
		virt = area->addr;
#endif

		//virt = vmap(pages, pagesnr, VM_MAP, prot);
		vfree(pages);
	} else {
		__dma_remap(page, len, prot);
		virt = __va(phys);
	}

	return virt;
    #else
    return NULL;
    #endif
}

/*
 * this function is added for some extra-requirements from
 * some customers, and the implementation is not strictly
 * in accordance with our early design idea, logically. FIXME
 */
static void __mmf_unmap(void *virt)
{
	unsigned long vaddr = (unsigned long)virt;
	if ((vaddr >= VMALLOC_START) && (vaddr < VMALLOC_END))
		vunmap(virt);
}

static int __allocator_init(char *s)
{
    #ifdef CONFIG_CMA
	hil_mmz_t *zone = NULL;
	char *line;
	struct cma_zone *cma_zone = NULL;
	int attempted = 0, registered = 0;

	while ((line = strsep(&s, ":")) != NULL) {
		int i;
		char *argv[6];
		/* Vendor BSP shipped hisi_get_cma_zone in mach-hi3516cv300; mainline
		 * has no equivalent (no per-zone CMA naming concept). Stub to NULL
		 * → the caller logs "can't get cma zone info" and skips. Multiple
		 * named CMA zones not exercised on neo boot+login path. */
		struct cma_zone *hisi_get_cma_zone(const char *name) { (void)name; return NULL; }
		/*
		 * FIXME: We got 4 args in "line", formated "argv[0],argv[1],argv[2],argv[3],argv[4]".
		 * eg: "<mmz_name>,<gfp>,<phys_start_addr>,<size>,<alloc_type>"
		 * For more convenient, "hard code" are used such as "arg[0]", i.e.
		 */
		for (i=0; (argv[i] = strsep(&line, ",")) != NULL;)
			if (++i == ARRAY_SIZE(argv)) break;

		cma_zone = hisi_get_cma_zone(argv[0]);
		if (!cma_zone) {
			printk(KERN_ERR"can't get cma zone info:%s\n",argv[0]);
			continue;
		}

		if (i == 4) {
			zone = hil_mmz_create("null",0,0,0);
			if (zone == NULL) {
				continue;
			}

			strlcpy(zone->name, argv[0], HIL_MMZ_NAME_LEN);

			printk("cmz zone gfp 0x%lx, phys 0x%lx, nbytes 0x%lx\n",
					cma_zone->gfp,
					cma_zone->phys_start,
					cma_zone->nbytes);
			zone->gfp = cma_zone->gfp;
			zone->phys_start = cma_zone->phys_start;
			zone->nbytes = cma_zone->nbytes;
			zone->cma_dev = &cma_zone->pdev;
		} else if (i == 6) {
			zone = hil_mmz_create_v2("null",0,0,0,0,0);

			if (zone == NULL) {
				continue;
			}

			strlcpy(zone->name, argv[0], HIL_MMZ_NAME_LEN);

			zone->gfp = cma_zone->gfp;
			zone->phys_start = cma_zone->phys_start;
			zone->nbytes = cma_zone->nbytes;
			zone->alloc_type = cma_zone->alloc_type;
			zone->block_align = cma_zone->block_align;
			zone->cma_dev = &cma_zone->pdev;
		} else {
			printk(KERN_ERR "MMZ: your parameter num is not correct!\n");
			continue;
		}

		attempted++;
		if (hil_mmz_register(zone)) {
			printk(KERN_WARNING "Add MMZ failed: " HIL_MMZ_FMT_S "\n", hil_mmz_fmt_arg(zone));
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

