/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "ot_adec_mod_init.h"

static int __init adec_mod_init(void)
{
    return adec_module_init();
}

static void __exit adec_mod_exit(void)
{
    adec_module_exit();
}

module_init(adec_mod_init);
module_exit(adec_mod_exit);

MODULE_LICENSE("Proprietary");
