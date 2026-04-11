#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/clk.h>
#include <linux/version.h>
#include <linux/of_platform.h>

#include "hi_common.h"
#include "hi_osal_init.h"

#define IVE_CLK_DEFAULT_FREQ 297000000 //297M

extern HI_BOOL g_bIvePowerSaveEn;
extern HI_U16 g_u16IveNodeNum;

extern int IVE_ModInit(void);
extern void IVE_ModExit(void);

module_param_named(save_power, g_bIvePowerSaveEn, uint, S_IRUGO);
module_param_named(max_node_num, g_u16IveNodeNum, ushort, S_IRUGO);

unsigned int ive_clk_frequency = IVE_CLK_DEFAULT_FREQ;
module_param(ive_clk_frequency, uint, S_IRUGO);

extern void *g_pstIveRegs;
extern unsigned int ive_irq;

struct drv_data {
	struct clk *ive_clk;
	HI_U32 ive_init_clk_freq;
	HI_U32 ive_clk_freq;
};

static int ive_hw_init(struct platform_device *pdev)
{
	HI_U32 clk_freq;
	struct drv_data *drv_priv_data;
	
	drv_priv_data = platform_get_drvdata(pdev);

	osal_clk_prepare_enable(drv_priv_data->ive_clk);
	clk_freq = drv_priv_data->ive_clk_freq;
	osal_clk_set_rate(drv_priv_data->ive_clk, clk_freq);

	return 0;
}

static void ive_hw_exit(struct platform_device *pdev)
{
	struct drv_data *drv_priv_data;

	drv_priv_data = platform_get_drvdata(pdev);

	osal_clk_disable_unprepare(drv_priv_data->ive_clk);
}

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static int hi35xx_ive_suspend(struct platform_device *pdev, pm_message_t state)
{
	HI_U32 clk_freq;
	struct drv_data *drv_priv_data;
	
	drv_priv_data = platform_get_drvdata(pdev);

	clk_freq = drv_priv_data->ive_init_clk_freq;
	osal_clk_set_rate(drv_priv_data->ive_clk, clk_freq);
	
	ive_hw_exit(pdev);
	return 0;
}

static int hi35xx_ive_resume(struct platform_device *pdev)
{
	ive_hw_init(pdev);
	return 0;
}
#endif

static int hi35xx_ive_probe(struct platform_device *pdev)
{
    struct resource *mem;
	struct drv_data *drv_priv_data;
	struct device_node *np = pdev->dev.of_node;
	HI_S32 ret;

	drv_priv_data = osal_devm_kzalloc(&pdev->dev, sizeof(*drv_priv_data));
	if (!drv_priv_data) {
		dev_err(&pdev->dev, "could not allocate vpss driver private data.\n");
		return -ENOMEM;
	}
	
	platform_set_drvdata(pdev, drv_priv_data);
    
    mem = osal_platform_get_resource(pdev, IORESOURCE_MEM, 0);
    g_pstIveRegs = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(g_pstIveRegs))
            return PTR_ERR(g_pstIveRegs);
    
    ive_irq = osal_platform_get_irq(pdev, 0);
    if (ive_irq <= 0) {
            dev_err(&pdev->dev, "cannot find vou IRQ\n");
    }
    //printk("++++++++++ g_pstIveRegs = %p ive_irq = %d\n",g_pstIveRegs,ive_irq);

	ret = osal_of_property_read_u32(np, "ive,clock-frequency", &drv_priv_data->ive_clk_freq);
	if (ret)
	{
        //printk("clk freqency: %d.\n", ive_clk_frequency);
		drv_priv_data->ive_clk_freq = ive_clk_frequency;
	}

	drv_priv_data->ive_clk = of_clk_get(np, 0);
    if (IS_ERR(drv_priv_data->ive_clk))
    {   
        dev_err(&pdev->dev, "ive cannot get clock\n");
        return -ENOENT;
    }

	drv_priv_data->ive_init_clk_freq = osal_clk_get_rate(drv_priv_data->ive_clk);
	ive_hw_init(pdev);

	osal_platform_get_module_param(pdev, "save_power", uint, &g_bIvePowerSaveEn);
    IVE_ModInit();
    
    return 0;
}

static int hi35xx_ive_remove(struct platform_device *pdev)
{
    //printk("<%s> is called\n",__FUNCTION__);
    IVE_ModExit();
	ive_hw_exit(pdev);
    return 0;
}


static const struct of_device_id hi35xx_ive_match[] = {
        { .compatible = "hisilicon,hi35xx_ive" },
        {},
};
MODULE_DEVICE_TABLE(of, hi35xx_ive_match);

static struct platform_driver hi35xx_ive_driver = {
        .probe          = hi35xx_ive_probe,
        .remove         = hi35xx_ive_remove,
        .driver         = {
                .name   = "hi35xx_ive",
                .of_match_table = hi35xx_ive_match,
        },

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
		.suspend		= hi35xx_ive_suspend,
		.resume 		= hi35xx_ive_resume,
#endif
};

osal_module_platform_driver(hi35xx_ive_driver);
MODULE_LICENSE("Proprietary");
