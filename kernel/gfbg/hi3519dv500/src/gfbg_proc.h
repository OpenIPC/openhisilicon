/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef GFBG_PROC_H
#define GFBG_PROC_H

#include "ot_type.h"
#include "ot_osal.h"

#define GFBG_ENTRY_NAME_RANGE 32

typedef td_s32 (*gfbg_proc_show)(struct osal_proc_dir_entry *entry);

typedef td_s32 (*gfbg_proc_write)(struct osal_proc_dir_entry *entry, const char *buf, int count, long long *);

typedef td_s32 (*gfbg_proc_ioctl)(unsigned int cmd, unsigned long arg, td_u32 *private_data);

typedef struct {
    td_char entry_name[GFBG_ENTRY_NAME_RANGE];
    struct osal_proc_dir_entry *entry;
    gfbg_proc_show show;
    gfbg_proc_write write;
    gfbg_proc_ioctl ioctl;
    td_void *data;
} gfbg_proc_item;

extern td_void gfbg_proc_add_module(const td_char *, gfbg_proc_show, gfbg_proc_write, gfbg_proc_ioctl, td_void *);
extern td_void gfbg_proc_remove_module(const td_char *);
extern td_void gfbg_proc_init(td_void);

#endif /* GFBG_PROC_H */
