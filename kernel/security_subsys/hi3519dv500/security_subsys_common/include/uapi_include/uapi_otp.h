/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef UAPI_OTP_H
#define UAPI_OTP_H

#include "ot_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

td_s32 uapi_otp_init(td_void);

td_s32 uapi_otp_deinit(td_void);

td_s32 uapi_otp_read_word(td_u32 offset, td_u32 *data);

td_s32 uapi_otp_read_byte(td_u32 offset, td_u8 *data);

td_s32 uapi_otp_write_byte(td_u32 offset, td_u8 data);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif