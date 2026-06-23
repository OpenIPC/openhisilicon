/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "drv_keyslot.h"
#include "hal_keyslot.h"

#include "crypto_drv_common.h"
#include "crypto_common_macro.h"

#define EXT_ID_KM      0x55

#define id_2_handle(id, key)    \
    (td_handle)((((EXT_ID_KM) & 0xff) << 24) | ((((key) & 0xff)<< 16)) | (((id) & 0xffff)))
#define handle_2_modid(handle)      (((handle) >> 24) & 0xff)
#define handle_2_id(handle)         (((handle)) & 0xffff)
#define handle_get_type(handle)     (((handle) >> 16) & 0xff)

#define KEYSLOT_MCIPHER_KEYSLOT_CNT     8
#define KEYSLOT_HMAC_KEYSLOT_CNT        2

#define KM_COMPAT_ERRNO(err_code)     DRV_COMPAT_ERRNO(ERROR_MODULE_KM, err_code)
#define km_null_ptr_chk(ptr)   \
    crypto_chk_return((ptr) == TD_NULL, KM_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), #ptr" is NULL\n")

td_s32 drv_keyslot_init(td_void)
{
    return TD_SUCCESS;
}

td_s32 drv_keyslot_deinit(td_void)
{
    return TD_SUCCESS;
}

td_s32 drv_keyslot_create(td_handle *keyslot_handle, crypto_keyslot_type keyslot_type)
{
    volatile td_s32 ret = TD_FAILURE;
    td_u32 i;
    td_u32 keyslot_cnt = 0;
    km_null_ptr_chk(keyslot_handle);

    if (keyslot_type == CRYPTO_KEYSLOT_TYPE_MCIPHER) {
        keyslot_cnt = KEYSLOT_MCIPHER_KEYSLOT_CNT;
    } else if (keyslot_type == CRYPTO_KEYSLOT_TYPE_HMAC) {
        keyslot_cnt = KEYSLOT_HMAC_KEYSLOT_CNT;
    } else {
        crypto_log_err("invalid keyslot_type\n");
        return KM_COMPAT_ERRNO(ERROR_INVALID_PARAM);
    }

    for (i = 0; i < keyslot_cnt; i++) {
        ret = hal_keyslot_lock(i, keyslot_type);
        if (ret == TD_SUCCESS) {
            break;
        }
    }
    if (i >= keyslot_cnt) {
        crypto_log_err("all keyslot channels are busy\n");
        return KM_COMPAT_ERRNO(ERROR_CHN_BUSY);
    }

    *keyslot_handle = id_2_handle(i, (td_u32)keyslot_type);
    return ret;
}

td_s32 drv_keyslot_destroy(td_handle keyslot_handle)
{
    crypto_keyslot_type keyslot_type_get = handle_get_type(keyslot_handle);

    return hal_keyslot_unlock(handle_2_id(keyslot_handle), keyslot_type_get);
}