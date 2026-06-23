/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include <linux/of_platform.h>
#include "ot_vo_mod_init.h"

EXPORT_SYMBOL(ot_vo_get_export_symbol);

static int ot_vo_probe(struct platform_device *pdev)
{
    int ret;
    int vo_irq;
    void *vo_reg = TD_NULL;
    struct resource *mem = TD_NULL;

    if (pdev == TD_NULL) {
        osal_printk("%s,%d,dev is NULL!!\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, "vo");
    vo_reg = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(vo_reg)) {
        return PTR_ERR(vo_reg);
    }
    vo_set_init_vo_reg(vo_reg);

    vo_irq = osal_platform_get_irq_byname(pdev, "vo");
    if (vo_irq <= 0) {
        dev_err(&pdev->dev, "cannot find vo IRQ\n");
        return TD_FAILURE;
    }
    vo_set_init_vo_irq((unsigned int)vo_irq);

    ret = vo_mod_init();
    if (ret) {
        return -1;
    }

    return 0;
}

static int ot_vo_remove(struct platform_device *pdev)
{
    ot_unused(pdev);
    vo_mod_exit();
    vo_set_init_vo_reg(TD_NULL);
    return 0;
}

static const struct of_device_id g_ot_vo_match[] = {
    { .compatible = "vendor,vo" },
    {},
};
MODULE_DEVICE_TABLE(of, g_ot_vo_match);

static struct platform_driver g_ot_vo_driver = {
    .probe = ot_vo_probe,
    .remove = ot_vo_remove,
    .driver = {
        .name = "ot_vo",
        .of_match_table = g_ot_vo_match,
    },
};

osal_module_platform_driver(g_ot_vo_driver);

MODULE_LICENSE("Proprietary");
