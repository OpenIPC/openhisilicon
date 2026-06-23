/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "mbedtls_harden_adapt.h"
#include "crypto_osal_user_lib.h"
#include "ot_mpi_sys_mem.h"
#include "uapi_hash.h"
#include "uapi_kdf.h"
#include "uapi_km.h"
#include "uapi_pke.h"
#include "uapi_symc.h"
#include "uapi_trng.h"
#include "uapi_otp.h"
#include "crypto_platform.h"

#define TEE_ENABLE_OFFSET        0x12
#define TEE_DISABLE_VALUE        0x42

typedef enum {
    KM_KEYSLOT_ENGINE_AES = 0,
    KM_KEYSLOT_ENGINE_SM4,
    KM_KEYSLOT_ENGINE_HMAC_SHA1,
    KM_KEYSLOT_ENGINE_HMAC_SHA224,
    KM_KEYSLOT_ENGINE_HMAC_SHA256,
    KM_KEYSLOT_ENGINE_HMAC_SHA384,
    KM_KEYSLOT_ENGINE_HMAC_SHA512,
    KM_KEYSLOT_ENGINE_HMAC_SM3
} km_keyslot_engine;

typedef struct {
    crypto_hash_type hmac_type;
    td_u8 *salt;
    td_u32 salt_length;
    td_u8 *ikm;
    td_u32 ikm_length;
} crypto_hkdf_extract_t;

typedef struct {
    crypto_hash_type hmac_type;
    td_u8 *prk;
    td_u32 prk_length;
    td_u8 *info;
    td_u32 info_length;
} crypto_hkdf_expand_t;

typedef struct {
    crypto_hash_type hmac_type;
    td_u8 *salt;
    td_u32 salt_length;
    td_u8 *ikm;
    td_u32 ikm_length;
    td_u8 *info;
    td_u32 info_length;
} crypto_hkdf_t;

/* HASH */
typedef td_s32 (*func_hash_init)(td_void);
typedef td_s32 (*func_hash_deinit)(td_void);
typedef td_s32 (*func_hash_start)(td_handle *uapi_hash_handle, const crypto_hash_attr *hash_attr);
typedef td_s32 (*func_hash_update)(td_handle uapi_hash_handle, const crypto_buf_attr *src_buf, const td_u32 len);
typedef td_s32 (*func_hash_get)(td_handle uapi_hash_handle, crypto_hash_clone_ctx *hash_clone_ctx);
typedef td_s32 (*func_hash_set)(td_handle uapi_hash_handle, const crypto_hash_clone_ctx *hash_clone_ctx);
typedef td_s32 (*func_hash_destroy)(td_handle uapi_hash_handle);
typedef td_s32 (*func_hash_finish)(td_handle uapi_hash_handle, td_u8 *out, td_u32 *out_len);

/* HKDF */
typedef td_s32 (*func_hkdf)(crypto_hkdf_t *hkdf_param, td_u8 *okm, td_u32 okm_length);
typedef td_s32 (*func_hkdf_extract)(crypto_hkdf_extract_t *extract_param, td_u8 *prk, td_u32 *prk_length);
typedef td_s32 (*func_hkdf_expand)(const crypto_hkdf_expand_t *expand_param, td_u8 *okm, td_u32 okm_length);

/* TRNG */
typedef td_s32 (*func_trng_get_random)(td_u32 *randnum);
typedef td_s32 (*func_trng_get_multi_random)(td_u32 size, td_u8 *randnum);

/* PBKDF2 */
typedef td_s32 (*func_pbkdf2)(const crypto_kdf_pbkdf2_param *param, td_u8 *out, const td_u32 out_len);

