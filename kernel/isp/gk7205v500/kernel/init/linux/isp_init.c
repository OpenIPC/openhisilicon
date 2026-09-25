/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>

#include "type.h"
#include "common.h"
#include "osal.h"

extern int ISP_ModInit(void);
extern void ISP_ModExit(void);


extern  unsigned int     isp_fe_irq;

static int vendor_isp_probe(struct platform_device *pdev)
{
    isp_fe_irq = osal_platform_get_irq_byname (pdev, "ISP");

    if (isp_fe_irq <= 0)
    {
        dev_err(&pdev->dev, "Can't find isp IRQ!\n");
        return -ENODEV;
    }

    return (ISP_ModInit() == XMEDIA_SUCCESS) ? 0 : -ENODEV;
}

static int vendor_isp_remove(struct platform_device *pdev)
{
    ISP_ModExit();
    
    return 0;
}

static const struct of_device_id vendor_isp_match[] =
{
    { .compatible = "xmedia,isp" },
    {},
};

MODULE_DEVICE_TABLE(of, vendor_isp_match);

static struct platform_driver vendor_isp_driver = 
{
        .probe          = vendor_isp_probe,
        .remove         = vendor_isp_remove,
        .driver         = 
        {
            .name           = "vendor_isp",
            .of_match_table = vendor_isp_match,
        },
};


#ifdef MODULE

osal_module_platform_driver(vendor_isp_driver);
MODULE_LICENSE("GPL");

#else

int __init isp_driver_init(void)
{
    return osal_platform_driver_register(&vendor_isp_driver);
}

#endif




