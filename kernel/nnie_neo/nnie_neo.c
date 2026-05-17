// SPDX-License-Identifier: GPL-2.0
/*
 * nnie_neo — open-source replacement for vendor open_nnie.ko on cv500/
 * av300/dv300 SoCs. Drives the NNIE CNN inference block at phys
 * 0x11100000 (distinct from the IVE block at 0x11230000) and exposes
 * the vendor-compatible ABI on /dev/nnie, so existing userspace using
 * libnnie.so works unchanged.
 *
 * Userspace consumer: libraries/nnie_neo/libnnie_neo.so.
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
#include <linux/delay.h>
#include <asm/cacheflush.h>

#include "hi_osal.h"
#include "hi_common.h"
#include "osal_mmz.h"

#include "nnie_hw_task.h"
#include "nnie_hw_regs.h"

/* ── Platform state, populated by nnie_init.c ─────────────────────── */

void *g_nnie_regs;
void *g_gdc_regs;
unsigned int g_nnie_irq;
unsigned int g_gdc_irq;
struct platform_device *g_nnie_pf_dev;

EXPORT_SYMBOL(g_nnie_regs);
EXPORT_SYMBOL(g_gdc_regs);
EXPORT_SYMBOL(g_nnie_irq);
EXPORT_SYMBOL(g_gdc_irq);
EXPORT_SYMBOL(g_nnie_pf_dev);

/* ── cv500 sys / CRG MMIO windows ─────────────────────────────────── */

#define NNIE_SYS2_BASE_PHYS        0x12030000UL
#define NNIE_SYS2_WINDOW_SIZE      0x1000
#define NNIE_SYS2_REG_NNIE_RAM     0x0034   /* bit 0 = NNIE owns shared SRAM */

#define NNIE_CRG_BASE_PHYS         0x12010000UL
#define NNIE_CRG_WINDOW_SIZE       0x1000
#define NNIE_CRG_REG_NNIE_CLK      0x00BC   /* bit 0=reset, bit 1=clk_en */
#define NNIE_CRG_REG_VEDU_CLK      0x00A4   /* shared with NNIE on cv500   */

#define NNIE_SYS_BIT_NNIE_RAM      (1u << 0)
#define NNIE_SYS_BIT_NNIE_RESET    (1u << 0)
#define NNIE_SYS_BIT_NNIE_CLK_EN   (1u << 1)

static void __iomem *g_crg_regs;
static void __iomem *g_sys2_regs;
static spinlock_t    g_crg_lock;
static spinlock_t    g_sys2_lock;

/* Task descriptor MMZ, allocated once at module init. The vendor driver
 * does the same — keeping a single ring of descriptors avoids per-task
 * MMZ churn and gives HW a stable TASK_ADDR. */
static hil_mmb_t *g_nnie_desc_mmb;
static void      *g_nnie_desc_kvirt;
static u64        g_nnie_desc_phys;

static void nnie_crg_set_bit(u32 reg_off, u32 bit)
{
	unsigned long flags;
	u32 v;

	if (!g_crg_regs)
		return;
	spin_lock_irqsave(&g_crg_lock, flags);
	v = readl(g_crg_regs + reg_off);
	writel(v | bit, g_crg_regs + reg_off);
	spin_unlock_irqrestore(&g_crg_lock, flags);
}

static void nnie_crg_clear_bit(u32 reg_off, u32 bit)
{
	unsigned long flags;
	u32 v;

	if (!g_crg_regs)
		return;
	spin_lock_irqsave(&g_crg_lock, flags);
	v = readl(g_crg_regs + reg_off);
	writel(v & ~bit, g_crg_regs + reg_off);
	spin_unlock_irqrestore(&g_crg_lock, flags);
}

static void nnie_sys2_clear_bit(u32 reg_off, u32 bit)
{
	unsigned long flags;
	u32 v;

	if (!g_sys2_regs)
		return;
	spin_lock_irqsave(&g_sys2_lock, flags);
	v = readl(g_sys2_regs + reg_off);
	writel(v & ~bit, g_sys2_regs + reg_off);
	spin_unlock_irqrestore(&g_sys2_lock, flags);
}

/* ── Task-buffer registry ─────────────────────────────────────────
 *
 * Userspace MMZ-allocates the scratch buffer it wants the HW to read
 * the variable-length descriptor tail from (stTskBuf in vendor terms),
 * then registers it via NNIE_IOC_ADD_TSKBUF. We keep a (phys → kvirt)
 * mapping so Forward can write the tail into it without touching
 * userspace. */
struct nnie_tskbuf {
	struct list_head list;
	u64    phys;
	u64    user_virt;       /* opaque, preserved for the caller */
	u32    size;
	void  *kvirt;
};

static LIST_HEAD(g_nnie_tskbuf_list);
static DEFINE_MUTEX(g_nnie_tskbuf_lock);

/* Serialize Forward calls: the descriptor MMZ, completion, and cause
 * atomic are single-instance globals. */
static DEFINE_MUTEX(g_nnie_forward_lock);

static struct nnie_tskbuf *nnie_find_tskbuf_locked(u64 phys)
{
	struct nnie_tskbuf *e;

	list_for_each_entry(e, &g_nnie_tskbuf_list, list)
		if (e->phys == phys)
			return e;
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
	/* memremap WB works for both CMA-backed RAM (cv500 MMZ uses this)
	 * and MMIO; ioremap WARNs+fails on RAM-backed regions. */
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
static atomic_t g_nnie_last_status;

static irqreturn_t nnie_irq_handler(int irq, void *dev)
{
	u32 status;

	/* SPI line is shared with vendor open_gdc.ko; check NNIE status
	 * first and return IRQ_NONE if no NNIE bits are pending. */
	if (!g_nnie_regs)
		return IRQ_NONE;

	status = readl(g_nnie_regs + NNIE_REG_IRQ_STATUS);
	if (!(status & NNIE_IRQ_ALL))
		return IRQ_NONE;

	atomic_set(&g_nnie_last_status, status & NNIE_IRQ_ALL);
	writel(status & NNIE_IRQ_ALL, g_nnie_regs + NNIE_REG_IRQ_CLEAR);
	complete(&g_nnie_done);
	return IRQ_HANDLED;
}

/* ── ioctl ABI (matches vendor libnnie.so on cv500) ────────────────
 *
 *   nr | size | full ioctl  | userspace entry
 *   ---+------+-------------+--------------------------
 *  0x00| 1624 | 0xc6584d00  | HI_MPI_SVP_NNIE_Forward
 *  0x01| 1728 | 0xc6c04d01  | HI_MPI_SVP_NNIE_ForwardWithBbox
 *  0x02|   24 | 0xc0184d02  | HI_MPI_SVP_NNIE_Query
 *  0x03|   24 | 0xc0184d03  | HI_MPI_SVP_NNIE_AddTskBuf
 *  0x04|   24 | 0xc0184d04  | HI_MPI_SVP_NNIE_RemoveTskBuf
 */
#define NNIE_IOC_FORWARD              0xc6584d00u
#define NNIE_IOC_FORWARD_WITH_BBOX    0xc6c04d01u
#define NNIE_IOC_QUERY                0xc0184d02u
#define NNIE_IOC_ADD_TSKBUF           0xc0184d03u
#define NNIE_IOC_REMOVE_TSKBUF        0xc0184d04u

static long nnie_op_forward(unsigned long arg);
static long nnie_op_forward_with_bbox(unsigned long arg);
static long nnie_op_query(unsigned long arg);
static long nnie_op_add_tskbuf(unsigned long arg);
static long nnie_op_remove_tskbuf(unsigned long arg);

static long nnie_dispatch(unsigned int cmd, unsigned long arg)
{
	switch (cmd) {
	case NNIE_IOC_FORWARD:           return nnie_op_forward(arg);
	case NNIE_IOC_FORWARD_WITH_BBOX: return nnie_op_forward_with_bbox(arg);
	case NNIE_IOC_QUERY:             return nnie_op_query(arg);
	case NNIE_IOC_ADD_TSKBUF:        return nnie_op_add_tskbuf(arg);
	case NNIE_IOC_REMOVE_TSKBUF:     return nnie_op_remove_tskbuf(arg);
	default:                         return -ENOIOCTLCMD;
	}
}

/* ── Forward arg buffer (1624 B) layout ───────────────────────────
 *
 *   off   | size | content
 *   ------+------+----------------------------------------------------
 *      0  |   4  | HI_HANDLE (out — kernel writes assigned handle)
 *      4  |   4  | pad
 *      8  | 768  | astSrc[16]  — 16 × SVP_BLOB_S, 48 B each
 *    776  |   8  | pstModel user virt (read by kernel for stBase + segs)
 *    784  | 768  | astDst[16]
 *   1552  |  64  | SVP_NNIE_FORWARD_CTRL_S
 *   1616  |   4  | bInstant
 *   1620  |   4  | pad
 */
#define NNIE_FORWARD_ARG_SIZE       1624u
#define NNIE_FORWARD_BBOX_ARG_SIZE  1728u
#define NNIE_BLOB_S_SIZE            48u

#define NNIE_FWD_OFF_HANDLE         0
#define NNIE_FWD_OFF_ASTSRC         8
#define NNIE_FWD_OFF_PSTMODEL_UVA   776
#define NNIE_FWD_OFF_ASTDST         784
#define NNIE_FWD_OFF_CTRL           1552
#define NNIE_FWD_OFF_INSTANT        1616

/* SVP_BLOB_S internal offsets (48 B; the union starts at +32 because
 * u64VirAddrStep in stSeq forces 8-byte alignment, leaving a 4-byte
 * hole at +28). */
#define NNIE_BLOB_OFF_STRIDE        4
#define NNIE_BLOB_OFF_PHYADDR       16
#define NNIE_BLOB_OFF_NUM           24
#define NNIE_BLOB_OFF_WIDTH         32   /* stWhc.u32Width */
#define NNIE_BLOB_OFF_HEIGHT        36   /* stWhc.u32Height */
#define NNIE_BLOB_OFF_CHN           40   /* stWhc.u32Chn   */

/* SVP_NNIE_FORWARD_CTRL_S internal offsets (64 B) */
#define NNIE_CTRL_OFF_SRCNUM        0
#define NNIE_CTRL_OFF_DSTNUM        4
#define NNIE_CTRL_OFF_NETSEGID      8
#define NNIE_CTRL_OFF_TMP_PHYS      16
#define NNIE_CTRL_OFF_TSK_PHYS      40
#define NNIE_CTRL_OFF_TSK_SIZE      56

/* Monotonic task index — must increment per Forward and wrap at 512
 * so HW can map each task to a ring slot. Starting at 0 matches the
 * vendor first-task value. */
static atomic_t g_nnie_task_idx;

static void nnie_fill_task_header(struct nnie_hw_task *task,
				  u64 model_phys, u32 inst_off, u32 inst_len,
				  u64 tsk_phys, u64 tmp_phys, u32 batch_num,
				  bool instant)
{
	u32 slot = (atomic_inc_return(&g_nnie_task_idx) - 1) & 0x1ff;

	memset(task, 0, sizeof(*task));
	task->trigger_mode    = cpu_to_le16(instant ? 1 : 0);
	task->slot_idx        = cpu_to_le32(slot);
	task->model_file_phys = cpu_to_le64(model_phys);
	task->seg_inst_offset = cpu_to_le32(inst_off);
	task->seg_inst_len    = cpu_to_le32(inst_len);
	task->tsk_buf_phys    = cpu_to_le64(tsk_phys);
	task->tmp_buf_phys    = cpu_to_le64(tmp_phys);
	task->src_batch_num   = cpu_to_le32(batch_num);
}

/* Build the variable-length descriptor tail into the registered
 * stTskBuf. HW follows tsk_buf_phys to read this tail; see
 * nnie_hw_task.h for the layout. Returns the number of bytes written
 * or a negative errno. */
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

