#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/clk.h>
#include <linux/version.h>
#include <linux/of_platform.h>

#include "hi_common.h"
#include "hi_osal_init.h"

#define VEDU_CLK_DEFAULT_FREQ 198000000 //198M

unsigned int vedu_clk_frequency = VEDU_CLK_DEFAULT_FREQ;
module_param(vedu_clk_frequency, uint, S_IRUGO);

extern void *s_apVeduAddr[VEDU_NUM];
extern unsigned int vedu_irq[VEDU_NUM];

extern int VEDU_ModInit(void);
extern void VEDU_ModExit(void);

struct drv_data {
	struct clk *vedu_clk[VEDU_NUM];
	HI_U32 vedu_init_clk_freq[VEDU_NUM];
	HI_U32 vedu_clk_freq[VEDU_NUM];
};

static int vedu_hw_init(struct platform_device *pdev)
{
	HI_U32 clk_freq;
	struct drv_data *drv_priv_data;
	HI_S32 ret;
	
	drv_priv_data = platform_get_drvdata(pdev);

	ret = osal_clk_prepare_enable(drv_priv_data->vedu_clk[0]);
	if (ret)
	{
		printk("clk enable failed.\n");
	}
	
	clk_freq = drv_priv_data->vedu_clk_freq[0];
	osal_clk_set_rate(drv_priv_data->vedu_clk[0], clk_freq);

	return 0;
}

static void vedu_hw_exit(struct platform_device *pdev)
{
	struct drv_data *drv_priv_data;

	drv_priv_data = platform_get_drvdata(pdev);

	osal_clk_disable_unprepare(drv_priv_data->vedu_clk[0]);
}

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static int hi35xx_vedu_suspend(struct platform_device *pdev, pm_message_t state)
{
	HI_U32 clk_freq;
	struct drv_data *drv_priv_data;
	
	drv_priv_data = platform_get_drvdata(pdev);
	clk_freq = drv_priv_data->vedu_init_clk_freq[0];
	osal_clk_set_rate(drv_priv_data->vedu_clk[0], clk_freq);
	
	vedu_hw_exit(pdev);
	return 0;
}

static int hi35xx_vedu_resume(struct platform_device *pdev)
{
	vedu_hw_init(pdev);
	return 0;
}
#endif

static int hi35xx_vedu_probe(struct platform_device *pdev)
{
    struct resource *mem;
	struct drv_data *drv_priv_data;
	struct device_node *np = pdev->dev.of_node;
	HI_S32 ret;
	
	drv_priv_data = osal_devm_kzalloc(&pdev->dev, sizeof(*drv_priv_data));
	if (!drv_priv_data) {
		dev_err(&pdev->dev, "could not allocate vedu driver private data.\n");
		return -ENOMEM;
	}
	
	platform_set_drvdata(pdev, drv_priv_data);

    /* if vedu num more than 1, use "platform_get_resource_byname" function to replace */
    mem = osal_platform_get_resource(pdev, IORESOURCE_MEM, 0);
    s_apVeduAddr[0] = (void *)devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(s_apVeduAddr[0]))
    {
		return PTR_ERR(s_apVeduAddr[0]);
	}

    vedu_irq[0] = (unsigned int)osal_platform_get_irq(pdev, 0);
    if (vedu_irq[0] <= 0) {
            dev_err(&pdev->dev, "cannot find vedu IRQ\n");
    }

	ret = osal_of_property_read_u32(np, "vedu0,clock-frequency", &drv_priv_data->vedu_clk_freq[0]);
	if (ret)
	{
        //printk("clk freqency: %d.\n", vedu_clk_frequency);
		drv_priv_data->vedu_clk_freq[0] = vedu_clk_frequency;
	}
	
	drv_priv_data->vedu_clk[0] = of_clk_get(np, 0);
    if (IS_ERR(drv_priv_data->vedu_clk[0]))
    {   
        dev_err(&pdev->dev, "vedu0 cannot get clock\n");
        return -ENOENT;
    }

	drv_priv_data->vedu_init_clk_freq[0] = osal_clk_get_rate(drv_priv_data->vedu_clk[0]);
	
    vedu_hw_init(pdev);

    return VEDU_ModInit();
}

static int hi35xx_vedu_remove(struct platform_device *pdev)
{
    VEDU_ModExit();
	vedu_hw_exit(pdev);
    return 0;
}

static const struct of_device_id hi35xx_vedu_match[] = {
        { .compatible = "hisilicon,hi35xx_vedu" },
        {},
};
MODULE_DEVICE_TABLE(of, hi35xx_vedu_match);

static struct platform_driver hi35xx_vedu_driver = {
		.probe          = hi35xx_vedu_probe,
		.remove         = hi35xx_vedu_remove,
		.driver         = {
		        .name   = "hi35xx_vedu",
		        .of_match_table = hi35xx_vedu_match,
		},

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
		.suspend        = hi35xx_vedu_suspend,
		.resume         = hi35xx_vedu_resume,
#endif
};

osal_module_platform_driver(hi35xx_vedu_driver);
MODULE_LICENSE("Proprietary");
