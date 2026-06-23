/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#include <linux/miscdevice.h>
#include <linux/mman.h>
#ifdef CONFIG_OT_SYS_SMMU_SUPPORT
#include <linux/iommu.h>
#include <linux/vendor/sva_ext.h>
#endif
#include "securec.h"
#include "ot_osal.h"
#include "mmz.h"
#include "mmz_common.h"
#include "media_mem.h"

struct mmz_userdev_info {
    pid_t pid;
    pid_t mmap_pid;
    osal_semaphore sem;
    struct osal_list_head list;
};

static int mmz_flush_dcache_mmb_dirty(struct dirty_area *p_area)
{
    if (p_area == NULL) {
        return -EINVAL;
    }

    osal_dcache_region_wb(p_area->dirty_virt_start, \
        p_area->dirty_phys_start, p_area->dirty_size);

    return 0;
}

static int mmz_userdev_open(struct inode *inode, struct file *file)
{
    struct mmz_userdev_info *pmu = NULL;

    pmu = osal_kmalloc(sizeof(*pmu), OSAL_GFP_KERNEL);
    if (pmu == NULL) {
        mmz_err_trace("alloc mmz_userdev_info failed!\n");
        return -ENOMEM;
    }
    (void)memset_s(pmu, sizeof(*pmu), 0, sizeof(*pmu));
    pmu->pid = current->pid;
    pmu->mmap_pid = 0;
    osal_sem_init(&pmu->sem, 1);
    OSAL_INIT_LIST_HEAD(&pmu->list);

    /* This file could be opened just for once */
    file->private_data = (void *)pmu;
    return 0;
}

static int ioctl_mmb_alloc(struct file *file, struct mmb_info *pmi)
{
    struct mmz_userdev_info *pmu = file->private_data;
    struct mmb_node *mmb_node = NULL;
    ot_mmb_t *mmb = NULL;
    ot_mmz_alloc_para_in para_in = {0};

    para_in.mmz_name = pmi->mmz_name;
    para_in.buf_name = pmi->mmb_name;
    para_in.size = pmi->size;
    para_in.align = pmi->align;
    para_in.gfp = pmi->gfp;
    para_in.kernel_only = 0;

    mmb = ot_mmb_alloc(&para_in);
    if (mmb == NULL) {
        mmz_err_trace("ot_mmb_alloc(%s, %lu, 0x%lx, %lu, %s) failed!\n",
            pmi->mmb_name, (unsigned long)pmi->size, (unsigned long)pmi->align, pmi->gfp, pmi->mmz_name);
        return -ENOMEM;
    }

    mmb_node = osal_kmalloc(sizeof(*mmb_node), OSAL_GFP_KERNEL);
    if (mmb_node == NULL) {
        ot_mmb_free(mmb);
        mmz_err_trace("alloc mmb_node failed!\n");
        return -ENOMEM;
    }

    (void)memcpy_s(&mmb_node->mmb_info, sizeof(mmb_node->mmb_info), pmi, sizeof(*pmi));
    mmb_node->mmb_info.phys_addr = ot_mmb_phys(mmb);
    mmb_node->mmb_info.prot = PROT_READ;
    mmb_node->mmb_info.flags = MAP_SHARED;

    mmb_node->map_ref = 0;
    mmb_node->mmb_ref = 0;
    mmb_node->mmb = mmb;
    osal_list_add_tail(&mmb_node->list, &pmu->list);

    pmi->phys_addr = mmb_node->mmb_info.phys_addr;

    ot_mmb_get(mmb);
    return 0;
}

static struct mmb_node *get_mmb_node(unsigned long addr, const struct mmz_userdev_info *pmu)
{
    struct mmb_node *p = NULL;

    osal_list_for_each_entry(p, &pmu->list, list) {
        if ((addr >= p->mmb_info.phys_addr) && (addr < (p->mmb_info.phys_addr + p->mmb_info.size))) {
            break;
        }
    }
    if (&p->list == &pmu->list) {
        return NULL;
    }
    return p;
}

static struct mmb_node *get_mmb_node_safe(unsigned long addr, const struct mmz_userdev_info *pmu)
{
    struct mmb_node *p = NULL;

    p = get_mmb_node(addr, pmu);
    if (p == NULL) {
        mmz_err_trace("mmb(0x%08lX) not found!\n", addr);
        return NULL;
    }
    return p;
}

static int mmz_usrdev_mmb_free(struct mmb_node *p)
{
    int ret;

    osal_list_del(&p->list);
    ot_mmb_put(p->mmb);
    ret = ot_mmb_free(p->mmb);
    osal_kfree(p);
    return ret;
}

