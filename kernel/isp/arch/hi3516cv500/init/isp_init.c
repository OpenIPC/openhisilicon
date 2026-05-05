#include <linux/module.h>
#include "../../../../compat/kernel_compat.h"
#include <linux/kernel.h>
#include "../../../../compat/kernel_compat.h"
#include <linux/printk.h>
#include "../../../../compat/kernel_compat.h"
#include <linux/version.h>
#include "../../../../compat/kernel_compat.h"
#include <linux/of_platform.h>
#include "../../../../compat/kernel_compat.h"

#include "hi_type.h"
#include "../../../../compat/kernel_compat.h"
#include "hi_common.h"
#include "../../../../compat/kernel_compat.h"
#include "hi_osal.h"
#include "../../../../compat/kernel_compat.h"

extern int isp_module_init(void);
extern void isp_module_exit(void);


extern  unsigned int     g_isp_fe_irq;

static int hi35xx_isp_probe(struct platform_device *pdev)
{
    g_isp_fe_irq = osal_platform_get_irq_byname (pdev, "ISP");

    if (g_isp_fe_irq <= 0)
    {
        dev_err(&pdev->dev, "Can't find isp IRQ!\n");
        return HI_FALSE;
    }

    isp_module_init();

    return 0;
}

static compat_platform_remove_ret hi35xx_isp_remove(struct platform_device *pdev)
{
    isp_module_exit();
    
    compat_platform_remove_return;
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


