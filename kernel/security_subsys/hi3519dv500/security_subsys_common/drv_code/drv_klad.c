/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "drv_klad.h"

#include "crypto_drv_common.h"
#include "crypto_common_macro.h"

#include "hal_klad.h"
#include "hal_rkp.h"

#define KM_COMPAT_ERRNO(err_code)     DRV_COMPAT_ERRNO(ERROR_MODULE_KM, err_code)
#define km_null_ptr_chk(ptr)   \
    crypto_chk_return((ptr) == TD_NULL, KM_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), #ptr" is NULL\n")

typedef struct {
    crypto_kdf_hard_key_type hard_key_type;
    crypto_klad_dest klad_dest;
    crypto_klad_attr klad_attr;
    td_handle keyslot_handle;
    td_u32 rkp_sw_cfg;
    td_bool is_open;
    td_bool is_attached;
    td_bool is_set_attr;
    td_bool is_set_key;
    td_bool is_set_session_key;
    crypto_klad_session_key session_key;
} drv_klad_context;

static drv_klad_context g_klad_ctx = {0};

#define KLAD_VALID_HANDLE       0x2D3C4B5A

td_s32 drv_klad_create(td_handle *klad_handle)
{
    crypto_drv_func_enter();

    km_null_ptr_chk(klad_handle);

    (td_void)memset_s(&g_klad_ctx, sizeof(drv_klad_context), 0, sizeof(drv_klad_context));

    g_klad_ctx.is_open = TD_TRUE;

    *klad_handle = KLAD_VALID_HANDLE;
    crypto_drv_func_enter();
    return TD_SUCCESS;
}

td_s32 drv_klad_destroy(td_handle klad_handle)
{
    crypto_drv_func_enter();

    crypto_chk_return(klad_handle != KLAD_VALID_HANDLE, KM_COMPAT_ERRNO(ERROR_INVALID_HANDLE), "invalid klad_handle\n");
    if (g_klad_ctx.is_open == TD_FALSE) {
        return TD_SUCCESS;
    }
    (td_void)memset_s(&g_klad_ctx, sizeof(drv_klad_context), 0, sizeof(drv_klad_context));

    crypto_drv_func_enter();
    return TD_SUCCESS;
}

td_s32 drv_klad_attach(td_handle klad_handle, crypto_klad_dest klad_dest, td_handle keyslot_handle)
{
    volatile td_s32 ret = TD_FAILURE;
    crypto_klad_flash_key_type flash_key_type = CRYPTO_KLAD_FLASH_KEY_TYPE_INVALID;
    crypto_drv_func_enter();

    crypto_chk_return(klad_handle != KLAD_VALID_HANDLE, KM_COMPAT_ERRNO(ERROR_INVALID_HANDLE), "invalid klad_handle\n");
    crypto_chk_return(g_klad_ctx.is_open == TD_FALSE, KM_COMPAT_ERRNO(ERROR_CTX_CLOSED), "call create first\n");

    if (klad_dest == CRYPTO_KLAD_DEST_FLASH) {
        flash_key_type = (crypto_klad_flash_key_type)keyslot_handle;
    } else if (klad_dest == CRYPTO_KLAD_DEST_MCIPHER || klad_dest == CRYPTO_KLAD_DEST_HMAC) {
        ret = hal_klad_set_key_addr(klad_dest, keyslot_handle);
        crypto_chk_return(ret != TD_SUCCESS, ret, "hal_klad_set_key_addr failed\n");
    } else if (klad_dest == CRYPTO_KLAD_DEST_NPU) {
        ret = hal_klad_set_key_addr(klad_dest, keyslot_handle);
        crypto_chk_return(ret != TD_SUCCESS, ret, "hal_klad_set_key_addr failed\n");
    } else {
        crypto_log_err("invalid klad_dest\n");
        return KM_COMPAT_ERRNO(ERROR_INVALID_PARAM);
    }

    ret = hal_klad_set_key_dest_cfg(klad_dest, flash_key_type);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_klad_set_key_dest_cfg failed\n");

    g_klad_ctx.keyslot_handle = keyslot_handle;
    g_klad_ctx.klad_dest = klad_dest;
    g_klad_ctx.is_attached = TD_TRUE;
    crypto_drv_func_enter();
    return ret;
}

