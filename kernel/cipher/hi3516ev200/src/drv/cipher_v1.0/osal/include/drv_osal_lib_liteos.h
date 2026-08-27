/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_osal_lib_liteos.h
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
#include "hi_types.h"
#include "hi_osal.h"
#include "drv_osal_chip.h"
#include "drv_cipher_kapi.h"
#include "hi_math.h"
#include "hi_common.h"
#include "osal_mmz.h"

/* osal_ioremap_nocache is adpat to ioremap_wc wiith cache in Linux */
#define crypto_ioremap_nocache(addr, size)  osal_ioremap_nocache(addr, size)
#define crypto_iounmap(addr, size)          osal_iounmap(addr)

#define crypto_memset(s, sn, c, n)          { if (sn >= n) memset(s, c, n); else HI_LOG_ERROR("error, memset overflow\n");}
#define crypto_memcpy(s, sn, c, n)          { if (sn >= n) memcpy(s, c, n); else HI_LOG_ERROR("error, memcpy overflow\n");}

#define crypto_read(addr)                 (*(volatile unsigned int *)(addr))
#define crypto_write(addr, val)           (*(volatile unsigned int *)(addr) = (val))
#define crypto_read_fifo(id, addr)        crypto_read((hi_u8*)(module_get_base_address(id) + addr))
#define crypto_write_fifo(id, addr, val)  crypto_write((hi_u8*)(module_get_base_address(id)+ addr), val)

hi_s32  crypto_mmz_malloc_nocache(hi_char* mmz_name, hi_char* buf_name,
                                  hi_u64* phy_addr, hi_void** vir_addr,
                                  hi_ulong length);

hi_void crypto_mmz_free(hi_u64 phy_addr, hi_void* vir_addr);

#define crypto_msleep(msec)         osal_msleep(msec)
#define crypto_udelay(msec)         osal_udelay(msec)

#define MAX_MALLOC_BUF_SIZE       (0x10000)
hi_void *crypto_calloc(size_t n, size_t size);
#define crypto_malloc(x)          ((x) > 0 ? osal_kmalloc(x, osal_gfp_atomic) : HI_NULL)
#define crypto_free(x)            {if ((x) != HI_NULL) osal_kfree(x);}

#define copy_from_user     osal_copy_from_user
#define copy_to_user       osal_copy_to_user

hi_s32 crypto_copy_from_user(hi_void *to, const hi_void *from, unsigned long n);
hi_s32 crypto_copy_to_user(hi_void  *to, const hi_void *from, unsigned long n);
hi_u32 get_rand(hi_void);
hi_s32 crypto_waitdone_callback(hi_void *param);

#define crypto_queue_head                          osal_wait_t
#define crypto_queue_init(x)                       osal_wait_init(x)
#define crypto_queue_wait_up(x)                    osal_wakeup(x)
#define crypto_queue_wait_timeout(head, con, time) osal_wait_event_timeout_interruptible(&head, crypto_waitdone_callback, con, time)

#define crypto_request_irq(irq, func, name)  request_irq(irq, (irq_handler_t)func, IRQF_SHARED, name, (hi_void*)name)
#define crypto_free_irq(irq, name)           osal_free_irq(irq, (hi_void*)name)

#define crypto_mutex                         osal_mutex_t
#define crypto_mutex_init(x)                 osal_mutex_init(x)
#define crypto_mutex_lock(x)                 osal_mutex_lock_interruptible(x)
#define crypto_mutex_unlock(x)               osal_mutex_unlock(x)
#define crypto_mutex_destroy(x)

#define crypto_osal_ioremap_nocache(phy_addr, size) osal_ioremap_nocache(phy_addr, ALIGN_UP(size, 4));
#define crypto_osal_iounmap(vir_addr)               osal_iounmap(vir_addr)
#define flush_cache()

#define crypto_owner                         hi_u32
#define crypto_get_owner(x)                  *x = 0

#define PROC_PRINT                           osal_seq_printf

#define irqreturn_t                          int
#define IRQ_HANDLED                          OSAL_IRQ_HANDLED
#define IRQ_NONE                             OSAL_IRQ_NONE

#define HI_PRINT(fmt...)                     osal_printk(fmt)
#define HI_LOG_FATAL(fmt...) \
    do{ \
        osal_printk("[FATAL-HI_CIPHER]:%s[%d]:",(hi_u8*)__FUNCTION__,__LINE__); \
        osal_printk(fmt); \
    }while(0)
#define HI_LOG_ERROR(fmt...) \
    do{ \
        osal_printk("[ERROR-HI_CIPHER]:%s[%d]:",(hi_u8*)__FUNCTION__,__LINE__); \
        osal_printk(fmt); \
    }while(0)
#define HI_LOG_WARN(fmt...)
#define HI_LOG_INFO(fmt...)
#define HI_LOG_DEBUG(fmt...)

/* auto test for cipher_test */
//#define HI_CIPHER_TEST_SUPPORT
#ifdef HI_CIPHER_TEST_SUPPORT
#define CIPHER_LITEOS_TEST_SUPPORT
#endif

#endif  /* End of #ifndef __DRV_OSAL_LIB_LITEOS_H__*/
