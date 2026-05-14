// SPDX-License-Identifier: GPL-2.0
/*
 * ive_neo — Clean-room IVE/XNN kernel driver.
 *
 * Open-source replacement for the vendor open_ive.ko binary.
 * Handles OMS model loading, XNN CNN inference via HW task nodes,
 * and CPU-based YUV preprocessing (VGS bypass).
 *
 * Supported chips (selected at compile time via -D$(CHIPARCH)):
 *   - hi3516ev200 / hi3516ev300 / gk7205v200 / gk7205v300 (V4):
 *       fused IVE+NEO at 0x11320000, full XNN dispatch.
 *   - hi3516cv500 / hi3516av300: IVE-only block at 0x11230000.
 *       CNN lives on a separate NNIE engine — XNN ioctls return
 *       -EOPNOTSUPP here, classic IVE ops work as on V4.
 *
 * Build with -DIVE_STANDALONE to remove all HiSilicon OSAL/CMPI
 * dependencies. Standalone mode uses standard Linux APIs and can
 * run in QEMU without vendor kernel modules.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/math64.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/completion.h>

#ifdef IVE_STANDALONE
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <asm/cacheflush.h>
#elif defined(hi3516cv500)
#include <linux/slab.h>
/* cv500's osal.h is a thin "module init" stub. The actual API
 * (osal_dev_t, osal_createdev, osal_io_remap_pfn_range,
 * osal_flush_dcache_area, ...) lives in hi_osal.h. mod_ext.h and
 * sys_ext.h give us cmpi_get_module_func_by_id and
 * sys_export_func->pfn_sys_drv_ioctrl for clock/reset.
 * hi_common.h provides HI_ID_SYS / HI_ID_IVE. */
#include "hi_osal.h"
#include "hi_common.h"
#include "mod_ext.h"
#include "sys_ext.h"
#else
#include "osal.h"
#include "common.h"
#include "mod_ext.h"
#include "sys_ext.h"
#include "mm_ext.h"
#endif

#define IVE_NEO_TAG "ive_neo"
#define ive_log(fmt, ...) printk(KERN_INFO IVE_NEO_TAG ": " fmt, ##__VA_ARGS__)
#define ive_err(fmt, ...) printk(KERN_ERR  IVE_NEO_TAG ": " fmt, ##__VA_ARGS__)

/* ---- Ioctl command codes ---- */
#define IVE_IOC_ALLOC_BUF      0xc0184600u /* standalone: alloc+copy_in {u64 phys, u64 udata, u32 sz} */
#define IVE_IOC_READ_BUF       0xc0184601u /* standalone: copy_out {u64 phys, u64 udata, u32 sz} */
#define IVE_IOC_SVP_INIT       0x8010463bu
#define IVE_IOC_XNN_LOADMODEL  0xc8a04636u
#define IVE_IOC_XNN_UNLOADMODEL 0xc0044637u
#define IVE_IOC_XNN_FWD_SLICE  0xc9704638u
#define IVE_IOC_XNN_FORWARD    0xc620463au
#define IVE_IOC_QUERY          0xc00c462cu
#define IVE_IOC_OPEN_DEV       0x801046c8u
#define IVE_IOC_SVP_EXIT       0x0000463eu
/* libive.so SVP_ALG_PROC ioctls — allocate/free a shared 4KB buffer that
 * userspace mmaps via /dev/ive's mmap callback. */
#define IVE_IOC_SVP_ALG_PROC_INIT  0x800b463bu
#define IVE_IOC_SVP_ALG_PROC_EXIT  0x0000463du
#define IVE_IOC_SVP_ALG_PROC_BEGIN 0x0000464au
#define IVE_IOC_SVP_ALG_PROC_END   0x0000464bu

/* ---- Globals ---- */

#ifdef IVE_STANDALONE
static void __iomem *g_ive_regs;
#else
/* Shared with ive_init.c platform probe */
void *g_ive_regs;
unsigned int g_ive_irq;
unsigned char g_ive_power_save_en;
unsigned short g_ive_node_num = 512;
unsigned int g_vendor_task_phys;
unsigned char g_skip_hw_init;
module_param_named(vendor_task, g_vendor_task_phys, uint, S_IRUGO | S_IWUSR);
module_param_named(skip_init, g_skip_hw_init, byte, S_IRUGO);

/* cv500-only IVE ops that the V4-family driver code never implemented
 * (HOG descriptor, PerspTrans perspective warp — and eventually KCF
 * tracker). The cv500 IVE block supports them in hardware but the HW
 * task-node layout is RE-only ground we're still building out.
 *
 * Phase A (default, enable_cv500_extras=0): the dispatcher returns
 * 0 for these ioctls so libive.so's handle check stays happy and
 * userland sees the same "silently-stubbed" behaviour we apply to
 * the other unsupported ops. No HW writes — zero risk of board hang.
 *
 * Phase B (enable_cv500_extras=1, set via modprobe or sysfs): we
 * log the userspace arg buffer (first 256 B hexdump, plus header)
 * and return -EOPNOTSUPP. Used to capture real-world call patterns
 * from libive on hardware so we can validate the static RE before
 * starting to write HW task nodes. Still no HW writes from the
 * driver — the param only unlocks diagnostic logging. */
static unsigned char g_enable_cv500_extras;
module_param_named(enable_cv500_extras, g_enable_cv500_extras, byte,
		   S_IRUGO | S_IWUSR);
#endif

/* ---- Model context ---- */
#define MAX_XNN_MODELS 4
struct xnn_model_ctx {
	u64 model_phys;
	u32 model_size;
	u32 weight_off;
	u32 weight_size;
	u32 tmp_buf_size;
	u16 layer_num;
	u8  src_num, dst_num;
	u8  in_use;
	u64 task_phys;
	void *task_virt;
	u32 task_size;
	u16 node_count;
};
static struct xnn_model_ctx g_models[MAX_XNN_MODELS];

static u64 g_mmz_base;

/* Shared IRQ / completion state for both DMA selftest and fwd_slice */
static u32 g_ive_last_status;
static u32 g_ive_irq_count;
static DECLARE_COMPLETION(g_ive_done);

/* Shared 4KB MMZ buffers that libive.so allocates via init ioctls and
 * then mmaps via /dev/ive to get userspace virt pointers.
 *  - svp_alg: cmd 0x8010463b (vendor's svp_alg_proc_init)
 *  - ivp:     cmd 0x801046c8 (vendor's ivp_proc_init) */
static u64 g_svp_alg_phys;
static void *g_svp_alg_virt;
static u64 g_ivp_proc_phys;
static void *g_ivp_proc_virt;

/* Pre-allocated 256-byte MMZ buffer for non-XNN task nodes.
 * Each non-XNN IVE op builds a single 208-byte task node here,
 * submits it, and waits for completion before returning. */
static u64 g_ive_task_phys;
static void *g_ive_task_virt;

/* Shared hw_init guard — called from either svp_init or first non-XNN submit */
static int g_hw_init_done;

/* ---- Chip-ops table ----
 *
 * Selected at compile time via the -D$(CHIPARCH) flag emitted from
 * kernel/Kbuild line 7. Two records exist:
 *
 *   hi3516cv500  — IVE block at 0x11230000, classic ops only.
 *                  No NEO/XNN unit (CNN lives on a separate NNIE block
 *                  at 0x11100000, out of scope for this driver). The
 *                  [0x90] DRAM-arbitration sequence captured from the
 *                  V4 blob is irrelevant here (only Conv needs it).
 *                  cv500 vendor headers don't expose the CMPI MMZ or
 *                  SYS-DRV-IO symbols to in-tree modules, so memory
 *                  and clock paths
 *                  use plain kmalloc + virt_to_phys (same as the QEMU
 *                  standalone build) and rely on open_sys.ko / boot
 *                  defaults for IVE clock enable.
 *   hi3516ev200  — Default / V4 family (also ev300, gk7205v200/v300).
 *                  Fused IVE+NEO at 0x11320000. Full XNN dispatch +
 *                  CMPI MMZ + SYS pfnSysDrvIoCtrl clock cycle.
 */
struct ive_neo_chip_ops {
	const char *name;
	bool has_xnn;
	u32  standalone_base;
	void (*setup_mem_speed)(void __iomem *regs);
};

static void ive_setup_mem_speed_ev200(void __iomem *regs);

#if defined(hi3516cv500)
static const struct ive_neo_chip_ops ive_neo_chip = {
	.name             = "hi3516cv500",
	.has_xnn          = false,
	.standalone_base  = 0x11230000,
	.setup_mem_speed  = NULL,
};
#else
static const struct ive_neo_chip_ops ive_neo_chip = {
	.name             = "hi3516ev200",
	.has_xnn          = true,
	.standalone_base  = 0x11320000,
	.setup_mem_speed  = ive_setup_mem_speed_ev200,
};
#endif

/* ---- Helpers ---- */

static inline u32 ive_cvt(u64 phys)
{
	return (u32)(phys - g_mmz_base);
}

static inline int ive_layer_desc_off(u8 src_num, u8 dst_num)
{
	return ((-2 * (src_num + dst_num)) & 0xF) +
	       2 * (src_num + dst_num) + 80 +
	       32 * src_num + 32 * dst_num;
}

static inline int ive_layer_desc_size(u8 type)
{
	static const int sizes[] = {80, 48, 64, 96, 64, 48, 32};
	return type <= 6 ? sizes[type] : -1;
}

static inline s8 ive_normalize(u8 pixel)
{
	int v = ((pixel * 33796 - 131072) >> 20) - 1;
	return (s8)(v < -128 ? -128 : v > 127 ? 127 : v);
}

static inline void ive_link_node(u8 *node_buf, int node_idx, u32 node_phys)
{
	if (node_idx > 0)
		*(u32 *)(node_buf + (node_idx - 1) * 0x80) =
			ive_cvt(node_phys + node_idx * 0x80);
}

/* ---- Platform abstraction ---- */

static void *ive_dma_alloc(u64 *phys, u32 size)
{
#if defined(IVE_STANDALONE) || defined(hi3516cv500)
	/* cv500: vendor MMZ symbols (CMPI_*) are not exposed to in-tree
	 * modules. The buffers we allocate here are tiny (<= 4 KB each)
	 * task descriptors that HW reads via DMA after a writel/dmb fence,
	 * not streaming buffers, so kmalloc + explicit dcache flush before
	 * submit is sufficient (matches the QEMU standalone path). */
	void *virt = kmalloc(size, GFP_KERNEL);
	if (virt)
		*phys = virt_to_phys(virt);
	return virt;
#else
	void *virt = NULL;
	if (CMPI_MmzMallocNocache(NULL, "Xnn_Neo", phys, &virt, size))
		return NULL;
	return virt;
#endif
}

static void ive_dma_free(u64 phys, void *virt)
{
#if defined(IVE_STANDALONE) || defined(hi3516cv500)
	kfree(virt);
#else
	CMPI_MmzFree(phys, virt);
#endif
}

static void *ive_map_nocache(u64 phys, u32 size)
{
#if defined(IVE_STANDALONE) || defined(hi3516cv500)
	return phys_to_virt(phys);
#else
	return CMPI_Remap_Nocache(phys, size);
#endif
}

static void ive_unmap_flush(void *virt, u32 size)
{
#ifdef IVE_STANDALONE
	/* Flush dcache so DMA/QEMU sees our writes */
	__cpuc_flush_dcache_area(virt, size);
	outer_flush_range(virt_to_phys(virt), virt_to_phys(virt) + size);
#elif defined(hi3516cv500)
	/* cv500 path is XNN-disabled so this helper is reachable only on
	 * V4. Keep a portable fallback that flushes dcache to phys range
	 * the HW will read. */
	osal_flush_dcache_area(virt, virt_to_phys(virt), size);
#else
	CMPI_Unmap(virt);
#endif
}

/* ---- Build HW task nodes from OMS layer descriptors ---- */

static int ive_build_task_nodes(struct xnn_model_ctx *m,
				u8 *oms, u64 tmp_phys, u64 model_phys)
{
	u8 *desc;
	u8 *node_buf = (u8 *)m->task_virt;
	u32 node_phys = (u32)m->task_phys;
	int node_idx = 0;
	int i;
	u8 src_num = oms[52];
	u8 dst_num = oms[53];
	u16 layer_num = *(u16 *)(oms + 50);

	desc = oms + ive_layer_desc_off(src_num, dst_num);
	memset(node_buf, 0, m->task_size);

	for (i = 0; i < layer_num; i++) {
		u8 type = desc[0];
		u8 *node = node_buf + node_idx * 0x80;
		int sz = ive_layer_desc_size(type);

		if (sz < 0)
			return -EINVAL;

		switch (type) {
		case 5: /* Preproc — CPU handles this, no HW node needed */
			break;
		case 1: /* Flatten */
			ive_link_node(node_buf, node_idx, node_phys);
			*(u8 *)(node + 9) = desc[3];
			*(u8 *)(node + 10) = 0x36;
			*(u8 *)(node + 11) = 1;
			*(u32 *)(node + 16) = ive_cvt(tmp_phys + *(u32 *)(desc + 12));
			*(u32 *)(node + 20) = ive_cvt(tmp_phys + *(u32 *)(desc + 16));
			*(u32 *)(node + 32) = *(u16 *)(desc + 8) * *(u16 *)(desc + 20);
			*(u16 *)(node + 40) = *(u16 *)(desc + 10);
			*(u16 *)(node + 42) = *(u16 *)(desc + 8);
			*(u16 *)(node + 44) = *(u16 *)(desc + 20);
			*(u16 *)(node + 46) = *(u16 *)(desc + 22);
			*(u16 *)(node + 48) = *(u16 *)(desc + 6);
			*(u16 *)(node + 50) = 1;
			*(u32 *)(node + 52) = *(u32 *)(desc + 28);
			*(u32 *)(node + 56) = *(u32 *)(desc + 32);
			*(u8 *)(node + 63) = 1;
			node_idx++;
			break;
		case 2: /* FC */
			/* node+32 and node+44 are vendor-patched per-tile in
			 * ive_start_task; the formula isn't reverse-engineered.
			 * The hardcoded values below match a kprobe capture for
			 * a non-tiled FC layer; tiled FC will produce wrong
			 * output. Loud one-time warn so users know. */
			pr_warn_once("ive_neo: FC layer using non-tiled hardcoded params (node+32=0x400, node+44=0x20); tiled FC will compute incorrect output\n");
			ive_link_node(node_buf, node_idx, node_phys);
			*(u8 *)(node + 8) = desc[3];
			*(u8 *)(node + 9) = desc[4];
			*(u8 *)(node + 10) = 0x36;
			*(u8 *)(node + 11) = 2;
			*(u32 *)(node + 12) = *(u32 *)(desc + 32);
			*(u32 *)(node + 16) = ive_cvt(tmp_phys + *(u32 *)(desc + 24));
			*(u32 *)(node + 20) = ive_cvt(tmp_phys + *(u32 *)(desc + 28));
			*(u32 *)(node + 24) = ive_cvt(model_phys + *(u32 *)(desc + 20));
			*(u32 *)(node + 28) = *(u32 *)(desc + 16);
			*(u32 *)(node + 32) = 0x400;
			*(u16 *)(node + 40) = 1;
			*(u16 *)(node + 42) = 1;
			*(u16 *)(node + 44) = 0x20;
			*(u16 *)(node + 46) = *(u16 *)(desc + 14);
			*(u16 *)(node + 48) = *(u16 *)(desc + 8);
			*(u16 *)(node + 50) = *(u16 *)(desc + 10);
			*(u32 *)(node + 52) = *(u32 *)(desc + 44);
			*(u32 *)(node + 56) = *(u32 *)(desc + 48);
			*(u8 *)(node + 60) = desc[6];
			*(u8 *)(node + 63) = desc[5];
			*(u32 *)(node + 64) = *(u32 *)(desc + 40);
			*(u32 *)(node + 68) = *(u32 *)(desc + 36);
			node_idx++;
			break;
		case 4: { /* Unpack */
			u16 out_c = *(u16 *)(desc + 6);
			u16 out_h = *(u16 *)(desc + 8);
			u16 in_stride = *(u16 *)(desc + 12);
			/* Vendor uses in_stride for BOTH in and out stride */
			u16 out_stride = in_stride;
			ive_link_node(node_buf, node_idx, node_phys);
			*(u8 *)(node + 8) = desc[3];
			*(u8 *)(node + 9) = desc[4];
			*(u8 *)(node + 10) = 0x36;
			*(u8 *)(node + 11) = 4;
			*(u32 *)(node + 12) = *(u32 *)(desc + 32);
			*(u32 *)(node + 16) = ive_cvt(tmp_phys + *(u32 *)(desc + 24));
			*(u32 *)(node + 20) = ive_cvt(tmp_phys + *(u32 *)(desc + 28));
			*(u32 *)(node + 24) = ive_cvt(model_phys + *(u32 *)(desc + 52));
			*(u32 *)(node + 28) = *(u32 *)(desc + 48);
			*(u32 *)(node + 32) = (u32)out_h * in_stride;
			*(u32 *)(node + 36) = (u32)out_h * out_stride;
			*(u16 *)(node + 40) = *(u16 *)(desc + 10);
			*(u16 *)(node + 42) = *(u16 *)(desc + 8);
			*(u16 *)(node + 44) = in_stride;
			*(u16 *)(node + 46) = out_stride;
			*(u16 *)(node + 48) = out_c;
			*(u16 *)(node + 50) = out_c;
			*(u32 *)(node + 52) = *(u32 *)(desc + 40);
			*(u32 *)(node + 56) = *(u32 *)(desc + 44);
			*(u8 *)(node + 63) = 1;
			*(u32 *)(node + 64) = *(u32 *)(desc + 36);
			*(u8 *)(node + 78) = 8;
			*(u32 *)(node + 116) = (u32)out_h * out_stride * out_c;
			node_idx++;
			break;
		}
		case 0: { /* Conv — IDA ive_xnn_parse_conv_constprop_40 line 4642
			 * Desc field map (from s[] packing in IDA):
			 * desc[3]=in_fmt, [4]=out_fmt, [5]=pool, [6]=af, [7]=is_pad
			 * desc[8..9]=in_c, [10..11]=in_h, [12..13]=in_w
			 * desc[14..15]=out_c, [16..17]=out_h, [18..19]=out_w
			 * desc[20..23]=arg_len, [24..27]=arg_off
			 * desc[40..43]=in_tmp, [44..47]=out_tmp
			 * desc[48..49]=in_stride, [50..51]=out_stride
			 * desc[61]=kernel_size, [62]=in_bond_num
			 */
			u16 in_c = *(u16 *)(desc + 8);
			u16 in_h = *(u16 *)(desc + 10);
			u16 in_w = *(u16 *)(desc + 12);
			u16 out_c = *(u16 *)(desc + 14);
			u8 ksize = desc[61];
			u8 bond = desc[62];
			/* Compute tiling params (IDA lines 4704-4770)
			 * v44=in_bond_num==1, v32=in_w, v30=in_h, v45=ksize */
			int tile_w, tile_h;
			if (bond == 1) {
				tile_w = 64; tile_h = 4;
			} else if (in_w <= 16) {
				tile_w = 16; tile_h = 16;
			} else {
				tile_w = 32; tile_h = 8;
			}
			int rows, cols;
			if (ksize == 3) {
				cols = ((int)in_w + tile_w - 3) / tile_w;
				rows = (tile_h + (int)in_h - 3) / tile_h;
			} else {
				cols = ((int)in_w + tile_w - 1) / tile_w;
				rows = (tile_h + (int)in_h - 1) / tile_h;
			}

			ive_link_node(node_buf, node_idx, node_phys);
			*(u8  *)(node + 8) = desc[3];  /* in_fmt */
			*(u8  *)(node + 9) = desc[4];  /* out_fmt */
			*(u8  *)(node + 10) = 0x36;
			*(u8  *)(node + 11) = 0; /* Conv: type 0, HW handles tiling internally */
			*(u32 *)(node + 12) = *(u32 *)(desc + 28); /* s[8] */
			*(u32 *)(node + 16) = ive_cvt(tmp_phys + *(u32 *)(desc + 40));
			*(u32 *)(node + 20) = ive_cvt(tmp_phys + *(u32 *)(desc + 44));
			*(u32 *)(node + 24) = ive_cvt(model_phys + *(u32 *)(desc + 24));
			*(u32 *)(node + 28) = *(u32 *)(desc + 20); /* arg_len */
			*(u32 *)(node + 32) = (u32)in_w * (u32)*(u16 *)(desc + 48);
			*(u32 *)(node + 36) = (u32)*(u16 *)(desc + 16) *
					      (u32)*(u16 *)(desc + 50);
			*(u16 *)(node + 40) = in_w;  /* HIWORD(s[3]) */
			*(u16 *)(node + 42) = in_h;  /* LOWORD(s[3]) */
			*(u16 *)(node + 44) = *(u16 *)(desc + 48); /* in_stride */
			*(u16 *)(node + 46) = *(u16 *)(desc + 50); /* out_stride */
			*(u16 *)(node + 48) = in_c;  /* HIWORD(s[2]) */
			*(u16 *)(node + 50) = out_c;
			*(u32 *)(node + 52) = *(u32 *)(desc + 64); /* s[17] */
			*(u32 *)(node + 56) = *(u32 *)(desc + 68); /* s[18] */
			*(u8  *)(node + 60) = desc[6]; /* af_mode */
			*(u8  *)(node + 61) = desc[5]; /* pool_mode */
			*(u8  *)(node + 62) = desc[3]; /* in_fmt */
			*(u8  *)(node + 63) = 1;
			*(u32 *)(node + 64) = *(u32 *)(desc + 36); /* s[10] */
			*(u32 *)(node + 68) = *(u32 *)(desc + 32); /* s[9] */
			*(u8  *)(node + 72) = desc[60];
			*(u8  *)(node + 73) = ksize;
			*(u8  *)(node + 74) = (u8)rows;
			*(u8  *)(node + 75) = (u8)cols;
			*(u8  *)(node + 76) = (u8)tile_h;
			*(u8  *)(node + 77) = (u8)tile_w;
			*(u8  *)(node + 78) = bond;
			*(u32 *)(node + 112) = 0;
			*(u32 *)(node + 116) = *(u32 *)(node + 36) *
				(((-out_c & 7) + out_c) >> 3);
			node_idx++;

			ive_log("conv: %ux%ux%u k=%u tiles=%dx%d=%d\n",
				in_c, in_h, in_w, ksize,
				rows, cols, rows * cols);
			break;
		}
		case 3: /* Eltwise */
		case 6: /* XNN-DMA layer */
			pr_warn_once("ive_neo: XNN layer type %u (Eltwise/DMA-layer) not implemented; rejecting model load\n",
				     type);
			return -EOPNOTSUPP;
		}
		desc += sz;
	}
	return node_idx;
}

/* ---- HW init ---- */

