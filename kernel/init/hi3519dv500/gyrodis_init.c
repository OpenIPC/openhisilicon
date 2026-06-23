/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>

#include "ot_common.h"
#include "ot_osal.h"
#include "ot_gyrodis_mod_init.h"

#define DIS_DEV_NAME_LENGTH 10

static int __init gyrodis_module_init(void)
{
    return gyrodis_mod_init();
}

static void __exit gyrodis_module_exit(void)
{
    gyrodis_mod_exit();
}

module_init(gyrodis_module_init);
module_exit(gyrodis_module_exit);

MODULE_LICENSE("Proprietary");

