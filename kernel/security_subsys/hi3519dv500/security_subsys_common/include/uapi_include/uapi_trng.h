/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef UAPI_TRNG_H
#define UAPI_TRNG_H

#include "crypto_type.h"

td_s32 unify_uapi_cipher_trng_get_random(td_u32 *randnum);

td_s32 unify_uapi_cipher_trng_get_multi_random(td_u32 size, td_u8 *randnum);

#endif