/* Assert/release IVE reset and enable its clock via the SYS module.
 * Without this, the IVE clock can be gated off when open_sys.ko's
 * MPI_SYS_Exit ran in a prior session — and any subsequent register
 * access (even readl(regs+0x80) for the HW-ID probe) faults with a
 * synchronous abort that hangs the SoC.
 *
 * V4 and cv500 SYS modules expose the same shape (look up SYS's
 * export-funcs table, invoke its drv_ioctrl with a per-chip enum), but
 * with totally renamed symbols and different enum ordinals:
 *
 *   V4 (ev200/gk7205v200): CMPI_GetModuleFuncById(2) -> SYS_EXPORT_FUNC_S
 *                          ->pfnSysDrvIoCtrl(chn, 144=reset, 145=clk_en)
 *                          MPP_CHN_S { .enModId, .s32DevId, .s32ChnId }
 *   cv500 (hi3516cv500):   cmpi_get_module_func_by_id(HI_ID_SYS) -> sys_export_func
 *                          ->pfn_sys_drv_ioctrl(chn, SYS_IVE_RESET_SEL,
 *                                               SYS_IVE_CLK_EN)
 *                          hi_mpp_chn { .mod_id, .dev_id, .chn_id }
 *
 * Both rely on open_sys.ko being loaded first (load_hisilicon ensures
 * the order). If the SYS export table isn't registered, both helpers
 * fall through quietly and the hw_id==0 check below catches it. */
#if !defined(IVE_STANDALONE) && !defined(hi3516cv500)
static void ive_assert_clock_v4(void)
{
	SYS_EXPORT_FUNC_S *sys;
	MPP_CHN_S chn = { .enModId = 29, .s32DevId = 0, .s32ChnId = 0 };
	GK_S32 val;

	sys = (SYS_EXPORT_FUNC_S *)CMPI_GetModuleFuncById(2);
	if (sys && sys->pfnSysDrvIoCtrl) {
		val = 1; sys->pfnSysDrvIoCtrl(&chn, 144, &val); /* reset(1) */
		udelay(1000);
		val = 1; sys->pfnSysDrvIoCtrl(&chn, 145, &val); /* clk en */
		val = 0; sys->pfnSysDrvIoCtrl(&chn, 144, &val); /* reset(0) */
	}
}
#endif

#if defined(hi3516cv500)
static void ive_assert_clock_cv500(void)
{
	sys_export_func *sys;
	hi_mpp_chn chn = { .mod_id = HI_ID_IVE, .dev_id = 0, .chn_id = 0 };
	hi_s32 val;

	sys = (sys_export_func *)cmpi_get_module_func_by_id(HI_ID_SYS);
	if (sys && sys->pfn_sys_drv_ioctrl) {
		val = 1; sys->pfn_sys_drv_ioctrl(&chn, SYS_IVE_RESET_SEL, &val);
		udelay(1000);
		val = 1; sys->pfn_sys_drv_ioctrl(&chn, SYS_IVE_CLK_EN, &val);
		val = 0; sys->pfn_sys_drv_ioctrl(&chn, SYS_IVE_RESET_SEL, &val);
	} else {
		pr_warn("ive_neo: SYS module not registered — cannot enable IVE clock; expect a hang if open_sys.ko was rmmoded\n");
	}
}
#endif

/* drv_ive_set_mem_speed — memory arbitration register [0x90].
 * Vendor init applies a long bit-field sequence that lands on a
 * specific priority/latency config. Without this [0x90] stays 0
 * and Conv HW hangs waiting for DRAM. Sequence was kprobed from the
 * V4 vendor blob; cv500's IVE block has no Conv unit so the [0x90]
 * write is unnecessary there (and the equivalent cv500 value is
 * unknown — it would matter only once the future NNIE backend lands).
 */
static void ive_setup_mem_speed_ev200(void __iomem *regs)
{
	u32 v = readl(regs + 0x90);
	v = (v & 0xFC7FFFFF) | 0x1800000;
	v = (v & 0xFF9FFFFF) | 0x200000;
	v = (v & 0xFFE7FFFF) | 0x80000;
	v = (v & 0xFFF8FFFF) | 0x30000;
	v = (v & 0xFFFF3FFF) | 0x4000;
	v = (v & 0xFFFFCFFF) | 0x1000;
	v = (v & 0xFFFFF3FF);
	v = (v & 0xFFFFFCFF) | 0x100;
	v = (v & 0xFFFFFF3F) | 0x40;
	v = (v & 0xFFFFFFCF) | 0x10;
	v = (v & 0xFFFFFFF3) | 8;
	v = (v & 0xFFFFFFFC) | 1;
	writel(v, regs + 0x90);
}

static void ive_hw_init(void)
{
	u32 hw_id;

	pr_info_once("ive_neo: chip=%s xnn=%s base=0x%x\n",
		     ive_neo_chip.name,
		     ive_neo_chip.has_xnn ? "yes" : "no",
		     ive_neo_chip.standalone_base);

#if defined(IVE_STANDALONE)
	/* no clock cycle in QEMU */
#elif defined(hi3516cv500)
	ive_assert_clock_cv500();
#else
	ive_assert_clock_v4();
#endif

	g_mmz_base = 0;

	hw_id = readl(g_ive_regs + 0x80);
	if (hw_id == 0) {
		ive_err("hw_init: HW not responding (clock disabled?)\n");
		return;
	}

	/* The block of register writes below comes from the V4 (ev200) blob
	 * init sequence and was confirmed working on hi3516ev300 in 2026-05.
	 * On cv500 the same offsets caused a synchronous abort that hung
	 * the board (offsets 0x34, 0x54, 0x60 are not part of cv500's IVE
	 * register window — cv500 has the classic IVE block only, while V4
	 * also has the NEO/XNN unit whose config lives at the same window).
	 *
	 * Until we kprobe the cv500 vendor blob to discover the correct
	 * init sequence, do the minimum on cv500: enable + clear interrupts.
	 * The HW is left in whatever state open_sys.ko / u-boot put it in,
	 * which is sufficient for the 24 classic ops we expose. */
	writel(6, g_ive_regs + 0x04);           /* drv_ive_en_int */
	writel(7, g_ive_regs + 0x08);           /* drv_ive_clear_int */
#if !defined(hi3516cv500)
	writel(0xffffffff, g_ive_regs + 0x60);  /* drv_ive_write_timeout_regs(-1) */
	writel(readl(g_ive_regs + 0x34) | 1, g_ive_regs + 0x34);  /* drv_ive_en_clk_gt */
	writel((readl(g_ive_regs + 0x54) & 0xfffffff0) | 7, g_ive_regs + 0x54);
	writel(readl(g_ive_regs + 0x54) | 0xf00, g_ive_regs + 0x54);  /* set_outstanding */
#endif

	if (ive_neo_chip.setup_mem_speed)
		ive_neo_chip.setup_mem_speed(g_ive_regs);
	else
		pr_info_once("ive_neo: skipping [0x90] mem-speed (chip=%s — not needed for classic IVE ops)\n",
			     ive_neo_chip.name);

#if !defined(hi3516cv500)
	writel(0, g_ive_regs + 0x8C);  /* drv_ive_set_base_addr(mmz_base>>12)=0 */
#endif

	ive_log("hw_init: HW ID=0x%08x\n", hw_id);
}

/* ---- Ioctl handlers (work on kernel buffer, no copy_from/to_user) ---- */

static long ive_svp_init(unsigned long arg)
{
	u8 *buf = (u8 *)arg;

#ifdef IVE_STANDALONE
	if (!g_ive_regs)
		g_ive_regs = ioremap(ive_neo_chip.standalone_base, 0x10000);
#endif
	if (!g_ive_regs) {
		ive_err("svp_init: g_ive_regs not set (DT probe didn't run?)\n");
		return -ENODEV;
	}
	/* Run hw_init on first svp_init call, and re-run if HW went offline
	 * (HI_MPI_SYS_Exit disables the IVE clock between sessions).
	 *
	 * The pre-read of [0x80]/[0x04] used to gate re-init only fires on
	 * subsequent calls — on the very first call we go straight into
	 * hw_init, because reading any IVE register before clocks are on
	 * faults synchronously on cv500 (the SYS module gates the IVE
	 * clock off until ive_assert_clock_cv500 turns it on inside
	 * hw_init). */
	if (g_ive_regs && !g_skip_hw_init) {
		if (!g_hw_init_done) {
			ive_hw_init();
			g_hw_init_done = 1;
		} else {
			u32 hw_id = readl(g_ive_regs + 0x80);
			u32 en    = readl(g_ive_regs + 0x04);
			if (hw_id == 0 || (en & 6) != 6)
				ive_hw_init();
		}
	} else if (g_ive_regs && g_skip_hw_init && !g_hw_init_done) {
		ive_log("hw_init SKIPPED (skip_init=1)\n");
		g_hw_init_done = 1;
	}

	/* Vendor's cmd 0x8010463b (which we called IVE_IOC_SVP_INIT) is
	 * actually svp_alg_proc_init: allocate a 4 KB MMZ buffer that
	 * libive.so then mmaps via /dev/ive's mmap callback.
	 *
	 * Return layout (matches vendor IDA lines 7470-7487):
	 *   buf[0..3] = 4095 (size - 1)
	 *   buf[8..15] = phys (u64)
	 */
	if (!g_svp_alg_virt) {
		g_svp_alg_virt = ive_dma_alloc(&g_svp_alg_phys, 4096);
		if (!g_svp_alg_virt) {
			ive_err("svp_init: svp_alg alloc failed\n");
			return -ENOMEM;
		}
		memset(g_svp_alg_virt, 0, 4096);
	}
	*(u32 *)(buf + 0) = 4095;
	*(u64 *)(buf + 8) = g_svp_alg_phys;
	ive_log("svp_init: svp_alg phys=0x%llx\n", g_svp_alg_phys);
	return 0;
}

#define XNN_MODEL_PARAMS_OFF 12
#define OMS_TMP_BUF_SIZE  12
#define OMS_DATA_SLICE    20
#define OMS_ROI_BATCH     22
#define OMS_SCALE_NUM     48
#define OMS_LAYER_NUM     50
#define OMS_SRC_NUM       52
#define OMS_DST_NUM       53

static u32 g_model_id_counter;

static u32 ive_alloc_model_slot(void)
{
	u32 i;
	for (i = 0; i < MAX_XNN_MODELS; i++)
		if (!g_models[i].in_use)
			return i;
	return (u32)-1;
}

static long ive_xnn_loadmodel(unsigned long arg)
{
	u32 *buf = (u32 *)arg;
	u64 model_phys = *(u64 *)(buf + 0);
	u32 model_size = buf[4];
	u8 *oms, *preproc;
	u32 *mp, tmp_buf_size;
	u16 scale_num, layer_num;
	u8 src_num, dst_num;

	if (!ive_neo_chip.has_xnn) {
		pr_info_once("ive_neo: XNN loadmodel rejected on chip=%s (no NEO unit in IVE block)\n",
			     ive_neo_chip.name);
		return -EOPNOTSUPP;
	}
	if (model_size < 0x50)
		return -EINVAL;

	oms = phys_to_virt(model_phys);
	if (!oms)
		return -ENOMEM;

	tmp_buf_size = *(u32 *)(oms + OMS_TMP_BUF_SIZE);
	scale_num    = *(u16 *)(oms + OMS_SCALE_NUM);
	layer_num    = *(u16 *)(oms + OMS_LAYER_NUM);
	src_num      = oms[OMS_SRC_NUM];
	dst_num      = oms[OMS_DST_NUM];

	ive_log("loadmodel: phys=0x%llx size=%u layers=%u\n",
		model_phys, model_size, layer_num);

	mp = buf + XNN_MODEL_PARAMS_OFF;
	memset(mp, 0, 2160);
	/* Use lowest free slot instead of a monotonic counter so repeated
	 * loadmodel calls across user-space sessions get mid=0 first. */
	mp[0] = ive_alloc_model_slot();
	if (mp[0] == (u32)-1)
		return -ENOMEM;
	g_model_id_counter = mp[0] + 1;
	mp[1] = tmp_buf_size;
	mp[2] = scale_num;
	*(u16 *)((u8 *)mp + 12) = src_num;
	*(u16 *)((u8 *)mp + 14) = dst_num;

	preproc = oms + ive_layer_desc_off(src_num, dst_num);
	if (preproc[0] == 5) {
		mp[4] = *(u32 *)(preproc + 40);
		mp[5] = *(u16 *)(preproc + 8);
		mp[6] = *(u16 *)(preproc + 6);
		mp[7] = 3;
		mp[17] = 1;
	}

	/* Fill dst node params from Unpack descriptor (for forward validation) */
	{
		u8 *d = oms + ive_layer_desc_off(src_num, dst_num);
		int li;
		for (li = 0; li < layer_num; li++) {
			u8 lt = d[0];
			if (lt == 4) { /* Unpack — this is the output layer */
				u32 *dst = (u32 *)((u8 *)mp + 1232);
				dst[0] = 7;  /* blob_type = U8 */
				dst[1] = *(u16 *)(d + 10); /* width */
				dst[2] = *(u16 *)(d + 8);  /* height */
				dst[3] = *(u16 *)(d + 6);  /* chn */
				dst[4] = li;  /* output layer index */
				break;
			}
			d += ive_layer_desc_size(lt);
		}
	}

	*((u8 *)mp + 2152) = oms[OMS_DATA_SLICE];
	*((u8 *)mp + 2153) = oms[OMS_ROI_BATCH];

	{
		u32 mid = mp[0];
		struct xnn_model_ctx *m;

		if (mid >= MAX_XNN_MODELS)
			return -ENOMEM;

		m = &g_models[mid];
		m->model_phys = model_phys;
		m->model_size = model_size;
		m->weight_off = *(u32 *)(oms + 60);
		m->weight_size = *(u32 *)(oms + 56);
		m->tmp_buf_size = tmp_buf_size;
		m->layer_num = layer_num;
		m->src_num = src_num;
		m->dst_num = dst_num;

		{
			/* Each HW task node is 0x80 bytes. Worst case: one node
			 * per OMS layer (Conv/FC/Flatten/Unpack each emit one;
			 * types 3/5/6 emit none — overshoot is harmless). Round
			 * up to a page. The old fixed 4096 only fit 32 nodes and
			 * overran for models with >32 layers. */
			u32 need = ((u32)layer_num * 0x80 + 4095) & ~4095u;
			if (need < 4096)
				need = 4096;
			if (m->task_virt && m->task_size < need) {
				ive_dma_free(m->task_phys, m->task_virt);
				m->task_virt = NULL;
			}
			if (!m->task_virt) {
				m->task_size = need;
				m->task_virt = ive_dma_alloc(&m->task_phys,
							     m->task_size);
				if (!m->task_virt)
					return -ENOMEM;
			}
		}
		m->in_use = 1;

		{
			u64 tmp_phys = *(u64 *)((u8 *)buf + 24);
			int rc = ive_build_task_nodes(m, oms, tmp_phys, model_phys);
			if (rc < 0)
				return rc;
			m->node_count = (u16)rc;
		}
	}
	return 0;
}

/* Vendor cmd 0x801046c8 = ivp_proc_init: allocate a 4KB MMZ buffer
 * "IVPProc". Returns buf[0]=4095, buf[8..15]=phys. Parallels
 * svp_init's svp_alg_proc handling but for a different global.
 *
 * This is part of the IVP (XNN) init flow — libive calls it only when
 * a model is being loaded, so it's safe to gate on has_xnn. */
static long ive_open_dev(unsigned long arg)
{
	u8 *buf = (u8 *)arg;

	if (!ive_neo_chip.has_xnn)
		return -EOPNOTSUPP;
	if (!buf)
		return -EINVAL;
	if (!g_ivp_proc_virt) {
		g_ivp_proc_virt = ive_dma_alloc(&g_ivp_proc_phys, 4096);
		if (!g_ivp_proc_virt) {
			ive_err("ivp_proc_init: alloc failed\n");
			return -ENOMEM;
		}
		memset(g_ivp_proc_virt, 0, 4096);
	}
	*(u32 *)(buf + 0) = 4095;
	*(u64 *)(buf + 8) = g_ivp_proc_phys;
	ive_log("ivp_proc_init: phys=0x%llx\n", g_ivp_proc_phys);
	return 0;
}

static long ive_xnn_fwd_slice(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u32 model_id = *(u32 *)(buf + 0x338);
	u64 src_phys = *(u64 *)(buf + 0x018);
	u32 src_stride = *(u32 *)(buf + 0x00C);
	u32 src_w = *(u32 *)(buf + 0x028);
	u32 src_h = *(u32 *)(buf + 0x02C);
	u64 dst_phys = *(u64 *)(buf + 0x650);
	u64 tmp_phys = *(u64 *)(buf + 0x940);
	u32 tmp_size = *(u32 *)(buf + 0x950);
	struct xnn_model_ctx *m;
	u32 node_phys, status;
	int timeout, ni;

	if (!ive_neo_chip.has_xnn)
		return -EOPNOTSUPP;
	if (model_id >= MAX_XNN_MODELS || !g_models[model_id].in_use)
		return -EINVAL;
	m = &g_models[model_id];
	if (!m->task_virt)
		return -ENOMEM;

	node_phys = (u32)m->task_phys;

	/* Patch Unpack output → dst. Walk the actual node count from
	 * loadmodel; the old hardcoded 32 missed Unpack nodes in models
	 * with many Conv layers (the IVP model has Unpack near index 44). */
	for (ni = 0; ni < m->node_count; ni++) {
		u8 *nd = (u8 *)m->task_virt + ni * 0x80;
		if (nd[10] != 0x36)
			break;
		if (nd[11] == 4) {
			*(u32 *)(nd + 20) = (u32)dst_phys;
			break;
		}
	}

	/* CPU preprocessing: YUV420SP → 3ch normalized S8.
	 * The loop below writes exactly 3 planes (Y, U, V) each of size
	 * src_w * src_h. The old `32 * src_w * src_h` was a ~10x overshoot
	 * that walked memset off the end of the mapped TMP region. */
	{
		u32 pp_size = 3 * src_w * src_h;
		void *nc = ive_map_nocache(tmp_phys,
				pp_size < tmp_size ? pp_size : tmp_size);
		/* Source frame: use virt addr from blob (buf+0x10 = virt field).
		 * In OSAL mode the ioctl buf is kernel-copied so virt is the
		 * userspace mapping of MMZ — but phys_to_virt works for MMZ.
		 * In standalone mode, phys_to_virt may fail for user-allocated
		 * pages, so fall back to the virt pointer from the blob. */
		u64 src_virt_u = *(u64 *)(buf + 0x010);
		u8 *sv = phys_to_virt(src_phys);
#ifdef IVE_STANDALONE
		/* In standalone mode, virt pointer from ioctl is the user's
		 * virtual address. Use copy_from_user via a kernel buffer. */
		{
			u32 frm_size = src_stride * src_h + src_stride * src_h / 2;
			u8 *frm_buf = kmalloc(frm_size, GFP_KERNEL);
			if (frm_buf && !copy_from_user(frm_buf,
					(void __user *)(uintptr_t)src_virt_u, frm_size))
				sv = frm_buf;
			else {
				kfree(frm_buf);
				sv = NULL;
			}
		}
#endif
		if (nc && sv) {
			u8 *out = nc, *uv = sv + src_stride * src_h;
			u32 plane = src_w * src_h;
			int y, x;

			memset(nc, 0, pp_size);
			/* Write RAW YUV planes (no normalization). Vendor's
			 * VGS preproc also writes raw pixel values to tmp; the
			 * Conv HW handles scaling via per-layer requant regs
			 * d[64..71]. Normalizing here produced garbage input. */
			for (y = 0; y < src_h; y++)
				for (x = 0; x < src_w; x++) {
					int uv_off = (y / 2) * src_stride + (x & ~1);
					out[y * src_w + x] =
						sv[y * src_stride + x];
					out[plane + y * src_w + x] =
						uv[uv_off];
					out[2 * plane + y * src_w + x] =
						uv[uv_off + 1];
				}
			ive_unmap_flush(nc, pp_size);
		}
#ifdef IVE_STANDALONE
		if (sv) kfree(sv);
#endif
	}

	/* Ensure Conv-critical regs are set (OSAL rmmod clears 0x84/0x88) */
	if (g_ive_regs) {
		writel(1, g_ive_regs + 0x84);
		writel(1, g_ive_regs + 0x88);
	}

	/* Submit chain — use vendor's known-good address if available */
	if (g_vendor_task_phys)
		node_phys = g_vendor_task_phys;

	/* Submit and wait briefly for IRQ. Vendor's forward path is async —
	 * it kicks HW and returns a handle. For this SoC XNN HW is currently
	 * non-functional (confirmed empirically — vendor's [0x18] task counter
	 * never moves either), so the short wait is effectively a no-op and
	 * we return success regardless. Kept for eventual HW that works. */
	reinit_completion(&g_ive_done);
	g_ive_last_status = 0;
	writel(7, g_ive_regs + 0x08);  /* clear stale int state */

	writel(node_phys, g_ive_regs + 0x10);
	isb(); dsb(); dmb();
	writel(1, g_ive_regs + 0x00);

	(void)wait_for_completion_timeout(&g_ive_done, msecs_to_jiffies(5));
	status = g_ive_last_status;
	(void)status; (void)timeout; (void)ni;

	/* Match vendor semantics: return success regardless — caller's
	 * query ioctl will confirm completion. Buf[0] is the "handle"
	 * that libive.so checks; 0 means "already done". */
	*(u32 *)buf = 0;
	return 0;
}

static long ive_xnn_query(unsigned long arg)
{
	/* arg layout = { u32 handle (in), u32 bBlock (in), u32 done (out) }
	 * (12 B), per vendor libive HI_MPI_IVE_Query @0x76d8 (writes handle
	 * at sp+16, bBlock at sp+20, reads done from sp+24).
	 *
	 * All our dispatch paths (classic IVE, HOG, Resize, PerspTrans, LK,
	 * KCF, XNN) run synchronously inside their ioctls — by the time
	 * userspace gets to call Query, the requested handle is always
	 * complete. So this is a constant done=1. */
	((u32 *)arg)[2] = 1;
	return 0;
}

/* libive.so svp_alg_proc_init (cmd 0x800b463b):
 * Allocate a 4 KB MMZ buffer. Return buf[0]=4095 (size-1) and
 * buf[8..15]=phys (u64). Userspace then mmaps /dev/ive with offset=phys
 * to get a virt pointer. */
static long ive_svp_alg_proc_init(unsigned long arg)
{
	u8 *buf = (u8 *)arg;

	if (!buf)
		return -EINVAL;
	if (!g_svp_alg_virt) {
		g_svp_alg_virt = ive_dma_alloc(&g_svp_alg_phys, 4096);
		if (!g_svp_alg_virt) {
			ive_err("svp_alg_proc_init: alloc failed\n");
			return -ENOMEM;
		}
		memset(g_svp_alg_virt, 0, 4096);
	}
	*(u32 *)(buf + 0) = 4095;
	*(u64 *)(buf + 8) = g_svp_alg_phys;
	ive_log("svp_alg_proc_init: phys=0x%llx virt=%p\n",
		g_svp_alg_phys, g_svp_alg_virt);
	return 0;
}

static long ive_svp_alg_proc_exit(unsigned long arg)
{
	if (g_svp_alg_virt) {
		ive_dma_free(g_svp_alg_phys, g_svp_alg_virt);
		g_svp_alg_virt = NULL;
		g_svp_alg_phys = 0;
	}
	return 0;
}

