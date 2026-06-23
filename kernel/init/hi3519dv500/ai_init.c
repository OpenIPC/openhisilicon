/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "ot_ai_mod_init.h"

static int __init ai_mod_init(void)
{
    return ai_module_init();
}

static void __exit ai_mod_exit(void)
{
    ai_module_exit();
}

module_init(ai_mod_init);
module_exit(ai_mod_exit);

MODULE_LICENSE("Proprietary");
