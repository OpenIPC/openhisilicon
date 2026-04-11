
#ifndef __KCOM_MEDIA_MEM_H__
#define __KCOM_MEDIA_MEM_H__

#define UUID_MEDIA_MEMORY "media-mem-1.0.0.0"
#define UUID_MEDIA_MEMORY_V2 "media-mem-2.0.0.0"

typedef void kcom_mmz_t;
typedef unsigned long mmb_addr_t;

#define MMB_ADDR_INVALID (~0)


mmb_addr_t 	new_mmb(const char *name, int size, unsigned long align, const char *zone_name);

void 		delete_mmb(mmb_addr_t addr);

void *		remap_mmb(mmb_addr_t addr);

void              *remap_mmb_cached(mmb_addr_t addr);

void             flush_mmb_byaddr(void *kvirt, unsigned long phys_addr, unsigned long length);

int 		unmap_mmb(void *mapped_addr);



// BEGIN: Modified by x00132463, 2011-3-22 16 : 31 : 45 
#if 0
struct kcom_media_memory {
	struct kcom_object kcom;

	kcom_mmz_t *	(*new_zone)(const char *name, unsigned long phys_start, unsigned long size);
	
	void 		(*delete_zone)(kcom_mmz_t *zone);

	mmb_addr_t 	(*new_mmb)(const char *name, int size, unsigned long align, const char *zone_name);
	
	
	void 		(*delete_mmb)(mmb_addr_t addr);

	void *		(*remap_mmb)(mmb_addr_t addr);
	void *		(*remap_mmb_cached)(mmb_addr_t addr);
	void *		(*refer_mapped_mmb)(void *mapped_addr);
	int 		(*unmap_mmb)(void *mapped_addr);

	int		(*get_mmb)(mmb_addr_t addr);
	int		(*put_mmb)(mmb_addr_t addr);
};

struct kcom_media_memory_v2 {
	struct kcom_object kcom;

	kcom_mmz_t *	(*new_zone)(const char *name, unsigned long phys_start, unsigned long size, unsigned int alloc_type, unsigned long block_align);
	
	void 		(*delete_zone)(kcom_mmz_t *zone);

	mmb_addr_t 	(*new_mmb)(const char *name, int size, unsigned long align, const char *zone_name, unsigned int order);
	
	
	void 		(*delete_mmb)(mmb_addr_t addr);

	void *		(*remap_mmb)(mmb_addr_t addr);
	void *		(*remap_mmb_cached)(mmb_addr_t addr);
	void *		(*refer_mapped_mmb)(void *mapped_addr);
	int 		(*unmap_mmb)(void *mapped_addr);

	int		(*get_mmb)(mmb_addr_t addr);
	int		(*put_mmb)(mmb_addr_t addr);
};
#endif

#if 0
#define kcom_to_mmz(__kcom) kcom_to_instance(__kcom, struct kcom_media_memory, kcom)
#define kcom_to_mmz_v2(__kcom) kcom_to_instance(__kcom, struct kcom_media_memory_v2, kcom)
#endif

#endif

