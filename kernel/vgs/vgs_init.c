#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>

#include "common.h"
#include "osal.h"

#define VGS_DEV_NAME_LENGTH 10

extern unsigned int vgs_en[VGS_IP_NUM];
module_param_array(vgs_en, uint, NULL, S_IRUGO);

extern unsigned int max_vgs_job;
extern unsigned int max_vgs_task;
extern unsigned int max_vgs_node;
extern bool bVgsExitInSys;
module_param(max_vgs_job, uint, S_IRUGO);
module_param(max_vgs_task, uint, S_IRUGO);
module_param(max_vgs_node, uint, S_IRUGO);
module_param(bVgsExitInSys, bool, S_IRUGO);

extern void *g_pVgsReg[VGS_IP_NUM];
extern int g_vgs_irq[VGS_IP_NUM];

extern int VGS_ModInit(void);
extern void VGS_ModExit(void);

static int hi35xx_vgs_probe(struct platform_device *pdev)
{
	GK_U32 i = 0;
	GK_CHAR VgsDevName[VGS_DEV_NAME_LENGTH] = { '\0' };
	struct resource *mem = NULL;

	for (; i < VGS_IP_NUM; ++i) {
		snprintf(VgsDevName, VGS_DEV_NAME_LENGTH, "vgs%d", i);
		mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM,
							VgsDevName);
		g_pVgsReg[i] = devm_ioremap_resource(&pdev->dev, mem);

		if (IS_ERR(g_pVgsReg[i])) {
			return PTR_ERR(g_pVgsReg[i]);
		}

		g_vgs_irq[i] = osal_platform_get_irq_byname(pdev, VgsDevName);

		if (g_vgs_irq[i] <= 0) {
			dev_err(&pdev->dev, "cannot find vgs%d IRQ\n", i);
		}
	}

	return VGS_ModInit();
}

static int hi35xx_vgs_remove(struct platform_device *pdev)
{
	GK_U32 i = 0;

	VGS_ModExit();

	for (; i < VGS_IP_NUM; ++i) {
		g_pVgsReg[i] = NULL;
	}

	return 0;
}

static const struct of_device_id hi35xx_vgs_match[] = {
	{ .compatible = "hisilicon,hisi-vgs" },
	{},
};
MODULE_DEVICE_TABLE(of, hi35xx_vgs_match);

static struct platform_driver hi35xx_vgs_driver =
{
    .probe  = hi35xx_vgs_probe,
    .remove = hi35xx_vgs_remove,
    .driver = {
        .name           = "hi35xx_vgs",
        .of_match_table = hi35xx_vgs_match,
    },
};
osal_module_platform_driver(hi35xx_vgs_driver);

MODULE_LICENSE("GPL");
