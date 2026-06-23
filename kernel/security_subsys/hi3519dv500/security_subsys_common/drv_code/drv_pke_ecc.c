/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "hal_pke.h"
#include "drv_pke.h"
#include "crypto_hash_struct.h"
#include "crypto_ecc_curve.h"
#include "securec.h"
#include "drv_hash.h"
#include "drv_trng.h"
#include "crypto_drv_common.h"
#include "drv_inner.h"

#define PKE_COMPAT_ERRNO(err_code)      DISPATCH_COMPAT_ERRNO(ERROR_MODULE_PKE, err_code)

#define pke_null_ptr_chk(ptr)   \
    crypto_chk_return((ptr) == TD_NULL, PKE_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), #ptr" is NULL\n")

#define ED25519_MAX_HASH_ELEMENTS                    3
#define SM2_CRYPTO_ADD_LENGTH_IN_BYTE                97 // 97 = SM2_LEN + SM2_LEN + SM3_LEN + 1
#define ECC_CHECK_DOT_BUF_CNT                        11
#define SM2_SIGN_BUF_CNT                             19
#define SM2_VERIFY_BUF_CNT                           10
#define ECDSA_SIGN_BUF_CNT                           15
#define ECDSA_VERIFY_BUF_CNT                         19
#define GENERATE_X_BUF_CNT                           13
#define POINT_DECOMPRESS_BUF_CNT                     13
#define EDDSA_SIGN_BUF_CNT                           14
#define EDDSA_VERIFY_BUF_CNT                         12
#define RANDNUM_TRY_TIMES                            20

typedef enum {
    // need buffer
    CHECKDOT_MONTGOMERY_P = 0,
    CHECKDOT_MX,
    CHECKDOT_MX2,
    CHECKDOT_MX3,
    CHECKDOT_MA,
    CHECKDOT_MAX,
    CHECKDOT_MB,
    CHECKDOT_MY,
    CHECKDOT_MY2,
    CHECKDOT_MX3_MAX,
    CHECKDOT_MX3_MAX_MB,
    // no need buffer
    CHECKDOT_ECC_P,
    CHECKDOT_ECC_A,
    CHECKDOT_ECC_B,
    CHECKDOT_X,
    CHECKDOT_Y,
    PKE_CHECK_DOT_ON_CURVE_VAR_MAX
} pke_check_dot_on_curve_var;

typedef enum {
    // need buffer
    SM2SIGN_E = 0,
    SM2SIGN_MONTGOMERY_N,
    SM2SIGN_K,
    SM2SIGN_R_X,
    SM2SIGN_R_Y,
    SM2SIGN_RK,
    SM2SIGN_MY,
    SM2SIGN_MR,
    SM2SIGN_MS,
    SM2SIGN_MK,
    SM2SIGN_MRK,
    SM2SIGN_MRKY,
    SM2SIGN_TA,
    SM2SIGN_MTA,
    SM2SIGN_MTY,
    SM2SIGN_MTZ,
    SM2SIGN_MINV,
    SM2SIGN_ZERO,
    SM2SIGN_ONE,
    // no need buffer
    SM2SIGN_SM2_N,
    SM2SIGN_SIG_R,
    SM2SIGN_SIG_S,
    PKE_SM2_SIGN_VAR_MAX
} pke_sm2_sign_var;

typedef enum {
    // need buffer
    SM2VERIFY_T = 0,
    SM2VERIFY_E,
    SM2VERIFY_V,
    SM2VERIFY_S_GX,
    SM2VERIFY_S_GY,
    SM2VERIFY_T_PAX,
    SM2VERIFY_T_PAY,
    SM2VERIFY_RX,
    SM2VERIFY_RY,
    SM2VERIFY_ZERO,
    // no need buffer
    SM2VERIFY_SM2_N,
    SM2VERIFY_SIG_R,
    SM2VERIFY_SIG_S,
    PKE_SM2_VERIFY_VAR_MAX
} pke_sm2_verify_var;

typedef enum {
    // need buffer
    ECCSIGN_K = 0,
    ECCSIGN_E,
    ECCSIGN_RX,
    ECCSIGN_RY,
    ECCSIGN_MONTGOMERY_N,
    ECCSIGN_ME,
    ECCSIGN_MR,
    ECCSIGN_MS,
    ECCSIGN_MD,
    ECCSIGN_MRD,
    ECCSIGN_MK,
    ECCSIGN_MKNI,
    ECCSIGN_Y,
    ECCSIGN_ZERO,
    ECCSIGN_ONE,
    // no need buffer
    ECCSIGN_ECC_N,
    ECCSIGN_SIG_R,
    ECCSIGN_SIG_S,
    ECCSIGN_HASH,
    PKE_ECDSA_SIGN_VAR_MAX
} pke_ecdsa_sign_var;

typedef enum {
    // need buffer
    ECCVERIFY_E = 0,
    ECCVERIFY_V,
    ECCVERIFY_U1,
    ECCVERIFY_U2,
    ECCVERIFY_ME,
    ECCVERIFY_MS,
    ECCVERIFY_MR,
    ECCVERIFY_MSNI,
    ECCVERIFY_MU1,
    ECCVERIFY_MU2,
    ECCVERIFY_RX,
    ECCVERIFY_RY,
    ECCVERIFY_U1X,
    ECCVERIFY_U1Y,
    ECCVERIFY_U2X,
    ECCVERIFY_U2Y,
    ECCVERIFY_MONTGOMERY_N,
    ECCVERIFY_ZERO,
    ECCVERIFY_ONE,
    // no need buffer
    ECCVERIFY_ECC_N,
    ECCVERIFY_SIG_R,
    ECCVERIFY_SIG_S,
    ECCVERIFY_HASH,
    PKE_ECDSA_VERIFY_VAR_MAX
} pke_ecdsa_verify_var;

typedef enum {
    // need buffer
    GENERATEX_MAX = 0,
    GENERATEX_P38,
    GENERATEX_XX,
    GENERATEX_C,
    GENERATEX_P14,
    GENERATEX_S,
    GENERATEX_MONTGOMERY_P,
    GENERATEX_MX,
    GENERATEX_MS,
    GENERATEX_MXS,
    GENERATEX_ONE,
    GENERATEX_TWO,
    GENERATEX_THREE,
    // no need buffer
    GENERATEX_ECC_P,
    GENERATEX_X,
    GENERATEX_X2,
    GENERATE_X_VAR_MAX
} generate_x_var;

typedef enum {
    // need buffer
    POINTDECOMPRESS_MONTGOMERY_P = 0,
    POINTDECOMPRESS_MY,
    POINTDECOMPRESS_MY2,
    POINTDECOMPRESS_MY21,
    POINTDECOMPRESS_MD,
    POINTDECOMPRESS_MDY2,
    POINTDECOMPRESS_MDY21,
    POINTDECOMPRESS_MINV,
    POINTDECOMPRESS_X,
    POINTDECOMPRESS_X2,
    POINTDECOMPRESS_MX2,
    POINTDECOMPRESS_ONE,
    POINTDECOMPRESS_MONE,
    // no need buffer
    POINTDECOMPRESS_ECC_P,
    POINTDECOMPRESS_ECC_A,
    POINTDECOMPRESS_OUTX,
    POINTDECOMPRESS_OUTY,
    POINT_DECOMPRESS_VAR_MAX
} point_decompress_var;

typedef enum {
    // need buffer
    EDDSIGN_A = 0,
    EDDSIGN_PREFIX,
    EDDSIGN_PUBKEY,
    EDDSIGN_R,
    EDDSIGN_S,
    EDDSIGN_RS,
    EDDSIGN_H,
    EDDSIGN_MONTGOMERY_N,
    EDDSIGN_MH,
    EDDSIGN_MA,
    EDDSIGN_MHA,
    EDDSIGN_MR,
    EDDSIGN_MS,
    EDDSIGN_ONE,
    // no need buffer
    EDDSIGN_ECC_N,
    EDDSIGN_MSG,
    PKE_EDDSA_SIGN_VAR_MAX
} pke_eddsa_sign_var;

/*
 * generate randnum. if max != TD_NULL. then randnum will < max
 */
static td_s32 get_randnum(td_u8 *randnum, td_u8 *max, td_u32 klen)
{
    td_s32 ret = TD_FAILURE;
    td_u32 i;
    td_u32 index;

    if (max == NULL) {
        return drv_cipher_trng_get_multi_random(klen, randnum);
    }

    for (index = 0; index < klen && max[index] == 0; ++index) {}

    if (index == klen) {
        return TD_FAILURE;
    }

    (void)memset_s(randnum, klen, 0, klen);

    randnum[index] = max[index];
    
    for (i = 0; i < RANDNUM_TRY_TIMES; i++) {
        ret = drv_cipher_trng_get_multi_random(klen - index - 1, randnum + index + 1);
        if (ret != TD_SUCCESS) {
            crypto_log_err("drv_cipher_trng_get_multi_random failed, ret is 0x%x\n", ret);
            return ret;
        }
        
        if (memcmp(randnum, max, klen) < 0) {
            return TD_SUCCESS;
        }
    }

    crypto_log_err("%s:%d Error! Can't gen randnum < max in %d times!\n", __FUNCTION__, __LINE__, RANDNUM_TRY_TIMES);
    return TD_FAILURE;
}

/* check val =? zero */
static td_bool drv_ecc_is_zero(const td_u8 *val, td_u32 klen)
{
    td_u32 i = 0;
    for (i = 0; i < klen; i++) {
        if (val[i] != 0x00) {
            return TD_FALSE;
        }
    }
    return TD_TRUE;
}

/* check 0 < val < max */
static td_s32 drv_range_check(const td_u8 *val, const td_u8 *max, td_u32 klen)
{
    if (drv_ecc_is_zero(val, klen)) {
        return PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM);
    }

    if (memcmp(val, max, klen) < 0) {
        return TD_SUCCESS;
    }
    return TD_FAILURE;
}

/*
 * montgomery_n = (r ^ 2 mod mod_n) = (2 ^ (2 * klen) mod mod_n)
 */
static td_s32 cal_montgomery_n(const drv_pke_data *mod_n, const drv_pke_data *montgomery_n)
{
    td_s32 ret = TD_FAILURE;
    td_u32 klen = 0;
    td_u32 max_len = 0;
    td_u32 r2_len;
    td_u8 *r2;
    drv_pke_data r2_data;

    klen = mod_n->length;
    // for some chips, the 512 length needs to be aligned into 521 length
    ret = hal_pke_align_len(klen, &max_len, HAL_ALIGN_LEN);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_pke_align_len failed, ret is 0x%x\n", ret);
    // r_len = 2k_len + 1
    r2_len = 2 * max_len + 1; // 2 : see comment above
    r2 = crypto_malloc(r2_len);
    if (r2 == TD_NULL) {
        crypto_log_err("%s:%d Error! Malloc memory failed!\n", __FUNCTION__, __LINE__);
        return PKE_COMPAT_ERRNO(ERROR_MALLOC);
    }
    (void)memset_s(r2, r2_len, 0, r2_len);
    r2[0] = 1;
    r2_data = (drv_pke_data){r2_len, r2};
    ret = drv_cipher_pke_mod(&r2_data, mod_n, montgomery_n);
    (void)memset_s(r2, r2_len, 0, r2_len);
    crypto_free(r2);
    return ret;
}

/*
 * check point->y ^ 2 ?= point->x ^ 3 + ecc->a * point->x  + ecc->b
 */
