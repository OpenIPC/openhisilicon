/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : drivers for kapi symc of cipher.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#include "drv_osal_lib.h"
#include "cryp_symc.h"
#include "hi_drv_compat.h"

/* max number of nodes */
#define MAX_PKG_NUMBER                  100000

/* max length of CCM/GCM AAD */
#define MAX_AEAD_A_LEN                  0x100000

typedef struct {
    hi_u32 open   : 1;                  /* open or close */
    hi_u32 config : 1;                  /* aleardy config or not */
    symc_func *func;
    void *cryp_ctx;                     /* Context of cryp instance */
    crypto_owner owner;                 /* user ID */
    hi_cipher_ctrl  ctrl;               /* control infomation */
} kapi_symc_ctx;

typedef struct {
    hi_u32 soft_id;
    symc_width width;
    hi_u32 byca;
    hi_u32 ca_type;
    hi_u32 klen;
} symc_set_cfg_param;

/*! Context of cipher */
static kapi_symc_ctx g_kapi_ctx[CRYPTO_HARD_CHANNEL_MAX];

/* symc mutex */
static crypto_mutex g_symc_mutex;

#define kapi_symc_lock_err_return()   \
    do { \
        ret = crypto_mutex_lock(&g_symc_mutex);  \
        if (ret != HI_SUCCESS) { \
            hi_log_error("error, symc lock failed\n"); \
            hi_log_print_func_err(crypto_mutex_lock, ret); \
            return ret; \
        } \
    } while (0)

#define kapi_symc_unlock()          crypto_mutex_unlock(&g_symc_mutex)
#define AES_CCM_MIN_TAG_LEN         4
#define AES_CCM_MAX_TAG_LEN         16
#define AES_GCM_MIN_TAG_LEN         1
#define AES_GCM_MAX_TAG_LEN         16

/* ****************************** API Code **************************** */
static hi_s32 kapi_symc_chk_handle(hi_handle handle)
{
    if ((hi_handle_get_modid(handle) != HI_ID_CIPHER) || (hi_handle_get_private_data(handle) != 0)) {
        hi_log_error("Invalid handle 0x%x!\n", handle);
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_HANDLE);
        return HI_ERR_CIPHER_INVALID_HANDLE;
    }

    if (hi_handle_get_chnid(handle) >= CRYPTO_HARD_CHANNEL_MAX) {
        hi_log_error("chan %d is too large, max: %d\n", hi_handle_get_chnid(handle), CRYPTO_HARD_CHANNEL_MAX);
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_HANDLE);
        return HI_ERR_CIPHER_INVALID_HANDLE;
    }

    if (g_kapi_ctx[hi_handle_get_chnid(handle)].open == HI_FALSE) {
        hi_log_error("chan %d is not open\n", hi_handle_get_chnid(handle));
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_HANDLE);
        return HI_ERR_CIPHER_INVALID_HANDLE;
    }

    return HI_SUCCESS;
}

