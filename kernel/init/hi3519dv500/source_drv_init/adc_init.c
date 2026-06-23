/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>

#include "ot_type.h"
#include "ot_adc.h"

#define OSDRV_MODULE_VERSION_STRING "OT_LSADC @OTMPP"

static int ot_adc_probe(struct platform_device *pdev)
{
    struct resource *mem = NULL;
    uintptr_t *ot_adc_irq = get_adc_irq();
    int irq_index;
    volatile void **ot_adc_reg_base = get_adc_reg_base();

    irq_index = platform_get_irq(pdev, 0);
    if (irq_index <= 0) {
        dev_err(&pdev->dev, "cannot find lsadc IRQ%lu. \n", *ot_adc_irq);
    } else {
        *ot_adc_irq = (uintptr_t)irq_index;
    }

    mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    *ot_adc_reg_base = (volatile void *)devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR((void*)*ot_adc_reg_base)) {
        dev_err(&pdev->dev, "lsadc reg map failed. \n");
    }

    return lsadc_init();
}

static int ot_adc_remove(struct platform_device *pdev)
{
    ot_adc_unused(pdev);

    lsadc_exit();
    return 0;
}

static const struct of_device_id g_ot_adc_match[] = {
    { .compatible = "vendor,lsadc" },
    {},
};

static struct platform_driver g_ot_adc_driver = {
    .probe  = ot_adc_probe,
    .remove = ot_adc_remove,
    .driver =  { .name = "lsadc",
                 .of_match_table = g_ot_adc_match,
               },
};

module_platform_driver(g_ot_adc_driver);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ADC Driver");
MODULE_VERSION("OT_VERSION=" OSDRV_MODULE_VERSION_STRING);