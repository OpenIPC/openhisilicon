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
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/vmalloc.h>
#include <linux/io.h>

#include "hi_osal.h"
#include "hi_common.h"

#include "nnie_hw_task.h"
#include "nnie_hw_regs.h"

/* ── Platform state exported to nnie_init.c ───────────────────────── */

void *g_nnie_regs;
void *g_gdc_regs;
unsigned int g_nnie_irq;
unsigned int g_gdc_irq;
struct platform_device *g_nnie_pf_dev;
static void __iomem *g_crg_regs;        /* CRG @ 0x12010000 (clock+reset) */
static spinlock_t g_crg_lock;

EXPORT_SYMBOL(g_nnie_regs);
EXPORT_SYMBOL(g_gdc_regs);
EXPORT_SYMBOL(g_nnie_irq);
EXPORT_SYMBOL(g_gdc_irq);
EXPORT_SYMBOL(g_nnie_pf_dev);

static void __maybe_unused nnie_crg_set_bit(u32 reg_off, u32 bit)
{
	unsigned long flags;
	u32 v;
	if (!g_crg_regs) return;
	spin_lock_irqsave(&g_crg_lock, flags);
	v = readl(g_crg_regs + reg_off);
	writel(v | bit, g_crg_regs + reg_off);
	spin_unlock_irqrestore(&g_crg_lock, flags);
}

static void __maybe_unused nnie_crg_clear_bit(u32 reg_off, u32 bit)
{
	unsigned long flags;
	u32 v;
	if (!g_crg_regs) return;
	spin_lock_irqsave(&g_crg_lock, flags);
	v = readl(g_crg_regs + reg_off);
	writel(v & ~bit, g_crg_regs + reg_off);
	spin_unlock_irqrestore(&g_crg_lock, flags);
}

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
/* cv500 sys-state window (status, mutex, RAM-using flags) */
#define NNIE_SYS_REG_VGS_RAM       0x0000   /* bit 13 = vgs_bootroom uses RAM */
#define NNIE_SYS_REG_MUTEX         0x0008   /* bits 0..2 = NNIE/GDC/VENC mutex */
#define NNIE_SYS_REG_NNIE_RAM      0x0034   /* bit  0 = NNIE has RAM */

/* cv500 CRG (Clock-Reset Generator) window — separate from sys.
 * Phys base 0x12010000 per DT (clock@12010000). */
#define NNIE_CRG_BASE_PHYS         0x12010000UL
#define NNIE_CRG_WINDOW_SIZE       0x1000
#define NNIE_CRG_REG_NNIE_CLK      0x00BC   /* bit 0=reset, bit 1=clk_en */

#define NNIE_SYS_BIT_VGS_RAM       (1u << 13)
#define NNIE_SYS_BIT_MUTEX_VENC    (1u <<  1)
#define NNIE_SYS_BIT_MUTEX_GDC     (1u <<  2)
#define NNIE_SYS_BIT_NNIE_RAM      (1u <<  0)
#define NNIE_SYS_BIT_NNIE_RESET    (1u <<  0)   /* 0=running, 1=reset */
#define NNIE_SYS_BIT_NNIE_CLK_EN   (1u <<  1)   /* 1=clock ungated     */

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

/* ── Task buffer registry ─────────────────────────────────────────
 *
 * Userspace MMZ-allocates the scratch buffers it wants the NNIE block
 * to read/write (model file, tskbuf, tmpbuf), then registers them via
 * NNIE_IOC_ADD_TSKBUF (vendor-equivalent: HI_MPI_SVP_NNIE_AddTskBuf).
 * Kernel needs the (phys → kvirt) mapping to write the variable-length
 * descriptor tail into stTskBuf during Forward dispatch.
 *
 * Vendor uses cmpi_remap_cached (cv500 vendor cmpi module 51 fn ?);
 * for the clean-room driver we just ioremap() the region. ioremap is
 * uncached on ARM, which means we avoid the cache-flush dance vendor
 * has at fill_forward_task @0x94ac. The trade-off is slower kernel
 * writes — acceptable because the tail is small (KB range, written
 * once per Forward call). */
