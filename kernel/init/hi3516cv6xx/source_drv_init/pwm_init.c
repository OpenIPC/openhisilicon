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
#include "ot_pwm_mod_init.h"

static int pwm_module_init(void)
{
    return pwm_mod_init();
}

static void pwm_module_exit(void)
{
    pwm_mod_exit();
}

module_init(pwm_module_init);
module_exit(pwm_module_exit);

MODULE_DESCRIPTION("piris driver");
MODULE_LICENSE("GPL");
