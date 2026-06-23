/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef DRV_HASH_H
#define DRV_HASH_H

#include "crypto_type.h"
#include "crypto_hash_struct.h"
#include "crypto_kdf_struct.h"

td_s32 drv_cipher_hash_init(td_void);

td_s32 drv_cipher_hash_deinit(td_void);

td_s32 drv_cipher_hash_start(td_handle *drv_hash_handle, const crypto_hash_attr *hash_attr);

td_s32 drv_cipher_hash_update(td_handle drv_hash_handle,  const crypto_buf_attr *src_buf, const td_u32 len);

td_s32 drv_cipher_hash_finish(td_handle drv_hash_handle, td_u8 *out, td_u32 *out_len);

td_s32 drv_cipher_hash_get(td_handle drv_hash_handle, crypto_hash_clone_ctx *hash_ctx);

td_s32 drv_cipher_hash_set(td_handle drv_hash_handle, const crypto_hash_clone_ctx *hash_ctx);

td_s32 drv_cipher_hash_destroy(td_handle drv_hash_handle);

td_s32 drv_cipher_pbkdf2(const crypto_kdf_pbkdf2_param *param, td_u8 *out, const td_u32 out_len);

td_s32 drv_cipher_hmac256_block(const td_u8 *input_data, td_u32 data_len, td_u8 *hmac, td_u32 hmac_len);

#endif