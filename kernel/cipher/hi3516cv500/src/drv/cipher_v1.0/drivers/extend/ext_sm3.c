/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : drivers for cipher adapt to sm3.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#include "drv_osal_lib.h"
#include "cryp_hash.h"

#ifdef SOFT_SM3_SUPPORT

/* ************************** Internal Structure Definition For SM3 *************************** */
hi_u32 rotate_left(hi_u32 x, hi_u32 n)
{
    n %= SHIFT_32BITS;

    if (n == 0) {
        return x;
    }

    return (x << n) | (x >> (SHIFT_32BITS - n));
}

#define p0(x) ((x) ^ rotate_left((x), SHIFT_9BITS)  ^ rotate_left((x), SHIFT_17BITS))
#define p1(x) ((x) ^ rotate_left((x), SHIFT_15BITS) ^ rotate_left((x), SHIFT_23BITS))

#define ff0(x, y, z) ((x) ^ (y) ^ (z))
#define ff1(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))

#define gg0(x, y, z) ((x) ^ (y) ^ (z))
#define gg1(x, y, z) (((x) & (y)) | ((~(x)) & (z)))

#define SM3_BLOCK_SIZE            64
#define SM3_W_SIZE                ((SM3_BLOCK_SIZE) + (WORD_WIDTH))
#define SM3_RESULT_SIZE_IN_WORD   ((SM3_RESULT_SIZE) / (WORD_WIDTH))
#define SM3_PAD_MIN_SIZE          9
#define SM3_PAD_LEN_SIZE          8
#define SM3_BYTE_MSB              0x80

/* SM3, the initial hash value, H(0). */
#define SM3_H0    0x7380166F
#define SM3_H1    0x4914B2B9
#define SM3_H2    0x172442D7
#define SM3_H3    0xDA8A0600
#define SM3_H4    0xA96F30BC
#define SM3_H5    0x163138AA
#define SM3_H6    0xE38DEE4D
#define SM3_H7    0xB0FB0E4E

/* 64K */
#define HASH_MAX_BUFFER_SIZE    0x10000

/* brief aes ccm context structure */
typedef struct {
    hi_u32 state[SM3_RESULT_SIZE_IN_WORD];
    hi_u8  tail[SM3_BLOCK_SIZE];
    hi_u32 tail_len;
    hi_u32 total;
} ext_sm3_context;

