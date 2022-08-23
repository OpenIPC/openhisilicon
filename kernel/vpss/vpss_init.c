#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>

#include "common.h"
#include "osal.h"
#include "defines.h"

extern int VPSS_ModInit(void);
extern void VPSS_ModExit(void);

extern void *g_pVpssReg[VPSS_IP_NUM];
extern int g_vpss_irq[VPSS_IP_NUM];

static int hi35xx_vpss_probe(struct platform_device *pdev)
{
	struct resource *mem;

	mem = osal_platform_get_resource(pdev, IORESOURCE_MEM, 0);
	g_pVpssReg[0] = devm_ioremap_resource(&pdev->dev, mem);

	if (IS_ERR(g_pVpssReg[0])) {
		return PTR_ERR(g_pVpssReg[0]);
	}

	g_vpss_irq[0] = osal_platform_get_irq(pdev, 0);

	//printk("++++++++++ g_pVpssReg[0] = %p g_vpss_irq[0] = %d\n",g_pVpssReg[0], g_vpss_irq[0]);

	if (g_vpss_irq[0] <= 0) {
		dev_err(&pdev->dev, "cannot find vpss IRQ\n");
	}

	VPSS_ModInit();

	return 0;
}

static int hi35xx_vpss_remove(struct platform_device *pdev)
{
	VPSS_ModExit();

	g_pVpssReg[0] = NULL;

	return 0;
}

static const struct of_device_id hi35xx_vpss_match[] = {
	{ .compatible = "hisilicon,hisi-vpss" },
	{},
};
MODULE_DEVICE_TABLE(of, hi35xx_vpss_match);

static struct platform_driver hi35xx_vpss_driver = {
        .probe          = hi35xx_vpss_probe,
        .remove         = hi35xx_vpss_remove,
        .driver         = {
                .name   = "hi35xx_vpss",
                .of_match_table = hi35xx_vpss_match,
        },
};

osal_module_platform_driver(hi35xx_vpss_driver);

MODULE_LICENSE("GPL");
