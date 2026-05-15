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

#include "nnie_hw_task.h"
#include "nnie_hw_regs.h"

/* ── Platform state exported to nnie_init.c ───────────────────────── */

void *g_nnie_regs;
void *g_gdc_regs;
unsigned int g_nnie_irq;
unsigned int g_gdc_irq;

EXPORT_SYMBOL(g_nnie_regs);
EXPORT_SYMBOL(g_gdc_regs);
EXPORT_SYMBOL(g_nnie_irq);
EXPORT_SYMBOL(g_gdc_irq);

/* ── cv500 sys coordination registers ─────────────────────────────
 *
 * The NNIE/GDC/VENC RAM-sharing + mutex bits live in the cv500 sys
 * MMIO window (phys 0x12020000, DT name "sys" inside the
 * hisilicon,hisi-sys node). Phase 4 will drive them around each
 * Forward dispatch.
 *
 * We share access with vendor's open_sys.ko (or our future clean-room
 * sys module): use plain ioremap, not request_mem_region, to avoid
 * EBUSY clash with the vendor driver's claim.
 */
#define NNIE_SYS_BASE_PHYS         0x12020000UL
#define NNIE_SYS_WINDOW_SIZE       0x1000UL
#define NNIE_SYS_REG_VGS_RAM       0x0000   /* bit 13 = vgs_bootroom uses RAM */
#define NNIE_SYS_REG_MUTEX         0x0008   /* bits 0..2 = NNIE/GDC/VENC mutex */
#define NNIE_SYS_REG_NNIE_RAM      0x0034   /* bit  0 = NNIE has RAM */

#define NNIE_SYS_BIT_VGS_RAM       (1u << 13)
#define NNIE_SYS_BIT_MUTEX_VENC    (1u <<  1)
#define NNIE_SYS_BIT_MUTEX_GDC     (1u <<  2)
#define NNIE_SYS_BIT_NNIE_RAM      (1u <<  0)

static void __iomem *g_sys_regs;       /* Phase 3 — read-only verification */
static spinlock_t    g_sys_lock;        /* Phase 4 — atomic R-M-W */

/* Phase 4 helpers (defined but not yet called from Forward path). */
static void __maybe_unused nnie_sys_set_bit(u32 reg_off, u32 bit)
{
	unsigned long flags;
	u32 v;

	if (!g_sys_regs)
		return;
	spin_lock_irqsave(&g_sys_lock, flags);
	v = readl(g_sys_regs + reg_off);
	writel(v | bit, g_sys_regs + reg_off);
	spin_unlock_irqrestore(&g_sys_lock, flags);
}

static void __maybe_unused nnie_sys_clear_bit(u32 reg_off, u32 bit)
{
	unsigned long flags;
	u32 v;

	if (!g_sys_regs)
		return;
	spin_lock_irqsave(&g_sys_lock, flags);
	v = readl(g_sys_regs + reg_off);
	writel(v & ~bit, g_sys_regs + reg_off);
	spin_unlock_irqrestore(&g_sys_lock, flags);
}

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
#define NNIE_FWD_OFF_PSTMODEL_UVA   776
#define NNIE_FWD_OFF_CTRL          1552
#define NNIE_FWD_OFF_INSTANT       1616

/* SVP_BLOB_S internal offsets (size 48 B) */
#define NNIE_BLOB_OFF_STRIDE          4
#define NNIE_BLOB_OFF_VIRADDR         8
#define NNIE_BLOB_OFF_PHYADDR        16
#define NNIE_BLOB_OFF_NUM            24
#define NNIE_BLOB_OFF_WIDTH          28
#define NNIE_BLOB_OFF_HEIGHT         32
#define NNIE_BLOB_OFF_CHN            36

