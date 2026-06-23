/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "ot_ao_mod_init.h"
#include "ot_ao_export.h"

EXPORT_SYMBOL(ot_ao_get_export_symbol);

static int __init ao_mod_init(void)
{
    return ao_module_init();
}

static void __exit ao_mod_exit(void)
{
    ao_module_exit();
}

module_init(ao_mod_init);
module_exit(ao_mod_exit);

MODULE_LICENSE("Proprietary");
