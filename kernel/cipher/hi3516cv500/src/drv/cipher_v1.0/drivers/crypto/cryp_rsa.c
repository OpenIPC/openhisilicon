/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : driver for cryp rsa.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#include "cryp_rsa.h"
#include "drv_osal_lib.h"
#include "drv_srsa.h"
#include "cryp_trng.h"
#include "mbedtls/rsa.h"
#include "mbedtls/bignum.h"

/* Internal Structure Definition */
#define RSA_PKCS1_TYPE_MIN_PAD_LEN               11

/* rsa padding value 0xff. */
#define RSA_PADDING_VAL_FF                       0xFF

/* rsa key len in bits */
#define RSA_BITS_1024                            1024
#define RSA_BITS_2048                            2048
#define RSA_BITS_3072                            3072
#define RSA_BITS_4096                            4096

/* rsa mutex */
static crypto_mutex g_rsa_mutex;
static hi_u32 g_rsa_key_ca_type = HI_CIPHER_KEY_SRC_USER;

#define kapi_rsa_lock_err_return()   \
    do { \
        ret = crypto_mutex_lock(&g_rsa_mutex);  \
        if (ret != HI_SUCCESS) { \
            hi_log_error("error, rsa lock failed\n"); \
            return ret; \
        } \
    } while (0)

#define kapi_rsa_unlock()   crypto_mutex_unlock(&g_rsa_mutex)

/* API Code for cryp rsa */
hi_void mbedtls_mpi_print(const mbedtls_mpi *x, const char *name)
{
#ifdef CIPHER_DEBUG_SUPPORT
    int ret;
    size_t n;
    hi_u8 buf[RSA_KEY_WIDTH_4096] = {0};

    n = mbedtls_mpi_size(x);
    MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(x, buf, n));
    HI_PRINT_HEX(name, (hi_u8 *)buf, n);

cleanup:
    return;
#endif
}