static td_s32 pke_check_dot_on_curve(const drv_pke_ecc_curve *ecc, const drv_pke_ecc_point *point, td_bool *is_on_curve)
{
    td_s32 ret = TD_FAILURE;
    td_u32 i;
    drv_pke_data var_arr[PKE_CHECK_DOT_ON_CURVE_VAR_MAX];
    td_u8 *buffer;

    buffer = crypto_malloc(ecc->ksize * ECC_CHECK_DOT_BUF_CNT);
    crypto_chk_return(buffer == TD_NULL, PKE_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");
    (void)memset_s(buffer, ecc->ksize * ECC_CHECK_DOT_BUF_CNT, 0, ecc->ksize * ECC_CHECK_DOT_BUF_CNT);

    for (i = 0; i < PKE_CHECK_DOT_ON_CURVE_VAR_MAX; i++) {
        if (i <= CHECKDOT_MX3_MAX_MB) {
            var_arr[i].data = buffer + ecc->ksize * i;
        }
        var_arr[i].length = ecc->ksize;
    }
    var_arr[CHECKDOT_ECC_P].data = (td_u8 *)ecc->p;
    var_arr[CHECKDOT_ECC_A].data = (td_u8 *)ecc->a;
    var_arr[CHECKDOT_ECC_B].data = (td_u8 *)ecc->b;
    var_arr[CHECKDOT_X].data = point->x;
    var_arr[CHECKDOT_Y].data = point->y;

    // Step 1: N = r^2 mod p where N is montgomery multiplier
    ret = cal_montgomery_n(&var_arr[CHECKDOT_ECC_P], &var_arr[CHECKDOT_MONTGOMERY_P]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "cal_montgomery_n failed, ret is 0x%x\n", ret);

    /* Step 1: mx = point->x * N mod p */
    ret = drv_cipher_pke_mul_mod(&var_arr[CHECKDOT_X], &var_arr[CHECKDOT_MONTGOMERY_P], &var_arr[CHECKDOT_ECC_P],
        &var_arr[CHECKDOT_MX]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

    /* Step 2: my = point->y * N mod p */
    ret = drv_cipher_pke_mul_mod(&var_arr[CHECKDOT_Y], &var_arr[CHECKDOT_MONTGOMERY_P], &var_arr[CHECKDOT_ECC_P],
        &var_arr[CHECKDOT_MY]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

    /* Step 3: mx2 = mx * mx mod p */
    ret = drv_cipher_pke_mul_mod(&var_arr[CHECKDOT_MX], &var_arr[CHECKDOT_MX], &var_arr[CHECKDOT_ECC_P],
        &var_arr[CHECKDOT_MX2]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

    /* Step 4: mx3 = mx2 * mx mod p */
    ret = drv_cipher_pke_mul_mod(&var_arr[CHECKDOT_MX2], &var_arr[CHECKDOT_MX], &var_arr[CHECKDOT_ECC_P],
        &var_arr[CHECKDOT_MX3]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

    /* Step 5: ma = a * N mod p */
    ret = drv_cipher_pke_mul_mod(&var_arr[CHECKDOT_ECC_A], &var_arr[CHECKDOT_MONTGOMERY_P], &var_arr[CHECKDOT_ECC_P],
        &var_arr[CHECKDOT_MA]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

    /* Step 6: max = ma * mx mod p */
    ret = drv_cipher_pke_mul_mod(&var_arr[CHECKDOT_MA], &var_arr[CHECKDOT_MX], &var_arr[CHECKDOT_ECC_P],
        &var_arr[CHECKDOT_MAX]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

    /* Step 7: mx3_max = mx3 + max mod p */
    ret = drv_cipher_pke_add_mod(&var_arr[CHECKDOT_MX3], &var_arr[CHECKDOT_MAX], &var_arr[CHECKDOT_ECC_P],
        &var_arr[CHECKDOT_MX3_MAX]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_add_mod failed, ret is 0x%x\n", ret);

    /* Step 8: mb = b * N mod p */
    ret = drv_cipher_pke_mul_mod(&var_arr[CHECKDOT_ECC_B], &var_arr[CHECKDOT_MONTGOMERY_P], &var_arr[CHECKDOT_ECC_P],
        &var_arr[CHECKDOT_MB]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

    /* Step 9: mx3_max_mb = mx3_max + mb mod p */
    ret = drv_cipher_pke_add_mod(&var_arr[CHECKDOT_MX3_MAX], &var_arr[CHECKDOT_MB], &var_arr[CHECKDOT_ECC_P],
        &var_arr[CHECKDOT_MX3_MAX_MB]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_add_mod failed, ret is 0x%x\n", ret);

    /* Step 10: my2 = my * my mod p */
    ret = drv_cipher_pke_mul_mod(&var_arr[CHECKDOT_MY], &var_arr[CHECKDOT_MY], &var_arr[CHECKDOT_ECC_P],
        &var_arr[CHECKDOT_MY2]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

    /* Step 11: my2 ?= mx3_max_mb */
    *is_on_curve = memcmp(var_arr[CHECKDOT_MY2].data, var_arr[CHECKDOT_MX3_MAX_MB].data, ecc->ksize) == 0;

exit__:
    (void)drv_cipher_pke_clean_ram();
    if (buffer != TD_NULL) {
        (void)memset_s(buffer, ecc->ksize * ECC_CHECK_DOT_BUF_CNT, 0, ecc->ksize * ECC_CHECK_DOT_BUF_CNT);
        crypto_free(buffer);
    }
    return ret;
}

static td_s32 sm2_encrypt_decrypt_hash(const drv_pke_ecc_point *param, const drv_pke_data *msg, drv_pke_data *hash)
{
    td_s32 ret = TD_FAILURE;
    drv_pke_data arr[3]; // The capacity of the array is 3.

    arr[0].data = param->x;
    arr[0].length = DRV_PKE_SM2_LEN_IN_BYTES;
    arr[1].data = msg->data;
    arr[1].length = msg->length;
    arr[2].data = param->y; // 2 is data of the 3th element
    arr[2].length = DRV_PKE_SM2_LEN_IN_BYTES; // 2 is data of the 3th element

    /* C3 = SM3( x2 || M || y2 ) */
    ret = drv_cipher_pke_calc_hash(arr, sizeof(arr) / sizeof(arr[0]), DRV_PKE_HASH_TYPE_SM3, hash);
    return ret;
}

static td_s32 sm2_kdf(const drv_pke_ecc_point *param, td_u8 *out, const td_u32 klen)
{
    td_s32 ret = TD_FAILURE;
    td_u32 block = 0;
    td_u32 i = 0;
    td_u32 ct = 0;
    drv_pke_data arr[3]; // The capacity of the array is 3.
    td_u8 h[DRV_PKE_SM2_LEN_IN_BYTES] = {0};
    drv_pke_data hash = {DRV_PKE_SM2_LEN_IN_BYTES, h};

    arr[0].data = param->x;
    arr[0].length = DRV_PKE_SM2_LEN_IN_BYTES;
    arr[1].data = param->y;
    arr[1].length = DRV_PKE_SM2_LEN_IN_BYTES;
    arr[2].data = (td_u8 *)&ct; // 2 is data of the 3th element
    arr[2].length = sizeof(ct); // 2 is data of the 3th element
    if (klen == 0) {
        return TD_SUCCESS;
    }

    block = (klen + DRV_PKE_SM2_LEN_IN_BYTES - 1) / DRV_PKE_SM2_LEN_IN_BYTES;
    for (i = 0; i < block; i++) {
        ct = crypto_cpu_to_be32(i + 1);
        /* H = SM3( X || Y || CT ) */
        ret = drv_cipher_pke_calc_hash(arr, sizeof(arr) / sizeof(arr[0]), DRV_PKE_HASH_TYPE_SM3, &hash);
        crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_calc_hash failed, ret is 0x%x\n", ret);

        if (i == (block - 1)) {
            ret = memcpy_s(out + i * DRV_PKE_SM2_LEN_IN_BYTES, klen - i * DRV_PKE_SM2_LEN_IN_BYTES, h,
                klen - i * DRV_PKE_SM2_LEN_IN_BYTES);
            crypto_chk_return(ret != TD_SUCCESS, ret, "memcpy_s failed, ret is 0x%x\n", ret);
        } else {
            ret = memcpy_s(out + i * DRV_PKE_SM2_LEN_IN_BYTES, klen - i * DRV_PKE_SM2_LEN_IN_BYTES, h,
                DRV_PKE_SM2_LEN_IN_BYTES);
            crypto_chk_return(ret != TD_SUCCESS, ret, "memcpy_s failed, ret is 0x%x\n", ret);
        }
    }
    if (drv_ecc_is_zero(out, klen)) {
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 pke_sm2_public_encrypt(const drv_pke_ecc_point *pub_key, const drv_pke_data *plain_text,
    drv_pke_data *cipher_text)
{
    // declaration and assignment of variables and parameter verification
    td_s32 ret = TD_FAILURE;
    td_u32 klen = DRV_PKE_SM2_LEN_IN_BYTES;
    td_u32 c1_index = 1;
    td_u32 c3_index = c1_index + klen * 2; // 2 : see the start position of algorithm C3.
    td_u32 c2_index = c1_index + klen * 3; // 3 : see the start position of algorithm C2.
    td_u32 i = 0;
    td_u8 k[DRV_PKE_SM2_LEN_IN_BYTES] = {0};
    td_u8 C1x[DRV_PKE_SM2_LEN_IN_BYTES] = {0};
    td_u8 C1y[DRV_PKE_SM2_LEN_IN_BYTES] = {0};
    td_u8 kPBx[DRV_PKE_SM2_LEN_IN_BYTES] = {0};
    td_u8 kPBy[DRV_PKE_SM2_LEN_IN_BYTES] = {0};
    td_u8 *t = cipher_text->data + c2_index;
    drv_pke_data k_data = {klen, k};
    drv_pke_data C3_data = {klen, cipher_text->data + c3_index};
    drv_pke_ecc_point C1_point = {C1x, C1y, klen};
    drv_pke_ecc_point kPB_point = {kPBx, kPBy, klen};
    drv_pke_ecc_point sm2_gpoint;
    const drv_pke_ecc_curve *sm2 = get_ecc_curve(DRV_PKE_ECC_TYPE_SM2);
    crypto_chk_return(sm2 == TD_NULL, PKE_COMPAT_ERRNO(ERROR_UNSUPPORT), "unsupport alg\n");

    sm2_gpoint = (drv_pke_ecc_point){(td_u8 *)sm2->gx, (td_u8 *)sm2->gy, klen};
    (void)memset_s(cipher_text->data, cipher_text->length, 0, cipher_text->length);
    // the beginning of the algorithm
    cipher_text->data[0] = SM2_PC_UNCOMPRESS;

    for (i = 0; i < ECC_TRY_CNT; i++) {
        /* Step 1: random k */
        ret = get_randnum(k_data.data, (td_u8 *)sm2->n, sm2->ksize);
        crypto_chk_return(ret != TD_SUCCESS, ret, "get_randnum failed, ret is 0x%x\n", ret);

        /* Step 2: C1 = k * G */
        ret = drv_cipher_pke_mul_dot(sm2, &k_data, &sm2_gpoint, &C1_point);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_dot failed, ret is 0x%x\n", ret);

        /* Step 3: kPB = k * PB */
        ret = drv_cipher_pke_mul_dot(sm2, &k_data, pub_key, &kPB_point);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_dot failed, ret is 0x%x\n", ret);

        /* Step 4: t = kdf( kPBx || kPBy, plain_text->length ) */
        ret = sm2_kdf(&kPB_point, t, plain_text->length);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "sm2_kdf failed, ret is 0x%x\n", ret);

        /* Step 5: t ?= 0 */
        if (drv_ecc_is_zero(t, plain_text->length) != TD_TRUE) {
            break;
        }
    }
    if (i == ECC_TRY_CNT) {
        crypto_log_err("%s:%d Error! Can't gen t in %d times!\n", __FUNCTION__, __LINE__, ECC_TRY_CNT);
        ret = PKE_COMPAT_ERRNO(ERROR_TRY_TIMES);
        goto exit__;
    }
    ret = memcpy_s(cipher_text->data + c1_index, cipher_text->length - c1_index, C1_point.x, klen);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "memcpy_s failed, ret is 0x%x\n", ret);

    ret = memcpy_s(cipher_text->data + c1_index + klen, cipher_text->length - c1_index - klen, C1_point.y, klen);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "memcpy_s failed, ret is 0x%x\n", ret);

    for (i = 0; i < plain_text->length; i++) {
        *(t + i) ^= *(plain_text->data + i);
    }
    ret = sm2_encrypt_decrypt_hash(&kPB_point, plain_text, &C3_data);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "sm2_encrypt_decrypt_hash failed, ret is 0x%x\n", ret);

exit__:
    (void)drv_cipher_pke_clean_ram();
    return ret;
}

static td_s32 pke_sm2_private_decrypt(
    const drv_pke_data *priv_key,
    const drv_pke_data *cipher_text,
    const drv_pke_data *plain_text)
{
    td_s32 ret = TD_FAILURE;
    td_u32 klen = DRV_PKE_SM2_LEN_IN_BYTES;
    td_u32 c1_index = 1;
    td_u32 c2_index = c1_index + klen * 3; // 3 : see the start position of algorithm C2.
    td_u32 c3_index = c1_index + klen * 2; // 2 : see the start position of algorithm C3.
    td_u32 i = 0;
    td_bool is_on_curve = TD_FALSE;
    td_u8 u[DRV_PKE_SM2_LEN_IN_BYTES];
    td_u8 C1x[DRV_PKE_SM2_LEN_IN_BYTES];
    td_u8 C1y[DRV_PKE_SM2_LEN_IN_BYTES];
    td_u8 x2[DRV_PKE_SM2_LEN_IN_BYTES];
    td_u8 y2[DRV_PKE_SM2_LEN_IN_BYTES];
    drv_pke_data u_data = {klen, u};
    drv_pke_data plain_data;
    const drv_pke_ecc_point C1_point = {C1x, C1y, klen};
    const drv_pke_ecc_point dBC1_point = {x2, y2, klen};
    const drv_pke_ecc_curve *sm2 = get_ecc_curve(DRV_PKE_ECC_TYPE_SM2);
    crypto_chk_return(sm2 == TD_NULL, PKE_COMPAT_ERRNO(ERROR_UNSUPPORT), "unsupport alg\n");

    crypto_param_check(cipher_text->data[0] != SM2_PC_UNCOMPRESS);
    plain_data = (drv_pke_data){cipher_text->length - SM2_CRYPTO_ADD_LENGTH_IN_BYTE, plain_text->data};

    ret = memcpy_s(C1x, sizeof(C1x), cipher_text->data + c1_index, klen);
    crypto_chk_return(ret != TD_SUCCESS, ret, "memcpy_s failed, ret is 0x%x\n", ret);
    ret = memcpy_s(C1y, sizeof(C1y), cipher_text->data + c1_index + klen, klen);
    crypto_chk_return(ret != TD_SUCCESS, ret, "memcpy_s failed, ret is 0x%x\n", ret);

    /* Step 1: check C1x, C1y on curve */
    ret = pke_check_dot_on_curve(sm2, &C1_point, &is_on_curve);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "pke_check_dot_on_curve failed, ret is 0x%x\n", ret);
    crypto_chk_goto(is_on_curve != TD_TRUE, exit__, "pke_check_dot_on_curve failed, ret is 0x%x\n", ret);

    /* Step 2: (x2, y2) = dB * C1 */
    ret = drv_cipher_pke_mul_dot(sm2, priv_key, &C1_point, &dBC1_point);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_dot failed, ret is 0x%x\n", ret);

    /* Step 3: check x2, y2 on curve */
    ret = pke_check_dot_on_curve(sm2, &dBC1_point, &is_on_curve);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "pke_check_dot_on_curve failed, ret is 0x%x\n", ret);
    crypto_chk_goto(is_on_curve != TD_TRUE, exit__,
        "pke_check_dot_on_curve failed, is_on_curve is 0x%x\n", is_on_curve);

    ret = sm2_kdf(&dBC1_point, plain_data.data, plain_data.length);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "sm2_kdf failed, ret is 0x%x\n", ret);

    if (drv_ecc_is_zero(plain_data.data, plain_data.length)) {
        crypto_log_err("%s:%d Error! Can't gen t in %d times!\n", __FUNCTION__, __LINE__, ECC_TRY_CNT);
        ret = PKE_COMPAT_ERRNO(ERROR_PKE_SM2_DECRYPT);
        goto exit__;
    }

    for (i = 0; i < plain_data.length; i++) {
        *(plain_data.data + i) ^= *(cipher_text->data + c2_index + i);
    }

    ret = sm2_encrypt_decrypt_hash(&dBC1_point, &plain_data, &u_data);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "sm2_encrypt_decrypt_hash failed, ret is 0x%x\n", ret);

    ret = memcmp(cipher_text->data + c3_index, u, DRV_PKE_SM2_LEN_IN_BYTES);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "memcmp failed, ret is 0x%x\n", ret);
exit__:
    (void)drv_cipher_pke_clean_ram();
    return ret;
}

/* Decode the private key for X25519 curve */
static td_void decode_priv_key(td_u8 *output_key, const td_u8 *input_key, td_u32 klen)
{
    td_u32 i = 0;
    /* Step 1: Do byte reverse on input_key */
    for (i = 0; i < klen; i++) {
        output_key[i] = input_key[klen - 1 - i];
    }

    /* Step 2: the priv_key[255], priv_key[254], priv_key[2], priv_key[1], priv_key[0] must be 0,1,0,0,0 */
    output_key[klen - 1] = output_key[klen - 1] & 0xF8; // 0xF8 : see comment above
    output_key[0] = output_key[0] & 0x7F; // 0x7F : see comment above
    output_key[0] = output_key[0] | 0x40; // 0x40 : see commnet above
    return;
}

/* Decode the public key for X25519 curve */
static td_void decode_pub_key(td_u8 *output_key, const td_u8 *input_key, td_u32 klen)
{
    td_u32 i = 0;
    /* Step 1: Do byte reverse on input_key */
    for (i = 0; i < klen; i++) {
        output_key[i] = input_key[klen - 1 - i];
    }
    /* Step 2: the pub_key[255] must be 0 */
    output_key[0] = output_key[0] & 0x7F; // 0x7F : see comment above
    return;
}

static td_s32 secret_expand(const td_u8 *secret, td_u8 *a, td_u8 *prefix, const td_u32 len)
{
    td_s32 ret = TD_FAILURE;
    td_u32 i = 0;
    td_u8 hash[DRV_PKE_LEN_512];
    drv_pke_data arr[1];
    drv_pke_data hash_data = {DRV_PKE_LEN_512, hash};

    arr[0].data = (td_u8 *)secret;
    arr[0].length = len;
    ret = drv_cipher_pke_calc_hash(arr, 1, DRV_PKE_HASH_TYPE_SHA512, &hash_data);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_calc_hash failed, ret is 0x%x\n", ret);

    for (i = 0; i < DRV_PKE_LEN_256; i++) {
        a[i] = hash[DRV_PKE_LEN_256 - 1 - i];
    }
    /* [255], [254], [2], [1], [0] must be 0,1,0,0,0 */
    a[DRV_PKE_LEN_256 - 1] &= 0xF8;
    a[0] &= 0x7F; // 0x7F : see comment above
    a[0] |= 0x40; // 0x40 : see commnet above
    (void)memcpy_s(prefix, DRV_PKE_LEN_256, hash + DRV_PKE_LEN_256, DRV_PKE_LEN_256);
    return TD_SUCCESS;
}

static td_s32 point_mul_compress(const drv_pke_ecc_curve *ecc, const td_u8 *k, td_u8 *out, td_u32 len)
{
    td_s32 ret = TD_FAILURE;
    td_u32 i;
    td_u8 rx[DRV_PKE_LEN_256] = {0};
    td_u8 ry[DRV_PKE_LEN_256] = {0};

    const drv_pke_data k_data = {DRV_PKE_LEN_256, (td_u8 *)k};
    const drv_pke_data ecc_p_data = {DRV_PKE_LEN_256, (td_u8 *)ecc->p};
    const drv_pke_ecc_point ecc_gpoint = {(td_u8 *)ecc->gx, (td_u8 *)ecc->gy, DRV_PKE_LEN_256};
    const drv_pke_ecc_point r_point = {rx, ry, DRV_PKE_LEN_256};

    crypto_param_check(len != DRV_PKE_LEN_256);

    /* R = k * G */
    ret = drv_cipher_pke_ed_mul_dot(&k_data, &ecc_gpoint, &ecc_p_data, &r_point);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_ed_mul_dot failed, ret is 0x%x\n", ret);
    for (i = 0; i < DRV_PKE_LEN_256; i++) {
        out[i] = ry[DRV_PKE_LEN_256 - 1 - i];
    }
    /* copy the least significant bit of the x coordinate to the most significant bit of the final octet. */
    out[DRV_PKE_LEN_256 - 1] |= (rx[DRV_PKE_LEN_256 - 1] & 0x1) << 7; /* shift 7 */
exit__:
    (void)drv_cipher_pke_clean_ram();
    return ret;
}

static td_s32 ed_secret_to_public(const drv_pke_ecc_curve *ecc, const drv_pke_data *priv_key,
    const drv_pke_ecc_point *pub_key)
{
    td_s32 ret = TD_FAILURE;
    td_u8 a[DRV_PKE_LEN_256] = {0};
    td_u8 dummy[DRV_PKE_LEN_256];

    /* Operation: a, prefix = secret_expand(secret) */
    ret = secret_expand(priv_key->data, a, dummy, DRV_PKE_LEN_256);
    crypto_chk_return(ret != TD_SUCCESS, ret, "secret_expand failed, ret is 0x%x\n", ret);
    /* Operation: A = point_compress(point_mul(a, G)) */
    (void)memset_s(pub_key->y, DRV_PKE_LEN_256, 0x00, DRV_PKE_LEN_256);
    ret = point_mul_compress(ecc, a, pub_key->y, DRV_PKE_LEN_256);
    crypto_chk_return(ret != TD_SUCCESS, ret, "point_mul_compress failed, ret is 0x%x\n", ret);

    return TD_SUCCESS;
}

static td_s32 pke_ecc_gen_key(const drv_pke_ecc_curve *ecc, const drv_pke_data *input_priv_key,
    const drv_pke_data *output_priv_key, const drv_pke_ecc_point *output_pub_key)
{
    td_s32 ret = TD_FAILURE;
    td_u32 i = 0;
    td_u8 key[DRV_PKE_LEN_256] = {0};
    td_u8 montgomery_p[DRV_PKE_LEN_256] = {0};
    td_u8 A24[DRV_PKE_LEN_256] = {0};
    td_u8 temp_result[DRV_PKE_LEN_256] = {0};
    const drv_pke_data key_data = {DRV_PKE_LEN_256, key};
    const drv_pke_data montgomery_p_data = {DRV_PKE_LEN_256, montgomery_p};
    const drv_pke_data A24_data = {DRV_PKE_LEN_256, A24};
    const drv_pke_data temp_result_data = {DRV_PKE_LEN_256, temp_result};
    const drv_pke_data ecc_p_data = {DRV_PKE_LEN_256, (td_u8 *)ecc->p};
    const drv_pke_data ecc_a_data = {DRV_PKE_LEN_256, (td_u8 *)ecc->a};
    const drv_pke_data ecc_gx_data = {DRV_PKE_LEN_256, (td_u8 *)ecc->gx};
    const drv_pke_ecc_point ecc_gpoint = {(td_u8 *)ecc->gx, (td_u8 *)ecc->gy, ecc->ksize};

    if ((input_priv_key != TD_NULL) && (input_priv_key->data != TD_NULL)) {
        // check whether the input priv key is valid
        if ((ecc->ecc_type != DRV_PKE_ECC_TYPE_RFC7748) && (ecc->ecc_type != DRV_PKE_ECC_TYPE_RFC8032)) {
            /* check 1 < input_priv_key < n */
            ret = drv_range_check(input_priv_key->data, ecc->n, ecc->ksize);
            crypto_chk_return(ret != TD_SUCCESS, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
                "drv_range_check failed, ret is 0x%x\n", ret);
        }
        (void)memcpy_s(output_priv_key->data, ecc->ksize, input_priv_key->data, ecc->ksize);
    } else {
        if ((ecc->ecc_type == DRV_PKE_ECC_TYPE_RFC7748) || (ecc->ecc_type == DRV_PKE_ECC_TYPE_RFC8032)) {
            /* Generate key with no limit */
            ret = get_randnum(output_priv_key->data, TD_NULL, ecc->ksize);
        } else { /* Generate key: 0 < key < n */
            ret = get_randnum(output_priv_key->data, (td_u8 *)ecc->n, ecc->ksize);
        }
        crypto_chk_return(ret != TD_SUCCESS, ret, "get_randnum failed, ret is 0x%x\n", ret);
    }
    /* PA = dA * G */
    if (ecc->ecc_type == DRV_PKE_ECC_TYPE_RFC7748) {
        /* The private key need to be decoded */
        ret = cal_montgomery_n(&ecc_p_data, &montgomery_p_data);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "cal_montgomery_n failed, ret is 0x%x\n", ret);
        ret = drv_cipher_pke_mul_mod(&ecc_a_data, &montgomery_p_data, &ecc_p_data, &A24_data);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);
        decode_priv_key(key, output_priv_key->data, ecc->ksize);
        ret = drv_cipher_pke_mg_mul_dot(&key_data, &ecc_gx_data, &ecc_p_data, &A24_data, &temp_result_data);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mg_mul_dot failed, ret is 0x%x\n", ret);
        /* The output should be byte reversed */
        for (i = 0; i < ecc->ksize; i++) {
            output_pub_key->x[i] = temp_result[ecc->ksize - 1 - i];
        }
        /* Returned priv_key[0]=.....000, priv_key[255] = 0....1.. */
        output_priv_key->data[0] = output_priv_key->data[0] & 0xF8; // 0xF8 : see comment above
        output_priv_key->data[ecc->ksize - 1] =
            output_priv_key->data[ecc->ksize - 1] & 0x7F; // 0x7F : see comment above
        output_priv_key->data[ecc->ksize - 1] =
            output_priv_key->data[ecc->ksize - 1] | 0x40; // 0x40 : see comment above
    } else if (ecc->ecc_type == DRV_PKE_ECC_TYPE_RFC8032) {
        ret = ed_secret_to_public(ecc, output_priv_key, output_pub_key);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "ed_secret_to_public failed, ret is 0x%x\n", ret);
    } else {
        ret = drv_cipher_pke_mul_dot(ecc, output_priv_key, &ecc_gpoint, output_pub_key);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_dot failed, ret is 0x%x\n", ret);
    }
exit__:
    (void)drv_cipher_pke_clean_ram();
    return ret;
}

/* z = d * p(x,y), sharekey = z(x) */
static td_s32 pke_ecc_gen_ecdh_key(
    const drv_pke_ecc_curve *ecc,
    const drv_pke_ecc_point *input_pub_key,
    const drv_pke_data *input_priv_key,
    const drv_pke_data *output_shared_key)
{
    td_s32 ret = TD_FAILURE;
    // for RFC7748
    td_u32 i = 0;
    td_u8 buffer[DRV_PKE_LEN_256 * 5] = {0}; // need buffer num is 5
    const drv_pke_data montgomery_p_data = {DRV_PKE_LEN_256, buffer + DRV_PKE_LEN_256 * i++};
    const drv_pke_data A24_data = {DRV_PKE_LEN_256, buffer + DRV_PKE_LEN_256 * i++};
    const drv_pke_data priv_key_data = {DRV_PKE_LEN_256, buffer + DRV_PKE_LEN_256 * i++};
    const drv_pke_data pub_key_data = {DRV_PKE_LEN_256, buffer + DRV_PKE_LEN_256 * i++};
    const drv_pke_data temp_result_data = {DRV_PKE_LEN_256, buffer + DRV_PKE_LEN_256 * i++};
    const drv_pke_data ecc_p_data = {DRV_PKE_LEN_256, (td_u8 *)ecc->p};
    const drv_pke_data ecc_a_data = {DRV_PKE_LEN_256, (td_u8 *)ecc->a};
    // for another curve type
    const drv_pke_ecc_point shared_point = {output_shared_key->data, TD_NULL, ecc->ksize};

    if (ecc->ecc_type == DRV_PKE_ECC_TYPE_RFC7748) {
        /* A24 = (a * 2 ^ len_n) mod p */
        ret = cal_montgomery_n(&ecc_p_data, &montgomery_p_data);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "cal_montgomery_n failed, ret is 0x%x\n", ret);
        ret = drv_cipher_pke_mul_mod(&ecc_a_data, &montgomery_p_data, &ecc_p_data, &A24_data);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

         /* The private and public key need to be decoded */
        decode_priv_key(priv_key_data.data, input_priv_key->data, DRV_PKE_LEN_256);
        decode_pub_key(pub_key_data.data, input_pub_key->x, DRV_PKE_LEN_256);

        ret = drv_cipher_pke_mg_mul_dot(&priv_key_data, &pub_key_data, &ecc_p_data, &A24_data, &temp_result_data);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mg_mul_dot failed, ret is 0x%x\n", ret);

        /* The output should be byte reversed */
        for (i = 0; i < ecc->ksize; i++) {
            output_shared_key->data[i] = temp_result_data.data[ecc->ksize - 1 - i];
        }
    } else {
        ret = drv_cipher_pke_mul_dot(ecc, input_priv_key, input_pub_key, &shared_point);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_dot failed, ret is 0x%x\n", ret);
    }
exit__:
    (void)drv_cipher_pke_clean_ram();
    return ret;
}

