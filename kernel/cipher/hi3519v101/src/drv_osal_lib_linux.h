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
#ifndef __DRV_OSAL_LIB_LINUX_H__
#define __DRV_OSAL_LIB_LINUX_H__
#include <linux/proc_fs.h>
#include <linux/module.h>
#include <linux/signal.h>
#include <linux/spinlock.h>
#include <linux/personality.h>
#include <linux/ptrace.h>
#include <linux/kallsyms.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/seq_file.h>
#include <linux/version.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <asm/atomic.h>
#include <asm/cacheflush.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>
#include <asm/traps.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>

#include "hi_type.h"

#define HI_KMALLOC(module_id, size, flags)        kmalloc(size, flags)
#define HI_KFREE(module_id, addr)                 kfree(addr)
#define HI_VMALLOC(module_id, size)               vmalloc(size)
#define HI_VFREE(module_id, addr)                 vfree(addr)

#define crypto_ioremap_nocache(addr, size)                    ioremap_nocache(addr, size)
#define crypto_iounmap(addr)                                  iounmap(addr)

#define crypto_get_tickcount()                                jiffies_to_msecs(jiffies)
#define crypto_msleep(msec)                                   msleep(msec)
#define crypto_udelay(msec)                                   udelay(msec)

#define crypto_mutex_init(x)                                  sema_init(x, 1)
#define crypto_mutex_lock_interruptible(x)                    down_interruptible(x)
#define crypto_mutex_unlock(x)                                up(x)

#define crypto_mb()                                           mb()

#define crypto_wait_t                                         wait_queue_head_t
#define crypto_wait_init(x)                                   init_waitqueue_head(x)
#define crypto_wakeup(x)                                      wake_up_interruptible(x)
#define crypto_wait_event_timeout(head, callbak, con, time)   wait_event_interruptible_timeout(head, *(con), time)

#define CRYPTO_IRQ_HANDLED                                    IRQ_HANDLED
#if (LINUX_VERSION_CODE > KERNEL_VERSION(3, 18, 0))
#define crypto_request_irq(irq, func, flag, name, arg)        request_irq(irq, func, IRQF_SHARED, name, arg)
#else
#define crypto_request_irq(irq, func, flag, name, arg)        request_irq(irq, func, IRQF_DISABLED, name, arg)
#endif
#define crypto_free_irq(irq, arg)                             free_irq(irq, arg)
#define crypto_irqreturn_t                                    irqreturn_t

#define HI_PRINT(fmt...)                                      printk(fmt)

#endif  /* End of #ifndef __HI_DRV_CIPHER_H__*/
