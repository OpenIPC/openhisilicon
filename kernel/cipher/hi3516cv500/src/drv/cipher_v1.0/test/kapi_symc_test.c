/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : cipher test cases for kapi symc.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#include "kapi_symc_test.h"

#define TEST_BLOCK_LEN          16
#define KEY_128BITS_LEN         16

typedef struct {
    hi_handle handle;
    hi_bool key_by_ca;
    hi_cipher_ca_type ca_type;
    hi_cipher_alg alg;
    hi_cipher_work_mode mode;
    hi_cipher_key_len key_len;
    hi_u8 *key;
    hi_u32 klen;
    hi_u8 *iv;
    hi_u32 ivlen;
} kapi_test_cipher_cfg;

typedef struct {
    hi_bool key_by_ca;
    hi_cipher_work_mode mode;
    hi_u8 *key;
    hi_u32 klen;
    hi_u8 *iv;
    hi_u32 ivlen;
    hi_u8 *in;
    hi_u8 *out;
    hi_u8 *expect_buf;
    hi_u32 buf_len;
} kapi_test_crypt_pack;


/* kapi_test end */
static hi_s32 set_cipher_config(kapi_test_cipher_cfg *set_cfg)
{
    hi_s32 ret;
    symc_cfg_t cfg;

    memset(&cfg, 0, sizeof(cfg));

    cfg.id = (hi_u32)set_cfg->handle;
    cfg.alg = set_cfg->alg;
    cfg.mode = set_cfg->mode;
    cfg.width = HI_CIPHER_BIT_WIDTH_128BIT;
    cfg.klen = set_cfg->key_len;
    cfg.hard_key = (set_cfg->ca_type & MAX_LOW_8BITS) << BITS_IN_BYTE;
    cfg.hard_key |= ((hi_u32)set_cfg->key_by_ca & MAX_LOW_8BITS);

    if (cfg.mode != HI_CIPHER_WORK_MODE_ECB) {
        cfg.iv_usage = 1;  /* must set for CBC , CFB mode */
        memcpy(cfg.iv, set_cfg->iv, set_cfg->ivlen);
    }

    memcpy(cfg.fkey, set_cfg->key, set_cfg->klen);

    addr_u64(cfg.aad) = 0x00;
    if ((cfg.alg == HI_CIPHER_ALG_3DES) || (cfg.alg == HI_CIPHER_ALG_DES)) {
        cfg.ivlen = DES_IV_SIZE;
    } else {
        cfg.ivlen = AES_IV_SIZE;
    }

    ret = kapi_symc_cfg(&cfg);
    if (ret != HI_SUCCESS) {
        pr_err("kapi_symc_cfg failed.\n");
        return ret;
    }

    return HI_SUCCESS;
}

static hi_s32 set_cipher_crypt(hi_handle handle, hi_phys_addr_t src_phys_addr,
                               hi_phys_addr_t dst_phys_addr, hi_u32 byte_len,
                               hi_u32 operation)
{
    hi_s32 ret;
    symc_encrypt_t crypt;

    memset(&crypt, 0, sizeof(symc_encrypt_t));
    crypt.id = handle;
    crypt.len = byte_len;
    addr_u64(crypt.in) = src_phys_addr;
    addr_u64(crypt.out) = dst_phys_addr;
    crypt.last = 0;
    crypt.operation = operation;

    ret = kapi_symc_crypto_via(&crypt, 0);
    if (ret != HI_SUCCESS) {
        pr_err("call kapi_symc_crypto_via failed.\n");
        return ret;
    }

    return HI_SUCCESS;
}

static hi_s32 cipher_aes_decrypt(kapi_test_crypt_pack *pcak)
{
    int ret;
    hi_handle test_chnid;
    kapi_test_cipher_cfg cfg;

    ret = kapi_symc_create(&test_chnid);
    if (ret != HI_SUCCESS) {
        hi_log_error("Error: CreateHandle failed!\n");
        return ret;
    }

    /* For decrypt */
    cfg.handle = test_chnid;
    cfg.key_by_ca = pcak->key_by_ca;
    cfg.ca_type = HI_CIPHER_KEY_SRC_KLAD_1;
    cfg.alg = HI_CIPHER_ALG_AES;
    cfg.mode = pcak->mode;
    cfg.key_len = HI_CIPHER_KEY_AES_128BIT;
    cfg.key = pcak->key;
    cfg.klen = pcak->klen;
    cfg.iv = pcak->iv;
    cfg.ivlen = pcak->ivlen;
    ret = set_cipher_config(&cfg);
    if (ret != 0) {
        hi_log_error("Set config info failed.\n");
        goto __CIPHER_EXIT__;
    }

    ret = set_cipher_crypt(test_chnid, (hi_phys_addr_t)pcak->in, (hi_phys_addr_t)pcak->out,
                           pcak->buf_len, SYMC_OPERATION_DECRYPT);
    if (ret != 0) {
        hi_log_error("[5]wrong data!\n");
        ret = -1;
        goto __CIPHER_EXIT__;
    }

    /* compare */
    if (memcmp(pcak->out, pcak->expect_buf, pcak->buf_len) != 0) {
        hi_log_error("Memcmp failed!\n");
        ret = HI_ERR_CIPHER_ILLEGAL_DATA;
        goto __CIPHER_EXIT__;
    }

__CIPHER_EXIT__:
    kapi_symc_destroy(test_chnid);

    return ret;
}

