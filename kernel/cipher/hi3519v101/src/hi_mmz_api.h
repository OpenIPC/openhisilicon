#ifndef __HI_MMZ_API_H
#define __HI_MMZ_API_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>
#include <linux/kernel.h>

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

static HI_S32 s_s32MemDev = -1;
#define MEM_DEV_OPEN if (s_s32MemDev <= 0)\
{\
    s_s32MemDev = open("/dev/mem", O_RDWR|O_SYNC);\
    if (s_s32MemDev < 0)\
    {\
        perror("Open dev/mem error");\
        return NULL;\
    }\
}\

HI_VOID * HI_SYS_Mmap(HI_U32 u32PhyAddr, HI_U32 u32Size)
{
    HI_U32 u32Diff;
    HI_U32 u32PagePhy;
    HI_U32 u32PageSize;
    HI_U8 * pPageAddr;

    MEM_DEV_OPEN;

    /**********************************************************
    u32PageSize will be 0 when u32size is 0 and u32Diff is 0,
    and then mmap will be error (error: Invalid argument)
    ***********************************************************/
    if (!u32Size)
    {
        printf("Func: %s u32Size can't be 0.\n", __FUNCTION__);
        return NULL;
    }

    /* The mmap address should align with page */
    u32PagePhy = u32PhyAddr & 0xfffff000;
    u32Diff    = u32PhyAddr - u32PagePhy;

    /* The mmap size shuld be mutliples of 1024 */
    u32PageSize = ((u32Size + u32Diff - 1) & 0xfffff000) + 0x1000;
#ifdef __HuaweiLite__
    pPageAddr = (HI_U8*)ioremap_nocache(u32PagePhy, u32PageSize);
#else
    pPageAddr   = (HI_U8*)mmap ((void*)0, u32PageSize, PROT_READ | PROT_WRITE,
                                MAP_SHARED, s_s32MemDev, u32PagePhy);

    if (MAP_FAILED == pPageAddr )
    {
        perror("mmap error");
        return NULL;
    }
#endif
    return (HI_VOID *) (pPageAddr + u32Diff);
}

HI_S32 HI_SYS_Munmap(HI_VOID* pVirAddr, HI_U32 u32Size)
{
    HI_U32 u32PageAddr;
    HI_U32 u32PageSize;
    HI_U32 u32Diff;

    if(NULL == pVirAddr)
    {
        return HI_FAILURE;
    }

    u32PageAddr = (((HI_U32)pVirAddr) & 0xfffff000);
    u32Diff     = (HI_U32)pVirAddr - u32PageAddr;
    u32PageSize = ((u32Size + u32Diff - 1) & 0xfffff000) + 0x1000;

    return munmap((HI_VOID*)u32PageAddr, u32PageSize);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__HI_MMZ_API_H*/
