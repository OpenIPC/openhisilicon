#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>


#include "hi_comm_video.h"
#include "hi_common.h"
#include "hi_osal.h"

extern int vi_do_mod_init(void);
extern void vi_do_mod_exit(void);

#define                  VI_NAME_LEN           10

extern void*            g_past_vi_cap_reg[VICAP_IP_NUM];
extern void*            g_past_vi_proc_reg[VIPROC_IP_NUM];
extern unsigned int     g_ast_vi_cap_irq_num[VICAP_IP_NUM];
extern unsigned int     g_ast_vi_proc_irq_num[VIPROC_IRQ_NUM];

static int hi35xx_vi_probe(struct platform_device* pdev)
{
    HI_U32   i;
    HI_S32   s32Ret;
    HI_U8    u8ViCapName[VICAP_IP_NUM][VI_NAME_LEN];
    HI_U8    u8ViProcName[VIPROC_IP_NUM][VI_NAME_LEN];
    struct   resource*    mem;

    for (i = 0; i < VICAP_IP_NUM; i++)
    {
        snprintf(u8ViCapName[i], sizeof(u8ViCapName[i]), "VI_CAP%d", i);
        mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, u8ViCapName[i]);
        g_past_vi_cap_reg[i] = devm_ioremap_resource(&pdev->dev, mem);

        if (IS_ERR(g_past_vi_cap_reg[i]))
        {
            return PTR_ERR(g_past_vi_cap_reg[i]);
        }

        g_ast_vi_cap_irq_num[i] = osal_platform_get_irq_byname(pdev, u8ViCapName[i]);

        if (g_ast_vi_cap_irq_num[i] <= 0)
        {
            dev_err(&pdev->dev, "can not find vi cap IRQ\n");
            return HI_FAILURE;
        }
    }

    for (i = 0; i < VIPROC_IP_NUM; i++)
    {
        snprintf(u8ViProcName[i], sizeof(u8ViProcName[i]), "VI_PROC%d", i);
        mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, u8ViProcName[i]);
        g_past_vi_proc_reg[i] = devm_ioremap_resource(&pdev->dev, mem);

        if (IS_ERR(g_past_vi_proc_reg[i]))
        {
            return PTR_ERR(g_past_vi_proc_reg[i]);
        }

        g_ast_vi_proc_irq_num[i] = osal_platform_get_irq_byname(pdev, u8ViProcName[i]);

        if (g_ast_vi_proc_irq_num[i] <= 0)
        {
            dev_err(&pdev->dev, "can not find vi proc IRQ\n");
            return HI_FAILURE;
        }
    }

    s32Ret = vi_do_mod_init();
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    };

    return 0;
}

static int hi35xx_vi_remove(struct platform_device* pdev)
{
    HI_U32  i;
    vi_do_mod_exit();

    for (i = 0; i < VICAP_IP_NUM; i++)
    {
        g_past_vi_cap_reg[i] = HI_NULL;
    }

    for (i = 0; i < VIPROC_IP_NUM; i++)
    {
        g_past_vi_proc_reg[i] = HI_NULL;
    }

    return 0;
}


static const struct of_device_id hi35xx_vi_match[] =
{
    { .compatible = "hisilicon,hisi-vi" },
    {},
};
MODULE_DEVICE_TABLE(of, hi35xx_vi_match);

static struct platform_driver hi35xx_vi_driver =
{
        .probe          = hi35xx_vi_probe,
        .remove         = hi35xx_vi_remove,
        .driver         = {
                .name   = "hi35xx_vi",
                .of_match_table = hi35xx_vi_match,
        },
};

osal_module_platform_driver(hi35xx_vi_driver);

MODULE_LICENSE("Proprietary");

