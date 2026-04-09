/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: hifb dma-buf release
 * Author: sdk
 * Create: 2019-04-19
 */

#include <linux/dma-buf.h>
#include <linux/slab.h>
#include <linux/memblock.h>
#include <linux/highmem.h>
#include <linux/version.h>
#include "hi_type.h"

#define hi_unused(x) ((x) = (x))

struct hifb_mem_block_pdata {
    phys_addr_t mem_base;
};

static struct sg_table *hifb_memblock_map(struct dma_buf_attachment *buf_attach,
                                          enum dma_data_direction data_direction)
{
    hi_s32 i;
    hi_s32 ret;
    hi_ulong page_pfn;
    struct hifb_mem_block_pdata *mem_block_data = NULL;
    struct page *fb_page = NULL;
    struct sg_table *fb_table = NULL;
    struct scatterlist *sg_list = NULL;

    hi_unused(data_direction);

    if ((buf_attach == NULL) ||
        (buf_attach->dmabuf == NULL) ||
        (buf_attach->dmabuf->priv == NULL)) {
        return NULL;
    }

    mem_block_data = buf_attach->dmabuf->priv;
    if (mem_block_data == NULL) {
        return NULL;
    }

    page_pfn = PFN_DOWN(mem_block_data->mem_base);
    fb_page = pfn_to_page(page_pfn);
    if (fb_page == NULL) {
        return NULL;
    }

    fb_table = kzalloc(sizeof(*fb_table), GFP_KERNEL);
    if (fb_table == NULL) {
        return ERR_PTR(-ENOMEM);
    }

    ret = sg_alloc_table(fb_table, 1, GFP_KERNEL);
    if (ret < 0) {
        goto err;
    }

    sg_set_page(fb_table->sgl, fb_page, buf_attach->dmabuf->size, 0);
    for_each_sg(fb_table->sgl, sg_list, fb_table->nents, i)
    if (sg_list) {
        sg_dma_address(sg_list) = sg_phys(sg_list);
    }

    return fb_table;
err:
    kfree(fb_table);
    return ERR_PTR(ret);
}

static hi_void hifb_memblock_unmap(struct dma_buf_attachment *buf_attach,
                                   struct sg_table *fb_table,
                                   enum dma_data_direction data_direction)
{
    hi_unused(buf_attach);
    hi_unused(data_direction);
    if (fb_table != NULL) {
        sg_free_table(fb_table);
    }
}

static hi_void __init_memblock hifb_memblock_release(struct dma_buf *fb_dma_buf)
{
    hi_unused(fb_dma_buf);
}

static hi_void *hifb_memblock_do_kmap(struct dma_buf *fb_dma_buf, hi_ulong pgoffset, bool atomic)
{
    hi_ulong page_pfn;
    struct hifb_mem_block_pdata *mem_block_data = NULL;
    struct page *fb_page = NULL;

    if ((fb_dma_buf == NULL) || (fb_dma_buf->priv == NULL)) {
        return NULL;
    }

    mem_block_data = fb_dma_buf->priv;
    if (mem_block_data == NULL) {
        return NULL;
    }

    page_pfn = PFN_DOWN(mem_block_data->mem_base) + pgoffset;
    fb_page = pfn_to_page(page_pfn);
    if (fb_page == NULL) {
        return NULL;
    }

    if (atomic) {
        return kmap_atomic(fb_page);
    }
    else {
        return kmap(fb_page);
    }
}

static hi_void *hifb_memblock_kmap_atomic(struct dma_buf *fb_dma_buf, hi_ulong pgoffset)
{
    return hifb_memblock_do_kmap(fb_dma_buf, pgoffset, true);
}

static hi_void hifb_memblock_kunmap_atomic(struct dma_buf *fb_dma_buf, hi_ulong pgoffset, hi_void *vir_addr)
{
    hi_unused(fb_dma_buf);
    hi_unused(pgoffset);
    if (vir_addr != NULL) {
        kunmap_atomic(vir_addr);
    }
}

static hi_void *hifb_memblock_kmap(struct dma_buf *fb_dma_buf, hi_ulong pgoffset)
{
    return hifb_memblock_do_kmap(fb_dma_buf, pgoffset, false);
}

static hi_void hifb_memblock_kunmap(struct dma_buf *fb_dma_buf, hi_ulong pgoffset, hi_void *vir_addr)
{
    hi_unused(fb_dma_buf);
    hi_unused(pgoffset);
    if (vir_addr != NULL) {
        kunmap(vir_addr);
    }
}

static hi_s32 hifb_memblock_mmap(struct dma_buf *fb_dma_buf, struct vm_area_struct *vma_area)
{
    hi_s32 size;
    struct hifb_mem_block_pdata *mem_block_data = NULL;

    if ((fb_dma_buf == NULL) || (vma_area == NULL)) {
        return -1;
    }

    mem_block_data = fb_dma_buf->priv;
    if (mem_block_data == NULL) {
        return -1;
    }

    if (mem_block_data->mem_base == 0) {
        return -1;
    }

    size =  vma_area->vm_end - vma_area->vm_start;
    if (size == 0) {
        return -1;
    }

    vma_area->vm_page_prot =  pgprot_writecombine(vma_area->vm_page_prot);

    return remap_pfn_range(vma_area, vma_area->vm_start, PFN_DOWN(mem_block_data->mem_base),
                           vma_area->vm_end - vma_area->vm_start, vma_area->vm_page_prot);
}

struct dma_buf_ops g_hifb_mem_block_ops = {
    .map_dma_buf   = hifb_memblock_map,
    .unmap_dma_buf = hifb_memblock_unmap,
    .release       = hifb_memblock_release,
    .kmap_atomic   = hifb_memblock_kmap_atomic,
    .kunmap_atomic = hifb_memblock_kunmap_atomic,
    .kmap          = hifb_memblock_kmap,
    .kunmap        = hifb_memblock_kunmap,
    .mmap          = hifb_memblock_mmap,
};

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,0,0)
struct dma_buf *hifb_memblock_export(phys_addr_t mem_base, size_t mem_size, hi_s32 flags)
{
    struct hifb_mem_block_pdata *mem_block_data = NULL;
    struct dma_buf *fb_dma_buf = NULL;

    if ((mem_base != PAGE_ALIGN(mem_base)) || (mem_size != PAGE_ALIGN(mem_size)))
        return ERR_PTR(-EINVAL);

    mem_block_data = kzalloc(sizeof(*mem_block_data), GFP_KERNEL);
    if (mem_block_data == NULL) {
        return ERR_PTR(-ENOMEM);
    }

    mem_block_data->mem_base = mem_base;
    fb_dma_buf = dma_buf_export(mem_block_data, &g_hifb_mem_block_ops, mem_size, flags, NULL);
    if (IS_ERR(fb_dma_buf))
        kfree(mem_block_data);

    return fb_dma_buf;
}

#else

struct dma_buf *hifb_memblock_export(phys_addr_t mem_base, size_t mem_size, hi_s32 flags)
{
    DEFINE_DMA_BUF_EXPORT_INFO(export_info);
    struct dma_buf *fb_dma_buf = NULL;

    if ((mem_base != PAGE_ALIGN(mem_base)) || (mem_size != PAGE_ALIGN(mem_size))) {
        return ERR_PTR(-EINVAL);
    }

    export_info.ops      = &g_hifb_mem_block_ops;
    export_info.size     = mem_size;
    export_info.flags    = flags;
    export_info.resv     = NULL;
    export_info.priv     = (hi_void *)mem_base;

    fb_dma_buf = dma_buf_export(&export_info);

    return fb_dma_buf;
}
#endif
