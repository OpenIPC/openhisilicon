/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : drivers for ext sm4.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#include "drv_osal_lib.h"
#include "cryp_symc.h"

#ifdef SOFT_SM4_SUPPORT

/* *********************** Internal Structure Definition ********************* */
#define SM4_BLOCK_SIZE  16

static const hi_u32 g_fk[SM4_BLOCK_SIZE / WORD_WIDTH] = {
    0xa3b1bac6, 0x56aa3350, 0x677d9197, 0xb27022dc
};

static const hi_u32 g_ck[] = {
    0x00070e15, 0x1c232a31, 0x383f464d, 0x545b6269,
    0x70777e85, 0x8c939aa1, 0xa8afb6bd, 0xc4cbd2d9,
    0xe0e7eef5, 0xfc030a11, 0x181f262d, 0x343b4249,
    0x50575e65, 0x6c737a81, 0x888f969d, 0xa4abb2b9,
    0xc0c7ced5, 0xdce3eaf1, 0xf8ff060d, 0x141b2229,
    0x30373e45, 0x4c535a61, 0x686f767d, 0x848b9299,
    0xa0a7aeb5, 0xbcc3cad1, 0xd8dfe6ed, 0xf4fb0209,
    0x10171e25, 0x2c333a41, 0x484f565d, 0x646b7279
};

static const hi_u32 g_sbox[16][16] = { /* Two-dimensional array 16, 16. */
    {0xd6, 0x90, 0xe9, 0xfe, 0xcc, 0xe1, 0x3d, 0xb7, 0x16, 0xb6, 0x14, 0xc2, 0x28, 0xfb, 0x2c, 0x05},
    {0x2b, 0x67, 0x9a, 0x76, 0x2a, 0xbe, 0x04, 0xc3, 0xaa, 0x44, 0x13, 0x26, 0x49, 0x86, 0x06, 0x99},
    {0x9c, 0x42, 0x50, 0xf4, 0x91, 0xef, 0x98, 0x7a, 0x33, 0x54, 0x0b, 0x43, 0xed, 0xcf, 0xac, 0x62},
    {0xe4, 0xb3, 0x1c, 0xa9, 0xc9, 0x08, 0xe8, 0x95, 0x80, 0xdf, 0x94, 0xfa, 0x75, 0x8f, 0x3f, 0xa6},
    {0x47, 0x07, 0xa7, 0xfc, 0xf3, 0x73, 0x17, 0xba, 0x83, 0x59, 0x3c, 0x19, 0xe6, 0x85, 0x4f, 0xa8},
    {0x68, 0x6b, 0x81, 0xb2, 0x71, 0x64, 0xda, 0x8b, 0xf8, 0xeb, 0x0f, 0x4b, 0x70, 0x56, 0x9d, 0x35},
    {0x1e, 0x24, 0x0e, 0x5e, 0x63, 0x58, 0xd1, 0xa2, 0x25, 0x22, 0x7c, 0x3b, 0x01, 0x21, 0x78, 0x87},
    {0xd4, 0x00, 0x46, 0x57, 0x9f, 0xd3, 0x27, 0x52, 0x4c, 0x36, 0x02, 0xe7, 0xa0, 0xc4, 0xc8, 0x9e},
    {0xea, 0xbf, 0x8a, 0xd2, 0x40, 0xc7, 0x38, 0xb5, 0xa3, 0xf7, 0xf2, 0xce, 0xf9, 0x61, 0x15, 0xa1},
    {0xe0, 0xae, 0x5d, 0xa4, 0x9b, 0x34, 0x1a, 0x55, 0xad, 0x93, 0x32, 0x30, 0xf5, 0x8c, 0xb1, 0xe3},
    {0x1d, 0xf6, 0xe2, 0x2e, 0x82, 0x66, 0xca, 0x60, 0xc0, 0x29, 0x23, 0xab, 0x0d, 0x53, 0x4e, 0x6f},
    {0xd5, 0xdb, 0x37, 0x45, 0xde, 0xfd, 0x8e, 0x2f, 0x03, 0xff, 0x6a, 0x72, 0x6d, 0x6c, 0x5b, 0x51},
    {0x8d, 0x1b, 0xaf, 0x92, 0xbb, 0xdd, 0xbc, 0x7f, 0x11, 0xd9, 0x5c, 0x41, 0x1f, 0x10, 0x5a, 0xd8},
    {0x0a, 0xc1, 0x31, 0x88, 0xa5, 0xcd, 0x7b, 0xbd, 0x2d, 0x74, 0xd0, 0x12, 0xb8, 0xe5, 0xb4, 0xb0},
    {0x89, 0x69, 0x97, 0x4a, 0x0c, 0x96, 0x77, 0x7e, 0x65, 0xb9, 0xf1, 0x09, 0xc5, 0x6e, 0xc6, 0x84},
    {0x18, 0xf0, 0x7d, 0xec, 0x3a, 0xdc, 0x4d, 0x20, 0x79, 0xee, 0x5f, 0x3e, 0xd7, 0xcb, 0x39, 0x48}
};

