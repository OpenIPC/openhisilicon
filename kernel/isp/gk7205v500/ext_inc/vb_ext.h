/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __VB_EXT_H__
#define __VB_EXT_H__

#include "comm_vb.h"
#include "common.h"
#include "comm_video.h"
#include "mod_ext.h"

typedef XMEDIA_U32 VB_BLKHANDLE;

typedef struct VB_CREATE_EX_INFO_S {
    MPP_CHN_S stMod;
    XMEDIA_S32 (*pCallBack)(MPP_CHN_S stMod, XMEDIA_U64 u64PhyAddr);
} VB_CREATE_EX_INFO_S;

typedef struct VB_POOL_INFO_S {
    XMEDIA_U32 u32PoolId;
    XMEDIA_U32 u32BlkCnt;
    XMEDIA_U64 u64BlkSize;
    XMEDIA_U64 u64PoolSize;
    XMEDIA_U64 u64PoolPhyAddr;
    XMEDIA_VOID ATTRIBUTE *pPoolVirAddr;
} VB_POOL_INFO_S;
/*****************************************************************************
 Prototype    : VB_CreatePool
 Description  : create a private VB pool
 Input        : u32BlkCnt      : count of buffer blocks
                u32BlkCnt      : size of buffer blocks
 Output       : pu32PoolId     : pointer to the ID of pool created
 Return Value : XMEDIA_S32         : 0 -- success; !0 : failure
*****************************************************************************/
extern XMEDIA_S32 VB_CreatePool(XMEDIA_U32 *pu32PoolId, XMEDIA_U32 u32BlkCnt, XMEDIA_U64 u64BlkSize,
                            XMEDIA_CHAR *pcMmzName, XMEDIA_CHAR *pBufName, VB_REMAP_MODE_E enVbRemapMode);

/*****************************************************************************
 Prototype    : VB_CreatePoolExt
 Description  : create a private VB pool,but it's not malloc buffer
 Input        : stVbRegInfo      : register info
                u32BlkCnt      : count of buffer blocks
 Output       : pu32PoolId     : pointer to the ID of pool created
 Return Value : XMEDIA_S32         : 0 -- success; !0 : failure
*****************************************************************************/
extern XMEDIA_S32 VB_CreatePoolExt(XMEDIA_U32 *pu32PoolId, XMEDIA_U32 u32BlkCnt, VB_CREATE_EX_INFO_S *pstVbRegInfo);

/*****************************************************************************
 Prototype    : VB_AddBlkToPool
 Description  : add a buffer block to a specified pool
 Input        : u32PoolId      : ID of a pool
                u64PhysAddr    : physcial address of the buffer
                u32Uid         : ID of user getting the buffer
 Output       :
 Return Value : VB_BLKHANDLE   :
                success : Not VB_INVALID_HANDLE
                failure : VB_INVALID_HANDLE
*****************************************************************************/
extern XMEDIA_S32 VB_AddBlkToPool(XMEDIA_U32 u32PoolId, XMEDIA_U64 u64PhysAddr, XMEDIA_U64 u64Size, XMEDIA_U32 u32Uid);

/*****************************************************************************
 Prototype    : VB_AddBlkToPool
 Description  : add a buffer block to a specified pool
 Input        : u32PoolId      : ID of a pool
                u64PhysAddr    : physcial address of the buffer
                u32Uid         : ID of user getting the buffer
 Output       :
 Return Value : VB_BLKHANDLE   :
                success : Not VB_INVALID_HANDLE
                failure : VB_INVALID_HANDLE
*****************************************************************************/
extern XMEDIA_S32 VB_GetPoolInfo(VB_POOL_INFO_S *pstInfo);

extern XMEDIA_S32 VB_DestroyPool(XMEDIA_U32 u32PoolId);

/*****************************************************************************
 Prototype    : VB_GetPoolId
 Description  : Get the first one in common pools in which block size is greater
                than  'u64BlkSize'
 Input        :
 Output       :
 Return Value : XMEDIA_S32
                success : 0
                failure : Not 0
*****************************************************************************/
extern XMEDIA_S32 VB_GetPoolId(XMEDIA_U64 u64BlkSize, XMEDIA_U32 *pu32PoolId, XMEDIA_CHAR *pcMmzName, XMEDIA_S32 s32Owner);