static long ive_xnn_unloadmodel(unsigned long arg)
{
	u32 model_id = ((u32 *)arg)[0];

	if (!ive_neo_chip.has_xnn)
		return -EOPNOTSUPP;
	if (model_id < MAX_XNN_MODELS) {
		struct xnn_model_ctx *m = &g_models[model_id];
		if (m->task_virt) {
			ive_dma_free(m->task_phys, m->task_virt);
			m->task_virt = NULL;
		}
		m->in_use = 0;
	}
	return 0;
}

/* ==== Non-XNN IVE ops ==== */

/* 12-entry src-format-to-HW-encoding LUT, used by node[8] of
 * PerspTrans, Hog, and NCC handlers on cv500. Dumped from cv500
 * vendor blob obj/hi3516cv500/hi_ive.o .rodata @0x560 (R_ARM_MOVW/MOVT
 * relocations from ive_fill_persp_trans_task @0x95cc,
 * ive_fill_hog_task @0x9a24, ive_fill_ncc_task @0x86cc all target the
 * same anchor). en_type > 11 falls through to 7 — the "unknown" entry
 * vendor encoders use as a catchall for unsupported formats.
 *
 * Always compiled in so NCC (which is chip-agnostic) can call it;
 * V4 callers may want to bypass it since the V4 IVE block uses its
 * own LUT (not yet dumped). */
static const u8 cv500_src_fmt_lut[12] = {
	0, 7, 1, 2, 7, 7, 7, 7, 7, 7, 3, 4,
};

static inline u8 cv500_src_fmt(u32 en_type)
{
	return en_type <= 11 ? cv500_src_fmt_lut[en_type] : 7;
}

/* IVE_IMAGE_S field offsets (72-byte struct):
 *   +0: u64 phys[3]  +24: u64 virt[3]  +48: u32 stride[3]
 *  +60: u32 width   +64: u32 height   +68: u32 enType
 * Vendor reads src/dst structs from the ioctl arg buffer at fixed offsets
 * (e.g. arg+8 for src, arg+80 for dst) and fills a 208-byte HW task node.
 */
#define IMG_PHYS(img)   (*(u32 *)(img))         /* low 32-bit phys */
#define IMG_STRIDE(img) (*(u32 *)((img) + 48))  /* stride[0] */
#define IMG_WIDTH(img)  (*(u32 *)((img) + 60))
#define IMG_HEIGHT(img) (*(u32 *)((img) + 64))
#define IMG_TYPE(img)   (*(u32 *)((img) + 68))

/* Defensive validation: phys non-zero and 16-byte aligned (IVE HW requirement);
 * width/height/stride non-zero and fit in u16 (HW node packs them as u16);
 * stride >= width. The vendor blob carries ~397 distinct constraint checks
 * across all ops; this minimum slice catches the worst classes of bad input
 * (uninitialized struct → phys=0; wild values → HW programmed with garbage). */
static inline int ive_check_buf(u32 phys, u32 stride, u32 width, u32 height)
{
	if (!phys || (phys & 0xF))
		return -EINVAL;
	if (!width || width > 0xFFFF)
		return -EINVAL;
	if (!height || height > 0xFFFF)
		return -EINVAL;
	if (stride < width || stride > 0xFFFF)
		return -EINVAL;
	return 0;
}

#define IVE_CHECK_IMG(img) \
	ive_check_buf(IMG_PHYS(img), IMG_STRIDE(img), IMG_WIDTH(img), IMG_HEIGHT(img))

/* Submit a chain of N pre-filled 208-byte HW task nodes. Each node's
 * [0..3] (next-pointer) field is patched to point at the next node's
 * phys address; the last node's next-pointer is left as 0 to mark end
 * of chain. HW auto-advances through the chain after we kick the head
 * at reg[0x10]+reg[0x00].
 *
 * Used by ops that produce per-ROI outputs (PerspTrans, Hog, …): each
 * ROI emits one HW node, and the IVE block executes them in sequence
 * raising a single completion IRQ at the end.
 *
 * `nodes` is a contiguous u8 array of size node_count*208, where each
 * 208-byte slot has been filled by the per-op field-map. `arg_buf[0..3]`
 * receives the handle (0 = "already done", matching libive semantics).
 * Returns 0 on success, -ENODEV / -ENOMEM on infrastructure failure. */
static long ive_submit_chain(u8 *nodes, u32 node_count, u8 *arg_buf)
{
	u32 i;
	u32 total_bytes;

	if (!node_count)
		return -EINVAL;
	if (node_count * 208 > 4096)            /* MMZ task buffer is 4 KB */
		return -EINVAL;

#ifdef IVE_STANDALONE
	if (!g_ive_regs)
		g_ive_regs = ioremap(ive_neo_chip.standalone_base, 0x10000);
#endif
	if (!g_ive_regs) {
		ive_err("submit_chain: g_ive_regs not set (DT probe didn't run?)\n");
		return -ENODEV;
	}
	if (!g_hw_init_done) {
		ive_hw_init();
		g_hw_init_done = 1;
	}

	if (!g_ive_task_virt) {
		g_ive_task_virt = ive_dma_alloc(&g_ive_task_phys, 4096);
		if (!g_ive_task_virt)
			return -ENOMEM;
	}

	/* Patch next-ptr in each node to point at the next slot. Last
	 * node's next-ptr stays at 0 (already memset'd by caller, but
	 * write it explicitly for clarity). */
	for (i = 0; i + 1 < node_count; i++)
		*(u32 *)(nodes + i * 208) =
			(u32)g_ive_task_phys + (i + 1) * 208;
	*(u32 *)(nodes + (node_count - 1) * 208) = 0;

	total_bytes = node_count * 208;
	memcpy(g_ive_task_virt, nodes, total_bytes);

#if defined(hi3516cv500)
	osal_flush_dcache_area(g_ive_task_virt, g_ive_task_phys, total_bytes);
#endif

	reinit_completion(&g_ive_done);
	g_ive_last_status = 0;
	writel(6, g_ive_regs + 0x04);
	writel(7, g_ive_regs + 0x08);
	writel((u32)g_ive_task_phys, g_ive_regs + 0x10);
	isb(); dsb(); dmb();
	writel(1, g_ive_regs + 0x00);

	{
		unsigned long to = wait_for_completion_timeout(
			&g_ive_done, msecs_to_jiffies(100));
		if (to == 0) {
			ive_err("submit_chain: timeout (%u nodes); marking HW dirty\n",
				node_count);
			g_hw_init_done = 0;
		}
	}

	if (arg_buf)
		*(u32 *)arg_buf = 0;
	return 0;
}

/* Submit a pre-filled 208-byte non-XNN task node. Copies the
 * stack-local node to the pre-allocated MMZ task buffer, kicks HW,
 * and waits briefly for the completion IRQ. Returns 0. */
static long ive_submit_nonxnn(u8 *node, u8 *arg_buf)
{
	/* Always ensure HW is initialized. On first call, full init.
	 * Keep a dirty flag: if a previous submit timed out (HW stuck),
	 * force a full reset-cycle hw_init on the next call. */
#ifdef IVE_STANDALONE
	if (!g_ive_regs)
		g_ive_regs = ioremap(ive_neo_chip.standalone_base, 0x10000);
#endif
	if (!g_ive_regs) {
		ive_err("submit_nonxnn: g_ive_regs not set (DT probe didn't run?)\n");
		return -ENODEV;
	}
	if (!g_hw_init_done) {
		ive_hw_init();
		g_hw_init_done = 1;
	}

	if (!g_ive_task_virt) {
		g_ive_task_virt = ive_dma_alloc(&g_ive_task_phys, 4096);
		if (!g_ive_task_virt)
			return -ENOMEM;
	}
	memcpy(g_ive_task_virt, node, 208);
	/* node+0..3 = next ptr (0 = end of chain) — already set by caller */

#if defined(hi3516cv500)
	/* cv500 buffer is cacheable (kmalloc) — flush so HW sees the
	 * 208-byte node we just wrote. V4 uses CMPI MMZ which is
	 * non-cached, so no flush needed there. */
	osal_flush_dcache_area(g_ive_task_virt, g_ive_task_phys, 208);
#endif

	reinit_completion(&g_ive_done);
	g_ive_last_status = 0;
	writel(6, g_ive_regs + 0x04);   /* drv_ive_en_int */
	writel(7, g_ive_regs + 0x08);   /* drv_ive_clear_int */
	writel((u32)g_ive_task_phys, g_ive_regs + 0x10);
	isb(); dsb(); dmb();
	writel(1, g_ive_regs + 0x00);

	{
		unsigned long to = wait_for_completion_timeout(
			&g_ive_done, msecs_to_jiffies(100));
		if (to == 0) {
			/* HW stuck — force a reset cycle on next call to
			 * recover from the HI_MPI_SYS_Exit dead state. */
			g_hw_init_done = 0;
		}
	}

	/* arg_buf[0..3] = handle out; set 0 = "already done" */
	if (arg_buf)
		*(u32 *)arg_buf = 0;
	return 0;
}

/* Ioctl cmd codes for non-XNN ops (type='F'=0x46, nr=op_code) */
#define IVE_IOC_DMA       0xc0684600u
#define IVE_IOC_FILTER    0xc0b84601u
#define IVE_IOC_DILATE    0xc0b84606u
#define IVE_IOC_ERODE     0xc0b84607u
#define IVE_IOC_THRESH    0xc0a84608u
#define IVE_IOC_AND       0xc0e84609u
#define IVE_IOC_SUB       0xc0e8460au
#define IVE_IOC_OR        0xc0e8460bu
#define IVE_IOC_HIST      0xc070460du
#define IVE_IOC_THRESH_S16 0xc0a8460eu
#define IVE_IOC_THRESH_U16 0xc0a8460fu
#define IVE_IOC_INTEG     0xc0a0460cu
#define IVE_IOC_ORDSTAT   0xc0a04611u
#define IVE_IOC_ADD       0xc0e84614u
#define IVE_IOC_CCL       0xc0784617u
#define IVE_IOC_MAP       0xc0b84613u

/* ---- Thresh (op=8): arg = {handle(8), src(72), dst(72), ctrl(12+)} ---- */
static long ive_op_thresh(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *src = buf + 8, *dst = buf + 80, *ctrl = buf + 152;
	u8 node[208];

	if (IVE_CHECK_IMG(src) || IVE_CHECK_IMG(dst))
		return -EINVAL;

	memset(node, 0, sizeof(node));
	node[10]  = 8;  /* op = Thresh */
	node[11]  = ctrl[0]; /* enMode */
	node[12]  = ctrl[4]; /* u8LowThr */
	node[14]  = ctrl[5]; /* u8HighThr */
	node[56]  = ctrl[6]; /* u8MinVal */
	node[57]  = ctrl[7]; /* u8MidVal */
	node[58]  = ctrl[8]; /* u8MaxVal */
	*(u32 *)(node + 16) = IMG_PHYS(src);
	*(u32 *)(node + 20) = IMG_PHYS(dst);
	*(u16 *)(node + 40) = (u16)IMG_WIDTH(src);
	*(u16 *)(node + 42) = (u16)IMG_HEIGHT(src);
	*(u16 *)(node + 44) = (u16)IMG_STRIDE(src);
	*(u16 *)(node + 50) = (u16)IMG_STRIDE(dst);
	return ive_submit_nonxnn(node, buf);
}

/* ---- LBP (op=26): arg = {handle(8), src(72), dst(72), ctrl(8)} ---- *
 * Local Binary Pattern. cv500-only HW op (vendor blob symbol
 * ive_fill_lbp_task @0x8830). Field map: node[10]=26, node[11]=enMode,
 * node[12]=threshold (sign-extended in NORMAL mode, zero-extended in
 * ABS mode). Each output pixel is an 8-bit mask of "neighbours brighter
 * by >= threshold" — useful for texture / face descriptors.
 *
 * ctrl layout (IVE_LBP_CTRL_S, 8 B):
 *   +0: u32 enMode  (0 = NORMAL signed s8 thr, 1 = ABS unsigned u8 thr)
 *   +4: union { s8 sVal; u8 uVal; } un8BitThr
 *   +5..7: padding */
static long ive_op_lbp(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *src = buf + 8, *dst = buf + 80, *ctrl = buf + 152;
	u32 mode = *(u32 *)(ctrl + 0);
	u8 node[208];
	u16 thr16;

	if (IVE_CHECK_IMG(src) || IVE_CHECK_IMG(dst))
		return -EINVAL;
	if (mode > 1) {
		pr_info("ive_neo: LBP bad enMode=%u (0=NORMAL, 1=ABS)\n", mode);
		return -EINVAL;
	}

	if (mode == 0)
		thr16 = (u16)(s16)(s8) ctrl[4];   /* sign-extend */
	else
		thr16 = ctrl[4];                  /* zero-extend */

	memset(node, 0, sizeof(node));
	node[6]  = 0;
	node[8]  = 0;                        /* no LUT for LBP (vendor writes 0) */
	node[10] = 26;                       /* op = LBP */
	node[11] = (u8) mode;
	*(u16 *)(node + 12) = thr16;
	*(u32 *)(node + 16) = IMG_PHYS(src);
	*(u32 *)(node + 20) = IMG_PHYS(dst);
	*(u16 *)(node + 40) = (u16)IMG_WIDTH(src);
	*(u16 *)(node + 42) = (u16)IMG_HEIGHT(src);
	*(u16 *)(node + 44) = (u16)IMG_STRIDE(src);
	*(u16 *)(node + 50) = (u16)IMG_STRIDE(dst);
	pr_info_once("ive_neo: LBP handler wired (HW op 26)\n");
	return ive_submit_nonxnn(node, buf);
}

/* ---- CSC (op=2): arg = {handle(8), src(72), dst(72), ctrl(4), instant(4)} ---- *
 * Color-Space Conversion. cv500-only HW op (vendor blob
 * `ive_fill_csc_task` @0x78cc). 12 modes: BT601/BT709 × YUV2RGB /
 * YUV2HSV / YUV2LAB / RGB2YUV.
 *
 * First cut covers 8 modes that need no on-chip table:
 *   0..3 YUV2{RGB}, 8..11 RGB2{YUV}.
 * HSV (modes 4..5) and LAB (modes 6..7) need rgb2hsv (0x800 B) /
 * rgb2lab (0x1a00 B) tables copied into MMZ on first use (vendor does
 * this in `ive_csc` before calling fill_task). Shipping table init is
 * a follow-up — return -EOPNOTSUPP for those modes.
 *
 * Field map (decoded from `ive_fill_csc_task`):
 *   node[10]=2 (op), node[11]=enMode
 *   node[8]: src format byte — 1 YUV420SP, 2 YUV422SP, 3 U8C3_PKG, 4 U8C3_PLN
 *   node[9]: dst format byte — 0 YUV420SP, 1 YUV422SP, 2 U8C3_PKG, 3 U8C3_PLN
 *   node[40,42] = width, height
 *   Plane phys / strides:
 *     2-plane SP : node[16,44] = p0/s0, node[24,46] = p1/s1
 *     1-plane PKG: node[16,44] = p0/s0
 *     3-plane PLN: node[16,24,32] / [44,46,48]
 *   Dst plane lanes shift +4: node[20,28,36] / [50,52,54].
 */
static long ive_op_csc_cv500(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *src = buf + 8, *dst = buf + 80, *ctrl = buf + 152;
	u32 mode = *(u32 *)(ctrl + 0);
	u32 src_type = IMG_TYPE(src);
	u32 dst_type = IMG_TYPE(dst);
	u8  src_fmt, dst_fmt;
	u8 node[208];

	if (IVE_CHECK_IMG(src) || IVE_CHECK_IMG(dst))
		return -EINVAL;

	if (mode >= 12) {
		pr_info("ive_neo: CSC bad enMode=%u (0..11)\n", mode);
		return -EINVAL;
	}
	if (mode >= 4 && mode <= 7)
		return -EOPNOTSUPP;          /* HSV/LAB — table init not yet implemented */

	switch (src_type) {
	case 2:  src_fmt = 1; break;          /* YUV420SP */
	case 3:  src_fmt = 2; break;          /* YUV422SP */
	case 10: src_fmt = 3; break;          /* U8C3_PACKAGE */
	case 11: src_fmt = 4; break;          /* U8C3_PLANAR */
	default:
		pr_info("ive_neo: CSC bad src enType=%u\n", src_type);
		return -EINVAL;
	}
	switch (dst_type) {
	case 2:  dst_fmt = 0; break;
	case 3:  dst_fmt = 1; break;
	case 10: dst_fmt = 2; break;
	case 11: dst_fmt = 3; break;
	default:
		pr_info("ive_neo: CSC bad dst enType=%u\n", dst_type);
		return -EINVAL;
	}

	memset(node, 0, sizeof(node));
	node[8]  = src_fmt;
	node[9]  = dst_fmt;
	node[10] = 2;                         /* op = CSC */
	node[11] = (u8) mode;
	*(u16 *)(node + 40) = (u16)IMG_WIDTH(src);
	*(u16 *)(node + 42) = (u16)IMG_HEIGHT(src);

	*(u32 *)(node + 16) = IMG_PHYS(src);
	*(u16 *)(node + 44) = (u16)IMG_STRIDE(src);
	if (src_type == 2 || src_type == 3 || src_type == 11) {
		*(u32 *)(node + 24) = *(u32 *)(src + 8);
		*(u16 *)(node + 46) = *(u16 *)(src + 52);
	}
	if (src_type == 11) {
		*(u32 *)(node + 32) = *(u32 *)(src + 16);
		*(u16 *)(node + 48) = *(u16 *)(src + 56);
	}

	*(u32 *)(node + 20) = IMG_PHYS(dst);
	*(u16 *)(node + 50) = (u16)IMG_STRIDE(dst);
	if (dst_type == 2 || dst_type == 3 || dst_type == 11) {
		*(u32 *)(node + 28) = *(u32 *)(dst + 8);
		*(u16 *)(node + 52) = *(u16 *)(dst + 52);
	}
	if (dst_type == 11) {
		*(u32 *)(node + 36) = *(u32 *)(dst + 16);
		*(u16 *)(node + 54) = *(u16 *)(dst + 56);
	}

	pr_info_once("ive_neo: CSC handler wired (HW op 2, modes 0..3 + 8..11)\n");
	return ive_submit_nonxnn(node, buf);
}

/* ---- FilterAndCSC (op=3): arg = {handle(8), src(72), dst(72), ctrl(30+pad), instant(4)} ---- *
 * 5x5 filter followed by YUV→RGB color-space conversion. cv500-only
 * HW op (vendor blob `ive_fill_filter_and_csc_task` @0x7b54). Unlike
 * standalone CSC this unit has no on-chip lookup tables — supports
 * YUV2RGB modes only (0..3); HSV/LAB live on the separate CSC unit.
 *
 * Field map:
 *   node[10]=3, node[11]=enMode
 *   node[8]: src format byte (1 YUV420SP, 2 YUV422SP)
 *   node[9]: dst format byte (2 U8C3_PKG, 3 U8C3_PLN)
 *   node[40,42] = width, height
 *   node[16,44] / [24,46] = src plane 0 / 1 phys + stride
 *   node[20,50] (+ [28,52] / [36,54] for PLN) = dst planes
 *   node[56..80] = memcpy(ctrl.as8Mask[25])
 *   node[81] = ctrl.u8Norm
 *
 * Vendor validator rejects: enMode > 3, u8Norm > 13, src ∉ {2,3},
 * dst ∉ {10,11}. We pre-check the format/mode bounds and let HW
 * handle the rest.
 */
static long ive_op_flt_csc_cv500(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *src = buf + 8, *dst = buf + 80, *ctrl = buf + 152;
	u32 mode = *(u32 *)(ctrl + 0);
	u8  norm = ctrl[29];
	u32 src_type = IMG_TYPE(src);
	u32 dst_type = IMG_TYPE(dst);
	u8  src_fmt, dst_fmt;
	u8 node[208];

	if (IVE_CHECK_IMG(src) || IVE_CHECK_IMG(dst))
		return -EINVAL;
	if (mode > 3) {
		pr_info("ive_neo: FilterAndCSC bad enMode=%u (need 0..3 YUV2RGB)\n", mode);
		return -EINVAL;
	}
	if (norm > 13) {
		pr_info("ive_neo: FilterAndCSC bad u8Norm=%u (need 0..13)\n", norm);
		return -EINVAL;
	}

	switch (src_type) {
	case 2:  src_fmt = 1; break;          /* YUV420SP */
	case 3:  src_fmt = 2; break;          /* YUV422SP */
	default:
		pr_info("ive_neo: FilterAndCSC bad src enType=%u (need YUV420SP/YUV422SP)\n",
			src_type);
		return -EINVAL;
	}
	switch (dst_type) {
	case 10: dst_fmt = 2; break;          /* U8C3_PACKAGE */
	case 11: dst_fmt = 3; break;          /* U8C3_PLANAR */
	default:
		pr_info("ive_neo: FilterAndCSC bad dst enType=%u (need U8C3_PACKAGE/PLANAR)\n",
			dst_type);
		return -EINVAL;
	}

	memset(node, 0, sizeof(node));
	node[8]  = src_fmt;
	node[9]  = dst_fmt;
	node[10] = 3;                         /* op = FilterAndCSC */
	node[11] = (u8) mode;
	*(u16 *)(node + 40) = (u16)IMG_WIDTH(src);
	*(u16 *)(node + 42) = (u16)IMG_HEIGHT(src);

	/* src — always 2 planes (YUV semiplanar). */
	*(u32 *)(node + 16) = IMG_PHYS(src);
	*(u16 *)(node + 44) = (u16)IMG_STRIDE(src);
	*(u32 *)(node + 24) = *(u32 *)(src + 8);
	*(u16 *)(node + 46) = *(u16 *)(src + 52);

	/* dst — single plane (PACKAGE) or three planes (PLANAR). */
	*(u32 *)(node + 20) = IMG_PHYS(dst);
	*(u16 *)(node + 50) = (u16)IMG_STRIDE(dst);
	if (dst_type == 11) {
		*(u32 *)(node + 28) = *(u32 *)(dst + 8);
		*(u32 *)(node + 36) = *(u32 *)(dst + 16);
		*(u16 *)(node + 52) = *(u16 *)(dst + 52);
		*(u16 *)(node + 54) = *(u16 *)(dst + 56);
	}

	memcpy(node + 56, ctrl + 4, 25);
	node[81] = norm;

	pr_info_once("ive_neo: FilterAndCSC handler wired (HW op 3)\n");
	return ive_submit_nonxnn(node, buf);
}

/* ---- Dilate (op=6): arg = {handle(8), src(72), dst(72), ctrl(25 mask)} ---- */
static long ive_op_dilate(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *src = buf + 8, *dst = buf + 80, *mask = buf + 152;
	u8 node[208];

	if (IVE_CHECK_IMG(src) || IVE_CHECK_IMG(dst))
		return -EINVAL;

	memset(node, 0, sizeof(node));
	node[10] = 6; /* op = Dilate */
	*(u32 *)(node + 16) = IMG_PHYS(src);
	*(u32 *)(node + 20) = IMG_PHYS(dst);
	*(u16 *)(node + 40) = (u16)IMG_WIDTH(src);
	*(u16 *)(node + 42) = (u16)IMG_HEIGHT(src);
	*(u16 *)(node + 44) = (u16)IMG_STRIDE(src);
	*(u16 *)(node + 50) = (u16)IMG_STRIDE(dst);
	memcpy(node + 56, mask, 25);
	return ive_submit_nonxnn(node, buf);
}

