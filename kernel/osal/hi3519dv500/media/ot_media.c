/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
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
#include <linux/delay.h>
#include <linux/version.h>
#include "base.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define mkstr(exp)        #exp
#define mkmarcotostr(exp) mkstr(exp)
#define VERSION_STRING    ("SDK_VERSION:[" mkmarcotostr(SDK_VERSION) "] Build Time:[" __DATE__ ", " __TIME__ "]")

/*
 * Head entry for the doubly linked media_device list
 */
static LIST_HEAD(media_list);
static DEFINE_MUTEX(media_sem);

/*
 * Assigned numbers, used for dynamic minors
 */
#define DYNAMIC_MINORS 128 /* like dynamic majors */
static unsigned char g_media_minors[DYNAMIC_MINORS / 8]; /* every 8 minors */

#ifdef CONFIG_PROC_FS
static void *media_seq_start(struct seq_file *seq, loff_t *pos)
{
    mutex_lock(&media_sem);
    return seq_list_start(&media_list, *pos);
}

static void *media_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
    return seq_list_next(v, &media_list, pos);
}

static void media_seq_stop(struct seq_file *seq, void *v)
{
    mutex_unlock(&media_sem);
}

static int media_seq_show(struct seq_file *seq, void *v)
{
    const pm_device *p = NULL;
    if (v == NULL) {
        return -1;
    }
    p = list_entry(v, pm_device, list);
    seq_printf(seq, "%3i %s\n", p->minor, (char *)p->name ? (char *)p->name : "");
    return 0;
}

static struct seq_operations g_media_seq_ops = {
    .start = media_seq_start,
    .next = media_seq_next,
    .stop = media_seq_stop,
    .show = media_seq_show,
};

static int media_seq_open(struct inode *inode, struct file *file)
{
    return seq_open(file, &g_media_seq_ops);
}
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
static struct proc_ops g_media_proc_fops = {
    .proc_open = media_seq_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = seq_release
};
#else
static struct file_operations g_media_proc_fops = {
    .owner = THIS_MODULE,
    .open = media_seq_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = seq_release,
};
#endif
#endif // #ifdef CONFIG_PROC_FS

static int media_open(struct inode *inode, struct file *file)
{
    int minor = iminor(inode);
    pm_device *c = NULL;
    int err = -1;
    const struct file_operations *old_fops = NULL;
    const struct file_operations *new_fops = NULL;
    if (file == NULL) {
        return -1;
    }
    mutex_lock(&media_sem);

    list_for_each_entry(c, &media_list, list)
    {
        if (c->minor == minor) {
            new_fops = fops_get(c->app_ops);
            break;
        }
    }

    if (new_fops == NULL) {
        goto fail;
    }

    err = 0;
    old_fops = file->f_op;
    file->f_op = new_fops;
    if (file->f_op->open) {
        file->private_data = c->dev;
        err = file->f_op->open(inode, file);
        if (err != 0) {
            fops_put(file->f_op);
            file->f_op = fops_get(old_fops);
        }
    }
    fops_put(old_fops);
fail:
    mutex_unlock(&media_sem);
    return err;
}

static struct class *g_media_class;

static struct file_operations g_media_fops = {
    .owner = THIS_MODULE,
    .open = media_open,
};

/**
 *    drv_pm_register    -    register a media device
 *    @media: device structure
 *
 *    Register a media device with the kernel. If the minor
 *    number is set to %MEDIA_DYNAMIC_MINOR a minor number is assigned
 *    and placed in the minor field of the structure. For other cases
 *    the minor number requested is used.
 *
 *    The structure passed is linked into the kernel and may not be
 *    destroyed until it has been unregistered.
 *
 *    A zero is returned on success and a negative errno code for
 *    failure.
 */
static int pm_register_get_media_minor(pm_device *media)
{
    pm_device *c = NULL;

    list_for_each_entry(c, &media_list, list) {
        if (c->minor == media->minor) {
        return -EBUSY;
        }
    }
    if (media->minor == MEDIA_DYNAMIC_MINOR) {
        int i = DYNAMIC_MINORS;
        while (--i >= 0) {
            if ((g_media_minors[(unsigned int)i >> 3] & (1 << ((unsigned int)i & 7))) == 0) { /* 3 1 7 is offset */
                break;
            }
        }
        if (i < 0) {
            return -EBUSY;
        }
        media->minor = i;
    }

    if (media->minor < DYNAMIC_MINORS) {
        g_media_minors[media->minor >> 3] |= 1 << (media->minor & 7); /* 3 1 7 is offset */
    } else {
        return -EBUSY;
    }
    return TRUE;
}

static int pm_register_add_media_device(pm_device *media, pm_basedev **bdev)
{
    int ret;

    if (bdev == NULL) {
        return FALSE;
    }

    *bdev = media_device_alloc(media->name, -1);
    if (*bdev == NULL) {
        return -ENOMEM;
    }
    ret = media_device_add(*bdev);
    if (ret != 0) {
        media_device_put(*bdev);
        return ret;
    }
    return TRUE;
}


static int pm_register_device_create(pm_device *media,
    pm_basedev *bdev, dev_t *dev, struct device **adev)
{
    *dev = MKDEV(MEDIA_DEVICE_MAJOR, media->minor);
    *adev = device_create(g_media_class, &(bdev->dev), *dev, NULL,
        "%s", media->name);
    if (IS_ERR(*adev)) {
        return PTR_ERR(*adev);
    }

    return TRUE;
}


