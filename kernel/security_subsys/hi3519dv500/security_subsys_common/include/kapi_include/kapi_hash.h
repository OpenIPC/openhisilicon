/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef KAPI_HASH_H
#define KAPI_HASH_H

#include "crypto_type.h"
#include "crypto_hash_struct.h"
#include "crypto_kdf_struct.h"
#include "crypto_drv_common.h"

#define CRYPTO_HASH_VIRT_CHN_NUM    32
#define MAX_PROCESS_NUM             10

typedef struct {
    td_u32 tid;
    td_bool is_open;
    td_bool is_long_term;
    td_handle drv_hash_handle;
    crypto_hash_clone_ctx hash_clone_ctx;
} crypto_kapi_hash_ctx;

typedef struct {
    td_u32 pid;
    crypto_owner owner;
    crypto_kapi_hash_ctx hash_ctx_list[CRYPTO_HASH_VIRT_CHN_NUM];
    crypto_mutex hash_ctx_mutex[CRYPTO_HASH_VIRT_CHN_NUM];
    td_u32 ctx_num;
    td_u32 init_counter;
    td_bool is_used;
} crypto_kapi_hash_process;

td_void kapi_cipher_hash_process_release(td_void);

td_s32 kapi_cipher_hash_init(td_void);

td_s32 kapi_cipher_hash_deinit(td_void);

td_s32 kapi_cipher_hash_start(td_handle *kapi_hash_handle, const crypto_hash_attr *hash_attr);

td_s32 kapi_cipher_hash_update(td_handle kapi_hash_handle,  const crypto_buf_attr *src_buf, const td_u32 len);

td_s32 kapi_cipher_hash_finish(td_handle kapi_hash_handle, td_u8 *out, td_u32 *out_len);

td_s32 kapi_cipher_hash_get(td_handle kapi_hash_handle, crypto_hash_clone_ctx *hash_clone_ctx);

td_s32 kapi_cipher_hash_set(td_handle kapi_hash_handle, const crypto_hash_clone_ctx *hash_clone_ctx);

td_s32 kapi_cipher_hash_destroy(td_handle kapi_hash_handle);

td_s32 kapi_cipher_pbkdf2(const crypto_kdf_pbkdf2_param *param, td_u8 *out, const td_u32 out_len);

#endif