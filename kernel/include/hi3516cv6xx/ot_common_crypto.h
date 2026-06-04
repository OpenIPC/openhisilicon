/**
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef OT_COMMON_CRYPTO_H
#define OT_COMMON_CRYPTO_H

#include "ot_type.h"

typedef td_handle           crypto_handle;

#define CRYPTO_PKE_CURVE_ID_SHIFT    10

#define compat_pke_curve_type(curve_id, klen)       \
    ((((curve_id) & 0x3FF) << CRYPTO_PKE_CURVE_ID_SHIFT) | ((klen) & 0x3FF))

typedef enum {
    CRYPTO_BUF_SECURE,
    CRYPTO_BUF_NONSECURE,
} crypto_buffer_secure;

typedef enum {
    DRV_PKE_BUF_NONSECURE = 0x00,
    DRV_PKE_BUF_SECURE,
    DRV_PKE_BUF_INVALID = 0xffffffff,
} drv_pke_buffer_secure;

typedef enum {
    DRV_PKE_LEN_192 = 24,
    DRV_PKE_LEN_224 = 32,
    DRV_PKE_LEN_256 = 32,
    DRV_PKE_LEN_384 = 48,
    DRV_PKE_LEN_448 = 56,
    DRV_PKE_LEN_512 = 64,
    DRV_PKE_LEN_521 = 68,
    DRV_PKE_LEN_576 = 72,
    DRV_PKE_LEN_1024 = 128,
    DRV_PKE_LEN_1536 = 192,
    DRV_PKE_LEN_2048 = 256,
    DRV_PKE_LEN_3072 = 384,
    DRV_PKE_LEN_4096 = 512,
    DRV_PKE_LEN_MAX,
    DRV_PKE_LEN_INVALID = 0xffffffff,
} drv_pke_len;

/*
 * Component of drv_pke_ecc_curve_type.
 * reserved(22 bits) | curve_id(10 bits) | klen(10 bits)
 */
typedef enum {
    RFC5639_P256_CURVE_ID = 0,
    RFC5639_P384_CURVE_ID,
    RFC5639_P512_CURVE_ID,
    FIPS_P256K_CURVE_ID,
    FIPS_P192R_CURVE_ID,
    FIPS_P224R_CURVE_ID,
    FIPS_P256R_CURVE_ID,
    FIPS_P384R_CURVE_ID,
    FIPS_P521R_CURVE_ID,
    RFC7748_CURVE_ID,
    RFC7748_448_CURVE_ID,
    RFC8032_CURVE_ID,
    SM2_CURVE_ID,
    CURVE_ID_MAX,
} drv_pke_curve_id;

typedef struct {
    td_u32  length;
    td_u8  *data;
} drv_pke_data;

/* * struct of ecc point */
typedef struct {
    td_u8 *x;   /* X coordinates of the generated public key, the caller ensures it is padded with leading
                   zeros if the effective size of this key is smaller than ecc key size. */
    td_u8 *y;   /* Y coordinates of the generated public key, the caller ensures it is padded with leading
                   zeros if the effective size of this key is smaller than ecc key size. */
    td_u32 length;
} drv_pke_ecc_point;

/* * struct of ecc signature */
typedef struct {
    td_u8 *r;   /* r component of the signature. */
    td_u8 *s;   /* s component of the signature. */
    td_u32 length;
} drv_pke_ecc_sig;

typedef struct {
    td_u32  length;
    td_u8  *data;
    drv_pke_buffer_secure buf_sec;
} drv_pke_msg;

/* * RSA private key struct */
typedef struct {
    td_u8 *n;          /* *< public modulus */
    td_u8 *e;          /* *< public exponent */
    td_u8 *d;          /* *< private exponent */
    td_u8 *p;          /* *< 1st prime factor */
    td_u8 *q;          /* *< 2nd prime factor */
    td_u8 *dp;         /* *< D % (P - 1) */
    td_u8 *dq;         /* *< D % (Q - 1) */
    td_u8 *qp;         /* *< 1 / (Q % P) */
    td_u16 n_len;      /* *< length of public modulus */
    td_u16 e_len;      /* *< length of public exponent */
    td_u16 d_len;      /* *< length of private exponent */
    td_u16 p_len;      /* *< length of 1st prime factor,should be half of u16NLen */
    td_u16 q_len;      /* *< length of 2nd prime factor,should be half of u16NLen */
    td_u16 dp_len;     /* *< length of D % (P - 1),should be half of u16NLen */
    td_u16 dq_len;     /* *< length of D % (Q - 1),should be half of u16NLen */
    td_u16 qp_len;     /* *< length of 1 / (Q % P),should be half of u16NLen */
} drv_pke_rsa_priv_key;

/* * struct of RSA public key */
typedef struct {
    td_u8  *n;            /* point to public modulus */
    td_u8  *e;            /* point to public exponent */
    td_u16 len;           /* length of public modulus, max value is 512Byte */
} drv_pke_rsa_pub_key;

