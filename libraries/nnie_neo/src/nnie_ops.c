/*
 * SPDX-License-Identifier: GPL-2.0
 *
 * libnnie_neo — clean-room replacement for vendor libnnie.so on
 * Hi3516CV500/AV300/DV300. Userspace half of issue #111.
 *
 * STATUS: Phase 2 in progress.
 *   - Phase 0 (#111): kernel scaffold in kernel/nnie_neo/ — done.
 *   - Phase 1 (#111): ioctl ABI decoded — done.
 *   - Phase 2 (#111): model loader RE — partial. .wk header layout
 *     scaffolded (libraries/nnie_neo/include/nnie_wk_format.h). The
 *     CRC32 verifier path is wired below; the full layer/quant-table
 *     parser is not yet decoded.
 *
 * Vendor exports (from /usr/lib/libnnie.so, 42 KB, 8 entries):
 *   HI_MPI_SVP_NNIE_LoadModel        @0x1bf4 — size 0x12d8 (biggest)
 *   HI_MPI_SVP_NNIE_UnloadModel      @0x2ecc
 *   HI_MPI_SVP_NNIE_Forward          @0x2f84
 *   HI_MPI_SVP_NNIE_ForwardWithBbox  @0x2f88
 *   HI_MPI_SVP_NNIE_Query            @0x2f94
 *   HI_MPI_SVP_NNIE_GetTskBufSize    @0x1388
 *   HI_MPI_SVP_NNIE_AddTskBuf        @0x3100
 *   HI_MPI_SVP_NNIE_RemoveTskBuf     @0x3250
 *
 * Of these, only Forward, ForwardWithBbox, Query, AddTskBuf, and
 * RemoveTskBuf reach /dev/nnie. The other three (LoadModel,
 * UnloadModel, GetTskBufSize) are pure-userspace and never call ioctl.
 */
#include "hi_type.h"
#include "hi_common.h"
#include "hi_comm_svp.h"
#include "hi_nnie.h"
#include "mpi_nnie.h"
#include "nnie_wk_format.h"

#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* IEEE 802.3 CRC-32 polynomial 0xEDB88320 (reflected), the canonical
 * zlib CRC32. Loader uses a 256-entry table-driven implementation
 * (sequence at libnnie.so 0x1cdc..0x1d0c). For our verify path we
 * compute it directly; performance is fine for one-shot LoadModel use. */
static uint32_t nnie_wk_crc32_step(uint32_t crc, uint8_t b)
{
	int i;
	crc ^= b;
	for (i = 0; i < 8; i++)
		crc = (crc >> 1) ^ (0xEDB88320u & -(int)(crc & 1));
	return crc;
}

int nnie_wk_verify_crc(const void *file_buf, uint32_t file_size)
{
	const uint8_t *p = (const uint8_t *)file_buf;
	uint32_t stored, computed = 0xFFFFFFFFu;
	uint32_t i;

	if (file_size < NNIE_WK_MIN_FILE_SIZE)
		return -1;

	stored = (uint32_t)p[0] | ((uint32_t)p[1] << 8) |
	         ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);

	for (i = 4; i < file_size; i++)
		computed = nnie_wk_crc32_step(computed, p[i]);
	computed = ~computed;

	return (stored == computed) ? 0 : -1;
}

/* Phase 2 stub — full parser deferred. Returns NOT_SUPPORT but
 * exercises the CRC + version checks so callers see the right errno
 * for malformed files. */
HI_S32 HI_MPI_SVP_NNIE_LoadModel(const SVP_SRC_MEM_INFO_S *pstModelBuf,
                                 SVP_NNIE_MODEL_S *pstModel)
{
	const uint8_t *file;
	uint32_t file_size;

	if (!pstModelBuf || !pstModel)
		return HI_ERR_SVP_NNIE_NULL_PTR;
	if (!pstModelBuf->u64VirAddr)
		return HI_ERR_SVP_NNIE_BADADDR;

	file = (const uint8_t *)(uintptr_t)pstModelBuf->u64VirAddr;
	file_size = pstModelBuf->u32Size;

	if (file_size < NNIE_WK_HEADER_SIZE)
		return HI_ERR_SVP_NNIE_ILLEGAL_PARAM;
	if (nnie_wk_verify_crc(file, file_size) != 0)
		return HI_ERR_SVP_NNIE_ILLEGAL_PARAM;
	if (file[16] != NNIE_WK_VER_MAJ || file[17] != NNIE_WK_VER_MIN ||
	    file[18] != NNIE_WK_VER_3   || file[19] != NNIE_WK_VER_4)
		return HI_ERR_SVP_NNIE_ILLEGAL_PARAM;

	/* Phase 2 stops here. Phase 3 will:
	 *   - memset(pstModel, 0, sizeof(*pstModel))
	 *   - parse header offsets [52..55] (inst_offset_extra),
	 *     [56..59] (inst_len), [132] (enRunMode), [133] (NetSegNum)
	 *   - iterate astSeg[u32NetSegNum] from the segment table
	 *   - iterate astRoiInfo[] from the ROI pool table
	 *   - fill pstModel->stBase = *pstSrc
	 */
	return HI_ERR_SVP_NNIE_NOT_SURPPORT;
}

