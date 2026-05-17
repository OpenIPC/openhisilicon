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
#define NNIE_FWD_BBOX_ARG_SIZE  1728u
#define NNIE_BLOB_SIZE            48u

/* ForwardWithBbox user buffer offsets (kernel-side mirror in
 * kernel/nnie_neo/nnie_neo.c). The bbox CTRL_S inserts u32ProposalNum
 * at +8, shifting NetSegId and the SVP_MEM_INFO_S members by 8 B
 * compared to plain Forward CTRL_S — see
 * `hiSVP_NNIE_FORWARD_WITHBBOX_CTRL_S` in hi_nnie.h. */
#define NNIE_BBOX_OFF_HANDLE         0
#define NNIE_BBOX_OFF_ASTSRC         8
#define NNIE_BBOX_OFF_ASTBBOX      776
#define NNIE_BBOX_OFF_PSTMODEL     872
#define NNIE_BBOX_OFF_ASTDST       880
#define NNIE_BBOX_OFF_CTRL        1648
#define NNIE_BBOX_OFF_INSTANT     1720

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

	p = file + NNIE_WK_HEADER_SIZE + (size_t)seg_num * sizeof(nnie_wk_seg_record_t);

	for (i = 0; i < seg_num; i++) {
		const uint8_t *r = file + NNIE_WK_HEADER_SIZE
		                   + i * sizeof(nnie_wk_seg_record_t);
		uint32_t j;

		pstModel->astSeg[i].enNetType     = (SVP_NNIE_NET_TYPE_E)r[0];
		pstModel->astSeg[i].u16SrcNum     = r[1];
		pstModel->astSeg[i].u16DstNum     = r[2];
		pstModel->astSeg[i].u16RoiPoolNum = r[3];
		pstModel->astSeg[i].u16MaxStep    = *(const uint16_t *)(r + 4);
		pstModel->astSeg[i].u32InstOffset = *(const uint32_t *)(r + 8);
		pstModel->astSeg[i].u32InstLen    = *(const uint32_t *)(r + 12);

		if (pstModel->astSeg[i].u32InstOffset +
		    pstModel->astSeg[i].u32InstLen > file_size)
			return HI_ERR_SVP_NNIE_ILLEGAL_PARAM;

		for (j = 0; j < pstModel->astSeg[i].u16SrcNum; j++) {
			SVP_NNIE_NODE_S *n = &pstModel->astSeg[i].astSrcNode[j];

			if ((size_t)(p + NNIE_WK_NODE_SIZE - file) > file_size)
				return HI_ERR_SVP_NNIE_ILLEGAL_PARAM;
			n->unShape.stWhc.u32Height = *(const uint32_t *)(p + 0);
			n->unShape.stWhc.u32Width  = *(const uint32_t *)(p + 4);
			n->unShape.stWhc.u32Chn    = *(const uint32_t *)(p + 8);
			n->enType                  = (SVP_BLOB_TYPE_E)p[14];
			n->u32NodeId               = p[15];
			memcpy(n->szName, p + 16, SVP_NNIE_NODE_NAME_LEN - 1);
			n->szName[SVP_NNIE_NODE_NAME_LEN - 1] = 0;
			p += NNIE_WK_NODE_SIZE;
		}
		for (j = 0; j < pstModel->astSeg[i].u16DstNum; j++) {
			SVP_NNIE_NODE_S *n = &pstModel->astSeg[i].astDstNode[j];

			if ((size_t)(p + NNIE_WK_NODE_SIZE - file) > file_size)
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
			p += NNIE_WK_NODE_SIZE;
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

HI_S32 HI_MPI_SVP_NNIE_GetTskBufSize(HI_U32 u32MaxInputNum, HI_U32 u32MaxBboxNum,
                                     const SVP_NNIE_MODEL_S *pstModel,
                                     HI_U32 au32TskBufSize[],
                                     HI_U32 u32NetSegNum)
{
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
	uint8_t buf[NNIE_FWD_BBOX_ARG_SIZE];
	uint64_t model_uva;
	int fd, src_n, dst_n, bbox_n;

	if (!phSvpNnieHandle || !astSrc || !pstModel ||
	    !astDst || !pstForwardCtrl)
		return HI_ERR_SVP_NNIE_NULL_PTR;

	src_n  = (int)pstForwardCtrl->u32SrcNum;
	dst_n  = (int)pstForwardCtrl->u32DstNum;
	bbox_n = (int)pstForwardCtrl->u32ProposalNum;
	if (src_n < 1 || src_n > 16 || dst_n < 1 || dst_n > 16 ||
	    bbox_n < 0 || bbox_n > 2)
		return HI_ERR_SVP_NNIE_ILLEGAL_PARAM;
	if (bbox_n > 0 && !astBbox)
		return HI_ERR_SVP_NNIE_NULL_PTR;

	fd = nnie_open_dev();
	if (fd < 0)
		return nnie_err_to_hi(fd);

	memset(buf, 0, sizeof(buf));
	memcpy(buf + NNIE_BBOX_OFF_ASTSRC,   astSrc,  NNIE_BLOB_SIZE * src_n);
	if (bbox_n)
		memcpy(buf + NNIE_BBOX_OFF_ASTBBOX, astBbox, NNIE_BLOB_SIZE * bbox_n);
	model_uva = (uint64_t)(uintptr_t)pstModel;
	memcpy(buf + NNIE_BBOX_OFF_PSTMODEL, &model_uva, 8);
	memcpy(buf + NNIE_BBOX_OFF_ASTDST,   astDst,  NNIE_BLOB_SIZE * dst_n);
	memcpy(buf + NNIE_BBOX_OFF_CTRL, pstForwardCtrl, sizeof(*pstForwardCtrl));
	*(uint32_t *)(buf + NNIE_BBOX_OFF_INSTANT) = bInstant ? 1 : 0;

	if (ioctl(fd, NNIE_IOC_FORWARD_WITH_BBOX, buf) < 0)
		return nnie_err_to_hi(-errno);

	*phSvpNnieHandle = *(uint32_t *)(buf + NNIE_BBOX_OFF_HANDLE);
	return HI_SUCCESS;
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
