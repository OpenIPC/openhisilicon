// SPDX-License-Identifier: GPL-2.0
/*
 * Platform-device wrapper for ive_neo. Resolves the IVE register
 * window and IRQ from the "hisilicon,hisi-ive" device tree node and
 * hands off to ive_std_mod_init().
 *
 * The vendor V4 and cv500 ive_init.c files differ only in how they
 * spell their OSAL header (osal.h vs hi_osal.h) and in whether they
 * use the gk_ or hi_ type-alias families. This open implementation
 * uses plain Linux kernel types throughout and picks the OSAL header
 * per CHIPARCH so the same source tree builds for both families.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>

#if defined(hi3516cv500)
#  include "hi_common.h"
#  include "hi_osal.h"
#else
#  include "common.h"
#  include "osal.h"
#endif

extern unsigned char g_ive_power_save_en;
extern unsigned short g_ive_node_num;

extern int ive_std_mod_init(void);
extern void ive_std_mod_exit(void);

module_param_named(save_power, g_ive_power_save_en, byte, S_IRUGO);
module_param_named(max_node_num, g_ive_node_num, ushort, S_IRUGO);

#define HI_IVE_DEV_NAME_LENGTH 10
extern void *g_ive_regs;
extern unsigned int g_ive_irq;

static int hi35xx_ive_probe(struct platform_device *pf_dev)
{
	struct resource *mem = NULL;
	char dev_name[HI_IVE_DEV_NAME_LENGTH] = { "ive" };
	int irq;

	mem = osal_platform_get_resource_byname(pf_dev, IORESOURCE_MEM,
						dev_name);
	g_ive_regs = devm_ioremap_resource(&pf_dev->dev, mem);
	if (IS_ERR((void *)g_ive_regs)) {
		return PTR_ERR((const void *)g_ive_regs);
	}

	irq = osal_platform_get_irq_byname(pf_dev, dev_name);
	if (irq <= 0) {
		dev_err(&pf_dev->dev, "cannot find ive IRQ\n");
		printk("cannot find ive IRQ\n");
	}
	g_ive_irq = (unsigned int)irq;

	return ive_std_mod_init();
}

static compat_platform_remove_ret hi35xx_ive_remove(struct platform_device *pf_dev)
{
	ive_std_mod_exit();
	g_ive_regs = NULL;

	compat_platform_remove_return;
}

static const struct of_device_id hi35xx_ive_match[] = {
	{ .compatible = "hisilicon,hisi-ive" },
	{},
};
MODULE_DEVICE_TABLE(of, hi35xx_ive_match);

static struct platform_driver hi35xx_ive_driver = {
    .probe = hi35xx_ive_probe,
    .remove = hi35xx_ive_remove,
    .driver = {
        .name = "hi35xx_ive",
        .of_match_table = hi35xx_ive_match,
    },
};

osal_module_platform_driver(hi35xx_ive_driver);

MODULE_LICENSE("GPL");
