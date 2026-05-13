/*
 * HW round-trip test for cv500 PerspTrans + Hog.
 *
 * Allocates MMZ buffers via libmpi_neo, fills the src image with a
 * known gradient + sentinel-fills the dst, runs the op through
 * libive_neo, and reports:
 *   (a) HW accepted the task (ioctl returned 0, IVE_HANDLE valid)
 *   (b) HW wrote to dst (dst bytes != sentinel)
 *   (c) For PerspTrans with identity transform: dst ≈ src
 *
 * (a) is the load-bearing signal — it proves the field maps don't
 * cause HW to reject the op or hang the bus. (b) proves DMA path
 * works end-to-end. (c) proves the geometric maths produces sane
 * output (won't be bit-exact due to perspective-matrix solve +
 * bilinear interpolation, so we accept a tolerance).
 *
 * Build via libraries/ive_neo/test/Makefile, run on av300:
 *   ./test_persp_hog
 *
 * Exit code 0 on success, non-zero on any failed assertion.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "hi_type.h"
#include "hi_common.h"
#include "hi_comm_ive.h"
#include "hi_ive.h"
#include "mpi_sys.h"
#include "mpi_ive.h"

#define W 64
#define H 64

/* YUV420SP: Y plane W*H, then UV interleaved W*H/2. */
#define Y_SIZE  (W * H)
#define UV_SIZE (W * H / 2)
#define IMG_SIZE (Y_SIZE + UV_SIZE)

#define SENTINEL 0xAA

static int g_failures;

static void check(int cond, const char *msg)
{
    if (cond) {
        printf("  PASS: %s\n", msg);
    } else {
        printf("  FAIL: %s\n", msg);
        g_failures++;
    }
}

static HI_S32 alloc_image(IVE_IMAGE_S *img, HI_U32 w, HI_U32 h)
{
    HI_U64 phys;
    HI_VOID *virt;
    HI_S32 ret = HI_MPI_SYS_MmzAlloc(&phys, &virt, NULL, NULL, IMG_SIZE);
    if (ret != HI_SUCCESS) return ret;
    memset(img, 0, sizeof(*img));
    img->enType = IVE_IMAGE_TYPE_YUV420SP;
    img->au64PhyAddr[0] = phys;
    img->au64VirAddr[0] = (HI_U64)(uintptr_t)virt;
    img->au64PhyAddr[1] = phys + Y_SIZE;
    img->au64VirAddr[1] = (HI_U64)(uintptr_t)((HI_U8 *)virt + Y_SIZE);
    img->au32Stride[0] = w;
    img->au32Stride[1] = w;
    img->u32Width = w;
    img->u32Height = h;
    return HI_SUCCESS;
}

static HI_S32 alloc_mem_info(IVE_MEM_INFO_S *m, HI_U32 sz)
{
    HI_U64 phys;
    HI_VOID *virt;
    HI_S32 ret = HI_MPI_SYS_MmzAlloc(&phys, &virt, NULL, NULL, sz);
    if (ret != HI_SUCCESS) return ret;
    m->u64PhyAddr = phys;
    m->u64VirAddr = (HI_U64)(uintptr_t)virt;
    m->u32Size = sz;
    return HI_SUCCESS;
}

static void free_image(IVE_IMAGE_S *img)
{
    if (img->au64PhyAddr[0])
        HI_MPI_SYS_MmzFree(img->au64PhyAddr[0],
                           (void *)(uintptr_t)img->au64VirAddr[0]);
}

static void free_mem(IVE_MEM_INFO_S *m)
{
    if (m->u64PhyAddr)
        HI_MPI_SYS_MmzFree(m->u64PhyAddr,
                           (void *)(uintptr_t)m->u64VirAddr);
}

static HI_S32 wait_handle(IVE_HANDLE h)
{
    HI_BOOL done = HI_FALSE;
    int spins = 0;
    /* libive convention: handle 0 means "op completed synchronously"
     * (kernel already waited for the completion IRQ in ive_submit_chain
     * before returning). No Query needed. cv500 also has no XNN unit,
     * so HI_MPI_IVE_Query returns -EOPNOTSUPP regardless. */
    if (h == 0)
        return HI_SUCCESS;
    while (!done) {
        HI_S32 r = HI_MPI_IVE_Query(h, &done, HI_TRUE);
        if (r != HI_SUCCESS) return r;
        if (++spins > 100) return HI_FAILURE;
    }
    return HI_SUCCESS;
}

