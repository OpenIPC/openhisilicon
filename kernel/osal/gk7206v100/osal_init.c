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
    osal_device_init();
    osal_proc_init();
    media_init();
    media_mem_init();
    osal_printk("osal %s init success!\n", OSAL_VERSION);
    return 0;
}

static void __exit osal_exit(void)
{
    media_exit();
    media_mem_exit();
    osal_proc_exit();
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


