#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/clk.h>
#include <linux/version.h>
#include <linux/of_platform.h>

#include "hi_type.h"
#include "hi_osal_init.h"

extern int AIAO_ModInit(void);
extern void AIAO_ModExit(void);

extern void* pAioBase;
EXPORT_SYMBOL(pAioBase);

extern void* pAcodec;
EXPORT_SYMBOL(pAcodec);

extern unsigned int aiao_irq;

struct drv_data
{
    struct clk* aiao_clk;
};

static int aiao_hw_init(struct platform_device* pdev)
{
    struct drv_data* drv_priv_data;

    drv_priv_data = platform_get_drvdata(pdev);

    osal_clk_prepare_enable(drv_priv_data->aiao_clk);

    return 0;
}

static void aiao_hw_exit(struct platform_device* pdev)
{
    struct drv_data* drv_priv_data;

    drv_priv_data = platform_get_drvdata(pdev);

    osal_clk_disable_unprepare(drv_priv_data->aiao_clk);
}

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static int hi35xx_aiao_suspend(struct platform_device* pdev, pm_message_t state)
{
    //aiao_hw_exit(pdev);
    return 0;
}

static int hi35xx_aiao_resume(struct platform_device* pdev)
{
    //osal_pinctrl_pm_select_default_state(&pdev->dev);
    //aiao_hw_init(pdev);
    return 0;
}
#endif


static int hi35xx_aiao_probe(struct platform_device* pdev)
{
    struct resource* mem;
    struct drv_data* drv_priv_data;
    struct device_node* np = pdev->dev.of_node;

    drv_priv_data = osal_devm_kzalloc(&pdev->dev, sizeof(*drv_priv_data));

    if (!drv_priv_data)
    {
        dev_err(&pdev->dev, "could not allocate vedu driver private data.\n");
        return -ENOMEM;
    }

    platform_set_drvdata(pdev, drv_priv_data);

    mem = osal_platform_get_resource(pdev, IORESOURCE_MEM, 0);
    pAioBase = devm_ioremap_resource(&pdev->dev, mem);

    if (IS_ERR(pAioBase))
    { return PTR_ERR(pAioBase); }

    mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, "acodec");

    if (NULL != mem)
    {
        pAcodec = devm_ioremap_resource(&pdev->dev, mem);

        if (IS_ERR(pAcodec))
        { return PTR_ERR(pAcodec); }
    }

    aiao_irq = osal_platform_get_irq(pdev, 0);

    if (aiao_irq <= 0)
    {
        dev_err(&pdev->dev, "cannot find aiao IRQ\n");
    }

    drv_priv_data->aiao_clk = of_clk_get(np, 0);

    if (IS_ERR(drv_priv_data->aiao_clk))
    {
        dev_err(&pdev->dev, "aiao cannot get clock\n");
        return -ENOENT;
    }

    aiao_hw_init(pdev);

    AIAO_ModInit();

    return 0;
}

static compat_platform_remove_ret hi35xx_aiao_remove(struct platform_device* pdev)
{
    AIAO_ModExit();
    pAioBase = HI_NULL;
    pAcodec = HI_NULL;
    aiao_hw_exit(pdev);

    compat_platform_remove_return;
}


static const struct of_device_id hi35xx_aiao_match[] =
{
    { .compatible = "hisilicon,hi35xx_aiao" },
    {},
};
MODULE_DEVICE_TABLE(of, hi35xx_aiao_match);

static struct platform_driver hi35xx_aiao_driver =
{
    .probe          = hi35xx_aiao_probe,
    .remove         = hi35xx_aiao_remove,
    .driver         = {
        .name   = "hi35xx_aiao",
        .of_match_table = hi35xx_aiao_match,
    },

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
    .suspend		= hi35xx_aiao_suspend,
    .resume 		= hi35xx_aiao_resume,
#endif
};

osal_module_platform_driver(hi35xx_aiao_driver);

MODULE_LICENSE("Proprietary");
