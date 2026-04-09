/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : head file for ext alg of cipher soft ware.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#ifndef __EXT_ALG_H_
#define __EXT_ALG_H_

#include "drv_hash.h"
#include "drv_symc.h"
#include "drv_cipher_kapi.h"
#include "drv_osal_lib.h"

/*
 * brief          Create symc handle
 *
 * param handle   symc handle to be initialized
 * param hard_key symc use hard key ot not
 */
hi_void *ext_mbedtls_aead_create(hi_u32 hard_key);

/*
 * brief          Clear symc context
 *
 * param handle      symc handle to be destory
 */
hi_s32 ext_mbedtls_aead_destory(hi_void *ctx);

/*
 * brief          symc iv schedule
 *
 * param handle   symc handle
 * param IV       encryption key
 * param ivlen    length of iv
 *
 * return         0 if successful.
 */
hi_s32 ext_mbedtls_aead_setiv(hi_void *ctx, const hi_u8 *iv, hi_u32 ivlen, hi_u32 usage);

/*
 * brief          symc key schedule
 *
 * param ctx      SYMC handle
 * param key      SYMC key
 * param keylen   SYMC key length
 *
 * return         0 if successful, or HI_SYMC_ERR_KEY_LEN_INVALID
 */
hi_s32 ext_mbedtls_aead_setkey(hi_void *ctx, const hi_u8 *fkey, const hi_u8 *skey, hi_u32 *hisi_klen);

/*
 * brief          SYMC alg and mode
 *
 * param ctx      SYMC handle
 * param aad      Associated Data
 * param alen     Associated Data Length
 * param tlen     Tag length
 *
 * return         0 if successful.
 */
hi_s32 ext_mbedtls_aead_set_aad(hi_void *ctx, compat_addr aad, hi_u32 alen, hi_u32 tlen);

/*
 * brief  get ccm/gcm tag.
 * param[in]   chn_num The channel number.
 * param[out]  tag The tag value.
 * param[out]  taglen tag length
 * \retval     On received interception, HI_TRUE is returned  otherwise HI_FALSE is returned.
 */
hi_s32 ext_mbedtls_aead_get_tag(hi_void *ctx, hi_u32 tag[AEAD_TAG_SIZE_IN_WORD], hi_u32 *taglen);

/*
 * brief          aead ccm buffer encryption/decryption.
 *
 * Note: Due to the nature of aes you should use the same key schedule for
 * both encryption and decryption.
 *
 * param ctx      symc ctx.
 * param operation  operation of encrypt or decrypt.
 * param pack     package of decrypt or encrypt.
 * param last     last or not.
 *
 * return         0 if successful
 */
hi_s32 ext_mbedtls_aead_ccm_crypto(hi_void *ctx, hi_u32 operation, symc_multi_pack *pack, hi_u32 last);

/*
 * brief          aead gcm buffer encryption/decryption.
 *
 * Note: Due to the nature of aes you should use the same key schedule for
 * both encryption and decryption.
 *
 * param ctx      symc ctx.
 * param operation  operation of encrypt or decrypt.
 * param pack     package of decrypt or encrypt.
 * param last     last or not.
 *
 * return         0 if successful
 */
hi_s32 ext_mbedtls_aead_gcm_crypto(hi_void *ctx, hi_u32 operation, symc_multi_pack *pack, hi_u32 last);

/*
 * brief          Create symc handle
 *
 * param handle   symc handle to be initialized
 * param hard_key symc use hard key ot not
 */
hi_void *ext_mbedtls_symc_create(hi_u32 hard_key);

/*
 * brief          Clear symc context
 *
 * param handle      symc handle to be destory
 */
hi_s32 ext_mbedtls_symc_destory(hi_void *ctx);

/*
 * brief          SYMC alg and mode
 *
 * param handle   SYMC handle
 * param alg      Symmetric cipher alg
 * param mode     Symmetric cipher mode
 *
 * return         0 if successful.
 */
hi_void ext_mbedtls_symc_setmode(hi_void *ctx, symc_alg alg, symc_mode mode, symc_width width);

/*
 * brief          symc iv schedule
 *
 * param handle   symc handle
 * param IV       encryption key
 * param ivlen    length of iv
 *
 * return         0 if successful.
 */
hi_s32 ext_mbedtls_symc_setiv(hi_void *ctx, const hi_u8 *iv, hi_u32 ivlen, hi_u32 usage);

/*
 * brief          Symc iv schedule
 *
 * param handle   symc handle
 * param IV       Symc IV
 * param ivlen    must be 128, 192 or 256
 *
 * return         0 if successful.
 */
hi_void ext_mbedtls_symc_getiv(hi_void *ctx, hi_u8 *iv, hi_u32 *ivlen);

