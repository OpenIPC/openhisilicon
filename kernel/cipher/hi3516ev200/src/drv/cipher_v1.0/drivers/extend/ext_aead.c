/******************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : ext_aead.c
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include "drv_osal_lib.h"
#include "cryp_symc.h"
#include "mbedtls/ccm.h"
#include "mbedtls/gcm.h"

#ifdef SOFT_AES_CCM_GCM_SUPPORT

/**
 * \brief          aes ccm context structure
 */
typedef struct {
    hi_u32 key[SYMC_KEY_SIZE / 4];    /*!<  SYMC even round keys, default */
    hi_u32 iv[AES_IV_SIZE / 4];       /*!<  symc IV */
    hi_u32 tag[AEAD_TAG_SIZE / 4];    /*!<  aead tag */
    hi_u32 ivlen;               /*!<  symc IV length */
    hi_u32 klen;                /*!<  symc key length */
    compat_addr aad;         /*!<  Associated Data */
    hi_u32 alen;                /*!<  Associated Data length */
    hi_u32 tlen;                /*!<  Tag length */
}
ext_aead_context;

void *ext_mbedtls_aead_create(hi_u32 hard_chn)
{
    ext_aead_context *ctx = HI_NULL;

    HI_LOG_FUNC_ENTER();

    ctx = crypto_calloc(1, sizeof(ext_aead_context));
    if (ctx == HI_NULL) {
        HI_LOG_ERROR("malloc failed \n");
        HI_LOG_PRINT_FUNC_ERR(crypto_calloc, 0);
        return HI_NULL;
    }

    HI_LOG_FUNC_EXIT();

    return ctx;
}

hi_s32 ext_mbedtls_aead_destory(void *ctx)
{
    HI_LOG_FUNC_ENTER();

    if (ctx != HI_NULL) {
        crypto_free(ctx);
        ctx = HI_NULL;
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 ext_mbedtls_aead_setiv(void *ctx, const hi_u8 *iv, hi_u32 ivlen, hi_u32 usage)
{
    ext_aead_context *aead = ctx;

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(aead == HI_NULL);
    HI_LOG_CHECK_PARAM(ivlen > AES_IV_SIZE);

    if (iv != HI_NULL) {
        crypto_memcpy(aead->iv, AES_IV_SIZE, iv, ivlen);
        aead->ivlen = ivlen;
        HI_LOG_DEBUG("ivlen %d\n", ivlen);
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 ext_mbedtls_aead_setkey(void *ctx, const hi_u8 *fkey, const hi_u8 *skey, hi_u32 *hisi_klen)
{
    hi_u32 klen = 0;
    ext_aead_context *aead = ctx;

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(aead == HI_NULL);
    HI_LOG_CHECK_PARAM(fkey == HI_NULL);
    HI_LOG_CHECK_PARAM(hisi_klen == HI_NULL);

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
            HI_LOG_ERROR("Invalid aes key len: 0x%x\n", klen);
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }
    HI_LOG_INFO("key len %d, type %d\n", klen, hisi_klen);

    crypto_memcpy(aead->key, SYMC_KEY_SIZE, fkey, klen);
    aead->klen = klen;
    *hisi_klen = klen;

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 ext_mbedtls_aead_set_aad(void *ctx, compat_addr aad, hi_u32 alen, hi_u32 tlen)
{
    ext_aead_context *aead = ctx;

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(aead == HI_NULL);

    aead->aad = aad;
    aead->alen = alen;
    aead->tlen = tlen;

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 ext_mbedtls_aead_get_tag(void *ctx, hi_u32 tag[AEAD_TAG_SIZE_IN_WORD], hi_u32 *taglen)
{
    ext_aead_context *aead = ctx;

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(aead  == HI_NULL);
    HI_LOG_CHECK_PARAM(*taglen < aead->tlen);

    HI_LOG_DEBUG("tag buffer len %d, tag len %d\n", *taglen,  aead->tlen);

    *taglen = aead->tlen;

    crypto_memcpy(tag, AEAD_TAG_SIZE, aead->tag, aead->tlen);

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 ext_mbedtls_aead_ccm_crypto(void *ctx,
                                hi_u32 operation,
                                compat_addr input[],
                                compat_addr output[],
                                hi_u32 length[],
                                symc_node_usage usage_list[],
                                hi_u32 pkg_num, hi_u32 last)
{
    ext_aead_context *aead = ctx;
    mbedtls_ccm_context ccm;
    crypto_mem mem_in, mem_out, aad;
    hi_s32 ret = HI_FAILURE;

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(aead == HI_NULL);
    HI_LOG_CHECK_PARAM(length == HI_NULL);
    HI_LOG_CHECK_PARAM(pkg_num != 0x01);

    ret = crypto_mem_open(&mem_in, input[0], length[0]);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("open mem of input failed\n");
        ret = HI_ERR_CIPHER_FAILED_MEM;
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_FAILED_MEM);
        return HI_ERR_CIPHER_FAILED_MEM;
    }

    ret = crypto_mem_open(&mem_out, output[0], length[0]);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("open mem of output failed\n");
        ret = HI_ERR_CIPHER_FAILED_MEM;
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_FAILED_MEM);
        goto error1;
    }

    ret = crypto_mem_open(&aad, aead->aad, aead->alen);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("open mem of aad failed\n");
        ret = HI_ERR_CIPHER_FAILED_MEM;
        HI_LOG_FUNC_EXIT();
        goto error2;
    }

    mbedtls_ccm_init(&ccm);

    HI_LOG_DEBUG("aead 0x%p, klen len: %d\n", aead, aead->klen);

    ret = mbedtls_ccm_setkey(&ccm, MBEDTLS_CIPHER_ID_AES, (hi_u8 *)aead->key,
                             aead->klen * BITS_IN_BYTE);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(mbedtls_ccm_setkey, ret);
        goto error3;
    }

    if (operation) {
        ret = mbedtls_ccm_auth_decrypt(&ccm, length[0],
                                       (hi_u8 *)aead->iv , aead->ivlen,
                                       crypto_mem_virt(&aad), aead->alen,
                                       crypto_mem_virt(&mem_in), crypto_mem_virt(&mem_out),
                                       (hi_u8 *)aead->tag, aead->tlen);
        if (ret != HI_SUCCESS) {
            HI_LOG_PRINT_FUNC_ERR(mbedtls_ccm_auth_decrypt, ret);
        }
    } else {
        ret = mbedtls_ccm_encrypt_and_tag(&ccm, length[0],
                                          (hi_u8 *)aead->iv , aead->ivlen,
                                          crypto_mem_virt(&aad), aead->alen,
                                          crypto_mem_virt(&mem_in), crypto_mem_virt(&mem_out),
                                          (hi_u8 *)aead->tag, aead->tlen);
        if (ret != HI_SUCCESS) {
            HI_LOG_PRINT_FUNC_ERR(mbedtls_ccm_encrypt_and_tag, ret);
        }
    }

    HI_LOG_FUNC_EXIT();

error3:
    mbedtls_ccm_free(&ccm);
    crypto_mem_close(&aad);
error2:
    crypto_mem_close(&mem_out);
error1:
    crypto_mem_close(&mem_in);

    return ret;
}

hi_s32 ext_mbedtls_aead_gcm_crypto(void *ctx,
                                hi_u32 operation,
                                compat_addr input[],
                                compat_addr output[],
                                hi_u32 length[],
                                symc_node_usage usage_list[],
                                hi_u32 pkg_num, hi_u32 last)
{
    ext_aead_context *aead = ctx;
    mbedtls_gcm_context *gcm = HI_NULL;
    crypto_mem mem_in, mem_out, aad;
    hi_s32 ret = HI_FAILURE;

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(aead == HI_NULL);
    HI_LOG_CHECK_PARAM(length == HI_NULL);
    HI_LOG_CHECK_PARAM(pkg_num != 0x01);

    crypto_memset(&aad, sizeof(aad), 0, sizeof(aad));
    crypto_memset(&mem_in, sizeof(mem_in), 0, sizeof(mem_in));
    crypto_memset(&mem_out, sizeof(mem_out), 0, sizeof(mem_out));

    gcm = crypto_calloc(1, sizeof(mbedtls_gcm_context));
    if (gcm == HI_NULL) {
        HI_LOG_ERROR("crypto calloc for mbedtls gcm context failed\n");
        ret = HI_ERR_CIPHER_FAILED_MEM;
        HI_LOG_PRINT_FUNC_ERR(crypto_calloc, ret);
        return ret;
    }

    ret = crypto_mem_open(&mem_in, input[0], length[0]);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("open mem of input failed\n");
        ret = HI_ERR_CIPHER_FAILED_MEM;
        HI_LOG_PRINT_FUNC_ERR(crypto_mem_open, ret);
        goto error0;
    }

    ret = crypto_mem_open(&mem_out, output[0], length[0]);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("open mem of output failed\n");
        ret = HI_ERR_CIPHER_FAILED_MEM;
        HI_LOG_PRINT_FUNC_ERR(crypto_mem_open, ret);
        goto error1;
    }

    ret = crypto_mem_open(&aad, aead->aad, aead->alen);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("open mem of aad failed\n");
        ret = HI_ERR_CIPHER_FAILED_MEM;
        HI_LOG_PRINT_FUNC_ERR(crypto_mem_open, ret);
        goto error2;
    }

    mbedtls_gcm_init(gcm);

    ret = mbedtls_gcm_setkey(gcm, MBEDTLS_CIPHER_ID_AES, (hi_u8 *)aead->key,
                             aead->klen * BITS_IN_BYTE);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(mbedtls_gcm_setkey, ret);
        goto error3;
    }

    ret = mbedtls_gcm_starts(gcm, operation ? MBEDTLS_DECRYPT : MBEDTLS_ENCRYPT,
                             (hi_u8 *)aead->iv , aead->ivlen,
                             crypto_mem_virt(&aad), aead->alen);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(mbedtls_gcm_starts, ret);
        goto error3;
    }

    ret = mbedtls_gcm_update(gcm, length[0], crypto_mem_virt(&mem_in),
                             crypto_mem_virt(&mem_out));
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(mbedtls_gcm_update, ret);
        goto error3;
    }

    ret = mbedtls_gcm_finish(gcm, (hi_u8 *)aead->tag, aead->tlen);

error3:
    mbedtls_gcm_free(gcm);
    crypto_mem_close(&aad);
error2:
    crypto_mem_close(&mem_out);
error1:
    crypto_mem_close(&mem_in);
error0:
    crypto_free(gcm);
    gcm = HI_NULL;

    return ret;
}

#endif // End of SOFT_AES_CCM_GCM_SUPPORT
