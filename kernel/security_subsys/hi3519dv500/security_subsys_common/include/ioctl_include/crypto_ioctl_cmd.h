/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef CRYPTO_IOCTL_CMD_H
#define CRYPTO_IOCTL_CMD_H

#include "crypto_ioctl.h"

#include "crypto_hash_struct.h"
#include "crypto_symc_struct.h"
#include "crypto_pke_struct.h"
#include "crypto_common_def.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CRYPTO_CIPHER_DEV_NAME      "soc_cipher"

typedef struct {
    crypto_compat_addr key;
    td_u32 key_len;
    td_u32 kapi_hash_handle;
    td_handle drv_keyslot_handle;
    td_bool is_keyslot;
    td_bool is_long_term;
    crypto_hash_type hash_type;
} hash_start_ctl_t;

typedef struct {
    crypto_compat_addr src_buf;
    td_u32 len;
    td_handle kapi_hash_handle;
} hash_update_ctl_t;

typedef struct {
    crypto_compat_addr out;
    td_u32 out_len;
    td_handle kapi_hash_handle;
} hash_finish_ctl_t;

typedef struct {
    crypto_compat_addr password;
    crypto_compat_addr salt;
    crypto_compat_addr out;
    td_u32 plen;
    td_u32 slen;
    td_u32 out_len;
    td_u16 count;
    crypto_hash_type hash_type;
} pbkdf2_ctl_t;

typedef struct {
    crypto_compat_addr hash_clone_ctx;
    td_u32 ctx_size;
    td_handle kapi_hash_handle;
} hash_clone_ctl_t;

typedef struct {
    td_handle kapi_hash_handle;
} hash_handle_ctl_t;

typedef struct {
    td_u32 randnum;
} trng_ctl_t;

typedef struct {
    td_u32 size;
    crypto_compat_addr randnum;
} trng_multi_ctl_t;

typedef struct {
    td_handle symc_handle;
    crypto_symc_attr symc_attr;
} symc_create_t;

typedef struct {
    td_handle symc_handle;
} symc_destroy_t;

typedef struct {
    td_handle symc_handle;
    crypto_symc_alg symc_alg;
    crypto_symc_work_mode work_mode;
    crypto_symc_key_length symc_key_length;
    crypto_symc_key_parity key_parity;
    crypto_symc_bit_width symc_bit_width;
    crypto_symc_iv_change_type iv_change_flag;
    td_u8 iv[CRYPTO_IV_LEN_IN_BYTES];
    td_u32 iv_length;
    td_u32 aad_len;
    td_u32 data_len;
    td_u32 tag_len;
    unsigned long long aad_mem_handle;
    td_phys_addr_t aad_phys_addr;
    crypto_compat_addr aad;
} symc_config_t;

typedef struct {
    td_handle symc_handle;
    td_handle keyslot_handle;
} symc_get_keyslot_t;

typedef struct {
    td_handle symc_handle;
    td_handle keyslot_handle;
} symc_attach_t;

typedef struct {
    td_handle symc_handle;
    td_u8 key[CRYPTO_256_KEY_LEN];
    td_u32 key_len;
} symc_set_key_t;

typedef struct {
    td_handle symc_handle;
    unsigned long long src_mem_handle;
    td_phys_addr_t src_phys_addr;
    crypto_buffer_secure src_buf_sec;
    unsigned long long src_addr_offset;
    unsigned long long dst_mem_handle;
    td_phys_addr_t dst_phys_addr;
    crypto_buffer_secure dst_buf_sec;
    unsigned long long dst_addr_offset;
    td_u32 length;
} symc_crypto_t;

typedef struct {
    unsigned long long uapi_mem_handle;
    td_phys_addr_t phys_addr;
    td_u32 length;
    crypto_buffer_secure buf_sec;           /* NONSECURE or SECURE */
} crypto_mem_pack;

typedef struct {
    td_handle symc_handle;
    crypto_symc_alg symc_alg;
    crypto_symc_work_mode work_mode;
    crypto_symc_key_length symc_key_length;
    crypto_symc_key_parity key_parity;
    crypto_symc_bit_width symc_bit_width;
    crypto_symc_iv_change_type iv_change_flag;
    td_u8 iv[CRYPTO_IV_LEN_IN_BYTES];
    td_u32 iv_length;
    crypto_compat_addr src_pack_addr;
    crypto_compat_addr dst_pack_addr;
    td_u32 pack_num;
} crypto_symc_multi_t;

