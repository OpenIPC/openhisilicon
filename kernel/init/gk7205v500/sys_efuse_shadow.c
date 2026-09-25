/*
 * Present the GK7201V200 efuse SKU code to the SPC020 V200 MPP blobs as a
 * code they know.
 *
 * sys, vi, vpss and venc read the efuse word at 0x100a0028 (through the
 * pointer sys hands out as SYS_HAL_GetEfuseAddr) and pick their
 * capabilities from bits [7:4]: 1..8 and 10..12 in this SDK release.
 * GK7201V200 fuses 0xD0/0xD4, which only the vendor's 2024 blobs know;
 * with it, sys refuses to start and venc cannot create a channel.
 *
 * sys.o is linked with osal_ioremap/osal_iounmap redirected here. The
 * efuse block is handed out as a RAM copy of its first 0x100 bytes (the
 * modules read 0x10, 0x28 and 0x34 from it) with a 0xDx code rewritten to
 * 0x1x -- the code stock's own vi groups 0xD0 with. Every other mapping,
 * and every other fuse code, passes through untouched.
 */

#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/string.h>

#include "osal.h"

#define XM_EFUSE_BASE 0x100a0000UL
#define XM_EFUSE_COPY 0x100
#define XM_EFUSE_SKU  0x28

static int efuse_sku = -1;
module_param(efuse_sku, int, 0444);
MODULE_PARM_DESC(efuse_sku,
		 "efuse byte at 0x28 as the MPP should see it (-1: map 0xDx to 0x1x)");

static void *efuse_real;
static void *efuse_shadow;

void *xm_sys_ioremap(unsigned long phys_addr, unsigned long size)
{
	u32 sku, seen;

	if (phys_addr != XM_EFUSE_BASE || size < XM_EFUSE_COPY)
		return osal_ioremap(phys_addr, size);

	efuse_real = osal_ioremap(phys_addr, size);
	if (!efuse_real)
		return NULL;
	efuse_shadow = kzalloc(size, GFP_KERNEL);
	if (!efuse_shadow)
		return efuse_real;
	memcpy_fromio(efuse_shadow, efuse_real, XM_EFUSE_COPY);

	sku = readl(efuse_real + XM_EFUSE_SKU);
	seen = sku;
	if (efuse_sku >= 0)
		seen = (sku & ~0xffU) | (efuse_sku & 0xff);
	else if ((sku & 0xf0) == 0xd0)
		seen = (sku & ~0xf0U) | 0x10;
	*(u32 *)(efuse_shadow + XM_EFUSE_SKU) = seen;
	if (seen != sku)
		pr_info("sys: efuse SKU 0x%02x presented as 0x%02x\n",
			sku & 0xff, seen & 0xff);
	return efuse_shadow;
}

void xm_sys_iounmap(void *addr)
{
	if (addr && addr == efuse_shadow) {
		osal_iounmap(efuse_real);
		kfree(efuse_shadow);
		efuse_shadow = NULL;
		efuse_real = NULL;
		return;
	}
	osal_iounmap(addr);
}
