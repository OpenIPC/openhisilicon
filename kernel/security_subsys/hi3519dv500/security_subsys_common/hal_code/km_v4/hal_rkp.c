/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "hal_rkp.h"
#include "hal_rkp_reg.h"
#include "crypto_drv_common.h"
#include "crypto_common_macro.h"

#define CRYPTO_U8_TO_U32_BIT_SHIFT(data, i) \
    ((td_u32)(data)[(i)*4]  | ((td_u32)(data)[(i)*4+1] << 8) | ((td_u32)(data)[(i)*4+2] << 16) | \
    ((td_u32)(data)[(i)*4+3]<< 24))

#define KM_COMPAT_ERRNO(err_code)     HAL_COMPAT_ERRNO(ERROR_MODULE_KM, err_code)
#define km_null_ptr_chk(ptr)   \
    crypto_chk_return((ptr) == TD_NULL, KM_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), #ptr" is NULL\n")

#define RKP_LOCK_TIMEOUT_IN_US      1000000
td_s32 hal_rkp_lock(td_void)
{
    td_u32 i = 0;
    rkp_lock lock_val = {0};
    td_u32 rkp_config_val = RKP_LOCK_CPU_REE;
    crypto_cpu_type cpu_type = crypto_get_cpu_type();
    if (cpu_type == CRYPTO_CPU_TYPE_SCPU) {
        rkp_config_val = RKP_LOCK_CPU_TEE;
    }

    for (i = 0; i < RKP_LOCK_TIMEOUT_IN_US; i++) {
        km_reg_write(RKP_LOCK, rkp_config_val);
        lock_val.u32 = km_reg_read(RKP_LOCK);
        if (lock_val.bits.km_lock_status == rkp_config_val) {
            break;
        }
        crypto_udelay(1);
    }

    if (i >= RKP_LOCK_TIMEOUT_IN_US) {
        crypto_log_err("drv_rkp_lock busy.\n");
        return KM_COMPAT_ERRNO(ERROR_RKP_LOCK_TIMEOUT);
    }

    return TD_SUCCESS;
}

td_s32 hal_rkp_unlock(void)
{
    km_reg_write(RKP_LOCK, RKP_LOCK_CPU_IDLE);
    return TD_SUCCESS;
}

#define DEOB_UPDATE_KEY_SEL_MRK1        0
#define DEOB_UPDATE_KEY_SEL_USK         1
#define DEOB_UPDATE_KEY_SEL_RUSK        2

#define DEOB_UPDATE_ALG_SEL_AES         0
#define DEOB_UPDATE_ALG_SEL_SM4         1

#define RKP_DEOB_UPDATE_TIMEOUT_IN_US      1000000

td_s32 hal_rkp_deob_update(crypto_kdf_otp_key otp_key, crypto_kdf_update_alg alg)
{
    volatile td_s32 ret = TD_FAILURE;
    td_u8 deob_key_reg_val = 0;
    td_u8 deob_alg_reg_val = 0;
    rkp_deob_cfg deob_cfg = { 0 };

    if (otp_key == CRYPTO_KDF_OTP_KEY_MRK1) {
        deob_key_reg_val = DEOB_UPDATE_KEY_SEL_MRK1;
    } else if (otp_key == CRYPTO_KDF_OTP_KEY_USK) {
        deob_key_reg_val = DEOB_UPDATE_KEY_SEL_USK;
    } else if (otp_key == CRYPTO_KDF_OTP_KEY_RUSK) {
        deob_key_reg_val = DEOB_UPDATE_KEY_SEL_RUSK;
    } else {
        crypto_log_err("invalid otp_key\n");
        return KM_COMPAT_ERRNO(ERROR_INVALID_PARAM);
    }

    if (alg == CRYPTO_KDF_UPDATE_ALG_AES) {
        deob_alg_reg_val = DEOB_UPDATE_ALG_SEL_AES;
    } else if (alg == CRYPTO_KDF_UPDATE_ALG_SM4) {
        deob_alg_reg_val = DEOB_UPDATE_ALG_SEL_SM4;
    } else {
        crypto_log_err("invalid alg\n");
        return KM_COMPAT_ERRNO(ERROR_INVALID_PARAM);
    }

    deob_cfg.u32 = km_reg_read(RKP_DEOB_CFG);
    deob_cfg.bits.deob_update_alg_sel = deob_alg_reg_val;
    deob_cfg.bits.deob_update_sel = deob_key_reg_val;
    deob_cfg.bits.deob_update_req = 0x1; /* start calculation */
    km_reg_write(RKP_DEOB_CFG, deob_cfg.u32);

    ret = hal_rkp_deob_wait_done();
    if (ret != TD_SUCCESS) {
        crypto_log_err("hal_rkp_deob_wait_done failed\n");
        ret = KM_COMPAT_ERRNO(ERROR_RKP_CALC_TIMEOUT);
    }

    return ret;
}

