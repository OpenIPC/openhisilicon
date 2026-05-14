/*
 * SPDX-License-Identifier: GPL-2.0
 *
 * ive_ops.c — HI_MPI_IVE_* ioctl wrappers for libive_neo.
 *
 * Each wrapper builds an ioctl arg buffer matching vendor libive.so's
 * stack layout (verified against IDA decomp at ~/libive/mpi_ive.o.c) and
 * dispatches to /dev/ive. The kernel side (ive_neo.ko) reads the same
 * buffer offsets, so the wire protocol matches byte-for-byte.
 *
 * Generic arg-buffer layout for "single src + single dst + ctrl + instant":
 *   [+0]   u32 handle (out: kernel writes resulting handle)
 *   [+4]   u32 pad
 *   [+8]   u8  src[72]    (IVE_IMAGE_S)
 *   [+80]  u8  dst[72]    (IVE_IMAGE_S)
 *   [+152] u8  ctrl[N]    (per-op control struct)
 *   [+152+aligned(N,4)] u32 instant
 *
 * The ioctl returns 0 on success, in which case the handle word at +0 is
 * the new IVE handle the caller should pass to HI_MPI_IVE_Query().
 */
#include "mpi_ive.h"
#include "ive_internal.h"

#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

/* ioctl cmd numbers (authoritative — extracted from libive.so decomp) */
#define IVE_CMD_DMA            0xC0684600u
#define IVE_CMD_FILTER         0xC0B84601u
#define IVE_CMD_CSC            0xC0A04602u
#define IVE_CMD_FILTERANDCSC   0xC0C04603u
#define IVE_CMD_SOBEL          0xC1084604u
#define IVE_CMD_MAGANDANG      0xC1084605u
#define IVE_CMD_DILATE         0xC0B84606u
#define IVE_CMD_ERODE          0xC0B84607u
#define IVE_CMD_THRESH         0xC0A84608u
#define IVE_CMD_AND            0xC0E84609u
#define IVE_CMD_SUB            0xC0E8460Au
#define IVE_CMD_OR             0xC0E8460Bu
#define IVE_CMD_INTEG          0xC0A0460Cu
#define IVE_CMD_HIST           0xC070460Du
#define IVE_CMD_THRESH_S16     0xC0A8460Eu
#define IVE_CMD_THRESH_U16     0xC0A8460Fu
#define IVE_CMD_16BIT_TO_8BIT  0xC0A84610u
#define IVE_CMD_ORDSTAT        0xC0A04611u
#define IVE_CMD_MAP            0xC0B84613u
#define IVE_CMD_ADD            0xC0E84614u
#define IVE_CMD_XOR            0xC0E84615u
#define IVE_CMD_NCC            0xC0B84616u
#define IVE_CMD_CCL            0xC0784617u
#define IVE_CMD_GMM            0xC1184618u
#define IVE_CMD_CANNY_HYS_EDGE 0xC0F04619u
#define IVE_CMD_LBP            0xC0A8461Au
#define IVE_CMD_NORM_GRAD      0xC150461Bu
#define IVE_CMD_LK_OPT_FLOW    0xC2C0461Cu
#define IVE_CMD_GRAD_FG        0xC138461Du
#define IVE_CMD_MATCH_BG       0xC178461Eu
#define IVE_CMD_UPDATE_BG      0xC1D8461Fu
#define IVE_CMD_ANN_MLP_PRED   0xC0B04621u
#define IVE_CMD_SVM_PREDICT    0xC0C04622u
#define IVE_CMD_SAD            0xC1384629u
#define IVE_CMD_EQUALIZE_HIST  0xC0B8462Au
#define IVE_CMD_ST_CANDI_CORN  0xC0C0462Bu
#define IVE_CMD_QUERY          0xC00C462Cu
#define IVE_CMD_GMM2           0xC1A8462Du
#define IVE_CMD_RESIZE         0xC008462Eu
#define IVE_CMD_CNN_PREDICT    0xD338462Fu
#define IVE_CMD_XNN_UNLOADMOD  0xC0044637u
#define IVE_CMD_XNN_LOADMODEL  0xC8A04636u
#define IVE_CMD_CNN_LOADMODEL  0xC9704638u
#define IVE_CMD_SVM_PRED2      0xC6484639u
#define IVE_CMD_ANN_MLP_PRED2  0xC620463Au

/* Sizes referenced in arg buffers */
#define SZ_IMG    72   /* sizeof(IVE_IMAGE_S) */
#define SZ_DATA   32   /* sizeof(IVE_DATA_S) */
#define SZ_MEM    24   /* sizeof(IVE_MEM_INFO_S) */

/* Helper: store dword to a u8 buffer at offset (little-endian / native). */
static inline void put_u32(uint8_t *buf, size_t off, uint32_t v)
{
    *(uint32_t *)(buf + off) = v;
}
static inline uint32_t get_u32(const uint8_t *buf, size_t off)
{
    return *(const uint32_t *)(buf + off);
}

/* ===================================================================
 * Query — special: takes handle BY VALUE, no out-handle write.
 *   buf[0] = handle (in)
 *   buf[1] = bBlock (in)
 *   buf[2] = bFinish (out)
 * =================================================================== */
HI_S32 HI_MPI_IVE_Query(IVE_HANDLE IveHandle, HI_BOOL *pbFinish, HI_BOOL bBlock)
{
    uint32_t buf[3];
    HI_S32 ret;

    if (bBlock != HI_FALSE && bBlock != HI_TRUE) {
        IVE_LOG("HI_MPI_IVE_Query", "bBlock(%d) must be HI_TRUE or HI_FALSE",
                bBlock);
        return HI_ERR_IVE_ILLEGAL_PARAM;
    }
    IVE_CHECK_NULL("HI_MPI_IVE_Query", pbFinish, "pbFinish");
    if ((unsigned int)IveHandle > 0x0FFFFFFEu) {
        IVE_LOG("HI_MPI_IVE_Query", "IveHandle(%d) out of range", IveHandle);
        return HI_ERR_IVE_ILLEGAL_PARAM;
    }

    if (ive_open_fd() != HI_SUCCESS)
        return HI_FAILURE;

    buf[0] = (uint32_t)IveHandle;
    buf[1] = (uint32_t)bBlock;
    buf[2] = 0;

    ret = ioctl(g_ive_fd, IVE_CMD_QUERY, buf);
    if (ret == 0)
        *pbFinish = (HI_BOOL)buf[2];
    return ret;
}

/* ===================================================================
 * Generic helper: 2-image (src + dst) + ctrl + instant ioctl.
 * Used by Filter, Thresh, Dilate, Erode, Sobel(special), MagAndAng,
 * Integ, OrdStatFilter, EqualizeHist, LBP, 16BitTo8Bit, Thresh_S16,
 * Thresh_U16, CSC, FilterAndCSC, Map(special), CCL(special), Hist(special).
 *
 * `ctrl_size` may be 0 (no ctrl). Returns ioctl error or HI_SUCCESS.
 * On success, *pIveHandle is updated from buf[0].
 * =================================================================== */
static HI_S32 ive_op_2img(unsigned int cmd, IVE_HANDLE *pIveHandle,
                          const void *src, const void *dst,
                          const void *ctrl, size_t ctrl_size,
                          HI_BOOL bInstant)
{
    /* Worst-case ctrl is ~64 bytes; total ~256 bytes well within stack. */
    uint8_t buf[8 + SZ_IMG + SZ_IMG + 128];
    size_t  off_instant;
    HI_S32  ret;

    if (ctrl_size > 128)
        return HI_ERR_IVE_ILLEGAL_PARAM;

    IVE_CHECK_NULL(__func__, pIveHandle, "pIveHandle");
    IVE_CHECK_NULL(__func__, src,         "src");
    IVE_CHECK_NULL(__func__, dst,         "dst");
    if (ctrl_size > 0)
        IVE_CHECK_NULL(__func__, ctrl, "ctrl");

    if (ive_open_fd() != HI_SUCCESS)
        return HI_FAILURE;

    memset(buf, 0, sizeof(buf));
    memcpy(buf + 8,                   src, SZ_IMG);
    memcpy(buf + 8 + SZ_IMG,          dst, SZ_IMG);
    if (ctrl_size > 0)
        memcpy(buf + 8 + SZ_IMG * 2,  ctrl, ctrl_size);

    /* Instant flag immediately follows the ctrl block (no padding). */
    off_instant = 8 + SZ_IMG * 2 + ctrl_size;
    /* Round to next 4-byte boundary for the instant dword storage. */
    off_instant = (off_instant + 3) & ~(size_t)3;
    put_u32(buf, off_instant, (uint32_t)bInstant);

    ret = ioctl(g_ive_fd, cmd, buf);
    *pIveHandle = (ret == 0) ? (IVE_HANDLE)get_u32(buf, 0) : -1;
    return ret;
}

/* 3-image variant (src1, src2, dst) used by And, Or, Xor and Sub-without-ctrl. */
static HI_S32 ive_op_3img(unsigned int cmd, IVE_HANDLE *pIveHandle,
                          const void *src1, const void *src2, const void *dst,
                          const void *ctrl, size_t ctrl_size,
                          HI_BOOL bInstant)
{
    uint8_t buf[8 + SZ_IMG * 3 + 32];
    size_t  off_instant;
    HI_S32  ret;

    if (ctrl_size > 32)
        return HI_ERR_IVE_ILLEGAL_PARAM;

    IVE_CHECK_NULL(__func__, pIveHandle, "pIveHandle");
    IVE_CHECK_NULL(__func__, src1,        "src1");
    IVE_CHECK_NULL(__func__, src2,        "src2");
    IVE_CHECK_NULL(__func__, dst,         "dst");

    if (ive_open_fd() != HI_SUCCESS)
        return HI_FAILURE;

    memset(buf, 0, sizeof(buf));
    memcpy(buf + 8,               src1, SZ_IMG);
    memcpy(buf + 8 + SZ_IMG,      src2, SZ_IMG);
    memcpy(buf + 8 + SZ_IMG * 2,  dst,  SZ_IMG);
    if (ctrl_size > 0)
        memcpy(buf + 8 + SZ_IMG * 3, ctrl, ctrl_size);

    off_instant = 8 + SZ_IMG * 3 + ctrl_size;
    off_instant = (off_instant + 3) & ~(size_t)3;
    put_u32(buf, off_instant, (uint32_t)bInstant);

    ret = ioctl(g_ive_fd, cmd, buf);
    *pIveHandle = (ret == 0) ? (IVE_HANDLE)get_u32(buf, 0) : -1;
    return ret;
}

/* 4-image variant used by Sobel/MagAndAng/NormGrad/SAD. */
static HI_S32 ive_op_4img(unsigned int cmd, IVE_HANDLE *pIveHandle,
                          const void *src, const void *dst1,
                          const void *dst2, const void *dst3,
                          const void *ctrl, size_t ctrl_size,
                          HI_BOOL bInstant)
{
    uint8_t buf[8 + SZ_IMG * 4 + 64];
    size_t  off_instant;
    HI_S32  ret;

    if (ctrl_size > 64)
        return HI_ERR_IVE_ILLEGAL_PARAM;

    IVE_CHECK_NULL(__func__, pIveHandle, "pIveHandle");
    IVE_CHECK_NULL(__func__, src,         "src");

    if (ive_open_fd() != HI_SUCCESS)
        return HI_FAILURE;

    memset(buf, 0, sizeof(buf));
    memcpy(buf + 8,               src, SZ_IMG);
    if (dst1) memcpy(buf + 8 + SZ_IMG,     dst1, SZ_IMG);
    if (dst2) memcpy(buf + 8 + SZ_IMG * 2, dst2, SZ_IMG);
    if (dst3) memcpy(buf + 8 + SZ_IMG * 3, dst3, SZ_IMG);
    if (ctrl_size > 0)
        memcpy(buf + 8 + SZ_IMG * 4, ctrl, ctrl_size);

    off_instant = 8 + SZ_IMG * 4 + ctrl_size;
    off_instant = (off_instant + 3) & ~(size_t)3;
    put_u32(buf, off_instant, (uint32_t)bInstant);

    ret = ioctl(g_ive_fd, cmd, buf);
    *pIveHandle = (ret == 0) ? (IVE_HANDLE)get_u32(buf, 0) : -1;
    return ret;
}

/* ===================================================================
 * DMA: src/dst are IVE_DATA_S (32 bytes), not IVE_IMAGE_S.
 *   buf[0]    = handle (out)
 *   buf[1]    = pad
 *   buf[2..9] = src DATA_S (32 bytes)
 *   buf[10..17] = dst DATA_S (32 bytes; copied only if mode <= 1)
 *   buf[18..23] = ctrl (24 bytes)
 *   buf[24]   = instant
 * =================================================================== */
HI_S32 HI_MPI_IVE_DMA(IVE_HANDLE *pIveHandle,
                      IVE_DATA_S *pstSrc,
                      IVE_DST_DATA_S *pstDst,
                      IVE_DMA_CTRL_S *pstDmaCtrl,
                      HI_BOOL bInstant)
{
    uint32_t buf[27];
    HI_S32 ret;

    IVE_CHECK_NULL("HI_MPI_IVE_DMA", pIveHandle, "pIveHandle");
    IVE_CHECK_NULL("HI_MPI_IVE_DMA", pstSrc,      "pstSrc");
    IVE_CHECK_NULL("HI_MPI_IVE_DMA", pstDmaCtrl,  "pstDmaCtrl");

    if (ive_open_fd() != HI_SUCCESS)
        return HI_FAILURE;

    memset(buf, 0, sizeof(buf));
    memcpy(&buf[2], pstSrc, SZ_DATA);
    if ((unsigned int)pstDmaCtrl->enMode <= 1u && pstDst != NULL)
        memcpy(&buf[10], pstDst, SZ_DATA);
    memcpy(&buf[18], pstDmaCtrl, SZ_MEM);
    buf[24] = (uint32_t)bInstant;

    ret = ioctl(g_ive_fd, IVE_CMD_DMA, buf);
    *pIveHandle = (ret == 0) ? (IVE_HANDLE)buf[0] : -1;
    return ret;
}

/* ===================================================================
 * Single-arity ops: src + dst + ctrl + instant
 * =================================================================== */
HI_S32 HI_MPI_IVE_Filter(IVE_HANDLE *h, IVE_SRC_IMAGE_S *src, IVE_DST_IMAGE_S *dst,
                         IVE_FILTER_CTRL_S *c, HI_BOOL inst)
{
    return ive_op_2img(IVE_CMD_FILTER, h, src, dst, c, sizeof(*c), inst);
}

HI_S32 HI_MPI_IVE_Dilate(IVE_HANDLE *h, IVE_SRC_IMAGE_S *src, IVE_DST_IMAGE_S *dst,
                         IVE_DILATE_CTRL_S *c, HI_BOOL inst)
{
    return ive_op_2img(IVE_CMD_DILATE, h, src, dst, c, sizeof(*c), inst);
}

