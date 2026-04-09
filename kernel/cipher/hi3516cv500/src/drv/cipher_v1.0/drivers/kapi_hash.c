/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description   : drv for kapi hash.
 * Author        : Hisilicon multimedia software group
 * Create        : 2018-10-23
 */

#include "cryp_hash.h"
#include "drv_osal_lib.h"

#ifdef MHASH_NONSUPPORT
#define HASH_SOFT_CHANNEL_MAX             0x01
#define HASH_SOFT_CHANNEL_MASK            0x01
#else
#define HASH_SOFT_CHANNEL_MAX             0x08
#define HASH_SOFT_CHANNEL_MASK            0xFF
#endif

/* hmac ipad byte */
#define HMAC_IPAD_BYTE                    0x36

/* hmac opad byte */
#define HMAC_OPAD_BYTE                    0x5C

#define HMAC_HASH                         0x01
#define HMAC_AESCBC                       0x02

typedef struct {
    hash_func *func;                      /* HASH function */
    hi_void *cryp_ctx;                    /* Context of cryp instance */
    hi_u32 hmac;                          /* HMAC or not */
    hi_u32 mac_id;                        /* CMAC handle */
    hi_u8 hmac_ipad[HASH_BLOCK_SIZE_128]; /* hmac ipad */
    hi_u8 hmac_opad[HASH_BLOCK_SIZE_128]; /* hmac opad */
    crypto_owner owner;                   /* user ID */
} kapi_hash_ctx;

/* Context of cipher */
static channel_context g_hash_ctx[HASH_SOFT_CHANNEL_MAX];

/* hash mutex */
static crypto_mutex g_hash_mutex;

#define kapi_hash_lock_err_return()   \
    do { \
        ret = crypto_mutex_lock(&g_hash_mutex);  \
        if (ret != HI_SUCCESS) { \
            hi_log_error("error, hash lock failed\n"); \
            hi_log_print_err_code(HI_ERR_CIPHER_BUSY); \
            return HI_ERR_CIPHER_BUSY; \
        } \
    } while (0)

#define kapi_hash_unlock()   crypto_mutex_unlock(&g_hash_mutex)

static hi_s32 kapi_hash_chk_handle(hi_handle handle)
{
    if((hi_handle_get_modid(handle) != HI_ID_CIPHER) || (hi_handle_get_private_data(handle) != 0)) { \
        hi_log_error("invalid handle 0x%x!\n", handle);
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_HANDLE);
        return HI_ERR_CIPHER_INVALID_HANDLE;
    }

    if (hi_handle_get_chnid(handle) >= HASH_SOFT_CHANNEL_MAX) {
        hi_log_error("chan %d is too large, max: %d\n", hi_handle_get_chnid(handle), HASH_SOFT_CHANNEL_MAX);
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_HANDLE);
        return HI_ERR_CIPHER_INVALID_HANDLE;
    }

    if (g_hash_ctx[hi_handle_get_chnid(handle)].open == HI_FALSE) {
        hi_log_error("chan %d is not open\n", hi_handle_get_chnid(handle));
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_HANDLE);
        return HI_ERR_CIPHER_INVALID_HANDLE;
    }

    return HI_SUCCESS;
}

