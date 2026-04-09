/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2018-2019. All rights reserved.
 * Description   : drivers for cipher init device.
 * Author        : Hisilicon multimedia software group
 * Create        : 2018-10-20
 */

#include "hi_types.h"
#include "drv_osal_lib.h"

#ifndef __HuaweiLite__
#ifdef IRQ_DTS_SUPPORT
#include <linux/of_platform.h>

static int hi35xx_cipher_probe(struct platform_device *pdev)
{
    hi_u32 irq_num;

#if defined(ARCH_TYPE_amp) && !defined(CRYPTO_SEC_CPU)
    irq_num = platform_get_irq_byname(pdev, "nonsec_cipher");
    if (irq_num <= 0) {
        dev_err(&pdev->dev, "cannot find cipher IRQ number %d.\n", irq_num);
        return -1;
    }
    module_set_irq(CRYPTO_MODULE_ID_SYMC, irq_num);

    irq_num = platform_get_irq_byname(pdev, "nonsec_hash");
    if (irq_num <= 0) {
        dev_err(&pdev->dev, "cannot find trng IRQ number %d.\n", irq_num);
        return -1;
    }
    module_set_irq(CRYPTO_MODULE_ID_HASH, irq_num);

    irq_num = platform_get_irq_byname(pdev, "nonsec_rsa");
    if (irq_num <= 0) {
        dev_err(&pdev->dev, "cannot find rsa IRQ number %d.\n", irq_num);
        return -1;
    }
    module_set_irq(CRYPTO_MODULE_ID_IFEP_RSA, irq_num);
#else
    irq_num = platform_get_irq_byname(pdev, "cipher");
    if (irq_num <= 0) {
        dev_err(&pdev->dev, "cannot find cipher IRQ number %d.\n", irq_num);
        return -1;
    }
    module_set_irq(CRYPTO_MODULE_ID_SYMC, irq_num);

    irq_num = platform_get_irq_byname(pdev, "hash");
    if (irq_num <= 0) {
        dev_err(&pdev->dev, "cannot find trng IRQ number %d.\n", irq_num);
        return -1;
    }
    module_set_irq(CRYPTO_MODULE_ID_HASH, irq_num);

    irq_num = platform_get_irq_byname(pdev, "rsa");
    if (irq_num <= 0) {
        dev_err(&pdev->dev, "cannot find rsa IRQ number %d.\n", irq_num);
        return -1;
    }
    module_set_irq(CRYPTO_MODULE_ID_IFEP_RSA, irq_num);
#endif

    if (cipher_drv_mod_init() != HI_SUCCESS) {
        return -1;
    }

    return 0;
}

static int hi35xx_cipher_remove(struct platform_device *pdev)
{
    cipher_drv_mod_exit();

    return 0;
}

static const struct of_device_id g_hi35xx_cipher_match[] = {
    { .compatible = "hisilicon,hisi-cipher" },
    { },
};
MODULE_DEVICE_TABLE(of, g_hi35xx_cipher_match);

static struct platform_driver g_hi35xx_cipher_driver = {
    .probe          = hi35xx_cipher_probe,
    .remove         = hi35xx_cipher_remove,
    .driver         = {
        .name   = "hi35xx_cipher",
        .of_match_table = g_hi35xx_cipher_match,
    },
};

module_platform_driver(g_hi35xx_cipher_driver);
MODULE_LICENSE("GPL");
#else
static int __init cipher_mod_init(void)
{
    return cipher_drv_mod_init();
}

static void __exit cipher_mod_exit(void)
{
    cipher_drv_mod_exit();
}

module_init(cipher_mod_init);
module_exit(cipher_mod_exit);

MODULE_LICENSE("Proprietary");
#endif
#endif