	for (i = 0; i < src_num; i++) {
		u32 stride = *(u32 *)(fwd_arg + NNIE_FWD_OFF_ASTSRC +
				      i * NNIE_BLOB_S_SIZE + NNIE_BLOB_OFF_STRIDE);
		TIP_PUT_U32(stride);
	}
	for (i = 0; i < dst_num; i++) {
		u32 stride = *(u32 *)(fwd_arg + NNIE_FWD_OFF_ASTDST +
				      i * NNIE_BLOB_S_SIZE + NNIE_BLOB_OFF_STRIDE);
		TIP_PUT_U32(stride);
	}
	TIP_ALIGN_16();

	for (i = 0; i < dst_num; i++) {
		u64 dst_phys = *(u64 *)(fwd_arg + NNIE_FWD_OFF_ASTDST +
					i * NNIE_BLOB_S_SIZE + NNIE_BLOB_OFF_PHYADDR);
		TIP_PUT_U64(dst_phys);
	}
	TIP_ALIGN_16();

	for (i = 0; i < src_num; i++) {
		const u8 *blob = fwd_arg + NNIE_FWD_OFF_ASTSRC + i * NNIE_BLOB_S_SIZE;
		u32 en_type = *(u32 *)(blob + 0);
		u64 phys    = *(u64 *)(blob + NNIE_BLOB_OFF_PHYADDR);
		u32 stride  = *(u32 *)(blob + NNIE_BLOB_OFF_STRIDE);
		u32 num     = *(u32 *)(blob + NNIE_BLOB_OFF_NUM);
		u32 height  = *(u32 *)(blob + NNIE_BLOB_OFF_HEIGHT);
		u32 chn     = *(u32 *)(blob + NNIE_BLOB_OFF_CHN);
		u64 plane_size = (u64)stride * height;

		/* en_type encoding matches SVP_BLOB_TYPE_E:
		 *   0 = S32 (4 B per pixel; HW expects one phys per frame)
		 *   1 = U8  image (chn=1 → 1 entry per frame; chn=3 → vendor
		 *                  emits a 4-u64 quartet per frame for the
		 *                  three planar channels + one zero slot)
		 *   4 = VEC_S32 vector (one entry per frame)
		 *
		 * Vendor svp_nnie_fill_image_src_addr (hi_nnie.o:0x78ac)
		 * encodes this; see kaeru node
		 * `nnie-neo-cv500-detector-tskbuf-pattern-2026-05-17`. */
		switch (en_type) {
		case 0: /* S32 frame */
			for (j = 0; j < num; j++)
				TIP_PUT_U64(phys + j * plane_size * chn);
			break;
		case 1: /* U8 image */
			if (chn == 1) {
				for (j = 0; j < num; j++)
					TIP_PUT_U64(phys + j * plane_size);
			} else if (chn == 3) {
				for (j = 0; j < num; j++) {
					TIP_PUT_U64(phys + (3 * j + 0) * plane_size);
					TIP_PUT_U64(phys + (3 * j + 1) * plane_size);
					TIP_PUT_U64(phys + (3 * j + 2) * plane_size);
					TIP_PUT_U64(0);
				}
			} else {
				return -EOPNOTSUPP;
			}
			break;
		case 4: /* VEC_S32 */
			for (j = 0; j < num; j++)
				TIP_PUT_U64(phys + j * plane_size);
			break;
		default:
			return -EOPNOTSUPP;
		}
	}
	TIP_ALIGN_16();

