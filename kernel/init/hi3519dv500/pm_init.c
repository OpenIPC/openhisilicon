/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#include "ot_pm_mod_init.h"
#include <linux/module.h>

#ifndef MODULE
static unsigned int g_pm_quick_start_flag = 1;
#endif

#ifdef OT_PM_DEBUG
static int g_pm_core_extra_comp = 0;
static int g_pm_cpu_extra_comp = 0;
static int g_pm_npu_extra_comp = 0;
#ifndef MODULE
osal_setup_num_param(g_pm_core_extra_comp, g_pm_core_extra_comp);
osal_setup_num_param(g_pm_cpu_extra_comp, g_pm_cpu_extra_comp);
osal_setup_num_param(g_pm_npu_extra_comp, g_pm_npu_extra_comp);
#else
module_param(g_pm_core_extra_comp, int, S_IRUGO);
module_param(g_pm_cpu_extra_comp, int, S_IRUGO);
module_param(g_pm_npu_extra_comp, int, S_IRUGO);
#endif
#endif

static int __init pm_mod_init(void) {
#ifdef OT_PM_DEBUG
    pm_set_core_extra_comp(g_pm_core_extra_comp);
    pm_set_cpu_extra_comp(g_pm_cpu_extra_comp);
    pm_set_npu_extra_comp(g_pm_npu_extra_comp);
#endif
#ifndef MODULE
    pm_set_quick_start_flag(g_pm_quick_start_flag);
#endif
    pm_module_init();
    return 0;
}

static void __exit pm_mod_exit(void) {
    pm_module_exit();
}

module_init(pm_mod_init);
module_exit(pm_mod_exit);

MODULE_LICENSE("Proprietary");
