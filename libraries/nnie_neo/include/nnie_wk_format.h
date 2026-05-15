/*
 * SPDX-License-Identifier: GPL-2.0
 *
 * NNIE .wk model file format — partial decode (Phase 2 of #111).
 *
 * The vendor "wired-kernel" model file (.wk extension) is a self-
 * contained binary produced by HiSilicon's `nnie_mapper` offline
 * converter from a Caffe protobuf + a quantization dataset. The
 * runtime userspace library HI_MPI_SVP_NNIE_LoadModel parses this
 * file in-memory (the caller MmzAlloc's a buffer, reads the file
 * contents, then passes the (phys, virt, size) tuple via
 * SVP_SRC_MEM_INFO_S).
 *
 * Decoded from vendor libnnie.so HI_MPI_SVP_NNIE_LoadModel @0x1bf4
 * (size 0x12d8) — see git log for the RE walkthrough. CRC + version +
 * top-level header fields + segment record header are confirmed
 * against the vendor mnist.wk sample (466176 B):
 *
 *   File header (192 B) → SVP_NNIE_MODEL_S top-level fields.
 *   Segment table at file[176] (= inst_offset_extra mirror; see below):
 *     N records of 16+ B each, N = file[49] = u32NetSegNum (1..8).
 *     Each record decodes to SVP_NNIE_SEG_S {enNetType, u16SrcNum,
 *     u16DstNum, u16RoiPoolNum, u16MaxStep, u32InstOffset, u32InstLen,
 *     astSrcNode[16], astDstNode[16], au32RoiIdx[2]}.
 *
 * Still TODO (Phase 3 prereq):
 *   - Node record layout (SVP_NNIE_NODE_S — enType, WHC/Dim union,
 *     u32NodeId, szName[32]).
 *   - ROIPool record layout (SVP_NNIE_ROIPOOL_INFO_S).
 *   - u32TmpBufSize derivation (file[96] read at sp+96).
 *
 * High-level layout:
 *   [0     ..    3 ] u32  CRC32 of bytes [4..file[52]+file[56])
 *                          poly=0xEDB88320 (IEEE), init=0, finxor=~
 *   [4     ..   15 ] preamble (size hints — partial decode)
 *   [16    ..   19 ] u8[4] format version digits {1,1,1,2}
 *   [20    ..   45 ] u8[26] build timestamp ASCII "YYYYMMDDhhmmss..."
 *   [48    ..   59 ] run_mode/net_seg_num/inst_off/inst_len
 *   [60..0xBF (191)] header tail (mirrors of inst_off/inst_len etc.)
 *   [0xC0  ..       ] instruction stream + weights + quantization tables
 *
 * CRC coverage = bytes [4..file[52]+file[56]); the unprotected tail
 * (file[52]+file[56]..filesize) holds weights/quant-tables that the
 * instruction stream addresses by file offset.
 */

#ifndef NNIE_WK_FORMAT_H
#define NNIE_WK_FORMAT_H

#include <stdint.h>

#define NNIE_WK_HEADER_SIZE     192u
#define NNIE_WK_MIN_FILE_SIZE   192u   /* header alone; instruction stream is extra */
#define NNIE_WK_VER_MAJ          1     /* file[16] required value */
#define NNIE_WK_VER_MIN          1     /* file[17] required value */
#define NNIE_WK_VER_3            1     /* file[18] required value */
#define NNIE_WK_VER_4            2     /* file[19] required value */
#define NNIE_WK_MAX_NET_SEG_NUM  8u    /* file[49] u8 ∈ [1..8] */

/* The 192-byte file header. Field offsets are *file* offsets, matching
 * what the loader reads via memcpy_s into a stack buffer at sp+84 (so
 * file[N] == buffer[N], i.e. our offsets match the file directly). */
