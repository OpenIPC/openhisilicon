/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include <linux/of_platform.h>

#include "ot_common.h"
#include "ot_osal.h"
#include "ot_gdc_mod_init.h"

static unsigned int g_max_gdc_job = OT_GDC_DEFAULT_JOB_NUM;
static unsigned int g_max_gdc_task = OT_GDC_DEFAULT_TASK_NUM;
static unsigned int g_max_gdc_node = OT_GDC_DEFAULT_NODE_NUM;

#ifndef MODULE
osal_setup_num_param(g_max_gdc_job, g_max_gdc_job);
osal_setup_num_param(g_max_gdc_task, g_max_gdc_task);
osal_setup_num_param(g_max_gdc_node, g_max_gdc_node);
#else
module_param(g_max_gdc_job,  uint, S_IRUGO);
module_param(g_max_gdc_task, uint, S_IRUGO);
module_param(g_max_gdc_node, uint, S_IRUGO);
#endif

#define GDC_DEV_NAME_LENGTH 10
#define GDC_MAX_DEV_NUM 1
static td_void *g_base_vir_addr_init[GDC_MAX_DEV_NUM] = {TD_NULL};
static td_char g_gdc_dev_name[GDC_DEV_NAME_LENGTH] = "gdc";
static int ot_gdc_probe(struct platform_device *pdev)
{
    td_u32 i;
    struct resource *mem = TD_NULL;
    td_s32 gdc_irq;

    for (i = 0; i < GDC_MAX_DEV_NUM; i++) {
        mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, g_gdc_dev_name);
        g_base_vir_addr_init[i] = osal_ioremap(mem->start, resource_size(mem));
        if (IS_ERR(g_base_vir_addr_init[i])) {
            osal_printk("devm_ioremap_resource!\n");
            return PTR_ERR(g_base_vir_addr_init[i]);
        }

        gdc_irq = osal_platform_get_irq_byname(pdev, g_gdc_dev_name);
        if (gdc_irq <= 0) {
            dev_err(&pdev->dev, "cannot find gdc%d IRQ\n", i);
        }

        if (gdc_set_init_gdc_reg(g_base_vir_addr_init[i], i) != TD_SUCCESS) {
            osal_printk("gdc_set_init_gdc_reg!\n");
            return TD_FAILURE;
        }
        if (gdc_set_init_gdc_irq(gdc_irq, i) != TD_SUCCESS) {
            osal_printk("gdc_set_init_gdc_irq!\n");
            return TD_FAILURE;
        }
    }

    if (g_max_gdc_job > 0) {
        gdc_set_max_job_num(g_max_gdc_job);
    }
    if (g_max_gdc_task > 0) {
        gdc_set_max_task_num(g_max_gdc_task);
    }
    if (g_max_gdc_node > 0) {
        gdc_set_max_node_num(g_max_gdc_node);
    }

    gdc_module_init();

    if (g_max_gdc_job < OT_GDC_MIN_JOB_NUM || g_max_gdc_job > OT_GDC_MAX_JOB_NUM) {
        g_max_gdc_job = OT_GDC_DEFAULT_JOB_NUM;
    }
    if (g_max_gdc_task < OT_GDC_MIN_TASK_NUM || g_max_gdc_task > OT_GDC_MAX_TASK_NUM) {
        g_max_gdc_task = OT_GDC_DEFAULT_TASK_NUM;
    }
    if (g_max_gdc_node < OT_GDC_MIN_NODE_NUM || g_max_gdc_node > OT_GDC_MAX_NODE_NUM) {
        g_max_gdc_node = OT_GDC_DEFAULT_NODE_NUM;
    }

    return 0;
}

static int ot_gdc_remove(struct platform_device *pdev)
{
    td_u32 i;
    struct resource *mem = TD_NULL;
    gdc_module_exit();

    ot_unused(pdev);
    for (i = 0; i < OT_GDC_IP_NUM; i++) {
        mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, g_gdc_dev_name);
        if (g_base_vir_addr_init[i] != TD_NULL) {
            osal_iounmap((td_void *)g_base_vir_addr_init[i], resource_size(mem));
        }
        g_base_vir_addr_init[i] = TD_NULL;
        gdc_set_init_gdc_reg(TD_NULL, i);
    }

    return 0;
}


static const struct of_device_id g_ot_gdc_match[] = {
    { .compatible = "vendor,gdc" },
    {},
};
MODULE_DEVICE_TABLE(of, g_ot_gdc_match);

static struct platform_driver g_ot_gdc_driver = {
    .probe          = ot_gdc_probe,
    .remove         = ot_gdc_remove,
    .driver         = {
        .name   = "ot_gdc",
        .of_match_table = g_ot_gdc_match,
    },
};

osal_module_platform_driver(g_ot_gdc_driver);

MODULE_LICENSE("Proprietary");
