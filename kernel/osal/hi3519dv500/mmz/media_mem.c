/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "media_mem.h"
#include <linux/highmem.h>
#include "securec.h"
#include "ot_osal.h"
#include "ot_common.h"
#include "mmz_common.h"
#include "allocator.h"

#define MMZ_SETUP_CMDLINE_LEN     256
#define MMZ_ALLOCATOR_NAME_LEN    32

OSAL_LIST_HEAD(g_mmz_list);
OSAL_LIST_HEAD(g_map_mmz_list);

static DEFINE_SEMAPHORE(g_mmz_lock);

static struct mmz_allocator g_the_allocator;

static int  anony = 0;
static bool mem_process_isolation = true;
static char g_setup_zones[MMZ_SETUP_CMDLINE_LEN] = {'\0'};
static char g_mmap_zones[MMZ_SETUP_CMDLINE_LEN] = {'\0'};
static char g_setup_allocator[MMZ_ALLOCATOR_NAME_LEN] = "ot"; /* default setting */

#ifndef MODULE
osal_setup_num_param(anony, anony);
osal_setup_num_param(mem_process_isolation, mem_process_isolation);
osal_setup_str_param(mmz_allocator, g_setup_allocator);
osal_setup_str_param(mmz_zones, g_setup_zones);
osal_setup_str_param(map_mmz, g_mmap_zones);
#else
module_param(anony, int, S_IRUGO);
module_param(mem_process_isolation, bool, S_IRUGO);
module_param_string(mmz, g_setup_zones, MMZ_SETUP_CMDLINE_LEN, 0600);
module_param_string(map_mmz, g_mmap_zones, MMZ_SETUP_CMDLINE_LEN, 0600);
module_param_string(mmz_allocator, g_setup_allocator, MMZ_ALLOCATOR_NAME_LEN, 0600);
MODULE_PARM_DESC(mmz, "mmz_allocator=allocator mmz=name,0,start,size:[others] map_mmz=start,size:[others]");
#endif

int ot_mmz_get_anony(void)
{
    return anony;
}

struct osal_list_head *ot_mmz_get_mmz_list(void)
{
    return &g_mmz_list;
}

typedef void (*mmz_destructor)(const void *);
ot_mmz_t *ot_mmz_create(const char *name, unsigned long gfp, unsigned long phys_start, unsigned long nbytes)
{
    ot_mmz_t *p = NULL;

    if (name == NULL) {
        mmz_err_trace("name should not be NULL!");
        return NULL;
    }

    p = osal_kmalloc(sizeof(ot_mmz_t) + 1, OSAL_GFP_KERNEL);
    if (p == NULL) {
        mmz_err_trace("System OOM!\n");
        return NULL;
    }

    (void)memset_s(p, sizeof(ot_mmz_t) + 1, 0, sizeof(ot_mmz_t) + 1);

    if (strncpy_s(p->name, OT_MMZ_NAME_LEN, name, OT_MMZ_NAME_LEN - 1) != EOK) {
        mmz_err_trace("strncpy_s failed!\n");
        osal_kfree(p);
        return NULL;
    }
    p->gfp = gfp;
    p->phys_start = phys_start;
    p->nbytes = nbytes;

    OSAL_INIT_LIST_HEAD(&p->list);
    OSAL_INIT_LIST_HEAD(&p->mmb_list);

    p->destructor = (mmz_destructor)osal_kfree;

    return p;
}
EXPORT_SYMBOL(ot_mmz_create);

int ot_mmz_destroy(const ot_mmz_t *zone)
{
    if (zone == NULL) {
        return -1;
    }

    if (zone->destructor) {
        zone->destructor(zone);
    }
    return 0;
}
EXPORT_SYMBOL(ot_mmz_destroy);

static int mmz_register_check(const ot_mmz_t *zone)
{
    ot_mmz_t *p = NULL;

    unsigned long new_start = zone->phys_start;
    unsigned long new_end = zone->phys_start + zone->nbytes;

    if (zone->nbytes == 0) {
        return -1;
    }

    if (!((new_start >= __pa((uintptr_t)high_memory)) ||
        ((new_start < PHYS_OFFSET) && (new_end <= PHYS_OFFSET)))) {
        mmz_err_trace("Conflict MMZ:\n"
            OT_MMZ_FMT_S "\n"
            "MMZ conflict to kernel memory (0x%08lX, 0x%08lX)\n", ot_mmz_fmt_arg(zone),
            (unsigned long)PHYS_OFFSET, (unsigned long)(__pa((uintptr_t)high_memory) - 1));
        return -1;
    }

    osal_list_for_each_entry(p, &g_mmz_list, list) {
        unsigned long start, end;
        start = p->phys_start;
        end   = p->phys_start + p->nbytes;
        if (new_start >= end) {
            continue;
        } else if (new_start < start && new_end <= start) {
            continue;
        } else {
        }

        mmz_err_trace("Conflict MMZ:\n"
            "MMZ new:   " OT_MMZ_FMT_S "\n"
            "MMZ exist: " OT_MMZ_FMT_S "\n"
            "Add new MMZ failed!\n", ot_mmz_fmt_arg(zone), ot_mmz_fmt_arg(p));
        return -1;
    }
    return 0;
}

