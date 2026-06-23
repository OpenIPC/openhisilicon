/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/mm_types.h>
#include <linux/mm.h>
#include <linux/kmod.h>
#include <linux/freezer.h>
#include <linux/rwlock.h>
#include <linux/rwlock_types.h>
#include <linux/arm-smccc.h>
#include "ot_osal.h"
#include "ot_media.h"
#include "base.h"
#include "securec.h"
#include "osal_inner.h"

#define OSAL_MAX_DEV_NAME_LEN 48
typedef struct osal_dev_info_ {
    osal_dev *dev;
    pm_device meida;
    struct osal_list_head node;
} osal_dev_info;

typedef struct osal_fileops_info_ {
    void *private_data;
    osal_dev *dev;
    osal_fileops *fops;
} osal_fileops_info;

static OSAL_LIST_HEAD(dev_list);

static DEFINE_RWLOCK(g_dev_rwlock);

static osal_pmops *osal_get_pmops(const pm_basedev *pm_base)
{
    osal_dev_info *dev_node = NULL;

    if (pm_base == NULL || pm_base->name == NULL) {
        return NULL;
    }
    read_lock(&g_dev_rwlock);

    osal_list_for_each_entry(dev_node, &dev_list, node) {
        if (dev_node->dev == NULL) {
            read_unlock(&g_dev_rwlock);
            return NULL;
        }
        if (osal_strcmp(dev_node->dev->name, pm_base->name) == 0) {
            read_unlock(&g_dev_rwlock);
            return dev_node->dev->pmops;
        }
    }

    read_unlock(&g_dev_rwlock);

    return NULL;
}

static int osal_open(struct inode *inode, struct file *file)
{
    int ret = 0;
    osal_fileops *fops = NULL;
    osal_dev *dev = NULL;
    osal_fileops_info *fileops_node = NULL;
    if (file == NULL) {
        osal_log("file NULL!\n");
        return OSAL_FAILURE;
    }
    dev = (osal_dev *)file->private_data;
    if (dev == NULL || dev->fops == NULL) {
        osal_log("get media device error!\n");
        return -1;
    }

    fileops_node = (osal_fileops_info *)kzalloc(sizeof(osal_fileops_info), GFP_KERNEL);
    if (fileops_node == NULL) {
        osal_log("fileops_node kmalloc failed!\n");
        return -1;
    }

    fops = dev->fops;
    fileops_node->dev = dev;
    fileops_node->private_data = dev;
    if (fops->open != NULL) {
        ret = fops->open((void *)&(fileops_node->private_data));
        if (ret != 0) {
            kfree(fileops_node);
            return -1;
        }
    }

    fileops_node->fops = fops;
    file->private_data = fileops_node;

    return ret;
}

static ssize_t osal_read(struct file *file, char __user *buf, size_t size, loff_t *offset)
{
    osal_fileops_info *fileops_node = NULL;
    if (file == NULL) {
        osal_log("file NULL!\n");
        return OSAL_FAILURE;
    }
    fileops_node = (osal_fileops_info *)file->private_data;
    if (fileops_node == NULL || fileops_node->fops == NULL) {
        osal_log("fileops_node NULL!\n");
        return OSAL_FAILURE;
    }

    if (fileops_node->fops->read != NULL) {
        return fileops_node->fops->read(buf, (int)size, (long *)offset, (void *)&(fileops_node->private_data));
    }

    return 0;
}

static ssize_t osal_write(struct file *file, const char __user *buf, size_t size, loff_t *offset)
{
    osal_unused(file, buf, size, offset);
    return 0;
}

static loff_t osal_llseek(struct file *file, loff_t offset, int whence)
{
    osal_unused(file, offset, whence);
    return 0;
}

static int osal_release(struct inode *inode, struct file *file)
{
    int ret = 0;
    osal_fileops_info *fileops_node = NULL;
    osal_unused(inode);
    if (file == NULL) {
        osal_log("file NULL!\n");
        return OSAL_FAILURE;
    }
    fileops_node = (osal_fileops_info *)file->private_data;
    if (fileops_node == NULL || fileops_node->fops == NULL) {
        osal_log("fileops_node NULL!\n");
        return OSAL_FAILURE;
    }

    if (fileops_node->fops->release != NULL) {
        ret = fileops_node->fops->release((void *)&(fileops_node->private_data));
    }

    kfree(file->private_data);
    file->private_data = NULL;

    return ret;
}

