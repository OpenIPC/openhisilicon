/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "ot_mpi_cipher.h"

#include "crypto_osal_user_lib.h"
#include "uapi_hash.h"

td_s32 ot_mpi_cipher_hash_init(td_void)
{
    return unify_uapi_cipher_hash_init();
}

td_s32 ot_mpi_cipher_hash_deinit(td_void)
{
    return unify_uapi_cipher_hash_deinit();
}

td_s32 ot_mpi_cipher_hash_create(td_handle *mpi_hash_handle, const crypto_hash_attr *hash_attr)
{
    return unify_uapi_cipher_hash_start(mpi_hash_handle, hash_attr);
}

td_s32 ot_mpi_cipher_hash_update(td_handle mpi_hash_handle, const crypto_buf_attr *src_buf, const td_u32 len)
{
    return unify_uapi_cipher_hash_update(mpi_hash_handle, src_buf, len);
}

td_s32 ot_mpi_cipher_hash_finish(td_handle mpi_hash_handle, td_u8 *virt_addr, td_u32 buffer_len, td_u32 *result_len)
{
    td_s32 ret;
    crypto_check_param_null(ERROR_LAYER_UAPI, ERROR_MODULE_HASH, virt_addr);
    crypto_check_param_null(ERROR_LAYER_UAPI, ERROR_MODULE_HASH, result_len);
    *result_len = buffer_len;
    ret = unify_uapi_cipher_hash_finish(mpi_hash_handle, virt_addr, result_len);
    return ret;
}

td_s32 ot_mpi_cipher_hash_get(td_handle mpi_hash_handle, crypto_hash_clone_ctx *hash_clone_ctx)
{
    return unify_uapi_cipher_hash_get(mpi_hash_handle, hash_clone_ctx);
}

td_s32 ot_mpi_cipher_hash_set(td_handle mpi_hash_handle, const crypto_hash_clone_ctx *hash_clone_ctx)
{
    return unify_uapi_cipher_hash_set(mpi_hash_handle, hash_clone_ctx);
}

td_s32 ot_mpi_cipher_hash_destroy(td_handle mpi_hash_handle)
{
    return unify_uapi_cipher_hash_destroy(mpi_hash_handle);
}
