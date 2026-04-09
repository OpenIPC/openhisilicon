/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : drivers for hal efuse.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#include "hi_types.h"
#include "drv_osal_lib.h"
#include "hal_efuse.h"

#ifdef EFUSE_SUPPORT
hi_u8 *g_efuse_otp_reg_base = HI_NULL;

#define EFUSE_REG_BASE_ADDR  g_efuse_otp_reg_base
#define CIPHER_KD_WKEY0      (EFUSE_REG_BASE_ADDR + 0x00)
#define CIPHER_KD_WKEY1      (EFUSE_REG_BASE_ADDR + 0x04)
#define CIPHER_KD_WKEY2      (EFUSE_REG_BASE_ADDR + 0x08)
#define CIPHER_KD_WKEY3      (EFUSE_REG_BASE_ADDR + 0x0c)
#define CIPHER_KD_CTRL       (EFUSE_REG_BASE_ADDR + 0x10)
#define CIPHER_KD_STA        (EFUSE_REG_BASE_ADDR + 0x14)
#define OTP_PGM_TIME         (EFUSE_REG_BASE_ADDR + 0x18)
#define OTP_RD_TIME          (EFUSE_REG_BASE_ADDR + 0x1c)
#define OTP_LOGIC_LEVEL      (EFUSE_REG_BASE_ADDR + 0x20)
#define kd_ctl_mode_cipher_key_addr(chn_id)  (chn_id << 8)
#define kd_ctl_mode_opt_key_addr(opt_id)     (opt_id << 4)
#define KD_CTL_MODE_HASH_KL                  0x8
#define KD_CTL_MODE_OPT_KD                   0x4
#define KD_CTL_MODE_CIPHER_KL                0x2
#define KD_CTL_MODE_START                    0x1
#define KD_TIME_OUT                          1000

#define  REG_SYS_EFUSE_CLK_ADDR_PHY     0x120100D8

