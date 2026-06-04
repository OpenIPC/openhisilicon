/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */
#ifndef DRV_MIPIRX_OSAL_H
#define DRV_MIPIRX_OSAL_H

#include "ot_type.h"
#include "ot_debug.h"
#include "ot_osal.h"
#include "securec.h"

#define mipirx_log_err(fmt, ...) osal_printk("[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

td_s32 drv_mipirx_get_reg_base(td_void *pdev, td_u32 **reg_base, const td_char *name);
td_s32 drv_mipirx_get_irq_num(td_void *pdev, td_u32 *irq);
td_s32 drv_mipirx_register_irq(td_u32 irq_num, td_u32 (*func)(td_u32, void *));
td_s32 drv_mipirx_unregister_irq(td_u32 irq_num, td_u32 (*func)(td_u32, void *));
td_s32 drv_mipirx_init_mutex(td_void);
td_void drv_mipirx_deinit_mutex(td_void);
td_s32 drv_mipirx_interruptible_mutex_lock(td_void);
td_void drv_mipirx_mutex_unlock(td_void);

#endif
