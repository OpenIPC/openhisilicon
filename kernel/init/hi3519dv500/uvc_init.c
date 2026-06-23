/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>


#include "ot_uvc_mod_init.h"


static int __init uvc_mod_init(void)
{
    if (uvc_module_init() != TD_SUCCESS) {
        return -1;
    }

    return 0;
}
static void __exit uvc_mod_exit(void)
{
    uvc_module_exit();
}

module_init(uvc_mod_init);
module_exit(uvc_mod_exit);

MODULE_LICENSE("Proprietary");

