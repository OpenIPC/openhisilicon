/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "mmz.h"

#define NAME_LEN_MAX 64

struct mmz_allocator {
    int (*init)(char *args);
    ot_mmb_t *(*mmb_alloc)(const ot_mmz_alloc_para_in *para_in);
    void *(*mmb_map2kern)(ot_mmb_t *mmb, int cached);
    int (*mmb_unmap)(ot_mmb_t *mmb);
    void (*mmb_free)(ot_mmb_t *mmb);
    void *(*mmf_map)(phys_addr_t phys, int len, int cache);
    void (*mmf_unmap)(void *virt);
};

int cma_allocator_setopt(struct mmz_allocator *allocator);
int ot_allocator_setopt(struct mmz_allocator *allocator);

int do_mmb_alloc(ot_mmb_t *mmb);
ot_mmb_t *mmb_real_alloc(const ot_mmz_alloc_para_in *alloc_para, ot_mmz_t *fixed_mmz,
    unsigned long fixed_start, unsigned long size);

#endif
