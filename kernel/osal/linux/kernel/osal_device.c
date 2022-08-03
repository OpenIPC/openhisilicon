/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include "media.h"
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/mm_types.h>
#include <linux/mm.h>
#include <linux/kmod.h>
#include "osal.h"

#define DRVAL_DEBUG 0

#define GET_FILE(file)                    \
	do {                              \
		if (__get_file(file) < 0) \
			return -1;        \
	} while (0)

#define PUT_FILE(file)                    \
	do {                              \
		if (__put_file(file) < 0) \
			return -1;        \
	} while (0)

typedef struct osal_coat_dev {
	struct osal_dev osal_dev;
	struct media_device media_dev;
} osal_coat_dev_t;

struct osal_private_data {
	struct osal_dev *dev;
	void *data;
	struct osal_poll table;
	int f_ref_cnt;
};

spinlock_t f_lock;

void osal_device_init(void)
{
	spin_lock_init(&f_lock);
}

static int __get_file(struct file *file)
{
	struct osal_private_data *pdata = NULL;

	spin_lock(&f_lock);
	pdata = file->private_data;
	if (pdata == NULL) {
		spin_unlock(&f_lock);
		return -1;
	}

	pdata->f_ref_cnt++;
	spin_unlock(&f_lock);

	return 0;
}

static int __put_file(struct file *file)
{
	struct osal_private_data *pdata = NULL;

	spin_lock(&f_lock);
	pdata = file->private_data;
	if (pdata == NULL) {
		spin_unlock(&f_lock);
		return -1;
	}

	pdata->f_ref_cnt--;
	spin_unlock(&f_lock);

	return 0;
}

static int osal_open(struct inode *inode, struct file *file)
{
	struct media_device *media = NULL;
	osal_coat_dev_t *coat_dev = NULL;
	struct osal_private_data *pdata = NULL;

	if (!capable(CAP_SYS_RAWIO) || !capable(CAP_SYS_ADMIN)) {
		return -EPERM;
	}

	// media = getmedia(inode);
	media = (struct media_device *)file->private_data;
	if (media == NULL) {
		osal_printk("%s - get media device error!\n", __FUNCTION__);
		return -1;
	}
	coat_dev = osal_container_of(media, struct osal_coat_dev, media_dev);
	pdata = (struct osal_private_data *)kmalloc(
		sizeof(struct osal_private_data), GFP_KERNEL);
	if (pdata == NULL) {
		osal_printk("%s - kmalloc error!\n", __FUNCTION__);
		return -1;
	}
	if (DRVAL_DEBUG) {
		osal_printk("%s - file->private_data=%pK!\n", __FUNCTION__,
			    pdata);
	}

	memset(pdata, 0, sizeof(struct osal_private_data));

	file->private_data = pdata;
	pdata->dev = &(coat_dev->osal_dev);
	if (coat_dev->osal_dev.fops->open != NULL) {
		return coat_dev->osal_dev.fops->open((void *)&(pdata->data));
	}
	return 0;
}

static ssize_t osal_read(struct file *file, char __user *buf, size_t size,
			 loff_t *offset)
{
	struct osal_private_data *pdata = file->private_data;
	int ret = 0;

	GET_FILE(file);

	if (pdata->dev->fops->read != NULL) {
		ret = pdata->dev->fops->read(buf, (int)size, (long *)offset,
					     (void *)&(pdata->data));
	}

	PUT_FILE(file);
	return ret;
}

static ssize_t osal_write(struct file *file, const char __user *buf,
			  size_t size, loff_t *offset)
{
	struct osal_private_data *pdata = file->private_data;
	int ret = 0;

	GET_FILE(file);
	if (pdata->dev->fops->write != NULL) {
		ret = pdata->dev->fops->write(buf, (int)size, (long *)offset,
					      (void *)&(pdata->data));
	}
	PUT_FILE(file);
	return ret;
}

