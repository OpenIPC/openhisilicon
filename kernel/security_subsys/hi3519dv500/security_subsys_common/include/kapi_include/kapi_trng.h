/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef KAPI_TRNG_H
#define KAPI_TRNG_H

#include "crypto_type.h"

td_s32 kapi_cipher_trng_get_random(td_u32 *randnum);

td_s32 kapi_cipher_trng_get_multi_random(td_u32 size, td_u8 *randnum);

#endif