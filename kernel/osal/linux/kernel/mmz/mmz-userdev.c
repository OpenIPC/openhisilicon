/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/spinlock.h>
#include <linux/vmalloc.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/time.h>
#include <linux/sched.h>
#include <linux/dma-mapping.h>

#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/cacheflush.h>

#include "osal_mmz.h"
#include "osal.h"

#define error(s...)                                        \
    do {                                                   \
        printk(KERN_ERR "mmz_userdev:%s: ", __FUNCTION__); \
        printk(s);                                         \
    } while (0)
#define warning(s...)                                          \
    do {                                                       \
        printk(KERN_WARNING "mmz_userdev:%s: ", __FUNCTION__); \
        printk(s);                                             \
    } while (0)

struct mmz_userdev_info {
    pid_t pid;
    pid_t mmap_pid;
    struct semaphore sem;
    struct osal_list_head list;
};

static int mmz_flush_dcache_mmb_dirty(struct dirty_area *p_area)
{
    if (p_area == NULL) {
        return -EINVAL;
    }

#ifdef CONFIG_64BIT
    __flush_dcache_area(p_area->dirty_virt_start, p_area->dirty_size);
#else
    /* flush l1 cache, use vir addr */
    __cpuc_flush_dcache_area(p_area->dirty_virt_start, p_area->dirty_size);
    /* flush with clean */
    // dmac_map_area(p_area->dirty_virt_start, p_area->dirty_size, DMA_TO_DEVICE);

#if defined(CONFIG_CACHE_L2V200) || defined(CONFIG_CACHE_L2X0)
    /* flush l2 cache, use paddr */
    outer_flush_range(p_area->dirty_phys_start,
                      p_area->dirty_phys_start + p_area->dirty_size);
#endif
#endif
    return 0;
}

static int mmz_flush_dcache_mmb(struct mmb_info *pmi)
{
    mmz_mmb_t *mmb = NULL;

    if (pmi == NULL) {
        return -EINVAL;
    }

    mmb = pmi->mmb;
    if ((mmb == NULL) || (pmi->map_cached == 0)) {
        printk("%s->%d,error!\n", __func__, __LINE__);
        return -EINVAL;
    }

#ifdef CONFIG_64BIT
    __flush_dcache_area(pmi->mapped, (size_t)pmi->size);
#else
    /* flush l1 cache, use vir addr */
    __cpuc_flush_dcache_area(pmi->mapped, (size_t)pmi->size);

#if defined(CONFIG_CACHE_L2V200) || defined(CONFIG_CACHE_L2X0)
    /* flush l2 cache, use paddr */
    outer_flush_range(mmb->phys_addr, mmb->phys_addr + mmb->length);
#endif
#endif
    return 0;
}

/*
 * this function should never be called with local irq disabled,
 * because on_each_cpu marco will raise ipi interrupt.
 */
int mmz_flush_dcache_all(void)
{
#ifdef CONFIG_64BIT
    on_each_cpu((smp_call_func_t)flush_cache_all, NULL, 1);
#else
    on_each_cpu((smp_call_func_t)__cpuc_flush_kern_all, NULL, 1);
    outer_flush_all();
#endif /* CONFIG_64BIT */
    return 0;
}

static int mmz_userdev_open(struct inode *inode, struct file *file)
{
    struct mmz_userdev_info *pmu = NULL;

    if (!capable(CAP_SYS_RAWIO)) {
        return -EPERM;
    }

    pmu = kmalloc(sizeof(*pmu), GFP_KERNEL);
    if (pmu == NULL) {
        error("alloc mmz_userdev_info failed!\n");
        return -ENOMEM;
    }
    memset(pmu, 0, sizeof(*pmu));
    pmu->pid = current->pid;
    pmu->mmap_pid = 0;
    sema_init(&pmu->sem, 1);
    OSAL_INIT_LIST_HEAD(&pmu->list);

    /* This file could be opened just for once */
    file->private_data = (void *)pmu;

    return 0;
}

