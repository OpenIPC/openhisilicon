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

#ifndef __UMAPDRV_PROC_H__
#define __UMAPDRV_PROC_H__

#include "hi_type.h"
#include "hi_osal.h"

typedef HI_S32 (*HIFB_PROC_SHOW)(struct osal_proc_dir_entry *entry);

typedef HI_S32 (*HIFB_PROC_WRITE)(struct osal_proc_dir_entry *entry, const char *buf, int count, long long *);


typedef HI_S32 (*HIFB_PROC_IOCTL)(unsigned int cmd, unsigned long arg, void *private_data);

typedef struct struHIFB_PROC_ITEM{
    HI_CHAR entry_name[32];
    struct osal_proc_dir_entry *entry;
    HIFB_PROC_SHOW  show;
    HIFB_PROC_WRITE write;
    HIFB_PROC_IOCTL ioctl;
    HI_VOID *data;
}HIFB_PROC_ITEM_S;

extern HI_VOID HIFB_PROC_AddModule(HI_CHAR *, HIFB_PROC_SHOW show, HIFB_PROC_WRITE write, HIFB_PROC_IOCTL ioctl, HI_VOID *);
extern HI_VOID HIFB_PROC_RemoveModule(const HI_CHAR *);
extern HI_VOID HIFB_PROC_RemoveAllModule(HI_VOID);

extern HI_VOID HIFB_PROC_Init(HI_VOID);
extern HI_VOID HIFB_PROC_Cleanup(HI_VOID);

#endif /* __UMAPDRV_PROC_H__ */