static loff_t osal_llseek(struct file *file, loff_t offset, int whence)
{
	struct osal_private_data *pdata = file->private_data;
	int ret = 0;

	GET_FILE(file);
	if (DRVAL_DEBUG) {
		osal_printk("%s - file->private_data=%pK!\n", __FUNCTION__,
			    pdata);
	}

	if (whence == SEEK_SET) {
		if (pdata->dev->fops->llseek != NULL) {
			ret = pdata->dev->fops->llseek((long)offset,
						       OSAL_SEEK_SET,
						       (void *)&(pdata->data));
		}
	} else if (whence == SEEK_CUR) {
		if (pdata->dev->fops->llseek != NULL) {
			ret = pdata->dev->fops->llseek((long)offset,
						       OSAL_SEEK_CUR,
						       (void *)&(pdata->data));
		}
	} else if (whence == SEEK_END) {
		if (pdata->dev->fops->llseek != NULL) {
			ret = pdata->dev->fops->llseek((long)offset,
						       OSAL_SEEK_END,
						       (void *)&(pdata->data));
		}
	}

	PUT_FILE(file);
	return (loff_t)ret;
}

static int osal_release(struct inode *inode, struct file *file)
{
	int ret = 0;
	struct osal_private_data *pdata = file->private_data;

	GET_FILE(file);

	if (DRVAL_DEBUG) {
		osal_printk("%s - file->private_data=%pK!\n", __FUNCTION__,
			    pdata);
	}

	if (pdata->dev->fops->release != NULL) {
		ret = pdata->dev->fops->release((void *)&(pdata->data));
	}
	if (ret != 0) {
		PUT_FILE(file);
		osal_printk("%s - release failed!\n", __FUNCTION__);
		return ret;
	}

	PUT_FILE(file);
	spin_lock(&f_lock);
	if (pdata->f_ref_cnt != 0) {
		osal_printk("%s - release failed!\n", __FUNCTION__);
		spin_unlock(&f_lock);
		return -1;
	}
	kfree(file->private_data);
	file->private_data = NULL;
	spin_unlock(&f_lock);

	return 0;
}

static long __osal_unlocked_ioctl(struct file *file, unsigned int cmd,
				  unsigned long arg)
{
	int ret = -1;
	struct osal_private_data *pdata = file->private_data;
	char *kbuf = NULL;

	if (DRVAL_DEBUG) {
		osal_printk("%s - file->private_data=%pK!\n", __FUNCTION__,
			    pdata);
	}

	if (((_IOC_SIZE(cmd) == 0) && (_IOC_DIR(cmd) != _IOC_NONE))) {
		return -1;
	}

	if ((_IOC_DIR(cmd) != _IOC_NONE) &&
	    (((char *)(uintptr_t)arg) == NULL)) {
		osal_printk("%s - Input param err,it is null!\n", __FUNCTION__);
		return -1;
	}

	if (_IOC_DIR(cmd) == _IOC_NONE) {
		if (pdata->dev->fops->unlocked_ioctl == NULL) {
			return -1;
		} else {
			ret = pdata->dev->fops->unlocked_ioctl(
				cmd, arg, (void *)&(pdata->data));
		}
	} else if (_IOC_DIR(cmd) == _IOC_WRITE) {
		kbuf = (char *)vmalloc(_IOC_SIZE(cmd));
		if (kbuf == NULL) {
			osal_printk("%s - vmalloc failed!\n", __FUNCTION__);
			return -1;
		}

		if (copy_from_user(kbuf, (char *)(uintptr_t)arg,
				   _IOC_SIZE(cmd))) {
			vfree(kbuf);
			return -1;
		}

		if (pdata->dev->fops->unlocked_ioctl == NULL) {
			vfree(kbuf);
			return -1;
		} else {
			ret = pdata->dev->fops->unlocked_ioctl(
				cmd, (unsigned long)(uintptr_t)kbuf,
				(void *)&(pdata->data));
		}
	} else if (_IOC_DIR(cmd) == _IOC_READ) {
		kbuf = vmalloc(_IOC_SIZE(cmd));
		if (kbuf == NULL) {
			osal_printk("%s - vmalloc failed!\n", __FUNCTION__);
			return -1;
		}
		if (pdata->dev->fops->unlocked_ioctl == NULL) {
			vfree(kbuf);
			return -1;
		} else {
			ret = pdata->dev->fops->unlocked_ioctl(
				cmd, (unsigned long)(uintptr_t)kbuf,
				(void *)&(pdata->data));
			if (ret == 0) {
				if (copy_to_user((char *)(uintptr_t)arg, kbuf,
						 _IOC_SIZE(cmd))) {
					vfree(kbuf);
					return -1;
				}
			}
		}
	} else if (_IOC_DIR(cmd) == (_IOC_READ + _IOC_WRITE)) {
		kbuf = vmalloc(_IOC_SIZE(cmd));
		if (kbuf == NULL) {
			osal_printk("%s - vmalloc failed!\n", __FUNCTION__);
			return -1;
		}

		if (copy_from_user(kbuf, (char *)(uintptr_t)arg,
				   _IOC_SIZE(cmd))) {
			vfree(kbuf);
			return -1;
		}

		if (pdata->dev->fops->unlocked_ioctl == NULL) {
			vfree(kbuf);
			return -1;
		} else {
			ret = pdata->dev->fops->unlocked_ioctl(
				cmd, (unsigned long)(uintptr_t)kbuf,
				(void *)&(pdata->data));
			if (ret == 0) {
				if (copy_to_user((char *)(uintptr_t)arg, kbuf,
						 _IOC_SIZE(cmd))) {
					vfree(kbuf);
					return -1;
				}
			}
		}
	}

	if (kbuf != NULL) {
		vfree(kbuf);
	}

	return ret;
}