struct nnie_tskbuf {
	struct list_head list;
	u64    phys;
	u64    user_virt;          /* opaque to us — preserved for caller */
	u32    size;
	void  *kvirt;              /* kernel mapping (memremap WB) */
};

static LIST_HEAD(g_nnie_tskbuf_list);
static DEFINE_MUTEX(g_nnie_tskbuf_lock);

static struct nnie_tskbuf *nnie_find_tskbuf_locked(u64 phys)
{
	struct nnie_tskbuf *e;

	list_for_each_entry(e, &g_nnie_tskbuf_list, list) {
		if (e->phys == phys)
			return e;
	}
	return NULL;
}

static int nnie_add_tskbuf(u64 phys, u64 user_virt, u32 size)
{
	struct nnie_tskbuf *e;

	if (!phys || !size)
		return -EINVAL;

	mutex_lock(&g_nnie_tskbuf_lock);
	if (nnie_find_tskbuf_locked(phys)) {
		mutex_unlock(&g_nnie_tskbuf_lock);
		return -EEXIST;
	}

	e = kzalloc(sizeof(*e), GFP_KERNEL);
	if (!e) {
		mutex_unlock(&g_nnie_tskbuf_lock);
		return -ENOMEM;
	}

	e->phys      = phys;
	e->user_virt = user_virt;
	e->size      = size;
	/* memremap WB: works for both CMA-backed kernel RAM (cv500 MMZ
	 * uses this) and MMIO. ioremap would WARN+fail on RAM-backed
	 * regions because the kernel direct map already covers them. */
	e->kvirt     = memremap(phys, size, MEMREMAP_WB);
	if (!e->kvirt) {
		kfree(e);
		mutex_unlock(&g_nnie_tskbuf_lock);
		return -ENOMEM;
	}

	list_add(&e->list, &g_nnie_tskbuf_list);
	mutex_unlock(&g_nnie_tskbuf_lock);
	return 0;
}

static int nnie_remove_tskbuf(u64 phys)
{
	struct nnie_tskbuf *e;

	mutex_lock(&g_nnie_tskbuf_lock);
	e = nnie_find_tskbuf_locked(phys);
	if (!e) {
		mutex_unlock(&g_nnie_tskbuf_lock);
		return -ENOENT;
	}

	list_del(&e->list);
	mutex_unlock(&g_nnie_tskbuf_lock);

	if (e->kvirt)
		memunmap(e->kvirt);
	kfree(e);
	return 0;
}

static void nnie_drain_tskbufs(void)
{
	struct nnie_tskbuf *e, *tmp;

	mutex_lock(&g_nnie_tskbuf_lock);
	list_for_each_entry_safe(e, tmp, &g_nnie_tskbuf_list, list) {
		list_del(&e->list);
		if (e->kvirt)
			memunmap(e->kvirt);
		kfree(e);
	}
	mutex_unlock(&g_nnie_tskbuf_lock);
}

/* ── /dev/nnie character device ─────────────────────────────────── */

static osal_dev_t *g_nnie_dev;
static int g_nnie_irq_requested;
static struct completion g_nnie_done;
static atomic_t g_nnie_last_status;  /* read & cleared by handler, consumed by Forward */

