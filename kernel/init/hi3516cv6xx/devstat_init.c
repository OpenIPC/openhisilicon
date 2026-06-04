/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include "ot_devstat_mod_init.h"

static int __init devstat_mod_init(void)
{
    if (devstat_module_init() != TD_SUCCESS) {
        return -1;
    }
    return 0;
}

static void __exit devstat_mod_exit(void)
{
    devstat_module_exit();
}

module_init(devstat_mod_init);
module_exit(devstat_mod_exit);

MODULE_LICENSE("Proprietary");