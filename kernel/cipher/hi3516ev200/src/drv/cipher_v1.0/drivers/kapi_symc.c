/*****************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : kapi_symc.c
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include "drv_osal_lib.h"
#include "cryp_symc.h"
#include "hi_drv_compat.h"

/* max number of nodes */
#define MAX_PKG_NUMBER              (100000)

/* max length of CCM/GCM AAD */
#define MAX_AEAD_A_LEN              (0x100000)

typedef struct {
    hi_u32 open   : 1;                  /*!<  open or close */
    hi_u32 config : 1;                  /*!<  aleardy config or not */
    symc_func *func;
    void *cryp_ctx;                  /*!<  Context of cryp instance */
    crypto_owner owner;              /*!<  user ID */
    hi_cipher_ctrl  ctrl;        /*!<  control infomation */
    /* OpenIPC extension: the batch path's DMA bounce buffer, allocated on
     * first use and held until the channel is destroyed. The single-package
     * virtual path (kapi_symc_crypto_via) does a dma_alloc_coherent and a
     * free around EVERY packet, which at video packet rates is most of what
     * the engine costs. Batching would be pointless if it kept paying that. */
    crypto_mem batch;
    hi_u32 batch_size;               /*!<  bytes in batch, 0 = not allocated */
} kapi_symc_ctx;

/* How many packets one batched call may carry, and how much data. The node
 * count is bounded by the descriptor ring (SYMC_MAX_LIST_NUM, 16) less the
 * slot symc_add_buf_list() may need to square a block-size tail; the byte
 * budget is that many SRTP packets rounded up. */
#define KAPI_SYMC_BATCH_MAX_PKG     (15)
#define KAPI_SYMC_BATCH_MAX_LEN     (2048)
#define KAPI_SYMC_BATCH_BYTES       (KAPI_SYMC_BATCH_MAX_PKG * KAPI_SYMC_BATCH_MAX_LEN)

/*! Context of cipher */
static kapi_symc_ctx kapi_ctx[CRYPTO_HARD_CHANNEL_MAX];

/* symc mutex */
static crypto_mutex symc_mutex;

#define KAPI_SYMC_CHECK_HANDLE(handle)   \
    do \
    { \
        if((HI_HANDLE_GET_MODID(handle) != HI_ID_CIPHER) \
           || (HI_HANDLE_GET_PriDATA(handle) != 0)) \
        { \
            HI_LOG_ERROR("Invalid handle 0x%x!\n", handle); \
            return HI_ERR_CIPHER_INVALID_HANDLE; \
        } \
        if (HI_HANDLE_GET_CHNID(handle) >= CRYPTO_HARD_CHANNEL_MAX) \
        { \
            HI_LOG_ERROR("chan %d is too large, max: %d\n", HI_HANDLE_GET_CHNID(handle), CRYPTO_HARD_CHANNEL_MAX); \
            return HI_ERR_CIPHER_INVALID_HANDLE; \
        } \
        if (kapi_ctx[HI_HANDLE_GET_CHNID(handle)].open == HI_FALSE) \
        { \
            HI_LOG_ERROR("chan %d is not open\n", HI_HANDLE_GET_CHNID(handle)); \
            return HI_ERR_CIPHER_INVALID_HANDLE; \
        } \
    } while (0)

#define KAPI_SYMC_LOCK()   \
    ret = crypto_mutex_lock(&symc_mutex);  \
    if (ret != HI_SUCCESS)        \
    {\
        HI_LOG_ERROR("error, symc lock failed\n");\
        HI_LOG_PRINT_FUNC_ERR(crypto_mutex_lock, ret);\
        return ret;\
    }

#define KAPI_SYMC_UNLOCK()   crypto_mutex_unlock(&symc_mutex)
#define AES_CCM_MIN_TAG_LEN     (4)
#define AES_CCM_MAX_TAG_LEN     (16)
#define AES_GCM_MIN_TAG_LEN     (1)
#define AES_GCM_MAX_TAG_LEN     (16)

/** @}*/  /** <!-- ==== Structure Definition end ====*/

/******************************* API Code *****************************/
/** \addtogroup      symc */
/** @{*/  /** <!-- [kapi]*/

