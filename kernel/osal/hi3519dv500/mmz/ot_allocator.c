/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "securec.h"
#include "mmz_common.h"
#include "allocator.h"
#include "media_mem.h"

long long g_ot_max_malloc_size = 0x40000000UL;        /* 1GB */

static int calculate_fixed_region(unsigned long end, unsigned long start,
    unsigned long *fixed_start, unsigned long *fixed_len, unsigned long len)
{
    unsigned long blank_len;
    int ret = 0;

    blank_len = end - start;
    if ((blank_len < *fixed_len) && (blank_len >= len)) {
        *fixed_len = blank_len;
        *fixed_start = start;
        mmz_debug_info("start=0x%08lX, len=%luKB\n", *fixed_start, *fixed_len / SZ_1K);
        ret = 1;
    }
    return ret;
}

static unsigned long find_fixed_region(unsigned long *region_len,
    ot_mmz_t *mmz, unsigned long size, unsigned long align)
{
    unsigned long start, len;
    unsigned long fixed_start = 0;
    unsigned long fixed_len = -1;
    ot_mmb_t *p = NULL;

    align = mmz_grain_align(align);
    if (align == 0) {
        align = MMZ_GRAIN;
    }
    start = mmz_align2(mmz->phys_start, align);
    len = mmz_grain_align(size);

    osal_list_for_each_entry(p, &mmz->mmb_list, list) {
        ot_mmb_t *next = NULL;
        mmz_debug_info("p->phys_addr=0x%08lX p->length = %luKB \t", p->phys_addr, p->length / SZ_1K);
        next = osal_list_entry(p->list.next, typeof(*p), list);
        mmz_debug_info(",next = 0x%08lX\n\n", next->phys_addr);
        /*
         * if p is the first entry or not.
         */
        if (osal_list_first_entry(&mmz->mmb_list, typeof(*p), list) == p) {
            (void)calculate_fixed_region(p->phys_addr, start, &fixed_start, &fixed_len, len);
        }
        start = mmz_align2((p->phys_addr + p->length), align);
        /* if aglin is larger than mmz->nbytes, it would trigger the BUG_ON */
        /* if we have to alloc after the last node.  */
        if (osal_list_is_last(&p->list, &mmz->mmb_list)) {
            if (calculate_fixed_region(mmz->phys_start + mmz->nbytes, start, &fixed_start, &fixed_len, len) == 1) {
                break;
            }
            if (fixed_len == (unsigned long)-1) {
                fixed_start = 0;
                mmz_debug_info("start=0x%08lX, len=%luKB\n", fixed_start, fixed_len / SZ_1K);
            }
            goto out;
        }
        /* blank is too small */
        if ((start + len) > next->phys_addr) {
            mmz_debug_info("start=0x%08lX, len=%lu, next=0x%08lX\n", start, len, next->phys_addr);
            continue;
        }

        (void)calculate_fixed_region(next->phys_addr, start, &fixed_start, &fixed_len, len);
    }

    if ((mmz_grain_align(start + len) <= (mmz->phys_start + mmz->nbytes)) &&
        (start >= mmz->phys_start) && (start < (mmz->phys_start + mmz->nbytes))) {
        fixed_start = start;
    } else {
        fixed_start = 0;
    }

    mmz_debug_info("start=0x%08lX, len=%luKB\n", fixed_start, len / SZ_1K);
out:
    *region_len = len;
    return fixed_start;
}

int do_mmb_alloc(ot_mmb_t *mmb)
{
    ot_mmb_t *p = NULL;

    /* add mmb sorted */
    osal_list_for_each_entry(p, &mmb->zone->mmb_list, list) {
        if (mmb->phys_addr < p->phys_addr) {
            break;
        }
        if (mmb->phys_addr == p->phys_addr) {
            mmz_err_trace("media-mem allocator bad in %s!", mmb->zone->name);
            return -1;
        }
    }
    osal_list_add(&mmb->list, p->list.prev);
    mmz_debug_info(OT_MMB_FMT_S, ot_mmb_fmt_arg(mmb));
    return 0;
}

