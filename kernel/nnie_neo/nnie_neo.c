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

/* ── Phase 3 stubs — arg ABIs decoded, HW dispatch TBD ────────────
 *
 * Forward arg buffer (1624 B), decoded from vendor libnnie.so Forward
 * worker @0x104c-0x117c:
 *
 *   off   | size | content
 *   ------+------+----------------------------------------------------
 *      0  |   4  | HI_HANDLE (out — kernel writes assigned handle)
 *      4  |   4  | pad
 *      8  | 768  | astSrc[16]  — 16 SVP_BLOB_S, 48 B each
 *    776  |   8  | pad
 *    784  | 768  | astDst[16]
 *   1552  |  64  | SVP_NNIE_FORWARD_CTRL_S {u32SrcNum, u32DstNum,
 *               |    u32NetSegId, enNnieId, stTmpBuf(24), stTskBuf(24)}
 *   1616  |   4  | bInstant
 *   1620  |   4  | pad
 *
 * ForwardWithBbox arg buffer (1728 B), 104 B larger — the extra block
 * holds the proposal-bbox MEM_INFO and u32ProposalNum. Layout same as
 * Forward up to the ctrl struct, then ctrl is 72 B (an extra u32 +
 * MEM_INFO_S vs 64 B Forward ctrl). TBD precise offset map; Phase 3
 * decode after Forward path works.
 */

#define NNIE_FORWARD_ARG_SIZE     1624u
#define NNIE_FORWARD_BBOX_ARG_SIZE 1728u
#define NNIE_BLOB_S_SIZE             48u
#define NNIE_FWD_CTRL_S_SIZE         64u

#define NNIE_FWD_OFF_HANDLE           0
#define NNIE_FWD_OFF_ASTSRC           8
#define NNIE_FWD_OFF_ASTDST         784
#define NNIE_FWD_OFF_CTRL          1552
#define NNIE_FWD_OFF_INSTANT       1616

static long nnie_op_forward(unsigned long arg, int with_bbox)
{
	const u32 arg_size = with_bbox ?
		NNIE_FORWARD_BBOX_ARG_SIZE : NNIE_FORWARD_ARG_SIZE;
	u8 *buf = (u8 *)arg;
	u32 src_num, dst_num, net_seg_id;

	if (!buf)
		return -EINVAL;

	/* Sanity: OSAL framework already did copy_from_user for ioctl
	 * size NNIE_FORWARD_*_ARG_SIZE, so buf is a kernel kbuf. */

	src_num    = *(u32 *)(buf + NNIE_FWD_OFF_CTRL + 0);
	dst_num    = *(u32 *)(buf + NNIE_FWD_OFF_CTRL + 4);
	net_seg_id = *(u32 *)(buf + NNIE_FWD_OFF_CTRL + 8);

	pr_info_once("nnie_neo: Forward%s arg=%u B  SrcNum=%u DstNum=%u NetSegId=%u — Phase 3 stub (no HW yet)\n",
		     with_bbox ? "WithBbox" : "",
		     arg_size, src_num, dst_num, net_seg_id);

	/* Phase 4 will:
	 *   1. Walk astSrc / astDst blobs (phys addresses) to set up the
	 *      HW task-node descriptor (vendor uses svp_nnie_fill_forward_task
	 *      @0x90d8 — large, 2134 lines of disasm; 64-byte HW task node
	 *      with fields at offsets 0,2,4,8..40 etc.).
	 *   2. Apply the [0x90] memory-priority knob. *Phase 3 finding*:
	 *      it's not a single "[0x90]" register write at all. Vendor
	 *      goes hal_svp_nnie_enable_ram -> cmpi_get_module_func_by_id
	 *      (module ID 51 = SYS, fn 0xd1) -> sys_hal_gdc_nnie_set_ram_using
	 *      in hi_sys.o (cv500 vendor blob). That function does an
	 *      atomic bit-set on bit 0 of register offset +0x34 of the
	 *      sys-module's MMIO window (g_sys_state[16] base). It's NNIE/
	 *      GDC RAM-sharing coordination, not an IVE-style mem-priority
	 *      knob. Companion funcs in hi_sys.o:
	 *        sys_hal_gdc_nnie_mutex_sel
	 *        sys_hal_venc_nnie_mutex_sel
	 *        sys_hal_nnie_get_mutex_state
	 *        sys_hal_nnie_gdc_get_mutex_state
	 *        sys_hal_vgs_bootroom_set_ram_using
	 *      Phase 4 will need a sweep of these to enumerate the full
	 *      set of bits, and either (a) drive them directly from
	 *      open_nnie_neo.ko by ioremap()'ing the sys register window,
	 *      or (b) export a small helper API from open_sys.ko (the
	 *      clean-room sys module — not yet implemented).
	 *   3. Submit task to NNIE block. svp_nnie_start_task @0x1934 also
	 *      goes through cmpi (module ID 37) — uses function-pointer table
	 *      slots [r5+120], [r5+124], [r5+128], [r5+132] to prepare/fire/
	 *      wait/finalise. Same architectural choice as IVE's chain submit
	 *      but mediated by cmpi.
	 *   4. Wait for IRQ on g_nnie_irq (Phase 0 currently fails its
	 *      request_irq with -EBUSY because vendor's open_gdc holds the
	 *      same SPI line with IRQF_SHARED — Phase 4 must request with
	 *      matching flags or use polling).
	 *   5. Write handle to buf+0; return 0.
	 *
	 * For now we set handle = 0 (matches ive_submit_nonxnn pattern)
	 * and return -EOPNOTSUPP so a userland test sees the right shape
	 * of failure. */
	*(u32 *)(buf + NNIE_FWD_OFF_HANDLE) = 0;
	return -EOPNOTSUPP;
}

static long nnie_op_query(unsigned long arg)
{
	/* arg = 24 B. Layout matches the IVE Query pattern enough that
	 * arg[2] = done_out is a safe stub. Vendor structure decode TBD. */
	if (arg)
		((u32 *)arg)[2] = 1;
	return 0;
}

/* AddTskBuf / RemoveTskBuf arg buffer (24 B), decoded from libnnie.so
 * 0x3134-0x3150: layout is plain SVP_MEM_INFO_S {u64 phys, u64 virt,
 * u32 size, u32 pad}. Kernel tracks the buffers in a per-instance list
 * so Forward dispatch knows which user-allocated regions are safe to
 * reference by phys. Phase 4 work. */
static long nnie_op_add_tskbuf(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u64 phys, virt;
	u32 size;

	if (!buf)
		return -EINVAL;
	phys = *(u64 *)(buf +  0);
	virt = *(u64 *)(buf +  8);
	size = *(u32 *)(buf + 16);
	pr_info_once("nnie_neo: AddTskBuf phys=0x%llx virt=0x%llx size=%u — Phase 4 stub\n",
		     phys, virt, size);
	return -EOPNOTSUPP;
}

static long nnie_op_remove_tskbuf(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u64 phys = buf ? *(u64 *)(buf + 0) : 0;
	pr_info_once("nnie_neo: RemoveTskBuf phys=0x%llx — Phase 4 stub\n", phys);
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

	pr_info("nnie_neo: /dev/nnie ready (Phase 3 — ioctl ABI wired, HW dispatch TBD)\n");
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