#define getu32(pt) (((hi_u32)(pt)[0] << 24) ^ ((hi_u32)(pt)[1] << 16) ^ ((hi_u32)(pt)[2] << 8) ^ ((hi_u32)(pt)[3]))
#define putu32(ct, st) {(ct)[0] = (hi_u8)((st) >> 24); (ct)[1] = (hi_u8)((st) >> 16); (ct)[2] = (hi_u8)((st) >> 8); \
                        (ct)[3] = (hi_u8)(st);}

#define KEY_EXT                     0
#define CIPHER                      1
#define SM4_RD_KEY_LEN              32
#define SM4_RD_KEY_BUF_LEN          36
typedef struct {
    hi_u32 rd_key[SM4_RD_KEY_BUF_LEN];
} sm4_key;

typedef union {
    hi_u32  i; /* descript: i = {c[3], c[2], c[1], c[0]} */
    hi_u8 c[WORD_WIDTH];
} is4;

typedef struct {
    hi_u8  key[SM4_KEY_SIZE];      /* sm4 even round keys, default */
    hi_u32 klen;                   /* symc key length */
    hi_u8  iv[AES_IV_SIZE];
    symc_mode mode;
} ext_sm4_context;

/* linearity replace: left shift left, right shift right, max for residual. */
#define linearity_replace(tmp, left, right, max)     (((tmp) << (left)) + (((tmp) >> (right)) & (max)))

/* Sbox */
hi_u32 mix_r(const hi_u32 data_in, const hi_s32 type)
{
    is4 temp;
    hi_u32 rep_rsl;
    hi_u8  sbox_tmp[WORD_WIDTH] = {0};
    hi_u8  sbox_c, sbox_r;
    hi_s32 j;

    for (j = 0; j < WORD_WIDTH; j++) {
        sbox_tmp[j] = 0;
        sbox_r = ((data_in << (j * BYTE_BITS)) >> 28);              /* soft sm4 alg: right shift 28 bits. */
        sbox_c = ((data_in << (j * BYTE_BITS + BYTE_4BIT)) >> 28);  /* soft sm4 alg: right shift 28 bits. */
        sbox_tmp[j] = g_sbox[sbox_r][sbox_c];
    }
    temp.c[WORD_IDX_3] = sbox_tmp[WORD_IDX_0];
    temp.c[WORD_IDX_2] = sbox_tmp[WORD_IDX_1];
    temp.c[WORD_IDX_1] = sbox_tmp[WORD_IDX_2];
    temp.c[WORD_IDX_0] = sbox_tmp[WORD_IDX_3];

    /* linearity replace */
    if (type == KEY_EXT) {
        rep_rsl = temp.i  ^ \
            linearity_replace(temp.i, 13, 19, 0x00001fff) ^ \ /* linearity replace: left shift 13, 19, 0x00001fff. */
            linearity_replace(temp.i, 23,  9, 0x007fffff);    /* linearity replace: left shift 23,  9, 0x007fffff. */
    } else {
        rep_rsl = temp.i ^ \
            linearity_replace(temp.i,  2, 30, 0x00000003) ^ \ /* linearity replace: left shift  2, 30, 0x00000003. */
            linearity_replace(temp.i, 10, 22, 0x000003ff) ^ \ /* linearity replace: left shift 10, 22, 0x000003ff. */
            linearity_replace(temp.i, 18, 14, 0x0003ffff) ^ \ /* linearity replace: left shift 18, 14, 0x0003ffff. */
            linearity_replace(temp.i, 24,  8, 0x00ffffff);    /* linearity replace: left shift 24,  8, 0x00ffffff. */
    }

    return rep_rsl;
}