ot_mmb_t *mmb_real_alloc(const ot_mmz_alloc_para_in *alloc_para, ot_mmz_t *fixed_mmz,
    unsigned long fixed_start, unsigned long size)
{
    ot_mmb_t *mmb = NULL;
    errno_t err_value;
    const char *name = alloc_para->buf_name;
    int i;

    mmb = osal_kmalloc(sizeof(ot_mmb_t), OSAL_GFP_KERNEL);
    if (mmb == NULL) {
        return NULL;
    }

    (void)memset_s(mmb, sizeof(ot_mmb_t), 0, sizeof(ot_mmb_t));

    mmb->zone      = fixed_mmz;
    mmb->phys_addr = fixed_start;
    mmb->length    = size;

    mmb->kernel_only = alloc_para->kernel_only;
    if (media_mem_is_check_pid()) {
        mmb->check_pid = 1;
        mmb->pid_num   = 1;
        mmb->pid[0]    = osal_get_current_tgid();
        for (i = 1; i < OT_MMB_MAX_PID_NUM; ++i) {
            mmb->pid[i] = -1;
        }
    }

    if (name != NULL) {
        err_value = strncpy_s(mmb->name, OT_MMB_NAME_LEN, name, OT_MMB_NAME_LEN - 1);
    } else {
        err_value = strncpy_s(mmb->name, OT_MMB_NAME_LEN, "<null>", OT_MMB_NAME_LEN - 1);
    }

    if ((err_value != EOK) || do_mmb_alloc(mmb)) {
        osal_kfree(mmb);
        return NULL;
    }
    return mmb;
}

static ot_mmb_t *allocator_mmb_alloc(const ot_mmz_alloc_para_in *para_in)
{
    ot_mmz_t *mmz = NULL;
    unsigned long start, region_len;
    unsigned long fixed_start = 0;
    unsigned long fixed_len = ~1;
    ot_mmz_t *fixed_mmz = NULL;

    const char *mmz_name = para_in->mmz_name;
    unsigned long size = para_in->size;
    unsigned long align = para_in->align;
    unsigned long gfp = para_in->gfp;

    if ((size == 0) || (size > g_ot_max_malloc_size)) {
        return NULL;
    }
    if (align == 0) {
        align = MMZ_GRAIN;
    }
    size = mmz_grain_align(size);

    mmz_debug_info("size=%luKB, align=%lu", size / SZ_1K, align);

    osal_list_for_each_entry(mmz, ot_mmz_get_mmz_list(), list) {
        if (mmz_default_filter(mmz, gfp, mmz_name) != 0) {
            continue;
        }
        start = find_fixed_region(&region_len, mmz, size, align);
        if ((fixed_len > region_len) && (start != 0)) {
            fixed_len = region_len;
            fixed_start = start;
            fixed_mmz = mmz;
        }
    }

    if (fixed_mmz == NULL) {
        return NULL;
    }

    return mmb_real_alloc(para_in, fixed_mmz, fixed_start, size);
}

static void *allocator_mmb_map2kern(ot_mmb_t *mmb, int cached)
{
    /*
      * already mapped? no need to remap again,
      * just return mmb's kernel virtual address.
      */
    if (mmb->flags & OT_MMB_MAP2KERN) {
        if ((!!cached * OT_MMB_MAP2KERN_CACHED) != (mmb->flags & OT_MMB_MAP2KERN_CACHED)) {
            mmz_err_trace("mmb<%s> has been kernel-mapped as %s, can not be re-mapped as %s.", mmb->name,
                (mmb->flags & OT_MMB_MAP2KERN_CACHED) ? "cached" : "non-cached",
                (cached) ? "cached" : "non-cached");
            return NULL;
        }

        mmb->map_ref++;

        return mmb->kvirt;
    }

    if (cached) {
        mmb->flags |= OT_MMB_MAP2KERN_CACHED;
        mmb->kvirt = osal_ioremap_cached(mmb->phys_addr, mmb->length);
    } else {
        mmb->flags &= ~OT_MMB_MAP2KERN_CACHED;
        /* ioremap_wc has better performance */
        mmb->kvirt = osal_ioremap_wc(mmb->phys_addr, mmb->length);
    }

    if (mmb->kvirt) {
        mmb->flags |= OT_MMB_MAP2KERN;
        mmb->map_ref++;
    } else {
        mmb->flags &= ~OT_MMB_MAP2KERN_CACHED;
    }
    return mmb->kvirt;
}

static void allocator_mmb_free(ot_mmb_t *mmb)
{
    if (mmb->flags & OT_MMB_MAP2KERN_CACHED) {
        osal_dcache_region_wb((void *)mmb->kvirt, mmb->phys_addr, mmb->length);
    }

    osal_list_del(&mmb->list);
    osal_kfree(mmb);
}