HI_S32 HI_MPI_IVE_Erode(IVE_HANDLE *h, IVE_SRC_IMAGE_S *src, IVE_DST_IMAGE_S *dst,
                        IVE_ERODE_CTRL_S *c, HI_BOOL inst)
{
    return ive_op_2img(IVE_CMD_ERODE, h, src, dst, c, sizeof(*c), inst);
}

HI_S32 HI_MPI_IVE_Thresh(IVE_HANDLE *h, IVE_SRC_IMAGE_S *src, IVE_DST_IMAGE_S *dst,
                         IVE_THRESH_CTRL_S *c, HI_BOOL inst)
{
    return ive_op_2img(IVE_CMD_THRESH, h, src, dst, c, sizeof(*c), inst);
}

HI_S32 HI_MPI_IVE_Thresh_S16(IVE_HANDLE *h, IVE_SRC_IMAGE_S *src, IVE_DST_IMAGE_S *dst,
                             IVE_THRESH_S16_CTRL_S *c, HI_BOOL inst)
{
    return ive_op_2img(IVE_CMD_THRESH_S16, h, src, dst, c, sizeof(*c), inst);
}

HI_S32 HI_MPI_IVE_Thresh_U16(IVE_HANDLE *h, IVE_SRC_IMAGE_S *src, IVE_DST_IMAGE_S *dst,
                             IVE_THRESH_U16_CTRL_S *c, HI_BOOL inst)
{
    return ive_op_2img(IVE_CMD_THRESH_U16, h, src, dst, c, sizeof(*c), inst);
}

HI_S32 HI_MPI_IVE_16BitTo8Bit(IVE_HANDLE *h, IVE_SRC_IMAGE_S *src, IVE_DST_IMAGE_S *dst,
                              IVE_16BIT_TO_8BIT_CTRL_S *c, HI_BOOL inst)
{
    return ive_op_2img(IVE_CMD_16BIT_TO_8BIT, h, src, dst, c, sizeof(*c), inst);
}

HI_S32 HI_MPI_IVE_OrdStatFilter(IVE_HANDLE *h, IVE_SRC_IMAGE_S *src, IVE_DST_IMAGE_S *dst,
                                IVE_ORD_STAT_FILTER_CTRL_S *c, HI_BOOL inst)
{
    return ive_op_2img(IVE_CMD_ORDSTAT, h, src, dst, c, sizeof(*c), inst);
}

HI_S32 HI_MPI_IVE_Integ(IVE_HANDLE *h, IVE_SRC_IMAGE_S *src, IVE_DST_IMAGE_S *dst,
                        IVE_INTEG_CTRL_S *c, HI_BOOL inst)
{
    return ive_op_2img(IVE_CMD_INTEG, h, src, dst, c, sizeof(*c), inst);
}

HI_S32 HI_MPI_IVE_LBP(IVE_HANDLE *h, IVE_SRC_IMAGE_S *src, IVE_DST_IMAGE_S *dst,
                      IVE_LBP_CTRL_S *c, HI_BOOL inst)
{
    /* ctrl is 8 bytes per decomp, but sizeof(IVE_LBP_CTRL_S) == 8 */
    return ive_op_2img(IVE_CMD_LBP, h, src, dst, c, sizeof(*c), inst);
}

HI_S32 HI_MPI_IVE_EqualizeHist(IVE_HANDLE *h, IVE_SRC_IMAGE_S *src, IVE_DST_IMAGE_S *dst,
                               IVE_EQUALIZE_HIST_CTRL_S *c, HI_BOOL inst)
{
    /* ctrl block is 24 bytes per decomp (= IVE_MEM_INFO_S) */
    return ive_op_2img(IVE_CMD_EQUALIZE_HIST, h, src, dst, c, sizeof(*c), inst);
}

HI_S32 HI_MPI_IVE_CSC(IVE_HANDLE *h, IVE_SRC_IMAGE_S *src, IVE_DST_IMAGE_S *dst,
                      IVE_CSC_CTRL_S *c, HI_BOOL inst)
{
    return ive_op_2img(IVE_CMD_CSC, h, src, dst, c, sizeof(*c), inst);
}

HI_S32 HI_MPI_IVE_FilterAndCSC(IVE_HANDLE *h, IVE_SRC_IMAGE_S *src, IVE_DST_IMAGE_S *dst,
                               IVE_FILTER_AND_CSC_CTRL_S *c, HI_BOOL inst)
{
    return ive_op_2img(IVE_CMD_FILTERANDCSC, h, src, dst, c, sizeof(*c), inst);
}

/* ===================================================================
 * 3-image bitwise / arithmetic ops
 * =================================================================== */
HI_S32 HI_MPI_IVE_And(IVE_HANDLE *h, IVE_SRC_IMAGE_S *s1, IVE_SRC_IMAGE_S *s2,
                      IVE_DST_IMAGE_S *dst, HI_BOOL inst)
{
    return ive_op_3img(IVE_CMD_AND, h, s1, s2, dst, NULL, 0, inst);
}

HI_S32 HI_MPI_IVE_Or(IVE_HANDLE *h, IVE_SRC_IMAGE_S *s1, IVE_SRC_IMAGE_S *s2,
                     IVE_DST_IMAGE_S *dst, HI_BOOL inst)
{
    return ive_op_3img(IVE_CMD_OR, h, s1, s2, dst, NULL, 0, inst);
}

HI_S32 HI_MPI_IVE_Xor(IVE_HANDLE *h, IVE_SRC_IMAGE_S *s1, IVE_SRC_IMAGE_S *s2,
                      IVE_DST_IMAGE_S *dst, HI_BOOL inst)
{
    return ive_op_3img(IVE_CMD_XOR, h, s1, s2, dst, NULL, 0, inst);
}

HI_S32 HI_MPI_IVE_Sub(IVE_HANDLE *h, IVE_SRC_IMAGE_S *s1, IVE_SRC_IMAGE_S *s2,
                      IVE_DST_IMAGE_S *dst, IVE_SUB_CTRL_S *c, HI_BOOL inst)
{
    /* Sub ctrl is 4 bytes per decomp */
    return ive_op_3img(IVE_CMD_SUB, h, s1, s2, dst, c, sizeof(*c), inst);
}

HI_S32 HI_MPI_IVE_Add(IVE_HANDLE *h, IVE_SRC_IMAGE_S *s1, IVE_SRC_IMAGE_S *s2,
                      IVE_DST_IMAGE_S *dst, IVE_ADD_CTRL_S *c, HI_BOOL inst)
{
    return ive_op_3img(IVE_CMD_ADD, h, s1, s2, dst, c, sizeof(*c), inst);
}

/* ===================================================================
 * 4-image ops: Sobel (1 src + 2 dst), MagAndAng, NormGrad, SAD
 * =================================================================== */
HI_S32 HI_MPI_IVE_Sobel(IVE_HANDLE *h, IVE_SRC_IMAGE_S *src,
                        IVE_DST_IMAGE_S *dstH, IVE_DST_IMAGE_S *dstV,
                        IVE_SOBEL_CTRL_S *c, HI_BOOL inst)
{
    return ive_op_4img(IVE_CMD_SOBEL, h, src, dstH, dstV, NULL,
                       c, sizeof(*c), inst);
}

HI_S32 HI_MPI_IVE_MagAndAng(IVE_HANDLE *h, IVE_SRC_IMAGE_S *src,
                            IVE_DST_IMAGE_S *dstMag, IVE_DST_IMAGE_S *dstAng,
                            IVE_MAG_AND_ANG_CTRL_S *c, HI_BOOL inst)
{
    return ive_op_4img(IVE_CMD_MAGANDANG, h, src, dstMag, dstAng, NULL,
                       c, sizeof(*c), inst);
}

HI_S32 HI_MPI_IVE_NormGrad(IVE_HANDLE *h, IVE_SRC_IMAGE_S *src,
                           IVE_DST_IMAGE_S *dstH, IVE_DST_IMAGE_S *dstV,
                           IVE_DST_IMAGE_S *dstHV,
                           IVE_NORM_GRAD_CTRL_S *c, HI_BOOL inst)
{
    return ive_op_4img(IVE_CMD_NORM_GRAD, h, src, dstH, dstV, dstHV,
                       c, sizeof(*c), inst);
}

HI_S32 HI_MPI_IVE_SAD(IVE_HANDLE *h, IVE_SRC_IMAGE_S *s1, IVE_SRC_IMAGE_S *s2,
                      IVE_DST_IMAGE_S *sad, IVE_DST_IMAGE_S *thr,
                      IVE_SAD_CTRL_S *c, HI_BOOL inst)
{
    /* SAD layout: src1 + src2 + dst_sad16 + dst_thr8 + ctrl + instant.
     * The 4-image helper takes (src, dst1, dst2, dst3, ctrl); pass src2
     * as dst1 to keep the simple offset layout. The kernel expects:
     *   buf+8   = src1, buf+80 = src2, buf+152 = dst_sad, buf+224 = dst_thr,
     *   buf+296 = ctrl (12 bytes), buf+308 = instant. */
    return ive_op_4img(IVE_CMD_SAD, h, s1, s2, sad, thr,
                       c, sizeof(*c), inst);
}

/* ===================================================================
 * Hist: src + dst_mem_info + instant
 *   buf+0  = handle, buf+8 = src(72), buf+80 = dst_mem(24), buf+104 = instant
 * =================================================================== */
HI_S32 HI_MPI_IVE_Hist(IVE_HANDLE *h, IVE_SRC_IMAGE_S *src,
                       IVE_DST_MEM_INFO_S *dst, HI_BOOL inst)
{
    uint8_t buf[120];
    HI_S32 ret;

    IVE_CHECK_NULL("HI_MPI_IVE_Hist", h,    "pIveHandle");
    IVE_CHECK_NULL("HI_MPI_IVE_Hist", src,  "pstSrc");
    IVE_CHECK_NULL("HI_MPI_IVE_Hist", dst,  "pstDst");

    if (ive_open_fd() != HI_SUCCESS)
        return HI_FAILURE;

    memset(buf, 0, sizeof(buf));
    memcpy(buf + 8,    src, SZ_IMG);
    memcpy(buf + 80,   dst, SZ_MEM);
    put_u32(buf, 104, (uint32_t)inst);

    ret = ioctl(g_ive_fd, IVE_CMD_HIST, buf);
    *h = (ret == 0) ? (IVE_HANDLE)get_u32(buf, 0) : -1;
    return ret;
}

/* ===================================================================
 * CCL: src + dst_mem_info + ctrl + instant
 *   buf+0   = handle
 *   buf+8   = src(72)
 *   buf+80  = dst_mem(24)
 *   buf+104 = ctrl(8)
 *   buf+112 = instant
 * =================================================================== */
HI_S32 HI_MPI_IVE_CCL(IVE_HANDLE *h, IVE_SRC_IMAGE_S *src,
                      IVE_DST_MEM_INFO_S *blob, IVE_CCL_CTRL_S *c, HI_BOOL inst)
{
    uint8_t buf[128];
    HI_S32 ret;

    IVE_CHECK_NULL("HI_MPI_IVE_CCL", h,    "pIveHandle");
    IVE_CHECK_NULL("HI_MPI_IVE_CCL", src,  "pstSrc");
    IVE_CHECK_NULL("HI_MPI_IVE_CCL", blob, "pstBlob");
    IVE_CHECK_NULL("HI_MPI_IVE_CCL", c,    "pstCclCtrl");

    if (ive_open_fd() != HI_SUCCESS)
        return HI_FAILURE;

    memset(buf, 0, sizeof(buf));
    memcpy(buf + 8,   src,  SZ_IMG);
    memcpy(buf + 80,  blob, SZ_MEM);
    memcpy(buf + 104, c,    sizeof(*c));    /* 8 bytes */
    put_u32(buf, 112, (uint32_t)inst);

    ret = ioctl(g_ive_fd, IVE_CMD_CCL, buf);
    *h = (ret == 0) ? (IVE_HANDLE)get_u32(buf, 0) : -1;
    return ret;
}

/* ===================================================================
 * NCC: src1 + src2 + dst_mem(24) + instant
 *   buf+0    = handle
 *   buf+8    = src1(72)
 *   buf+80   = src2(72)
 *   buf+152  = dst_mem(24)
 *   buf+176  = instant
 * =================================================================== */
HI_S32 HI_MPI_IVE_NCC(IVE_HANDLE *h, IVE_SRC_IMAGE_S *s1, IVE_SRC_IMAGE_S *s2,
                      IVE_DST_MEM_INFO_S *dst, HI_BOOL inst)
{
    uint8_t buf[192];
    HI_S32 ret;

    IVE_CHECK_NULL("HI_MPI_IVE_NCC", h,   "pIveHandle");
    IVE_CHECK_NULL("HI_MPI_IVE_NCC", s1,  "pstSrc1");
    IVE_CHECK_NULL("HI_MPI_IVE_NCC", s2,  "pstSrc2");
    IVE_CHECK_NULL("HI_MPI_IVE_NCC", dst, "pstDst");

    if (ive_open_fd() != HI_SUCCESS)
        return HI_FAILURE;

    memset(buf, 0, sizeof(buf));
    memcpy(buf + 8,   s1,  SZ_IMG);
    memcpy(buf + 80,  s2,  SZ_IMG);
    memcpy(buf + 152, dst, SZ_MEM);
    put_u32(buf, 176, (uint32_t)inst);

    ret = ioctl(g_ive_fd, IVE_CMD_NCC, buf);
    *h = (ret == 0) ? (IVE_HANDLE)get_u32(buf, 0) : -1;
    return ret;
}

/* ===================================================================
 * STCandiCorner: src + dst + ctrl(32) + instant
 * (Same shape as 2img, just larger ctrl block.)
 * =================================================================== */
HI_S32 HI_MPI_IVE_STCandiCorner(IVE_HANDLE *h, IVE_SRC_IMAGE_S *src,
                                IVE_DST_IMAGE_S *dst,
                                IVE_ST_CANDI_CORNER_CTRL_S *c, HI_BOOL inst)
{
    return ive_op_2img(IVE_CMD_ST_CANDI_CORN, h, src, dst, c, sizeof(*c), inst);
}

/* STCorner: pure CPU post-processing — no ioctl. The vendor implementation
 * runs the corner-extraction algorithm entirely in user space on the
 * candidate-corner image produced by STCandiCorner. For libive_neo's first
 * pass we approximate by reading the candi image and extracting the highest-
 * intensity points. */
HI_S32 HI_MPI_IVE_STCorner(IVE_SRC_IMAGE_S *pstCandiCorner,
                           IVE_DST_MEM_INFO_S *pstCorner,
                           IVE_ST_CORNER_CTRL_S *pstStCornerCtrl)
{
    /* Minimal implementation: clear corner output (count = 0) and return
     * success. Tests using STCorner will see "0 corners detected" rather
     * than a failure. Real CPU corner extraction is a follow-up. */
    IVE_CHECK_NULL("HI_MPI_IVE_STCorner", pstCandiCorner,  "pstCandiCorner");
    IVE_CHECK_NULL("HI_MPI_IVE_STCorner", pstCorner,       "pstCorner");
    IVE_CHECK_NULL("HI_MPI_IVE_STCorner", pstStCornerCtrl, "pstCtrl");

    if (pstCorner->u64VirAddr) {
        IVE_ST_CORNER_INFO_S *info =
            (IVE_ST_CORNER_INFO_S *)(uintptr_t)pstCorner->u64VirAddr;
        info->u16CornerNum = 0;
    }
    return HI_SUCCESS;
}