typedef enum {
    DRV_PKE_RSA_SCHEME_PKCS1_V15 = 0x00,  /* PKCS#1 V15 */
    DRV_PKE_RSA_SCHEME_PKCS1_V21,         /* PKCS#1 V21, PSS for signning, OAEP for encryption */
    DRV_PKE_RSA_SCHEME_MAX,
    DRV_PKE_RSA_SCHEME_INVALID = 0xffffffff,
} drv_pke_rsa_scheme;

typedef enum {
    DRV_PKE_HASH_TYPE_SHA1 = 0x00,   /* Suggest Not to use */
    DRV_PKE_HASH_TYPE_SHA224,
    DRV_PKE_HASH_TYPE_SHA256,
    DRV_PKE_HASH_TYPE_SHA384,
    DRV_PKE_HASH_TYPE_SHA512,
    DRV_PKE_HASH_TYPE_SM3,
    DRV_PKE_HASH_TYPE_MAX,
    DRV_PKE_HASH_TYPE_INVALID = 0xffffffff,
} drv_pke_hash_type;

typedef enum {
    /* 0x20, RFC 5639 - Brainpool P256 */
    DRV_PKE_ECC_TYPE_RFC5639_P256 = compat_pke_curve_type(RFC5639_P256_CURVE_ID, DRV_PKE_LEN_256),
    /* 0x430, RFC 5639 - Brainpool P384 */
    DRV_PKE_ECC_TYPE_RFC5639_P384 = compat_pke_curve_type(RFC5639_P384_CURVE_ID, DRV_PKE_LEN_384),
    /* 0x840, RFC 5639 - Brainpool P512 */
    DRV_PKE_ECC_TYPE_RFC5639_P512 = compat_pke_curve_type(RFC5639_P512_CURVE_ID, DRV_PKE_LEN_512),
    /* 0xc20, RFC 5639 - Brainpool P512 */
    DRV_PKE_ECC_TYPE_FIPS_P256K = compat_pke_curve_type(FIPS_P256K_CURVE_ID, DRV_PKE_LEN_256),
    /* 0x1018, NIST FIPS 186-4 P192/224/256/384/521, suggest not to use */
    DRV_PKE_ECC_TYPE_FIPS_P192R = compat_pke_curve_type(FIPS_P192R_CURVE_ID, DRV_PKE_LEN_192),
    /* 0x1420, NIST FIPS 186-4 P192/224/256/384/521, suggest not to use */
    DRV_PKE_ECC_TYPE_FIPS_P224R = compat_pke_curve_type(FIPS_P224R_CURVE_ID, DRV_PKE_LEN_224),
    /* 0x1820, NIST FIPS 186-4 P192/224/256/384/521, suggest not to use */
    DRV_PKE_ECC_TYPE_FIPS_P256R = compat_pke_curve_type(FIPS_P256R_CURVE_ID, DRV_PKE_LEN_256),
    /* 0x1c30, NIST FIPS 186-4 P192/224/256/384/521, suggest not to use */
    DRV_PKE_ECC_TYPE_FIPS_P384R = compat_pke_curve_type(FIPS_P384R_CURVE_ID, DRV_PKE_LEN_384),
    /* 0x2048, NIST FIPS 186-4 P192/224/256/384/521, suggest not to use */
    DRV_PKE_ECC_TYPE_FIPS_P521R = compat_pke_curve_type(FIPS_P521R_CURVE_ID, DRV_PKE_LEN_576),
    /* 0x2420, RFC 7748 - Curve25519 */
    DRV_PKE_ECC_TYPE_RFC7748 = compat_pke_curve_type(RFC7748_CURVE_ID, DRV_PKE_LEN_256),
    /* 0x2838, RFC 7748 - Curve448 */
    DRV_PKE_ECC_TYPE_RFC7748_448 = compat_pke_curve_type(RFC7748_448_CURVE_ID, DRV_PKE_LEN_448),
    /* 0x2c20, RFC 8032 - ED25519 */
    DRV_PKE_ECC_TYPE_RFC8032 = compat_pke_curve_type(RFC8032_CURVE_ID, DRV_PKE_LEN_256),
    /* 0x3020, GMT 0003.2-2012 */
    DRV_PKE_ECC_TYPE_SM2 = compat_pke_curve_type(SM2_CURVE_ID, DRV_PKE_LEN_256),
    DRV_PKE_ECC_TYPE_MAX,
    DRV_PKE_ECC_TYPE_INVALID = 0xffffffff,
} drv_pke_ecc_curve_type;

/* Structure for HASH. */
#define CRYPTO_HASH_RESULT_SIZE_MAX 64              // for SHA-512
#define CRYPTO_HASH_RESULT_SIZE_MAX_IN_WORD 16      // for SHA-512
#define CRYPTO_HASH_BLOCK_SIZE_MAX 128              // for SHA-512