static td_s32 sm2_sign_verify_hash(const sm2_sign_verify_hash_pack *param, const drv_pke_msg *msg, drv_pke_data *hash)
{
    td_s32 ret = TD_FAILURE;
    td_u8 entla[ENTLA_LEN];
    td_u8 h[DRV_PKE_SM2_LEN_IN_BYTES];
    drv_pke_data hh = {DRV_PKE_SM2_LEN_IN_BYTES, h};
    drv_pke_data arr[8]; // The capacity of the array is 8.
    drv_pke_data arr2[2]; // The capacity of the array is 2.
    const drv_pke_ecc_curve *sm2 = get_ecc_curve(DRV_PKE_ECC_TYPE_SM2);
    crypto_chk_return(sm2 == TD_NULL, PKE_COMPAT_ERRNO(ERROR_UNSUPPORT), "unsupport alg\n");

    entla[0] = (td_u8)((param->id_len * BYTE_BITS) >> BYTE_BITS);
    entla[1] = (td_u8)((param->id_len * BYTE_BITS));

    /* ENTLA */
    arr[0].data = entla;
    arr[0].length = ENTLA_LEN;
    /* IDA */
    arr[1].data = param->id;
    arr[1].length = param->id_len;
    /* a */
    arr[2].data = (td_u8 *)sm2->a; // 2 is data of the 3th element
    arr[2].length = DRV_PKE_SM2_LEN_IN_BYTES; // 2 is length of the 3th element
    /* b */
    arr[3].data = (td_u8 *)sm2->b; // 3 is data of the 4th element
    arr[3].length = DRV_PKE_SM2_LEN_IN_BYTES; // 3 is length of the 4th element
    /* xG */
    arr[4].data = (td_u8 *)sm2->gx; // 4 is data of the 5th element
    arr[4].length = DRV_PKE_SM2_LEN_IN_BYTES; // 4 is length of the 5th element
    /* yG */
    arr[5].data = (td_u8 *)sm2->gy; // 5 is data of the 6th element
    arr[5].length = DRV_PKE_SM2_LEN_IN_BYTES; // 5 is length of the 6th element
    /* xA */
    arr[6].data = param->px; // 6 is data of the 7th element
    arr[6].length = DRV_PKE_SM2_LEN_IN_BYTES; // 6 islength of the 7th element
    /* yA */
    arr[7].data = param->py; // 7 is data of the 8th element
    arr[7].length = DRV_PKE_SM2_LEN_IN_BYTES; // 7 is length of the 8th element

    /* ZA */
    arr2[0].data = h;
    arr2[0].length = DRV_PKE_SM2_LEN_IN_BYTES;
    /* M */
    arr2[1].data = msg->data;
    arr2[1].length = msg->length;

    /* ZA = SM3( ENTLA || IDA || a || b || xG || yG || xA || yA ) */
    ret = drv_cipher_pke_calc_hash(arr, sizeof(arr) / sizeof(arr[0]), DRV_PKE_HASH_TYPE_SM3, &hh);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_calc_hash failed, ret is 0x%x\n", ret);
    /* e = SM3( ZA || M ) */
    ret = drv_cipher_pke_calc_hash(arr2, sizeof(arr2) / sizeof(arr2[0]), DRV_PKE_HASH_TYPE_SM3, hash);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_calc_hash failed, ret is 0x%x\n", ret);

    return ret;
}

