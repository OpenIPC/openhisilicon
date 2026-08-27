/*****************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : ext_alg.h
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#ifndef __EXT_ALG_H_
#define __EXT_ALG_H_

#include "drv_osal_lib.h"
#include "drv_hash.h"
#include "drv_symc.h"

/******************************* API Declaration *****************************/
/** \addtogroup      hash */
/** @{ */  /** <!--[hash]*/

/** @}*/  /** <!-- ==== Structure Definition end ====*/

/******************************* API Code *****************************/
/** \addtogroup      cipher drivers*/
/** @{*/  /** <!-- [cipher]*/

/**
 * \brief          Create symc handle
 *
 * \param handle   symc handle to be initialized
 * \param hard_key symc use hard key ot not
 */
void *ext_mbedtls_aead_create(hi_u32 hard_key);

/**
 * \brief          Clear symc context
 *
 * \param handle      symc handle to be destory
 */
hi_s32 ext_mbedtls_aead_destory(void *ctx);

/**
 * \brief          symc iv schedule
 *
 * \param handle   symc handle
 * \param IV       encryption key
 * \param ivlen    length of iv
 *
 * \return         0 if successful.
 */
hi_s32 ext_mbedtls_aead_setiv(void *ctx, const hi_u8 *iv, hi_u32 ivlen, hi_u32 usage);

/**
 * \brief          symc key schedule
 *
 * \param ctx      SYMC handle
 * \param key      SYMC key
 * \param keylen   SYMC key length
 *
 * \return         0 if successful, or HI_SYMC_ERR_KEY_LEN_INVALID
 */
hi_s32 ext_mbedtls_aead_setkey(void *ctx, const hi_u8 *fkey, const hi_u8 *skey, hi_u32 *hisi_klen);

/**
 * \brief          SYMC alg and mode
 *
 * \param ctx      SYMC handle
 * \param aad      Associated Data
 * \param alen     Associated Data Length
 * \param tlen     Tag length
 *
 * \return         0 if successful.
 */
hi_s32 ext_mbedtls_aead_set_aad( void *ctx, compat_addr aad, hi_u32 alen, hi_u32 tlen);

/**
\brief  get ccm/gcm tag.
\param[in]   chn_num The channel number.
\param[out]  tag The tag value.
\param[out]  taglen tag length
\retval     On received interception, HI_TRUE is returned  otherwise HI_FALSE is returned.
*/
hi_s32 ext_mbedtls_aead_get_tag(void *ctx, hi_u32 tag[AEAD_TAG_SIZE_IN_WORD], hi_u32 *taglen);

/**
 * \brief          aead ccm buffer encryption/decryption.
 *
 * Note: Due to the nature of aes you should use the same key schedule for
 * both encryption and decryption.
 *
 * \param handle   symc handle
 * \param decrypt  decrypt or encrypt
 * \param mode     mode
 * \param length   length of the input data
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 * \param usage_list usage of buffer
 * \param pkg_num  numbers of buffer
 *
 * \return         0 if successful
 */
hi_s32 ext_mbedtls_aead_ccm_crypto(void *ctx,
                                   hi_u32 operation,
                                   compat_addr input[],
                                   compat_addr output[],
                                   hi_u32 length[],
                                   hi_u32 usage_list[],
                                   hi_u32 pkg_num, hi_u32 last);

/**
 * \brief          aead gcm buffer encryption/decryption.
 *
 * Note: Due to the nature of aes you should use the same key schedule for
 * both encryption and decryption.
 *
 * \param handle   symc handle
 * \param decrypt  decrypt or encrypt
 * \param mode     mode
 * \param length   length of the input data
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 * \param usage_list usage of buffer
 * \param pkg_num  numbers of buffer
 *
 * \return         0 if successful
 */
hi_s32 ext_mbedtls_aead_gcm_crypto(void *ctx,
                                   hi_u32 operation,
                                   compat_addr input[],
                                   compat_addr output[],
                                   hi_u32 length[],
                                   hi_u32 usage_list[],
                                   hi_u32 pkg_num, hi_u32 last);

/**
 * \brief          Create symc handle
 *
 * \param handle   symc handle to be initialized
 * \param hard_key symc use hard key ot not
 */
void *ext_mbedtls_symc_create(hi_u32 hard_key);

/**
 * \brief          Clear symc context
 *
 * \param handle      symc handle to be destory
 */
hi_s32 ext_mbedtls_symc_destory(void *ctx);

/**
 * \brief          SYMC alg and mode
 *
 * \param handle   SYMC handle
 * \param alg      Symmetric cipher alg
 * \param mode     Symmetric cipher mode
 *
 * \return         0 if successful.
 */
void ext_mbedtls_symc_setmode(void *ctx, symc_alg alg, symc_mode mode, symc_width width);

/**
 * \brief          symc iv schedule
 *
 * \param handle   symc handle
 * \param IV       encryption key
 * \param ivlen    length of iv
 *
 * \return         0 if successful.
 */
hi_s32 ext_mbedtls_symc_setiv(void *ctx, const hi_u8 *iv, hi_u32 ivlen, hi_u32 usage);

/**
 * \brief          Symc iv schedule
 *
 * \param handle   symc handle
 * \param IV       Symc IV
 * \param ivlen    must be 128, 192 or 256
 *
 * \return         0 if successful.
 */
