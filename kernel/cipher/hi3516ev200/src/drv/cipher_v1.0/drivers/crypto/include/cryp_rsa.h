/*****************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : cryp_rsa.h
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#ifndef __CRYP_RSA_H_
#define __CRYP_RSA_H_

#include "drv_osal_lib.h"
#include "drv_srsa.h"

/******************************* API Declaration *****************************/
/** \addtogroup      rsa */
/** @{ */  /** <!--[rsa]*/

/** @}*/  /** <!-- ==== Structure Definition end ====*/

/******************************* API Code *****************************/
/** \addtogroup      cipher drivers*/
/** @{*/  /** <!-- [cipher]*/

/**
* \brief          Initialize crypto of rsa *
*/
hi_s32 cryp_rsa_init(hi_void);

/**
* \brief          Deinitialize crypto of rsa *
*/
hi_void cryp_rsa_deinit(hi_void);

/**
\brief RSA encryption a plaintext with a RSA key.
\param[in] key:         rsa key.
\param[in] scheme:      rsa encrypt scheme.
\param[in] in:          input data to be encryption
\param[in] in:          input data to be encryption
\param[out] inlen:      length of input data to be encryption
\param[out] out:        output data to be encryption
\param[out] outlen:     length of output data to be encryption
\retval ::HI_SUCCESS  Call this API successful.
\retval ::HI_FAILURE  Call this API fails.
\see \n
N/A
*/
hi_s32 cryp_rsa_encrypt(cryp_rsa_key *key, hi_cipher_rsa_enc_scheme scheme,
                        hi_u8 *in, hi_u32 inlen,
                        hi_u8 *out, hi_u32 *outlen);

/**
\brief RSA decryption a plaintext with a RSA key.
\param[in] key:         rsa key.
\param[in] scheme:      rsa encrypt scheme.
\param[in] in:          input data to be encryption
\param[in] in:          input data to be encryption
\param[out] inlen:      length of input data to be encryption
\param[out] out:        output data to be encryption
\param[out] outlen:     length of output data to be encryption
\retval ::HI_SUCCESS  Call this API successful.
\retval ::HI_FAILURE  Call this API fails.
\see \n
N/A
*/
hi_s32 cryp_rsa_decrypt(cryp_rsa_key *key, hi_cipher_rsa_enc_scheme scheme,
                        hi_u8 *in, hi_u32 inlen,
                        hi_u8 *out, hi_u32 *outlen);

/**
\brief RSA sign a hash value with a RSA private key.
\param[in] key:         rsa key.
\param[in] scheme:      rsa sign scheme.
\param[in] in:          input data to be encryption
\param[in] in:          input data to be encryption
\param[out] inlen:      length of input data to be encryption
\param[out] out:        output data to be encryption
\param[out] outlen:     length of output data to be encryption
\retval ::HI_SUCCESS  Call this API successful.
\retval ::HI_FAILURE  Call this API fails.
\see \n
N/A
*/
hi_s32 cryp_rsa_sign_hash(cryp_rsa_key *key, hi_cipher_rsa_sign_scheme scheme,
                          hi_u8 *in, hi_u32 inlen,
                          hi_u8 *out, hi_u32 *outlen, hi_u32 saltlen);

/**
\brief RSA verify a hash value with a RSA public key.
\param[in] key:         rsa key.
\param[in] scheme:      rsa sign scheme.
\param[in] in:          input data to be encryption
\param[in] in:          input data to be encryption
\param[out] inlen:      length of input data to be encryption
\param[out] out:        output data to be encryption
\param[out] outlen:     length of output data to be encryption
\retval ::HI_SUCCESS  Call this API successful.
\retval ::HI_FAILURE  Call this API fails.
\see \n
N/A
*/
hi_s32 cryp_rsa_verify_hash(cryp_rsa_key *key, hi_cipher_rsa_sign_scheme scheme,
                            hi_u8 *in, hi_u32 inlen,
                            hi_u8 *sign, hi_u32 signlen, hi_u32 saltlen);

/**
\brief Generate a RSA private key.
\param[in] numbits:     bit numbers of the integer public modulus.
\param[in] exponent:    value of public exponent
\param[out] key:        rsa key.
\retval ::HI_SUCCESS  Call this API successful.
\retval ::HI_FAILURE  Call this API fails.
\see \n
N/A
*/
hi_s32 cryp_rsa_gen_key(hi_u32 numbits, hi_u32 exponent, cryp_rsa_key *key);

/**
\brief Generate random.
N/A
*/
int mbedtls_get_random(hi_void *param, hi_u8 *rand, size_t size);

/** @} */  /** <!-- ==== API declaration end ==== */
#endif
