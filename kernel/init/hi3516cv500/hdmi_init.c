/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019. All rights reserved.
 * Description: hdmi init
 * Author: Hisilicon multimedia software group
 * Create: 2012/06/26
 */
#include "hdmi_init.h"

#define HDMI_DEV_NAME_LENGTH 16

static int hi35xx_hdmi_probe(struct platform_device *pdev)
{
    struct resource *mem = NULL;
    hi_char *hdmi_reg = NULL;
    hi_char hdmi_dev_name[HDMI_DEV_NAME_LENGTH] = "hdmi0";

    mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, hdmi_dev_name);
    hdmi_reg = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(hdmi_reg)) {
        return PTR_ERR(hdmi_reg);
    }
    hdmi_set_reg(hdmi_reg);

    return hdmi_drv_mod_init();
}

static int hi35xx_hdmi_remove(struct platform_device *pdev)
{
    hdmi_drv_mod_exit();
    hdmi_set_reg(NULL);
    return HI_SUCCESS;
}

static const struct of_device_id g_hi35xx_hdmi_match[] = {
    { .compatible = "hisilicon,hisi-hdmi" },
    {}
};

MODULE_DEVICE_TABLE(of, g_hi35xx_hdmi_match);

static struct platform_driver g_hi35xx_hdmi_driver = {
    .probe  = hi35xx_hdmi_probe,
    .remove = hi35xx_hdmi_remove,
    .driver = {
        .name = "hi35xx_hdmi",
        .of_match_table = g_hi35xx_hdmi_match,
    },
};

osal_module_platform_driver(g_hi35xx_hdmi_driver);

MODULE_LICENSE("Proprietary");