/*****************************************************************************
 Prototype    : VB_GetBlkByPoolId
 Description  : Get a buffer block from a specified pool
 Input        : u32PoolId      : ID of a pool
                u32Uid         : ID of user getting the buffer
 Output       :
 Return Value : VB_BLKHANDLE   :
                success : Not VB_INVALID_HANDLE
                failure : VB_INVALID_HANDLE
*****************************************************************************/
extern VB_BLKHANDLE VB_GetBlkByPoolId(XMEDIA_U32 u32PoolId, XMEDIA_U32 u32Uid);

/*****************************************************************************
 Prototype    : VB_GetBlkBySize
 Description  : Get a block in the first fit common pool
 Input        : u64BlkSize : size of buffer block
                u32Uid     : ID of user who will use the buffer
 Output       : NONE
 Return Value : VB_BLKHANDLE
                success : Not VB_INVALID_HANDLE
                failure : VB_INVALID_HANDLE
*****************************************************************************/
extern VB_BLKHANDLE VB_GetBlkBySize(XMEDIA_U64 u64BlkSize, XMEDIA_U32 u32Uid, XMEDIA_CHAR *pcMmzName);

/*****************************************************************************
 Prototype    : VB_GetBlkBySizeAndModule
 Description  : Get a block in the module common pool
 Input        : u64BlkSize : size of buffer block
                u32Uid     : ID of user who build the buffer
 Output       : NONE
 Return Value : VB_BLKHANDLE
                success : Not VB_INVALID_HANDLE
                failure : VB_INVALID_HANDLE
*****************************************************************************/
extern VB_BLKHANDLE VB_GetBlkBySizeAndModule(XMEDIA_U64 u64BlkSize, XMEDIA_U32 u32Uid, XMEDIA_CHAR *pcMmzName);

/*****************************************************************************
 Prototype    : VB_GetBlkBySizeAndPoolId
 Description  : Get a block in the pool with id
 Input        : u32BlkSize : size of buffer block
                u32PoolId  : ID of block
                u32Uid     : ID of user who build the buffer
 Output       : NONE
 Return Value : VB_BLKHANDLE
                success : Not VB_INVALID_HANDLE
                failure : VB_INVALID_HANDLE
*****************************************************************************/
extern VB_BLKHANDLE VB_GetBlkBySizeAndPoolId(XMEDIA_U32 u32PoolId, XMEDIA_U64 u64BlkSize, XMEDIA_U32 u32Uid);

/*****************************************************************************
 Prototype    : VB_PutBlk
 Description  : forcebly free the buffer even if it is used by more than one
                user. Put a block back to free list, reset all couter for all users.
 Input        : u32PoolId      : ID of pool in which the buffer is.
                u64PhysAddr    : physical address of the buffer
 Output       :
 Return Value : XMEDIA_S32
                success : 0
                failure : Not 0
*****************************************************************************/
extern XMEDIA_S32 VB_PutBlk(XMEDIA_U32 u32PoolId, XMEDIA_U64 u64PhyAddr);

/*****************************************************************************
 Prototype    : VB_Phy2Handle
 Description  : convert physical address to handle
 Input        :
 Output       :
 Return Value : XMEDIA_U32
******************************************************************************/
extern VB_BLKHANDLE VB_Phy2Handle(XMEDIA_U64 u64PhyAddr);

/*****************************************************************************
 Prototype    : VB_Handle2Phys
 Description  : convert handle to physical address of a buffer
 Input        :
 Output       :
 Return Value : XMEDIA_U32
******************************************************************************/
extern XMEDIA_U64 VB_Handle2Phys(VB_BLKHANDLE Handle);

/*****************************************************************************
 Prototype    : VB_Handle2Kern
 Description  : convert handle to kernel virtual address of a buffer
 Input        : Handle     : hanle of a buffer
 Output       :
 Return Value : XMEDIA_U32
*****************************************************************************/
extern XMEDIA_U64 VB_Handle2Kern(VB_BLKHANDLE Handle);

/*****************************************************************************
 Prototype    : VB_Handle2BlkSize
 Description  : convert handle to the size a buffer
 Input        : Handle     : hanle of a buffer
 Output       : ?T
 Return Value : XMEDIA_U32
*****************************************************************************/
extern XMEDIA_U64 VB_Handle2BlkSize(VB_BLKHANDLE Handle);