static irqreturn_t nnie_irq_handler(int irq, void *dev)
{
	u32 status;

	/* Shared SPI line with vendor open_gdc.ko (and possibly others) —
	 * read IRQ_STATUS first; if no NNIE bits are pending, return
	 * IRQ_NONE so the next handler in the chain gets a chance. */
	if (!g_nnie_regs)
		return IRQ_NONE;

	status = readl(g_nnie_regs + NNIE_REG_IRQ_STATUS);
	if (!(status & NNIE_IRQ_ALL))
		return IRQ_NONE;

	/* Save the cause for the dispatcher to inspect, then ack. */
	atomic_set(&g_nnie_last_status, status & NNIE_IRQ_ALL);
	writel(status & NNIE_IRQ_ALL, g_nnie_regs + NNIE_REG_IRQ_CLEAR);
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

/* SVP_BLOB_S internal offsets (size 48 B; cross-checked with cv500 ARM
 * toolchain — the union starts at +32 because u64VirAddrStep in stSeq
 * forces 8-byte alignment, leaving a 4-byte hole at +28). */
#define NNIE_BLOB_OFF_STRIDE          4
#define NNIE_BLOB_OFF_VIRADDR         8
#define NNIE_BLOB_OFF_PHYADDR        16
#define NNIE_BLOB_OFF_NUM            24
/* +28..+31: 4 B padding for union alignment */
#define NNIE_BLOB_OFF_WIDTH          32   /* stWhc.u32Width  / stSeq.u32Dim */
#define NNIE_BLOB_OFF_HEIGHT         36   /* stWhc.u32Height               */
#define NNIE_BLOB_OFF_CHN            40   /* stWhc.u32Chn   / stSeq.u64VAStep lo */
#define NNIE_BLOB_OFF_DIM            32   /* alias for stSeq */
#define NNIE_BLOB_OFF_VASTEP         40   /* alias for stSeq */

/* SVP_NNIE_FORWARD_CTRL_S internal offsets (size 64 B) */
#define NNIE_CTRL_OFF_SRCNUM          0
#define NNIE_CTRL_OFF_DSTNUM          4
#define NNIE_CTRL_OFF_NETSEGID        8
#define NNIE_CTRL_OFF_NNIEID         12
#define NNIE_CTRL_OFF_TMP_PHYS       16   /* stTmpBuf.u64PhyAddr */
#define NNIE_CTRL_OFF_TSK_PHYS       40   /* stTskBuf.u64PhyAddr */

static void __maybe_unused
nnie_fill_task_header(struct nnie_hw_task *task,
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

/* Variable-length descriptor tail builder. See nnie_hw_task.h "Variable-
 * length descriptor tail" for the section layout. Handles non-LSTM
 * (net_type != SVP_NNIE_NET_TYPE_RECURRENT) CNN inputs with enType==0;
 * enType ∈ [1..5] paths are stubbed and return -EOPNOTSUPP. */
static int nnie_build_task_tail(struct nnie_tskbuf *tb, const u8 *fwd_arg,
                                u32 src_num, u32 dst_num)
{
	u8 *base = tb->kvirt;
	u32 off = 0;
	u32 i, j;

	#define TIP_PUT_U32(v) do {                                          \
		if (off + 4 > tb->size) return -ENOSPC;                      \
		*(u32 *)(base + off) = (u32)(v); off += 4;                   \
	} while (0)
	#define TIP_PUT_U64(v) do {                                          \
		if (off + 8 > tb->size) return -ENOSPC;                      \
		*(u64 *)(base + off) = (u64)(v); off += 8;                   \
	} while (0)
	#define TIP_ALIGN_16() while (off & 0xF) TIP_PUT_U32(0)

	/* §1: per-input stride table */
	for (i = 0; i < src_num; i++) {
		u32 stride = *(u32 *)(fwd_arg + NNIE_FWD_OFF_ASTSRC +
				      i * NNIE_BLOB_S_SIZE + NNIE_BLOB_OFF_STRIDE);
		TIP_PUT_U32(stride);
	}
	TIP_ALIGN_16();

	/* §2: 16 destination phys addresses, zero-padded past dst_num */
	for (i = 0; i < 16; i++) {
		u64 dst_phys = 0;
		if (i < dst_num)
			dst_phys = *(u64 *)(fwd_arg + NNIE_FWD_OFF_ASTDST +
					    i * NNIE_BLOB_S_SIZE + NNIE_BLOB_OFF_PHYADDR);
		TIP_PUT_U64(dst_phys);
	}

	/* §3: per-source DMA address vector */
	for (i = 0; i < src_num; i++) {
		const u8 *blob = fwd_arg + NNIE_FWD_OFF_ASTSRC + i * NNIE_BLOB_S_SIZE;
		u32 en_type = *(u32 *)(blob + 0);
		u64 phys    = *(u64 *)(blob + NNIE_BLOB_OFF_PHYADDR);
		u32 stride  = *(u32 *)(blob + NNIE_BLOB_OFF_STRIDE);
		u32 num     = *(u32 *)(blob + NNIE_BLOB_OFF_NUM);
		u32 height  = *(u32 *)(blob + NNIE_BLOB_OFF_HEIGHT);
		u32 chn     = *(u32 *)(blob + NNIE_BLOB_OFF_CHN);
		u64 batch_size;

		switch (en_type) {
		case 0:                  /* SVP_BLOB_TYPE_S32 — generic CNN feature map */
			batch_size = (u64)stride * height * chn;
			break;
		case 1:                  /* SVP_BLOB_TYPE_U8 — image input */
			if (chn != 1) {
				/* Chn==3 packed-RGB needs 32 B/batch (3 plane
				 * addrs + zero pad) per vendor svp_nnie_fill_
				 * image_src_addr @0x7a8c. Phase 8 work. */
				pr_info_once("nnie_neo: U8 Chn=%u not yet supported (mnist Chn=1 only)\n",
					     chn);
				return -EOPNOTSUPP;
			}
			batch_size = (u64)stride * height;
			break;
		case 4:                  /* SVP_BLOB_TYPE_VEC_S32 — FC layer vector */
			batch_size = (u64)stride * height;
			break;
		case 2: case 3:          /* YVU420SP / YVU422SP */
		case 5:                  /* SEQ_S32 (LSTM/RNN) */
			pr_info_once("nnie_neo: src[%u] enType=%u not yet supported\n",
				     i, en_type);
			return -EOPNOTSUPP;
		default:
			return -EINVAL;
		}

		for (j = 0; j < num; j++)
			TIP_PUT_U64(phys + j * batch_size);
		TIP_ALIGN_16();
	}

	#undef TIP_PUT_U32
	#undef TIP_PUT_U64
	#undef TIP_ALIGN_16
	return off;   /* bytes used */
}

