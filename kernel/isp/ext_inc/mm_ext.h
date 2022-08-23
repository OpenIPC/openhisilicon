/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
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

extern GK_U64 CMPI_MmzMalloc(GK_CHAR *pMmzName, const GK_CHAR *pBufName,
			     GK_UL ulSize);
extern GK_VOID CMPI_MmzFree(GK_U64 u64PhyAddr, GK_VOID *pVirAddr);
extern GK_S32 CMPI_MmzMallocNocache(GK_CHAR *cpMmzName, const GK_CHAR *pBufName,
				    GK_U64 *pu64PhyAddr, GK_VOID **ppVirAddr,
				    GK_UL ulLen);
extern GK_S32 CMPI_MmzMallocCached(GK_CHAR *cpMmzName, const GK_CHAR *pBufName,
				   GK_U64 *pu64PhyAddr, GK_VOID **ppVirAddr,
				   GK_UL ulLen);

GK_VOID *CMPI_Remap_Cached(GK_U64 u64PhyAddr, GK_UL ulSize);
GK_VOID *CMPI_Remap_Nocache(GK_U64 u64PhyAddr, GK_UL ulSize);
GK_VOID CMPI_Unmap(GK_VOID *pVirtAddr);

static inline GK_S32 CMPI_CheckMmzPhyAddr(GK_U64 u64PhyAddr, GK_U64 u64Len)
{
	/* if address in mmz of current system */
	if (mmz_is_phys_in_mmz(u64PhyAddr, u64Len)) {
		/* if in other system */
		if (mmz_map_mmz_check_phys(u64PhyAddr, u64Len)) {
			return GK_FAILURE;
		}
	}

	return GK_SUCCESS;
}

static inline GK_BOOL read_user_linear_space_valid(GK_U8 *pu8AddrStart,
						   GK_U32 u32Len)
{
	GK_U8 u8Check;
	GK_U8 *pu8AddrEnd = GK_NULL;

	if (u32Len == 0) {
		return GK_FALSE;
	}

	if (!osal_access_ok(OSAL_VERIFY_READ, pu8AddrStart, u32Len)) {
		return GK_FALSE;
	}

	pu8AddrEnd = pu8AddrStart + u32Len - 1;
	if (osal_copy_from_user(&u8Check, pu8AddrEnd, 1)) {
		return GK_FALSE;
	}

	return GK_TRUE;
}

static inline GK_BOOL write_user_linear_space_valid(GK_U8 *pu8AddrStart,
						    GK_U32 u32Len)
{
	GK_U8 u8Check = 0;
	GK_U8 *pu8AddrEnd = GK_NULL;

	if (u32Len == 0) {
		return GK_FALSE;
	}

	if (!osal_access_ok(OSAL_VERIFY_WRITE, pu8AddrStart, u32Len)) {
		return GK_FALSE;
	}

	pu8AddrEnd = pu8AddrStart + u32Len - 1;
	if (osal_copy_to_user(pu8AddrEnd, &u8Check, 1)) {
		return GK_FALSE;
	}

	return GK_TRUE;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
