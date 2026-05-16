// SPDX-License-Identifier: GPL-2.0
/* Minimal kprobe test — probe printk to confirm kprobes infrastructure works.
 * Pass addr=0x... from /proc/kallsyms 'T printk' line. */
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/kernel.h>

static unsigned long g_addr = 0;
module_param_named(addr, g_addr, ulong, 0644);

static int probe_pre(struct kprobe *p, struct pt_regs *regs)
{
	pr_emerg("PROBE FIRED at 0x%lx\n", g_addr);
	return 0;
}

static struct kprobe g_kp = { .pre_handler = probe_pre };

static int __init probe_test_init(void)
{
	int ret;
	if (!g_addr) {
		pr_err("probe_test: need addr=...\n");
		return -EINVAL;
	}
	g_kp.addr = (kprobe_opcode_t *)g_addr;
	ret = register_kprobe(&g_kp);
	pr_info("probe_test: register_kprobe(0x%lx) -> %d\n", g_addr, ret);
	return ret;
}
static void __exit probe_test_exit(void) { unregister_kprobe(&g_kp); }
module_init(probe_test_init);
module_exit(probe_test_exit);
MODULE_LICENSE("GPL");