static long osal_unlocked_ioctl(struct file *file, unsigned int cmd,
				unsigned long arg)
{
	long ret = 0;

	GET_FILE(file);

	ret = __osal_unlocked_ioctl(file, cmd, arg);
	PUT_FILE(file);

	return ret;
}

#ifdef CONFIG_COMPAT
static long __osal_compat_ioctl(struct file *file, unsigned int cmd,
				unsigned long arg)
{
	int ret = -1;
	struct osal_private_data *pdata = file->private_data;
	char *kbuf = NULL;

	if (DRVAL_DEBUG) {
		osal_printk("%s - file->private_data=%pK!\n", __FUNCTION__,
			    pdata);
	}

	if (((_IOC_SIZE(cmd) == 0) && (_IOC_DIR(cmd) != _IOC_NONE))) {
		return -1;
	}

	if ((_IOC_DIR(cmd) != _IOC_NONE) &&
	    (((char *)(uintptr_t)arg) == NULL)) {
		osal_printk("%s - Input param err,it is null!\n", __FUNCTION__);
		return -1;
	}

	if (_IOC_DIR(cmd) == _IOC_NONE) {
		if (pdata->dev->fops->compat_ioctl == NULL) {
			return -1;
		} else {
			ret = pdata->dev->fops->compat_ioctl(
				cmd, arg, (void *)&(pdata->data));
		}
	} else if (_IOC_DIR(cmd) == _IOC_WRITE) {
		kbuf = (char *)vmalloc(_IOC_SIZE(cmd));
		if (kbuf == NULL) {
			osal_printk("%s - vmalloc failed!\n", __FUNCTION__);
			return -1;
		}

		if (copy_from_user(kbuf, (char *)(uintptr_t)arg,
				   _IOC_SIZE(cmd))) {
			vfree(kbuf);
			return -1;
		}

		if (pdata->dev->fops->compat_ioctl == NULL) {
			vfree(kbuf);
			return -1;
		} else {
			ret = pdata->dev->fops->compat_ioctl(
				cmd, (unsigned long)(uintptr_t)kbuf,
				(void *)&(pdata->data));
		}
	} else if (_IOC_DIR(cmd) == _IOC_READ) {
		kbuf = vmalloc(_IOC_SIZE(cmd));
		if (kbuf == NULL) {
			osal_printk("%s - vmalloc failed!\n", __FUNCTION__);
			return -1;
		}
		if (pdata->dev->fops->compat_ioctl == NULL) {
			vfree(kbuf);
			return -1;
		} else {
			ret = pdata->dev->fops->compat_ioctl(
				cmd, (unsigned long)(uintptr_t)kbuf,
				(void *)&(pdata->data));
			if (ret == 0) {
				if (copy_to_user((char *)(uintptr_t)arg, kbuf,
						 _IOC_SIZE(cmd))) {
					vfree(kbuf);
					return -1;
				}
			}
		}
	} else if (_IOC_DIR(cmd) == (_IOC_READ + _IOC_WRITE)) {
		kbuf = vmalloc(_IOC_SIZE(cmd));
		if (kbuf == NULL) {
			osal_printk("%s - vmalloc failed!\n", __FUNCTION__);
			return -1;
		}

		if (copy_from_user(kbuf, (char *)(uintptr_t)arg,
				   _IOC_SIZE(cmd))) {
			vfree(kbuf);
			return -1;
		}

		if (pdata->dev->fops->compat_ioctl == NULL) {
			vfree(kbuf);
			return -1;
		} else {
			ret = pdata->dev->fops->compat_ioctl(
				cmd, (unsigned long)(uintptr_t)kbuf,
				(void *)&(pdata->data));
			if (ret == 0) {
				if (copy_to_user((char *)(uintptr_t)arg, kbuf,
						 _IOC_SIZE(cmd))) {
					vfree(kbuf);
					return -1;
				}
			}
		}
	}

	if (kbuf != NULL) {
		vfree(kbuf);
	}

	return ret;
}

