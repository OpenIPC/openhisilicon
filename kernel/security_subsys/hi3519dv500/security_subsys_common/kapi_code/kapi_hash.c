/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "kapi_hash.h"
#include "kapi_inner.h"
#include "drv_hash.h"
#include "crypto_common_macro.h"
#include "crypto_errno.h"
#include "crypto_drv_common.h"

#define HASH_COMPAT_ERRNO(err_code)     KAPI_COMPAT_ERRNO(ERROR_MODULE_HASH, err_code)

#define CRYPTO_HASH_INIT_MAX_NUM            0xffffffff

static crypto_kapi_hash_process g_kapi_hash_channel[MAX_PROCESS_NUM];

static crypto_mutex g_hash_mutex;

#define kapi_hash_lock() do {                   \
    crypto_mutex_lock(&g_hash_mutex);           \
} while (0)
#define kapi_hash_unlock() do {                 \
    crypto_mutex_unlock(&g_hash_mutex);         \
} while (0)

static td_s32 priv_hash_handle_check(td_handle kapi_hash_handle)
{
    crypto_chk_return(kapi_get_module_id(kapi_hash_handle) != KAPI_HASH_MODULE_ID,
        HASH_COMPAT_ERRNO(ERROR_INVALID_HANDLE), "Invalid Hash Handle! 0x%x\n", kapi_hash_handle);
    crypto_chk_return(kapi_get_ctx_idx(kapi_hash_handle) >= CRYPTO_HASH_VIRT_CHN_NUM,
        HASH_COMPAT_ERRNO(ERROR_INVALID_HANDLE), "Invalid Hash Handle! 0x%x\n", kapi_hash_handle);
    return TD_SUCCESS;
}

static crypto_kapi_hash_process *priv_get_current_hash_channel(td_void)
{
    td_u32 i;
    crypto_owner owner;
    if (crypto_get_owner(&owner) != CRYPTO_SUCCESS) {
        return TD_NULL;
    }
    for (i = 0; i < MAX_PROCESS_NUM; i++) {
        if (memcmp(&owner, &g_kapi_hash_channel[i].owner, sizeof(crypto_owner)) == 0) {
            return &g_kapi_hash_channel[i];
        }
    }
    return TD_NULL;
}

static crypto_kapi_hash_ctx *priv_occupy_hash_soft_chn(crypto_kapi_hash_process *hash_channel, td_u32 *idx)
{
    td_u32 i;
    td_u32 tid = crypto_gettid();
    crypto_kapi_hash_ctx *hash_ctx_list = TD_NULL;
    crypto_kapi_hash_ctx *hash_ctx = TD_NULL;

    kapi_hash_lock();

    hash_ctx_list = hash_channel->hash_ctx_list;
    for (i = 0; i < CRYPTO_HASH_VIRT_CHN_NUM; i++) {
        if (hash_ctx_list[i].is_open == TD_FALSE) {
            break;
        }
    }
    if (i >= CRYPTO_HASH_VIRT_CHN_NUM) {
        crypto_log_err("All Hash Channels are busy!\n");
        goto exit_unlock;
    }
    hash_ctx = &hash_channel->hash_ctx_list[i];
    (td_void)memset_s(hash_ctx, sizeof(crypto_kapi_hash_ctx), 0, sizeof(crypto_kapi_hash_ctx));
    hash_ctx->is_open = TD_TRUE;
    hash_ctx->tid = tid;

    *idx = i;

exit_unlock:
    kapi_hash_unlock();
    return hash_ctx;
}

static td_void priv_release_hash_soft_chn(crypto_kapi_hash_ctx *hash_ctx)
{
    kapi_hash_lock();
    (td_void)memset_s(hash_ctx, sizeof(crypto_kapi_hash_ctx), 0, sizeof(crypto_kapi_hash_ctx));
    kapi_hash_unlock();
}

static td_bool priv_check_is_init(crypto_owner *owner)
{
    td_u32 i;
    for (i = 0; i < MAX_PROCESS_NUM; i++) {
        if (memcmp(owner, &g_kapi_hash_channel[i].owner, sizeof(crypto_owner)) == 0) {
            return TD_TRUE;
        }
    }
    return TD_FALSE;
}

