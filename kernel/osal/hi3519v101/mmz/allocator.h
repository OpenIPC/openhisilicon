/*
 * Copyright (c) 2006 Hisilicon Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 */

#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__


#include "osal_mmz.h"

#define NAME_LEN_MAX	64


struct mmz_allocator {
	int (*init)(char *args);
	hil_mmb_t *(*mmb_alloc)(const char *name,
			unsigned long size,
			unsigned long align,
			unsigned long gfp,
			const char *mmz_name,
			hil_mmz_t *_user_mmz);
	hil_mmb_t *(*mmb_alloc_v2)(const char *name,
			unsigned long size,
			unsigned long align,
			unsigned long gfp,
			const char *mmz_name,
			hil_mmz_t *_user_mmz,
			unsigned int order);
	void *(*mmb_map2kern)(hil_mmb_t *mmb, int cached);
	int (*mmb_unmap)(hil_mmb_t *mmb);
	void (*mmb_free)(hil_mmb_t *mmb);
	void *(*mmf_map)(unsigned long phys, int len, int cache);
	void (*mmf_unmap)(void *virt);
};

int cma_allocator_setopt(struct mmz_allocator *allocator);
int hisi_allocator_setopt(struct mmz_allocator *allocator);

#endif