static long osal_compat_ioctl(struct file *file, unsigned int cmd,
			      unsigned long arg)
{
	long ret = 0;

	GET_FILE(file);

	ret = __osal_compat_ioctl(file, cmd, arg);
	PUT_FILE(file);

	return ret;
}

#endif

static unsigned int osal_poll(struct file *file,
			      struct poll_table_struct *table)
{
	struct osal_private_data *pdata = file->private_data;
	struct osal_poll t;
	unsigned int ret = 0;

	GET_FILE(file);

	if (DRVAL_DEBUG) {
		osal_printk("%s - table=%pK, file=%pK!\n", __FUNCTION__, table,
			    file);
	}
	t.poll_table = table;
	t.data = file;
	if (pdata->dev->fops->poll != NULL) {
		ret = pdata->dev->fops->poll(&t, (void *)&(pdata->data));
	}

	PUT_FILE(file);
	return ret;
}

extern int allocator_type;

int valid_mmap_phys_addr_range(unsigned long pfn, size_t size)
{
	/* check physical addr greater than the max addr supported by the system */
	if ((pfn + (size >> PAGE_SHIFT)) > (1 + ((~0UL) >> PAGE_SHIFT))) {
		osal_printk("--%s - %d--!\n", __FUNCTION__, __LINE__);
		return 0;
	}

	/* CMA need to alloc ram region, so annotate the code as follows */
	if (allocator_type == 0) {
		/* check physical addr is ram region */
		if (pfn_valid(pfn) || pfn_valid(pfn + (size >> PAGE_SHIFT))) {
			osal_printk("--%s - %d--!\n", __FUNCTION__, __LINE__);
			return 0;
		}
	}

	/* It's necessary for the variable "size" to align 4k(page_size). */
#define PAGE_SIZE_MASK 0xfffffffffffff000ULL
	if ((unsigned long)size & (~PAGE_SIZE_MASK)) {
		osal_printk("--%s - %d--!\n", __FUNCTION__, __LINE__);
		return 0;
	}
#undef PAGE_SIZE_MASK

	return 1;
}

static int osal_mmap(struct file *file, struct vm_area_struct *vm)
{
	struct osal_vm osal_vm;
	struct osal_private_data *pdata = file->private_data;
	osal_vm.vm = vm;

	if (!valid_mmap_phys_addr_range(vm->vm_pgoff,
					vm->vm_end - vm->vm_start)) {
		osal_printk("\n%s - invalid argument   size=%ld!!!\n",
			    __FUNCTION__, vm->vm_end - vm->vm_start);
		return -EINVAL;
	}

	if (DRVAL_DEBUG) {
		osal_printk("%s - start=%lx, end=%lx!, off=%lx\n", __FUNCTION__,
			    vm->vm_start, vm->vm_end, vm->vm_pgoff);
	}
	if (pdata->dev->fops->mmap != NULL) {
		return pdata->dev->fops->mmap(&osal_vm, vm->vm_start,
					      vm->vm_end, vm->vm_pgoff,
					      (void *)&(pdata->data));
	}
	return 0;
}

