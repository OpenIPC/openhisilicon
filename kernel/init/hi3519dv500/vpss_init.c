/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include <linux/of_platform.h>
#include "securec.h"
#include "ot_osal.h"
#include "ot_defines.h"
#include "ot_vpss_mod_init.h"

#define VPSS_DEV_NAME_LENGTH 10

static bool g_vpss_high_profile = TD_FALSE;

#ifndef MODULE
osal_setup_num_param(g_vpss_high_profile, g_vpss_high_profile);
#else
module_param(g_vpss_high_profile, bool, S_IRUGO);
#endif

static int ot_vpss_probe(struct platform_device *pdev)
{
    td_u32 i;
    td_char vpss_dev_name[VPSS_DEV_NAME_LENGTH] = {'\0'};
    struct resource *mem = TD_NULL;
    td_void *vpss_reg = TD_NULL;
    td_s32 vpss_irq;

    if (pdev == TD_NULL) {
        osal_printk("%s,%d,dev is NULL!!\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }
    for (i = 0; i < OT_VPSS_IP_NUM; i++) {
        if (snprintf_s(vpss_dev_name, VPSS_DEV_NAME_LENGTH, VPSS_DEV_NAME_LENGTH - 1, "vpss%u", i) == -1) {
            osal_printk("set vpss%u name err!\n", i);
            /* deinitialization is unnecessary without resource allocation */
            return TD_FAILURE;
        }
        mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, vpss_dev_name);
        vpss_reg = devm_ioremap_resource(&pdev->dev, mem);
        if (IS_ERR(vpss_reg)) {
            return PTR_ERR(vpss_reg);
        }

        vpss_irq = osal_platform_get_irq_byname(pdev, vpss_dev_name);
        if (vpss_irq <= 0) {
            dev_err(&pdev->dev, "cannot find vpss%d IRQ\n", i);
        }
        if (vpss_set_init_vpss_reg(vpss_reg, i) != TD_SUCCESS) {
            return TD_FAILURE;
        }
        if (vpss_set_init_vpss_irq(vpss_irq, i) != TD_SUCCESS) {
            return TD_FAILURE;
        }
    }
    vpss_set_high_profile(g_vpss_high_profile);
    vpss_module_init();
    return 0;
}


static int ot_vpss_remove(struct platform_device *pdev)
{
    td_u32 i;
    ot_unused(pdev);

    vpss_module_exit();

    for (i = 0; i < OT_VPSS_IP_NUM; i++) {
        vpss_set_init_vpss_reg(TD_NULL, i);
    }

    return 0;
}

static const struct of_device_id g_ot_vpss_match[] = {
    { .compatible = "vendor,vpss" },
    {},
};

MODULE_DEVICE_TABLE(of, g_ot_vpss_match);

static struct platform_driver g_ot_vpss_driver = {
    .probe = ot_vpss_probe,
    .remove = ot_vpss_remove,
    .driver = {
        .name = "ot_vpss",
        .of_match_table = g_ot_vpss_match,
    },
};

osal_module_platform_driver(g_ot_vpss_driver);

MODULE_LICENSE("Proprietary");
