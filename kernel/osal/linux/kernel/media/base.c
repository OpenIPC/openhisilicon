/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/dma-mapping.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/kmod.h>

#include "base.h"

/*****************************************************************************/
/**** media bus  ****/
/*****************************************************************************/

static void media_bus_release(struct device *dev)
{
    return;
}

struct device media_bus = {
    .init_name = "media",
    .release = media_bus_release
};

/* sysfs modalias store/show */
static ssize_t modalias_show(struct device *dev, struct device_attribute *a,
                             char *buf)
{
    struct media_device *pdev = to_media_device(dev);
    int len = snprintf(buf, PAGE_SIZE, "media:%s\n", pdev->devfs_name);

    return (len >= PAGE_SIZE) ? (PAGE_SIZE - 1) : len;
}

static struct device_attribute media_dev_attrs[] = {
    __ATTR_RO(modalias),
    __ATTR_NULL,
};

/* bus match & uevent */
static int media_match(struct device *dev, struct device_driver *drv)
{
    struct media_device *pdev = to_media_device(dev);
    return (strncmp(pdev->devfs_name, drv->name, sizeof(pdev->devfs_name)) == 0);
}

static int media_uevent(struct device *dev, struct kobj_uevent_env *env)
{
    struct media_device *pdev = to_media_device(dev);
    add_uevent_var(env, "MODALIAS=media:%s", pdev->devfs_name);
    return 0;
}

/*****************************************************************************/
// pm methods
static int media_pm_prepare(struct device *dev)
{
    struct media_device *pdev = to_media_device(dev);
    struct media_driver *pdrv = to_media_driver(dev->driver);

    if ((pdrv->ops == NULL) || (pdrv->ops->pm_prepare == NULL)) {
        return 0;
    }

    return pdrv->ops->pm_prepare(pdev);
}

static void media_pm_complete(struct device *dev)
{
    struct media_device *pdev = to_media_device(dev);
    struct media_driver *pdrv = to_media_driver(dev->driver);

    if ((pdrv->ops == NULL) || (pdrv->ops->pm_complete == NULL)) {
        return;
    }

    pdrv->ops->pm_complete(pdev);
}

static int media_pm_suspend(struct device *dev)
{
    struct media_device *pdev = to_media_device(dev);
    struct media_driver *pdrv = to_media_driver(dev->driver);

    if ((pdrv->ops == NULL) || (pdrv->ops->pm_suspend == NULL)) {
        return 0;
    }

    return pdrv->ops->pm_suspend(pdev);
}

static int media_pm_resume(struct device *dev)
{
    struct media_device *pdev = to_media_device(dev);
    struct media_driver *pdrv = to_media_driver(dev->driver);

    if ((pdrv->ops == NULL) || (pdrv->ops->pm_resume == NULL)) {
        return 0;
    }

    return pdrv->ops->pm_resume(pdev);
}

static int media_pm_freeze(struct device *dev)
{
    struct media_device *pdev = to_media_device(dev);
    struct media_driver *pdrv = to_media_driver(dev->driver);

    if ((pdrv->ops == NULL) || (pdrv->ops->pm_freeze == NULL)) {
        return 0;
    }

    return pdrv->ops->pm_freeze(pdev);
}

static int media_pm_thaw(struct device *dev)
{
    struct media_device *pdev = to_media_device(dev);
    struct media_driver *pdrv = to_media_driver(dev->driver);

    if ((pdrv->ops == NULL) || (pdrv->ops->pm_thaw == NULL)) {
        return 0;
    }

    return pdrv->ops->pm_thaw(pdev);
}

static int media_pm_poweroff(struct device *dev)
{
    struct media_device *pdev = to_media_device(dev);
    struct media_driver *pdrv = to_media_driver(dev->driver);

    if ((pdrv->ops == NULL) || (pdrv->ops->pm_poweroff == NULL)) {
        return 0;
    }

    return pdrv->ops->pm_poweroff(pdev);
}

static int media_pm_restore(struct device *dev)
{
    struct media_device *pdev = to_media_device(dev);
    struct media_driver *pdrv = to_media_driver(dev->driver);

    if ((pdrv->ops == NULL) || (pdrv->ops->pm_restore == NULL)) {
        return 0;
    }

    return pdrv->ops->pm_restore(pdev);
}
static int media_pm_suspend_noirq(struct device *dev)
{
    struct media_device *pdev = to_media_device(dev);
    struct media_driver *pdrv = to_media_driver(dev->driver);

    if ((pdrv->ops == NULL) || (pdrv->ops->pm_suspend_noirq == NULL)) {
        return 0;
    }

    return pdrv->ops->pm_suspend_noirq(pdev);
}

static int media_pm_resume_noirq(struct device *dev)
{
    struct media_device *pdev = to_media_device(dev);
    struct media_driver *pdrv = to_media_driver(dev->driver);

    if ((pdrv->ops == NULL) || (pdrv->ops->pm_resume_noirq == NULL)) {
        return 0;
    }

    return pdrv->ops->pm_resume_noirq(pdev);
}