static struct file_operations s_osal_fops = {
	.owner = THIS_MODULE,
	.open = osal_open,
	.read = osal_read,
	.write = osal_write,
	.llseek = osal_llseek,
	.unlocked_ioctl = osal_unlocked_ioctl,
	.release = osal_release,
	.poll = osal_poll,
	.mmap = osal_mmap,
#ifdef CONFIG_COMPAT
	.compat_ioctl = osal_compat_ioctl,
#endif
};

static int osal_pm_prepare(struct media_device *media)
{
	osal_coat_dev_t *coat_dev =
		container_of(media, struct osal_coat_dev, media_dev);
	if (coat_dev->osal_dev.osal_pmops &&
	    coat_dev->osal_dev.osal_pmops->pm_prepare) {
		return coat_dev->osal_dev.osal_pmops->pm_prepare(
			&(coat_dev->osal_dev));
	}
	return 0;
}

static void osal_pm_complete(struct media_device *media)
{
	osal_coat_dev_t *coat_dev =
		container_of(media, struct osal_coat_dev, media_dev);
	if (coat_dev->osal_dev.osal_pmops &&
	    coat_dev->osal_dev.osal_pmops->pm_complete) {
		coat_dev->osal_dev.osal_pmops->pm_complete(
			&(coat_dev->osal_dev));
	}
}

static int osal_pm_suspend(struct media_device *media)
{
	osal_coat_dev_t *coat_dev =
		container_of(media, struct osal_coat_dev, media_dev);
	if (coat_dev->osal_dev.osal_pmops &&
	    coat_dev->osal_dev.osal_pmops->pm_suspend) {
		return coat_dev->osal_dev.osal_pmops->pm_suspend(
			&(coat_dev->osal_dev));
	}
	return 0;
}

static int osal_pm_resume(struct media_device *media)
{
	osal_coat_dev_t *coat_dev =
		container_of(media, struct osal_coat_dev, media_dev);
	if (coat_dev->osal_dev.osal_pmops &&
	    coat_dev->osal_dev.osal_pmops->pm_resume) {
		return coat_dev->osal_dev.osal_pmops->pm_resume(
			&(coat_dev->osal_dev));
	}
	return 0;
}

static int osal_pm_freeze(struct media_device *media)
{
	osal_coat_dev_t *coat_dev =
		container_of(media, struct osal_coat_dev, media_dev);
	if (coat_dev->osal_dev.osal_pmops &&
	    coat_dev->osal_dev.osal_pmops->pm_freeze) {
		return coat_dev->osal_dev.osal_pmops->pm_freeze(
			&(coat_dev->osal_dev));
	}
	return 0;
}

static int osal_pm_thaw(struct media_device *media)
{
	osal_coat_dev_t *coat_dev =
		container_of(media, struct osal_coat_dev, media_dev);
	if (coat_dev->osal_dev.osal_pmops &&
	    coat_dev->osal_dev.osal_pmops->pm_thaw) {
		return coat_dev->osal_dev.osal_pmops->pm_thaw(
			&(coat_dev->osal_dev));
	}
	return 0;
}

static int osal_pm_poweroff(struct media_device *media)
{
	osal_coat_dev_t *coat_dev =
		container_of(media, struct osal_coat_dev, media_dev);
	if (coat_dev->osal_dev.osal_pmops &&
	    coat_dev->osal_dev.osal_pmops->pm_poweroff) {
		return coat_dev->osal_dev.osal_pmops->pm_poweroff(
			&(coat_dev->osal_dev));
	}
	return 0;
}

static int osal_pm_restore(struct media_device *media)
{
	osal_coat_dev_t *coat_dev =
		container_of(media, struct osal_coat_dev, media_dev);
	if (coat_dev->osal_dev.osal_pmops &&
	    coat_dev->osal_dev.osal_pmops->pm_restore) {
		return coat_dev->osal_dev.osal_pmops->pm_restore(
			&(coat_dev->osal_dev));
	}
	return 0;
}

