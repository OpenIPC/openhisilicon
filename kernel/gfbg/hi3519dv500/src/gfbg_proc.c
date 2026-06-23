/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "gfbg_proc.h"
#include "proc_ext.h"
#include "securec.h"
#include "gfbg_main.h"
#include "gfbg_comm.h"

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT

#define MAX_PROC_ENTRIES 16

static osal_proc_entry g_proc_items[MAX_PROC_ENTRIES];

td_void gfbg_proc_add_module(const td_char *entry_name, gfbg_proc_show show, gfbg_proc_write write,
                             gfbg_proc_ioctl ioctl, td_void *data)
{
    td_s32 i, ret;
    osal_proc_entry* proc_item = TD_NULL;
    ot_unused(ioctl);

    for (i = 0; i < MAX_PROC_ENTRIES; i++) {
        if (!g_proc_items[i].proc_dir_entry) {
            break;
        }
    }
    if (i == MAX_PROC_ENTRIES) {
        osal_printk("gfbg proc num full. \n");
        return;
    }

    proc_item = osal_create_proc_entry(entry_name, TD_NULL);
    if (proc_item == TD_NULL) {
        osal_printk("create proc err. \n");
        return;
    }

    proc_item->write = write;
    proc_item->read = show,
    proc_item->private_data = data;

    ret = memcpy_s(&g_proc_items[i], sizeof(osal_proc_entry), proc_item, sizeof(osal_proc_entry));
    gfbg_unequal_eok_return_void(ret);
}

td_void gfbg_proc_remove_module(const char *entry_name)
{
    td_s32 i;

    if (entry_name == TD_NULL) {
        return;
    }

    for (i = 0; i < MAX_PROC_ENTRIES; i++) {
        if (!strcmp(g_proc_items[i].name, entry_name)) {
            break;
        }
    }

    if (i == MAX_PROC_ENTRIES) {
        return;
    }

    osal_remove_proc_entry(entry_name, TD_NULL);
    (td_void)memset_s(&g_proc_items[i], sizeof(osal_proc_entry), 0, sizeof(osal_proc_entry));

    return;
}

td_void gfbg_proc_init(td_void)
{
    /* gfbg proc:Uniformly hang under umap, not separately under graphic */
    (td_void)memset_s(g_proc_items, sizeof(g_proc_items), 0, sizeof(g_proc_items));
}

#endif
