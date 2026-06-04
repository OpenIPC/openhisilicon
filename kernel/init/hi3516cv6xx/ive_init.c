/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>
#include "securec.h"
#include "ot_ive_mod_init.h"
#include "ot_common.h"
#include "ot_osal.h"
#include "sys_ext.h"

#define OT_IVE_NAME_LENGTH 10
static td_u8 g_power_save_en = 1;
static td_u16 g_node_num = OT_IVE_DEFAULT_NODE_NUM;
static td_void *g_mod_reg = TD_NULL;

#ifndef MODULE
osal_setup_num_param(save_power, g_power_save_en);
osal_setup_num_param(max_node_num, g_node_num);
#else
module_param_named(save_power, g_power_save_en, byte, S_IRUGO);
module_param_named(max_node_num, g_node_num, ushort, S_IRUGO);
#endif

static int ot_ive_probe(struct platform_device *pf_dev)
{
    struct resource *mem = TD_NULL;
    ot_ive_mod_param mod_param;
    td_char dev_name[OT_IVE_NAME_LENGTH] = { "ive" };
    td_s32 irq, ret;
    td_u32 dev_id = 0;
    td_u32 reg_id = 1;

    mem = osal_platform_get_resource_byname(pf_dev, IORESOURCE_MEM, dev_name);
    g_mod_reg = osal_ioremap(mem->start, resource_size(mem));
    if (IS_ERR((void *)g_mod_reg)) {
        return PTR_ERR((const void *)g_mod_reg);
    }
    irq = osal_platform_get_irq_byname(pf_dev, "ive0");
    if (irq <= 0) {
        dev_err(&pf_dev->dev, "cannot find ive IRQ\n");
        printk("cannot find ive IRQ\n");
        return TD_FAILURE;
    }
    ret = ive_set_init_ive_irq(reg_id, (td_u32)irq);
    if (ret != TD_SUCCESS) {
        printk("ive_set_init_ive_irq failed!\n");
        return TD_FAILURE;
    }
    ret = ive_set_init_ive_reg(dev_id, (td_void *)g_mod_reg);
    if (ret != TD_SUCCESS) {
        printk("ive_set_init_ive_reg failed!\n");
        return TD_FAILURE;
    }
    mod_param.power_save_en = g_power_save_en;
    mod_param.mod_node_num = g_node_num;

    return ive_std_mod_init(&mod_param);
}

static int ot_ive_remove(struct platform_device *pf_dev)
{
    struct resource *mem = TD_NULL;
    td_char dev_name[OT_IVE_NAME_LENGTH] = { "ive" };
    ot_unused(pf_dev);

    mem = osal_platform_get_resource_byname(pf_dev, IORESOURCE_MEM, dev_name);
    ive_std_mod_exit();
    if (g_mod_reg != TD_NULL && mem != TD_NULL) {
        osal_iounmap((td_void *)g_mod_reg, resource_size(mem));
    }
    g_mod_reg = TD_NULL;
    return 0;
}

static const struct of_device_id g_ot_ive_match[] = {
    { .compatible = "vendor,ive" },
    {},
};
MODULE_DEVICE_TABLE(of, g_ot_ive_match);

static struct platform_driver g_ot_ive_driver = {
    .probe = ot_ive_probe,
    .remove = ot_ive_remove,
    .driver = {
        .name = "ot_ive",
        .of_match_table = g_ot_ive_match,
    },
};

static int __init ot_ive_driver_init(void)
{
    td_s32 ret;
    if (!ckfn_sys_entry()) {
        OT_PRINT("no sys ko!\n");
        return TD_FAILURE;
    }
    ret = ive_svp_npu_load_all_model();
    if (ret != TD_SUCCESS) {
        printk("ive_svp_npu_load_all_model failed!\n");
        return TD_FAILURE;
    }
    return osal_platform_driver_register(&g_ot_ive_driver);
}
module_init(ot_ive_driver_init);

static void __exit ot_ive_driver_exit(void)
{
    ive_svp_npu_unload_all_model();
    return osal_platform_driver_unregister(&g_ot_ive_driver);
}
module_exit(ot_ive_driver_exit);

MODULE_LICENSE("Proprietary");
