/*****************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : kapi_rsa.c
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include "drv_osal_lib.h"
#include "cryp_rsa.h"

/** @}*/  /** <!-- ==== Structure Definition end ====*/

/******************************* API Code *****************************/
/** \addtogroup      rsa */
/** @{*/  /** <!-- [kapi]*/

/**
\brief   Kapi Init.
\retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
*/
hi_s32 kapi_rsa_init(void)
{
    hi_s32 ret = HI_FAILURE;

    HI_LOG_FUNC_ENTER();

    ret = cryp_rsa_init();
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(cryp_rsa_init, ret);
        return ret;
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

/**
\brief   Kapi Deinitialize.
\retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
*/
hi_s32 kapi_rsa_deinit(void)
{
    HI_LOG_FUNC_ENTER();

    cryp_rsa_deinit();

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 kapi_rsa_encrypt(cryp_rsa_key *key,
                     hi_cipher_rsa_enc_scheme scheme,
                     hi_u8 *in, hi_u32 inlen,
                     hi_u8 *out, hi_u32 *outlen)
{
    hi_s32 ret = HI_FAILURE;

    HI_LOG_FUNC_ENTER();

    ret = cryp_rsa_encrypt(key, scheme, in, inlen, out, outlen);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(cryp_rsa_encrypt, ret);
        return ret;
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 kapi_rsa_decrypt(cryp_rsa_key *key,
                     hi_cipher_rsa_enc_scheme scheme,
                     hi_u8 *in, hi_u32 inlen,
                     hi_u8 *out, hi_u32 *outlen)
{
    hi_s32 ret = HI_FAILURE;

    HI_LOG_FUNC_ENTER();

    ret = cryp_rsa_decrypt(key, scheme, in, inlen, out, outlen);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(cryp_rsa_decrypt, ret);
        return ret;
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 kapi_rsa_sign_hash(cryp_rsa_key *key,
                       hi_cipher_rsa_sign_scheme scheme,
                       hi_u8 *hash, hi_u32 hlen,
                       hi_u8 *sign, hi_u32 *signlen)
{
    hi_s32 ret = HI_FAILURE;

    HI_LOG_FUNC_ENTER();

    ret = cryp_rsa_sign_hash(key, scheme, hash, hlen, sign, signlen, hlen);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(cryp_rsa_sign_hash, ret);
        return ret;
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 kapi_rsa_verify_hash(cryp_rsa_key *key,
                         hi_cipher_rsa_sign_scheme scheme,
                         hi_u8 *hash, hi_u32 hlen,
                         hi_u8 *sign, hi_u32 signlen)
{
    hi_s32 ret = HI_FAILURE;

    HI_LOG_FUNC_ENTER();

    ret = cryp_rsa_verify_hash(key, scheme, hash, hlen, sign, signlen, hlen);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(cryp_rsa_verify_hash, ret);
        return ret;
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

/** @}*/  /** <!-- ==== Structure Definition end ====*/