int ot_mmz_register(ot_mmz_t *zone)
{
    int ret;

    if (zone == NULL) {
        return -1;
    }
    mmz_debug_info(OT_MMZ_FMT_S, ot_mmz_fmt_arg(zone));

    down(&g_mmz_lock);
    if (strcmp(g_setup_allocator, "ot") == 0) {
        ret = mmz_register_check(zone);
        if (ret) {
            up(&g_mmz_lock);
            return ret;
        }
    }
    OSAL_INIT_LIST_HEAD(&zone->mmb_list);
    osal_list_add(&zone->list, &g_mmz_list);
    up(&g_mmz_lock);
    return 0;
}

int ot_mmz_unregister(ot_mmz_t *zone)
{
    int losts = 0;
    ot_mmb_t *p = NULL;

    if (zone == NULL) {
        return -1;
    }

    down(&g_mmz_lock);
    osal_list_for_each_entry(p, &zone->mmb_list, list) {
        mmz_warn_trace("MMB Lost: " OT_MMB_FMT_S "\n", ot_mmb_fmt_arg(p));
        losts++;
    }

    if (losts) {
        mmz_err_trace("%d mmbs not free, mmz<%s> can not be unregistered!\n", losts, zone->name);
        up(&g_mmz_lock);
        return -1;
    }

    osal_list_del(&zone->list);
    up(&g_mmz_lock);
    return 0;
}

ot_mmb_t *ot_mmb_alloc(const ot_mmz_alloc_para_in *para_in)
{
    ot_mmb_t *mmb = NULL;

    if (para_in == NULL) {
        return NULL;
    }

    down(&g_mmz_lock);
    if (g_the_allocator.mmb_alloc != NULL) {
        mmb = g_the_allocator.mmb_alloc(para_in);
    }
    up(&g_mmz_lock);
    return mmb;
}
EXPORT_SYMBOL(ot_mmb_alloc);

void *ot_mmb_map2kern(ot_mmb_t *mmb)
{
    void *p = NULL;

    if (mmb == NULL) {
        return NULL;
    }

    down(&g_mmz_lock);
    if (g_the_allocator.mmb_map2kern != NULL) {
        p = g_the_allocator.mmb_map2kern(mmb, 0);
    }
    up(&g_mmz_lock);
    return p;
}
EXPORT_SYMBOL(ot_mmb_map2kern);

/* mmf: media-memory fragment */
void *ot_mmf_map2kern_nocache(unsigned long phys, int len)
{
    if (g_the_allocator.mmf_map != NULL) {
        return g_the_allocator.mmf_map(phys, len, 0);
    }
    return NULL;
}
EXPORT_SYMBOL(ot_mmf_map2kern_nocache);

void *ot_mmf_map2kern_cache(unsigned long phys, int len)
{
    if (g_the_allocator.mmf_map != NULL) {
        return g_the_allocator.mmf_map(phys, len, 1);
    }
    return NULL;
}
EXPORT_SYMBOL(ot_mmf_map2kern_cache);

void ot_mmf_unmap(void *virt)
{
    if (g_the_allocator.mmf_unmap != NULL) {
        g_the_allocator.mmf_unmap(virt);
    }
}
EXPORT_SYMBOL(ot_mmf_unmap);

void *ot_mmb_map2kern_cached(ot_mmb_t *mmb)
{
    void *p = NULL;

    if (mmb == NULL) {
        return NULL;
    }

    down(&g_mmz_lock);
    if (g_the_allocator.mmb_map2kern != NULL) {
        p = g_the_allocator.mmb_map2kern(mmb, 1);
    }
    up(&g_mmz_lock);
    return p;
}
EXPORT_SYMBOL(ot_mmb_map2kern_cached);

int ot_mmb_flush_dcache_byaddr(void *kvirt, unsigned long phys_addr, unsigned long length)
{
    if (kvirt == NULL) {
        return -EINVAL;
    }
    osal_dcache_region_wb(kvirt, phys_addr, length);
    return 0;
}
EXPORT_SYMBOL(ot_mmb_flush_dcache_byaddr);

