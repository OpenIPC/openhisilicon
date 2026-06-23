/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef HAL_SYMC_H
#define HAL_SYMC_H

#include "hal_common.h"
#include "crypto_symc_struct.h"

td_s32 hal_cipher_symc_init(td_void);

td_s32 hal_cipher_symc_deinit(td_void);

td_s32 hal_cipher_symc_low_power_set(td_bool flag);

td_s32 hal_cipher_symc_lock_chn(td_u32 chn_num);

td_s32 hal_cipher_symc_unlock_chn(td_u32 chn_num);

td_s32 hal_cipher_symc_attach(td_u32 symc_chn_num, td_u32 keyslot_chn_num);

td_s32 hal_cipher_symc_set_iv(td_u32 chn_num, const td_u8 *iv, td_u32 iv_len);

td_s32 hal_cipher_symc_get_iv(td_u32 chn_num, td_u8 *iv, td_u32 iv_len);

typedef struct {
    crypto_symc_alg symc_alg;
    crypto_symc_work_mode work_mode;
    crypto_symc_key_length symc_key_length;
    crypto_symc_bit_width symc_bit_width;
    td_bool is_decrypt;
    crypto_symc_iv_change_type iv_change_flag;
    /* iv_mac for ccm, iv_ghash for gcm. */
    td_u8 iv_mac[16];
    /* s0 for ccm, j0 for gcm. */
    td_u8 iv0[16];
    /* for gcm calculate j0. */
    const td_u8 *gcm_iv_ptr;
    td_u32 gcm_iv_len;
} hal_symc_config_t;

td_s32 hal_cipher_symc_config(td_u32 chn_num, const hal_symc_config_t *symc_config);

td_s32 hal_cipher_symc_get_module_info(crypto_symc_module_info *module_info);

td_s32 hal_cipher_symc_get_proc_info(td_u32 chn_num, crypto_symc_proc_info *proc_symc_info);

td_s32 hal_cipher_symc_get_config(td_u32 chn_num, hal_symc_config_t *symc_config);

td_s32 hal_cipher_symc_add_in_node(td_u32 chn_num, td_phys_addr_t data_phys_addr, td_u32 data_len,
    in_node_type_e in_node_type);

td_s32 hal_cipher_symc_add_out_node(td_u32 chn_num, td_phys_addr_t data_phys_addr, td_u32 data_len);

td_s32 hal_cipher_symc_get_tag(td_u32 chn_num, td_u8 *tag, td_u32 tag_len);

td_s32 hal_cipher_symc_start(td_u32 chn_num);

td_s32 hal_cipher_symc_wait_done(td_u32 chn_num, td_bool is_wait);

td_s32 hal_cipher_symc_done_try(td_u32 chn_num);

td_s32 hal_cipher_symc_done_notify(td_u32 chn_num);

td_s32 hal_cipher_symc_register_wait_func(td_u32 chn_num, td_void *wait,
    crypto_wait_timeout_interruptible wait_func, td_u32 timeout_ms);

td_void hal_cipher_set_chn_secure(td_u32 chn_num, td_bool dest_sec, td_bool src_sec);

#endif
