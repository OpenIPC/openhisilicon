/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "ot_reg.h"
#include <linux/module.h>
#include <linux/types.h>
#include <linux/err.h>
#include <linux/scatterlist.h>
#include <crypto/internal/hash.h>
#include <crypto/internal/skcipher.h>

#define TEE_DISABLE 0x42
#define TEE_STATUS_OFFSET 0x12

static int aes_init(struct cipher_aes_ctx *ctx)
{
    int ret = TD_SUCCESS;

    ret = kapi_cipher_symc_init();
    cryptodev_chk_return(ret != TD_SUCCESS, ret, "kapi_cipher_symc_init failed\r\n");

    ret = kapi_cipher_symc_create(&ctx->symc_handle, &ctx->cipher_attr);
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, cipher_deinit, ret, "kapi_cipher_symc_create failed\r\n");

    ret = kapi_keyslot_create(&ctx->keyslot_handle, ctx->keyslot_type);
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, cipher_destroy, ret, "kapi_keyslot_create failed\r\n");

    ret = kapi_cipher_symc_attach(ctx->symc_handle, ctx->keyslot_handle);
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, keyslot_destroy, ret, "kapi_cipher_symc_attach failed\r\n");

    return 0;
keyslot_destroy:
    kapi_keyslot_destroy(ctx->keyslot_handle);
cipher_destroy:
    kapi_cipher_symc_destroy(ctx->symc_handle);
cipher_deinit:
    kapi_cipher_symc_deinit();

    return ret;
}

static void aes_deinit(struct cipher_aes_ctx *ctx)
{
    if (ctx->keyslot_handle != INVALID_HANDLE) {
        kapi_keyslot_destroy(ctx->keyslot_handle);
    }

    if (ctx->symc_handle != INVALID_HANDLE) {
        kapi_cipher_symc_destroy(ctx->symc_handle);
    }
    kapi_cipher_symc_deinit();
}

static td_s32 priv_check_tee_is_open(td_bool *is_open)
{
    td_s32 ret = TD_FAILURE;
    td_u8 value = 0;
    *is_open = TD_TRUE;

    ret = kapi_otp_read_byte(TEE_STATUS_OFFSET, &value);
    cryptodev_chk_return(ret != TD_SUCCESS, ret, "kapi_otp_read_byte failed\r\n");

    if (value == TEE_DISABLE) {
        *is_open = TD_FALSE;
    }

    return TD_SUCCESS;
}

int inner_cipher_set_key(td_handle keyslot_handle, const u8 *key, unsigned int keylen)
{
    int ret;
    td_handle klad;
    td_bool tee_is_open = TD_FALSE;
    km_klad_attr klad_attr = {
        .key_cfg = {
            .engine = KM_CRYPTO_ALG_AES,
            .decrypt_support = TD_TRUE,
            .encrypt_support = TD_TRUE
        },
        .key_sec_cfg = {
            .key_sec = KM_KLAD_SEC_DISABLE,
            .master_only_enable = TD_TRUE,
            .dest_buf_sec_support = TD_TRUE,
            .dest_buf_non_sec_support = TD_FALSE,
            .src_buf_sec_support = TD_TRUE,
            .src_buf_non_sec_support = TD_FALSE
        }
    };

    km_klad_clear_key clr_key = {
        .key = (td_u8 *)key,
        .key_size = keylen,
    };

    ret = priv_check_tee_is_open(&tee_is_open);
    cryptodev_chk_return(ret != TD_SUCCESS, ret, "priv_check_tee_is_open failed\r\n");

    if (tee_is_open == TD_TRUE) {
        klad_attr.key_sec_cfg.key_sec = KM_KLAD_SEC_ENABLE;
        klad_attr.key_sec_cfg.master_only_enable = TD_FALSE;
        klad_attr.key_sec_cfg.dest_buf_sec_support = TD_FALSE;
        klad_attr.key_sec_cfg.src_buf_sec_support = TD_FALSE;
        klad_attr.key_sec_cfg.dest_buf_non_sec_support = TD_TRUE;
        klad_attr.key_sec_cfg.src_buf_non_sec_support = TD_TRUE;
    }
    
    ret = kapi_klad_create(&klad);
    cryptodev_chk_return(ret != TD_SUCCESS, ret, "kapi_klad_create failed\r\n");

    ret = kapi_klad_set_attr(klad, &klad_attr);
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, klad_destroy, ret, "kapi_klad_set_attr failed\r\n");

    ret = kapi_klad_attach(klad, KM_KLAD_DEST_TYPE_MCIPHER, keyslot_handle);
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, klad_destroy, ret, "kapi_klad_attach failed\r\n");

    ret = kapi_klad_set_clear_key(klad, &clr_key);
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, klad_detach, ret, "kapi_klad_set_clear_key failed\r\n");
klad_detach:
    kapi_klad_detach(klad, KM_KLAD_DEST_TYPE_MCIPHER, keyslot_handle);
