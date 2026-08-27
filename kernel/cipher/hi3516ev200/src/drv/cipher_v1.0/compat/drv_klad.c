/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : spacc_intf.c
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       :
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include "hi_types.h"
#include "drv_osal_lib.h"
#include "hal_efuse.h"
#include "hal_otp.h"
#include "drv_cipher_kapi.h"

#define KLAD_REG_BASE_ADDR                      cipher_klad_base
#define KLAD_REG_KLAD_CTRL                      (KLAD_REG_BASE_ADDR + 0x00)
#define KLAD_REG_DAT_IN                         (KLAD_REG_BASE_ADDR + 0x10)
#define KLAD_REG_ENC_OUT                        (KLAD_REG_BASE_ADDR + 0x20)

#define CIPHER_WAIT_IDEL_TIMES         1000

static hi_u8 *cipher_klad_base = HI_NULL;
extern hi_u8 *efuse_otp_reg_base;

hi_s32 hal_cipher_klad_config(hi_u32 chn_id, hi_u32 opt_id, hi_cipher_klad_target target, hi_bool is_decrypt)
{
    hi_s32 ret = HI_FAILURE;
    hi_u32 ctrl;

    /* Load efuse or OTP key to KLAD */
    ret = hal_efuse_otp_load_cipher_key(chn_id, opt_id);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    ctrl  = chn_id << 16;
    ctrl |= ((hi_u32)target) << 2;     /* cipher klad */
    ctrl |= ((hi_u32)is_decrypt) << 1; /* decrypt     */
    ctrl |= 0x00;                      /* start       */

    (hi_void)HAL_CIPHER_WRITE_REG(KLAD_REG_KLAD_CTRL, ctrl);

    return HI_SUCCESS;
}

hi_void hal_cipher_start_klad(hi_u32 block_num)
{
    hi_u32 ctrl = 0;
    hi_u32 high = 0;

    high = (block_num == 1 ? 1 : 0);

    /* start */
    (hi_void)HAL_CIPHER_READ_REG(KLAD_REG_KLAD_CTRL, &ctrl);
    ctrl &= ~(0x01 << 4);
    ctrl |= high << 4;
    ctrl |= 0x01;  /* start */
    (hi_void)HAL_CIPHER_WRITE_REG(KLAD_REG_KLAD_CTRL, ctrl);
}

hi_void hal_cipher_set_klad_data(hi_u32 *data_in)
{
    hi_u32 i = 0;

    for (i = 0; i < 4; i++) {
        (hi_void)HAL_CIPHER_WRITE_REG(KLAD_REG_DAT_IN + i * 4, data_in[i]);
    }
}

hi_void hal_cipher_get_klad_data(hi_u32 *data_out)
{
    hi_u32 i = 0;

    for (i = 0; i < 4; i++) {
        (hi_void)HAL_CIPHER_READ_REG(KLAD_REG_ENC_OUT + i * 4, &data_out[i]);
    }
}

hi_s32 hal_cipher_wait_klad_done(hi_void)
{
    hi_u32 try_count = 0;
    hi_u32 ctrl = 0;

    do {
        HAL_CIPHER_READ_REG(KLAD_REG_KLAD_CTRL, &ctrl);
        if ((ctrl & 0x01) == 0x00) {
            return HI_SUCCESS;
        }
        try_count++;
    } while (try_count < CIPHER_WAIT_IDEL_TIMES);

    HI_LOG_ERROR("Klad time out!\n");

    return HI_FAILURE;

}

hi_void hal_cipher_klad_init(hi_void)
{
    hi_u32 crg_value = 0;
    hi_u32 *sys_addr = HI_NULL;

    sys_addr = crypto_ioremap_nocache(KLAD_CRG_ADDR_PHY, 0x100);
    if (sys_addr == HI_NULL) {
        HI_LOG_ERROR("ERROR: sys_addr ioremap with nocache failed!!\n");
        return ;
    }

    HAL_CIPHER_READ_REG(sys_addr, &crg_value);
    crg_value |= KLAD_CRG_RESET_BIT;   /* reset */
    crg_value |= KLAD_CRG_CLOCK_BIT;   /* set the bit 0, clock opened */
    HAL_CIPHER_WRITE_REG(sys_addr, crg_value);

    /* clock select and cancel reset 0x30100*/
    crg_value &= (~KLAD_CRG_RESET_BIT); /* cancel reset */
    crg_value |= KLAD_CRG_CLOCK_BIT;    /* set the bit 0, clock opened */
    HAL_CIPHER_WRITE_REG(sys_addr, crg_value);

    crypto_iounmap(sys_addr, 0x100);
}

