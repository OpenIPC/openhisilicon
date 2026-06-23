/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "hal_keyslot.h"
#include "hal_keyslot_reg.h"
#include "crypto_drv_common.h"
#include "crypto_common_macro.h"

#define KEYSLOT_TYPE_MCIPHER    0
#define KEYSLOT_TYPE_HMAC       1

#define KM_COMPAT_ERRNO(err_code)     HAL_COMPAT_ERRNO(ERROR_MODULE_KM, err_code)
#define km_null_ptr_chk(ptr)   \
    crypto_chk_return((ptr) == TD_NULL, KM_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), #ptr" is NULL\n")

static td_bool inner_kslot_chn_is_locked(td_u32 keyslot_num, td_u32 keyslot_type_reg_value, td_u32 *locked_cpu)
{
    kc_rd_slot_num slot = {0};
    kc_rd_lock_status stat = {0};

    slot.u32 = km_reg_read(KC_RD_SLOT_NUM);
    slot.bits.slot_cfg_type = keyslot_type_reg_value; /* sym or hash */
    slot.bits.slot_num_cfg = keyslot_num;
    km_reg_write(KC_RD_SLOT_NUM, slot.u32);

    stat.u32 = km_reg_read(KC_RD_LOCK_STATUS);
    if (locked_cpu != TD_NULL) {
        *locked_cpu = stat.bits.rd_lock_status;
    }
    if (stat.bits.rd_lock_status) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

static td_bool inner_kslot_is_busy(td_void)
{
    kc_flush_busy kc_busy = {0};
    crypto_cpu_type cpu_type = crypto_get_cpu_type();
    switch (cpu_type) {
        case CRYPTO_CPU_TYPE_ACPU:
            kc_busy.u32 = km_reg_read(KC_REECPU_FLUSH_BUSY);
            break;
        case CRYPTO_CPU_TYPE_SCPU:
            kc_busy.u32 = km_reg_read(KC_TEECPU_FLUSH_BUSY);
            break;
        default:
            crypto_log_err("invalid cpu type\n");
            return TD_TRUE;
    }
    if (kc_busy.bits.flush_busy) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

static td_bool inner_kslot_is_unlock_failed(td_void)
{
    kc_flush_busy kc_busy = {0};
    crypto_cpu_type cpu_type = crypto_get_cpu_type();
    switch (cpu_type) {
        case CRYPTO_CPU_TYPE_ACPU:
            kc_busy.u32 = km_reg_read(KC_REECPU_FLUSH_BUSY);
            break;
        case CRYPTO_CPU_TYPE_SCPU:
            kc_busy.u32 = km_reg_read(KC_TEECPU_FLUSH_BUSY);
            break;
        default:
            crypto_log_err("invalid cpu type\n");
            return TD_TRUE;
    }
    if (kc_busy.bits.flush_unlock_fail) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

#define KSLOT_TIMEOUT           1000000
static td_s32 inner_kslot_wait_idle(void)
{
    td_u32 time_out = KSLOT_TIMEOUT;

    while (time_out) {
        if (inner_kslot_is_busy() == TD_FALSE) {
            break;
        }
        crypto_udelay(1);
        --time_out;
    }
    if (time_out == 0) {
        return KM_COMPAT_ERRNO(ERROR_KEYSLOT_TIMEOUT);
    }

    return TD_SUCCESS;
}

#define KS_STAT_UN_LOCK     0x0
#define KS_STAT_REE_LOCK    0x1
#define KS_STAT_TEE_LOCK    0x2

#define KS_CPU_LOCK         1
#define KS_CPU_UNLOCK       0
td_s32 hal_keyslot_lock(td_u32 keyslot_num, crypto_keyslot_type keyslot_type)
{
    td_u32 keyslot_type_reg_value = 0;
    crypto_cpu_type cpu_type = crypto_get_cpu_type();
    td_u32 cpu_reg_addr = KC_REECPU_LOCK_CMD;       /* default is ree cpu. */
    td_u32 lock_state_expected = KS_STAT_REE_LOCK;  /* default is ree cpu. */
    td_u32 lock_state = 0;
    kc_cpu_lock_cmd lock_cmd = {0};

    switch (keyslot_type) {
        case CRYPTO_KEYSLOT_TYPE_MCIPHER:
            keyslot_type_reg_value = KEYSLOT_TYPE_MCIPHER;
            break;
        case CRYPTO_KEYSLOT_TYPE_HMAC:
            keyslot_type_reg_value = KEYSLOT_TYPE_HMAC;
            break;
        default:
            crypto_log_err("invalid keyslot_type\n");
            return KM_COMPAT_ERRNO(ERROR_INVALID_PARAM);
    }

    if (inner_kslot_chn_is_locked(keyslot_num, keyslot_type_reg_value, TD_NULL)) {
        return KM_COMPAT_ERRNO(ERROR_KEYSLOT_LOCK);
    }

    if (inner_kslot_is_busy()) {
        return KM_COMPAT_ERRNO(ERROR_CHN_BUSY);
    }

    if (cpu_type == CRYPTO_CPU_TYPE_SCPU) {
        lock_state_expected = KS_STAT_TEE_LOCK;
        cpu_reg_addr = KC_TEECPU_LOCK_CMD;
    }

    lock_cmd.u32 = km_reg_read(cpu_reg_addr);
    lock_cmd.bits.flush_hmac_kslot_ind = keyslot_type_reg_value;
    lock_cmd.bits.key_slot_num = keyslot_num;
    lock_cmd.bits.lock_cmd = KS_CPU_LOCK;         /* start lock and unlock */
    km_reg_write(cpu_reg_addr, lock_cmd.u32);

    if (inner_kslot_chn_is_locked(keyslot_num, keyslot_type_reg_value, &lock_state) == TD_FALSE) {
        crypto_log_err("lock failed\n");
        return KM_COMPAT_ERRNO(ERROR_KEYSLOT_LOCK);
    }
    if (lock_state != lock_state_expected) {
        crypto_log_err("lock failed\n");
        return KM_COMPAT_ERRNO(ERROR_KEYSLOT_LOCK);
    }
    return TD_SUCCESS;
}

td_s32 hal_keyslot_unlock(td_u32 keyslot_num, crypto_keyslot_type keyslot_type)
{
    volatile td_s32 ret = TD_FAILURE;
    td_u32 keyslot_type_reg_value = 0;
    crypto_cpu_type cpu_type = crypto_get_cpu_type();
    td_u32 cpu_reg_addr = 0;
    kc_cpu_lock_cmd lock_cmd = {0};

    switch (keyslot_type) {
        case CRYPTO_KEYSLOT_TYPE_MCIPHER:
            keyslot_type_reg_value = KEYSLOT_TYPE_MCIPHER;
            break;
        case CRYPTO_KEYSLOT_TYPE_HMAC:
            keyslot_type_reg_value = KEYSLOT_TYPE_HMAC;
            break;
        default:
            crypto_log_err("invalid keyslot_type\n");
            return KM_COMPAT_ERRNO(ERROR_INVALID_PARAM);
    }

    if (inner_kslot_is_busy()) {
        return KM_COMPAT_ERRNO(ERROR_CHN_BUSY);
    }
    if (inner_kslot_chn_is_locked(keyslot_num, keyslot_type_reg_value, TD_NULL) == TD_FALSE) {
        return TD_SUCCESS;
    }

    switch (cpu_type) {
        case CRYPTO_CPU_TYPE_SCPU:
            cpu_reg_addr = KC_TEECPU_LOCK_CMD;
            break;
        case CRYPTO_CPU_TYPE_ACPU:
            cpu_reg_addr = KC_REECPU_LOCK_CMD;
            break;
        default:
            crypto_log_err("invalid cpu type\n");
            return KM_COMPAT_ERRNO(ERROR_INVALID_CPU_TYPE);
    }

    lock_cmd.u32 = km_reg_read(cpu_reg_addr);
    lock_cmd.bits.flush_hmac_kslot_ind = keyslot_type_reg_value;
    lock_cmd.bits.key_slot_num = keyslot_num;
    lock_cmd.bits.lock_cmd = KS_CPU_UNLOCK;         /* start lock and unlock */
    km_reg_write(cpu_reg_addr, lock_cmd.u32);

    ret = inner_kslot_wait_idle();
    crypto_chk_return(ret != TD_SUCCESS, ret, "inner_kslot_wait_idle failed\n");

    if (inner_kslot_is_unlock_failed() == TD_TRUE) {
        crypto_log_err("kslot unlock failed\n");
        return KM_COMPAT_ERRNO(ERROR_KEYSLOT_LOCK);
    }
    return TD_SUCCESS;
}