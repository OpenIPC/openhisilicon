/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "dispatch_otp.h"
#include "crypto_osal_lib.h"
#include "crypto_common_macro.h"
#include "kapi_otp.h"

static td_s32 dispatch_otp_read_word(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    otp_word_ctl_t *otp_word_ctl = TD_NULL;
    crypto_dispatch_func_enter();

    crypto_unused(cmd);
    crypto_unused(private_data);

    otp_word_ctl = (otp_word_ctl_t *)argp;

    ret = kapi_otp_read_word(otp_word_ctl->addr, &otp_word_ctl->word);
    crypto_chk_return(ret != TD_SUCCESS, ret, "kapi_otp_read_word failed, ret is 0x%x\n", ret);

    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_otp_read_byte(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    otp_byte_ctl_t *otp_byte_ctl = TD_NULL;
    crypto_dispatch_func_enter();

    crypto_unused(cmd);
    crypto_unused(private_data);

    otp_byte_ctl = (otp_byte_ctl_t *)argp;

#ifdef CONFIG_DISPATCH_DEBUG
    crypto_print("addr is 0x%x\n", otp_byte_ctl->addr);
    crypto_print("byte is 0x%x\n", &otp_byte_ctl->byte);
#endif

    ret = kapi_otp_read_byte(otp_byte_ctl->addr, &otp_byte_ctl->byte);
    crypto_chk_return(ret != TD_SUCCESS, ret, "kapi_otp_read_byte failed, ret is 0x%x\n", ret);

    crypto_dispatch_func_exit();
    return ret;
}

static td_s32 dispatch_otp_write_byte(unsigned int cmd, td_void *argp, void *private_data)
{
    td_s32 ret = TD_SUCCESS;
    otp_byte_ctl_t *otp_byte_ctl = TD_NULL;
    crypto_dispatch_func_enter();

    crypto_unused(cmd);
    crypto_unused(private_data);

    otp_byte_ctl = (otp_byte_ctl_t *)argp;

    ret = kapi_otp_write_byte(otp_byte_ctl->addr, otp_byte_ctl->byte);
    crypto_chk_return(ret != TD_SUCCESS, ret, "kapi_otp_write_byte failed, ret is 0x%x\n", ret);

    crypto_dispatch_func_exit();
    return ret;
}

static crypto_ioctl_cmd g_otp_func_list[] = {
    {CMD_OTP_READ_WORD,                 dispatch_otp_read_word},
    {CMD_OTP_READ_BYTE,                 dispatch_otp_read_byte},
    {CMD_OTP_WRITE_BYTE,                dispatch_otp_write_byte},
};

crypto_ioctl_cmd *get_otp_func_list(td_void)
{
    return g_otp_func_list;
}

td_u32 get_otp_func_num(td_void)
{
    return crypto_array_size(g_otp_func_list);
}