#define ARG_BUF_TEMP_SIZE 256

static int osal_ioctl_copy_from_user(unsigned int cmd, unsigned long arg, void **ioctl_arg, unsigned int arg_size,
    char *arg_buf)
{
    int ret = -1;

    /*  Copy arguments into temp kernel buffer  */
    switch (_IOC_DIR(cmd)) {
        case _IOC_NONE:
            *ioctl_arg = NULL;
            break;
        case _IOC_READ:
        case _IOC_WRITE:
        case (_IOC_WRITE | _IOC_READ):
            if (arg_size <= ARG_BUF_TEMP_SIZE) {
                *ioctl_arg = arg_buf;
            } else {
                /* too big to allocate from stack; <16K use kmalloc */
                *ioctl_arg = kmalloc(arg_size, GFP_KERNEL);
                if (*ioctl_arg == NULL) {
                    printk("kmalloc cmd buffer failed\n");
                    return -ENOMEM;
                }
            }

            if (_IOC_DIR(cmd) & _IOC_WRITE) {
                ret = (int)copy_from_user(*ioctl_arg, (void __user *)(uintptr_t)arg, arg_size);
                if (ret != 0) {
                    printk("copy_from_user failed, cmd=0x%x\n", cmd);
                    return -1;
                }
            }

            break;
        default:
            osal_dump_stack();
            printk("cmd %x invalid\n", _IOC_DIR(cmd));
    }

    return 0;
}

static int osal_ioctl_call_fun(unsigned int cmd, void *ioctl_arg, const osal_fileops *fops, void *private_data)
{
    unsigned int i;
    /* call func; only used inosal_ioctl; no need check cmd, ioctl_arg, fops */
    for (i = 0; i < fops->cmd_cnt; i++) {
        if (fops->cmd_list[i].cmd != cmd || fops->cmd_list[i].handler == NULL) {
            continue;
        }
        return fops->cmd_list[i].handler(cmd, ioctl_arg, private_data);
    }

    return OSAL_FAILURE;
}

static int osal_ioctl_copy_to_user(unsigned int cmd, unsigned long arg, const void *ioctl_arg, unsigned int arg_size)
{
    /*  Copy results into user buffer  */
    switch (_IOC_DIR(cmd)) {
        case _IOC_READ:
        case (_IOC_WRITE | _IOC_READ): {
            unsigned long ret = copy_to_user((void __user *)(uintptr_t)arg, ioctl_arg, arg_size);
            return (ret == 0) ? 0 : -1;
        }
        /* when the user runs the write only command, no content is returned. */
        case _IOC_WRITE:
        case _IOC_NONE:
            break;
        default:
            osal_dump_stack();
            printk("cmd %x invalid\n", _IOC_DIR(cmd));
    }

    return 0;
}

static long osal_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int ret;
    unsigned int arg_size;
    char arg_buf[ARG_BUF_TEMP_SIZE] = {}; /* temp to prevent tiny vmalloc */
    void *ioctl_arg = NULL;
    osal_fileops *fops = NULL;
    osal_fileops_info *fileops_node = NULL;

    if (file == NULL) {
        osal_log("file NULL!\n");
        return OSAL_FAILURE;
    }

    arg_size = _IOC_SIZE(cmd);

    if (_IOC_DIR(cmd) != _IOC_NONE) {
        if ((arg_size == 0) || (((char *)(uintptr_t)arg) == NULL)) {
            return OSAL_FAILURE; /* arg_size max 0x4000, no need to check max */
        }
    }

    fileops_node = file->private_data;
    if (fileops_node == NULL || fileops_node->fops == NULL) {
        osal_log("fileops_node NULL!\n");
        return OSAL_FAILURE;
    }

    fops = fileops_node->fops;

    if ((fops->cmd_cnt == 0) || (fops->cmd_list == NULL)) {
        return OSAL_FAILURE;
    }

    ret = osal_ioctl_copy_from_user(cmd, arg, &ioctl_arg, arg_size, arg_buf);
    if (ret != 0) {
        goto OUT;
    }

    ret = osal_ioctl_call_fun(cmd, ioctl_arg, fops, (void *)&(fileops_node->private_data));
    if (ret != 0) {
        goto OUT;
    }

    ret = osal_ioctl_copy_to_user(cmd, arg, ioctl_arg, arg_size);
    if (ret != 0) {
        goto OUT;
    }

