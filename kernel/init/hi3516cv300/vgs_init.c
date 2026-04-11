#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/clk.h>
#include <linux/version.h>
#include <linux/of_platform.h>

#include "hi_common.h"
#include "hi_osal_init.h"

#define VGS_CLK_DEFAULT_FREQ 297000000 //297M

extern unsigned int max_vgs_job;
extern unsigned int max_vgs_task;
extern unsigned int max_vgs_node;

module_param(max_vgs_job,  uint, S_IRUGO);
module_param(max_vgs_task, uint, S_IRUGO);
module_param(max_vgs_node, uint, S_IRUGO);

unsigned int vgs_clk_frequency = VGS_CLK_DEFAULT_FREQ;
module_param(vgs_clk_frequency, uint, S_IRUGO);

extern int VGS_ModInit(void);
extern void VGS_ModExit(void);

extern void * pVgsReg[2];
extern unsigned int vgs_irq;

struct drv_data {
	struct clk *vgs_clk;
	HI_U32 vgs_init_clk_freq;
	HI_U32 vgs_clk_freq;
};

static int vgs_hw_init(struct platform_device *pdev)
{
	HI_U32 clk_freq;
	struct drv_data *drv_priv_data;
	
	drv_priv_data = platform_get_drvdata(pdev);

	osal_clk_prepare_enable(drv_priv_data->vgs_clk);
	clk_freq = drv_priv_data->vgs_clk_freq;
	osal_clk_set_rate(drv_priv_data->vgs_clk, clk_freq);

	return 0;
}

static void vgs_hw_exit(struct platform_device *pdev)
{
	struct drv_data *drv_priv_data;

	drv_priv_data = platform_get_drvdata(pdev);

	osal_clk_disable_unprepare(drv_priv_data->vgs_clk);
}

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static int hi35xx_vgs_suspend(struct platform_device *pdev, pm_message_t state)
{
	HI_U32 clk_freq;
	struct drv_data *drv_priv_data;
	
	drv_priv_data = platform_get_drvdata(pdev);

	clk_freq = drv_priv_data->vgs_init_clk_freq;
	osal_clk_set_rate(drv_priv_data->vgs_clk, clk_freq);
	
	vgs_hw_exit(pdev);
	return 0;
}

static int hi35xx_vgs_resume(struct platform_device *pdev)
{
	vgs_hw_init(pdev);
	return 0;
}
#endif


static int hi35xx_vgs_probe(struct platform_device *pdev)
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
    pVgsReg[0] = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(pVgsReg[0]))
            return PTR_ERR(pVgsReg[0]);
    
    vgs_irq = osal_platform_get_irq(pdev, 0);
    if (vgs_irq <= 0) {
            dev_err(&pdev->dev, "cannot find vgs IRQ\n");
    }

	ret = osal_of_property_read_u32(np, "vgs,clock-frequency", &drv_priv_data->vgs_clk_freq);
	if (ret)
	{
        //printk("clk freqency: %d.\n", vgs_clk_frequency);
		drv_priv_data->vgs_clk_freq = vgs_clk_frequency;
	}

	drv_priv_data->vgs_clk = of_clk_get(np, 0);
    if (IS_ERR(drv_priv_data->vgs_clk))
    {   
        dev_err(&pdev->dev, "vgs cannot get clock\n");
        return -ENOENT;
    }

	drv_priv_data->vgs_init_clk_freq = osal_clk_get_rate(drv_priv_data->vgs_clk);

	//printk("vgs init clk: %d.\n", drv_priv_data->vgs_init_clk_freq);
	
    //printk("++++++++++ pVgsReg[0] = %x vgs_irq = %d\n",pVgsReg[0],vgs_irq);
	vgs_hw_init(pdev);

	osal_platform_get_module_param(pdev, "max_vgs_job", uint, &max_vgs_job);
	osal_platform_get_module_param(pdev, "max_vgs_task", uint, &max_vgs_task);
	osal_platform_get_module_param(pdev, "max_vgs_node", uint, &max_vgs_node);
	
    VGS_ModInit();
    
    return 0;
}

static int hi35xx_vgs_remove(struct platform_device *pdev)
{
    //printk("<%s> is called\n",__FUNCTION__);
    VGS_ModExit();
    pVgsReg[0] = 0;
	vgs_hw_exit(pdev);

    return 0;
}


static const struct of_device_id hi35xx_vgs_match[] = {
        { .compatible = "hisilicon,hi35xx_vgs" },
        {},
};
MODULE_DEVICE_TABLE(of, hi35xx_vgs_match);

static struct platform_driver hi35xx_vgs_driver = {
        .probe          = hi35xx_vgs_probe,
        .remove         = hi35xx_vgs_remove,
        .driver         = {
                .name   = "hi35xx_vgs",
                .of_match_table = hi35xx_vgs_match,
        },

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
		.suspend		= hi35xx_vgs_suspend,
		.resume 		= hi35xx_vgs_resume,
#endif

};

osal_module_platform_driver(hi35xx_vgs_driver);

MODULE_LICENSE("Proprietary");
