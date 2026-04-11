/* linux/include/asm-arm/arch-hi3510_v100_m01/media-mem.h
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



#ifndef __ASM_ARCH_MEDIA_MEM_H
#define __ASM_ARCH_MEDIA_MEM_H

#include <linux/seq_file.h>

#define HIL_MMZ_NAME_LEN 32
#define	CACHE_LINE_SIZE (0x20)

struct hil_media_memory_zone {
	char name[HIL_MMZ_NAME_LEN];

	unsigned long gfp;

	unsigned long phys_start;
	unsigned long nbytes;

	struct list_head list;

	unsigned char *bitmap;
	struct list_head mmb_list;
	
	unsigned int alloc_type;
	unsigned long block_align;

	void (*destructor)(const void *);
};
typedef struct hil_media_memory_zone hil_mmz_t;

#define HIL_MMZ_FMT_S "PHYS(0x%08lX, 0x%08lX), GFP=%lu, nBYTES=%luKB,	NAME=\"%s\""
#define hil_mmz_fmt_arg(p) (p)->phys_start,(p)->phys_start+(p)->nbytes-1,(p)->gfp,(p)->nbytes/SZ_1K,(p)->name

#define HIL_MMB_NAME_LEN 16
struct hil_media_memory_block {
	#ifndef MMZ_V2_SUPPORT
	unsigned int id;
	#endif
	char name[HIL_MMB_NAME_LEN];
	struct hil_media_memory_zone *zone;
	struct list_head list;

	unsigned long phys_addr;
	void *kvirt;
	unsigned long length;

	unsigned long flags;
	
	unsigned int order;
	
	int phy_ref;
	int map_ref;
};
typedef struct hil_media_memory_block hil_mmb_t;

#define hil_mmb_kvirt(p)	({hil_mmb_t *__mmb=(p); BUG_ON(__mmb==NULL); __mmb->kvirt;})
#define hil_mmb_phys(p)		({hil_mmb_t *__mmb=(p); BUG_ON(__mmb==NULL); __mmb->phys_addr;})
#define hil_mmb_length(p)	({hil_mmb_t *__mmb=(p); BUG_ON(__mmb==NULL); __mmb->length;})
#define hil_mmb_name(p)		({hil_mmb_t *__mmb=(p); BUG_ON(__mmb==NULL); __mmb->name;})
#define hil_mmb_zone(p)		({hil_mmb_t *__mmb=(p); BUG_ON(__mmb==NULL); __mmb->zone;})

#define HIL_MMB_MAP2KERN	(1<<0)
#define HIL_MMB_MAP2KERN_CACHED	(1<<1)
#define HIL_MMB_RELEASED	(1<<2)

#ifdef MMZ_V2_SUPPORT
#define HIL_MMB_FMT_S "phys(0x%08lX, 0x%08lX), kvirt=0x%p, flags=0x%08lX, length=%luKB,    name=\"%s\""
#define hil_mmb_fmt_arg(p) (p)->phys_addr,mmz_grain_align((p)->phys_addr+(p)->length)-1,(p)->kvirt,(p)->flags,(p)->length/SZ_1K,(p)->name
#else
#define HIL_MMB_FMT_S "phys(0x%08lX, 0x%08lX), kvirt=0x%p, flags=0x%08lX, length=%luKB,	name=\"%s\""
#define hil_mmb_fmt_arg(p) (p)->phys_addr,mmz_grain_align((p)->phys_addr+(p)->length)-1,(p)->kvirt,(p)->flags,(p)->length/SZ_1K,(p)->name
#endif

#define DEFAULT_ALLOC 0
#define SLAB_ALLOC 1
#define EQ_BLOCK_ALLOC 2

#define LOW_TO_HIGH 0
#define HIGH_TO_LOW 1

/*
 * APIs
 */

extern hil_mmz_t *hil_mmz_create(const char *name, unsigned long gfp, unsigned long phys_start, 
			unsigned long nbytes);
extern hil_mmz_t *hil_mmz_create_v2(const char *name, unsigned long gfp, unsigned long phys_start, 
			unsigned long nbytes,  unsigned int alloc_type, unsigned long block_align);	
					
extern int hil_mmz_destroy(hil_mmz_t *zone);

extern int hil_mmz_register(hil_mmz_t *zone);
extern int hil_mmz_unregister(hil_mmz_t *zone);
extern hil_mmz_t *hil_mmz_find(unsigned long gfp, const char *mmz_name);

extern hil_mmb_t *hil_mmb_alloc(const char *name, unsigned long size, unsigned long align, 
		unsigned long gfp, const char *mmz_name);
extern hil_mmb_t *hil_mmb_alloc_v2(const char *name, unsigned long size, unsigned long align, 
		unsigned long gfp, const char *mmz_name, unsigned int order);		
		
extern int hil_mmb_free(hil_mmb_t *mmb);
extern hil_mmb_t *hil_mmb_getby_phys(unsigned long addr);
extern hil_mmb_t *hil_mmb_getby_phys_2(unsigned long addr, unsigned long *Outoffset);
extern hil_mmb_t *hil_mmb_getby_kvirt(void *virt);

extern hil_mmb_t *hil_mmb_alloc_in(const char *name, unsigned long size, unsigned long align, 
		hil_mmz_t *_user_mmz);
extern hil_mmb_t *hil_mmb_alloc_in_v2(const char *name, unsigned long size, unsigned long align, 
		hil_mmz_t *_user_mmz, unsigned int order);		
		

#define hil_mmb_freeby_phys(phys_addr) hil_mmb_free(hil_mmb_getby_phys(phys_addr))
#define hil_mmb_freeby_kvirt(kvirt) hil_mmb_free(hil_mmb_getby_kvirt(kvirt))

extern void* hil_mmb_map2kern(hil_mmb_t *mmb);
extern void* hil_mmb_map2kern_cached(hil_mmb_t *mmb);
extern int hil_mmb_flush_dcache_byaddr(void *kvirt, unsigned long phys_addr, unsigned long length);

extern int hil_mmb_unmap(hil_mmb_t *mmb);
extern int hil_mmb_get(hil_mmb_t *mmb);
extern int hil_mmb_put(hil_mmb_t *mmb);

#ifdef MMZ_V2_SUPPORT
extern int hil_mmb_force_put(hil_mmb_t *mmb);
#endif

/* helper micros */

extern unsigned long hil_strtoul_ex(const char *s, char **ep, unsigned int base);

/* for mmz userdev */
int mmz_userdev_init(void);
void mmz_userdev_exit(void);
int mmz_flush_dcache_all(void);
extern int mmz_read_proc(struct seq_file *sfile);

#endif

