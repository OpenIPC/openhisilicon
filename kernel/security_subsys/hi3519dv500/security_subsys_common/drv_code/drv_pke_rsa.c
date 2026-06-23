/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "hal_pke.h"
#include "drv_pke.h"
#include "crypto_hash_struct.h"
#include "securec.h"
#include "drv_hash.h"
#include "drv_trng.h"
#include "crypto_drv_common.h"
#include "drv_inner.h"

#define RANDOM_TRY_TIME                 100
#define RSA_GEN_KEY_WAIT_TIMES          10
#define RSA_GEN_KEY_TRY_TIMES           10
#define BYTE_MAX                        0xFF
#define PKE_COMPAT_ERRNO(err_code)      DISPATCH_COMPAT_ERRNO(ERROR_MODULE_PKE, err_code)

#define pke_null_ptr_chk(ptr)   \
    crypto_chk_return((ptr) == TD_NULL, PKE_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), #ptr" is NULL\n")

// hash ASN.1
static const td_u8 g_asn1_sha1[] = {
    0x30, 0x21, 0x30, 0x09, 0x06, 0x05, 0x2b, 0x0e,
    0x03, 0x02, 0x1a, 0x05, 0x00, 0x04, 0x14,
};

static const td_u8 g_asn1_sha256[] = {
    0x30, 0x31, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86,
    0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x01, 0x05,
    0x00, 0x04, 0x20,
};

static const td_u8 g_asn1_sha384[] = {
    0x30, 0x41, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86,
    0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x02, 0x05,
    0x00, 0x04, 0x30,
};

static const td_u8 g_asn1_sha512[] = {
    0x30, 0x51, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86,
    0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x03, 0x05,
    0x00, 0x04, 0x40,
};

static const td_u8 g_empty_l_sha1[] = {
    0xda, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d,
    0x32, 0x55, 0xbf, 0xef, 0x95, 0x60, 0x18, 0x90,
    0xaf, 0xd8, 0x07, 0x09,
};

static const td_u8 g_empty_l_sha256[] = {
    0xe3, 0xb0, 0xc4, 0x42, 0x98, 0xfc, 0x1c, 0x14,
    0x9a, 0xfb, 0xf4, 0xc8, 0x99, 0x6f, 0xb9, 0x24,
    0x27, 0xae, 0x41, 0xe4, 0x64, 0x9b, 0x93, 0x4c,
    0xa4, 0x95, 0x99, 0x1b, 0x78, 0x52, 0xb8, 0x55,
};

static const td_u8 g_empty_l_sha384[] = {
    0x38, 0xb0, 0x60, 0xa7, 0x51, 0xac, 0x96, 0x38,
    0x4c, 0xd9, 0x32, 0x7e, 0xb1, 0xb1, 0xe3, 0x6a,
    0x21, 0xfd, 0xb7, 0x11, 0x14, 0xbe, 0x07, 0x43,
    0x4c, 0x0c, 0xc7, 0xbf, 0x63, 0xf6, 0xe1, 0xda,
    0x27, 0x4e, 0xde, 0xbf, 0xe7, 0x6f, 0x65, 0xfb,
    0xd5, 0x1a, 0xd2, 0xf1, 0x48, 0x98, 0xb9, 0x5b,
};

static const td_u8 g_empty_l_sha512[] = {
    0xcf, 0x83, 0xe1, 0x35, 0x7e, 0xef, 0xb8, 0xbd,
    0xf1, 0x54, 0x28, 0x50, 0xd6, 0x6d, 0x80, 0x07,
    0xd6, 0x20, 0xe4, 0x05, 0x0b, 0x57, 0x15, 0xdc,
    0x83, 0xf4, 0xa9, 0x21, 0xd3, 0x6c, 0xe9, 0xce,
    0x47, 0xd0, 0xd1, 0x3c, 0x5d, 0x85, 0xf2, 0xb0,
    0xff, 0x83, 0x18, 0xd2, 0x87, 0x7e, 0xec, 0x2f,
    0x63, 0xb9, 0x31, 0xbd, 0x47, 0x41, 0x7a, 0x81,
    0xa5, 0x38, 0x32, 0x7a, 0xf9, 0x27, 0xda, 0x3e,
};

static const td_s8 g_bits[] = {0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4};

static td_u8 g_l_hash[HASH_SIZE_SHA_512] = {0x0};

static const pke_hash_properties_type g_pke_hash_properties[] = {
    {DRV_PKE_HASH_TYPE_SHA1, SHA1_BLOCK_LENGTH, SHA1_RESULT_LENGTH},
    {DRV_PKE_HASH_TYPE_SHA224, SHA224_BLOCK_LENGTH, SHA224_RESULT_LENGTH},
    {DRV_PKE_HASH_TYPE_SHA256, SHA256_BLOCK_LENGTH, SHA256_RESULT_LENGTH},
    {DRV_PKE_HASH_TYPE_SHA384, SHA384_BLOCK_LENGTH, SHA384_RESULT_LENGTH},
    {DRV_PKE_HASH_TYPE_SHA512, SHA512_BLOCK_LENGTH, SHA512_RESULT_LENGTH},
    {DRV_PKE_HASH_TYPE_SM3, SM3_BLOCK_LENGTH, SM3_RESULT_LENGTH}
};

static td_s32 drv_get_hash_len(const drv_pke_hash_type hash_type)
{
    td_u32 i = 0;

    for (i = 0; i < sizeof(g_pke_hash_properties) / sizeof(pke_hash_properties_type); i++) {
        if (g_pke_hash_properties[i].hash_type == hash_type) {
            return g_pke_hash_properties[i].result_len;
        }
    }
    return 0;
}

static td_s32 rsa_exp_mod(const td_u8 *n, const td_u8 *k, const td_u32 klen,
    const drv_pke_data *in, const drv_pke_data *out)
{
    td_s32 ret = TD_FAILURE;
    drv_pke_data n_data = {klen, (td_u8 *)n};
    drv_pke_data k_data = {klen, (td_u8 *)k};

    ret = drv_cipher_pke_exp_mod(&n_data, &k_data, in, out);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_exp_mod failed, ret is 0x%x\n", ret);
exit__:
    (void)drv_cipher_pke_clean_ram();
    return ret;
}

static td_s32 get_random(td_u8 *rand, const td_u32 size)
{
    td_s32 ret = TD_FAILURE;
    td_u32 k = 0;
    td_u32 i = 0;
    td_u32 randnum = 0;

    ret = drv_cipher_trng_get_multi_random(size, rand);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_trng_get_multi_random failed, ret is 0x%x\n", ret);

    k = 0;
    while (k < size) { // It's almost impossible to randomize 100 times and not get a non-zero value.
        if (rand[k] != 0x00) {
            i = 0;
            k++;
            continue;
        }
        crypto_chk_return(i == RANDOM_TRY_TIME, PKE_COMPAT_ERRNO(ERROR_TRY_TIMES),
            "drv_cipher_trng_get_random failed!\n");
        i++;
        ret = drv_cipher_trng_get_random(&randnum);
        crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_trng_get_random failed, ret is 0x%x\n", ret);
        rand[k] = (td_u8)(randnum & BYTE_MAX);
    }
    return ret;
}

