/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>
#include "ot_type.h"
#include "ot_common.h"
#include "ot_osal.h"
#include "ot_tde_mod_init.h"

td_u32 g_tde_tmp_buf = 1658880; /* 1658880 buffer size */
bool g_is_resize_filter = true;
td_u32 g_max_node_num = OT_TDE_DEFAULT_NODE_NUM;
td_u32 g_rgb_truncation_mode = 0; /* 0 for rgb truncation mode */
td_u32 *g_base_vir_addr_init = TD_NULL;

#ifndef MODULE
osal_setup_num_param(g_tde_tmp_buf, g_tde_tmp_buf);
osal_setup_num_param(g_is_resize_filter, g_is_resize_filter);
osal_setup_num_param(g_max_node_num, g_max_node_num);
osal_setup_num_param(g_rgb_truncation_mode, g_rgb_truncation_mode);
#else
module_param(g_tde_tmp_buf, uint, S_IRUGO);
module_param(g_is_resize_filter, bool, S_IRUGO);
module_param(g_max_node_num, uint, S_IRUGO);
module_param(g_rgb_truncation_mode, uint, S_IRUGO);
#endif

static int ot_tde_probe(struct platform_device *pdev)
{
    struct resource *mem = TD_NULL;
    td_s32 tde_irq;

    mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, "tde");
    g_base_vir_addr_init = osal_ioremap(mem->start, resource_size(mem));
    tde_hal_set_base_vir_addr(g_base_vir_addr_init);
    if (IS_ERR(g_base_vir_addr_init)) {
        return PTR_ERR(g_base_vir_addr_init);
    }

    tde_irq = osal_platform_get_irq_byname(pdev, "tde_osr_isr");
    if (tde_irq <= 0) {
        dev_err(&pdev->dev, "cannot find tde IRQ\n");
        return -1;
    }
    set_tde_irq((td_u32)tde_irq);
    if (g_max_node_num > 0) {
        tde_set_max_node_num(g_max_node_num);
    }
    tde_set_resize_filter(g_is_resize_filter);
    if ((g_rgb_truncation_mode == 0) || (g_rgb_truncation_mode == 1) ||
        (g_rgb_truncation_mode == 2)) { /* 0 1 2 for rgb truncation mode */
        tde_init_set_rgb_truncation_mode(g_rgb_truncation_mode);
    } else {
        osal_printk("set g_rgb_truncation_mode failed! mode:0~2 \n");
    }
    if (g_tde_tmp_buf > 0) {
        tde_set_tde_tmp_buffer(g_tde_tmp_buf);
    }
    if (tde_drv_mod_init() != TD_SUCCESS) {
        osal_printk("load tde.ko ....FAILED!\n");
        return -1;
    }
    osal_printk("load tde.ko ....OK!\n");
    return 0;
}

static int ot_tde_remove(struct platform_device *pdev)
{
    struct resource *mem = TD_NULL;

    ot_unused(pdev);
    tde_drv_mod_exit();
    mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, "tde");
    if (g_base_vir_addr_init != TD_NULL) {
        osal_iounmap((td_void *)g_base_vir_addr_init, resource_size(mem));
    }

    g_base_vir_addr_init = TD_NULL;
    osal_printk("unload tde.ko ....OK!\n");
    return 0;
}

static const struct of_device_id g_ot_tde_match[] = {
    { .compatible = "vendor,tde" },
    {},
};
MODULE_DEVICE_TABLE(of, g_ot_tde_match);

static struct platform_driver g_ot_tde_driver = {
    .probe = ot_tde_probe,
    .remove = ot_tde_remove,
    .driver = {
        .name = "ot_tde",
        .of_match_table = g_ot_tde_match,
    },
};

osal_module_platform_driver(g_ot_tde_driver);

MODULE_LICENSE("Proprietary");