static int osal_pm_suspend_late(struct media_device *media)
{
	osal_coat_dev_t *coat_dev =
		container_of(media, struct osal_coat_dev, media_dev);
	if (coat_dev->osal_dev.osal_pmops &&
	    coat_dev->osal_dev.osal_pmops->pm_suspend_late) {
		return coat_dev->osal_dev.osal_pmops->pm_suspend_late(
			&(coat_dev->osal_dev));
	}
	return 0;
}

static int osal_pm_resume_early(struct media_device *media)
{
	osal_coat_dev_t *coat_dev =
		container_of(media, struct osal_coat_dev, media_dev);
	if (coat_dev->osal_dev.osal_pmops &&
	    coat_dev->osal_dev.osal_pmops->pm_resume_early) {
		return coat_dev->osal_dev.osal_pmops->pm_resume_early(
			&(coat_dev->osal_dev));
	}
	return 0;
}

static int osal_pm_freeze_late(struct media_device *media)
{
	osal_coat_dev_t *coat_dev =
		container_of(media, struct osal_coat_dev, media_dev);
	if (coat_dev->osal_dev.osal_pmops &&
	    coat_dev->osal_dev.osal_pmops->pm_freeze_late) {
		return coat_dev->osal_dev.osal_pmops->pm_freeze_late(
			&(coat_dev->osal_dev));
	}
	return 0;
}

static int osal_pm_thaw_early(struct media_device *media)
{
	osal_coat_dev_t *coat_dev =
		container_of(media, struct osal_coat_dev, media_dev);
	if (coat_dev->osal_dev.osal_pmops &&
	    coat_dev->osal_dev.osal_pmops->pm_thaw_early) {
		return coat_dev->osal_dev.osal_pmops->pm_thaw_early(
			&(coat_dev->osal_dev));
	}
	return 0;
}

static int osal_pm_poweroff_late(struct media_device *media)
{
	osal_coat_dev_t *coat_dev =
		container_of(media, struct osal_coat_dev, media_dev);
	if (coat_dev->osal_dev.osal_pmops &&
	    coat_dev->osal_dev.osal_pmops->pm_poweroff_late) {
		return coat_dev->osal_dev.osal_pmops->pm_poweroff_late(
			&(coat_dev->osal_dev));
	}
	return 0;
}

static int osal_pm_restore_early(struct media_device *media)
{
	osal_coat_dev_t *coat_dev =
		container_of(media, struct osal_coat_dev, media_dev);
	if (coat_dev->osal_dev.osal_pmops &&
	    coat_dev->osal_dev.osal_pmops->pm_restore_early) {
		return coat_dev->osal_dev.osal_pmops->pm_restore_early(
			&(coat_dev->osal_dev));
	}
	return 0;
}

static int osal_pm_suspend_noirq(struct media_device *media)
{
	osal_coat_dev_t *coat_dev =
		container_of(media, struct osal_coat_dev, media_dev);
	if (coat_dev->osal_dev.osal_pmops &&
	    coat_dev->osal_dev.osal_pmops->pm_suspend_noirq) {
		return coat_dev->osal_dev.osal_pmops->pm_suspend_noirq(
			&(coat_dev->osal_dev));
	}
	return 0;
}

static int osal_pm_resume_noirq(struct media_device *media)
{
	osal_coat_dev_t *coat_dev =
		container_of(media, struct osal_coat_dev, media_dev);
	if (coat_dev->osal_dev.osal_pmops &&
	    coat_dev->osal_dev.osal_pmops->pm_resume_noirq) {
		return coat_dev->osal_dev.osal_pmops->pm_resume_noirq(
			&(coat_dev->osal_dev));
	}
	return 0;
}

static int osal_pm_freeze_noirq(struct media_device *media)
{
	osal_coat_dev_t *coat_dev =
		container_of(media, struct osal_coat_dev, media_dev);
	if (coat_dev->osal_dev.osal_pmops &&
	    coat_dev->osal_dev.osal_pmops->pm_freeze_noirq) {
		return coat_dev->osal_dev.osal_pmops->pm_freeze_noirq(
			&(coat_dev->osal_dev));
	}
	return 0;
}

static int osal_pm_thaw_noirq(struct media_device *media)
{
	osal_coat_dev_t *coat_dev =
		container_of(media, struct osal_coat_dev, media_dev);
	if (coat_dev->osal_dev.osal_pmops &&
	    coat_dev->osal_dev.osal_pmops->pm_thaw_noirq) {
		return coat_dev->osal_dev.osal_pmops->pm_thaw_noirq(
			&(coat_dev->osal_dev));
	}
	return 0;
}