int ot_mmb_invalid_cache_byaddr(void *kvirt, unsigned long phys_addr, unsigned long length)
{
    if (kvirt == NULL) {
        return -EINVAL;
    }

    osal_dcache_region_inv(kvirt, length);
    return 0;
}
EXPORT_SYMBOL(ot_mmb_invalid_cache_byaddr);

int ot_mmb_unmap(ot_mmb_t *mmb)
{
    int ref;

    if ((mmb == NULL) || (g_the_allocator.mmb_unmap == NULL)) {
        return -1;
    }

    down(&g_mmz_lock);
    ref = g_the_allocator.mmb_unmap(mmb);
    up(&g_mmz_lock);
    return ref;
}
EXPORT_SYMBOL(ot_mmb_unmap);

int ot_mmb_get(ot_mmb_t *mmb)
{
    int ref;

    if (mmb == NULL) {
        return -1;
    }
    down(&g_mmz_lock);
    if (mmb->flags & OT_MMB_RELEASED) {
        mmz_warn_trace("mmb<%s> is released!\n", mmb->name);
    }
    ref = ++mmb->phy_ref;
    up(&g_mmz_lock);
    return ref;
}

int ot_mmb_put(ot_mmb_t *mmb)
{
    int ref;

    if (mmb == NULL) {
        return -1;
    }

    down(&g_mmz_lock);
    if (mmb->flags & OT_MMB_RELEASED) {
        mmz_warn_trace("mmb<%s> is released!\n", mmb->name);
    }
    ref = --mmb->phy_ref;
    if ((mmb->flags & OT_MMB_RELEASED) && (mmb->phy_ref == 0) && (mmb->map_ref == 0)) {
        if (g_the_allocator.mmb_free != NULL) {
            g_the_allocator.mmb_free(mmb);
        }
    }
    up(&g_mmz_lock);
    return ref;
}

int ot_mmb_free(ot_mmb_t *mmb)
{
    if (mmb == NULL) {
        return -1;
    }

    mmz_debug_info(OT_MMB_FMT_S, ot_mmb_fmt_arg(mmb));
    down(&g_mmz_lock);

    if (mmb->flags & OT_MMB_RELEASED) {
        mmz_warn_trace("mmb<%s> has been released, but is still in use!\n", mmb->name);
        up(&g_mmz_lock);
        return 0;
    }

    if (mmb->phy_ref > 0) {
        mmz_warn_trace("free mmb<%s> delayed for which ref-count is %d!\n", mmb->name, mmb->map_ref);
        mmb->flags |= OT_MMB_RELEASED;
        up(&g_mmz_lock);
        return 0;
    }

    if (mmb->flags & OT_MMB_MAP2KERN) {
        mmz_warn_trace("free mmb<%s> delayed for which is kernel-mapped with map_ref %d!\n",
            mmb->name, mmb->map_ref);
        mmb->flags |= OT_MMB_RELEASED;
        up(&g_mmz_lock);
        return 0;
    }
    if (g_the_allocator.mmb_free != NULL) {
        g_the_allocator.mmb_free(mmb);
    }
    up(&g_mmz_lock);
    return 0;
}
EXPORT_SYMBOL(ot_mmb_free);

ot_mmb_t *ot_mmb_getby_phys(unsigned long addr)
{
    ot_mmb_t *mmb = NULL;
    ot_mmz_t *mmz = NULL;

    down(&g_mmz_lock);
    osal_list_for_each_entry(mmz, &g_mmz_list, list) {
        osal_list_for_each_entry(mmb, &mmz->mmb_list, list) {
            if (mmb->phys_addr == addr) {
                up(&g_mmz_lock);
                return mmb;
            }
        }
    }
    up(&g_mmz_lock);
    return NULL;
}
EXPORT_SYMBOL(ot_mmb_getby_phys);

