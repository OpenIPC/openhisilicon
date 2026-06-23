/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "ot_reg.h"
#include <linux/module.h>
#include <linux/crypto.h>
#include <crypto/algapi.h>
#include <linux/scatterlist.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <asm/cacheflush.h>

osal_mutex aes_lock;

static int alg_name_2_workmode(const char *alg_name, crypto_symc_work_mode *mode)
{
    if (!strcmp(alg_name, "cbc(aes)")) {
        *mode = CRYPTO_SYMC_WORK_MODE_CBC;
    } else if (!strcmp(alg_name, "ctr(aes)")) {
        *mode = CRYPTO_SYMC_WORK_MODE_CTR;
    } else if (!strcmp(alg_name, "gcm(aes)")) {
        *mode = CRYPTO_SYMC_WORK_MODE_GCM;
    } else if (!strcmp(alg_name, "ccm(aes)")) {
        *mode = CRYPTO_SYMC_WORK_MODE_CCM;
    } else {
        *mode = CRYPTO_SYMC_WORK_MODE_INVALID;
        cprint("the %s mode is not support now!\r\n", alg_name);
        return -EFAULT;
    }
    return 0;
}

static int normal_aes_init(struct crypto_tfm *tfm)
{
    // init transform
    int ret;
    const mm_malloc_param mmz_param = {
        .size = CIPHER_BUFF_LENGTH,
        .kernel_only = TD_FALSE
    };

    const char *alg_name = crypto_tfm_alg_name(tfm);
    struct cipher_aes_ctx *ctx = crypto_tfm_ctx(tfm);
    ret = memset_s(ctx, sizeof(struct cipher_aes_ctx), 0, sizeof(struct cipher_aes_ctx));
    cryptodev_chk_return(ret != TD_SUCCESS, ret, "normal_aes_init failed\r\n");
    
    ctx->cipher_attr.symc_type = CRYPTO_SYMC_TYPE_NORMAL;
    ctx->cipher_attr.symc_alg = CRYPTO_SYMC_ALG_AES;
    ctx->cipher_attr.is_long_term = TD_TRUE;
    ctx->keyslot_type = KM_KEYSLOT_TYPE_MCIPHER;
    ctx->keyslot_handle = INVALID_HANDLE;
    ctx->symc_handle = INVALID_HANDLE;

    ret = alg_name_2_workmode(alg_name, &ctx->cipher_attr.work_mode);
    cryptodev_chk_return(ret != TD_SUCCESS, -EFAULT, "input alg name not support yet\r\n");

    ret = cmpi_mmz_malloc_cached(&mmz_param, &ctx->mem_phys, (td_void **)&ctx->mem_virt);
    cryptodev_chk_return(ret != TD_SUCCESS, -EFAULT, "cmpi_mmz_malloc_cached failed\r\n");
    cryptodev_chk_return(ctx->mem_virt == NULL, -EFAULT, "buff piont to null addr!\r\n");
    ctx->is_init = 1;

    return 0;
}

static void normal_aes_deinit(struct crypto_tfm *tfm)
{
    // destroy transform
    struct cipher_aes_ctx *ctx = crypto_tfm_ctx(tfm);

    if (ctx->is_init == 1) {
        (void)memset_s(ctx->mem_virt, CIPHER_BUFF_LENGTH, 0, CIPHER_BUFF_LENGTH);
        cmpi_mmz_free(ctx->mem_phys, ctx->mem_virt);
        (void)memset_s(ctx, sizeof(struct cipher_aes_ctx), 0, sizeof(struct cipher_aes_ctx));
        ctx->is_init = 0;
    }
}

static int normal_aes_set_key(struct crypto_tfm *tfm, const u8 *key,
    unsigned int keylen)
{
    struct cipher_aes_ctx *ctx = crypto_tfm_ctx(tfm);

    if (ctx->is_init == 0) {
        cprint("set_key failed. init first\r\n");
        return -EINVAL;
    }

    if (key == NULL || keylen > AES_MAX_KEY_SIZE) {
        cprint("key length is overflow\r\n");
        return -EINVAL;
    }

    ctx->key_len = keylen;
    cryptodev_chk_return(memcpy_s(ctx->key, sizeof(ctx->key), key, keylen) != EOK,
        -EFAULT, "key set failed\r\n");

    return 0;
}

static int normal_aes_encrypt(struct skcipher_request *req)
{
    struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
    struct crypto_tfm *cry_tfm = crypto_skcipher_tfm(tfm);
    const char *alg_name = crypto_tfm_alg_name(cry_tfm);
    cipher_param param = {
        .cipher_alg = CRYPTO_SYMC_ALG_AES,
        .flag = OT_DRV_ENCRYPT
    };

    cryptodev_chk_return(alg_name_2_workmode(alg_name, &param.work_mode) != TD_SUCCESS,
        -EFAULT, "alg not support!\r\n");

    if (inner_aes_crypt(req, &param) != 0) {
        return -EINVAL;
    }
    return 0;
}

