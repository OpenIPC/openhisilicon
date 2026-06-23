/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "uapi_otp.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <securec.h>

#include "ot_type.h"
#include "crypto_errno.h"
#include "crypto_osal_user_lib.h"
#include "crypto_common_macro.h"
#include "ioctl_otp.h"

#define REF_COUNT_MAX_NUM     0x7FFFFFFF

typedef struct mpi_mgnt {
    crypto_mutex_t      lock;
    td_s32              ref_count;
    td_s32              dev_fd;
} mpi_mgnt_t;

static mpi_mgnt_t otp_mgnt = {
    .lock = CRYPTO_PTHREAD_MUTEX_INITIALIZER,
    .ref_count = -1,
    .dev_fd = -1
};

td_s32 uapi_otp_init(td_void)
{
    td_s32 ret = TD_SUCCESS;
    crypto_uapi_func_enter();
    crypto_pthread_mutex_lock(&otp_mgnt.lock);

    if (otp_mgnt.ref_count >= 0) {
        if (otp_mgnt.ref_count < REF_COUNT_MAX_NUM) {
            otp_mgnt.ref_count++;
            goto exit;
        }
    }
    otp_mgnt.dev_fd = crypto_open("/dev/otp", O_RDWR, 0);
    if (otp_mgnt.dev_fd < 0) {
        printf("open /dev/otp failed\n");
        ret = TD_FAILURE;
        goto exit;
    }

    otp_mgnt.ref_count = 0;

exit:
    crypto_pthread_mutex_unlock(&otp_mgnt.lock);
    crypto_uapi_func_exit();
    return ret;
}

td_s32 uapi_otp_deinit(td_void)
{
    td_s32 ret = TD_SUCCESS;
    crypto_pthread_mutex_lock(&otp_mgnt.lock);

    if (otp_mgnt.ref_count > 0) {
        otp_mgnt.ref_count--;
        goto exit;
    }

    if (otp_mgnt.ref_count == 0) {
        otp_mgnt.ref_count--;
        crypto_close(otp_mgnt.dev_fd);
        otp_mgnt.dev_fd = -1;
    }

exit:
    crypto_pthread_mutex_unlock(&otp_mgnt.lock);
    return ret;
}

td_s32 uapi_otp_read_word(td_u32 offset, td_u32 *data)
{
    td_s32 ret = TD_SUCCESS;
    otp_word_ctl_t word_ctl = {0};

    crypto_uapi_func_enter();
    crypto_param_require(data != TD_NULL);

    word_ctl.addr = offset;
    ret = crypto_otp_ioctl(otp_mgnt.dev_fd, CMD_OTP_READ_WORD, &word_ctl);
    crypto_chk_goto(ret != TD_SUCCESS, exit, "crypto_otp_ioctl failed, ret is 0x%x\n", ret);

    *data = word_ctl.word;
    crypto_uapi_func_exit();

exit:
    (td_void)memset_s(&word_ctl, sizeof(word_ctl), 0, sizeof(word_ctl));
    return ret;
}

td_s32 uapi_otp_read_byte(td_u32 offset, td_u8 *data)
{
    td_s32 ret = TD_SUCCESS;
    otp_byte_ctl_t byte_ctl = {0};

    crypto_uapi_func_enter();
    crypto_param_require(data != TD_NULL);

    byte_ctl.addr = offset;
    ret = crypto_otp_ioctl(otp_mgnt.dev_fd, CMD_OTP_READ_BYTE, &byte_ctl);
    crypto_chk_goto(ret != TD_SUCCESS, exit, "crypto_otp_ioctl failed, ret is 0x%x\n", ret);

    *data = byte_ctl.byte;

    crypto_uapi_func_exit();
exit:
    (td_void)memset_s(&byte_ctl, sizeof(otp_byte_ctl_t), 0, sizeof(otp_byte_ctl_t));
    return ret;
}

td_s32 uapi_otp_write_byte(td_u32 offset, td_u8 data)
{
    td_s32 ret = TD_SUCCESS;
    otp_byte_ctl_t byte_ctl = {0};

    crypto_uapi_func_enter();

    byte_ctl.addr = offset;
    byte_ctl.byte = data;
    ret = crypto_otp_ioctl(otp_mgnt.dev_fd, CMD_OTP_WRITE_BYTE, &byte_ctl);
    crypto_chk_goto(ret != TD_SUCCESS, exit, "crypto_otp_ioctl failed, ret is 0x%x\n", ret);

    crypto_uapi_func_exit();
exit:
    (td_void)memset_s(&byte_ctl, sizeof(byte_ctl), 0, sizeof(byte_ctl));
    return ret;
}