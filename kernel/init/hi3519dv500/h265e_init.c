/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#include <linux/module.h>
#include "ot_osal.h"
#include "ot_h265e_mod_init.h"

static td_u32 g_h265e_feature_en = 1;
#ifndef MODULE
osal_setup_num_param(g_h265e_feature_en, g_h265e_feature_en);
#else
module_param(g_h265e_feature_en, uint, S_IRUGO);
#endif

static int __init h265e_mod_init(void)
{
    h265e_set_feature_en_mod(g_h265e_feature_en);
    return h265e_module_init();
}
static void __exit h265e_mod_exit(void)
{
    h265e_module_exit();
}

module_init(h265e_mod_init);
module_exit(h265e_mod_exit);

MODULE_LICENSE("Proprietary");