typedef struct __attribute__((packed)) nnie_wk_header {
	uint32_t crc32;                /* [0..3]   CRC32 of bytes [4..file[52]+file[56]) */
	uint8_t  unk_04_0F[12];        /* [4..15]  Phase 2 TODO — header size? offsets? */
	uint8_t  ver_maj;              /* [16]     must == 1 */
	uint8_t  ver_min;              /* [17]     must == 1 */
	uint8_t  ver_3;                /* [18]     must == 1 */
	uint8_t  ver_4;                /* [19]     must == 2 */
	uint8_t  build_str[26];        /* [20..45] ASCII "YYYYMMDDhhmmss..." */
	uint16_t unk_2E_2F;            /* [46..47] */
	uint8_t  run_mode;             /* [48]     -> SVP_NNIE_MODEL_S.enRunMode */
	uint8_t  net_seg_num;          /* [49]     -> u32NetSegNum, ∈ [1..8] */
	uint8_t  unk_32_33[2];         /* [50..51] */
	uint32_t inst_offset_extra;    /* [52..55] header tail offset (> 0xBF) */
	uint32_t inst_len;             /* [56..59] instruction-stream byte length */
	uint32_t tmp_buf_size;         /* [60..63] -> SVP_NNIE_MODEL_S.u32TmpBufSize */
	uint8_t  unk_40_5F[32];        /* [64..95] */
	uint32_t unk_60;               /* [96..99] non-zero check (sp+96) */
	uint8_t  unk_64_AF[76];        /* [100..175] timestamps + reserved */
	uint32_t inst_offset_extra_dup;/* [176..179] must equal inst_offset_extra */
	uint32_t seg_tab_len;          /* [180..183] segment-table length (> 47) */
	uint8_t  unk_B8_BF[8];         /* [184..191] */
} nnie_wk_header_t;

_Static_assert(sizeof(nnie_wk_header_t) == NNIE_WK_HEADER_SIZE,
	       "nnie_wk_header_t must be exactly 192 bytes");

/* Per-segment record in the segment table at file[inst_offset_extra].
 * Loader at 1e90-1edc reads these fields and copies them into the
 * corresponding SVP_NNIE_SEG_S layout (which is the public API struct
 * declared in <hi_nnie.h>). The on-disk format is more compact:
 * SrcNum/DstNum/RoiPoolNum are u8 in the file but u16 in the struct.
 *
 * The "after_hdr" region (offset 16+) presumably holds the node arrays
 * (astSrcNode[SrcNum] + astDstNode[DstNum]) and the ROIPool index
 * pairs. Layout TODO in Phase 3. */
typedef struct __attribute__((packed)) nnie_wk_seg_record {
	uint8_t  net_type;             /* [0]     -> enNetType,  must be <= 2 */
	uint8_t  src_num;              /* [1]     -> u16SrcNum,  <= 16 */
	uint8_t  dst_num;              /* [2]     -> u16DstNum,  <= 16 */
	uint8_t  roi_pool_num;         /* [3]     -> u16RoiPoolNum, <= 4 */
	uint16_t max_step;             /* [4..5]  -> u16MaxStep, <= 1024 */
	uint16_t unk_06_07;            /* [6..7]  */
	uint32_t inst_offset;          /* [8..11] -> u32InstOffset (bounds-checked vs
	                                                 inst_offset_extra + inst_len) */
	uint32_t inst_len;             /* [12..15] -> u32InstLen, alignment-checked (& 15 == 0) */
	/* Then: src node records + dst node records + ROIPool index pairs.
	 * Total record size depends on src_num + dst_num — Phase 3 decodes. */
} nnie_wk_seg_record_t;

_Static_assert(sizeof(nnie_wk_seg_record_t) == 16,
	       "nnie_wk_seg_record_t must be exactly 16 bytes (header part)");

/* Forward declarations for Phase 2+ work. The full parser will produce
 * a populated SVP_NNIE_MODEL_S (defined in <hi_nnie.h>) from a .wk
 * file in memory. */
struct hiSVP_NNIE_MODEL_S;
struct hiSVP_SRC_MEM_INFO_S;

int nnie_wk_verify_crc(const void *file_buf, uint32_t file_size);
int nnie_wk_parse(const struct hiSVP_SRC_MEM_INFO_S *src,
		  struct hiSVP_NNIE_MODEL_S *out_model);

#endif /* NNIE_WK_FORMAT_H */