typedef struct {
    td_handle symc_handle;
    crypto_symc_alg alg;
    crypto_symc_work_mode work_mode;
    td_u32 iv[CRYPTO_AES_IV_SIZE_IN_WORD];
    td_bool use_odd_key;
    td_u32 first_encrypt_offset;
    td_u32 subsample_num;
    crypto_compat_addr subsample_addr;
    unsigned long long src_mem_handle;
    crypto_buffer_secure src_buf_sec;
    unsigned long long dst_mem_handle;
    crypto_buffer_secure dst_buf_sec;
    td_u32 length;
} crypto_symc_cenc_t;

typedef struct {
    td_handle symc_handle;
    td_u8 tag[CRYPTO_AES_MAX_TAG_LEN];
    td_u32 tag_length;
} symc_get_tag_t;

typedef struct {
    td_handle symc_handle;
    crypto_symc_mac_attr mac_attr;
} symc_mac_start_t;

typedef struct {
    td_handle symc_handle;
    crypto_compat_addr src_buf;
    td_u32 length;
} symc_mac_update_t;

typedef struct {
    td_handle symc_handle;
    td_u8 mac[CRYPTO_AES_BLOCK_SIZE_IN_BYTES];
    td_u32 mac_length;
} symc_mac_finish_t;

typedef struct {
    td_handle keyslot_handle;
    td_u8 key[32];
    td_u32 key_len;
    keyslot_engine engine;
} keyslot_create_t;

typedef struct {
    td_handle keyslot_handle;
} keyslot_destroy_t;

typedef struct {
    crypto_compat_addr data;
    td_u32  length;
} drv_pke_data_t;

typedef struct {
    td_u32  length;
    crypto_compat_addr data;
    drv_pke_buffer_secure buf_sec;
} drv_pke_msg_t;

typedef struct {
    crypto_compat_addr x;
    crypto_compat_addr y;
    td_u32 length;
} drv_pke_ecc_point_t;

/* * struct of ecc signature */
typedef struct {
    crypto_compat_addr r;   /* r component of the signature. */
    crypto_compat_addr s;   /* s component of the signature. */
    td_u32 length;
} drv_pke_ecc_sig_t;

typedef struct {
    drv_pke_ecc_curve_type curve_type;
    drv_pke_data_t input_priv_key;
    drv_pke_data_t output_priv_key;
    drv_pke_ecc_point_t output_pub_key;
} pke_ecc_gen_key_ctl_t;

typedef struct {
    drv_pke_ecc_curve_type curve_type;
    drv_pke_ecc_point_t input_pub_key;
    drv_pke_data_t input_priv_key;
    drv_pke_data_t output_shared_key;
} pke_ecc_gen_ecdh_key_ctl_t;

typedef struct {
    drv_pke_ecc_curve_type curve_type;
    drv_pke_data_t priv_key;
    drv_pke_data_t hash;
    drv_pke_ecc_sig_t sig;
} pke_ecdsa_sign_ctl_t;

typedef struct {
    drv_pke_ecc_curve_type curve_type;
    drv_pke_ecc_point_t pub_key;
    drv_pke_data_t hash;
    drv_pke_ecc_sig_t sig;
    crypto_compat_addr v;
} pke_ecdsa_verify_ctl_t;

typedef struct {
    drv_pke_ecc_curve_type curve_type;
    drv_pke_data_t priv_key;
    drv_pke_msg_t msg;
    drv_pke_ecc_sig_t sig;
} pke_eddsa_sign_ctl_t;

typedef struct {
    drv_pke_ecc_curve_type curve_type;
    drv_pke_ecc_point_t pub_key;
    drv_pke_msg_t msg;
    drv_pke_ecc_sig_t sig;
} pke_eddsa_verify_ctl_t;

typedef struct {
    drv_pke_ecc_curve_type curve_type;
    drv_pke_ecc_point_t pub_key;
    td_bool is_on_curve;
} pke_check_dot_on_curve_ctl_t;

typedef struct pke_sm2_dsa_hash_ctl {
    drv_pke_data_t sm2_id;
    drv_pke_ecc_point_t pub_key;
    drv_pke_msg_t msg;
    drv_pke_data_t hash;
} pke_sm2_dsa_hash_ctl_t;

typedef struct pke_sm2_public_encrypt_ctl {
    drv_pke_ecc_point_t pub_key;
    drv_pke_data_t plain_text;
    drv_pke_data_t cipher_text;
} pke_sm2_public_encrypt_ctl_t;

typedef struct {
    drv_pke_data_t priv_key;
    drv_pke_data_t cipher_text;
    drv_pke_data_t plain_text;
} pke_sm2_private_decrypt_ctl_t;

/* * RSA private key struct */
typedef struct {
    crypto_compat_addr n;          /* *< public modulus */
    crypto_compat_addr e;          /* *< public exponent */
    crypto_compat_addr d;          /* *< private exponent */
    crypto_compat_addr p;          /* *< 1st prime factor */
    crypto_compat_addr q;          /* *< 2nd prime factor */
    crypto_compat_addr dp;         /* *< D % (P - 1) */
    crypto_compat_addr dq;         /* *< D % (Q - 1) */
    crypto_compat_addr qp;         /* *< 1 / (Q % P) */
    td_u16 n_len;      /* *< length of public modulus */
    td_u16 e_len;      /* *< length of public exponent */
    td_u16 d_len;      /* *< length of private exponent */
    td_u16 p_len;      /* *< length of 1st prime factor,should be half of u16NLen */
    td_u16 q_len;      /* *< length of 2nd prime factor,should be half of u16NLen */
    td_u16 dp_len;     /* *< length of D % (P - 1),should be half of u16NLen */
    td_u16 dq_len;     /* *< length of D % (Q - 1),should be half of u16NLen */
    td_u16 qp_len;     /* *< length of 1 / (Q % P),should be half of u16NLen */
} drv_pke_rsa_priv_key_t;

/* * struct of RSA public key */
typedef struct {
    crypto_compat_addr n;            /* point to public modulus */
    crypto_compat_addr e;            /* point to public exponent */
    td_u16 len;           /* length of public modulus, max value is 512Byte */
} drv_pke_rsa_pub_key_t;

typedef struct {
    drv_pke_data_t a;
    drv_pke_data_t p;
    drv_pke_data_t c;
} pke_mod_ctl_t;

typedef struct {
    drv_pke_data_t n;
    drv_pke_data_t k;
    drv_pke_data_t in;
    drv_pke_data_t out;
} pke_exp_mod_ctl_t;

typedef struct {
    drv_pke_data_t input_e;
    drv_pke_rsa_priv_key_t priv_key;
} pke_rsa_gen_key_ctl_t;

typedef struct {
    drv_pke_rsa_priv_key_t priv_key;
    drv_pke_rsa_scheme scheme;
    drv_pke_hash_type hash_type;
    drv_pke_data_t input_hash;
    drv_pke_data_t sig;
} pke_rsa_sign_ctl_t;

typedef struct {
    drv_pke_rsa_pub_key_t pub_key;
    drv_pke_rsa_scheme scheme;
    drv_pke_hash_type hash_type;
    drv_pke_data_t input_hash;
    drv_pke_data_t sig;
    drv_pke_data_t output_hash;
} pke_rsa_verify_ctl_t;

typedef struct {
    drv_pke_rsa_scheme scheme;
    drv_pke_hash_type hash_type;
    drv_pke_rsa_pub_key_t pub_key;
    drv_pke_data_t input;
    drv_pke_data_t label;
    drv_pke_data_t output;
} pke_rsa_pub_crypto_ctl_t;

typedef struct {
    drv_pke_rsa_scheme scheme;
    drv_pke_hash_type hash_type;
    drv_pke_rsa_priv_key_t priv_key;
    drv_pke_data_t input;
    drv_pke_data_t label;
    drv_pke_data_t output;
} pke_rsa_priv_crypto_ctl_t;


#define HASH_BLOCK_SIZE         64