	#undef TIP_PUT_U32
	#undef TIP_PUT_U64
	#undef TIP_ALIGN_16
	return off;
}

#define NNIE_MODEL_SIZE  13992u  /* sizeof(SVP_NNIE_MODEL_S) on cv500 */

/* Kick the HW with a filled descriptor, then block on the IRQ. */
static long nnie_dispatch_forward(const struct nnie_hw_task *task)
{
	void *task_kvirt;
	unsigned long task_dma;
	unsigned long completed;
	u32 cause;
	long ret = 0;

	if (!g_nnie_regs || !g_nnie_pf_dev)
		return -ENODEV;

	if (mutex_lock_interruptible(&g_nnie_forward_lock))
		return -ERESTARTSYS;

	if (!g_nnie_desc_kvirt) {
		mutex_unlock(&g_nnie_forward_lock);
		return -ENOMEM;
	}
	task_kvirt = g_nnie_desc_kvirt;
	task_dma   = g_nnie_desc_phys;

	memcpy(task_kvirt, task, NNIE_HW_TASK_SIZE);
	wmb();

	/* Clock + SRAM-ownership sequence. The off/on toggle is an HW
	 * reset pulse; without it the NNIE block stays in some power-on
	 * state that doesn't accept tasks. The NNIE_RAM bit must stay 0
	 * from here through START — it signals "NNIE owns the shared
	 * SRAM", and toggling it back to 1 invalidates ownership. */
	nnie_crg_set_bit  (NNIE_CRG_REG_NNIE_CLK, NNIE_SYS_BIT_NNIE_CLK_EN);
	nnie_crg_clear_bit(NNIE_CRG_REG_NNIE_CLK, NNIE_SYS_BIT_NNIE_RESET);
	nnie_crg_clear_bit(NNIE_CRG_REG_NNIE_CLK, NNIE_SYS_BIT_NNIE_CLK_EN);
	udelay(1);
	nnie_crg_set_bit  (NNIE_CRG_REG_NNIE_CLK, NNIE_SYS_BIT_NNIE_CLK_EN);
	nnie_sys2_clear_bit(NNIE_SYS2_REG_NNIE_RAM, NNIE_SYS_BIT_NNIE_RAM);
	nnie_crg_set_bit  (NNIE_CRG_REG_NNIE_CLK, NNIE_SYS_BIT_NNIE_CLK_EN);

	/* NNIE shares VEDU clock infrastructure on cv500; CRG +0xa4 low
	 * 3 bits must be 0x6 or HW returns cfg_err. */
	if (g_crg_regs) {
		u32 v = readl(g_crg_regs + NNIE_CRG_REG_VEDU_CLK);
		if ((v & 0x7) != 0x6)
			writel((v & ~0x7u) | 0x6u, g_crg_regs + NNIE_CRG_REG_VEDU_CLK);
	}

	atomic_set(&g_nnie_last_status, 0);
	reinit_completion(&g_nnie_done);

	/* Per-task NNIE register setup, all RMW to preserve HW-set bits
	 * in the upper halves of CLK_GATE / OUTSTANDING. */
	{
		u32 v;

		v = readl(g_nnie_regs + NNIE_REG_CLK_GATE);
		writel(v | NNIE_CLK_GATE_EN, g_nnie_regs + NNIE_REG_CLK_GATE);

		v = readl(g_nnie_regs + NNIE_REG_OUTSTANDING);
		writel((v & ~0x1Fu) | 0x0Fu | 0xF00u,
		       g_nnie_regs + NNIE_REG_OUTSTANDING);

		v = readl(g_nnie_regs + NNIE_REG_IRQ_CFG);
		writel(v | NNIE_IRQ_ALL, g_nnie_regs + NNIE_REG_IRQ_CFG);
	}

	/* ~2 s at 500 MHz NNIE clock. Without a non-zero timeout HW
	 * stalls waiting on a counter that never advances. */
	writel(0xFFFFFFFFu, g_nnie_regs + NNIE_REG_TIMEOUT_LO);
	writel(0x000000FFu, g_nnie_regs + NNIE_REG_TIMEOUT_HI);

	/* Disable check_sum via BFC bit 0 — plain writel(0) would clobber
	 * other config bits in the upper half. */
	{
		u32 v = readl(g_nnie_regs + NNIE_REG_CHECK_SUM);
		writel(v & ~NNIE_CHECK_SUM_EN, g_nnie_regs + NNIE_REG_CHECK_SUM);
	}

	/* W1C any stale pending IRQ bits. */
	{
		u32 v = readl(g_nnie_regs + NNIE_REG_IRQ_CLEAR);
		writel(v | NNIE_IRQ_ALL, g_nnie_regs + NNIE_REG_IRQ_CLEAR);
	}

	writel((u32)task_dma,              g_nnie_regs + NNIE_REG_TASK_ADDR_LO);
	writel((u32)((u64)task_dma >> 32), g_nnie_regs + NNIE_REG_TASK_ADDR_HI);
	wmb();

	{
		u32 v = readl(g_nnie_regs + NNIE_REG_START);
		writel(v | NNIE_START_GO, g_nnie_regs + NNIE_REG_START);
	}

	completed = wait_for_completion_timeout(&g_nnie_done, 5 * HZ);
	cause = atomic_xchg(&g_nnie_last_status, 0);

	nnie_sys2_clear_bit(NNIE_SYS2_REG_NNIE_RAM, NNIE_SYS_BIT_NNIE_RAM);

	if (!completed)
		ret = -ETIMEDOUT;
	else if (cause & NNIE_IRQ_CFG_ERR) {
		pr_warn_ratelimited("nnie_neo: cfg_err info=0x%x\n",
				    readl(g_nnie_regs + NNIE_REG_CFG_ERR_INFO));
		ret = -EIO;
	} else if (cause & NNIE_IRQ_TIMEOUT)
		ret = -ETIMEDOUT;
	else if (!(cause & NNIE_IRQ_FINISH))
		ret = -EIO;

	mutex_unlock(&g_nnie_forward_lock);
	return ret;
}

