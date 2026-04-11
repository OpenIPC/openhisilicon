/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     :hi_drv_cipher.h
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       :
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#ifndef __DRV_OSAL_LIB_H__
#define __DRV_OSAL_LIB_H__

#ifdef __HuaweiLite__
#include "drv_osal_lib_liteos.h"
#else
#include "drv_osal_lib_linux.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define crypto_memset(s, c, n)    memset(s, c, n)
#define crypto_memcpy(s, c, n)    memcpy(s, c, n)
#define crypto_memcmp(s1, s2, n)  memcmp(s1, s2, n)

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

typedef struct hiCI_MMZ_BUF_S
{
    HI_U32 u32StartPhyAddr;
    HI_U8* pu8StartVirAddr;
    HI_U32 u32Size;
}MMZ_BUFFER_S;

HI_S32  HI_DRV_MMZ_AllocAndMap(const char *name, 
                               char *mmzzonename, 
                               HI_U32 size, 
                               int align, 
                               MMZ_BUFFER_S *psMBuf);

HI_VOID HI_DRV_MMZ_UnmapAndRelease(MMZ_BUFFER_S *psMBuf);

HI_VOID HI_DRV_MMZ_Map(MMZ_BUFFER_S *psMBuf);

HI_VOID HI_DRV_MMZ_Unmap(MMZ_BUFFER_S *psMBuf);

void cipher_flush_cache_all(void);

HI_S32 cipher_kmalloc(HI_U32 size, MMZ_BUFFER_S *psMBuf);

HI_S32 cipher_virt_to_phys(MMZ_BUFFER_S *psMBuf);

void cipher_kfree(MMZ_BUFFER_S *psMBuf);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif  /* End of #ifndef __HI_DRV_CIPHER_H__*/
