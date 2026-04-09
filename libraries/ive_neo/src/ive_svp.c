/*
 * SPDX-License-Identifier: GPL-2.0
 *
 * ive_svp.c — SVP / XNN / IVP / MD private entry points for libive_neo.
 *
 * These are NOT public HI_MPI_IVE_* functions; they are internal libive.so
 * symbols (no "HI_MPI_" prefix) that vendor's test binaries reference
 * directly. The kernel ive_neo.ko handles the corresponding ioctls.
 *
 * Coverage:
 *   mp_ive_svp_alg_proc_init / _exit / _begin_write / _end_write
 *   MPI_IVE_IvpProcInit / Exit / BeginWrite / EndWrite
 *   MPI_IVE_MdProcInit / Exit / BeginWrite / EndWrite
 *   mpi_ive_xnn_loadmodel / unloadmodel / forward_slice
 *   mpi_ive_xnn_get_tmpbuf_size
 *
 * Layouts come from ~/libive/mpi_ive.o.c IDA decomp; cmd numbers verified
 * against ive_neo.c kernel dispatch.
 */
#include "ive_internal.h"

#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdlib.h>

/* SVP/IVP/MD ioctl cmds */
#define IVE_CMD_SVP_ALG_PROC_INIT  0x8010463Bu
#define IVE_CMD_SVP_ALG_PROC_EXIT  0x0000463Eu
#define IVE_CMD_SVP_ALG_PROC_BEGIN 0x0000463Cu
#define IVE_CMD_SVP_ALG_PROC_END   0x0000463Du
#define IVE_CMD_MD_PROC_INIT       0x80104664u
#define IVE_CMD_MD_PROC_EXIT       0x00004667u
#define IVE_CMD_MD_PROC_BEGIN      0x00004665u
#define IVE_CMD_MD_PROC_END        0x00004666u
#define IVE_CMD_IVP_PROC_INIT      0x801046C8u
#define IVE_CMD_IVP_PROC_EXIT      0x000046CBu
#define IVE_CMD_IVP_PROC_BEGIN     0x000046C9u
#define IVE_CMD_IVP_PROC_END       0x000046CAu

/* XNN ioctl cmds */
#define IVE_CMD_XNN_LOADMODEL      0xC8A04636u
#define IVE_CMD_XNN_UNLOADMODEL    0xC0044637u
#define IVE_CMD_XNN_FWD_SLICE      0xC9704638u

/* ===================================================================
 * Generic "alloc + return phys + len" pattern used by SVP/MD/IVP init.
 *
 * Vendor stack layout (16 bytes):
 *   [+0]  u32 buf_len  (out)
 *   [+4]  u32 pad
 *   [+8]  u64 phys_addr (out)
 *
 * Caller passes two void * out-pointers (vendor convention). The test
 * code passes raw uint8_t[16] arrays which may not be 8-byte aligned —
 * use memcpy on the userside writes to avoid an unaligned u64 store
 * trapping on ARM. The libive.so vendor decomp does the same.
 * =================================================================== */
static HI_S32 svp_proc_init_common(unsigned int cmd, void *pphys, void *plen)
{
    uint8_t buf[16];
    HI_S32 ret;

    if (!pphys || !plen)
        return HI_ERR_IVE_NULL_PTR;
    if (ive_open_fd() != HI_SUCCESS)
        return HI_FAILURE;

    memset(buf, 0, sizeof(buf));
    ret = ioctl(g_ive_fd, cmd, buf);
    if (ret == 0) {
        memcpy(plen,  buf + 0, 4);
        memcpy(pphys, buf + 8, 8);
    }
    return ret;
}

/* ===================================================================
 * SVP algorithm proc buffer (used by test-fc-model, test-xnn-direct)
 * =================================================================== */
HI_S32 mp_ive_svp_alg_proc_init(void *pphys, void *plen)
{
    return svp_proc_init_common(IVE_CMD_SVP_ALG_PROC_INIT, pphys, plen);
}

HI_S32 mp_ive_svp_alg_proc_exit(void)
{
    if (ive_open_fd() != HI_SUCCESS)
        return HI_FAILURE;
    return ioctl(g_ive_fd, IVE_CMD_SVP_ALG_PROC_EXIT, 0);
}

HI_S32 mpi_ive_svp_alg_proc_begin_write(void)
{
    if (ive_open_fd() != HI_SUCCESS)
        return HI_FAILURE;
    return ioctl(g_ive_fd, IVE_CMD_SVP_ALG_PROC_BEGIN, 0);
}

HI_S32 mpi_ive_svp_alg_proc_end_write(void)
{
    if (ive_open_fd() != HI_SUCCESS)
        return HI_FAILURE;
    return ioctl(g_ive_fd, IVE_CMD_SVP_ALG_PROC_END, 0);
}

