/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : drivers for sys cipher.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#include "sys_cipher.h"

/* ****************************** API Code **************************** */
hi_s32 sys_symc_create(hi_u32 *id)
{
    hi_s32 ret;
    symc_create_t create = {0};

    hi_log_func_enter();

    ret = memset_s(&create, sizeof(create), 0, sizeof(create));
    if (ret != 0) {
        hi_log_print_func_err(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    ret = crypto_ioctl(CRYPTO_CMD_SYMC_CREATEHANDLE, &create);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_ioctl, ret);
        return ret;
    }

    *id = create.id;

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 sys_symc_destroy(hi_u32 id)
{
    symc_destroy_t destroy = {0};
    hi_s32 ret;

    hi_log_func_enter();

    ret = memset_s(&destroy, sizeof(destroy), 0, sizeof(destroy));
    if (ret != 0) {
        hi_log_print_func_err(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    destroy.id = id;

    ret = crypto_ioctl(CRYPTO_CMD_SYMC_DESTROYHANDLE, &destroy);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_ioctl, ret);
        return ret;
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

static hi_s32 sys_symc_get_key(symc_cfg_t *cfg, sys_symc_ctrl *ctrl)
{
    hi_s32 ret;

    if (ctrl->iv != HI_NULL) {
        ret = memcpy_s(cfg->iv, sizeof(cfg->iv), ctrl->iv, ctrl->iv_len);
        if (ret != 0) {
            hi_log_print_func_err(memcpy_s, ret);
            return HI_ERR_CIPHER_MEMCPY_S_FAILED;
        }
    }

    if (ctrl->fkey != HI_NULL) {
        ret = memcpy_s(cfg->fkey, sizeof(cfg->fkey), ctrl->fkey, ctrl->klen);
        if (ret != 0) {
            hi_log_print_func_err(memcpy_s, ret);
            return HI_ERR_CIPHER_MEMCPY_S_FAILED;
        }

        if (ctrl->alg == HI_CIPHER_ALG_SM1) {
            /* SM1 key: EK + AK + SK, fkey: EK + AK. */
            ret = memcpy_s(cfg->fkey + SYMC_SM1_SK_SIZE, SYMC_SM1_SK_SIZE,
                ctrl->fkey + SYMC_SM1_SK_SIZE, SYMC_SM1_SK_SIZE);
            if (ret != 0) {
                hi_log_print_func_err(memcpy_s, ret);
                return HI_ERR_CIPHER_MEMCPY_S_FAILED;
            }
        }
    }

    if (ctrl->skey != HI_NULL) {
        ret = memcpy_s(cfg->skey, sizeof(cfg->skey), ctrl->skey, ctrl->klen);
        if (ret != 0) {
            hi_log_print_func_err(memcpy_s, ret);
            return HI_ERR_CIPHER_MEMCPY_S_FAILED;
        }
    }

    return HI_SUCCESS;
}

static hi_s32 sys_symc_set_cfg(symc_cfg_t *cfg, sys_symc_ctrl *ctrl)
{
    hi_s32 ret;

    cfg->id = ctrl->id;
    cfg->hard_key = ctrl->hard_key;
    cfg->alg = ctrl->alg;
    cfg->mode = ctrl->work_mode;
    cfg->klen = ctrl->key_len;
    cfg->iv_usage = ctrl->iv_usage;
    cfg->sm1_round_num = ctrl->sm1_round_num;
    cfg->aad = ctrl->aad;
    cfg->alen = ctrl->aad_len;
    cfg->tlen = ctrl->tag_len;
    cfg->ivlen = ctrl->iv_len;
    cfg->width = ctrl->bit_width;

    hi_dbg_print_u32(ctrl->iv_len);
    hi_dbg_print_h32(ctrl->key_len);

    ret = sys_symc_get_key(cfg, ctrl);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(sys_symc_get_key, ret);
        crypto_zeroize(cfg, sizeof(symc_cfg_t));
        return ret;
    }

    return HI_SUCCESS;
}

hi_s32 sys_symc_cfg(sys_symc_ctrl *ctrl)
{
    symc_cfg_t cfg;
    hi_s32 ret;

    hi_log_func_enter();

    ret = memset_s(&cfg, sizeof(cfg), 0, sizeof(cfg));
    if (ret != 0) {
        hi_log_print_func_err(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    ret = sys_symc_set_cfg(&cfg, ctrl);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(memcpy_s, ret);
        return ret;
    }

    ret = crypto_ioctl(CRYPTO_CMD_SYMC_CONFIGHANDLE, &cfg);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_ioctl, ret);
        crypto_zeroize(&cfg, sizeof(cfg));
        return ret;
    }

    crypto_zeroize(&cfg, sizeof(cfg));
    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 sys_symc_get_cfg(hi_u32 id, hi_cipher_ctrl *ctrl)
{
    symc_get_cfg_t get_cfg;
    hi_s32 ret;

    hi_log_func_enter();

    ret = memset_s(&get_cfg, sizeof(get_cfg), 0, sizeof(get_cfg));
    if (ret != 0) {
        hi_log_print_func_err(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    get_cfg.id = id;

    ret = crypto_ioctl(CRYPTO_CMD_SYMC_GET_CONFIG, &get_cfg);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_ioctl, ret);
        return ret;
    }

    ret = memcpy_s(ctrl, sizeof(hi_cipher_ctrl), &get_cfg.ctrl, sizeof(hi_cipher_ctrl));
    if (ret != 0) {
        hi_log_print_func_err(memcpy_s, ret);
        crypto_zeroize(&get_cfg, sizeof(get_cfg));
        return HI_ERR_CIPHER_MEMCPY_S_FAILED;
    }

    crypto_zeroize(&get_cfg, sizeof(get_cfg));
    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 sys_symc_crypto(hi_u32 id, const compat_addr input, const compat_addr output, hi_u32 length, hi_u32 operation)
{
    symc_encrypt_t crypt;
    hi_s32 ret;

    hi_log_func_enter();

    ret = memset_s(&crypt, sizeof(crypt), 0, sizeof(crypt));
    if (ret != 0) {
        hi_log_print_func_err(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    crypt.id = id;
    crypt.in = input;
    crypt.out = output;
    crypt.len = length;
    crypt.operation = operation;

    ret = crypto_ioctl(CRYPTO_CMD_SYMC_ENCRYPT, &crypt);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_ioctl, ret);
        return ret;
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 sys_symc_crypto_multi(hi_u32 id, const void *pkg, hi_u32 pkg_num, hi_u32 operation)
{
    symc_encrypt_multi_t encrypt_multi;
    hi_s32 ret;

    hi_log_func_enter();

    ret = memset_s(&encrypt_multi, sizeof(encrypt_multi), 0, sizeof(encrypt_multi));
    if (ret != 0) {
        hi_log_print_func_err(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    encrypt_multi.id = id;
    addr_via_const(encrypt_multi.pack) = pkg;
    encrypt_multi.pack_num = pkg_num;
    encrypt_multi.operation = operation;

    ret = crypto_ioctl(CRYPTO_CMD_SYMC_ENCRYPTMULTI, &encrypt_multi);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_ioctl, ret);
        return ret;
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 sys_klad_encrypt_key(hi_u32 keysel, hi_u32 target, const hi_u8 clear[AES_BLOCK_SIZE],
    hi_u8 encrypt[AES_BLOCK_SIZE], hi_u32 key_len)
{
    klad_key_t klad;
    hi_s32 ret;

    hi_log_func_enter();
    hi_log_chk_param_return(key_len != AES_BLOCK_SIZE);

    ret = memset_s(&klad, sizeof(klad), 0, sizeof(klad));
    if (ret != 0) {
        hi_log_print_func_err(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    klad.keysel = keysel;
    klad.target = target;
    ret = memcpy_s(klad.clear, sizeof(klad.clear), clear, key_len);
    if (ret != 0) {
        hi_log_print_func_err(memcpy_s, ret);
        return HI_ERR_CIPHER_MEMCPY_S_FAILED;
    }

    ret = crypto_ioctl(CRYPTO_CMD_KLAD_KEY, &klad);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_ioctl, ret);
        return ret;
    }

    ret = memcpy_s(encrypt, key_len, klad.encrypt, sizeof(klad.encrypt));
    if (ret != 0) {
        hi_log_print_func_err(memcpy_s, ret);
        crypto_zeroize(&klad, sizeof(klad));
        return HI_ERR_CIPHER_MEMCPY_S_FAILED;
    }

    crypto_zeroize(&klad, sizeof(klad));
    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 sys_aead_get_tag(hi_u32 id, hi_u8 *tag, hi_u32 *taglen)
{
    hi_s32 ret;
    aead_tag_t aead_tag;

    hi_log_func_enter();

    ret = memset_s(&aead_tag, sizeof(aead_tag), 0, sizeof(aead_tag));
    if (ret != 0) {
        hi_log_print_func_err(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    aead_tag.id = id;
    aead_tag.taglen = *taglen;

    ret = crypto_ioctl(CRYPTO_CMD_SYMC_GETTAG, &aead_tag);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_ioctl, ret);
        return ret;
    }

    ret = memcpy_s(tag, AEAD_TAG_SIZE, aead_tag.tag, aead_tag.taglen);
    if (ret != 0) {
        hi_log_print_func_err(memcpy_s, ret);
        crypto_zeroize(&aead_tag, sizeof(aead_tag));
        return HI_ERR_CIPHER_MEMCPY_S_FAILED;
    }

    *taglen = aead_tag.taglen;

    crypto_zeroize(&aead_tag, sizeof(aead_tag));
    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 sys_hash_start(hi_u32 *id, hi_cipher_hash_type type, const hi_u8 *key, hi_u32 keylen)
{
    hi_s32 ret;
    hash_start_t start;

    hi_log_func_enter();

    ret = memset_s(&start, sizeof(start), 0, sizeof(start));
    if (ret != 0) {
        hi_log_print_func_err(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    start.id = 0;
    start.type = type;
    start.keylen = keylen;
    addr_via_const(start.key) = key;

    ret =  crypto_ioctl(CRYPTO_CMD_HASH_START, &start);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_ioctl, ret);
        return ret;
    }

    *id = start.id;

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 sys_hash_update(hi_u32 id, const hi_u8 *in, hi_u32 length, hash_chunk_src src)
{
    hi_s32 ret;
    hash_update_t update;

    hi_log_func_enter();

    ret = memset_s(&update, sizeof(update), 0, sizeof(update));
    if (ret != 0) {
        hi_log_print_func_err(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    update.id = id;
    addr_via_const(update.input) = in;
    update.length = length;
    update.src = src;

    ret = crypto_ioctl(CRYPTO_CMD_HASH_UPDATE, &update);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_ioctl, ret);
        return ret;
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 sys_hash_finish(hi_u32 id, hi_u8 *hash, hi_u32 *hashlen)
{
    hi_s32 ret;
    hash_finish_t finish;

    hi_log_func_enter();

    ret = memset_s(&finish, sizeof(finish), 0, sizeof(finish));
    if (ret != 0) {
        hi_log_print_func_err(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    finish.id = id;

    ret = crypto_ioctl(CRYPTO_CMD_HASH_FINISH, &finish);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_ioctl, ret);
        return ret;
    }

    ret = memcpy_s(hash, HASH_RESULT_MAX_SIZE, finish.hash, finish.hashlen);
    if (ret != 0) {
        hi_log_print_func_err(memcpy_s, ret);
        return HI_ERR_CIPHER_MEMCPY_S_FAILED;
    }

    *hashlen = finish.hashlen;

    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 sys_rsa_encrypt(const cryp_rsa_key *key, hi_cipher_rsa_encrypt_scheme scheme, hi_cipher_rsa_crypt *rsa_data)
{
    hi_s32 ret;
    rsa_info_t rsa_info;

    hi_log_func_enter();

    ret = memset_s(&rsa_info, sizeof(rsa_info_t), 0, sizeof(rsa_info_t));
    if (ret != 0) {
        hi_log_print_func_err(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    addr_via_const(rsa_info.n) = key->n;
    addr_via_const(rsa_info.d) = key->d;
    addr_via_const(rsa_info.p) = key->p;
    addr_via_const(rsa_info.q) = key->q;
    addr_via_const(rsa_info.dp) = key->dp;
    addr_via_const(rsa_info.dq) = key->dq;
    addr_via_const(rsa_info.qp) = key->qp;

    rsa_info.e = key->e;
    rsa_info.public = key->public;
    rsa_info.klen = key->klen;
    rsa_info.scheme = scheme;
    addr_via_const(rsa_info.in) = rsa_data->in;
    rsa_info.inlen = rsa_data->in_len;
    addr_via(rsa_info.out) = rsa_data->out;
    rsa_info.outlen = rsa_data->out_buf_len;
    rsa_info.ca_type = key->ca_type;

    ret = crypto_ioctl(CRYPTO_CMD_RSA_ENC, &rsa_info);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_ioctl, ret);
        crypto_zeroize(&rsa_info.e, sizeof(rsa_info.e));
        return ret;
    }

    *rsa_data->out_len = rsa_info.outlen;
    crypto_zeroize(&rsa_info.e, sizeof(rsa_info.e));
    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 sys_rsa_decrypt(const cryp_rsa_key *key, hi_cipher_rsa_encrypt_scheme scheme, hi_cipher_rsa_crypt *rsa_data)
{
    hi_s32 ret;
    rsa_info_t rsa_info;

    hi_log_func_enter();

    ret = memset_s(&rsa_info, sizeof(rsa_info), 0, sizeof(rsa_info));
    if (ret != 0) {
        hi_log_print_func_err(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    addr_via_const(rsa_info.n) = key->n;
    addr_via_const(rsa_info.d) = key->d;
    addr_via_const(rsa_info.p) = key->p;
    addr_via_const(rsa_info.q) = key->q;
    addr_via_const(rsa_info.dp) = key->dp;
    addr_via_const(rsa_info.dq) = key->dq;
    addr_via_const(rsa_info.qp) = key->qp;

    rsa_info.e = key->e;
    rsa_info.public = key->public;
    rsa_info.klen = key->klen;
    rsa_info.scheme = scheme;
    addr_via_const(rsa_info.in) = rsa_data->in;
    rsa_info.inlen = rsa_data->in_len;
    addr_via(rsa_info.out) = rsa_data->out;
    rsa_info.outlen = rsa_data->out_buf_len;
    rsa_info.ca_type = key->ca_type;

    ret = crypto_ioctl(CRYPTO_CMD_RSA_DEC, &rsa_info);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_ioctl, ret);
        crypto_zeroize(&rsa_info.e, sizeof(rsa_info.e));
        return ret;
    }

    *rsa_data->out_len = rsa_info.outlen;
    crypto_zeroize(&rsa_info.e, sizeof(rsa_info.e));
    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 sys_rsa_sign_hash(const sys_rsa_sign_verify *rsa_sign, hi_u8 *sign, hi_u32 *signlen)
{
    hi_s32 ret;
    rsa_info_t rsa_info;

    hi_log_func_enter();

    ret = memset_s(&rsa_info, sizeof(rsa_info), 0, sizeof(rsa_info));
    if (ret != 0) {
        hi_log_print_func_err(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    addr_via_const(rsa_info.n)  = rsa_sign->key.n;
    addr_via_const(rsa_info.d)  = rsa_sign->key.d;
    addr_via_const(rsa_info.p)  = rsa_sign->key.p;
    addr_via_const(rsa_info.q)  = rsa_sign->key.q;
    addr_via_const(rsa_info.dp) = rsa_sign->key.dp;
    addr_via_const(rsa_info.dq) = rsa_sign->key.dq;
    addr_via_const(rsa_info.qp) = rsa_sign->key.qp;

    rsa_info.e = rsa_sign->key.e;
    rsa_info.public = rsa_sign->key.public;
    rsa_info.klen = rsa_sign->key.klen;
    rsa_info.scheme = rsa_sign->scheme;
    addr_via_const(rsa_info.in) = rsa_sign->hash;
    rsa_info.inlen = rsa_sign->hash_len;
    addr_via(rsa_info.out) = sign;
    rsa_info.outlen = rsa_sign->key.klen;
    rsa_info.ca_type = rsa_sign->key.ca_type;

    ret = crypto_ioctl(CRYPTO_CMD_RSA_SIGN, &rsa_info);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_ioctl, ret);
        crypto_zeroize(&rsa_info.e, sizeof(rsa_info.e));
        return ret;
    }

    *signlen = rsa_info.outlen;
    crypto_zeroize(&rsa_info.e, sizeof(rsa_info.e));
    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 sys_rsa_verify_hash(const sys_rsa_sign_verify *rsa_verify, const hi_u8 *sign, hi_u32 signlen)
{
    hi_s32 ret;
    rsa_info_t rsa_info;

    hi_log_func_enter();

    ret = memset_s(&rsa_info, sizeof(rsa_info_t), 0, sizeof(rsa_info_t));
    if (ret != 0) {
        hi_log_print_func_err(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    addr_via_const(rsa_info.n) = rsa_verify->key.n;

    rsa_info.e = rsa_verify->key.e;
    rsa_info.public = rsa_verify->key.public;
    rsa_info.klen = rsa_verify->key.klen;
    rsa_info.scheme = rsa_verify->scheme;
    addr_via_const(rsa_info.in) = sign;
    rsa_info.inlen = signlen;
    addr_via_const(rsa_info.out) = rsa_verify->hash;
    rsa_info.outlen = rsa_verify->hash_len;

    ret = crypto_ioctl(CRYPTO_CMD_RSA_VERIFY, &rsa_info);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(crypto_ioctl, ret);
        crypto_zeroize(&rsa_info.e, sizeof(rsa_info.e));
        return ret;
    }

    crypto_zeroize(&rsa_info.e, sizeof(rsa_info.e));
    hi_log_func_exit();
    return HI_SUCCESS;
}

hi_s32 sys_trng_get_random(hi_u32 *randnum, hi_u32 timeout)
{
    hi_s32 ret;
    trng_t trng;

    hi_log_func_enter();

    ret = memset_s(&trng, sizeof(trng), 0, sizeof(trng));
    if (ret != 0) {
        hi_log_print_func_err(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    trng.timeout = timeout;

    ret = crypto_ioctl(CRYPTO_CMD_TRNG, &trng);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    *randnum = trng.randnum;
    hi_log_func_exit();
    return HI_SUCCESS;
}
