// SPDX-License-Identifier: GPL-2.0
/*
 * nnie_trace — kprobe every interesting hal_svp_nnie_* function so we
 * can capture vendor's exact per-task register-write sequence and diff
 * against our cleanroom. Pass each kprobe target as a module param
 * since vendor symbols are local (lowercase t) and we need explicit
 * addresses.
 *
 *   wta=<hal_svp_nnie_write_task_addr addr>
 *   sta=<hal_svp_nnie_start addr>
 *   cir=<hal_svp_nnie_cfg_irq addr>
 *   tmo=<hal_svp_nnie_set_timeout addr>
 *   clg=<hal_svp_nnie_enable_clk_gt addr>
 *   out=<hal_svp_nnie_set_outstanding addr>
 *   ckm=<hal_svp_nnie_disable_check_sum addr>
 *
 * Each probe logs the function name + ARM_r0..r3 at entry. For
 * write_task_addr, also dumps the descriptor + tskbuf via phys_to_virt.
 */
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/printk.h>
#include <linux/mm.h>
#include <linux/io.h>

#define MK_PARAM(name, label)                                                       \
	static unsigned long name##_addr = 0;                                       \
	module_param_named(name, name##_addr, ulong, 0644);                         \
	static int name##_pre(struct kprobe *p, struct pt_regs *regs);              \
	static struct kprobe name##_kp = { .pre_handler = name##_pre };             \
	static int name##_pre(struct kprobe *p, struct pt_regs *regs)               \
	{                                                                           \
		pr_info("nnie_trace: %-30s r0=%08lx r1=%08lx r2=%08lx r3=%08lx\n", \
			label, regs->ARM_r0, regs->ARM_r1,                          \
			regs->ARM_r2, regs->ARM_r3);                                \
		return 0;                                                           \
	}

/* Override sta_pre to also dump the full NNIE reg window at the moment
 * vendor calls hal_svp_nnie_start — gives us vendor's exact pre-START
 * HW state for direct comparison vs cleanroom's dmesg dump. */
static void __iomem *g_nnie_regs_trace;
static int sta_with_regs_pre(struct kprobe *p, struct pt_regs *regs)
{
	int o;
	pr_info("nnie_trace: start                       r0=%08lx r1=%08lx r2=%08lx r3=%08lx\n",
		regs->ARM_r0, regs->ARM_r1, regs->ARM_r2, regs->ARM_r3);
	if (!g_nnie_regs_trace) {
		g_nnie_regs_trace = ioremap(0x11100000, 0x1000);
		if (!g_nnie_regs_trace) {
			pr_warn("nnie_trace: ioremap failed\n");
			return 0;
		}
	}
	for (o = 0; o < 0xd0; o += 16)
		pr_info("nnie_trace: pre-START reg+0x%02x: %08x %08x %08x %08x\n",
			o,
			readl(g_nnie_regs_trace + o + 0), readl(g_nnie_regs_trace + o + 4),
			readl(g_nnie_regs_trace + o + 8), readl(g_nnie_regs_trace + o + 12));
	return 0;
}

/* Special handler for write_task_addr: r2 = task descriptor phys (lo).
 * Walk the 64-byte descriptor + the tskbuf it points at, dump both via
 * phys_to_virt so we can byte-compare vendor's runtime state to our
 * cleanroom's. Defined before MK_PARAM(wta, ...) so we can override. */
static int wta_with_dump_pre(struct kprobe *p, struct pt_regs *regs)
{
	unsigned long desc_phys = regs->ARM_r2;
	const u32 *d;
	int i;

	pr_info("nnie_trace: write_task_addr            r0=%08lx r1=%08lx r2=%08lx r3=%08lx\n",
		regs->ARM_r0, regs->ARM_r1, regs->ARM_r2, regs->ARM_r3);
	if (!desc_phys || !pfn_valid(desc_phys >> PAGE_SHIFT)) {
		pr_warn("nnie_trace: desc phys 0x%lx not in direct map\n", desc_phys);
		return 0;
	}
	d = (const u32 *)phys_to_virt(desc_phys);
	pr_info("nnie_trace: desc @0x%lx: %08x %08x %08x %08x  %08x %08x %08x %08x\n",
		desc_phys, d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7]);
	pr_info("nnie_trace: desc @0x%lx: %08x %08x %08x %08x  %08x %08x %08x %08x\n",
		desc_phys + 32,
		d[8], d[9], d[10], d[11], d[12], d[13], d[14], d[15]);

	/* d[8] = tskbuf phys (low half). Dump up to 320 bytes. */
	if (d[8] && pfn_valid(d[8] >> PAGE_SHIFT)) {
		const u32 *t = (const u32 *)phys_to_virt(d[8]);
		for (i = 0; i < 20; i++)
			pr_info("nnie_trace: tskbuf @0x%x +%03x: %08x %08x %08x %08x\n",
				d[8], i * 16, t[i*4], t[i*4+1], t[i*4+2], t[i*4+3]);
	}
	return 0;
}

MK_PARAM(wta, "write_task_addr")
MK_PARAM(sta, "start")
MK_PARAM(cir, "cfg_irq")
MK_PARAM(tmo, "set_timeout")
MK_PARAM(clg, "enable_clk_gt")
MK_PARAM(out, "set_outstanding")
MK_PARAM(ckm, "disable_check_sum")

static void try_register(struct kprobe *kp, unsigned long addr, const char *label)
{
	int ret;
	if (!addr) return;
	kp->addr = (kprobe_opcode_t *)addr;
	ret = register_kprobe(kp);
	if (ret)
		pr_warn("nnie_trace: %s @0x%lx -> %d\n", label, addr, ret);
	else
		pr_info("nnie_trace: %s @0x%lx hooked\n", label, addr);
}

static int __init nnie_trace_init(void)
{
	wta_kp.pre_handler = wta_with_dump_pre;
	sta_kp.pre_handler = sta_with_regs_pre;
	try_register(&wta_kp, wta_addr, "write_task_addr");
	try_register(&sta_kp, sta_addr, "start");
	try_register(&cir_kp, cir_addr, "cfg_irq");
	try_register(&tmo_kp, tmo_addr, "set_timeout");
	try_register(&clg_kp, clg_addr, "enable_clk_gt");
	try_register(&out_kp, out_addr, "set_outstanding");
	try_register(&ckm_kp, ckm_addr, "disable_check_sum");
	return 0;
}

static void __exit nnie_trace_exit(void)
{
	if (wta_addr) unregister_kprobe(&wta_kp);
	if (sta_addr) unregister_kprobe(&sta_kp);
	if (cir_addr) unregister_kprobe(&cir_kp);
	if (tmo_addr) unregister_kprobe(&tmo_kp);
	if (clg_addr) unregister_kprobe(&clg_kp);
	if (out_addr) unregister_kprobe(&out_kp);
	if (ckm_addr) unregister_kprobe(&ckm_kp);
	if (g_nnie_regs_trace) iounmap(g_nnie_regs_trace);
}

module_init(nnie_trace_init);
module_exit(nnie_trace_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Trace vendor hal_svp_nnie_* call sequence");