/* ===================================================================
 * Motion Detection proc buffer (vendor compatibility)
 * =================================================================== */
HI_S32 MPI_IVE_MdProcInit(void *pphys, void *plen)
{
    return svp_proc_init_common(IVE_CMD_MD_PROC_INIT, pphys, plen);
}

HI_S32 MPI_IVE_MdProcExit(void)
{
    if (ive_open_fd() != HI_SUCCESS)
        return HI_FAILURE;
    return ioctl(g_ive_fd, IVE_CMD_MD_PROC_EXIT, 0);
}

HI_S32 MPI_IVE_MdProcBeginWrite(void)
{
    if (ive_open_fd() != HI_SUCCESS)
        return HI_FAILURE;
    return ioctl(g_ive_fd, IVE_CMD_MD_PROC_BEGIN, 0);
}

HI_S32 MPI_IVE_MdProcEndWrite(void)
{
    if (ive_open_fd() != HI_SUCCESS)
        return HI_FAILURE;
    return ioctl(g_ive_fd, IVE_CMD_MD_PROC_END, 0);
}

/* ===================================================================
 * IVP proc buffer (used by test-ivp through libivp.so)
 * =================================================================== */
HI_S32 MPI_IVE_IvpProcInit(void *pphys, void *plen)
{
    return svp_proc_init_common(IVE_CMD_IVP_PROC_INIT, pphys, plen);
}

HI_S32 MPI_IVE_IvpProcExit(void)
{
    if (ive_open_fd() != HI_SUCCESS)
        return HI_FAILURE;
    return ioctl(g_ive_fd, IVE_CMD_IVP_PROC_EXIT, 0);
}

HI_S32 MPI_IVE_IvpProcBeginWrite(void)
{
    if (ive_open_fd() != HI_SUCCESS)
        return HI_FAILURE;
    return ioctl(g_ive_fd, IVE_CMD_IVP_PROC_BEGIN, 0);
}

HI_S32 MPI_IVE_IvpProcEndWrite(void)
{
    if (ive_open_fd() != HI_SUCCESS)
        return HI_FAILURE;
    return ioctl(g_ive_fd, IVE_CMD_IVP_PROC_END, 0);
}

/* ===================================================================
 * XNN: load / unload / forward_slice
 *
 * mpi_ive_xnn_loadmodel(model_mem, tmp_mem, model_params)
 *   - model_mem: 24-byte struct {phys, virt, size}
 *   - tmp_mem:   24-byte struct {phys, virt, size}
 *   - model_params: 2160-byte struct populated by kernel
 *   Buffer: 24 + 24 + 2160 = 2208 bytes
 *
 *   ioctl 0xC8A04636 → kernel parses model header from model_mem,
 *   builds task chain in tmp_mem, fills model_params with state.
 *
 *   Returns: 0 on success. The (filled) model_params is copied back.
 * =================================================================== */
HI_S32 mpi_ive_xnn_loadmodel(void *model_mem, void *tmp_mem, void *model_params)
{
    uint8_t buf[2208];
    HI_S32 ret;

    IVE_CHECK_NULL("mpi_ive_xnn_loadmodel", model_mem,    "model_mem");
    IVE_CHECK_NULL("mpi_ive_xnn_loadmodel", tmp_mem,      "tmp_mem");
    IVE_CHECK_NULL("mpi_ive_xnn_loadmodel", model_params, "model_params");

    if (ive_open_fd() != HI_SUCCESS)
        return HI_FAILURE;

    memset(buf, 0, sizeof(buf));
    memcpy(buf,         model_mem,    24);
    memcpy(buf + 24,    tmp_mem,      24);
    memcpy(buf + 48,    model_params, 2160);

    ret = ioctl(g_ive_fd, IVE_CMD_XNN_LOADMODEL, buf);

    /* Copy filled model_params back to caller. */
    memcpy(model_params, buf + 48, 2160);
    return ret;
}

/*
 * mpi_ive_xnn_unloadmodel(model_params): reads model id from
 * *(int *)model_params and submits a 4-byte ioctl.
 */
HI_S32 mpi_ive_xnn_unloadmodel(void *model_params)
{
    uint32_t buf;

    IVE_CHECK_NULL("mpi_ive_xnn_unloadmodel", model_params, "model_params");
    if (ive_open_fd() != HI_SUCCESS)
        return HI_FAILURE;

    buf = *(uint32_t *)model_params;
    return ioctl(g_ive_fd, IVE_CMD_XNN_UNLOADMODEL, &buf);
}

