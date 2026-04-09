#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>

#include "hi_type.h"
#include "hi_osal.h"

#define PHASE               32
#define COEFF_BUTT          (PHASE / 2 + 1)

extern hi_bool g_sys_reg_by_dts;

extern int sys_do_mod_init(void);
extern void sys_do_mod_exit(void);

extern void  *g_reg_crg_base_va;
EXPORT_SYMBOL(g_reg_crg_base_va);
extern void  *g_reg_sys_base_va;
EXPORT_SYMBOL(g_reg_sys_base_va);
extern void  *g_reg_ddr0_base_va;
EXPORT_SYMBOL(g_reg_ddr0_base_va);
extern void  *g_reg_misc_base_va;
EXPORT_SYMBOL(g_reg_misc_base_va);
extern void  *g_reg_otp_base_va;
EXPORT_SYMBOL(g_reg_otp_base_va);

static int hi35xx_sys_probe(struct platform_device *pdev)
{
    struct resource *mem;

    mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, "crg");
    g_reg_crg_base_va = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(g_reg_crg_base_va)) {
        return PTR_ERR(g_reg_crg_base_va);
    }

    mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, "sys");
    g_reg_sys_base_va = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(g_reg_sys_base_va)) {
        return PTR_ERR(g_reg_sys_base_va);
    }

    mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, "ddr");
    g_reg_ddr0_base_va = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(g_reg_ddr0_base_va)) {
        return PTR_ERR(g_reg_ddr0_base_va);
    }

    mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, "misc");
    g_reg_misc_base_va = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(g_reg_misc_base_va)) {
        return PTR_ERR(g_reg_misc_base_va);
    }

    g_sys_reg_by_dts = HI_TRUE;
    if (HI_SUCCESS != sys_do_mod_init()) {
        return HI_FAILURE;
    }

    return 0;
}

static int hi35xx_sys_remove(struct platform_device *pdev)
{
    sys_do_mod_exit();

    g_reg_crg_base_va = NULL;
    g_reg_sys_base_va = NULL;
    g_reg_ddr0_base_va = NULL;
    g_reg_misc_base_va = NULL;

    return 0;
}

static const struct of_device_id hi35xx_sys_match[] = {
    { .compatible = "hisilicon,hisi-sys" },
    {},
};
MODULE_DEVICE_TABLE(of, hi35xx_sys_match);

static struct platform_driver hi35xx_sys_driver = {
    .probe = hi35xx_sys_probe,
    .remove = hi35xx_sys_remove,
    .driver = {
        .name = "hi35xx_sys",
        .of_match_table = hi35xx_sys_match,
    },
};

osal_module_platform_driver(hi35xx_sys_driver);

MODULE_LICENSE("Proprietary");