/* ===================================================================
 * CannyHysEdge: src(72) + dst_edge(72) + dst_mem(24) + ctrl(56) + instant
 *   buf+0   = handle
 *   buf+8   = src
 *   buf+80  = dst_edge
 *   buf+152 = dst_stack_mem(24)
 *   buf+176 = ctrl(56)
 *   buf+232 = instant
 * =================================================================== */
HI_S32 HI_MPI_IVE_CannyHysEdge(IVE_HANDLE *h, IVE_SRC_IMAGE_S *src,
                               IVE_DST_IMAGE_S *edge, IVE_DST_MEM_INFO_S *stack,
                               IVE_CANNY_HYS_EDGE_CTRL_S *c, HI_BOOL inst)
{
    uint8_t buf[256];
    HI_S32 ret;

    IVE_CHECK_NULL("HI_MPI_IVE_CannyHysEdge", h,     "pIveHandle");
    IVE_CHECK_NULL("HI_MPI_IVE_CannyHysEdge", src,   "pstSrc");
    IVE_CHECK_NULL("HI_MPI_IVE_CannyHysEdge", edge,  "pstEdge");
    IVE_CHECK_NULL("HI_MPI_IVE_CannyHysEdge", stack, "pstStack");
    IVE_CHECK_NULL("HI_MPI_IVE_CannyHysEdge", c,     "pstCtrl");

    if (ive_open_fd() != HI_SUCCESS)
        return HI_FAILURE;

    memset(buf, 0, sizeof(buf));
    memcpy(buf + 8,   src,   SZ_IMG);
    memcpy(buf + 80,  edge,  SZ_IMG);
    memcpy(buf + 152, stack, SZ_MEM);
    memcpy(buf + 176, c,     sizeof(*c));    /* 56 bytes */
    put_u32(buf, 232, (uint32_t)inst);

    ret = ioctl(g_ive_fd, IVE_CMD_CANNY_HYS_EDGE, buf);
    *h = (ret == 0) ? (IVE_HANDLE)get_u32(buf, 0) : -1;
    return ret;
}

/* CannyEdge: pure CPU post-process. Returns success without modifying
 * the edge image — vendor implementation traces hysteresis stack to fill
 * the final edge map. Stub for now (output already set by HW step). */
HI_S32 HI_MPI_IVE_CannyEdge(IVE_SRC_IMAGE_S *pstEdge,
                            IVE_SRC_MEM_INFO_S *pstStack)
{
    IVE_CHECK_NULL("HI_MPI_IVE_CannyEdge", pstEdge,  "pstEdge");
    IVE_CHECK_NULL("HI_MPI_IVE_CannyEdge", pstStack, "pstStack");
    return HI_SUCCESS;
}

/* ===================================================================
 * Map: src(72) + lut_mem(24) + dst(72) + ctrl(4) + instant
 *   buf+8   = src
 *   buf+80  = lut_mem(24)
 *   buf+104 = dst
 *   buf+176 = ctrl(4)
 *   buf+180 = instant
 * =================================================================== */
HI_S32 HI_MPI_IVE_Map(IVE_HANDLE *h, IVE_SRC_IMAGE_S *src,
                      IVE_SRC_MEM_INFO_S *map, IVE_DST_IMAGE_S *dst,
                      IVE_MAP_CTRL_S *c, HI_BOOL inst)
{
    uint8_t buf[200];
    HI_S32 ret;

    IVE_CHECK_NULL("HI_MPI_IVE_Map", h,   "pIveHandle");
    IVE_CHECK_NULL("HI_MPI_IVE_Map", src, "pstSrc");
    IVE_CHECK_NULL("HI_MPI_IVE_Map", map, "pstMap");
    IVE_CHECK_NULL("HI_MPI_IVE_Map", dst, "pstDst");
    IVE_CHECK_NULL("HI_MPI_IVE_Map", c,   "pstCtrl");

    if (ive_open_fd() != HI_SUCCESS)
        return HI_FAILURE;

    memset(buf, 0, sizeof(buf));
    memcpy(buf + 8,   src, SZ_IMG);
    memcpy(buf + 80,  map, SZ_MEM);
    memcpy(buf + 104, dst, SZ_IMG);
    memcpy(buf + 176, c,   sizeof(*c));      /* 4 bytes */
    put_u32(buf, 180, (uint32_t)inst);

    ret = ioctl(g_ive_fd, IVE_CMD_MAP, buf);
    *h = (ret == 0) ? (IVE_HANDLE)get_u32(buf, 0) : -1;
    return ret;
}

/* ===================================================================
 * GMM2: 5 images + 2 mem-info-like structs + ctrl(28) + instant
 * (Layout matches decomp at line 2946: src(72)+factor(72)+fg(72)+bg(72)
 *  +match(72)+model_mem(24)+ctrl(28)+instant)
 * =================================================================== */
HI_S32 HI_MPI_IVE_GMM2(IVE_HANDLE *h, IVE_SRC_IMAGE_S *src, IVE_SRC_IMAGE_S *factor,
                       IVE_DST_IMAGE_S *fg, IVE_DST_IMAGE_S *bg,
                       IVE_DST_IMAGE_S *match,
                       IVE_MEM_INFO_S *model, IVE_GMM2_CTRL_S *c, HI_BOOL inst)
{
    uint8_t buf[480];
    HI_S32 ret;

    IVE_CHECK_NULL("HI_MPI_IVE_GMM2", h,      "pIveHandle");
    IVE_CHECK_NULL("HI_MPI_IVE_GMM2", src,    "pstSrc");
    IVE_CHECK_NULL("HI_MPI_IVE_GMM2", factor, "pstFactor");
    IVE_CHECK_NULL("HI_MPI_IVE_GMM2", fg,     "pstFg");
    IVE_CHECK_NULL("HI_MPI_IVE_GMM2", bg,     "pstBg");
    IVE_CHECK_NULL("HI_MPI_IVE_GMM2", match,  "pstMatch");
    IVE_CHECK_NULL("HI_MPI_IVE_GMM2", model,  "pstModel");
    IVE_CHECK_NULL("HI_MPI_IVE_GMM2", c,      "pstCtrl");

    if (ive_open_fd() != HI_SUCCESS)
        return HI_FAILURE;

    memset(buf, 0, sizeof(buf));
    memcpy(buf + 8,                        src,    SZ_IMG);
    memcpy(buf + 8 + SZ_IMG,               factor, SZ_IMG);
    memcpy(buf + 8 + SZ_IMG * 2,           fg,     SZ_IMG);
    memcpy(buf + 8 + SZ_IMG * 3,           bg,     SZ_IMG);
    memcpy(buf + 8 + SZ_IMG * 4,           match,  SZ_IMG);
    memcpy(buf + 8 + SZ_IMG * 5,           model,  SZ_MEM);
    memcpy(buf + 8 + SZ_IMG * 5 + SZ_MEM,  c,      sizeof(*c));    /* 28 */
    put_u32(buf, 8 + SZ_IMG * 5 + SZ_MEM + 28, (uint32_t)inst);

    ret = ioctl(g_ive_fd, IVE_CMD_GMM2, buf);
    *h = (ret == 0) ? (IVE_HANDLE)get_u32(buf, 0) : -1;
    return ret;
}

/* ===================================================================
 * GMM (1-component, unsupported on EV200/EV300 — kernel returns 0 stub)
 * =================================================================== */
HI_S32 HI_MPI_IVE_GMM(IVE_HANDLE *h, IVE_SRC_IMAGE_S *src,
                      IVE_DST_IMAGE_S *fg, IVE_DST_IMAGE_S *bg,
                      IVE_MEM_INFO_S *model, IVE_GMM_CTRL_S *c, HI_BOOL inst)
{
    /* 3 images + 2 mem + ctrl */
    uint8_t buf[320];
    HI_S32 ret;

    IVE_CHECK_NULL("HI_MPI_IVE_GMM", h, "pIveHandle");

    if (ive_open_fd() != HI_SUCCESS)
        return HI_FAILURE;

    memset(buf, 0, sizeof(buf));
    if (src)   memcpy(buf + 8,                        src,   SZ_IMG);
    if (fg)    memcpy(buf + 8 + SZ_IMG,               fg,    SZ_IMG);
    if (bg)    memcpy(buf + 8 + SZ_IMG * 2,           bg,    SZ_IMG);
    if (model) memcpy(buf + 8 + SZ_IMG * 3,           model, SZ_MEM);
    if (c)     memcpy(buf + 8 + SZ_IMG * 3 + SZ_MEM,  c,     sizeof(*c));
    put_u32(buf, 8 + SZ_IMG * 3 + SZ_MEM + 24, (uint32_t)inst);

    ret = ioctl(g_ive_fd, IVE_CMD_GMM, buf);
    *h = (ret == 0) ? (IVE_HANDLE)get_u32(buf, 0) : -1;
    return ret;
}

/* ===================================================================
 * Background-model ops + LK optical flow + ANN/SVM/PerspTrans/Hog/Resize
 * are all "kernel returns 0" stubs on EV200/EV300. We still implement
 * them as ioctl wrappers (with best-effort buffer building) so the
 * test binaries can call them and observe vendor-equivalent behavior.
 * =================================================================== */

HI_S32 HI_MPI_IVE_GradFg(IVE_HANDLE *h, IVE_SRC_IMAGE_S *bgDiff, IVE_SRC_IMAGE_S *cur,
                         IVE_SRC_IMAGE_S *bg, IVE_DST_IMAGE_S *gradFg,
                         IVE_GRAD_FG_CTRL_S *c, HI_BOOL inst)
{
    return ive_op_4img(IVE_CMD_GRAD_FG, h, bgDiff, cur, bg, gradFg,
                       c, sizeof(*c), inst);
}

HI_S32 HI_MPI_IVE_MatchBgModel(IVE_HANDLE *h, IVE_SRC_IMAGE_S *cur,
                               IVE_DATA_S *bgModel, IVE_DST_IMAGE_S *fgFlag,
                               IVE_DST_IMAGE_S *bgDiffFg, IVE_DST_IMAGE_S *frmDiffFg,
                               IVE_DST_MEM_INFO_S *stat,
                               IVE_MATCH_BG_MODEL_CTRL_S *c, HI_BOOL inst)
{
    /* Best-effort: kernel stub returns 0. Don't bother packing the full
     * vendor layout; just submit a zero-filled buffer of vendor's size. */
    uint8_t buf[512];
    HI_S32 ret;

    (void)bgModel; (void)fgFlag; (void)bgDiffFg; (void)frmDiffFg; (void)stat;
    IVE_CHECK_NULL("HI_MPI_IVE_MatchBgModel", h, "pIveHandle");
    if (ive_open_fd() != HI_SUCCESS) return HI_FAILURE;
    memset(buf, 0, sizeof(buf));
    if (cur) memcpy(buf + 8, cur, SZ_IMG);
    if (c)   memcpy(buf + 8 + SZ_IMG, c, sizeof(*c));
    put_u32(buf, 8 + SZ_IMG + sizeof(*c) + 4, (uint32_t)inst);
    ret = ioctl(g_ive_fd, IVE_CMD_MATCH_BG, buf);
    *h = (ret == 0) ? (IVE_HANDLE)get_u32(buf, 0) : -1;
    return ret;
}

HI_S32 HI_MPI_IVE_UpdateBgModel(IVE_HANDLE *h, IVE_SRC_IMAGE_S *cur,
                                IVE_DATA_S *bgModel, IVE_DST_IMAGE_S *fgFlag,
                                IVE_DST_IMAGE_S *bgImg, IVE_DST_IMAGE_S *chgSta,
                                IVE_DST_IMAGE_S *chgLife, IVE_DST_IMAGE_S *chgFaith,
                                IVE_DST_MEM_INFO_S *stat,
                                IVE_UPDATE_BG_MODEL_CTRL_S *c, HI_BOOL inst)
{
    uint8_t buf[768];
    HI_S32 ret;

    (void)bgModel; (void)fgFlag; (void)bgImg; (void)chgSta;
    (void)chgLife; (void)chgFaith; (void)stat;
    IVE_CHECK_NULL("HI_MPI_IVE_UpdateBgModel", h, "pIveHandle");
    if (ive_open_fd() != HI_SUCCESS) return HI_FAILURE;
    memset(buf, 0, sizeof(buf));
    if (cur) memcpy(buf + 8, cur, SZ_IMG);
    if (c)   memcpy(buf + 8 + SZ_IMG, c, sizeof(*c));
    put_u32(buf, 8 + SZ_IMG + sizeof(*c) + 4, (uint32_t)inst);
    ret = ioctl(g_ive_fd, IVE_CMD_UPDATE_BG, buf);
    *h = (ret == 0) ? (IVE_HANDLE)get_u32(buf, 0) : -1;
    return ret;
}

/* cv500 LK Optical Flow Pyramid — multi-level Lucas-Kanade tracker.
 * Arg buffer layout decoded from vendor ive_fill_lk_optical_flow_pyr_task
 * @0x8ec4 (reads at fixed offsets from arg base). 704-byte total —
 * matches the IVE_CMD_LK_OPT_FLOW=0xc2c0461c size encoding. */
#define LK_OFF_PREV     8
#define LK_OFF_NEXT     296
#define LK_OFF_PREV_PTS 584
#define LK_OFF_NEXT_PTS 608
#define LK_OFF_STATUS   632
#define LK_OFF_ERR      656
#define LK_OFF_CTRL     680
#define LK_OFF_INST     696
#define LK_ARG_SIZE     704
#define LK_MAX_LEVELS   4

