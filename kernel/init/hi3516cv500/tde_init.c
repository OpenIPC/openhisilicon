/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2018-2019. All rights reserved.
 * Description: tde init
 */

#include "tde_init.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>
#include "hi_type.h"
#include "hi_common.h"
#include "hi_osal.h"
#include "../../compat/kernel_compat.h"

static char *g_pszTdeMmzName = HI_NULL;
module_param(g_pszTdeMmzName, charp, S_IRUGO);

hi_u32 g_u32TdeTmpBuf = 1658880; /* 1658880 buffer size */
module_param(g_u32TdeTmpBuf, uint, S_IRUGO);

bool g_bResizeFilter = true;
module_param(g_bResizeFilter, bool, S_IRUGO);

hi_u32 g_u32TdeBuf = 0x20000; /* 0x20000 buffer size */
module_param(g_u32TdeBuf, uint, S_IRUGO);

hi_u32 tde_init_get_tmp_buf(hi_void)
{
    return g_u32TdeTmpBuf;
}

bool tde_init_get_resize_filter(hi_void)
{
    return g_bResizeFilter;
}

hi_u32 tde_init_get_buf(hi_void)
{
    return g_u32TdeBuf;
}

hi_void tde_init_set_buf(hi_u32 temp_buf)
{
    g_u32TdeBuf = temp_buf;
}

static int hi35xx_tde_probe(struct platform_device *pdev)
{
    struct resource *mem = HI_NULL;
    unsigned int tde_irq;
    hi_u32 *base_vir_addr = HI_NULL;

    mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, "tde");
    base_vir_addr = devm_ioremap_resource(&pdev->dev, mem);
    tde_hal_set_base_vir_addr(base_vir_addr);
    if (IS_ERR(base_vir_addr)) {
        return PTR_ERR(base_vir_addr);
    }

    tde_irq = osal_platform_get_irq_byname(pdev, "tde_osr_isr");
    set_tde_irq(tde_irq);
    if (tde_irq <= 0) {
        dev_err(&pdev->dev, "cannot find tde IRQ\n");
    }

    if (tde_drv_mod_init() != HI_SUCCESS) {
        osal_printk("load tde.ko for %s...FAILED!\n", CHIP_NAME);
        return -1;
    }
    osal_printk("load tde.ko for %s...OK!\n", CHIP_NAME);
    return 0;
}

static compat_platform_remove_ret hi35xx_tde_remove(struct platform_device *pdev)
{
    tde_drv_mod_exit();
    osal_printk("unload tde.ko for %s...OK!\n", CHIP_NAME);
    compat_platform_remove_return;
}

static const struct of_device_id g_hi35xx_tde_match[] = {
    {.compatible = "hisilicon,hisi-tde"},
    {},
};
MODULE_DEVICE_TABLE(of, g_hi35xx_tde_match);

static struct platform_driver g_hi35xx_tde_driver = {
    .probe = hi35xx_tde_probe,
    .remove = hi35xx_tde_remove,
    .driver =
        {
            .name = "hi35xx_tde",
            .of_match_table = g_hi35xx_tde_match,
        },
};

osal_module_platform_driver(g_hi35xx_tde_driver);

MODULE_LICENSE("GPL");