void ext_mbedtls_symc_getiv(void *ctx, hi_u8 *iv, hi_u32 *ivlen);

/**
 * \brief          symc key schedule
 *
 * \param ctx      SYMC handle
 * \param key      SYMC key
 * \param keylen   SYMC key length
 *
 * \return         0 if successful, or HI_SYMC_ERR_KEY_LEN_INVALID
 */
hi_s32 ext_mbedtls_symc_setkey(void *ctx, const hi_u8 *fkey, const hi_u8 *skey, hi_u32 *hisi_klen);

/**
 * \brief          symc buffer encryption/decryption.
 *
 * Note: Due to the nature of aes you should use the same key schedule for
 * both encryption and decryption.
 *
 * \param handle   symc handle
 * \param decrypt  decrypt or encrypt
 * \param mode     mode
 * \param length   length of the input data
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 * \param usage_list usage of buffer
 * \param pkg_num  numbers of buffer
 *
 * \return         0 if successful
 */
hi_s32 ext_mbedtls_symc_crypto(void *ctx,
                               hi_u32 operation,
                               compat_addr input[],
                               compat_addr output[],
                               hi_u32 length[],
                               symc_node_usage usage_list[],
                               hi_u32 pkg_num, hi_u32 last);

/**
 * \brief          Create sm4 handle
 *
 * \param handle   sm4 handle to be initialized
 * \param hard_key sm4 use hard key ot not
 */
void *ext_sm4_create(hi_u32 hard_key);

/**
 * \brief          Clear sm4 context
 *
 * \param handle      sm4 handle to be destory
 */
hi_s32 ext_sm4_destory(void *ctx);

/**
 * \brief          sm4 alg and mode
 *
 * \param handle   sm4 handle
 * \param alg      Symmetric cipher alg
 * \param mode     Symmetric cipher mode
 *
 * \return         NA.
 */
void ext_sm4_setmode(void *ctx, symc_alg alg, symc_mode mode, symc_width width);

/**
 * \brief          sm4 iv schedule
 *
 * \param handle   sm4 handle
 * \param IV       encryption key
 * \param ivlen    length of iv
 *
 * \return         0 if successful.
 */
hi_s32 ext_sm4_setiv(void *ctx, const hi_u8 *iv, hi_u32 ivlen, hi_u32 usage);

/**
 * \brief          Symc iv schedule
 *
 * \param handle   symc handle
 * \param IV       Symc IV
 * \param ivlen    must be 128, 192 or 256
 *
 * \return         NA.
 */
void ext_sm4_getiv(void *ctx, hi_u8 *iv, hi_u32 *ivlen);

/**
 * \brief          sm4 key schedule
 *
 * \param ctx      sm4 handle
 * \param key      sm4 key
 * \param keylen   sm4 key length
 *
 * \return         0 if successful, or HI_SYMC_ERR_KEY_LEN_INVALID
 */
hi_s32 ext_sm4_setkey(void *ctx, const hi_u8 *fkey, const hi_u8 *skey, hi_u32 *hisi_klen);

/**
 * \brief          sm4 buffer encryption/decryption.
 *
 * Note: Due to the nature of aes you should use the same key schedule for
 * both encryption and decryption.
 *
 * \param handle   sm4 handle
 * \param decrypt  decrypt or encrypt
 * \param mode     mode
 * \param length   length of the input data
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 * \param usage_list usage of buffer
 * \param pkg_num  numbers of buffer
 *
 * \return         0 if successful
 */
hi_s32 ext_sm4_crypto(void *ctx,
                      hi_u32 operation,
                      compat_addr input[],
                      compat_addr output[],
                      hi_u32 length[],
                      symc_node_usage usage_list[],
                      hi_u32 pkg_num, hi_u32 last);

/**
 * \brief          Clear hash context
 *
 * \param ctx      symc handle to be destory
 */
void *mbedtls_hash_create(hash_mode mode);

/**
 * \brief          Hash message chunk calculation
 *
 * Note: the message must be write to the buffer
 * which get from cryp_hash_get_cpu_addr, and the length of message chunk
 * can't large than the length which get from cryp_hash_get_cpu_addr.
 *
 * \param ctx      hash handle to be destory
 * \param chunk    hash message to update
 * \param length   length of hash message
 * \param src      source of hash message
 */
hi_s32 mbedtls_hash_update(void *ctx, hi_u8 *chunk, hi_u32 chunkLen, hash_chunk_src src);

/**
 * \brief          HASH final digest
 *
 * \param ctx      Hash handle
 * \param hash     HASH checksum result
 * \param hashlen  Length of HASH checksum result
 */
hi_s32 mbedtls_hash_finish(void *ctx,  void *hash, hi_u32 *hashlen);

/**
 * \brief          Clear hash context
 *
 * \param ctx      symc handle to be destory
 */
hi_s32 mbedtls_hash_destory(void *ctx);

/* sm3 */
void *ext_sm3_create(hash_mode mode);

hi_s32 ext_sm3_update(void *ctx, hi_u8 *chunk, hi_u32 chunkLen, hash_chunk_src src);

hi_s32 ext_sm3_finish(void *ctx,  void *hash, hi_u32 *hashlen);

hi_s32 ext_sm3_destory(void *ctx);

/** @} */  /** <!-- ==== API declaration end ==== */
#endif
