/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_KM_H
#define SS_MPI_KM_H

#include "crypto_km_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

td_s32 ss_mpi_km_init(td_void);

td_s32 ss_mpi_km_deinit(td_void);

/* Keyslot. */
td_s32 ss_mpi_keyslot_create(crypto_handle *mpi_keyslot_handle, km_keyslot_type keyslot_type);
td_s32 ss_mpi_keyslot_destroy(crypto_handle mpi_keyslot_handle);

/* Klad. */
td_s32 ss_mpi_klad_create(crypto_handle *mpi_klad_handle);

td_s32 ss_mpi_klad_destroy(crypto_handle mpi_klad_handle);

td_s32 ss_mpi_klad_attach(crypto_handle mpi_klad_handle, km_klad_dest_type klad_type,
    crypto_handle mpi_keyslot_handle);

td_s32 ss_mpi_klad_detach(crypto_handle mpi_klad_handle, km_klad_dest_type klad_type,
    crypto_handle mpi_keyslot_handle);

td_s32 ss_mpi_klad_set_attr(crypto_handle mpi_klad_handle, const km_klad_attr *attr);

td_s32 ss_mpi_klad_get_attr(crypto_handle mpi_klad_handle, km_klad_attr *attr);

td_s32 ss_mpi_klad_set_session_key(crypto_handle mpi_klad_handle, const km_klad_session_key *key);

td_s32 ss_mpi_klad_set_content_key(crypto_handle mpi_klad_handle, const km_klad_content_key *key);

td_s32 ss_mpi_klad_set_clear_key(crypto_handle mpi_klad_handle, const km_klad_clear_key *key);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif