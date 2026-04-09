/******************************************************************************
Copyright (C), 2016, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : mipi_tx_init.c
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

extern unsigned int mipi_tx_irq_num;
extern void *mipi_tx_regs_va;

extern int mipi_tx_module_init(int smooth);
extern void  mipi_tx_module_exit(void);
static int smooth = 0;

static int hi35xx_mipi_tx_probe(struct platform_device *pdev)
{
    struct  resource *mem;

    mem = platform_get_resource_byname(pdev, IORESOURCE_MEM, "mipi_tx");
    mipi_tx_regs_va = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(mipi_tx_regs_va))
    {
        dev_err(&pdev->dev, "mipi_tx remap mem error.\n");
        return PTR_ERR(mipi_tx_regs_va);
    }

    mipi_tx_irq_num = platform_get_irq_byname(pdev, "mipi_tx");
    if (mipi_tx_irq_num <= 0)
    {
        dev_err(&pdev->dev, "can not find mipi_tx IRQ\n");
    }
    return mipi_tx_module_init(smooth);
}

static int hi35xx_mipi_tx_remove(struct platform_device *pdev)
{
    mipi_tx_module_exit();
    mipi_tx_regs_va = NULL;

    return 0;
}

static const struct of_device_id hi35xx_mipi_tx_match[] = {
        { .compatible = "hisilicon,hisi-mipi_tx" },
        {},
};
MODULE_DEVICE_TABLE(of, hi35xx_mipi_tx_match);

static struct platform_driver hi35xx_mipi_tx_driver = {
        .probe          = hi35xx_mipi_tx_probe,
        .remove         = hi35xx_mipi_tx_remove,
        .driver         = {
                .name   = "hi35xx_mipi_tx",
                .of_match_table = hi35xx_mipi_tx_match,
        },
};

module_param(smooth, int, S_IRUGO);
module_platform_driver(hi35xx_mipi_tx_driver);

MODULE_AUTHOR("Digital Media Team ,Hisilicon crop.");
MODULE_DESCRIPTION("Mipi_Tx Driver");
MODULE_VERSION("HI_VERSION = HISI_mipi_tx @HiMPP");
MODULE_LICENSE("GPL");