static td_u32 rsa_get_bit_num(const td_u8 *big_num, td_u32 num_len)
{
    td_u32 i = 0;
    td_s8 num = 0;

    for (i = 0; i < num_len; i++) {
        if (big_num[i] == 0x00) {
            continue;
        }
        num = g_bits[(big_num[i] & (MAX_LOW_8BITS - MAX_LOW_4BITS)) >> SHIFT_4BITS];
        if (num > 0) {
            return (num_len - i - BOUND_VALUE_1) * BYTE_BITS + num + DRV_WORD_WIDTH;
        }

        num = g_bits[big_num[i] & MAX_LOW_4BITS];
        if (num > 0) {
            return (num_len - i - BOUND_VALUE_1) * BYTE_BITS + num;
        }
    }
    return 0;
}

static td_s32 pkcs1_mgf(const rsa_pkcs1_hash_info *hash_info, const td_u8 *seed, const td_u32 seed_len,
    td_u8 *mask, const td_u32 mask_len)
{
    td_s32 ret = TD_FAILURE;
    td_u32 i = 0;
    td_u32 j = 0;
    td_u32 out_len = 0;
    td_u8 cnt[DRV_WORD_WIDTH];
    td_u8 md[HASH_SIZE_SHA_MAX];
    drv_pke_data hash = {0};
    td_u8 seed_buf[DRV_PKE_LEN_4096];
    drv_pke_data arr[2];

    (void)memset_s(cnt, DRV_WORD_WIDTH, 0x00, DRV_WORD_WIDTH);
    (void)memset_s(md, HASH_SIZE_SHA_MAX, 0x00, HASH_SIZE_SHA_MAX);
    ret = memcpy_s(seed_buf, DRV_PKE_LEN_4096, seed, seed_len);
    crypto_chk_return(ret != TD_SUCCESS, ret, "memcpy_s failed, ret is 0x%x\n", ret);

    hash.length = HASH_SIZE_SHA_MAX;
    hash.data = md;

    arr[0].data = seed_buf;
    arr[0].length = seed_len;
    arr[1].data = cnt;
    arr[1].length = (uintptr_t)sizeof(cnt);

    for (i = 0; out_len < mask_len; i++) {
        cnt[WORD_INDEX_0] = (td_u8)((i >> SHIFT_24BITS) & MAX_LOW_8BITS);
        cnt[WORD_INDEX_1] = (td_u8)((i >> SHIFT_16BITS) & MAX_LOW_8BITS);
        cnt[WORD_INDEX_2] = (td_u8)((i >> SHIFT_8BITS)) & MAX_LOW_8BITS;
        cnt[WORD_INDEX_3] = (td_u8)(i & MAX_LOW_8BITS);

        /* H = Hash(seedbuf || cnt) */
        ret = drv_cipher_pke_calc_hash(arr, sizeof(arr) / sizeof(arr[0]), hash_info->hash_type, &hash);
        crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_calc_hash failed, ret is 0x%x\n", ret);

        for (j = 0; (j < hash_info->hash_len) && (out_len < mask_len); j++) {
            mask[out_len++] ^= md[j];
        }
    }
    return ret;
}

static td_s32 pkcs1_get_hash(const drv_pke_hash_type hash_type, const drv_pke_data *label,
    rsa_pkcs1_hash_info *hash_info)
{
    td_s32 ret = TD_FAILURE;
    drv_pke_data h_hash = {0};
    switch (hash_type) {
        case DRV_PKE_HASH_TYPE_SHA1:
            hash_info->hash_type = DRV_PKE_HASH_TYPE_SHA1;
            hash_info->hash_len = HASH_SIZE_SHA_1;
            hash_info->asn1_data = (td_u8 *)g_asn1_sha1;
            hash_info->asn1_len = sizeof(g_asn1_sha1);
            hash_info->lhash_data = (td_u8 *)g_empty_l_sha1;
            break;
        case DRV_PKE_HASH_TYPE_SHA256:
            hash_info->hash_type = DRV_PKE_HASH_TYPE_SHA256;
            hash_info->hash_len = HASH_SIZE_SHA_256;
            hash_info->asn1_data = (td_u8 *)g_asn1_sha256;
            hash_info->asn1_len = sizeof(g_asn1_sha256);
            hash_info->lhash_data = (td_u8 *)g_empty_l_sha256;
            break;
        case DRV_PKE_HASH_TYPE_SHA384:
            hash_info->hash_type = DRV_PKE_HASH_TYPE_SHA384;
            hash_info->hash_len = HASH_SIZE_SHA_384;
            hash_info->asn1_data = (td_u8 *)g_asn1_sha384;
            hash_info->asn1_len = sizeof(g_asn1_sha384);
            hash_info->lhash_data = (td_u8 *)g_empty_l_sha384;
            break;
        case DRV_PKE_HASH_TYPE_SHA512:
            hash_info->hash_type = DRV_PKE_HASH_TYPE_SHA512;
            hash_info->hash_len = HASH_SIZE_SHA_512;
            hash_info->asn1_data = (td_u8 *)g_asn1_sha512;
            hash_info->asn1_len = sizeof(g_asn1_sha512);
            hash_info->lhash_data = (td_u8 *)g_empty_l_sha512;
            break;
        default:
            crypto_log_err("unsupport hash type\n");
            return PKE_COMPAT_ERRNO(ERROR_UNSUPPORT);
    }

    if (label != TD_NULL && label->data != TD_NULL && label->length != 0) {
        hash_info->lhash_data = g_l_hash;
        h_hash.data = hash_info->lhash_data;
        h_hash.length = hash_info->hash_len;

        ret = drv_cipher_pke_calc_hash(label, 1, hash_info->hash_type, &h_hash);
        crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_calc_hash failed, ret is 0x%x\n", ret);
    }

    return TD_SUCCESS;
}

/* H = Hash( M' ) = Hash( Padding1 || mHash || salt ) */
static td_s32 pkcs1_pss_hash(const rsa_pkcs1_hash_info *hash_info, const td_u8 *m_hash, td_u32 klen,
    const drv_pke_data *salt, td_u8 *hash)
{
    td_s32 ret = TD_FAILURE;
    td_u8 ps[RSA_PADLEN_8] = {0};
    td_u32 h_len = HASH_SIZE_SHA_MAX;
    drv_pke_data h_hash = {0};
    drv_pke_data arr[3]; // The capacity of the array is 3.

    h_hash.length = h_len;
    h_hash.data = hash;

    /* H = Hash(PS || MHash || SALT) */
    arr[0].data = ps; // 0 th element is ps
    arr[0].length = RSA_PADLEN_8; // 0 element is PS
    arr[1].data = (td_u8 *)m_hash; // 1 element is MHash
    arr[1].length = klen; // 1 element is MHash
    arr[2].data = (td_u8 *)salt->data; // 2 element is SALT
    arr[2].length = salt->length; // 2 element is SALT
    ret = drv_cipher_pke_calc_hash(arr, sizeof(arr) / sizeof(arr[0]), hash_info->hash_type, &h_hash);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_calc_hash failed, ret is 0x%x\n", ret);

    return ret;
}