/* Set key */
hi_s32 sm4_set_encrypt_key(const hi_u8 *user_key, const hi_s32 bits, sm4_key *key)
{
    hi_s32 i;
    hi_u32 k_temp[SM4_KEY_SIZE / WORD_WIDTH] = {0};
    hi_u32 K[SM4_RD_KEY_BUF_LEN] = {0};
    hi_u32 temp;

    if (user_key == HI_NULL || key == HI_NULL) {
        return -1;
    }

    k_temp[WORD_IDX_0] = getu32(user_key);
    k_temp[WORD_IDX_1] = getu32(user_key + WORD_IDX_1 * WORD_WIDTH);
    k_temp[WORD_IDX_2] = getu32(user_key + WORD_IDX_2 * WORD_WIDTH);
    k_temp[WORD_IDX_3] = getu32(user_key + WORD_IDX_3 * WORD_WIDTH);

    for (i = 0; i < SM4_KEY_SIZE / WORD_WIDTH; i++) {
        K[i] = k_temp[i] ^ g_fk[i];
    }
    i = 0;
    while (1) {
        temp = K[i + WORD_IDX_1] ^ K[i + WORD_IDX_2] ^ K[i + WORD_IDX_3] ^ g_ck[i];
        K[i + WORD_IDX_4] = K[i] ^ mix_r(temp, KEY_EXT);
        key->rd_key[i] = K[i + WORD_IDX_4];

        if (++i == SM4_RD_KEY_LEN) {
            return 0;
        }
    }

    return 0;
}

/* SM4 Encrypt */
hi_void sm4_encrypt(const hi_u8 *in, hi_u8 *out, const sm4_key *key, hi_u32 len)
{
    hi_u32 s[SM4_RD_KEY_BUF_LEN] = {0};
    hi_u32 temp;
    hi_s32 i;

    crypto_unused(len);

    s[WORD_IDX_0] = getu32(in + WORD_IDX_0 * WORD_WIDTH);
    s[WORD_IDX_1] = getu32(in + WORD_IDX_1 * WORD_WIDTH);
    s[WORD_IDX_2] = getu32(in + WORD_IDX_2 * WORD_WIDTH);
    s[WORD_IDX_3] = getu32(in + WORD_IDX_3 * WORD_WIDTH);

    for (i = 0; i < SM4_RD_KEY_LEN; i++) {
        temp = s[i + WORD_IDX_1] ^ s[i + WORD_IDX_2] ^ s[i + WORD_IDX_3] ^ key->rd_key[i];
        s[i + WORD_IDX_4] = s[i] ^ mix_r(temp, CIPHER);
    }
    putu32(out + WORD_IDX_0 * WORD_WIDTH, s[SM4_RD_KEY_LEN + WORD_IDX_3]);
    putu32(out + WORD_IDX_1 * WORD_WIDTH, s[SM4_RD_KEY_LEN + WORD_IDX_2]);
    putu32(out + WORD_IDX_2 * WORD_WIDTH, s[SM4_RD_KEY_LEN + WORD_IDX_1]);
    putu32(out + WORD_IDX_3 * WORD_WIDTH, s[SM4_RD_KEY_LEN + WORD_IDX_0]);

    return;
}

/* SM4 Decrypt */
hi_void sm4_decrypt(const hi_u8 *in, hi_u8 *out, const sm4_key *key, hi_u32 len)
{
    hi_u32 s[SM4_RD_KEY_BUF_LEN] = {0};
    hi_u32 temp;
    hi_s32 i;

    crypto_unused(len);

    s[WORD_IDX_0] = getu32(in + WORD_IDX_0 * WORD_WIDTH);
    s[WORD_IDX_1] = getu32(in + WORD_IDX_1 * WORD_WIDTH);
    s[WORD_IDX_2] = getu32(in + WORD_IDX_2 * WORD_WIDTH);
    s[WORD_IDX_3] = getu32(in + WORD_IDX_3 * WORD_WIDTH);

    for (i = 0; i < SM4_RD_KEY_LEN; i++) {
        temp = s[i + WORD_IDX_1] ^ s[i + WORD_IDX_2] ^ s[i + WORD_IDX_3] ^ \
            key->rd_key[SM4_RD_KEY_LEN - BOUND_VAL_1 - i];
        s[i + WORD_IDX_4] = s[i] ^ mix_r(temp, CIPHER);
    }

    putu32(out + WORD_IDX_0 * WORD_WIDTH, s[SM4_RD_KEY_LEN + WORD_IDX_3]);
    putu32(out + WORD_IDX_1 * WORD_WIDTH, s[SM4_RD_KEY_LEN + WORD_IDX_2]);
    putu32(out + WORD_IDX_2 * WORD_WIDTH, s[SM4_RD_KEY_LEN + WORD_IDX_1]);
    putu32(out + WORD_IDX_3 * WORD_WIDTH, s[SM4_RD_KEY_LEN + WORD_IDX_0]);

    return;
}

