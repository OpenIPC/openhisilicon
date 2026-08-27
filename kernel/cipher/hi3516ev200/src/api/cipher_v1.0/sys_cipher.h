/*****************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : sys_cipher.h
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#ifndef __SYS_CIPHER_H__
#define __SYS_CIPHER_H__

#include "hi_mpi_cipher.h"
#include "user_osal_lib.h"

#ifdef __cplusplus
extern "C" {
#endif    /* __cplusplus */

/******************************* API Declaration *****************************/
/** \addtogroup      mpi */
/** @{ */  /** <!--[mpi]*/

/** @}*/  /** <!-- ==== Structure Definition end ====*/

/******************************* API Code *****************************/
/** \addtogroup      crypto*/
/** @{*/  /** <!-- [link]*/

/**
\brief   mpi Init.
\retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
*/
hi_s32 sys_symc_init(hi_void);

/**
\brief   Kapi Deinit.
\retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
*/
hi_s32 sys_symc_deinit(hi_void);

/**
\brief   Create symc handle.
\param[in]  id The channel number.
\param[in]  uuid The user identification.
\retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
*/
hi_s32 sys_symc_create(hi_u32 *id);

/**
\brief   Destroy symc handle.
\param[in]  id The channel number.
\param[in]  uuid The user identification.
\retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
*/
hi_s32 sys_symc_destroy(hi_u32 id);

/**
\brief  set work params.
* \param[in]  id            The channel number.
* \param[in]  hard_key      whether use the hard key or not.
* \param[in]  alg           The symmetric cipher algorithm.
* \param[in]  work_mode     The symmetric cipher mode.
* \param[in]  bit_width     The symmetric cipher bit width.
* \param[in]  key_len       The symmetric cipher key len.
* \param[in]  sm1_round_num The round number of sm1.
* \param[in]  fkey          first key buffer, defualt
* \param[in]  skey          second key buffer, expand
* \param[in]  keylen        The length of fkey buffer,if skey is not null,equal to the length of skey.
* \param[in]  iv            iv buffer.
* \param[in]  ivlen         The length of iv buffer.
* \param[in]  iv_usage      iv change.
* \param[in]  aad           Associated Data
* \param[in]  alen          Associated Data Length
* \param[in]  tlen          Tag length
\retval       On success,   HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
*/
hi_s32 sys_symc_config(hi_u32 id,
                    hi_u32 hard_key,
                    hi_cipher_alg alg,
                    hi_cipher_work_mode work_mode,
                    hi_cipher_bit_width bit_width,
                    hi_cipher_key_length key_len,
                    hi_cipher_sm1_round sm1_round_num,
                    hi_u8 *fkey, hi_u8 *skey, hi_u32 keylen,
                    hi_u8 *iv, hi_u32 ivlen, hi_u32 iv_usage,
                    compat_addr aad, hi_u32 alen, hi_u32 tlen);


/**
\brief  get work params.
* \param[in]  id The channel number.
* \param[out] ctrl infomation.
\retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
*/
hi_s32 sys_symc_get_config(hi_u32 id, hi_cipher_ctrl *ctrl);

/**
 * \brief          SYMC  buffer encryption/decryption.
 *
 * Note: Due to the nature of aes you should use the same key schedule for
 * both encryption and decryption.
 *
 * \param[in] id   The channel number.
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 * \param length   length of the input data
 * \param decrypt  decrypt or encrypt
 *
 * \return         0 if successful
 */
hi_s32 sys_symc_crypto(hi_u32 id, const compat_addr input,
                       const compat_addr output, hi_u32 length,
                       hi_u32 operation);

/**
 * \brief          SYMC multiple buffer encryption/decryption.
 *
 * Note: Due to the nature of aes you should use the same key schedule for
 * both encryption and decryption.
 *
 * \param[in]  id The channel number.
 * \param pkg       Buffer of package infomation
 * \param pkg_num   Number of package infomation
 * \param decrypt   decrypt or encrypt
 *
 * \return         0 if successful
 */
hi_s32 sys_symc_crypto_multi(hi_u32 id, const hi_void *pkg,
                             hi_u32 pkg_num, hi_u32 operation);

/**
 * \brief          SYMC multiple buffer encryption/decryption.
 * \param[in]  id The channel number.
 * \param[in]  tag tag data of CCM/GCM
 * \param uuid uuid The user identification.
 *
 * \return         0 if successful
 */
hi_s32 sys_aead_get_tag(hi_u32 id, hi_u8 *tag, hi_u32 *taglen);

/**
 * \brief       Klad encrypt clear key.
 * \keysel[in]  keysel root key seclect.
 * \param[in]   target target seclect
 * \param[in]   clear clear key
 * \param[out]  encrypt encrypt key
 * \param uuid  uuid The user identification.
 *
 * \return         0 if successful
 */
hi_s32 sys_klad_encrypt_key(hi_u32 keysel, hi_u32 target,
                            const hi_u8 clear[AES_BLOCK_SIZE],
                            hi_u8 encrypt[AES_BLOCK_SIZE]);

/**
\brief   Kapi Init.
\retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
*/
hi_s32 sys_hash_init(hi_void);

