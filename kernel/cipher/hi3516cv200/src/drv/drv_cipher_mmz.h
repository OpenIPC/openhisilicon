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
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/module.h>
#include "media-mem.h"

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#ifndef HI_REG_READ8
#define HI_REG_READ8(addr,result)  ((result) = *(volatile unsigned char *)(addr))
#endif

#ifndef HI_REG_READ16
#define HI_REG_READ16(addr,result)  ((result) = *(volatile unsigned short *)(addr))
#endif

#ifndef HI_REG_READ32
#define HI_REG_READ32(addr,result)  ((result) = *(volatile unsigned int *)(addr))
#endif

#ifndef HI_REG_WRITE8
#define HI_REG_WRITE8(addr,result)  (*(volatile unsigned char *)(addr) = (result))
#endif

#ifndef HI_REG_WRITE16
#define HI_REG_WRITE16(addr,result)  (*(volatile unsigned short *)(addr) = (result))
#endif

#ifndef HI_REG_WRITE32
#define HI_REG_WRITE32(addr,result)  (*(volatile unsigned int *)(addr) = (result))
#endif

#ifndef HI_REG_READ
#define HI_REG_READ HI_REG_READ32
#endif

#ifndef HI_REG_WRITE
#define HI_REG_WRITE HI_REG_WRITE32
#endif

/*process of bit*/
#define HAL_SET_BIT(src, bit)        ((src) |= (1<<bit))
#define HAL_CLEAR_BIT(src,bit)       ((src) &= ~(1<<bit))

#define HI_KMALLOC(module_id, size, flags)      kmalloc(size, flags)
#define HI_KFREE(module_id, addr)               kfree(addr)
#define HI_VMALLOC(module_id, size)             vmalloc(size)
#define HI_VFREE(module_id, addr)               vfree(addr)


typedef struct hiCI_MMZ_BUF_S
{
    HI_U32 u32StartPhyAddr;
    HI_U8* pu8StartVirAddr;
    HI_U32 u32Size;
}MMZ_BUFFER_S;

static inline HI_S32  HI_DRV_MMZ_AllocAndMap(const char *name, 
                                            char *mmzzonename, 
                                            HI_U32 size, 
                                            int align, 
                                            MMZ_BUFFER_S *psMBuf)
{
    hil_mmb_t *pmmb = NULL;
    pmmb = hil_mmb_alloc(name, size, 0, 0, mmzzonename);
    if(NULL == pmmb)
    {
        return HI_FAILURE;
    }
    
    psMBuf->u32StartPhyAddr = hil_mmb_phys(pmmb);
    psMBuf->u32Size = hil_mmb_length(pmmb);

    //printk("pMmzBuf->u32Size:%d\n",pMmzBuf->u32Size);

    psMBuf->pu8StartVirAddr = (HI_U8*)ioremap_nocache(psMBuf->u32StartPhyAddr, psMBuf->u32Size);

    return HI_SUCCESS;
}

static inline HI_VOID HI_DRV_MMZ_UnmapAndRelease(MMZ_BUFFER_S *psMBuf)
{
    if(psMBuf->u32StartPhyAddr != 0)
    {
        hil_mmb_freeby_phys(psMBuf->u32StartPhyAddr);
        psMBuf->u32StartPhyAddr = 0;
    }

    if(psMBuf->pu8StartVirAddr != 0)
    {
        iounmap((void __iomem *)psMBuf->pu8StartVirAddr);
        psMBuf->pu8StartVirAddr = 0;
    }
}

static inline HI_VOID HI_DRV_MMZ_Map(MMZ_BUFFER_S *psMBuf)
{
    psMBuf->pu8StartVirAddr = (HI_U8*)ioremap_nocache(psMBuf->u32StartPhyAddr, psMBuf->u32Size);
}

static inline HI_VOID HI_DRV_MMZ_Unmap(MMZ_BUFFER_S *psMBuf)
{
    iounmap((void __iomem *)psMBuf->pu8StartVirAddr);
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif /* End of #ifndef __CIPHER_MMZ_H__ */
