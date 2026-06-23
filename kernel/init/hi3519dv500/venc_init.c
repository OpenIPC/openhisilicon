/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of_platform.h>
#include "securec.h"
#include "ot_type.h"
#include "ot_defines.h"
#include "ot_osal.h"
#include "ot_venc_mod_init.h"

static td_u32 g_venc_max_chn_num = OT_VENC_MAX_CHN_NUM;

static td_void *g_vedu_addr[OT_VENC_VEDU_IP_NUM];
static td_s32 g_vedu_irq[OT_VENC_VEDU_IP_NUM];

static td_void *g_jpge_chip_reg[OT_VENC_JPGE_IP_NUM];
static td_s32 g_jpeg_irq[OT_VENC_JPGE_IP_NUM];

static td_u32 g_vedu_en[OT_VENC_VEDU_IP_NUM];
static bool g_high_profile = TD_FALSE;

#ifndef MODULE
osal_setup_num_param(g_venc_max_chn_num, g_venc_max_chn_num);
osal_setup_num_param(g_high_profile, g_high_profile);
#else
module_param(g_venc_max_chn_num, uint, S_IRUGO);
module_param(g_high_profile, bool, S_IRUGO);
#endif

EXPORT_SYMBOL(vedu_get_hal_addr);

#define VEDU_DEV_NAME_LENGTH 10

static td_u32 g_vedu_num = 0;

static td_void vedu_get_vpu_info(td_u32 vpu[], td_u32 len)
{
    td_u32 i;

    for (i = 0; i < len; i++) {
        if (g_vedu_en[i] == 1) {
            vpu[i] = 1;
            g_vedu_num++;
        } else {
            vpu[i] = 0;
        }
    }

    if (g_vedu_num == 0) {
        vedu_get_vedu_enable(vpu, OT_VENC_VEDU_IP_NUM);
    } else {
        vedu_set_vedu_enable(vpu, OT_VENC_VEDU_IP_NUM);
        osal_printk("%s,%d, user set vedu num %d !\n", __FUNCTION__, __LINE__, g_vedu_num);
    }

    return;
}

static td_s32 ot_vedu_probe(struct platform_device *pdev)
{
    td_u32 i;
    td_s32 ret;
    struct resource *mem = TD_NULL;
    td_char vedu_dev_name[VEDU_DEV_NAME_LENGTH] = {'\0'};
    td_u32 vpu[OT_VENC_VEDU_IP_NUM] = {0};

    ret = vedu_set_high_profile(g_high_profile);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    vedu_get_vpu_info(vpu, OT_VENC_VEDU_IP_NUM);

    for (i = 0; i < OT_VENC_VEDU_IP_NUM; i++) {
        if (vpu[i] == 0) {
            continue;
        }

        /* vedu num more than 1, use "platform_get_resource_byname" function to replace */
        if (snprintf_s(vedu_dev_name, VEDU_DEV_NAME_LENGTH, VEDU_DEV_NAME_LENGTH - 1, "vedu%d", i) == -1) {
            osal_printk("%s, %d, set vedu(%d) name error\n", __FUNCTION__, __LINE__, i);
            return TD_FAILURE;
        }
        mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, vedu_dev_name);

        g_vedu_addr[i] = (td_void *)devm_ioremap_resource(&pdev->dev, mem);
        if (IS_ERR(g_vedu_addr[i])) {
            osal_printk("%s,%d,remap vedu(%d) reg addr failed!!\n", __FUNCTION__, __LINE__, i);
            return PTR_ERR(g_vedu_addr[i]);
        }

        g_vedu_irq[i] = osal_platform_get_irq_byname(pdev, vedu_dev_name);
        if (g_vedu_irq[i] <= 0) {
            osal_printk("%s,%d,cannot find vedu(%d) IRQ!!\n", __FUNCTION__, __LINE__, vpu[i]);
            dev_err(&pdev->dev, "cannot find vedu IRQ\n");
            return TD_FAILURE;
        }
        vedu_set_vedu_addr(i, g_vedu_addr[i]);
        vedu_set_irq(i, g_vedu_irq[i]);
    }

    return TD_SUCCESS;
}


static td_s32 ot_jpge_probe(struct platform_device *pdev)
{
    struct resource *mem = TD_NULL;
    td_char jpge_dev_name[VEDU_DEV_NAME_LENGTH] = {'\0'};

    if (snprintf_s(jpge_dev_name, VEDU_DEV_NAME_LENGTH, VEDU_DEV_NAME_LENGTH - 1, "jpge") == -1) {
        osal_printk("%s, %d, set jpge name err!!\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, jpge_dev_name);

    g_jpge_chip_reg[0] = (td_void *)devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(g_jpge_chip_reg[0])) {
        osal_printk("%s,%d,remap jpge err!!\n", __FUNCTION__, __LINE__);
        return PTR_ERR(g_jpge_chip_reg[0]);
    }

    g_jpeg_irq[0] = osal_platform_get_irq_byname(pdev, jpge_dev_name);
    if (g_jpeg_irq[0] <= 0) {
        osal_printk("%s,%d,cannot find jpge IRQ!!\n", __FUNCTION__, __LINE__);
        dev_err(&pdev->dev, "cannot find jpge IRQ\n");
        return TD_FAILURE;
    }
    jpge_set_jpge_addr(0, g_jpge_chip_reg[0]);
    jpeg_set_irq(0, g_jpeg_irq[0]);
    return TD_SUCCESS;
}

static td_s32 ot_vpu_probe(struct platform_device *pdev)
{
    td_s32 ret;

    if (pdev == TD_NULL) {
        osal_printk("%s, %d, dev is NULL.\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }

    ret = ot_vedu_probe(pdev);
    if (ret != TD_SUCCESS) {
        osal_printk("%s, %d, vedu probe err.\n", __FUNCTION__, __LINE__);
        return ret;
    }

    ret = ot_jpge_probe(pdev);
    if (ret != TD_SUCCESS) {
        osal_printk("%s, %d, jpge probe err.\n", __FUNCTION__, __LINE__);
        return ret;
    }

    ret = vpu_mod_init();
    if (ret != TD_SUCCESS) {
        osal_printk("%s, %d, vpu mod_init err.\n", __FUNCTION__, __LINE__);
        return ret;
    }

    return ret;
}

static td_s32 ot_vpu_remove(struct platform_device *pdev)
{
    ot_unused(pdev);
    vpu_mod_exit();
    return 0;
}

static const struct of_device_id g_ot_vpu_match[] = {
    { .compatible = "vendor,venc" },
    {},
};
MODULE_DEVICE_TABLE(of, g_ot_vpu_match);

static struct platform_driver g_ot_vpu_driver = {
    .probe = ot_vpu_probe,
    .remove = ot_vpu_remove,
    .driver = {
        .name = "ot_venc",
        .of_match_table = g_ot_vpu_match,
    }
};

static int __init venc_mod_init(void)
{
    venc_set_max_chn_num(g_venc_max_chn_num);
    if (venc_module_init() != TD_SUCCESS) {
        return -1;
    }

    osal_platform_driver_register(&g_ot_vpu_driver);
    return 0;
}

static void __exit venc_mod_exit(void)
{
    venc_module_exit();
    osal_platform_driver_unregister(&g_ot_vpu_driver);
}

module_init(venc_mod_init);
module_exit(venc_mod_exit);

MODULE_LICENSE("Proprietary");