hi_s32 kapi_symc_init(void)
{
    hi_s32 ret = HI_FAILURE;

    HI_LOG_INFO("kapi_symc_init()\n");

    HI_LOG_FUNC_ENTER();

    crypto_mutex_init(&symc_mutex);

    crypto_memset(kapi_ctx, sizeof(kapi_ctx), 0, sizeof(kapi_ctx));

    ret = cryp_symc_init();
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(cryp_symc_init, ret);
        return ret;
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 kapi_symc_deinit(void)
{
    HI_LOG_INFO("kapi_symc_deinit()\n");

    HI_LOG_FUNC_ENTER();

    cryp_symc_deinit();

    crypto_mutex_destroy(&symc_mutex);

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 kapi_symc_release(void)
{
    hi_u32 i = 0, chn = 0;
    hi_s32 ret = HI_FAILURE;
    kapi_symc_ctx *ctx = HI_NULL;
    crypto_owner owner;

    HI_LOG_FUNC_ENTER();

    crypto_get_owner(&owner);

    HI_LOG_INFO("symc release owner 0x%x\n", owner);

    /* destroy the channel which are created by current user */
    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        ctx = &kapi_ctx[i];
        if (ctx->open == HI_TRUE) {
            if (memcmp(&owner, &ctx->owner, sizeof(owner)) == 0) {
                chn = HI_HANDLE_MAKEHANDLE(HI_ID_CIPHER, 0, i);
                HI_LOG_INFO("symc release chn %d\n", chn);
                ret = kapi_symc_destroy(chn);
                if (ret != HI_SUCCESS) {
                    HI_LOG_PRINT_FUNC_ERR(kapi_symc_destroy, ret);
                    return ret;
                }
            }
        }
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}


hi_s32 kapi_symc_create(hi_u32 *id)
{
    hi_s32 ret = HI_FAILURE;
    hi_u32 chn = 0;
    kapi_symc_ctx *ctx = HI_NULL;

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(id == HI_NULL);

    KAPI_SYMC_LOCK();

    /* allocate a aes soft channel for hard channel allocted */
    ret = cryp_symc_alloc_chn(&chn);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("error, allocate symc channel failed\n");
        HI_LOG_PRINT_FUNC_ERR(cryp_symc_alloc_chn, ret);
        KAPI_SYMC_UNLOCK();
        return ret;
    }
    ctx = &kapi_ctx[chn];

    crypto_memset(ctx, sizeof(kapi_symc_ctx), 0, sizeof(kapi_symc_ctx));
    crypto_get_owner(&ctx->owner);

    *id = HI_HANDLE_MAKEHANDLE(HI_ID_CIPHER, 0, chn);
    ctx->open = HI_TRUE;
    ctx->config = HI_FALSE;

    HI_LOG_INFO("kapi_symc_create()- chn %d\n", chn);

    KAPI_SYMC_UNLOCK();

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 kapi_symc_destroy(hi_u32 id)
{
    hi_s32 ret = HI_FAILURE;
    kapi_symc_ctx *ctx = HI_NULL;
    hi_u32 soft_id = 0;

    HI_LOG_FUNC_ENTER();

    KAPI_SYMC_CHECK_HANDLE(id);
    soft_id = HI_HANDLE_GET_CHNID(id);
    ctx = &kapi_ctx[soft_id];
    CHECK_OWNER(&ctx->owner);

    KAPI_SYMC_LOCK();

    /* Before the instance goes: this is the only owner of that buffer, and
     * with eight channels on the chip a leak here is eight batches' worth of
     * coherent DMA memory that nothing will ever reclaim. */
    if (ctx->batch_size != 0) {
        crypto_mem_destory(&ctx->batch);
        ctx->batch_size = 0;
    }

    cryp_symc_free_chn(soft_id);

    /* Destroy the attached instance of Symmetric cipher engine */
    if ((ctx->func != HI_NULL) && (ctx->func->destroy != HI_NULL)) {
        ret = ctx->func->destroy(ctx->cryp_ctx);
        if (ret != HI_SUCCESS) {
            HI_LOG_PRINT_FUNC_ERR(ctx->func->destroy, ret);
            KAPI_SYMC_UNLOCK();
            return ret;
        }
        ctx->cryp_ctx = HI_NULL;
    }

    ctx->open = HI_FALSE;

    HI_LOG_INFO("kapi_symc_destroy()- chn 0x%x\n", id);

    KAPI_SYMC_UNLOCK();

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

static hi_s32 kapi_symc_width_check(hi_cipher_alg alg, hi_cipher_work_mode mode, hi_u32 width, hi_u32 round)
{
    /* the bit width depend on alg and mode, which limit to hardware
     * des/3des with cfb/ofb support bit1, bit8, bit 64.
     * aes with cfb/ofb only support bit128.
     * sm1 with ofb only support bit128, cfb support bit1, bit8, bit 64.
     */

    HI_LOG_FUNC_ENTER();

    if ((alg == HI_CIPHER_ALG_3DES) || (alg == HI_CIPHER_ALG_DES)) {
#ifndef CHIP_DES_SUPPORT
        if (alg == HI_CIPHER_ALG_DES) {
            HI_LOG_ERROR("Invalid alg, unsupport des.\n");
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
            return HI_ERR_CIPHER_INVALID_PARA;
        }
#endif
#ifndef CHIP_3DES_SUPPORT
        if (alg == HI_CIPHER_ALG_3DES) {
            HI_LOG_ERROR("Invalid alg, unsupport 3des.\n");
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
            return HI_ERR_CIPHER_INVALID_PARA;
        }
#endif
        if (mode > HI_CIPHER_WORK_MODE_OFB) {
            HI_LOG_ERROR("Invalid alg %d and mode %d\n", alg, mode);
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
            return HI_ERR_CIPHER_INVALID_PARA;
        }

        if ((mode == HI_CIPHER_WORK_MODE_CFB) || (mode == HI_CIPHER_WORK_MODE_OFB)) {
            if ((width != SYMC_DAT_WIDTH_64)
                && (width != SYMC_DAT_WIDTH_8)
                && (width != SYMC_DAT_WIDTH_1)) {
                HI_LOG_ERROR("Invalid mode %d and bit width %d\n", mode, width);
                HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
                return HI_ERR_CIPHER_INVALID_PARA;
            }
        }
    }

    if (alg == HI_CIPHER_ALG_AES) {
        if (mode > HI_CIPHER_WORK_MODE_BUTT) {
            HI_LOG_ERROR("Invalid alg %d and mode %d\n", alg, mode);
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
            return HI_ERR_CIPHER_INVALID_PARA;
        }

        if ((mode == HI_CIPHER_WORK_MODE_CFB)
            && (width != SYMC_DAT_WIDTH_1)
            && (width != SYMC_DAT_WIDTH_8)
            && (width != SYMC_DAT_WIDTH_128)) {
            HI_LOG_ERROR("Invalid alg %d mode %d and width %d\n", alg, mode, width);
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
            return HI_ERR_CIPHER_INVALID_PARA;
        }

        if ((mode == HI_CIPHER_WORK_MODE_OFB)
            && (width != SYMC_DAT_WIDTH_128)) {
            HI_LOG_ERROR("Invalid alg %d mode %d and width %d\n", alg, mode, width);
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }

    if (alg == HI_CIPHER_ALG_SM1) {
        if (mode > HI_CIPHER_WORK_MODE_OFB) {
            HI_LOG_ERROR("Invalid alg %d and mode %d\n", alg, mode);
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
            return HI_ERR_CIPHER_INVALID_PARA;
        }

        if ((mode == HI_CIPHER_WORK_MODE_OFB)
            && (width != SYMC_DAT_WIDTH_128)) {
            HI_LOG_ERROR("Invalid alg %d mode %d and width %d\n", alg, mode, width);
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
            return HI_ERR_CIPHER_INVALID_PARA;
        }

        if ((mode == HI_CIPHER_WORK_MODE_CFB)
            && (width >= SYMC_DAT_WIDTH_COUNT)) {
            HI_LOG_ERROR("Invalid alg %d mode %d and width %d\n", alg, mode, width);
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }

    if ((alg == HI_CIPHER_ALG_SM4)
        && (mode != HI_CIPHER_WORK_MODE_ECB)
        && (mode != HI_CIPHER_WORK_MODE_CBC)
        && (mode != HI_CIPHER_WORK_MODE_CTR)) {
        HI_LOG_ERROR("Invalid alg %d and mode %d\n", alg, mode);
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if ((alg == HI_CIPHER_ALG_SM1) && (round >= HI_CIPHER_SM1_ROUND_BUTT)) {
        HI_LOG_ERROR("Invalid alg %d and Sm1Round %d\n", alg, round);
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (alg >= HI_CIPHER_ALG_BUTT) {
        HI_LOG_ERROR("Invalid alg %d .\n", alg);
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (width >= SYMC_DAT_WIDTH_COUNT) {
        HI_LOG_ERROR("Invalid mode %d\n", width);
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

static hi_s32 kapi_symc_match_width(hi_cipher_work_mode work_mode,
                                    hi_cipher_bit_width bit_width,
                                    symc_width *width)
{
    HI_LOG_FUNC_ENTER();

    /* set the bit width which depend on alg and mode */
    if ((HI_CIPHER_WORK_MODE_CFB == work_mode)
        || (HI_CIPHER_WORK_MODE_OFB == work_mode)) {
        switch (bit_width) {
            case HI_CIPHER_BIT_WIDTH_64BIT: {
                *width = SYMC_DAT_WIDTH_64;
                break;
            }
            case HI_CIPHER_BIT_WIDTH_8BIT: {
                *width = SYMC_DAT_WIDTH_8;
                break;
            }
            case HI_CIPHER_BIT_WIDTH_1BIT: {
                *width = SYMC_DAT_WIDTH_1;
                break;
            }
            case HI_CIPHER_BIT_WIDTH_128BIT: {
                *width = SYMC_DAT_WIDTH_128;
                break;
            }
            default: {
                HI_LOG_ERROR("Invalid width: 0x%x, mode 0x%x\n",
                             bit_width, work_mode);
                HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
                return HI_ERR_CIPHER_INVALID_PARA;
            }
        }
    } else {
        *width = SYMC_DAT_WIDTH_128;
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

static hi_s32 kapi_symc_check_param(hi_u32 hard_key, hi_u32 iv_usage,
                                    hi_cipher_alg alg,
                                    hi_cipher_work_mode work_mode,
                                    hi_cipher_bit_width bit_width,
                                    hi_cipher_key_length key_len,
                                    hi_cipher_sm1_round sm1_round_num,
                                    symc_width *width)
{
    hi_s32 ret = HI_FAILURE;

    HI_LOG_FUNC_ENTER();

    if (alg == HI_CIPHER_ALG_DMA) {
        HI_LOG_INFO("Alg is DMA.\n");
        HI_DBG_PRINT_U32(alg);
        return HI_SUCCESS;
    }

    if ((hard_key != HI_TRUE)  && (hard_key != HI_FALSE)) {
        HI_LOG_ERROR("Invalid hard_key: 0x%x\n", hard_key);
        return HI_ERR_CIPHER_INVALID_PARA;
    }
    if (key_len > HI_CIPHER_KEY_DES_2KEY) {
        HI_LOG_ERROR("Invalid key len: 0x%x\n", key_len);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    /* set the bit width which depend on alg and mode */
    ret = kapi_symc_match_width(work_mode, bit_width, width);
    if (ret != HI_SUCCESS) {
        HI_ERR_PRINT_U32(work_mode);
        HI_ERR_PRINT_U32(bit_width);
        HI_ERR_PRINT_U32(*width);
        HI_LOG_PRINT_FUNC_ERR(kapi_symc_match_width, ret);
        return ret;
    }

    ret = kapi_symc_width_check(alg, work_mode, *width, sm1_round_num);
    if (ret != HI_SUCCESS) {
        HI_ERR_PRINT_U32(alg);
        HI_ERR_PRINT_U32(work_mode);
        HI_ERR_PRINT_U32(*width);
        HI_ERR_PRINT_U32(sm1_round_num);
        HI_LOG_PRINT_FUNC_ERR(kapi_symc_width_check, ret);
        return ret;
    }

    if (iv_usage > CIPHER_IV_CHANGE_ALL_PKG) {
        HI_LOG_ERROR("Invalid IV Change Flags: 0x%x\n", iv_usage);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if ((iv_usage == CIPHER_IV_CHANGE_ALL_PKG)
        && ((work_mode == HI_CIPHER_WORK_MODE_CCM)
            || (work_mode == HI_CIPHER_WORK_MODE_GCM))) {
        HI_LOG_ERROR("Invalid IV Change Flags: 0x%x\n", iv_usage);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

static hi_s32 kapi_symc_check_ccm_gcm_taglen(hi_cipher_alg alg, hi_cipher_work_mode work_mode, hi_u32 tlen)
{
    HI_LOG_CHECK_PARAM(alg != HI_CIPHER_ALG_AES);

    if (work_mode == HI_CIPHER_WORK_MODE_CCM) {
        /* the parameter t denotes the octet length of T(tag)
         * t is an element of  { 4, 6, 8, 10, 12, 14, 16}
         * here t is pConfig->u32TagLen
         */
        if ((tlen & 0x01)
            || (tlen < AES_CCM_MIN_TAG_LEN)
            || (tlen > AES_CCM_MAX_TAG_LEN)) {
            HI_LOG_ERROR("Invalid ccm tag len, tlen = 0x%x.\n", tlen);
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    } else if (work_mode == HI_CIPHER_WORK_MODE_GCM) {
        if ((tlen < AES_GCM_MIN_TAG_LEN) || (tlen > AES_GCM_MAX_TAG_LEN)) {
            HI_LOG_ERROR("Invalid gcm tag len, tlen = 0x%x.\n", tlen);
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    } else {
        HI_LOG_ERROR("Aes with invalid work mode 0x%x for check tag length.\n", work_mode);
        HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    return HI_SUCCESS;
}

hi_s32 kapi_symc_config(hi_u32 id,
                     hi_u32 hard_key,
                     hi_cipher_alg alg,
                     hi_cipher_work_mode work_mode,
                     hi_cipher_bit_width bit_width,
                     hi_cipher_key_length key_len,
                     hi_cipher_sm1_round sm1_round_num,
                     hi_u8 *fkey, hi_u8 *skey,
                     hi_u8 *iv, hi_u32 ivlen, hi_u32 iv_usage,
                     compat_addr aad, hi_u32 alen, hi_u32 tlen)
{
    hi_s32 ret = HI_FAILURE;
    kapi_symc_ctx *ctx = HI_NULL;
    symc_width width = SYMC_DAT_WIDTH_COUNT;
    hi_u32 soft_id = 0;
    hi_u32 byca = HI_FALSE;
    hi_u32 ca_type = 0;
    hi_u32 klen = key_len;

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(fkey == HI_NULL);
    HI_LOG_CHECK_PARAM(alen > MAX_AEAD_A_LEN);
    HI_LOG_CHECK_PARAM(ADDR_L32(aad) + alen < alen);

    KAPI_SYMC_CHECK_HANDLE(id);
    soft_id = HI_HANDLE_GET_CHNID(id);
    ctx = &kapi_ctx[soft_id];
    CHECK_OWNER(&ctx->owner);

    /***
    hard_key: bit[0~7]  flag of hard key or not
              bit[8~31] ca type
    */
    byca = hard_key & 0xFF;
    ca_type = hard_key >> BITS_IN_BYTE;

    ret = kapi_symc_check_param(byca, iv_usage, alg, work_mode,
                                bit_width, key_len, sm1_round_num, &width);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("hard_key 0x%x\n", hard_key);
        HI_LOG_PRINT_FUNC_ERR(kapi_symc_check_param, ret);
        return ret;
    }

    KAPI_SYMC_LOCK();

    /* Destroy the last attached instance of Symmetric cipher engine */
    if ((ctx->func != HI_NULL) && (ctx->func->destroy != HI_NULL)) {
        (void)ctx->func->destroy(ctx->cryp_ctx);
    }
    ctx->cryp_ctx = HI_NULL;

    /* Clone the function from template of symc engine*/
    ctx->func = cryp_get_symc_op(alg, work_mode);

    if (ctx->func == HI_NULL) {
        HI_LOG_ERROR("error, get symc function failed, alg %d, work_mode %d\n",
                     alg, work_mode);
        HI_LOG_PRINT_FUNC_ERR(cryp_get_symc_op, ret);
        KAPI_SYMC_UNLOCK();
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    /* null means can ignore the function */
    if (ctx->func->create) {
        /* Create a instance from template of engine */
        ctx->cryp_ctx = ctx->func->create(soft_id);
        if (ctx->cryp_ctx == HI_NULL) {
            HI_LOG_ERROR("attach contxet buffer to soft_id %d failed\n", soft_id);
            HI_LOG_PRINT_FUNC_ERR(cryp_symc_create, ret);
            goto exit__;
        }
    }

    /* set mode and alg */
    if (ctx->func->setmode) {
        ctx->func->setmode(ctx->cryp_ctx, ctx->func->alg, ctx->func->mode, width);
    }

    /* Set even key, may be also need set odd key */
    if (ctx->func->setkey) {
        if (byca == HI_TRUE) {
            CHECK_EXIT(ctx->func->setkey(ctx->cryp_ctx, HI_NULL, HI_NULL, &klen));

            if (key_len == HI_CIPHER_KEY_AES_192BIT) {
                klen = AES_KEY_256BIT;
            }

            CHECK_EXIT(klad_load_hard_key(id, ca_type, fkey, klen));
        } else {
            CHECK_EXIT(ctx->func->setkey(ctx->cryp_ctx, fkey, skey, &klen));
        }
    }

    /* Set IV */
    if (ctx->func->setiv) {
        CHECK_EXIT(ctx->func->setiv(ctx->cryp_ctx, iv, ivlen, iv_usage));
    }

    /* set sm1 round num */
    if (ctx->func->setround) {
        CHECK_EXIT(ctx->func->setround(ctx->cryp_ctx, sm1_round_num));
    }

    /* Set AAD */
    if (ctx->func->setadd) {
        ret = cipher_check_mmz_phy_addr(ADDR_U64(aad), alen);
        if (ret != HI_SUCCESS) {
            HI_LOG_ERROR("Invalid aad mmz phy addr.\n");
            HI_LOG_PRINT_FUNC_ERR(cipher_check_mmz_phy_addr, ret);
            goto exit__;
        }

        HI_LOG_INFO("set add, phy 0x%x, alen %d, tlen %d\n", ADDR_L32(aad), alen, tlen);
        CHECK_EXIT(kapi_symc_check_ccm_gcm_taglen(alg, work_mode, tlen));
        CHECK_EXIT(ctx->func->setadd(ctx->cryp_ctx, aad, alen, tlen));
    }

    /* save crtl */
    crypto_memset(&ctx->ctrl, sizeof(hi_cipher_ctrl), 0, sizeof(hi_cipher_ctrl));
    ctx->ctrl.key_by_ca = byca;
    ctx->ctrl.alg = alg;
    ctx->ctrl.bit_width = bit_width;
    ctx->ctrl.ca_type = ca_type;
    ctx->ctrl.key_len = key_len;
    ctx->ctrl.work_mode = work_mode;
    ctx->ctrl.change_flags.bit1_iv = iv_usage;

    if (iv != HI_NULL) {
        if (ivlen > AES_IV_SIZE) {
            HI_LOG_ERROR("Invalid iv len.\n");
            ret = HI_ERR_CIPHER_INVALID_PARA;
            goto exit__;
        }

        crypto_memcpy(ctx->ctrl.iv, AES_IV_SIZE, iv, ivlen);
    }
    if (fkey != HI_NULL) {
        if (klen > AES_KEY_256BIT) {
            HI_LOG_ERROR("Invalid key len.\n");
            ret = HI_ERR_CIPHER_INVALID_PARA;
            goto exit__;
        }

        crypto_memcpy(ctx->ctrl.key, AES_KEY_256BIT, fkey, klen);
    }

    ctx->config = HI_TRUE;

    KAPI_SYMC_UNLOCK();
    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;

exit__:
    KAPI_SYMC_UNLOCK();

    return ret;
}

hi_s32 kapi_symc_get_config(hi_u32 id, hi_cipher_ctrl *ctrl)
{
    hi_s32 ret = HI_FAILURE;
    kapi_symc_ctx *ctx = HI_NULL;
    hi_u32 soft_id = 0;

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(ctrl == HI_NULL);

    KAPI_SYMC_CHECK_HANDLE(id);
    soft_id = HI_HANDLE_GET_CHNID(id);
    ctx = &kapi_ctx[soft_id];
    CHECK_OWNER(&ctx->owner);
    HI_LOG_CHECK_PARAM(ctx->config != HI_TRUE);

    KAPI_SYMC_LOCK();

    crypto_memcpy(ctrl, sizeof(hi_cipher_ctrl), &ctx->ctrl, sizeof(hi_cipher_ctrl));
    crypto_zeroize(ctrl->key, sizeof(ctrl->key));

    KAPI_SYMC_UNLOCK();

    HI_LOG_FUNC_EXIT();

    return ret;
}

hi_s32 kapi_symc_crypto(hi_u32 id, compat_addr input,
                     compat_addr output, hi_u32 length,
                     hi_u32 operation, hi_u32 last)
{
    hi_s32 ret = HI_FAILURE;
    symc_node_usage usage;
    kapi_symc_ctx *ctx = HI_NULL;
    hi_u32 soft_id = 0;

    HI_LOG_FUNC_ENTER();

    KAPI_SYMC_CHECK_HANDLE(id);
    soft_id = HI_HANDLE_GET_CHNID(id);
    ctx = &kapi_ctx[soft_id];
    CHECK_OWNER(&ctx->owner);
    HI_LOG_CHECK_PARAM(ADDR_U64(input) + length < length);
    HI_LOG_CHECK_PARAM(ADDR_U64(output) + length < length);
    HI_LOG_CHECK_PARAM(ctx->func == HI_NULL);
    HI_LOG_CHECK_PARAM(ctx->func->crypto == HI_NULL);
    HI_LOG_CHECK_PARAM(ctx->config != HI_TRUE);
    HI_LOG_CHECK_PARAM((operation != 0x00) && (operation != 0x01));

    HI_LOG_INFO("src/dest phyaddr information.\n");
    HI_DBG_PRINT_U32(operation);
    HI_DBG_PRINT_H32(ADDR_L32(input));
    HI_DBG_PRINT_H32(ADDR_L32(output));
    HI_DBG_PRINT_H32(length);

    usage = SYMC_NODE_USAGE_NORMAL;

    KAPI_SYMC_LOCK();

    ret = ctx->func->crypto(ctx->cryp_ctx, operation, &input,
                            &output, &length, &usage, 1, HI_TRUE);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(ctx->func->crypto, ret);
        KAPI_SYMC_UNLOCK();
        return ret;
    }

    KAPI_SYMC_UNLOCK();

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 kapi_symc_crypto_via(hi_u32 id, compat_addr input,
                         compat_addr output, hi_u32 length,
                         hi_u32 operation, hi_u32 last, hi_u32 is_from_user)
{
    hi_s32 ret = HI_FAILURE;
    hi_s32 ret_exit = HI_FAILURE;
    crypto_mem mem = {0};

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(ADDR_VIA(input) == HI_NULL);
    HI_LOG_CHECK_PARAM(ADDR_VIA(output) == HI_NULL);
    HI_LOG_CHECK_PARAM(length == 0x00);

    ret = crypto_mem_create(&mem, SEC_MMZ, "AES_IN", length);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(crypto_mem_create, ret);
        return ret;
    }

    if (is_from_user == HI_TRUE) {
        ret = crypto_copy_from_user(mem.dma_virt, ADDR_VIA(input), length);
        if (ret != HI_SUCCESS) {
            HI_LOG_PRINT_FUNC_ERR(crypto_copy_from_user, ret);
            goto exit;
        }
    } else {
        crypto_memcpy(mem.dma_virt, length, ADDR_VIA(input), length);
    }

    ret = kapi_symc_crypto(id, mem.dma_addr, mem.dma_addr, length, operation & 0x01, last);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(kapi_symc_crypto, ret);
        goto exit;
    }

    if (is_from_user == HI_TRUE) {
        ret = crypto_copy_to_user(ADDR_VIA(output), mem.dma_virt, length);
        if (ret != HI_SUCCESS) {
            HI_LOG_PRINT_FUNC_ERR(crypto_copy_to_user, ret);
            goto exit;
        }
    } else {
        crypto_memcpy(ADDR_VIA(output), length, mem.dma_virt, length);
    }

exit:
    ret_exit = crypto_mem_destory(&mem);
    if (ret_exit != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(crypto_mem_destory, ret_exit);
        HI_ERR_PRINT_S32(ret);
        return ret_exit;
    }

    HI_LOG_FUNC_EXIT();
    return ret;
}

/* OpenIPC extension: one ioctl, one DMA buffer, one hardware start and one
 * completion interrupt for a whole burst of packets, each under its own IV.
 * See symc_encrypt_via_multi_t for why the vendor's multi-package call cannot
 * do this and why the silicon can.
 *
 * SHAPE OF THE WORK, because it is what the cost is made of. Per package the
 * old path pays: two ioctls, a dma_alloc_coherent/free pair, a
 * copy_from_user, a hardware start, a sleep on the completion IRQ and a
 * copy_to_user. Here the ioctl, the allocation, the start and the sleep are
 * paid once for the batch; only the two copies stay per package, and those
 * are a memcpy each.
 *
 * EVERY PACKAGE IS PADDED TO THE BLOCK SIZE, and that is not cosmetic.
 * symc_add_buf_list() squares a job whose total length is not a multiple of
 * the block size by SPLITTING a node in two — which under a per-node IV list
 * would run the second half under the next packet's IV and produce a
 * plausible, wrong keystream. Padding each package makes every total aligned,
 * so the splitting path is never entered. CTR is a stream cipher, so the
 * padding costs bytes and changes nothing. */
static hi_s32 kapi_symc_crypto_via_multi_run(kapi_symc_ctx *ctx,
                                             const symc_via_pkg *pkg,
                                             hi_u32 pkg_num,
                                             hi_u32 operation)
{
    hi_s32 ret = HI_FAILURE;
    compat_addr input[KAPI_SYMC_BATCH_MAX_PKG];
    compat_addr output[KAPI_SYMC_BATCH_MAX_PKG];
    hi_u32 length[KAPI_SYMC_BATCH_MAX_PKG];
    symc_node_usage usage[KAPI_SYMC_BATCH_MAX_PKG];
    hi_u8 ivs[KAPI_SYMC_BATCH_MAX_PKG][AES_IV_SIZE];
    hi_u32 offset[KAPI_SYMC_BATCH_MAX_PKG];
    hi_u32 i, used = 0;

    for (i = 0; i < pkg_num; i++) {
        const hi_u32 len = pkg[i].length;
        const hi_u32 padded =
            (len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE * AES_BLOCK_SIZE;

        if ((len == 0) || (len > KAPI_SYMC_BATCH_MAX_LEN)) {
            HI_LOG_ERROR("batch pkg %d: bad length 0x%x\n", i, len);
            return HI_ERR_CIPHER_INVALID_LENGTH;
        }
        if ((used + padded > ctx->batch_size) || (used + padded < used)) {
            HI_LOG_ERROR("batch overflows its buffer at pkg %d\n", i);
            return HI_ERR_CIPHER_INVALID_LENGTH;
        }
        if (ADDR_VIA(pkg[i].src) == HI_NULL ||
            ADDR_VIA(pkg[i].dst) == HI_NULL) {
            return HI_ERR_CIPHER_INVALID_POINT;
        }

        ret = crypto_copy_from_user((hi_u8 *)ctx->batch.dma_virt + used,
                                    ADDR_VIA(pkg[i].src), len);
        if (ret != HI_SUCCESS) {
            HI_LOG_PRINT_FUNC_ERR(crypto_copy_from_user, ret);
            return ret;
        }
        if (padded > len) {
            crypto_memset((hi_u8 *)ctx->batch.dma_virt + used + len,
                          padded - len, 0, padded - len);
        }

        crypto_memcpy(ivs[i], AES_IV_SIZE, pkg[i].iv, AES_IV_SIZE);
        offset[i] = used;
        length[i] = padded;
        usage[i] = SYMC_NODE_USAGE_NORMAL;
        ADDR_U64(input[i]) = ADDR_U64(ctx->batch.dma_addr) + used;
        ADDR_U64(output[i]) = ADDR_U64(ctx->batch.dma_addr) + used;
        used += padded;
    }

    /* In place: the engine reads and writes the same node, which halves the
     * buffer and costs nothing — the block reads a node before it writes it. */
    ret = ctx->func->setivlist(ctx->cryp_ctx, (const hi_u8 *)ivs, pkg_num,
                               pkg_num);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(ctx->func->setivlist, ret);
        return ret;
    }

    ret = ctx->func->crypto(ctx->cryp_ctx, operation, input, output, length,
                            usage, pkg_num, HI_TRUE);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(ctx->func->crypto, ret);
        return ret;
    }

    /* Only the requested length goes back, so the padding never reaches the
     * caller's buffer — the same contract the single-package path keeps. */
    for (i = 0; i < pkg_num; i++) {
        ret = crypto_copy_to_user(ADDR_VIA(pkg[i].dst),
                                  (hi_u8 *)ctx->batch.dma_virt + offset[i],
                                  pkg[i].length);
        if (ret != HI_SUCCESS) {
            HI_LOG_PRINT_FUNC_ERR(crypto_copy_to_user, ret);
            return ret;
        }
    }

    return HI_SUCCESS;
}

hi_s32 kapi_symc_crypto_via_multi(hi_u32 id, const void *pkg, hi_u32 pkg_num,
                                  hi_u32 operation)
{
    hi_s32 ret = HI_FAILURE;
    kapi_symc_ctx *ctx = HI_NULL;
    symc_via_pkg desc[KAPI_SYMC_BATCH_MAX_PKG];
    hi_u32 soft_id = 0;

    HI_LOG_FUNC_ENTER();

    KAPI_SYMC_CHECK_HANDLE(id);
    soft_id = HI_HANDLE_GET_CHNID(id);
    ctx = &kapi_ctx[soft_id];
    CHECK_OWNER(&ctx->owner);
    HI_LOG_CHECK_PARAM(ctx->config != HI_TRUE);
    HI_LOG_CHECK_PARAM(pkg == HI_NULL);
    HI_LOG_CHECK_PARAM(pkg_num == 0x00);
    HI_LOG_CHECK_PARAM(pkg_num > KAPI_SYMC_BATCH_MAX_PKG);
    HI_LOG_CHECK_PARAM((operation != 0x00) && (operation != 0x01));
    HI_LOG_CHECK_PARAM(ctx->func == HI_NULL);
    HI_LOG_CHECK_PARAM(ctx->func->crypto == HI_NULL);

    /* The software (mbedTLS) implementations register no list setter, and
     * CCM/GCM/CTS clear theirs because their crypto() does not read it. So a
     * NULL here is "this algorithm cannot be batched", not a bug — the caller
     * falls back to the single-package call, as it would on a camera with no
     * engine at all. */
    if (ctx->func->setivlist == HI_NULL) {
        HI_LOG_INFO("batch unsupported for this alg/mode\n");
        return HI_ERR_CIPHER_UNSUPPORTED;
    }

    /* CTR ONLY, and the reason is the padding below rather than the IV list.
     * Every package is rounded up to a whole block and only the requested
     * bytes are handed back, which is invisible in a stream cipher and wrong
     * in every other mode: a CBC caller would get ciphertext truncated
     * mid-block on the way out, and a decrypt would run over fabricated zero
     * bytes. The block itself rejects an unaligned length outside CTR
     * (drv_symc_node_check), so padding here would be quietly relaxing a
     * check the hardware makes. Widening this needs a per-mode length rule,
     * not just a flag. */
    if (ctx->ctrl.work_mode != HI_CIPHER_WORK_MODE_CTR) {
        HI_LOG_INFO("batch is CTR only, mode %d\n", ctx->ctrl.work_mode);
        return HI_ERR_CIPHER_UNSUPPORTED;
    }

    ret = crypto_copy_from_user(desc, pkg, sizeof(symc_via_pkg) * pkg_num);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(crypto_copy_from_user, ret);
        return ret;
    }

    KAPI_SYMC_LOCK();

    /* Everything above was read without the lock, which is the shape the
     * vendor's own crypto paths use. It is not good enough here: destroy()
     * frees the batch buffer and the crypto context under this same lock, and
     * create() may then hand the slot to somebody else — so a request that
     * waited would arrive holding conclusions about a channel that no longer
     * exists. Re-checked, cheaply, now that nothing can move. */
    if ((ctx->open != HI_TRUE) || (ctx->config != HI_TRUE) ||
        (ctx->func == HI_NULL) || (ctx->func->setivlist == HI_NULL) ||
        (ctx->cryp_ctx == HI_NULL)) {
        KAPI_SYMC_UNLOCK();
        return HI_ERR_CIPHER_INVALID_HANDLE;
    }

    if (ctx->batch_size == 0) {
        ret = crypto_mem_create(&ctx->batch, SEC_MMZ, "AES_BATCH",
                                KAPI_SYMC_BATCH_BYTES);
        if (ret != HI_SUCCESS) {
            HI_LOG_PRINT_FUNC_ERR(crypto_mem_create, ret);
            KAPI_SYMC_UNLOCK();
            return ret;
        }
        ctx->batch_size = KAPI_SYMC_BATCH_BYTES;
    }

    ret = kapi_symc_crypto_via_multi_run(ctx, desc, pkg_num, operation & 0x01);

    KAPI_SYMC_UNLOCK();

    HI_LOG_FUNC_EXIT();
    return ret;
}

static hi_s32 kapi_symc_crypto_multi_start(kapi_symc_ctx *ctx, const void *pkg, hi_u32 pkg_num, hi_u32 operation, hi_u32 wait)
{
    hi_s32 ret = HI_FAILURE;
    void *buf = HI_NULL, *temp = HI_NULL;
    compat_addr *input = HI_NULL;
    compat_addr *output = HI_NULL;
    symc_node_usage *usage = HI_NULL;
    hi_cipher_data pkg_tmp;
    hi_u32 *length = HI_NULL;
    hi_u32 size = 0;
    hi_u32 i;

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(ctx->func == HI_NULL);
    HI_LOG_CHECK_PARAM(ctx->func->crypto == HI_NULL);
    HI_LOG_CHECK_PARAM(pkg == HI_NULL);
    HI_LOG_CHECK_PARAM(pkg_num > MAX_PKG_NUMBER);
    HI_LOG_CHECK_PARAM(pkg_num == 0x00);

    /* size of input:output:usage:length */
    size = (sizeof(compat_addr) + sizeof(compat_addr) + sizeof(hi_u32) + sizeof(hi_u32)) * pkg_num;

    buf = crypto_malloc(size);
    if (buf == HI_NULL) {
        HI_LOG_ERROR("Malloc for pkg failed.\n");
        HI_LOG_PRINT_FUNC_ERR(crypto_malloc, ret);
        return HI_ERR_CIPHER_FAILED_MEM;
    }

    temp = buf;
    input = (compat_addr *)temp;
    temp = (hi_u8 *)temp + sizeof(compat_addr) * pkg_num; /*buf + input*/
    output = (compat_addr *)temp;
    temp = (hi_u8 *)temp + sizeof(compat_addr) * pkg_num; /*buf + input + output*/
    usage = temp;
    temp = (hi_u8 *)temp + sizeof(hi_u32) * pkg_num; /*buf + input + output + usage*/
    length = temp;

    /*Compute and check the nodes length*/
    for (i = 0; i < pkg_num; i++) {
        /*copy node list from user space to kernel*/
        ret = crypto_copy_from_user(&pkg_tmp, (hi_u8 *)pkg + sizeof(hi_cipher_data) * i,
                                    sizeof(hi_cipher_data));
        if (ret != HI_SUCCESS) {
            HI_LOG_ERROR("copy data from user fail!\n");
            HI_LOG_PRINT_FUNC_ERR(crypto_copy_from_user, ret);
            crypto_free(buf);
            buf = HI_NULL;
            return ret;
        }

        if (pkg_tmp.src_phy_addr + pkg_tmp.byte_length < pkg_tmp.byte_length) {
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
            crypto_free(buf);
            buf = HI_NULL;
            return HI_ERR_CIPHER_INVALID_PARA;
        }

        if (pkg_tmp.dest_phy_addr + pkg_tmp.byte_length < pkg_tmp.byte_length) {
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
            crypto_free(buf);
            buf = HI_NULL;
            return HI_ERR_CIPHER_INVALID_PARA;
        }

        if ((pkg_tmp.odd_key != HI_TRUE) && (pkg_tmp.odd_key != HI_FALSE)) {
            HI_LOG_ERROR("invalid odd key for multicipher crypt!\n");
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA);
            crypto_free(buf);
            buf = HI_NULL;
            return HI_ERR_CIPHER_INVALID_PARA;
        }

        ret = cipher_check_mmz_phy_addr(pkg_tmp.src_phy_addr, pkg_tmp.byte_length);
        if (ret != HI_SUCCESS) {
            HI_LOG_ERROR("Invalid output mmz phy addr for multicipher crypt.\n");
            HI_LOG_PRINT_FUNC_ERR(cipher_check_mmz_phy_addr, ret);
            crypto_free(buf);
            buf = HI_NULL;
            return ret;
        }

        ret = cipher_check_mmz_phy_addr(pkg_tmp.dest_phy_addr, pkg_tmp.byte_length);
        if (ret != HI_SUCCESS) {
            HI_LOG_ERROR("Invalid output mmz phy addr for multicipher crypt.\n");
            HI_LOG_PRINT_FUNC_ERR(cipher_check_mmz_phy_addr, ret);
            crypto_free(buf);
            buf = HI_NULL;
            return ret;
        }

        ADDR_U64(input[i]) = pkg_tmp.src_phy_addr;
        ADDR_U64(output[i]) = pkg_tmp.dest_phy_addr;
        length[i] = pkg_tmp.byte_length;
        usage[i] = SYMC_NODE_USAGE_EVEN_KEY;

        HI_LOG_DEBUG("pkg %d, in 0x%x, out 0x%x, length 0x%x, usage 0x%x\n", i,
                     ADDR_L32(input[i]), ADDR_L32(output[i]), length[i], usage[i]);
    }

    ret = ctx->func->crypto(ctx->cryp_ctx, operation, input,
                            output, length, usage, pkg_num, wait);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(cryp_symc_crypto, ret);
        crypto_free(buf);
        buf = HI_NULL;
        return ret;
    }

    crypto_free(buf);
    buf = HI_NULL;

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 kapi_symc_crypto_multi(hi_u32 id, const void *pkg, hi_u32 pkg_num, hi_u32 operation, hi_u32 last)
{
    hi_s32 ret = HI_FAILURE;
    kapi_symc_ctx *ctx = HI_NULL;
    hi_u32 soft_id = 0;

    HI_LOG_FUNC_ENTER();

    KAPI_SYMC_CHECK_HANDLE(id);
    soft_id = HI_HANDLE_GET_CHNID(id);
    ctx = &kapi_ctx[soft_id];
    CHECK_OWNER(&ctx->owner);
    HI_LOG_CHECK_PARAM(ctx->func == HI_NULL);
    HI_LOG_CHECK_PARAM(ctx->config != HI_TRUE);
    HI_LOG_CHECK_PARAM((operation != 0x00) && (operation != 0x01));

    KAPI_SYMC_LOCK();

    ret = kapi_symc_crypto_multi_start(ctx, pkg, pkg_num, operation, HI_TRUE);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(kapi_symc_crypto_multi_start, ret);
        KAPI_SYMC_UNLOCK();
        return ret;
    }

    KAPI_SYMC_UNLOCK();

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 kapi_aead_get_tag(hi_u32 id, hi_u32 tag[AEAD_TAG_SIZE_IN_WORD], hi_u32 *taglen)
{
    hi_s32 ret = HI_FAILURE;
    kapi_symc_ctx *ctx = HI_NULL;
    hi_u32 soft_id = 0;

    HI_LOG_FUNC_ENTER();

    HI_LOG_CHECK_PARAM(tag == HI_NULL);
    HI_LOG_CHECK_PARAM(taglen == HI_NULL);
    HI_LOG_CHECK_PARAM(*taglen != AES_CCM_MAX_TAG_LEN);

    KAPI_SYMC_CHECK_HANDLE(id);
    soft_id = HI_HANDLE_GET_CHNID(id);
    ctx = &kapi_ctx[soft_id];
    CHECK_OWNER(&ctx->owner);
    HI_LOG_CHECK_PARAM(ctx->func == HI_NULL);
    HI_LOG_CHECK_PARAM(ctx->func->gettag == HI_NULL);

    KAPI_SYMC_LOCK();

    if (ctx->func->gettag) {
        ret = ctx->func->gettag(ctx->cryp_ctx, tag, taglen);
        if (ret != HI_SUCCESS) {
            HI_LOG_PRINT_FUNC_ERR(cryp_aead_get_tag, ret);
            KAPI_SYMC_UNLOCK();
            return ret;
        }
    }

    KAPI_SYMC_UNLOCK();

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

/** @}*/  /** <!-- ==== Structure Definition end ====*/
