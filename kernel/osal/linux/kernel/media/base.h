/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef _MEDIA_BASE_H_
#define _MEDIA_BASE_H_

#include "media.h"

int media_bus_init(void);
void media_bus_exit(void);

#if 0
struct device *media_device_register(struct class *class,
                                       dev_t devt, const char *name);

void media_device_unregister(struct class *class, dev_t devt);

#else

int media_device_register(struct media_device *pdev);

void media_device_unregister(struct media_device *pdev);

#endif

struct media_driver *media_driver_register(const char *name,
                                               struct module *owner, struct media_ops *ops);

void media_driver_unregister(struct media_driver *pdrv);

#endif
