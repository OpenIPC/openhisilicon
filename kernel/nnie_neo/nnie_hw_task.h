/* SPDX-License-Identifier: GPL-2.0 */
/*
 * cv500 NNIE 64-byte HW task descriptor.
 *
 * Decoded from vendor svp_nnie_fill_forward_task @0x90d8 (hi_nnie.o,
 * 109 KB blob), prologue 90d8..91a8. The vendor function takes 4 args:
 *
 *   r0 = global HW state ptr     (.bss + 8, opaque to userspace)
 *   r1 = SVP_NNIE_MODEL_S *      (kernel-side copy at .LANCHOR0+200)
 *   r2 = forward arg kbuf        (1624 B, layout in nnie_neo.c)
 *   r3 = task_node out ptr       (stack — sp+32 in svp_nnie_forward)
 *
 * The vendor caller `svp_nnie_forward` @0x2150 sets these up at
 * 2208..2218; the task_node is on the calling function's stack so the
 * descriptor lives only for the duration of the dispatch.
 *
 * Decoded field sources (all numeric offsets are within the 64-B node):
 *
 *   +0   u16  trigger_mode      = CLZ(bInstant-1) >> 5  → 1 if bInstant else 0
 *                                 (bInstant from fwd_arg[1616])
 *   +2   u16  status            = 0 written; IRQ likely updates this
 *   +4   u32  reserved          = 0
 *   +8   u64  reserved          = 0
 *   +16  u64  model_file_phys   = pstModel->stBase.u64PhyAddr
 *                                 (the .wk MMZ block — instruction stream +
 *                                 weights + quant tables live here)
 *                                 source: ldrd r2, [#0x3690, r6=pstModel]
 *                                 0x3690 = offsetof(SVP_NNIE_MODEL_S, stBase)
 *   +24  u32  seg_inst_offset   = pstModel->astSeg[NetSegId].u32InstOffset
 *                                 source: r9=pstModel+1692*NetSegId, [r9,#24]
 *                                 The +24 lands inside astSeg[i] at the
 *                                 u32InstOffset field because sizeof(SEG_S)
 *                                 stride = 1692 B (verified: SEG_S layout has
 *                                 InstOffset at +12 within the struct, plus
 *                                 the 12-byte MODEL_S header shifts everything
 *                                 so r9+24 == astSeg[NetSegId].u32InstOffset).
 *   +28  u32  seg_inst_len      = pstModel->astSeg[NetSegId].u32InstLen
 *   +32  u64  tsk_buf_phys      = forward_ctrl->stTskBuf.u64PhyAddr
 *                                 source: ldrd r4, [r8=ctrl, #40]
 *                                 ctrl layout: SrcNum(4) DstNum(4) NetSegId(4)
 *                                              NnieId(4) stTmpBuf(24) stTskBuf(24)
 *                                 ctrl+40 = stTskBuf.u64PhyAddr ✓
 *   +40  u64  reserved          = 0
 *   +48  u64  tmp_buf_phys      = forward_ctrl->stTmpBuf.u64PhyAddr
 *                                 source: ldrd r4, [r8=ctrl, #16]
 *                                 ctrl+16 = stTmpBuf.u64PhyAddr ✓
 *   +56  u32  src_batch_num     = forward_arg.astSrc[0].u32Num
 *                                 source: ldr fp, [r7=fwd_arg, #32]
 *                                 fwd_arg+32 = astSrc[0]+24 = u32Num
 *                                 SVP_BLOB_S: enType(4)+Stride(4)+VirAddr(8)
 *                                            +PhyAddr(8)+u32Num(4)+... = 48
 *   +60  u32  reserved          = 0
 *
 * After the 64-B header, fill_forward_task continues with a variable-
 * length section starting at task_node+64 (16-byte aligned, zero-padded
 * gap). The trailing region contains:
 *
 *   1. A per-input-blob stride table: one u32 per astSrc[i], packed,
 *      i ∈ [0..SrcNum-1]. Source: each entry = astSrc[i].u32Stride from
 *      fwd_arg+8+48*i+4. Loop @91d4-91f4.
 *
 *   2. For non-LSTM segments (net_type != 2), a copy of the per-input
 *      and per-output node shape data from pstModel->astSeg[NetSegId].
 *      astSrcNode[i]/astDstNode[i] (Loop @9260-93e8 unrolled for
 *      DstNum ∈ [1..16]). Each entry is 8 B (two u64s, ldrd/strd).
 *
 *   3. For CNN/ROI nets, a per-source DMA address vector: for each of
 *      astSrc[i].u32Num batches, computes
 *      astSrc[i].u64PhyAddr + j * (astSrc[i].u32Stride *
 *         astSrc[i].u32Height * astSrc[i].u32Chn)
 *      and writes the resulting 64-bit phys address. Loop @9428-9494
 *      uses umlal to do 32×32→64-bit multiply-accumulate.
 *
 *   4. Image source helper for LSTM (net_type==2, src_type ∈ [1..3]):
 *      calls svp_nnie_fill_image_src_addr (@0x78ac) instead of the
 *      simple DMA-stride generator. Path @9580-95e8.
 *
 *   5. Optional cache flush (94a0-94ac): if ctrl + 32 is set, calls
 *      hil_mmb_flush_dcache_byaddr with parameters from fwd_arg+1592
 *      and fwd_arg+1608.
 *
 * Phase 4 wiring:
 *   - Allocate a 64-B descriptor on stack (or DMA-coherent region),
 *     populate with the fixed-layout fields above.
 *   - Append the variable-length sections.
 *   - Pass descriptor phys to NNIE start register (TBD: which offset
 *     in the 0x11100000 window? Discovered by RE'ing svp_nnie_start_task
 *     @0x1934 — likely a single u32 write of descriptor phys, then a
 *     "go" bit. cmpi-mediated, so requires open_sys.ko interop).
 *   - Surround the dispatch with nnie_sys_set_bit/clear_bit on the
 *     0x12020034 RAM-using register (Phase 3 finding).
 *   - Wait on g_nnie_done completion or poll status (IRQ flags issue
 *     with vendor open_gdc.ko sharing SPI 53/54 — TBD).
 */

