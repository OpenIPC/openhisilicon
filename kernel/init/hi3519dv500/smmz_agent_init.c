/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#include <linux/module.h>

#include "ot_smmz_agent_mod_init.h"
#include "smmz_agent_export.h"

EXPORT_SYMBOL(smmb_phyaddr);

static int __init smmz_agent_mod_init(void)
{
    smmz_agent_driver_init();
    return 0;
}

static void __exit smmz_agent_mod_exit(void)
{
    smmz_agent_driver_exit();
}

module_init(smmz_agent_mod_init);
module_exit(smmz_agent_mod_exit);

MODULE_LICENSE("GPL");