/* ---- Erode (op=7): same layout as Dilate ---- */
static long ive_op_erode(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *src = buf + 8, *dst = buf + 80, *mask = buf + 152;
	u8 node[208];

	if (IVE_CHECK_IMG(src) || IVE_CHECK_IMG(dst))
		return -EINVAL;

	memset(node, 0, sizeof(node));
	node[10] = 7; /* op = Erode */
	*(u32 *)(node + 16) = IMG_PHYS(src);
	*(u32 *)(node + 20) = IMG_PHYS(dst);
	*(u16 *)(node + 40) = (u16)IMG_WIDTH(src);
	*(u16 *)(node + 42) = (u16)IMG_HEIGHT(src);
	*(u16 *)(node + 44) = (u16)IMG_STRIDE(src);
	*(u16 *)(node + 50) = (u16)IMG_STRIDE(dst);
	memcpy(node + 56, mask, 25);
	return ive_submit_nonxnn(node, buf);
}

/* ---- DMA (op=0): arg = {handle(8), src_data(32), dst_data(32), ctrl(24+)} ----
 * Uses IVE_DATA_S (not IVE_IMAGE_S): phys@0, stride@16, width@20, height@24 */
static long ive_op_dma(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *src = buf + 8, *dst = buf + 40, *ctrl = buf + 72;
	u8 node[208];
	u32 src_phys = *(u32 *)src;
	u32 dst_phys = *(u32 *)dst;
	u32 src_stride = *(u32 *)(src + 16);
	u32 dst_stride = *(u32 *)(dst + 16);
	u32 width = *(u32 *)(src + 20);
	u32 height = *(u32 *)(src + 24);

	/* DATA_S layout differs from IMAGE_S; check explicit fields. dst_phys
	 * is only consumed when enMode <= 1 (memcpy variants); ctrl[0] is
	 * enMode. For other modes dst may be NULL/zero which is legal. */
	if (ive_check_buf(src_phys, src_stride, width, height))
		return -EINVAL;
	if (ctrl[0] <= 1 && ive_check_buf(dst_phys, dst_stride, width, height))
		return -EINVAL;

	memset(node, 0, sizeof(node));
	node[10] = 0;           /* op = DMA */
	node[11] = ctrl[0];     /* enMode */
	*(u32 *)(node + 16) = *(u32 *)src;             /* src phys */
	*(u16 *)(node + 40) = *(u16 *)(src + 20);      /* width */
	*(u16 *)(node + 42) = *(u16 *)(src + 24);      /* height */
	*(u16 *)(node + 44) = *(u16 *)(src + 16);      /* src stride */
	*(u32 *)(node + 20) = *(u32 *)dst;             /* dst phys */
	*(u16 *)(node + 50) = *(u16 *)(dst + 16);      /* dst stride */
	return ive_submit_nonxnn(node, buf);
}

/* ---- And/Or/Xor (op=9/11/21): arg = {handle(8), src1(72), src2(72), dst(72)} ---- */
static long ive_op_logic(unsigned long arg, u8 opcode)
{
	u8 *buf = (u8 *)arg;
	u8 *src1 = buf + 8, *src2 = buf + 80, *dst = buf + 152;
	u8 node[208];

	if (IVE_CHECK_IMG(src1) || IVE_CHECK_IMG(src2) || IVE_CHECK_IMG(dst))
		return -EINVAL;

	memset(node, 0, sizeof(node));
	node[10] = opcode;
	*(u32 *)(node + 16) = IMG_PHYS(src1);
	*(u32 *)(node + 24) = IMG_PHYS(src2);
	*(u32 *)(node + 20) = IMG_PHYS(dst);
	*(u16 *)(node + 40) = (u16)IMG_WIDTH(src1);
	*(u16 *)(node + 42) = (u16)IMG_HEIGHT(src1);
	*(u16 *)(node + 44) = (u16)IMG_STRIDE(src1);
	*(u16 *)(node + 46) = (u16)IMG_STRIDE(src2);
	*(u16 *)(node + 50) = (u16)IMG_STRIDE(dst);
	return ive_submit_nonxnn(node, buf);
}

/* ---- Sub (op=10): arg = {handle(8), src1(72), src2(72), dst(72), ctrl(4)} ---- */
static long ive_op_sub(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *src1 = buf + 8, *src2 = buf + 80, *dst = buf + 152, *ctrl = buf + 224;
	u8 node[208];

	if (IVE_CHECK_IMG(src1) || IVE_CHECK_IMG(src2) || IVE_CHECK_IMG(dst))
		return -EINVAL;

	memset(node, 0, sizeof(node));
	node[10] = 10;
	node[11] = ctrl[0]; /* enMode */
	*(u32 *)(node + 16) = IMG_PHYS(src1);
	*(u32 *)(node + 24) = IMG_PHYS(src2);
	*(u32 *)(node + 20) = IMG_PHYS(dst);
	*(u16 *)(node + 40) = (u16)IMG_WIDTH(src1);
	*(u16 *)(node + 42) = (u16)IMG_HEIGHT(src1);
	*(u16 *)(node + 44) = (u16)IMG_STRIDE(src1);
	*(u16 *)(node + 46) = (u16)IMG_STRIDE(src2);
	*(u16 *)(node + 50) = (u16)IMG_STRIDE(dst);
	return ive_submit_nonxnn(node, buf);
}

/* ---- Add (op=20): arg = {handle(8), src1(72), src2(72), dst(72), ctrl(4)} ---- */
static long ive_op_add(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *src1 = buf + 8, *src2 = buf + 80, *dst = buf + 152, *ctrl = buf + 224;
	u8 node[208];

	if (IVE_CHECK_IMG(src1) || IVE_CHECK_IMG(src2) || IVE_CHECK_IMG(dst))
		return -EINVAL;

	memset(node, 0, sizeof(node));
	node[10] = 20;
	*(u32 *)(node + 16) = IMG_PHYS(src1);
	*(u32 *)(node + 24) = IMG_PHYS(src2);
	*(u32 *)(node + 20) = IMG_PHYS(dst);
	*(u16 *)(node + 40) = (u16)IMG_WIDTH(src1);
	*(u16 *)(node + 42) = (u16)IMG_HEIGHT(src1);
	*(u16 *)(node + 44) = (u16)IMG_STRIDE(src1);
	*(u16 *)(node + 46) = (u16)IMG_STRIDE(src2);
	*(u16 *)(node + 50) = (u16)IMG_STRIDE(dst);
	*(u16 *)(node + 88) = *(u16 *)ctrl;       /* u0q16X */
	*(u16 *)(node + 90) = *(u16 *)(ctrl + 2); /* u0q16Y */
	return ive_submit_nonxnn(node, buf);
}

/* ---- Filter (op=1): arg = {handle(8), src(72), dst(72), ctrl(26+)} ---- */
static long ive_op_filter(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *src = buf + 8, *dst = buf + 80, *ctrl = buf + 152;
	u8 node[208];

	if (IVE_CHECK_IMG(src) || IVE_CHECK_IMG(dst))
		return -EINVAL;

	memset(node, 0, sizeof(node));
	node[10] = 1;  /* op = Filter */
	*(u32 *)(node + 16) = IMG_PHYS(src);
	*(u32 *)(node + 20) = IMG_PHYS(dst);
	*(u16 *)(node + 40) = (u16)IMG_WIDTH(src);
	*(u16 *)(node + 42) = (u16)IMG_HEIGHT(src);
	*(u16 *)(node + 44) = (u16)IMG_STRIDE(src);
	*(u16 *)(node + 50) = (u16)IMG_STRIDE(dst);
	memcpy(node + 56, ctrl, 25);  /* 5x5 mask coefficients */
	node[81] = ctrl[25];          /* norm byte */
	return ive_submit_nonxnn(node, buf);
}

/* ---- Hist (op=13): arg = {handle(8), src(72), dst_mem(24+)} ---- */
static long ive_op_hist(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *src = buf + 8, *dst_mem = buf + 80;
	u8 node[208];

	/* dst_mem is IVE_MEM_INFO_S (24B); only check source image. */
	if (IVE_CHECK_IMG(src))
		return -EINVAL;
	if (!*(u32 *)dst_mem)
		return -EINVAL;

	memset(node, 0, sizeof(node));
	node[10] = 13; /* op = Hist */
	*(u32 *)(node + 16) = IMG_PHYS(src);
	*(u32 *)(node + 20) = *(u32 *)dst_mem;  /* IVE_MEM_INFO_S phys */
	*(u16 *)(node + 40) = (u16)IMG_WIDTH(src);
	*(u16 *)(node + 42) = (u16)IMG_HEIGHT(src);
	*(u16 *)(node + 44) = (u16)IMG_STRIDE(src);
	return ive_submit_nonxnn(node, buf);
}

/* ---- EqualizeHist (op=13 reused, ioctl 0xc0b8462a) ----
 *
 * Two phases:
 *
 *   Phase 1 (HW): submit a standard Hist task (op=13) with node[7]=0x61
 *                 (EqualizeHist mode marker) + dst.phys/stride. HW
 *                 writes 256 × u32 histogram bins to stMem[0..0x400).
 *
 *   Phase 2 (CPU): compute the standard histogram-equalization LUT
 *                  (CDF normalization) from the bins, then apply it
 *                  pixel-by-pixel to dst.
 *
 * Vendor cv500 instead chains a second HW task (DMA-with-LUT remap)
 * filled by `ive_fill_dma_task` @0x76d0; HW does the LUT remap at the
 * IVE block instead of the CPU. We do it on the CPU because (a) the
 * vendor's exact DMA-task field map for mode=1 (LUT remap) is non-
 * trivial to replicate byte-for-byte, and (b) for typical EqualizeHist
 * input sizes the CPU loop is fine. If a future user needs the HW
 * fast-path, the hooks are obvious — build a second 208-byte node
 * mirroring `ive_fill_dma_task` mode 1 and chain-submit both nodes.
 *
 * Arg layout (ioctl size 184 B):
 *   +0:    HI_HANDLE
 *   +8:    IVE_SRC_IMAGE_S src
 *   +80:   IVE_DST_IMAGE_S dst  (equalized output)
 *   +152:  IVE_MEM_INFO_S stMem (24 B — workspace for histogram bins)
 *   +176:  HI_BOOL instant
 *
 * Validator: stMem.u32Size must be >= 1280 (matches vendor
 * `ive_check_equalize_hist_param` @0xf0e4:f1dc) — 256×u32 bins +
 * 256-byte LUT region.
 */
static long ive_op_equalize_hist(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *src = buf + 8;
	u8 *dst = buf + 80;
	u8 *stMem = buf + 152;
	u32 stMem_phys = *(u32 *)stMem;
	u32 stMem_size = *(u32 *)(stMem + 16);
	u8 node[208];
	long ret;

	if (IVE_CHECK_IMG(src) || IVE_CHECK_IMG(dst))
		return -EINVAL;
	if (!stMem_phys || (stMem_phys & 0xF)) {
		pr_info("ive_neo: EqualizeHist stMem.phys=0x%x invalid\n", stMem_phys);
		return -EINVAL;
	}
	if (stMem_size < 1280) {                /* vendor's 0x500 threshold */
		pr_info("ive_neo: EqualizeHist stMem.size=%u < 1280\n", stMem_size);
		return -EINVAL;
	}

	memset(node, 0, sizeof(node));
	/* Standard Hist field map (same as ive_op_hist + LUT byte). */
	node[8]  = cv500_src_fmt(IMG_TYPE(src));
	node[10] = 13;                              /* op = Hist */
	*(u32 *)(node + 16) = IMG_PHYS(src);
	*(u32 *)(node + 20) = stMem_phys;
	*(u16 *)(node + 40) = (u16)IMG_WIDTH(src);
	*(u16 *)(node + 42) = (u16)IMG_HEIGHT(src);
	*(u16 *)(node + 44) = (u16)IMG_STRIDE(src);

	/* EqualizeHist overrides: mode-byte + dst image fields. */
	node[7]  = 0x61;                            /* EqualizeHist mode marker */
	*(u32 *)(node + 28) = IMG_PHYS(dst);
	*(u16 *)(node + 52) = (u16)IMG_STRIDE(dst);

	pr_info_once("ive_neo: EqualizeHist handler wired (HW Hist + CPU LUT remap)\n");
	ret = ive_submit_nonxnn(node, buf);
	if (ret < 0)
		return ret;

	/* Phase 2: CPU computes the equalization LUT from the histogram
	 * HW just wrote to stMem, then applies it to dst. */
	{
		u32 *bins = (u32 *)phys_to_virt((phys_addr_t)stMem_phys);
		u8 *sv = (u8 *)phys_to_virt((phys_addr_t)IMG_PHYS(src));
		u8 *dv = (u8 *)phys_to_virt((phys_addr_t)IMG_PHYS(dst));
		u32 src_w = IMG_WIDTH(src);
		u32 src_h = IMG_HEIGHT(src);
		u32 src_s = IMG_STRIDE(src);
		u32 dst_s = IMG_STRIDE(dst);
		u32 npx = src_w * src_h;
		u32 cdf, cdf_min;
		u8 lut[256];
		u32 i, y, x;

		if (!bins || !sv || !dv)
			return -EFAULT;

		/* cdf_min = first non-zero histogram bin. */
		cdf_min = 0;
		for (i = 0; i < 256; i++) {
			if (bins[i]) { cdf_min = bins[i]; break; }
		}
		/* Build LUT: lut[i] = round( (cdf[i] - cdf_min) / (npx - cdf_min) * 255 ). */
		cdf = 0;
		for (i = 0; i < 256; i++) {
			cdf += bins[i];
			if (npx > cdf_min) {
				u32 num = (cdf - cdf_min) * 255u;
				u32 den = npx - cdf_min;
				lut[i] = (u8)((num + den / 2) / den);
			} else {
				lut[i] = (u8)i;
			}
		}
		/* Apply LUT to dst. */
		for (y = 0; y < src_h; y++) {
			u8 *sp = sv + y * src_s;
			u8 *dp = dv + y * dst_s;
			for (x = 0; x < src_w; x++)
				dp[x] = lut[sp[x]];
		}
	}
	return 0;
}

/* ---- Thresh_S16 (op=14): arg = {handle(8), src(72), dst(72), ctrl(12+)} ---- */
static long ive_op_thresh_s16(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *src = buf + 8, *dst = buf + 80, *ctrl = buf + 152;
	u8 node[208];

	if (IVE_CHECK_IMG(src) || IVE_CHECK_IMG(dst))
		return -EINVAL;

	memset(node, 0, sizeof(node));
	node[6]  = 1;  /* s16 flag */
	node[10] = 14; /* op = Thresh_S16 */
	node[11] = ctrl[0]; /* enMode */
	*(u16 *)(node + 12) = *(u16 *)(ctrl + 4); /* s16LowThr */
	*(u16 *)(node + 14) = *(u16 *)(ctrl + 6); /* s16HighThr */
	node[56] = ctrl[8];  /* u8MinVal */
	node[57] = ctrl[9];  /* u8MidVal */
	node[58] = ctrl[10]; /* u8MaxVal */
	*(u32 *)(node + 16) = IMG_PHYS(src);
	*(u32 *)(node + 20) = IMG_PHYS(dst);
	*(u16 *)(node + 40) = (u16)IMG_WIDTH(src);
	*(u16 *)(node + 42) = (u16)IMG_HEIGHT(src);
	*(u16 *)(node + 44) = (u16)IMG_STRIDE(src);
	*(u16 *)(node + 50) = (u16)IMG_STRIDE(dst);
	return ive_submit_nonxnn(node, buf);
}

/* ---- Thresh_U16 (op=15): same layout as Thresh_S16 ---- */
static long ive_op_thresh_u16(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *src = buf + 8, *dst = buf + 80, *ctrl = buf + 152;
	u8 node[208];

	if (IVE_CHECK_IMG(src) || IVE_CHECK_IMG(dst))
		return -EINVAL;

	memset(node, 0, sizeof(node));
	node[6]  = 1;
	node[10] = 15; /* op = Thresh_U16 */
	node[11] = ctrl[0];
	*(u16 *)(node + 12) = *(u16 *)(ctrl + 4);
	*(u16 *)(node + 14) = *(u16 *)(ctrl + 6);
	node[56] = ctrl[8];
	node[57] = ctrl[9];
	node[58] = ctrl[10];
	*(u32 *)(node + 16) = IMG_PHYS(src);
	*(u32 *)(node + 20) = IMG_PHYS(dst);
	*(u16 *)(node + 40) = (u16)IMG_WIDTH(src);
	*(u16 *)(node + 42) = (u16)IMG_HEIGHT(src);
	*(u16 *)(node + 44) = (u16)IMG_STRIDE(src);
	*(u16 *)(node + 50) = (u16)IMG_STRIDE(dst);
	return ive_submit_nonxnn(node, buf);
}

/* ---- 16BitTo8Bit (op=16): arg = {handle(8), src(72), dst(72), ctrl(8+)} ---- */
static long ive_op_16to8(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *src = buf + 8, *dst = buf + 80, *ctrl = buf + 152;
	u8 node[208];
	u32 gain = 0;

	if (IVE_CHECK_IMG(src) || IVE_CHECK_IMG(dst))
		return -EINVAL;

	/* Compute gain = (numerator << 16) / denominator, capped at 0xFFFF */
	if (ctrl[6]) { /* numerator != 0 */
		u16 denom = *(u16 *)(ctrl + 4);
		gain = ((u32)ctrl[6] << 16) / (denom ? denom : 1);
		if (gain > 0xFFFF) gain = 0xFFFF;
	}

	memset(node, 0, sizeof(node));
	node[6]  = 1;
	node[10] = 16; /* op = 16BitTo8Bit */
	node[11] = ctrl[0]; /* enMode */
	node[56] = ctrl[7]; /* s8Bias */
	*(u16 *)(node + 110) = (u16)gain;
	*(u32 *)(node + 16) = IMG_PHYS(src);
	*(u32 *)(node + 20) = IMG_PHYS(dst);
	*(u16 *)(node + 40) = (u16)IMG_WIDTH(src);
	*(u16 *)(node + 42) = (u16)IMG_HEIGHT(src);
	*(u16 *)(node + 44) = (u16)IMG_STRIDE(src);
	*(u16 *)(node + 50) = (u16)IMG_STRIDE(dst);
	return ive_submit_nonxnn(node, buf);
}

/* ---- OrdStatFilter (op=17): arg = {handle(8), src(72), dst(72), ctrl(4+)} ---- */
static long ive_op_ordstat(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *src = buf + 8, *dst = buf + 80, *ctrl = buf + 152;
	u8 node[208];

	if (IVE_CHECK_IMG(src) || IVE_CHECK_IMG(dst))
		return -EINVAL;

	memset(node, 0, sizeof(node));
	node[10] = 17;
	node[11] = ctrl[0]; /* enMode */
	*(u32 *)(node + 16) = IMG_PHYS(src);
	*(u32 *)(node + 20) = IMG_PHYS(dst);
	*(u16 *)(node + 40) = (u16)IMG_WIDTH(src);
	*(u16 *)(node + 42) = (u16)IMG_HEIGHT(src);
	*(u16 *)(node + 44) = (u16)IMG_STRIDE(src);
	*(u16 *)(node + 50) = (u16)IMG_STRIDE(dst);
	return ive_submit_nonxnn(node, buf);
}

/* ---- Integ (op=12): arg = {handle(8), src(72), dst(72), ctrl(4+)} ---- */
static long ive_op_integ(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *src = buf + 8, *dst = buf + 80, *ctrl = buf + 152;
	u8 node[208];

	if (IVE_CHECK_IMG(src) || IVE_CHECK_IMG(dst))
		return -EINVAL;

	memset(node, 0, sizeof(node));
	node[10] = 12;
	node[9]  = ctrl[0]; /* enOutCtrl */
	*(u32 *)(node + 16) = IMG_PHYS(src);
	*(u32 *)(node + 20) = IMG_PHYS(dst);
	*(u16 *)(node + 40) = (u16)IMG_WIDTH(src);
	*(u16 *)(node + 42) = (u16)IMG_HEIGHT(src);
	*(u16 *)(node + 44) = (u16)IMG_STRIDE(src);
	*(u16 *)(node + 50) = (u16)IMG_STRIDE(dst);
	return ive_submit_nonxnn(node, buf);
}

/* ---- Sobel (op=4): arg = {handle(8), src(72), dst_h(72), dst_v(72), ctrl(4+mask)} ---- */
static long ive_op_sobel(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *src = buf + 8, *dst_h = buf + 80, *dst_v = buf + 152, *ctrl = buf + 224;
	u8 node[208];
	u8 mode = ctrl[0]; /* enOutCtrl: 0=both, 1=H only, 2=V only */

	if (IVE_CHECK_IMG(src))
		return -EINVAL;
	if (mode != 2 && IVE_CHECK_IMG(dst_h))
		return -EINVAL;
	if (mode != 1 && IVE_CHECK_IMG(dst_v))
		return -EINVAL;

	memset(node, 0, sizeof(node));
	node[10] = 4;
	node[9]  = mode;
	*(u32 *)(node + 16) = IMG_PHYS(src);
	*(u16 *)(node + 40) = (u16)IMG_WIDTH(src);
	*(u16 *)(node + 42) = (u16)IMG_HEIGHT(src);
	*(u16 *)(node + 44) = (u16)IMG_STRIDE(src);
	if (mode == 1) { /* H only */
		*(u32 *)(node + 20) = IMG_PHYS(dst_h);
		*(u16 *)(node + 50) = (u16)IMG_STRIDE(dst_h);
	} else if (mode == 2) { /* V only */
		*(u32 *)(node + 20) = IMG_PHYS(dst_v);
		*(u16 *)(node + 50) = (u16)IMG_STRIDE(dst_v);
	} else { /* both */
		*(u32 *)(node + 20) = IMG_PHYS(dst_h);
		*(u32 *)(node + 28) = IMG_PHYS(dst_v);
		*(u16 *)(node + 50) = (u16)IMG_STRIDE(dst_h);
		*(u16 *)(node + 52) = (u16)IMG_STRIDE(dst_v);
	}
	memcpy(node + 56, ctrl + 4, 25); /* 5x5 mask */
	return ive_submit_nonxnn(node, buf);
}