OUT:
    if ((ioctl_arg != NULL) && (arg_size > ARG_BUF_TEMP_SIZE)) {
        kfree(ioctl_arg);
    }

    return ret;
}

static unsigned int _osal_poll(struct file *file, struct poll_table_struct *table)
{
    unsigned int ret = 0;
    osal_poll t;
    osal_fileops *fops = NULL;
    osal_fileops_info *fileops_node = NULL;

    t.poll_table = table;
    t.data = file;
    fileops_node = file->private_data;
    if (fileops_node == NULL || fileops_node->fops == NULL) {
        osal_log("fileops_node NULL!\n");
        return OSAL_FAILURE;
    }

    fops = fileops_node->fops;

    if (fops->poll != NULL) {
        ret = fops->poll(&t, (void *)&(fileops_node->private_data));
    }

    return ret;
}

static int osal_valid_mmap_phys_addr_range(unsigned long pfn, size_t size)
{
    /* check physical addr greater than the max addr supported by the system */
    if ((pfn + (size >> PAGE_SHIFT)) > (1 + ((~0UL) >> PAGE_SHIFT))) {
        osal_log("The physical address is greater than the maximum address supported by the system\n");
        return 0;
    }

    /* It's necessary for the variable "size" to align 4k(page_size). */
#define PAGE_SIZE_MASK 0xfffffffffffff000ULL
    if ((unsigned long)size & (~PAGE_SIZE_MASK)) {
        osal_log("This variable size is not align 4k!\n");
        return 0;
    }
#undef PAGE_SIZE_MASK

    return 1;
}

static int osal_dev_mmap(struct file *file, struct vm_area_struct *vm)
{
    int ret = 0;
    osal_vm osal_vm;
    osal_fileops *fops = NULL;
    osal_fileops_info *fileops_node = NULL;
    if (file == NULL || vm == NULL) {
        return OSAL_FAILURE;
    }
    if (!osal_valid_mmap_phys_addr_range(vm->vm_pgoff, vm->vm_end - vm->vm_start)) {
        osal_log("invalid argument   size=%ld!!!\n", vm->vm_end - vm->vm_start);
        return -EINVAL;
    }

    fileops_node = file->private_data;
    if (fileops_node == NULL || fileops_node->fops == NULL) {
        osal_log("fileops_node NULL!\n");
        return OSAL_FAILURE;
    }

    fops = fileops_node->fops;

    osal_vm.vm = vm;
    if (fops->mmap != NULL) {
        ret = fops->mmap(&osal_vm, vm->vm_start, vm->vm_end, vm->vm_pgoff, (void *)&(fileops_node->private_data));
    }

    return ret;
}

static int osal_fasync(int fd, struct file *filp, int mode)
{
    int ret = 0;
    osal_fileops *fops = NULL;
    osal_fileops_info *fileops_node = NULL;
    if (filp == NULL) {
        osal_log("filp NULL!\n");
        return OSAL_FAILURE;
    }
    fileops_node = filp->private_data;
    if (fileops_node == NULL || fileops_node->fops == NULL) {
        osal_log("fileops_node NULL!\n");
        return OSAL_FAILURE;
    }

    fops = fileops_node->fops;

    if (fops->fasync != NULL) {
        ret = fops->fasync(fd, filp, mode);
    }

    return ret;
}

static struct file_operations g_osal_fops = {
    .open = osal_open,
    .read = osal_read,
    .write = osal_write,
    .llseek = osal_llseek,
    .unlocked_ioctl = osal_ioctl,
    .release = osal_release,
    .poll = _osal_poll,
    .mmap = osal_dev_mmap,
    .fasync = osal_fasync,
#ifdef CONFIG_COMPAT
    .compat_ioctl = osal_ioctl,
#endif
};

static void osal_pm_shutdown(const pm_basedev *pm_base)
{
    osal_pmops *pmops = NULL;

    pmops = osal_get_pmops(pm_base);
    if (pmops == NULL) {
        return;
    }

    if (pmops->pm_poweroff != NULL) {
        if (pmops->private_data != NULL) {
            (void)pmops->pm_poweroff(pmops->private_data);
        } else {
            (void)pmops->pm_poweroff(NULL);
        }
    }
}