/* SVP_NNIE_FORWARD_CTRL_S internal offsets (size 64 B) */
#define NNIE_CTRL_OFF_SRCNUM          0
#define NNIE_CTRL_OFF_DSTNUM          4
#define NNIE_CTRL_OFF_NETSEGID        8
#define NNIE_CTRL_OFF_NNIEID         12
#define NNIE_CTRL_OFF_TMP_PHYS       16   /* stTmpBuf.u64PhyAddr */
#define NNIE_CTRL_OFF_TSK_PHYS       40   /* stTskBuf.u64PhyAddr */

static void nnie_fill_task_header(struct nnie_hw_task *task,
                                  u64 model_phys, u32 inst_off, u32 inst_len,
                                  u64 tsk_phys, u64 tmp_phys, u32 batch_num,
                                  bool instant)
{
	memset(task, 0, sizeof(*task));
	task->trigger_mode    = cpu_to_le16(instant ? 1 : 0);
	task->model_file_phys = cpu_to_le64(model_phys);
	task->seg_inst_offset = cpu_to_le32(inst_off);
	task->seg_inst_len    = cpu_to_le32(inst_len);
	task->tsk_buf_phys    = cpu_to_le64(tsk_phys);
	task->tmp_buf_phys    = cpu_to_le64(tmp_phys);
	task->src_batch_num   = cpu_to_le32(batch_num);
}

