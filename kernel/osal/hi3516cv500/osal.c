/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2016-2019.
 * Description: osal main source file.
 * Author: Hisilicon multimedia software group
 * Create: 2016-11-11
 */

#include "osal.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of_platform.h>
#include <linux/of.h>
#include "hi_osal.h"
#include "osal_mmz.h"
#include "../../compat/kernel_compat.h"

static int __init osal_init(void)
{
    int ret;

    if (mem_check_module_param() == -1) {
        return -1;
    }

    osal_device_init();
    osal_proc_init();
    himedia_init();
    ret = media_mem_init();
    if (ret) {
        himedia_exit();
        osal_proc_exit();
        return ret;
    }
    osal_printk("hi_osal %s init success!\n", HI_OSAL_VERSION);
    return 0;
}

static void __exit osal_exit(void)
{
    osal_proc_exit();
    himedia_exit();
    media_mem_exit();
    osal_printk("hi_osal v1.0 exit!\n");
}

static int osal_probe(struct platform_device* pdev)
{
    int ret;

    osal_platform_get_module_param(pdev, "anony", int, &anony);
    osal_platform_get_modparam_string(pdev, "setup_zones", MMZ_SETUP_CMDLINE_LEN, g_setup_zones);
    osal_platform_get_modparam_string(pdev, "mmap_zones", MMZ_SETUP_CMDLINE_LEN, g_mmap_zones);
    osal_platform_get_modparam_string(pdev, "setup_allocator", MMZ_ALLOCATOR_NAME_LEN, g_setup_allocator);

    ret = osal_init();
    if (ret) {
        return ret;
    }

    return 0;
}

static compat_platform_remove_ret osal_remove(struct platform_device* pdev)
{
    osal_exit();
    compat_platform_remove_return;
}

static const struct of_device_id g_osal_match[] = {
    { .compatible = "hisilicon,osal" },
    { },
};
MODULE_DEVICE_TABLE(of, g_osal_match);

static struct platform_driver g_osal_driver = {
    .probe  = osal_probe,
    .remove = osal_remove,
    .driver = {
        .name = "osal",
        .of_match_table = g_osal_match,
    },
};

osal_module_platform_driver(g_osal_driver);

MODULE_AUTHOR("Hisilicon");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");

/*
 * printk shim: 4.9-era blobs reference 'printk' directly, but 6.x+ renamed
 * it to '_printk' (printk is now an inline wrapper). Provide a real function
 * named 'printk' that blob modules can link against.
 */
#ifdef COMPAT_NEEDS_PRINTK_SHIM
#include <linux/printk.h>
#undef printk
asmlinkage __visible int printk(const char *fmt, ...)
{
	va_list args;
	int r;
	va_start(args, fmt);
	r = vprintk(fmt, args);
	va_end(args);
	return r;
}
EXPORT_SYMBOL(printk);
#endif