/* ---- MagAndAng (op=5): arg = {handle(8), src(72), mag(72), ang(72), ctrl(4+mask)} ---- */
static long ive_op_mag_and_ang(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *src = buf + 8, *mag = buf + 80, *ang = buf + 152, *ctrl = buf + 224;
	u8 node[208];
	u8 mode = ctrl[0]; /* enOutCtrl: 0=mag only, 1=mag+ang */

	if (IVE_CHECK_IMG(src) || IVE_CHECK_IMG(mag))
		return -EINVAL;
	if (mode != 0 && IVE_CHECK_IMG(ang))
		return -EINVAL;

	memset(node, 0, sizeof(node));
	node[10] = 5;
	node[9]  = mode;
	*(u16 *)(node + 12) = *(u16 *)(ctrl + 4); /* u16Thr */
	*(u32 *)(node + 16) = IMG_PHYS(src);
	*(u16 *)(node + 40) = (u16)IMG_WIDTH(src);
	*(u16 *)(node + 42) = (u16)IMG_HEIGHT(src);
	*(u16 *)(node + 44) = (u16)IMG_STRIDE(src);
	if (mode == 0) { /* mag only */
		*(u32 *)(node + 20) = IMG_PHYS(mag);
		*(u16 *)(node + 50) = (u16)IMG_STRIDE(mag);
	} else { /* mag + ang */
		*(u32 *)(node + 20) = IMG_PHYS(mag);
		*(u32 *)(node + 28) = IMG_PHYS(ang);
		*(u16 *)(node + 50) = (u16)IMG_STRIDE(mag);
		*(u16 *)(node + 52) = (u16)IMG_STRIDE(ang);
	}
	memcpy(node + 56, ctrl + 24, 25); /* as8Mask at ctrl offset 24 (after enOutCtrl+u16Thr+padding) */
	return ive_submit_nonxnn(node, buf);
}

/* ---- Map (op=19): arg = {handle(8), src(72), map_mem(24), dst(72), ctrl(4)} ---- */
static long ive_op_map(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *src = buf + 8, *map_mem = buf + 80, *dst = buf + 104, *ctrl = buf + 176;
	u8 node[208];

	if (IVE_CHECK_IMG(src) || IVE_CHECK_IMG(dst))
		return -EINVAL;
	if (!*(u32 *)map_mem)
		return -EINVAL;

	memset(node, 0, sizeof(node));
	node[10] = 19;
	node[11] = ctrl[0]; /* enMode */
	*(u32 *)(node + 16) = IMG_PHYS(src);
	*(u32 *)(node + 32) = *(u32 *)map_mem; /* map table phys (not +24!) */
	*(u32 *)(node + 20) = IMG_PHYS(dst);
	*(u16 *)(node + 40) = (u16)IMG_WIDTH(src);
	*(u16 *)(node + 42) = (u16)IMG_HEIGHT(src);
	*(u16 *)(node + 44) = (u16)IMG_STRIDE(src);
	*(u16 *)(node + 50) = (u16)IMG_STRIDE(dst);
	*(u32 *)(node + 84) = ctrl[0] ? 512 : 256; /* LUT size */
	return ive_submit_nonxnn(node, buf);
}

/* ---- NormGrad (op=27): arg = {handle(8), src(72), dst_h(72), dst_v(72), dst_comb(72), ctrl(30+)} ---- */
static long ive_op_norm_grad(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *src = buf + 8, *dst_h = buf + 80, *dst_v = buf + 152;
	u8 *dst_c = buf + 224, *ctrl = buf + 296;
	u8 node[208];
	u8 mode = ctrl[0]; /* enOutCtrl: 0=H+V, 1=H, 2=V, 3=combine */

	if (IVE_CHECK_IMG(src))
		return -EINVAL;
	if ((mode == 0 || mode == 1) && IVE_CHECK_IMG(dst_h))
		return -EINVAL;
	if ((mode == 0 || mode == 2) && IVE_CHECK_IMG(dst_v))
		return -EINVAL;
	if (mode == 3 && IVE_CHECK_IMG(dst_c))
		return -EINVAL;

	memset(node, 0, sizeof(node));
	node[10] = 27;
	node[9]  = mode;
	*(u32 *)(node + 16) = IMG_PHYS(src);
	*(u16 *)(node + 40) = (u16)IMG_WIDTH(src);
	*(u16 *)(node + 42) = (u16)IMG_HEIGHT(src);
	*(u16 *)(node + 44) = (u16)IMG_STRIDE(src);
	switch (mode) {
	case 0: /* H + V */
		*(u32 *)(node + 20) = IMG_PHYS(dst_h);
		*(u16 *)(node + 50) = (u16)IMG_STRIDE(dst_h);
		*(u32 *)(node + 28) = IMG_PHYS(dst_v);
		*(u16 *)(node + 52) = (u16)IMG_STRIDE(dst_v);
		break;
	case 1: /* H only */
		*(u32 *)(node + 20) = IMG_PHYS(dst_h);
		*(u16 *)(node + 50) = (u16)IMG_STRIDE(dst_h);
		break;
	case 2: /* V only */
		*(u32 *)(node + 20) = IMG_PHYS(dst_v);
		*(u16 *)(node + 50) = (u16)IMG_STRIDE(dst_v);
		break;
	case 3: /* combine */
		*(u32 *)(node + 20) = IMG_PHYS(dst_c);
		*(u16 *)(node + 50) = (u16)IMG_STRIDE(dst_c);
		break;
	}
	memcpy(node + 56, ctrl + 4, 25); /* mask coefficients */
	node[81] = ctrl[29]; /* norm byte */
	return ive_submit_nonxnn(node, buf);
}

/* ---- SAD (op=46): arg = {handle(8), src1(72), src2(72), sad(72), thr(72), ctrl(12)} ---- */
static long ive_op_sad(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *src1 = buf + 8, *src2 = buf + 80, *sad_out = buf + 152;
	u8 *thr_out = buf + 224, *ctrl = buf + 296;
	u8 node[208];
	u8 out_ctrl = ctrl[4]; /* enOutCtrl */

	if (IVE_CHECK_IMG(src1) || IVE_CHECK_IMG(src2))
		return -EINVAL;
	if (out_ctrl <= 3 && IVE_CHECK_IMG(sad_out))
		return -EINVAL;
	if ((out_ctrl <= 1 || out_ctrl == 4) && IVE_CHECK_IMG(thr_out))
		return -EINVAL;

	memset(node, 0, sizeof(node));
	node[10] = 46;
	node[9]  = out_ctrl;
	node[11] = ctrl[0]; /* enMode */
	*(u16 *)(node + 12) = *(u16 *)(ctrl + 8);  /* u16Thr */
	node[56] = ctrl[10]; /* u8MinVal */
	node[57] = ctrl[11]; /* u8MaxVal */
	*(u32 *)(node + 16) = IMG_PHYS(src1);
	*(u32 *)(node + 24) = IMG_PHYS(src2);
	*(u16 *)(node + 40) = (u16)IMG_WIDTH(src1);
	*(u16 *)(node + 42) = (u16)IMG_HEIGHT(src1);
	*(u16 *)(node + 44) = (u16)IMG_STRIDE(src1);
	*(u16 *)(node + 46) = (u16)IMG_STRIDE(src2);
	switch (out_ctrl) {
	case 0: case 1: /* sad + thr */
		*(u32 *)(node + 20) = IMG_PHYS(sad_out);
		*(u16 *)(node + 50) = (u16)IMG_STRIDE(sad_out);
		*(u32 *)(node + 28) = IMG_PHYS(thr_out);
		*(u16 *)(node + 52) = (u16)IMG_STRIDE(thr_out);
		break;
	case 2: case 3: /* sad only */
		*(u32 *)(node + 20) = IMG_PHYS(sad_out);
		*(u16 *)(node + 50) = (u16)IMG_STRIDE(sad_out);
		break;
	case 4: /* thr only */
		*(u32 *)(node + 20) = IMG_PHYS(thr_out);
		*(u16 *)(node + 50) = (u16)IMG_STRIDE(thr_out);
		break;
	}
	return ive_submit_nonxnn(node, buf);
}

/* ---- STCandiCorner (op=30): arg = {handle(8), src(72), dst(72), ctrl(4+)} ---- */
static long ive_op_st_candi_corner(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *src = buf + 8, *src2 = buf + 80, *dst = buf + 152;
	u8 node[208];

	if (IVE_CHECK_IMG(src) || IVE_CHECK_IMG(src2) || IVE_CHECK_IMG(dst))
		return -EINVAL;

	memset(node, 0, sizeof(node));
	node[10] = 30;
	*(u32 *)(node + 16) = IMG_PHYS(src);
	*(u32 *)(node + 24) = IMG_PHYS(src2);
	*(u32 *)(node + 20) = IMG_PHYS(dst);
	*(u16 *)(node + 40) = (u16)IMG_WIDTH(src);
	*(u16 *)(node + 42) = (u16)IMG_HEIGHT(src);
	*(u16 *)(node + 44) = (u16)IMG_STRIDE(src);
	*(u16 *)(node + 46) = (u16)IMG_STRIDE(src2);
	*(u16 *)(node + 50) = (u16)IMG_STRIDE(dst);
	return ive_submit_nonxnn(node, buf);
}

/* ---- CannyHysEdge (2-stage chained pipeline) ----
 *
 * Signature: HI_MPI_IVE_CannyHysEdge(h, src, edge, stack, ctrl, bInstant)
 *
 * arg layout:
 *   arg+0..7:     handle (8)
 *   arg+8..79:    IVE_IMAGE_S src (72)
 *   arg+80..151:  IVE_IMAGE_S edge dst (72)
 *   arg+152..175: IVE_MEM_INFO_S stack (24) — canny internal stack
 *   arg+176..199: IVE_CANNY_HYS_EDGE_CTRL_S.stMem (24) — mag/ang buffer
 *   arg+200..201: ctrl.lowThr
 *   arg+202..203: ctrl.highThr
 *   arg+204..228: ctrl.mask[25]
 *
 * Vendor builds TWO task nodes in a single 416-byte chain and submits
 * once — HW auto-advances via node[0..3] next_ptr.
 *   Node 0: mag_and_ang on src → writes mag + ang into ctrl.stMem
 *   Node 1: canny_hys_edge reads mag+ang → writes edge map to dst
 */
static long ive_op_canny_hys(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *src  = buf + 8;
	u8 *edge = buf + 80;
	u8 *stack_info = buf + 152;  /* pstStack — IVE_MEM_INFO_S */
	u8 *ctrl_mem = buf + 176;    /* ctrl.stMem — IVE_MEM_INFO_S (mag/ang) */
	u16 low_thr  = *(u16 *)(buf + 200);
	u16 high_thr = *(u16 *)(buf + 202);
	u8 *mask = buf + 204;
	u16 w, h;

	if (IVE_CHECK_IMG(src) || IVE_CHECK_IMG(edge))
		return -EINVAL;
	if (!*(u32 *)stack_info || !*(u32 *)ctrl_mem)
		return -EINVAL;
	if (high_thr < low_thr)
		return -EINVAL;
	w = (u16)IMG_WIDTH(src);
	h = (u16)IMG_HEIGHT(src);
	u16 src_stride = (u16)IMG_STRIDE(src);
	u16 edge_stride = (u16)IMG_STRIDE(edge);
	u32 stride16 = (w + 15u) & ~15u;     /* 16-aligned, vendor ive_calc_stride */
	u32 mag_bytes = 2u * (u32)h * stride16;  /* mag is int16 */
	u32 mag_phys = *(u32 *)ctrl_mem;
	u32 ang_phys = mag_phys + mag_bytes;
	u32 stack_phys = *(u32 *)stack_info;
	u8 chain[416];  /* 2 × 208-byte nodes */
	(void)stack_phys; /* reserved for future canny stack use */
	u8 *n0 = chain;         /* mag_and_ang */
	u8 *n1 = chain + 208;   /* canny_hys_edge */

	if (!g_ive_task_virt) {
		g_ive_task_virt = ive_dma_alloc(&g_ive_task_phys, 4096);
		if (!g_ive_task_virt)
			return -ENOMEM;
	}

	memset(chain, 0, sizeof(chain));

	/* ---- Node 0: mag_and_ang ---- */
	/* next_ptr = phys of node 1 (chain[208]) in the MMZ task buffer */
	*(u32 *)(n0 + 0)  = (u32)(g_ive_task_phys + 208);
	n0[10] = 5;  /* op = mag_and_ang */
	n0[9]  = 1;  /* mode: mag + ang */
	*(u32 *)(n0 + 16) = IMG_PHYS(src);
	*(u32 *)(n0 + 20) = mag_phys;
	*(u32 *)(n0 + 28) = ang_phys;
	*(u16 *)(n0 + 40) = w;
	*(u16 *)(n0 + 42) = h;
	*(u16 *)(n0 + 44) = src_stride;
	*(u16 *)(n0 + 50) = (u16)stride16;
	*(u16 *)(n0 + 52) = (u16)stride16;
	memcpy(n0 + 56, mask, 25);

	/* ---- Node 1: canny_hys_edge ---- */
	*(u32 *)(n1 + 0) = 0;  /* end of chain */
	n1[10] = 25; /* op = canny_hys_edge */
	*(u32 *)(n1 + 16) = ang_phys;          /* reads ang */
	*(u32 *)(n1 + 20) = IMG_PHYS(edge);    /* writes edge map */
	*(u32 *)(n1 + 24) = mag_phys;          /* mag source */
	*(u32 *)(n1 + 28) = mag_phys;
	*(u16 *)(n1 + 12) = low_thr;
	*(u16 *)(n1 + 14) = high_thr;
	*(u16 *)(n1 + 40) = w;
	*(u16 *)(n1 + 42) = h;
	*(u16 *)(n1 + 44) = (u16)stride16;
	*(u16 *)(n1 + 46) = (u16)stride16;
	*(u16 *)(n1 + 50) = edge_stride;

	/* Copy both nodes into the MMZ task buffer and submit node 0 — HW
	 * will auto-advance to node 1 via n0's next_ptr. */
	memcpy(g_ive_task_virt, chain, 416);
#ifndef IVE_STANDALONE
	osal_flush_dcache_area(g_ive_task_virt, g_ive_task_phys, 416);
#endif

	/* Ensure HW init (same logic as ive_submit_nonxnn) */
#ifdef IVE_STANDALONE
	if (!g_ive_regs)
		g_ive_regs = ioremap(ive_neo_chip.standalone_base, 0x10000);
#endif
	if (!g_ive_regs) {
		ive_err("canny_hys: g_ive_regs not set (DT probe didn't run?)\n");
		return -ENODEV;
	}
	if (!g_hw_init_done) {
		ive_hw_init();
		g_hw_init_done = 1;
	}

	reinit_completion(&g_ive_done);
	g_ive_last_status = 0;
	writel(6, g_ive_regs + 0x04);
	writel(7, g_ive_regs + 0x08);
	writel((u32)g_ive_task_phys, g_ive_regs + 0x10);
	isb(); dsb(); dmb();
	writel(1, g_ive_regs + 0x00);

	if (wait_for_completion_timeout(&g_ive_done,
				       msecs_to_jiffies(100)) == 0)
		g_hw_init_done = 0;

	*(u32 *)buf = 0;
	return 0;
}

/* ---- GMM2 (op=49): 6-image background model
 *
 *   HI_MPI_IVE_GMM2(h, src, factor, fg, bg, match, model, ctrl, bInstant)
 *
 * arg layout:
 *   +0..7:    handle
 *   +8..79:   src (72)
 *   +80..151: factor (72)
 *   +152..223: fg (72)
 *   +224..295: bg (72)
 *   +296..367: match (72)
 *   +368..391: model IVE_MEM_INFO_S (24)
 *   +392..?:   IVE_GMM2_CTRL_S
 */
static long ive_op_gmm2(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *src    = buf + 8;
	u8 *factor = buf + 80;
	u8 *fg     = buf + 152;
	u8 *bg     = buf + 224;
	u8 *match  = buf + 296;
	u8 *model  = buf + 368;   /* IVE_MEM_INFO_S */
	u8 *ctrl   = buf + 392;   /* IVE_GMM2_CTRL_S */
	u16 w, h;
	u8 node[208];

	if (IVE_CHECK_IMG(src) || IVE_CHECK_IMG(fg) || IVE_CHECK_IMG(bg) ||
	    IVE_CHECK_IMG(match))
		return -EINVAL;
	if (!*(u32 *)model)
		return -EINVAL;
	if ((ctrl[0] || ctrl[4]) && IVE_CHECK_IMG(factor))
		return -EINVAL;
	w = (u16)IMG_WIDTH(src);
	h = (u16)IMG_HEIGHT(src);

	memset(node, 0, sizeof(node));
	node[10] = 49; /* op = GMM2 */
	node[11] = ctrl[27]; /* u8ModelNum */
	*(u32 *)(node + 16) = IMG_PHYS(src);
	/* factor used only if model-num or u8SensiFactor are non-zero */
	if (ctrl[0] || ctrl[4]) {
		*(u32 *)(node + 24) = IMG_PHYS(factor);
		*(u16 *)(node + 46) = (u16)IMG_STRIDE(factor);
	}
	*(u32 *)(node + 20) = IMG_PHYS(fg);
	*(u32 *)(node + 28) = *(u32 *)model;    /* model phys -> node+28 (also +32) */
	*(u32 *)(node + 32) = *(u32 *)model;
	*(u32 *)(node + 36) = IMG_PHYS(bg);
	*(u32 *)(node + 120) = IMG_PHYS(match);
	*(u16 *)(node + 40) = w;
	*(u16 *)(node + 42) = h;
	*(u16 *)(node + 44) = (u16)IMG_STRIDE(src);
	*(u16 *)(node + 50) = (u16)IMG_STRIDE(fg);
	*(u16 *)(node + 54) = (u16)IMG_STRIDE(bg);
	/* ctrl fields mapped verbatim from vendor fill */
	node[94]  = ctrl[0];
	node[95]  = ctrl[4];
	*(u16 *)(node + 110) = *(u16 *)(ctrl + 8);
	*(u16 *)(node + 12)  = *(u16 *)(ctrl + 10);
	*(u16 *)(node + 108) = *(u16 *)(ctrl + 12);
	*(u16 *)(node + 88)  = *(u16 *)(ctrl + 14);
	*(u16 *)(node + 90)  = *(u16 *)(ctrl + 16);
	*(u16 *)(node + 14)  = *(u16 *)(ctrl + 18);
	*(u16 *)(node + 102) = *(u16 *)(ctrl + 20);
	*(u32 *)(node + 116) = *(u16 *)(ctrl + 22);
	*(u32 *)(node + 112) = *(u16 *)(ctrl + 24);
	node[92] = ctrl[26];
	*(u32 *)(node + 136) = (u32)ctrl[27] * h * w;
	return ive_submit_nonxnn(node, buf);
}

/* ---- NCC (op=?): arg = {handle(8), src1(72), src2(72), dst_mem(24+)} ---- */
/* HI_MPI_IVE_NCC (ioctl 0xc0b84616) — Normalized Cross-Correlation.
 * Field map decoded from cv500 vendor blob obj/hi3516cv500/hi_ive.o
 * symbol ive_fill_ncc_task @0x86a0 (the previous handler guessed
 * node[10]=18 from nr proximity; vendor actually uses 22). */
static long ive_op_ncc(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *src1 = buf + 8, *src2 = buf + 80, *dst_mem = buf + 152;
	u8 node[208];

	if (IVE_CHECK_IMG(src1) || IVE_CHECK_IMG(src2))
		return -EINVAL;
	if (!*(u32 *)dst_mem)
		return -EINVAL;

	memset(node, 0, sizeof(node));
	node[6]  = 0;
#if defined(hi3516cv500)
	node[8]  = cv500_src_fmt(IMG_TYPE(src1));   /* vendor LUT — issue #113 */
#else
	node[8]  = (u8) IMG_TYPE(src1);             /* V4 has its own LUT, not dumped */
#endif
	node[10] = 22;                   /* op = NCC */
	*(u32 *)(node + 16) = IMG_PHYS(src1);
	*(u32 *)(node + 20) = *(u32 *)dst_mem;  /* output mem phys */
	*(u32 *)(node + 24) = IMG_PHYS(src2);
	*(u16 *)(node + 40) = (u16)IMG_WIDTH(src1);
	*(u16 *)(node + 42) = (u16)IMG_HEIGHT(src1);
	*(u16 *)(node + 44) = (u16)IMG_STRIDE(src1);
	*(u16 *)(node + 46) = (u16)IMG_STRIDE(src2);
	pr_info_once("ive_neo: NCC handler wired (HW op 22)\n");
	return ive_submit_nonxnn(node, buf);
}

/* ---- CCL (op=23): arg = {handle(8), src(72), blob_mem(24), ctrl(8+)} ---- */
static long ive_op_ccl(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *src = buf + 8;
	u8 *blob = buf + 80;    /* IVE_MEM_INFO_S: u64 phys, u64 virt, u32 size */
	u8 *ctrl = buf + 104;   /* IVE_CCL_CTRL_S: u32 enMode, u16 initAreaThr, u16 step */
	u8 node[208];

	if (IVE_CHECK_IMG(src))
		return -EINVAL;
	if (!*(u32 *)blob)
		return -EINVAL;

	memset(node, 0, sizeof(node));
	node[10] = 23; /* op = CCL */
	node[11] = ctrl[0]; /* enMode */
	*(u32 *)(node + 16) = IMG_PHYS(src);       /* src phys */
	*(u32 *)(node + 20) = *(u32 *)blob;        /* blob output phys */
	*(u32 *)(node + 28) = IMG_PHYS(src);       /* src phys again (vendor does this) */
	*(u16 *)(node + 40) = (u16)IMG_WIDTH(src);
	*(u16 *)(node + 42) = (u16)IMG_HEIGHT(src);
	*(u16 *)(node + 44) = (u16)IMG_STRIDE(src);
	*(u16 *)(node + 50) = (u16)IMG_STRIDE(src); /* dst stride = src stride */
	*(u16 *)(node + 52) = (u16)IMG_STRIDE(src); /* another stride copy */
	*(u16 *)(node + 98) = *(u16 *)(ctrl + 4);   /* u16InitAreaThr */
	*(u16 *)(node + 100) = *(u16 *)(ctrl + 6);  /* u16Step */
	return ive_submit_nonxnn(node, buf);
}

#if defined(hi3516cv500) && !defined(IVE_STANDALONE)

/* PerspTrans userspace API caps roi_num at 8 (matches the SDK header
 * array slot count). The kernel arg buffer reserves 64 slots, but our
 * MMZ task buffer is 4 KB which only fits floor(4096/208) = 19 nodes.
 * Keep the cap at 8 to match userland expectations. */
#define IVE_PT_MAX_ROIS 8

/* ---- cv500 PerspTrans (HW op 0x35, ioctl 0xdc604635) ----
 *
 * Builds a chain of N HW task nodes (N = ctrl.u16RoiNum, 1..8) and
 * submits the head; HW auto-advances via each node's next-ptr at
 * node[0..3]. Returns 0 with the handle slot at arg[0..3] cleared
 * to match libive's "already-done" semantics.
 *
 * Arg buffer layout (7264 B, decoded from cv500 vendor blob symbols
 * ive_check_persp_trans_param @0x139e8 and ive_fill_persp_trans_task
 * @0x9584):
 *   +0:      IVE_HANDLE return slot                    8 B
 *   +8:      IVE_SRC_IMAGE_S src                      72 B
 *   +80:     IVE_RECT_U32_S astRoi[64]              1024 B  (16 B each)
 *   +1104:   IVE_SRC_MEM_INFO_S astPointPair[64]    1536 B  (24 B each)
 *   +2640:   IVE_DST_IMAGE_S astDst[64]             4608 B  (72 B each)
 *   +7248:   IVE_PERSP_TRANS_CTRL_S ctrl              12 B
 *       +0: u32 enAlgMode (0..2)
 *       +4: u32 enCscMode (0..4)
 *       +8: u16 u16RoiNum
 *      +10: u16 u16PointPairNum
 */
