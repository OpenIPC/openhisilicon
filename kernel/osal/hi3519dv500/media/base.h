/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#ifndef _MEDIA_BASE_H_
#define _MEDIA_BASE_H_

#include "ot_media.h"

// bus
int media_bus_init(void);
void media_bus_exit(void);

// device
int media_device_register(pm_basedev *pdev);
void media_device_unregister(pm_basedev *pdev);
int media_device_add(pm_basedev *pdev);
void media_device_del(pm_basedev *pdev);
void media_device_put(pm_basedev *pdev);
pm_basedev *media_device_alloc(const char *name, int id);

// driver
int media_driver_register(pm_basedrv *drv);
void media_driver_unregister(pm_basedrv *drv);
void media_driver_release(pm_basedrv *drv);
pm_basedrv *media_driver_alloc(const char *name, struct module *owner, const pm_baseops *ops);

#endif
