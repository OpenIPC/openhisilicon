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
#ifndef __DRV_OSAL_LIB_LITEOS_H__
#define __DRV_OSAL_LIB_LITEOS_H__
#include "securec.h"
#include "hi_type.h"
#include "hi_osal.h"

#define HI_KMALLOC(module_id, size, flags)      osal_kmalloc(size, flags)
#define HI_KFREE(module_id, addr)               osal_kfree(addr)
#define HI_VMALLOC(module_id, size)             osal_vmalloc(size)
#define HI_VFREE(module_id, addr)               osal_vfree(addr)

HI_S32 DRV_CIPHER_WaitConditionCallBack(HI_VOID *pParam);

#define copy_from_user                          osal_copy_from_user
#define copy_to_user                            osal_copy_to_user

/* osal_ioremap_nocache is adpat to ioremap_wc wiith cache in Linux */
#define crypto_ioremap_nocache(addr, size)       osal_ioremap_nocache(addr, size)
#define crypto_iounmap(addr)                     osal_iounmap(addr)

#define crypto_get_tickcount()                   osal_get_tickcount()
#define crypto_msleep(msec)                      osal_msleep(msec)
#define crypto_udelay(msec)                      osal_udelay(msec)

#define crypto_mutex_t                           osal_mutex_t
#define crypto_mutex_init(x)                     osal_mutex_init(x)
#define crypto_mutex_lock_interruptible(x)       osal_mutex_lock_interruptible(x)
#define crypto_mutex_unlock(x)                   osal_mutex_unlock(x)

#define crypto_mb()                              osal_mb()
#define CRYPTO_IRQ_HANDLED                       OSAL_IRQ_HANDLED

#define crypto_wait_t                            osal_wait_t
#define  crypto_wait_init(x)                     osal_wait_init(x)
#define crypto_wakeup(x)                         osal_wakeup(x)
#define crypto_wait_event_timeout(head, callbak, param, time)  osal_wait_event_timeout_interruptible(&head, DRV_CIPHER_WaitConditionCallBack, param, time)

#define crypto_request_irq(irq, func, flags, name, arg)        osal_request_irq(irq, func, flags, name, arg)
#define crypto_free_irq(irq, arg)                              osal_free_irq(irq, arg)
#define crypto_irqreturn_t                                     int

#define HI_PRINT(fmt...)                                       osal_printk(fmt)

#endif  /* End of #ifndef __HI_DRV_CIPHER_H__*/
