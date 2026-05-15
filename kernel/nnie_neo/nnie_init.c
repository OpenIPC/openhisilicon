// SPDX-License-Identifier: GPL-2.0
/*
 * Platform-device wrapper for nnie_neo. Resolves the two NNIE
 * register windows (nnie0 @ 0x11100000 + gdc @ 0x11110000) and IRQs
 * from the "hisilicon,hisi-nnie" device tree node and hands off to
 * nnie_std_mod_init().
 *
 * cv500-only: the NNIE block lives on cv500-family SoCs (cv500, av300,
 * dv300). V4 family (ev200/ev300/gk7205v300) has no NNIE — the small
 * XNN unit on those SoCs is silicon-broken (issue #33).
 *
 * Mirrors kernel/ive_neo/ive_init.c structure so the build and DT-bind
 * paths are familiar. Symbols (g_nnie_regs, g_nnie_irq, g_gdc_irq,
 * nnie_std_mod_init/exit) are resolved by nnie_neo.c in the same
 * module.
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

extern void *g_nnie_regs;       /* nnie0 register window (0x11100000) */
extern void *g_gdc_regs;        /* gdc  register window (0x11110000) */
extern unsigned int g_nnie_irq; /* SPI 41 on cv500 per DT */
extern unsigned int g_gdc_irq;  /* SPI 42 on cv500 per DT */

static int hi35xx_nnie_probe(struct platform_device *pf_dev)
{
	struct resource *mem = NULL;
	int irq;

	/* "nnie0" — primary NNIE block registers (0x11100000 on cv500).
	 * The cv500 DT node also lists "gdc" reg @0x11110000, but that
	 * region is owned by the separate open_gdc.ko platform driver —
	 * we only need its IRQ for CNN→GDC pipeline routing in Phase 3+.
	 * Skipping the gdc ioremap avoids an -EBUSY clash on probe. */
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
	if (irq <= 0) {
		dev_warn(&pf_dev->dev, "cannot find gdc IRQ — nnie still ok\n");
		g_gdc_irq = 0;
	} else {
		g_gdc_irq = (unsigned int)irq;
	}
	g_gdc_regs = NULL;        /* gdc region owned by open_gdc.ko */

	pr_info("nnie_neo: probed nnie0=%p irq=%u  gdc_irq=%u\n",
		g_nnie_regs, g_nnie_irq, g_gdc_irq);

	return nnie_std_mod_init();
}

static compat_platform_remove_ret hi35xx_nnie_remove(struct platform_device *pf_dev)
{
	nnie_std_mod_exit();
	g_nnie_regs = NULL;
	g_gdc_regs  = NULL;

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