#define CRYPTO_HASH_TYPE        0
#define CRYPTO_HMAC_TYPE        1
#define CRYPTO_IS_HMAC_MASK     0xF0000000
#define CRYPTO_IS_HMAC_SHIFT    28

#define CRYPTO_HASH_ALG_SHA1    0
#define CRYPTO_HASH_ALG_SHA2    1
#define CRYPTO_HASH_ALG_SM3     2
#define CRYPTO_HASH_ALG_MASK    0x0F000000
#define CRYPTO_HASH_ALG_SHIFT   24

#define CRYPTO_HASH_MODE_224    0
#define CRYPTO_HASH_MODE_256    1
#define CRYPTO_HASH_MODE_384    2
#define CRYPTO_HASH_MODE_512    3
#define CRYPTO_HASH_MODE_UNDEF  0xf
#define CRYPTO_HASH_MODE_MASK   0x00F00000
#define CRYPTO_HASH_MODE_SHIFT  20

#define CRYPTO_HASH_MAX_MESSAGE_LEN_64BIT   0x6
#define CRYPTO_HASH_MAX_MESSAGE_LEN_128BIT  0x7
#define CRYPTO_HASH_MAX_MESSAGE_LEN_MASK    0x000F0000
#define CRYPTO_HASH_MAX_MESSAGE_LEN_SHIFT   16

#define CRYPTO_HASH_BLOCK_SIZE_512BIT       0x9
#define CRYPTO_HASH_BLOCK_SIZE_1024BIT      0xa
#define CRYPTO_HASH_BLOCK_SIZE_MASK         0x0000F000
#define CRYPTO_HASH_BLOCK_SIZE_SHIFT        12

#define CRYPTO_HASH_RESULT_SIZE_160BIT      0xa0
#define CRYPTO_HASH_RESULT_SIZE_224BIT      0xe0
#define CRYPTO_HASH_RESULT_SIZE_256BIT      0x100
#define CRYPTO_HASH_RESULT_SIZE_384BIT      0x180
#define CRYPTO_HASH_RESULT_SIZE_512BIT      0x200
#define CRYPTO_HASH_RESULT_SIZE_MASK        0x00000FFF
#define CRYPTO_HASH_RESULT_SIZE_SHIFT       0

/*
 * Component of crypto_hash_type.
 * is_hmac(4 bits) | alg(4 bits) | mode(4 bits) | max_message_length(4 bits) | block_size(4 bits) | result_size(12 bits)
 * is_hmac: h'0 - hash
 *          h'1 - hmac
 * alg:     h'0 - sha1
 *          h'1 - sha2
 *          h'2 - sm3
 * mode:    h'0 - 224
 *          h'1 - 256
 *          h'2 - 384
 *          h'3 - 512
 * max_message_length:
 *          h'6 - 2**6 -> 64, longest 2**64 Bits. For SHA256.
 *          h'7 - 2**7 -> 128, longest 2**128 Bits. For SHA384/SHA512.
 * block_size:
 *          h'9 - 2**9 -> 512, Block Size is 512 Bits. For SHA256.
 *          h'a - 2**10 -> 1024, Block Size is 1024 Bits. For SHA384/SHA512.
 * result_size:
 *          h'100 - 256, Result Size is 256 Bits. For SHA256.
 *          h'180 - 384, Result Size is 384 Bits. For SHA384.
 *          h'200 - 512, Result Size is 512 Bits. For SHA512.
 */
#define compat_hash_type(is_hmac, alg, mode, max_message_length, block_size, result_size)          \
    ((((is_hmac) & 0xF) << 28) | (((alg) & 0xF) << 24) | (((mode) & 0xF) << 20) |                  \
    (((max_message_length) & 0xF) << 16) | (((block_size) & 0xF) << 12) | ((result_size) & 0xFFF))