static td_s32 pke_sm2_sign(const drv_pke_data *priv_key, const drv_pke_data *hash, const drv_pke_ecc_sig *sig)
{
    td_s32 ret = TD_FAILURE;
    td_u32 klen = DRV_PKE_SM2_LEN_IN_BYTES;
    td_u32 i;
    drv_pke_data var_arr[PKE_SM2_SIGN_VAR_MAX];
    const drv_pke_ecc_curve *sm2;
    drv_pke_ecc_point sm2_gpoint;
    drv_pke_ecc_point r_point;
    td_u8 *buffer;

    sm2 = get_ecc_curve(DRV_PKE_ECC_TYPE_SM2);
    crypto_chk_return(sm2 == TD_NULL, PKE_COMPAT_ERRNO(ERROR_UNSUPPORT), "unsupport alg\n");

    buffer = crypto_malloc(klen * SM2_SIGN_BUF_CNT);
    if (buffer == TD_NULL) {
        crypto_log_err("%s:%d Error! Malloc memory failed!\n", __FUNCTION__, __LINE__);
        return PKE_COMPAT_ERRNO(ERROR_MALLOC);
    }
    (void)memset_s(buffer, klen * SM2_SIGN_BUF_CNT, 0, klen * SM2_SIGN_BUF_CNT);

    for (i = 0; i < PKE_SM2_SIGN_VAR_MAX; i++) {
        if (i <= SM2SIGN_ONE) {
            var_arr[i].data = buffer + klen * i;
        }
        var_arr[i].length = klen;
    }

    var_arr[SM2SIGN_SM2_N].data = (td_u8 *)sm2->n;
    var_arr[SM2SIGN_SIG_R].data = sig->r;
    var_arr[SM2SIGN_SIG_S].data = sig->s;
    var_arr[SM2SIGN_ONE].data[klen - 1] = 0x1;
    sm2_gpoint = (drv_pke_ecc_point){(td_u8 *)sm2->gx, (td_u8 *)sm2->gy, klen};
    r_point = (drv_pke_ecc_point){var_arr[SM2SIGN_R_X].data, var_arr[SM2SIGN_R_Y].data, klen};

    // Step 1: N = r^2 mod p where N is montgomery multiplier
    ret = cal_montgomery_n(&var_arr[SM2SIGN_SM2_N], &var_arr[SM2SIGN_MONTGOMERY_N]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "cal_montgomery_n failed, ret is 0x%x\n", ret);

    for (i = 0; i < PKE_MAX_TIMES; i++) {
        /* Step 1: random k */
        ret = get_randnum(var_arr[SM2SIGN_K].data, (td_u8 *)sm2->n, klen);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "get_randnum failed, ret is 0x%x\n", ret);

        /* Step 2: R = k * G */
        ret = drv_cipher_pke_mul_dot(sm2, &var_arr[SM2SIGN_K], &sm2_gpoint, &r_point);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_dot failed, ret is 0x%x\n", ret);

        /* Step 3: E = e + 0 mod n */
        ret = drv_cipher_pke_add_mod(hash, &var_arr[SM2SIGN_ZERO], &var_arr[SM2SIGN_SM2_N], &var_arr[SM2SIGN_E]);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_add_mod failed, ret is 0x%x\n", ret);

        /* Step 4: r = E + Rx mod n */
        ret = drv_cipher_pke_add_mod(&var_arr[SM2SIGN_E], &var_arr[SM2SIGN_R_X], &var_arr[SM2SIGN_SM2_N],
            &var_arr[SM2SIGN_SIG_R]);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_add_mod failed, ret is 0x%x\n", ret);

        /* Step 5: r = 0 ? */
        if (drv_ecc_is_zero(var_arr[SM2SIGN_SIG_R].data, klen)) {
            continue;
        }
        /* Step 6: rk = r + k mod n */
        ret = drv_cipher_pke_add_mod(&var_arr[SM2SIGN_SIG_R], &var_arr[SM2SIGN_K], &var_arr[SM2SIGN_SM2_N],
            &var_arr[SM2SIGN_RK]);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_add_mod failed, ret is 0x%x\n", ret);

        /* Step 7: rk = 0 ? */
        if (drv_ecc_is_zero(var_arr[SM2SIGN_RK].data, klen)) {
            continue;
        }

        /* Step 8: random my */
        ret = get_randnum(var_arr[SM2SIGN_MY].data, (td_u8 *)sm2->n, klen);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "get_randnum failed, ret is 0x%x\n", ret);

        /* Step 9: mr = r * N mod n */
        ret = drv_cipher_pke_mul_mod(&var_arr[SM2SIGN_SIG_R], &var_arr[SM2SIGN_MONTGOMERY_N], &var_arr[SM2SIGN_SM2_N],
            &var_arr[SM2SIGN_MR]);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

        /* Step 10: mk = k * N mod n */
        ret = drv_cipher_pke_mul_mod(&var_arr[SM2SIGN_K], &var_arr[SM2SIGN_MONTGOMERY_N], &var_arr[SM2SIGN_SM2_N],
            &var_arr[SM2SIGN_MK]);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

        /* Step 11: mrk = mr + mk mod n */
        ret = drv_cipher_pke_add_mod(&var_arr[SM2SIGN_MR], &var_arr[SM2SIGN_MK],
            &var_arr[SM2SIGN_SM2_N], &var_arr[SM2SIGN_MRK]);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_add_mod failed, ret is 0x%x\n", ret);

        /* Step 12: mrky = mrk * my mod n */
        ret = drv_cipher_pke_mul_mod(&var_arr[SM2SIGN_MRK], &var_arr[SM2SIGN_MY],
            &var_arr[SM2SIGN_SM2_N], &var_arr[SM2SIGN_MRKY]);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

        /* Step 13: TA = dA * my mod n */
        ret = drv_cipher_pke_mul_mod(priv_key, &var_arr[SM2SIGN_MY], &var_arr[SM2SIGN_SM2_N], &var_arr[SM2SIGN_TA]);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

        /* Step 14: mTA = TA * N mod n */
        ret = drv_cipher_pke_mul_mod(&var_arr[SM2SIGN_TA], &var_arr[SM2SIGN_MONTGOMERY_N], &var_arr[SM2SIGN_SM2_N],
            &var_arr[SM2SIGN_MTA]);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

        /* Step 15: mTy = mTA + my mod n */
        ret = drv_cipher_pke_add_mod(&var_arr[SM2SIGN_MTA], &var_arr[SM2SIGN_MY], &var_arr[SM2SIGN_SM2_N],
            &var_arr[SM2SIGN_MTY]);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_add_mod failed, ret is 0x%x\n", ret);
        
        /* Step 16: minv = mTy^-1 mod n */
        ret = drv_cipher_pke_inv_mod(&var_arr[SM2SIGN_MTY], &var_arr[SM2SIGN_SM2_N], &var_arr[SM2SIGN_MINV]);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_inv_mod failed, ret is 0x%x\n", ret);

        /* Step 20: mTz = mrky * minv mod n */
        ret = drv_cipher_pke_mul_mod(&var_arr[SM2SIGN_MRKY], &var_arr[SM2SIGN_MINV], &var_arr[SM2SIGN_SM2_N],
            &var_arr[SM2SIGN_MTZ]);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

        /* Step 21: ms = mTz - mr mod n */
        ret = drv_cipher_pke_sub_mod(&var_arr[SM2SIGN_MTZ], &var_arr[SM2SIGN_MR],
            &var_arr[SM2SIGN_SM2_N], &var_arr[SM2SIGN_MS]);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_sub_mod failed, ret is 0x%x\n", ret);

        /* Step 22: s = ms * 1 mod n */
        ret = drv_cipher_pke_mul_mod(&var_arr[SM2SIGN_MS], &var_arr[SM2SIGN_ONE], &var_arr[SM2SIGN_SM2_N],
            &var_arr[SM2SIGN_SIG_S]);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

        /* Step 23: s = 0 ? */
        if (drv_ecc_is_zero(var_arr[SM2SIGN_SIG_S].data, klen) == TD_FALSE) {
            break;
        }
    }

    if (i >= PKE_MAX_TIMES) {
        crypto_log_err("Generate rk timeout!\n");
        ret = PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM);
        goto exit__;
    }

exit__:
    (void)drv_cipher_pke_clean_ram();
    if (buffer != TD_NULL) {
        (void)memset_s(buffer, klen * SM2_SIGN_BUF_CNT, 0, klen * SM2_SIGN_BUF_CNT);
        crypto_free(buffer);
    }
    return ret;
}

