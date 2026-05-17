/*
 * SPDX-License-Identifier: GPL-2.0
 *
 * Host-runnable sanity test for HI_MPI_SVP_NNIE_LoadModel.
 *
 * Synthesises a minimal .wk file in memory that mirrors the structure
 * of a real mnist model (1 segment, 1 input, 1 output, 28×28×1 input,
 * named "data"), feeds it to LoadModel, and asserts every decoded
 * field matches what the real parser produced for the vendor mnist
 * .wk in development. Catches regressions in:
 *
 *   - .wk header decode (run_mode, net_seg_num, inst_off_extra)
 *   - CRC32 verifier (vendor uses init=0, final XOR, non-standard zlib
 *     CRC32 setup)
 *   - Segment record decode (net_type, src/dst counts, inst offset/len)
 *   - Per-node compact form (H/W/C, enType, NodeId, name)
 *   - astSrcNode[0].enType in particular — was the load-bearing field
 *     that took NNIE Phase 14 to fix
 *
 * Pure host test: doesn't touch /dev/nnie, doesn't need cross-compile,
 * runs in <50ms on a CI worker.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "hi_type.h"
#include "hi_common.h"
#include "hi_comm_svp.h"
#include "hi_nnie.h"
#include "mpi_nnie.h"
#include "nnie_wk_format.h"

/* IEEE 802.3 CRC-32, reflected, poly 0xEDB88320 — vendor variant uses
 * init=0 (NOT ~0) and final XOR ~. Has to match nnie_wk_verify_crc()
 * in libnnie_neo bit-for-bit. */
static uint32_t crc32_step(uint32_t crc, uint8_t b)
{
	int i;

	crc ^= b;
	for (i = 0; i < 8; i++)
		crc = (crc >> 1) ^ (0xEDB88320u & -(int)(crc & 1));
	return crc;
}

static uint32_t crc32_range(const uint8_t *buf, size_t off, size_t end)
{
	uint32_t crc = 0;
	size_t i;

	for (i = off; i < end; i++)
		crc = crc32_step(crc, buf[i]);
	return ~crc;
}

/* Layout of the synthesised file:
 *   [0..191]     header (192 B)
 *   [192..207]   segment record (16 B)
 *   [208..255]   src node compact (16 B) + name (32 B)
 *   [256..303]   dst node compact (16 B) + name (32 B)
 *   [304..0x150) header-tail filler (zeros)
 *   [0x150..]    instruction stream region (header_extra to end of inst)
 *   then weights / quant tables follow (not CRC'd)
 *
 * The seg record's u32InstOffset must point at a 16-byte-aligned offset
 * inside [inst_off_extra, file_size). LoadModel checks
 * `inst_off + inst_len <= file_size`. */
#define WK_HEADER_EXTRA  0x150u    /* file[52]: where instruction stream starts */
#define WK_INST_LEN      0x100u    /* short synthetic instruction stream */
#define WK_FILE_SIZE     (WK_HEADER_EXTRA + WK_INST_LEN + 16u)  /* + tail pad */

static void put_le32(uint8_t *p, uint32_t v)
{
	p[0] = v; p[1] = v >> 8; p[2] = v >> 16; p[3] = v >> 24;
}

static void put_le16(uint8_t *p, uint16_t v)
{
	p[0] = v; p[1] = v >> 8;
}