static int osal_pm_poweroff_noirq(struct media_device *media)
{
	osal_coat_dev_t *coat_dev =
		container_of(media, struct osal_coat_dev, media_dev);
	if (coat_dev->osal_dev.osal_pmops &&
	    coat_dev->osal_dev.osal_pmops->pm_poweroff_noirq) {
		return coat_dev->osal_dev.osal_pmops->pm_poweroff_noirq(
			&(coat_dev->osal_dev));
	}
	return 0;
}

static int osal_pm_restore_noirq(struct media_device *media)
{
	osal_coat_dev_t *coat_dev =
		container_of(media, struct osal_coat_dev, media_dev);
	if (coat_dev->osal_dev.osal_pmops &&
	    coat_dev->osal_dev.osal_pmops->pm_restore_noirq) {
		return coat_dev->osal_dev.osal_pmops->pm_restore_noirq(
			&(coat_dev->osal_dev));
	}
	return 0;
}

static struct media_ops s_osal_pmops = {
	.pm_prepare = osal_pm_prepare,
	.pm_complete = osal_pm_complete,
	.pm_suspend = osal_pm_suspend,
	.pm_resume = osal_pm_resume,
	.pm_freeze = osal_pm_freeze,
	.pm_thaw = osal_pm_thaw,
	.pm_poweroff = osal_pm_poweroff,
	.pm_restore = osal_pm_restore,
	.pm_suspend_late = osal_pm_suspend_late,
	.pm_resume_early = osal_pm_resume_early,
	.pm_freeze_late = osal_pm_freeze_late,
	.pm_thaw_early = osal_pm_thaw_early,
	.pm_poweroff_late = osal_pm_poweroff_late,
	.pm_restore_early = osal_pm_restore_early,
	.pm_suspend_noirq = osal_pm_suspend_noirq,
	.pm_resume_noirq = osal_pm_resume_noirq,
	.pm_freeze_noirq = osal_pm_freeze_noirq,
	.pm_thaw_noirq = osal_pm_thaw_noirq,
	.pm_poweroff_noirq = osal_pm_poweroff_noirq,
	.pm_restore_noirq = osal_pm_restore_noirq,
};

osal_dev_t *osal_createdev(const char *name)
{
	osal_coat_dev_t *pdev = NULL;
	if (name == NULL) {
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return NULL;
	}
	pdev = (osal_coat_dev_t *)kmalloc(sizeof(osal_coat_dev_t), GFP_KERNEL);
	if (pdev == NULL) {
		osal_printk("%s - kmalloc error!\n", __FUNCTION__);
		return NULL;
	}
	memset(pdev, 0, sizeof(osal_coat_dev_t));
	osal_strncpy(pdev->osal_dev.name, name,
		     sizeof(pdev->osal_dev.name) - 1);
	pdev->osal_dev.dev = pdev;
	return &(pdev->osal_dev);
}
EXPORT_SYMBOL(osal_createdev);

