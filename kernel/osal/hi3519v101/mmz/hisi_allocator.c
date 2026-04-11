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

#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/list.h>
#include <asm/cacheflush.h>
#include <linux/version.h>

//#include "media-mem.h"
#include "osal_mmz.h"
#include "allocator.h"

extern struct osal_list_head mmz_list;

extern int anony;
extern int mmb_number; /* for mmb id */

static unsigned long _strtoul_ex(const char *s, char **ep, unsigned int base)
{
        char *__end_p;
        unsigned long __value;

        __value = simple_strtoul(s,&__end_p,base);

        switch(*__end_p) {
        case 'm':
        case 'M':
                __value <<= 10;
        case 'k':
        case 'K':
                __value <<= 10;
                if(ep)
                        (*ep) = __end_p + 1;
        default:
                break;
        }

        return __value;
}

static unsigned long find_fixed_region(unsigned long *region_len,
		hil_mmz_t *mmz,
		unsigned long size,
		unsigned long align)
{
	unsigned long start;
	unsigned long fixed_start = 0;
	unsigned long fixed_len = -1;
	unsigned long len =0;
	unsigned long blank_len =0;
	hil_mmb_t *p = NULL;

	mmz_trace_func();
	align = mmz_grain_align(align);
	if(align == 0)align = MMZ_GRAIN;
	start = mmz_align2(mmz->phys_start, align);
	len = mmz_grain_align(size);

	osal_list_for_each_entry(p,&mmz->mmb_list, list) {
		hil_mmb_t *next;
		mmz_trace(4,"p->phys_addr=0x%08lX p->length = %luKB \t",
				p->phys_addr, p->length/SZ_1K);
		next = list_entry(p->list.next, __typeof__(*p), list);
		mmz_trace(4,",next = 0x%08lX\n\n", next->phys_addr);
		/*if p is the first entry or not*/
		if(osal_list_first_entry(&mmz->mmb_list, __typeof__(*p), list) == p) {
			blank_len = p->phys_addr - start;
			if((blank_len < fixed_len) && (blank_len>=len)) {
				fixed_len = blank_len;
				fixed_start = start;
				mmz_trace(4,"%d: fixed_region: start=0x%08lX, len=%luKB\n",
						__LINE__, fixed_start, fixed_len/SZ_1K);
			}
		}
		start = mmz_align2((p->phys_addr + p->length),align);
		
		/*if aglin is larger than mmz->nbytes, it would trigger the BUG_ON */
		//BUG_ON((start < mmz->phys_start) || (start > (mmz->phys_start + mmz->nbytes)));
		
		/*if we have to alloc after the last node*/
		if (osal_list_is_last(&p->list, &mmz->mmb_list)) {
			blank_len = mmz->phys_start + mmz->nbytes - start;
			if ((blank_len < fixed_len) && (blank_len >= len)){
				fixed_len = blank_len;
				fixed_start = start;
				mmz_trace(4,"%d: fixed_region: start=0x%08lX, len=%luKB\n",
						__LINE__, fixed_start, fixed_len/SZ_1K);
				break;
			} else {
				if(fixed_len != -1)
					goto out;
				fixed_start = 0;
				mmz_trace(4,"%d: fixed_region: start=0x%08lX, len=%luKB\n",
						__LINE__, fixed_start, fixed_len/SZ_1K);
				goto out;
			}
		}
		/* blank is too small */
		if ((start + len) > next->phys_addr) {
			mmz_trace(4,"start=0x%08lX ,len=%lu,next=0x%08lX\n",
					start,len,next->phys_addr);
			continue;
		}
		blank_len = next->phys_addr - start;
		if((blank_len < fixed_len) && (blank_len >= len)) {
			fixed_len = blank_len;
			fixed_start = start;
			mmz_trace(4,"%d: fixed_region: start=0x%08lX, len=%luKB\n",
					__LINE__, fixed_start, fixed_len/SZ_1K);
		}
	}

	if ((mmz_grain_align(start+len) <= (mmz->phys_start + mmz->nbytes))
			&& (start >= mmz->phys_start)
			&& (start < (mmz->phys_start + mmz->nbytes))) {
		fixed_len = len;
		fixed_start = start;
		mmz_trace(4,"%d: fixed_region: start=0x%08lX, len=%luKB\n",
				__LINE__, fixed_start, fixed_len/SZ_1K);
	} else {
		fixed_start = 0;
		mmz_trace(4,"%d: fixed_region: start=0x%08lX, len=%luKB\n",
				__LINE__, fixed_start, len/SZ_1K);
	}
out:
	*region_len = len;
	return fixed_start;
}