/* SM4 ECB Crypt */
hi_void sm4_ecb_crypt(const hi_u8 *in, hi_u8 *out, hi_u32 len, const sm4_key *key, const hi_s32 dec)
{
    if (dec == SYMC_OPERATION_ENCRYPT) {
        sm4_encrypt(in, out, key, len);
    } else {
        sm4_decrypt(in, out, key, len);
    }
    return;
}

/* SM4 CBC Crypt */
hi_void sm4_cbc_crypt(const hi_u8 *in, hi_u8 *out, const hi_u32 length, const sm4_key *key, hi_u8 *ivec,
    const hi_s32 dec)
{
    hi_u32 n = 0;
    hi_u32 len = length;
    hi_u8 tmp[SM4_BLOCK_SIZE] = {0};

    if (dec == SYMC_OPERATION_ENCRYPT) {
        while (len >= SM4_BLOCK_SIZE) {
            for (n = 0; n < SM4_BLOCK_SIZE; ++n) {
                tmp[n] = in[n] ^ ivec[n];
            }

            sm4_encrypt(tmp, out, key, SM4_BLOCK_SIZE);

            memcpy((hi_void *)ivec, out, SM4_BLOCK_SIZE);
            len -= SM4_BLOCK_SIZE;
            in  += SM4_BLOCK_SIZE;
            out += SM4_BLOCK_SIZE;
        }
    } else {
        while (len >= SM4_BLOCK_SIZE) {
            memcpy(tmp, in, sizeof(tmp));
            sm4_decrypt(tmp, out, key, SM4_BLOCK_SIZE);
            for (n = 0; n < SM4_BLOCK_SIZE; ++n) {
                out[n] ^= ivec[n];
            }
            memcpy((hi_void *)ivec, tmp, SM4_BLOCK_SIZE);
            len -= SM4_BLOCK_SIZE;
            in  += SM4_BLOCK_SIZE;
            out += SM4_BLOCK_SIZE;
        }
    }

    return;
}

/* increment counter (128bit hi_s32) by 2^64 */
static hi_void sm4_ctr128_inc(hi_u8 *counter)
{
    hi_s32 i;

    for (i = SM4_BLOCK_SIZE - BOUND_VAL_1; i >= 0; i--) {
        counter[i]++;
        if (counter[i] != 0) {
            return;
        }
    }
    return;
}

/* SM4 CTR Crypt, CTR mode is big-endian. The rest of SM4 code is endian-neutral */
hi_void sm4_ctr128_crypt(const hi_u8 *in, hi_u8 *out, hi_s32 length, const sm4_key *key, hi_u8 counter[SM4_BLOCK_SIZE],
    hi_u8 ecount_buf[SM4_BLOCK_SIZE], hi_s32 *num)
{
    hi_s32 n, l;

    l = length;
    n = *num;
    while (l--) {
        if (n == 0) {
            sm4_encrypt(counter, ecount_buf, key, SM4_BLOCK_SIZE);
            sm4_ctr128_inc(counter);
        }
        *(out++) = *(in++) ^ ecount_buf[n];
        n = (n + 1) % SM4_BLOCK_SIZE;
    }
    *num = n;

    return;
}

