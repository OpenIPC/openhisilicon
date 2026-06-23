/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include <linux/of_platform.h>
#include "securec.h"
#include "ot_defines.h"
#include "ot_osal.h"
#include "ot_common.h"
#include "ot_jpegd_mod_init.h"

#define OT_JPEGD_IP_NAME_LENGTH 10
#define OT_JPEGD_IRQ_NAME_LENGTH 10

static td_void* g_jpegd_reg_addr[OT_JPEGD_IP_NUM];
static td_s32 g_jpegd_irq[OT_JPEGD_IP_NUM];
static td_ulong g_jpegd_reg_size;

static td_s32 jpegd_dev_init(struct platform_device* pdev)
{
    td_u32 i;
    td_char jpegd_ip_name[OT_JPEGD_IP_NAME_LENGTH] = {'\0'};
    td_char jpegd_isr_name[OT_JPEGD_IRQ_NAME_LENGTH] = {'\0'};
    struct resource* mem = TD_NULL;
    td_s32 irq_num;

    for (i = 0; i < OT_JPEGD_IP_NUM; i++) {
        if (snprintf_s(jpegd_ip_name, OT_JPEGD_IP_NAME_LENGTH, OT_JPEGD_IP_NAME_LENGTH - 1, "jpegd") == -1) {
            osal_printk("jpegd config ip name failed\n");
            return TD_FAILURE;
        }
        if (snprintf_s(jpegd_isr_name, OT_JPEGD_IRQ_NAME_LENGTH, OT_JPEGD_IRQ_NAME_LENGTH - 1, "jpegd%d", i) == -1) {
            osal_printk("jpegd config irq name failed\n");
            return TD_FAILURE;
        }
        mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, jpegd_ip_name);
        g_jpegd_reg_size = resource_size(mem);
        g_jpegd_reg_addr[i] = osal_ioremap(mem->start, g_jpegd_reg_size);

        if (IS_ERR(g_jpegd_reg_addr[i])) {
            osal_printk("devm_ioremap_resource fail for %ld\n", PTR_ERR(g_jpegd_reg_addr[i]));
            return PTR_ERR(g_jpegd_reg_addr[i]);
        }

        irq_num = osal_platform_get_irq_byname(pdev, jpegd_isr_name);
        if (irq_num <= 0) {
            dev_err(&pdev->dev, "cannot find jpegd%d IRQ\n", i);
            return TD_FAILURE;
        }

        g_jpegd_irq[i] = (td_u32)irq_num;
    }
    jpegd_set_reg_addr(g_jpegd_reg_addr, OT_JPEGD_IP_NUM);
    jpegd_set_jpegd_irq(g_jpegd_irq, OT_JPEGD_IP_NUM);
    return 0;
}

static td_void jpegd_dev_exit(td_void)
{
    td_u32 i;

    for (i = 0; i < OT_JPEGD_IP_NUM; i++) {
        if (g_jpegd_reg_addr[i] != NULL) {
            osal_iounmap(g_jpegd_reg_addr[i], g_jpegd_reg_size);
            g_jpegd_reg_addr[i] = NULL;
        }
    }
    jpegd_set_reg_addr(g_jpegd_reg_addr, OT_JPEGD_IP_NUM);
}

static int ot_jpegd_probe(struct platform_device* pdev)
{
    td_s32 ret;

    if (pdev == TD_NULL) {
        osal_printk("%s, %d, dev is NULL!!\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    ret = jpegd_dev_init(pdev);
    if (ret != TD_SUCCESS) {
        osal_printk("jpegd_register_dev fail!\n");
        return ret;
    }

    ret = jpegd_module_init();

    return ret;
}

static int ot_jpegd_remove(struct platform_device* pdev)
{
    ot_unused(pdev);
    jpegd_module_exit();
    jpegd_dev_exit();

    return TD_SUCCESS;
}

static const struct of_device_id g_ot_jpegd_match[] = {
    { .compatible = "vendor,jpegd" },
    {},
};
MODULE_DEVICE_TABLE(of, g_ot_jpegd_match);

static struct platform_driver g_ot_jpegd_driver = {
    .probe          = ot_jpegd_probe,
    .remove         = ot_jpegd_remove,
    .driver         = {
        .name           = "ot_jpegd",
        .of_match_table = g_ot_jpegd_match,
    },
};

osal_module_platform_driver(g_ot_jpegd_driver);

MODULE_LICENSE("Proprietary");
