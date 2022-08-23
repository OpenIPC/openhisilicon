#include <linux/module.h>
#include <linux/of_platform.h>

#include "defines.h"
#include "type.h"
#include "osal.h"
#include "vou_exp.h"

extern VOU_EXPORT_SYMBOL_S g_stVouExpSymbol;

EXPORT_SYMBOL(g_stVouExpSymbol);

extern int VOU_ModInit(void);
extern void VOU_ModExit(void);

extern void *g_pVoReg;
extern unsigned int g_vou_irq;

static int hi35xx_vo_probe(struct platform_device *pdev)
{
	struct resource *mem;
	int ret;
	int tmp_vou_irq;

	mem = osal_platform_get_resource(pdev, IORESOURCE_MEM, 0);
	g_pVoReg = devm_ioremap_resource(&pdev->dev, mem);
	if (IS_ERR(g_pVoReg)) {
		return PTR_ERR(g_pVoReg);
	}

	tmp_vou_irq = osal_platform_get_irq(pdev, 0);
	if (tmp_vou_irq <= 0) {
		dev_err(&pdev->dev, "cannot find vou IRQ\n");
		return GK_FAILURE;
	} else {
		g_vou_irq = (unsigned int)tmp_vou_irq;
	}

	ret = VOU_ModInit();
	if (ret) {
		return -1;
	}

	return 0;
}

static int hi35xx_vo_remove(struct platform_device *pdev)
{
	VOU_ModExit();
	g_pVoReg = NULL;
	return 0;
}

static const struct of_device_id g_hi35xx_vo_match[] = {
	{ .compatible = "hisilicon,hisi-vo" },
	{},
};
MODULE_DEVICE_TABLE(of, g_hi35xx_vo_match);

static struct platform_driver g_hi35xx_vo_driver = {
        .probe          = hi35xx_vo_probe,
        .remove         = hi35xx_vo_remove,
        .driver         = {
                .name   = "hi35xx_vo",
                .of_match_table = g_hi35xx_vo_match,
        },
};

osal_module_platform_driver(g_hi35xx_vo_driver);

MODULE_LICENSE("GPL");