static long nnie_op_forward(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u32 src_num, dst_num, net_seg_id, instant, batch_num, net_seg_num;
	u64 tsk_phys, tmp_phys, model_uva, model_phys;
	u32 inst_off, inst_len, tsk_size;
	u8 *model_kbuf;
	struct nnie_tskbuf *tb;
	int tail_bytes;
	struct nnie_hw_task task;
	long ret;

	if (!buf)
		return -EINVAL;

	src_num    = *(u32 *)(buf + NNIE_FWD_OFF_CTRL + NNIE_CTRL_OFF_SRCNUM);
	dst_num    = *(u32 *)(buf + NNIE_FWD_OFF_CTRL + NNIE_CTRL_OFF_DSTNUM);
	net_seg_id = *(u32 *)(buf + NNIE_FWD_OFF_CTRL + NNIE_CTRL_OFF_NETSEGID);
	tmp_phys   = *(u64 *)(buf + NNIE_FWD_OFF_CTRL + NNIE_CTRL_OFF_TMP_PHYS);
	tsk_phys   = *(u64 *)(buf + NNIE_FWD_OFF_CTRL + NNIE_CTRL_OFF_TSK_PHYS);
	tsk_size   = *(u32 *)(buf + NNIE_FWD_OFF_CTRL + NNIE_CTRL_OFF_TSK_SIZE);
	instant    = *(u32 *)(buf + NNIE_FWD_OFF_INSTANT);
	batch_num  = *(u32 *)(buf + NNIE_FWD_OFF_ASTSRC + NNIE_BLOB_OFF_NUM);
	model_uva  = *(u64 *)(buf + NNIE_FWD_OFF_PSTMODEL_UVA);

	if (!model_uva ||
	    src_num == 0 || src_num > 16 ||
	    dst_num == 0 || dst_num > 16 ||
	    net_seg_id >= 8)
		return -EINVAL;

	/* Copy the full SVP_NNIE_MODEL_S to kernel space to read
	 *   stBase.u64PhyAddr           (file[+0x3690])  → task[+16]
	 *   astSeg[seg].u32InstOffset   → task[+24]
	 *   astSeg[seg].u32InstLen      → task[+28]
	 */
	model_kbuf = vmalloc(NNIE_MODEL_SIZE);
	if (!model_kbuf)
		return -ENOMEM;

	if (copy_from_user(model_kbuf, (void __user *)(uintptr_t)model_uva,
			   NNIE_MODEL_SIZE)) {
		vfree(model_kbuf);
		return -EFAULT;
	}

	net_seg_num = *(u32 *)(model_kbuf + 8);
	if (net_seg_id >= net_seg_num) {
		vfree(model_kbuf);
		return -EINVAL;
	}

	model_phys = *(u64 *)(model_kbuf + NNIE_MODEL_STBASE_OFFSET);
	inst_off   = *(u32 *)(model_kbuf + 12 + net_seg_id * NNIE_SEG_S_STRIDE + 12);
	inst_len   = *(u32 *)(model_kbuf + 12 + net_seg_id * NNIE_SEG_S_STRIDE + 16);
	vfree(model_kbuf);

	/* Build the tail into the tskbuf and flush it: cached mapping
	 * means CPU stores hit L1/L2, but HW reads via DMA.
	 *
	 * Vendor libnnie has two patterns for the tskbuf:
	 *   - mnist-style: AddTskBuf called explicitly before Forward.
	 *   - SSD/detector-style: libnnie allocates an MMZ block and
	 *     references it from CTRL.{TskPhys,TskSize} without an
	 *     AddTskBuf round-trip — the kernel is expected to memremap
	 *     it on demand. We cover both by lazy-adding when the lookup
	 *     misses; the entry stays cached until module rmmod
	 *     (nnie_drain_tskbufs). */
	mutex_lock(&g_nnie_tskbuf_lock);
	tb = nnie_find_tskbuf_locked(tsk_phys);
	mutex_unlock(&g_nnie_tskbuf_lock);
	if (!tb) {
		if (!tsk_size) {
			pr_warn_ratelimited("nnie_neo: tskbuf phys=%llx not registered and CTRL.TskSize=0\n",
					    (unsigned long long)tsk_phys);
			return -ENOENT;
		}
		ret = nnie_add_tskbuf(tsk_phys, 0, tsk_size);
		if (ret && ret != -EEXIST)
			return ret;
		mutex_lock(&g_nnie_tskbuf_lock);
		tb = nnie_find_tskbuf_locked(tsk_phys);
		mutex_unlock(&g_nnie_tskbuf_lock);
		if (!tb)
			return -ENOENT;
	}
	mutex_lock(&g_nnie_tskbuf_lock);
	tail_bytes = nnie_build_task_tail(tb, buf, src_num, dst_num);
	if (tail_bytes > 0)
		__cpuc_flush_dcache_area(tb->kvirt, tb->size);
	mutex_unlock(&g_nnie_tskbuf_lock);

	if (tail_bytes < 0)
		return tail_bytes;

	nnie_fill_task_header(&task, model_phys, inst_off, inst_len,
			      tsk_phys, tmp_phys, batch_num, !!instant);

	ret = nnie_dispatch_forward(&task);
	if (ret)
		return ret;

	*(u32 *)(buf + NNIE_FWD_OFF_HANDLE) = 0;
	return 0;
}