static int ioctl_mmb_alloc(struct file *file,
                           unsigned int iocmd,
                           struct mmb_info *pmi)
{
    struct mmz_userdev_info *pmu = file->private_data;
    struct mmb_info *new_mmbinfo = NULL;
    mmz_mmb_t *mmb = NULL;

    mmb = mmz_mmb_alloc(pmi->mmb_name, pmi->size,
                        pmi->align, pmi->gfp, pmi->mmz_name);
    if (mmb == NULL) {
#if defined(KERNEL_BIT_64) && defined(USER_BIT_32)
        error("mmz_mmb_alloc(%s, %llu, 0x%llx, %lu, %s) failed!\n",
              pmi->mmb_name, pmi->size, pmi->align,
              pmi->gfp, pmi->mmz_name);
#else
        error("mmz_mmb_alloc(%s, %lu, 0x%lx, %lu, %s) failed!\n",
              pmi->mmb_name, pmi->size, pmi->align,
              pmi->gfp, pmi->mmz_name);
#endif
        return -ENOMEM;
    }

    new_mmbinfo = kmalloc(sizeof(*new_mmbinfo), GFP_KERNEL);
    if (new_mmbinfo == NULL) {
        mmz_mmb_free(mmb);
        error("alloc mmb_info failed!\n");
        return -ENOMEM;
    }

    memcpy(new_mmbinfo, pmi, sizeof(*new_mmbinfo));
    new_mmbinfo->phys_addr = mmz_mmb_phys(mmb);
    new_mmbinfo->mmb = mmb;
    new_mmbinfo->prot = PROT_READ;
    new_mmbinfo->flags = MAP_SHARED;
    osal_list_add_tail(&new_mmbinfo->list, &pmu->list);

    pmi->phys_addr = new_mmbinfo->phys_addr;

    mmz_mmb_get(mmb);

    return 0;
}

static int ioctl_mmb_alloc_v2(struct file *file,
                              unsigned int iocmd, struct mmb_info *pmi)
{
    struct mmz_userdev_info *pmu = file->private_data;
    struct mmb_info *new_mmbinfo = NULL;
    mmz_mmb_t *mmb = NULL;

    mmb = mmz_mmb_alloc_v2(pmi->mmb_name, pmi->size, pmi->align,
                           pmi->gfp, pmi->mmz_name, pmi->order);

    if (mmb == NULL) {
#if defined(KERNEL_BIT_64) && defined(USER_BIT_32)
        error("mmz_mmb_alloc(%s, %llu, 0x%llx, %lu, %s) failed!\n",
              pmi->mmb_name, pmi->size, pmi->align,
              pmi->gfp, pmi->mmz_name);
#else
        error("mmz_mmb_alloc(%s, %lu, 0x%lx, %lu, %s) failed!\n",
              pmi->mmb_name, pmi->size, pmi->align,
              pmi->gfp, pmi->mmz_name);
#endif
        return -ENOMEM;
    }

    new_mmbinfo = kmalloc(sizeof(*new_mmbinfo), GFP_KERNEL);

    if (new_mmbinfo == NULL) {
        mmz_mmb_free(mmb);
        error("alloc mmb_info failed!\n");
        return -ENOMEM;
    }

    memcpy(new_mmbinfo, pmi, sizeof(*new_mmbinfo));
    new_mmbinfo->phys_addr = mmz_mmb_phys(mmb);
    new_mmbinfo->mmb = mmb;
    new_mmbinfo->prot = PROT_READ;
    new_mmbinfo->flags = MAP_SHARED;
    osal_list_add_tail(&new_mmbinfo->list, &pmu->list);

    pmi->phys_addr = new_mmbinfo->phys_addr;

    mmz_mmb_get(mmb);

    return 0;
}

static struct mmb_info *get_mmbinfo(unsigned long addr,
                                    struct mmz_userdev_info *pmu)
{
    struct mmb_info *p = NULL;

    osal_list_for_each_entry(p, &pmu->list, list) {
        if ((addr >= p->phys_addr) && (addr < (p->phys_addr + p->size))) {
            break;
        }
    }
    if (&p->list == &pmu->list) {
        return NULL;
    }

