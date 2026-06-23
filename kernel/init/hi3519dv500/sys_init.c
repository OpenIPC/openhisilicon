/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>
#include "ot_common.h"
#include "ot_osal.h"
#include "ot_sys_mod_init.h"

#define PHASE               32
#define COEFF_BUTT          (PHASE / 2 + 1)

static td_bool g_sys_reg_by_dts = TD_FALSE;
static td_void *g_reg_crg_base_addr = TD_NULL;
static td_void *g_reg_sys_base_addr = TD_NULL;
static td_void *g_reg_ddr0_base_addr = TD_NULL;
static td_void *g_reg_misc_base_addr = TD_NULL;

static void sys_set_base_config(void)
{
    sys_set_reg_by_dts(g_sys_reg_by_dts);
    sys_set_crg_base_addr(g_reg_crg_base_addr);
    sys_set_sys_base_addr(g_reg_sys_base_addr);
    sys_set_ddr0_base_addr(g_reg_ddr0_base_addr);
    sys_set_misc_base_addr(g_reg_misc_base_addr);
}

static int ot_sys_probe(struct platform_device *pdev)
{
    struct resource *mem = TD_NULL;

    if (pdev == TD_NULL) {
        osal_printk("%s, %d, dev is NULL!!\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, "crg");
    g_reg_crg_base_addr = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(g_reg_crg_base_addr)) {
        return PTR_ERR(g_reg_crg_base_addr);
    }

    mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, "sys");
    g_reg_sys_base_addr = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(g_reg_sys_base_addr)) {
        return PTR_ERR(g_reg_sys_base_addr);
    }

    mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, "ddr");
    g_reg_ddr0_base_addr = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(g_reg_ddr0_base_addr)) {
        return PTR_ERR(g_reg_ddr0_base_addr);
    }

    mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, "misc");
    g_reg_misc_base_addr = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(g_reg_misc_base_addr)) {
        return PTR_ERR(g_reg_misc_base_addr);
    }

    g_sys_reg_by_dts = TD_TRUE;
    sys_set_base_config();
    if (sys_do_mod_init() != TD_SUCCESS) {
        return TD_FAILURE;
    }

    return 0;
}

static int ot_sys_remove(struct platform_device *pdev)
{
    ot_unused(pdev);

    sys_do_mod_exit();

    g_reg_crg_base_addr = NULL;
    g_reg_sys_base_addr = NULL;
    g_reg_ddr0_base_addr = NULL;
    g_reg_misc_base_addr = NULL;

    return 0;
}

static const struct of_device_id g_ot_sys_match[] = {
    { .compatible = "vendor,sys" },
    {},
};
MODULE_DEVICE_TABLE(of, g_ot_sys_match);

static struct platform_driver g_ot_sys_driver = {
    .probe = ot_sys_probe,
    .remove = ot_sys_remove,
    .driver = {
        .name = "ot_sys",
        .of_match_table = g_ot_sys_match,
    },
};

osal_module_platform_driver(g_ot_sys_driver);

MODULE_LICENSE("Proprietary");
