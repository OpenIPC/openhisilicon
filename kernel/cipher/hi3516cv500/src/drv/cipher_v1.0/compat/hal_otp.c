/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : drivers for hal otp.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#include "hi_types.h"
#include "drv_osal_lib.h"
#include "hal_otp.h"

#ifdef OTP_SUPPORT

hi_u8 *g_efuse_otp_reg_base = HI_NULL;

/* OTP init */
hi_s32 hal_efuse_otp_init(hi_void)
{
    hi_u32 crg_value = 0;
    hi_u32 *sys_addr = HI_NULL;

    sys_addr = crypto_ioremap_nocache(REG_SYS_OTP_CLK_ADDR_PHY, 0x100);
    if (sys_addr == HI_NULL) {
        hi_log_error("ERROR: sys_addr ioremap with nocache failed!!\n");
        return HI_ERR_CIPHER_FAILED_MEM;
    }

    hal_cipher_read_reg(sys_addr, &crg_value);
#if defined(CHIP_TYPE_hi3559av100) || defined(CHIP_TYPE_hi3569v100)
    crg_value |= OTP_CRG_RESET_BIT;   /* reset */
    crg_value |= OTP_CRG_CLOCK_BIT;   /* set the bit 0, clock opened */
    hal_cipher_write_reg(sys_addr, crg_value);

    /* clock select and cancel reset 0x30100. */
    crg_value &= (~OTP_CRG_RESET_BIT); /* cancel reset */
#endif
    crg_value |= OTP_CRG_CLOCK_BIT;   /* set the bit 0, clock opened */
    hal_cipher_write_reg(sys_addr, crg_value);

    crypto_iounmap(sys_addr, 0x100);

    g_efuse_otp_reg_base = crypto_ioremap_nocache(OTP_REG_BASE_ADDR_PHY, 0x100);
    if (g_efuse_otp_reg_base == HI_NULL) {
        hi_log_error("ERROR: osal_ioremap_nocache for OTP failed!!\n");
        return HI_ERR_CIPHER_FAILED_MEM;
    }

    return HI_SUCCESS;
}

hi_s32 hal_otp_wait_free(hi_void)
{
    hi_u32 time_out_cnt = 0;
    hi_u32 reg_value = 0;

    while (1) {
        hal_cipher_read_reg(OTP_USER_CTRL_STA, &reg_value);
        if ((reg_value & 0x1) == 0) {
            /* bit0:otp_op_busy 0:idle, 1:busy */
            return HI_SUCCESS;
        }

        time_out_cnt++;
        if (time_out_cnt >= OTP_WAIT_TIME_OUT) {
            hi_log_error("Otp wait free time out!\n");
            break;
        }
    }
    return HI_ERR_CIPHER_TIMEOUT;
}

hi_s32 hal_otp_set_mode(otp_user_work_mode otp_mode)
{
    hi_u32 reg_value = otp_mode;

    if (otp_mode >= OTP_UNKOOWN_MODE) {
        hi_log_error("Mode Unknown!\n");
        return  HI_ERR_CIPHER_INVALID_PARAM;
    }

    (hi_void)hal_cipher_write_reg(OTP_USER_WORK_MODE, reg_value);
    return HI_SUCCESS;
}

hi_void hal_otp_op_start(hi_void)
{
    hi_u32 reg_value = OTP_OP_START_VAL;
    (hi_void)hal_cipher_write_reg(OTP_USER_OP_START, reg_value);
}

hi_s32 hal_otp_wait_op_done(hi_void)
{
    hi_u32 time_out_cnt = 0;
    hi_u32 reg_value = 0;

    while (1) {
        hal_cipher_read_reg(OTP_USER_CTRL_STA, &reg_value);
        if (reg_value & 0x2) {
            return HI_SUCCESS;
        }

        time_out_cnt++;
        if (time_out_cnt >= OTP_WAIT_TIME_OUT) {
            hi_log_error("Otp wait op time out!\n");
            break;
        }
    }
    return HI_ERR_CIPHER_TIMEOUT;
}

hi_void hal_choose_otp_key(otp_user_key_index which_key)
{
    hi_u32 reg_value;

    reg_value = which_key;
    (hi_void)hal_cipher_write_reg(OTP_USER_KEY_INDEX, reg_value);
}

/* set otp key to klad */
hi_s32 hal_efuse_otp_load_cipher_key(hi_u32 chn_id, hi_u32 opt_id)
{
    hi_s32 ret;

    if (opt_id > OTP_USER_KEY3) {
        opt_id = OTP_USER_KEY0;
    }

    ret = hal_otp_wait_free();
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(hal_otp_wait_free, ret);
        return ret;
    }

    hal_choose_otp_key(opt_id);

    ret = hal_otp_set_mode(OTP_LOCK_CIPHER_KEY_MODE);
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(hal_otp_set_mode, ret);
        return ret;
    }

    hal_otp_op_start();

    ret = hal_otp_wait_op_done();
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(hal_otp_wait_op_done, ret);
        return ret;
    }

    return  HI_SUCCESS;
}

hi_void hal_efuse_otp_set_reg_base(hi_u8 *reg_base)
{
    g_efuse_otp_reg_base = reg_base;
}

hi_u8 *hal_efuse_otp_get_reg_base(hi_void)
{
    return g_efuse_otp_reg_base;
}
#endif