#define KDF_MAX_VAL_LENGTH      64

td_s32 hal_rkp_kdf_set_val(const td_u8 *kdf_val, td_u32 val_length)
{
    td_u32 i;
    td_u32 kdf_val_word = 0;

    km_null_ptr_chk(kdf_val);
    crypto_chk_return(val_length > KDF_MAX_VAL_LENGTH, KM_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "val_length is too long\n");
    crypto_chk_return((val_length % CRYPTO_WORD_WIDTH) != 0, KM_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "invalid val_length\n");

    for (i = 0; i < val_length / CRYPTO_WORD_WIDTH; i++) {
        kdf_val_word = CRYPTO_U8_TO_U32_BIT_SHIFT(kdf_val, i);
        km_reg_write(RKP_PBKDF2_VAL(i), kdf_val_word);
    }
    return TD_SUCCESS;
}

#define KDF_PADDING_VAL_LEN         64
td_s32 hal_rkp_kdf_get_val(td_u8 *kdf_val, td_u32 val_length)
{
    td_u32 i;
    td_u32 kdf_val_word = 0;

    km_null_ptr_chk(kdf_val);
    crypto_chk_return(val_length > KDF_MAX_VAL_LENGTH, KM_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "val_length is too long\n");
    crypto_chk_return((val_length % CRYPTO_WORD_WIDTH) != 0, KM_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "invalid val_length\n");

    for (i = 0; i < val_length / CRYPTO_WORD_WIDTH; i++) {
        kdf_val_word = km_reg_read(RKP_PBKDF2_VAL(i));
        (td_void)memcpy_s(kdf_val + i * CRYPTO_WORD_WIDTH, sizeof(kdf_val_word), &kdf_val_word, sizeof(kdf_val_word));
    }
    return TD_SUCCESS;
}

#define KDF_PADDING_SALT_LEN         128
td_s32 hal_rkp_kdf_set_padding_salt(const td_u8 *padding_salt, td_u32 salt_length)
{
    td_u32 i;
    td_u32 salt_word = 0;

    km_null_ptr_chk(padding_salt);
    crypto_chk_return(salt_length != KDF_PADDING_SALT_LEN, KM_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "invalid salt_length\n");

    for (i = 0; i < salt_length / CRYPTO_WORD_WIDTH; i++) {
        salt_word = CRYPTO_U8_TO_U32_BIT_SHIFT(padding_salt, i);
        km_reg_write(RKP_PBKDF2_DATA(i), salt_word);
    }

    return TD_SUCCESS;
}

#define KDF_PADDING_KEY_LEN         128
td_s32 hal_rkp_kdf_set_padding_key(const td_u8 *padding_key, td_u32 key_length)
{
    td_u32 i;
    td_u32 key_word = 0;

    km_null_ptr_chk(padding_key);
    crypto_chk_return(key_length != KDF_PADDING_KEY_LEN, KM_COMPAT_ERRNO(ERROR_INVALID_PARAM),
        "invalid key_length\n");

    for (i = 0; i < key_length / CRYPTO_WORD_WIDTH; i++) {
        key_word = CRYPTO_U8_TO_U32_BIT_SHIFT(padding_key, i);
        km_reg_write(RKP_PBKDF2_KEY(i), key_word);
    }

    return TD_SUCCESS;
}