/* sizeof(SVP_NNIE_MODEL_S) on cv500 ARM — cross-checked in
 * nnie_hw_task.h. Used for the per-call copy_from_user(pstModel). */
#define NNIE_MODEL_SIZE  13992u

/* Forward dispatch: blocks until the NNIE HW signals completion (or
 * 5 s timeout). Caller must have populated the 64-byte descriptor and
 * already written the variable-length tail into the registered
 * stTskBuf. Returns 0 on success, negative errno on HW error /
 * timeout. */
static long nnie_dispatch_forward(const struct nnie_hw_task *task)
{
	void *task_kvirt;
	dma_addr_t task_dma;
	unsigned long completed;
	u32 cause;
	long ret = 0;

	if (!g_nnie_regs || !g_nnie_pf_dev)
		return -ENODEV;

	task_kvirt = dma_alloc_coherent(&g_nnie_pf_dev->dev,
					NNIE_HW_TASK_SIZE,
					&task_dma, GFP_KERNEL);
	if (!task_kvirt)
		return -ENOMEM;

	memcpy(task_kvirt, task, NNIE_HW_TASK_SIZE);

	/* Bring NNIE block out of reset + ungate its clock. CRG register
	 * 0x120100bc holds bit 0 = reset (1=held), bit 1 = clk enable.
	 * Without these, NNIE register writes silently drop. Vendor goes
	 * through cmpi mod 2 (sys_config) fns 0xb3 (reset) and 0xb4
	 * (clk_en); we drive the CRG register directly. */
	nnie_crg_clear_bit(NNIE_CRG_REG_NNIE_CLK, NNIE_SYS_BIT_NNIE_RESET);
	nnie_crg_set_bit  (NNIE_CRG_REG_NNIE_CLK, NNIE_SYS_BIT_NNIE_CLK_EN);

	/* Coordinate with vendor open_sys.ko / open_gdc.ko via the cv500
	 * sys-window RAM-using flag (bit 0 of 0x12020034). Vendor's
	 * equivalent goes through cmpi mod 51 fn 0xd1 (Phase 3 decode). */
	nnie_sys_set_bit(NNIE_SYS_REG_NNIE_RAM, NNIE_SYS_BIT_NNIE_RAM);

	atomic_set(&g_nnie_last_status, 0);
	reinit_completion(&g_nnie_done);

	/* Boot the NNIE block. The CLK_GATE/OUTSTANDING/IRQ_CFG writes
	 * are idempotent and survive across Forward calls — we re-issue
	 * them every time so the block recovers if vendor open_gdc.ko (or
	 * a stale boot state) left them in a weird state. */
	/* Init-time register setup (vendor does this once in svp_nnie_init
	 * @0x10f4; we redo each call for simplicity/idempotence). */
	writel(NNIE_CLK_GATE_EN,         g_nnie_regs + NNIE_REG_CLK_GATE);
	writel(NNIE_OUTSTANDING_DEFAULT, g_nnie_regs + NNIE_REG_OUTSTANDING);
	writel(NNIE_IRQ_ALL,             g_nnie_regs + NNIE_REG_IRQ_CFG);
	/* Vendor sets TIMEOUT to ~2 s at 500 MHz NNIE clock; without it
	 * (TIMEOUT=0) HW seems to never IRQ — possibly stuck waiting on
	 * a timeout counter that never advances. */
	writel(0xFFFFFFFFu,              g_nnie_regs + NNIE_REG_TIMEOUT_LO);
	writel(0x000000FFu,              g_nnie_regs + NNIE_REG_TIMEOUT_HI);
	/* Vendor: disable_check_sum at init — clear bit 0 of +0x68. */
	{
		u32 v = readl(g_nnie_regs + NNIE_REG_CHECK_SUM);
		writel(v & ~NNIE_CHECK_SUM_EN, g_nnie_regs + NNIE_REG_CHECK_SUM);
	}
	writel((u32)task_dma,            g_nnie_regs + NNIE_REG_TASK_ADDR_LO);
	writel((u32)((u64)task_dma >> 32), g_nnie_regs + NNIE_REG_TASK_ADDR_HI);
	wmb();
	writel(NNIE_START_GO,            g_nnie_regs + NNIE_REG_START);

	completed = wait_for_completion_timeout(&g_nnie_done, 5 * HZ);
	cause = atomic_xchg(&g_nnie_last_status, 0);

	nnie_sys_clear_bit(NNIE_SYS_REG_NNIE_RAM, NNIE_SYS_BIT_NNIE_RAM);

	dma_free_coherent(&g_nnie_pf_dev->dev, NNIE_HW_TASK_SIZE,
			  task_kvirt, task_dma);

	if (!completed) {
		u32 live_status, live_start, live_task_id, live_clk;

		live_status  = readl(g_nnie_regs + NNIE_REG_IRQ_STATUS);
		live_start   = readl(g_nnie_regs + NNIE_REG_START);
		live_task_id = readl(g_nnie_regs + NNIE_REG_TASK_ID);
		live_clk     = readl(g_nnie_regs + NNIE_REG_CLK_GATE);
		pr_warn("nnie_neo: Forward timed out (5s)  cause_snap=0x%x  "
			"STATUS=0x%x START=0x%x TASK_ID=0x%x CLK_GATE=0x%x\n",
			cause, live_status, live_start, live_task_id, live_clk);
		return -ETIMEDOUT;
	}
	if (cause & NNIE_IRQ_CFG_ERR) {
		u32 info = readl(g_nnie_regs + NNIE_REG_CFG_ERR_INFO);

		pr_warn("nnie_neo: Forward cfg_err (cause=0x%x info=0x%x)\n",
			cause, info);
		ret = -EIO;
	} else if (cause & NNIE_IRQ_TIMEOUT) {
		pr_warn("nnie_neo: Forward HW timeout (cause=0x%x)\n", cause);
		ret = -ETIMEDOUT;
	} else if (!(cause & NNIE_IRQ_FINISH)) {
		pr_warn("nnie_neo: Forward woke without finish bit (cause=0x%x)\n",
			cause);
		ret = -EIO;
	}
	return ret;
}

