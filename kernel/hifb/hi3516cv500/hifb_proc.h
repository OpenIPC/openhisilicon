/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: hifb hal operation
 */

#ifndef __UMAPDRV_PROC_H__
#define __UMAPDRV_PROC_H__

#include "hi_type.h"
#include "hi_osal.h"

#define HIFB_ENTRY_NAME_RANGE 32

typedef hi_s32 (*hifb_proc_show)(struct osal_proc_dir_entry *entry);

typedef hi_s32 (*hifb_proc_write)(struct osal_proc_dir_entry *entry, const char *buf, int count, long long *);

typedef hi_s32 (*hifb_proc_ioctl)(unsigned int cmd, unsigned long arg, hi_u32 *private_data);

typedef struct {
    hi_char entry_name[HIFB_ENTRY_NAME_RANGE];
    struct osal_proc_dir_entry *entry;
    hifb_proc_show show;
    hifb_proc_write write;
    hifb_proc_ioctl ioctl;
    hi_void *data;
} hifb_proc_item;

hi_void hifb_proc_add_module(hi_char *, hi_u32, hifb_proc_show, hifb_proc_write, hi_void *);
hi_void hifb_proc_remove_module(const hi_char *);
hi_void hifb_proc_remove_all_module(hi_void);

hi_void hifb_proc_init(hi_void);

#endif /* __UMAPDRV_PROC_H__ */