/* ****************************** API Code for sm3 **************************** */
hi_s32 sm3_compress(hi_u32 digest[SM3_RESULT_SIZE_IN_WORD], const hi_u8 block[SM3_BLOCK_SIZE], hi_u32 block_len)
{
    hi_s32 j;
    hi_u32 *W = HI_NULL;
    hi_u32 *W1 = HI_NULL;
    hi_u32 *T = HI_NULL;
    const hi_u32 *local_block = (const hi_u32 *)block;
    hi_u32 A = digest[WORD_IDX_0];
    hi_u32 B = digest[WORD_IDX_1];
    hi_u32 C = digest[WORD_IDX_2];
    hi_u32 D = digest[WORD_IDX_3];
    hi_u32 E = digest[WORD_IDX_4];
    hi_u32 F = digest[WORD_IDX_5];
    hi_u32 G = digest[WORD_IDX_6];
    hi_u32 H = digest[WORD_IDX_7];
    hi_u32 SS1, SS2, TT1, TT2;
    hi_u32 *buf = HI_NULL;
    hi_u32 buf_size;

    if (block_len != SM3_BLOCK_SIZE) {
        return HI_ERR_CIPHER_INVALID_LENGTH;
    }

    buf_size = sizeof(hi_u32) * (SM3_W_SIZE + SM3_BLOCK_SIZE + SM3_BLOCK_SIZE);
    buf = (hi_u32 *)crypto_malloc(buf_size);
    if (buf == HI_NULL) {
        hi_log_error("sm3 compress crypto malloc buff failed!\n");
        return HI_ERR_CIPHER_FAILED_MEM;
    }
    crypto_memset(buf, buf_size, 0, buf_size);
    W = buf;
    W1 = buf + SM3_W_SIZE;
    T = buf + SM3_W_SIZE + SM3_BLOCK_SIZE;

    for (j = 0; j < SM3_BLOCK_SIZE / WORD_WIDTH; j++) {
        W[j] = crypto_cpu_to_be32(local_block[j]);
    }
    for (j = SM3_BLOCK_SIZE / WORD_WIDTH; j < SM3_W_SIZE; j++) {
        W[j] = p1(W[j - 16] ^ W[j - 9] ^ \  /* soft sm3 alg:offset 16, 9. */
            rotate_left(W[j - 3], 15))  ^ \  /* soft sm3 alg:offset 3, and rotate left 15. */
            rotate_left(W[j - 13], 7)   ^ W[j - 6];  /* soft sm3 alg:offset 13, 6 and rotate left 7. */
    }
    for (j = 0; j < SM3_BLOCK_SIZE; j++) {
        W1[j] = W[j] ^ W[j + 4];    /* soft sm3 alg:offset 4. */
    }

    for (j = 0; j < SM3_BLOCK_SIZE / WORD_WIDTH; j++) {
        T[j] = 0x79CC4519;  /* soft sm3 alg: T init 0x79CC4519. */
        SS1 = rotate_left((rotate_left(A, 12) + E + rotate_left(T[j], j)), 7);     /* soft sm3 alg:rotate left 12, 7. */
        SS2 = SS1 ^ rotate_left(A, 12);  /* soft sm3 alg:rotate left 12. */
        TT1 = ff0(A, B, C) + D + SS2 + W1[j];
        TT2 = gg0(E, F, G) + H + SS1 + W[j];
        D = C;
        C = rotate_left(B, 9);   /* soft sm3 alg:rotate left 9. */
        B = A;
        A = TT1;
        H = G;
        G = rotate_left(F, 19);  /* soft sm3 alg:rotate left 19. */
        F = E;
        E = p0(TT2);
    }

    for (j = SM3_BLOCK_SIZE / WORD_WIDTH; j < SM3_BLOCK_SIZE; j++) {
        T[j] = 0x7A879D8A; /* soft sm3 alg: T init 0x7A879D8A. */
        SS1 = rotate_left((rotate_left(A, 12) + E + rotate_left(T[j], j)), 7); /* soft sm3 alg:rotate left 12, 7. */
        SS2 = SS1 ^ rotate_left(A, 12);  /* soft sm3 alg:rotate left 12. */
        TT1 = ff1(A, B, C) + D + SS2 + W1[j];
        TT2 = gg1(E, F, G) + H + SS1 + W[j];
        D = C;
        C = rotate_left(B, 9);   /* soft sm3 alg:rotate left 9. */
        B = A;
        A = TT1;
        H = G;
        G = rotate_left(F, 19);  /* soft sm3 alg:rotate left 19. */
        F = E;
        E = p0(TT2);
    }

    digest[WORD_IDX_0] ^= A;
    digest[WORD_IDX_1] ^= B;
    digest[WORD_IDX_2] ^= C;
    digest[WORD_IDX_3] ^= D;
    digest[WORD_IDX_4] ^= E;
    digest[WORD_IDX_5] ^= F;
    digest[WORD_IDX_6] ^= G;
    digest[WORD_IDX_7] ^= H;

    if (buf != HI_NULL) {
        crypto_free(buf);
        buf = HI_NULL;
    }

    return HI_SUCCESS;
}

