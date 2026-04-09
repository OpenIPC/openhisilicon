/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : drivers for klad.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#include "hi_types.h"
#include "drv_osal_lib.h"
#include "hal_efuse.h"
#include "hal_otp.h"
#include "drv_cipher_kapi.h"
#include "drv_klad.h"

#define KLAD_REG_BASE_ADDR                      g_cipher_klad_base
#define KLAD_BASE_BUF_SIZE                      0x100
#define KLAD_REG_KLAD_CTRL                      (KLAD_REG_BASE_ADDR + 0x00)
#define KLAD_REG_DAT_IN                         (KLAD_REG_BASE_ADDR + 0x10)
#define KLAD_REG_ENC_OUT                        (KLAD_REG_BASE_ADDR + 0x20)

#define CIPHER_WAIT_IDEL_TIMES         1000

static hi_u8 *g_cipher_klad_base = HI_NULL;

hi_s32 hal_cipher_klad_config(hi_u32 chn_id, hi_u32 opt_id, hi_cipher_klad_target target, hi_bool is_decrypt)
{
    hi_s32 ret;
    klad_ctrl ctrl;

    /* Load efuse or OTP key to KLAD */
    ret = hal_efuse_otp_load_cipher_key(chn_id, opt_id);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(hal_efuse_otp_load_cipher_key, ret);
        return ret;
    }

    ctrl.u32 = 0;
    ctrl.bits.klad2ci_addr = chn_id;
    ctrl.bits.type = target;
    ctrl.bits.decrypt = is_decrypt;
    ctrl.bits.start = KLAD_CTRL_NOT_START;

    (hi_void)hal_cipher_write_reg(KLAD_REG_KLAD_CTRL, ctrl.u32);
    return HI_SUCCESS;
}

hi_void hal_cipher_start_klad(hi_u32 block_num)
{
    klad_ctrl ctrl;

    ctrl.u32 = 0;
    hal_cipher_read_reg(KLAD_REG_KLAD_CTRL, &ctrl.u32);

    /* High 128bits is just meaningful for loading cipher 256 bits key, and meaningless to rsa. */
    if (block_num == KLAD_HIGN_128BIT_BLOCK) {
        ctrl.bits.high_low_128bit_flag = KLAD_CTRL_HIGH_128BIT_FLAG;
    } else {
        ctrl.bits.high_low_128bit_flag = KLAD_CTRL_LOW_128BIT_FLAG;
    }

    /* start */
    ctrl.bits.start = KLAD_CTRL_START;
    hal_cipher_write_reg(KLAD_REG_KLAD_CTRL, ctrl.u32);
}

hi_void hal_cipher_set_klad_data(hi_u32 *data_in, hi_u32 data_len_in_word)
{
    hi_u32 i;

    /* The length of klad input key is 16 bytes. */
    for (i = 0; i < data_len_in_word; i++) {
        (hi_void)hal_cipher_write_reg(KLAD_REG_DAT_IN + i * WORD_WIDTH, data_in[i]);
    }
}

hi_void hal_cipher_get_klad_data(hi_u32 *data_out, hi_u32 data_len_in_word)
{
    hi_u32 i;

    /* The length of klad ouput key is 16 bytes. */
    for (i = 0; i < data_len_in_word; i++) {
        (hi_void)hal_cipher_read_reg(KLAD_REG_ENC_OUT + i * WORD_WIDTH, &data_out[i]);
    }
}

hi_s32 hal_cipher_wait_klad_done(hi_void)
{
    hi_s32 i;
    klad_ctrl ctrl;

    for (i = 0; i < CIPHER_WAIT_IDEL_TIMES; i++) {
        ctrl.u32 = 0;
        hal_cipher_read_reg(KLAD_REG_KLAD_CTRL, &ctrl.u32);
        if (ctrl.bits.start == KLAD_CTRL_NOT_START) {
            return HI_SUCCESS;
        }
    }

    if (i >= CIPHER_WAIT_IDEL_TIMES) {
        hi_log_error("Klad wait time out!\n");
        return HI_ERR_CIPHER_TIMEOUT;
    }

    return HI_SUCCESS;
}

hi_void hal_cipher_klad_init(hi_void)
{
    hi_u32 crg_value = 0;
    hi_u32 *sys_addr = HI_NULL;

    sys_addr = crypto_ioremap_nocache(KLAD_CRG_ADDR_PHY, KLAD_BASE_BUF_SIZE);
    if (sys_addr == HI_NULL) {
        hi_log_error("ERROR: sys_addr ioremap with nocache failed!!\n");
        return ;
    }

    hal_cipher_read_reg(sys_addr, &crg_value);
    crg_value |= KLAD_CRG_RESET_BIT;   /* reset */
    crg_value |= KLAD_CRG_CLOCK_BIT;   /* set the bit 0, clock opened */
    hal_cipher_write_reg(sys_addr, crg_value);

    /* clock select and cancel reset 0x30100. */
    crg_value &= (~KLAD_CRG_RESET_BIT); /* cancel reset */
    crg_value |= KLAD_CRG_CLOCK_BIT;    /* set the bit 0, clock opened */
    hal_cipher_write_reg(sys_addr, crg_value);

    crypto_iounmap(sys_addr, KLAD_BASE_BUF_SIZE);
}