static unsigned long find_fixed_region_from_highaddr(unsigned long *region_len,
		hil_mmz_t *mmz,
		unsigned long size,
		unsigned long align)
{
	int i, j;
	unsigned long fixed_start=0;
	unsigned long fixed_len=~1;

	mmz_trace_func();

	i = mmz_length2grain(mmz->nbytes);

	for(; i>0; i--) {
		unsigned long start;
		unsigned long len;
		unsigned long start_highaddr;
		
		if(mmz_get_bit(mmz,i))
			continue;

		len = 0;
		start_highaddr = mmz_pos2phy_addr(mmz,i);
		for(; i>0; i--) {
			if(mmz_get_bit(mmz,i)) {
				break;
			}

			len += MMZ_GRAIN;
		}

		if(len>=size) {
			j = mmz_phy_addr2pos(mmz, mmz_align2low(start_highaddr-size, align));
			//align = mmz_grain_align(align)/MMZ_GRAIN;
			//start = mmz_pos2phy_addr(mmz, j - align);
			start = mmz_pos2phy_addr(mmz, j);
			if((start_highaddr - len <= start) && (start <= start_highaddr - size)){
				fixed_len = len;
				fixed_start = start;
				break;
			}				
						
			mmz_trace(1,"fixed_region: start=0x%08lX, len=%luKB",
					fixed_start, fixed_len/SZ_1K);
		}
	}

	*region_len = fixed_len;

	return fixed_start;
}

static int do_mmb_alloc(hil_mmb_t *mmb)
{
	hil_mmb_t *p=NULL;
	mmz_trace_func();

	/* add mmb sorted */
	osal_list_for_each_entry(p,&mmb->zone->mmb_list, list) {
		if(mmb->phys_addr < p->phys_addr)
			break;
		if(mmb->phys_addr == p->phys_addr){
			printk("ERROR: media-mem allocator bad in %s! (%s, %d)",
					mmb->zone->name,  __FUNCTION__, __LINE__);
			return -1;
		}
	}
	osal_list_add(&mmb->list, p->list.prev);

	mmz_trace(1,HIL_MMB_FMT_S,hil_mmb_fmt_arg(mmb));

	return 0;
}