/*
 * brief          symc key schedule
 *
 * param ctx      SYMC handle
 * param key      SYMC key
 * param keylen   SYMC key length
 *
 * return         0 if successful, or HI_SYMC_ERR_KEY_LEN_INVALID
 */
hi_s32 ext_mbedtls_symc_setkey(hi_void *ctx, const hi_u8 *fkey, const hi_u8 *skey, hi_u32 *hisi_klen);

/*
 * brief          symc buffer encryption/decryption.
 *
 * Note: Due to the nature of aes you should use the same key schedule for
 * both encryption and decryption.
 *
 * param ctx       symc ctx
 * param operation decrypt or encrypt
 * param pack     package for encrypt or decrypt.
 * param last     last or not
 * return         0 if successful
 */
hi_s32 ext_mbedtls_symc_crypto(hi_void *ctx, hi_u32 operation, symc_multi_pack *pack, hi_u32 last);

/*
 * brief          Create sm4 handle
 *
 * param handle   sm4 handle to be initialized
 * param hard_key sm4 use hard key ot not
 */
hi_void *ext_sm4_create(hi_u32 hard_key);

/*
 * brief          Clear sm4 context
 *
 * param handle      sm4 handle to be destory
 */
hi_s32 ext_sm4_destory(hi_void *ctx);

/*
 * brief          sm4 alg and mode
 *
 * param handle   sm4 handle
 * param alg      Symmetric cipher alg
 * param mode     Symmetric cipher mode
 *
 * return         NA.
 */
hi_void ext_sm4_setmode(hi_void *ctx, symc_alg alg, symc_mode mode, symc_width width);

/*
 * brief          sm4 iv schedule
 *
 * param handle   sm4 handle
 * param IV       encryption key
 * param ivlen    length of iv
 *
 * return         0 if successful.
 */
hi_s32 ext_sm4_setiv(hi_void *ctx, const hi_u8 *iv, hi_u32 ivlen, hi_u32 usage);

/*
 * brief          Symc iv schedule
 *
 * param handle   symc handle
 * param IV       Symc IV
 * param ivlen    must be 128, 192 or 256
 *
 * return         NA.
 */
hi_void ext_sm4_getiv(hi_void *ctx, hi_u8 *iv, hi_u32 *ivlen);

/*
 * brief          sm4 key schedule
 *
 * param ctx      sm4 handle
 * param key      sm4 key
 * param keylen   sm4 key length
 *
 * return         0 if successful, or HI_SYMC_ERR_KEY_LEN_INVALID
 */
hi_s32 ext_sm4_setkey(hi_void *ctx, const hi_u8 *fkey, const hi_u8 *skey, hi_u32 *hisi_klen);

/*
 * brief          sm4 buffer encryption/decryption.
 *
 * Note: Due to the nature of aes you should use the same key schedule for
 * both encryption and decryption.
 *
 * param ctx       sm4 ctx
 * param operation decrypt or encrypt
 * param pack     package for encrypt or decrypt.
 * param last     last or not
 *
 * return         0 if successful
 */
hi_s32 ext_sm4_crypto(hi_void *ctx, hi_u32 operation, symc_multi_pack *pack, hi_u32 last);

/*
 * brief          Clear hash context
 *
 * param ctx      symc handle to be destory
 */
hi_void *mbedtls_hash_create(hash_mode mode);

/*
 * brief          Hash message chunk calculation
 *
 * Note: the message must be write to the buffer
 * which get from cryp_hash_get_cpu_addr, and the length of message chunk
 * can't large than the length which get from cryp_hash_get_cpu_addr.
 *
 * param ctx      hash handle to be destory
 * param chunk    hash message to update
 * param length   length of hash message
 * param src      source of hash message
 */
hi_s32 mbedtls_hash_update(hi_void *ctx, hi_u8 *chunk, hi_u32 chunk_len, hash_chunk_src src);

/*
 * brief          HASH final digest
 *
 * param ctx      Hash handle
 * param hash     HASH checksum result
 * param hashlen  Length of HASH checksum result
 */
hi_s32 mbedtls_hash_finish(hi_void *ctx,  hi_void *hash, hi_u32 *hashlen);

/*
 * brief          Clear hash context
 *
 * param ctx      symc handle to be destory
 */
hi_s32 mbedtls_hash_destory(hi_void *ctx);

/* sm3 */
hi_void *ext_sm3_create(hash_mode mode);

hi_s32 ext_sm3_update(hi_void *ctx, hi_u8 *chunk, hi_u32 chunkLen, hash_chunk_src src);

hi_s32 ext_sm3_finish(hi_void *ctx,  hi_void *hash, hi_u32 *hashlen);

hi_s32 ext_sm3_destory(hi_void *ctx);
#endif
