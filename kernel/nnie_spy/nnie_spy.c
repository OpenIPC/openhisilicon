// SPDX-License-Identifier: GPL-2.0
/*
 * nnie_spy — kprobe-based capture of vendor's NNIE HW dispatch.
 *
 * Single-purpose debug module for Phase 9 of #111: load vendor
 * open_nnie.ko, then load this module to kprobe
 *   hal_svp_nnie_write_task_addr
 * which is the single chokepoint where vendor writes the task
 * descriptor phys address into the NNIE HW. r2/r3 hold the phys
 * low/high half. We then phys_to_virt the address (works because
 * vendor's ring lives in CMA-managed kernel RAM) and dump the
 * 64-byte descriptor to dmesg so we can diff against our
 * clean-room descriptor byte-for-byte.
 *
 * Build: drop in kernel/nnie_spy/Kbuild + add to cv500 build like
 * nnie_neo. Load order on target:
 *   insmod open_nnie.ko       (vendor)
 *   insmod open_nnie_spy.ko   (this)
 *   run any libnnie.so client that triggers Forward
 *   dmesg | grep nnie_spy
 *   rmmod open_nnie_spy
 */
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/mm.h>
#include <linux/io.h>

static unsigned int g_hits;

static int nnie_spy_pre(struct kprobe *p, struct pt_regs *regs)
{
	u32 task_phys_lo = (u32)regs->ARM_r2;
	u32 task_phys_hi = (u32)regs->ARM_r3;
	u32 core_id      = (u32)regs->ARM_r0;
	u64 task_phys    = ((u64)task_phys_hi << 32) | task_phys_lo;
	const u32 *d;
	void *vir;

	g_hits++;

	/* CMA-backed kernel RAM is in the direct map, so phys_to_virt
	 * works. If the phys is somehow outside lowmem this WILL fault
	 * — accept that risk for a one-shot debug capture. */
	if (!pfn_valid(task_phys >> PAGE_SHIFT)) {
		pr_info("nnie_spy[%u]: core=%u task_phys=0x%llx NOT lowmem — skip dump\n",
			g_hits, core_id, task_phys);
		return 0;
	}
	vir = phys_to_virt((unsigned long)task_phys);
	d = (const u32 *)vir;

	pr_info("nnie_spy[%u]: core=%u task_phys=0x%llx vir=%p\n",
		g_hits, core_id, task_phys, vir);
	pr_info("nnie_spy[%u]:   d[0..7]  = %08x %08x %08x %08x  %08x %08x %08x %08x\n",
		g_hits, d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7]);
	pr_info("nnie_spy[%u]:   d[8..15] = %08x %08x %08x %08x  %08x %08x %08x %08x\n",
		g_hits, d[8], d[9], d[10], d[11], d[12], d[13], d[14], d[15]);

	/* Also dump the first 256 bytes pointed to by d[8] (= tsk_buf_phys
	 * low half) if it looks like a valid MMZ phys. That's the variable-
	 * length tail (§1-§3 in our nnie_hw_task.h notes). */
	if (d[8] && d[9] == 0 && pfn_valid(d[8] >> PAGE_SHIFT)) {
		const u32 *t = (const u32 *)phys_to_virt(d[8]);
		int i;

		pr_info("nnie_spy[%u]:   tail @0x%08x (= d[8] = tsk_buf_phys):\n",
			g_hits, d[8]);
		for (i = 0; i < 32; i += 8)
			pr_info("nnie_spy[%u]:     +%03x: %08x %08x %08x %08x  %08x %08x %08x %08x\n",
				g_hits, i * 4,
				t[i+0], t[i+1], t[i+2], t[i+3],
				t[i+4], t[i+5], t[i+6], t[i+7]);
	}

	return 0;
}

static struct kprobe g_kp = {
	.symbol_name = "hal_svp_nnie_write_task_addr",
	.pre_handler = nnie_spy_pre,
};

static int __init nnie_spy_init(void)
{
	int ret = register_kprobe(&g_kp);

	if (ret < 0) {
		pr_err("nnie_spy: register_kprobe('%s') failed: %d\n",
		       g_kp.symbol_name, ret);
		return ret;
	}
	pr_info("nnie_spy: hooked %s @%p — run a Forward to capture\n",
		g_kp.symbol_name, g_kp.addr);
	return 0;
}

static void __exit nnie_spy_exit(void)
{
	unregister_kprobe(&g_kp);
	pr_info("nnie_spy: %u hits total\n", g_hits);
}

module_init(nnie_spy_init);
module_exit(nnie_spy_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Phase 9 debug: kprobe vendor NNIE task-addr writes");
