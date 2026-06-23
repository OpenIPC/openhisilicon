/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "init_otp.h"

#include <asm/io.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/slab.h>

#include <securec.h>

#include "ot_osal.h"
#include "mm_ext.h"
#include "kapi_otp.h"
#include "dispatch_otp.h"
#include "crypto_osal_adapt.h"
#include "crypto_common_macro.h"
#include "crypto_osal_lib.h"

static td_s32 ot_otp_open(td_void *private_data)
{
    crypto_unused(private_data);
    return TD_SUCCESS;
}

static td_s32 ot_otp_release(td_void *private_data)
{
    crypto_unused(private_data);
    return TD_SUCCESS;
}

static osal_fileops g_dev_otp_fops = {
    .open               = ot_otp_open,
    .release            = ot_otp_release,
    .cmd_list           = TD_NULL,
    .cmd_cnt            = 0
};

static osal_dev *g_otp_dev = TD_NULL;

td_s32 crypto_otp_init(td_void)
{
    td_s32 ret;

    g_otp_dev = osal_dev_create("otp");
    crypto_chk_return(g_otp_dev == TD_NULL, TD_FAILURE, "osal_dev_create failed\n");

    g_dev_otp_fops.cmd_list = get_otp_func_list();
    g_dev_otp_fops.cmd_cnt = get_otp_func_num();

    g_otp_dev->minor = UMAP_OTP_MINOR_BASE;
    g_otp_dev->fops = &g_dev_otp_fops;

    ret = osal_dev_register(g_otp_dev);
    crypto_chk_goto(ret != TD_SUCCESS, error_dev_destroy, "osal_dev_register failed, ret is 0x%x\n", ret);

    ret = kapi_otp_init();
    crypto_chk_goto(ret != TD_SUCCESS, error_dev_unregister, "kapi_otp_init failed, ret is 0x%x\n", ret);

    return ret;

error_dev_unregister:
    osal_dev_unregister(g_otp_dev);
error_dev_destroy:
    if (g_otp_dev != TD_NULL) {
        osal_dev_destroy(g_otp_dev);
        g_otp_dev = TD_NULL;
    }
    return TD_FAILURE;
}

td_void crypto_otp_deinit(td_void)
{
    kapi_otp_deinit();
    osal_dev_unregister(g_otp_dev);
    osal_dev_destroy(g_otp_dev);

    g_otp_dev = TD_NULL;
}