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
#include "ot_sensor_spi_mod_init.h"

static int spi_module_init(void)
{
    return spi_mod_init();
}

static void spi_module_exit(void)
{
    spi_mod_exit();
}

module_init(spi_module_init);
module_exit(spi_module_exit);
MODULE_LICENSE("GPL");
MODULE_ALIAS("sensor spidev");
