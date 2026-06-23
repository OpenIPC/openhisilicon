/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef HAL_KLAD_H
#define HAL_KLAD_H

#include "crypto_type.h"
#include "crypto_km_struct.h"

td_s32 hal_klad_lock(td_void);

td_s32 hal_klad_unlock(td_void);

td_s32 hal_klad_set_key_crypto_cfg(td_bool encrypt_support, td_bool decrypt_support, crypto_klad_engine engine);

td_s32 hal_klad_set_key_dest_cfg(crypto_klad_dest dest, crypto_klad_flash_key_type flash_key_type);

td_s32 hal_klad_set_key_secure_cfg(const crypto_klad_key_secure_config *secure_cfg);

td_s32 hal_klad_set_key_addr(crypto_klad_dest klad_dest, td_u32 keyslot_chn);

td_s32 hal_klad_set_data(const td_u8 *data, td_u32 data_length);

td_s32 hal_klad_start_clr_route(crypto_klad_dest klad_dest, const crypto_klad_clear_key *clear_key);

td_s32 hal_klad_com_start(crypto_klad_level_sel level, crypto_klad_alg_sel alg, crypto_klad_key_size key_size,
    crypto_kdf_hard_key_type rk_type);

td_s32 hal_klad_wait_com_route_done(td_void);

td_void hal_klad_clear_data(td_void);

td_void hal_klad_set_key_odd(td_bool odd);

#endif