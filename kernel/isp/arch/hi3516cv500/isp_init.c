#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>

#include "hi_type.h"
#include "hi_common.h"
#include "hi_osal.h"

extern int ISP_ModInit(void);
extern void ISP_ModExit(void);


extern  unsigned int     isp_fe_irq;

static int hi35xx_isp_probe(struct platform_device *pdev)
{
    isp_fe_irq = osal_platform_get_irq_byname (pdev, "ISP");

    if (isp_fe_irq <= 0)
    {
        dev_err(&pdev->dev, "Can't find isp IRQ!\n");
        return HI_FALSE;
    }

    ISP_ModInit();

    return 0;
}

static int hi35xx_isp_remove(struct platform_device *pdev)
{
    ISP_ModExit();
    
    return 0;
}

static const struct of_device_id hi35xx_isp_match[] =
{
    { .compatible = "hisilicon,hisi-isp" },
    {},
};

MODULE_DEVICE_TABLE(of, hi35xx_isp_match);

static struct platform_driver hi35xx_isp_driver = 
{
        .probe          = hi35xx_isp_probe,
        .remove         = hi35xx_isp_remove,
        .driver         = 
        {
            .name           = "hi35xx_isp",
            .of_match_table = hi35xx_isp_match,
        },
};

osal_module_platform_driver(hi35xx_isp_driver);

MODULE_LICENSE("Proprietary");


