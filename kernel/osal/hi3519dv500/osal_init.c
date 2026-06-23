/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include <linux/of_irq.h>
#include <linux/platform_device.h>
#include "ot_osal.h"
#include "ot_media.h"
#include "osal_inner.h"
// warning "you should redefine OSAL_PROC_DIR. default is umap"
#define OSAL_PROC_DIR "umap"

static struct platform_device *g_osal_virt_dev = NULL;

int osal_get_irq_by_name(const char *name)
{
    struct resource *res = NULL;

    if (g_osal_virt_dev == NULL) {
        osal_log("osal_virt_dev is not registered!\n");
        return -1;
    }

    if (name == NULL) {
        osal_log("input module name is NULL!\n");
        return -1;
    }

    res = platform_get_resource_byname(g_osal_virt_dev, IORESOURCE_IRQ, name);
    if (res == NULL) {
        osal_log("module %s platform_get_resource_byname fail!\n", name);
        return -1;
    }

    return (int)res->start;
}
EXPORT_SYMBOL(osal_get_irq_by_name);

static int drv_osal_virt_probe(struct platform_device *dev)
{
    osal_log("virtual device initialization.\n");
    g_osal_virt_dev = dev;
    return 0;
}

static const struct of_device_id g_osal_virt_dev_match[] __maybe_unused = {
    { .compatible = "virt-device", },
    {},
};

MODULE_DEVICE_TABLE(of, g_osal_virt_dev_match);

struct platform_driver g_osal_virt_platform_drv = {
    .probe  = drv_osal_virt_probe,
    .driver = {
        .name           = "virt-device",
        .owner          = THIS_MODULE,
        .of_match_table = of_match_ptr(g_osal_virt_dev_match),
    },
};

static int ext_drv_osal_init(void)
{
    int ret;

    ret = platform_driver_register(&g_osal_virt_platform_drv);
    if (ret != 0) {
        osal_log("register driver failed! ret = %#x.\n", ret);
        return -1;
    }

    ret = drv_pm_mod_init();
    if (ret != OSAL_SUCCESS) {
        osal_log("drv_pm_mod_init failed! ret = %#x.\n", ret);
        goto exit_platform;
    }
    osal_proc_init(OSAL_PROC_DIR);
    return OSAL_SUCCESS;

exit_platform:
    platform_driver_unregister(&g_osal_virt_platform_drv);
    return ret;
}

static void ext_drv_osal_exit(void)
{
    platform_driver_unregister(&g_osal_virt_platform_drv);
    drv_pm_mod_exit();
    osal_proc_exit(OSAL_PROC_DIR);
    return;
}

static int osal_module_init(void)
{
    int ret;
    ret = ext_drv_osal_init();
    if (ret != 0) {
        osal_log("common init failed! ret = %#x.\n", ret);
        goto err_osal_exit;
    }
    return 0;

err_osal_exit:
    ext_drv_osal_exit();

    return -1;
}

static void osal_module_exit(void)
{
    ext_drv_osal_exit();

    return;
}

module_init(osal_module_init);
module_exit(osal_module_exit);

MODULE_AUTHOR("OSAL");
MODULE_LICENSE("GPL");