/* SM4 ECB RM */
hi_u32 sm4_ecb_rm(const hi_u8 *data_in, hi_u8 *data_out, hi_s32 data_len, const hi_u8 *key, hi_s32 bit, hi_u32 decrypt)
{
    sm4_key ctx;

    sm4_set_encrypt_key(key, bit, &ctx);

    while (data_len >= SM4_BLOCK_SIZE) {
        sm4_ecb_crypt(data_in, data_out, SM4_BLOCK_SIZE, &ctx, decrypt);
        data_len = data_len - SM4_BLOCK_SIZE;
        data_in  = data_in + SM4_BLOCK_SIZE;
        data_out = data_out + SM4_BLOCK_SIZE;
    }

    return data_len;
}

/* SM4 CBC RM */
hi_u32 sm4_cbc_rm(const hi_u8 *data_in, hi_u8 *data_out, hi_s32 data_len, const hi_u8 *key, hi_s32 bit, hi_u32 decrypt,
    hi_u8 *iv)
{
    hi_s32 left_len;
    hi_s32 valid_data_len;
    sm4_key ctx;

    sm4_set_encrypt_key(key, bit, &ctx);

    left_len = data_len % SM4_BLOCK_SIZE;
    valid_data_len = data_len - left_len;
    sm4_cbc_crypt(data_in, data_out, valid_data_len, &ctx, iv, decrypt);

    return left_len;
}

/* SM4 CTR RM */
hi_u32 sm4_ctr_rm(const hi_u8 *data_in, hi_u8 *data_out, hi_s32 data_len, const hi_u8 *key, hi_s32 bit, hi_u32 decrypt,
    const hi_u8 *iv)
{
    hi_s32 num = 0;
    hi_s32 i;
    hi_u32 valid_data_len;
    hi_u8 encrypt_cnt[SM4_BLOCK_SIZE] = {0};
    sm4_key ctx;

    /* The SM4_ctr128_crypt request:
     * The extra state information to record how much of the
     * 128bit block we have used is contained in *num, and the
     * encrypted counter is kept in ecount_buf. Both *num and
     * ecount_buf must be initialized with zeros before the first
     * called to SM4_ctr128_crypt().
     */
    valid_data_len = data_len;

    sm4_set_encrypt_key(key, bit, &ctx);
    sm4_ctr128_crypt(data_in, data_out, valid_data_len, &ctx, (hi_u8 *)iv, encrypt_cnt, &num);

    return 0;
}

hi_void *ext_sm4_create(hi_u32 hard_chn)
{
    ext_sm4_context *ctx = HI_NULL;

    hi_log_func_enter();

    ctx = crypto_malloc(sizeof(ext_sm4_context));
    if (ctx == HI_NULL) {
        hi_log_print_err_code(HI_ERR_CIPHER_FAILED_MEM);
        return HI_NULL;
    }
    crypto_memset(ctx, sizeof(ext_sm4_context), 0, sizeof(ext_sm4_context));

    hi_log_func_exit();

    return ctx;
}

