/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>
#include <ot_mipi_rx_mod_init.h>
#include <ot_mipi_rx.h>
#include <ot_osal.h>

static int ot_mipi_rx_probe(struct platform_device *pdev)
{
    struct  resource *mem = NULL;
    int irq_num;
    void *regs = NULL;

    mipi_rx_unused(pdev);

    mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, "mipi_rx");
    regs = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(regs)) {
        dev_err(&pdev->dev, "mipi_rx remap mem error.\n");
        return PTR_ERR(regs);
    }
    mipi_rx_set_regs(regs);

    irq_num = osal_platform_get_irq_byname(pdev, "mipi_rx");
    if (irq_num <= 0) {
        dev_err(&pdev->dev, "can not find mipi_rx IRQ\n");
    }
    mipi_rx_set_irq_num((unsigned int)irq_num);

    return mipi_rx_mod_init();
}

static int ot_mipi_rx_remove(struct platform_device *pdev)
{
    mipi_rx_unused(pdev);
    mipi_rx_mod_exit();
    mipi_rx_set_regs(NULL);

    return 0;
}

static const struct of_device_id g_ot_mipi_rx_match[] = {
    { .compatible = "vendor,mipi_rx" },
    {},
};
MODULE_DEVICE_TABLE(of, g_ot_mipi_rx_match);

static struct platform_driver g_ot_mipi_rx_driver = {
    .probe  = ot_mipi_rx_probe,
    .remove = ot_mipi_rx_remove,
    .driver = {
        .name           = "ot_mipi_rx",
        .of_match_table = g_ot_mipi_rx_match,
    },
};

module_platform_driver(g_ot_mipi_rx_driver);

MODULE_AUTHOR("Digital Media Team.");
MODULE_DESCRIPTION("Mipi_Rx Driver");
MODULE_VERSION("OT_VERSION = mipi_rx @OT_MPP");
MODULE_LICENSE("GPL");
