/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "uapi_kdf.h"

#include <securec.h>

#include "uapi_common.h"
#include "crypto_ioctl_cmd.h"
#include "crypto_osal_user_lib.h"

#define HASH_COMPAT_ERRNO(err_code)     UAPI_COMPAT_ERRNO(ERROR_MODULE_HASH, err_code)

td_s32 unify_uapi_cipher_pbkdf2(const crypto_kdf_pbkdf2_param *param, td_u8 *out, const td_u32 out_len)
{
    td_s32 ret = TD_SUCCESS;
    pbkdf2_ctl_t pbkdf2_ctl;
    crypto_uapi_func_enter();

    crypto_chk_return(param == TD_NULL, HASH_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "param is NULL\n");

    (td_void)memset_s(&pbkdf2_ctl, sizeof(pbkdf2_ctl), 0, sizeof(pbkdf2_ctl));
    pbkdf2_ctl.hash_type = param->hash_type;
    pbkdf2_ctl.password.p = param->password;
    pbkdf2_ctl.salt.p = param->salt;
    pbkdf2_ctl.plen = param->plen;
    pbkdf2_ctl.slen = param->slen;
    pbkdf2_ctl.count = param->count;
    pbkdf2_ctl.out.p = out;
    pbkdf2_ctl.out_len = out_len;

    ret = crypto_cipher_open();
    crypto_chk_return(ret != TD_SUCCESS, ret, "crypto_cipher_open failed\n");

    ret = crypto_cipher_ioctl(CRYPTO_CMD_PBKDF2, &pbkdf2_ctl);
    crypto_chk_goto(ret != TD_SUCCESS, exit_close, "crypto_ioctl failed, ret is 0x%x\n", ret);

exit_close:
    crypto_cipher_close();
    crypto_uapi_func_exit();
    return ret;
}