/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "ot_aenc_mod_init.h"

static int __init aenc_mod_init(void)
{
    return aenc_module_init();
}

static void __exit aenc_mod_exit(void)
{
    aenc_module_exit();
}

module_init(aenc_mod_init);
module_exit(aenc_mod_exit);

MODULE_LICENSE("Proprietary");