typedef enum {
    /* Hash. */
    CRYPTO_HASH_TYPE_SHA1 = compat_hash_type(
        CRYPTO_HASH_TYPE, CRYPTO_HASH_ALG_SHA1, CRYPTO_HASH_MODE_UNDEF, CRYPTO_HASH_MAX_MESSAGE_LEN_64BIT,
        CRYPTO_HASH_BLOCK_SIZE_512BIT, CRYPTO_HASH_RESULT_SIZE_160BIT
    ),
    CRYPTO_HASH_TYPE_SHA224 = compat_hash_type(
        CRYPTO_HASH_TYPE, CRYPTO_HASH_ALG_SHA2, CRYPTO_HASH_MODE_224, CRYPTO_HASH_MAX_MESSAGE_LEN_64BIT,
        CRYPTO_HASH_BLOCK_SIZE_512BIT, CRYPTO_HASH_RESULT_SIZE_224BIT
    ),
    CRYPTO_HASH_TYPE_SHA256 = compat_hash_type(
        CRYPTO_HASH_TYPE, CRYPTO_HASH_ALG_SHA2, CRYPTO_HASH_MODE_256, CRYPTO_HASH_MAX_MESSAGE_LEN_64BIT,
        CRYPTO_HASH_BLOCK_SIZE_512BIT, CRYPTO_HASH_RESULT_SIZE_256BIT
    ),
    CRYPTO_HASH_TYPE_SHA384 = compat_hash_type(
        CRYPTO_HASH_TYPE, CRYPTO_HASH_ALG_SHA2, CRYPTO_HASH_MODE_384, CRYPTO_HASH_MAX_MESSAGE_LEN_128BIT,
        CRYPTO_HASH_BLOCK_SIZE_1024BIT, CRYPTO_HASH_RESULT_SIZE_384BIT
    ),
    CRYPTO_HASH_TYPE_SHA512 = compat_hash_type(
        CRYPTO_HASH_TYPE, CRYPTO_HASH_ALG_SHA2, CRYPTO_HASH_MODE_512, CRYPTO_HASH_MAX_MESSAGE_LEN_128BIT,
        CRYPTO_HASH_BLOCK_SIZE_1024BIT, CRYPTO_HASH_RESULT_SIZE_512BIT
    ),
    CRYPTO_HASH_TYPE_SM3 = compat_hash_type(
        CRYPTO_HASH_TYPE, CRYPTO_HASH_ALG_SM3, CRYPTO_HASH_MODE_256, CRYPTO_HASH_MAX_MESSAGE_LEN_64BIT,
        CRYPTO_HASH_BLOCK_SIZE_512BIT, CRYPTO_HASH_RESULT_SIZE_256BIT
    ),

    /* HMAC. */
    CRYPTO_HASH_TYPE_HMAC_SHA1 = compat_hash_type(
        CRYPTO_HMAC_TYPE, CRYPTO_HASH_ALG_SHA1, CRYPTO_HASH_MODE_UNDEF, CRYPTO_HASH_MAX_MESSAGE_LEN_64BIT,
        CRYPTO_HASH_BLOCK_SIZE_512BIT, CRYPTO_HASH_RESULT_SIZE_160BIT
    ),
    CRYPTO_HASH_TYPE_HMAC_SHA224 = compat_hash_type(
        CRYPTO_HMAC_TYPE, CRYPTO_HASH_ALG_SHA2, CRYPTO_HASH_MODE_224, CRYPTO_HASH_MAX_MESSAGE_LEN_64BIT,
        CRYPTO_HASH_BLOCK_SIZE_512BIT, CRYPTO_HASH_RESULT_SIZE_224BIT
    ),
    CRYPTO_HASH_TYPE_HMAC_SHA256 = compat_hash_type(
        CRYPTO_HMAC_TYPE, CRYPTO_HASH_ALG_SHA2, CRYPTO_HASH_MODE_256, CRYPTO_HASH_MAX_MESSAGE_LEN_64BIT,
        CRYPTO_HASH_BLOCK_SIZE_512BIT, CRYPTO_HASH_RESULT_SIZE_256BIT
    ),
    CRYPTO_HASH_TYPE_HMAC_SHA384 = compat_hash_type(
        CRYPTO_HMAC_TYPE, CRYPTO_HASH_ALG_SHA2, CRYPTO_HASH_MODE_384, CRYPTO_HASH_MAX_MESSAGE_LEN_128BIT,
        CRYPTO_HASH_BLOCK_SIZE_1024BIT, CRYPTO_HASH_RESULT_SIZE_384BIT
    ),
    CRYPTO_HASH_TYPE_HMAC_SHA512 = compat_hash_type(
        CRYPTO_HMAC_TYPE, CRYPTO_HASH_ALG_SHA2, CRYPTO_HASH_MODE_512, CRYPTO_HASH_MAX_MESSAGE_LEN_128BIT,
        CRYPTO_HASH_BLOCK_SIZE_1024BIT, CRYPTO_HASH_RESULT_SIZE_512BIT
    ),
    CRYPTO_HASH_TYPE_HMAC_SM3 = compat_hash_type(
        CRYPTO_HMAC_TYPE, CRYPTO_HASH_ALG_SM3, CRYPTO_HASH_MODE_256, CRYPTO_HASH_MAX_MESSAGE_LEN_64BIT,
        CRYPTO_HASH_BLOCK_SIZE_512BIT, CRYPTO_HASH_RESULT_SIZE_256BIT
    ),

    CRYPTO_HASH_TYPE_INVALID = 0xffffffff,
} crypto_hash_type;

typedef struct {
    unsigned long long uapi_mem_handle;     /* Handle to buffer header address */
    unsigned long long addr_offset;         /* buffer offset, unused. */
    td_void *kapi_mem_handle;

    unsigned long long phys_addr;
    void *virt_addr;
    crypto_buffer_secure buf_sec;           /* NONSECURE or SECURE */
} crypto_buf_attr;

typedef struct {
    td_u8 *key;
    td_u32 key_len;
    td_handle drv_keyslot_handle;
    crypto_hash_type hash_type;
    td_bool is_keyslot;
    td_bool is_long_term;
} crypto_hash_attr;

