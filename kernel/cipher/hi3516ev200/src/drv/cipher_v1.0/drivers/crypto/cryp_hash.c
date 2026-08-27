/******************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : cryp_hash.c
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include "drv_osal_lib.h"
#include "drv_hash.h"
#include "cryp_hash.h"
#include "ext_alg.h"

/*************************** Internal Structure Definition *******************/
/** \addtogroup      rsa */
/** @{*/  /** <!-- [rsa]*/

/* size of hash physical memory */
#define HASH_PHY_MEM_SIZE             (0x100000)/* 1M */

/* try to create memory for HASH */
#define HASH_PHY_MEM_CREATE_TRY_TIME  (10)

/* block size */
#define SHA1_BLOCK_SIZE            (64)   /* SHA1 */
#define SHA224_BLOCK_SIZE          (64)   /* SHA224 */
#define SHA256_BLOCK_SIZE          (64)   /* SHA256 */
#define SHA384_BLOCK_SIZE          (128)  /* SHA384 */
#define SHA512_BLOCK_SIZE          (128)  /* SHA512 */
#define SM3_BLOCK_SIZE             (64)   /* SM3 */

/* first byte of hash padding */
#define HASH_PADDING_B0               (0x80)

/* Pading size */
#define HASH_BLOCK64_PAD_MIN       (9)    /* 0x80 || Len(64)  */
#define HASH_BLOCK128_PAD_MIN      (17)   /* 0x80 || Len(128) */

/* The max tab size of symc function */
#define HASH_FUNC_TAB_SIZE          (16)

/* SHA-1, the initial hash value, H(0)*/
#define SHA1_H0     0x67452301
#define SHA1_H1     0xefcdab89
#define SHA1_H2     0x98badcfe
#define SHA1_H3     0x10325476
#define SHA1_H4     0xc3d2e1f0

/* SHA-224, the initial hash value, H(0)*/
#define SHA224_H0    0xc1059ed8
#define SHA224_H1    0x367cd507
#define SHA224_H2    0x3070dd17
#define SHA224_H3    0xf70e5939
#define SHA224_H4    0xffc00b31
#define SHA224_H5    0x68581511
#define SHA224_H6    0x64f98fa7
#define SHA224_H7    0xbefa4fa4

/* SHA-256, the initial hash value, H(0)*/
#define SHA256_H0    0x6a09e667
#define SHA256_H1    0xbb67ae85
#define SHA256_H2    0x3c6ef372
#define SHA256_H3    0xa54ff53a
#define SHA256_H4    0x510e527f
#define SHA256_H5    0x9b05688c
#define SHA256_H6    0x1f83d9ab
#define SHA256_H7    0x5be0cd19

/* SHA-384, the initial hash value, H(0)*/
#define SHA384_H0    0xcbbb9d5dc1059ed8ULL
#define SHA384_H1    0x629a292a367cd507ULL
#define SHA384_H2    0x9159015a3070dd17ULL
#define SHA384_H3    0x152fecd8f70e5939ULL
#define SHA384_H4    0x67332667ffc00b31ULL
#define SHA384_H5    0x8eb44a8768581511ULL
#define SHA384_H6    0xdb0c2e0d64f98fa7ULL
#define SHA384_H7    0x47b5481dbefa4fa4ULL

/* SHA-512, the initial hash value, H(0)*/
#define SHA512_H0    0x6a09e667f3bcc908ULL
#define SHA512_H1    0xbb67ae8584caa73bULL
#define SHA512_H2    0x3c6ef372fe94f82bULL
#define SHA512_H3    0xa54ff53a5f1d36f1ULL
#define SHA512_H4    0x510e527fade682d1ULL
#define SHA512_H5    0x9b05688c2b3e6c1fULL
#define SHA512_H6    0x1f83d9abfb41bd6bULL
#define SHA512_H7    0x5be0cd19137e2179ULL

/* SM3, the initial hash value, H(0)*/
#define SM3_H0    0x7380166F
#define SM3_H1    0x4914B2B9
#define SM3_H2    0x172442D7
#define SM3_H3    0xDA8A0600
#define SM3_H4    0xA96F30BC
#define SM3_H5    0x163138AA
#define SM3_H6    0xE38DEE4D
#define SM3_H7    0xB0FB0E4E

/* hash function list */
static hash_func hash_descriptor[HASH_FUNC_TAB_SIZE];

