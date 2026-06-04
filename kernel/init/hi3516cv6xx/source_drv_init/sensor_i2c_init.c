/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>
#include "ot_osal.h"
#include "ot_sensor_i2c_mod_init.h"

static int i2c_module_init(void)
{
    return i2c_mod_init();
}

static void i2c_module_exit(void)
{
    i2c_mod_exit();
}

module_init(i2c_module_init);
module_exit(i2c_module_exit);
MODULE_LICENSE("GPL");