static int ioctl_mmb_free(struct file *file, struct mmb_info *pmi)
{
    struct mmz_userdev_info *pmu = file->private_data;
    struct mmb_node *p = get_mmb_node_safe(pmi->phys_addr, pmu);

    if ((p == NULL) || (p->mmb == NULL)) {
        return -EPERM;
    }

    if (p->mmb_info.delayed_free) {
        mmz_warn_trace("mmb<%s> is delayed_free, can not free again!\n", p->mmb->name);
        return -EBUSY;
    }

    if ((p->map_ref > 0) || (p->mmb_ref > 0)) {
        mmz_warn_trace("mmb<%s> is still in use!\n", p->mmb->name);
        p->mmb_info.delayed_free = 1;
        return -EBUSY;
    }
    return mmz_usrdev_mmb_free(p);
}

static int ioctl_mmb_attr(struct file *file, struct mmb_info *pmi)
{
    struct mmz_userdev_info *pmu = file->private_data;
    struct mmb_node *p = NULL;

    if ((p = get_mmb_node_safe(pmi->phys_addr, pmu)) == NULL) {
        return -EPERM;
    }

    (void)memcpy_s(pmi, sizeof(*pmi), &p->mmb_info, sizeof(p->mmb_info));
    return 0;
}

static int ioctl_mmb_user_remap(struct file *file, struct mmb_info *pmi, int cached)
{
    struct mmz_userdev_info *pmu = file->private_data;
    struct mmb_node *p = NULL;
    unsigned long addr, len, prot, flags, pgoff;

    if ((p = get_mmb_node_safe(pmi->phys_addr, pmu)) == NULL) {
        return -EPERM;
    }
    /*
     * mmb could be remapped for more than once, but should not
     * be remapped with confusing cache type.
     */
    if (p->mmb_info.mapped && (p->map_ref > 0)) {
        if (cached != p->mmb_info.map_cached) {
            mmz_err_trace("mmb<%s> already mapped as %s, cannot remap as %s.\n", p->mmb->name,
                p->mmb_info.map_cached ? "cached" : "non-cached", cached ? "cached" : "non-cached");
            return -EINVAL;
        }

        p->map_ref++;
        p->mmb_ref++;

        ot_mmb_get(p->mmb);

        /*
         * pmi->phys may not always start at p->mmb_info.phys,
         * and may start with offset from p->mmb_info.phys.
         * so, we need to calculate with the offset.
         */
        pmi->mapped = p->mmb_info.mapped + (pmi->phys_addr - p->mmb_info.phys_addr);
        return 0;
    }

    if (p->mmb_info.phys_addr & ~PAGE_MASK) {
        return -EINVAL;
    }

    addr = 0;
    len = PAGE_ALIGN(p->mmb_info.size);

    prot = pmi->prot;
    flags = pmi->flags;
    if (prot == 0) {
        prot = p->mmb_info.prot;
    }
    if (flags == 0) {
        flags = p->mmb_info.flags;
    }

    pmu->mmap_pid = current->pid;
    p->mmb_info.map_cached = cached;

    pgoff = p->mmb_info.phys_addr;
    addr = vm_mmap(file, addr, len, prot, flags, pgoff);

    pmu->mmap_pid = 0;

    if (IS_ERR_VALUE((uintptr_t)addr)) {
        mmz_err_trace("vm_mmap(file,0,%lu,0x%08lX,0x%08lX,0x%08lX) return 0x%08lX\n", len, prot, flags, pgoff, addr);
        return addr;
    }

    p->mmb_info.mapped = (void *)(uintptr_t)addr;
    p->mmb_info.prot = prot;
    p->mmb_info.flags = flags;

    p->map_ref++;
    p->mmb_ref++;
    ot_mmb_get(p->mmb);

    /*
     * pmi->phys may not always start at p->mmb_info.phys,
     * and may start with offset from p->mmb_info.phys.
     * so, we need to calculate with the offset.
     */
    pmi->mapped = p->mmb_info.mapped + (pmi->phys_addr - p->mmb_info.phys_addr);
    return 0;
}

static int ioctl_mmb_user_unmap_with_cache(const struct mmb_node *p, unsigned long addr, unsigned long len)
{
    struct mm_struct *mm = current->mm;
    mmz_map_down(mm);
    if (ot_vma_check(addr, addr + len)) {
        mmz_err_trace("mmb<%s> vma is invalid.\n", p->mmb->name);
        mmz_map_up(mm);
        return -EPERM;
    }
    osal_dcache_region_wb((void *)(uintptr_t)addr, p->mmb->phys_addr, len);
    mmz_map_up(mm);
    return 0;
}