typedef struct {
    td_u32 length[2];
    td_u32 state[CRYPTO_HASH_RESULT_SIZE_MAX_IN_WORD];
    td_u32 tail_len;
    crypto_hash_type hash_type;
    td_u8 o_key_pad[CRYPTO_HASH_BLOCK_SIZE_MAX];
    td_u8 i_key_pad[CRYPTO_HASH_BLOCK_SIZE_MAX];
    td_u8 tail[CRYPTO_HASH_BLOCK_SIZE_MAX];
} crypto_hash_clone_ctx;

typedef struct {
    crypto_hash_type hash_type;
    td_u8 *password;
    td_u32 plen;
    td_u8 *salt;
    td_u32 slen;
    td_u16 count;
} crypto_kdf_pbkdf2_param;

#define CRYPTO_IV_LEN_IN_BYTES      16

typedef struct {
    crypto_buf_attr buf_attr;
    td_u32 length;
} crypto_symc_pack;

/* Structure for SYMC Create. */
typedef enum {
    CRYPTO_SYMC_TYPE_NORMAL = 0x0,
    CRYPTO_SYMC_TYPE_REG,
    CRYPTO_SYMC_TYPE_MAX,
    CRYPTO_SYMC_TYPE_INVALID = 0xffffffff,
} crypto_symc_type;

typedef enum {
    CRYPTO_SYMC_KEY_64BIT =  0x0,
    CRYPTO_SYMC_KEY_128BIT = 0x1,
    CRYPTO_SYMC_KEY_192BIT = 0x2,
    CRYPTO_SYMC_KEY_256BIT = 0x3,
    CRYPTO_SYMC_KEY_LENGTH_MAX,
    CRYPTO_SYMC_KEY_LENGTH_INVALID = 0xffffffff,
} crypto_symc_key_length;

typedef enum {
    CRYPTO_SYMC_KEY_EVEN =  0x0,
    CRYPTO_SYMC_KEY_ODD  =  0x1,
    CRYPTO_SYMC_KEY_PARITY_MAX,
    CRYPTO_SYMC_KEY_PARITY_INVALID = 0xffffffff,
} crypto_symc_key_parity;


typedef enum {
    CRYPTO_SYMC_BIT_WIDTH_1BIT = 0x0,
    CRYPTO_SYMC_BIT_WIDTH_8BIT = 0x1,
    CRYPTO_SYMC_BIT_WIDTH_64BIT = 0x2,
    CRYPTO_SYMC_BIT_WIDTH_128BIT = 0x3,
    CRYPTO_SYMC_BIT_WIDTH_MAX,
    CRYPTO_SYMC_BIT_WIDTH_INVALID = 0xffffffff,
} crypto_symc_bit_width;

typedef enum {
    CRYPTO_SYMC_IV_DO_NOT_CHANGE = 0,
    CRYPTO_SYMC_IV_CHANGE_ONE_PKG,
    CRYPTO_SYMC_IV_CHANGE_ALL_PKG,
    /* GCM. */
    CRYPTO_SYMC_GCM_IV_DO_NOT_CHANGE,
    CRYPTO_SYMC_GCM_IV_CHANGE_START,
    CRYPTO_SYMC_GCM_IV_CHANGE_UPDATE,
    CRYPTO_SYMC_GCM_IV_CHANGE_FINISH,
    /* CCM. */
    CRYPTO_SYMC_CCM_IV_DO_NOT_CHANGE,
    CRYPTO_SYMC_CCM_IV_CHANGE_START,
    CRYPTO_SYMC_CCM_IV_CHANGE_UPDATE,
    CRYPTO_SYMC_CCM_IV_CHANGE_FINISH,
    CRYPTO_SYMC_IV_CHANGE_MAX,
    CRYPTO_SYMC_IV_CHANGE_INVALID = 0xffffffff,
} crypto_symc_iv_change_type;


typedef enum {
    CRYPTO_SYMC_ALG_TDES = 0x0,
    CRYPTO_SYMC_ALG_AES = 0x1,
    CRYPTO_SYMC_ALG_SM4 = 0x2,
    CRYPTO_SYMC_ALG_LEA = 0x3,
    CRYPTO_SYMC_ALG_DMA = 0x4,
    CRYPTO_SYMC_ALG_MAX,
    CRYPTO_SYMC_ALG_INVALID = 0xffffffff,
} crypto_symc_alg;

typedef enum {
    CRYPTO_SYMC_WORK_MODE_ECB = 0x0,
    CRYPTO_SYMC_WORK_MODE_CBC,
    CRYPTO_SYMC_WORK_MODE_CTR,
    CRYPTO_SYMC_WORK_MODE_OFB,
    CRYPTO_SYMC_WORK_MODE_CFB,
    CRYPTO_SYMC_WORK_MODE_CCM,
    CRYPTO_SYMC_WORK_MODE_GCM,
    CRYPTO_SYMC_WORK_MODE_CBC_MAC,
    CRYPTO_SYMC_WORK_MODE_CMAC,
    CRYPTO_SYMC_WORK_MODE_MAX,
    CRYPTO_SYMC_WORK_MODE_INVALID = 0xffffffff,
} crypto_symc_work_mode;