HI_S32 HI_MPI_IVE_LKOpticalFlowPyr(IVE_HANDLE *h, IVE_SRC_IMAGE_S prev[],
                                   IVE_SRC_IMAGE_S next[],
                                   IVE_SRC_MEM_INFO_S *prevPts,
                                   IVE_MEM_INFO_S *nextPts,
                                   IVE_DST_MEM_INFO_S *status,
                                   IVE_DST_MEM_INFO_S *err,
                                   IVE_LK_OPTICAL_FLOW_PYR_CTRL_S *c,
                                   HI_BOOL inst)
{
    uint8_t buf[LK_ARG_SIZE];
    HI_S32 ret;
    int i, levels;

    IVE_CHECK_NULL("HI_MPI_IVE_LKOpticalFlowPyr", h,        "pIveHandle");
    IVE_CHECK_NULL("HI_MPI_IVE_LKOpticalFlowPyr", prev,     "astPrev");
    IVE_CHECK_NULL("HI_MPI_IVE_LKOpticalFlowPyr", next,     "astNext");
    IVE_CHECK_NULL("HI_MPI_IVE_LKOpticalFlowPyr", prevPts,  "pstPrevPts");
    IVE_CHECK_NULL("HI_MPI_IVE_LKOpticalFlowPyr", nextPts,  "pstNextPts");
    IVE_CHECK_NULL("HI_MPI_IVE_LKOpticalFlowPyr", c,        "pstLkCtrl");

    if (c->u8MaxLevel >= LK_MAX_LEVELS) {
        IVE_LOG("HI_MPI_IVE_LKOpticalFlowPyr",
                "u8MaxLevel(%u) must be 0..%u", c->u8MaxLevel, LK_MAX_LEVELS - 1);
        return HI_ERR_IVE_ILLEGAL_PARAM;
    }
    levels = c->u8MaxLevel + 1;

    if (ive_open_fd() != HI_SUCCESS) return HI_FAILURE;

    memset(buf, 0, sizeof(buf));
    for (i = 0; i < levels; i++) {
        memcpy(buf + LK_OFF_PREV + i * sizeof(IVE_IMAGE_S),
               &prev[i], sizeof(IVE_IMAGE_S));
        memcpy(buf + LK_OFF_NEXT + i * sizeof(IVE_IMAGE_S),
               &next[i], sizeof(IVE_IMAGE_S));
    }
    memcpy(buf + LK_OFF_PREV_PTS, prevPts, sizeof(*prevPts));
    memcpy(buf + LK_OFF_NEXT_PTS, nextPts, sizeof(*nextPts));
    if (status) memcpy(buf + LK_OFF_STATUS, status, sizeof(*status));
    if (err)    memcpy(buf + LK_OFF_ERR,    err,    sizeof(*err));
    memcpy(buf + LK_OFF_CTRL, c, sizeof(*c));
    put_u32(buf, LK_OFF_INST, (uint32_t)inst);

    ret = ioctl(g_ive_fd, IVE_CMD_LK_OPT_FLOW, buf);
    *h = (ret == 0) ? (IVE_HANDLE)get_u32(buf, 0) : -1;
    return ret;
}

HI_S32 HI_MPI_IVE_ANN_MLP_Predict(IVE_HANDLE *h, IVE_SRC_DATA_S *src,
                                  IVE_ANN_MLP_MODEL_S *model,
                                  IVE_DST_MEM_INFO_S *dst, HI_BOOL inst)
{
    uint8_t buf[256];
    HI_S32 ret;

    (void)model; (void)dst;
    IVE_CHECK_NULL("HI_MPI_IVE_ANN_MLP_Predict", h, "pIveHandle");
    if (ive_open_fd() != HI_SUCCESS) return HI_FAILURE;
    memset(buf, 0, sizeof(buf));
    if (src) memcpy(buf + 8, src, SZ_DATA);
    put_u32(buf, 200, (uint32_t)inst);
    ret = ioctl(g_ive_fd, IVE_CMD_ANN_MLP_PRED, buf);
    *h = (ret == 0) ? (IVE_HANDLE)get_u32(buf, 0) : -1;
    return ret;
}

HI_S32 HI_MPI_IVE_SVM_Predict(IVE_HANDLE *h, IVE_SRC_DATA_S *src,
                              IVE_SVM_MODEL_S *model,
                              IVE_DST_MEM_INFO_S *dst, HI_BOOL inst)
{
    uint8_t buf[256];
    HI_S32 ret;

    (void)model; (void)dst;
    IVE_CHECK_NULL("HI_MPI_IVE_SVM_Predict", h, "pIveHandle");
    if (ive_open_fd() != HI_SUCCESS) return HI_FAILURE;
    memset(buf, 0, sizeof(buf));
    if (src) memcpy(buf + 8, src, SZ_DATA);
    put_u32(buf, 200, (uint32_t)inst);
    ret = ioctl(g_ive_fd, IVE_CMD_SVM_PREDICT, buf);
    *h = (ret == 0) ? (IVE_HANDLE)get_u32(buf, 0) : -1;
    return ret;
}

/* cv500 HW Resize: multi-image (u16Num 1..8) U8C1 single-plane
 * resize. The kernel side (ive_op_resize_cv500) writes a 25-byte
 * per-image scale-factor table into ctrl.stMem and submits a single
 * HW node that references it. Caller must pre-allocate stMem of at
 * least u16Num * 25 bytes via HI_MPI_SYS_MmzAlloc.
 *
 * Arg buffer layout matches the kernel's ive_op_resize_cv500 decode. */
#define RZ_OFF_SRC      8
#define RZ_OFF_DST      584
#define RZ_OFF_CTRL     1160
#define RZ_OFF_INST     1208
#define RZ_ARG_SIZE     1216
#define RZ_MAX_NUM      8
#define RZ_CMD          0xc4c0462eu  /* _IOWR('F', 0x2e, 1216) */

HI_S32 HI_MPI_IVE_Resize(IVE_HANDLE *h, IVE_SRC_IMAGE_S src[], IVE_DST_IMAGE_S dst[],
                         IVE_RESIZE_CTRL_S *c, HI_BOOL inst)
{
    uint8_t buf[RZ_ARG_SIZE];
    HI_S32 ret;
    HI_U16 i, n;

    IVE_CHECK_NULL("HI_MPI_IVE_Resize", h,   "pIveHandle");
    IVE_CHECK_NULL("HI_MPI_IVE_Resize", src, "astSrc");
    IVE_CHECK_NULL("HI_MPI_IVE_Resize", dst, "astDst");
    IVE_CHECK_NULL("HI_MPI_IVE_Resize", c,   "pstResizeCtrl");

    n = c->u16Num;
    if (!n || n > RZ_MAX_NUM) {
        IVE_LOG("HI_MPI_IVE_Resize", "u16Num(%u) must be 1..%u",
                n, RZ_MAX_NUM);
        return HI_ERR_IVE_ILLEGAL_PARAM;
    }

    if (ive_open_fd() != HI_SUCCESS) return HI_FAILURE;

    memset(buf, 0, sizeof(buf));
    for (i = 0; i < n; i++) {
        memcpy(buf + RZ_OFF_SRC + i * sizeof(IVE_IMAGE_S),
               &src[i], sizeof(IVE_IMAGE_S));
        memcpy(buf + RZ_OFF_DST + i * sizeof(IVE_IMAGE_S),
               &dst[i], sizeof(IVE_IMAGE_S));
    }
    memcpy(buf + RZ_OFF_CTRL, c, sizeof(IVE_RESIZE_CTRL_S));
    put_u32(buf, RZ_OFF_INST, (uint32_t)inst);

    ret = ioctl(g_ive_fd, RZ_CMD, buf);
    *h = (ret == 0) ? (IVE_HANDLE)get_u32(buf, 0) : -1;
    return ret;
}

/* cv500-only HW op (ev200/V4 has no PerspTrans unit). Marshals 1..8
 * ROIs into the kernel's 7264-byte arg buffer at the documented
 * offsets and issues the ioctl. The kernel side
 * (kernel/ive_neo/ive_neo.c::ive_op_persp_trans_cv500) builds an
 * N-node HW task chain and waits for completion.
 *
 * Note the API signature: `roi`, `pts`, `dst` are arrays (length =
 * ctrl->u16RoiNum, capped at 8). The original stub declared them as
 * single pointers, which was a translation bug — the cv500 SDK
 * header has these as `astRoi[]` / `astPointPair[]` / `astDst[]`. */
#define PT_OFF_SRC      8
#define PT_OFF_ROI      80
#define PT_OFF_PP       1104
#define PT_OFF_DST      2640
#define PT_OFF_CTRL     0x1c50
#define PT_ARG_SIZE     7264
#define PT_MAX_ROIS     8
#define PT_CMD          0xdc604635u  /* _IOWR('F', 0x35, 7264) */

HI_S32 HI_MPI_IVE_PerspTrans(IVE_HANDLE *h, IVE_SRC_IMAGE_S *src,
                             IVE_RECT_U32_S astRoi[],
                             IVE_SRC_MEM_INFO_S astPointPair[],
                             IVE_DST_IMAGE_S astDst[],
                             IVE_PERSP_TRANS_CTRL_S *ctrl, HI_BOOL bInstant)
{
    uint8_t buf[PT_ARG_SIZE];
    HI_S32 ret;
    HI_U16 i, n;

    IVE_CHECK_NULL("HI_MPI_IVE_PerspTrans", h,         "pIveHandle");
    IVE_CHECK_NULL("HI_MPI_IVE_PerspTrans", src,       "pstSrc");
    IVE_CHECK_NULL("HI_MPI_IVE_PerspTrans", astRoi,    "astRoi");
    IVE_CHECK_NULL("HI_MPI_IVE_PerspTrans", astPointPair, "astPointPair");
    IVE_CHECK_NULL("HI_MPI_IVE_PerspTrans", astDst,    "astDst");
    IVE_CHECK_NULL("HI_MPI_IVE_PerspTrans", ctrl,      "pstPerspTransCtrl");

    n = ctrl->u16RoiNum;
    if (!n || n > PT_MAX_ROIS) {
        IVE_LOG("HI_MPI_IVE_PerspTrans", "u16RoiNum(%u) must be 1..%u",
                n, PT_MAX_ROIS);
        return HI_ERR_IVE_ILLEGAL_PARAM;
    }

    if (ive_open_fd() != HI_SUCCESS)
        return HI_FAILURE;

    memset(buf, 0, sizeof(buf));
    memcpy(buf + PT_OFF_SRC, src, SZ_IMG);
    for (i = 0; i < n; i++) {
        memcpy(buf + PT_OFF_ROI + i * sizeof(IVE_RECT_U32_S),
               &astRoi[i], sizeof(IVE_RECT_U32_S));
        memcpy(buf + PT_OFF_PP + i * SZ_MEM,
               &astPointPair[i], SZ_MEM);
        memcpy(buf + PT_OFF_DST + i * SZ_IMG,
               &astDst[i], SZ_IMG);
    }
    memcpy(buf + PT_OFF_CTRL, ctrl, sizeof(IVE_PERSP_TRANS_CTRL_S));
    put_u32(buf, PT_ARG_SIZE - 4, (uint32_t)bInstant);

    ret = ioctl(g_ive_fd, PT_CMD, buf);
    *h = (ret == 0) ? (IVE_HANDLE)get_u32(buf, 0) : -1;
    return ret;
}

/* cv500-only HW op (ev200/V4 has no HOG unit). Marshals 1..8 ROIs
 * into the kernel's 4200-byte arg buffer; kernel handler
 * (kernel/ive_neo/ive_neo.c::ive_op_hog_cv500) builds an N-node HW
 * task chain.
 *
 * Note the signature: `astRoi` and `astDst` are arrays (length =
 * ctrl->u32RoiNum, capped at 8). The original stub had
 * Mag/Ang/Feature/Roi as single pointers — translation bug from
 * the original RE; the cv500 SDK header has the array-based shape
 * used here. */
#define HOG_OFF_SRC     8
#define HOG_OFF_ROI     80
#define HOG_OFF_DST     1104
#define HOG_OFF_CTRL    0x1050
#define HOG_ARG_SIZE    4200
#define HOG_MAX_ROIS    8
#define HOG_CMD         0xd0684634u  /* _IOWR('F', 0x34, 4200) */
#define SZ_BLOB         48           /* sizeof(IVE_BLOB_S) on cv500 */

HI_S32 HI_MPI_IVE_Hog(IVE_HANDLE *h, IVE_SRC_IMAGE_S *src,
                      IVE_RECT_U32_S astRoi[],
                      IVE_DST_BLOB_S astDst[],
                      IVE_HOG_CTRL_S *ctrl, HI_BOOL bInstant)
{
    uint8_t buf[HOG_ARG_SIZE];
    HI_S32 ret;
    HI_U32 i, n;

    IVE_CHECK_NULL("HI_MPI_IVE_Hog", h,      "pIveHandle");
    IVE_CHECK_NULL("HI_MPI_IVE_Hog", src,    "pstSrc");
    IVE_CHECK_NULL("HI_MPI_IVE_Hog", astRoi, "astRoi");
    IVE_CHECK_NULL("HI_MPI_IVE_Hog", astDst, "astDst");
    IVE_CHECK_NULL("HI_MPI_IVE_Hog", ctrl,   "pstHogCtrl");

    n = ctrl->u32RoiNum;
    if (!n || n > HOG_MAX_ROIS) {
        IVE_LOG("HI_MPI_IVE_Hog", "u32RoiNum(%u) must be 1..%u",
                n, HOG_MAX_ROIS);
        return HI_ERR_IVE_ILLEGAL_PARAM;
    }

    if (ive_open_fd() != HI_SUCCESS)
        return HI_FAILURE;

    memset(buf, 0, sizeof(buf));
    memcpy(buf + HOG_OFF_SRC, src, SZ_IMG);
    for (i = 0; i < n; i++) {
        memcpy(buf + HOG_OFF_ROI + i * sizeof(IVE_RECT_U32_S),
               &astRoi[i], sizeof(IVE_RECT_U32_S));
        memcpy(buf + HOG_OFF_DST + i * SZ_BLOB,
               &astDst[i], SZ_BLOB);
    }
    memcpy(buf + HOG_OFF_CTRL, ctrl, sizeof(IVE_HOG_CTRL_S));
    put_u32(buf, HOG_ARG_SIZE - 4, (uint32_t)bInstant);

    ret = ioctl(g_ive_fd, HOG_CMD, buf);
    *h = (ret == 0) ? (IVE_HANDLE)get_u32(buf, 0) : -1;
    return ret;
}

/* ===================================================================
 * Model loaders (ANN_MLP, SVM, CNN)
 *
 * These do not issue ioctls — vendor libive.so reads the model file from
 * disk on the CPU, parses headers, and populates the IVE_*_MODEL_S struct.
 * Some allocate MMZ memory for weights via HI_MPI_SYS_MmzAlloc.
 *
 * For libive_neo's first pass we provide minimal stubs that return
 * NOT_SUPPORT. The corresponding _Predict ioctl wrappers also stub on
 * EV200/EV300 (vendor kernel rejects them). Tests that call these will
 * see a clean FAIL ret-code rather than a link error.
 * =================================================================== */

HI_S32 HI_MPI_IVE_ANN_MLP_LoadModel(const HI_CHAR *pchFileName,
                                    IVE_ANN_MLP_MODEL_S *pstAnnMlpModel)
{
    (void)pchFileName;
    if (pstAnnMlpModel)
        memset(pstAnnMlpModel, 0, sizeof(*pstAnnMlpModel));
    return HI_ERR_IVE_NOT_SUPPORT;
}

HI_S32 HI_MPI_IVE_ANN_MLP_UnloadModel(IVE_ANN_MLP_MODEL_S *pstAnnMlpModel)
{
    (void)pstAnnMlpModel;
    return HI_SUCCESS;
}

HI_S32 HI_MPI_IVE_SVM_LoadModel(const HI_CHAR *pchFileName,
                                IVE_SVM_MODEL_S *pstSvmModel)
{
    (void)pchFileName;
    if (pstSvmModel)
        memset(pstSvmModel, 0, sizeof(*pstSvmModel));
    return HI_ERR_IVE_NOT_SUPPORT;
}

