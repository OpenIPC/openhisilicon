/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "dispatch_km.h"
#include "crypto_common_macro.h"
#include "kapi_km.h"

#define KM_COMPAT_ERRNO(err_code)     DISPATCH_COMPAT_ERRNO(ERROR_MODULE_KM, err_code)
#define km_null_ptr_chk(ptr)   \
    crypto_chk_return((ptr) == TD_NULL, KM_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), #ptr" is NULL\n")

static td_s32 dispatch_keyslot_create(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    keyslot_create_ctl_t *keyslot_create_t = TD_NULL;
    td_handle keyslot_handle;
    crypto_dispatch_func_enter();

    crypto_unused(cmd);
    crypto_unused(private_data);

    keyslot_create_t = (keyslot_create_ctl_t *)argp;
    ret = kapi_keyslot_create(&keyslot_handle, keyslot_create_t->keyslot_type);
    crypto_chk_return(ret != TD_SUCCESS, ret, "kapi_keyslot_create failed, ret is 0x%x\n", ret);
    keyslot_create_t->kapi_keyslot_handle = (crypto_handle)keyslot_handle;
    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_keyslot_destroy(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    keyslot_destroy_ctl_t *keyslot_destroy_t = TD_NULL;
    crypto_dispatch_func_enter();

    crypto_unused(cmd);
    crypto_unused(private_data);

    keyslot_destroy_t = (keyslot_destroy_ctl_t *)argp;
    ret = kapi_keyslot_destroy(keyslot_destroy_t->kapi_keyslot_handle);
    crypto_chk_return(ret != TD_SUCCESS, ret, "kapi_keyslot_destroy failed, ret is 0x%x\n", ret);

    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_klad_create(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    klad_handle_ctl_t *klad_handle_ctl = TD_NULL;
    td_handle klad_handle;
    crypto_dispatch_func_enter();

    crypto_unused(cmd);
    crypto_unused(private_data);

    klad_handle_ctl = (klad_handle_ctl_t *)argp;
    ret = kapi_klad_create(&klad_handle);
    crypto_chk_return(ret != TD_SUCCESS, ret, "kapi_keyslot_create failed, ret is 0x%x\n", ret);
    klad_handle_ctl->kapi_klad_handle = (crypto_handle)klad_handle;
    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_klad_destroy(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    klad_handle_ctl_t *klad_handle_ctl = TD_NULL;
    crypto_dispatch_func_enter();

    crypto_unused(cmd);
    crypto_unused(private_data);

    klad_handle_ctl = (klad_handle_ctl_t *)argp;
    ret = kapi_klad_destroy(klad_handle_ctl->kapi_klad_handle);
    crypto_chk_return(ret != TD_SUCCESS, ret, "kapi_klad_destroy failed, ret is 0x%x\n", ret);

    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_klad_set_attr(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    klad_attr_ctl_t *klad_attr_ctl = TD_NULL;
    crypto_dispatch_func_enter();

    crypto_unused(cmd);
    crypto_unused(private_data);

    klad_attr_ctl = (klad_attr_ctl_t *)argp;
    ret = kapi_klad_set_attr(klad_attr_ctl->kapi_klad_handle, &klad_attr_ctl->attr);
    crypto_chk_return(ret != TD_SUCCESS, ret, "kapi_klad_set_attr failed, ret is 0x%x\n", ret);

    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_klad_get_attr(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    klad_attr_ctl_t *klad_attr_ctl = TD_NULL;
    crypto_dispatch_func_enter();

    crypto_unused(cmd);
    crypto_unused(private_data);

    klad_attr_ctl = (klad_attr_ctl_t *)argp;

    ret = kapi_klad_get_attr(klad_attr_ctl->kapi_klad_handle, &klad_attr_ctl->attr);
    crypto_chk_return(ret != TD_SUCCESS, ret, "kapi_klad_get_attr failed, ret is 0x%x\n", ret);

    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_klad_attach(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    klad_attach_ctl_t *klad_attach_ctl = TD_NULL;
    crypto_dispatch_func_enter();

    crypto_unused(cmd);
    crypto_unused(private_data);

    klad_attach_ctl = (klad_attach_ctl_t *)argp;
    ret = kapi_klad_attach(klad_attach_ctl->kapi_klad_handle, klad_attach_ctl->klad_type,
        klad_attach_ctl->kapi_keyslot_handle);
    crypto_chk_return(ret != TD_SUCCESS, ret, "kapi_klad_attach failed, ret is 0x%x\n", ret);

    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_klad_detach(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    klad_attach_ctl_t *klad_attach_ctl = TD_NULL;
    crypto_dispatch_func_enter();

    crypto_unused(cmd);
    crypto_unused(private_data);

    klad_attach_ctl = (klad_attach_ctl_t *)argp;
    ret = kapi_klad_detach(klad_attach_ctl->kapi_klad_handle, klad_attach_ctl->klad_type,
        klad_attach_ctl->kapi_keyslot_handle);
    crypto_chk_return(ret != TD_SUCCESS, ret, "kapi_klad_detach failed, ret is 0x%x\n", ret);

    crypto_dispatch_func_exit();
    return ret;
}

#define MAX_KEY_LEN     128
static td_s32 dispatch_klad_set_clear_key(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    klad_set_clear_key_ctl_t *klad_set_clear_key_ctl = TD_NULL;
    td_u8 key[MAX_KEY_LEN] = {0};
    km_klad_clear_key clear_key = {0};
    crypto_dispatch_func_enter();

    crypto_unused(cmd);
    crypto_unused(private_data);

    klad_set_clear_key_ctl = (klad_set_clear_key_ctl_t *)argp;

    crypto_chk_return(klad_set_clear_key_ctl->key_size > MAX_KEY_LEN, KM_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "Unsupported key_len, the max lenth of the key is: %d\n", MAX_KEY_LEN);
    crypto_param_require(klad_set_clear_key_ctl->key.p != TD_NULL);

    ret = crypto_copy_from_user(key, MAX_KEY_LEN, klad_set_clear_key_ctl->key.p,  \
        klad_set_clear_key_ctl->key_size);
    crypto_chk_return(ret != TD_SUCCESS, ret, "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    clear_key.key = key;
    clear_key.hmac_type = klad_set_clear_key_ctl->hmac_type;
    clear_key.key_size = klad_set_clear_key_ctl->key_size;
    clear_key.key_parity = klad_set_clear_key_ctl->key_parity;

    ret = kapi_klad_set_clear_key(klad_set_clear_key_ctl->kapi_klad_handle, &clear_key);
    crypto_chk_goto(ret != TD_SUCCESS, exit, "kapi_klad_set_clear_key failed, ret is 0x%x\n", ret);

exit:
    (td_void)memset_s(key, MAX_KEY_LEN, 0, MAX_KEY_LEN);
    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_klad_set_session_key(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    klad_set_session_key_ctl_t *klad_set_session_key_ctl = TD_NULL;
    td_u8 key[MAX_KEY_LEN] = {0};
    km_klad_session_key session_key = {0};
    crypto_dispatch_func_enter();

    crypto_unused(cmd);
    crypto_unused(private_data);

    klad_set_session_key_ctl = (klad_set_session_key_ctl_t *)argp;

    crypto_chk_return(klad_set_session_key_ctl->key_size > MAX_KEY_LEN, KM_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "Unsupported key_len, the max lenth of the key is: %d\n", MAX_KEY_LEN);
    crypto_param_require(klad_set_session_key_ctl->key.p != TD_NULL);

    ret = crypto_copy_from_user(key, MAX_KEY_LEN, klad_set_session_key_ctl->key.p,  \
        klad_set_session_key_ctl->key_size);
    crypto_chk_return(ret != TD_SUCCESS, ret, "crypto_copy_from_user failed, ret is 0x%x\n", ret);
    session_key.key = key;
    session_key.alg = klad_set_session_key_ctl->alg;
    session_key.key_size = klad_set_session_key_ctl->key_size;
    session_key.level = klad_set_session_key_ctl->level;

    ret = kapi_klad_set_session_key(klad_set_session_key_ctl->kapi_klad_handle, &session_key);
    crypto_chk_goto(ret != TD_SUCCESS, exit, "kapi_klad_set_session_key failed, ret is 0x%x\n", ret);

exit:
    (td_void)memset_s(key, MAX_KEY_LEN, 0, MAX_KEY_LEN);
    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_klad_set_content_key(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    klad_set_content_key_ctl_t *klad_set_content_key_ctl = TD_NULL;
    td_u8 key[MAX_KEY_LEN] = {0};
    km_klad_content_key content_key = {0};
    crypto_dispatch_func_enter();

    crypto_unused(cmd);
    crypto_unused(private_data);

    klad_set_content_key_ctl = (klad_set_content_key_ctl_t *)argp;

    crypto_chk_return(klad_set_content_key_ctl->key_size > MAX_KEY_LEN, KM_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "Unsupported key_len, the max lenth of the key is: %d\n", MAX_KEY_LEN);
    crypto_param_require(klad_set_content_key_ctl->key.p != TD_NULL);

    ret = crypto_copy_from_user(key, MAX_KEY_LEN, klad_set_content_key_ctl->key.p,  \
        klad_set_content_key_ctl->key_size);
    crypto_chk_return(ret != TD_SUCCESS, ret, "crypto_copy_from_user failed, ret is 0x%x\n", ret);

    content_key.key = key;
    content_key.alg = klad_set_content_key_ctl->alg;
    content_key.key_size = klad_set_content_key_ctl->key_size;
    content_key.key_parity = klad_set_content_key_ctl->key_parity;

    ret = kapi_klad_set_content_key(klad_set_content_key_ctl->kapi_klad_handle, &content_key);
    crypto_chk_goto(ret != TD_SUCCESS, exit, "kapi_klad_set_content_key failed, ret is 0x%x\n", ret);

exit:
    (td_void)memset_s(key, MAX_KEY_LEN, 0, MAX_KEY_LEN);
    crypto_dispatch_func_exit();
    return ret;
}

static crypto_ioctl_cmd g_km_func_list[] = {
    {CMD_KEYSLOT_CREATE_HANDLE,             dispatch_keyslot_create},
    {CMD_KEYSLOT_DESTROY_HANDLE,            dispatch_keyslot_destroy},
    {CMD_KLAD_CREATE_HANDLE,                dispatch_klad_create},
    {CMD_KLAD_DESTROY_HANDLE,               dispatch_klad_destroy},
    {CMD_KLAD_ATTACH,                       dispatch_klad_attach},
    {CMD_KLAD_DETACH,                       dispatch_klad_detach},
    {CMD_KLAD_SET_ATTR,                     dispatch_klad_set_attr},
    {CMD_KLAD_GET_ATTR,                     dispatch_klad_get_attr},
    {CMD_KLAD_SET_SESSION_KEY,              dispatch_klad_set_session_key},
    {CMD_KLAD_SET_CONTENT_KEY,              dispatch_klad_set_content_key},
    {CMD_KLAD_SET_CLEAR_KEY,                dispatch_klad_set_clear_key},
};

crypto_ioctl_cmd *get_km_func_list(td_void)
{
    return g_km_func_list;
}

td_u32 get_km_func_num(td_void)
{
    return crypto_array_size(g_km_func_list);
}