static int ioctl_mmb_user_unmap(struct file *file, struct mmb_info *pmi)
{
    int ret;
    unsigned long addr, len;
    struct mmb_node *p = NULL;
    struct mmz_userdev_info *pmu = file->private_data;

    p = get_mmb_node_safe(pmi->phys_addr, pmu);
    if (p == NULL) {
        return -EPERM;
    }

    if (p->mmb_info.mapped == NULL) {
        mmz_warn_trace("mmb(0x%lx) isn't user-mapped!\n", (unsigned long)p->mmb_info.phys_addr);
        pmi->mapped = NULL;
        return -EIO;
    }

    if (!((p->map_ref > 0) && (p->mmb_ref > 0))) {
        mmz_err_trace("mmb<%s> has invalid refer: map_ref=%d, mmb_ref=%d.\n",
            p->mmb->name, p->map_ref, p->mmb_ref);
        return -EIO;
    }

    p->map_ref--;
    p->mmb_ref--;
    ot_mmb_put(p->mmb);

    if (p->map_ref > 0) {
        return 0;
    }

    addr = (unsigned long)(uintptr_t)p->mmb_info.mapped;
    len = PAGE_ALIGN(p->mmb_info.size);

    /* before unmap,refresh cache manually */
    if (p->mmb_info.map_cached) {
        ret = ioctl_mmb_user_unmap_with_cache(p, addr, len);
        if (ret != 0) {
            return ret;
        }
    }

    ret = vm_munmap(addr, len);
    if (!IS_ERR_VALUE((uintptr_t)ret)) {
        p->mmb_info.mapped = NULL;
        pmi->mapped = NULL;
    }

    if (p->mmb_info.delayed_free && (p->map_ref == 0) && (p->mmb_ref == 0)) {
        mmz_usrdev_mmb_free(p);
    }
    return ret;
}

static int ioctl_mmb_virt2phys(struct file *file, struct mmb_info *pmi)
{
    int ret = 0;
    unsigned long virt, phys;
    unsigned long offset = 0;

    virt = (unsigned long)(uintptr_t)pmi->mapped;
    phys = usr_virt_to_phys(virt);
    if (!phys) {
        ret = -ENOMEM;
    }

    if (ot_mmb_getby_phys_2(phys, &offset) == NULL) {
        mmz_err_trace("Not mmz alloc memory[0x%lx 0x%lx]! 0x%lx\n", virt, phys, offset);
        return -EINVAL;
    }
    pmi->phys_addr = phys;
    return ret;
}

int ioctl_mmb_sys_flush_cache(__phys_addr_type__ phy_addr, void *vir_addr, unsigned int size)
{
    unsigned long end_vir_addr;
    unsigned long end_phy_addr;
    unsigned long trans_phy;

    /* check start address */
    trans_phy = usr_virt_to_phys((unsigned long)(uintptr_t)vir_addr);
    if (trans_phy == 0) {
        mmz_err_trace("start virtual address %pK is err.\n", vir_addr);
        return -1;
    }

    if ((trans_phy & 0xFFFFFFFFFFFFFFFEULL) != phy_addr) {
        mmz_err_trace("trans_phy 0x%lx and phy_addr 0x%lx are not equal!\n", trans_phy, phy_addr);
        return -1;
    }

    /* check end address */
    end_vir_addr = (unsigned long)(uintptr_t)vir_addr + size - CACHE_LINE_SIZE;
    trans_phy = usr_virt_to_phys(end_vir_addr);
    if (trans_phy == 0) {
        mmz_err_trace("end virtual address 0x%lx is err.\n", end_vir_addr);
        return -1;
    }

    end_phy_addr = phy_addr + size - CACHE_LINE_SIZE;
    if ((trans_phy & 0xFFFFFFFFFFFFFFFEULL) != end_phy_addr) {
        mmz_err_trace("trans_phy 0x%lx and end_phy_addr 0x%lx are not equal!\n", trans_phy, end_phy_addr);
        return -1;
    }

    return ot_mmb_flush_dcache_byaddr_safe(vir_addr, phy_addr, size);
}

int ioctl_mmb_check_mmz_phy_addr(unsigned long long phy_addr, unsigned int len)
{
    /* if address in mmz of current system */
    if (ot_is_phys_in_mmz((unsigned long)phy_addr, len)) {
        /* if in other system */
        if (ot_map_mmz_check_phys((unsigned long)phy_addr, len)) {
            return -1;
        }
    }
    return 0;
}

static int mmz_userdev_ioctl_m(struct file *file, unsigned int cmd, struct mmb_info *pmi)
{
    switch (_IOC_NR(cmd)) {
        case _IOC_NR(IOC_MMB_ALLOC):
            return ioctl_mmb_alloc(file, pmi);
        case _IOC_NR(IOC_MMB_ATTR):
            return ioctl_mmb_attr(file, pmi);
        case _IOC_NR(IOC_MMB_FREE):
            return ioctl_mmb_free(file, pmi);
        case _IOC_NR(IOC_MMB_USER_REMAP):
            return ioctl_mmb_user_remap(file, pmi, 0);
        case _IOC_NR(IOC_MMB_USER_REMAP_CACHED):
            return ioctl_mmb_user_remap(file, pmi, 1);
        case _IOC_NR(IOC_MMB_USER_UNMAP):
            return ioctl_mmb_user_unmap(file, pmi);
        case _IOC_NR(IOC_MMB_VIRT_GET_PHYS):
            return ioctl_mmb_virt2phys(file, pmi);
        case _IOC_NR(IOC_MMB_SYS_FLUSH_CACHE):
            return ioctl_mmb_sys_flush_cache(pmi->phys_addr, pmi->mapped, pmi->size);
        case _IOC_NR(IOC_MMB_BASE_CHECK_ADDR):
            return ioctl_mmb_check_mmz_phy_addr(pmi->phys_addr, pmi->size);
        default:
            mmz_err_trace("invalid ioctl cmd = %08X\n", cmd);
            return -EINVAL;
    }
}

