/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include "ot_osal.h"
#include "mmz_init.h"

static int __init mmz_init(void)
{
    if (mem_check_module_param() == -1) {
        return -1;
    }
    if (media_mem_init() != 0) {
        goto failed;
    }
    osal_printk("load ot_mmz ....OK!\n");
    return 0;
failed:
    osal_printk("load ot_mmz failed!\n");
    return -1;
}

static void __exit mmz_exit(void)
{
    media_mem_exit();
    osal_printk("unload ot_mmz ....OK!\n");
}

module_init(mmz_init);
module_exit(mmz_exit);

MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
