/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include "ot_rgn_mod_init.h"

static int __init rgn_mod_init(void)
{
    region_module_init();
    return 0;
}
static void __exit rgn_mod_exit(void)
{
    region_module_exit();
}

module_init(rgn_mod_init);
module_exit(rgn_mod_exit);

MODULE_LICENSE("Proprietary");

