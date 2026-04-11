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
#include "hi_type.h"
#include "mmz-userdev.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
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
												if( g_s32fd <=0) return NULL; \
										  }while(0); 

static HI_S32 s_s32MemDev = -1;
#define MEM_DEV_OPEN \
do \
{\
	if (s_s32MemDev <= 0)\
	{\
		s_s32MemDev = open("/dev/mem", O_RDWR|O_SYNC);\
		if (s_s32MemDev < 0)\
		{\
			perror("Open dev/mem error");\
			return NULL;\
		}\
	}\
}while(0)\

static inline HI_VOID *HI_MMB_New(HI_U32 size , HI_U32 align, HI_CHAR *mmz_name, HI_CHAR *mmb_name )
{
	struct mmb_info mmi = {0};
	CHECK_INIT2();

	if( (size ==0) || (size > 0x40000000)) 
	{
		return NULL;
	}

	mmi.size = size;
	mmi.align = align;
	
	if( (mmb_name!= NULL) && (strlen(mmb_name) < (HIL_MMB_NAME_LEN-1)))
	{
		strncpy(mmi.mmb_name, mmb_name, HIL_MMB_NAME_LEN-1);
	}
	if( (mmz_name != NULL)  && (strlen(mmz_name) < (HIL_MMB_NAME_LEN-1)))
	{
		strncpy(mmi.mmz_name, mmz_name, HIL_MMB_NAME_LEN-1);
	}

	if(ioctl(g_s32fd, IOC_MMB_ALLOC, &mmi) !=0)
	{
		return NULL;
	}
	else
	{
		return (HI_VOID *)mmi.phys_addr;
	}
}

static inline HI_VOID *HI_MMB_Map(HI_U32 phys_addr, HI_U32 cached)
{

	struct mmb_info mmi = {0};
	HI_S32 ret;
	
	CHECK_INIT2();
	
	if(cached != 0 && cached != 1)  return NULL;
	
	mmi.prot = PROT_READ | PROT_WRITE;
	mmi.flags = MAP_SHARED;	
	mmi.phys_addr = phys_addr;
	
	if (cached)
			ret = ioctl(g_s32fd, IOC_MMB_USER_REMAP_CACHED, &mmi);
	else
			ret = ioctl(g_s32fd, IOC_MMB_USER_REMAP, &mmi);
			
	if( ret !=0 ) return NULL;

	return (HI_VOID *)mmi.mapped;

}

static inline HI_S32 HI_MMB_Unmap(HI_U32 phys_addr)
{

	struct mmb_info mmi = {0};
	
	CHECK_INIT();
	
	mmi.phys_addr = phys_addr;
	
	return ioctl(g_s32fd, IOC_MMB_USER_UNMAP, &mmi);

}
 
static inline HI_S32 HI_MMB_Delete(HI_U32 phys_addr)
{

	struct mmb_info mmi = {0};

	CHECK_INIT();
	
	mmi.phys_addr = phys_addr;
	
	return ioctl(g_s32fd, IOC_MMB_FREE, &mmi);

}

static inline HI_S32 HI_MMB_Flush()
{
	return ioctl(g_s32fd, IOC_MMB_FLUSH_DCACHE, NULL);
}

static inline HI_VOID * HI_SYS_Mmap(HI_U32 u32PhyAddr, HI_U32 u32Size)
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
    pPageAddr   = mmap ((void *)0, u32PageSize, PROT_READ|PROT_WRITE,
                                    MAP_SHARED, s_s32MemDev, u32PagePhy);   
    if (MAP_FAILED == pPageAddr )   
    {    
        perror("mmap error");
        return NULL;    
    }
    return (HI_VOID *) (pPageAddr + u32Diff);
}

static inline HI_S32 HI_SYS_Munmap(HI_VOID* pVirAddr, HI_U32 u32Size)
{
    HI_U32 u32PageAddr;
    HI_U32 u32PageSize;
    HI_U32 u32Diff;

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