static int mmz_userdev_ioctl_of_m(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct mmb_info mi = { 0 };
    int ret;

    if ((_IOC_SIZE(cmd) > sizeof(mi)) || (arg == 0)) {
        mmz_err_trace("_IOC_SIZE(cmd)=%d\n", _IOC_SIZE(cmd));
        return -EINVAL;
    }
    (void)memset_s(&mi, sizeof(mi), 0, sizeof(mi));
    if (copy_from_user(&mi, (void *)(uintptr_t)arg, _IOC_SIZE(cmd))) {
        mmz_err_trace("copy_from_user error.\n");
        return -EFAULT;
    }
    mi.mmz_name[OT_MMZ_NAME_LEN - 1] = '\0';
    mi.mmb_name[OT_MMB_NAME_LEN - 1] = '\0';
    ret = mmz_userdev_ioctl_m(file, cmd, &mi);
    if (!ret && (cmd & IOC_OUT)) {
        if (copy_to_user((void *)(uintptr_t)arg, &mi, _IOC_SIZE(cmd))) {
            mmz_err_trace("copy_to_user error.\n");
            return -EFAULT;
        }
    }
    return ret;
}

static int ioctl_mmb_mem_share(const struct mmb_share_info *share_info)
{
    ot_mmb_t *mmb = NULL;

    mmb = media_mem_get_mmb_by_handle(share_info->mem_handle);
    if (mmb == NULL) {
        mmz_err_trace("invalid mem handle!\n");
        return -EINVAL;
    }

    if (mmb->kernel_only == 1) {
        media_mem_put_mmb(mmb);
        mmz_err_trace("share failed, mmb used only in kernel!\n");
        return -EPERM;
    }

    if (!media_mem_is_check_pid()) {
        media_mem_put_mmb(mmb);
        return 0;
    }

    if (mmb->check_pid != 1) {
        media_mem_put_mmb(mmb);
        mmz_err_trace("buffer is already share all!\n");
        return -EPERM;
    }
    /* pid[0] can share */
    if (osal_get_current_tgid() != mmb->pid[0]) {
        media_mem_put_mmb(mmb);
        mmz_err_trace("only process that alloc buffer can share!\n");
        return -EPERM;
    }
    if (share_info->shared_pid == mmb->pid[0]) {
        media_mem_put_mmb(mmb);
        mmz_err_trace("cannot share to process that alloc buffer!\n");
        return -EPERM;
    }
    if (mmb->pid_num >= OT_MMB_MAX_PID_NUM) {
        media_mem_put_mmb(mmb);
        mmz_err_trace("share failed, max pid_num is %d!\n", OT_MMB_MAX_PID_NUM);
        return -EPERM;
    }
    if (find_pid_in_mmb(mmb, share_info->shared_pid) < 0) {
        /* share */
        mmb->pid[mmb->pid_num++] = share_info->shared_pid;
    }
    media_mem_put_mmb(mmb);
    return 0;
}

static int ioctl_mmb_mem_unshare(const struct mmb_share_info *share_info)
{
    ot_mmb_t *mmb = NULL;
    int cur_pid;
    int pos;

    mmb = media_mem_get_mmb_by_handle(share_info->mem_handle);
    if (mmb == NULL) {
        mmz_err_trace("invalid mem handle!\n");
        return -EINVAL;
    }

    if (mmb->kernel_only == 1) {
        media_mem_put_mmb(mmb);
        mmz_err_trace("unshare failed, mmb used only in kernel!\n");
        return -EPERM;
    }

    if (!media_mem_is_check_pid()) {
        media_mem_put_mmb(mmb);
        return 0;
    }

    if (mmb->check_pid != 1) {
        media_mem_put_mmb(mmb);
        mmz_err_trace("buffer is already share all!\n");
        return -EPERM;
    }
    /* pid[0] and pid[n] can unshare with pid[n] (except pid[0]) */
    pos = find_pid_in_mmb(mmb, share_info->shared_pid);
    if (pos < 0) {
        media_mem_put_mmb(mmb);
        mmz_err_trace("mmb is not shared with pid(%d)!\n", share_info->shared_pid);
        return -EPERM;
    }
    if (share_info->shared_pid == mmb->pid[0]) {
        media_mem_put_mmb(mmb);
        mmz_err_trace("unshare failed, pid(%d) is the process that alloc buffer!\n", share_info->shared_pid);
        return -EPERM;
    }
    cur_pid = osal_get_current_tgid();
    if (cur_pid != mmb->pid[0] && share_info->shared_pid != cur_pid) {
        media_mem_put_mmb(mmb);
        mmz_err_trace("process cannot unshare with pid(%d)!\n", share_info->shared_pid);
        return -EPERM;
    }
    /* unshare */
    clear_pid_in_mmb(mmb, pos);
    media_mem_put_mmb(mmb);
    return 0;
}