#ifdef CHIP_RSA_SUPPORT
static hi_s32 rsa_get_klen(unsigned long module_len, hi_u32 *keylen, rsa_key_width *width)
{
    if (module_len <= RSA_KEY_LEN_1024) {
        *keylen = RSA_KEY_LEN_1024;
        *width = RSA_KEY_WIDTH_1024;
    } else if (module_len <= RSA_KEY_LEN_2048) {
        *keylen = RSA_KEY_LEN_2048;
        *width = RSA_KEY_WIDTH_2048;
    } else if (module_len <= RSA_KEY_LEN_4096) {
        *keylen = RSA_KEY_LEN_4096;
        *width = RSA_KEY_WIDTH_4096;
    } else {
        hi_log_error("error, invalid key len %ld\n", module_len);
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    return HI_SUCCESS;
}

static hi_s32 cryp_check_data(hi_u8 *n, hi_u8 *e, hi_u8 *mc, hi_u32 len)
{
    hi_u32 i;

    /* MC > 0 */
    for (i = 0; i < len; i++) {
        if (mc[i] > 0) {
            break;
        }
    }
    if (i >= len) {
        hi_log_error("RSA M/C is zero, error!\n");
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    /* MC < N */
    for (i = 0; i < len; i++) {
        if (mc[i] < n[i]) {
            break;
        }
    }
    if (i >= len) {
        hi_log_error("RSA M/C is larger than N, error!\n");
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    /* E >= 1 */
    for (i = 0; i < len; i++) {
        if (e[i] > 0) {
            break;
        }
    }
    if (i >= len) {
        hi_log_error("RSA D/E is zero, error!\n");
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    return HI_SUCCESS;
}

static int cryp_ifep_rsa_exp_mod(hi_u32 ca_type, mbedtls_mpi *x, const mbedtls_mpi *a,
                                 const mbedtls_mpi *e, const mbedtls_mpi *n, mbedtls_mpi *rr)
{
    hi_u32 module_len;
    hi_u8 *buf = HI_NULL;
    hi_u8 *tmp_n = HI_NULL;
    hi_u8 *k = HI_NULL;
    hi_u8 *in = HI_NULL;
    hi_u8 *out = HI_NULL;
    hi_u32 keylen = 0;
    rsa_key_width width = 0;
    mbedtls_mpi tmp_a;
    hi_s32 ret;

    hi_log_func_enter();

    /* computes valid bits of N */
    module_len = crypto_max(mbedtls_mpi_size(n), mbedtls_mpi_size(e));

    ret = rsa_get_klen(module_len, &keylen, &width);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(rsa_get_klen, ret);
        return ret;
    }

    /* mallc buf to store n || k(e or d) || in || out */
    buf = crypto_calloc(MUL_VAL_4, keylen);
    if (buf == HI_NULL) {
        hi_log_print_func_err(crypto_calloc, HI_ERR_CIPHER_FAILED_MEM);
        return HI_ERR_CIPHER_FAILED_MEM;
    }

    tmp_n = buf;
    k = tmp_n + keylen;
    in = k + keylen;
    out = in + keylen;

    mbedtls_mpi_init(&tmp_a);
    crypto_chk_err_exit(mbedtls_mpi_mod_mpi(&tmp_a, a, n));

    /* read A, E, N */
    crypto_chk_err_exit(mbedtls_mpi_write_binary(&tmp_a, in, keylen));
    crypto_chk_err_exit(mbedtls_mpi_write_binary(e, k, keylen));
    crypto_chk_err_exit(mbedtls_mpi_write_binary(n, tmp_n, keylen));

    /* key and data valid ? */
    crypto_chk_err_exit(cryp_check_data(tmp_n, k, in, keylen));

    /* out = in ^ k mod n */
    ret = drv_ifep_rsa_exp_mod(ca_type, tmp_n, k, in, out, width);
    if (ret == HI_SUCCESS) {
        /* write d */
        mbedtls_mpi_read_binary(x, out, keylen);
    }

exit__:

    mbedtls_mpi_free(&tmp_a);
    crypto_free(buf);
    buf = HI_NULL;

    hi_log_func_exit();

    return ret;
}
#endif

int mbedtls_mpi_exp_mod(mbedtls_mpi *x, const mbedtls_mpi *a, const mbedtls_mpi *e, const mbedtls_mpi *n,
    mbedtls_mpi *rr)
{
    hi_s32 ret;
#if defined(CHIP_IFEP_RSA_VER_V100)
    hi_u32 elen;
#endif

    hi_log_func_enter();

    hi_log_chk_param_return(x == HI_NULL);
    hi_log_chk_param_return(a == HI_NULL);
    hi_log_chk_param_return(e == HI_NULL);
    hi_log_chk_param_return(n == HI_NULL);

    mbedtls_mpi_print(a, "M");
    mbedtls_mpi_print(e, "E");
    mbedtls_mpi_print(n, "N");

#if defined(CHIP_IFEP_RSA_VER_V100)
    elen = mbedtls_mpi_size(e);
    if (elen <= RSA_KEY_LEN_4096) {
        /* The private key may be not from user when generate rsa key pare
         * in this case use klad key will failed.
         */
        ret = cryp_ifep_rsa_exp_mod(g_rsa_key_ca_type, x, a, e, n, rr);
    } else {
        if (g_rsa_key_ca_type != HI_CIPHER_KEY_SRC_USER) {
            hi_log_error("sofrware rsa nonsupport klad key\n");
            return HI_ERR_CIPHER_ILLEGAL_KEY;
        }

        ret = mbedtls_mpi_exp_mod_sw(x, a, e, n, rr);
    }
#else
    if (g_rsa_key_ca_type != HI_CIPHER_KEY_SRC_USER) {
        hi_log_error("sofrware rsa nonsupport klad key\n");
        return HI_ERR_CIPHER_ILLEGAL_KEY;
    }

    ret = mbedtls_mpi_exp_mod_sw(x, a, e, n, rr);
#endif
    mbedtls_mpi_print(x, "X");

    if (ret != HI_SUCCESS) {
        hi_log_error("rsa mpi_exp_mod failed, ret = 0x%x\n", ret);
        return ret;
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

int cryp_rsa_init(hi_void)
{
    hi_log_func_enter();

    crypto_mutex_init(&g_rsa_mutex);

#if defined(CHIP_IFEP_RSA_VER_V100)
    {
        hi_s32 ret;

        ret = drv_rsa_init();
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(drv_rsa_init, ret);
            return ret;
        }
    }
#endif

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_void cryp_rsa_deinit(hi_void)
{
#if defined(CHIP_IFEP_RSA_VER_V100)
    rsa_capacity capacity;

    drv_ifep_rsa_get_capacity(&capacity);

    /* recovery the rsa function of mbedtls */
    if (capacity.rsa) {
        drv_rsa_deinit();
    }
#endif

    crypto_mutex_destroy(&g_rsa_mutex);
}

int mbedtls_get_random(hi_void *param, hi_u8 *rand, size_t size)
{
    hi_u32 i;
    hi_u32 randnum, left_size;

    for (i = 0; i < size; i += WORD_WIDTH) {
        cryp_trng_get_random(&randnum, CRYP_TRNG_TIMEOUT);

        left_size = (size - i) > WORD_WIDTH ? WORD_WIDTH : (size - i);
        switch (left_size) {
            case WORD_IDX_4:
                rand[i + WORD_IDX_3] = (hi_u8)(randnum >> SHIFT_24BITS) & MAX_LOW_8BITS;
                /* fall through */
            case WORD_IDX_3:
                rand[i + WORD_IDX_2] = (hi_u8)(randnum >> SHIFT_16BITS) & MAX_LOW_8BITS;
                /* fall through */
            case WORD_IDX_2:
                rand[i + WORD_IDX_1] = (hi_u8)(randnum >> SHIFT_8BITS) & MAX_LOW_8BITS;
                /* fall through */
            case WORD_IDX_1:
                rand[i + WORD_IDX_0] = (hi_u8)(randnum) & MAX_LOW_8BITS;
                break;
            default:
                hi_log_error("left size %u is error\n", left_size);
                return HI_ERR_CIPHER_INVALID_LEN;
        }
    }

    return HI_SUCCESS;
}

static hi_s32 cryp_rsa_init_key(cryp_rsa_key *key, hi_u32 *mode, mbedtls_rsa_context *rsa)
{
    hi_s32 ret;

    hi_log_func_enter();

    mbedtls_mpi_init(&rsa->N);
    mbedtls_mpi_init(&rsa->E);
    mbedtls_mpi_init(&rsa->D);
    mbedtls_mpi_init(&rsa->P);
    mbedtls_mpi_init(&rsa->Q);
    mbedtls_mpi_init(&rsa->DP);
    mbedtls_mpi_init(&rsa->DQ);
    mbedtls_mpi_init(&rsa->QP);

    crypto_chk_err_exit(mbedtls_mpi_read_binary(&rsa->N, key->n, key->klen));
    rsa->len = key->klen;
    if ((rsa->len < RSA_MIN_KEY_LEN) || (rsa->len > RSA_MAX_KEY_LEN)) {
        hi_log_error("RSA invalid keylen: 0x%x!\n", (hi_u32)rsa->len);
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        ret = HI_ERR_CIPHER_INVALID_PARAM;
        goto exit__;
    }

    if (key->public) {
        crypto_chk_err_exit(mbedtls_mpi_read_binary(&rsa->E, (hi_u8 *)&key->e, WORD_WIDTH));
        *mode = MBEDTLS_RSA_PUBLIC;
    } else {
        if (key->d != HI_NULL) { /* Non CRT */
            crypto_chk_err_exit(mbedtls_mpi_read_binary(&rsa->D, key->d, key->klen));
            *mode = MBEDTLS_RSA_PRIVATE;
        } else { /* CRT */
            crypto_chk_err_exit(mbedtls_mpi_read_binary(&rsa->P, key->p, key->klen / MUL_VAL_2));
            crypto_chk_err_exit(mbedtls_mpi_read_binary(&rsa->Q, key->q, key->klen / MUL_VAL_2));
            crypto_chk_err_exit(mbedtls_mpi_read_binary(&rsa->DP, key->dp, key->klen / MUL_VAL_2));
            crypto_chk_err_exit(mbedtls_mpi_read_binary(&rsa->DQ, key->dq, key->klen / MUL_VAL_2));
            crypto_chk_err_exit(mbedtls_mpi_read_binary(&rsa->QP, key->qp, key->klen / MUL_VAL_2));
            *mode = MBEDTLS_RSA_PRIVATE;
        }
    }

    g_rsa_key_ca_type = key->ca_type;

    hi_log_debug("mode %d, e 0x%x\n", *mode, key->e);

    hi_log_func_exit();
    return HI_SUCCESS;

exit__:

    mbedtls_mpi_free(&rsa->N);
    mbedtls_mpi_free(&rsa->E);
    mbedtls_mpi_free(&rsa->D);
    mbedtls_mpi_free(&rsa->P);
    mbedtls_mpi_free(&rsa->Q);
    mbedtls_mpi_free(&rsa->DP);
    mbedtls_mpi_free(&rsa->DQ);
    mbedtls_mpi_free(&rsa->QP);

    return ret;
}

static hi_void cryp_rsa_deinit_key(mbedtls_rsa_context *rsa)
{
    hi_log_func_enter();

    mbedtls_mpi_free(&rsa->N);
    mbedtls_mpi_free(&rsa->E);
    mbedtls_mpi_free(&rsa->D);
    mbedtls_mpi_free(&rsa->P);
    mbedtls_mpi_free(&rsa->Q);
    mbedtls_mpi_free(&rsa->DP);
    mbedtls_mpi_free(&rsa->DQ);
    mbedtls_mpi_free(&rsa->QP);

    hi_log_func_exit();
}

static hi_s32 cryp_rsa_get_alg(hi_u32 scheme, int *padding, int *hash_id, int *hashlen)
{
    switch (scheme) {
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_NO_PADDING:
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_BLOCK_TYPE_0:
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_BLOCK_TYPE_1:
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_BLOCK_TYPE_2:
            *padding = 0x00;
            *hash_id = 0;
            *hashlen = 0;
            break;
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_PKCS1_V1_5:
            *padding = MBEDTLS_RSA_PKCS_V15;
            *hash_id = 0;
            *hashlen = 0;
            break;
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA1:
            *padding = MBEDTLS_RSA_PKCS_V15;
            *hash_id = MBEDTLS_MD_SHA1;
            *hashlen = SHA1_RESULT_SIZE;
            break;
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA224:
            *padding = MBEDTLS_RSA_PKCS_V15;
            *hash_id = MBEDTLS_MD_SHA224;
            *hashlen = SHA224_RESULT_SIZE;
            break;
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA256:
            *padding = MBEDTLS_RSA_PKCS_V15;
            *hash_id = MBEDTLS_MD_SHA256;
            *hashlen = SHA256_RESULT_SIZE;
            break;
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA384:
            *padding = MBEDTLS_RSA_PKCS_V15;
            *hash_id = MBEDTLS_MD_SHA384;
            *hashlen = SHA384_RESULT_SIZE;
            break;
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA512:
            *padding = MBEDTLS_RSA_PKCS_V15;
            *hash_id = MBEDTLS_MD_SHA512;
            *hashlen = SHA512_RESULT_SIZE;
            break;
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA1:
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA1:
            *padding = MBEDTLS_RSA_PKCS_V21;
            *hash_id = MBEDTLS_MD_SHA1;
            *hashlen = SHA1_RESULT_SIZE;
            break;
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA224:
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA224:
            *padding = MBEDTLS_RSA_PKCS_V21;
            *hash_id = MBEDTLS_MD_SHA224;
            *hashlen = SHA224_RESULT_SIZE;
            break;
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA256:
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA256:
            *padding = MBEDTLS_RSA_PKCS_V21;
            *hash_id = MBEDTLS_MD_SHA256;
            *hashlen = SHA256_RESULT_SIZE;
            break;
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA384:
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA384:
            *padding = MBEDTLS_RSA_PKCS_V21;
            *hash_id = MBEDTLS_MD_SHA384;
            *hashlen = SHA384_RESULT_SIZE;
            break;
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA512:
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA512:
            *padding = MBEDTLS_RSA_PKCS_V21;
            *hash_id = MBEDTLS_MD_SHA512;
            *hashlen = SHA512_RESULT_SIZE;
            break;
        default:
            hi_log_error("RSA padding mode error, mode = 0x%x.\n", scheme);
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
            return HI_ERR_CIPHER_INVALID_PARAM;
    }

    hi_log_debug("padding %d, hash_id %d, hashlen %d\n", *padding, *hash_id, *hashlen);
    return HI_SUCCESS;
}

/*
 * PKCS #1: block type 0, 1, 2 message padding.
 * description: EB = 00 || BT || PS || 00 || D
 * description: PS_LEN >= 8, mlen < key_len - 11
 */
static hi_s32 ext_rsa_calc(mbedtls_rsa_context *rsa, hi_u32 mode, const hi_u8 *in, hi_u8 *out, hi_u32 len)
{
    hi_s32 ret;

    crypto_unused(len);

    if (mode == MBEDTLS_RSA_PUBLIC) {
        ret = mbedtls_rsa_public(rsa, in, out);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(mbedtls_rsa_public, ret);
            return HI_ERR_CIPHER_RSA_CRYPT_FAILED;
        }
    } else {
        ret = mbedtls_rsa_private(rsa, HI_NULL, 0, in, out);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(mbedtls_rsa_private, ret);
            return HI_ERR_CIPHER_RSA_CRYPT_FAILED;
        }
    }

    return HI_SUCCESS;
}

static hi_s32 rsa_pkcs1_block_padding(hi_u8 bt, hi_u8 *buf, hi_u32 buf_len)
{
    switch (bt) {
        case RSA_BLOCK_YTPE_0:
            /* For block type 00, the octets shall have value 0x00 */
            crypto_memset(buf, buf_len, 0x00, buf_len);
            break;
        case RSA_BLOCK_YTPE_1:
            /* for block type 01, they shall have value 0xFF */
            crypto_memset(buf, buf_len, RSA_PADDING_VAL_FF, buf_len);
            break;
        case RSA_BLOCK_YTPE_2: {
            hi_s32 i;

            /* for block type 02, they shall be pseudorandomly generated and nonzero. */
            (hi_void)mbedtls_get_random(HI_NULL, buf, buf_len);

            /* make sure nonzero */
            for (i = 0; i < buf_len; i++) {
                if (buf[i] == 0) {
                    buf[i] = CRYPTO_NUM_1;
                }
            }
            break;
        }
        default: {
            hi_log_error("BT(0x%x) is invalid.\n", buf_len);
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
            return HI_ERR_CIPHER_INVALID_PARAM;
        }
    }

    return HI_SUCCESS;
}

static hi_s32 rsa_padding_add_pkcs1_type(mbedtls_rsa_context *rsa, rsa_padding_pack *pad)
{
    hi_s32 ret;
    hi_u32 plen;
    hi_u8 *peb = HI_NULL;

    hi_log_func_enter();

    crypto_unused(pad->out_len);

    if (pad->in_len > pad->klen - RSA_PKCS1_TYPE_MIN_PAD_LEN) {
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    peb = pad->out;

    /* first byte is 0x00 */
    *(peb++) = 0;

    /* Private Key BT (Block Type) */
    *(peb++) = pad->bt;

    /*
     * The padding string PS shall consist of k-3-||D|| octets, 3 bytes is used by 0x00||BT||...||0x00, the last 0
     * follow PS.
     */
    plen = pad->klen - CRYPTO_NUM_3 - pad->in_len;

    ret = rsa_pkcs1_block_padding(pad->bt, peb, plen);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(rsa_pkcs1_block_padding, ret);
        return ret;
    }

    /* skip the padding string */
    peb += plen;

    /* set 0x00 follow PS */
    *(peb++) = 0x00;

    /* input data */
    crypto_memcpy(peb, pad->in_len, pad->in, pad->in_len);

    ret = ext_rsa_calc(rsa, pad->mode, pad->out, pad->out, *pad->out_len);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(ext_rsa_calc, ret);
        return ret;
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

/*
 * PKCS #1: block type 0, 1, 2 message padding.
 * description: EB = 00 || BT || PS || 00 || D.
 * description: PS_LEN >= 8, mlen < key_len - 11.
 */
static hi_s32 rsa_pkcs1_chk_block_padding(hi_u8 *peb, hi_u32 *idx, rsa_padding_pack *pad)
{
    hi_u32 offset = *idx;

    switch (pad->bt) {
        case RSA_BLOCK_YTPE_0:
            /* For block type 00, the octets shall have value 00 */
            for (; offset < pad->in_len - 1; offset++) {
                if ((peb[offset] == 0x00) && (peb[offset + 1] != 0)) {
                    break;
                }
            }
            break;
        case RSA_BLOCK_YTPE_1:
            /* For block type 0x01 the octets shall have value 0xFF */
            for (; offset < pad->in_len - 1; offset++) {
                if (peb[offset] == 0xFF) {
                    continue;
                } else if (peb[offset] == 0x00) {
                    break;
                } else {
                    offset = pad->in_len - 1;
                    break;
                }
            }
            break;
        case RSA_BLOCK_YTPE_2:
            /* for block type 02, they shall be pseudorandomly generated and nonzero. */
            for (; offset < pad->in_len - 1; offset++) {
                if (peb[offset] == 0) {
                    break;
                }
            }
            break;
        default:
            hi_log_error("BT(0x%x) is invalid.\n", pad->bt);
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
            return HI_ERR_CIPHER_INVALID_PARAM;
    }

    if (offset >= pad->in_len - 1) {
        hi_log_error("PS Error.\n");
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    *idx = offset;
    return HI_SUCCESS;
}

static hi_s32 rsa_padding_check_pkcs1_type(mbedtls_rsa_context *rsa, rsa_padding_pack *pad)
{
    hi_s32 ret;
    hi_u8 *peb = HI_NULL;
    hi_u32 idx;

    hi_log_func_enter();

    ret = ext_rsa_calc(rsa, pad->mode, pad->in, pad->in, pad->in_len);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(ext_rsa_calc, ret);
        return ret;
    }

    *(pad->out_len) = 0x00;
    peb = pad->in;
    idx = 0;

    /* first byte must be 0x00 */
    if (peb[idx] != 0x00) {
        hi_log_error("EB[0] != 0x00.\n");
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }
    idx++;

    /* Private Key BT (Block Type) */
    if (peb[idx] != pad->bt) {
        hi_log_error("EB[1] != BT(0x%x).\n", pad->bt);
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }
    idx++;

    /* Block Type padding. */
    ret = rsa_pkcs1_chk_block_padding(peb, &idx, pad);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(rsa_pkcs1_chk_block_padding, ret);
        return ret;
    }

    /* skip 0x00 after PS */
    idx++;

    /* get payload data */
    *(pad->out_len) = pad->klen - idx;
    crypto_memcpy(pad->out, pad->klen, &peb[idx], *(pad->out_len));

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 rsa_no_padding(mbedtls_rsa_context *rsa, rsa_padding_pack *no_pad)
{
    hi_s32 ret;
    hi_u8 *data = HI_NULL;

    hi_log_func_enter();

    if (no_pad->in_len > no_pad->klen) {
        hi_log_error("input length %d invalid.\n", no_pad->in_len);
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    /* mallc data buf */
    data = crypto_malloc(RSA_MAX_KEY_LEN);
    if (data == HI_NULL) {
        hi_log_print_func_err(crypto_malloc, HI_ERR_CIPHER_FAILED_MEM);
        return HI_ERR_CIPHER_FAILED_MEM;
    }

    crypto_memset(data, RSA_MAX_KEY_LEN, 0, RSA_MAX_KEY_LEN);
    crypto_memcpy(data + no_pad->klen - no_pad->in_len, RSA_MAX_KEY_LEN, no_pad->in, no_pad->in_len);

    ret = ext_rsa_calc(rsa, no_pad->mode, data, no_pad->out, no_pad->in_len);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(ext_rsa_calc, ret);
        crypto_free(data);
        data = HI_NULL;
        return ret;
    }

    crypto_free(data);
    data = HI_NULL;

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 ext_rsa_encrypt(hi_cipher_rsa_encrypt_scheme scheme, mbedtls_rsa_context *rsa, rsa_padding_pack *pad)
{
    hi_s32 ret;

    switch (scheme) {
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_NO_PADDING:
            ret = rsa_no_padding(rsa, pad);
            if (ret != HI_SUCCESS) {
                hi_log_print_func_err(rsa_no_padding, ret);
                return ret;
            }
            break;
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_BLOCK_TYPE_0:
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_BLOCK_TYPE_1:
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_BLOCK_TYPE_2:
            pad->bt = scheme - HI_CIPHER_RSA_ENCRYPT_SCHEME_BLOCK_TYPE_0;
            ret = rsa_padding_add_pkcs1_type(rsa, pad);
            if (ret != HI_SUCCESS) {
                hi_log_print_func_err(rsa_padding_add_pkcs1_type, ret);
                return ret;
            }
            break;
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA1:
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA224:
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA256:
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA384:
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA512:
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_PKCS1_V1_5:
            ret = mbedtls_rsa_pkcs1_encrypt(rsa, mbedtls_get_random, HI_NULL, pad->mode, pad->in_len, pad->in,
                pad->out);
            if (ret != HI_SUCCESS) {
                hi_log_print_func_err(mbedtls_rsa_pkcs1_encrypt, ret);
                return ret;
            }
            break;
        default:
            hi_log_error("RSA padding mode error, mode = 0x%x.\n", scheme);
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
            return HI_ERR_CIPHER_INVALID_PARAM;
    }

    return HI_SUCCESS;
}

hi_s32 cryp_rsa_encrypt(cryp_rsa_key *key, cryp_rsa_crypt_data *rsa_crypt)
{
    hi_s32 ret;
    int padding = 0;
    int hash_id = 0;
    int hashlen = 0;
    mbedtls_rsa_context rsa;
    rsa_padding_pack pad;

    hi_log_func_enter();

    hi_log_chk_param_return(key            == HI_NULL);
    hi_log_chk_param_return(rsa_crypt      == HI_NULL);
    hi_log_chk_param_return(rsa_crypt->in  == HI_NULL);
    hi_log_chk_param_return(rsa_crypt->out == HI_NULL);
    hi_log_chk_param_return(key->klen > RSA_KEY_LEN_4096);
    hi_log_chk_param_return(rsa_crypt->in_len > key->klen);
    hi_log_chk_param_return(key->ca_type >= HI_CIPHER_KEY_SRC_BUTT);

    ret = cryp_rsa_get_alg(rsa_crypt->scheme, &padding, &hash_id, &hashlen);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cryp_rsa_get_alg, ret);
        return ret;
    }

    kapi_rsa_lock_err_return();

    mbedtls_rsa_init(&rsa, padding, hash_id);
    crypto_memset(&pad, sizeof(pad), 0, sizeof(pad));

    ret = cryp_rsa_init_key(key, &pad.mode, &rsa);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cryp_rsa_init_key, ret);
        kapi_rsa_unlock();
        return ret;
    }

    pad.in = rsa_crypt->in;
    pad.in_len = rsa_crypt->in_len;
    pad.out = rsa_crypt->out;
    pad.out_len = &rsa_crypt->out_len;
    pad.klen = key->klen;
    ret = ext_rsa_encrypt(rsa_crypt->scheme, &rsa, &pad);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(ext_rsa_encrypt, ret);
        cryp_rsa_deinit_key(&rsa);
        kapi_rsa_unlock();
        return ret;
    }

    rsa_crypt->out_len = key->klen;
    cryp_rsa_deinit_key(&rsa);
    kapi_rsa_unlock();
    hi_log_func_exit();
    return ret;
}

