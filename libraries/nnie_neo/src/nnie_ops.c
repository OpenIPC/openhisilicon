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
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <pthread.h>

/* /dev/nnie ioctl numbers — see kernel/nnie_neo/nnie_neo.c. Layouts:
 *
 *   FORWARD              (1624 B I/O):
 *     +0   u32 handle (out)
 *     +8   SVP_BLOB_S astSrc[16]    (48 B each)
 *     +776 u64 pstModel user VA
 *     +784 SVP_BLOB_S astDst[16]
 *     +1552 SVP_NNIE_FORWARD_CTRL_S (64 B)
 *     +1616 u32 bInstant
 *
 *   ADD_TSKBUF / REMOVE_TSKBUF (24 B I/O): SVP_MEM_INFO_S
 */
#define NNIE_IOC_FORWARD              0xc6584d00u
#define NNIE_IOC_FORWARD_WITH_BBOX    0xc6c04d01u
#define NNIE_IOC_QUERY                0xc0184d02u
#define NNIE_IOC_ADD_TSKBUF           0xc0184d03u
#define NNIE_IOC_REMOVE_TSKBUF        0xc0184d04u

#define NNIE_FWD_ARG_SIZE       1624u
#define NNIE_FWD_BBOX_ARG_SIZE  1728u
#define NNIE_BLOB_SIZE            48u

static int      g_nnie_fd = -1;
static pthread_mutex_t g_nnie_fd_lock = PTHREAD_MUTEX_INITIALIZER;

static int nnie_open_dev(void)
{
	int fd;

	pthread_mutex_lock(&g_nnie_fd_lock);
	if (g_nnie_fd < 0) {
		fd = open("/dev/nnie", O_RDWR | O_CLOEXEC);
		if (fd < 0) {
			pthread_mutex_unlock(&g_nnie_fd_lock);
			return -errno;
		}
		g_nnie_fd = fd;
	}
	pthread_mutex_unlock(&g_nnie_fd_lock);
	return g_nnie_fd;
}

/* Translate Linux errno to vendor HI_ERR codes for upstream callers. */
static HI_S32 nnie_err_to_hi(int err)
{
	if (err >= 0) return HI_SUCCESS;
	switch (-err) {
	case EFAULT:    return HI_ERR_SVP_NNIE_BADADDR;
	case ENOMEM:    return HI_ERR_SVP_NNIE_NOMEM;
	case ENOSPC:    return HI_ERR_SVP_NNIE_NOBUF;
	case EINVAL:    return HI_ERR_SVP_NNIE_ILLEGAL_PARAM;
	case ENODEV:    return HI_ERR_SVP_NNIE_NOTREADY;
	case ETIMEDOUT: return HI_ERR_SVP_NNIE_BUSY;
	case EOPNOTSUPP: return HI_ERR_SVP_NNIE_NOT_SURPPORT;
#if defined(ENOTSUP) && ENOTSUP != EOPNOTSUPP
	case ENOTSUP:   return HI_ERR_SVP_NNIE_NOT_SURPPORT;
#endif
	default:        return HI_ERR_SVP_NNIE_NOT_SURPPORT;
	}
}

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
	uint32_t stored, computed = 0;    /* vendor init = 0, not ~0 */
	uint32_t inst_off, inst_len, crc_end;
	uint32_t i;

	if (file_size < NNIE_WK_MIN_FILE_SIZE)
		return -1;

	stored = (uint32_t)p[0] | ((uint32_t)p[1] << 8) |
	         ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);

	/* Vendor LoadModel covers bytes [4..file[52]+file[56]) — the header
	 * and the instruction-stream region pointed to by header fields
	 * inst_offset_extra (off 52) + inst_len (off 56). Anything after
	 * that (weights / quant tables) is *not* CRC-protected; vendor relies
	 * on the instruction stream itself to address those by file offset.
	 *
	 * Polynomial = standard IEEE 802.3 reflected (0xEDB88320), but the
	 * initial accumulator is 0, not 0xFFFFFFFF. Final XOR is 0xFFFFFFFF
	 * (the `mvn r0, r0` at libnnie.so 0x1d0c). Verified byte-equivalent
	 * against vendor mnist.wk: stored 0xa4a25b1a == computed. */
	inst_off = (uint32_t)p[52] | ((uint32_t)p[53] << 8) |
	           ((uint32_t)p[54] << 16) | ((uint32_t)p[55] << 24);
	inst_len = (uint32_t)p[56] | ((uint32_t)p[57] << 8) |
	           ((uint32_t)p[58] << 16) | ((uint32_t)p[59] << 24);
	crc_end = inst_off + inst_len;
	if (crc_end > file_size || crc_end < 4)
		return -1;

	for (i = 4; i < crc_end; i++)
		computed = nnie_wk_crc32_step(computed, p[i]);
	computed = ~computed;

	return (stored == computed) ? 0 : -1;
}

/* Parse just enough of the .wk file to satisfy the kernel Forward
 * ioctl: stBase (so kernel can DMA the instruction stream from MMZ),
 * u32NetSegNum, and astSeg[i].{enNetType, u16SrcNum, u16DstNum,
 * u16MaxStep, u32InstOffset, u32InstLen}. Node/ROI tables are left
 * zero-initialised; they're only needed by the userspace post-process
 * paths (vendor's softmax / detect / cluster helpers — not yet
 * implemented here). */
