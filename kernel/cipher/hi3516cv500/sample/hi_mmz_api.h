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
#include "hi_types.h"
#include "osal_mmz.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define HI_MMZ_New 	HI_MMB_New
#define HI_MMZ_Map 	HI_MMB_Map
#define HI_MMZ_Unmap 	HI_MMB_Unmap
#define HI_MMZ_Delete 	HI_MMB_Delete
#define HI_MMZ_Flush 	HI_MMB_Flush

static HI_S32 g_s32fd = 0;

#define CHECK_INIT() 	do{ if( g_s32fd <=0) g_s32fd =open("/dev/mmz_userdev", O_RDWR); \
        if( g_s32fd <=0) return HI_FAILURE; \
    }while(0);

#define CHECK_INIT2() do{ if( g_s32fd <=0) g_s32fd =open("/dev/mmz_userdev", O_RDWR); \
        if( g_s32fd <=0) return 0; \
    }while(0);

static inline unsigned long HI_MMB_New(HI_U32 size , HI_U32 align, HI_CHAR* mmz_name, HI_CHAR* mmb_name )
{
    struct mmb_info mmi = {0};
    CHECK_INIT2();

    if ( (size == 0) || (size > 0x40000000))
    {
        return 0;
    }

    mmi.size = size;
    mmi.align = align;

    if ( (mmb_name != NULL) && (strlen(mmb_name) < (HIL_MMB_NAME_LEN - 1)))
    {
        strncpy(mmi.mmb_name, mmb_name, HIL_MMB_NAME_LEN - 1);
    }

    if ( (mmz_name != NULL)  && (strlen(mmz_name) < (HIL_MMB_NAME_LEN - 1)))
    {
        strncpy(mmi.mmz_name, mmz_name, HIL_MMB_NAME_LEN - 1);
    }

    if (ioctl(g_s32fd, IOC_MMB_ALLOC, &mmi) != 0)
    {
        return 0;
    }
    else
    {
        return mmi.phys_addr;
    }
}

static inline HI_VOID* HI_MMB_Map(HI_U32 phys_addr, HI_U32 size , HI_U32 cached)
{
    struct mmb_info mmi = {0};
    HI_S32 ret;

    CHECK_INIT2();

    if (cached != 0 && cached != 1)  { return NULL; }

    mmi.size = size;
    mmi.prot = PROT_READ | PROT_WRITE;
    mmi.flags = MAP_SHARED;
    mmi.phys_addr = phys_addr;

    if (cached)
    { ret = ioctl(g_s32fd, IOC_MMB_USER_REMAP_CACHED, &mmi); }
    else
    { ret = ioctl(g_s32fd, IOC_MMB_USER_REMAP, &mmi); }

    if ( ret != 0 ) { return NULL; }

    return (HI_VOID*)mmi.mapped;
}

static inline HI_S32 HI_MMB_Unmap(unsigned long phys_addr)
{

    struct mmb_info mmi = {0};

    CHECK_INIT();

    mmi.phys_addr = phys_addr;

    return ioctl(g_s32fd, IOC_MMB_USER_UNMAP, &mmi);

}

static inline HI_S32 HI_MMB_Delete(unsigned long phys_addr)
{

    struct mmb_info mmi = {0};

    CHECK_INIT();

    mmi.phys_addr = phys_addr;

    return ioctl(g_s32fd, IOC_MMB_FREE, &mmi);

}

static inline HI_S32 HI_MMB_Flush(HI_VOID)
{
    return ioctl(g_s32fd, IOC_MMB_FLUSH_DCACHE, NULL);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__HI_MMZ_API_H*/
