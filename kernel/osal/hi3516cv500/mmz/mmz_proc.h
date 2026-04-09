/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2016-2019.
 * Description: mmz proc header file.
 * Author: Hisilicon multimedia software group
 * Create: 2016-11-11
 */

#ifndef __MMZ_PROC_H__
#define __MMZ_PROC_H__

#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#include "osal_mmz.h"

extern int mmz_read_proc(struct seq_file *sfile);
static void *mmz_seq_start(struct seq_file *s, loff_t *pos)
{
    static unsigned long counter;

    if (*pos == 0) {
        return &counter;
    } else {
        *pos = 0;
        return NULL;
    }
}

static void *mmz_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
    (*pos)++;
    return NULL;
}

static void mmz_seq_stop(struct seq_file *s, void *v)
{
}

static int mmz_seq_show(struct seq_file *sfile, void *v)
{
    return mmz_read_proc(sfile);
}

#endif