/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include "ot_type.h"
#include "ot_rc_mod_init.h"

static int __init rc_mod_init(void)
{
    if (rc_module_init() != TD_SUCCESS) {
        return -1;
    }
    return 0;
}
static void __exit rc_mod_exit(void)
{
    rc_module_exit();
}

module_init(rc_mod_init);
module_exit(rc_mod_exit);

MODULE_LICENSE("Proprietary");

