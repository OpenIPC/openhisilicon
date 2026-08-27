/*****************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : cryp_rsa.c
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include "drv_osal_lib.h"
#include "drv_srsa.h"
#include "cryp_rsa.h"
#include "cryp_trng.h"
#include "mbedtls/rsa.h"

/********************** Internal Structure Definition ************************/
/** \addtogroup      rsa */
/** @{*/  /** <!-- [rsa]*/

#define RSA_PKCS1_TYPE_MIN_PAD_LEN               (11)
#define RSA_BITS_1024                            1024
#define RSA_BITS_2048                            2048
#define RSA_BITS_3072                            3072
#define RSA_BITS_4096                            4096

/*! rsa mutex */
static crypto_mutex rsa_mutex;
static hi_u32 rsa_key_ca_type = HI_UNF_CIPHER_KEY_SRC_USER;

#define KAPI_RSA_LOCK()   \
    ret = crypto_mutex_lock(&rsa_mutex);  \
    if (ret != HI_SUCCESS)        \
    {\
        HI_LOG_ERROR("error, rsa lock failed\n");\
        HI_LOG_FUNC_EXIT();\
        return ret;\
    }

#define KAPI_RSA_UNLOCK()   crypto_mutex_unlock(&rsa_mutex)

/*! \rsa rsa soft function */
extern int mbedtls_mpi_exp_mod_sw(mbedtls_mpi *X, const mbedtls_mpi *A,
                                  const mbedtls_mpi *E, const mbedtls_mpi *N,
                                  mbedtls_mpi *_RR);

/** @}*/  /** <!-- ==== Structure Definition end ====*/

/******************************* API Code *****************************/
/** \addtogroup      rsa drivers*/
/** @{*/  /** <!-- [rsa]*/

void mbedtls_mpi_print(const mbedtls_mpi *X, const char *name)
{
#ifdef CIPHER_DEBUG_SUPPORT
    int ret;
    size_t n;
    hi_u8 buf[512] = {0};

    n = mbedtls_mpi_size(X);
    MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(X, buf, n));
    HI_PRINT_HEX(name, (hi_u8 *)buf, n);

cleanup:
    return;
#endif
}

