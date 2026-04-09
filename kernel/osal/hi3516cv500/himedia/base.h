/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2016-2019.
 * Description: osal himedia base header file.
 * Author: Hisilicon multimedia software group
 * Create: 2016-11-11
 */
#ifndef _HIMEDIA_BASE_H_
#define _HIMEDIA_BASE_H_

#include "himedia.h"

int himedia_bus_init(void);
void himedia_bus_exit(void);

int himedia_device_register(struct himedia_device *pdev);

void himedia_device_unregister(struct himedia_device *pdev);

struct himedia_driver *himedia_driver_register(const char *name,
                                               struct module *owner, struct himedia_ops *ops);

void himedia_driver_unregister(struct himedia_driver *pdrv);

#endif