static hi_s32 ext_rsa_decrypt(hi_cipher_rsa_encrypt_scheme scheme, mbedtls_rsa_context *rsa, rsa_padding_pack *pad)
{
    hi_s32 ret;
    size_t out_size = 0;

    switch (scheme) {
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_NO_PADDING:
            ret = rsa_no_padding(rsa, pad);
            if (ret != HI_SUCCESS) {
                hi_log_print_func_err(rsa_no_padding, ret);
                return ret;
            }
            *pad->out_len = pad->klen;
            break;
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_BLOCK_TYPE_0:
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_BLOCK_TYPE_1:
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_BLOCK_TYPE_2:
            pad->bt = scheme - HI_CIPHER_RSA_ENCRYPT_SCHEME_BLOCK_TYPE_0;
            ret = rsa_padding_check_pkcs1_type(rsa, pad);
            if (ret != HI_SUCCESS) {
                hi_log_print_func_err(rsa_padding_check_pkcs1_type, ret);
                hi_log_print_err_code(HI_ERR_CIPHER_FAILED_DECRYPT);
                return HI_ERR_CIPHER_FAILED_DECRYPT;
            }
            break;
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA1:
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA224:
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA256:
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA384:
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA512:
        case HI_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_PKCS1_V1_5:
            ret = mbedtls_rsa_pkcs1_decrypt(rsa, mbedtls_get_random, HI_NULL, pad->mode, &out_size,
                pad->in, pad->out, pad->klen);
            *pad->out_len = (hi_u32)out_size;
            if (ret != HI_SUCCESS) {
                hi_log_print_func_err(mbedtls_rsa_pkcs1_decrypt, ret);
                hi_log_print_err_code(HI_ERR_CIPHER_FAILED_DECRYPT);
                return HI_ERR_CIPHER_FAILED_DECRYPT;
            }
            break;
        default:
            hi_log_error("RSA padding mode error, mode = 0x%x.\n", scheme);
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
            return HI_ERR_CIPHER_INVALID_PARAM;
    }

    return HI_SUCCESS;
}

