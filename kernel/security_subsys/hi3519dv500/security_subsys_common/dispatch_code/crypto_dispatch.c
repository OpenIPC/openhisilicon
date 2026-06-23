/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "crypto_dispatch.h"
#include "crypto_ioctl_cmd.h"
#include "kapi_hash.h"
#include "kapi_trng.h"
#include "kapi_symc.h"
#include "kapi_pke.h"

#define SYMC_COMPAT_ERRNO(err_code)     DISPATCH_COMPAT_ERRNO(ERROR_MODULE_SYMC, err_code)
#define HASH_COMPAT_ERRNO(err_code)     DISPATCH_COMPAT_ERRNO(ERROR_MODULE_HASH, err_code)
#define PKE_COMPAT_ERRNO(err_code)      DISPATCH_COMPAT_ERRNO(ERROR_MODULE_PKE, err_code)
#define TRNG_COMPAT_ERRNO(err_code)     DISPATCH_COMPAT_ERRNO(ERROR_MODULE_TRNG, err_code)

#if defined(CONFIG_CRYPTO_DISPATCH_CMD_CHK_ENABLE)
#define dispatch_symc_cmd_chk(expected_cmd)   \
    crypto_chk_return(cmd != (expected_cmd), SYMC_COMPAT_ERRNO(ERROR_CMD_DISMATCH), \
        "cmd is dismatched, real cmd is 0x%x, expected_cmd is 0x%x\n", cmd, (td_u32)(expected_cmd))
#define dispatch_hash_cmd_chk(expected_cmd)   \
    crypto_chk_return(cmd != (expected_cmd), HASH_COMPAT_ERRNO(ERROR_CMD_DISMATCH), \
        "cmd is dismatched, real cmd is 0x%x, expected_cmd is 0x%x\n", cmd, (td_u32)(expected_cmd))
#define dispatch_pke_cmd_chk(expected_cmd)   \
    crypto_chk_return(cmd != (expected_cmd), PKE_COMPAT_ERRNO(ERROR_CMD_DISMATCH),  \
        "cmd is dismatched, real cmd is 0x%x, expected_cmd is 0x%x\n", cmd, (td_u32)(expected_cmd))
#define dispatch_trng_cmd_chk(expected_cmd)   \
    crypto_chk_return(cmd != (expected_cmd), TRNG_COMPAT_ERRNO(ERROR_CMD_DISMATCH), \
        "cmd is dismatched, real cmd is 0x%x, expected_cmd is 0x%x\n", cmd, (td_u32)(expected_cmd))
#else
#define dispatch_symc_cmd_chk(expected_cmd)
#define dispatch_hash_cmd_chk(expected_cmd)
#define dispatch_pke_cmd_chk(expected_cmd)
#define dispatch_trng_cmd_chk(expected_cmd)
#endif

#define symc_null_ptr_chk(ptr)   \
    crypto_chk_return((ptr) == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), #ptr" is NULL\n")
#define hash_null_ptr_chk(ptr)   \
    crypto_chk_return((ptr) == TD_NULL, HASH_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), #ptr" is NULL\n")
#define pke_null_ptr_chk(ptr)   \
    crypto_chk_return((ptr) == TD_NULL, PKE_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), #ptr" is NULL\n")
#define trng_null_ptr_chk(ptr)   \
    crypto_chk_return((ptr) == TD_NULL, TRNG_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), #ptr" is NULL\n")

#define CRYPTO_MAX_DATA_LEN                 (64 * 1024)

/* Notice: For Sensitive Information, You Should Clean it Before Free. */
#define crypto_chk_free_with_clean(ptr, length, free_func) do {                \
    if ((ptr) != NULL) {                                      \
        (td_void)memset_s(ptr, length, 0, length);          \
        free_func(ptr);                                     \
        ptr = NULL;                                         \
    }                                                       \
} while (0)

#define crypto_chk_crypto_free_with_clean(ptr, length)  crypto_chk_free_with_clean(ptr, length, crypto_free)

static td_s32 dispatch_hash_init(td_u32 cmd, td_void *argp, td_void *private_data)
{
    crypto_unused(argp);
    crypto_unused(private_data);
    dispatch_hash_cmd_chk(CRYPTO_CMD_HASH_INIT);
    return kapi_cipher_hash_init();
}

static td_s32 dispatch_hash_deinit(td_u32 cmd, td_void *argp, td_void *private_data)
{
    crypto_unused(argp);
    crypto_unused(private_data);
    dispatch_hash_cmd_chk(CRYPTO_CMD_HASH_DEINIT);
    return kapi_cipher_hash_deinit();
}

