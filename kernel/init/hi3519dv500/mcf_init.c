/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/of_platform.h>

#include "ot_type.h"
#include "ot_osal.h"
#include "ot_mcf_mod_init.h"

static td_s32 mcf_init(td_void)
{
    return mcf_mod_init();
}

static td_void mcf_exit(td_void)
{
    mcf_mod_exit();
}

module_init(mcf_init);
module_exit(mcf_exit);

MODULE_DESCRIPTION("mcf driver");
MODULE_LICENSE("Proprietary");