td_s32 drv_klad_detach(td_handle klad_handle, crypto_klad_dest klad_dest, td_handle keyslot_handle)
{
    crypto_unused(keyslot_handle);
    crypto_chk_return(klad_handle != KLAD_VALID_HANDLE, KM_COMPAT_ERRNO(ERROR_INVALID_HANDLE), "invalid klad_handle\n");
    crypto_chk_return(g_klad_ctx.is_open == TD_FALSE, KM_COMPAT_ERRNO(ERROR_CTX_CLOSED), "call create first\n");
    crypto_chk_return(g_klad_ctx.klad_dest != klad_dest, KM_COMPAT_ERRNO(ERROR_INVALID_PARAM), "invalid klad_dest\n");
    crypto_chk_return(g_klad_ctx.keyslot_handle != keyslot_handle, KM_COMPAT_ERRNO(ERROR_INVALID_HANDLE),
        "invalid keyslot_handle\n");

    if (g_klad_ctx.is_attached == TD_FALSE) {
        return TD_SUCCESS;
    }

    g_klad_ctx.is_attached = TD_FALSE;
    g_klad_ctx.keyslot_handle = 0;
    g_klad_ctx.klad_dest = 0;

    return TD_SUCCESS;
}

td_s32 drv_klad_set_attr(td_handle klad_handle, const crypto_klad_attr *klad_attr)
{
    volatile td_s32 ret = TD_FAILURE;
    const crypto_klad_key_config *key_cfg = TD_NULL;
    const crypto_klad_key_secure_config *key_sec_cfg = TD_NULL;
    crypto_drv_func_enter();

    km_null_ptr_chk(klad_attr);
    crypto_chk_return(klad_handle != KLAD_VALID_HANDLE, KM_COMPAT_ERRNO(ERROR_INVALID_HANDLE), "invalid klad_handle\n");
    crypto_chk_return(g_klad_ctx.is_open == TD_FALSE, KM_COMPAT_ERRNO(ERROR_CTX_CLOSED), "call create first\n");

    ret = memcpy_s(&g_klad_ctx.klad_attr, sizeof(crypto_klad_attr), klad_attr, sizeof(crypto_klad_attr));
    crypto_chk_return(ret != EOK, KM_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    key_cfg = &(klad_attr->key_cfg);
    key_sec_cfg = &(klad_attr->key_sec_cfg);

    ret = hal_klad_set_key_crypto_cfg(key_cfg->encrypt_support, key_cfg->decrypt_support, key_cfg->engine);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_klad_set_key_crypto_cfg failed\n");

    ret = hal_klad_set_key_secure_cfg(key_sec_cfg);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_klad_set_key_secure_cfg failed\n");

    g_klad_ctx.hard_key_type = klad_attr->klad_cfg.rootkey_type;
    g_klad_ctx.is_set_attr = TD_TRUE;
    g_klad_ctx.rkp_sw_cfg = klad_attr->rkp_sw_cfg;

    crypto_drv_func_enter();
    return ret;
}

td_s32 drv_klad_get_attr(td_handle klad_handle, crypto_klad_attr *klad_attr)
{
    volatile td_s32 ret = TD_FAILURE;
    crypto_drv_func_enter();

    km_null_ptr_chk(klad_attr);
    crypto_chk_return(klad_handle != KLAD_VALID_HANDLE, KM_COMPAT_ERRNO(ERROR_INVALID_HANDLE), "invalid klad_handle\n");
    crypto_chk_return(g_klad_ctx.is_open == TD_FALSE, KM_COMPAT_ERRNO(ERROR_CTX_CLOSED), "call create first\n");
    crypto_chk_return(g_klad_ctx.is_set_attr == TD_FALSE, KM_COMPAT_ERRNO(ERROR_NOT_SET_CONFIG),
        "call set_attr first\n");

    ret = memcpy_s(klad_attr, sizeof(crypto_klad_attr), &g_klad_ctx.klad_attr, sizeof(crypto_klad_attr));
    crypto_chk_return(ret != EOK, KM_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    crypto_drv_func_enter();
    return ret;
}

td_s32 drv_klad_set_clear_key(td_handle klad_handle, const crypto_klad_clear_key *clear_key)
{
    volatile td_s32 ret = TD_FAILURE;
    crypto_drv_func_enter();

    crypto_chk_return(klad_handle != KLAD_VALID_HANDLE, KM_COMPAT_ERRNO(ERROR_INVALID_HANDLE), "invalid klad_handle\n");
    km_null_ptr_chk(clear_key);
    km_null_ptr_chk(clear_key->key);

    crypto_chk_return(g_klad_ctx.is_open == TD_FALSE, KM_COMPAT_ERRNO(ERROR_CTX_CLOSED), "call create first\n");
    crypto_chk_return(g_klad_ctx.is_set_attr == TD_FALSE, KM_COMPAT_ERRNO(ERROR_NOT_SET_CONFIG),
        "call set_attr first\n");

    ret = hal_klad_start_clr_route(g_klad_ctx.klad_dest, clear_key);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_klad_start_clr_route failed\n");

    g_klad_ctx.is_set_key = TD_TRUE;

    crypto_drv_func_enter();
    return ret;
}

td_s32 drv_klad_set_session_key(td_handle klad_handle, const crypto_klad_session_key *session_key)
{
    volatile td_s32 ret = TD_FAILURE;
    crypto_drv_func_enter();

    crypto_chk_return(klad_handle != KLAD_VALID_HANDLE, KM_COMPAT_ERRNO(ERROR_INVALID_HANDLE), "invalid klad_handle\n");
    km_null_ptr_chk(session_key);

    ret = memcpy_s(&g_klad_ctx.session_key, sizeof (crypto_klad_session_key),
        session_key, sizeof(crypto_klad_session_key));
    crypto_chk_return(ret != EOK, KM_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    g_klad_ctx.is_set_session_key = TD_TRUE;

    crypto_drv_func_enter();
    return 0;
}

static td_s32 inner_drv_set_content_key(const crypto_klad_content_key *content_key)
{
    volatile td_s32 ret = TD_FAILURE;
    td_bool is_odd = content_key->key_parity;
    crypto_klad_key_size key_size = CRYPTO_KLAD_KEY_SIZE_128BIT;

    /* set session key. */
    ret = hal_klad_set_data(g_klad_ctx.session_key.key, CRYPTO_128_KEY_LEN);
    crypto_chk_goto(ret != TD_SUCCESS, exit_clean, "hal_klad_set_data failed\n");

    ret = hal_klad_com_start(CRYPTO_KLAD_LEVEL_SEL_FIRST, g_klad_ctx.session_key.alg,
        CRYPTO_KLAD_KEY_SIZE_128BIT, g_klad_ctx.hard_key_type);
    crypto_chk_goto(ret != TD_SUCCESS, exit_clean, "hal_klad_com_start failed\n");

    ret = hal_klad_wait_com_route_done();
    crypto_chk_goto(ret != TD_SUCCESS, exit_clean, "hal_klad_wait_com_route_done failed\n");

    /*
     * For MCipher:
     * If key_length is 16, the key_parity is passed by caller;
     * If key_length is 32, the high 128bit's key_parity is even,
        the low 128bit's key_parity is odd.
     */
    if (content_key->key_length == CRYPTO_256_KEY_LEN) {
        is_odd = TD_FALSE;
        key_size = CRYPTO_KLAD_KEY_SIZE_256BIT;
    }
    /* config the high 128bit  */
    if (g_klad_ctx.klad_dest == CRYPTO_KLAD_DEST_MCIPHER) {
        hal_klad_set_key_odd(is_odd);
    }

    ret = hal_klad_set_data(content_key->key, CRYPTO_128_KEY_LEN);
    crypto_chk_goto(ret != TD_SUCCESS, exit_clean, "hal_klad_set_data failed\n");

    ret = hal_klad_com_start(CRYPTO_KLAD_LEVEL_SEL_SECOND, g_klad_ctx.session_key.alg,
        key_size, g_klad_ctx.hard_key_type);
    crypto_chk_goto(ret != TD_SUCCESS, exit_clean, "hal_klad_com_start failed\n");

    ret = hal_klad_wait_com_route_done();
    crypto_chk_goto(ret != TD_SUCCESS, exit_clean, "hal_klad_wait_com_route_done failed\n");

    /* config the low 128bit */
    if (content_key->key_length == CRYPTO_256_KEY_LEN) {
        if (g_klad_ctx.klad_dest == CRYPTO_KLAD_DEST_MCIPHER) {
            hal_klad_set_key_odd(TD_TRUE);
        }
        ret = hal_klad_set_data(content_key->key + CRYPTO_128_KEY_LEN, CRYPTO_128_KEY_LEN);
        crypto_chk_goto(ret != TD_SUCCESS, exit_clean, "hal_klad_set_data failed\n");

        ret = hal_klad_com_start(CRYPTO_KLAD_LEVEL_SEL_SECOND, g_klad_ctx.session_key.alg,
            key_size, g_klad_ctx.hard_key_type);
        crypto_chk_goto(ret != TD_SUCCESS, exit_clean, "hal_klad_com_start failed\n");

        ret = hal_klad_wait_com_route_done();
        crypto_chk_goto(ret != TD_SUCCESS, exit_clean, "hal_klad_wait_com_route_done failed\n");
    }
exit_clean:
    hal_klad_clear_data();
    return ret;
}

td_s32 drv_klad_set_content_key(const td_handle klad_handle, const crypto_klad_content_key *content_key)
{
    volatile td_s32 ret = TD_FAILURE;
    crypto_kdf_hard_calc_param param = {0};
    (td_void)(klad_handle);

    km_null_ptr_chk(content_key);
    crypto_chk_return(content_key->key_length != CRYPTO_128_KEY_LEN && content_key->key_length != CRYPTO_256_KEY_LEN,
        KM_COMPAT_ERRNO(ERROR_INVALID_PARAM), "invalid key_length\n");

    (td_void)memset_s(&param, sizeof(crypto_kdf_hard_calc_param), 0, sizeof(crypto_kdf_hard_calc_param));

    /* lock klad */
    ret = hal_klad_lock();
    crypto_chk_return((ret != TD_SUCCESS), ret, "hal_klad_lock failed, ret is 0x%x\n", ret);

    /* lock rkp */
    ret = hal_rkp_lock();
    crypto_chk_goto((ret != TD_SUCCESS), exit_klad_unlock, "hal_rkp_lock failed, ret is 0x%x\n", ret);

    param.hard_key_type = g_klad_ctx.hard_key_type;
    param.hard_alg = CRYPTO_KDF_HARD_ALG_SHA256;        /* default is SHA256. */
    param.rkp_sw_cfg = g_klad_ctx.rkp_sw_cfg;

    ret = hal_rkp_kdf_hard_calculation(&param);
    crypto_chk_goto(ret != TD_SUCCESS, exit_rkp_unlock, "hal_rkp_kdf_hard_calculation failed\n");

    ret = inner_drv_set_content_key(content_key);
    crypto_chk_goto(ret != TD_SUCCESS, exit_rkp_unlock, "inner_drv_set_content_key failed\n");

exit_rkp_unlock:
    hal_rkp_unlock();
exit_klad_unlock:
    hal_klad_unlock();
    return ret;
}