static hil_mmb_t *__mmb_alloc(const char *name,
		unsigned long size,
		unsigned long align,
		unsigned long gfp,
		const char *mmz_name,
		hil_mmz_t *_user_mmz)
{
	hil_mmz_t *mmz;
	hil_mmb_t *mmb;

	unsigned long start;
	unsigned long region_len;

	unsigned long fixed_start=0;
	unsigned long fixed_len=~1;
	hil_mmz_t *fixed_mmz=NULL;

	mmz_trace_func();

	if(size == 0 || size > 0x40000000UL)
		return NULL;
	if(align == 0)
		align = MMZ_GRAIN;

	size = mmz_grain_align(size);

	mmz_trace(1,"size=%luKB, align=%lu", size/SZ_1K, align);

	begin_list_for_each_mmz(mmz, gfp, mmz_name)
		if(_user_mmz!=NULL && _user_mmz!=mmz)
			continue;

		start = find_fixed_region(&region_len, mmz, size, align);
		if( (fixed_len > region_len) && (start!=0)) {
			fixed_len = region_len;
			fixed_start = start;
			fixed_mmz = mmz;
		}
	end_list_for_each_mmz()

	if(fixed_mmz == NULL) {
		return NULL;
	}

	mmb = kmalloc(sizeof(hil_mmb_t), GFP_KERNEL);
	if (mmb == NULL){
		return NULL;
	}

	memset(mmb, 0, sizeof(hil_mmb_t));
	mmb->zone = fixed_mmz;
	mmb->phys_addr = fixed_start;
	mmb->length = size;
	mmb->id = ++mmb_number;
	if(name)
		strlcpy(mmb->name, name, HIL_MMB_NAME_LEN);
	else 
		strncpy(mmb->name, "<null>", HIL_MMB_NAME_LEN);

	if(do_mmb_alloc(mmb)) {
		kfree(mmb);
		mmb = NULL;
	}

	return mmb;
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

	unsigned long start = 0;
	unsigned long region_len = 0;

	unsigned long fixed_start=0;
	unsigned long fixed_len=~1;
	hil_mmz_t *fixed_mmz=NULL;

	mmz_trace_func();

	if(size == 0 || size > 0x40000000UL)
		return NULL;
	if(align == 0)
		align = 1;
		
	size = mmz_grain_align(size);

	mmz_trace(1,"size=%luKB, align=%lu", size/SZ_1K, align);

	begin_list_for_each_mmz(mmz, gfp, mmz_name)
		if(_user_mmz!=NULL && _user_mmz!=mmz)
			continue;
			
		if(mmz->alloc_type == SLAB_ALLOC){
			if((size-1) & size){
				for(i = 1; i <= 32; i++){
					if(!((size >> i) & ~0)){
						size = 1 << i;	
						break;
					}						
				}	
			}
		}
		else if(mmz->alloc_type == EQ_BLOCK_ALLOC){
			size = mmz_align2(size,mmz->block_align);
		}
		if(order == LOW_TO_HIGH){
			start = find_fixed_region(&region_len, mmz, size, align);
		}
		else if(order == HIGH_TO_LOW)
			start = find_fixed_region_from_highaddr(&region_len, mmz, size, align);
		if( (fixed_len > region_len) && (start!=0)) {
			fixed_len = region_len;
			fixed_start = start;
			fixed_mmz = mmz;
		}
	end_list_for_each_mmz()

	if(fixed_mmz == NULL) {
		return NULL;
	}

	mmb = kmalloc(sizeof(hil_mmb_t), GFP_KERNEL);
	if (mmb == NULL) {
	    return NULL;
	}

	memset(mmb, 0, sizeof(hil_mmb_t));
	mmb->zone = fixed_mmz;
	mmb->phys_addr = fixed_start;
	mmb->length = size;
	mmb->order = order;
	if(name)
		strlcpy(mmb->name, name, HIL_MMB_NAME_LEN);
	else 
		strncpy(mmb->name, "<null>", HIL_MMB_NAME_LEN);

	if(do_mmb_alloc(mmb)) {
		kfree(mmb);
		mmb = NULL;
	}

	return mmb;
}

