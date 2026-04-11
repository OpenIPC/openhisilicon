#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/clk.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>

#include "hi_common.h"
#include "hi_osal_init.h"

extern int JPEGE_ModInit(void);
extern void JPEGE_ModExit(void);

extern HI_U32  JpegeOneStreamBuffer;
module_param(JpegeOneStreamBuffer, uint, S_IRUGO);

extern HI_U32  JpegeMiniBufMode;
module_param(JpegeMiniBufMode, uint, S_IRUGO);

extern void * g_pstJpeguCHIP_Reg[1];
extern unsigned int  jpeg_irq[1];

struct drv_data {
	struct clk *jpge_clk[1];
};

static int jpge_hw_init(struct platform_device *pdev)
{
	struct drv_data *drv_priv_data;
	
	drv_priv_data = platform_get_drvdata(pdev);

	osal_clk_prepare_enable(drv_priv_data->jpge_clk[0]);

	return 0;
}

static void jpge_hw_exit(struct platform_device *pdev)
{
	struct drv_data *drv_priv_data;

	drv_priv_data = platform_get_drvdata(pdev);

	osal_clk_disable_unprepare(drv_priv_data->jpge_clk[0]);
}

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static int hi35xx_jpge_suspend(struct platform_device *pdev, pm_message_t state)
{
	jpge_hw_exit(pdev);
	return 0;
}

static int hi35xx_jpge_resume(struct platform_device *pdev)
{
	jpge_hw_init(pdev);
	return 0;
}
#endif

static int hi35xx_jpege_probe(struct platform_device *pdev)
{
    struct resource *mem;
	struct drv_data *drv_priv_data;
	struct device_node *np = pdev->dev.of_node;
	
	drv_priv_data = osal_devm_kzalloc(&pdev->dev, sizeof(*drv_priv_data));
	if (!drv_priv_data) {
		dev_err(&pdev->dev, "could not allocate vedu driver private data.\n");
		return -ENOMEM;
	}
	
	platform_set_drvdata(pdev, drv_priv_data);
    
    mem = osal_platform_get_resource(pdev, IORESOURCE_MEM, 0);
    g_pstJpeguCHIP_Reg[0] = (void *)devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(g_pstJpeguCHIP_Reg[0]))
            return PTR_ERR(g_pstJpeguCHIP_Reg[0]);
    
    jpeg_irq[0] = osal_platform_get_irq(pdev, 0);
    if (jpeg_irq[0] <= 0) {
            dev_err(&pdev->dev, "cannot find jpege IRQ\n");
    }
    //osal_printk("++++++++++ pJpegeReg = %p jpege_irq = %d\n",g_pstJpeguCHIP_Reg[0],jpeg_irq[0]);

	drv_priv_data->jpge_clk[0] = of_clk_get(np, 0);
    if (IS_ERR(drv_priv_data->jpge_clk[0]))
    {   
        dev_err(&pdev->dev, "jpge_clk0 cannot get clock\n");
        return -ENOENT;
    }
	
    jpge_hw_init(pdev);
    JPEGE_ModInit();
    
    return 0;
}

static int hi35xx_jpege_remove(struct platform_device *pdev)
{
    //osal_printk("<%s> is called\n",__FUNCTION__);
    JPEGE_ModExit();
	jpge_hw_exit(pdev);
    return 0;
}


static const struct of_device_id hi35xx_jpege_match[] = {
        { .compatible = "hisilicon,hi35xx_jpege" },
        {},
};

MODULE_DEVICE_TABLE(of, hi35xx_jpege_match);

static struct platform_driver hi35xx_jpege_driver = {
        .probe          = hi35xx_jpege_probe,
        .remove         = hi35xx_jpege_remove,
        .driver         = {
                .name   = "hi35xx_jpege",
                .of_match_table = hi35xx_jpege_match,
        },

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
		.suspend        = hi35xx_jpge_suspend,
		.resume         = hi35xx_jpge_resume,
#endif
};

osal_module_platform_driver(hi35xx_jpege_driver);

MODULE_LICENSE("Proprietary");
