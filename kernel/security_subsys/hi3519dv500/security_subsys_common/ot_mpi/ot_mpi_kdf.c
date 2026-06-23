/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "ot_mpi_cipher.h"

#include "uapi_kdf.h"

td_s32 ot_mpi_cipher_pbkdf2(const crypto_kdf_pbkdf2_param *param, td_u8 *out, const td_u32 out_len)
{
    return unify_uapi_cipher_pbkdf2(param, out, out_len);
}