#if defined(CONFIG_PKE_RSA_V15_SUPPORT)
/* EM = 00 || 01 || PS || 00 || T */
static td_s32 pkcs1_v15_sign(const drv_pke_hash_type hash_type, const rsa_pkcs1_pack *pack)
{
    td_s32 ret = TD_FAILURE;
    rsa_pkcs1_hash_info hash_info = {0};
    td_u8 *p = TD_NULL;
    td_u32 idx = 0;

    ret = pkcs1_get_hash(hash_type, TD_NULL, &hash_info);
    crypto_chk_return(ret != TD_SUCCESS, ret, "pkcs1_get_hash failed, ret is 0x%x\n", ret);

    p = pack->em;
    p[idx++] = 0x00;
    p[idx++] = 0x01;

    /* PS */
    while (idx < (pack->em_len - hash_info.hash_len - hash_info.asn1_len - 1)) {
        p[idx++] = BYTE_MAX;
    }

    p[idx++] = 0x00;

    /* T */
    ret = memcpy_s(&p[idx], pack->em_len - idx, hash_info.asn1_data, hash_info.asn1_len);
    crypto_chk_return(ret != TD_SUCCESS, ret, "memcpy_s failed, ret is 0x%x\n", ret);
    idx += hash_info.asn1_len;
    ret = memcpy_s(&p[idx], pack->em_len - idx, pack->hash, hash_info.hash_len);
    crypto_chk_return(ret != TD_SUCCESS, ret, "memcpy_s failed, ret is 0x%x\n", ret);

    return ret;
}

/* check EM = 00 || 01 || PS || 00 || T */
static td_s32 pkcs1_v15_verify(const drv_pke_hash_type hash_type, const rsa_pkcs1_pack *pack)
{
    td_s32 ret = TD_FAILURE;
    rsa_pkcs1_hash_info hash_info = {0};
    td_u8 *p = TD_NULL;
    td_u32 idx = 0;

    ret = pkcs1_get_hash(hash_type, TD_NULL, &hash_info);
    crypto_chk_return(ret != TD_SUCCESS, ret, "pkcs1_get_hash failed, ret is 0x%x\n", ret);

    p = pack->em;
    if (p[idx++] != 0x00) {
        return PKE_COMPAT_ERRNO(ERROR_PKE_RSA_VERIFY_V15_CHECK);
    }
    if (p[idx++] != 0x01) {
        return PKE_COMPAT_ERRNO(ERROR_PKE_RSA_VERIFY_V15_CHECK);
    }

    /* PS */
    while (idx < (pack->em_len - hash_info.hash_len - hash_info.asn1_len - 1)) {
        if (p[idx++] != BYTE_MAX) {
            return PKE_COMPAT_ERRNO(ERROR_PKE_RSA_VERIFY_V15_CHECK);
        }
    }
    if (p[idx++] != 0x00) {
        return PKE_COMPAT_ERRNO(ERROR_PKE_RSA_VERIFY_V15_CHECK);
    }

    /* T */
    if (memcmp(&p[idx], hash_info.asn1_data, hash_info.asn1_len) != 0) {
        crypto_log_err("check v15 asn1 failed!\n");
        return PKE_COMPAT_ERRNO(ERROR_PKE_RSA_VERIFY_V15_CHECK);
    }

    idx += hash_info.asn1_len;
    if (memcmp(&p[idx], pack->hash, hash_info.hash_len) != 0) {
        crypto_log_err("check v15 hash failed!\n");
        return PKE_COMPAT_ERRNO(ERROR_PKE_RSA_VERIFY_V15_CHECK);
    }

    return ret;
}
#endif

static td_s32 pkcs1_pss_sign(const drv_pke_hash_type hash_type, const rsa_pkcs1_pack *pack)
{
    td_s32 ret = TD_FAILURE;
    rsa_pkcs1_hash_info hash_info = {0};
    td_u8 salt_data[HASH_SIZE_SHA_MAX];
    td_u32 salt_len = 0;
    td_u32 tmp_len = 0;
    td_u32 ms_bits = 0;
    td_u8 *masked_db = TD_NULL;
    td_u32 masked_db_len = pack->em_len - pack->hash_len - 1; /* 1 byte for bound. */
    td_u8 *masked_seed = TD_NULL;
    drv_pke_data salt = {0};

    ret = pkcs1_get_hash(hash_type, TD_NULL, &hash_info);
    crypto_chk_return(ret != TD_SUCCESS, ret, "pkcs1_get_hash failed, ret is 0x%x\n", ret);

    salt_len = hash_info.hash_len;

    /* em_bit is the number of bits of key n. */
    ms_bits = (pack->em_bit - BOUND_VALUE_1) & MAX_LOW_3BITS;

    ret = get_random((td_u8 *)salt_data, salt_len);
    crypto_chk_return(ret != TD_SUCCESS, ret, "get_random failed, ret is 0x%x\n", ret);

    /* EM = masked_db || masked_seed || 0xbc */
    masked_db = pack->em;
    masked_seed = pack->em + masked_db_len;

    salt.data = salt_data;
    salt.length = salt_len;
    ret = pkcs1_pss_hash(&hash_info, pack->hash, pack->hash_len, &salt, masked_seed);
    crypto_chk_return(ret != TD_SUCCESS, ret, "pkcs1_pss_hash failed, ret is 0x%x\n", ret);

    /* DB = PS || 0x01 || salt */
    /* set PS, here tmp_len means the length of PS */
    tmp_len = pack->em_len - pack->hash_len - salt_len - RSA_PADLEN_2; /* padding2.length - 1 */
    (void)memset_s(masked_db, tmp_len, 0x00, tmp_len);

    /* set 0x01 after PS */
    masked_db[tmp_len++] = 0x01;

    /* set salt */
    ret = memcpy_s(masked_db + tmp_len, pack->em_len - tmp_len, salt_data, salt_len);
    crypto_chk_return(ret != TD_SUCCESS, ret, "memcpy_s failed, ret is 0x%x\n", ret);

    ret = pkcs1_mgf(&hash_info, masked_seed, pack->hash_len, masked_db, masked_db_len);
    crypto_chk_return(ret != TD_SUCCESS, ret, "pkcs1_mgf failed, ret is 0x%x\n", ret);

    pack->em[0] &= MAX_LOW_8BITS >> (BYTE_BITS - ms_bits);
    pack->em[pack->em_len - 1] = RSA_PAD_XBC; /* 1 byte for bound. */
    return ret;
}