hi_s32 ext_sm4_destory(hi_void *ctx)
{
    hi_log_func_enter();

    if (ctx != HI_NULL) {
        crypto_free(ctx);
        ctx = HI_NULL;
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_void ext_sm4_setmode(hi_void *ctx, symc_alg alg, symc_mode mode, symc_width width)
{
    ext_sm4_context *symc = ctx;

    hi_log_func_enter();

    if (symc == HI_NULL) {
        hi_log_error("ctx is null\n");
        return;
    }

    if (width != SYMC_DAT_WIDTH_128) {
        hi_log_error("Invalid width: 0x%x\n", width);
        return;
    }

    switch (mode) {
        case SYMC_MODE_ECB:
            symc->mode = SYMC_MODE_ECB;
            break;
        case SYMC_MODE_CBC:
            symc->mode = SYMC_MODE_CBC;
            break;
        case SYMC_MODE_CTR:
            symc->mode = SYMC_MODE_CTR;
            break;
        default:
            hi_log_error("unsupport mode %d\n", mode);
            return;
    }

    hi_log_func_exit();
    return;
}

hi_s32 ext_sm4_setiv(hi_void *ctx, const hi_u8 *iv, hi_u32 ivlen, hi_u32 usage)
{
    ext_sm4_context *symc = ctx;

    hi_log_func_enter();

    hi_log_chk_param_return(symc == HI_NULL);
    hi_log_chk_param_return(iv == HI_NULL);
    hi_log_chk_param_return(ivlen != HI_NULL);

    crypto_memcpy(symc->iv, AES_IV_SIZE, iv, ivlen);

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_void ext_sm4_getiv(hi_void *ctx, hi_u8 *iv, hi_u32 *ivlen)
{
    ext_sm4_context *symc = ctx;

    hi_log_func_enter();

    if ((symc == HI_NULL) || (iv == HI_NULL) || (ivlen == HI_NULL)) {
        return;
    }

    crypto_memcpy(iv, AES_IV_SIZE, symc->iv, sizeof(symc->iv));
    *ivlen = AES_IV_SIZE;

    hi_log_func_exit();

    return;
}

hi_s32 ext_sm4_setkey(hi_void *ctx, const hi_u8 *fkey, const hi_u8 *skey, hi_u32 *hisi_klen)
{
    hi_u32 klen = 0;
    ext_sm4_context *symc = ctx;

    hi_log_func_enter();

    hi_log_chk_param_return(symc == HI_NULL);
    hi_log_chk_param_return(fkey == HI_NULL);
    hi_log_chk_param_return(hisi_klen == HI_NULL);

    switch (*hisi_klen) {
        case HI_CIPHER_KEY_AES_128BIT:
            klen = AES_KEY_128BIT;
            break;
        default:
            hi_log_error("Invalid aes key len: 0x%x\n", klen);
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
            return HI_ERR_CIPHER_INVALID_PARAM;
    }
    hi_log_info("key len %d, type %d\n", klen, hisi_klen);

    crypto_memcpy(symc->key, SYMC_KEY_SIZE, fkey, klen);
    symc->klen = klen;
    *hisi_klen = klen;

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 ext_sm4_rm(ext_sm4_context *symc, crypto_mem *mem_in, crypto_mem *mem_out, hi_u32 len, hi_u32 operation)
{
    switch (symc->mode) {
        case SYMC_MODE_ECB: {
            sm4_ecb_rm(crypto_mem_virt(mem_in), crypto_mem_virt(mem_out),
                       len, symc->key, symc->klen, operation);
            break;
        }
        case SYMC_MODE_CBC: {
            sm4_cbc_rm(crypto_mem_virt(mem_in), crypto_mem_virt(mem_out),
                       len, symc->key, symc->klen, operation, symc->iv);
            break;
        }
        case SYMC_MODE_CTR: {
            sm4_ctr_rm(crypto_mem_virt(mem_in), crypto_mem_virt(mem_out),
                       len, symc->key, symc->klen, operation, symc->iv);
            break;
        }
        default: {
            HI_PRINT("Err, Invalid mode 0x%x\n", symc->mode);
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
            return HI_ERR_CIPHER_INVALID_PARAM;
        }
    }

    return HI_SUCCESS;
}

hi_s32 ext_sm4_crypto(hi_void *ctx, hi_u32 operation, symc_multi_pack *pack, hi_u32 last)
{
    ext_sm4_context *symc = ctx;
    crypto_mem mem_in, mem_out;
    hi_s32 ret;

    hi_log_func_enter();

    hi_log_chk_param_return(symc == HI_NULL);
    hi_log_chk_param_return(pack == HI_NULL);
    hi_log_chk_param_return(pack->len == HI_NULL);
    hi_log_chk_param_return(pack->in == HI_NULL);
    hi_log_chk_param_return(pack->out == HI_NULL);
    hi_log_chk_param_return(pack->usage == HI_NULL);
    hi_log_chk_param_return(pack->num != 0x01);

    crypto_memset(&mem_in, sizeof(mem_in), 0, sizeof(mem_in));
    crypto_memset(&mem_out, sizeof(mem_out), 0, sizeof(mem_out));

    ret = crypto_mem_open(&mem_in, pack->in[0], pack->len[0]);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_mem_open, ret);
        return ret;
    }

    ret = crypto_mem_open(&mem_out, pack->out[0], pack->len[0]);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_mem_open, ret);
        crypto_mem_close(&mem_in);
        return ret;
    }

    ret = ext_sm4_rm(symc, &mem_in, &mem_out, pack->len[0], operation);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(ext_sm4_rm, ret);
        crypto_mem_close(&mem_out);
        crypto_mem_close(&mem_in);
        return ret;
    }

    crypto_mem_close(&mem_out);
    crypto_mem_close(&mem_in);
    hi_log_func_exit();
    return HI_SUCCESS;
}

#endif