typedef struct {
    crypto_symc_alg symc_alg;
    crypto_symc_work_mode work_mode;
    crypto_symc_type symc_type;
    td_bool is_long_term;
} crypto_symc_attr;

typedef struct {
    crypto_symc_alg symc_alg;
    crypto_symc_work_mode work_mode;
    crypto_symc_key_length symc_key_length;
    crypto_symc_key_parity key_parity;
    crypto_symc_bit_width symc_bit_width;
    crypto_symc_iv_change_type iv_change_flag;
    td_u8 iv[CRYPTO_IV_LEN_IN_BYTES];
    td_u32 iv_length;
    td_void *param;
} crypto_symc_ctrl_t;

typedef struct {
    td_bool is_long_term;
    crypto_symc_alg symc_alg;
    crypto_symc_work_mode work_mode;
    crypto_symc_key_length symc_key_length;
    td_u32 keyslot_chn;
} crypto_symc_mac_attr;

typedef struct {
    crypto_buf_attr aad_buf;
    td_u32 aad_len;         /* Addition Data Length In Bytes at this time. */
    td_u32 data_len;        /* Crypto Data Length In Bytes. */
    td_u32 tag_len;         /* Tag Length In Bytes. */
    td_u32 total_aad_len;   /* Total Addition Data Length In Bytes. */
} crypto_symc_config_aes_ccm_gcm;

/* Define the structure of keyladder configuration. */
typedef struct {
    td_u32 rootkey_type;  /* Keyladder rootkey type, KM_KLAD_KEY_TYPE_xxx. */
} km_klad_config;

/* Define the key security attribute. */
typedef enum {
    KM_KLAD_SEC_DISABLE = 0,
    KM_KLAD_SEC_ENABLE,
    KM_KLAD_SEC_MAX,
    KM_KLAD_SEC_INVALID = 0xffffffff,
} km_klad_sec;

enum {
    /* SBRK0, TEE Global Boot Root Key0, delivered from MRK1, soc_tee_enable and Hash of TEE Root Public Key */
    KM_KLAD_KEY_TYPE_SBRK0  = 0x03000000,
    /* SBRK1, TEE Global Boot Root Key1, delivered from MRK1, soc_tee_enable and Hash of TEE Root Public Key */
    KM_KLAD_KEY_TYPE_SBRK1,
    /* SBRK2, TEE Global Boot Root Key2, delivered from MRK1, soc_tee_enable and Hash of TEE Root Public Key */
    KM_KLAD_KEY_TYPE_SBRK2,
    /* ABRK0, REE Global Boot Root Key0, delivered from MRK1 */
    KM_KLAD_KEY_TYPE_ABRK0,
    /* ABRK1, REE Global Boot Root Key1, delivered from MRK1 */
    KM_KLAD_KEY_TYPE_ABRK1,
    /* ABRK2, REE Global Boot Root Key2, delivered from MRK1 */
    KM_KLAD_KEY_TYPE_ABRK2,
    /* DRK0, TEE unique data decryption root key, delivered from USK */
    KM_KLAD_KEY_TYPE_DRK0,
    /* DRK1, REE unique data decryption root key, delivered from USK */
    KM_KLAD_KEY_TYPE_DRK1,
    /* RDRK0, TEE random data decryption root key, delivered from RUSK */
    KM_KLAD_KEY_TYPE_RDRK0,
    /* RDRK1, REE random data decryption root key, delivered from RUSK */
    KM_KLAD_KEY_TYPE_RDRK1,
    /* PSK, delivered from MRK1 */
    KM_KLAD_KEY_TYPE_PSK,
    /* ODRK0, TEE flash online decryptin root key, delivered from MRK0/1,
        soc_tee_enable and Hash of TEE Root Public Key */
    KM_KLAD_KEY_TYPE_ODRK0,
    /* OARK0  TEE flash online authentication root key, delivered from MRK0/1,
        soc_tee_enable and Hash of TEE Root Public Key */
    KM_KLAD_KEY_TYPE_OARK0,
    /* ODRK1, REE flash online decryption root key, delivered from MRK1 */
    KM_KLAD_KEY_TYPE_ODRK1,
    /* MDRK0 */
    KM_KLAD_KEY_TYPE_MDRK0,
    /* MDRK1 */
    KM_KLAD_KEY_TYPE_MDRK1,
    /* MDRK2 */
    KM_KLAD_KEY_TYPE_MDRK2,
    /* MDRK3 */
    KM_KLAD_KEY_TYPE_MDRK3,

