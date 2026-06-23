/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "ot_reg.h"
#include <crypto/sha.h>
#include <linux/module.h>
#include <linux/crypto.h>
#include <linux/crypto.h>
 
osal_mutex hash_channel_lock;

static int get_hash_type(struct hash_ctx *ctx, const char *alg_name)
{
    ctx->hash_attr.is_long_term = TD_FALSE;

    if (!strcmp(alg_name, "sha256")) {
        ctx->hash_attr.hash_type = CRYPTO_HASH_TYPE_SHA256;
        ctx->digest_lenth = SHA256_DIGEST_SIZE;
    } else if (!strcmp(alg_name, "sha384")) {
        ctx->hash_attr.hash_type = CRYPTO_HASH_TYPE_SHA384;
        ctx->digest_lenth = SHA384_DIGEST_SIZE;
    } else if (!strcmp(alg_name, "sha512")) {
        ctx->hash_attr.hash_type = CRYPTO_HASH_TYPE_SHA512;
        ctx->digest_lenth = SHA512_DIGEST_SIZE;
    } else {
        cprint("input hash type is %s, not support yet\r\n", alg_name);
        return -EINVAL;
    }

    return TD_SUCCESS;
}

static int crypto_hash_ctx_get(td_handle handle, hardware_hash_clone_ctx *hard_ctx)
{
    int ret;
    crypto_hash_clone_ctx crypto_ctx;

    ret = kapi_cipher_hash_get(handle, &crypto_ctx);
    cryptodev_chk_return(ret != TD_SUCCESS, -EINVAL, "drv get ctx failed\r\n");

    hard_ctx->tail_len = crypto_ctx.tail_len;
    hard_ctx->hash_type = crypto_ctx.hash_type;
    ret = memcpy_s(hard_ctx->length, sizeof(hard_ctx->length), crypto_ctx.length, sizeof(crypto_ctx.length));
    cryptodev_chk_return(ret != TD_SUCCESS, -EINVAL, "memcpy_s failed\r\n");
    ret = memcpy_s(hard_ctx->state, sizeof(hard_ctx->state), crypto_ctx.state, sizeof(crypto_ctx.state));
    cryptodev_chk_return(ret != TD_SUCCESS, -EINVAL, "memcpy_s failed\r\n");
    ret = memcpy_s(hard_ctx->tail, sizeof(hard_ctx->tail), crypto_ctx.tail, sizeof(crypto_ctx.tail));
    cryptodev_chk_return(ret != TD_SUCCESS, -EINVAL, "memcpy_s failed\r\n");

    return TD_SUCCESS;
}

static int crypto_hash_ctx_set(td_handle handle, hardware_hash_clone_ctx *hard_ctx)
{
    int ret;
    crypto_hash_clone_ctx crypto_ctx;

    crypto_ctx.tail_len = hard_ctx->tail_len;
    crypto_ctx.hash_type = hard_ctx->hash_type;
    ret = memcpy_s(crypto_ctx.length, sizeof(crypto_ctx.length), hard_ctx->length, sizeof(hard_ctx->length));
    cryptodev_chk_return(ret != TD_SUCCESS, -EINVAL, "memcpy_s failed\r\n");
    ret = memcpy_s(crypto_ctx.state, sizeof(crypto_ctx.state), hard_ctx->state, sizeof(hard_ctx->state));
    cryptodev_chk_return(ret != TD_SUCCESS, -EINVAL, "memcpy_s failed\r\n");
    ret = memcpy_s(crypto_ctx.tail, sizeof(crypto_ctx.tail), hard_ctx->tail, sizeof(hard_ctx->tail));
    cryptodev_chk_return(ret != TD_SUCCESS, -EINVAL, "memcpy_s failed\r\n");

    ret = kapi_cipher_hash_set(handle, &crypto_ctx);
    cryptodev_chk_return(ret != TD_SUCCESS, -EINVAL, "drv get ctx failed\r\n");

    return TD_SUCCESS;
}

