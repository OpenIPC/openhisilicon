/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "ot_mpi_cipher.h"

#include "uapi_trng.h"

td_s32 ot_mpi_cipher_trng_get_random(td_u32 *randnum)
{
    return unify_uapi_cipher_trng_get_random(randnum);
}

td_s32 ot_mpi_cipher_trng_get_multi_random(td_u32 size, td_u8 *randnum)
{
    return unify_uapi_cipher_trng_get_multi_random(size, randnum);
}