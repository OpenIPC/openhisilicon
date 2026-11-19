/******************************************************************************
Copyright (C), 2016, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : mipi_rx_init.c
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2016/11/15
Last Modified :
Description   :
Function List :
******************************************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>
#include <linux/reboot.h>
#include <linux/notifier.h>
#include <linux/io.h>
#include <linux/delay.h>

#include "../compat/compat.h"

extern unsigned int mipi_rx_irq_num;
extern void *mipi_rx_regs_va;

extern int mipi_rx_mod_init(void);
extern void mipi_rx_mod_exit(void);

/*
 * Issue #159: when machine_restart fires with active video streaming on
 * hi3516ev300, the MIPI RX / VI_CAP DMA path can still be live, and the
 * SoC reset triggered by hisi_restart_handler races against in-flight
 * transactions. The result is a hang in the next kernel's pre-init phase.
 *
 * Assert the MIPI RX bus soft-reset (MIPI_RX_CRG_ADDR=0x120100F8, bit 6
 * = mipi_bus_srst_req; see kernel/mipi_rx/mipi_rx_hal.c) from a reboot
 * notifier that runs in kernel_restart_prepare(), BEFORE device_shutdown.
 *
 * Empirical: this stops the source of the video pipeline; downstream
 * blocks (VI_CAP, VPSS, VEDU) drain naturally. Stress on hi3516ev300
 * dlab dropped the hang rate from ~25% baseline to ~10%; combining
 * with clock-gate clears made it WORSE (50%), so we only assert the
 * soft-reset and leave clocks alone.
 *
 * TODO(issue #159): move this notifier into open_osal once the approach
 * is validated; osal is the centralised place but cannot be live-replaced
 * on a running camera (refcount > 60), so we keep it here for iteration
 * agility.
 */
#define HI3516EV300_MIPI_RX_CRG_ADDR 0x120100F8
#define MIPI_BUS_SRST_BIT            (1U << 6)

static int hi35xx_mipi_rx_reboot_notify(struct notifier_block *nb,
					unsigned long action, void *data)
{
	void __iomem *crg;
	u32 v;

	if (action != SYS_RESTART && action != SYS_POWER_OFF &&
	    action != SYS_HALT)
		return NOTIFY_DONE;

	crg = ioremap(HI3516EV300_MIPI_RX_CRG_ADDR, 4);
	if (!crg)
		return NOTIFY_DONE;
	v = readl(crg);
	writel(v | MIPI_BUS_SRST_BIT, crg);
	wmb();
	/* Let the soft-reset propagate before machine_restart fires. */
	mdelay(2);
	iounmap(crg);
	return NOTIFY_DONE;
}

static struct notifier_block hi35xx_mipi_rx_reboot_nb = {
	.notifier_call = hi35xx_mipi_rx_reboot_notify,
};

static int hi35xx_mipi_rx_probe(struct platform_device *pdev)
{
	struct resource *mem;
	int ret;

	mem = platform_get_resource_byname(pdev, IORESOURCE_MEM, "mipi_rx");
	mipi_rx_regs_va = devm_ioremap_resource(&pdev->dev, mem);
	if (IS_ERR(mipi_rx_regs_va)) {
		dev_err(&pdev->dev, "mipi_rx remap mem error.\n");
		return PTR_ERR(mipi_rx_regs_va);
	}

	mipi_rx_irq_num = platform_get_irq_byname(pdev, "mipi_rx");
	if (mipi_rx_irq_num <= 0) {
		dev_err(&pdev->dev, "can not find mipi_rx IRQ\n");
	}

	ret = mipi_rx_mod_init();
	if (ret)
		return ret;

	register_reboot_notifier(&hi35xx_mipi_rx_reboot_nb);
	return 0;
}

static compat_platform_remove_ret hi35xx_mipi_rx_remove(struct platform_device *pdev)
{
	unregister_reboot_notifier(&hi35xx_mipi_rx_reboot_nb);
	mipi_rx_mod_exit();
	mipi_rx_regs_va = NULL;

	compat_platform_remove_return;
}

static const struct of_device_id hi35xx_mipi_rx_match[] = {
	{ .compatible = PLATFORM_NAME "," HISI_PRX "mipi" },
	{},
};
MODULE_DEVICE_TABLE(of, hi35xx_mipi_rx_match);

static struct platform_driver hi35xx_mipi_rx_driver = {
        .probe          = hi35xx_mipi_rx_probe,
        .remove         = hi35xx_mipi_rx_remove,
        .driver         = {
                .name   = "mipi_rx",
                .of_match_table = hi35xx_mipi_rx_match,
        },
};

module_platform_driver(hi35xx_mipi_rx_driver);

MODULE_AUTHOR("Digital Media Team ,Hisilicon crop.");
MODULE_DESCRIPTION("Mipi_Rx Driver");
MODULE_VERSION("HI_VERSION = HISI_mipi @HiMPP");
MODULE_LICENSE("GPL");
