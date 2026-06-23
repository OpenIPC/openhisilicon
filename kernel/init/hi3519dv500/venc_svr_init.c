/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include "ot_type.h"
#include "ot_venc_svr_mod_init.h"

static int __init venc_svr_mod_init(void)
{
    td_s32 ret;

    ret = venc_svr_module_init();
    if (ret != TD_SUCCESS) {
        return -1;
    }
    return 0;
}

static void __exit venc_svr_mod_exit(void)
{
    venc_svr_module_exit();
}

module_init(venc_svr_mod_init);
module_exit(venc_svr_mod_exit);

MODULE_LICENSE("Proprietary");