/* API Code for kapi hash. */
hi_s32 kapi_hash_init(hi_void)
{
    hi_s32 ret;

    hi_log_func_enter();

    hi_log_info("HASH init\n");

    crypto_mutex_init(&g_hash_mutex);

    ret = cryp_hash_init();
    if (ret != HI_SUCCESS) {
        hi_log_error("error, cryp_hash_init failed\n");
        hi_log_print_func_err(cryp_hash_init, ret);
        return ret;
    }

    /* Initialize soft channel list */
    ret = crypto_channel_init(g_hash_ctx, HASH_SOFT_CHANNEL_MAX, sizeof(kapi_hash_ctx));
    if (ret != HI_SUCCESS) {
        hi_log_error("error, hash channel list init failed\n");
        hi_log_print_func_err(crypto_channel_init, ret);
        cryp_hash_deinit();
        return ret;
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 kapi_hash_deinit(hi_void)
{
    hi_s32 ret;

    hi_log_func_enter();

    ret = crypto_channel_deinit(g_hash_ctx, HASH_SOFT_CHANNEL_MAX);
    if (ret != HI_SUCCESS) {
        hi_log_error("error, hash channel list deinit failed\n");
        hi_log_print_func_err(crypto_channel_deinit, ret);
        return ret;
    }

    cryp_hash_deinit();

    crypto_mutex_destroy(&g_hash_mutex);

    hi_log_func_exit();
    return HI_SUCCESS;
}

static kapi_hash_ctx *kapi_hash_get_ctx(hi_u32 id)
{
    return crypto_channel_get_context(g_hash_ctx, HASH_SOFT_CHANNEL_MAX, id);
}

static hi_s32 kapi_hash_create(hi_u32 *id)
{
    hi_s32 ret;
    hi_u32 chn = 0;

    hi_log_func_enter();

    /* allocate a hash channel */
    ret = crypto_channel_alloc(g_hash_ctx, HASH_SOFT_CHANNEL_MAX, HASH_SOFT_CHANNEL_MASK, &chn);
    if (ret != HI_SUCCESS) {
        hi_log_error("error, allocate hash channel failed\n");
        hi_log_print_func_err(crypto_channel_alloc, ret);
        return ret;
    }

    *id = chn;

    hi_log_debug("kapi create soft chn %d\n", chn);

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 kapi_hash_destroy(hi_u32 id)
{
    hi_log_func_enter();

    hi_log_chk_param_return(id >= HASH_SOFT_CHANNEL_MAX);

    /* Free soft channel */
    crypto_channel_free(g_hash_ctx, HASH_SOFT_CHANNEL_MAX, id);

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 kpai_hash_mode_transform(hi_cipher_hash_type type, hash_mode *mode, hi_u32 *hmac)
{
    *hmac = HI_FALSE;

    /* transform hash mode */
    switch (type) {
        case HI_CIPHER_HASH_TYPE_HMAC_SHA1:
            *hmac = HMAC_HASH;
            *mode = HASH_MODE_SHA1;
            break;
        case HI_CIPHER_HASH_TYPE_SHA1:
            *mode = HASH_MODE_SHA1;
            break;
        case HI_CIPHER_HASH_TYPE_HMAC_SHA224:
            *hmac = HMAC_HASH;
            *mode = HASH_MODE_SHA224;
            break;
        case HI_CIPHER_HASH_TYPE_SHA224:
            *mode = HASH_MODE_SHA224;
            break;
        case HI_CIPHER_HASH_TYPE_HMAC_SHA256:
            *hmac = HMAC_HASH;
            *mode = HASH_MODE_SHA256;
            break;
        case HI_CIPHER_HASH_TYPE_SHA256:
            *mode = HASH_MODE_SHA256;
            break;
        case HI_CIPHER_HASH_TYPE_HMAC_SHA384:
            *hmac = HMAC_HASH;
            *mode = HASH_MODE_SHA384;
            break;
        case HI_CIPHER_HASH_TYPE_SHA384:
            *mode = HASH_MODE_SHA384;
            break;
        case HI_CIPHER_HASH_TYPE_HMAC_SHA512:
            *hmac = HMAC_HASH;
            *mode = HASH_MODE_SHA512;
            break;
        case HI_CIPHER_HASH_TYPE_SHA512:
            *mode = HASH_MODE_SHA512;
            break;
        case HI_CIPHER_HASH_TYPE_SM3:
            *mode = HASH_MODE_SM3;
            break;
        default:
            hi_log_error("error, invalid hash type %d\n", type);
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
            return HI_ERR_CIPHER_INVALID_PARAM;
    }
    return HI_SUCCESS;
}

static hi_s32 kapi_hmac_start(kapi_hash_ctx *ctx, hi_u8 *key, hi_u32 keylen)
{
    hi_s32 ret;
    hi_u8 sum[HASH_RESULT_MAX_SIZE] = {0};
    hi_u32 len = 0;
    hi_u32 i;

    hi_log_func_enter();

    hi_log_chk_param_return(key == HI_NULL);

    /* clean ipad and opad */
    crypto_memset(ctx->hmac_ipad, HASH_BLOCK_SIZE_128, 0x00, ctx->func->block_size);
    crypto_memset(ctx->hmac_opad, HASH_BLOCK_SIZE_128, 0x00, ctx->func->block_size);

    /* compute K0 */
    if (keylen <= ctx->func->block_size) {
        /* If the length of K = B: set K0 = K.
         *
         * If the length of K > B: hash K to obtain an L byte string,
         * then append (B-L) zeros to create a B-byte
         * string K0 (i.e., K0 = H(K) || 00...00).
         */
        crypto_memcpy(ctx->hmac_ipad, HASH_BLOCK_SIZE_128, key, keylen);
        crypto_memcpy(ctx->hmac_opad, HASH_BLOCK_SIZE_128, key, keylen);
    } else {
        /* If the length of K > B: hash K to obtain an L byte string,
         * then append (B-L) zeros to create a B-byte
         * string K0 (i.e., K0 = H(K) || 00...00).
         */
        ctx->cryp_ctx = ctx->func->create(ctx->func->mode); /* H(K) */
        if (ctx->cryp_ctx == HI_NULL) {
            hi_log_print_func_err(ctx->func->create, 0);
            return HI_ERR_CIPHER_BUSY;
        }

        /* update key */
        crypto_chk_err_exit(ctx->func->update(ctx->cryp_ctx, key, keylen, HASH_CHUNCK_SRC_LOCAL));

        /* sum */
        crypto_chk_err_exit(ctx->func->finish(ctx->cryp_ctx, sum, sizeof(sum), &len));
        ctx->func->destroy(ctx->cryp_ctx);
        ctx->cryp_ctx = HI_NULL;

        /* descript: K0 = H(K) || 00...00. */
        crypto_memcpy(ctx->hmac_ipad, sizeof(ctx->hmac_ipad), sum, len);
        crypto_memcpy(ctx->hmac_opad, sizeof(ctx->hmac_opad), sum, len);
    }

    /* Exclusive-Or K0 with ipad/opad byte to produce K0 ^ ipad and K0 ^ opad */
    for (i = 0; i < ctx->func->block_size; i++) {
        ctx->hmac_ipad[i] ^= HMAC_IPAD_BYTE;
        ctx->hmac_opad[i] ^= HMAC_OPAD_BYTE;
    }

    /* H(K0 ^ ipad) */
    ctx->cryp_ctx = ctx->func->create(ctx->func->mode);
    if (ctx->cryp_ctx == HI_NULL) {
        hi_log_print_func_err(ctx->func->create, 0);
        return HI_ERR_CIPHER_BUSY;
    }
    crypto_chk_err_exit(ctx->func->update(ctx->cryp_ctx, ctx->hmac_ipad, ctx->func->block_size, HASH_CHUNCK_SRC_LOCAL));

    hi_log_func_exit();
    return HI_SUCCESS;

exit__:
    ctx->func->destroy(ctx->cryp_ctx);
    ctx->cryp_ctx = HI_NULL;
    crypto_memset(ctx->hmac_ipad, sizeof(ctx->hmac_ipad), 0, sizeof(ctx->hmac_ipad));
    crypto_memset(ctx->hmac_opad, sizeof(ctx->hmac_opad), 0, sizeof(ctx->hmac_opad));
    return ret;
}

static hi_s32 kapi_hmac_finish(kapi_hash_ctx *ctx, hi_u8 *hash, hi_u32 hash_buf_len, hi_u32 *hashlen)
{
    hi_s32 ret;
    hi_u8 sum[HASH_RESULT_MAX_SIZE] = {0};

    hi_log_func_enter();

    /* descript: sum = H((K0 ^ ipad) || text). */
    ctx->func->finish(ctx->cryp_ctx, sum, sizeof(sum), hashlen);
    ctx->func->destroy(ctx->cryp_ctx);
    ctx->cryp_ctx = HI_NULL;

    /* H((K0 ^ opad)|| sum). */
    ctx->cryp_ctx = ctx->func->create(ctx->func->mode);
    if (ctx->cryp_ctx == HI_NULL) {
        hi_log_print_func_err(ctx->func->create, 0);
        return HI_ERR_CIPHER_BUSY;
    }

    /* update(K0 ^ opad) */
    ret = ctx->func->update(ctx->cryp_ctx, ctx->hmac_opad, ctx->func->block_size, HASH_CHUNCK_SRC_LOCAL);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(ctx->func->update, ret);
        return ret;
    }

    /* update(sum) */
    ret = ctx->func->update(ctx->cryp_ctx, sum, ctx->func->size, HASH_CHUNCK_SRC_LOCAL);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(ctx->func->update, ret);
        return ret;
    }

    /* H */
    ret = ctx->func->finish(ctx->cryp_ctx, hash, hash_buf_len, hashlen);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(ctx->func->finish, ret);
        return ret;
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 kapi_hash_finsh_calc(kapi_hash_ctx *ctx, hi_u8 *hash, hi_u32 hash_buf_len, hi_u32 *hashlen)
{
    hi_s32 ret;

    if (ctx->hmac) {
        ret = kapi_hmac_finish(ctx, hash, hash_buf_len, hashlen);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(kapi_hmac_finish, ret);
            return ret;
        }
    } else {
        ret = ctx->func->finish(ctx->cryp_ctx, hash, hash_buf_len, hashlen);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(ctx->func->finish, ret);
            return ret;
        }
    }

    return HI_SUCCESS;
}

static hi_s32 kapi_hash_chk_ctx(hi_u32 id, hi_u32 *soft_hash_id)
{
    hi_s32 ret;
    kapi_hash_ctx *ctx = HI_NULL;

    ret = kapi_hash_chk_handle((hi_handle)id);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(kapi_hash_chk_handle, ret);
        return ret;
    }

    *soft_hash_id = hi_handle_get_chnid(id);
    ctx = kapi_hash_get_ctx(*soft_hash_id);
    if (ctx == HI_NULL) {
        hi_log_error("kapi hash get ctx is null.\n");
        hi_log_print_func_err(kapi_hash_get_ctx, HI_ERR_CIPHER_INVALID_POINT);
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    crypto_chk_owner_err_return(&ctx->owner);
    return HI_SUCCESS;
}

static hi_s32 kapi_hash_set_ctx(kapi_hash_ctx *ctx, hash_mode mode, hi_u32 hmac)
{
    crypto_memset(ctx, sizeof(kapi_hash_ctx), 0, sizeof(kapi_hash_ctx));
    /* record owner */
    crypto_get_owner(&ctx->owner);
    ctx->hmac = hmac;

    /* Clone the function from template of hash engine */
    ctx->cryp_ctx = HI_NULL;
    ctx->func = cryp_get_hash(mode);
    if (ctx->func == HI_NULL) {
        hi_log_print_func_err(cryp_get_hash, HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    return HI_SUCCESS;
}

static hi_s32 kapi_hash_create_calc(kapi_hash_ctx *ctx, hash_mode mode, hi_u32 hmac, hi_u8 *key, hi_u32 keylen)
{
    hi_s32 ret;

    ret = kapi_hash_set_ctx(ctx, mode, hmac);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(kapi_hash_set_ctx, ret);
        return ret;
    }

    if ((ctx->func->create == HI_NULL) || (ctx->func->update == HI_NULL) || (ctx->func->finish == HI_NULL) ||
        (ctx->func->destroy == HI_NULL)) {
        hi_log_error("error, cryp hash func is null.\n");
        hi_log_print_err_code(HI_ERR_CIPHER_UNSUPPORTED);
        return HI_ERR_CIPHER_FAILED_MEM;
    }

    if (ctx->hmac) {
        ret = kapi_hmac_start(ctx, key, keylen);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(kapi_hmac_start, ret);
            return ret;
        }
    } else {
        ctx->cryp_ctx = ctx->func->create(mode);
        if (ctx->cryp_ctx == HI_NULL) {
            hi_log_print_func_err(ctx->func->create, HI_ERR_CIPHER_FAILED_MEM);
            return HI_ERR_CIPHER_FAILED_MEM;
        }
    }
    return HI_SUCCESS;
}

static hi_s32 kapi_hash_chk_type(hi_cipher_hash_type type)
{
#ifndef CHIP_SHA1_SUPPORT
    if (type == HI_CIPHER_HASH_TYPE_SHA1 || type == HI_CIPHER_HASH_TYPE_HMAC_SHA1) {
        hi_log_error("Invalid alg, unsupport sha1.\n");
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARA);
        return HI_ERR_CIPHER_INVALID_PARA;
    }
#endif
#ifndef CHIP_SHA224_SUPPORT
    if (type == HI_CIPHER_HASH_TYPE_SHA224 || type == HI_CIPHER_HASH_TYPE_HMAC_SHA224) {
        hi_log_error("Invalid alg, unsupport sha224.\n");
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARA);
        return HI_ERR_CIPHER_INVALID_PARA;
    }