static long nnie_op_forward(unsigned long arg, int with_bbox)
{
	const u32 arg_size = with_bbox ?
		NNIE_FORWARD_BBOX_ARG_SIZE : NNIE_FORWARD_ARG_SIZE;
	u8 *buf = (u8 *)arg;
	u32 src_num, dst_num, net_seg_id, instant, batch_num;
	u64 tsk_phys, tmp_phys, model_uva;
	u64 model_phys = 0;
	u32 inst_off = 0, inst_len = 0;
	u32 net_seg_num;
	u8 *model_kbuf;
	struct nnie_tskbuf *tb;
	int tail_bytes = -ENOENT;
	struct nnie_hw_task task;

	if (!buf)
		return -EINVAL;

	src_num    = *(u32 *)(buf + NNIE_FWD_OFF_CTRL + NNIE_CTRL_OFF_SRCNUM);
	dst_num    = *(u32 *)(buf + NNIE_FWD_OFF_CTRL + NNIE_CTRL_OFF_DSTNUM);
	net_seg_id = *(u32 *)(buf + NNIE_FWD_OFF_CTRL + NNIE_CTRL_OFF_NETSEGID);
	tmp_phys   = *(u64 *)(buf + NNIE_FWD_OFF_CTRL + NNIE_CTRL_OFF_TMP_PHYS);
	tsk_phys   = *(u64 *)(buf + NNIE_FWD_OFF_CTRL + NNIE_CTRL_OFF_TSK_PHYS);
	instant    = *(u32 *)(buf + NNIE_FWD_OFF_INSTANT);
	batch_num  = *(u32 *)(buf + NNIE_FWD_OFF_ASTSRC + NNIE_BLOB_OFF_NUM);
	model_uva  = *(u64 *)(buf + NNIE_FWD_OFF_PSTMODEL_UVA);

	pr_info_once("nnie_neo: Forward%s arg=%u B  SrcNum=%u DstNum=%u NetSegId=%u  Instant=%u  Batch=%u\n",
		     with_bbox ? "WithBbox" : "",
		     arg_size, src_num, dst_num, net_seg_id, instant, batch_num);
	pr_info_once("nnie_neo:   ctrl.stTskBuf.phys=0x%llx  stTmpBuf.phys=0x%llx  pstModel.uva=0x%llx\n",
		     tsk_phys, tmp_phys, model_uva);

	if (!model_uva || src_num == 0 || dst_num == 0 || src_num > 16 || dst_num > 16)
		return -EINVAL;
	if (net_seg_id >= 8)
		return -EINVAL;

	/* copy_from_user the full SVP_NNIE_MODEL_S so we can read
	 *   - stBase.u64PhyAddr     (file[+0x3690])  → task[+16]
	 *   - astSeg[net_seg_id].u32InstOffset (file[+12 + seg*1692 + 12])
	 *   - astSeg[net_seg_id].u32InstLen    (file[+12 + seg*1692 + 16])
	 * vmalloc because 13992 B is close to kmalloc's default-slab cliff. */
	model_kbuf = vmalloc(NNIE_MODEL_SIZE);
	if (!model_kbuf)
		return -ENOMEM;

	if (copy_from_user(model_kbuf, (void __user *)(uintptr_t)model_uva,
			   NNIE_MODEL_SIZE)) {
		vfree(model_kbuf);
		return -EFAULT;
	}

	net_seg_num = *(u32 *)(model_kbuf + 8);   /* MODEL_S.u32NetSegNum */
	if (net_seg_id >= net_seg_num) {
		pr_warn("nnie_neo: net_seg_id=%u >= u32NetSegNum=%u\n",
			net_seg_id, net_seg_num);
		vfree(model_kbuf);
		return -EINVAL;
	}

	model_phys = *(u64 *)(model_kbuf + NNIE_MODEL_STBASE_OFFSET);
	inst_off   = *(u32 *)(model_kbuf + 12 + net_seg_id * NNIE_SEG_S_STRIDE + 12);
	inst_len   = *(u32 *)(model_kbuf + 12 + net_seg_id * NNIE_SEG_S_STRIDE + 16);
	vfree(model_kbuf);

	/* Build the variable-length tail into the registered stTskBuf.
	 * (Failure here is fatal — HW reads §3 to know input/output phys
	 * addresses, so without the tail the inference can't run.) */
	mutex_lock(&g_nnie_tskbuf_lock);
	tb = nnie_find_tskbuf_locked(tsk_phys);
	if (tb)
		tail_bytes = nnie_build_task_tail(tb, buf, src_num, dst_num);
	mutex_unlock(&g_nnie_tskbuf_lock);

	if (!tb) {
		pr_warn("nnie_neo: stTskBuf 0x%llx not registered via AddTskBuf\n",
			tsk_phys);
		return -ENOENT;
	}
	if (tail_bytes < 0) {
		pr_warn("nnie_neo: tail builder failed: %d\n", tail_bytes);
		return tail_bytes;
	}

	/* Fill the 64-byte HW task descriptor (still on stack; Phase 7
	 * will copy this into a dma_alloc_coherent buffer and pass that
	 * phys to the NNIE START register). */
	nnie_fill_task_header(&task, model_phys, inst_off, inst_len,
			      tsk_phys, tmp_phys, batch_num, !!instant);

	pr_info_once("nnie_neo:   task hdr: model_phys=0x%llx inst_off=%u inst_len=%u tail=%d B\n",
		     model_phys, inst_off, inst_len, tail_bytes);

	{
		long disp_ret = nnie_dispatch_forward(&task);

		if (disp_ret) {
			pr_warn_ratelimited("nnie_neo: HW dispatch failed: %ld\n",
					    disp_ret);
			return disp_ret;
		}
	}

	*(u32 *)(buf + NNIE_FWD_OFF_HANDLE) = 0;
	return 0;
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
 * u32 size, u32 pad}. Kernel tracks the buffers so Forward dispatch
 * has a kvirt mapping to write the variable-length descriptor tail
 * into stTskBuf (Phase 5). */
static long nnie_op_add_tskbuf(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u64 phys, virt;
	u32 size;
	int ret;

	if (!buf)
		return -EINVAL;
	phys = *(u64 *)(buf +  0);
	virt = *(u64 *)(buf +  8);
	size = *(u32 *)(buf + 16);

	ret = nnie_add_tskbuf(phys, virt, size);
	pr_info_once("nnie_neo: AddTskBuf phys=0x%llx virt=0x%llx size=%u -> %d\n",
		     phys, virt, size, ret);
	return ret;
}

static long nnie_op_remove_tskbuf(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u64 phys = buf ? *(u64 *)(buf + 0) : 0;
	int ret = nnie_remove_tskbuf(phys);

	pr_info_once("nnie_neo: RemoveTskBuf phys=0x%llx -> %d\n", phys, ret);
	return ret;
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
	spin_lock_init(&g_crg_lock);

	/* Map the cv500 CRG (clock-reset generator) window so we can ungate
	 * the NNIE clock + release reset before HW dispatch. Vendor open_sys
	 * also owns this window; ioremap shares it non-exclusively. */
	g_crg_regs = ioremap(NNIE_CRG_BASE_PHYS, NNIE_CRG_WINDOW_SIZE);
	if (g_crg_regs) {
		u32 nclk = readl(g_crg_regs + NNIE_CRG_REG_NNIE_CLK);
		pr_info("nnie_neo: crg @0x%lx mapped — NNIE_CLK[0xbc]=0x%08x\n",
			NNIE_CRG_BASE_PHYS, nclk);
	} else {
		pr_warn("nnie_neo: failed to ioremap CRG @0x%lx\n",
			NNIE_CRG_BASE_PHYS);
	}

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
		/* IRQF_SHARED: vendor open_gdc.ko on the same SPI line uses
		 * IRQF_SHARED (kprobed from its request_irq call), so we have
		 * to match. The dev_id (&g_nnie_dev) must be unique per
		 * registered handler — sharing dev_id with another module
		 * would make IRQF_SHARED reject the registration. */
		ret = request_irq(g_nnie_irq, nnie_irq_handler, IRQF_SHARED,
				  "nnie_neo", &g_nnie_dev);
		if (ret)
			pr_err("nnie_neo: request_irq(%u) failed: %d\n",
			       g_nnie_irq, ret);
		else
			g_nnie_irq_requested = 1;
	}

	pr_info("nnie_neo: /dev/nnie ready (Phase 7 — full Forward path wired, IRQF_SHARED)\n");
	return 0;
}

void nnie_std_mod_exit(void)
{
	nnie_drain_tskbufs();

	if (g_nnie_irq_requested) {
		free_irq(g_nnie_irq, &g_nnie_dev);
		g_nnie_irq_requested = 0;
	}
	if (g_sys_regs) {
		iounmap(g_sys_regs);
		g_sys_regs = NULL;
	}
	if (g_crg_regs) {
		iounmap(g_crg_regs);
		g_crg_regs = NULL;
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