static int ioctl_mmb_mem_share_all(const struct mmb_share_info *share_info)
{
    ot_mmb_t *mmb = NULL;

    mmb = media_mem_get_mmb_by_handle(share_info->mem_handle);
    if (mmb == NULL) {
        mmz_err_trace("invalid mem handle!\n");
        return -EINVAL;
    }

    if (mmb->kernel_only == 1) {
        media_mem_put_mmb(mmb);
        mmz_err_trace("share all failed, mmb used only in kernel!\n");
        return -EPERM;
    }

    if ((!media_mem_is_check_pid()) || (mmb->check_pid != 1)) {
        media_mem_put_mmb(mmb);
        return 0;
    }
    /* pid[0] can share all */
    if (osal_get_current_tgid() != mmb->pid[0]) {
        media_mem_put_mmb(mmb);
        mmz_err_trace("only process that alloc buffer can share all!\n");
        return -EPERM;
    }
    /* share all */
    mmb->check_pid = 0;
    media_mem_put_mmb(mmb);
    return 0;
}

static int ioctl_mmb_mem_unshare_all(const struct mmb_share_info *share_info)
{
    ot_mmb_t *mmb = NULL;

    mmb = media_mem_get_mmb_by_handle(share_info->mem_handle);
    if (mmb == NULL) {
        mmz_err_trace("invalid mem handle!\n");
        return -EINVAL;
    }

    if (mmb->kernel_only == 1) {
        media_mem_put_mmb(mmb);
        mmz_err_trace("unshare all failed, mmb used only in kernel!\n");
        return -EPERM;
    }

    if ((!media_mem_is_check_pid()) || (mmb->check_pid == 1)) {
        media_mem_put_mmb(mmb);
        return 0;
    }
    /* pid[0] can unshare all */
    if (osal_get_current_tgid() != mmb->pid[0]) {
        media_mem_put_mmb(mmb);
        mmz_err_trace("only process that alloc buffer can unshare all!\n");
        return -EPERM;
    }
    /* unshare all */
    mmb->check_pid = 1;
    media_mem_put_mmb(mmb);
    return 0;
}

static int ioctl_mmb_get_sys_mem_by_virt(struct mmb_share_info *share_info)
{
    unsigned long virt_addr, phys_addr;
    ot_mmb_t *mmb = NULL;

    virt_addr = (unsigned long)(uintptr_t)share_info->virt_addr;
    phys_addr = usr_virt_to_phys(virt_addr);
    if (phys_addr == 0) {
        mmz_err_trace("invalid virt_addr!\n");
        return -EINVAL;
    }

    mmb = media_mem_get_mmb_by_phys(phys_addr);
    if (mmb == NULL) {
        mmz_err_trace("not mmz alloc memory[0x%lx 0x%lx]!\n", virt_addr, phys_addr);
        return -EINVAL;
    }

    share_info->phys_addr = mmb->phys_addr;
    share_info->offset = phys_addr - mmb->phys_addr;
    share_info->mem_handle = mmb;
    media_mem_put_mmb(mmb);
    return 0;
}

static int ioctl_mmb_get_sys_mem_by_phys(struct mmb_share_info *share_info)
{
    unsigned long phys_addr;
    ot_mmb_t *mmb = NULL;

    phys_addr = (unsigned long)share_info->phys_addr;
    mmb = media_mem_get_mmb_by_phys(phys_addr);
    if (mmb == NULL) {
        mmz_err_trace("not mmz alloc memory[0x%lx]!\n", phys_addr);
        return -EINVAL;
    }

    if (ot_mmb_check_mem_share(mmb) < 0) {
        media_mem_put_mmb(mmb);
        mmz_err_trace("mem share check failed!\n");
        return -EPERM;
    }

    share_info->phys_addr = mmb->phys_addr;
    share_info->offset = phys_addr - mmb->phys_addr;
    share_info->mem_handle = mmb;
    media_mem_put_mmb(mmb);
    return 0;
}

static int ioctl_mmb_get_sys_mem_by_handle(struct mmb_share_info *share_info)
{
    ot_mmb_t *mmb = NULL;

    mmb = media_mem_get_mmb_by_handle(share_info->mem_handle);
    if (mmb == NULL) {
        mmz_err_trace("invalid mem handle!\n");
        return -EINVAL;
    }

    if (ot_mmb_check_mem_share(mmb) < 0) {
        media_mem_put_mmb(mmb);
        mmz_err_trace("mem share check failed!\n");
        return -EPERM;
    }

    share_info->phys_addr = mmb->phys_addr;
    share_info->offset = 0;
    share_info->mem_handle = mmb;
    media_mem_put_mmb(mmb);
    return 0;
}