static int pm_register_device_register(pm_device *media,
    struct device *adev, pm_basedev *bdev)
{
    pm_basedrv *bdrv = NULL;
    int ret;

    bdrv = media_driver_alloc(media->name, media->owner, media->base_ops);
    if (bdrv == NULL) {
        return -ENOMEM;
    }

    ret = media_driver_register(bdrv);
    if (ret != 0) {
        media_driver_release(bdrv);
        return ret;
    }

    media->app_device = adev;
    media->base_device = bdev;
    media->base_driver = bdrv;

    return TRUE;
}
int drv_pm_register(pm_device *media)
{
    int ret;
    dev_t dev;
    struct device *adev = NULL;
    pm_basedev *bdev = NULL;

    if ((media == NULL) || (media->name == NULL) || (media->app_ops == NULL)) {
        return -EINVAL;
    }

#ifdef MODULE
    if (media->owner == NULL) {
        return -EINVAL;
    }
#endif
    mutex_lock(&media_sem);
    // 1
    ret = pm_register_get_media_minor(media);
    if (ret != TRUE) {
        goto out;
    }

    // 2 base device, then class = NULL;
    ret = pm_register_add_media_device(media, &bdev);
    if (ret != TRUE) {
        goto err0;
    }

    // 3 app class
    ret = pm_register_device_create(media, bdev, &dev, &adev);
    if (ret != TRUE) {
        goto err1;
    }

    // 4 base driver
    ret = pm_register_device_register(media, adev, bdev);
    if (ret != TRUE) {
        goto err2;
    }
    /*
     * Add it to the front, so that later devices can "override"
     * earlier defaults
     */
    INIT_LIST_HEAD(&media->list);
    list_add(&media->list, &media_list);
    goto out;

err2:
    device_destroy(g_media_class, dev);
err1:
    media_device_unregister(bdev);
err0:
    g_media_minors[media->minor >> 3] &= ~(1 << (media->minor & 7)); /* 3 1 7 is offset */
out:
    mutex_unlock(&media_sem);
    return ret;
}

/**
 *    drv_pm_un_register - unregister a media device
 *    @media: device to unregister
 *
 *    Unregister a media device that was previously
 *    successfully registered with drv_pm_register(). Success
 *    is indicated by a zero return, a negative errno code
 *    indicates an error.
 */
int drv_pm_un_register(pm_device *media)
{
    unsigned int i;

    if (g_media_class == NULL || media == NULL || (media->name == NULL) || (media->app_ops == NULL)) {
        return -EINVAL;
    }
#ifdef MODULE
    if (media->owner == NULL) {
        return -EINVAL;
    }
#endif
    i = media->minor;
    // 0
    if (list_empty(&media->list)) {
        return -EINVAL;
    }
    if (i >= DYNAMIC_MINORS) {
        return -EINVAL;
    }
    // 1
    mutex_lock(&media_sem);
    // 1.0
    list_del(&media->list);
    // 1.1
    if (media->base_driver) {
        media_driver_unregister(media->base_driver);
        media_driver_release(media->base_driver);
        media->base_driver = NULL;
    }
    // 1.2
    if (media->app_device) {
        device_destroy(g_media_class, MKDEV(MEDIA_DEVICE_MAJOR, media->minor));
        media->app_device = NULL;
    }
    // 1.3
    if (media->base_device) {
        media_device_unregister(media->base_device);
        media->base_device = NULL;
    }
    // 1.4
    g_media_minors[i >> 3] &= ~(1 << (i & 7)); /* 3 1 7 is offset */
    mutex_unlock(&media_sem);
    return 0;
}

int drv_pm_mod_init(void)
{
    int ret;
    // 0
    proc_create("media", 0, NULL, &g_media_proc_fops);
    // 1
    ret = media_bus_init();
    if (ret != 0) {
        goto err0;
    }
    // 2
    g_media_class = class_create(THIS_MODULE, "media_class");
    ret = PTR_ERR(g_media_class);
    if (IS_ERR(g_media_class)) {
        goto err1;
    }
    // 3
    ret = -EIO;
    if (register_chrdev(MEDIA_DEVICE_MAJOR, "media_char_dev", &g_media_fops)) {
        goto err2;
    }
    return 0;
    // 4
err2:

    printk("!!! Module media: unable to get major %d for media devices\n", MEDIA_DEVICE_MAJOR);

    class_destroy(g_media_class);
err1:
    media_bus_exit();
err0:
    remove_proc_entry("media", NULL);
    return ret;
}

void drv_pm_mod_exit(void)
{
    // 0
    if (list_empty(&media_list) == 0) {
        return;
    }
    // 1
    unregister_chrdev(MEDIA_DEVICE_MAJOR, "media");
    // 2
    class_destroy(g_media_class);
    // 3
    media_bus_exit();
    // 4
    remove_proc_entry("media", NULL);
#if defined(MODULE)
    printk("ko remove success.\n");
#endif
    return;
}

EXPORT_SYMBOL(drv_pm_register);
EXPORT_SYMBOL(drv_pm_un_register);
EXPORT_SYMBOL(drv_pm_mod_init);
EXPORT_SYMBOL(drv_pm_mod_exit);

MODULE_LICENSE("GPL");
