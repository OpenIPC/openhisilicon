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

/* ── ioctl numbers ───────────────────────────────────────────────
 *
 * Decoded from cv500 vendor libnnie.so (42 KB, kaeru:
 * nnie-neo-cv500-ioctl-abi). Type byte = 'M' (0x4d). Five entries
 * are reached from libnnie.so; vendor kernel svp_nnie_ioctl also
 * recognises four more (0x05/0x06/0x07/0x08/0x09) used when the
 * per-call context state equals 0xc — those are bbox-mode dispatch
 * variants we'll wire in Phase 3 after seeing them used on-target.
 *
 *   nr | size | full ioctl   | userspace entry
 *   ---+------+--------------+--------------------------
 *  0x00| 1624 | 0xc6584d00   | HI_MPI_SVP_NNIE_Forward
 *  0x01| 1728 | 0xc6c04d01   | HI_MPI_SVP_NNIE_ForwardWithBbox
 *  0x02|   24 | 0xc0184d02   | HI_MPI_SVP_NNIE_Query
 *  0x03|   24 | 0xc0184d03   | HI_MPI_SVP_NNIE_AddTskBuf
 *  0x04|   24 | 0xc0184d04   | HI_MPI_SVP_NNIE_RemoveTskBuf
 */
#define NNIE_IOC_FORWARD              0xc6584d00u
#define NNIE_IOC_FORWARD_WITH_BBOX    0xc6c04d01u
#define NNIE_IOC_QUERY                0xc0184d02u
#define NNIE_IOC_ADD_TSKBUF           0xc0184d03u
#define NNIE_IOC_REMOVE_TSKBUF        0xc0184d04u

static long nnie_op_forward(unsigned long arg, int with_bbox);
static long nnie_op_query(unsigned long arg);
static long nnie_op_add_tskbuf(unsigned long arg);
static long nnie_op_remove_tskbuf(unsigned long arg);

static long nnie_dispatch(unsigned int cmd, unsigned long arg)
{
	switch (cmd) {
	case NNIE_IOC_FORWARD:
		return nnie_op_forward(arg, 0);
	case NNIE_IOC_FORWARD_WITH_BBOX:
		return nnie_op_forward(arg, 1);
	case NNIE_IOC_QUERY:
		return nnie_op_query(arg);
	case NNIE_IOC_ADD_TSKBUF:
		return nnie_op_add_tskbuf(arg);
	case NNIE_IOC_REMOVE_TSKBUF:
		return nnie_op_remove_tskbuf(arg);
	default:
		pr_info_once("nnie_neo: unknown ioctl 0x%08x — vendor recognises 0x4d05..0x4d09 in bbox mode (Phase 3)\n",
			     cmd);
		return -ENOIOCTLCMD;
	}
}

/* ── Phase 0/1 stubs (all reject with -EOPNOTSUPP for now) ─────── */

static long nnie_op_forward(unsigned long arg, int with_bbox)
{
	pr_info_once("nnie_neo: Forward%s arg=%lx (Phase 1 stub — needs HW dispatch)\n",
		     with_bbox ? "WithBbox" : "", arg);
	return -EOPNOTSUPP;
}

static long nnie_op_query(unsigned long arg)
{
	/* arg = { u32 handle (in), u32 bBlock (in), u32 done_out } —
	 * mirrors ive Query layout (kernel/ive_neo/ive_neo.c) until we
	 * can verify the actual NNIE Query struct. For Phase 1 just say
	 * "always done" since we never dispatch HW work yet. */
	if (arg)
		((u32 *)arg)[2] = 1;
	return 0;
}

static long nnie_op_add_tskbuf(unsigned long arg)
{
	pr_info_once("nnie_neo: AddTskBuf (Phase 1 stub)\n");
	return -EOPNOTSUPP;
}

static long nnie_op_remove_tskbuf(unsigned long arg)
{
	pr_info_once("nnie_neo: RemoveTskBuf (Phase 1 stub)\n");
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