    return p;
}

static struct mmb_info *get_mmbinfo_safe(unsigned long addr,
                                         struct mmz_userdev_info *pmu)
{
    struct mmb_info *p = NULL;

    p = get_mmbinfo(addr, pmu);

    if (p == NULL) {
        error("mmb(0x%08lX) not found!\n", addr);
        return NULL;
    }

    return p;
}

static int ioctl_mmb_user_unmap(struct file *file, unsigned int iocmd, struct mmb_info *pmi);

static int _usrdev_mmb_free(struct mmb_info *p)
{
    int ret = 0;

    osal_list_del(&p->list);
    mmz_mmb_put(p->mmb);
    ret = mmz_mmb_free(p->mmb);
    kfree(p);

    return ret;
}

static int ioctl_mmb_free(struct file *file,
                          unsigned int iocmd, struct mmb_info *pmi)
{
    int ret = 0;
    struct mmz_userdev_info *pmu = file->private_data;
    struct mmb_info *p = get_mmbinfo_safe(pmi->phys_addr, pmu);

    if (p == NULL) {
        return -EPERM;
    }

    if (p->delayed_free) {
        warning("mmb<%s> is delayed_free, can not free again!\n", p->mmb->name);
        return -EBUSY;
    }

    if ((p->map_ref > 0) || (p->mmb_ref > 0)) {
        warning("mmb<%s> is still in use!\n", p->mmb->name);
        p->delayed_free = 1;
        return -EBUSY;
    }

    ret = _usrdev_mmb_free(p);

    return ret;
}

static int ioctl_mmb_attr(struct file *file,
                          unsigned int iocmd, struct mmb_info *pmi)
{
    struct mmz_userdev_info *pmu = file->private_data;
    struct mmb_info *p = NULL;

    if ((p = get_mmbinfo_safe(pmi->phys_addr, pmu)) == NULL) {
        return -EPERM;
    }

    memcpy(pmi, p, sizeof(*pmi));
    return 0;
}

static int ioctl_mmb_user_remap(struct file *file,
                                unsigned int iocmd,
                                struct mmb_info *pmi,
                                int cached)
{
    struct mmz_userdev_info *pmu = file->private_data;
    struct mmb_info *p = NULL;
    unsigned long addr, len, prot, flags, pgoff;

    if ((p = get_mmbinfo_safe(pmi->phys_addr, pmu)) == NULL) {
        return -EPERM;
    }
    /*
     * mmb could be remapped for more than once, but should not
     * be remapped with confusing cache type.
     */
    if (p->mapped && (p->map_ref > 0)) {
        if (cached != p->map_cached) {
            error("mmb<%s> already mapped as %s, cannot remap as %s.\n",
                  p->mmb->name,
                  p->map_cached ? "cached" : "non-cached",
                  cached ? "cached" : "non-cached");
            return -EINVAL;
        }

        p->map_ref++;
        p->mmb_ref++;

        mmz_mmb_get(p->mmb);

        /*
         * pmi->phys may not always start at p->phys,
         * and may start with offset from p->phys.
         * so, we need to calculate with the offset.
         */
        pmi->mapped = p->mapped + (pmi->phys_addr - p->phys_addr);

        return 0;
    }

    if (p->phys_addr & ~PAGE_MASK) {
        return -EINVAL;
    }

    addr = 0;
    len = PAGE_ALIGN(p->size);

    prot = pmi->prot;
    flags = pmi->flags;
    if (prot == 0) {
        prot = p->prot;
    }
    if (flags == 0) {
        flags = p->flags;
    }

    pmu->mmap_pid = current->pid;
    p->map_cached = cached;

    pgoff = p->phys_addr;
    addr = vm_mmap(file, addr, len, prot, flags, pgoff);

    pmu->mmap_pid = 0;

    if (IS_ERR_VALUE((uintptr_t)addr)) {
        error("vm_mmap(file, 0, %lu, 0x%08lX, 0x%08lX, 0x%08lX) return 0x%08lX\n",
              len, prot, flags, pgoff, addr);
        return addr;
    }

    p->mapped = (void *)(uintptr_t)addr;
    p->prot = prot;
    p->flags = flags;

    p->map_ref++;
    p->mmb_ref++;
    mmz_mmb_get(p->mmb);

    /*
     * pmi->phys may not always start at p->phys,
     * and may start with offset from p->phys.
     * so, we need to calculate with the offset.
     */
    pmi->mapped = p->mapped + (pmi->phys_addr - p->phys_addr);

    return 0;
}

