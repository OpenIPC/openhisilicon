/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef __OSAL_MEDIA_H__
#define __OSAL_MEDIA_H__

#include <asm/types.h>
#include <asm/atomic.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/major.h>
#include <linux/fs.h>

#ifdef __cplusplus
extern "C" {
#endif /* End of #ifdef __cplusplus */

#define MEDIA_DEVICE_MAJOR 218
#define MEDIA_DYNAMIC_MINOR 255
#define MEDIA_DEVICE_NAME_MAX_LEN 64

typedef struct tag_pm_basedev {
    int id;
    const char *name;
    struct device dev;
} pm_basedev;

#define to_media_basedrv(drv) container_of((drv), pm_basedrv, driver)

#define to_pm_basedev(x) container_of((x), pm_basedev, dev)

typedef struct tag_pm_baseops {
    int (*probe)(pm_basedev *);
    int (*remove)(pm_basedev *);
    void (*shutdown)(const pm_basedev *);
    int (*prepare)(pm_basedev *);
    void (*complete)(pm_basedev *);
    int (*suspend)(const pm_basedev *, pm_message_t state);
    int (*suspend_late)(pm_basedev *, pm_message_t state);
    int (*resume_early)(const pm_basedev *);
    int (*resume)(const pm_basedev *);
} pm_baseops;

typedef struct tag_pm_basedrv {
    int (*probe)(pm_basedev *);
    int (*remove)(pm_basedev *);
    void (*shutdown)(const pm_basedev *);
    int (*suspend)(const pm_basedev *, pm_message_t state);
    int (*suspend_late)(pm_basedev *, pm_message_t state);
    int (*resume_early)(const pm_basedev *);
    int (*resume)(const pm_basedev *);
    struct device_driver driver;
} pm_basedrv;

#define to_media_basedrv(drv) container_of((drv), pm_basedrv, driver)

typedef struct ext_umap_device {
    char devfs_name[MEDIA_DEVICE_NAME_MAX_LEN]; /* devfs */
    int minor;
    unsigned int parent_minor;
    struct module *owner;
    struct file_operations *fops;
    pm_baseops *drvops;
} umap_device, *ptr_umap_device;

typedef struct tag_pm_device {
    unsigned int minor;
    unsigned int parent_minor;
    const char *name;
    struct module *owner;
    const struct file_operations *app_ops;
    pm_baseops *base_ops;
    struct list_head list;
    struct device *app_device;
    pm_basedev *base_device;
    pm_basedrv *base_driver;
    umap_device *umap_dev;
    void *dev;
} pm_device;


int drv_pm_mod_init(void);
void drv_pm_mod_exit(void);
int drv_pm_register(pm_device *media);
int drv_pm_un_register(pm_device *media);

#ifdef __cplusplus
}
#endif /* End of #ifdef __cplusplus */

#endif /* __DRV_MEDIA_EXT_H__ */
