/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef KAPI_PKE_H
#define KAPI_PKE_H

#include "crypto_type.h"
#include "crypto_pke_struct.h"

td_s32 kapi_cipher_pke_mod(const drv_pke_data *a, const drv_pke_data *p, const drv_pke_data *c);

td_s32 kapi_cipher_pke_exp_mod(const drv_pke_data *n, const drv_pke_data *k,
    const drv_pke_data *in, const drv_pke_data *out);

td_s32 kapi_pke_ecc_gen_key(drv_pke_ecc_curve_type curve_type, const drv_pke_data *input_priv_key,
    const drv_pke_data *output_priv_key, const drv_pke_ecc_point *output_pub_key);

td_s32 kapi_pke_ecdsa_sign(drv_pke_ecc_curve_type curve_type, const drv_pke_data *priv_key,
    const drv_pke_data *hash, const drv_pke_ecc_sig *sig);

td_s32 kapi_pke_ecdsa_verify(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *pub_key,
    const drv_pke_data *hash, const drv_pke_ecc_sig *sig);

td_s32 kapi_pke_eddsa_sign(drv_pke_ecc_curve_type curve_type, const drv_pke_data *priv_key,
    const drv_pke_msg *msg, const drv_pke_ecc_sig *sig);

td_s32 kapi_pke_eddsa_verify(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *pub_key,
    const drv_pke_msg *msg, const drv_pke_ecc_sig *sig);

td_s32 kapi_pke_ecc_gen_ecdh_key(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *input_pub_key,
    const drv_pke_data *input_priv_key, const drv_pke_data *output_shared_key);

td_s32 kapi_pke_check_dot_on_curve(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *pub_key,
    td_bool *is_on_curve);

td_s32 kapi_pke_sm2_dsa_hash(const drv_pke_data *sm2_id, const drv_pke_ecc_point *pub_key,
    const drv_pke_msg *msg, drv_pke_data *hash);

td_s32 kapi_pke_sm2_public_encrypt(const drv_pke_ecc_point *pub_key, const drv_pke_data *plain_text,
    drv_pke_data *cipher_text);

td_s32 kapi_pke_sm2_private_decrypt(const drv_pke_data *priv_key, const drv_pke_data *cipher_text,
    drv_pke_data *plain_text);

/* RSA. */
td_s32 kapi_pke_rsa_sign(const drv_pke_rsa_priv_key *priv_key, drv_pke_rsa_scheme scheme,
    drv_pke_hash_type hash_type, const drv_pke_data *input_hash,
    drv_pke_data *sign);

td_s32 kapi_pke_rsa_verify(const drv_pke_rsa_pub_key *pub_key, drv_pke_rsa_scheme scheme,
    drv_pke_hash_type hash_type, drv_pke_data *input_hash, const drv_pke_data *sig);

td_s32 kapi_pke_rsa_public_encrypt(drv_pke_rsa_scheme scheme, drv_pke_hash_type hash_type,
    const drv_pke_rsa_pub_key *pub_key, const drv_pke_data *input, const drv_pke_data *label,
    drv_pke_data *output);

td_s32 kapi_pke_rsa_private_decrypt(drv_pke_rsa_scheme scheme, drv_pke_hash_type hash_type,
    const drv_pke_rsa_priv_key *priv_key, const drv_pke_data *input, const drv_pke_data *label,
    drv_pke_data *output);

#endif