/* otp key is be written, then excute tihs test */
static hi_s32 kapi_test1(hi_void)
{
    hi_s32 ret;
    kapi_test_crypt_pack pack;
    hi_u8 aes_128_enc_key[TEST_BLOCK_LEN] = {0xc1, 0x1b, 0x54, 0x4a, 0x12, 0x9c, 0x08, 0xa5,
        0xcc, 0xd3, 0xeb, 0xec, 0x7a, 0x3b, 0x00, 0x2b};
    hi_u8 aes_128_cbc_IV[TEST_BLOCK_LEN] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    hi_u8 aes_128_src_buf[TEST_BLOCK_LEN] = {0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96,
        0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A};
    hi_u8 aes_128_dst_buf[TEST_BLOCK_LEN] = {0xb0, 0x1b, 0x77, 0x09, 0xe8, 0xdc, 0xf9, 0xef,
        0x37, 0x13, 0x0b, 0x13, 0xda, 0x11, 0xbf, 0x24};
    hi_u8 aes_128_src2_buf[TEST_BLOCK_LEN] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

    hi_log_error("klad test!\n");
    pack.key_by_ca  = HI_TRUE;
    pack.mode       = HI_CIPHER_WORK_MODE_CBC;
    pack.key        = aes_128_enc_key;
    pack.klen       = sizeof(aes_128_enc_key);
    pack.iv         = aes_128_cbc_IV;
    pack.ivlen      = sizeof(aes_128_cbc_IV);
    pack.in         = aes_128_dst_buf;
    pack.out        = aes_128_src2_buf;
    pack.expect_buf = aes_128_src_buf;
    pack.buf_len    = sizeof(aes_128_src_buf);
    ret = cipher_aes_decrypt(&pack);
    if (ret != HI_SUCCESS) {
        hi_log_error("klad failed!\n");
        return -1;
    }

    hi_log_error("klad success!\n");
    return HI_SUCCESS;
}

static hi_s32 kapi_test2(hi_void)
{
    hi_s32 ret;
    kapi_test_crypt_pack pack;
    hi_u8 aes_key[TEST_BLOCK_LEN] = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
        0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};
    hi_u8 aes_IV[TEST_BLOCK_LEN]  = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    hi_u8 aes_src[TEST_BLOCK_LEN] = {0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96,
        0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A};
    hi_u8 aes_dst[TEST_BLOCK_LEN] = {0x76, 0x49, 0xAB, 0xAC, 0x81, 0x19, 0xB2, 0x46,
        0xCE, 0xE9, 0x8E, 0x9B, 0x12, 0xE9, 0x19, 0x7D};
    hi_u8 aes_src2[TEST_BLOCK_LEN]  = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

    hi_log_error("cbc test!\n");

    pack.key_by_ca  = HI_FALSE;
    pack.mode       = HI_CIPHER_WORK_MODE_CBC;
    pack.key        = aes_key;
    pack.klen       = sizeof(aes_key);
    pack.iv         = aes_IV;
    pack.ivlen      = sizeof(aes_IV);
    pack.in         = aes_dst;
    pack.out        = aes_src2;
    pack.expect_buf = aes_src;
    pack.buf_len    = sizeof(aes_src);
    ret = cipher_aes_decrypt(&pack);
    if (ret != HI_SUCCESS) {
        hi_log_error("cbc failed!\n");
        return -1;
    }

    hi_log_error("cbc success!\n");
    return HI_SUCCESS;
}

static hi_void kapi_test(hi_void)
{
    hi_s32 ret;

    ret = kapi_test1();
    if (ret != HI_SUCCESS)
        hi_log_error("kapi_test1 failed!\n");

    ret = kapi_test2();
    if (ret != HI_SUCCESS)
        hi_log_error("kapi_test2 failed!\n");
}

hi_void kapi_test_main(hi_void)
{
    kapi_test();
}
