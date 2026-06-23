/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "uapi_km.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <securec.h>

#include "ot_type.h"
#include "crypto_osal_user_lib.h"
#include "crypto_km_struct.h"
#include "crypto_errno.h"
#include "ioctl_km.h"
#include "crypto_common_macro.h"

#define REF_COUNT_MAX_NUM     0x7FFFFFFF
#define KM_COMPAT_ERRNO(err_code)     UAPI_COMPAT_ERRNO(ERROR_MODULE_KM, err_code)
#define km_null_ptr_chk(ptr)   \
    crypto_chk_return((ptr) == TD_NULL, KM_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), #ptr" is NULL\n")

typedef struct mpi_mgnt {
    crypto_mutex_t      lock;
    td_s32              ref_count;
    td_s32              dev_fd;
} mpi_mgnt_t;

static mpi_mgnt_t km_mgnt = {
    .lock = CRYPTO_PTHREAD_MUTEX_INITIALIZER,
    .ref_count = -1,
    .dev_fd = -1
};

static td_bool inner_km_is_init(td_void)
{
    if (km_mgnt.ref_count >= 0) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

td_s32 uapi_km_init(td_void)
{
    td_s32 ret = TD_SUCCESS;
    crypto_uapi_func_enter();
    crypto_pthread_mutex_lock(&km_mgnt.lock);

    if (km_mgnt.ref_count >= 0) {
        if (km_mgnt.ref_count < REF_COUNT_MAX_NUM) {
            km_mgnt.ref_count++;
            goto exit;
        }
    }
    km_mgnt.dev_fd = crypto_open("/dev/km", O_RDWR, 0);
    if (km_mgnt.dev_fd < 0) {
        printf("open /dev/km failed\n");
        ret = KM_COMPAT_ERRNO(ERROR_DEV_OPEN_FAILED);
        goto exit;
    }

    km_mgnt.ref_count = 0;

exit:
    crypto_pthread_mutex_unlock(&km_mgnt.lock);
    crypto_uapi_func_exit();
    return ret;
}

td_s32 uapi_km_deinit(td_void)
{
    td_s32 ret = TD_SUCCESS;
    crypto_uapi_func_enter();
    crypto_pthread_mutex_lock(&km_mgnt.lock);

    if (km_mgnt.ref_count > 0) {
        km_mgnt.ref_count--;
        goto exit;
    }

    if (km_mgnt.ref_count == 0) {
        km_mgnt.ref_count--;
        crypto_close(km_mgnt.dev_fd);
        km_mgnt.dev_fd = -1;
    }

exit:
    crypto_pthread_mutex_unlock(&km_mgnt.lock);
    crypto_uapi_func_exit();
    return ret;
}

/* Keyslot. */
td_s32 uapi_keyslot_create(crypto_handle *mpi_keyslot_handle, km_keyslot_type keyslot_type)
{
    td_s32 ret = TD_SUCCESS;
    keyslot_create_ctl_t keyslot_create_ctl = {0};

    crypto_uapi_func_enter();
    crypto_chk_return(inner_km_is_init() != TD_TRUE, KM_COMPAT_ERRNO(ERROR_NOT_INIT), "KM not init\n");
    km_null_ptr_chk(mpi_keyslot_handle);

    keyslot_create_ctl.keyslot_type = keyslot_type;
    ret = crypto_km_ioctl(km_mgnt.dev_fd, CMD_KEYSLOT_CREATE_HANDLE, &keyslot_create_ctl);
    crypto_chk_return(ret != TD_SUCCESS, ret, "crypto_km_ioctl failed, ret is 0x%x\n", ret);

    *mpi_keyslot_handle = keyslot_create_ctl.kapi_keyslot_handle;

    crypto_uapi_func_exit();
    return ret;
}

td_s32 uapi_keyslot_destroy(crypto_handle mpi_keyslot_handle)
{
    td_s32 ret = TD_SUCCESS;

    keyslot_destroy_ctl_t keyslot_destroy_ctl = {0};

    crypto_uapi_func_enter();
    crypto_chk_return(inner_km_is_init() != TD_TRUE, KM_COMPAT_ERRNO(ERROR_NOT_INIT), "KM not init\n");

    keyslot_destroy_ctl.kapi_keyslot_handle = mpi_keyslot_handle;
    ret = crypto_km_ioctl(km_mgnt.dev_fd, CMD_KEYSLOT_DESTROY_HANDLE, &keyslot_destroy_ctl);
    crypto_chk_return(ret != TD_SUCCESS, ret, "crypto_km_ioctl failed, ret is 0x%x\n", ret);

    crypto_uapi_func_exit();
    return ret;
}

/* Klad. */
td_s32 uapi_klad_create(crypto_handle *mpi_klad_handle)
{
    td_s32 ret = TD_SUCCESS;
    klad_handle_ctl_t klad_handle_ctl = {0};

    crypto_uapi_func_enter();
    crypto_chk_return(inner_km_is_init() != TD_TRUE, KM_COMPAT_ERRNO(ERROR_NOT_INIT), "KM not init\n");
    km_null_ptr_chk(mpi_klad_handle);

    ret = crypto_km_ioctl(km_mgnt.dev_fd, CMD_KLAD_CREATE_HANDLE, &klad_handle_ctl);
    crypto_chk_return(ret != TD_SUCCESS, ret, "crypto_km_ioctl failed, ret is 0x%x\n", ret);

    *mpi_klad_handle = klad_handle_ctl.kapi_klad_handle;

    crypto_uapi_func_exit();
    return ret;
}

td_s32 uapi_klad_destroy(crypto_handle mpi_klad_handle)
{
    td_s32 ret = TD_SUCCESS;
    klad_handle_ctl_t klad_handle_ctl = {0};

    crypto_uapi_func_enter();
    crypto_chk_return(inner_km_is_init() != TD_TRUE, KM_COMPAT_ERRNO(ERROR_NOT_INIT), "KM not init\n");

    klad_handle_ctl.kapi_klad_handle = mpi_klad_handle;
    ret = crypto_km_ioctl(km_mgnt.dev_fd, CMD_KLAD_DESTROY_HANDLE, &klad_handle_ctl);
    crypto_chk_print(ret != TD_SUCCESS, "crypto_km_ioctl failed, ret is 0x%x\n", ret);

    crypto_uapi_func_exit();
    return ret;
}

td_s32 uapi_klad_attach(crypto_handle mpi_klad_handle, km_klad_dest_type klad_type,
    crypto_handle mpi_keyslot_handle)
{
    td_s32 ret = TD_SUCCESS;
    klad_attach_ctl_t klad_attach_ctl = {0};

    crypto_uapi_func_enter();
    crypto_chk_return(inner_km_is_init() != TD_TRUE, KM_COMPAT_ERRNO(ERROR_NOT_INIT), "KM not init\n");

    klad_attach_ctl.kapi_klad_handle = mpi_klad_handle;
    klad_attach_ctl.kapi_keyslot_handle =mpi_keyslot_handle;
    klad_attach_ctl.klad_type = klad_type;

    ret = crypto_km_ioctl(km_mgnt.dev_fd, CMD_KLAD_ATTACH, &klad_attach_ctl);
    crypto_chk_return(ret != TD_SUCCESS, ret, "crypto_km_ioctl failed, ret is 0x%x\n", ret);

    crypto_uapi_func_exit();
    return ret;
}

td_s32 uapi_klad_detach(crypto_handle mpi_klad_handle, km_klad_dest_type klad_type,
    crypto_handle mpi_keyslot_handle)
{
    td_s32 ret = TD_SUCCESS;

    klad_attach_ctl_t klad_attach_ctl = {0};

    crypto_uapi_func_enter();
    crypto_chk_return(inner_km_is_init() != TD_TRUE, KM_COMPAT_ERRNO(ERROR_NOT_INIT), "KM not init\n");

    klad_attach_ctl.kapi_klad_handle = mpi_klad_handle;
    klad_attach_ctl.kapi_keyslot_handle = mpi_keyslot_handle;
    klad_attach_ctl.klad_type = klad_type;

    ret = crypto_km_ioctl(km_mgnt.dev_fd, CMD_KLAD_DETACH, &klad_attach_ctl);
    crypto_chk_return(ret != TD_SUCCESS, ret, "crypto_km_ioctl failed, ret is 0x%x\n", ret);

    crypto_uapi_func_exit();
    return ret;
}

td_s32 uapi_klad_set_attr(crypto_handle mpi_klad_handle, const km_klad_attr *attr)
{
    td_s32 ret = TD_SUCCESS;
    klad_attr_ctl_t klad_attr_ctl = {0};

    crypto_uapi_func_enter();
    crypto_chk_return(inner_km_is_init() != TD_TRUE, KM_COMPAT_ERRNO(ERROR_NOT_INIT), "KM not init\n");
    km_null_ptr_chk(attr);

    klad_attr_ctl.kapi_klad_handle = mpi_klad_handle;
    (td_void)memcpy_s(&klad_attr_ctl.attr, sizeof(km_klad_attr), attr, sizeof(km_klad_attr));
    ret = crypto_km_ioctl(km_mgnt.dev_fd, CMD_KLAD_SET_ATTR, &klad_attr_ctl);
    crypto_chk_return(ret != TD_SUCCESS, ret, "crypto_km_ioctl failed, ret is 0x%x\n", ret);

    crypto_uapi_func_exit();
    return ret;
}

td_s32 uapi_klad_get_attr(crypto_handle mpi_klad_handle, km_klad_attr *attr)
{
    td_s32 ret = TD_SUCCESS;
    klad_attr_ctl_t klad_attr_ctl = {0};

    crypto_uapi_func_enter();
    crypto_chk_return(inner_km_is_init() != TD_TRUE, KM_COMPAT_ERRNO(ERROR_NOT_INIT), "KM not init\n");
    km_null_ptr_chk(attr);

    klad_attr_ctl.kapi_klad_handle = mpi_klad_handle;
    ret = crypto_km_ioctl(km_mgnt.dev_fd, CMD_KLAD_GET_ATTR, &klad_attr_ctl);
    crypto_chk_return(ret != TD_SUCCESS, ret, "crypto_km_ioctl failed, ret is 0x%x\n", ret);
    (td_void)memcpy_s(attr, sizeof(km_klad_attr), &klad_attr_ctl.attr, sizeof(km_klad_attr));

    crypto_uapi_func_exit();
    return ret;
}

td_s32 uapi_klad_set_session_key(crypto_handle mpi_klad_handle, const km_klad_session_key *key)
{
    td_s32 ret = TD_SUCCESS;
    klad_set_session_key_ctl_t klad_set_session_key_ctl = {0};

    crypto_uapi_func_enter();
    crypto_chk_return(inner_km_is_init() != TD_TRUE, KM_COMPAT_ERRNO(ERROR_NOT_INIT), "KM not init\n");
    km_null_ptr_chk(key);
    km_null_ptr_chk(key->key);

    klad_set_session_key_ctl.kapi_klad_handle = mpi_klad_handle;
    klad_set_session_key_ctl.alg = key->alg;
    klad_set_session_key_ctl.level = key->level;
    klad_set_session_key_ctl.key_size = key->key_size;
    klad_set_session_key_ctl.key.p = key->key;
    ret = crypto_km_ioctl(km_mgnt.dev_fd, CMD_KLAD_SET_SESSION_KEY, &klad_set_session_key_ctl);
    crypto_chk_return(ret != TD_SUCCESS, ret, "crypto_km_ioctl failed, ret is 0x%x\n", ret);

    crypto_uapi_func_exit();
    return ret;
}

td_s32 uapi_klad_set_content_key(crypto_handle mpi_klad_handle, const km_klad_content_key *key)
{
    td_s32 ret = TD_SUCCESS;
    klad_set_content_key_ctl_t klad_set_content_key_ctl = {0};

    crypto_uapi_func_enter();
    crypto_chk_return(inner_km_is_init() != TD_TRUE, KM_COMPAT_ERRNO(ERROR_NOT_INIT), "KM not init\n");
    km_null_ptr_chk(key);
    km_null_ptr_chk(key->key);

    klad_set_content_key_ctl.kapi_klad_handle = mpi_klad_handle;
    klad_set_content_key_ctl.alg = key->alg;
    klad_set_content_key_ctl.key_parity = key->key_parity;
    klad_set_content_key_ctl.key_size = key->key_size;
    klad_set_content_key_ctl.key.p = key->key;
    ret = crypto_km_ioctl(km_mgnt.dev_fd, CMD_KLAD_SET_CONTENT_KEY, &klad_set_content_key_ctl);
    crypto_chk_return(ret != TD_SUCCESS, ret, "crypto_km_ioctl failed, ret is 0x%x\n", ret);

    crypto_uapi_func_exit();
    return ret;
}

td_s32 uapi_klad_set_clear_key(crypto_handle mpi_klad_handle, const km_klad_clear_key *key)
{
    td_s32 ret = TD_SUCCESS;
    klad_set_clear_key_ctl_t klad_set_clear_key_ctl = {0};

    crypto_uapi_func_enter();
    crypto_chk_return(inner_km_is_init() != TD_TRUE, KM_COMPAT_ERRNO(ERROR_NOT_INIT), "KM not init\n");
    km_null_ptr_chk(key);
    km_null_ptr_chk(key->key);

    klad_set_clear_key_ctl.kapi_klad_handle = mpi_klad_handle;
    klad_set_clear_key_ctl.hmac_type = key->hmac_type;
    klad_set_clear_key_ctl.key_parity = key->key_parity;
    klad_set_clear_key_ctl.key_size = key->key_size;
    klad_set_clear_key_ctl.key.p = key->key;
    ret = crypto_km_ioctl(km_mgnt.dev_fd, CMD_KLAD_SET_CLEAR_KEY, &klad_set_clear_key_ctl);
    crypto_chk_return(ret != TD_SUCCESS, ret, "crypto_km_ioctl failed, ret is 0x%x\n", ret);

    crypto_uapi_func_exit();
    return ret;
}