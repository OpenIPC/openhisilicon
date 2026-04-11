#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/clk.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>

#include "hi_common.h"
#include "hi_osal_init.h"

extern HI_U32 pwm_num;
module_param(pwm_num, uint, S_IRUGO);
extern HI_U32 proc_param;
module_param(proc_param, uint, S_IRUGO);
extern HI_U32 update_pos;
module_param(update_pos, uint, S_IRUGO);
extern HI_BOOL int_bottomhalf;
module_param(int_bottomhalf, uint, S_IRUGO);
extern HI_U32 port_init_delay;
module_param(port_init_delay, uint, S_IRUGO);

extern int ISP_ModInit(void);
extern void ISP_ModExit(void);

extern unsigned int isp_irq;

static int hi35xx_isp_probe(struct platform_device *pdev)
{
    isp_irq = osal_platform_get_irq(pdev, 0);
    if (isp_irq <= 0) {
            dev_err(&pdev->dev, "cannot find isp IRQ\n");
    }
	
    //printk("++++++++++ isp_irq = %d\n", isp_irq);

	//get module param
	osal_platform_get_module_param(pdev, "pwm_num", uint, &pwm_num);
	osal_platform_get_module_param(pdev, "proc_param", uint, &proc_param);
	osal_platform_get_module_param(pdev, "update_pos", uint, &update_pos);
	osal_platform_get_module_param(pdev, "int_bottomhalf", uint, &int_bottomhalf);
	osal_platform_get_module_param(pdev, "port_init_delay", uint, &port_init_delay);

    ISP_ModInit();
    
    return 0;
}

static int hi35xx_isp_remove(struct platform_device *pdev)
{
    //printk("<%s> is called\n",__FUNCTION__);
    ISP_ModExit();
    return 0;
}


static const struct of_device_id hi35xx_isp_match[] = {
        { .compatible = "hisilicon,hi35xx_isp" },
        {},
};
MODULE_DEVICE_TABLE(of, hi35xx_isp_match);

static struct platform_driver hi35xx_isp_driver = {
        .probe          = hi35xx_isp_probe,
        .remove         = hi35xx_isp_remove,
        .driver         = {
                .name   = "hi35xx_isp",
                .of_match_table = hi35xx_isp_match,
        },
};

osal_module_platform_driver(hi35xx_isp_driver);

MODULE_LICENSE("Proprietary");