hi_s32 kapi_symc_init(void)
{
    hi_s32 ret;

    hi_log_info("kapi_symc_init()\n");

    hi_log_func_enter();

    crypto_mutex_init(&g_symc_mutex);

    crypto_memset(g_kapi_ctx, sizeof(g_kapi_ctx), 0, sizeof(g_kapi_ctx));

    ret = cryp_symc_init();
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cryp_symc_init, ret);
        return ret;
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 kapi_symc_deinit(void)
{
    hi_log_info("kapi_symc_deinit()\n");

    hi_log_func_enter();

    cryp_symc_deinit();

    crypto_mutex_destroy(&g_symc_mutex);

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 kapi_symc_release(void)
{
    hi_u32 i, chn;
    hi_s32 ret;
    kapi_symc_ctx *ctx = HI_NULL;
    crypto_owner owner;

    hi_log_func_enter();

    crypto_get_owner(&owner);

    hi_log_info("symc release owner 0x%x\n", owner);

    /* destroy the channel which are created by current user */
    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        ctx = &g_kapi_ctx[i];
        if (ctx->open == HI_TRUE) {
            if (memcmp(&owner, &ctx->owner, sizeof(owner)) != 0) {
                continue;
            }
            chn = hi_handle_makehandle(HI_ID_CIPHER, 0, i);
            hi_log_info("symc release chn %d\n", chn);
            ret = kapi_symc_destroy(chn);
            if (ret != HI_SUCCESS) {
                hi_log_print_func_err(kapi_symc_destroy, ret);
                return ret;
            }
        }
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 kapi_symc_create(hi_u32 *id)
{
    hi_s32 ret;
    hi_u32 chn = 0;
    kapi_symc_ctx *ctx = HI_NULL;

    hi_log_func_enter();

    hi_log_chk_param_return(id == HI_NULL);

    kapi_symc_lock_err_return();

    /* allocate a aes soft channel for hard channel allocted */
    ret = cryp_symc_alloc_chn(&chn);
    if (ret != HI_SUCCESS) {
        hi_log_error("error, allocate symc channel failed\n");
        hi_log_print_func_err(cryp_symc_alloc_chn, ret);
        kapi_symc_unlock();
        return ret;
    }
    ctx = &g_kapi_ctx[chn];

    crypto_memset(ctx, sizeof(kapi_symc_ctx), 0, sizeof(kapi_symc_ctx));
    crypto_get_owner(&ctx->owner);

    *id = hi_handle_makehandle(HI_ID_CIPHER, 0, chn);
    ctx->open = HI_TRUE;
    ctx->config = HI_FALSE;

    hi_log_info("kapi_symc_create()- chn %d\n", chn);

    kapi_symc_unlock();

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 kapi_symc_destroy(hi_u32 id)
{
    hi_s32 ret;
    kapi_symc_ctx *ctx = HI_NULL;
    hi_u32 soft_id;

    hi_log_func_enter();

    ret = kapi_symc_chk_handle((hi_handle)id);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(kapi_symc_chk_handle, ret);
        return ret;
    }

    soft_id = hi_handle_get_chnid(id);
    ctx = &g_kapi_ctx[soft_id];
    crypto_chk_owner_err_return(&ctx->owner);

    kapi_symc_lock_err_return();

    cryp_symc_free_chn(soft_id);

    /* Destroy the attached instance of Symmetric cipher engine */
    if ((ctx->func != HI_NULL) && (ctx->func->destroy != HI_NULL)) {
        ret = ctx->func->destroy(ctx->cryp_ctx);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(ctx->func->destroy, ret);
            kapi_symc_unlock();
            return ret;
        }
        ctx->cryp_ctx = HI_NULL;
    }

    ctx->open = HI_FALSE;

    hi_log_info("kapi_symc_destroy()- chn 0x%x\n", id);

    kapi_symc_unlock();

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 kapi_symc_chk_des_3des_param(hi_cipher_alg alg, hi_cipher_work_mode mode, hi_u32 width)
{
#ifndef CHIP_DES_SUPPORT
    if (alg == HI_CIPHER_ALG_DES) {
        hi_log_error("Invalid alg, unsupport des.\n");
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARA);
        return HI_ERR_CIPHER_INVALID_PARA;
    }
#endif
#ifndef CHIP_3DES_SUPPORT
    if (alg == HI_CIPHER_ALG_3DES) {
        hi_log_error("Invalid alg, unsupport 3des.\n");
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARA);
        return HI_ERR_CIPHER_INVALID_PARA;
    }
#endif
    if (mode > HI_CIPHER_WORK_MODE_OFB) {
        hi_log_error("Invalid alg %d and mode %d\n", alg, mode);
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARA);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if ((mode == HI_CIPHER_WORK_MODE_CFB) || (mode == HI_CIPHER_WORK_MODE_OFB)) {
        if ((width != SYMC_DAT_WIDTH_64) && (width != SYMC_DAT_WIDTH_8) && (width != SYMC_DAT_WIDTH_1)) {
            hi_log_error("Invalid mode %d and bit width %d\n", mode, width);
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARA);
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }

    return HI_SUCCESS;
}