hi_s32 drv_cipher_klad_init(hi_void)
{
    hi_s32 ret;

    g_cipher_klad_base = crypto_ioremap_nocache(KLAD_REG_BASE_ADDR_PHY, KLAD_BASE_BUF_SIZE);
    if (g_cipher_klad_base == HI_NULL) {
        hi_log_error("ERROR: osal_ioremap_nocache for KLAD failed!!\n");
        return HI_ERR_CIPHER_FAILED_MEM;
    }

    ret = hal_efuse_otp_init();
    if (ret != HI_SUCCESS) {
        crypto_iounmap(g_cipher_klad_base, KLAD_BASE_BUF_SIZE);
        return ret;
    }

    hal_cipher_klad_init();

    return HI_SUCCESS;
}

hi_void drv_cipher_klad_deinit(hi_void)
{
    hi_u8 *local_efuse_otp_reg_base = HI_NULL;

    if (g_cipher_klad_base != HI_NULL) {
        crypto_iounmap(g_cipher_klad_base, KLAD_BASE_BUF_SIZE);
        g_cipher_klad_base = HI_NULL;
    }

    local_efuse_otp_reg_base = hal_efuse_otp_get_reg_base();
    if (local_efuse_otp_reg_base != HI_NULL) {
        crypto_iounmap(local_efuse_otp_reg_base, KLAD_BASE_BUF_SIZE);
        local_efuse_otp_reg_base = HI_NULL;
        hal_efuse_otp_set_reg_base(local_efuse_otp_reg_base);
    }

    return ;
}

static hi_void drv_cipher_inverse_data(hi_u8 *data, hi_u32 len)
{
    hi_u32 i;
    hi_u8 ch;

    for (i = 0; i < len / MUL_VAL_2; i++) {
        ch = data[i];
        data[i] = data[len - i - BOUND_VAL_1];
        data[len - i - BOUND_VAL_1] = ch;
    }
}

/* load aes or rsa clean key to klad. */
hi_s32 drv_cipher_klad_load_key(hi_u32 chn_id, hi_cipher_ca_type root_key, hi_cipher_klad_target target, hi_u8 *data_in,
    hi_u32 key_len)
{
    hi_s32 ret;
    hi_u32 i, opt_id;
    hi_u32 key[AES_BLOCK_SIZE / WORD_WIDTH] = {0};

    hi_log_chk_param_return((root_key < HI_CIPHER_KEY_SRC_KLAD_1) || (root_key > HI_CIPHER_KEY_SRC_KLAD_3));
    hi_log_chk_param_return(target >= HI_CIPHER_KLAD_TARGET_BUTT);
    hi_log_chk_param_return(data_in == HI_NULL);
    hi_log_chk_param_return(((key_len % AES_BLOCK_SIZE) != 0) || (key_len == 0));

    opt_id = root_key - HI_CIPHER_KEY_SRC_KLAD_1 + BOUND_VAL_1;

    ret = hal_cipher_klad_config(chn_id, opt_id, target, HI_TRUE);
    if (ret != HI_SUCCESS) {
        hi_log_error("Error: cipher klad config failed!\n");
        return ret;
    }

    for (i = 0; i < key_len / AES_BLOCK_SIZE; i++) {
        crypto_memcpy(key, sizeof(key), data_in + i * AES_BLOCK_SIZE, AES_BLOCK_SIZE);
        hal_cipher_set_klad_data(key, sizeof(key) / WORD_WIDTH);
        hal_cipher_start_klad(i);
        ret = hal_cipher_wait_klad_done();
        if (ret != HI_SUCCESS) {
            hi_log_error("Error: cipher klad wait done failed!\n");
            crypto_zeroize(key, sizeof(key));
            return ret;
        }
    }

    crypto_zeroize(key, sizeof(key));
    return HI_SUCCESS;
}

hi_s32 drv_cipher_klad_encrypt_key(hi_cipher_ca_type root_key, hi_cipher_klad_target target, hi_u8 *clean_key,
    hi_u8 *encrypt_key, hi_u32 key_len)
{
    hi_s32 ret;
    hi_u32 opt_id;

    hi_log_chk_param_return((root_key < HI_CIPHER_KEY_SRC_KLAD_1) || (root_key >= HI_CIPHER_KEY_SRC_BUTT));
    hi_log_chk_param_return(target >= HI_CIPHER_KLAD_TARGET_BUTT);
    hi_log_chk_param_return((clean_key == HI_NULL) || (encrypt_key == HI_NULL));
    hi_log_chk_param_return(key_len != AES_BLOCK_SIZE);

    opt_id = root_key - HI_CIPHER_KEY_SRC_KLAD_1 + BOUND_VAL_1;

    ret = hal_cipher_klad_config(0, opt_id, HI_CIPHER_KLAD_TARGET_AES, HI_FALSE);
    if (ret != HI_SUCCESS) {
        hi_log_error("Error: cipher klad config failed!\n");
        return ret;
    }

    if (target == HI_CIPHER_KLAD_TARGET_RSA) {
        drv_cipher_inverse_data(clean_key, key_len);
    }

    hal_cipher_set_klad_data((hi_u32 *)clean_key, key_len / WORD_WIDTH);
    hal_cipher_start_klad(0);
    ret = hal_cipher_wait_klad_done();
    if (ret != HI_SUCCESS) {
        hi_log_error("Error: cipher klad wait done failed!\n");
        return ret;
    }
    hal_cipher_get_klad_data((hi_u32 *)encrypt_key, key_len / WORD_WIDTH);

    return HI_SUCCESS;
}

