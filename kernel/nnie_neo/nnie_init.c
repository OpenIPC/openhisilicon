// SPDX-License-Identifier: GPL-2.0
/*
 * Platform-device wrapper for nnie_neo. Resolves the NNIE register
 * window and IRQ from the "hisilicon,hisi-nnie" device tree node and
 * hands off to nnie_std_mod_init().
 *
 * cv500-only: the NNIE block lives on cv500-family SoCs (cv500, av300,
 * dv300). V4 family (ev200/ev300/gk7205v300) has no NNIE.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>
#include "hi_common.h"
#include "hi_osal.h"
#include "../compat/kernel_compat.h"

extern int   nnie_std_mod_init(void);
extern void  nnie_std_mod_exit(void);

extern void *g_nnie_regs;
extern void *g_gdc_regs;
extern unsigned int g_nnie_irq;
extern unsigned int g_gdc_irq;
extern struct platform_device *g_nnie_pf_dev;

static int hi35xx_nnie_probe(struct platform_device *pf_dev)
{
	struct resource *mem;
	int irq;

	/* The cv500 DT node lists "nnie0" (0x11100000, our window) plus
	 * "gdc" (0x11110000, owned by open_gdc.ko). We only ioremap
	 * "nnie0" to avoid an -EBUSY clash with GDC. */
	mem = osal_platform_get_resource_byname(pf_dev, IORESOURCE_MEM, "nnie0");
	g_nnie_regs = devm_ioremap_resource(&pf_dev->dev, mem);
	if (IS_ERR((void *)g_nnie_regs))
		return PTR_ERR((const void *)g_nnie_regs);

	irq = osal_platform_get_irq_byname(pf_dev, "nnie0");
	if (irq <= 0) {
		dev_err(&pf_dev->dev, "cannot find nnie0 IRQ\n");
		return -EINVAL;
	}
	g_nnie_irq = (unsigned int)irq;

	irq = osal_platform_get_irq_byname(pf_dev, "gdc");
	g_gdc_irq  = (irq > 0) ? (unsigned int)irq : 0;
	g_gdc_regs = NULL;
	g_nnie_pf_dev = pf_dev;

	return nnie_std_mod_init();
}

static compat_platform_remove_ret hi35xx_nnie_remove(struct platform_device *pf_dev)
{
	nnie_std_mod_exit();
	g_nnie_regs   = NULL;
	g_gdc_regs    = NULL;
	g_nnie_pf_dev = NULL;

	compat_platform_remove_return;
}

static const struct of_device_id hi35xx_nnie_match[] = {
	{ .compatible = "hisilicon,hisi-nnie" },
	{},
};
MODULE_DEVICE_TABLE(of, hi35xx_nnie_match);

static struct platform_driver hi35xx_nnie_driver = {
	.probe  = hi35xx_nnie_probe,
	.remove = hi35xx_nnie_remove,
	.driver = {
		.name = "hi35xx_nnie_neo",
		.of_match_table = hi35xx_nnie_match,
	},
};

osal_module_platform_driver(hi35xx_nnie_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("nnie_neo: open-source NNIE CNN driver (Hi3516CV500/AV300)");
