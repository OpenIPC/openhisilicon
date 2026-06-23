/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef HAL_RKP_H
#define HAL_RKP_H

#include "crypto_type.h"
#include "crypto_km_struct.h"

td_s32 hal_rkp_lock(td_void);

td_s32 hal_rkp_unlock(td_void);

td_s32 hal_rkp_deob_update(crypto_kdf_otp_key otp_key, crypto_kdf_update_alg alg);

td_s32 hal_rkp_kdf_set_val(const td_u8 *kdf_val, td_u32 val_length);

td_s32 hal_rkp_kdf_get_val(td_u8 *kdf_val, td_u32 val_length);

td_s32 hal_rkp_kdf_set_padding_salt(const td_u8 *padding_salt, td_u32 salt_length);

td_s32 hal_rkp_kdf_set_padding_key(const td_u8 *padding_key, td_u32 key_length);

td_s32 hal_rkp_clear_reg_key(td_void);

td_s32 hal_rkp_kdf_sw_start(crypto_kdf_sw_alg sw_alg, td_u32 count, td_bool is_wait);

td_s32 hal_rkp_deob_wait_done(td_void);

td_s32 hal_rkp_kdf_wait_done(td_void);

td_s32 hal_rkp_kdf_hard_calculation(const crypto_kdf_hard_calc_param *param);

td_void hal_rkp_debug(td_void);

#endif