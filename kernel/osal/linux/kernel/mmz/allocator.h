/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include <linux/device.h>
#include "osal_mmz.h"

#define NAME_LEN_MAX 64

struct mmz_allocator {
    int (*init)(char *args);
    mmz_mmb_t *(*mmb_alloc)(const char *name,
                            unsigned long size,
                            unsigned long align,
                            unsigned long gfp,
                            const char *mmz_name,
                            mmz_mmz_t *_user_mmz);
    mmz_mmb_t *(*mmb_alloc_v2)(const char *name,
                               unsigned long size,
                               unsigned long align,
                               unsigned long gfp,
                               const char *mmz_name,
                               mmz_mmz_t *_user_mmz,
                               unsigned int order);
    void *(*mmb_map2kern)(mmz_mmb_t *mmb, int cached);
    int (*mmb_unmap)(mmz_mmb_t *mmb);
    void (*mmb_free)(mmz_mmb_t *mmb);
    void *(*mmf_map)(phys_addr_t phys, int len, int cache);
    void (*mmf_unmap)(void *virt);
};

int cma_allocator_setopt(struct mmz_allocator *allocator);
int allocator_setopt(struct mmz_allocator *allocator);

#endif
