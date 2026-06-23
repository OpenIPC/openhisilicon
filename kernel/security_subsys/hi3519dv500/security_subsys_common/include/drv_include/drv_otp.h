/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef DRV_OTP_API_H
#define DRV_OTP_API_H

#include "ot_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif  /* __cplusplus */

/**
\brief  otp word read.
\attention \n
N/A

\param[in] addr: read address(0x00 ~ 0x1ff), requires 4 bytes align.
\param[out] data: read word buffer(data is put in little endian).
\param[in] check_word: the checksum of the parameters.
\retval ::TD_SUCCESS  Call this API succussful.
\retval ::TD_FAILURE  Call this API fails.

\see \n
N/A
*/
td_s32 drv_otp_read_word(const td_u32 addr, td_u32 *data);

/**
\brief  otp byte read.
\attention \n
N/A

\param[in] addr: read address(0x00 ~ 0x1ff).
\param[out] data: read byte buffer(data is put in little endian).
\param[in] chech_word: the checksum of the parameters.
\retval ::TD_SUCCESS  Call this API succussful.
\retval ::TD_FAILURE  Call this API fails.

\see \n
N/A
*/
td_s32 drv_otp_read_byte(const td_u32 addr, td_u8 *data);

/**
\brief  otp byte write.
\attention \n
N/A

\param[in] addr: write address(0x00 ~ 0x1ff).
\param[in] data: write data.
\param[in] chech_word: the checksum of the parameters.
\retval ::TD_SUCCESS  Call this API succussful.
\retval ::TD_FAILURE  Call this API fails.

\see \n
N/A
*/
td_s32 drv_otp_write_byte(const td_u32 addr, const td_u8 data);

td_s32 drv_otp_get_die_id(td_u8 *die_id, const td_u32 len);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
