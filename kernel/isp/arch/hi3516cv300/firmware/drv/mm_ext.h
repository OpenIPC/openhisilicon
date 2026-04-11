/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mm_ext.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/11/16
  Description   : 
  History       :
  1.Date        : 2010/11/16
    Author      : Z44949
    Modification: Created file

******************************************************************************/
#ifndef __MM_EXT_H__
#define __MM_EXT_H__

#include "hi_osal.h"

#include "hi_common.h"
#include "osal_mmz.h"
#include "hi_comm_sys.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

extern HI_U32  CMPI_MmzMalloc(HI_CHAR *pMmzName, HI_CHAR *pBufName, HI_U32 u32Size);
extern HI_VOID CMPI_MmzFree(HI_U32 u32PhyAddr, HI_VOID* pVirtAddr);
extern HI_S32  CMPI_MmzMallocNocache(HI_CHAR* cpMmzName, HI_CHAR* pBufName,
                             HI_U32* pu32PhyAddr, HI_VOID** ppVitAddr, HI_S32 s32Len);
extern HI_S32  CMPI_MmzMallocCached(HI_CHAR* cpMmzName, HI_CHAR* pBufName,
                            HI_U32* pu32PhyAddr, HI_VOID** ppVitAddr, HI_S32 s32Len);


#define MAP_DEBUG_MSG(...) 
/******************************************************************************
 ** The following functions can be used for physical memory which is outside OS.
 ** CMPI_Remap_Cached 
 ** CMPI_Remap_Nocache
 ** CMPI_Unmap
 ******************************************************************************/

extern int odirect;
__inline static HI_VOID * CMPI_Remap_Cached(HI_U32 u32PhyAddr, HI_U32 u32Size)
{
    #if 0
	unsigned long Outoffset;
	hil_mmb_t *mmb = hil_mmb_getby_phys_2(u32PhyAddr, &Outoffset);
	if (mmb != NULL)
	{
		//HI_VOID* tempVir = (hil_mmb_map2kern_cached(mmb) + Outoffset);	
		HI_VOID* tempVir = (hil_mmb_map2kern_cached(mmb) + Outoffset);	
                MAP_DEBUG_MSG("FUNC: %s, phy: 0x%x, virt: 0x%p\n", __FUNCTION__, u32PhyAddr, tempVir);
		return tempVir;
	}

	return NULL;
    #else
    HI_U32 u32Diff;
    HI_U32 u32PagePhy;
    HI_U32 u32PageSize;
    HI_U8 * pPageAddr;
    
    /* The mmap address should align with PAGE_SIZE */
    u32PagePhy = u32PhyAddr & (0xfffff000);   
    u32Diff    = u32PhyAddr - u32PagePhy;

    /* The mmap size shuld be mutliples of PAGE_SIZE */
    u32PageSize = ((u32Size + u32Diff - 1) & 0xfffff000) + 0x1000;

    pPageAddr = hil_mmf_map2kern_cache(u32PagePhy, u32PageSize);
    if (!pPageAddr)
    {
        HI_TRACE_SYS(HI_DBG_ERR, "phy(0x%x) size(0x%x) map cache err!\n", u32PhyAddr, u32Size);
        return NULL;
    }

	return (pPageAddr + u32Diff);
    #endif
	

}

__inline static HI_VOID * CMPI_Remap_Nocache(HI_U32 u32PhyAddr, HI_U32 u32Size)
{
    #if 0
	unsigned long Outoffset;
	hil_mmb_t *mmb = hil_mmb_getby_phys_2(u32PhyAddr, &Outoffset);
	if (mmb != NULL)
	{
		HI_VOID* tempVir = (hil_mmb_map2kern(mmb) + Outoffset);
                MAP_DEBUG_MSG("FUNC: %s, phy: 0x%x, virt: 0x%p\n", __FUNCTION__, u32PhyAddr, tempVir);
		return tempVir;
	}

	return NULL; 
    #else
    HI_U32 u32Diff;
    HI_U32 u32PagePhy;
    HI_U32 u32PageSize;
    HI_U8 * pPageAddr;
    
    /* The mmap address should align with PAGE_SIZE */
    u32PagePhy = u32PhyAddr & 0xfffff000;   
    u32Diff    = u32PhyAddr - u32PagePhy;

    /* The mmap size shuld be mutliples of PAGE_SIZE */
    u32PageSize = ((u32Size + u32Diff - 1) & 0xfffff000) + 0x1000;

    pPageAddr = hil_mmf_map2kern_nocache(u32PagePhy, u32PageSize);    
    if (!pPageAddr)
    {
        HI_TRACE_SYS(HI_DBG_ERR, "phy(0x%x) size(0x%x) map nocache err!\n", u32PhyAddr, u32Size);
        return NULL;
    }

    return (pPageAddr + u32Diff);
    #endif
	
}

__inline static HI_VOID   CMPI_Unmap(HI_VOID *pVirtAddr)
{
    #if 0
	hil_mmb_t *mmb = hil_mmb_getby_kvirt(pVirtAddr);
	if (mmb != NULL)
	{
		MAP_DEBUG_MSG("pVirtAddr: 0x%p------------\n", pVirtAddr);
		hil_mmb_unmap(mmb);
	}
	else 
	{
		MAP_DEBUG_MSG("pVirtAddr: 0x%p, mmb null\n", pVirtAddr);
	}
    #else
    hil_mmf_unmap(pVirtAddr);
    #endif
}

/******************************************************************************
** The following functions only can be used for memory which is inside the OS.
** CMPI_Vmalloc 
** CMPI_Vfree
******************************************************************************/
__inline static HI_VOID * CMPI_Vmalloc(HI_U32 u32Size)
{
    HI_VOID *pMalloc = NULL;
    
    pMalloc = osal_vmalloc(u32Size);

    return pMalloc;
}

__inline static HI_VOID CMPI_Vfree(HI_VOID *pVirAddr)
{
    osal_vfree(pVirAddr);
}

/******************************************************************************
** Attention: Only the memory is malloced by CMPI_Kmalloc can use it!!
** CMPI_Virt2Phys
** CMPI_Phys2Virt
******************************************************************************/
/*
__inline static HI_U32 CMPI_Virt2Phys(HI_VOID *pVirAddr)
{
    return virt_to_phys(pVirAddr);
}

__inline static HI_VOID * CMPI_Phys2Virt(HI_U32 u32PhyAddr)
{
    return phys_to_virt(u32PhyAddr);
}
*/

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef  __MM_EXT_H__ */