#endif
    return HI_SUCCESS;
}

hi_s32 kapi_hash_start(hi_u32 *id, hi_cipher_hash_type type, hi_u8 *key, hi_u32 keylen)
{
    hi_s32 ret, ret_error;
    kapi_hash_ctx *ctx = HI_NULL;
    hash_mode mode = 0x00;
    hi_u32 hmac = 0;
    hi_u32 soft_hash_id = 0;

    hi_log_func_enter();

    hi_log_chk_param_return(id == HI_NULL);

    ret = kapi_hash_chk_type(type);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    /* transform hash mode */
    ret = kpai_hash_mode_transform(type, &mode, &hmac);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(kpai_hash_mode_transform, ret);
        return ret;
    }

    kapi_hash_lock_err_return();

    /* Create hash channel */
    ret = kapi_hash_create(&soft_hash_id);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(kapi_hash_create, ret);
        kapi_hash_unlock();
        return ret;
    }

    ctx = kapi_hash_get_ctx(soft_hash_id);
    if (ctx == HI_NULL) {
        hi_log_print_func_err(kapi_hash_get_ctx, HI_ERR_CIPHER_FAILED_MEM);
        ret = HI_ERR_CIPHER_FAILED_MEM;
        goto error1;
    }

    ret = kapi_hash_create_calc(ctx, mode, hmac, key, keylen);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(kapi_hash_create_calc, ret);
        goto error1;
    }

    *id = hi_handle_makehandle(HI_ID_CIPHER, 0, soft_hash_id);
    kapi_hash_unlock();
    hi_log_func_exit();
    return HI_SUCCESS;

