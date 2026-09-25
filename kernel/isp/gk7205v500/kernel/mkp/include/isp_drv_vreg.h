/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#ifndef __ISP_DRV_VREG_H__
#define __ISP_DRV_VREG_H__

#include "comm_3a.h"
#include "mkp_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define IOC_TYPE_ISP         'I'

typedef enum IOC_NR_VREG_E {
    IOC_NR_VREG_FD = 200,
    IOC_NR_VREG_INIT,
    IOC_NR_VREG_EXIT,
    IOC_NR_VREG_RELEASE_ALL,
    IOC_NR_VREG_GETADDR,

    IOC_NR_VREG_BUTT,
} IOC_NR_VREG_E;

typedef struct DRV_VREG_ARGS_S {
    XMEDIA_U64  u64Size;
    XMEDIA_U64  u64BaseAddr;
    XMEDIA_U64  u64PhyAddr;
    XMEDIA_VOID *pVirtAddr;
} DRV_VREG_ARGS_S;

#define VREG_DRV_FD          _IOW(IOC_TYPE_ISP, IOC_NR_VREG_FD, XMEDIA_S32)
#define VREG_DRV_INIT        _IOW(IOC_TYPE_ISP, IOC_NR_VREG_INIT, DRV_VREG_ARGS_S)
#define VREG_DRV_EXIT        _IOW(IOC_TYPE_ISP, IOC_NR_VREG_EXIT, DRV_VREG_ARGS_S)
#define VREG_DRV_RELEASE_ALL _IOW(IOC_TYPE_ISP, IOC_NR_VREG_RELEASE_ALL, DRV_VREG_ARGS_S)
#define VREG_DRV_GETADDR     _IOWR(IOC_TYPE_ISP, IOC_NR_VREG_GETADDR, DRV_VREG_ARGS_S)

long VREG_DRV_ioctl(unsigned int cmd, unsigned long arg, void *private_data);
XMEDIA_S32 VREG_DRV_ReleaseAll(VI_PIPE ViPipe);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef __ISP_VREG_H__ */
