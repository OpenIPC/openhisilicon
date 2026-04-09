/*
* Copyright (c) 2018 HiSilicon Technologies Co., Ltd.
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/

#include <linux/module.h>
#include <linux/fs.h>  /* everything... */
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#include <hifb_proc.h>
#include <linux/slab.h>
#include "proc_ext.h"
#include "hifb_main.h"

#define MAX_PROC_ENTRIES 16


static osal_proc_entry_t s_proc_items[MAX_PROC_ENTRIES];


#ifndef DISABLE_DEBUG_INFO
HI_VOID HIFB_PROC_AddModule( HI_CHAR * entry_name,
                                       HIFB_PROC_SHOW show,
                                       HIFB_PROC_WRITE write,
                                       HIFB_PROC_IOCTL ioctl,
                                       HI_VOID * data)
{
    HI_S32 i;
    osal_proc_entry_t* pstProcItem = NULL;

    for (i = 0; i < MAX_PROC_ENTRIES; i++)
    {
        if (!s_proc_items[i].proc_dir_entry)
        {
            break;
        }
    }
    if (MAX_PROC_ENTRIES == i)
    {
        printk("hifb proc num full. \n");
        return ;
    }

    pstProcItem = osal_create_proc_entry(entry_name, NULL);
    if (!pstProcItem)
    {
        printk("create proc err. \n");
        return;
    }

    pstProcItem->write = write;
    pstProcItem->read = show,
    pstProcItem->private = data;

    osal_memcpy(&s_proc_items[i], pstProcItem, sizeof(osal_proc_entry_t));
}


HI_VOID HIFB_PROC_RemoveModule(const char *entry_name)
{
    HI_S32 i;

    for (i = 0; i < MAX_PROC_ENTRIES; i++)
    {
        if (! strcmp(s_proc_items[i].name, entry_name))
        {
            break;
        }
    }

    if (MAX_PROC_ENTRIES == i)
    {
        return ;
    }

    osal_remove_proc_entry(entry_name, NULL);
    osal_memset(&s_proc_items[i], 0, sizeof(osal_proc_entry_t));

    return;
}

HI_VOID HIFB_PROC_RemoveAllModule(HI_VOID)
{
    HI_S32 i;

    for (i = 0; i < MAX_PROC_ENTRIES; i++)
    {
        if(NULL == s_proc_items[i].proc_dir_entry)
        {
            continue;
        }

        osal_remove_proc_entry(s_proc_items[i].name, NULL);
        osal_memset(&s_proc_items[i], 0, sizeof(osal_proc_entry_t));
    }
}


HI_VOID HIFB_PROC_Init(HI_VOID)
{
    /* hifb proc:Uniformly hang under umap, not separately under graphic */
    osal_memset(s_proc_items, 0, sizeof(s_proc_items));
}

HI_VOID HIFB_PROC_Cleanup(HI_VOID)
{
    return;
}
#endif
