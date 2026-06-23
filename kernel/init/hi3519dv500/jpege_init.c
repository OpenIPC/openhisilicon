/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include "ot_jpege_mod_init.h"

static int __init jpege_mod_init(void)
{
    if (jpege_module_init() != TD_SUCCESS) {
        return -1;
    }
    return 0;
}
static void __exit jpege_mod_exit(void)
{
    jpege_module_exit();
}

module_init(jpege_mod_init);
module_exit(jpege_mod_exit);

MODULE_LICENSE("Proprietary");
