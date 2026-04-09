/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2018-2019. All rights reserved.
 * Description   : ext for aes ccm and gcm.
 * Author        : Hisilicon multimedia software group
 * Create        : 2018-10-23
 */

#include "drv_osal_lib.h"
#include "cryp_symc.h"
#include "mbedtls/ccm.h"
#include "mbedtls/gcm.h"

#ifdef SOFT_AES_CCM_GCM_SUPPORT

/*
 * aes ccm context structure
 */
typedef struct {
    hi_u32 key[SYMC_KEY_SIZE / WORD_WIDTH];     /* SYMC even round keys, default */
    hi_u32 iv[AES_IV_SIZE / WORD_WIDTH];        /* symc IV */
    hi_u32 tag[AEAD_TAG_SIZE / WORD_WIDTH];     /* aead tag */
    hi_u32 ivlen;                               /* symc IV length */
    hi_u32 klen;                                /* symc key length */
    compat_addr aad;                            /* Associated Data */
    hi_u32 alen;                                /* Associated Data length */
    hi_u32 tlen;                                /* Tag length */
} ext_aead_context;

typedef struct {
    crypto_mem in;
    crypto_mem out;
    crypto_mem aad;
} ext_ccm_gcm_mem;

hi_void *ext_mbedtls_aead_create(hi_u32 hard_chn)
{
    ext_aead_context *ctx = HI_NULL;

    hi_log_func_enter();

    ctx = crypto_calloc(1, sizeof(ext_aead_context));
    if (ctx == HI_NULL) {
        hi_log_error("malloc failed \n");
        hi_log_print_func_err(crypto_calloc, 0);
        return HI_NULL;
    }

    hi_log_func_exit();

    return ctx;
}

hi_s32 ext_mbedtls_aead_destory(hi_void *ctx)
{
    hi_log_func_enter();

    if (ctx != HI_NULL) {
        crypto_free(ctx);
        ctx = HI_NULL;
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 ext_mbedtls_aead_setiv(hi_void *ctx, const hi_u8 *iv, hi_u32 ivlen, hi_u32 usage)
{
    ext_aead_context *aead = ctx;

    hi_log_func_enter();

    hi_log_chk_param_return(aead == HI_NULL);
    hi_log_chk_param_return(ivlen > AES_IV_SIZE);

    if (iv != HI_NULL) {
        crypto_memcpy(aead->iv, AES_IV_SIZE, iv, ivlen);
        aead->ivlen = ivlen;
        hi_log_debug("ivlen %d\n", ivlen);
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 ext_mbedtls_aead_setkey(hi_void *ctx, const hi_u8 *fkey, const hi_u8 *skey, hi_u32 *hisi_klen)
{
    hi_u32 klen;
    ext_aead_context *aead = ctx;

    hi_log_func_enter();

    hi_log_chk_param_return(aead == HI_NULL);
    hi_log_chk_param_return(fkey == HI_NULL);
    hi_log_chk_param_return(hisi_klen == HI_NULL);

    switch (*hisi_klen) {
        case HI_CIPHER_KEY_AES_128BIT: {
            klen = AES_KEY_128BIT;
            break;
        }
        case HI_CIPHER_KEY_AES_192BIT: {
            klen = AES_KEY_192BIT;
            break;
        }
        case HI_CIPHER_KEY_AES_256BIT: {
            klen = AES_KEY_256BIT;
            break;
        }
        default: {
            hi_log_error("Invalid aes key len: 0x%x\n", *hisi_klen);
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
            return HI_ERR_CIPHER_INVALID_PARAM;
        }
    }
    hi_log_info("key len %d, type %d\n", klen, hisi_klen);

    crypto_memcpy(aead->key, SYMC_KEY_SIZE, fkey, klen);
    aead->klen = klen;
    *hisi_klen = klen;

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 ext_mbedtls_aead_set_aad(hi_void *ctx, compat_addr aad, hi_u32 alen, hi_u32 tlen)
{
    ext_aead_context *aead = ctx;

    hi_log_func_enter();

    hi_log_chk_param_return(aead == HI_NULL);

    aead->aad = aad;
    aead->alen = alen;
    aead->tlen = tlen;

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 ext_mbedtls_aead_get_tag(hi_void *ctx, hi_u32 tag[AEAD_TAG_SIZE_IN_WORD], hi_u32 *taglen)
{
    ext_aead_context *aead = ctx;

    hi_log_func_enter();

    hi_log_chk_param_return(aead  == HI_NULL);
    hi_log_chk_param_return(*taglen < aead->tlen);

    hi_log_debug("tag buffer len %d, tag len %d\n", *taglen,  aead->tlen);

    *taglen = aead->tlen;

    crypto_memcpy(tag, AEAD_TAG_SIZE, aead->tag, aead->tlen);

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 ext_ccm_gcm_mem_open(ext_ccm_gcm_mem *mem, symc_multi_pack *pack, compat_addr aad, hi_u32 aad_len)
{
    hi_s32 ret;

    ret = crypto_mem_open(&mem->in, pack->in[0], pack->len[0]);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_mem_open, ret);
        return ret;
    }

    ret = crypto_mem_open(&mem->out, pack->out[0], pack->len[0]);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_mem_open, ret);
        (hi_void)crypto_mem_close(&mem->in);
        return ret;
    }

    ret = crypto_mem_open(&mem->aad, aad, aad_len);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_mem_open, ret);
        (hi_void)crypto_mem_close(&mem->out);
        (hi_void)crypto_mem_close(&mem->in);
        return ret;
    }

    return HI_SUCCESS;
}

static hi_s32 ext_ccm_gcm_mem_close(ext_ccm_gcm_mem *mem)
{
    hi_s32 ret;

    ret = crypto_mem_close(&mem->aad);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_mem_close, ret);
        (hi_void)crypto_mem_close(&mem->out);
        (hi_void)crypto_mem_close(&mem->in);
        return ret;
    }

    ret = crypto_mem_close(&mem->out);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_mem_close, ret);
        (hi_void)crypto_mem_close(&mem->in);
        return ret;
    }

    ret = crypto_mem_close(&mem->in);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_mem_close, ret);
        return ret;
    }

    return HI_SUCCESS;
}

hi_s32 ext_mbedtls_aead_ccm_crypto(hi_void *ctx, hi_u32 operation, symc_multi_pack *pack, hi_u32 last)
{
    hi_s32 ret, ret_err;
    ext_aead_context *aead = ctx;
    mbedtls_ccm_context ccm;
    ext_ccm_gcm_mem mem;

    hi_log_func_enter();

    hi_log_chk_param_return((aead == HI_NULL) || (pack == HI_NULL));
    hi_log_chk_param_return((pack->num != 1) || (pack->len == HI_NULL));

    crypto_memset(&mem, sizeof(mem), 0, sizeof(mem));

    ret = ext_ccm_gcm_mem_open(&mem, pack, aead->aad, aead->alen);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(ext_ccm_gcm_mem_open, ret);
        return ret;
    }

    mbedtls_ccm_init(&ccm);

    hi_log_debug("aead 0x%p, klen len: %d\n", aead, aead->klen);

    ret = mbedtls_ccm_setkey(&ccm, MBEDTLS_CIPHER_ID_AES, (hi_u8 *)aead->key, aead->klen * BITS_IN_BYTE);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(mbedtls_ccm_setkey, ret);
        goto error0;
    }

    if (operation) {
        ret = mbedtls_ccm_auth_decrypt(&ccm, pack->len[0], (hi_u8 *)aead->iv, aead->ivlen, crypto_mem_virt(&mem.aad),
            aead->alen, crypto_mem_virt(&mem.in), crypto_mem_virt(&mem.out), (hi_u8 *)aead->tag, aead->tlen);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(mbedtls_ccm_auth_decrypt, ret);
        }
    } else {
        ret = mbedtls_ccm_encrypt_and_tag(&ccm, pack->len[0], (hi_u8 *)aead->iv, aead->ivlen, crypto_mem_virt(&mem.aad),
            aead->alen, crypto_mem_virt(&mem.in), crypto_mem_virt(&mem.out), (hi_u8 *)aead->tag, aead->tlen);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(mbedtls_ccm_encrypt_and_tag, ret);
        }
    }

    hi_log_func_exit();

error0:
    mbedtls_ccm_free(&ccm);
    ret_err = ext_ccm_gcm_mem_close(&mem);
    if (ret_err != HI_SUCCESS) {
        hi_log_print_func_err(ext_ccm_gcm_mem_close, ret_err);
    }
    return ret;
}

hi_s32 ext_mbedtls_aead_gcm_crypto(hi_void *ctx, hi_u32 operation, symc_multi_pack *pack, hi_u32 last)
{
    hi_s32 ret, ret_err;
    ext_aead_context *aead = ctx;
    mbedtls_gcm_context *gcm = HI_NULL;
    ext_ccm_gcm_mem mem;

    hi_log_func_enter();

    hi_log_chk_param_return((aead == HI_NULL) || (pack == HI_NULL));
    hi_log_chk_param_return((pack->num != 1) || (pack->len == HI_NULL));

    crypto_memset(&mem, sizeof(mem), 0, sizeof(mem));

    gcm = crypto_calloc(1, sizeof(mbedtls_gcm_context));
    if (gcm == HI_NULL) {
        hi_log_print_func_err(crypto_calloc, HI_ERR_CIPHER_FAILED_MEM);
        return HI_ERR_CIPHER_FAILED_MEM;
    }

    ret = ext_ccm_gcm_mem_open(&mem, pack, aead->aad, aead->alen);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(ext_ccm_gcm_mem_open, ret);
        goto error0;
    }

    mbedtls_gcm_init(gcm);

    ret = mbedtls_gcm_setkey(gcm, MBEDTLS_CIPHER_ID_AES, (hi_u8 *)aead->key, aead->klen * BITS_IN_BYTE);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(mbedtls_gcm_setkey, ret);
        goto error1;
    }

    ret = mbedtls_gcm_starts(gcm, operation ? MBEDTLS_DECRYPT : MBEDTLS_ENCRYPT, (hi_u8 *)aead->iv, aead->ivlen,
        crypto_mem_virt(&mem.aad), aead->alen);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(mbedtls_gcm_starts, ret);
        goto error1;
    }

    ret = mbedtls_gcm_update(gcm, pack->len[0], crypto_mem_virt(&mem.in), crypto_mem_virt(&mem.out));
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(mbedtls_gcm_update, ret);
        goto error1;
    }

    ret = mbedtls_gcm_finish(gcm, (hi_u8 *)aead->tag, aead->tlen);

error1:
    mbedtls_gcm_free(gcm);
    ret_err = ext_ccm_gcm_mem_close(&mem);
    if (ret_err != HI_SUCCESS) {
        hi_log_print_func_err(ext_ccm_gcm_mem_close, ret_err);
    }

error0:
    crypto_free(gcm);
    gcm = HI_NULL;
    return ret;
}

#endif /* End of SOFT_AES_CCM_GCM_SUPPORT */
