// SPDX-License-Identifier: GPL-2.0
/*
 * nnie_neo — clean-room replacement for the vendor cv500 NNIE driver
 * (Hisilicon's CNN inference IP block at phys 0x11100000 on cv500/
 * av300/dv300, distinct from the IVE block at 0x11230000).
 *
 * STATUS: Phase 0 scaffold (issue #111). The module builds, binds to
 * the "hisilicon,hisi-nnie" DT node, reserves /dev/nnie, and returns
 * -EOPNOTSUPP for every ioctl. Subsequent phases reverse-engineer the
 * eight HI_MPI_SVP_NNIE_* userspace entry points (LoadModel,
 * UnloadModel, Forward, ForwardWithBbox, Query, GetTskBufSize,
 * AddTskBuf, RemoveTskBuf) into real implementations.
 *
 * Userspace consumer: libraries/nnie_neo/libnnie_neo.so (separate
 * library, distinct from libive_neo). Vendor's userspace lib is
 * libnnie.so (42 KB, 8 exports). The matching kernel ioctl ABI is
 * decoded in Phase 1 — until then the dispatcher just rejects.
 *
 * Vendor blob references (kernel/obj/hi3516cv500/hi_nnie.o, 109 KB):
 *   svp_nnie_ioctl       — dispatcher
 *   svp_nnie_forward     — single-pass inference dispatch
 *   svp_nnie_start_task  — HW task-node fill + submit
 *   svp_nnie_irq_route   — IRQ + completion bookkeeping
 *   svp_nnie_add_tskbuf  — task-buffer management
 *
 * Approach A from issue #111: separate kernel module rather than
 * extending ive_neo. Matches how the vendor ships it (hi3516cv500_ive.ko
 * + hi3516cv500_nnie.ko are independent).
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/completion.h>

#include "hi_osal.h"
#include "hi_common.h"

/* ── Platform state exported to nnie_init.c ───────────────────────── */

void *g_nnie_regs;
void *g_gdc_regs;
unsigned int g_nnie_irq;
unsigned int g_gdc_irq;

EXPORT_SYMBOL(g_nnie_regs);
EXPORT_SYMBOL(g_gdc_regs);
EXPORT_SYMBOL(g_nnie_irq);
EXPORT_SYMBOL(g_gdc_irq);

/* ── /dev/nnie character device ─────────────────────────────────── */

static osal_dev_t *g_nnie_dev;
static int g_nnie_irq_requested;
static struct completion g_nnie_done;

static irqreturn_t nnie_irq_handler(int irq, void *dev)
{
	/* Phase 0: stub. Phase 3 will read status + ack + signal completion. */
	complete(&g_nnie_done);
	return IRQ_HANDLED;
}

static long nnie_dispatch(unsigned int cmd, unsigned long arg)
{
	/* Phase 0: reject every ioctl with -EOPNOTSUPP. Phase 1 will
	 * decode the libnnie.so ioctl numbers and wire real handlers. */
	pr_info_once("nnie_neo: ioctl path reached (cmd=0x%08x) — Phase 0 stub\n",
		     cmd);
	return -EOPNOTSUPP;
}

static long nnie_osal_ioctl(unsigned int cmd, unsigned long arg, void *priv)
{
	return nnie_dispatch(cmd, arg);
}

static int nnie_osal_open(void *priv)    { return 0; }
static int nnie_osal_close(void *priv)   { return 0; }

static int nnie_osal_mmap(osal_vm_t *vm, unsigned long start, unsigned long end,
			  unsigned long vm_pgoff, void *priv)
{
	unsigned long size = end - start;
	osal_pgprot_noncached(vm);
	return osal_io_remap_pfn_range(vm, start, vm_pgoff, size);
}

static osal_fileops_t g_nnie_fops = {
	.open  = nnie_osal_open,
	.release = nnie_osal_close,
	.unlocked_ioctl = nnie_osal_ioctl,
	.mmap = nnie_osal_mmap,
};

/* ── Module init / exit called from nnie_init.c platform probe ──── */

int nnie_std_mod_init(void)
{
	int ret;

	init_completion(&g_nnie_done);

	g_nnie_dev = osal_createdev("nnie");
	if (!g_nnie_dev)
		return -ENOMEM;
	g_nnie_dev->fops = &g_nnie_fops;
	g_nnie_dev->minor = 100;   /* arbitrary — collision-check vs vendor minor 100? */

	ret = osal_registerdevice(g_nnie_dev);
	if (ret) {
		osal_destroydev(g_nnie_dev);
		g_nnie_dev = NULL;
		return ret;
	}

	if (g_nnie_irq) {
		ret = request_irq(g_nnie_irq, nnie_irq_handler, 0,
				  "nnie_neo", &g_nnie_dev);
		if (ret)
			pr_err("nnie_neo: request_irq(%u) failed: %d\n",
			       g_nnie_irq, ret);
		else
			g_nnie_irq_requested = 1;
	}

	pr_info("nnie_neo: /dev/nnie ready (Phase 0 stub — all ioctls return -EOPNOTSUPP)\n");
	return 0;
}

void nnie_std_mod_exit(void)
{
	if (g_nnie_irq_requested) {
		free_irq(g_nnie_irq, &g_nnie_dev);
		g_nnie_irq_requested = 0;
	}
	if (g_nnie_dev) {
		osal_deregisterdevice(g_nnie_dev);
		osal_destroydev(g_nnie_dev);
		g_nnie_dev = NULL;
	}
}

EXPORT_SYMBOL(nnie_std_mod_init);
EXPORT_SYMBOL(nnie_std_mod_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("nnie_neo: clean-room NNIE CNN driver (cv500/av300/dv300)");
