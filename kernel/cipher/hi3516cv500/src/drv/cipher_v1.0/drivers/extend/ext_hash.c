/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : drivers for ext hash.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#include "drv_osal_lib.h"
#include "cryp_hash.h"
#include "mbedtls/md.h"

#define HASH_MAX_BUFFER_SIZE    0x100000 /* 1M */

#if defined(SOFT_SHA1_SUPPORT) \
    || defined(SOFT_SHA256_SUPPORT) \
    || defined(SOFT_SHA512_SUPPORT)

hi_void *mbedtls_hash_create(hash_mode mode)
{
    mbedtls_md_type_t md_type;
    const mbedtls_md_info_t *info;
    mbedtls_md_context_t *ctx = HI_NULL;

    hi_log_func_enter();

    /* convert to mebdtls type */
    md_type = MBEDTLS_MD_SHA1 + (mode - HASH_MODE_SHA1);

    info = mbedtls_md_info_from_type(md_type);
    if (info == HI_NULL) {
        hi_log_error("error, invalid hash mode %d\n", mode);
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_NULL;
    }

    ctx = crypto_malloc(sizeof(mbedtls_md_context_t));
    if (ctx == HI_NULL) {
        hi_log_error("malloc hash context buffer failed!");
        hi_log_print_err_code(HI_ERR_CIPHER_FAILED_MEM);
        return HI_NULL;
    }
    crypto_memset(ctx, sizeof(mbedtls_md_context_t), 0, sizeof(mbedtls_md_context_t));

    mbedtls_md_init(ctx);
    mbedtls_md_setup(ctx, info, HI_FALSE);
    mbedtls_md_starts(ctx);

    hi_log_func_exit();

    return ctx;
}

static hi_s32 ext_hash_update_from_user(mbedtls_md_context_t *md, hi_u8 *chunk, hi_u32 chunk_len)
{
    hi_s32 ret;
    hi_u8 *ptr = HI_NULL;
    hi_u32 len;
    hi_u32 offset = 0;

    ptr = crypto_calloc(HASH_MAX_BUFFER_SIZE);
    if (ptr == HI_NULL) {
        hi_log_print_func_err(crypto_malloc, HI_ERR_CIPHER_FAILED_MEM);
        return HI_ERR_CIPHER_FAILED_MEM;
    }

    while (offset < chunk_len) {
        len = chunk_len - offset;
        if (len > HASH_MAX_BUFFER_SIZE) {
            len = HASH_MAX_BUFFER_SIZE;
        }
        ret = crypto_copy_from_user(ptr, chunk + offset, len);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(crypto_copy_from_user, ret);
            crypto_free(ptr);
            ptr = HI_NULL;
            return HI_ERR_CIPHER_FAILED_MEM;
        }
        ret = mbedtls_md_update(md, ptr, len);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(mbedtls_md_update, ret);
            crypto_free(ptr);
            ptr = HI_NULL;
            return ret;
        }
        crypto_msleep(1);
        offset += len;
    }

    if (ptr != HI_NULL) {
        crypto_free(ptr);
        ptr = HI_NULL;
    }

    return HI_SUCCESS;
}

hi_s32 mbedtls_hash_update(hi_void *ctx, hi_u8 *chunk, hi_u32 chunk_len, hash_chunk_src src)
{
    hi_u8 *ptr = HI_NULL;
    hi_s32 ret;
    mbedtls_md_context_t *md = ctx;

    hi_log_func_enter();

    hi_log_chk_param_return(ctx == HI_NULL);

    if (chunk_len == 0x00) {
        return HI_SUCCESS;
    }

    if (src == HASH_CHUNCK_SRC_USER) {
        ret = ext_hash_update_from_user(md, chunk, chunk_len);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(mbedtls_md_update, ret);
            return ret;
        }
    } else {
        ret = mbedtls_md_update(md, chunk, chunk_len);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(mbedtls_md_update, ret);
            return ret;
        }
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 mbedtls_hash_finish(hi_void *ctx,  hi_void *hash, hi_u32 hash_buf_len, hi_u32 *hashlen)
{
    mbedtls_md_context_t *md = ctx;

    hi_log_func_enter();

    hi_log_chk_param_return(ctx == HI_NULL);
    hi_log_chk_param_return(hash_buf_len == 0);

    mbedtls_md_finish(md, hash);

    *hashlen = mbedtls_md_get_size(md->md_info);

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 mbedtls_hash_destory(hi_void *ctx)
{
    mbedtls_md_context_t *md = ctx;

    hi_log_func_enter();

    hi_log_chk_param_return(ctx == HI_NULL);

    mbedtls_md_free(md);
    crypto_free(ctx);
    ctx = HI_NULL;

    hi_log_func_exit();
    return HI_SUCCESS;
}

#endif /* End of SOFT_AES_CCM_GCM_SUPPORT */
