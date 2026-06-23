/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef DRV_INNER_H
#define DRV_INNER_H

#include "crypto_hash_struct.h"
#include "crypto_kdf_struct.h"
#include "crypto_symc_struct.h"
#include "crypto_pke_struct.h"

typedef struct {
    td_u8 mac[CRYPTO_IV_LEN_IN_BYTES];
    td_u32 mac_length;
    td_u8 tail[CRYPTO_IV_LEN_IN_BYTES];
    td_u32 tail_length;
} crypto_symc_mac_ctx;

typedef struct {
    td_u8 iv[CRYPTO_IV_LEN_IN_BYTES];
    td_u32 iv_length;
    td_u32 chn_num;
    td_phys_addr_t aad_phys;
    td_u8 *aad_virt;
    td_u32 aad_len;
    crypto_symc_alg symc_alg;
    crypto_symc_work_mode work_mode;
    crypto_symc_key_length symc_key_length;
    td_u32 last_pattern_len;
    union {
        crypto_symc_config_aes_ccm_gcm ccm_config;
    } param;
    /*
     * For CBC_MAC, store data copy;
     * For CCM, store N + ccm_header + ccm_padding
     * For GCM, store aad_padding + clen
     */
    td_u8 *dma_addr;
    td_u32 dma_size;
    td_u32 data_len;        /* For CCM/GCM. */
    td_u32 processed_len;   /* For CCM/GCM. */
    td_u8 tail[CRYPTO_AES_BLOCK_SIZE_IN_BYTES];
    td_u32 tail_length;
    td_bool is_create_keyslot;
    td_handle keyslot_handle;
    td_bool is_open;
    td_bool is_config;
    td_bool is_attached;
    td_u32 iv_change_flag;
#if defined(CRYPTO_CTR_NON_ALIGN_SUPPORT)
    td_u32 ctr_offset;
    td_u32 ctr_used;
    td_u8 ctr_last_block[CRYPTO_AES_BLOCK_SIZE_IN_BYTES];
#endif
} drv_symc_context_t;


td_s32 drv_cipher_pke_clean_ram(void);

td_s32 drv_cipher_pke_add_mod(const drv_pke_data *a, const drv_pke_data *b,
    const drv_pke_data *p, const drv_pke_data *c);

td_s32 drv_cipher_pke_sub_mod(const drv_pke_data *a, const drv_pke_data *b,
    const drv_pke_data *p, const drv_pke_data *c);

td_s32 drv_cipher_pke_mul_dot(const drv_pke_ecc_curve *ecc, const drv_pke_data *k, const drv_pke_ecc_point *p,
    const drv_pke_ecc_point *r);

td_s32 drv_cipher_pke_mul_mod(const drv_pke_data *a, const drv_pke_data *b, const drv_pke_data  *p,
    const drv_pke_data  *c);

td_s32 drv_cipher_pke_inv_mod(const drv_pke_data *a, const drv_pke_data *p, drv_pke_data *c);

td_s32 drv_cipher_pke_add_dot(const drv_pke_ecc_curve *ecc, const drv_pke_ecc_point *s, const drv_pke_ecc_point *r,
    const drv_pke_ecc_point *c);

td_s32 drv_cipher_pke_mg_mul_dot(const drv_pke_data *k, const drv_pke_data *U, const drv_pke_data *p,
    const drv_pke_data *a24, const drv_pke_data *rx);

td_s32 drv_cipher_pke_ed_mul_dot(const drv_pke_data *k, const drv_pke_ecc_point *U, const drv_pke_data *p,
    const drv_pke_ecc_point *r);

td_s32 drv_cipher_pke_ed_add_dot(const drv_pke_ecc_curve *ecc, const drv_pke_ecc_point *s,
    const drv_pke_ecc_point *r, const drv_pke_ecc_point *c);

td_s32 drv_cipher_pke_calc_hash(const drv_pke_data* arr, td_u32 arr_len, const drv_pke_hash_type hash_type,
    drv_pke_data *hash);

drv_symc_context_t *inner_get_symc_ctx(td_handle symc_handle);

td_u32 drv_hash_get_state_iv(crypto_hash_type hash_type, td_u32 *iv_size, td_u32 *state_iv, td_u32 state_iv_len);

td_s32 inner_hash_drv_handle_chk(td_handle hash_handle);

td_s32 inner_hash_start_param_chk(td_handle *drv_hash_handle, const crypto_hash_attr *hash_attr);

td_s32 inner_drv_symc_crypto_chk(td_handle symc_handle, const crypto_buf_attr *src_buf,
    const crypto_buf_attr *dst_buf, td_u32 length);

td_s32 inner_symc_drv_handle_chk(td_handle symc_handle);

td_s32 inner_drv_get_mac_ctx(td_handle symc_handle, crypto_symc_mac_ctx *mac_ctx);

td_s32 inner_drv_set_mac_ctx(td_handle symc_handle, const crypto_symc_mac_ctx *mac_ctx);

td_s32 inner_symc_cfg_param_check(const crypto_symc_ctrl_t *symc_ctrl);

typedef struct {
    td_u8 *iv0;
    td_u32 iv0_length;
    td_u8 *iv_mac;
    td_u32 iv_mac_length;
    td_u32 data_length;
    td_u32 processed_length;
    td_u32 aad_len;
} drv_symc_ex_context_t;

td_s32 inner_drv_symc_get_iv0(td_handle symc_handle, td_u8 *iv0, td_u32 iv0_length);

td_s32 inner_drv_symc_get_iv_mac(td_handle symc_handle, td_u8 *iv_mac, td_u32 iv_mac_length);

td_s32 inner_drv_symc_ex_restore(td_handle symc_handle, const drv_symc_ex_context_t *symc_ex_ctx);

td_s32 inner_drv_symc_set_ctr_block(td_handle symc_handle, const td_u8 *block, td_u32 block_size, td_u32 ctr_offset);

td_s32 inner_drv_symc_get_ctr_block(td_handle symc_handle, td_u8 *block, td_u32 block_size, td_u32 *ctr_offset);

#endif