#define CRYPTO_IO(nr)           CRYPTO_IOC(CRYPTO_IOC_NA, SOC_ID_CIPHER, (nr), 0)
#define CRYPTO_IOR(nr, size)    CRYPTO_IOC(CRYPTO_IOC_R, SOC_ID_CIPHER, (nr), size)
#define CRYPTO_IOW(nr, size)    CRYPTO_IOC(CRYPTO_IOC_W, SOC_ID_CIPHER, (nr), size)
#define CRYPTO_IOWR(nr, size)   CRYPTO_IOC(CRYPTO_IOC_RW, SOC_ID_CIPHER, (nr), size)

enum {
    /* Hash. */
    CRYPTO_CMD_HASH_INIT_NR     = 0x0,
    CRYPTO_CMD_HASH_DEINIT_NR,
    CRYPTO_CMD_HASH_START_NR,
    CRYPTO_CMD_HASH_UPDATE_NR,
    CRYPTO_CMD_HASH_FINISH_NR,
    CRYPTO_CMD_HASH_GET_NR,
    CRYPTO_CMD_HASH_SET_NR,
    CRYPTO_CMD_HASH_DESTROY_NR,
    CRYPTO_CMD_PBKDF2_NR,
    CRYPTO_CMD_TRNG_GET_RANDOM_NR,
    CRYPTO_CMD_TRNG_GET_MULTI_RANDOM_NR,

    /* Cipher. */
    CRYPTO_CMD_CIPHER_INIT_NR,
    CRYPTO_CMD_CIPHER_DEINIT_NR,
    CRYPTO_CMD_CIPHER_CREATE_NR,
    CRYPTO_CMD_CIPHER_DESTROY_NR,
    CRYPTO_CMD_CIPHER_SET_CONFIG_NR,
    CRYPTO_CMD_CIPHER_GET_CONFIG_NR,
    CRYPTO_CMD_CIPHER_ATTACH_NR,
    CRYPTO_CMD_CIPHER_ENCRYPT_NR,
    CRYPTO_CMD_CIPHER_DECRYPT_NR,
    CRYPTO_CMD_CIPHER_ENCRYPT_MULTI_NR,
    CRYPTO_CMD_CIPHER_DECRYPT_MULTI_NR,
    CRYPTO_CMD_CIPHER_GET_TAG_NR,
    CRYPTO_CMD_CIPHER_MAC_START_NR,
    CRYPTO_CMD_CIPHER_MAC_UPDATE_NR,
    CRYPTO_CMD_CIPHER_MAC_FINISH_NR,

    /* PKE. ECC */
    CRYPTO_CMD_PKE_ECC_GEN_KEY_NR,
    CRYPTO_CMD_PKE_ECC_GEN_ECDH_KEY_NR,
    CRYPTO_CMD_PKE_ECDSA_SIGN_NR,
    CRYPTO_CMD_PKE_ECDSA_VERIFY_NR,
    CRYPTO_CMD_PKE_EDDSA_SIGN_NR,
    CRYPTO_CMD_PKE_EDDSA_VERIFY_NR,
    CRYPTO_CMD_PKE_CHECK_DOT_ON_CURVE_NR,
    CRYPTO_CMD_PKE_SM2_DSA_HASH_NR,
    CRYPTO_CMD_PKE_SM2_PUBLIC_ENCRYPT_NR,
    CRYPTO_CMD_PKE_SM2_PRIVATE_DECRYPT_NR,
    /* PKE. BIGNUM */
    CRYPTO_CMD_PKE_MOD_NR,
    CRYPTO_CMD_PKE_EXP_MOD_NR,
    /* PKE. RSA. */
    CRYPTO_CMD_PKE_RSA_SIGN_NR,
    CRYPTO_CMD_PKE_RSA_VERIFY_NR,
    CRYPTO_CMD_PKE_RSA_PUBLIC_ENCRYPT_NR,
    CRYPTO_CMD_PKE_RSA_PRIVATE_DECRYPT_NR,

    CRYPTO_CMD_COUNT
};