/*
 * mpi_ive_xnn_get_tmpbuf_size(model_params, *out_size)
 *
 * Vendor reads `*(u32 *)(*(u32 *)(model_params + 8) + 12)`. The model
 * params struct (offset 8) holds a pointer to an internal allocation
 * descriptor whose +12 contains the required tmp buffer size.
 *
 * For libive_neo we expose the same interface but compute the size from
 * the model header bytes the user already loaded into model_params.
 * Many test binaries call this AFTER mpi_ive_xnn_loadmodel populated
 * model_params, so the read pattern works.
 */
HI_S32 mpi_ive_xnn_get_tmpbuf_size(void *model_params, HI_U32 *out_size)
{
    void *internal;

    IVE_CHECK_NULL("mpi_ive_xnn_get_tmpbuf_size", model_params, "model_params");
    IVE_CHECK_NULL("mpi_ive_xnn_get_tmpbuf_size", out_size,     "out_size");

    internal = *(void **)((uint8_t *)model_params + 8);
    if (!internal)
        return HI_ERR_IVE_NULL_PTR;
    *out_size = *(uint32_t *)((uint8_t *)internal + 12);
    return HI_SUCCESS;
}

/*
 * mpi_ive_xnn_forward_slice — the main XNN inference call.
 *
 * Vendor signature (8 args):
 *   (handle*, src_blobs, ds_dst_blobs, model_params,
 *    tmp_mem, dst_blobs, ctrl, instant)
 *
 * Vendor builds a stack buffer ~2412 bytes:
 *   [+0]    handle (out)
 *   [+8]    src blobs (768 = 16 * 48)
 *   [+776]  ds_dst blobs (48 bytes — copied only if ctrl[2] != 0)
 *   [+824]  model id (= *(u32*)model_params)
 *   [+832]  cpu_preproc workspace (768 bytes — leave zero)
 *   [+1600] dst blobs (768 = 16 * 48)
 *   [+2368] tmp_mem (24 bytes)
 *   [+2392] ctrl (16 bytes = 4 dwords)
 *   [+2408] instant (4 bytes)
 *
 * The kernel ive_xnn_fwd_slice reads:
 *   model_id @ +0x338 = +824
 *   src blob phys @ +0x18 = +24 (first src blob's phys field)
 *   dst blob phys @ +0x650 = +1616 (first dst blob's phys field)
 *   tmp phys @ +0x940 = +2368
 *   tmp size @ +0x950 = +2384
 */
HI_S32 mpi_ive_xnn_forward_slice(IVE_HANDLE *pHandle,
                                 void *src_blobs,    /* IVE_BLOB_S[ctrl[1]] */
                                 void *ds_dst_blobs, /* IVE_BLOB_S[ctrl[2]] or NULL */
                                 void *model_params,
                                 void *tmp_mem,      /* {phys, virt, size} 24 bytes */
                                 void *dst_blobs,    /* IVE_BLOB_S[ctrl[3]] */
                                 const uint32_t *ctrl,
                                 HI_BOOL instant)
{
    uint8_t buf[2416];   /* slightly oversized */
    HI_S32 ret;

    IVE_CHECK_NULL("mpi_ive_xnn_forward_slice", pHandle,      "pHandle");
    IVE_CHECK_NULL("mpi_ive_xnn_forward_slice", src_blobs,    "src_blobs");
    IVE_CHECK_NULL("mpi_ive_xnn_forward_slice", model_params, "model_params");
    IVE_CHECK_NULL("mpi_ive_xnn_forward_slice", tmp_mem,      "tmp_mem");
    IVE_CHECK_NULL("mpi_ive_xnn_forward_slice", dst_blobs,    "dst_blobs");
    IVE_CHECK_NULL("mpi_ive_xnn_forward_slice", ctrl,         "ctrl");

    if (ive_open_fd() != HI_SUCCESS)
        return HI_FAILURE;

    memset(buf, 0, sizeof(buf));
    /* src blobs at +8 */
    memcpy(buf + 8,    src_blobs, 48 * ctrl[1]);
    /* ds_dst blobs at +776 (only if ctrl[2] > 0) */
    if (ctrl[2] > 0 && ds_dst_blobs)
        memcpy(buf + 776, ds_dst_blobs, 48 * ctrl[2]);
    /* model id at +824 */
    *(uint32_t *)(buf + 824) = *(uint32_t *)model_params;
    /* dst blobs at +1600 */
    memcpy(buf + 1600, dst_blobs, 48 * ctrl[3]);
    /* tmp_mem at +2368 */
    memcpy(buf + 2368, tmp_mem, 24);
    /* ctrl at +2392 (16 bytes = 4 dwords) */
    memcpy(buf + 2392, ctrl, 16);
    /* instant at +2408 */
    *(uint32_t *)(buf + 2408) = (uint32_t)instant;

    ret = ioctl(g_ive_fd, IVE_CMD_XNN_FWD_SLICE, buf);
    *pHandle = (ret == 0) ? (IVE_HANDLE) *(uint32_t *)buf : -1;
    return ret;
}
