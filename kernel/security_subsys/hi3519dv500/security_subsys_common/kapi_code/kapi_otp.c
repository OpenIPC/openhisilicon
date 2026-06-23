/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "kapi_otp.h"

#include "drv_otp.h"
#include "crypto_common_macro.h"
#include "crypto_osal_adapt.h"
#include "crypto_osal_lib.h"

crypto_mutex g_otp_mutex;

#define kapi_otp_mutex_lock() do {          \
    crypto_mutex_lock(&g_otp_mutex);            \
} while (0)

#define kapi_otp_mutex_unlock() do {        \
    crypto_mutex_unlock(&g_otp_mutex);          \
} while (0)

td_s32 kapi_otp_init(td_void)
{
    td_s32 ret = TD_SUCCESS;
    crypto_kapi_func_enter();

    ret = crypto_mutex_init(&g_otp_mutex);
    crypto_chk_print(ret != TD_SUCCESS, "crypto_mutex_init failed, ret is 0x%x\n", ret);
    crypto_kapi_func_exit();

    return ret;
}

td_s32 kapi_otp_deinit(td_void)
{
    crypto_kapi_func_enter();
    crypto_mutex_destroy(&g_otp_mutex);
    crypto_kapi_func_exit();
    return TD_SUCCESS;
}

td_s32 kapi_otp_read_word(td_u32 offset, td_u32 *data)
{
    td_s32 ret = TD_SUCCESS;
    crypto_kapi_func_enter();

    kapi_otp_mutex_lock();
    ret = drv_otp_read_word(offset, data);
    crypto_chk_print(ret != TD_SUCCESS, "drv_otp_read_word failed, ret is 0x%x\n", ret);
    kapi_otp_mutex_unlock();

    crypto_kapi_func_exit();
    return ret;
}
CRYPTO_EXPORT_SYMBOL(kapi_otp_read_word);

td_s32 kapi_otp_read_byte(td_u32 offset, td_u8 *data)
{
    td_s32 ret = TD_SUCCESS;
    crypto_kapi_func_enter();

    kapi_otp_mutex_lock();
    ret = drv_otp_read_byte(offset, data);
    crypto_chk_print(ret != TD_SUCCESS, "drv_otp_read_byte failed, ret is 0x%x\n", ret);
    kapi_otp_mutex_unlock();

    crypto_kapi_func_exit();
    return ret;
}
CRYPTO_EXPORT_SYMBOL(kapi_otp_read_byte);

td_s32 kapi_otp_write_byte(td_u32 offset, td_u8 data)
{
    td_s32 ret = TD_SUCCESS;
    crypto_kapi_func_enter();

    kapi_otp_mutex_lock();
    ret = drv_otp_write_byte(offset, data);
    crypto_chk_print(ret != TD_SUCCESS, "drv_otp_write_byte failed, ret is 0x%x\n", ret);
    kapi_otp_mutex_unlock();

    crypto_kapi_func_exit();
    return ret;
}
CRYPTO_EXPORT_SYMBOL(kapi_otp_write_byte);