static int hash_init(struct shash_desc *desc)
{
    int ret = TD_FAILURE;
    struct hash_ctx *ctx = NULL;
    const char *alg_name = NULL;

    ctx = shash_desc_ctx(desc);
    ctx->is_init = TD_FALSE;
    alg_name = crypto_tfm_alg_name(&desc->tfm->base);

    ret = get_hash_type(ctx, alg_name);
    cryptodev_chk_return(ret != TD_SUCCESS, -EINVAL, "get_hash_type failed\r\n");

    ret = kapi_cipher_hash_init();
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, channel_free, -EINVAL, "kapi_cipher_hash_init failed\r\n");

    ret = kapi_cipher_hash_start(&ctx->hash_handle, &ctx->hash_attr);
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, hash_deinit, -EINVAL, "kapi_cipher_hash_start failed\r\n");

    ret = crypto_hash_ctx_get(ctx->hash_handle, &ctx->clone_ctx);
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, hash_destory, -EINVAL, "kapi_cipher_hash_set failed\r\n");

    ctx->is_init = TD_TRUE;
hash_destory:
    (void)kapi_cipher_hash_destroy(ctx->hash_handle);
hash_deinit:
    (void)kapi_cipher_hash_deinit();
channel_free:
    return ret;
}


static int hash_update(struct shash_desc *desc, const u8 *data,
    unsigned int len)
{
    int ret = TD_FAILURE;
    struct hash_ctx *ctx = NULL;
    const crypto_buf_attr src_buf = {
        .virt_addr = (u8 *)data
    };

    ctx = shash_desc_ctx(desc);
    cryptodev_chk_goto_with_ret(ctx->is_init != TD_TRUE, not_init_exit, -EINVAL, "hash channel not init\r\n");

    ret = kapi_cipher_hash_init();
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, not_init_exit, -EINVAL, "kapi_cipher_hash_init failed\r\n");

    ret = kapi_cipher_hash_start(&ctx->hash_handle, &ctx->hash_attr);
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, hash_deinit, -EINVAL, "kapi_cipher_hash_start failed\r\n");

    ret = crypto_hash_ctx_set(ctx->hash_handle, &ctx->clone_ctx);
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, hash_destroy, -EINVAL, "kapi_cipher_hash_set failed\r\n");

    ret = kapi_cipher_hash_update(ctx->hash_handle, &src_buf, len);
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, hash_destroy, -EINVAL, "kapi_cipher_hash_update failed\r\n");

    ret = crypto_hash_ctx_get(ctx->hash_handle, &ctx->clone_ctx);
hash_destroy:
    (void)kapi_cipher_hash_destroy(ctx->hash_handle);
hash_deinit:
    (void)kapi_cipher_hash_deinit();
not_init_exit:
    if (ret != TD_SUCCESS) {
        ctx->is_init = TD_FALSE;
    }
    return ret;
}

static int hash_final(struct shash_desc *desc, u8 *out)
{
    int ret = TD_FAILURE;
    struct hash_ctx *ctx = NULL;

    ctx = shash_desc_ctx(desc);
    cryptodev_chk_goto_with_ret(ctx->is_init != TD_TRUE, not_init_exit, -EINVAL, "hash channel not init\r\n");

    ret = kapi_cipher_hash_init();
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, not_init_exit, -EINVAL, "kapi_cipher_hash_init failed\r\n");

    ret = kapi_cipher_hash_start(&ctx->hash_handle, &ctx->hash_attr);
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, hash_deinit, -EINVAL, "kapi_cipher_hash_start failed\r\n");

    ret = crypto_hash_ctx_set(ctx->hash_handle, &ctx->clone_ctx);
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, hash_destroy, -EINVAL, "kapi_cipher_hash_set failed\r\n");

    ret = kapi_cipher_hash_finish(ctx->hash_handle, out, &ctx->digest_lenth);
    if (ret == TD_SUCCESS) {
        goto hash_deinit;
    }
hash_destroy:
    (void)kapi_cipher_hash_destroy(ctx->hash_handle);
hash_deinit:
    (void)kapi_cipher_hash_deinit();
not_init_exit:
    ctx->is_init = TD_FALSE;
    return ret;
}

