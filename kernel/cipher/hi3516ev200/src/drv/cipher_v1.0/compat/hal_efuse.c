/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : SAR_ADC_DRIVER.c
  Version       : Initial Draft
  Author        : Hisilicon BVT
  Created       : 2012/5/2
  Description   :
  History       :
  1.Date        : 2012/5/2
    Author      : hkf67331
    Modification: Created file

******************************************************************************/
#include "hi_types.h"
#include "drv_osal_lib.h"
#include "hal_efuse.h"

#ifdef EFUSE_SUPPORT

hi_u8 *efuse_otp_reg_base = HI_NULL;

#define EFUSE_REG_BASE_ADDR  efuse_otp_reg_base
#define CIPHER_KD_WKEY0      (EFUSE_REG_BASE_ADDR + 0x00)
#define CIPHER_KD_WKEY1      (EFUSE_REG_BASE_ADDR + 0x04)
#define CIPHER_KD_WKEY2      (EFUSE_REG_BASE_ADDR + 0x08)
#define CIPHER_KD_WKEY3      (EFUSE_REG_BASE_ADDR + 0x0c)
#define CIPHER_KD_CTRL       (EFUSE_REG_BASE_ADDR + 0x10)
#define CIPHER_KD_STA        (EFUSE_REG_BASE_ADDR + 0x14)
#define OTP_PGM_TIME         (EFUSE_REG_BASE_ADDR + 0x18)
#define OTP_RD_TIME          (EFUSE_REG_BASE_ADDR + 0x1c)
#define OTP_LOGIC_LEVEL      (EFUSE_REG_BASE_ADDR + 0x20)
#define KD_CTL_MODE_CIPHER_KEY_ADDR(chn_id)  (chn_id<<8)
#define KD_CTL_MODE_OPT_KEY_ADDR(opt_id)     (opt_id<<4)
#define KD_CTL_MODE_HASH_KL                   (0x8)
#define KD_CTL_MODE_OPT_KD                    (0x4)
#define KD_CTL_MODE_CIPHER_KL                 (0x2)
#define KD_CTL_MODE_START                     (0x1)
#define KD_TIME_OUT                           (1000)

#define  REG_SYS_EFUSE_CLK_ADDR_PHY     0x120100D8

/* Define the union cipher_kd_sta */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    cipher_kl_finish       : 1   ; /* [0]  */
        unsigned int    hash_key_read_busy     : 1   ; /* [1]  */
        unsigned int    Reserved_3             : 25  ; /* [26..2]  */
        unsigned int    ctrl_rdy               : 1   ; /* [27]  */
        unsigned int    ctrl_busy0             : 1   ; /* [28]  */
        unsigned int    ctrl_busy1             : 1   ; /* [29]  */
        unsigned int    key_wt_error           : 1   ; /* [30]  */
        unsigned int    key_wt_finish          : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} cipher_kd_sta;

static hi_u32 is_efuse_busy_flag = HI_FALSE;