/*****************************************************************************
 Prototype    : VB_Handle2PoolId
 Description  : convert Handle to pool ID
 Input        : VB_BLKHANDLE   : handle of the buffer
 Output       :
 Return Value : XMEDIA_U32
*****************************************************************************/
extern XMEDIA_U32 VB_Handle2PoolId(VB_BLKHANDLE Handle);

/*****************************************************************************
 Prototype    : VB_Handle2BlkId
 Description  : convert Handle to pool ID
 Input        : VB_BLKHANDLE   : handle of the buffer
 Output       :
 Return Value : XMEDIA_U32
*****************************************************************************/
extern XMEDIA_U32 VB_Handle2BlkId(VB_BLKHANDLE Handle);

/*****************************************************************************
 Prototype    : VB_UserAdd
 Description  : increase one to reference counter of  a buffer
 Input        : u32PoolId      : ID of pool the buffer is in
                u64PhysAddr    : physcial address of the buffer
                u32Uid         : ID of the user
 Output       :
 Return Value : XMEDIA_S32
                success    : 0
                failure    : Not 0
*****************************************************************************/
extern XMEDIA_S32 VB_UserAdd(XMEDIA_U32 u32PoolId, XMEDIA_U64 u64PhyAddr, XMEDIA_U32 u32Uid);

/*****************************************************************************
 Prototype    : VB_UserSub
 Description  : decrease one to reference counter of a buffer
 Input        : u32PoolId      : ID of pool the buffer is in
                u64PhysAddr    : physcial address of the buffer
                u32Uid         : ID of the user
 Output       :
 Return Value : XMEDIA_S32
*****************************************************************************/
extern XMEDIA_S32 VB_UserSub(XMEDIA_U32 u32PoolId, XMEDIA_U64 u64PhyAddr, XMEDIA_U32 u32Uid);

/*****************************************************************************
 Prototype    : VB_InquireUserCnt
 Description  : Inquire how many users are using the buffer
 Input        : Handle     : hanle of a buffer
 Output       :
 Return Value : XMEDIA_U32
******************************************************************************/
extern XMEDIA_U32 VB_InquireUserCnt(VB_BLKHANDLE Handle);

extern XMEDIA_U32 VB_InquireOneUserCnt(VB_BLKHANDLE Handle, XMEDIA_U32 u32Uid);

extern XMEDIA_U32 VB_InquireTotalUserCnt(VB_BLKHANDLE Handle);
/*****************************************************************************
 Prototype    : VB_InquireBlkCnt
 Description  : Inquire how many blocks the user is using
 Input        : u32Uid
                bIsCommPool
 Output       :
 Return Value : XMEDIA_U32
*****************************************************************************/
extern XMEDIA_U32 VB_InquireBlkCnt(XMEDIA_U32 u32Uid, XMEDIA_BOOL bIsCommPool);

/*****************************************************************************
 Prototype    : VB_InquirePool
 Description  : Inquire the statistic of a pool
 Input        : u32PoolId     : ID of pool the buffer is in
 Output       : pstPoolStatus : pool status
 Return Value : XMEDIA_U32
******************************************************************************/
extern XMEDIA_S32 VB_InquirePool(XMEDIA_U32 u32PoolId, VB_POOL_STATUS_S *pstPoolStatus);

/*****************************************************************************
 Prototype    : VB_InquirePoolUserCnt
 Description  : Inquire how many blocks the user used in this pool
 Input        : u32PoolId     : ID of pool the buffer is in
                 u32Uid          : ID of the user
 Output       : pu32Cnt          : Count of vb used by user
 Return Value : XMEDIA_U32
******************************************************************************/
extern XMEDIA_S32 VB_InquirePoolUserCnt(XMEDIA_U32 u32PoolId, XMEDIA_U32 u32Uid, XMEDIA_U32 *pu32Cnt);

/*****************************************************************************
 Prototype    : VB_IsBlkValid
 Description  : Check if the address is valid
 Input        : u32PoolId
                u64PhysAddr
 Output       :
 Return Value : XMEDIA_BOOL
*****************************************************************************/
extern XMEDIA_BOOL VB_IsBlkValid(XMEDIA_U32 u32PoolId, XMEDIA_U64 u64PhysAddr);

