/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "drv_hash.h"
#include "drv_inner.h"

#include "hal_hash.h"

#include "crypto_drv_common.h"

#define CRYPTO_INVALID_CHN_NUM          (0xFFFFFFFF)
#define HASH_COMPAT_ERRNO(err_code)     DRV_COMPAT_ERRNO(ERROR_MODULE_HASH, err_code)
#define hash_null_ptr_chk(ptr)   \
    crypto_chk_return((ptr) == TD_NULL, HASH_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), #ptr" is NULL\n")

#define CRYPTO_DRV_HASH_MAX_DMA_SIZE    (CRYPTO_HASH_DRV_BUFFER_SIZE * CRYPTO_HASH_HARD_CHN_CNT)
static td_u8 *g_drv_dma_buf = TD_NULL;

typedef struct {
    td_u32 length[2];
    td_u32 state[CRYPTO_HASH_RESULT_SIZE_MAX_IN_WORD];
    td_u8 o_key_pad[CRYPTO_HASH_BLOCK_SIZE_MAX];
    td_u8 i_key_pad[CRYPTO_HASH_BLOCK_SIZE_MAX];
    td_u8 tail[CRYPTO_HASH_BLOCK_SIZE_MAX];
    td_u32 tail_len;
    td_u8 *data_buffer;
    td_u32 data_buffer_len;
} crypto_hash_ctx;

typedef struct {
    td_bool open;
    td_bool is_keyslot;
    crypto_hash_type hash_type;
    crypto_hash_ctx hash_ctx;
} hal_hash_channel_context;

static hal_hash_channel_context g_hash_channel_ctx_list[CRYPTO_HASH_HARD_CHN_CNT];

// SHA-256, the initial hash value
static const td_u8 g_sha256_ival[32] = {
    0x6A, 0x09, 0xE6, 0x67,
    0xBB, 0x67, 0xAE, 0x85,
    0x3C, 0x6E, 0xF3, 0x72,
    0xA5, 0x4F, 0xF5, 0x3A,
    0x51, 0x0E, 0x52, 0x7F,
    0x9B, 0x05, 0x68, 0x8C,
    0x1F, 0x83, 0xD9, 0xAB,
    0x5B, 0xE0, 0xCD, 0x19
};

// SHA-384, the initial hash value
static const td_u8 g_sha384_ival[64] = {
    0xCB, 0xBB, 0x9D, 0x5D, 0xC1, 0x05, 0x9E, 0xD8,
    0x62, 0x9A, 0x29, 0x2A, 0x36, 0x7C, 0xD5, 0x07,
    0x91, 0x59, 0x01, 0x5A, 0x30, 0x70, 0xDD, 0x17,
    0x15, 0x2F, 0xEC, 0xD8, 0xF7, 0x0E, 0x59, 0x39,
    0x67, 0x33, 0x26, 0x67, 0xFF, 0xC0, 0x0B, 0x31,
    0x8E, 0xB4, 0x4A, 0x87, 0x68, 0x58, 0x15, 0x11,
    0xDB, 0x0C, 0x2E, 0x0D, 0x64, 0xF9, 0x8F, 0xA7,
    0x47, 0xB5, 0x48, 0x1D, 0xBE, 0xFA, 0x4F, 0xA4
};

// SHA-512, the initial hash value
static const td_u8 g_sha512_ival[64] = {
    0x6A, 0x09, 0xE6, 0x67, 0xF3, 0xBC, 0xC9, 0x08,
    0xBB, 0x67, 0xAE, 0x85, 0x84, 0xCA, 0xA7, 0x3B,
    0x3C, 0x6E, 0xF3, 0x72, 0xFE, 0x94, 0xF8, 0x2B,
    0xA5, 0x4F, 0xF5, 0x3A, 0x5F, 0x1D, 0x36, 0xF1,
    0x51, 0x0E, 0x52, 0x7F, 0xAD, 0xE6, 0x82, 0xD1,
    0x9B, 0x05, 0x68, 0x8C, 0x2B, 0x3E, 0x6C, 0x1F,
    0x1F, 0x83, 0xD9, 0xAB, 0xFB, 0x41, 0xBD, 0x6B,
    0x5B, 0xE0, 0xCD, 0x19, 0x13, 0x7E, 0x21, 0x79
};

// SM3, the initial hash value
static const td_u8 g_sm3_ival[32] = {
    0x73, 0x80, 0x16, 0x6F,
    0x49, 0x14, 0xB2, 0xB9,
    0x17, 0x24, 0x42, 0xD7,
    0xDA, 0x8A, 0x06, 0x00,
    0xA9, 0x6F, 0x30, 0xBC,
    0x16, 0x31, 0x38, 0xAA,
    0xE3, 0x8D, 0xEE, 0x4D,
    0xB0, 0xFB, 0x0E, 0x4E
};

static td_void inner_drv_hash_length_add(td_u32 length[2], td_u32 addition)
{
    td_u32 diff = 0;
    if (length[1] > length[1] + addition) {
        length[0]++;
        diff = 0xFFFFFFFF - length[1] + 1;
        length[1] = addition - diff;
        return;
    }
    length[1] += addition;
}

