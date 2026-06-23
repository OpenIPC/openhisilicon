/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "uapi_trng.h"
#include <securec.h>

#include "uapi_common.h"
#include "crypto_ioctl_cmd.h"
#include "crypto_osal_user_lib.h"

#define TRNG_COMPAT_ERRNO(err_code)     UAPI_COMPAT_ERRNO(ERROR_MODULE_TRNG, err_code)

td_s32 unify_uapi_cipher_trng_get_random(td_u32 *randnum)
{
    td_s32 ret = TD_SUCCESS;
    trng_ctl_t trng_ctl;
    crypto_uapi_func_enter();

    if (randnum == TD_NULL) {
        return TRNG_COMPAT_ERRNO(ERROR_PARAM_IS_NULL);
    }
    ret = crypto_cipher_open();
    if (ret != TD_SUCCESS) {
        return ret;
    }

    (td_void)memset_s(&trng_ctl, sizeof(trng_ctl), 0, sizeof(trng_ctl));
    ret = crypto_cipher_ioctl(CRYPTO_CMD_TRNG_GET_RANDOM, &trng_ctl);
    if (ret != TD_SUCCESS) {
        printf("crypto_cipher_ioctl failed, ret is 0x%x\n", ret);
        goto exit_close;
    }
    *randnum = trng_ctl.randnum;

exit_close:
    crypto_cipher_close();
    crypto_uapi_func_exit();
    return ret;
}

td_s32 unify_uapi_cipher_trng_get_multi_random(td_u32 size, td_u8 *randnum)
{
    td_s32 ret = TD_SUCCESS;
    trng_multi_ctl_t trng_multi_ctl;
    crypto_uapi_func_enter();

    if (randnum == TD_NULL) {
        return TRNG_COMPAT_ERRNO(ERROR_PARAM_IS_NULL);
    }
    ret = crypto_cipher_open();
    if (ret != TD_SUCCESS) {
        return ret;
    }
    (td_void)memset_s(&trng_multi_ctl, sizeof(trng_multi_ctl), 0, sizeof(trng_multi_ctl));
    trng_multi_ctl.randnum.p = randnum;
    trng_multi_ctl.size = size;
    ret = crypto_cipher_ioctl(CRYPTO_CMD_TRNG_GET_MULTI_RANDOM, &trng_multi_ctl);
    if (ret != TD_SUCCESS) {
        printf("crypto_ioctl failed, ret is 0x%x\n", ret);
        goto exit_close;
    }

exit_close:
    crypto_cipher_close();
    crypto_uapi_func_exit();
    return ret;
}