static int mmz_userdev_ioctl_s(struct file *file, unsigned int cmd, struct mmb_share_info *share_info)
{
    switch (_IOC_NR(cmd)) {
        case _IOC_NR(IOC_MMB_MEM_SHARE):
            return ioctl_mmb_mem_share(share_info);
        case _IOC_NR(IOC_MMB_MEM_UNSHARE):
            return ioctl_mmb_mem_unshare(share_info);
        case _IOC_NR(IOC_MMB_MEM_SHARE_ALL):
            return ioctl_mmb_mem_share_all(share_info);
        case _IOC_NR(IOC_MMB_MEM_UNSHARE_ALL):
            return ioctl_mmb_mem_unshare_all(share_info);
        case _IOC_NR(IOC_MMB_VIRT_GET_SYS_MEM):
            return ioctl_mmb_get_sys_mem_by_virt(share_info);
        case _IOC_NR(IOC_MMB_PHYS_GET_SYS_MEM):
            return ioctl_mmb_get_sys_mem_by_phys(share_info);
        case _IOC_NR(IOC_MMB_HANDLE_GET_SYS_MEM):
            return ioctl_mmb_get_sys_mem_by_handle(share_info);
        default:
            mmz_err_trace("invalid ioctl cmd = %08X\n", cmd);
            return -EINVAL;
    }
}

static int mmz_userdev_ioctl_of_s(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct mmb_share_info share_info = {0};
    int ret;

    if ((_IOC_SIZE(cmd) > sizeof(share_info)) || (arg == 0)) {
        mmz_err_trace("_IOC_SIZE(cmd)=%d\n", _IOC_SIZE(cmd));
        return -EINVAL;
    }

    if (copy_from_user(&share_info, (void *)(uintptr_t)arg, _IOC_SIZE(cmd))) {
        mmz_err_trace("copy_from_user error.\n");
        return -EFAULT;
    }

    ret = mmz_userdev_ioctl_s(file, cmd, &share_info);
    if (!ret && (cmd & IOC_OUT)) {
        if (copy_to_user((void *)(uintptr_t)arg, &share_info, _IOC_SIZE(cmd))) {
            mmz_err_trace("copy_to_user error.\n");
            return -EFAULT;
        }
    }
    return ret;
}

static int mmz_userdev_check_dirty_area_range(const struct dirty_area *area,
    const struct mmz_userdev_info *pmu)
{
    ot_mmb_t *mmb = NULL;
    struct mmb_node *mmb_node = NULL;
    unsigned long offset;

    mmb = media_mem_get_mmb_by_phys(area->dirty_phys_start);
    if (mmb == NULL) {
        mmz_err_trace("dirty_phys_addr=0x%lx\n", (unsigned long)area->dirty_phys_start);
        return -EFAULT;
    }

    mmb_node = get_mmb_node_safe(mmb->phys_addr, pmu);
    if (mmb_node == NULL) {
        media_mem_put_mmb(mmb);
        return -EPERM;
    }
    offset = area->dirty_phys_start - mmb->phys_addr;
    if ((uintptr_t)area->dirty_virt_start != (uintptr_t)mmb_node->mmb_info.mapped + offset) {
        media_mem_put_mmb(mmb);
        mmz_warn_trace("dirty_virt_start addr was not consistent with dirty_phys_start addr!\n");
        return -EFAULT;
    }
    if (area->dirty_phys_start + area->dirty_size > mmb->phys_addr + mmb->length) {
        media_mem_put_mmb(mmb);
        mmz_warn_trace("dirty area overflow!\n");
        return -EFAULT;
    }
    media_mem_put_mmb(mmb);
    return 0;
}

static int mmz_userdev_ioctl_of_d(unsigned int cmd, unsigned long arg, const struct mmz_userdev_info *pmu)
{
    struct dirty_area area;
    __phys_addr_type__ orig_addr;
    unsigned long virt_addr;
    struct mm_struct *mm = current->mm;
    int ret;

    if ((_IOC_SIZE(cmd) != sizeof(area)) || (arg == 0)) {
        mmz_err_trace("_IOC_SIZE(cmd)=%d\n", _IOC_SIZE(cmd));
        return -EINVAL;
    }
    (void)memset_s(&area, sizeof(area), 0, sizeof(area));
    if (copy_from_user(&area, (void *)(uintptr_t)arg, _IOC_SIZE(cmd))) {
        mmz_warn_trace("copy_from_user error.\n");
        return -EFAULT;
    }

    ret = mmz_userdev_check_dirty_area_range(&area, pmu);
    if (ret != 0) {
        return ret;
    }

    mmz_map_down(mm);

    if (ot_vma_check((uintptr_t)area.dirty_virt_start, (uintptr_t)area.dirty_virt_start + area.dirty_size)) {
        mmz_warn_trace("dirty area[0x%lx,0x%lx] overflow!\n", (unsigned long)(uintptr_t)area.dirty_virt_start,
            (unsigned long)(uintptr_t)area.dirty_virt_start + area.dirty_size);
        mmz_map_up(mm);
        return -EFAULT;
    }

    /* cache line aligned */
    orig_addr = area.dirty_phys_start;
    area.dirty_phys_start &= ~(CACHE_LINE_SIZE - 1);
    virt_addr = (unsigned long)(uintptr_t)area.dirty_virt_start;
    virt_addr &= ~(CACHE_LINE_SIZE - 1);
    area.dirty_virt_start = (void *)(uintptr_t)virt_addr;
    area.dirty_size = (area.dirty_size + (orig_addr - area.dirty_phys_start) +
                      (CACHE_LINE_SIZE - 1)) & ~(CACHE_LINE_SIZE - 1);

    mmz_flush_dcache_mmb_dirty(&area);
    mmz_map_up(mm);
    return 0;
}

