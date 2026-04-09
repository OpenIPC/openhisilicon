/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : drivers for kapi rsa of cipher.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#include "drv_osal_lib.h"
#include "cryp_rsa.h"

/* ****************************** API Code **************************** */
/*
 * brief   Kapi Init.
 * retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
 */
hi_s32 kapi_rsa_init(void)
{
    hi_s32 ret;

    hi_log_func_enter();

    ret = cryp_rsa_init();
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cryp_rsa_init, ret);
        return ret;
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

/*
 * brief   Kapi Deinitialize.
 * retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
 */
hi_s32 kapi_rsa_deinit(void)
{
    hi_log_func_enter();

    cryp_rsa_deinit();

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 kapi_rsa_encrypt(cryp_rsa_key *key, cryp_rsa_crypt_data *rsa)
{
    hi_s32 ret;

    hi_log_func_enter();

    ret = cryp_rsa_encrypt(key, rsa);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cryp_rsa_encrypt, ret);
        return ret;
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 kapi_rsa_decrypt(cryp_rsa_key *key, cryp_rsa_crypt_data *rsa)
{
    hi_s32 ret;

    hi_log_func_enter();

    ret = cryp_rsa_decrypt(key, rsa);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cryp_rsa_decrypt, ret);
        return ret;
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 kapi_rsa_sign_hash(cryp_rsa_key *key, cryp_rsa_sign_data *rsa)
{
    hi_s32 ret;

    hi_log_func_enter();

    ret = cryp_rsa_sign_hash(key, rsa);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cryp_rsa_sign_hash, ret);
        return ret;
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 kapi_rsa_verify_hash(cryp_rsa_key *key, cryp_rsa_sign_data *rsa)
{
    hi_s32 ret;

    hi_log_func_enter();

    ret = cryp_rsa_verify_hash(key, rsa);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cryp_rsa_verify_hash, ret);
        return ret;
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}