#ifndef _NNIE_HW_TASK_H_
#define _NNIE_HW_TASK_H_

#include <linux/types.h>

#define NNIE_HW_TASK_SIZE   64u

/* Offset of pstModel->stBase within SVP_NNIE_MODEL_S, used by the
 * vendor as a literal in `movw r2, #0x3690; ldrd r2, [r2, r6=pstModel]`.
 * Cross-checked against the kernel headers using the cv500 ARM toolchain:
 *
 *   sizeof(SVP_NNIE_NODE_S)         = 52
 *   sizeof(SVP_NNIE_SEG_S)          = 1692  ← matches `movw sl, #0x69c`
 *   sizeof(SVP_NNIE_ROIPOOL_INFO_S) = 104   (with all three HI_BOOL
 *                                            ping-pang flags)
 *   sizeof(SVP_MEM_INFO_S)          = 24
 *   sizeof(SVP_NNIE_MODEL_S)        = 13992 ← matches vendor copy_from_user
 *                                              size 0x36a8
 *   offsetof(MODEL_S, stBase)       = 13968 = 0x3690 ✓
 *
 * = 12 (enRunMode+u32TmpBufSize+u32NetSegNum)
 * + 1692 * 8  (astSeg[SVP_NNIE_MAX_NET_SEG_NUM=8])
 * + 104  * 4  (astRoiInfo[SVP_NNIE_MAX_ROI_LAYER_NUM=4])
 * = 12 + 13536 + 416 = 13964, then +4 alignment slack before stBase = 13968.
 */
#define NNIE_MODEL_STBASE_OFFSET    0x3690u

/* Stride between consecutive astSeg[i] entries in SVP_NNIE_MODEL_S,
 * used by `mla r9, #1692, NetSegId, pstModel`. Equals sizeof(SVP_NNIE_SEG_S)
 * = 4 + 8 + 8 + 16*52 + 16*52 + 8 = 1692. */
#define NNIE_SEG_S_STRIDE           1692u

/* offsetof(SVP_NNIE_SEG_S, u32InstOffset) - sizeof(MODEL_S_header).
 * The vendor uses `[r9, #24]` to mean astSeg[NetSegId].u32InstOffset
 * after r9 = pstModel + 1692*NetSegId. */
#define NNIE_SEG_INST_OFFSET_AT_24  24u
#define NNIE_SEG_INST_LEN_AT_28     28u

/* Field offsets within the 64-byte HW task descriptor */
#define NNIE_TASK_OFF_TRIGGER       0
#define NNIE_TASK_OFF_STATUS        2
#define NNIE_TASK_OFF_MODEL_PHYS    16
#define NNIE_TASK_OFF_INST_OFFSET   24
#define NNIE_TASK_OFF_INST_LEN      28
#define NNIE_TASK_OFF_TSK_PHYS      32
#define NNIE_TASK_OFF_TMP_PHYS      48
#define NNIE_TASK_OFF_BATCH_NUM     56

struct nnie_hw_task {
	__le16 trigger_mode;    /* +0:  1=instant/sync, 0=queued */
	__le16 status;          /* +2:  0=initial, IRQ updates */
	__le32 reserved_04;     /* +4 */
	__le64 reserved_08;     /* +8 */
	__le64 model_file_phys; /* +16: pstModel->stBase.u64PhyAddr */
	__le32 seg_inst_offset; /* +24: astSeg[NetSegId].u32InstOffset */
	__le32 seg_inst_len;    /* +28: astSeg[NetSegId].u32InstLen */
	__le64 tsk_buf_phys;    /* +32: ctrl.stTskBuf.u64PhyAddr */
	__le64 reserved_28;     /* +40 */
	__le64 tmp_buf_phys;    /* +48: ctrl.stTmpBuf.u64PhyAddr */
	__le32 src_batch_num;   /* +56: astSrc[0].u32Num */
	__le32 reserved_3c;     /* +60 */
} __packed;

_Static_assert(sizeof(struct nnie_hw_task) == NNIE_HW_TASK_SIZE,
	       "nnie_hw_task must be exactly 64 bytes");

#endif /* _NNIE_HW_TASK_H_ */