static long mmz_userdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int ret;
    struct mmz_userdev_info *pmu = file->private_data;

    if (osal_sem_down(&pmu->sem)) {
        return -ERESTARTSYS;
    }

    if (_IOC_TYPE(cmd) == MMZ_MMB_IOC_MAGIC) {
        ret = mmz_userdev_ioctl_of_m(file, cmd, arg);
    } else if (_IOC_TYPE(cmd) == MMZ_MMB_SHARE_IOC_MAGIC) {
        ret = mmz_userdev_ioctl_of_s(file, cmd, arg);
    } else if (_IOC_TYPE(cmd) == MMZ_MMB_DCACHE_DIRTY_IOC_MAGIC) {
        ret = mmz_userdev_ioctl_of_d(cmd, arg, pmu);
    } else {
        ret = -EINVAL;
    }

    osal_sem_up(&pmu->sem);
    return ret;
}

#ifdef CONFIG_COMPAT
static long compat_mmz_userdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    return mmz_userdev_ioctl(file, cmd, (unsigned long)(uintptr_t)compat_ptr(arg));
}
#endif

static void mmz_userdev_mmap_page_valid(struct vm_area_struct *vma)
{
    unsigned long start = vma->vm_start;
    unsigned long pfn = vma->vm_pgoff;
    size_t size = vma->vm_end - vma->vm_start;

    while (size) {
        if (pfn_valid(pfn)) {
            if (vm_insert_page(vma, start, pfn_to_page(pfn))) {
                mmz_err_trace("insert page failed.\n");
                break;
            }
        } else {
            mmz_err_trace("vm map failed for phy address(0x%lx)\n", (unsigned long)__pfn_to_phys(pfn));
        }

        start += PAGE_SIZE;
        size = (size < PAGE_SIZE) ? 0 : (size - PAGE_SIZE);
        pfn++;
    }
}

static int mmz_userdev_mmap_page_invalid(struct vm_area_struct *vma)
{
    size_t size = vma->vm_end - vma->vm_start;
    if (size == 0) {
        return -EPERM;
    }
    /*
     * Remap-pfn-range will mark the range
     * as VM_IO and VM_RESERVED
     */
    if (remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff, vma->vm_end - vma->vm_start,
                        vma->vm_page_prot)) {
        return -EAGAIN;
    }
    return 0;
}

static int mmz_userdev_do_mmap(struct vm_area_struct *vma, int mmb_mapped_k, int mmb_cached_k, int mmb_cached_u)
{
    if (mmb_cached_u == 0) {
        /* mapping as non-cacheable */
        if ((mmb_mapped_k != 0) && (mmb_cached_k != 0)) {
            /* mmb has already been mapped and mapped as cacheable */
            mmz_err_trace("memory mmap[0x%lx, 0x%lx] prot as non-cacheable conflit\n",
                vma->vm_pgoff, vma->vm_end - vma->vm_start);
            return -EPERM;
        }
#ifdef CONFIG_64BIT
        vma->vm_page_prot = __pgprot(pgprot_val(vma->vm_page_prot)
                                     | PTE_WRITE | PTE_DIRTY);
#endif
        vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
    } else {
        /* mapping as cacheable */
        if ((mmb_mapped_k != 0) && (mmb_cached_k == 0)) {
            /* mmb has already been mapped and mapped as non-cacheable */
            mmz_err_trace("memory mmap[0x%lx, 0x%lx] prot as cacheable conflit\n",
                vma->vm_pgoff, vma->vm_end - vma->vm_start);
            return -EPERM;
        }
#ifdef CONFIG_64BIT
        vma->vm_page_prot = __pgprot(pgprot_val(vma->vm_page_prot)
                                     | PTE_WRITE | PTE_DIRTY);
#else
        vma->vm_page_prot = __pgprot(pgprot_val(vma->vm_page_prot)
                                     | L_PTE_PRESENT | L_PTE_YOUNG
                                     | L_PTE_DIRTY | L_PTE_MT_DEV_CACHED);
#endif
    }

    if (pfn_valid(vma->vm_pgoff)) {
        mmz_userdev_mmap_page_valid(vma);
        return 0;
    }
    return mmz_userdev_mmap_page_invalid(vma);
}

