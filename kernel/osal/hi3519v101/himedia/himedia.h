/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2016-2019.
 * Description: osal himedia header file.
 * Author: Hisilicon multimedia software group
 * Create: 2016-11-11
 */

#ifndef _LINUX_HIMEDIA_DEVICE_H_
#define _LINUX_HIMEDIA_DEVICE_H_

#include <linux/module.h>
#include <linux/major.h>
#include <linux/device.h>
#include "osal_list.h"

#define HIMEDIA_DEVICE_MAJOR     218
#define HIMEDIA_DYNAMIC_MINOR    255

struct himedia_device;

struct himedia_ops {
    /* pm methos */
    int (*pm_prepare)(struct himedia_device *);
    void (*pm_complete)(struct himedia_device *);

    int (*pm_suspend)(struct himedia_device *);
    int (*pm_resume)(struct himedia_device *);

    int (*pm_freeze)(struct himedia_device *);
    int (*pm_thaw)(struct himedia_device *);
    int (*pm_poweroff)(struct himedia_device *);
    int (*pm_restore)(struct himedia_device *);

    int (*pm_suspend_late)(struct himedia_device *);
    int (*pm_resume_early)(struct himedia_device *);
    int (*pm_freeze_late)(struct himedia_device *);
    int (*pm_thaw_early)(struct himedia_device *);
    int (*pm_poweroff_late)(struct himedia_device *);
    int (*pm_restore_early)(struct himedia_device *);

    int (*pm_suspend_noirq)(struct himedia_device *);
    int (*pm_resume_noirq)(struct himedia_device *);

    int (*pm_freeze_noirq)(struct himedia_device *);
    int (*pm_thaw_noirq)(struct himedia_device *);
    int (*pm_poweroff_noirq)(struct himedia_device *);
    int (*pm_restore_noirq)(struct himedia_device *);
};

#define HIMIDIA_MAX_DEV_NAME_LEN 32

struct himedia_driver {
    struct device_driver driver;
    struct himedia_ops *ops;
    char name[1];
};

#define to_himedia_driver(drv) \
    container_of((drv), struct himedia_driver, driver)

struct himedia_device {
    struct osal_list_head list;

    char devfs_name[HIMIDIA_MAX_DEV_NAME_LEN];

    unsigned int minor;

    struct device device;

    struct module *owner;

    const struct file_operations *fops;

    struct himedia_ops *drvops;

    /* for internal use */
    struct himedia_driver *driver;
};

#define to_himedia_device(dev) \
    container_of((dev), struct himedia_device, device)

int himedia_register(struct himedia_device *pdev);

int himedia_unregister(struct himedia_device *pdev);

#define MODULE_ALIAS_HIMEDIA(minor) \
    MODULE_ALIAS("himedia-char-major-" __stringify(HIMEDIA_DEVICE_MAJOR) "-" __stringify(minor))

#endif /* _LINUX_HIMEDIA_DEVICE_H_ */