static td_s32 priv_process_hash_init(td_void)
{
    td_u32 i;
    td_u32 ret = TD_SUCCESS;
    crypto_owner owner;
    crypto_kapi_hash_process *hash_channel = TD_NULL;

    crypto_kapi_func_enter();
    kapi_hash_lock();
    ret = crypto_get_owner(&owner);
    crypto_chk_goto_with_ret(ret != CRYPTO_SUCCESS, exit_unlock, HASH_COMPAT_ERRNO(ERROR_GET_OWNER),
        "crypto_get_owner failed\n");
    if (priv_check_is_init(&owner) == TD_TRUE) {
        hash_channel = priv_get_current_hash_channel();
        if (hash_channel->init_counter >= CRYPTO_HASH_INIT_MAX_NUM) {
            ret = HASH_COMPAT_ERRNO(ERROR_COUNT_OVERFLOW);
        } else {
            ret = TD_SUCCESS;
            ++(hash_channel->init_counter);
        }
        goto exit_unlock;
    }
    for (i = 0; i < MAX_PROCESS_NUM; i++) {
        if (g_kapi_hash_channel[i].is_used == TD_FALSE) {
            break;
        }
    }
    if (i >= MAX_PROCESS_NUM) {
        crypto_log_err("Process Num is More Than %u\n", MAX_PROCESS_NUM);
        ret = HASH_COMPAT_ERRNO(ERROR_MAX_PROCESS);
        goto exit_unlock;
    }
    hash_channel = &g_kapi_hash_channel[i];
    (td_void)memset_s(hash_channel, sizeof(crypto_kapi_hash_process), 0, sizeof(crypto_kapi_hash_process));

    for (i = 0; i < CRYPTO_HASH_VIRT_CHN_NUM; ++i) {
        ret = crypto_mutex_init(&hash_channel->hash_ctx_mutex[i]);
        crypto_chk_goto_with_ret(ret != TD_SUCCESS, exit_destroy, HASH_COMPAT_ERRNO(ERROR_MUTEX_INIT),
            "hash ctx mutex init failed at chn: %u\n", i);
    }
    (td_void)memcpy_s(&hash_channel->owner, sizeof(crypto_owner), &owner, sizeof(crypto_owner));
    hash_channel->is_used = TD_TRUE;
    hash_channel->ctx_num = CRYPTO_HASH_VIRT_CHN_NUM;
    ++(hash_channel->init_counter);
    (td_void)memset_s(hash_channel->hash_ctx_list, sizeof(crypto_kapi_hash_ctx) * CRYPTO_HASH_VIRT_CHN_NUM,
        0, sizeof(crypto_kapi_hash_ctx) * CRYPTO_HASH_VIRT_CHN_NUM);
    kapi_hash_unlock();

    crypto_kapi_func_exit();
    return ret;

exit_destroy:
    for (i = 0; i < CRYPTO_HASH_VIRT_CHN_NUM; ++i) {
        crypto_mutex_destroy(&hash_channel->hash_ctx_mutex[i]);
    }
exit_unlock:
    kapi_hash_unlock();
    crypto_kapi_func_exit();
    return ret;
}

static td_void priv_process_release(crypto_kapi_hash_process *hash_channel)
{
    td_u32 i;
    crypto_kapi_hash_ctx *hash_ctx_list = TD_NULL;
    crypto_kapi_hash_ctx *hash_ctx = TD_NULL;
    hash_ctx_list = hash_channel->hash_ctx_list;
    for (i = 0; i < CRYPTO_HASH_VIRT_CHN_NUM; ++i) {
        hash_ctx = &hash_ctx_list[i];
        crypto_mutex_destroy(&hash_channel->hash_ctx_mutex[i]);
        if (hash_ctx->is_open == TD_TRUE && hash_ctx->is_long_term) {
            drv_cipher_hash_destroy(hash_ctx->drv_hash_handle);
        }
    }
    (td_void)memset_s(hash_channel, sizeof(crypto_kapi_hash_process), 0, sizeof(crypto_kapi_hash_process));
}

