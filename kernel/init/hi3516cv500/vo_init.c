/*
* Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#include <linux/module.h>
#include <linux/of_platform.h>

#include "hi_defines.h"
#include "hi_type.h"
#include "hi_osal.h"
#include "vou_exp.h"

extern VOU_EXPORT_SYMBOL_S g_vou_exp_symbol;

EXPORT_SYMBOL(g_vou_exp_symbol);

extern int vou_mod_init(void);
extern void vou_mod_exit(void);

extern void *g_vo_reg;
extern unsigned int g_vou_irq;

static int hi35xx_vo_probe(struct platform_device *pdev)
{
    struct resource *mem;
    int ret;
    int tmp_vou_irq;

    mem = osal_platform_get_resource(pdev, IORESOURCE_MEM, 0);
    g_vo_reg = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(g_vo_reg)) {
        return PTR_ERR(g_vo_reg);
    }

    tmp_vou_irq = osal_platform_get_irq(pdev, 0);
    if (tmp_vou_irq <= 0) {
        dev_err(&pdev->dev, "cannot find vou IRQ\n");
        return HI_FAILURE;
    }
    else {
        g_vou_irq = (unsigned int)tmp_vou_irq;
    }

    ret = vou_mod_init();
    if (ret) {
        return -1;
    }

    return 0;
}

static int hi35xx_vo_remove(struct platform_device *pdev)
{
    vou_mod_exit();
    g_vo_reg = NULL;
    return 0;
}


static const struct of_device_id g_hi35xx_vo_match[] = {
        { .compatible = "hisilicon,hisi-vo" },
        {},
};
MODULE_DEVICE_TABLE(of, g_hi35xx_vo_match);

static struct platform_driver g_hi35xx_vo_driver = {
        .probe          = hi35xx_vo_probe,
        .remove         = hi35xx_vo_remove,
        .driver         = {
                .name   = "hi35xx_vo",
                .of_match_table = g_hi35xx_vo_match,
        },
};

osal_module_platform_driver(g_hi35xx_vo_driver);

MODULE_LICENSE("Proprietary");