hi_s32 hal_efuse_otp_init(hi_void)
{
    hi_u32 crg_value;
    hi_u32 *sys_addr;

    sys_addr = crypto_ioremap_nocache(REG_SYS_EFUSE_CLK_ADDR_PHY, 0x100);
    if (sys_addr == HI_NULL) {
        HI_LOG_ERROR("Error! addr ioremap failed!\n");
        return HI_FAILURE;
    }

    HAL_CIPHER_READ_REG(sys_addr, &crg_value);
    crg_value |= 0x01;/* reset */
    crg_value |= 0x02;   /* set the bit 0, clock opened */
    HAL_CIPHER_WRITE_REG(sys_addr, crg_value);

    /* clock select and cancel reset 0x30100*/
    crg_value &= (~0x01); /* cancel reset */
    crg_value |= 0x02;   /* set the bit 0, clock opened */
    HAL_CIPHER_WRITE_REG(sys_addr, crg_value);

    crypto_iounmap(sys_addr, 0x100);

    efuse_otp_reg_base = crypto_ioremap_nocache(ENFUSE_REG_BASE_ADDR_PHY, 0x100);
    if (efuse_otp_reg_base == HI_NULL) {
        HI_LOG_ERROR("ERROR: osal_ioremap_nocache for EFUSE failed!!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

hi_s32 hal_efuse_wait_write_key(hi_void)
{
    cipher_kd_sta efuse_sta;
    hi_u32 ul_start_time = 0;
    hi_u32 ul_last_time = 0;
    hi_u32 ul_dura_time = 0;

    /* wait for hash_rdy */
    ul_start_time = osal_get_tickcount();
    while (1) {
        HAL_CIPHER_READ_REG(CIPHER_KD_STA, &efuse_sta.u32);
        if (efuse_sta.bits.key_wt_finish == 1) {
            break;
        }

        ul_last_time = osal_get_tickcount();
        ul_dura_time = ul_last_time - ul_start_time;
        if (ul_dura_time >= KD_TIME_OUT ) {
            HI_LOG_ERROR("Error! efuse write key time out!\n");
            return HI_FAILURE;
        }

        osal_msleep(1);
    }
    return HI_SUCCESS;
}

hi_s32 hal_efuse_wait_cipher_load_key(hi_void)
{
    cipher_kd_sta efuse_sta;
    hi_u32 ul_start_time = 0;
    hi_u32 ul_last_time = 0;
    hi_u32 ul_dura_time = 0;

    ul_start_time = osal_get_tickcount();

    while (1) {
        HAL_CIPHER_READ_REG(CIPHER_KD_STA, &efuse_sta.u32);
        if (efuse_sta.bits.cipher_kl_finish == 1) {
            break;
        }

        ul_last_time = osal_get_tickcount();
        ul_dura_time = (ul_last_time - ul_start_time);
        if (ul_dura_time >= KD_TIME_OUT ) {
            HI_LOG_ERROR("Error! efuse load key time out!\n");
            return HI_FAILURE;
        }
        osal_msleep(1);
    }
    return HI_SUCCESS;
}

hi_s32 hal_efuse_wait_hash_load_key(hi_void)
{
    cipher_kd_sta efuse_sta;
    hi_u32 ul_start_time = 0;
    hi_u32 ul_last_time = 0;
    hi_u32 ul_dura_time = 0;

    ul_start_time = osal_get_tickcount();

    while (1) {
        HAL_CIPHER_READ_REG(CIPHER_KD_STA, &efuse_sta.u32);
        if (efuse_sta.bits.hash_key_read_busy == 0) {
            break;
        }

        ul_last_time = osal_get_tickcount();
        ul_dura_time = (ul_last_time - ul_start_time);
        if (ul_dura_time >= KD_TIME_OUT ) {
            HI_LOG_ERROR("Error! efuse load key out!\n");
            return HI_FAILURE;
        }
        osal_msleep(1);
    }
    return HI_SUCCESS;
}

hi_s32 hal_efuse_wait_ready(hi_void)
{
    cipher_kd_sta efuse_sta;
    hi_u32 ul_start_time = 0;
    hi_u32 ul_last_time = 0;
    hi_u32 ul_dura_time = 0;

    ul_start_time = osal_get_tickcount();

    while (1) {
        HAL_CIPHER_READ_REG(CIPHER_KD_STA, &efuse_sta.u32);
        if (efuse_sta.bits.ctrl_rdy && (!efuse_sta.bits.ctrl_busy1) && (!efuse_sta.bits.ctrl_busy0)) {
            break;
        }

        ul_last_time = osal_get_tickcount();
        ul_dura_time = (ul_last_time - ul_start_time);
        if (ul_dura_time >= KD_TIME_OUT ) {
            HI_LOG_ERROR("Error! efuse load key out!\n");
            return HI_FAILURE;
        }
        osal_msleep(1);
    }
    return HI_SUCCESS;
}

hi_s32 hal_efuse_get_err_stat(hi_void)
{
    cipher_kd_sta efuse_sta;

    HAL_CIPHER_READ_REG(CIPHER_KD_STA, &efuse_sta.u32);
    return efuse_sta.bits.key_wt_error;
}

hi_s32 hal_efuse_write_key(hi_u32 *p_key, hi_u32 opt_id)
{
    hi_s32 ret = HI_FAILURE;
    hi_u32 kd_ctl_mode = 0;

    if (is_efuse_busy_flag != HI_FALSE) {
        return HI_FAILURE;
    }

    is_efuse_busy_flag = HI_TRUE;
    kd_ctl_mode = KD_CTL_MODE_OPT_KEY_ADDR(opt_id) | KD_CTL_MODE_OPT_KD | KD_CTL_MODE_START;

    HAL_CIPHER_WRITE_REG(CIPHER_KD_WKEY0, *p_key);
    HAL_CIPHER_WRITE_REG(CIPHER_KD_WKEY1, *(p_key + 1));
    HAL_CIPHER_WRITE_REG(CIPHER_KD_WKEY2, *(p_key + 2));
    HAL_CIPHER_WRITE_REG(CIPHER_KD_WKEY3, *(p_key + 3));

    hal_efuse_wait_ready();

    HAL_CIPHER_WRITE_REG(CIPHER_KD_CTRL, kd_ctl_mode);

    ret = hal_efuse_wait_write_key();
    if (ret != HI_SUCCESS) {
        HI_LOG_PRINT_FUNC_ERR(hal_efuse_wait_write_key, ret);
        is_efuse_busy_flag = HI_FALSE;
        return ret;
    }

    ret = hal_efuse_get_err_stat()
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("efuse key is already write.\n");
        HI_LOG_PRINT_FUNC_ERR(hal_efuse_get_err_stat, ret);
        is_efuse_busy_flag = HI_FALSE;
        return ret;
    }

    is_efuse_busy_flag = HI_FALSE;
    return HI_SUCCESS;
}

hi_s32 hal_efuse_otp_load_cipher_key(hi_u32 chn_id, hi_u32 opt_id)
{
    hi_s32 ret = HI_FAILURE;
    hi_u32 kd_ctl_mode = 0;

    if (is_efuse_busy_flag != HI_FALSE) {
        return HI_FAILURE;
    }

    is_efuse_busy_flag = HI_TRUE;

    kd_ctl_mode = (KD_CTL_MODE_CIPHER_KEY_ADDR(chn_id) \
                   | KD_CTL_MODE_OPT_KEY_ADDR(opt_id)  \
                   | KD_CTL_MODE_CIPHER_KL | KD_CTL_MODE_START);

    hal_efuse_wait_ready();
    HAL_CIPHER_WRITE_REG(CIPHER_KD_CTRL, kd_ctl_mode);
    ret = hal_efuse_wait_cipher_load_key();
    if (ret != HI_SUCCESS) {
        is_efuse_busy_flag = HI_FALSE;
        HI_LOG_PRINT_FUNC_ERR(hal_efuse_wait_cipher_load_key, ret);
        return ret;
    }

    is_efuse_busy_flag = HI_FALSE;
    return HI_SUCCESS;
}

hi_s32 hal_efuse_load_hash_key(hi_u32 opt_id)
{
    hi_s32 ret = HI_FAILURE;
    hi_u32 kd_ctl_mode = 0;

    if (is_efuse_busy_flag)
    { return HI_FAILURE; }

    is_efuse_busy_flag = HI_TRUE;

    kd_ctl_mode = (KD_CTL_MODE_OPT_KEY_ADDR(opt_id) | KD_CTL_MODE_HASH_KL | KD_CTL_MODE_START);
    hal_efuse_wait_ready();
    HAL_CIPHER_WRITE_REG(CIPHER_KD_CTRL, kd_ctl_mode);
    ret = hal_efuse_wait_hash_load_key();
    if (ret != HI_SUCCESS) {
        is_efuse_busy_flag = HI_FALSE;
        HI_LOG_PRINT_FUNC_ERR(hal_efuse_wait_hash_load_key, ret);
        return ret;
    }

    is_efuse_busy_flag = HI_FALSE;
    return HI_SUCCESS;
}
#endif


