/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>

#include "ot_common.h"
#include "ot_osal.h"
#include "ot_dis_mod_init.h"

#define DIS_DEV_NAME_LENGTH 10

static int __init dis_mod_init(void)
{
    return dis_module_init();
}
static void __exit dis_mod_exit(void)
{
    dis_module_exit();
}
module_init(dis_mod_init);
module_exit(dis_mod_exit);


MODULE_LICENSE("Proprietary");