static int osal_pm_suspend(const pm_basedev *pm_base, pm_message_t state)
{
    osal_pmops *pmops = NULL;

    pmops = osal_get_pmops(pm_base);
    if (pmops == NULL) {
        return -1;
    }

    if (pmops->pm_suspend != NULL) {
        if (pmops->private_data != NULL) {
            return pmops->pm_suspend(pmops->private_data);
        } else {
            return pmops->pm_suspend(NULL);
        }
    }

    return -1;
}

static int osal_pm_resume(const pm_basedev *pm_base)
{
    osal_pmops *pmops = NULL;

    pmops = osal_get_pmops(pm_base);
    if (pmops == NULL) {
        return -1;
    }

    if (pmops->pm_resume != NULL) {
        if (pmops->private_data != NULL) {
            return pmops->pm_resume(pmops->private_data);
        } else {
            return pmops->pm_resume(NULL);
        }
    }

    return -1;
}

static int osal_pm_resume_early(const pm_basedev *pm_base)
{
    osal_pmops *pmops = NULL;

    pmops = osal_get_pmops(pm_base);
    if (pmops == NULL) {
        return 0;
    }

    if (pmops->pm_resume_early != NULL) {
        if (pmops->private_data != NULL) {
            return pmops->pm_resume_early(pmops->private_data);
        } else {
            return pmops->pm_resume_early(NULL);
        }
    }

    return 0;
}

void osal_pm_lowpower_enter(void)
{
    osal_dev_info *dev_node = NULL;

    osal_list_for_each_entry(dev_node, &dev_list, node) {
        if ((dev_node->dev != NULL) && (dev_node->dev->pmops != NULL) &&
            (dev_node->dev->pmops->pm_lowpower_enter != NULL)) {
            dev_node->dev->pmops->pm_lowpower_enter(dev_node->dev->pmops->private_data);
        }
    }
}
EXPORT_SYMBOL(osal_pm_lowpower_enter);

void osal_pm_lowpower_exit(void)
{
    osal_dev_info *dev_node = NULL;

    osal_list_for_each_entry_reverse(dev_node, &dev_list, node) {
        if ((dev_node->dev != NULL) && (dev_node->dev->pmops != NULL) &&
            (dev_node->dev->pmops->pm_lowpower_exit != NULL)) {
            dev_node->dev->pmops->pm_lowpower_exit(dev_node->dev->pmops->private_data);
        }
    }
}
EXPORT_SYMBOL(osal_pm_lowpower_exit);

static pm_baseops g_osal_pmops = {
    .probe = NULL,
    .remove = NULL,
    .shutdown = osal_pm_shutdown,
    .prepare = NULL,
    .complete = NULL,
    .suspend = osal_pm_suspend,
    .suspend_late = NULL,
    .resume_early = osal_pm_resume_early,
    .resume = osal_pm_resume,
};

osal_dev *osal_dev_create(const char *name)
{
    osal_dev *dev = NULL;
    int ret;

    if (name == NULL) {
        osal_log("parameter invalid!\n");
        return NULL;
    }
    dev = kmalloc(sizeof(osal_dev), GFP_KERNEL);
    if (dev == NULL) {
        osal_log("kmalloc error!\n");
        return NULL;
    }

    if (memset_s(dev, sizeof(osal_dev), 0, sizeof(osal_dev)) != EOK) {
        kfree(dev);
        osal_log("memset_s failed!\n");
        return NULL;
    }
    ret = strncpy_s(dev->name, OSAL_DEV_NAME_LEN, name, strlen(name));
    if (ret != EOK) {
        kfree(dev);
        osal_log("strncpy_s failed!\n");
        return NULL;
    }
    return dev;
}
EXPORT_SYMBOL(osal_dev_create);

