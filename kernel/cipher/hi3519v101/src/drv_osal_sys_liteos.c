/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : cipher_mmz.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2011/05/28
  Description   : cipher_mmz.c header file
  History       :
  1.Date        : 2011/05/28
    Modification: Created file

******************************************************************************/

#ifndef __CIPHER_MMZ_H__
#define __CIPHER_MMZ_H__

#include "hi_osal.h"
#include "osal_mmz.h"

#include "hi_type.h"
#include "drv_osal_lib.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

extern hil_mmb_t *hil_mmb_getby_phys_2(unsigned long addr, unsigned long *Outoffset);

HI_S32  HI_DRV_MMZ_AllocAndMap(const char *name,
                               char *mmzzonename,
                               HI_U32 size,
                               int align,
                               MMZ_BUFFER_S *psMBuf)
{
    hil_mmb_t *pmmb = NULL;
    hil_mmb_t *pmmb_map = NULL;
    unsigned long Outoffset;

    pmmb = hil_mmb_alloc(name, size, 0, 0, mmzzonename);
    if(NULL == pmmb)
    {
        return HI_FAILURE;
    }

    psMBuf->u32StartPhyAddr = hil_mmb_phys(pmmb);
    if(0 == psMBuf->u32StartPhyAddr)
    {
        return HI_FAILURE;
    }
    psMBuf->u32Size = hil_mmb_length(pmmb);

    pmmb_map = hil_mmb_getby_phys_2(psMBuf->u32StartPhyAddr, &Outoffset);
    if(NULL != pmmb_map)
    {
        psMBuf->pu8StartVirAddr = (HI_U8*)hil_mmb_map2kern(pmmb_map) + Outoffset;
        if(NULL == psMBuf->pu8StartVirAddr)
        {
             if(0 != psMBuf->u32StartPhyAddr)
             {
               hil_mmb_freeby_phys(psMBuf->u32StartPhyAddr);
             }
             return HI_FAILURE;
        }
    }
    else
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_VOID HI_DRV_MMZ_UnmapAndRelease(MMZ_BUFFER_S *psMBuf)
{
    hil_mmb_t *mmb;

    if(psMBuf->u32StartPhyAddr != 0)
    {
        hil_mmb_freeby_phys(psMBuf->u32StartPhyAddr);
        psMBuf->u32StartPhyAddr = 0;
    }

    mmb = hil_mmb_getby_kvirt(psMBuf->pu8StartVirAddr);
    if (mmb != NULL)
    {
        hil_mmb_unmap(mmb);
        psMBuf->pu8StartVirAddr = 0;
    }
}

HI_VOID HI_DRV_MMZ_Map(MMZ_BUFFER_S *psMBuf)
{
    unsigned long Outoffset;
    hil_mmb_t *mmb = hil_mmb_getby_phys_2(psMBuf->u32StartPhyAddr, &Outoffset);
    if (mmb != NULL)
    {
        psMBuf->pu8StartVirAddr = (HI_U8*) hil_mmb_map2kern(mmb) + Outoffset;
    }
}

HI_VOID HI_DRV_MMZ_Unmap(MMZ_BUFFER_S *psMBuf)
{
    hil_mmb_t *mmb = hil_mmb_getby_kvirt(psMBuf->pu8StartVirAddr);
    if (mmb != NULL)
    {
        hil_mmb_unmap(mmb);
    }
}

void cipher_flush_cache_all(void)
{
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef __CIPHER_MMZ_H__ */
