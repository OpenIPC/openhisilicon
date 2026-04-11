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

#include "hi_type.h"
#include "drv_osal_lib.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


HI_S32  HI_DRV_MMZ_AllocAndMap(const char *name,
                               char *mmzzonename,
                               HI_U32 size,
                               int align,
                               MMZ_BUFFER_S *psMBuf)
{
    psMBuf->u32Size = size;
    psMBuf->pu8StartVirAddr = kzalloc(size, GFP_KERNEL);
    if(NULL == psMBuf->pu8StartVirAddr)
    {
        return HI_FAILURE;
    }

    psMBuf->u32StartPhyAddr = virt_to_phys(psMBuf->pu8StartVirAddr);
    if(0 == psMBuf->u32StartPhyAddr)
    {
        if(NULL != psMBuf->pu8StartVirAddr)
        {
             kfree(psMBuf->pu8StartVirAddr);
             psMBuf->pu8StartVirAddr = NULL;
        }

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_VOID HI_DRV_MMZ_UnmapAndRelease(MMZ_BUFFER_S *psMBuf)
{
     if(NULL != psMBuf->pu8StartVirAddr)
     {
          kfree(psMBuf->pu8StartVirAddr);
          psMBuf->pu8StartVirAddr = NULL;
     }
}

HI_VOID HI_DRV_MMZ_Map(MMZ_BUFFER_S *psMBuf)
{
    psMBuf->pu8StartVirAddr = phys_to_virt(psMBuf->u32StartPhyAddr);
}

HI_VOID HI_DRV_MMZ_Unmap(MMZ_BUFFER_S *psMBuf)
{
}

void cipher_flush_cache_all(void)
{
    flush_cache_all();
}

HI_S32 cipher_kmalloc(HI_U32 size, MMZ_BUFFER_S *psMBuf)
{
    psMBuf->pu8StartVirAddr = kzalloc(size, GFP_KERNEL);
    if(NULL == psMBuf->pu8StartVirAddr)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 cipher_virt_to_phys(MMZ_BUFFER_S *psMBuf)
{
    psMBuf->u32StartPhyAddr = virt_to_phys(psMBuf->pu8StartVirAddr);
    if(0 == psMBuf->u32StartPhyAddr)
    {
        if(NULL != psMBuf->pu8StartVirAddr)
        {
             kfree(psMBuf->pu8StartVirAddr);
        }

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

void cipher_kfree(MMZ_BUFFER_S *psMBuf)
{
     kfree(psMBuf->pu8StartVirAddr);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif /* End of #ifndef __CIPHER_MMZ_H__ */
