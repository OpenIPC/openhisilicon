/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "kapi_inner.h"

#include "crypto_drv_common.h"

td_s32 kapi_env_init(td_void)
{
    td_s32 ret;
    /* KAPI Init. */
    ret = kapi_cipher_hash_env_init();
    crypto_chk_return(ret != TD_SUCCESS, ret, "kapi_cipher_hash_env_init failed, ret is 0x%x.\n", ret);

    ret = kapi_cipher_trng_env_init();
    crypto_chk_goto(ret != TD_SUCCESS, error_kapi_hash_env_deinit,
        "kapi_cipher_trng_env_init failed, ret is 0x%x.\n", ret);

    ret = kapi_cipher_symc_env_init();
    crypto_chk_goto(ret != TD_SUCCESS, error_kapi_trng_env_deinit,
        "kapi_cipher_symc_env_init failed, ret is 0x%x.\n", ret);

    ret = kapi_cipher_pke_env_init();
    crypto_chk_goto(ret != TD_SUCCESS, error_kapi_symc_env_deinit,
        "kapi_cipher_pke_env_init failed, ret is 0x%x.\n", ret);

    return ret;
error_kapi_symc_env_deinit:
    kapi_cipher_symc_env_deinit();
error_kapi_trng_env_deinit:
    kapi_cipher_trng_env_deinit();
error_kapi_hash_env_deinit:
    kapi_cipher_hash_env_deinit();
    return ret;
}

td_void kapi_env_deinit(td_void)
{
    kapi_cipher_pke_env_deinit();
    kapi_cipher_symc_env_deinit();
    kapi_cipher_trng_env_deinit();
    kapi_cipher_hash_env_deinit();
}