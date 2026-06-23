/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef CRYPTO_OTP_INIT_H
#define CRYPTO_OTP_INIT_H

#include "ot_type.h"

#define UMAP_OTP_MINOR_BASE         57

/* OTP. */
td_s32 crypto_otp_init(td_void);
td_void crypto_otp_deinit(td_void);

#endif