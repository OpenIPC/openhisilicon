/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include "ot_osal.h"
#include "ot_vdec_mod_init.h"
#include "ot_vdec_export.h"

#define OT_VDEC_DEFAULT_COMPAT_MODE 0

static td_s32 g_vdec_max_chn_num = OT_VDEC_MAX_CHN_NUM;
static td_u32 g_vdec_compat_mode = OT_VDEC_DEFAULT_COMPAT_MODE;

EXPORT_SYMBOL(ot_vdec_get_export_symbol);

#ifndef MODULE
osal_setup_num_param(g_vdec_max_chn_num, g_vdec_max_chn_num);
osal_setup_num_param(g_vdec_compat_mode, g_vdec_compat_mode);
#else
module_param(g_vdec_max_chn_num, int, S_IRUGO);
module_param(g_vdec_compat_mode, uint, S_IRUGO);
#endif

static int __init vdec_mod_init(void)
{
    vdec_set_max_chn_num(g_vdec_max_chn_num);
    vdec_set_compat_mode(g_vdec_compat_mode);
    vdec_module_init();
    return 0;
}

static void __exit vdec_mod_exit(void)
{
    vdec_module_exit();
}

module_init(vdec_mod_init);
module_exit(vdec_mod_exit);

MODULE_LICENSE("Proprietary");