static int media_pm_freeze_noirq(struct device *dev)
{
    struct media_device *pdev = to_media_device(dev);
    struct media_driver *pdrv = to_media_driver(dev->driver);

    if ((pdrv->ops == NULL) || (pdrv->ops->pm_freeze_noirq == NULL)) {
        return 0;
    }

    return pdrv->ops->pm_freeze_noirq(pdev);
}

static int media_pm_thaw_noirq(struct device *dev)
{
    struct media_device *pdev = to_media_device(dev);
    struct media_driver *pdrv = to_media_driver(dev->driver);

    if ((pdrv->ops == NULL) || (pdrv->ops->pm_thaw_noirq == NULL)) {
        return 0;
    }

    return pdrv->ops->pm_thaw_noirq(pdev);
}

static int media_pm_poweroff_noirq(struct device *dev)
{
    struct media_device *pdev = to_media_device(dev);
    struct media_driver *pdrv = to_media_driver(dev->driver);

    if ((pdrv->ops == NULL) || (pdrv->ops->pm_poweroff_noirq == NULL)) {
        return 0;
    }

    return pdrv->ops->pm_poweroff_noirq(pdev);
}

static int media_pm_restore_noirq(struct device *dev)
{
    struct media_device *pdev = to_media_device(dev);
    struct media_driver *pdrv = to_media_driver(dev->driver);

    if ((pdrv->ops == NULL) || (pdrv->ops->pm_restore_noirq == NULL)) {
        return 0;
    }

    return pdrv->ops->pm_restore_noirq(pdev);
}

static struct dev_pm_ops media_bus_pm_ops = {
    .prepare = media_pm_prepare,
    .complete = media_pm_complete,

    // with irq
    .suspend = media_pm_suspend,
    .resume = media_pm_resume,

    .freeze = media_pm_freeze,
    .thaw = media_pm_thaw,
    .poweroff = media_pm_poweroff,
    .restore = media_pm_restore,

    // with noirq
    .suspend_noirq = media_pm_suspend_noirq,
    .resume_noirq = media_pm_resume_noirq,
    .freeze_noirq = media_pm_freeze_noirq,
    .thaw_noirq = media_pm_thaw_noirq,
    .poweroff_noirq = media_pm_poweroff_noirq,
    .restore_noirq = media_pm_restore_noirq,
};

struct bus_type media_bus_type = {
    .name = "media",
    .dev_attrs = media_dev_attrs,
    .match = media_match,
    .uevent = media_uevent,
    .pm = &media_bus_pm_ops,
};

int media_bus_init(void)
{
    int ret;
    ret = device_register(&media_bus);
    if (ret) {
        return ret;
    }

    ret = bus_register(&media_bus_type);
    if (ret) {
        goto error;
    }

    return 0;
error:

    device_unregister(&media_bus);
    return ret;
}

void media_bus_exit(void)
{
    bus_unregister(&media_bus_type);
    device_unregister(&media_bus);
}

static void media_device_release(struct device *dev)
{
    return;
}

int media_device_register(struct media_device *pdev)
{
    dev_set_name(&pdev->device, "%s", pdev->devfs_name);

    pdev->device.devt = MKDEV(MEDIA_DEVICE_MAJOR, pdev->minor);
    pdev->device.release = media_device_release;
    pdev->device.bus = &media_bus_type;

    return device_register(&pdev->device);
}

void media_device_unregister(struct media_device *pdev)
{
    device_unregister(&pdev->device);
}

struct media_driver *media_driver_register(const char *name,
                                               struct module *owner, struct media_ops *ops)
{
    int ret;
    struct media_driver *pdrv = NULL;

    if ((name == NULL) || (owner == NULL)) {
        return ERR_PTR(-EINVAL);
    }

    pdrv = kzalloc(sizeof(struct media_driver) + strnlen(name, MEDIA_MAX_DEV_NAME_LEN), GFP_KERNEL);
    if (pdrv == NULL) {
        return ERR_PTR(-ENOMEM);
    }

    /* init driver object */
    strncpy(pdrv->name, name, strnlen(name, MEDIA_MAX_DEV_NAME_LEN));

    pdrv->ops = ops;

    pdrv->driver.name = pdrv->name;
    pdrv->driver.owner = owner;
    pdrv->driver.bus = &media_bus_type;

    ret = driver_register(&pdrv->driver);
    if (ret) {
        kfree(pdrv);
        return ERR_PTR(ret);
    }

    return pdrv;
}

void media_driver_unregister(struct media_driver *pdrv)
{
    if (pdrv != NULL) {
        driver_unregister(&pdrv->driver);
        kfree(pdrv);
    }
}

// end!
/*****************************************************************************/