static long nnie_op_forward(unsigned long arg, int with_bbox)
{
	const u32 arg_size = with_bbox ?
		NNIE_FORWARD_BBOX_ARG_SIZE : NNIE_FORWARD_ARG_SIZE;
	u8 *buf = (u8 *)arg;
	u32 src_num, dst_num, net_seg_id, instant, batch_num;
	u64 tsk_phys, tmp_phys;
	struct nnie_hw_task task;

	if (!buf)
		return -EINVAL;

	/* Sanity: OSAL framework already did copy_from_user for ioctl
	 * size NNIE_FORWARD_*_ARG_SIZE, so buf is a kernel kbuf. */

	src_num    = *(u32 *)(buf + NNIE_FWD_OFF_CTRL + NNIE_CTRL_OFF_SRCNUM);
	dst_num    = *(u32 *)(buf + NNIE_FWD_OFF_CTRL + NNIE_CTRL_OFF_DSTNUM);
	net_seg_id = *(u32 *)(buf + NNIE_FWD_OFF_CTRL + NNIE_CTRL_OFF_NETSEGID);
	tmp_phys   = *(u64 *)(buf + NNIE_FWD_OFF_CTRL + NNIE_CTRL_OFF_TMP_PHYS);
	tsk_phys   = *(u64 *)(buf + NNIE_FWD_OFF_CTRL + NNIE_CTRL_OFF_TSK_PHYS);
	instant    = *(u32 *)(buf + NNIE_FWD_OFF_INSTANT);
	batch_num  = *(u32 *)(buf + NNIE_FWD_OFF_ASTSRC + NNIE_BLOB_OFF_NUM);

	/* Build the fixed-layout part of the 64-B HW task descriptor.
	 * Note: model_phys (task[+16]) requires copying the user's
	 * SVP_NNIE_MODEL_S to get model->stBase.u64PhyAddr — deferred to
	 * Phase 5 along with the variable-length descriptor tail (per-input
	 * stride table, per-node shape data, per-batch DMA addrs). For now
	 * we build with model_phys=0/inst_off=0/inst_len=0 just to validate
	 * the struct layout at compile time. */
	nnie_fill_task_header(&task,
	                      /*model_phys=*/0, /*inst_off=*/0, /*inst_len=*/0,
	                      tsk_phys, tmp_phys, batch_num, !!instant);

	pr_info_once("nnie_neo: Forward%s arg=%u B  SrcNum=%u DstNum=%u NetSegId=%u  Instant=%u  Batch=%u\n",
		     with_bbox ? "WithBbox" : "",
		     arg_size, src_num, dst_num, net_seg_id, instant, batch_num);
	pr_info_once("nnie_neo:   ctrl.stTskBuf.phys=0x%llx  stTmpBuf.phys=0x%llx\n",
		     tsk_phys, tmp_phys);
	pr_info_once("nnie_neo:   task hdr: trigger=%u model=0x%llx inst_off=%u inst_len=%u\n",
		     le16_to_cpu(task.trigger_mode),
		     le64_to_cpu(task.model_file_phys),
		     le32_to_cpu(task.seg_inst_offset),
		     le32_to_cpu(task.seg_inst_len));

	/* Phase 5 will:
	 *   1. copy_from_user *pstModel (13992 B) to read stBase.u64PhyAddr
	 *      + astSeg[NetSegId].u32InstOffset/InstLen — the fixed task[16],
	 *      task[24], task[28] fields. (offsetof(MODEL_S, stBase)=0x3690,
	 *      cross-checked against vendor literal in nnie_hw_task.h.)
	 *   2. Append the variable-length descriptor tail: per-input stride
	 *      table (u32 × SrcNum), per-node shape data (16 × u64), per-batch
	 *      DMA addrs (u64 = PhyAddr + j*Stride*H*Chn). Decoded from
	 *      vendor svp_nnie_fill_forward_task @0x91d4-0x94c0.
	 *   3. Acquire sys-window mutex: nnie_sys_set_bit(NNIE_SYS_REG_NNIE_RAM,
	 *      NNIE_SYS_BIT_NNIE_RAM). (cv500 sys @0x12020034 bit 0; vendor
	 *      goes via cmpi mod 51 fn 0xd1 = sys_hal_gdc_nnie_set_ram_using.)
	 *   4. Drive NNIE registers per nnie_hw_regs.h:
	 *        writel(NNIE_CLK_GATE_EN,        regs + NNIE_REG_CLK_GATE);
	 *        writel(NNIE_OUTSTANDING_DEFAULT,regs + NNIE_REG_OUTSTANDING);
	 *        writel(NNIE_IRQ_ALL,            regs + NNIE_REG_IRQ_CFG);
	 *        writel(task_dma_lo,             regs + NNIE_REG_TASK_ADDR_LO);
	 *        writel(0,                       regs + NNIE_REG_TASK_ADDR_HI);
	 *        wmb();
	 *        writel(NNIE_START_GO,           regs + NNIE_REG_START);
	 *   5. Wait on g_nnie_done completion. (Phase 0 request_irq currently
	 *      fails -EBUSY because vendor open_gdc.ko shares SPI 53/54 with
	 *      IRQF_SHARED — Phase 5 must request with matching flags or fall
	 *      back to polling NNIE_REG_IRQ_STATUS.)
	 *   6. Ack: writel(NNIE_IRQ_ALL, regs + NNIE_REG_IRQ_CLEAR);
	 *      Release: nnie_sys_clear_bit(...). Write handle=0 to buf+0.
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
	spin_lock_init(&g_sys_lock);

	/* Map the cv500 sys window for NNIE coordination registers
	 * (Phase 3 — read-only verification; Phase 4 will use the bit
	 * helpers above). Vendor open_sys.ko also owns this window;
	 * plain ioremap shares the mapping non-exclusively. */
	g_sys_regs = ioremap(NNIE_SYS_BASE_PHYS, NNIE_SYS_WINDOW_SIZE);
	if (g_sys_regs) {
		u32 vgs = readl(g_sys_regs + NNIE_SYS_REG_VGS_RAM);
		u32 mtx = readl(g_sys_regs + NNIE_SYS_REG_MUTEX);
		u32 nra = readl(g_sys_regs + NNIE_SYS_REG_NNIE_RAM);
		pr_info("nnie_neo: sys @0x%lx mapped — VGS=0x%08x MUTEX=0x%08x NNIE_RAM=0x%08x\n",
			NNIE_SYS_BASE_PHYS, vgs, mtx, nra);
	} else {
		pr_warn("nnie_neo: failed to ioremap sys window @0x%lx — Phase 4 coordination unavailable\n",
			NNIE_SYS_BASE_PHYS);
	}

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
	if (g_sys_regs) {
		iounmap(g_sys_regs);
		g_sys_regs = NULL;
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
