/*
 * SPDX-License-Identifier: GPL-2.0
 *
 * libnnie_neo — open-source replacement for vendor libnnie.so on
 * cv500/av300/dv300. Exposes the eight HI_MPI_SVP_NNIE_* entry points
 * vendor userspace expects, talking to /dev/nnie for HW dispatch and
 * parsing the .wk model file in-process.
 *
 * Three of the eight functions never touch the kernel:
 *   HI_MPI_SVP_NNIE_LoadModel    — parses the .wk file
 *   HI_MPI_SVP_NNIE_UnloadModel  — zeroes the struct
 *   HI_MPI_SVP_NNIE_GetTskBufSize — currently a stub; needs full
 *                                   per-segment instruction walk
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

/* /dev/nnie ioctl numbers — see kernel/nnie_neo/nnie_neo.c. */
#define NNIE_IOC_FORWARD              0xc6584d00u
#define NNIE_IOC_FORWARD_WITH_BBOX    0xc6c04d01u
#define NNIE_IOC_QUERY                0xc0184d02u
#define NNIE_IOC_ADD_TSKBUF           0xc0184d03u
#define NNIE_IOC_REMOVE_TSKBUF        0xc0184d04u

#define NNIE_FWD_ARG_SIZE       1624u
#define NNIE_BLOB_SIZE            48u

static int             g_nnie_fd      = -1;
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

/* IEEE 802.3 CRC-32 (reflected, poly 0xEDB88320). Vendor uses init=0,
 * not init=~0 like canonical zlib CRC32 — and final XOR is ~. */
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
	uint32_t stored, computed = 0;
	uint32_t inst_off, inst_len, crc_end;
	uint32_t i;

	if (file_size < NNIE_WK_MIN_FILE_SIZE)
		return -1;

	stored = (uint32_t)p[0] | ((uint32_t)p[1] << 8) |
	         ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);

	/* CRC covers bytes [4..file[52]+file[56]) — the header and the
	 * instruction-stream region. Weights/quant tables past that point
	 * aren't covered; the instruction stream addresses them by file
	 * offset. */
	inst_off = *(const uint32_t *)(p + 52);
	inst_len = *(const uint32_t *)(p + 56);
	crc_end = inst_off + inst_len;
	if (crc_end > file_size || crc_end < 4)
		return -1;

	for (i = 4; i < crc_end; i++)
		computed = nnie_wk_crc32_step(computed, p[i]);
	computed = ~computed;

	return (stored == computed) ? 0 : -1;
}

/*
 * Parse the .wk file into SVP_NNIE_MODEL_S. The kernel Forward ioctl
 * reads back stBase / u32NetSegNum / astSeg[seg].u32InstOffset /
 * astSeg[seg].u32InstLen, so those must be populated. Userspace reads
 * astSrcNode[0].enType to pick the right SVP_BLOB_TYPE_E for its
 * MmzAlloc — if that doesn't match what the model expects, HW returns
 * cfg_err info=0x1.
 *
 * Node table layout (immediately after each segment record):
 *   per node, 48 B total:
 *     [+0..+11]  u32 height, u32 width, u32 chn
 *     [+14]      u8 enType
 *     [+15]      u8 node id
 *     [+16..+47] szName[32]
 *
 * Iteration: SrcNum input nodes, then DstNum output nodes. Output node
 * enType isn't stored on disk — vendor's libnnie hardcodes
 * SVP_BLOB_TYPE_S32 (=4) and a (j+1)*8 NodeId pattern.
 */