/* Define the union cipher_kd_sta */
typedef union {
    /* Define the struct bits */
    struct {
        hi_u32    cipher_kl_finish       : 1   ; /* [0]  */
        hi_u32    hash_key_read_busy     : 1   ; /* [1]  */
        hi_u32    reserved_3             : 25  ; /* [26..2]  */
        hi_u32    ctrl_rdy               : 1   ; /* [27]  */
        hi_u32    ctrl_busy0             : 1   ; /* [28]  */
        hi_u32    ctrl_busy1             : 1   ; /* [29]  */
        hi_u32    key_wt_error           : 1   ; /* [30]  */
        hi_u32    key_wt_finish          : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    hi_u32    u32;
} cipher_kd_sta;

static hi_u32 g_is_efuse_busy_flag = HI_FALSE;

hi_s32 hal_efuse_otp_init(hi_void)
{
    hi_u32 crg_value;
    hi_u32 *sys_addr;

    sys_addr = crypto_ioremap_nocache(REG_SYS_EFUSE_CLK_ADDR_PHY, 0x100);
    if (sys_addr == HI_NULL) {
        hi_log_error("Error! addr ioremap failed!\n");
        return HI_ERR_CIPHER_FAILED_MEM;
    }

    hal_cipher_read_reg(sys_addr, &crg_value);
    crg_value |= 0x01;      /* reset */
    crg_value |= 0x02;      /* set the bit 0, clock opened */
    hal_cipher_write_reg(sys_addr, crg_value);

    /* clock select and cancel reset 0x30100. */
    crg_value &= (~0x01);   /* cancel reset. */
    crg_value |= 0x02;      /* set the bit 0, clock opened */
    hal_cipher_write_reg(sys_addr, crg_value);

    crypto_iounmap(sys_addr, 0x100);

    g_efuse_otp_reg_base = crypto_ioremap_nocache(ENFUSE_REG_BASE_ADDR_PHY, 0x100);
    if (g_efuse_otp_reg_base == HI_NULL) {
        hi_log_error("ERROR: osal_ioremap_nocache for EFUSE failed!!\n");
        return HI_ERR_CIPHER_FAILED_MEM;
    }

    return HI_SUCCESS;
}

hi_s32 hal_efuse_wait_write_key(hi_void)
{
    cipher_kd_sta efuse_sta;
    hi_u32 ul_start_time, ul_last_time, ul_dura_time;

    /* wait for hash_rdy */
    ul_start_time = osal_get_tickcount();
    while (1) {
        hal_cipher_read_reg(CIPHER_KD_STA, &efuse_sta.u32);
        if (efuse_sta.bits.key_wt_finish == 1) {
            break;
        }

        ul_last_time = osal_get_tickcount();
        ul_dura_time = ul_last_time - ul_start_time;
        if (ul_dura_time >= KD_TIME_OUT) {
            hi_log_error("Error! efuse write key time out!\n");
            return HI_ERR_CIPHER_TIMEOUT;
        }

        osal_msleep(1);
    }
    return HI_SUCCESS;
}

hi_s32 hal_efuse_wait_cipher_load_key(hi_void)
{
    cipher_kd_sta efuse_sta;
    hi_u32 ul_start_time, ul_last_time, ul_dura_time;

    ul_start_time = osal_get_tickcount();

    while (1) {
        hal_cipher_read_reg(CIPHER_KD_STA, &efuse_sta.u32);
        if (efuse_sta.bits.cipher_kl_finish == 1) {
            break;
        }

        ul_last_time = osal_get_tickcount();
        ul_dura_time = (ul_last_time - ul_start_time);
        if (ul_dura_time >= KD_TIME_OUT) {
            hi_log_error("Error! efuse load key time out!\n");
            return HI_ERR_CIPHER_TIMEOUT;
        }
        osal_msleep(1);
    }
    return HI_SUCCESS;
}

hi_s32 hal_efuse_wait_hash_load_key(hi_void)
{
    cipher_kd_sta efuse_sta;
    hi_u32 ul_start_time, ul_last_time, ul_dura_time;

    ul_start_time = osal_get_tickcount();

    while (1) {
        hal_cipher_read_reg(CIPHER_KD_STA, &efuse_sta.u32);
        if (efuse_sta.bits.hash_key_read_busy == 0) {
            break;
        }

        ul_last_time = osal_get_tickcount();
        ul_dura_time = (ul_last_time - ul_start_time);
        if (ul_dura_time >= KD_TIME_OUT) {
            hi_log_error("Error! efuse load key out!\n");
            return HI_ERR_CIPHER_TIMEOUT;
        }
        osal_msleep(1);
    }
    return HI_SUCCESS;
}

hi_s32 hal_efuse_wait_ready(hi_void)
{
    cipher_kd_sta efuse_sta;
    hi_u32 ul_start_time, ul_last_time, ul_dura_time;

    ul_start_time = osal_get_tickcount();

    while (1) {
        hal_cipher_read_reg(CIPHER_KD_STA, &efuse_sta.u32);
        if (efuse_sta.bits.ctrl_rdy && (!efuse_sta.bits.ctrl_busy1) && (!efuse_sta.bits.ctrl_busy0)) {
            break;
        }

        ul_last_time = osal_get_tickcount();
        ul_dura_time = (ul_last_time - ul_start_time);
        if (ul_dura_time >= KD_TIME_OUT) {
            hi_log_error("Error! efuse load key out!\n");
            return HI_ERR_CIPHER_TIMEOUT;
        }
        osal_msleep(1);
    }
    return HI_SUCCESS;
}

hi_s32 hal_efuse_get_err_stat(hi_void)
{
    cipher_kd_sta efuse_sta;

    hal_cipher_read_reg(CIPHER_KD_STA, &efuse_sta.u32);
    return efuse_sta.bits.key_wt_error;
}

hi_s32 hal_efuse_write_key(hi_u32 *p_key, hi_u32 opt_id)
{
    hi_s32 ret;
    hi_u32 kd_ctl_mode;

    if (g_is_efuse_busy_flag != HI_FALSE) {
        return HI_ERR_CIPHER_BUSY;
    }

    g_is_efuse_busy_flag = HI_TRUE;
    kd_ctl_mode = kd_ctl_mode_opt_key_addr(opt_id) | KD_CTL_MODE_OPT_KD | KD_CTL_MODE_START;

    hal_cipher_write_reg(CIPHER_KD_WKEY0, *p_key);
    hal_cipher_write_reg(CIPHER_KD_WKEY1, *(p_key + WORD_IDX_1));
    hal_cipher_write_reg(CIPHER_KD_WKEY2, *(p_key + WORD_IDX_2));
    hal_cipher_write_reg(CIPHER_KD_WKEY3, *(p_key + WORD_IDX_3));

    ret = hal_efuse_wait_ready();
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(hal_efuse_wait_ready, ret);
        g_is_efuse_busy_flag = HI_FALSE;
        return ret;
    }

    hal_cipher_write_reg(CIPHER_KD_CTRL, kd_ctl_mode);

    ret = hal_efuse_wait_write_key();
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(hal_efuse_wait_write_key, ret);
        g_is_efuse_busy_flag = HI_FALSE;
        return ret;
    }

    ret = hal_efuse_get_err_stat()
    if (ret != HI_SUCCESS) {
        hi_log_error("efuse key is already write.\n");
        hi_log_print_func_err(hal_efuse_get_err_stat, ret);
        g_is_efuse_busy_flag = HI_FALSE;
        return ret;
    }

    g_is_efuse_busy_flag = HI_FALSE;
    return HI_SUCCESS;
}

