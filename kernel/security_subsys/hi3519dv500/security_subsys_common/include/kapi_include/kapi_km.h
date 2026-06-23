/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef KAPI_KM_H
#define KAPI_KM_H

#include "crypto_km_struct.h"

td_s32 kapi_km_env_init(td_void);
td_s32 kapi_km_env_deinit(td_void);
td_s32 kapi_km_deinit(td_void);

/* Keyslot. */
td_s32 kapi_keyslot_create(td_handle *kapi_keyslot_handle, km_keyslot_type keyslot_type);
td_s32 kapi_keyslot_destroy(td_handle kapi_keyslot_handle);
td_void kapi_keyslot_release_process(td_void);

/* Klad. */
td_s32 kapi_klad_create(td_handle *kapi_klad_handle);
td_s32 kapi_klad_destroy(td_handle kapi_klad_handle);

td_s32 kapi_klad_attach(td_handle kapi_klad_handle, km_klad_dest_type klad_type,
    td_handle kapi_keyslot_handle);
td_s32 kapi_klad_detach(td_handle kapi_klad_handle, km_klad_dest_type klad_type,
    td_handle kapi_keyslot_handle);

td_s32 kapi_klad_set_attr(td_handle kapi_klad_handle, const km_klad_attr *attr);
td_s32 kapi_klad_get_attr(td_handle kapi_klad_handle, km_klad_attr *attr);

td_s32 kapi_klad_set_session_key(td_handle kapi_klad_handle, const km_klad_session_key *key);
td_s32 kapi_klad_set_content_key(td_handle kapi_klad_handle, const km_klad_content_key *key);

td_s32 kapi_klad_set_clear_key(td_handle kapi_klad_handle, const km_klad_clear_key *key);

#endif