/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef KAPI_INNER_H
#define KAPI_INNER_H

#include "crypto_type.h"

td_s32 kapi_cipher_hash_env_init(td_void);

td_s32 kapi_cipher_hash_env_deinit(td_void);

td_s32 kapi_cipher_symc_env_init(td_void);

td_s32 kapi_cipher_symc_env_deinit(td_void);

td_s32 kapi_cipher_trng_env_init(td_void);

td_s32 kapi_cipher_trng_env_deinit(td_void);

td_s32 kapi_cipher_pke_env_init(td_void);

td_s32 kapi_cipher_pke_env_deinit(td_void);

td_s32 kapi_env_init(td_void);

td_void kapi_env_deinit(td_void);

#endif