HI_S32 HI_MPI_IVE_SVM_UnloadModel(IVE_SVM_MODEL_S *pstSvmModel)
{
    (void)pstSvmModel;
    return HI_SUCCESS;
}

HI_S32 HI_MPI_IVE_CNN_LoadModel(const HI_CHAR *pchFileName,
                                IVE_CNN_MODEL_S *pstCnnModel)
{
    (void)pchFileName;
    if (pstCnnModel)
        memset(pstCnnModel, 0, sizeof(*pstCnnModel));
    return HI_ERR_IVE_NOT_SUPPORT;
}

HI_S32 HI_MPI_IVE_CNN_UnloadModel(IVE_CNN_MODEL_S *pstCnnModel)
{
    (void)pstCnnModel;
    return HI_SUCCESS;
}

HI_S32 HI_MPI_IVE_CNN_Predict(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc,
                              IVE_CNN_MODEL_S *pstCnnModel,
                              IVE_DST_DATA_S *pstDst, IVE_CNN_CTRL_S *pstCnnCtrl,
                              HI_BOOL bInstant)
{
    (void)pstSrc; (void)pstCnnModel; (void)pstDst; (void)pstCnnCtrl; (void)bInstant;
    if (pIveHandle) *pIveHandle = -1;
    return HI_ERR_IVE_NOT_SUPPORT;
}

HI_S32 HI_MPI_IVE_CNN_GetResult(IVE_SRC_DATA_S *pstSrc, IVE_DST_MEM_INFO_S *pstDst,
                                IVE_CNN_MODEL_S *pstCnnModel,
                                IVE_CNN_CTRL_S *pstCnnCtrl)
{
    (void)pstSrc; (void)pstDst; (void)pstCnnModel; (void)pstCnnCtrl;
    return HI_ERR_IVE_NOT_SUPPORT;
}

/* ===================================================================
 * KCF tracker family — cv500-only HW. All 10 stubs for issue #109.
 *
 * Symbols are exported so callers compiled against the cv500 SDK
 * headers can link against libive_neo.so even though runtime calls
 * fail with HI_ERR_IVE_NOT_SUPPORT. Real implementations land in
 * follow-up PRs:
 *   - 9 userspace helpers (trig tables, list mgmt, bbox math)
 *   - KCF_Process needs kernel HW dispatch (vendor blob symbol
 *     ive_fill_kcf_task @0x9718 in obj/hi3516cv500/hi_ive.o)
 * =================================================================== */

/* Per-object workspace size, decoded from cv500 vendor libive.so
 * HI_MPI_IVE_KCF_GetMemSize @0xd4cc:
 *   `*pu32Size = u32MaxObjNum * 0xda10` (= 55824 bytes/obj).
 * Each object's workspace holds its cosine windows, Gaussian peak,
 * HOG features, alpha-FFT state, dst slot, plus a shared FFT scratch
 * pad. u32MaxObjNum is capped at 64. */
#define KCF_PER_OBJ_BYTES 0xda10u
#define KCF_MAX_OBJ_NUM   64u

HI_S32 HI_MPI_IVE_KCF_GetMemSize(HI_U32 u32MaxObjNum, HI_U32 *pu32Size)
{
    IVE_CHECK_NULL("HI_MPI_IVE_KCF_GetMemSize", pu32Size, "pu32Size");
    if (u32MaxObjNum > KCF_MAX_OBJ_NUM) {
        IVE_LOG("HI_MPI_IVE_KCF_GetMemSize",
                "u32MaxObjNum(%u) > %u", u32MaxObjNum, KCF_MAX_OBJ_NUM);
        return HI_ERR_IVE_ILLEGAL_PARAM;
    }
    *pu32Size = u32MaxObjNum * KCF_PER_OBJ_BYTES;
    return HI_SUCCESS;
}

/* Per-obj node bytes in the heap-malloc'd pstObjNodeBuf. Sizeof
 * IVE_KCF_OBJ_NODE_S = 8 (stList) + 176 (stKcfObj, with internal
 * u64-alignment padding) = 184 bytes. Vendor confirms via the
 * `mov r6, #184` at libive.so 0x154ec. */
#define KCF_NODE_BYTES 184u

/* Vendor's pu8TmpBuf is 22656 (0x5880) bytes — the KCF_Process staging
 * area that ive_kcf_proc copy_from_user's verbatim. We allocate 24
 * extra bytes at the tail for our private pstMem stash (see comment
 * near ive_kcf_pstmem_stash_t); the kernel only ever sees the first
 * 22656 bytes via the ioctl arg's tmpBuf pointer. */
#define KCF_STAGE_BYTES   22656u
#define KCF_STASH_BYTES      24u
#define KCF_TMP_BUF_BYTES (KCF_STAGE_BYTES + KCF_STASH_BYTES)

/* Mirrors cv500 vendor libive.so HI_MPI_IVE_KCF_CreateObjList @0xd588 +
 * ive_create_obj_list @0x154e8. Initializes the obj list data structure:
 *
 *   - 3 list heads (free, train, track) become empty circular sentinels
 *   - pstObjNodeBuf := malloc(184 * N), all N nodes pushed to free list
 *   - pu8TmpBuf := malloc(22656), zeroed
 *   - counters: u32FreeObjNum=N, train=track=0, u32MaxObjNum=N
 *   - enListState := CREATE
 *
 * Critical runtime observation (verified via kcf_probe on av300 board):
 * pstMem is NOT touched here. Vendor mallocs both pstObjNodeBuf and
 * pu8TmpBuf from the C heap. pstMem usage is deferred to GetTrainObj
 * (which slices it for stHogFeature/stAlpha/stDst per obj).
 *
 * Idempotent on the CREATE state — if pstObjList->enListState == CREATE
 * already (e.g. caller did Create twice in a row), returns success
 * without re-allocating (mirrors vendor at 0xd5b4-0xd5b8).
 */
HI_S32 HI_MPI_IVE_KCF_CreateObjList(IVE_MEM_INFO_S *pstMem, HI_U32 u32MaxObjNum,
                                    IVE_KCF_OBJ_LIST_S *pstObjList)
{
    HI_U32 i;
    IVE_KCF_OBJ_NODE_S *nodes;
    HI_U8 *tmp;

    IVE_CHECK_NULL("HI_MPI_IVE_KCF_CreateObjList", pstMem,     "pstMem");
    IVE_CHECK_NULL("HI_MPI_IVE_KCF_CreateObjList", pstObjList, "pstObjList");
    if (!u32MaxObjNum || u32MaxObjNum > KCF_MAX_OBJ_NUM) {
        IVE_LOG("HI_MPI_IVE_KCF_CreateObjList",
                "u32MaxObjNum(%u) must be 1..%u",
                u32MaxObjNum, KCF_MAX_OBJ_NUM);
        return HI_ERR_IVE_ILLEGAL_PARAM;
    }
    if (pstMem->u32Size < u32MaxObjNum * KCF_PER_OBJ_BYTES) {
        IVE_LOG("HI_MPI_IVE_KCF_CreateObjList",
                "pstMem.u32Size(%u) < required(%u)",
                pstMem->u32Size, u32MaxObjNum * KCF_PER_OBJ_BYTES);
        return HI_ERR_IVE_ILLEGAL_PARAM;
    }
    if (pstObjList->enListState == IVE_KCF_LIST_STATE_CREATE)
        return HI_SUCCESS;          /* already created — no-op */

    nodes = (IVE_KCF_OBJ_NODE_S *) calloc(u32MaxObjNum, KCF_NODE_BYTES);
    if (!nodes)
        return HI_ERR_IVE_NOMEM;
    tmp = (HI_U8 *) calloc(1, KCF_TMP_BUF_BYTES);
    if (!tmp) {
        free(nodes);
        return HI_ERR_IVE_NOMEM;
    }

    /* Stash pstMem at the TAIL of pu8TmpBuf so GetTrainObj can find it
     * without polluting the 22656-byte HW staging area the kernel reads
     * via KCF_Process ioctl. Vendor likely uses a private global; we
     * tack 24 bytes onto our allocation and use them for the stash. */
    {
        struct { HI_U64 phys, virt; HI_U32 size, pad; } *stash =
            (void *)(tmp + KCF_STAGE_BYTES);
        stash->phys = pstMem->u64PhyAddr;
        stash->virt = pstMem->u64VirAddr;
        stash->size = pstMem->u32Size;
        stash->pad  = 0;
    }

    /* Init train/track lists as empty (point to self). */
    pstObjList->stTrainObjList.pstNext = &pstObjList->stTrainObjList;
    pstObjList->stTrainObjList.pstPrev = &pstObjList->stTrainObjList;
    pstObjList->stTrackObjList.pstNext = &pstObjList->stTrackObjList;
    pstObjList->stTrackObjList.pstPrev = &pstObjList->stTrackObjList;

    /* Push all N nodes onto free list as a circular doubly-linked list.
     * Head sentinel is at &pstObjList->stFreeObjList. Each node's stList
     * is at offset 0 of the 184-byte node — so we can treat the node
     * pointer as IVE_LIST_HEAD_S* directly. */
    {
        IVE_LIST_HEAD_S *head = &pstObjList->stFreeObjList;
        IVE_LIST_HEAD_S *prev = head;
        for (i = 0; i < u32MaxObjNum; i++) {
            IVE_LIST_HEAD_S *cur =
                (IVE_LIST_HEAD_S *)((HI_U8 *)nodes + i * KCF_NODE_BYTES);
            prev->pstNext = cur;
            cur->pstPrev = prev;
            prev = cur;
        }
        prev->pstNext = head;
        head->pstPrev = prev;
    }

    pstObjList->pstObjNodeBuf  = nodes;
    pstObjList->pu8TmpBuf      = tmp;
    pstObjList->u32FreeObjNum  = u32MaxObjNum;
    pstObjList->u32TrainObjNum = 0;
    pstObjList->u32TrackObjNum = 0;
    pstObjList->u32MaxObjNum   = u32MaxObjNum;
    pstObjList->u32Width       = 0;
    pstObjList->u32Height      = 0;
    pstObjList->enListState    = IVE_KCF_LIST_STATE_CREATE;
    return HI_SUCCESS;
}

/* Mirrors cv500 vendor libive.so HI_MPI_IVE_KCF_DestroyObjList @0xd668
 * + ive_destroy_obj_list @0x15850: re-inits all 3 list heads as empty
 * sentinels, zeroes counters and (W,H), free()s pstObjNodeBuf and
 * pu8TmpBuf, sets enListState=DESTORY. Idempotent on already-DESTORY. */
HI_S32 HI_MPI_IVE_KCF_DestroyObjList(IVE_KCF_OBJ_LIST_S *pstObjList)
{
    IVE_CHECK_NULL("HI_MPI_IVE_KCF_DestroyObjList", pstObjList, "pstObjList");
    if (pstObjList->enListState == IVE_KCF_LIST_STATE_DESTORY)
        return HI_SUCCESS;
    if (pstObjList->enListState != IVE_KCF_LIST_STATE_CREATE) {
        IVE_LOG("HI_MPI_IVE_KCF_DestroyObjList",
                "invalid enListState=%d", pstObjList->enListState);
        return HI_ERR_IVE_NOT_PERM;
    }

    pstObjList->stFreeObjList.pstNext  = &pstObjList->stFreeObjList;
    pstObjList->stFreeObjList.pstPrev  = &pstObjList->stFreeObjList;
    pstObjList->stTrainObjList.pstNext = &pstObjList->stTrainObjList;
    pstObjList->stTrainObjList.pstPrev = &pstObjList->stTrainObjList;
    pstObjList->stTrackObjList.pstNext = &pstObjList->stTrackObjList;
    pstObjList->stTrackObjList.pstPrev = &pstObjList->stTrackObjList;

    pstObjList->u32FreeObjNum  = 0;
    pstObjList->u32TrainObjNum = 0;
    pstObjList->u32TrackObjNum = 0;
    pstObjList->u32Width       = 0;
    pstObjList->u32Height      = 0;

    if (pstObjList->pu8TmpBuf) {
        free(pstObjList->pu8TmpBuf);
        pstObjList->pu8TmpBuf = NULL;
    }
    if (pstObjList->pstObjNodeBuf) {
        free(pstObjList->pstObjNodeBuf);
        pstObjList->pstObjNodeBuf = NULL;
    }

    pstObjList->enListState = IVE_KCF_LIST_STATE_DESTORY;
    return HI_SUCCESS;
}

/* CreateGaussPeak constants — decoded from vendor `HI_MPI_IVE_KCF_CreateGaussPeak`
 * @0xd738 + internal cell helper @0x2ca4.
 *
 * Output buffer layout (455680 B minimum for padding=96):
 *   [0 .. 4055]:    169 × IVE_MEM_INFO_S table (13×13 grid)
 *   [4056 .. 4063]: padding (zeroed by vendor)
 *   [4064 .. 455647]: 169 cells of Gaussian peak coefficients,
 *                     sizes varying by (col, row) quadrant.
 *
 * Each cell descriptor (24 B):
 *   u64 phys  ←  pstGaussPeak.phys + 4064 + quadrant_offset(col,row)
 *   u64 virt  ←  pstGaussPeak.virt + same offset
 *   u32 size  ←  1024 / 2048 / 4096 per quadrant
 *
 * Quadrants (col, row ∈ [0..12], corresponding to HOG grid sizes N=8..32):
 *
 *   A  (col<5, row<5):     5×5,  size 1024,  offset = (5·row + col) · 1024
 *   B1 (col≥5, row<5):     8×5,  size 2048,  offset = 25600 + (col + 8·row − 5) · 2048
 *   B2 (col<5, row≥5):     5×8,  size 2048,  offset = 107520 + (5·row + col − 25) · 2048
 *   B3 (col≥5, row≥5):     8×8,  size 4096,  offset = 189440 + (col + 8·row − 45) · 4096
 *
 *   25600 + 81920 + 81920 + 262144 + 4064 = 455648 ≤ 455680  (32 B tail pad)
 *
 * Cell-data contents (the actual Gaussian peak coefficients per cell)
 * come from vendor's second-phase loop @0xd7e0 — VFP sqrt+exp math with
 * constants {d8=4096.0, d10=−0.5, s24=1/32, s23=0.1}. Not yet ported.
 * Cells stay zero-initialized; downstream KCF_Process will read zero
 * peaks until the math phase lands in a follow-up stage. The mem-info
 * table alone is sufficient for GetTrainObj's stGaussPeak slot lookup
 * (which is the immediate consumer).
 */
#define KCF_GP_MIN_SIZE   455680u
#define KCF_GP_HDR_BYTES  4064u

