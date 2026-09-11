/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>

#include "../compat/kernel_compat.h"
#include "watchdog.h"

extern int default_margin;
module_param(default_margin, int, 0);
MODULE_PARM_DESC(
	default_margin,
	"Watchdog default_margin in seconds. (default=" __MODULE_STRING(
		DOG_TIMER_MARGIN) ")");

//extern int nowayout;
//module_param(nowayout, int, 0);
//MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started (default=CONFIG_WATCHDOG_NOWAYOUT)");

extern int nodeamon;
module_param(nodeamon, int, 0);
MODULE_PARM_DESC(
	nodeamon,
	"By default, a kernel deamon feed watchdog when idle, set 'nodeamon=1' to disable this. (default=0)");

extern volatile void *gpWtdgAllReg;

extern int watchdog_init(void);
extern void watchdog_exit(void);

/*
 * Two ways in, for one driver.
 *
 * The generations that describe the watchdog in the device tree bind as a
 * platform driver and take the register mapping from the node, which is why
 * WDT_BASE is only a fallback. The older parts (the 0x20040000 map:
 * hi3516cv100/cv200, hi3516av100, hi3520dv200) have no such node, so their
 * kbuild defines WDT_NO_OF and the module simply registers on load and lets
 * watchdog_init() ioremap WDT_BASE itself.
 */
#ifdef WDT_NO_OF

/* Wrappers rather than module_init(watchdog_init): the alias the macro builds
 * has to resolve inside this object, and the driver itself lives in wdt.c. */
static int __init wdg_mod_init(void)
{
	return watchdog_init();
}

static void __exit wdg_mod_exit(void)
{
	watchdog_exit();
}

module_init(wdg_mod_init);
module_exit(wdg_mod_exit);

#else

static int wdg_probe(struct platform_device *pdev)
{
	struct resource *mem;

	mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	gpWtdgAllReg = devm_ioremap_resource(&pdev->dev, mem);
	if (IS_ERR((void *)gpWtdgAllReg))
		return PTR_ERR((void *)gpWtdgAllReg);

	return watchdog_init();
}

static compat_platform_remove_ret wdg_remove(struct platform_device *pdev)
{
	watchdog_exit();
	compat_platform_remove_return;
}

/*
 * Every spelling the watchdog node carries across the families served here:
 * "hisilicon,hi_wdg" and "hisilicon,hi-wdg" on the HiSilicon trees, "goke,wdg"
 * on the Goke ones. of_match_table is what binds, so listing all of them costs
 * nothing and is what lets one wrapper cover every DT part. Naming them
 * outright rather than building one from compat.h's PLATFORM_NAME is also what
 * keeps this file usable outside the two chips that header knows about.
 */
static const struct of_device_id wdg_match[] = {
	{ .compatible = "hisilicon,hi_wdg" },
	{ .compatible = "hisilicon,hi-wdg" },
	{ .compatible = "goke,wdg" },
	{},
};

static struct platform_driver wdg_driver = {
    .probe  = wdg_probe,
    .remove = wdg_remove,
    .driver =  { .name = "wdg",
                .of_match_table = wdg_match,
               },
};

module_platform_driver(wdg_driver);

#endif

MODULE_LICENSE("GPL");