static int normal_aes_decrypt(struct skcipher_request *req)
{
    struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
    struct crypto_tfm *cry_tfm = crypto_skcipher_tfm(tfm);
    const char *alg_name = crypto_tfm_alg_name(cry_tfm);
    cipher_param param = {
        .cipher_alg = CRYPTO_SYMC_ALG_AES,
        .flag = OT_DRV_DECRYPT
    };

    cryptodev_chk_return(alg_name_2_workmode(alg_name, &param.work_mode) != TD_SUCCESS,
        -EFAULT, "alg not support!\r\n");

    if (inner_aes_crypt(req, &param) != 0) {
        return -EINVAL;
    }
    return 0;
}

static int lock_aes_init(struct crypto_skcipher *tfm)
{
    int ret;
    enter_func();
    osal_mutex_lock(&aes_lock);
    ret = normal_aes_init(&tfm->base);
    osal_mutex_unlock(&aes_lock);
    exit_func();
    cryptodev_chk_return(ret != TD_SUCCESS, -EINVAL, "aes_init failed\r\n");
    return ret;
}

static void lock_aes_deinit(struct crypto_skcipher *tfm)
{
    enter_func();
    osal_mutex_lock(&aes_lock);
    normal_aes_deinit(&tfm->base);
    osal_mutex_unlock(&aes_lock);
    exit_func();
}

static int lock_aes_set_key(struct crypto_skcipher *tfm, const u8 *key,
    unsigned int keylen)
{
    int ret;
    enter_func();
    osal_mutex_lock(&aes_lock);
    ret = normal_aes_set_key(&tfm->base, key, keylen);
    osal_mutex_unlock(&aes_lock);
    exit_func();
    cryptodev_chk_return(ret != TD_SUCCESS, -EINVAL, "aes_set_key failed\r\n");
    return ret;
}

static int lock_aes_encrypt(struct skcipher_request *req)
{
    int ret;
    enter_func();
    osal_mutex_lock(&aes_lock);
    ret = normal_aes_encrypt(req);
    osal_mutex_unlock(&aes_lock);
    exit_func();
    cryptodev_chk_return(ret != TD_SUCCESS, -EINVAL, "aes_encrypt failed\r\n");
    return ret;
}

static int lock_aes_decrypt(struct skcipher_request *req)
{
    int ret;
    enter_func();
    osal_mutex_lock(&aes_lock);
    ret = normal_aes_decrypt(req);
    osal_mutex_unlock(&aes_lock);
    exit_func();
    cryptodev_chk_return(ret != TD_SUCCESS, -EINVAL, "aes_decrypt failed\r\n");
    return ret;
}

struct skcipher_alg aes_algs[2] = {
    {
        .base.cra_name = "cbc(aes)",
        .base.cra_driver_name = "ot_drv",
        .base.cra_priority = 2000,
        .base.cra_flags = CRYPTO_ALG_TYPE_SKCIPHER,
        .base.cra_blocksize = AES_BLOCK_SIZE,
        .base.cra_ctxsize = sizeof(struct cipher_aes_ctx),
        .base.cra_module = THIS_MODULE,
        .init = lock_aes_init,
        .exit = lock_aes_deinit,
        .setkey = lock_aes_set_key,
        .encrypt = lock_aes_encrypt,
        .decrypt = lock_aes_decrypt,
        .ivsize = AES_BLOCK_SIZE,
        .min_keysize = AES_MIN_KEY_SIZE,
        .max_keysize = AES_MAX_KEY_SIZE,
    },
    {
        .base.cra_name = "ctr(aes)",
        .base.cra_driver_name = "ot_drv",
        .base.cra_priority = 2000,
        .base.cra_flags = CRYPTO_ALG_TYPE_SKCIPHER,
        .base.cra_blocksize = 1,
        .base.cra_ctxsize = sizeof(struct cipher_aes_ctx),
        .base.cra_module = THIS_MODULE,
        .init = lock_aes_init,
        .exit = lock_aes_deinit,
        .setkey = lock_aes_set_key,
        .encrypt = lock_aes_encrypt,
        .decrypt = lock_aes_decrypt,
        .ivsize = AES_BLOCK_SIZE,
        .min_keysize = AES_MIN_KEY_SIZE,
        .max_keysize = AES_MAX_KEY_SIZE,
    }
};

