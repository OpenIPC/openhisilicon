/*****************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : ext_hash.c
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include "drv_osal_lib.h"
#include "cryp_hash.h"
#include "mbedtls/md.h"

#define HASH_MAX_BUFFER_SIZE    0x100000 /* 1M */

#if defined(SOFT_SHA1_SUPPORT) \
    || defined(SOFT_SHA256_SUPPORT) \
    || defined(SOFT_SHA512_SUPPORT)

void *mbedtls_hash_create(hash_mode mode)
{
    mbedtls_md_type_t md_type;
    const mbedtls_md_info_t *info;
    mbedtls_md_context_t *ctx = HI_NULL;

    HI_LOG_FUNC_ENTER();

    /* convert to mebdtls type */
    md_type = MBEDTLS_MD_SHA1 + (mode - HASH_MODE_SHA1);

    info = mbedtls_md_info_from_type(md_type);
    if (info == HI_NULL) {
        HI_LOG_ERROR("error, invalid hash mode %d\n", mode);
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
        return HI_NULL;
    }

    ctx = crypto_malloc(sizeof(mbedtls_md_context_t));
    if (ctx == HI_NULL) {
        HI_LOG_ERROR("malloc hash context buffer failed!");
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_FAILED_MEM);
        return HI_NULL;
    }
    crypto_memset(ctx, sizeof(mbedtls_md_context_t), 0, sizeof(mbedtls_md_context_t));

    mbedtls_md_init(ctx);
    mbedtls_md_setup(ctx, info, HI_FALSE);
    mbedtls_md_starts(ctx);

    HI_LOG_FUNC_EXIT();

    return ctx;
}

hi_s32 mbedtls_hash_update(void *ctx, hi_u8 *chunk, hi_u32 chunkLen, hash_chunk_src src)
{
    hi_u8 *ptr = HI_NULL;
    hi_s32 ret = HI_FAILURE;
    mbedtls_md_context_t *md = ctx;
    hi_u32 offset = 0, length = 0;

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(ctx == HI_NULL);

    if (chunkLen == 0x00) {
        return HI_SUCCESS;
    }

    if (src == HASH_CHUNCK_SRC_USER) {
        ptr = crypto_malloc(HASH_MAX_BUFFER_SIZE);
        if (ptr == HI_NULL) {
            HI_LOG_ERROR("malloc hash chunk buffer failed, chunkLen 0x%x\n!", chunkLen);
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_FAILED_MEM);
            return HI_ERR_CIPHER_FAILED_MEM;
        }
        crypto_memset(ptr, HASH_MAX_BUFFER_SIZE, 0, HASH_MAX_BUFFER_SIZE);

        while (offset < chunkLen) {
            length = chunkLen - offset;
            if (length > HASH_MAX_BUFFER_SIZE) {
                length = HASH_MAX_BUFFER_SIZE;
            }
            if (crypto_copy_from_user(ptr, chunk + offset, length)) {
                HI_LOG_ERROR("copy hash chunk from user failed!");
                HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_FAILED_MEM);
                ret = HI_ERR_CIPHER_FAILED_MEM;
                goto exit;
            }
            ret = mbedtls_md_update(md, ptr, length);
            if (ret != HI_SUCCESS) {
                HI_LOG_PRINT_FUNC_ERR(mbedtls_md_update, ret);
                break;
            }
            crypto_msleep(1);
            offset   += length;
        }
    } else {
        ret = mbedtls_md_update(md, chunk, chunkLen);
        if (ret != HI_SUCCESS) {
            HI_LOG_PRINT_FUNC_ERR(mbedtls_md_update, ret);
        }
    }

exit:
    if (ptr != HI_NULL) {
        crypto_free(ptr);
        ptr = HI_NULL;
    }

    HI_LOG_FUNC_EXIT();

    return ret;
}

hi_s32 mbedtls_hash_finish(void *ctx,  void *hash, hi_u32 *hashlen)
{
    mbedtls_md_context_t *md = ctx;

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(ctx == HI_NULL);

    mbedtls_md_finish(md, hash);

    *hashlen = mbedtls_md_get_size(md->md_info);

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 mbedtls_hash_destory(void *ctx)
{
    mbedtls_md_context_t *md = ctx;

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(ctx == HI_NULL);

    mbedtls_md_free(md);
    crypto_free(ctx);
    ctx = HI_NULL;

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

#endif // End of SOFT_AES_CCM_GCM_SUPPORT