static int ioctl_mmb_user_unmap(struct file *file,
                                unsigned int iocmd, struct mmb_info *pmi)
{
    int ret;
    unsigned long addr, len;
    struct mmb_info *p = NULL;
    struct mmz_userdev_info *pmu = file->private_data;

    if ((p = get_mmbinfo_safe(pmi->phys_addr, pmu)) == NULL) {
        return -EPERM;
    }

    if (p->mapped == NULL) {
#if defined(KERNEL_BIT_64) && defined(USER_BIT_32)
        warning("mmb(0x%llx) isn't user-mapped!\n", p->phys_addr);
#else
        warning("mmb(0x%lx) isn't user-mapped!\n", p->phys_addr);
#endif
        pmi->mapped = NULL;
        return -EIO;
    }

    if (!((p->map_ref > 0) && (p->mmb_ref > 0))) {
        error("mmb<%s> has invalid refer: map_ref=%d, mmb_ref=%d.\n",
              p->mmb->name, p->map_ref, p->mmb_ref);
        return -EIO;
    }

    p->map_ref--;
    p->mmb_ref--;
    mmz_mmb_put(p->mmb);

    if (p->map_ref > 0) {
        return 0;
    }

    addr = (unsigned long)(uintptr_t)p->mapped;
    len = PAGE_ALIGN(p->size);

    /* todo,before unmap,refresh cache manually */
    if (p->map_cached) {
        struct mm_struct *mm = current->mm;
        down_read(&mm->mmap_sem);
        if (mmz_vma_check(addr, addr + len)) {
            error("mmb<%s> vma is invalid.\n", p->mmb->name);
            up_read(&mm->mmap_sem);
            return -EPERM;
        }
#ifdef CONFIG_64BIT
        __flush_dcache_area ((void *)(uintptr_t)addr, (size_t)len);
#else
        __cpuc_flush_dcache_area ((void *)(uintptr_t)addr, (size_t)len);
#if defined(CONFIG_CACHE_L2V200) || defined(CONFIG_CACHE_L2X0)
        /* flush without clean */
        // dmac_map_area(mmb->kvirt, mmb->length, DMA_TO_DEVICE);
        outer_flush_range(p->phys_addr, p->phys_addr + len);
#endif
#endif /* CONFIG_64BIT */
        up_read(&mm->mmap_sem);
    }

    ret = vm_munmap(addr, len);

    if (!IS_ERR_VALUE((uintptr_t)ret)) {
        p->mapped = NULL;
        pmi->mapped = NULL;
    }

    if (p->delayed_free && (p->map_ref == 0) && (p->mmb_ref == 0)) {
        _usrdev_mmb_free(p);
    }

    return ret;
}

static int ioctl_mmb_virt2phys(struct file *file,
                               unsigned int iocmd, struct mmb_info *pmi)
{
    int ret = 0;
    unsigned long virt = 0, phys = 0;
    unsigned long offset = 0;

    virt = (unsigned long)(uintptr_t)pmi->mapped;
    phys = usr_virt_to_phys(virt);
    if (!phys) {
        ret = -ENOMEM;
    }

    if (mmz_mmb_getby_phys_2(phys, &offset) == NULL) {
        error("Not mmz alloc memory[0x%lx 0x%lx]! 0x%lx\n", virt, phys, offset);
        return -EINVAL;
    }

    pmi->phys_addr = phys;

    return ret;
}

