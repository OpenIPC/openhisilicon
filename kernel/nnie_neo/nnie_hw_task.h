/* SPDX-License-Identifier: GPL-2.0 */
/*
 * cv500 NNIE 64-byte HW task descriptor + variable-length tskbuf tail.
 *
 * The descriptor lives in nocache MMZ; its phys is written to
 * TASK_ADDR_LO/HI before START. HW fetches descriptor + follows
 * tsk_buf_phys to read the variable-length tail that lists per-batch
 * source addresses and output destination addresses.
 *
 * Tskbuf tail layout for non-LSTM nets (built by nnie_build_task_tail):
 *
 *   §1  src strides (SrcNum × u32) followed by dst strides (DstNum × u32)
 *       Padded to 16-byte alignment.
 *   §2  dst phys (DstNum × u64), padded to 16-byte alignment.
 *   §3  Per-source DMA address vector. For each input i:
 *         enType=0 (U8C1):       batch_size = stride * height * chn
 *         enType=1..3 (YUV):     batch_size = stride * height
 *         enType=4 (single):     batch_size = stride * height
 *       Writes Num × u64 = (phys + j*batch_size for j in [0..Num-1]).
 *       Padded to 16-byte alignment after each source.
 */

#ifndef _NNIE_HW_TASK_H_
#define _NNIE_HW_TASK_H_

#include <linux/types.h>

#define NNIE_HW_TASK_SIZE   64u

/* SVP_NNIE_MODEL_S layout constants. sizeof(MODEL_S) = 13992,
 * sizeof(SVP_NNIE_SEG_S) = 1692 on the cv500 ABI. */
#define NNIE_MODEL_STBASE_OFFSET    0x3690u
#define NNIE_SEG_S_STRIDE           1692u
#define NNIE_SEG_INST_OFFSET_AT_24  24u
#define NNIE_SEG_INST_LEN_AT_28     28u

#define NNIE_TASK_OFF_TRIGGER       0
#define NNIE_TASK_OFF_STATUS        2
#define NNIE_TASK_OFF_MODEL_PHYS    16
#define NNIE_TASK_OFF_INST_OFFSET   24
#define NNIE_TASK_OFF_INST_LEN      28
#define NNIE_TASK_OFF_TSK_PHYS      32
#define NNIE_TASK_OFF_TMP_PHYS      48
#define NNIE_TASK_OFF_BATCH_NUM     56

struct nnie_hw_task {
	__le16 trigger_mode;    /* +0:  1=instant/sync, 0=queued                 */
	__le16 status;          /* +2:  HW updates on completion                 */
	__le32 slot_idx;        /* +4:  per-task index, mod 512; HW tracks it    */
	__le64 reserved_08;     /* +8                                            */
	__le64 model_file_phys; /* +16: pstModel->stBase.u64PhyAddr              */
	__le32 seg_inst_offset; /* +24: astSeg[NetSegId].u32InstOffset           */
	__le32 seg_inst_len;    /* +28: astSeg[NetSegId].u32InstLen              */
	__le64 tsk_buf_phys;    /* +32: ctrl.stTskBuf.u64PhyAddr                 */
	__le64 reserved_28;     /* +40                                           */
	__le64 tmp_buf_phys;    /* +48: ctrl.stTmpBuf.u64PhyAddr                 */
	__le32 src_batch_num;   /* +56: astSrc[0].u32Num                         */
	__le32 reserved_3c;     /* +60                                           */
} __packed;

_Static_assert(sizeof(struct nnie_hw_task) == NNIE_HW_TASK_SIZE,
	       "nnie_hw_task must be exactly 64 bytes");

#endif /* _NNIE_HW_TASK_H_ */
