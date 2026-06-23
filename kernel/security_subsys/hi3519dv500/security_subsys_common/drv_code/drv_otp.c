/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "drv_otp.h"

#include "crypto_osal_lib.h"
#include "otpc_register.h"
#include "ca_misc_register.h"
#include "crypto_osal_adapt.h"
#include "crypto_common_macro.h"

#define OTP_READ_DELAY_TIME_IN_US 1
#define OTP_WRITE_DELAY_TIME_IN_US 1
#define OTP_NON_SPECIAL_VAL 0xA5C36987

#define OTP_TIMEOUT                 1000000
#define OTP_MEMORY_MAX_ADDR         0xfff
#define OTP_ADDR_4BYTES_ALIGN       4
#define OTP_DIE_ID_OFFSET           0xf0
#define OTP_DIE_ID_LEN              16
#define OTP_WORD_LEN                4

#define OTP_COMPAT_ERRNO(err_code)      DRV_COMPAT_ERRNO(ERROR_MODULE_OTP, err_code)

td_s32 otp_lock_on(void);
void otp_unlock_off(void);

td_s32 otp_lock_on(void)
{
    otp_lock lock_ctrl = {.u32 = OTP_NON_SPECIAL_VAL};
    td_u32 lock_status = OTP_NON_SPECIAL_VAL;
    td_u32 lock_code = ca_misc_reg_read(CPU_ID_STAT);
    td_u32 i = 0;

    /* lock otp */
    for (i = 0; i < OTP_LOCK_TIMEOUT; i++) {
        /* otp unlock ? */
        lock_status = otpc_reg_read(OTP_LOCK_STATUS);
        if (lock_status != OTP_LOCK_UNLOCK) {
            crypto_udelay(OTP_READ_DELAY_TIME_IN_US);
            continue;
        }

        /* try to lock otp */
        lock_ctrl.u32 = otpc_reg_read(OTP_LOCK);
        lock_ctrl.bits.otp_lock_type = OTP_LOCK_TYPE_LOCK; /* lock command */
        lock_ctrl.bits.otp_lock = OTP_LOCK_CMD;
        otpc_reg_write(OTP_LOCK, lock_ctrl.u32);

        /* check lock result */
        lock_status = otpc_reg_read(OTP_LOCK_STATUS);
        if (lock_status == lock_code) {
            break;
        }
    }

    if (i >= OTP_LOCK_TIMEOUT) {
        crypto_log_err("otp lock timeout\n");
        return OTP_COMPAT_ERRNO(ERROR_CHN_BUSY);
    }

    return TD_SUCCESS;
}

void otp_unlock_off(void)
{
    otp_lock lock_ctrl = {.u32 = OTP_NON_SPECIAL_VAL};

    /* unlock otp */
    lock_ctrl.u32 = otpc_reg_read(OTP_LOCK);
    lock_ctrl.bits.otp_lock_type = OTP_LOCK_TYPE_UNLOCK; /* unlock command */
    lock_ctrl.bits.otp_lock = OTP_LOCK_CMD;
    otpc_reg_write(OTP_LOCK, lock_ctrl.u32);
}

td_s32 drv_otp_read_word(const td_u32 addr, td_u32 *data)
{
    otp_rw_ctrl_u un_otp_rw_ctrl = {.u32 = OTP_NON_SPECIAL_VAL};
    otp_status otp_status_reg = {.u32 = OTP_NON_SPECIAL_VAL};
    td_u32 time_out = 0;
    volatile td_s32 ret = TD_FAILURE;

    crypto_param_check(addr > OTP_MEMORY_MAX_ADDR);
    crypto_param_check(data == TD_NULL);
    crypto_chk_return((addr % OTP_ADDR_4BYTES_ALIGN) != 0, OTP_COMPAT_ERRNO(ERROR_OTP_ADDR_NOT_ALIGNED));

    ret = otp_lock_on();
    if (ret != TD_SUCCESS) {
        return ret;
    }

    // 1.Configure Address OTP_ADDR
    otpc_reg_write(OTP_ADDR, addr);
    crypto_chk_print(otpc_reg_read(OTP_ADDR) != addr, "%s:%d: check equal failed\n", __func__, __LINE__);

    // 2.Configure OTP_RW_CTRL
    un_otp_rw_ctrl.u32 = otpc_reg_read(OTP_RW_CTRL);
#ifdef EFUSE_OTP
    un_otp_rw_ctrl.bits.wr_sel = OTP_WR_SEL_TSMC_EFUSE_NORMAL_RD;
#else
    un_otp_rw_ctrl.bits.wr_sel = OTP_WR_SEL_OTP_RD;
#endif
    un_otp_rw_ctrl.bits.start = OTP_START_BUSY;               // 1'b1 means busy.
    un_otp_rw_ctrl.bits.test_mode = OTP_TEST_MODE_NORMAL;     // 3'b000 means normal mode;
    otpc_reg_write(OTP_RW_CTRL, un_otp_rw_ctrl.u32);

    // 3.Wait idle
    while (time_out++ < OTP_TIMEOUT) {
        if ((otpc_reg_read(OTP_RW_CTRL) & 0x1) == OTP_START_IDLE) {
            break;
        }
        crypto_udelay(OTP_READ_DELAY_TIME_IN_US);
    }
    if (time_out >= OTP_TIMEOUT) {
        crypto_log_err("err:otp time out on read!\n");
        otp_unlock_off();
        return OTP_COMPAT_ERRNO(ERROR_OTP_TIMEOUT);
    }

    // 4.Check OTP_STATUS.
    otp_status_reg.u32 = otpc_reg_read(OTP_STATUS);
    if (otp_status_reg.bits.check_fail == OTP_CHECK_FAIL) { // 0x0 means pass; 0x1 means fail.
        crypto_log_err("\nerr: OTP read fail\n");
        otp_unlock_off();
        return OTP_COMPAT_ERRNO(ERROR_OTP_PERMISSION);
    }

    // 5.Read Data
    *data = otpc_reg_read(OTP_RDATA);

    otp_unlock_off();
    return TD_SUCCESS;
}

