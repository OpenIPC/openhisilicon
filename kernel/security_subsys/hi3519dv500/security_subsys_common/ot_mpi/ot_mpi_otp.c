/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "ot_mpi_otp.h"
#include "uapi_otp.h"

td_s32 ot_mpi_otp_init(td_void)
{
    return uapi_otp_init();
}

td_s32 ot_mpi_otp_deinit(td_void)
{
    return uapi_otp_deinit();
}

td_s32 ot_mpi_otp_read_word(td_u32 offset, td_u32 *data)
{
    return uapi_otp_read_word(offset, data);
}

td_s32 ot_mpi_otp_read_byte(td_u32 offset, td_u8 *data)
{
    return uapi_otp_read_byte(offset, data);
}

td_s32 ot_mpi_otp_write_byte(td_u32 offset, td_u8 data)
{
    return uapi_otp_write_byte(offset, data);
}