/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include <linux/of_platform.h>
#include "securec.h"
#include "ot_osal.h"
#include "ot_defines.h"
#include "ot_vgs_mod_init.h"

#define VGS_DEV_NAME_LENGTH 10

static unsigned int g_max_vgs_job = OT_VGS_DEFAULT_JOB_NUM;
static unsigned int g_max_vgs_task = OT_VGS_DEFAULT_TASK_NUM;
static unsigned int g_max_vgs_node = OT_VGS_DEFAULT_NODE_NUM;
#ifndef MODULE
osal_setup_num_param(g_max_vgs_job, g_max_vgs_job);
osal_setup_num_param(g_max_vgs_task, g_max_vgs_task);
osal_setup_num_param(g_max_vgs_node, g_max_vgs_node);
#else
module_param(g_max_vgs_job,  uint, S_IRUGO);
module_param(g_max_vgs_task, uint, S_IRUGO);
module_param(g_max_vgs_node, uint, S_IRUGO);
#endif

static int ot_vgs_device_probe(struct platform_device *pdev)
{
    td_u32 i;
    td_char vgs_dev_name[VGS_DEV_NAME_LENGTH] = {'\0'};
    struct resource *mem = TD_NULL;
    td_void *vgs_reg = TD_NULL;
    td_s32 vgs_irq;

    if (pdev == TD_NULL) {
        osal_printk("%s,%d,dev is NULL!!\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }
    for (i = 0; i < OT_VGS_IP_NUM; i++) {
        if (snprintf_s(vgs_dev_name, VGS_DEV_NAME_LENGTH, VGS_DEV_NAME_LENGTH - 1, "vgs%d", i) == -1) {
            osal_printk("set vgs%u name err!\n", i);
            /* deinitialization is unnecessary without resource allocation */
            return TD_FAILURE;
        }
        mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, vgs_dev_name);
        vgs_reg = devm_ioremap_resource(&pdev->dev, mem);
        if (IS_ERR(vgs_reg)) {
            return PTR_ERR(vgs_reg);
        }

        vgs_irq = osal_platform_get_irq_byname(pdev, vgs_dev_name);
        if (vgs_irq <= 0) {
            dev_err(&pdev->dev, "cannot find vgs%d IRQ\n", i);
        }

        if (vgs_set_init_vgs_reg(vgs_reg, i) != TD_SUCCESS) {
            return TD_FAILURE;
        }
        if (vgs_set_init_vgs_irq(vgs_irq, i) != TD_SUCCESS) {
            return TD_FAILURE;
        }
    }

    if (g_max_vgs_job > 0) {
        vgs_set_max_job_num(g_max_vgs_job);
    }
    if (g_max_vgs_task > 0) {
        vgs_set_max_task_num(g_max_vgs_task);
    }
    if (g_max_vgs_node > 0) {
        vgs_set_max_node_num(g_max_vgs_node);
    }
    return vgs_module_init(&g_max_vgs_job, &g_max_vgs_task, &g_max_vgs_node);
}

static int ot_vgs_device_remove(struct platform_device *pdev)
{
    td_u32 i;
    ot_unused(pdev);

    vgs_module_exit();

    for (i = 0; i < OT_VGS_IP_NUM; i++) {
        vgs_set_init_vgs_reg(TD_NULL, i);
    }
    return 0;
}

static const struct of_device_id g_ot_vgs_device_match[] = {
    { .compatible = "vendor,vgs" },
    {},
};
MODULE_DEVICE_TABLE(of, g_ot_vgs_device_match);

static struct platform_driver g_ot_vgs_device_driver = {
    .probe = ot_vgs_device_probe,
    .remove = ot_vgs_device_remove,
    .driver = {
        .name = "ot_vgs_device",
        .of_match_table = g_ot_vgs_device_match,
    },
};

osal_module_platform_driver(g_ot_vgs_device_driver);

MODULE_LICENSE("Proprietary");
