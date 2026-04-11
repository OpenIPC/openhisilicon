
#ifndef __MMZ_USERDEV_H
#define __MMZ_USERDEV_H

#define	CACHE_LINE_SIZE  (0x40)
#define HIL_MMZ_NAME_LEN 32
#define HIL_MMB_NAME_LEN 16

struct mmb_info {
	unsigned long phys_addr;	/* phys-memory address */
	unsigned long align;		/* if you need your phys-memory have special align size */
	unsigned long size;		/* length of memory you need, in bytes */
	unsigned int order;

	void *mapped;			/* userspace mapped ptr */

	union {
		struct {
			unsigned long prot  :8;	/* PROT_READ or PROT_WRITE */
			unsigned long flags :12;/* MAP_SHARED or MAP_PRIVATE */

#ifdef __KERNEL__
			unsigned long reserved :8; /* reserved, do not use */
			unsigned long delayed_free :1; 
			unsigned long map_cached :1; 
#endif
		};
		unsigned long w32_stuf;
	};

	char mmb_name[HIL_MMB_NAME_LEN];
	char mmz_name[HIL_MMZ_NAME_LEN];
	unsigned long gfp;		/* reserved, do set to 0 */

#ifdef __KERNEL__
	int map_ref;
	int mmb_ref;

	struct list_head list;
	hil_mmb_t *mmb;
#endif
};

struct dirty_area {
	unsigned long dirty_phys_start;	/* dirty physical address */
	unsigned long dirty_virt_start; /* dirty virtual  address,
					   must be coherent with dirty_phys_addr */
	unsigned long dirty_size;
};
#define IOC_MMB_ALLOC		_IOWR('m', 10,  struct mmb_info)
#define IOC_MMB_ATTR		_IOR('m',  11,  struct mmb_info)
#define IOC_MMB_FREE		_IOW('m',  12,  struct mmb_info)
#define IOC_MMB_ALLOC_V2	_IOWR('m', 13,  struct mmb_info)

#define IOC_MMB_USER_REMAP	_IOWR('m', 20,  struct mmb_info)
#define IOC_MMB_USER_REMAP_CACHED _IOWR('m', 21,  struct mmb_info)
#define IOC_MMB_USER_UNMAP	_IOWR('m', 22,  struct mmb_info)

#define IOC_MMB_VIRT_GET_PHYS	_IOWR('m',  23,  struct mmb_info)

#define IOC_MMB_ADD_REF		_IO('r', 30)	/* ioctl(file, cmd, arg), arg is mmb_addr */
#define IOC_MMB_DEC_REF		_IO('r', 31)	/* ioctl(file, cmd, arg), arg is mmb_addr */

#define IOC_MMB_FLUSH_DCACHE	_IO('c', 40)

#define IOC_MMB_FLUSH_DCACHE_DIRTY		_IOW('d', 50, struct dirty_area)
#define IOC_MMB_TEST_CACHE	_IOW('t',  11,  struct mmb_info)

#endif