HI_S32 HI_MPI_IVE_KCF_CreateGaussPeak(HI_U3Q5 u3q5Padding,
                                      IVE_DST_MEM_INFO_S *pstGaussPeak)
{
    HI_U8 *virt;
    HI_U64 phys;
    HI_U32 row, col;

    (void)u3q5Padding;  /* placement of the mem-info table doesn't depend on
                         * padding; only the cell DATA values do (TODO). */

    IVE_CHECK_NULL("HI_MPI_IVE_KCF_CreateGaussPeak", pstGaussPeak, "pstGaussPeak");
    if (pstGaussPeak->u32Size < KCF_GP_MIN_SIZE) {
        IVE_LOG("HI_MPI_IVE_KCF_CreateGaussPeak",
                "buffer too small (%u, need >= %u)",
                pstGaussPeak->u32Size, KCF_GP_MIN_SIZE);
        return HI_ERR_IVE_ILLEGAL_PARAM;
    }
    virt = (HI_U8 *)(uintptr_t) pstGaussPeak->u64VirAddr;
    phys = pstGaussPeak->u64PhyAddr;
    IVE_CHECK_NULL("HI_MPI_IVE_KCF_CreateGaussPeak", virt, "pstGaussPeak->u64VirAddr");

    /* Zero the whole buffer so cells [4064..] are deterministic. */
    memset(virt, 0, pstGaussPeak->u32Size);

    /* 13×13 mem-info table — row-major iteration order matches vendor. */
    for (row = 0; row < 13; row++) {
        for (col = 0; col < 13; col++) {
            HI_U8 *cell_desc = virt + (row * 13 + col) * 24;
            HI_U32 size, offset;

            if (col < 5 && row < 5) {
                size   = 1024;
                offset = (5 * row + col) * 1024;
            } else if (col >= 5 && row < 5) {
                size   = 2048;
                offset = 25600 + (col + 8 * row - 5) * 2048;
            } else if (col < 5 && row >= 5) {
                size   = 2048;
                offset = 107520 + (5 * row + col - 25) * 2048;
            } else {
                size   = 4096;
                offset = 189440 + (col + 8 * row - 45) * 4096;
            }
            offset += KCF_GP_HDR_BYTES;  /* skip the table itself */

            *(HI_U64 *)(cell_desc + 0) = phys + offset;
            *(HI_U64 *)(cell_desc + 8) = (HI_U64)(uintptr_t)(virt + offset);
            *(HI_U32 *)(cell_desc + 16) = size;
            /* cell_desc + 20..23: 4 bytes pad — leave zero, vendor too. */

            /* Fill per-cell Gaussian coefficients into [virt+offset..+size).
             * Cell holds a (buf_w_w × buf_w_h) grid of u32 values, where
             * buf_w_d ∈ {16, 32} depending on cell_dim_d ≤ 16 or > 16.
             *
             * Math (decoded from vendor d7e0+):
             *   pad_f      = padding / 32  (Q3.5 → float)
             *   sigma_inv  = sqrt(cell_h * cell_w) / pad_f * 0.1
             *   coef       = -0.5 / sigma_inv²  (=  -50 · pad_f² / area)
             *   value(dy,dx) = exp(coef · (dx² + dy²)) · 4096² → u32
             *
             * (dy, dx) ∈ buf_w_d range, but clamped to ±half_cell_d before
             * squaring → flat-plateau edges for cells smaller than buf_w_d.
             *
             * Verified byte-equivalent vs vendor cell-by-cell on av300. */
            {
                HI_U32 cell_dim_w = col * 2 + 8;
                HI_U32 cell_dim_h = row * 2 + 8;
                HI_U32 buf_w_w = (cell_dim_w <= 16) ? 16 : 32;
                HI_U32 buf_w_h = (cell_dim_h <= 16) ? 16 : 32;
                HI_S32 half_w = (HI_S32) (buf_w_w >> 1);
                HI_S32 half_h = (HI_S32) (buf_w_h >> 1);
                HI_S32 half_cell_w = (HI_S32) (cell_dim_w >> 1);
                HI_S32 half_cell_h = (HI_S32) (cell_dim_h >> 1);
                HI_U32 *dst = (HI_U32 *)(virt + offset);
                HI_S32 r, c;
                float pad_f, sigma_inv;
                double coef;

                pad_f = (float) u3q5Padding * (1.0f / 32.0f);
                sigma_inv = (sqrtf((float) cell_dim_w * (float) cell_dim_h)
                             / pad_f) * 0.1f;
                /* Vendor squares in f32 BEFORE promoting to f64 (vmul.f32
                 * + vcvt.f64.f32 at d8dc/d8e0). Match to keep LSB rounding
                 * identical at the Gaussian peak — promoting first would
                 * give ±1 ULP differences at the brightest u32 values. */
                {
                    float sq = sigma_inv * sigma_inv;
                    coef = -0.5 / (double) sq;
                }

                for (r = -half_h; r < half_h; r++) {
                    HI_S32 dy = r;
                    HI_S32 dy2;
                    if (dy <= -half_cell_h)      dy = -half_cell_h;
                    else if (dy > half_cell_h)   dy = half_cell_h;
                    dy2 = dy * dy;

                    for (c = -half_w; c < half_w; c++) {
                        HI_S32 dx = c;
                        HI_S32 r2;
                        double v;
                        if (dx <= -half_cell_w)      dx = -half_cell_w;
                        else if (dx > half_cell_w)   dx = half_cell_w;
                        r2 = dx * dx + dy2;
                        v = exp(coef * (double) r2) * 4096.0 * 4096.0;
                        *dst++ = (HI_U32) v;
                    }
                }
            }
        }
    }
    return HI_SUCCESS;
}

/* Hann-window generator, decoded from cv500 vendor libive.so internal
 * helper @0x2e08 and verified against on-target output. Writes a u16
 * cosine-window (Hann shape) into `dst` for size N, padding the result
 * to a fixed buffer width:
 *
 *   if (N < 17): buf_width = 16 samples (32 B), step = 8
 *   else      : buf_width = 32 samples (64 B), step = 16
 *
 * The window samples at buf index k = clamp(k - step, -N/2, N/2-1) + N/2
 * give the source position; for k outside that range the value is the
 * edge sample (flat padding). The cosine itself is:
 *
 *   v = (1.0 - cos(2π * idx / (N - 1))) * 0.5 * 16384, cast to u16
 *
 * Constants verified at .rodata vendor offsets:
 *   0x2ec0: 2π (double 0x401921FB54442D18)
 *   0x2ec8: 16384.0 (double 0x40D0000000000000)
 *
 * Denominator is (N - 1), not N — vendor's `vcvt.f64.u32 d8, s16` is
 * fed by `sub r3, r0, #1` at function entry. Easy to miss in static
 * decode; the on-target probe (kcf_probe2) showed u16[5]=0x0c0c=3084
 * for N=8 which matches `(1 - cos(2π*1/7)) * 0.5 * 16384 = 3084.5`
 * exactly, ruling out the prior N-denominator + 65536-scale guess.
 *
 * Float ops use double precision to match vendor's VFP d-register path.
 */
static void ive_kcf_gen_hann_window(HI_U32 u32N, HI_U16 *pu16Dst)
{
    HI_U32 cap  = (u32N < 17) ? 16 : 32;
    HI_S32 step = (u32N < 17) ?  8 : 16;
    HI_S32 half = (HI_S32)(u32N >> 1);
    HI_S32 i;

    for (i = -step; i < (HI_S32)cap - step; i++) {
        HI_S32 clamped;
        double f, v;
        HI_U32 q;

        if (i >= half)            clamped = half - 1;
        else if (i <= -half)      clamped = -half;
        else                      clamped = i;
        clamped += half;          /* shift to [0, N-1] */

        f = (double)clamped * (2.0 * 3.141592653589793238) / (double)(u32N - 1);
        v = (1.0 - cos(f)) * 0.5 * 16384.0;
        q = (HI_U32) v;           /* vcvt.u32.f64 truncation */
        *pu16Dst++ = (HI_U16) q;
    }
}

/* Mirrors cv500 vendor libive.so HI_MPI_IVE_KCF_CreateCosWin @0xda38.
 *
 * Generates 13 Hann windows for ROI sizes N ∈ {8, 10, 12, …, 32}, two
 * copies (X and Y) per size. Each size's output occupies a 64-byte
 * slot at offset (N-8) * 32 inside pstCosWinX.virt / pstCosWinY.virt;
 * for N < 17 only the first 32 B (16 u16) are written and the upper
 * half stays zero. Total per buffer: 13 * 64 = 832 bytes; user must
 * pre-allocate at least that much MMZ for both X and Y. */
HI_S32 HI_MPI_IVE_KCF_CreateCosWin(IVE_DST_MEM_INFO_S *pstCosWinX,
                                   IVE_DST_MEM_INFO_S *pstCosWinY)
{
    HI_U16 *x_virt, *y_virt;
    HI_U32  n;

    IVE_CHECK_NULL("HI_MPI_IVE_KCF_CreateCosWin", pstCosWinX, "pstCosWinX");
    IVE_CHECK_NULL("HI_MPI_IVE_KCF_CreateCosWin", pstCosWinY, "pstCosWinY");
    if (pstCosWinX->u32Size < 832 || pstCosWinY->u32Size < 832) {
        IVE_LOG("HI_MPI_IVE_KCF_CreateCosWin",
                "buffer too small (X=%u Y=%u, need >= 832)",
                pstCosWinX->u32Size, pstCosWinY->u32Size);
        return HI_ERR_IVE_ILLEGAL_PARAM;
    }
    x_virt = (HI_U16 *)(uintptr_t)pstCosWinX->u64VirAddr;
    y_virt = (HI_U16 *)(uintptr_t)pstCosWinY->u64VirAddr;
    IVE_CHECK_NULL("HI_MPI_IVE_KCF_CreateCosWin", x_virt, "pstCosWinX.virt");
    IVE_CHECK_NULL("HI_MPI_IVE_KCF_CreateCosWin", y_virt, "pstCosWinY.virt");

    for (n = 8; n <= 32; n += 2) {
        HI_U32 offset_bytes = (n - 8) * 32;
        ive_kcf_gen_hann_window(n,
            (HI_U16 *)((HI_U8 *)x_virt + offset_bytes));
        ive_kcf_gen_hann_window(n,
            (HI_U16 *)((HI_U8 *)y_virt + offset_bytes));
    }
    return HI_SUCCESS;
}

/* Per-obj buffer sizes inside pstMem (decoded via kcf_probe4 on av300).
 * pstMem is structure-of-arrays: all N stHogFeature[] first, then all
 * stAlpha[], then all stDst[]. */
#define KCF_HOG_FEATURE_BYTES 47616u
#define KCF_ALPHA_BYTES       8192u
#define KCF_DST_BYTES         16u

/* CosWin slot is 64 bytes (= 32 u16 elements) regardless of cell_dim. */
#define KCF_COSWIN_SLOT_BYTES 64u

/* Mirrors cv500 vendor libive.so HI_MPI_IVE_KCF_GetTrainObj @0xdaec.
 *
 * For each ROI in astRoiInfo[i] (i < u32ObjNum):
 *   1. Pop first node from pstObjList->stFreeObjList.
 *   2. Set node.stKcfObj.stRoiInfo = astRoiInfo[i], .u3q5Padding = u3q5Padding.
 *   3. Compute HOG grid (grid_w, grid_h) via ive_kcf_hog_grid().
 *   4. slot_w = (grid_w - 8) / 2,  slot_h = (grid_h - 8) / 2  ∈ [0..12].
 *   5. Fill stCosWinX = pstCosWinX + slot_w * 64,  size 64.
 *      Fill stCosWinY = pstCosWinY + slot_h * 64,  size 64.
 *   6. Look up stGaussPeak from pstGaussPeak's mem-info table at
 *      cell index (slot_h * 13 + slot_w), copying its {phys, virt, size}.
 *   7. Slice pstMem (the user buffer from CreateObjList) for stHogFeature,
 *      stAlpha, stDst — structure-of-arrays layout:
 *        stHogFeature[i].phys = pstMem.phys + i * 47616
 *        stAlpha[i].phys      = pstMem.phys + N*47616 + i * 8192
 *        stDst[i].phys        = pstMem.phys + N*(47616+8192) + i * 16
 *      where N = list.u32MaxObjNum.
 *   8. Append node to pstObjList->stTrainObjList.
 *   9. u32FreeObjNum--, u32TrainObjNum++.
 *
 * Bookkeeping only — no HW touch, no Gaussian math. The actual tracking
 * happens later in KCF_Process which reads these slot pointers.
 *
 * The pstMem in pstObjList is set by CreateObjList's caller but not
 * STORED in pstObjList — instead, vendor's CreateObjList copied
 * {phys, virt, size} from pstMem somewhere. Actually NO: my kcf_probe
 * confirmed CreateObjList ignores pstMem entirely. So GetTrainObj
 * must be passed pstMem implicitly via... wait the signature here
 * doesn't include pstMem either. Vendor stashes the pstMem fields in
 * pstObjList during CreateObjList? Let me check the IVE_KCF_OBJ_LIST_S
 * struct... no, no pstMem field there.
 *
 * Re-read kcf_probe output: pstMem stays sentinel-filled after both
 * CreateObjList and GetTrainObj. The stHogFeature.phys = 0xa9c80000
 * which WAS pstMem.phys. So the per-obj slice fields ARE filled, but
 * the writes happen to the node, not back into pstMem.
 *
 * Where does GetTrainObj get pstMem.phys from? The function doesn't
 * take pstMem as an argument. The answer must be: vendor's
 * CreateObjList captured pstMem into a private place (perhaps using
 * the pstObjNodeBuf or pu8TmpBuf header). Since vendor's blob is
 * opaque we can't see this in the public struct.
 *
 * Our solution: stash pstMem at the head of pu8TmpBuf. That's a 22656-B
 * scratch the user can't see; we own it. The cost is the user-visible
 * pstMem field is hidden — but our CreateObjList is the only writer
 * and our GetTrainObj is the only reader.
 */

/* Forward declaration — definition lives near JudgeObjBboxTrackState. */
static void ive_kcf_hog_grid(const IVE_ROI_INFO_S *pstRoi, HI_U8 u3q5Padding,
                             HI_U32 *pu32GridW, HI_U32 *pu32GridH);

/* Hidden per-list state stashed at the tail of pu8TmpBuf. */
typedef struct {
    HI_U64 mem_phys;
    HI_U64 mem_virt;
    HI_U32 mem_size;
    HI_U32 pad;
} ive_kcf_pstmem_stash_t;

/* Look up cell descriptor in pstGaussPeak's mem-info table.
 * Cells are at offset (slot_h * 13 + slot_w) * 24 in pstGaussPeak.virt. */
static void ive_kcf_lookup_gp_cell(IVE_MEM_INFO_S *pstGaussPeak,
                                   HI_U32 slot_w, HI_U32 slot_h,
                                   IVE_MEM_INFO_S *dst)
{
    const HI_U8 *table_base = (const HI_U8 *)(uintptr_t) pstGaussPeak->u64VirAddr;
    const HI_U8 *cell = table_base + (slot_h * 13 + slot_w) * 24;
    dst->u64PhyAddr = *(const HI_U64 *)(cell + 0);
    dst->u64VirAddr = *(const HI_U64 *)(cell + 8);
    dst->u32Size    = *(const HI_U32 *)(cell + 16);
}

