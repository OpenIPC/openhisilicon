/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef MEDIA_MEM_H
#define MEDIA_MEM_H

#include <linux/version.h>
#include "mmz.h"

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
#define mmz_map_down(mm)    down_read(&(mm)->mmap_lock)
#define mmz_map_up(mm)      up_read(&(mm)->mmap_lock)
#else
#define mmz_map_down(mm)    down_read(&(mm)->mmap_sem)
#define mmz_map_up(mm)      up_read(&(mm)->mmap_sem)
#endif

unsigned long mmz_strtoul_ex(const char *s, char **ep, unsigned int base);

ot_mmb_t *media_mem_get_mmb_by_handle(const void *mem_handle);
ot_mmb_t *media_mem_get_mmb_by_phys_and_size(unsigned long phys_addr, unsigned long size);

void media_mem_clear_pid_in_all_mmb(int pid);

static inline int media_mem_is_check_pid(void)
{
    return ot_mmz_get_mem_process_isolation();
}

/* function returns the index of pid array */
static inline int find_pid_in_mmb(const ot_mmb_t *mmb, int pid)
{
    int i;
    for (i = 0; i < mmb->pid_num; ++i) {
        if (mmb->pid[i] == pid) {
            return i;
        }
    }
    return -1;
}

static inline void clear_pid_in_mmb(ot_mmb_t *mmb, int pos)
{
    int cur = pos;

    for (mmb->pid_num--; cur < mmb->pid_num; ++cur) {
        mmb->pid[cur] = mmb->pid[cur + 1];
    }
    mmb->pid[mmb->pid_num] = -1;
}

#endif  /* MEDIA_MEM_H */
