/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#include <linux/module.h>
#include <linux/of_platform.h>
#include "ot_vo_dev_mod_init.h"

EXPORT_SYMBOL(ot_vo_enable_dev_export);
EXPORT_SYMBOL(ot_vo_disable_dev_export);

static int __init vo_dev_mod_init(void)
{
    vo_dev_module_init();
    return 0;
}

static void __exit vo_dev_mod_exit(void)
{
    vo_dev_module_exit();
}

module_init(vo_dev_mod_init);
module_exit(vo_dev_mod_exit);

MODULE_LICENSE("Proprietary");