klad_destroy:
    kapi_klad_destroy(klad);

    return ret;
}

static int crypto_set_config(struct skcipher_request *req, struct cipher_aes_ctx *ctx,
    crypto_symc_work_mode work_mode, crypto_symc_alg cipher_alg)
{
    int ret;
    crypto_symc_ctrl_t cipher_ctrl = {0};
    cipher_ctrl.symc_alg = cipher_alg;
    cipher_ctrl.work_mode = work_mode;

    cipher_ctrl.symc_bit_width = CRYPTO_SYMC_BIT_WIDTH_128BIT;
    cipher_ctrl.iv_change_flag = CRYPTO_SYMC_IV_DO_NOT_CHANGE;
    cipher_ctrl.iv_length = AES_BLOCK_SIZE;

    switch (ctx->key_len) {
        case AES_128_KEY_SIZE:
            cipher_ctrl.symc_key_length = CRYPTO_SYMC_KEY_128BIT;
            break;
        case AES_192_KEY_SIZE:
            cipher_ctrl.symc_key_length = CRYPTO_SYMC_KEY_192BIT;
            break;
        case AES_256_KEY_SIZE:
            cipher_ctrl.symc_key_length = CRYPTO_SYMC_KEY_256BIT;
            break;
        default:
            cprint("the key length is %d, not support yet\r\n", ctx->key_len);
            return -1;
    }

    ret = memcpy_s(cipher_ctrl.iv, sizeof(cipher_ctrl.iv), req->iv, AES_BLOCK_SIZE);
    cryptodev_chk_return(ret != EOK, ret, "set iv failed\r\n");

    ret = kapi_cipher_symc_set_config(ctx->symc_handle, &cipher_ctrl);
    cryptodev_chk_return(ret != TD_SUCCESS, ret, "kapi_cipher_symc_set_config failed\r\n");

    return ret;
}

static int aead_ivformat_trans(struct aead_request *req, crypto_symc_work_mode work_mode,
    crypto_symc_ctrl_t *cipher_ctrl, td_bool direct)
{
    int ret;
    int offset = (work_mode == CRYPTO_SYMC_WORK_MODE_CCM) ? 1 : 0;

    if (work_mode == CRYPTO_SYMC_WORK_MODE_CCM && (req->iv[0] < 1 || req->iv[0] > 7)) { // iv[0] limited in [1, 7]
        cprint("CCM mode iv size range from [7, 13]\r\n");
        return -EINVAL;
    }

    cipher_ctrl->iv_length = offset ? 14 - req->iv[0] : crypto_aead_ivsize(crypto_aead_reqtfm(req));  // ccm base iv 14

    if (direct == TD_TRUE) {
        ret = memcpy_s(cipher_ctrl->iv, sizeof(cipher_ctrl->iv), req->iv + offset, cipher_ctrl->iv_length);
        cryptodev_chk_return(ret != TD_SUCCESS, ret, "ex iv memcpy_s failed\r\n");
    } else {
        ret = memcpy_s(req->iv + offset, cipher_ctrl->iv_length, cipher_ctrl->iv, cipher_ctrl->iv_length);
        cryptodev_chk_return(ret != TD_SUCCESS, ret, "ex iv memcpy_s failed\r\n");
    }

    return ret;
}

