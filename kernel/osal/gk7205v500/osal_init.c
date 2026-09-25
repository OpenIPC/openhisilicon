/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include "osal.h"

extern void osal_proc_init(void);
extern void osal_proc_exit(void);
extern int media_init(void);
extern void media_exit(void);
extern int media_mem_init(void);
extern void media_mem_exit(void);
extern void osal_device_init(void);

static int __init osal_init(void)
{
    int ret;

    osal_device_init();
    osal_proc_init();
    ret = media_init();
    if (ret) {
        goto err_proc;
    }
    ret = media_mem_init();
    if (ret) {
        goto err_media;
    }
    osal_printk("osal %s init success!\n", OSAL_VERSION);
    return 0;

err_media:
    media_exit();
err_proc:
    osal_proc_exit();
    osal_printk("osal %s init failed: %d\n", OSAL_VERSION, ret);
    return ret;
}

static void __exit osal_exit(void)
{
    osal_proc_exit();
    media_exit();
    media_mem_exit();
    osal_printk("osal v1.0 exit!\n");
}



#ifdef MODULE

module_init(osal_init);
module_exit(osal_exit);
MODULE_LICENSE("GPL");

#else

int __init osal_driver_init(void)
{
    return osal_init();
}

#endif