static int hash_finup(struct shash_desc *desc, const u8 *data,
    unsigned int len, u8 *out)
{
    int ret;
    struct hash_ctx *ctx = NULL;
    const crypto_buf_attr src_buf = {
        .virt_addr = (u8 *)data
    };

    ctx = shash_desc_ctx(desc);
    cryptodev_chk_goto_with_ret(ctx->is_init != TD_TRUE, not_init_exit, -EINVAL, "hash channel not init\r\n");

    ret = kapi_cipher_hash_init();
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, not_init_exit, -EINVAL, "kapi_cipher_hash_init failed\r\n");

    ret = kapi_cipher_hash_start(&ctx->hash_handle, &ctx->hash_attr);
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, hash_deinit, -EINVAL, "kapi_cipher_hash_start failed\r\n");

    ret = crypto_hash_ctx_set(ctx->hash_handle, &ctx->clone_ctx);
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, hash_destroy, -EINVAL, "kapi_cipher_hash_set failed\r\n");

    ret = kapi_cipher_hash_update(ctx->hash_handle, &src_buf, len);
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, hash_destroy, -EINVAL, "kapi_cipher_hash_update failed\r\n");

    ret = kapi_cipher_hash_finish(ctx->hash_handle, out, &ctx->digest_lenth);
    if (ret == TD_SUCCESS) {
        goto hash_deinit;
    }
hash_destroy:
    (void)kapi_cipher_hash_destroy(ctx->hash_handle);
hash_deinit:
    (void)kapi_cipher_hash_deinit();
not_init_exit:
    ctx->is_init = TD_FALSE;
    return ret;
}

static int lock_hash_init(struct shash_desc *desc)
{
    int ret;
    enter_func();
    osal_mutex_lock(&hash_channel_lock);
    ret = hash_init(desc);
    osal_mutex_unlock(&hash_channel_lock);
    exit_func();
    cryptodev_chk_return(ret != TD_SUCCESS, -EINVAL, "lock_hash_init failed\r\n");
    return ret;
}

static int lock_hash_update(struct shash_desc *desc, const u8 *data,
    unsigned int len)
{
    int ret;
    enter_func();
    osal_mutex_lock(&hash_channel_lock);
    ret = hash_update(desc, data, len);
    osal_mutex_unlock(&hash_channel_lock);
    exit_func();
    cryptodev_chk_return(ret != TD_SUCCESS, -EINVAL, "lock_hash_update failed\r\n");
    return ret;
}

static int lock_hash_final(struct shash_desc *desc, u8 *out)
{
    int ret;
    enter_func();
    osal_mutex_lock(&hash_channel_lock);
    ret = hash_final(desc, out);
    osal_mutex_unlock(&hash_channel_lock);
    exit_func();
    cryptodev_chk_return(ret != TD_SUCCESS, -EINVAL, "lock_hash_final failed\r\n");
    return ret;
}

static int lock_hash_finup(struct shash_desc *desc, const u8 *data,
    unsigned int len, u8 *out)
{
    int ret;
    enter_func();
    osal_mutex_lock(&hash_channel_lock);
    ret = hash_finup(desc, data, len, out);
    osal_mutex_unlock(&hash_channel_lock);
    exit_func();
    cryptodev_chk_return(ret != TD_SUCCESS, -EINVAL, "lock_hash_finup failed\r\n");
    return ret;
}

struct shash_alg shash_alges[3] = {
    {
        .digestsize	=	SHA256_DIGEST_SIZE,
        .init		=	lock_hash_init,
        .update		=	lock_hash_update,
        .final		=	lock_hash_final,
        .finup		=	lock_hash_finup,
        .descsize	=	sizeof(struct hash_ctx),
        .base		=	{
            .cra_name = "sha256",
            .cra_driver_name = "ot_drv",
            .cra_priority =	2000,
            .cra_blocksize = SHA256_BLOCK_SIZE,
            .cra_module	= THIS_MODULE,
        }
    },
    {
        .digestsize	=	SHA384_DIGEST_SIZE,
        .init		=	lock_hash_init,
        .update		=	lock_hash_update,
        .final		=	lock_hash_final,
        .finup		=	lock_hash_finup,
        .descsize	=	sizeof(struct hash_ctx),
        .base		=	{
            .cra_name =	"sha384",
            .cra_driver_name =	"ot_drv",
            .cra_priority =	2000,
            .cra_blocksize = SHA384_BLOCK_SIZE,
            .cra_module	= THIS_MODULE,
        }
    },
    {
        .digestsize	=	SHA512_DIGEST_SIZE,
        .init		=	lock_hash_init,
        .update		=	lock_hash_update,
        .final		=	lock_hash_final,
        .finup		=	lock_hash_finup,
        .descsize	=	sizeof(struct hash_ctx),
        .base		=	{
            .cra_name =	"sha512",
            .cra_driver_name =	"ot_drv",
            .cra_priority =	2000,
            .cra_blocksize = SHA512_BLOCK_SIZE,
            .cra_module	= THIS_MODULE,
        }
    }
};