static int crypto_set_config_ex(struct aead_request *req, struct cipher_aes_ctx *ctx,
    crypto_symc_work_mode work_mode, crypto_symc_alg cipher_alg)
{
    int ret;
    crypto_symc_ctrl_t cipher_ctrl = {0};
    crypto_symc_config_aes_ccm_gcm ctrl_aes_ex = {
        .aad_len = req->assoclen,
        .tag_len = ctx->authsize,
        .data_len = ctx->cryptlen,
        .aad_buf.virt_addr = ctx->aad_mem_virt
    };

    cipher_ctrl.symc_alg = cipher_alg;
    cipher_ctrl.work_mode = work_mode;
    cipher_ctrl.symc_bit_width = CRYPTO_SYMC_BIT_WIDTH_128BIT;
    cipher_ctrl.iv_change_flag = (work_mode == CRYPTO_SYMC_WORK_MODE_CCM) ?
        CRYPTO_SYMC_CCM_IV_CHANGE_START : CRYPTO_SYMC_GCM_IV_CHANGE_START;
    cipher_ctrl.param = (td_void *)&ctrl_aes_ex;

    switch (ctx->key_len) {
        case AES_128_KEY_SIZE:
            cipher_ctrl.symc_key_length = CRYPTO_SYMC_KEY_128BIT;
            break;
        case AES_192_KEY_SIZE:
            cipher_ctrl.symc_key_length = CRYPTO_SYMC_KEY_192BIT;
            break;
        case AES_256_KEY_SIZE:
            cipher_ctrl.symc_key_length = CRYPTO_SYMC_KEY_256BIT;
            break;
        default:
            cprint("the key length is %d, not support yet\r\n", ctx->key_len);
            return -1;
    }

    ret = aead_ivformat_trans(req, work_mode, &cipher_ctrl, TD_TRUE);
    cryptodev_chk_return(ret != TD_SUCCESS, ret, "inner_ivformat_transto_drv failed\r\n");

    // copy src aad data to drv
    sg_copy_buffer(req->src, sg_nents(req->src), ctx->aad_mem_virt, req->assoclen, 0, true);

    ret = kapi_cipher_symc_set_config(ctx->symc_handle, &cipher_ctrl);
    cryptodev_chk_return(ret != TD_SUCCESS, ret, "ot_drv_cipher_set_cfg failed\r\n");
    return ret;
}

static int cryptodev_cipher_crypto(struct cipher_aes_ctx *ctx, unsigned int len, cipher_param *param)
{
    int ret = TD_SUCCESS;
    crypto_symc_work_mode mode = param->work_mode;
    crypt_flag flag = param->flag;
    unsigned int remain_len = len % AES_BLOCK_SIZE;
    crypto_buf_attr src_buf = {
        .phys_addr = ctx->mem_phys,
        .virt_addr = (void *)ctx->mem_virt
    };
    crypto_buf_attr dst_buf = {
        .phys_addr = ctx->mem_phys,
        .virt_addr = (void *)ctx->mem_virt
    };

    if (mode != CRYPTO_SYMC_WORK_MODE_CTR && mode != CRYPTO_SYMC_WORK_MODE_GCM &&
            mode != CRYPTO_SYMC_WORK_MODE_CCM  && remain_len) {
        cprint("The value to be calculated must be aligned with 16 bytes.");
        return -EINVAL;
    }

    if (len != 0 || mode == CRYPTO_SYMC_WORK_MODE_GCM || mode == CRYPTO_SYMC_WORK_MODE_CCM) {
        if (flag == OT_DRV_ENCRYPT) {
            ret = kapi_cipher_symc_encrypt(ctx->symc_handle, &src_buf, &dst_buf, len);
        } else {
            ret = kapi_cipher_symc_decrypt(ctx->symc_handle, &src_buf, &dst_buf, len);
        }
        cryptodev_chk_return(ret != TD_SUCCESS, ret, "ot_drv_cipher_encrypt failed\r\n");
    }

    return ret;
}

