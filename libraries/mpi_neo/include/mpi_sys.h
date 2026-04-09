/*
 * SPDX-License-Identifier: GPL-2.0
 *
 * mpi_sys.h — HI_MPI_SYS_* entry points.
 *
 * Declared by libmpi_neo, an open-source replacement for vendor libmpi.so.
 * Only the 5 calls used by qemu-boot/test-*.c are provided.
 */
#ifndef __MPI_SYS_H__
#define __MPI_SYS_H__

#include "hi_type.h"
#include "hi_common.h"

#ifdef __cplusplus
extern "C" {
#endif

HI_S32 HI_MPI_SYS_Init(HI_VOID);
HI_S32 HI_MPI_SYS_Exit(HI_VOID);

HI_S32 HI_MPI_SYS_MmzAlloc(HI_U64 *pu64PhyAddr,
                           HI_VOID **ppVirtAddr,
                           const HI_CHAR *pszMmb,
                           const HI_CHAR *pszZone,
                           HI_U32 u32Len);

HI_S32 HI_MPI_SYS_MmzAlloc_Cached(HI_U64 *pu64PhyAddr,
                                  HI_VOID **ppVirtAddr,
                                  const HI_CHAR *pszMmb,
                                  const HI_CHAR *pszZone,
                                  HI_U32 u32Len);

HI_S32 HI_MPI_SYS_MmzFree(HI_U64 u64PhyAddr, HI_VOID *pVirtAddr);

HI_S32 HI_MPI_SYS_MmzFlushCache(HI_U64 u64PhyAddr,
                                HI_VOID *pVirtAddr,
                                HI_U32 u32Size);

#ifdef __cplusplus
}
#endif

#endif /* __MPI_SYS_H__ */
