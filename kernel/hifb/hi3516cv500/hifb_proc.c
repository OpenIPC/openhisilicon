/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: hifb hal operation
 */

#include <linux/module.h>
#include <linux/fs.h> /* everything... */
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include "hifb_proc.h" /* because of the redefined warning, fs.h go first. */
#include "proc_ext.h"
#include "hifb_main.h"

#define MAX_PROC_ENTRIES 16

static osal_proc_entry_t g_proc_items[MAX_PROC_ENTRIES];

#ifndef DISABLE_DEBUG_INFO
hi_void hifb_proc_add_module(hi_char *entry_name, hi_u32 size, hifb_proc_show show,
                             hifb_proc_write write, hi_void *data)
{
    hi_s32 i;
    osal_proc_entry_t *proc_item = HI_NULL;

    for (i = 0; i < MAX_PROC_ENTRIES; i++) {
        if (!g_proc_items[i].proc_dir_entry) {
            break;
        }
    }
    if (i == MAX_PROC_ENTRIES) {
        printk("hifb proc num full. \n");
        return;
    }

    proc_item = osal_create_proc_entry(entry_name, HI_NULL);
    if (proc_item == HI_NULL) {
        printk("create proc err. \n");
        return;
    }

    proc_item->write = write;
    proc_item->read = show,
    proc_item->private = data;

    osal_memcpy(&g_proc_items[i], proc_item, sizeof(osal_proc_entry_t));
}

hi_void hifb_proc_remove_module(const char *entry_name)
{
    hi_s32 i;

    for (i = 0; i < MAX_PROC_ENTRIES; i++) {
        if (!strcmp(g_proc_items[i].name, entry_name)) {
            break;
        }
    }

    if (i == MAX_PROC_ENTRIES) {
        return;
    }

    osal_remove_proc_entry(entry_name, HI_NULL);
    osal_memset(&g_proc_items[i], 0, sizeof(osal_proc_entry_t));

    return;
}

hi_void hifb_proc_remove_all_module(hi_void)
{
    hi_s32 i;

    for (i = 0; i < MAX_PROC_ENTRIES; i++) {
        if (g_proc_items[i].proc_dir_entry == HI_NULL) {
            continue;
        }

        osal_remove_proc_entry(g_proc_items[i].name, HI_NULL);
        osal_memset(&g_proc_items[i], 0, sizeof(osal_proc_entry_t));
    }
}

hi_void hifb_proc_init(hi_void)
{
    /* hifb proc:Uniformly hang under umap, not separately under graphic */
    osal_memset(g_proc_items, 0, sizeof(g_proc_items));
}

#endif