static int mmz_userdev_ioctl_m(struct file *file, unsigned int cmd, struct mmb_info *pmi)
{
    int ret = 0;

    switch (_IOC_NR(cmd)) {
        case _IOC_NR(IOC_MMB_ALLOC):
            ret = ioctl_mmb_alloc(file, cmd, pmi);
            break;
        case _IOC_NR(IOC_MMB_ALLOC_V2):
            ret = ioctl_mmb_alloc_v2(file, cmd, pmi);
            break;
        case _IOC_NR(IOC_MMB_ATTR):
            ret = ioctl_mmb_attr(file, cmd, pmi);
            break;
        case _IOC_NR(IOC_MMB_FREE):
            ret = ioctl_mmb_free(file, cmd, pmi);
            break;

        case _IOC_NR(IOC_MMB_USER_REMAP):
            ret = ioctl_mmb_user_remap(file, cmd, pmi, 0);
            break;
        case _IOC_NR(IOC_MMB_USER_REMAP_CACHED):
            ret = ioctl_mmb_user_remap(file, cmd, pmi, 1);
            break;
        case _IOC_NR(IOC_MMB_USER_UNMAP):
            ret = ioctl_mmb_user_unmap(file, cmd, pmi);
            break;
        case _IOC_NR(IOC_MMB_VIRT_GET_PHYS):
            ret = ioctl_mmb_virt2phys(file, cmd, pmi);
            break;
        default:
            error("invalid ioctl cmd = %08X\n", cmd);
            ret = -EINVAL;
            break;
    }

    return ret;
}

static int mmz_userdev_ioctl_r(struct file *file, unsigned int cmd, struct mmb_info *pmi)
{
    switch (_IOC_NR(cmd)) {
        case _IOC_NR(IOC_MMB_ADD_REF):
            pmi->mmb_ref++;
            mmz_mmb_get(pmi->mmb);
            break;
        case _IOC_NR(IOC_MMB_DEC_REF):
            if (pmi->mmb_ref <= 0) {
                error("mmb<%s> mmb_ref is %d!\n", pmi->mmb->name, pmi->mmb_ref);
                return -EPERM;
            }
            pmi->mmb_ref--;
            mmz_mmb_put(pmi->mmb);
            if (pmi->delayed_free && (pmi->mmb_ref == 0) && (pmi->map_ref == 0)) {
                _usrdev_mmb_free(pmi);
            }
            break;
        default:
            return -EINVAL;
            break;
    }

    return 0;
}

/* just for test */
static int mmz_userdev_ioctl_t(struct file *file, unsigned int cmd, struct mmb_info *pmi);

