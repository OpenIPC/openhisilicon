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
 * (size 0x12d8) — see git log for the RE walkthrough. The decode is
 * partial; only the fields we've confirmed by tracing the loader's
 * read paths are exposed. Unknown fields are left as opaque u8 arrays.
 *
 * High-level layout:
 *   [0     ..    3 ] u32  CRC32 of bytes [4..u32End)
 *   [4     ..   15 ] header preamble (size/offset table)
 *   [16    ..   19 ] u8[4] format version digits {1,1,1,2}
 *   [20    ..   45 ] u8[26] build timestamp ASCII "YYYYMMDDhhmmssNN..."
 *   [48..0xBF (191)] header tail (instOffset, instLen, segments table)
 *   [0xC0  ..       ] instruction stream + weights + quantization tables
 *
 * Minimum file size enforced: header (192 B) + an instruction region
 * referenced by header[52..55] (length) and header[56..59] (more
 * length); bounds check at 0x1cac ensures (a + b + tail) <= filesize.
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

/* The 192-byte file header. Field offsets are *file* offsets, matching
 * what the loader reads via memcpy_s into a stack buffer at sp+84 (so
 * file[N] == buffer[N], i.e. our offsets match the file directly). */
typedef struct __attribute__((packed)) nnie_wk_header {
	uint32_t crc32;                /* [0..3]   CRC32 of bytes [4..filesize) */
	uint8_t  unk_04_0F[12];        /* [4..15]  Phase 2 TODO — header size? offsets? */
	uint8_t  ver_maj;              /* [16]     must == 1 */
	uint8_t  ver_min;              /* [17]     must == 1 */
	uint8_t  ver_3;                /* [18]     must == 1 */
	uint8_t  ver_4;                /* [19]     must == 2 */
	uint8_t  build_str[26];        /* [20..45] ASCII "YYYYMMDDhhmmssNN..." */
	uint16_t unk_2E_2F;            /* [46..47] */
	uint8_t  run_mode;             /* [48]     copied to SVP_NNIE_MODEL_S.enRunMode */
	uint8_t  net_seg_num;          /* [49]     copied to SVP_NNIE_MODEL_S.u32NetSegNum */
	uint8_t  unk_32_33[2];         /* [50..51] */
	uint32_t inst_offset_extra;    /* [52..55] sp+136 read; must be > 0xBF, used in bounds check */
	uint32_t inst_len;             /* [56..59] sp+140 read; non-zero, used in bounds check */
	uint8_t  unk_3C_5F[36];        /* [60..95]  */
	uint32_t unk_60;               /* [96..99] sp+96 read */
	uint8_t  unk_64_AF[76];        /* [100..175] timestamps + reserved */
	uint32_t unk_B0;               /* [176..179] sp+260 read; must equal inst_offset_extra */
	uint32_t unk_B4;               /* [180..183] sp+264 read; must be > 47 */
	uint8_t  unk_B8_BF[8];         /* [184..191] */
} nnie_wk_header_t;

_Static_assert(sizeof(nnie_wk_header_t) == NNIE_WK_HEADER_SIZE,
	       "nnie_wk_header_t must be exactly 192 bytes");

/* Forward declarations for Phase 2+ work. The full parser will produce
 * a populated SVP_NNIE_MODEL_S (defined in <hi_nnie.h>) from a .wk
 * file in memory. */
struct hiSVP_NNIE_MODEL_S;
struct hiSVP_SRC_MEM_INFO_S;

int nnie_wk_verify_crc(const void *file_buf, uint32_t file_size);
int nnie_wk_parse(const struct hiSVP_SRC_MEM_INFO_S *src,
		  struct hiSVP_NNIE_MODEL_S *out_model);

#endif /* NNIE_WK_FORMAT_H */
