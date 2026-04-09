/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : drivers for cipher compat.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#include "drv_osal_lib.h"
#include "drv_symc.h"
#include "drv_klad.h"

hi_s32 klad_load_hard_key(hi_u32 handle, hi_u32 ca_type, hi_u8 *key, hi_u32 key_len)
{
    hi_s32 ret;

    ret = drv_cipher_klad_load_key(handle, ca_type, HI_CIPHER_KLAD_TARGET_AES, key, key_len);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_cipher_klad_load_key, ret);
        return ret;
    }

    return HI_SUCCESS;
}

hi_s32 klad_encrypt_key(hi_u32 keysel, hi_u32 target, hi_u8 *clear, hi_u8 *encrypt, hi_u32 key_len)
{
    hi_s32 ret;

    ret = drv_cipher_klad_encrypt_key(keysel, target, clear, encrypt, key_len);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_cipher_klad_encrypt_key, ret);
        return ret;
    }

    return HI_SUCCESS;
}

hi_s32 hi_drv_compat_init(void)
{
    hi_s32 ret;

    ret = drv_cipher_klad_init();
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(drv_cipher_klad_init, ret);
        return ret;
    }

    return HI_SUCCESS;
}

hi_s32 hi_drv_compat_deinit(void)
{
    drv_cipher_klad_deinit();

    return HI_SUCCESS;
}