static td_void priv_process_hash_deinit(td_void)
{
    crypto_kapi_hash_process *hash_channel = TD_NULL;
    crypto_kapi_func_enter();
    kapi_hash_lock();
    hash_channel = priv_get_current_hash_channel();
    if (hash_channel == TD_NULL) {
        kapi_hash_unlock();
        return;
    }
    if (hash_channel->init_counter > 1) {
        --(hash_channel->init_counter);
        kapi_hash_unlock();
        return;
    }
    priv_process_release(hash_channel);
    kapi_hash_unlock();
    crypto_kapi_func_exit();
}

static td_s32 priv_drv_lock_start(crypto_kapi_hash_ctx *hash_ctx, const crypto_hash_attr *hash_attr)
{
    td_s32 ret = TD_SUCCESS;
    kapi_hash_lock();
    ret = drv_cipher_hash_start(&hash_ctx->drv_hash_handle, hash_attr);
    kapi_hash_unlock();
    return ret;
}

static td_s32 priv_drv_lock_finish(td_handle drv_hash_handle, td_u8 *out, td_u32 *out_len)
{
    td_s32 ret = TD_SUCCESS;
    kapi_hash_lock();
    ret = drv_cipher_hash_finish(drv_hash_handle, out, out_len);
    kapi_hash_unlock();
    return ret;
}

static td_void priv_drv_lock_destroy(td_handle drv_hash_handle)
{
    kapi_hash_lock();
    (td_void)drv_cipher_hash_destroy(drv_hash_handle);
    kapi_hash_unlock();
}

td_void kapi_cipher_hash_process_release(td_void)
{
    crypto_kapi_hash_process *hash_channel = TD_NULL;
    crypto_kapi_func_enter();
    kapi_hash_lock();
    hash_channel = priv_get_current_hash_channel();
    if (hash_channel == TD_NULL) {
        kapi_hash_unlock();
        return;
    }
    priv_process_release(hash_channel);
    kapi_hash_unlock();
    crypto_kapi_func_exit();
}

td_s32 kapi_cipher_hash_env_init(td_void)
{
    td_s32 ret = TD_SUCCESS;
    crypto_kapi_func_enter();

    ret = drv_cipher_hash_init();
    if (ret != TD_SUCCESS) {
        crypto_log_err("drv_cipher_hash_init failed\n");
        return ret;
    }

    (td_void)memset_s(&g_kapi_hash_channel, sizeof(g_kapi_hash_channel), 0, sizeof(g_kapi_hash_channel));
    ret = crypto_mutex_init(&g_hash_mutex);
    if (ret != TD_SUCCESS) {
        crypto_log_err("crypto_mutex_init failed\n");
        ret = HASH_COMPAT_ERRNO(ERROR_MUTEX_INIT);
        goto error_hash_deinit;
    }

    crypto_kapi_func_exit();
    return ret;

error_hash_deinit:
    drv_cipher_hash_deinit();
    return ret;
}

td_s32 kapi_cipher_hash_env_deinit(td_void)
{
    td_s32 ret = TD_SUCCESS;
    td_u32 i, j;
    crypto_kapi_hash_process *hash_channel = TD_NULL;
    crypto_kapi_hash_ctx *hash_ctx = TD_NULL;
    crypto_kapi_func_enter();

    /* release all the resource. */
    for (i = 0; i < MAX_PROCESS_NUM; i++) {
        hash_channel = &g_kapi_hash_channel[i];
        if (hash_channel->is_used == TD_FALSE) {
            continue;
        }
        for (j = 0; j < CRYPTO_HASH_VIRT_CHN_NUM; j++) {
            hash_ctx = &hash_channel->hash_ctx_list[j];
            if (hash_ctx->is_open == TD_FALSE) {
                continue;
            }
            priv_release_hash_soft_chn(hash_ctx);
            hash_ctx->is_open = TD_FALSE;
        }
        hash_channel->is_used = TD_FALSE;
    }
    crypto_mutex_destroy(&g_hash_mutex);
    (td_void)memset_s(&g_kapi_hash_channel, sizeof(g_kapi_hash_channel), 0, sizeof(g_kapi_hash_channel));
    drv_cipher_hash_deinit();
    crypto_kapi_func_exit();
    return ret;
}