/*
    pbkdf2_key_config
*/
#define KDF_SW_GEN              3

#define PBKDF2_ALG_SEL_SHA1     1
#define PBKDF2_ALG_SEL_SHA256   0
#define PBKDF2_ALG_SEL_SHA384   3
#define PBKDF2_ALG_SEL_SHA512   4
#define PBKDF2_ALG_SEL_SM3      5
static crypto_table_item g_rkp_alg_sel_table[] = {
    {
        .index = CRYPTO_KDF_SW_ALG_SHA1, .value = PBKDF2_ALG_SEL_SHA1
    },
    {
        .index = CRYPTO_KDF_SW_ALG_SHA256, .value = PBKDF2_ALG_SEL_SHA256
    },
    {
        .index = CRYPTO_KDF_SW_ALG_SHA384, .value = PBKDF2_ALG_SEL_SHA384
    },
    {
        .index = CRYPTO_KDF_SW_ALG_SHA512, .value = PBKDF2_ALG_SEL_SHA512
    },
    {
        .index = CRYPTO_KDF_SW_ALG_SM3, .value = PBKDF2_ALG_SEL_SM3
    },
};

td_s32 hal_rkp_kdf_sw_start(crypto_kdf_sw_alg sw_alg, td_u32 count, td_bool is_wait)
{
    rkp_cmd_cfg cfgval = {0};
    td_u32 alg_reg_val = 0;
    volatile td_s32 ret = TD_FAILURE;

    crypto_unused(is_wait);

    /* get alg_sel. */
    ret = crypto_get_value_by_index(g_rkp_alg_sel_table, crypto_array_size(g_rkp_alg_sel_table),
        sw_alg, &alg_reg_val);
    crypto_chk_return(ret != TD_SUCCESS, ret, "get alg_sel failed\n");

    cfgval.u32 = km_reg_read(RKP_CMD_CFG);
    cfgval.bits.rkp_pbkdf_calc_time = count;
    cfgval.bits.pbkdf2_alg_sel_cfg = alg_reg_val;
    cfgval.bits.pbkdf2_key_sel_cfg = KDF_SW_GEN;
    cfgval.bits.sw_calc_req = 0x1;         /* start calculation */
    km_reg_write(RKP_CMD_CFG, cfgval.u32);

    return TD_SUCCESS;
}

#define RKP_WAIT_TIMEOUT_IN_US          1000000
td_s32 hal_rkp_kdf_wait_done(td_void)
{
    td_u32 i;
    volatile td_s32 ret = TD_FAILURE;
    td_u32 kdf_err = 0;
    rkp_cmd_cfg cmd_cfg = { 0 };

    for (i = 0; i < RKP_WAIT_TIMEOUT_IN_US; ++i) {
        cmd_cfg.u32 = km_reg_read(RKP_CMD_CFG);
        if (cmd_cfg.bits.sw_calc_req == 0x0) {
            km_reg_write(RKP_RAW_INT, 0x1);
            break;
        }
        crypto_udelay(1);
    }
    if (i >= RKP_WAIT_TIMEOUT_IN_US) {
        ret = KM_COMPAT_ERRNO(ERROR_RKP_CALC_TIMEOUT);
    } else {
        ret = TD_SUCCESS;
    }

    /* check kdf err. */
    kdf_err = km_reg_read(KDF_ERROR);
    if (kdf_err != 0) {
        crypto_log_err("kdf_err is 0x%x\n", kdf_err);
        ret = KM_COMPAT_ERRNO(ERROR_KM_LOGIC);
    }

    return ret;
}