static int aes_aead_gcm_setauthsize(struct crypto_aead *tfm, unsigned int authsize)
{
    struct cipher_aes_ctx *ctx = crypto_aead_ctx(tfm);
    // gcm tag len should below 16
    if (authsize > 16) {
        cprint("set authsize not successful\r\n");
        return -EINVAL;
    }
    ctx->authsize = authsize;
    return 0;
}

static int aes_aead_ccm_setauthsize(struct crypto_aead *tfm, unsigned int authsize)
{
    struct cipher_aes_ctx *ctx = crypto_aead_ctx(tfm);
    // ccm tag len Can only be 4 6 8 10 12 14 16
    switch (authsize) {
        case 4:
        case 6:
        case 8:
        case 10:
        case 12:
        case 14:
        case 16:
            break;
        default:
            cprint("set authsize not successful\r\n");
            return -EINVAL;
    }

    ctx->authsize = authsize;
    return 0;
}

static int aead_aes_init(struct crypto_tfm *tfm)
{
    // init transform
    int ret;
    const mm_malloc_param cipher_param = {
        .size = CIPHER_BUFF_LENGTH,
        .kernel_only = TD_FALSE
    };
    const mm_malloc_param aad_param = {
        .size = MAX_AAD_LEN,
        .kernel_only = TD_FALSE
    };

    const char *alg_name = crypto_tfm_alg_name(tfm);
    struct cipher_aes_ctx *ctx = crypto_tfm_ctx(tfm);
    ret = memset_s(ctx, sizeof(struct cipher_aes_ctx), 0, sizeof(struct cipher_aes_ctx));
    cryptodev_chk_return(ret != TD_SUCCESS, ret, "normal_aes_init failed\r\n");
    
    ctx->cipher_attr.symc_type = CRYPTO_SYMC_TYPE_NORMAL;
    ctx->cipher_attr.symc_alg = CRYPTO_SYMC_ALG_AES;
    ctx->cipher_attr.is_long_term = TD_TRUE;
    ctx->keyslot_type = KM_KEYSLOT_TYPE_MCIPHER;
    ctx->keyslot_handle = INVALID_HANDLE;
    ctx->symc_handle = INVALID_HANDLE;

    ret = alg_name_2_workmode(alg_name, &ctx->cipher_attr.work_mode);
    cryptodev_chk_return(ret != TD_SUCCESS, -EFAULT, "input alg name not support yet\r\n");

    ret = cmpi_mmz_malloc_cached(&cipher_param, &ctx->mem_phys, (td_void **)&ctx->mem_virt);
    cryptodev_chk_return(ret != TD_SUCCESS, -EFAULT, "cmpi_mmz_malloc_cached failed\r\n");
    cryptodev_chk_return(ctx->mem_virt == NULL, -EFAULT, "buff piont to null addr!\r\n");

    ret = cmpi_mmz_malloc_nocache(&aad_param, &ctx->aad_mem_phys, (td_void **)&ctx->aad_mem_virt);
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, buff_free, ret, "cmpi_mmz_malloc_cached failed\r\n");
    cryptodev_chk_goto_with_ret(ctx->aad_mem_virt == TD_NULL, buff_free, ret, "buff point to nullptr\r\n");
    ctx->is_init = 1;
    return 0;
buff_free:
    cmpi_mmz_free(ctx->mem_phys, ctx->mem_virt);
    return -EFAULT;
}

static void aead_aes_deinit(struct crypto_tfm *tfm)
{
    // destroy transform
    struct cipher_aes_ctx *ctx = crypto_tfm_ctx(tfm);

    if (ctx->is_init == 1) {
        (void)memset_s(ctx->mem_virt, CIPHER_BUFF_LENGTH, 0, CIPHER_BUFF_LENGTH);
        (void)memset_s(ctx->aad_mem_virt, MAX_AAD_LEN, 0, MAX_AAD_LEN);
        cmpi_mmz_free(ctx->mem_phys, ctx->mem_virt);
        cmpi_mmz_free(ctx->aad_mem_phys, ctx->aad_mem_virt);
        (void)memset_s(ctx, sizeof(struct cipher_aes_ctx), 0, sizeof(struct cipher_aes_ctx));
        ctx->is_init = 0;
    }
}

static int lock_aes_aead_init(struct crypto_aead *tfm)
{
    int ret;
    enter_func();
    osal_mutex_lock(&aes_lock);
    ret = aead_aes_init(&tfm->base);
    osal_mutex_unlock(&aes_lock);
    exit_func();
    cryptodev_chk_return(ret != TD_SUCCESS, -EINVAL, "lock_aes_aead_init failed\r\n");
    return ret;
}