/*****************************************************************************
 Prototype    : VB_Handle2Supplement
 Description  : convert handle to VIDEO_SUPPLEMENT_S
 Input        :
 Output       :
 Return Value : VIDEO_SUPPLEMENT_S*
******************************************************************************/
extern VIDEO_SUPPLEMENT_S *VB_Handle2Supplement(VB_BLKHANDLE Handle);

/*****************************************************************************
 Prototype    : VB_IsSupplementSupport
 Description  : check whether supplement is supported
 Input        :
 Output       :
 Return Value : XMEDIA_BOOL
******************************************************************************/
extern XMEDIA_BOOL VB_IsSupplementSupport(XMEDIA_U32 u32Mask);

/*****************************************************************************
 Prototype    : VB_Init
 Description  : Initialize video buffer modules
 Input        : pArg       : unused now
 Output       :
 Return Value : XMEDIA_S32
                success    : 0
                failure    : Not 0
*****************************************************************************/
extern XMEDIA_S32 VB_Init(XMEDIA_VOID *);

/*****************************************************************************
 Prototype    : VB_Exit
 Description  : cleanup video buffer module
 Input        :
 Output       :
 Return Value :XMEDIA_S32
               success    : 0
               failure    : Not 0 , we never reture failure in fact.
*****************************************************************************/
extern XMEDIA_S32 VB_Exit(XMEDIA_VOID);

/*****************************************************************************
 Prototype    : VB_CopySupplement
 Description  : Copy the supplement info form source to destination
 Input        : pstDstVFrame : source
                pstSrcVFrame : destination
 Output       : NONE
 Return Value : XMEDIA_VOID
*****************************************************************************/
extern XMEDIA_VOID VB_CopySupplement(VIDEO_FRAME_S *pstDstVFrame, const VIDEO_FRAME_S *pstSrcVFrame);

typedef struct VB_EXPORT_FUNC_S {
    XMEDIA_S32(*pfnVB_CreatePool)
    (XMEDIA_U32 *pu32PoolId, XMEDIA_U32 u32BlkCnt, XMEDIA_U64 u64BlkSize, XMEDIA_CHAR *pcMmzName,
     XMEDIA_CHAR *pBufName, VB_REMAP_MODE_E enVbRemapMode);
    XMEDIA_S32 (*pfnVB_DestroyPool)(XMEDIA_U32 u32PoolId);
    VB_BLKHANDLE (*pfnVB_GetBlkByPoolId)(XMEDIA_U32 u32PoolId, XMEDIA_U32 u32Uid);
    VB_BLKHANDLE (*pfnVB_GetBlkBySize)(XMEDIA_U64 u64BlkSize, XMEDIA_U32 u32Uid, XMEDIA_CHAR *pcMmzName);
    VB_BLKHANDLE (*pfnVB_GetBlkBySizeAndModule)(XMEDIA_U64 u64BlkSize, XMEDIA_U32 u32Uid, XMEDIA_CHAR *pcMmzName);
    VB_BLKHANDLE (*pfnVB_GetBlkBySizeAndPoolId)(XMEDIA_U32 u32PoolId, XMEDIA_U64 u64BlkSize, XMEDIA_U32 u32Uid);
    XMEDIA_S32 (*pfnVB_GetPoolId)(XMEDIA_U64 u64BlkSize, XMEDIA_U32 *pu32PoolId, XMEDIA_CHAR *pcMmzName, XMEDIA_S32 s32Owner);
    XMEDIA_S32 (*pfnVB_GetPoolInfo)(VB_POOL_INFO_S *pstInfo);
    XMEDIA_S32 (*pfnVB_PutBlk)(XMEDIA_U32 u32PoolId, XMEDIA_U64 u64PhyAddr);
    VB_BLKHANDLE (*pfnVB_Phy2Handle)(XMEDIA_U64 u64PhyAddr);
    XMEDIA_U32 (*pfnVB_Handle2PoolId)(VB_BLKHANDLE Handle);
    XMEDIA_U32 (*pfnVB_Handle2BlkId)(VB_BLKHANDLE Handle);
    XMEDIA_U64 (*pfnVB_Handle2Kern)(VB_BLKHANDLE Handle);
    XMEDIA_U64 (*pfnVB_Handle2Phys)(VB_BLKHANDLE Handle);
    XMEDIA_U64 (*pfnVB_Handle2BlkSize)(VB_BLKHANDLE Handle);
    XMEDIA_S32 (*pfnVB_UserAdd)(XMEDIA_U32 u32PoolId, XMEDIA_U64 u64PhyAddr, XMEDIA_U32 u32Uid);
    XMEDIA_S32 (*pfnVB_UserSub)(XMEDIA_U32 u32PoolId, XMEDIA_U64 u64PhyAddr, XMEDIA_U32 u32Uid);
    XMEDIA_U32 (*pfnVB_InquireUserCnt)(VB_BLKHANDLE Handle);
    XMEDIA_U32 (*pfnVB_InquireOneUserCnt)(VB_BLKHANDLE Handle, XMEDIA_U32 u32Uid);
    XMEDIA_U32 (*pfnVB_InquireTotalUserCnt)(VB_BLKHANDLE Handle);
    XMEDIA_U32 (*pfnVB_InquireBlkCnt)(XMEDIA_U32 u32Uid, XMEDIA_BOOL bIsCommPool);
    XMEDIA_BOOL (*pfnVB_IsBlkValid)(XMEDIA_U32 u32PoolId, XMEDIA_U64 u64PhyAddr);
    XMEDIA_BOOL (*pfnVB_IsPoolIdValid)(XMEDIA_U32 u32PoolId);
    XMEDIA_S32 (*pfnVB_InquirePool)(XMEDIA_U32 u32PoolId, VB_POOL_STATUS_S *pstPoolStatus);
    XMEDIA_S32 (*pfnVB_InquirePoolUserCnt)(XMEDIA_U32 u32PoolId, XMEDIA_U32 u32Uid, XMEDIA_U32 *pu32Cnt);
    VIDEO_SUPPLEMENT_S *(*pfnVB_Handle2Supplement)(VB_BLKHANDLE Handle);
    XMEDIA_BOOL (*pfnVB_IsSupplementSupport)(XMEDIA_U32 u32Mask);
    XMEDIA_VOID (*pfnVB_CopySupplement)(VIDEO_FRAME_S *pstDstVFrame, const VIDEO_FRAME_S *pstSrcVFrame);
} VB_EXPORT_FUNC_S;