#ifdef CHIP_RSA_SUPPORT
static hi_s32 rsa_get_klen(unsigned long module_len, hi_u32 *keylen, rsa_key_width *width)
{
    if (module_len <= RSA_KEY_LEN_1024) {
        *keylen = 128;
        *width = RSA_KEY_WIDTH_1024;
    } else if (module_len <= RSA_KEY_LEN_2048) {
        *keylen = 256;
        *width = RSA_KEY_WIDTH_2048;
    } else if (module_len <= RSA_KEY_LEN_4096) {
        *keylen = 512;
        *width = RSA_KEY_WIDTH_4096;
    } else {
        HI_LOG_ERROR("error, invalid key len %ld\n", module_len);
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    return HI_SUCCESS;
}

static hi_s32 cryp_check_data(hi_u8 *N, hi_u8 *E, hi_u8 *MC, hi_u32 len)
{
    hi_u32 i;

    /*MC > 0*/
    for (i = 0; i < len; i++) {
        if (0 < MC[i]) {
            break;
        }
    }
    if (i >= len) {
        HI_LOG_ERROR("RSA M/C is zero, error!\n");
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    /*MC < N*/
    for (i = 0; i < len; i++) {
        if (MC[i] < N[i]) {
            break;
        }
    }
    if (i >= len) {
        HI_LOG_ERROR("RSA M/C is larger than N, error!\n");
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    /*E >= 1*/
    for (i = 0; i < len; i++) {
        if (E[i] > 0) {
            break;
        }
    }
    if (i >= len) {
        HI_LOG_ERROR("RSA D/E is zero, error!\n");
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    return HI_SUCCESS;
}

static int cryp_ifep_rsa_exp_mod(hi_u32 ca_type, mbedtls_mpi *X, const mbedtls_mpi *A,
                                 const mbedtls_mpi *E, const mbedtls_mpi *N,
                                 mbedtls_mpi *_RR)
{
    hi_u32 module_len = 0;
    hi_u8 *buf = HI_NULL;
    hi_u8 *n = HI_NULL, *k = HI_NULL;
    hi_u8 *in = HI_NULL, *out = HI_NULL;
    hi_u32 keylen = 0;
    rsa_key_width width = 0;
    mbedtls_mpi _A;
    hi_s32 ret = HI_FAILURE;

    HI_LOG_FUNC_ENTER();

    /* computes valid bits of N */
    module_len = MAX(mbedtls_mpi_size(N), mbedtls_mpi_size(E));

    ret = rsa_get_klen(module_len, &keylen, &width);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(rsa_get_klen, ret);
        return ret;
    }

    /* mallc buf to store n || k(e or d) || in || out */
    buf = crypto_malloc(keylen * 4);
    if (buf == HI_NULL) {
        HI_LOG_PRINT_FUNC_ERR(crypto_malloc, HI_ERR_CIPHER_FAILED_MEM);
        return HI_ERR_CIPHER_FAILED_MEM;
    }

    /* zero */
    crypto_memset(buf, keylen * 4, 0, keylen * 4);

    n = buf;
    k = n + keylen;
    in = k + keylen;
    out = in + keylen;

    mbedtls_mpi_init(&_A);
    CHECK_EXIT(mbedtls_mpi_mod_mpi(&_A, A, N));

    /* read A, E, N */
    CHECK_EXIT(mbedtls_mpi_write_binary(&_A, in, keylen));
    CHECK_EXIT(mbedtls_mpi_write_binary(E, k, keylen));
    CHECK_EXIT(mbedtls_mpi_write_binary(N, n, keylen));

    /* key and data valid ?*/
    CHECK_EXIT(cryp_check_data(n, k, in, keylen));

    /* out = in ^ k mod n */
    ret = drv_ifep_rsa_exp_mod(ca_type, n, k, in, out, width);
    if (ret == HI_SUCCESS) {
        /* write d */
        mbedtls_mpi_read_binary(X, out, keylen);
    }

exit__:

    mbedtls_mpi_free(&_A);
    crypto_free(buf);
    buf = HI_NULL;

    HI_LOG_FUNC_EXIT();

    return ret;
}
#endif

int mbedtls_mpi_exp_mod(mbedtls_mpi *X, const mbedtls_mpi *A,
                        const mbedtls_mpi *E,
                        const mbedtls_mpi *N,
                        mbedtls_mpi *_RR)
{
    hi_s32 ret = HI_FAILURE;
#if defined(CHIP_IFEP_RSA_VER_V100)
    hi_u32 elen = 0;
#endif

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(X == HI_NULL);
    HI_LOG_CHECK_PARAM(A == HI_NULL);
    HI_LOG_CHECK_PARAM(E == HI_NULL);
    HI_LOG_CHECK_PARAM(N == HI_NULL);

    mbedtls_mpi_print(A, "M");
    mbedtls_mpi_print(E, "E");
    mbedtls_mpi_print(N, "N");

#if defined(CHIP_IFEP_RSA_VER_V100)
    elen = mbedtls_mpi_size(E);
    if (elen <= RSA_KEY_LEN_4096) {
        /* The private key may be not from user when generate rsa key pare
         * in this case use klad key will failed.
         */
        ret = cryp_ifep_rsa_exp_mod(rsa_key_ca_type, X, A, E, N, _RR);
    } else
#endif
    {
        if (rsa_key_ca_type != HI_UNF_CIPHER_KEY_SRC_USER) {
            HI_LOG_ERROR("sofrware rsa nonsupport klad key\n");
            return HI_ERR_CIPHER_ILLEGAL_KEY;
        }

        ret = mbedtls_mpi_exp_mod_sw(X, A, E, N, _RR);
    }

    mbedtls_mpi_print(X, "X");

    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("rsa mpi_exp_mod failed, ret = 0x%x\n", ret);
        return ret;
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

int cryp_rsa_init(void)
{
    HI_LOG_FUNC_ENTER();

    crypto_mutex_init(&rsa_mutex);

#if defined(CHIP_IFEP_RSA_VER_V100)
    {
        hi_s32 ret = HI_FAILURE;

        ret = drv_rsa_init();
        if (ret != HI_SUCCESS) {
            HI_LOG_PRINT_FUNC_ERR(drv_rsa_init, ret);
            return ret;
        }
    }
#endif

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

void cryp_rsa_deinit(void)
{
#if defined(CHIP_IFEP_RSA_VER_V100)
    rsa_capacity capacity;

    drv_ifep_rsa_get_capacity(&capacity);

    /* recovery the rsa function of mbedtls */
    if (capacity.rsa) {
        drv_rsa_deinit();
    }
#endif

    crypto_mutex_destroy(&rsa_mutex);
}

int mbedtls_get_random(void *param, hi_u8 *rand, size_t size)
{
    hi_u32 i;
    hi_u32 randnum = 0;

    for (i = 0; i < size; i += 4) {
        cryp_trng_get_random(&randnum, -1);
        rand[i + 3] = (hi_u8)(randnum >> 24) & 0xFF;
        rand[i + 2] = (hi_u8)(randnum >> 16) & 0xFF;
        rand[i + 1] = (hi_u8)(randnum >> 8) & 0xFF;
        rand[i + 0] = (hi_u8)(randnum) & 0xFF;
    }

    return HI_SUCCESS;
}

static hi_s32 cryp_rsa_init_key(cryp_rsa_key *key, hi_u32 *mode, mbedtls_rsa_context *rsa)
{
    hi_s32 ret = HI_FAILURE;

    HI_LOG_FUNC_ENTER();

    mbedtls_mpi_init(&rsa->N);
    mbedtls_mpi_init(&rsa->E);
    mbedtls_mpi_init(&rsa->D);
    mbedtls_mpi_init(&rsa->P);
    mbedtls_mpi_init(&rsa->Q);
    mbedtls_mpi_init(&rsa->DP);
    mbedtls_mpi_init(&rsa->DQ);
    mbedtls_mpi_init(&rsa->QP);

    CHECK_EXIT(mbedtls_mpi_read_binary(&rsa->N, key->n, key->klen));
    rsa->len = key->klen;
    if ((rsa->len < RSA_MIN_KEY_LEN) || (rsa->len > RSA_MAX_KEY_LEN)) {
        HI_LOG_ERROR("RSA invalid keylen: 0x%x!\n", (hi_u32)rsa->len);
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
        ret = HI_ERR_CIPHER_INVALID_PARA;
        goto exit__;
    }

    if (key->public) {
        CHECK_EXIT(mbedtls_mpi_read_binary(&rsa->E, (hi_u8 *)&key->e, WORD_WIDTH));
        *mode = MBEDTLS_RSA_PUBLIC;
    } else {
        if (key->d != HI_NULL) { /* Non CRT */
            CHECK_EXIT(mbedtls_mpi_read_binary(&rsa->D, key->d, key->klen));
            *mode = MBEDTLS_RSA_PRIVATE;
        } else { /* CRT */
            CHECK_EXIT(mbedtls_mpi_read_binary(&rsa->P, key->p, key->klen / 2));
            CHECK_EXIT(mbedtls_mpi_read_binary(&rsa->Q, key->q, key->klen / 2));
            CHECK_EXIT(mbedtls_mpi_read_binary(&rsa->DP, key->dp, key->klen / 2));
            CHECK_EXIT(mbedtls_mpi_read_binary(&rsa->DQ, key->dq, key->klen / 2));
            CHECK_EXIT(mbedtls_mpi_read_binary(&rsa->QP, key->qp, key->klen / 2));
            *mode = MBEDTLS_RSA_PRIVATE;
        }
    }

    rsa_key_ca_type = key->ca_type;

    HI_LOG_DEBUG("mode %d, e 0x%x\n", *mode, key->e);

    HI_LOG_FUNC_EXIT();
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

static void cryp_rsa_deinit_key(mbedtls_rsa_context *rsa)
{
    HI_LOG_FUNC_ENTER();

    mbedtls_mpi_free(&rsa->N);
    mbedtls_mpi_free(&rsa->E);
    mbedtls_mpi_free(&rsa->D);
    mbedtls_mpi_free(&rsa->P);
    mbedtls_mpi_free(&rsa->Q);
    mbedtls_mpi_free(&rsa->DP);
    mbedtls_mpi_free(&rsa->DQ);
    mbedtls_mpi_free(&rsa->QP);

    HI_LOG_FUNC_EXIT();
}

static hi_s32 cryp_rsa_get_alg(hi_u32 scheme, int *padding, int *hash_id, int *hashlen)
{
    switch (scheme) {
        case HI_CIPHER_RSA_ENC_SCHEME_NO_PADDING:
        case HI_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_0:
        case HI_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_1:
        case HI_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_2: {
            *padding = 0x00;
            *hash_id = 0;
            *hashlen = 0;
            break;
        }
        case HI_CIPHER_RSA_ENC_SCHEME_RSAES_PKCS1_V1_5: {
            *padding = MBEDTLS_RSA_PKCS_V15;
            *hash_id = 0;
            *hashlen = 0;
            break;
        }
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA1: {
            *padding = MBEDTLS_RSA_PKCS_V15;
            *hash_id = MBEDTLS_MD_SHA1;
            *hashlen = SHA1_RESULT_SIZE;
            break;
        }
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA224: {
            *padding = MBEDTLS_RSA_PKCS_V15;
            *hash_id = MBEDTLS_MD_SHA224;
            *hashlen = SHA224_RESULT_SIZE;
            break;
        }
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA256: {
            *padding = MBEDTLS_RSA_PKCS_V15;
            *hash_id = MBEDTLS_MD_SHA256;
            *hashlen = SHA256_RESULT_SIZE;
            break;
        }
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA384: {
            *padding = MBEDTLS_RSA_PKCS_V15;
            *hash_id = MBEDTLS_MD_SHA384;
            *hashlen = SHA384_RESULT_SIZE;
            break;
        }
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA512: {
            *padding = MBEDTLS_RSA_PKCS_V15;
            *hash_id = MBEDTLS_MD_SHA512;
            *hashlen = SHA512_RESULT_SIZE;
            break;
        }
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA1:
        case HI_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA1: {
            *padding = MBEDTLS_RSA_PKCS_V21;
            *hash_id = MBEDTLS_MD_SHA1;
            *hashlen = SHA1_RESULT_SIZE;
            break;
        }
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA224:
        case HI_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA224: {
            *padding = MBEDTLS_RSA_PKCS_V21;
            *hash_id = MBEDTLS_MD_SHA224;
            *hashlen = SHA224_RESULT_SIZE;
            break;
        }
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA256:
        case HI_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA256: {
            *padding = MBEDTLS_RSA_PKCS_V21;
            *hash_id = MBEDTLS_MD_SHA256;
            *hashlen = SHA256_RESULT_SIZE;
            break;
        }
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA384:
        case HI_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA384: {
            *padding = MBEDTLS_RSA_PKCS_V21;
            *hash_id = MBEDTLS_MD_SHA384;
            *hashlen = SHA384_RESULT_SIZE;
            break;
        }
        case HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA512:
        case HI_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA512: {
            *padding = MBEDTLS_RSA_PKCS_V21;
            *hash_id = MBEDTLS_MD_SHA512;
            *hashlen = SHA512_RESULT_SIZE;
            break;
        }
        default: {
            HI_LOG_ERROR("RSA padding mode error, mode = 0x%x.\n", scheme);
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }

    HI_LOG_DEBUG("padding %d, hash_id %d, hashlen %d\n", *padding, *hash_id, *hashlen);

    return HI_SUCCESS;
}

/*PKCS #1: block type 0,1,2 message padding*/
/*************************************************
EB = 00 || BT || PS || 00 || D

PS_LEN >= 8, mlen < key_len - 11
*************************************************/
static hi_s32 rsa_padding_add_pkcs1_type(mbedtls_rsa_context *rsa, hi_u32 mode, hi_u32 klen,
                                      hi_u8  bt, hi_u8 *in, hi_u8 inlen, hi_u8 *out)
{
    hi_s32 ret = HI_FAILURE;
    hi_u32 plen = 0;
    hi_u8 *peb = HI_NULL;
    hi_u32 i = 0;

    HI_LOG_FUNC_ENTER();

    if (inlen > klen - RSA_PKCS1_TYPE_MIN_PAD_LEN) {
        HI_LOG_ERROR("klen is invalid.\n");
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    peb = out;

    /* first byte is 0x00 */
    *(peb++) = 0;

    /* Private Key BT (Block Type) */
    *(peb++) = bt;

    /* The padding string PS shall consist of k-3-||D|| octets */
    plen = klen - 3 - inlen;
    switch (bt) {
        case 0x00: {
            /* For block type 00, the octets shall have value 00 */
            crypto_memset(peb, plen, 0x00, plen);
            break;
        }
        case 0x01: {
            /* for block type 01, they shall have value FF */
            crypto_memset(peb, plen, 0xFF, plen);
            break;
        }
        case 0x02: {
            /* for block type 02, they shall be pseudorandomly generated and nonzero. */
            (hi_void)mbedtls_get_random(HI_NULL, peb, plen);

            /* make sure nonzero */
            for (i = 0; i < plen; i++) {
                if (0x00 == peb[i]) {
                    peb[i] = 0x01;
                }
            }
            break;
        }
        default: {
            HI_LOG_ERROR("BT(0x%x) is invalid.\n", plen);
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }

    /* skip the padding string */
    peb += plen;

    /* set 0x00 follow PS */
    *(peb++) = 0x00;

    /* input data */
    crypto_memcpy(peb, inlen, in, inlen);

    if (mode == MBEDTLS_RSA_PUBLIC) {
        ret = mbedtls_rsa_public(rsa, out, out);
        if (ret != HI_SUCCESS) {
            HI_LOG_ERROR("rsa public failed.\n");
            HI_LOG_PRINT_FUNC_ERR(mbedtls_rsa_public, ret);
            return HI_ERR_CIPHER_RSA_CRYPT_FAILED;
        }
    } else {
        ret = mbedtls_rsa_private(rsa, HI_NULL, 0, out, out);
        if (ret != HI_SUCCESS) {
            HI_LOG_ERROR("rsa private failed.\n");
            HI_LOG_PRINT_FUNC_ERR(mbedtls_rsa_private, ret);
            return HI_ERR_CIPHER_RSA_CRYPT_FAILED;
        }
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

/*PKCS #1: block type 0,1,2 message padding*/
/*************************************************
EB = 00 || BT || PS || 00 || D

PS_LEN >= 8, mlen < key_len - 11
*************************************************/
static hi_s32 rsa_padding_check_pkcs1_type(mbedtls_rsa_context *rsa, hi_u32 klen, hi_u32 mode,
                                        hi_u8  bt, hi_u8 *in, hi_u32 inlen,
                                        hi_u8 *out, hi_u32 *outlen)
{
    hi_s32 ret = HI_FAILURE;
    hi_u8 *peb = HI_NULL;

    HI_LOG_FUNC_ENTER();

    if (mode == MBEDTLS_RSA_PUBLIC) {
        ret = mbedtls_rsa_public(rsa, in, in);
        if (ret != HI_SUCCESS) {
            HI_LOG_ERROR("rsa public failed.\n");
            HI_LOG_PRINT_FUNC_ERR(mbedtls_rsa_public, ret);
            return HI_ERR_CIPHER_RSA_CRYPT_FAILED;
        }
    } else {
        ret = mbedtls_rsa_private(rsa, HI_NULL, 0, in, in);
        if (ret != HI_SUCCESS) {
            HI_LOG_ERROR("rsa private failed.\n");
            HI_LOG_PRINT_FUNC_ERR(mbedtls_rsa_private, ret);
            return HI_ERR_CIPHER_RSA_CRYPT_FAILED;
        }
    }

    *outlen = 0x00;
    peb = in;

    /* first byte must be 0x00 */
    if (*peb != 0x00) {
        HI_LOG_ERROR("EB[0] != 0x00.\n");
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
        return HI_ERR_CIPHER_INVALID_PARA;
    }
    peb++;

    /* Private Key BT (Block Type) */
    if (*peb != bt) {
        HI_LOG_ERROR("EB[1] != BT(0x%x).\n", bt);
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
        return HI_ERR_CIPHER_INVALID_PARA;
    }
    peb++;

    switch (bt) {
        case 0x00:
            /* For block type 00, the octets shall have value 00 */
            for (; peb < in + inlen - 1; peb++) {
                if ((*peb == 0x00) && (*(peb + 1) != 0)) {
                    break;
                }
            }
            break;
        case 0x01:
            /* For block type 0x01 the octets shall have value 0xFF */
            for (; peb < in + inlen - 1; peb++) {
                if (*peb == 0xFF) {
                    continue;
                } else if (*peb == 0x00) {
                    break;
                } else {
                    peb = in + inlen - 1;
                    break;
                }
            }
            break;
        case 0x02:
            /* for block type 02, they shall be pseudorandomly generated and nonzero. */
            for (; peb < in + inlen - 1; peb++) {
                if (0x00 == *peb) {
                    break;
                }
            }
            break;
        default:
            HI_LOG_ERROR("BT(0x%x) is invalid.\n", bt);
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
            return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (peb >= (in + inlen - 1)) {
        HI_LOG_ERROR("PS Error.\n");
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    /* skip 0x00 after PS */
    peb++;

    /* get payload data */
    *outlen = in + klen - peb;
    crypto_memcpy(out, klen, peb, *outlen);

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

static hi_s32 rsa_no_padding(mbedtls_rsa_context *rsa, hi_u32 klen, hi_u32 mode,
                          hi_u8 *in, hi_u32 inlen, hi_u8 *out)
{
    hi_s32 ret = HI_FAILURE;
    hi_u8 *data = HI_NULL;

    HI_LOG_FUNC_ENTER();

    if (inlen > klen) {
        HI_LOG_ERROR("input length %d invalid.\n", inlen);
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    /* mallc data buf */
    data = crypto_malloc(RSA_MAX_KEY_LEN);
    if (data == HI_NULL) {
        HI_LOG_PRINT_FUNC_ERR(crypto_malloc, HI_ERR_CIPHER_FAILED_MEM);
        return HI_ERR_CIPHER_FAILED_MEM;
    }

    crypto_memset(data, RSA_MAX_KEY_LEN, 0, RSA_MAX_KEY_LEN);
    crypto_memcpy(data + klen - inlen, RSA_MAX_KEY_LEN, in, inlen);

    if (mode == MBEDTLS_RSA_PUBLIC) {
        ret = mbedtls_rsa_public(rsa, data, out);
        if (ret != HI_SUCCESS) {
            HI_LOG_ERROR("rsa public failed.\n");
            HI_LOG_PRINT_FUNC_ERR(mbedtls_rsa_public, ret);
            crypto_free(data);
            data = HI_NULL;
            return HI_ERR_CIPHER_RSA_CRYPT_FAILED;
        }
    } else {
        ret = mbedtls_rsa_private(rsa, HI_NULL, 0, data, out);
        if (ret != HI_SUCCESS) {
            HI_LOG_ERROR("rsa private failed, ret = %d.\n", ret);
            HI_LOG_PRINT_FUNC_ERR(mbedtls_rsa_private, ret);
            crypto_free(data);
            data = HI_NULL;
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }

    crypto_free(data);
    data = HI_NULL;

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 cryp_rsa_encrypt(cryp_rsa_key *key, hi_cipher_rsa_enc_scheme scheme,
                     hi_u8 *in, hi_u32 inlen, hi_u8 *out, hi_u32 *outlen)
{
    hi_s32 ret = HI_FAILURE;
    hi_u32 mode = 0;
    int padding = 0;
    int hash_id = 0;
    int hashlen = 0;
    hi_u32 bt = 0;
    mbedtls_rsa_context rsa;

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(key    == HI_NULL);
    HI_LOG_CHECK_PARAM(in     == HI_NULL);
    HI_LOG_CHECK_PARAM(out    == HI_NULL);
    HI_LOG_CHECK_PARAM(outlen == HI_NULL);
    HI_LOG_CHECK_PARAM(key->klen > RSA_KEY_LEN_4096);
    HI_LOG_CHECK_PARAM(inlen > key->klen);
    HI_LOG_CHECK_PARAM(key->ca_type >= HI_CIPHER_KEY_SRC_BUTT);

    ret = cryp_rsa_get_alg(scheme, &padding, &hash_id, &hashlen);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("error, cryp_rsa_get_alg failed");
        HI_LOG_PRINT_ERR_CODE(ret);
        return ret;
    }

    KAPI_RSA_LOCK();

    mbedtls_rsa_init(&rsa, padding, hash_id);

    ret = cryp_rsa_init_key(key, &mode, &rsa);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("error, cryp_rsa_init_key failed");
        HI_LOG_PRINT_ERR_CODE(ret);
        KAPI_RSA_UNLOCK();
        return ret;
    }

    switch (scheme) {
        case HI_CIPHER_RSA_ENC_SCHEME_NO_PADDING: {
            ret = rsa_no_padding(&rsa, key->klen, mode, in, inlen, out);
            break;
        }
        case HI_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_0:
        case HI_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_1:
        case HI_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_2: {
            bt = scheme - HI_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_0;
            ret = rsa_padding_add_pkcs1_type(&rsa, mode, key->klen, bt, in, inlen, out);
            if (ret != HI_SUCCESS) {
                HI_LOG_ERROR("error, rsa add pkcs1_type failed, ret = 0x%x", ret);
                HI_LOG_PRINT_FUNC_ERR(rsa_padding_add_pkcs1_type, ret);
                ret = HI_ERR_CIPHER_FAILED_ENCRYPT;
            }
            break;
        }
        case HI_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA1:
        case HI_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA224:
        case HI_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA256:
        case HI_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA384:
        case HI_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA512:
        case HI_CIPHER_RSA_ENC_SCHEME_RSAES_PKCS1_V1_5: {
            ret = mbedtls_rsa_pkcs1_encrypt(&rsa, mbedtls_get_random,
                                            HI_NULL, mode, inlen, in, out);
            if (ret != HI_SUCCESS) {
                HI_LOG_ERROR("error, rsa pkcs1 encrypt failed, ret = %d", ret);
                HI_LOG_PRINT_FUNC_ERR(rsa_padding_add_pkcs1_type, ret);
                ret = HI_ERR_CIPHER_FAILED_ENCRYPT;
            }
            break;
        }
        default: {
            HI_LOG_ERROR("RSA padding mode error, mode = 0x%x.\n", scheme);
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
            ret = HI_ERR_CIPHER_INVALID_PARA;
            break;
        }
    }

    if (ret == HI_SUCCESS) {
        *outlen = key->klen;
    }

    cryp_rsa_deinit_key(&rsa);

    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("rsa encrypt failed, scheme %d, ret = 0x%x\n", scheme, ret);
        KAPI_RSA_UNLOCK();
        return ret;
    }

    KAPI_RSA_UNLOCK();
    HI_LOG_FUNC_EXIT();

    return ret;
}

hi_s32 cryp_rsa_decrypt(cryp_rsa_key *key, hi_cipher_rsa_enc_scheme scheme,
                     hi_u8 *in, hi_u32 inlen, hi_u8 *out, hi_u32 *outlen)
{
    hi_s32 ret = HI_FAILURE;
    hi_u32 mode = 0;
    int padding = 0;
    int hash_id = 0;
    int hashlen = 0;
    hi_u32 bt = 0;
    size_t outsize = 0;
    mbedtls_rsa_context rsa;

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(key    == HI_NULL);
    HI_LOG_CHECK_PARAM(in     == HI_NULL);
    HI_LOG_CHECK_PARAM(out    == HI_NULL);
    HI_LOG_CHECK_PARAM(outlen == HI_NULL);
    HI_LOG_CHECK_PARAM(key->klen > RSA_KEY_LEN_4096);
    HI_LOG_CHECK_PARAM(inlen != key->klen);
    HI_LOG_CHECK_PARAM(key->ca_type >= HI_CIPHER_KEY_SRC_BUTT);

    ret = cryp_rsa_get_alg(scheme, &padding, &hash_id, &hashlen);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("error, cryp_rsa_sign_get_alg failed");
        HI_LOG_PRINT_FUNC_ERR(cryp_rsa_get_alg, ret);
        return ret;
    }

    KAPI_RSA_LOCK();

    mbedtls_rsa_init(&rsa, padding, hash_id);

    ret = cryp_rsa_init_key(key, &mode, &rsa);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("error, cryp_rsa_init_key failed");
        HI_LOG_PRINT_FUNC_ERR(cryp_rsa_init_key, ret);
        KAPI_RSA_UNLOCK();
        return ret;
    }

    switch (scheme) {
        case HI_CIPHER_RSA_ENC_SCHEME_NO_PADDING: {
            ret = rsa_no_padding(&rsa, key->klen, mode, in, inlen, out);
            *outlen = key->klen;
            if (ret != HI_SUCCESS) {
                HI_LOG_PRINT_FUNC_ERR(rsa_no_padding, ret);
            }
            break;
        }
        case HI_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_0:
        case HI_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_1:
        case HI_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_2: {
            bt = scheme - HI_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_0;
            ret = rsa_padding_check_pkcs1_type(&rsa, key->klen, mode, bt, in, inlen, out, outlen);
            if (ret != HI_SUCCESS) {
                HI_LOG_ERROR("error, rsa check pkcs1 type failed, ret = %d", ret);
                HI_LOG_PRINT_FUNC_ERR(rsa_padding_check_pkcs1_type, ret);
                ret = HI_ERR_CIPHER_FAILED_DECRYPT;
            }
            break;
        }
        case HI_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA1:
        case HI_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA224:
        case HI_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA256:
        case HI_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA384:
        case HI_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA512:
        case HI_CIPHER_RSA_ENC_SCHEME_RSAES_PKCS1_V1_5: {
            ret = mbedtls_rsa_pkcs1_decrypt(&rsa, mbedtls_get_random,
                                            HI_NULL, mode, &outsize, in, out, key->klen);
            *outlen = (hi_u32)outsize;
            if (ret != HI_SUCCESS) {
                HI_LOG_ERROR("error, rsa pkcs1 decrypt failed, ret = %d", ret);
                HI_LOG_PRINT_FUNC_ERR(mbedtls_rsa_pkcs1_decrypt, ret);
                ret = HI_ERR_CIPHER_FAILED_DECRYPT;
            }
            break;
        }
        default: {
            HI_LOG_ERROR("RSA padding mode error, mode = 0x%x.\n", scheme);
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
            ret = HI_ERR_CIPHER_INVALID_PARA;
            break;
        }
    }

    cryp_rsa_deinit_key(&rsa);

    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("rsa decrypt failed, scheme %d, ret = 0x%x\n", scheme, ret);
        KAPI_RSA_UNLOCK();
        return ret;
    }

    KAPI_RSA_UNLOCK();
    HI_LOG_FUNC_EXIT();

    return HI_SUCCESS;
}

hi_s32 cryp_rsa_sign_hash(cryp_rsa_key *key, hi_cipher_rsa_sign_scheme scheme,
                       hi_u8 *in, hi_u32 inlen, hi_u8 *out, hi_u32 *outlen, hi_u32 saltlen)
{
    hi_s32 ret = HI_FAILURE;
    hi_u32 mode = 0;
    int padding = 0;
    int hash_id = 0;
    int hashlen = 0;
    mbedtls_rsa_context rsa;

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(key    == HI_NULL);
    HI_LOG_CHECK_PARAM(in     == HI_NULL);
    HI_LOG_CHECK_PARAM(out    == HI_NULL);
    HI_LOG_CHECK_PARAM(outlen == HI_NULL);
    HI_LOG_CHECK_PARAM(key->klen > RSA_KEY_LEN_4096);
    HI_LOG_CHECK_PARAM(inlen > key->klen);

    ret = cryp_rsa_get_alg(scheme, &padding, &hash_id, &hashlen);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("error, cryp_rsa_sign_get_alg failed");
        HI_LOG_PRINT_FUNC_ERR(cryp_rsa_get_alg, ret);
        return ret;
    }

    KAPI_RSA_LOCK();

    mbedtls_rsa_init(&rsa, padding, hash_id);

    ret = cryp_rsa_init_key(key, &mode, &rsa);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("error, cryp_rsa_init_key failed");
        HI_LOG_PRINT_FUNC_ERR(cryp_rsa_init_key, ret);
        KAPI_RSA_UNLOCK();
        return ret;
    }

    ret = mbedtls_rsa_pkcs1_sign(&rsa, mbedtls_get_random, HI_NULL,
                                 mode, hash_id, hashlen, in, out);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("error, rsa_pkcs1 sign failed, ret = 0x%x\n", ret);
        HI_LOG_PRINT_FUNC_ERR(mbedtls_rsa_pkcs1_sign, ret);
        ret = HI_ERR_CIPHER_RSA_SIGN;
        cryp_rsa_deinit_key(&rsa);
        KAPI_RSA_UNLOCK();
        return ret;
    }

    *outlen = key->klen;

    cryp_rsa_deinit_key(&rsa);

    KAPI_RSA_UNLOCK();
    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 cryp_rsa_verify_hash(cryp_rsa_key *key, hi_cipher_rsa_sign_scheme scheme,
                         hi_u8 *hash, hi_u32 hlen, hi_u8 *sign, hi_u32 signlen, hi_u32 saltlen)
{
    hi_s32 ret = HI_FAILURE;
    int padding = 0;
    int hash_id = 0;
    int hashlen = 0;
    hi_u32 mode = 0;
    mbedtls_rsa_context rsa;

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(key  == HI_NULL);
    HI_LOG_CHECK_PARAM(hash == HI_NULL);
    HI_LOG_CHECK_PARAM(sign == HI_NULL);
    HI_LOG_CHECK_PARAM(key->klen > RSA_KEY_LEN_4096);
    HI_LOG_CHECK_PARAM(signlen > key->klen);
    HI_LOG_CHECK_PARAM(key->ca_type != HI_CIPHER_KEY_SRC_USER);

    ret = cryp_rsa_get_alg(scheme, &padding, &hash_id, &hashlen);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("error, cryp_rsa_sign_get_alg failed");
        HI_LOG_PRINT_FUNC_ERR(cryp_rsa_get_alg, ret);
        return ret;
    }

    KAPI_RSA_LOCK();

    mbedtls_rsa_init(&rsa, padding, hash_id);

    ret = cryp_rsa_init_key(key, &mode, &rsa);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("error, cryp_rsa_init_key failed, ret = %d\n", ret);
        HI_LOG_PRINT_FUNC_ERR(cryp_rsa_init_key, ret);
        KAPI_RSA_UNLOCK();
        return ret;
    }

    ret = mbedtls_rsa_pkcs1_verify(&rsa, mbedtls_get_random, HI_NULL,
                                   mode, hash_id, hashlen, hash, sign);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("error,  rsa pkcs1 verify failed, ret = 0x%x\n", ret);
        HI_LOG_PRINT_FUNC_ERR(mbedtls_rsa_pkcs1_verify, ret);
        ret = HI_ERR_CIPHER_RSA_VERIFY;
        cryp_rsa_deinit_key(&rsa);
        KAPI_RSA_UNLOCK();
        return ret;
    }

    cryp_rsa_deinit_key(&rsa);
    KAPI_RSA_UNLOCK();
    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

/** @}*/  /** <!-- ==== API Code end ====*/