error1:
    ret_error = kapi_hash_destroy(soft_hash_id);
    if (ret_error != HI_SUCCESS) {
        hi_log_print_func_err(kapi_hash_destroy, ret_error);
    }

    kapi_hash_unlock();
    hi_log_func_exit();
    return ret;
}

hi_s32 kapi_hash_update(hi_u32 id, hi_u8 *input, hi_u32 length, hash_chunk_src src)
{
    hi_s32 ret;
    kapi_hash_ctx *ctx = HI_NULL;
    hi_u32 soft_hash_id = 0;

    hi_log_func_enter();

    hi_log_chk_param_return(input > input + length); /* check overflow */
    ret = kapi_hash_chk_ctx((hi_handle)id, &soft_hash_id);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(kapi_hash_chk_ctx, ret);
        return ret;
    }

    ctx = kapi_hash_get_ctx(soft_hash_id);
    hi_log_chk_param_return(ctx->func == HI_NULL);
    hi_log_chk_param_return(ctx->func->update == HI_NULL);

    kapi_hash_lock_err_return();

    ret = ctx->func->update(ctx->cryp_ctx, input, length, src);
    /* release resource */
    if (ret != HI_SUCCESS) {
        ctx->func->destroy(ctx->cryp_ctx);
        ctx->cryp_ctx = HI_NULL;
        kapi_hash_destroy(soft_hash_id);
        hi_log_print_func_err(ctx->func->update, ret);
        kapi_hash_unlock();
        return ret;
    }

    kapi_hash_unlock();

    hi_log_func_exit();

    return HI_SUCCESS;
}

