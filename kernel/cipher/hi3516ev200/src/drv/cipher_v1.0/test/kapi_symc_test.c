/*****************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : kapi_symc_test.c
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include "drv_osal_lib.h"

/* kapi_test end*/
static hi_s32 set_cipher_config(hi_handle handle,
                                hi_bool key_by_ca,
                                hi_cipher_ca_type ca_type,
                                hi_cipher_alg alg,
                                hi_cipher_work_mode mode,
                                hi_cipher_key_length keyLen,
                                const hi_u8 key_buf[16],
                                const hi_u8 iv_buf[16])
{
    hi_u32 ivlen = AES_IV_SIZE;
    hi_s32 ret = HI_SUCCESS;
    hi_u32 hard_key = 0;
    hi_cipher_ctrl cipher_ctrl;
    compat_addr aad;

    memset(&cipher_ctrl, 0, sizeof(hi_cipher_ctrl));
    cipher_ctrl.alg = alg;
    cipher_ctrl.work_mode = mode;
    cipher_ctrl.bit_width = HI_CIPHER_BIT_WIDTH_128BIT;
    cipher_ctrl.key_len = keyLen;
    cipher_ctrl.key_by_ca = key_by_ca;
    cipher_ctrl.ca_type = ca_type;
    if (cipher_ctrl.work_mode != HI_CIPHER_WORK_MODE_ECB) {
        cipher_ctrl.change_flags.bit1_iv = 1;  //must set for CBC , CFB mode
        memcpy(cipher_ctrl.iv, iv_buf, 16);
    }

    memcpy(cipher_ctrl.key, key_buf, 16);

    if (HI_TRUE == cipher_ctrl.key_by_ca) {
        if (HI_CIPHER_KEY_SRC_BUTT <= cipher_ctrl.ca_type) {
            pr_err("Invalid ca_type with key_by_ca is HI_TRUE.\n");
            return HI_ERR_CIPHER_INVALID_PARA;

        }
        hard_key  = (cipher_ctrl.ca_type & 0xFF) << BITS_IN_BYTE;
        hard_key |= 0x01;
    }

    ADDR_U64(aad) = 0x00;

    if ((cipher_ctrl.alg == HI_CIPHER_ALG_3DES)
        || (cipher_ctrl.alg == HI_CIPHER_ALG_DES)) {
        ivlen = DES_IV_SIZE;
    }

    ret = kapi_symc_config(handle, hard_key,
                           cipher_ctrl.alg, cipher_ctrl.work_mode,
                           cipher_ctrl.bit_width, cipher_ctrl.key_len,
                           0, (hi_u8 *)cipher_ctrl.key, HI_NULL,
                           (hi_u8 *)cipher_ctrl.iv, ivlen,
                           cipher_ctrl.change_flags.bit1_iv,
                           aad, 0, 0);
    if (HI_SUCCESS != ret) {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static hi_s32 set_cipher_crypt(hi_handle handle, hi_size_t src_phy_addr,
                               hi_size_t dest_phy_addr, hi_u32 byte_length,
                               hi_u32 operation)
{
    compat_addr input;
    compat_addr output;
    hi_s32 ret = HI_FAILURE;

    ADDR_U64(input) = src_phy_addr;
    ADDR_U64(output) = dest_phy_addr;

    ret = kapi_symc_crypto_via(handle, input, output, byte_length, operation, 0, 0);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    return HI_SUCCESS;
}

static hi_s32 cipher_aes_decrypt(hi_bool key_by_ca,
                                 hi_cipher_work_mode mode,
                                 const unsigned char *key,
                                 const unsigned char *iv,
                                 const unsigned char *in_buf,
                                 const unsigned char *out_buf,
                                 const unsigned char *expect_buf,
                                 const unsigned int buf_len)
{
    int ret = 0;
    hi_handle test_chnid;

    ret = kapi_symc_create(&test_chnid);
    if (0 != ret) {
        HI_LOG_ERROR("Error: CreateHandle failed!\n");
        return HI_FAILURE;
    }

    /* For decrypt */

    ret = set_cipher_config(test_chnid,
                            key_by_ca,
                            HI_CIPHER_KEY_SRC_KLAD_1,
                            HI_CIPHER_ALG_AES,
                            mode,
                            HI_CIPHER_KEY_AES_128BIT,
                            key,
                            iv);
    if (0 != ret) {
        HI_LOG_ERROR("Set config info failed.\n");
        goto __CIPHER_EXIT__;
    }

    ret = set_cipher_crypt(test_chnid, (hi_size_t)in_buf, (hi_size_t)out_buf,
                           buf_len, SYMC_OPERATION_DECRYPT);
    if (0 != ret) {
        HI_LOG_ERROR("[5]wrong data!\n");
        ret = -1;
        goto __CIPHER_EXIT__;
    }

    /* compare */
    if ( 0 != memcmp(out_buf, expect_buf, 16) ) {
        HI_LOG_ERROR("Memcmp failed!\n");
        ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

__CIPHER_EXIT__:
    kapi_symc_destroy(test_chnid);

    return ret;
}

/* otp key is be written, then excute tihs test */
static hi_s32 kapi_test1(void)
{
    hi_s32 ret = HI_FAILURE;
    hi_u8 aes_128_enc_key[16] = {0xc1, 0x1b, 0x54, 0x4a, 0x12, 0x9c, 0x08, 0xa5,
                                0xcc, 0xd3, 0xeb, 0xec, 0x7a, 0x3b, 0x00, 0x2b};
    hi_u8 aes_128_cbc_IV[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                               0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    hi_u8 aes_128_src_buf[16] = {0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96,
                                0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A};
    hi_u8 aes_128_dst_buf[16] = {0xb0, 0x1b, 0x77, 0x09, 0xe8, 0xdc, 0xf9, 0xef,
                                0x37, 0x13, 0x0b, 0x13, 0xda, 0x11, 0xbf, 0x24};
    hi_u8 aes_128_src2_buf[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

    HI_LOG_ERROR("klad test!\n");
    ret = cipher_aes_decrypt(HI_TRUE,
                             HI_CIPHER_WORK_MODE_CBC,
                             aes_128_enc_key,
                             aes_128_cbc_IV,
                             aes_128_dst_buf,
                             aes_128_src2_buf,
                             aes_128_src_buf,
                             16);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("klad failed!\n");
        return -1;
    }

    HI_LOG_ERROR("klad success!\n");
    return HI_SUCCESS;
}

static hi_s32 kapi_test2(void)
{
    hi_s32 ret = HI_FAILURE;
    hi_u8 aes_key[16] = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
        0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};
    hi_u8 aes_IV[16]  = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    hi_u8 aes_src[16] = {0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96,
        0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A};
    hi_u8 aes_dst[16] = {0x76, 0x49, 0xAB, 0xAC, 0x81, 0x19, 0xB2, 0x46,
        0xCE, 0xE9, 0x8E, 0x9B, 0x12, 0xE9, 0x19, 0x7D};
    hi_u8 aes_src2[16]  = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

    HI_LOG_ERROR("cbc test!\n");
    ret = cipher_aes_decrypt(
              HI_FALSE,
              HI_CIPHER_WORK_MODE_CBC,
              aes_key, aes_IV,
              aes_dst, aes_src2,
              aes_src,
              16);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("cbc failed!\n");
        return -1;
    }

    HI_LOG_ERROR("cbc success!\n");
    return HI_SUCCESS;
}

void kapi_test(void)
{
    kapi_test1();
    kapi_test2();
}

/* kapi_test end */

/** @}*/  /** <!-- ==== Structure Definition end ====*/
