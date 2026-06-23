/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_CIPHER_H
#define SS_MPI_CIPHER_H

#include "crypto_type.h"
#include "crypto_hash_struct.h"
#include "crypto_kdf_struct.h"
#include "crypto_pke_struct.h"
#include "crypto_symc_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* HASH */
td_s32 ss_mpi_cipher_hash_init(td_void);

td_s32 ss_mpi_cipher_hash_deinit(td_void);

td_s32 ss_mpi_cipher_hash_create(td_handle *mpi_hash_handle, const crypto_hash_attr *hash_attr);

td_s32 ss_mpi_cipher_hash_update(td_handle mpi_hash_handle, const crypto_buf_attr *src_buf, const td_u32 len);

td_s32 ss_mpi_cipher_hash_finish(td_handle mpi_hash_handle, td_u8 *virt_addr, td_u32 buffer_len, td_u32 *result_len);

td_s32 ss_mpi_cipher_hash_get(td_handle mpi_hash_handle, crypto_hash_clone_ctx *hash_clone_ctx);

td_s32 ss_mpi_cipher_hash_set(td_handle mpi_hash_handle, const crypto_hash_clone_ctx *hash_clone_ctx);

td_s32 ss_mpi_cipher_hash_destroy(td_handle mpi_hash_handle);

/* KDF */
td_s32 ss_mpi_cipher_pbkdf2(const crypto_kdf_pbkdf2_param *param, td_u8 *out, const td_u32 out_len);

/* PKE */
td_s32 ss_mpi_cipher_pke_init(td_void);
td_s32 ss_mpi_cipher_pke_deinit(td_void);

/*
 * input_priv_key: Could be NULL.
 */
td_s32 ss_mpi_cipher_pke_ecc_gen_key(drv_pke_ecc_curve_type curve_type, const drv_pke_data *input_priv_key,
    const drv_pke_data *output_priv_key, const drv_pke_ecc_point *output_pub_key);

td_s32 ss_mpi_cipher_pke_ecc_gen_ecdh_key(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *input_pub_key,
    const drv_pke_data *input_priv_key, const drv_pke_data *output_shared_key);

td_s32 ss_mpi_cipher_pke_ecdsa_sign(drv_pke_ecc_curve_type curve_type, const drv_pke_data *priv_key,
    const drv_pke_data *hash, const drv_pke_ecc_sig *sig);

td_s32 ss_mpi_cipher_pke_ecdsa_verify(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *pub_key,
    const drv_pke_data *hash, const drv_pke_ecc_sig *sig);

td_s32 ss_mpi_cipher_pke_eddsa_sign(drv_pke_ecc_curve_type curve_type, const drv_pke_data *priv_key,
    const drv_pke_msg *msg, const drv_pke_ecc_sig *sig);

td_s32 ss_mpi_cipher_pke_eddsa_verify(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *pub_key,
    const drv_pke_msg *msg, const drv_pke_ecc_sig *sig);

td_s32 ss_mpi_cipher_pke_check_dot_on_curve(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *pub_key,
    td_bool *is_on_curve);

td_s32 ss_mpi_cipher_pke_sm2_dsa_hash(const drv_pke_data *sm2_id, const drv_pke_ecc_point *pub_key,
    const drv_pke_msg *msg, drv_pke_data *hash);

td_s32 ss_mpi_cipher_pke_sm2_public_encrypt(const drv_pke_ecc_point *pub_key, const drv_pke_data *plain_text,
    drv_pke_data *cipher_text);

td_s32 ss_mpi_cipher_pke_sm2_private_decrypt(const drv_pke_data *priv_key, const drv_pke_data *cipher_text,
    drv_pke_data *plain_text);

/* RSA. */
td_s32 ss_mpi_cipher_pke_rsa_sign(const drv_pke_rsa_priv_key *priv_key, drv_pke_rsa_scheme scheme,
    drv_pke_hash_type hash_type, const drv_pke_data *input_hash,
    drv_pke_data *sign);

td_s32 ss_mpi_cipher_pke_rsa_verify(const drv_pke_rsa_pub_key *pub_key, drv_pke_rsa_scheme scheme,
    drv_pke_hash_type hash_type, drv_pke_data *input_hash, const drv_pke_data *sig);

td_s32 ss_mpi_cipher_pke_rsa_public_encrypt(drv_pke_rsa_scheme scheme, drv_pke_hash_type hash_type,
    const drv_pke_rsa_pub_key *pub_key, const drv_pke_data *input, const drv_pke_data *label,
    drv_pke_data *output);

td_s32 ss_mpi_cipher_pke_rsa_private_decrypt(drv_pke_rsa_scheme scheme, drv_pke_hash_type hash_type,
    const drv_pke_rsa_priv_key *priv_key, const drv_pke_data *input, const drv_pke_data *label,
    drv_pke_data *output);

/* SYMC */
td_s32 ss_mpi_cipher_symc_init(td_void);

td_s32 ss_mpi_cipher_symc_deinit(td_void);

td_s32 ss_mpi_cipher_symc_create(td_handle *symc_handle, const crypto_symc_attr *symc_attr);

td_s32 ss_mpi_cipher_symc_destroy(td_handle symc_handle);

td_s32 ss_mpi_cipher_symc_set_config(td_handle symc_handle, const crypto_symc_ctrl_t *symc_ctrl);

td_s32 ss_mpi_cipher_symc_attach(td_handle symc_handle, td_handle keyslot_handle);

td_s32 ss_mpi_cipher_symc_encrypt(td_handle symc_handle, const crypto_buf_attr *src_buf,
    const crypto_buf_attr *dst_buf, td_u32 length);

td_s32 ss_mpi_cipher_symc_decrypt(td_handle symc_handle, const crypto_buf_attr *src_buf,
    const crypto_buf_attr *dst_buf, td_u32 length);

td_s32 ss_mpi_cipher_symc_encrypt_multi(td_handle symc_handle, const crypto_symc_ctrl_t *symc_ctrl,
    const crypto_symc_pack *src_buf_pack, const crypto_symc_pack *dst_buf_pack, td_u32 pack_num);

td_s32 ss_mpi_cipher_symc_decrypt_multi(td_handle symc_handle, const crypto_symc_ctrl_t *symc_ctrl,
    const crypto_symc_pack *src_buf_pack, const crypto_symc_pack *dst_buf_pack, td_u32 pack_num);

td_s32 ss_mpi_cipher_symc_get_tag(td_handle symc_handle, td_u8 *tag, td_u32 tag_length);

td_s32 ss_mpi_cipher_mac_start(td_handle *symc_handle, const crypto_symc_mac_attr *mac_attr);

td_s32 ss_mpi_cipher_mac_update(td_handle symc_handle, const crypto_buf_attr *src_buf, td_u32 length);

td_s32 ss_mpi_cipher_mac_finish(td_handle symc_handle, td_u8 *mac, td_u32 *mac_length);

/* TRNG */
td_s32 ss_mpi_cipher_trng_get_random(td_u32 *randnum);

td_s32 ss_mpi_cipher_trng_get_multi_random(td_u32 size, td_u8 *randnum);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif