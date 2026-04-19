#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>

#include "hi_common.h"
#include "hi_osal.h"
#include "../../compat/kernel_compat.h"

#define VGS_DEV_NAME_LENGTH 10

extern unsigned int vgs_en[VGS_IP_NUM];
module_param_array(vgs_en, uint, HI_NULL, S_IRUGO);

extern unsigned int max_vgs_job;
extern unsigned int max_vgs_task;
extern unsigned int max_vgs_node;
module_param(max_vgs_job,  uint, S_IRUGO);
module_param(max_vgs_task, uint, S_IRUGO);
module_param(max_vgs_node, uint, S_IRUGO);

extern void *g_vgs_reg[VGS_IP_NUM];
extern int g_vgs_irq[VGS_IP_NUM];

extern int vgs_module_init(void);
extern void vgs_module_exit(void);

static int hi35xx_vgs_probe(struct platform_device *pdev)
{
    HI_U32 i = 0;
    HI_CHAR VgsDevName[VGS_DEV_NAME_LENGTH] = {'\0'};
    struct resource *mem = HI_NULL;

    for (; i < VGS_IP_NUM; ++i) {
        snprintf(VgsDevName, VGS_DEV_NAME_LENGTH, "vgs%d", i);
        mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, VgsDevName);
        g_vgs_reg[i] = devm_ioremap_resource(&pdev->dev, mem);

        if (IS_ERR(g_vgs_reg[i])) {
            return PTR_ERR(g_vgs_reg[i]);
        }

        g_vgs_irq[i] = osal_platform_get_irq_byname(pdev, VgsDevName);

        if (g_vgs_irq[i] <= 0) {
            dev_err(&pdev->dev, "cannot find vgs%d IRQ\n", i);
        }
    }

	osal_platform_get_module_param(pdev, "max_vgs_job", int, &max_vgs_job);
	osal_platform_get_module_param(pdev, "max_vgs_task", int, &max_vgs_task);
	osal_platform_get_module_param(pdev, "max_vgs_node", int, &max_vgs_node);
    return vgs_module_init();
}

static compat_platform_remove_ret hi35xx_vgs_remove(struct platform_device *pdev)
{
    HI_U32 i = 0;

    vgs_module_exit();

    for (; i < VGS_IP_NUM; ++i) {
        g_vgs_reg[i] = HI_NULL;
    }

    compat_platform_remove_return;
}

static const struct of_device_id hi35xx_vgs_match[] = {
    { .compatible = "hisilicon,hisi-vgs" },
    {},
};
MODULE_DEVICE_TABLE(of, hi35xx_vgs_match);

static struct platform_driver hi35xx_vgs_driver = {
    .probe  = hi35xx_vgs_probe,
    .remove = hi35xx_vgs_remove,
    .driver = {
        .name           = "hi35xx_vgs",
        .of_match_table = hi35xx_vgs_match,
    },
};
osal_module_platform_driver(hi35xx_vgs_driver);

MODULE_LICENSE("GPL");