/**
\brief   Kapi Deinit.
\retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
*/
hi_s32 sys_hash_deinit(hi_void);

/**
 * \brief          HASH context setup.
 *
 *
 * \param[out] id The channel number.
 * \param[in] type    Hash type
 * \param[in] key     hmac key
 * \param[in] keylen  hmac key length
 *
 * \return         0 if successful
 */
hi_s32 sys_hash_start(hi_u32 *id, hi_cipher_hash_type type,
                      const hi_u8 *key, hi_u32 keylen);

/**
 * \brief          HASH process buffer.
 *
 * \param[in] id       The channel number.
 * \param[in] input    buffer holding the input data
 * \param[in] length   length of the input data
 * \param[in] src      source of hash message
 *
 * \return         0 if successful
 */
hi_s32 sys_hash_update(hi_u32 id, const hi_u8 *input, hi_u32 length,
                       hash_chunk_src src);

/**
 * \brief          HASH final digest.
 *
 * \param[in]  id      The channel number.
 * \param[out] hash    buffer holding the hash data
 * \param[out] hashlen length of the hash data
 * \param[in]  uuid    uuid The user identification.
 *
 * \return         0 if successful
 */
hi_s32 sys_hash_finish(hi_u32 id, hi_u8 *hash, hi_u32 *hashlen);

/**
\brief   Kapi Init.
\retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
*/
hi_s32 sys_rsa_init(hi_void);

/**
\brief   Kapi Deinitialize.
\retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
*/
hi_s32 sys_rsa_deinit(hi_void);

/**
* \brief RSA encryption a plaintext with a RSA private key.
*
* \param[in] key:     rsa key struct.
* \param[in] scheme:  rsa encrypt/decrypt scheme.
* \param[in] in:      input data to be encryption
* \param[out] inlen:  length of input data to be encryption
* \param[out] out:    output data of encryption
* \param[out] outlen: length of output data to be encryption
* \retval HI_SUCCESS  Call this API successful
* \retval HI_FAILURE  Call this API fails.
*/
hi_s32 sys_rsa_encrypt(const cryp_rsa_key *key,
                       hi_cipher_rsa_enc_scheme scheme,
                       const hi_u8 *in, hi_u32 inlen,
                       hi_u8 *out, hi_u32 *outlen);

/**
* \brief RSA decryption a ciphertext with a RSA public key.
*
* \param[in] key:     rsa key struct.
* \param[in] scheme:  rsa encrypt/decrypt scheme.
* \param[in] in:      input data to be encryption
* \param[in] inlen:   length of input data to be encryption
* \param[out] out:    output data to be encryption
* \param[out] outlen: length of output data to be encryption
* \retval HI_SUCCESS  Call this API successful
* \retval HI_FAILURE  Call this API fails.
*/
hi_s32 sys_rsa_decrypt(const cryp_rsa_key *key,
                       hi_cipher_rsa_enc_scheme scheme,
                       const hi_u8 *in, hi_u32 inlen,
                       hi_u8 *out, hi_u32 *outlen);

/**
* \brief RSA signature a context with appendix, where a signer's RSA private key is used.
*
* \param[in] key:     rsa key struct.
* \param[in] scheme:  rsa signature/verify scheme.
* \param[in] in:      input data to be encryption
* \param[in] inlen:   length of input data to be encryption
* \param[in] hash:    hash value of context,if NULL, let hash = Hash(context) automatically
* \param[out] out:    output data to be encryption
* \param[out] outlen: length of output data to be encryption
* \param[in]  src     source of hash message
* \param[in]  uuid    uuid The user identification.
* \retval HI_SUCCESS  Call this API successful
* \retval HI_FAILURE  Call this API fails.
*/
hi_s32 sys_rsa_sign_hash(const cryp_rsa_key *key,
                         hi_cipher_rsa_sign_scheme scheme,
                         const hi_u8 *hash, hi_u32 hlen,
                         hi_u8 *sign, hi_u32 *signlen);

/**
* \brief RSA verify a ciphertext with a RSA public key.
*
* \param[in]  key_info:encryption struct.
* \param[in]  in:     input data to be encryption
* \param[in]  inlen:  length of input data to be encryption
* \param[in]  hash:   hash value of context,if NULL, let hash = Hash(context) automatically
* \param[out] out:    output data to be encryption
* \param[out] outlen: length of output data to be encryption
* \param[in]  src     source of hash message
* \param[in]  uuid    uuid The user identification.
* \retval HI_SUCCESS  Call this API successful
* \retval HI_FAILURE  Call this API fails.
*/
hi_s32 sys_rsa_verify_hash(const cryp_rsa_key *key,
                           hi_cipher_rsa_sign_scheme scheme,
                           const hi_u8 *hash, hi_u32 hlen,
                           const hi_u8 *sign, hi_u32 signlen);

/**
\brief get rand number.
\param[out]  randnum rand number.
\param[in]   timeout time out.
\retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
*/
hi_s32 sys_trng_get_random( hi_u32 *randnum, hi_u32 timeout);

/** @}*/  /** <!-- ==== API Code end ====*/

#ifdef __cplusplus
}
#endif    /* __cplusplus */

#endif    /* End of #ifndef __MPI_CIPHER_H__*/