td_s32 hal_rkp_deob_wait_done(td_void)
{
    td_u32 i;
    volatile td_s32 ret = TD_FAILURE;
    td_u32 deob_err = 0;
    rkp_deob_cfg deob_cfg = { 0 };

    for (i = 0; i < RKP_WAIT_TIMEOUT_IN_US; ++i) {
        deob_cfg.u32 = km_reg_read(RKP_DEOB_CFG);
        if (deob_cfg.bits.deob_update_req == 0x0) {
            break;
        }
        crypto_udelay(1);
    }
    if (i >= RKP_WAIT_TIMEOUT_IN_US) {
        ret = KM_COMPAT_ERRNO(ERROR_RKP_CALC_TIMEOUT);
    } else {
        ret = TD_SUCCESS;
    }

    /* check deob err. */
    deob_err = km_reg_read(DEOB_ERROR);
    if (deob_err != 0) {
        crypto_log_err("deob_err is 0x%x\n", deob_err);
        ret = KM_COMPAT_ERRNO(ERROR_KM_LOGIC);
    }

    return ret;
}

#define PBKDF2_KEY_SEL_ABRK_REE     20
#define PBKDF2_KEY_SEL_RDRK_REE     22

#define PBKDF2_KEY_SEL_SBRK1        5
#define PBKDF2_KEY_SEL_ABRK1        6
#define PBKDF2_KEY_SEL_ODRK0        7
#define PBKDF2_KEY_SEL_ODRK1        8
#define PBKDF2_KEY_SEL_RDRK         9
#define PBKDF2_KEY_SEL_MDRK0        10
#define PBKDF2_KEY_SEL_FDRK         11
#define PBKDF2_KEY_SEL_MDRK1        12
#define PBKDF2_KEY_SEL_DRK          13
#define PBKDF2_KEY_SEL_MDRK2        14
#define PBKDF2_KEY_SEL_MDRK3        15
#define PBKDF2_KEY_SEL_SBRK2        16
#define PBKDF2_KEY_SEL_ABRK2        17
#define PBKDF2_KEY_SEL_PSK          18

static crypto_table_item g_rkp_key_sel_table[] = {
    {
        .index = CRYPTO_KDF_HARD_KEY_TYPE_ODRK0, .value = PBKDF2_KEY_SEL_ODRK0
    },
    {
        .index = CRYPTO_KDF_HARD_KEY_TYPE_OARK0, .value = PBKDF2_KEY_SEL_ODRK0
    },
    {
        .index = CRYPTO_KDF_HARD_KEY_TYPE_ODRK1, .value = PBKDF2_KEY_SEL_ODRK1
    },
    {
        .index = CRYPTO_KDF_HARD_KEY_TYPE_ABRK_REE, .value = PBKDF2_KEY_SEL_ABRK_REE
    },
    {
        .index = CRYPTO_KDF_HARD_KEY_TYPE_RDRK_REE, .value = PBKDF2_KEY_SEL_RDRK_REE
    },
    {
        .index = CRYPTO_KDF_HARD_KEY_TYPE_SBRK0, .value = PBKDF2_KEY_SEL_SBRK1
    },
    {
        .index = CRYPTO_KDF_HARD_KEY_TYPE_SBRK1, .value = PBKDF2_KEY_SEL_SBRK1
    },
    {
        .index = CRYPTO_KDF_HARD_KEY_TYPE_SBRK2, .value = PBKDF2_KEY_SEL_SBRK2
    },
    {
        .index = CRYPTO_KDF_HARD_KEY_TYPE_ABRK0, .value = PBKDF2_KEY_SEL_ABRK1
    },
    {
        .index = CRYPTO_KDF_HARD_KEY_TYPE_ABRK1, .value = PBKDF2_KEY_SEL_ABRK1
    },
    {
        .index = CRYPTO_KDF_HARD_KEY_TYPE_ABRK2, .value = PBKDF2_KEY_SEL_ABRK2
    },
    {
        .index = CRYPTO_KDF_HARD_KEY_TYPE_DRK0, .value = PBKDF2_KEY_SEL_DRK
    },
    {
        .index = CRYPTO_KDF_HARD_KEY_TYPE_DRK1, .value = PBKDF2_KEY_SEL_DRK
    },
    {
        .index = CRYPTO_KDF_HARD_KEY_TYPE_RDRK0, .value = PBKDF2_KEY_SEL_RDRK
    },
    {
        .index = CRYPTO_KDF_HARD_KEY_TYPE_RDRK1, .value = PBKDF2_KEY_SEL_RDRK
    },
    {
        .index = CRYPTO_KDF_HARD_KEY_TYPE_MDRK0, .value = PBKDF2_KEY_SEL_MDRK0
    },
    {
        .index = CRYPTO_KDF_HARD_KEY_TYPE_MDRK1, .value = PBKDF2_KEY_SEL_MDRK1
    },
    {
        .index = CRYPTO_KDF_HARD_KEY_TYPE_MDRK2, .value = PBKDF2_KEY_SEL_MDRK2
    },
    {
        .index = CRYPTO_KDF_HARD_KEY_TYPE_MDRK3, .value = PBKDF2_KEY_SEL_MDRK3
    },
    {
        .index = CRYPTO_KDF_HARD_KEY_TYPE_PSK, .value = PBKDF2_KEY_SEL_PSK
    }
};

