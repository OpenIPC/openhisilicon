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
    Author      : j00169368
    Modification: Created file

******************************************************************************/

#ifndef __CIPHER_MMZ_H__
#define __CIPHER_MMZ_H__

#include <asm/io.h>

#include <linux/module.h>
#include "media-mem.h"

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct hiCI_MMZ_BUF_S
{
    HI_U32 u32StartPhyAddr;
    HI_U32 u32StartVirAddr;
    HI_U32 u32Size;
}CI_MMZ_BUF_S;

static inline HI_S32 CI_MmzAlloc(const HI_CHAR *pstrMmb, const HI_CHAR *pstrZone, 
      HI_U32 u32Len, HI_U32 cached, CI_MMZ_BUF_S *pMmzBuf)
{
    hil_mmb_t *pmmb = NULL;
    pmmb = hil_mmb_alloc(pstrMmb, u32Len, 0, 0, pstrZone);
    if(NULL == pmmb)
    {
        return HI_FAILURE;
    }
    
    pMmzBuf->u32StartPhyAddr = hil_mmb_phys(pmmb);
    pMmzBuf->u32Size = hil_mmb_length(pmmb);

    //printk("pMmzBuf->u32Size:%d\n",pMmzBuf->u32Size);

    pMmzBuf->u32StartVirAddr = (HI_U32)ioremap_nocache(pMmzBuf->u32StartPhyAddr,pMmzBuf->u32Size);

    return HI_SUCCESS;
}



static inline HI_VOID CI_MmzFree(CI_MMZ_BUF_S *pMmzBuf)
{
    if(pMmzBuf->u32StartPhyAddr != 0)
    {
        hil_mmb_freeby_phys(pMmzBuf->u32StartPhyAddr);
        pMmzBuf->u32StartPhyAddr = 0;
    }

    if(pMmzBuf->u32StartVirAddr != 0)
    {
        iounmap((void __iomem *)pMmzBuf->u32StartVirAddr);
        pMmzBuf->u32StartVirAddr = 0;
    }
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif /* End of #ifndef __CIPHER_MMZ_H__ */