static long mmz_userdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int ret = 0;
    struct mmz_userdev_info *pmu = file->private_data;

    down(&pmu->sem);

    if (_IOC_TYPE(cmd) == 'm') {
        struct mmb_info mi = { 0 };

        if ((_IOC_SIZE(cmd) > sizeof(mi)) || (arg == 0)) {
            error("_IOC_SIZE(cmd)=%d, arg==0x%08lX\n", _IOC_SIZE(cmd), arg);
            ret = -EINVAL;
            goto __error_exit;
        }
        memset(&mi, 0, sizeof(mi));
        if (copy_from_user(&mi, (void *)(uintptr_t)arg, _IOC_SIZE(cmd))) {
            printk("\nmmz_userdev_ioctl: copy_from_user error.\n");
            ret = -EFAULT;
            goto __error_exit;
        }
        mi.mmz_name[MMZ_MMZ_NAME_LEN - 1] = '\0';
        mi.mmb_name[MMZ_MMB_NAME_LEN - 1] = '\0';
        ret = mmz_userdev_ioctl_m(file, cmd, &mi);

        if (!ret && (cmd & IOC_OUT)) {
            if (copy_to_user((void *)(uintptr_t)arg, &mi, _IOC_SIZE(cmd))) {
                printk("\nmmz_userdev_ioctl: copy_to_user error.\n");
                ret = -EFAULT;
                goto __error_exit;
            }
        }

    } else if (_IOC_TYPE(cmd) == 'r') {
        struct mmb_info *pmi = NULL;
        if ((pmi = get_mmbinfo_safe(arg, pmu)) == NULL) {
            ret = -EPERM;
            goto __error_exit;
        }

        ret = mmz_userdev_ioctl_r(file, cmd, pmi);

    } else if (_IOC_TYPE(cmd) == 'c') {
        struct mmb_info *pmi = NULL;
        if (arg == 0) {
            mmz_flush_dcache_all();
            goto __error_exit;
        }
        if ((pmi = get_mmbinfo_safe(arg, pmu)) == NULL) {
            ret = -EPERM;
            goto __error_exit;
        }

        switch (_IOC_NR(cmd)) {
            case _IOC_NR(IOC_MMB_FLUSH_DCACHE):
                mmz_flush_dcache_mmb(pmi);
                break;
            default:
                ret = -EINVAL;
                break;
        }

    } else if (_IOC_TYPE(cmd) == 'd') {
        mmz_mmb_t *mmb = NULL;
        struct mmb_info *pmi = NULL;
        struct dirty_area area;
        __phys_addr_type__ orig_addr;
        unsigned long virt_addr, offset;
        struct mm_struct *mm = current->mm;

        if ((_IOC_SIZE(cmd) != sizeof(area)) || (arg == 0)) {
            error("_IOC_SIZE(cmd)=%d, arg==0x%08lx\n", _IOC_SIZE(cmd), arg);
            ret = -EINVAL;
            goto __error_exit;
        }
        memset(&area, 0, sizeof(area));
        if (copy_from_user(&area, (void *)(uintptr_t)arg, _IOC_SIZE(cmd))) {
            printk(KERN_WARNING "\nmmz_userdev_ioctl: copy_from_user error.\n");
            ret = -EFAULT;
            goto __error_exit;
        }

        if ((mmb = mmz_mmb_getby_phys_2(area.dirty_phys_start, &offset)) == NULL) {
#if defined(KERNEL_BIT_64) && defined(USER_BIT_32)
            error("dirty_phys_addr=0x%llx\n", area.dirty_phys_start);
#else
            error("dirty_phys_addr=0x%lx\n", area.dirty_phys_start);
#endif
            ret = -EFAULT;
            goto __error_exit;
        }

        pmi = get_mmbinfo_safe(mmb->phys_addr, pmu);
        if (pmi == NULL) {
            ret = -EPERM;
            goto __error_exit;
        }
        if ((uintptr_t)area.dirty_virt_start != (uintptr_t)pmi->mapped + offset) {
            printk(KERN_WARNING
                   "dirty_virt_start addr was not consistent with dirty_phys_start addr!\n");
            ret = -EFAULT;
            goto __error_exit;
        }
        if (area.dirty_phys_start + area.dirty_size > mmb->phys_addr + mmb->length) {
            printk(KERN_WARNING "\ndirty area overflow!\n");
            ret = -EFAULT;
            goto __error_exit;
        }

        down_read(&mm->mmap_sem);

        if (mmz_vma_check((uintptr_t)area.dirty_virt_start, (uintptr_t)area.dirty_virt_start + area.dirty_size)) {
            printk(KERN_WARNING "\ndirty area[0x%lx,0x%lx] overflow!\n",
                  (unsigned long)(uintptr_t)area.dirty_virt_start,
                  (unsigned long)(uintptr_t)area.dirty_virt_start + area.dirty_size);
            ret = -EFAULT;
            up_read(&mm->mmap_sem);
            goto __error_exit;
        }

        /* cache line aligned */
        orig_addr = area.dirty_phys_start;
        area.dirty_phys_start &= ~(CACHE_LINE_SIZE - 1);
        virt_addr = (unsigned long)(uintptr_t)area.dirty_virt_start;
        virt_addr &= ~(CACHE_LINE_SIZE - 1);
        area.dirty_virt_start = (void *)(uintptr_t)virt_addr;
        // area.dirty_virt_start &= ~(CACHE_LINE_SIZE - 1);
        area.dirty_size = (area.dirty_size + (orig_addr - area.dirty_phys_start) +
                           (CACHE_LINE_SIZE - 1)) & ~(CACHE_LINE_SIZE - 1);

        mmz_flush_dcache_mmb_dirty(&area);
        up_read(&mm->mmap_sem);
    } else if (_IOC_TYPE(cmd) == 't') {
        struct mmb_info mi;

        if ((_IOC_SIZE(cmd) != sizeof(mi)) || (arg == 0)) {
            error("_IOC_SIZE(cmd)=%d, arg==0x%08lx\n", _IOC_SIZE(cmd), arg);
            ret = -EINVAL;
            goto __error_exit;
        }

        memset(&mi, 0, sizeof(mi));
        if (copy_from_user(&mi, (void *)(uintptr_t)arg, sizeof(mi))) {
            printk("\nmmz_userdev_ioctl: copy_from_user error.\n");
            ret = -EFAULT;
            goto __error_exit;
        }

        if ((get_mmbinfo_safe(mi.phys_addr, pmu)) == NULL) {
            ret = -EPERM;
            goto __error_exit;
        }
        ret = mmz_userdev_ioctl_t(file, cmd, &mi);
    } else {
        ret = -EINVAL;
    }

