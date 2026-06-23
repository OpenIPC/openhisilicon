/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/dma-mapping.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/version.h>
#include "securec.h"
#include "base.h"

#ifndef NULL
#define NULL ((void *) 0)
#endif

/* ******************media bus**************** */
static void media_bus_release(struct device *dev)
{
    (void)dev;
    return;
}

struct device g_media_bus = {
    .init_name = "media_bus_dev",
    .release = media_bus_release
};
/* top level bus, parent and bus member are both NULL */ /* CNcomment: this is top bus,parent and bus member is NULL */
static ssize_t modalias_show(struct device *dev, struct device_attribute *a,
                             char *buf)
{
    pm_basedev *pdev = to_pm_basedev(dev);
    int len = snprintf_s(buf, PAGE_SIZE, (PAGE_SIZE - 1), "media:%s\n", (char *)pdev->name);
    if (len < 0) {
        printk("modalias_show call snprintf_s failed!\n");
    }

    return (len >= PAGE_SIZE) ? (PAGE_SIZE - 1) : len;
}


#if defined (LINUX_VERSION_CODE) && (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
static DEVICE_ATTR_RO(modalias);

static struct attribute *g_media_dev_attrs[] = {
    &dev_attr_modalias.attr,
    NULL,
};

ATTRIBUTE_GROUPS(g_media_dev);
#else
static struct device_attribute g_media_dev_attrs[] = {
    __ATTR_RO(modalias),
    __ATTR_NULL,
};
#endif

static int media_match(struct device *dev, struct device_driver *drv)
{
    pm_basedev *pdev = NULL;
    if (dev == NULL || drv == NULL) {
        return -1;
    }
    pdev = to_pm_basedev(dev);
    return (strncmp(pdev->name, drv->name, MEDIA_DEVICE_NAME_MAX_LEN + 8) == 0); /* 8 used calc len(drv->name) */
}

static int media_uevent(struct device *dev, struct kobj_uevent_env *env)
{
    int ret;
    pm_basedev *pdev = NULL;
    if (dev == NULL || env == NULL) {
        return -1;
    }
    pdev = to_pm_basedev(dev);
    ret = add_uevent_var(env, "MODALIAS=media:%s", pdev->name);
    if (ret != 0) {
        return ret;
    }
    return 0;
}

static int media_legacy_suspend(struct device *dev, pm_message_t mesg)
{
    int ret = 0;

    if (dev->driver != NULL && dev->driver->suspend != NULL) {
        ret = dev->driver->suspend(dev, mesg);
    }

    return ret;
}

static int media_legacy_suspend_late(struct device *dev, pm_message_t mesg)
{
    int ret = 0;
    pm_basedev *pdev = to_pm_basedev(dev);
    pm_basedrv *pdrv = to_media_basedrv(dev->driver);

    if (dev->driver != NULL && pdrv->suspend_late != NULL) {
        ret = pdrv->suspend_late(pdev, mesg);
    }

    return ret;
}

static int media_legacy_resume_early(struct device *dev)
{
    int ret = 0;
    pm_basedev *pdev = to_pm_basedev(dev);
    pm_basedrv *pdrv = to_media_basedrv(dev->driver);

    if (dev->driver != NULL && pdrv->resume_early != NULL) {
        ret = pdrv->resume_early(pdev);
    }

    return ret;
}

static int media_legacy_resume(struct device *dev)
{
    int ret = 0;

    if (dev->driver != NULL && dev->driver->resume != NULL) {
        ret = dev->driver->resume(dev);
    }

    return ret;
}

static int media_pm_prepare(struct device *dev)
{
    struct device_driver *drv = dev->driver;
    int ret = 0;

    if (drv != NULL && drv->pm != NULL && drv->pm->prepare != NULL) {
        ret = drv->pm->prepare(dev);
    }

    return ret;
}

static void media_pm_complete(struct device *dev)
{
    struct device_driver *drv = NULL;
    if (dev == NULL) {
        return;
    }
    drv = dev->driver;

    if (drv != NULL && drv->pm != NULL && drv->pm->complete != NULL) {
        drv->pm->complete(dev);
    }
}

static int media_pm_suspend(struct device *dev)
{
    int ret = 0;
    struct device_driver *drv = NULL;
    if (dev == NULL || dev->driver == NULL) {
        return -1;
    }
    drv = dev->driver;
    if (drv->pm) {
        if (drv->pm->suspend) {
            ret = drv->pm->suspend(dev);
        }
    } else {
        ret = media_legacy_suspend(dev, PMSG_SUSPEND);
    }

    return ret;
}

static int media_pm_suspend_noirq(struct device *dev)
{
    int ret = 0;
    struct device_driver *drv = NULL;
    if (dev == NULL || dev->driver == NULL) {
        return -1;
    }
    drv = dev->driver;
    if (drv->pm != NULL) {
        if (drv->pm->suspend_noirq != NULL) {
            ret = drv->pm->suspend_noirq(dev);
        }
    } else {
        ret = media_legacy_suspend_late(dev, PMSG_SUSPEND);
    }

    return ret;
}