#define PBKDF2_KEY_LEN_128BIT       1
#define PBKDF2_KEY_LEN_192BIT       2
#define PBKDF2_KEY_LEN_256BIT       3
static crypto_table_item g_rkp_key_len_table[] = {
    {
        .index = CRYPTO_KDF_HARD_KEY_SIZE_128BIT, .value = PBKDF2_KEY_LEN_128BIT
    },
    {
        .index = CRYPTO_KDF_HARD_KEY_SIZE_192BIT, .value = PBKDF2_KEY_LEN_192BIT
    },
    {
        .index = CRYPTO_KDF_HARD_KEY_SIZE_256BIT, .value = PBKDF2_KEY_LEN_256BIT
    },
};

#define KDF_HARD_SALT_LEN           28

#if defined(CRYPTO_IOT_V200)
#define PBKDF2_RDRK_REE_ONEWAY_OFFSET   0
#define PBKDF2_ABRK_REE_ONEWAY_OFFSET   1
#define PBKDF2_ODRK1_ONEWAY_OFFSET      2
static crypto_table_item g_rkp_oneway_offset_table[] = {
    {
        .index = CRYPTO_KDF_HARD_KEY_TYPE_RDRK_REE, .value = PBKDF2_RDRK_REE_ONEWAY_OFFSET
    },
    {
        .index = CRYPTO_KDF_HARD_KEY_TYPE_ABRK_REE, .value = PBKDF2_ABRK_REE_ONEWAY_OFFSET
    },
    {
        .index = CRYPTO_KDF_HARD_KEY_TYPE_ODRK1, .value = PBKDF2_ODRK1_ONEWAY_OFFSET
    },
};
#endif

