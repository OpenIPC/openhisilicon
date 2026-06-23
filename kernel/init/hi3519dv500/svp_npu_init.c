/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <asm/io.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>
#include "ot_common.h"
#include "ot_osal.h"
#include "securec.h"
#include "ot_svp_npu_mod_init.h"

#define SVP_NPU_DEV_NAME_LENGTH 16
#define SVP_NPU_DEV_NUM 1

static td_u8 g_power_save_en = 1;
static td_u16 g_max_stream_num = OT_SVP_NPU_DEF_STREAM_NUM;
static td_u16 g_max_task_node_num = OT_SVP_NPU_DEF_TASK_NODE_NUM;
static td_u16 g_max_event_num = OT_SVP_NPU_DEF_EVENT_NUM;
static td_u32 g_prof_buf_size = OT_SVP_NPU_DEF_PROF_MEM_KB;

static td_u32 *g_svp_npu_init_reg[SVP_NPU_DEV_NUM] = { TD_NULL };
static td_u32 g_svp_npu_init_irq[SVP_NPU_DEV_NUM] = { 0 };

static td_u32 *g_pres_dec_brg_init_reg = TD_NULL;
#ifndef MODULE
osal_setup_num_param(svp_npu_save_power, g_power_save_en);
osal_setup_num_param(svp_npu_max_stream_num, g_max_stream_num);
osal_setup_num_param(svp_npu_max_task_node_num, g_max_task_node_num);
osal_setup_num_param(svp_npu_max_event_num, g_max_event_num);
osal_setup_num_param(svp_npu_prof_buf_size, g_prof_buf_size);
#else
module_param_named(svp_npu_save_power, g_power_save_en, byte, S_IRUGO);
module_param_named(svp_npu_max_stream_num, g_max_stream_num, ushort, S_IRUGO);
module_param_named(svp_npu_max_task_node_num, g_max_task_node_num, ushort, S_IRUGO);
module_param_named(svp_npu_max_event_num, g_max_event_num, ushort, S_IRUGO);
module_param_named(svp_npu_prof_buf_size, g_prof_buf_size, uint, S_IRUGO);
#endif

static int svp_npu_probe_pres(struct platform_device *pf_dev)
{
    td_s32 ret;
    td_char svp_npu_dev_name[SVP_NPU_DEV_NAME_LENGTH] = { '\0' };
    struct resource *mem = TD_NULL;
    td_void *reg = TD_NULL;

    ret = snprintf_s(svp_npu_dev_name, SVP_NPU_DEV_NAME_LENGTH, SVP_NPU_DEV_NAME_LENGTH - 1, "pres_dec_brg");
    if ((ret < 0) || (ret > (SVP_NPU_DEV_NAME_LENGTH - 1))) {
        printk("snprintf_s npu name failed\n");
        return TD_FAILURE;
    }
    mem = osal_platform_get_resource_byname(pf_dev, IORESOURCE_MEM, svp_npu_dev_name);
    reg = devm_ioremap_resource(&pf_dev->dev, mem);
    if (IS_ERR(reg)) {
        printk("devm_ioremap_resource failed\n");
        return PTR_ERR(reg);
    }

    g_pres_dec_brg_init_reg = (td_u32 *)reg;
    if (svp_npu_pres_dec_set_init_reg(g_pres_dec_brg_init_reg) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static int svp_npu_probe(struct platform_device *pf_dev)
{
    td_s32 irq, ret;
    td_char svp_npu_dev_name[SVP_NPU_DEV_NAME_LENGTH] = { '\0' };
    struct resource *mem = TD_NULL;
    ot_svp_npu_mod_param mod_param;
    td_void *reg = TD_NULL;

    ret = snprintf_s(svp_npu_dev_name, SVP_NPU_DEV_NAME_LENGTH, SVP_NPU_DEV_NAME_LENGTH - 1, "svp_npu");
    if ((ret < 0) || (ret > (SVP_NPU_DEV_NAME_LENGTH - 1))) {
        printk("snprintf_s npu name failed\n");
        return TD_FAILURE;
    }
    mem = osal_platform_get_resource_byname(pf_dev, IORESOURCE_MEM, svp_npu_dev_name);
    reg = devm_ioremap_resource(&pf_dev->dev, mem);
    if (IS_ERR(reg)) {
        printk("devm_ioremap_resource failed\n");
        return PTR_ERR(reg);
    }

    irq = osal_platform_get_irq_byname(pf_dev, "svp_npu_ns0");
    if (irq <= 0) {
        dev_err(&pf_dev->dev, "can't find npu IRQ\n");
        return TD_FAILURE;
    }
    g_svp_npu_init_irq[0] = (td_u32)irq;
    g_svp_npu_init_reg[0] = (td_u32 *)reg;

    if (svp_npu_set_init_reg(g_svp_npu_init_reg, SVP_NPU_DEV_NUM) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    if (svp_npu_set_init_irq(g_svp_npu_init_irq, SVP_NPU_DEV_NUM) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    if (svp_npu_probe_pres(pf_dev) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    mod_param.power_save_en = g_power_save_en;
    mod_param.max_stream_num = g_max_stream_num;
    mod_param.max_task_node_num = g_max_task_node_num;
    mod_param.max_event_num = g_max_event_num;
    mod_param.prof_buf_size = g_prof_buf_size;
    (td_void)svp_npu_set_mod_param(&mod_param);

    return svp_npu_mod_init();
}

static int svp_npu_remove(struct platform_device *pf_dev)
{
    td_u32 i;

    ot_unused(pf_dev);
    svp_npu_mod_exit();
    for (i = 0; i < SVP_NPU_DEV_NUM; i++) {
        g_svp_npu_init_reg[i] = TD_NULL;
    }
    g_pres_dec_brg_init_reg = TD_NULL;
    return 0;
}

static const struct of_device_id g_svp_npu_match[] = {
    { .compatible = "vendor,svp_npu" },
    {},
};
MODULE_DEVICE_TABLE(of, g_svp_npu_match);

static struct platform_driver g_svp_npu_driver = {
    .probe          = svp_npu_probe,
    .remove         = svp_npu_remove,
    .driver         = {
        .name   = "svp_npu",
        .of_match_table = g_svp_npu_match,
    },
};

osal_module_platform_driver(g_svp_npu_driver);

MODULE_LICENSE("Proprietary");
