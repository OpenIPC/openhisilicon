/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef _LINUX_MEDIA_DEVICE_H_
#define _LINUX_MEDIA_DEVICE_H_

#include <linux/module.h>
#include <linux/major.h>
#include <linux/device.h>
#include "osal_list.h"

#define MEDIA_DEVICE_MAJOR 218
#define MEDIA_DYNAMIC_MINOR 255

struct media_device;

struct media_ops {
	// pm methos
	int (*pm_prepare)(struct media_device *);
	void (*pm_complete)(struct media_device *);

	int (*pm_suspend)(struct media_device *);
	int (*pm_resume)(struct media_device *);

	int (*pm_freeze)(struct media_device *);
	int (*pm_thaw)(struct media_device *);
	int (*pm_poweroff)(struct media_device *);
	int (*pm_restore)(struct media_device *);

	int (*pm_suspend_late)(struct media_device *);
	int (*pm_resume_early)(struct media_device *);
	int (*pm_freeze_late)(struct media_device *);
	int (*pm_thaw_early)(struct media_device *);
	int (*pm_poweroff_late)(struct media_device *);
	int (*pm_restore_early)(struct media_device *);

	int (*pm_suspend_noirq)(struct media_device *);
	int (*pm_resume_noirq)(struct media_device *);

	int (*pm_freeze_noirq)(struct media_device *);
	int (*pm_thaw_noirq)(struct media_device *);
	int (*pm_poweroff_noirq)(struct media_device *);
	int (*pm_restore_noirq)(struct media_device *);
};

#define MEDIA_MAX_DEV_NAME_LEN 32

struct media_driver {
	struct device_driver driver;
	struct media_ops *ops;
	char name[1];
};

#define to_media_driver(drv) container_of((drv), struct media_driver, driver)

struct media_device {
	struct osal_list_head list;

	char devfs_name[MEDIA_MAX_DEV_NAME_LEN];

	unsigned int minor;

	struct device device;

	struct module *owner;

	const struct file_operations *fops;

	struct media_ops *drvops;

	/* for internal use */
	struct media_driver *driver;
};

#define to_media_device(dev) container_of((dev), struct media_device, device)

int media_register(struct media_device *pdev);

int media_unregister(struct media_device *pdev);

#define MODULE_ALIAS_MEDIA(minor)                     \
	MODULE_ALIAS("media-char-major-" __stringify( \
		MEDIA_DEVICE_MAJOR) "-" __stringify(minor))

#endif /* _LINUX_MEDIA_DEVICE_H_ */