#define CKFN_VB_CreatePool() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_CreatePool != NULL)
#define CALL_VB_CreatePool(pu32PoolId, u32BlkCnt, u64BlkSize, pcMmzName, pBufName, enVbRemapMode) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_CreatePool(pu32PoolId, u32BlkCnt, u64BlkSize, \
    pcMmzName, pBufName, enVbRemapMode)

#define CKFN_VB_DestroyPool() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_DestroyPool != NULL)
#define CALL_VB_DestroyPool(u32PoolId) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_DestroyPool(u32PoolId)

#define CKFN_VB_GetBlkByPoolId() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_GetBlkByPoolId != NULL)
#define CALL_VB_GetBlkByPoolId(u32PoolId, u32Uid) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_GetBlkByPoolId(u32PoolId, u32Uid)

#define CKFN_VB_GetBlkBySize() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_GetBlkBySize != NULL)
#define CALL_VB_GetBlkBySize(u64BlkSize, u32Uid, pcMmzName) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_GetBlkBySize(u64BlkSize, u32Uid, pcMmzName)

#define CKFN_VB_GetBlkBySizeAndModule() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_GetBlkBySizeAndModule != NULL)
#define CALL_VB_GetBlkBySizeAndModule(u64BlkSize, u32Uid, pcMmzName) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_GetBlkBySizeAndModule(u64BlkSize, u32Uid, pcMmzName)

#define CKFN_VB_GetBlkBySizeAndPoolId() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_GetBlkBySizeAndPoolId != NULL)
#define CALL_VB_GetBlkBySizeAndPoolId(u32PoolId, u64BlkSize, u32Uid) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_GetBlkBySizeAndPoolId(u32PoolId, u64BlkSize, u32Uid)

#define CKFN_VB_GetPoolId() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_GetPoolId != NULL)
#define CALL_VB_GetPoolId(u64BlkSize, pu32PoolId, pcMmzName, s32Owner) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_GetPoolId(u64BlkSize, pu32PoolId, pcMmzName, s32Owner)

#define CKFN_VB_GetPoolInfo() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_GetPoolInfo != NULL)
#define CALL_VB_GetPoolInfo(pstInfo) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_GetPoolInfo(pstInfo)