hi_s32 kapi_hash_finish(hi_u32 id, hi_u8 *hash, hi_u32 hash_buf_len, hi_u32 *hashlen)
{
    hi_s32 ret;
    kapi_hash_ctx *ctx = HI_NULL;
    hi_u32 soft_hash_id = 0;

    hi_log_func_enter();

    hi_log_chk_param_return(hash == HI_NULL);
    hi_log_chk_param_return(hash_buf_len == 0);
    hi_log_chk_param_return(hashlen == HI_NULL);

    ret = kapi_hash_chk_ctx((hi_handle)id, &soft_hash_id);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(kapi_hash_chk_ctx, ret);
        return ret;
    }
    ctx = kapi_hash_get_ctx(soft_hash_id);

    hi_log_chk_param_return(ctx->func == HI_NULL);
    hi_log_chk_param_return(ctx->func->destroy == HI_NULL);

    kapi_hash_lock_err_return();

    ret = kapi_hash_finsh_calc(ctx, hash, hash_buf_len, hashlen);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(kapi_hash_finsh_calc, ret);
        ctx->func->destroy(ctx->cryp_ctx);
        ctx->cryp_ctx = HI_NULL;
        (hi_void)kapi_hash_destroy(soft_hash_id);
        kapi_hash_unlock();
        return ret;
    }

    /* release resource */
    ctx->func->destroy(ctx->cryp_ctx);
    ctx->cryp_ctx = HI_NULL;
    ret = kapi_hash_destroy(soft_hash_id);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(kapi_hash_destroy, ret);
        kapi_hash_unlock();
        return ret;
    }

    kapi_hash_unlock();
    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 kapi_hash_release(hi_void)
{
    hi_u32 i = 0;
    hi_s32 ret;
    kapi_hash_ctx *ctx = HI_NULL;
    crypto_owner owner;

    hi_log_func_enter();

    crypto_get_owner(&owner);

    hi_log_info("hash release owner 0x%x\n", owner);

    /* destroy the channel which are created by current user */
    for (i = 0; i < HASH_SOFT_CHANNEL_MAX; i++) {
        if (g_hash_ctx[i].open == HI_TRUE) {
            ctx = kapi_hash_get_ctx(i);
            if (ctx == HI_NULL) {
                hi_log_error("kapi hash get ctx failed, point is null.\n");
                hi_log_print_err_code(HI_ERR_CIPHER_INVALID_POINT);
                return HI_ERR_CIPHER_INVALID_POINT;
            }
            if (memcmp(&owner, &ctx->owner, sizeof(owner)) != 0) {
                continue;
            }

            hi_log_info("hash release chn %d\n", i);
            if ((ctx->func != HI_NULL) && (ctx->func->destroy != HI_NULL) && (ctx->cryp_ctx != HI_NULL)) {
                ctx->func->destroy(ctx->cryp_ctx);
            }
            ctx->cryp_ctx = HI_NULL;
            ret = kapi_hash_destroy(i);
            if (ret != HI_SUCCESS) {
                hi_log_print_func_err(kapi_hash_destroy, ret);
                return ret;
            }
        }
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}
