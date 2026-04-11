#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>

#include "hi_common.h"
#include "hi_osal_init.h"


extern unsigned int detectCycle;
extern unsigned int transparentTransmit ;
extern unsigned int lowPowerMode; 

module_param(detectCycle, uint, S_IRUGO);
module_param(transparentTransmit, uint, S_IRUGO);
module_param(lowPowerMode, uint, S_IRUGO);

char * vou_mode = "0";
module_param(vou_mode, charp, S_IRUGO);

extern int VOU_ModInit(void);
extern void VOU_ModExit(void);

extern void * pVoReg;
extern unsigned int vou_irq;

struct drv_data {
	struct pinctrl *p;
	struct pinctrl_state *state;
};


static int vou_hw_init(struct platform_device *pdev)
{
	return 0;
}

static void vou_hw_exit(struct platform_device *pdev)
{
}

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static int hi35xx_vou_suspend(struct platform_device *pdev, pm_message_t state)
{
	vou_hw_exit(pdev);
	osal_pinctrl_get_select(&pdev->dev, "sleep");
	return 0;
}

static int hi35xx_vou_resume(struct platform_device *pdev)
{
	struct drv_data *drv_priv_data;

	drv_priv_data = platform_get_drvdata(pdev);
	if (drv_priv_data->p && drv_priv_data->state)
	{
		osal_pinctrl_select_state(drv_priv_data->p, drv_priv_data->state);
	}
	
	vou_hw_init(pdev);
	return 0;
}
#endif


static int hi35xx_vou_probe(struct platform_device *pdev)
{
    struct resource *mem;
	struct drv_data *drv_priv_data;
	HI_S32 ret;

	drv_priv_data = osal_devm_kzalloc(&pdev->dev, sizeof(*drv_priv_data));
	if (!drv_priv_data) {
		dev_err(&pdev->dev, "could not allocate vou driver private data.\n");
		return -ENOMEM;
	}
	
	platform_set_drvdata(pdev, drv_priv_data);
    
    mem = osal_platform_get_resource(pdev, IORESOURCE_MEM, 0);
    pVoReg = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(pVoReg))
            return PTR_ERR(pVoReg);
    
    vou_irq = osal_platform_get_irq(pdev, 0);
    if (vou_irq <= 0) {
            dev_err(&pdev->dev, "cannot find vou IRQ\n");
    }
    //printk("++++++++++ pVoReg = %x vou_irq = %d\n",pVoReg,vou_irq);

	vou_hw_init(pdev);

	if (!strncmp(vou_mode, "bt656", 10) 
		|| !strncmp(vou_mode, "lcd", 10)
		|| !strncmp(vou_mode, "default", 10))
	{
		struct pinctrl *p;
		struct pinctrl_state *state;
		
		p = (struct pinctrl *)osal_devm_pinctrl_get(&pdev->dev);
		if (IS_ERR(p)) {
			dev_err(&pdev->dev, "no pinctrl handle\n");
			return -ENOENT;
		}

		state = (struct pinctrl_state *)osal_pinctrl_lookup_state(p, vou_mode);
		if (IS_ERR(state)) {
			dev_err(&pdev->dev, "no %s pinctrl state\n", vou_mode);
			osal_devm_pinctrl_put(p);
			return -ENOENT;
		}

		ret = osal_pinctrl_select_state(p, state);
		if (ret) {
			dev_err(&pdev->dev, "failed to activate %s pinctrl state\n", vou_mode);
			osal_devm_pinctrl_put(p);
			return -ENOENT;
		}

		drv_priv_data->p = p;
		drv_priv_data->state = state;
	}

	osal_platform_get_module_param(pdev, "detectCycle", uint, &detectCycle);
	osal_platform_get_module_param(pdev, "transparentTransmit", uint, &transparentTransmit);
	osal_platform_get_module_param(pdev, "lowPowerMode", uint, &lowPowerMode);

    VOU_ModInit();
    
    return 0;
}

static int hi35xx_vou_remove(struct platform_device *pdev)
{
    //printk("<%s> is called\n",__FUNCTION__);
    VOU_ModExit();
	pVoReg = NULL;
	vou_hw_exit(pdev);
    return 0;
}


static const struct of_device_id hi35xx_vou_match[] = {
	    { .compatible = "hisilicon,hi35xx_vou" },
	    {},
};
MODULE_DEVICE_TABLE(of, hi35xx_vou_match);

static struct platform_driver hi35xx_vou_driver = {
		.probe          = hi35xx_vou_probe,
		.remove         = hi35xx_vou_remove,
		.driver         = {
		        .name   = "hi35xx_vou",
		        .of_match_table = hi35xx_vou_match,
		},

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
		.suspend        = hi35xx_vou_suspend,
		.resume         = hi35xx_vou_resume,
#endif
};

osal_module_platform_driver(hi35xx_vou_driver);

MODULE_LICENSE("Proprietary");
