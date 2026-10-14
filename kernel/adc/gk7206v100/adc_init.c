/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>

#include "osal.h"


extern unsigned int lsadc_irq;
extern volatile void *lsadc_reg;
extern int  lsadc_init(void);
extern void  lsadc_exit(void);

static int adc_probe(struct platform_device *pdev)
{
    struct resource *mem;
    void __iomem *reg;
    int irq;

    /* lsadc_init() falls back to the fixed LSADC IRQ and register base when
     * these are 0/NULL, so a missing resource is reported and handed over as
     * that -- not as a negative IRQ in an unsigned, or an ERR_PTR it would
     * then use as the register base. */
    irq = platform_get_irq(pdev, 0);
    if (irq <= 0) {
        dev_err(&pdev->dev, "cannot find lsadc IRQ (%d), using the default\n", irq);
        irq = 0;
    }
    lsadc_irq = irq;

    mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    reg = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(reg)) {
        dev_err(&pdev->dev, "lsadc reg map failed (%ld), using the default\n", PTR_ERR(reg));
        reg = NULL;
    }
    lsadc_reg = (volatile void *)reg;

    return lsadc_init();
}

static int adc_remove(struct platform_device *pdev)
{
    lsadc_exit();
    return 0;
}

static const struct of_device_id adc_match[] = {
    { .compatible = "lotus,lsadc" },
    {},
};

static struct platform_driver lsadc_driver = {
    .probe  = adc_probe,
    .remove = adc_remove,
    .driver =  { .name = "xmedia_lsadc",
                .of_match_table = adc_match,
               },
};

#ifdef MODULE

osal_module_platform_driver(lsadc_driver);
MODULE_LICENSE("GPL");

#else

int __init adc_driver_init(void)
{
    return osal_platform_driver_register(&lsadc_driver);
}

#endif