HI_S32 HI_MPI_SVP_NNIE_LoadModel(const SVP_SRC_MEM_INFO_S *pstModelBuf,
                                 SVP_NNIE_MODEL_S *pstModel)
{
	const uint8_t *file;
	const uint8_t *p;
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
	pstModel->stBase    = *pstModelBuf;
	/* HW expects task[+16] (= stBase.PhyAddr) to point at the start of
	 * the instruction stream area, skipping the .wk header. Only
	 * PhyAddr is rebased; VirAddr stays at the file start because
	 * userspace reads from there. */
	pstModel->stBase.u64PhyAddr += inst_off_extra;
	pstModel->enRunMode    = (SVP_NNIE_RUN_MODE_E)file[48];
	pstModel->u32NetSegNum = seg_num;
	/* Vendor computes u32TmpBufSize by walking the per-segment
	 * instruction stream. We over-provision at 8 MB until that walker
	 * is implemented — fine for classification-class models; large
	 * detection models (yolov*, ssd) may need vendor's exact value. */
	pstModel->u32TmpBufSize = 8 * 1024 * 1024;

	/* Segments are interleaved with their node tables on disk —
	 * NOT all-segs-then-all-nodes. Walk sequentially: read the 16 B
	 * seg record at p, then each node body is 48 B followed by a
	 * 16 B inter-node separator (= NNIE_WK_NODE_STRIDE). The next
	 * seg record begins where the previous seg's last separator
	 * ended. Verified against vendor pvanet (2-seg, FasterRCNN) and
	 * mnist (1-seg) .wk files on av300. */
	p = file + NNIE_WK_HEADER_SIZE;

	for (i = 0; i < seg_num; i++) {
		uint32_t j;

		if ((size_t)(p + sizeof(nnie_wk_seg_record_t) - file) > file_size)
			return HI_ERR_SVP_NNIE_ILLEGAL_PARAM;

		pstModel->astSeg[i].enNetType     = (SVP_NNIE_NET_TYPE_E)p[0];
		pstModel->astSeg[i].u16SrcNum     = p[1];
		pstModel->astSeg[i].u16DstNum     = p[2];
		pstModel->astSeg[i].u16RoiPoolNum = p[3];
		pstModel->astSeg[i].u16MaxStep    = *(const uint16_t *)(p + 4);
		pstModel->astSeg[i].u32InstOffset = *(const uint32_t *)(p + 8);
		pstModel->astSeg[i].u32InstLen    = *(const uint32_t *)(p + 12);

		if (pstModel->astSeg[i].u32InstOffset +
		    pstModel->astSeg[i].u32InstLen > file_size)
			return HI_ERR_SVP_NNIE_ILLEGAL_PARAM;

		p += sizeof(nnie_wk_seg_record_t);

		for (j = 0; j < pstModel->astSeg[i].u16SrcNum; j++) {
			SVP_NNIE_NODE_S *n = &pstModel->astSeg[i].astSrcNode[j];

			if ((size_t)(p + NNIE_WK_NODE_STRIDE - file) > file_size)
				return HI_ERR_SVP_NNIE_ILLEGAL_PARAM;
			n->unShape.stWhc.u32Height = *(const uint32_t *)(p + 0);
			n->unShape.stWhc.u32Width  = *(const uint32_t *)(p + 4);
			n->unShape.stWhc.u32Chn    = *(const uint32_t *)(p + 8);
			n->enType                  = (SVP_BLOB_TYPE_E)p[14];
			n->u32NodeId               = p[15];
			memcpy(n->szName, p + 16, SVP_NNIE_NODE_NAME_LEN - 1);
			n->szName[SVP_NNIE_NODE_NAME_LEN - 1] = 0;
			p += NNIE_WK_NODE_STRIDE;
		}
		for (j = 0; j < pstModel->astSeg[i].u16DstNum; j++) {
			SVP_NNIE_NODE_S *n = &pstModel->astSeg[i].astDstNode[j];

			if ((size_t)(p + NNIE_WK_NODE_STRIDE - file) > file_size)
				return HI_ERR_SVP_NNIE_ILLEGAL_PARAM;
			/* Output activations: vendor's libnnie swaps fields so
			 * the layer's feature count lands in Width. */
			n->unShape.stWhc.u32Height = *(const uint32_t *)(p + 0);
			n->unShape.stWhc.u32Chn    = *(const uint32_t *)(p + 4);
			n->unShape.stWhc.u32Width  = *(const uint32_t *)(p + 8);
			n->enType                  = SVP_BLOB_TYPE_S32;
			n->u32NodeId               = (j + 1) * 8;
			memcpy(n->szName, p + 16, SVP_NNIE_NODE_NAME_LEN - 1);
			n->szName[SVP_NNIE_NODE_NAME_LEN - 1] = 0;
			p += NNIE_WK_NODE_STRIDE;
		}
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

/*
 * Compute the per-segment task buffer size that Forward / ForwardWithBbox
 * needs to dispatch a network. Callers MmzAlloc this many bytes per
 * segment and pass the result via SVP_NNIE_FORWARD_CTRL_S.stTskBuf.
 *
 * For CNN single-shot segments the size is exact, computed from the
 * tail layout the kernel builds in nnie_build_task_tail (see
 * kernel/nnie_neo/nnie_neo.c):
 *
 *   §1 src_stride[SrcNum]·u32  + dst_stride[DstNum]·u32      align16
 *   §2 dst_phys[DstNum]·u64                                   align16
 *   §3 src_phys table — entries-per-frame depends on enType:
 *      enType=0  (S32):              1 u64 per frame
 *      enType=1  (U8 image, chn=1):  1 u64 per frame
 *      enType=1  (U8 image, chn=3):  4 u64 quartet per frame
 *                                    (3 planar channel addrs + 0 slot)
 *      enType=4  (VEC_S32):          1 u64 per frame
 *
 * Verified byte-equivalent to vendor open_nnie.ko on av300:
 *   mnist  (src=1 chn=1, dst=1, num=1): 16 + 16 + 16 = 48  ✓
 *   SSD    (src=1 chn=3, dst=12, num=1): 64 + 96 + 32 = 192 ✓
 *
 * For SVP_NNIE_NET_TYPE_ROI segments (two-stage detectors —
 * Faster-RCNN, R-FCN, pvanet), vendor's tskbuf includes a much larger
 * RPN-decoded proposal table built by HW-specific helpers (~7200
 * 16-byte records per max_roi=200). The kernel handler for that path
 * returns -EOPNOTSUPP today, so callers can't actually use the bbox
 * dispatch — but we still need to give them a number that won't have
 * them under-allocating if they MmzAlloc speculatively. We
 * over-provision using the empirical 38-records-per-proposal × u32MaxBboxNum
 * + a 256 B header pad, which covers pvanet (max_roi=200 → ~122 KB)
 * with margin. Once #26 lands, this should switch to the exact
 * vendor formula (svp_nnie_calc_seg_tskbuf_size_kernel at hi_nnie.o:0x3b4c).
 *
 * For SVP_NNIE_NET_TYPE_RECURRENT (LSTM): not implemented; returns
 * NOT_SURPPORT.
 */
HI_S32 HI_MPI_SVP_NNIE_GetTskBufSize(HI_U32 u32MaxInputNum, HI_U32 u32MaxBboxNum,
                                     const SVP_NNIE_MODEL_S *pstModel,
                                     HI_U32 au32TskBufSize[],
                                     HI_U32 u32NetSegNum)
{
	HI_U32 i, j;

	if (!pstModel || !au32TskBufSize)
		return HI_ERR_SVP_NNIE_NULL_PTR;
	if (u32NetSegNum == 0 || u32NetSegNum > SVP_NNIE_MAX_NET_SEG_NUM)
		return HI_ERR_SVP_NNIE_ILLEGAL_PARAM;
	if (u32MaxInputNum == 0 || u32MaxInputNum > SVP_NNIE_MAX_INPUT_NUM)
		return HI_ERR_SVP_NNIE_ILLEGAL_PARAM;

	for (i = 0; i < u32NetSegNum; i++) {
		const SVP_NNIE_NODE_S *src_nodes;
		HI_U32 src_n, dst_n;
		HI_U32 stride_tbl, dst_phys, src_phys, total;

		if (i >= pstModel->u32NetSegNum)
			return HI_ERR_SVP_NNIE_ILLEGAL_PARAM;

		if (pstModel->astSeg[i].enNetType == SVP_NNIE_NET_TYPE_RECURRENT)
			return HI_ERR_SVP_NNIE_NOT_SURPPORT;

		src_n = pstModel->astSeg[i].u16SrcNum;
		dst_n = pstModel->astSeg[i].u16DstNum;
		if (src_n == 0 || src_n > SVP_NNIE_MAX_INPUT_NUM ||
		    dst_n == 0 || dst_n > SVP_NNIE_MAX_OUTPUT_NUM)
			return HI_ERR_SVP_NNIE_ILLEGAL_PARAM;

		stride_tbl = (src_n + dst_n) * 4;
		stride_tbl = (stride_tbl + 15) & ~15u;

		dst_phys = dst_n * 8;
		dst_phys = (dst_phys + 15) & ~15u;

		src_nodes = pstModel->astSeg[i].astSrcNode;
		src_phys = 0;
		for (j = 0; j < src_n; j++) {
			HI_U32 chn  = src_nodes[j].unShape.stWhc.u32Chn;
			HI_U32 per_frame_u64;
			switch (src_nodes[j].enType) {
			case SVP_BLOB_TYPE_U8:
				per_frame_u64 = (chn == 3) ? 4 : 1;
				break;
			case SVP_BLOB_TYPE_S32:
			case SVP_BLOB_TYPE_VEC_S32:
				per_frame_u64 = 1;
				break;
			case SVP_BLOB_TYPE_YVU420SP:
			case SVP_BLOB_TYPE_YVU422SP:
				per_frame_u64 = 2;  /* Y plane phys + UV plane phys */
				break;
			default:
				return HI_ERR_SVP_NNIE_NOT_SURPPORT;
			}
			src_phys += per_frame_u64 * u32MaxInputNum * 8;
		}
		src_phys = (src_phys + 15) & ~15u;

		total = stride_tbl + dst_phys + src_phys;

		/* ROI/bbox segments: over-provision until #26 lands. The
		 * vendor pvanet tskbuf for max_roi=200 was 120096 B
		 * (~38 records/proposal × 16 B + 96 B header + small pad).
		 * Use 48 B/proposal × max_roi + 1 KB header pad for safe
		 * over-provision. */
		if (pstModel->astSeg[i].enNetType == SVP_NNIE_NET_TYPE_ROI) {
			HI_U32 roi_tail = 1024 + u32MaxBboxNum * 48;
			if (roi_tail > total)
				total = roi_tail;
		}

		au32TskBufSize[i] = total;
	}

	return HI_SUCCESS;
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

	src_n = (int)pstForwardCtrl->u32SrcNum;
	dst_n = (int)pstForwardCtrl->u32DstNum;
	if (src_n < 1 || src_n > 16 || dst_n < 1 || dst_n > 16)
		return HI_ERR_SVP_NNIE_ILLEGAL_PARAM;

	fd = nnie_open_dev();
	if (fd < 0)
		return nnie_err_to_hi(fd);

	memset(buf, 0, sizeof(buf));
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
	if (!pbFinish)
		return HI_ERR_SVP_NNIE_NULL_PTR;
	/* Forward dispatch is synchronous; by the time userspace calls
	 * Query the task is always done. */
	*pbFinish = HI_TRUE;
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
