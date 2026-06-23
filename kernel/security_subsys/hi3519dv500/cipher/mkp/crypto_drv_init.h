/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef CRYPTO_INIT_H
#define CRYPTO_INIT_H

#include "crypto_type.h"

#define UMAP_SPACC_MINOR_BASE       53
#define UMAP_KM_MINOR_BASE          54
#define UMAP_PKE_MINOR_BASE         56
#define UMAP_OTP_MINOR_BASE         57

/* SPACC. */
td_s32 crypto_spacc_init(td_void);
td_void crypto_spacc_deinit(td_void);

/* KM. */
td_s32 crypto_km_init(td_void);
td_void crypto_km_deinit(td_void);

/* PKE. */
td_s32 crypto_pke_init(td_void);
td_void crypto_pke_deinit(td_void);

/* OTP. */
td_s32 crypto_otp_init(td_void);
td_void crypto_otp_deinit(td_void);

#endif