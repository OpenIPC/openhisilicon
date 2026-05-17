/* One-shot phys-mem dumper. insmod with phys=<u64> size=<u32> and it
 * memremaps + hex-dumps to dmesg, then unloads cleanly. Used to RE
 * vendor's NNIE bbox tskbuf layout without /dev/mem access. */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/io.h>

static unsigned long long phys;
static unsigned int       size;
module_param(phys, ullong, 0);
module_param(size, uint,  0);

static int __init dump_init(void)
{
	void *v;

	if (!phys || !size || size > (1u << 20)) {
		pr_warn("nnie_phys_dump: bad params phys=%llx size=%u\n", phys, size);
		return -EINVAL;
	}
	v = memremap((phys_addr_t)phys, size, MEMREMAP_WB);
	if (!v) {
		pr_warn("nnie_phys_dump: memremap failed\n");
		return -EIO;
	}
	pr_info("nnie_phys_dump: %u bytes at 0x%llx:\n", size, phys);
	print_hex_dump(KERN_INFO, "dump ", DUMP_PREFIX_OFFSET, 32, 4, v, size, false);
	memunmap(v);
	return 0;
}

static void __exit dump_exit(void) {}

module_init(dump_init);
module_exit(dump_exit);
MODULE_LICENSE("GPL");
