/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __MMZ_H__
#define __MMZ_H__

#include <linux/types.h>

unsigned long long mmz_alloc(const char *mmz_name, const char *buf_name, unsigned long size);

void mmz_free(unsigned long long phy_addr);

void *mmz_map(unsigned long long phy_addr, unsigned long size, bool cached);

void mmz_unmap(void *vir_addr);

int mmz_check_phyaddr(unsigned long long phy_addr, unsigned long len);

int mmz_flush_cache(unsigned long phy_addr, void *kvirt, unsigned long length);

#endif
