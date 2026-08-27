/*****************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : ext_symc.c
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include "drv_osal_lib.h"
#include "cryp_symc.h"
#include "mbedtls/cipher.h"

#if defined(SOFT_AES_SUPPORT) || defined(SOFT_TDES_SUPPORT)

/**
 * \brief          aes ccm context structure
 */
typedef struct {
    hi_u32 key[SYMC_KEY_SIZE / 4];    /*!<  SYMC even round keys, default */
    hi_u32 klen;                /*!<  symc key length */
    mbedtls_cipher_id_t cipher_id;
    mbedtls_cipher_mode_t mode;
    mbedtls_cipher_context_t cipher;
}
ext_symc_context;

void *ext_mbedtls_symc_create(hi_u32 hard_chn)
{
    ext_symc_context *ctx = HI_NULL;

    HI_LOG_FUNC_ENTER();

    ctx = crypto_malloc(sizeof(ext_symc_context));
    if (ctx == HI_NULL) {
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_FAILED_MEM);
        HI_LOG_ERROR("malloc failed \n");
        return HI_NULL;
    }
    crypto_memset(ctx, sizeof(ext_symc_context), 0, sizeof(ext_symc_context));

    mbedtls_cipher_init(&ctx->cipher);

    HI_LOG_FUNC_EXIT();

    return ctx;
}

hi_s32 ext_mbedtls_symc_destory(void *ctx)
{
    ext_symc_context *symc = ctx;

    HI_LOG_FUNC_ENTER();

    if (ctx != HI_NULL) {
        mbedtls_cipher_free(&symc->cipher);
        crypto_free(ctx);
        ctx = HI_NULL;
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 ext_mbedtls_symc_setmode(void *ctx, symc_alg alg, symc_mode mode, symc_width width)
{
    ext_symc_context *symc = ctx;

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(symc == HI_NULL);
    HI_LOG_CHECK_PARAM(width != SYMC_DAT_WIDTH_128);

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
            HI_LOG_ERROR("unsupport alg %d\n", alg);
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
            return HI_ERR_CIPHER_INVALID_PARA;
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
            HI_LOG_ERROR("unsupport mode %d\n", mode);
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
            return HI_ERR_CIPHER_INVALID_PARA;
    }

    HI_LOG_DEBUG("cipher_id %d, mode %d\n", symc->cipher_id, symc->mode);

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 ext_mbedtls_symc_setiv(void *ctx, const hi_u8 *iv, hi_u32 ivlen, hi_u32 usage)
{
    hi_s32 ret = HI_FAILURE;
    ext_symc_context *symc = ctx;

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(symc == HI_NULL);
    HI_LOG_CHECK_PARAM(iv   == HI_NULL);
    HI_LOG_CHECK_PARAM(ivlen > AES_IV_SIZE);

    ret = mbedtls_cipher_set_iv(&symc->cipher, iv, ivlen);

    HI_LOG_FUNC_EXIT();

    return ret;
}

hi_s32 ext_mbedtls_symc_getiv(void *ctx, hi_u8 *iv, hi_u32 *ivlen)
{
    ext_symc_context *symc = ctx;

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(symc  == HI_NULL);
    HI_LOG_CHECK_PARAM(iv    == HI_NULL);
    HI_LOG_CHECK_PARAM(ivlen == HI_NULL);

    crypto_memcpy(iv, AES_IV_SIZE, symc->cipher.iv, symc->cipher.iv_size);
    *ivlen = symc->cipher.iv_size;

    HI_LOG_FUNC_EXIT();

    return HI_SUCCESS;
}

hi_s32 ext_mbedtls_symc_setkey(void *ctx, const hi_u8 *fkey, const hi_u8 *skey, hi_u32 *hisi_klen)
{
    hi_s32 ret = HI_FAILURE;
    hi_u32 klen = 0;
    ext_symc_context *symc = ctx;
    const mbedtls_cipher_info_t *info = HI_NULL;

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(symc == HI_NULL);
    HI_LOG_CHECK_PARAM(fkey == HI_NULL);
    HI_LOG_CHECK_PARAM(hisi_klen == HI_NULL);

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
                HI_LOG_ERROR("Invalid aes key len: 0x%x\n", klen);
                HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
                return HI_ERR_CIPHER_INVALID_PARA;
        }
        crypto_memcpy(symc->key, SYMC_KEY_SIZE, fkey, klen);
    } else if (symc->cipher_id == MBEDTLS_CIPHER_ID_3DES) {
        klen = TDES_KEY_192BIT;
        crypto_memcpy(symc->key, SYMC_KEY_SIZE, fkey, klen);
        if (*hisi_klen == HI_CIPHER_KEY_DES_2KEY) {
            /* k3 = k1*/
            symc->key[4] = symc->key[0];
            symc->key[5] = symc->key[1];
        }
    } else if (symc->cipher_id == MBEDTLS_CIPHER_ID_DES) {
        klen = DES_KEY_SIZE;
        crypto_memcpy(symc->key, SYMC_KEY_SIZE, fkey, klen);
    }
    HI_LOG_INFO("key len %d, type %d\n", klen, *hisi_klen);

    symc->klen = klen;

    HI_LOG_DEBUG("cipher_id %d, klen %d, mode %d\n", symc->cipher_id, klen, symc->mode);
    info = mbedtls_cipher_info_from_values(symc->cipher_id, klen * 8, symc->mode);
    HI_LOG_CHECK_PARAM(info == HI_NULL);

    ret = mbedtls_cipher_setup(&symc->cipher, info);

    *hisi_klen = klen;

    HI_LOG_FUNC_EXIT();

    return ret;
}

