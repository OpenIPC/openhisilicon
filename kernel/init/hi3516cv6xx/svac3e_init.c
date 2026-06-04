/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */
#include <linux/module.h>
#include "ot_svac3e_mod_init.h"

static int __init svac3e_mod_init(void)
{
    return svac3e_module_init();
}
static void __exit svac3e_mod_exit(void)
{
    svac3e_module_exit();
}

module_init(svac3e_mod_init);
module_exit(svac3e_mod_exit);

MODULE_LICENSE("Proprietary");