static td_s32 dispatch_hash_start(td_u32 cmd, td_void *argp, td_void *private_data)
{
    td_s32 ret = CRYPTO_SUCCESS;
    hash_start_ctl_t *start_ctl = (hash_start_ctl_t *)argp;
    crypto_hash_attr hash_attr = {
        .hash_type = start_ctl->hash_type,
        .is_long_term = start_ctl->is_long_term,
        .is_keyslot = start_ctl->is_keyslot,
        .key_len = start_ctl->key_len,
        .drv_keyslot_handle = start_ctl->drv_keyslot_handle,
    };
    td_u8 *hmac_key = TD_NULL;

    crypto_dispatch_func_enter();
    dispatch_hash_cmd_chk(CRYPTO_CMD_HASH_START);
    crypto_unused(private_data);

    if ((hash_attr.is_keyslot == TD_FALSE) && (CRYPTO_HASH_IS_HMAC(hash_attr.hash_type) == TD_TRUE) &&
        (hash_attr.key_len != 0)) {
        hash_null_ptr_chk(start_ctl->key.p);
        crypto_chk_return(hash_attr.key_len > CRYPTO_HASH_KEY_MAX_LENGTH, HASH_COMPAT_ERRNO(ERROR_INVALID_PARAM),
            "key length is too long\n");
        hmac_key = crypto_malloc(hash_attr.key_len);
        crypto_chk_return(hmac_key == TD_NULL, HASH_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");

        ret = crypto_copy_from_user(hmac_key, hash_attr.key_len, start_ctl->key.p, start_ctl->key_len);
        crypto_chk_goto_with_ret(ret != CRYPTO_SUCCESS, exit_clean, HASH_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
            "crypto_copy_from_user failed\n");
    }
    hash_attr.key = hmac_key;

    ret = kapi_cipher_hash_start(&start_ctl->kapi_hash_handle, &hash_attr);
    if (ret != CRYPTO_SUCCESS) {
        crypto_log_err("kapi_cipher_hash_start failed\n");
        goto exit_clean;
    }

exit_clean:
    if (hmac_key != TD_NULL) {
        (td_void)memset_s(hmac_key, start_ctl->key_len, 0, start_ctl->key_len);
        crypto_free(hmac_key);
    }

    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_hash_update(td_u32 cmd, td_void *argp, td_void *private_data)
{
    td_s32 ret = CRYPTO_SUCCESS;
    crypto_buf_attr src_buf = {0};
    hash_update_ctl_t *update_ctl = (hash_update_ctl_t *)argp;
    td_u8 *data_buffer = TD_NULL;
    td_u32 left = update_ctl->len;
    td_u32 malloc_size = 0;
    td_u32 processing_len = 0;
    td_u32 processed_len = 0;

    crypto_dispatch_func_enter();
    crypto_unused(private_data);
    dispatch_hash_cmd_chk(CRYPTO_CMD_HASH_UPDATE);
    hash_null_ptr_chk(update_ctl->src_buf.p);

    crypto_chk_return(update_ctl->len == 0 || update_ctl->len > CRYPTO_HASH_UPDATE_MAX_LEN,
        HASH_COMPAT_ERRNO(ERROR_INVALID_PARAM), "len is invalid\n");

    malloc_size = crypto_min(CRYPTO_MAX_DATA_LEN, left);
    data_buffer = crypto_malloc(malloc_size);
    crypto_chk_return(data_buffer == TD_NULL, HASH_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");
    src_buf.virt_addr = data_buffer;

    while (left > 0) {
        processing_len = crypto_min(CRYPTO_MAX_DATA_LEN, left);
        ret = crypto_copy_from_user(data_buffer, malloc_size, (td_u8 *)update_ctl->src_buf.p + processed_len,
            processing_len);
        crypto_chk_goto_with_ret(ret != CRYPTO_SUCCESS, exit_clean, HASH_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
            "crypto_copy_from_user failed\n");

        ret = kapi_cipher_hash_update(update_ctl->kapi_hash_handle, &src_buf, processing_len);
        crypto_chk_goto(ret != CRYPTO_SUCCESS, exit_clean, "kapi_cipher_hash_update failed\n");
        left -= processing_len;
        processed_len += processing_len;
    }

exit_clean:
    (td_void)memset_s(data_buffer, malloc_size, 0, malloc_size);
    crypto_free(data_buffer);
    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_hash_finish(td_u32 cmd, td_void *argp, td_void *private_data)
{
    td_s32 ret = CRYPTO_SUCCESS;
    hash_finish_ctl_t *finish_ctl = (hash_finish_ctl_t *)argp;
    td_u8 out_hash[CRYPTO_HASH_MAX_LEN] = {0};
    td_u32 out_len = CRYPTO_HASH_MAX_LEN;

    crypto_dispatch_func_enter();
    crypto_unused(private_data);
    dispatch_hash_cmd_chk(CRYPTO_CMD_HASH_FINISH);
    hash_null_ptr_chk(finish_ctl->out.p);

    ret = kapi_cipher_hash_finish(finish_ctl->kapi_hash_handle, out_hash, &out_len);
    crypto_chk_return(ret != CRYPTO_SUCCESS, ret, "kapi_cipher_hash_finish failed\n");

    ret = crypto_copy_to_user(finish_ctl->out.p, finish_ctl->out_len, out_hash, out_len);
    crypto_chk_goto_with_ret(ret != CRYPTO_SUCCESS, exit_clean, HASH_COMPAT_ERRNO(ERROR_COPY_TO_USER),
        "crypto_copy_to_user failed\n");
    finish_ctl->out_len = out_len;

exit_clean:
    (td_void)memset_s(out_hash, sizeof(out_hash), 0, sizeof(out_hash));
    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_hash_get(td_u32 cmd, td_void *argp, td_void *private_data)
{
    td_s32 ret = CRYPTO_SUCCESS;
    hash_clone_ctl_t *clone_ctl = (hash_clone_ctl_t *)argp;
    crypto_hash_clone_ctx *clone_ctx = TD_NULL;

    crypto_unused(private_data);
    dispatch_hash_cmd_chk(CRYPTO_CMD_HASH_GET);
    hash_null_ptr_chk(clone_ctl->hash_clone_ctx.p);
    crypto_chk_return(clone_ctl->ctx_size != sizeof(crypto_hash_clone_ctx), HASH_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "clone_ctx's size is invalid\n");
    clone_ctx = (crypto_hash_clone_ctx *)crypto_malloc(sizeof(crypto_hash_clone_ctx));
    crypto_chk_return(clone_ctx == TD_NULL, HASH_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");

    ret = kapi_cipher_hash_get(clone_ctl->kapi_hash_handle, clone_ctx);
    crypto_chk_goto(ret != CRYPTO_SUCCESS, exit_clean, "kapi_cipher_hash_get failed\n");

    ret = crypto_copy_to_user(clone_ctl->hash_clone_ctx.p, clone_ctl->ctx_size,
        clone_ctx, sizeof(crypto_hash_clone_ctx));
    crypto_chk_goto_with_ret(ret != CRYPTO_SUCCESS, exit_clean, HASH_COMPAT_ERRNO(ERROR_COPY_TO_USER),
        "crypto_copy_to_user failed\n");

exit_clean:
    (td_void)memset_s(clone_ctx, sizeof(crypto_hash_clone_ctx), 0, sizeof(crypto_hash_clone_ctx));
    crypto_free(clone_ctx);

    return ret;
}

static td_s32 dispatch_hash_set(td_u32 cmd, td_void *argp, td_void *private_data)
{
    td_s32 ret = CRYPTO_SUCCESS;
    hash_clone_ctl_t *clone_ctl = (hash_clone_ctl_t *)argp;
    crypto_hash_clone_ctx *clone_ctx = TD_NULL;
    crypto_unused(private_data);
    dispatch_hash_cmd_chk(CRYPTO_CMD_HASH_SET);

    hash_null_ptr_chk(clone_ctl->hash_clone_ctx.p);
    crypto_chk_return(clone_ctl->ctx_size != sizeof(crypto_hash_clone_ctx), HASH_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "clone_ctx's size is invalid\n");

    clone_ctx = (crypto_hash_clone_ctx *)crypto_malloc(sizeof(crypto_hash_clone_ctx));
    crypto_chk_return(clone_ctx == TD_NULL, HASH_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");

    ret = crypto_copy_from_user(clone_ctx, sizeof(crypto_hash_clone_ctx),
        clone_ctl->hash_clone_ctx.p, clone_ctl->ctx_size);
    crypto_chk_goto_with_ret(ret != CRYPTO_SUCCESS, exit_clean, HASH_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed\n");

    ret = kapi_cipher_hash_set(clone_ctl->kapi_hash_handle, clone_ctx);
    crypto_chk_goto(ret != CRYPTO_SUCCESS, exit_clean, "kapi_cipher_hash_set failed\n");

exit_clean:
    (td_void)memset_s(clone_ctx, sizeof(crypto_hash_clone_ctx), 0, sizeof(crypto_hash_clone_ctx));
    crypto_free(clone_ctx);
    return ret;
}

static td_s32 dispatch_hash_destroy(td_u32 cmd, td_void *argp, td_void *private_data)
{
    td_s32 ret = CRYPTO_SUCCESS;
    hash_handle_ctl_t *handle_ctl = (hash_handle_ctl_t *)argp;
    crypto_unused(private_data);
    dispatch_hash_cmd_chk(CRYPTO_CMD_HASH_DESTROY);

    ret = kapi_cipher_hash_destroy(handle_ctl->kapi_hash_handle);
    if (ret != CRYPTO_SUCCESS) {
        crypto_log_err("kapi_cipher_hash_destroy failed, ret is 0x%x\n", ret);
        return ret;
    }

    return ret;
}

static td_s32 dispatch_cipher_pbkdf2(td_u32 cmd, td_void *argp, td_void *private_data)
{
    td_s32 ret = CRYPTO_SUCCESS;
    pbkdf2_ctl_t *pbkdf2_ctl = (pbkdf2_ctl_t *)argp;
    td_u8 *password = TD_NULL;
    td_u8 *salt = TD_NULL;
    td_u8 *out = TD_NULL;
    td_u8 *buffer = TD_NULL;
    crypto_kdf_pbkdf2_param param = {0};
    td_u32 total_len = 0;
    crypto_unused(private_data);
    dispatch_hash_cmd_chk(CRYPTO_CMD_PBKDF2);

    hash_null_ptr_chk(pbkdf2_ctl->out.p);
    crypto_chk_return(pbkdf2_ctl->out_len == 0 || pbkdf2_ctl->out_len > CRYPTO_PBKDF2_OUT_MAX_LENGTH,
        HASH_COMPAT_ERRNO(ERROR_INVALID_PARAM), "out_len is Invalid\n");
    crypto_chk_return(pbkdf2_ctl->plen > CRYPTO_PBKDF2_PASS_MAX_LENGTH, HASH_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "plen is too long!\n");
    crypto_chk_return(pbkdf2_ctl->slen > CRYPTO_PBKDF2_SALT_MAX_LENGTH, HASH_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "slen is too long!\n");

    total_len = pbkdf2_ctl->plen + pbkdf2_ctl->slen + pbkdf2_ctl->out_len;
    buffer = crypto_malloc(total_len);
    crypto_chk_return(buffer == TD_NULL, HASH_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");

    password = buffer;
    if (pbkdf2_ctl->plen != 0) {
        crypto_chk_goto_with_ret(pbkdf2_ctl->password.p == TD_NULL, exit_free, HASH_COMPAT_ERRNO(ERROR_PARAM_IS_NULL),
            "pbkdf2_ctl->password.p is NULL\n");
        ret = crypto_copy_from_user(password, total_len, pbkdf2_ctl->password.p, pbkdf2_ctl->plen);
        crypto_chk_goto_with_ret(ret != CRYPTO_SUCCESS, exit_free, HASH_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
            "crypto_copy_from_user failed\n");
    }

    salt = password + pbkdf2_ctl->plen;
    if (pbkdf2_ctl->slen != 0) {
        crypto_chk_goto_with_ret(pbkdf2_ctl->salt.p == TD_NULL, exit_free, HASH_COMPAT_ERRNO(ERROR_PARAM_IS_NULL),
            "pbkdf2_ctl->salt.p is NULL\n");
        ret = crypto_copy_from_user(salt, total_len - pbkdf2_ctl->plen, pbkdf2_ctl->salt.p, pbkdf2_ctl->slen);
        crypto_chk_goto_with_ret(ret != CRYPTO_SUCCESS, exit_free, HASH_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
            "crypto_copy_from_user failed\n");
    }

    out = salt + pbkdf2_ctl->slen;
    param.hash_type = pbkdf2_ctl->hash_type;
    param.password = (pbkdf2_ctl->plen == 0) ? TD_NULL : password;
    param.plen = pbkdf2_ctl->plen;
    param.salt = (pbkdf2_ctl->slen == 0) ? TD_NULL : salt;
    param.slen = pbkdf2_ctl->slen;
    param.count = pbkdf2_ctl->count;

    ret = kapi_cipher_pbkdf2(&param, out, pbkdf2_ctl->out_len);
    crypto_chk_goto(ret != CRYPTO_SUCCESS, exit_free, "kapi_cipher_pbkdf2 failed\n");

    ret = crypto_copy_to_user(pbkdf2_ctl->out.p, pbkdf2_ctl->out_len, out, pbkdf2_ctl->out_len);
    crypto_chk_goto_with_ret(ret != CRYPTO_SUCCESS, exit_free, HASH_COMPAT_ERRNO(ERROR_COPY_TO_USER),
        "crypto_copy_to_user failed\n");

exit_free:
    (td_void)memset_s(buffer, total_len, 0, total_len);
    crypto_free(buffer);
    return ret;
}

static td_s32 dispatch_trng_get_random(td_u32 cmd, td_void *argp, td_void *private_data)
{
    td_s32 ret = CRYPTO_SUCCESS;
    trng_ctl_t *trng_ctl = (trng_ctl_t *)argp;
    crypto_unused(private_data);
    dispatch_trng_cmd_chk(CRYPTO_CMD_TRNG_GET_RANDOM);
    ret = kapi_cipher_trng_get_random(&trng_ctl->randnum);
    if (ret != CRYPTO_SUCCESS) {
        crypto_log_err("kapi_cipher_trng_get_random failed\n");
        return ret;
    }
    return ret;
}

static td_s32 dispatch_trng_get_multi_random(td_u32 cmd, td_void *argp, td_void *private_data)
{
    td_s32 ret = CRYPTO_SUCCESS;
    trng_multi_ctl_t *trng_multi_ctl = (trng_multi_ctl_t *)argp;
    td_u32 size = 0;
    td_u8 *randnum;
    crypto_unused(private_data);
    dispatch_trng_cmd_chk(CRYPTO_CMD_TRNG_GET_MULTI_RANDOM);

    trng_null_ptr_chk(trng_multi_ctl->randnum.p);
    size = trng_multi_ctl->size;
    crypto_chk_return(size == 0 || size > CRYPTO_MULTI_RANDOM_MAX_LENGTH, TRNG_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "size is invalid!\n");
    randnum = crypto_malloc(size);
    crypto_chk_return(randnum == TD_NULL, TRNG_COMPAT_ERRNO(ERROR_INVALID_PARAM), "crypto_malloc failed\n");
    ret = kapi_cipher_trng_get_multi_random(size, randnum);
    if (ret != CRYPTO_SUCCESS) {
        crypto_log_err("kapi_cipher_trng_get_multi_random failed\n");
        goto exit_free;
    }
    ret = crypto_copy_to_user(trng_multi_ctl->randnum.p, size, randnum, size);
    crypto_chk_goto_with_ret(ret != CRYPTO_SUCCESS, exit_free, TRNG_COMPAT_ERRNO(ERROR_COPY_TO_USER),
        "crypto_copy_to_user failed\n");
exit_free:
    if (randnum != TD_NULL) {
        (void)memset_s(randnum, size, 0, size);
        crypto_free(randnum);
    }
    return ret;
}

static td_s32 dispatch_symc_init(td_u32 cmd, td_void *argp, td_void *private_data)
{
    crypto_unused(argp);
    crypto_unused(private_data);
    dispatch_symc_cmd_chk(CRYPTO_CMD_SYMC_INIT);
    return kapi_cipher_symc_init();
}

static td_s32 dispatch_symc_deinit(td_u32 cmd, td_void *argp, td_void *private_data)
{
    crypto_unused(argp);
    crypto_unused(private_data);
    dispatch_symc_cmd_chk(CRYPTO_CMD_SYMC_DEINIT);
    return kapi_cipher_symc_deinit();
}

static td_s32 dispatch_symc_create(td_u32 cmd, td_void *argp, td_void *private_data)
{
    td_s32 ret;
    symc_create_t *symc_create = (symc_create_t *)argp;
    crypto_dispatch_func_enter();
    crypto_unused(private_data);
    dispatch_symc_cmd_chk(CRYPTO_CMD_SYMC_CREATE);

    ret = kapi_cipher_symc_create(&symc_create->symc_handle, &symc_create->symc_attr);
    if (ret != CRYPTO_SUCCESS) {
        crypto_log_err("kapi_cipher_symc_create failed\n");
        return ret;
    }
    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_symc_destroy(td_u32 cmd, td_void *argp, td_void *private_data)
{
    td_s32 ret;
    symc_destroy_t *symc_destroy = (symc_destroy_t *)argp;
    crypto_dispatch_func_enter();
    crypto_unused(private_data);
    dispatch_symc_cmd_chk(CRYPTO_CMD_SYMC_DESTROY);

    ret = kapi_cipher_symc_destroy(symc_destroy->symc_handle);
    if (ret != CRYPTO_SUCCESS) {
        crypto_log_err("kapi_cipher_symc_destroy failed\n");
        return ret;
    }
    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_symc_set_config(td_u32 cmd, td_void *argp, td_void *private_data)
{
    td_s32 ret;
    symc_config_t *symc_config = (symc_config_t *)argp;
    crypto_symc_ctrl_t symc_ctrl = {0};
    crypto_symc_config_aes_ccm_gcm ccm_gcm_config;
    td_u8 *aad_buf = TD_NULL;
    td_u32 aad_len = symc_config->aad_len;

    crypto_dispatch_func_enter();
    crypto_unused(private_data);
    dispatch_symc_cmd_chk(CRYPTO_CMD_SYMC_SET_CONFIG);

    (td_void)memset_s(&ccm_gcm_config, sizeof(ccm_gcm_config), 0, sizeof(ccm_gcm_config));
    symc_ctrl.symc_alg = symc_config->symc_alg;
    symc_ctrl.work_mode = symc_config->work_mode;
    symc_ctrl.symc_bit_width = symc_config->symc_bit_width;
    symc_ctrl.symc_key_length = symc_config->symc_key_length;
    symc_ctrl.iv_change_flag = symc_config->iv_change_flag;
    symc_ctrl.iv_length = symc_config->iv_length;
    ret = memcpy_s(symc_ctrl.iv, sizeof(symc_ctrl.iv), symc_config->iv, sizeof(symc_config->iv));
    crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    if (symc_config->work_mode == CRYPTO_SYMC_WORK_MODE_GCM || symc_config->work_mode == CRYPTO_SYMC_WORK_MODE_CCM) {
        crypto_chk_return(aad_len > CRYPTO_SYMC_AAD_MAX_SIZE, SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM),
            "aad_len is too long\n");
        ccm_gcm_config.aad_len = aad_len;
        ccm_gcm_config.data_len = symc_config->data_len;
        ccm_gcm_config.tag_len = symc_config->tag_len;
        if (aad_len != 0) {
            symc_null_ptr_chk(symc_config->aad.p);
            aad_buf = crypto_malloc(aad_len);
            crypto_chk_return(aad_buf == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");

            ret = crypto_copy_from_user(aad_buf, aad_len, symc_config->aad.p, aad_len);
            crypto_chk_goto_with_ret(ret != TD_SUCCESS, exit_free, SYMC_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
                "crypto_copy_from_user failed\n");
            ccm_gcm_config.aad_buf.virt_addr = aad_buf;
        }
        symc_ctrl.param = (td_void *)&ccm_gcm_config;
    }

    ret = kapi_cipher_symc_set_config(symc_config->symc_handle, &symc_ctrl);
    crypto_chk_goto(ret != TD_SUCCESS, exit_free, "kapi_cipher_symc_set_config failed\n");

exit_free:
    if (aad_buf != TD_NULL) {
        (td_void)memset_s(aad_buf, aad_len, 0, aad_len);
        crypto_free(aad_buf);
    }
    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_symc_get_config(td_u32 cmd, td_void *argp, td_void *private_data)
{
    td_s32 ret;
    symc_config_t *symc_config = (symc_config_t *)argp;
    crypto_symc_ctrl_t symc_ctrl = {0};
    crypto_symc_config_aes_ccm_gcm ccm_gcm_config;

    crypto_dispatch_func_enter();
    crypto_unused(private_data);
    dispatch_symc_cmd_chk(CRYPTO_CMD_SYMC_GET_CONFIG);

    (td_void)memset_s(&ccm_gcm_config, sizeof(ccm_gcm_config), 0, sizeof(ccm_gcm_config));
    symc_ctrl.param = (td_void *)&ccm_gcm_config;

    ret = kapi_cipher_symc_get_config(symc_config->symc_handle, &symc_ctrl);
    crypto_chk_return(ret != TD_SUCCESS, ret, "kapi_cipher_symc_get_config failed\n");

    symc_config->symc_alg = symc_ctrl.symc_alg;
    symc_config->work_mode = symc_ctrl.work_mode;
    symc_config->symc_bit_width = symc_ctrl.symc_bit_width;
    symc_config->symc_key_length = symc_ctrl.symc_key_length;
    symc_config->iv_change_flag = symc_ctrl.iv_change_flag;
    symc_config->iv_length = symc_ctrl.iv_length;
    ret = memcpy_s(symc_config->iv, sizeof(symc_config->iv), symc_ctrl.iv, sizeof(symc_ctrl.iv));
    crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    if (symc_config->work_mode == CRYPTO_SYMC_WORK_MODE_GCM || symc_config->work_mode == CRYPTO_SYMC_WORK_MODE_CCM) {
        symc_config->aad_len = ccm_gcm_config.aad_len;
        symc_config->data_len = ccm_gcm_config.data_len;
        symc_config->tag_len = ccm_gcm_config.tag_len;
    }

    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_symc_attach(td_u32 cmd, td_void *argp, td_void *private_data)
{
    td_s32 ret;
    symc_attach_t *symc_attch = (symc_attach_t *)argp;
    crypto_dispatch_func_enter();
    crypto_unused(private_data);
    dispatch_symc_cmd_chk(CRYPTO_CMD_SYMC_ATTACH);

    ret = kapi_cipher_symc_attach(symc_attch->symc_handle, symc_attch->keyslot_handle);
    if (ret != CRYPTO_SUCCESS) {
        crypto_log_err("kapi_cipher_symc_attach failed\n");
        return ret;
    }
    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_symc_crypto(td_u32 cmd, td_void *argp, td_void *private_data, td_u32 crypto_type)
{
    td_s32 ret;
    symc_crypto_t *symc_crypto = (symc_crypto_t *)argp;
    crypto_buf_attr src_buf = {0};
    crypto_buf_attr dst_buf = {0};
    td_u32 length = symc_crypto->length;
    crypto_unused(cmd);
    crypto_unused(private_data);

    src_buf.kapi_mem_handle =
        (td_void *)crypto_osal_mem_handle_get((unsigned long)symc_crypto->src_mem_handle, SOC_ID_CIPHER);
    crypto_chk_goto_with_ret(src_buf.kapi_mem_handle == TD_NULL, mem_handle_put_exit,
        SYMC_COMPAT_ERRNO(ERROR_MEM_HANDLE_GET), "crypto_osal_mem_handle_get failed\n");
    src_buf.phys_addr = symc_crypto->src_phys_addr;
    src_buf.buf_sec = symc_crypto->src_buf_sec;
    src_buf.addr_offset = symc_crypto->src_addr_offset;
    dst_buf.kapi_mem_handle =
        (td_void *)crypto_osal_mem_handle_get((unsigned long)symc_crypto->dst_mem_handle, SOC_ID_CIPHER);
    crypto_chk_goto_with_ret(dst_buf.kapi_mem_handle == TD_NULL, mem_handle_put_exit,
        SYMC_COMPAT_ERRNO(ERROR_MEM_HANDLE_GET), "crypto_osal_mem_handle_get failed\n");
    dst_buf.phys_addr = symc_crypto->dst_phys_addr;
    dst_buf.buf_sec = symc_crypto->dst_buf_sec;
    dst_buf.addr_offset = symc_crypto->dst_addr_offset;

    switch (crypto_type) {
        case CRYPTO_TYPE_ENCRYPT:
            ret = kapi_cipher_symc_encrypt(symc_crypto->symc_handle, &src_buf, &dst_buf, length);
            break;
        case CRYPTO_TYPE_DECRYPT:
            ret = kapi_cipher_symc_decrypt(symc_crypto->symc_handle, &src_buf, &dst_buf, length);
            break;
        default:
            crypto_log_err("Invalid Crypto Type!\n");
            ret = SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM);
    }

mem_handle_put_exit:
    crypto_osal_mem_handle_put(src_buf.kapi_mem_handle, SOC_ID_CIPHER);
    crypto_osal_mem_handle_put(dst_buf.kapi_mem_handle, SOC_ID_CIPHER);
    return ret;
}

static td_s32 dispatch_symc_encrypt(td_u32 cmd, td_void *argp, td_void *private_data)
{
    td_s32 ret;
    crypto_dispatch_func_enter();
    dispatch_symc_cmd_chk(CRYPTO_CMD_SYMC_ENCRYPT);
    ret = dispatch_symc_crypto(cmd, argp, private_data, CRYPTO_TYPE_ENCRYPT);
    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_symc_decrypt(td_u32 cmd, td_void *argp, td_void *private_data)
{
    td_s32 ret;
    crypto_dispatch_func_enter();
    dispatch_symc_cmd_chk(CRYPTO_CMD_SYMC_DECRYPT);
    ret = dispatch_symc_crypto(cmd, argp, private_data, CRYPTO_TYPE_DECRYPT);
    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 priv_dispatch_symc_multi_config_ctrl(const crypto_symc_multi_t *symc_multi, crypto_symc_ctrl_t *symc_ctrl)
{
    td_s32 ret;

    symc_ctrl->symc_alg = symc_multi->symc_alg;
    symc_ctrl->work_mode = symc_multi->work_mode;
    symc_ctrl->symc_bit_width = symc_multi->symc_bit_width;
    symc_ctrl->symc_key_length = symc_multi->symc_key_length;
    symc_ctrl->iv_change_flag = symc_multi->iv_change_flag;
    symc_ctrl->iv_length = symc_multi->iv_length;
    ret = memcpy_s(symc_ctrl->iv, sizeof(symc_ctrl->iv), symc_multi->iv, sizeof(symc_multi->iv));
    crypto_chk_return(ret != EOK, SYMC_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    return ret;
}

static td_void priv_dispatch_symc_multi_handle_put(const crypto_symc_pack *src_buf_pack,
    const crypto_symc_pack *dst_buf_pack, td_u32 pack_num)
{
    td_u32 i;
    for (i = 0; i < pack_num; i++) {
        if (src_buf_pack[i].buf_attr.kapi_mem_handle != TD_NULL) {
            crypto_osal_mem_handle_put(src_buf_pack[i].buf_attr.kapi_mem_handle, SOC_ID_CIPHER);
        }
        if (dst_buf_pack[i].buf_attr.kapi_mem_handle != TD_NULL) {
            crypto_osal_mem_handle_put(dst_buf_pack[i].buf_attr.kapi_mem_handle, SOC_ID_CIPHER);
        }
    }
}

static td_s32 priv_dispatch_symc_multi_config_pack(const crypto_symc_multi_t *symc_multi,
    crypto_symc_pack *src_buf_pack, crypto_symc_pack *dst_buf_pack,
    crypto_mem_pack *src_buf_mem_pack, crypto_mem_pack *dst_buf_mem_pack)
{
    td_u32 ret;
    td_u32 i;
    td_u32 pack_num = symc_multi->pack_num;
    td_u32 mem_size = sizeof(crypto_mem_pack) * pack_num;

    ret = crypto_copy_from_user(src_buf_mem_pack, mem_size, symc_multi->src_pack_addr.p, mem_size);
    crypto_chk_return(ret != TD_SUCCESS, SYMC_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_from_user(dst_buf_mem_pack, mem_size, symc_multi->dst_pack_addr.p, mem_size);
    crypto_chk_return(ret != TD_SUCCESS, SYMC_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    for (i = 0; i < pack_num; i++) {
        /* transfer uapi mem handle to kapi mem handle. */
        src_buf_pack[i].buf_attr.kapi_mem_handle = (td_void *)crypto_osal_mem_handle_get(
            (unsigned long)src_buf_mem_pack[i].uapi_mem_handle, SOC_ID_CIPHER);
        crypto_chk_goto_with_ret(src_buf_pack[i].buf_attr.kapi_mem_handle == TD_NULL, error_mem_handle_put,
            SYMC_COMPAT_ERRNO(ERROR_MEM_HANDLE_GET), "crypto_osal_mem_handle_get failed\n");
        src_buf_pack[i].length = src_buf_mem_pack[i].length;
        src_buf_pack[i].buf_attr.phys_addr = src_buf_mem_pack[i].phys_addr;
        src_buf_pack[i].buf_attr.buf_sec = src_buf_mem_pack[i].buf_sec;
        dst_buf_pack[i].buf_attr.kapi_mem_handle = (td_void *)crypto_osal_mem_handle_get(
            (unsigned long)dst_buf_mem_pack[i].uapi_mem_handle, SOC_ID_CIPHER);
        crypto_chk_goto_with_ret(dst_buf_pack[i].buf_attr.kapi_mem_handle == TD_NULL, error_mem_handle_put,
            SYMC_COMPAT_ERRNO(ERROR_MEM_HANDLE_GET), "crypto_osal_mem_handle_get failed\n");
        dst_buf_pack[i].length = dst_buf_mem_pack[i].length;
        dst_buf_pack[i].buf_attr.phys_addr = dst_buf_mem_pack[i].phys_addr;
        dst_buf_pack[i].buf_attr.buf_sec = dst_buf_mem_pack[i].buf_sec;
    }

    return TD_SUCCESS;

error_mem_handle_put:
    priv_dispatch_symc_multi_handle_put(src_buf_pack, dst_buf_pack, pack_num);
    return ret;
}

static td_s32 dispatch_symc_crypto_multi(td_u32 cmd, td_void *argp, td_void *private_data, td_u32 crypto_type)
{
    td_s32 ret;
    td_u8 *buffer = TD_NULL;
    crypto_symc_multi_t *symc_multi = (crypto_symc_multi_t *)argp;
    td_u32 pack_num = symc_multi->pack_num;
    crypto_mem_pack *src_buf_mem_pack = TD_NULL;
    crypto_mem_pack *dst_buf_mem_pack = TD_NULL;
    crypto_symc_pack *src_buf_pack = TD_NULL;
    crypto_symc_pack *dst_buf_pack = TD_NULL;
    td_u32 pack_size = (sizeof(crypto_mem_pack) + sizeof(crypto_symc_pack)) * pack_num;
    crypto_symc_ctrl_t symc_ctrl = {0};
    crypto_unused(cmd);
    crypto_unused(private_data);

    symc_null_ptr_chk(symc_multi->src_pack_addr.p);
    symc_null_ptr_chk(symc_multi->dst_pack_addr.p);
    crypto_chk_return(pack_num > CRYPTO_SYMC_MULTI_PACK_MAX_SIZE, SYMC_COMPAT_ERRNO(ERROR_PARAM_IS_NULL),
        "pack_num is too large\n");

    ret = priv_dispatch_symc_multi_config_ctrl(symc_multi, &symc_ctrl);
    crypto_chk_return(ret != TD_SUCCESS, ret, "priv_dispatch_symc_multi_config failed\n");

    buffer = crypto_malloc(pack_size * 2);    /* 2: For Both src and dst. */
    crypto_chk_return(buffer == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");
    (td_void)memset_s(buffer, pack_size * 2, 0, pack_size * 2);   /* 2: For Both src and dst. */

    src_buf_mem_pack = (void *)buffer;
    dst_buf_mem_pack = (void *)((td_u8 *)src_buf_mem_pack + sizeof(crypto_mem_pack) * pack_num);
    src_buf_pack = (void *)((td_u8 *)dst_buf_mem_pack + sizeof(crypto_mem_pack) * pack_num);
    dst_buf_pack = (void *)((td_u8 *)src_buf_pack + sizeof(crypto_symc_pack) * pack_num);

    ret = priv_dispatch_symc_multi_config_pack(symc_multi, src_buf_pack, dst_buf_pack,
        src_buf_mem_pack, dst_buf_mem_pack);
    crypto_chk_goto(ret != TD_SUCCESS, exit_free, "priv_dispatch_symc_multi_config_pack failed, ret is 0x%x\n", ret);

    switch (crypto_type) {
        case CRYPTO_TYPE_ENCRYPT:
            ret = kapi_cipher_symc_encrypt_multi(symc_multi->symc_handle, &symc_ctrl,
                src_buf_pack, dst_buf_pack, pack_num);
            break;
        case CRYPTO_TYPE_DECRYPT:
            ret = kapi_cipher_symc_decrypt_multi(symc_multi->symc_handle, &symc_ctrl,
                src_buf_pack, dst_buf_pack, pack_num);
            break;
        default:
            crypto_log_err("Invalid Crypto Type!\n");
            ret = SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM);
    }

    priv_dispatch_symc_multi_handle_put(src_buf_pack, dst_buf_pack, pack_num);
exit_free:
    (td_void)memset_s(buffer, pack_size * 2, 0, pack_size * 2);   /* 2: For Both src and dst. */
    crypto_free(buffer);
    return ret;
}

static td_s32 dispatch_symc_encrypt_multi(td_u32 cmd, td_void *argp, td_void *private_data)
{
    td_s32 ret;
    crypto_dispatch_func_enter();
    dispatch_symc_cmd_chk(CRYPTO_CMD_SYMC_ENCRYPT_MULTI);
    ret = dispatch_symc_crypto_multi(cmd, argp, private_data, CRYPTO_TYPE_ENCRYPT);
    crypto_chk_return(ret != TD_SUCCESS, ret, "dispatch_symc_crypto_multi for Encrypt failed\n");

    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_symc_decrypt_multi(td_u32 cmd, td_void *argp, td_void *private_data)
{
    td_s32 ret;
    crypto_dispatch_func_enter();
    dispatch_symc_cmd_chk(CRYPTO_CMD_SYMC_DECRYPT_MULTI);
    ret = dispatch_symc_crypto_multi(cmd, argp, private_data, CRYPTO_TYPE_DECRYPT);
    crypto_chk_return(ret != TD_SUCCESS, ret, "dispatch_symc_crypto_multi for Decrypt failed\n");

    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_symc_get_tag(td_u32 cmd, td_void *argp, td_void *private_data)
{
    td_s32 ret;
    symc_get_tag_t *symc_get_tag = (symc_get_tag_t *)argp;

    crypto_dispatch_func_enter();
    crypto_unused(private_data);
    dispatch_symc_cmd_chk(CRYPTO_CMD_SYMC_GET_TAG);

    ret = kapi_cipher_symc_get_tag(symc_get_tag->symc_handle, symc_get_tag->tag, symc_get_tag->tag_length);
    crypto_chk_return(ret != TD_SUCCESS, ret, "kapi_cipher_symc_get_tag failed\n");
    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_symc_mac_start(td_u32 cmd, td_void *argp, td_void *private_data)
{
    td_s32 ret;
    symc_mac_start_t *mac_start = (symc_mac_start_t *)argp;
    crypto_dispatch_func_enter();
    crypto_unused(private_data);
    dispatch_symc_cmd_chk(CRYPTO_CMD_SYMC_MAC_START);

    ret = kapi_cipher_mac_start(&mac_start->symc_handle, &mac_start->mac_attr);
    crypto_chk_return(ret != TD_SUCCESS, ret, "kapi_cipher_mac_start failed, ret is 0x%x\n", ret);

    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_symc_mac_update(td_u32 cmd, td_void *argp, td_void *private_data)
{
    td_s32 ret;
    symc_mac_update_t *mac_update = (symc_mac_update_t *)argp;
    td_u8 *data_buffer = TD_NULL;
    td_u32 left = mac_update->length;
    td_u32 malloc_size = 0;
    td_u32 processing_len = 0;
    td_u32 processed_len = 0;
    crypto_buf_attr src_buf = {0};
    crypto_dispatch_func_enter();
    crypto_unused(private_data);
    dispatch_symc_cmd_chk(CRYPTO_CMD_SYMC_MAC_UPDATE);
    symc_null_ptr_chk(mac_update->src_buf.p);
    crypto_chk_return(mac_update->length == 0 || mac_update->length > CRYPTO_SYMC_MAC_UPDATE_MAX_LEN,
        SYMC_COMPAT_ERRNO(ERROR_INVALID_PARAM), "length is Invalid\n");

    malloc_size = crypto_min(CRYPTO_MAX_DATA_LEN, left);
    data_buffer = crypto_malloc(malloc_size);
    src_buf.virt_addr = data_buffer;
    crypto_chk_return(data_buffer == TD_NULL, SYMC_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");

    while (left > 0) {
        processing_len = crypto_min(CRYPTO_MAX_DATA_LEN, left);
        ret = crypto_copy_from_user(data_buffer, malloc_size, (td_u8 *)mac_update->src_buf.p + processed_len,
            processing_len);
        crypto_chk_goto_with_ret(ret != TD_SUCCESS, exit_free, SYMC_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
            "crypto_copy_from_user failed, ret is 0x%x\n", ret);

        ret = kapi_cipher_mac_update(mac_update->symc_handle, &src_buf, processing_len);
        crypto_chk_goto(ret != TD_SUCCESS, exit_free, "kapi_cipher_mac_update failed, ret is 0x%x\n", ret);
        left -= processing_len;
        processed_len += processing_len;
    }

exit_free:
    (td_void)memset_s(data_buffer, malloc_size, 0, malloc_size);
    crypto_free(data_buffer);
    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_symc_mac_finish(td_u32 cmd, td_void *argp, td_void *private_data)
{
    td_s32 ret;
    symc_mac_finish_t *mac_finish = (symc_mac_finish_t *)argp;
    crypto_dispatch_func_enter();
    crypto_unused(private_data);
    dispatch_symc_cmd_chk(CRYPTO_CMD_SYMC_MAC_FINISH);
    mac_finish->mac_length = CRYPTO_AES_BLOCK_SIZE_IN_BYTES;
    ret = kapi_cipher_mac_finish(mac_finish->symc_handle, mac_finish->mac, &mac_finish->mac_length);
    crypto_chk_return(ret != TD_SUCCESS, ret, "kapi_cipher_mac_finish failed, ret is 0x%x\n", ret);

    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_pke_ecc_gen_key(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    pke_ecc_gen_key_ctl_t *gen_key_ctl = (pke_ecc_gen_key_ctl_t *)argp;
    drv_pke_ecc_curve_type curve_type = 0;
    drv_pke_data input_priv_key = { 0 };
    drv_pke_data output_priv_key = { 0 };
    drv_pke_ecc_point output_pub_key = { 0 };
    td_u32 klen = 0;
    crypto_dispatch_func_enter();
    dispatch_pke_cmd_chk(CRYPTO_CMD_PKE_ECC_GEN_KEY);

    crypto_chk_return(gen_key_ctl->curve_type >= DRV_PKE_ECC_TYPE_MAX, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "Invalid curve_type\n");

    crypto_unused(private_data);
    pke_null_ptr_chk(gen_key_ctl->output_priv_key.data.p);

    if (gen_key_ctl->curve_type != DRV_PKE_ECC_TYPE_RFC8032) {
        pke_null_ptr_chk(gen_key_ctl->output_pub_key.x.p);
    }
    if (gen_key_ctl->curve_type != DRV_PKE_ECC_TYPE_RFC7748) {
        pke_null_ptr_chk(gen_key_ctl->output_pub_key.y.p);
    }
    curve_type = gen_key_ctl->curve_type;
    klen = gen_key_ctl->output_priv_key.length;
    crypto_chk_return(klen < CRYPTO_PKE_ECC_KEY_MIN_SIZE || klen > CRYPTO_PKE_ECC_KEY_MAX_SIZE,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "klen is Invalid\n");

    crypto_chk_return(gen_key_ctl->output_pub_key.length != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "gen_key_ctl->output_pub_key.length is invalid\n");

    /* alloc memory. */
    if (gen_key_ctl->input_priv_key.length != 0) {
        pke_null_ptr_chk(gen_key_ctl->input_priv_key.data.p);
        crypto_chk_return(gen_key_ctl->input_priv_key.length != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
            "gen_key_ctl->input_priv_key.length is invalid\n");
        /* input_priv_key. */
        input_priv_key.data = crypto_malloc(klen);
        crypto_chk_goto_with_ret(input_priv_key.data == TD_NULL, free_exit,
            PKE_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");
        input_priv_key.length = klen;

        ret = crypto_copy_from_user(input_priv_key.data, klen, gen_key_ctl->input_priv_key.data.p, klen);
        crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit,
            PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER), "crypto_copy_from_user failed, ret is 0x%x\n", ret);
    }

    /* output_priv_key. */
    output_priv_key.data = crypto_malloc(klen);
    crypto_chk_goto_with_ret(output_priv_key.data == TD_NULL, free_exit,
        PKE_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");
    output_priv_key.length = klen;

    /* output_pub_key.  */
    if (gen_key_ctl->curve_type != DRV_PKE_ECC_TYPE_RFC8032) {
        output_pub_key.x = crypto_malloc(klen);
        crypto_chk_goto_with_ret(output_pub_key.x == TD_NULL, free_exit,
            PKE_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");
    }
    if (gen_key_ctl->curve_type != DRV_PKE_ECC_TYPE_RFC7748) {
        output_pub_key.y = crypto_malloc(klen);
        crypto_chk_goto_with_ret(output_pub_key.y == TD_NULL, free_exit,
            PKE_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");
    }
    output_pub_key.length = klen;

    if (gen_key_ctl->input_priv_key.length != 0) {
        ret = kapi_pke_ecc_gen_key(curve_type, &input_priv_key, &output_priv_key, &output_pub_key);
    } else {
        ret = kapi_pke_ecc_gen_key(curve_type, TD_NULL, &output_priv_key, &output_pub_key);
    }
    crypto_chk_goto(ret != TD_SUCCESS, free_exit, "kapi_pke_ecc_gen_key failed, ret is 0x%x\n", ret);
    /* copy to user. */
    ret = crypto_copy_to_user(gen_key_ctl->output_priv_key.data.p, klen, output_priv_key.data, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_TO_USER),
        "crypto_copy_to_user failed, ret is 0x%x\n", ret);
    if (gen_key_ctl->curve_type != DRV_PKE_ECC_TYPE_RFC8032) {
        ret = crypto_copy_to_user(gen_key_ctl->output_pub_key.x.p, klen, output_pub_key.x, klen);
        crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_TO_USER),
            "crypto_copy_to_user failed, ret is 0x%x\n", ret);
    }
    if (gen_key_ctl->curve_type != DRV_PKE_ECC_TYPE_RFC7748) {
        ret = crypto_copy_to_user(gen_key_ctl->output_pub_key.y.p, klen, output_pub_key.y, klen);
        crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_TO_USER),
            "crypto_copy_to_user failed, ret is 0x%x\n", ret);
    }

free_exit:
    if (gen_key_ctl->curve_type != DRV_PKE_ECC_TYPE_RFC7748) {
        crypto_chk_crypto_free_with_clean(output_pub_key.y, klen);
    }
    if (gen_key_ctl->curve_type != DRV_PKE_ECC_TYPE_RFC8032) {
        crypto_chk_crypto_free_with_clean(output_pub_key.x, klen);
    }
    crypto_chk_crypto_free_with_clean(output_priv_key.data, klen);
    crypto_chk_crypto_free_with_clean(input_priv_key.data, klen);
    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_pke_ecc_gen_ecdh_key(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    pke_ecc_gen_ecdh_key_ctl_t *gen_ecdh_key_ctl = (pke_ecc_gen_ecdh_key_ctl_t *)argp;
    drv_pke_ecc_curve_type curve_type = 0;
    drv_pke_data input_priv_key = { 0 };
    drv_pke_data output_shared_key = { 0 };
    drv_pke_ecc_point input_pub_key = { 0 };
    td_u32 klen = 0;

    crypto_dispatch_func_enter();
    dispatch_pke_cmd_chk(CRYPTO_CMD_PKE_ECC_GEN_ECDH_KEY);

    crypto_unused(private_data);
    crypto_chk_return(gen_ecdh_key_ctl->curve_type >= DRV_PKE_ECC_TYPE_MAX, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "Invalid curve_type\n");
    pke_null_ptr_chk(gen_ecdh_key_ctl->input_priv_key.data.p);
    pke_null_ptr_chk(gen_ecdh_key_ctl->input_pub_key.x.p);
    pke_null_ptr_chk(gen_ecdh_key_ctl->output_shared_key.data.p);
    if (gen_ecdh_key_ctl->curve_type != DRV_PKE_ECC_TYPE_RFC7748) {
        pke_null_ptr_chk(gen_ecdh_key_ctl->input_pub_key.y.p);
    }

    klen = gen_ecdh_key_ctl->input_priv_key.length;
    crypto_chk_return(klen < CRYPTO_PKE_ECC_KEY_MIN_SIZE || klen > CRYPTO_PKE_ECC_KEY_MAX_SIZE,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "klen is Invalid\n");
    crypto_chk_return(gen_ecdh_key_ctl->input_pub_key.length != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "gen_ecdh_key_ctl->input_pub_key.length is Invalid\n");
    crypto_chk_return(gen_ecdh_key_ctl->output_shared_key.length != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "gen_ecdh_key_ctl->output_shared_key.length is Invalid\n");

    curve_type = gen_ecdh_key_ctl->curve_type;
    input_priv_key.length = klen;
    input_pub_key.length = klen;
    output_shared_key.length = klen;
    /* input_priv_key. */
    input_priv_key.data = crypto_malloc(klen);
    crypto_chk_goto_with_ret(input_priv_key.data == TD_NULL, free_exit,
        PKE_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");

    ret = crypto_copy_from_user(input_priv_key.data, klen, gen_ecdh_key_ctl->input_priv_key.data.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    /* output_shared_key. */
    output_shared_key.data = crypto_malloc(klen);
    crypto_chk_goto_with_ret(output_shared_key.data == TD_NULL, free_exit,
        PKE_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");

    /* input_pub_key.  */
    input_pub_key.x = crypto_malloc(klen);
    crypto_chk_goto_with_ret(input_pub_key.x == TD_NULL, free_exit,
        PKE_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");

    if (gen_ecdh_key_ctl->curve_type != DRV_PKE_ECC_TYPE_RFC7748) {
        input_pub_key.y = crypto_malloc(klen);
        crypto_chk_goto_with_ret(input_pub_key.y == TD_NULL, free_exit,
            PKE_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");
    }

    ret = crypto_copy_from_user(input_pub_key.x, klen, gen_ecdh_key_ctl->input_pub_key.x.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    if (gen_ecdh_key_ctl->curve_type != DRV_PKE_ECC_TYPE_RFC7748) {
        ret = crypto_copy_from_user(input_pub_key.y, klen, gen_ecdh_key_ctl->input_pub_key.y.p, klen);
        crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
            "crypto_copy_from_user failed, ret is 0x%x\n", ret);
    }

    ret = kapi_pke_ecc_gen_ecdh_key(curve_type, &input_pub_key, &input_priv_key, &output_shared_key);
    crypto_chk_goto(ret != TD_SUCCESS, free_exit, "kapi_pke_ecc_gen_key failed, ret is 0x%x\n", ret);

    /* copy to user. */
    ret = crypto_copy_to_user(gen_ecdh_key_ctl->output_shared_key.data.p, klen, output_shared_key.data, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_TO_USER),
        "crypto_copy_to_user failed, ret is 0x%x\n", ret);
free_exit:
    if (gen_ecdh_key_ctl->curve_type != DRV_PKE_ECC_TYPE_RFC7748) {
        crypto_chk_crypto_free_with_clean(input_pub_key.y, klen);
    }
    crypto_chk_crypto_free_with_clean(input_pub_key.x, klen);
    crypto_chk_crypto_free_with_clean(output_shared_key.data, klen);
    crypto_chk_crypto_free_with_clean(input_priv_key.data, klen);
    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_pke_ecdsa_sign(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    pke_ecdsa_sign_ctl_t *ecdsa_sign_ctl = (pke_ecdsa_sign_ctl_t *)argp;
    drv_pke_ecc_curve_type curve_type = 0;
    drv_pke_data priv_key = { 0 };
    drv_pke_data hash = { 0 };
    drv_pke_ecc_sig sig = { 0 };
    td_u32 klen = 0;
    td_u32 hash_len = 0;
    td_u8 *buffer = TD_NULL;
    td_u32 buffer_size = 0;
    crypto_dispatch_func_enter();

    dispatch_pke_cmd_chk(CRYPTO_CMD_PKE_ECDSA_SIGN);
    crypto_unused(private_data);

    pke_null_ptr_chk(ecdsa_sign_ctl->priv_key.data.p);
    pke_null_ptr_chk(ecdsa_sign_ctl->hash.data.p);
    pke_null_ptr_chk(ecdsa_sign_ctl->sig.r.p);
    pke_null_ptr_chk(ecdsa_sign_ctl->sig.s.p);
    klen = ecdsa_sign_ctl->priv_key.length;
    hash_len = ecdsa_sign_ctl->hash.length;

    crypto_chk_return(klen < CRYPTO_PKE_ECC_KEY_MIN_SIZE || klen > CRYPTO_PKE_ECC_KEY_MAX_SIZE,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "klen is Invalid\n");
    crypto_chk_return(hash_len < CRYPTO_HASH_MIN_LEN || hash_len > CRYPTO_HASH_MAX_LEN,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "hash_len is Invalid\n");
    crypto_chk_return(ecdsa_sign_ctl->sig.length != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "ecdsa_sign_ctl->sig.length is Invalid\n");
    curve_type = ecdsa_sign_ctl->curve_type;

    /* alloc memory. */
    buffer_size = klen * 3 + hash_len;      // 3: for priv->d, sig->r and sig->s
    buffer = crypto_malloc(buffer_size);
    crypto_chk_return(buffer == TD_NULL, PKE_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");

    priv_key.data = buffer;
    priv_key.length = klen;

    hash.data = priv_key.data + priv_key.length;
    hash.length = hash_len;

    sig.r = hash.data + hash.length;
    sig.s = sig.r + klen;
    sig.length = klen;

    /* copy from user. */
    ret = crypto_copy_from_user(priv_key.data, klen, ecdsa_sign_ctl->priv_key.data.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_from_user(hash.data, hash_len, ecdsa_sign_ctl->hash.data.p, hash_len);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = kapi_pke_ecdsa_sign(curve_type, &priv_key, &hash, &sig);
    crypto_chk_goto(ret != TD_SUCCESS, free_exit, "kapi_pke_ecdsa_sign failed, ret is 0x%x\n", ret);

    /* copy to user. */
    ret = crypto_copy_to_user(ecdsa_sign_ctl->sig.r.p, klen, sig.r, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_TO_USER),
        "crypto_copy_to_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_to_user(ecdsa_sign_ctl->sig.s.p, klen, sig.s, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_TO_USER),
        "crypto_copy_to_user failed, ret is 0x%x\n", ret);
free_exit:
    crypto_chk_crypto_free_with_clean(buffer, buffer_size);
    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_pke_ecdsa_verify(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    pke_ecdsa_verify_ctl_t *ecdsa_verify_ctl = (pke_ecdsa_verify_ctl_t *)argp;
    drv_pke_ecc_curve_type curve_type = 0;
    drv_pke_ecc_point pub_key = { 0 };
    drv_pke_data hash = { 0 };
    drv_pke_ecc_sig sig = { 0 };
    td_u32 klen = 0;
    td_u32 hash_length = 0;
    td_u8 *buffer = TD_NULL;
    td_u32 buffer_size = 0;

    crypto_dispatch_func_enter();
    dispatch_pke_cmd_chk(CRYPTO_CMD_PKE_ECDSA_VERIFY);
    crypto_unused(private_data);

    pke_null_ptr_chk(ecdsa_verify_ctl->pub_key.x.p);
    pke_null_ptr_chk(ecdsa_verify_ctl->pub_key.y.p);
    pke_null_ptr_chk(ecdsa_verify_ctl->hash.data.p);
    pke_null_ptr_chk(ecdsa_verify_ctl->sig.r.p);
    pke_null_ptr_chk(ecdsa_verify_ctl->sig.s.p);

    klen = ecdsa_verify_ctl->pub_key.length;
    hash_length = ecdsa_verify_ctl->hash.length;
    crypto_chk_return(klen < CRYPTO_PKE_ECC_KEY_MIN_SIZE || klen > CRYPTO_PKE_ECC_KEY_MAX_SIZE,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "klen is Invalid\n");
    crypto_chk_return(hash_length < CRYPTO_HASH_MIN_LEN || hash_length > CRYPTO_HASH_MAX_LEN,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "hash_len is Invalid\n");
    crypto_chk_return(ecdsa_verify_ctl->sig.length != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "ecdsa_verify_ctl->sig.length is Invalid\n");

    curve_type = ecdsa_verify_ctl->curve_type;

    /* alloc memory. */
    buffer_size = klen * 4 + hash_length; // 4: for pub.x, pub.y, sig.r, sig.s
    buffer = crypto_malloc(buffer_size);
    crypto_chk_goto_with_ret(buffer == TD_NULL, free_exit, PKE_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");

    pub_key.x = buffer;
    pub_key.y = buffer + klen;
    pub_key.length = klen;

    hash.data = pub_key.y + pub_key.length;
    hash.length = hash_length;

    sig.r = hash.data + hash.length;
    sig.s = sig.r + klen;
    sig.length = klen;

    /* copy from user. */
    ret = crypto_copy_from_user(pub_key.x, klen, ecdsa_verify_ctl->pub_key.x.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_from_user(pub_key.y, klen, ecdsa_verify_ctl->pub_key.y.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_from_user(hash.data, hash_length, ecdsa_verify_ctl->hash.data.p, hash_length);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_from_user(sig.r, klen, ecdsa_verify_ctl->sig.r.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_from_user(sig.s, klen, ecdsa_verify_ctl->sig.s.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = kapi_pke_ecdsa_verify(curve_type, &pub_key, &hash, &sig);
    crypto_chk_goto(ret != TD_SUCCESS, free_exit, "kapi_pke_ecdsa_verify failed, ret is 0x%x\n", ret);

free_exit:
    crypto_chk_crypto_free_with_clean(buffer, buffer_size);
    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_pke_eddsa_sign(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    pke_eddsa_sign_ctl_t *eddsa_sign_ctl = (pke_eddsa_sign_ctl_t *)argp;
    drv_pke_ecc_curve_type curve_type = 0;
    drv_pke_data priv_key = { 0 };
    drv_pke_msg msg = { 0 };
    drv_pke_ecc_sig sig = { 0 };
    td_u32 klen = 0;
    td_u32 msg_len = 0;
    td_u8 *buffer = TD_NULL;
    td_u32 buffer_size = 0;
    crypto_dispatch_func_enter();

    dispatch_pke_cmd_chk(CRYPTO_CMD_PKE_EDDSA_SIGN);
    crypto_unused(private_data);

    pke_null_ptr_chk(eddsa_sign_ctl->priv_key.data.p);
    pke_null_ptr_chk(eddsa_sign_ctl->msg.data.p);
    pke_null_ptr_chk(eddsa_sign_ctl->sig.r.p);
    pke_null_ptr_chk(eddsa_sign_ctl->sig.s.p);
    klen = eddsa_sign_ctl->priv_key.length;
    msg_len = eddsa_sign_ctl->msg.length;
    crypto_chk_return(klen < CRYPTO_PKE_ECC_KEY_MIN_SIZE || klen > CRYPTO_PKE_ECC_KEY_MAX_SIZE,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "klen is Invalid\n");
    crypto_chk_return(msg_len < CRYPTO_PKE_MSG_MIN_SIZE || msg_len > CRYPTO_PKE_MSG_MAX_SIZE,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "msg_len is Invalid\n");
    crypto_chk_return(eddsa_sign_ctl->sig.length != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "eddsa_sign_ctl->sig.length is Invalid\n");

    curve_type = eddsa_sign_ctl->curve_type;

    /* alloc memory. */
    buffer_size = klen * 3 + msg_len;   // 3: for priv.d, sig.r, sig.s
    buffer = crypto_malloc(buffer_size);
    crypto_chk_return(buffer == TD_NULL, PKE_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");

    priv_key.data = buffer;
    priv_key.length = klen;

    msg.data = priv_key.data + priv_key.length;
    msg.length = msg_len;
    msg.buf_sec = eddsa_sign_ctl->msg.buf_sec;

    sig.r = msg.data + msg.length;
    sig.s = sig.r + klen;
    sig.length = klen;

    /* copy from user. */
    ret = crypto_copy_from_user(priv_key.data, klen, eddsa_sign_ctl->priv_key.data.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_from_user(msg.data, msg_len, eddsa_sign_ctl->msg.data.p, msg_len);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = kapi_pke_eddsa_sign(curve_type, &priv_key, &msg, &sig);
    crypto_chk_goto(ret != TD_SUCCESS, free_exit, "kapi_pke_ecdsa_sign failed, ret is 0x%x\n", ret);

    /* copy to user. */
    ret = crypto_copy_to_user(eddsa_sign_ctl->sig.r.p, klen, sig.r, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_TO_USER),
        "crypto_copy_to_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_to_user(eddsa_sign_ctl->sig.s.p, klen, sig.s, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_TO_USER),
        "crypto_copy_to_user failed, ret is 0x%x\n", ret);
free_exit:
    crypto_chk_crypto_free_with_clean(buffer, buffer_size);
    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_pke_eddsa_verify(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    pke_eddsa_verify_ctl_t *eddsa_verify_ctl = (pke_eddsa_verify_ctl_t *)argp;
    drv_pke_ecc_curve_type curve_type = 0;
    drv_pke_ecc_point pub_key = { 0 };
    drv_pke_msg msg = { 0 };
    drv_pke_ecc_sig sig = { 0 };
    td_u32 klen = 0;
    td_u32 msg_len = 0;
    td_u8 *buffer = TD_NULL;
    td_u32 buffer_size = 0;
    crypto_dispatch_func_enter();
    dispatch_pke_cmd_chk(CRYPTO_CMD_PKE_EDDSA_VERIFY);
    crypto_unused(private_data);

    pke_null_ptr_chk(eddsa_verify_ctl->pub_key.y.p);
    pke_null_ptr_chk(eddsa_verify_ctl->msg.data.p);
    pke_null_ptr_chk(eddsa_verify_ctl->sig.r.p);
    pke_null_ptr_chk(eddsa_verify_ctl->sig.s.p);

    klen = eddsa_verify_ctl->pub_key.length;
    msg_len = eddsa_verify_ctl->msg.length;
    crypto_chk_return(klen < CRYPTO_PKE_ECC_KEY_MIN_SIZE || klen > CRYPTO_PKE_ECC_KEY_MAX_SIZE,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "klen is Invalid\n");
    crypto_chk_return(msg_len < CRYPTO_PKE_MSG_MIN_SIZE || msg_len > CRYPTO_PKE_MSG_MAX_SIZE,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "msg_len is Invalid\n");
    crypto_chk_return(eddsa_verify_ctl->sig.length != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "eddsa_verify_ctl->sig.length is Invalid\n");
    curve_type = eddsa_verify_ctl->curve_type;

    /* alloc memory. */
    buffer_size = klen * 3 + msg_len;   // 3: for pub_key.y, sig.r, sig.s
    buffer = crypto_malloc(buffer_size);
    crypto_chk_return(buffer == TD_NULL, PKE_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");

    pub_key.y = buffer;
    pub_key.length = klen;

    msg.data = pub_key.y + klen;
    msg.length = msg_len;
    msg.buf_sec = eddsa_verify_ctl->msg.buf_sec;

    sig.r = msg.data + msg.length;
    sig.s = sig.r + klen;
    sig.length = klen;

    /* copy from user. */

    ret = crypto_copy_from_user(pub_key.y, klen, eddsa_verify_ctl->pub_key.y.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_from_user(msg.data, msg_len, eddsa_verify_ctl->msg.data.p, msg_len);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_from_user(sig.r, klen, eddsa_verify_ctl->sig.r.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_from_user(sig.s, klen, eddsa_verify_ctl->sig.s.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = kapi_pke_eddsa_verify(curve_type, &pub_key, &msg, &sig);
    crypto_chk_goto(ret != TD_SUCCESS, free_exit, "kapi_pke_ecdsa_verify failed, ret is 0x%x\n", ret);

free_exit:
    crypto_chk_crypto_free_with_clean(buffer, buffer_size);
    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_pke_check_dot_on_curve(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    pke_check_dot_on_curve_ctl_t *dot_on_curve_ctl = (pke_check_dot_on_curve_ctl_t *)argp;
    drv_pke_ecc_curve_type curve_type = 0;
    drv_pke_ecc_point pub_key = { 0 };
    td_u32 klen = 0;
    td_u8 *buffer = TD_NULL;
    td_u32 buffer_size = 0;
    crypto_dispatch_func_enter();

    dispatch_pke_cmd_chk(CRYPTO_CMD_PKE_CHECK_DOT_ON_CURVE);

    crypto_unused(private_data);

    pke_null_ptr_chk(dot_on_curve_ctl->pub_key.x.p);
    pke_null_ptr_chk(dot_on_curve_ctl->pub_key.y.p);
    klen = dot_on_curve_ctl->pub_key.length;

    crypto_chk_return(klen < CRYPTO_PKE_ECC_KEY_MIN_SIZE || klen > CRYPTO_PKE_ECC_KEY_MAX_SIZE,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "klen is invalid\n");
    curve_type = dot_on_curve_ctl->curve_type;

    buffer_size = klen * 2; // 2: for pub_key.x, pub_key.y
    buffer = crypto_malloc(buffer_size);
    crypto_chk_return(buffer == TD_NULL, PKE_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");

    /* pub_key.  */
    pub_key.x = buffer;
    pub_key.y = buffer + klen;
    pub_key.length = klen;
    /* copy from user. */
    ret = crypto_copy_from_user(pub_key.x, klen, dot_on_curve_ctl->pub_key.x.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_from_user(pub_key.y, klen, dot_on_curve_ctl->pub_key.y.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = kapi_pke_check_dot_on_curve(curve_type, &pub_key, &dot_on_curve_ctl->is_on_curve);
    crypto_chk_goto(ret != TD_SUCCESS, free_exit, "kapi_pke_check_dot_on_curve failed, ret is 0x%x\n", ret);
free_exit:
    crypto_chk_crypto_free_with_clean(buffer, buffer_size);
    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_pke_sm2_dsa_hash(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    pke_sm2_dsa_hash_ctl_t *dsa_hash_ctl = (pke_sm2_dsa_hash_ctl_t *)argp;
    drv_pke_data sm2_id = { 0 };
    drv_pke_ecc_point pub_key = { 0 };
    drv_pke_msg msg = { 0 };
    drv_pke_data hash = { 0 };
    td_u32 klen = 0;
    td_u32 msg_len = 0;
    td_u32 hash_len = 0;
    td_u32 sm2_id_len = 0;
    td_u8 *buffer = TD_NULL;
    td_u32 buffer_size = 0;
    crypto_dispatch_func_enter();

    dispatch_pke_cmd_chk(CRYPTO_CMD_PKE_SM2_DSA_HASH);

    crypto_unused(private_data);

    pke_null_ptr_chk(dsa_hash_ctl->sm2_id.data.p);
    pke_null_ptr_chk(dsa_hash_ctl->pub_key.x.p);
    pke_null_ptr_chk(dsa_hash_ctl->pub_key.y.p);
    pke_null_ptr_chk(dsa_hash_ctl->msg.data.p);
    pke_null_ptr_chk(dsa_hash_ctl->hash.data.p);
    klen = dsa_hash_ctl->pub_key.length;
    msg_len = dsa_hash_ctl->msg.length;
    hash_len = dsa_hash_ctl->hash.length;
    sm2_id_len = dsa_hash_ctl->sm2_id.length;

    crypto_chk_return(klen < CRYPTO_PKE_ECC_KEY_MIN_SIZE || klen > CRYPTO_PKE_ECC_KEY_MAX_SIZE,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "klen is Invalid\n");
    crypto_chk_return(msg_len < CRYPTO_PKE_MSG_MIN_SIZE || msg_len > CRYPTO_PKE_MSG_MAX_SIZE,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "msg_len is Invalid\n");
    crypto_chk_return(hash_len < CRYPTO_HASH_MIN_LEN || hash_len > CRYPTO_HASH_MAX_LEN,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "hash_len is Invalid\n");
    crypto_chk_return(sm2_id_len < CRYPTO_PKE_SM2_ID_MIN_SIZE || sm2_id_len > CRYPTO_PKE_SM2_ID_MAX_SIZE,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "sm2_id_len is Invalid\n");

    buffer_size = sm2_id_len + klen * 2 + msg_len + hash_len;   // 2: for pub_key.x, pub_key.y
    buffer = crypto_malloc(buffer_size);
    crypto_chk_return(buffer == TD_NULL, PKE_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");

    sm2_id.data = buffer;
    sm2_id.length = sm2_id_len;

    pub_key.x = sm2_id.data + sm2_id.length;
    pub_key.y = pub_key.x + klen;
    pub_key.length = klen;

    msg.data = pub_key.y + pub_key.length;
    msg.length = msg_len;
    msg.buf_sec = dsa_hash_ctl->msg.buf_sec;

    hash.data = msg.data + msg.length;
    hash.length = hash_len;

    /* copy from user. */
    ret = crypto_copy_from_user(sm2_id.data, sm2_id_len, dsa_hash_ctl->sm2_id.data.p, sm2_id_len);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_from_user(pub_key.x, klen, dsa_hash_ctl->pub_key.x.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_from_user(pub_key.y, klen, dsa_hash_ctl->pub_key.y.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_from_user(msg.data, msg_len, dsa_hash_ctl->msg.data.p, msg_len);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = kapi_pke_sm2_dsa_hash(&sm2_id, &pub_key, &msg, &hash);
    crypto_chk_goto(ret != TD_SUCCESS, free_exit, "kapi_pke_sm2_dsa_hash failed, ret is 0x%x\n", ret);

    ret = crypto_copy_to_user(dsa_hash_ctl->hash.data.p, hash_len, hash.data, hash_len);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_TO_USER),
        "crypto_copy_to_user failed, ret is 0x%x\n", ret);

free_exit:
    crypto_chk_crypto_free_with_clean(buffer, buffer_size);
    crypto_dispatch_func_exit();

    return ret;
}

static td_s32 dispatch_pke_sm2_public_encrypt(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    pke_sm2_public_encrypt_ctl_t *public_encrypt_ctl = (pke_sm2_public_encrypt_ctl_t *)argp;
    drv_pke_ecc_point pub_key = { 0 };
    drv_pke_data plain_text = { 0 };
    drv_pke_data cipher_text = { 0 };
    td_u32 klen = 0;
    td_u32 plain_text_len = 0;
    td_u32 cipher_text_len = 0;
    td_u8 *buffer = TD_NULL;
    td_u32 buffer_size = 0;

    crypto_dispatch_func_enter();
    dispatch_pke_cmd_chk(CRYPTO_CMD_PKE_SM2_PUBLIC_ENCRYPT);
    crypto_unused(private_data);

    pke_null_ptr_chk(public_encrypt_ctl->pub_key.x.p);
    pke_null_ptr_chk(public_encrypt_ctl->pub_key.y.p);
    pke_null_ptr_chk(public_encrypt_ctl->plain_text.data.p);
    pke_null_ptr_chk(public_encrypt_ctl->cipher_text.data.p);

    klen = public_encrypt_ctl->pub_key.length;
    plain_text_len = public_encrypt_ctl->plain_text.length;
    cipher_text_len = public_encrypt_ctl->cipher_text.length;

    crypto_chk_return(klen < CRYPTO_PKE_ECC_KEY_MIN_SIZE || klen > CRYPTO_PKE_ECC_KEY_MAX_SIZE,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "klen is Invalid\n");
    crypto_chk_return(plain_text_len < CRYPTO_PKE_SM2_PLAIN_TEXT_MIN_SIZE ||
        plain_text_len > CRYPTO_PKE_SM2_PLAIN_TEXT_MAX_SIZE, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "plain_text_len is Invalid\n");
    crypto_chk_return(cipher_text_len < CRYPTO_PKE_SM2_PLAIN_TEXT_MIN_SIZE ||
        cipher_text_len > CRYPTO_PKE_SM2_PLAIN_TEXT_MAX_SIZE, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "cipher_text_len is Invalid\n");

    buffer_size = klen * 2 + plain_text_len + cipher_text_len;    // 2: for pub_key.x, pub_key.y
    buffer = crypto_malloc(buffer_size);
    crypto_chk_return(buffer == TD_NULL, PKE_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");

    pub_key.x = buffer;
    pub_key.y = buffer + klen;
    pub_key.length = klen;

    plain_text.data = pub_key.y + klen;
    plain_text.length = plain_text_len;

    cipher_text.data = plain_text.data + plain_text.length;
    cipher_text.length = cipher_text_len;

    /* copy from user. */
    ret = crypto_copy_from_user(plain_text.data, plain_text_len, public_encrypt_ctl->plain_text.data.p, plain_text_len);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_from_user(pub_key.x, klen, public_encrypt_ctl->pub_key.x.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_from_user(pub_key.y, klen, public_encrypt_ctl->pub_key.y.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = kapi_pke_sm2_public_encrypt(&pub_key, &plain_text, &cipher_text);
    crypto_chk_goto(ret != TD_SUCCESS, free_exit, "kapi_pke_sm2_public_encrypt failed, ret is 0x%x\n", ret);

    ret = crypto_copy_to_user(public_encrypt_ctl->cipher_text.data.p, public_encrypt_ctl->cipher_text.length,
        cipher_text.data, cipher_text.length);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_TO_USER),
        "crypto_copy_to_user failed, ret is 0x%x\n", ret);
    public_encrypt_ctl->cipher_text.length = cipher_text.length;
free_exit:
    crypto_chk_crypto_free_with_clean(buffer, buffer_size);
    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_pke_sm2_private_decrypt(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    pke_sm2_private_decrypt_ctl_t *private_decrypt_ctl = (pke_sm2_private_decrypt_ctl_t *)argp;
    drv_pke_data priv_key = { 0 };
    drv_pke_data cipher_text = { 0 };
    drv_pke_data plain_text = { 0 };
    td_u32 klen = 0;
    td_u32 plain_text_len = 0;
    td_u32 cipher_text_len = 0;
    td_u8 *buffer = TD_NULL;
    td_u32 buffer_size = 0;
    crypto_dispatch_func_enter();
    dispatch_pke_cmd_chk(CRYPTO_CMD_PKE_SM2_PRIVATE_DECRYPT);

    crypto_unused(private_data);

    pke_null_ptr_chk(private_decrypt_ctl->priv_key.data.p);
    pke_null_ptr_chk(private_decrypt_ctl->cipher_text.data.p);
    pke_null_ptr_chk(private_decrypt_ctl->plain_text.data.p);

    klen = private_decrypt_ctl->priv_key.length;
    plain_text_len = private_decrypt_ctl->plain_text.length;
    cipher_text_len = private_decrypt_ctl->cipher_text.length;

    crypto_chk_return(klen < CRYPTO_PKE_ECC_KEY_MIN_SIZE || klen > CRYPTO_PKE_ECC_KEY_MAX_SIZE,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "klen is Invalid\n");
    crypto_chk_return(plain_text_len < CRYPTO_PKE_SM2_PLAIN_TEXT_MIN_SIZE ||
        plain_text_len > CRYPTO_PKE_SM2_PLAIN_TEXT_MAX_SIZE, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "plain_text_len is Invalid\n");
    crypto_chk_return(cipher_text_len < CRYPTO_PKE_SM2_PLAIN_TEXT_MIN_SIZE ||
        cipher_text_len > CRYPTO_PKE_SM2_PLAIN_TEXT_MAX_SIZE, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "cipher_text_len is Invalid\n");
    buffer_size = klen + plain_text_len + cipher_text_len;
    buffer = crypto_malloc(buffer_size);
    crypto_chk_return(buffer == TD_NULL, PKE_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");

    priv_key.data = buffer;
    priv_key.length = klen;

    cipher_text.data = priv_key.data + priv_key.length;
    cipher_text.length = cipher_text_len;

    plain_text.data = cipher_text.data + cipher_text.length;
    plain_text.length = plain_text_len;

    /* copy from user. */
    ret = crypto_copy_from_user(cipher_text.data, cipher_text_len, private_decrypt_ctl->cipher_text.data.p,
        cipher_text_len);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_from_user(priv_key.data, klen, private_decrypt_ctl->priv_key.data.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = kapi_pke_sm2_private_decrypt(&priv_key, &cipher_text, &plain_text);
    crypto_chk_goto(ret != TD_SUCCESS, free_exit, "kapi_pke_sm2_private_decrypt failed, ret is 0x%x\n", ret);

    ret = crypto_copy_to_user(private_decrypt_ctl->plain_text.data.p, private_decrypt_ctl->plain_text.length,
        plain_text.data, plain_text.length);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_TO_USER),
        "crypto_copy_to_user failed, ret is 0x%x\n", ret);
    private_decrypt_ctl->plain_text.length = plain_text.length;
free_exit:
    crypto_chk_crypto_free_with_clean(buffer, buffer_size);
    crypto_dispatch_func_exit();

    return ret;
}

static td_s32 dispatch_pke_mod(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    pke_mod_ctl_t *mod_ctr = (pke_mod_ctl_t *)argp;
    drv_pke_data a;
    drv_pke_data p;
    drv_pke_data c;
    td_u8 *buffer = TD_NULL;
    td_u32 buffer_size = 0;
    td_u32 klen;
    crypto_dispatch_func_enter();

    dispatch_pke_cmd_chk(CRYPTO_CMD_PKE_MOD);
    crypto_unused(private_data);

    pke_null_ptr_chk(mod_ctr->a.data.p);
    pke_null_ptr_chk(mod_ctr->p.data.p);
    pke_null_ptr_chk(mod_ctr->c.data.p);

    klen = mod_ctr->p.length;
    crypto_chk_return(klen < CRYPTO_PKE_RSA_KEY_MIN_SIZE || klen > CRYPTO_PKE_RSA_KEY_MAX_SIZE,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "klen is Invalid\n");
    crypto_chk_return(mod_ctr->a.length != klen || mod_ctr->c.length != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "mod_ctr->a.length or mod_ctr->c.length is Invalid\n");
    
    buffer_size = klen * 3;   // 3: for a、p、c
    buffer = crypto_malloc(buffer_size);
    crypto_chk_return(buffer == TD_NULL, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "crypto_malloc failed\n");
    (void)memset_s(buffer, buffer_size, 0, buffer_size);

    a.data = buffer;
    a.length = klen;

    p.data = buffer + klen;
    p.length = klen;

    c.data = buffer + klen + klen;
    c.length = klen;

    /* copy from user. */
    ret = crypto_copy_from_user(a.data, a.length, mod_ctr->a.data.p, mod_ctr->a.length);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);
    ret = crypto_copy_from_user(p.data, p.length, mod_ctr->p.data.p, mod_ctr->p.length);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = kapi_cipher_pke_mod(&a, &p, &c);
    crypto_chk_goto(ret != TD_SUCCESS, free_exit, "kapi_cipher_pke_mod failed, ret is 0x%x\n", ret);

    /* copy to user. */
    ret = crypto_copy_to_user(mod_ctr->c.data.p, mod_ctr->c.length, c.data, c.length);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_TO_USER),
        "crypto_copy_to_user failed, ret is 0x%x\n", ret);

free_exit:
    crypto_chk_crypto_free_with_clean(buffer, buffer_size);
    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_pke_exp_mod(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    pke_exp_mod_ctl_t *exp_mod_ctr = (pke_exp_mod_ctl_t *)argp;
    drv_pke_data n;
    drv_pke_data k;
    drv_pke_data in;
    drv_pke_data out;
    td_u8 *buffer = TD_NULL;
    td_u32 buffer_size = 0;
    td_u32 klen;
    crypto_dispatch_func_enter();

    dispatch_pke_cmd_chk(CRYPTO_CMD_PKE_EXP_MOD);
    crypto_unused(private_data);

    pke_null_ptr_chk(exp_mod_ctr->n.data.p);
    pke_null_ptr_chk(exp_mod_ctr->k.data.p);
    pke_null_ptr_chk(exp_mod_ctr->in.data.p);
    pke_null_ptr_chk(exp_mod_ctr->out.data.p);

    klen = exp_mod_ctr->n.length;
    crypto_chk_return(klen < CRYPTO_PKE_RSA_KEY_MIN_SIZE || klen > CRYPTO_PKE_RSA_KEY_MAX_SIZE,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "klen is Invalid\n");
    crypto_chk_return(exp_mod_ctr->k.length != klen || exp_mod_ctr->in.length != klen ||
        exp_mod_ctr->out.length != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "exp_mod_ctr->k.length or exp_mod_ctr->in.length or exp_mod_ctr->out.length is Invalid\n");
    
    buffer_size = klen * 4;   // 4: for n、k、in、out
    buffer = crypto_malloc(buffer_size);
    crypto_chk_return(buffer == TD_NULL, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "crypto_malloc failed\n");
    (void)memset_s(buffer, buffer_size, 0, buffer_size);

    n.data = buffer;
    n.length = klen;

    k.data = buffer + klen;
    k.length = klen;

    in.data = buffer + klen + klen;
    in.length = klen;

    out.data = buffer + klen + klen;
    out.length = klen;

    /* copy from user. */
    ret = crypto_copy_from_user(n.data, n.length, exp_mod_ctr->n.data.p, exp_mod_ctr->n.length);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);
    ret = crypto_copy_from_user(k.data, k.length, exp_mod_ctr->k.data.p, exp_mod_ctr->k.length);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);
    ret = crypto_copy_from_user(in.data, in.length, exp_mod_ctr->in.data.p, exp_mod_ctr->in.length);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = kapi_cipher_pke_exp_mod(&n, &k, &in, &out);
    crypto_chk_goto(ret != TD_SUCCESS, free_exit, "kapi_cipher_pke_exp_mod failed, ret is 0x%x\n", ret);

    /* copy to user. */
    ret = crypto_copy_to_user(exp_mod_ctr->out.data.p, exp_mod_ctr->out.length, out.data, out.length);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_TO_USER),
        "crypto_copy_to_user failed, ret is 0x%x\n", ret);

free_exit:
    crypto_chk_crypto_free_with_clean(buffer, buffer_size);
    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_pke_rsa_sign(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    pke_rsa_sign_ctl_t *rsa_sign_ctl = (pke_rsa_sign_ctl_t *)argp;
    drv_pke_rsa_priv_key priv_key;
    drv_pke_rsa_scheme scheme;
    drv_pke_hash_type hash_type;
    drv_pke_data input_hash;
    drv_pke_data sig;
    td_u32 klen = 0;
    td_u32 hash_length = 0;
    td_u8 *buffer = TD_NULL;
    td_u32 buffer_size = 0;
    crypto_dispatch_func_enter();

    dispatch_pke_cmd_chk(CRYPTO_CMD_PKE_RSA_SIGN);
    crypto_unused(private_data);

    pke_null_ptr_chk(rsa_sign_ctl->priv_key.d.p);
    pke_null_ptr_chk(rsa_sign_ctl->priv_key.n.p);
    pke_null_ptr_chk(rsa_sign_ctl->input_hash.data.p);
    pke_null_ptr_chk(rsa_sign_ctl->sig.data.p);

    klen = rsa_sign_ctl->priv_key.d_len;
    hash_length = rsa_sign_ctl->input_hash.length;
    crypto_chk_return(klen < CRYPTO_PKE_RSA_KEY_MIN_SIZE || klen > CRYPTO_PKE_RSA_KEY_MAX_SIZE,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "klen is Invalid\n");
    crypto_chk_return(hash_length < CRYPTO_HASH_MIN_LEN || hash_length > CRYPTO_HASH_MAX_LEN,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "hash_len is Invalid\n");
    crypto_chk_return(rsa_sign_ctl->sig.length < klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "rsa_sign_ctl->sig.length is Invalid\n");
    crypto_chk_return(rsa_sign_ctl->priv_key.n_len != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "rsa_sign_ctl->priv_key.n_len is Invalid\n");

    scheme = rsa_sign_ctl->scheme;
    hash_type = rsa_sign_ctl->hash_type;

    /* alloc memory. */
    buffer_size = hash_length + klen * 3;   // 3: for priv_key.n, priv_key.d, sign
    buffer = crypto_malloc(buffer_size);
    crypto_chk_return(buffer == TD_NULL, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "crypto_malloc failed\n");

    input_hash.data = buffer;
    input_hash.length = hash_length;

    sig.data = input_hash.data + input_hash.length;
    sig.length = klen;

    priv_key.n = sig.data + sig.length;
    priv_key.n_len = klen;

    priv_key.d = priv_key.n + priv_key.n_len;
    priv_key.d_len = klen;

    /* copy from user. */
    ret = crypto_copy_from_user(input_hash.data, hash_length, rsa_sign_ctl->input_hash.data.p, hash_length);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_from_user(priv_key.n, klen, rsa_sign_ctl->priv_key.n.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_from_user(priv_key.d, klen, rsa_sign_ctl->priv_key.d.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = kapi_pke_rsa_sign(&priv_key, scheme, hash_type, &input_hash, &sig);
    crypto_chk_goto(ret != TD_SUCCESS, free_exit, "kapi_pke_rsa_sign failed, ret is 0x%x\n", ret);

    /* copy to user. */
    ret = crypto_copy_to_user(rsa_sign_ctl->sig.data.p, klen, sig.data, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_TO_USER),
        "crypto_copy_to_user failed, ret is 0x%x\n", ret);
    rsa_sign_ctl->sig.length = klen;

free_exit:
    crypto_chk_crypto_free_with_clean(buffer, buffer_size);
    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_pke_rsa_verify(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    pke_rsa_verify_ctl_t *rsa_verify_ctl = (pke_rsa_verify_ctl_t *)argp;

    drv_pke_rsa_pub_key pub_key = { 0 };
    drv_pke_data input_hash = { 0 };
    drv_pke_data sig = { 0 };
    td_u32 klen = 0;
    td_u32 hash_len = 0;
    td_u8 *buffer = TD_NULL;
    td_u32 buffer_size = 0;
    crypto_dispatch_func_enter();
    dispatch_pke_cmd_chk(CRYPTO_CMD_PKE_RSA_VERIFY);

    crypto_unused(private_data);
    pke_null_ptr_chk(rsa_verify_ctl->pub_key.e.p);
    pke_null_ptr_chk(rsa_verify_ctl->pub_key.n.p);
    pke_null_ptr_chk(rsa_verify_ctl->input_hash.data.p);
    pke_null_ptr_chk(rsa_verify_ctl->sig.data.p);

    klen = rsa_verify_ctl->pub_key.len;
    hash_len = rsa_verify_ctl->input_hash.length;
    crypto_chk_return(klen < CRYPTO_PKE_RSA_KEY_MIN_SIZE || klen > CRYPTO_PKE_RSA_KEY_MAX_SIZE,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "klen is Invalid\n");
    crypto_chk_return(hash_len < CRYPTO_HASH_MIN_LEN || hash_len > CRYPTO_HASH_MAX_LEN,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "hash_len is Invalid\n");

    crypto_chk_return(rsa_verify_ctl->sig.length < klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "klen is Invalid\n");

    /* alloc memory. */
    buffer_size = klen * 3 + hash_len;  // 3: for pub_key.n, pub_key.e, sign hash_len: for input_hash
    buffer = crypto_malloc(buffer_size);
    crypto_chk_return(buffer == TD_NULL, PKE_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");

    pub_key.n = buffer;
    pub_key.e = buffer + klen;
    pub_key.len = klen;

    input_hash.data = pub_key.e + pub_key.len;
    input_hash.length = hash_len;

    sig.data = input_hash.data + input_hash.length;
    sig.length = klen;

    /* copy from user. */
    ret = crypto_copy_from_user(pub_key.n, klen, rsa_verify_ctl->pub_key.n.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_from_user(pub_key.e, klen, rsa_verify_ctl->pub_key.e.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_from_user(input_hash.data, hash_len, rsa_verify_ctl->input_hash.data.p, hash_len);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_from_user(sig.data, klen, rsa_verify_ctl->sig.data.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = kapi_pke_rsa_verify(&pub_key, rsa_verify_ctl->scheme, rsa_verify_ctl->hash_type, &input_hash, &sig);
    crypto_chk_goto(ret != TD_SUCCESS, free_exit, "kapi_pke_rsa_verify failed, ret is 0x%x\n", ret);

free_exit:
    crypto_chk_crypto_free_with_clean(buffer, buffer_size);
    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_pke_rsa_public_encrypt(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    td_u8 *data_buffer = TD_NULL;
    pke_rsa_pub_crypto_ctl_t *crypto_ctl = (pke_rsa_pub_crypto_ctl_t *)argp;
    drv_pke_rsa_pub_key pub_key = { 0 };
    drv_pke_data input = { 0 };
    drv_pke_data label = { 0 };
    drv_pke_data output = { 0 };
    td_u32 total_len = 0;
    td_u32 klen = 0;
    td_u32 input_len = 0;
    td_u32 label_len = 0;
    crypto_dispatch_func_enter();
    dispatch_pke_cmd_chk(CRYPTO_CMD_PKE_RSA_PUBLIC_ENCRYPT);

    crypto_unused(private_data);
    pke_null_ptr_chk(crypto_ctl->pub_key.e.p);
    pke_null_ptr_chk(crypto_ctl->pub_key.n.p);
    pke_null_ptr_chk(crypto_ctl->input.data.p);
    pke_null_ptr_chk(crypto_ctl->output.data.p);

    klen = crypto_ctl->pub_key.len;
    input_len = crypto_ctl->input.length;
    label_len = crypto_ctl->label.length;
    crypto_chk_return(klen < CRYPTO_PKE_RSA_KEY_MIN_SIZE || klen > CRYPTO_PKE_RSA_KEY_MAX_SIZE,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "klen is Invalid\n");
    crypto_chk_return(input_len == 0 || input_len > CRYPTO_PKE_RSA_KEY_MAX_SIZE,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "input_len is Invalid\n");
    crypto_chk_return(crypto_ctl->output.length < klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "crypto_ctl->output.length is Invalid\n");
    crypto_chk_return(label_len > CRYPTO_PKE_RSA_LABLE_MAX_SIZE, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "label_len is Invalid\n");

    /* 2 pub_key len */
    total_len = klen * 3 + input_len + label_len;   // 3: for pub_key.n, pub_key.e,output_data
    /* alloc memory. */
    data_buffer = crypto_malloc(total_len);
    crypto_chk_return(data_buffer == TD_NULL, PKE_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");

    pub_key.n = data_buffer;
    pub_key.e = pub_key.n + klen;
    pub_key.len = klen;

    input.data = pub_key.e + klen;
    input.length = input_len;

    output.data = input.data + input.length;
    output.length = klen;

    label.data = output.data + output.length;
    label.length = label_len;

    ret = crypto_copy_from_user(pub_key.n, klen, crypto_ctl->pub_key.n.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_from_user(pub_key.e, klen, crypto_ctl->pub_key.e.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_from_user(input.data, input_len, crypto_ctl->input.data.p, input_len);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    if (label_len != 0) {
        ret = crypto_copy_from_user(label.data, label_len, crypto_ctl->label.data.p, label_len);
        crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
            "crypto_copy_from_user failed, ret is 0x%x\n", ret);
        ret = kapi_pke_rsa_public_encrypt(crypto_ctl->scheme, crypto_ctl->hash_type, &pub_key, &input, &label, &output);
    } else {
        ret =
            kapi_pke_rsa_public_encrypt(crypto_ctl->scheme, crypto_ctl->hash_type, &pub_key, &input, TD_NULL, &output);
    }
    crypto_chk_goto(ret != TD_SUCCESS, free_exit, "kapi_pke_rsa_public_encrypt failed, ret is 0x%x\n", ret);
    /* copy to user. */
    ret = crypto_copy_to_user(crypto_ctl->output.data.p, klen, output.data, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_TO_USER),
        "crypto_copy_to_user failed, ret is 0x%x\n", ret);
    crypto_ctl->output.length = klen;
free_exit:
    crypto_chk_crypto_free_with_clean(data_buffer, total_len);
    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_pke_rsa_private_decrypt(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    td_u8 *data_buffer = TD_NULL;
    pke_rsa_priv_crypto_ctl_t *crypto_ctl = (pke_rsa_priv_crypto_ctl_t *)argp;

    drv_pke_rsa_priv_key priv_key = { 0 };
    drv_pke_data input = { 0 };
    drv_pke_data label = { 0 };
    drv_pke_data output = { 0 };

    td_u32 total_len = 0;
    td_u32 klen = 0;
    td_u32 label_len = 0;
    crypto_dispatch_func_enter();
    dispatch_pke_cmd_chk(CRYPTO_CMD_PKE_RSA_PRIVATE_DECRYPT);

    crypto_unused(private_data);
    pke_null_ptr_chk(crypto_ctl->priv_key.d.p);
    pke_null_ptr_chk(crypto_ctl->priv_key.n.p);
    pke_null_ptr_chk(crypto_ctl->input.data.p);
    pke_null_ptr_chk(crypto_ctl->output.data.p);

    klen = crypto_ctl->priv_key.n_len;
    label_len = crypto_ctl->label.length;
    crypto_chk_return(klen < CRYPTO_PKE_RSA_KEY_MIN_SIZE || klen > CRYPTO_PKE_RSA_KEY_MAX_SIZE,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "klen is Invalid\n");
    crypto_chk_return(crypto_ctl->priv_key.d_len != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "crypto_ctl->priv_key.d_len is Invalid\n");
    crypto_chk_return(crypto_ctl->input.length != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "crypto_ctl->input.length is Invalid\n");
    crypto_chk_return(crypto_ctl->output.length == 0, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "crypto_ctl->output.length is Invalid\n");
    crypto_chk_return(label_len > CRYPTO_PKE_RSA_LABLE_MAX_SIZE, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "label_len is Invalid\n");

    total_len = klen * 4 + label_len;   // 4: for priv_key.n, priv_key.d, input.data, output.data
    /* alloc memory. */
    data_buffer = crypto_malloc(total_len);
    crypto_chk_return(data_buffer == TD_NULL, PKE_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");

    priv_key.n = data_buffer;
    priv_key.n_len = klen;
    priv_key.d = priv_key.n + priv_key.n_len;
    priv_key.d_len = klen;

    input.data = priv_key.d + priv_key.d_len;
    input.length = klen;

    output.data = input.data + input.length;
    output.length = klen;

    label.data = output.data + output.length;
    label.length = label_len;

    ret = crypto_copy_from_user(priv_key.n, klen, crypto_ctl->priv_key.n.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_from_user(priv_key.d, klen, crypto_ctl->priv_key.d.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    ret = crypto_copy_from_user(input.data, klen, crypto_ctl->input.data.p, klen);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
        "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    if (label_len != 0) {
        ret = crypto_copy_from_user(label.data, label_len, crypto_ctl->label.data.p, label_len);
        crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_FROM_USER),
            "crypto_copy_from_user failed, ret is 0x%x\n", ret);
        ret =
            kapi_pke_rsa_private_decrypt(crypto_ctl->scheme, crypto_ctl->hash_type, &priv_key, &input, &label, &output);
    } else {
        ret = kapi_pke_rsa_private_decrypt(crypto_ctl->scheme, crypto_ctl->hash_type, &priv_key, &input, TD_NULL,
            &output);
    }
    crypto_chk_goto(ret != TD_SUCCESS, free_exit, "kapi_pke_rsa_private_decrypt failed, ret is 0x%x\n", ret);
    /* copy to user. */
    ret = crypto_copy_to_user(crypto_ctl->output.data.p, crypto_ctl->output.length, output.data, output.length);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, free_exit, PKE_COMPAT_ERRNO(ERROR_COPY_TO_USER),
        "crypto_copy_to_user failed, ret is 0x%x\n", ret);
    crypto_ctl->output.length = output.length;
free_exit:
    crypto_chk_crypto_free_with_clean(data_buffer, total_len);
    crypto_dispatch_func_exit();
    return ret;
}

static crypto_ioctl_cmd g_dispatch_func[] = {
    { CRYPTO_CMD_HASH_INIT,                     dispatch_hash_init },
    { CRYPTO_CMD_HASH_DEINIT,                   dispatch_hash_deinit },
    { CRYPTO_CMD_HASH_START,                    dispatch_hash_start },
    { CRYPTO_CMD_HASH_UPDATE,                   dispatch_hash_update },
    { CRYPTO_CMD_HASH_FINISH,                   dispatch_hash_finish },
    { CRYPTO_CMD_HASH_GET,                      dispatch_hash_get },
    { CRYPTO_CMD_HASH_SET,                      dispatch_hash_set },
    { CRYPTO_CMD_HASH_DESTROY,                  dispatch_hash_destroy },
    { CRYPTO_CMD_PBKDF2,                        dispatch_cipher_pbkdf2 },
    { CRYPTO_CMD_TRNG_GET_RANDOM,               dispatch_trng_get_random },
    { CRYPTO_CMD_TRNG_GET_MULTI_RANDOM,         dispatch_trng_get_multi_random },

    { CRYPTO_CMD_SYMC_INIT,                     dispatch_symc_init },
    { CRYPTO_CMD_SYMC_DEINIT,                   dispatch_symc_deinit },
    { CRYPTO_CMD_SYMC_CREATE,                   dispatch_symc_create },
    { CRYPTO_CMD_SYMC_DESTROY,                  dispatch_symc_destroy },
    { CRYPTO_CMD_SYMC_SET_CONFIG,               dispatch_symc_set_config },
    { CRYPTO_CMD_SYMC_GET_CONFIG,               dispatch_symc_get_config },
    { CRYPTO_CMD_SYMC_ATTACH,                   dispatch_symc_attach },
    { CRYPTO_CMD_SYMC_ENCRYPT,                  dispatch_symc_encrypt },
    { CRYPTO_CMD_SYMC_DECRYPT,                  dispatch_symc_decrypt },
    { CRYPTO_CMD_SYMC_ENCRYPT_MULTI,            dispatch_symc_encrypt_multi },
    { CRYPTO_CMD_SYMC_DECRYPT_MULTI,            dispatch_symc_decrypt_multi },
    { CRYPTO_CMD_SYMC_GET_TAG,                  dispatch_symc_get_tag },
    { CRYPTO_CMD_SYMC_MAC_START,                dispatch_symc_mac_start },
    { CRYPTO_CMD_SYMC_MAC_UPDATE,               dispatch_symc_mac_update },
    { CRYPTO_CMD_SYMC_MAC_FINISH,               dispatch_symc_mac_finish },

    { CRYPTO_CMD_PKE_ECC_GEN_KEY,               dispatch_pke_ecc_gen_key},
    { CRYPTO_CMD_PKE_ECC_GEN_ECDH_KEY,          dispatch_pke_ecc_gen_ecdh_key},
    { CRYPTO_CMD_PKE_ECDSA_SIGN,                dispatch_pke_ecdsa_sign},
    { CRYPTO_CMD_PKE_ECDSA_VERIFY,              dispatch_pke_ecdsa_verify},
    { CRYPTO_CMD_PKE_EDDSA_SIGN,                dispatch_pke_eddsa_sign},
    { CRYPTO_CMD_PKE_EDDSA_VERIFY,              dispatch_pke_eddsa_verify},
    { CRYPTO_CMD_PKE_CHECK_DOT_ON_CURVE,        dispatch_pke_check_dot_on_curve},

    { CRYPTO_CMD_PKE_SM2_DSA_HASH,              dispatch_pke_sm2_dsa_hash},
    { CRYPTO_CMD_PKE_SM2_PUBLIC_ENCRYPT,        dispatch_pke_sm2_public_encrypt},
    { CRYPTO_CMD_PKE_SM2_PRIVATE_DECRYPT,       dispatch_pke_sm2_private_decrypt},

    { CRYPTO_CMD_PKE_MOD,                       dispatch_pke_mod},
    { CRYPTO_CMD_PKE_EXP_MOD,                   dispatch_pke_exp_mod},

    { CRYPTO_CMD_PKE_RSA_SIGN,                  dispatch_pke_rsa_sign},
    { CRYPTO_CMD_PKE_RSA_VERIFY,                dispatch_pke_rsa_verify},
    { CRYPTO_CMD_PKE_RSA_PUBLIC_ENCRYPT,        dispatch_pke_rsa_public_encrypt},
    { CRYPTO_CMD_PKE_RSA_PRIVATE_DECRYPT,       dispatch_pke_rsa_private_decrypt},
};

crypto_ioctl_cmd *crypto_get_ioctl_cmd_list(td_void)
{
    return g_dispatch_func;
}

td_u32 crypto_get_ioctl_cmd_cnt(td_void)
{
    return sizeof(g_dispatch_func) / sizeof(crypto_ioctl_cmd);
}