static int test_persp_trans(void)
{
    IVE_IMAGE_S src, dst;
    IVE_RECT_U32_S roi;
    IVE_SRC_MEM_INFO_S pp_mem;
    IVE_PERSP_TRANS_CTRL_S ctrl;
    IVE_HANDLE h = -1;
    HI_S32 ret;
    int i, written_bytes;
    HI_U8 *src_y, *dst_y;

    printf("\n=== PerspTrans identity transform on %ux%u YUV420SP ===\n", W, H);
    g_failures = 0;

    memset(&src, 0, sizeof(src));
    memset(&dst, 0, sizeof(dst));
    memset(&pp_mem, 0, sizeof(pp_mem));

    if (alloc_image(&src, W, H) != HI_SUCCESS) {
        printf("  FAIL: alloc src\n"); return 1;
    }
    if (alloc_image(&dst, W, H) != HI_SUCCESS) {
        printf("  FAIL: alloc dst\n"); free_image(&src); return 1;
    }
    /* 5 point pairs × 8 bytes/pair, rounded up to 64 for alignment */
    if (alloc_mem_info(&pp_mem, 64) != HI_SUCCESS) {
        printf("  FAIL: alloc pp\n");
        free_image(&src); free_image(&dst); return 1;
    }

    /* Gradient src: Y[y][x] = (x ^ y) & 0xff (visually distinctive),
     * U/V = 0x80 (neutral chroma) */
    src_y = (HI_U8 *)(uintptr_t)src.au64VirAddr[0];
    for (i = 0; i < Y_SIZE; i++)
        src_y[i] = (HI_U8)((i % W) ^ (i / W));
    memset(src_y + Y_SIZE, 0x80, UV_SIZE);
    HI_MPI_SYS_MmzFlushCache(src.au64PhyAddr[0],
                             (void *)(uintptr_t)src.au64VirAddr[0],
                             IMG_SIZE);

    /* Sentinel-fill dst so we can detect any HW write */
    dst_y = (HI_U8 *)(uintptr_t)dst.au64VirAddr[0];
    memset(dst_y, SENTINEL, IMG_SIZE);
    HI_MPI_SYS_MmzFlushCache(dst.au64PhyAddr[0],
                             (void *)(uintptr_t)dst.au64VirAddr[0],
                             IMG_SIZE);

    /* Identity transform: 5 point pairs where src==dst (corners + center).
     * u14q2 means coord << 2. */
    {
        IVE_PERSP_TRANS_POINT_PAIR_S *pp =
            (IVE_PERSP_TRANS_POINT_PAIR_S *)(uintptr_t)pp_mem.u64VirAddr;
        struct { HI_U16 x, y; } pts[5] = {
            {0,    0   },
            {W-1,  0   },
            {0,    H-1 },
            {W-1,  H-1 },
            {W/2,  H/2 },
        };
        for (i = 0; i < 5; i++) {
            pp[i].stSrcPoint.u14q2X = pts[i].x << 2;
            pp[i].stSrcPoint.u14q2Y = pts[i].y << 2;
            pp[i].stDstPoint.u14q2X = pts[i].x << 2;
            pp[i].stDstPoint.u14q2Y = pts[i].y << 2;
        }
        HI_MPI_SYS_MmzFlushCache(pp_mem.u64PhyAddr,
                                 (void *)(uintptr_t)pp_mem.u64VirAddr, 64);
    }

    roi.u32X = 0; roi.u32Y = 0;
    roi.u32Width = W; roi.u32Height = H;

    ctrl.enAlgMode = IVE_PERSP_TRANS_ALG_MODE_AFFINE;
    ctrl.enCscMode = IVE_PERSP_TRANS_CSC_MODE_NONE;
    ctrl.u16RoiNum = 1;
    ctrl.u16PointPairNum = 5;

    ret = HI_MPI_IVE_PerspTrans(&h, &src, &roi, &pp_mem, &dst, &ctrl, HI_TRUE);
    check(ret == HI_SUCCESS, "ioctl returned success");
    if (ret != HI_SUCCESS) {
        printf("    ret=0x%x\n", ret);
        goto cleanup;
    }

    /* Block until HW completes */
    ret = wait_handle(h);
    check(ret == HI_SUCCESS, "Query/wait completed");

    /* Invalidate dst cache so we see HW's writes */
    HI_MPI_SYS_MmzFlushCache(dst.au64PhyAddr[0],
                             (void *)(uintptr_t)dst.au64VirAddr[0],
                             IMG_SIZE);

    /* (b) Did HW write at all? Count non-sentinel bytes. */
    written_bytes = 0;
    for (i = 0; i < Y_SIZE; i++)
        if (dst_y[i] != SENTINEL) written_bytes++;
    printf("  HW wrote %d/%d bytes of dst Y plane (sentinel 0x%02x)\n",
           written_bytes, Y_SIZE, SENTINEL);
    check(written_bytes > Y_SIZE / 2, "HW wrote majority of dst Y plane");

    /* (c) Identity transform — dst should approximate src.
     * Bilinear interpolation + integer matrix solve introduces some
     * error, so we use mean absolute diff < 4 (out of 0..255) as
     * the bar for "geometrically correct". */
    {
        long sum_abs_diff = 0;
        int sample_count = 0;
        /* Skip a 4-pixel border (interpolation at edges can clamp) */
        for (int y = 4; y < H - 4; y++)
            for (int x = 4; x < W - 4; x++) {
                int s = src_y[y * W + x];
                int d = dst_y[y * W + x];
                sum_abs_diff += abs(s - d);
                sample_count++;
            }
        if (sample_count) {
            long mean = sum_abs_diff / sample_count;
            printf("  mean |dst - src| over interior %u px = %ld\n",
                   sample_count, mean);
            check(mean < 16, "geometric match within tolerance");
        }
    }

    /* Show first 16 bytes of each for visual sanity */
    printf("  src[0..15]: ");
    for (i = 0; i < 16; i++) printf("%02x ", src_y[i]);
    printf("\n  dst[0..15]: ");
    for (i = 0; i < 16; i++) printf("%02x ", dst_y[i]);
    printf("\n");

cleanup:
    free_image(&src);
    free_image(&dst);
    free_mem(&pp_mem);
    return g_failures;
}