#define CKFN_VB_PutBlk() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_PutBlk != NULL)
#define CALL_VB_PutBlk(u32PoolId, u64PhyAddr) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_PutBlk(u32PoolId, u64PhyAddr)

#define CKFN_VB_Phy2Handle() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_Phy2Handle != NULL)
#define CALL_VB_Phy2Handle(u64PhyAddr) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_Phy2Handle(u64PhyAddr)

#define CKFN_VB_Handle2PoolId() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_Handle2PoolId != NULL)
#define CALL_VB_Handle2PoolId(Handle) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_Handle2PoolId(Handle)

#define CKFN_VB_Handle2BlkId() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_Handle2BlkId != NULL)
#define CALL_VB_Handle2BlkId(Handle) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_Handle2BlkId(Handle)

#define CKFN_VB_Handle2Kern() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_Handle2Kern != NULL)
#define CALL_VB_Handle2Kern(Handle) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_Handle2Kern(Handle)

#define CKFN_VB_Handle2Phys() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_Handle2Phys != NULL)
#define CALL_VB_Handle2Phys(Handle) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_Handle2Phys(Handle)

#define CKFN_VB_Handle2BlkSize() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_Handle2BlkSize != NULL)
#define CALL_VB_Handle2BlkSize(Handle) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_Handle2BlkSize(Handle)

#define CKFN_VB_UserAdd() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_UserAdd != NULL)
#define CALL_VB_UserAdd(u32PoolId, u64PhyAddr, u32Uid) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_UserAdd(u32PoolId, u64PhyAddr, u32Uid)

#define CKFN_VB_UserSub() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_UserSub != NULL)
#define CALL_VB_UserSub(u32PoolId, u64PhyAddr, u32Uid) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_UserSub(u32PoolId, u64PhyAddr, u32Uid)

#define CKFN_VB_InquireUserCnt() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_InquireUserCnt != NULL)
#define CALL_VB_InquireUserCnt(Handle) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_InquireUserCnt(Handle)

#define CKFN_VB_InquireOneUserCnt() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_InquireOneUserCnt != NULL)
#define CALL_VB_InquireOneUserCnt(Handle, u32Uid) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_InquireOneUserCnt(Handle, u32Uid)

#define CKFN_VB_InquireTotalUserCnt() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_InquireTotalUserCnt != NULL)
#define CALL_VB_InquireTotalUserCnt(Handle) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_InquireTotalUserCnt(Handle)

#define CKFN_VB_InquireBlkCnt() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_InquireBlkCnt != NULL)
#define CALL_VB_InquireBlkCnt(u32Uid, bIsCommPool) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_InquireBlkCnt(u32Uid, bIsCommPool)

#define CKFN_VB_IsBlkValid() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_IsBlkValid != NULL)
#define CALL_VB_IsBlkValid(u32PoolId, u64PhyAddr) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_IsBlkValid(u32PoolId, u64PhyAddr)

#define CKFN_VB_IsPoolIdValid() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_IsPoolIdValid != NULL)
#define CALL_VB_IsPoolIdValid(u32PoolId) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_IsPoolIdValid(u32PoolId)

#define CKFN_VB_InquirePool() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_InquirePool != NULL)
#define CALL_VB_InquirePool(u32PoolId, pstPoolStatus) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_InquirePool(u32PoolId, pstPoolStatus)

#define CKFN_VB_InquirePoolUserCnt() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_InquirePoolUserCnt != NULL)
#define CALL_VB_InquirePoolUserCnt(u32PoolId, u32Uid, pu32Cnt) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_InquirePoolUserCnt(u32PoolId, u32Uid, pu32Cnt)

#define CKFN_VB_Handle2Supplement() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_Handle2Supplement != NULL)
#define CALL_VB_Handle2Supplement(Handle) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_Handle2Supplement(Handle)

#define CKFN_VB_IsSupplementSupport() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_IsSupplementSupport != NULL)
#define CALL_VB_IsSupplementSupport(u32Mask) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_IsSupplementSupport(u32Mask)

#define CKFN_VB_CopySupplement() \
    (FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_CopySupplement != NULL)
#define CALL_VB_CopySupplement(pstDstVFrame, pstSrcVFrame) \
    FUNC_ENTRY(VB_EXPORT_FUNC_S, MOD_ID_VB)->pfnVB_CopySupplement(pstDstVFrame, pstSrcVFrame)
#endif /* __VB_EXT_H__ */

