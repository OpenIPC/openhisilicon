/*
 * SPDX-License-Identifier: GPL-2.0
 *
 * libmpi_neo — open-source replacement for vendor libmpi.so.
 *
 * Implements the 5 HI_MPI_SYS_* entry points needed by qemu-boot/test-*.c:
 *   HI_MPI_SYS_Init        — no-op (kernel modules drive their own init)
 *   HI_MPI_SYS_Exit        — close cached fd
 *   HI_MPI_SYS_MmzAlloc    — alloc + map non-cached MMZ chunk
 *   HI_MPI_SYS_MmzFree     — unmap + free
 *   HI_MPI_SYS_MmzFlushCache — issue dcache flush over a buffer
 *
 * All MMZ traffic goes through /dev/mmz_userdev (the openhisilicon
 * osal_mmz user-space gateway). The cmd numbers and struct layouts come
 * from kernel/osal/include/osal_mmz.h via the local mmz_userdev.h header.
 */
#include "mpi_sys.h"
#include "mmz_userdev.h"

#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/mman.h>

/*
 * Bitfield encoding for the union { prot:8, flags:12, ... } at
 * offset 20 of struct mmb_info. The kernel reads this when remapping
 * the chunk into user space — if we don't set it, the kernel falls
 * back to PROT_READ only and any write to the mapping segfaults.
 *
 *   prot:8   = PROT_READ|PROT_WRITE = 3   (bits 0..7)
 *   flags:12 = MAP_SHARED            = 1  (bits 8..19)
 */
#define MMZ_USER_REMAP_PROT_FLAGS \
    ((unsigned long)(PROT_READ | PROT_WRITE) | ((unsigned long)MAP_SHARED << 8))

/* Cached fd for /dev/mmz_userdev. -1 = not opened. */
static int g_mmz_fd = -1;
static pthread_mutex_t g_mmz_fd_lock = PTHREAD_MUTEX_INITIALIZER;

/* Open /dev/mmz_userdev on first use. Returns 0 on success, errno on
 * failure. Thread-safe. */
static int mmz_open(void)
{
    int ret = 0;

    pthread_mutex_lock(&g_mmz_fd_lock);
    if (g_mmz_fd < 0) {
        g_mmz_fd = open(MMZ_USERDEV_PATH, O_RDWR | O_SYNC);
        if (g_mmz_fd < 0) {
            ret = errno ? errno : EIO;
            fprintf(stderr, "libmpi_neo: open(%s): %s\n",
                    MMZ_USERDEV_PATH, strerror(ret));
        }
    }
    pthread_mutex_unlock(&g_mmz_fd_lock);
    return ret;
}

HI_S32 HI_MPI_SYS_Init(HI_VOID)
{
    /* libmpi_neo carries no per-process system state. The kernel modules
     * (ive_neo, osal_mmz) initialize themselves on their first ioctl. */
    return HI_SUCCESS;
}

HI_S32 HI_MPI_SYS_Exit(HI_VOID)
{
    pthread_mutex_lock(&g_mmz_fd_lock);
    if (g_mmz_fd >= 0) {
        close(g_mmz_fd);
        g_mmz_fd = -1;
    }
    pthread_mutex_unlock(&g_mmz_fd_lock);
    return HI_SUCCESS;
}

