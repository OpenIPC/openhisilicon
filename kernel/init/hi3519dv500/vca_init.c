/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include "ot_vca_mod_init.h"

static int __init vca_mod_init(void)
{
    if (vca_module_init() != TD_SUCCESS) {
        return -1;
    }
    return 0;
}

static void __exit vca_mod_exit(void)
{
    vca_module_exit();
}

module_init(vca_mod_init);
module_exit(vca_mod_exit);

MODULE_LICENSE("Proprietary");
