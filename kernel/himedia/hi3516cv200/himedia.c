/*
 * linux/drivers/char/himedia.c
 *
 * HISILICON driver base frameforwk
 * Changed for hisilicon-media devices by Liu Jiandong 10-Dec-2007
 */

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/platform_device.h>
#include <linux/sched.h>

#include "base.h"

static LIST_HEAD(himedia_list);
static DEFINE_MUTEX(himedia_sem);

/*
 * Assigned numbers, used for dynamic minors
 */
#define DYNAMIC_MINORS 64 /* like dynamic majors */
static unsigned char himedia_minors[DYNAMIC_MINORS / 8];

static int himedia_open(struct inode * inode, struct file * file)
{
	int minor = iminor(inode);
	struct himedia_device *c;
	int err = -ENODEV;
	const struct file_operations *old_fops, *new_fops = NULL;

	mutex_lock(&himedia_sem);

	list_for_each_entry(c, &himedia_list, list) {
		if (c->minor == minor) {
			new_fops = fops_get(c->fops);
			break;
		}
	}

	if (!new_fops) {
		mutex_unlock(&himedia_sem);
		request_module("char-major-%d-%d", HIMEDIA_DEVICE_MAJOR, minor);
		mutex_lock(&himedia_sem);

		list_for_each_entry(c, &himedia_list, list) {
			if (c->minor == minor) {
				new_fops = fops_get(c->fops);
				break;
			}
		}

		if (!new_fops)
			goto fail;
	}

	err = 0;

	old_fops = file->f_op;
	file->f_op = new_fops;
	if (file->f_op->open) {
		err=file->f_op->open(inode,file);
		if (err) {
			fops_put(file->f_op);
			file->f_op = fops_get(old_fops);
		}
	}

	fops_put(old_fops);
fail:
	mutex_unlock(&himedia_sem);
	return err;
}


static struct file_operations himedia_fops = {
	.owner		= THIS_MODULE,
	.open		= himedia_open,
};

/**
 *	himedia_register - register a himedia device
 *	@himedia: device structure
 *
 *	Register a himedia device with the kernel. If the minor
 *	number is set to %HIMEDIA_DYNAMIC_MINOR a minor number is assigned
 *	and placed in the minor field of the structure. For other cases
 *	the minor number requested is used.
 *
 *	The structure passed is linked into the kernel and may not be
 *	destroyed until it has been unregistered.
 *
 *	A zero is returned on success and a negative errno code for
 *	failure.
 */

int himedia_register(struct himedia_device *himedia)
{
	struct himedia_device  *ptmp;
    struct himedia_driver  *pdrv;

	int err = 0;

	mutex_lock(&himedia_sem);

	/*check if registered*/
	list_for_each_entry(ptmp, &himedia_list, list) {
		if (ptmp->minor == himedia->minor) {
			mutex_unlock(&himedia_sem);
			return -EBUSY;
		}
	}

    /*check minor*/
	if (himedia->minor == HIMEDIA_DYNAMIC_MINOR) {
		int i = DYNAMIC_MINORS;

		while (--i >= 0)
			if ( (himedia_minors[i>>3] & (1 << (i&7))) == 0)
				break;
		if (i<0) {
			mutex_unlock(&himedia_sem);
			return -EBUSY;
		}

		himedia->minor = i;
	}

	if (himedia->minor < DYNAMIC_MINORS)
		himedia_minors[himedia->minor >> 3] |= 1 << (himedia->minor & 7);

    /*device register*/
    err = himedia_device_register(himedia);
    if (err < 0) {
        himedia_minors[himedia->minor >> 3] &= ~(1 << (himedia->minor & 7));
		goto out;
    }


    /*driver register*/
	pdrv = himedia_driver_register(
	    himedia->devfs_name, himedia->owner, himedia->drvops);
	if (IS_ERR(pdrv)) {

        himedia_device_unregister(himedia);

		himedia_minors[himedia->minor >> 3] &= ~(1 << (himedia->minor & 7));

		err = PTR_ERR(pdrv);
		goto out;
	}

	himedia->driver = pdrv;

	/*
	 * Add it to the front, so that later devices can "override"
	 * earlier defaults
	 */
	list_add(&himedia->list, &himedia_list);

 out:
	mutex_unlock(&himedia_sem);
	return err;
}
EXPORT_SYMBOL(himedia_register);