static td_s32 pkcs1_pss_verify(const drv_pke_hash_type hash_type, const rsa_pkcs1_pack *pack)
{
    td_s32 ret = TD_FAILURE;
    rsa_pkcs1_hash_info hash_info = {0};
    td_u8 em[DRV_PKE_LEN_4096];
    td_u8 hash[HASH_SIZE_SHA_MAX];
    td_u32 salt_len = 0;
    td_u32 ms_bits = 0;
    td_u8 *masked_db = TD_NULL;
    td_u32 masked_db_len = pack->em_len - pack->hash_len - 1; /* 1 byte for bound. */
    td_u8 *masked_seed = TD_NULL;
    drv_pke_data salt = {0};

    ret = pkcs1_get_hash(hash_type, TD_NULL, &hash_info);
    crypto_chk_return(ret != TD_SUCCESS, ret, "pkcs1_get_hash failed, ret is 0x%x\n", ret);

    if (pack->em[pack->em_len - 1] != RSA_PAD_XBC) {
        return PKE_COMPAT_ERRNO(ERROR_PKE_RSA_VERIFY_PSS_CHECK);
    }

    ms_bits = (pack->em_bit - BOUND_VALUE_1) & MAX_LOW_3BITS;
    if ((pack->em[0] & (MAX_LOW_8BITS << ms_bits))) {
        return PKE_COMPAT_ERRNO(ERROR_PKE_RSA_VERIFY_PSS_CHECK);
    }

    ret = memcpy_s(em, DRV_PKE_LEN_4096, pack->em, pack->em_len);
    crypto_chk_return(ret != TD_SUCCESS, ret, "memcpy_s failed, ret is 0x%x\n", ret);

    masked_db = em;
    masked_seed = em + masked_db_len;
    ret = pkcs1_mgf(&hash_info, masked_seed, pack->hash_len, masked_db, masked_db_len);
    crypto_chk_return(ret != TD_SUCCESS, ret, "pkcs1_mgf failed, ret is 0x%x\n", ret);

    em[0] &= MAX_LOW_8BITS >> (BYTE_BITS - ms_bits);

    while (masked_db < masked_seed - 1 && *masked_db == 0) {
        masked_db++;
    }

    if (*masked_db++ != 0x01) {
        crypto_log_err("pss padding check failed!\n");
        return PKE_COMPAT_ERRNO(ERROR_PKE_RSA_VERIFY_PSS_CHECK);
    }

    salt_len = masked_seed - masked_db;
    salt.length = salt_len;
    salt.data = masked_db;

    ret = pkcs1_pss_hash(&hash_info, pack->hash, pack->hash_len, &salt, (td_u8 *)hash);
    crypto_chk_return(ret != TD_SUCCESS, ret, "pkcs1_pss_hash failed, ret is 0x%x\n", ret);

    if (memcmp(masked_seed, hash, hash_info.hash_len) != 0) {
        crypto_log_err("pss hash check failed!\n");
        return PKE_COMPAT_ERRNO(ERROR_PKE_RSA_VERIFY_PSS_CHECK);
    }

    return ret;
}

#if defined(CONFIG_PKE_RSA_V15_SUPPORT)
/* EM = 00 || 02 || PS || 00 || M */
static td_s32 pkcs1_v15_encrypt(const rsa_pkcs1_pack *pack)
{
    td_s32 ret = TD_FAILURE;
    td_u8 *p = TD_NULL;
    td_u32 idx = 0;
    td_u32 ps_len = pack->em_len - pack->data_len - RSA_PADLEN_3;

    p = pack->em;
    p[idx++] = 0x00;
    p[idx++] = 0x02; // 0x02 see comment above func

    /* PS */
    ret = get_random(&p[idx], ps_len);
    if (ret != TD_SUCCESS) {
        crypto_log_err("get random ps failed!\n");
        return ret;
    }
    idx += ps_len;

    p[idx++] = 0x00;
    /* M */
    if (pack->data_len != 0) {
        ret = memcpy_s(&p[idx], pack->em_len - idx, pack->data, pack->data_len);
        crypto_chk_return(ret != TD_SUCCESS, ret, "memcpy_s failed, ret is 0x%x\n", ret);
    }

    return ret;
}

/* check EM = 00 || 02 || PS || 00 || M */
static td_s32 pkcs1_v15_decrypt(rsa_pkcs1_pack *pack)
{
    td_s32 ret = TD_FAILURE;
    td_u8 *p = TD_NULL;
    td_u32 idx = 0;
    td_u32 len = 0;

    p = pack->em;
    if (p[idx++] != 0x00) {
        crypto_log_err("padding check failed!\n");
        return PKE_COMPAT_ERRNO(ERROR_PKE_RSA_CRYPTO_V15_CHECK);
    }

    if (p[idx++] != 0x02) { // 0x02 see comment above func
        crypto_log_err("padding check failed!\n");
        return PKE_COMPAT_ERRNO(ERROR_PKE_RSA_CRYPTO_V15_CHECK);
    }

    /* PS */
    while ((idx < (pack->em_len - RSA_PADLEN_1)) && p[idx] != 0x00) {
        idx++;
    }

    if (p[idx++] != 0x00) {
        crypto_log_err("padding check failed!\n");
        return PKE_COMPAT_ERRNO(ERROR_PKE_RSA_CRYPTO_V15_CHECK);
    }

    len = pack->em_len - idx;
    if (len > pack->em_len - RSA_PADLEN_11) {
        crypto_log_err("padding check failed!\n");
        return PKE_COMPAT_ERRNO(ERROR_PKE_RSA_CRYPTO_V15_CHECK);
    }

    /* M */
    if (len != 0) {
        ret = memcpy_s(pack->data, pack->data_len, &p[idx], len);
        crypto_chk_return(ret != TD_SUCCESS, ret, "memcpy_s failed, ret is 0x%x\n", ret);
    }
    pack->data_len = len; // record length of plain text for return

    return TD_SUCCESS;
}
#endif

