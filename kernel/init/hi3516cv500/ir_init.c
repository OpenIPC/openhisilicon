/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2006-2019. All rights reserved.
 * Description: Ir module's probe, remove.
 * Author: Hisilicon multimedia software group
 * Create: 2006-12-16
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>
#include "hiir.h"

#define OSDRV_MODULE_VERSION_STRING "HISI_ir @HiMPP"

extern unsigned int g_ir_irq;
extern volatile void *g_ir_reg_base;

static int hi_ir_probe(struct platform_device *pdev)
{
    struct resource *mem = NULL;

    g_ir_irq = platform_get_irq(pdev, 0);
    if (g_ir_irq <= 0) {
        dev_err(&pdev->dev, "cannot find ir IRQ%d. \n", g_ir_irq);
    }

    mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    g_ir_reg_base = (volatile void *)devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR((void *)g_ir_reg_base)) {
        dev_err(&pdev->dev, "ir reg map failed. \n");
    }

    return hiir_init();
}

static int hi_ir_remove(struct platform_device *pdev)
{
    hiir_exit();
    return 0;
}

static const struct of_device_id g_hi_ir_match[] = {
    { .compatible = "hisilicon,hi_ir" },
    { .compatible = "hisilicon,hi-ir" },
    {},
};

static struct platform_driver g_hi_ir_driver = {
    .probe  = hi_ir_probe,
    .remove = hi_ir_remove,
    .driver =  {
        .name = "hi_ir", /* hi-ir */
        .of_match_table = g_hi_ir_match,
    },
};

module_platform_driver(g_hi_ir_driver);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hisilicon");
MODULE_DESCRIPTION("Hisilicon Infrared remoter(HIIR11) Device Driver");
MODULE_VERSION("HI_VERSION=" OSDRV_MODULE_VERSION_STRING);