/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef DRV_PKE_H
#define DRV_PKE_H

#include "crypto_pke_struct.h"

/* Define ECC-HASH related data & types */
#define SHA1_BLOCK_LENGTH                          64
#define SHA224_BLOCK_LENGTH                        64
#define SHA256_BLOCK_LENGTH                        64
#define SHA384_BLOCK_LENGTH                        128
#define SHA512_BLOCK_LENGTH                        128
#define SM3_BLOCK_LENGTH                           64

#define SHA1_RESULT_LENGTH                         20
#define SHA224_RESULT_LENGTH                       28
#define SHA256_RESULT_LENGTH                       32
#define SHA384_RESULT_LENGTH                       48
#define SHA512_RESULT_LENGTH                       64
#define SM3_RESULT_LENGTH                          32

/* result size */
#define HASH_SIZE_SHA_1                            20
#define HASH_SIZE_SHA_224                          28
#define HASH_SIZE_SHA_256                          32
#define HASH_SIZE_SHA_384                          48
#define HASH_SIZE_SHA_512                          64
#define HASH_SIZE_SHA_MAX                          64

#define DRV_PKE_SM2_LEN_IN_BYTES                   32

#define PKE_MAX_TIMES              3000

#define WORD_INDEX_0               0
#define WORD_INDEX_1               1
#define WORD_INDEX_2               2
#define WORD_INDEX_3               3

#define MAX_LOW_2BITS              3
#define MAX_LOW_3BITS              7
#define MAX_LOW_4BITS              0xF
#define MAX_LOW_8BITS              0xFF

#define SHIFT_4BITS                4
#define SHIFT_8BITS                8
#define SHIFT_16BITS               16
#define SHIFT_24BITS               24

#define BOUND_VALUE_1              1

#define REGISTER_WIDTH             8
#define DRV_WORD_WIDTH             4
#define BYTE_BITS                  8

#define RSA_PUBLIC                 0x5A /* public key operation */
#define RSA_PRIVATE                0xA5 /* private key operation */

#define RSA_PADLEN_1               1
#define RSA_PADLEN_2               2
#define RSA_PADLEN_3               3
#define RSA_PADLEN_8               8
#define RSA_PADLEN_11              11
#define RSA_PADLEN_19              19
#define RSA_PADLEN_51              51
#define RSA_PAD_X12                0x12
#define RSA_PAD_X55                0x55
#define RSA_PAD_XBC                0xBC

#define RSA_MIN_KEY_LEN            32
#define RSA_MAX_KEY_LEN            512
#define HASH_SIZE_SHA_MAX          64

#define ECC_TRY_CNT                8

#define ECC_KEY_LEN_IN_WORD        0x12
#define ECC_KEY_LEN_IN_BYTE        (ECC_KEY_LEN_IN_WORD * 4)
#define ECC_KEY_LEN_IN_BYTE_X2     (ECC_KEY_LEN_IN_BYTE * 2) // 2: x2 is for security enhancement


#define SM2_PC_UNCOMPRESS          0x04
#define SM2_TRY_CNT                8
#define ENTLA_LEN                  2
#define PKE_U16_MAX             0xFFFF

typedef struct {
    td_u32 klen;
    td_u32 em_bit;
    td_u8 *em;
    td_u32 em_len;
    td_u8 *hash;
    td_u32 hash_len;
    td_u8 *data;
    td_u32 data_len;
} rsa_pkcs1_pack;

typedef struct {
    drv_pke_hash_type hash_type;
    td_u32 hash_len;
    td_u8 *lhash_data;
    td_u8 *asn1_data;
    td_u32 asn1_len;
} rsa_pkcs1_hash_info;

typedef struct {
    drv_pke_hash_type hash_type;
    td_u32 block_len;
    td_u32 result_len;
} pke_hash_properties_type;

typedef struct {
    td_u8 *px;
    td_u8 *py;
    td_u8 *id;
    td_u16 id_len;
} sm2_sign_verify_hash_pack;

td_s32 drv_cipher_pke_init(void);

td_s32 drv_cipher_pke_deinit(void);

td_s32 drv_cipher_pke_mod(const drv_pke_data *a, const drv_pke_data *p, const drv_pke_data *c);

td_s32 drv_cipher_pke_exp_mod(const drv_pke_data *n, const drv_pke_data *k,
    const drv_pke_data *in, const drv_pke_data *out);

td_s32 drv_cipher_pke_lock_secure(void);

td_s32 drv_cipher_pke_unlock_secure(void);

td_s32 drv_cipher_pke_rsa_sign(const drv_pke_rsa_priv_key *priv_key, const drv_pke_rsa_scheme scheme,
    const drv_pke_hash_type hash_type, const drv_pke_data *input_hash,
    drv_pke_data *sign);

td_s32 drv_cipher_pke_rsa_verify(const drv_pke_rsa_pub_key *pub_key, drv_pke_rsa_scheme scheme,
    const drv_pke_hash_type hash_type, const drv_pke_data *input_hash, const drv_pke_data *sign);

td_s32 drv_cipher_pke_rsa_public_encrypt(const drv_pke_rsa_scheme scheme, const drv_pke_hash_type hash_type,
    const drv_pke_rsa_pub_key *pub_key, const drv_pke_data *input, const drv_pke_data *label,
    drv_pke_data *output);

td_s32 drv_cipher_pke_rsa_private_decrypt(const drv_pke_rsa_scheme scheme, const drv_pke_hash_type hash_type,
    const drv_pke_rsa_priv_key *priv_key, const drv_pke_data *input, const drv_pke_data *label,
    drv_pke_data *output);

td_s32 drv_cipher_pke_check_dot_on_curve(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *pub_key,
    td_bool *is_on_curve);

td_s32 drv_cipher_pke_ecc_gen_key(drv_pke_ecc_curve_type curve_type, const drv_pke_data *input_priv_key,
    const drv_pke_data *output_priv_key, const drv_pke_ecc_point *output_pub_key);

td_s32 drv_cipher_pke_ecc_gen_ecdh_key(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *input_pub_key,
    const drv_pke_data *input_priv_key, const drv_pke_data *output_shared_key);

td_s32 drv_cipher_pke_ecdsa_sign(drv_pke_ecc_curve_type curve_type, const drv_pke_data *priv_key,
    const drv_pke_data *hash, const drv_pke_ecc_sig *sig);

td_s32 drv_cipher_pke_ecdsa_verify(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *pub_key,
    const drv_pke_data *hash, const drv_pke_ecc_sig *sig);

td_s32 drv_cipher_pke_eddsa_sign(drv_pke_ecc_curve_type curve_type, const drv_pke_data *priv_key,
    const drv_pke_msg *msg, const drv_pke_ecc_sig *sig);

td_s32 drv_cipher_pke_eddsa_verify(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *pub_key,
    const drv_pke_msg *msg, const drv_pke_ecc_sig *sig);

td_s32 drv_cipher_pke_sm2_dsa_hash(const drv_pke_data *sm2_id, const drv_pke_ecc_point *pub_key,
    const drv_pke_msg *msg, drv_pke_data *hash);

td_s32 drv_cipher_pke_sm2_public_encrypt(const drv_pke_ecc_point *pub_key, const drv_pke_data *plain_text,
    drv_pke_data *cipher_text);

td_s32 drv_cipher_pke_sm2_private_decrypt(const drv_pke_data *priv_key, const drv_pke_data *cipher_text,
    drv_pke_data *plain_text);

#endif