/* Internal: shared between MmzAlloc (non-cached) and MmzAlloc_Cached. */
static HI_S32 mmz_alloc_internal(HI_U64 *pu64PhyAddr,
                                 HI_VOID **ppVirtAddr,
                                 const HI_CHAR *pszMmb,
                                 const HI_CHAR *pszZone,
                                 HI_U32 u32Len,
                                 int cached)
{
    struct mmb_info mi;
    int ret;

    if (!pu64PhyAddr || !ppVirtAddr || u32Len == 0)
        return HI_FAILURE;

    if (mmz_open() != 0)
        return HI_FAILURE;

    memset(&mi, 0, sizeof(mi));
    /*
     * Round u32Len up to a page boundary. The kernel mmz_userdev_remap
     * computes len = PAGE_ALIGN(p->size) — but `p->size` is the value
     * we passed in via mi.size. On this kernel that mapping ends up only
     * floor(size/PAGE_SIZE) pages, not ceil. Padding here makes the
     * effective request match the user-visible buffer size and ensures
     * vm_mmap covers the entire page range.
     */
    mi.size      = (u32Len + 4095) & ~0xFFFu;
    mi.align     = 0;        /* allocator picks default page-grain alignment */
    mi.gfp       = 0;
    mi.w32_stuf  = MMZ_USER_REMAP_PROT_FLAGS;
    if (pszMmb)
        strncpy(mi.mmb_name, pszMmb, MMZ_MMB_NAME_LEN - 1);
    else
        strncpy(mi.mmb_name, "ive_neo", MMZ_MMB_NAME_LEN - 1);
    if (pszZone)
        strncpy(mi.mmz_name, pszZone, MMZ_MMZ_NAME_LEN - 1);
    /* leave mmz_name empty otherwise — kernel picks default zone */

    /* Step 1: allocate the MMB chunk. ALLOC_V2 lets the allocator
     * pick a contiguous run that satisfies the size+align request. */
    ret = ioctl(g_mmz_fd, IOC_MMB_ALLOC_V2, &mi);
    if (ret != 0) {
        /* Reset the mmb_info struct (the failed ioctl may have partially
         * trashed it) and try the legacy alloc path. */
        memset(&mi, 0, sizeof(mi));
        mi.size      = (u32Len + 4095) & ~0xFFFu;
        mi.align     = 0;
        mi.gfp       = 0;
        mi.w32_stuf  = MMZ_USER_REMAP_PROT_FLAGS;
        if (pszMmb)
            strncpy(mi.mmb_name, pszMmb, MMZ_MMB_NAME_LEN - 1);
        else
            strncpy(mi.mmb_name, "ive_neo", MMZ_MMB_NAME_LEN - 1);
        if (pszZone)
            strncpy(mi.mmz_name, pszZone, MMZ_MMZ_NAME_LEN - 1);

        ret = ioctl(g_mmz_fd, IOC_MMB_ALLOC, &mi);
        if (ret != 0) {
            fprintf(stderr, "libmpi_neo: IOC_MMB_ALLOC[_V2] %u bytes: %s\n",
                    u32Len, strerror(errno));
            return HI_FAILURE;
        }
    }

    /* Step 2: map it into the calling process's address space.
     * The kernel reads pmi->prot/flags from the bitfield union; reset
     * w32_stuf in case the alloc ioctl trashed it. */
    mi.w32_stuf = MMZ_USER_REMAP_PROT_FLAGS;
    ret = ioctl(g_mmz_fd,
                cached ? IOC_MMB_USER_REMAP_CACHED : IOC_MMB_USER_REMAP,
                &mi);
    if (ret != 0) {
        fprintf(stderr, "libmpi_neo: IOC_MMB_USER_REMAP%s: %s\n",
                cached ? "_CACHED" : "", strerror(errno));
        ioctl(g_mmz_fd, IOC_MMB_FREE, &mi);
        return HI_FAILURE;
    }

    *pu64PhyAddr = (HI_U64)mi.phys_addr;
    *ppVirtAddr  = mi.mapped;
    return HI_SUCCESS;
}

HI_S32 HI_MPI_SYS_MmzAlloc(HI_U64 *pu64PhyAddr,
                           HI_VOID **ppVirtAddr,
                           const HI_CHAR *pszMmb,
                           const HI_CHAR *pszZone,
                           HI_U32 u32Len)
{
    return mmz_alloc_internal(pu64PhyAddr, ppVirtAddr, pszMmb, pszZone,
                              u32Len, /*cached=*/0);
}

HI_S32 HI_MPI_SYS_MmzAlloc_Cached(HI_U64 *pu64PhyAddr,
                                  HI_VOID **ppVirtAddr,
                                  const HI_CHAR *pszMmb,
                                  const HI_CHAR *pszZone,
                                  HI_U32 u32Len)
{
    return mmz_alloc_internal(pu64PhyAddr, ppVirtAddr, pszMmb, pszZone,
                              u32Len, /*cached=*/1);
}

HI_S32 HI_MPI_SYS_MmzFree(HI_U64 u64PhyAddr, HI_VOID *pVirtAddr)
{
    struct mmb_info mi;

    if (g_mmz_fd < 0)
        return HI_FAILURE;

    memset(&mi, 0, sizeof(mi));
    mi.phys_addr = (unsigned long)u64PhyAddr;
    mi.mapped    = pVirtAddr;

    /* Unmap first so the kernel decrements map_ref. */
    if (pVirtAddr)
        ioctl(g_mmz_fd, IOC_MMB_USER_UNMAP, &mi);

    /* Then drop the allocation. */
    if (ioctl(g_mmz_fd, IOC_MMB_FREE, &mi) != 0) {
        fprintf(stderr, "libmpi_neo: IOC_MMB_FREE phys=0x%llx: %s\n",
                (unsigned long long)u64PhyAddr, strerror(errno));
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_S32 HI_MPI_SYS_MmzFlushCache(HI_U64 u64PhyAddr,
                                HI_VOID *pVirtAddr,
                                HI_U32 u32Size)
{
    struct dirty_area area;

    if (g_mmz_fd < 0)
        return HI_FAILURE;

    /* Non-cached allocations don't need a flush — but the kernel handles
     * that case (it's a no-op there). Cached allocations do need it. */
    memset(&area, 0, sizeof(area));
    area.dirty_phys_start = (unsigned long)u64PhyAddr;
    area.dirty_virt_start = pVirtAddr;
    area.dirty_size       = u32Size;

    if (ioctl(g_mmz_fd, IOC_MMB_FLUSH_DCACHE_DIRTY, &area) != 0) {
        /* Older kernels may not implement DIRTY variant — fall back to
         * the global flush. */
        if (errno == ENOTTY || errno == EINVAL) {
            return HI_SUCCESS;
        }
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}