static crypto_table_item g_rkp_hard_alg_sel_table[] = {
    {
        .index = CRYPTO_KDF_HARD_ALG_SHA256, .value = PBKDF2_ALG_SEL_SHA256
    },
    {
        .index = CRYPTO_KDF_HARD_ALG_SM3, .value = PBKDF2_ALG_SEL_SM3
    },
};
td_s32 hal_rkp_kdf_hard_calculation(const crypto_kdf_hard_calc_param *param)
{
    volatile td_s32 ret = TD_FAILURE;
    td_u32 i;
    td_u32 key_sel_reg_val = 0;
    td_u32 alg_reg_val = 0;
    td_u32 key_len_reg_val = 0;
#if defined(CRYPTO_IOT_V200)
    rkp_oneway_ree onewayval = {0};
    td_u32 oneway_offset = 0;
#endif
    rkp_cmd_cfg cfgval = {0};
    td_u32 salt_word = 0;

    km_null_ptr_chk(param);
    if (param->salt != TD_NULL) {
        crypto_chk_return(param->salt_length != KDF_HARD_SALT_LEN, KM_COMPAT_ERRNO(ERROR_INVALID_PARAM),
            "invalid param->salt_length\n");
    }
    crypto_chk_return(param->hard_alg != CRYPTO_KDF_HARD_ALG_SHA256 && param->hard_alg != CRYPTO_KDF_HARD_ALG_SM3,
        KM_COMPAT_ERRNO(ERROR_INVALID_PARAM), "invalid param->hard->alg\n");

    /* get key_sel. */
    ret = crypto_get_value_by_index(g_rkp_key_sel_table, crypto_array_size(g_rkp_key_sel_table),
        param->hard_key_type, &key_sel_reg_val);
    crypto_chk_return(ret != TD_SUCCESS, ret, "get key_sel failed\n");

    /* get alg_sel. */
    ret = crypto_get_value_by_index(g_rkp_hard_alg_sel_table, crypto_array_size(g_rkp_hard_alg_sel_table),
        param->hard_alg, &alg_reg_val);
    crypto_chk_return(ret != TD_SUCCESS, ret, "get alg_sel failed\n");

    /* get key_len. */
    ret = crypto_get_value_by_index(g_rkp_key_len_table, crypto_array_size(g_rkp_key_len_table),
        param->hard_key_size, &key_len_reg_val);
    crypto_chk_return(ret != TD_SUCCESS, ret, "get key_len failed\n");

#if defined(CRYPTO_IOT_V200)
    /* get oneway_offset. */
    ret = crypto_get_value_by_index(g_rkp_oneway_offset_table, crypto_array_size(g_rkp_oneway_offset_table),
        param->hard_key_type, &oneway_offset);
    crypto_chk_return(ret != TD_SUCCESS, ret, "get oneway_offset failed\n");

    /* config oneway. */
    onewayval.u32 = km_reg_read(RKP_ONEWAY);
    onewayval.u32 |= (param->is_oneway << oneway_offset);
    km_reg_write(RKP_ONEWAY, onewayval.u32);
#endif
    /* config salt. */
    if (param->salt != TD_NULL) {
        for (i = 0; i < param->salt_length / CRYPTO_WORD_WIDTH; i++) {
            salt_word = CRYPTO_U8_TO_U32_BIT_SHIFT(param->salt, i);
            km_reg_write(RKP_SALT(i), salt_word);
        }
    }
    /* config sw_cfg for MDRK0 MDRK1 MDRK2 MDRK3 */
    if (param->hard_key_type == CRYPTO_KDF_HARD_KEY_TYPE_MDRK0 ||
        param->hard_key_type == CRYPTO_KDF_HARD_KEY_TYPE_MDRK1 ||
        param->hard_key_type == CRYPTO_KDF_HARD_KEY_TYPE_MDRK2 ||
        param->hard_key_type == CRYPTO_KDF_HARD_KEY_TYPE_MDRK3) {
            km_reg_write(SW_CFG, param->rkp_sw_cfg);
        }
    /* config rkp_cmd_cfg. */
    cfgval.u32 = km_reg_read(RKP_CMD_CFG);
    cfgval.bits.pbkdf2_key_len = key_len_reg_val;
    cfgval.bits.pbkdf2_alg_sel_cfg = alg_reg_val;
    cfgval.bits.pbkdf2_key_sel_cfg = key_sel_reg_val;

    cfgval.bits.sw_calc_req = 0x1;         /* start kdf calculation */
    km_reg_write(RKP_CMD_CFG, cfgval.u32);

    ret = hal_rkp_kdf_wait_done();
    crypto_chk_return(ret != TD_SUCCESS, ret, "hal_rkp_kdf_wait_done failed\n");

    return ret;
}