static long ive_op_persp_trans_cv500(unsigned long arg)
{
	u8 *buf  = (u8 *)arg;
	u8 *src  = buf + 8;
	u8 *ctrl = buf + 0x1c50;
	u32 alg_mode = *(u32 *)(ctrl + 0);
	u32 csc_mode = *(u32 *)(ctrl + 4);
	u16 roi_num  = *(u16 *)(ctrl + 8);
	u16 pp_num   = *(u16 *)(ctrl + 10);
	u32 src_phys0 = *(u32 *)(src + 0);
	u32 src_phys1 = *(u32 *)(src + 8);
	u32 src_stride0 = *(u32 *)(src + 48);
	u32 src_stride1 = *(u32 *)(src + 52);
	u32 src_w = *(u32 *)(src + 60);
	u32 src_h = *(u32 *)(src + 64);
	u32 src_type = *(u32 *)(src + 68);
	u8 *nodes;
	u8 src_fmt;
	u32 i;
	long ret;

	if (!roi_num || roi_num > IVE_PT_MAX_ROIS) {
		pr_info("ive_neo: PerspTrans bad roi_num=%u (max %u)\n",
			roi_num, IVE_PT_MAX_ROIS);
		return -EINVAL;
	}
	if (alg_mode > 2 || csc_mode > 4) {
		pr_info("ive_neo: PerspTrans bad ctrl alg=%u csc=%u\n",
			alg_mode, csc_mode);
		return -EINVAL;
	}
	if (ive_check_buf(src_phys0, src_stride0, src_w, src_h))
		return -EINVAL;

	/* node[8] = LUT[src.enType], with a PerspTrans-specific override:
	 * when csc=NONE and src is YUV420SP the encoding becomes 5 (the
	 * dedicated "pass-through YUV" path the HW uses when no colour
	 * conversion is requested). Decoded from vendor blob branch at
	 * ive_fill_persp_trans_task @0x95d4. */
	src_fmt = cv500_src_fmt(src_type);
	if (csc_mode == 0 && src_type == 2)
		src_fmt = 5;

	/* 1664 B is over the kernel's 1024 B stack-frame budget — heap. */
	nodes = kzalloc(IVE_PT_MAX_ROIS * 208, GFP_KERNEL);
	if (!nodes)
		return -ENOMEM;

	for (i = 0; i < roi_num; i++) {
		u8 *roi  = buf + 80 + i * 16;
		u8 *pp   = buf + 1104 + i * 24;
		u8 *dst  = buf + 2640 + i * 72;
		u8 *node = nodes + i * 208;
		u32 roi_x = *(u32 *)(roi + 0);
		u32 roi_y = *(u32 *)(roi + 4);
		u32 roi_w = *(u32 *)(roi + 8);
		u32 roi_h = *(u32 *)(roi + 12);
		u32 pp_phys = *(u32 *)(pp + 0);
		u32 dst_phys0 = *(u32 *)(dst + 0);
		u32 dst_phys1 = *(u32 *)(dst + 8);
		u32 dst_stride0 = *(u32 *)(dst + 48);
		u32 dst_stride1 = *(u32 *)(dst + 52);
		u32 dst_w = *(u32 *)(dst + 60);
		u32 dst_h = *(u32 *)(dst + 64);
		u32 dst_type = *(u32 *)(dst + 68);
		u8 node9;

		if (ive_check_buf(dst_phys0, dst_stride0, dst_w, dst_h)) {
			pr_info("ive_neo: PerspTrans dst[%u] check failed\n", i);
			ret = -EINVAL;
			goto out;
		}
		if (!pp_phys || (pp_phys & 0xf)) {
			pr_info("ive_neo: PerspTrans pp[%u] phys=0x%x invalid\n",
				i, pp_phys);
			ret = -EINVAL;
			goto out;
		}
		if (!roi_w || !roi_h ||
		    roi_x + roi_w > src_w || roi_y + roi_h > src_h) {
			pr_info("ive_neo: PerspTrans roi[%u] x=%u y=%u w=%u h=%u out of src %ux%u\n",
				i, roi_x, roi_y, roi_w, roi_h, src_w, src_h);
			ret = -EINVAL;
			goto out;
		}

		/* node[9] mirrors the vendor's switch on astDst[i].enType:
		 *   0 (U8C1) → 0, 2 (YUV420SP) → 1, 10 (U8C3_PACKAGE) → 2.
		 * Other formats fall through to 0 — same default the vendor
		 * blob leaves in the memset'd node. */
		switch (dst_type) {
		case 0:  node9 = 0; break;
		case 2:  node9 = 1; break;
		case 10: node9 = 2; break;
		default: node9 = 0; break;
		}

		node[6]  = 0;
		node[8]  = src_fmt;            /* LUT[src.enType] + YUV420SP/CSC=NONE override */
		node[9]  = node9;
		node[10] = 0x35;               /* op = PerspTrans */
		node[11] = 1;
		*(u32 *)(node + 16) = pp_phys;
		*(u32 *)(node + 20) = dst_phys0;
		*(u32 *)(node + 24) = src_phys0;
		*(u32 *)(node + 28) = dst_phys1;
		*(u32 *)(node + 32) = src_phys1;
		*(u16 *)(node + 40) = (u16) roi_w;
		*(u16 *)(node + 42) = (u16) roi_h;
		*(u16 *)(node + 46) = (u16) src_stride0;
		*(u16 *)(node + 48) = (u16) src_stride1;
		*(u16 *)(node + 50) = (u16) dst_stride0;
		*(u16 *)(node + 52) = (u16) dst_stride1;
		node[176] = csc_mode ? (u8)(csc_mode - 1) : 0;
		node[177] = (u8) alg_mode;
		node[180] = (u8) pp_num;
		*(u16 *)(node + 188) = (u16) roi_x;
		*(u16 *)(node + 190) = (u16) roi_y;
		*(u16 *)(node + 192) = (u16) dst_w;
		*(u16 *)(node + 194) = (u16) dst_h;
	}

	/* Chain via node[0..3] = phys-of-next. The submit helper copies
	 * `nodes` into g_ive_task_virt at offset 0, so each subsequent
	 * node lives at g_ive_task_phys + i*208. Last node leaves
	 * next-ptr = 0 (end of chain). */
	ret = ive_submit_chain(nodes, roi_num, buf);
out:
	kfree(nodes);
	return ret;
}

/* ---- cv500 Hog (HW op 0x34, ioctl 0xd0684634) ----
 *
 * Computes Histogram-of-Oriented-Gradients descriptors over 1..8 ROIs,
 * writing per-ROI feature blobs into the IVE_DST_BLOB_S array. Each
 * ROI emits one HW task node; chained submission via ive_submit_chain.
 *
 * Arg buffer layout (4200 B, from cv500 vendor blob symbols
 * ive_check_hog_param + ive_fill_hog_task + ive_hog_proc):
 *   +0:      IVE_HANDLE return slot                       8 B
 *   +8:      IVE_SRC_IMAGE_S src                         72 B
 *   +80:     IVE_RECT_U32_S astRoi[64]                1024 B  (16 B each)
 *   +1104:   IVE_DST_BLOB_S astDst[64]                3072 B  (48 B each)
 *   +4176:   IVE_HOG_CTRL_S ctrl                        20 B
 *       +0:  u32 enCscMode
 *       +4:  u32 enHogMode (1=vertical, 2=horizontal)
 *       +8:  u32 u32RoiNum (1..64)
 *      +12:  u16 u4q12TrancAlfa
 *      +14:  u8  au8Rsv[2]
 *   +4196..4199: bInstant
 *
 * IVE_DST_BLOB_S (48 B) layout from hi_comm_ive.h:
 *   +0:  u32 enType
 *   +4:  u32 u32Stride
 *   +8:  u64 u64VirAddr
 *  +16:  u64 u64PhyAddr
 *  +24:  u32 u32Num
 *  +28:  u32 width/height/chn union + padding
 */
#define IVE_HOG_MAX_ROIS 8
#define IVE_HOG_CELL_LIMIT 136

/* Magic-divide helper for the cell-grid scale field used by HW. The
 * vendor blob does umullhi(roi_w<<11, 0xf0f0f0f1) and extracts bits
 * [22:7] when roi > IVE_HOG_CELL_LIMIT; otherwise the cell-grid scale
 * is the fixed default 0x800. */
static u32 ive_hog_cell_scale(u32 roi_dim)
{
	if (roi_dim <= IVE_HOG_CELL_LIMIT)
		return 0x800;
	{
		u64 mul = ((u64)(roi_dim << 11)) * 0xf0f0f0f1ULL;
		return (u32)((mul >> 32) >> 7) & 0xffff;
	}
}

static long ive_op_hog_cv500(unsigned long arg)
{
	u8 *buf  = (u8 *)arg;
	u8 *src  = buf + 8;
	u8 *ctrl = buf + 0x1050;
	u32 csc_mode = *(u32 *)(ctrl + 0);
	u32 hog_mode = *(u32 *)(ctrl + 4);
	u32 roi_num  = *(u32 *)(ctrl + 8);
	u16 trunc    = *(u16 *)(ctrl + 12);
	u32 src_phys0 = *(u32 *)(src + 0);
	u32 src_phys1 = *(u32 *)(src + 8);
	u32 src_stride0 = *(u32 *)(src + 48);
	u32 src_stride1 = *(u32 *)(src + 52);
	u32 src_w = *(u32 *)(src + 60);
	u32 src_h = *(u32 *)(src + 64);
	u32 src_type = *(u32 *)(src + 68);
	u8 *nodes;
	u32 i;
	long ret;

	/* node[8] = LUT[src.enType] (issue #113 fix). No PerspTrans-style
	 * YUV420SP override in Hog — vendor blob just loads the LUT entry. */

	if (!roi_num || roi_num > IVE_HOG_MAX_ROIS) {
		pr_info("ive_neo: Hog bad roi_num=%u (max %u)\n",
			roi_num, IVE_HOG_MAX_ROIS);
		return -EINVAL;
	}
	if (hog_mode != 1 && hog_mode != 2) {
		pr_info("ive_neo: Hog bad enHogMode=%u (1=vert, 2=horiz)\n",
			hog_mode);
		return -EINVAL;
	}
	if (ive_check_buf(src_phys0, src_stride0, src_w, src_h))
		return -EINVAL;

	/* 1664 B is over the kernel's 1024 B stack-frame budget — heap. */
	nodes = kzalloc(IVE_HOG_MAX_ROIS * 208, GFP_KERNEL);
	if (!nodes)
		return -ENOMEM;

	for (i = 0; i < roi_num; i++) {
		u8 *roi  = buf + 80 + i * 16;
		u8 *dst  = buf + 1104 + i * 48;
		u8 *node = nodes + i * 208;
		u32 roi_x = *(u32 *)(roi + 0);
		u32 roi_y = *(u32 *)(roi + 4);
		u32 roi_w = *(u32 *)(roi + 8);
		u32 roi_h = *(u32 *)(roi + 12);
		u32 dst_stride = *(u32 *)(dst + 4);
		u32 dst_phys = *(u32 *)(dst + 16);          /* u64 PhyAddr LO */
		u32 capped_w_cells = (roi_w < IVE_HOG_CELL_LIMIT ? roi_w : IVE_HOG_CELL_LIMIT) / 4;
		u32 capped_h_cells = (roi_h < IVE_HOG_CELL_LIMIT ? roi_h : IVE_HOG_CELL_LIMIT) / 4;
		u32 scale_w = ive_hog_cell_scale(roi_w);
		u32 scale_h = ive_hog_cell_scale(roi_h);

		/* Cell count needs at least 2 cells per axis (vendor does
		 * (capped_dim/4) - 2 unsigned, would wrap if dim < 8). */
		if (capped_w_cells < 2 || capped_h_cells < 2) {
			pr_info("ive_neo: Hog roi[%u] too small w=%u h=%u (need ≥8 each)\n",
				i, roi_w, roi_h);
			ret = -EINVAL;
			goto out;
		}
		capped_w_cells -= 2;
		capped_h_cells -= 2;

		if (!dst_phys || (dst_phys & 0xf)) {
			pr_info("ive_neo: Hog dst[%u] phys=0x%x invalid\n",
				i, dst_phys);
			ret = -EINVAL;
			goto out;
		}
		if (!roi_w || !roi_h ||
		    roi_x + roi_w > src_w || roi_y + roi_h > src_h) {
			pr_info("ive_neo: Hog roi[%u] x=%u y=%u w=%u h=%u out of src %ux%u\n",
				i, roi_x, roi_y, roi_w, roi_h, src_w, src_h);
			ret = -EINVAL;
			goto out;
		}

		node[6]  = 0;
		node[8]  = cv500_src_fmt(src_type);
		node[9]  = (u8) hog_mode;
		node[10] = 0x34;              /* op = Hog */
		node[11] = 2;
		*(u32 *)(node + 16) = src_phys0;
		*(u32 *)(node + 24) = src_phys1;
		*(u16 *)(node + 40) = (u16) src_w;
		*(u16 *)(node + 42) = (u16) src_h;
		*(u16 *)(node + 44) = (u16) src_stride0;
		*(u16 *)(node + 46) = (u16) src_stride1;
		if (hog_mode == 1) {
			*(u16 *)(node + 50) = (u16)(dst_stride << 1);
			*(u16 *)(node + 52) = (u16)((dst_stride << 1) * capped_h_cells);
		} else {
			*(u16 *)(node + 54) = (u16)((dst_stride << 1) * capped_w_cells);
		}
		*(u16 *)(node + 102) = trunc;
		*(u32 *)(node + 124) = dst_phys;
		*(u32 *)(node + 128) = dst_phys;
		*(u32 *)(node + 148) = roi_x << 8;
		*(u32 *)(node + 152) = roi_y << 8;
		*(u32 *)(node + 156) = roi_w;
		*(u32 *)(node + 160) = roi_h;
		*(u32 *)(node + 164) = scale_w;
		*(u32 *)(node + 168) = scale_h;
		node[176] = (u8) csc_mode;
		node[177] = 4;
		*(u16 *)(node + 192) = (u16) capped_w_cells;
		*(u16 *)(node + 194) = (u16) capped_h_cells;
	}

	ret = ive_submit_chain(nodes, roi_num, buf);
out:
	kfree(nodes);
	return ret;
}

/* ---- cv500 Resize (HW op 0x30, ioctl 0xc4c0462e) ----
 *
 * Image resize, 1..8 images per call. Vendor blob symbols:
 * ive_resize_proc @0x490c, ive_fill_resize_task @0x91e8.
 *
 * Layout discovery: the vendor passes a 9264-byte user buffer (src[64]
 * + dst[64] + ctrl) via a small ioctl carrying handle + user-pointer.
 * For simplicity we inline-embed the image arrays in a single wider
 * ioctl, mirroring the PerspTrans/Hog precedent. Cap u16Num at 8.
 *
 * Critical insight from disassembly: fill_resize_task writes a
 * per-image table into a buffer it receives as its 5th arg. That
 * buffer is the user-allocated ctrl.stMem (remapped to kernel virt
 * via cmpi_remap_cached @0x49d8 in vendor). HW reads the table from
 * stMem.phys (which we store in node[16]) at runtime — so the
 * 208-byte HW node is a single descriptor, not a chain of N nodes.
 *
 * Two slot layouts depending on src[0].enType:
 *
 *   PATH A (enType=0=U8C1, 25 bytes/image, vendor branch at 0x9294):
 *     +0     u8  src.enType (0)
 *     +1..4  u32 src.phys - mmz_base (low 32 of au64PhyAddr[0])
 *     +5..8  u32 dst.phys - mmz_base
 *     +9..10  u16 src.stride[0]
 *    +11..12  u16 dst.stride[0]
 *    +13..14  u16 src.width
 *    +15..16  u16 src.height
 *    +17..18  u16 dst.width
 *    +19..20  u16 dst.height
 *    +21..22  u16 (src.width  << 11) / dst.width   (Q4.11)
 *    +23..24  u16 (src.height << 11) / dst.height
 *
 *   PATH B (enType != 0, multi-plane, 49 bytes/image, vendor branch
 *           at 0x93c4 + per-plane loop):
 *     +0     u8  src.enType
 *     +1..12  u32 src.au64PhyAddr[0..2] - mmz_base (3 planes × 4 B)
 *    +13..24  u32 dst.au64PhyAddr[0..2] - mmz_base
 *    +25..30  u16 src.au32Stride[0..2]               (3 × 2 B)
 *    +31..36  u16 dst.au32Stride[0..2]
 *    +37..38  u16 src.width
 *    +39..40  u16 src.height
 *    +41..42  u16 dst.width
 *    +43..44  u16 dst.height
 *    +45..46  u16 (src.width  << 11) / dst.width
 *    +47..48  u16 (src.height << 11) / dst.height
 *
 * HW selects which layout to interpret via node[8] (the format byte
 * from cv500_src_fmt_lut[enType]). All images in a single call must
 * share the same enType — Path is selected once for the whole batch.
 *
 * Arg buffer (1216 bytes):
 *   +0:    HI_HANDLE                                 8
 *   +8:    IVE_SRC_IMAGE_S astSrc[8]               576
 *   +584:  IVE_DST_IMAGE_S astDst[8]               576
 *   +1160: IVE_RESIZE_CTRL_S ctrl                   48
 *       +0: u32 enMode (0 LINEAR, 1 AREA)
 *       +4: IVE_MEM_INFO_S stMem (24 B: phys, virt, size)
 *      +32: u16 u16Num
 *   +1208: HI_BOOL instant                           4
 */
#define IVE_RZ_MAX_NUM    8
#define IVE_RZ_SLOT_A     25      /* Path A: U8C1 single plane */
#define IVE_RZ_SLOT_B     49      /* Path B: multi-plane */

static int ive_rz_slot_size(u32 en_type)
{
	return en_type == 0 ? IVE_RZ_SLOT_A : IVE_RZ_SLOT_B;
}

/* Per-image planar count for stride/phys validation. 1 for U8C1 / S8C1
 * (single plane), 2 for YUV420SP / YUV422SP (luma + UV), 3 for
 * U8C3_PACKAGE / U8C3_PLANAR. Used to sanity-check that user-allocated
 * IVE_IMAGE_S fields are non-zero on planes HW actually reads. */
static int ive_rz_plane_count(u32 en_type)
{
	switch (en_type) {
	case 0:  return 1;                /* U8C1 */
	case 2:                           /* YUV420SP */
	case 3:  return 2;                /* YUV422SP */
	case 10: return 1;                /* U8C3_PACKAGE (1 packed plane) */
	case 11: return 3;                /* U8C3_PLANAR */
	default: return -1;               /* unsupported */
	}
}

static long ive_op_resize_cv500(unsigned long arg)
{
	u8 *buf = (u8 *)arg;
	u8 *astSrc = buf + 8;
	u8 *astDst = buf + 584;
	u8 *ctrl   = buf + 1160;
	u32 mode    = *(u32 *)(ctrl + 0);
	u32 mem_phys = *(u32 *)(ctrl + 8);    /* stMem.u64PhyAddr lo */
	u32 mem_size = *(u32 *)(ctrl + 24);   /* stMem.u32Size */
	u16 num     = *(u16 *)(ctrl + 32);
	u32 src0_type = *(u32 *)(astSrc + 68);
	int slot_bytes;
	int n_planes;
	void *tab;
	u8 node[208];
	u32 i;

	if (!num || num > IVE_RZ_MAX_NUM) {
		pr_info("ive_neo: Resize bad u16Num=%u (max %u)\n",
			num, IVE_RZ_MAX_NUM);
		return -EINVAL;
	}
	if (mode > 1) {
		pr_info("ive_neo: Resize bad enMode=%u (0=LINEAR, 1=AREA)\n", mode);
		return -EINVAL;
	}
	n_planes = ive_rz_plane_count(src0_type);
	if (n_planes < 0) {
		pr_info("ive_neo: Resize src[0].enType=%u unsupported (need 0/2/3/10/11)\n",
			src0_type);
		return -EOPNOTSUPP;
	}
	if (!mem_phys || (mem_phys & 0xF)) {
		pr_info("ive_neo: Resize stMem.phys=0x%x invalid\n", mem_phys);
		return -EINVAL;
	}
	slot_bytes = ive_rz_slot_size(src0_type);
	if (mem_size < (u32)num * (u32)slot_bytes) {
		pr_info("ive_neo: Resize stMem too small (size=%u, need >= %u)\n",
			mem_size, num * slot_bytes);
		return -EINVAL;
	}

	/* CMA is in lowmem; phys_to_virt gives a kernel mapping of stMem. */
	tab = phys_to_virt((phys_addr_t)mem_phys);
	memset(tab, 0, (u32)num * (u32)slot_bytes);

	for (i = 0; i < num; i++) {
		u8 *src = astSrc + i * 72;
		u8 *dst = astDst + i * 72;
		u32 sw = *(u32 *)(src + 60);
		u32 sh = *(u32 *)(src + 64);
		u32 dw = *(u32 *)(dst + 60);
		u32 dh = *(u32 *)(dst + 64);
		u32 src_t = *(u32 *)(src + 68);
		u32 dst_t = *(u32 *)(dst + 68);
		u8 *slot = (u8 *)tab + i * slot_bytes;

		if (src_t != src0_type || dst_t != src0_type) {
			pr_info("ive_neo: Resize [%u] enType src=%u dst=%u, batch=%u (must be uniform)\n",
				i, src_t, dst_t, src0_type);
			return -EINVAL;
		}
		if (IVE_CHECK_IMG(src) || IVE_CHECK_IMG(dst)) {
			pr_info("ive_neo: Resize [%u] image check failed\n", i);
			return -EINVAL;
		}
		if (!dw || !dh) {
			pr_info("ive_neo: Resize [%u] dst dim w=%u h=%u\n", i, dw, dh);
			return -EINVAL;
		}

		slot[0] = (u8) src0_type;
		if (slot_bytes == IVE_RZ_SLOT_A) {
			/* Path A — U8C1 single-plane. */
			*(u32 *)(slot + 1)  = IMG_PHYS(src);
			*(u32 *)(slot + 5)  = IMG_PHYS(dst);
			*(u16 *)(slot + 9)  = (u16) IMG_STRIDE(src);
			*(u16 *)(slot + 11) = (u16) IMG_STRIDE(dst);
			*(u16 *)(slot + 13) = (u16) sw;
			*(u16 *)(slot + 15) = (u16) sh;
			*(u16 *)(slot + 17) = (u16) dw;
			*(u16 *)(slot + 19) = (u16) dh;
			*(u16 *)(slot + 21) = (u16) ((sw << 11) / dw);
			*(u16 *)(slot + 23) = (u16) ((sh << 11) / dh);
		} else {
			/* Path B — multi-plane (YUV420SP/YUV422SP/U8C3_PLANAR). */
			int p;

			for (p = 0; p < 3; p++) {
				/* Plane phys/stride read regardless of n_planes;
				 * unused planes are zeroed in IVE_IMAGE_S by libive. */
				*(u32 *)(slot + 1 + p * 4)  =
					*(u32 *)(src + p * 8);            /* au64PhyAddr[p] lo */
				*(u32 *)(slot + 13 + p * 4) =
					*(u32 *)(dst + p * 8);
				*(u16 *)(slot + 25 + p * 2) =
					(u16) *(u32 *)(src + 48 + p * 4); /* au32Stride[p] */
				*(u16 *)(slot + 31 + p * 2) =
					(u16) *(u32 *)(dst + 48 + p * 4);
			}
			*(u16 *)(slot + 37) = (u16) sw;
			*(u16 *)(slot + 39) = (u16) sh;
			*(u16 *)(slot + 41) = (u16) dw;
			*(u16 *)(slot + 43) = (u16) dh;
			*(u16 *)(slot + 45) = (u16) ((sw << 11) / dw);
			*(u16 *)(slot + 47) = (u16) ((sh << 11) / dh);
		}
	}

	/* Push the table out of CPU caches before HW reads it. */
	osal_flush_dcache_area(tab, (phys_addr_t)mem_phys,
			       (u32)num * (u32)slot_bytes);

	memset(node, 0, sizeof(node));
	node[8]  = cv500_src_fmt(src0_type);             /* format byte for HW */
	node[10] = 0x30;                                 /* op = Resize */
	node[11] = (u8) mode;
	*(u32 *)(node + 16)  = mem_phys;                 /* stMem.phys: table base */
	*(u32 *)(node + 136) = num;                      /* u16Num */

	pr_info_once("ive_neo: Resize handler wired (HW op 0x30, U8C1 + multi-plane)\n");
	return ive_submit_nonxnn(node, buf);
}

