/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include "ot_vpp_mod_init.h"

static int __init vpp_mod_init(void)
{
    if (vpp_module_init() != TD_SUCCESS) {
        return -1;
    }

    return 0;
}
static void __exit vpp_mod_exit(void)
{
    vpp_module_exit();
}

module_init(vpp_mod_init);
module_exit(vpp_mod_exit);

MODULE_LICENSE("Proprietary");