#define CRYPTO_CMD_HASH_INIT                   CRYPTO_IO(CRYPTO_CMD_HASH_INIT_NR)
#define CRYPTO_CMD_HASH_DEINIT                 CRYPTO_IO(CRYPTO_CMD_HASH_DEINIT_NR)
#define CRYPTO_CMD_HASH_START                  CRYPTO_IOWR(CRYPTO_CMD_HASH_START_NR, sizeof(hash_start_ctl_t))
#define CRYPTO_CMD_HASH_UPDATE                 CRYPTO_IOWR(CRYPTO_CMD_HASH_UPDATE_NR, sizeof(hash_update_ctl_t))
#define CRYPTO_CMD_HASH_FINISH                 CRYPTO_IOWR(CRYPTO_CMD_HASH_FINISH_NR, sizeof(hash_finish_ctl_t))
#define CRYPTO_CMD_HASH_GET                    CRYPTO_IOWR(CRYPTO_CMD_HASH_GET_NR, sizeof(hash_clone_ctl_t))
#define CRYPTO_CMD_HASH_SET                    CRYPTO_IOWR(CRYPTO_CMD_HASH_SET_NR, sizeof(hash_clone_ctl_t))
#define CRYPTO_CMD_HASH_DESTROY                CRYPTO_IOWR(CRYPTO_CMD_HASH_DESTROY_NR, sizeof(hash_handle_ctl_t))
#define CRYPTO_CMD_PBKDF2                      CRYPTO_IOWR(CRYPTO_CMD_PBKDF2_NR, sizeof(pbkdf2_ctl_t))
#define CRYPTO_CMD_TRNG_GET_RANDOM             CRYPTO_IOWR(CRYPTO_CMD_TRNG_GET_RANDOM_NR, sizeof(trng_ctl_t))
#define CRYPTO_CMD_TRNG_GET_MULTI_RANDOM      CRYPTO_IOWR(CRYPTO_CMD_TRNG_GET_MULTI_RANDOM_NR, sizeof(trng_multi_ctl_t))

#define CRYPTO_CMD_SYMC_INIT                   CRYPTO_IO(CRYPTO_CMD_CIPHER_INIT_NR)
#define CRYPTO_CMD_SYMC_DEINIT                 CRYPTO_IO(CRYPTO_CMD_CIPHER_DEINIT_NR)
#define CRYPTO_CMD_SYMC_CREATE                 CRYPTO_IOWR(CRYPTO_CMD_CIPHER_CREATE_NR, sizeof(symc_create_t))
#define CRYPTO_CMD_SYMC_DESTROY                CRYPTO_IOWR(CRYPTO_CMD_CIPHER_DESTROY_NR, sizeof(symc_destroy_t))
#define CRYPTO_CMD_SYMC_SET_CONFIG             CRYPTO_IOWR(CRYPTO_CMD_CIPHER_SET_CONFIG_NR, sizeof(symc_config_t))
#define CRYPTO_CMD_SYMC_GET_CONFIG             CRYPTO_IOWR(CRYPTO_CMD_CIPHER_GET_CONFIG_NR, sizeof(symc_config_t))
#define CRYPTO_CMD_SYMC_ATTACH                 CRYPTO_IOWR(CRYPTO_CMD_CIPHER_ATTACH_NR, sizeof(symc_attach_t))
#define CRYPTO_CMD_SYMC_ENCRYPT                CRYPTO_IOWR(CRYPTO_CMD_CIPHER_ENCRYPT_NR, sizeof(symc_crypto_t))
#define CRYPTO_CMD_SYMC_DECRYPT                CRYPTO_IOWR(CRYPTO_CMD_CIPHER_DECRYPT_NR, sizeof(symc_crypto_t))
#define CRYPTO_CMD_SYMC_ENCRYPT_MULTI          \
            CRYPTO_IOWR(CRYPTO_CMD_CIPHER_ENCRYPT_MULTI_NR, sizeof(crypto_symc_multi_t))
#define CRYPTO_CMD_SYMC_DECRYPT_MULTI          \
            CRYPTO_IOWR(CRYPTO_CMD_CIPHER_DECRYPT_MULTI_NR, sizeof(crypto_symc_multi_t))

#define CRYPTO_CMD_SYMC_GET_TAG                CRYPTO_IOWR(CRYPTO_CMD_CIPHER_GET_TAG_NR, sizeof(symc_get_tag_t))
#define CRYPTO_CMD_SYMC_MAC_START              CRYPTO_IOWR(CRYPTO_CMD_CIPHER_MAC_START_NR, sizeof(symc_mac_start_t))
#define CRYPTO_CMD_SYMC_MAC_UPDATE             CRYPTO_IOWR(CRYPTO_CMD_CIPHER_MAC_UPDATE_NR, sizeof(symc_mac_update_t))
#define CRYPTO_CMD_SYMC_MAC_FINISH             CRYPTO_IOWR(CRYPTO_CMD_CIPHER_MAC_FINISH_NR, sizeof(symc_mac_finish_t))

