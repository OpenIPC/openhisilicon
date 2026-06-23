/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>

#include "ot_osal.h"
#include "ot_common.h"
#include "ot_vi_mod_init.h"
#include "securec.h"

#define IP_NAME_LENGTH 8
#define OF_NAME_LENGTH 10

typedef struct {
    char ip_name[IP_NAME_LENGTH];
    void (*get_ip_info)(unsigned int *irq_num, unsigned int *ip_num);
    void (*set_base_addr)(unsigned int ip_id, const void *base_addr);
    void (*set_irq_num)(unsigned int ip_id, unsigned int irq_index, unsigned int irq_num);
} ip_source;

static const ip_source g_vicap_src = {
    .ip_name       = "vi_cap",
    .get_ip_info    = vi_get_vicap_ip_info,
    .set_base_addr = vi_set_vicap_base_addr,
    .set_irq_num   = vi_set_vicap_irq_num,
};

static const ip_source g_viproc_src = {
    .ip_name       = "vi_proc",
    .get_ip_info    = vi_get_viproc_ip_info,
    .set_base_addr = vi_set_viproc_base_addr,
    .set_irq_num   = vi_set_viproc_irq_num,
};

static int vi_platform_get_source(struct platform_device *pdev, const ip_source *ip_src)
{
    unsigned int i;
    unsigned int ip_num = 0;
    unsigned int irq_num = 0;

    ip_src->get_ip_info(&irq_num, &ip_num);
    for (i = 0; i < ip_num * irq_num; i++) {
        char of_name[OF_NAME_LENGTH];
        void *base_addr = NULL;
        struct resource *mem = NULL;
        int irq_index;

        if (snprintf_s(of_name, OF_NAME_LENGTH, OF_NAME_LENGTH - 1, "%s%u", ip_src->ip_name, i) == -1) {
            osal_printk("set %s%u name err!\n", ip_src->ip_name, i);
            return -1;
        }

        if (i % irq_num == 0) {
            mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, of_name);
            base_addr = devm_ioremap_resource(&pdev->dev, mem);
            if (IS_ERR(base_addr)) {
                return PTR_ERR(base_addr);
            } else {
                ip_src->set_base_addr(i / irq_num, base_addr);
            }
        }

        irq_index = osal_platform_get_irq_byname(pdev, of_name);
        if (irq_index <= 0) {
            dev_err(&pdev->dev, "can not find %s irq number!\n", of_name);
            return -1;
        } else {
            ip_src->set_irq_num(i / irq_num, i % irq_num, (unsigned int)irq_index);
        }
    }

    return 0;
}

static int ot_vi_probe(struct platform_device *pdev)
{
    int ret;

    /* get vicap base addr and irq_index */
    ret = vi_platform_get_source(pdev, &g_vicap_src);
    if (ret) {
        return -1;
    }

    /* get viproc base addr and irq_num */
    ret = vi_platform_get_source(pdev, &g_viproc_src);
    if (ret) {
        return -1;
    }

    /* vi module init */
    ret = vi_mod_init();
    if (ret) {
        return -1;
    }

    return 0;
}

static int ot_vi_remove(struct platform_device *pdev)
{
    ot_unused(pdev);
    vi_mod_exit();
    return 0;
}

static const struct of_device_id g_ot_vi_match[] = {
    { .compatible = "vendor,vi" },
    {},
};
MODULE_DEVICE_TABLE(of, g_ot_vi_match);

static struct platform_driver g_ot_vi_driver = {
    .probe  = ot_vi_probe,
    .remove = ot_vi_remove,
    .driver = {
        .name           = "ot_vi",
        .of_match_table = g_ot_vi_match,
    },
};

osal_module_platform_driver(g_ot_vi_driver);

MODULE_LICENSE("Proprietary");