__error_exit:

    up(&pmu->sem);

    return ret;
}

#ifdef CONFIG_COMPAT
static long compat_mmz_userdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    return mmz_userdev_ioctl(file, cmd, (unsigned long)compat_ptr(arg));
}
#endif

int mmz_userdev_mmap(struct file *file, struct vm_area_struct *vma)
{
    struct mmb_info *p = NULL;
    struct mmz_userdev_info *pmu = file->private_data;
    unsigned long offset = vma->vm_pgoff << PAGE_SHIFT;
    mmz_mmb_t *mmb = NULL;
    int mmb_cached = 0;

    p = get_mmbinfo(offset, pmu);

    if (p == NULL) {
        unsigned long mmb_offset;
        mmb = mmz_mmb_getby_phys_2(offset, &mmb_offset);
        if (mmb == NULL) {
            /* Allow mmap MMZ allocated by other core. */
            if (mmz_map_mmz_check_phys(offset, vma->vm_end - vma->vm_start)) {
                return -EPERM;
            }
        } else {
            mmb_cached = mmb->flags & MMZ_MMB_MAP2KERN_CACHED;
        }
    } else {
        if (p->mapped != NULL) {
            error("mmb(0x%08lX) have been mapped already?!\n", offset);
            return -EIO;
        }
        mmb_cached = p->map_cached;
    }

    if (file->f_flags & O_SYNC) {
#ifdef CONFIG_64BIT
        vma->vm_page_prot = __pgprot(pgprot_val(vma->vm_page_prot)
                                     | PTE_WRITE | PTE_DIRTY);
#endif
#if 0
        vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
#else
        vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
#endif
    } else {
#ifdef CONFIG_64BIT
        vma->vm_page_prot = __pgprot(pgprot_val(vma->vm_page_prot)
                                     | PTE_WRITE | PTE_DIRTY);
#else
        vma->vm_page_prot = __pgprot(pgprot_val(vma->vm_page_prot)
                                     | L_PTE_PRESENT | L_PTE_YOUNG
                                     | L_PTE_DIRTY | L_PTE_MT_DEV_CACHED);
#endif
        if (mmb_cached) {
            /*
             * notice: tag:ARM: pgtable:
             * provide RDONLY page table bit
             * rather than WRITE bit
             */
#if 0
#ifdef CONFIG_64BIT
            vma->vm_page_prot = __pgprot(pgprot_val(vma->vm_page_prot)
                                         | PTE_VALID | PTE_DIRTY | PTE_AF);
#else
            // vma->vm_page_prot = __pgprot(pgprot_val(vma->vm_page_prot)
            //        | L_PTE_PRESENT | L_PTE_YOUNG
            //        | L_PTE_DIRTY | L_PTE_WRITE
            //        | L_PTE_MT_DEV_CACHED);
            vma->vm_page_prot = __pgprot(pgprot_val(vma->vm_page_prot)
                                         | L_PTE_PRESENT | L_PTE_YOUNG
                                         | L_PTE_DIRTY | L_PTE_MT_DEV_CACHED);
#endif
#endif
        } else {
            /*
             * pagetable property changes from <normal nocache> to
             * <strong order>, for enhance ddr access performance,
             */
#if 0
            /*  <strong order>  */
            vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
#else
            /*  <normal nocache> */
            vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
#endif
        }
    }

    if (pfn_valid(vma->vm_pgoff)) {
        unsigned long start = vma->vm_start;
        unsigned long pfn = vma->vm_pgoff;
        size_t size = vma->vm_end - vma->vm_start;

        while (size) {
            if (pfn_valid(pfn)) {
                if (vm_insert_page(vma, start, pfn_to_page(pfn))) {
                    error("insert page failed.\n");
                    break;
                }
            } else {
#ifdef CONFIG_64BIT
                error("vm map failed for phy address(0x%llx)\n", __pfn_to_phys(pfn));
#else
                error("vm map failed for phy address(0x%x)\n", __pfn_to_phys(pfn));
#endif
            }

            start += PAGE_SIZE;
            size -= PAGE_SIZE;
            pfn++;
        }
    } else {
        size_t size = vma->vm_end - vma->vm_start;
        if (size == 0) {
            return -EPERM;
        }
        /*
         * Remap-pfn-range will mark the range
         * as VM_IO and VM_RESERVED
         */
        if (remap_pfn_range(vma,
                            vma->vm_start,
                            vma->vm_pgoff,
                            vma->vm_end - vma->vm_start,
                            vma->vm_page_prot)) {
            return -EAGAIN;
        }
    }

    return 0;
}

