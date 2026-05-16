// SPDX-License-Identifier: GPL-2.0
/*
 * nnie_watch — poll NNIE register window for a brief window after
 * insmod and capture any task-address write. Run concurrently with a
 * vendor Forward to grab the vendor's task descriptor phys before HW
 * clears it.
 *
 *   insmod nnie_watch.ko &
 *   (within 200ms) run test_vendor_nnie
 *   wait for nnie_watch to print findings
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/mm.h>
#include <linux/kthread.h>

#define NNIE_PHYS 0x11100000UL
#define NNIE_SIZE 0x1000

static void __iomem *g_regs;
static struct task_struct *g_thr;

static int watch_thread(void *unused)
{
	u32 last_lo = 0, last_hi = 0;
	int iters = 0;
	int captures = 0;

	pr_info("nnie_watch: polling +0x20/+0x24 for task addr writes\n");

	while (!kthread_should_stop() && iters++ < 100000) {  /* up to ~30s */
		u32 lo = readl(g_regs + 0x20);
		u32 hi = readl(g_regs + 0x24);

		if (lo != last_lo || hi != last_hi) {
			u32 start = readl(g_regs + 0x30);
			u32 status = readl(g_regs + 0x3c);
			u32 tid    = readl(g_regs + 0x48);
			int o;

			pr_info("nnie_watch[%d]: TASK_ADDR=0x%x_%08x  START=0x%x STATUS=0x%x TASK_ID=0x%x\n",
				captures++, hi, lo, start, status, tid);
			/* Dump all NNIE registers at this moment. */
			for (o = 0; o < 0xc0; o += 16) {
				pr_info("nnie_watch[%d]: reg+0x%02x: %08x %08x %08x %08x\n",
					captures-1, o,
					readl(g_regs + o + 0), readl(g_regs + o + 4),
					readl(g_regs + o + 8), readl(g_regs + o + 12));
			}

			if (lo && pfn_valid(lo >> PAGE_SHIFT)) {
				const u32 *d = (const u32 *)phys_to_virt(lo);
				pr_info("nnie_watch[%d]: desc d[0..7]:  %08x %08x %08x %08x  %08x %08x %08x %08x\n",
					captures-1,
					d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7]);
				pr_info("nnie_watch[%d]: desc d[8..15]: %08x %08x %08x %08x  %08x %08x %08x %08x\n",
					captures-1,
					d[8], d[9], d[10], d[11], d[12], d[13], d[14], d[15]);
				/* tskbuf phys is at d[8] */
				if (d[8] && pfn_valid(d[8] >> PAGE_SHIFT)) {
					const u32 *t = (const u32 *)phys_to_virt(d[8]);
					int i;
					for (i = 0; i < 8; i++)
						pr_info("nnie_watch[%d]: tskbuf @0x%x +%02x: %08x %08x %08x %08x\n",
							captures-1, d[8], i*16,
							t[i*4], t[i*4+1], t[i*4+2], t[i*4+3]);
				}
			}
			last_lo = lo;
			last_hi = hi;
		}
		udelay(1);  /* 1us — fast enough to catch even a brief TASK_ADDR write */
	}
	pr_info("nnie_watch: exit after %d iters, %d captures\n", iters, captures);
	return 0;
}

static int __init watch_init(void)
{
	g_regs = ioremap(NNIE_PHYS, NNIE_SIZE);
	if (!g_regs) return -ENOMEM;
	g_thr = kthread_run(watch_thread, NULL, "nnie_watch");
	if (IS_ERR(g_thr)) { iounmap(g_regs); g_regs = NULL; return PTR_ERR(g_thr); }
	return 0;
}

static void __exit watch_exit(void)
{
	if (g_thr) kthread_stop(g_thr);
	if (g_regs) iounmap(g_regs);
}

module_init(watch_init);
module_exit(watch_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Poll NNIE TASK_ADDR register to capture vendor descriptors");
