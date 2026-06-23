/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>
#include "securec.h"
#include "ot_common.h"
#include "ot_osal.h"
#include "ot_dpu_rect_mod_init.h"

#define OT_DPU_RECT_NAME_LENGTH 16
#define OT_DPU_RECT_EAR_NUM 2

static int ot_dpu_rect_probe(struct platform_device *dev)
{
    td_char irq_name[OT_DPU_RECT_NAME_LENGTH] = "rect";
    td_s32 irq;
    td_u32 i;
    for (i = 0; i < OT_DPU_RECT_EAR_NUM; i++) {
        if (snprintf_s(irq_name, OT_DPU_RECT_NAME_LENGTH, OT_DPU_RECT_NAME_LENGTH - 1, "rect%u", i) == -1) {
            printk("set rect%u name err!\n", i);
            return TD_FAILURE;
        }
        irq = osal_platform_get_irq_byname(dev, irq_name);
        if (irq <= 0) {
            dev_err(&dev->dev, "can not find rect IRQ\n");
            printk("can not find rect IRQ\n");
            return TD_FAILURE;
        }
        dpu_rect_set_init_irq(i, (td_u32)irq);
    }
    return dpu_rect_init();
}

static int ot_dpu_rect_remove(struct platform_device *dev)
{
    ot_unused(dev);
    dpu_rect_exit();
    return 0;
}

static const struct of_device_id g_ot_dpu_rect_match[] = {
    { .compatible = "vendor,dpu_rect" },
    {},
};
MODULE_DEVICE_TABLE(of, g_ot_dpu_rect_match);

static struct platform_driver g_ot_dpu_rect_driver = {
    .probe = ot_dpu_rect_probe,
    .remove = ot_dpu_rect_remove,
    .driver = {
        .name = "ot_dpu_rect",
        .of_match_table = g_ot_dpu_rect_match,
    },
};

osal_module_platform_driver(g_ot_dpu_rect_driver);

MODULE_LICENSE("Proprietary");