td_s32 drv_otp_read_byte(const td_u32 addr, td_u8 *data)
{
    td_u32 read_data = 0;
    volatile td_s32 ret = TD_FAILURE;
    td_u32 align_addr = OTP_NON_SPECIAL_VAL;

    crypto_drv_func_enter();

    crypto_param_check(addr > OTP_MEMORY_MAX_ADDR);
    crypto_param_check(data == TD_NULL);

    align_addr = addr - (addr % OTP_ADDR_4BYTES_ALIGN);

    ret = drv_otp_read_word(align_addr, &read_data);
    if (ret != TD_SUCCESS) {
        crypto_log_err("\nerr: OTP read word fail\n");
        return ret;
    }

    *data = (td_u8)(read_data >> (8 * (addr % OTP_ADDR_4BYTES_ALIGN))); // 8 bits one byte

    crypto_drv_func_exit();
    return TD_SUCCESS;
}

td_s32 drv_otp_write_byte(const td_u32 addr, const td_u8 data)
{
    otp_rw_ctrl_u un_otp_rw_ctrl = {.u32 = OTP_NON_SPECIAL_VAL};
    otp_status otp_status_reg = {.u32 = OTP_NON_SPECIAL_VAL};
    td_u32 time_out = 0;
    volatile td_s32 ret = TD_FAILURE;

    crypto_param_check(addr > OTP_MEMORY_MAX_ADDR);

    if (data == 0) { // Default value is 0, no need and no necessary to write 0 directly
        return TD_SUCCESS;
    }

    ret = otp_lock_on();
    if (ret != TD_SUCCESS) {
        return ret;
    }

    // 1.Configure Address OTP_ADDR,
    otpc_reg_write(OTP_ADDR, addr);
    crypto_chk_print(otpc_reg_read(OTP_ADDR) != addr, "%s:%d: check equal failed\n", __func__, __LINE__);
    // 2.OTP_RW_ST_1 Check:(1)Readback Address (2)whether write is successful or not

    // 3.Write Data
    otpc_reg_write(OTP_WDATA, data);
    crypto_chk_print(otpc_reg_read(OTP_WDATA) != data, "%s:%d: check equal failed\n", __func__, __LINE__);

    // 4.Configure OTP_RW_CTRL
    un_otp_rw_ctrl.u32 = otpc_reg_read(OTP_RW_CTRL);
    un_otp_rw_ctrl.bits.wr_sel = OTP_WR_SEL_PGM_ACCESS;         // (3'b010) 0x2 means pgm access;
    un_otp_rw_ctrl.bits.start = OTP_START_BUSY;                 // (1'b1)   0x1 means busy. 0x0: idle.
    un_otp_rw_ctrl.bits.test_mode = OTP_TEST_MODE_NORMAL;       // (3'b000) 0x0 means normal mode;
    otpc_reg_write(OTP_RW_CTRL, un_otp_rw_ctrl.u32);

    // 5.Wait idle
    while (time_out++ < OTP_TIMEOUT) {
        un_otp_rw_ctrl.u32 = otpc_reg_read(OTP_RW_CTRL);
        if (un_otp_rw_ctrl.bits.start == OTP_START_IDLE) { // 0x0: idle.
            break;
        }
        crypto_udelay(OTP_WRITE_DELAY_TIME_IN_US);
    }
    if (time_out >= OTP_TIMEOUT) {
        crypto_log_err("err: OTP write time out!\n");
        otp_unlock_off();
        return OTP_COMPAT_ERRNO(ERROR_OTP_TIMEOUT);
    }

    // 6.Check OTP_STATUS.
    otp_status_reg.u32 = otpc_reg_read(OTP_STATUS);
    if (otp_status_reg.bits.check_fail == OTP_CHECK_PASS) { // 0x0 means pass; 0x1 means fail.
        otp_unlock_off();
        return TD_SUCCESS;
    } else {
        crypto_log_err("err: OTP write fail\n");
        otp_unlock_off();
        return OTP_COMPAT_ERRNO(ERROR_OTP_PERMISSION);
    }
}

td_s32 drv_otp_get_die_id(td_u8 *die_id, const td_u32 len)
{
    volatile td_s32 ret = TD_FAILURE;
    td_u32 addr = OTP_DIE_ID_OFFSET;
    td_u32 word_data = 0;
    td_u32 i = 0;

    crypto_chk_return(len < OTP_DIE_ID_LEN, OTP_COMPAT_ERRNO(ERROR_INVALID_PARAM), "len < OTP_DIE_ID_LEN\n");

    for (i = 0; i < OTP_DIE_ID_LEN; i += OTP_WORD_LEN) {
        ret = drv_otp_read_word(addr, &word_data);
        if (ret != TD_SUCCESS) {
            crypto_log_err("\nerr: OTP read word fail\n");
            return ret;
        }
        (void)memcpy_s(die_id, len, &word_data, OTP_WORD_LEN);
        addr += OTP_WORD_LEN;
        die_id += OTP_WORD_LEN;
    }

    crypto_chk_print(i != OTP_DIE_ID_LEN, "i == OTP_DIE_ID_LEN check equal failed\n");

    return TD_SUCCESS;
}