static int media_pm_resume(struct device *dev)
{
    int ret = 0;
    struct device_driver *drv = NULL;
    if (dev == NULL || dev->driver == NULL) {
        return -1;
    }
    drv = dev->driver;
    if (drv->pm != NULL) {
        if (drv->pm->resume != NULL) {
            ret = drv->pm->resume(dev);
        }
    } else {
        ret = media_legacy_resume(dev);
    }

    return ret;
}

static int media_pm_resume_early(struct device *dev)
{
    int ret = 0;
    struct device_driver *drv = NULL;
    if (dev == NULL || dev->driver == NULL) {
        return -1;
    }
    drv = dev->driver;
    if (drv->pm != NULL) {
        if (drv->pm->resume_early != NULL) {
            ret = drv->pm->resume_early(dev);
        }
    } else {
        ret = media_legacy_resume_early(dev);
    }

    return ret;
}

static int media_pm_resume_noirq(struct device *dev)
{
    int ret = 0;
    struct device_driver *drv = NULL;
    if (dev == NULL || dev->driver == NULL) {
        return -1;
    }
    drv = dev->driver;
    if (drv->pm != NULL) {
        if (drv->pm->resume_noirq != NULL) {
            ret = drv->pm->resume_noirq(dev);
        }
    }

    return ret;
}

static struct dev_pm_ops g_media_dev_pm_ops = {
    .prepare = media_pm_prepare,
    .complete = media_pm_complete,
    .suspend = media_pm_suspend,
    .resume = media_pm_resume,
    .freeze = NULL,
    .thaw = NULL,
    .poweroff = NULL,
    .restore = NULL,
    .suspend_noirq = media_pm_suspend_noirq,
    .resume_noirq = media_pm_resume_noirq,
    .freeze_noirq = NULL,
    .thaw_noirq = NULL,
    .poweroff_noirq = NULL,
    .restore_noirq = NULL,
    .resume_early = media_pm_resume_early,
};

#define MEDIA_PM_OPS_PTR (&g_media_dev_pm_ops)

struct bus_type g_media_bus_type = {
    .name = "media_bus",
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
    .dev_groups = g_media_dev_groups,
#else
    .dev_attrs = g_media_dev_attrs,
#endif
    .match = media_match,
    .uevent = media_uevent,
    .pm = MEDIA_PM_OPS_PTR,
};

int media_bus_init(void)
{
    int ret;
    ret = device_register(&g_media_bus);
    if (ret != 0) {
        return ret;
    }
    ret = bus_register(&g_media_bus_type);
    if (ret != 0) {
        device_unregister(&g_media_bus);
    }
    return ret;
}

void media_bus_exit(void)
{
    bus_unregister(&g_media_bus_type);
    device_unregister(&g_media_bus);
    return;
}

/* ************************media  base  device****************************** */
int media_device_add(pm_basedev *pdev)
{
    // 0
    if (pdev == NULL) {
        return -EINVAL;
    }
    // 1
    if (pdev->dev.parent == NULL) {
        pdev->dev.parent = &g_media_bus;
    }
    pdev->dev.bus = &g_media_bus_type;

    if (pdev->id != -1) {
        dev_set_name(&pdev->dev, "%s.%d", pdev->name, pdev->id);
    } else {
        dev_set_name(&pdev->dev, "%s", pdev->name);
    }

    return device_add(&pdev->dev);
}

void media_device_del(pm_basedev *pdev)
{
    if (pdev != NULL) {
        device_del(&pdev->dev);
    }
    return;
}

void media_device_put(pm_basedev *pdev)
{
    if (pdev != NULL) {
        put_device(&pdev->dev);
    }
}

struct media_devobj {
    pm_basedev pdev;
    char name[1];
};

static void media_device_release(struct device *dev)
{
    struct media_devobj *pa = container_of(dev, struct media_devobj, pdev.dev);
    kfree(pa);
    return;
}

pm_basedev *media_device_alloc(const char *name, int id)
{
    int size;
    struct media_devobj *pa = NULL;
    if (name == NULL) {
        printk("name is NULL.\n");
        return NULL;
    }
    size = (int)strlen(name);
    pa = kmalloc(sizeof(struct media_devobj) + size, GFP_KERNEL | __GFP_ZERO);
    if (pa != NULL) {
        (void)memset_s(pa, sizeof(struct media_devobj) + size, 0, sizeof(struct media_devobj) + size);
        if (snprintf_s(pa->name, sizeof(struct media_devobj) + size - sizeof(pa->pdev), size, "%s", name) != size) {
            printk("call snprintf_s func error.\n");
        }
        pa->pdev.name = pa->name;
        pa->pdev.id = id;
        device_initialize(&pa->pdev.dev);
        pa->pdev.dev.release = media_device_release;
    }
    return pa ? &pa->pdev : NULL;
}