    KM_KLAD_KEY_TYPE_ABRK_REE,
    KM_KLAD_KEY_TYPE_ABRK_TEE,
    KM_KLAD_KEY_TYPE_RDRK_REE,
    KM_KLAD_KEY_TYPE_RDRK_TEE,
    KM_KLAD_KEY_TYPE_ERK_TEE,
    KM_KLAD_KEY_TYPE_ERK_REE
};

/**
 * @brief  Select the derived key type during KDF key derivation.
 */
typedef enum {
    CRYPTO_KDF_HARD_KEY_TYPE_SBRK0  = 0x03000000,
    CRYPTO_KDF_HARD_KEY_TYPE_SBRK1,
    CRYPTO_KDF_HARD_KEY_TYPE_SBRK2,
    CRYPTO_KDF_HARD_KEY_TYPE_ABRK0,
    CRYPTO_KDF_HARD_KEY_TYPE_ABRK1,
    CRYPTO_KDF_HARD_KEY_TYPE_ABRK2,
    CRYPTO_KDF_HARD_KEY_TYPE_DRK0,
    CRYPTO_KDF_HARD_KEY_TYPE_DRK1,
    CRYPTO_KDF_HARD_KEY_TYPE_RDRK0,
    CRYPTO_KDF_HARD_KEY_TYPE_RDRK1,
    CRYPTO_KDF_HARD_KEY_TYPE_PSK,
    CRYPTO_KDF_HARD_KEY_TYPE_ODRK0,
    CRYPTO_KDF_HARD_KEY_TYPE_ODRK1,
    CRYPTO_KDF_HARD_KEY_TYPE_OARK0,
    CRYPTO_KDF_HARD_KEY_TYPE_MDRK0,
    CRYPTO_KDF_HARD_KEY_TYPE_MDRK1,
    CRYPTO_KDF_HARD_KEY_TYPE_MDRK2,
    CRYPTO_KDF_HARD_KEY_TYPE_MDRK3,

    CRYPTO_KDF_HARD_KEY_TYPE_ABRK_REE,
    CRYPTO_KDF_HARD_KEY_TYPE_ABRK_TEE,
    CRYPTO_KDF_HARD_KEY_TYPE_RDRK_REE,
    CRYPTO_KDF_HARD_KEY_TYPE_RDRK_TEE,
    CRYPTO_KDF_HARD_KEY_TYPE_ERK_TEE,
    CRYPTO_KDF_HARD_KEY_TYPE_ERK_REE,
    CRYPTO_KDF_HARD_KEY_TYPE_ERK1_REE,
    CRYPTO_KDF_HARD_KEY_TYPE_ERK2_REE
} crypto_kdf_hard_key_type;

/* Define the algorithm of crypto engine. */
typedef enum {
    KM_CRYPTO_ALG_AES = 0x20,
    KM_CRYPTO_ALG_LEA = 0x40,        /* Flash online decryption and Cipher offline encryption */
    KM_CRYPTO_ALG_SM4 = 0x50,
    KM_CRYPTO_ALG_TDES = 0x70,       /* Do Not Recommend to Use */
    KM_CRYPTO_ALG_HMAC_SHA1 = 0xA0,  /* Do Not Recommend to Use */
    KM_CRYPTO_ALG_HMAC_SHA2 = 0xA1,
    KM_CRYPTO_ALG_HMAC_SM3 = 0xA2,
    KM_CRYPTO_ALG_MAX,
    KM_CRYPTO_ALG_INVALID = 0xffffffff,
} km_crypto_alg;

/* Define the structure of content key security configurations. */
/* when cipher work mode is CBC_MAC, dest_buf_sec_support and dest_buf_non_sec_support cannot be both false */
typedef struct {
    km_klad_sec key_sec; /* Secure key can only be used by TEE CPU and AIDSP locked cipher and hash channel */
    /* Only the cipher or hash channel which is locked by same CPU as keyladder can use this key,
    valid only for TEE CPU and AIDSP */
    td_bool master_only_enable;
    td_bool dest_buf_sec_support;     /* The destination buffer of target engine can be secure. */
    td_bool dest_buf_non_sec_support; /* The destination buffer of target engine can be non-secure. */
    td_bool src_buf_sec_support;      /* The source buffer of target engine can be secure. */
    td_bool src_buf_non_sec_support;  /* The source buffer of target engine can be non-secure. */
} km_klad_key_secure_config;

/* Define the keyladder level. */
typedef enum {
    KM_KLAD_LEVEL1 = 0,
    KM_KLAD_LEVEL_MAX,
    KM_KLAD_LEVEL_INVALID = 0xffffffff,
} km_klad_level;

/* Define the keyladder algorithm. */
typedef enum {
    KM_KLAD_ALG_TYPE_TDES = 0, /* Do Not Recommend to Use */
    KM_KLAD_ALG_TYPE_AES,
    KM_KLAD_ALG_TYPE_SM4,
    KM_KLAD_ALG_TYPE_MAX,
    KM_KLAD_ALG_TYPE_INVALID = 0xffffffff,
} km_klad_alg_type;

