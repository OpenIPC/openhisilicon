/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: hifb dma-buf release
 */

#include "hifb_dmabuf.h"
#include <linux/slab.h>
#include <linux/memblock.h>
#include <linux/highmem.h>
#include <linux/version.h>
#include "hi_type.h"

#define hi_unused(x) ((x) = (x))

typedef struct {
    phys_addr_t mem_base;
} hifb_mem_block_pdata;

static struct sg_table *hifb_memblock_map(struct dma_buf_attachment *buf_attach,
                                          enum dma_data_direction data_direction)
{
    hi_s32 i = 0;
    hi_s32 ret;
    hi_ulong page_pfn;
    hifb_mem_block_pdata *mem_block_data = HI_NULL;
    struct page *fb_page = HI_NULL;
    struct sg_table *fb_table = HI_NULL;
    struct scatterlist *sg_list = HI_NULL;

    hi_unused(data_direction);

    if ((buf_attach == HI_NULL) || (buf_attach->dmabuf == HI_NULL) || (buf_attach->dmabuf->priv == HI_NULL)) {
        return HI_NULL;
    }

    mem_block_data = buf_attach->dmabuf->priv;
    if (mem_block_data == HI_NULL) {
        return HI_NULL;
    }

    page_pfn = PFN_DOWN(mem_block_data->mem_base);
    fb_page = pfn_to_page(page_pfn);
    if (fb_page == HI_NULL) {
        return HI_NULL;
    }

    fb_table = kzalloc(sizeof(*fb_table), GFP_KERNEL);
    if (fb_table == HI_NULL) {
        return ERR_PTR(-ENOMEM);
    }

    ret = sg_alloc_table(fb_table, 1, GFP_KERNEL);
    if (ret < 0) {
        kfree(fb_table);
        fb_table = HI_NULL;
        return ERR_PTR(ret);
    }

    sg_set_page(fb_table->sgl, fb_page, buf_attach->dmabuf->size, 0);
    for_each_sg(fb_table->sgl, sg_list, fb_table->nents, i) {
        if (sg_list != HI_NULL) {
            sg_dma_address(sg_list) = sg_phys(sg_list);
        }
    }
    return fb_table;
}

static hi_void hifb_memblock_unmap(struct dma_buf_attachment *buf_attach, struct sg_table *fb_table,
                                   enum dma_data_direction data_direction)
{
    hi_unused(buf_attach);
    hi_unused(data_direction);
    if (fb_table != HI_NULL) {
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
    hifb_mem_block_pdata *mem_block_data = HI_NULL;
    struct page *fb_page = HI_NULL;

    if ((fb_dma_buf == HI_NULL) || (fb_dma_buf->priv == HI_NULL)) {
        return HI_NULL;
    }

    mem_block_data = fb_dma_buf->priv;
    if (mem_block_data == HI_NULL) {
        return HI_NULL;
    }

    page_pfn = PFN_DOWN(mem_block_data->mem_base) + pgoffset;
    fb_page = pfn_to_page(page_pfn);
    if (fb_page == HI_NULL) {
        return HI_NULL;
    }

    if (atomic) {
        return kmap_atomic(fb_page);
    } else {
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
    if (vir_addr != HI_NULL) {
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
    if (vir_addr != HI_NULL) {
        kunmap(vir_addr);
    }
}

static inline int hifb_valid_mmap_phys_addr_range(unsigned long pfn, size_t size)
{
    return (pfn + (size >> PAGE_SHIFT)) <= (1 + (PHYS_MASK >> PAGE_SHIFT));
}

static hi_s32 hifb_memblock_mmap(struct dma_buf *fb_dma_buf, struct vm_area_struct *vma_area)
{
    hi_ulong size;
    hifb_mem_block_pdata *mem_block_data = HI_NULL;

    if ((fb_dma_buf == HI_NULL) || (vma_area == HI_NULL)) {
        return -1;
    }

    mem_block_data = fb_dma_buf->priv;
    if (mem_block_data == HI_NULL) {
        return -1;
    }

    if (mem_block_data->mem_base == 0) {
        return -1;
    }

    size = vma_area->vm_end - vma_area->vm_start;
    if (size == 0) {
        return -1;
    }

    if (!hifb_valid_mmap_phys_addr_range(vma_area->vm_pgoff, size)) {
        return -1;
    }
    vma_area->vm_page_prot = pgprot_writecombine(vma_area->vm_page_prot);

    return remap_pfn_range(vma_area, vma_area->vm_start, PFN_DOWN(mem_block_data->mem_base), size,
                           vma_area->vm_page_prot);
}

struct dma_buf_ops g_hifb_mem_block_ops = {
    .map_dma_buf = hifb_memblock_map,
    .unmap_dma_buf = hifb_memblock_unmap,
    .release = hifb_memblock_release,
    .kmap_atomic = hifb_memblock_kmap_atomic,
    .kunmap_atomic = hifb_memblock_kunmap_atomic,
    .kmap = hifb_memblock_kmap,
    .kunmap = hifb_memblock_kunmap,
    .mmap = hifb_memblock_mmap,
};

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0)
struct dma_buf *hifb_memblock_export(phys_addr_t mem_base, size_t mem_size, hi_s32 flags)
{
    struct hifb_mem_block_pdata *mem_block_data = HI_NULL;
    struct dma_buf *fb_dma_buf = HI_NULL;

    if ((mem_base != PAGE_ALIGN(mem_base)) || (mem_size != PAGE_ALIGN(mem_size))) {
        return ERR_PTR(-EINVAL);
    }
    mem_block_data = kzalloc(sizeof(*mem_block_data), GFP_KERNEL);
    if (mem_block_data == HI_NULL) {
        return ERR_PTR(-ENOMEM);
    }

    mem_block_data->mem_base = mem_base;
    fb_dma_buf = dma_buf_export(mem_block_data, &g_hifb_mem_block_ops, mem_size, flags, HI_NULL);
    if (IS_ERR(fb_dma_buf)) {
        kfree(mem_block_data);
        mem_block_data = HI_NULL;
    }
    return fb_dma_buf;
}
#else
struct dma_buf *hifb_memblock_export(phys_addr_t mem_base, size_t mem_size, hi_s32 flags)
{
    DEFINE_DMA_BUF_EXPORT_INFO(export_info);
    struct dma_buf *fb_dma_buf = HI_NULL;

    if ((mem_base != PAGE_ALIGN(mem_base)) || (mem_size != PAGE_ALIGN(mem_size))) {
        return ERR_PTR(-EINVAL);
    }

    export_info.ops = &g_hifb_mem_block_ops;
    export_info.size = mem_size;
    export_info.flags = flags;
    export_info.resv = HI_NULL;
    export_info.priv = (hi_void *)mem_base;

    fb_dma_buf = dma_buf_export(&export_info);

    return fb_dma_buf;
}
#endif