static void synth_wk(uint8_t *buf, size_t buf_size)
{
	uint8_t *seg, *src_node, *dst_node;
	uint32_t crc;

	assert(buf_size >= WK_FILE_SIZE);
	memset(buf, 0, buf_size);

	/* Header (192 B). */
	buf[16] = NNIE_WK_VER_MAJ;       /* file[16..19] = {1,1,1,2} */
	buf[17] = NNIE_WK_VER_MIN;
	buf[18] = NNIE_WK_VER_3;
	buf[19] = NNIE_WK_VER_4;
	buf[48] = 0;                     /* run_mode */
	buf[49] = 1;                     /* net_seg_num = 1 */
	put_le32(buf + 52, WK_HEADER_EXTRA);  /* inst_offset_extra */
	put_le32(buf + 56, WK_INST_LEN);      /* inst total length */

	/* Segment record (16 B at file[192]). */
	seg = buf + 192;
	seg[0] = 0;                     /* net_type: 0 = CNN */
	seg[1] = 1;                     /* src_num */
	seg[2] = 1;                     /* dst_num */
	seg[3] = 0;                     /* roi_pool_num */
	put_le16(seg + 4, 0);           /* max_step */
	/* The seg's u32InstOffset must be inside the CRC range
	 * [inst_offset_extra, inst_offset_extra + inst_len). Pick the
	 * start of the instruction stream itself. */
	put_le32(seg + 8,  WK_HEADER_EXTRA);  /* u32InstOffset */
	put_le32(seg + 12, WK_INST_LEN);      /* u32InstLen */

	/* Src node (48 B): mnist-style 28×28×1 with enType=1, name "data". */
	src_node = buf + 208;
	put_le32(src_node + 0,  28);    /* height */
	put_le32(src_node + 4,  28);    /* width */
	put_le32(src_node + 8,   1);    /* chn */
	src_node[14] = 1;               /* enType (SVP_BLOB_TYPE_YVU420SP) */
	src_node[15] = 0;               /* NodeId */
	memcpy(src_node + 16, "data", 4);

	/* Dst node (48 B): mnist-style 10-class output, name "prob". */
	dst_node = buf + 256;
	put_le32(dst_node + 0,   1);    /* height */
	put_le32(dst_node + 4,   1);    /* (chn in file order) */
	put_le32(dst_node + 8,  10);    /* (width in file order — vendor's
	                                   libnnie remaps so Width holds the
	                                   layer's feature count) */
	memcpy(dst_node + 16, "prob", 4);

	/* Fill the instruction stream region with a deterministic pattern
	 * so the CRC isn't trivially zero. */
	for (size_t i = WK_HEADER_EXTRA;
	     i < WK_HEADER_EXTRA + WK_INST_LEN; i++)
		buf[i] = (uint8_t)(i * 0x9e + 0x37);

	/* Compute CRC over bytes [4..inst_offset_extra + inst_len) and
	 * write it at file[0..3]. */
	crc = crc32_range(buf, 4, WK_HEADER_EXTRA + WK_INST_LEN);
	put_le32(buf + 0, crc);
}

#define CHECK(cond, fmt, ...) do {                                          \
	if (!(cond)) {                                                      \
		fprintf(stderr, "FAIL %s:%d: %s — " fmt "\n",               \
		        __FILE__, __LINE__, #cond, ##__VA_ARGS__);          \
		exit(1);                                                    \
	}                                                                   \
} while (0)