int osal_dev_destroy(osal_dev *dev)
{
    if (dev == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    kfree(dev);
    return OSAL_SUCCESS;
}
EXPORT_SYMBOL(osal_dev_destroy);

int osal_dev_register(osal_dev *dev)
{
    osal_dev_info *dev_node = NULL;
    pm_device *meida = NULL;

    if (dev == NULL || (dev->fops == NULL)) {
        osal_log("dev invalid!\n");
        osal_dump_stack();
        return OSAL_FAILURE;
    }

    dev_node = (osal_dev_info *)kmalloc(sizeof(osal_dev_info), GFP_KERNEL);
    if (dev_node == NULL) {
        osal_log("kmalloc error!\n");
        osal_dump_stack();
        return OSAL_FAILURE;
    }

    if (memset_s(dev_node, sizeof(osal_dev_info), 0, sizeof(osal_dev_info)) != EOK) {
        kfree(dev_node);
        dev_node = NULL;
        osal_log("memset_s is failed.\n");
        osal_dump_stack();
        return OSAL_FAILURE;
    }

    dev_node->dev = dev;
    dev->dev = (void *)dev_node;
    meida = &dev_node->meida;

    meida->minor = ((dev->minor != 0) ? dev->minor : MEDIA_DYNAMIC_MINOR);

    if (dev->owner != NULL) {
        meida->owner = (struct module *)dev->owner;
        g_osal_fops.owner = (struct module *)dev->owner;
    } else {
        meida->owner = THIS_MODULE;
        g_osal_fops.owner = THIS_MODULE;
    }

    meida->app_ops = &g_osal_fops;
    meida->base_ops = &g_osal_pmops;
    meida->name = dev->name;
    meida->dev = dev;
    meida->parent_minor = ((dev->parent_minor != 0) ? (dev->parent_minor) : (0));
    if (drv_pm_register(meida) != TRUE) {
        kfree(dev_node);
        return OSAL_FAILURE;
    }

    write_lock(&g_dev_rwlock);
    osal_list_add(&dev_node->node, &dev_list);
    write_unlock(&g_dev_rwlock);
    return OSAL_SUCCESS;
}
EXPORT_SYMBOL(osal_dev_register);

void osal_dev_unregister(osal_dev *dev)
{
    osal_dev_info *dev_node = NULL;

    if (dev == NULL || dev->dev == NULL) {
        osal_log("dev invalid!\n");
        osal_dump_stack();
        return;
    }

    dev_node = (osal_dev_info *)(dev->dev);

    drv_pm_un_register(&dev_node->meida);

    write_lock(&g_dev_rwlock);
    osal_list_del(&dev_node->node);
    write_unlock(&g_dev_rwlock);
    kfree(dev_node);
    dev->dev = NULL;
}
EXPORT_SYMBOL(osal_dev_unregister);

void osal_poll_wait(osal_poll *table, osal_wait *wait)
{
    if (table == NULL || table->data == NULL || table->poll_table == NULL) {
        osal_log("table(NULL) invalid!\n");
        osal_dump_stack();
        return;
    }

    if (wait == NULL || wait->wait == NULL) {
        osal_log("wait(NULL) invalid!\n");
        osal_dump_stack();
        return;
    }

    poll_wait((struct file *)table->data, (wait_queue_head_t *)(wait->wait), table->poll_table);
}
EXPORT_SYMBOL(osal_poll_wait);

void osal_pgprot_noncached(osal_vm *vm)
{
    struct vm_area_struct *v = NULL;

    if ((vm == NULL) || (vm->vm == NULL)) {
        osal_log("vm(NULL) invalid!\n");
        return;
    }
    v = (struct vm_area_struct *)(vm->vm);
    v->vm_page_prot = pgprot_noncached(v->vm_page_prot);
}
EXPORT_SYMBOL(osal_pgprot_noncached);

void osal_pgprot_cached(osal_vm *vm)
{
    struct vm_area_struct *v = NULL;

    if ((vm == NULL) || (vm->vm == NULL)) {
        osal_log("vm(NULL) invalid!\n");
        return;
    }
    v = (struct vm_area_struct *)(vm->vm);

#ifdef CONFIG_64BIT
    v->vm_page_prot = __pgprot(pgprot_val(v->vm_page_prot) | PTE_VALID | PTE_DIRTY | PTE_AF);
#else
    v->vm_page_prot =
        __pgprot(pgprot_val(v->vm_page_prot) | L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY | L_PTE_MT_DEV_CACHED);
#endif
}
EXPORT_SYMBOL(osal_pgprot_cached);

void osal_pgprot_writecombine(osal_vm *vm)
{
    struct vm_area_struct *v = NULL;

    if ((vm == NULL) || (vm->vm == NULL)) {
        osal_log("vm(NULL) invalid!\n");
        return;
    }
    v = (struct vm_area_struct *)(vm->vm);
    v->vm_page_prot = pgprot_writecombine(v->vm_page_prot);
}
EXPORT_SYMBOL(osal_pgprot_writecombine);

int osal_remap_pfn_range(osal_vm *vm, unsigned long addr, unsigned long pfn, unsigned long size)
{
    struct vm_area_struct *v = NULL;

    if ((vm == NULL) || (vm->vm == NULL)) {
        osal_log("vm(NULL) invalid!\n");
        osal_dump_stack();
        return OSAL_FAILURE;
    }

    if (size == 0) {
        osal_log("size(0) invalid!\n");
        osal_dump_stack();
        return OSAL_FAILURE;
    }

    v = (struct vm_area_struct *)(vm->vm);

    return remap_pfn_range(v, addr, pfn, size, v->vm_page_prot);
}
EXPORT_SYMBOL(osal_remap_pfn_range);

int osal_try_to_freeze(void)
{
    return try_to_freeze();
}
EXPORT_SYMBOL(osal_try_to_freeze);

int osal_set_freezable(void)
{
    return set_freezable();
}
EXPORT_SYMBOL(osal_set_freezable);

int osal_kobject_uevent_env(osal_dev *dev, osal_kobject_action action, char *envp[])
{
    osal_dev_info *dev_node = NULL;
    struct device *base_dev = NULL;
    pm_device *meida = NULL;

    if (dev == NULL || dev->dev == NULL) {
        osal_log("dev invalid!\n");
        osal_dump_stack();
        return -1;
    }

    dev_node = (osal_dev_info *)dev->dev;
    meida = &dev_node->meida;
    base_dev = &meida->base_device->dev;

    return kobject_uevent_env(&base_dev->kobj, (enum kobject_action)action, envp);
}
EXPORT_SYMBOL(osal_kobject_uevent_env);

int osal_fasync_helper(int fd, void *filp, int mode, void **fapp)
{
    int ret;

    if (fd < 0 || filp == NULL) {
        printk("osal fasync create input params invalid.\n");
        osal_dump_stack();
        return OSAL_FAILURE;
    }

    ret = fasync_helper(fd, (struct file *)filp, mode, (struct fasync_struct **)fapp);
    if (ret != 1) {
        printk("osal fasync create failed ret:0x%x\n", ret);
        osal_dump_stack();
        return OSAL_FAILURE;
    }
    return OSAL_SUCCESS;
}
EXPORT_SYMBOL(osal_fasync_helper);

void osal_fasync_notify(void **fapp, int sig, int band)
{
    if (fapp == NULL) {
        printk("osal fasync create input params invalid.\n");
        osal_dump_stack();
        return;
    }
    kill_fasync((struct fasync_struct **)fapp, sig, band);
}
EXPORT_SYMBOL(osal_fasync_notify);

void osal_device_set_async(unsigned int minor)
{
    osal_dev_info *dev_node = NULL;

    osal_list_for_each_entry(dev_node, &dev_list, node) {
        if (dev_node->meida.minor == minor) {
            device_enable_async_suspend(dev_node->meida.app_device);
            device_enable_async_suspend(&(dev_node->meida.base_device->dev));
            pr_info("PM_RESUME: enable pm async %s\n", dev_node->meida.name);
            break;
        }
    }

    return;
}
EXPORT_SYMBOL(osal_device_set_async);

void osal_smccc_smc(const osal_smccc_info *info, osal_smccc_res *res)
{
    struct arm_smccc_res res_tmp = { 0 };

    if (info == NULL || res == NULL) {
        osal_log("info or res null ptr!\n");
        return;
    }

    arm_smccc_smc(info->a0, info->a1, info->a2, info->a3, info->a4, info->a5, info->a6, info->a7, &res_tmp);

    res->a0 = res_tmp.a0;
    res->a1 = res_tmp.a1;
    res->a2 = res_tmp.a2;
    res->a3 = res_tmp.a3;
}
EXPORT_SYMBOL(osal_smccc_smc);
