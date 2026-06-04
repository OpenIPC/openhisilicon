/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */
#include <linux/module.h>
#include "ot_type.h"
#include "ot_common.h"
#include "ot_user_proc_mod_init.h"

static int __init user_proc_module_init(td_void)
{
    return user_proc_init();
}

static void __exit user_proc_module_exit(td_void)
{
    user_proc_exit();
    return;
}

module_init(user_proc_module_init);
module_exit(user_proc_module_exit);

MODULE_LICENSE("Proprietary");