int osal_destroydev(osal_dev_t *osal_dev)
{
	osal_coat_dev_t *pdev = NULL;
	if (osal_dev == NULL) {
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
	pdev = osal_dev->dev;
	if (pdev == NULL) {
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
	kfree(pdev);
	return 0;
}
EXPORT_SYMBOL(osal_destroydev);

int osal_registerdevice(osal_dev_t *osal_dev)
{
	struct media_device *media = NULL;
	if ((osal_dev == NULL) || (osal_dev->fops == NULL)) {
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
	media = &(((osal_coat_dev_t *)(osal_dev->dev))->media_dev);
	if (osal_dev->minor != 0) {
		media->minor = osal_dev->minor;
	} else {
		media->minor = MEDIA_DYNAMIC_MINOR;
	}
	media->owner = THIS_MODULE;
	media->fops = &s_osal_fops;
	media->drvops = &s_osal_pmops;
	osal_strncpy(media->devfs_name, osal_dev->name,
		     sizeof(media->devfs_name) - 1);
	return media_register(media);
}
EXPORT_SYMBOL(osal_registerdevice);

void osal_deregisterdevice(osal_dev_t *pdev)
{
	if (pdev == NULL) {
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return;
	}
	media_unregister((struct media_device *)&(
		((osal_coat_dev_t *)(pdev->dev))->media_dev));
}
EXPORT_SYMBOL(osal_deregisterdevice);

void osal_poll_wait(osal_poll_t *table, osal_wait_t *wait)
{
	if (DRVAL_DEBUG) {
		osal_printk("%s - call poll_wait +!, table=%pK, file=%pK\n",
			    __FUNCTION__, table->poll_table, table->data);
	}

	poll_wait((struct file *)table->data, (wait_queue_head_t *)(wait->wait),
		  table->poll_table);

	if (DRVAL_DEBUG) {
		osal_printk("%s - call poll_wait -!\n", __FUNCTION__);
	}
}
EXPORT_SYMBOL(osal_poll_wait);

#if 0
void osal_pgprot_noncached(osal_vm_t *vm) {
    struct vm_area_struct *v = (struct vm_area_struct *)(vm->vm);
    v->vm_page_prot = pgprot_noncached(v->vm_page_prot);
}
#else
void osal_pgprot_noncached(osal_vm_t *vm)
{
	struct vm_area_struct *v = (struct vm_area_struct *)(vm->vm);
	v->vm_page_prot = pgprot_writecombine(v->vm_page_prot);
}

#endif
EXPORT_SYMBOL(osal_pgprot_noncached);

void osal_pgprot_cached(osal_vm_t *vm)
{
	struct vm_area_struct *v = (struct vm_area_struct *)(vm->vm);

#ifdef CONFIG_64BIT
	v->vm_page_prot = __pgprot(pgprot_val(v->vm_page_prot) | PTE_VALID |
				   PTE_DIRTY | PTE_AF);
#else

	v->vm_page_prot =
		__pgprot(pgprot_val(v->vm_page_prot) | L_PTE_PRESENT |
			 L_PTE_YOUNG | L_PTE_DIRTY | L_PTE_MT_DEV_CACHED);
#endif
}
EXPORT_SYMBOL(osal_pgprot_cached);

void osal_pgprot_writecombine(osal_vm_t *vm)
{
	struct vm_area_struct *v = (struct vm_area_struct *)(vm->vm);
	v->vm_page_prot = pgprot_writecombine(v->vm_page_prot);
}
EXPORT_SYMBOL(osal_pgprot_writecombine);

void osal_pgprot_stronglyordered(osal_vm_t *vm)
{
	struct vm_area_struct *v = (struct vm_area_struct *)(vm->vm);

#ifdef CONFIG_64BIT
	v->vm_page_prot = pgprot_device(v->vm_page_prot);
#else
	v->vm_page_prot = pgprot_stronglyordered(v->vm_page_prot);
#endif
}
EXPORT_SYMBOL(osal_pgprot_stronglyordered);

int osal_remap_pfn_range(osal_vm_t *vm, unsigned long addr, unsigned long pfn,
			 unsigned long size)
{
	struct vm_area_struct *v = (struct vm_area_struct *)(vm->vm);
	if (size == 0) {
		return -EPERM;
	}
	return remap_pfn_range(v, addr, pfn, size, v->vm_page_prot);
}
EXPORT_SYMBOL(osal_remap_pfn_range);

int osal_io_remap_pfn_range(osal_vm_t *vm, unsigned long addr,
			    unsigned long pfn, unsigned long size)
{
	struct vm_area_struct *v = (struct vm_area_struct *)(vm->vm);
	v->vm_flags |= VM_IO;
	if (size == 0) {
		return -EPERM;
	}
	return io_remap_pfn_range(v, addr, pfn, size, v->vm_page_prot);
}
EXPORT_SYMBOL(osal_io_remap_pfn_range);

#ifdef CONFIG_SNAPSHOT_BOOT
int osal_call_usermodehelper_force(char *path, char **argv, char **envp,
				   int wait)
{
	return call_usermodehelper_force(path, argv, envp, wait);
}
EXPORT_SYMBOL(osal_call_usermodehelper_force);
#endif