hi_s32 hal_efuse_otp_load_cipher_key(hi_u32 chn_id, hi_u32 opt_id)
{
    hi_s32 ret;
    hi_u32 kd_ctl_mode;

    if (g_is_efuse_busy_flag != HI_FALSE) {
        return HI_ERR_CIPHER_BUSY;
    }

    g_is_efuse_busy_flag = HI_TRUE;

    kd_ctl_mode = (kd_ctl_mode_cipher_key_addr(chn_id) | kd_ctl_mode_opt_key_addr(opt_id) | \
        KD_CTL_MODE_CIPHER_KL | KD_CTL_MODE_START);

    ret = hal_efuse_wait_ready();
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(hal_efuse_wait_ready, ret);
        g_is_efuse_busy_flag = HI_FALSE;
        return ret;
    }

    hal_cipher_write_reg(CIPHER_KD_CTRL, kd_ctl_mode);
    ret = hal_efuse_wait_cipher_load_key();
    if (ret != HI_SUCCESS) {
        g_is_efuse_busy_flag = HI_FALSE;
        hi_log_print_func_err(hal_efuse_wait_cipher_load_key, ret);
        return ret;
    }

    g_is_efuse_busy_flag = HI_FALSE;
    return HI_SUCCESS;
}

hi_s32 hal_efuse_load_hash_key(hi_u32 opt_id)
{
    hi_s32 ret;
    hi_u32 kd_ctl_mode;

    if (g_is_efuse_busy_flag == HI_TRUE) {
        return HI_ERR_CIPHER_BUSY;
    }

    g_is_efuse_busy_flag = HI_TRUE;

    kd_ctl_mode = (kd_ctl_mode_opt_key_addr(opt_id) | KD_CTL_MODE_HASH_KL | KD_CTL_MODE_START);
    ret = hal_efuse_wait_ready();
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(hal_efuse_wait_ready, ret);
        g_is_efuse_busy_flag = HI_FALSE;
        return ret;
    }

    hal_cipher_write_reg(CIPHER_KD_CTRL, kd_ctl_mode);
    ret = hal_efuse_wait_hash_load_key();
    if (ret != HI_SUCCESS) {
        g_is_efuse_busy_flag = HI_FALSE;
        hi_log_print_func_err(hal_efuse_wait_hash_load_key, ret);
        return ret;
    }

    g_is_efuse_busy_flag = HI_FALSE;
    return HI_SUCCESS;
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
