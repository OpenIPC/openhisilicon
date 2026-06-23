/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>

#include "ot_osal.h"
#include "ot_isp_mod_init.h"

static int isp_mod_init(void)
{
    return isp_module_init();
}

static void isp_mod_exit(void)
{
    isp_module_exit();
}

module_init(isp_mod_init);
module_exit(isp_mod_exit);

MODULE_LICENSE("GPL");
