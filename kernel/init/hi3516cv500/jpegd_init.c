#include <linux/module.h>
#include <linux/of_platform.h>

#include "hi_type.h"
#include "hi_defines.h"
#include "hi_osal.h"

extern int jpegd_module_init(void);
extern void jpegd_module_exit(void);
extern hi_void* g_jpegd_reg_addr[JPEGD_IP_NUM];
extern hi_u32 g_jpegd_irq[JPEGD_IP_NUM];

#define JPEGD_IP_NAME_LENGTH 10

static hi_s32 jpegd_dev_init(struct platform_device* pdev)
{
    hi_u32 i;
    hi_char jpegd_ip_name[JPEGD_IP_NAME_LENGTH] = {'\0'};
    struct resource* mem;

    for (i = 0; i < JPEGD_IP_NUM; i++)
    {
        //snprintf(jpegd_ip_name, JPEGD_IP_NAME_LENGTH, "jpegd%d", i);
        snprintf(jpegd_ip_name, JPEGD_IP_NAME_LENGTH, "jpegd");
        mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, jpegd_ip_name);
        g_jpegd_reg_addr[i] = devm_ioremap_resource(&pdev->dev, mem);

        if (IS_ERR(g_jpegd_reg_addr[i]))
        {
            osal_printk("devm_ioremap_resource fail for %ld\n", PTR_ERR(g_jpegd_reg_addr[i]));
            return PTR_ERR(g_jpegd_reg_addr[i]);
        }

        g_jpegd_irq[i] = osal_platform_get_irq_byname(pdev, jpegd_ip_name);

        if (g_jpegd_irq[i] <= 0)
        {
            dev_err(&pdev->dev, "cannot find jpegd%d IRQ\n", i);
            return HI_FAILURE;
        }
    }

    return 0;
}

static hi_void jpegd_dev_exit(hi_void)
{
    hi_u32 i;

    for (i = 0; i < JPEGD_IP_NUM; i++)
    {
        g_jpegd_reg_addr[i] = NULL;
    }
}

static int hi35xx_jpegd_probe(struct platform_device* pdev)
{
    hi_s32 ret = HI_SUCCESS;

    ret = jpegd_dev_init(pdev);
    if(HI_SUCCESS != ret)
    {
        osal_printk("jpegd_register_dev fail!\n");
        return ret;
    }

    ret = jpegd_module_init();

    return ret;
}

static int hi35xx_jpegd_remove(struct platform_device* pdev)
{
    jpegd_module_exit();
    jpegd_dev_exit();

    return HI_SUCCESS;
}





static const struct of_device_id hi35xx_jpegd_match[] =
{
    { .compatible = "hisilicon,hisi-jpegd" },
    {},
};
MODULE_DEVICE_TABLE(of, hi35xx_jpegd_match);

static struct platform_driver hi35xx_jpegd_driver =
{
    .probe          = hi35xx_jpegd_probe,
    .remove         = hi35xx_jpegd_remove,
    .driver         = {
        .name           = "hi35xx_jpegd",
        .of_match_table = hi35xx_jpegd_match,
    },
};

osal_module_platform_driver(hi35xx_jpegd_driver);









