/*
 * SPDX-License-Identifier: GPL-2.0
 *
 * NNIE .wk model file format. Produced by HiSilicon's `nnie_mapper`
 * offline converter from a Caffe protobuf + a quantisation dataset.
 * The runtime userspace library parses it in-memory (the caller
 * MmzAlloc's a buffer, reads the file contents into it, then passes
 * the (phys, virt, size) tuple via SVP_SRC_MEM_INFO_S).
 *
 * High-level layout:
 *   [0    ..    3 ] u32  CRC32 of bytes [4..file[52]+file[56])
 *                          poly=0xEDB88320 (IEEE), init=0, finxor=~
 *   [4    ..   15 ] preamble
 *   [16   ..   19 ] format version digits {1,1,1,2}
 *   [20   ..   45 ] build timestamp ASCII "YYYYMMDDhhmmss..."
 *   [48   ..   59 ] run_mode / net_seg_num / inst_off / inst_len
 *   [60   .. 0xBF ] header tail
 *   [0xC0 ..      ] segment table + node tables + instruction stream +
 *                   weights + quantisation tables
 */

#ifndef NNIE_WK_FORMAT_H
#define NNIE_WK_FORMAT_H

#include <stdint.h>

#define NNIE_WK_HEADER_SIZE     192u
#define NNIE_WK_MIN_FILE_SIZE   192u
#define NNIE_WK_VER_MAJ          1
#define NNIE_WK_VER_MIN          1
#define NNIE_WK_VER_3            1
#define NNIE_WK_VER_4            2
#define NNIE_WK_MAX_NET_SEG_NUM  8u

/* 192-byte file header. Offsets are file offsets. */
typedef struct __attribute__((packed)) nnie_wk_header {
	uint32_t crc32;
	uint8_t  unk_04_0F[12];
	uint8_t  ver_maj;
	uint8_t  ver_min;
	uint8_t  ver_3;
	uint8_t  ver_4;
	uint8_t  build_str[26];
	uint16_t unk_2E_2F;
	uint8_t  run_mode;             /* → SVP_NNIE_MODEL_S.enRunMode */
	uint8_t  net_seg_num;          /* → u32NetSegNum, ∈ [1..8]     */
	uint8_t  unk_32_33[2];
	uint32_t inst_offset_extra;    /* header-tail offset (>= 0xC0) */
	uint32_t inst_len;             /* instruction-stream length    */
	uint32_t tmp_buf_size;         /* → SVP_NNIE_MODEL_S.u32TmpBufSize */
	uint8_t  unk_40_5F[32];
	uint32_t unk_60;
	uint8_t  unk_64_AF[76];
	uint32_t inst_offset_extra_dup;
	uint32_t seg_tab_len;
	uint8_t  unk_B8_BF[8];
} nnie_wk_header_t;

_Static_assert(sizeof(nnie_wk_header_t) == NNIE_WK_HEADER_SIZE,
	       "nnie_wk_header_t must be exactly 192 bytes");

/* Per-segment record at file[192]. Compact on-disk form: SrcNum /
 * DstNum / RoiPoolNum are u8 here but u16 in the in-memory struct. */
typedef struct __attribute__((packed)) nnie_wk_seg_record {
	uint8_t  net_type;             /* → enNetType (0=CNN, 2=RECURRENT) */
	uint8_t  src_num;
	uint8_t  dst_num;
	uint8_t  roi_pool_num;
	uint16_t max_step;
	uint16_t unk_06_07;
	uint32_t inst_offset;          /* file offset, 16-byte aligned */
	uint32_t inst_len;
} nnie_wk_seg_record_t;

_Static_assert(sizeof(nnie_wk_seg_record_t) == 16,
	       "nnie_wk_seg_record_t must be exactly 16 bytes");

/* Per-node record (48 bytes), one per src then one per dst, immediately
 * after the segment record(s):
 *   [+0..+11]  u32 height, u32 width, u32 chn
 *   [+12..+13] padding
 *   [+14]      u8  enType (SVP_BLOB_TYPE_E)
 *   [+15]      u8  node id
 *   [+16..+47] szName[32]
 */
#define NNIE_WK_NODE_SIZE       48u

int nnie_wk_verify_crc(const void *file_buf, uint32_t file_size);

#endif /* NNIE_WK_FORMAT_H */