static hi_void sm3_init(ext_sm3_context *ctx)
{
    hi_log_func_enter();

    ctx->state[WORD_IDX_0] = SM3_H0;
    ctx->state[WORD_IDX_1] = SM3_H1;
    ctx->state[WORD_IDX_2] = SM3_H2;
    ctx->state[WORD_IDX_3] = SM3_H3;
    ctx->state[WORD_IDX_4] = SM3_H4;
    ctx->state[WORD_IDX_5] = SM3_H5;
    ctx->state[WORD_IDX_6] = SM3_H6;
    ctx->state[WORD_IDX_7] = SM3_H7;

    hi_log_func_exit();

    return;
}

static hi_s32 sm3_update(ext_sm3_context *ctx, const hi_u8 *data, hi_u32 data_len)
{
    hi_u32 left = 0;
    hi_s32 ret;

    hi_log_func_enter();

    ctx->total += data_len;

    if (ctx->tail_len) {
        left = SM3_BLOCK_SIZE - ctx->tail_len;
        if (data_len < left) {
            crypto_memcpy(ctx->tail + ctx->tail_len, left, data, data_len);
            ctx->tail_len += data_len;
            return HI_SUCCESS;
        } else {
            crypto_memcpy(ctx->tail + ctx->tail_len, left, data, left);
            ret = sm3_compress(ctx->state, ctx->tail, sizeof(ctx->tail));
            if (ret != HI_SUCCESS) {
                hi_log_print_func_err(sm3_compress, ret);
                return ret;
            }

            data += left;
            data_len -= left;
        }
    }

    while (data_len >= SM3_BLOCK_SIZE) {
        ret = sm3_compress(ctx->state, data, SM3_BLOCK_SIZE);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(sm3_compress, ret);
            return ret;
        }

        data += SM3_BLOCK_SIZE;
        data_len -= SM3_BLOCK_SIZE;
    }

    ctx->tail_len = data_len;
    if (data_len) {
        crypto_memcpy(ctx->tail, SM3_BLOCK_SIZE, data, data_len);
    }

    hi_log_func_exit();

    return HI_SUCCESS;
}

static hi_s32 sm3_final(ext_sm3_context *ctx, hi_u8 *digest, hi_u32 digest_len)
{
    hi_s32 i = 0;
    hi_s32 ret;
    hi_u32 idx = 0;
    hi_u32 hash[SM3_RESULT_SIZE_IN_WORD] = {0};

    hi_log_func_enter();
    hi_log_chk_param_return(digest_len < SM3_BLOCK_SIZE);

    ctx->tail[ctx->tail_len] = SM3_BYTE_MSB;
    idx++;

    /* a block is enough */
    if (ctx->tail_len + SM3_PAD_MIN_SIZE <= SM3_BLOCK_SIZE) {
        crypto_memset(ctx->tail + ctx->tail_len + idx, SM3_BLOCK_SIZE - ctx->tail_len - idx, 0,
            SM3_BLOCK_SIZE - ctx->tail_len - idx);
    } else {
        /* 2 block is request */
        crypto_memset(ctx->tail + ctx->tail_len + idx, SM3_BLOCK_SIZE - ctx->tail_len - idx, 0,
            SM3_BLOCK_SIZE - ctx->tail_len - idx);
        ret = sm3_compress(ctx->state, ctx->tail, sizeof(ctx->tail));
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(sm3_compress, ret);
            return ret;
        }
        crypto_memset(ctx->tail, SM3_BLOCK_SIZE, 0, SM3_BLOCK_SIZE);
    }

    /* write 8 bytes fix data length * 8 */
    ctx->tail[SM3_BLOCK_SIZE - WORD_IDX_5] = (hi_u8)((ctx->total >> SHIFT_29BITS) & MAX_LOW_3BITS);
    ctx->tail[SM3_BLOCK_SIZE - WORD_IDX_4] = (hi_u8)((ctx->total >> SHIFT_21BITS) & MAX_LOW_8BITS);
    ctx->tail[SM3_BLOCK_SIZE - WORD_IDX_3] = (hi_u8)((ctx->total >> SHIFT_13BITS) & MAX_LOW_8BITS);
    ctx->tail[SM3_BLOCK_SIZE - WORD_IDX_2] = (hi_u8)((ctx->total >> SHIFT_5BITS)  & MAX_LOW_8BITS);
    ctx->tail[SM3_BLOCK_SIZE - WORD_IDX_1] = (hi_u8)((ctx->total << SHIFT_3BITS)  & MAX_LOW_8BITS);

    ret = sm3_compress(ctx->state, ctx->tail, sizeof(ctx->tail));
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(sm3_compress, ret);
        return ret;
    }

    for (i = 0; i < SM3_RESULT_SIZE_IN_WORD; i++) {
        hash[i] = crypto_cpu_to_be32(ctx->state[i]);
    }

    crypto_memcpy(digest, digest_len, hash, SM3_RESULT_SIZE);

    hi_log_func_exit();

    return HI_SUCCESS;
}

