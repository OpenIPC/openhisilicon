/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: hifb hal operation
 */
#ifndef __HIFB_DMABUF_H__
#define __HIFB_DMABUF_H__

#include <linux/dma-buf.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

struct dma_buf *hifb_memblock_export(phys_addr_t base, size_t size, int flags);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __HIFB_DMABUF_H__ */