/* ForwardWithBbox (RPN/SSD detection-net second stage):
 *
 *   The 1728 B user buffer has a different layout from Forward:
 *   astBbox[2] inserted at +776, pstModel at +872, astDst at +880,
 *   CTRL_S (72 B) at +1648, bInstant at +1720. The 64 B HW
 *   descriptor is identical to Forward — the bbox-ness lives in
 *   tskbuf §3, which carries the per-proposal phys-addr table.
 *
 *   The §3 layout is not yet decoded (vendor
 *   svp_nnie_fill_forward_with_bbox_task is ~1.4K instructions,
 *   220 B stack frame); see kaeru node
 *   `nnie-neo-cv500-vendor-forward-with-bbox-kernel-re`.
 *
 *   Reject explicitly so callers fall back rather than silently
 *   running Forward-shaped offsets against a bbox-shaped buffer.
 */
static long nnie_op_forward_with_bbox(unsigned long arg)
{
	pr_info_once("nnie_neo: ForwardWithBbox not implemented yet; use Forward for classifier nets\n");
	return -EOPNOTSUPP;
}

static long nnie_op_query(unsigned long arg)
{
	/* Synchronous dispatch: by the time userspace calls Query the
	 * Forward ioctl has already returned, so the task is always done. */
	if (arg)
		((u32 *)arg)[2] = 1;
	return 0;
}

