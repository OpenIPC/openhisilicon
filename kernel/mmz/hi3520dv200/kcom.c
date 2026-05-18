/* kcom.c
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

//#include <linux/autoconf.h>
#include <linux/kernel.h>

#include <linux/version.h>
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
//#include <linux/smp_lock.h>
//#include <linux/devfs_fs_kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <mach/hardware.h>
#include <asm/io.h>
#include <asm/system.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/spinlock.h>
#include <linux/vmalloc.h>

#include <linux/string.h>
#include <linux/list.h>

#include <linux/time.h>

// BEGIN: Modified by x00132463, 2011-3-22 16 : 29 : 43 
//#include <linux/kcom.h>

#include "kcom.h"
#include "media-mem.h"


mmb_addr_t new_mmb(const char *name, int size, unsigned long align, 
		const char *zone_name)
{
	hil_mmb_t *mmb;
	
	mmb = hil_mmb_alloc(name, size, align, 0, zone_name);
	if(mmb ==NULL)
		return MMB_ADDR_INVALID;

	return (mmb_addr_t)hil_mmb_phys(mmb);
}

void delete_mmb(mmb_addr_t addr)
{
	hil_mmb_t *mmb;

	mmb = hil_mmb_getby_phys((unsigned long)addr);
	if(mmb ==NULL)
		return;

	hil_mmb_free(mmb);
}

void *remap_mmb(mmb_addr_t addr)
{
	hil_mmb_t *mmb;

	mmb = hil_mmb_getby_phys((unsigned long)addr);
	if(mmb ==NULL)
		return NULL;

	return hil_mmb_map2kern(mmb);
}


void *remap_mmb_cached(mmb_addr_t addr)
{
	hil_mmb_t *mmb;

	mmb = hil_mmb_getby_phys((unsigned long)addr);
	if(mmb ==NULL)
		return NULL;

	return hil_mmb_map2kern_cached(mmb);
}

void flush_mmb_byaddr(void *kvirt, unsigned long phys_addr, unsigned long length)
{
	 hil_mmb_flush_dcache_byaddr(kvirt,phys_addr,length);

	 return;
}

int unmap_mmb(void *mapped_addr)
{
	hil_mmb_t *mmb;

	mmb = hil_mmb_getby_kvirt(mapped_addr);
	if(mmb ==NULL)
		return -1;

	return hil_mmb_unmap(mmb);
}

EXPORT_SYMBOL(new_mmb);
EXPORT_SYMBOL(delete_mmb);
EXPORT_SYMBOL(remap_mmb);
EXPORT_SYMBOL(remap_mmb_cached);
EXPORT_SYMBOL(unmap_mmb);
EXPORT_SYMBOL(flush_mmb_byaddr);

// BEGIN: Modified by x00132463, 2011-3-22 16 : 30 : 44 
#if 0
static struct kcom_media_memory kcom_mmz = {
	.kcom = KCOM_OBJ_INIT(UUID_MEDIA_MEMORY, UUID_MEDIA_MEMORY, NULL, THIS_MODULE, KCOM_TYPE_OBJECT, NULL),

	.new_zone = new_zone,
	.delete_zone = delete_zone,

	.new_mmb = new_mmb,
	.delete_mmb = delete_mmb,

	.remap_mmb = remap_mmb,
	.remap_mmb_cached = remap_mmb_cached,
	.refer_mapped_mmb = refer_mapped_mmb,
	.unmap_mmb = unmap_mmb,

	.get_mmb = get_mmb,
	.put_mmb = put_mmb,
};

static struct kcom_media_memory_v2 kcom_mmz_v2 = {
	.kcom = KCOM_OBJ_INIT(UUID_MEDIA_MEMORY_V2, UUID_MEDIA_MEMORY_V2, NULL, THIS_MODULE, KCOM_TYPE_OBJECT, NULL),

	.new_zone = new_zone_v2,
	.delete_zone = delete_zone,

	.new_mmb = new_mmb_v2,
	.delete_mmb = delete_mmb,

	.remap_mmb = remap_mmb,
	.remap_mmb_cached = remap_mmb_cached,
	.refer_mapped_mmb = refer_mapped_mmb,
	.unmap_mmb = unmap_mmb,

	.get_mmb = get_mmb,
	.put_mmb = put_mmb,
};
//#endif


EXPORT_SYMBOL(remap_mmb_cached);

// BEGIN: Modified by x00132463, 2011-3-22 16 : 30 : 38 
//#if 0
int kcom_mmz_register(void)
{
	kcom_register(&kcom_mmz.kcom);
	kcom_register(&kcom_mmz_v2.kcom);
	return 0;
}

void kcom_mmz_unregister(void)
{
	kcom_unregister(&kcom_mmz.kcom);
	kcom_unregister(&kcom_mmz_v2.kcom);
}
#endif