/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : head file cryp hash.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#ifndef __CRYP_RSA_H_
#define __CRYP_RSA_H_

#include "drv_cipher_kapi.h"
#include "drv_osal_lib.h"
#include "drv_srsa.h"

#define CRYP_TRNG_TIMEOUT  (-1)

typedef enum {
    RSA_BLOCK_YTPE_0 = 0X0,
    RSA_BLOCK_YTPE_1,
    RSA_BLOCK_YTPE_2,
} rsa_pkcs1_padding_type;

typedef struct {
    hi_u32 mode;
    hi_u32 klen;
    hi_u8 *in;
    hi_u32 in_len;
    hi_u8 *out;
    hi_u32 *out_len;
    hi_u8 bt;
} rsa_padding_pack;

/*
 * brief          Initialize crypto of rsa *
 */
hi_s32 cryp_rsa_init(hi_void);

/*
 * brief          Deinitialize crypto of rsa *
 */
hi_void cryp_rsa_deinit(hi_void);

/*
 * brief RSA encryption a plaintext with a RSA key.
 * param[in] key:         rsa key.
 * param[in] rsa_crypt:   rsa encrypt data.
 * retval ::HI_SUCCESS  Call this API successful.
 * retval ::HI_FAILURE  Call this API fails.
 * see \n
 * N/A
 */
hi_s32 cryp_rsa_encrypt(cryp_rsa_key *key, cryp_rsa_crypt_data *rsa_crypt);

/*
 * brief RSA decryption a plaintext with a RSA key.
 * param[in] key:         rsa key.
 * param[in] rsa_crypt:   rsa decrypt data.
 * retval ::HI_SUCCESS  Call this API successful.
 * retval ::HI_FAILURE  Call this API fails.
 * see \n
 * N/A
 */
hi_s32 cryp_rsa_decrypt(cryp_rsa_key *key, cryp_rsa_crypt_data *rsa_crypt);

/*
 * brief RSA sign a hash value with a RSA private key.
 * param[in] key:         rsa key.
 * param[in] rsa_sign:    rsa sign data.
 * retval ::HI_SUCCESS  Call this API successful.
 * retval ::HI_FAILURE  Call this API fails.
 * see \n
 * N/A
 */
hi_s32 cryp_rsa_sign_hash(cryp_rsa_key *key, cryp_rsa_sign_data *rsa_sign);

/*
 * brief RSA verify a hash value with a RSA public key.
 * param[in] key:         rsa key.
 * param[in] rsa_verify:  rsa sign data.
 * retval ::HI_SUCCESS  Call this API successful.
 * retval ::HI_FAILURE  Call this API fails.
 * see \n
 * N/A
 */
hi_s32 cryp_rsa_verify_hash(cryp_rsa_key *key, cryp_rsa_sign_data *rsa_verify);

/*
 * brief Generate a RSA private key.
 * param[in] numbits:     bit numbers of the integer public modulus.
 * param[in] exponent:    value of public exponent
 * param[out] key:        rsa key.
 * retval ::HI_SUCCESS  Call this API successful.
 * retval ::HI_FAILURE  Call this API fails.
 * see \n
 * N/A
 */
hi_s32 cryp_rsa_gen_key(hi_u32 numbits, hi_u32 exponent, cryp_rsa_key *key);

/*
 * brief Generate random.
 * N/A
 */
int mbedtls_get_random(hi_void *param, hi_u8 *rand, size_t size);
#endif