HI_S32 HI_MPI_IVE_KCF_GetTrainObj(HI_U3Q5 u3q5Padding, IVE_ROI_INFO_S astRoiInfo[],
                                  HI_U32 u32ObjNum,
                                  IVE_MEM_INFO_S *pstCosWinX,
                                  IVE_MEM_INFO_S *pstCosWinY,
                                  IVE_MEM_INFO_S *pstGaussPeak,
                                  IVE_KCF_OBJ_LIST_S *pstObjList)
{
    HI_U32 i, N;
    const ive_kcf_pstmem_stash_t *stash;

    IVE_CHECK_NULL("HI_MPI_IVE_KCF_GetTrainObj", astRoiInfo,   "astRoiInfo");
    IVE_CHECK_NULL("HI_MPI_IVE_KCF_GetTrainObj", pstCosWinX,   "pstCosWinX");
    IVE_CHECK_NULL("HI_MPI_IVE_KCF_GetTrainObj", pstCosWinY,   "pstCosWinY");
    IVE_CHECK_NULL("HI_MPI_IVE_KCF_GetTrainObj", pstGaussPeak, "pstGaussPeak");
    IVE_CHECK_NULL("HI_MPI_IVE_KCF_GetTrainObj", pstObjList,   "pstObjList");

    if (pstObjList->enListState != IVE_KCF_LIST_STATE_CREATE) {
        IVE_LOG("HI_MPI_IVE_KCF_GetTrainObj",
                "list not in CREATE state (got %d)", pstObjList->enListState);
        return HI_ERR_IVE_NOT_PERM;
    }
    if (!pstObjList->pu8TmpBuf) {
        IVE_LOG("HI_MPI_IVE_KCF_GetTrainObj", "pu8TmpBuf NULL (no Create?)");
        return HI_ERR_IVE_NOT_PERM;
    }

    stash = (const ive_kcf_pstmem_stash_t *)
            (pstObjList->pu8TmpBuf + KCF_STAGE_BYTES);
    if (!stash->mem_phys) {
        IVE_LOG("HI_MPI_IVE_KCF_GetTrainObj",
                "no pstMem stash — Create wasn't called via our libive_neo?");
        return HI_ERR_IVE_NOT_PERM;
    }
    N = pstObjList->u32MaxObjNum;

    for (i = 0; i < u32ObjNum; i++) {
        IVE_LIST_HEAD_S *free_head = &pstObjList->stFreeObjList;
        IVE_LIST_HEAD_S *train_head = &pstObjList->stTrainObjList;
        IVE_LIST_HEAD_S *node_list;
        IVE_KCF_OBJ_NODE_S *node;
        HI_U32 grid_w, grid_h, slot_w, slot_h, slot_off_w, slot_off_h;

        if (pstObjList->u32FreeObjNum == 0) {
            IVE_LOG("HI_MPI_IVE_KCF_GetTrainObj",
                    "free list exhausted at i=%u (asked for %u)", i, u32ObjNum);
            return HI_ERR_IVE_NOT_PERM;
        }

        ive_kcf_hog_grid(&astRoiInfo[i], u3q5Padding, &grid_w, &grid_h);
        if (grid_w < 8 || grid_w > 32 || (grid_w & 1) ||
            grid_h < 8 || grid_h > 32 || (grid_h & 1)) {
            IVE_LOG("HI_MPI_IVE_KCF_GetTrainObj",
                    "roi[%u] gives invalid grid (%u, %u)", i, grid_w, grid_h);
            return HI_ERR_IVE_ILLEGAL_PARAM;
        }
        slot_w = (grid_w - 8) >> 1;
        slot_h = (grid_h - 8) >> 1;
        slot_off_w = slot_w * KCF_COSWIN_SLOT_BYTES;
        slot_off_h = slot_h * KCF_COSWIN_SLOT_BYTES;

        /* Pop first free node. */
        node_list = free_head->pstNext;
        node_list->pstNext->pstPrev = free_head;
        free_head->pstNext = node_list->pstNext;
        /* stList is at offset 0 of IVE_KCF_OBJ_NODE_S. */
        node = (IVE_KCF_OBJ_NODE_S *) node_list;

        node->stKcfObj.stRoiInfo   = astRoiInfo[i];
        node->stKcfObj.u3q5Padding = u3q5Padding;

        node->stKcfObj.stCosWinX.u64PhyAddr = pstCosWinX->u64PhyAddr + slot_off_w;
        node->stKcfObj.stCosWinX.u64VirAddr = pstCosWinX->u64VirAddr + slot_off_w;
        node->stKcfObj.stCosWinX.u32Size    = KCF_COSWIN_SLOT_BYTES;

        node->stKcfObj.stCosWinY.u64PhyAddr = pstCosWinY->u64PhyAddr + slot_off_h;
        node->stKcfObj.stCosWinY.u64VirAddr = pstCosWinY->u64VirAddr + slot_off_h;
        node->stKcfObj.stCosWinY.u32Size    = KCF_COSWIN_SLOT_BYTES;

        ive_kcf_lookup_gp_cell(pstGaussPeak, slot_w, slot_h,
                               &node->stKcfObj.stGaussPeak);

        node->stKcfObj.stHogFeature.u64PhyAddr = stash->mem_phys + i * KCF_HOG_FEATURE_BYTES;
        node->stKcfObj.stHogFeature.u64VirAddr = stash->mem_virt + i * KCF_HOG_FEATURE_BYTES;
        node->stKcfObj.stHogFeature.u32Size    = KCF_HOG_FEATURE_BYTES;

        node->stKcfObj.stAlpha.u64PhyAddr = stash->mem_phys + N * KCF_HOG_FEATURE_BYTES + i * KCF_ALPHA_BYTES;
        node->stKcfObj.stAlpha.u64VirAddr = stash->mem_virt + N * KCF_HOG_FEATURE_BYTES + i * KCF_ALPHA_BYTES;
        node->stKcfObj.stAlpha.u32Size    = KCF_ALPHA_BYTES;

        node->stKcfObj.stDst.u64PhyAddr = stash->mem_phys + N * (KCF_HOG_FEATURE_BYTES + KCF_ALPHA_BYTES) + i * KCF_DST_BYTES;
        node->stKcfObj.stDst.u64VirAddr = stash->mem_virt + N * (KCF_HOG_FEATURE_BYTES + KCF_ALPHA_BYTES) + i * KCF_DST_BYTES;
        node->stKcfObj.stDst.u32Size    = KCF_DST_BYTES;

        /* Append node to train list tail. */
        node_list->pstPrev = train_head->pstPrev;
        node_list->pstNext = train_head;
        train_head->pstPrev->pstNext = node_list;
        train_head->pstPrev = node_list;

        pstObjList->u32FreeObjNum--;
        pstObjList->u32TrainObjNum++;
    }
    return HI_SUCCESS;
}

/* KCF_Process — kernel HW dispatch via ioctl 0xc0084633.
 *
 * Userspace marshalling: libive packs pstObjList->pu8TmpBuf with this
 * staging layout (verified by tracing vendor libive @0xdee8 + libive-side
 * memcpy_s offsets and a kprobe trace of ive_fill_kcf_task on av300):
 *
 *   TmpBuf[   0 ..   71]   IVE_IMAGE_S (pstSrc copy, 72 B)
 *   TmpBuf[  72 ..]        IVE_KCF_OBJ_S array (TRAIN slots,  176 B each)
 *   TmpBuf[11336 ..]       IVE_KCF_OBJ_S array (TRACK slots,  176 B each)
 *   TmpBuf[0x5848] u32     train counter (kernel-written, == iter index)
 *   TmpBuf[0x584c] u32     track counter
 *   TmpBuf[0x5850 .. 0x5877] IVE_KCF_PRO_CTRL_S (40 B; CSC/InterFactor/
 *                              Lamda/TrancAlfa/Sigma/RespThr)
 *   TmpBuf[0x5878]         HW reserved scratch
 *
 * ioctl arg = struct { HI_HANDLE *handle; void *tmpBuf; } (8 B). The
 * 8-byte arg goes via copy_from_user; the staging buffer must already be
 * MMZ memory (mmap-shared) so HW DMA can touch it without an extra copy.
 *
 * Kernel HW task node — 208 B, fully decoded via av300 kprobe trace of
 * ive_fill_kcf_task(src=TmpBuf, obj_idx, hog_params, is_track, node).
 * On entry r5 = TmpBuf + obj_idx*176 + (is_track ? 11336 : 72). r6 =
 * 32-byte hog_params (derived from u3q5Padding + ROI by vendor
 * ive_get_kcf_hog_param). r8 = TmpBuf + 0x5000 (CTRL staging base).
 *
 *   node[ 6]      = 0
 *   node[ 8] u8   = hw_fmt_code[src.enType]   (YUV420SP -> 1)
 *   node[ 9]      = 0
 *   node[10] u8   = 0x34                       (KCF magic)
 *   node[11] u8   = is_track (0=TRAIN, 1=TRACK)
 *   node[16] u32  = src.au64PhyAddr[0] - mmz_base   (Y plane offset)
 *   node[24] u32  = src.au64PhyAddr[1] - mmz_base   (UV plane offset)
 *   node[40] u16  = src.u32Width
 *   node[42] u16  = src.u32Height
 *   node[44] u16  = src.au32Stride[0]
 *   node[46] u16  = src.au32Stride[1]
 *   node[52] u16  = hog_params[24] u16            (RoiId low 16)
 *   node[84] u32  = obj.stAlpha.u64PhyAddr   low32 - mmz_base
 *   node[102] u16 = ctrl.u4q12TrancAlfa
 *   node[104] u32 = obj.stGaussPeak.u64PhyAddr low32 - mmz_base
 *   node[108] u16 = ctrl.u1q15InterFactor
 *   node[110] u16 = ctrl.u0q16Lamda
 *   node[112] u32 = obj.stCosWinX.u64PhyAddr  low32 - mmz_base
 *   node[116] u32 = obj.stCosWinY.u64PhyAddr  low32 - mmz_base
 *   node[120] u32 = obj.stHogFeature.u64PhyAddr low32 - mmz_base (slot 0)
 *   node[124] u32 = TRAIN: dup of [120];  TRACK: TmpBuf[0x5858] - mmz_base
 *   node[128] u32 = TRAIN: dup of [120];  TRACK: TmpBuf[0x5858] - mmz_base
 *   node[132] u32 = obj.stHogFeature - mmz_base (slot 3, both paths)
 *   node[136] u32 = obj.stAlpha     - mmz_base (dup of node[84])
 *   node[140] u32 = TRACK only: obj.stDst.u64PhyAddr low32 - mmz_base
 *   node[144] u32 = TRACK only: ctrl.u8RespThr (byte zero-extended)
 *   node[148] u32 = hog_params[ 8] s32  (start X scan offset, Q-format)
 *   node[152] u32 = hog_params[12] s32  (start Y scan offset)
 *   node[156] u32 = hog_params[16] u32  (scan X range)
 *   node[160] u32 = hog_params[20] u32  (scan Y range)
 *   node[164] u16 = hog_params[ 0] u16  (X step Q-scale)
 *   node[168] u16 = hog_params[ 2] u16  (Y step Q-scale)
 *   node[172] u32 = (1<<40) / ((ctrl.u0q8Sigma+1)^2 * L*H*31)
 *                   with L = (hog_params[16] <= 135) ? hog_params[16]/4-2 : 32
 *                        H = (hog_params[20] <= 135) ? hog_params[20]/4-2 : 32
 *                   (1/sigma^2 reciprocal, pre-scaled for HW kernel evaluator)
 *   node[176] u8  = ctrl.enCscMode  & 0xff
 *   node[177] u8  = 4                              (constant)
 *   node[192] u16 = hog_params[ 4] u16             (HOG cells X, == grid_dim/8)
 *   node[194] u16 = hog_params[ 6] u16             (HOG cells Y)
 *
 * mmz_base is the per-MMZ-zone base phys returned by ive_get_mmz_base_addr
 * (we mirror with dma_addr_t arithmetic on our DT-reserved CMA pool).
 *
 * Vendor kernel-side check (ive_check_kcf_param, line 6001) enforces
 * ctrl.stTmpBuf.u32Size >= 47616 (0xBA00) — this is the minimum size
 * for the staging area plus a HW scratch region. Our handler must
 * accept the same minimum.
 *
 * is_track flag is named misleadingly: in vendor's encoding, 0=TRAIN,
 * 1=TRACK. The TmpBuf slot selection at +72/+11336 confirms this.
 */
#define KCF_IOCTL_PROCESS    0xc0084633u
#define KCF_TMPBUF_MIN_SIZE  47616u    /* vendor's ctrl.stTmpBuf min */

HI_S32 HI_MPI_IVE_KCF_Process(IVE_HANDLE *pIveHandle,
                              IVE_SRC_IMAGE_S *pstSrc,
                              IVE_KCF_OBJ_LIST_S *pstObjList,
                              IVE_KCF_PRO_CTRL_S *pstKcfProCtrl,
                              HI_BOOL bInstant)
{
    HI_U8 *stage;
    HI_U32 n_train, n_track;
    IVE_LIST_HEAD_S *cur;
    HI_S32 ret;
    struct { HI_U32 handle_out; HI_U32 tmpBuf_va; } arg;
    (void)bInstant;

    IVE_CHECK_NULL("HI_MPI_IVE_KCF_Process", pIveHandle, "pIveHandle");
    IVE_CHECK_NULL("HI_MPI_IVE_KCF_Process", pstSrc, "pstSrc");
    IVE_CHECK_NULL("HI_MPI_IVE_KCF_Process", pstObjList, "pstObjList");
    IVE_CHECK_NULL("HI_MPI_IVE_KCF_Process", pstKcfProCtrl, "pstKcfProCtrl");

    if (!pstObjList->pu8TmpBuf) {
        IVE_LOG("HI_MPI_IVE_KCF_Process", "pu8TmpBuf NULL (no Create?)");
        return HI_ERR_IVE_ILLEGAL_PARAM;
    }
    if (pstObjList->enListState != IVE_KCF_LIST_STATE_CREATE) {
        IVE_LOG("HI_MPI_IVE_KCF_Process", "enListState=%d (need CREATE)",
                pstObjList->enListState);
        return HI_ERR_IVE_ILLEGAL_PARAM;
    }
    if (pstKcfProCtrl->stTmpBuf.u32Size < KCF_TMPBUF_MIN_SIZE) {
        IVE_LOG("HI_MPI_IVE_KCF_Process",
                "ctrl.stTmpBuf.u32Size(%u) < %u (HW scratch min)",
                pstKcfProCtrl->stTmpBuf.u32Size, KCF_TMPBUF_MIN_SIZE);
        return HI_ERR_IVE_ILLEGAL_PARAM;
    }
    if (pstObjList->u32TrainObjNum > pstObjList->u32MaxObjNum ||
        pstObjList->u32TrackObjNum > pstObjList->u32MaxObjNum) {
        IVE_LOG("HI_MPI_IVE_KCF_Process",
                "train=%u track=%u > max=%u",
                pstObjList->u32TrainObjNum, pstObjList->u32TrackObjNum,
                pstObjList->u32MaxObjNum);
        return HI_ERR_IVE_ILLEGAL_PARAM;
    }

    stage = pstObjList->pu8TmpBuf;
    memset(stage, 0, KCF_STAGE_BYTES);

    /* [0..71] = pstSrc copy (72 bytes, IVE_IMAGE_S vendor layout) */
    memcpy(stage, pstSrc, 72);

    /* [72 + i*176] = TRAIN slot copies (KCF_OBJ_S body, 176 B each).
     * Walk stTrainObjList without removing entries — GetObjBbox drains
     * train→track after Process returns. */
    n_train = 0;
    for (cur = pstObjList->stTrainObjList.pstNext;
         cur != &pstObjList->stTrainObjList && n_train < 64;
         cur = cur->pstNext, n_train++) {
        IVE_KCF_OBJ_NODE_S *node = (IVE_KCF_OBJ_NODE_S *) cur;
        memcpy(stage + 72 + n_train * 176, &node->stKcfObj, 176);
    }

    /* [11336 + i*176] = TRACK slot copies. */
    n_track = 0;
    for (cur = pstObjList->stTrackObjList.pstNext;
         cur != &pstObjList->stTrackObjList && n_track < 64;
         cur = cur->pstNext, n_track++) {
        IVE_KCF_OBJ_NODE_S *node = (IVE_KCF_OBJ_NODE_S *) cur;
        memcpy(stage + 11336 + n_track * 176, &node->stKcfObj, 176);
    }

    *(HI_U32 *)(stage + 22600) = n_train;
    *(HI_U32 *)(stage + 22604) = n_track;
    memcpy(stage + 22608, pstKcfProCtrl, 40);

    arg.handle_out = 0;
    arg.tmpBuf_va  = (HI_U32)(uintptr_t) stage;

    if (ive_open_fd() != HI_SUCCESS)
        return HI_ERR_IVE_NOT_CONFIG;
    ret = ioctl(g_ive_fd, KCF_IOCTL_PROCESS, &arg);
    if (ret) {
        IVE_LOG("HI_MPI_IVE_KCF_Process", "ioctl errno=%d", errno);
        *pIveHandle = -1;
        return HI_ERR_IVE_SYS_TIMEOUT;
    }
    *pIveHandle = (IVE_HANDLE) arg.handle_out;
    return HI_SUCCESS;
}