int media_device_register(pm_basedev *pdev)
{
    if (pdev == NULL) {
        return -1;
    }
    device_initialize(&pdev->dev);
    return media_device_add(pdev);
}

void media_device_unregister(pm_basedev *pdev)
{
    media_device_del(pdev);
    media_device_put(pdev);
    return;
}

/* ***********************media  base  driver************************* */
static int media_drv_probe(struct device *dev)
{
    pm_basedrv *pdrv = NULL;
    pm_basedev *pdev = NULL;
    if (dev == NULL) {
        return -1;
    }
    pdrv = to_media_basedrv(dev->driver);
    pdev = to_pm_basedev(dev);
    return pdrv->probe(pdev);
}

static int media_drv_remove(struct device *dev)
{
    pm_basedrv *pdrv = NULL;
    pm_basedev *pdev = NULL;
    if (dev == NULL) {
        return -1;
    }
    pdrv = to_media_basedrv(dev->driver);
    pdev = to_pm_basedev(dev);

    return pdrv->remove(pdev);
}

static void media_drv_shutdown(struct device *dev)
{
    pm_basedrv *pdrv = NULL;
    pm_basedev *pdev = NULL;
    if (dev == NULL) {
        return;
    }
    pdrv = to_media_basedrv(dev->driver);
    pdev = to_pm_basedev(dev);
    pdrv->shutdown(pdev);
    return;
}

static int media_drv_suspend(struct device *dev, pm_message_t state)
{
    pm_basedrv *pdrv = NULL;
    pm_basedev *pdev = NULL;
    if (dev == NULL) {
        return -1;
    }
    pdrv = to_media_basedrv(dev->driver);
    pdev = to_pm_basedev(dev);

    return pdrv->suspend(pdev, state);
}

static int media_drv_resume(struct device *dev)
{
    pm_basedrv *pdrv = NULL;
    pm_basedev *pdev = NULL;
    if (dev == NULL) {
        return -1;
    }
    pdrv = to_media_basedrv(dev->driver);
    pdev = to_pm_basedev(dev);

    return pdrv->resume(pdev);
}

int media_driver_register(pm_basedrv *drv)
{
    drv->driver.bus = &g_media_bus_type;
    if (drv->probe) {
        drv->driver.probe = media_drv_probe;
    }
    if (drv->remove) {
        drv->driver.remove = media_drv_remove;
    }
    if (drv->shutdown) {
        drv->driver.shutdown = media_drv_shutdown;
    }
    if (drv->suspend) {
        drv->driver.suspend = media_drv_suspend;
    }
    if (drv->resume) {
        drv->driver.resume = media_drv_resume;
    }
    return driver_register(&drv->driver);
}

void media_driver_unregister(pm_basedrv *drv)
{
    driver_unregister(&drv->driver);
}

struct media_drvobj {
    pm_basedrv pdrv;
    char name[1];
};

pm_basedrv *media_driver_alloc(const char *name, struct module *owner, const pm_baseops *ops)
{
    int size;
    int ret;
    struct media_drvobj *pa = NULL;
    if (name == NULL || ops == NULL) {
        return NULL;
    }
#ifdef MODULE
    if (owner == NULL) {
        return NULL;
    }
#endif
    size = (int)strlen(name);
    pa = kmalloc(sizeof(struct media_drvobj) + size, GFP_KERNEL | __GFP_ZERO);
    if (pa == NULL) {
        return NULL;
    }
    (void)memset_s(pa, sizeof(struct media_drvobj) + size, 0, sizeof(struct media_drvobj) + size);
    ret = snprintf_s(pa->name, sizeof(struct media_devobj) + size - sizeof(pa->pdrv), size, "%s", name);
    if (ret != size) {
        printk("call snprintf_s func error.\n");
        kfree(pa);
        pa = NULL;
        return NULL;
    }

    pa->pdrv.driver.name = pa->name;
    pa->pdrv.driver.owner = owner;
    if (ops == NULL) {
        pa->pdrv.probe = NULL;
        pa->pdrv.remove = NULL;
        pa->pdrv.shutdown = NULL;
        pa->pdrv.suspend = NULL;
        pa->pdrv.resume = NULL;
        pa->pdrv.suspend_late = NULL;
        pa->pdrv.resume_early = NULL;
    } else {
        pa->pdrv.probe = ops->probe;
        pa->pdrv.remove = ops->remove;
        pa->pdrv.shutdown = ops->shutdown;
        pa->pdrv.suspend = ops->suspend;
        pa->pdrv.resume = ops->resume;
        pa->pdrv.suspend_late = ops->suspend_late;
        pa->pdrv.resume_early = ops->resume_early;
    }

    return &pa->pdrv;
}

void media_driver_release(pm_basedrv *drv)
{
    struct media_drvobj *pa = NULL;
    if (drv == NULL) {
        return;
    }
    pa = container_of(drv, struct media_drvobj, pdrv);
    kfree(pa);
    return;
}