hi_s32 drv_cipher_klad_init(hi_void)
{
    hi_s32 ret = HI_FAILURE;

    cipher_klad_base = crypto_ioremap_nocache(KLAD_REG_BASE_ADDR_PHY, 0x100);
    if (cipher_klad_base == HI_NULL) {
        HI_LOG_ERROR("ERROR: osal_ioremap_nocache for KLAD failed!!\n");
        return HI_FAILURE;
    }

    ret = hal_efuse_otp_init();
    if (ret != HI_SUCCESS) {
        crypto_iounmap(cipher_klad_base, 0x100);
        return ret;
    }

    hal_cipher_klad_init();

    return HI_SUCCESS;
}

hi_void drv_cipher_klad_deinit(hi_void)
{
    if (cipher_klad_base != HI_NULL) {
        crypto_iounmap(cipher_klad_base, 0x100);
        cipher_klad_base = HI_NULL;
    }

    if (efuse_otp_reg_base != HI_NULL) {
        crypto_iounmap(efuse_otp_reg_base, 0x100);
        efuse_otp_reg_base = HI_NULL;
    }

    return ;
}

hi_void drv_cipher_input_buf(hi_u8 *buf, hi_u32 length)
{
    hi_u32 i;
    hi_u8 ch;

    for (i = 0; i < length / 2; i++) {
        ch = buf[i];
        buf[i] = buf[length - i - 1];
        buf[length - i - 1] = ch;
    }
}

hi_s32 drv_cipher_klad_load_key(hi_u32 chn_id,
                              hi_cipher_ca_type root_key,
                              hi_cipher_klad_target target,
                              hi_u8 *data_in,
                              hi_u32 key_len)
{
    hi_s32 ret = HI_FAILURE;
    hi_u32 i = 0;
    hi_u32 key[4] = {0};
    hi_u32 opt_id = 0;;

    if ((root_key < HI_CIPHER_KEY_SRC_KLAD_1) ||
        (root_key > HI_CIPHER_KEY_SRC_KLAD_3)) {
        HI_LOG_ERROR("Error: Invalid Root Key src 0x%x!\n", root_key);
        return HI_ERR_CIPHER_INVALID_PARA;
    }
    if (((key_len % 16 ) != 0) || (key_len == 0)) {
        HI_LOG_ERROR("Error: Invalid key len 0x%x!\n", key_len);
        return HI_ERR_CIPHER_INVALID_PARA;
    }
    if (data_in == HI_NULL) {
        HI_LOG_ERROR("Error: point for input data is null!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    opt_id = root_key - HI_CIPHER_KEY_SRC_KLAD_1 + 1;

    ret = hal_cipher_klad_config(chn_id, opt_id, target, HI_TRUE);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("Error: cipher klad config failed!\n");
        return ret;
    }

    for (i = 0; i < key_len / 16; i++) {
        crypto_memcpy(key, sizeof(key), data_in + i * 16, 16);
        hal_cipher_set_klad_data(key);
        hal_cipher_start_klad(i);
        ret = hal_cipher_wait_klad_done();
        if (ret != HI_SUCCESS) {
            HI_LOG_ERROR("Error: cipher klad wait done failed!\n");
            crypto_memset(key, sizeof(key), 0, sizeof(key));
            return ret;
        }
    }

    crypto_memset(key, sizeof(key), 0, sizeof(key));
    return HI_SUCCESS;
}

hi_s32 drv_cipher_klad_encrypt_key(hi_cipher_ca_type root_key,
                                 hi_cipher_klad_target target,
                                 hi_u32 clean_key[4],
                                 hi_u32 encrypt_key[4])
{
    hi_s32 ret;
    hi_u32 opt_id;

    if ((root_key < HI_CIPHER_KEY_SRC_KLAD_1) ||
        (root_key >= HI_CIPHER_KEY_SRC_BUTT)) {
        HI_LOG_ERROR("Error: Invalid Root Key src 0x%x!\n", root_key);
        return HI_ERR_CIPHER_INVALID_PARA;
    }
    if ((clean_key == HI_NULL) || (encrypt_key == HI_NULL)) {
        HI_LOG_ERROR("Clean key or encrypt key is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    opt_id = root_key - HI_CIPHER_KEY_SRC_KLAD_1 + 1;

    ret = hal_cipher_klad_config(0, opt_id, HI_CIPHER_KLAD_TARGET_AES, HI_FALSE);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("Error: cipher klad config failed!\n");
        return HI_FAILURE;
    }

    if (target == HI_CIPHER_KLAD_TARGET_RSA) {
        drv_cipher_input_buf((hi_u8 *)clean_key, 16);
    }

    hal_cipher_set_klad_data(clean_key);
    hal_cipher_start_klad(0);
    ret = hal_cipher_wait_klad_done();
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("Error: cipher klad wait done failed!\n");
        return ret;
    }
    hal_cipher_get_klad_data(encrypt_key);

    return HI_SUCCESS;
}

