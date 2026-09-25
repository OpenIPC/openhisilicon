/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __MM_EXT_H__
#define __MM_EXT_H__

#include "osal.h"
#include "math_fun.h"
#include "common.h"
#include "osal_mmz.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

extern XMEDIA_U64 CMPI_MmzMalloc(XMEDIA_CHAR *pMmzName, const XMEDIA_CHAR *pBufName, XMEDIA_UL ulSize);
extern XMEDIA_VOID CMPI_MmzFree(XMEDIA_U64 u64PhyAddr, XMEDIA_VOID *pVirAddr);
extern XMEDIA_S32 CMPI_MmzMallocNocache(XMEDIA_CHAR *cpMmzName, const XMEDIA_CHAR *pBufName,
                                    XMEDIA_U64 *pu64PhyAddr, XMEDIA_VOID **ppVirAddr, XMEDIA_UL ulLen);
extern XMEDIA_S32 CMPI_MmzMallocCached(XMEDIA_CHAR *cpMmzName, const XMEDIA_CHAR *pBufName,
                                   XMEDIA_U64 *pu64PhyAddr, XMEDIA_VOID **ppVirAddr, XMEDIA_UL ulLen);


XMEDIA_VOID *CMPI_Remap_Cached(XMEDIA_U64 u64PhyAddr, XMEDIA_UL ulSize);
XMEDIA_VOID *CMPI_Remap_Nocache(XMEDIA_U64 u64PhyAddr, XMEDIA_UL ulSize);
XMEDIA_VOID CMPI_Unmap(XMEDIA_VOID *pVirtAddr);


static inline XMEDIA_S32 CMPI_CheckMmzPhyAddr(XMEDIA_U64 u64PhyAddr, XMEDIA_U64 u64Len)
{
    /* if address in mmz of current system */
    if (mmz_is_phys_in_mmz(u64PhyAddr, u64Len))
        return XMEDIA_FAILURE;

    return XMEDIA_SUCCESS;
}

static inline XMEDIA_BOOL read_user_linear_space_valid(XMEDIA_U8 *pu8AddrStart, XMEDIA_U32 u32Len)
{
    XMEDIA_U8 u8Check;
    XMEDIA_U8 *pu8AddrEnd = XMEDIA_NULL;

    if (u32Len == 0) {
        return XMEDIA_FALSE;
    }

    if (!osal_access_ok(OSAL_VERIFY_READ, pu8AddrStart, u32Len)) {
        return XMEDIA_FALSE;
    }

    pu8AddrEnd = pu8AddrStart + u32Len - 1;
    if (osal_copy_from_user(&u8Check, pu8AddrEnd, 1)) {
        return XMEDIA_FALSE;
    }

    return XMEDIA_TRUE;
}

static inline XMEDIA_BOOL write_user_linear_space_valid(XMEDIA_U8 *pu8AddrStart, XMEDIA_U32 u32Len)
{
    XMEDIA_U8 u8Check = 0;
    XMEDIA_U8 *pu8AddrEnd = XMEDIA_NULL;

    if (u32Len == 0) {
        return XMEDIA_FALSE;
    }

    if (!osal_access_ok(OSAL_VERIFY_WRITE, pu8AddrStart, u32Len)) {
        return XMEDIA_FALSE;
    }

    pu8AddrEnd = pu8AddrStart + u32Len - 1;
    if (osal_copy_to_user(pu8AddrEnd, &u8Check, 1)) {
        return XMEDIA_FALSE;
    }

    return XMEDIA_TRUE;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
