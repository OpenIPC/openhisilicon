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
#ifndef __HIFB_DMABUF_H__
#define __HIFB_DMABUF_H__

#include <linux/dma-buf.h>

#ifdef __cplusplus
#if __cplusplus
   extern "C"
{
#endif
#endif

struct dma_buf *hifb_memblock_export(phys_addr_t base, size_t size, int flags);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __HIFB_DMABUF_H__ */
