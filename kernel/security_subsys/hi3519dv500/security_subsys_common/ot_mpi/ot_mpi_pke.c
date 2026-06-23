/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "ot_mpi_cipher.h"

#include "uapi_pke.h"

td_s32 ot_mpi_cipher_pke_init(td_void)
{
    return unify_uapi_cipher_pke_init();
}

td_s32 ot_mpi_cipher_pke_deinit(td_void)
{
    return unify_uapi_cipher_pke_deinit();
}

td_s32 ot_mpi_cipher_pke_ecc_gen_key(drv_pke_ecc_curve_type curve_type, const drv_pke_data *input_priv_key,
    const drv_pke_data *output_priv_key, const drv_pke_ecc_point *output_pub_key)
{
    return unify_uapi_cipher_pke_ecc_gen_key(curve_type, input_priv_key, output_priv_key, output_pub_key);
}

td_s32 ot_mpi_cipher_pke_ecc_gen_ecdh_key(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *input_pub_key,
    const drv_pke_data *input_priv_key, const drv_pke_data *output_shared_key)
{
    return unify_uapi_cipher_pke_ecc_gen_ecdh_key(curve_type, input_pub_key, input_priv_key, output_shared_key);
}

td_s32 ot_mpi_cipher_pke_ecdsa_sign(drv_pke_ecc_curve_type curve_type, const drv_pke_data *priv_key,
    const drv_pke_data *hash, const drv_pke_ecc_sig *sig)
{
    return unify_uapi_cipher_pke_ecdsa_sign(curve_type, priv_key, hash, sig);
}

td_s32 ot_mpi_cipher_pke_ecdsa_verify(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *pub_key,
    const drv_pke_data *hash, const drv_pke_ecc_sig *sig)
{
    return unify_uapi_cipher_pke_ecdsa_verify(curve_type, pub_key, hash, sig);
}

td_s32 ot_mpi_cipher_pke_eddsa_sign(drv_pke_ecc_curve_type curve_type, const drv_pke_data *priv_key,
    const drv_pke_msg *msg, const drv_pke_ecc_sig *sig)
{
    return unify_uapi_cipher_pke_eddsa_sign(curve_type, priv_key, msg, sig);
}

td_s32 ot_mpi_cipher_pke_eddsa_verify(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *pub_key,
    const drv_pke_msg *msg, const drv_pke_ecc_sig *sig)
{
    return unify_uapi_cipher_pke_eddsa_verify(curve_type, pub_key, msg, sig);
}

td_s32 ot_mpi_cipher_pke_check_dot_on_curve(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *pub_key,
    td_bool *is_on_curve)
{
    return unify_uapi_cipher_pke_check_dot_on_curve(curve_type, pub_key, is_on_curve);
}

td_s32 ot_mpi_cipher_pke_sm2_dsa_hash(const drv_pke_data *sm2_id, const drv_pke_ecc_point *pub_key,
    const drv_pke_msg *msg, drv_pke_data *hash)
{
    return unify_uapi_cipher_pke_sm2_dsa_hash(sm2_id, pub_key, msg, hash);
}

td_s32 ot_mpi_cipher_pke_sm2_public_encrypt(const drv_pke_ecc_point *pub_key, const drv_pke_data *plain_text,
    drv_pke_data *cipher_text)
{
    return unify_uapi_cipher_pke_sm2_public_encrypt(pub_key, plain_text, cipher_text);
}


td_s32 ot_mpi_cipher_pke_sm2_private_decrypt(const drv_pke_data *priv_key, const drv_pke_data *cipher_text,
    drv_pke_data *plain_text)
{
    return unify_uapi_cipher_pke_sm2_private_decrypt(priv_key, cipher_text, plain_text);
}

/* RSA. */
td_s32 ot_mpi_cipher_pke_rsa_sign(const drv_pke_rsa_priv_key *priv_key, drv_pke_rsa_scheme scheme,
    drv_pke_hash_type hash_type, const drv_pke_data *input_hash,
    drv_pke_data *sign)
{
    return unify_uapi_cipher_pke_rsa_sign(priv_key, scheme, hash_type, input_hash, sign);
}

td_s32 ot_mpi_cipher_pke_rsa_verify(const drv_pke_rsa_pub_key *pub_key, drv_pke_rsa_scheme scheme,
    drv_pke_hash_type hash_type, drv_pke_data *input_hash, const drv_pke_data *sig)
{
    return unify_uapi_cipher_pke_rsa_verify(pub_key, scheme, hash_type, input_hash, sig);
}

td_s32 ot_mpi_cipher_pke_rsa_public_encrypt(drv_pke_rsa_scheme scheme, drv_pke_hash_type hash_type,
    const drv_pke_rsa_pub_key *pub_key, const drv_pke_data *input, const drv_pke_data *label,
    drv_pke_data *output)
{
    return unify_uapi_cipher_pke_rsa_public_encrypt(scheme, hash_type, pub_key, input, label, output);
}

td_s32 ot_mpi_cipher_pke_rsa_private_decrypt(drv_pke_rsa_scheme scheme, drv_pke_hash_type hash_type,
    const drv_pke_rsa_priv_key *priv_key, const drv_pke_data *input, const drv_pke_data *label,
    drv_pke_data *output)
{
    return unify_uapi_cipher_pke_rsa_private_decrypt(scheme, hash_type, priv_key, input, label, output);
}