static td_s32 pkcs1_oaep_encrypt(const drv_pke_hash_type hash_type, const rsa_pkcs1_pack *pack,
    const drv_pke_data *label)
{
    td_s32 ret = TD_FAILURE;
    rsa_pkcs1_hash_info hash_info = {0};
    td_u8 *p = TD_NULL;
    td_u8 *db = TD_NULL;
    td_u8 *seed = TD_NULL;
    td_u32 idx = 0;
    td_u32 hash_len = 0;
    td_u32 tmp_len = 0;

    ret = pkcs1_get_hash(hash_type, label, &hash_info);
    crypto_chk_return(ret != TD_SUCCESS, ret, "pkcs1_get_hash failed, ret is 0x%x\n", ret);
    hash_len = hash_info.hash_len;

    p = pack->em;
    seed = p + 1;
    db = p + hash_len + 1;

    /* 1. set data[0] = 0 */
    p[idx++] = 0x00;

    /* 2. set data[1, hash_len + 1] = random */
    ret = get_random(&p[idx], hash_len);
    crypto_chk_return(ret != TD_SUCCESS, ret, "get_random failed, ret is 0x%x\n", ret);
    idx += hash_len;

    /* 3. set data[hash_len + 1, 2hash_len + 1] = lHash */
    ret = memcpy_s(p + idx, pack->em_len - idx, hash_info.lhash_data, hash_len);
    crypto_chk_return(ret != TD_SUCCESS, ret, "memcpy_s failed, ret is 0x%x\n", ret);
    idx += hash_len;

    /* 4. set PS with 0x00 */
    tmp_len = pack->klen - pack->data_len - 2 * hash_len - 2; /* 2 */
    (void)memset_s(p + idx, tmp_len, 0x00, tmp_len);
    idx += tmp_len;

    /* 5. set 0x01 after PS */
    p[idx++] = 0x01;

    /* 6. set M */
    if (pack->data_len != 0) {
        ret = memcpy_s(p + idx, pack->em_len - idx, pack->data, pack->data_len);
        crypto_chk_return(ret != TD_SUCCESS, ret, "memcpy_s failed, ret is 0x%x\n", ret);
    }
    /* 7. MGF: seed -> db */
    tmp_len = pack->klen - hash_len - 1;
    ret = pkcs1_mgf(&hash_info, seed, hash_len, db, tmp_len);
    crypto_chk_return(ret != TD_SUCCESS, ret, "pkcs1_mgf failed, ret is 0x%x\n", ret);

    /* 8. MGF: db -> seed */
    ret = pkcs1_mgf(&hash_info, db, tmp_len, seed, hash_len);
    crypto_chk_return(ret != TD_SUCCESS, ret, "pkcs1_mgf failed, ret is 0x%x\n", ret);

    return ret;
}

static td_s32 pkcs1_oaep_decrypt(const drv_pke_hash_type hash_type, rsa_pkcs1_pack *pack,
    const drv_pke_data *label)
{
    td_s32 ret = TD_FAILURE;
    rsa_pkcs1_hash_info hash_info = {0};
    td_u32 idx = 0;
    td_u32 hash_len = 0;
    td_u32 tmp_len = 0;
    td_u8 *p = TD_NULL;
    td_u8 *db = TD_NULL;
    td_u8 *seed = TD_NULL;

    ret = pkcs1_get_hash(hash_type, label, &hash_info);
    crypto_chk_return(ret != TD_SUCCESS, ret, "pkcs1_get_hash failed, ret is 0x%x\n", ret);

    hash_len = hash_info.hash_len;

    p = pack->em;
    seed = p + 1;
    db = p + hash_len + 1;

    /* 1. check data[0] = 0 */
    if (p[idx++] != 0x00) {
        crypto_log_err("oaep padding check failed!\n");
        return PKE_COMPAT_ERRNO(ERROR_PKE_RSA_CRYPTO_OAEP_CHECK);
    }

    /* 2. MGF: db -> seed */
    tmp_len = pack->klen - hash_len - 1;
    ret = pkcs1_mgf(&hash_info, db, tmp_len, seed, hash_len);
    crypto_chk_return(ret != TD_SUCCESS, ret, "pkcs1_mgf failed, ret is 0x%x\n", ret);

    /* 3. MGF: seed -> db */
    ret = pkcs1_mgf(&hash_info, seed, hash_len, db, tmp_len);
    crypto_chk_return(ret != TD_SUCCESS, ret, "pkcs1_mgf failed, ret is 0x%x\n", ret);

    /* 4. check data[hash + 1, 2hash + 1] */
    idx += hash_len;
    if (memcmp(p + idx, hash_info.lhash_data, hash_len) != 0) {
        crypto_log_err("oaep lhash check failed!\n");
        return PKE_COMPAT_ERRNO(ERROR_PKE_RSA_CRYPTO_OAEP_CHECK);
    }

    /* 5. remove PS */
    idx += hash_len;
    for (; idx < pack->klen - 1; idx++) {
        if (p[idx] != 0x00) {
            break;
        }
    }

    /* 6. check 0x01 */
    if (p[idx++] != 0x01) {
        crypto_log_err("oaep check failed!\n");
        return PKE_COMPAT_ERRNO(ERROR_PKE_RSA_CRYPTO_OAEP_CHECK);
    }

    /* 7. check data length */
    tmp_len = pack->klen - idx;
    if (tmp_len > pack->klen - 2 * hash_len - 2) { /* 2 */
        crypto_log_err("PS error.\n");
        return PKE_COMPAT_ERRNO(ERROR_PKE_RSA_CRYPTO_OAEP_CHECK);
    }

    if (tmp_len > pack->data_len) {
        crypto_log_err("Input buffer too small.\n");
        return PKE_COMPAT_ERRNO(ERROR_PKE_RSA_CRYPTO_OAEP_CHECK);
    }

    if (tmp_len != 0) {
        ret = memcpy_s(pack->data, pack->data_len, p + idx, tmp_len);
        crypto_chk_return(ret != TD_SUCCESS, ret, "memcpy_s failed, ret is 0x%x\n", ret);
    }
    pack->data_len = tmp_len; // record length of plain text for return

    return ret;
}

static td_s32 rsa_sign(
    drv_pke_rsa_scheme scheme,
    drv_pke_hash_type hash_type,
    td_u8 *em,
    td_u32 klen,
    td_u32 em_bit,
    const drv_pke_data *msg)
{
    td_s32 ret = TD_FAILURE;
    rsa_pkcs1_pack sg_pack = {
        .klen = klen,
        .em_bit = em_bit,
        .em = em,
        .em_len = klen,
        .hash = msg->data,
        .hash_len = msg->length,
        .data = TD_NULL,
        .data_len = 0
    };

    switch (scheme) {
#if defined(CONFIG_PKE_RSA_V15_SUPPORT)
        case DRV_PKE_RSA_SCHEME_PKCS1_V15: {
            ret = pkcs1_v15_sign(hash_type, &sg_pack);
            crypto_chk_return(ret != TD_SUCCESS, ret, "pkcs1_v15_sign failed, ret is 0x%x\n", ret);
            break;
        }
#endif
        case DRV_PKE_RSA_SCHEME_PKCS1_V21: {
            ret = pkcs1_pss_sign(hash_type, &sg_pack);
            crypto_chk_return(ret != TD_SUCCESS, ret, "pkcs1_pss_sign failed, ret is 0x%x\n", ret);
            break;
        }
        default:
            crypto_log_err("Invalid rsa sign padding type.\n");
            return PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM);
    }
    return ret;
}

