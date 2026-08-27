/*****************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : sys_cipher.c
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/

#include "sys_cipher.h"

/******************************* API Code *****************************/
/** \addtogroup      symc */
/** @{*/  /** <!-- [mpi]*/

hi_s32 sys_symc_create(hi_u32 *id)
{
    hi_s32 ret = HI_FAILURE;
    symc_create_t create = {0};

    HI_LOG_FUNC_ENTER();

    ret = memset_s(&create, sizeof(create), 0, sizeof(create));
    if (ret != 0) {
        HI_LOG_PRINT_FUNC_ERR(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    ret = CRYPTO_IOCTL(CRYPTO_CMD_SYMC_CREATEHANDLE, &create);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(CRYPTO_IOCTL, ret);
        return ret;
    }

    *id = create.id;

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 sys_symc_destroy(hi_u32 id)
{
    symc_destroy_t destroy = {0};
    hi_s32 ret = HI_FAILURE;

    HI_LOG_FUNC_ENTER();

    ret = memset_s(&destroy, sizeof(destroy), 0, sizeof(destroy));
    if (ret != 0) {
        HI_LOG_PRINT_FUNC_ERR(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    destroy.id = id;

    ret = CRYPTO_IOCTL(CRYPTO_CMD_SYMC_DESTROYHANDLE, &destroy);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(CRYPTO_IOCTL, ret);
        return ret;
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 sys_symc_config(hi_u32 id,
                    hi_u32 hard_key,
                    hi_cipher_alg alg,
                    hi_cipher_work_mode work_mode,
                    hi_cipher_bit_width bit_width,
                    hi_cipher_key_length key_len,
                    hi_cipher_sm1_round sm1_round_num,
                    hi_u8 *fkey, hi_u8 *skey, hi_u32 keylen,
                    hi_u8 *iv, hi_u32 ivlen, hi_u32 iv_usage,
                    compat_addr aad, hi_u32 alen, hi_u32 tlen)
{
    symc_config_t config;
    hi_s32 ret = HI_FAILURE;

    HI_LOG_FUNC_ENTER();

    ret = memset_s(&config, sizeof(config), 0, sizeof(config));
    if (ret != 0) {
        HI_LOG_PRINT_FUNC_ERR(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    config.id = id;
    config.hard_key = hard_key;
    config.alg = alg;
    config.mode = work_mode;
    config.klen = key_len;
    config.iv_usage = iv_usage;
    config.sm1_round_num = sm1_round_num;
    config.aad = aad;
    config.alen = alen;
    config.tlen = tlen;
    config.ivlen = ivlen;
    config.width = bit_width;

    HI_DBG_PRINT_U32(ivlen);
    HI_DBG_PRINT_H32(key_len);

    if (fkey != HI_NULL) {
        ret = memcpy_s(config.fkey, sizeof(config.fkey), fkey, keylen);
        if (ret != 0) {
            HI_LOG_PRINT_FUNC_ERR(memcpy_s, ret);
            return HI_ERR_CIPHER_MEMCPY_S_FAILED;
        }

        if (alg == HI_CIPHER_ALG_SM1) {
            /* SM1 key:EK+AK+SK,fkey:EK+AK. */
            ret = memcpy_s(config.fkey + SYMC_SM1_SK_SIZE, SYMC_SM1_SK_SIZE,
                           fkey + SYMC_SM1_SK_SIZE, SYMC_SM1_SK_SIZE);
            if (ret != 0) {
                HI_LOG_PRINT_FUNC_ERR(memcpy_s, ret);
                crypto_zeroize(&config, sizeof(config));
                return HI_ERR_CIPHER_MEMCPY_S_FAILED;
            }
        }
    }

    if (iv != HI_NULL) {
        ret = memcpy_s(config.iv, sizeof(config.iv), iv, ivlen);
        if (ret != 0) {
            HI_LOG_PRINT_FUNC_ERR(memcpy_s, ret);
            crypto_zeroize(&config, sizeof(config));
            return HI_ERR_CIPHER_MEMCPY_S_FAILED;
        }
    }

    if (skey != HI_NULL) {
        ret = memcpy_s(config.skey, sizeof(config.skey), skey, keylen);
        if (ret != 0) {
            HI_LOG_PRINT_FUNC_ERR(memcpy_s, ret);
            crypto_zeroize(&config, sizeof(config));
            return HI_ERR_CIPHER_MEMCPY_S_FAILED;
        }
    }

    ret = CRYPTO_IOCTL(CRYPTO_CMD_SYMC_CONFIGHANDLE, &config);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(CRYPTO_IOCTL, ret);
        crypto_zeroize(&config, sizeof(config));
        return ret;
    }

    crypto_zeroize(&config, sizeof(config));
    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 sys_symc_get_config(hi_u32 id, hi_cipher_ctrl *ctrl)
{
    symc_get_config_t get_config;
    hi_s32 ret = HI_FAILURE;

    HI_LOG_FUNC_ENTER();

    ret = memset_s(&get_config, sizeof(get_config), 0, sizeof(get_config));
    if (ret != 0) {
        HI_LOG_PRINT_FUNC_ERR(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    get_config.id = id;

    ret = CRYPTO_IOCTL(CRYPTO_CMD_SYMC_GET_CONFIG, &get_config);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(CRYPTO_IOCTL, ret);
        return ret;
    }

    ret = memcpy_s(ctrl, sizeof(hi_cipher_ctrl), &get_config.ctrl, sizeof(hi_cipher_ctrl));
    if (ret != 0) {
        HI_LOG_PRINT_FUNC_ERR(memcpy_s, ret);
        crypto_zeroize(&get_config, sizeof(get_config));
        return HI_ERR_CIPHER_MEMCPY_S_FAILED;
    }

    crypto_zeroize(&get_config, sizeof(get_config));
    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 sys_symc_crypto(hi_u32 id, const compat_addr input,
                    const compat_addr output, hi_u32 length,
                    hi_u32 operation)
{
    symc_encrypt_t encrypt;
    hi_s32 ret = HI_FAILURE;

    HI_LOG_FUNC_ENTER();

    ret = memset_s(&encrypt, sizeof(encrypt), 0, sizeof(encrypt));
    if (ret != 0) {
        HI_LOG_PRINT_FUNC_ERR(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    encrypt.id = id;
    encrypt.input = input;
    encrypt.output = output;
    encrypt.length = length;
    encrypt.operation = operation;

    ret = CRYPTO_IOCTL(CRYPTO_CMD_SYMC_ENCRYPT, &encrypt);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(CRYPTO_IOCTL, ret);
        return ret;
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 sys_symc_crypto_multi(hi_u32 id, const void *pkg, hi_u32 pkg_num, hi_u32 operation)
{
    symc_encrypt_multi_t encrypt_multi;
    hi_s32 ret = HI_FAILURE;

    HI_LOG_FUNC_ENTER();

    ret = memset_s(&encrypt_multi, sizeof(encrypt_multi), 0, sizeof(encrypt_multi));
    if (ret != 0) {
        HI_LOG_PRINT_FUNC_ERR(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    encrypt_multi.id = id;
    ADDR_VIA_CONST(encrypt_multi.pkg) = pkg;
    encrypt_multi.pkg_num = pkg_num;
    encrypt_multi.operation = operation;

    ret = CRYPTO_IOCTL(CRYPTO_CMD_SYMC_ENCRYPTMULTI, &encrypt_multi);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(CRYPTO_IOCTL, ret);
        return ret;
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 sys_klad_encrypt_key(hi_u32 keysel, hi_u32 target, const hi_u8 clear[AES_BLOCK_SIZE], hi_u8 encrypt[AES_BLOCK_SIZE])
{
    klad_key_t klad;
    hi_s32 ret = HI_FAILURE;

    HI_LOG_FUNC_ENTER();

    ret = memset_s(&klad, sizeof(klad), 0, sizeof(klad));
    if (ret != 0) {
        HI_LOG_PRINT_FUNC_ERR(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    klad.keysel = keysel;
    klad.target = target;
    ret = memcpy_s(klad.clear, AES_BLOCK_SIZE, clear, AES_BLOCK_SIZE);
    if (ret != 0) {
        HI_LOG_PRINT_FUNC_ERR(memcpy_s, ret);
        return HI_ERR_CIPHER_MEMCPY_S_FAILED;
    }

    ret = CRYPTO_IOCTL(CRYPTO_CMD_KLAD_KEY, &klad);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(CRYPTO_IOCTL, ret);
        return ret;
    }

    ret = memcpy_s(encrypt, AES_BLOCK_SIZE, klad.encrypt, AES_BLOCK_SIZE);
    if (ret != 0) {
        HI_LOG_PRINT_FUNC_ERR(memcpy_s, ret);
        crypto_zeroize(&klad, sizeof(klad));
        return HI_ERR_CIPHER_MEMCPY_S_FAILED;
    }

    crypto_zeroize(&klad, sizeof(klad));
    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 sys_aead_get_tag(hi_u32 id, hi_u8 *tag, hi_u32 *taglen)
{
    hi_s32 ret = HI_FAILURE;
    aead_tag_t aead_tag;

    HI_LOG_FUNC_ENTER();

    ret = memset_s(&aead_tag, sizeof(aead_tag), 0, sizeof(aead_tag));
    if (ret != 0) {
        HI_LOG_PRINT_FUNC_ERR(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    aead_tag.id = id;
    aead_tag.taglen = *taglen;

    ret = CRYPTO_IOCTL(CRYPTO_CMD_SYMC_GETTAG, &aead_tag);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(CRYPTO_IOCTL, ret);
        return ret;
    }

    ret = memcpy_s(tag, AEAD_TAG_SIZE, aead_tag.tag, aead_tag.taglen);
    if (ret != 0) {
        HI_LOG_PRINT_FUNC_ERR(memcpy_s, ret);
        crypto_zeroize(&aead_tag, sizeof(aead_tag));
        return HI_ERR_CIPHER_MEMCPY_S_FAILED;
    }

    *taglen = aead_tag.taglen;

    crypto_zeroize(&aead_tag, sizeof(aead_tag));
    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 sys_hash_start(hi_u32 *id, hi_cipher_hash_type type,
                   const hi_u8 *key, hi_u32 keylen)
{
    hi_s32 ret = HI_FAILURE;
    hash_start_t start;

    HI_LOG_FUNC_ENTER();

    ret = memset_s(&start, sizeof(start), 0, sizeof(start));
    if (ret != 0) {
        HI_LOG_PRINT_FUNC_ERR(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    start.id = 0;
    start.type = type;
    start.keylen = keylen;
    ADDR_VIA_CONST(start.key) = key;

    ret =  CRYPTO_IOCTL(CRYPTO_CMD_HASH_START, &start);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(CRYPTO_IOCTL, ret);
        return ret;
    }

    *id = start.id;

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 sys_hash_update(hi_u32 id, const hi_u8 *input, hi_u32 length, hash_chunk_src src)
{
    hi_s32 ret = HI_FAILURE;
    hash_update_t update;

    HI_LOG_FUNC_ENTER();

    ret = memset_s(&update, sizeof(update), 0, sizeof(update));
    if (ret != 0) {
        HI_LOG_PRINT_FUNC_ERR(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    update.id = id;
    ADDR_VIA_CONST(update.input) = input;
    update.length = length;
    update.src = src;

    ret = CRYPTO_IOCTL(CRYPTO_CMD_HASH_UPDATE, &update);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(CRYPTO_IOCTL, ret);
        return ret;
    }

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 sys_hash_finish(hi_u32 id, hi_u8 *hash, hi_u32 *hashlen)
{
    hi_s32 ret = HI_FAILURE;
    hash_finish_t finish;

    HI_LOG_FUNC_ENTER();

    ret = memset_s(&finish, sizeof(finish), 0, sizeof(finish));
    if (ret != 0) {
        HI_LOG_PRINT_FUNC_ERR(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    finish.id = id;

    ret = CRYPTO_IOCTL(CRYPTO_CMD_HASH_FINISH, &finish);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(CRYPTO_IOCTL, ret);
        return ret;
    }

    ret = memcpy_s(hash, HASH_RESULT_MAX_SIZE, finish.hash, finish.hashlen);
    if (ret != 0) {
        HI_LOG_PRINT_FUNC_ERR(memcpy_s, ret);
        return HI_ERR_CIPHER_MEMCPY_S_FAILED;
    }

    *hashlen = finish.hashlen;

    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 sys_rsa_encrypt(const cryp_rsa_key *key,
                    hi_cipher_rsa_enc_scheme scheme,
                    const hi_u8 *in, hi_u32 inlen,
                    hi_u8 *out, hi_u32 *outlen)
{
    hi_s32 ret = HI_FAILURE;
    rsa_info_t rsa_info;

    HI_LOG_FUNC_ENTER();

    ret = memset_s(&rsa_info, sizeof(rsa_info_t), 0, sizeof(rsa_info_t));
    if (ret != 0) {
        HI_LOG_PRINT_FUNC_ERR(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    ADDR_VIA_CONST(rsa_info.n) = key->n;
    ADDR_VIA_CONST(rsa_info.d) = key->d;
    ADDR_VIA_CONST(rsa_info.p) = key->p;
    ADDR_VIA_CONST(rsa_info.q) = key->q;
    ADDR_VIA_CONST(rsa_info.dp) = key->dp;
    ADDR_VIA_CONST(rsa_info.dq) = key->dq;
    ADDR_VIA_CONST(rsa_info.qp) = key->qp;

    rsa_info.e = key->e;
    rsa_info.public = key->public;
    rsa_info.klen = key->klen;
    rsa_info.scheme = scheme;
    ADDR_VIA_CONST(rsa_info.in) = in;
    rsa_info.inlen = inlen;
    ADDR_VIA(rsa_info.out) = out;
    rsa_info.outlen = key->klen;
    rsa_info.ca_type = key->ca_type;

    ret = CRYPTO_IOCTL(CRYPTO_CMD_RSA_ENC, &rsa_info);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(CRYPTO_IOCTL, ret);
        crypto_zeroize(&rsa_info.e, sizeof(rsa_info.e));
        return ret;
    }

    *outlen = rsa_info.outlen;

    crypto_zeroize(&rsa_info.e, sizeof(rsa_info.e));
    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 sys_rsa_decrypt(const cryp_rsa_key *key,
                    hi_cipher_rsa_enc_scheme scheme,
                    const hi_u8 *in, hi_u32 inlen,
                    hi_u8 *out, hi_u32 *outlen)
{
    hi_s32 ret = HI_FAILURE;
    rsa_info_t rsa_info;

    HI_LOG_FUNC_ENTER();

    ret = memset_s(&rsa_info, sizeof(rsa_info), 0, sizeof(rsa_info));
    if (ret != 0) {
        HI_LOG_PRINT_FUNC_ERR(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    ADDR_VIA_CONST(rsa_info.n) = key->n;
    ADDR_VIA_CONST(rsa_info.d) = key->d;
    ADDR_VIA_CONST(rsa_info.p) = key->p;
    ADDR_VIA_CONST(rsa_info.q) = key->q;
    ADDR_VIA_CONST(rsa_info.dp) = key->dp;
    ADDR_VIA_CONST(rsa_info.dq) = key->dq;
    ADDR_VIA_CONST(rsa_info.qp) = key->qp;

    rsa_info.e = key->e;
    rsa_info.public = key->public;
    rsa_info.klen = key->klen;
    rsa_info.scheme = scheme;
    ADDR_VIA_CONST(rsa_info.in) = in;
    rsa_info.inlen = inlen;
    ADDR_VIA(rsa_info.out) = out;
    rsa_info.outlen = key->klen;
    rsa_info.ca_type = key->ca_type;

    ret = CRYPTO_IOCTL(CRYPTO_CMD_RSA_DEC, &rsa_info);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(CRYPTO_IOCTL, ret);
        crypto_zeroize(&rsa_info.e, sizeof(rsa_info.e));
        return ret;
    }

    *outlen = rsa_info.outlen;

    crypto_zeroize(&rsa_info.e, sizeof(rsa_info.e));
    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 sys_rsa_sign_hash(const cryp_rsa_key *key,
                      hi_cipher_rsa_sign_scheme scheme,
                      const hi_u8 *hash, hi_u32 hlen,
                      hi_u8 *sign, hi_u32 *signlen)
{
    hi_s32 ret = HI_FAILURE;
    rsa_info_t rsa_info;

    HI_LOG_FUNC_ENTER();

    ret = memset_s(&rsa_info, sizeof(rsa_info), 0, sizeof(rsa_info));
    if (ret != 0) {
        HI_LOG_PRINT_FUNC_ERR(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    ADDR_VIA_CONST(rsa_info.n) = key->n;
    ADDR_VIA_CONST(rsa_info.d) = key->d;
    ADDR_VIA_CONST(rsa_info.p) = key->p;
    ADDR_VIA_CONST(rsa_info.q) = key->q;
    ADDR_VIA_CONST(rsa_info.dp) = key->dp;
    ADDR_VIA_CONST(rsa_info.dq) = key->dq;
    ADDR_VIA_CONST(rsa_info.qp) = key->qp;

    rsa_info.e = key->e;
    rsa_info.public = key->public;
    rsa_info.klen = key->klen;
    rsa_info.scheme = scheme;
    ADDR_VIA_CONST(rsa_info.in) = hash;
    rsa_info.inlen = hlen;
    ADDR_VIA(rsa_info.out) = sign;
    rsa_info.outlen = key->klen;
    rsa_info.ca_type = key->ca_type;

    ret = CRYPTO_IOCTL(CRYPTO_CMD_RSA_SIGN, &rsa_info);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(CRYPTO_IOCTL, ret);
        crypto_zeroize(&rsa_info.e, sizeof(rsa_info.e));
        return ret;
    }

    *signlen = rsa_info.outlen;

    crypto_zeroize(&rsa_info.e, sizeof(rsa_info.e));
    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 sys_rsa_verify_hash(const cryp_rsa_key *key,
                        hi_cipher_rsa_sign_scheme scheme,
                        const hi_u8 *hash, hi_u32 hlen,
                        const hi_u8 *sign, hi_u32 signlen)
{
    hi_s32 ret = HI_FAILURE;
    rsa_info_t rsa_info;

    HI_LOG_FUNC_ENTER();

    ret = memset_s(&rsa_info, sizeof(rsa_info_t), 0, sizeof(rsa_info_t));
    if (ret != 0) {
        HI_LOG_PRINT_FUNC_ERR(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    ADDR_VIA_CONST(rsa_info.n) = key->n;

    rsa_info.e = key->e;
    rsa_info.public = key->public;
    rsa_info.klen = key->klen;
    rsa_info.scheme = scheme;
    ADDR_VIA_CONST(rsa_info.in) = sign;
    rsa_info.inlen = signlen;
    ADDR_VIA_CONST(rsa_info.out) = hash;
    rsa_info.outlen = hlen;

    ret = CRYPTO_IOCTL(CRYPTO_CMD_RSA_VERIFY, &rsa_info);
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(CRYPTO_IOCTL, ret);
        crypto_zeroize(&rsa_info.e, sizeof(rsa_info.e));
        return ret;
    }

    crypto_zeroize(&rsa_info.e, sizeof(rsa_info.e));
    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

hi_s32 sys_trng_get_random(hi_u32 *randnum, hi_u32 timeout)
{
    hi_s32 ret = HI_FAILURE;
    trng_t trng;

    HI_LOG_FUNC_ENTER();

    ret = memset_s(&trng, sizeof(trng), 0, sizeof(trng));
    if (ret != 0) {
        HI_LOG_PRINT_FUNC_ERR(memset_s, ret);
        return HI_ERR_CIPHER_MEMSET_S_FAILED;
    }

    trng.timeout = timeout;

    ret = CRYPTO_IOCTL(CRYPTO_CMD_TRNG, &trng);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    *randnum = trng.randnum;
    HI_LOG_FUNC_EXIT();
    return HI_SUCCESS;
}

/** @}*/  /** <!-- ==== API Code end ====*/