/* AddTskBuf / RemoveTskBuf arg is SVP_MEM_INFO_S { u64 phys, u64 virt,
 * u32 size, u32 pad } = 24 B. */
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

	return nnie_add_tskbuf(phys, virt, size);
}

static long nnie_op_remove_tskbuf(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u64 phys = buf ? *(u64 *)(buf + 0) : 0;

	return nnie_remove_tskbuf(phys);
}

static long nnie_osal_ioctl(unsigned int cmd, unsigned long arg, void *priv)
{
	return nnie_dispatch(cmd, arg);
}

static int nnie_osal_open(void *priv)  { return 0; }
static int nnie_osal_close(void *priv) { return 0; }

static int nnie_osal_mmap(osal_vm_t *vm, unsigned long start, unsigned long end,
			  unsigned long vm_pgoff, void *priv)
{
	osal_pgprot_noncached(vm);
	return osal_io_remap_pfn_range(vm, start, vm_pgoff, end - start);
}

static osal_fileops_t g_nnie_fops = {
	.open           = nnie_osal_open,
	.release        = nnie_osal_close,
	.unlocked_ioctl = nnie_osal_ioctl,
	.mmap           = nnie_osal_mmap,
};

/* ── Module init / exit, called from the platform probe ─────────── */

int nnie_std_mod_init(void)
{
	int ret;

	init_completion(&g_nnie_done);
	spin_lock_init(&g_crg_lock);
	spin_lock_init(&g_sys2_lock);

	g_sys2_regs = ioremap(NNIE_SYS2_BASE_PHYS, NNIE_SYS2_WINDOW_SIZE);
	if (!g_sys2_regs)
		pr_warn("nnie_neo: failed to ioremap sys2 @0x%lx\n",
			NNIE_SYS2_BASE_PHYS);

	g_crg_regs = ioremap(NNIE_CRG_BASE_PHYS, NNIE_CRG_WINDOW_SIZE);
	if (!g_crg_regs)
		pr_warn("nnie_neo: failed to ioremap CRG @0x%lx\n",
			NNIE_CRG_BASE_PHYS);

	g_nnie_dev = osal_createdev("nnie");
	if (!g_nnie_dev)
		return -ENOMEM;
	g_nnie_dev->fops = &g_nnie_fops;
	g_nnie_dev->minor = 100;

	ret = osal_registerdevice(g_nnie_dev);
	if (ret) {
		osal_destroydev(g_nnie_dev);
		g_nnie_dev = NULL;
		return ret;
	}

	if (g_nnie_irq) {
		/* IRQF_SHARED: vendor open_gdc.ko uses the same SPI. */
		ret = request_irq(g_nnie_irq, nnie_irq_handler, IRQF_SHARED,
				  "nnie_neo", &g_nnie_dev);
		if (ret)
			pr_err("nnie_neo: request_irq(%u) failed: %d\n",
			       g_nnie_irq, ret);
		else
			g_nnie_irq_requested = 1;
	}

	/* Pre-allocate the task descriptor MMZ at init time (matching
	 * vendor's allocation order) so its phys is stable across
	 * Forwards. Zero the full 64 KB so HW can't mistake uninitialised
	 * trailing bytes for valid pending ring slots. */
	g_nnie_desc_mmb = hil_mmb_alloc("nnie_desc", 64 * 1024, 4096, 0, NULL);
	if (g_nnie_desc_mmb) {
		g_nnie_desc_kvirt = hil_mmb_map2kern(g_nnie_desc_mmb);
		g_nnie_desc_phys  = g_nnie_desc_mmb->phys_addr;
		if (g_nnie_desc_kvirt)
			memset(g_nnie_desc_kvirt, 0, 64 * 1024);
	} else {
		pr_err("nnie_neo: descriptor MMZ alloc failed\n");
	}

	pr_info("nnie_neo: /dev/nnie ready\n");
	return 0;
}

void nnie_std_mod_exit(void)
{
	nnie_drain_tskbufs();

	if (g_nnie_desc_mmb) {
		hil_mmb_free(g_nnie_desc_mmb);
		g_nnie_desc_mmb   = NULL;
		g_nnie_desc_kvirt = NULL;
		g_nnie_desc_phys  = 0;
	}
	if (g_nnie_irq_requested) {
		free_irq(g_nnie_irq, &g_nnie_dev);
		g_nnie_irq_requested = 0;
	}
	if (g_crg_regs) {
		iounmap(g_crg_regs);
		g_crg_regs = NULL;
	}
	if (g_sys2_regs) {
		iounmap(g_sys2_regs);
		g_sys2_regs = NULL;
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
MODULE_DESCRIPTION("nnie_neo: open-source NNIE CNN driver (cv500/av300/dv300)");
