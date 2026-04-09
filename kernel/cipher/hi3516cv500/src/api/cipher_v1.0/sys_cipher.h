/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : head file for sys cipher.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#ifndef __SYS_CIPHER_H__
#define __SYS_CIPHER_H__

#include "hi_cipher_compat.h"
#include "user_osal_lib.h"

#ifdef __cplusplus
extern "C" {
#endif    /* __cplusplus */

typedef struct {
    hi_u32 id;                          /* The channel number. */
    hi_u32 hard_key;                    /* Whether use the hard key or not. */
    hi_cipher_alg alg;                  /* The symmetric cipher algorithm. */
    hi_cipher_work_mode work_mode;      /* The symmetric cipher mode. */
    hi_cipher_bit_width bit_width;      /* The symmetric cipher bit width. */
    hi_cipher_key_len key_len;          /* The symmetric cipher key len. */
    hi_cipher_sm1_round sm1_round_num;  /* The round number of sm1. */
    hi_u8 *fkey;                        /* First key buffer, defualt. */
    hi_u8 *skey;                        /* Second key buffer, expand. */
    hi_u32 klen;                        /* The length of fkey buffer, if skey is not null,
                                           equal to the length of skey. */
    hi_u8 *iv;                          /* The buffer of iv. */
    hi_u32 iv_len;                      /* The length of iv buffer. */
    hi_u32 iv_usage;                    /* iv change. */
    compat_addr aad;                    /* Associated data. */
    hi_u32 aad_len;                     /* Associated data length. */
    hi_u32 tag_len;                     /* Tag length. */
} sys_symc_ctrl;

/* rsa sign and verify struct for sys class. */
typedef struct {
    hi_cipher_rsa_sign_scheme scheme;
    cryp_rsa_key key;
    hi_u8 *hash;
    hi_u32 hash_len;
} sys_rsa_sign_verify;

/*
 * brief   mpi Init.
 * retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
 */
hi_s32 sys_symc_init(hi_void);

/*
 * brief   Kapi Deinit.
 * retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
 */
hi_s32 sys_symc_deinit(hi_void);

/*
 * brief   Create symc handle.
 * param[in]  id The channel number.
 * param[in]  uuid The user identification.
 * retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
 */
hi_s32 sys_symc_create(hi_u32 *id);

/*
 * brief   Destroy symc handle.
 * param[in]  id The channel number.
 * param[in]  uuid The user identification.
 * retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
 */
hi_s32 sys_symc_destroy(hi_u32 id);

/*
 * brief  set work params.
 * param[in]  ctrl          Sys symc config ctrl.
 * retval     On success,   HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
 */
hi_s32 sys_symc_cfg(sys_symc_ctrl *ctrl);


/*
 * brief  get work params.
 * param[in]  id The channel number.
 * param[out] ctrl infomation.
 * retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
 */
hi_s32 sys_symc_get_cfg(hi_u32 id, hi_cipher_ctrl *ctrl);

/*
 * brief          SYMC  buffer encryption/decryption.
 *
 * Note: Due to the nature of aes you should use the same key schedule for
 * both encryption and decryption.
 *
 * param[in] id   The channel number.
 * param input    buffer holding the input data
 * param output   buffer holding the output data
 * param length   length of the input data
 * param decrypt  decrypt or encrypt
 *
 * return         0 if successful
 */
hi_s32 sys_symc_crypto(hi_u32 id, const compat_addr in, const compat_addr out, hi_u32 len, hi_u32 operation);

/*
 * brief          SYMC multiple buffer encryption/decryption.
 *
 * Note: Due to the nature of aes you should use the same key schedule for
 * both encryption and decryption.
 *
 * param[in]  id The channel number.
 * param pkg       Buffer of package infomation
 * param pkg_num   Number of package infomation
 * param decrypt   decrypt or encrypt
 *
 * return         0 if successful
 */
hi_s32 sys_symc_crypto_multi(hi_u32 id, const hi_void *pack, hi_u32 pack_num, hi_u32 operation);

/*
 * brief          SYMC multiple buffer encryption/decryption.
 * param[in]  id The channel number.
 * param[in]  tag tag data of CCM/GCM
 * param[in/out]  tag length.
 * param uuid uuid The user identification.
 *
 * return         0 if successful
 */
hi_s32 sys_aead_get_tag(hi_u32 id, hi_u8 *tag, hi_u32 *tag_len);

/*
 * brief       Klad encrypt clear key.
 * keysel[in]  keysel root key seclect.
 * param[in]   target target seclect
 * param[in]   clear clear key
 * param[out]  encrypt encrypt key
 * param uuid  uuid The user identification.
 *
 * return         0 if successful
 */
hi_s32 sys_klad_encrypt_key(hi_u32 keysel, hi_u32 target, const hi_u8 clear[AES_BLOCK_SIZE],
    hi_u8 encrypt[AES_BLOCK_SIZE], hi_u32 key_len);

/*
 * brief   Kapi Init.
 * retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
 */
hi_s32 sys_hash_init(hi_void);

/*
 * brief   Kapi Deinit.
 * retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
 */
hi_s32 sys_hash_deinit(hi_void);

/*
 * brief          HASH context setup.
 *
 *
 * param[out] id The channel number.
 * param[in] type    Hash type
 * param[in] key     hmac key
 * param[in] keylen  hmac key length
 *
 * return         0 if successful
 */
hi_s32 sys_hash_start(hi_u32 *id, hi_cipher_hash_type type,
                      const hi_u8 *key, hi_u32 keylen);

/*
 * brief          HASH process buffer.
 *
 * param[in] id       The channel number.
 * param[in] input    buffer holding the input data
 * param[in] length   length of the input data
 * param[in] src      source of hash message
 *
 * return         0 if successful
 */
hi_s32 sys_hash_update(hi_u32 id, const hi_u8 *input, hi_u32 length,
                       hash_chunk_src src);

/*
 * brief          HASH final digest.
 *
 * param[in]  id      The channel number.
 * param[out] hash    buffer holding the hash data
 * param[out] hashlen length of the hash data
 * param[in]  uuid    uuid The user identification.
 *
 * return         0 if successful
 */
hi_s32 sys_hash_finish(hi_u32 id, hi_u8 *hash, hi_u32 *hashlen);

/*
 * brief   Kapi Init.
 * retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
 */
hi_s32 sys_rsa_init(hi_void);

/*
 * brief   Kapi Deinitialize.
 * retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
 */
hi_s32 sys_rsa_deinit(hi_void);

/*
 * brief RSA encryption a plaintext with a RSA private key.
 *
 * param[in] key:     rsa key struct.
 * param[in] scheme:  rsa encrypt/decrypt scheme.
 * param[in/out] rsa_data: input and output data of encryption
 * retval HI_SUCCESS  Call this API successful
 * retval HI_FAILURE  Call this API fails.
 */
hi_s32 sys_rsa_encrypt(const cryp_rsa_key *key, hi_cipher_rsa_encrypt_scheme scheme, hi_cipher_rsa_crypt *rsa_data);

/*
 * brief RSA decryption a ciphertext with a RSA public key.
 *
 * param[in] key:     rsa key struct.
 * param[in] scheme:  rsa encrypt/decrypt scheme.
 * param[in/out] rsa_data: input and output data of decryption
 * retval HI_SUCCESS  Call this API successful
 * retval HI_FAILURE  Call this API fails.
 */
hi_s32 sys_rsa_decrypt(const cryp_rsa_key *key, hi_cipher_rsa_encrypt_scheme scheme, hi_cipher_rsa_crypt *rsa_data);

/*
 * brief RSA signature a context with appendix, where a signer's RSA private key is used.
 *
 * param[in] rsa_sign:rsa sign struct info.
 * param[out]sign:    rsa signature.
 * param[out] sign_len:length of rsa signature
 * retval HI_SUCCESS  Call this API successful
 * retval HI_FAILURE  Call this API fails.
 */
hi_s32 sys_rsa_sign_hash(const sys_rsa_sign_verify *rsa_sign, hi_u8 *sign, hi_u32 *sign_len);

/*
 * brief RSA verify a ciphertext with a RSA public key.
 *
 * param[in]  rsa_verify:rsa verify struct info.
 * param[in]  sign:   signature info
 * param[in]  sign_len:  length of rsa signature
 * retval HI_SUCCESS  Call this API successful
 * retval HI_FAILURE  Call this API fails.
 */
hi_s32 sys_rsa_verify_hash(const sys_rsa_sign_verify *rsa_verify, const hi_u8 *sign, hi_u32 sign_len);

/*
 * brief get rand number.
 * param[out]  randnum rand number.
 * param[in]   timeout time out.
 * retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
 */
hi_s32 sys_trng_get_random(hi_u32 *randnum, hi_u32 timeout);

#ifdef __cplusplus
}
#endif    /* __cplusplus */

#endif    /* End of __SYS_CIPHER_H__ */
