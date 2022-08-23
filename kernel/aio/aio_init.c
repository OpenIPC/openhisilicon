#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>

#include "type.h"
#include "osal.h"

extern int AIAO_ModInit(void);
extern void AIAO_ModExit(void);

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 9, 0)

static int __init aiao_mod_init(void)
{
	AIAO_ModInit();
	return 0;
}
static void __exit aiao_mod_exit(void)
{
	AIAO_ModExit();
}

module_init(aiao_mod_init);
module_exit(aiao_mod_exit);

MODULE_LICENSE("GPL");

#else

#include <linux/of_platform.h>

extern void *pAioBase;
EXPORT_SYMBOL(pAioBase);

extern void *pAcodec;
EXPORT_SYMBOL(pAcodec);

extern void *reg_acodec_base_va;
EXPORT_SYMBOL(reg_acodec_base_va);

extern unsigned int aiao_irq;

static int hi35xx_aiao_probe(struct platform_device *pdev)
{
	struct resource *mem;
	int aiao_irq_tmp = -1;

	mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, "aiao");
	pAioBase = devm_ioremap_resource(&pdev->dev, mem);
	if (IS_ERR(pAioBase))
		return PTR_ERR(pAioBase);

	mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, "acodec");
	pAcodec = devm_ioremap_resource(&pdev->dev, mem);
	if (IS_ERR(pAcodec))
		return PTR_ERR(pAcodec);
	reg_acodec_base_va = pAcodec;

	aiao_irq_tmp = osal_platform_get_irq_byname(pdev, "AIO");
	if (aiao_irq_tmp <= 0) {
		dev_err(&pdev->dev, "cannot find aiao IRQ\n");
	} else {
		aiao_irq = (unsigned int)aiao_irq_tmp;
	}

	AIAO_ModInit();

	return 0;
}

static int hi35xx_aiao_remove(struct platform_device *pdev)
{
	AIAO_ModExit();
	pAioBase = NULL;
	pAcodec = NULL;
	return 0;
}

static const struct of_device_id hi35xx_aiao_match[] = {
	{ .compatible = "hisilicon,hisi-aiao" },
	{},
};
MODULE_DEVICE_TABLE(of, hi35xx_aiao_match);

static struct platform_driver hi35xx_aiao_driver = {
        .probe          = hi35xx_aiao_probe,
        .remove         = hi35xx_aiao_remove,
        .driver         = {
                .name   = "hi35xx_aiao",
                .of_match_table = hi35xx_aiao_match,
        },
};

osal_module_platform_driver(hi35xx_aiao_driver);

MODULE_LICENSE("GPL");
#endif