int mmz_userdev_mmap(struct file *file, struct vm_area_struct *vma)
{
    struct mmb_node *p = NULL;
    unsigned long offset;
    ot_mmb_t *mmb = NULL;
    int mmb_mapped_k = 0;
    int mmb_cached_k = 0;
    int mmb_cached_u;
    int ret;

    if ((file == NULL) || (file->private_data == NULL) || (vma == NULL) || (vma->vm_mm == NULL)) {
        return -1;
    }

    offset = vma->vm_pgoff << PAGE_SHIFT;
    p = get_mmb_node(offset, (struct mmz_userdev_info *)file->private_data);
    mmb = media_mem_get_mmb_by_phys_and_size(offset, vma->vm_end - vma->vm_start);
    if (p == NULL) {
        if (mmb == NULL) {
            /*
             * Allow mmap MMZ allocated by other co-working OS, but we don't have and
             * don't manage any information on its memory mapping. So we presume
             * the memory is not mapped before, this is actually a compromise.
             */
            if (ot_map_mmz_check_phys(offset, vma->vm_end - vma->vm_start)) {
                return -EPERM;
            }
        } else {
            if (ot_mmb_check_mem_share(mmb) < 0) {
                media_mem_put_mmb(mmb);
                return -EPERM;
            }
            /* Obviously the mmb is allocated before, and may be mapped in kernel space. */
            mmb_mapped_k = mmb->flags & OT_MMB_MAP2KERN;
            mmb_cached_k = mmb->flags & OT_MMB_MAP2KERN_CACHED;
            media_mem_put_mmb(mmb);
        }
        /*
         * If file opened as O_SYNC, mmap the mmz area as write-combine, which
         * is also non-cacheable, otherwise mmap the area as normal(cacheable).
         */
        mmb_cached_u = (file->f_flags & O_SYNC) ? 0 : 1;
    } else {
        if (mmb == NULL) {
            mmz_err_trace("the mmap addr must be in the mmb.\n");
            return -EPERM;
        }
        media_mem_put_mmb(mmb);
        /* The mmb allocated by ioctl processing function in the same fd. */
        if (p->mmb_info.mapped != NULL) {
            /*
             * Since mapped is the virtual address of the mmb in current process,
             * it's clear that the mmb is mapped when it get here.
             */
            mmb_mapped_k = 1;
            mmb_cached_k = p->mmb_info.map_cached;
            mmb_cached_u = (file->f_flags & O_SYNC) ? 0 : 1;
        } else {
            /* The value of map_cached is from ioctl processing function, which is non-cacheable or cacheable. */
            mmb_cached_u = p->mmb_info.map_cached;
        }
    }

    ret = mmz_userdev_do_mmap(vma, mmb_mapped_k, mmb_cached_k, mmb_cached_u);
    if (ret != 0) {
        return ret;
    }

#ifdef CONFIG_OT_SYS_SMMU_SUPPORT
    /* flush page table cache to share the data with CBBs that have sMMU */
    svm_flush_cache(vma->vm_mm, vma->vm_start, vma->vm_end - vma->vm_start);
#endif

    return 0;
}

static int mmz_userdev_release(struct inode *inode, struct file *file)
{
    struct mmz_userdev_info *pmu = file->private_data;
    struct mmb_node *p = NULL;
    struct mmb_node *n = NULL;
    int pid = pmu->pid;

    osal_list_for_each_entry_safe(p, n, &pmu->list, list) {
        mmz_err_trace("MMB LEAK(pid=%d): 0x%lX, %lu bytes, '%s'\n",
            pmu->pid, ot_mmb_phys(p->mmb), ot_mmb_length(p->mmb), ot_mmb_name(p->mmb));
        /*
         * we do not need to release mapped-area here,
         * system will do it for us
         */
        if (p->mmb_info.mapped != NULL) {
            mmz_printk(KERN_WARNING "mmb<0x%lx> mapped to userspace 0x%pK will be unmaped!\n",
                (unsigned long)p->mmb_info.phys_addr, p->mmb_info.mapped);
        }
        for (; p->mmb_ref > 0; p->mmb_ref--) {
            ot_mmb_put(p->mmb);
        }
        mmz_usrdev_mmb_free(p);
    }
    osal_sem_destroy(&pmu->sem);

    file->private_data = NULL;
    osal_kfree(pmu);

    if (media_mem_is_check_pid()) {
        media_mem_clear_pid_in_all_mmb(pid);
    }
    return 0;
}

static struct file_operations g_mmz_userdev_fops = {
    .owner = THIS_MODULE,
    .open = mmz_userdev_open,
    .release = mmz_userdev_release,
#ifdef CONFIG_COMPAT
    .compat_ioctl = compat_mmz_userdev_ioctl,
#endif
    .unlocked_ioctl = mmz_userdev_ioctl,
    .mmap = mmz_userdev_mmap,
};

static struct miscdevice g_mmz_userdev = {
    .minor = MISC_DYNAMIC_MINOR,
    .fops = &g_mmz_userdev_fops,
    .name = "mmz_userdev"
};

int __init mmz_userdev_init(void)
{
    int ret;
    ret = misc_register(&g_mmz_userdev);
    if (ret) {
        mmz_err_trace("register mmz dev failure!\n");
        return -1;
    }

    return 0;
}

void __exit mmz_userdev_exit(void)
{
    misc_deregister(&g_mmz_userdev);
}