static int allocator_mmb_unmap(ot_mmb_t *mmb)
{
    int ref;

    if (mmb->flags & OT_MMB_MAP2KERN_CACHED) {
        osal_dcache_region_wb((void *)mmb->kvirt, mmb->phys_addr, mmb->length);
    }

    if (mmb->flags & OT_MMB_MAP2KERN) {
        ref = --mmb->map_ref;
        if (mmb->map_ref != 0) {
            return ref;
        }
        osal_iounmap(mmb->kvirt, mmb->length);
    }

    mmb->kvirt = NULL;
    mmb->flags &= ~OT_MMB_MAP2KERN;
    mmb->flags &= ~OT_MMB_MAP2KERN_CACHED;

    if ((mmb->flags & OT_MMB_RELEASED) && (mmb->phy_ref == 0)) {
        allocator_mmb_free(mmb);
    }
    return 0;
}

static void *allocator_mmf_map(phys_addr_t phys, int len, int cache)
{
    void *virt = NULL;
    if (cache) {
        virt = osal_ioremap_cached(phys, len);
    } else {
        virt = osal_ioremap_wc(phys, len);
    }
    return virt;
}

static void allocator_mmf_unmap(void *virt)
{
    if (virt != NULL) {
        osal_iounmap(virt, 0);
    }
}

static int allocator_init_parse_args(ot_mmz_t **zone, int argc, char **argv)
{
    ot_mmz_t *mmz_zone = NULL;

    if (argc == 4) { /* 4:Number of parameters */
        mmz_zone = ot_mmz_create("null", 0, 0, 0);
    } else {
        mmz_err_trace("error parameters\n");
        return -EINVAL;
    }
    if (mmz_zone == NULL) {
        return 0;
    }
    if (strncpy_s(mmz_zone->name, OT_MMZ_NAME_LEN, argv[0], OT_MMZ_NAME_LEN - 1) != EOK) {
        mmz_err_trace("strncpy_s failed!\n");
        ot_mmz_destroy(mmz_zone);
        return -1;
    }
    mmz_zone->gfp        = mmz_strtoul_ex(argv[1], NULL, 0); /* 1: the second args */
    mmz_zone->phys_start = mmz_strtoul_ex(argv[2], NULL, 0); /* 2: the third args */
    mmz_zone->nbytes     = mmz_strtoul_ex(argv[3], NULL, 0); /* 3: the fourth args */
    if (mmz_zone->nbytes > g_ot_max_malloc_size) {
        g_ot_max_malloc_size = mmz_zone->nbytes;
    }
    *zone = mmz_zone;
    return 1;
}

static int allocator_init(char *s)
{
    ot_mmz_t *zone = NULL;
    char *line = NULL;
    unsigned long phys_end;
    int ret;

    while ((line = strsep(&s, ":")) != NULL) {
        int i;
        char *argv[MMZ_PARAM_NUM];

        for (i = 0; (argv[i] = strsep(&line, ",")) != NULL;) {
            if (++i == ARRAY_SIZE(argv)) {
                break;
            }
        }

        ret = allocator_init_parse_args(&zone, i, argv);
        if (ret == 0) {
            continue;
        } else if (ret < 0) {
            return ret;
        }

        if (ot_mmz_register(zone)) {
            mmz_warn_trace("Add MMZ failed: " OT_MMZ_FMT_S "\n", ot_mmz_fmt_arg(zone));
            ot_mmz_destroy(zone);
            return -1;
        }

        /* if phys_end is maximum value (ex, 0xFFFFFFFF 32bit) */
        phys_end = (zone->phys_start + zone->nbytes);

        if ((phys_end == 0) && (zone->nbytes >= PAGE_SIZE)) {
            /* reserve last PAGE_SIZE memory */
            zone->nbytes = zone->nbytes - PAGE_SIZE;
        }

        /* if phys_end exceed 0xFFFFFFFF (32bit), wrapping error */
        if ((zone->phys_start > phys_end) && (phys_end != 0)) {
            mmz_err_trace("MMZ: parameter is not correct! Address exceeds 0xFFFFFFFF\n");
            ot_mmz_unregister(zone);
            ot_mmz_destroy(zone);
            return -1;
        }

        zone = NULL;
    }

    return 0;
}

int ot_allocator_setopt(struct mmz_allocator *allocator)
{
    if (allocator == NULL) {
        mmz_err_trace("error parameters\n");
        return -1;
    }
    allocator->init         = allocator_init;
    allocator->mmb_alloc    = allocator_mmb_alloc;
    allocator->mmb_map2kern = allocator_mmb_map2kern;
    allocator->mmb_unmap    = allocator_mmb_unmap;
    allocator->mmb_free     = allocator_mmb_free;
    allocator->mmf_map      = allocator_mmf_map;
    allocator->mmf_unmap    = allocator_mmf_unmap;
    return 0;
}
