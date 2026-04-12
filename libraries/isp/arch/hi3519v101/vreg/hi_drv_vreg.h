/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_drv_vreg.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/09
  Description   : 
  History       :
  1.Date        : 2013/01/09
    Author      : n00168968
    Modification: Created file

******************************************************************************/
#ifndef __HI_DRV_VREG_H__
#define __HI_DRV_VREG_H__

#include "hi_comm_3a.h"
#ifdef __KERNEL__
#include "hi_osal.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define IOC_TYPE_ISP   'I'

#ifndef  ISP_CHECK_POINTER
#define ISP_CHECK_POINTER(ptr)\
do {\
    if (HI_NULL == ptr)\
    {\
        ISP_TRACE(HI_DBG_ERR, "Null Pointer!\n");\
        return HI_ERR_ISP_NULL_PTR;\
    }\
}while(0)
#endif

typedef enum hiIOC_NR_VREG_E
{
    IOC_NR_VREG_INIT = 0xA0,
    IOC_NR_VREG_EXIT,
    IOC_NR_VREG_RELEASE_ALL,
    IOC_NR_VREG_GETADDR,

    IOC_NR_VREG_BUTT,
} IOC_NR_VREG_E;

typedef struct hiVREG_ARGS_S
{
    HI_U32  u32Size;
    HI_U32  u32BaseAddr;
    HI_U32  u32PhyAddr;
    HI_U32  u32VirtAddr;
} VREG_ARGS_S;

#define VREG_DRV_INIT           _IOW(IOC_TYPE_ISP, IOC_NR_VREG_INIT, VREG_ARGS_S)
#define VREG_DRV_EXIT           _IOW(IOC_TYPE_ISP, IOC_NR_VREG_EXIT, VREG_ARGS_S)
#define VREG_DRV_RELEASE_ALL    _IOW(IOC_TYPE_ISP, IOC_NR_VREG_RELEASE_ALL, VREG_ARGS_S)
#define VREG_DRV_GETADDR        _IOWR(IOC_TYPE_ISP, IOC_NR_VREG_GETADDR, VREG_ARGS_S)

#ifdef __KERNEL__
long VREG_DRV_ioctl(unsigned int cmd, unsigned long arg, void *private_data);
HI_VOID VREG_DRV_ReleaseAll(HI_VOID);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef __ISP_VREG_H__ */