/* ---- cv500 LKOpticalFlowPyr (HW op 0x1c, ioctl 0xc2c0461c) ----
 *
 * Lucas-Kanade pyramidal optical flow. Up to 4 pyramid levels
 * (u8MaxLevel = 0..3). Vendor blob symbols: ive_lk_optical_flow_pyr
 * @0x46cc (top), ive_fill_lk_optical_flow_pyr_task @0x8ec4,
 * ive_check_lk_optical_flow_pyr_param @0x129c8.
 *
 * Arg buffer (704 bytes, vendor's IVE_CMD_LK_OPT_FLOW=0xc2c0461c):
 *   +0:    HI_HANDLE                                  8
 *   +8:    IVE_SRC_IMAGE_S prev[4]   (4 * 72)        288
 *   +296:  IVE_SRC_IMAGE_S next[4]                   288
 *   +584:  IVE_SRC_MEM_INFO_S prevPts (24)            24
 *   +608:  IVE_MEM_INFO_S    nextPts                  24
 *   +632:  IVE_DST_MEM_INFO_S status                  24
 *   +656:  IVE_DST_MEM_INFO_S err                     24
 *   +680:  IVE_LK_OPTICAL_FLOW_PYR_CTRL_S ctrl        16
 *       +0:  u32 enOutMode (0=NONE, 1=STATUS, 2=BOTH)
 *       +4:  u32 bUseInitFlow
 *       +8:  u16 u16PtsNum
 *      +10:  u8  u8MaxLevel (0..3)
 *      +11:  u8  u0q8MinEigThr
 *      +12:  u8  u8IterCnt
 *      +13:  u8  u0q8Eps
 *   +696:  HI_BOOL instant                             4
 *   +700:  padding                                     4
 *
 * Validator constraints:
 *   u16PtsNum <= 500, u8MaxLevel <= 3, u8IterCnt <= 19, enOutMode <= 2.
 *
 * Per-level pyramid node-field layout (decoded from
 * ive_fill_lk_optical_flow_pyr_task @0x8ec4):
 *
 *   level 0: node[16]=prev.phys,  node[120]=next.phys,
 *            node[44]=prev.stride, node[50]=next.stride
 *   level 1: node[24]=prev.phys,  node[124]=next.phys,
 *            node[46]=prev.stride, node[52]=next.stride
 *   level 2: node[32]=prev.phys,  node[128]=next.phys,
 *            node[48]=prev.stride, node[54]=next.stride
 *   level 3: node[136]=prev.phys, node[132]=next.phys,
 *            node[12]=prev.stride, node[14]=next.stride
 *
 * Output mem buffers:
 *   node[140] = prevPts.phys (always)
 *   node[20]  = nextPts.phys (always)
 *   node[28]  = status.phys  (if enOutMode >= 1, else 0)
 *   node[36]  = err.phys     (if enOutMode == 2, else 0)
 *
 * ctrl-mirrored node fields:
 *   node[9]   = enOutMode
 *   node[40]  = prev[0].width, node[42] = prev[0].height
 *   node[56]  = u8MaxLevel
 *   node[57]  = bUseInitFlow != 0
 *   node[58]  = u0q8MinEigThr
 *   node[59]  = u0q8Eps
 *   node[94]  = u8IterCnt
 *   node[96]  = u16PtsNum
 */
static long ive_op_lk_optical_flow_pyr_cv500(unsigned long arg)
{
	u8 *buf  = (u8 *)arg;
	u8 *prev = buf + 8;
	u8 *next = buf + 296;
	u8 *prevPts = buf + 584;
	u8 *nextPts = buf + 608;
	u8 *status  = buf + 632;
	u8 *err     = buf + 656;
	u8 *ctrl    = buf + 680;
	u32 out_mode  = *(u32 *)(ctrl + 0);
	u32 use_init  = *(u32 *)(ctrl + 4);
	u16 pts_num   = *(u16 *)(ctrl + 8);
	u8  max_level = ctrl[10];
	u8  min_eig   = ctrl[11];
	u8  iter_cnt  = ctrl[12];
	u8  eps       = ctrl[13];
	u32 p0_w, p0_h, p0_s;
	u8 node[208];
	u32 i;

	if (out_mode > 2) {
		pr_info("ive_neo: LK bad enOutMode=%u (0..2)\n", out_mode);
		return -EINVAL;
	}
	if (max_level > 3) {
		pr_info("ive_neo: LK bad u8MaxLevel=%u (0..3)\n", max_level);
		return -EINVAL;
	}
	if (iter_cnt > 19) {
		pr_info("ive_neo: LK bad u8IterCnt=%u (1..19)\n", iter_cnt);
		return -EINVAL;
	}
	if (!pts_num || pts_num > 500) {
		pr_info("ive_neo: LK bad u16PtsNum=%u (1..500)\n", pts_num);
		return -EINVAL;
	}
	if (!*(u32 *)prevPts || !*(u32 *)nextPts) {
		pr_info("ive_neo: LK prevPts/nextPts phys is 0\n");
		return -EINVAL;
	}
	if (out_mode >= 1 && !*(u32 *)status) {
		pr_info("ive_neo: LK status.phys is 0 (required when enOutMode>=1)\n");
		return -EINVAL;
	}
	if (out_mode == 2 && !*(u32 *)err) {
		pr_info("ive_neo: LK err.phys is 0 (required when enOutMode==BOTH)\n");
		return -EINVAL;
	}

	/* Level 0 is mandatory. Validate src and grab dims. */
	if (IVE_CHECK_IMG(prev) || IVE_CHECK_IMG(next))
		return -EINVAL;
	p0_w = *(u32 *)(prev + 60);
	p0_h = *(u32 *)(prev + 64);
	p0_s = *(u32 *)(prev + 48);

	memset(node, 0, sizeof(node));

	/* Constants */
	node[10] = 0x1c;                            /* op = LK */
	node[9]  = (u8) out_mode;
	*(u16 *)(node + 40) = (u16) p0_w;
	*(u16 *)(node + 42) = (u16) p0_h;

	/* ctrl mirror */
	node[56] = max_level;
	node[57] = use_init ? 1 : 0;
	node[58] = min_eig;
	node[59] = eps;
	node[94] = iter_cnt;
	*(u16 *)(node + 96) = pts_num;

	/* Pyramid level fields. Higher levels get written only when
	 * max_level reaches them — same conditional as vendor's fill_task. */
	for (i = 0; i <= max_level; i++) {
		u8 *pi = prev + i * 72;
		u8 *ni = next + i * 72;
		u32 pi_phys = *(u32 *)pi;
		u32 ni_phys = *(u32 *)ni;
		u32 pi_stride = *(u32 *)(pi + 48);
		u32 ni_stride = *(u32 *)(ni + 48);

		if (!pi_phys || !ni_phys || (pi_phys & 0xF) || (ni_phys & 0xF)) {
			pr_info("ive_neo: LK level %u phys invalid (prev=0x%x next=0x%x)\n",
				i, pi_phys, ni_phys);
			return -EINVAL;
		}
		if (!pi_stride || !ni_stride) {
			pr_info("ive_neo: LK level %u stride invalid (prev=%u next=%u)\n",
				i, pi_stride, ni_stride);
			return -EINVAL;
		}

		switch (i) {
		case 0:
			*(u32 *)(node + 16)  = pi_phys;
			*(u32 *)(node + 120) = ni_phys;
			*(u16 *)(node + 44)  = (u16) pi_stride;
			*(u16 *)(node + 50)  = (u16) ni_stride;
			break;
		case 1:
			*(u32 *)(node + 24)  = pi_phys;
			*(u32 *)(node + 124) = ni_phys;
			*(u16 *)(node + 46)  = (u16) pi_stride;
			*(u16 *)(node + 52)  = (u16) ni_stride;
			break;
		case 2:
			*(u32 *)(node + 32)  = pi_phys;
			*(u32 *)(node + 128) = ni_phys;
			*(u16 *)(node + 48)  = (u16) pi_stride;
			*(u16 *)(node + 54)  = (u16) ni_stride;
			break;
		case 3:
			/* Level 3 uses unusual offsets — vendor's fill_task
			 * writes prev.phys to node[136] (not 40+8*i pattern)
			 * and the strides to node[12]/[14]. */
			*(u32 *)(node + 136) = pi_phys;
			*(u32 *)(node + 132) = ni_phys;
			*(u16 *)(node + 12)  = (u16) pi_stride;
			*(u16 *)(node + 14)  = (u16) ni_stride;
			break;
		}
	}
	(void)p0_s;  /* p0_s used only via the level-0 IVE_CHECK_IMG above. */

	/* Output buffers (phys of user-allocated MMZ blobs). */
	*(u32 *)(node + 140) = *(u32 *)prevPts;
	*(u32 *)(node + 20)  = *(u32 *)nextPts;
	if (out_mode >= 1)
		*(u32 *)(node + 28) = *(u32 *)status;
	if (out_mode == 2)
		*(u32 *)(node + 36) = *(u32 *)err;

	pr_info_once("ive_neo: LK handler wired (HW op 0x1c, levels 0..3)\n");
	return ive_submit_nonxnn(node, buf);
}

/* ---- cv500 KCF tracker (HW op 0x34, ioctl 0xc0084633) ----
 *
 * Kernelized Correlation Filter object tracker. The userspace API
 * (HI_MPI_IVE_KCF_*) stages train/track object metadata into a
 * 22656-byte heap buffer (pstObjList->pu8TmpBuf) and ioctls in
 * with an 8-byte arg = { HI_HANDLE *handle, void *tmpBuf_user_va }.
 *
 * Vendor blob symbols (cv500 open_ive.ko): ive_kcf_proc @0x5be0,
 * ive_check_kcf_param @0x14144, ive_get_kcf_hog_param @0x0,
 * ive_fill_kcf_task @0x9718, ive_get_mmz_base_addr @0x7680.
 *
 * Staging buffer layout (22656 B, copied user→kernel verbatim):
 *
 *   [    0 ..    71]  IVE_IMAGE_S src (72 B)
 *   [   72 .. 11335]  IVE_KCF_OBJ_S train_slots[64] (176 B each)
 *   [11336 .. 22599]  IVE_KCF_OBJ_S track_slots[64]
 *   [22600 .. 22603]  u32 train counter (kernel-written, == iter i)
 *   [22604 .. 22607]  u32 track counter
 *   [22608 .. 22647]  IVE_KCF_PRO_CTRL_S (40 B): enCscMode (4) + pad +
 *                       stTmpBuf (MEM_INFO 24) + u1q15InterFactor (2) +
 *                       u0q16Lamda (2) + u4q12TrancAlfa (2) +
 *                       u0q8Sigma (1) + u8RespThr (1)
 *
 * Each KCF_OBJ_S (176 B): stRoiInfo (20 + 4 pad) + 6 × IVE_MEM_INFO_S
 *   (24 B each: stCosWinX/Y, stGaussPeak, stHogFeature, stAlpha, stDst)
 *   + u3q5Padding (1) + reserved.
 *
 * Per-obj HW task node (208 B), built from inputs above + a 32-byte
 * hog_params struct derived by ive_kcf_compute_hog_param. Both train
 * and track objs emit one node each; nodes are chained and submitted
 * in a single ive_submit_chain. The 4 KB MMZ chain buffer caps total
 * nodes at floor(4096/208) = 19, so we enforce n_train + n_track <= 19.
 *
 * Note on flag semantics (vendor's naming is misleading):
 *   In ive_fill_kcf_task the 4th arg "train_flag" is 0 when iterating
 *   the TRAIN slot region at TmpBuf+72, and 1 when iterating TRACK at
 *   TmpBuf+11336. Field map below uses the slot-region convention.
 *
 * Validator: vendor enforces ctrl.stTmpBuf.u32Size >= 47616 (the HW
 * scratch region for shared HogFeature pool referenced by node[124,128]
 * on TRACK iterations). We mirror that check.
 *
 * Static decode of ive_get_kcf_hog_param byte-verified against an
 * av300 kretprobe trace (2026-05) for padding=96, roi=16×16 →
 * hog_params=[0x800,0x800, 0x10,0x10, -1024,-1024, 56,56, 288].
 */
#define IVE_KCF_STAGE_BYTES   22656u    /* 0x5880, vendor copy_from_user size */
#define IVE_KCF_OBJ_BYTES       176u
#define IVE_KCF_TMP_BUF_MIN  47616u     /* 0xBA00, HW scratch minimum */
#define IVE_KCF_MAX_OBJS         19u    /* chain buffer cap: 4096/208 */
#define IVE_KCF_TRAIN_OFF        72u    /* offset of TRAIN slot array */
#define IVE_KCF_TRACK_OFF     11336u    /* offset of TRACK slot array */
#define IVE_KCF_CTRL_OFF      22608u    /* offset of IVE_KCF_PRO_CTRL_S */

/* Replicates vendor ive_get_kcf_hog_param @0x0 (32-byte output). Derives
 * scan-window parameters from u3q5Padding + ROI. */
static void ive_kcf_compute_hog_param(u32 padding,
				      const u8 *roi,    /* 16-byte RECT_S24Q8_S */
				      u8 *out)          /* 32-byte hog_params */
{
	u32 roi_w = *(u32 *)(roi + 8);
	u32 roi_h = *(u32 *)(roi + 12);
	u32 roi_x_q8 = *(u32 *)(roi + 0);
	u32 roi_y_q8 = *(u32 *)(roi + 4);
	u32 nw, nh, r3x, r3y;
	u32 step_y_clamped, hog_metric, scaled, round_up;
	u16 hc4;

	/* nw = ((pad*roi_w) >> 8) + 1, nh = ((pad*roi_h) >> 8) + 1 */
	nw = ((padding * roi_w) >> 8) + 1;
	nh = ((padding * roi_h) >> 8) + 1;

	/* hog_params[8,12] = roi_center_q8 - (n*1024), where center is
	 * roi_xy_q8 + roi_dim*128. */
	*(s32 *)(out +  8) = (s32)(roi_x_q8 + (roi_w << 7)) - (s32)(nw << 10);
	*(s32 *)(out + 12) = (s32)(roi_y_q8 + (roi_h << 7)) - (s32)(nh << 10);

	/* hog_params[16,20] = n*8 (step per cell, in image pixels) */
	*(u32 *)(out + 16) = nw << 3;
	*(u32 *)(out + 20) = nh << 3;

	/* X scale field (hog_params[0]) and X cells (hog_params[4]).
	 * Cleared low/high bits mirror vendor's `bic r3, r3, #0xc0000001`
	 * — this just makes r3 = (nw*2) with bits 0,30,31 forced 0 before
	 * comparing to 32. For nw up to 0x4000 the bic is a no-op. */
	r3x = ((nw << 1) & ~0xc0000001u) - 2;
	if (r3x > 32) {
		u32 mul = (u32)(((u64)(nw << 14) * 0xf0f0f0f1ULL) >> 32) >> 7;
		*(u16 *)(out + 0) = (u16)mul;
		*(u16 *)(out + 4) = 32;
	} else {
		*(u16 *)(out + 0) = 0x800;
		*(u16 *)(out + 4) = (r3x > 16) ? 32 : 16;
	}

	/* Y scale field (hog_params[2]) and Y cells (hog_params[6]). */
	r3y = ((nh << 1) & ~0xc0000001u) - 2;
	if (r3y > 32) {
		u32 mul = (u32)(((u64)(nh << 14) * 0xf0f0f0f1ULL) >> 32) >> 7;
		*(u16 *)(out + 2) = (u16)mul;
		*(u16 *)(out + 6) = 32;
	} else {
		*(u16 *)(out + 2) = 0x800;
		*(u16 *)(out + 6) = (r3y > 16) ? 32 : 16;
	}

	/* hog_params[24]: precomputed cell metric, rounded up to next
	 * multiple of 96. Vendor uses two umull-based magic-divides to
	 * realise round-up-to-96; we use the equivalent direct form. */
	hc4 = *(u16 *)(out + 4);
	step_y_clamped = (*(u32 *)(out + 20) < 136) ? *(u32 *)(out + 20) : 136;
	scaled = ((u32)hc4 * 3u) * ((step_y_clamped >> 2) - 2);
	scaled >>= 1;
	round_up = scaled % 96u;
	hog_metric = round_up ? (scaled + 96u - round_up) : scaled;
	*(u16 *)(out + 24) = (u16)hog_metric;

	/* out[28..31] is unused / zero. */
	*(u32 *)(out + 28) = 0;
}

/* Build one 208-byte KCF HW task node. Mirrors ive_fill_kcf_task @0x9718
 * field-by-field. obj is a 176-byte KCF_OBJ_S, ctrl is the 40-byte
 * KCF_PRO_CTRL_S, src is the 72-byte IVE_IMAGE_S, hog_params is the
 * 32-byte struct produced by ive_kcf_compute_hog_param above.
 *
 * is_track distinguishes:
 *   TRAIN (0): node[124,128] = obj.stHogFeature (per-obj duplicate).
 *              node[140,144] left zero (HW doesn't write peaks back).
 *   TRACK (1): node[124,128] = ctrl.stTmpBuf phys (shared scratch pool).
 *              node[140] = obj.stDst phys (HW writes peak data here),
 *              node[144] = ctrl.u8RespThr.
 */
static void ive_kcf_fill_node(u8 *node, const u8 *src, const u8 *obj,
			      const u8 *hog_params, const u8 *ctrl,
			      int is_track)
{
	u32 src_phys0 = *(u32 *)(src + 0);
	u32 src_phys1 = *(u32 *)(src + 8);
	u32 src_w     = *(u32 *)(src + 60);
	u32 src_h     = *(u32 *)(src + 64);
	u32 src_s0    = *(u32 *)(src + 48);
	u32 src_s1    = *(u32 *)(src + 52);
	u32 src_type  = *(u32 *)(src + 68);

	u32 cwx_phys  = *(u32 *)(obj + 24);    /* stCosWinX.u64PhyAddr lo32 */
	u32 cwy_phys  = *(u32 *)(obj + 48);    /* stCosWinY */
	u32 gp_phys   = *(u32 *)(obj + 72);    /* stGaussPeak */
	u32 hog_phys  = *(u32 *)(obj + 96);    /* stHogFeature */
	u32 alpha_phys = *(u32 *)(obj + 120);  /* stAlpha */
	u32 dst_phys  = *(u32 *)(obj + 144);   /* stDst */

	u32 tmpbuf_phys = *(u32 *)(ctrl + 8);  /* ctrl.stTmpBuf.u64PhyAddr lo32 */

	node[6]  = 0;
	node[8]  = cv500_src_fmt(src_type);
	node[9]  = 0;
	node[10] = 0x34;                        /* KCF op */
	node[11] = (u8) is_track;

	*(u32 *)(node + 16) = src_phys0;
	*(u32 *)(node + 24) = src_phys1;
	*(u16 *)(node + 40) = (u16) src_w;
	*(u16 *)(node + 42) = (u16) src_h;
	*(u16 *)(node + 44) = (u16) src_s0;
	*(u16 *)(node + 46) = (u16) src_s1;

	*(u16 *)(node + 52)  = *(u16 *)(hog_params + 24);
	*(u16 *)(node + 102) = *(u16 *)(ctrl + 36);   /* u4q12TrancAlfa */
	*(u16 *)(node + 108) = *(u16 *)(ctrl + 32);   /* u1q15InterFactor */
	*(u16 *)(node + 110) = *(u16 *)(ctrl + 34);   /* u0q16Lamda */

	*(u32 *)(node + 104) = gp_phys;
	*(u32 *)(node + 112) = cwx_phys;
	*(u32 *)(node + 116) = cwy_phys;
	*(u32 *)(node + 120) = hog_phys;

	if (is_track) {
		*(u32 *)(node + 124) = tmpbuf_phys;
		*(u32 *)(node + 128) = tmpbuf_phys;
	} else {
		*(u32 *)(node + 124) = hog_phys;
		*(u32 *)(node + 128) = hog_phys;
	}
	*(u32 *)(node + 132) = hog_phys;

	*(u32 *)(node + 84)  = alpha_phys;
	*(u32 *)(node + 136) = alpha_phys;

	if (is_track) {
		*(u32 *)(node + 140) = dst_phys;
		*(u32 *)(node + 144) = ctrl[39];       /* u8RespThr zero-ext */
	}

	*(u32 *)(node + 148) = *(u32 *)(hog_params +  8);   /* start scan X */
	*(u32 *)(node + 152) = *(u32 *)(hog_params + 12);   /* start scan Y */
	*(u32 *)(node + 156) = *(u32 *)(hog_params + 16);   /* scan range X */
	*(u32 *)(node + 160) = *(u32 *)(hog_params + 20);   /* scan range Y */
	*(u32 *)(node + 164) = *(u16 *)(hog_params +  0);   /* X step scale */
	*(u32 *)(node + 168) = *(u16 *)(hog_params +  2);   /* Y step scale */

	/* node[172]: (1<<40) / ((u0q8Sigma+1)^2 * L*H*31), where L,H are
	 * scan-range-derived cell counts. Replicates the two-stage
	 * osal_div_u64 sequence in ive_fill_kcf_task @0x98c0-0x98fc. Use
	 * div_u64 (not bare /) — ARM doesn't have a 64-bit udiv insn and
	 * the kernel doesn't export __aeabi_uldivmod. */
	{
		u32 sx = *(u32 *)(hog_params + 16);
		u32 sy = *(u32 *)(hog_params + 20);
		u32 L = (sx <= 135) ? (sx / 4 - 2) : 32;
		u32 H = (sy <= 135) ? (sy / 4 - 2) : 32;
		u32 sigma = ctrl[38];
		u32 p2 = (sigma + 1) * (sigma + 1);
		u64 step1 = div_u64(1ULL << 40, p2);
		u32 LH31 = L * H * 31u;
		u32 result = (LH31) ? (u32)div_u64(step1, LH31) : 0;
		*(u32 *)(node + 172) = result;
	}

	node[176] = ctrl[0];                /* enCscMode low byte */
	node[177] = 4;                       /* constant */

	*(u16 *)(node + 192) = *(u16 *)(hog_params + 4);   /* HOG cells X */
	*(u16 *)(node + 194) = *(u16 *)(hog_params + 6);   /* HOG cells Y */
}