td_s32 hal_rkp_clear_reg_key(td_void)
{
    td_u32 i = 0;
    td_u32 clear_key_val = 0;

    for (i = 0; i < KDF_PADDING_KEY_LEN / CRYPTO_WORD_WIDTH; i++) {
        km_reg_write(RKP_PBKDF2_KEY(i), clear_key_val);
    }

    for (i = 0; i < KDF_PADDING_SALT_LEN / CRYPTO_WORD_WIDTH; i++) {
        km_reg_write(RKP_PBKDF2_DATA(i), clear_key_val);
    }

    for (i = 0; i < KDF_PADDING_VAL_LEN / CRYPTO_WORD_WIDTH; i++) {
        km_reg_write(RKP_PBKDF2_VAL(i), clear_key_val);
    }
    return TD_SUCCESS;
}

td_void hal_rkp_debug(td_void)
{
    td_u32 i;
    td_u32 reg_value = 0;
    rkp_cmd_cfg cmd_cfg;
    /* RKP_LOCK. */
    reg_value = km_reg_read(RKP_LOCK);
    if (reg_value == RKP_LOCK_CPU_REE) {
        crypto_print("RKP locked by REE CPU!\r\n");
    } else if (reg_value == RKP_LOCK_CPU_TEE) {
        crypto_print("RKP locked by TEE CPU!\r\n");
    }

    /* RKP_CMD_CFG. */
    cmd_cfg.u32 = km_reg_read(RKP_CMD_CFG);
    crypto_print("RKP_CMD_CFG: sw_calc_req is 0x%x\r\n", cmd_cfg.bits.sw_calc_req);
    crypto_print("RKP_CMD_CFG: pbkdf2_alg_sel_cfg is 0x%x\r\n", cmd_cfg.bits.pbkdf2_alg_sel_cfg);
    crypto_print("RKP_CMD_CFG: pbkdf2_key_sel_cfg is 0x%x\r\n", cmd_cfg.bits.pbkdf2_key_sel_cfg);
    crypto_print("RKP_CMD_CFG: pbkdf2_key_len is 0x%x\r\n", cmd_cfg.bits.pbkdf2_key_len);
    crypto_print("RKP_CMD_CFG: rkp_pbkdf_calc_time is 0x%x\r\n", cmd_cfg.bits.rkp_pbkdf_calc_time);

    /* KDF_ERROR. */
    crypto_print("KDF_ERROR is 0x%x\r\n", km_reg_read(KDF_ERROR));
    /* RKP_RAW_INT */
    crypto_print("RKP_RAW_INT is 0x%x\r\n", km_reg_read(RKP_RAW_INT));
    /* RKP_INT_ENABLE */
    crypto_print("RKP_INT_ENABLE is 0x%x\r\n", km_reg_read(RKP_INT_ENABLE));
    /* RKP_INT */
    crypto_print("RKP_INT is 0x%x\r\n", km_reg_read(RKP_INT));
    /* RKP_DEOB_CFG */
    crypto_print("RKP_DEOB_CFG is 0x%x\r\n", km_reg_read(RKP_DEOB_CFG));
    /* DEOB_ERROR */
    crypto_print("DEOB_ERROR is 0x%x\r\n", km_reg_read(DEOB_ERROR));
    /* RK_RDY */
    crypto_print("RK_RDY is 0x%x\r\n", km_reg_read(RK_RDY));
    /* RKP_SALT */
    for (i = 0; i < KDF_HARD_SALT_LEN / CRYPTO_WORD_WIDTH; i++) {
        crypto_print("RK_RDY is 0x%x\r\n", km_reg_read(RKP_SALT(i)));
    }
    /* RKP_ONEWAY */
    crypto_print("RKP_ONEWAY is 0x%x\r\n", km_reg_read(RKP_ONEWAY));
}