static td_s32 rsa_verify(
    const drv_pke_rsa_scheme scheme,
    const drv_pke_hash_type hash_type,
    td_u8 *em,
    const td_u32 klen,
    const td_u32 em_bit,
    const drv_pke_data *msg)
{
    td_s32 ret = TD_FAILURE;
    rsa_pkcs1_pack vf_pack = {
        .klen = klen,
        .em_bit = em_bit,
        .em = em,
        .em_len = klen,
        .hash = msg->data,
        .hash_len = msg->length,
        .data = TD_NULL,
        .data_len = 0
    };

    switch (scheme) {
#if defined(CONFIG_PKE_RSA_V15_SUPPORT)
        case DRV_PKE_RSA_SCHEME_PKCS1_V15: {
            ret = pkcs1_v15_verify(hash_type, &vf_pack);
            crypto_chk_return(ret != TD_SUCCESS, ret, "pkcs1_v15_verify failed, ret is 0x%x\n", ret);
            break;
        }
#endif
        case DRV_PKE_RSA_SCHEME_PKCS1_V21: {
            ret = pkcs1_pss_verify(hash_type, &vf_pack);
            crypto_chk_return(ret != TD_SUCCESS, ret, "pkcs1_pss_verify failed, ret is 0x%x\n", ret);
            break;
        }
        default:
            crypto_log_err("Invalid rsa verify padding type.\n");
            return PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM);
    }
    return ret;
}

static td_s32 rsa_encrypt(
    const drv_pke_rsa_scheme scheme,
    const drv_pke_hash_type hash_type,
    td_u8 *em,
    const td_u32 klen,
    const drv_pke_data *msg,
    const drv_pke_data *label)
{
    td_s32 ret = TD_FAILURE;
    rsa_pkcs1_pack en_pack = {
        .klen = klen,
        .em_bit = 0,
        .em = em,
        .em_len = klen,
        .hash = TD_NULL,
        .hash_len = 0,
        .data = msg->data,
        .data_len = msg->length
    };

    switch (scheme) {
#if defined(CONFIG_PKE_RSA_V15_SUPPORT)
        case DRV_PKE_RSA_SCHEME_PKCS1_V15: {
            ret = pkcs1_v15_encrypt(&en_pack);
            crypto_chk_return(ret != TD_SUCCESS, ret, "pkcs1_v15_encrypt failed, ret is 0x%x\n", ret);
            break;
        }
#endif
        case DRV_PKE_RSA_SCHEME_PKCS1_V21: {
            ret = pkcs1_oaep_encrypt(hash_type, &en_pack, label);
            crypto_chk_return(ret != TD_SUCCESS, ret, "pkcs1_oaep_encrypt failed, ret is 0x%x\n", ret);
            break;
        }
        default:
            crypto_log_err("Invalid rsa encrypt padding type.\n");
            return PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM);
    }
    return ret;
}

static td_s32 rsa_decrypt(const drv_pke_rsa_scheme scheme, const drv_pke_hash_type hash_type, td_u8 *em,
    const td_u32 klen, const drv_pke_data *label, drv_pke_data *msg)
{
    td_s32 ret = TD_FAILURE;
    rsa_pkcs1_pack de_pack = {
        .klen = klen,
        .em_bit = 0,
        .em = em,
        .em_len = klen,
        .hash = TD_NULL,
        .hash_len = 0,
        .data = msg->data,
        .data_len = msg->length
    };

    switch (scheme) {
#if defined(CONFIG_PKE_RSA_V15_SUPPORT)
        case DRV_PKE_RSA_SCHEME_PKCS1_V15: {
            ret = pkcs1_v15_decrypt(&de_pack);
            crypto_chk_return(ret != TD_SUCCESS, ret, "pkcs1_v15_decrypt failed, ret is 0x%x\n", ret);
            msg->length = de_pack.data_len; // record length of plain text for return
            break;
        }
#endif
        case DRV_PKE_RSA_SCHEME_PKCS1_V21: {
            ret = pkcs1_oaep_decrypt(hash_type, &de_pack, label);
            crypto_chk_return(ret != TD_SUCCESS, ret, "pkcs1_oaep_decrypt failed, ret is 0x%x\n", ret);
            msg->length = de_pack.data_len; // record length of plain text for return
            break;
        }
        default:
            crypto_log_err("Invalid rsa decrypt padding type.\n");
            return PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM);
    }
    return ret;
}

static td_s32 pke_rsa_sign(
    const drv_pke_rsa_priv_key *priv_key,
    drv_pke_rsa_scheme scheme,
    drv_pke_hash_type hash_type,
    const drv_pke_data *input_hash,
    drv_pke_data *sign)
{
    td_s32 ret = TD_FAILURE;
    td_u8 *em;
    td_u32 klen = priv_key->n_len;
    td_u32 em_bit = 0;
    drv_pke_data in = {0};

    em = crypto_malloc(DRV_PKE_LEN_4096);
    if (em == TD_NULL) {
        crypto_log_err("%s:%d Error! Malloc memory failed!\n", __FUNCTION__, __LINE__);
        return PKE_COMPAT_ERRNO(ERROR_MALLOC);
    }
    (void)memset_s(em, DRV_PKE_LEN_4096, 0x00, DRV_PKE_LEN_4096);

    em_bit = rsa_get_bit_num((const td_u8 *)priv_key->n, priv_key->n_len);
    ret = rsa_sign(scheme, hash_type, em, klen, em_bit, input_hash);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, exit__, ret, "rsa_sign failed, ret is 0x%x\n", ret);

    in = (drv_pke_data) {.length = klen, .data = em};
    ret = rsa_exp_mod(priv_key->n, priv_key->d, priv_key->n_len, &in, sign);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, exit__, ret, "rsa_exp_mod failed, ret is 0x%x\n", ret);

    if (memcmp(input_hash->data, sign->data, input_hash->length) == 0) {
        ret = PKE_COMPAT_ERRNO(ERROR_PKE_RSA_SAME_DATA);
    }

exit__:
    if (em != TD_NULL) {
        (void)memset_s(em, DRV_PKE_LEN_4096, 0, DRV_PKE_LEN_4096);
        crypto_free(em);
    }
    return ret;
}

static td_s32 pke_rsa_verify(
    const drv_pke_rsa_pub_key *pub_key,
    drv_pke_rsa_scheme scheme,
    drv_pke_hash_type hash_type,
    const drv_pke_data *input_hash,
    const drv_pke_data *sign)
{
    td_s32 ret = TD_FAILURE;
    td_u8 *em;
    td_u8 *pem;
    td_u32 klen = pub_key->len;
    td_u32 em_bit = 0;
    drv_pke_data out = {0};

    em = crypto_malloc(DRV_PKE_LEN_4096);
    if (em == TD_NULL) {
        crypto_log_err("%s:%d Error! Malloc memory failed!\n", __FUNCTION__, __LINE__);
        return PKE_COMPAT_ERRNO(ERROR_MALLOC);
    }
    (void)memset_s(em, DRV_PKE_LEN_4096, 0x00, DRV_PKE_LEN_4096);
    pem = em;

    out = (drv_pke_data) {.length = klen, .data = em};
    ret = rsa_exp_mod(pub_key->n, pub_key->e, pub_key->len, sign, &out);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, exit__, ret, "rsa_exp_mod failed, ret is 0x%x\n", ret);

    if (memcmp(sign->data, pem, pub_key->len) == 0) {
        ret = PKE_COMPAT_ERRNO(ERROR_PKE_RSA_SAME_DATA);
        goto exit__;
    }

    em_bit = rsa_get_bit_num((const td_u8 *)pub_key->n, pub_key->len);
    ret = rsa_verify(scheme, hash_type, em, klen, em_bit, input_hash);
    crypto_chk_goto_with_ret(ret != TD_SUCCESS, exit__, ret, "rsa_verify failed, ret is 0x%x\n", ret);