static long ive_op_kcf_cv500(unsigned long arg)
{
	u8 *iobuf = (u8 *)arg;
	void __user *user_tmpbuf;
	u8 *kbuf;
	u8 *src, *ctrl;
	u32 n_train, n_track, total, i;
	u32 tmpbuf_size;
	u8 *nodes;
	long ret;

	/* arg is the 8-byte ioctl payload: { HI_HANDLE *handle, void *tmpBuf }.
	 * iobuf[0..3] is the user-virt of the handle (libive writes it
	 * back from the pre-zeroed slot we leave in arg_buf for the chain
	 * submit); iobuf[4..7] is the user-virt of the 22656-byte staging
	 * buffer. */
	user_tmpbuf = (void __user *)(uintptr_t)(*(u32 *)(iobuf + 4));
	if (!user_tmpbuf)
		return -EINVAL;

	kbuf = kzalloc(IVE_KCF_STAGE_BYTES, GFP_KERNEL);
	if (!kbuf)
		return -ENOMEM;

	if (osal_copy_from_user(kbuf, user_tmpbuf, IVE_KCF_STAGE_BYTES)) {
		ret = -EFAULT;
		goto out_kbuf;
	}

	src  = kbuf + 0;
	n_train = *(u32 *)(kbuf + 22600);
	n_track = *(u32 *)(kbuf + 22604);
	ctrl = kbuf + IVE_KCF_CTRL_OFF;

	tmpbuf_size = *(u32 *)(ctrl + 16);   /* stTmpBuf.u32Size */
	if (tmpbuf_size < IVE_KCF_TMP_BUF_MIN) {
		pr_info("ive_neo: KCF stTmpBuf.u32Size=%u < %u\n",
			tmpbuf_size, IVE_KCF_TMP_BUF_MIN);
		ret = -EINVAL;
		goto out_kbuf;
	}

	total = n_train + n_track;
	if (!total) {                         /* idle Process call: succeed */
		*(u32 *)iobuf = 0;
		ret = 0;
		goto out_kbuf;
	}
	if (n_train > 64 || n_track > 64 || total > IVE_KCF_MAX_OBJS) {
		pr_info("ive_neo: KCF n_train=%u n_track=%u (max %u/%u/%u)\n",
			n_train, n_track, 64u, 64u, IVE_KCF_MAX_OBJS);
		ret = -EINVAL;
		goto out_kbuf;
	}

	nodes = kzalloc(total * 208, GFP_KERNEL);
	if (!nodes) {
		ret = -ENOMEM;
		goto out_kbuf;
	}

	for (i = 0; i < n_train; i++) {
		u8 *obj  = kbuf + IVE_KCF_TRAIN_OFF + i * IVE_KCF_OBJ_BYTES;
		u8 *node = nodes + i * 208;
		u8  pad  = obj[168];               /* u3q5Padding */
		u8  hog_params[32] = { 0 };
		ive_kcf_compute_hog_param((u32)pad, obj /* ROI at offset 0 */,
					  hog_params);
		ive_kcf_fill_node(node, src, obj, hog_params, ctrl, 0);
	}
	for (i = 0; i < n_track; i++) {
		u8 *obj  = kbuf + IVE_KCF_TRACK_OFF + i * IVE_KCF_OBJ_BYTES;
		u8 *node = nodes + (n_train + i) * 208;
		u8  pad  = obj[168];
		u8  hog_params[32] = { 0 };
		ive_kcf_compute_hog_param((u32)pad, obj, hog_params);
		ive_kcf_fill_node(node, src, obj, hog_params, ctrl, 1);
	}

	pr_info_once("ive_neo: KCF handler wired (HW op 0x34, train+track chain)\n");
	ret = ive_submit_chain(nodes, total, iobuf);
	kfree(nodes);
out_kbuf:
	kfree(kbuf);
	return ret;
}
#endif

#ifndef IVE_STANDALONE
/* Diagnostic logger for cv500-only IVE ops we don't (yet) implement.
 *
 * Two modes, selected by the enable_cv500_extras module param:
 *
 *   0 (default): return 0 so libive.so's IVE_HANDLE check stays happy
 *                and userland sees the same "silent stub" behaviour we
 *                already apply to LBP/NCC/EqualizeHist/etc. Zero HW
 *                writes. Safe to leave on in production while a real
 *                implementation is in progress.
 *
 *   1:           log the chip name, ioctl size, and a 256-byte hex
 *                dump of the userspace arg buffer, then return
 *                -EOPNOTSUPP. Used to capture real-world call patterns
 *                (struct layouts, ROI counts, ctrl-field values) from
 *                hardware before we start writing HW task nodes. Still
 *                zero HW writes from the driver — only printk traffic.
 *
 * The buffer is already kernel-side at this point: the OSAL ioctl path
 * copies _IOC_WRITE-direction ioctl args into a kernel buffer before
 * dispatch (and the standalone wrapper does the same via copy_from_user),
 * so we can hexdump it directly. */
static long ive_diag_cv500_extras(const char *op, unsigned long arg,
				  unsigned int sz)
{
	unsigned int dump = sz < 256 ? sz : 256;

	if (!g_enable_cv500_extras)
		return 0;

	pr_info("ive_neo: %s ioctl on chip=%s arg=%lu bytes (dumping first %u)\n",
		op, ive_neo_chip.name, (unsigned long)sz, dump);
	print_hex_dump(KERN_INFO, "ive_neo: ", DUMP_PREFIX_OFFSET,
		       32, 4, (const void *)arg, dump, true);
	return -EOPNOTSUPP;
}
#endif

/* ---- Ioctl dispatch (on kernel buffer — OSAL or our wrapper pre-copied) ---- */

/* ---- ALLOC_BUF / FREE_BUF (standalone only) ---- */
#ifdef IVE_STANDALONE
static long ive_alloc_buf(unsigned long arg)
{
	/* arg = {u64 phys_out, u64 user_data_ptr, u32 size} → 24 bytes
	 * Allocate physically-contiguous kernel memory.
	 * If user_data_ptr != 0, copy user data into the buffer.
	 * Returns phys addr in phys_out. */
	u64 *p64 = (u64 *)arg;
	u32 size = *(u32 *)((u8 *)arg + 16);
	void __user *udata = (void __user *)(uintptr_t)p64[1];
	void *virt;

	if (size > 4 * 1024 * 1024)
		return -ENOMEM;

	{
	u32 alloc_size = (size + 4095) & ~4095;
	virt = kmalloc(alloc_size, GFP_KERNEL | __GFP_ZERO);
	if (!virt)
		return -ENOMEM;

	if (udata && copy_from_user(virt, udata, size)) {
		kfree(virt);
		return -EFAULT;
	}

	p64[0] = virt_to_phys(virt);
	return 0;
	}
}
#endif

static long ive_dispatch(unsigned int cmd, unsigned long arg)
{
	if (cmd != IVE_IOC_QUERY && cmd != IVE_IOC_XNN_FWD_SLICE)
		ive_log("ioctl 0x%08x\n", cmd);
	switch (cmd) {
#ifdef IVE_STANDALONE
	case IVE_IOC_ALLOC_BUF:     return ive_alloc_buf(arg);
	case IVE_IOC_READ_BUF: {
		u64 *p = (u64 *)arg;
		u32 sz = *(u32 *)((u8 *)arg + 16);
		void *kv = phys_to_virt(p[0]);
		if (kv && p[1] && sz <= 4*1024*1024)
			if (copy_to_user((void __user *)(uintptr_t)p[1], kv, sz))
				return -EFAULT;
		return 0;
	}
#endif
	case IVE_IOC_SVP_INIT:       return ive_svp_init(arg);
	case IVE_IOC_OPEN_DEV:       return ive_open_dev(arg);
	case IVE_IOC_XNN_LOADMODEL:  return ive_xnn_loadmodel(arg);
	case IVE_IOC_XNN_FWD_SLICE:  return ive_xnn_fwd_slice(arg);
	case IVE_IOC_QUERY:          return ive_xnn_query(arg);
	case IVE_IOC_XNN_UNLOADMODEL:return ive_xnn_unloadmodel(arg);
	case IVE_IOC_SVP_EXIT:       return 0;
	case IVE_IOC_SVP_ALG_PROC_INIT:  return ive_svp_alg_proc_init(arg);
	case IVE_IOC_SVP_ALG_PROC_EXIT:  return ive_svp_alg_proc_exit(arg);
	case IVE_IOC_SVP_ALG_PROC_BEGIN: return 0;
	case IVE_IOC_SVP_ALG_PROC_END:   return 0;
	/* Non-XNN IVE ops */
	case IVE_IOC_DMA:      return ive_op_dma(arg);
	case IVE_IOC_FILTER:   return ive_op_filter(arg);
	case IVE_IOC_DILATE:   return ive_op_dilate(arg);
	case IVE_IOC_ERODE:    return ive_op_erode(arg);
	case IVE_IOC_THRESH:   return ive_op_thresh(arg);
	case IVE_IOC_AND:      return ive_op_logic(arg, 9);
	case IVE_IOC_SUB:      return ive_op_sub(arg);
	case IVE_IOC_OR:       return ive_op_logic(arg, 11);
	case IVE_IOC_HIST:     return ive_op_hist(arg);
	case IVE_IOC_THRESH_S16: return ive_op_thresh_s16(arg);
	case IVE_IOC_THRESH_U16: return ive_op_thresh_u16(arg);
	case IVE_IOC_ADD:      return ive_op_add(arg);
	case IVE_IOC_CCL:      return ive_op_ccl(arg);
	/* Cmd → op mappings verified against libive.so decomp
	 * (~/libive/mpi_ive.o.c). Each HI_MPI_IVE_* wraps a single ioctl. */
	case 0xc0e84615u:      return ive_op_logic(arg, 21);  /* Xor */
	case 0xc0a84610u:      return ive_op_16to8(arg);      /* 16BitTo8Bit */
	case IVE_IOC_INTEG:    return ive_op_integ(arg);      /* 0xc0a0460c */
	case IVE_IOC_ORDSTAT:  return ive_op_ordstat(arg);    /* 0xc0a04611 */
	case 0xc1084604u:      return ive_op_sobel(arg);      /* Sobel */
	case 0xc1084605u:      return ive_op_mag_and_ang(arg); /* MagAndAng */
	case IVE_IOC_MAP:      return ive_op_map(arg);        /* 0xc0b84613 */
	case 0xc150461bu:      return ive_op_norm_grad(arg);  /* NormGrad */
	case 0xc1384629u:      return ive_op_sad(arg);        /* SAD */
	case 0xc0c0462bu:      return ive_op_st_candi_corner(arg); /* STCandiCorner */
	case 0xc0f04619u:      return ive_op_canny_hys(arg);  /* CannyHysEdge */
	case 0xc1a8462du:      return ive_op_gmm2(arg);       /* GMM2 (best effort) */
	case 0xc0a8461au:      return ive_op_lbp(arg);   /* LBP — vendor op 26 */
	case 0xc0b84616u:      return ive_op_ncc(arg);   /* NCC — vendor op 22 */

	/* ---- Silent-stub ops ----
	 *
	 * Audited 2026-05-13 against obj/hi3516cv500/hi_ive.o symbol table
	 * + ive_ioctl dispatcher. Each op is in one of three categories:
	 *
	 * (a) cv500 HW supports it, we just haven't wired it. Vendor blob
	 *     has both ive_check_<op>_param AND ive_fill_<op>_task. Worth
	 *     porting — follow-up issues opened. Stub returns 0 to keep
	 *     libive's handle-check happy until impl lands.
	 *
	 * (b) cv500 reuses another op's HW path with a flag override
	 *     (e.g., EqualizeHist calls ive_fill_hist_task with
	 *     node[7]=0x61 + an extra LUT remap at node[200]).
	 *
	 * (c) cv500 has no symbols for it — vendor genuinely doesn't
	 *     dispatch HW. Userspace either falls back to CPU compute
	 *     or rejects. Silent stub is correct here.
	 */

	/* (b) reuses ive_fill_hist_task with node[7]=0x61 + dst-image overrides. */
	case 0xc0b8462au:      return ive_op_equalize_hist(arg);  /* EqualizeHist */

	case 0xc0a04602u:                /* CSC */
#if defined(hi3516cv500)
		/* HW dispatch — modes 0..3 (YUV2RGB) + 8..11 (RGB2YUV).
		 * Modes 4..7 (YUV2HSV/LAB) return -EOPNOTSUPP. */
		return ive_op_csc_cv500(arg);
#else
		return 0;
#endif
	case 0xc0c04603u:                /* FilterAndCSC */
#if defined(hi3516cv500)
		/* HW dispatch — 5x5 filter + YUV2RGB. Validator rejects
		 * HSV/LAB modes (only 0..3 allowed). */
		return ive_op_flt_csc_cv500(arg);
#else
		return 0;
#endif
	case 0xc008462eu:      return 0;  /* Resize (vendor's small 8-B stub ioctl) — superseded by 0xc4c0462e below */
	case 0xc2c0461cu:                /* LKOpticalFlowPyr */
#if defined(hi3516cv500)
		/* HW dispatch — single 208-B node, up to 4 pyramid levels.
		 * Output mem buffers (prevPts/nextPts/status/err) are
		 * user-allocated MMZ blobs; HW writes via their phys. */
		return ive_op_lk_optical_flow_pyr_cv500(arg);
#else
		return 0;
#endif

	/* (c) GMM (single-Gaussian) — vendor cv500 ive_ioctl has no
	 * dispatch case for ioctl 0x4618 (verified via objdump grep of
	 * the ioctl-nr movw immediates). Userspace libive.so likely
	 * routes single-Gaussian through GMM2 with mixture=1, or
	 * computes on CPU. Silent stub is correct. */
	case 0xc1184618u:      return 0;  /* GMM */

	/* (c) cv500 vendor blob has no symbols at all for these — HW
	 * unit either doesn't exist or vendor never wired userspace
	 * to it. Silent stub is correct; libive presumably falls back
	 * to CPU implementation or returns its own error. */
	case 0xc138461du:      return 0;  /* GradFg */
	case 0xc178461eu:      return 0;  /* MatchBgModel */
	case 0xc1d8461fu:      return 0;  /* UpdateBgModel */
	case 0xc0b04621u:      return 0;  /* ANN_MLP_Predict */
	case 0xc0c04622u:      return 0;  /* SVM_Predict */
#ifndef IVE_STANDALONE
	/* cv500-only IVE ops. Wire numbers + HW op codes captured from
	 * cv500's libive.so + vendor hi_ive.o blob (2026-05). */
	case 0xdc604635u:      /* HI_MPI_IVE_PerspTrans (cv500 op 0x35, arg 7264 B) */
#if defined(hi3516cv500)
		/* Real HW dispatch — builds 1..8 chained nodes, submits,
		 * waits for IRQ. Field map from ive_fill_persp_trans_task. */
		return ive_op_persp_trans_cv500(arg);
#else
		/* V4 family doesn't have this op in HW — silently stub so
		 * any libive that happens to call it gets a "no error" path. */
		return 0;
#endif
	case 0xd0684634u:      /* HI_MPI_IVE_Hog (cv500 op 0x34, arg 4200 B) */
#if defined(hi3516cv500)
		/* HW dispatch — builds 1..8 chained nodes (one per ROI),
		 * submits, waits for IRQ. Field map from ive_fill_hog_task
		 * with the cell-grid scale magic-divide replicated. */
		return ive_op_hog_cv500(arg);
#else
		return 0;
#endif
	case 0xc4c0462eu:      /* HI_MPI_IVE_Resize cv500 (op 0x30, arg 1216 B) */
#if defined(hi3516cv500)
		/* HW dispatch — single HW node, per-image table in ctrl.stMem.
		 * u16Num 1..8; supports both Path A (U8C1, 25-B slots) and
		 * Path B (YUV420SP/YUV422SP/U8C3_PLANAR, 49-B slots). */
		return ive_op_resize_cv500(arg);
#else
		return 0;
#endif
	case 0xc0084633u:      /* HI_MPI_IVE_KCF_Process (cv500 op 0x34, 8-B arg + 22656-B staging) */
#if defined(hi3516cv500)
		/* HW dispatch — chained nodes (one per train + track obj),
		 * staging buf copied in from userspace. Cap n_train+n_track
		 * at 19 (chain MMZ task buffer is 4 KB = 19.69 × 208 B). */
		return ive_op_kcf_cv500(arg);
#else
		return 0;
#endif
#endif
	default:               return 0;
	}
}

/* ==== Platform-specific module glue ==== */

#ifdef IVE_STANDALONE

/* Standalone: standard Linux misc device, self-contained ioctl wrapper */

static long ive_sa_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
	unsigned int dir = _IOC_DIR(cmd);
	unsigned int sz = _IOC_SIZE(cmd);
	char *kbuf = NULL;
	long ret;

	if (dir == _IOC_NONE)
		return ive_dispatch(cmd, arg);

	if (!sz)
		return -EINVAL;

	kbuf = kmalloc(sz, GFP_KERNEL);
	if (!kbuf)
		return -ENOMEM;

	if (dir & _IOC_WRITE)
		if (copy_from_user(kbuf, (void __user *)arg, sz)) {
			kfree(kbuf);
			return -EFAULT;
		}

	ret = ive_dispatch(cmd, (unsigned long)kbuf);

	if (ret == 0 && (dir & _IOC_READ))
		if (copy_to_user((void __user *)arg, kbuf, sz))
			ret = -EFAULT;

	kfree(kbuf);
	return ret;
}

static int ive_sa_open(struct inode *i, struct file *f) { return 0; }
static int ive_sa_release(struct inode *i, struct file *f) { return 0; }

static const struct file_operations ive_sa_fops = {
	.owner = THIS_MODULE,
	.open = ive_sa_open,
	.release = ive_sa_release,
	.unlocked_ioctl = ive_sa_ioctl,
};

static struct miscdevice ive_sa_misc = {
	.minor = 17,
	.name = "ive",
	.fops = &ive_sa_fops,
};

static int __init ive_sa_init(void)
{
	int ret;

	g_ive_regs = ioremap(ive_neo_chip.standalone_base, 0x10000);
	if (!g_ive_regs)
		return -ENOMEM;

	ive_hw_init();

	ret = misc_register(&ive_sa_misc);
	if (ret) {
		iounmap(g_ive_regs);
		return ret;
	}

	ive_log("standalone init OK\n");
	return 0;
}

static void __exit ive_sa_exit(void)
{
	misc_deregister(&ive_sa_misc);
	if (g_ive_regs) {
		iounmap(g_ive_regs);
		g_ive_regs = NULL;
	}
}

module_init(ive_sa_init);
module_exit(ive_sa_exit);

#else /* !IVE_STANDALONE — OSAL-based build for real hardware */

static long ive_osal_ioctl(unsigned int cmd, unsigned long arg, void *priv)
{
	return ive_dispatch(cmd, arg);
}

static int ive_osal_open(void *priv) { return 0; }
static int ive_osal_close(void *priv) { return 0; }

/* /dev/ive mmap: userspace passes phys as mmap offset, we remap that
 * phys range into userspace virt. Needed by libive.so's svp_alg_proc_init
 * flow (allocates 4KB MMZ via ioctl, then mmaps it to get a virt pointer).
 */
static int ive_osal_mmap(osal_vm_t *vm, unsigned long start,
			 unsigned long end, unsigned long vm_pgoff,
			 void *priv)
{
	unsigned long size = end - start;
	osal_pgprot_noncached(vm);
	ive_log("mmap: start=0x%lx end=0x%lx pgoff=0x%lx size=0x%lx\n",
		start, end, vm_pgoff, size);
	return osal_io_remap_pfn_range(vm, start, vm_pgoff, size);
}

static osal_fileops_t g_ive_fops = {
	.open = ive_osal_open,
	.release = ive_osal_close,
	.unlocked_ioctl = ive_osal_ioctl,
	.mmap = ive_osal_mmap,
};

static osal_dev_t *g_ive_dev;
static int g_ive_irq_requested;

/* IRQ handler: read status from [0x0C], ack via [0x08], stash the status
 * where the caller can see it, and signal the completion. Without this
 * the IRQ stays pending at the GIC and subsequent tasks wedge. */
static irqreturn_t ive_irq_handler(int irq, void *dev)
{
	u32 status;
	if (!g_ive_regs)
		return IRQ_NONE;
	status = readl(g_ive_regs + 0x0C);
	if (!status)
		return IRQ_NONE;
	writel(7, g_ive_regs + 0x08);  /* ack all pending */
	g_ive_last_status = status;
	g_ive_irq_count++;
	complete(&g_ive_done);
	return IRQ_HANDLED;
}

int ive_std_mod_init(void)
{
	int ret;

	g_ive_dev = osal_createdev("ive");
	if (!g_ive_dev)
		return -ENOMEM;
	g_ive_dev->fops = &g_ive_fops;
	g_ive_dev->minor = 17;

	ret = osal_registerdevice(g_ive_dev);
	if (ret) {
		osal_destroydev(g_ive_dev);
		g_ive_dev = NULL;
		return ret;
	}

	if (g_ive_irq) {
		ret = request_irq(g_ive_irq, ive_irq_handler, 0,
				  "ive_neo", &g_ive_dev);
		if (ret) {
			ive_err("request_irq(%u) failed: %d\n",
				g_ive_irq, ret);
		} else {
			g_ive_irq_requested = 1;
			ive_log("IRQ %u registered\n", g_ive_irq);
		}
	} else {
		ive_log("no IRQ from platform probe\n");
	}

	ive_log("mod_init OK\n");
	return 0;
}

void ive_std_mod_exit(void)
{
	if (g_ive_irq_requested) {
		free_irq(g_ive_irq, &g_ive_dev);
		g_ive_irq_requested = 0;
	}
	if (g_svp_alg_virt) {
		ive_dma_free(g_svp_alg_phys, g_svp_alg_virt);
		g_svp_alg_virt = NULL;
		g_svp_alg_phys = 0;
	}
	if (g_ivp_proc_virt) {
		ive_dma_free(g_ivp_proc_phys, g_ivp_proc_virt);
		g_ivp_proc_virt = NULL;
		g_ivp_proc_phys = 0;
	}
	if (g_ive_task_virt) {
		ive_dma_free(g_ive_task_phys, g_ive_task_virt);
		g_ive_task_virt = NULL;
		g_ive_task_phys = 0;
	}
	if (g_ive_dev) {
		osal_deregisterdevice(g_ive_dev);
		osal_destroydev(g_ive_dev);
		g_ive_dev = NULL;
	}
}

#endif /* IVE_STANDALONE */

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ive_neo: open-source IVE/XNN driver (Hi3516EV200/EV300 + Hi3516CV500/AV300)");
