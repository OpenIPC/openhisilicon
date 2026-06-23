/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of_platform.h>

#include "ot_aiisp_mod_init.h"

td_s32 aiisp_init(td_void)
{
    return aiv_drv_mod_init();
}

td_void aiisp_exit(td_void)
{
    aiv_drv_mod_exit();
}

module_init(aiisp_init);
module_exit(aiisp_exit);

MODULE_DESCRIPTION("aiisp driver");
MODULE_LICENSE("Proprietary");
