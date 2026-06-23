/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "init_km.h"

#include <linux/module.h>
#include <linux/of_platform.h>
#include <asm/io.h>
#include <linux/slab.h>

#include <securec.h>

#include "drv_klad.h"
#include "ot_osal.h"
#include "mm_ext.h"
#include "kapi_km.h"
#include "dispatch_km.h"
#include "crypto_common_macro.h"
#include "crypto_osal_lib.h"

#define IRQ_TIMEOUT_MS                          (1000 * 5)

static td_s32 ot_km_open(td_void *private_data)
{
    crypto_unused(private_data);
    return TD_SUCCESS;
}

static td_s32 ot_km_release(td_void *private_data)
{
    crypto_unused(private_data);
    kapi_km_deinit();
    return TD_SUCCESS;
}

static osal_fileops g_dev_km_fops = {
    .open               = ot_km_open,
    .release            = ot_km_release,
    .cmd_list           = TD_NULL,
    .cmd_cnt            = 0
};

static osal_dev *g_km_dev = TD_NULL;

td_s32 crypto_km_init(td_void)
{
    td_s32 ret;

    g_km_dev = osal_dev_create("km");
    crypto_chk_return(g_km_dev == TD_NULL, TD_FAILURE, "osal_dev_create failed\n");

    g_dev_km_fops.cmd_list = get_km_func_list();
    g_dev_km_fops.cmd_cnt = get_km_func_num();

    g_km_dev->minor = UMAP_KM_MINOR_BASE;
    g_km_dev->fops = &g_dev_km_fops;

    ret = osal_dev_register(g_km_dev);
    crypto_chk_goto(ret != TD_SUCCESS, error_dev_destroy, "osal_dev_register failed, ret is 0x%x\n", ret);

    ret = kapi_km_env_init();
    crypto_chk_goto(ret != TD_SUCCESS, error_dev_unregister, "kapi_km_init failed, ret is 0x%x\n", ret);

    return ret;

error_dev_unregister:
    osal_dev_unregister(g_km_dev);
error_dev_destroy:
    if (g_km_dev != TD_NULL) {
        osal_dev_destroy(g_km_dev);
        g_km_dev = TD_NULL;
    }
    return TD_FAILURE;
}

td_void crypto_km_deinit(td_void)
{
    kapi_km_env_deinit();
    osal_dev_unregister(g_km_dev);
    osal_dev_destroy(g_km_dev);
    g_km_dev = TD_NULL;
}