int inner_aes_crypt(struct skcipher_request *req, cipher_param *param)
{
    int ret;
    unsigned int len = 0;
    unsigned int total = 0;
    crypto_symc_ctrl_t cipher_ctrl = {0};
    struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
    struct cipher_aes_ctx *ctx = crypto_skcipher_ctx(tfm);

    struct scatterlist *src = req->src;
    struct scatterlist *dst = req->dst;

    if (ctx->is_init == 0) {
        cprint("aes_crypt failed. init first\r\n");
        return -EINVAL;
    }

    ret = aes_init(ctx);
    cryptodev_chk_return(ret != TD_SUCCESS, ret, "inner_aes_init failed\r\n");

    ret = inner_cipher_set_key(ctx->keyslot_handle, ctx->key, ctx->key_len);
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, aes_deinit, ret, "inner_cipher_set_key failed\r\n");

    ret = crypto_set_config(req, ctx, param->work_mode, param->cipher_alg);
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, aes_deinit, ret, "crypto_set_config failed\r\n");

    while (total < req->cryptlen) {
        len = min(req->cryptlen - total, (unsigned int)CIPHER_BUFF_LENGTH);
        sg_copy_buffer(src, sg_nents(src), ctx->mem_virt, len, total, true);
        crypto_flush_dcache(ctx->mem_virt, ctx->mem_phys, len);

        ret = cryptodev_cipher_crypto(ctx, len, param);
        cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, aes_deinit, ret, "ot_cipher_crypt failed\r\n");

        sg_copy_buffer(dst, sg_nents(dst), ctx->mem_virt, len, total, false);
        total += len;
    }

    ret = kapi_cipher_symc_get_config(ctx->symc_handle, &cipher_ctrl);
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, aes_deinit, ret, "kapi_cipher_symc_get_config failed\r\n");

    ret = memcpy_s(req->iv, AES_BLOCK_SIZE, cipher_ctrl.iv, sizeof(cipher_ctrl.iv));
    cryptodev_chk_goto_with_ret(ret != EOK, aes_deinit, ret, "memcpy_s iv failed\r\n");
aes_deinit:
    (void)memset_s(cipher_ctrl.iv, sizeof(cipher_ctrl.iv), 0, sizeof(cipher_ctrl.iv));
    (void)memset_s(ctx->mem_virt, CIPHER_BUFF_LENGTH, 0, CIPHER_BUFF_LENGTH);
    aes_deinit(ctx);

    return ret;
}

