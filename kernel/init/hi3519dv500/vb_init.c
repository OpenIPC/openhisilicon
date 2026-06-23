/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#include <linux/module.h>
#include "ot_type.h"
#include "ot_vb_mod_init.h"

static int __init vb_mod_init(void)
{
    if (vb_module_init() != TD_SUCCESS) {
        return -EPERM;
    }
    return 0;
}

static void __exit vb_mod_exit(void)
{
    vb_module_exit();
}

module_init(vb_mod_init);
module_exit(vb_mod_exit);

MODULE_LICENSE("Proprietary");