exit__:
    if (em != TD_NULL) {
        (void)memset_s(em, DRV_PKE_LEN_4096, 0, DRV_PKE_LEN_4096);
        crypto_free(em);
    }
    return ret;
}

static td_s32 pke_rsa_encrypt(
    drv_pke_rsa_scheme scheme,
    drv_pke_hash_type hash_type,
    const drv_pke_rsa_pub_key *pub_key,
    const drv_pke_data *input,
    const drv_pke_data *label,
    const drv_pke_data *output)
{
    td_s32 ret = TD_FAILURE;
    td_u8 em[DRV_PKE_LEN_4096];
    td_u32 klen = pub_key->len;
    td_u8 *n = pub_key->n;
    td_u8 *key = pub_key->e;
    drv_pke_data data_in = {0};

    (void)memset_s(em, sizeof(em), 0x00, sizeof(em));
    ret = rsa_encrypt(scheme, hash_type, em, klen, input, label);
    crypto_chk_return(ret != TD_SUCCESS, ret, "rsa_encrypt failed, ret is 0x%x\n", ret);
    data_in = (drv_pke_data) {.length = klen, .data = em};
    ret = rsa_exp_mod(n, key, klen, &data_in, output);
    crypto_chk_return(ret != TD_SUCCESS, ret, "rsa_exp_mod failed, ret is 0x%x\n", ret);
    if (memcmp(input->data, output->data, input->length) == 0) {
        return PKE_COMPAT_ERRNO(ERROR_PKE_RSA_SAME_DATA);
    }

    return ret;
}

static td_s32 pke_rsa_decrypt(
    drv_pke_rsa_scheme scheme,
    drv_pke_hash_type hash_type,
    const drv_pke_rsa_priv_key *priv_key,
    const drv_pke_data *input,
    const drv_pke_data *label,
    drv_pke_data *output)
{
    td_s32 ret = TD_FAILURE;
    td_u8 em[DRV_PKE_LEN_4096];
    td_u32 klen = priv_key->n_len;
    td_u8 *n = priv_key->n;
    td_u8 *key = priv_key->d;
    drv_pke_data data_out = {0};

    (void)memset_s(em, sizeof(em), 0x00, sizeof(em));

    data_out = (drv_pke_data) {.length = klen, .data = em};
    ret = rsa_exp_mod(n, key, klen, input, &data_out);
    crypto_chk_return(ret != TD_SUCCESS, ret, "rsa_exp_mod failed, ret is 0x%x\n", ret);
    ret = rsa_decrypt(scheme, hash_type, em, klen, label, output);
    crypto_chk_return(ret != TD_SUCCESS, ret, "rsa_decrypt failed, ret is 0x%x\n", ret);
    if (memcmp(input->data, output->data, output->length) == 0) {
        return PKE_COMPAT_ERRNO(ERROR_PKE_RSA_SAME_DATA);
    }
    return ret;
}

// expose layer
td_s32 drv_cipher_pke_rsa_sign(
    const drv_pke_rsa_priv_key *priv_key,
    const drv_pke_rsa_scheme scheme,
    const drv_pke_hash_type hash_type,
    const drv_pke_data *input_hash,
    drv_pke_data *sign)
{
    td_s32 ret = TD_FAILURE;
    td_u32 klen = 0;
    td_u32 hash_len = 0;
    /* check ptr. */
    pke_null_ptr_chk(priv_key);
    pke_null_ptr_chk(priv_key->n);
    pke_null_ptr_chk(priv_key->d);
    pke_null_ptr_chk(input_hash);
    pke_null_ptr_chk(input_hash->data);
    pke_null_ptr_chk(sign);
    pke_null_ptr_chk(sign->data);

    /* check enum. */
    crypto_chk_return(scheme >= DRV_PKE_RSA_SCHEME_MAX, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "scheme is Invalid\n");
    crypto_chk_return(hash_type < DRV_PKE_HASH_TYPE_SHA256 || hash_type > DRV_PKE_HASH_TYPE_SHA512,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "hash_type is Invalid\n");

    /* check length. */
    klen = priv_key->n_len;
    hash_len = drv_get_hash_len(hash_type);
    crypto_chk_return(input_hash->length != hash_len, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "input_hash->length is Invalid\n");

    crypto_chk_return(klen != DRV_PKE_LEN_2048 && klen != DRV_PKE_LEN_3072 && klen != DRV_PKE_LEN_4096,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "n_len is Invalid\n");
    if ((priv_key->d_len != klen)) {
        ret = PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM);
        crypto_log_err("d_len is Invalid!\n");
        return ret;
    }
    crypto_chk_return(sign->length < klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "sign_len is Invalid\n");
    crypto_chk_return(crypto_rsa_support(klen, scheme) == TD_FALSE, PKE_COMPAT_ERRNO(ERROR_UNSUPPORT),
        "alg is unsupport\n");
    ret = drv_cipher_pke_lock_secure();
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_lock_secure failed, ret is 0x%x\n", ret);
    ret = pke_rsa_sign(priv_key, scheme, hash_type, input_hash, sign);
    if (ret == TD_SUCCESS) {
        sign->length = klen;
    }
    (void)drv_cipher_pke_unlock_secure();
    return ret;
}

