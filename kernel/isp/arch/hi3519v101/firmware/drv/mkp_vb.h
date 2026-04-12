/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mkp_vb.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2006/11/27
  Description   : VB private Head File
  History       :
  1.Date        : 2006/11/27
    Author      : w47960
    Modification: Created file

******************************************************************************/
#ifndef  __MKP_VB_H__
#define  __MKP_VB_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#include "hi_common.h"
#include "hi_comm_vb.h"
#include "mkp_ioctl.h"

typedef struct hiVB_IOC_ARG_S
{
    HI_U32 u32PoolId;
    HI_U32 u32BlkSize;
    HI_U32 u32BlkCnt;
    HI_U32 u32BlkHandle;
    HI_BOOL bIsCommPool;
    HI_CHAR acMmzName[MAX_MMZ_NAME_LEN];
}VB_IOC_ARG_S;

typedef struct hiVB_IOC_CFG_ARG_S
{
    VB_UID_E enVbUid;
    VB_CONF_S stVbConf;
} VB_IOC_CFG_ARG_S;

typedef struct hiVB_IOC_COPY_SUPPLEMENT_ARG_S
{
    VIDEO_FRAME_S stDstVFrame;
    VIDEO_FRAME_S stSrcVFrame;
} VB_IOC_COPY_SUPPLEMENT_ARG_S;

typedef struct hiVB_POOL_INFO_S
{
    HI_U32  u32PoolId;    
    HI_U32  u32PoolSize;
    HI_U32  u32PoolPhyAddr;
    HI_BOOL bIsUserVb;
    HI_VOID *pPoolVirAddr;   
} VB_POOL_INFO_S;

typedef struct hiVIDEO_SUPPLEMENT_INFO_S
{
	VB_BLK Block;
	VIDEO_SUPPLEMENT_S *pstSupplement;
}VIDEO_SUPPLEMENT_INFO_S;


typedef enum hiIOC_NR_VB_E
{
	IOC_NR_VB_CREATE = 1,    
	IOC_NR_VB_DESTROY,    
	IOC_NR_VB_GETBLK,    
	IOC_NR_VB_RELBLK,    
	IOC_NR_VB_H2PHYS,
	IOC_NR_VB_H2POOL,
	IOC_NR_VB_INITCPOOL,
	IOC_NR_VB_EXITCPOOL,
	IOC_NR_VB_GETCPOOL,
	IOC_NR_VB_SETCPOOL,
	IOC_NR_VB_GETINFO,
	IOC_NR_VB_QRYUSER,
	IOC_NR_VB_INITMODCPOOL,
	IOC_NR_VB_EXITMODCPOOL,
	IOC_NR_VB_GETMODCPOOL,
	IOC_NR_VB_SETMODCPOOL,
    IOC_NR_VB_SETSUPPLEMENT,
    IOC_NR_VB_GETSUPPLEMENT,    
	IOC_NR_VB_GETSUPPLEMENTADDR,
    IOC_NR_VB_COPYSUPPLEMENT,
    IOC_NR_VB_SETMODPARAM,
    IOC_NR_VB_GETMODPARAM,    
} IOC_NR_VB_E;

#define VB_CREATE_CTRL  _IOWR(IOC_TYPE_VB, IOC_NR_VB_CREATE, VB_IOC_ARG_S)
#define VB_DESTROY_CTRL  _IOWR(IOC_TYPE_VB, IOC_NR_VB_DESTROY, HI_U32)
#define VB_GETBLK_CTRL  _IOWR(IOC_TYPE_VB, IOC_NR_VB_GETBLK, VB_IOC_ARG_S)
#define VB_RELBLK_CTRL  _IOWR(IOC_TYPE_VB, IOC_NR_VB_RELBLK, HI_U32)

#define VB_H2PHYS_CTRL  _IOWR(IOC_TYPE_VB, IOC_NR_VB_H2PHYS, HI_U32)
#define VB_H2POOL_CTRL  _IOWR(IOC_TYPE_VB, IOC_NR_VB_H2POOL, HI_U32)

#define VB_GETSUPPLEMENTADDR_CTRL _IOWR(IOC_TYPE_VB, IOC_NR_VB_GETSUPPLEMENTADDR, VIDEO_SUPPLEMENT_INFO_S)

#define VB_INIT_POOL_CTRL  _IO(IOC_TYPE_VB, IOC_NR_VB_INITCPOOL)
#define VB_EXIT_POOL_CTRL  _IO(IOC_TYPE_VB, IOC_NR_VB_EXITCPOOL)
#define VB_GET_CONF_CTRL  _IOR(IOC_TYPE_VB, IOC_NR_VB_GETCPOOL, VB_CONF_S)
#define VB_SET_CONF_CTRL  _IOW(IOC_TYPE_VB, IOC_NR_VB_SETCPOOL, VB_CONF_S)

#define VB_GET_INFO_CTRL  _IOWR(IOC_TYPE_VB, IOC_NR_VB_GETINFO, VB_POOL_INFO_S)
#define VB_QRY_USER_CTRL  _IOWR(IOC_TYPE_VB, IOC_NR_VB_QRYUSER, HI_U32)

#define VB_INIT_MOD_POOL_CTRL  _IOW(IOC_TYPE_VB, IOC_NR_VB_INITMODCPOOL, VB_UID_E)
#define VB_EXIT_MOD_POOL_CTRL  _IOW(IOC_TYPE_VB, IOC_NR_VB_EXITMODCPOOL, VB_UID_E)
#define VB_GET_MOD_CONF_CTRL  _IOWR(IOC_TYPE_VB, IOC_NR_VB_GETMODCPOOL, VB_IOC_CFG_ARG_S)
#define VB_SET_MOD_CONF_CTRL  _IOW(IOC_TYPE_VB, IOC_NR_VB_SETMODCPOOL, VB_IOC_CFG_ARG_S)

#define VB_SET_SUPPLEMENT_CONF_CTRL _IOW(IOC_TYPE_VB, IOC_NR_VB_SETSUPPLEMENT, VB_SUPPLEMENT_CONF_S)
#define VB_GET_SUPPLEMENT_CONF_CTRL _IOR(IOC_TYPE_VB, IOC_NR_VB_GETSUPPLEMENT, VB_SUPPLEMENT_CONF_S)
#define VB_COPY_SUPPLEMENT_CTRL _IOWR(IOC_TYPE_VB, IOC_NR_VB_COPYSUPPLEMENT, VB_IOC_COPY_SUPPLEMENT_ARG_S)

#define VB_SET_MODPARAM_CTRL _IOW(IOC_TYPE_VB, IOC_NR_VB_SETMODPARAM, VB_MOD_PARAM_S)
#define VB_GET_MODPARAM_CTRL _IOR(IOC_TYPE_VB, IOC_NR_VB_GETMODPARAM, VB_MOD_PARAM_S)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif/* __MKP_VB_H__*/