/* * Key parity, valid when key length is not more than 128bit */
/* * CNcomment: Key Parity Attribute */
typedef enum {
    KM_KLAD_KEY_EVEN =  0x0,   /* *< even key  */
    KM_KLAD_KEY_ODD  =  0x1,   /* *< odd key */
    KM_KLAD_KEY_PARITY_MAX,
    KM_KLAD_KEY_PARITY_INVALID = 0xffffffff,
} km_klad_key_parity;

typedef enum {
    KM_KLAD_HMAC_SHA1 = 0x20, /* Do Not Recommend to Use */
    KM_KLAD_HMAC_SHA224,      /* Software Kdf not support */
    KM_KLAD_HMAC_SHA256,
    KM_KLAD_HMAC_SHA384,
    KM_KLAD_HMAC_SHA512,
    KM_KLAD_HMAC_SM3 = 0x30,
    KM_KLAD_HMAC_MAX,
    KM_KLAD_HMAC_INVALID = 0xffffffff,
} km_klad_hmac_type;

/**
 * @brief  KDF key derivation, hash algorithm selection when the hardware PBKDF2 algorithm is used.
 */
typedef enum {
    CRYPTO_KDF_HARD_ALG_SHA256 = 0,
    CRYPTO_KDF_HARD_ALG_SM3,
    CRYPTO_KDF_HARD_ALG_MAX
} crypto_kdf_hard_alg;

/**
 * @brief  Symmetric key length. Determines the length of the final working key.
 */
typedef enum {
    CRYPTO_KLAD_KEY_SIZE_128BIT,
    CRYPTO_KLAD_KEY_SIZE_192BIT,
    CRYPTO_KLAD_KEY_SIZE_256BIT,
} crypto_klad_key_size;

/* Define the structure of content key configurations. */
typedef struct {
    km_crypto_alg engine; /* The content key can be used for which algorithm of the crypto engine. */
    td_bool decrypt_support;  /* The content key can be used for decrypting. */
    td_bool encrypt_support;  /* The content key can be used for encrypting. */
} km_klad_key_config;


/* soc keyslot */
/* Define the keyslot type. */
typedef enum {
    KM_KEYSLOT_TYPE_MCIPHER = 0x00,
    KM_KEYSLOT_TYPE_HMAC,
    KM_KEYSLOT_TYPE_MAX,
    KM_KEYSLOT_TYPE_INVALID = 0xffffffff,
} km_keyslot_type;

/* Define the destination type of keyladder. */
typedef enum {
    KM_KLAD_DEST_TYPE_MCIPHER = 0x00,   /* keyslot for mcipher */
    KM_KLAD_DEST_TYPE_HMAC,             /* keyslot for hmac */
    KM_KLAD_DEST_TYPE_FLASH,            /* flash controller, for flash on line decryption and authentication */
    KM_KLAD_DEST_TYPE_NPU,              /* for NPU */
    KM_KLAD_DEST_TYPE_MAX,
    KM_KLAD_DEST_TYPE_INVALID = 0xffffffff,
} km_klad_dest_type;

/* Structure of keyladder extend attributes. */
typedef struct {
    km_klad_config klad_cfg;          /* The keyladder configuration, valid for harware key. */
    km_klad_key_config key_cfg;       /* The content key configuration. */
    km_klad_key_secure_config key_sec_cfg; /* The content key security configuration. */
    td_u32 rkp_sw_cfg; /* The software configuration for NPU */
} km_klad_attr;

/* Structure of setting session key. */
typedef struct {
    km_klad_level level;              /* the level of session key. */
    km_klad_alg_type alg;             /* the algorithm used to decrypt session key. */
    td_u32 key_size;                      /* the size of session key, should be 16. */
    td_u8 *key;   /* the session key. */
} km_klad_session_key;

/* Structure of setting content key. */
typedef struct {
    td_u32 key_size;                      /* The size of content key, should be 16 or 32. */
    km_klad_alg_type alg;             /* The algorithm of the content key. */
    km_klad_key_parity key_parity;    /* Odd or even key flag. */
    td_u8 *key;                           /* The content key. */
} km_klad_content_key;

/* Structure of sending clear key. */
typedef struct {
    km_klad_hmac_type hmac_type;      /* hmac type, only send hmac key need to config. */
    td_u32 key_size;                      /* The size of content key, 16 or 32 for cipher,
                                             not more than block size for HMAC. */
    td_u8 *key;                           /* The content key. */
    km_klad_key_parity key_parity;    /* Odd or even key flag. */
} km_klad_clear_key;

typedef struct {
    crypto_kdf_hard_alg kdf_hard_alg;
    td_bool key_parity;
    crypto_klad_key_size key_size;
    td_u8 *salt;
    td_u32 salt_length;
    td_bool oneway;
} km_klad_effective_key;
#endif
