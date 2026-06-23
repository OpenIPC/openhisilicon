/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "ot_mpi_km.h"

#include "crypto_km_struct.h"
#include "uapi_km.h"

/* Init */
td_s32 ot_mpi_km_init(td_void)
{
    return uapi_km_init();
}

td_s32 ot_mpi_km_deinit(td_void)
{
    return uapi_km_deinit();
}

/* Keyslot. */
td_s32 ot_mpi_keyslot_create(crypto_handle *mpi_keyslot_handle, km_keyslot_type keyslot_type)
{
    return uapi_keyslot_create(mpi_keyslot_handle, keyslot_type);
}

td_s32 ot_mpi_keyslot_destroy(crypto_handle mpi_keyslot_handle)
{
    return uapi_keyslot_destroy(mpi_keyslot_handle);
}

/* Klad. */
td_s32 ot_mpi_klad_create(crypto_handle *mpi_klad_handle)
{
    return uapi_klad_create(mpi_klad_handle);
}

td_s32 ot_mpi_klad_destroy(crypto_handle mpi_klad_handle)
{
    return uapi_klad_destroy(mpi_klad_handle);
}

td_s32 ot_mpi_klad_attach(crypto_handle mpi_klad_handle, km_klad_dest_type klad_type,
    crypto_handle mpi_keyslot_handle)
{
    return uapi_klad_attach(mpi_klad_handle, klad_type, mpi_keyslot_handle);
}

td_s32 ot_mpi_klad_detach(crypto_handle mpi_klad_handle, km_klad_dest_type klad_type,
    crypto_handle mpi_keyslot_handle)
{
    return uapi_klad_detach(mpi_klad_handle, klad_type, mpi_keyslot_handle);
}

td_s32 ot_mpi_klad_set_attr(crypto_handle mpi_klad_handle, const km_klad_attr *attr)
{
    return uapi_klad_set_attr(mpi_klad_handle, attr);
}

td_s32 ot_mpi_klad_get_attr(crypto_handle mpi_klad_handle, km_klad_attr *attr)
{
    return uapi_klad_get_attr(mpi_klad_handle, attr);
}

td_s32 ot_mpi_klad_set_session_key(crypto_handle mpi_klad_handle, const km_klad_session_key *key)
{
    return uapi_klad_set_session_key(mpi_klad_handle, key);
}

td_s32 ot_mpi_klad_set_content_key(crypto_handle mpi_klad_handle, const km_klad_content_key *key)
{
    return uapi_klad_set_content_key(mpi_klad_handle, key);
}

td_s32 ot_mpi_klad_set_clear_key(crypto_handle mpi_klad_handle, const km_klad_clear_key *key)
{
    return uapi_klad_set_clear_key(mpi_klad_handle, key);
}