/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>
#include "drv_mipirx_comm.h"
#include "drv_mipirx_osal.h"
static osal_mutex g_mipi_mutex;

td_s32 drv_mipirx_init_mutex(td_void)
{
    td_s32 ret;
    ret = osal_mutex_init(&g_mipi_mutex);
    if (ret < 0) {
        mipirx_log_err("mipirx init mutex fail!\n");
        return ret;
    }
    return TD_SUCCESS;
}

td_void drv_mipirx_deinit_mutex(td_void)
{
    osal_mutex_destroy(&g_mipi_mutex);
}

td_s32 drv_mipirx_interruptible_mutex_lock(td_void)
{
    return osal_mutex_lock_interruptible(&g_mipi_mutex);
}

td_void drv_mipirx_mutex_unlock(td_void)
{
    osal_mutex_unlock(&g_mipi_mutex);
}

td_s32 drv_mipirx_get_reg_base(td_void *pdev, td_u32 **reg_base, const td_char *name)
{
    struct resource *dtsi_addr = TD_NULL;
    void *reg_map = TD_NULL;

    if (pdev == TD_NULL || name == TD_NULL || reg_base == TD_NULL) {
        mipirx_log_err("input param error!\n");
        return TD_FAILURE;
    }

    dtsi_addr = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, name);
    reg_map = devm_ioremap_resource(&((struct platform_device *) pdev)->dev, dtsi_addr);
    if (reg_map == TD_NULL) {
        mipirx_log_err("reg ioremap error!\n");
        return TD_FAILURE;
    }

    *reg_base = reg_map;
    return TD_SUCCESS;
}

td_s32 drv_mipirx_register_irq(td_u32 irq_num, td_u32 (*func)(td_u32, void *))
{
    td_s32 ret;
    ret = osal_irq_request(irq_num, (osal_irq_handler) func, TD_NULL, "MIPI_RX", func);
    if (ret < 0) {
        mipirx_log_err("failed to register irq.\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

td_s32 drv_mipirx_unregister_irq(td_u32 irq_num, td_u32 (*func)(td_u32, void *))
{
    osal_irq_free(irq_num, func);
    return TD_SUCCESS;
}

td_s32 drv_mipirx_get_irq_num(td_void *pdev, td_u32 *irq)
{
    td_u32 irq_num;

    irq_num = osal_platform_get_irq_byname(pdev, "mipi_rx");
    if (irq_num <= 0) {
        mipirx_log_err("can not find mipi_rx IRQ\n");
        return TD_FAILURE;
    }

    *irq = irq_num;

    return TD_SUCCESS;
}
