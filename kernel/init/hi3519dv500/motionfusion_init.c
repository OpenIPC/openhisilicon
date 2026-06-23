/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of_platform.h>

#include "ot_motionfusion_mod_init.h"

td_s32 motionfusion_init(td_void)
{
    return mfusion_drv_mod_init();
}

td_void motionfusion_exit(td_void)
{
    mfusion_drv_mod_exit();
}

module_init(motionfusion_init);
module_exit(motionfusion_exit);

MODULE_DESCRIPTION("motionfusion driver");
MODULE_LICENSE("Proprietary");