/* SYMC */
typedef td_s32 (*func_symc_init)(td_void);
typedef td_s32 (*func_symc_deinit)(td_void);
typedef td_s32 (*func_symc_create)(td_handle *symc_handle, const crypto_symc_attr *symc_attr);
typedef td_s32 (*func_symc_destroy)(td_handle symc_handle);
typedef td_s32 (*func_symc_set_config)(td_handle symc_handle, const crypto_symc_ctrl_t *symc_ctrl);
typedef td_s32 (*func_symc_get_config)(td_handle symc_handle, crypto_symc_ctrl_t *symc_ctrl);
typedef td_s32 (*func_symc_attach)(td_handle symc_handle, td_handle keyslot_handle);
typedef td_s32 (*func_symc_encrypt)(td_handle symc_handle, const crypto_buf_attr *src_buf,
    const crypto_buf_attr *dst_buf, td_u32 length);
typedef td_s32 (*func_symc_decrypt)(td_handle symc_handle, const crypto_buf_attr *src_buf,
    const crypto_buf_attr *dst_buf, td_u32 length);
typedef td_s32 (*func_symc_get_tag)(td_handle symc_handle, td_u8 *tag, td_u32 tag_length);
typedef td_s32 (*func_symc_mac_start)(td_handle *symc_handle, const crypto_symc_mac_attr *mac_attr);
typedef td_s32 (*func_symc_mac_update)(td_handle symc_handle, const crypto_buf_attr *src_buf, td_u32 length);
typedef td_s32 (*func_symc_mac_finish)(td_handle symc_handle, td_u8 *mac, td_u32 *mac_length);

/* PKE */
typedef td_s32 (*func_pke_init)(td_void);
typedef td_s32 (*func_pke_deinit)(td_void);
typedef td_s32 (*func_pke_mod)(const drv_pke_data *a, const drv_pke_data *p, const drv_pke_data *c);
typedef td_s32 (*func_pke_exp_mod)(const drv_pke_data *n, const drv_pke_data *k,
    const drv_pke_data *in, const drv_pke_data *out);
typedef td_s32 (*func_pke_ecc_gen_key)(drv_pke_ecc_curve_type curve_type, const drv_pke_data *input_priv_key,
    const drv_pke_data *output_priv_key, const drv_pke_ecc_point *output_pub_key);
typedef td_s32 (*func_pke_ecdsa_sign)(drv_pke_ecc_curve_type curve_type, const drv_pke_data *priv_key,
    const drv_pke_data *hash, const drv_pke_ecc_sig *sig);
typedef td_s32 (*func_pke_ecdsa_verify)(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *pub_key,
    const drv_pke_data *hash, const drv_pke_ecc_sig *sig);
typedef td_s32 (*func_pke_eddsa_sign)(drv_pke_ecc_curve_type curve_type, const drv_pke_data *priv_key,
    const drv_pke_msg *msg, const drv_pke_ecc_sig *sig);
typedef td_s32 (*func_pke_eddsa_verify)(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *pub_key,
    const drv_pke_msg *msg, const drv_pke_ecc_sig *sig);
typedef td_s32 (*func_pke_gen_ecdh_key)(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *input_pub_key,
    const drv_pke_data *input_priv_key, const drv_pke_data *output_shared_key);
typedef td_s32 (*func_pke_check_dot_on_curve)(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *pub_key,
    td_bool *is_on_curve);
typedef td_s32 (*func_pke_rsa_sign)(const drv_pke_rsa_priv_key *priv_key, drv_pke_rsa_scheme scheme,
    drv_pke_hash_type hash_type, const drv_pke_data *input_hash,
    drv_pke_data *sign);
typedef td_s32 (*func_pke_rsa_verify)(const drv_pke_rsa_pub_key *pub_key, drv_pke_rsa_scheme scheme,
    drv_pke_hash_type hash_type, drv_pke_data *input_hash, const drv_pke_data *sig);
typedef td_s32 (*func_pke_rsa_public_encrypt)(drv_pke_rsa_scheme scheme, drv_pke_hash_type hash_type,
    const drv_pke_rsa_pub_key *pub_key, const drv_pke_data *input, const drv_pke_data *label,
    drv_pke_data *output);
typedef td_s32 (*func_pke_rsa_private_decrypt)(drv_pke_rsa_scheme scheme, drv_pke_hash_type hash_type,
    const drv_pke_rsa_priv_key *priv_key, const drv_pke_data *input, const drv_pke_data *label,
    drv_pke_data *output);

/* KM */
typedef td_s32 (*func_km_init)(td_void);
typedef td_s32 (*func_km_deinit)(td_void);
typedef td_s32 (*func_km_create_keyslot)(td_handle *keyslot_handle, km_keyslot_engine key_engine);
typedef td_void (*func_km_destroy_keyslot)(td_handle keyslot_handle);
typedef td_s32 (*func_km_set_clear_key)(td_handle keyslot_handle, td_u8 *key, td_u32 keylen,
    km_keyslot_engine key_engine);

/* MEM */
typedef td_s32 (*func_alloc_phys_buf)(crypto_buf_attr *buf_attr, td_void **virt_addr, td_u32 size);
typedef td_void (*func_free_phys_buf)(crypto_buf_attr *buf_attr, td_void *virt_addr, td_u32 size);
typedef td_s32 (*func_get_phys_addr)(td_void *virt_addr, td_ulong *phys_addr);

typedef struct {
    func_hash_init harden_hash_init;
    func_hash_deinit harden_hash_deinit;
    func_hash_start harden_hash_start;
    func_hash_update harden_hash_update;
    func_hash_get harden_hash_get;
    func_hash_set harden_hash_set;
    func_hash_destroy harden_hash_destroy;
    func_hash_finish harden_hash_finish;
    func_hkdf harden_hkdf;
    func_hkdf_extract harden_hkdf_extract;
    func_hkdf_expand harden_hkdf_expand;
    func_trng_get_random harden_trng_get_random;
    func_trng_get_multi_random harden_trng_get_multi_random;
    func_pbkdf2 harden_pbkdf2;
    func_symc_init harden_symc_init;
    func_symc_deinit harden_symc_deinit;
    func_symc_create harden_symc_create;
    func_symc_destroy harden_symc_destroy;
    func_symc_set_config harden_symc_set_config;
    func_symc_get_config harden_symc_get_config;
    func_symc_attach harden_symc_attach;
    func_symc_encrypt harden_symc_encrypt;
    func_symc_decrypt harden_symc_decrypt;
    func_symc_get_tag harden_symc_get_tag;
    func_symc_mac_start harden_symc_mac_start;
    func_symc_mac_update harden_symc_mac_update;
    func_symc_mac_finish harden_symc_mac_finish;
    func_pke_init harden_pke_init;
    func_pke_deinit harden_pke_deinit;
    func_pke_mod harden_pke_mod;
    func_pke_exp_mod harden_pke_exp_mod;
    func_pke_ecc_gen_key harden_pke_ecc_gen_key;
    func_pke_ecdsa_sign harden_pke_ecdsa_sign;
    func_pke_ecdsa_verify harden_pke_ecdsa_verify;
    func_pke_eddsa_sign harden_pke_eddsa_sign;
    func_pke_eddsa_verify harden_pke_eddsa_verify;
    func_pke_gen_ecdh_key harden_pke_gen_ecdh_key;
    func_pke_check_dot_on_curve harden_pke_check_dot_on_curve;
    func_pke_rsa_sign harden_pke_rsa_sign;
    func_pke_rsa_verify harden_pke_rsa_verify;
    func_pke_rsa_public_encrypt harden_pke_rsa_public_encrypt;
    func_pke_rsa_private_decrypt harden_pke_rsa_private_decrypt;
} mbedtls_harden_cipher_func;

typedef struct {
    func_km_init harden_km_init;
    func_km_deinit harden_km_deinit;
    func_km_create_keyslot harden_km_create_keyslot;
    func_km_destroy_keyslot harden_km_destroy_keyslot;
    func_km_set_clear_key harden_km_set_clear_key;
} mbedtls_harden_km_func;

typedef struct {
    func_alloc_phys_buf harden_alloc_phys_buf;
    func_free_phys_buf harden_free_phys_buf;
    func_get_phys_addr harden_get_phys_addr;
} mbedtls_harden_mem_func;

td_void mbedtls_cipher_adapt_register_func(mbedtls_harden_cipher_func *harden_cipher_func);
td_void mbedtls_km_adapt_register_func(mbedtls_harden_km_func *harden_km_func);
td_void mbedtls_mem_adapt_register_func(mbedtls_harden_mem_func *harden_mem_func);

td_s32 mbedtls_adapt_create_keyslot(td_handle *keyslot_handle, km_keyslot_engine key_engine)
{
    td_s32 ret = TD_SUCCESS;
    crypto_handle ks_handle = 0;
    km_keyslot_type keyslot_type = KM_KEYSLOT_TYPE_MCIPHER;
    crypto_chk_return(keyslot_handle == TD_NULL, TD_FAILURE, "keyslot_handle is null\n");
    if (key_engine == KM_KEYSLOT_ENGINE_HMAC_SHA1 || key_engine == KM_KEYSLOT_ENGINE_HMAC_SHA224 ||
        key_engine == KM_KEYSLOT_ENGINE_HMAC_SHA256 || key_engine == KM_KEYSLOT_ENGINE_HMAC_SHA384 ||
        key_engine == KM_KEYSLOT_ENGINE_HMAC_SHA512 || key_engine == KM_KEYSLOT_ENGINE_HMAC_SM3) {
        keyslot_type = KM_KEYSLOT_TYPE_HMAC;
    }
    ret = uapi_keyslot_create(&ks_handle, keyslot_type);
    if (ret != TD_SUCCESS) {
        crypto_print("uapi_keyslot_create failed\n");
        return ret;
    }
    *keyslot_handle = (td_handle)ks_handle;
    return ret;
}

static td_s32 inner_set_klad_attr(km_klad_attr *klad_attr, km_keyslot_engine key_engine)
{
    td_u32 offset = TEE_ENABLE_OFFSET;
    td_u8 tee_enable = 0;
    klad_attr->key_cfg.decrypt_support = TD_TRUE;
    klad_attr->key_cfg.encrypt_support = TD_TRUE;
    (td_void)uapi_otp_init();
    (td_void)uapi_otp_read_byte(offset, &tee_enable);
    (td_void)uapi_otp_deinit();
    if (tee_enable != TEE_DISABLE_VALUE) {
        klad_attr->key_sec_cfg.key_sec = KM_KLAD_SEC_DISABLE;
        klad_attr->key_sec_cfg.master_only_enable = TD_FALSE;
        klad_attr->key_sec_cfg.dest_buf_sec_support = TD_FALSE;
        klad_attr->key_sec_cfg.src_buf_sec_support = TD_FALSE;
        klad_attr->key_sec_cfg.dest_buf_non_sec_support = TD_TRUE;
        klad_attr->key_sec_cfg.src_buf_non_sec_support = TD_TRUE;
    } else {
        klad_attr->key_sec_cfg.key_sec = KM_KLAD_SEC_ENABLE;
        klad_attr->key_sec_cfg.master_only_enable = TD_TRUE;
        klad_attr->key_sec_cfg.dest_buf_sec_support = TD_TRUE;
        klad_attr->key_sec_cfg.src_buf_sec_support = TD_TRUE;
        klad_attr->key_sec_cfg.dest_buf_non_sec_support = TD_FALSE;
        klad_attr->key_sec_cfg.src_buf_non_sec_support = TD_FALSE;
    }
    switch (key_engine) {
        case KM_KEYSLOT_ENGINE_AES:
            klad_attr->key_cfg.engine = KM_CRYPTO_ALG_AES;
            break;
        case KM_KEYSLOT_ENGINE_SM4:
            klad_attr->key_cfg.engine = KM_CRYPTO_ALG_SM4;
            break;
        case KM_KEYSLOT_ENGINE_HMAC_SHA1:
            klad_attr->key_cfg.engine = KM_CRYPTO_ALG_HMAC_SHA1;
            break;
        case KM_KEYSLOT_ENGINE_HMAC_SHA224:
        case KM_KEYSLOT_ENGINE_HMAC_SHA256:
        case KM_KEYSLOT_ENGINE_HMAC_SHA384:
        case KM_KEYSLOT_ENGINE_HMAC_SHA512:
            klad_attr->key_cfg.engine = KM_CRYPTO_ALG_HMAC_SHA2;
            break;
        case KM_KEYSLOT_ENGINE_HMAC_SM3:
            klad_attr->key_cfg.engine = KM_CRYPTO_ALG_HMAC_SM3;
            break;
        default:
            crypto_print("invalid keyslot_engine\n");
            return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 inner_set_key_cfg(km_klad_clear_key *clear_key, td_u8 *key, td_u32 keylen, km_keyslot_engine key_engine)
{
    clear_key->key = key;
    clear_key->key_size = keylen;
    switch (key_engine) {
        case KM_KEYSLOT_ENGINE_AES:
        case KM_KEYSLOT_ENGINE_SM4:
            break;
        case KM_KEYSLOT_ENGINE_HMAC_SHA1:
            clear_key->hmac_type = KM_KLAD_HMAC_SHA1;
            break;
        case KM_KEYSLOT_ENGINE_HMAC_SHA224:
            clear_key->hmac_type = KM_KLAD_HMAC_SHA224;
            break;
        case KM_KEYSLOT_ENGINE_HMAC_SHA256:
            clear_key->hmac_type = KM_KLAD_HMAC_SHA256;
            break;
        case KM_KEYSLOT_ENGINE_HMAC_SHA384:
            clear_key->hmac_type = KM_KLAD_HMAC_SHA384;
            break;
        case KM_KEYSLOT_ENGINE_HMAC_SHA512:
            clear_key->hmac_type = KM_KLAD_HMAC_SHA512;
            break;
        case KM_KEYSLOT_ENGINE_HMAC_SM3:
            clear_key->hmac_type = KM_KLAD_HMAC_SM3;
            break;
        default:
            crypto_print("invalid keyslot_engine\n");
            return TD_FAILURE;
    }
    return TD_SUCCESS;
}

td_s32 mbedtls_adapt_set_clear_key(td_handle keyslot_handle, td_u8 *key, td_u32 keylen, km_keyslot_engine key_engine)
{
    crypto_handle klad_handle = 0;
    td_s32 ret = TD_SUCCESS;
    km_klad_attr klad_attr = {0};
    km_klad_clear_key clear_key = {0};
    km_klad_dest_type klad_type = KM_KLAD_DEST_TYPE_MCIPHER;

    crypto_chk_return(key == TD_NULL, TD_FAILURE, "key is null\n");

    if (key_engine == KM_KEYSLOT_ENGINE_HMAC_SHA1 || key_engine == KM_KEYSLOT_ENGINE_HMAC_SHA224 ||
        key_engine == KM_KEYSLOT_ENGINE_HMAC_SHA256 || key_engine == KM_KEYSLOT_ENGINE_HMAC_SHA384 ||
        key_engine == KM_KEYSLOT_ENGINE_HMAC_SHA512 || key_engine == KM_KEYSLOT_ENGINE_HMAC_SM3) {
        klad_type = KM_KLAD_DEST_TYPE_HMAC;
    }

    ret = inner_set_klad_attr(&klad_attr, key_engine);
    crypto_chk_return(ret != TD_SUCCESS, TD_FAILURE);

    ret = inner_set_key_cfg(&clear_key, key, keylen, key_engine);
    crypto_chk_return(ret != TD_SUCCESS, TD_FAILURE);

    ret = uapi_klad_create(&klad_handle);
    crypto_chk_return(ret != TD_SUCCESS, TD_FAILURE, "uapi_klad_create failed\n");

    ret = uapi_klad_attach(klad_handle, klad_type, keyslot_handle);
    crypto_chk_goto(ret != TD_SUCCESS, error_destroy_klad, "uapi_klad_attach failed\n");

    ret = uapi_klad_set_attr(klad_handle, &klad_attr);
    crypto_chk_goto(ret != TD_SUCCESS, error_klad_detach, "uapi_klad_set_attr failed\n");

    ret = uapi_klad_set_clear_key(klad_handle, &clear_key);
    crypto_chk_goto(ret != TD_SUCCESS, error_klad_detach, "uapi_klad_set_clear_key failed\n");

    (td_void)uapi_klad_detach(klad_handle, klad_type, keyslot_handle);
    (td_void)uapi_klad_destroy(klad_handle);

    return ret;
error_klad_detach:
    (td_void)uapi_klad_detach(klad_handle, klad_type, keyslot_handle);
error_destroy_klad:
    (td_void)uapi_klad_destroy(klad_handle);
    return ret;
}

td_void mbedtls_adapt_destroy_keyslot(td_handle keyslot_handle)
{
    crypto_handle ks_handle = 0;
    ks_handle = keyslot_handle;
    (td_void)uapi_keyslot_destroy(ks_handle);
}

td_s32 mbedtls_adapt_alloc_buf_attr(crypto_buf_attr *buf_attr, td_void **virt_addr, td_u32 size)
{
    td_s32 ret;
    crypto_chk_return(buf_attr == TD_NULL, TD_FAILURE, "buf_attr is null\n");
    td_phys_addr_t phys_addr;
    ret = ot_mpi_sys_mmz_alloc(&phys_addr, virt_addr, NULL, NULL, size);
    if (ret != TD_SUCCESS) {
        crypto_print("uapi_sys_mmz_alloc failed\n");
        return -1;
    }
    buf_attr->phys_addr = (td_ulong)phys_addr;
    return TD_SUCCESS;
}

td_void mbedtls_adapt_free_buf_attr(crypto_buf_attr *buf_attr, td_void *virt_addr, td_u32 size)
{
    (td_void)(size);
    if (buf_attr == TD_NULL) {
        crypto_print("buf_attr is null\n");
        return;
    }
    if (virt_addr != TD_NULL) {
        (td_void)ot_mpi_sys_mmz_free((td_phys_addr_t)buf_attr->phys_addr, virt_addr);
    }
}

td_s32 mbedtls_adapt_get_phys_mem(td_void *virt_addr, td_ulong *phys_addr)
{
    td_s32 ret;
    ot_sys_mem_info mem_info = {0};
    crypto_chk_return(phys_addr == TD_NULL, TD_FAILURE, "phys_addr is null\n");
    ret = ot_mpi_sys_get_mem_info_by_virt(virt_addr, &mem_info);
    if (ret == TD_SUCCESS) {
        *phys_addr = (td_ulong)mem_info.phys_addr + (td_ulong)mem_info.offset;
        return ret;
    }
    return TD_FAILURE;
}

mbedtls_harden_cipher_func harden_cipher_func = {
    .harden_hash_init = unify_uapi_cipher_hash_init,
    .harden_hash_deinit = unify_uapi_cipher_hash_deinit,
    .harden_hash_start = unify_uapi_cipher_hash_start,
    .harden_hash_update = unify_uapi_cipher_hash_update,
    .harden_hash_get = unify_uapi_cipher_hash_get,
    .harden_hash_set = unify_uapi_cipher_hash_set,
    .harden_hash_destroy = unify_uapi_cipher_hash_destroy,
    .harden_hash_finish = unify_uapi_cipher_hash_finish,
    .harden_hkdf = TD_NULL,
    .harden_hkdf_extract = TD_NULL,
    .harden_hkdf_expand = TD_NULL,
    .harden_trng_get_random = unify_uapi_cipher_trng_get_random,
    .harden_trng_get_multi_random = unify_uapi_cipher_trng_get_multi_random,
    .harden_pbkdf2 = unify_uapi_cipher_pbkdf2,
    .harden_symc_init = unify_uapi_cipher_symc_init,
    .harden_symc_deinit = unify_uapi_cipher_symc_deinit,
    .harden_symc_create = unify_uapi_cipher_symc_create,
    .harden_symc_destroy = unify_uapi_cipher_symc_destroy,
    .harden_symc_set_config = unify_uapi_cipher_symc_set_config,
    .harden_symc_get_config = unify_uapi_cipher_symc_get_config,
    .harden_symc_attach = unify_uapi_cipher_symc_attach,
    .harden_symc_encrypt = unify_uapi_cipher_symc_encrypt,
    .harden_symc_decrypt = unify_uapi_cipher_symc_decrypt,
    .harden_symc_get_tag= unify_uapi_cipher_symc_get_tag,
    .harden_symc_mac_start = unify_uapi_cipher_mac_start,
    .harden_symc_mac_update = unify_uapi_cipher_mac_update,
    .harden_symc_mac_finish = unify_uapi_cipher_mac_finish,
    .harden_pke_init = unify_uapi_cipher_pke_init,
    .harden_pke_deinit = unify_uapi_cipher_pke_deinit,
    .harden_pke_mod = unify_uapi_cipher_pke_mod,
    .harden_pke_exp_mod = unify_uapi_cipher_pke_exp_mod,
    .harden_pke_ecc_gen_key = unify_uapi_cipher_pke_ecc_gen_key,
    .harden_pke_ecdsa_sign = unify_uapi_cipher_pke_ecdsa_sign,
    .harden_pke_ecdsa_verify = unify_uapi_cipher_pke_ecdsa_verify,
    .harden_pke_eddsa_sign = unify_uapi_cipher_pke_eddsa_sign,
    .harden_pke_eddsa_verify = unify_uapi_cipher_pke_eddsa_verify,
    .harden_pke_gen_ecdh_key = unify_uapi_cipher_pke_ecc_gen_ecdh_key,
    .harden_pke_check_dot_on_curve = unify_uapi_cipher_pke_check_dot_on_curve,
    .harden_pke_rsa_sign = unify_uapi_cipher_pke_rsa_sign,
    .harden_pke_rsa_verify = unify_uapi_cipher_pke_rsa_verify,
    .harden_pke_rsa_public_encrypt = unify_uapi_cipher_pke_rsa_public_encrypt,
    .harden_pke_rsa_private_decrypt = unify_uapi_cipher_pke_rsa_private_decrypt,
};

mbedtls_harden_km_func harden_km_func = {
    .harden_km_init = uapi_km_init,
    .harden_km_deinit = uapi_km_deinit,
    .harden_km_create_keyslot = mbedtls_adapt_create_keyslot,
    .harden_km_destroy_keyslot = mbedtls_adapt_destroy_keyslot,
    .harden_km_set_clear_key = mbedtls_adapt_set_clear_key,
};

mbedtls_harden_mem_func harden_mem_func = {
    .harden_alloc_phys_buf = mbedtls_adapt_alloc_buf_attr,
    .harden_free_phys_buf = mbedtls_adapt_free_buf_attr,
    .harden_get_phys_addr = mbedtls_adapt_get_phys_mem,
};

td_s32 mbedtls_adapt_register_func()
{
    mbedtls_cipher_adapt_register_func(&harden_cipher_func);
    mbedtls_km_adapt_register_func(&harden_km_func);
    mbedtls_mem_adapt_register_func(&harden_mem_func);
    return TD_SUCCESS;
}