HI_S32 HI_MPI_SVP_NNIE_UnloadModel(SVP_NNIE_MODEL_S *pstModel)
{
	if (!pstModel)
		return HI_ERR_SVP_NNIE_NULL_PTR;
	memset(pstModel, 0, sizeof(*pstModel));
	return HI_SUCCESS;
}

HI_S32 HI_MPI_SVP_NNIE_GetTskBufSize(HI_U32 u32MaxInputNum, HI_U32 u32MaxBboxNum,
                                     const SVP_NNIE_MODEL_S *pstModel,
                                     HI_U32 au32TskBufSize[],
                                     HI_U32 u32NetSegNum)
{
	/* Phase 2 stub. Vendor walks the parsed model to compute per-core
	 * task buffer sizes. Without a parsed model (Phase 3+), we can't
	 * produce a meaningful answer — reject. */
	(void)u32MaxInputNum; (void)u32MaxBboxNum;
	(void)pstModel; (void)au32TskBufSize; (void)u32NetSegNum;
	return HI_ERR_SVP_NNIE_NOT_SURPPORT;
}

HI_S32 HI_MPI_SVP_NNIE_Forward(SVP_NNIE_HANDLE *phSvpNnieHandle,
                               const SVP_SRC_BLOB_S astSrc[],
                               const SVP_NNIE_MODEL_S *pstModel,
                               const SVP_DST_BLOB_S astDst[],
                               const SVP_NNIE_FORWARD_CTRL_S *pstForwardCtrl,
                               HI_BOOL bInstant)
{
	(void)phSvpNnieHandle; (void)astSrc; (void)pstModel;
	(void)astDst; (void)pstForwardCtrl; (void)bInstant;
	return HI_ERR_SVP_NNIE_NOT_SURPPORT;
}

HI_S32 HI_MPI_SVP_NNIE_ForwardWithBbox(SVP_NNIE_HANDLE *phSvpNnieHandle,
                                       const SVP_SRC_BLOB_S astSrc[],
                                       const SVP_SRC_BLOB_S astBbox[],
                                       const SVP_NNIE_MODEL_S *pstModel,
                                       const SVP_DST_BLOB_S astDst[],
                                       const SVP_NNIE_FORWARD_WITHBBOX_CTRL_S *pstForwardCtrl,
                                       HI_BOOL bInstant)
{
	(void)phSvpNnieHandle; (void)astSrc; (void)astBbox; (void)pstModel;
	(void)astDst; (void)pstForwardCtrl; (void)bInstant;
	return HI_ERR_SVP_NNIE_NOT_SURPPORT;
}

HI_S32 HI_MPI_SVP_NNIE_Query(SVP_NNIE_ID_E enNnieId,
                             SVP_NNIE_HANDLE svpNnieHandle,
                             HI_BOOL *pbFinish, HI_BOOL bBlock)
{
	(void)enNnieId; (void)svpNnieHandle; (void)bBlock;
	if (!pbFinish) return HI_ERR_SVP_NNIE_NULL_PTR;
	*pbFinish = HI_TRUE;            /* synchronous dispatch (Phase 3+) */
	return HI_SUCCESS;
}

HI_S32 HI_MPI_SVP_NNIE_AddTskBuf(const SVP_MEM_INFO_S *pstTskBuf)
{
	(void)pstTskBuf;
	return HI_ERR_SVP_NNIE_NOT_SURPPORT;
}

HI_S32 HI_MPI_SVP_NNIE_RemoveTskBuf(const SVP_MEM_INFO_S *pstTskBuf)
{
	(void)pstTskBuf;
	return HI_ERR_SVP_NNIE_NOT_SURPPORT;
}