static void lock_aes_aead_deinit(struct crypto_aead *tfm)
{
    enter_func();
    osal_mutex_lock(&aes_lock);
    aead_aes_deinit(&tfm->base);
    osal_mutex_unlock(&aes_lock);
    exit_func();
}

static int lock_aes_aead_set_key(struct crypto_aead *tfm, const u8 *key, unsigned int keylen)
{
    int ret;
    enter_func();
    osal_mutex_lock(&aes_lock);
    ret = normal_aes_set_key(&tfm->base, key, keylen);
    osal_mutex_unlock(&aes_lock);
    exit_func();
    cryptodev_chk_return(ret != TD_SUCCESS, -EINVAL, "lock_aes_aead_set_key failed\r\n");
    return ret;
}

static int aes_aead_encrypt(struct aead_request *req)
{
    int ret;
    const char *alg_name = crypto_aead_alg(crypto_aead_reqtfm(req))->base.cra_name;
    cipher_param param = {
        .cipher_alg = CRYPTO_SYMC_ALG_AES,
        .flag = OT_DRV_ENCRYPT
    };

    ret = alg_name_2_workmode(alg_name, &param.work_mode);
    cryptodev_chk_return(ret != TD_SUCCESS, ret, "aes_aead_encrypt failed\r\n");

    ret = inner_aes_aead_crypt(req, &param);
    cryptodev_chk_return(ret != TD_SUCCESS, -EFAULT, "aes_aead_encrypt failed\r\n");
    return 0;
}

static int aes_aead_decrypt(struct aead_request *req)
{
    int ret;
    const char *alg_name = crypto_aead_alg(crypto_aead_reqtfm(req))->base.cra_name;

    cipher_param param = {
        .cipher_alg = CRYPTO_SYMC_ALG_AES,
        .flag = OT_DRV_DECRYPT
    };

    ret = alg_name_2_workmode(alg_name, &param.work_mode);
    cryptodev_chk_return(ret != TD_SUCCESS, ret, "aes_aead_decrypt failed\r\n");

    ret = inner_aes_aead_crypt(req, &param);
    cryptodev_chk_return(ret != TD_SUCCESS, -EFAULT, "aes_aead_decrypt failed\r\n");
    return 0;
}

static int lock_aes_aead_encrypt(struct aead_request *req)
{
    int ret;
    enter_func();
    osal_mutex_lock(&aes_lock);
    ret = aes_aead_encrypt(req);
    osal_mutex_unlock(&aes_lock);
    exit_func();
    cryptodev_chk_return(ret != TD_SUCCESS, -EINVAL, "lock_aes_aead_encrypt failed\r\n");
    return ret;
}

static int lock_aes_aead_decrypt(struct aead_request *req)
{
    int ret;
    enter_func();
    osal_mutex_lock(&aes_lock);
    ret = aes_aead_decrypt(req);
    osal_mutex_unlock(&aes_lock);
    exit_func();
    cryptodev_chk_return(ret != TD_SUCCESS, -EINVAL, "lock_aes_aead_decrypt failed\r\n");
    return ret;
}

struct aead_alg aes_aead_algs[2] = {
    {
        .base.cra_name = "gcm(aes)",
        .base.cra_driver_name = "ot_drv",
        .base.cra_priority = 2000,
        .base.cra_flags = CRYPTO_ALG_TYPE_AEAD,
        .base.cra_blocksize = 1,
        .base.cra_ctxsize = sizeof(struct cipher_aes_ctx),
        .base.cra_module = THIS_MODULE,

        .ivsize = GCM_IV_SIZE,
        .maxauthsize = AES_BLOCK_SIZE,
        .init = lock_aes_aead_init,
        .exit = lock_aes_aead_deinit,
        .encrypt = lock_aes_aead_encrypt,
        .decrypt = lock_aes_aead_decrypt,
        .setkey = lock_aes_aead_set_key,
        .setauthsize = aes_aead_gcm_setauthsize,
    },
    {
        .base.cra_name = "ccm(aes)",
        .base.cra_driver_name = "ot_drv",
        .base.cra_priority = 2000,
        .base.cra_flags = CRYPTO_ALG_TYPE_AEAD,
        .base.cra_blocksize = 1,
        .base.cra_ctxsize = sizeof(struct cipher_aes_ctx),
        .base.cra_module = THIS_MODULE,

        .ivsize = AES_BLOCK_SIZE,
        .maxauthsize = AES_BLOCK_SIZE,
        .init = lock_aes_aead_init,
        .exit = lock_aes_aead_deinit,
        .encrypt = lock_aes_aead_encrypt,
        .decrypt = lock_aes_aead_decrypt,
        .setkey = lock_aes_aead_set_key,
        .setauthsize = aes_aead_ccm_setauthsize,
    }
};