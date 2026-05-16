// SPDX-License-Identifier: GPL-2.0
/*
 * nnie_dump — read N bytes of MMZ phys via module-param and dump to
 * dmesg. Phase 9 helper: vendor's tskbuf/descriptor live in CMA-
 * managed lowmem which the userspace devmem can't mmap. Module params:
 *   phys=0xa9cb0000 size=320
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/mm.h>
#include <linux/io.h>

static unsigned long phys = 0;
static unsigned int  size = 64;
module_param(phys, ulong, 0644);
module_param(size, uint,  0644);

static int __init dump_init(void)
{
	const u32 *v;
	unsigned int i;

	if (!phys || !size) {
		pr_info("nnie_dump: usage: insmod nnie_dump.ko phys=0xXXX size=N\n");
		return 0;
	}
	if (!pfn_valid(phys >> PAGE_SHIFT)) {
		pr_warn("nnie_dump: phys 0x%lx not in lowmem direct map\n", phys);
		return 0;
	}
	v = (const u32 *)phys_to_virt(phys);
	pr_info("nnie_dump: phys=0x%lx size=%u\n", phys, size);
	for (i = 0; i < size / 16; i++)
		pr_info("nnie_dump:   +%04x: %08x %08x %08x %08x\n",
			i * 16, v[i*4], v[i*4+1], v[i*4+2], v[i*4+3]);
	return 0;
}

static void __exit dump_exit(void) {}

module_init(dump_init);
module_exit(dump_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MMZ phys-mem dumper");