unsigned long usr_virt_to_phys(unsigned long virt)
{
    pgd_t *pgd = NULL;
    pud_t *pud = NULL;
    pmd_t *pmd = NULL;
    pte_t *pte = NULL;
    p4d_t *p4d = NULL;
    unsigned int cacheable = 0;
    unsigned long page_addr, page_offset, phys_addr;

    if (virt & 0x3) {
        mmz_err_trace("invalid virt addr 0x%08lx[not 4 bytes align]\n", virt);
        return 0;
    }

    if (virt >= PAGE_OFFSET) {
        mmz_err_trace("invalid user space virt addr 0x%08lx\n", virt);
        return 0;
    }

    pgd = pgd_offset(current->mm, virt);
    if (pgd_none(*pgd)) {
        mmz_err_trace("not mapped in pgd!\n");
        return 0;
    }

    p4d = p4d_offset(pgd, virt);
    if (p4d_none(*p4d)) {
        mmz_err_trace("not mapped in p4d!\n");
        return 0;
    }
    pud = pud_offset(p4d, virt);
    if (pud_none(*pud)) {
        mmz_err_trace("not mapped in pud!\n");
        return 0;
    }

    pmd = pmd_offset(pud, virt);
    if (pmd_none(*pmd)) {
        mmz_err_trace("not mapped in pmd!\n");
        return 0;
    }

    pte = pte_offset_map(pmd, virt);
    if (pte_none(*pte)) {
        mmz_err_trace("not mapped in pte!\n");
        pte_unmap(pte);
        return 0;
    }

    page_addr = (unsigned long)((pte_val(*pte) & PHYS_MASK) & PAGE_MASK);
    page_offset = virt & ~PAGE_MASK;
    phys_addr = page_addr | page_offset;
#ifdef CONFIG_64BIT
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
    if (((pte_val(*pte) & PTE_ATTRINDX_MASK) == PTE_ATTRINDX(MT_NORMAL)) ||
       ((pte_val(*pte) & PTE_ATTRINDX_MASK) == PTE_ATTRINDX(MT_NORMAL_TAGGED))) {
#else
    if (pte_val(*pte) & (1 << 4)) { /* 4: cacheable flag of 64-bit linux is bit 4 */
#endif
#else
    if (pte_val(*pte) & (1 << 3)) { /* 3: cacheable flag of 32-bit linux is bit 3 */
#endif
        cacheable = 1;
    }

    /*
     * phys_addr: the lowest bit indicates its cache attribute
     * 1: cacheable
     * 0: uncacheable
     */
    phys_addr |= cacheable;
    pte_unmap(pte);
    return phys_addr;
}
EXPORT_SYMBOL(usr_virt_to_phys);

ot_mmb_t *ot_mmb_getby_kvirt(const void *virt)
{
    ot_mmb_t *mmb = NULL;
    ot_mmz_t *mmz = NULL;

    if (virt == NULL) {
        return NULL;
    }

    down(&g_mmz_lock);
    osal_list_for_each_entry(mmz, &g_mmz_list, list) {
        osal_list_for_each_entry(mmb, &mmz->mmb_list, list) {
            if ((mmb->kvirt <= virt) && (virt < mmb->kvirt + mmb->length)) {
                up(&g_mmz_lock);
                return mmb;
            }
        }
    }
    up(&g_mmz_lock);
    return NULL;
}
EXPORT_SYMBOL(ot_mmb_getby_kvirt);

ot_mmb_t *ot_mmb_getby_phys_2(unsigned long addr, unsigned long *out_offset)
{
    ot_mmb_t *mmb = NULL;
    ot_mmz_t *mmz = NULL;

    if (out_offset == NULL) {
        return NULL;
    }

    down(&g_mmz_lock);
    osal_list_for_each_entry(mmz, &g_mmz_list, list) {
        osal_list_for_each_entry(mmb, &mmz->mmb_list, list) {
            if ((mmb->phys_addr <= addr) && (addr < mmb->phys_addr + mmb->length)) {
                *out_offset = addr - mmb->phys_addr;
                up(&g_mmz_lock);
                return mmb;
            }
        }
    }
    up(&g_mmz_lock);
    return NULL;
}
EXPORT_SYMBOL(ot_mmb_getby_phys_2);

ot_mmb_t *media_mem_get_mmb_by_handle(const void *mem_handle)
{
    ot_mmb_t *mmb = NULL;
    ot_mmz_t *mmz = NULL;

    if (mem_handle == NULL) {
        return NULL;
    }

    down(&g_mmz_lock);
    osal_list_for_each_entry(mmz, &g_mmz_list, list) {
        osal_list_for_each_entry(mmb, &mmz->mmb_list, list) {
            if (mmb == (ot_mmb_t *)mem_handle) {
                ++mmb->phy_ref;
                up(&g_mmz_lock);
                return mmb;
            }
        }
    }
    up(&g_mmz_lock);
    return NULL;
}

ot_mmb_t *media_mem_get_mmb_by_phys(unsigned long phys_addr)
{
    ot_mmb_t *mmb = NULL;
    ot_mmz_t *mmz = NULL;

    down(&g_mmz_lock);
    osal_list_for_each_entry(mmz, &g_mmz_list, list) {
        osal_list_for_each_entry(mmb, &mmz->mmb_list, list) {
            if ((mmb->phys_addr <= phys_addr) && (phys_addr < mmb->phys_addr + mmb->length)) {
                ++mmb->phy_ref;
                up(&g_mmz_lock);
                return mmb;
            }
        }
    }
    up(&g_mmz_lock);
    return NULL;
}

/* [phys_addr, phys_addr + size] must in mmb range */
ot_mmb_t *media_mem_get_mmb_by_phys_and_size(unsigned long phys_addr, unsigned long size)
{
    ot_mmb_t *mmb = NULL;
    ot_mmz_t *mmz = NULL;
    unsigned long addr_end = phys_addr + size;

    if (size == 0 || addr_end < phys_addr) {
        return NULL;
    }

    down(&g_mmz_lock);
    osal_list_for_each_entry(mmz, &g_mmz_list, list) {
        osal_list_for_each_entry(mmb, &mmz->mmb_list, list) {
            if ((mmb->phys_addr <= phys_addr) && (addr_end <= mmb->phys_addr + mmb->length)) {
                ++mmb->phy_ref;
                up(&g_mmz_lock);
                return mmb;
            }
        }
    }
    up(&g_mmz_lock);
    return NULL;
}

void media_mem_put_mmb(ot_mmb_t *mmb)
{
    (void)ot_mmb_put(mmb);
}

void media_mem_clear_pid_in_all_mmb(int pid)
{
    ot_mmz_t *mmz = NULL;
    ot_mmb_t *mmb = NULL;
    int pos;

    down(&g_mmz_lock);
    osal_list_for_each_entry(mmz, &g_mmz_list, list) {
        osal_list_for_each_entry(mmb, &mmz->mmb_list, list) {
            pos = find_pid_in_mmb(mmb, pid);
            /* is not the process that alloc the buffer */
            if (pos > 0) {
                clear_pid_in_mmb(mmb, pos);
            }
        }
    }
    up(&g_mmz_lock);
}

ot_mmz_t *ot_mmz_find(unsigned long gfp, const char *mmz_name)
{
    ot_mmz_t *p = NULL;

    down(&g_mmz_lock);
    osal_list_for_each_entry(p, ot_mmz_get_mmz_list(), list) {
        if (mmz_default_filter(p, gfp, mmz_name) != 0) {
            continue;
        }
        up(&g_mmz_lock);
        return p;
    }
    up(&g_mmz_lock);
    return NULL;
}
EXPORT_SYMBOL(ot_mmz_find);

unsigned long ot_mmz_get_phys(const char *zone_name)
{
    ot_mmz_t *zone = NULL;

    zone = ot_mmz_find(0, zone_name);
    if (zone != NULL) {
        return zone->phys_start;
    }

    return 0;
}
EXPORT_SYMBOL(ot_mmz_get_phys);

unsigned long mmz_strtoul_ex(const char *s, char **ep, unsigned int base)
{
    char *end_p = NULL;
    unsigned long value;

    value = simple_strtoul(s, &end_p, base);

    switch (*end_p) {
        case 'm':
        case 'M':
            value <<= 10; /* 10: 1M=1024k, left shift 10bit */
            /* fall-through */
        case 'k':
        case 'K':
            value <<= 10; /* 10: 1K=1024Byte, left shift 10bit */
            if (ep != NULL) {
                (*ep) = end_p + 1;
            }
            /* fall-through */
        default:
            break;
    }

    return value;
}

static int map_mmz_register_check(const ot_mmz_t *zone)
{
    ot_mmz_t *p = NULL;

    unsigned long new_start = zone->phys_start;
    unsigned long new_end = zone->phys_start + zone->nbytes;

    if (new_end <= new_start) {
        mmz_err_trace("Invalid map_mmz(0x%lX,%lu)!", new_start, zone->nbytes);
        return -1;
    }

    osal_list_for_each_entry(p, &g_map_mmz_list, list) {
        unsigned long start, end;
        start = p->phys_start;
        end   = p->phys_start + p->nbytes;

        if (new_start >= end) {
            continue;
        } else if ((new_start < start) && (new_end <= start)) {
            continue;
        } else {
        }

        mmz_err_trace("Conflict MMZ:\n"
            "MMZ new:   " OT_MMZ_FMT_S "\n"
            "MMZ exist: " OT_MMZ_FMT_S "\n"
            "Add new MMZ failed!\n", ot_mmz_fmt_arg(zone), ot_mmz_fmt_arg(p));
        return -1;
    }

    return 0;
}

int ot_map_mmz_register(ot_mmz_t *zone)
{
    int ret;

    if (zone == NULL) {
        return -1;
    }
    mmz_debug_info(OT_MMZ_FMT_S, ot_mmz_fmt_arg(zone));

    down(&g_mmz_lock);

    if (strcmp(g_setup_allocator, "ot") == 0) {
        ret = mmz_register_check(zone);
        if (ret != 0) {
            up(&g_mmz_lock);
            return ret;
        }
    }

    ret = map_mmz_register_check(zone);
    if (ret) {
        up(&g_mmz_lock);
        return ret;
    }

    OSAL_INIT_LIST_HEAD(&zone->mmb_list);
    osal_list_add(&zone->list, &g_map_mmz_list);

    up(&g_mmz_lock);
    return 0;
}

int ot_map_mmz_unregister(ot_mmz_t *zone)
{
    int losts = 0;
    ot_mmb_t *p = NULL;

    if (zone == NULL) {
        return -1;
    }

    down(&g_mmz_lock);
    osal_list_for_each_entry(p, &zone->mmb_list, list) {
        mmz_warn_trace("MMB Lost: " OT_MMB_FMT_S "\n", ot_mmb_fmt_arg(p));
        losts++;
    }

    if (losts) {
        mmz_err_trace("%d mmbs not free, mmz<%s> can not be unregistered!\n", losts, zone->name);
        up(&g_mmz_lock);
        return -1;
    }

    osal_list_del(&zone->list);
    up(&g_mmz_lock);
    return 0;
}

static int map_mmz_init(char *s)
{
    ot_mmz_t *zone = NULL;
    char *line = NULL;

    if (s[0] == '\0') {
        return 0;
    }

    while ((line = strsep(&s, ":")) != NULL) {
        int i;
        char *argv[2]; /* 2: map mmz has two arguments */

        for (i = 0; (argv[i] = strsep(&line, ",")) != NULL;) {
            if (++i == ARRAY_SIZE(argv)) {
                break;
            }
        }

        if (i == 2) { /* 2: had parse two args */
            zone = ot_mmz_create("null", 0, 0, 0);
            if (zone == NULL) {
                continue;
            }
            zone->phys_start = mmz_strtoul_ex(argv[0], NULL, 0);
            zone->nbytes = mmz_strtoul_ex(argv[1], NULL, 0);
        } else {
            mmz_err_trace("error parameters\n");
            return -EINVAL;
        }

        if (ot_map_mmz_register(zone)) {
            mmz_warn_trace("Add MMZ failed: " OT_MMZ_FMT_S "\n", ot_mmz_fmt_arg(zone));
            ot_mmz_destroy(zone);
        }
        zone = NULL;
    }

    return 0;
}

static void map_mmz_exit(void)
{
    ot_mmz_t *ptr = NULL;
    struct osal_list_head *p = NULL;
    struct osal_list_head *n = NULL;

    osal_list_for_each_safe(p, n, &g_map_mmz_list) {
        ptr = osal_list_entry(p, ot_mmz_t, list);
        mmz_warn_trace("MMZ force removed: " OT_MMZ_FMT_S "\n", ot_mmz_fmt_arg(ptr));
        ot_map_mmz_unregister(ptr);
        ot_mmz_destroy(ptr);
    }
}

int ot_map_mmz_check_phys(unsigned long addr_start, unsigned long addr_len)
{
    ot_mmz_t *p = NULL;
    unsigned long addr_end = addr_start + addr_len;
    unsigned long temp_start, temp_end;

    if ((addr_len > 0) && (addr_end > addr_start)) {
        osal_list_for_each_entry(p, &g_map_mmz_list, list) {
            temp_start = p->phys_start;
            temp_end   = p->phys_start + p->nbytes;
            if ((addr_start >= temp_start) && (addr_end <= temp_end)) {
                return 0;
            }
        }
    }

    mmz_err_trace("invalid mmap addr: 0x%lx-0x%lx\n", addr_start, addr_end);
    return -1;
}
EXPORT_SYMBOL(ot_map_mmz_check_phys);

int ot_mmb_check_mem_share(const ot_mmb_t *mmb)
{
    return ot_mmb_check_mem_share_with_pid(mmb, osal_get_current_tgid());
}
EXPORT_SYMBOL(ot_mmb_check_mem_share);

int ot_mmb_check_mem_share_with_pid(const ot_mmb_t *mmb, int pid)
{
    if (mmb == NULL) {
        return -EINVAL;
    }
    if (mmb->kernel_only == 1) {
        mmz_err_trace("mmb used only in kernel!\n");
        return -EPERM;
    }
    if (mmb->check_pid == 0) {
        /* default share */
        return 0;
    }
    if (find_pid_in_mmb(mmb, pid) < 0) {
        mmz_err_trace("mmb is not shared with pid(%d)!\n", pid);
        return -EPERM;
    }
    return 0;
}
EXPORT_SYMBOL(ot_mmb_check_mem_share_with_pid);

int ot_mmz_check_phys_addr(unsigned long phys_addr, unsigned long size)
{
    ot_mmb_t *mmb = NULL;
    int ret;

    /* if allocated by mmz of current system */
    mmb = media_mem_get_mmb_by_phys_and_size(phys_addr, size);
    if (mmb == NULL) {
        /* if in other system */
        if (ot_map_mmz_check_phys(phys_addr, size) < 0) {
            return -EINVAL;
        }
        return 0;
    }
    /* mem share check */
    ret = ot_mmb_check_mem_share(mmb);
    media_mem_put_mmb(mmb);
    return ret;
}
EXPORT_SYMBOL(ot_mmz_check_phys_addr);

int ot_mmz_get_mem_process_isolation(void)
{
    return mem_process_isolation;
}
EXPORT_SYMBOL(ot_mmz_get_mem_process_isolation);

int ot_vma_check(unsigned long vm_start, unsigned long vm_end)
{
    struct vm_area_struct *pvma1 = NULL;
    struct vm_area_struct *pvma2 = NULL;

    pvma1 = find_vma(current->mm, vm_start);
    if (pvma1 == NULL) {
        mmz_err_trace("pvma1 is null\n");
        return -1;
    }

    pvma2 = find_vma(current->mm, vm_end - 1);
    if (pvma2 == NULL) {
        mmz_err_trace("pvma2 is null\n");
        return -1;
    }

    if (pvma1 != pvma2) {
        mmz_err_trace("pvma1:[0x%lx,0x%lx) and pvma2:[0x%lx,0x%lx) are not equal\n",
            pvma1->vm_start, pvma1->vm_end, pvma2->vm_start, pvma2->vm_end);
        return -1;
    }

    if (!(pvma1->vm_flags & VM_WRITE)) {
        mmz_err_trace("invalid vma flag:0x%lx\n", pvma1->vm_flags);
        return -1;
    }

    if (pvma1->vm_start > vm_start) {
        mmz_err_trace("cannot find corresponding vma, vm[%lx, %lx], user range[%lx,%lx]\n",
            pvma1->vm_start, pvma1->vm_end, vm_start, vm_end);
        return -1;
    }
    return 0;
}
EXPORT_SYMBOL(ot_vma_check);

int ot_is_phys_in_mmz(unsigned long addr_start, unsigned long addr_len)
{
    ot_mmz_t *p = NULL;
    unsigned long addr_end = addr_start + addr_len;
    unsigned long temp_start, temp_end;

    if ((addr_len > 0) && (addr_end > addr_start)) {
        osal_list_for_each_entry(p, &g_mmz_list, list) {
            temp_start = p->phys_start;
            temp_end   = p->phys_start + p->nbytes;
            if ((addr_start >= temp_start) && (addr_end <= temp_end)) {
                return 0;
            }
        }
    }
    return -1;
}
EXPORT_SYMBOL(ot_is_phys_in_mmz);

int ot_mmb_flush_dcache_byaddr_safe(void *kvirt, unsigned long phys_addr, unsigned long length)
{
    int ret;
    struct mm_struct *mm = current->mm;

    if (kvirt == NULL) {
        return -EINVAL;
    }

    mmz_map_down(mm);

    if (ot_vma_check((unsigned long)(uintptr_t)kvirt, (unsigned long)(uintptr_t)kvirt + length)) {
        mmz_map_up(mm);
        return -EPERM;
    }

    ret = ot_mmb_flush_dcache_byaddr(kvirt, phys_addr, length);

    mmz_map_up(mm);
    return ret;
}
EXPORT_SYMBOL(ot_mmb_flush_dcache_byaddr_safe);

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
int mmz_seq_show(struct osal_proc_dir_entry *sfile)
{
    ot_mmz_t *p = NULL;
    unsigned int zone_number = 0;
    unsigned int block_number = 0;
    unsigned int used_size = 0;
    unsigned int free_size;
    unsigned int mmz_total_size = 0;

    osal_seq_printf(sfile->seqfile, " anony=%d mmz_allocator=%s mem_process_isolation=%d\n\n",
        anony, g_setup_allocator, mem_process_isolation);

    down(&g_mmz_lock);
    if (!osal_list_empty(&g_map_mmz_list)) {
        osal_seq_printf(sfile->seqfile, " map_mmz_zone:");
        osal_list_for_each_entry(p, &g_map_mmz_list, list) {
            osal_seq_printf(sfile->seqfile, "(0x%lX, 0x%lX) ", p->phys_start, p->phys_start + p->nbytes - 1);
        }
        osal_seq_printf(sfile->seqfile, "\n\n");
    }

    osal_list_for_each_entry(p, &g_mmz_list, list) {
        ot_mmb_t *mmb = NULL;
        ot_mmb_t *temp_mmb = NULL;
        osal_seq_printf(sfile->seqfile, "+---ZONE: " OT_MMZ_FMT_S "\n", ot_mmz_fmt_arg(p));
        mmz_total_size += p->nbytes / 1024; /* 1024: 1KByte = 1024Byte */
        ++zone_number;

        osal_list_for_each_entry(mmb, &p->mmb_list, list) {
            if (temp_mmb != NULL && ((mmb)->phys_addr > mmz_grain_align((temp_mmb)->phys_addr + (temp_mmb)->length))) {
                osal_seq_printf(sfile->seqfile, "   *-MMB: " OT_MMB_FMT_S "\n", ot_mmb_fmt_arg(mmb));
            } else {
                osal_seq_printf(sfile->seqfile, "   |-MMB: " OT_MMB_FMT_S "\n", ot_mmb_fmt_arg(mmb));
            }
            temp_mmb = mmb;
            used_size += mmb->length / 1024; /* 1024: 1KByte = 1024Byte */
            ++block_number;
        }
    }

    if (mmz_total_size != 0) {
        free_size = mmz_total_size - used_size;
        osal_seq_printf(sfile->seqfile, "\n---MMZ_USE_INFO:\n total size=%dKB(%dMB),"
            "used=%dKB(%dMB + %dKB),remain=%dKB(%dMB + %dKB),"
            "zone_number=%d,block_number=%d\n",
            mmz_total_size, mmz_total_size / 1024, /* 1024: 1MByte = 1024KByte */
            used_size, used_size / 1024, used_size % 1024, /* 1024: 1MByte = 1024KByte */
            free_size, free_size / 1024, free_size % 1024, /* 1024: 1MByte = 1024KByte */
            zone_number, block_number);
        mmz_total_size = 0;
        zone_number = 0;
        block_number = 0;
    }
    up(&g_mmz_lock);
    return 0;
}

static int __init media_mem_proc_init(void)
{
    osal_proc_entry *proc = NULL;

    proc = osal_create_proc_entry(MEDIA_MEM_NAME, NULL);
    if (proc == NULL) {
        mmz_err_trace("Create mmz proc fail!\n");
        return -1;
    }
    proc->read = mmz_seq_show;
    return 0;
}

static void __exit media_mem_proc_exit(void)
{
    osal_remove_proc_entry(MEDIA_MEM_NAME, NULL);
}
#endif /* CONFIG_OT_PROC_SHOW_SUPPORT */

int mem_check_module_param(void)
{
    if (anony != 1) {
        mmz_err_trace("The module param \"anony\" should only be 1 which is %d\n", anony);
        return -1;
    }
    return 0;
}

static void mmz_exit_check(void)
{
    ot_mmz_t *ptr = NULL;
    struct osal_list_head *p = NULL;
    struct osal_list_head *n = NULL;

    osal_list_for_each_safe(p, n, &g_mmz_list) {
        ptr = osal_list_entry(p, ot_mmz_t, list);
        mmz_warn_trace("MMZ force removed: " OT_MMZ_FMT_S "\n", ot_mmz_fmt_arg(ptr));
        ot_mmz_unregister(ptr);
        ot_mmz_destroy(ptr);
    }
}

int __init media_mem_init(void)
{
    int ret = 0;

    mmz_printk(KERN_INFO "Media Memory Zone Manager\n");

    if (anony != 1) {
        mmz_err_trace("The module param \"anony\" should only be 1 which is %d\n", anony);
        return -EINVAL;
    }

    if (strcmp(g_setup_allocator, "cma") == 0) {
#ifdef CONFIG_CMA
        cma_allocator_setopt(&g_the_allocator);
#else
        pr_err("cma is not enabled in kernel, please check!\n");
        return -EINVAL;
#endif
    } else if (strcmp(g_setup_allocator, "ot") == 0) {
        ot_allocator_setopt(&g_the_allocator);
    } else {
        mmz_err_trace("The module param \"g_setup_allocator\" should be \"cma\" or \"ot\", which is \"%s\"\n",
            g_setup_allocator);
        mmz_exit_check();
        return -EINVAL;
    }

    ret = g_the_allocator.init(g_setup_zones);
    if (ret != 0) {
        mmz_exit_check();
        return ret;
    }

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    media_mem_proc_init();
#endif
    mmz_userdev_init();
    map_mmz_init(g_mmap_zones);
    return 0;
}

void __exit media_mem_exit(void)
{
    map_mmz_exit();
    mmz_userdev_exit();
#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    media_mem_proc_exit();
#endif
    mmz_exit_check();
}
