/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : drivers for ext symc.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#include "drv_osal_lib.h"
#include "cryp_symc.h"
#include "mbedtls/cipher.h"

#if defined(SOFT_AES_SUPPORT) || defined(SOFT_TDES_SUPPORT)

/*
 * brief          aes ccm context structure
 */
typedef struct {
    hi_u32 key[SYMC_KEY_SIZE / WORD_WIDTH];     /* symc even round keys, default */
    hi_u32 klen;                                /* symc key length */
    mbedtls_cipher_id_t cipher_id;
    mbedtls_cipher_mode_t mode;
    mbedtls_cipher_context_t cipher;
} ext_symc_context;

hi_void *ext_mbedtls_symc_create(hi_u32 hard_chn)
{
    ext_symc_context *ctx = HI_NULL;

    hi_log_func_enter();

    ctx = crypto_malloc(sizeof(ext_symc_context));
    if (ctx == HI_NULL) {
        hi_log_print_err_code(HI_ERR_CIPHER_FAILED_MEM);
        hi_log_error("malloc failed \n");
        return HI_NULL;
    }
    crypto_memset(ctx, sizeof(ext_symc_context), 0, sizeof(ext_symc_context));

    mbedtls_cipher_init(&ctx->cipher);

    hi_log_func_exit();

    return ctx;
}

hi_s32 ext_mbedtls_symc_destory(hi_void *ctx)
{
    ext_symc_context *symc = ctx;

    hi_log_func_enter();

    if (ctx != HI_NULL) {
        mbedtls_cipher_free(&symc->cipher);
        crypto_free(ctx);
        ctx = HI_NULL;
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 ext_mbedtls_symc_setmode(hi_void *ctx, symc_alg alg, symc_mode mode, symc_width width)
{
    ext_symc_context *symc = ctx;

    hi_log_func_enter();

    hi_log_chk_param_return(symc == HI_NULL);
    hi_log_chk_param_return(width != SYMC_DAT_WIDTH_128);

    switch (alg) {
        case SYMC_ALG_AES:
            symc->cipher_id = MBEDTLS_CIPHER_ID_AES;
            break;
        case SYMC_ALG_TDES:
            symc->cipher_id = MBEDTLS_CIPHER_ID_3DES;
            break;
        case SYMC_ALG_DES:
            symc->cipher_id = MBEDTLS_CIPHER_ID_DES;
            break;
        default:
            hi_log_error("unsupport alg %d\n", alg);
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
            return HI_ERR_CIPHER_INVALID_PARAM;
    }

    switch (mode) {
        case SYMC_MODE_ECB:
            symc->mode = MBEDTLS_MODE_ECB;
            break;
        case SYMC_MODE_CBC:
            symc->mode = MBEDTLS_MODE_CBC;
            break;
        case SYMC_MODE_CFB:
            symc->mode = MBEDTLS_MODE_CFB;
            break;
        case SYMC_MODE_OFB:
            symc->mode = MBEDTLS_MODE_OFB;
            break;
        case SYMC_MODE_CTR:
            symc->mode = MBEDTLS_MODE_CTR;
            break;
        default:
            hi_log_error("unsupport mode %d\n", mode);
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
            return HI_ERR_CIPHER_INVALID_PARAM;
    }

    hi_log_debug("cipher_id %d, mode %d\n", symc->cipher_id, symc->mode);

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 ext_mbedtls_symc_setiv(hi_void *ctx, const hi_u8 *iv, hi_u32 ivlen, hi_u32 usage)
{
    hi_s32 ret;
    ext_symc_context *symc = ctx;

    hi_log_func_enter();

    hi_log_chk_param_return(symc == HI_NULL);
    hi_log_chk_param_return(iv   == HI_NULL);
    hi_log_chk_param_return(ivlen > AES_IV_SIZE);

    ret = mbedtls_cipher_set_iv(&symc->cipher, iv, ivlen);

    hi_log_func_exit();

    return ret;
}

hi_s32 ext_mbedtls_symc_getiv(hi_void *ctx, hi_u8 *iv, hi_u32 *ivlen)
{
    ext_symc_context *symc = ctx;

    hi_log_func_enter();

    hi_log_chk_param_return(symc  == HI_NULL);
    hi_log_chk_param_return(iv    == HI_NULL);
    hi_log_chk_param_return(ivlen == HI_NULL);

    crypto_memcpy(iv, AES_IV_SIZE, symc->cipher.iv, symc->cipher.iv_size);
    *ivlen = symc->cipher.iv_size;

    hi_log_func_exit();

    return HI_SUCCESS;
}

hi_s32 ext_mbedtls_symc_setkey(hi_void *ctx, const hi_u8 *fkey, const hi_u8 *skey, hi_u32 *hisi_klen)
{
    hi_s32 ret;
    hi_u32 klen = 0;
    ext_symc_context *symc = ctx;
    const mbedtls_cipher_info_t *info = HI_NULL;

    hi_log_func_enter();

    hi_log_chk_param_return(symc == HI_NULL);
    hi_log_chk_param_return(fkey == HI_NULL);
    hi_log_chk_param_return(hisi_klen == HI_NULL);

    if (symc->cipher_id == MBEDTLS_CIPHER_ID_AES) {
        switch (*hisi_klen) {
            case HI_CIPHER_KEY_AES_128BIT:
                klen = AES_KEY_128BIT;
                break;
            case HI_CIPHER_KEY_AES_192BIT:
                klen = AES_KEY_192BIT;
                break;
            case HI_CIPHER_KEY_AES_256BIT:
                klen = AES_KEY_256BIT;
                break;
            default:
                hi_log_error("Invalid aes key len: 0x%x\n", klen);
                hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
                return HI_ERR_CIPHER_INVALID_PARAM;
        }
        crypto_memcpy(symc->key, SYMC_KEY_SIZE, fkey, klen);
    } else if (symc->cipher_id == MBEDTLS_CIPHER_ID_3DES) {
        klen = TDES_KEY_192BIT;
        crypto_memcpy(symc->key, SYMC_KEY_SIZE, fkey, klen);
        if (*hisi_klen == HI_CIPHER_KEY_DES_2KEY) {
            /* descript: k3 = k1 */
            symc->key[WORD_IDX_4] = symc->key[WORD_IDX_0];
            symc->key[WORD_IDX_5] = symc->key[WORD_IDX_1];
        }
    } else if (symc->cipher_id == MBEDTLS_CIPHER_ID_DES) {
        klen = DES_KEY_SIZE;
        crypto_memcpy(symc->key, SYMC_KEY_SIZE, fkey, klen);
    }
    hi_log_info("key len %d, type %d\n", klen, *hisi_klen);

    symc->klen = klen;

    hi_log_debug("cipher_id %d, klen %d, mode %d\n", symc->cipher_id, klen, symc->mode);
    info = mbedtls_cipher_info_from_values(symc->cipher_id, klen * BYTE_BITS, symc->mode);
    hi_log_chk_param_return(info == HI_NULL);

    ret = mbedtls_cipher_setup(&symc->cipher, info);

    *hisi_klen = klen;

    hi_log_func_exit();

    return ret;
}

static hi_s32 ext_symc_update(ext_symc_context *symc, crypto_mem *mem_in, crypto_mem *mem_out, hi_u32 len, hi_u32 *olen)
{
    hi_s32 ret;
    if (symc->mode == MBEDTLS_MODE_ECB) {
        hi_u32 offset = 0;
        while (offset < len) {
            ret = mbedtls_cipher_update(symc->cipher, crypto_mem_virt(mem_in) + offset,
                mbedtls_cipher_get_block_size(symc->cipher), crypto_mem_virt(mem_out) + offset, olen);
            if (ret != HI_SUCCESS) {
                hi_log_print_func_err(mbedtls_cipher_update, ret);
                return ret;
            }
            offset += mbedtls_cipher_get_block_size(symc->cipher);
        }
    } else {
        ret = mbedtls_cipher_update(symc->cipher, crypto_mem_virt(mem_in), len, crypto_mem_virt(mem_out), olen);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(mbedtls_cipher_update, ret);
            return ret;
        }
    }

    return HI_SUCCESS;
}

hi_s32 ext_mbedtls_symc_crypto(hi_void *ctx, hi_u32 operation, symc_multi_pack *pack, hi_u32 last)
{
    ext_symc_context *symc = ctx;
    crypto_mem mem_in, mem_out;
    hi_u32 offset = 0;
    size_t olen = 0;
    hi_s32 ret;

    hi_log_func_enter();

    hi_log_chk_param_return(symc   == HI_NULL);
    hi_log_chk_param_return(pack   == HI_NULL);
    hi_log_chk_param_return(pack->in == HI_NULL);
    hi_log_chk_param_return(pack->out == HI_NULL);
    hi_log_chk_param_return(pack->len == HI_NULL);
    hi_log_chk_param_return(pack->num != 0x01);

    ret = mbedtls_cipher_setkey(&symc->cipher, (hi_u8 *)symc->key, symc->klen * BYTE_BITS,
                                operation ? MBEDTLS_DECRYPT : MBEDTLS_ENCRYPT);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(mbedtls_cipher_setkey, ret);
        return ret;
    }

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

    hi_log_debug("symc 0x%p, klen len: %d\n", symc, symc->klen);

    ret = ext_symc_update(symc, mem_in, mem_out, pack->len[0], &olen);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(ext_symc_update, ret);
        crypto_mem_close(&mem_out);
        crypto_mem_close(&mem_in);
        return ret;
    }

    crypto_mem_close(&mem_out);
    crypto_mem_close(&mem_in);
    hi_log_func_exit();
    return ret;
}
#endif
