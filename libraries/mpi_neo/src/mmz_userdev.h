/*
 * SPDX-License-Identifier: GPL-2.0
 *
 * mmz_userdev.h — userspace view of /dev/mmz_userdev's ABI.
 *
 * Mirrors the relevant fields of `struct mmb_info` from
 * openhisilicon/kernel/osal/include/osal_mmz.h, but only the parts that
 * are valid for the userspace ioctl interface (no __KERNEL__-only fields).
 *
 * The struct is fixed-layout per the kernel's __phys_addr_align__ rules:
 * for the standard 32-bit ARM build (kernel + userspace both 32-bit musl),
 * __phys_addr_type__ = unsigned long (4 bytes) and the alignment is 4.
 */
#ifndef __LIBMPI_NEO_MMZ_USERDEV_H__
#define __LIBMPI_NEO_MMZ_USERDEV_H__

#include <stdint.h>
#include <sys/ioctl.h>

#define MMZ_USERDEV_PATH    "/dev/mmz_userdev"

#define MMZ_MMB_NAME_LEN    16
#define MMZ_MMZ_NAME_LEN    32

/*
 * Layout matches struct mmb_info from osal_mmz.h with __phys_addr_type__
 * = unsigned long. On a 32-bit ARM kernel build this is exactly 80 bytes
 * (so _IOWR('m', N, struct mmb_info) encodes size=80 in the cmd word).
 *
 * The "w32_stuf" field maps to the prot:8 / flags:12 bitfield union
 * in the kernel header — we treat it as an opaque unsigned long that
 * the kernel sets/uses internally.
 */
struct mmb_info {
    unsigned long phys_addr;
    unsigned long align;
    unsigned long size;
    unsigned int  order;
    void         *mapped;
    unsigned long w32_stuf;
    char          mmb_name[MMZ_MMB_NAME_LEN];
    char          mmz_name[MMZ_MMZ_NAME_LEN];
    unsigned long gfp;
} __attribute__((aligned(8)));

struct dirty_area {
    unsigned long  dirty_phys_start;
    void          *dirty_virt_start;
    unsigned long  dirty_size;
} __attribute__((aligned(sizeof(long))));

/*
 * ioctl command encodings — must match the values produced by the
 * _IOWR/_IOW/_IO macros in the kernel header for the same struct sizes.
 * Using the standard <sys/ioctl.h> macros gives identical encodings.
 */
#define IOC_MMB_ALLOC               _IOWR('m', 10, struct mmb_info)
#define IOC_MMB_FREE                _IOW ('m', 12, struct mmb_info)
#define IOC_MMB_ALLOC_V2            _IOWR('m', 13, struct mmb_info)
#define IOC_MMB_USER_REMAP          _IOWR('m', 20, struct mmb_info)
#define IOC_MMB_USER_REMAP_CACHED   _IOWR('m', 21, struct mmb_info)
#define IOC_MMB_USER_UNMAP          _IOWR('m', 22, struct mmb_info)
#define IOC_MMB_FLUSH_DCACHE_DIRTY  _IOW ('d', 50, struct dirty_area)

#endif /* __LIBMPI_NEO_MMZ_USERDEV_H__ */
