/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef DRV_KLAD_H
#define DRV_KLAD_H

#include "crypto_type.h"
#include "crypto_km_struct.h"

td_s32 drv_klad_create(td_handle *klad_handle);

td_s32 drv_klad_destroy(td_handle klad_handle);

td_s32 drv_klad_attach(td_handle klad_handle, crypto_klad_dest klad_dest, td_handle keyslot_handle);

td_s32 drv_klad_detach(td_handle klad_handle, crypto_klad_dest klad_dest, td_handle keyslot_handle);

td_s32 drv_klad_set_attr(td_handle klad_handle, const crypto_klad_attr *attr);

td_s32 drv_klad_get_attr(td_handle klad_handle, crypto_klad_attr *attr);

td_s32 drv_klad_set_clear_key(td_handle klad_handle, const crypto_klad_clear_key *clear_key);

td_s32 drv_klad_set_session_key(td_handle klad_handle, const crypto_klad_session_key *session_key);

td_s32 drv_klad_set_content_key(td_handle klad_handle, const crypto_klad_content_key *content_key);

#endif