static hi_s32 kapi_symc_chk_aes_param(hi_cipher_alg alg, hi_cipher_work_mode mode, hi_u32 width)
{
    if (mode > HI_CIPHER_WORK_MODE_BUTT) {
        hi_log_error("Invalid alg %d and mode %d\n", alg, mode);
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    if ((mode == HI_CIPHER_WORK_MODE_CFB) && (width != SYMC_DAT_WIDTH_1) && (width != SYMC_DAT_WIDTH_8) &&
        (width != SYMC_DAT_WIDTH_128)) {
        hi_log_error("Invalid alg %d mode %d and width %d\n", alg, mode, width);
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    if ((mode == HI_CIPHER_WORK_MODE_OFB) && (width != SYMC_DAT_WIDTH_128)) {
        hi_log_error("Invalid alg %d mode %d and width %d\n", alg, mode, width);
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    return HI_SUCCESS;
}

static hi_s32 kapi_symc_chk_sm1_param(hi_cipher_alg alg, hi_cipher_work_mode mode, hi_u32 width, hi_u32 round)
{
    if (mode > HI_CIPHER_WORK_MODE_OFB) {
        hi_log_error("Invalid alg %d and mode %d\n", alg, mode);
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    if ((mode == HI_CIPHER_WORK_MODE_OFB)
        && (width != SYMC_DAT_WIDTH_128)) {
        hi_log_error("Invalid alg %d mode %d and width %d\n", alg, mode, width);
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    if ((mode == HI_CIPHER_WORK_MODE_CFB)
        && (width >= SYMC_DAT_WIDTH_COUNT)) {
        hi_log_error("Invalid alg %d mode %d and width %d\n", alg, mode, width);
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    if ((alg == HI_CIPHER_ALG_SM1) && (round >= HI_CIPHER_SM1_ROUND_BUTT)) {
        hi_log_error("Invalid alg %d and Sm1Round %d\n", alg, round);
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    return HI_SUCCESS;
}

static hi_s32 kapi_symc_chk_sm4_param(hi_cipher_alg alg, hi_cipher_work_mode mode)
{
    if ((mode != HI_CIPHER_WORK_MODE_ECB) && (mode != HI_CIPHER_WORK_MODE_CBC) && (mode != HI_CIPHER_WORK_MODE_CTR)) {
        hi_log_error("Invalid alg %d and mode %d\n", alg, mode);
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    return HI_SUCCESS;
}

static hi_s32 kapi_symc_width_check(hi_cipher_alg alg, hi_cipher_work_mode mode, hi_u32 width, hi_u32 round)
{
    hi_s32 ret;

    /* the bit width depend on alg and mode, which limit to hardware
     * des/3des with cfb/ofb support bit1, bit8, bit 64.
     * aes with cfb/ofb only support bit128.
     * sm1 with ofb only support bit128, cfb support bit1, bit8, bit 64.
     */
    hi_log_func_enter();

    hi_log_chk_param_return(alg >= HI_CIPHER_ALG_BUTT);
    hi_log_chk_param_return((alg != HI_CIPHER_ALG_DMA) && (mode >= HI_CIPHER_WORK_MODE_BUTT));
    hi_log_chk_param_return((alg != HI_CIPHER_ALG_DMA) && (width >= SYMC_DAT_WIDTH_COUNT));

    if ((alg == HI_CIPHER_ALG_3DES) || (alg == HI_CIPHER_ALG_DES)) {
        ret = kapi_symc_chk_des_3des_param(alg, mode, width);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(kapi_symc_chk_des_3des_param, ret);
            return ret;
        }
    }

    if (alg == HI_CIPHER_ALG_AES) {
        ret = kapi_symc_chk_aes_param(alg, mode, width);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(kapi_symc_chk_aes_param, ret);
            return ret;
        }
    }

    if (alg == HI_CIPHER_ALG_SM1) {
        ret = kapi_symc_chk_sm1_param(alg, mode, width, round);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(kapi_symc_chk_sm1_param, ret);
            return ret;
        }
    }

    if (alg == HI_CIPHER_ALG_SM4) {
        ret = kapi_symc_chk_sm4_param(alg, mode);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(kapi_symc_chk_sm4_param, ret);
            return ret;
        }
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 kapi_symc_match_width(hi_cipher_work_mode work_mode, hi_cipher_bit_width bit_width, symc_width *width)
{
    hi_log_func_enter();

    /* set the bit width which depend on alg and mode */
    if ((work_mode == HI_CIPHER_WORK_MODE_CFB) || (work_mode == HI_CIPHER_WORK_MODE_OFB)) {
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
                hi_log_error("Invalid width: 0x%x, mode 0x%x\n", bit_width, work_mode);
                hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
                return HI_ERR_CIPHER_INVALID_PARAM;
            }
        }
    } else {
        if (bit_width == HI_CIPHER_BIT_WIDTH_128BIT) {
            *width = SYMC_DAT_WIDTH_128;
        }
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 kapi_symc_chk_param(hi_u32 hard_key, symc_cfg_t *cfg, symc_width *width)
{
    hi_s32 ret;

    hi_log_func_enter();

    if (cfg->alg == HI_CIPHER_ALG_DMA) {
        hi_log_info("Alg is DMA.\n");
        hi_dbg_print_u32(cfg->alg);
        return HI_SUCCESS;
    }

    if ((hard_key != HI_TRUE) && (hard_key != HI_FALSE)) {
        hi_log_error("Invalid hard_key: 0x%x\n", hard_key);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }
    if (cfg->klen > HI_CIPHER_KEY_DES_2KEY) {
        hi_log_error("Invalid key len: 0x%x\n", cfg->klen);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    /* set the bit width which depend on alg and mode */
    ret = kapi_symc_match_width(cfg->mode, cfg->width, width);
    if (ret != HI_SUCCESS) {
        hi_err_print_u32(cfg->mode);
        hi_err_print_u32(cfg->width);
        hi_err_print_u32(*width);
        hi_log_print_func_err(kapi_symc_match_width, ret);
        return ret;
    }

    ret = kapi_symc_width_check(cfg->alg, cfg->mode, *width, cfg->sm1_round_num);
    if (ret != HI_SUCCESS) {
        hi_err_print_u32(cfg->alg);
        hi_err_print_u32(cfg->mode);
        hi_err_print_u32(*width);
        hi_err_print_u32(cfg->sm1_round_num);
        hi_log_print_func_err(kapi_symc_width_check, ret);
        return ret;
    }

    if (cfg->iv_usage > HI_CIPHER_IV_CHG_ALL_PACK) {
        hi_log_error("Invalid IV Change Flags: 0x%x\n", cfg->iv_usage);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    if ((cfg->iv_usage == HI_CIPHER_IV_CHG_ALL_PACK) &&
        ((cfg->mode == HI_CIPHER_WORK_MODE_CCM) || (cfg->mode == HI_CIPHER_WORK_MODE_GCM))) {
        hi_log_error("Invalid IV Change Flags: 0x%x\n", cfg->iv_usage);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 kapi_symc_check_ccm_gcm_taglen(hi_cipher_alg alg, hi_cipher_work_mode work_mode, hi_u32 tlen)
{
    hi_log_chk_param_return(alg != HI_CIPHER_ALG_AES);

    if (work_mode == HI_CIPHER_WORK_MODE_CCM) {
        /* the parameter t denotes the octet length of T(tag)
         * t is an element of  { 4, 6, 8, 10, 12, 14, 16}
         * here t is pConfig->u32TagLen
         */
        if ((tlen & 0x01) || (tlen < AES_CCM_MIN_TAG_LEN) || (tlen > AES_CCM_MAX_TAG_LEN)) {
            hi_log_error("Invalid ccm tag len, tlen = 0x%x.\n", tlen);
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
            return HI_ERR_CIPHER_INVALID_PARAM;
        }
    } else if (work_mode == HI_CIPHER_WORK_MODE_GCM) {
        if ((tlen < AES_GCM_MIN_TAG_LEN) || (tlen > AES_GCM_MAX_TAG_LEN)) {
            hi_log_error("Invalid gcm tag len, tlen = 0x%x.\n", tlen);
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
            return HI_ERR_CIPHER_INVALID_PARAM;
        }
    } else {
        hi_log_error("Aes with invalid work mode 0x%x for check tag length.\n", work_mode);
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    return HI_SUCCESS;
}

static hi_s32 kapi_symc_chk_cfg(symc_cfg_t *cfg, symc_set_cfg_param *set_cfg)
{
    hi_s32 ret;
    kapi_symc_ctx *ctx = HI_NULL;

    hi_log_chk_param_return(cfg == HI_NULL);
    hi_log_chk_param_return(cfg->alen > MAX_AEAD_A_LEN);
    hi_log_chk_param_return(addr_l32(cfg->aad) + cfg->alen < cfg->alen);

    ret = kapi_symc_chk_handle((hi_handle)cfg->id);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(kapi_symc_chk_handle, ret);
        return ret;
    }

    set_cfg->soft_id = hi_handle_get_chnid(cfg->id);
    ctx = &g_kapi_ctx[set_cfg->soft_id];
    if (ctx == HI_NULL) {
        hi_log_error("kapi symc ctx is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    crypto_chk_owner_err_return(&ctx->owner);

    /*
     * hard_key: bit[0~7]  flag of hard key or not
     *           bit[8~31] ca type
     */
    set_cfg->byca = cfg->hard_key & 0xFF;
    set_cfg->ca_type = cfg->hard_key >> BITS_IN_BYTE;

    ret = kapi_symc_chk_param(set_cfg->byca, cfg, &set_cfg->width);
    if (ret != HI_SUCCESS) {
        hi_log_error("hard_key 0x%x\n", cfg->hard_key);
        hi_log_print_func_err(kapi_symc_chk_param, ret);
        return ret;
    }

    set_cfg->klen = (hi_u32)cfg->klen;

    return HI_SUCCESS;
}

static hi_void kapi_symc_set_alg_mode(kapi_symc_ctx *ctx, symc_set_cfg_param *set_cfg)
{
    /* set mode and alg */
    if (ctx->func->setmode) {
        ctx->func->setmode(ctx->cryp_ctx, ctx->func->alg, ctx->func->mode, set_cfg->width);
    }
}

static hi_s32 kapi_symc_set_key(symc_cfg_t *cfg, kapi_symc_ctx *ctx, symc_set_cfg_param *set_cfg)
{
    hi_s32 ret;

    if (ctx->func->setkey == HI_NULL) {
        return HI_SUCCESS;
    }

    if (set_cfg->byca == HI_TRUE) {
        ret = ctx->func->setkey(ctx->cryp_ctx, HI_NULL, HI_NULL, &set_cfg->klen);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(ctx->func->setkey, ret);
            return ret;
        }

        if (cfg->klen == HI_CIPHER_KEY_AES_192BIT) {
            set_cfg->klen = AES_KEY_256BIT;
        }

        ret = klad_load_hard_key(cfg->id, set_cfg->ca_type, cfg->fkey, set_cfg->klen);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(klad_load_hard_key, ret);
            return ret;
        }
    } else {
        ret = ctx->func->setkey(ctx->cryp_ctx, cfg->fkey, cfg->skey, &set_cfg->klen);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(ctx->func->setkey, ret);
            return ret;
        }
    }

    return HI_SUCCESS;
}

static hi_s32 kapi_symc_cpy_key_iv(symc_cfg_t *cfg, kapi_symc_ctx *ctx, symc_set_cfg_param *set_cfg)
{
    if (cfg->iv != HI_NULL) {
        if (cfg->ivlen > AES_IV_SIZE) {
            hi_log_error("Invalid iv len.\n");
            return HI_ERR_CIPHER_INVALID_PARAM;
        }

        crypto_memcpy(ctx->ctrl.iv, AES_IV_SIZE, cfg->iv, cfg->ivlen);
    }

    if (cfg->fkey != HI_NULL) {
        if (set_cfg->klen > AES_KEY_256BIT) {
            hi_log_error("Invalid key len.\n");
            return HI_ERR_CIPHER_INVALID_PARAM;
        }

        crypto_memcpy(ctx->ctrl.key, AES_KEY_256BIT, cfg->fkey, set_cfg->klen);
    }

    return HI_SUCCESS;
}

static hi_s32 kapi_symc_cfg_set_param(symc_cfg_t *cfg, kapi_symc_ctx *ctx, symc_set_cfg_param *set_cfg)
{
    hi_s32 ret;

    /* null means can ignore the function */
    if (ctx->func->create) {
        /* Create a instance from template of engine */
        ctx->cryp_ctx = ctx->func->create(set_cfg->soft_id);
        if (ctx->cryp_ctx == HI_NULL) {
            hi_log_error("attach contxet buffer to soft_id %d failed\n", set_cfg->soft_id);
            hi_log_print_func_err(ctx->func->create, HI_ERR_CIPHER_FAILED_MEM);
            return HI_ERR_CIPHER_FAILED_MEM;
        }
    }

    /* set mode and alg */
    kapi_symc_set_alg_mode(ctx, set_cfg);

    /* Set even key, may be also need set odd key */
    crypto_chk_err_exit(kapi_symc_set_key(cfg, ctx, set_cfg));

    /* Set IV */
    if (ctx->func->setiv) {
        crypto_chk_err_exit(ctx->func->setiv(ctx->cryp_ctx, cfg->iv, cfg->ivlen, cfg->iv_usage));
    }

    /* set sm1 round num */
    if (ctx->func->setround) {
        crypto_chk_err_exit(ctx->func->setround(ctx->cryp_ctx, cfg->sm1_round_num));
    }

    /* Set AAD */
    if (ctx->func->setadd) {
        crypto_chk_err_exit(cipher_check_mmz_phy_addr(addr_u64(cfg->aad), cfg->alen));
        hi_log_info("set add, phy 0x%x, alen %d, tlen %d\n", addr_l32(cfg->aad), cfg->alen, cfg->tlen);
        crypto_chk_err_exit(kapi_symc_check_ccm_gcm_taglen(cfg->alg, cfg->mode, cfg->tlen));
        crypto_chk_err_exit(ctx->func->setadd(ctx->cryp_ctx, cfg->aad, cfg->alen, cfg->tlen));
    }

    /* save crtl */
    crypto_memset(&ctx->ctrl, sizeof(hi_cipher_ctrl), 0, sizeof(hi_cipher_ctrl));
    ctx->ctrl.key_by_ca = set_cfg->byca;
    ctx->ctrl.alg = cfg->alg;
    ctx->ctrl.bit_width = cfg->width;
    ctx->ctrl.ca_type = set_cfg->ca_type;
    ctx->ctrl.key_len = cfg->klen;
    ctx->ctrl.work_mode = cfg->mode;
    ctx->ctrl.chg_flags.bits_iv = cfg->iv_usage;

    crypto_chk_err_exit(kapi_symc_cpy_key_iv(cfg, ctx, set_cfg));
    ctx->config = HI_TRUE;
    return HI_SUCCESS;
exit__:
    return ret;
}

hi_s32 kapi_symc_cfg(symc_cfg_t *cfg)
{
    hi_s32 ret;
    kapi_symc_ctx *ctx = HI_NULL;
    symc_set_cfg_param set_cfg;

    hi_log_func_enter();

    crypto_memset(&set_cfg, sizeof(set_cfg), 0, sizeof(set_cfg));

    ret = kapi_symc_chk_cfg(cfg, &set_cfg);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(kapi_symc_chk_cfg, ret);
        return ret;
    }
    ctx = &g_kapi_ctx[set_cfg.soft_id];

    kapi_symc_lock_err_return();

    /* Destroy the last attached instance of Symmetric cipher engine. */
    if ((ctx->func != HI_NULL) && (ctx->func->destroy != HI_NULL)) {
        (void)ctx->func->destroy(ctx->cryp_ctx);
    }
    ctx->cryp_ctx = HI_NULL;

    /* Clone the function from template of symc engine. */
    ctx->func = cryp_get_symc_op(cfg->alg, cfg->mode);
    if (ctx->func == HI_NULL) {
        hi_log_error("error, get symc function failed, alg %d, work_mode %d\n", cfg->alg, cfg->mode);
        hi_log_print_func_err(cryp_get_symc_op, ret);
        kapi_symc_unlock();
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    ret = kapi_symc_cfg_set_param(cfg, ctx, &set_cfg);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(klad_load_hard_key, ret);
        kapi_symc_unlock();
        return ret;
    }

    kapi_symc_unlock();
    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 kapi_symc_get_cfg(hi_u32 id, hi_cipher_ctrl *ctrl)
{
    hi_s32 ret;
    kapi_symc_ctx *ctx = HI_NULL;
    hi_u32 soft_id;

    hi_log_func_enter();

    hi_log_chk_param_return(ctrl == HI_NULL);

    ret = kapi_symc_chk_handle((hi_handle)id);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(kapi_symc_chk_handle, ret);
        return ret;
    }

    soft_id = hi_handle_get_chnid(id);
    ctx = &g_kapi_ctx[soft_id];
    crypto_chk_owner_err_return(&ctx->owner);
    hi_log_chk_param_return(ctx->config != HI_TRUE);

    kapi_symc_lock_err_return();

    crypto_memcpy(ctrl, sizeof(hi_cipher_ctrl), &ctx->ctrl, sizeof(hi_cipher_ctrl));
    crypto_zeroize(ctrl->key, sizeof(ctrl->key));

    kapi_symc_unlock();

    hi_log_func_exit();

    return ret;
}

hi_s32 kapi_symc_crypto(symc_encrypt_t *crypt)
{
    hi_s32 ret;
    symc_node_usage usage;
    kapi_symc_ctx *ctx = HI_NULL;
    hi_u32 soft_id;
    symc_multi_pack pack;

    hi_log_func_enter();
    hi_log_chk_param_return(crypt == HI_NULL);
    ret = kapi_symc_chk_handle((hi_handle)crypt->id);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(kapi_symc_chk_handle, ret);
        return ret;
    }

    soft_id = hi_handle_get_chnid(crypt->id);
    ctx = &g_kapi_ctx[soft_id];
    crypto_chk_owner_err_return(&ctx->owner);
    hi_log_chk_param_return(addr_u64(crypt->in) + crypt->len < crypt->len);
    hi_log_chk_param_return(addr_u64(crypt->out) + crypt->len < crypt->len);
    hi_log_chk_param_return(ctx->func == HI_NULL);
    hi_log_chk_param_return(ctx->func->crypto == HI_NULL);
    hi_log_chk_param_return(ctx->config != HI_TRUE);
    hi_log_chk_param_return((crypt->operation != SYMC_OPERATION_ENCRYPT) &&
        (crypt->operation != SYMC_OPERATION_DECRYPT));

    hi_log_info("src/dest phyaddr information.\n");
    hi_dbg_print_u32(crypt->operation);
    hi_dbg_print_h32(addr_l32(crypt->in));
    hi_dbg_print_h32(addr_l32(crypt->out));
    hi_dbg_print_h32(crypt->len);

    usage = SYMC_NODE_USAGE_NORMAL;
    crypto_memset(&pack, sizeof(pack), 0, sizeof(pack));
    pack.in = &crypt->in;
    pack.out = &crypt->out;
    pack.len = &crypt->len;
    pack.usage = &usage;
    pack.num = 1; /* 1 single package encrypt or decrypt. */

    kapi_symc_lock_err_return();

    ret = ctx->func->crypto(ctx->cryp_ctx, crypt->operation, &pack, HI_TRUE);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(ctx->func->crypto, ret);
        kapi_symc_unlock();
        return ret;
    }

    kapi_symc_unlock();
    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 kapi_symc_crypto_via(symc_encrypt_t *crypt, hi_u32 is_from_user)
{
    hi_s32 ret, ret_exit;
    crypto_mem mem = {0};
    symc_encrypt_t local_crypt;

    hi_log_func_enter();

    hi_log_chk_param_return(crypt == HI_NULL);
    hi_log_chk_param_return(addr_via(crypt->in) == HI_NULL);
    hi_log_chk_param_return(addr_via(crypt->out) == HI_NULL);
    hi_log_chk_param_return(crypt->len == 0x00);

    crypto_memset(&local_crypt, sizeof(local_crypt), 0, sizeof(local_crypt));

    ret = crypto_mem_create(&mem, SEC_MMZ, "AES_IN", crypt->len);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_mem_create, ret);
        return ret;
    }

    if (is_from_user == HI_TRUE) {
        crypto_chk_err_exit(crypto_copy_from_user(mem.dma_virt, addr_via(crypt->in), crypt->len));
    } else {
        crypto_memcpy(mem.dma_virt, crypt->len, addr_via(crypt->in), crypt->len);
    }

    local_crypt.id   = crypt->id;
    local_crypt.in   = mem.dma_addr;
    local_crypt.out  = mem.dma_addr;
    local_crypt.len  = crypt->len;
    local_crypt.last = crypt->last;
    local_crypt.operation = crypt->operation & SYMC_OPERATION_DECRYPT;
    crypto_chk_err_exit(kapi_symc_crypto(&local_crypt));

    if (is_from_user == HI_TRUE) {
        crypto_chk_err_exit(crypto_copy_to_user(addr_via(crypt->out), mem.dma_virt, crypt->len));
    } else {
        crypto_memcpy(addr_via(crypt->out), crypt->len, mem.dma_virt, crypt->len);
    }

exit__:
    ret_exit = crypto_mem_destory(&mem);
    if (ret_exit != HI_SUCCESS) {
        hi_log_print_func_err(crypto_mem_destory, ret_exit);
        return ret_exit;
    }

    hi_log_func_exit();
    return ret;
}

static hi_s32 kapi_symc_chk_multi_pack(hi_cipher_data *tmp, hi_cipher_data *pack)
{
    hi_s32 ret;

    /* copy node list from user space to kernel. */
    ret = crypto_copy_from_user(tmp, pack, sizeof(hi_cipher_data));
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_copy_from_user, ret);
        return ret;
    }

    if (tmp->src_phys_addr + tmp->byte_len < tmp->byte_len) {
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    if (tmp->dst_phys_addr + tmp->byte_len < tmp->byte_len) {
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    if ((tmp->odd_key != HI_TRUE) && (tmp->odd_key != HI_FALSE)) {
        hi_log_error("invalid odd key for multicipher crypt!\n");
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM);
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    ret = cipher_check_mmz_phy_addr(tmp->src_phys_addr, tmp->byte_len);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cipher_check_mmz_phy_addr, ret);
        return ret;
    }

    ret = cipher_check_mmz_phy_addr(tmp->dst_phys_addr, tmp->byte_len);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(cipher_check_mmz_phy_addr, ret);
        return ret;
    }

    return HI_SUCCESS;
}

static hi_void kapi_symc_multi_pack_set_mem(symc_multi_pack *pack, hi_u8 *buf, hi_u32 size)
{
    hi_u8 *tmp = buf;
    hi_u32 pack_num = pack->num;

    crypto_unused(size);

    tmp = buf;
    pack->in = (compat_addr *)tmp;

    tmp = (hi_u8 *)tmp + sizeof(compat_addr) * pack_num;    /* descrypt: buf + input. */
    pack->out = (compat_addr *)tmp;

    tmp = (hi_u8 *)tmp + sizeof(compat_addr) * pack_num;    /* descrypt: buf + input + output. */
    pack->usage = (symc_node_usage *)tmp;

    tmp = (hi_u8 *)tmp + sizeof(symc_node_usage) * pack_num;    /* descrypt: buf + input + output + usage. */
    pack->len = (hi_u32 *)tmp;
}

static hi_s32 kapi_symc_crypto_multi_start(kapi_symc_ctx *ctx, const hi_void *pkg, hi_u32 pkg_num, hi_u32 operation,
    hi_u32 wait)
{
    hi_s32 ret;
    hi_void *buf = HI_NULL;
    hi_cipher_data pkg_tmp;
    hi_u32 i, size;
    symc_multi_pack pack;

    hi_log_func_enter();

    hi_log_chk_param_return((ctx == HI_NULL) || (ctx->func == HI_NULL) || (ctx->func->crypto == HI_NULL));
    hi_log_chk_param_return((pkg == HI_NULL) || (pkg_num > MAX_PKG_NUMBER) || (pkg_num == 0x00));

    /* size of input:output:usage:length */
    size = (sizeof(compat_addr) + sizeof(compat_addr) + sizeof(symc_node_usage) + sizeof(hi_u32)) * pkg_num;
    buf = crypto_calloc(1, size);
    if (buf == HI_NULL) {
        hi_log_print_func_err(crypto_calloc, HI_ERR_CIPHER_FAILED_MEM);
        return HI_ERR_CIPHER_FAILED_MEM;
    }

    crypto_memset(&pack, sizeof(pack), 0, sizeof(pack));

    pack.num = pkg_num;
    kapi_symc_multi_pack_set_mem(&pack, buf, size);

    /* Compute and check the nodes length. */
    for (i = 0; i < pkg_num; i++) {
        ret = kapi_symc_chk_multi_pack(&pkg_tmp, (hi_cipher_data *)((hi_u8 *)pkg + sizeof(hi_cipher_data) * i));
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(kapi_symc_chk_multi_pack, ret);
            crypto_free(buf);
            buf = HI_NULL;
            return ret;
        }

        addr_u64(pack.in[i]) = pkg_tmp.src_phys_addr;
        addr_u64(pack.out[i]) = pkg_tmp.dst_phys_addr;
        pack.len[i] = pkg_tmp.byte_len;
        pack.usage[i] = SYMC_NODE_USAGE_EVEN_KEY;

        hi_log_debug("pkg %d, in 0x%x, out 0x%x, length 0x%x, usage 0x%x\n", i, addr_l32(pack.in[i]),
            addr_l32(pack.out[i]), pack.len[i], pack.usage[i]);
    }

    ret = ctx->func->crypto(ctx->cryp_ctx, operation, &pack, wait);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(ctx->func->crypto, ret);
        crypto_free(buf);
        buf = HI_NULL;
        return ret;
    }

    crypto_free(buf);
    buf = HI_NULL;

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 kapi_symc_crypto_multi(hi_u32 id, const void *pkg, hi_u32 pkg_num, hi_u32 operation, hi_u32 last)
{
    hi_s32 ret;
    kapi_symc_ctx *ctx = HI_NULL;
    hi_u32 soft_id;

    hi_log_func_enter();

    ret = kapi_symc_chk_handle((hi_handle)id);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(kapi_symc_chk_handle, ret);
        return ret;
    }

    soft_id = hi_handle_get_chnid(id);
    ctx = &g_kapi_ctx[soft_id];
    crypto_chk_owner_err_return(&ctx->owner);
    hi_log_chk_param_return(ctx->func == HI_NULL);
    hi_log_chk_param_return(ctx->config != HI_TRUE);
    hi_log_chk_param_return((operation != 0x00) && (operation != 0x01));

    kapi_symc_lock_err_return();

    ret = kapi_symc_crypto_multi_start(ctx, pkg, pkg_num, operation, HI_TRUE);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(kapi_symc_crypto_multi_start, ret);
        kapi_symc_unlock();
        return ret;
    }

    kapi_symc_unlock();

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 kapi_aead_get_tag(hi_u32 id, hi_u32 tag[AEAD_TAG_SIZE_IN_WORD], hi_u32 *taglen)
{
    hi_s32 ret;
    kapi_symc_ctx *ctx = HI_NULL;
    hi_u32 soft_id;

    hi_log_func_enter();

    hi_log_chk_param_return(tag == HI_NULL);
    hi_log_chk_param_return(taglen == HI_NULL);
    hi_log_chk_param_return(*taglen != AES_CCM_MAX_TAG_LEN);

    ret = kapi_symc_chk_handle((hi_handle)id);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(kapi_symc_chk_handle, ret);
        return ret;
    }

    soft_id = hi_handle_get_chnid(id);
    ctx = &g_kapi_ctx[soft_id];
    crypto_chk_owner_err_return(&ctx->owner);
    hi_log_chk_param_return(ctx->func == HI_NULL);
    hi_log_chk_param_return(ctx->func->gettag == HI_NULL);

    kapi_symc_lock_err_return();

    if (ctx->func->gettag) {
        ret = ctx->func->gettag(ctx->cryp_ctx, tag, taglen);
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(cryp_aead_get_tag, ret);
            kapi_symc_unlock();
            return ret;
        }
    }

    kapi_symc_unlock();

    hi_log_func_exit();
    return HI_SUCCESS;
}