int inner_aes_aead_crypt(struct aead_request *req, cipher_param *param)
{
    int ret;
    unsigned int len = 0;
    unsigned int total = 0;
    u8 tag[MAX_TAG_LEN] = {0};
    u8 verify_tag[MAX_TAG_LEN] = {0};
    unsigned int nbytes = req->cryptlen;
    struct cipher_aes_ctx *ctx = crypto_aead_ctx(crypto_aead_reqtfm(req));
    unsigned int tag_len = ctx->authsize;

    if (ctx->is_init == 0) {
        cprint("crypto need init first!\r\n");
        return -1;
    }

    ctx->cryptlen = nbytes;
    if (param->flag == OT_DRV_DECRYPT) {
        if (nbytes < tag_len) {
            cprint("crypt data's length is invalid\r\n");
            return -1;
        }
        ctx->cryptlen = (nbytes -= tag_len);
        // store tag to verify decryt result
        sg_copy_buffer(req->src, sg_nents(req->src), verify_tag, tag_len, req->assoclen + nbytes, true);
    }

    ret = aes_init(ctx);
    cryptodev_chk_return(ret != TD_SUCCESS, ret, "inner_aes_init failed\r\n");

    ret = inner_cipher_set_key(ctx->keyslot_handle, ctx->key, ctx->key_len);
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, aes_deinit, ret, "inner_cipher_set_key failed\r\n");

    ret = crypto_set_config_ex(req, ctx, param->work_mode, param->cipher_alg);
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, aes_deinit, ret, "crypto_set_config_ex failed\r\n");

    do {
        len = min(nbytes - total, (unsigned int)CIPHER_BUFF_LENGTH);
        sg_copy_buffer(req->src, sg_nents(req->src), ctx->mem_virt, len, total + req->assoclen, true);
        crypto_flush_dcache(ctx->mem_virt, ctx->mem_phys, len);

        ret = cryptodev_cipher_crypto(ctx, len, param);
        cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, aes_deinit, ret, "ot_cipher_crypt failed\r\n");

        sg_copy_buffer(req->dst, sg_nents(req->dst), ctx->mem_virt, len, total + req->assoclen, false);
        total += len;
    } while (total < nbytes);

    ret = kapi_cipher_symc_get_tag(ctx->symc_handle, tag, sizeof(tag));
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, aes_deinit, ret, "ot_drv_cipher_get_tag failed\r\n");

    if (param->flag == OT_DRV_ENCRYPT) {
        sg_copy_buffer(req->dst, sg_nents(req->dst), tag, tag_len, total + req->assoclen, false);
    } else {
        ret = memcmp(tag, verify_tag, tag_len);
        cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, aes_deinit, -1, "decrypt tag not right\r\n");
    }
aes_deinit:
    (void)memset_s(ctx->mem_virt, CIPHER_BUFF_LENGTH, 0, CIPHER_BUFF_LENGTH);
    aes_deinit(ctx);

    return ret;
}

static int __init hardware_crypto_init(void)
{
    int ret = osal_mutex_init(&hash_channel_lock);
    cryptodev_chk_return(ret != TD_SUCCESS, -EFAULT, "hash_channel_lock init failed\r\n");

    ret = osal_mutex_init(&aes_lock);
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, hash_lock_destroy, -EFAULT, "aes_lock init failed\r\n");

    ret = crypto_register_skciphers(aes_algs, ARRAY_SIZE(aes_algs));
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, aes_lock_destroy, -EFAULT, "crypto_register_alg failed\r\n");

    ret = crypto_register_shashes(shash_alges, ARRAY_SIZE(shash_alges));
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, unreg_skcipher, -EFAULT, "crypto_register_shashes failed\r\n");

    ret = crypto_register_aeads(aes_aead_algs, ARRAY_SIZE(aes_aead_algs));
    cryptodev_chk_goto_with_ret(ret != TD_SUCCESS, unreg_hash, ret, "crypto_register_shashes failed\r\n");

    printk(KERN_INFO "hardware algorithm registered\n");
    return TD_SUCCESS;
unreg_hash:
    crypto_unregister_shashes(shash_alges, ARRAY_SIZE(shash_alges));
unreg_skcipher:
    crypto_unregister_skciphers(aes_algs, ARRAY_SIZE(aes_algs));
aes_lock_destroy:
    osal_mutex_destroy(&aes_lock);
hash_lock_destroy:
    osal_mutex_destroy(&hash_channel_lock);
    return -EINVAL;
}

static void __exit hardware_crypto_exit(void)
{
    crypto_unregister_aeads(aes_aead_algs, ARRAY_SIZE(aes_aead_algs));
    crypto_unregister_skciphers(aes_algs, ARRAY_SIZE(aes_algs));
    crypto_unregister_shashes(shash_alges, ARRAY_SIZE(shash_alges));
    osal_mutex_destroy(&hash_channel_lock);
    osal_mutex_destroy(&aes_lock);
    printk(KERN_INFO "hardware algorithm unregistered\n");
}

int hard_crypto_debug = 0;
module_param(hard_crypto_debug, int, 0644);
MODULE_PARM_DESC(hard_crypto_debug, "0: normal, other: debug");

module_init(hardware_crypto_init);
module_exit(hardware_crypto_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("component of hardware crypto algorithm");