static int mmz_userdev_release(struct inode *inode, struct file *file)
{
    struct mmz_userdev_info *pmu = file->private_data;
    struct mmb_info *p = NULL, *n = NULL;

    list_for_each_entry_safe(p, n, &pmu->list, list) {
        error("MMB LEAK(pid=%d): 0x%lX, %lu bytes, '%s'\n",
              pmu->pid, mmz_mmb_phys(p->mmb),
              mmz_mmb_length(p->mmb),
              mmz_mmb_name(p->mmb));

        /*
         * we do not need to release mapped-area here,
         * system will do it for us
         */
        if (p->mapped != NULL) {
#if defined(KERNEL_BIT_64) && defined(USER_BIT_32)
            warning("mmb<0x%llx> mapped to userspace 0x%pK will be unmaped!\n",
                    p->phys_addr, p->mapped);
#else
            warning("mmb<0x%lx> mapped to userspace 0x%pK will be unmaped!\n",
                    p->phys_addr, p->mapped);
#endif
        }
        for (; p->mmb_ref > 0; p->mmb_ref--) {
            mmz_mmb_put(p->mmb);
        }
        _usrdev_mmb_free(p);
    }

    file->private_data = NULL;
    kfree(pmu);

    return 0;
}

static struct file_operations mmz_userdev_fops = {
    .owner = THIS_MODULE,
    .open = mmz_userdev_open,
    .release = mmz_userdev_release,
#ifdef CONFIG_COMPAT
    .compat_ioctl = compat_mmz_userdev_ioctl,
#endif
    .unlocked_ioctl = mmz_userdev_ioctl,
    .mmap = mmz_userdev_mmap,
};

static struct miscdevice mmz_userdev = {
    .minor = MISC_DYNAMIC_MINOR,
    .fops = &mmz_userdev_fops,
    .name = "mmz_userdev"
};

int __init mmz_userdev_init(void)
{
    int ret;
    ret = misc_register(&mmz_userdev);
    if (ret) {
        printk("register mmz dev failure!\n");
        return -1;
    }

    return 0;
}

void __exit mmz_userdev_exit(void)
{
    misc_deregister(&mmz_userdev);
}

/* Test func */
static int mmz_userdev_ioctl_t(struct file *file, unsigned int cmd, struct mmb_info *pmi)
{
    return 0;
}