hi_s32 ext_mbedtls_symc_crypto(void *ctx,
                            hi_u32 operation,
                            compat_addr input[],
                            compat_addr output[],
                            hi_u32 length[],
                            symc_node_usage usage_list[],
                            hi_u32 pkg_num, hi_u32 last)
{
    ext_symc_context *symc = ctx;
    crypto_mem mem_in, mem_out;
    hi_u32 offset = 0;
    size_t olen = 0;
    hi_s32 ret = HI_FAILURE;

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(symc   == HI_NULL);
    HI_LOG_CHECK_PARAM(input  == HI_NULL);
    HI_LOG_CHECK_PARAM(output == HI_NULL);
    HI_LOG_CHECK_PARAM(length == HI_NULL);
    HI_LOG_CHECK_PARAM(pkg_num != 0x01);

    ret = mbedtls_cipher_setkey(&symc->cipher, (hi_u8 *)symc->key, symc->klen * 8,
                                operation ? MBEDTLS_DECRYPT : MBEDTLS_ENCRYPT);
    if (ret != HI_SUCCESS)
    { return ret; }

    ret = crypto_mem_open(&mem_in, input[0], length[0]);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("open mem of input failed\n");
        ret = HI_ERR_CIPHER_FAILED_MEM;
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
        goto error0;
    }

    ret = crypto_mem_open(&mem_out, output[0], length[0]);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("open mem of output failed\n");
        ret = HI_ERR_CIPHER_FAILED_MEM;
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
        goto error1;
    }

    HI_LOG_DEBUG("symc 0x%p, klen len: %d\n", symc, symc->klen);

    if (symc->mode == MBEDTLS_MODE_ECB) {
        offset = 0;
        while (offset < length[0]) {
            ret = mbedtls_cipher_update(&symc->cipher, crypto_mem_virt(&mem_in) + offset,
                                        mbedtls_cipher_get_block_size(&symc->cipher),
                                        crypto_mem_virt(&mem_out) + offset, &olen);
            if (ret != HI_SUCCESS) {
                HI_LOG_ERROR("mbedtls_cipher_update failed\n");
                break;
            }
            offset += mbedtls_cipher_get_block_size(&symc->cipher);
        }
    } else {
        ret = mbedtls_cipher_update(&symc->cipher, crypto_mem_virt(&mem_in),
                                    length[0], crypto_mem_virt(&mem_out), &olen);
    }

    HI_LOG_FUNC_EXIT();

    crypto_mem_close(&mem_out);
error1:
    crypto_mem_close(&mem_in);
error0:

    return ret;
}
#endif
