/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include "ot_h264e_mod_init.h"

static int __init h264e_mod_init(void)
{
    return h264e_module_init();
}
static void __exit h264e_mod_exit(void)
{
    h264e_module_exit();
}

module_init(h264e_mod_init);
module_exit(h264e_mod_exit);

MODULE_LICENSE("Proprietary");