/**
 *	himedia_unregister - unregister a himedia device
 *	@himedia: device to unregister
 *
 *	Unregister a himedia device that was previously
 *	successfully registered with himedia_register(). Success
 *	is indicated by a zero return, a negative errno code
 *	indicates an error.
 */

int himedia_unregister(struct himedia_device * himedia)
{
	struct himedia_device  *ptmp,*_ptmp;

	if (list_empty(&himedia->list))
		return -EINVAL;

	mutex_lock(&himedia_sem);

	list_for_each_entry_safe(ptmp, _ptmp, &himedia_list, list) {

	    /*if found, unregister device & driver*/
		if (ptmp->minor == himedia->minor) {

            list_del(&himedia->list);

        	himedia_driver_unregister(himedia->driver);

        	himedia->driver = NULL;

            himedia_device_unregister(himedia);

        	himedia_minors[himedia->minor>>3] &= ~(1 << (himedia->minor & 7));

        	break;
		}
	}

	mutex_unlock(&himedia_sem);

	return 0;
}
EXPORT_SYMBOL(himedia_unregister);

unsigned long long hi_sched_clock(void)
{
    return sched_clock();
}
EXPORT_SYMBOL(hi_sched_clock);

int himedia_platform_driver_register(void *drv)
{
    return __platform_driver_register((struct platform_driver *)drv, THIS_MODULE);
}
EXPORT_SYMBOL(himedia_platform_driver_register);

void himedia_platform_driver_unregister(void *drv)
{
    return platform_driver_unregister((struct platform_driver *)drv);
}
EXPORT_SYMBOL(himedia_platform_driver_unregister);

void* himedia_platform_get_resource_byname(void *dev, unsigned int type,
    const char *name)
{
    return (void*)platform_get_resource_byname((struct platform_device *)dev, type, name);
}
EXPORT_SYMBOL(himedia_platform_get_resource_byname);

void* himedia_platform_get_resource(void *dev, unsigned int type, unsigned int num)
{
    return (void*)platform_get_resource((struct platform_device *)dev, type, num);
}
EXPORT_SYMBOL(himedia_platform_get_resource);

int himedia_platform_get_irq(void *dev, unsigned int num)
{
    return platform_get_irq((struct platform_device *)dev, num);
}
EXPORT_SYMBOL(himedia_platform_get_irq);

int himedia_platform_get_irq_byname(void *dev, const char *name)
{
    return platform_get_irq_byname((struct platform_device *)dev, name);
}
EXPORT_SYMBOL(himedia_platform_get_irq_byname);


static int __init himedia_init(void)
{
	int ret;
	// 1
	ret = himedia_bus_init();
	if(ret)
		goto err0;

	// 2
	ret = -EIO;

	if (register_chrdev(HIMEDIA_DEVICE_MAJOR, "himedia", &himedia_fops))
		goto err1;

	printk("Module himedia: init ok\n");

	return 0;

	// 3
err1:
	himedia_bus_exit();
err0:
	return ret;
}


static void __exit himedia_exit(void)
{
	// 0
	if (!list_empty(&himedia_list))
	{
		printk("!!! Module himedia: sub module in list\n");
		return;
	}

	unregister_chrdev(HIMEDIA_DEVICE_MAJOR, "himedia");

	himedia_bus_exit();

	printk("!!! Module himedia: exit ok\n");
}

module_init(himedia_init);
module_exit(himedia_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hisilicon");