static td_s32 pke_sm2_verify(const drv_pke_ecc_point *pub_key, const drv_pke_data *hash,
    const drv_pke_ecc_sig *sig)
{
    td_s32 ret = TD_FAILURE;
    td_u32 klen = DRV_PKE_SM2_LEN_IN_BYTES;
    td_u32 i;
    drv_pke_data var_arr[PKE_SM2_VERIFY_VAR_MAX];
    const drv_pke_ecc_curve *sm2;
    drv_pke_ecc_point sm2_gpoint;
    drv_pke_ecc_point sG_point;
    drv_pke_ecc_point tPA_point;
    drv_pke_ecc_point r_point;
    td_u8 *buffer;

    sm2 = get_ecc_curve(DRV_PKE_ECC_TYPE_SM2);
    crypto_chk_return(sm2 == TD_NULL, PKE_COMPAT_ERRNO(ERROR_UNSUPPORT), "unsupport alg\n");

    if (drv_range_check(sig->r, sm2->n, DRV_PKE_SM2_LEN_IN_BYTES) != TD_SUCCESS) {
        crypto_log_err("%s:%d: sig->r check range failed!\n", __func__, __LINE__);
        return PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM);
    }
    if (drv_range_check(sig->s, sm2->n, DRV_PKE_SM2_LEN_IN_BYTES) != TD_SUCCESS) {
        crypto_log_err("%s:%d: sig->s check range failed!\n", __func__, __LINE__);
        return PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM);
    }

    buffer = crypto_malloc(klen * SM2_VERIFY_BUF_CNT);
    if (buffer == TD_NULL) {
        crypto_log_err("%s:%d Error! Malloc memory failed!\n", __FUNCTION__, __LINE__);
        return PKE_COMPAT_ERRNO(ERROR_MALLOC);
    }
    (void)memset_s(buffer, klen * SM2_VERIFY_BUF_CNT, 0, klen * SM2_VERIFY_BUF_CNT);
    for (i = 0; i < PKE_SM2_VERIFY_VAR_MAX; i++) {
        if (i <= SM2VERIFY_ZERO) {
            var_arr[i].data = buffer + klen * i;
        }
        var_arr[i].length = klen;
    }
    var_arr[SM2VERIFY_SM2_N].data = (td_u8 *)sm2->n;
    var_arr[SM2VERIFY_SIG_R].data = sig->r;
    var_arr[SM2VERIFY_SIG_S].data = sig->s;
    sm2_gpoint = (drv_pke_ecc_point){(td_u8 *)sm2->gx, (td_u8 *)sm2->gy, klen};
    sG_point = (drv_pke_ecc_point){var_arr[SM2VERIFY_S_GX].data, var_arr[SM2VERIFY_S_GY].data, klen};
    tPA_point = (drv_pke_ecc_point){var_arr[SM2VERIFY_T_PAX].data, var_arr[SM2VERIFY_T_PAY].data, klen};
    r_point = (drv_pke_ecc_point){var_arr[SM2VERIFY_RX].data, var_arr[SM2VERIFY_RY].data, klen};
    /* Step 1: t = r + s mod n */
    ret = drv_cipher_pke_add_mod(&var_arr[SM2VERIFY_SIG_R], &var_arr[SM2VERIFY_SIG_S], &var_arr[SM2VERIFY_SM2_N],
        &var_arr[SM2VERIFY_T]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_add_mod failed, ret is 0x%x\n", ret);
    /* Step 2: t = 0 ? */
    if (drv_ecc_is_zero(var_arr[SM2VERIFY_T].data, klen)) {
        crypto_log_err("t = 0!\n");
        ret = TD_FAILURE;
        goto exit__;
    }
    /* Step 3: sG = s * G */
    ret = drv_cipher_pke_mul_dot(sm2, &var_arr[SM2VERIFY_SIG_S], &sm2_gpoint, &sG_point);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_dot failed, ret is 0x%x\n", ret);
    /* Step 4: tPA = t * PA */
    ret = drv_cipher_pke_mul_dot(sm2, &var_arr[SM2VERIFY_T], pub_key, &tPA_point);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_dot failed, ret is 0x%x\n", ret);
    /* Step 5: R = sG + tPA */
    ret = drv_cipher_pke_add_dot(sm2, &sG_point, &tPA_point, &r_point);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_add_dot failed, ret is 0x%x\n", ret);
    /* Step 6: e = hash + 0 mod n */
    ret = drv_cipher_pke_add_mod(hash, &var_arr[SM2VERIFY_ZERO], &var_arr[SM2VERIFY_SM2_N], &var_arr[SM2VERIFY_E]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_add_mod failed, ret is 0x%x\n", ret);
    /* Step 7: v = e + Rx mod n */
    ret = drv_cipher_pke_add_mod(&var_arr[SM2VERIFY_E], &var_arr[SM2VERIFY_RX], &var_arr[SM2VERIFY_SM2_N],
        &var_arr[SM2VERIFY_V]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_add_mod failed, ret is 0x%x\n", ret);
    /* Step 8: v = r ? */
    if (memcmp(sig->r, var_arr[SM2VERIFY_V].data, klen) != 0) {
        crypto_log_err("v != r!\n");
        ret = TD_FAILURE;
    }
exit__:
    (void)drv_cipher_pke_clean_ram();
    if (buffer != TD_NULL) {
        (void)memset_s(buffer, klen * SM2_VERIFY_BUF_CNT, 0, klen * SM2_VERIFY_BUF_CNT);
        crypto_free(buffer);
    }
    return ret;
}

static td_s32 pke_ecdsa_sign(
    const drv_pke_ecc_curve *ecc,
    const drv_pke_data *priv_key,
    const drv_pke_data *hash,
    const drv_pke_ecc_sig *sig)
{
    td_s32 ret = TD_FAILURE;
    td_u32 klen = ecc->ksize;
    td_u32 i;
    drv_pke_data var_arr[PKE_ECDSA_SIGN_VAR_MAX];
    drv_pke_ecc_point ecc_gpoint;
    drv_pke_ecc_point r_point;
    td_u8 *buffer;

    if (ecc->ecc_type == DRV_PKE_ECC_TYPE_SM2) {
        return pke_sm2_sign(priv_key, hash, sig);
    }
    buffer = crypto_malloc(klen * ECDSA_SIGN_BUF_CNT);
    crypto_chk_return(buffer == TD_NULL, PKE_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");

    (void)memset_s(buffer, klen * ECDSA_SIGN_BUF_CNT, 0, klen * ECDSA_SIGN_BUF_CNT);
    for (i = 0; i < PKE_ECDSA_SIGN_VAR_MAX - 1; i++) {
        if (i <= ECCSIGN_ONE) {
            var_arr[i].data = buffer + klen * i;
        }
        var_arr[i].length = klen;
    }
    var_arr[ECCSIGN_ECC_N].data = (td_u8 *)ecc->n;
    var_arr[ECCSIGN_SIG_R].data = sig->r;
    var_arr[ECCSIGN_SIG_S].data = sig->s;
    var_arr[ECCSIGN_HASH].data = hash->data;
    var_arr[ECCSIGN_HASH].length = crypto_min(hash->length, ecc->ksize);
    var_arr[ECCSIGN_ONE].data[klen - 1] = 0x1;
    ecc_gpoint = (drv_pke_ecc_point){(td_u8 *)ecc->gx, (td_u8 *)ecc->gy, klen};
    r_point = (drv_pke_ecc_point){var_arr[ECCSIGN_RX].data, var_arr[ECCSIGN_RY].data, klen};

    // 1. N = 2 ^ 2len_n mod n where N = R ^ 2
    ret = cal_montgomery_n(&var_arr[ECCSIGN_ECC_N], &var_arr[ECCSIGN_MONTGOMERY_N]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "cal_montgomery_n failed, ret is 0x%x\n", ret);

    for (i = 0; i < ECC_TRY_CNT; i++) {
        // 2. generate randnum k
        ret = get_randnum(var_arr[ECCSIGN_K].data, (td_u8 *)ecc->n, klen);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "get_randnum failed, ret is 0x%x\n", ret);
        // 3. R = k*G
        ret = drv_cipher_pke_mul_dot(ecc, &var_arr[ECCSIGN_K], &ecc_gpoint, &r_point);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_dot failed, ret is 0x%x\n", ret);
        // 4. r= (xR + 0) mod n
        ret = drv_cipher_pke_add_mod(&var_arr[ECCSIGN_RX], &var_arr[ECCSIGN_ZERO], &var_arr[ECCSIGN_ECC_N],
            &var_arr[ECCSIGN_SIG_R]);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_add_mod failed, ret is 0x%x\n", ret);
        // 5. check r ?=0
        if (drv_ecc_is_zero(var_arr[ECCSIGN_SIG_R].data, klen)) {
            continue;
        }

        // 6. E = hash + 0 mod n
        ret = drv_cipher_pke_add_mod(&var_arr[ECCSIGN_HASH], &var_arr[ECCSIGN_ZERO], &var_arr[ECCSIGN_ECC_N],
            &var_arr[ECCSIGN_E]);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_add_mod failed, ret is 0x%x\n", ret);

        // 7. mE = E * N mod n
        ret = drv_cipher_pke_mul_mod(&var_arr[ECCSIGN_E], &var_arr[ECCSIGN_MONTGOMERY_N], &var_arr[ECCSIGN_ECC_N],
            &var_arr[ECCSIGN_ME]);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

        // 8. mr = r * N mod n
        ret = drv_cipher_pke_mul_mod(&var_arr[ECCSIGN_SIG_R], &var_arr[ECCSIGN_MONTGOMERY_N], &var_arr[ECCSIGN_ECC_N],
            &var_arr[ECCSIGN_MR]);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

        // 9. md = d * N mod n
        ret = drv_cipher_pke_mul_mod(priv_key, &var_arr[ECCSIGN_MONTGOMERY_N], &var_arr[ECCSIGN_ECC_N],
            &var_arr[ECCSIGN_MD]);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

        // 10. mrd = md * mr mod n
        ret = drv_cipher_pke_mul_mod(&var_arr[ECCSIGN_MD], &var_arr[ECCSIGN_MR], &var_arr[ECCSIGN_ECC_N],
            &var_arr[ECCSIGN_MRD]);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

        // 11. y = mE + mrd mod n
        ret = drv_cipher_pke_add_mod(&var_arr[ECCSIGN_ME], &var_arr[ECCSIGN_MRD], &var_arr[ECCSIGN_ECC_N],
            &var_arr[ECCSIGN_Y]);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_add_mod failed, ret is 0x%x\n", ret);

        // 12. mk = k * N mod n
        ret = drv_cipher_pke_mul_mod(&var_arr[ECCSIGN_K], &var_arr[ECCSIGN_MONTGOMERY_N], &var_arr[ECCSIGN_ECC_N],
            &var_arr[ECCSIGN_MK]);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

        // 13. mkni = mk ^ -1 mod n
        ret = drv_cipher_pke_inv_mod(&var_arr[ECCSIGN_MK], &var_arr[ECCSIGN_ECC_N], &var_arr[ECCSIGN_MKNI]);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_inv_mod failed, ret is 0x%x\n", ret);

        // 14. ms = mkni * y mod n
        ret = drv_cipher_pke_mul_mod(&var_arr[ECCSIGN_MKNI], &var_arr[ECCSIGN_Y], &var_arr[ECCSIGN_ECC_N],
            &var_arr[ECCSIGN_MS]);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

        // 15. s = ms * 1 mod n
        ret = drv_cipher_pke_mul_mod(&var_arr[ECCSIGN_MS], &var_arr[ECCSIGN_ONE], &var_arr[ECCSIGN_ECC_N],
            &var_arr[ECCSIGN_SIG_S]);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

        // 16. check s ?=0
        if (drv_ecc_is_zero(var_arr[ECCSIGN_SIG_S].data, klen)) {
            continue;
        }
        break;
    }
    if (i == ECC_TRY_CNT) {
        crypto_log_err("Error! K is Invalid!\n");
        ret = PKE_COMPAT_ERRNO(ERROR_TRY_TIMES);
        goto exit__;
    }
exit__:
    (void)drv_cipher_pke_clean_ram();
    if (buffer != TD_NULL) {
        (void)memset_s(buffer, klen * ECDSA_SIGN_BUF_CNT, 0, klen * ECDSA_SIGN_BUF_CNT);
        crypto_free(buffer);
        buffer = TD_NULL;
    }
    return ret;
}

static td_s32 pke_ecdsa_verify(
    const drv_pke_ecc_curve *ecc,
    const drv_pke_ecc_point *pub_key,
    const drv_pke_data *hash,
    const drv_pke_ecc_sig *sig)
{
    td_s32 ret = TD_FAILURE;
    td_u32 klen = ecc->ksize;
    td_u32 i;
    drv_pke_data var_arr[PKE_ECDSA_VERIFY_VAR_MAX];
    drv_pke_ecc_point ecc_gpoint;
    drv_pke_ecc_point r_point;
    drv_pke_ecc_point u1_point;
    drv_pke_ecc_point u2_point;
    td_u8 *buffer;

    if (ecc->ecc_type == DRV_PKE_ECC_TYPE_SM2) {
        return pke_sm2_verify(pub_key, hash, sig);
    }
    buffer = crypto_malloc(klen * ECDSA_VERIFY_BUF_CNT);
    crypto_chk_return(buffer == TD_NULL, PKE_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");
    (void)memset_s(buffer, klen * ECDSA_VERIFY_BUF_CNT, 0, klen * ECDSA_VERIFY_BUF_CNT);

    for (i = 0; i < PKE_ECDSA_VERIFY_VAR_MAX - 1; i++) {
        if (i <= ECCVERIFY_ONE) {
            var_arr[i].data = buffer + klen * i;
        }
        var_arr[i].length = klen;
    }
    var_arr[ECCVERIFY_ECC_N].data = (td_u8 *)ecc->n;
    var_arr[ECCVERIFY_SIG_R].data = sig->r;
    var_arr[ECCVERIFY_SIG_S].data = sig->s;
    var_arr[ECCVERIFY_HASH].data = hash->data;
    var_arr[ECCVERIFY_HASH].length = crypto_min(hash->length, klen);
    var_arr[ECCVERIFY_ONE].data[klen - 1] = 0x1;

    ecc_gpoint = (drv_pke_ecc_point){(td_u8 *)ecc->gx, (td_u8 *)ecc->gy, klen};
    r_point = (drv_pke_ecc_point){var_arr[ECCVERIFY_RX].data, var_arr[ECCVERIFY_RY].data, klen};
    u1_point = (drv_pke_ecc_point){var_arr[ECCVERIFY_U1X].data, var_arr[ECCVERIFY_U1Y].data, klen};
    u2_point = (drv_pke_ecc_point){var_arr[ECCVERIFY_U2X].data, var_arr[ECCVERIFY_U2Y].data, klen};

    /* check 0 < r < n ? */
    if (drv_range_check(sig->r, ecc->n, klen) != TD_SUCCESS) {
        ret = PKE_COMPAT_ERRNO(ERROR_PKE_ECDSA_VERIFY_CHECK);
        goto exit__;
    }

    /* check 0< s < n ? */
    if (drv_range_check(sig->s, ecc->n, klen) != TD_SUCCESS) {
        ret = PKE_COMPAT_ERRNO(ERROR_PKE_ECDSA_VERIFY_CHECK);
        goto exit__;
    }

    // 1. N =2 ^ 2len_n mod n
    ret = cal_montgomery_n(&var_arr[ECCVERIFY_ECC_N], &var_arr[ECCVERIFY_MONTGOMERY_N]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "cal_montgomery_n failed, ret is 0x%x\n", ret);

    // 2. ms = s * N mod n
    ret = drv_cipher_pke_mul_mod(&var_arr[ECCVERIFY_SIG_S], &var_arr[ECCVERIFY_MONTGOMERY_N], &var_arr[ECCVERIFY_ECC_N],
        &var_arr[ECCVERIFY_MS]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

    // 3. msni = ms ^ -1 mod n
    ret = drv_cipher_pke_inv_mod(&var_arr[ECCVERIFY_MS], &var_arr[ECCVERIFY_ECC_N], &var_arr[ECCVERIFY_MSNI]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_inv_mod failed, ret is 0x%x\n", ret);

    // 4. E = hash + 0 mod n
    ret = drv_cipher_pke_add_mod(&var_arr[ECCVERIFY_HASH], &var_arr[ECCVERIFY_ZERO], &var_arr[ECCVERIFY_ECC_N],
        &var_arr[ECCVERIFY_E]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_add_mod failed, ret is 0x%x\n", ret);

    // 5. mE = E * N mod n
    ret = drv_cipher_pke_mul_mod(&var_arr[ECCVERIFY_E], &var_arr[ECCVERIFY_MONTGOMERY_N], &var_arr[ECCVERIFY_ECC_N],
        &var_arr[ECCVERIFY_ME]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

    // 6. mu1 = mE * msni mod n
    ret = drv_cipher_pke_mul_mod(&var_arr[ECCVERIFY_ME], &var_arr[ECCVERIFY_MSNI], &var_arr[ECCVERIFY_ECC_N],
        &var_arr[ECCVERIFY_MU1]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

    // 7. u1 = mu1 * 1 mod n
    ret = drv_cipher_pke_mul_mod(&var_arr[ECCVERIFY_MU1], &var_arr[ECCVERIFY_ONE], &var_arr[ECCVERIFY_ECC_N],
        &var_arr[ECCVERIFY_U1]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

    // 8. mr = r * N mod n
    ret = drv_cipher_pke_mul_mod(&var_arr[ECCVERIFY_SIG_R], &var_arr[ECCVERIFY_MONTGOMERY_N], &var_arr[ECCVERIFY_ECC_N],
        &var_arr[ECCVERIFY_MR]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

    // 9. mu2 = mr * msni mod n
    ret = drv_cipher_pke_mul_mod(&var_arr[ECCVERIFY_MR], &var_arr[ECCVERIFY_MSNI], &var_arr[ECCVERIFY_ECC_N],
        &var_arr[ECCVERIFY_MU2]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

    // 10. u2 = mu2 * 1 mod n
    ret = drv_cipher_pke_mul_mod(&var_arr[ECCVERIFY_MU2], &var_arr[ECCVERIFY_ONE], &var_arr[ECCVERIFY_ECC_N],
        &var_arr[ECCVERIFY_U2]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

    // 11. u1G = u1 * G
    ret = drv_cipher_pke_mul_dot(ecc, &var_arr[ECCVERIFY_U1], &ecc_gpoint, &u1_point);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_dot failed, ret is 0x%x\n", ret);

    // 12. u2Q = u2 * Qu
    ret = drv_cipher_pke_mul_dot(ecc, &var_arr[ECCVERIFY_U2], pub_key, &u2_point);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_dot failed, ret is 0x%x\n", ret);

    // 13. R = u1G + u2Q
    ret = drv_cipher_pke_add_dot(ecc, &u1_point, &u2_point, &r_point);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_add_dot failed, ret is 0x%x\n", ret);

    // 14. v= (xR + 0) mod n
    ret = drv_cipher_pke_add_mod(&var_arr[ECCVERIFY_RX], &var_arr[ECCVERIFY_ZERO], &var_arr[ECCVERIFY_ECC_N],
        &var_arr[ECCVERIFY_V]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_add_mod failed, ret is 0x%x\n", ret);

    // v = r ?
    if (memcmp(sig->r, var_arr[ECCVERIFY_V].data, klen) != 0) {
        crypto_log_err("Error! r != v!\n");
        ret = PKE_COMPAT_ERRNO(ERROR_PKE_ECDSA_VERIFY_CHECK);
    }

exit__:
    (void)drv_cipher_pke_clean_ram();
    if (buffer != TD_NULL) {
        (void)memset_s(buffer, klen * ECDSA_VERIFY_BUF_CNT, 0, klen * ECDSA_VERIFY_BUF_CNT);
        crypto_free(buffer);
        buffer = TD_NULL;
    }
    return ret;
}

static td_s32 sha512_modq(const drv_pke_data *q, const drv_pke_data *in1, const drv_pke_data *in2,
    const drv_pke_data *in3, const drv_pke_data *out)
{
    td_s32 ret = TD_FAILURE;
    td_u8 h[DRV_PKE_LEN_512];
    td_u8 hash[DRV_PKE_LEN_512];
    td_u32 i = 0;
    drv_pke_data arr[ED25519_MAX_HASH_ELEMENTS];
    drv_pke_data hash_data = {DRV_PKE_LEN_512, hash};
    const drv_pke_data h_data = {DRV_PKE_LEN_512, h};

    /* H = Hash(in1 || in2 || in3) */
    if ((in1 != TD_NULL) && (in1->data != TD_NULL)) {
        arr[i].data = in1->data;
        arr[i].length = in1->length;
        i++;
    }
    if ((in2 != TD_NULL) && (in2->data != TD_NULL)) {
        arr[i].data = in2->data;
        arr[i].length = in2->length;
        i++;
    }
    if ((in3 != TD_NULL) && (in3->data != TD_NULL)) {
        arr[i].data = in3->data;
        arr[i].length = in3->length;
        i++;
    }
    ret = drv_cipher_pke_calc_hash(arr, i, DRV_PKE_HASH_TYPE_SHA512, &hash_data);
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_calc_hash failed, ret is 0x%x\n", ret);
    for (i = 0; i < DRV_PKE_LEN_512; i++) {
        h[i] = hash[DRV_PKE_LEN_512 - 1 - i];
    }
    /* For efficiency, reduce k modulo L first. L equals to ecc->n */
    ret = drv_cipher_pke_mod(&h_data, q, out);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mod failed, ret is 0x%x\n", ret);
exit__:
    (void)drv_cipher_pke_clean_ram();
    return ret;
}

/* out = in ^ k mod n */
static td_s32 ed_exp_mod(const td_u8 *n, const td_u8 *k, const td_u8 *in, td_u8 *out)
{
    td_s32 ret = TD_FAILURE;
    td_u8 *nn;
    td_u8 *kk;
    td_u8 *ii;
    td_u8 *oo;
    td_u32 i = 0;
    drv_pke_data n_data;
    drv_pke_data k_data;
    drv_pke_data i_data;
    drv_pke_data o_data;
    td_u8 *buffer;
    buffer = crypto_malloc(DRV_PKE_LEN_2048 * 4); // need buffer num is 4
    if (buffer == TD_NULL) {
        crypto_log_err("%s:%d Error! Malloc memory failed!\n", __FUNCTION__, __LINE__);
        return PKE_COMPAT_ERRNO(ERROR_MALLOC);
    }
    (void)memset_s(buffer, DRV_PKE_LEN_2048 * 4, 0x00, DRV_PKE_LEN_2048 * 4); // need buffer num is 4

    nn = buffer + DRV_PKE_LEN_2048 * i++;
    kk = buffer + DRV_PKE_LEN_2048 * i++;
    ii = buffer + DRV_PKE_LEN_2048 * i++;
    oo = buffer + DRV_PKE_LEN_2048 * i++;
    n_data = (drv_pke_data){DRV_PKE_LEN_2048, nn};
    k_data = (drv_pke_data){DRV_PKE_LEN_2048, kk};
    i_data = (drv_pke_data){DRV_PKE_LEN_2048, ii};
    o_data = (drv_pke_data){DRV_PKE_LEN_2048, oo};

    for (i = 0; i < DRV_PKE_LEN_256; i++) {
        nn[i + DRV_PKE_LEN_2048 - DRV_PKE_LEN_256] = n[i];
        kk[i + DRV_PKE_LEN_2048 - DRV_PKE_LEN_256] = k[i];
        ii[i + DRV_PKE_LEN_2048 - DRV_PKE_LEN_256] = in[i];
    }
    ret = drv_cipher_pke_exp_mod(&n_data, &k_data, &i_data, &o_data);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_exp_mod failed, ret is 0x%x\n", ret);
    for (i = 0; i < DRV_PKE_LEN_256; i++) {
        out[i] = oo[i + DRV_PKE_LEN_2048 - DRV_PKE_LEN_256];
    }
exit__:
    (void)drv_cipher_pke_clean_ram();
    if (buffer != TD_NULL) {
        (void)memset_s(buffer, DRV_PKE_LEN_2048 * 4, 0x00, DRV_PKE_LEN_2048 * 4); // need free buffer num is 4
        crypto_free(buffer);
    }
    return ret;
}

static td_s32 generate_x(const drv_pke_ecc_curve *ecc, const td_u8 *x2, td_u8 *x, const td_u32 klen)
{
    td_s32 ret = TD_FAILURE;
    td_u32 i;
    drv_pke_data var_arr[GENERATE_X_VAR_MAX];
    td_u8 *buffer;

    buffer = crypto_malloc(klen * GENERATE_X_BUF_CNT);
    if (buffer == TD_NULL) {
        crypto_log_err("%s:%d Error! Malloc memory failed!\n", __FUNCTION__, __LINE__);
        return PKE_COMPAT_ERRNO(ERROR_MALLOC);
    }
    (void)memset_s(buffer, klen * GENERATE_X_BUF_CNT, 0, klen * GENERATE_X_BUF_CNT);

    for (i = 0; i < GENERATE_X_VAR_MAX; i++) {
        if (i <= GENERATEX_THREE) {
            var_arr[i].data = buffer + klen * i;
        }
        var_arr[i].length = klen;
    }
    var_arr[GENERATEX_ECC_P].data = (td_u8 *)ecc->p;
    var_arr[GENERATEX_X].data = x;
    var_arr[GENERATEX_X2].data = (td_u8 *)x2;
    var_arr[GENERATEX_ONE].data[klen - 1] = 0x1;
    var_arr[GENERATEX_TWO].data[klen - 1] = 0x2; // 0x2 : GENERATEX_TWO
    var_arr[GENERATEX_THREE].data[klen - 1] = 0x3; // 0x3 : GENERATEX_THREE

    /* x = x2 ^ ((p + 3) // 8) mod p */
    ret = drv_cipher_pke_add_mod(&var_arr[GENERATEX_ECC_P], &var_arr[GENERATEX_THREE], &var_arr[GENERATEX_MAX],
        &var_arr[GENERATEX_P38]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_add_mod failed, ret is 0x%x\n", ret);

    for (i = DRV_PKE_LEN_256 - 1; i > 0; i--) {
        var_arr[GENERATEX_P38].data[i] = ((var_arr[GENERATEX_P38].data[i] & 0xF8) >> 3) | /* shift 3 */
            ((var_arr[GENERATEX_P38].data[i - 1] & 0x07) << 5); /* shift 3 5 */
    }
    var_arr[GENERATEX_P38].data[0] = (var_arr[GENERATEX_P38].data[0] & 0xF8) >> 3; /* shift 3 */
    ret = ed_exp_mod(ecc->p, var_arr[GENERATEX_P38].data, x2, x);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "ed_exp_mod failed, ret is 0x%x\n", ret);
    /* xx = x ^ 2 mod p */
    ret = ed_exp_mod(ecc->p, var_arr[GENERATEX_TWO].data, x, var_arr[GENERATEX_XX].data);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "ed_exp_mod failed, ret is 0x%x\n", ret);
     /* c = xx - x2 mod p */
    ret = drv_cipher_pke_sub_mod(&var_arr[GENERATEX_XX], &var_arr[GENERATEX_X2], &var_arr[GENERATEX_ECC_P],
        &var_arr[GENERATEX_C]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_sub_mod failed, ret is 0x%x\n", ret);
    if (drv_ecc_is_zero(var_arr[GENERATEX_C].data, klen)) {
        ret = TD_SUCCESS;
        goto exit__;
    }
    /* c = xx + x2 mod p */
    ret = drv_cipher_pke_add_mod(&var_arr[GENERATEX_XX], &var_arr[GENERATEX_X2], &var_arr[GENERATEX_ECC_P],
        &var_arr[GENERATEX_C]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_add_mod failed, ret is 0x%x\n", ret);
    if (drv_ecc_is_zero(var_arr[GENERATEX_C].data, klen) != TD_TRUE) {
        ret = TD_FAILURE;
        goto exit__;
    }
    /* s = 2 ^ ((p - 1) / 4) mod p */
    ret = drv_cipher_pke_sub_mod(&var_arr[GENERATEX_ECC_P], &var_arr[GENERATEX_ONE], &var_arr[GENERATEX_ECC_P],
        &var_arr[GENERATEX_P14]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_sub_mod failed, ret is 0x%x\n", ret);
    for (i = DRV_PKE_LEN_256 - 1; i > 0; i--) {
        var_arr[GENERATEX_P14].data[i] = ((var_arr[GENERATEX_P14].data[i] & 0xFC) >> 2) | /* shift 2 */
            ((var_arr[GENERATEX_P14].data[i - 1] & 0x03) << 6); /* shift 2 6 */
    }
    var_arr[GENERATEX_P14].data[0] = (var_arr[GENERATEX_P14].data[0] & 0xFC) >> 2; /* shift 2 */
    ret = ed_exp_mod(ecc->p, var_arr[GENERATEX_P14].data, var_arr[GENERATEX_TWO].data, var_arr[GENERATEX_S].data);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "ed_exp_mod failed, ret is 0x%x\n", ret);
    ret = cal_montgomery_n(&var_arr[GENERATEX_ECC_P], &var_arr[GENERATEX_MONTGOMERY_P]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "cal_montgomery_n failed, ret is 0x%x\n", ret);
    /* x = x * sqrt mod p */
    /* x -> mx */
    ret = drv_cipher_pke_mul_mod(&var_arr[GENERATEX_X], &var_arr[GENERATEX_MONTGOMERY_P], &var_arr[GENERATEX_ECC_P],
        &var_arr[GENERATEX_MX]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);
    /* s -> ms */
    ret = drv_cipher_pke_mul_mod(&var_arr[GENERATEX_S], &var_arr[GENERATEX_MONTGOMERY_P], &var_arr[GENERATEX_ECC_P],
        &var_arr[GENERATEX_MS]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);
    /* mxs = mx * ms mod p */
    ret = drv_cipher_pke_mul_mod(&var_arr[GENERATEX_MX], &var_arr[GENERATEX_MS], &var_arr[GENERATEX_ECC_P],
        &var_arr[GENERATEX_MXS]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);
    /* x' = mxs * 1 mod p */
    ret = drv_cipher_pke_mul_mod(&var_arr[GENERATEX_MXS], &var_arr[GENERATEX_ONE], &var_arr[GENERATEX_ECC_P],
        &var_arr[GENERATEX_X]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);
exit__:
    (void)drv_cipher_pke_clean_ram();
    if (buffer != TD_NULL) {
        (void)memset_s(buffer, klen * GENERATE_X_BUF_CNT, 0, klen * GENERATE_X_BUF_CNT);
        crypto_free(buffer);
    }
    return ret;
}

static td_s32 point_decompress(const drv_pke_ecc_curve *ecc, const drv_pke_data *in, const drv_pke_ecc_point *out)
{
    td_s32 ret = TD_FAILURE;
    td_u32 i;
    td_u32 sign = 0;
    td_u32 klen = DRV_PKE_LEN_256;
    drv_pke_data var_arr[POINT_DECOMPRESS_VAR_MAX];
    td_u8 *buffer;

    buffer = crypto_malloc(klen * POINT_DECOMPRESS_BUF_CNT);
    if (buffer == TD_NULL) {
        crypto_log_err("%s:%d Error! Malloc memory failed!\n", __FUNCTION__, __LINE__);
        return PKE_COMPAT_ERRNO(ERROR_MALLOC);
    }
    (void)memset_s(buffer, klen * POINT_DECOMPRESS_BUF_CNT, 0, klen * POINT_DECOMPRESS_BUF_CNT);
    for (i = 0; i < POINT_DECOMPRESS_VAR_MAX; i++) {
        if (i <= POINTDECOMPRESS_MONE) {
            var_arr[i].data = buffer + klen * i;
        }
        var_arr[i].length = klen;
    }
    var_arr[POINTDECOMPRESS_ECC_P].data = (td_u8 *)ecc->p;
    var_arr[POINTDECOMPRESS_ECC_A].data = (td_u8 *)ecc->a;
    var_arr[POINTDECOMPRESS_OUTX].data = out->x;
    var_arr[POINTDECOMPRESS_OUTY].data = out->y;
    var_arr[POINTDECOMPRESS_ONE].data[klen - 1] = 0x1;

    for (i = 0; i < klen; i++) {
        out->y[i] = in->data[klen - 1 - i];
    }
    sign = out->y[0] >> 7; /* shift 7 */
    out->y[0] &= 0x7F; /* y is recovered by clearing 255 bit */

    ret = cal_montgomery_n(&var_arr[POINTDECOMPRESS_ECC_P], &var_arr[POINTDECOMPRESS_MONTGOMERY_P]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "cal_montgomery_n failed, ret is 0x%x\n", ret);
    /* y -> my */
    ret = drv_cipher_pke_mul_mod(&var_arr[POINTDECOMPRESS_OUTY], &var_arr[POINTDECOMPRESS_MONTGOMERY_P],
        &var_arr[POINTDECOMPRESS_ECC_P], &var_arr[POINTDECOMPRESS_MY]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);
    /* my2 = my * my mod p */
    ret = drv_cipher_pke_mul_mod(&var_arr[POINTDECOMPRESS_MY], &var_arr[POINTDECOMPRESS_MY],
        &var_arr[POINTDECOMPRESS_ECC_P], &var_arr[POINTDECOMPRESS_MY2]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);
    /* 1 -> m1 */
    ret = drv_cipher_pke_mul_mod(&var_arr[POINTDECOMPRESS_ONE], &var_arr[POINTDECOMPRESS_MONTGOMERY_P],
        &var_arr[POINTDECOMPRESS_ECC_P], &var_arr[POINTDECOMPRESS_MONE]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);
    /* my21 = my2 - m1 mod p */
    ret = drv_cipher_pke_sub_mod(&var_arr[POINTDECOMPRESS_MY2], &var_arr[POINTDECOMPRESS_MONE],
        &var_arr[POINTDECOMPRESS_ECC_P], &var_arr[POINTDECOMPRESS_MY21]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);
    /* d -> md */
    ret = drv_cipher_pke_mul_mod(&var_arr[POINTDECOMPRESS_ECC_A], &var_arr[POINTDECOMPRESS_MONTGOMERY_P],
        &var_arr[POINTDECOMPRESS_ECC_P], &var_arr[POINTDECOMPRESS_MD]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);
    /* mdy2 = md * my2 mod p */
    ret = drv_cipher_pke_mul_mod(&var_arr[POINTDECOMPRESS_MD], &var_arr[POINTDECOMPRESS_MY2],
        &var_arr[POINTDECOMPRESS_ECC_P], &var_arr[POINTDECOMPRESS_MDY2]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);
    /* mdy21 = mdy2 + m1 mod p */
    ret = drv_cipher_pke_add_mod(&var_arr[POINTDECOMPRESS_MDY2], &var_arr[POINTDECOMPRESS_MONE],
        &var_arr[POINTDECOMPRESS_ECC_P], &var_arr[POINTDECOMPRESS_MDY21]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_add_mod failed, ret is 0x%x\n", ret);
    /* minv = mdy21 ^ -1 mod p */
    ret = drv_cipher_pke_inv_mod(&var_arr[POINTDECOMPRESS_MDY21], &var_arr[POINTDECOMPRESS_ECC_P],
        &var_arr[POINTDECOMPRESS_MINV]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_inv_mod failed, ret is 0x%x\n", ret);
    /* mx2 = my21 * minv */
    ret = drv_cipher_pke_mul_mod(&var_arr[POINTDECOMPRESS_MY21], &var_arr[POINTDECOMPRESS_MINV],
        &var_arr[POINTDECOMPRESS_ECC_P], &var_arr[POINTDECOMPRESS_MX2]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);
    /* x2 = mx2 * 1 mod p */
    ret = drv_cipher_pke_mul_mod(&var_arr[POINTDECOMPRESS_MX2], &var_arr[POINTDECOMPRESS_ONE],
        &var_arr[POINTDECOMPRESS_ECC_P], &var_arr[POINTDECOMPRESS_X2]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);
    if (drv_ecc_is_zero(var_arr[POINTDECOMPRESS_X2].data, klen)) {
        if (sign == 0) {
            (void)memset_s(out->x, klen, 0x00, klen);
            ret = TD_SUCCESS;
            goto exit__;
        } else {
            ret = TD_FAILURE;
            goto exit__;
        }
    }
    ret = generate_x(ecc, var_arr[POINTDECOMPRESS_X2].data, var_arr[POINTDECOMPRESS_X].data, klen);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "generate_x failed, ret is 0x%x\n", ret);
    if ((var_arr[POINTDECOMPRESS_X].data[klen - 1] & 0x1) != sign) {
        /* x = p - x */
        ret = drv_cipher_pke_sub_mod(&var_arr[POINTDECOMPRESS_ECC_P], &var_arr[POINTDECOMPRESS_X],
            &var_arr[POINTDECOMPRESS_ECC_P], &var_arr[POINTDECOMPRESS_OUTX]);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_sub_mod failed, ret is 0x%x\n", ret);
    } else {
        ret = memcpy_s(out->x, out->length, var_arr[POINTDECOMPRESS_X].data, klen);
        crypto_chk_goto(ret != TD_SUCCESS, exit__, "memcpy_s failed, ret is 0x%x\n", ret);
    }
exit__:
    (void)drv_cipher_pke_clean_ram();
    if (buffer != TD_NULL) {
        (void)memset_s(buffer, klen * POINT_DECOMPRESS_BUF_CNT, 0, klen * POINT_DECOMPRESS_BUF_CNT);
        crypto_free(buffer);
    }
    return ret;
}

static td_s32 pke_eddsa_sign(const drv_pke_ecc_curve *ecc, const drv_pke_data *priv_key,
    const drv_pke_msg *msg, const drv_pke_ecc_sig *sig)
{
    td_s32 ret = TD_FAILURE;
    td_u32 i;
    td_u32 klen = DRV_PKE_LEN_256;
    drv_pke_data var_arr[PKE_EDDSA_SIGN_VAR_MAX];
    td_u8 *buffer;

    buffer = crypto_malloc(klen * EDDSA_SIGN_BUF_CNT);
    crypto_chk_return(buffer == TD_NULL, PKE_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");
    (void)memset_s(buffer, klen * EDDSA_SIGN_BUF_CNT, 0, klen * EDDSA_SIGN_BUF_CNT);

    for (i = 0; i < PKE_EDDSA_SIGN_VAR_MAX - 1; i++) {
        if (i <= EDDSIGN_ONE) {
            var_arr[i].data = buffer + klen * i;
        }
        var_arr[i].length = klen;
    }
    var_arr[EDDSIGN_ECC_N].data = (td_u8 *)ecc->n;
    var_arr[EDDSIGN_MSG].data = msg->data;
    var_arr[EDDSIGN_MSG].length = msg->length;
    var_arr[EDDSIGN_ONE].data[klen - 1] = 0x1;

    /* Operation: a, prefix = secret_expand(secret) */
    ret = secret_expand(priv_key->data, var_arr[EDDSIGN_A].data, var_arr[EDDSIGN_PREFIX].data, klen);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "secret_expand failed, ret is 0x%x\n", ret);

    /* Operation: A = point_compress(point_mul(a, G)) */
    ret = point_mul_compress(ecc, var_arr[EDDSIGN_A].data, var_arr[EDDSIGN_PUBKEY].data, klen);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "point_mul_compress failed, ret is 0x%x\n", ret);

    /* Operation: r = sha512_modq(prefix + msg) */
    ret = sha512_modq(&var_arr[EDDSIGN_ECC_N], TD_NULL, &var_arr[EDDSIGN_PREFIX], &var_arr[EDDSIGN_MSG],
        &var_arr[EDDSIGN_R]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "sha512_modq failed, ret is 0x%x\n", ret);

    /* Operation: Rs = point_compress(point_mul(r, G)) */
    ret = point_mul_compress(ecc, var_arr[EDDSIGN_R].data, var_arr[EDDSIGN_RS].data, klen);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "point_mul_compress failed, ret is 0x%x\n", ret);

    /* Operation: h = sha512_modq(Rs + A + msg) */
    ret = sha512_modq(&var_arr[EDDSIGN_ECC_N], &var_arr[EDDSIGN_RS], &var_arr[EDDSIGN_PUBKEY],
        &var_arr[EDDSIGN_MSG], &var_arr[EDDSIGN_H]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "sha512_modq failed, ret is 0x%x\n", ret);

    ret = cal_montgomery_n(&var_arr[EDDSIGN_ECC_N], &var_arr[EDDSIGN_MONTGOMERY_N]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "cal_montgomery_n failed, ret is 0x%x\n", ret);

    /* Operation: pseudocode is s = (r + h * a) % q; h -> mh */
    ret = drv_cipher_pke_mul_mod(&var_arr[EDDSIGN_H], &var_arr[EDDSIGN_MONTGOMERY_N], &var_arr[EDDSIGN_ECC_N],
        &var_arr[EDDSIGN_MH]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

    /* Operation: a -> ma */
    ret = drv_cipher_pke_mul_mod(&var_arr[EDDSIGN_A], &var_arr[EDDSIGN_MONTGOMERY_N], &var_arr[EDDSIGN_ECC_N],
        &var_arr[EDDSIGN_MA]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

    /* Operation: mha = mh * ma mod q */
    ret = drv_cipher_pke_mul_mod(&var_arr[EDDSIGN_MH], &var_arr[EDDSIGN_MA], &var_arr[EDDSIGN_ECC_N],
        &var_arr[EDDSIGN_MHA]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

    /* Operation: r -> mr */
    ret = drv_cipher_pke_mul_mod(&var_arr[EDDSIGN_R], &var_arr[EDDSIGN_MONTGOMERY_N], &var_arr[EDDSIGN_ECC_N],
        &var_arr[EDDSIGN_MR]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

    /* Operation: ms = mr + mha mod q */
    ret = drv_cipher_pke_add_mod(&var_arr[EDDSIGN_MHA], &var_arr[EDDSIGN_MR], &var_arr[EDDSIGN_ECC_N],
        &var_arr[EDDSIGN_MS]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_add_mod failed, ret is 0x%x\n", ret);

    /* Operation: s = ms * 1 mod q */
    ret = drv_cipher_pke_mul_mod(&var_arr[EDDSIGN_MS], &var_arr[EDDSIGN_ONE], &var_arr[EDDSIGN_ECC_N],
        &var_arr[EDDSIGN_S]);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_mul_mod failed, ret is 0x%x\n", ret);

    /* Operation: return Rs + int.to_bytes(s, 32, "little") */
    ret = memcpy_s(sig->r, sig->length, var_arr[EDDSIGN_RS].data, klen);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "memcpy_s failed, ret is 0x%x\n", ret);
    for (i = 0; i < klen; i++) {
        sig->s[i] = var_arr[EDDSIGN_S].data[klen - 1 - i];
    }
exit__:
    (void)drv_cipher_pke_clean_ram();
    if (buffer != TD_NULL) {
        (void)memset_s(buffer, klen * EDDSA_SIGN_BUF_CNT, 0, klen * EDDSA_SIGN_BUF_CNT);
        crypto_free(buffer);
    }
    return ret;
}

static td_s32 pke_eddsa_verify(const drv_pke_ecc_curve *ecc, const drv_pke_ecc_point *pub_key,
    const drv_pke_msg *msg, const drv_pke_ecc_sig *sig)
{
    td_s32 ret = TD_FAILURE;
    td_u32 klen = DRV_PKE_LEN_256;
    td_u32 i = 0;
    drv_pke_data h_data = {.length = klen};
    drv_pke_data s_data = {.length = klen};
    drv_pke_data pubkey_y_data = {klen, pub_key->y};
    drv_pke_data sig_r_data = {klen, sig->r};
    drv_pke_data ecc_n_data = {klen, (td_u8 *)ecc->n};
    drv_pke_data ecc_p_data = {klen, (td_u8 *)ecc->p};
    drv_pke_data msg_data = {msg->length, msg->data};
    drv_pke_ecc_point ecc_a_point = {.length = klen};
    drv_pke_ecc_point ecc_g_point = {(td_u8 *)ecc->gx, (td_u8 *)ecc->gy, klen};
    drv_pke_ecc_point ecc_r_point = {.length = klen};
    drv_pke_ecc_point ecc_sB_point = {.length = klen};
    drv_pke_ecc_point ecc_hA_point = {.length = klen};
    drv_pke_ecc_point ecc_rhA_point = {.length = klen};
    td_u8 *buffer;

    buffer = crypto_malloc(klen * EDDSA_VERIFY_BUF_CNT);
    crypto_chk_return(buffer == TD_NULL, PKE_COMPAT_ERRNO(ERROR_MALLOC), "crypto_malloc failed\n");
    (void)memset_s(buffer, klen * EDDSA_VERIFY_BUF_CNT, 0, klen * EDDSA_VERIFY_BUF_CNT);

    h_data.data = buffer + klen * i++;
    s_data.data = buffer + klen * i++;
    ecc_a_point.x = buffer + klen * i++;
    ecc_a_point.y = buffer + klen * i++;
    ecc_r_point.x = buffer + klen * i++;
    ecc_r_point.y = buffer + klen * i++;
    ecc_sB_point.x = buffer + klen * i++;
    ecc_sB_point.y = buffer + klen * i++;
    ecc_hA_point.x = buffer + klen * i++;
    ecc_hA_point.y = buffer + klen * i++;
    ecc_rhA_point.x = buffer + klen * i++;
    ecc_rhA_point.y = buffer + klen * i++;
    /* ecc_a = point_decompress(public) */
    ret = point_decompress(ecc, &pubkey_y_data, &ecc_a_point);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "point_mul_compress failed, ret is 0x%x\n", ret);
    /* ecc_rs = signature[:32] */
    /* ecc_r = point_decompress(Rs) */
    ret = point_decompress(ecc, &sig_r_data, &ecc_r_point);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "point_mul_compress failed, ret is 0x%x\n", ret);
    /* s = int.from_bytes(signature[32:], "little") */
    for (i = 0; i < klen; i++) {
        s_data.data[i] = sig->s[klen - 1 - i];
    }
    /* h = sha512_modq(Rs + public + msg) */
    ret = sha512_modq(&ecc_n_data, &sig_r_data, &pubkey_y_data, &msg_data, &h_data);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "sha512_modq failed, ret is 0x%x\n", ret);
     /* ecc_sb = point_mul(ecc_s, ecc_g) */
    ret = drv_cipher_pke_ed_mul_dot(&s_data, &ecc_g_point, &ecc_p_data, &ecc_sB_point);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_ed_mul_dot failed, ret is 0x%x\n", ret);
    /* ecc_ha = point_mul(h, ecc_a) */
    ret = drv_cipher_pke_ed_mul_dot(&h_data, &ecc_a_point, &ecc_p_data, &ecc_hA_point);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_ed_mul_dot failed, ret is 0x%x\n", ret);
    /* return point_equal(ecc_sb, point_add(ecc_r, ecc_ha)) */
    ret = drv_cipher_pke_ed_add_dot(ecc, &ecc_r_point, &ecc_hA_point, &ecc_rhA_point);
    crypto_chk_goto(ret != TD_SUCCESS, exit__, "drv_cipher_pke_ed_add_dot failed, ret is 0x%x\n", ret);

    if ((memcmp(ecc_sB_point.x, ecc_rhA_point.x, klen) != 0) || (memcmp(ecc_sB_point.y, ecc_rhA_point.y, klen) != 0)) {
        ret = TD_FAILURE;
        goto exit__;
    }
exit__:
    (void)drv_cipher_pke_clean_ram();
    if (buffer != TD_NULL) {
        (void)memset_s(buffer, klen * EDDSA_VERIFY_BUF_CNT, 0, klen * EDDSA_VERIFY_BUF_CNT);
        crypto_free(buffer);
    }
    return ret;
}

/* expose layer, do param check here */
td_s32 drv_cipher_pke_ecc_gen_key(
    drv_pke_ecc_curve_type curve_type,
    const drv_pke_data *input_priv_key,
    const drv_pke_data *output_priv_key,
    const drv_pke_ecc_point *output_pub_key)
{
    td_s32 ret = TD_FAILURE;
    td_u32 klen = 0;
    const drv_pke_ecc_curve *ecc_curve = get_ecc_curve(curve_type);
    crypto_chk_return(ecc_curve == TD_NULL, PKE_COMPAT_ERRNO(ERROR_UNSUPPORT), "unsupport alg\n");

    /* check ptr. */
    if (input_priv_key != TD_NULL) {
        pke_null_ptr_chk(input_priv_key->data);
    }
    pke_null_ptr_chk(output_priv_key);
    pke_null_ptr_chk(output_priv_key->data);
    pke_null_ptr_chk(output_pub_key);
    if (curve_type != DRV_PKE_ECC_TYPE_RFC8032) {
        pke_null_ptr_chk(output_pub_key->x);
    }
    if (curve_type != DRV_PKE_ECC_TYPE_RFC7748) {
        pke_null_ptr_chk(output_pub_key->y);
    }

    /* check curve_type. */
    crypto_chk_return(curve_type >= DRV_PKE_ECC_TYPE_MAX, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "curve_type is Invalid\n");

    /* check length. */
    klen = ecc_curve->ksize;
    if (input_priv_key != TD_NULL) {
        crypto_chk_return(input_priv_key->length != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
            "input_priv_key->length is Invalid\n");
    }
    crypto_chk_return(output_priv_key->length != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "output_priv_key->length is Invalid\n");
    crypto_chk_return(output_pub_key->length != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "output_pub_key->length is Invalid\n");

    ret = drv_cipher_pke_lock_secure();
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_lock_secure failed, ret is 0x%x\n", ret);
    ret = pke_ecc_gen_key(ecc_curve, input_priv_key, output_priv_key, output_pub_key);
    (void)drv_cipher_pke_unlock_secure();
    return ret;
}

td_s32 drv_cipher_pke_ecc_gen_ecdh_key(
    drv_pke_ecc_curve_type curve_type,
    const drv_pke_ecc_point *input_pub_key,
    const drv_pke_data *input_priv_key,
    const drv_pke_data *output_shared_key)
{
    td_s32 ret = TD_FAILURE;
    td_u32 klen = 0;
    const drv_pke_ecc_curve *ecc_curve = get_ecc_curve(curve_type);
    crypto_chk_return(curve_type >= DRV_PKE_ECC_TYPE_MAX, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "Invalid curve_type\n");
    crypto_chk_return(ecc_curve == TD_NULL, PKE_COMPAT_ERRNO(ERROR_UNSUPPORT), "unsupport alg\n");

    /* check ptr. */
    pke_null_ptr_chk(input_pub_key);
    pke_null_ptr_chk(input_pub_key->x);
    if (curve_type != DRV_PKE_ECC_TYPE_RFC7748) {
        pke_null_ptr_chk(input_pub_key->y);
    }
    pke_null_ptr_chk(input_priv_key);
    pke_null_ptr_chk(input_priv_key->data);
    pke_null_ptr_chk(output_shared_key);
    pke_null_ptr_chk(output_shared_key->data);

    /* check curve_type. */
    crypto_chk_return(curve_type >= DRV_PKE_ECC_TYPE_MAX || curve_type == DRV_PKE_ECC_TYPE_RFC8032,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "curve_type is Invalid\n");

    /* check length. */
    klen = ecc_curve->ksize;
    crypto_chk_return(input_pub_key->length != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "input_pub_key->length is Invalid\n");
    crypto_chk_return(input_priv_key->length != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "input_priv_key->length is Invalid\n");
    crypto_chk_return(output_shared_key->length != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "output_shared_key->length is Invalid\n");

    ret = drv_cipher_pke_lock_secure();
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_lock_secure failed, ret is 0x%x\n", ret);
    ret = pke_ecc_gen_ecdh_key(ecc_curve, input_pub_key, input_priv_key, output_shared_key);
    (void)drv_cipher_pke_unlock_secure();
    return ret;
}

td_s32 drv_cipher_pke_ecdsa_sign(
    drv_pke_ecc_curve_type curve_type,
    const drv_pke_data *priv_key,
    const drv_pke_data *hash,
    const drv_pke_ecc_sig *sig)
{
    td_s32 ret = TD_FAILURE;
    td_u32 klen = 0;
    td_u32 hash_len = 0;
    const drv_pke_ecc_curve *ecc_curve = get_ecc_curve(curve_type);
    crypto_chk_return(ecc_curve == TD_NULL, PKE_COMPAT_ERRNO(ERROR_UNSUPPORT), "unsupport alg\n");

    /* check ptr. */
    pke_null_ptr_chk(priv_key);
    pke_null_ptr_chk(priv_key->data);
    pke_null_ptr_chk(hash);
    pke_null_ptr_chk(hash->data);
    pke_null_ptr_chk(sig);
    pke_null_ptr_chk(sig->r);
    pke_null_ptr_chk(sig->s);

    /* check curve_type. */
    crypto_chk_return(curve_type >= DRV_PKE_ECC_TYPE_MAX || curve_type == DRV_PKE_ECC_TYPE_RFC8032 ||
        curve_type == DRV_PKE_ECC_TYPE_RFC7748, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "curve_type is Invalid\n");

    /* check length. */
    klen = ecc_curve->ksize;
    hash_len = hash->length;
    if ((priv_key->length != klen) ||
        (hash_len != DRV_PKE_LEN_256 && hash_len != DRV_PKE_LEN_384 && hash_len != DRV_PKE_LEN_512)) {
        ret = PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM);
        crypto_log_err("priv_key->length or hash_len is Invaild!\n");
        return ret;
    }
    crypto_chk_return(sig->length != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "sig->length is Invalid\n");

    ret = drv_cipher_pke_lock_secure();
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_lock_secure failed, ret is 0x%x\n", ret);
    ret = pke_ecdsa_sign(ecc_curve, priv_key, hash, sig);
    (void)drv_cipher_pke_unlock_secure();
    return ret;
}

td_s32 drv_cipher_pke_ecdsa_verify(
    drv_pke_ecc_curve_type curve_type,
    const drv_pke_ecc_point *pub_key,
    const drv_pke_data *hash,
    const drv_pke_ecc_sig *sig)
{
    td_s32 ret = TD_FAILURE;
    td_u32 klen = 0;
    td_u32 hash_len = 0;
    const drv_pke_ecc_curve *ecc_curve = get_ecc_curve(curve_type);
    crypto_chk_return(ecc_curve == TD_NULL, PKE_COMPAT_ERRNO(ERROR_UNSUPPORT), "unsupport alg\n");

    /* check ptr. */
    pke_null_ptr_chk(pub_key);
    pke_null_ptr_chk(pub_key->x);
    pke_null_ptr_chk(pub_key->y);
    pke_null_ptr_chk(hash);
    pke_null_ptr_chk(hash->data);
    pke_null_ptr_chk(sig);
    pke_null_ptr_chk(sig->r);
    pke_null_ptr_chk(sig->s);

    /* check curve_type. */
    crypto_chk_return(curve_type >= DRV_PKE_ECC_TYPE_MAX || curve_type == DRV_PKE_ECC_TYPE_RFC8032 ||
        curve_type == DRV_PKE_ECC_TYPE_RFC7748, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "curve_type is Invalid\n");

    /* check length. */
    klen = ecc_curve->ksize;
    hash_len = hash->length;
    if ((pub_key->length != klen) ||
        (hash_len != DRV_PKE_LEN_256 && hash_len != DRV_PKE_LEN_384 && hash_len != DRV_PKE_LEN_512)) {
        ret = PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM);
        crypto_log_err("pub_key->length or hash_len is Invaild!\n");
        return ret;
    }
    crypto_chk_return(sig->length != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "sig->length is Invalid\n");

    ret = drv_cipher_pke_lock_secure();
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_lock_secure failed, ret is 0x%x\n", ret);
    ret = pke_ecdsa_verify(ecc_curve, pub_key, hash, sig);
    (void)drv_cipher_pke_unlock_secure();
    return ret;
}

td_s32 drv_cipher_pke_eddsa_sign(
    drv_pke_ecc_curve_type curve_type,
    const drv_pke_data *priv_key,
    const drv_pke_msg *msg,
    const drv_pke_ecc_sig *sig)
{
    td_s32 ret = TD_FAILURE;
    td_u32 klen = 0;
    const drv_pke_ecc_curve *ecc_curve = get_ecc_curve(curve_type);
    crypto_chk_return(ecc_curve == TD_NULL, PKE_COMPAT_ERRNO(ERROR_UNSUPPORT), "unsupport alg\n");

    /* check ptr. */
    pke_null_ptr_chk(priv_key);
    pke_null_ptr_chk(priv_key->data);
    pke_null_ptr_chk(msg);
    pke_null_ptr_chk(msg->data);
    pke_null_ptr_chk(sig);
    pke_null_ptr_chk(sig->r);
    pke_null_ptr_chk(sig->s);

    /* check curve_type. */
    crypto_chk_return(curve_type != DRV_PKE_ECC_TYPE_RFC8032, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "curve_type is Invalid\n");

    /* check length. */
    klen = ecc_curve->ksize;
    crypto_chk_return(priv_key->length != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "priv_key->length is Invalid\n");
    crypto_chk_return(sig->length != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "sig->length is Invalid\n");

    ret = drv_cipher_pke_lock_secure();
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_lock_secure failed, ret is 0x%x\n", ret);
    ret = pke_eddsa_sign(ecc_curve, priv_key, msg, sig);
    (void)drv_cipher_pke_unlock_secure();
    return ret;
}

td_s32 drv_cipher_pke_eddsa_verify(
    drv_pke_ecc_curve_type curve_type,
    const drv_pke_ecc_point *pub_key,
    const drv_pke_msg *msg,
    const drv_pke_ecc_sig *sig)
{
    td_s32 ret = TD_FAILURE;
    td_u32 klen = 0;
    const drv_pke_ecc_curve *ecc_curve = get_ecc_curve(curve_type);
    crypto_chk_return(ecc_curve == TD_NULL, PKE_COMPAT_ERRNO(ERROR_UNSUPPORT), "unsupport alg\n");

    /* check ptr. */
    pke_null_ptr_chk(pub_key);
    pke_null_ptr_chk(pub_key->y);
    pke_null_ptr_chk(msg);
    pke_null_ptr_chk(msg->data);
    pke_null_ptr_chk(sig);
    pke_null_ptr_chk(sig->r);
    pke_null_ptr_chk(sig->s);

    /* check curve_type. */
    crypto_chk_return(curve_type != DRV_PKE_ECC_TYPE_RFC8032, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "curve_type is Invalid\n");

    /* check length. */
    klen = ecc_curve->ksize;
    crypto_chk_return(pub_key->length != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "pub_key->length is Invalid\n");
    crypto_chk_return(sig->length != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "sig->length is Invalid\n");

    ret = drv_cipher_pke_lock_secure();
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_lock_secure failed, ret is 0x%x\n", ret);
    ret = pke_eddsa_verify(ecc_curve, pub_key, msg, sig);
    (void)drv_cipher_pke_unlock_secure();
    return ret;
}

td_s32 drv_cipher_pke_sm2_public_encrypt(
    const drv_pke_ecc_point *pub_key,
    const drv_pke_data *plain_text,
    drv_pke_data *cipher_text)
{
    td_s32 ret = TD_FAILURE;
    td_u32 plain_text_len = 0;
    td_u32 cipher_text_len = 0;

    /* check ptr. */
    pke_null_ptr_chk(pub_key);
    pke_null_ptr_chk(pub_key->x);
    pke_null_ptr_chk(pub_key->y);
    pke_null_ptr_chk(plain_text);
    pke_null_ptr_chk(plain_text->data);
    pke_null_ptr_chk(cipher_text);
    pke_null_ptr_chk(cipher_text->data);

    /* check length. */
    plain_text_len = plain_text->length;
    cipher_text_len = cipher_text->length;
    crypto_chk_return(pub_key->length != DRV_PKE_LEN_256, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "pub_key->length is Invalid\n");
    /* For SM2 Crypto, the cipher_text is 97 longer than plain_text. */
    crypto_chk_return(cipher_text_len < plain_text_len + SM2_CRYPTO_ADD_LENGTH_IN_BYTE,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "cipher_text_len is not enough\n");

    ret = drv_cipher_pke_lock_secure();
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_lock_secure failed, ret is 0x%x\n", ret);
    ret = pke_sm2_public_encrypt(pub_key, plain_text, cipher_text);
    if (ret == TD_SUCCESS) {
        cipher_text->length = plain_text->length + SM2_CRYPTO_ADD_LENGTH_IN_BYTE;
    }
    (void)drv_cipher_pke_unlock_secure();
    return ret;
}

td_s32 drv_cipher_pke_sm2_private_decrypt(
    const drv_pke_data *priv_key,
    const drv_pke_data *cipher_text,
    drv_pke_data *plain_text)
{
    td_s32 ret = TD_FAILURE;
    td_u32 plain_text_len = 0;
    td_u32 cipher_text_len = 0;

    /* check ptr. */
    pke_null_ptr_chk(priv_key);
    pke_null_ptr_chk(priv_key->data);
    pke_null_ptr_chk(cipher_text);
    pke_null_ptr_chk(cipher_text->data);
    pke_null_ptr_chk(plain_text);
    pke_null_ptr_chk(plain_text->data);

    /* check length. */
    plain_text_len = plain_text->length;
    cipher_text_len = cipher_text->length;
    crypto_chk_return(priv_key->length != DRV_PKE_LEN_256, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "priv_key->length is Invalid\n");
    /* For SM2 Crypto, the cipher_text is 97 longer than plain_text. */
    crypto_chk_return(plain_text_len > CRYPTO_PKE_SM2_PLAIN_TEXT_MAX_SIZE ||
        plain_text_len + SM2_CRYPTO_ADD_LENGTH_IN_BYTE < cipher_text_len,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "plain_text_len is not enough\n");

    ret = drv_cipher_pke_lock_secure();
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_lock_secure failed, ret is 0x%x\n", ret);
    ret = pke_sm2_private_decrypt(priv_key, cipher_text, plain_text);
    if (ret == TD_SUCCESS) {
        plain_text->length = cipher_text_len - SM2_CRYPTO_ADD_LENGTH_IN_BYTE;
    }
    (void)drv_cipher_pke_unlock_secure();
    return ret;
}

td_s32 drv_cipher_pke_check_dot_on_curve(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *pub_key,
    td_bool *is_on_curve)
{
    td_s32 ret = TD_FAILURE;
    td_u32 klen = 0;
    const drv_pke_ecc_curve *ecc_curve = get_ecc_curve(curve_type);
    crypto_chk_return(ecc_curve == TD_NULL, PKE_COMPAT_ERRNO(ERROR_UNSUPPORT), "unsupport alg\n");

    /* check ptr. */
    pke_null_ptr_chk(pub_key);
    pke_null_ptr_chk(pub_key->x);
    pke_null_ptr_chk(pub_key->y);
    pke_null_ptr_chk(is_on_curve);

    /* check curve_type. */
    crypto_chk_return(curve_type == DRV_PKE_ECC_TYPE_RFC7748 || curve_type == DRV_PKE_ECC_TYPE_RFC8032,
        PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM), "curve_type is Invalid\n");

    /* check length. */
    klen = ecc_curve->ksize;
    crypto_chk_return(pub_key->length != klen, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "pub_key->length is Invalid\n");

    ret = drv_cipher_pke_lock_secure();
    crypto_chk_return(ret != TD_SUCCESS, ret, "drv_cipher_pke_lock_secure failed, ret is 0x%x\n", ret);
    ret = pke_check_dot_on_curve(ecc_curve, pub_key, is_on_curve);
    (void)drv_cipher_pke_unlock_secure();
    return ret;
}

td_s32 drv_cipher_pke_sm2_dsa_hash(const drv_pke_data *sm2_id, const drv_pke_ecc_point *pub_key,
    const drv_pke_msg *msg, drv_pke_data *hash)
{
    td_s32 ret = TD_FAILURE;
    sm2_sign_verify_hash_pack hash_param;
    drv_pke_data hash_data;

    /* check ptr. */
    pke_null_ptr_chk(sm2_id);
    pke_null_ptr_chk(sm2_id->data);
    pke_null_ptr_chk(pub_key);
    pke_null_ptr_chk(pub_key->x);
    pke_null_ptr_chk(pub_key->y);
    pke_null_ptr_chk(msg);
    pke_null_ptr_chk(msg->data);
    pke_null_ptr_chk(hash);
    pke_null_ptr_chk(hash->data);

    /* check length. */
    crypto_chk_return(pub_key->length != DRV_PKE_SM2_LEN_IN_BYTES, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "pub_key->length is Invalid\n");
    crypto_chk_return(hash->length != DRV_PKE_SM2_LEN_IN_BYTES, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "hash_len is Invalid\n");
    crypto_chk_return(sm2_id->length > PKE_U16_MAX / BYTE_BITS, PKE_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "sm2_id_len is Invalid\n");

    hash_param.px = pub_key->x;
    hash_param.py = pub_key->y;
    hash_param.id = sm2_id->data;
    hash_param.id_len = sm2_id->length;
    hash_data.data = hash->data;
    hash_data.length = hash->length;

    /* e = Hash */
    ret = sm2_sign_verify_hash(&hash_param, msg, &hash_data);
    crypto_chk_return(ret != TD_SUCCESS, ret, "sm2_sign_verify_hash failed, ret is 0x%x\n", ret);
    return TD_SUCCESS;
}
