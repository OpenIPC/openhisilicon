/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#include <linux/module.h>
#include "ot_chnl_mod_init.h"

EXPORT_SYMBOL(chnl_reg_vpu);
EXPORT_SYMBOL(chnl_unreg_vpu);

static int __init chnl_mod_init(void)
{
    if (chnl_module_init() != TD_SUCCESS) {
        return -1;
    }

    return 0;
}
static void __exit chnl_mod_exit(void)
{
    chnl_module_exit();
}

module_init(chnl_mod_init);
module_exit(chnl_mod_exit);

MODULE_LICENSE("Proprietary");

