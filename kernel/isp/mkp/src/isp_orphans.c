/*
 * Release the frame references a dead process was still holding.
 *
 * A process that takes a frame out of the pipeline -- VPSS_GetChnFrame,
 * VI_GetPipeFrame, VB_GetBlock -- holds one VB_UID_USER reference on its block
 * until it gives it back. If it dies first, nothing gives it back: the vendor
 * modules' release callbacks do nothing, so the reference outlives the
 * process. The next process's HI_MPI_SYS_Exit stops the pipeline and drops
 * every module's references, but that one remains, and HI_MPI_VB_Exit refuses
 * a pool with a busy block (ERR_VB_BUSY). The pools stay allocated and nothing
 * short of a reboot or a module reload clears it.
 *
 * Measured on a hi3516ev300: an OOM-killed majestic left exactly one block
 * (pool 0, block 2) with one USER reference, and the next start failed at
 * VB_Exit. Dropping that reference from a new process (PhysAddr2Handle, then
 * ReleaseBlock) let VB_Exit succeed, MMZ fell to its idle figure, and the
 * stream came back. A plain kill -9 rarely leaves one, because it usually
 * lands between a Get and its Release; a process stalling under memory
 * pressure does not.
 *
 * So when osal reports that the last MPP device file has closed -- no live
 * process can be holding a USER reference at that moment -- every one still
 * counted is dropped. Only references are dropped, never pools: freeing the
 * pools remains the job of the next process's SYS_Exit/VB_Exit, as after a
 * clean exit.
 *
 * Why here and not in base, where VB lives: base is a vendor blob on the Goke
 * images (load_goke inserts gk7205v200_base.ko, not open_base.ko), and isp is
 * the one module built from source here that every V4 image loads after base.
 * VB is reached through the table base registers with CMPI rather than by
 * symbol, because the Goke base does not export the VB_* symbols; the table's
 * layout (vb_ext.h) matches both the hi3516ev200 blob and gk7205v200_base.ko
 * entry for entry.
 *
 * Not vb_force_exit=1: that makes VB_Exit free every busy block, including one
 * a module still holds, rather than only the orphans. Not a per-process record
 * in osal: a USER reference is taken inside the blobs and carries no pid, so
 * the last close is the only moment that is known to be safe.
 */
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>

#include "osal.h"
#include "type.h"
#include "common.h"
#include "mod_ext.h"
#include "vb_ext.h"
#include "isp_orphans.h"

static unsigned int vb_release_orphans = 1;
module_param(vb_release_orphans, uint, 0644);
MODULE_PARM_DESC(
	vb_release_orphans,
	"Release frames a dead process still held once the last MPP file closes (default 1)");

static void isp_release_orphan_frames(void)
{
	VB_EXPORT_FUNC_S *vb;
	GK_U32 pool, blk, refs = 0, blocks = 0;

	if (!vb_release_orphans)
		return;
	vb = FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB);
	if (vb == GK_NULL || vb->pfnVB_InquireBlkCnt == GK_NULL ||
	    vb->pfnVB_InquirePool == GK_NULL ||
	    vb->pfnVB_InquireOneUserCnt == GK_NULL ||
	    vb->pfnVB_Handle2Phys == GK_NULL || vb->pfnVB_UserSub == GK_NULL)
		return;

	if (vb->pfnVB_InquireBlkCnt(VB_UID_USER, GK_TRUE) == 0 &&
	    vb->pfnVB_InquireBlkCnt(VB_UID_USER, GK_FALSE) == 0)
		return;

	for (pool = 0; pool < VB_MAX_POOLS; pool++) {
		VB_POOL_STATUS_S st;

		if (vb->pfnVB_InquirePool(pool, &st) != GK_SUCCESS)
			continue;
		/* The count bounds the walk: a handle past the last block of its
		 * pool is an osal_panic inside the blob, not an error. */
		for (blk = 0; blk < st.u32BlkCnt; blk++) {
			VB_BLKHANDLE h = (pool << 16) | blk;
			GK_U32 n = vb->pfnVB_InquireOneUserCnt(h, VB_UID_USER);
			GK_U64 phys;

			if (n == 0)
				continue;
			phys = vb->pfnVB_Handle2Phys(h);
			blocks++;
			while (n--) {
				if (vb->pfnVB_UserSub(pool, phys,
						      VB_UID_USER) !=
				    GK_SUCCESS)
					break;
				refs++;
			}
		}
	}

	if (blocks)
		pr_warn("isp: last MPP user closed still holding %u frame reference(s) in %u VB block(s): released\n",
			refs, blocks);
}

void ISP_OrphansInit(void)
{
	if (osal_register_last_close(isp_release_orphan_frames) != 0)
		pr_warn("isp: last-close hook already taken; frames a dead process held will not be released\n");
}

void ISP_OrphansExit(void)
{
	osal_register_last_close(GK_NULL);
}