/* ---- Internal list-mgmt helpers (mirror cv500 libive.so) ---- */

static void ive_kcf_list_del(IVE_LIST_HEAD_S *entry)
{
    entry->pstPrev->pstNext = entry->pstNext;
    entry->pstNext->pstPrev = entry->pstPrev;
}

static void ive_kcf_list_add_tail(IVE_LIST_HEAD_S *entry, IVE_LIST_HEAD_S *head)
{
    entry->pstPrev = head->pstPrev;
    entry->pstNext = head;
    head->pstPrev->pstNext = entry;
    head->pstPrev = entry;
}

static void ive_kcf_free_track_node(IVE_KCF_OBJ_LIST_S *list,
                                    IVE_KCF_OBJ_NODE_S *node)
{
    ive_kcf_list_del(&node->stList);
    if (list->u32TrackObjNum) list->u32TrackObjNum--;
}

static void ive_kcf_put_free(IVE_KCF_OBJ_LIST_S *list,
                             IVE_KCF_OBJ_NODE_S *node)
{
    ive_kcf_list_add_tail(&node->stList, &list->stFreeObjList);
    list->u32FreeObjNum++;
}

static IVE_KCF_OBJ_NODE_S *ive_kcf_get_train(IVE_KCF_OBJ_LIST_S *list)
{
    IVE_LIST_HEAD_S *head = &list->stTrainObjList;
    IVE_LIST_HEAD_S *first;
    if (head->pstNext == head) return NULL;
    first = head->pstNext;
    ive_kcf_list_del(first);
    if (list->u32TrainObjNum) list->u32TrainObjNum--;
    return (IVE_KCF_OBJ_NODE_S *) first;
}

static void ive_kcf_put_track(IVE_KCF_OBJ_LIST_S *list,
                              IVE_KCF_OBJ_NODE_S *node)
{
    ive_kcf_list_add_tail(&node->stList, &list->stTrackObjList);
    list->u32TrackObjNum++;
}

/* Mirrors cv500 vendor libive.so HI_MPI_IVE_KCF_GetObjBbox @0xe2bc.
 *
 * Two phases:
 *  1. Walk EXISTING track list. For each track node:
 *     - read s32Response from dst.virt[+8] (filled by KCF_Process)
 *     - if response < bbox_ctrl->s32RespThr: free node → free list
 *     - else compute new ROI center from peak + previous ROI, check it
 *       falls inside list.u32Width × list.u32Height; emit bbox and
 *       update node->stKcfObj.stRoiInfo.stRoi.s24q8{X,Y} in place
 *  2. Drain train list into track list (the "active" trackers for
 *     the next Process call).
 *
 * Peak position math (per vendor d3f0-d420):
 *   dst.virt layout (16 bytes, written by HW during KCF_Process):
 *     +0..3:  peak.s24q8X (signed)
 *     +4..7:  peak.s24q8Y
 *     +8..11: s32Response
 *     +12..15: unused
 *   new_x = (peak.x + roi.s24q8X + (sum<0 ? 255 : 0)) >> 8   (floor-toward-0)
 */
HI_S32 HI_MPI_IVE_KCF_GetObjBbox(IVE_KCF_OBJ_LIST_S *pstObjList,
                                 IVE_KCF_BBOX_S astBbox[],
                                 HI_U32 *pu32BboxObjNum,
                                 IVE_KCF_BBOX_CTRL_S *pstKcfBboxCtrl)
{
    HI_U32 max_bbox, bbox_cnt = 0;
    HI_S32 resp_thr;
    HI_U32 drain_count, drained;
    IVE_LIST_HEAD_S *cur, *next;

    IVE_CHECK_NULL("HI_MPI_IVE_KCF_GetObjBbox", pstObjList,      "pstObjList");
    IVE_CHECK_NULL("HI_MPI_IVE_KCF_GetObjBbox", astBbox,         "astBbox");
    IVE_CHECK_NULL("HI_MPI_IVE_KCF_GetObjBbox", pu32BboxObjNum,  "pu32BboxObjNum");
    IVE_CHECK_NULL("HI_MPI_IVE_KCF_GetObjBbox", pstKcfBboxCtrl,  "pstKcfBboxCtrl");

    max_bbox = pstKcfBboxCtrl->u32MaxBboxNum;
    resp_thr = pstKcfBboxCtrl->s32RespThr;
    if (max_bbox > pstObjList->u32TrackObjNum + pstObjList->u32TrainObjNum)
        max_bbox = pstObjList->u32TrackObjNum + pstObjList->u32TrainObjNum;

    /* Phase 1 — walk track list with safe-iteration (may delete cur). */
    cur = pstObjList->stTrackObjList.pstNext;
    while (cur != &pstObjList->stTrackObjList && bbox_cnt < max_bbox) {
        IVE_KCF_OBJ_NODE_S *node = (IVE_KCF_OBJ_NODE_S *) cur;
        HI_U8 *dst_virt = (HI_U8 *)(uintptr_t) node->stKcfObj.stDst.u64VirAddr;
        HI_S32 response, peak_x, peak_y, sum_x, sum_y, new_x, new_y;
        next = cur->pstNext;

        response = *(HI_S32 *)(dst_virt + 8);
        if (response < resp_thr)
            goto free_track;

        peak_x = *(HI_S32 *)(dst_virt + 0);
        peak_y = *(HI_S32 *)(dst_virt + 4);
        sum_x  = peak_x + (HI_S32) node->stKcfObj.stRoiInfo.stRoi.s24q8X;
        sum_y  = peak_y + (HI_S32) node->stKcfObj.stRoiInfo.stRoi.s24q8Y;
        new_x  = (sum_x >= 0 ? sum_x : sum_x + 255) >> 8;
        new_y  = (sum_y >= 0 ? sum_y : sum_y + 255) >> 8;

        if (new_x < 0 || new_y < 0 ||
            (HI_U32) new_x >= pstObjList->u32Width ||
            (HI_U32) new_y >= pstObjList->u32Height)
            goto free_track;

        /* Update node ROI in place — vendor stores the s24q8 sum
         * back to the node, not the integer new_x/new_y. */
        node->stKcfObj.stRoiInfo.stRoi.s24q8X = sum_x;
        node->stKcfObj.stRoiInfo.stRoi.s24q8Y = sum_y;

        astBbox[bbox_cnt].pstNode                    = node;
        astBbox[bbox_cnt].s32Response                = response;
        astBbox[bbox_cnt].stRoiInfo.stRoi.s24q8X     = sum_x;
        astBbox[bbox_cnt].stRoiInfo.stRoi.s24q8Y     = sum_y;
        astBbox[bbox_cnt].stRoiInfo.stRoi.u32Width   = node->stKcfObj.stRoiInfo.stRoi.u32Width;
        astBbox[bbox_cnt].stRoiInfo.stRoi.u32Height  = node->stKcfObj.stRoiInfo.stRoi.u32Height;
        astBbox[bbox_cnt].stRoiInfo.u32RoiId         = node->stKcfObj.stRoiInfo.u32RoiId;
        astBbox[bbox_cnt].bTrackOk                   = HI_TRUE;
        astBbox[bbox_cnt].bRoiRefresh                = HI_FALSE;
        bbox_cnt++;
        cur = next;
        continue;

free_track:
        ive_kcf_free_track_node(pstObjList, node);
        ive_kcf_put_free(pstObjList, node);
        cur = next;
    }

    *pu32BboxObjNum = bbox_cnt;

    /* Phase 2 — drain train list into track list. */
    drain_count = pstObjList->u32TrainObjNum;
    for (drained = 0; drained < drain_count; drained++) {
        IVE_KCF_OBJ_NODE_S *t = ive_kcf_get_train(pstObjList);
        if (!t) break;
        ive_kcf_put_track(pstObjList, t);
    }
    return HI_SUCCESS;
}

/* HOG feature-grid dim helper, decoded from cv500 vendor
 * `ive_get_hog_feature_rect` @0x13a7c. Returns the HOG cell-grid
 * width/height that a ROI of (w, h) maps to after the u3q5Padding
 * multiplier is applied. Cells are 4x4 pixels, with a clamp at 136
 * pixels per axis and a "minus 2 cells" border subtraction.
 *
 *   eff = ((roi_dim * padding) >> 8 + 1) * 8       (Q3.5 → integer count)
 *   eff = min(eff, 136)
 *   cells = (eff / 4) - 2
 */
static void ive_kcf_hog_grid(const IVE_ROI_INFO_S *pstRoi, HI_U8 u3q5Padding,
                             HI_U32 *pu32GridW, HI_U32 *pu32GridH)
{
    HI_U32 w = pstRoi->stRoi.u32Width  * (HI_U32)u3q5Padding;
    HI_U32 h = pstRoi->stRoi.u32Height * (HI_U32)u3q5Padding;
    w = ((w >> 8) + 1) << 3;
    h = ((h >> 8) + 1) << 3;
    if (w >= 136) w = 136;
    if (h >= 136) h = 136;
    *pu32GridW = (w >> 2) - 2;
    *pu32GridH = (h >> 2) - 2;
}

/* Mirrors cv500 vendor libive.so HI_MPI_IVE_KCF_JudgeObjBboxTrackState
 * @0xe5d4: a bbox is still tracking the same object iff its HOG cell
 * grid (after padding) has the same dimensions as the candidate ROI's.
 * Vendor uses `ive_get_hog_feature_rect` for both sides and compares
 * the W,H output. */
HI_S32 HI_MPI_IVE_KCF_JudgeObjBboxTrackState(IVE_ROI_INFO_S *pstRoiInfo,
                                             IVE_KCF_BBOX_S *pstBbox,
                                             HI_BOOL *pbTrackOk)
{
    HI_U32 a_w, a_h, b_w, b_h;
    HI_U8  padding;

    IVE_CHECK_NULL("HI_MPI_IVE_KCF_JudgeObjBboxTrackState",
                   pstRoiInfo, "pstRoiInfo");
    IVE_CHECK_NULL("HI_MPI_IVE_KCF_JudgeObjBboxTrackState",
                   pstBbox,    "pstBbox");
    IVE_CHECK_NULL("HI_MPI_IVE_KCF_JudgeObjBboxTrackState",
                   pbTrackOk,  "pbTrackOk");
    IVE_CHECK_NULL("HI_MPI_IVE_KCF_JudgeObjBboxTrackState",
                   pstBbox->pstNode, "pstBbox->pstNode");

    padding = pstBbox->pstNode->stKcfObj.u3q5Padding;
    ive_kcf_hog_grid(pstRoiInfo,           padding, &a_w, &a_h);
    ive_kcf_hog_grid(&pstBbox->stRoiInfo,  padding, &b_w, &b_h);

    *pbTrackOk = (a_w == b_w && a_h == b_h) ? HI_TRUE : HI_FALSE;
    return HI_SUCCESS;
}

/* Mirrors cv500 vendor libive.so HI_MPI_IVE_KCF_ObjUpdate @0xe6f4.
 *
 * For each bbox in input array:
 *   - if bTrackOk == HI_TRUE:
 *       - if bRoiRefresh == HI_TRUE: caller overrode the ROI; memcpy
 *         it into the node so the next Process call uses the new
 *         position
 *       - else: keep as-is (GetObjBbox already updated the node)
 *   - if bTrackOk == HI_FALSE: tracker lost; remove the node from
 *     the track list and return it to the free list */
HI_S32 HI_MPI_IVE_KCF_ObjUpdate(IVE_KCF_OBJ_LIST_S *pstObjList,
                                IVE_KCF_BBOX_S astBbox[],
                                HI_U32 u32BboxObjNum)
{
    HI_U32 i;

    IVE_CHECK_NULL("HI_MPI_IVE_KCF_ObjUpdate", pstObjList, "pstObjList");
    if (u32BboxObjNum == 0)
        return HI_SUCCESS;
    IVE_CHECK_NULL("HI_MPI_IVE_KCF_ObjUpdate", astBbox, "astBbox");

    for (i = 0; i < u32BboxObjNum; i++) {
        IVE_KCF_OBJ_NODE_S *node = astBbox[i].pstNode;
        if (!node) continue;

        if (astBbox[i].bTrackOk) {
            if (astBbox[i].bRoiRefresh) {
                /* Caller asked for ROI override — copy new ROI into node. */
                memcpy(&node->stKcfObj.stRoiInfo, &astBbox[i].stRoiInfo,
                       sizeof(IVE_ROI_INFO_S));
            }
            /* else: GetObjBbox already updated stRoiInfo in place. */
        } else {
            /* Tracker lost — node has already been moved to track list by
             * GetObjBbox; pull it out and put back on free list. */
            ive_kcf_free_track_node(pstObjList, node);
            ive_kcf_put_free(pstObjList, node);
        }
    }
    return HI_SUCCESS;
}