#ifdef CHIP_HASH_SUPPORT
/**
 * \brief          hash context structure
 */
typedef struct {
    hash_mode mode;    /*!<  HASH mode */
    hi_u32 block_size;    /*!<  HASH block size */
    hi_u32 hash_size;     /*!<  HASH result size */
    hi_u32 hard_chn;      /*!<  HASH hardware channel number */
    hi_u8 tail[HASH_BLOCK_SIZE_128 * 2]; /*!<  buffer to store the tail and padding data */
    hi_u32 tail_len;                  /*!<  length of the tail message */
    hi_u32 total;                    /*!<  total length of the message */
    hi_u32 hash[HASH_RESULT_MAX_SIZE_IN_WORD]; /*!<  buffer to store the result */
    crypto_mem mem;    /*!<  DMA memory of hash message */
}
cryp_hash_context;

/* hash dma memory */
static crypto_mem hash_mem;

/** @}*/  /** <!-- ==== Structure Definition end ====*/

/******************************* API Code *****************************/
/** \addtogroup      hash drivers*/
/** @{*/  /** <!-- [hash]*/

static hi_s32 cryp_hash_initial(cryp_hash_context *hisi_ctx, hash_mode mode)
{
#ifndef CHIP_TYPE_hi3516ev200
    hi_u64 H0;
#endif

    HI_LOG_FUNC_ENTER();

    switch (mode) {
        case HASH_MODE_SHA1: {
            hisi_ctx->block_size = SHA1_BLOCK_SIZE;
            hisi_ctx->hash_size = SHA1_RESULT_SIZE;
            hisi_ctx->hash[0] = CPU_TO_BE32(SHA1_H0);
            hisi_ctx->hash[1] = CPU_TO_BE32(SHA1_H1);
            hisi_ctx->hash[2] = CPU_TO_BE32(SHA1_H2);
            hisi_ctx->hash[3] = CPU_TO_BE32(SHA1_H3);
            hisi_ctx->hash[4] = CPU_TO_BE32(SHA1_H4);
            break;
        }
        case HASH_MODE_SHA224: {
            hisi_ctx->hash_size = SHA224_RESULT_SIZE;
            hisi_ctx->block_size = SHA224_BLOCK_SIZE;
            hisi_ctx->hash[0] = CPU_TO_BE32(SHA224_H0);
            hisi_ctx->hash[1] = CPU_TO_BE32(SHA224_H1);
            hisi_ctx->hash[2] = CPU_TO_BE32(SHA224_H2);
            hisi_ctx->hash[3] = CPU_TO_BE32(SHA224_H3);
            hisi_ctx->hash[4] = CPU_TO_BE32(SHA224_H4);
            hisi_ctx->hash[5] = CPU_TO_BE32(SHA224_H5);
            hisi_ctx->hash[6] = CPU_TO_BE32(SHA224_H6);
            hisi_ctx->hash[7] = CPU_TO_BE32(SHA224_H7);
            break;
        }
        case HASH_MODE_SHA256: {
            hisi_ctx->hash_size = SHA256_RESULT_SIZE;
            hisi_ctx->block_size = SHA256_BLOCK_SIZE;
            hisi_ctx->hash[0] = CPU_TO_BE32(SHA256_H0);
            hisi_ctx->hash[1] = CPU_TO_BE32(SHA256_H1);
            hisi_ctx->hash[2] = CPU_TO_BE32(SHA256_H2);
            hisi_ctx->hash[3] = CPU_TO_BE32(SHA256_H3);
            hisi_ctx->hash[4] = CPU_TO_BE32(SHA256_H4);
            hisi_ctx->hash[5] = CPU_TO_BE32(SHA256_H5);
            hisi_ctx->hash[6] = CPU_TO_BE32(SHA256_H6);
            hisi_ctx->hash[7] = CPU_TO_BE32(SHA256_H7);
            break;
        }
#ifndef CHIP_TYPE_hi3516ev200
        case HASH_MODE_SHA384: {
            hisi_ctx->hash_size = SHA384_RESULT_SIZE;
            hisi_ctx->block_size = SHA384_BLOCK_SIZE;
            H0 = CPU_TO_BE64(SHA384_H0);
            crypto_memcpy(&hisi_ctx->hash[0], DOUBLE_WORD_WIDTH, &H0, sizeof(H0));
            H0 = CPU_TO_BE64(SHA384_H1);
            crypto_memcpy(&hisi_ctx->hash[2], DOUBLE_WORD_WIDTH, &H0, sizeof(H0));
            H0 = CPU_TO_BE64(SHA384_H2);
            crypto_memcpy(&hisi_ctx->hash[4], DOUBLE_WORD_WIDTH, &H0, sizeof(H0));
            H0 = CPU_TO_BE64(SHA384_H3);
            crypto_memcpy(&hisi_ctx->hash[6], DOUBLE_WORD_WIDTH, &H0, sizeof(H0));
            H0 = CPU_TO_BE64(SHA384_H4);
            crypto_memcpy(&hisi_ctx->hash[8], DOUBLE_WORD_WIDTH, &H0, sizeof(H0));
            H0 = CPU_TO_BE64(SHA384_H5);
            crypto_memcpy(&hisi_ctx->hash[10], DOUBLE_WORD_WIDTH, &H0, sizeof(H0));
            H0 = CPU_TO_BE64(SHA384_H6);
            crypto_memcpy(&hisi_ctx->hash[12], DOUBLE_WORD_WIDTH, &H0, sizeof(H0));
            H0 = CPU_TO_BE64(SHA384_H7);
            crypto_memcpy(&hisi_ctx->hash[14], DOUBLE_WORD_WIDTH, &H0, sizeof(H0));
            break;
        }
        case HASH_MODE_SHA512: {
            hisi_ctx->hash_size = SHA512_RESULT_SIZE;
            hisi_ctx->block_size = SHA512_BLOCK_SIZE;
            H0 = CPU_TO_BE64(SHA512_H0);
            crypto_memcpy(&hisi_ctx->hash[0], DOUBLE_WORD_WIDTH, &H0, sizeof(H0));
            H0 = CPU_TO_BE64(SHA512_H1);
            crypto_memcpy(&hisi_ctx->hash[2], DOUBLE_WORD_WIDTH, &H0, sizeof(H0));
            H0 = CPU_TO_BE64(SHA512_H2);
            crypto_memcpy(&hisi_ctx->hash[4], DOUBLE_WORD_WIDTH, &H0, sizeof(H0));
            H0 = CPU_TO_BE64(SHA512_H3);
            crypto_memcpy(&hisi_ctx->hash[6], DOUBLE_WORD_WIDTH, &H0, sizeof(H0));
            H0 = CPU_TO_BE64(SHA512_H4);
            crypto_memcpy(&hisi_ctx->hash[8], DOUBLE_WORD_WIDTH, &H0, sizeof(H0));
            H0 = CPU_TO_BE64(SHA512_H5);
            crypto_memcpy(&hisi_ctx->hash[10], DOUBLE_WORD_WIDTH, &H0, sizeof(H0));
            H0 = CPU_TO_BE64(SHA512_H6);
            crypto_memcpy(&hisi_ctx->hash[12], DOUBLE_WORD_WIDTH, &H0, sizeof(H0));
            H0 = CPU_TO_BE64(SHA512_H7);
            crypto_memcpy(&hisi_ctx->hash[14], DOUBLE_WORD_WIDTH, &H0, sizeof(H0));
            break;
        }
#endif
        case HASH_MODE_SM3: {
            hisi_ctx->hash_size = SM3_RESULT_SIZE;
            hisi_ctx->block_size = SM3_BLOCK_SIZE;
            hisi_ctx->hash[0] = CPU_TO_BE32(SM3_H0);
            hisi_ctx->hash[1] = CPU_TO_BE32(SM3_H1);
            hisi_ctx->hash[2] = CPU_TO_BE32(SM3_H2);
            hisi_ctx->hash[3] = CPU_TO_BE32(SM3_H3);
            hisi_ctx->hash[4] = CPU_TO_BE32(SM3_H4);
            hisi_ctx->hash[5] = CPU_TO_BE32(SM3_H5);
            hisi_ctx->hash[6] = CPU_TO_BE32(SM3_H6);
            hisi_ctx->hash[7] = CPU_TO_BE32(SM3_H7);
            break;
        }
        default: {
            HI_LOG_ERROR("Invalid hash mode, mode = 0x%x.\n", mode);
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

/**
* \brief          Create DMA memory of HASH message
*/
static hi_s32 cryp_hash_create_mem(hi_void)
{
    hi_s32 ret = HI_FAILURE;
    hi_u32 i = 0;
    hi_u32 length = HASH_PHY_MEM_SIZE;

    HI_LOG_FUNC_ENTER();

    crypto_memset(&hash_mem, sizeof(hash_mem), 0x00, sizeof(hash_mem));

    /* Try to alloc memory, halve the length if failed */
    for (i = 0; i < HASH_PHY_MEM_CREATE_TRY_TIME; i++) {
        ret = hash_mem_create(&hash_mem, SEC_MMZ, "hash_msg_dma", length);
        if (ret == HI_SUCCESS) {
            return HI_SUCCESS;
        } else {
            /* halve the length */
            length /= 0x02;
        }
    }

    HI_LOG_FUNC_EXIT();
    return HI_ERR_CIPHER_FAILED_MEM;
}

static hi_s32 cryp_hash_destory_mem(hi_void)
{
    hi_s32 ret = HI_FAILURE;

    ret = hash_mem_destory(&hash_mem);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(crypto_mem_destory, ret);
        return ret;
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

static hi_s32 cryp_hash_chunk_copy(const hi_void *chunk, hi_void *dma, hi_u32 len, hash_chunk_src src)
{
    hi_s32 ret = HI_FAILURE;

    HI_LOG_FUNC_ENTER();

    /* Don't process the empty message */
    if (len == 0x00) {
        HI_LOG_FUNC_EXIT();
        return HI_SUCCESS;
    }

    HI_LOG_CHECK_PARAM(chunk == HI_NULL);
    HI_LOG_CHECK_PARAM(dma   == HI_NULL);

    if (src == HASH_CHUNCK_SRC_LOCAL) {
        crypto_memcpy(dma, len, chunk, len);
    } else {
        ret = crypto_copy_from_user(dma, chunk, len);
        if (ret != HI_SUCCESS) {
            HI_LOG_PRINT_FUNC_ERR(crypto_copy_from_user, ret);
            return ret;
        }
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

/* hash hardware computation */
static hi_s32 cryp_hash_process(cryp_hash_context *hisi_ctx,
                             hi_u8 *msg, hi_u32 length,
                             hash_chunk_src src)
{
    hi_s32 ret = HI_FAILURE;
    hi_void *buf = HI_NULL;
    hi_u32 left = 0, size = 0, max = 0;

    HI_LOG_FUNC_ENTER();

    /* Don't process the empty message */
    if (length == 0x00) {
        HI_LOG_FUNC_EXIT();
        return HI_SUCCESS;
    }

    HI_LOG_DEBUG("length 0x%x, dma_size 0x%x\n", length, hisi_ctx->mem.dma_size);

    /* get dma buffer */
    buf = crypto_mem_virt(&hisi_ctx->mem);
    left = length;

    /* align at block size */
    max  = hisi_ctx->mem.dma_size / hisi_ctx->block_size;
    max *= hisi_ctx->block_size;

    while (left > 0) {
        if (left <= max) {
            /* left size less than dma buffer,
             * can process all left mesaage
             */
            size = left;
        } else {
            /* left size large than dma buffer,
             * just process mesaage with dma size
             */
            size = max;
        }

        HI_LOG_DEBUG("msg 0x%p, size 0x%x, left 0x%x, max 0x%x\n", msg, size, left, max);

        /* copy message to dma buffer */
        ret = cryp_hash_chunk_copy(msg, buf, size, src);
        if (ret != HI_SUCCESS) {
            HI_LOG_PRINT_FUNC_ERR(cryp_hash_chunk_copy, ret);
            goto exit;
        }

        /* configure mode */
        ret = drv_hash_config(hisi_ctx->hard_chn, hisi_ctx->mode, hisi_ctx->hash);
        if (ret != HI_SUCCESS) {
            HI_LOG_PRINT_FUNC_ERR(drv_hash_config, ret);
            goto exit;
        }

        /* start */
        ret = drv_hash_start(hisi_ctx->hard_chn, &hisi_ctx->mem, size);
        if (ret != HI_SUCCESS) {
            HI_LOG_PRINT_FUNC_ERR(drv_hash_start, ret);
            goto exit;
        }

        /* wait done */
        ret = drv_hash_wait_done(hisi_ctx->hard_chn, hisi_ctx->hash);
        if (ret != HI_SUCCESS) {
            HI_LOG_PRINT_FUNC_ERR(drv_hash_wait_done, ret);
            break;
        }

        /* re-compute left length */
        left -= size;
        msg += size;
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;

exit:
    return ret;
}

/* hash message paading to align at block size */
static hi_u32 cryp_hash_pading(hi_u32 block_size, hi_u8 *msg, hi_u32 tail_size, hi_u32 total)
{
    hi_u32 pad_len = 0, min = 0;

    HI_LOG_FUNC_ENTER();

    /* get min padding size */
    if (block_size == HASH_BLOCK_SIZE_64) {
        min = HASH_BLOCK64_PAD_MIN;
    } else {
        min = HASH_BLOCK128_PAD_MIN;
    }

    pad_len = block_size - (total % block_size);

    /* if pad len less than min value, add a block */
    if (pad_len < min) {
        pad_len += block_size;
    }

    /* Format(binary): {data|| 0x80 || 00 00 ... || Len(64)} */
    crypto_memset(&msg[tail_size], HASH_BLOCK_SIZE_128 * 2 - tail_size, 0, pad_len);
    msg[tail_size] = HASH_PADDING_B0;
    tail_size += pad_len - 8;

    /* write 8 bytes fix data length * 8 */
    msg[tail_size++] = 0x00;
    msg[tail_size++] = 0x00;
    msg[tail_size++] = 0x00;
    msg[tail_size++] = (hi_u8)((total >> 29) & 0x07);
    msg[tail_size++] = (hi_u8)((total >> 21) & 0xff);
    msg[tail_size++] = (hi_u8)((total >> 13) & 0xff);
    msg[tail_size++] = (hi_u8)((total >> 5)  & 0xff);
    msg[tail_size++] = (hi_u8)((total << 3)  & 0xff);

    HI_LOG_FUNC_EXIT();

    return tail_size;
}

static hi_void *cryp_hash_create(hash_mode mode)
{
    hi_s32 ret = HI_FAILURE;
    cryp_hash_context *hisi_ctx = HI_NULL;

    HI_LOG_FUNC_ENTER();

    hisi_ctx = crypto_calloc(1, sizeof(cryp_hash_context));
    if (hisi_ctx == HI_NULL) {
        HI_LOG_ERROR("malloc hash context buffer failed!");
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_FAILED_MEM);
        return HI_NULL;
    }

    hisi_ctx->mode = mode;
    hisi_ctx->hard_chn = HASH_HARD_CHANNEL;

    ret = cryp_hash_initial(hisi_ctx, mode);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(cryp_hash_initial, ret);
        goto error1;
    }

    crypto_memcpy(&hisi_ctx->mem, sizeof(crypto_mem), &hash_mem, sizeof(crypto_mem));

    HI_LOG_FUNC_EXIT();

    return hisi_ctx;

error1:
    crypto_free(hisi_ctx);
    hisi_ctx = HI_NULL;

    return HI_NULL;
}

static hi_s32 cryp_hash_destory(hi_void *ctx)
{
    cryp_hash_context *hisi_ctx = ctx;

    HI_LOG_FUNC_ENTER();
    if (hisi_ctx == HI_NULL) {
        HI_LOG_FUNC_EXIT();
        return HI_SUCCESS;
    }

    drv_hash_reset(hisi_ctx->hard_chn);
    crypto_free(ctx);
    ctx = HI_NULL;

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

static hi_s32 cryp_hash_update(hi_void *ctx, hi_u8 *chunk, hi_u32 chunkLen, hash_chunk_src src)
{
    hi_s32 ret = HI_FAILURE;
    cryp_hash_context *hisi_ctx = ctx;
    hi_u32 inverse_len = 0;

    HI_LOG_FUNC_ENTER();
    HI_LOG_CHECK_PARAM(hisi_ctx == HI_NULL);

    HI_LOG_DEBUG("last: total 0x%x, block size %d, tail_len %d, chunkLen 0x%x, src %d\n", hisi_ctx->total,
                 hisi_ctx->block_size, hisi_ctx->tail_len, chunkLen, src);

    /* total len */
    hisi_ctx->total += chunkLen;

    /* left tail len to make up a block */
    inverse_len = hisi_ctx->block_size - hisi_ctx->tail_len;

    if (chunkLen  < inverse_len) {
        /* can't make up a block */
        inverse_len = chunkLen;
    }

    /* try to make up the tail data to be a block */
    ret = cryp_hash_chunk_copy(chunk,
                               hisi_ctx->tail + hisi_ctx->tail_len,
                               inverse_len,
                               src);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(cryp_hash_chunk_copy, ret);
        return ret;
    }
    hisi_ctx->tail_len += inverse_len;
    chunk += inverse_len;
    chunkLen -= inverse_len;  /* the chunkLen may be zero */

    HI_LOG_DEBUG("new: total 0x%x, tail_len %d, chunkLen 0x%x\n", hisi_ctx->total,
                 hisi_ctx->tail_len, chunkLen);

    /* process tail block */
    if (hisi_ctx->tail_len == hisi_ctx->block_size) {
        ret = cryp_hash_process(hisi_ctx,
                                hisi_ctx->tail,
                                hisi_ctx->tail_len,
                                HASH_CHUNCK_SRC_LOCAL);
        if (ret != HI_SUCCESS) {
            HI_LOG_PRINT_FUNC_ERR(cryp_hash_process, ret);
            return ret;
        }
        /* new tail len */
        hisi_ctx->tail_len = chunkLen % hisi_ctx->block_size;

        /* new chunk len align at block size */
        chunkLen -= hisi_ctx->tail_len;

        /* save new tail */
        ret = cryp_hash_chunk_copy(chunk + chunkLen,
                                   hisi_ctx->tail,
                                   hisi_ctx->tail_len,
                                   src);
        if (ret != HI_SUCCESS) {
            HI_LOG_PRINT_FUNC_ERR(cryp_hash_chunk_copy, ret);
            return ret;
        }
    }

    HI_LOG_DEBUG("new: total 0x%x, tail_len %d, chunkLen 0x%x\n", hisi_ctx->total,
                 hisi_ctx->tail_len, chunkLen);

    /* process left block, just resurn HI_SUCCESS if the chunkLen is zero */
    ret = cryp_hash_process(hisi_ctx, chunk, chunkLen, src);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(cryp_hash_process, ret);
        return ret;
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

static hi_s32 cryp_hash_finish(hi_void *ctx,  hi_void *hash, hi_u32 *hashlen)
{
    hi_s32 ret = HI_FAILURE;
    cryp_hash_context *hisi_ctx = ctx;
    hi_u32 left = 0;

    HI_LOG_FUNC_ENTER();
    HI_LOG_CHECK_PARAM(hisi_ctx == HI_NULL);

    /* padding message */
    left = cryp_hash_pading(hisi_ctx->block_size,
                            hisi_ctx->tail,
                            hisi_ctx->tail_len,
                            hisi_ctx->total);

    ret = cryp_hash_process(hisi_ctx, hisi_ctx->tail, left, HASH_CHUNCK_SRC_LOCAL);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(cryp_hash_process, ret);
        return ret;
    }

    crypto_memcpy(hash,
                  HASH_RESULT_MAX_SIZE,
                  hisi_ctx->hash,
                  hisi_ctx->hash_size);
    *hashlen = hisi_ctx->hash_size;

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}
#endif

static hi_s32 func_register_hash(hash_func *func)
{
    hi_u32 i = 0;

    HI_LOG_FUNC_ENTER();

    /* check availability */
    if ((func->create  == HI_NULL)
        || (func->destroy == HI_NULL)
        || (func->update == HI_NULL)
        || (func->finish == HI_NULL)) {
        HI_LOG_ERROR("hash function is null.\n");
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    HI_LOG_DEBUG("register hash mode %d\n", func->mode);

    /* is it already registered? */
    for (i = 0; i < HASH_FUNC_TAB_SIZE; i++) {
        if (hash_descriptor[i].valid && hash_descriptor[i].mode == func->mode) {
            HI_LOG_FUNC_EXIT();
            return HI_SUCCESS;
        }
    }

    /* find a blank spot */
    for (i = 0; i < HASH_FUNC_TAB_SIZE; i++) {
        if (!hash_descriptor[i].valid) {
            hash_descriptor[i] = *func;
            hash_descriptor[i].valid = HI_TRUE;
            HI_LOG_FUNC_EXIT();
            return HI_SUCCESS;
        }
    }

    return HI_ERR_CIPHER_INVALID_PARA;
}

/* hash function register */
static hi_void cryp_register_hash_default(hi_u32 capacity, hash_mode mode, hi_u32 blocksize, hi_u32 hashlen)
{
    hash_func func;

    crypto_memset(&func, sizeof(func), 0 , sizeof(func));

    /* register the hash function if supported */
    if (capacity) {
#ifdef CHIP_HASH_SUPPORT
        func.mode = mode;
        func.block_size = blocksize;
        func.size = hashlen;
        func.create = cryp_hash_create;
        func.destroy = cryp_hash_destory;
        func.update = cryp_hash_update;
        func.finish = cryp_hash_finish;
        func_register_hash(&func);
#endif
    } else if (mode == HASH_MODE_SM3) {
#if defined(SOFT_SM3_SUPPORT)
        func.mode = mode;
        func.block_size = blocksize;
        func.size = hashlen;
        func.create = ext_sm3_create;
        func.destroy = ext_sm3_destory;
        func.update = ext_sm3_update;
        func.finish = ext_sm3_finish;
        func_register_hash(&func);
#endif
    } else {
#if defined(SOFT_SHA1_SUPPORT) \
    || defined(SOFT_SHA256_SUPPORT) \
    || defined(SOFT_SHA512_SUPPORT)
        func.mode = mode;
        func.block_size = blocksize;
        func.size = hashlen;
        func.create = mbedtls_hash_create;
        func.destroy = mbedtls_hash_destory;
        func.update = mbedtls_hash_update;
        func.finish = mbedtls_hash_finish;
        func_register_hash(&func);
#endif
    }
    return;
}

/* hash function register */
static hi_void cryp_register_all_hash(hi_void)
{
    hash_capacity capacity;

    crypto_memset(&capacity, sizeof(capacity), 0, sizeof(capacity));

#ifdef CHIP_HASH_SUPPORT
    /* get hash capacity */
    drv_hash_get_capacity(&capacity);
#endif

    cryp_register_hash_default(capacity.sha1, HASH_MODE_SHA1, SHA1_BLOCK_SIZE, SHA1_RESULT_SIZE);
    cryp_register_hash_default(capacity.sha224, HASH_MODE_SHA224, SHA224_BLOCK_SIZE, SHA224_RESULT_SIZE);
    cryp_register_hash_default(capacity.sha256, HASH_MODE_SHA256, SHA256_BLOCK_SIZE, SHA256_RESULT_SIZE);
    cryp_register_hash_default(capacity.sha384, HASH_MODE_SHA384, SHA384_BLOCK_SIZE, SHA384_RESULT_SIZE);
    cryp_register_hash_default(capacity.sha512, HASH_MODE_SHA512, SHA512_BLOCK_SIZE, SHA512_RESULT_SIZE);
    cryp_register_hash_default(capacity.sm3, HASH_MODE_SM3, SM3_BLOCK_SIZE, SM3_RESULT_SIZE);

    return;
}

hash_func *cryp_get_hash(hi_u32 mode)
{
    hi_u32 i = 0;
    hash_func *template = HI_NULL;

    HI_LOG_FUNC_ENTER();

    /* find the valid function */
    for (i = 0; i < HASH_FUNC_TAB_SIZE; i++) {
        if (hash_descriptor[i].valid) {
            if (hash_descriptor[i].mode == mode) {
                template = &hash_descriptor[i];
                break;
            }
        }
    }

    HI_LOG_FUNC_EXIT();
    return template;
}

hi_s32 cryp_hash_init(hi_void)
{
    HI_LOG_FUNC_ENTER();

    crypto_memset(hash_descriptor, sizeof(hash_descriptor), 0, sizeof(hash_descriptor));

#ifdef CHIP_HASH_SUPPORT
    {
        hi_s32 ret = HI_FAILURE;

        ret = drv_hash_init();
        if (ret != HI_SUCCESS) {
            HI_LOG_PRINT_FUNC_ERR(drv_hash_init, ret);
            return ret;
        }

        ret = cryp_hash_create_mem();
        if (ret != HI_SUCCESS) {
            HI_LOG_PRINT_FUNC_ERR(cryp_hash_create_mem, ret);
            drv_hash_deinit();
            return ret;
        }
    }
#endif

    /* hash function register */
    cryp_register_all_hash();

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_void cryp_hash_deinit(hi_void)
{
    HI_LOG_FUNC_ENTER();

#ifdef CHIP_HASH_SUPPORT
    drv_hash_deinit();
    cryp_hash_destory_mem();
#endif

    HI_LOG_FUNC_EXIT();
}

/** @}*/  /** <!-- ==== API Code end ====*/