td_s32 kapi_cipher_hash_init(td_void)
{
    td_s32 ret = TD_SUCCESS;
    crypto_kapi_func_enter();

    ret = priv_process_hash_init();
    if (ret != TD_SUCCESS) {
        crypto_log_err("hash priv_process_hash_init failed\n");
        return ret;
    }

    crypto_kapi_func_exit();
    return ret;
}
CRYPTO_EXPORT_SYMBOL(kapi_cipher_hash_init);

td_s32 kapi_cipher_hash_deinit(td_void)
{
    crypto_kapi_func_enter();
    priv_process_hash_deinit();
    crypto_kapi_func_exit();
    return TD_SUCCESS;
}
CRYPTO_EXPORT_SYMBOL(kapi_cipher_hash_deinit);

td_s32 kapi_cipher_hash_start(td_handle *kapi_hash_handle, const crypto_hash_attr *hash_attr)
{
    td_s32 ret = TD_SUCCESS;
    crypto_kapi_hash_process *hash_channel = TD_NULL;
    crypto_kapi_hash_ctx *hash_ctx = TD_NULL;
    td_u32 idx = 0;

    crypto_kapi_func_enter();
    crypto_chk_return(kapi_hash_handle == TD_NULL, HASH_COMPAT_ERRNO(ERROR_PARAM_IS_NULL),
        "kapi_hash_handle is NULL\n");
    crypto_chk_return(hash_attr == TD_NULL, HASH_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "hash_attr is NULL\n");

    if (CRYPTO_HASH_IS_HMAC(hash_attr->hash_type) == TD_TRUE) {
        crypto_chk_return(hash_attr->is_long_term != TD_TRUE && hash_attr->is_keyslot == TD_TRUE,
            HASH_COMPAT_ERRNO(ERROR_UNSUPPORT), "Hmac does not support short term calculation for keyslot\n");
    }

    hash_channel = priv_get_current_hash_channel();
    crypto_chk_return(hash_channel == TD_NULL, HASH_COMPAT_ERRNO(ERROR_PROCESS_NOT_INIT), "call init first!\n");

    hash_ctx = priv_occupy_hash_soft_chn(hash_channel, &idx);
    crypto_chk_return(hash_ctx == TD_NULL, HASH_COMPAT_ERRNO(ERROR_CHN_BUSY), "all hash soft chns are busy\n");

    /* lock soft chn. */
    crypto_mutex_lock(&hash_channel->hash_ctx_mutex[idx]);

    if (hash_attr->is_long_term == TD_TRUE) {
        ret = priv_drv_lock_start(hash_ctx, hash_attr);
        crypto_chk_goto(ret != TD_SUCCESS, error_unlock_hash_ctx, "priv_drv_lock_start failed\n");
    } else {
        ret = priv_drv_lock_start(hash_ctx, hash_attr);
        crypto_chk_goto(ret != TD_SUCCESS, error_unlock_hash_ctx, "priv_drv_lock_start failed\n");
        ret = drv_cipher_hash_get(hash_ctx->drv_hash_handle, &hash_ctx->hash_clone_ctx);
        crypto_chk_goto(ret != TD_SUCCESS, error_unlock_drv_hash_ctx, "drv_cipher_hash_get failed\n");
        priv_drv_lock_destroy(hash_ctx->drv_hash_handle);
    }

    hash_ctx->is_long_term = hash_attr->is_long_term;

    crypto_mutex_unlock(&hash_channel->hash_ctx_mutex[idx]);

    *kapi_hash_handle = synthesize_kapi_handle(KAPI_HASH_MODULE_ID, idx);

    crypto_kapi_func_exit();
    return ret;
error_unlock_drv_hash_ctx:
    priv_drv_lock_destroy(hash_ctx->drv_hash_handle);
error_unlock_hash_ctx:
    priv_release_hash_soft_chn(hash_ctx);
    crypto_mutex_unlock(&hash_channel->hash_ctx_mutex[idx]);
    crypto_kapi_func_exit();
    return ret;
}
CRYPTO_EXPORT_SYMBOL(kapi_cipher_hash_start);

