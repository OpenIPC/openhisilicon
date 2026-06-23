/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "ot_aio_mod_init.h"
#include "ot_common.h"

EXPORT_SYMBOL(aiao_get_acodec_base);

static int ot_aiao_probe(struct platform_device *pdev)
{
    struct resource *mem = TD_NULL;
    int aiao_irq_tmp;
    td_void *aio_base_tmp = TD_NULL;
    td_void *acodec_base_tmp = TD_NULL;
    td_void *dmic_base_tmp = TD_NULL;

    mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, "aiao");
    aio_base_tmp = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(aio_base_tmp)) {
        return PTR_ERR(aio_base_tmp);
    }
    aiao_set_aio_base(aio_base_tmp);

    mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, "dmic");
    dmic_base_tmp = devm_ioremap_resource(&pdev->dev, mem);
    if (dmic_base_tmp == TD_NULL) {
        dev_err(&pdev->dev, "cannot remap dmic reg\n");
        return PTR_ERR(dmic_base_tmp);
    }
    aiao_set_dmic_base(dmic_base_tmp);

    mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, "acodec");
    acodec_base_tmp = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(acodec_base_tmp)) {
        return PTR_ERR(acodec_base_tmp);
    }
    aiao_set_acodec_base(acodec_base_tmp);
    aiao_set_reg_acodec_base(acodec_base_tmp);

    aiao_irq_tmp = osal_platform_get_irq_byname(pdev, "AIO");
    if (aiao_irq_tmp <= 0) {
        dev_err(&pdev->dev, "cannot find aiao IRQ\n");
    } else {
        aiao_set_aiao_irq((td_u32)aiao_irq_tmp);
    }

    return aiao_module_init();
}

static int ot_aiao_remove(struct platform_device *pdev)
{
    ot_unused(pdev);
    aiao_module_exit();
    return 0;
}


static const struct of_device_id g_ot_aiao_match[] = {
    { .compatible = "vendor,aiao" },
    {},
};
MODULE_DEVICE_TABLE(of, g_ot_aiao_match);

static struct platform_driver g_ot_aiao_driver = {
    .probe = ot_aiao_probe,
    .remove = ot_aiao_remove,
    .driver = {
        .name = "ot_aiao",
        .of_match_table = g_ot_aiao_match,
    },
};

osal_module_platform_driver(g_ot_aiao_driver);

MODULE_LICENSE("Proprietary");
