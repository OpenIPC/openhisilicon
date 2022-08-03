/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>

extern unsigned int lsadc_irq;
extern volatile void *lsadc_reg;
extern int lsadc_init(void);
extern void lsadc_exit(void);

static int adc_probe(struct platform_device *pdev)
{
	struct resource *mem;

	lsadc_irq = platform_get_irq(pdev, 0);
	if (lsadc_irq <= 0) {
		dev_err(&pdev->dev, "cannot find lsadc IRQ%d. \n", lsadc_irq);
	}

	mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	lsadc_reg = (volatile void *)devm_ioremap_resource(&pdev->dev, mem);
	if (IS_ERR((void *)lsadc_reg)) {
		//printk("mem->start %#x. \n", mem->start);
		dev_err(&pdev->dev, "lsadc reg map failed. \n");
	}

	//printk("lsadc_irq :%d  mem->start %#x. \n", lsadc_irq, mem->start);

	return lsadc_init();
}

static int adc_remove(struct platform_device *pdev)
{
	lsadc_exit();
	return 0;
}

static const struct of_device_id adc_match[] = {
	{ .compatible = "goke,lsadc" },
	{},
};

static struct platform_driver lsadc_driver = {
    .probe  = adc_probe,
    .remove = adc_remove,
    .driver =  { .name = "lsadc",
                .of_match_table = adc_match,
               },
};

module_platform_driver(lsadc_driver);

MODULE_LICENSE("GPL");