hi_void *ext_sm3_create(hash_mode mode)
{
    ext_sm3_context *ctx = HI_NULL;

    hi_log_func_enter();

    ctx = crypto_malloc(sizeof(ext_sm3_context));
    if (ctx == HI_NULL) {
        hi_log_error("malloc hash context buffer failed!");
        return HI_NULL;
    }
    crypto_memset(ctx, sizeof(ext_sm3_context), 0, sizeof(ext_sm3_context));

    sm3_init(ctx);

    hi_log_func_exit();

    return ctx;
}

hi_s32 ext_sm3_update(hi_void *ctx, hi_u8 *chunk, hi_u32 chunk_len, hash_chunk_src src)
{
    hi_u8 *ptr = HI_NULL;
    hi_s32 ret;
    hi_u32 offset = 0;
    hi_u32 length;

    hi_log_func_enter();

    hi_log_chk_param_return(ctx == HI_NULL);

    if (chunk_len == 0x00) {
        return HI_SUCCESS;
    }

    if (src == HASH_CHUNCK_SRC_USER) {
        ptr = crypto_calloc(HASH_MAX_BUFFER_SIZE);
        if (ptr == HI_NULL) {
            hi_log_print_func_err(crypto_calloc, HI_ERR_CIPHER_FAILED_MEM);
            return HI_ERR_CIPHER_FAILED_MEM;
        }

        while (offset < chunk_len) {
            length = chunk_len - offset;
            if (length > HASH_MAX_BUFFER_SIZE) {
                length = HASH_MAX_BUFFER_SIZE;
            }

            crypto_chk_err_exit(crypto_copy_from_user(ptr, chunk + offset, length));
            crypto_chk_err_exit(sm3_update(ctx, ptr, length));
            crypto_msleep(1);
            offset += length;
        }
    } else {
        if (chunk == HI_NULL) {
            hi_log_error("Invalid point, chunk is null!\n");
            ret = HI_ERR_CIPHER_INVALID_POINT;
            goto exit__;
        }
        crypto_chk_err_exit(sm3_update(ctx, chunk, chunk_len));
        ret = HI_SUCCESS;
    }

exit__:
    if (ptr != HI_NULL) {
        crypto_free(ptr);
        ptr = HI_NULL;
    }

    hi_log_func_exit();
    return ret;
}

hi_s32 ext_sm3_finish(hi_void *ctx,  hi_void *hash, hi_u32 hash_buf_len, hi_u32 *hashlen)
{
    hi_s32 ret;
    hi_log_func_enter();

    hi_log_chk_param_return(ctx == HI_NULL);

    ret = sm3_final(ctx, hash, hash_buf_len);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(sm3_final, ret);
        return ret;
    }
    *hashlen = SM3_RESULT_SIZE;

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 ext_sm3_destory(hi_void *ctx)
{
    hi_log_func_enter();

    hi_log_chk_param_return(ctx == HI_NULL);

    crypto_free(ctx);
    ctx  = HI_NULL;

    hi_log_func_exit();
    return HI_SUCCESS;
}
#endif