static void *__mmb_map2kern(hil_mmb_t *mmb, int cached)
{
	/*
	 * already mapped? no need to remap again,
	 * just return mmb's kernel virtual address.
	 */
	if (mmb->flags & HIL_MMB_MAP2KERN) {
		if ((cached * HIL_MMB_MAP2KERN_CACHED)
				!= (mmb->flags & HIL_MMB_MAP2KERN_CACHED)) {
			printk(KERN_ERR "mmb<%s> has been kernel-mapped as %s,\
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
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,18,10)
		mmb->kvirt = ioremap_cached(mmb->phys_addr, mmb->length);
#else
		mmb->kvirt = ioremap_cache(mmb->phys_addr, mmb->length);
#endif
	} else {
		mmb->flags &= ~HIL_MMB_MAP2KERN_CACHED;
		/* ioremap_wc has better performance */ 
		mmb->kvirt = ioremap_wc(mmb->phys_addr, mmb->length);
	}

	if(mmb->kvirt) {
	       	mmb->flags |= HIL_MMB_MAP2KERN;
		mmb->map_ref++;
	}

	return mmb->kvirt;
}

static void __mmb_free(hil_mmb_t *mmb)
{
	if (mmb->flags & HIL_MMB_MAP2KERN_CACHED) {
		__cpuc_flush_dcache_area((void *)mmb->kvirt, (size_t)mmb->length);
		outer_flush_range(mmb->phys_addr, mmb->phys_addr + mmb->length);
	}

	osal_list_del(&mmb->list);
	kfree(mmb);
}

static int __mmb_unmap(hil_mmb_t *mmb)
{
	int ref;
	
	if (mmb->flags & HIL_MMB_MAP2KERN_CACHED) {
		__cpuc_flush_dcache_area((void *)mmb->kvirt, (size_t)mmb->length);
		outer_flush_range(mmb->phys_addr, mmb->phys_addr + mmb->length);
	}

	if(mmb->flags & HIL_MMB_MAP2KERN) {
		ref = --mmb->map_ref;
		if(mmb->map_ref !=0) {
			return ref;
		}
		iounmap(mmb->kvirt);
	}

	mmb->kvirt = NULL;
	mmb->flags &= ~HIL_MMB_MAP2KERN;
	mmb->flags &= ~HIL_MMB_MAP2KERN_CACHED;

	if((mmb->flags & HIL_MMB_RELEASED) && mmb->phy_ref ==0) {
		__mmb_free(mmb);
	}

	return 0;
}

static void *__mmf_map(unsigned long phys, int len, int cache)
{
	void *virt;
	if (cache) {
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,18,10)
		virt = ioremap_cached(phys, len);
#else
		virt = ioremap_cache(phys, len);
#endif
	} else {
		virt = ioremap_wc(phys, len);
	}

	return virt;
}

static void __mmf_unmap(void *virt)
{
	if (virt)
	    iounmap(virt);
}

static int __allocator_init(char *s)
{
	hil_mmz_t *zone = NULL;
	char *line;

	while ((line = strsep(&s, ":")) != NULL) {
		int i;
		char *argv[6];

		for (i = 0; (argv[i] = strsep(&line, ",")) != NULL;)
			if (++i == ARRAY_SIZE(argv))
				break;

		if (i == 4) {
			zone = hil_mmz_create("null",0,0,0);
			if (NULL == zone)
				continue;
			strlcpy(zone->name, argv[0], HIL_MMZ_NAME_LEN);
			zone->gfp = _strtoul_ex(argv[1], NULL, 0);
			zone->phys_start = _strtoul_ex(argv[2], NULL, 0);
			zone->nbytes = _strtoul_ex(argv[3], NULL, 0);
		} else if (i == 6) {
			zone = hil_mmz_create_v2("null",0,0,0,0,0);
			if (zone == NULL)
				continue;

			strlcpy(zone->name, argv[0], HIL_MMZ_NAME_LEN);
			zone->gfp = _strtoul_ex(argv[1], NULL, 0);
			zone->phys_start = _strtoul_ex(argv[2], NULL, 0);
			zone->nbytes = _strtoul_ex(argv[3], NULL, 0);
			zone->alloc_type = _strtoul_ex(argv[4], NULL, 0);
			zone->block_align = _strtoul_ex(argv[5], NULL, 0);
		} else {
			printk(KERN_ERR "error parameters\n");
			return -EINVAL;
		}

		//mmz_info_phys_start = zone->phys_start + zone->nbytes - 0x2000;

		if (hil_mmz_register(zone)) {
			printk(KERN_WARNING "Add MMZ failed: " HIL_MMZ_FMT_S "\n", hil_mmz_fmt_arg(zone));
			hil_mmz_destroy(zone);
		}
		zone = NULL;
	}

	return 0;
}

int hisi_allocator_setopt(struct mmz_allocator *allocator)
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