td_s32 drv_cipher_pke_rsa_verify(
    const drv_pke_rsa_pub_key *pub_key,
    const drv_pke_rsa_scheme scheme,
    const drv_pke_hash_type hash_type,
    const drv_pke_data *input_hash,
    const drv_pke_data *sign)
{
    td_s32 ret = TD_FAILURE;
    td_u32 klen = 0;
    td_u32 hash_len;
    /* check ptr. */
    pke_null_ptr_chk(pub_key);
    pke_null_ptr_chk(pub_key->n);
    pke_null_ptr_chk(pub_key->e);
    pke_null_ptr_chk(input_hash);
    pke_null_ptr_chk(input_hash->data);
    pke_null_ptr_chk(sign);
    pke_null_ptr_chk(sign->data);

    /* check enum. */
    crypto_chk_return(scheme >= DRV_PKE_RSA_SCHEME_MAX, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "scheme is Invalid\n");
    crypto_chk_return(hash_type < DRV_PKE_HASH_TYPE_SHA1 || hash_type > DRV_PKE_HASH_TYPE_SHA512,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "hash_type is Invalid\n");

    /* check length. */
    hash_len = drv_get_hash_len(hash_type);
    crypto_chk_return(input_hash->length != hash_len, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "input_hash->length is Invalid\n");

    klen = pub_key->len;
    if ((klen != DRV_PKE_LEN_2048 && klen != DRV_PKE_LEN_3072 && klen != DRV_PKE_LEN_4096)) {
        ret = PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM);
        crypto_log_err("k_len is Invalid!\n");
        return ret;
    }
    crypto_chk_return(sign->length < klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "sign_len is Invalid\n");
    crypto_chk_return(crypto_rsa_support(klen, scheme) == TD_FALSE, PKE_COMPAT_ERRNO(ERROR_UNSUPPORT),
        "alg is unsupport\n");
    ret = drv_cipher_pke_lock_secure();
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_lock_secure failed, ret is 0x%x\n", ret);
    ret = pke_rsa_verify(pub_key, scheme, hash_type, input_hash, sign);
    (void)drv_cipher_pke_unlock_secure();
    return ret;
}

td_s32 drv_cipher_pke_rsa_public_encrypt(
    const drv_pke_rsa_scheme scheme,
    const drv_pke_hash_type hash_type,
    const drv_pke_rsa_pub_key *pub_key,
    const drv_pke_data *input,
    const drv_pke_data *label,
    drv_pke_data *output)
{
    td_s32 ret = TD_FAILURE;
    td_u32 klen = 0;
    td_u32 hash_len = 0;
    td_u32 input_len = 0;
    /* check ptr. */
    pke_null_ptr_chk(pub_key);
    pke_null_ptr_chk(pub_key->n);
    pke_null_ptr_chk(pub_key->e);
    pke_null_ptr_chk(input);
    pke_null_ptr_chk(input->data);
    pke_null_ptr_chk(output);
    pke_null_ptr_chk(output->data);
    if (label != TD_NULL) {
        pke_null_ptr_chk(label->data);
    }

    /* check enum. */
    crypto_chk_return(scheme >= DRV_PKE_RSA_SCHEME_MAX, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "scheme is Invalid\n");
    crypto_chk_return(hash_type < DRV_PKE_HASH_TYPE_SHA256 || hash_type > DRV_PKE_HASH_TYPE_SHA512,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "hash_type is Invalid\n");

    /* check length. */
    klen = pub_key->len;
    input_len = input->length;
    hash_len = drv_get_hash_len(hash_type);
    crypto_chk_return(hash_len == 0, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "hash_type is Invalid\n");

    crypto_chk_return(klen != DRV_PKE_LEN_2048 && klen != DRV_PKE_LEN_3072 && klen != DRV_PKE_LEN_4096,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "n_len is Invalid\n");
    /*
     * For V15, the plain_text's max length is klen - 11.
     * For OAEP, the plain_text's max length is klen - 2 * hash_len - 2.
     */
    if (scheme == DRV_PKE_RSA_SCHEME_PKCS1_V15) {
        crypto_chk_return(input_len > klen - RSA_PADLEN_11, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
            "input_len is too long for V15\n");
    } else {
        crypto_chk_return(input_len > klen - 2 * hash_len - 2,  // 2: refer to comment.
            PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "input_len is too long for V21\n");
    }
    crypto_chk_return(input_len == 0, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "input_len is zero\n");
    crypto_chk_return(output->length < klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "output_len is Invalid\n");
    crypto_chk_return(crypto_rsa_support(klen, scheme) == TD_FALSE, PKE_COMPAT_ERRNO(ERROR_UNSUPPORT),
        "alg is unsupport\n");
    ret = drv_cipher_pke_lock_secure();
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_lock_secure failed, ret is 0x%x\n", ret);
    ret = pke_rsa_encrypt(scheme, hash_type, pub_key, input, label, output);
    if (ret == TD_SUCCESS) {
        output->length = klen;
    }
    (void)drv_cipher_pke_unlock_secure();
    return ret;
}

td_s32 drv_cipher_pke_rsa_private_decrypt(
    const drv_pke_rsa_scheme scheme,
    const drv_pke_hash_type hash_type,
    const drv_pke_rsa_priv_key *priv_key,
    const drv_pke_data *input,
    const drv_pke_data *label,
    drv_pke_data *output)
{
    td_s32 ret = TD_FAILURE;

    td_u32 klen = 0;
    td_u32 hash_len = 0;
    td_u32 output_len = 0;
    /* check ptr. */
    pke_null_ptr_chk(priv_key);
    pke_null_ptr_chk(priv_key->n);
    pke_null_ptr_chk(priv_key->d);
    pke_null_ptr_chk(input);
    pke_null_ptr_chk(input->data);
    pke_null_ptr_chk(output);
    pke_null_ptr_chk(output->data);
    if (label != TD_NULL) {
        pke_null_ptr_chk(label->data);
    }

    /* check enum. */
    crypto_chk_return(scheme >= DRV_PKE_RSA_SCHEME_MAX, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "scheme is Invalid\n");
    crypto_chk_return(hash_type < DRV_PKE_HASH_TYPE_SHA256 || hash_type > DRV_PKE_HASH_TYPE_SHA512,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "hash_type is Invalid\n");

    /* check length. */
    klen = priv_key->n_len;
    output_len = output->length;
    hash_len = drv_get_hash_len(hash_type);
    crypto_chk_return(hash_len == 0, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "hash_type is Invalid\n");

    crypto_chk_return(klen != DRV_PKE_LEN_2048 && klen != DRV_PKE_LEN_3072 && klen != DRV_PKE_LEN_4096,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "n_len is Invalid\n");
    crypto_chk_return(priv_key->d_len != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "d_len is Invalid\n");
    crypto_chk_return(input->length != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "input_len is Invalid\n");

    /*
     * For V15, the plain_text's max length is klen - 11.
     * For OAEP, the plain_text's max length is klen - 2 * hash_len - 2.
     */
    if (scheme == DRV_PKE_RSA_SCHEME_PKCS1_V15) {
        crypto_chk_return(output_len < klen - RSA_PADLEN_11, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
            "output_len is not enough for V15\n");
    } else {
        crypto_chk_return(output_len < klen - 2 * hash_len - 2,  // 2: refer to comment.
            PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "input_len is not enough for V21\n");
    }
    crypto_chk_return(crypto_rsa_support(klen, scheme) == TD_FALSE, PKE_COMPAT_ERRNO(ERROR_UNSUPPORT),
        "alg is unsupport\n");
    ret = drv_cipher_pke_lock_secure();
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_lock_secure failed, ret is 0x%x\n", ret);
    ret = pke_rsa_decrypt(scheme, hash_type, priv_key, input, label, output);
    (void)drv_cipher_pke_unlock_secure();
    return ret;
}