static td_s32 drv_hash_get_state(crypto_hash_type hash_type, td_u32 *state, td_u32 state_size)
{
    td_s32 ret = TD_SUCCESS;
    td_u32 hash_alg = CRYPTO_HASH_GET_ALG(hash_type);
    td_u32 hash_mode = CRYPTO_HASH_GET_MODE(hash_type);

    switch (hash_mode) {
        case CRYPTO_HASH_MODE_256: {
            if (hash_alg == CRYPTO_HASH_ALG_SHA2) {
                ret = memcpy_s(state, state_size, g_sha256_ival, sizeof(g_sha256_ival));
                crypto_chk_return(ret != EOK, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");
            } else if (hash_alg == CRYPTO_HASH_ALG_SM3) {
                ret = memcpy_s(state, state_size, g_sm3_ival, sizeof(g_sm3_ival));
                crypto_chk_return(ret != EOK, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");
            } else {
                crypto_log_err("Invalid Hash Alg!\n");
                ret = HASH_COMPAT_ERRNO(ERROR_INVALID_PARAM);
            }
            break;
        }
        case CRYPTO_HASH_MODE_384: {
            ret = memcpy_s(state, state_size, g_sha384_ival, sizeof(g_sha384_ival));
            crypto_chk_return(ret != EOK, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");
            break;
        }
        case CRYPTO_HASH_MODE_512: {
            ret = memcpy_s(state, state_size, g_sha512_ival, sizeof(g_sha512_ival));
            crypto_chk_return(ret != EOK, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");
            break;
        }
        default: {
            crypto_log_err("Invalid Hash Mode!\n");
            ret = HASH_COMPAT_ERRNO(ERROR_INVALID_PARAM);
            break;
        }
    }
    return ret;
}

td_s32 drv_cipher_hash_init(td_void)
{
    td_s32 ret = TD_SUCCESS;

    crypto_drv_func_enter();

    ret = hal_cipher_hash_init();
    crypto_chk_return(ret != CRYPTO_SUCCESS, ret, "hal_cipher_hash_init failed\n");

    g_drv_dma_buf = crypto_malloc_mmz(CRYPTO_DRV_HASH_MAX_DMA_SIZE, "cipher_hash_drv");
    if (g_drv_dma_buf == TD_NULL) {
        crypto_log_err("crypto_malloc_mmz failed\n");
        hal_cipher_hash_deinit();
        return HASH_COMPAT_ERRNO(ERROR_MALLOC);
    }

    crypto_drv_func_exit();
    return ret;
}

td_s32 drv_cipher_hash_deinit(td_void)
{
    td_u32 i;
    hal_hash_channel_context *ctx = TD_NULL;
    crypto_drv_func_enter();

    for (i = 0; i < CRYPTO_HASH_HARD_CHN_CNT; i++) {
        ctx = &g_hash_channel_ctx_list[i];
        if (ctx->open == TD_FALSE) {
            continue;
        }
        (td_void)drv_cipher_hash_destroy(i);
    }

    if (g_drv_dma_buf != TD_NULL) {
        (td_void)memset_s(g_drv_dma_buf, CRYPTO_DRV_HASH_MAX_DMA_SIZE, 0, CRYPTO_DRV_HASH_MAX_DMA_SIZE);
        crypto_free_coherent(g_drv_dma_buf);
        g_drv_dma_buf = TD_NULL;
    }
    hal_cipher_hash_deinit();
    crypto_drv_func_exit();
    return TD_SUCCESS;
}

static td_s32 drv_hash_lock_chn(td_u32 *chn_num)
{
    td_u32 i;
    td_s32 ret = TD_SUCCESS;
    *chn_num = CRYPTO_INVALID_CHN_NUM;
    /* Lock one free Hash Hard Channel. */
    for (i = 0; i < CRYPTO_HASH_HARD_CHN_CNT; i++) {
        if (g_hash_channel_ctx_list[i].open == TD_TRUE) {
            continue;
        }

        ret = hal_hash_lock(i);
        if (ret == TD_SUCCESS) {
            *chn_num = i;
            break;
        }
    }

    if (*chn_num == CRYPTO_INVALID_CHN_NUM) {
        crypto_log_err("Hash Channel is Busy\n");
        return HASH_COMPAT_ERRNO(ERROR_CHN_BUSY);
    }
    return TD_SUCCESS;
}

static td_s32 inner_drv_hmac_start(td_u32 chn_num, hal_hash_channel_context *ctx, const crypto_hash_attr *hash_attr)
{
    td_s32 ret = TD_SUCCESS;
    td_u32 i;
    td_u32 block_size = CRYPTO_HASH_GET_BLOCK_SIZE(hash_attr->hash_type) / CRYPTO_BITS_IN_BYTE;
    if (hash_attr->key_len != 0) {
        ret = memcpy_s(ctx->hash_ctx.tail, sizeof(ctx->hash_ctx.tail), hash_attr->key, hash_attr->key_len);
        crypto_chk_return(ret != EOK, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");
    }

    /* Calc o_key_pad and i_key_pad */
    for (i = 0; i < block_size; i++) {
        ctx->hash_ctx.o_key_pad[i] = 0x5c ^ ctx->hash_ctx.tail[i];
        ctx->hash_ctx.i_key_pad[i] = 0x36 ^ ctx->hash_ctx.tail[i];
    }
    ret = memcpy_s(ctx->hash_ctx.data_buffer, ctx->hash_ctx.data_buffer_len, ctx->hash_ctx.i_key_pad, block_size);
    crypto_chk_return(ret != EOK, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    inner_drv_hash_length_add(ctx->hash_ctx.length, block_size * CRYPTO_BITS_IN_BYTE);

    crypto_cache_flush((uintptr_t)ctx->hash_ctx.data_buffer, block_size);
    ret = hal_cipher_hash_add_in_node(chn_num, crypto_get_phys_addr(ctx->hash_ctx.data_buffer), block_size,
        IN_NODE_TYPE_FIRST | IN_NODE_TYPE_LAST);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_hash_add_in_node failed\n");

    ret = hal_cipher_hash_start(chn_num, TD_FALSE);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_hash_start failed\n");

    ret = hal_cipher_hash_wait_done(chn_num, ctx->hash_ctx.state, sizeof(ctx->hash_ctx.state));
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_hash_wait_done failed\n");

    return ret;
}

td_s32 drv_cipher_hash_start(td_handle *drv_hash_handle, const crypto_hash_attr *hash_attr)
{
    td_u32 chn_num = CRYPTO_INVALID_CHN_NUM;
    td_s32 ret = TD_FAILURE;
    hal_hash_channel_context *ctx = TD_NULL;
    crypto_drv_func_enter();

    ret = inner_hash_start_param_chk(drv_hash_handle, hash_attr);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    crypto_chk_return(crypto_hash_support(hash_attr->hash_type) == TD_FALSE, HASH_COMPAT_ERRNO(ERROR_UNSUPPORT),
        "alg is unsupport\n");
    if (hash_attr->hash_type == CRYPTO_HASH_TYPE_SM3 || hash_attr->hash_type == CRYPTO_HASH_TYPE_HMAC_SM3) {
        crypto_chk_return(crypto_sm_support(CRYPTO_SM_ALG_SM3) == TD_FALSE, HASH_COMPAT_ERRNO(ERROR_UNSUPPORT),
            "alg is unsupport\n");
    }

    /* Lock one free Hash Hard Channel. */
    ret = drv_hash_lock_chn(&chn_num);
    if (ret != TD_SUCCESS) {
        crypto_log_err("drv_hash_lock_chn failed\n");
        return ret;
    }
    crypto_chk_return(g_drv_dma_buf == TD_NULL, HASH_COMPAT_ERRNO(ERROR_UNEXPECTED), "g_drv_dma_buf == TD_NULL\n");
    ctx = &g_hash_channel_ctx_list[chn_num];
    (td_void)memset_s(ctx, sizeof(hal_hash_channel_context), 0, sizeof(hal_hash_channel_context));

    /* Alloc data_buffer. */
    ctx->hash_ctx.data_buffer = g_drv_dma_buf + CRYPTO_HASH_DRV_BUFFER_SIZE * chn_num;
    ctx->hash_ctx.data_buffer_len = CRYPTO_HASH_DRV_BUFFER_SIZE;
    (td_void)memset_s(ctx->hash_ctx.data_buffer, ctx->hash_ctx.data_buffer_len, 0, ctx->hash_ctx.data_buffer_len);
    ctx->hash_type = hash_attr->hash_type;
    /* Set Config. */
    ret = drv_hash_get_state(hash_attr->hash_type, ctx->hash_ctx.state, sizeof(ctx->hash_ctx.state));
    crypto_chk_goto(ret != CRYPTO_SUCCESS, error_hash_unlock, "drv_hash_get_state failed\n");

    ret = hal_cipher_hash_config(chn_num, hash_attr->hash_type, ctx->hash_ctx.state);
    crypto_chk_goto(ret != CRYPTO_SUCCESS, error_hash_unlock, "hal_cipher_hash_config failed\n");

    if (CRYPTO_HASH_IS_HMAC(hash_attr->hash_type) == TD_TRUE) {
        if (hash_attr->is_keyslot) {
            ret = hal_cipher_hash_attach(chn_num, hash_attr->drv_keyslot_handle);
            crypto_chk_goto(ret != CRYPTO_SUCCESS, error_hash_unlock, "hal_cipher_hash_attach failed\n");
        } else {
            ret = inner_drv_hmac_start(chn_num, ctx, hash_attr);
            crypto_chk_goto(ret != TD_SUCCESS, error_hash_unlock, "inner_drv_hmac_start failed\n");
        }
        ctx->is_keyslot = hash_attr->is_keyslot;
    }

    *drv_hash_handle = chn_num;
    ctx->open = TD_TRUE;
    crypto_drv_func_exit();
    return ret;
error_hash_unlock:
    (td_void)hal_hash_unlock(chn_num);
    return ret;
}

td_s32 drv_cipher_hash_update(td_handle drv_hash_handle,  const crypto_buf_attr *src_buf, const td_u32 len)
{
    td_s32 ret = TD_SUCCESS;
    td_u32 chn_num = (td_u32)drv_hash_handle;
    hal_hash_channel_context *ctx = TD_NULL;
    td_u32 tail_len = 0;
    td_u32 *length = TD_NULL;
    td_u32 block_size;
    td_u32 processing_len = 0;
    td_u32 processed_len = 0;
    td_u8 *data_buffer = TD_NULL;
    td_u32 left = 0;
    in_node_type_e node_type = IN_NODE_TYPE_FIRST;
    crypto_drv_func_enter();

    hash_null_ptr_chk(src_buf);
    hash_null_ptr_chk(src_buf->virt_addr);
    crypto_chk_return(len == 0 || len > CRYPTO_HASH_UPDATE_MAX_LEN, HASH_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "len is invalid\n");

    ret = inner_hash_drv_handle_chk(drv_hash_handle);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ctx = &g_hash_channel_ctx_list[chn_num];
    crypto_chk_return(ctx->open == TD_FALSE, HASH_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed!\n");

    data_buffer = ctx->hash_ctx.data_buffer;
    crypto_chk_return(data_buffer == TD_NULL, HASH_COMPAT_ERRNO(ERROR_UNEXPECTED), "unexpected error\n");

    left = len;
    tail_len = ctx->hash_ctx.tail_len;
    length = ctx->hash_ctx.length;
    block_size = CRYPTO_HASH_GET_BLOCK_SIZE(ctx->hash_type) / CRYPTO_BITS_IN_BYTE;
    if (ctx->is_keyslot == TD_FALSE) {
        node_type |= IN_NODE_TYPE_LAST;
    }
    /* If the data can be saved to Tail, Update only copies the data. */
    if (tail_len + left < block_size) {
        processing_len = left;
        ret = memcpy_s(ctx->hash_ctx.tail + tail_len, sizeof(ctx->hash_ctx.tail) - tail_len,
            src_buf->virt_addr, processing_len);
        crypto_chk_return(ret != EOK, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");
        inner_drv_hash_length_add(length, processing_len * CRYPTO_BITS_IN_BYTE);
        ctx->hash_ctx.tail_len += processing_len;
        return TD_SUCCESS;
    }

    /* Process the tail. */
    ret = memcpy_s(data_buffer, ctx->hash_ctx.data_buffer_len, ctx->hash_ctx.tail, tail_len);
    crypto_chk_return(ret != EOK, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    processing_len = block_size - tail_len;
    ret = memcpy_s(data_buffer + tail_len, ctx->hash_ctx.data_buffer_len - tail_len,
        src_buf->virt_addr, processing_len);
    crypto_chk_return(ret != EOK, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    crypto_cache_flush((uintptr_t)data_buffer, block_size);
    ret = hal_cipher_hash_add_in_node(chn_num, crypto_get_phys_addr(data_buffer), block_size, node_type);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_hash_add_in_node failed\n");

    ret = hal_cipher_hash_start(chn_num, TD_FALSE);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_hash_start failed\n");

    ret = hal_cipher_hash_wait_done(chn_num, ctx->hash_ctx.state, sizeof(ctx->hash_ctx.state));
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_hash_wait_done failed\n");

    left -= processing_len;
    processed_len += processing_len;
    ctx->hash_ctx.tail_len = 0;

    /* Process the Remain Data. */
    while (left >= block_size) {
        if (left >= ctx->hash_ctx.data_buffer_len) {
            processing_len = ctx->hash_ctx.data_buffer_len;
        } else {
            processing_len = left - left % block_size;
        }
        ret = memcpy_s(data_buffer, ctx->hash_ctx.data_buffer_len, (td_u8 *)src_buf->virt_addr + processed_len,
            processing_len);
        crypto_chk_return(ret != EOK, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

        crypto_cache_flush((uintptr_t)data_buffer, processing_len);
        ret = hal_cipher_hash_add_in_node(chn_num, crypto_get_phys_addr(data_buffer), processing_len, node_type);
        crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_hash_add_in_node failed\n");

        ret = hal_cipher_hash_start(chn_num, TD_TRUE);
        crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_hash_start failed\n");

        ret = hal_cipher_hash_wait_done(chn_num, ctx->hash_ctx.state, sizeof(ctx->hash_ctx.state));
        crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_hash_wait_done failed\n");

        left -= processing_len;
        processed_len += processing_len;
    }

    if (left != 0) {
        ret = memcpy_s(ctx->hash_ctx.tail, sizeof(ctx->hash_ctx.tail), (td_u8 *)src_buf->virt_addr + processed_len,
            left);
        crypto_chk_return(ret != EOK, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");
    }
    ctx->hash_ctx.tail_len = left;
    inner_drv_hash_length_add(length, len * CRYPTO_BITS_IN_BYTE);
    crypto_drv_func_exit();
    return ret;
}

static td_s32 drv_process_tail(td_u32 chn_num, hal_hash_channel_context *ctx)
{
    td_s32 ret = TD_SUCCESS;
    td_u8 *buffer = ctx->hash_ctx.data_buffer;
    td_u32 buffer_len = ctx->hash_ctx.data_buffer_len;
    td_u32 tail_len = ctx->hash_ctx.tail_len;
    td_u32 *length = ctx->hash_ctx.length;
    td_u32 max_message_len = CRYPTO_HASH_GET_MESSAGE_LEN(ctx->hash_type) / CRYPTO_BITS_IN_BYTE;
    td_u32 block_size = CRYPTO_HASH_GET_BLOCK_SIZE(ctx->hash_type) / CRYPTO_BITS_IN_BYTE;
    td_u32 processing_len = 0;

    ret = memcpy_s(buffer, buffer_len, ctx->hash_ctx.tail, tail_len);
    crypto_chk_return(ret != EOK, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    (td_void)memset_s(buffer + tail_len, ctx->hash_ctx.data_buffer_len - tail_len,
        0x00, ctx->hash_ctx.data_buffer_len - tail_len);

    if (tail_len + 1 + max_message_len > block_size) {
        processing_len = block_size * 2; // in this case , we need 2 buffer
    } else {
        processing_len = block_size;
    }
    buffer[tail_len] = 0x80;
    if (ctx->is_keyslot) {
        inner_drv_hash_length_add(length, block_size * CRYPTO_BITS_IN_BYTE);
    }
    length[1] = crypto_cpu_to_be32(length[1]);
    length[0] = crypto_cpu_to_be32(length[0]);

    ret = memcpy_s(buffer + processing_len - sizeof(ctx->hash_ctx.length), sizeof(ctx->hash_ctx.length),
        length, sizeof(ctx->hash_ctx.length));
    crypto_chk_return(ret != EOK, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    crypto_cache_flush((uintptr_t)buffer, processing_len);
    ret = hal_cipher_hash_add_in_node(chn_num, crypto_get_phys_addr(buffer), processing_len,
        IN_NODE_TYPE_FIRST | IN_NODE_TYPE_LAST);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_hash_add_in_node failed\n");

    ret = hal_cipher_hash_start(chn_num, TD_TRUE);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_hash_start failed\n");

    ret = hal_cipher_hash_wait_done(chn_num, ctx->hash_ctx.state, sizeof(ctx->hash_ctx.state));
    if (ret != TD_SUCCESS) {
        crypto_print("hal_cipher_hash_wait_done failed\n");
        return ret;
    }

    return ret;
}

static td_s32 drv_hmac_finish_process(td_u32 chn_num, hal_hash_channel_context *ctx, td_u8 *out, td_u32 out_len)
{
    td_s32 ret = TD_SUCCESS;
    td_u8 *buffer = ctx->hash_ctx.data_buffer;
    td_u32 buffer_len = ctx->hash_ctx.data_buffer_len;
    td_u32 *length = ctx->hash_ctx.length;
    td_u32 processed_len = 0;
    td_u32 block_size = CRYPTO_HASH_GET_BLOCK_SIZE(ctx->hash_type) / 8; // 8 means 1 byte = 8 bits

    (td_void)memset_s(buffer, buffer_len, 0, buffer_len);
    /* Copy o_key_pad */
    ret = memcpy_s(buffer, buffer_len, ctx->hash_ctx.o_key_pad, block_size);
    crypto_chk_return(ret != EOK, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    processed_len += block_size;

    /* Copy hash. */
    ret = memcpy_s(buffer + processed_len, buffer_len - processed_len, out, out_len);
    crypto_chk_return(ret != EOK, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    processed_len += out_len;
    buffer[processed_len] = 0x80;
    length[1] = crypto_cpu_to_be32(processed_len * 8); // 8 means 1 byte = 8 bits
    ret = memcpy_s(buffer + 2 * block_size - sizeof(ctx->hash_ctx.length), // 2 block offset
        sizeof(ctx->hash_ctx.length), length, sizeof(ctx->hash_ctx.length));
    crypto_chk_return(ret != EOK, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    ret = drv_hash_get_state(ctx->hash_type, ctx->hash_ctx.state, sizeof(ctx->hash_ctx.state));
    if (ret != TD_SUCCESS) {
        crypto_log_err("drv_hash_get_state failed\n");
        return ret;
    }
    ret = hal_cipher_hash_config(chn_num, ctx->hash_type, ctx->hash_ctx.state);
    if (ret != TD_SUCCESS) {
        crypto_log_err("hal_cipher_hash_config failed\n");
        return ret;
    }

    crypto_cache_flush((uintptr_t)buffer, 2 * block_size); // 2 block_size needed
    ret = hal_cipher_hash_add_in_node(chn_num, crypto_get_phys_addr(buffer), 2 * block_size, // 2 block_size needed
        IN_NODE_TYPE_FIRST | IN_NODE_TYPE_LAST);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_hash_add_in_node failed\n");

    ret = hal_cipher_hash_start(chn_num, TD_TRUE);
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_cipher_hash_start failed\n");

    ret = hal_cipher_hash_wait_done(chn_num, ctx->hash_ctx.state, sizeof(ctx->hash_ctx.state));
    if (ret != TD_SUCCESS) {
        crypto_print("hal_cipher_hash_wait_done failed\n");
        return ret;
    }

    return ret;
}

td_s32 drv_cipher_hash_finish(td_handle drv_hash_handle, td_u8 *out, td_u32 *out_len)
{
    td_u32 i;
    td_s32 ret = TD_FAILURE;
    td_u32 chn_num = (td_u32)drv_hash_handle;
    hal_hash_channel_context *ctx = TD_NULL;
    td_u32 result_size;

    crypto_drv_func_enter();

    hash_null_ptr_chk(out);
    hash_null_ptr_chk(out_len);

    ret = inner_hash_drv_handle_chk(drv_hash_handle);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ctx = &g_hash_channel_ctx_list[chn_num];
    crypto_chk_return(ctx->open == TD_FALSE, HASH_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed!\n");

    crypto_chk_return(ctx->hash_ctx.data_buffer == TD_NULL, HASH_COMPAT_ERRNO(ERROR_UNEXPECTED), "unexpected error\n");

    result_size = CRYPTO_HASH_GET_RESULT_SIZE(ctx->hash_type) / CRYPTO_BITS_IN_BYTE;
    crypto_chk_return(*out_len < result_size, HASH_COMPAT_ERRNO(ERROR_INVALID_PARAM), "out's size is not enough\n");

    /* Process the Tail Data. */
    ret = drv_process_tail(chn_num, ctx);
    crypto_chk_goto(ret != TD_SUCCESS, exit_hash_unlock, "drv_process_tail failed\n");

    for (i = 0; i < result_size / CRYPTO_WORD_WIDTH; i++) {
        (td_void)memcpy_s(&out[i * CRYPTO_WORD_WIDTH], sizeof(td_u32), &ctx->hash_ctx.state[i], sizeof(td_u32));
    }

    if (CRYPTO_HASH_IS_HMAC(ctx->hash_type) && ctx->is_keyslot == TD_FALSE) {
        ret = drv_hmac_finish_process(chn_num, ctx, out, result_size);
        crypto_chk_goto(ret != TD_SUCCESS, exit_hash_unlock, "drv_hmac_finish_process failed\n");
        for (i = 0; i < result_size / CRYPTO_WORD_WIDTH; i++) {
            (td_void)memcpy_s(&out[i * CRYPTO_WORD_WIDTH], sizeof(td_u32), &ctx->hash_ctx.state[i], sizeof(td_u32));
        }
    }
    *out_len = result_size;
exit_hash_unlock:
    hal_hash_unlock(chn_num);
    (td_void)memset_s(ctx, sizeof(hal_hash_channel_context), 0, sizeof(hal_hash_channel_context));

    crypto_drv_func_exit();
    return ret;
}

td_s32 drv_cipher_hash_get(td_handle drv_hash_handle, crypto_hash_clone_ctx *hash_ctx)
{
    td_s32 ret = TD_SUCCESS;
    td_u32 chn_num = (td_u32)drv_hash_handle;
    hal_hash_channel_context *ctx = TD_NULL;
    crypto_drv_func_enter();

    hash_null_ptr_chk(hash_ctx);
    ret = inner_hash_drv_handle_chk(drv_hash_handle);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ctx = &g_hash_channel_ctx_list[chn_num];
    crypto_chk_return(ctx->open == TD_FALSE, HASH_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed!\n");

    (td_void)memset_s(hash_ctx, sizeof(crypto_hash_clone_ctx), 0, sizeof(crypto_hash_clone_ctx));
    /* Clone Length. */
    ret = memcpy_s(hash_ctx->length, sizeof(hash_ctx->length), ctx->hash_ctx.length, sizeof(ctx->hash_ctx.length));
    crypto_chk_goto_with_ret(ret != EOK, error_clear_hash_ctx, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    /* Clone state. */
    ret = memcpy_s(hash_ctx->state, sizeof(hash_ctx->state), ctx->hash_ctx.state, sizeof(ctx->hash_ctx.state));
    crypto_chk_goto_with_ret(ret != EOK, error_clear_hash_ctx, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    /* Clone o_key_pad. */
    ret = memcpy_s(hash_ctx->o_key_pad, sizeof(hash_ctx->o_key_pad),
        ctx->hash_ctx.o_key_pad, sizeof(ctx->hash_ctx.o_key_pad));
    crypto_chk_goto_with_ret(ret != EOK, error_clear_hash_ctx, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    /* Clone i_key_pad. */
    ret = memcpy_s(hash_ctx->i_key_pad, sizeof(hash_ctx->i_key_pad),
        ctx->hash_ctx.i_key_pad, sizeof(ctx->hash_ctx.i_key_pad));
    crypto_chk_goto_with_ret(ret != EOK, error_clear_hash_ctx, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    /* Clone tail. */
    ret = memcpy_s(hash_ctx->tail, sizeof(hash_ctx->tail), ctx->hash_ctx.tail, ctx->hash_ctx.tail_len);
    crypto_chk_goto_with_ret(ret != EOK, error_clear_hash_ctx, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    hash_ctx->tail_len = ctx->hash_ctx.tail_len;

    /* Clone Hash Type. */
    hash_ctx->hash_type = ctx->hash_type;
    crypto_drv_func_exit();

    return ret;
error_clear_hash_ctx:
    (td_void)memset_s(hash_ctx, sizeof(crypto_hash_clone_ctx), 0, sizeof(crypto_hash_clone_ctx));
    return ret;
}

td_s32 drv_cipher_hash_set(td_handle drv_hash_handle, const crypto_hash_clone_ctx *hash_ctx)
{
    td_s32 ret = TD_SUCCESS;
    td_u32 chn_num = (td_u32)drv_hash_handle;
    hal_hash_channel_context *ctx = TD_NULL;
    crypto_drv_func_enter();

    hash_null_ptr_chk(hash_ctx);
    ret = inner_hash_drv_handle_chk(drv_hash_handle);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    crypto_chk_return(g_drv_dma_buf == TD_NULL, HASH_COMPAT_ERRNO(ERROR_UNEXPECTED), "g_drv_dma_buf == TD_NULL\n");

    ctx = &g_hash_channel_ctx_list[chn_num];
    crypto_chk_return(ctx->open == TD_FALSE, HASH_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed!\n");

    /* Clone Length. */
    ret = memcpy_s(ctx->hash_ctx.length, sizeof(ctx->hash_ctx.length), hash_ctx->length, sizeof(hash_ctx->length));
    crypto_chk_goto_with_ret(ret != EOK, error_clear_hash_ctx, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    /* Clone state. */
    ret = memcpy_s(ctx->hash_ctx.state, sizeof(ctx->hash_ctx.state), hash_ctx->state, sizeof(hash_ctx->state));
    crypto_chk_goto_with_ret(ret != EOK, error_clear_hash_ctx, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    /* Clone o_key_pad. */
    ret = memcpy_s(ctx->hash_ctx.o_key_pad, sizeof(ctx->hash_ctx.o_key_pad),
        hash_ctx->o_key_pad, sizeof(hash_ctx->o_key_pad));
    crypto_chk_goto_with_ret(ret != EOK, error_clear_hash_ctx, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    /* Clone i_key_pad. */
    ret = memcpy_s(ctx->hash_ctx.i_key_pad, sizeof(ctx->hash_ctx.i_key_pad),
        hash_ctx->i_key_pad, sizeof(hash_ctx->i_key_pad));
    crypto_chk_goto_with_ret(ret != EOK, error_clear_hash_ctx, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    /* Clone tail. */
    ret = memcpy_s(ctx->hash_ctx.tail, sizeof(ctx->hash_ctx.tail), hash_ctx->tail, sizeof(hash_ctx->tail));
    crypto_chk_goto_with_ret(ret != EOK, error_clear_hash_ctx, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "memcpy_s failed\n");

    ctx->hash_ctx.tail_len = hash_ctx->tail_len;

    /* Clone Hash Type. */
    ctx->hash_type = hash_ctx->hash_type;

    /* Set Hash Config. */
    ret = hal_cipher_hash_config(chn_num, ctx->hash_type, ctx->hash_ctx.state);
    if (ret != TD_SUCCESS) {
        crypto_print("hal_cipher_hash_config expected is 0x%x, real ret is 0x%x\n", TD_SUCCESS, ret);
        goto error_clear_hash_ctx;
    }
    /* Set Data Buffer. */
    ctx->hash_ctx.data_buffer = g_drv_dma_buf + CRYPTO_HASH_DRV_BUFFER_SIZE * chn_num;
    ctx->hash_ctx.data_buffer_len = CRYPTO_HASH_DRV_BUFFER_SIZE;
    crypto_drv_func_exit();

    return ret;
error_clear_hash_ctx:
    (td_void)memset_s(ctx, sizeof(hal_hash_channel_context), 0, sizeof(hal_hash_channel_context));
    return ret;
}

td_s32 drv_cipher_hash_destroy(td_handle drv_hash_handle)
{
    td_s32 ret = TD_SUCCESS;
    td_u32 chn_num = (td_u32)drv_hash_handle;
    hal_hash_channel_context *ctx = TD_NULL;
    crypto_drv_func_enter();

    ret = inner_hash_drv_handle_chk(drv_hash_handle);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    ctx = &g_hash_channel_ctx_list[chn_num];
    crypto_chk_return(ctx->open == TD_FALSE, HASH_COMPAT_ERRNO(ERROR_CTX_CLOSED), "ctx is closed!\n");

    hal_hash_unlock(chn_num);
#if defined(CRYPTO_DEBUG_ON)
    crypto_print("Unlock Hash CHN %u success\n", chn_num);
    hal_hash_debug();
#endif
    (td_void)memset_s(ctx, sizeof(hal_hash_channel_context), 0, sizeof(hal_hash_channel_context));
    crypto_drv_func_exit();
    return ret;
}

td_u32 drv_hash_get_state_iv(crypto_hash_type hash_type, td_u32 *iv_size, td_u32 *state_iv, td_u32 state_iv_len)
{
    td_s32 ret = TD_SUCCESS;
    td_u32 hash_alg = CRYPTO_HASH_GET_ALG(hash_type);
    td_u32 hash_mode = CRYPTO_HASH_GET_MODE(hash_type);

    crypto_chk_return(iv_size == TD_NULL, TD_NULL, "iv_size is NULL!\n");

    switch (hash_mode) {
        case CRYPTO_HASH_MODE_256: {
            if (hash_alg == CRYPTO_HASH_ALG_SHA2) {
                ret = memcpy_s(state_iv, state_iv_len, g_sha256_ival, sizeof(g_sha256_ival));
                crypto_chk_return(ret != EOK, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "MEMCPY_S FAILED\n");
                *iv_size = sizeof(g_sha256_ival);
            } else if (hash_alg == CRYPTO_HASH_ALG_SM3) {
                ret = memcpy_s(state_iv, state_iv_len, g_sm3_ival, sizeof(g_sm3_ival));
                crypto_chk_return(ret != EOK, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "MEMCPY_S FAILED\n");
                *iv_size = sizeof(g_sm3_ival);
            }
            break;
        }
        case CRYPTO_HASH_MODE_384: {
            ret = memcpy_s(state_iv, state_iv_len, g_sha384_ival, sizeof(g_sha384_ival));
            crypto_chk_return(ret != EOK, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "MEMCPY_S FAILED\n");
            *iv_size = sizeof(g_sha384_ival);
            break;
        }
        case CRYPTO_HASH_MODE_512: {
            ret = memcpy_s(state_iv, state_iv_len, g_sha512_ival, sizeof(g_sha512_ival));
            crypto_chk_return(ret != EOK, HASH_COMPAT_ERRNO(ERROR_MEMCPY_S), "MEMCPY_S FAILED\n");
            *iv_size = sizeof(g_sha512_ival);
            break;
        }
        default: {
            crypto_log_err("Invalid Hash Mode!\n");
            return HASH_COMPAT_ERRNO(ERROR_INVALID_PARAM);
            break;
        }
    }
    return ret;
}

td_s32 inner_hash_drv_handle_chk(td_handle hash_handle)
{
    td_u32 chn_num = (td_u32)hash_handle;
    crypto_chk_return(chn_num >= CRYPTO_HASH_HARD_CHN_CNT, HASH_COMPAT_ERRNO(ERROR_INVALID_HANDLE),
        "hash_handle is invalid\n");
    crypto_chk_return(((1 << chn_num) & CRYPTO_HASH_HARD_CHN_MASK) == 0, HASH_COMPAT_ERRNO(ERROR_INVALID_HANDLE),
        "hash_handle is invalid\n");
    
    return TD_SUCCESS;
}

td_s32 inner_hash_start_param_chk(td_handle *drv_hash_handle, const crypto_hash_attr *hash_attr)
{
    crypto_hash_type type;
    td_u32 block_size = 0;

    hash_null_ptr_chk(drv_hash_handle);
    hash_null_ptr_chk(hash_attr);

    type = hash_attr->hash_type;
    crypto_chk_return(type != CRYPTO_HASH_TYPE_SHA256 && type != CRYPTO_HASH_TYPE_SHA384 &&
        type != CRYPTO_HASH_TYPE_SHA512 && type != CRYPTO_HASH_TYPE_SM3 &&
        type != CRYPTO_HASH_TYPE_HMAC_SHA256 && type != CRYPTO_HASH_TYPE_HMAC_SHA384 &&
        type != CRYPTO_HASH_TYPE_HMAC_SHA512 && type != CRYPTO_HASH_TYPE_HMAC_SM3,
        HASH_COMPAT_ERRNO(ERROR_INVALID_PARAM), "hash_type is invalid!\n");

    if (CRYPTO_HASH_IS_HMAC(type) != TD_TRUE) {
        return TD_SUCCESS;
    }

    /* HMAC Check. */
    block_size = CRYPTO_HASH_GET_BLOCK_SIZE(type) / CRYPTO_BITS_IN_BYTE;
    if (hash_attr->is_keyslot == TD_TRUE) {
    } else {
        if (hash_attr->key_len != 0) {
            crypto_chk_return(hash_attr->key == TD_NULL, HASH_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "key is NULL\n");
        }
        crypto_chk_return(hash_attr->key_len > block_size, HASH_COMPAT_ERRNO(ERROR_INVALID_PARAM),
            "key_len shouldn't larget than block_size\n");
    }
    return TD_SUCCESS;
}