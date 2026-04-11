#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/clk.h>
#include <linux/version.h>
#include <linux/of_platform.h>

#include "hi_common.h"
#include "hi_osal_init.h"

#define VPSS_CLK_DEFAULT_FREQ 250000000 //250M

extern unsigned int rfr_frame_comp;
module_param(rfr_frame_comp, uint, S_IRUGO);

extern unsigned int bOneBufferforLowDelay;
module_param(bOneBufferforLowDelay, uint, S_IRUGO);

//extern unsigned int bEnViDelayInt;
//module_param(bEnViDelayInt, uint, S_IRUGO);

unsigned int vpss_clk_frequency = VPSS_CLK_DEFAULT_FREQ;
module_param(vpss_clk_frequency, uint, S_IRUGO);

extern int VPSS_ModInit(void);
extern void VPSS_ModExit(void);

extern void * pVpssReg[2];
extern unsigned int vpss_irq;
extern unsigned int vpss_vi_irq;


struct drv_data {
	struct clk *vpss_clk;
	HI_U32 vpss_init_clk_freq;
	HI_U32 vpss_clk_freq;
};

static int vpss_hw_init(struct platform_device *pdev)
{
	HI_U32 clk_freq;
	struct drv_data *drv_priv_data;

	drv_priv_data = platform_get_drvdata(pdev);

	osal_clk_prepare_enable(drv_priv_data->vpss_clk);
	clk_freq = drv_priv_data->vpss_clk_freq;
	osal_clk_set_rate(drv_priv_data->vpss_clk, clk_freq);

	return 0;
}

static void vpss_hw_exit(struct platform_device *pdev)
{
	struct drv_data *drv_priv_data;

	drv_priv_data = platform_get_drvdata(pdev);

	osal_clk_disable_unprepare(drv_priv_data->vpss_clk);
}

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static int hi35xx_vpss_suspend(struct platform_device *pdev, pm_message_t state)
{
	HI_U32 clk_freq;
	struct drv_data *drv_priv_data;

	drv_priv_data = platform_get_drvdata(pdev);
	clk_freq = drv_priv_data->vpss_init_clk_freq;
	osal_clk_set_rate(drv_priv_data->vpss_clk, clk_freq);

	vpss_hw_exit(pdev);
	return 0;
}

static int hi35xx_vpss_resume(struct platform_device *pdev)
{
	vpss_hw_init(pdev);
	return 0;
}
#endif

static int hi35xx_vpss_probe(struct platform_device *pdev)
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
    pVpssReg[0] = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(pVpssReg[0]))
            return PTR_ERR(pVpssReg[0]);

    vpss_irq = osal_platform_get_irq_byname(pdev, "vpss");
    if (vpss_irq <= 0) {
            dev_err(&pdev->dev, "cannot find vpss IRQ\n");
    }

    vpss_vi_irq = osal_platform_get_irq_byname(pdev, "vi");
    if (vpss_vi_irq <= 0) {
            dev_err(&pdev->dev, "cannot find vpss_vi IRQ\n");
    }

	ret = osal_of_property_read_u32(np, "vpss,clock-frequency", &drv_priv_data->vpss_clk_freq);
	if (ret)
	{
        //printk("clk freqency: %d.\n", vpss_clk_frequency);
		drv_priv_data->vpss_clk_freq = vpss_clk_frequency;
	}

	drv_priv_data->vpss_clk = of_clk_get(np, 0);
    if (IS_ERR(drv_priv_data->vpss_clk))
    {
        dev_err(&pdev->dev, "vpss cannot get clock\n");
        return -ENOENT;
    }

	drv_priv_data->vpss_init_clk_freq = osal_clk_get_rate(drv_priv_data->vpss_clk);
    //printk("++++++++++ pVpssReg[0] = %x vpss_irq = %d\n",pVpssReg[0],vpss_irq);

	vpss_hw_init(pdev);

	osal_platform_get_module_param(pdev, "rfr_frame_comp", uint, &rfr_frame_comp);
	osal_platform_get_module_param(pdev, "bOneBufferforLowDelay", uint, &bOneBufferforLowDelay);
    //osal_platform_get_module_param(pdev, "bEnViDelayInt", uint, &bEnViDelayInt);

    VPSS_ModInit();

    return 0;
}

static int hi35xx_vpss_remove(struct platform_device *pdev)
{
    //printk("<%s> is called\n",__FUNCTION__);
    VPSS_ModExit();

    pVpssReg[0] = 0;

	vpss_hw_exit(pdev);
    return 0;
}


static const struct of_device_id hi35xx_vpss_match[] = {
        { .compatible = "hisilicon,hi35xx_vpss" },
        {},
};
MODULE_DEVICE_TABLE(of, hi35xx_vpss_match);

static struct platform_driver hi35xx_vpss_driver = {
        .probe          = hi35xx_vpss_probe,
        .remove         = hi35xx_vpss_remove,
        .driver         = {
                .name   = "hi35xx_vpss",
                .of_match_table = hi35xx_vpss_match,
        },

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
		.suspend		= hi35xx_vpss_suspend,
		.resume 		= hi35xx_vpss_resume,
#endif
};

osal_module_platform_driver(hi35xx_vpss_driver);

MODULE_LICENSE("Proprietary");