HI_S32 HI_MPI_SVP_NNIE_LoadModel(const SVP_SRC_MEM_INFO_S *pstModelBuf,
                                 SVP_NNIE_MODEL_S *pstModel)
{
	const uint8_t *file;
	uint32_t file_size, inst_off_extra, inst_total_len;
	uint32_t i;
	uint8_t  seg_num;

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

	seg_num = file[49];
	if (seg_num < 1 || seg_num > NNIE_WK_MAX_NET_SEG_NUM)
		return HI_ERR_SVP_NNIE_ILLEGAL_PARAM;

	inst_off_extra = *(const uint32_t *)(file + 52);
	inst_total_len = *(const uint32_t *)(file + 56);
	if ((uint64_t)inst_off_extra + inst_total_len > file_size)
		return HI_ERR_SVP_NNIE_ILLEGAL_PARAM;

	memset(pstModel, 0, sizeof(*pstModel));

	/* The kernel Forward ioctl DMAs the instruction stream from
	 * stBase.u64PhyAddr at the offset/length stored in
	 * astSeg[i].u32InstOffset/u32InstLen. So stBase points at the
	 * whole .wk file in MMZ — the per-segment offsets are file-
	 * relative. */
	pstModel->stBase    = *pstModelBuf;
	pstModel->enRunMode = (SVP_NNIE_RUN_MODE_E)file[48];
	pstModel->u32NetSegNum  = seg_num;
	pstModel->u32TmpBufSize = *(const uint32_t *)(file + 60);

	/* Segment table is at file[192]; each on-disk record is 16 B and
	 * decodes to the SVP_NNIE_SEG_S layout (see nnie_wk_format.h).
	 * Node + ROI tables follow but aren't decoded here — leaving
	 * astSrcNode/astDstNode/au32RoiIdx zeroed is safe because the
	 * kernel Forward path only touches u32InstOffset / u32InstLen. */
	for (i = 0; i < seg_num; i++) {
		const uint8_t *r = file + 192 + i * sizeof(nnie_wk_seg_record_t);

		pstModel->astSeg[i].enNetType     = (SVP_NNIE_NET_TYPE_E)r[0];
		pstModel->astSeg[i].u16SrcNum     = r[1];
		pstModel->astSeg[i].u16DstNum     = r[2];
		pstModel->astSeg[i].u16RoiPoolNum = r[3];
		pstModel->astSeg[i].u16MaxStep    = *(const uint16_t *)(r + 4);
		pstModel->astSeg[i].u32InstOffset = *(const uint32_t *)(r + 8);
		pstModel->astSeg[i].u32InstLen    = *(const uint32_t *)(r + 12);

		if (pstModel->astSeg[i].u32InstOffset + pstModel->astSeg[i].u32InstLen
		    > file_size)
			return HI_ERR_SVP_NNIE_ILLEGAL_PARAM;
	}

	return HI_SUCCESS;
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
	uint8_t buf[NNIE_FWD_ARG_SIZE];
	uint64_t model_uva;
	int fd, src_n, dst_n;

	if (!phSvpNnieHandle || !astSrc || !pstModel ||
	    !astDst || !pstForwardCtrl)
		return HI_ERR_SVP_NNIE_NULL_PTR;

	fd = nnie_open_dev();
	if (fd < 0)
		return nnie_err_to_hi(fd);

	memset(buf, 0, sizeof(buf));
	src_n = (int)pstForwardCtrl->u32SrcNum;
	dst_n = (int)pstForwardCtrl->u32DstNum;
	if (src_n < 1 || src_n > 16 || dst_n < 1 || dst_n > 16)
		return HI_ERR_SVP_NNIE_ILLEGAL_PARAM;

	memcpy(buf +    8, astSrc, NNIE_BLOB_SIZE * src_n);
	model_uva = (uint64_t)(uintptr_t)pstModel;
	memcpy(buf +  776, &model_uva, 8);
	memcpy(buf +  784, astDst, NNIE_BLOB_SIZE * dst_n);
	memcpy(buf + 1552, pstForwardCtrl, sizeof(*pstForwardCtrl));
	*(uint32_t *)(buf + 1616) = bInstant ? 1 : 0;

	if (ioctl(fd, NNIE_IOC_FORWARD, buf) < 0)
		return nnie_err_to_hi(-errno);

	*phSvpNnieHandle = *(uint32_t *)(buf + 0);
	return HI_SUCCESS;
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
	uint8_t arg[24];
	int fd;

	if (!pstTskBuf)
		return HI_ERR_SVP_NNIE_NULL_PTR;

	fd = nnie_open_dev();
	if (fd < 0)
		return nnie_err_to_hi(fd);

	memset(arg, 0, sizeof(arg));
	memcpy(arg, pstTskBuf, sizeof(*pstTskBuf));
	if (ioctl(fd, NNIE_IOC_ADD_TSKBUF, arg) < 0)
		return nnie_err_to_hi(-errno);
	return HI_SUCCESS;
}

HI_S32 HI_MPI_SVP_NNIE_RemoveTskBuf(const SVP_MEM_INFO_S *pstTskBuf)
{
	uint8_t arg[24];
	int fd;

	if (!pstTskBuf)
		return HI_ERR_SVP_NNIE_NULL_PTR;

	fd = nnie_open_dev();
	if (fd < 0)
		return nnie_err_to_hi(fd);

	memset(arg, 0, sizeof(arg));
	memcpy(arg, pstTskBuf, sizeof(*pstTskBuf));
	if (ioctl(fd, NNIE_IOC_REMOVE_TSKBUF, arg) < 0)
		return nnie_err_to_hi(-errno);
	return HI_SUCCESS;
}
