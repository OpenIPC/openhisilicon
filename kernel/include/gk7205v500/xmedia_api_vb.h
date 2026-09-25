/*
 * Copyright (c) XMEDIA. All rights reserved.
 */


#ifndef __XMEDIA_API_VB_H__
#define __XMEDIA_API_VB_H__

#include "comm_vb.h"
#include "comm_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

VB_POOL XMEDIA_API_VB_CreatePool(VB_POOL_CONFIG_S *pstVbPoolCfg);
XMEDIA_S32 XMEDIA_API_VB_DestroyPool(VB_POOL Pool);

VB_BLK XMEDIA_API_VB_GetBlock(VB_POOL Pool, XMEDIA_U64 u64BlkSize, const XMEDIA_CHAR *pcMmzName);
XMEDIA_S32 XMEDIA_API_VB_ReleaseBlock(VB_BLK Block);

VB_BLK XMEDIA_API_VB_PhysAddr2Handle(XMEDIA_U64 u64PhyAddr);
XMEDIA_U64 XMEDIA_API_VB_Handle2PhysAddr(VB_BLK Block);
VB_POOL XMEDIA_API_VB_Handle2PoolId(VB_BLK Block);

XMEDIA_S32 XMEDIA_API_VB_InquireUserCnt(VB_BLK Block);

XMEDIA_S32 XMEDIA_API_VB_GetSupplementAddr(VB_BLK Block, VIDEO_SUPPLEMENT_S *pstSupplement);
XMEDIA_S32 XMEDIA_API_VB_SetSupplementConfig(const VB_SUPPLEMENT_CONFIG_S *pstSupplementConfig);
XMEDIA_S32 XMEDIA_API_VB_GetSupplementConfig(VB_SUPPLEMENT_CONFIG_S *pstSupplementConfig);

XMEDIA_S32 XMEDIA_API_VB_Init(XMEDIA_VOID);
XMEDIA_S32 XMEDIA_API_VB_Exit(XMEDIA_VOID);
XMEDIA_S32 XMEDIA_API_VB_SetConfig(const VB_CONFIG_S *pstVbConfig);
XMEDIA_S32 XMEDIA_API_VB_GetConfig(VB_CONFIG_S *pstVbConfig);

XMEDIA_S32 XMEDIA_API_VB_MmapPool(VB_POOL Pool);
XMEDIA_S32 XMEDIA_API_VB_MunmapPool(VB_POOL Pool);

XMEDIA_S32 XMEDIA_API_VB_GetBlockVirAddr(VB_POOL Pool, XMEDIA_U64 u64PhyAddr, XMEDIA_VOID **ppVirAddr);

XMEDIA_S32 XMEDIA_API_VB_InitModCommPool(VB_UID_E enVbUid);
XMEDIA_S32 XMEDIA_API_VB_ExitModCommPool(VB_UID_E enVbUid);

XMEDIA_S32 XMEDIA_API_VB_SetModPoolConfig(VB_UID_E enVbUid, const VB_CONFIG_S *pstVbConfig);
XMEDIA_S32 XMEDIA_API_VB_GetModPoolConfig(VB_UID_E enVbUid, VB_CONFIG_S *pstVbConfig);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