int main(void)
{
	uint8_t buf[WK_FILE_SIZE];
	SVP_SRC_MEM_INFO_S mem = { 0 };
	SVP_NNIE_MODEL_S model;
	HI_S32 ret;

	synth_wk(buf, sizeof(buf));

	mem.u64VirAddr = (uintptr_t)buf;
	mem.u64PhyAddr = 0xAA880000ULL;     /* arbitrary stable phys */
	mem.u32Size    = sizeof(buf);

	ret = HI_MPI_SVP_NNIE_LoadModel(&mem, &model);
	CHECK(ret == HI_SUCCESS, "LoadModel returned 0x%x", (unsigned)ret);

	/* Top-level model. */
	CHECK(model.u32NetSegNum == 1, "got %u", model.u32NetSegNum);
	CHECK(model.enRunMode == 0,    "got %u", (unsigned)model.enRunMode);
	CHECK(model.stBase.u64PhyAddr == 0xAA880000ULL + WK_HEADER_EXTRA,
	      "stBase.PhyAddr should be rebased by inst_offset_extra; got 0x%llx",
	      (unsigned long long)model.stBase.u64PhyAddr);

	/* Segment 0. */
	CHECK(model.astSeg[0].enNetType  == 0,   "got %u", model.astSeg[0].enNetType);
	CHECK(model.astSeg[0].u16SrcNum  == 1,   "got %u", model.astSeg[0].u16SrcNum);
	CHECK(model.astSeg[0].u16DstNum  == 1,   "got %u", model.astSeg[0].u16DstNum);
	CHECK(model.astSeg[0].u32InstOffset == WK_HEADER_EXTRA,
	      "got 0x%x", model.astSeg[0].u32InstOffset);
	CHECK(model.astSeg[0].u32InstLen == WK_INST_LEN,
	      "got 0x%x", model.astSeg[0].u32InstLen);

	/* Src node — the field that took NNIE Phase 14 to get right. */
	CHECK(model.astSeg[0].astSrcNode[0].enType == 1,
	      "src enType must be YVU420SP (1); got %u",
	      (unsigned)model.astSeg[0].astSrcNode[0].enType);
	CHECK(model.astSeg[0].astSrcNode[0].unShape.stWhc.u32Width  == 28,
	      "got %u", model.astSeg[0].astSrcNode[0].unShape.stWhc.u32Width);
	CHECK(model.astSeg[0].astSrcNode[0].unShape.stWhc.u32Height == 28,
	      "got %u", model.astSeg[0].astSrcNode[0].unShape.stWhc.u32Height);
	CHECK(model.astSeg[0].astSrcNode[0].unShape.stWhc.u32Chn    == 1,
	      "got %u", model.astSeg[0].astSrcNode[0].unShape.stWhc.u32Chn);
	CHECK(model.astSeg[0].astSrcNode[0].u32NodeId == 0,
	      "got %u", model.astSeg[0].astSrcNode[0].u32NodeId);
	CHECK(strcmp(model.astSeg[0].astSrcNode[0].szName, "data") == 0,
	      "got '%s'", model.astSeg[0].astSrcNode[0].szName);

	/* Dst node — vendor remaps fields so Width holds the layer's
	 * feature count (10 for mnist) and enType is hardcoded to S32. */
	CHECK(model.astSeg[0].astDstNode[0].enType == SVP_BLOB_TYPE_S32,
	      "dst enType must be S32 (4); got %u",
	      (unsigned)model.astSeg[0].astDstNode[0].enType);
	CHECK(model.astSeg[0].astDstNode[0].unShape.stWhc.u32Width  == 10,
	      "dst W (feature count) should be 10; got %u",
	      model.astSeg[0].astDstNode[0].unShape.stWhc.u32Width);
	CHECK(model.astSeg[0].astDstNode[0].u32NodeId == 8,
	      "dst NodeId pattern (j+1)*8 → 8; got %u",
	      model.astSeg[0].astDstNode[0].u32NodeId);
	CHECK(strcmp(model.astSeg[0].astDstNode[0].szName, "prob") == 0,
	      "got '%s'", model.astSeg[0].astDstNode[0].szName);

	/* Tear-down: must zero the struct + return HI_SUCCESS. */
	ret = HI_MPI_SVP_NNIE_UnloadModel(&model);
	CHECK(ret == HI_SUCCESS, "UnloadModel returned 0x%x", (unsigned)ret);
	CHECK(model.u32NetSegNum == 0, "model not zeroed by UnloadModel");

	/* Negative tests: bad CRC, bad version, undersized buffer. */
	{
		SVP_NNIE_MODEL_S m;

		/* corrupt CRC */
		buf[0] ^= 0xff;
		CHECK(HI_MPI_SVP_NNIE_LoadModel(&mem, &m) != HI_SUCCESS,
		      "LoadModel must reject bad CRC");
		buf[0] ^= 0xff;

		/* corrupt version */
		buf[16] = 99;
		CHECK(HI_MPI_SVP_NNIE_LoadModel(&mem, &m) != HI_SUCCESS,
		      "LoadModel must reject bad version");
		buf[16] = NNIE_WK_VER_MAJ;

		/* undersized buffer */
		mem.u32Size = 10;
		CHECK(HI_MPI_SVP_NNIE_LoadModel(&mem, &m) != HI_SUCCESS,
		      "LoadModel must reject too-small buffer");
		mem.u32Size = sizeof(buf);
	}

	printf("ok: NNIE LoadModel sanity (synth .wk + negative cases)\n");
	return 0;
}
