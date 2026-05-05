#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>

#include "hi_common.h"
#include "hi_osal.h"
#include "../../compat/kernel_compat.h"

extern unsigned int gdc_en[GDC_IP_NUM];
extern HI_U32 max_gdc_job;
extern HI_U32 max_gdc_task;
extern HI_U32 max_gdc_node;

module_param_array(gdc_en, uint, HI_NULL, S_IRUGO);
module_param(max_gdc_job,  uint, S_IRUGO);
module_param(max_gdc_task, uint, S_IRUGO);
module_param(max_gdc_node, uint, S_IRUGO);

extern int  gdc_module_init(void);
extern void gdc_module_exit(void);

extern void* g_gdc_reg[2];
extern unsigned int g_gdc_irq[2];
extern unsigned int g_gdc_nnie_irq[1];

#define GDC_DEV_NAME_LENGTH 10
#define GDC_MAX_DEV_NUM 1

static int hi35xx_gdc_probe(struct platform_device* pdev)
{
    HI_U32 i;
    HI_CHAR GdcDevName[GDC_DEV_NAME_LENGTH] = "gdc";
    HI_CHAR NnieDevName[GDC_DEV_NAME_LENGTH] = "nnie0";
    struct resource *mem;

    for(i = 0; i < GDC_MAX_DEV_NUM; i++)
    {
        mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, GdcDevName);
        g_gdc_reg[i] = devm_ioremap_resource(&pdev->dev, mem);
        if (IS_ERR(g_gdc_reg[i]))
            return PTR_ERR(g_gdc_reg[i]);

        g_gdc_irq[i] = osal_platform_get_irq_byname(pdev, GdcDevName);
        if (g_gdc_irq[i] <= 0)
        {
            dev_err(&pdev->dev, "cannot find gdc%d IRQ\n",i);
        }
    }

    g_gdc_nnie_irq[0] = osal_platform_get_irq_byname(pdev, NnieDevName);
    if(g_gdc_nnie_irq[0] <= 0)
    {
        dev_err(&pdev->dev, "cannot find nnie0 IRQ\n");
    }

    gdc_module_init();

	osal_platform_get_module_param(pdev, "max_gdc_job", int, &max_gdc_job);
	osal_platform_get_module_param(pdev, "max_gdc_task", int, &max_gdc_task);
	osal_platform_get_module_param(pdev, "max_gdc_node", int, &max_gdc_node);
    return 0;
}

static compat_platform_remove_ret hi35xx_gdc_remove(struct platform_device* pdev)
{
    HI_U32 i;

    gdc_module_exit();

    for (i = 0; i < GDC_MAX_DEV_NUM; i++)
    {
        g_gdc_reg[0] = HI_NULL;
    }

    compat_platform_remove_return;
}


static const struct of_device_id hi35xx_gdc_match[] =
{
    { .compatible = "hisilicon,hisi-gdc" },
    {},
};
MODULE_DEVICE_TABLE(of, hi35xx_gdc_match);

static struct platform_driver hi35xx_gdc_driver =
{
    .probe          = hi35xx_gdc_probe,
    .remove         = hi35xx_gdc_remove,
    .driver         = {
        .name   = "hi35xx_gdc",
        .of_match_table = hi35xx_gdc_match,
    },
};

osal_module_platform_driver(hi35xx_gdc_driver);

MODULE_LICENSE("GPL");