hi_s32 cryp_rsa_decrypt(cryp_rsa_key *key, cryp_rsa_crypt_data *rsa_crypt)
{
    hi_s32 ret;
    int padding = 0;
    int hash_id = 0;
    int hashlen = 0;
    mbedtls_rsa_context rsa;
    rsa_padding_pack pad;

    hi_log_func_enter();

    hi_log_chk_param_return(key            == HI_NULL);
    hi_log_chk_param_return(rsa_crypt      == HI_NULL);
    hi_log_chk_param_return(rsa_crypt->in  == HI_NULL);
    hi_log_chk_param_return(rsa_crypt->out == HI_NULL);
    hi_log_chk_param_return(key->klen > RSA_KEY_LEN_4096);
    hi_log_chk_param_return(rsa_crypt->in_len != key->klen);
    hi_log_chk_param_return(key->ca_type >= HI_CIPHER_KEY_SRC_BUTT);

    ret = cryp_rsa_get_alg(rsa_crypt->scheme, &padding, &hash_id, &hashlen);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cryp_rsa_get_alg, ret);
        return ret;
    }

    kapi_rsa_lock_err_return();

    mbedtls_rsa_init(&rsa, padding, hash_id);
    crypto_memset(&pad, sizeof(pad), 0, sizeof(pad));

    ret = cryp_rsa_init_key(key, &pad.mode, &rsa);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cryp_rsa_init_key, ret);
        kapi_rsa_unlock();
        return ret;
    }

    pad.in = rsa_crypt->in;
    pad.in_len = rsa_crypt->in_len;
    pad.out = rsa_crypt->out;
    pad.out_len = &rsa_crypt->out_len;
    pad.klen = key->klen;
    ret = ext_rsa_decrypt(rsa_crypt->scheme, &rsa, &pad);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(ext_rsa_decrypt, ret);
        cryp_rsa_deinit_key(&rsa);
        kapi_rsa_unlock();
        return ret;
    }

    rsa_crypt->out_len = *pad.out_len;
    cryp_rsa_deinit_key(&rsa);
    kapi_rsa_unlock();
    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 cryp_rsa_sign_hash(cryp_rsa_key *key, cryp_rsa_sign_data *rsa_sign)
{
    hi_s32 ret;
    hi_u32 mode = 0;
    int padding = 0;
    int hash_id = 0;
    int hashlen = 0;
    mbedtls_rsa_context rsa;

    hi_log_func_enter();

    hi_log_chk_param_return(key           == HI_NULL);
    hi_log_chk_param_return(rsa_sign      == HI_NULL);
    hi_log_chk_param_return(rsa_sign->in  == HI_NULL);
    hi_log_chk_param_return(rsa_sign->out == HI_NULL);
    hi_log_chk_param_return(key->klen > RSA_KEY_LEN_4096);
    hi_log_chk_param_return(rsa_sign->in_len > key->klen);

    ret = cryp_rsa_get_alg(rsa_sign->scheme, &padding, &hash_id, &hashlen);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cryp_rsa_get_alg, ret);
        return ret;
    }
    hi_log_chk_param_return(rsa_sign->in_len < hashlen);

    kapi_rsa_lock_err_return();

    mbedtls_rsa_init(&rsa, padding, hash_id);

    ret = cryp_rsa_init_key(key, &mode, &rsa);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cryp_rsa_init_key, ret);
        kapi_rsa_unlock();
        return ret;
    }

    /* rsa_sign->in is input hash data, rsa_sign->out is ouput sign data. */
    ret = mbedtls_rsa_pkcs1_sign(&rsa, mbedtls_get_random, HI_NULL,
                                 mode, hash_id, hashlen, rsa_sign->in, rsa_sign->out);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(mbedtls_rsa_pkcs1_sign, ret);
        hi_log_print_err_code(HI_ERR_CIPHER_RSA_SIGN);
        cryp_rsa_deinit_key(&rsa);
        kapi_rsa_unlock();
        return HI_ERR_CIPHER_RSA_SIGN;
    }

    rsa_sign->out_len = key->klen;
    cryp_rsa_deinit_key(&rsa);

    kapi_rsa_unlock();
    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 cryp_rsa_verify_hash(cryp_rsa_key *key, cryp_rsa_sign_data *rsa_verify)
{
    hi_s32 ret;
    int padding = 0;
    int hash_id = 0;
    int hashlen = 0;
    hi_u32 mode = 0;
    mbedtls_rsa_context rsa;

    hi_log_func_enter();

    hi_log_chk_param_return(key             == HI_NULL);
    hi_log_chk_param_return(rsa_verify      == HI_NULL);
    hi_log_chk_param_return(rsa_verify->in  == HI_NULL);
    hi_log_chk_param_return(rsa_verify->out == HI_NULL);
    hi_log_chk_param_return(key->klen > RSA_KEY_LEN_4096);
    hi_log_chk_param_return(rsa_verify->in_len != key->klen);
    hi_log_chk_param_return(rsa_verify->out_len > key->klen);
    hi_log_chk_param_return(key->ca_type != HI_CIPHER_KEY_SRC_USER);

    ret = cryp_rsa_get_alg(rsa_verify->scheme, &padding, &hash_id, &hashlen);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cryp_rsa_get_alg, ret);
        return ret;
    }
    hi_log_chk_param_return(rsa_verify->in_len < hashlen);

    kapi_rsa_lock_err_return();

    mbedtls_rsa_init(&rsa, padding, hash_id);

    ret = cryp_rsa_init_key(key, &mode, &rsa);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cryp_rsa_init_key, ret);
        kapi_rsa_unlock();
        return ret;
    }

    /* rsa_verify->out is input hash data, rsa_verify->in is input sign data. */
    ret = mbedtls_rsa_pkcs1_verify(&rsa, mbedtls_get_random, HI_NULL,
                                   mode, hash_id, hashlen, rsa_verify->out, rsa_verify->in);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(mbedtls_rsa_pkcs1_verify, ret);
        hi_log_print_err_code(HI_ERR_CIPHER_RSA_VERIFY);
        cryp_rsa_deinit_key(&rsa);
        kapi_rsa_unlock();
        return HI_ERR_CIPHER_RSA_VERIFY;
    }

    cryp_rsa_deinit_key(&rsa);
    kapi_rsa_unlock();
    hi_log_func_exit();
    return HI_SUCCESS;
}