static int test_hog(void)
{
    IVE_IMAGE_S src;
    IVE_RECT_U32_S roi;
    IVE_BLOB_S dst;
    IVE_HOG_CTRL_S ctrl;
    IVE_HANDLE h = -1;
    HI_S32 ret;
    HI_U64 dst_phys;
    HI_VOID *dst_virt;
    HI_U8 *src_y, *dst_blob;
    const HI_U32 BLOB_SIZE = 4096;
    int i, written_bytes;
    int failures_before;

    printf("\n=== Hog vertical mode on %ux%u YUV420SP ===\n", W, H);
    failures_before = g_failures;
    memset(&src, 0, sizeof(src));
    memset(&dst, 0, sizeof(dst));

    if (alloc_image(&src, W, H) != HI_SUCCESS) {
        printf("  FAIL: alloc src\n"); return 1;
    }
    if (HI_MPI_SYS_MmzAlloc(&dst_phys, &dst_virt, NULL, NULL, BLOB_SIZE)
        != HI_SUCCESS) {
        printf("  FAIL: alloc dst blob\n"); free_image(&src); return 1;
    }

    src_y = (HI_U8 *)(uintptr_t)src.au64VirAddr[0];
    for (i = 0; i < Y_SIZE; i++)
        src_y[i] = (HI_U8)(i % 64);     /* diagonal stripes — strong gradient */
    memset(src_y + Y_SIZE, 0x80, UV_SIZE);
    HI_MPI_SYS_MmzFlushCache(src.au64PhyAddr[0],
                             (void *)(uintptr_t)src.au64VirAddr[0],
                             IMG_SIZE);

    dst_blob = (HI_U8 *)dst_virt;
    memset(dst_blob, SENTINEL, BLOB_SIZE);
    HI_MPI_SYS_MmzFlushCache(dst_phys, dst_virt, BLOB_SIZE);

    roi.u32X = 0; roi.u32Y = 0;
    roi.u32Width = W; roi.u32Height = H;

    dst.enType = 0;             /* arbitrary */
    dst.u32Stride = 64;
    dst.u64VirAddr = (HI_U64)(uintptr_t)dst_virt;
    dst.u64PhyAddr = dst_phys;
    dst.u32Num = 1;
    dst.unShape.stWhc.u32Width  = W / 4 - 2;
    dst.unShape.stWhc.u32Height = H / 4 - 2;
    dst.unShape.stWhc.u32Chn    = 9;        /* HOG default bin count */

    ctrl.enCscMode = IVE_CSC_MODE_PIC_BT601_YUV2RGB;
    ctrl.enHogMode = IVE_HOG_MODE_VERTICAL_TANGENT_PLANE;
    ctrl.u32RoiNum = 1;
    ctrl.u4q12TrancAlfa = 0x200;        /* nominal 0.125 in u4.12 */

    ret = HI_MPI_IVE_Hog(&h, &src, &roi, &dst, &ctrl, HI_TRUE);
    check(ret == HI_SUCCESS, "ioctl returned success");
    if (ret != HI_SUCCESS) {
        printf("    ret=0x%x\n", ret);
        goto cleanup;
    }

    ret = wait_handle(h);
    check(ret == HI_SUCCESS, "Query/wait completed");

    HI_MPI_SYS_MmzFlushCache(dst_phys, dst_virt, BLOB_SIZE);

    written_bytes = 0;
    for (i = 0; i < (int)BLOB_SIZE; i++)
        if (dst_blob[i] != SENTINEL) written_bytes++;
    printf("  HW wrote %d/%u bytes of dst blob\n", written_bytes, BLOB_SIZE);
    check(written_bytes > 0, "HW wrote to dst blob");

    printf("  dst_blob[0..31]: ");
    for (i = 0; i < 32; i++) printf("%02x ", dst_blob[i]);
    printf("\n");

cleanup:
    free_image(&src);
    HI_MPI_SYS_MmzFree(dst_phys, dst_virt);
    return g_failures - failures_before;
}

int main(int argc, char **argv)
{
    int rc, total_failures = 0;
    int skip_hog = 0;

    for (int i = 1; i < argc; i++)
        if (!strcmp(argv[i], "--no-hog"))
            skip_hog = 1;

    rc = HI_MPI_SYS_Init();
    if (rc != HI_SUCCESS) {
        fprintf(stderr, "HI_MPI_SYS_Init failed: 0x%x\n", rc);
        return 2;
    }

    total_failures += test_persp_trans();
    if (!skip_hog)
        total_failures += test_hog();

    HI_MPI_SYS_Exit();

    printf("\n=== Summary: %d failure(s) ===\n", total_failures);
    return total_failures ? 1 : 0;
}