#define CRYPTO_CMD_PKE_MOD                     CRYPTO_IOWR(CRYPTO_CMD_PKE_MOD_NR, sizeof(pke_mod_ctl_t))
#define CRYPTO_CMD_PKE_EXP_MOD                 CRYPTO_IOWR(CRYPTO_CMD_PKE_EXP_MOD_NR, sizeof(pke_exp_mod_ctl_t))

#define CRYPTO_CMD_PKE_ECC_GEN_KEY             \
            CRYPTO_IOWR(CRYPTO_CMD_PKE_ECC_GEN_KEY_NR, sizeof(pke_ecc_gen_key_ctl_t))
#define CRYPTO_CMD_PKE_ECC_GEN_ECDH_KEY        \
            CRYPTO_IOWR(CRYPTO_CMD_PKE_ECC_GEN_ECDH_KEY_NR, sizeof(pke_ecc_gen_ecdh_key_ctl_t))
#define CRYPTO_CMD_PKE_ECDSA_SIGN              \
            CRYPTO_IOWR(CRYPTO_CMD_PKE_ECDSA_SIGN_NR, sizeof(pke_ecdsa_sign_ctl_t))
#define CRYPTO_CMD_PKE_ECDSA_VERIFY            \
            CRYPTO_IOWR(CRYPTO_CMD_PKE_ECDSA_VERIFY_NR, sizeof(pke_ecdsa_verify_ctl_t))
#define CRYPTO_CMD_PKE_EDDSA_SIGN              \
            CRYPTO_IOWR(CRYPTO_CMD_PKE_EDDSA_SIGN_NR, sizeof(pke_eddsa_sign_ctl_t))
#define CRYPTO_CMD_PKE_EDDSA_VERIFY            \
            CRYPTO_IOWR(CRYPTO_CMD_PKE_EDDSA_VERIFY_NR, sizeof(pke_eddsa_verify_ctl_t))
#define CRYPTO_CMD_PKE_CHECK_DOT_ON_CURVE      \
            CRYPTO_IOWR(CRYPTO_CMD_PKE_CHECK_DOT_ON_CURVE_NR, sizeof(pke_check_dot_on_curve_ctl_t))
#define CRYPTO_CMD_PKE_SM2_DSA_HASH            \
            CRYPTO_IOWR(CRYPTO_CMD_PKE_SM2_DSA_HASH_NR, sizeof(pke_sm2_dsa_hash_ctl_t))
#define CRYPTO_CMD_PKE_SM2_PUBLIC_ENCRYPT      \
            CRYPTO_IOWR(CRYPTO_CMD_PKE_SM2_PUBLIC_ENCRYPT_NR, sizeof(pke_sm2_public_encrypt_ctl_t))
#define CRYPTO_CMD_PKE_SM2_PRIVATE_DECRYPT     \
            CRYPTO_IOWR(CRYPTO_CMD_PKE_SM2_PRIVATE_DECRYPT_NR, sizeof(pke_sm2_private_decrypt_ctl_t))

#define CRYPTO_CMD_PKE_RSA_SIGN                \
            CRYPTO_IOWR(CRYPTO_CMD_PKE_RSA_SIGN_NR, sizeof(pke_rsa_sign_ctl_t))
#define CRYPTO_CMD_PKE_RSA_VERIFY              \
            CRYPTO_IOWR(CRYPTO_CMD_PKE_RSA_VERIFY_NR, sizeof(pke_rsa_verify_ctl_t))
#define CRYPTO_CMD_PKE_RSA_PUBLIC_ENCRYPT      \
            CRYPTO_IOWR(CRYPTO_CMD_PKE_RSA_PUBLIC_ENCRYPT_NR, sizeof(pke_rsa_pub_crypto_ctl_t))
#define CRYPTO_CMD_PKE_RSA_PRIVATE_DECRYPT     \
            CRYPTO_IOWR(CRYPTO_CMD_PKE_RSA_PRIVATE_DECRYPT_NR, sizeof(pke_rsa_priv_crypto_ctl_t))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef CRYPTO_IOCTL_CMD_H */
