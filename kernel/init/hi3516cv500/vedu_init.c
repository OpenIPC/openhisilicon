/******************************************************************************

  Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : vedu_init.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2017
  Description   :
******************************************************************************/


#include <linux/module.h>
#include <linux/of_platform.h>
#include "hi_type.h"
#include "hi_defines.h"
#include "hi_osal.h"


#define VEDU_DEV_NAME_LENGTH 10

extern void *g_vedu_addr[VEDU_IP_NUM];
extern int g_vedu_irq[VEDU_IP_NUM];
extern void * g_jpge_chip_reg[VEDU_IP_NUM];
extern int  g_jpeg_irq[VEDU_IP_NUM];

extern int vpu_mod_init(void);
extern void vpu_mod_exit(void);


static int hi35xx_vedu_probe(struct platform_device *pdev)
{
    hi_u32 i;
    struct resource *mem;
    hi_char vedu_dev_name[VEDU_DEV_NAME_LENGTH] = {'\0'};


    /* if vedu num more than 1, use "platform_get_resource_byname" function to replace */
    for(i=0; i< VEDU_IP_NUM; i++)
    {
        snprintf(vedu_dev_name, VEDU_DEV_NAME_LENGTH, "vedu%d", i);
        mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, vedu_dev_name);

        g_vedu_addr[i] = (void *)devm_ioremap_resource(&pdev->dev, mem);
        if (IS_ERR(g_vedu_addr[i]))
        {
            osal_printk("%s,%d,remap vedu(%d) IRQ!!\n",__FUNCTION__,__LINE__,i);
            return PTR_ERR(g_vedu_addr[i]);
        }

        g_vedu_irq[i] = osal_platform_get_irq_byname(pdev, vedu_dev_name);
        if (g_vedu_irq[i] <= 0) {
            osal_printk("%s,%d,cannot find vedu(%d) IRQ!!\n",__FUNCTION__,__LINE__,i);
                dev_err(&pdev->dev, "cannot find vedu IRQ\n");
                return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}


static int hi35xx_jpge_probe(struct platform_device *pdev)
{
    struct resource *mem;
    hi_char jpge_dev_name[VEDU_DEV_NAME_LENGTH] = {'\0'};

    snprintf(jpge_dev_name, VEDU_DEV_NAME_LENGTH, "jpge");

    mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, jpge_dev_name);

    g_jpge_chip_reg[0] = (void *)devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(g_jpge_chip_reg[0]))
    {
        osal_printk("%s,%d,remap jpge err!!\n",__FUNCTION__,__LINE__);
        return PTR_ERR(g_jpge_chip_reg[0]);
    }

    g_jpeg_irq[0] = osal_platform_get_irq_byname(pdev, jpge_dev_name);
    if (g_jpeg_irq[0] <= 0) {
            osal_printk("%s,%d,cannot find jpge IRQ!!\n",__FUNCTION__,__LINE__);
            dev_err(&pdev->dev, "cannot find jpge IRQ\n");
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}


static int hi35xx_vpu_probe(struct platform_device *pdev)
{
    hi_s32 ret;

    ret = hi35xx_vedu_probe(pdev);
    if(HI_SUCCESS != ret)
    {
        osal_printk("%s,%d,vedu probe err!!\n",__FUNCTION__,__LINE__);
        return ret;
    }

    ret = hi35xx_jpge_probe(pdev);
    if(HI_SUCCESS != ret)
    {
        osal_printk("%s,%d,jpge probe err!!\n",__FUNCTION__,__LINE__);
        return ret;
    }

    ret = vpu_mod_init();
    if(HI_SUCCESS != ret)
    {
        osal_printk("%s,%d,VPU mod_init err!!\n",__FUNCTION__,__LINE__);
        return ret;
    }

    return ret;
}

static int hi35xx_vpu_remove(struct platform_device *pdev)
{
    vpu_mod_exit();
    return 0;
}

static const struct of_device_id hi35xx_vpu_match[] = {
        { .compatible = "hisilicon,hisi-vedu" },
        {},
};
MODULE_DEVICE_TABLE(of, hi35xx_vpu_match);

static struct platform_driver hi35xx_vpu_driver = {
        .probe          = hi35xx_vpu_probe,
        .remove         = hi35xx_vpu_remove,
        .driver         = {
                .name   = "hi35xx_vedu",
                .of_match_table = hi35xx_vpu_match,
        }
};

osal_module_platform_driver(hi35xx_vpu_driver);
MODULE_LICENSE("Proprietary");