td_s32 kapi_cipher_hash_update(td_handle kapi_hash_handle,  const crypto_buf_attr *src_buf, const td_u32 len)
{
    td_s32 ret = TD_SUCCESS;
    crypto_kapi_hash_process *hash_channel = TD_NULL;
    crypto_kapi_hash_ctx *hash_ctx = TD_NULL;
    crypto_hash_attr default_hash_attr = {
        .hash_type = CRYPTO_HASH_TYPE_SHA256,
        .is_long_term = TD_TRUE
    };
    td_u32 idx;
    crypto_kapi_func_enter();

    ret =  priv_hash_handle_check(kapi_hash_handle);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_hash_handle_check failed, ret is 0x%x\n", ret);
    idx = kapi_get_ctx_idx(kapi_hash_handle);

    hash_channel = priv_get_current_hash_channel();
    crypto_chk_return(hash_channel == TD_NULL, HASH_COMPAT_ERRNO(ERROR_PROCESS_NOT_INIT), "call init first!\n");

    hash_ctx = &hash_channel->hash_ctx_list[idx];
    crypto_chk_return(hash_ctx->is_open == TD_FALSE, HASH_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed!\n");

    /* lock soft chn. */
    crypto_mutex_lock(&hash_channel->hash_ctx_mutex[idx]);
    if (hash_ctx->is_long_term == TD_TRUE) {
        ret = drv_cipher_hash_update(hash_ctx->drv_hash_handle, src_buf, len);
        crypto_chk_goto(ret != TD_SUCCESS, error_unlock_hash_ctx, "drv_cipher_hash_update failed\n");
    } else {
        ret = priv_drv_lock_start(hash_ctx, &default_hash_attr);
        crypto_chk_goto(ret != TD_SUCCESS, error_unlock_hash_ctx, "priv_drv_lock_start failed\n");
        ret = drv_cipher_hash_set(hash_ctx->drv_hash_handle, &hash_ctx->hash_clone_ctx);
        crypto_chk_goto(ret != TD_SUCCESS, hash_destroy_exit, "drv_cipher_hash_set failed\n");
        ret = drv_cipher_hash_update(hash_ctx->drv_hash_handle, src_buf, len);
        crypto_chk_goto(ret != TD_SUCCESS, hash_destroy_exit, "drv_cipher_hash_update failed\n");
        ret = drv_cipher_hash_get(hash_ctx->drv_hash_handle, &hash_ctx->hash_clone_ctx);
        crypto_chk_goto(ret != TD_SUCCESS, hash_destroy_exit, "drv_cipher_hash_set failed\n");
        priv_drv_lock_destroy(hash_ctx->drv_hash_handle);
    }
    crypto_mutex_unlock(&hash_channel->hash_ctx_mutex[idx]);
    crypto_kapi_func_exit();
    return ret;

hash_destroy_exit:
    priv_drv_lock_destroy(hash_ctx->drv_hash_handle);
error_unlock_hash_ctx:
    crypto_mutex_unlock(&hash_channel->hash_ctx_mutex[idx]);
    crypto_kapi_func_exit();
    return ret;
}
CRYPTO_EXPORT_SYMBOL(kapi_cipher_hash_update);

td_s32 kapi_cipher_hash_finish(td_handle kapi_hash_handle, td_u8 *out, td_u32 *out_len)
{
    td_s32 ret = TD_SUCCESS;
    crypto_kapi_hash_process *hash_channel = TD_NULL;
    crypto_kapi_hash_ctx *hash_ctx = TD_NULL;
    crypto_hash_attr default_hash_attr = {
        .hash_type = CRYPTO_HASH_TYPE_SHA256,
        .is_long_term = TD_TRUE
    };
    td_u32 idx = 0;
    crypto_kapi_func_enter();

    ret =  priv_hash_handle_check(kapi_hash_handle);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_hash_handle_check failed, ret is 0x%x\n", ret);
    idx = kapi_get_ctx_idx(kapi_hash_handle);

    hash_channel = priv_get_current_hash_channel();
    crypto_chk_return(hash_channel == TD_NULL, HASH_COMPAT_ERRNO(ERROR_PROCESS_NOT_INIT), "call init first!\n");

    hash_ctx = &hash_channel->hash_ctx_list[idx];
    crypto_chk_return(hash_ctx->is_open == TD_FALSE, HASH_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed!\n");

    /* lock soft chn. */
    crypto_mutex_lock(&hash_channel->hash_ctx_mutex[idx]);
    if (hash_ctx->is_long_term == TD_TRUE) {
        ret = priv_drv_lock_finish(hash_ctx->drv_hash_handle, out, out_len);
        crypto_chk_goto(ret != TD_SUCCESS, error_unlock_hash_ctx, "priv_drv_lock_finish failed\n");
    } else {
        ret = priv_drv_lock_start(hash_ctx, &default_hash_attr);
        crypto_chk_goto(ret != TD_SUCCESS, error_unlock_hash_ctx, "priv_drv_lock_start failed\n");
        ret = drv_cipher_hash_set(hash_ctx->drv_hash_handle, &hash_ctx->hash_clone_ctx);
        crypto_chk_goto(ret != TD_SUCCESS, hash_destroy_exit, "drv_cipher_hash_set failed\n");
        ret = priv_drv_lock_finish(hash_ctx->drv_hash_handle, out, out_len);
        crypto_chk_goto(ret != TD_SUCCESS, error_unlock_hash_ctx, "priv_drv_lock_finish failed\n");
    }

    priv_release_hash_soft_chn(hash_ctx);
    crypto_mutex_unlock(&hash_channel->hash_ctx_mutex[idx]);
    crypto_kapi_func_exit();
    return ret;

hash_destroy_exit:
    priv_drv_lock_destroy(hash_ctx->drv_hash_handle);
error_unlock_hash_ctx:
    crypto_mutex_unlock(&hash_channel->hash_ctx_mutex[idx]);
    crypto_kapi_func_exit();
    return ret;
}
CRYPTO_EXPORT_SYMBOL(kapi_cipher_hash_finish);

td_s32 kapi_cipher_hash_get(td_handle kapi_hash_handle, crypto_hash_clone_ctx *hash_clone_ctx)
{
    td_s32 ret = TD_SUCCESS;
    crypto_kapi_hash_process *hash_channel = TD_NULL;
    crypto_kapi_hash_ctx *hash_ctx = TD_NULL;
    td_u32 idx = 0;
    crypto_kapi_func_enter();

    crypto_chk_return(hash_clone_ctx == TD_NULL, HASH_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "hash_clone_ctx is NULL\n");

    ret =  priv_hash_handle_check(kapi_hash_handle);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_hash_handle_check failed, ret is 0x%x\n", ret);
    idx = kapi_get_ctx_idx(kapi_hash_handle);

    hash_channel = priv_get_current_hash_channel();
    crypto_chk_return(hash_channel == TD_NULL, HASH_COMPAT_ERRNO(ERROR_PROCESS_NOT_INIT), "call init first!\n");

    hash_ctx = &hash_channel->hash_ctx_list[idx];
    crypto_chk_return(hash_ctx->is_open == TD_FALSE, HASH_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed!\n");

    /* lock soft chn. */
    crypto_mutex_lock(&hash_channel->hash_ctx_mutex[idx]);
    if (hash_ctx->is_long_term == TD_TRUE) {
        ret = drv_cipher_hash_get(hash_ctx->drv_hash_handle, hash_clone_ctx);
        crypto_chk_goto(ret != TD_SUCCESS, exit_unlock, "drv_cipher_hash_get failed\n");
    } else {
        ret = memcpy_s(hash_clone_ctx, sizeof(crypto_hash_clone_ctx),
            &hash_ctx->hash_clone_ctx, sizeof(crypto_hash_clone_ctx));
        crypto_chk_goto_with_ret(ret != EOK, exit_unlock, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");
    }

exit_unlock:
    crypto_mutex_unlock(&hash_channel->hash_ctx_mutex[idx]);
    crypto_kapi_func_exit();
    return ret;
}
CRYPTO_EXPORT_SYMBOL(kapi_cipher_hash_get);

td_s32 kapi_cipher_hash_set(td_handle kapi_hash_handle, const crypto_hash_clone_ctx *hash_clone_ctx)
{
    td_s32 ret = TD_SUCCESS;
    crypto_kapi_hash_process *hash_channel = TD_NULL;
    crypto_kapi_hash_ctx *hash_ctx = TD_NULL;
    td_u32 idx = 0;
    crypto_kapi_func_enter();

    crypto_chk_return(hash_clone_ctx == TD_NULL, HASH_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "hash_clone_ctx is NULL\n");
    ret =  priv_hash_handle_check(kapi_hash_handle);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_hash_handle_check failed, ret is 0x%x\n", ret);
    idx = kapi_get_ctx_idx(kapi_hash_handle);

    hash_channel = priv_get_current_hash_channel();
    crypto_chk_return(hash_channel == TD_NULL, HASH_COMPAT_ERRNO(ERROR_PROCESS_NOT_INIT), "call init first!\n");

    hash_ctx = &hash_channel->hash_ctx_list[idx];
    crypto_chk_return(hash_ctx->is_open == TD_FALSE, HASH_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed!\n");

    /* lock soft chn. */
    crypto_mutex_lock(&hash_channel->hash_ctx_mutex[idx]);
    if (hash_ctx->is_long_term == TD_TRUE) {
        ret = drv_cipher_hash_set(hash_ctx->drv_hash_handle, hash_clone_ctx);
        crypto_chk_goto(ret != TD_SUCCESS, exit_unlock, "drv_cipher_hash_set failed\n");
    } else {
        ret = memcpy_s(&hash_ctx->hash_clone_ctx, sizeof(hash_ctx->hash_clone_ctx),  \
            hash_clone_ctx, sizeof(crypto_hash_clone_ctx));
        crypto_chk_goto_with_ret(ret != EOK, exit_unlock, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");
    }

exit_unlock:
    crypto_mutex_unlock(&hash_channel->hash_ctx_mutex[idx]);
    crypto_kapi_func_exit();
    return ret;
}
CRYPTO_EXPORT_SYMBOL(kapi_cipher_hash_set);

td_s32 kapi_cipher_hash_destroy(td_handle kapi_hash_handle)
{
    td_s32 ret = TD_SUCCESS;
    crypto_kapi_hash_process *hash_channel = TD_NULL;
    crypto_kapi_hash_ctx *hash_ctx = TD_NULL;
    td_u32 idx = 0;
    crypto_kapi_func_enter();

    ret =  priv_hash_handle_check(kapi_hash_handle);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_hash_handle_check failed, ret is 0x%x\n", ret);
    idx = kapi_get_ctx_idx(kapi_hash_handle);

    hash_channel = priv_get_current_hash_channel();
    crypto_chk_return(hash_channel == TD_NULL, HASH_COMPAT_ERRNO(ERROR_PROCESS_NOT_INIT), "call init first!\n");

    hash_ctx = &hash_channel->hash_ctx_list[idx];
    crypto_chk_return(hash_ctx->is_open == TD_FALSE, HASH_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed!\n");

    /* lock soft chn. */
    crypto_mutex_lock(&hash_channel->hash_ctx_mutex[idx]);
    if (hash_ctx->is_long_term == TD_TRUE) {
        priv_drv_lock_destroy(hash_ctx->drv_hash_handle);
    }

    priv_release_hash_soft_chn(hash_ctx);

    crypto_mutex_unlock(&hash_channel->hash_ctx_mutex[idx]);

    crypto_kapi_func_exit();
    return ret;
}
CRYPTO_EXPORT_SYMBOL(kapi_cipher_hash_destroy);

td_s32 kapi_cipher_pbkdf2(const crypto_kdf_pbkdf2_param *param, td_u8 *out, const td_u32 out_len)
{
    td_s32 ret = TD_SUCCESS;
    crypto_kapi_func_enter();

    kapi_hash_lock();
    ret = drv_cipher_pbkdf2(param, out, out_len);
    kapi_hash_unlock();
    crypto_kapi_func_exit();
    return ret;
}
CRYPTO_EXPORT_SYMBOL(kapi_cipher_pbkdf2);