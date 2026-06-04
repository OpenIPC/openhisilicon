/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include <linux/of_platform.h>
#include "drv_mipirx_ioctl.h"
#include "drv_mipirx_kapi.h"
#include "drv_mipirx_comm.h"
#include "drv_mipirx_osal.h"

static td_s32 mipirx_module_init(struct platform_device *pdev)
{
    td_s32 ret = 0;

    ret = drv_mipirx_ioctl_init();
    if (ret != TD_SUCCESS) {
        mipirx_log_err("init ioctl failed, ret :%d\n", ret);
        return ret;
    }

    if (drv_mipirx_kapi_init(pdev) != TD_SUCCESS) {
        mipirx_log_err("mipirx kapi init fail\n");
        return ret;
    }

    osal_printk("load mipirx_dev.ko ....OK!");
    return TD_SUCCESS;
}

static td_s32 mipirx_module_exit(struct platform_device *pdev)
{
    ot_unused(pdev);
    drv_mipirx_ioctl_deinit();
    drv_mipirx_kapi_deinit();
    osal_printk("unload mipirx_dev.ko ....OK!");
    return TD_SUCCESS;
}

static const struct of_device_id g_mipirx_match[] = {
    { .compatible = "vendor,mipi_rx" },
    {},
};

MODULE_DEVICE_TABLE(of, g_mipirx_match);

static struct platform_driver g_mipirx_driver = {
    .probe  = mipirx_module_init,
    .remove = mipirx_module_exit,
    .driver = {
        .name = MIPIRX_DEV_NAME,
        .of_match_table = g_mipirx_match,
    },
};

osal_module_platform_driver(g_mipirx_driver);
MODULE_LICENSE("GPL");
