/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>
#include "ot_common.h"
#include "ot_vfmw_mod_init.h"
#include "ot_type.h"
#include "ot_osal.h"

static int g_vfmw_max_chn_num;
#ifndef MODULE
osal_setup_num_param(g_vfmw_max_chn_num, g_vfmw_max_chn_num);
#else
module_param(g_vfmw_max_chn_num, uint, S_IRUGO);
#endif
EXPORT_SYMBOL(pdt_set_pxp_int);

static int vfmw_probe(struct platform_device *device)
{
    struct resource *scd_mem = NULL;
    void *scd_reg_addr = NULL;
    int scd_int;
    int pxp_int;

    if (device == TD_NULL) {
        osal_printk("%s,%d,dev is NULL!!\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    scd_mem = osal_platform_get_resource_byname(device, IORESOURCE_MEM, "scd");
    scd_reg_addr = devm_ioremap_resource(&device->dev, scd_mem);
    if (IS_ERR(scd_reg_addr)) {
        return PTR_ERR(scd_reg_addr);
    }
    pdt_set_scd_reg_addr(scd_reg_addr);

    scd_int = osal_platform_get_irq_byname(device, "scd");
    if (scd_int <= 0) {
        dev_err(&device->dev, "cannot find scd IRQ\n");
    }
    pdt_set_scd_int(scd_int);

    pxp_int = osal_platform_get_irq_byname(device, "vedu1");
    if (pxp_int <= 0) {
        dev_err(&device->dev, "cannot find vedu1 IRQ\n");
    }
    pdt_set_pxp_int(pxp_int);

    if (g_vfmw_max_chn_num > 0) {
        vfmw_set_max_chn_num(g_vfmw_max_chn_num);
    }

    vfmw_module_init();

    return 0;
}

static int vfmw_remove(struct platform_device *device)
{
    ot_unused(device);
    vfmw_module_exit();

    return 0;
}

static const struct of_device_id g_ot_vfmw_match[] = {
    { .compatible = "vendor,vfmw" },
    {},
};
MODULE_DEVICE_TABLE(of, g_ot_vfmw_match);

static struct platform_driver g_ot_vfmw_driver = {
    .probe = vfmw_probe,
    .remove = vfmw_remove,
    .driver = {
        .name = "ot_vfmw",
        .of_match_table = g_ot_vfmw_match,
    },
};

osal_module_platform_driver(g_ot_vfmw_driver);

MODULE_LICENSE("Proprietary");
